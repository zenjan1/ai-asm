/* audit_office: Financial audit management (v1.0)
 * Financial audits, performance audits, tracking, internal controls, reports
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

#define MAX_AUDITS     16
#define MAX_PERF       14
#define MAX_TRACKING   12
#define MAX_CONTROLS   10
#define MAX_REPORTS    10

typedef struct {
    int    audit_id;
    int    type;
    int    entity;
    int    zone;
    int    amount;
    int    findings;
    int    status;
    int    year;
    int    active;
} audit_t;

typedef struct {
    int    perf_id;
    int    project_id;
    int    efficiency;
    int    effectiveness;
    int    economy;
    int    score;
    int    year;
    int    active;
} performance_t;

typedef struct {
    int    track_id;
    int    audit_id;
    int    issue;
    int    amount_involved;
    int    corrected;
    int    person_accountable;
    int    status;
    int    active;
} tracking_t;

typedef struct {
    int    control_id;
    int    entity;
    int    type;
    int    risk_level;
    int    assessment_score;
    int    recommendations;
    int    year;
    int    active;
} control_t;

typedef struct {
    int    report_id;
    int    type;
    int    audit_count;
    int    total_findings;
    int    total_amount;
    int    corrected_count;
    int    year;
    int    published;
    int    active;
} report_t;

typedef struct {
    int    n_audits;
    int    n_perf;
    int    n_tracking;
    int    n_controls;
    int    n_reports;
    int    total_amount_audited;
    int    total_findings;
    int    total_corrected;
    int    total_persons_accountable;
    int    total_risk_assessed;
} ao_state_t;

static audit_t audits[MAX_AUDITS];
static performance_t performances[MAX_PERF];
static tracking_t trackings[MAX_TRACKING];
static control_t controls[MAX_CONTROLS];
static report_t reports[MAX_REPORTS];
static ao_state_t ao;

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

int ao_init(void) {
    if (initialized) return -1;
    ao.n_audits = 0; ao.n_perf = 0; ao.n_tracking = 0;
    ao.n_controls = 0; ao.n_reports = 0;
    ao.total_amount_audited = 0; ao.total_findings = 0;
    ao.total_corrected = 0; ao.total_persons_accountable = 0;
    ao.total_risk_assessed = 0;
    for (int i = 0; i < MAX_AUDITS; i++) audits[i].active = 0;
    for (int i = 0; i < MAX_PERF; i++) performances[i].active = 0;
    for (int i = 0; i < MAX_TRACKING; i++) trackings[i].active = 0;
    for (int i = 0; i < MAX_CONTROLS; i++) controls[i].active = 0;
    for (int i = 0; i < MAX_REPORTS; i++) reports[i].active = 0;
    initialized = 1;
    print_str("[AO] Audit office initialized\n");
    return 0;
}

int ao_add_audit(int type, int entity, int zone, int amount, int findings, int year) {
    if (ao.n_audits >= MAX_AUDITS) return -1;
    audit_t* a = &audits[ao.n_audits];
    a->audit_id = ao.n_audits;
    a->type = type;
    a->entity = entity;
    a->zone = zone;
    a->amount = amount;
    a->findings = findings;
    a->status = 1;
    a->year = year;
    a->active = 1;
    ao.total_amount_audited += amount;
    ao.total_findings += findings;
    ao.n_audits++;
    print_str("[AO] Audit "); print_int(ao.n_audits - 1);
    print_str(" type="); print_int(type);
    print_str(" ent="); print_int(entity);
    print_str(" zone="); print_int(zone);
    print_str(" amt=$"); print_int(amount);
    print_str(" find="); print_int(findings); print_str("\n");
    return ao.n_audits - 1;
}

int ao_complete_audit(int audit_id) {
    if (audit_id >= ao.n_audits) return -1;
    audits[audit_id].status = 2;
    print_str("[AO] Complete Au"); print_int(audit_id); print_str("\n");
    return 0;
}

int ao_add_performance(int project_id, int efficiency, int effectiveness, int economy, int year) {
    if (ao.n_perf >= MAX_PERF) return -1;
    performance_t* p = &performances[ao.n_perf];
    p->perf_id = ao.n_perf;
    p->project_id = project_id;
    p->efficiency = efficiency;
    p->effectiveness = effectiveness;
    p->economy = economy;
    p->score = (efficiency + effectiveness + economy) / 3;
    p->year = year;
    p->active = 1;
    ao.n_perf++;
    print_str("[AO] Perf "); print_int(ao.n_perf - 1);
    print_str(" proj="); print_int(project_id);
    print_str(" eff="); print_int(efficiency);
    print_str(" effv="); print_int(effectiveness);
    print_str(" econ="); print_int(economy);
    print_str(" score="); print_int(p->score); print_str("\n");
    return ao.n_perf - 1;
}

int ao_add_tracking(int audit_id, int issue, int amount_involved, int person) {
    if (ao.n_tracking >= MAX_TRACKING) return -1;
    tracking_t* t = &trackings[ao.n_tracking];
    t->track_id = ao.n_tracking;
    t->audit_id = audit_id;
    t->issue = issue;
    t->amount_involved = amount_involved;
    t->corrected = 0;
    t->person_accountable = person;
    t->status = 1;
    t->active = 1;
    ao.total_persons_accountable++;
    ao.n_tracking++;
    print_str("[AO] Track "); print_int(ao.n_tracking - 1);
    print_str(" au="); print_int(audit_id);
    print_str(" issue="); print_int(issue);
    print_str(" amt=$"); print_int(amount_involved);
    print_str(" pers="); print_int(person); print_str("\n");
    return ao.n_tracking - 1;
}

int ao_correct_issue(int track_id) {
    if (track_id >= ao.n_tracking) return -1;
    trackings[track_id].corrected = 1;
    trackings[track_id].status = 2;
    ao.total_corrected++;
    print_str("[AO] Correct T"); print_int(track_id); print_str("\n");
    return 0;
}

int ao_assess_control(int entity, int type, int risk_level, int score, int recommendations, int year) {
    if (ao.n_controls >= MAX_CONTROLS) return -1;
    control_t* c = &controls[ao.n_controls];
    c->control_id = ao.n_controls;
    c->entity = entity;
    c->type = type;
    c->risk_level = risk_level;
    c->assessment_score = score;
    c->recommendations = recommendations;
    c->year = year;
    c->active = 1;
    ao.total_risk_assessed += risk_level;
    ao.n_controls++;
    print_str("[AO] Control "); print_int(ao.n_controls - 1);
    print_str(" ent="); print_int(entity);
    print_str(" risk="); print_int(risk_level);
    print_str(" score="); print_int(score);
    print_str(" rec="); print_int(recommendations); print_str("\n");
    return ao.n_controls - 1;
}

int ao_publish_report(int type, int audit_count, int total_findings, int total_amount, int corrected, int year) {
    if (ao.n_reports >= MAX_REPORTS) return -1;
    report_t* r = &reports[ao.n_reports];
    r->report_id = ao.n_reports;
    r->type = type;
    r->audit_count = audit_count;
    r->total_findings = total_findings;
    r->total_amount = total_amount;
    r->corrected_count = corrected;
    r->year = year;
    r->published = 1;
    r->active = 1;
    ao.n_reports++;
    print_str("[AO] Report "); print_int(ao.n_reports - 1);
    print_str(" type="); print_int(type);
    print_str(" audits="); print_int(audit_count);
    print_str(" find="); print_int(total_findings);
    print_str(" amt=$"); print_int(total_amount); print_str("\n");
    return ao.n_reports - 1;
}

void ao_audit_report(void) {
    print_str("[AO] Audit report:\n");
    print_str("  Audits: "); print_int(ao.n_audits); print_str("\n");
    print_str("  Amount audited: $"); print_int(ao.total_amount_audited); print_str("\n");
    print_str("  Findings: "); print_int(ao.total_findings); print_str("\n");
}

void ao_tracking_report(void) {
    print_str("[AO] Tracking report:\n");
    print_str("  Issues tracked: "); print_int(ao.n_tracking); print_str("\n");
    print_str("  Corrected: "); print_int(ao.total_corrected); print_str("\n");
    print_str("  Persons accountable: "); print_int(ao.total_persons_accountable); print_str("\n");
}

void ao_control_report(void) {
    print_str("[AO] Control report:\n");
    print_str("  Assessments: "); print_int(ao.n_controls); print_str("\n");
    int avg_risk = ao.n_controls > 0 ? ao.total_risk_assessed / ao.n_controls : 0;
    print_str("  Avg risk: "); print_int(avg_risk); print_str("\n");
}

void ao_print_state(void) {
    print_str("[AO] Au="); print_int(ao.n_audits);
    print_str(" Pf="); print_int(ao.n_perf);
    print_str(" Tk="); print_int(ao.n_tracking);
    print_str(" Ct="); print_int(ao.n_controls);
    print_str(" Rp="); print_int(ao.n_reports);
    print_str("\n");
}

int main(void) {
    print_str("=== Audit Office Demo ===\n\n");
    ao_init();

    print_str("Adding audits...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 4) + 1;
        int entity = (i % 8) + 1;
        int zone = (i % 4) + 1;
        int amount = 1000000 + (i * 500000);
        int findings = 2 + (i % 8);
        int year = 2022 + (i % 3);
        ao_add_audit(type, entity, zone, amount, findings, year);
    }

    print_str("\nCompleting audits...\n");
    for (int i = 0; i < 16; i++) {
        ao_complete_audit(i);
    }

    print_str("\nPerformance assessments...\n");
    for (int i = 0; i < 14; i++) {
        int proj = (i % 10) + 1;
        int eff = 70 + (i % 30);
        int effv = 65 + (i % 35);
        int econ = 60 + (i % 40);
        int year = 2024;
        ao_add_performance(proj, eff, effv, econ, year);
    }

    print_str("\nTracking issues...\n");
    for (int i = 0; i < 12; i++) {
        int au = i % 16;
        int issue = (i % 5) + 1;
        int amt = 50000 + (i * 20000);
        int person = (i % 10) + 1;
        ao_add_tracking(au, issue, amt, person);
    }

    print_str("\nCorrecting issues...\n");
    for (int i = 0; i < 9; i++) {
        ao_correct_issue(i);
    }

    print_str("\nAssessing controls...\n");
    for (int i = 0; i < 10; i++) {
        int entity = (i % 8) + 1;
        int type = (i % 3) + 1;
        int risk = (i % 5) + 1;
        int score = 60 + (i % 40);
        int rec = 3 + (i % 5);
        int year = 2024;
        ao_assess_control(entity, type, risk, score, rec, year);
    }

    print_str("\nPublishing reports...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 3) + 1;
        int au_count = 5 + (i * 2);
        int findings = 10 + (i * 3);
        int amount = 500000 + (i * 200000);
        int corrected = 8 + (i * 2);
        int year = 2020 + i;
        ao_publish_report(type, au_count, findings, amount, corrected, year);
    }

    print_str("\nAudit report...\n");
    ao_audit_report();

    print_str("\nTracking report...\n");
    ao_tracking_report();

    print_str("\nControl report...\n");
    ao_control_report();

    print_str("\nFinal state...\n");
    ao_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
