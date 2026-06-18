/* fire_department: Fire department management (v1.0)
 * Stations, fire calls, investigations, prevention, training
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

#define MAX_STATIONS   12
#define MAX_TRUCKS     14
#define MAX_CALLS      20
#define MAX_PERSONNEL  18
#define MAX_TRAINING   10

typedef struct {
    int    station_id;
    int    zone;
    int    capacity;
    int    personnel;
    int    trucks;
    int    response_time;
    int    year_built;
    int    active;
} station_t;

typedef struct {
    int    truck_id;
    int    station_id;
    int    type;
    int    capacity;
    int    water_tank;
    int    status;
    int    mileage;
    int    active;
} truck_t;

typedef struct {
    int    call_id;
    int    type;
    int    zone;
    int    severity;
    int    response_time;
    int    trucks_dispatched;
    int    personnel_dispatched;
    int    damage_level;
    int    casualties;
    int    status;
    int    active;
} call_t;

typedef struct {
    int    personnel_id;
    int    station_id;
    int    rank;
    int    years_service;
    int    specialty;
    int    fitness_score;
    int    certifications;
    int    active;
} personnel_t;

typedef struct {
    int    training_id;
    int    type;
    int    zone;
    int    participants;
    int    duration_hours;
    int    pass_rate;
    int    year;
    int    active;
} training_t;

typedef struct {
    int    n_stations;
    int    n_trucks;
    int    n_calls;
    int    n_personnel;
    int    n_training;
    int    total_calls;
    int    total_responses;
    int    total_casualties;
    int    total_damage;
    int    total_training_done;
    int    avg_response_time;
} fd_state_t;

static station_t stations[MAX_STATIONS];
static truck_t trucks[MAX_TRUCKS];
static call_t calls[MAX_CALLS];
static personnel_t personnel[MAX_PERSONNEL];
static training_t trainings[MAX_TRAINING];
static fd_state_t fd;

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

int fd_init(void) {
    if (initialized) return -1;
    fd.n_stations = 0; fd.n_trucks = 0; fd.n_calls = 0;
    fd.n_personnel = 0; fd.n_training = 0;
    fd.total_calls = 0; fd.total_responses = 0;
    fd.total_casualties = 0; fd.total_damage = 0;
    fd.total_training_done = 0; fd.avg_response_time = 0;
    for (int i = 0; i < MAX_STATIONS; i++) stations[i].active = 0;
    for (int i = 0; i < MAX_TRUCKS; i++) trucks[i].active = 0;
    for (int i = 0; i < MAX_CALLS; i++) calls[i].active = 0;
    for (int i = 0; i < MAX_PERSONNEL; i++) personnel[i].active = 0;
    for (int i = 0; i < MAX_TRAINING; i++) trainings[i].active = 0;
    initialized = 1;
    print_str("[FD] Fire department initialized\n");
    return 0;
}

int fd_add_station(int zone, int capacity, int personnel_count, int trucks_count, int response_time, int year) {
    if (fd.n_stations >= MAX_STATIONS) return -1;
    station_t* s = &stations[fd.n_stations];
    s->station_id = fd.n_stations;
    s->zone = zone;
    s->capacity = capacity;
    s->personnel = personnel_count;
    s->trucks = trucks_count;
    s->response_time = response_time;
    s->year_built = year;
    s->active = 1;
    fd.n_stations++;
    print_str("[FD] Station "); print_int(fd.n_stations - 1);
    print_str(" zone="); print_int(zone);
    print_str(" cap="); print_int(capacity);
    print_str(" pers="); print_int(personnel_count);
    print_str(" trucks="); print_int(trucks_count);
    print_str(" rt="); print_int(response_time); print_str("min\n");
    return fd.n_stations - 1;
}

int fd_add_truck(int station_id, int type, int capacity, int water_tank) {
    if (fd.n_trucks >= MAX_TRUCKS) return -1;
    truck_t* t = &trucks[fd.n_trucks];
    t->truck_id = fd.n_trucks;
    t->station_id = station_id;
    t->type = type;
    t->capacity = capacity;
    t->water_tank = water_tank;
    t->status = 1;
    t->mileage = 0;
    t->active = 1;
    fd.n_trucks++;
    print_str("[FD] Truck "); print_int(fd.n_trucks - 1);
    print_str(" stn="); print_int(station_id);
    print_str(" type="); print_int(type);
    print_str(" cap="); print_int(capacity);
    print_str(" water="); print_int(water_tank); print_str("L\n");
    return fd.n_trucks - 1;
}

int fd_receive_call(int type, int zone, int severity) {
    if (fd.n_calls >= MAX_CALLS) return -1;
    call_t* c = &calls[fd.n_calls];
    c->call_id = fd.n_calls;
    c->type = type;
    c->zone = zone;
    c->severity = severity;
    c->response_time = 0;
    c->trucks_dispatched = 0;
    c->personnel_dispatched = 0;
    c->damage_level = 0;
    c->casualties = 0;
    c->status = 1;
    c->active = 1;
    fd.total_calls++;
    fd.n_calls++;
    print_str("[FD] Call "); print_int(fd.n_calls - 1);
    print_str(" type="); print_int(type);
    print_str(" zone="); print_int(zone);
    print_str(" sev="); print_int(severity); print_str("\n");
    return fd.n_calls - 1;
}

int fd_dispatch(int call_id, int trucks_count, int personnel_count, int response_time) {
    if (call_id >= fd.n_calls) return -1;
    calls[call_id].trucks_dispatched = trucks_count;
    calls[call_id].personnel_dispatched = personnel_count;
    calls[call_id].response_time = response_time;
    calls[call_id].status = 2;
    fd.total_responses++;
    fd.avg_response_time = (fd.avg_response_time * (fd.total_responses - 1) + response_time) / fd.total_responses;
    print_str("[FD] Dispatch C"); print_int(call_id);
    print_str(" trucks="); print_int(trucks_count);
    print_str(" pers="); print_int(personnel_count);
    print_str(" rt="); print_int(response_time); print_str("min\n");
    return 0;
}

int fd_report_damage(int call_id, int damage_level, int casualties) {
    if (call_id >= fd.n_calls) return -1;
    calls[call_id].damage_level = damage_level;
    calls[call_id].casualties = casualties;
    calls[call_id].status = 3;
    fd.total_damage += damage_level;
    fd.total_casualties += casualties;
    print_str("[FD] Damage C"); print_int(call_id);
    print_str(" dmg="); print_int(damage_level);
    print_str(" cas="); print_int(casualties); print_str("\n");
    return 0;
}

int fd_close_call(int call_id) {
    if (call_id >= fd.n_calls) return -1;
    calls[call_id].status = 4;
    print_str("[FD] Close C"); print_int(call_id); print_str("\n");
    return 0;
}

int fd_add_personnel(int station_id, int rank, int years, int specialty, int fitness, int certs) {
    if (fd.n_personnel >= MAX_PERSONNEL) return -1;
    personnel_t* p = &personnel[fd.n_personnel];
    p->personnel_id = fd.n_personnel;
    p->station_id = station_id;
    p->rank = rank;
    p->years_service = years;
    p->specialty = specialty;
    p->fitness_score = fitness;
    p->certifications = certs;
    p->active = 1;
    fd.n_personnel++;
    print_str("[FD] Pers "); print_int(fd.n_personnel - 1);
    print_str(" stn="); print_int(station_id);
    print_str(" rank="); print_int(rank);
    print_str(" yrs="); print_int(years);
    print_str(" spec="); print_int(specialty); print_str("\n");
    return fd.n_personnel - 1;
}

int fd_add_training(int type, int zone, int participants, int duration, int year) {
    if (fd.n_training >= MAX_TRAINING) return -1;
    training_t* t = &trainings[fd.n_training];
    t->training_id = fd.n_training;
    t->type = type;
    t->zone = zone;
    t->participants = participants;
    t->duration_hours = duration;
    t->pass_rate = 0;
    t->year = year;
    t->active = 1;
    fd.n_training++;
    print_str("[FD] Training "); print_int(fd.n_training - 1);
    print_str(" type="); print_int(type);
    print_str(" zone="); print_int(zone);
    print_str(" part="); print_int(participants);
    print_str(" hrs="); print_int(duration); print_str("\n");
    return fd.n_training - 1;
}

int fd_complete_training(int training_id, int pass_rate) {
    if (training_id >= fd.n_training) return -1;
    trainings[training_id].pass_rate = pass_rate;
    fd.total_training_done++;
    print_str("[FD] Complete Tr"); print_int(training_id);
    print_str(" pass="); print_int(pass_rate); print_str("%\n");
    return 0;
}

void fd_call_report(void) {
    print_str("[FD] Call report:\n");
    print_str("  Total calls: "); print_int(fd.total_calls); print_str("\n");
    print_str("  Total responses: "); print_int(fd.total_responses); print_str("\n");
    print_str("  Avg response: "); print_int(fd.avg_response_time); print_str(" min\n");
    print_str("  Total casualties: "); print_int(fd.total_casualties); print_str("\n");
    print_str("  Total damage: "); print_int(fd.total_damage); print_str("\n");
}

void fd_station_report(void) {
    print_str("[FD] Station report:\n");
    print_str("  Stations: "); print_int(fd.n_stations); print_str("\n");
    print_str("  Trucks: "); print_int(fd.n_trucks); print_str("\n");
    print_str("  Personnel: "); print_int(fd.n_personnel); print_str("\n");
    for (int i = 0; i < fd.n_stations; i++) {
        station_t* s = &stations[i];
        print_str("  Stn"); print_int(i);
        print_str(" Z"); print_int(s->zone);
        print_str(" cap="); print_int(s->capacity);
        print_str(" p="); print_int(s->personnel);
        print_str(" t="); print_int(s->trucks); print_str("\n");
    }
}

void fd_training_report(void) {
    print_str("[FD] Training report:\n");
    print_str("  Sessions: "); print_int(fd.n_training); print_str("\n");
    print_str("  Completed: "); print_int(fd.total_training_done); print_str("\n");
}

void fd_print_state(void) {
    print_str("[FD] Stn="); print_int(fd.n_stations);
    print_str(" Trk="); print_int(fd.n_trucks);
    print_str(" Cal="); print_int(fd.n_calls);
    print_str(" Per="); print_int(fd.n_personnel);
    print_str(" Trn="); print_int(fd.n_training);
    print_str("\n");
}

int main(void) {
    print_str("=== Fire Department Demo ===\n\n");
    fd_init();

    print_str("Adding stations...\n");
    for (int i = 0; i < 12; i++) {
        int zone = (i % 4) + 1;
        int cap = 20 + (i * 5);
        int pers = 15 + (i * 3);
        int trucks = 2 + (i % 4);
        int rt = 5 + (i % 8);
        int year = 2000 + (i * 2);
        fd_add_station(zone, cap, pers, trucks, rt, year);
    }

    print_str("\nAdding trucks...\n");
    for (int i = 0; i < 14; i++) {
        int stn = i % 12;
        int type = (i % 3) + 1;
        int cap = 500 + (i * 100);
        int water = 2000 + (i * 500);
        fd_add_truck(stn, type, cap, water);
    }

    print_str("\nReceiving calls...\n");
    for (int i = 0; i < 20; i++) {
        int type = (i % 4) + 1;
        int zone = (i % 4) + 1;
        int sev = (i % 5) + 1;
        fd_receive_call(type, zone, sev);
    }

    print_str("\nDispatching...\n");
    for (int i = 0; i < 20; i++) {
        int trucks = 1 + (i % 4);
        int pers = 4 + (i % 8);
        int rt = 3 + (i % 10);
        fd_dispatch(i, trucks, pers, rt);
    }

    print_str("\nReporting damage...\n");
    for (int i = 0; i < 20; i++) {
        int dmg = (i % 6) * 10;
        int cas = i % 3;
        fd_report_damage(i, dmg, cas);
    }

    print_str("\nClosing calls...\n");
    for (int i = 0; i < 20; i++) {
        fd_close_call(i);
    }

    print_str("\nAdding personnel...\n");
    for (int i = 0; i < 18; i++) {
        int stn = i % 12;
        int rank = (i % 5) + 1;
        int yrs = 1 + (i % 20);
        int spec = (i % 4) + 1;
        int fit = 70 + (i % 30);
        int certs = 1 + (i % 5);
        fd_add_personnel(stn, rank, yrs, spec, fit, certs);
    }

    print_str("\nAdding training...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 3) + 1;
        int zone = (i % 4) + 1;
        int part = 10 + (i * 5);
        int dur = 4 + (i * 2);
        int year = 2024;
        fd_add_training(type, zone, part, dur, year);
    }

    print_str("\nCompleting training...\n");
    for (int i = 0; i < 10; i++) {
        int rate = 75 + (i % 25);
        fd_complete_training(i, rate);
    }

    print_str("\nCall report...\n");
    fd_call_report();

    print_str("\nStation report...\n");
    fd_station_report();

    print_str("\nTraining report...\n");
    fd_training_report();

    print_str("\nFinal state...\n");
    fd_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
