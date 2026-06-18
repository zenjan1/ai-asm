/* traditional_chinese_medicine_admin: TCM administration system (v1.0)
 * TCM basics, diagnosis, materia medica, formulogy, acupuncture
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

#define MAX_TCM_BASE    16
#define MAX_TCM_DIAG    14
#define MAX_TCM_HERB    12
#define MAX_TCM_FORM    10
#define MAX_TCM_ACU     10

typedef struct {
    int    tb_id;
    int    tb_type;
    int    tb_category;
    int    yin_yang;
    int    zang_fu;
    int    jing_luo;
    int    qi_blood;
    int    year;
    int    active;
} tcm_base_t;

typedef struct {
    int    td_id;
    int    td_type;
    int    td_category;
    int    si_zhen;
    int    bian_zheng;
    int    tongue_p;
    int    pulse_p;
    int    year;
    int    active;
} tcm_diag_t;

typedef struct {
    int    th_id;
    int    th_type;
    int    th_category;
    int    herb_id;
    int    herb_proc;
    int    herb_prep;
    int    herb_phar;
    int    year;
    int    active;
} tcm_herb_t;

typedef struct {
    int    tf_id;
    int    tf_type;
    int    tf_category;
    int    form_comp;
    int    form_cls;
    int    form_app;
    int    form_res;
    int    year;
    int    active;
} tcm_form_t;

typedef struct {
    int    ta_id;
    int    ta_type;
    int    ta_category;
    int    jing_xue;
    int    zhen_ci;
    int    jiu_fa;
    int    tui_na;
    int    year;
    int    active;
} tcm_acu_t;

typedef struct {
    int    n_tcm_base;
    int    n_tcm_diag;
    int    n_tcm_herb;
    int    n_tcm_form;
    int    n_tcm_acu;
    int    total_yin_yang;
    int    total_si_zhen;
    int    total_herb_id;
    int    total_form_comp;
    int    total_jing_xue;
} tca_state_t;

static tcm_base_t tcm_bases[MAX_TCM_BASE];
static tcm_diag_t tcm_diags[MAX_TCM_DIAG];
static tcm_herb_t tcm_herbs[MAX_TCM_HERB];
static tcm_form_t tcm_forms[MAX_TCM_FORM];
static tcm_acu_t tcm_acus[MAX_TCM_ACU];
static tca_state_t tca;

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

int tca_init(void) {
    if (initialized) return -1;
    tca.n_tcm_base = 0; tca.n_tcm_diag = 0; tca.n_tcm_herb = 0;
    tca.n_tcm_form = 0; tca.n_tcm_acu = 0;
    tca.total_yin_yang = 0; tca.total_si_zhen = 0;
    tca.total_herb_id = 0; tca.total_form_comp = 0;
    tca.total_jing_xue = 0;
    for (int i = 0; i < MAX_TCM_BASE; i++) tcm_bases[i].active = 0;
    for (int i = 0; i < MAX_TCM_DIAG; i++) tcm_diags[i].active = 0;
    for (int i = 0; i < MAX_TCM_HERB; i++) tcm_herbs[i].active = 0;
    for (int i = 0; i < MAX_TCM_FORM; i++) tcm_forms[i].active = 0;
    for (int i = 0; i < MAX_TCM_ACU; i++) tcm_acus[i].active = 0;
    initialized = 1;
    print_str("[TCA] TCM initialized\n");
    return 0;
}

int tca_tcm_base(int bt_type, int cat, int yny, int zfx, int jlx, int qbx, int year) {
    if (tca.n_tcm_base >= MAX_TCM_BASE) return -1;
    tcm_base_t* t = &tcm_bases[tca.n_tcm_base];
    t->tb_id = tca.n_tcm_base;
    t->tb_type = bt_type;
    t->tb_category = cat;
    t->yin_yang = yny;
    t->zang_fu = zfx;
    t->jing_luo = jlx;
    t->qi_blood = qbx;
    t->year = year;
    t->active = 1;
    tca.total_yin_yang += yny;
    tca.n_tcm_base++;
    print_str("[TCA] TCM base "); print_int(tca.n_tcm_base - 1);
    print_str(" type="); print_int(bt_type);
    print_str(" cat="); print_int(cat);
    print_str(" yny="); print_int(yny);
    print_str(" zfx="); print_int(zfx);
    print_str(" jlx="); print_int(jlx);
    print_str(" qbx="); print_int(qbx); print_str("\n");
    return tca.n_tcm_base - 1;
}

int tca_tcm_diag(int dt_type, int cat, int szh, int bzh, int tgp, int mzp, int year) {
    if (tca.n_tcm_diag >= MAX_TCM_DIAG) return -1;
    tcm_diag_t* t = &tcm_diags[tca.n_tcm_diag];
    t->td_id = tca.n_tcm_diag;
    t->td_type = dt_type;
    t->td_category = cat;
    t->si_zhen = szh;
    t->bian_zheng = bzh;
    t->tongue_p = tgp;
    t->pulse_p = mzp;
    t->year = year;
    t->active = 1;
    tca.total_si_zhen += szh;
    tca.n_tcm_diag++;
    print_str("[TCA] TCM diag "); print_int(tca.n_tcm_diag - 1);
    print_str(" type="); print_int(dt_type);
    print_str(" cat="); print_int(cat);
    print_str(" szh="); print_int(szh);
    print_str(" bzh="); print_int(bzh);
    print_str(" tgp="); print_int(tgp);
    print_str(" mzp="); print_int(mzp); print_str("\n");
    return tca.n_tcm_diag - 1;
}

int tca_tcm_herb(int ht_type, int cat, int hbi, int hbp, int hbpp, int hbph, int year) {
    if (tca.n_tcm_herb >= MAX_TCM_HERB) return -1;
    tcm_herb_t* t = &tcm_herbs[tca.n_tcm_herb];
    t->th_id = tca.n_tcm_herb;
    t->th_type = ht_type;
    t->th_category = cat;
    t->herb_id = hbi;
    t->herb_proc = hbp;
    t->herb_prep = hbpp;
    t->herb_phar = hbph;
    t->year = year;
    t->active = 1;
    tca.total_herb_id += hbi;
    tca.n_tcm_herb++;
    print_str("[TCA] TCM herb "); print_int(tca.n_tcm_herb - 1);
    print_str(" type="); print_int(ht_type);
    print_str(" cat="); print_int(cat);
    print_str(" hbi="); print_int(hbi);
    print_str(" hbp="); print_int(hbp);
    print_str(" hbpp="); print_int(hbpp);
    print_str(" hbph="); print_int(hbph); print_str("\n");
    return tca.n_tcm_herb - 1;
}

int tca_tcm_form(int ft_type, int cat, int fcp, int fcl, int fap, int frs, int year) {
    if (tca.n_tcm_form >= MAX_TCM_FORM) return -1;
    tcm_form_t* t = &tcm_forms[tca.n_tcm_form];
    t->tf_id = tca.n_tcm_form;
    t->tf_type = ft_type;
    t->tf_category = cat;
    t->form_comp = fcp;
    t->form_cls = fcl;
    t->form_app = fap;
    t->form_res = frs;
    t->year = year;
    t->active = 1;
    tca.total_form_comp += fcp;
    tca.n_tcm_form++;
    print_str("[TCA] TCM form "); print_int(tca.n_tcm_form - 1);
    print_str(" type="); print_int(ft_type);
    print_str(" cat="); print_int(cat);
    print_str(" fcp="); print_int(fcp);
    print_str(" fcl="); print_int(fcl);
    print_str(" fap="); print_int(fap);
    print_str(" frs="); print_int(frs); print_str("\n");
    return tca.n_tcm_form - 1;
}

int tca_tcm_acu(int at_type, int cat, int jgx, int znc, int juf, int tnn, int year) {
    if (tca.n_tcm_acu >= MAX_TCM_ACU) return -1;
    tcm_acu_t* t = &tcm_acus[tca.n_tcm_acu];
    t->ta_id = tca.n_tcm_acu;
    t->ta_type = at_type;
    t->ta_category = cat;
    t->jing_xue = jgx;
    t->zhen_ci = znc;
    t->jiu_fa = juf;
    t->tui_na = tnn;
    t->year = year;
    t->active = 1;
    tca.total_jing_xue += jgx;
    tca.n_tcm_acu++;
    print_str("[TCA] TCM acu "); print_int(tca.n_tcm_acu - 1);
    print_str(" type="); print_int(at_type);
    print_str(" cat="); print_int(cat);
    print_str(" jgx="); print_int(jgx);
    print_str(" znc="); print_int(znc);
    print_str(" juf="); print_int(juf);
    print_str(" tnn="); print_int(tnn); print_str("\n");
    return tca.n_tcm_acu - 1;
}

void tca_base_report(void) {
    print_str("[TCA] TCM basics report:\n");
    print_str("  Basics categories: "); print_int(tca.n_tcm_base); print_str("\n");
    print_str("  Total yin-yang: "); print_int(tca.total_yin_yang); print_str("\n");
}

void tca_diag_report(void) {
    print_str("[TCA] TCM diagnosis report:\n");
    print_str("  Diagnosis categories: "); print_int(tca.n_tcm_diag); print_str("\n");
    print_str("  Total si-zhen: "); print_int(tca.total_si_zhen); print_str("\n");
}

void tca_full_report(void) {
    print_str("[TCA] Full report:\n");
    print_str("  Herb categories: "); print_int(tca.n_tcm_herb); print_str("\n");
    print_str("  Total herb identification: "); print_int(tca.total_herb_id); print_str("\n");
    print_str("  Formula categories: "); print_int(tca.n_tcm_form); print_str("\n");
    print_str("  Total formula composition: "); print_int(tca.total_form_comp); print_str("\n");
    print_str("  Acupuncture categories: "); print_int(tca.n_tcm_acu); print_str("\n");
    print_str("  Total jing-xue points: "); print_int(tca.total_jing_xue); print_str("\n");
}

void tca_print_state(void) {
    print_str("[TCA] Bs="); print_int(tca.n_tcm_base);
    print_str(" Dg="); print_int(tca.n_tcm_diag);
    print_str(" Hb="); print_int(tca.n_tcm_herb);
    print_str(" Fm="); print_int(tca.n_tcm_form);
    print_str(" Ac="); print_int(tca.n_tcm_acu);
    print_str("\n");
}

int main(void) {
    print_str("=== TCM Admin Demo ===\n\n");
    tca_init();

    print_str("TCM basics...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int yny = 55 + (i * 13);
        int zfx = 40 + (i * 10);
        int jlx = 22 + (i * 5);
        int qbx = 15 + (i * 3);
        int year = 2020 + (i % 5);
        tca_tcm_base(type, cat, yny, zfx, jlx, qbx, year);
    }

    print_str("\nTCM diagnosis...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int szh = 48 + (i * 11);
        int bzh = 35 + (i * 8);
        int tgp = 20 + (i * 4);
        int mzp = 12 + (i * 3);
        int year = 2021 + (i % 4);
        tca_tcm_diag(type, cat, szh, bzh, tgp, mzp, year);
    }

    print_str("\nChinese herbs...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int hbi = 42 + (i * 10);
        int hbp = 28 + (i * 7);
        int hbpp = 18 + (i * 4);
        int hbph = 10 + (i * 2);
        int year = 2022 + (i % 3);
        tca_tcm_herb(type, cat, hbi, hbp, hbpp, hbph, year);
    }

    print_str("\nFormulas...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int fcp = 35 + (i * 8);
        int fcl = 25 + (i * 6);
        int fap = 15 + (i * 3);
        int frs = 10 + (i * 2);
        int year = 2023 + (i % 2);
        tca_tcm_form(type, cat, fcp, fcl, fap, frs, year);
    }

    print_str("\nAcupuncture...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int jgx = 30 + (i * 7);
        int znc = 22 + (i * 5);
        int juf = 12 + (i * 3);
        int tnn = 8 + (i * 2);
        int year = 2024;
        tca_tcm_acu(type, cat, jgx, znc, juf, tnn, year);
    }

    print_str("\nBasics report...\n");
    tca_base_report();

    print_str("\nDiagnosis report...\n");
    tca_diag_report();

    print_str("\nFull report...\n");
    tca_full_report();

    print_str("\nFinal state...\n");
    tca_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
