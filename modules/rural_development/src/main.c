/* rural_development: Rural development service management (v1.0)
 * Rural planning, industrialization, reform, training, governance
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

#define MAX_PLANS        16
#define MAX_AGRI         14
#define MAX_REFORM       12
#define MAX_TRAIN        10
#define MAX_GOVERN       10

typedef struct {
    int    plan_id;
    int    village_id;
    int    type;
    int    area_hectares;
    int    population;
    int    budget;
    int    year;
    int    status;
    int    active;
} plan_t;

typedef struct {
    int    agri_id;
    int    type;
    int    operator_id;
    int    scale_hectares;
    int    revenue;
    int    employees;
    int    year;
    int    status;
    int    active;
} agri_business_t;

typedef struct {
    int    reform_id;
    int    village_id;
    int    type;
    int    land_transferred;
    int    collective_assets;
    int    beneficiaries;
    int    year;
    int    status;
    int    active;
} reform_t;

typedef struct {
    int    train_id;
    int    type;
    int    participants;
    int    duration_hours;
    int    completed;
    int    employed;
    int    year;
    int    active;
} training_t;

typedef struct {
    int    govern_id;
    int    village_id;
    int    council_members;
    int    meetings_held;
    int    public_services;
    int    disputes_resolved;
    int    year;
    int    active;
} governance_t;

typedef struct {
    int    n_plans;
    int    n_agri;
    int    n_reform;
    int    n_train;
    int    n_govern;
    int    total_population;
    int    total_revenue;
    int    total_transferred;
    int    total_trained;
    int    total_services;
} rd_state_t;

static plan_t plans[MAX_PLANS];
static agri_business_t agri_businesses[MAX_AGRI];
static reform_t reforms[MAX_REFORM];
static training_t trainings[MAX_TRAIN];
static governance_t governances[MAX_GOVERN];
static rd_state_t rd;

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

int rd_init(void) {
    if (initialized) return -1;
    rd.n_plans = 0; rd.n_agri = 0; rd.n_reform = 0;
    rd.n_train = 0; rd.n_govern = 0;
    rd.total_population = 0; rd.total_revenue = 0;
    rd.total_transferred = 0; rd.total_trained = 0;
    rd.total_services = 0;
    for (int i = 0; i < MAX_PLANS; i++) plans[i].active = 0;
    for (int i = 0; i < MAX_AGRI; i++) agri_businesses[i].active = 0;
    for (int i = 0; i < MAX_REFORM; i++) reforms[i].active = 0;
    for (int i = 0; i < MAX_TRAIN; i++) trainings[i].active = 0;
    for (int i = 0; i < MAX_GOVERN; i++) governances[i].active = 0;
    initialized = 1;
    print_str("[RD] Rural development initialized\n");
    return 0;
}

int rd_plan(int village, int type, int area, int population, int budget, int year) {
    if (rd.n_plans >= MAX_PLANS) return -1;
    plan_t* p = &plans[rd.n_plans];
    p->plan_id = rd.n_plans;
    p->village_id = village;
    p->type = type;
    p->area_hectares = area;
    p->population = population;
    p->budget = budget;
    p->year = year;
    p->status = 1;
    p->active = 1;
    rd.total_population += population;
    rd.n_plans++;
    print_str("[RD] Plan "); print_int(rd.n_plans - 1);
    print_str(" vil="); print_int(village);
    print_str(" type="); print_int(type);
    print_str(" area="); print_int(area); print_str("ha");
    print_str(" pop="); print_int(population);
    print_str(" bud=$"); print_int(budget); print_str("\n");
    return rd.n_plans - 1;
}

int rd_agri_business(int type, int operator_val, int scale, int revenue, int employees, int year) {
    if (rd.n_agri >= MAX_AGRI) return -1;
    agri_business_t* a = &agri_businesses[rd.n_agri];
    a->agri_id = rd.n_agri;
    a->type = type;
    a->operator_id = operator_val;
    a->scale_hectares = scale;
    a->revenue = revenue;
    a->employees = employees;
    a->year = year;
    a->status = 1;
    a->active = 1;
    rd.total_revenue += revenue;
    rd.n_agri++;
    print_str("[RD] AgriBiz "); print_int(rd.n_agri - 1);
    print_str(" type="); print_int(type);
    print_str(" op="); print_int(operator_val);
    print_str(" scale="); print_int(scale); print_str("ha");
    print_str(" rev=$"); print_int(revenue);
    print_str(" emp="); print_int(employees); print_str("\n");
    return rd.n_agri - 1;
}

int rd_reform(int village, int type, int transferred, int assets, int beneficiaries, int year) {
    if (rd.n_reform >= MAX_REFORM) return -1;
    reform_t* r = &reforms[rd.n_reform];
    r->reform_id = rd.n_reform;
    r->village_id = village;
    r->type = type;
    r->land_transferred = transferred;
    r->collective_assets = assets;
    r->beneficiaries = beneficiaries;
    r->year = year;
    r->status = 1;
    r->active = 1;
    rd.total_transferred += transferred;
    rd.n_reform++;
    print_str("[RD] Reform "); print_int(rd.n_reform - 1);
    print_str(" vil="); print_int(village);
    print_str(" type="); print_int(type);
    print_str(" trans="); print_int(transferred); print_str("ha");
    print_str(" ast=$"); print_int(assets);
    print_str(" ben="); print_int(beneficiaries); print_str("\n");
    return rd.n_reform - 1;
}

int rd_train(int type, int participants, int duration, int completed, int employed, int year) {
    if (rd.n_train >= MAX_TRAIN) return -1;
    training_t* t = &trainings[rd.n_train];
    t->train_id = rd.n_train;
    t->type = type;
    t->participants = participants;
    t->duration_hours = duration;
    t->completed = completed;
    t->employed = employed;
    t->year = year;
    t->active = 1;
    rd.total_trained += completed;
    rd.n_train++;
    print_str("[RD] Train "); print_int(rd.n_train - 1);
    print_str(" type="); print_int(type);
    print_str(" ptc="); print_int(participants);
    print_str(" dur="); print_int(duration); print_str("h");
    print_str(" cmp="); print_int(completed);
    print_str(" emp="); print_int(employed); print_str("\n");
    return rd.n_train - 1;
}

int rd_govern(int village, int council, int meetings, int services, int disputes, int year) {
    if (rd.n_govern >= MAX_GOVERN) return -1;
    governance_t* g = &governances[rd.n_govern];
    g->govern_id = rd.n_govern;
    g->village_id = village;
    g->council_members = council;
    g->meetings_held = meetings;
    g->public_services = services;
    g->disputes_resolved = disputes;
    g->year = year;
    g->active = 1;
    rd.total_services += services;
    rd.n_govern++;
    print_str("[RD] Govern "); print_int(rd.n_govern - 1);
    print_str(" vil="); print_int(village);
    print_str(" cnc="); print_int(council);
    print_str(" mtg="); print_int(meetings);
    print_str(" svc="); print_int(services);
    print_str(" dsp="); print_int(disputes); print_str("\n");
    return rd.n_govern - 1;
}

void rd_planning_report(void) {
    print_str("[RD] Planning report:\n");
    print_str("  Villages planned: "); print_int(rd.n_plans); print_str("\n");
    print_str("  Total population: "); print_int(rd.total_population); print_str("\n");
}

void rd_agriculture_report(void) {
    print_str("[RD] Agriculture report:\n");
    print_str("  Agri-businesses: "); print_int(rd.n_agri); print_str("\n");
    print_str("  Total revenue: $"); print_int(rd.total_revenue); print_str("\n");
    print_str("  Land transferred: "); print_int(rd.total_transferred); print_str(" ha\n");
}

void rd_governance_report(void) {
    print_str("[RD] Governance report:\n");
    print_str("  Villages: "); print_int(rd.n_govern); print_str("\n");
    print_str("  Public services: "); print_int(rd.total_services); print_str("\n");
    print_str("  Trained: "); print_int(rd.total_trained); print_str("\n");
}

void rd_print_state(void) {
    print_str("[RD] Pl="); print_int(rd.n_plans);
    print_str(" Ag="); print_int(rd.n_agri);
    print_str(" Rf="); print_int(rd.n_reform);
    print_str(" Tr="); print_int(rd.n_train);
    print_str(" Gv="); print_int(rd.n_govern);
    print_str("\n");
}

int main(void) {
    print_str("=== Rural Development Demo ===\n\n");
    rd_init();

    print_str("Creating village plans...\n");
    for (int i = 0; i < 16; i++) {
        int village = 100 + (i * 5);
        int type = (i % 3) + 1;
        int area = 50 + (i * 20);
        int pop = 200 + (i * 100);
        int budget = 500000 + (i * 200000);
        int year = 2019 + (i % 6);
        rd_plan(village, type, area, pop, budget, year);
    }

    print_str("\nDeveloping agri-businesses...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int op = 2000 + (i * 11);
        int scale = 10 + (i * 5);
        int rev = 200000 + (i * 100000);
        int emp = 5 + (i * 3);
        int year = 2020 + (i % 5);
        rd_agri_business(type, op, scale, rev, emp, year);
    }

    print_str("\nRural reforms...\n");
    for (int i = 0; i < 12; i++) {
        int village = 200 + (i * 7);
        int type = (i % 3) + 1;
        int trans = 5 + (i * 3);
        int assets = 100000 + (i * 50000);
        int ben = 50 + (i * 20);
        int year = 2021 + (i % 4);
        rd_reform(village, type, trans, assets, ben, year);
    }

    print_str("\nTraining programs...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int ptc = 20 + (i * 10);
        int dur = 10 + (i * 5);
        int cmp = 15 + (i * 8);
        int emp = 10 + (i * 5);
        int year = 2022 + (i % 3);
        rd_train(type, ptc, dur, cmp, emp, year);
    }

    print_str("\nGovernance activities...\n");
    for (int i = 0; i < 10; i++) {
        int village = 300 + (i * 8);
        int cnc = 5 + (i % 5);
        int mtg = 4 + (i * 2);
        int svc = 3 + (i % 6);
        int dsp = 1 + (i % 4);
        int year = 2023 + (i % 2);
        rd_govern(village, cnc, mtg, svc, dsp, year);
    }

    print_str("\nPlanning report...\n");
    rd_planning_report();

    print_str("\nAgriculture report...\n");
    rd_agriculture_report();

    print_str("\nGovernance report...\n");
    rd_governance_report();

    print_str("\nFinal state...\n");
    rd_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
