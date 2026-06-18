/* maritime_fleet: Fleet management (v1.0)
 * Maintenance, navigation, compliance, insurance, spare parts, costs
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

#define MAX_VESSELS   10
#define MAX_MAINT     16
#define MAX_VOYAGES   12
#define MAX_COMPLIANCE 10
#define MAX_INSURANCE 8
#define MAX_SPARES    14

typedef struct {
    int    vessel_id;
    int    type;
    int    dwt;
    int    age_years;
    int    class_expiry;
    double hull_value;
    int    status;
    double maintenance_total;
    int    active;
} vessel_t;

typedef struct {
    int    maint_id;
    int    vessel_id;
    int    system;
    int    type;
    int    hours_work;
    double cost;
    int    scheduled_day;
    int    completed;
    int    active;
} maintenance_t;

typedef struct {
    int    voyage_id;
    int    vessel_id;
    int    plan_type;
    int    distance_nm;
    int    weather_risk;
    int    days;
    double fuel_cost;
    int    active;
} voyage_plan_t;

typedef struct {
    int    comp_id;
    int    vessel_id;
    int    type;
    int    audit_day;
    int    findings;
    int    resolved;
    double cost;
    int    active;
} compliance_t;

typedef struct {
    int    ins_id;
    int    vessel_id;
    int    type;
    double coverage;
    double premium;
    int    claim_count;
    double claims_total;
    int    active;
} insurance_t;

typedef struct {
    int    spare_id;
    int    system;
    int    qty_onhand;
    int    reorder_level;
    double unit_cost;
    int    n_used;
    double total_cost;
    int    active;
} spare_t;

typedef struct {
    int    n_vessels;
    int    n_maint;
    int    n_voyages;
    int    n_compliance;
    int    n_insurance;
    int    n_spares;
    double maintenance_costs;
    double insurance_costs;
    double compliance_costs;
    double spare_costs;
    double voyage_costs;
    double total_costs;
    int    total_maint_tasks;
    int    total_findings;
    int    total_claims;
} mf_state_t;

static vessel_t vessels[MAX_VESSELS];
static maintenance_t maintenance[MAX_MAINT];
static voyage_plan_t voyage_plans[MAX_VOYAGES];
static compliance_t compliance[MAX_COMPLIANCE];
static insurance_t insurance[MAX_INSURANCE];
static spare_t spares[MAX_SPARES];
static mf_state_t mf;

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

int mf_init(void) {
    if (initialized) return -1;
    mf.n_vessels = 0; mf.n_maint = 0; mf.n_voyages = 0;
    mf.n_compliance = 0; mf.n_insurance = 0; mf.n_spares = 0;
    mf.maintenance_costs = 0.0; mf.insurance_costs = 0.0;
    mf.compliance_costs = 0.0; mf.spare_costs = 0.0;
    mf.voyage_costs = 0.0; mf.total_costs = 0.0;
    mf.total_maint_tasks = 0; mf.total_findings = 0;
    mf.total_claims = 0;
    for (int i = 0; i < MAX_VESSELS; i++) vessels[i].active = 0;
    for (int i = 0; i < MAX_MAINT; i++) maintenance[i].active = 0;
    for (int i = 0; i < MAX_VOYAGES; i++) voyage_plans[i].active = 0;
    for (int i = 0; i < MAX_COMPLIANCE; i++) compliance[i].active = 0;
    for (int i = 0; i < MAX_INSURANCE; i++) insurance[i].active = 0;
    for (int i = 0; i < MAX_SPARES; i++) spares[i].active = 0;
    initialized = 1;
    print_str("[MF] Maritime fleet initialized\n");
    return 0;
}

int mf_add_vessel(int type, int dwt, int age, int class_expiry, double hull_value) {
    if (mf.n_vessels >= MAX_VESSELS) return -1;
    vessel_t* v = &vessels[mf.n_vessels];
    v->vessel_id = mf.n_vessels;
    v->type = type;
    v->dwt = dwt;
    v->age_years = age;
    v->class_expiry = class_expiry;
    v->hull_value = hull_value;
    v->status = 1;
    v->maintenance_total = 0.0;
    v->active = 1;
    mf.n_vessels++;
    print_str("[MF] Vessel "); print_int(mf.n_vessels - 1);
    print_str(" type="); print_int(type);
    print_str(" dwt="); print_int(dwt);
    print_str(" age="); print_int(age);
    print_str("yr hull=$"); print_int((int)hull_value); print_str("\n");
    return mf.n_vessels - 1;
}

int mf_schedule_maintenance(int vessel_id, int system, int type, int hours, int day) {
    if (mf.n_maint >= MAX_MAINT || vessel_id >= mf.n_vessels) return -1;
    maintenance_t* m = &maintenance[mf.n_maint];
    m->maint_id = mf.n_maint;
    m->vessel_id = vessel_id;
    m->system = system;
    m->type = type;
    m->hours_work = hours;
    m->cost = hours * 150.0 + type * 500.0;
    m->scheduled_day = day;
    m->completed = 0;
    m->active = 1;
    mf.total_maint_tasks++;
    mf.n_maint++;
    print_str("[MF] Maint "); print_int(mf.n_maint - 1);
    print_str(" V"); print_int(vessel_id);
    print_str(" sys="); print_int(system);
    print_str(" hrs="); print_int(hours);
    print_str(" $"); print_int((int)m->cost); print_str("\n");
    return mf.n_maint - 1;
}

int mf_complete_maintenance(int maint_id) {
    if (maint_id >= mf.n_maint) return -1;
    maintenance_t* m = &maintenance[maint_id];
    m->completed = 1;
    mf.maintenance_costs += m->cost;
    mf.total_costs += m->cost;
    vessels[m->vessel_id].maintenance_total += m->cost;
    print_str("[MF] Complete M"); print_int(maint_id);
    print_str(" $"); print_int((int)m->cost); print_str("\n");
    return 0;
}

int mf_plan_voyage(int vessel_id, int plan_type, int distance, int weather, int days) {
    if (mf.n_voyages >= MAX_VOYAGES || vessel_id >= mf.n_vessels) return -1;
    voyage_plan_t* vp = &voyage_plans[mf.n_voyages];
    vp->voyage_id = mf.n_voyages;
    vp->vessel_id = vessel_id;
    vp->plan_type = plan_type;
    vp->distance_nm = distance;
    vp->weather_risk = weather;
    vp->days = days;
    vp->fuel_cost = distance * 0.8 + days * 500.0;
    vp->active = 1;
    mf.voyage_costs += vp->fuel_cost;
    mf.total_costs += vp->fuel_cost;
    mf.n_voyages++;
    print_str("[MF] VoyagePlan "); print_int(mf.n_voyages - 1);
    print_str(" V"); print_int(vessel_id);
    print_str(" nm="); print_int(distance);
    print_str(" d="); print_int(days);
    print_str(" $"); print_int((int)vp->fuel_cost); print_str("\n");
    return mf.n_voyages - 1;
}

int mf_add_compliance(int vessel_id, int type, int audit_day, int findings) {
    if (mf.n_compliance >= MAX_COMPLIANCE || vessel_id >= mf.n_vessels) return -1;
    compliance_t* c = &compliance[mf.n_compliance];
    c->comp_id = mf.n_compliance;
    c->vessel_id = vessel_id;
    c->type = type;
    c->audit_day = audit_day;
    c->findings = findings;
    c->resolved = 0;
    c->cost = findings * 2000.0;
    c->active = 1;
    mf.total_findings += findings;
    mf.compliance_costs += c->cost;
    mf.total_costs += c->cost;
    mf.n_compliance++;
    print_str("[MF] Compliance "); print_int(mf.n_compliance - 1);
    print_str(" V"); print_int(vessel_id);
    print_str(" type="); print_int(type);
    print_str(" findings="); print_int(findings);
    print_str(" $"); print_int((int)c->cost); print_str("\n");
    return mf.n_compliance - 1;
}

int mf_resolve_compliance(int comp_id) {
    if (comp_id >= mf.n_compliance) return -1;
    compliance[comp_id].resolved = 1;
    print_str("[MF] Resolve C"); print_int(comp_id); print_str("\n");
    return 0;
}

int mf_add_insurance(int vessel_id, int type, double coverage, double premium) {
    if (mf.n_insurance >= MAX_INSURANCE || vessel_id >= mf.n_vessels) return -1;
    insurance_t* ins = &insurance[mf.n_insurance];
    ins->ins_id = mf.n_insurance;
    ins->vessel_id = vessel_id;
    ins->type = type;
    ins->coverage = coverage;
    ins->premium = premium;
    ins->claim_count = 0;
    ins->claims_total = 0.0;
    ins->active = 1;
    mf.insurance_costs += premium;
    mf.total_costs += premium;
    mf.n_insurance++;
    print_str("[MF] Insurance "); print_int(mf.n_insurance - 1);
    print_str(" V"); print_int(vessel_id);
    print_str(" type="); print_int(type);
    print_str(" cov="); print_int((int)coverage);
    print_str(" prem="); print_int((int)premium); print_str("\n");
    return mf.n_insurance - 1;
}

int mf_file_claim(int ins_id, double amount) {
    if (ins_id >= mf.n_insurance) return -1;
    insurance_t* ins = &insurance[ins_id];
    ins->claim_count++;
    ins->claims_total += amount;
    mf.total_claims++;
    print_str("[MF] Claim I"); print_int(ins_id);
    print_str(" $"); print_int((int)amount); print_str("\n");
    return 0;
}

int mf_add_spare(int system, int qty, int reorder, double cost) {
    if (mf.n_spares >= MAX_SPARES) return -1;
    spare_t* sp = &spares[mf.n_spares];
    sp->spare_id = mf.n_spares;
    sp->system = system;
    sp->qty_onhand = qty;
    sp->reorder_level = reorder;
    sp->unit_cost = cost;
    sp->n_used = 0;
    sp->total_cost = 0.0;
    sp->active = 1;
    double purchase = qty * cost;
    mf.spare_costs += purchase;
    mf.total_costs += purchase;
    mf.n_spares++;
    print_str("[MF] Spare "); print_int(mf.n_spares - 1);
    print_str(" sys="); print_int(system);
    print_str(" qty="); print_int(qty);
    print_str(" $"); print_int((int)cost); print_str("/ea\n");
    return mf.n_spares - 1;
}

int mf_use_spare(int spare_id, int qty) {
    if (spare_id >= mf.n_spares) return -1;
    spare_t* sp = &spares[spare_id];
    if (sp->qty_onhand < qty) return -2;
    sp->qty_onhand -= qty;
    sp->n_used += qty;
    double cost = qty * sp->unit_cost;
    sp->total_cost += cost;
    print_str("[MF] Use Sp"); print_int(spare_id);
    print_str(" qty="); print_int(qty);
    print_str(" $"); print_int((int)cost); print_str("\n");
    return 0;
}

void mf_maintenance_report(void) {
    print_str("[MF] Maintenance report:\n");
    for (int i = 0; i < mf.n_vessels; i++) {
        vessel_t* v = &vessels[i];
        print_str("  V"); print_int(i);
        print_str(" type="); print_int(v->type);
        print_str(" age="); print_int(v->age_years);
        print_str("yr maint=$"); print_int((int)v->maintenance_total); print_str("\n");
    }
    for (int i = 0; i < mf.n_maint; i++) {
        maintenance_t* m = &maintenance[i];
        print_str("  M"); print_int(i);
        print_str(" V"); print_int(m->vessel_id);
        print_str(" sys="); print_int(m->system);
        print_str(" done="); print_int(m->completed);
        print_str(" $"); print_int((int)m->cost); print_str("\n");
    }
}

void mf_cost_report(void) {
    print_str("[MF] Cost report:\n");
    print_str("  Maintenance: "); print_int((int)mf.maintenance_costs); print_str("\n");
    print_str("  Insurance: "); print_int((int)mf.insurance_costs); print_str("\n");
    print_str("  Compliance: "); print_int((int)mf.compliance_costs); print_str("\n");
    print_str("  Spares: "); print_int((int)mf.spare_costs); print_str("\n");
    print_str("  Voyages: "); print_int((int)mf.voyage_costs); print_str("\n");
    print_str("  Total costs: "); print_int((int)mf.total_costs); print_str("\n");
    print_str("  Tasks: "); print_int(mf.total_maint_tasks);
    print_str(" Findings: "); print_int(mf.total_findings);
    print_str(" Claims: "); print_int(mf.total_claims); print_str("\n");
}

void mf_print_state(void) {
    print_str("[MF] Vessels="); print_int(mf.n_vessels);
    print_str(" Maint="); print_int(mf.n_maint);
    print_str(" Voyages="); print_int(mf.n_voyages);
    print_str(" Comp="); print_int(mf.n_compliance);
    print_str(" Ins="); print_int(mf.n_insurance);
    print_str(" Spares="); print_int(mf.n_spares);
    print_str("\n");
    print_str("  Costs: "); print_int((int)mf.total_costs); print_str("\n");
}

int main(void) {
    print_str("=== Maritime Fleet Demo ===\n\n");
    mf_init();

    print_str("Adding vessels...\n");
    mf_add_vessel(1, 80000, 5, 2028, 45000000.0);
    mf_add_vessel(1, 75000, 8, 2027, 38000000.0);
    mf_add_vessel(2, 50000, 3, 2029, 30000000.0);
    mf_add_vessel(2, 45000, 10, 2026, 25000000.0);
    mf_add_vessel(3, 120000, 6, 2028, 60000000.0);
    mf_add_vessel(3, 100000, 12, 2025, 42000000.0);
    mf_add_vessel(1, 70000, 4, 2029, 40000000.0);
    mf_add_vessel(4, 80000, 7, 2027, 50000000.0);
    mf_add_vessel(2, 42000, 9, 2026, 22000000.0);
    mf_add_vessel(4, 75000, 2, 2030, 55000000.0);

    print_str("\nScheduling maintenance...\n");
    mf_schedule_maintenance(0, 1, 1, 200, 10);
    mf_schedule_maintenance(0, 2, 2, 150, 15);
    mf_schedule_maintenance(1, 1, 1, 180, 12);
    mf_schedule_maintenance(1, 3, 3, 300, 20);
    mf_schedule_maintenance(2, 1, 2, 220, 11);
    mf_schedule_maintenance(2, 2, 1, 160, 18);
    mf_schedule_maintenance(3, 1, 3, 280, 14);
    mf_schedule_maintenance(4, 2, 2, 200, 16);
    mf_schedule_maintenance(4, 3, 1, 170, 22);
    mf_schedule_maintenance(5, 1, 2, 240, 13);
    mf_schedule_maintenance(5, 2, 3, 320, 25);
    mf_schedule_maintenance(6, 1, 1, 190, 17);
    mf_schedule_maintenance(7, 3, 2, 210, 19);
    mf_schedule_maintenance(8, 1, 1, 175, 21);
    mf_schedule_maintenance(8, 2, 3, 290, 24);
    mf_schedule_maintenance(9, 1, 2, 230, 23);

    print_str("\nCompleting maintenance...\n");
    for (int i = 0; i < 16; i++) {
        mf_complete_maintenance(i);
    }

    print_str("\nPlanning voyages...\n");
    mf_plan_voyage(0, 1, 8000, 2, 15);
    mf_plan_voyage(1, 2, 6500, 3, 12);
    mf_plan_voyage(2, 1, 7000, 1, 14);
    mf_plan_voyage(3, 3, 5500, 4, 10);
    mf_plan_voyage(4, 2, 10000, 2, 20);
    mf_plan_voyage(5, 1, 9000, 3, 18);
    mf_plan_voyage(6, 3, 4000, 1, 8);
    mf_plan_voyage(7, 2, 12000, 4, 22);
    mf_plan_voyage(8, 1, 7500, 2, 16);
    mf_plan_voyage(9, 3, 6000, 3, 11);
    mf_plan_voyage(0, 2, 8500, 1, 17);
    mf_plan_voyage(1, 1, 5000, 2, 9);

    print_str("\nCompliance audits...\n");
    mf_add_compliance(0, 1, 30, 3);
    mf_add_compliance(1, 2, 32, 2);
    mf_add_compliance(2, 1, 34, 4);
    mf_add_compliance(3, 3, 36, 1);
    mf_add_compliance(4, 2, 38, 5);
    mf_add_compliance(5, 1, 40, 2);
    mf_add_compliance(6, 3, 42, 3);
    mf_add_compliance(7, 2, 44, 4);
    mf_add_compliance(8, 1, 46, 1);
    mf_add_compliance(9, 3, 48, 2);

    print_str("\nResolving compliance...\n");
    for (int i = 0; i < 10; i++) {
        mf_resolve_compliance(i);
    }

    print_str("\nInsurance...\n");
    mf_add_insurance(0, 1, 40000000.0, 200000.0);
    mf_add_insurance(0, 2, 5000000.0, 50000.0);
    mf_add_insurance(1, 1, 35000000.0, 180000.0);
    mf_add_insurance(2, 1, 28000000.0, 150000.0);
    mf_add_insurance(3, 1, 22000000.0, 120000.0);
    mf_add_insurance(4, 1, 55000000.0, 280000.0);
    mf_add_insurance(5, 1, 38000000.0, 200000.0);
    mf_add_insurance(6, 1, 36000000.0, 190000.0);

    print_str("\nFiling claims...\n");
    mf_file_claim(0, 150000.0);
    mf_file_claim(2, 80000.0);
    mf_file_claim(4, 250000.0);
    mf_file_claim(5, 120000.0);
    mf_file_claim(1, 45000.0);
    mf_file_claim(3, 95000.0);
    mf_file_claim(6, 60000.0);
    mf_file_claim(7, 180000.0);

    print_str("\nAdding spares...\n");
    mf_add_spare(1, 20, 5, 2500.0);
    mf_add_spare(1, 15, 3, 4500.0);
    mf_add_spare(2, 30, 8, 800.0);
    mf_add_spare(2, 25, 6, 1200.0);
    mf_add_spare(3, 10, 2, 8000.0);
    mf_add_spare(3, 12, 3, 6500.0);
    mf_add_spare(4, 40, 10, 350.0);
    mf_add_spare(4, 35, 8, 450.0);
    mf_add_spare(1, 18, 4, 3200.0);
    mf_add_spare(2, 22, 5, 950.0);
    mf_add_spare(3, 8, 2, 9500.0);
    mf_add_spare(5, 50, 12, 200.0);
    mf_add_spare(5, 45, 10, 280.0);
    mf_add_spare(1, 16, 4, 3800.0);

    print_str("\nUsing spares...\n");
    for (int i = 0; i < 14; i++) {
        mf_use_spare(i, 2 + (i % 5));
    }

    print_str("\nMaintenance report...\n");
    mf_maintenance_report();

    print_str("\nCost report...\n");
    mf_cost_report();

    print_str("\nFinal state...\n");
    mf_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
