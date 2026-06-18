/* mining_authority: Mining authority management (v1.0)
 * Mining rights, supervision, exploration, statistics, reclamation
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

#define MAX_RIGHTS       16
#define MAX_SUPERV       14
#define MAX_EXPLOR       12
#define MAX_STATS        10
#define MAX_RECLAIM      10

typedef struct {
    int    right_id;
    int    type;
    int    holder_id;
    int    mineral;
    int    area_hectares;
    int    reserve;
    int    year_granted;
    int    expiry_year;
    int    active;
} mining_right_t;

typedef struct {
    int    superv_id;
    int    mine_id;
    int    inspector_id;
    int    safety_score;
    int    env_score;
    int    violations;
    int    year;
    int    status;
    int    active;
} supervision_t;

typedef struct {
    int    explor_id;
    int    region_id;
    int    surveyor_id;
    int    mineral;
    int    estimated_reserve;
    int    confidence;
    int    year;
    int    status;
    int    active;
} exploration_t;

typedef struct {
    int    stat_id;
    int    mine_id;
    int    production_tons;
    int    revenue;
    int    employees;
    int    taxes_paid;
    int    year;
    int    active;
} stat_t;

typedef struct {
    int    reclaim_id;
    int    mine_id;
    int    area_hectares;
    int    cost;
    int    progress_pct;
    int    completion_year;
    int    year;
    int    status;
    int    active;
} reclamation_t;

typedef struct {
    int    n_rights;
    int    n_superv;
    int    n_explor;
    int    n_stats;
    int    n_reclaim;
    int    total_production;
    int    total_revenue;
    int    total_taxes;
    int    total_reclaim_area;
    int    total_employees;
} ma_state_t;

static mining_right_t rights[MAX_RIGHTS];
static supervision_t supervisions[MAX_SUPERV];
static exploration_t explorations[MAX_EXPLOR];
static stat_t stats[MAX_STATS];
static reclamation_t reclamations[MAX_RECLAIM];
static ma_state_t ma;

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

int ma_init(void) {
    if (initialized) return -1;
    ma.n_rights = 0; ma.n_superv = 0; ma.n_explor = 0;
    ma.n_stats = 0; ma.n_reclaim = 0;
    ma.total_production = 0; ma.total_revenue = 0;
    ma.total_taxes = 0; ma.total_reclaim_area = 0;
    ma.total_employees = 0;
    for (int i = 0; i < MAX_RIGHTS; i++) rights[i].active = 0;
    for (int i = 0; i < MAX_SUPERV; i++) supervisions[i].active = 0;
    for (int i = 0; i < MAX_EXPLOR; i++) explorations[i].active = 0;
    for (int i = 0; i < MAX_STATS; i++) stats[i].active = 0;
    for (int i = 0; i < MAX_RECLAIM; i++) reclamations[i].active = 0;
    initialized = 1;
    print_str("[MA] Mining authority initialized\n");
    return 0;
}

int ma_grant_right(int type, int holder, int mineral, int area, int reserve, int year, int expiry) {
    if (ma.n_rights >= MAX_RIGHTS) return -1;
    mining_right_t* r = &rights[ma.n_rights];
    r->right_id = ma.n_rights;
    r->type = type;
    r->holder_id = holder;
    r->mineral = mineral;
    r->area_hectares = area;
    r->reserve = reserve;
    r->year_granted = year;
    r->expiry_year = expiry;
    r->active = 1;
    ma.n_rights++;
    print_str("[MA] Right "); print_int(ma.n_rights - 1);
    print_str(" type="); print_int(type);
    print_str(" hld="); print_int(holder);
    print_str(" min="); print_int(mineral);
    print_str(" area="); print_int(area); print_str("ha");
    print_str(" rsv="); print_int(reserve); print_str("\n");
    return ma.n_rights - 1;
}

int ma_supervise(int mine, int inspector, int safety, int env, int violations, int year) {
    if (ma.n_superv >= MAX_SUPERV) return -1;
    supervision_t* s = &supervisions[ma.n_superv];
    s->superv_id = ma.n_superv;
    s->mine_id = mine;
    s->inspector_id = inspector;
    s->safety_score = safety;
    s->env_score = env;
    s->violations = violations;
    s->year = year;
    s->status = 1;
    s->active = 1;
    ma.n_superv++;
    print_str("[MA] Supervision "); print_int(ma.n_superv - 1);
    print_str(" mine="); print_int(mine);
    print_str(" insp="); print_int(inspector);
    print_str(" safe="); print_int(safety);
    print_str(" env="); print_int(env);
    print_str(" viol="); print_int(violations); print_str("\n");
    return ma.n_superv - 1;
}

int ma_explore(int region, int surveyor, int mineral, int reserve, int confidence, int year) {
    if (ma.n_explor >= MAX_EXPLOR) return -1;
    exploration_t* e = &explorations[ma.n_explor];
    e->explor_id = ma.n_explor;
    e->region_id = region;
    e->surveyor_id = surveyor;
    e->mineral = mineral;
    e->estimated_reserve = reserve;
    e->confidence = confidence;
    e->year = year;
    e->status = 1;
    e->active = 1;
    ma.n_explor++;
    print_str("[MA] Explor "); print_int(ma.n_explor - 1);
    print_str(" reg="); print_int(region);
    print_str(" surv="); print_int(surveyor);
    print_str(" min="); print_int(mineral);
    print_str(" rsv="); print_int(reserve);
    print_str(" conf="); print_int(confidence); print_str("%\n");
    return ma.n_explor - 1;
}

int ma_record_stat(int mine, int production, int revenue, int employees, int taxes, int year) {
    if (ma.n_stats >= MAX_STATS) return -1;
    stat_t* s = &stats[ma.n_stats];
    s->stat_id = ma.n_stats;
    s->mine_id = mine;
    s->production_tons = production;
    s->revenue = revenue;
    s->employees = employees;
    s->taxes_paid = taxes;
    s->year = year;
    s->active = 1;
    ma.total_production += production;
    ma.total_revenue += revenue;
    ma.total_taxes += taxes;
    ma.total_employees += employees;
    ma.n_stats++;
    print_str("[MA] Stat "); print_int(ma.n_stats - 1);
    print_str(" mine="); print_int(mine);
    print_str(" prod="); print_int(production); print_str("t");
    print_str(" rev=$"); print_int(revenue);
    print_str(" emp="); print_int(employees);
    print_str(" tax=$"); print_int(taxes); print_str("\n");
    return ma.n_stats - 1;
}

int ma_reclaim(int mine, int area, int cost, int progress, int completion_year, int year) {
    if (ma.n_reclaim >= MAX_RECLAIM) return -1;
    reclamation_t* r = &reclamations[ma.n_reclaim];
    r->reclaim_id = ma.n_reclaim;
    r->mine_id = mine;
    r->area_hectares = area;
    r->cost = cost;
    r->progress_pct = progress;
    r->completion_year = completion_year;
    r->year = year;
    r->status = 1;
    r->active = 1;
    ma.total_reclaim_area += area;
    ma.n_reclaim++;
    print_str("[MA] Reclaim "); print_int(ma.n_reclaim - 1);
    print_str(" mine="); print_int(mine);
    print_str(" area="); print_int(area); print_str("ha");
    print_str(" cost=$"); print_int(cost);
    print_str(" prog="); print_int(progress); print_str("%\n");
    return ma.n_reclaim - 1;
}

void ma_rights_report(void) {
    print_str("[MA] Rights report:\n");
    print_str("  Mining rights: "); print_int(ma.n_rights); print_str("\n");
    print_str("  Explorations: "); print_int(ma.n_explor); print_str("\n");
}

void ma_production_report(void) {
    print_str("[MA] Production report:\n");
    print_str("  Total production: "); print_int(ma.total_production); print_str(" tons\n");
    print_str("  Total revenue: $"); print_int(ma.total_revenue); print_str("\n");
    print_str("  Total taxes: $"); print_int(ma.total_taxes); print_str("\n");
}

void ma_reclamation_report(void) {
    print_str("[MA] Reclamation report:\n");
    print_str("  Sites: "); print_int(ma.n_reclaim); print_str("\n");
    print_str("  Total area: "); print_int(ma.total_reclaim_area); print_str(" ha\n");
}

void ma_print_state(void) {
    print_str("[MA] Rt="); print_int(ma.n_rights);
    print_str(" Sp="); print_int(ma.n_superv);
    print_str(" Ex="); print_int(ma.n_explor);
    print_str(" St="); print_int(ma.n_stats);
    print_str(" Rc="); print_int(ma.n_reclaim);
    print_str("\n");
}

int main(void) {
    print_str("=== Mining Authority Demo ===\n\n");
    ma_init();

    print_str("Granting mining rights...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 3) + 1;
        int holder = 1000 + (i * 11);
        int mineral = (i % 6) + 1;
        int area = 100 + (i * 50);
        int reserve = 10000 + (i * 5000);
        int year = 2015 + (i % 8);
        int expiry = year + 10 + (i % 5);
        ma_grant_right(type, holder, mineral, area, reserve, year, expiry);
    }

    print_str("\nSupervising mines...\n");
    for (int i = 0; i < 14; i++) {
        int mine = i % 16;
        int insp = 500 + (i % 8);
        int safe = 60 + (i * 3);
        int env = 55 + (i * 3);
        int viol = i % 5;
        int year = 2022 + (i % 3);
        ma_supervise(mine, insp, safe, env, viol, year);
    }

    print_str("\nConducting exploration...\n");
    for (int i = 0; i < 12; i++) {
        int reg = (i % 8) + 1;
        int surv = 600 + (i * 5);
        int mineral = (i % 6) + 1;
        int reserve = 50000 + (i * 10000);
        int conf = 40 + (i * 5);
        int year = 2020 + (i % 5);
        ma_explore(reg, surv, mineral, reserve, conf, year);
    }

    print_str("\nRecording statistics...\n");
    for (int i = 0; i < 10; i++) {
        int mine = i % 16;
        int prod = 5000 + (i * 2000);
        int rev = 1000000 + (i * 500000);
        int emp = 50 + (i * 20);
        int tax = 100000 + (i * 50000);
        int year = 2021 + (i % 4);
        ma_record_stat(mine, prod, rev, emp, tax, year);
    }

    print_str("\nReclamation projects...\n");
    for (int i = 0; i < 10; i++) {
        int mine = i % 16;
        int area = 20 + (i * 10);
        int cost = 200000 + (i * 100000);
        int prog = 20 + (i * 8);
        int comp = 2025 + (i % 5);
        int year = 2023 + (i % 2);
        ma_reclaim(mine, area, cost, prog, comp, year);
    }

    print_str("\nRights report...\n");
    ma_rights_report();

    print_str("\nProduction report...\n");
    ma_production_report();

    print_str("\nReclamation report...\n");
    ma_reclamation_report();

    print_str("\nFinal state...\n");
    ma_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
