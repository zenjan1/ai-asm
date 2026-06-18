/* elderly_care: Elderly care service system (v1.0)
 * Home care, community care, institution care, health, smart care
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

#define MAX_HOME         16
#define MAX_COMMUNITY    14
#define MAX_INSTITUTION  12
#define MAX_HEALTH       10
#define MAX_SMART        10

typedef struct {
    int    home_id;
    int    elder_id;
    int    service_type;
    int    visits;
    int    hours;
    int    cost;
    int    satisfaction;
    int    year;
    int    active;
} home_t;

typedef struct {
    int    community_id;
    int    center_id;
    int    day_care;
    int    meals_served;
    int    activities;
    int    participants;
    int    budget;
    int    year;
    int    active;
} community_t;

typedef struct {
    int    inst_id;
    int    facility_id;
    int    capacity;
    int    occupied;
    int    care_level;
    int    staff;
    int    monthly_fee;
    int    year;
    int    active;
} institution_t;

typedef struct {
    int    health_id;
    int    elder_id;
    int    checkup_count;
    int    chronic_diseases;
    int    medications;
    int    telemedicine;
    int    year;
    int    active;
} health_t;

typedef struct {
    int    smart_id;
    int    elder_id;
    int    device_type;
    int    alerts_triggered;
    int    gps_enabled;
    int    battery_pct;
    int    year;
    int    active;
} smart_t;

typedef struct {
    int    n_home;
    int    n_community;
    int    n_institution;
    int    n_health;
    int    n_smart;
    int    total_visits;
    int    total_meals;
    int    total_occupants;
    int    total_staff;
    int    total_alerts;
} ec_state_t;

static home_t homes[MAX_HOME];
static community_t communities[MAX_COMMUNITY];
static institution_t institutions[MAX_INSTITUTION];
static health_t healths[MAX_HEALTH];
static smart_t smarts[MAX_SMART];
static ec_state_t ec;

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

int ec_init(void) {
    if (initialized) return -1;
    ec.n_home = 0; ec.n_community = 0; ec.n_institution = 0;
    ec.n_health = 0; ec.n_smart = 0;
    ec.total_visits = 0; ec.total_meals = 0;
    ec.total_occupants = 0; ec.total_staff = 0;
    ec.total_alerts = 0;
    for (int i = 0; i < MAX_HOME; i++) homes[i].active = 0;
    for (int i = 0; i < MAX_COMMUNITY; i++) communities[i].active = 0;
    for (int i = 0; i < MAX_INSTITUTION; i++) institutions[i].active = 0;
    for (int i = 0; i < MAX_HEALTH; i++) healths[i].active = 0;
    for (int i = 0; i < MAX_SMART; i++) smarts[i].active = 0;
    initialized = 1;
    print_str("[EC] Elderly care initialized\n");
    return 0;
}

int ec_home(int elder, int service, int visits, int hours, int cost, int satisfaction, int year) {
    if (ec.n_home >= MAX_HOME) return -1;
    home_t* h = &homes[ec.n_home];
    h->home_id = ec.n_home;
    h->elder_id = elder;
    h->service_type = service;
    h->visits = visits;
    h->hours = hours;
    h->cost = cost;
    h->satisfaction = satisfaction;
    h->year = year;
    h->active = 1;
    ec.total_visits += visits;
    ec.n_home++;
    print_str("[EC] Home "); print_int(ec.n_home - 1);
    print_str(" eld="); print_int(elder);
    print_str(" svc="); print_int(service);
    print_str(" vst="); print_int(visits);
    print_str(" hrs="); print_int(hours);
    print_str(" cst=$"); print_int(cost); print_str("\n");
    return ec.n_home - 1;
}

int ec_community(int center, int daycare, int meals, int activities, int participants, int budget, int year) {
    if (ec.n_community >= MAX_COMMUNITY) return -1;
    community_t* c = &communities[ec.n_community];
    c->community_id = ec.n_community;
    c->center_id = center;
    c->day_care = daycare;
    c->meals_served = meals;
    c->activities = activities;
    c->participants = participants;
    c->budget = budget;
    c->year = year;
    c->active = 1;
    ec.total_meals += meals;
    ec.n_community++;
    print_str("[EC] Community "); print_int(ec.n_community - 1);
    print_str(" ctr="); print_int(center);
    print_str(" day="); print_int(daycare);
    print_str(" mls="); print_int(meals);
    print_str(" act="); print_int(activities);
    print_str(" ptc="); print_int(participants); print_str("\n");
    return ec.n_community - 1;
}

int ec_institution(int facility, int capacity, int occupied, int care_level, int staff_count, int fee, int year) {
    if (ec.n_institution >= MAX_INSTITUTION) return -1;
    institution_t* inst = &institutions[ec.n_institution];
    inst->inst_id = ec.n_institution;
    inst->facility_id = facility;
    inst->capacity = capacity;
    inst->occupied = occupied;
    inst->care_level = care_level;
    inst->staff = staff_count;
    inst->monthly_fee = fee;
    inst->year = year;
    inst->active = 1;
    ec.total_occupants += occupied;
    ec.total_staff += staff_count;
    ec.n_institution++;
    print_str("[EC] Institution "); print_int(ec.n_institution - 1);
    print_str(" fac="); print_int(facility);
    print_str(" cap="); print_int(capacity);
    print_str(" occ="); print_int(occupied);
    print_str(" lvl="); print_int(care_level);
    print_str(" stf="); print_int(staff_count);
    print_str(" fee=$"); print_int(fee); print_str("\n");
    return ec.n_institution - 1;
}

int ec_health(int elder, int checkups, int chronic, int medications, int telemedicine, int year) {
    if (ec.n_health >= MAX_HEALTH) return -1;
    health_t* h = &healths[ec.n_health];
    h->health_id = ec.n_health;
    h->elder_id = elder;
    h->checkup_count = checkups;
    h->chronic_diseases = chronic;
    h->medications = medications;
    h->telemedicine = telemedicine;
    h->year = year;
    h->active = 1;
    ec.n_health++;
    print_str("[EC] Health "); print_int(ec.n_health - 1);
    print_str(" eld="); print_int(elder);
    print_str(" chk="); print_int(checkups);
    print_str(" chr="); print_int(chronic);
    print_str(" med="); print_int(medications);
    print_str(" tel="); print_int(telemedicine); print_str("\n");
    return ec.n_health - 1;
}

int ec_smart(int elder, int device, int alerts, int gps, int battery, int year) {
    if (ec.n_smart >= MAX_SMART) return -1;
    smart_t* s = &smarts[ec.n_smart];
    s->smart_id = ec.n_smart;
    s->elder_id = elder;
    s->device_type = device;
    s->alerts_triggered = alerts;
    s->gps_enabled = gps;
    s->battery_pct = battery;
    s->year = year;
    s->active = 1;
    ec.total_alerts += alerts;
    ec.n_smart++;
    print_str("[EC] Smart "); print_int(ec.n_smart - 1);
    print_str(" eld="); print_int(elder);
    print_str(" dev="); print_int(device);
    print_str(" alr="); print_int(alerts);
    print_str(" gps="); print_int(gps);
    print_str(" bat="); print_int(battery); print_str("%\n");
    return ec.n_smart - 1;
}

void ec_home_report(void) {
    print_str("[EC] Home care report:\n");
    print_str("  Home care cases: "); print_int(ec.n_home); print_str("\n");
    print_str("  Total visits: "); print_int(ec.total_visits); print_str("\n");
}

void ec_community_report(void) {
    print_str("[EC] Community care report:\n");
    print_str("  Community centers: "); print_int(ec.n_community); print_str("\n");
    print_str("  Total meals served: "); print_int(ec.total_meals); print_str("\n");
}

void ec_institution_report(void) {
    print_str("[EC] Institution report:\n");
    print_str("  Institutions: "); print_int(ec.n_institution); print_str("\n");
    print_str("  Total occupants: "); print_int(ec.total_occupants); print_str("\n");
    print_str("  Total staff: "); print_int(ec.total_staff); print_str("\n");
    print_str("  Health records: "); print_int(ec.n_health); print_str("\n");
    print_str("  Smart devices: "); print_int(ec.n_smart); print_str("\n");
    print_str("  Total alerts: "); print_int(ec.total_alerts); print_str("\n");
}

void ec_print_state(void) {
    print_str("[EC] Hm="); print_int(ec.n_home);
    print_str(" Cm="); print_int(ec.n_community);
    print_str(" In="); print_int(ec.n_institution);
    print_str(" Hl="); print_int(ec.n_health);
    print_str(" Sm="); print_int(ec.n_smart);
    print_str("\n");
}

int main(void) {
    print_str("=== Elderly Care Demo ===\n\n");
    ec_init();

    print_str("Home care services...\n");
    for (int i = 0; i < 16; i++) {
        int eld = 1000 + (i * 13);
        int svc = (i % 4) + 1;
        int vst = 5 + (i * 3);
        int hrs = vst * 2;
        int cst = 500 + (i * 200);
        int sat = 70 + (i * 2);
        int year = 2021 + (i % 4);
        ec_home(eld, svc, vst, hrs, cst, sat, year);
    }

    print_str("\nCommunity care...\n");
    for (int i = 0; i < 14; i++) {
        int ctr = 200 + (i * 10);
        int day = 10 + (i * 5);
        int mls = 50 + (i * 20);
        int act = 3 + (i % 5);
        int ptc = 20 + (i * 10);
        int bgt = 20000 + (i * 10000);
        int year = 2022 + (i % 3);
        ec_community(ctr, day, mls, act, ptc, bgt, year);
    }

    print_str("\nInstitution care...\n");
    for (int i = 0; i < 12; i++) {
        int fac = 300 + (i * 20);
        int cap = 50 + (i * 20);
        int occ = 30 + (i * 15);
        int lvl = (i % 4) + 1;
        int stf = 10 + (i * 5);
        int fee = 3000 + (i * 500);
        int year = 2023 + (i % 2);
        ec_institution(fac, cap, occ, lvl, stf, fee, year);
    }

    print_str("\nHealth management...\n");
    for (int i = 0; i < 10; i++) {
        int eld = 4000 + (i * 11);
        int chk = 2 + (i % 4);
        int chr = (i % 3);
        int med = 1 + (i % 5);
        int tel = (i % 3 == 0) ? 1 : 0;
        int year = 2024;
        ec_health(eld, chk, chr, med, tel, year);
    }

    print_str("\nSmart elderly care...\n");
    for (int i = 0; i < 10; i++) {
        int eld = 5000 + (i * 7);
        int dev = (i % 4) + 1;
        int alr = i % 5;
        int gps = (i % 2);
        int bat = 60 + (i * 4);
        int year = 2024;
        ec_smart(eld, dev, alr, gps, bat, year);
    }

    print_str("\nHome care report...\n");
    ec_home_report();

    print_str("\nCommunity care report...\n");
    ec_community_report();

    print_str("\nInstitution report...\n");
    ec_institution_report();

    print_str("\nFinal state...\n");
    ec_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
