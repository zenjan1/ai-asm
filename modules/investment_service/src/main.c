/* investment_service: Investment service administration system (v1.0)
 * Investment promotion, enterprise service, park management, project tracking
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

#define MAX_INVESTMENT   16
#define MAX_ENTERPRISE   14
#define MAX_PARK         12
#define MAX_PROJECT      10
#define MAX_POLICY       10

typedef struct {
    int    investment_id;
    int    investment_type;
    int    source_region;
    int    projects_attracted;
    int    capital_invested;
    int    jobs_created;
    int    year;
    int    active;
} investment_t;

typedef struct {
    int    enterprise_id;
    int    enterprise_type;
    int    industry_sector;
    int    enterprises_registered;
    int    support_provided;
    int    training_sessions;
    int    year;
    int    active;
} enterprise_t;

typedef struct {
    int    park_id;
    int    park_type;
    int    park_level;
    int    parks_operated;
    int    tenants_leased;
    int    revenue_generated;
    int    year;
    int    active;
} park_t;

typedef struct {
    int    project_id;
    int    project_type;
    int    project_scale;
    int    projects_docked;
    int    projects_landed;
    int    completion_rate;
    int    year;
    int    active;
} project_t;

typedef struct {
    int    policy_id;
    int    policy_type;
    int    incentive_category;
    int    policies_issued;
    int    applications_processed;
    int    incentives_granted;
    int    year;
    int    active;
} policy_t;

typedef struct {
    int    n_investment;
    int    n_enterprise;
    int    n_park;
    int    n_project;
    int    n_policy;
    int    total_invested;
    int    total_jobs;
    int    total_tenants;
    int    total_landed;
    int    total_incentives;
} is_state_t;

static investment_t investments[MAX_INVESTMENT];
static enterprise_t enterprises[MAX_ENTERPRISE];
static park_t parks[MAX_PARK];
static project_t projects[MAX_PROJECT];
static policy_t policies[MAX_POLICY];
static is_state_t is;

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

int is_init(void) {
    if (initialized) return -1;
    is.n_investment = 0; is.n_enterprise = 0; is.n_park = 0;
    is.n_project = 0; is.n_policy = 0;
    is.total_invested = 0; is.total_jobs = 0;
    is.total_tenants = 0; is.total_landed = 0;
    is.total_incentives = 0;
    for (int i = 0; i < MAX_INVESTMENT; i++) investments[i].active = 0;
    for (int i = 0; i < MAX_ENTERPRISE; i++) enterprises[i].active = 0;
    for (int i = 0; i < MAX_PARK; i++) parks[i].active = 0;
    for (int i = 0; i < MAX_PROJECT; i++) projects[i].active = 0;
    for (int i = 0; i < MAX_POLICY; i++) policies[i].active = 0;
    initialized = 1;
    print_str("[IS] Investment service initialized\n");
    return 0;
}

int is_investment(int inv_type, int source, int attracted, int capital, int jobs, int year) {
    if (is.n_investment >= MAX_INVESTMENT) return -1;
    investment_t* inv = &investments[is.n_investment];
    inv->investment_id = is.n_investment;
    inv->investment_type = inv_type;
    inv->source_region = source;
    inv->projects_attracted = attracted;
    inv->capital_invested = capital;
    inv->jobs_created = jobs;
    inv->year = year;
    inv->active = 1;
    is.total_invested += capital;
    is.total_jobs += jobs;
    is.n_investment++;
    print_str("[IS] Investment "); print_int(is.n_investment - 1);
    print_str(" type="); print_int(inv_type);
    print_str(" src="); print_int(source);
    print_str(" atr="); print_int(attracted);
    print_str(" cap="); print_int(capital);
    print_str(" job="); print_int(jobs); print_str("\n");
    return is.n_investment - 1;
}

int is_enterprise(int ent_type, int sector, int registered_cnt, int support, int training, int year) {
    if (is.n_enterprise >= MAX_ENTERPRISE) return -1;
    enterprise_t* e = &enterprises[is.n_enterprise];
    e->enterprise_id = is.n_enterprise;
    e->enterprise_type = ent_type;
    e->industry_sector = sector;
    e->enterprises_registered = registered_cnt;
    e->support_provided = support;
    e->training_sessions = training;
    e->year = year;
    e->active = 1;
    is.n_enterprise++;
    print_str("[IS] Enterprise "); print_int(is.n_enterprise - 1);
    print_str(" type="); print_int(ent_type);
    print_str(" sec="); print_int(sector);
    print_str(" reg="); print_int(registered_cnt);
    print_str(" sup="); print_int(support);
    print_str(" trn="); print_int(training); print_str("\n");
    return is.n_enterprise - 1;
}

int is_park(int park_type, int level, int operated, int tenants, int revenue, int year) {
    if (is.n_park >= MAX_PARK) return -1;
    park_t* p = &parks[is.n_park];
    p->park_id = is.n_park;
    p->park_type = park_type;
    p->park_level = level;
    p->parks_operated = operated;
    p->tenants_leased = tenants;
    p->revenue_generated = revenue;
    p->year = year;
    p->active = 1;
    is.total_tenants += tenants;
    is.n_park++;
    print_str("[IS] Park "); print_int(is.n_park - 1);
    print_str(" type="); print_int(park_type);
    print_str(" lvl="); print_int(level);
    print_str(" opr="); print_int(operated);
    print_str(" tnt="); print_int(tenants);
    print_str(" rev="); print_int(revenue); print_str("\n");
    return is.n_park - 1;
}

int is_project(int proj_type, int scale, int docked, int landed, int completion, int year) {
    if (is.n_project >= MAX_PROJECT) return -1;
    project_t* pr = &projects[is.n_project];
    pr->project_id = is.n_project;
    pr->project_type = proj_type;
    pr->project_scale = scale;
    pr->projects_docked = docked;
    pr->projects_landed = landed;
    pr->completion_rate = completion;
    pr->year = year;
    pr->active = 1;
    is.total_landed += landed;
    is.n_project++;
    print_str("[IS] Project "); print_int(is.n_project - 1);
    print_str(" type="); print_int(proj_type);
    print_str(" scl="); print_int(scale);
    print_str(" dck="); print_int(docked);
    print_str(" lnd="); print_int(landed);
    print_str(" cmp="); print_int(completion); print_str("\n");
    return is.n_project - 1;
}

int is_policy(int pol_type, int incentive, int issued, int processed, int granted, int year) {
    if (is.n_policy >= MAX_POLICY) return -1;
    policy_t* pl = &policies[is.n_policy];
    pl->policy_id = is.n_policy;
    pl->policy_type = pol_type;
    pl->incentive_category = incentive;
    pl->policies_issued = issued;
    pl->applications_processed = processed;
    pl->incentives_granted = granted;
    pl->year = year;
    pl->active = 1;
    is.total_incentives += granted;
    is.n_policy++;
    print_str("[IS] Policy "); print_int(is.n_policy - 1);
    print_str(" type="); print_int(pol_type);
    print_str(" cat="); print_int(incentive);
    print_str(" iss="); print_int(issued);
    print_str(" prc="); print_int(processed);
    print_str(" grt="); print_int(granted); print_str("\n");
    return is.n_policy - 1;
}

void is_investment_report(void) {
    print_str("[IS] Investment report:\n");
    print_str("  Investment categories: "); print_int(is.n_investment); print_str("\n");
    print_str("  Total capital invested: "); print_int(is.total_invested); print_str("\n");
    print_str("  Total jobs created: "); print_int(is.total_jobs); print_str("\n");
}

void is_park_report(void) {
    print_str("[IS] Park report:\n");
    print_str("  Enterprise types: "); print_int(is.n_enterprise); print_str("\n");
    print_str("  Park categories: "); print_int(is.n_park); print_str("\n");
    print_str("  Total tenants: "); print_int(is.total_tenants); print_str("\n");
}

void is_project_report(void) {
    print_str("[IS] Project report:\n");
    print_str("  Project categories: "); print_int(is.n_project); print_str("\n");
    print_str("  Total projects landed: "); print_int(is.total_landed); print_str("\n");
    print_str("  Policy categories: "); print_int(is.n_policy); print_str("\n");
    print_str("  Total incentives granted: "); print_int(is.total_incentives); print_str("\n");
}

void is_print_state(void) {
    print_str("[IS] In="); print_int(is.n_investment);
    print_str(" En="); print_int(is.n_enterprise);
    print_str(" Pk="); print_int(is.n_park);
    print_str(" Pj="); print_int(is.n_project);
    print_str(" Pl="); print_int(is.n_policy);
    print_str("\n");
}

int main(void) {
    print_str("=== Investment Service Demo ===\n\n");
    is_init();

    print_str("Investment promotion...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int src = (i % 6) + 1;
        int atr = 30 + (i * 8);
        int cap = 5000 + (i * 1200);
        int job = 500 + (i * 120);
        int year = 2020 + (i % 5);
        is_investment(type, src, atr, cap, job, year);
    }

    print_str("\nEnterprise service...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int sec = (i % 6) + 1;
        int reg = 200 + (i * 50);
        int sup = 100 + (i * 25);
        int trn = 50 + (i * 12);
        int year = 2021 + (i % 4);
        is_enterprise(type, sec, reg, sup, trn, year);
    }

    print_str("\nPark management...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int lvl = (i % 3) + 1;
        int opr = 5 + (i * 2);
        int tnt = 50 + (i * 15);
        int rev = 1000 + (i * 300);
        int year = 2022 + (i % 3);
        is_park(type, lvl, opr, tnt, rev, year);
    }

    print_str("\nProject tracking...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int scl = (i % 5) + 1;
        int dck = 20 + (i * 5);
        int lnd = 15 + (i * 4);
        int cmp = 70 + (i % 30);
        int year = 2023 + (i % 2);
        is_project(type, scl, dck, lnd, cmp, year);
    }

    print_str("\nPolicy service...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int iss = 15 + (i * 4);
        int prc = 40 + (i * 10);
        int grt = 30 + (i * 8);
        int year = 2024;
        is_policy(type, cat, iss, prc, grt, year);
    }

    print_str("\nInvestment report...\n");
    is_investment_report();

    print_str("\nPark report...\n");
    is_park_report();

    print_str("\nProject report...\n");
    is_project_report();

    print_str("\nFinal state...\n");
    is_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
