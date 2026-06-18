/* planning_commission: Development planning and management (v1.0)
 * Five-year plans, projects, budgets, industrial policy, regional development
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

#define MAX_PLANS      10
#define MAX_PROJECTS   18
#define MAX_BUDGETS    14
#define MAX_POLICIES   12
#define MAX_REGIONS    10

typedef struct {
    int    plan_id;
    int    type;
    int    period_start;
    int    period_end;
    int    gdp_target;
    int    growth_target;
    int    investment_target;
    int    progress;
    int    active;
} plan_t;

typedef struct {
    int    project_id;
    int    type;
    int    zone;
    int    budget;
    int    spent;
    int    progress;
    int    status;
    int    year_started;
    int    year_expected;
    int    active;
} project_t;

typedef struct {
    int    budget_id;
    int    zone;
    int    revenue;
    int    expenditure;
    int    deficit;
    int    category;
    int    year;
    int    approved;
    int    active;
} budget_t;

typedef struct {
    int    policy_id;
    int    type;
    int    sector;
    int    target_growth;
    int    investment;
    int    jobs_created;
    int    year;
    int    active;
} policy_t;

typedef struct {
    int    region_id;
    int    zone;
    int    gdp;
    int    population;
    int    per_capita;
    int    growth_rate;
    int    urbanization;
    int    year;
    int    active;
} region_t;

typedef struct {
    int    n_plans;
    int    n_projects;
    int    n_budgets;
    int    n_policies;
    int    n_regions;
    int    total_budget;
    int    total_spent;
    int    total_revenue;
    int    total_expenditure;
    int    total_jobs;
    int    total_gdp;
} pc_state_t;

static plan_t plans[MAX_PLANS];
static project_t projects[MAX_PROJECTS];
static budget_t budgets[MAX_BUDGETS];
static policy_t policies[MAX_POLICIES];
static region_t regions[MAX_REGIONS];
static pc_state_t pc;

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

int pc_init(void) {
    if (initialized) return -1;
    pc.n_plans = 0; pc.n_projects = 0; pc.n_budgets = 0;
    pc.n_policies = 0; pc.n_regions = 0;
    pc.total_budget = 0; pc.total_spent = 0;
    pc.total_revenue = 0; pc.total_expenditure = 0;
    pc.total_jobs = 0; pc.total_gdp = 0;
    for (int i = 0; i < MAX_PLANS; i++) plans[i].active = 0;
    for (int i = 0; i < MAX_PROJECTS; i++) projects[i].active = 0;
    for (int i = 0; i < MAX_BUDGETS; i++) budgets[i].active = 0;
    for (int i = 0; i < MAX_POLICIES; i++) policies[i].active = 0;
    for (int i = 0; i < MAX_REGIONS; i++) regions[i].active = 0;
    initialized = 1;
    print_str("[PC] Planning commission initialized\n");
    return 0;
}

int pc_add_plan(int type, int start, int end, int gdp_target, int growth, int investment) {
    if (pc.n_plans >= MAX_PLANS) return -1;
    plan_t* p = &plans[pc.n_plans];
    p->plan_id = pc.n_plans;
    p->type = type;
    p->period_start = start;
    p->period_end = end;
    p->gdp_target = gdp_target;
    p->growth_target = growth;
    p->investment_target = investment;
    p->progress = 0;
    p->active = 1;
    pc.n_plans++;
    print_str("[PC] Plan "); print_int(pc.n_plans - 1);
    print_str(" type="); print_int(type);
    print_str(" "); print_int(start); print_str("-"); print_int(end);
    print_str(" gdp="); print_int(gdp_target);
    print_str(" growth="); print_int(growth); print_str("%\n");
    return pc.n_plans - 1;
}

int pc_update_plan(int plan_id, int progress) {
    if (plan_id >= pc.n_plans) return -1;
    plans[plan_id].progress = progress;
    print_str("[PC] Update Pn"); print_int(plan_id);
    print_str(" prog="); print_int(progress); print_str("%\n");
    return 0;
}

int pc_add_project(int type, int zone, int budget, int year_started, int year_expected) {
    if (pc.n_projects >= MAX_PROJECTS) return -1;
    project_t* p = &projects[pc.n_projects];
    p->project_id = pc.n_projects;
    p->type = type;
    p->zone = zone;
    p->budget = budget;
    p->spent = 0;
    p->progress = 0;
    p->status = 1;
    p->year_started = year_started;
    p->year_expected = year_expected;
    p->active = 1;
    pc.total_budget += budget;
    pc.n_projects++;
    print_str("[PC] Project "); print_int(pc.n_projects - 1);
    print_str(" type="); print_int(type);
    print_str(" zone="); print_int(zone);
    print_str(" budget=$"); print_int(budget);
    print_str(" "); print_int(year_started); print_str("-"); print_int(year_expected); print_str("\n");
    return pc.n_projects - 1;
}

int pc_spend_project(int project_id, int amount, int progress) {
    if (project_id >= pc.n_projects) return -1;
    projects[project_id].spent += amount;
    projects[project_id].progress = progress;
    pc.total_spent += amount;
    print_str("[PC] Spend Pj"); print_int(project_id);
    print_str(" amt=$"); print_int(amount);
    print_str(" prog="); print_int(progress); print_str("%\n");
    return 0;
}

int pc_complete_project(int project_id) {
    if (project_id >= pc.n_projects) return -1;
    projects[project_id].status = 2;
    projects[project_id].progress = 100;
    print_str("[PC] Complete Pj"); print_int(project_id); print_str("\n");
    return 0;
}

int pc_add_budget(int zone, int revenue, int expenditure, int category, int year) {
    if (pc.n_budgets >= MAX_BUDGETS) return -1;
    budget_t* b = &budgets[pc.n_budgets];
    b->budget_id = pc.n_budgets;
    b->zone = zone;
    b->revenue = revenue;
    b->expenditure = expenditure;
    b->deficit = expenditure - revenue;
    b->category = category;
    b->year = year;
    b->approved = 1;
    b->active = 1;
    pc.total_revenue += revenue;
    pc.total_expenditure += expenditure;
    pc.n_budgets++;
    print_str("[PC] Budget "); print_int(pc.n_budgets - 1);
    print_str(" zone="); print_int(zone);
    print_str(" rev=$"); print_int(revenue);
    print_str(" exp=$"); print_int(expenditure);
    print_str(" cat="); print_int(category); print_str("\n");
    return pc.n_budgets - 1;
}

int pc_add_policy(int type, int sector, int target_growth, int investment, int jobs, int year) {
    if (pc.n_policies >= MAX_POLICIES) return -1;
    policy_t* p = &policies[pc.n_policies];
    p->policy_id = pc.n_policies;
    p->type = type;
    p->sector = sector;
    p->target_growth = target_growth;
    p->investment = investment;
    p->jobs_created = jobs;
    p->year = year;
    p->active = 1;
    pc.total_jobs += jobs;
    pc.n_policies++;
    print_str("[PC] Policy "); print_int(pc.n_policies - 1);
    print_str(" type="); print_int(type);
    print_str(" sector="); print_int(sector);
    print_str(" growth="); print_int(target_growth); print_str("%");
    print_str(" jobs="); print_int(jobs); print_str("\n");
    return pc.n_policies - 1;
}

int pc_add_region(int zone, int gdp, int population, int per_capita, int growth, int urban, int year) {
    if (pc.n_regions >= MAX_REGIONS) return -1;
    region_t* r = &regions[pc.n_regions];
    r->region_id = pc.n_regions;
    r->zone = zone;
    r->gdp = gdp;
    r->population = population;
    r->per_capita = per_capita;
    r->growth_rate = growth;
    r->urbanization = urban;
    r->year = year;
    r->active = 1;
    pc.total_gdp += gdp;
    pc.n_regions++;
    print_str("[PC] Region "); print_int(pc.n_regions - 1);
    print_str(" zone="); print_int(zone);
    print_str(" gdp=$"); print_int(gdp);
    print_str(" pop="); print_int(population);
    print_str(" pc=$"); print_int(per_capita);
    print_str(" urb="); print_int(urban); print_str("%\n");
    return pc.n_regions - 1;
}

void pc_plan_report(void) {
    print_str("[PC] Plan report:\n");
    print_str("  Plans: "); print_int(pc.n_plans); print_str("\n");
    for (int i = 0; i < pc.n_plans; i++) {
        plan_t* p = &plans[i];
        print_str("  Pn"); print_int(i);
        print_str(" T"); print_int(p->type);
        print_str(" "); print_int(p->period_start); print_str("-"); print_int(p->period_end);
        print_str(" prog="); print_int(p->progress); print_str("%\n");
    }
}

void pc_project_report(void) {
    print_str("[PC] Project report:\n");
    print_str("  Projects: "); print_int(pc.n_projects); print_str("\n");
    print_str("  Total budget: $"); print_int(pc.total_budget); print_str("\n");
    print_str("  Total spent: $"); print_int(pc.total_spent); print_str("\n");
}

void pc_budget_report(void) {
    print_str("[PC] Budget report:\n");
    print_str("  Budgets: "); print_int(pc.n_budgets); print_str("\n");
    print_str("  Revenue: $"); print_int(pc.total_revenue); print_str("\n");
    print_str("  Expenditure: $"); print_int(pc.total_expenditure); print_str("\n");
}

void pc_print_state(void) {
    print_str("[PC] Pn="); print_int(pc.n_plans);
    print_str(" Pj="); print_int(pc.n_projects);
    print_str(" Bd="); print_int(pc.n_budgets);
    print_str(" Po="); print_int(pc.n_policies);
    print_str(" Rg="); print_int(pc.n_regions);
    print_str("\n");
}

int main(void) {
    print_str("=== Planning Commission Demo ===\n\n");
    pc_init();

    print_str("Adding plans...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 3) + 1;
        int start = 2001 + (i * 5);
        int end = start + 4;
        int gdp = 10000000 + (i * 2000000);
        int growth = 6 + (i % 5);
        int invest = 3000000 + (i * 500000);
        pc_add_plan(type, start, end, gdp, growth, invest);
    }

    print_str("\nUpdating plans...\n");
    for (int i = 0; i < 10; i++) {
        int prog = 50 + (i * 5);
        pc_update_plan(i, prog);
    }

    print_str("\nAdding projects...\n");
    for (int i = 0; i < 18; i++) {
        int type = (i % 5) + 1;
        int zone = (i % 4) + 1;
        int budget = 500000 + (i * 200000);
        int start = 2020 + (i % 5);
        int expected = start + 2 + (i % 3);
        pc_add_project(type, zone, budget, start, expected);
    }

    print_str("\nSpending on projects...\n");
    for (int i = 0; i < 18; i++) {
        int amount = 100000 + (i * 50000);
        int prog = 30 + (i * 3);
        pc_spend_project(i, amount, prog);
    }

    print_str("\nCompleting projects...\n");
    for (int i = 0; i < 12; i++) {
        pc_complete_project(i);
    }

    print_str("\nAdding budgets...\n");
    for (int i = 0; i < 14; i++) {
        int zone = (i % 4) + 1;
        int rev = 2000000 + (i * 500000);
        int exp = 1800000 + (i * 400000);
        int cat = (i % 5) + 1;
        int year = 2020 + (i % 5);
        pc_add_budget(zone, rev, exp, cat, year);
    }

    print_str("\nAdding policies...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 3) + 1;
        int sector = (i % 5) + 1;
        int growth = 5 + (i % 8);
        int invest = 1000000 + (i * 300000);
        int jobs = 5000 + (i * 2000);
        int year = 2020 + (i % 5);
        pc_add_policy(type, sector, growth, invest, jobs, year);
    }

    print_str("\nAdding regions...\n");
    for (int i = 0; i < 10; i++) {
        int zone = (i % 4) + 1;
        int gdp = 5000000 + (i * 1000000);
        int pop = 2000000 + (i * 500000);
        int pc_val = gdp / pop;
        int growth = 4 + (i % 6);
        int urban = 50 + (i * 3);
        int year = 2024;
        pc_add_region(zone, gdp, pop, pc_val, growth, urban, year);
    }

    print_str("\nPlan report...\n");
    pc_plan_report();

    print_str("\nProject report...\n");
    pc_project_report();

    print_str("\nBudget report...\n");
    pc_budget_report();

    print_str("\nFinal state...\n");
    pc_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
