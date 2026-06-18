/* social_security: Social security management (v1.0)
 * Pension, medical, unemployment, work injury, maternity insurance
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

#define MAX_ENROLLEE    20
#define MAX_PENSION     16
#define MAX_MEDICAL     14
#define MAX_UNEMPLOY    8
#define MAX_INJURY      10
#define MAX_MATERNITY   8

typedef struct {
    int    en_id;
    int    person_id;
    int    employer_id;
    int    base_salary;
    int    start_year;
    int    pension_years;
    int    medical_years;
    int    status;
    int    active;
} enrollee_t;

typedef struct {
    int    pen_id;
    int    en_id;
    int    contribution;
    int    year;
    int    month;
    int    active;
} pension_t;

typedef struct {
    int    med_id;
    int    en_id;
    int    type;
    int    amount;
    int    reimbursed;
    int    year;
    int    active;
} medical_t;

typedef struct {
    int    unemp_id;
    int    en_id;
    int    reason;
    int    benefit;
    int    months;
    int    start_year;
    int    paid_months;
    int    active;
} unemployment_t;

typedef struct {
    int    inj_id;
    int    en_id;
    int    type;
    int    severity;
    int    compensation;
    int    year;
    int    approved;
    int    active;
} injury_t;

typedef struct {
    int    mat_id;
    int    en_id;
    int    benefit;
    int    leave_days;
    int    year;
    int    paid;
    int    active;
} maternity_t;

typedef struct {
    int    n_enrollees;
    int    n_pension;
    int    n_medical;
    int    n_unemploy;
    int    n_injury;
    int    n_maternity;
    double total_pension_contrib;
    double total_medical_contrib;
    double total_medical_reimb;
    double total_unemploy_benefit;
    double total_injury_comp;
    double total_maternity_benefit;
    int    total_enrolled;
    int    total_claims;
} ss_state_t;

static enrollee_t enrollees[MAX_ENROLLEE];
static pension_t pensions[MAX_PENSION];
static medical_t medicals[MAX_MEDICAL];
static unemployment_t unemployments[MAX_UNEMPLOY];
static injury_t injuries[MAX_INJURY];
static maternity_t maternities[MAX_MATERNITY];
static ss_state_t ss;

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

int ss_init(void) {
    if (initialized) return -1;
    ss.n_enrollees = 0; ss.n_pension = 0; ss.n_medical = 0;
    ss.n_unemploy = 0; ss.n_injury = 0; ss.n_maternity = 0;
    ss.total_pension_contrib = 0.0; ss.total_medical_contrib = 0.0;
    ss.total_medical_reimb = 0.0; ss.total_unemploy_benefit = 0.0;
    ss.total_injury_comp = 0.0; ss.total_maternity_benefit = 0.0;
    ss.total_enrolled = 0; ss.total_claims = 0;
    for (int i = 0; i < MAX_ENROLLEE; i++) enrollees[i].active = 0;
    for (int i = 0; i < MAX_PENSION; i++) pensions[i].active = 0;
    for (int i = 0; i < MAX_MEDICAL; i++) medicals[i].active = 0;
    for (int i = 0; i < MAX_UNEMPLOY; i++) unemployments[i].active = 0;
    for (int i = 0; i < MAX_INJURY; i++) injuries[i].active = 0;
    for (int i = 0; i < MAX_MATERNITY; i++) maternities[i].active = 0;
    initialized = 1;
    print_str("[SS] Social security initialized\n");
    return 0;
}

int ss_enroll(int person, int employer, int base_salary, int year) {
    if (ss.n_enrollees >= MAX_ENROLLEE) return -1;
    enrollee_t* e = &enrollees[ss.n_enrollees];
    e->en_id = ss.n_enrollees;
    e->person_id = person;
    e->employer_id = employer;
    e->base_salary = base_salary;
    e->start_year = year;
    e->pension_years = 0;
    e->medical_years = 0;
    e->status = 1;
    e->active = 1;
    ss.total_enrolled++;
    ss.n_enrollees++;
    print_str("[SS] Enroll "); print_int(ss.n_enrollees - 1);
    print_str(" P"); print_int(person);
    print_str(" Emp"); print_int(employer);
    print_str(" $"); print_int(base_salary);
    print_str(" "); print_int(year); print_str("\n");
    return ss.n_enrollees - 1;
}

int ss_pension_contrib(int en_id, int amount, int year, int month) {
    if (ss.n_pension >= MAX_PENSION || en_id >= ss.n_enrollees) return -1;
    pension_t* p = &pensions[ss.n_pension];
    p->pen_id = ss.n_pension;
    p->en_id = en_id;
    p->contribution = amount;
    p->year = year;
    p->month = month;
    p->active = 1;
    enrollees[en_id].pension_years++;
    ss.total_pension_contrib += amount;
    ss.n_pension++;
    print_str("[SS] Pension En"); print_int(en_id);
    print_str(" $"); print_int(amount);
    print_str(" "); print_int(year); print_str("-"); print_int(month); print_str("\n");
    return ss.n_pension - 1;
}

int ss_medical_claim(int en_id, int type, int amount, int reimbursed, int year) {
    if (ss.n_medical >= MAX_MEDICAL || en_id >= ss.n_enrollees) return -1;
    medical_t* m = &medicals[ss.n_medical];
    m->med_id = ss.n_medical;
    m->en_id = en_id;
    m->type = type;
    m->amount = amount;
    m->reimbursed = reimbursed;
    m->year = year;
    m->active = 1;
    enrollees[en_id].medical_years++;
    ss.total_medical_contrib += amount;
    ss.total_medical_reimb += reimbursed;
    ss.total_claims++;
    ss.n_medical++;
    print_str("[SS] Medical En"); print_int(en_id);
    print_str(" type="); print_int(type);
    print_str(" $"); print_int(amount);
    print_str(" reimb=$"); print_int(reimbursed); print_str("\n");
    return ss.n_medical - 1;
}

int ss_unemployment_claim(int en_id, int reason, int benefit, int months, int year) {
    if (ss.n_unemploy >= MAX_UNEMPLOY || en_id >= ss.n_enrollees) return -1;
    unemployment_t* u = &unemployments[ss.n_unemploy];
    u->unemp_id = ss.n_unemploy;
    u->en_id = en_id;
    u->reason = reason;
    u->benefit = benefit;
    u->months = months;
    u->start_year = year;
    u->paid_months = 0;
    u->active = 1;
    ss.total_unemploy_benefit += benefit * months;
    ss.total_claims++;
    ss.n_unemploy++;
    print_str("[SS] Unemploy En"); print_int(en_id);
    print_str(" reason="); print_int(reason);
    print_str(" $"); print_int(benefit);
    print_str(" months="); print_int(months); print_str("\n");
    return ss.n_unemploy - 1;
}

int ss_pay_unemployment(int unemp_id, int months) {
    if (unemp_id >= ss.n_unemploy) return -1;
    unemployments[unemp_id].paid_months += months;
    print_str("[SS] Pay Un"); print_int(unemp_id);
    print_str(" +"); print_int(months); print_str("mo\n");
    return 0;
}

int ss_injury_claim(int en_id, int type, int severity, int comp, int year) {
    if (ss.n_injury >= MAX_INJURY || en_id >= ss.n_enrollees) return -1;
    injury_t* inj = &injuries[ss.n_injury];
    inj->inj_id = ss.n_injury;
    inj->en_id = en_id;
    inj->type = type;
    inj->severity = severity;
    inj->compensation = comp;
    inj->year = year;
    inj->approved = 0;
    inj->active = 1;
    ss.total_injury_comp += comp;
    ss.total_claims++;
    ss.n_injury++;
    print_str("[SS] Injury En"); print_int(en_id);
    print_str(" type="); print_int(type);
    print_str(" sev="); print_int(severity);
    print_str(" $"); print_int(comp); print_str("\n");
    return ss.n_injury - 1;
}

int ss_approve_injury(int inj_id) {
    if (inj_id >= ss.n_injury) return -1;
    injuries[inj_id].approved = 1;
    print_str("[SS] Approve Inj"); print_int(inj_id); print_str("\n");
    return 0;
}

int ss_maternity_claim(int en_id, int benefit, int leave_days, int year) {
    if (ss.n_maternity >= MAX_MATERNITY || en_id >= ss.n_enrollees) return -1;
    maternity_t* mat = &maternities[ss.n_maternity];
    mat->mat_id = ss.n_maternity;
    mat->en_id = en_id;
    mat->benefit = benefit;
    mat->leave_days = leave_days;
    mat->year = year;
    mat->paid = 0;
    mat->active = 1;
    ss.total_maternity_benefit += benefit;
    ss.total_claims++;
    ss.n_maternity++;
    print_str("[SS] Maternity En"); print_int(en_id);
    print_str(" $"); print_int(benefit);
    print_str(" days="); print_int(leave_days);
    print_str(" "); print_int(year); print_str("\n");
    return ss.n_maternity - 1;
}

int ss_pay_maternity(int mat_id) {
    if (mat_id >= ss.n_maternity) return -1;
    maternities[mat_id].paid = 1;
    print_str("[SS] Pay Mat"); print_int(mat_id); print_str("\n");
    return 0;
}

void ss_enrollee_report(void) {
    print_str("[SS] Enrollees: "); print_int(ss.n_enrollees); print_str("\n");
    for (int i = 0; i < ss.n_enrollees; i++) {
        enrollee_t* e = &enrollees[i];
        print_str("  En"); print_int(i);
        print_str(" P"); print_int(e->person_id);
        print_str(" Emp"); print_int(e->employer_id);
        print_str(" $"); print_int(e->base_salary);
        print_str(" pen="); print_int(e->pension_years); print_str("yr");
        print_str(" med="); print_int(e->medical_years); print_str("yr\n");
    }
}

void ss_financial_report(void) {
    print_str("[SS] Financial report:\n");
    print_str("  Pension contrib: "); print_int((int)ss.total_pension_contrib); print_str("\n");
    print_str("  Medical contrib: "); print_int((int)ss.total_medical_contrib); print_str("\n");
    print_str("  Medical reimb: "); print_int((int)ss.total_medical_reimb); print_str("\n");
    print_str("  Unemploy benefit: "); print_int((int)ss.total_unemploy_benefit); print_str("\n");
    print_str("  Injury comp: "); print_int((int)ss.total_injury_comp); print_str("\n");
    print_str("  Maternity benefit: "); print_int((int)ss.total_maternity_benefit); print_str("\n");
    print_str("  Total claims: "); print_int(ss.total_claims); print_str("\n");
}

void ss_print_state(void) {
    print_str("[SS] Enrollees="); print_int(ss.n_enrollees);
    print_str(" Pen="); print_int(ss.n_pension);
    print_str(" Med="); print_int(ss.n_medical);
    print_str(" Unemp="); print_int(ss.n_unemploy);
    print_str(" Inj="); print_int(ss.n_injury);
    print_str(" Mat="); print_int(ss.n_maternity);
    print_str("\n");
}

int main(void) {
    print_str("=== Social Security Demo ===\n\n");
    ss_init();

    print_str("Enrolling...\n");
    for (int i = 0; i < 20; i++) {
        int person = 5000 + i;
        int employer = 100 + (i % 10);
        int salary = 5000 + (i * 1000);
        int year = 2015 + (i % 10);
        ss_enroll(person, employer, salary, year);
    }

    print_str("\nPension contributions...\n");
    for (int i = 0; i < 16; i++) {
        int en = i % 20;
        int amount = 500 + (i * 100);
        int year = 2023 + (i % 2);
        int month = 1 + (i % 12);
        ss_pension_contrib(en, amount, year, month);
    }

    print_str("\nMedical claims...\n");
    for (int i = 0; i < 14; i++) {
        int en = i % 20;
        int type = (i % 4) + 1;
        int amount = 2000 + (i * 1000);
        int reimbursed = amount / 2;
        int year = 2024;
        ss_medical_claim(en, type, amount, reimbursed, year);
    }

    print_str("\nUnemployment claims...\n");
    for (int i = 0; i < 8; i++) {
        int en = i;
        int reason = (i % 3) + 1;
        int benefit = 3000 + (i * 500);
        int months = 6 + (i * 2);
        int year = 2024;
        ss_unemployment_claim(en, reason, benefit, months, year);
    }

    print_str("\nPaying unemployment...\n");
    for (int i = 0; i < 8; i++) {
        ss_pay_unemployment(i, 3);
    }

    print_str("\nInjury claims...\n");
    for (int i = 0; i < 10; i++) {
        int en = i % 20;
        int type = (i % 3) + 1;
        int severity = (i % 5) + 1;
        int comp = 10000 + (i * 5000);
        int year = 2024;
        ss_injury_claim(en, type, severity, comp, year);
    }

    print_str("\nApproving injuries...\n");
    for (int i = 0; i < 10; i++) {
        ss_approve_injury(i);
    }

    print_str("\nMaternity claims...\n");
    for (int i = 0; i < 8; i++) {
        int en = i * 2;
        int benefit = 20000 + (i * 5000);
        int leave = 120 + (i * 10);
        int year = 2024;
        ss_maternity_claim(en, benefit, leave, year);
    }

    print_str("\nPaying maternity...\n");
    for (int i = 0; i < 8; i++) {
        ss_pay_maternity(i);
    }

    print_str("\nEnrollee report...\n");
    ss_enrollee_report();

    print_str("\nFinancial report...\n");
    ss_financial_report();

    print_str("\nFinal state...\n");
    ss_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
