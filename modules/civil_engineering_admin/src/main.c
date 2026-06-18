/* civil_engineering_admin: Civil engineering administration system (v1.0)
 * Structural, geotechnical, bridge, tunnel, road engineering
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

#define MAX_STRUCT_EN   16
#define MAX_GEOT_EN     14
#define MAX_BRIDGE_EN   12
#define MAX_TUNNEL_EN   10
#define MAX_ROAD_EN     10

typedef struct {
    int    se_id;
    int    se_type;
    int    se_category;
    int    concrete_st;
    int    steel_st;
    int    masonry_st;
    int    year;
    int    active;
} struct_en_t;

typedef struct {
    int    ge_id;
    int    ge_type;
    int    ge_category;
    int    found_tr;
    int    found_eng;
    int    slope_eng;
    int    year;
    int    active;
} geot_en_t;

typedef struct {
    int    br_id;
    int    br_type;
    int    br_category;
    int    beam_br;
    int    arch_br;
    int    cable_br;
    int    year;
    int    active;
} bridge_en_t;

typedef struct {
    int    tu_id;
    int    tu_type;
    int    tu_category;
    int    mtn_tunnel;
    int    under_water;
    int    city_tunnel;
    int    year;
    int    active;
} tunnel_en_t;

typedef struct {
    int    rd_id;
    int    rd_type;
    int    rd_category;
    int    subgrade;
    int    pavement;
    int    road_ds;
    int    year;
    int    active;
} road_en_t;

typedef struct {
    int    n_struct_en;
    int    n_geot_en;
    int    n_bridge_en;
    int    n_tunnel_en;
    int    n_road_en;
    int    total_concrete_st;
    int    total_found_tr;
    int    total_beam_br;
    int    total_mtn_tunnel;
    int    total_subgrade;
} cva_state_t;

static struct_en_t struct_ens[MAX_STRUCT_EN];
static geot_en_t geot_ens[MAX_GEOT_EN];
static bridge_en_t bridge_ens[MAX_BRIDGE_EN];
static tunnel_en_t tunnel_ens[MAX_TUNNEL_EN];
static road_en_t road_ens[MAX_ROAD_EN];
static cva_state_t cva;

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

int cva_init(void) {
    if (initialized) return -1;
    cva.n_struct_en = 0; cva.n_geot_en = 0; cva.n_bridge_en = 0;
    cva.n_tunnel_en = 0; cva.n_road_en = 0;
    cva.total_concrete_st = 0; cva.total_found_tr = 0;
    cva.total_beam_br = 0; cva.total_mtn_tunnel = 0;
    cva.total_subgrade = 0;
    for (int i = 0; i < MAX_STRUCT_EN; i++) struct_ens[i].active = 0;
    for (int i = 0; i < MAX_GEOT_EN; i++) geot_ens[i].active = 0;
    for (int i = 0; i < MAX_BRIDGE_EN; i++) bridge_ens[i].active = 0;
    for (int i = 0; i < MAX_TUNNEL_EN; i++) tunnel_ens[i].active = 0;
    for (int i = 0; i < MAX_ROAD_EN; i++) road_ens[i].active = 0;
    initialized = 1;
    print_str("[CVA] Civil engineering initialized\n");
    return 0;
}

int cva_struct_en(int st_type, int cat, int cct, int stl, int mas, int year) {
    if (cva.n_struct_en >= MAX_STRUCT_EN) return -1;
    struct_en_t* s = &struct_ens[cva.n_struct_en];
    s->se_id = cva.n_struct_en;
    s->se_type = st_type;
    s->se_category = cat;
    s->concrete_st = cct;
    s->steel_st = stl;
    s->masonry_st = mas;
    s->year = year;
    s->active = 1;
    cva.total_concrete_st += cct;
    cva.n_struct_en++;
    print_str("[CVA] Struct en "); print_int(cva.n_struct_en - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" cct="); print_int(cct);
    print_str(" stl="); print_int(stl);
    print_str(" mas="); print_int(mas); print_str("\n");
    return cva.n_struct_en - 1;
}

int cva_geot_en(int gt_type, int cat, int fdt, int fde, int spe, int year) {
    if (cva.n_geot_en >= MAX_GEOT_EN) return -1;
    geot_en_t* g = &geot_ens[cva.n_geot_en];
    g->ge_id = cva.n_geot_en;
    g->ge_type = gt_type;
    g->ge_category = cat;
    g->found_tr = fdt;
    g->found_eng = fde;
    g->slope_eng = spe;
    g->year = year;
    g->active = 1;
    cva.total_found_tr += fdt;
    cva.n_geot_en++;
    print_str("[CVA] Geot en "); print_int(cva.n_geot_en - 1);
    print_str(" type="); print_int(gt_type);
    print_str(" cat="); print_int(cat);
    print_str(" fdt="); print_int(fdt);
    print_str(" fde="); print_int(fde);
    print_str(" spe="); print_int(spe); print_str("\n");
    return cva.n_geot_en - 1;
}

int cva_bridge_en(int bt_type, int cat, int bmb, int arb, int clb, int year) {
    if (cva.n_bridge_en >= MAX_BRIDGE_EN) return -1;
    bridge_en_t* b = &bridge_ens[cva.n_bridge_en];
    b->br_id = cva.n_bridge_en;
    b->br_type = bt_type;
    b->br_category = cat;
    b->beam_br = bmb;
    b->arch_br = arb;
    b->cable_br = clb;
    b->year = year;
    b->active = 1;
    cva.total_beam_br += bmb;
    cva.n_bridge_en++;
    print_str("[CVA] Bridge en "); print_int(cva.n_bridge_en - 1);
    print_str(" type="); print_int(bt_type);
    print_str(" cat="); print_int(cat);
    print_str(" bmb="); print_int(bmb);
    print_str(" arb="); print_int(arb);
    print_str(" clb="); print_int(clb); print_str("\n");
    return cva.n_bridge_en - 1;
}

int cva_tunnel_en(int tt_type, int cat, int mtt, int uwt, int ctt, int year) {
    if (cva.n_tunnel_en >= MAX_TUNNEL_EN) return -1;
    tunnel_en_t* t = &tunnel_ens[cva.n_tunnel_en];
    t->tu_id = cva.n_tunnel_en;
    t->tu_type = tt_type;
    t->tu_category = cat;
    t->mtn_tunnel = mtt;
    t->under_water = uwt;
    t->city_tunnel = ctt;
    t->year = year;
    t->active = 1;
    cva.total_mtn_tunnel += mtt;
    cva.n_tunnel_en++;
    print_str("[CVA] Tunnel en "); print_int(cva.n_tunnel_en - 1);
    print_str(" type="); print_int(tt_type);
    print_str(" cat="); print_int(cat);
    print_str(" mtt="); print_int(mtt);
    print_str(" uwt="); print_int(uwt);
    print_str(" ctt="); print_int(ctt); print_str("\n");
    return cva.n_tunnel_en - 1;
}

int cva_road_en(int rt_type, int cat, int sbg, int pvt, int rds, int year) {
    if (cva.n_road_en >= MAX_ROAD_EN) return -1;
    road_en_t* r = &road_ens[cva.n_road_en];
    r->rd_id = cva.n_road_en;
    r->rd_type = rt_type;
    r->rd_category = cat;
    r->subgrade = sbg;
    r->pavement = pvt;
    r->road_ds = rds;
    r->year = year;
    r->active = 1;
    cva.total_subgrade += sbg;
    cva.n_road_en++;
    print_str("[CVA] Road en "); print_int(cva.n_road_en - 1);
    print_str(" type="); print_int(rt_type);
    print_str(" cat="); print_int(cat);
    print_str(" sbg="); print_int(sbg);
    print_str(" pvt="); print_int(pvt);
    print_str(" rds="); print_int(rds); print_str("\n");
    return cva.n_road_en - 1;
}

void cva_struct_en_report(void) {
    print_str("[CVA] Structural engineering report:\n");
    print_str("  Structural engineering categories: "); print_int(cva.n_struct_en); print_str("\n");
    print_str("  Total concrete structures: "); print_int(cva.total_concrete_st); print_str("\n");
}

void cva_geot_en_report(void) {
    print_str("[CVA] Geotechnical engineering report:\n");
    print_str("  Geotechnical categories: "); print_int(cva.n_geot_en); print_str("\n");
    print_str("  Total foundation treatment: "); print_int(cva.total_found_tr); print_str("\n");
}

void cva_full_report(void) {
    print_str("[CVA] Full report:\n");
    print_str("  Bridge engineering categories: "); print_int(cva.n_bridge_en); print_str("\n");
    print_str("  Total beam bridges: "); print_int(cva.total_beam_br); print_str("\n");
    print_str("  Tunnel engineering categories: "); print_int(cva.n_tunnel_en); print_str("\n");
    print_str("  Total mountain tunnels: "); print_int(cva.total_mtn_tunnel); print_str("\n");
    print_str("  Road engineering categories: "); print_int(cva.n_road_en); print_str("\n");
    print_str("  Total subgrade: "); print_int(cva.total_subgrade); print_str("\n");
}

void cva_print_state(void) {
    print_str("[CVA] Se="); print_int(cva.n_struct_en);
    print_str(" Ge="); print_int(cva.n_geot_en);
    print_str(" Br="); print_int(cva.n_bridge_en);
    print_str(" Tu="); print_int(cva.n_tunnel_en);
    print_str(" Rd="); print_int(cva.n_road_en);
    print_str("\n");
}

int main(void) {
    print_str("=== Civil Engineering Admin Demo ===\n\n");
    cva_init();

    print_str("Structural engineering...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int cct = 55 + (i * 13);
        int stl = 40 + (i * 10);
        int mas = 22 + (i * 5);
        int year = 2020 + (i % 5);
        cva_struct_en(type, cat, cct, stl, mas, year);
    }

    print_str("\nGeotechnical engineering...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int fdt = 48 + (i * 11);
        int fde = 35 + (i * 8);
        int spe = 20 + (i * 4);
        int year = 2021 + (i % 4);
        cva_geot_en(type, cat, fdt, fde, spe, year);
    }

    print_str("\nBridge engineering...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int bmb = 42 + (i * 10);
        int arb = 28 + (i * 7);
        int clb = 18 + (i * 4);
        int year = 2022 + (i % 3);
        cva_bridge_en(type, cat, bmb, arb, clb, year);
    }

    print_str("\nTunnel engineering...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int mtt = 35 + (i * 8);
        int uwt = 25 + (i * 6);
        int ctt = 15 + (i * 3);
        int year = 2023 + (i % 2);
        cva_tunnel_en(type, cat, mtt, uwt, ctt, year);
    }

    print_str("\nRoad engineering...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int sbg = 30 + (i * 7);
        int pvt = 22 + (i * 5);
        int rds = 12 + (i * 3);
        int year = 2024;
        cva_road_en(type, cat, sbg, pvt, rds, year);
    }

    print_str("\nStructural engineering report...\n");
    cva_struct_en_report();

    print_str("\nGeotechnical engineering report...\n");
    cva_geot_en_report();

    print_str("\nFull report...\n");
    cva_full_report();

    print_str("\nFinal state...\n");
    cva_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
