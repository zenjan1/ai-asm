/* air_transport: Airline operations management (v1.0)
 * Fleet, flights, passengers, cargo, loyalty
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

#define MAX_FLEET      14
#define MAX_ROUTES     16
#define MAX_FLIGHTS    18
#define MAX_CARGO      10
#define MAX_MEMBERS    12

typedef struct {
    int    fleet_id;
    int    type;
    int    registration;
    int    capacity;
    int    range_km;
    int    year_built;
    int    status;
    int    flight_hours;
    int    active;
} fleet_t;

typedef struct {
    int    route_id;
    int    from_zone;
    int    to_zone;
    int    distance_km;
    int    frequency;
    int    duration_min;
    int    year_started;
    int    active;
} route_t;

typedef struct {
    int    flight_id;
    int    route_id;
    int    fleet_id;
    int    passengers;
    int    capacity;
    int    revenue;
    int    on_time;
    int    year;
    int    active;
} flight_t;

typedef struct {
    int    cargo_id;
    int    route_id;
    int    weight_kg;
    int    volume_m3;
    int    revenue;
    int    type;
    int    year;
    int    active;
} cargo_t;

typedef struct {
    int    member_id;
    int    tier;
    int    miles;
    int    flights_taken;
    int    points_earned;
    int    points_redeemed;
    int    year_joined;
    int    active;
} member_t;

typedef struct {
    int    n_fleet;
    int    n_routes;
    int    n_flights;
    int    n_cargo;
    int    n_members;
    int    total_passengers;
    int    total_revenue;
    int    total_cargo_kg;
    int    total_miles;
    int    total_on_time;
} at_state_t;

static fleet_t fleet[MAX_FLEET];
static route_t routes[MAX_ROUTES];
static flight_t flights[MAX_FLIGHTS];
static cargo_t cargos[MAX_CARGO];
static member_t members[MAX_MEMBERS];
static at_state_t at;

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

int at_init(void) {
    if (initialized) return -1;
    at.n_fleet = 0; at.n_routes = 0; at.n_flights = 0;
    at.n_cargo = 0; at.n_members = 0;
    at.total_passengers = 0; at.total_revenue = 0;
    at.total_cargo_kg = 0; at.total_miles = 0;
    at.total_on_time = 0;
    for (int i = 0; i < MAX_FLEET; i++) fleet[i].active = 0;
    for (int i = 0; i < MAX_ROUTES; i++) routes[i].active = 0;
    for (int i = 0; i < MAX_FLIGHTS; i++) flights[i].active = 0;
    for (int i = 0; i < MAX_CARGO; i++) cargos[i].active = 0;
    for (int i = 0; i < MAX_MEMBERS; i++) members[i].active = 0;
    initialized = 1;
    print_str("[AT] Air transport initialized\n");
    return 0;
}

int at_add_fleet(int type, int registration, int capacity, int range, int year) {
    if (at.n_fleet >= MAX_FLEET) return -1;
    fleet_t* f = &fleet[at.n_fleet];
    f->fleet_id = at.n_fleet;
    f->type = type;
    f->registration = registration;
    f->capacity = capacity;
    f->range_km = range;
    f->year_built = year;
    f->status = 1;
    f->flight_hours = 0;
    f->active = 1;
    at.n_fleet++;
    print_str("[AT] Fleet "); print_int(at.n_fleet - 1);
    print_str(" type="); print_int(type);
    print_str(" reg="); print_int(registration);
    print_str(" cap="); print_int(capacity);
    print_str(" range="); print_int(range); print_str("km\n");
    return at.n_fleet - 1;
}

int at_add_route(int from, int to, int distance, int frequency, int duration, int year) {
    if (at.n_routes >= MAX_ROUTES) return -1;
    route_t* r = &routes[at.n_routes];
    r->route_id = at.n_routes;
    r->from_zone = from;
    r->to_zone = to;
    r->distance_km = distance;
    r->frequency = frequency;
    r->duration_min = duration;
    r->year_started = year;
    r->active = 1;
    at.n_routes++;
    print_str("[AT] Route "); print_int(at.n_routes - 1);
    print_str(" Z"); print_int(from); print_str("->Z"); print_int(to);
    print_str(" dist="); print_int(distance); print_str("km");
    print_str(" freq="); print_int(frequency);
    print_str(" dur="); print_int(duration); print_str("min\n");
    return at.n_routes - 1;
}

int at_schedule_flight(int route_id, int fleet_id, int passengers, int revenue, int on_time, int year) {
    if (at.n_flights >= MAX_FLIGHTS) return -1;
    flight_t* f = &flights[at.n_flights];
    f->flight_id = at.n_flights;
    f->route_id = route_id;
    f->fleet_id = fleet_id;
    f->passengers = passengers;
    f->capacity = fleet[fleet_id].capacity;
    f->revenue = revenue;
    f->on_time = on_time;
    f->year = year;
    f->active = 1;
    at.total_passengers += passengers;
    at.total_revenue += revenue;
    at.total_on_time += on_time;
    at.n_flights++;
    print_str("[AT] Flight "); print_int(at.n_flights - 1);
    print_str(" rt="); print_int(route_id);
    print_str(" fl="); print_int(fleet_id);
    print_str(" pax="); print_int(passengers);
    print_str(" rev=$"); print_int(revenue);
    print_str(" ot="); print_int(on_time); print_str("%\n");
    return at.n_flights - 1;
}

int at_add_cargo(int route_id, int weight, int volume, int revenue, int type, int year) {
    if (at.n_cargo >= MAX_CARGO) return -1;
    cargo_t* c = &cargos[at.n_cargo];
    c->cargo_id = at.n_cargo;
    c->route_id = route_id;
    c->weight_kg = weight;
    c->volume_m3 = volume;
    c->revenue = revenue;
    c->type = type;
    c->year = year;
    c->active = 1;
    at.total_cargo_kg += weight;
    at.total_revenue += revenue;
    at.n_cargo++;
    print_str("[AT] Cargo "); print_int(at.n_cargo - 1);
    print_str(" rt="); print_int(route_id);
    print_str(" wt="); print_int(weight); print_str("kg");
    print_str(" vol="); print_int(volume); print_str("m3");
    print_str(" rev=$"); print_int(revenue); print_str("\n");
    return at.n_cargo - 1;
}

int at_enroll_member(int tier, int miles, int flights, int year) {
    if (at.n_members >= MAX_MEMBERS) return -1;
    member_t* m = &members[at.n_members];
    m->member_id = at.n_members;
    m->tier = tier;
    m->miles = miles;
    m->flights_taken = flights;
    m->points_earned = miles;
    m->points_redeemed = 0;
    m->year_joined = year;
    m->active = 1;
    at.total_miles += miles;
    at.n_members++;
    print_str("[AT] Member "); print_int(at.n_members - 1);
    print_str(" tier="); print_int(tier);
    print_str(" miles="); print_int(miles);
    print_str(" fl="); print_int(flights);
    print_str(" "); print_int(year); print_str("\n");
    return at.n_members - 1;
}

int at_redeem_points(int member_id, int points) {
    if (member_id >= at.n_members) return -1;
    members[member_id].points_redeemed += points;
    print_str("[AT] Redeem M"); print_int(member_id);
    print_str(" pts="); print_int(points); print_str("\n");
    return 0;
}

void at_fleet_report(void) {
    print_str("[AT] Fleet report:\n");
    print_str("  Aircraft: "); print_int(at.n_fleet); print_str("\n");
    print_str("  Routes: "); print_int(at.n_routes); print_str("\n");
}

void at_traffic_report(void) {
    print_str("[AT] Traffic report:\n");
    print_str("  Flights: "); print_int(at.n_flights); print_str("\n");
    print_str("  Passengers: "); print_int(at.total_passengers); print_str("\n");
    int avg_ot = at.n_flights > 0 ? at.total_on_time / at.n_flights : 0;
    print_str("  Avg on-time: "); print_int(avg_ot); print_str("%\n");
}

void at_cargo_report(void) {
    print_str("[AT] Cargo report:\n");
    print_str("  Cargo: "); print_int(at.total_cargo_kg); print_str(" kg\n");
    print_str("  Revenue: $"); print_int(at.total_revenue); print_str("\n");
}

void at_loyalty_report(void) {
    print_str("[AT] Loyalty report:\n");
    print_str("  Members: "); print_int(at.n_members); print_str("\n");
    print_str("  Total miles: "); print_int(at.total_miles); print_str("\n");
}

void at_print_state(void) {
    print_str("[AT] Fl="); print_int(at.n_fleet);
    print_str(" Rt="); print_int(at.n_routes);
    print_str(" Fg="); print_int(at.n_flights);
    print_str(" Cg="); print_int(at.n_cargo);
    print_str(" Mb="); print_int(at.n_members);
    print_str("\n");
}

int main(void) {
    print_str("=== Air Transport Demo ===\n\n");
    at_init();

    print_str("Adding fleet...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int reg = 2000 + i;
        int cap = 150 + (i * 20);
        int range = 3000 + (i * 500);
        int year = 2010 + (i % 12);
        at_add_fleet(type, reg, cap, range, year);
    }

    print_str("\nAdding routes...\n");
    for (int i = 0; i < 16; i++) {
        int from = (i % 4) + 1;
        int to = ((i + 2) % 4) + 1;
        int dist = 500 + (i * 200);
        int freq = 1 + (i % 4);
        int dur = 60 + (i * 30);
        int year = 2015 + (i % 8);
        at_add_route(from, to, dist, freq, dur, year);
    }

    print_str("\nScheduling flights...\n");
    for (int i = 0; i < 18; i++) {
        int rt = i % 16;
        int fl = i % 14;
        int pax = 100 + (i * 10);
        int rev = 10000 + (i * 2000);
        int ot = 80 + (i % 20);
        int year = 2024;
        at_schedule_flight(rt, fl, pax, rev, ot, year);
    }

    print_str("\nAdding cargo...\n");
    for (int i = 0; i < 10; i++) {
        int rt = i % 16;
        int wt = 5000 + (i * 2000);
        int vol = 20 + (i * 10);
        int rev = 20000 + (i * 5000);
        int type = (i % 4) + 1;
        int year = 2024;
        at_add_cargo(rt, wt, vol, rev, type, year);
    }

    print_str("\nEnrolling members...\n");
    for (int i = 0; i < 12; i++) {
        int tier = (i % 4) + 1;
        int miles = 10000 + (i * 5000);
        int fl = 5 + (i * 2);
        int year = 2018 + (i % 6);
        at_enroll_member(tier, miles, fl, year);
    }

    print_str("\nRedeeming points...\n");
    for (int i = 0; i < 8; i++) {
        int pts = 2000 + (i * 1000);
        at_redeem_points(i, pts);
    }

    print_str("\nFleet report...\n");
    at_fleet_report();

    print_str("\nTraffic report...\n");
    at_traffic_report();

    print_str("\nCargo report...\n");
    at_cargo_report();

    print_str("\nLoyalty report...\n");
    at_loyalty_report();

    print_str("\nFinal state...\n");
    at_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
