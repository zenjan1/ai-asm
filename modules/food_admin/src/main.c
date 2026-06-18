/* food_admin: Food science administration system (v1.0)
 * Food science, food processing, agricultural products, brewing, restaurant
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

#define MAX_FOO_SCI     16
#define MAX_FOO_PRO     14
#define MAX_AGR_PRO     12
#define MAX_BRE_ENG     10
#define MAX_RES_MAN     10

typedef struct {
    int    fs_id;
    int    fs_type;
    int    fs_category;
    int    foo_che;
    int    foo_mic;
    int    foo_nut;
    int    foo_saf;
    int    year;
    int    active;
} foo_sci_t;

typedef struct {
    int    fp_id;
    int    fp_type;
    int    fp_category;
    int    foo_tec;
    int    foo_mac;
    int    foo_pac;
    int    foo_sto;
    int    year;
    int    active;
} foo_pro_t;

typedef struct {
    int    ap_id;
    int    ap_type;
    int    ap_category;
    int    gra_pro;
    int    oi_pro;
    int    ani_pro;
    int    fru_pro;
    int    year;
    int    active;
} agr_pro_t;

typedef struct {
    int    be_id;
    int    be_type;
    int    be_category;
    int    bre_tec;
    int    fer_eng;
    int    bre_ski;
    int    liq_eva;
    int    year;
    int    active;
} bre_eng_t;

typedef struct {
    int    rm_id;
    int    rm_type;
    int    rm_category;
    int    coo_tec;
    int    res_nut;
    int    res_hyg;
    int    res_biz;
    int    year;
    int    active;
} res_man_t;

typedef struct {
    int    n_foo_sci;
    int    n_foo_pro;
    int    n_agr_pro;
    int    n_bre_eng;
    int    n_res_man;
    int    total_foo_che;
    int    total_foo_tec;
    int    total_gra_pro;
    int    total_bre_tec;
    int    total_coo_tec;
} fda_state_t;

static foo_sci_t foo_scis[MAX_FOO_SCI];
static foo_pro_t foo_pros[MAX_FOO_PRO];
static agr_pro_t agr_pros[MAX_AGR_PRO];
static bre_eng_t bre_engs[MAX_BRE_ENG];
static res_man_t res_mans[MAX_RES_MAN];
static fda_state_t fda;

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

int fda_init(void) {
    if (initialized) return -1;
    fda.n_foo_sci = 0; fda.n_foo_pro = 0; fda.n_agr_pro = 0;
    fda.n_bre_eng = 0; fda.n_res_man = 0;
    fda.total_foo_che = 0; fda.total_foo_tec = 0;
    fda.total_gra_pro = 0; fda.total_bre_tec = 0;
    fda.total_coo_tec = 0;
    for (int i = 0; i < MAX_FOO_SCI; i++) foo_scis[i].active = 0;
    for (int i = 0; i < MAX_FOO_PRO; i++) foo_pros[i].active = 0;
    for (int i = 0; i < MAX_AGR_PRO; i++) agr_pros[i].active = 0;
    for (int i = 0; i < MAX_BRE_ENG; i++) bre_engs[i].active = 0;
    for (int i = 0; i < MAX_RES_MAN; i++) res_mans[i].active = 0;
    initialized = 1;
    print_str("[FDA] Food initialized\n");
    return 0;
}

int fda_foo_sci(int ft_type, int cat, int fch, int fmi, int fnu, int fsa, int year) {
    if (fda.n_foo_sci >= MAX_FOO_SCI) return -1;
    foo_sci_t* f = &foo_scis[fda.n_foo_sci];
    f->fs_id = fda.n_foo_sci;
    f->fs_type = ft_type;
    f->fs_category = cat;
    f->foo_che = fch;
    f->foo_mic = fmi;
    f->foo_nut = fnu;
    f->foo_saf = fsa;
    f->year = year;
    f->active = 1;
    fda.total_foo_che += fch;
    fda.n_foo_sci++;
    print_str("[FDA] Foo sci "); print_int(fda.n_foo_sci - 1);
    print_str(" type="); print_int(ft_type);
    print_str(" cat="); print_int(cat);
    print_str(" fch="); print_int(fch);
    print_str(" fmi="); print_int(fmi);
    print_str(" fnu="); print_int(fnu);
    print_str(" fsa="); print_int(fsa); print_str("\n");
    return fda.n_foo_sci - 1;
}

int fda_foo_pro(int pt_type, int cat, int fte, int fma, int fpa, int fst, int year) {
    if (fda.n_foo_pro >= MAX_FOO_PRO) return -1;
    foo_pro_t* p = &foo_pros[fda.n_foo_pro];
    p->fp_id = fda.n_foo_pro;
    p->fp_type = pt_type;
    p->fp_category = cat;
    p->foo_tec = fte;
    p->foo_mac = fma;
    p->foo_pac = fpa;
    p->foo_sto = fst;
    p->year = year;
    p->active = 1;
    fda.total_foo_tec += fte;
    fda.n_foo_pro++;
    print_str("[FDA] Foo pro "); print_int(fda.n_foo_pro - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" fte="); print_int(fte);
    print_str(" fma="); print_int(fma);
    print_str(" fpa="); print_int(fpa);
    print_str(" fst="); print_int(fst); print_str("\n");
    return fda.n_foo_pro - 1;
}

int fda_agr_pro(int at_type, int cat, int grp, int oip, int anp, int frp, int year) {
    if (fda.n_agr_pro >= MAX_AGR_PRO) return -1;
    agr_pro_t* a = &agr_pros[fda.n_agr_pro];
    a->ap_id = fda.n_agr_pro;
    a->ap_type = at_type;
    a->ap_category = cat;
    a->gra_pro = grp;
    a->oi_pro = oip;
    a->ani_pro = anp;
    a->fru_pro = frp;
    a->year = year;
    a->active = 1;
    fda.total_gra_pro += grp;
    fda.n_agr_pro++;
    print_str("[FDA] Agr pro "); print_int(fda.n_agr_pro - 1);
    print_str(" type="); print_int(at_type);
    print_str(" cat="); print_int(cat);
    print_str(" grp="); print_int(grp);
    print_str(" oip="); print_int(oip);
    print_str(" anp="); print_int(anp);
    print_str(" frp="); print_int(frp); print_str("\n");
    return fda.n_agr_pro - 1;
}

int fda_bre_eng(int bt_type, int cat, int bte, int fen, int bsk, int lev, int year) {
    if (fda.n_bre_eng >= MAX_BRE_ENG) return -1;
    bre_eng_t* b = &bre_engs[fda.n_bre_eng];
    b->be_id = fda.n_bre_eng;
    b->be_type = bt_type;
    b->be_category = cat;
    b->bre_tec = bte;
    b->fer_eng = fen;
    b->bre_ski = bsk;
    b->liq_eva = lev;
    b->year = year;
    b->active = 1;
    fda.total_bre_tec += bte;
    fda.n_bre_eng++;
    print_str("[FDA] Bre eng "); print_int(fda.n_bre_eng - 1);
    print_str(" type="); print_int(bt_type);
    print_str(" cat="); print_int(cat);
    print_str(" bte="); print_int(bte);
    print_str(" fen="); print_int(fen);
    print_str(" bsk="); print_int(bsk);
    print_str(" lev="); print_int(lev); print_str("\n");
    return fda.n_bre_eng - 1;
}

int fda_res_man(int rt_type, int cat, int cot, int rnu, int rhy, int rbi, int year) {
    if (fda.n_res_man >= MAX_RES_MAN) return -1;
    res_man_t* r = &res_mans[fda.n_res_man];
    r->rm_id = fda.n_res_man;
    r->rm_type = rt_type;
    r->rm_category = cat;
    r->coo_tec = cot;
    r->res_nut = rnu;
    r->res_hyg = rhy;
    r->res_biz = rbi;
    r->year = year;
    r->active = 1;
    fda.total_coo_tec += cot;
    fda.n_res_man++;
    print_str("[FDA] Res man "); print_int(fda.n_res_man - 1);
    print_str(" type="); print_int(rt_type);
    print_str(" cat="); print_int(cat);
    print_str(" cot="); print_int(cot);
    print_str(" rnu="); print_int(rnu);
    print_str(" rhy="); print_int(rhy);
    print_str(" rbi="); print_int(rbi); print_str("\n");
    return fda.n_res_man - 1;
}

void fda_science_report(void) {
    print_str("[FDA] Food science report:\n");
    print_str("  Science categories: "); print_int(fda.n_foo_sci); print_str("\n");
    print_str("  Total food chemistry: "); print_int(fda.total_foo_che); print_str("\n");
}

void fda_process_report(void) {
    print_str("[FDA] Food process report:\n");
    print_str("  Process categories: "); print_int(fda.n_foo_pro); print_str("\n");
    print_str("  Total food technology: "); print_int(fda.total_foo_tec); print_str("\n");
}

void fda_full_report(void) {
    print_str("[FDA] Full report:\n");
    print_str("  Agricultural categories: "); print_int(fda.n_agr_pro); print_str("\n");
    print_str("  Total grain processing: "); print_int(fda.total_gra_pro); print_str("\n");
    print_str("  Brewing categories: "); print_int(fda.n_bre_eng); print_str("\n");
    print_str("  Total brewing technology: "); print_int(fda.total_bre_tec); print_str("\n");
    print_str("  Restaurant categories: "); print_int(fda.n_res_man); print_str("\n");
    print_str("  Total cooking technology: "); print_int(fda.total_coo_tec); print_str("\n");
}

void fda_print_state(void) {
    print_str("[FDA] Fs="); print_int(fda.n_foo_sci);
    print_str(" Fp="); print_int(fda.n_foo_pro);
    print_str(" Ap="); print_int(fda.n_agr_pro);
    print_str(" Be="); print_int(fda.n_bre_eng);
    print_str(" Rm="); print_int(fda.n_res_man);
    print_str("\n");
}

int main(void) {
    print_str("=== Food Admin Demo ===\n\n");
    fda_init();

    print_str("Food science...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int fch = 55 + (i * 13);
        int fmi = 40 + (i * 10);
        int fnu = 22 + (i * 5);
        int fsa = 15 + (i * 3);
        int year = 2020 + (i % 5);
        fda_foo_sci(type, cat, fch, fmi, fnu, fsa, year);
    }

    print_str("\nFood processing...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int fte = 48 + (i * 11);
        int fma = 35 + (i * 8);
        int fpa = 20 + (i * 4);
        int fst = 12 + (i * 3);
        int year = 2021 + (i % 4);
        fda_foo_pro(type, cat, fte, fma, fpa, fst, year);
    }

    print_str("\nAgricultural processing...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int grp = 42 + (i * 10);
        int oip = 28 + (i * 7);
        int anp = 18 + (i * 4);
        int frp = 10 + (i * 2);
        int year = 2022 + (i % 3);
        fda_agr_pro(type, cat, grp, oip, anp, frp, year);
    }

    print_str("\nBrewing...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int bte = 35 + (i * 8);
        int fen = 25 + (i * 6);
        int bsk = 15 + (i * 3);
        int lev = 10 + (i * 2);
        int year = 2023 + (i % 2);
        fda_bre_eng(type, cat, bte, fen, bsk, lev, year);
    }

    print_str("\nRestaurant...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cot = 30 + (i * 7);
        int rnu = 22 + (i * 5);
        int rhy = 12 + (i * 3);
        int rbi = 8 + (i * 2);
        int year = 2024;
        fda_res_man(type, cat, cot, rnu, rhy, rbi, year);
    }

    print_str("\nScience report...\n");
    fda_science_report();

    print_str("\nProcess report...\n");
    fda_process_report();

    print_str("\nFull report...\n");
    fda_full_report();

    print_str("\nFinal state...\n");
    fda_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
