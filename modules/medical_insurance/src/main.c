/* medical_insurance: Medical insurance administration system (v1.0)
 * Insurance enrollment, fund management, medical services, settlement, supervision
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

#define MAX_ENROLLMENT   16
#define MAX_FUND         14
#define MAX_MEDICAL      12
#define MAX_SETTLEMENT   10
#define MAX_SUPERVISION  10

typedef struct {
    int    enrollment_id;
    int    insurance_type;
    int    participant_category;
    int    participants_enrolled;
    int    premiums_collected;
    int    coverage_rate;
    int    year;
    int    active;
} enrollment_t;

typedef struct {
    int    fund_id;
    int    fund_type;
    int    fund_source;
    int    funds_collected;
    int    funds_disbursed;
    int    fund_balance;
    int    year;
    int    active;
} fund_t;

typedef struct {
    int    medical_id;
    int    institution_type;
    int    service_level;
    int    hospitals_designated;
    int    pharmacies_designated;
    int    services_monitored;
    int    year;
    int    active;
} medical_t;

typedef struct {
    int    settlement_id;
    int    claim_type;
    int    settlement_method;
    int    claims_processed;
    int    amount_settled;
    int    cross_region_claims;
    int    year;
    int    active;
} settlement_t;

typedef struct {
    int    supervision_id;
    int    supervision_type;
    int    violation_category;
    int    inspections_conducted;
    int    violations_found;
    int    penalties_imposed;
    int    year;
    int    active;
} supervision_t;

typedef struct {
    int    n_enrollment;
    int    n_fund;
    int    n_medical;
    int    n_settlement;
    int    n_supervision;
    int    total_participants;
    int    total_collected;
    int    total_hospitals;
    int    total_claims;
    int    total_violations;
} mi_state_t;

static enrollment_t enrollments[MAX_ENROLLMENT];
static fund_t funds[MAX_FUND];
static medical_t medicals[MAX_MEDICAL];
static settlement_t settlements[MAX_SETTLEMENT];
static supervision_t supervisions[MAX_SUPERVISION];
static mi_state_t mi;

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

int mi_init(void) {
    if (initialized) return -1;
    mi.n_enrollment = 0; mi.n_fund = 0; mi.n_medical = 0;
    mi.n_settlement = 0; mi.n_supervision = 0;
    mi.total_participants = 0; mi.total_collected = 0;
    mi.total_hospitals = 0; mi.total_claims = 0;
    mi.total_violations = 0;
    for (int i = 0; i < MAX_ENROLLMENT; i++) enrollments[i].active = 0;
    for (int i = 0; i < MAX_FUND; i++) funds[i].active = 0;
    for (int i = 0; i < MAX_MEDICAL; i++) medicals[i].active = 0;
    for (int i = 0; i < MAX_SETTLEMENT; i++) settlements[i].active = 0;
    for (int i = 0; i < MAX_SUPERVISION; i++) supervisions[i].active = 0;
    initialized = 1;
    print_str("[MI] Medical insurance initialized\n");
    return 0;
}

int mi_enrollment(int ins_type, int part_cat, int enrolled, int premiums, int coverage, int year) {
    if (mi.n_enrollment >= MAX_ENROLLMENT) return -1;
    enrollment_t* e = &enrollments[mi.n_enrollment];
    e->enrollment_id = mi.n_enrollment;
    e->insurance_type = ins_type;
    e->participant_category = part_cat;
    e->participants_enrolled = enrolled;
    e->premiums_collected = premiums;
    e->coverage_rate = coverage;
    e->year = year;
    e->active = 1;
    mi.total_participants += enrolled;
    mi.total_collected += premiums;
    mi.n_enrollment++;
    print_str("[MI] Enrollment "); print_int(mi.n_enrollment - 1);
    print_str(" type="); print_int(ins_type);
    print_str(" cat="); print_int(part_cat);
    print_str(" enr="); print_int(enrolled);
    print_str(" prm="); print_int(premiums);
    print_str(" cov="); print_int(coverage); print_str("\n");
    return mi.n_enrollment - 1;
}

int mi_fund(int fund_type, int source, int collected, int disbursed, int balance, int year) {
    if (mi.n_fund >= MAX_FUND) return -1;
    fund_t* f = &funds[mi.n_fund];
    f->fund_id = mi.n_fund;
    f->fund_type = fund_type;
    f->fund_source = source;
    f->funds_collected = collected;
    f->funds_disbursed = disbursed;
    f->fund_balance = balance;
    f->year = year;
    f->active = 1;
    mi.n_fund++;
    print_str("[MI] Fund "); print_int(mi.n_fund - 1);
    print_str(" type="); print_int(fund_type);
    print_str(" src="); print_int(source);
    print_str(" col="); print_int(collected);
    print_str(" dis="); print_int(disbursed);
    print_str(" bal="); print_int(balance); print_str("\n");
    return mi.n_fund - 1;
}

int mi_medical(int inst_type, int level, int hospitals, int pharmacies, int monitored, int year) {
    if (mi.n_medical >= MAX_MEDICAL) return -1;
    medical_t* m = &medicals[mi.n_medical];
    m->medical_id = mi.n_medical;
    m->institution_type = inst_type;
    m->service_level = level;
    m->hospitals_designated = hospitals;
    m->pharmacies_designated = pharmacies;
    m->services_monitored = monitored;
    m->year = year;
    m->active = 1;
    mi.total_hospitals += hospitals;
    mi.n_medical++;
    print_str("[MI] Medical "); print_int(mi.n_medical - 1);
    print_str(" type="); print_int(inst_type);
    print_str(" lvl="); print_int(level);
    print_str(" hsp="); print_int(hospitals);
    print_str(" phr="); print_int(pharmacies);
    print_str(" mon="); print_int(monitored); print_str("\n");
    return mi.n_medical - 1;
}

int mi_settlement(int claim_type, int method, int processed, int amount, int cross_region, int year) {
    if (mi.n_settlement >= MAX_SETTLEMENT) return -1;
    settlement_t* s = &settlements[mi.n_settlement];
    s->settlement_id = mi.n_settlement;
    s->claim_type = claim_type;
    s->settlement_method = method;
    s->claims_processed = processed;
    s->amount_settled = amount;
    s->cross_region_claims = cross_region;
    s->year = year;
    s->active = 1;
    mi.total_claims += processed;
    mi.n_settlement++;
    print_str("[MI] Settlement "); print_int(mi.n_settlement - 1);
    print_str(" type="); print_int(claim_type);
    print_str(" mth="); print_int(method);
    print_str(" prc="); print_int(processed);
    print_str(" amt="); print_int(amount);
    print_str(" xrg="); print_int(cross_region); print_str("\n");
    return mi.n_settlement - 1;
}

int mi_supervision(int sup_type, int violation_cat, int inspections, int violations, int penalties, int year) {
    if (mi.n_supervision >= MAX_SUPERVISION) return -1;
    supervision_t* sv = &supervisions[mi.n_supervision];
    sv->supervision_id = mi.n_supervision;
    sv->supervision_type = sup_type;
    sv->violation_category = violation_cat;
    sv->inspections_conducted = inspections;
    sv->violations_found = violations;
    sv->penalties_imposed = penalties;
    sv->year = year;
    sv->active = 1;
    mi.total_violations += violations;
    mi.n_supervision++;
    print_str("[MI] Supervision "); print_int(mi.n_supervision - 1);
    print_str(" type="); print_int(sup_type);
    print_str(" cat="); print_int(violation_cat);
    print_str(" ins="); print_int(inspections);
    print_str(" vio="); print_int(violations);
    print_str(" pen="); print_int(penalties); print_str("\n");
    return mi.n_supervision - 1;
}

void mi_enrollment_report(void) {
    print_str("[MI] Enrollment report:\n");
    print_str("  Insurance categories: "); print_int(mi.n_enrollment); print_str("\n");
    print_str("  Total participants: "); print_int(mi.total_participants); print_str("\n");
    print_str("  Total premiums: "); print_int(mi.total_collected); print_str("\n");
}

void mi_medical_report(void) {
    print_str("[MI] Medical report:\n");
    print_str("  Fund types: "); print_int(mi.n_fund); print_str("\n");
    print_str("  Medical institutions: "); print_int(mi.n_medical); print_str("\n");
    print_str("  Total hospitals: "); print_int(mi.total_hospitals); print_str("\n");
}

void mi_supervision_report(void) {
    print_str("[MI] Supervision report:\n");
    print_str("  Settlement categories: "); print_int(mi.n_settlement); print_str("\n");
    print_str("  Total claims processed: "); print_int(mi.total_claims); print_str("\n");
    print_str("  Supervision units: "); print_int(mi.n_supervision); print_str("\n");
    print_str("  Total violations: "); print_int(mi.total_violations); print_str("\n");
}

void mi_print_state(void) {
    print_str("[MI] En="); print_int(mi.n_enrollment);
    print_str(" Fn="); print_int(mi.n_fund);
    print_str(" Md="); print_int(mi.n_medical);
    print_str(" St="); print_int(mi.n_settlement);
    print_str(" Sv="); print_int(mi.n_supervision);
    print_str("\n");
}

int main(void) {
    print_str("=== Medical Insurance Demo ===\n\n");
    mi_init();

    print_str("Insurance enrollment...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int enr = 50000 + (i * 10000);
        int prm = 1000 + (i * 200);
        int cov = 85 + (i % 15);
        int year = 2020 + (i % 5);
        mi_enrollment(type, cat, enr, prm, cov, year);
    }

    print_str("\nFund management...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int src = (i % 5) + 1;
        int col = 5000 + (i * 1000);
        int dis = 4000 + (i * 800);
        int bal = 2000 + (i * 500);
        int year = 2021 + (i % 4);
        mi_fund(type, src, col, dis, bal, year);
    }

    print_str("\nMedical services...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int lvl = (i % 3) + 1;
        int hsp = 50 + (i * 15);
        int phr = 100 + (i * 25);
        int mon = 200 + (i * 50);
        int year = 2022 + (i % 3);
        mi_medical(type, lvl, hsp, phr, mon, year);
    }

    print_str("\nClaims settlement...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int mth = (i % 3) + 1;
        int prc = 10000 + (i * 3000);
        int amt = 5000 + (i * 1500);
        int xrg = 500 + (i * 150);
        int year = 2023 + (i % 2);
        mi_settlement(type, mth, prc, amt, xrg, year);
    }

    print_str("\nSupervision...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ins = 100 + (i * 30);
        int vio = 10 + (i * 3);
        int pen = 5 + (i * 2);
        int year = 2024;
        mi_supervision(type, cat, ins, vio, pen, year);
    }

    print_str("\nEnrollment report...\n");
    mi_enrollment_report();

    print_str("\nMedical report...\n");
    mi_medical_report();

    print_str("\nSupervision report...\n");
    mi_supervision_report();

    print_str("\nFinal state...\n");
    mi_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
