/* carbon_fiber_admin: Carbon fiber administration system (v1.0)
 * PAN-based, pitch-based, rayon-based, carbon fiber products, applications
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

#define MAX_PAN        16
#define MAX_PITCH      14
#define MAX_RAYON      12
#define MAX_PRODUCT    10
#define MAX_APP        10

typedef struct {
    int    pan_id;
    int    pan_type;
    int    pan_category;
    int    tow_count;
    int    tensile_str;
    int    modulus;
    int    year;
    int    active;
} pan_t;

typedef struct {
    int    pitch_id;
    int    pitch_type;
    int    pitch_category;
    int    isotropic;
    int    anisotropic;
    int    mesophase;
    int    year;
    int    active;
} pitch_t;

typedef struct {
    int    rayon_id;
    int    rayon_type;
    int    rayon_category;
    int    standard_ray;
    int    high_perf;
    int    special_ray;
    int    year;
    int    active;
} rayon_t;

typedef struct {
    int    prod_id;
    int    prod_type;
    int    prod_category;
    int    carbon_cloth;
    int    prepreg;
    int    composite_p;
    int    year;
    int    active;
} product_t;

typedef struct {
    int    app_id;
    int    app_type;
    int    app_category;
    int    aerospace_app;
    int    automotive_app;
    int    sports_app;
    int    year;
    int    active;
} app_t;

typedef struct {
    int    n_pan;
    int    n_pitch;
    int    n_rayon;
    int    n_product;
    int    n_app;
    int    total_tow;
    int    total_iso;
    int    total_std_ray;
    int    total_cloth;
    int    total_aero;
} cfa_state_t;

static pan_t pans[MAX_PAN];
static pitch_t pitches[MAX_PITCH];
static rayon_t rayons[MAX_RAYON];
static product_t products[MAX_PRODUCT];
static app_t apps[MAX_APP];
static cfa_state_t cfa;

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

int cfa_init(void) {
    if (initialized) return -1;
    cfa.n_pan = 0; cfa.n_pitch = 0; cfa.n_rayon = 0;
    cfa.n_product = 0; cfa.n_app = 0;
    cfa.total_tow = 0; cfa.total_iso = 0;
    cfa.total_std_ray = 0; cfa.total_cloth = 0;
    cfa.total_aero = 0;
    for (int i = 0; i < MAX_PAN; i++) pans[i].active = 0;
    for (int i = 0; i < MAX_PITCH; i++) pitches[i].active = 0;
    for (int i = 0; i < MAX_RAYON; i++) rayons[i].active = 0;
    for (int i = 0; i < MAX_PRODUCT; i++) products[i].active = 0;
    for (int i = 0; i < MAX_APP; i++) apps[i].active = 0;
    initialized = 1;
    print_str("[CFA] Carbon fiber initialized\n");
    return 0;
}

int cfa_pan(int p_type, int cat, int tow, int tensile, int modl, int year) {
    if (cfa.n_pan >= MAX_PAN) return -1;
    pan_t* p = &pans[cfa.n_pan];
    p->pan_id = cfa.n_pan;
    p->pan_type = p_type;
    p->pan_category = cat;
    p->tow_count = tow;
    p->tensile_str = tensile;
    p->modulus = modl;
    p->year = year;
    p->active = 1;
    cfa.total_tow += tow;
    cfa.n_pan++;
    print_str("[CFA] PAN "); print_int(cfa.n_pan - 1);
    print_str(" type="); print_int(p_type);
    print_str(" cat="); print_int(cat);
    print_str(" tow="); print_int(tow);
    print_str(" tns="); print_int(tensile);
    print_str(" mdl="); print_int(modl); print_str("\n");
    return cfa.n_pan - 1;
}

int cfa_pitch(int pt_type, int cat, int iso, int aniso, int meso, int year) {
    if (cfa.n_pitch >= MAX_PITCH) return -1;
    pitch_t* p = &pitches[cfa.n_pitch];
    p->pitch_id = cfa.n_pitch;
    p->pitch_type = pt_type;
    p->pitch_category = cat;
    p->isotropic = iso;
    p->anisotropic = aniso;
    p->mesophase = meso;
    p->year = year;
    p->active = 1;
    cfa.total_iso += iso;
    cfa.n_pitch++;
    print_str("[CFA] Pitch "); print_int(cfa.n_pitch - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" iso="); print_int(iso);
    print_str(" ani="); print_int(aniso);
    print_str(" mes="); print_int(meso); print_str("\n");
    return cfa.n_pitch - 1;
}

int cfa_rayon(int r_type, int cat, int std_r, int hi_perf, int spec_r, int year) {
    if (cfa.n_rayon >= MAX_RAYON) return -1;
    rayon_t* r = &rayons[cfa.n_rayon];
    r->rayon_id = cfa.n_rayon;
    r->rayon_type = r_type;
    r->rayon_category = cat;
    r->standard_ray = std_r;
    r->high_perf = hi_perf;
    r->special_ray = spec_r;
    r->year = year;
    r->active = 1;
    cfa.total_std_ray += std_r;
    cfa.n_rayon++;
    print_str("[CFA] Rayon "); print_int(cfa.n_rayon - 1);
    print_str(" type="); print_int(r_type);
    print_str(" cat="); print_int(cat);
    print_str(" std="); print_int(std_r);
    print_str(" hip="); print_int(hi_perf);
    print_str(" spr="); print_int(spec_r); print_str("\n");
    return cfa.n_rayon - 1;
}

int cfa_product(int pd_type, int cat, int cloth, int prep, int comp, int year) {
    if (cfa.n_product >= MAX_PRODUCT) return -1;
    product_t* p = &products[cfa.n_product];
    p->prod_id = cfa.n_product;
    p->prod_type = pd_type;
    p->prod_category = cat;
    p->carbon_cloth = cloth;
    p->prepreg = prep;
    p->composite_p = comp;
    p->year = year;
    p->active = 1;
    cfa.total_cloth += cloth;
    cfa.n_product++;
    print_str("[CFA] Product "); print_int(cfa.n_product - 1);
    print_str(" type="); print_int(pd_type);
    print_str(" cat="); print_int(cat);
    print_str(" clt="); print_int(cloth);
    print_str(" prp="); print_int(prep);
    print_str(" cmp="); print_int(comp); print_str("\n");
    return cfa.n_product - 1;
}

int cfa_app(int ap_type, int cat, int aero, int auto_v, int sport, int year) {
    if (cfa.n_app >= MAX_APP) return -1;
    app_t* a = &apps[cfa.n_app];
    a->app_id = cfa.n_app;
    a->app_type = ap_type;
    a->app_category = cat;
    a->aerospace_app = aero;
    a->automotive_app = auto_v;
    a->sports_app = sport;
    a->year = year;
    a->active = 1;
    cfa.total_aero += aero;
    cfa.n_app++;
    print_str("[CFA] App "); print_int(cfa.n_app - 1);
    print_str(" type="); print_int(ap_type);
    print_str(" cat="); print_int(cat);
    print_str(" aer="); print_int(aero);
    print_str(" aut="); print_int(auto_v);
    print_str(" spr="); print_int(sport); print_str("\n");
    return cfa.n_app - 1;
}

void cfa_pan_report(void) {
    print_str("[CFA] PAN report:\n");
    print_str("  PAN-based CF categories: "); print_int(cfa.n_pan); print_str("\n");
    print_str("  Total tow count: "); print_int(cfa.total_tow); print_str("\n");
}

void cfa_pitch_report(void) {
    print_str("[CFA] Pitch report:\n");
    print_str("  Pitch-based CF categories: "); print_int(cfa.n_pitch); print_str("\n");
    print_str("  Total isotropic: "); print_int(cfa.total_iso); print_str("\n");
}

void cfa_app_report(void) {
    print_str("[CFA] App report:\n");
    print_str("  Rayon-based CF categories: "); print_int(cfa.n_rayon); print_str("\n");
    print_str("  Total standard rayon: "); print_int(cfa.total_std_ray); print_str("\n");
    print_str("  CF product categories: "); print_int(cfa.n_product); print_str("\n");
    print_str("  Total carbon cloth: "); print_int(cfa.total_cloth); print_str("\n");
    print_str("  CF application categories: "); print_int(cfa.n_app); print_str("\n");
    print_str("  Total aerospace: "); print_int(cfa.total_aero); print_str("\n");
}

void cfa_print_state(void) {
    print_str("[CFA] Pn="); print_int(cfa.n_pan);
    print_str(" Pt="); print_int(cfa.n_pitch);
    print_str(" Ry="); print_int(cfa.n_rayon);
    print_str(" Pd="); print_int(cfa.n_product);
    print_str(" Ap="); print_int(cfa.n_app);
    print_str("\n");
}

int main(void) {
    print_str("=== Carbon Fiber Admin Demo ===\n\n");
    cfa_init();

    print_str("PAN-based CF...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int tow = 58 + (i * 14);
        int tns = 42 + (i * 10);
        int mdl = 25 + (i * 6);
        int year = 2020 + (i % 5);
        cfa_pan(type, cat, tow, tns, mdl, year);
    }

    print_str("\nPitch-based CF...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int iso = 50 + (i * 12);
        int ani = 35 + (i * 8);
        int mes = 22 + (i * 5);
        int year = 2021 + (i % 4);
        cfa_pitch(type, cat, iso, ani, mes, year);
    }

    print_str("\nRayon-based CF...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int std = 45 + (i * 11);
        int hip = 30 + (i * 7);
        int spr = 18 + (i * 4);
        int year = 2022 + (i % 3);
        cfa_rayon(type, cat, std, hip, spr, year);
    }

    print_str("\nCF products...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int clt = 38 + (i * 9);
        int prp = 25 + (i * 6);
        int cmp = 15 + (i * 3);
        int year = 2023 + (i % 2);
        cfa_product(type, cat, clt, prp, cmp, year);
    }

    print_str("\nCF applications...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int aer = 32 + (i * 8);
        int aut = 22 + (i * 5);
        int spr = 12 + (i * 3);
        int year = 2024;
        cfa_app(type, cat, aer, aut, spr, year);
    }

    print_str("\nPAN report...\n");
    cfa_pan_report();

    print_str("\nPitch report...\n");
    cfa_pitch_report();

    print_str("\nApp report...\n");
    cfa_app_report();

    print_str("\nFinal state...\n");
    cfa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
