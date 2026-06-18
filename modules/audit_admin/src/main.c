/* audit_admin: Audit administration system (v1.0)
 * Fiscal audit, performance audit, compliance, IT audit, reports
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

#define MAX_FISCAL       16
#define MAX_PERFORMANCE  14
#define MAX_COMPLIANCE   12
#define MAX_ITAUDIT      10
#define MAX_REPORT       10

typedef struct {
    int    fiscal_id;
    int    auditee_id;
    int    budget_type;
    int    budget_amount;
    int    expenditure;
    int    discrepancy;
    int    year;
    int    active;
} fiscal_t;

typedef struct {
    int    performance_id;
    int    project_id;
    int    program_type;
    int    target_score;
    int    actual_score;
    int    efficiency;
    int    year;
    int    active;
} performance_t;

typedef struct {
    int    compliance_id;
    int    entity_id;
    int    compliance_type;
    int    controls_tested;
    int    exceptions;
    int    risk_score;
    int    year;
    int    active;
} compliance_t;

typedef struct {
    int    itaudit_id;
    int    system_id;
    int    audit_type;
    int    vulnerabilities;
    int    security_score;
    int    remediation;
    int    year;
    int    active;
} itaudit_t;

typedef struct {
    int    report_id;
    int    audit_id;
    int    report_type;
    int    findings;
    int    recommendations;
    int    implemented;
    int    year;
    int    active;
} report_t;

typedef struct {
    int    n_fiscal;
    int    n_performance;
    int    n_compliance;
    int    n_itaudit;
    int    n_report;
    int    total_budget;
    int    total_expenditure;
    int    total_findings;
    int    total_recommendations;
    int    total_discrepancy;
} aud_state_t;

static fiscal_t fiscals[MAX_FISCAL];
static performance_t performances[MAX_PERFORMANCE];
static compliance_t compliances[MAX_COMPLIANCE];
static itaudit_t itaudits[MAX_ITAUDIT];
static report_t reports[MAX_REPORT];
static aud_state_t aud;

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

int aud_init(void) {
    if (initialized) return -1;
    aud.n_fiscal = 0; aud.n_performance = 0; aud.n_compliance = 0;
    aud.n_itaudit = 0; aud.n_report = 0;
    aud.total_budget = 0; aud.total_expenditure = 0;
    aud.total_findings = 0; aud.total_recommendations = 0;
    aud.total_discrepancy = 0;
    for (int i = 0; i < MAX_FISCAL; i++) fiscals[i].active = 0;
    for (int i = 0; i < MAX_PERFORMANCE; i++) performances[i].active = 0;
    for (int i = 0; i < MAX_COMPLIANCE; i++) compliances[i].active = 0;
    for (int i = 0; i < MAX_ITAUDIT; i++) itaudits[i].active = 0;
    for (int i = 0; i < MAX_REPORT; i++) reports[i].active = 0;
    initialized = 1;
    print_str("[AUD] Audit admin initialized\n");
    return 0;
}

int aud_fiscal(int auditee, int budget_type, int budget, int expenditure, int discrepancy, int year) {
    if (aud.n_fiscal >= MAX_FISCAL) return -1;
    fiscal_t* f = &fiscals[aud.n_fiscal];
    f->fiscal_id = aud.n_fiscal;
    f->auditee_id = auditee;
    f->budget_type = budget_type;
    f->budget_amount = budget;
    f->expenditure = expenditure;
    f->discrepancy = discrepancy;
    f->year = year;
    f->active = 1;
    aud.total_budget += budget;
    aud.total_expenditure += expenditure;
    aud.total_discrepancy += discrepancy;
    aud.n_fiscal++;
    print_str("[AUD] Fiscal "); print_int(aud.n_fiscal - 1);
    print_str(" adt="); print_int(auditee);
    print_str(" type="); print_int(budget_type);
    print_str(" bgt=$"); print_int(budget);
    print_str(" exp=$"); print_int(expenditure);
    print_str(" dsc=$"); print_int(discrepancy); print_str("\n");
    return aud.n_fiscal - 1;
}

int aud_performance(int project, int program_type, int target, int actual, int efficiency, int year) {
    if (aud.n_performance >= MAX_PERFORMANCE) return -1;
    performance_t* p = &performances[aud.n_performance];
    p->performance_id = aud.n_performance;
    p->project_id = project;
    p->program_type = program_type;
    p->target_score = target;
    p->actual_score = actual;
    p->efficiency = efficiency;
    p->year = year;
    p->active = 1;
    aud.n_performance++;
    print_str("[AUD] Performance "); print_int(aud.n_performance - 1);
    print_str(" prj="); print_int(project);
    print_str(" type="); print_int(program_type);
    print_str(" tgt="); print_int(target);
    print_str(" act="); print_int(actual);
    print_str(" eff="); print_int(efficiency); print_str("%\n");
    return aud.n_performance - 1;
}

int aud_compliance(int entity, int compliance_type, int controls, int exceptions, int risk_score, int year) {
    if (aud.n_compliance >= MAX_COMPLIANCE) return -1;
    compliance_t* c = &compliances[aud.n_compliance];
    c->compliance_id = aud.n_compliance;
    c->entity_id = entity;
    c->compliance_type = compliance_type;
    c->controls_tested = controls;
    c->exceptions = exceptions;
    c->risk_score = risk_score;
    c->year = year;
    c->active = 1;
    aud.n_compliance++;
    print_str("[AUD] Compliance "); print_int(aud.n_compliance - 1);
    print_str(" ent="); print_int(entity);
    print_str(" type="); print_int(compliance_type);
    print_str(" ctl="); print_int(controls);
    print_str(" exc="); print_int(exceptions);
    print_str(" rsk="); print_int(risk_score); print_str("\n");
    return aud.n_compliance - 1;
}

int aud_itaudit(int system, int audit_type, int vulnerabilities, int security_score, int remediation, int year) {
    if (aud.n_itaudit >= MAX_ITAUDIT) return -1;
    itaudit_t* it = &itaudits[aud.n_itaudit];
    it->itaudit_id = aud.n_itaudit;
    it->system_id = system;
    it->audit_type = audit_type;
    it->vulnerabilities = vulnerabilities;
    it->security_score = security_score;
    it->remediation = remediation;
    it->year = year;
    it->active = 1;
    aud.n_itaudit++;
    print_str("[AUD] ITAudit "); print_int(aud.n_itaudit - 1);
    print_str(" sys="); print_int(system);
    print_str(" type="); print_int(audit_type);
    print_str(" vln="); print_int(vulnerabilities);
    print_str(" sec="); print_int(security_score);
    print_str(" rmd="); print_int(remediation); print_str("\n");
    return aud.n_itaudit - 1;
}

int aud_report(int audit_id, int report_type, int findings, int recommendations, int implemented, int year) {
    if (aud.n_report >= MAX_REPORT) return -1;
    report_t* r = &reports[aud.n_report];
    r->report_id = aud.n_report;
    r->audit_id = audit_id;
    r->report_type = report_type;
    r->findings = findings;
    r->recommendations = recommendations;
    r->implemented = implemented;
    r->year = year;
    r->active = 1;
    aud.total_findings += findings;
    aud.total_recommendations += recommendations;
    aud.n_report++;
    print_str("[AUD] Report "); print_int(aud.n_report - 1);
    print_str(" aid="); print_int(audit_id);
    print_str(" type="); print_int(report_type);
    print_str(" fnd="); print_int(findings);
    print_str(" rec="); print_int(recommendations);
    print_str(" imp="); print_int(implemented); print_str("\n");
    return aud.n_report - 1;
}

void aud_fiscal_report(void) {
    print_str("[AUD] Fiscal report:\n");
    print_str("  Audits conducted: "); print_int(aud.n_fiscal); print_str("\n");
    print_str("  Total budget: $"); print_int(aud.total_budget); print_str("\n");
    print_str("  Total expenditure: $"); print_int(aud.total_expenditure); print_str("\n");
    print_str("  Total discrepancy: $"); print_int(aud.total_discrepancy); print_str("\n");
}

void aud_performance_report(void) {
    print_str("[AUD] Performance report:\n");
    print_str("  Performance audits: "); print_int(aud.n_performance); print_str("\n");
}

void aud_compliance_report(void) {
    print_str("[AUD] Compliance report:\n");
    print_str("  Compliance audits: "); print_int(aud.n_compliance); print_str("\n");
    print_str("  IT audits: "); print_int(aud.n_itaudit); print_str("\n");
    print_str("  Audit reports: "); print_int(aud.n_report); print_str("\n");
    print_str("  Total findings: "); print_int(aud.total_findings); print_str("\n");
    print_str("  Total recommendations: "); print_int(aud.total_recommendations); print_str("\n");
}

void aud_print_state(void) {
    print_str("[AUD] Fs="); print_int(aud.n_fiscal);
    print_str(" Pf="); print_int(aud.n_performance);
    print_str(" Cm="); print_int(aud.n_compliance);
    print_str(" It="); print_int(aud.n_itaudit);
    print_str(" Rp="); print_int(aud.n_report);
    print_str("\n");
}

int main(void) {
    print_str("=== Audit Admin Demo ===\n\n");
    aud_init();

    print_str("Fiscal audits...\n");
    for (int i = 0; i < 16; i++) {
        int adt = 100 + (i * 10);
        int type = (i % 4) + 1;
        int bgt = 1000000 + (i * 500000);
        int exp = bgt - (i * 50000);
        int dsc = (i % 5 == 0) ? bgt / 20 : 0;
        int year = 2020 + (i % 5);
        aud_fiscal(adt, type, bgt, exp, dsc, year);
    }

    print_str("\nPerformance audits...\n");
    for (int i = 0; i < 14; i++) {
        int prj = 200 + (i * 8);
        int type = (i % 5) + 1;
        int tgt = 85 + (i % 10);
        int act = tgt - (i % 8);
        int eff = 70 + (i * 2);
        int year = 2021 + (i % 4);
        aud_performance(prj, type, tgt, act, eff, year);
    }

    print_str("\nCompliance audits...\n");
    for (int i = 0; i < 12; i++) {
        int ent = 300 + (i * 7);
        int type = (i % 3) + 1;
        int ctl = 50 + (i * 5);
        int exc = i % 8;
        int rsk = 20 + (i * 5);
        int year = 2022 + (i % 3);
        aud_compliance(ent, type, ctl, exc, rsk, year);
    }

    print_str("\nIT audits...\n");
    for (int i = 0; i < 10; i++) {
        int sys = 400 + (i * 11);
        int type = (i % 3) + 1;
        int vln = 3 + (i % 6);
        int sec = 80 - (i * 2);
        int rmd = vln - (i % 3);
        int year = 2023 + (i % 2);
        aud_itaudit(sys, type, vln, sec, rmd, year);
    }

    print_str("\nAudit reports...\n");
    for (int i = 0; i < 10; i++) {
        int aid = 500 + (i * 13);
        int type = (i % 3) + 1;
        int fnd = 5 + (i * 2);
        int rec = fnd + 3;
        int imp = rec - (i % 4);
        int year = 2024;
        aud_report(aid, type, fnd, rec, imp, year);
    }

    print_str("\nFiscal report...\n");
    aud_fiscal_report();

    print_str("\nPerformance report...\n");
    aud_performance_report();

    print_str("\nCompliance report...\n");
    aud_compliance_report();

    print_str("\nFinal state...\n");
    aud_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
