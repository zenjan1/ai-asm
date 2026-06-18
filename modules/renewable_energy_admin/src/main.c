/* renewable_energy_admin: Renewable energy administration system (v1.0)
 * Resource assessment, project development, grid connection, consumption utilization, policy support
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

#define MAX_RESOURCE     16
#define MAX_PROJECT      14
#define MAX_GRID         12
#define MAX_CONSUME      10
#define MAX_POLICY       10

typedef struct {
    int    resource_id;
    int    resource_type;
    int    resource_category;
    int    resource_survey;
    int    resource_monitor;
    int    resource_predict;
    int    year;
    int    active;
} resource_t;

typedef struct {
    int    project_id;
    int    project_type;
    int    project_category;
    int    project_plan;
    int    project_approve;
    int    project_build;
    int    year;
    int    active;
} project_t;

typedef struct {
    int    grid_id;
    int    grid_type;
    int    grid_category;
    int    grid_access;
    int    grid_dispatch;
    int    grid_safety;
    int    year;
    int    active;
} grid_t;

typedef struct {
    int    consume_id;
    int    consume_type;
    int    consume_category;
    int    consume_guarantee;
    int    cross_region;
    int    comprehensive;
    int    year;
    int    active;
} consume_t;

typedef struct {
    int    policy_id;
    int    policy_type;
    int    policy_category;
    int    subsidy;
    int    electricity_price;
    int    tax_benefit;
    int    year;
    int    active;
} policy_t;

typedef struct {
    int    n_resource;
    int    n_project;
    int    n_grid;
    int    n_consume;
    int    n_policy;
    int    total_survey;
    int    total_plan;
    int    total_access;
    int    total_guarantee;
    int    total_subsidy;
} rea_state_t;

static resource_t resources[MAX_RESOURCE];
static project_t projects[MAX_PROJECT];
static grid_t grids[MAX_GRID];
static consume_t consumes[MAX_CONSUME];
static policy_t policies[MAX_POLICY];
static rea_state_t rea;

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

int rea_init(void) {
    if (initialized) return -1;
    rea.n_resource = 0; rea.n_project = 0; rea.n_grid = 0;
    rea.n_consume = 0; rea.n_policy = 0;
    rea.total_survey = 0; rea.total_plan = 0;
    rea.total_access = 0; rea.total_guarantee = 0;
    rea.total_subsidy = 0;
    for (int i = 0; i < MAX_RESOURCE; i++) resources[i].active = 0;
    for (int i = 0; i < MAX_PROJECT; i++) projects[i].active = 0;
    for (int i = 0; i < MAX_GRID; i++) grids[i].active = 0;
    for (int i = 0; i < MAX_CONSUME; i++) consumes[i].active = 0;
    for (int i = 0; i < MAX_POLICY; i++) policies[i].active = 0;
    initialized = 1;
    print_str("[REA] Renewable energy initialized\n");
    return 0;
}

int rea_resource(int rsc_type, int cat, int survey, int monitor, int predict, int year) {
    if (rea.n_resource >= MAX_RESOURCE) return -1;
    resource_t* r = &resources[rea.n_resource];
    r->resource_id = rea.n_resource;
    r->resource_type = rsc_type;
    r->resource_category = cat;
    r->resource_survey = survey;
    r->resource_monitor = monitor;
    r->resource_predict = predict;
    r->year = year;
    r->active = 1;
    rea.total_survey += survey;
    rea.n_resource++;
    print_str("[REA] Resource "); print_int(rea.n_resource - 1);
    print_str(" type="); print_int(rsc_type);
    print_str(" cat="); print_int(cat);
    print_str(" srv="); print_int(survey);
    print_str(" mon="); print_int(monitor);
    print_str(" prd="); print_int(predict); print_str("\n");
    return rea.n_resource - 1;
}

int rea_project(int prj_type, int cat, int plan, int approve, int build, int year) {
    if (rea.n_project >= MAX_PROJECT) return -1;
    project_t* p = &projects[rea.n_project];
    p->project_id = rea.n_project;
    p->project_type = prj_type;
    p->project_category = cat;
    p->project_plan = plan;
    p->project_approve = approve;
    p->project_build = build;
    p->year = year;
    p->active = 1;
    rea.total_plan += plan;
    rea.n_project++;
    print_str("[REA] Project "); print_int(rea.n_project - 1);
    print_str(" type="); print_int(prj_type);
    print_str(" cat="); print_int(cat);
    print_str(" pln="); print_int(plan);
    print_str(" apr="); print_int(approve);
    print_str(" bld="); print_int(build); print_str("\n");
    return rea.n_project - 1;
}

int rea_grid(int grd_type, int cat, int access, int dispatch, int safety, int year) {
    if (rea.n_grid >= MAX_GRID) return -1;
    grid_t* g = &grids[rea.n_grid];
    g->grid_id = rea.n_grid;
    g->grid_type = grd_type;
    g->grid_category = cat;
    g->grid_access = access;
    g->grid_dispatch = dispatch;
    g->grid_safety = safety;
    g->year = year;
    g->active = 1;
    rea.total_access += access;
    rea.n_grid++;
    print_str("[REA] Grid "); print_int(rea.n_grid - 1);
    print_str(" type="); print_int(grd_type);
    print_str(" cat="); print_int(cat);
    print_str(" acc="); print_int(access);
    print_str(" dsp="); print_int(dispatch);
    print_str(" sft="); print_int(safety); print_str("\n");
    return rea.n_grid - 1;
}

int rea_consume(int csm_type, int cat, int guarantee, int cross_reg, int compreh, int year) {
    if (rea.n_consume >= MAX_CONSUME) return -1;
    consume_t* c = &consumes[rea.n_consume];
    c->consume_id = rea.n_consume;
    c->consume_type = csm_type;
    c->consume_category = cat;
    c->consume_guarantee = guarantee;
    c->cross_region = cross_reg;
    c->comprehensive = compreh;
    c->year = year;
    c->active = 1;
    rea.total_guarantee += guarantee;
    rea.n_consume++;
    print_str("[REA] Consume "); print_int(rea.n_consume - 1);
    print_str(" type="); print_int(csm_type);
    print_str(" cat="); print_int(cat);
    print_str(" gnt="); print_int(guarantee);
    print_str(" crs="); print_int(cross_reg);
    print_str(" cmp="); print_int(compreh); print_str("\n");
    return rea.n_consume - 1;
}

int rea_policy(int plc_type, int cat, int sub, int elec_price, int tax, int year) {
    if (rea.n_policy >= MAX_POLICY) return -1;
    policy_t* p = &policies[rea.n_policy];
    p->policy_id = rea.n_policy;
    p->policy_type = plc_type;
    p->policy_category = cat;
    p->subsidy = sub;
    p->electricity_price = elec_price;
    p->tax_benefit = tax;
    p->year = year;
    p->active = 1;
    rea.total_subsidy += sub;
    rea.n_policy++;
    print_str("[REA] Policy "); print_int(rea.n_policy - 1);
    print_str(" type="); print_int(plc_type);
    print_str(" cat="); print_int(cat);
    print_str(" sub="); print_int(sub);
    print_str(" elp="); print_int(elec_price);
    print_str(" tax="); print_int(tax); print_str("\n");
    return rea.n_policy - 1;
}

void rea_resource_report(void) {
    print_str("[REA] Resource report:\n");
    print_str("  Resource assessment categories: "); print_int(rea.n_resource); print_str("\n");
    print_str("  Total resource survey: "); print_int(rea.total_survey); print_str("\n");
}

void rea_project_report(void) {
    print_str("[REA] Project report:\n");
    print_str("  Project development categories: "); print_int(rea.n_project); print_str("\n");
    print_str("  Total project plans: "); print_int(rea.total_plan); print_str("\n");
}

void rea_policy_report(void) {
    print_str("[REA] Policy report:\n");
    print_str("  Grid connection categories: "); print_int(rea.n_grid); print_str("\n");
    print_str("  Total grid access: "); print_int(rea.total_access); print_str("\n");
    print_str("  Consumption categories: "); print_int(rea.n_consume); print_str("\n");
    print_str("  Total consumption guarantee: "); print_int(rea.total_guarantee); print_str("\n");
    print_str("  Policy categories: "); print_int(rea.n_policy); print_str("\n");
    print_str("  Total subsidy: "); print_int(rea.total_subsidy); print_str("\n");
}

void rea_print_state(void) {
    print_str("[REA] Rs="); print_int(rea.n_resource);
    print_str(" Pj="); print_int(rea.n_project);
    print_str(" Gr="); print_int(rea.n_grid);
    print_str(" Cn="); print_int(rea.n_consume);
    print_str(" Pl="); print_int(rea.n_policy);
    print_str("\n");
}

int main(void) {
    print_str("=== Renewable Energy Admin Demo ===\n\n");
    rea_init();

    print_str("Resource assessment...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int srv = 60 + (i * 15);
        int mon = 40 + (i * 10);
        int prd = 25 + (i * 6);
        int year = 2020 + (i % 5);
        rea_resource(type, cat, srv, mon, prd, year);
    }

    print_str("\nProject development...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pln = 50 + (i * 12);
        int apr = 35 + (i * 8);
        int bld = 22 + (i * 5);
        int year = 2021 + (i % 4);
        rea_project(type, cat, pln, apr, bld, year);
    }

    print_str("\nGrid connection...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int acc = 45 + (i * 11);
        int dsp = 30 + (i * 7);
        int sft = 20 + (i * 5);
        int year = 2022 + (i % 3);
        rea_grid(type, cat, acc, dsp, sft, year);
    }

    print_str("\nConsumption utilization...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int gnt = 38 + (i * 9);
        int crs = 25 + (i * 6);
        int cmp = 18 + (i * 4);
        int year = 2023 + (i % 2);
        rea_consume(type, cat, gnt, crs, cmp, year);
    }

    print_str("\nPolicy support...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int sub = 30 + (i * 7);
        int elp = 22 + (i * 5);
        int tax = 15 + (i * 3);
        int year = 2024;
        rea_policy(type, cat, sub, elp, tax, year);
    }

    print_str("\nResource report...\n");
    rea_resource_report();

    print_str("\nProject report...\n");
    rea_project_report();

    print_str("\nPolicy report...\n");
    rea_policy_report();

    print_str("\nFinal state...\n");
    rea_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
