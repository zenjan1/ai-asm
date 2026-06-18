/* mining_engineering_admin: Mining engineering administration system (v1.0)
 * Mining, mineral processing, mine safety, mine electromechanical, mine environmental
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

#define MAX_MINING      16
#define MAX_MIN_PR      14
#define MAX_MIN_SF      12
#define MAX_MIN_EM      10
#define MAX_MIN_EP      10

typedef struct {
    int    mi_id;
    int    mi_type;
    int    mi_category;
    int    open_pit;
    int    undergnd;
    int    solution;
    int    year;
    int    active;
} mining_t;

typedef struct {
    int    mp_id;
    int    mp_type;
    int    mp_category;
    int    crushing;
    int    grinding;
    int    flotation;
    int    year;
    int    active;
} min_pr_t;

typedef struct {
    int    sf_id;
    int    sf_type;
    int    sf_category;
    int    ventil;
    int    gas_ctrl;
    int    dust_ctrl;
    int    year;
    int    active;
} min_sf_t;

typedef struct {
    int    em_id;
    int    em_type;
    int    em_category;
    int    min_mach;
    int    hoist_tr;
    int    drain_eq;
    int    year;
    int    active;
} min_em_t;

typedef struct {
    int    ep_id;
    int    ep_type;
    int    ep_category;
    int    land_rec;
    int    water_tr;
    int    gas_tr;
    int    year;
    int    active;
} min_ep_t;

typedef struct {
    int    n_mining;
    int    n_min_pr;
    int    n_min_sf;
    int    n_min_em;
    int    n_min_ep;
    int    total_open_pit;
    int    total_crushing;
    int    total_ventil;
    int    total_min_mach;
    int    total_land_rec;
} mea_state_t;

static mining_t minings[MAX_MINING];
static min_pr_t min_prs[MAX_MIN_PR];
static min_sf_t min_sfs[MAX_MIN_SF];
static min_em_t min_ems[MAX_MIN_EM];
static min_ep_t min_eps[MAX_MIN_EP];
static mea_state_t mea;

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

int mea_init(void) {
    if (initialized) return -1;
    mea.n_mining = 0; mea.n_min_pr = 0; mea.n_min_sf = 0;
    mea.n_min_em = 0; mea.n_min_ep = 0;
    mea.total_open_pit = 0; mea.total_crushing = 0;
    mea.total_ventil = 0; mea.total_min_mach = 0;
    mea.total_land_rec = 0;
    for (int i = 0; i < MAX_MINING; i++) minings[i].active = 0;
    for (int i = 0; i < MAX_MIN_PR; i++) min_prs[i].active = 0;
    for (int i = 0; i < MAX_MIN_SF; i++) min_sfs[i].active = 0;
    for (int i = 0; i < MAX_MIN_EM; i++) min_ems[i].active = 0;
    for (int i = 0; i < MAX_MIN_EP; i++) min_eps[i].active = 0;
    initialized = 1;
    print_str("[MEA] Mining engineering initialized\n");
    return 0;
}

int mea_mining(int mt_type, int cat, int opn, int und, int sol, int year) {
    if (mea.n_mining >= MAX_MINING) return -1;
    mining_t* m = &minings[mea.n_mining];
    m->mi_id = mea.n_mining;
    m->mi_type = mt_type;
    m->mi_category = cat;
    m->open_pit = opn;
    m->undergnd = und;
    m->solution = sol;
    m->year = year;
    m->active = 1;
    mea.total_open_pit += opn;
    mea.n_mining++;
    print_str("[MEA] Mining "); print_int(mea.n_mining - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" opn="); print_int(opn);
    print_str(" und="); print_int(und);
    print_str(" sol="); print_int(sol); print_str("\n");
    return mea.n_mining - 1;
}

int mea_min_pr(int pt_type, int cat, int crs, int grd, int flt, int year) {
    if (mea.n_min_pr >= MAX_MIN_PR) return -1;
    min_pr_t* p = &min_prs[mea.n_min_pr];
    p->mp_id = mea.n_min_pr;
    p->mp_type = pt_type;
    p->mp_category = cat;
    p->crushing = crs;
    p->grinding = grd;
    p->flotation = flt;
    p->year = year;
    p->active = 1;
    mea.total_crushing += crs;
    mea.n_min_pr++;
    print_str("[MEA] Min pr "); print_int(mea.n_min_pr - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" crs="); print_int(crs);
    print_str(" grd="); print_int(grd);
    print_str(" flt="); print_int(flt); print_str("\n");
    return mea.n_min_pr - 1;
}

int mea_min_sf(int st_type, int cat, int ven, int gsc, int dtc, int year) {
    if (mea.n_min_sf >= MAX_MIN_SF) return -1;
    min_sf_t* s = &min_sfs[mea.n_min_sf];
    s->sf_id = mea.n_min_sf;
    s->sf_type = st_type;
    s->sf_category = cat;
    s->ventil = ven;
    s->gas_ctrl = gsc;
    s->dust_ctrl = dtc;
    s->year = year;
    s->active = 1;
    mea.total_ventil += ven;
    mea.n_min_sf++;
    print_str("[MEA] Min sf "); print_int(mea.n_min_sf - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" ven="); print_int(ven);
    print_str(" gsc="); print_int(gsc);
    print_str(" dtc="); print_int(dtc); print_str("\n");
    return mea.n_min_sf - 1;
}

int mea_min_em(int et_type, int cat, int mmc, int hst, int drn, int year) {
    if (mea.n_min_em >= MAX_MIN_EM) return -1;
    min_em_t* e = &min_ems[mea.n_min_em];
    e->em_id = mea.n_min_em;
    e->em_type = et_type;
    e->em_category = cat;
    e->min_mach = mmc;
    e->hoist_tr = hst;
    e->drain_eq = drn;
    e->year = year;
    e->active = 1;
    mea.total_min_mach += mmc;
    mea.n_min_em++;
    print_str("[MEA] Min em "); print_int(mea.n_min_em - 1);
    print_str(" type="); print_int(et_type);
    print_str(" cat="); print_int(cat);
    print_str(" mmc="); print_int(mmc);
    print_str(" hst="); print_int(hst);
    print_str(" drn="); print_int(drn); print_str("\n");
    return mea.n_min_em - 1;
}

int mea_min_ep(int et_type, int cat, int ldr, int wtr, int gtr, int year) {
    if (mea.n_min_ep >= MAX_MIN_EP) return -1;
    min_ep_t* e = &min_eps[mea.n_min_ep];
    e->ep_id = mea.n_min_ep;
    e->ep_type = et_type;
    e->ep_category = cat;
    e->land_rec = ldr;
    e->water_tr = wtr;
    e->gas_tr = gtr;
    e->year = year;
    e->active = 1;
    mea.total_land_rec += ldr;
    mea.n_min_ep++;
    print_str("[MEA] Min ep "); print_int(mea.n_min_ep - 1);
    print_str(" type="); print_int(et_type);
    print_str(" cat="); print_int(cat);
    print_str(" ldr="); print_int(ldr);
    print_str(" wtr="); print_int(wtr);
    print_str(" gtr="); print_int(gtr); print_str("\n");
    return mea.n_min_ep - 1;
}

void mea_mining_report(void) {
    print_str("[MEA] Mining report:\n");
    print_str("  Mining categories: "); print_int(mea.n_mining); print_str("\n");
    print_str("  Total open pit: "); print_int(mea.total_open_pit); print_str("\n");
}

void mea_min_pr_report(void) {
    print_str("[MEA] Mineral processing report:\n");
    print_str("  Mineral processing categories: "); print_int(mea.n_min_pr); print_str("\n");
    print_str("  Total crushing: "); print_int(mea.total_crushing); print_str("\n");
}

void mea_full_report(void) {
    print_str("[MEA] Full report:\n");
    print_str("  Mine safety categories: "); print_int(mea.n_min_sf); print_str("\n");
    print_str("  Total ventilation: "); print_int(mea.total_ventil); print_str("\n");
    print_str("  Mine electromechanical categories: "); print_int(mea.n_min_em); print_str("\n");
    print_str("  Total mining machinery: "); print_int(mea.total_min_mach); print_str("\n");
    print_str("  Mine environmental categories: "); print_int(mea.n_min_ep); print_str("\n");
    print_str("  Total land reclamation: "); print_int(mea.total_land_rec); print_str("\n");
}

void mea_print_state(void) {
    print_str("[MEA] Mi="); print_int(mea.n_mining);
    print_str(" Mp="); print_int(mea.n_min_pr);
    print_str(" Sf="); print_int(mea.n_min_sf);
    print_str(" Em="); print_int(mea.n_min_em);
    print_str(" Ep="); print_int(mea.n_min_ep);
    print_str("\n");
}

int main(void) {
    print_str("=== Mining Engineering Admin Demo ===\n\n");
    mea_init();

    print_str("Mining...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int opn = 55 + (i * 13);
        int und = 40 + (i * 10);
        int sol = 22 + (i * 5);
        int year = 2020 + (i % 5);
        mea_mining(type, cat, opn, und, sol, year);
    }

    print_str("\nMineral processing...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int crs = 48 + (i * 11);
        int grd = 35 + (i * 8);
        int flt = 20 + (i * 4);
        int year = 2021 + (i % 4);
        mea_min_pr(type, cat, crs, grd, flt, year);
    }

    print_str("\nMine safety...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ven = 42 + (i * 10);
        int gsc = 28 + (i * 7);
        int dtc = 18 + (i * 4);
        int year = 2022 + (i % 3);
        mea_min_sf(type, cat, ven, gsc, dtc, year);
    }

    print_str("\nMine electromechanical...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int mmc = 35 + (i * 8);
        int hst = 25 + (i * 6);
        int drn = 15 + (i * 3);
        int year = 2023 + (i % 2);
        mea_min_em(type, cat, mmc, hst, drn, year);
    }

    print_str("\nMine environmental...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ldr = 30 + (i * 7);
        int wtr = 22 + (i * 5);
        int gtr = 12 + (i * 3);
        int year = 2024;
        mea_min_ep(type, cat, ldr, wtr, gtr, year);
    }

    print_str("\nMining report...\n");
    mea_mining_report();

    print_str("\nMineral processing report...\n");
    mea_min_pr_report();

    print_str("\nFull report...\n");
    mea_full_report();

    print_str("\nFinal state...\n");
    mea_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
