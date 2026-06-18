/* aerospace_admin: Aerospace administration system (v1.0)
 * Aircraft manufacturing, aero engines, airborne systems, airport construction, aviation operations
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

#define MAX_AIRCRAFT     16
#define MAX_ENGINE       14
#define MAX_AIRBORNE     12
#define MAX_AIRPORT      10
#define MAX_OPERATION    10

typedef struct {
    int    aircraft_id;
    int    aircraft_type;
    int    aircraft_category;
    int    large_aircraft;
    int    regional;
    int    general_aviation;
    int    year;
    int    active;
} aircraft_t;

typedef struct {
    int    engine_id;
    int    engine_type;
    int    engine_category;
    int    turbofan;
    int    turboprop;
    int    aero_motor;
    int    year;
    int    active;
} engine_t;

typedef struct {
    int    airborne_id;
    int    airborne_type;
    int    airborne_category;
    int    avionics;
    int    flight_control;
    int    airborne_equip;
    int    year;
    int    active;
} airborne_t;

typedef struct {
    int    airport_id;
    int    airport_type;
    int    airport_category;
    int    airport_plan;
    int    runway;
    int    terminal;
    int    year;
    int    active;
} airport_t;

typedef struct {
    int    operation_id;
    int    operation_type;
    int    operation_category;
    int    airline;
    int    route_manage;
    int    maintenance;
    int    year;
    int    active;
} operation_t;

typedef struct {
    int    n_aircraft;
    int    n_engine;
    int    n_airborne;
    int    n_airport;
    int    n_operation;
    int    total_large;
    int    total_turbofan;
    int    total_avionics;
    int    total_runway;
    int    total_airline;
} asa_state_t;

static aircraft_t aircrafts[MAX_AIRCRAFT];
static engine_t engines[MAX_ENGINE];
static airborne_t airbornes[MAX_AIRBORNE];
static airport_t airports[MAX_AIRPORT];
static operation_t operations[MAX_OPERATION];
static asa_state_t asa;

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

int asa_init(void) {
    if (initialized) return -1;
    asa.n_aircraft = 0; asa.n_engine = 0; asa.n_airborne = 0;
    asa.n_airport = 0; asa.n_operation = 0;
    asa.total_large = 0; asa.total_turbofan = 0;
    asa.total_avionics = 0; asa.total_runway = 0;
    asa.total_airline = 0;
    for (int i = 0; i < MAX_AIRCRAFT; i++) aircrafts[i].active = 0;
    for (int i = 0; i < MAX_ENGINE; i++) engines[i].active = 0;
    for (int i = 0; i < MAX_AIRBORNE; i++) airbornes[i].active = 0;
    for (int i = 0; i < MAX_AIRPORT; i++) airports[i].active = 0;
    for (int i = 0; i < MAX_OPERATION; i++) operations[i].active = 0;
    initialized = 1;
    print_str("[ASA] Aerospace initialized\n");
    return 0;
}

int asa_aircraft(int ac_type, int cat, int large, int regional, int general, int year) {
    if (asa.n_aircraft >= MAX_AIRCRAFT) return -1;
    aircraft_t* a = &aircrafts[asa.n_aircraft];
    a->aircraft_id = asa.n_aircraft;
    a->aircraft_type = ac_type;
    a->aircraft_category = cat;
    a->large_aircraft = large;
    a->regional = regional;
    a->general_aviation = general;
    a->year = year;
    a->active = 1;
    asa.total_large += large;
    asa.n_aircraft++;
    print_str("[ASA] Aircraft "); print_int(asa.n_aircraft - 1);
    print_str(" type="); print_int(ac_type);
    print_str(" cat="); print_int(cat);
    print_str(" lrg="); print_int(large);
    print_str(" rgn="); print_int(regional);
    print_str(" gen="); print_int(general); print_str("\n");
    return asa.n_aircraft - 1;
}

int asa_engine(int eng_type, int cat, int turbofan, int turboprop, int motor, int year) {
    if (asa.n_engine >= MAX_ENGINE) return -1;
    engine_t* e = &engines[asa.n_engine];
    e->engine_id = asa.n_engine;
    e->engine_type = eng_type;
    e->engine_category = cat;
    e->turbofan = turbofan;
    e->turboprop = turboprop;
    e->aero_motor = motor;
    e->year = year;
    e->active = 1;
    asa.total_turbofan += turbofan;
    asa.n_engine++;
    print_str("[ASA] Engine "); print_int(asa.n_engine - 1);
    print_str(" type="); print_int(eng_type);
    print_str(" cat="); print_int(cat);
    print_str(" tfn="); print_int(turbofan);
    print_str(" tpr="); print_int(turboprop);
    print_str(" mtr="); print_int(motor); print_str("\n");
    return asa.n_engine - 1;
}

int asa_airborne(int ab_type, int cat, int avio, int fcs, int equip, int year) {
    if (asa.n_airborne >= MAX_AIRBORNE) return -1;
    airborne_t* a = &airbornes[asa.n_airborne];
    a->airborne_id = asa.n_airborne;
    a->airborne_type = ab_type;
    a->airborne_category = cat;
    a->avionics = avio;
    a->flight_control = fcs;
    a->airborne_equip = equip;
    a->year = year;
    a->active = 1;
    asa.total_avionics += avio;
    asa.n_airborne++;
    print_str("[ASA] Airborne "); print_int(asa.n_airborne - 1);
    print_str(" type="); print_int(ab_type);
    print_str(" cat="); print_int(cat);
    print_str(" avn="); print_int(avio);
    print_str(" fcs="); print_int(fcs);
    print_str(" equ="); print_int(equip); print_str("\n");
    return asa.n_airborne - 1;
}

int asa_airport(int apt_type, int cat, int plan, int runway_cnt, int terminal_cnt, int year) {
    if (asa.n_airport >= MAX_AIRPORT) return -1;
    airport_t* a = &airports[asa.n_airport];
    a->airport_id = asa.n_airport;
    a->airport_type = apt_type;
    a->airport_category = cat;
    a->airport_plan = plan;
    a->runway = runway_cnt;
    a->terminal = terminal_cnt;
    a->year = year;
    a->active = 1;
    asa.total_runway += runway_cnt;
    asa.n_airport++;
    print_str("[ASA] Airport "); print_int(asa.n_airport - 1);
    print_str(" type="); print_int(apt_type);
    print_str(" cat="); print_int(cat);
    print_str(" pln="); print_int(plan);
    print_str(" rnw="); print_int(runway_cnt);
    print_str(" tmn="); print_int(terminal_cnt); print_str("\n");
    return asa.n_airport - 1;
}

int asa_operation(int opt_type, int cat, int airline_cnt, int route, int maint, int year) {
    if (asa.n_operation >= MAX_OPERATION) return -1;
    operation_t* o = &operations[asa.n_operation];
    o->operation_id = asa.n_operation;
    o->operation_type = opt_type;
    o->operation_category = cat;
    o->airline = airline_cnt;
    o->route_manage = route;
    o->maintenance = maint;
    o->year = year;
    o->active = 1;
    asa.total_airline += airline_cnt;
    asa.n_operation++;
    print_str("[ASA] Operation "); print_int(asa.n_operation - 1);
    print_str(" type="); print_int(opt_type);
    print_str(" cat="); print_int(cat);
    print_str(" aln="); print_int(airline_cnt);
    print_str(" rte="); print_int(route);
    print_str(" mnt="); print_int(maint); print_str("\n");
    return asa.n_operation - 1;
}

void asa_aircraft_report(void) {
    print_str("[ASA] Aircraft report:\n");
    print_str("  Aircraft manufacturing categories: "); print_int(asa.n_aircraft); print_str("\n");
    print_str("  Total large aircraft: "); print_int(asa.total_large); print_str("\n");
}

void asa_engine_report(void) {
    print_str("[ASA] Engine report:\n");
    print_str("  Aero engine categories: "); print_int(asa.n_engine); print_str("\n");
    print_str("  Total turbofan: "); print_int(asa.total_turbofan); print_str("\n");
}

void asa_operation_report(void) {
    print_str("[ASA] Operation report:\n");
    print_str("  Airborne system categories: "); print_int(asa.n_airborne); print_str("\n");
    print_str("  Total avionics: "); print_int(asa.total_avionics); print_str("\n");
    print_str("  Airport construction categories: "); print_int(asa.n_airport); print_str("\n");
    print_str("  Total runway: "); print_int(asa.total_runway); print_str("\n");
    print_str("  Aviation operation categories: "); print_int(asa.n_operation); print_str("\n");
    print_str("  Total airline: "); print_int(asa.total_airline); print_str("\n");
}

void asa_print_state(void) {
    print_str("[ASA] Ac="); print_int(asa.n_aircraft);
    print_str(" En="); print_int(asa.n_engine);
    print_str(" Ab="); print_int(asa.n_airborne);
    print_str(" Ap="); print_int(asa.n_airport);
    print_str(" Op="); print_int(asa.n_operation);
    print_str("\n");
}

int main(void) {
    print_str("=== Aerospace Admin Demo ===\n\n");
    asa_init();

    print_str("Aircraft manufacturing...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int lrg = 45 + (i * 11);
        int rgn = 30 + (i * 7);
        int gen = 20 + (i * 5);
        int year = 2020 + (i % 5);
        asa_aircraft(type, cat, lrg, rgn, gen, year);
    }

    print_str("\nAero engines...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int tfn = 38 + (i * 9);
        int tpr = 25 + (i * 6);
        int mtr = 18 + (i * 4);
        int year = 2021 + (i % 4);
        asa_engine(type, cat, tfn, tpr, mtr, year);
    }

    print_str("\nAirborne systems...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int avn = 42 + (i * 10);
        int fcs = 28 + (i * 7);
        int equ = 20 + (i * 5);
        int year = 2022 + (i % 3);
        asa_airborne(type, cat, avn, fcs, equ, year);
    }

    print_str("\nAirport construction...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pln = 30 + (i * 8);
        int rnw = 20 + (i * 5);
        int tmn = 15 + (i * 3);
        int year = 2023 + (i % 2);
        asa_airport(type, cat, pln, rnw, tmn, year);
    }

    print_str("\nAviation operations...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int aln = 25 + (i * 6);
        int rte = 18 + (i * 4);
        int mnt = 12 + (i * 3);
        int year = 2024;
        asa_operation(type, cat, aln, rte, mnt, year);
    }

    print_str("\nAircraft report...\n");
    asa_aircraft_report();

    print_str("\nEngine report...\n");
    asa_engine_report();

    print_str("\nOperation report...\n");
    asa_operation_report();

    print_str("\nFinal state...\n");
    asa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
