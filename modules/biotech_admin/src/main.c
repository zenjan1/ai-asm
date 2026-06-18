/* biotech_admin: Biotechnology administration system (v1.0)
 * Medical biotech, agricultural biotech, industrial biotech, environmental biotech, marine biotech
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

#define MAX_MEDICAL      16
#define MAX_AGRICULTURE  14
#define MAX_INDUSTRIAL   12
#define MAX_ENVIRONMENT  10
#define MAX_MARINE       10

typedef struct {
    int    medical_id;
    int    medical_type;
    int    medical_category;
    int    biopharma;
    int    vaccine;
    int    gene_therapy;
    int    year;
    int    active;
} medical_t;

typedef struct {
    int    agri_id;
    int    agri_type;
    int    agri_category;
    int    transgenic;
    int    biofertilizer;
    int    biopesticide;
    int    year;
    int    active;
} agri_t;

typedef struct {
    int    ind_id;
    int    ind_type;
    int    ind_category;
    int    biomanufacture;
    int    biocatalysis;
    int    biomaterial;
    int    year;
    int    active;
} industrial_t;

typedef struct {
    int    env_id;
    int    env_type;
    int    env_category;
    int    bioremediation;
    int    biotreatment;
    int    biomonitor;
    int    year;
    int    active;
} environment_t;

typedef struct {
    int    marine_id;
    int    marine_type;
    int    marine_category;
    int    marine_pharma;
    int    marine_material;
    int    marine_energy;
    int    year;
    int    active;
} marine_t;

typedef struct {
    int    n_medical;
    int    n_agri;
    int    n_industrial;
    int    n_environment;
    int    n_marine;
    int    total_biopharma;
    int    total_transgenic;
    int    total_biomanufacture;
    int    total_bioremediation;
    int    total_marine_pharma;
} bta_state_t;

static medical_t medicals[MAX_MEDICAL];
static agri_t agri_list[MAX_AGRICULTURE];
static industrial_t industrials[MAX_INDUSTRIAL];
static environment_t environments[MAX_ENVIRONMENT];
static marine_t marines[MAX_MARINE];
static bta_state_t bta;

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

int bta_init(void) {
    if (initialized) return -1;
    bta.n_medical = 0; bta.n_agri = 0; bta.n_industrial = 0;
    bta.n_environment = 0; bta.n_marine = 0;
    bta.total_biopharma = 0; bta.total_transgenic = 0;
    bta.total_biomanufacture = 0; bta.total_bioremediation = 0;
    bta.total_marine_pharma = 0;
    for (int i = 0; i < MAX_MEDICAL; i++) medicals[i].active = 0;
    for (int i = 0; i < MAX_AGRICULTURE; i++) agri_list[i].active = 0;
    for (int i = 0; i < MAX_INDUSTRIAL; i++) industrials[i].active = 0;
    for (int i = 0; i < MAX_ENVIRONMENT; i++) environments[i].active = 0;
    for (int i = 0; i < MAX_MARINE; i++) marines[i].active = 0;
    initialized = 1;
    print_str("[BTA] Biotech initialized\n");
    return 0;
}

int bta_medical(int med_type, int cat, int bio, int vac, int gene, int year) {
    if (bta.n_medical >= MAX_MEDICAL) return -1;
    medical_t* m = &medicals[bta.n_medical];
    m->medical_id = bta.n_medical;
    m->medical_type = med_type;
    m->medical_category = cat;
    m->biopharma = bio;
    m->vaccine = vac;
    m->gene_therapy = gene;
    m->year = year;
    m->active = 1;
    bta.total_biopharma += bio;
    bta.n_medical++;
    print_str("[BTA] Medical "); print_int(bta.n_medical - 1);
    print_str(" type="); print_int(med_type);
    print_str(" cat="); print_int(cat);
    print_str(" bio="); print_int(bio);
    print_str(" vac="); print_int(vac);
    print_str(" gen="); print_int(gene); print_str("\n");
    return bta.n_medical - 1;
}

int bta_agri(int agr_type, int cat, int trans, int fert, int pest, int year) {
    if (bta.n_agri >= MAX_AGRICULTURE) return -1;
    agri_t* a = &agri_list[bta.n_agri];
    a->agri_id = bta.n_agri;
    a->agri_type = agr_type;
    a->agri_category = cat;
    a->transgenic = trans;
    a->biofertilizer = fert;
    a->biopesticide = pest;
    a->year = year;
    a->active = 1;
    bta.total_transgenic += trans;
    bta.n_agri++;
    print_str("[BTA] Agri "); print_int(bta.n_agri - 1);
    print_str(" type="); print_int(agr_type);
    print_str(" cat="); print_int(cat);
    print_str(" trs="); print_int(trans);
    print_str(" frt="); print_int(fert);
    print_str(" pst="); print_int(pest); print_str("\n");
    return bta.n_agri - 1;
}

int bta_industrial(int ind_type, int cat, int manufact, int catal, int mater, int year) {
    if (bta.n_industrial >= MAX_INDUSTRIAL) return -1;
    industrial_t* ind = &industrials[bta.n_industrial];
    ind->ind_id = bta.n_industrial;
    ind->ind_type = ind_type;
    ind->ind_category = cat;
    ind->biomanufacture = manufact;
    ind->biocatalysis = catal;
    ind->biomaterial = mater;
    ind->year = year;
    ind->active = 1;
    bta.total_biomanufacture += manufact;
    bta.n_industrial++;
    print_str("[BTA] Industrial "); print_int(bta.n_industrial - 1);
    print_str(" type="); print_int(ind_type);
    print_str(" cat="); print_int(cat);
    print_str(" mfr="); print_int(manufact);
    print_str(" ctl="); print_int(catal);
    print_str(" mtr="); print_int(mater); print_str("\n");
    return bta.n_industrial - 1;
}

int bta_environment(int env_type, int cat, int remed, int treat, int mon, int year) {
    if (bta.n_environment >= MAX_ENVIRONMENT) return -1;
    environment_t* e = &environments[bta.n_environment];
    e->env_id = bta.n_environment;
    e->env_type = env_type;
    e->env_category = cat;
    e->bioremediation = remed;
    e->biotreatment = treat;
    e->biomonitor = mon;
    e->year = year;
    e->active = 1;
    bta.total_bioremediation += remed;
    bta.n_environment++;
    print_str("[BTA] Environment "); print_int(bta.n_environment - 1);
    print_str(" type="); print_int(env_type);
    print_str(" cat="); print_int(cat);
    print_str(" rmd="); print_int(remed);
    print_str(" trt="); print_int(treat);
    print_str(" mon="); print_int(mon); print_str("\n");
    return bta.n_environment - 1;
}

int bta_marine(int mar_type, int cat, int mphp, int mmtr, int menrg, int year) {
    if (bta.n_marine >= MAX_MARINE) return -1;
    marine_t* m = &marines[bta.n_marine];
    m->marine_id = bta.n_marine;
    m->marine_type = mar_type;
    m->marine_category = cat;
    m->marine_pharma = mphp;
    m->marine_material = mmtr;
    m->marine_energy = menrg;
    m->year = year;
    m->active = 1;
    bta.total_marine_pharma += mphp;
    bta.n_marine++;
    print_str("[BTA] Marine "); print_int(bta.n_marine - 1);
    print_str(" type="); print_int(mar_type);
    print_str(" cat="); print_int(cat);
    print_str(" mph="); print_int(mphp);
    print_str(" mmt="); print_int(mmtr);
    print_str(" men="); print_int(menrg); print_str("\n");
    return bta.n_marine - 1;
}

void bta_medical_report(void) {
    print_str("[BTA] Medical report:\n");
    print_str("  Medical biotech categories: "); print_int(bta.n_medical); print_str("\n");
    print_str("  Total biopharma: "); print_int(bta.total_biopharma); print_str("\n");
}

void bta_agri_report(void) {
    print_str("[BTA] Agri report:\n");
    print_str("  Agricultural biotech categories: "); print_int(bta.n_agri); print_str("\n");
    print_str("  Total transgenic: "); print_int(bta.total_transgenic); print_str("\n");
}

void bta_marine_report(void) {
    print_str("[BTA] Marine report:\n");
    print_str("  Industrial biotech categories: "); print_int(bta.n_industrial); print_str("\n");
    print_str("  Total biomanufacture: "); print_int(bta.total_biomanufacture); print_str("\n");
    print_str("  Environmental biotech categories: "); print_int(bta.n_environment); print_str("\n");
    print_str("  Total bioremediation: "); print_int(bta.total_bioremediation); print_str("\n");
    print_str("  Marine biotech categories: "); print_int(bta.n_marine); print_str("\n");
    print_str("  Total marine pharma: "); print_int(bta.total_marine_pharma); print_str("\n");
}

void bta_print_state(void) {
    print_str("[BTA] Md="); print_int(bta.n_medical);
    print_str(" Ag="); print_int(bta.n_agri);
    print_str(" In="); print_int(bta.n_industrial);
    print_str(" Ev="); print_int(bta.n_environment);
    print_str(" Mr="); print_int(bta.n_marine);
    print_str("\n");
}

int main(void) {
    print_str("=== Biotech Admin Demo ===\n\n");
    bta_init();

    print_str("Medical biotech...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int bio = 60 + (i * 15);
        int vac = 40 + (i * 10);
        int gen = 25 + (i * 6);
        int year = 2020 + (i % 5);
        bta_medical(type, cat, bio, vac, gen, year);
    }

    print_str("\nAgricultural biotech...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int trs = 50 + (i * 12);
        int frt = 35 + (i * 8);
        int pst = 20 + (i * 5);
        int year = 2021 + (i % 4);
        bta_agri(type, cat, trs, frt, pst, year);
    }

    print_str("\nIndustrial biotech...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int mfr = 40 + (i * 10);
        int ctl = 28 + (i * 7);
        int mtr = 18 + (i * 4);
        int year = 2022 + (i % 3);
        bta_industrial(type, cat, mfr, ctl, mtr, year);
    }

    print_str("\nEnvironmental biotech...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int rmd = 30 + (i * 7);
        int trt = 22 + (i * 5);
        int mon = 15 + (i * 3);
        int year = 2023 + (i % 2);
        bta_environment(type, cat, rmd, trt, mon, year);
    }

    print_str("\nMarine biotech...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int mph = 25 + (i * 6);
        int mmt = 18 + (i * 4);
        int men = 12 + (i * 3);
        int year = 2024;
        bta_marine(type, cat, mph, mmt, men, year);
    }

    print_str("\nMedical report...\n");
    bta_medical_report();

    print_str("\nAgri report...\n");
    bta_agri_report();

    print_str("\nMarine report...\n");
    bta_marine_report();

    print_str("\nFinal state...\n");
    bta_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
