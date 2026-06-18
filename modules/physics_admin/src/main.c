/* physics_admin: Physics administration system (v1.0)
 * Theoretical physics, particle physics, condensed matter, optics, astrophysics
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

#define MAX_THEO_P      16
#define MAX_PART_P      14
#define MAX_COND_P      12
#define MAX_OPTICS      10
#define MAX_ASTRO_P     10

typedef struct {
    int    tp_id;
    int    tp_type;
    int    tp_category;
    int    cls_mec;
    int    quan_mec;
    int    sta_phy;
    int    ele_dyn;
    int    year;
    int    active;
} theo_p_t;

typedef struct {
    int    pp_id;
    int    pp_type;
    int    pp_category;
    int    bas_par;
    int    qua_fie;
    int    sta_mod;
    int    hig_phy;
    int    year;
    int    active;
} part_p_t;

typedef struct {
    int    cp_id;
    int    cp_type;
    int    cp_category;
    int    sol_phy;
    int    semic;
    int    super_c;
    int    mag_mat;
    int    year;
    int    active;
} cond_p_t;

typedef struct {
    int    op_id;
    int    op_type;
    int    op_category;
    int    geo_opt;
    int    phy_opt;
    int    qua_opt;
    int    las_phy;
    int    year;
    int    active;
} optics_t;

typedef struct {
    int    ap_id;
    int    ap_type;
    int    ap_category;
    int    sta_phy_a;
    int    gal_phy;
    int    cosm;
    int    gra_phy;
    int    year;
    int    active;
} astro_p_t;

typedef struct {
    int    n_theo_p;
    int    n_part_p;
    int    n_cond_p;
    int    n_optics;
    int    n_astro_p;
    int    total_cls_mec;
    int    total_bas_par;
    int    total_sol_phy;
    int    total_geo_opt;
    int    total_sta_phy_a;
} pya_state_t;

static theo_p_t theo_ps[MAX_THEO_P];
static part_p_t part_ps[MAX_PART_P];
static cond_p_t cond_ps[MAX_COND_P];
static optics_t opticss[MAX_OPTICS];
static astro_p_t astro_ps[MAX_ASTRO_P];
static pya_state_t pya;

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

int pya_init(void) {
    if (initialized) return -1;
    pya.n_theo_p = 0; pya.n_part_p = 0; pya.n_cond_p = 0;
    pya.n_optics = 0; pya.n_astro_p = 0;
    pya.total_cls_mec = 0; pya.total_bas_par = 0;
    pya.total_sol_phy = 0; pya.total_geo_opt = 0;
    pya.total_sta_phy_a = 0;
    for (int i = 0; i < MAX_THEO_P; i++) theo_ps[i].active = 0;
    for (int i = 0; i < MAX_PART_P; i++) part_ps[i].active = 0;
    for (int i = 0; i < MAX_COND_P; i++) cond_ps[i].active = 0;
    for (int i = 0; i < MAX_OPTICS; i++) opticss[i].active = 0;
    for (int i = 0; i < MAX_ASTRO_P; i++) astro_ps[i].active = 0;
    initialized = 1;
    print_str("[PYA] Physics initialized\n");
    return 0;
}

int pya_theo_p(int tt_type, int cat, int clm, int qum, int stp, int eld, int year) {
    if (pya.n_theo_p >= MAX_THEO_P) return -1;
    theo_p_t* t = &theo_ps[pya.n_theo_p];
    t->tp_id = pya.n_theo_p;
    t->tp_type = tt_type;
    t->tp_category = cat;
    t->cls_mec = clm;
    t->quan_mec = qum;
    t->sta_phy = stp;
    t->ele_dyn = eld;
    t->year = year;
    t->active = 1;
    pya.total_cls_mec += clm;
    pya.n_theo_p++;
    print_str("[PYA] Theo p "); print_int(pya.n_theo_p - 1);
    print_str(" type="); print_int(tt_type);
    print_str(" cat="); print_int(cat);
    print_str(" clm="); print_int(clm);
    print_str(" qum="); print_int(qum);
    print_str(" stp="); print_int(stp);
    print_str(" eld="); print_int(eld); print_str("\n");
    return pya.n_theo_p - 1;
}

int pya_part_p(int pt_type, int cat, int bpr, int qft, int smd, int hph, int year) {
    if (pya.n_part_p >= MAX_PART_P) return -1;
    part_p_t* p = &part_ps[pya.n_part_p];
    p->pp_id = pya.n_part_p;
    p->pp_type = pt_type;
    p->pp_category = cat;
    p->bas_par = bpr;
    p->qua_fie = qft;
    p->sta_mod = smd;
    p->hig_phy = hph;
    p->year = year;
    p->active = 1;
    pya.total_bas_par += bpr;
    pya.n_part_p++;
    print_str("[PYA] Part p "); print_int(pya.n_part_p - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" bpr="); print_int(bpr);
    print_str(" qft="); print_int(qft);
    print_str(" smd="); print_int(smd);
    print_str(" hph="); print_int(hph); print_str("\n");
    return pya.n_part_p - 1;
}

int pya_cond_p(int ct_type, int cat, int spl, int smc, int spr, int mgt, int year) {
    if (pya.n_cond_p >= MAX_COND_P) return -1;
    cond_p_t* c = &cond_ps[pya.n_cond_p];
    c->cp_id = pya.n_cond_p;
    c->cp_type = ct_type;
    c->cp_category = cat;
    c->sol_phy = spl;
    c->semic = smc;
    c->super_c = spr;
    c->mag_mat = mgt;
    c->year = year;
    c->active = 1;
    pya.total_sol_phy += spl;
    pya.n_cond_p++;
    print_str("[PYA] Cond p "); print_int(pya.n_cond_p - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" spl="); print_int(spl);
    print_str(" smc="); print_int(smc);
    print_str(" spr="); print_int(spr);
    print_str(" mgt="); print_int(mgt); print_str("\n");
    return pya.n_cond_p - 1;
}

int pya_optics(int ot_type, int cat, int gop, int pop, int qop, int lsp, int year) {
    if (pya.n_optics >= MAX_OPTICS) return -1;
    optics_t* o = &opticss[pya.n_optics];
    o->op_id = pya.n_optics;
    o->op_type = ot_type;
    o->op_category = cat;
    o->geo_opt = gop;
    o->phy_opt = pop;
    o->qua_opt = qop;
    o->las_phy = lsp;
    o->year = year;
    o->active = 1;
    pya.total_geo_opt += gop;
    pya.n_optics++;
    print_str("[PYA] Optics "); print_int(pya.n_optics - 1);
    print_str(" type="); print_int(ot_type);
    print_str(" cat="); print_int(cat);
    print_str(" gop="); print_int(gop);
    print_str(" pop="); print_int(pop);
    print_str(" qop="); print_int(qop);
    print_str(" lsp="); print_int(lsp); print_str("\n");
    return pya.n_optics - 1;
}

int pya_astro_p(int at_type, int cat, int spr, int gph, int csm, int grp, int year) {
    if (pya.n_astro_p >= MAX_ASTRO_P) return -1;
    astro_p_t* a = &astro_ps[pya.n_astro_p];
    a->ap_id = pya.n_astro_p;
    a->ap_type = at_type;
    a->ap_category = cat;
    a->sta_phy_a = spr;
    a->gal_phy = gph;
    a->cosm = csm;
    a->gra_phy = grp;
    a->year = year;
    a->active = 1;
    pya.total_sta_phy_a += spr;
    pya.n_astro_p++;
    print_str("[PYA] Astro p "); print_int(pya.n_astro_p - 1);
    print_str(" type="); print_int(at_type);
    print_str(" cat="); print_int(cat);
    print_str(" spr="); print_int(spr);
    print_str(" gph="); print_int(gph);
    print_str(" csm="); print_int(csm);
    print_str(" grp="); print_int(grp); print_str("\n");
    return pya.n_astro_p - 1;
}

void pya_theo_report(void) {
    print_str("[PYA] Theoretical physics report:\n");
    print_str("  Theory categories: "); print_int(pya.n_theo_p); print_str("\n");
    print_str("  Total classical mechanics: "); print_int(pya.total_cls_mec); print_str("\n");
}

void pya_part_report(void) {
    print_str("[PYA] Particle physics report:\n");
    print_str("  Particle categories: "); print_int(pya.n_part_p); print_str("\n");
    print_str("  Total basic particles: "); print_int(pya.total_bas_par); print_str("\n");
}

void pya_full_report(void) {
    print_str("[PYA] Full report:\n");
    print_str("  Condensed matter categories: "); print_int(pya.n_cond_p); print_str("\n");
    print_str("  Total solid physics: "); print_int(pya.total_sol_phy); print_str("\n");
    print_str("  Optics categories: "); print_int(pya.n_optics); print_str("\n");
    print_str("  Total geometric optics: "); print_int(pya.total_geo_opt); print_str("\n");
    print_str("  Astrophysics categories: "); print_int(pya.n_astro_p); print_str("\n");
    print_str("  Total stellar physics: "); print_int(pya.total_sta_phy_a); print_str("\n");
}

void pya_print_state(void) {
    print_str("[PYA] Tp="); print_int(pya.n_theo_p);
    print_str(" Pp="); print_int(pya.n_part_p);
    print_str(" Cp="); print_int(pya.n_cond_p);
    print_str(" Op="); print_int(pya.n_optics);
    print_str(" Ap="); print_int(pya.n_astro_p);
    print_str("\n");
}

int main(void) {
    print_str("=== Physics Admin Demo ===\n\n");
    pya_init();

    print_str("Theoretical physics...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int clm = 55 + (i * 13);
        int qum = 40 + (i * 10);
        int stp = 22 + (i * 5);
        int eld = 15 + (i * 3);
        int year = 2020 + (i % 5);
        pya_theo_p(type, cat, clm, qum, stp, eld, year);
    }

    print_str("\nParticle physics...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int bpr = 48 + (i * 11);
        int qft = 35 + (i * 8);
        int smd = 20 + (i * 4);
        int hph = 12 + (i * 3);
        int year = 2021 + (i % 4);
        pya_part_p(type, cat, bpr, qft, smd, hph, year);
    }

    print_str("\nCondensed matter...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int spl = 42 + (i * 10);
        int smc = 28 + (i * 7);
        int spr = 18 + (i * 4);
        int mgt = 10 + (i * 2);
        int year = 2022 + (i % 3);
        pya_cond_p(type, cat, spl, smc, spr, mgt, year);
    }

    print_str("\nOptics...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int gop = 35 + (i * 8);
        int pop = 25 + (i * 6);
        int qop = 15 + (i * 3);
        int lsp = 10 + (i * 2);
        int year = 2023 + (i % 2);
        pya_optics(type, cat, gop, pop, qop, lsp, year);
    }

    print_str("\nAstrophysics...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int spr = 30 + (i * 7);
        int gph = 22 + (i * 5);
        int csm = 12 + (i * 3);
        int grp = 8 + (i * 2);
        int year = 2024;
        pya_astro_p(type, cat, spr, gph, csm, grp, year);
    }

    print_str("\nTheory report...\n");
    pya_theo_report();

    print_str("\nParticle report...\n");
    pya_part_report();

    print_str("\nFull report...\n");
    pya_full_report();

    print_str("\nFinal state...\n");
    pya_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
