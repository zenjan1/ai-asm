/* ethnic_economy: Ethnic economy development administration system (v1.0)
 * Ethnic industry, cultural protection, economic support, education health, regional cooperation
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

#define MAX_INDUSTRY     16
#define MAX_CULTURE      14
#define MAX_SUPPORT      12
#define MAX_EDUCATION    10
#define MAX_COOPERATION  10

typedef struct {
    int    industry_id;
    int    industry_type;
    int    ethnic_category;
    int    specialty_industries;
    int    handicraft;
    int    ethnic_medicine;
    int    year;
    int    active;
} industry_t;

typedef struct {
    int    culture_id;
    int    culture_type;
    int    heritage_category;
    int    cultural_heritage;
    int    language_preservation;
    int    festival_events;
    int    year;
    int    active;
} culture_t;

typedef struct {
    int    support_id;
    int    support_type;
    int    support_area;
    int    investments;
    int    fiscal_transfers;
    int    partner_assistance;
    int    year;
    int    active;
} support_t;

typedef struct {
    int    education_id;
    int    education_type;
    int    education_category;
    int    ethnic_schools;
    int    health_facilities;
    int    talent_training;
    int    year;
    int    active;
} education_t;

typedef struct {
    int    cooperation_id;
    int    cooperation_type;
    int    cooperation_area;
    int    regional_cooperation;
    int    cross_regional;
    int    border_trade;
    int    year;
    int    active;
} cooperation_t;

typedef struct {
    int    n_industry;
    int    n_culture;
    int    n_support;
    int    n_education;
    int    n_cooperation;
    int    total_specialty;
    int    total_heritage;
    int    total_investment;
    int    total_schools;
    int    total_cooperation;
} ee_state_t;

static industry_t industries[MAX_INDUSTRY];
static culture_t cultures[MAX_CULTURE];
static support_t supports[MAX_SUPPORT];
static education_t educations[MAX_EDUCATION];
static cooperation_t cooperations[MAX_COOPERATION];
static ee_state_t ee;

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

int ee_init(void) {
    if (initialized) return -1;
    ee.n_industry = 0; ee.n_culture = 0; ee.n_support = 0;
    ee.n_education = 0; ee.n_cooperation = 0;
    ee.total_specialty = 0; ee.total_heritage = 0;
    ee.total_investment = 0; ee.total_schools = 0;
    ee.total_cooperation = 0;
    for (int i = 0; i < MAX_INDUSTRY; i++) industries[i].active = 0;
    for (int i = 0; i < MAX_CULTURE; i++) cultures[i].active = 0;
    for (int i = 0; i < MAX_SUPPORT; i++) supports[i].active = 0;
    for (int i = 0; i < MAX_EDUCATION; i++) educations[i].active = 0;
    for (int i = 0; i < MAX_COOPERATION; i++) cooperations[i].active = 0;
    initialized = 1;
    print_str("[EE] Ethnic economy initialized\n");
    return 0;
}

int ee_industry(int ind_type, int cat, int specialty, int handicraft, int medicine, int year) {
    if (ee.n_industry >= MAX_INDUSTRY) return -1;
    industry_t* ind = &industries[ee.n_industry];
    ind->industry_id = ee.n_industry;
    ind->industry_type = ind_type;
    ind->ethnic_category = cat;
    ind->specialty_industries = specialty;
    ind->handicraft = handicraft;
    ind->ethnic_medicine = medicine;
    ind->year = year;
    ind->active = 1;
    ee.total_specialty += specialty;
    ee.n_industry++;
    print_str("[EE] Industry "); print_int(ee.n_industry - 1);
    print_str(" type="); print_int(ind_type);
    print_str(" cat="); print_int(cat);
    print_str(" spc="); print_int(specialty);
    print_str(" hnc="); print_int(handicraft);
    print_str(" mdc="); print_int(medicine); print_str("\n");
    return ee.n_industry - 1;
}

int ee_culture(int cul_type, int cat, int heritage, int language, int festival, int year) {
    if (ee.n_culture >= MAX_CULTURE) return -1;
    culture_t* c = &cultures[ee.n_culture];
    c->culture_id = ee.n_culture;
    c->culture_type = cul_type;
    c->heritage_category = cat;
    c->cultural_heritage = heritage;
    c->language_preservation = language;
    c->festival_events = festival;
    c->year = year;
    c->active = 1;
    ee.total_heritage += heritage;
    ee.n_culture++;
    print_str("[EE] Culture "); print_int(ee.n_culture - 1);
    print_str(" type="); print_int(cul_type);
    print_str(" cat="); print_int(cat);
    print_str(" hrt="); print_int(heritage);
    print_str(" lng="); print_int(language);
    print_str(" fst="); print_int(festival); print_str("\n");
    return ee.n_culture - 1;
}

int ee_support(int sup_type, int area, int investments, int transfers, int assistance, int year) {
    if (ee.n_support >= MAX_SUPPORT) return -1;
    support_t* s = &supports[ee.n_support];
    s->support_id = ee.n_support;
    s->support_type = sup_type;
    s->support_area = area;
    s->investments = investments;
    s->fiscal_transfers = transfers;
    s->partner_assistance = assistance;
    s->year = year;
    s->active = 1;
    ee.total_investment += investments;
    ee.n_support++;
    print_str("[EE] Support "); print_int(ee.n_support - 1);
    print_str(" type="); print_int(sup_type);
    print_str(" area="); print_int(area);
    print_str(" inv="); print_int(investments);
    print_str(" trf="); print_int(transfers);
    print_str(" ast="); print_int(assistance); print_str("\n");
    return ee.n_support - 1;
}

int ee_education(int edu_type, int cat, int schools, int health, int talent, int year) {
    if (ee.n_education >= MAX_EDUCATION) return -1;
    education_t* e = &educations[ee.n_education];
    e->education_id = ee.n_education;
    e->education_type = edu_type;
    e->education_category = cat;
    e->ethnic_schools = schools;
    e->health_facilities = health;
    e->talent_training = talent;
    e->year = year;
    e->active = 1;
    ee.total_schools += schools;
    ee.n_education++;
    print_str("[EE] Education "); print_int(ee.n_education - 1);
    print_str(" type="); print_int(edu_type);
    print_str(" cat="); print_int(cat);
    print_str(" sch="); print_int(schools);
    print_str(" hlt="); print_int(health);
    print_str(" tlt="); print_int(talent); print_str("\n");
    return ee.n_education - 1;
}

int ee_cooperation(int cop_type, int area, int regional, int cross_regional, int border, int year) {
    if (ee.n_cooperation >= MAX_COOPERATION) return -1;
    cooperation_t* c = &cooperations[ee.n_cooperation];
    c->cooperation_id = ee.n_cooperation;
    c->cooperation_type = cop_type;
    c->cooperation_area = area;
    c->regional_cooperation = regional;
    c->cross_regional = cross_regional;
    c->border_trade = border;
    c->year = year;
    c->active = 1;
    ee.total_cooperation += regional;
    ee.n_cooperation++;
    print_str("[EE] Cooperation "); print_int(ee.n_cooperation - 1);
    print_str(" type="); print_int(cop_type);
    print_str(" area="); print_int(area);
    print_str(" rgn="); print_int(regional);
    print_str(" crs="); print_int(cross_regional);
    print_str(" bdr="); print_int(border); print_str("\n");
    return ee.n_cooperation - 1;
}

void ee_industry_report(void) {
    print_str("[EE] Industry report:\n");
    print_str("  Ethnic industry categories: "); print_int(ee.n_industry); print_str("\n");
    print_str("  Total specialty industries: "); print_int(ee.total_specialty); print_str("\n");
}

void ee_culture_report(void) {
    print_str("[EE] Culture report:\n");
    print_str("  Cultural categories: "); print_int(ee.n_culture); print_str("\n");
    print_str("  Total cultural heritage: "); print_int(ee.total_heritage); print_str("\n");
}

void ee_cooperation_report(void) {
    print_str("[EE] Cooperation report:\n");
    print_str("  Support categories: "); print_int(ee.n_support); print_str("\n");
    print_str("  Total investments: "); print_int(ee.total_investment); print_str("\n");
    print_str("  Education categories: "); print_int(ee.n_education); print_str("\n");
    print_str("  Total ethnic schools: "); print_int(ee.total_schools); print_str("\n");
    print_str("  Cooperation categories: "); print_int(ee.n_cooperation); print_str("\n");
    print_str("  Total regional cooperation: "); print_int(ee.total_cooperation); print_str("\n");
}

void ee_print_state(void) {
    print_str("[EE] In="); print_int(ee.n_industry);
    print_str(" Cl="); print_int(ee.n_culture);
    print_str(" Sp="); print_int(ee.n_support);
    print_str(" Ed="); print_int(ee.n_education);
    print_str(" Cp="); print_int(ee.n_cooperation);
    print_str("\n");
}

int main(void) {
    print_str("=== Ethnic Economy Demo ===\n\n");
    ee_init();

    print_str("Ethnic industry...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int spc = 15 + (i * 4);
        int hnc = 20 + (i * 5);
        int mdc = 8 + (i * 2);
        int year = 2020 + (i % 5);
        ee_industry(type, cat, spc, hnc, mdc, year);
    }

    print_str("\nCultural protection...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int hrt = 10 + (i * 3);
        int lng = 5 + (i * 2);
        int fst = 8 + (i * 2);
        int year = 2021 + (i % 4);
        ee_culture(type, cat, hrt, lng, fst, year);
    }

    print_str("\nEconomic support...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int area = (i % 5) + 1;
        int inv = 100 + (i * 25);
        int trf = 200 + (i * 50);
        int ast = 80 + (i * 20);
        int year = 2022 + (i % 3);
        ee_support(type, area, inv, trf, ast, year);
    }

    print_str("\nEducation health...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int sch = 25 + (i * 6);
        int hlt = 15 + (i * 4);
        int tlt = 10 + (i * 3);
        int year = 2023 + (i % 2);
        ee_education(type, cat, sch, hlt, tlt, year);
    }

    print_str("\nRegional cooperation...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int area = (i % 5) + 1;
        int rgn = 12 + (i * 3);
        int crs = 8 + (i * 2);
        int bdr = 15 + (i * 4);
        int year = 2024;
        ee_cooperation(type, area, rgn, crs, bdr, year);
    }

    print_str("\nIndustry report...\n");
    ee_industry_report();

    print_str("\nCulture report...\n");
    ee_culture_report();

    print_str("\nCooperation report...\n");
    ee_cooperation_report();

    print_str("\nFinal state...\n");
    ee_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
