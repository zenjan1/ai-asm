/* ethnic_affairs: Ethnic affairs management (v1.0)
 * Ethnic policy, economic development, culture, unity, religious affairs
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

#define MAX_POLICY       16
#define MAX_ECONOMY      14
#define MAX_CULTURE      12
#define MAX_UNITY        10
#define MAX_RELIGION     10

typedef struct {
    int    policy_id;
    int    type;
    int    region_id;
    int    ethnic_group;
    int    beneficiaries;
    int    budget;
    int    year;
    int    status;
    int    active;
} policy_t;

typedef struct {
    int    economy_id;
    int    region_id;
    int    project_type;
    int    investment;
    int    employment;
    int    revenue;
    int    poverty_reduction;
    int    year;
    int    active;
} economy_t;

typedef struct {
    int    culture_id;
    int    type;
    int    ethnic_group;
    int    participants;
    int    funding;
    int    preservation_score;
    int    year;
    int    active;
} culture_t;

typedef struct {
    int    unity_id;
    int    event_type;
    int    region_id;
    int    participants;
    int    ethnic_groups;
    int    satisfaction;
    int    year;
    int    active;
} unity_t;

typedef struct {
    int    religion_id;
    int    type;
    int    location_id;
    int    registered_members;
    int    activities;
    int    compliance_score;
    int    year;
    int    active;
} religion_t;

typedef struct {
    int    n_policy;
    int    n_economy;
    int    n_culture;
    int    n_unity;
    int    n_religion;
    int    total_beneficiaries;
    int    total_investment;
    int    total_participants;
    int    total_employment;
    int    total_revenue;
} ea_state_t;

static policy_t policies[MAX_POLICY];
static economy_t economies[MAX_ECONOMY];
static culture_t cultures[MAX_CULTURE];
static unity_t unity_events[MAX_UNITY];
static religion_t religions[MAX_RELIGION];
static ea_state_t ea;

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

int ea_init(void) {
    if (initialized) return -1;
    ea.n_policy = 0; ea.n_economy = 0; ea.n_culture = 0;
    ea.n_unity = 0; ea.n_religion = 0;
    ea.total_beneficiaries = 0; ea.total_investment = 0;
    ea.total_participants = 0; ea.total_employment = 0;
    ea.total_revenue = 0;
    for (int i = 0; i < MAX_POLICY; i++) policies[i].active = 0;
    for (int i = 0; i < MAX_ECONOMY; i++) economies[i].active = 0;
    for (int i = 0; i < MAX_CULTURE; i++) cultures[i].active = 0;
    for (int i = 0; i < MAX_UNITY; i++) unity_events[i].active = 0;
    for (int i = 0; i < MAX_RELIGION; i++) religions[i].active = 0;
    initialized = 1;
    print_str("[EA] Ethnic affairs initialized\n");
    return 0;
}

int ea_policy(int type, int region, int ethnic, int beneficiaries, int budget, int year) {
    if (ea.n_policy >= MAX_POLICY) return -1;
    policy_t* p = &policies[ea.n_policy];
    p->policy_id = ea.n_policy;
    p->type = type;
    p->region_id = region;
    p->ethnic_group = ethnic;
    p->beneficiaries = beneficiaries;
    p->budget = budget;
    p->year = year;
    p->status = 1;
    p->active = 1;
    ea.total_beneficiaries += beneficiaries;
    ea.n_policy++;
    print_str("[EA] Policy "); print_int(ea.n_policy - 1);
    print_str(" type="); print_int(type);
    print_str(" reg="); print_int(region);
    print_str(" eth="); print_int(ethnic);
    print_str(" ben="); print_int(beneficiaries);
    print_str(" bgt=$"); print_int(budget); print_str("\n");
    return ea.n_policy - 1;
}

int ea_economy(int region, int project, int investment, int employment, int revenue, int poverty, int year) {
    if (ea.n_economy >= MAX_ECONOMY) return -1;
    economy_t* e = &economies[ea.n_economy];
    e->economy_id = ea.n_economy;
    e->region_id = region;
    e->project_type = project;
    e->investment = investment;
    e->employment = employment;
    e->revenue = revenue;
    e->poverty_reduction = poverty;
    e->year = year;
    e->active = 1;
    ea.total_investment += investment;
    ea.total_employment += employment;
    ea.total_revenue += revenue;
    ea.n_economy++;
    print_str("[EA] Economy "); print_int(ea.n_economy - 1);
    print_str(" reg="); print_int(region);
    print_str(" prj="); print_int(project);
    print_str(" inv=$"); print_int(investment);
    print_str(" emp="); print_int(employment);
    print_str(" rev=$"); print_int(revenue); print_str("\n");
    return ea.n_economy - 1;
}

int ea_culture(int type, int ethnic, int participants, int funding, int score, int year) {
    if (ea.n_culture >= MAX_CULTURE) return -1;
    culture_t* c = &cultures[ea.n_culture];
    c->culture_id = ea.n_culture;
    c->type = type;
    c->ethnic_group = ethnic;
    c->participants = participants;
    c->funding = funding;
    c->preservation_score = score;
    c->year = year;
    c->active = 1;
    ea.total_participants += participants;
    ea.n_culture++;
    print_str("[EA] Culture "); print_int(ea.n_culture - 1);
    print_str(" type="); print_int(type);
    print_str(" eth="); print_int(ethnic);
    print_str(" ptc="); print_int(participants);
    print_str(" fnd=$"); print_int(funding);
    print_str(" scr="); print_int(score); print_str("\n");
    return ea.n_culture - 1;
}

int ea_unity(int event, int region, int participants, int ethnic_groups, int satisfaction, int year) {
    if (ea.n_unity >= MAX_UNITY) return -1;
    unity_t* u = &unity_events[ea.n_unity];
    u->unity_id = ea.n_unity;
    u->event_type = event;
    u->region_id = region;
    u->participants = participants;
    u->ethnic_groups = ethnic_groups;
    u->satisfaction = satisfaction;
    u->year = year;
    u->active = 1;
    ea.n_unity++;
    print_str("[EA] Unity "); print_int(ea.n_unity - 1);
    print_str(" evt="); print_int(event);
    print_str(" reg="); print_int(region);
    print_str(" ptc="); print_int(participants);
    print_str(" grp="); print_int(ethnic_groups);
    print_str(" sat="); print_int(satisfaction); print_str("\n");
    return ea.n_unity - 1;
}

int ea_religion(int type, int location, int members, int activities, int compliance, int year) {
    if (ea.n_religion >= MAX_RELIGION) return -1;
    religion_t* r = &religions[ea.n_religion];
    r->religion_id = ea.n_religion;
    r->type = type;
    r->location_id = location;
    r->registered_members = members;
    r->activities = activities;
    r->compliance_score = compliance;
    r->year = year;
    r->active = 1;
    ea.n_religion++;
    print_str("[EA] Religion "); print_int(ea.n_religion - 1);
    print_str(" type="); print_int(type);
    print_str(" loc="); print_int(location);
    print_str(" mem="); print_int(members);
    print_str(" act="); print_int(activities);
    print_str(" cmp="); print_int(compliance); print_str("\n");
    return ea.n_religion - 1;
}

void ea_policy_report(void) {
    print_str("[EA] Policy report:\n");
    print_str("  Policies: "); print_int(ea.n_policy); print_str("\n");
    print_str("  Total beneficiaries: "); print_int(ea.total_beneficiaries); print_str("\n");
}

void ea_economy_report(void) {
    print_str("[EA] Economy report:\n");
    print_str("  Projects: "); print_int(ea.n_economy); print_str("\n");
    print_str("  Total investment: $"); print_int(ea.total_investment); print_str("\n");
    print_str("  Total employment: "); print_int(ea.total_employment); print_str("\n");
    print_str("  Total revenue: $"); print_int(ea.total_revenue); print_str("\n");
}

void ea_culture_report(void) {
    print_str("[EA] Culture report:\n");
    print_str("  Cultural programs: "); print_int(ea.n_culture); print_str("\n");
    print_str("  Total participants: "); print_int(ea.total_participants); print_str("\n");
    print_str("  Unity events: "); print_int(ea.n_unity); print_str("\n");
    print_str("  Religious sites: "); print_int(ea.n_religion); print_str("\n");
}

void ea_print_state(void) {
    print_str("[EA] Pl="); print_int(ea.n_policy);
    print_str(" Ec="); print_int(ea.n_economy);
    print_str(" Cu="); print_int(ea.n_culture);
    print_str(" Un="); print_int(ea.n_unity);
    print_str(" Rl="); print_int(ea.n_religion);
    print_str("\n");
}

int main(void) {
    print_str("=== Ethnic Affairs Demo ===\n\n");
    ea_init();

    print_str("Ethnic policies...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 4) + 1;
        int reg = (i % 6) + 1;
        int eth = (i % 8) + 1;
        int ben = 1000 + (i * 500);
        int bgt = 50000 + (i * 20000);
        int year = 2020 + (i % 5);
        ea_policy(type, reg, eth, ben, bgt, year);
    }

    print_str("\nEconomic development...\n");
    for (int i = 0; i < 14; i++) {
        int reg = (i % 6) + 1;
        int prj = (i % 5) + 1;
        int inv = 100000 + (i * 50000);
        int emp = 50 + (i * 20);
        int rev = 200000 + (i * 80000);
        int pov = 20 + (i * 10);
        int year = 2021 + (i % 4);
        ea_economy(reg, prj, inv, emp, rev, pov, year);
    }

    print_str("\nCultural preservation...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int eth = (i % 8) + 1;
        int ptc = 100 + (i * 50);
        int fnd = 20000 + (i * 10000);
        int scr = 60 + (i * 3);
        int year = 2022 + (i % 3);
        ea_culture(type, eth, ptc, fnd, scr, year);
    }

    print_str("\nUnity activities...\n");
    for (int i = 0; i < 10; i++) {
        int evt = (i % 3) + 1;
        int reg = (i % 6) + 1;
        int ptc = 200 + (i * 100);
        int grp = 3 + (i % 5);
        int sat = 70 + (i * 3);
        int year = 2023 + (i % 2);
        ea_unity(evt, reg, ptc, grp, sat, year);
    }

    print_str("\nReligious affairs...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int loc = 300 + (i * 20);
        int mem = 50 + (i * 30);
        int act = 5 + (i * 3);
        int cmp = 75 + (i * 2);
        int year = 2024;
        ea_religion(type, loc, mem, act, cmp, year);
    }

    print_str("\nPolicy report...\n");
    ea_policy_report();

    print_str("\nEconomy report...\n");
    ea_economy_report();

    print_str("\nCulture report...\n");
    ea_culture_report();

    print_str("\nFinal state...\n");
    ea_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
