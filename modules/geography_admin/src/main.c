/* geography_admin: Geography administration system (v1.0)
 * Physical geography, human geography, GIS, regional geography, cartography
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

#define MAX_PHYS_G      16
#define MAX_HUMAN_G     14
#define MAX_GIS_S       12
#define MAX_REG_G       10
#define MAX_CART_S      10

typedef struct {
    int    pg_id;
    int    pg_type;
    int    pg_category;
    int    geo_mor;
    int    climate;
    int    hydro;
    int    soil_g;
    int    bio_g;
    int    year;
    int    active;
} phys_g_t;

typedef struct {
    int    hg_id;
    int    hg_type;
    int    hg_category;
    int    urban_g;
    int    econ_g;
    int    pop_g;
    int    cult_g;
    int    polit_g;
    int    year;
    int    active;
} human_g_t;

typedef struct {
    int    gi_id;
    int    gi_type;
    int    gi_category;
    int    gis_pri;
    int    spa_ana;
    int    remote;
    int    mapping;
    int    year;
    int    active;
} gis_s_t;

typedef struct {
    int    rg_id;
    int    rg_type;
    int    rg_category;
    int    chi_reg;
    int    wor_reg;
    int    reg_plan;
    int    reg_dev;
    int    year;
    int    active;
} reg_g_t;

typedef struct {
    int    ct_id;
    int    ct_type;
    int    ct_category;
    int    map_proj;
    int    map_des;
    int    dig_map;
    int    map_vis;
    int    year;
    int    active;
} cart_s_t;

typedef struct {
    int    n_phys_g;
    int    n_human_g;
    int    n_gis_s;
    int    n_reg_g;
    int    n_cart_s;
    int    total_geo_mor;
    int    total_urban_g;
    int    total_gis_pri;
    int    total_chi_reg;
    int    total_map_proj;
} ga_state_t;

static phys_g_t phys_gs[MAX_PHYS_G];
static human_g_t human_gs[MAX_HUMAN_G];
static gis_s_t gis_ss[MAX_GIS_S];
static reg_g_t reg_gs[MAX_REG_G];
static cart_s_t cart_ss[MAX_CART_S];
static ga_state_t ga;

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

int ga_init(void) {
    if (initialized) return -1;
    ga.n_phys_g = 0; ga.n_human_g = 0; ga.n_gis_s = 0;
    ga.n_reg_g = 0; ga.n_cart_s = 0;
    ga.total_geo_mor = 0; ga.total_urban_g = 0;
    ga.total_gis_pri = 0; ga.total_chi_reg = 0;
    ga.total_map_proj = 0;
    for (int i = 0; i < MAX_PHYS_G; i++) phys_gs[i].active = 0;
    for (int i = 0; i < MAX_HUMAN_G; i++) human_gs[i].active = 0;
    for (int i = 0; i < MAX_GIS_S; i++) gis_ss[i].active = 0;
    for (int i = 0; i < MAX_REG_G; i++) reg_gs[i].active = 0;
    for (int i = 0; i < MAX_CART_S; i++) cart_ss[i].active = 0;
    initialized = 1;
    print_str("[GA] Geography initialized\n");
    return 0;
}

int ga_phys_g(int pt_type, int cat, int gmr, int clm, int hdr, int slg, int bgr, int year) {
    if (ga.n_phys_g >= MAX_PHYS_G) return -1;
    phys_g_t* p = &phys_gs[ga.n_phys_g];
    p->pg_id = ga.n_phys_g;
    p->pg_type = pt_type;
    p->pg_category = cat;
    p->geo_mor = gmr;
    p->climate = clm;
    p->hydro = hdr;
    p->soil_g = slg;
    p->bio_g = bgr;
    p->year = year;
    p->active = 1;
    ga.total_geo_mor += gmr;
    ga.n_phys_g++;
    print_str("[GA] Phys g "); print_int(ga.n_phys_g - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" gmr="); print_int(gmr);
    print_str(" clm="); print_int(clm);
    print_str(" hdr="); print_int(hdr);
    print_str(" slg="); print_int(slg);
    print_str(" bgr="); print_int(bgr); print_str("\n");
    return ga.n_phys_g - 1;
}

int ga_human_g(int ht_type, int cat, int ubg, int ecg, int pog, int cug, int ptg, int year) {
    if (ga.n_human_g >= MAX_HUMAN_G) return -1;
    human_g_t* h = &human_gs[ga.n_human_g];
    h->hg_id = ga.n_human_g;
    h->hg_type = ht_type;
    h->hg_category = cat;
    h->urban_g = ubg;
    h->econ_g = ecg;
    h->pop_g = pog;
    h->cult_g = cug;
    h->polit_g = ptg;
    h->year = year;
    h->active = 1;
    ga.total_urban_g += ubg;
    ga.n_human_g++;
    print_str("[GA] Human g "); print_int(ga.n_human_g - 1);
    print_str(" type="); print_int(ht_type);
    print_str(" cat="); print_int(cat);
    print_str(" ubg="); print_int(ubg);
    print_str(" ecg="); print_int(ecg);
    print_str(" pog="); print_int(pog);
    print_str(" cug="); print_int(cug);
    print_str(" ptg="); print_int(ptg); print_str("\n");
    return ga.n_human_g - 1;
}

int ga_gis_s(int gt_type, int cat, int gsp, int spa, int rmt, int mpp, int year) {
    if (ga.n_gis_s >= MAX_GIS_S) return -1;
    gis_s_t* g = &gis_ss[ga.n_gis_s];
    g->gi_id = ga.n_gis_s;
    g->gi_type = gt_type;
    g->gi_category = cat;
    g->gis_pri = gsp;
    g->spa_ana = spa;
    g->remote = rmt;
    g->mapping = mpp;
    g->year = year;
    g->active = 1;
    ga.total_gis_pri += gsp;
    ga.n_gis_s++;
    print_str("[GA] Gis "); print_int(ga.n_gis_s - 1);
    print_str(" type="); print_int(gt_type);
    print_str(" cat="); print_int(cat);
    print_str(" gsp="); print_int(gsp);
    print_str(" spa="); print_int(spa);
    print_str(" rmt="); print_int(rmt);
    print_str(" mpp="); print_int(mpp); print_str("\n");
    return ga.n_gis_s - 1;
}

int ga_reg_g(int rt_type, int cat, int chr, int wrr, int rpl, int rdv, int year) {
    if (ga.n_reg_g >= MAX_REG_G) return -1;
    reg_g_t* r = &reg_gs[ga.n_reg_g];
    r->rg_id = ga.n_reg_g;
    r->rg_type = rt_type;
    r->rg_category = cat;
    r->chi_reg = chr;
    r->wor_reg = wrr;
    r->reg_plan = rpl;
    r->reg_dev = rdv;
    r->year = year;
    r->active = 1;
    ga.total_chi_reg += chr;
    ga.n_reg_g++;
    print_str("[GA] Reg g "); print_int(ga.n_reg_g - 1);
    print_str(" type="); print_int(rt_type);
    print_str(" cat="); print_int(cat);
    print_str(" chr="); print_int(chr);
    print_str(" wrr="); print_int(wrr);
    print_str(" rpl="); print_int(rpl);
    print_str(" rdv="); print_int(rdv); print_str("\n");
    return ga.n_reg_g - 1;
}

int ga_cart_s(int ct_type, int cat, int mpr, int mds, int dgm, int mvs, int year) {
    if (ga.n_cart_s >= MAX_CART_S) return -1;
    cart_s_t* c = &cart_ss[ga.n_cart_s];
    c->ct_id = ga.n_cart_s;
    c->ct_type = ct_type;
    c->ct_category = cat;
    c->map_proj = mpr;
    c->map_des = mds;
    c->dig_map = dgm;
    c->map_vis = mvs;
    c->year = year;
    c->active = 1;
    ga.total_map_proj += mpr;
    ga.n_cart_s++;
    print_str("[GA] Cart "); print_int(ga.n_cart_s - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" mpr="); print_int(mpr);
    print_str(" mds="); print_int(mds);
    print_str(" dgm="); print_int(dgm);
    print_str(" mvs="); print_int(mvs); print_str("\n");
    return ga.n_cart_s - 1;
}

void ga_phys_report(void) {
    print_str("[GA] Physical geography report:\n");
    print_str("  Physical categories: "); print_int(ga.n_phys_g); print_str("\n");
    print_str("  Total geomorphology: "); print_int(ga.total_geo_mor); print_str("\n");
}

void ga_human_report(void) {
    print_str("[GA] Human geography report:\n");
    print_str("  Human categories: "); print_int(ga.n_human_g); print_str("\n");
    print_str("  Total urban geography: "); print_int(ga.total_urban_g); print_str("\n");
}

void ga_full_report(void) {
    print_str("[GA] Full report:\n");
    print_str("  GIS categories: "); print_int(ga.n_gis_s); print_str("\n");
    print_str("  Total GIS principles: "); print_int(ga.total_gis_pri); print_str("\n");
    print_str("  Regional categories: "); print_int(ga.n_reg_g); print_str("\n");
    print_str("  Total China regional: "); print_int(ga.total_chi_reg); print_str("\n");
    print_str("  Cartography categories: "); print_int(ga.n_cart_s); print_str("\n");
    print_str("  Total map projection: "); print_int(ga.total_map_proj); print_str("\n");
}

void ga_print_state(void) {
    print_str("[GA] Pg="); print_int(ga.n_phys_g);
    print_str(" Hg="); print_int(ga.n_human_g);
    print_str(" Gs="); print_int(ga.n_gis_s);
    print_str(" Rg="); print_int(ga.n_reg_g);
    print_str(" Cs="); print_int(ga.n_cart_s);
    print_str("\n");
}

int main(void) {
    print_str("=== Geography Admin Demo ===\n\n");
    ga_init();

    print_str("Physical geography...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int gmr = 55 + (i * 13);
        int clm = 40 + (i * 10);
        int hdr = 22 + (i * 5);
        int slg = 15 + (i * 3);
        int bgr = 10 + (i * 2);
        int year = 2020 + (i % 5);
        ga_phys_g(type, cat, gmr, clm, hdr, slg, bgr, year);
    }

    print_str("\nHuman geography...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ubg = 48 + (i * 11);
        int ecg = 35 + (i * 8);
        int pog = 20 + (i * 4);
        int cug = 12 + (i * 3);
        int ptg = 8 + (i * 2);
        int year = 2021 + (i % 4);
        ga_human_g(type, cat, ubg, ecg, pog, cug, ptg, year);
    }

    print_str("\nGIS...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int gsp = 42 + (i * 10);
        int spa = 28 + (i * 7);
        int rmt = 18 + (i * 4);
        int mpp = 10 + (i * 2);
        int year = 2022 + (i % 3);
        ga_gis_s(type, cat, gsp, spa, rmt, mpp, year);
    }

    print_str("\nRegional geography...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int chr = 35 + (i * 8);
        int wrr = 25 + (i * 6);
        int rpl = 15 + (i * 3);
        int rdv = 10 + (i * 2);
        int year = 2023 + (i % 2);
        ga_reg_g(type, cat, chr, wrr, rpl, rdv, year);
    }

    print_str("\nCartography...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int mpr = 30 + (i * 7);
        int mds = 22 + (i * 5);
        int dgm = 12 + (i * 3);
        int mvs = 8 + (i * 2);
        int year = 2024;
        ga_cart_s(type, cat, mpr, mds, dgm, mvs, year);
    }

    print_str("\nPhysical report...\n");
    ga_phys_report();

    print_str("\nHuman report...\n");
    ga_human_report();

    print_str("\nFull report...\n");
    ga_full_report();

    print_str("\nFinal state...\n");
    ga_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
