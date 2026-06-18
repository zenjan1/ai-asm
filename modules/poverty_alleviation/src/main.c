/* poverty_alleviation: Poverty alleviation administration system (v1.0)
 * Poverty management, industry support, education aid, health aid, relocation
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

#define MAX_POVERTY      16
#define MAX_INDUSTRY     14
#define MAX_EDUCATION    12
#define MAX_HEALTH       10
#define MAX_RELOCATION   10

typedef struct {
    int    poverty_id;
    int    poverty_type;
    int    region_id;
    int    households_identified;
    int    persons_identified;
    int    funds_allocated;
    int    year;
    int    active;
} poverty_t;

typedef struct {
    int    industry_id;
    int    industry_type;
    int    project_scale;
    int    projects_launched;
    int    households_driven;
    int    income_generated;
    int    year;
    int    active;
} industry_t;

typedef struct {
    int    education_id;
    int    aid_type;
    int    education_level;
    int    students_assisted;
    int    training_sessions;
    int    employment_placed;
    int    year;
    int    active;
} education_t;

typedef struct {
    int    health_id;
    int    health_aid_type;
    int    disease_category;
    int    patients_assisted;
    int    medical_expenses;
    int    insurance_coverage;
    int    year;
    int    active;
} health_t;

typedef struct {
    int    relocation_id;
    int    relocation_type;
    int    settlement_type;
    int    households_relocated;
    int    housing_built;
    int    follow_up_services;
    int    year;
    int    active;
} relocation_t;

typedef struct {
    int    n_poverty;
    int    n_industry;
    int    n_education;
    int    n_health;
    int    n_relocation;
    int    total_households;
    int    total_projects;
    int    total_students;
    int    total_patients;
    int    total_relocated;
} pa_state_t;

static poverty_t poverty_list[MAX_POVERTY];
static industry_t industries[MAX_INDUSTRY];
static education_t educations[MAX_EDUCATION];
static health_t health_list[MAX_HEALTH];
static relocation_t relocations[MAX_RELOCATION];
static pa_state_t pa;

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

int pa_init(void) {
    if (initialized) return -1;
    pa.n_poverty = 0; pa.n_industry = 0; pa.n_education = 0;
    pa.n_health = 0; pa.n_relocation = 0;
    pa.total_households = 0; pa.total_projects = 0;
    pa.total_students = 0; pa.total_patients = 0;
    pa.total_relocated = 0;
    for (int i = 0; i < MAX_POVERTY; i++) poverty_list[i].active = 0;
    for (int i = 0; i < MAX_INDUSTRY; i++) industries[i].active = 0;
    for (int i = 0; i < MAX_EDUCATION; i++) educations[i].active = 0;
    for (int i = 0; i < MAX_HEALTH; i++) health_list[i].active = 0;
    for (int i = 0; i < MAX_RELOCATION; i++) relocations[i].active = 0;
    initialized = 1;
    print_str("[PA] Poverty alleviation initialized\n");
    return 0;
}

int pa_poverty(int pov_type, int region, int households_cnt, int persons, int funds, int year) {
    if (pa.n_poverty >= MAX_POVERTY) return -1;
    poverty_t* p = &poverty_list[pa.n_poverty];
    p->poverty_id = pa.n_poverty;
    p->poverty_type = pov_type;
    p->region_id = region;
    p->households_identified = households_cnt;
    p->persons_identified = persons;
    p->funds_allocated = funds;
    p->year = year;
    p->active = 1;
    pa.total_households += households_cnt;
    pa.n_poverty++;
    print_str("[PA] Poverty "); print_int(pa.n_poverty - 1);
    print_str(" type="); print_int(pov_type);
    print_str(" rgn="); print_int(region);
    print_str(" hhd="); print_int(households_cnt);
    print_str(" per="); print_int(persons);
    print_str(" fnd="); print_int(funds); print_str("\n");
    return pa.n_poverty - 1;
}

int pa_industry(int ind_type, int scale, int launched, int driven, int income, int year) {
    if (pa.n_industry >= MAX_INDUSTRY) return -1;
    industry_t* ind = &industries[pa.n_industry];
    ind->industry_id = pa.n_industry;
    ind->industry_type = ind_type;
    ind->project_scale = scale;
    ind->projects_launched = launched;
    ind->households_driven = driven;
    ind->income_generated = income;
    ind->year = year;
    ind->active = 1;
    pa.total_projects += launched;
    pa.n_industry++;
    print_str("[PA] Industry "); print_int(pa.n_industry - 1);
    print_str(" type="); print_int(ind_type);
    print_str(" scl="); print_int(scale);
    print_str(" lch="); print_int(launched);
    print_str(" drv="); print_int(driven);
    print_str(" inc="); print_int(income); print_str("\n");
    return pa.n_industry - 1;
}

int pa_education(int aid_type, int level, int students, int sessions, int employed, int year) {
    if (pa.n_education >= MAX_EDUCATION) return -1;
    education_t* e = &educations[pa.n_education];
    e->education_id = pa.n_education;
    e->aid_type = aid_type;
    e->education_level = level;
    e->students_assisted = students;
    e->training_sessions = sessions;
    e->employment_placed = employed;
    e->year = year;
    e->active = 1;
    pa.total_students += students;
    pa.n_education++;
    print_str("[PA] Education "); print_int(pa.n_education - 1);
    print_str(" type="); print_int(aid_type);
    print_str(" lvl="); print_int(level);
    print_str(" stu="); print_int(students);
    print_str(" trn="); print_int(sessions);
    print_str(" emp="); print_int(employed); print_str("\n");
    return pa.n_education - 1;
}

int pa_health(int aid_type, int disease, int patients, int expenses, int coverage, int year) {
    if (pa.n_health >= MAX_HEALTH) return -1;
    health_t* h = &health_list[pa.n_health];
    h->health_id = pa.n_health;
    h->health_aid_type = aid_type;
    h->disease_category = disease;
    h->patients_assisted = patients;
    h->medical_expenses = expenses;
    h->insurance_coverage = coverage;
    h->year = year;
    h->active = 1;
    pa.total_patients += patients;
    pa.n_health++;
    print_str("[PA] Health "); print_int(pa.n_health - 1);
    print_str(" type="); print_int(aid_type);
    print_str(" dis="); print_int(disease);
    print_str(" pat="); print_int(patients);
    print_str(" exp="); print_int(expenses);
    print_str(" cov="); print_int(coverage); print_str("\n");
    return pa.n_health - 1;
}

int pa_relocation(int rel_type, int settlement, int relocated_cnt, int housing_units, int follow_up, int year) {
    if (pa.n_relocation >= MAX_RELOCATION) return -1;
    relocation_t* r = &relocations[pa.n_relocation];
    r->relocation_id = pa.n_relocation;
    r->relocation_type = rel_type;
    r->settlement_type = settlement;
    r->households_relocated = relocated_cnt;
    r->housing_built = housing_units;
    r->follow_up_services = follow_up;
    r->year = year;
    r->active = 1;
    pa.total_relocated += relocated_cnt;
    pa.n_relocation++;
    print_str("[PA] Relocation "); print_int(pa.n_relocation - 1);
    print_str(" type="); print_int(rel_type);
    print_str(" stl="); print_int(settlement);
    print_str(" rlc="); print_int(relocated_cnt);
    print_str(" hsg="); print_int(housing_units);
    print_str(" flw="); print_int(follow_up); print_str("\n");
    return pa.n_relocation - 1;
}

void pa_poverty_report(void) {
    print_str("[PA] Poverty report:\n");
    print_str("  Poverty categories: "); print_int(pa.n_poverty); print_str("\n");
    print_str("  Total households identified: "); print_int(pa.total_households); print_str("\n");
}

void pa_industry_report(void) {
    print_str("[PA] Industry report:\n");
    print_str("  Industry categories: "); print_int(pa.n_industry); print_str("\n");
    print_str("  Total projects launched: "); print_int(pa.total_projects); print_str("\n");
}

void pa_relocation_report(void) {
    print_str("[PA] Relocation report:\n");
    print_str("  Education aid categories: "); print_int(pa.n_education); print_str("\n");
    print_str("  Total students assisted: "); print_int(pa.total_students); print_str("\n");
    print_str("  Health aid categories: "); print_int(pa.n_health); print_str("\n");
    print_str("  Total patients assisted: "); print_int(pa.total_patients); print_str("\n");
    print_str("  Relocation categories: "); print_int(pa.n_relocation); print_str("\n");
    print_str("  Total households relocated: "); print_int(pa.total_relocated); print_str("\n");
}

void pa_print_state(void) {
    print_str("[PA] Pv="); print_int(pa.n_poverty);
    print_str(" In="); print_int(pa.n_industry);
    print_str(" Ed="); print_int(pa.n_education);
    print_str(" Hl="); print_int(pa.n_health);
    print_str(" Rl="); print_int(pa.n_relocation);
    print_str("\n");
}

int main(void) {
    print_str("=== Poverty Alleviation Demo ===\n\n");
    pa_init();

    print_str("Poverty identification...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int rgn = (i % 8) + 1;
        int hhd = 500 + (i * 100);
        int per = 2000 + (i * 400);
        int fnd = 300 + (i * 60);
        int year = 2020 + (i % 5);
        pa_poverty(type, rgn, hhd, per, fnd, year);
    }

    print_str("\nIndustry support...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 5) + 1;
        int scl = (i % 4) + 1;
        int lch = 20 + (i * 5);
        int drv = 100 + (i * 25);
        int inc = 500 + (i * 100);
        int year = 2021 + (i % 4);
        pa_industry(type, scl, lch, drv, inc, year);
    }

    print_str("\nEducation aid...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int lvl = (i % 4) + 1;
        int stu = 300 + (i * 80);
        int trn = 50 + (i * 12);
        int emp = 100 + (i * 25);
        int year = 2022 + (i % 3);
        pa_education(type, lvl, stu, trn, emp, year);
    }

    print_str("\nHealth aid...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int dis = (i % 5) + 1;
        int pat = 200 + (i * 50);
        int exp = 100 + (i * 25);
        int cov = 70 + (i * 3);
        int year = 2023 + (i % 2);
        pa_health(type, dis, pat, exp, cov, year);
    }

    print_str("\nRelocation...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int stl = (i % 3) + 1;
        int rlc = 150 + (i * 35);
        int hsg = 120 + (i * 30);
        int flw = 80 + (i * 20);
        int year = 2024;
        pa_relocation(type, stl, rlc, hsg, flw, year);
    }

    print_str("\nPoverty report...\n");
    pa_poverty_report();

    print_str("\nIndustry report...\n");
    pa_industry_report();

    print_str("\nRelocation report...\n");
    pa_relocation_report();

    print_str("\nFinal state...\n");
    pa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
