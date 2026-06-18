/* autonomous_shipping: Autonomous shipping system (v1.0)
 * Vessel control, cargo management, port operations, maritime weather, fleet management
 */
#include <stddef.h>

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_VESSELS    8
#define MAX_CONTAINERS 16
#define MAX_BERTHS     4
#define MAX_ROUTES     8

typedef struct {
    int    vessel_id;
    double x, y;
    double heading;
    double speed;
    double fuel_level;
    double cargo_capacity;
    double cargo_loaded;
    int    status;
    int    autopilot;
    int    active;
} vessel_t;

typedef struct {
    int    container_id;
    int    vessel_id;
    double weight;
    int    type;
    double temp_required;
    double temp_current;
    int    hazardous;
    int    active;
} container_t;

typedef struct {
    int    berth_id;
    int    occupied;
    int    vessel_id;
    double max_length;
    double max_draft;
    double crane_capacity;
    int    active;
} berth_t;

typedef struct {
    int    route_id;
    double distance;
    double estimated_time;
    double fuel_consumption;
    double weather_risk;
    int    waypoints;
    int    active;
} route_t;

typedef struct {
    int    n_vessels;
    int    n_containers;
    int    n_berths;
    int    n_routes;
    double total_cargo;
    double total_fuel;
    int    port_operations;
    int    weather_alerts;
} as_state_t;

static vessel_t vessels[MAX_VESSELS];
static container_t containers[MAX_CONTAINERS];
static berth_t berths[MAX_BERTHS];
static route_t routes[MAX_ROUTES];
static as_state_t as;

static int initialized = 0;

static void print_str(const char* s) { host_print(s); }
static void print_int(int val) {
    char buf[32]; int i = 0;
    if (val < 0) { buf[i++] = '-'; val = -val; }
    if (val == 0) { buf[i++] = '0'; }
    else { int s = i; while (val > 0) { buf[i++] = '0' + (val % 10); val /= 10; }
           int e = i - 1; while (s < e) { char t = buf[s]; buf[s] = buf[e]; buf[e] = t; s++; e--; } }
    buf[i] = '\0'; host_print(buf);
}
static double dabs(double x) { return x < 0 ? -x : x; }
static double dsqrt(double x) {
    if (x <= 0.0) return 0.0;
    double g = x, p = x / g;
    for (int i = 0; i < 30; i++) { g = (g + p) * 0.5; p = x / g; }
    return g;
}

int as_init(void) {
    if (initialized) return -1;
    as.n_vessels = 0; as.n_containers = 0;
    as.n_berths = 0; as.n_routes = 0;
    as.total_cargo = 0.0; as.total_fuel = 0.0;
    as.port_operations = 0; as.weather_alerts = 0;
    for (int i = 0; i < MAX_VESSELS; i++) vessels[i].active = 0;
    for (int i = 0; i < MAX_CONTAINERS; i++) containers[i].active = 0;
    for (int i = 0; i < MAX_BERTHS; i++) berths[i].active = 0;
    for (int i = 0; i < MAX_ROUTES; i++) routes[i].active = 0;
    initialized = 1;
    print_str("[AS] Autonomous shipping initialized\n");
    return 0;
}

int as_add_vessel(double capacity, double x, double y) {
    if (as.n_vessels >= MAX_VESSELS) return -1;
    vessel_t* v = &vessels[as.n_vessels];
    v->vessel_id = as.n_vessels;
    v->x = x; v->y = y;
    v->heading = 0.0; v->speed = 0.0;
    v->fuel_level = 1000.0; v->cargo_capacity = capacity;
    v->cargo_loaded = 0.0; v->status = 1;
    v->autopilot = 0; v->active = 1;
    as.n_vessels++;
    print_str("[AS] Vessel "); print_int(as.n_vessels - 1);
    print_str(" capacity="); print_int((int)capacity); print_str("TEU");
    print_str(" pos=("); print_int((int)x); print_str(","); print_int((int)y); print_str(")\n");
    return as.n_vessels - 1;
}

void as_navigate_to(int vessel_id, double target_x, double target_y) {
    if (vessel_id >= as.n_vessels) return;
    vessel_t* v = &vessels[vessel_id];
    double dx = target_x - v->x;
    double dy = target_y - v->y;
    double distance = dsqrt(dx * dx + dy * dy);
    v->heading = 90.0;
    if (dx < 0) v->heading = 270.0;
    if (dabs(dy) > dabs(dx)) v->heading = (dy > 0) ? 0.0 : 180.0;
    v->speed = 15.0;
    v->autopilot = 1;
    double time_hours = distance / v->speed;
    double fuel_used = time_hours * 2.0;
    v->fuel_level -= fuel_used;
    v->x = target_x; v->y = target_y;
    print_str("[AS] Vessel "); print_int(vessel_id);
    print_str(" navigating to ("); print_int((int)target_x); print_str(","); print_int((int)target_y); print_str(")");
    print_str(" dist="); print_int((int)distance); print_str("nm");
    print_str(" ETA="); print_int((int)time_hours); print_str("h\n");
}

int as_add_container(int vessel_id, double weight, int type, double temp_req) {
    if (vessel_id >= as.n_vessels) return -1;
    if (as.n_containers >= MAX_CONTAINERS) return -1;
    container_t* c = &containers[as.n_containers];
    c->container_id = as.n_containers;
    c->vessel_id = vessel_id;
    c->weight = weight; c->type = type;
    c->temp_required = temp_req;
    c->temp_current = temp_req;
    c->hazardous = (type == 3) ? 1 : 0;
    c->active = 1;
    vessels[vessel_id].cargo_loaded += 1.0;
    as.n_containers++;
    as.total_cargo += weight;
    print_str("[AS] Container "); print_int(as.n_containers - 1);
    print_str(" vessel="); print_int(vessel_id);
    print_str(" weight="); print_int((int)weight); print_str("t");
    print_str(" type="); print_int(type);
    print_str(" temp="); print_int((int)temp_req); print_str("C");
    print_str(" haz="); print_int(c->hazardous); print_str("\n");
    return as.n_containers - 1;
}

void as_collision_avoidance(int vessel_id) {
    if (vessel_id >= as.n_vessels) return;
    vessel_t* v = &vessels[vessel_id];
    print_str("[AS] Collision avoidance vessel "); print_int(vessel_id); print_str(":\n");
    int threats = 0;
    for (int i = 0; i < as.n_vessels; i++) {
        if (i == vessel_id) continue;
        double dx = vessels[i].x - v->x;
        double dy = vessels[i].y - v->y;
        double dist = dsqrt(dx * dx + dy * dy);
        if (dist < 5.0) {
            print_str("  THREAT: Vessel "); print_int(i);
            print_str(" dist="); print_int((int)(dist * 1000)); print_str("m\n");
            threats++;
        }
    }
    if (threats == 0) print_str("  No threats detected\n");
    else print_str("  Total threats: "); print_int(threats); print_str("\n");
}

int as_add_berth(double max_length, double max_draft) {
    if (as.n_berths >= MAX_BERTHS) return -1;
    berth_t* b = &berths[as.n_berths];
    b->berth_id = as.n_berths;
    b->occupied = 0; b->vessel_id = -1;
    b->max_length = max_length; b->max_draft = max_draft;
    b->crane_capacity = 40.0;
    b->active = 1;
    as.n_berths++;
    print_str("[AS] Berth "); print_int(as.n_berths - 1);
    print_str(" max_L="); print_int((int)max_length); print_str("m");
    print_str(" max_D="); print_int((int)max_draft); print_str("m");
    print_str(" crane="); print_int((int)b->crane_capacity); print_str("t\n");
    return as.n_berths - 1;
}

void as_berth_allocation(int vessel_id) {
    if (vessel_id >= as.n_vessels) return;
    int allocated = -1;
    for (int i = 0; i < as.n_berths; i++) {
        if (!berths[i].occupied) {
            berths[i].occupied = 1;
            berths[i].vessel_id = vessel_id;
            allocated = i;
            as.port_operations++;
            break;
        }
    }
    print_str("[AS] Berth allocation vessel "); print_int(vessel_id);
    if (allocated >= 0) { print_str(" -> berth "); print_int(allocated); print_str("\n"); }
    else { print_str(" FAILED - no available berths\n"); }
}

int as_plan_route(double distance, double fuel_consumption) {
    if (as.n_routes >= MAX_ROUTES) return -1;
    route_t* r = &routes[as.n_routes];
    r->route_id = as.n_routes;
    r->distance = distance;
    r->estimated_time = distance / 15.0;
    r->fuel_consumption = fuel_consumption;
    r->weather_risk = 0.2 + (double)(as.n_routes % 4) * 0.15;
    r->waypoints = 3 + as.n_routes % 3;
    r->active = 1;
    as.n_routes++;
    print_str("[AS] Route "); print_int(as.n_routes - 1);
    print_str(" dist="); print_int((int)distance); print_str("nm");
    print_str(" ETA="); print_int((int)r->estimated_time); print_str("h");
    print_str(" fuel="); print_int((int)fuel_consumption); print_str("t");
    print_str(" risk="); print_int((int)(r->weather_risk * 100)); print_str("%\n");
    return as.n_routes - 1;
}

void as_weather_monitoring(void) {
    print_str("[AS] Maritime weather monitoring:\n");
    for (int i = 0; i < as.n_routes; i++) {
        if (routes[i].weather_risk > 0.4) {
            print_str("  ALERT: Route "); print_int(i);
            print_str(" weather_risk="); print_int((int)(routes[i].weather_risk * 100)); print_str("%\n");
            as.weather_alerts++;
        }
    }
    if (as.weather_alerts == 0) print_str("  All routes clear\n");
}

void as_fleet_status(void) {
    print_str("[AS] Fleet status:\n");
    for (int i = 0; i < as.n_vessels; i++) {
        vessel_t* v = &vessels[i];
        print_str("  Vessel "); print_int(i);
        print_str(" cargo="); print_int((int)v->cargo_loaded); print_str("/"); print_int((int)v->cargo_capacity);
        print_str(" fuel="); print_int((int)v->fuel_level); print_str("t");
        print_str(" auto="); print_int(v->autopilot); print_str("\n");
        as.total_fuel += v->fuel_level;
    }
}

void as_print_state(void) {
    print_str("[AS] Vessels="); print_int(as.n_vessels);
    print_str(" Containers="); print_int(as.n_containers);
    print_str(" Berths="); print_int(as.n_berths);
    print_str(" Routes="); print_int(as.n_routes);
    print_str(" Cargo="); print_int((int)as.total_cargo); print_str("t");
    print_str(" Alerts="); print_int(as.weather_alerts); print_str("\n");
}

int main(void) {
    print_str("=== Autonomous Shipping System Demo ===\n\n");
    as_init();

    print_str("Adding vessels...\n");
    as_add_vessel(500.0, 0.0, 0.0);
    as_add_vessel(300.0, 10.0, 5.0);
    as_add_vessel(800.0, 20.0, 0.0);
    as_add_vessel(400.0, 10.0, 10.0);

    print_str("\nNavigation...\n");
    as_navigate_to(0, 50.0, 30.0);
    as_navigate_to(1, 40.0, 20.0);
    as_navigate_to(2, 60.0, 40.0);

    print_str("\nCollision avoidance...\n");
    as_collision_avoidance(0);
    as_collision_avoidance(1);

    print_str("\nLoading containers...\n");
    as_add_container(0, 25.0, 1, 5.0);
    as_add_container(0, 30.0, 2, -18.0);
    as_add_container(0, 20.0, 3, 20.0);
    as_add_container(1, 15.0, 1, 10.0);
    as_add_container(1, 22.0, 2, -20.0);
    as_add_container(2, 35.0, 1, 8.0);
    as_add_container(2, 28.0, 3, 15.0);

    print_str("\nAdding berths...\n");
    as_add_berth(300.0, 12.0);
    as_add_berth(250.0, 10.0);
    as_add_berth(400.0, 15.0);

    print_str("\nBerth allocation...\n");
    as_berth_allocation(0);
    as_berth_allocation(1);
    as_berth_allocation(2);
    as_berth_allocation(3);

    print_str("\nRoute planning...\n");
    as_plan_route(500.0, 100.0);
    as_plan_route(300.0, 60.0);
    as_plan_route(800.0, 160.0);
    as_plan_route(400.0, 80.0);

    print_str("\nWeather monitoring...\n");
    as_weather_monitoring();

    print_str("\nFleet status...\n");
    as_fleet_status();

    as_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
