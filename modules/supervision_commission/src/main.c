/* supervision_commission: Discipline inspection and supervision system (v1.0)
 * Integrity building, case investigation, inspection tour, accountability, international cooperation
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

#define MAX_INTEGRITY    16
#define MAX_CASE         14
#define MAX_INSPECTION   12
#define MAX_ACCOUNT      10
#define MAX_INTER        10

typedef struct {
    int    integrity_id;
    int    department_id;
    int    education_type;
    int    education_sessions;
    int    prevention_measures;
    int    supervision_count;
    int    year;
    int    active;
} integrity_t;

typedef struct {
    int    case_id;
    int    clue_type;
    int    investigation_type;
    int    clues_received;
    int    cases_filed;
    int    cases_closed;
    int    year;
    int    active;
} case_t;

typedef struct {
    int    inspection_id;
    int    unit_id;
    int    inspection_type;
    int    problems_found;
    int    rectified;
    int    transferred;
    int    year;
    int    active;
} inspection_t;

typedef struct {
    int    account_id;
    int    official_id;
    int    accountability_type;
    int    accountability_decisions;
    int    disciplinary_actions;
    int    appeals;
    int    year;
    int    active;
} account_t;

typedef struct {
    int    inter_id;
    int    country_id;
    int    cooperation_type;
    int    fugitives_pursued;
    int    assets_recovered;
    int    extraditions;
    int    year;
    int    active;
} inter_t;

typedef struct {
    int    n_integrity;
    int    n_case;
    int    n_inspection;
    int    n_account;
    int    n_inter;
    int    total_education;
    int    total_clues;
    int    total_cases_filed;
    int    total_problems;
    int    total_assets;
} sc_state_t;

static integrity_t integrities[MAX_INTEGRITY];
static case_t cases[MAX_CASE];
static inspection_t inspections[MAX_INSPECTION];
static account_t accounts[MAX_ACCOUNT];
static inter_t inters[MAX_INTER];
static sc_state_t sc;

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

int sc_init(void) {
    if (initialized) return -1;
    sc.n_integrity = 0; sc.n_case = 0; sc.n_inspection = 0;
    sc.n_account = 0; sc.n_inter = 0;
    sc.total_education = 0; sc.total_clues = 0;
    sc.total_cases_filed = 0; sc.total_problems = 0;
    sc.total_assets = 0;
    for (int i = 0; i < MAX_INTEGRITY; i++) integrities[i].active = 0;
    for (int i = 0; i < MAX_CASE; i++) cases[i].active = 0;
    for (int i = 0; i < MAX_INSPECTION; i++) inspections[i].active = 0;
    for (int i = 0; i < MAX_ACCOUNT; i++) accounts[i].active = 0;
    for (int i = 0; i < MAX_INTER; i++) inters[i].active = 0;
    initialized = 1;
    print_str("[SC] Supervision commission initialized\n");
    return 0;
}

int sc_integrity(int dept, int edu_type, int sessions, int prevention, int supervision, int year) {
    if (sc.n_integrity >= MAX_INTEGRITY) return -1;
    integrity_t* ig = &integrities[sc.n_integrity];
    ig->integrity_id = sc.n_integrity;
    ig->department_id = dept;
    ig->education_type = edu_type;
    ig->education_sessions = sessions;
    ig->prevention_measures = prevention;
    ig->supervision_count = supervision;
    ig->year = year;
    ig->active = 1;
    sc.total_education += sessions;
    sc.n_integrity++;
    print_str("[SC] Integrity "); print_int(sc.n_integrity - 1);
    print_str(" dept="); print_int(dept);
    print_str(" edu="); print_int(edu_type);
    print_str(" ses="); print_int(sessions);
    print_str(" prev="); print_int(prevention);
    print_str(" sup="); print_int(supervision); print_str("\n");
    return sc.n_integrity - 1;
}

int sc_case(int clue_type, int inv_type, int clues, int filed, int closed, int year) {
    if (sc.n_case >= MAX_CASE) return -1;
    case_t* c = &cases[sc.n_case];
    c->case_id = sc.n_case;
    c->clue_type = clue_type;
    c->investigation_type = inv_type;
    c->clues_received = clues;
    c->cases_filed = filed;
    c->cases_closed = closed;
    c->year = year;
    c->active = 1;
    sc.total_clues += clues;
    sc.total_cases_filed += filed;
    sc.n_case++;
    print_str("[SC] Case "); print_int(sc.n_case - 1);
    print_str(" clue="); print_int(clue_type);
    print_str(" inv="); print_int(inv_type);
    print_str(" rcv="); print_int(clues);
    print_str(" fld="); print_int(filed);
    print_str(" cls="); print_int(closed); print_str("\n");
    return sc.n_case - 1;
}

int sc_inspection(int unit, int insp_type, int problems, int rectified, int transferred, int year) {
    if (sc.n_inspection >= MAX_INSPECTION) return -1;
    inspection_t* in = &inspections[sc.n_inspection];
    in->inspection_id = sc.n_inspection;
    in->unit_id = unit;
    in->inspection_type = insp_type;
    in->problems_found = problems;
    in->rectified = rectified;
    in->transferred = transferred;
    in->year = year;
    in->active = 1;
    sc.total_problems += problems;
    sc.n_inspection++;
    print_str("[SC] Inspection "); print_int(sc.n_inspection - 1);
    print_str(" unit="); print_int(unit);
    print_str(" type="); print_int(insp_type);
    print_str(" prob="); print_int(problems);
    print_str(" rec="); print_int(rectified);
    print_str(" trf="); print_int(transferred); print_str("\n");
    return sc.n_inspection - 1;
}

int sc_account(int official, int acc_type, int decisions, int actions, int appeals, int year) {
    if (sc.n_account >= MAX_ACCOUNT) return -1;
    account_t* a = &accounts[sc.n_account];
    a->account_id = sc.n_account;
    a->official_id = official;
    a->accountability_type = acc_type;
    a->accountability_decisions = decisions;
    a->disciplinary_actions = actions;
    a->appeals = appeals;
    a->year = year;
    a->active = 1;
    sc.n_account++;
    print_str("[SC] Account "); print_int(sc.n_account - 1);
    print_str(" off="); print_int(official);
    print_str(" type="); print_int(acc_type);
    print_str(" dec="); print_int(decisions);
    print_str(" act="); print_int(actions);
    print_str(" app="); print_int(appeals); print_str("\n");
    return sc.n_account - 1;
}

int sc_inter(int country, int coop_type, int fugitives, int assets, int extraditions, int year) {
    if (sc.n_inter >= MAX_INTER) return -1;
    inter_t* it = &inters[sc.n_inter];
    it->inter_id = sc.n_inter;
    it->country_id = country;
    it->cooperation_type = coop_type;
    it->fugitives_pursued = fugitives;
    it->assets_recovered = assets;
    it->extraditions = extraditions;
    it->year = year;
    it->active = 1;
    sc.total_assets += assets;
    sc.n_inter++;
    print_str("[SC] Inter "); print_int(sc.n_inter - 1);
    print_str(" cty="); print_int(country);
    print_str(" coop="); print_int(coop_type);
    print_str(" fug="); print_int(fugitives);
    print_str(" ast=$"); print_int(assets);
    print_str(" ext="); print_int(extraditions); print_str("\n");
    return sc.n_inter - 1;
}

void sc_integrity_report(void) {
    print_str("[SC] Integrity report:\n");
    print_str("  Departments: "); print_int(sc.n_integrity); print_str("\n");
    print_str("  Total education sessions: "); print_int(sc.total_education); print_str("\n");
}

void sc_case_report(void) {
    print_str("[SC] Case report:\n");
    print_str("  Case categories: "); print_int(sc.n_case); print_str("\n");
    print_str("  Total clues received: "); print_int(sc.total_clues); print_str("\n");
    print_str("  Total cases filed: "); print_int(sc.total_cases_filed); print_str("\n");
}

void sc_inspection_report(void) {
    print_str("[SC] Inspection report:\n");
    print_str("  Inspection tours: "); print_int(sc.n_inspection); print_str("\n");
    print_str("  Total problems found: "); print_int(sc.total_problems); print_str("\n");
    print_str("  Accountability cases: "); print_int(sc.n_account); print_str("\n");
    print_str("  International cooperation: "); print_int(sc.n_inter); print_str("\n");
    print_str("  Total assets recovered: $"); print_int(sc.total_assets); print_str("\n");
}

void sc_print_state(void) {
    print_str("[SC] In="); print_int(sc.n_integrity);
    print_str(" Cs="); print_int(sc.n_case);
    print_str(" Ip="); print_int(sc.n_inspection);
    print_str(" Ac="); print_int(sc.n_account);
    print_str(" Ir="); print_int(sc.n_inter);
    print_str("\n");
}

int main(void) {
    print_str("=== Supervision Commission Demo ===\n\n");
    sc_init();

    print_str("Integrity building...\n");
    for (int i = 0; i < 16; i++) {
        int dept = 100 + (i * 11);
        int edu = (i % 5) + 1;
        int ses = 20 + (i * 5);
        int prev = 10 + (i * 3);
        int sup = 30 + (i * 10);
        int year = 2020 + (i % 5);
        sc_integrity(dept, edu, ses, prev, sup, year);
    }

    print_str("\nCase investigation...\n");
    for (int i = 0; i < 14; i++) {
        int clue = (i % 6) + 1;
        int inv = (i % 4) + 1;
        int rcv = 50 + (i * 10);
        int fld = rcv - (i % 5);
        int cls = fld - (i % 3);
        int year = 2021 + (i % 4);
        sc_case(clue, inv, rcv, fld, cls, year);
    }

    print_str("\nInspection tours...\n");
    for (int i = 0; i < 12; i++) {
        int unit = 200 + (i * 13);
        int type = (i % 4) + 1;
        int prob = 15 + (i * 5);
        int rec = prob - (i % 3);
        int trf = i % 4;
        int year = 2022 + (i % 3);
        sc_inspection(unit, type, prob, rec, trf, year);
    }

    print_str("\nAccountability...\n");
    for (int i = 0; i < 10; i++) {
        int off = 300 + (i * 17);
        int type = (i % 4) + 1;
        int dec = 5 + (i * 2);
        int act = dec + (i % 3);
        int app = i % 3;
        int year = 2023 + (i % 2);
        sc_account(off, type, dec, act, app, year);
    }

    print_str("\nInternational cooperation...\n");
    for (int i = 0; i < 10; i++) {
        int cty = 400 + (i * 11);
        int coop = (i % 4) + 1;
        int fug = 3 + (i % 5);
        int ast = 1000000 + (i * 500000);
        int ext = i % 3;
        int year = 2024;
        sc_inter(cty, coop, fug, ast, ext, year);
    }

    print_str("\nIntegrity report...\n");
    sc_integrity_report();

    print_str("\nCase report...\n");
    sc_case_report();

    print_str("\nInspection report...\n");
    sc_inspection_report();

    print_str("\nFinal state...\n");
    sc_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
