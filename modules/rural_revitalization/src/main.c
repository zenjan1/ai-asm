/* rural_revitalization: Rural revitalization administration system (v1.0)
 * Industry, talent, culture, ecology, organization revitalization
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

#define MAX_INDUSTRY_RR  16
#define MAX_TALENT       14
#define MAX_CULTURE      12
#define MAX_ECOLOGY      10
#define MAX_ORGANIZATION 10

typedef struct {
    int    industry_id;
    int    industry_type;
    int    scale_level;
    int    farms_established;
    int    tourism_sites;
    int    ecommerce_volume;
    int    year;
    int    active;
} industry_rr_t;

typedef struct {
    int    talent_id;
    int    talent_category;
    int    training_type;
    int    talents_trained;
    int    talents_introduced;
    int    incentives_granted;
    int    year;
    int    active;
} talent_t;

typedef struct {
    int    culture_id;
    int    culture_type;
    int    heritage_category;
    int    cultural_events;
    int    traditions_preserved;
    int    civilization_score;
    int    year;
    int    active;
} culture_t;

typedef struct {
    int    ecology_id;
    int    ecology_type;
    int    environment_area;
    int    improvements_made;
    int    protection_areas;
    int    green_development;
    int    year;
    int    active;
} ecology_t;

typedef struct {
    int    org_id;
    int    org_type;
    int    region_id;
    int    party_branches;
    int    collective_income;
    int    self_governance;
    int    year;
    int    active;
} organization_t;

typedef struct {
    int    n_industry_rr;
    int    n_talent;
    int    n_culture;
    int    n_ecology;
    int    n_organization;
    int    total_farms;
    int    total_talents;
    int    total_events;
    int    total_improvements;
    int    total_branches;
} rr_state_t;

static industry_rr_t industries_rr[MAX_INDUSTRY_RR];
static talent_t talents[MAX_TALENT];
static culture_t cultures[MAX_CULTURE];
static ecology_t ecologies[MAX_ECOLOGY];
static organization_t organizations[MAX_ORGANIZATION];
static rr_state_t rr;

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

int rr_init(void) {
    if (initialized) return -1;
    rr.n_industry_rr = 0; rr.n_talent = 0; rr.n_culture = 0;
    rr.n_ecology = 0; rr.n_organization = 0;
    rr.total_farms = 0; rr.total_talents = 0;
    rr.total_events = 0; rr.total_improvements = 0;
    rr.total_branches = 0;
    for (int i = 0; i < MAX_INDUSTRY_RR; i++) industries_rr[i].active = 0;
    for (int i = 0; i < MAX_TALENT; i++) talents[i].active = 0;
    for (int i = 0; i < MAX_CULTURE; i++) cultures[i].active = 0;
    for (int i = 0; i < MAX_ECOLOGY; i++) ecologies[i].active = 0;
    for (int i = 0; i < MAX_ORGANIZATION; i++) organizations[i].active = 0;
    initialized = 1;
    print_str("[RR] Rural revitalization initialized\n");
    return 0;
}

int rr_industry(int ind_type, int scale, int farms, int tourism, int ecommerce, int year) {
    if (rr.n_industry_rr >= MAX_INDUSTRY_RR) return -1;
    industry_rr_t* ind = &industries_rr[rr.n_industry_rr];
    ind->industry_id = rr.n_industry_rr;
    ind->industry_type = ind_type;
    ind->scale_level = scale;
    ind->farms_established = farms;
    ind->tourism_sites = tourism;
    ind->ecommerce_volume = ecommerce;
    ind->year = year;
    ind->active = 1;
    rr.total_farms += farms;
    rr.n_industry_rr++;
    print_str("[RR] Industry "); print_int(rr.n_industry_rr - 1);
    print_str(" type="); print_int(ind_type);
    print_str(" scl="); print_int(scale);
    print_str(" frm="); print_int(farms);
    print_str(" trs="); print_int(tourism);
    print_str(" ecm="); print_int(ecommerce); print_str("\n");
    return rr.n_industry_rr - 1;
}

int rr_talent(int category, int train_type, int trained, int introduced, int incentives, int year) {
    if (rr.n_talent >= MAX_TALENT) return -1;
    talent_t* t = &talents[rr.n_talent];
    t->talent_id = rr.n_talent;
    t->talent_category = category;
    t->training_type = train_type;
    t->talents_trained = trained;
    t->talents_introduced = introduced;
    t->incentives_granted = incentives;
    t->year = year;
    t->active = 1;
    rr.total_talents += trained;
    rr.n_talent++;
    print_str("[RR] Talent "); print_int(rr.n_talent - 1);
    print_str(" cat="); print_int(category);
    print_str(" type="); print_int(train_type);
    print_str(" trn="); print_int(trained);
    print_str(" int="); print_int(introduced);
    print_str(" inc="); print_int(incentives); print_str("\n");
    return rr.n_talent - 1;
}

int rr_culture(int cul_type, int heritage, int events, int traditions, int civilization, int year) {
    if (rr.n_culture >= MAX_CULTURE) return -1;
    culture_t* c = &cultures[rr.n_culture];
    c->culture_id = rr.n_culture;
    c->culture_type = cul_type;
    c->heritage_category = heritage;
    c->cultural_events = events;
    c->traditions_preserved = traditions;
    c->civilization_score = civilization;
    c->year = year;
    c->active = 1;
    rr.total_events += events;
    rr.n_culture++;
    print_str("[RR] Culture "); print_int(rr.n_culture - 1);
    print_str(" type="); print_int(cul_type);
    print_str(" hrt="); print_int(heritage);
    print_str(" evt="); print_int(events);
    print_str(" trd="); print_int(traditions);
    print_str(" cvl="); print_int(civilization); print_str("\n");
    return rr.n_culture - 1;
}

int rr_ecology(int eco_type, int area, int improvements, int protection, int green_dev, int year) {
    if (rr.n_ecology >= MAX_ECOLOGY) return -1;
    ecology_t* e = &ecologies[rr.n_ecology];
    e->ecology_id = rr.n_ecology;
    e->ecology_type = eco_type;
    e->environment_area = area;
    e->improvements_made = improvements;
    e->protection_areas = protection;
    e->green_development = green_dev;
    e->year = year;
    e->active = 1;
    rr.total_improvements += improvements;
    rr.n_ecology++;
    print_str("[RR] Ecology "); print_int(rr.n_ecology - 1);
    print_str(" type="); print_int(eco_type);
    print_str(" area="); print_int(area);
    print_str(" imp="); print_int(improvements);
    print_str(" prt="); print_int(protection);
    print_str(" grn="); print_int(green_dev); print_str("\n");
    return rr.n_ecology - 1;
}

int rr_organization(int org_type, int region, int branches, int collective, int governance, int year) {
    if (rr.n_organization >= MAX_ORGANIZATION) return -1;
    organization_t* o = &organizations[rr.n_organization];
    o->org_id = rr.n_organization;
    o->org_type = org_type;
    o->region_id = region;
    o->party_branches = branches;
    o->collective_income = collective;
    o->self_governance = governance;
    o->year = year;
    o->active = 1;
    rr.total_branches += branches;
    rr.n_organization++;
    print_str("[RR] Organization "); print_int(rr.n_organization - 1);
    print_str(" type="); print_int(org_type);
    print_str(" rgn="); print_int(region);
    print_str(" brn="); print_int(branches);
    print_str(" col="); print_int(collective);
    print_str(" gov="); print_int(governance); print_str("\n");
    return rr.n_organization - 1;
}

void rr_industry_report(void) {
    print_str("[RR] Industry report:\n");
    print_str("  Industry categories: "); print_int(rr.n_industry_rr); print_str("\n");
    print_str("  Total farms established: "); print_int(rr.total_farms); print_str("\n");
}

void rr_talent_report(void) {
    print_str("[RR] Talent report:\n");
    print_str("  Talent categories: "); print_int(rr.n_talent); print_str("\n");
    print_str("  Total talents trained: "); print_int(rr.total_talents); print_str("\n");
}

void rr_ecology_report(void) {
    print_str("[RR] Ecology report:\n");
    print_str("  Culture categories: "); print_int(rr.n_culture); print_str("\n");
    print_str("  Total cultural events: "); print_int(rr.total_events); print_str("\n");
    print_str("  Ecology categories: "); print_int(rr.n_ecology); print_str("\n");
    print_str("  Total improvements: "); print_int(rr.total_improvements); print_str("\n");
    print_str("  Organization categories: "); print_int(rr.n_organization); print_str("\n");
    print_str("  Total party branches: "); print_int(rr.total_branches); print_str("\n");
}

void rr_print_state(void) {
    print_str("[RR] In="); print_int(rr.n_industry_rr);
    print_str(" Tl="); print_int(rr.n_talent);
    print_str(" Cu="); print_int(rr.n_culture);
    print_str(" Ec="); print_int(rr.n_ecology);
    print_str(" Og="); print_int(rr.n_organization);
    print_str("\n");
}

int main(void) {
    print_str("=== Rural Revitalization Demo ===\n\n");
    rr_init();

    print_str("Industry revitalization...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int scl = (i % 4) + 1;
        int frm = 50 + (i * 15);
        int trs = 10 + (i * 3);
        int ecm = 200 + (i * 50);
        int year = 2020 + (i % 5);
        rr_industry(type, scl, frm, trs, ecm, year);
    }

    print_str("\nTalent revitalization...\n");
    for (int i = 0; i < 14; i++) {
        int cat = (i % 5) + 1;
        int type = (i % 4) + 1;
        int trn = 100 + (i * 25);
        int intro = 20 + (i * 5);
        int inc = 10 + (i * 3);
        int year = 2021 + (i % 4);
        rr_talent(cat, type, trn, intro, inc, year);
    }

    print_str("\nCulture revitalization...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int hrt = (i % 5) + 1;
        int evt = 30 + (i * 8);
        int trd = 15 + (i * 4);
        int cvl = 70 + (i % 30);
        int year = 2022 + (i % 3);
        rr_culture(type, hrt, evt, trd, cvl, year);
    }

    print_str("\nEcology revitalization...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int area = 500 + (i * 150);
        int imp = 40 + (i * 10);
        int prt = 20 + (i * 5);
        int grn = 30 + (i * 8);
        int year = 2023 + (i % 2);
        rr_ecology(type, area, imp, prt, grn, year);
    }

    print_str("\nOrganization revitalization...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int rgn = (i % 8) + 1;
        int brn = 15 + (i * 4);
        int col = 100 + (i * 30);
        int gov = 60 + (i * 4);
        int year = 2024;
        rr_organization(type, rgn, brn, col, gov, year);
    }

    print_str("\nIndustry report...\n");
    rr_industry_report();

    print_str("\nTalent report...\n");
    rr_talent_report();

    print_str("\nEcology report...\n");
    rr_ecology_report();

    print_str("\nFinal state...\n");
    rr_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
