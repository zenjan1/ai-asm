/* electronics_admin: Electronics administration system (v1.0)
 * Circuit theory, analog electronics, digital electronics, microelectronics, optoelectronics
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

#define MAX_CIR_TH     16
#define MAX_ANA_E      14
#define MAX_DIG_E      12
#define MAX_MICRO_E    10
#define MAX_OPTO_E     10

typedef struct {
    int    ct_id;
    int    ct_type;
    int    ct_category;
    int    cir_ana;
    int    cir_syn;
    int    non_cir;
    int    dis_par;
    int    year;
    int    active;
} cir_th_t;

typedef struct {
    int    ae_id;
    int    ae_type;
    int    ae_category;
    int    amp_cir;
    int    op_amp;
    int    fbk_cir;
    int    pwr_cir;
    int    year;
    int    active;
} ana_e_t;

typedef struct {
    int    de_id;
    int    de_type;
    int    de_category;
    int    log_gate;
    int    com_log;
    int    seq_log;
    int    dig_sys;
    int    year;
    int    active;
} dig_e_t;

typedef struct {
    int    me_id;
    int    me_type;
    int    me_category;
    int    sem_dev;
    int    ic_des;
    int    mic_fab;
    int    pkg_tst;
    int    year;
    int    active;
} micro_e_t;

typedef struct {
    int    oe_id;
    int    oe_type;
    int    oe_category;
    int    opt_dev;
    int    fib_com;
    int    las_tec;
    int    opt_det;
    int    year;
    int    active;
} opto_e_t;

typedef struct {
    int    n_cir_th;
    int    n_ana_e;
    int    n_dig_e;
    int    n_micro_e;
    int    n_opto_e;
    int    total_cir_ana;
    int    total_amp_cir;
    int    total_log_gate;
    int    total_sem_dev;
    int    total_opt_dev;
} ela_state_t;

static cir_th_t cir_ths[MAX_CIR_TH];
static ana_e_t ana_es[MAX_ANA_E];
static dig_e_t dig_es[MAX_DIG_E];
static micro_e_t micro_es[MAX_MICRO_E];
static opto_e_t opto_es[MAX_OPTO_E];
static ela_state_t ela;

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

int ela_init(void) {
    if (initialized) return -1;
    ela.n_cir_th = 0; ela.n_ana_e = 0; ela.n_dig_e = 0;
    ela.n_micro_e = 0; ela.n_opto_e = 0;
    ela.total_cir_ana = 0; ela.total_amp_cir = 0;
    ela.total_log_gate = 0; ela.total_sem_dev = 0;
    ela.total_opt_dev = 0;
    for (int i = 0; i < MAX_CIR_TH; i++) cir_ths[i].active = 0;
    for (int i = 0; i < MAX_ANA_E; i++) ana_es[i].active = 0;
    for (int i = 0; i < MAX_DIG_E; i++) dig_es[i].active = 0;
    for (int i = 0; i < MAX_MICRO_E; i++) micro_es[i].active = 0;
    for (int i = 0; i < MAX_OPTO_E; i++) opto_es[i].active = 0;
    initialized = 1;
    print_str("[ELA] Electronics initialized\n");
    return 0;
}

int ela_cir_th(int ct_type, int cat, int can, int csy, int ncr, int dpr, int year) {
    if (ela.n_cir_th >= MAX_CIR_TH) return -1;
    cir_th_t* c = &cir_ths[ela.n_cir_th];
    c->ct_id = ela.n_cir_th;
    c->ct_type = ct_type;
    c->ct_category = cat;
    c->cir_ana = can;
    c->cir_syn = csy;
    c->non_cir = ncr;
    c->dis_par = dpr;
    c->year = year;
    c->active = 1;
    ela.total_cir_ana += can;
    ela.n_cir_th++;
    print_str("[ELA] Cir th "); print_int(ela.n_cir_th - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" can="); print_int(can);
    print_str(" csy="); print_int(csy);
    print_str(" ncr="); print_int(ncr);
    print_str(" dpr="); print_int(dpr); print_str("\n");
    return ela.n_cir_th - 1;
}

int ela_ana_e(int at_type, int cat, int acr, int oam, int fcr, int pcr, int year) {
    if (ela.n_ana_e >= MAX_ANA_E) return -1;
    ana_e_t* a = &ana_es[ela.n_ana_e];
    a->ae_id = ela.n_ana_e;
    a->ae_type = at_type;
    a->ae_category = cat;
    a->amp_cir = acr;
    a->op_amp = oam;
    a->fbk_cir = fcr;
    a->pwr_cir = pcr;
    a->year = year;
    a->active = 1;
    ela.total_amp_cir += acr;
    ela.n_ana_e++;
    print_str("[ELA] Ana e "); print_int(ela.n_ana_e - 1);
    print_str(" type="); print_int(at_type);
    print_str(" cat="); print_int(cat);
    print_str(" acr="); print_int(acr);
    print_str(" oam="); print_int(oam);
    print_str(" fcr="); print_int(fcr);
    print_str(" pcr="); print_int(pcr); print_str("\n");
    return ela.n_ana_e - 1;
}

int ela_dig_e(int dt_type, int cat, int lgt, int cgl, int sql, int dsy, int year) {
    if (ela.n_dig_e >= MAX_DIG_E) return -1;
    dig_e_t* d = &dig_es[ela.n_dig_e];
    d->de_id = ela.n_dig_e;
    d->de_type = dt_type;
    d->de_category = cat;
    d->log_gate = lgt;
    d->com_log = cgl;
    d->seq_log = sql;
    d->dig_sys = dsy;
    d->year = year;
    d->active = 1;
    ela.total_log_gate += lgt;
    ela.n_dig_e++;
    print_str("[ELA] Dig e "); print_int(ela.n_dig_e - 1);
    print_str(" type="); print_int(dt_type);
    print_str(" cat="); print_int(cat);
    print_str(" lgt="); print_int(lgt);
    print_str(" cgl="); print_int(cgl);
    print_str(" sql="); print_int(sql);
    print_str(" dsy="); print_int(dsy); print_str("\n");
    return ela.n_dig_e - 1;
}

int ela_micro_e(int mt_type, int cat, int sdy, int icd, int mfb, int ptst, int year) {
    if (ela.n_micro_e >= MAX_MICRO_E) return -1;
    micro_e_t* m = &micro_es[ela.n_micro_e];
    m->me_id = ela.n_micro_e;
    m->me_type = mt_type;
    m->me_category = cat;
    m->sem_dev = sdy;
    m->ic_des = icd;
    m->mic_fab = mfb;
    m->pkg_tst = ptst;
    m->year = year;
    m->active = 1;
    ela.total_sem_dev += sdy;
    ela.n_micro_e++;
    print_str("[ELA] Micro e "); print_int(ela.n_micro_e - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" sdy="); print_int(sdy);
    print_str(" icd="); print_int(icd);
    print_str(" mfb="); print_int(mfb);
    print_str(" ptst="); print_int(ptst); print_str("\n");
    return ela.n_micro_e - 1;
}

int ela_opto_e(int ot_type, int cat, int ody, int fbc, int lsr, int opd, int year) {
    if (ela.n_opto_e >= MAX_OPTO_E) return -1;
    opto_e_t* o = &opto_es[ela.n_opto_e];
    o->oe_id = ela.n_opto_e;
    o->oe_type = ot_type;
    o->oe_category = cat;
    o->opt_dev = ody;
    o->fib_com = fbc;
    o->las_tec = lsr;
    o->opt_det = opd;
    o->year = year;
    o->active = 1;
    ela.total_opt_dev += ody;
    ela.n_opto_e++;
    print_str("[ELA] Opto e "); print_int(ela.n_opto_e - 1);
    print_str(" type="); print_int(ot_type);
    print_str(" cat="); print_int(cat);
    print_str(" ody="); print_int(ody);
    print_str(" fbc="); print_int(fbc);
    print_str(" lsr="); print_int(lsr);
    print_str(" opd="); print_int(opd); print_str("\n");
    return ela.n_opto_e - 1;
}

void ela_cir_report(void) {
    print_str("[ELA] Circuit theory report:\n");
    print_str("  Circuit categories: "); print_int(ela.n_cir_th); print_str("\n");
    print_str("  Total circuit analysis: "); print_int(ela.total_cir_ana); print_str("\n");
}

void ela_ana_report(void) {
    print_str("[ELA] Analog electronics report:\n");
    print_str("  Analog categories: "); print_int(ela.n_ana_e); print_str("\n");
    print_str("  Total amplifier circuits: "); print_int(ela.total_amp_cir); print_str("\n");
}

void ela_full_report(void) {
    print_str("[ELA] Full report:\n");
    print_str("  Digital categories: "); print_int(ela.n_dig_e); print_str("\n");
    print_str("  Total logic gates: "); print_int(ela.total_log_gate); print_str("\n");
    print_str("  Microelectronics categories: "); print_int(ela.n_micro_e); print_str("\n");
    print_str("  Total semiconductor devices: "); print_int(ela.total_sem_dev); print_str("\n");
    print_str("  Optoelectronics categories: "); print_int(ela.n_opto_e); print_str("\n");
    print_str("  Total opto devices: "); print_int(ela.total_opt_dev); print_str("\n");
}

void ela_print_state(void) {
    print_str("[ELA] Ct="); print_int(ela.n_cir_th);
    print_str(" Ae="); print_int(ela.n_ana_e);
    print_str(" De="); print_int(ela.n_dig_e);
    print_str(" Me="); print_int(ela.n_micro_e);
    print_str(" Oe="); print_int(ela.n_opto_e);
    print_str("\n");
}

int main(void) {
    print_str("=== Electronics Admin Demo ===\n\n");
    ela_init();

    print_str("Circuit theory...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int can = 55 + (i * 13);
        int csy = 40 + (i * 10);
        int ncr = 22 + (i * 5);
        int dpr = 15 + (i * 3);
        int year = 2020 + (i % 5);
        ela_cir_th(type, cat, can, csy, ncr, dpr, year);
    }

    print_str("\nAnalog electronics...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int acr = 48 + (i * 11);
        int oam = 35 + (i * 8);
        int fcr = 20 + (i * 4);
        int pcr = 12 + (i * 3);
        int year = 2021 + (i % 4);
        ela_ana_e(type, cat, acr, oam, fcr, pcr, year);
    }

    print_str("\nDigital electronics...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int lgt = 42 + (i * 10);
        int cgl = 28 + (i * 7);
        int sql = 18 + (i * 4);
        int dsy = 10 + (i * 2);
        int year = 2022 + (i % 3);
        ela_dig_e(type, cat, lgt, cgl, sql, dsy, year);
    }

    print_str("\nMicroelectronics...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int sdy = 35 + (i * 8);
        int icd = 25 + (i * 6);
        int mfb = 15 + (i * 3);
        int ptst = 10 + (i * 2);
        int year = 2023 + (i % 2);
        ela_micro_e(type, cat, sdy, icd, mfb, ptst, year);
    }

    print_str("\nOptoelectronics...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ody = 30 + (i * 7);
        int fbc = 22 + (i * 5);
        int lsr = 12 + (i * 3);
        int opd = 8 + (i * 2);
        int year = 2024;
        ela_opto_e(type, cat, ody, fbc, lsr, opd, year);
    }

    print_str("\nCircuit report...\n");
    ela_cir_report();

    print_str("\nAnalog report...\n");
    ela_ana_report();

    print_str("\nFull report...\n");
    ela_full_report();

    print_str("\nFinal state...\n");
    ela_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
