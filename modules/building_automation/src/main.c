/* building_automation: Building automation system (v1.0)
 * HVAC, lighting, elevator, security, energy management
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

#define MAX_FLOORS   8
#define MAX_ROOMS    16
#define MAX_HVAC     8
#define MAX_LIGHTS   16
#define MAX_ACCESS   8
#define MAX_CAMERAS  8

#define HVAC_COOL    0x01
#define HVAC_HEAT    0x02
#define HVAC_AUTO    0x03
#define HVAC_FAN     0x04

#define LIGHT_MANUAL 0x01
#define LIGHT_OCCUP  0x02
#define LIGHT_DAYLT  0x03
#define LIGHT_SCENE  0x04

#define ELEV_IDLE    0x01
#define ELEV_UP      0x02
#define ELEV_DOWN    0x03
#define ELEV_MAINT   0x04

#define SEC_ARMED    0x01
#define SEC_DISARM   0x02
#define SEC_ALARM    0x03

typedef struct {
    double temp_set;
    double temp_curr;
    double humidity;
    double airflow;
    double power;
    int    mode;
    int    zone;
    int    id;
    int    active;
} hvac_t;

typedef struct {
    double level;
    double dimming;
    double daylight;
    double occupancy;
    double power;
    int    mode;
    int    room_id;
    int    id;
    int    active;
} light_t;

typedef struct {
    int    current_floor;
    int    target_floor;
    int    direction;
    int    load;
    int    trips;
    double energy;
    double health;
    int    id;
    int    active;
} elevator_t;

typedef struct {
    int    zone;
    int    status;
    int    auth_count;
    int    deny_count;
    int    alarm;
    int    id;
    int    active;
} access_t;

typedef struct {
    int    zone;
    int    recording;
    int    motion;
    double storage_used;
    int    id;
    int    active;
} camera_t;

typedef struct {
    int    n_hvac;
    int    n_lights;
    int    n_elevators;
    int    n_access;
    int    n_cameras;
    double total_power;
    double total_energy;
    double pue;
    int    alarm_count;
    int    sec_status;
} ba_state_t;

static hvac_t hvacs[MAX_HVAC];
static light_t lights[MAX_LIGHTS];
static elevator_t elevators[MAX_FLOORS];
static access_t access_points[MAX_ACCESS];
static camera_t cameras[MAX_CAMERAS];
static ba_state_t ba;

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

int ba_init(void) {
    if (initialized) return -1;
    ba.n_hvac = 0; ba.n_lights = 0; ba.n_elevators = 0;
    ba.n_access = 0; ba.n_cameras = 0;
    ba.total_power = 0.0; ba.total_energy = 0.0;
    ba.pue = 1.4; ba.alarm_count = 0; ba.sec_status = SEC_DISARM;
    for (int i = 0; i < MAX_HVAC; i++) hvacs[i].active = 0;
    for (int i = 0; i < MAX_LIGHTS; i++) lights[i].active = 0;
    for (int i = 0; i < MAX_FLOORS; i++) elevators[i].active = 0;
    for (int i = 0; i < MAX_ACCESS; i++) access_points[i].active = 0;
    for (int i = 0; i < MAX_CAMERAS; i++) cameras[i].active = 0;
    initialized = 1;
    print_str("[BA] Building automation initialized\n");
    return 0;
}

int ba_add_hvac(int mode, double temp_set, int zone) {
    if (ba.n_hvac >= MAX_HVAC) return -1;
    hvac_t* h = &hvacs[ba.n_hvac];
    h->mode = mode; h->temp_set = temp_set;
    h->temp_curr = 22.0 + (double)(zone % 3);
    h->humidity = 50.0; h->airflow = 500.0;
    h->power = 2.0 + dabs(h->temp_curr - h->temp_set) * 0.5;
    h->zone = zone; h->id = ba.n_hvac; h->active = 1;
    ba.n_hvac++;
    print_str("[BA] HVAC "); print_int(ba.n_hvac - 1);
    print_str(" mode="); print_int(mode);
    print_str(" set="); print_int((int)temp_set); print_str("C");
    print_str(" curr="); print_int((int)h->temp_curr); print_str("C");
    print_str(" P="); print_int((int)(h->power * 10)); print_str("kW\n");
    return ba.n_hvac - 1;
}

void ba_hvac_control(int zone, double ambient) {
    double total_power = 0.0;
    for (int i = 0; i < ba.n_hvac; i++) {
        if (hvacs[i].zone != zone) continue;
        hvac_t* h = &hvacs[i];
        double error = h->temp_set - ambient;
        if (error > 2.0) h->mode = HVAC_HEAT;
        else if (error < -2.0) h->mode = HVAC_COOL;
        h->temp_curr = ambient + (h->mode == HVAC_HEAT ? 2.0 : (h->mode == HVAC_COOL ? -2.0 : 0.0));
        h->power = 1.5 + dabs(error) * 0.3;
        total_power += h->power;
        print_str("[BA] HVAC "); print_int(h->id);
        print_str(" mode="); print_int(h->mode);
        print_str(" curr="); print_int((int)h->temp_curr); print_str("C\n");
    }
    print_str("[BA] Zone "); print_int(zone);
    print_str(" HVAC power: "); print_int((int)(total_power * 10)); print_str("kW\n");
}

int ba_add_light(int mode, int room_id) {
    if (ba.n_lights >= MAX_LIGHTS) return -1;
    light_t* l = &lights[ba.n_lights];
    l->mode = mode; l->room_id = room_id;
    l->level = 100.0; l->dimming = 100.0;
    l->daylight = 30.0; l->occupancy = 1.0;
    l->power = 0.05;
    if (mode == LIGHT_DAYLT) { l->level = 100.0 - l->daylight; l->dimming = l->level; }
    if (mode == LIGHT_OCCUP) { l->occupancy = 0.0; l->level = 0.0; }
    l->id = ba.n_lights; l->active = 1;
    ba.n_lights++;
    print_str("[BA] Light "); print_int(ba.n_lights - 1);
    print_str(" mode="); print_int(mode);
    print_str(" room="); print_int(room_id);
    print_str(" lvl="); print_int((int)l->level); print_str("%\n");
    return ba.n_lights - 1;
}

void ba_lighting_scene(int scene_id) {
    print_str("[BA] Lighting scene "); print_int(scene_id); print_str(":\n");
    for (int i = 0; i < ba.n_lights; i++) {
        lights[i].mode = LIGHT_SCENE;
        if (scene_id == 1) { lights[i].level = 100.0; lights[i].dimming = 100.0; }
        else if (scene_id == 2) { lights[i].level = 50.0; lights[i].dimming = 50.0; }
        else if (scene_id == 3) { lights[i].level = 20.0; lights[i].dimming = 20.0; }
        else { lights[i].level = 0.0; lights[i].dimming = 0.0; }
        print_str("  Light "); print_int(i);
        print_str(" lvl="); print_int((int)lights[i].level); print_str("%\n");
    }
}

int ba_add_elevator(int n_floors) {
    if (ba.n_elevators >= MAX_FLOORS) return -1;
    elevator_t* e = &elevators[ba.n_elevators];
    e->current_floor = 0; e->target_floor = 0;
    e->direction = ELEV_IDLE; e->load = 0;
    e->trips = 0; e->energy = 0.0; e->health = 95.0;
    e->id = ba.n_elevators; e->active = 1;
    ba.n_elevators++;
    print_str("[BA] Elevator "); print_int(ba.n_elevators - 1);
    print_str(" floors="); print_int(n_floors);
    print_str(" health="); print_int((int)e->health); print_str("%\n");
    return ba.n_elevators - 1;
}

void ba_elevator_dispatch(int target, int load) {
    int best = -1; double best_dist = 1e10;
    for (int i = 0; i < ba.n_elevators; i++) {
        double dist = dabs((double)(elevators[i].current_floor - target));
        if (elevators[i].direction == ELEV_IDLE && dist < best_dist) {
            best_dist = dist; best = i;
        }
    }
    if (best < 0) best = 0;
    elevator_t* e = &elevators[best];
    e->target_floor = target; e->load = load;
    e->direction = target > e->current_floor ? ELEV_UP : ELEV_DOWN;
    double trips = dabs((double)(target - e->current_floor));
    e->energy += trips * 0.5;
    e->trips++;
    e->current_floor = target;
    e->direction = ELEV_IDLE;
    e->health -= 0.1;
    print_str("[BA] Elev "); print_int(best);
    print_str(" -> F"); print_int(target);
    print_str(" load="); print_int(load);
    print_str(" trips="); print_int(e->trips);
    print_str(" E="); print_int((int)(e->energy * 10)); print_str("kWh\n");
}

int ba_add_access(int zone) {
    if (ba.n_access >= MAX_ACCESS) return -1;
    access_t* a = &access_points[ba.n_access];
    a->zone = zone; a->status = SEC_DISARM;
    a->auth_count = 0; a->deny_count = 0;
    a->alarm = 0; a->id = ba.n_access; a->active = 1;
    ba.n_access++;
    print_str("[BA] Access "); print_int(ba.n_access - 1);
    print_str(" zone="); print_int(zone); print_str("\n");
    return ba.n_access - 1;
}

int ba_add_camera(int zone) {
    if (ba.n_cameras >= MAX_CAMERAS) return -1;
    camera_t* c = &cameras[ba.n_cameras];
    c->zone = zone; c->recording = 1;
    c->motion = 0; c->storage_used = 10.0;
    c->id = ba.n_cameras; c->active = 1;
    ba.n_cameras++;
    print_str("[BA] Camera "); print_int(ba.n_cameras - 1);
    print_str(" zone="); print_int(zone); print_str(" REC\n");
    return ba.n_cameras - 1;
}

void ba_security_arm(int status) {
    ba.sec_status = status;
    for (int i = 0; i < ba.n_access; i++) access_points[i].status = status;
    print_str("[BA] Security: status="); print_int(status);
    if (status == SEC_ARMED) print_str(" ARMED\n");
    else if (status == SEC_ALARM) { print_str(" ALARM\n"); ba.alarm_count++; }
    else print_str(" DISARMED\n");
}

void ba_energy_management(void) {
    double total_power = 0.0;
    for (int i = 0; i < ba.n_hvac; i++) total_power += hvacs[i].power;
    for (int i = 0; i < ba.n_lights; i++) total_power += lights[i].power;
    for (int i = 0; i < ba.n_elevators; i++) total_power += elevators[i].energy * 0.01;
    ba.total_power = total_power;
    ba.total_energy = total_power * 8.0;
    double it_power = 50.0;
    ba.pue = (total_power + it_power) / (it_power + 1e-10);
    if (ba.pue < 1.0) ba.pue = 1.0;
    print_str("[BA] Energy: total="); print_int((int)(total_power * 10)); print_str("kW");
    print_str(" daily="); print_int((int)(ba.total_energy * 10)); print_str("kWh");
    print_str(" PUE="); print_int((int)(ba.pue * 100)); print_str("/100\n");
}

void ba_print_state(void) {
    print_str("[BA] HVAC="); print_int(ba.n_hvac);
    print_str(" Light="); print_int(ba.n_lights);
    print_str(" Elev="); print_int(ba.n_elevators);
    print_str(" Access="); print_int(ba.n_access);
    print_str(" Cam="); print_int(ba.n_cameras);
    print_str(" Sec="); print_int(ba.sec_status);
    print_str(" Alarm="); print_int(ba.alarm_count); print_str("\n");
}

int main(void) {
    print_str("=== Building Automation System Demo ===\n\n");
    ba_init();

    print_str("Adding HVAC...\n");
    ba_add_hvac(HVAC_AUTO, 22.0, 0);
    ba_add_hvac(HVAC_AUTO, 23.0, 1);
    ba_add_hvac(HVAC_COOL, 20.0, 2);
    ba_add_hvac(HVAC_HEAT, 24.0, 0);

    print_str("\nHVAC control...\n");
    ba_hvac_control(0, 25.0);
    ba_hvac_control(1, 18.0);
    ba_hvac_control(2, 28.0);

    print_str("\nAdding lights...\n");
    ba_add_light(LIGHT_MANUAL, 0);
    ba_add_light(LIGHT_OCCUP, 1);
    ba_add_light(LIGHT_DAYLT, 2);
    ba_add_light(LIGHT_SCENE, 3);
    ba_add_light(LIGHT_MANUAL, 4);
    ba_add_light(LIGHT_DAYLT, 5);

    print_str("\nLighting scenes...\n");
    ba_lighting_scene(1);
    ba_lighting_scene(2);
    ba_lighting_scene(3);

    print_str("\nAdding elevators...\n");
    ba_add_elevator(10);
    ba_add_elevator(10);
    ba_add_elevator(8);

    print_str("\nElevator dispatch...\n");
    ba_elevator_dispatch(5, 4);
    ba_elevator_dispatch(8, 2);
    ba_elevator_dispatch(3, 6);
    ba_elevator_dispatch(7, 1);
    ba_elevator_dispatch(0, 3);

    print_str("\nSecurity system...\n");
    ba_add_access(0);
    ba_add_access(1);
    ba_add_access(2);
    ba_add_camera(0);
    ba_add_camera(1);
    ba_add_camera(2);
    ba_security_arm(SEC_ARMED);
    ba_security_arm(SEC_ALARM);
    ba_security_arm(SEC_DISARM);

    print_str("\nEnergy management...\n");
    ba_energy_management();

    ba_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
