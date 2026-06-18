/* air_traffic_control: Air traffic control system (v1.0)
 * Flight tracking, airspace management, landing scheduling, conflict detection, weather
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

#define MAX_FLIGHTS    12
#define MAX_RUNWAYS    3
#define MAX_SECTORS    8
#define MAX_CONFLICTS  8

#define FLIGHT_ENROUTE 0x01
#define FLIGHT_DESCENT 0x02
#define FLIGHT_APPROACH 0x03
#define FLIGHT_LANDING 0x04
#define FLIGHT_GROUND  0x05

typedef struct {
    int    flight_id;
    double x, y, z;
    double heading;
    double speed;
    double vertical_speed;
    int    status;
    int    assigned_runway;
    int    squawk;
    int    active;
} flight_t;

typedef struct {
    int    runway_id;
    int    available;
    double length;
    int    current_flight;
    int    landings_today;
    int    takeoffs_today;
    int    active;
} runway_t;

typedef struct {
    int    sector_id;
    int    flight_count;
    double max_capacity;
    double x_min, x_max;
    double y_min, y_max;
    int    active;
} sector_t;

typedef struct {
    int    flight1;
    int    flight2;
    double distance;
    double time_to_conflict;
    int    severity;
    int    active;
} conflict_t;

typedef struct {
    int    n_flights;
    int    n_runways;
    int    n_sectors;
    int    n_conflicts;
    int    total_landings;
    int    total_takeoffs;
    int    conflict_alerts;
    int    weather_alerts;
} atc_state_t;

static flight_t flights[MAX_FLIGHTS];
static runway_t runways[MAX_RUNWAYS];
static sector_t sectors[MAX_SECTORS];
static conflict_t conflicts[MAX_CONFLICTS];
static atc_state_t atc;

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

int atc_init(void) {
    if (initialized) return -1;
    atc.n_flights = 0; atc.n_runways = 0;
    atc.n_sectors = 0; atc.n_conflicts = 0;
    atc.total_landings = 0; atc.total_takeoffs = 0;
    atc.conflict_alerts = 0; atc.weather_alerts = 0;
    for (int i = 0; i < MAX_FLIGHTS; i++) flights[i].active = 0;
    for (int i = 0; i < MAX_RUNWAYS; i++) runways[i].active = 0;
    for (int i = 0; i < MAX_SECTORS; i++) sectors[i].active = 0;
    for (int i = 0; i < MAX_CONFLICTS; i++) conflicts[i].active = 0;
    initialized = 1;
    print_str("[ATC] Air traffic control initialized\n");
    return 0;
}

int atc_add_flight(double x, double y, double z, double heading) {
    if (atc.n_flights >= MAX_FLIGHTS) return -1;
    flight_t* f = &flights[atc.n_flights];
    f->flight_id = atc.n_flights;
    f->x = x; f->y = y; f->z = z;
    f->heading = heading;
    f->speed = 250.0 + (double)(atc.n_flights % 4) * 20.0;
    f->vertical_speed = 0.0;
    f->status = FLIGHT_ENROUTE;
    f->assigned_runway = -1;
    f->squawk = 1000 + atc.n_flights * 100;
    f->active = 1;
    atc.n_flights++;
    print_str("[ATC] Flight "); print_int(atc.n_flights - 1);
    print_str(" pos=("); print_int((int)x); print_str(","); print_int((int)y); print_str(","); print_int((int)z); print_str(")");
    print_str(" hdg="); print_int((int)heading);
    print_str(" spd="); print_int((int)f->speed); print_str("kt");
    print_str(" sq="); print_int(f->squawk); print_str("\n");
    return atc.n_flights - 1;
}

int atc_add_runway(double length) {
    if (atc.n_runways >= MAX_RUNWAYS) return -1;
    runway_t* r = &runways[atc.n_runways];
    r->runway_id = atc.n_runways;
    r->available = 1; r->length = length;
    r->current_flight = -1;
    r->landings_today = 0; r->takeoffs_today = 0;
    r->active = 1;
    atc.n_runways++;
    print_str("[ATC] Runway "); print_int(atc.n_runways - 1);
    print_str(" length="); print_int((int)length); print_str("m\n");
    return atc.n_runways - 1;
}

int atc_add_sector(double x_min, double x_max, double y_min, double y_max) {
    if (atc.n_sectors >= MAX_SECTORS) return -1;
    sector_t* s = &sectors[atc.n_sectors];
    s->sector_id = atc.n_sectors;
    s->flight_count = 0;
    s->max_capacity = 6.0;
    s->x_min = x_min; s->x_max = x_max;
    s->y_min = y_min; s->y_max = y_max;
    s->active = 1;
    atc.n_sectors++;
    print_str("[ATC] Sector "); print_int(atc.n_sectors - 1);
    print_str(" bounds=("); print_int((int)x_min); print_str(","); print_int((int)y_min);
    print_str(")-("); print_int((int)x_max); print_str(","); print_int((int)y_max); print_str(")");
    print_str(" cap="); print_int((int)s->max_capacity); print_str("\n");
    return atc.n_sectors - 1;
}

void atc_sector_management(void) {
    for (int i = 0; i < atc.n_sectors; i++) sectors[i].flight_count = 0;
    for (int i = 0; i < atc.n_flights; i++) {
        for (int j = 0; j < atc.n_sectors; j++) {
            if (flights[i].x >= sectors[j].x_min && flights[i].x <= sectors[j].x_max &&
                flights[i].y >= sectors[j].y_min && flights[i].y <= sectors[j].y_max) {
                sectors[j].flight_count++;
            }
        }
    }
    print_str("[ATC] Sector status:\n");
    for (int i = 0; i < atc.n_sectors; i++) {
        print_str("  Sector "); print_int(i);
        print_str(" flights="); print_int(sectors[i].flight_count);
        print_str("/"); print_int((int)sectors[i].max_capacity);
        if (sectors[i].flight_count >= (int)sectors[i].max_capacity) print_str(" FULL");
        print_str("\n");
    }
}

void atc_landing_sequence(int flight_id, int runway_id) {
    if (flight_id >= atc.n_flights || runway_id >= atc.n_runways) return;
    flight_t* f = &flights[flight_id];
    runway_t* r = &runways[runway_id];
    if (!r->available) {
        print_str("[ATC] Runway "); print_int(runway_id);
        print_str(" not available\n");
        return;
    }
    f->assigned_runway = runway_id;
    f->status = FLIGHT_APPROACH;
    f->vertical_speed = -500.0;
    f->speed = 180.0;
    r->available = 0;
    r->current_flight = flight_id;
    print_str("[ATC] Flight "); print_int(flight_id);
    print_str(" cleared approach runway "); print_int(runway_id);
    print_str(" sq="); print_int(f->squawk); print_str("\n");
}

void atc_landing_complete(int flight_id) {
    if (flight_id >= atc.n_flights) return;
    flight_t* f = &flights[flight_id];
    if (f->assigned_runway >= 0) {
        runway_t* r = &runways[f->assigned_runway];
        r->available = 1;
        r->current_flight = -1;
        r->landings_today++;
        atc.total_landings++;
    }
    f->status = FLIGHT_GROUND;
    f->z = 0.0; f->speed = 0.0;
    f->vertical_speed = 0.0;
    print_str("[ATC] Flight "); print_int(flight_id);
    print_str(" landed runway "); print_int(f->assigned_runway); print_str("\n");
}

void atc_takeoff(int flight_id, int runway_id) {
    if (flight_id >= atc.n_flights || runway_id >= atc.n_runways) return;
    runway_t* r = &runways[runway_id];
    if (!r->available) return;
    flight_t* f = &flights[flight_id];
    r->available = 0;
    f->assigned_runway = runway_id;
    f->status = FLIGHT_ENROUTE;
    f->z = 500.0; f->speed = 250.0;
    f->vertical_speed = 1000.0;
    r->takeoffs_today++;
    atc.total_takeoffs++;
    print_str("[ATC] Flight "); print_int(flight_id);
    print_str(" takeoff runway "); print_int(runway_id);
    print_str(" sq="); print_int(f->squawk); print_str("\n");
    r->available = 1;
}

void atc_conflict_detection(void) {
    atc.n_conflicts = 0;
    for (int i = 0; i < atc.n_flights; i++) {
        for (int j = i + 1; j < atc.n_flights; j++) {
            double dx = flights[i].x - flights[j].x;
            double dy = flights[i].y - flights[j].y;
            double dz = flights[i].z - flights[j].z;
            double dist = dsqrt(dx * dx + dy * dy + dz * dz);
            if (dist < 5.0 && atc.n_conflicts < MAX_CONFLICTS) {
                conflict_t* c = &conflicts[atc.n_conflicts];
                c->flight1 = i; c->flight2 = j;
                c->distance = dist;
                c->time_to_conflict = dist / 5.0;
                c->severity = dist < 3.0 ? 2 : 1;
                c->active = 1;
                atc.n_conflicts++;
                atc.conflict_alerts++;
            }
        }
    }
    print_str("[ATC] Conflict detection: "); print_int(atc.n_conflicts); print_str(" conflicts\n");
    for (int i = 0; i < atc.n_conflicts; i++) {
        print_str("  ALERT: Flight "); print_int(conflicts[i].flight1);
        print_str(" vs Flight "); print_int(conflicts[i].flight2);
        print_str(" dist="); print_int((int)(conflicts[i].distance * 1000)); print_str("m");
        print_str(" sev="); print_int(conflicts[i].severity); print_str("\n");
    }
}

void atc_weather_service(void) {
    print_str("[ATC] Weather service:\n");
    print_str("  Wind: 270/15kt\n");
    print_str("  Visibility: 10km\n");
    print_str("  Clouds: SCT030\n");
    print_str("  QNH: 1013hPa\n");
    if (atc.n_flights > 8) {
        print_str("  ALERT: High traffic volume\n");
        atc.weather_alerts++;
    }
}

void atc_print_state(void) {
    print_str("[ATC] Flights="); print_int(atc.n_flights);
    print_str(" Runways="); print_int(atc.n_runways);
    print_str(" Sectors="); print_int(atc.n_sectors);
    print_str(" Landings="); print_int(atc.total_landings);
    print_str(" Takeoffs="); print_int(atc.total_takeoffs);
    print_str(" Conflicts="); print_int(atc.conflict_alerts); print_str("\n");
}

int main(void) {
    print_str("=== Air Traffic Control System Demo ===\n\n");
    atc_init();

    print_str("Adding runways...\n");
    atc_add_runway(3000.0);
    atc_add_runway(2500.0);
    atc_add_runway(2800.0);

    print_str("\nAdding sectors...\n");
    atc_add_sector(0.0, 50.0, 0.0, 50.0);
    atc_add_sector(50.0, 100.0, 0.0, 50.0);
    atc_add_sector(0.0, 50.0, 50.0, 100.0);
    atc_add_sector(50.0, 100.0, 50.0, 100.0);

    print_str("\nAdding flights...\n");
    atc_add_flight(10.0, 10.0, 5000.0, 90.0);
    atc_add_flight(30.0, 20.0, 8000.0, 180.0);
    atc_add_flight(60.0, 40.0, 6000.0, 270.0);
    atc_add_flight(80.0, 70.0, 4000.0, 0.0);
    atc_add_flight(20.0, 80.0, 7000.0, 135.0);
    atc_add_flight(70.0, 15.0, 5500.0, 225.0);

    print_str("\nSector management...\n");
    atc_sector_management();

    print_str("\nLanding sequence...\n");
    atc_landing_sequence(0, 0);
    atc_landing_sequence(1, 1);
    atc_landing_sequence(2, 2);

    print_str("\nLanding complete...\n");
    atc_landing_complete(0);
    atc_landing_complete(1);

    print_str("\nTakeoff...\n");
    atc_takeoff(3, 0);
    atc_takeoff(4, 1);
    atc_takeoff(5, 2);

    print_str("\nConflict detection...\n");
    flights[0].x = 30.0; flights[0].y = 30.0;
    flights[3].x = 32.0; flights[3].y = 31.0;
    atc_conflict_detection();

    print_str("\nWeather service...\n");
    atc_weather_service();

    atc_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
