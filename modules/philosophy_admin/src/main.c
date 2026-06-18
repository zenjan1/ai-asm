/* philosophy_admin: Philosophy administration system (v1.0)
 * Marxist philosophy, Chinese philosophy, Western philosophy, logic, ethics
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

#define MAX_MARX_PH     16
#define MAX_CHIN_PH     14
#define MAX_WEST_PH     12
#define MAX_LOGIC_S     10
#define MAX_ETHICS_S    10

typedef struct {
    int    mp_id;
    int    mp_type;
    int    mp_category;
    int    dialec;
    int    histo_m;
    int    practic;
    int    contrad;
    int    year;
    int    active;
} marx_ph_t;

typedef struct {
    int    cp_id;
    int    cp_type;
    int    cp_category;
    int    pre_qin;
    int    confuc;
    int    taoism;
    int    buddh;
    int    neo_con;
    int    year;
    int    active;
} chin_ph_t;

typedef struct {
    int    wp_id;
    int    wp_type;
    int    wp_category;
    int    ancient_g;
    int    modern_w;
    int    german_c;
    int    contemp;
    int    year;
    int    active;
} west_ph_t;

typedef struct {
    int    li_id;
    int    li_type;
    int    li_category;
    int    form_log;
    int    math_log;
    int    dia_log;
    int    argu_th;
    int    year;
    int    active;
} logic_s_t;

typedef struct {
    int    et_id;
    int    et_type;
    int    et_category;
    int    meta_eth;
    int    norm_eth;
    int    appl_eth;
    int    virtue;
    int    year;
    int    active;
} ethics_s_t;

typedef struct {
    int    n_marx_ph;
    int    n_chin_ph;
    int    n_west_ph;
    int    n_logic_s;
    int    n_ethics_s;
    int    total_dialec;
    int    total_pre_qin;
    int    total_ancient_g;
    int    total_form_log;
    int    total_meta_eth;
} pa_state_t;

static marx_ph_t marx_phs[MAX_MARX_PH];
static chin_ph_t chin_phs[MAX_CHIN_PH];
static west_ph_t west_phs[MAX_WEST_PH];
static logic_s_t logic_ss[MAX_LOGIC_S];
static ethics_s_t ethics_ss[MAX_ETHICS_S];
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
    pa.n_marx_ph = 0; pa.n_chin_ph = 0; pa.n_west_ph = 0;
    pa.n_logic_s = 0; pa.n_ethics_s = 0;
    pa.total_dialec = 0; pa.total_pre_qin = 0;
    pa.total_ancient_g = 0; pa.total_form_log = 0;
    pa.total_meta_eth = 0;
    for (int i = 0; i < MAX_MARX_PH; i++) marx_phs[i].active = 0;
    for (int i = 0; i < MAX_CHIN_PH; i++) chin_phs[i].active = 0;
    for (int i = 0; i < MAX_WEST_PH; i++) west_phs[i].active = 0;
    for (int i = 0; i < MAX_LOGIC_S; i++) logic_ss[i].active = 0;
    for (int i = 0; i < MAX_ETHICS_S; i++) ethics_ss[i].active = 0;
    initialized = 1;
    print_str("[PA] Philosophy initialized\n");
    return 0;
}

int pa_marx_ph(int mt_type, int cat, int dia, int hst, int prac, int cont, int year) {
    if (pa.n_marx_ph >= MAX_MARX_PH) return -1;
    marx_ph_t* m = &marx_phs[pa.n_marx_ph];
    m->mp_id = pa.n_marx_ph;
    m->mp_type = mt_type;
    m->mp_category = cat;
    m->dialec = dia;
    m->histo_m = hst;
    m->practic = prac;
    m->contrad = cont;
    m->year = year;
    m->active = 1;
    pa.total_dialec += dia;
    pa.n_marx_ph++;
    print_str("[PA] Marx ph "); print_int(pa.n_marx_ph - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" dia="); print_int(dia);
    print_str(" hst="); print_int(hst);
    print_str(" prac="); print_int(prac);
    print_str(" cont="); print_int(cont); print_str("\n");
    return pa.n_marx_ph - 1;
}

int pa_chin_ph(int ct_type, int cat, int pqq, int cfc, int tsm, int bdh, int ncn, int year) {
    if (pa.n_chin_ph >= MAX_CHIN_PH) return -1;
    chin_ph_t* c = &chin_phs[pa.n_chin_ph];
    c->cp_id = pa.n_chin_ph;
    c->cp_type = ct_type;
    c->cp_category = cat;
    c->pre_qin = pqq;
    c->confuc = cfc;
    c->taoism = tsm;
    c->buddh = bdh;
    c->neo_con = ncn;
    c->year = year;
    c->active = 1;
    pa.total_pre_qin += pqq;
    pa.n_chin_ph++;
    print_str("[PA] Chin ph "); print_int(pa.n_chin_ph - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" pqq="); print_int(pqq);
    print_str(" cfc="); print_int(cfc);
    print_str(" tsm="); print_int(tsm);
    print_str(" bdh="); print_int(bdh);
    print_str(" ncn="); print_int(ncn); print_str("\n");
    return pa.n_chin_ph - 1;
}

int pa_west_ph(int wt_type, int cat, int acg, int mdw, int gmc, int ctw, int year) {
    if (pa.n_west_ph >= MAX_WEST_PH) return -1;
    west_ph_t* w = &west_phs[pa.n_west_ph];
    w->wp_id = pa.n_west_ph;
    w->wp_type = wt_type;
    w->wp_category = cat;
    w->ancient_g = acg;
    w->modern_w = mdw;
    w->german_c = gmc;
    w->contemp = ctw;
    w->year = year;
    w->active = 1;
    pa.total_ancient_g += acg;
    pa.n_west_ph++;
    print_str("[PA] West ph "); print_int(pa.n_west_ph - 1);
    print_str(" type="); print_int(wt_type);
    print_str(" cat="); print_int(cat);
    print_str(" acg="); print_int(acg);
    print_str(" mdw="); print_int(mdw);
    print_str(" gmc="); print_int(gmc);
    print_str(" ctw="); print_int(ctw); print_str("\n");
    return pa.n_west_ph - 1;
}

int pa_logic_s(int lt_type, int cat, int fml, int mtl, int dlg, int arg, int year) {
    if (pa.n_logic_s >= MAX_LOGIC_S) return -1;
    logic_s_t* l = &logic_ss[pa.n_logic_s];
    l->li_id = pa.n_logic_s;
    l->li_type = lt_type;
    l->li_category = cat;
    l->form_log = fml;
    l->math_log = mtl;
    l->dia_log = dlg;
    l->argu_th = arg;
    l->year = year;
    l->active = 1;
    pa.total_form_log += fml;
    pa.n_logic_s++;
    print_str("[PA] Logic "); print_int(pa.n_logic_s - 1);
    print_str(" type="); print_int(lt_type);
    print_str(" cat="); print_int(cat);
    print_str(" fml="); print_int(fml);
    print_str(" mtl="); print_int(mtl);
    print_str(" dlg="); print_int(dlg);
    print_str(" arg="); print_int(arg); print_str("\n");
    return pa.n_logic_s - 1;
}

int pa_ethics_s(int et_type, int cat, int met, int nrm, int apl, int vrt, int year) {
    if (pa.n_ethics_s >= MAX_ETHICS_S) return -1;
    ethics_s_t* e = &ethics_ss[pa.n_ethics_s];
    e->et_id = pa.n_ethics_s;
    e->et_type = et_type;
    e->et_category = cat;
    e->meta_eth = met;
    e->norm_eth = nrm;
    e->appl_eth = apl;
    e->virtue = vrt;
    e->year = year;
    e->active = 1;
    pa.total_meta_eth += met;
    pa.n_ethics_s++;
    print_str("[PA] Ethics "); print_int(pa.n_ethics_s - 1);
    print_str(" type="); print_int(et_type);
    print_str(" cat="); print_int(cat);
    print_str(" met="); print_int(met);
    print_str(" nrm="); print_int(nrm);
    print_str(" apl="); print_int(apl);
    print_str(" vrt="); print_int(vrt); print_str("\n");
    return pa.n_ethics_s - 1;
}

void pa_marx_report(void) {
    print_str("[PA] Marxist philosophy report:\n");
    print_str("  Marxist categories: "); print_int(pa.n_marx_ph); print_str("\n");
    print_str("  Total dialectics: "); print_int(pa.total_dialec); print_str("\n");
}

void pa_chin_report(void) {
    print_str("[PA] Chinese philosophy report:\n");
    print_str("  Chinese philosophy categories: "); print_int(pa.n_chin_ph); print_str("\n");
    print_str("  Total pre-Qin studies: "); print_int(pa.total_pre_qin); print_str("\n");
}

void pa_full_report(void) {
    print_str("[PA] Full report:\n");
    print_str("  Western philosophy categories: "); print_int(pa.n_west_ph); print_str("\n");
    print_str("  Total ancient Greek studies: "); print_int(pa.total_ancient_g); print_str("\n");
    print_str("  Logic categories: "); print_int(pa.n_logic_s); print_str("\n");
    print_str("  Total formal logic: "); print_int(pa.total_form_log); print_str("\n");
    print_str("  Ethics categories: "); print_int(pa.n_ethics_s); print_str("\n");
    print_str("  Total meta-ethics: "); print_int(pa.total_meta_eth); print_str("\n");
}

void pa_print_state(void) {
    print_str("[PA] Mp="); print_int(pa.n_marx_ph);
    print_str(" Cp="); print_int(pa.n_chin_ph);
    print_str(" Wp="); print_int(pa.n_west_ph);
    print_str(" Ls="); print_int(pa.n_logic_s);
    print_str(" Es="); print_int(pa.n_ethics_s);
    print_str("\n");
}

int main(void) {
    print_str("=== Philosophy Admin Demo ===\n\n");
    pa_init();

    print_str("Marxist philosophy...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int dia = 55 + (i * 13);
        int hst = 40 + (i * 10);
        int prac = 22 + (i * 5);
        int cont = 15 + (i * 3);
        int year = 2020 + (i % 5);
        pa_marx_ph(type, cat, dia, hst, prac, cont, year);
    }

    print_str("\nChinese philosophy...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pqq = 48 + (i * 11);
        int cfc = 35 + (i * 8);
        int tsm = 20 + (i * 4);
        int bdh = 12 + (i * 3);
        int ncn = 8 + (i * 2);
        int year = 2021 + (i % 4);
        pa_chin_ph(type, cat, pqq, cfc, tsm, bdh, ncn, year);
    }

    print_str("\nWestern philosophy...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int acg = 42 + (i * 10);
        int mdw = 28 + (i * 7);
        int gmc = 18 + (i * 4);
        int ctw = 10 + (i * 2);
        int year = 2022 + (i % 3);
        pa_west_ph(type, cat, acg, mdw, gmc, ctw, year);
    }

    print_str("\nLogic...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int fml = 35 + (i * 8);
        int mtl = 25 + (i * 6);
        int dlg = 15 + (i * 3);
        int arg = 10 + (i * 2);
        int year = 2023 + (i % 2);
        pa_logic_s(type, cat, fml, mtl, dlg, arg, year);
    }

    print_str("\nEthics...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int met = 30 + (i * 7);
        int nrm = 22 + (i * 5);
        int apl = 12 + (i * 3);
        int vrt = 8 + (i * 2);
        int year = 2024;
        pa_ethics_s(type, cat, met, nrm, apl, vrt, year);
    }

    print_str("\nMarxist report...\n");
    pa_marx_report();

    print_str("\nChinese report...\n");
    pa_chin_report();

    print_str("\nFull report...\n");
    pa_full_report();

    print_str("\nFinal state...\n");
    pa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
