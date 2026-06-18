/* labor_bureau: Labor bureau management system (v1.0)
 * Labor contracts, disputes, inspection, wages, occupational safety
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

#define MAX_CONTRACTS    18
#define MAX_DISPUTES     14
#define MAX_INSPECT      12
#define MAX_WAGES        10
#define MAX_SAFETY       10

typedef struct {
    int    contract_id;
    int    employee_id;
    int    employer_id;
    int    type;
    int    duration_months;
    int    salary;
    int    year_signed;
    int    status;
    int    active;
} contract_t;

typedef struct {
    int    dispute_id;
    int    employee_id;
    int    employer_id;
    int    type;
    int    amount;
    int    mediation;
    int    result;
    int    year;
    int    active;
} dispute_t;

typedef struct {
    int    inspect_id;
    int    employer_id;
    int    inspector_id;
    int    violations;
    int    severity;
    int    corrective;
    int    year;
    int    status;
    int    active;
} inspection_t;

typedef struct {
    int    wage_id;
    int    region_id;
    int    min_wage;
    int    avg_wage;
    int    arrears_cases;
    int    arrears_amount;
    int    year;
    int    active;
} wage_stat_t;

typedef struct {
    int    safety_id;
    int    employer_id;
    int    type;
    int    incidents;
    int    injuries;
    int   防护措施;
    int    year;
    int    status;
    int    active;
} safety_t;

typedef struct {
    int    n_contracts;
    int    n_disputes;
    int    n_inspect;
    int    n_wages;
    int    n_safety;
    int    total_signed;
    int    total_resolved;
    int    total_violations;
    int    total_arrears;
    int    total_incidents;
} lb_state_t;

static contract_t contracts[MAX_CONTRACTS];
static dispute_t disputes[MAX_DISPUTES];
static inspection_t inspections[MAX_INSPECT];
static wage_stat_t wages[MAX_WAGES];
static safety_t safety_records[MAX_SAFETY];
static lb_state_t lb;

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

int lb_init(void) {
    if (initialized) return -1;
    lb.n_contracts = 0; lb.n_disputes = 0; lb.n_inspect = 0;
    lb.n_wages = 0; lb.n_safety = 0;
    lb.total_signed = 0; lb.total_resolved = 0;
    lb.total_violations = 0; lb.total_arrears = 0;
    lb.total_incidents = 0;
    for (int i = 0; i < MAX_CONTRACTS; i++) contracts[i].active = 0;
    for (int i = 0; i < MAX_DISPUTES; i++) disputes[i].active = 0;
    for (int i = 0; i < MAX_INSPECT; i++) inspections[i].active = 0;
    for (int i = 0; i < MAX_WAGES; i++) wages[i].active = 0;
    for (int i = 0; i < MAX_SAFETY; i++) safety_records[i].active = 0;
    initialized = 1;
    print_str("[LB] Labor bureau initialized\n");
    return 0;
}

int lb_sign_contract(int employee, int employer, int type, int duration, int salary, int year) {
    if (lb.n_contracts >= MAX_CONTRACTS) return -1;
    contract_t* c = &contracts[lb.n_contracts];
    c->contract_id = lb.n_contracts;
    c->employee_id = employee;
    c->employer_id = employer;
    c->type = type;
    c->duration_months = duration;
    c->salary = salary;
    c->year_signed = year;
    c->status = 1;
    c->active = 1;
    lb.total_signed++;
    lb.n_contracts++;
    print_str("[LB] Contract "); print_int(lb.n_contracts - 1);
    print_str(" emp="); print_int(employee);
    print_str(" er="); print_int(employer);
    print_str(" type="); print_int(type);
    print_str(" dur="); print_int(duration); print_str("mo");
    print_str(" sal=$"); print_int(salary); print_str("\n");
    return lb.n_contracts - 1;
}

int lb_file_dispute(int employee, int employer, int type, int amount, int mediation, int result, int year) {
    if (lb.n_disputes >= MAX_DISPUTES) return -1;
    dispute_t* d = &disputes[lb.n_disputes];
    d->dispute_id = lb.n_disputes;
    d->employee_id = employee;
    d->employer_id = employer;
    d->type = type;
    d->amount = amount;
    d->mediation = mediation;
    d->result = result;
    d->year = year;
    d->active = 1;
    if (result > 0) lb.total_resolved++;
    lb.n_disputes++;
    print_str("[LB] Dispute "); print_int(lb.n_disputes - 1);
    print_str(" emp="); print_int(employee);
    print_str(" er="); print_int(employer);
    print_str(" type="); print_int(type);
    print_str(" amt=$"); print_int(amount);
    print_str(" med="); print_int(mediation);
    print_str(" res="); print_int(result); print_str("\n");
    return lb.n_disputes - 1;
}

int lb_inspect(int employer, int inspector, int violations, int severity, int corrective, int year) {
    if (lb.n_inspect >= MAX_INSPECT) return -1;
    inspection_t* ins = &inspections[lb.n_inspect];
    ins->inspect_id = lb.n_inspect;
    ins->employer_id = employer;
    ins->inspector_id = inspector;
    ins->violations = violations;
    ins->severity = severity;
    ins->corrective = corrective;
    ins->year = year;
    ins->status = 1;
    ins->active = 1;
    lb.total_violations += violations;
    lb.n_inspect++;
    print_str("[LB] Inspect "); print_int(lb.n_inspect - 1);
    print_str(" er="); print_int(employer);
    print_str(" insp="); print_int(inspector);
    print_str(" viol="); print_int(violations);
    print_str(" sev="); print_int(severity);
    print_str(" corr="); print_int(corrective); print_str("\n");
    return lb.n_inspect - 1;
}

int lb_record_wage(int region, int min_wage, int avg_wage, int arrears_cases, int arrears_amount, int year) {
    if (lb.n_wages >= MAX_WAGES) return -1;
    wage_stat_t* w = &wages[lb.n_wages];
    w->wage_id = lb.n_wages;
    w->region_id = region;
    w->min_wage = min_wage;
    w->avg_wage = avg_wage;
    w->arrears_cases = arrears_cases;
    w->arrears_amount = arrears_amount;
    w->year = year;
    w->active = 1;
    lb.total_arrears += arrears_amount;
    lb.n_wages++;
    print_str("[LB] Wage "); print_int(lb.n_wages - 1);
    print_str(" reg="); print_int(region);
    print_str(" min=$"); print_int(min_wage);
    print_str(" avg=$"); print_int(avg_wage);
    print_str(" arr=$"); print_int(arrears_amount); print_str("\n");
    return lb.n_wages - 1;
}

int lb_record_safety(int employer, int type, int incidents, int injuries, int precautions, int year) {
    if (lb.n_safety >= MAX_SAFETY) return -1;
    safety_t* s = &safety_records[lb.n_safety];
    s->safety_id = lb.n_safety;
    s->employer_id = employer;
    s->type = type;
    s->incidents = incidents;
    s->injuries = injuries;
    s->防护措施 = precautions;
    s->year = year;
    s->status = 1;
    s->active = 1;
    lb.total_incidents += incidents;
    lb.n_safety++;
    print_str("[LB] Safety "); print_int(lb.n_safety - 1);
    print_str(" er="); print_int(employer);
    print_str(" type="); print_int(type);
    print_str(" inc="); print_int(incidents);
    print_str(" inj="); print_int(injuries);
    print_str(" prc="); print_int(precautions); print_str("\n");
    return lb.n_safety - 1;
}

void lb_contract_report(void) {
    print_str("[LB] Contract report:\n");
    print_str("  Contracts: "); print_int(lb.total_signed); print_str("\n");
    print_str("  Disputes: "); print_int(lb.n_disputes); print_str("\n");
    print_str("  Resolved: "); print_int(lb.total_resolved); print_str("\n");
}

void lb_inspection_report(void) {
    print_str("[LB] Inspection report:\n");
    print_str("  Inspections: "); print_int(lb.n_inspect); print_str("\n");
    print_str("  Violations: "); print_int(lb.total_violations); print_str("\n");
}

void lb_wage_report(void) {
    print_str("[LB] Wage report:\n");
    print_str("  Regions: "); print_int(lb.n_wages); print_str("\n");
    print_str("  Total arrears: $"); print_int(lb.total_arrears); print_str("\n");
    print_str("  Safety incidents: "); print_int(lb.total_incidents); print_str("\n");
}

void lb_print_state(void) {
    print_str("[LB] Ct="); print_int(lb.n_contracts);
    print_str(" Ds="); print_int(lb.n_disputes);
    print_str(" In="); print_int(lb.n_inspect);
    print_str(" Wg="); print_int(lb.n_wages);
    print_str(" Sf="); print_int(lb.n_safety);
    print_str("\n");
}

int main(void) {
    print_str("=== Labor Bureau Demo ===\n\n");
    lb_init();

    print_str("Signing contracts...\n");
    for (int i = 0; i < 18; i++) {
        int emp = 1000 + (i * 13);
        int er = 2000 + (i * 7);
        int type = (i % 3) + 1;
        int dur = 12 + (i % 24);
        int sal = 3000 + (i * 200);
        int year = 2019 + (i % 6);
        lb_sign_contract(emp, er, type, dur, sal, year);
    }

    print_str("\nFiling disputes...\n");
    for (int i = 0; i < 14; i++) {
        int emp = 1000 + (i * 11);
        int er = 2000 + (i * 5);
        int type = (i % 4) + 1;
        int amt = 5000 + (i * 2000);
        int med = (i % 3) + 1;
        int res = (i % 3);
        int year = 2020 + (i % 5);
        lb_file_dispute(emp, er, type, amt, med, res, year);
    }

    print_str("\nConducting inspections...\n");
    for (int i = 0; i < 12; i++) {
        int er = 3000 + (i * 10);
        int insp = 400 + (i % 8);
        int viol = 1 + (i % 6);
        int sev = (i % 4) + 1;
        int corr = i % 4;
        int year = 2021 + (i % 4);
        lb_inspect(er, insp, viol, sev, corr, year);
    }

    print_str("\nRecording wages...\n");
    for (int i = 0; i < 10; i++) {
        int reg = (i % 5) + 1;
        int min_w = 2000 + (i * 100);
        int avg_w = 4000 + (i * 300);
        int arr_c = 2 + (i % 5);
        int arr_a = 10000 + (i * 5000);
        int year = 2020 + (i % 4);
        lb_record_wage(reg, min_w, avg_w, arr_c, arr_a, year);
    }

    print_str("\nRecording safety...\n");
    for (int i = 0; i < 10; i++) {
        int er = 5000 + (i * 8);
        int type = (i % 3) + 1;
        int inc = i % 4;
        int inj = i % 3;
        int prc = 2 + (i % 5);
        int year = 2022 + (i % 3);
        lb_record_safety(er, type, inc, inj, prc, year);
    }

    print_str("\nContract report...\n");
    lb_contract_report();

    print_str("\nInspection report...\n");
    lb_inspection_report();

    print_str("\nWage report...\n");
    lb_wage_report();

    print_str("\nFinal state...\n");
    lb_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
