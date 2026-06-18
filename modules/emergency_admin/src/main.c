/* emergency_admin: Emergency administration system (v1.0)
 * Disaster response, fire rescue, emergency shelter, public safety, plans
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

#define MAX_DISASTER     16
#define MAX_FIRE         14
#define MAX_SHELTER      12
#define MAX_SAFETY       10
#define MAX_PLAN         10

typedef struct {
    int    disaster_id;
    int    region_id;
    int    disaster_type;
    int    severity;
    int    affected;
    int    response_level;
    int    year;
    int    active;
} disaster_t;

typedef struct {
    int    fire_id;
    int    station_id;
    int    incident_type;
    int    response_time;
    int    units_dispatched;
    int    casualties;
    int    year;
    int    active;
} fire_t;

typedef struct {
    int    shelter_id;
    int    location_id;
    int    shelter_type;
    int    capacity;
    int    current_occupants;
    int    supplies_days;
    int    year;
    int    active;
} shelter_t;

typedef struct {
    int    safety_id;
    int    area_id;
    int    risk_type;
    int    monitoring_score;
    int    hazards_found;
    int    rectified;
    int    year;
    int    active;
} safety_t;

typedef struct {
    int    plan_id;
    int    plan_type;
    int    region_id;
    int    personnel;
    int    drill_count;
    int    training_hours;
    int    year;
    int    active;
} plan_t;

typedef struct {
    int    n_disaster;
    int    n_fire;
    int    n_shelter;
    int    n_safety;
    int    n_plan;
    int    total_affected;
    int    total_responses;
    int    total_capacity;
    int    total_hazards;
    int    total_drills;
} emg_state_t;

static disaster_t disasters[MAX_DISASTER];
static fire_t fires[MAX_FIRE];
static shelter_t shelters[MAX_SHELTER];
static safety_t safeties[MAX_SAFETY];
static plan_t plans[MAX_PLAN];
static emg_state_t emg;

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

int emg_init(void) {
    if (initialized) return -1;
    emg.n_disaster = 0; emg.n_fire = 0; emg.n_shelter = 0;
    emg.n_safety = 0; emg.n_plan = 0;
    emg.total_affected = 0; emg.total_responses = 0;
    emg.total_capacity = 0; emg.total_hazards = 0;
    emg.total_drills = 0;
    for (int i = 0; i < MAX_DISASTER; i++) disasters[i].active = 0;
    for (int i = 0; i < MAX_FIRE; i++) fires[i].active = 0;
    for (int i = 0; i < MAX_SHELTER; i++) shelters[i].active = 0;
    for (int i = 0; i < MAX_SAFETY; i++) safeties[i].active = 0;
    for (int i = 0; i < MAX_PLAN; i++) plans[i].active = 0;
    initialized = 1;
    print_str("[EMG] Emergency admin initialized\n");
    return 0;
}

int emg_disaster(int region, int disaster_type, int severity, int affected, int response_level, int year) {
    if (emg.n_disaster >= MAX_DISASTER) return -1;
    disaster_t* d = &disasters[emg.n_disaster];
    d->disaster_id = emg.n_disaster;
    d->region_id = region;
    d->disaster_type = disaster_type;
    d->severity = severity;
    d->affected = affected;
    d->response_level = response_level;
    d->year = year;
    d->active = 1;
    emg.total_affected += affected;
    emg.n_disaster++;
    print_str("[EMG] Disaster "); print_int(emg.n_disaster - 1);
    print_str(" rgn="); print_int(region);
    print_str(" type="); print_int(disaster_type);
    print_str(" sev="); print_int(severity);
    print_str(" aff="); print_int(affected);
    print_str(" rsp="); print_int(response_level); print_str("\n");
    return emg.n_disaster - 1;
}

int emg_fire(int station, int incident_type, int response_time, int units, int casualties, int year) {
    if (emg.n_fire >= MAX_FIRE) return -1;
    fire_t* f = &fires[emg.n_fire];
    f->fire_id = emg.n_fire;
    f->station_id = station;
    f->incident_type = incident_type;
    f->response_time = response_time;
    f->units_dispatched = units;
    f->casualties = casualties;
    f->year = year;
    f->active = 1;
    emg.total_responses++;
    emg.n_fire++;
    print_str("[EMG] Fire "); print_int(emg.n_fire - 1);
    print_str(" stn="); print_int(station);
    print_str(" type="); print_int(incident_type);
    print_str(" rsp="); print_int(response_time); print_str("min");
    print_str(" unt="); print_int(units);
    print_str(" cas="); print_int(casualties); print_str("\n");
    return emg.n_fire - 1;
}

int emg_shelter(int location, int shelter_type, int capacity, int occupants, int supplies, int year) {
    if (emg.n_shelter >= MAX_SHELTER) return -1;
    shelter_t* s = &shelters[emg.n_shelter];
    s->shelter_id = emg.n_shelter;
    s->location_id = location;
    s->shelter_type = shelter_type;
    s->capacity = capacity;
    s->current_occupants = occupants;
    s->supplies_days = supplies;
    s->year = year;
    s->active = 1;
    emg.total_capacity += capacity;
    emg.n_shelter++;
    print_str("[EMG] Shelter "); print_int(emg.n_shelter - 1);
    print_str(" loc="); print_int(location);
    print_str(" type="); print_int(shelter_type);
    print_str(" cap="); print_int(capacity);
    print_str(" occ="); print_int(occupants);
    print_str(" sup="); print_int(supplies); print_str("d\n");
    return emg.n_shelter - 1;
}

int emg_safety(int area, int risk_type, int score, int hazards, int rectified, int year) {
    if (emg.n_safety >= MAX_SAFETY) return -1;
    safety_t* sf = &safeties[emg.n_safety];
    sf->safety_id = emg.n_safety;
    sf->area_id = area;
    sf->risk_type = risk_type;
    sf->monitoring_score = score;
    sf->hazards_found = hazards;
    sf->rectified = rectified;
    sf->year = year;
    sf->active = 1;
    emg.total_hazards += hazards;
    emg.n_safety++;
    print_str("[EMG] Safety "); print_int(emg.n_safety - 1);
    print_str(" area="); print_int(area);
    print_str(" type="); print_int(risk_type);
    print_str(" scr="); print_int(score);
    print_str(" hzd="); print_int(hazards);
    print_str(" rct="); print_int(rectified); print_str("\n");
    return emg.n_safety - 1;
}

int emg_plan(int plan_type, int region, int personnel, int drills, int training, int year) {
    if (emg.n_plan >= MAX_PLAN) return -1;
    plan_t* p = &plans[emg.n_plan];
    p->plan_id = emg.n_plan;
    p->plan_type = plan_type;
    p->region_id = region;
    p->personnel = personnel;
    p->drill_count = drills;
    p->training_hours = training;
    p->year = year;
    p->active = 1;
    emg.total_drills += drills;
    emg.n_plan++;
    print_str("[EMG] Plan "); print_int(emg.n_plan - 1);
    print_str(" type="); print_int(plan_type);
    print_str(" rgn="); print_int(region);
    print_str(" prs="); print_int(personnel);
    print_str(" drl="); print_int(drills);
    print_str(" trn="); print_int(training); print_str("h\n");
    return emg.n_plan - 1;
}

void emg_disaster_report(void) {
    print_str("[EMG] Disaster report:\n");
    print_str("  Disasters: "); print_int(emg.n_disaster); print_str("\n");
    print_str("  Total affected: "); print_int(emg.total_affected); print_str("\n");
}

void emg_fire_report(void) {
    print_str("[EMG] Fire report:\n");
    print_str("  Fire incidents: "); print_int(emg.n_fire); print_str("\n");
    print_str("  Total responses: "); print_int(emg.total_responses); print_str("\n");
}

void emg_safety_report(void) {
    print_str("[EMG] Safety report:\n");
    print_str("  Shelters: "); print_int(emg.n_shelter); print_str("\n");
    print_str("  Total capacity: "); print_int(emg.total_capacity); print_str("\n");
    print_str("  Safety inspections: "); print_int(emg.n_safety); print_str("\n");
    print_str("  Hazards found: "); print_int(emg.total_hazards); print_str("\n");
    print_str("  Emergency plans: "); print_int(emg.n_plan); print_str("\n");
    print_str("  Total drills: "); print_int(emg.total_drills); print_str("\n");
}

void emg_print_state(void) {
    print_str("[EMG] Ds="); print_int(emg.n_disaster);
    print_str(" Fr="); print_int(emg.n_fire);
    print_str(" Sh="); print_int(emg.n_shelter);
    print_str(" Sf="); print_int(emg.n_safety);
    print_str(" Pl="); print_int(emg.n_plan);
    print_str("\n");
}

int main(void) {
    print_str("=== Emergency Admin Demo ===\n\n");
    emg_init();

    print_str("Disaster response...\n");
    for (int i = 0; i < 16; i++) {
        int rgn = (i % 8) + 1;
        int type = (i % 5) + 1;
        int sev = (i % 4) + 1;
        int aff = 1000 + (i * 500);
        int rsp = (i % 3) + 1;
        int year = 2020 + (i % 5);
        emg_disaster(rgn, type, sev, aff, rsp, year);
    }

    print_str("\nFire rescue...\n");
    for (int i = 0; i < 14; i++) {
        int stn = 100 + (i * 5);
        int type = (i % 4) + 1;
        int rsp = 5 + (i % 10);
        int unt = 2 + (i % 5);
        int cas = i % 3;
        int year = 2021 + (i % 4);
        emg_fire(stn, type, rsp, unt, cas, year);
    }

    print_str("\nEmergency shelters...\n");
    for (int i = 0; i < 12; i++) {
        int loc = 200 + (i * 10);
        int type = (i % 3) + 1;
        int cap = 500 + (i * 100);
        int occ = cap - (i * 50);
        int sup = 7 + (i % 14);
        int year = 2022 + (i % 3);
        emg_shelter(loc, type, cap, occ, sup, year);
    }

    print_str("\nPublic safety...\n");
    for (int i = 0; i < 10; i++) {
        int area = 300 + (i * 15);
        int type = (i % 4) + 1;
        int scr = 70 + (i * 3);
        int hzd = 5 + (i % 8);
        int rct = hzd - (i % 3);
        int year = 2023 + (i % 2);
        emg_safety(area, type, scr, hzd, rct, year);
    }

    print_str("\nEmergency plans...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 3) + 1;
        int rgn = (i % 8) + 1;
        int prs = 50 + (i * 20);
        int drl = 2 + (i % 4);
        int trn = 20 + (i * 5);
        int year = 2024;
        emg_plan(type, rgn, prs, drl, trn, year);
    }

    print_str("\nDisaster report...\n");
    emg_disaster_report();

    print_str("\nFire report...\n");
    emg_fire_report();

    print_str("\nSafety report...\n");
    emg_safety_report();

    print_str("\nFinal state...\n");
    emg_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
