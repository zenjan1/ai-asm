/* labor_economics_admin: Labor economics administration system (v1.0)
 * Labor market, employment theory, wage theory, labor relations, social security
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

#define MAX_LAB_MKT    16
#define MAX_EMP_THE    14
#define MAX_WAG_THE    12
#define MAX_LAB_REL    10
#define MAX_SOC_SEC    10

typedef struct {
    int    lm_id;
    int    lm_type;
    int    lm_category;
    int    sup_ana;
    int    dem_ana;
    int    mkt_equ;
    int    flo_mec;
    int    year;
    int    active;
} lab_mkt_t;

typedef struct {
    int    et_id;
    int    et_type;
    int    et_category;
    int    emp_the;
    int    une_the;
    int    emp_pol;
    int    ent_pro;
    int    year;
    int    active;
} emp_the_t;

typedef struct {
    int    wt_id;
    int    wt_type;
    int    wt_category;
    int    wag_det;
    int    wag_gab;
    int    min_wag;
    int    com_sys;
    int    year;
    int    active;
} wag_the_t;

typedef struct {
    int    lr_id;
    int    lr_type;
    int    lr_category;
    int    lab_con;
    int    col_neg;
    int    lab_dis;
    int    tra_org;
    int    year;
    int    active;
} lab_rel_t;

typedef struct {
    int    ss_id;
    int    ss_type;
    int    ss_category;
    int    soc_ins;
    int    soc_ass;
    int    soc_wel;
    int    soc_fun;
    int    year;
    int    active;
} soc_sec_t;

typedef struct {
    int    n_lab_mkt;
    int    n_emp_the;
    int    n_wag_the;
    int    n_lab_rel;
    int    n_soc_sec;
    int    total_sup_ana;
    int    total_emp_the;
    int    total_wag_det;
    int    total_lab_con;
    int    total_soc_ins;
} lea_state_t;

static lab_mkt_t lab_mkts[MAX_LAB_MKT];
static emp_the_t emp_thes[MAX_EMP_THE];
static wag_the_t wag_thes[MAX_WAG_THE];
static lab_rel_t lab_rels[MAX_LAB_REL];
static soc_sec_t soc_secs[MAX_SOC_SEC];
static lea_state_t lea;

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

int lea_init(void) {
    if (initialized) return -1;
    lea.n_lab_mkt = 0; lea.n_emp_the = 0; lea.n_wag_the = 0;
    lea.n_lab_rel = 0; lea.n_soc_sec = 0;
    lea.total_sup_ana = 0; lea.total_emp_the = 0;
    lea.total_wag_det = 0; lea.total_lab_con = 0;
    lea.total_soc_ins = 0;
    for (int i = 0; i < MAX_LAB_MKT; i++) lab_mkts[i].active = 0;
    for (int i = 0; i < MAX_EMP_THE; i++) emp_thes[i].active = 0;
    for (int i = 0; i < MAX_WAG_THE; i++) wag_thes[i].active = 0;
    for (int i = 0; i < MAX_LAB_REL; i++) lab_rels[i].active = 0;
    for (int i = 0; i < MAX_SOC_SEC; i++) soc_secs[i].active = 0;
    initialized = 1;
    print_str("[LEA] Labor economics initialized\n");
    return 0;
}

int lea_lab_mkt(int lt_type, int cat, int sana, int dana, int mequ, int fmec, int year) {
    if (lea.n_lab_mkt >= MAX_LAB_MKT) return -1;
    lab_mkt_t* l = &lab_mkts[lea.n_lab_mkt];
    l->lm_id = lea.n_lab_mkt;
    l->lm_type = lt_type;
    l->lm_category = cat;
    l->sup_ana = sana;
    l->dem_ana = dana;
    l->mkt_equ = mequ;
    l->flo_mec = fmec;
    l->year = year;
    l->active = 1;
    lea.total_sup_ana += sana;
    lea.n_lab_mkt++;
    print_str("[LEA] Lab mkt "); print_int(lea.n_lab_mkt - 1);
    print_str(" type="); print_int(lt_type);
    print_str(" cat="); print_int(cat);
    print_str(" sana="); print_int(sana);
    print_str(" dana="); print_int(dana);
    print_str(" mequ="); print_int(mequ);
    print_str(" fmec="); print_int(fmec); print_str("\n");
    return lea.n_lab_mkt - 1;
}

int lea_emp_the(int et_type, int cat, int ethe, int uth, int epol, int epro, int year) {
    if (lea.n_emp_the >= MAX_EMP_THE) return -1;
    emp_the_t* e = &emp_thes[lea.n_emp_the];
    e->et_id = lea.n_emp_the;
    e->et_type = et_type;
    e->et_category = cat;
    e->emp_the = ethe;
    e->une_the = uth;
    e->emp_pol = epol;
    e->ent_pro = epro;
    e->year = year;
    e->active = 1;
    lea.total_emp_the += ethe;
    lea.n_emp_the++;
    print_str("[LEA] Emp the "); print_int(lea.n_emp_the - 1);
    print_str(" type="); print_int(et_type);
    print_str(" cat="); print_int(cat);
    print_str(" ethe="); print_int(ethe);
    print_str(" uth="); print_int(uth);
    print_str(" epol="); print_int(epol);
    print_str(" epro="); print_int(epro); print_str("\n");
    return lea.n_emp_the - 1;
}

int lea_wag_the(int wt_type, int cat, int wdet, int wgab, int mwag, int csys, int year) {
    if (lea.n_wag_the >= MAX_WAG_THE) return -1;
    wag_the_t* w = &wag_thes[lea.n_wag_the];
    w->wt_id = lea.n_wag_the;
    w->wt_type = wt_type;
    w->wt_category = cat;
    w->wag_det = wdet;
    w->wag_gab = wgab;
    w->min_wag = mwag;
    w->com_sys = csys;
    w->year = year;
    w->active = 1;
    lea.total_wag_det += wdet;
    lea.n_wag_the++;
    print_str("[LEA] Wag the "); print_int(lea.n_wag_the - 1);
    print_str(" type="); print_int(wt_type);
    print_str(" cat="); print_int(cat);
    print_str(" wdet="); print_int(wdet);
    print_str(" wgab="); print_int(wgab);
    print_str(" mwag="); print_int(mwag);
    print_str(" csys="); print_int(csys); print_str("\n");
    return lea.n_wag_the - 1;
}

int lea_lab_rel(int lt_type, int cat, int lcon, int cneg, int ldis, int torg, int year) {
    if (lea.n_lab_rel >= MAX_LAB_REL) return -1;
    lab_rel_t* l = &lab_rels[lea.n_lab_rel];
    l->lr_id = lea.n_lab_rel;
    l->lr_type = lt_type;
    l->lr_category = cat;
    l->lab_con = lcon;
    l->col_neg = cneg;
    l->lab_dis = ldis;
    l->tra_org = torg;
    l->year = year;
    l->active = 1;
    lea.total_lab_con += lcon;
    lea.n_lab_rel++;
    print_str("[LEA] Lab rel "); print_int(lea.n_lab_rel - 1);
    print_str(" type="); print_int(lt_type);
    print_str(" cat="); print_int(cat);
    print_str(" lcon="); print_int(lcon);
    print_str(" cneg="); print_int(cneg);
    print_str(" ldis="); print_int(ldis);
    print_str(" torg="); print_int(torg); print_str("\n");
    return lea.n_lab_rel - 1;
}

int lea_soc_sec(int st_type, int cat, int sins, int sass, int swel, int sfun, int year) {
    if (lea.n_soc_sec >= MAX_SOC_SEC) return -1;
    soc_sec_t* s = &soc_secs[lea.n_soc_sec];
    s->ss_id = lea.n_soc_sec;
    s->ss_type = st_type;
    s->ss_category = cat;
    s->soc_ins = sins;
    s->soc_ass = sass;
    s->soc_wel = swel;
    s->soc_fun = sfun;
    s->year = year;
    s->active = 1;
    lea.total_soc_ins += sins;
    lea.n_soc_sec++;
    print_str("[LEA] Soc sec "); print_int(lea.n_soc_sec - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" sins="); print_int(sins);
    print_str(" sass="); print_int(sass);
    print_str(" swel="); print_int(swel);
    print_str(" sfun="); print_int(sfun); print_str("\n");
    return lea.n_soc_sec - 1;
}

void lea_market_report(void) {
    print_str("[LEA] Labor market report:\n");
    print_str("  Market categories: "); print_int(lea.n_lab_mkt); print_str("\n");
    print_str("  Total supply analysis: "); print_int(lea.total_sup_ana); print_str("\n");
}

void lea_employment_report(void) {
    print_str("[LEA] Employment report:\n");
    print_str("  Employment categories: "); print_int(lea.n_emp_the); print_str("\n");
    print_str("  Total employment theory: "); print_int(lea.total_emp_the); print_str("\n");
}

void lea_full_report(void) {
    print_str("[LEA] Full report:\n");
    print_str("  Wage categories: "); print_int(lea.n_wag_the); print_str("\n");
    print_str("  Total wage determination: "); print_int(lea.total_wag_det); print_str("\n");
    print_str("  Relations categories: "); print_int(lea.n_lab_rel); print_str("\n");
    print_str("  Total labor contracts: "); print_int(lea.total_lab_con); print_str("\n");
    print_str("  Social security categories: "); print_int(lea.n_soc_sec); print_str("\n");
    print_str("  Total social insurance: "); print_int(lea.total_soc_ins); print_str("\n");
}

void lea_print_state(void) {
    print_str("[LEA] Lm="); print_int(lea.n_lab_mkt);
    print_str(" Et="); print_int(lea.n_emp_the);
    print_str(" Wt="); print_int(lea.n_wag_the);
    print_str(" Lr="); print_int(lea.n_lab_rel);
    print_str(" Ss="); print_int(lea.n_soc_sec);
    print_str("\n");
}

int main(void) {
    print_str("=== Labor Economics Admin Demo ===\n\n");
    lea_init();

    print_str("Labor market...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int sana = 55 + (i * 13);
        int dana = 40 + (i * 10);
        int mequ = 22 + (i * 5);
        int fmec = 15 + (i * 3);
        int year = 2020 + (i % 5);
        lea_lab_mkt(type, cat, sana, dana, mequ, fmec, year);
    }

    print_str("\nEmployment theory...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ethe = 48 + (i * 11);
        int uth = 35 + (i * 8);
        int epol = 20 + (i * 4);
        int epro = 12 + (i * 3);
        int year = 2021 + (i % 4);
        lea_emp_the(type, cat, ethe, uth, epol, epro, year);
    }

    print_str("\nWage theory...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int wdet = 42 + (i * 10);
        int wgab = 28 + (i * 7);
        int mwag = 18 + (i * 4);
        int csys = 10 + (i * 2);
        int year = 2022 + (i % 3);
        lea_wag_the(type, cat, wdet, wgab, mwag, csys, year);
    }

    print_str("\nLabor relations...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int lcon = 35 + (i * 8);
        int cneg = 25 + (i * 6);
        int ldis = 15 + (i * 3);
        int torg = 10 + (i * 2);
        int year = 2023 + (i % 2);
        lea_lab_rel(type, cat, lcon, cneg, ldis, torg, year);
    }

    print_str("\nSocial security...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int sins = 30 + (i * 7);
        int sass = 22 + (i * 5);
        int swel = 12 + (i * 3);
        int sfun = 8 + (i * 2);
        int year = 2024;
        lea_soc_sec(type, cat, sins, sass, swel, sfun, year);
    }

    print_str("\nMarket report...\n");
    lea_market_report();

    print_str("\nEmployment report...\n");
    lea_employment_report();

    print_str("\nFull report...\n");
    lea_full_report();

    print_str("\nFinal state...\n");
    lea_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
