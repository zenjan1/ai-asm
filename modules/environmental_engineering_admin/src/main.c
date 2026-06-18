/* environmental_engineering_admin: Environmental engineering administration system (v1.0)
 * Water pollution, air pollution, solid waste, noise/vibration, environmental remediation
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

#define MAX_WATER_PC    16
#define MAX_AIR_PC      14
#define MAX_SOLID_W     12
#define MAX_NOISE_VC    10
#define MAX_ENV_RF      10

typedef struct {
    int    wp_id;
    int    wp_type;
    int    wp_category;
    int    sewage_tr;
    int    indust_wt;
    int    water_recy;
    int    year;
    int    active;
} water_pc_t;

typedef struct {
    int    ap_id;
    int    ap_type;
    int    ap_category;
    int    dust_rem;
    int    desulfur;
    int    vocs_tr;
    int    year;
    int    active;
} air_pc_t;

typedef struct {
    int    sw_id;
    int    sw_type;
    int    sw_category;
    int    landfill;
    int    inciner;
    int    compost;
    int    year;
    int    active;
} solid_w_t;

typedef struct {
    int    nv_id;
    int    nv_type;
    int    nv_category;
    int    sound_ins;
    int    sound_abs;
    int    vibr_red;
    int    year;
    int    active;
} noise_vc_t;

typedef struct {
    int    er_id;
    int    er_type;
    int    er_category;
    int    soil_rem;
    int    ground_rem;
    int    eco_rest;
    int    year;
    int    active;
} env_rf_t;

typedef struct {
    int    n_water_pc;
    int    n_air_pc;
    int    n_solid_w;
    int    n_noise_vc;
    int    n_env_rf;
    int    total_sewage_tr;
    int    total_dust_rem;
    int    total_landfill;
    int    total_sound_ins;
    int    total_soil_rem;
} eva_state_t;

static water_pc_t water_pcs[MAX_WATER_PC];
static air_pc_t air_pcs[MAX_AIR_PC];
static solid_w_t solid_ws[MAX_SOLID_W];
static noise_vc_t noise_vcs[MAX_NOISE_VC];
static env_rf_t env_rfs[MAX_ENV_RF];
static eva_state_t eva;

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

int eva_init(void) {
    if (initialized) return -1;
    eva.n_water_pc = 0; eva.n_air_pc = 0; eva.n_solid_w = 0;
    eva.n_noise_vc = 0; eva.n_env_rf = 0;
    eva.total_sewage_tr = 0; eva.total_dust_rem = 0;
    eva.total_landfill = 0; eva.total_sound_ins = 0;
    eva.total_soil_rem = 0;
    for (int i = 0; i < MAX_WATER_PC; i++) water_pcs[i].active = 0;
    for (int i = 0; i < MAX_AIR_PC; i++) air_pcs[i].active = 0;
    for (int i = 0; i < MAX_SOLID_W; i++) solid_ws[i].active = 0;
    for (int i = 0; i < MAX_NOISE_VC; i++) noise_vcs[i].active = 0;
    for (int i = 0; i < MAX_ENV_RF; i++) env_rfs[i].active = 0;
    initialized = 1;
    print_str("[EVA] Environmental engineering initialized\n");
    return 0;
}

int eva_water_pc(int wt_type, int cat, int sew, int ind, int rec, int year) {
    if (eva.n_water_pc >= MAX_WATER_PC) return -1;
    water_pc_t* w = &water_pcs[eva.n_water_pc];
    w->wp_id = eva.n_water_pc;
    w->wp_type = wt_type;
    w->wp_category = cat;
    w->sewage_tr = sew;
    w->indust_wt = ind;
    w->water_recy = rec;
    w->year = year;
    w->active = 1;
    eva.total_sewage_tr += sew;
    eva.n_water_pc++;
    print_str("[EVA] Water pc "); print_int(eva.n_water_pc - 1);
    print_str(" type="); print_int(wt_type);
    print_str(" cat="); print_int(cat);
    print_str(" sew="); print_int(sew);
    print_str(" ind="); print_int(ind);
    print_str(" rec="); print_int(rec); print_str("\n");
    return eva.n_water_pc - 1;
}

int eva_air_pc(int at_type, int cat, int dst, int dsf, int vct, int year) {
    if (eva.n_air_pc >= MAX_AIR_PC) return -1;
    air_pc_t* a = &air_pcs[eva.n_air_pc];
    a->ap_id = eva.n_air_pc;
    a->ap_type = at_type;
    a->ap_category = cat;
    a->dust_rem = dst;
    a->desulfur = dsf;
    a->vocs_tr = vct;
    a->year = year;
    a->active = 1;
    eva.total_dust_rem += dst;
    eva.n_air_pc++;
    print_str("[EVA] Air pc "); print_int(eva.n_air_pc - 1);
    print_str(" type="); print_int(at_type);
    print_str(" cat="); print_int(cat);
    print_str(" dst="); print_int(dst);
    print_str(" dsf="); print_int(dsf);
    print_str(" vct="); print_int(vct); print_str("\n");
    return eva.n_air_pc - 1;
}

int eva_solid_w(int st_type, int cat, int ldf, int inc, int cmt, int year) {
    if (eva.n_solid_w >= MAX_SOLID_W) return -1;
    solid_w_t* s = &solid_ws[eva.n_solid_w];
    s->sw_id = eva.n_solid_w;
    s->sw_type = st_type;
    s->sw_category = cat;
    s->landfill = ldf;
    s->inciner = inc;
    s->compost = cmt;
    s->year = year;
    s->active = 1;
    eva.total_landfill += ldf;
    eva.n_solid_w++;
    print_str("[EVA] Solid w "); print_int(eva.n_solid_w - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" ldf="); print_int(ldf);
    print_str(" inc="); print_int(inc);
    print_str(" cmt="); print_int(cmt); print_str("\n");
    return eva.n_solid_w - 1;
}

int eva_noise_vc(int nt_type, int cat, int sni, int sna, int vbr, int year) {
    if (eva.n_noise_vc >= MAX_NOISE_VC) return -1;
    noise_vc_t* n = &noise_vcs[eva.n_noise_vc];
    n->nv_id = eva.n_noise_vc;
    n->nv_type = nt_type;
    n->nv_category = cat;
    n->sound_ins = sni;
    n->sound_abs = sna;
    n->vibr_red = vbr;
    n->year = year;
    n->active = 1;
    eva.total_sound_ins += sni;
    eva.n_noise_vc++;
    print_str("[EVA] Noise vc "); print_int(eva.n_noise_vc - 1);
    print_str(" type="); print_int(nt_type);
    print_str(" cat="); print_int(cat);
    print_str(" sni="); print_int(sni);
    print_str(" sna="); print_int(sna);
    print_str(" vbr="); print_int(vbr); print_str("\n");
    return eva.n_noise_vc - 1;
}

int eva_env_rf(int rt_type, int cat, int slr, int gnr, int ecr, int year) {
    if (eva.n_env_rf >= MAX_ENV_RF) return -1;
    env_rf_t* e = &env_rfs[eva.n_env_rf];
    e->er_id = eva.n_env_rf;
    e->er_type = rt_type;
    e->er_category = cat;
    e->soil_rem = slr;
    e->ground_rem = gnr;
    e->eco_rest = ecr;
    e->year = year;
    e->active = 1;
    eva.total_soil_rem += slr;
    eva.n_env_rf++;
    print_str("[EVA] Env rf "); print_int(eva.n_env_rf - 1);
    print_str(" type="); print_int(rt_type);
    print_str(" cat="); print_int(cat);
    print_str(" slr="); print_int(slr);
    print_str(" gnr="); print_int(gnr);
    print_str(" ecr="); print_int(ecr); print_str("\n");
    return eva.n_env_rf - 1;
}

void eva_water_pc_report(void) {
    print_str("[EVA] Water pollution report:\n");
    print_str("  Water pollution categories: "); print_int(eva.n_water_pc); print_str("\n");
    print_str("  Total sewage treatment: "); print_int(eva.total_sewage_tr); print_str("\n");
}

void eva_air_pc_report(void) {
    print_str("[EVA] Air pollution report:\n");
    print_str("  Air pollution categories: "); print_int(eva.n_air_pc); print_str("\n");
    print_str("  Total dust removal: "); print_int(eva.total_dust_rem); print_str("\n");
}

void eva_full_report(void) {
    print_str("[EVA] Full report:\n");
    print_str("  Solid waste categories: "); print_int(eva.n_solid_w); print_str("\n");
    print_str("  Total landfill: "); print_int(eva.total_landfill); print_str("\n");
    print_str("  Noise/vibration categories: "); print_int(eva.n_noise_vc); print_str("\n");
    print_str("  Total sound insulation: "); print_int(eva.total_sound_ins); print_str("\n");
    print_str("  Environmental remediation categories: "); print_int(eva.n_env_rf); print_str("\n");
    print_str("  Total soil remediation: "); print_int(eva.total_soil_rem); print_str("\n");
}

void eva_print_state(void) {
    print_str("[EVA] Wp="); print_int(eva.n_water_pc);
    print_str(" Ap="); print_int(eva.n_air_pc);
    print_str(" Sw="); print_int(eva.n_solid_w);
    print_str(" Nv="); print_int(eva.n_noise_vc);
    print_str(" Er="); print_int(eva.n_env_rf);
    print_str("\n");
}

int main(void) {
    print_str("=== Environmental Engineering Admin Demo ===\n\n");
    eva_init();

    print_str("Water pollution...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int sew = 55 + (i * 13);
        int ind = 40 + (i * 10);
        int rec = 22 + (i * 5);
        int year = 2020 + (i % 5);
        eva_water_pc(type, cat, sew, ind, rec, year);
    }

    print_str("\nAir pollution...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int dst = 48 + (i * 11);
        int dsf = 35 + (i * 8);
        int vct = 20 + (i * 4);
        int year = 2021 + (i % 4);
        eva_air_pc(type, cat, dst, dsf, vct, year);
    }

    print_str("\nSolid waste...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ldf = 42 + (i * 10);
        int inc = 28 + (i * 7);
        int cmt = 18 + (i * 4);
        int year = 2022 + (i % 3);
        eva_solid_w(type, cat, ldf, inc, cmt, year);
    }

    print_str("\nNoise/vibration...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int sni = 35 + (i * 8);
        int sna = 25 + (i * 6);
        int vbr = 15 + (i * 3);
        int year = 2023 + (i % 2);
        eva_noise_vc(type, cat, sni, sna, vbr, year);
    }

    print_str("\nEnvironmental remediation...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int slr = 30 + (i * 7);
        int gnr = 22 + (i * 5);
        int ecr = 12 + (i * 3);
        int year = 2024;
        eva_env_rf(type, cat, slr, gnr, ecr, year);
    }

    print_str("\nWater pollution report...\n");
    eva_water_pc_report();

    print_str("\nAir pollution report...\n");
    eva_air_pc_report();

    print_str("\nFull report...\n");
    eva_full_report();

    print_str("\nFinal state...\n");
    eva_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
