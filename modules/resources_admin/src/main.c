/* resources_admin: Resources administration system (v1.0)
 * Resource science, mineral resources, land resources, water resources, bioresources
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

#define MAX_RES_SCI     16
#define MAX_MIN_RES     14
#define MAX_LAN_RES     12
#define MAX_WAT_RES     10
#define MAX_BIO_RES     10

typedef struct {
    int    rs_id;
    int    rs_type;
    int    rs_category;
    int    res_the;
    int    res_eva;
    int    res_pla;
    int    res_man;
    int    year;
    int    active;
} res_sci_t;

typedef struct {
    int    mi_id;
    int    mi_type;
    int    mi_category;
    int    met_min;
    int    non_min;
    int    ene_min;
    int    wat_min;
    int    year;
    int    active;
} min_res_t;

typedef struct {
    int    la_id;
    int    la_type;
    int    la_category;
    int    lan_eva;
    int    lan_pla;
    int    lan_use;
    int    lan_pro;
    int    year;
    int    active;
} lan_res_t;

typedef struct {
    int    wa_id;
    int    wa_type;
    int    wa_category;
    int    sur_wat;
    int    gro_wat;
    int    wat_eva;
    int    wat_pro;
    int    year;
    int    active;
} wat_res_t;

typedef struct {
    int    bi_id;
    int    bi_type;
    int    bi_category;
    int    for_res;
    int    gra_res;
    int    wild_ani;
    int    pla_res;
    int    year;
    int    active;
} bio_res_t;

typedef struct {
    int    n_res_sci;
    int    n_min_res;
    int    n_lan_res;
    int    n_wat_res;
    int    n_bio_res;
    int    total_res_the;
    int    total_met_min;
    int    total_lan_eva;
    int    total_sur_wat;
    int    total_for_res;
} rca_state_t;

static res_sci_t res_scis[MAX_RES_SCI];
static min_res_t min_res[MAX_MIN_RES];
static lan_res_t lan_res[MAX_LAN_RES];
static wat_res_t wat_res[MAX_WAT_RES];
static bio_res_t bio_res[MAX_BIO_RES];
static rca_state_t rca;

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

int rca_init(void) {
    if (initialized) return -1;
    rca.n_res_sci = 0; rca.n_min_res = 0; rca.n_lan_res = 0;
    rca.n_wat_res = 0; rca.n_bio_res = 0;
    rca.total_res_the = 0; rca.total_met_min = 0;
    rca.total_lan_eva = 0; rca.total_sur_wat = 0;
    rca.total_for_res = 0;
    for (int i = 0; i < MAX_RES_SCI; i++) res_scis[i].active = 0;
    for (int i = 0; i < MAX_MIN_RES; i++) min_res[i].active = 0;
    for (int i = 0; i < MAX_LAN_RES; i++) lan_res[i].active = 0;
    for (int i = 0; i < MAX_WAT_RES; i++) wat_res[i].active = 0;
    for (int i = 0; i < MAX_BIO_RES; i++) bio_res[i].active = 0;
    initialized = 1;
    print_str("[RCA] Resources initialized\n");
    return 0;
}

int rca_res_sci(int st_type, int cat, int rth, int rev, int rpl, int rma, int year) {
    if (rca.n_res_sci >= MAX_RES_SCI) return -1;
    res_sci_t* r = &res_scis[rca.n_res_sci];
    r->rs_id = rca.n_res_sci;
    r->rs_type = st_type;
    r->rs_category = cat;
    r->res_the = rth;
    r->res_eva = rev;
    r->res_pla = rpl;
    r->res_man = rma;
    r->year = year;
    r->active = 1;
    rca.total_res_the += rth;
    rca.n_res_sci++;
    print_str("[RCA] Res sci "); print_int(rca.n_res_sci - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" rth="); print_int(rth);
    print_str(" rev="); print_int(rev);
    print_str(" rpl="); print_int(rpl);
    print_str(" rma="); print_int(rma); print_str("\n");
    return rca.n_res_sci - 1;
}

int rca_min_res(int mt_type, int cat, int mmi, int nmi, int emi, int wmi, int year) {
    if (rca.n_min_res >= MAX_MIN_RES) return -1;
    min_res_t* m = &min_res[rca.n_min_res];
    m->mi_id = rca.n_min_res;
    m->mi_type = mt_type;
    m->mi_category = cat;
    m->met_min = mmi;
    m->non_min = nmi;
    m->ene_min = emi;
    m->wat_min = wmi;
    m->year = year;
    m->active = 1;
    rca.total_met_min += mmi;
    rca.n_min_res++;
    print_str("[RCA] Min res "); print_int(rca.n_min_res - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" mmi="); print_int(mmi);
    print_str(" nmi="); print_int(nmi);
    print_str(" emi="); print_int(emi);
    print_str(" wmi="); print_int(wmi); print_str("\n");
    return rca.n_min_res - 1;
}

int rca_lan_res(int lt_type, int cat, int lev, int lpl, int lus, int lpr, int year) {
    if (rca.n_lan_res >= MAX_LAN_RES) return -1;
    lan_res_t* l = &lan_res[rca.n_lan_res];
    l->la_id = rca.n_lan_res;
    l->la_type = lt_type;
    l->la_category = cat;
    l->lan_eva = lev;
    l->lan_pla = lpl;
    l->lan_use = lus;
    l->lan_pro = lpr;
    l->year = year;
    l->active = 1;
    rca.total_lan_eva += lev;
    rca.n_lan_res++;
    print_str("[RCA] Lan res "); print_int(rca.n_lan_res - 1);
    print_str(" type="); print_int(lt_type);
    print_str(" cat="); print_int(cat);
    print_str(" lev="); print_int(lev);
    print_str(" lpl="); print_int(lpl);
    print_str(" lus="); print_int(lus);
    print_str(" lpr="); print_int(lpr); print_str("\n");
    return rca.n_lan_res - 1;
}

int rca_wat_res(int wt_type, int cat, int swa, int gwa, int wev, int wpr, int year) {
    if (rca.n_wat_res >= MAX_WAT_RES) return -1;
    wat_res_t* w = &wat_res[rca.n_wat_res];
    w->wa_id = rca.n_wat_res;
    w->wa_type = wt_type;
    w->wa_category = cat;
    w->sur_wat = swa;
    w->gro_wat = gwa;
    w->wat_eva = wev;
    w->wat_pro = wpr;
    w->year = year;
    w->active = 1;
    rca.total_sur_wat += swa;
    rca.n_wat_res++;
    print_str("[RCA] Wat res "); print_int(rca.n_wat_res - 1);
    print_str(" type="); print_int(wt_type);
    print_str(" cat="); print_int(cat);
    print_str(" swa="); print_int(swa);
    print_str(" gwa="); print_int(gwa);
    print_str(" wev="); print_int(wev);
    print_str(" wpr="); print_int(wpr); print_str("\n");
    return rca.n_wat_res - 1;
}

int rca_bio_res(int bt_type, int cat, int frs, int grs, int wla, int prs, int year) {
    if (rca.n_bio_res >= MAX_BIO_RES) return -1;
    bio_res_t* b = &bio_res[rca.n_bio_res];
    b->bi_id = rca.n_bio_res;
    b->bi_type = bt_type;
    b->bi_category = cat;
    b->for_res = frs;
    b->gra_res = grs;
    b->wild_ani = wla;
    b->pla_res = prs;
    b->year = year;
    b->active = 1;
    rca.total_for_res += frs;
    rca.n_bio_res++;
    print_str("[RCA] Bio res "); print_int(rca.n_bio_res - 1);
    print_str(" type="); print_int(bt_type);
    print_str(" cat="); print_int(cat);
    print_str(" frs="); print_int(frs);
    print_str(" grs="); print_int(grs);
    print_str(" wla="); print_int(wla);
    print_str(" prs="); print_int(prs); print_str("\n");
    return rca.n_bio_res - 1;
}

void rca_science_report(void) {
    print_str("[RCA] Resource science report:\n");
    print_str("  Science categories: "); print_int(rca.n_res_sci); print_str("\n");
    print_str("  Total resource theory: "); print_int(rca.total_res_the); print_str("\n");
}

void rca_mineral_report(void) {
    print_str("[RCA] Mineral report:\n");
    print_str("  Mineral categories: "); print_int(rca.n_min_res); print_str("\n");
    print_str("  Total metal mining: "); print_int(rca.total_met_min); print_str("\n");
}

void rca_full_report(void) {
    print_str("[RCA] Full report:\n");
    print_str("  Land categories: "); print_int(rca.n_lan_res); print_str("\n");
    print_str("  Total land evaluation: "); print_int(rca.total_lan_eva); print_str("\n");
    print_str("  Water categories: "); print_int(rca.n_wat_res); print_str("\n");
    print_str("  Total surface water: "); print_int(rca.total_sur_wat); print_str("\n");
    print_str("  Bio categories: "); print_int(rca.n_bio_res); print_str("\n");
    print_str("  Total forest resources: "); print_int(rca.total_for_res); print_str("\n");
}

void rca_print_state(void) {
    print_str("[RCA] Rs="); print_int(rca.n_res_sci);
    print_str(" Mi="); print_int(rca.n_min_res);
    print_str(" La="); print_int(rca.n_lan_res);
    print_str(" Wa="); print_int(rca.n_wat_res);
    print_str(" Bi="); print_int(rca.n_bio_res);
    print_str("\n");
}

int main(void) {
    print_str("=== Resources Admin Demo ===\n\n");
    rca_init();

    print_str("Resource science...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int rth = 55 + (i * 13);
        int rev = 40 + (i * 10);
        int rpl = 22 + (i * 5);
        int rma = 15 + (i * 3);
        int year = 2020 + (i % 5);
        rca_res_sci(type, cat, rth, rev, rpl, rma, year);
    }

    print_str("\nMineral...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int mmi = 48 + (i * 11);
        int nmi = 35 + (i * 8);
        int emi = 20 + (i * 4);
        int wmi = 12 + (i * 3);
        int year = 2021 + (i % 4);
        rca_min_res(type, cat, mmi, nmi, emi, wmi, year);
    }

    print_str("\nLand...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int lev = 42 + (i * 10);
        int lpl = 28 + (i * 7);
        int lus = 18 + (i * 4);
        int lpr = 10 + (i * 2);
        int year = 2022 + (i % 3);
        rca_lan_res(type, cat, lev, lpl, lus, lpr, year);
    }

    print_str("\nWater...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int swa = 35 + (i * 8);
        int gwa = 25 + (i * 6);
        int wev = 15 + (i * 3);
        int wpr = 10 + (i * 2);
        int year = 2023 + (i % 2);
        rca_wat_res(type, cat, swa, gwa, wev, wpr, year);
    }

    print_str("\nBio...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int frs = 30 + (i * 7);
        int grs = 22 + (i * 5);
        int wla = 12 + (i * 3);
        int prs = 8 + (i * 2);
        int year = 2024;
        rca_bio_res(type, cat, frs, grs, wla, prs, year);
    }

    print_str("\nScience report...\n");
    rca_science_report();

    print_str("\nMineral report...\n");
    rca_mineral_report();

    print_str("\nFull report...\n");
    rca_full_report();

    print_str("\nFinal state...\n");
    rca_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
