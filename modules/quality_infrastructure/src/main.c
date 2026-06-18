/* quality_infrastructure: Quality infrastructure administration system (v1.0)
 * Standards, metrology, certification, inspection, quality improvement
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

#define MAX_STANDARD     16
#define MAX_METROLOGY    14
#define MAX_CERTIFICATION 12
#define MAX_INSPECTION   10
#define MAX_IMPROVEMENT  10

typedef struct {
    int    standard_id;
    int    standard_type;
    int    standard_level;
    int    standards_developed;
    int    standards_revised;
    int    standards_implemented;
    int    year;
    int    active;
} standard_t;

typedef struct {
    int    metrology_id;
    int    metrology_type;
    int    measurement_category;
    int    standards_established;
    int    verifications_done;
    int    supervision_ops;
    int    year;
    int    active;
} metrology_t;

typedef struct {
    int    certification_id;
    int    cert_type;
    int    cert_category;
    int    certs_issued;
    int    certs_renewed;
    int    certs_suspended;
    int    year;
    int    active;
} certification_t;

typedef struct {
    int    inspection_id;
    int    inspection_type;
    int    testing_category;
    int    labs_accredited;
    int    tests_conducted;
    int    reports_issued;
    int    year;
    int    active;
} inspection_t;

typedef struct {
    int    improvement_id;
    int    improvement_type;
    int    quality_area;
    int    projects_launched;
    int    demonstrations_made;
    int    awards_given;
    int    year;
    int    active;
} improvement_t;

typedef struct {
    int    n_standard;
    int    n_metrology;
    int    n_certification;
    int    n_inspection;
    int    n_improvement;
    int    total_standards;
    int    total_verifications;
    int    total_certs;
    int    total_tests;
    int    total_projects;
} qi_state_t;

static standard_t standards[MAX_STANDARD];
static metrology_t metrologies[MAX_METROLOGY];
static certification_t certifications[MAX_CERTIFICATION];
static inspection_t inspections[MAX_INSPECTION];
static improvement_t improvements[MAX_IMPROVEMENT];
static qi_state_t qi;

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

int qi_init(void) {
    if (initialized) return -1;
    qi.n_standard = 0; qi.n_metrology = 0; qi.n_certification = 0;
    qi.n_inspection = 0; qi.n_improvement = 0;
    qi.total_standards = 0; qi.total_verifications = 0;
    qi.total_certs = 0; qi.total_tests = 0;
    qi.total_projects = 0;
    for (int i = 0; i < MAX_STANDARD; i++) standards[i].active = 0;
    for (int i = 0; i < MAX_METROLOGY; i++) metrologies[i].active = 0;
    for (int i = 0; i < MAX_CERTIFICATION; i++) certifications[i].active = 0;
    for (int i = 0; i < MAX_INSPECTION; i++) inspections[i].active = 0;
    for (int i = 0; i < MAX_IMPROVEMENT; i++) improvements[i].active = 0;
    initialized = 1;
    print_str("[QI] Quality infrastructure initialized\n");
    return 0;
}

int qi_standard(int std_type, int level, int developed, int revised, int implemented, int year) {
    if (qi.n_standard >= MAX_STANDARD) return -1;
    standard_t* s = &standards[qi.n_standard];
    s->standard_id = qi.n_standard;
    s->standard_type = std_type;
    s->standard_level = level;
    s->standards_developed = developed;
    s->standards_revised = revised;
    s->standards_implemented = implemented;
    s->year = year;
    s->active = 1;
    qi.total_standards += developed;
    qi.n_standard++;
    print_str("[QI] Standard "); print_int(qi.n_standard - 1);
    print_str(" type="); print_int(std_type);
    print_str(" lvl="); print_int(level);
    print_str(" dvp="); print_int(developed);
    print_str(" rvs="); print_int(revised);
    print_str(" imp="); print_int(implemented); print_str("\n");
    return qi.n_standard - 1;
}

int qi_metrology(int met_type, int category, int established, int verifications, int supervision, int year) {
    if (qi.n_metrology >= MAX_METROLOGY) return -1;
    metrology_t* m = &metrologies[qi.n_metrology];
    m->metrology_id = qi.n_metrology;
    m->metrology_type = met_type;
    m->measurement_category = category;
    m->standards_established = established;
    m->verifications_done = verifications;
    m->supervision_ops = supervision;
    m->year = year;
    m->active = 1;
    qi.total_verifications += verifications;
    qi.n_metrology++;
    print_str("[QI] Metrology "); print_int(qi.n_metrology - 1);
    print_str(" type="); print_int(met_type);
    print_str(" cat="); print_int(category);
    print_str(" est="); print_int(established);
    print_str(" vrf="); print_int(verifications);
    print_str(" sup="); print_int(supervision); print_str("\n");
    return qi.n_metrology - 1;
}

int qi_certification(int cert_type, int category, int issued, int renewed, int suspended, int year) {
    if (qi.n_certification >= MAX_CERTIFICATION) return -1;
    certification_t* c = &certifications[qi.n_certification];
    c->certification_id = qi.n_certification;
    c->cert_type = cert_type;
    c->cert_category = category;
    c->certs_issued = issued;
    c->certs_renewed = renewed;
    c->certs_suspended = suspended;
    c->year = year;
    c->active = 1;
    qi.total_certs += issued;
    qi.n_certification++;
    print_str("[QI] Certification "); print_int(qi.n_certification - 1);
    print_str(" type="); print_int(cert_type);
    print_str(" cat="); print_int(category);
    print_str(" iss="); print_int(issued);
    print_str(" rnw="); print_int(renewed);
    print_str(" ssp="); print_int(suspended); print_str("\n");
    return qi.n_certification - 1;
}

int qi_inspection(int insp_type, int testing, int labs, int tests, int reports, int year) {
    if (qi.n_inspection >= MAX_INSPECTION) return -1;
    inspection_t* ins = &inspections[qi.n_inspection];
    ins->inspection_id = qi.n_inspection;
    ins->inspection_type = insp_type;
    ins->testing_category = testing;
    ins->labs_accredited = labs;
    ins->tests_conducted = tests;
    ins->reports_issued = reports;
    ins->year = year;
    ins->active = 1;
    qi.total_tests += tests;
    qi.n_inspection++;
    print_str("[QI] Inspection "); print_int(qi.n_inspection - 1);
    print_str(" type="); print_int(insp_type);
    print_str(" tst="); print_int(testing);
    print_str(" lab="); print_int(labs);
    print_str(" tsc="); print_int(tests);
    print_str(" rpt="); print_int(reports); print_str("\n");
    return qi.n_inspection - 1;
}

int qi_improvement(int imp_type, int area, int launched, int demonstrations, int awards, int year) {
    if (qi.n_improvement >= MAX_IMPROVEMENT) return -1;
    improvement_t* im = &improvements[qi.n_improvement];
    im->improvement_id = qi.n_improvement;
    im->improvement_type = imp_type;
    im->quality_area = area;
    im->projects_launched = launched;
    im->demonstrations_made = demonstrations;
    im->awards_given = awards;
    im->year = year;
    im->active = 1;
    qi.total_projects += launched;
    qi.n_improvement++;
    print_str("[QI] Improvement "); print_int(qi.n_improvement - 1);
    print_str(" type="); print_int(imp_type);
    print_str(" area="); print_int(area);
    print_str(" lch="); print_int(launched);
    print_str(" dmn="); print_int(demonstrations);
    print_str(" awd="); print_int(awards); print_str("\n");
    return qi.n_improvement - 1;
}

void qi_standard_report(void) {
    print_str("[QI] Standard report:\n");
    print_str("  Standard categories: "); print_int(qi.n_standard); print_str("\n");
    print_str("  Total standards developed: "); print_int(qi.total_standards); print_str("\n");
}

void qi_metrology_report(void) {
    print_str("[QI] Metrology report:\n");
    print_str("  Metrology categories: "); print_int(qi.n_metrology); print_str("\n");
    print_str("  Total verifications: "); print_int(qi.total_verifications); print_str("\n");
}

void qi_improvement_report(void) {
    print_str("[QI] Improvement report:\n");
    print_str("  Certification categories: "); print_int(qi.n_certification); print_str("\n");
    print_str("  Total certifications: "); print_int(qi.total_certs); print_str("\n");
    print_str("  Inspection categories: "); print_int(qi.n_inspection); print_str("\n");
    print_str("  Total tests conducted: "); print_int(qi.total_tests); print_str("\n");
    print_str("  Improvement categories: "); print_int(qi.n_improvement); print_str("\n");
    print_str("  Total projects launched: "); print_int(qi.total_projects); print_str("\n");
}

void qi_print_state(void) {
    print_str("[QI] St="); print_int(qi.n_standard);
    print_str(" Mt="); print_int(qi.n_metrology);
    print_str(" Ct="); print_int(qi.n_certification);
    print_str(" In="); print_int(qi.n_inspection);
    print_str(" Im="); print_int(qi.n_improvement);
    print_str("\n");
}

int main(void) {
    print_str("=== Quality Infrastructure Demo ===\n\n");
    qi_init();

    print_str("Standard management...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int lvl = (i % 4) + 1;
        int dvp = 30 + (i * 8);
        int rvs = 15 + (i * 4);
        int imp = 25 + (i * 6);
        int year = 2020 + (i % 5);
        qi_standard(type, lvl, dvp, rvs, imp, year);
    }

    print_str("\nMetrology management...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int est = 10 + (i * 3);
        int vrf = 200 + (i * 50);
        int sup = 50 + (i * 12);
        int year = 2021 + (i % 4);
        qi_metrology(type, cat, est, vrf, sup, year);
    }

    print_str("\nCertification...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int iss = 100 + (i * 25);
        int rnw = 60 + (i * 15);
        int ssp = 5 + (i % 8);
        int year = 2022 + (i % 3);
        qi_certification(type, cat, iss, rnw, ssp, year);
    }

    print_str("\nInspection testing...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int tst = (i % 5) + 1;
        int lab = 20 + (i * 5);
        int tsc = 500 + (i * 120);
        int rpt = 400 + (i * 100);
        int year = 2023 + (i % 2);
        qi_inspection(type, tst, lab, tsc, rpt, year);
    }

    print_str("\nQuality improvement...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int area = (i % 5) + 1;
        int lch = 15 + (i * 4);
        int dmn = 8 + (i * 2);
        int awd = 5 + (i % 5);
        int year = 2024;
        qi_improvement(type, area, lch, dmn, awd, year);
    }

    print_str("\nStandard report...\n");
    qi_standard_report();

    print_str("\nMetrology report...\n");
    qi_metrology_report();

    print_str("\nImprovement report...\n");
    qi_improvement_report();

    print_str("\nFinal state...\n");
    qi_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
