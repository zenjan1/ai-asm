/* smart_manufacturing: Smart manufacturing administration system (v1.0)
 * Smart factory, smart production, smart service, smart management, smart collaboration
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

#define MAX_FACTORY      16
#define MAX_PRODUCTION   14
#define MAX_SERVICE      12
#define MAX_MANAGEMENT   10
#define MAX_COLLABORATION 10

typedef struct {
    int    factory_id;
    int    factory_type;
    int    factory_category;
    int    factory_planning;
    int    production_line;
    int    smart_equipment;
    int    year;
    int    active;
} factory_t;

typedef struct {
    int    production_id;
    int    production_type;
    int    production_category;
    int    smart_scheduling;
    int    smart_inspection;
    int    smart_logistics;
    int    year;
    int    active;
} production_t;

typedef struct {
    int    service_id;
    int    service_type;
    int    service_category;
    int    remote_maintenance;
    int    predictive_maintenance;
    int    customization;
    int    year;
    int    active;
} service_t;

typedef struct {
    int    mgmt_id;
    int    mgmt_type;
    int    mgmt_category;
    int    digital_management;
    int    visual_monitoring;
    int    smart_decision;
    int    year;
    int    active;
} mgmt_t;

typedef struct {
    int    collab_id;
    int    collab_type;
    int    collab_category;
    int    supply_chain_collab;
    int    industry_collab;
    int    innovation_collab;
    int    year;
    int    active;
} collab_t;

typedef struct {
    int    n_factory;
    int    n_production;
    int    n_service;
    int    n_mgmt;
    int    n_collab;
    int    total_planning;
    int    total_scheduling;
    int    total_maintenance;
    int    total_digital;
    int    total_supply;
} sm_state_t;

static factory_t factories[MAX_FACTORY];
static production_t productions[MAX_PRODUCTION];
static service_t services[MAX_SERVICE];
static mgmt_t mgmts[MAX_MANAGEMENT];
static collab_t collabs[MAX_COLLABORATION];
static sm_state_t sm;

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

int sm_init(void) {
    if (initialized) return -1;
    sm.n_factory = 0; sm.n_production = 0; sm.n_service = 0;
    sm.n_mgmt = 0; sm.n_collab = 0;
    sm.total_planning = 0; sm.total_scheduling = 0;
    sm.total_maintenance = 0; sm.total_digital = 0;
    sm.total_supply = 0;
    for (int i = 0; i < MAX_FACTORY; i++) factories[i].active = 0;
    for (int i = 0; i < MAX_PRODUCTION; i++) productions[i].active = 0;
    for (int i = 0; i < MAX_SERVICE; i++) services[i].active = 0;
    for (int i = 0; i < MAX_MANAGEMENT; i++) mgmts[i].active = 0;
    for (int i = 0; i < MAX_COLLABORATION; i++) collabs[i].active = 0;
    initialized = 1;
    print_str("[SM] Smart manufacturing initialized\n");
    return 0;
}

int sm_factory(int fct_type, int cat, int planning, int line, int equipment, int year) {
    if (sm.n_factory >= MAX_FACTORY) return -1;
    factory_t* f = &factories[sm.n_factory];
    f->factory_id = sm.n_factory;
    f->factory_type = fct_type;
    f->factory_category = cat;
    f->factory_planning = planning;
    f->production_line = line;
    f->smart_equipment = equipment;
    f->year = year;
    f->active = 1;
    sm.total_planning += planning;
    sm.n_factory++;
    print_str("[SM] Factory "); print_int(sm.n_factory - 1);
    print_str(" type="); print_int(fct_type);
    print_str(" cat="); print_int(cat);
    print_str(" pln="); print_int(planning);
    print_str(" lin="); print_int(line);
    print_str(" eqp="); print_int(equipment); print_str("\n");
    return sm.n_factory - 1;
}

int sm_production(int prd_type, int cat, int scheduling, int inspection, int logistics, int year) {
    if (sm.n_production >= MAX_PRODUCTION) return -1;
    production_t* p = &productions[sm.n_production];
    p->production_id = sm.n_production;
    p->production_type = prd_type;
    p->production_category = cat;
    p->smart_scheduling = scheduling;
    p->smart_inspection = inspection;
    p->smart_logistics = logistics;
    p->year = year;
    p->active = 1;
    sm.total_scheduling += scheduling;
    sm.n_production++;
    print_str("[SM] Production "); print_int(sm.n_production - 1);
    print_str(" type="); print_int(prd_type);
    print_str(" cat="); print_int(cat);
    print_str(" sch="); print_int(scheduling);
    print_str(" isp="); print_int(inspection);
    print_str(" lgs="); print_int(logistics); print_str("\n");
    return sm.n_production - 1;
}

int sm_service(int svc_type, int cat, int remote, int predictive, int custom, int year) {
    if (sm.n_service >= MAX_SERVICE) return -1;
    service_t* s = &services[sm.n_service];
    s->service_id = sm.n_service;
    s->service_type = svc_type;
    s->service_category = cat;
    s->remote_maintenance = remote;
    s->predictive_maintenance = predictive;
    s->customization = custom;
    s->year = year;
    s->active = 1;
    sm.total_maintenance += remote;
    sm.n_service++;
    print_str("[SM] Service "); print_int(sm.n_service - 1);
    print_str(" type="); print_int(svc_type);
    print_str(" cat="); print_int(cat);
    print_str(" rmt="); print_int(remote);
    print_str(" pdt="); print_int(predictive);
    print_str(" csm="); print_int(custom); print_str("\n");
    return sm.n_service - 1;
}

int sm_mgmt(int mg_type, int cat, int digital, int visual, int decision, int year) {
    if (sm.n_mgmt >= MAX_MANAGEMENT) return -1;
    mgmt_t* m = &mgmts[sm.n_mgmt];
    m->mgmt_id = sm.n_mgmt;
    m->mgmt_type = mg_type;
    m->mgmt_category = cat;
    m->digital_management = digital;
    m->visual_monitoring = visual;
    m->smart_decision = decision;
    m->year = year;
    m->active = 1;
    sm.total_digital += digital;
    sm.n_mgmt++;
    print_str("[SM] Mgmt "); print_int(sm.n_mgmt - 1);
    print_str(" type="); print_int(mg_type);
    print_str(" cat="); print_int(cat);
    print_str(" dgt="); print_int(digital);
    print_str(" vsl="); print_int(visual);
    print_str(" dcs="); print_int(decision); print_str("\n");
    return sm.n_mgmt - 1;
}

int sm_collab(int cl_type, int cat, int supply, int industry, int innovation, int year) {
    if (sm.n_collab >= MAX_COLLABORATION) return -1;
    collab_t* c = &collabs[sm.n_collab];
    c->collab_id = sm.n_collab;
    c->collab_type = cl_type;
    c->collab_category = cat;
    c->supply_chain_collab = supply;
    c->industry_collab = industry;
    c->innovation_collab = innovation;
    c->year = year;
    c->active = 1;
    sm.total_supply += supply;
    sm.n_collab++;
    print_str("[SM] Collab "); print_int(sm.n_collab - 1);
    print_str(" type="); print_int(cl_type);
    print_str(" cat="); print_int(cat);
    print_str(" scl="); print_int(supply);
    print_str(" icl="); print_int(industry);
    print_str(" ikl="); print_int(innovation); print_str("\n");
    return sm.n_collab - 1;
}

void sm_factory_report(void) {
    print_str("[SM] Factory report:\n");
    print_str("  Smart factory categories: "); print_int(sm.n_factory); print_str("\n");
    print_str("  Total factory planning: "); print_int(sm.total_planning); print_str("\n");
}

void sm_production_report(void) {
    print_str("[SM] Production report:\n");
    print_str("  Smart production categories: "); print_int(sm.n_production); print_str("\n");
    print_str("  Total smart scheduling: "); print_int(sm.total_scheduling); print_str("\n");
}

void sm_collab_report(void) {
    print_str("[SM] Collab report:\n");
    print_str("  Smart service categories: "); print_int(sm.n_service); print_str("\n");
    print_str("  Total remote maintenance: "); print_int(sm.total_maintenance); print_str("\n");
    print_str("  Smart management categories: "); print_int(sm.n_mgmt); print_str("\n");
    print_str("  Total digital management: "); print_int(sm.total_digital); print_str("\n");
    print_str("  Smart collaboration categories: "); print_int(sm.n_collab); print_str("\n");
    print_str("  Total supply chain collab: "); print_int(sm.total_supply); print_str("\n");
}

void sm_print_state(void) {
    print_str("[SM] Fc="); print_int(sm.n_factory);
    print_str(" Pd="); print_int(sm.n_production);
    print_str(" Sv="); print_int(sm.n_service);
    print_str(" Mg="); print_int(sm.n_mgmt);
    print_str(" Cl="); print_int(sm.n_collab);
    print_str("\n");
}

int main(void) {
    print_str("=== Smart Manufacturing Demo ===\n\n");
    sm_init();

    print_str("Smart factory...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int pln = 10 + (i * 3);
        int lin = 8 + (i * 2);
        int eqp = 20 + (i * 5);
        int year = 2020 + (i % 5);
        sm_factory(type, cat, pln, lin, eqp, year);
    }

    print_str("\nSmart production...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int sch = 50 + (i * 12);
        int isp = 40 + (i * 10);
        int lgs = 30 + (i * 8);
        int year = 2021 + (i % 4);
        sm_production(type, cat, sch, isp, lgs, year);
    }

    print_str("\nSmart service...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int rmt = 25 + (i * 6);
        int pdt = 20 + (i * 5);
        int csm = 15 + (i * 4);
        int year = 2022 + (i % 3);
        sm_service(type, cat, rmt, pdt, csm, year);
    }

    print_str("\nSmart management...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int dgt = 30 + (i * 8);
        int vsl = 25 + (i * 6);
        int dcs = 20 + (i * 5);
        int year = 2023 + (i % 2);
        sm_mgmt(type, cat, dgt, vsl, dcs, year);
    }

    print_str("\nSmart collaboration...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int scl = 15 + (i * 4);
        int icl = 12 + (i * 3);
        int ikl = 10 + (i * 3);
        int year = 2024;
        sm_collab(type, cat, scl, icl, ikl, year);
    }

    print_str("\nFactory report...\n");
    sm_factory_report();

    print_str("\nProduction report...\n");
    sm_production_report();

    print_str("\nCollab report...\n");
    sm_collab_report();

    print_str("\nFinal state...\n");
    sm_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
