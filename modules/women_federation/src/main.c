/* women_federation: Women federation system (v1.0)
 * Rights protection, development, family, care services, organization
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

#define MAX_RIGHTS       16
#define MAX_DEVELOP      14
#define MAX_FAMILY       12
#define MAX_CARE         10
#define MAX_ORG          10

typedef struct {
    int    rights_id;
    int    woman_id;
    int    case_type;
    int    lawyer_id;
    int    sessions;
    int    resolved;
    int    year;
    int    status;
    int    active;
} rights_t;

typedef struct {
    int    develop_id;
    int    woman_id;
    int    program_type;
    int    training_hours;
    int    skills;
    int    employed;
    int    income;
    int    year;
    int    active;
} develop_t;

typedef struct {
    int    family_id;
    int    couple_id;
    int    counseling_type;
    int    sessions;
    int    children;
    int    outcome;
    int    year;
    int    active;
} family_t;

typedef struct {
    int    care_id;
    int    woman_id;
    int    service_type;
    int    screening;
    int    subsidy;
    int    children_helped;
    int    year;
    int    active;
} care_t;

typedef struct {
    int    org_id;
    int    region_id;
    int    members;
    int    executives;
    int    activities;
    int    funding;
    int    year;
    int    active;
} org_t;

typedef struct {
    int    n_rights;
    int    n_develop;
    int    n_family;
    int    n_care;
    int    n_org;
    int    total_resolved;
    int    total_employed;
    int    total_training;
    int    total_screenings;
    int    total_members;
} wf_state_t;

static rights_t rights_cases[MAX_RIGHTS];
static develop_t develops[MAX_DEVELOP];
static family_t families[MAX_FAMILY];
static care_t cares[MAX_CARE];
static org_t organizations[MAX_ORG];
static wf_state_t wf;

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

int wf_init(void) {
    if (initialized) return -1;
    wf.n_rights = 0; wf.n_develop = 0; wf.n_family = 0;
    wf.n_care = 0; wf.n_org = 0;
    wf.total_resolved = 0; wf.total_employed = 0;
    wf.total_training = 0; wf.total_screenings = 0;
    wf.total_members = 0;
    for (int i = 0; i < MAX_RIGHTS; i++) rights_cases[i].active = 0;
    for (int i = 0; i < MAX_DEVELOP; i++) develops[i].active = 0;
    for (int i = 0; i < MAX_FAMILY; i++) families[i].active = 0;
    for (int i = 0; i < MAX_CARE; i++) cares[i].active = 0;
    for (int i = 0; i < MAX_ORG; i++) organizations[i].active = 0;
    initialized = 1;
    print_str("[WF] Women federation initialized\n");
    return 0;
}

int wf_rights(int woman, int case_type, int lawyer, int sessions, int resolved, int year) {
    if (wf.n_rights >= MAX_RIGHTS) return -1;
    rights_t* r = &rights_cases[wf.n_rights];
    r->rights_id = wf.n_rights;
    r->woman_id = woman;
    r->case_type = case_type;
    r->lawyer_id = lawyer;
    r->sessions = sessions;
    r->resolved = resolved;
    r->year = year;
    r->status = 1;
    r->active = 1;
    if (resolved) wf.total_resolved++;
    wf.n_rights++;
    print_str("[WF] Rights "); print_int(wf.n_rights - 1);
    print_str(" wom="); print_int(woman);
    print_str(" type="); print_int(case_type);
    print_str(" lwy="); print_int(lawyer);
    print_str(" ses="); print_int(sessions);
    print_str(" res="); print_int(resolved); print_str("\n");
    return wf.n_rights - 1;
}

int wf_develop(int woman, int program, int training, int skills, int employed, int income, int year) {
    if (wf.n_develop >= MAX_DEVELOP) return -1;
    develop_t* d = &develops[wf.n_develop];
    d->develop_id = wf.n_develop;
    d->woman_id = woman;
    d->program_type = program;
    d->training_hours = training;
    d->skills = skills;
    d->employed = employed;
    d->income = income;
    d->year = year;
    d->active = 1;
    wf.total_training += training;
    if (employed) wf.total_employed++;
    wf.n_develop++;
    print_str("[WF] Develop "); print_int(wf.n_develop - 1);
    print_str(" wom="); print_int(woman);
    print_str(" prg="); print_int(program);
    print_str(" trn="); print_int(training); print_str("h");
    print_str(" skl="); print_int(skills);
    print_str(" emp="); print_int(employed); print_str("\n");
    return wf.n_develop - 1;
}

int wf_family(int couple, int counseling, int sessions, int children, int outcome, int year) {
    if (wf.n_family >= MAX_FAMILY) return -1;
    family_t* f = &families[wf.n_family];
    f->family_id = wf.n_family;
    f->couple_id = couple;
    f->counseling_type = counseling;
    f->sessions = sessions;
    f->children = children;
    f->outcome = outcome;
    f->year = year;
    f->active = 1;
    wf.n_family++;
    print_str("[WF] Family "); print_int(wf.n_family - 1);
    print_str(" cpl="); print_int(couple);
    print_str(" cns="); print_int(counseling);
    print_str(" ses="); print_int(sessions);
    print_str(" chn="); print_int(children);
    print_str(" out="); print_int(outcome); print_str("\n");
    return wf.n_family - 1;
}

int wf_care(int woman, int service, int screening, int subsidy, int children, int year) {
    if (wf.n_care >= MAX_CARE) return -1;
    care_t* c = &cares[wf.n_care];
    c->care_id = wf.n_care;
    c->woman_id = woman;
    c->service_type = service;
    c->screening = screening;
    c->subsidy = subsidy;
    c->children_helped = children;
    c->year = year;
    c->active = 1;
    if (screening) wf.total_screenings++;
    wf.n_care++;
    print_str("[WF] Care "); print_int(wf.n_care - 1);
    print_str(" wom="); print_int(woman);
    print_str(" svc="); print_int(service);
    print_str(" scr="); print_int(screening);
    print_str(" sub=$"); print_int(subsidy);
    print_str(" chn="); print_int(children); print_str("\n");
    return wf.n_care - 1;
}

int wf_org(int region, int members, int executives, int activities, int funding, int year) {
    if (wf.n_org >= MAX_ORG) return -1;
    org_t* o = &organizations[wf.n_org];
    o->org_id = wf.n_org;
    o->region_id = region;
    o->members = members;
    o->executives = executives;
    o->activities = activities;
    o->funding = funding;
    o->year = year;
    o->active = 1;
    wf.total_members += members;
    wf.n_org++;
    print_str("[WF] Org "); print_int(wf.n_org - 1);
    print_str(" reg="); print_int(region);
    print_str(" mem="); print_int(members);
    print_str(" exv="); print_int(executives);
    print_str(" act="); print_int(activities);
    print_str(" fnd=$"); print_int(funding); print_str("\n");
    return wf.n_org - 1;
}

void wf_rights_report(void) {
    print_str("[WF] Rights report:\n");
    print_str("  Cases: "); print_int(wf.n_rights); print_str("\n");
    print_str("  Resolved: "); print_int(wf.total_resolved); print_str("\n");
}

void wf_develop_report(void) {
    print_str("[WF] Development report:\n");
    print_str("  Programs: "); print_int(wf.n_develop); print_str("\n");
    print_str("  Total training hours: "); print_int(wf.total_training); print_str("\n");
    print_str("  Total employed: "); print_int(wf.total_employed); print_str("\n");
}

void wf_care_report(void) {
    print_str("[WF] Care report:\n");
    print_str("  Care services: "); print_int(wf.n_care); print_str("\n");
    print_str("  Screenings: "); print_int(wf.total_screenings); print_str("\n");
    print_str("  Family counseling: "); print_int(wf.n_family); print_str("\n");
    print_str("  Organizations: "); print_int(wf.n_org); print_str("\n");
    print_str("  Total members: "); print_int(wf.total_members); print_str("\n");
}

void wf_print_state(void) {
    print_str("[WF] Rt="); print_int(wf.n_rights);
    print_str(" Dv="); print_int(wf.n_develop);
    print_str(" Fm="); print_int(wf.n_family);
    print_str(" Cr="); print_int(wf.n_care);
    print_str(" Og="); print_int(wf.n_org);
    print_str("\n");
}

int main(void) {
    print_str("=== Women Federation Demo ===\n\n");
    wf_init();

    print_str("Rights protection...\n");
    for (int i = 0; i < 16; i++) {
        int wom = 1000 + (i * 13);
        int ctype = (i % 4) + 1;
        int lwy = 200 + (i % 8);
        int ses = 1 + (i % 5);
        int res = (i % 3 == 0) ? 0 : 1;
        int year = 2021 + (i % 4);
        wf_rights(wom, ctype, lwy, ses, res, year);
    }

    print_str("\nDevelopment programs...\n");
    for (int i = 0; i < 14; i++) {
        int wom = 2000 + (i * 11);
        int prg = (i % 5) + 1;
        int trn = 20 + (i * 10);
        int skl = 1 + (i % 4);
        int emp = (i % 3 == 0) ? 0 : 1;
        int inc = 2000 + (i * 300);
        int year = 2022 + (i % 3);
        wf_develop(wom, prg, trn, skl, emp, inc, year);
    }

    print_str("\nFamily building...\n");
    for (int i = 0; i < 12; i++) {
        int cpl = 3000 + (i * 7);
        int cns = (i % 3) + 1;
        int ses = 2 + (i % 5);
        int chn = (i % 4);
        int out = (i % 3) + 1;
        int year = 2023 + (i % 2);
        wf_family(cpl, cns, ses, chn, out, year);
    }

    print_str("\nCare services...\n");
    for (int i = 0; i < 10; i++) {
        int wom = 4000 + (i * 17);
        int svc = (i % 4) + 1;
        int scr = (i % 3 == 0) ? 1 : 0;
        int sub = 500 + (i * 200);
        int chn = i % 3;
        int year = 2024;
        wf_care(wom, svc, scr, sub, chn, year);
    }

    print_str("\nOrganization building...\n");
    for (int i = 0; i < 10; i++) {
        int reg = (i % 6) + 1;
        int mem = 50 + (i * 20);
        int exv = 3 + (i % 5);
        int act = 5 + (i * 3);
        int fnd = 10000 + (i * 5000);
        int year = 2024;
        wf_org(reg, mem, exv, act, fnd, year);
    }

    print_str("\nRights report...\n");
    wf_rights_report();

    print_str("\nDevelopment report...\n");
    wf_develop_report();

    print_str("\nCare report...\n");
    wf_care_report();

    print_str("\nFinal state...\n");
    wf_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
