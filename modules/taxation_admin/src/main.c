/* taxation_admin: Taxation administration system (v1.0)
 * Tax registration, collection, inspection, service, incentives
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

#define MAX_REGISTER     16
#define MAX_COLLECTION   14
#define MAX_INSPECT      12
#define MAX_SERVICE      10
#define MAX_INCENTIVE    10

typedef struct {
    int    register_id;
    int    taxpayer_id;
    int    tax_type;
    int    registration_type;
    int    region_id;
    int    status;
    int    year;
    int    active;
} register_t;

typedef struct {
    int    collection_id;
    int    taxpayer_id;
    int    tax_type;
    int    declared;
    int    paid;
    int    arrears;
    int    year;
    int    active;
} collection_t;

typedef struct {
    int    inspect_id;
    int    taxpayer_id;
    int    case_type;
    int    amount_involved;
    int    penalty;
    int    resolved;
    int    year;
    int    status;
    int    active;
} inspect_t;

typedef struct {
    int    service_id;
    int    taxpayer_id;
    int    service_type;
    int    consultations;
    int    processing_time;
    int    satisfaction;
    int    year;
    int    active;
} service_t;

typedef struct {
    int    incentive_id;
    int    taxpayer_id;
    int    incentive_type;
    int    amount;
    int    approval;
    int    year;
    int    active;
} incentive_t;

typedef struct {
    int    n_register;
    int    n_collection;
    int    n_inspect;
    int    n_service;
    int    n_incentive;
    int    total_declared;
    int    total_paid;
    int    total_arrears;
    int    total_penalty;
    int    total_incentive;
} ta_state_t;

static register_t registers[MAX_REGISTER];
static collection_t collections[MAX_COLLECTION];
static inspect_t inspects[MAX_INSPECT];
static service_t services[MAX_SERVICE];
static incentive_t incentives[MAX_INCENTIVE];
static ta_state_t ta;

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

int ta_init(void) {
    if (initialized) return -1;
    ta.n_register = 0; ta.n_collection = 0; ta.n_inspect = 0;
    ta.n_service = 0; ta.n_incentive = 0;
    ta.total_declared = 0; ta.total_paid = 0;
    ta.total_arrears = 0; ta.total_penalty = 0;
    ta.total_incentive = 0;
    for (int i = 0; i < MAX_REGISTER; i++) registers[i].active = 0;
    for (int i = 0; i < MAX_COLLECTION; i++) collections[i].active = 0;
    for (int i = 0; i < MAX_INSPECT; i++) inspects[i].active = 0;
    for (int i = 0; i < MAX_SERVICE; i++) services[i].active = 0;
    for (int i = 0; i < MAX_INCENTIVE; i++) incentives[i].active = 0;
    initialized = 1;
    print_str("[TA] Taxation admin initialized\n");
    return 0;
}

int ta_register(int taxpayer, int tax_type, int reg_type, int region, int status, int year) {
    if (ta.n_register >= MAX_REGISTER) return -1;
    register_t* r = &registers[ta.n_register];
    r->register_id = ta.n_register;
    r->taxpayer_id = taxpayer;
    r->tax_type = tax_type;
    r->registration_type = reg_type;
    r->region_id = region;
    r->status = status;
    r->year = year;
    r->active = 1;
    ta.n_register++;
    print_str("[TA] Register "); print_int(ta.n_register - 1);
    print_str(" tpy="); print_int(taxpayer);
    print_str(" type="); print_int(tax_type);
    print_str(" reg="); print_int(reg_type);
    print_str(" rgn="); print_int(region);
    print_str(" st="); print_int(status); print_str("\n");
    return ta.n_register - 1;
}

int ta_collect(int taxpayer, int tax_type, int declared, int paid, int arrears, int year) {
    if (ta.n_collection >= MAX_COLLECTION) return -1;
    collection_t* c = &collections[ta.n_collection];
    c->collection_id = ta.n_collection;
    c->taxpayer_id = taxpayer;
    c->tax_type = tax_type;
    c->declared = declared;
    c->paid = paid;
    c->arrears = arrears;
    c->year = year;
    c->active = 1;
    ta.total_declared += declared;
    ta.total_paid += paid;
    ta.total_arrears += arrears;
    ta.n_collection++;
    print_str("[TA] Collect "); print_int(ta.n_collection - 1);
    print_str(" tpy="); print_int(taxpayer);
    print_str(" type="); print_int(tax_type);
    print_str(" dcl=$"); print_int(declared);
    print_str(" paid=$"); print_int(paid);
    print_str(" arr=$"); print_int(arrears); print_str("\n");
    return ta.n_collection - 1;
}

int ta_inspect(int taxpayer, int case_type, int amount, int penalty, int resolved, int year) {
    if (ta.n_inspect >= MAX_INSPECT) return -1;
    inspect_t* ins = &inspects[ta.n_inspect];
    ins->inspect_id = ta.n_inspect;
    ins->taxpayer_id = taxpayer;
    ins->case_type = case_type;
    ins->amount_involved = amount;
    ins->penalty = penalty;
    ins->resolved = resolved;
    ins->year = year;
    ins->status = 1;
    ins->active = 1;
    ta.total_penalty += penalty;
    ta.n_inspect++;
    print_str("[TA] Inspect "); print_int(ta.n_inspect - 1);
    print_str(" tpy="); print_int(taxpayer);
    print_str(" case="); print_int(case_type);
    print_str(" amt=$"); print_int(amount);
    print_str(" pen=$"); print_int(penalty);
    print_str(" res="); print_int(resolved); print_str("\n");
    return ta.n_inspect - 1;
}

int ta_service(int taxpayer, int service_type, int consultations, int processing, int satisfaction, int year) {
    if (ta.n_service >= MAX_SERVICE) return -1;
    service_t* s = &services[ta.n_service];
    s->service_id = ta.n_service;
    s->taxpayer_id = taxpayer;
    s->service_type = service_type;
    s->consultations = consultations;
    s->processing_time = processing;
    s->satisfaction = satisfaction;
    s->year = year;
    s->active = 1;
    ta.n_service++;
    print_str("[TA] Service "); print_int(ta.n_service - 1);
    print_str(" tpy="); print_int(taxpayer);
    print_str(" type="); print_int(service_type);
    print_str(" cns="); print_int(consultations);
    print_str(" prc="); print_int(processing); print_str("d");
    print_str(" sat="); print_int(satisfaction); print_str("\n");
    return ta.n_service - 1;
}

int ta_incentive(int taxpayer, int incentive_type, int amount, int approval, int year) {
    if (ta.n_incentive >= MAX_INCENTIVE) return -1;
    incentive_t* inc = &incentives[ta.n_incentive];
    inc->incentive_id = ta.n_incentive;
    inc->taxpayer_id = taxpayer;
    inc->incentive_type = incentive_type;
    inc->amount = amount;
    inc->approval = approval;
    inc->year = year;
    inc->active = 1;
    ta.total_incentive += amount;
    ta.n_incentive++;
    print_str("[TA] Incentive "); print_int(ta.n_incentive - 1);
    print_str(" tpy="); print_int(taxpayer);
    print_str(" type="); print_int(incentive_type);
    print_str(" amt=$"); print_int(amount);
    print_str(" apr="); print_int(approval); print_str("\n");
    return ta.n_incentive - 1;
}

void ta_register_report(void) {
    print_str("[TA] Registration report:\n");
    print_str("  Taxpayers: "); print_int(ta.n_register); print_str("\n");
}

void ta_collection_report(void) {
    print_str("[TA] Collection report:\n");
    print_str("  Collections: "); print_int(ta.n_collection); print_str("\n");
    print_str("  Total declared: $"); print_int(ta.total_declared); print_str("\n");
    print_str("  Total paid: $"); print_int(ta.total_paid); print_str("\n");
    print_str("  Total arrears: $"); print_int(ta.total_arrears); print_str("\n");
}

void ta_inspect_report(void) {
    print_str("[TA] Inspection report:\n");
    print_str("  Inspections: "); print_int(ta.n_inspect); print_str("\n");
    print_str("  Total penalties: $"); print_int(ta.total_penalty); print_str("\n");
    print_str("  Tax services: "); print_int(ta.n_service); print_str("\n");
    print_str("  Incentives: "); print_int(ta.n_incentive); print_str("\n");
    print_str("  Total incentives: $"); print_int(ta.total_incentive); print_str("\n");
}

void ta_print_state(void) {
    print_str("[TA] Rg="); print_int(ta.n_register);
    print_str(" Cl="); print_int(ta.n_collection);
    print_str(" In="); print_int(ta.n_inspect);
    print_str(" Sv="); print_int(ta.n_service);
    print_str(" Ic="); print_int(ta.n_incentive);
    print_str("\n");
}

int main(void) {
    print_str("=== Taxation Admin Demo ===\n\n");
    ta_init();

    print_str("Tax registration...\n");
    for (int i = 0; i < 16; i++) {
        int tpy = 1000 + (i * 13);
        int type = (i % 4) + 1;
        int reg = (i % 3) + 1;
        int rgn = (i % 8) + 1;
        int st = (i % 3) + 1;
        int year = 2020 + (i % 5);
        ta_register(tpy, type, reg, rgn, st, year);
    }

    print_str("\nTax collection...\n");
    for (int i = 0; i < 14; i++) {
        int tpy = 2000 + (i * 11);
        int type = (i % 4) + 1;
        int dcl = 50000 + (i * 20000);
        int paid = dcl - (i * 3000);
        int arr = (i % 4 == 0) ? 5000 + (i * 2000) : 0;
        int year = 2021 + (i % 4);
        ta_collect(tpy, type, dcl, paid, arr, year);
    }

    print_str("\nTax inspection...\n");
    for (int i = 0; i < 12; i++) {
        int tpy = 3000 + (i * 17);
        int case_t = (i % 3) + 1;
        int amt = 10000 + (i * 5000);
        int pen = 2000 + (i * 1000);
        int res = (i % 3 == 0) ? 0 : 1;
        int year = 2022 + (i % 3);
        ta_inspect(tpy, case_t, amt, pen, res, year);
    }

    print_str("\nTax services...\n");
    for (int i = 0; i < 10; i++) {
        int tpy = 4000 + (i * 7);
        int type = (i % 4) + 1;
        int cns = 1 + (i % 5);
        int prc = 3 + (i % 7);
        int sat = 70 + (i * 3);
        int year = 2023 + (i % 2);
        ta_service(tpy, type, cns, prc, sat, year);
    }

    print_str("\nTax incentives...\n");
    for (int i = 0; i < 10; i++) {
        int tpy = 5000 + (i * 13);
        int type = (i % 3) + 1;
        int amt = 10000 + (i * 5000);
        int apr = (i % 3 == 0) ? 0 : 1;
        int year = 2024;
        ta_incentive(tpy, type, amt, apr, year);
    }

    print_str("\nRegistration report...\n");
    ta_register_report();

    print_str("\nCollection report...\n");
    ta_collection_report();

    print_str("\nInspection report...\n");
    ta_inspect_report();

    print_str("\nFinal state...\n");
    ta_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
