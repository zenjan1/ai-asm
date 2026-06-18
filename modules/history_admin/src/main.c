/* history_admin: History administration system (v1.0)
 * Chinese ancient, Chinese modern, World ancient, World modern, historical documents
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

#define MAX_CHI_ANC     16
#define MAX_CHI_MOD     14
#define MAX_WOR_ANC     12
#define MAX_WOR_MOD     10
#define MAX_DOC_S       10

typedef struct {
    int    ca_id;
    int    ca_type;
    int    ca_category;
    int    pre_qin;
    int    qin_han;
    int    wei_jin;
    int    sui_tang;
    int    song_yuan;
    int    year;
    int    active;
} chi_anc_t;

typedef struct {
    int    cm_id;
    int    cm_type;
    int    cm_category;
    int    late_qing;
    int    republ;
    int    revolu;
    int    contemp;
    int    year;
    int    active;
} chi_mod_t;

typedef struct {
    int    wa_id;
    int    wa_type;
    int    wa_category;
    int    ancient_e;
    int    ancient_g;
    int    ancient_r;
    int    medieval;
    int    year;
    int    active;
} wor_anc_t;

typedef struct {
    int    wm_id;
    int    wm_type;
    int    wm_category;
    int    renaiss;
    int    enlight;
    int    indust;
    int    worldw;
    int    year;
    int    active;
} wor_mod_t;

typedef struct {
    int    dc_id;
    int    dc_type;
    int    dc_category;
    int    ancient_b;
    int    doc_verify;
    int    archive;
    int    digital;
    int    year;
    int    active;
} doc_s_t;

typedef struct {
    int    n_chi_anc;
    int    n_chi_mod;
    int    n_wor_anc;
    int    n_wor_mod;
    int    n_doc_s;
    int    total_pre_qin;
    int    total_late_qing;
    int    total_ancient_e;
    int    total_renaiss;
    int    total_ancient_b;
} ha_state_t;

static chi_anc_t chi_ancs[MAX_CHI_ANC];
static chi_mod_t chi_mods[MAX_CHI_MOD];
static wor_anc_t wor_ancs[MAX_WOR_ANC];
static wor_mod_t wor_mods[MAX_WOR_MOD];
static doc_s_t doc_ss[MAX_DOC_S];
static ha_state_t ha;

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

int ha_init(void) {
    if (initialized) return -1;
    ha.n_chi_anc = 0; ha.n_chi_mod = 0; ha.n_wor_anc = 0;
    ha.n_wor_mod = 0; ha.n_doc_s = 0;
    ha.total_pre_qin = 0; ha.total_late_qing = 0;
    ha.total_ancient_e = 0; ha.total_renaiss = 0;
    ha.total_ancient_b = 0;
    for (int i = 0; i < MAX_CHI_ANC; i++) chi_ancs[i].active = 0;
    for (int i = 0; i < MAX_CHI_MOD; i++) chi_mods[i].active = 0;
    for (int i = 0; i < MAX_WOR_ANC; i++) wor_ancs[i].active = 0;
    for (int i = 0; i < MAX_WOR_MOD; i++) wor_mods[i].active = 0;
    for (int i = 0; i < MAX_DOC_S; i++) doc_ss[i].active = 0;
    initialized = 1;
    print_str("[HA] History initialized\n");
    return 0;
}

int ha_chi_anc(int ct_type, int cat, int pqq, int qhh, int wjj, int stg, int syy, int year) {
    if (ha.n_chi_anc >= MAX_CHI_ANC) return -1;
    chi_anc_t* c = &chi_ancs[ha.n_chi_anc];
    c->ca_id = ha.n_chi_anc;
    c->ca_type = ct_type;
    c->ca_category = cat;
    c->pre_qin = pqq;
    c->qin_han = qhh;
    c->wei_jin = wjj;
    c->sui_tang = stg;
    c->song_yuan = syy;
    c->year = year;
    c->active = 1;
    ha.total_pre_qin += pqq;
    ha.n_chi_anc++;
    print_str("[HA] Chi anc "); print_int(ha.n_chi_anc - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" pqq="); print_int(pqq);
    print_str(" qhh="); print_int(qhh);
    print_str(" wjj="); print_int(wjj);
    print_str(" stg="); print_int(stg);
    print_str(" syy="); print_int(syy); print_str("\n");
    return ha.n_chi_anc - 1;
}

int ha_chi_mod(int ct_type, int cat, int lqq, int rpb, int rvl, int ctm, int year) {
    if (ha.n_chi_mod >= MAX_CHI_MOD) return -1;
    chi_mod_t* c = &chi_mods[ha.n_chi_mod];
    c->cm_id = ha.n_chi_mod;
    c->cm_type = ct_type;
    c->cm_category = cat;
    c->late_qing = lqq;
    c->republ = rpb;
    c->revolu = rvl;
    c->contemp = ctm;
    c->year = year;
    c->active = 1;
    ha.total_late_qing += lqq;
    ha.n_chi_mod++;
    print_str("[HA] Chi mod "); print_int(ha.n_chi_mod - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" lqq="); print_int(lqq);
    print_str(" rpb="); print_int(rpb);
    print_str(" rvl="); print_int(rvl);
    print_str(" ctm="); print_int(ctm); print_str("\n");
    return ha.n_chi_mod - 1;
}

int ha_wor_anc(int wt_type, int cat, int ace, int acg, int acr, int mdv, int year) {
    if (ha.n_wor_anc >= MAX_WOR_ANC) return -1;
    wor_anc_t* w = &wor_ancs[ha.n_wor_anc];
    w->wa_id = ha.n_wor_anc;
    w->wa_type = wt_type;
    w->wa_category = cat;
    w->ancient_e = ace;
    w->ancient_g = acg;
    w->ancient_r = acr;
    w->medieval = mdv;
    w->year = year;
    w->active = 1;
    ha.total_ancient_e += ace;
    ha.n_wor_anc++;
    print_str("[HA] Wor anc "); print_int(ha.n_wor_anc - 1);
    print_str(" type="); print_int(wt_type);
    print_str(" cat="); print_int(cat);
    print_str(" ace="); print_int(ace);
    print_str(" acg="); print_int(acg);
    print_str(" acr="); print_int(acr);
    print_str(" mdv="); print_int(mdv); print_str("\n");
    return ha.n_wor_anc - 1;
}

int ha_wor_mod(int wt_type, int cat, int rns, int eng, int ids, int wdw, int year) {
    if (ha.n_wor_mod >= MAX_WOR_MOD) return -1;
    wor_mod_t* w = &wor_mods[ha.n_wor_mod];
    w->wm_id = ha.n_wor_mod;
    w->wm_type = wt_type;
    w->wm_category = cat;
    w->renaiss = rns;
    w->enlight = eng;
    w->indust = ids;
    w->worldw = wdw;
    w->year = year;
    w->active = 1;
    ha.total_renaiss += rns;
    ha.n_wor_mod++;
    print_str("[HA] Wor mod "); print_int(ha.n_wor_mod - 1);
    print_str(" type="); print_int(wt_type);
    print_str(" cat="); print_int(cat);
    print_str(" rns="); print_int(rns);
    print_str(" eng="); print_int(eng);
    print_str(" ids="); print_int(ids);
    print_str(" wdw="); print_int(wdw); print_str("\n");
    return ha.n_wor_mod - 1;
}

int ha_doc_s(int dt_type, int cat, int acb, int dcv, int arc, int dgt, int year) {
    if (ha.n_doc_s >= MAX_DOC_S) return -1;
    doc_s_t* d = &doc_ss[ha.n_doc_s];
    d->dc_id = ha.n_doc_s;
    d->dc_type = dt_type;
    d->dc_category = cat;
    d->ancient_b = acb;
    d->doc_verify = dcv;
    d->archive = arc;
    d->digital = dgt;
    d->year = year;
    d->active = 1;
    ha.total_ancient_b += acb;
    ha.n_doc_s++;
    print_str("[HA] Doc "); print_int(ha.n_doc_s - 1);
    print_str(" type="); print_int(dt_type);
    print_str(" cat="); print_int(cat);
    print_str(" acb="); print_int(acb);
    print_str(" dcv="); print_int(dcv);
    print_str(" arc="); print_int(arc);
    print_str(" dgt="); print_int(dgt); print_str("\n");
    return ha.n_doc_s - 1;
}

void ha_chi_anc_report(void) {
    print_str("[HA] Chinese ancient history report:\n");
    print_str("  Chinese ancient categories: "); print_int(ha.n_chi_anc); print_str("\n");
    print_str("  Total pre-Qin studies: "); print_int(ha.total_pre_qin); print_str("\n");
}

void ha_chi_mod_report(void) {
    print_str("[HA] Chinese modern history report:\n");
    print_str("  Chinese modern categories: "); print_int(ha.n_chi_mod); print_str("\n");
    print_str("  Total late-Qing studies: "); print_int(ha.total_late_qing); print_str("\n");
}

void ha_full_report(void) {
    print_str("[HA] Full report:\n");
    print_str("  World ancient categories: "); print_int(ha.n_wor_anc); print_str("\n");
    print_str("  Total ancient Egypt studies: "); print_int(ha.total_ancient_e); print_str("\n");
    print_str("  World modern categories: "); print_int(ha.n_wor_mod); print_str("\n");
    print_str("  Total Renaissance studies: "); print_int(ha.total_renaiss); print_str("\n");
    print_str("  Document categories: "); print_int(ha.n_doc_s); print_str("\n");
    print_str("  Total ancient book studies: "); print_int(ha.total_ancient_b); print_str("\n");
}

void ha_print_state(void) {
    print_str("[HA] Ca="); print_int(ha.n_chi_anc);
    print_str(" Cm="); print_int(ha.n_chi_mod);
    print_str(" Wa="); print_int(ha.n_wor_anc);
    print_str(" Wm="); print_int(ha.n_wor_mod);
    print_str(" Ds="); print_int(ha.n_doc_s);
    print_str("\n");
}

int main(void) {
    print_str("=== History Admin Demo ===\n\n");
    ha_init();

    print_str("Chinese ancient history...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int pqq = 55 + (i * 13);
        int qhh = 40 + (i * 10);
        int wjj = 22 + (i * 5);
        int stg = 15 + (i * 3);
        int syy = 10 + (i * 2);
        int year = 2020 + (i % 5);
        ha_chi_anc(type, cat, pqq, qhh, wjj, stg, syy, year);
    }

    print_str("\nChinese modern history...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int lqq = 48 + (i * 11);
        int rpb = 35 + (i * 8);
        int rvl = 20 + (i * 4);
        int ctm = 12 + (i * 3);
        int year = 2021 + (i % 4);
        ha_chi_mod(type, cat, lqq, rpb, rvl, ctm, year);
    }

    print_str("\nWorld ancient history...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ace = 42 + (i * 10);
        int acg = 28 + (i * 7);
        int acr = 18 + (i * 4);
        int mdv = 10 + (i * 2);
        int year = 2022 + (i % 3);
        ha_wor_anc(type, cat, ace, acg, acr, mdv, year);
    }

    print_str("\nWorld modern history...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int rns = 35 + (i * 8);
        int eng = 25 + (i * 6);
        int ids = 15 + (i * 3);
        int wdw = 10 + (i * 2);
        int year = 2023 + (i % 2);
        ha_wor_mod(type, cat, rns, eng, ids, wdw, year);
    }

    print_str("\nHistorical documents...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int acb = 30 + (i * 7);
        int dcv = 22 + (i * 5);
        int arc = 12 + (i * 3);
        int dgt = 8 + (i * 2);
        int year = 2024;
        ha_doc_s(type, cat, acb, dcv, arc, dgt, year);
    }

    print_str("\nChinese ancient report...\n");
    ha_chi_anc_report();

    print_str("\nChinese modern report...\n");
    ha_chi_mod_report();

    print_str("\nFull report...\n");
    ha_full_report();

    print_str("\nFinal state...\n");
    ha_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
