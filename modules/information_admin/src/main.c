/* information_admin: Information science administration system (v1.0)
 * Information theory, data science, knowledge management, information systems, information security
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

#define MAX_INFO_TH     16
#define MAX_DATA_SC     14
#define MAX_KNOW_MG     12
#define MAX_INFO_SYS    10
#define MAX_INFO_SEC    10

typedef struct {
    int    it_id;
    int    it_type;
    int    it_category;
    int    info_ent;
    int    cha_cod;
    int    sou_cod;
    int    info_mea;
    int    year;
    int    active;
} info_th_t;

typedef struct {
    int    ds_id;
    int    ds_type;
    int    ds_category;
    int    dat_min;
    int    mac_lea;
    int    sta_ana;
    int    dat_vis;
    int    year;
    int    active;
} data_sc_t;

typedef struct {
    int    km_id;
    int    km_type;
    int    km_category;
    int    kno_acq;
    int    kno_rep;
    int    kno_rea;
    int    kno_app;
    int    year;
    int    active;
} know_mg_t;

typedef struct {
    int    is_id;
    int    is_type;
    int    is_category;
    int    sys_des;
    int    sys_ana;
    int    sys_imp;
    int    sys_eva;
    int    year;
    int    active;
} info_sys_t;

typedef struct {
    int    sc_id;
    int    sc_type;
    int    sc_category;
    int    crypto;
    int    net_sec;
    int    sys_sec;
    int    privacy;
    int    year;
    int    active;
} info_sec_t;

typedef struct {
    int    n_info_th;
    int    n_data_sc;
    int    n_know_mg;
    int    n_info_sys;
    int    n_info_sec;
    int    total_info_ent;
    int    total_dat_min;
    int    total_kno_acq;
    int    total_sys_des;
    int    total_crypto;
} ina_state_t;

static info_th_t info_ths[MAX_INFO_TH];
static data_sc_t data_scs[MAX_DATA_SC];
static know_mg_t know_mgs[MAX_KNOW_MG];
static info_sys_t info_syss[MAX_INFO_SYS];
static info_sec_t info_secs[MAX_INFO_SEC];
static ina_state_t ina;

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

int ina_init(void) {
    if (initialized) return -1;
    ina.n_info_th = 0; ina.n_data_sc = 0; ina.n_know_mg = 0;
    ina.n_info_sys = 0; ina.n_info_sec = 0;
    ina.total_info_ent = 0; ina.total_dat_min = 0;
    ina.total_kno_acq = 0; ina.total_sys_des = 0;
    ina.total_crypto = 0;
    for (int i = 0; i < MAX_INFO_TH; i++) info_ths[i].active = 0;
    for (int i = 0; i < MAX_DATA_SC; i++) data_scs[i].active = 0;
    for (int i = 0; i < MAX_KNOW_MG; i++) know_mgs[i].active = 0;
    for (int i = 0; i < MAX_INFO_SYS; i++) info_syss[i].active = 0;
    for (int i = 0; i < MAX_INFO_SEC; i++) info_secs[i].active = 0;
    initialized = 1;
    print_str("[INA] Information initialized\n");
    return 0;
}

int ina_info_th(int it_type, int cat, int ien, int ccd, int sco, int ime, int year) {
    if (ina.n_info_th >= MAX_INFO_TH) return -1;
    info_th_t* n = &info_ths[ina.n_info_th];
    n->it_id = ina.n_info_th;
    n->it_type = it_type;
    n->it_category = cat;
    n->info_ent = ien;
    n->cha_cod = ccd;
    n->sou_cod = sco;
    n->info_mea = ime;
    n->year = year;
    n->active = 1;
    ina.total_info_ent += ien;
    ina.n_info_th++;
    print_str("[INA] Info th "); print_int(ina.n_info_th - 1);
    print_str(" type="); print_int(it_type);
    print_str(" cat="); print_int(cat);
    print_str(" ien="); print_int(ien);
    print_str(" ccd="); print_int(ccd);
    print_str(" sco="); print_int(sco);
    print_str(" ime="); print_int(ime); print_str("\n");
    return ina.n_info_th - 1;
}

int ina_data_sc(int dt_type, int cat, int dmn, int mcl, int sta, int dvs, int year) {
    if (ina.n_data_sc >= MAX_DATA_SC) return -1;
    data_sc_t* d = &data_scs[ina.n_data_sc];
    d->ds_id = ina.n_data_sc;
    d->ds_type = dt_type;
    d->ds_category = cat;
    d->dat_min = dmn;
    d->mac_lea = mcl;
    d->sta_ana = sta;
    d->dat_vis = dvs;
    d->year = year;
    d->active = 1;
    ina.total_dat_min += dmn;
    ina.n_data_sc++;
    print_str("[INA] Data sc "); print_int(ina.n_data_sc - 1);
    print_str(" type="); print_int(dt_type);
    print_str(" cat="); print_int(cat);
    print_str(" dmn="); print_int(dmn);
    print_str(" mcl="); print_int(mcl);
    print_str(" sta="); print_int(sta);
    print_str(" dvs="); print_int(dvs); print_str("\n");
    return ina.n_data_sc - 1;
}

int ina_know_mg(int kt_type, int cat, int kac, int kre, int krn, int kap, int year) {
    if (ina.n_know_mg >= MAX_KNOW_MG) return -1;
    know_mg_t* k = &know_mgs[ina.n_know_mg];
    k->km_id = ina.n_know_mg;
    k->km_type = kt_type;
    k->km_category = cat;
    k->kno_acq = kac;
    k->kno_rep = kre;
    k->kno_rea = krn;
    k->kno_app = kap;
    k->year = year;
    k->active = 1;
    ina.total_kno_acq += kac;
    ina.n_know_mg++;
    print_str("[INA] Know mg "); print_int(ina.n_know_mg - 1);
    print_str(" type="); print_int(kt_type);
    print_str(" cat="); print_int(cat);
    print_str(" kac="); print_int(kac);
    print_str(" kre="); print_int(kre);
    print_str(" krn="); print_int(krn);
    print_str(" kap="); print_int(kap); print_str("\n");
    return ina.n_know_mg - 1;
}

int ina_info_sys(int st_type, int cat, int sds, int san, int sim, int sev, int year) {
    if (ina.n_info_sys >= MAX_INFO_SYS) return -1;
    info_sys_t* s = &info_syss[ina.n_info_sys];
    s->is_id = ina.n_info_sys;
    s->is_type = st_type;
    s->is_category = cat;
    s->sys_des = sds;
    s->sys_ana = san;
    s->sys_imp = sim;
    s->sys_eva = sev;
    s->year = year;
    s->active = 1;
    ina.total_sys_des += sds;
    ina.n_info_sys++;
    print_str("[INA] Info sys "); print_int(ina.n_info_sys - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" sds="); print_int(sds);
    print_str(" san="); print_int(san);
    print_str(" sim="); print_int(sim);
    print_str(" sev="); print_int(sev); print_str("\n");
    return ina.n_info_sys - 1;
}

int ina_info_sec(int st_type, int cat, int cry, int nsc, int ssc, int prv, int year) {
    if (ina.n_info_sec >= MAX_INFO_SEC) return -1;
    info_sec_t* s = &info_secs[ina.n_info_sec];
    s->sc_id = ina.n_info_sec;
    s->sc_type = st_type;
    s->sc_category = cat;
    s->crypto = cry;
    s->net_sec = nsc;
    s->sys_sec = ssc;
    s->privacy = prv;
    s->year = year;
    s->active = 1;
    ina.total_crypto += cry;
    ina.n_info_sec++;
    print_str("[INA] Info sec "); print_int(ina.n_info_sec - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" cry="); print_int(cry);
    print_str(" nsc="); print_int(nsc);
    print_str(" ssc="); print_int(ssc);
    print_str(" prv="); print_int(prv); print_str("\n");
    return ina.n_info_sec - 1;
}

void ina_theory_report(void) {
    print_str("[INA] Information theory report:\n");
    print_str("  Theory categories: "); print_int(ina.n_info_th); print_str("\n");
    print_str("  Total information entropy: "); print_int(ina.total_info_ent); print_str("\n");
}

void ina_data_report(void) {
    print_str("[INA] Data science report:\n");
    print_str("  Data categories: "); print_int(ina.n_data_sc); print_str("\n");
    print_str("  Total data mining: "); print_int(ina.total_dat_min); print_str("\n");
}

void ina_full_report(void) {
    print_str("[INA] Full report:\n");
    print_str("  Knowledge categories: "); print_int(ina.n_know_mg); print_str("\n");
    print_str("  Total knowledge acquisition: "); print_int(ina.total_kno_acq); print_str("\n");
    print_str("  System categories: "); print_int(ina.n_info_sys); print_str("\n");
    print_str("  Total system design: "); print_int(ina.total_sys_des); print_str("\n");
    print_str("  Security categories: "); print_int(ina.n_info_sec); print_str("\n");
    print_str("  Total cryptography: "); print_int(ina.total_crypto); print_str("\n");
}

void ina_print_state(void) {
    print_str("[INA] It="); print_int(ina.n_info_th);
    print_str(" Ds="); print_int(ina.n_data_sc);
    print_str(" Km="); print_int(ina.n_know_mg);
    print_str(" Is="); print_int(ina.n_info_sys);
    print_str(" Sc="); print_int(ina.n_info_sec);
    print_str("\n");
}

int main(void) {
    print_str("=== Information Admin Demo ===\n\n");
    ina_init();

    print_str("Information theory...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int ien = 55 + (i * 13);
        int ccd = 40 + (i * 10);
        int sco = 22 + (i * 5);
        int ime = 15 + (i * 3);
        int year = 2020 + (i % 5);
        ina_info_th(type, cat, ien, ccd, sco, ime, year);
    }

    print_str("\nData science...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int dmn = 48 + (i * 11);
        int mcl = 35 + (i * 8);
        int sta = 20 + (i * 4);
        int dvs = 12 + (i * 3);
        int year = 2021 + (i % 4);
        ina_data_sc(type, cat, dmn, mcl, sta, dvs, year);
    }

    print_str("\nKnowledge management...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int kac = 42 + (i * 10);
        int kre = 28 + (i * 7);
        int krn = 18 + (i * 4);
        int kap = 10 + (i * 2);
        int year = 2022 + (i % 3);
        ina_know_mg(type, cat, kac, kre, krn, kap, year);
    }

    print_str("\nInformation systems...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int sds = 35 + (i * 8);
        int san = 25 + (i * 6);
        int sim = 15 + (i * 3);
        int sev = 10 + (i * 2);
        int year = 2023 + (i % 2);
        ina_info_sys(type, cat, sds, san, sim, sev, year);
    }

    print_str("\nInformation security...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cry = 30 + (i * 7);
        int nsc = 22 + (i * 5);
        int ssc = 12 + (i * 3);
        int prv = 8 + (i * 2);
        int year = 2024;
        ina_info_sec(type, cat, cry, nsc, ssc, prv, year);
    }

    print_str("\nTheory report...\n");
    ina_theory_report();

    print_str("\nData report...\n");
    ina_data_report();

    print_str("\nFull report...\n");
    ina_full_report();

    print_str("\nFinal state...\n");
    ina_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
