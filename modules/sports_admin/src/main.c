/* sports_admin: Sports administration system (v1.0)
 * Competitive sports, mass sports, sports industry, youth sports, facilities
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

#define MAX_COMPETITIVE  16
#define MAX_MASS         14
#define MAX_INDUSTRY     12
#define MAX_YOUTH        10
#define MAX_FACILITY     10

typedef struct {
    int    comp_id;
    int    sport_type;
    int    athlete_id;
    int    coach_id;
    int    training_hours;
    int    medals;
    int    records;
    int    year;
    int    active;
} competitive_t;

typedef struct {
    int    mass_id;
    int    activity_type;
    int    region_id;
    int    participants;
    int    instructors;
    int    events;
    int    budget;
    int    year;
    int    active;
} mass_t;

typedef struct {
    int    industry_id;
    int    company_type;
    int    company_id;
    int    revenue;
    int    employees;
    int    events_hosted;
    int    lottery_sales;
    int    year;
    int    active;
} industry_t;

typedef struct {
    int    youth_id;
    int    school_id;
    int    sport_type;
    int    students;
    int    talent_pool;
    int    competitions;
    int    year;
    int    active;
} youth_t;

typedef struct {
    int    facility_id;
    int    venue_type;
    int    location_id;
    int    capacity;
    int    utilization_pct;
    int    maintenance_cost;
    int    events_held;
    int    year;
    int    active;
} facility_t;

typedef struct {
    int    n_competitive;
    int    n_mass;
    int    n_industry;
    int    n_youth;
    int    n_facility;
    int    total_medals;
    int    total_participants;
    int    total_revenue;
    int    total_students;
    int    total_capacity;
} sa_state_t;

static competitive_t competitives[MAX_COMPETITIVE];
static mass_t mass_sports[MAX_MASS];
static industry_t industries[MAX_INDUSTRY];
static youth_t youths[MAX_YOUTH];
static facility_t facilities[MAX_FACILITY];
static sa_state_t sa;

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

int sa_init(void) {
    if (initialized) return -1;
    sa.n_competitive = 0; sa.n_mass = 0; sa.n_industry = 0;
    sa.n_youth = 0; sa.n_facility = 0;
    sa.total_medals = 0; sa.total_participants = 0;
    sa.total_revenue = 0; sa.total_students = 0;
    sa.total_capacity = 0;
    for (int i = 0; i < MAX_COMPETITIVE; i++) competitives[i].active = 0;
    for (int i = 0; i < MAX_MASS; i++) mass_sports[i].active = 0;
    for (int i = 0; i < MAX_INDUSTRY; i++) industries[i].active = 0;
    for (int i = 0; i < MAX_YOUTH; i++) youths[i].active = 0;
    for (int i = 0; i < MAX_FACILITY; i++) facilities[i].active = 0;
    initialized = 1;
    print_str("[SA] Sports admin initialized\n");
    return 0;
}

int sa_competitive(int sport, int athlete, int coach, int training, int medals, int records, int year) {
    if (sa.n_competitive >= MAX_COMPETITIVE) return -1;
    competitive_t* c = &competitives[sa.n_competitive];
    c->comp_id = sa.n_competitive;
    c->sport_type = sport;
    c->athlete_id = athlete;
    c->coach_id = coach;
    c->training_hours = training;
    c->medals = medals;
    c->records = records;
    c->year = year;
    c->active = 1;
    sa.total_medals += medals;
    sa.n_competitive++;
    print_str("[SA] Competitive "); print_int(sa.n_competitive - 1);
    print_str(" spr="); print_int(sport);
    print_str(" ath="); print_int(athlete);
    print_str(" cch="); print_int(coach);
    print_str(" trn="); print_int(training); print_str("h");
    print_str(" mdl="); print_int(medals); print_str("\n");
    return sa.n_competitive - 1;
}

int sa_mass(int activity, int region, int participants, int instructors, int events, int budget, int year) {
    if (sa.n_mass >= MAX_MASS) return -1;
    mass_t* m = &mass_sports[sa.n_mass];
    m->mass_id = sa.n_mass;
    m->activity_type = activity;
    m->region_id = region;
    m->participants = participants;
    m->instructors = instructors;
    m->events = events;
    m->budget = budget;
    m->year = year;
    m->active = 1;
    sa.total_participants += participants;
    sa.n_mass++;
    print_str("[SA] Mass "); print_int(sa.n_mass - 1);
    print_str(" act="); print_int(activity);
    print_str(" reg="); print_int(region);
    print_str(" ptc="); print_int(participants);
    print_str(" ins="); print_int(instructors);
    print_str(" evt="); print_int(events); print_str("\n");
    return sa.n_mass - 1;
}

int sa_industry(int company_type, int company, int revenue, int employees, int events, int lottery, int year) {
    if (sa.n_industry >= MAX_INDUSTRY) return -1;
    industry_t* ind = &industries[sa.n_industry];
    ind->industry_id = sa.n_industry;
    ind->company_type = company_type;
    ind->company_id = company;
    ind->revenue = revenue;
    ind->employees = employees;
    ind->events_hosted = events;
    ind->lottery_sales = lottery;
    ind->year = year;
    ind->active = 1;
    sa.total_revenue += revenue;
    sa.n_industry++;
    print_str("[SA] Industry "); print_int(sa.n_industry - 1);
    print_str(" typ="); print_int(company_type);
    print_str(" co="); print_int(company);
    print_str(" rev=$"); print_int(revenue);
    print_str(" emp="); print_int(employees);
    print_str(" lot=$"); print_int(lottery); print_str("\n");
    return sa.n_industry - 1;
}

int sa_youth(int school, int sport, int students, int talent, int competitions, int year) {
    if (sa.n_youth >= MAX_YOUTH) return -1;
    youth_t* y = &youths[sa.n_youth];
    y->youth_id = sa.n_youth;
    y->school_id = school;
    y->sport_type = sport;
    y->students = students;
    y->talent_pool = talent;
    y->competitions = competitions;
    y->year = year;
    y->active = 1;
    sa.total_students += students;
    sa.n_youth++;
    print_str("[SA] Youth "); print_int(sa.n_youth - 1);
    print_str(" sch="); print_int(school);
    print_str(" spr="); print_int(sport);
    print_str(" std="); print_int(students);
    print_str(" tln="); print_int(talent);
    print_str(" cmp="); print_int(competitions); print_str("\n");
    return sa.n_youth - 1;
}

int sa_facility(int venue, int location, int capacity, int utilization, int maint, int events, int year) {
    if (sa.n_facility >= MAX_FACILITY) return -1;
    facility_t* f = &facilities[sa.n_facility];
    f->facility_id = sa.n_facility;
    f->venue_type = venue;
    f->location_id = location;
    f->capacity = capacity;
    f->utilization_pct = utilization;
    f->maintenance_cost = maint;
    f->events_held = events;
    f->year = year;
    f->active = 1;
    sa.total_capacity += capacity;
    sa.n_facility++;
    print_str("[SA] Facility "); print_int(sa.n_facility - 1);
    print_str(" vnu="); print_int(venue);
    print_str(" loc="); print_int(location);
    print_str(" cap="); print_int(capacity);
    print_str(" utl="); print_int(utilization); print_str("%");
    print_str(" mnt=$"); print_int(maint); print_str("\n");
    return sa.n_facility - 1;
}

void sa_competitive_report(void) {
    print_str("[SA] Competitive report:\n");
    print_str("  Athletes: "); print_int(sa.n_competitive); print_str("\n");
    print_str("  Total medals: "); print_int(sa.total_medals); print_str("\n");
}

void sa_mass_report(void) {
    print_str("[SA] Mass sports report:\n");
    print_str("  Activities: "); print_int(sa.n_mass); print_str("\n");
    print_str("  Total participants: "); print_int(sa.total_participants); print_str("\n");
}

void sa_facility_report(void) {
    print_str("[SA] Facility report:\n");
    print_str("  Facilities: "); print_int(sa.n_facility); print_str("\n");
    print_str("  Total capacity: "); print_int(sa.total_capacity); print_str("\n");
    print_str("  Industry companies: "); print_int(sa.n_industry); print_str("\n");
    print_str("  Total revenue: $"); print_int(sa.total_revenue); print_str("\n");
    print_str("  Youth sports schools: "); print_int(sa.n_youth); print_str("\n");
    print_str("  Total students: "); print_int(sa.total_students); print_str("\n");
}

void sa_print_state(void) {
    print_str("[SA] Cp="); print_int(sa.n_competitive);
    print_str(" Ms="); print_int(sa.n_mass);
    print_str(" In="); print_int(sa.n_industry);
    print_str(" Yh="); print_int(sa.n_youth);
    print_str(" Fc="); print_int(sa.n_facility);
    print_str("\n");
}

int main(void) {
    print_str("=== Sports Admin Demo ===\n\n");
    sa_init();

    print_str("Competitive sports...\n");
    for (int i = 0; i < 16; i++) {
        int spr = (i % 8) + 1;
        int ath = 1000 + (i * 13);
        int cch = 200 + (i % 10);
        int trn = 200 + (i * 50);
        int mdl = i % 5;
        int rec = (i % 6 == 0) ? 1 : 0;
        int year = 2020 + (i % 5);
        sa_competitive(spr, ath, cch, trn, mdl, rec, year);
    }

    print_str("\nMass sports...\n");
    for (int i = 0; i < 14; i++) {
        int act = (i % 5) + 1;
        int reg = (i % 8) + 1;
        int ptc = 100 + (i * 50);
        int ins = 5 + (i * 2);
        int evt = 2 + (i % 4);
        int bgt = 30000 + (i * 15000);
        int year = 2022 + (i % 3);
        sa_mass(act, reg, ptc, ins, evt, bgt, year);
    }

    print_str("\nSports industry...\n");
    for (int i = 0; i < 12; i++) {
        int ctype = (i % 4) + 1;
        int co = 3000 + (i * 11);
        int rev = 500000 + (i * 200000);
        int emp = 20 + (i * 10);
        int evt = 3 + (i % 6);
        int lot = 100000 + (i * 50000);
        int year = 2023 + (i % 2);
        sa_industry(ctype, co, rev, emp, evt, lot, year);
    }

    print_str("\nYouth sports...\n");
    for (int i = 0; i < 10; i++) {
        int sch = 4000 + (i * 17);
        int spr = (i % 6) + 1;
        int std = 50 + (i * 20);
        int tln = 5 + (i * 3);
        int cmp = 2 + (i % 4);
        int year = 2024;
        sa_youth(sch, spr, std, tln, cmp, year);
    }

    print_str("\nSports facilities...\n");
    for (int i = 0; i < 10; i++) {
        int vnu = (i % 4) + 1;
        int loc = (i % 8) + 1;
        int cap = 1000 + (i * 500);
        int utl = 50 + (i * 5);
        int mnt = 20000 + (i * 10000);
        int evt = 5 + (i * 3);
        int year = 2024;
        sa_facility(vnu, loc, cap, utl, mnt, evt, year);
    }

    print_str("\nCompetitive report...\n");
    sa_competitive_report();

    print_str("\nMass sports report...\n");
    sa_mass_report();

    print_str("\nFacility report...\n");
    sa_facility_report();

    print_str("\nFinal state...\n");
    sa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
