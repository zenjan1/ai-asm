/* social_insurance: Social insurance administration system (v1.0)
 * Pension, unemployment, work injury, maternity insurance, fund management
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

#define MAX_PENSION      16
#define MAX_UNEMPLOYMENT 14
#define MAX_WORKINJURY   12
#define MAX_MATERNITY    10
#define MAX_SI_FUND      10

typedef struct {
    int    pension_id;
    int    pension_type;
    int    participant_category;
    int    participants_enrolled;
    int    pensions_paid;
    int    pension_adjusted;
    int    year;
    int    active;
} pension_t;

typedef struct {
    int    unemployment_id;
    int    unemployment_type;
    int    region_id;
    int    participants_enrolled;
    int    benefits_paid;
    int    reemployed;
    int    year;
    int    active;
} unemployment_t;

typedef struct {
    int    workinjury_id;
    int    injury_category;
    int    industry_risk;
    int    participants_enrolled;
    int    injuries_recognized;
    int    compensation_paid;
    int    year;
    int    active;
} workinjury_t;

typedef struct {
    int    maternity_id;
    int    maternity_type;
    int    beneficiary_category;
    int    participants_enrolled;
    int    allowances_paid;
    int    medical_services;
    int    year;
    int    active;
} maternity_t;

typedef struct {
    int    fund_id;
    int    fund_category;
    int    fund_source;
    int    funds_collected;
    int    funds_invested;
    int    fund_surplus;
    int    year;
    int    active;
} si_fund_t;

typedef struct {
    int    n_pension;
    int    n_unemployment;
    int    n_workinjury;
    int    n_maternity;
    int    n_fund;
    int    total_pensioners;
    int    total_unemployed;
    int    total_injuries;
    int    total_beneficiaries;
    int    total_funds;
} si_state_t;

static pension_t pensions[MAX_PENSION];
static unemployment_t unemployment_list[MAX_UNEMPLOYMENT];
static workinjury_t workinjuries[MAX_WORKINJURY];
static maternity_t maternities[MAX_MATERNITY];
static si_fund_t si_funds[MAX_SI_FUND];
static si_state_t si;

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

int si_init(void) {
    if (initialized) return -1;
    si.n_pension = 0; si.n_unemployment = 0; si.n_workinjury = 0;
    si.n_maternity = 0; si.n_fund = 0;
    si.total_pensioners = 0; si.total_unemployed = 0;
    si.total_injuries = 0; si.total_beneficiaries = 0;
    si.total_funds = 0;
    for (int i = 0; i < MAX_PENSION; i++) pensions[i].active = 0;
    for (int i = 0; i < MAX_UNEMPLOYMENT; i++) unemployment_list[i].active = 0;
    for (int i = 0; i < MAX_WORKINJURY; i++) workinjuries[i].active = 0;
    for (int i = 0; i < MAX_MATERNITY; i++) maternities[i].active = 0;
    for (int i = 0; i < MAX_SI_FUND; i++) si_funds[i].active = 0;
    initialized = 1;
    print_str("[SI] Social insurance initialized\n");
    return 0;
}

int si_pension(int pen_type, int part_cat, int enrolled, int paid, int adjusted, int year) {
    if (si.n_pension >= MAX_PENSION) return -1;
    pension_t* p = &pensions[si.n_pension];
    p->pension_id = si.n_pension;
    p->pension_type = pen_type;
    p->participant_category = part_cat;
    p->participants_enrolled = enrolled;
    p->pensions_paid = paid;
    p->pension_adjusted = adjusted;
    p->year = year;
    p->active = 1;
    si.total_pensioners += paid;
    si.n_pension++;
    print_str("[SI] Pension "); print_int(si.n_pension - 1);
    print_str(" type="); print_int(pen_type);
    print_str(" cat="); print_int(part_cat);
    print_str(" enr="); print_int(enrolled);
    print_str(" paid="); print_int(paid);
    print_str(" adj="); print_int(adjusted); print_str("\n");
    return si.n_pension - 1;
}

int si_unemployment(int unemp_type, int region, int enrolled, int benefits, int reemployed_cnt, int year) {
    if (si.n_unemployment >= MAX_UNEMPLOYMENT) return -1;
    unemployment_t* u = &unemployment_list[si.n_unemployment];
    u->unemployment_id = si.n_unemployment;
    u->unemployment_type = unemp_type;
    u->region_id = region;
    u->participants_enrolled = enrolled;
    u->benefits_paid = benefits;
    u->reemployed = reemployed_cnt;
    u->year = year;
    u->active = 1;
    si.total_unemployed += benefits;
    si.n_unemployment++;
    print_str("[SI] Unemployment "); print_int(si.n_unemployment - 1);
    print_str(" type="); print_int(unemp_type);
    print_str(" rgn="); print_int(region);
    print_str(" enr="); print_int(enrolled);
    print_str(" ben="); print_int(benefits);
    print_str(" rem="); print_int(reemployed_cnt); print_str("\n");
    return si.n_unemployment - 1;
}

int si_workinjury(int inj_cat, int risk, int enrolled, int recognized, int compensation, int year) {
    if (si.n_workinjury >= MAX_WORKINJURY) return -1;
    workinjury_t* w = &workinjuries[si.n_workinjury];
    w->workinjury_id = si.n_workinjury;
    w->injury_category = inj_cat;
    w->industry_risk = risk;
    w->participants_enrolled = enrolled;
    w->injuries_recognized = recognized;
    w->compensation_paid = compensation;
    w->year = year;
    w->active = 1;
    si.total_injuries += recognized;
    si.n_workinjury++;
    print_str("[SI] WorkInjury "); print_int(si.n_workinjury - 1);
    print_str(" cat="); print_int(inj_cat);
    print_str(" rsk="); print_int(risk);
    print_str(" enr="); print_int(enrolled);
    print_str(" rec="); print_int(recognized);
    print_str(" cmp="); print_int(compensation); print_str("\n");
    return si.n_workinjury - 1;
}

int si_maternity(int mat_type, int ben_cat, int enrolled, int allowances, int med_svc, int year) {
    if (si.n_maternity >= MAX_MATERNITY) return -1;
    maternity_t* m = &maternities[si.n_maternity];
    m->maternity_id = si.n_maternity;
    m->maternity_type = mat_type;
    m->beneficiary_category = ben_cat;
    m->participants_enrolled = enrolled;
    m->allowances_paid = allowances;
    m->medical_services = med_svc;
    m->year = year;
    m->active = 1;
    si.total_beneficiaries += allowances;
    si.n_maternity++;
    print_str("[SI] Maternity "); print_int(si.n_maternity - 1);
    print_str(" type="); print_int(mat_type);
    print_str(" cat="); print_int(ben_cat);
    print_str(" enr="); print_int(enrolled);
    print_str(" all="); print_int(allowances);
    print_str(" med="); print_int(med_svc); print_str("\n");
    return si.n_maternity - 1;
}

int si_fund_mgmt(int fund_cat, int source, int collected, int invested, int surplus, int year) {
    if (si.n_fund >= MAX_SI_FUND) return -1;
    si_fund_t* f = &si_funds[si.n_fund];
    f->fund_id = si.n_fund;
    f->fund_category = fund_cat;
    f->fund_source = source;
    f->funds_collected = collected;
    f->funds_invested = invested;
    f->fund_surplus = surplus;
    f->year = year;
    f->active = 1;
    si.total_funds += collected;
    si.n_fund++;
    print_str("[SI] Fund "); print_int(si.n_fund - 1);
    print_str(" cat="); print_int(fund_cat);
    print_str(" src="); print_int(source);
    print_str(" col="); print_int(collected);
    print_str(" inv="); print_int(invested);
    print_str(" spr="); print_int(surplus); print_str("\n");
    return si.n_fund - 1;
}

void si_pension_report(void) {
    print_str("[SI] Pension report:\n");
    print_str("  Pension categories: "); print_int(si.n_pension); print_str("\n");
    print_str("  Total pensions paid: "); print_int(si.total_pensioners); print_str("\n");
}

void si_unemployment_report(void) {
    print_str("[SI] Unemployment report:\n");
    print_str("  Unemployment units: "); print_int(si.n_unemployment); print_str("\n");
    print_str("  Total benefits paid: "); print_int(si.total_unemployed); print_str("\n");
}

void si_workinjury_report(void) {
    print_str("[SI] Work injury report:\n");
    print_str("  Work injury categories: "); print_int(si.n_workinjury); print_str("\n");
    print_str("  Total injuries recognized: "); print_int(si.total_injuries); print_str("\n");
    print_str("  Maternity units: "); print_int(si.n_maternity); print_str("\n");
    print_str("  Total beneficiaries: "); print_int(si.total_beneficiaries); print_str("\n");
    print_str("  Fund categories: "); print_int(si.n_fund); print_str("\n");
    print_str("  Total funds collected: "); print_int(si.total_funds); print_str("\n");
}

void si_print_state(void) {
    print_str("[SI] Pn="); print_int(si.n_pension);
    print_str(" Un="); print_int(si.n_unemployment);
    print_str(" Wi="); print_int(si.n_workinjury);
    print_str(" Mt="); print_int(si.n_maternity);
    print_str(" Fn="); print_int(si.n_fund);
    print_str("\n");
}

int main(void) {
    print_str("=== Social Insurance Demo ===\n\n");
    si_init();

    print_str("Pension insurance...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int enr = 100000 + (i * 20000);
        int paid = 50000 + (i * 10000);
        int adj = 3 + (i % 5);
        int year = 2020 + (i % 5);
        si_pension(type, cat, enr, paid, adj, year);
    }

    print_str("\nUnemployment insurance...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int rgn = (i % 8) + 1;
        int enr = 80000 + (i * 15000);
        int ben = 5000 + (i * 1000);
        int rem = 3000 + (i * 600);
        int year = 2021 + (i % 4);
        si_unemployment(type, rgn, enr, ben, rem, year);
    }

    print_str("\nWork injury insurance...\n");
    for (int i = 0; i < 12; i++) {
        int cat = (i % 5) + 1;
        int rsk = (i % 4) + 1;
        int enr = 60000 + (i * 12000);
        int rec = 500 + (i * 100);
        int cmp = 300 + (i * 60);
        int year = 2022 + (i % 3);
        si_workinjury(cat, rsk, enr, rec, cmp, year);
    }

    print_str("\nMaternity insurance...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 3) + 1;
        int cat = (i % 4) + 1;
        int enr = 70000 + (i * 15000);
        int all = 8000 + (i * 2000);
        int med = 5000 + (i * 1200);
        int year = 2023 + (i % 2);
        si_maternity(type, cat, enr, all, med, year);
    }

    print_str("\nFund management...\n");
    for (int i = 0; i < 10; i++) {
        int cat = (i % 5) + 1;
        int src = (i % 4) + 1;
        int col = 20000 + (i * 5000);
        int inv = 10000 + (i * 3000);
        int spr = 5000 + (i * 1500);
        int year = 2024;
        si_fund_mgmt(cat, src, col, inv, spr, year);
    }

    print_str("\nPension report...\n");
    si_pension_report();

    print_str("\nUnemployment report...\n");
    si_unemployment_report();

    print_str("\nWork injury report...\n");
    si_workinjury_report();

    print_str("\nFinal state...\n");
    si_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
