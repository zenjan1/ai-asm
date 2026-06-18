/* supply_chain_admin: Supply chain administration system (v1.0)
 * Supplier management, procurement, inventory, distribution, risk management
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

#define MAX_SUPPLIER     16
#define MAX_PROCUREMENT  14
#define MAX_INVENTORY    12
#define MAX_DISTRIBUTION 10
#define MAX_RISK         10

typedef struct {
    int    supplier_id;
    int    supplier_type;
    int    supplier_category;
    int    suppliers_admitted;
    int    suppliers_evaluated;
    int    suppliers_graded;
    int    year;
    int    active;
} supplier_t;

typedef struct {
    int    procurement_id;
    int    procurement_type;
    int    procurement_category;
    int    procurement_plans;
    int    procurement_executed;
    int    procurement_settled;
    int    year;
    int    active;
} procurement_t;

typedef struct {
    int    inventory_id;
    int    inventory_type;
    int    inventory_category;
    int    inventory_checks;
    int    inventory_optimizations;
    int    inventory_alerts;
    int    year;
    int    active;
} inventory_t;

typedef struct {
    int    distribution_id;
    int    distribution_type;
    int    distribution_area;
    int    distribution_plans;
    int    dispatches;
    int    tracking_ops;
    int    year;
    int    active;
} distribution_t;

typedef struct {
    int    risk_id;
    int    risk_type;
    int    risk_category;
    int    supply_risks;
    int    logistics_risks;
    int    contingency_plans;
    int    year;
    int    active;
} risk_t;

typedef struct {
    int    n_supplier;
    int    n_procurement;
    int    n_inventory;
    int    n_distribution;
    int    n_risk;
    int    total_admitted;
    int    total_plans;
    int    total_checks;
    int    total_dispatches;
    int    total_contingency;
} sca_state_t;

static supplier_t suppliers[MAX_SUPPLIER];
static procurement_t procurements[MAX_PROCUREMENT];
static inventory_t inventories[MAX_INVENTORY];
static distribution_t distributions[MAX_DISTRIBUTION];
static risk_t risks[MAX_RISK];
static sca_state_t sca;

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

int sca_init(void) {
    if (initialized) return -1;
    sca.n_supplier = 0; sca.n_procurement = 0; sca.n_inventory = 0;
    sca.n_distribution = 0; sca.n_risk = 0;
    sca.total_admitted = 0; sca.total_plans = 0;
    sca.total_checks = 0; sca.total_dispatches = 0;
    sca.total_contingency = 0;
    for (int i = 0; i < MAX_SUPPLIER; i++) suppliers[i].active = 0;
    for (int i = 0; i < MAX_PROCUREMENT; i++) procurements[i].active = 0;
    for (int i = 0; i < MAX_INVENTORY; i++) inventories[i].active = 0;
    for (int i = 0; i < MAX_DISTRIBUTION; i++) distributions[i].active = 0;
    for (int i = 0; i < MAX_RISK; i++) risks[i].active = 0;
    initialized = 1;
    print_str("[SCA] Supply chain initialized\n");
    return 0;
}

int sca_supplier(int sup_type, int cat, int admitted, int evaluated, int graded, int year) {
    if (sca.n_supplier >= MAX_SUPPLIER) return -1;
    supplier_t* s = &suppliers[sca.n_supplier];
    s->supplier_id = sca.n_supplier;
    s->supplier_type = sup_type;
    s->supplier_category = cat;
    s->suppliers_admitted = admitted;
    s->suppliers_evaluated = evaluated;
    s->suppliers_graded = graded;
    s->year = year;
    s->active = 1;
    sca.total_admitted += admitted;
    sca.n_supplier++;
    print_str("[SCA] Supplier "); print_int(sca.n_supplier - 1);
    print_str(" type="); print_int(sup_type);
    print_str(" cat="); print_int(cat);
    print_str(" adm="); print_int(admitted);
    print_str(" evl="); print_int(evaluated);
    print_str(" grd="); print_int(graded); print_str("\n");
    return sca.n_supplier - 1;
}

int sca_procurement(int proc_type, int cat, int plans, int executed, int settled, int year) {
    if (sca.n_procurement >= MAX_PROCUREMENT) return -1;
    procurement_t* p = &procurements[sca.n_procurement];
    p->procurement_id = sca.n_procurement;
    p->procurement_type = proc_type;
    p->procurement_category = cat;
    p->procurement_plans = plans;
    p->procurement_executed = executed;
    p->procurement_settled = settled;
    p->year = year;
    p->active = 1;
    sca.total_plans += plans;
    sca.n_procurement++;
    print_str("[SCA] Procurement "); print_int(sca.n_procurement - 1);
    print_str(" type="); print_int(proc_type);
    print_str(" cat="); print_int(cat);
    print_str(" pln="); print_int(plans);
    print_str(" ecx="); print_int(executed);
    print_str(" stl="); print_int(settled); print_str("\n");
    return sca.n_procurement - 1;
}

int sca_inventory(int inv_type, int cat, int checks, int optimizations, int alerts, int year) {
    if (sca.n_inventory >= MAX_INVENTORY) return -1;
    inventory_t* inv = &inventories[sca.n_inventory];
    inv->inventory_id = sca.n_inventory;
    inv->inventory_type = inv_type;
    inv->inventory_category = cat;
    inv->inventory_checks = checks;
    inv->inventory_optimizations = optimizations;
    inv->inventory_alerts = alerts;
    inv->year = year;
    inv->active = 1;
    sca.total_checks += checks;
    sca.n_inventory++;
    print_str("[SCA] Inventory "); print_int(sca.n_inventory - 1);
    print_str(" type="); print_int(inv_type);
    print_str(" cat="); print_int(cat);
    print_str(" chk="); print_int(checks);
    print_str(" opt="); print_int(optimizations);
    print_str(" alr="); print_int(alerts); print_str("\n");
    return sca.n_inventory - 1;
}

int sca_distribution(int dst_type, int area, int plans, int dispatches, int tracking, int year) {
    if (sca.n_distribution >= MAX_DISTRIBUTION) return -1;
    distribution_t* d = &distributions[sca.n_distribution];
    d->distribution_id = sca.n_distribution;
    d->distribution_type = dst_type;
    d->distribution_area = area;
    d->distribution_plans = plans;
    d->dispatches = dispatches;
    d->tracking_ops = tracking;
    d->year = year;
    d->active = 1;
    sca.total_dispatches += dispatches;
    sca.n_distribution++;
    print_str("[SCA] Distribution "); print_int(sca.n_distribution - 1);
    print_str(" type="); print_int(dst_type);
    print_str(" area="); print_int(area);
    print_str(" pln="); print_int(plans);
    print_str(" dsp="); print_int(dispatches);
    print_str(" trk="); print_int(tracking); print_str("\n");
    return sca.n_distribution - 1;
}

int sca_risk(int rsk_type, int cat, int supply_risk, int logistics_risk, int contingency, int year) {
    if (sca.n_risk >= MAX_RISK) return -1;
    risk_t* r = &risks[sca.n_risk];
    r->risk_id = sca.n_risk;
    r->risk_type = rsk_type;
    r->risk_category = cat;
    r->supply_risks = supply_risk;
    r->logistics_risks = logistics_risk;
    r->contingency_plans = contingency;
    r->year = year;
    r->active = 1;
    sca.total_contingency += contingency;
    sca.n_risk++;
    print_str("[SCA] Risk "); print_int(sca.n_risk - 1);
    print_str(" type="); print_int(rsk_type);
    print_str(" cat="); print_int(cat);
    print_str(" spr="); print_int(supply_risk);
    print_str(" lgr="); print_int(logistics_risk);
    print_str(" ctp="); print_int(contingency); print_str("\n");
    return sca.n_risk - 1;
}

void sca_supplier_report(void) {
    print_str("[SCA] Supplier report:\n");
    print_str("  Supplier categories: "); print_int(sca.n_supplier); print_str("\n");
    print_str("  Total suppliers admitted: "); print_int(sca.total_admitted); print_str("\n");
}

void sca_procurement_report(void) {
    print_str("[SCA] Procurement report:\n");
    print_str("  Procurement categories: "); print_int(sca.n_procurement); print_str("\n");
    print_str("  Total procurement plans: "); print_int(sca.total_plans); print_str("\n");
}

void sca_risk_report(void) {
    print_str("[SCA] Risk report:\n");
    print_str("  Inventory categories: "); print_int(sca.n_inventory); print_str("\n");
    print_str("  Total inventory checks: "); print_int(sca.total_checks); print_str("\n");
    print_str("  Distribution categories: "); print_int(sca.n_distribution); print_str("\n");
    print_str("  Total dispatches: "); print_int(sca.total_dispatches); print_str("\n");
    print_str("  Risk categories: "); print_int(sca.n_risk); print_str("\n");
    print_str("  Total contingency plans: "); print_int(sca.total_contingency); print_str("\n");
}

void sca_print_state(void) {
    print_str("[SCA] Sp="); print_int(sca.n_supplier);
    print_str(" Pc="); print_int(sca.n_procurement);
    print_str(" In="); print_int(sca.n_inventory);
    print_str(" Dt="); print_int(sca.n_distribution);
    print_str(" Rk="); print_int(sca.n_risk);
    print_str("\n");
}

int main(void) {
    print_str("=== Supply Chain Admin Demo ===\n\n");
    sca_init();

    print_str("Supplier management...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int adm = 30 + (i * 8);
        int evl = 25 + (i * 6);
        int grd = 20 + (i * 5);
        int year = 2020 + (i % 5);
        sca_supplier(type, cat, adm, evl, grd, year);
    }

    print_str("\nProcurement management...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pln = 40 + (i * 10);
        int ecx = 35 + (i * 9);
        int stl = 30 + (i * 8);
        int year = 2021 + (i % 4);
        sca_procurement(type, cat, pln, ecx, stl, year);
    }

    print_str("\nInventory management...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int chk = 50 + (i * 12);
        int opt = 20 + (i * 5);
        int alr = 10 + (i * 3);
        int year = 2022 + (i % 3);
        sca_inventory(type, cat, chk, opt, alr, year);
    }

    print_str("\nDistribution management...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int area = (i % 5) + 1;
        int pln = 25 + (i * 6);
        int dsp = 60 + (i * 15);
        int trk = 40 + (i * 10);
        int year = 2023 + (i % 2);
        sca_distribution(type, area, pln, dsp, trk, year);
    }

    print_str("\nRisk management...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int spr = 15 + (i * 4);
        int lgr = 10 + (i * 3);
        int ctp = 8 + (i * 2);
        int year = 2024;
        sca_risk(type, cat, spr, lgr, ctp, year);
    }

    print_str("\nSupplier report...\n");
    sca_supplier_report();

    print_str("\nProcurement report...\n");
    sca_procurement_report();

    print_str("\nRisk report...\n");
    sca_risk_report();

    print_str("\nFinal state...\n");
    sca_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
