/* railway_company: Railway transportation management (v1.0)
 * Lines, trains, passengers, freight, maintenance
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

#define MAX_LINES      12
#define MAX_TRAINS     14
#define MAX_STATIONS   18
#define MAX_FREIGHT    10
#define MAX_MAINT      10

typedef struct {
    int    line_id;
    int    zone;
    int    length_km;
    int    stations;
    int    speed_limit;
    int    electrified;
    int    year_built;
    int    active;
} line_t;

typedef struct {
    int    train_id;
    int    line_id;
    int    type;
    int    capacity;
    int    speed;
    int    status;
    int    mileage;
    int    active;
} train_t;

typedef struct {
    int    station_id;
    int    line_id;
    int    zone;
    int    platforms;
    int    passengers_daily;
    int    staff;
    int    year_built;
    int    active;
} station_t;

typedef struct {
    int    freight_id;
    int    line_id;
    int    type;
    int    weight_tons;
    int    distance_km;
    int    revenue;
    int    year;
    int    active;
} freight_t;

typedef struct {
    int    maint_id;
    int    type;
    int    line_id;
    int    cost;
    int    duration_days;
    int    status;
    int    year;
    int    active;
} maintenance_t;

typedef struct {
    int    n_lines;
    int    n_trains;
    int    n_stations;
    int    n_freight;
    int    n_maint;
    int    total_track_km;
    int    total_passengers;
    int    total_freight_tons;
    int    total_revenue;
    int    total_maint_cost;
} rc_state_t;

static line_t lines[MAX_LINES];
static train_t trains[MAX_TRAINS];
static station_t stations[MAX_STATIONS];
static freight_t freights[MAX_FREIGHT];
static maintenance_t maints[MAX_MAINT];
static rc_state_t rc;

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

int rc_init(void) {
    if (initialized) return -1;
    rc.n_lines = 0; rc.n_trains = 0; rc.n_stations = 0;
    rc.n_freight = 0; rc.n_maint = 0;
    rc.total_track_km = 0; rc.total_passengers = 0;
    rc.total_freight_tons = 0; rc.total_revenue = 0;
    rc.total_maint_cost = 0;
    for (int i = 0; i < MAX_LINES; i++) lines[i].active = 0;
    for (int i = 0; i < MAX_TRAINS; i++) trains[i].active = 0;
    for (int i = 0; i < MAX_STATIONS; i++) stations[i].active = 0;
    for (int i = 0; i < MAX_FREIGHT; i++) freights[i].active = 0;
    for (int i = 0; i < MAX_MAINT; i++) maints[i].active = 0;
    initialized = 1;
    print_str("[RC] Railway company initialized\n");
    return 0;
}

int rc_add_line(int zone, int length, int stations, int speed, int electrified, int year) {
    if (rc.n_lines >= MAX_LINES) return -1;
    line_t* l = &lines[rc.n_lines];
    l->line_id = rc.n_lines;
    l->zone = zone;
    l->length_km = length;
    l->stations = stations;
    l->speed_limit = speed;
    l->electrified = electrified;
    l->year_built = year;
    l->active = 1;
    rc.total_track_km += length;
    rc.n_lines++;
    print_str("[RC] Line "); print_int(rc.n_lines - 1);
    print_str(" zone="); print_int(zone);
    print_str(" len="); print_int(length); print_str("km");
    print_str(" stn="); print_int(stations);
    print_str(" spd="); print_int(speed); print_str("km/h\n");
    return rc.n_lines - 1;
}

int rc_add_train(int line_id, int type, int capacity, int speed) {
    if (rc.n_trains >= MAX_TRAINS) return -1;
    train_t* t = &trains[rc.n_trains];
    t->train_id = rc.n_trains;
    t->line_id = line_id;
    t->type = type;
    t->capacity = capacity;
    t->speed = speed;
    t->status = 1;
    t->mileage = 0;
    t->active = 1;
    rc.n_trains++;
    print_str("[RC] Train "); print_int(rc.n_trains - 1);
    print_str(" line="); print_int(line_id);
    print_str(" type="); print_int(type);
    print_str(" cap="); print_int(capacity);
    print_str(" spd="); print_int(speed); print_str("km/h\n");
    return rc.n_trains - 1;
}

int rc_add_station(int line_id, int zone, int platforms, int passengers, int staff, int year) {
    if (rc.n_stations >= MAX_STATIONS) return -1;
    station_t* s = &stations[rc.n_stations];
    s->station_id = rc.n_stations;
    s->line_id = line_id;
    s->zone = zone;
    s->platforms = platforms;
    s->passengers_daily = passengers;
    s->staff = staff;
    s->year_built = year;
    s->active = 1;
    rc.total_passengers += passengers;
    rc.n_stations++;
    print_str("[RC] Stn "); print_int(rc.n_stations - 1);
    print_str(" line="); print_int(line_id);
    print_str(" zone="); print_int(zone);
    print_str(" plt="); print_int(platforms);
    print_str(" pax="); print_int(passengers); print_str("/day\n");
    return rc.n_stations - 1;
}

int rc_add_freight(int line_id, int type, int weight, int distance, int revenue, int year) {
    if (rc.n_freight >= MAX_FREIGHT) return -1;
    freight_t* f = &freights[rc.n_freight];
    f->freight_id = rc.n_freight;
    f->line_id = line_id;
    f->type = type;
    f->weight_tons = weight;
    f->distance_km = distance;
    f->revenue = revenue;
    f->year = year;
    f->active = 1;
    rc.total_freight_tons += weight;
    rc.total_revenue += revenue;
    rc.n_freight++;
    print_str("[RC] Freight "); print_int(rc.n_freight - 1);
    print_str(" line="); print_int(line_id);
    print_str(" type="); print_int(type);
    print_str(" tons="); print_int(weight);
    print_str(" dist="); print_int(distance); print_str("km");
    print_str(" rev=$"); print_int(revenue); print_str("\n");
    return rc.n_freight - 1;
}

int rc_schedule_maintenance(int type, int line_id, int cost, int duration, int year) {
    if (rc.n_maint >= MAX_MAINT) return -1;
    maintenance_t* m = &maints[rc.n_maint];
    m->maint_id = rc.n_maint;
    m->type = type;
    m->line_id = line_id;
    m->cost = cost;
    m->duration_days = duration;
    m->status = 1;
    m->year = year;
    m->active = 1;
    rc.total_maint_cost += cost;
    rc.n_maint++;
    print_str("[RC] Maint "); print_int(rc.n_maint - 1);
    print_str(" type="); print_int(type);
    print_str(" line="); print_int(line_id);
    print_str(" cost=$"); print_int(cost);
    print_str(" days="); print_int(duration); print_str("\n");
    return rc.n_maint - 1;
}

int rc_complete_maintenance(int maint_id) {
    if (maint_id >= rc.n_maint) return -1;
    maints[maint_id].status = 2;
    print_str("[RC] Complete Mt"); print_int(maint_id); print_str("\n");
    return 0;
}

void rc_line_report(void) {
    print_str("[RC] Line report:\n");
    print_str("  Lines: "); print_int(rc.n_lines); print_str("\n");
    print_str("  Track: "); print_int(rc.total_track_km); print_str(" km\n");
    print_str("  Stations: "); print_int(rc.n_stations); print_str("\n");
    print_str("  Trains: "); print_int(rc.n_trains); print_str("\n");
}

void rc_passenger_report(void) {
    print_str("[RC] Passenger report:\n");
    print_str("  Daily passengers: "); print_int(rc.total_passengers); print_str("\n");
}

void rc_freight_report(void) {
    print_str("[RC] Freight report:\n");
    print_str("  Total freight: "); print_int(rc.total_freight_tons); print_str(" tons\n");
    print_str("  Total revenue: $"); print_int(rc.total_revenue); print_str("\n");
    print_str("  Maintenance cost: $"); print_int(rc.total_maint_cost); print_str("\n");
}

void rc_print_state(void) {
    print_str("[RC] Ln="); print_int(rc.n_lines);
    print_str(" Tr="); print_int(rc.n_trains);
    print_str(" St="); print_int(rc.n_stations);
    print_str(" Fr="); print_int(rc.n_freight);
    print_str(" Mt="); print_int(rc.n_maint);
    print_str("\n");
}

int main(void) {
    print_str("=== Railway Company Demo ===\n\n");
    rc_init();

    print_str("Adding lines...\n");
    for (int i = 0; i < 12; i++) {
        int zone = (i % 4) + 1;
        int length = 100 + (i * 50);
        int stns = 5 + (i * 2);
        int speed = 120 + (i * 20);
        int elec = (i % 3 == 0) ? 1 : 0;
        int year = 1990 + (i * 3);
        rc_add_line(zone, length, stns, speed, elec, year);
    }

    print_str("\nAdding trains...\n");
    for (int i = 0; i < 14; i++) {
        int line = i % 12;
        int type = (i % 3) + 1;
        int cap = 200 + (i * 50);
        int speed = 100 + (i * 15);
        rc_add_train(line, type, cap, speed);
    }

    print_str("\nAdding stations...\n");
    for (int i = 0; i < 18; i++) {
        int line = i % 12;
        int zone = (i % 4) + 1;
        int plt = 2 + (i % 4);
        int pax = 1000 + (i * 500);
        int staff = 10 + (i * 3);
        int year = 1995 + (i * 2);
        rc_add_station(line, zone, plt, pax, staff, year);
    }

    print_str("\nAdding freight...\n");
    for (int i = 0; i < 10; i++) {
        int line = i % 12;
        int type = (i % 4) + 1;
        int weight = 500 + (i * 200);
        int dist = 200 + (i * 100);
        int rev = 10000 + (i * 5000);
        int year = 2024;
        rc_add_freight(line, type, weight, dist, rev, year);
    }

    print_str("\nScheduling maintenance...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 3) + 1;
        int line = i % 12;
        int cost = 50000 + (i * 20000);
        int dur = 3 + (i % 7);
        int year = 2024;
        rc_schedule_maintenance(type, line, cost, dur, year);
    }

    print_str("\nCompleting maintenance...\n");
    for (int i = 0; i < 8; i++) {
        rc_complete_maintenance(i);
    }

    print_str("\nLine report...\n");
    rc_line_report();

    print_str("\nPassenger report...\n");
    rc_passenger_report();

    print_str("\nFreight report...\n");
    rc_freight_report();

    print_str("\nFinal state...\n");
    rc_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
