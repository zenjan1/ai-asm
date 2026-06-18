/* clinical_medicine_admin: Clinical medicine administration system (v1.0)
 * Internal medicine, surgery, obstetrics/gynecology, pediatrics, emergency
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

#define MAX_INTERN_MED  16
#define MAX_SURGERY     14
#define MAX_OBS_GYN     12
#define MAX_PEDIATRICS  10
#define MAX_EMERGENCY   10

typedef struct {
    int    im_id;
    int    im_type;
    int    im_category;
    int    respir_med;
    int    cardio_med;
    int    digest_med;
    int    neuro_med;
    int    year;
    int    active;
} intern_med_t;

typedef struct {
    int    sg_id;
    int    sg_type;
    int    sg_category;
    int    gen_surg;
    int    ortho_surg;
    int    neuro_surg;
    int    cardio_surg;
    int    year;
    int    active;
} surgery_t;

typedef struct {
    int    og_id;
    int    og_type;
    int    og_category;
    int    obstet;
    int    gynec;
    int    repro;
    int    peri_med;
    int    year;
    int    active;
} obs_gyn_t;

typedef struct {
    int    pd_id;
    int    pd_type;
    int    pd_category;
    int    neonat;
    int    ped_int;
    int    ped_surg;
    int    child_hlth;
    int    year;
    int    active;
} pediatrics_t;

typedef struct {
    int    em_id;
    int    em_type;
    int    em_category;
    int    emerg_int;
    int    trauma;
    int    critical;
    int    rescue;
    int    year;
    int    active;
} emergency_t;

typedef struct {
    int    n_intern_med;
    int    n_surgery;
    int    n_obs_gyn;
    int    n_pediatrics;
    int    n_emergency;
    int    total_respir_med;
    int    total_gen_surg;
    int    total_obstet;
    int    total_neonat;
    int    total_emerg_int;
} cma_state_t;

static intern_med_t intern_meds[MAX_INTERN_MED];
static surgery_t surgeries[MAX_SURGERY];
static obs_gyn_t obs_gyns[MAX_OBS_GYN];
static pediatrics_t pediatricss[MAX_PEDIATRICS];
static emergency_t emergencys[MAX_EMERGENCY];
static cma_state_t cma;

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

int cma_init(void) {
    if (initialized) return -1;
    cma.n_intern_med = 0; cma.n_surgery = 0; cma.n_obs_gyn = 0;
    cma.n_pediatrics = 0; cma.n_emergency = 0;
    cma.total_respir_med = 0; cma.total_gen_surg = 0;
    cma.total_obstet = 0; cma.total_neonat = 0;
    cma.total_emerg_int = 0;
    for (int i = 0; i < MAX_INTERN_MED; i++) intern_meds[i].active = 0;
    for (int i = 0; i < MAX_SURGERY; i++) surgeries[i].active = 0;
    for (int i = 0; i < MAX_OBS_GYN; i++) obs_gyns[i].active = 0;
    for (int i = 0; i < MAX_PEDIATRICS; i++) pediatricss[i].active = 0;
    for (int i = 0; i < MAX_EMERGENCY; i++) emergencys[i].active = 0;
    initialized = 1;
    print_str("[CMA] Clinical medicine initialized\n");
    return 0;
}

int cma_intern_med(int it_type, int cat, int rsp, int crd, int dgs, int nro, int year) {
    if (cma.n_intern_med >= MAX_INTERN_MED) return -1;
    intern_med_t* c = &intern_meds[cma.n_intern_med];
    c->im_id = cma.n_intern_med;
    c->im_type = it_type;
    c->im_category = cat;
    c->respir_med = rsp;
    c->cardio_med = crd;
    c->digest_med = dgs;
    c->neuro_med = nro;
    c->year = year;
    c->active = 1;
    cma.total_respir_med += rsp;
    cma.n_intern_med++;
    print_str("[CMA] Intern med "); print_int(cma.n_intern_med - 1);
    print_str(" type="); print_int(it_type);
    print_str(" cat="); print_int(cat);
    print_str(" rsp="); print_int(rsp);
    print_str(" crd="); print_int(crd);
    print_str(" dgs="); print_int(dgs);
    print_str(" nro="); print_int(nro); print_str("\n");
    return cma.n_intern_med - 1;
}

int cma_surgery(int st_type, int cat, int gns, int ort, int nrs, int crs, int year) {
    if (cma.n_surgery >= MAX_SURGERY) return -1;
    surgery_t* s = &surgeries[cma.n_surgery];
    s->sg_id = cma.n_surgery;
    s->sg_type = st_type;
    s->sg_category = cat;
    s->gen_surg = gns;
    s->ortho_surg = ort;
    s->neuro_surg = nrs;
    s->cardio_surg = crs;
    s->year = year;
    s->active = 1;
    cma.total_gen_surg += gns;
    cma.n_surgery++;
    print_str("[CMA] Surgery "); print_int(cma.n_surgery - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" gns="); print_int(gns);
    print_str(" ort="); print_int(ort);
    print_str(" nrs="); print_int(nrs);
    print_str(" crs="); print_int(crs); print_str("\n");
    return cma.n_surgery - 1;
}

int cma_obs_gyn(int ot_type, int cat, int obt, int gyc, int rpr, int prm, int year) {
    if (cma.n_obs_gyn >= MAX_OBS_GYN) return -1;
    obs_gyn_t* o = &obs_gyns[cma.n_obs_gyn];
    o->og_id = cma.n_obs_gyn;
    o->og_type = ot_type;
    o->og_category = cat;
    o->obstet = obt;
    o->gynec = gyc;
    o->repro = rpr;
    o->peri_med = prm;
    o->year = year;
    o->active = 1;
    cma.total_obstet += obt;
    cma.n_obs_gyn++;
    print_str("[CMA] Obs gyn "); print_int(cma.n_obs_gyn - 1);
    print_str(" type="); print_int(ot_type);
    print_str(" cat="); print_int(cat);
    print_str(" obt="); print_int(obt);
    print_str(" gyc="); print_int(gyc);
    print_str(" rpr="); print_int(rpr);
    print_str(" prm="); print_int(prm); print_str("\n");
    return cma.n_obs_gyn - 1;
}

int cma_pediatrics(int pt_type, int cat, int nnt, int pin, int psr, int chl, int year) {
    if (cma.n_pediatrics >= MAX_PEDIATRICS) return -1;
    pediatrics_t* p = &pediatricss[cma.n_pediatrics];
    p->pd_id = cma.n_pediatrics;
    p->pd_type = pt_type;
    p->pd_category = cat;
    p->neonat = nnt;
    p->ped_int = pin;
    p->ped_surg = psr;
    p->child_hlth = chl;
    p->year = year;
    p->active = 1;
    cma.total_neonat += nnt;
    cma.n_pediatrics++;
    print_str("[CMA] Pediatrics "); print_int(cma.n_pediatrics - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" nnt="); print_int(nnt);
    print_str(" pin="); print_int(pin);
    print_str(" psr="); print_int(psr);
    print_str(" chl="); print_int(chl); print_str("\n");
    return cma.n_pediatrics - 1;
}

int cma_emergency(int et_type, int cat, int ein, int tra, int crt, int resc, int year) {
    if (cma.n_emergency >= MAX_EMERGENCY) return -1;
    emergency_t* e = &emergencys[cma.n_emergency];
    e->em_id = cma.n_emergency;
    e->em_type = et_type;
    e->em_category = cat;
    e->emerg_int = ein;
    e->trauma = tra;
    e->critical = crt;
    e->rescue = resc;
    e->year = year;
    e->active = 1;
    cma.total_emerg_int += ein;
    cma.n_emergency++;
    print_str("[CMA] Emergency "); print_int(cma.n_emergency - 1);
    print_str(" type="); print_int(et_type);
    print_str(" cat="); print_int(cat);
    print_str(" ein="); print_int(ein);
    print_str(" tra="); print_int(tra);
    print_str(" crt="); print_int(crt);
    print_str(" resc="); print_int(resc); print_str("\n");
    return cma.n_emergency - 1;
}

void cma_intern_report(void) {
    print_str("[CMA] Internal medicine report:\n");
    print_str("  Internal medicine categories: "); print_int(cma.n_intern_med); print_str("\n");
    print_str("  Total respiratory medicine: "); print_int(cma.total_respir_med); print_str("\n");
}

void cma_surgery_report(void) {
    print_str("[CMA] Surgery report:\n");
    print_str("  Surgery categories: "); print_int(cma.n_surgery); print_str("\n");
    print_str("  Total general surgery: "); print_int(cma.total_gen_surg); print_str("\n");
}

void cma_full_report(void) {
    print_str("[CMA] Full report:\n");
    print_str("  Obstetrics/gynecology categories: "); print_int(cma.n_obs_gyn); print_str("\n");
    print_str("  Total obstetrics: "); print_int(cma.total_obstet); print_str("\n");
    print_str("  Pediatrics categories: "); print_int(cma.n_pediatrics); print_str("\n");
    print_str("  Total neonatology: "); print_int(cma.total_neonat); print_str("\n");
    print_str("  Emergency categories: "); print_int(cma.n_emergency); print_str("\n");
    print_str("  Total emergency internal: "); print_int(cma.total_emerg_int); print_str("\n");
}

void cma_print_state(void) {
    print_str("[CMA] Im="); print_int(cma.n_intern_med);
    print_str(" Sg="); print_int(cma.n_surgery);
    print_str(" Og="); print_int(cma.n_obs_gyn);
    print_str(" Pd="); print_int(cma.n_pediatrics);
    print_str(" Em="); print_int(cma.n_emergency);
    print_str("\n");
}

int main(void) {
    print_str("=== Clinical Medicine Admin Demo ===\n\n");
    cma_init();

    print_str("Internal medicine...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int rsp = 55 + (i * 13);
        int crd = 40 + (i * 10);
        int dgs = 22 + (i * 5);
        int nro = 15 + (i * 3);
        int year = 2020 + (i % 5);
        cma_intern_med(type, cat, rsp, crd, dgs, nro, year);
    }

    print_str("\nSurgery...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int gns = 48 + (i * 11);
        int ort = 35 + (i * 8);
        int nrs = 20 + (i * 4);
        int crs = 12 + (i * 3);
        int year = 2021 + (i % 4);
        cma_surgery(type, cat, gns, ort, nrs, crs, year);
    }

    print_str("\nObstetrics/gynecology...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int obt = 42 + (i * 10);
        int gyc = 28 + (i * 7);
        int rpr = 18 + (i * 4);
        int prm = 10 + (i * 2);
        int year = 2022 + (i % 3);
        cma_obs_gyn(type, cat, obt, gyc, rpr, prm, year);
    }

    print_str("\nPediatrics...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int nnt = 35 + (i * 8);
        int pin = 25 + (i * 6);
        int psr = 15 + (i * 3);
        int chl = 10 + (i * 2);
        int year = 2023 + (i % 2);
        cma_pediatrics(type, cat, nnt, pin, psr, chl, year);
    }

    print_str("\nEmergency...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ein = 30 + (i * 7);
        int tra = 22 + (i * 5);
        int crt = 12 + (i * 3);
        int resc = 8 + (i * 2);
        int year = 2024;
        cma_emergency(type, cat, ein, tra, crt, resc, year);
    }

    print_str("\nInternal medicine report...\n");
    cma_intern_report();

    print_str("\nSurgery report...\n");
    cma_surgery_report();

    print_str("\nFull report...\n");
    cma_full_report();

    print_str("\nFinal state...\n");
    cma_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
