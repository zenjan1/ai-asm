/* advanced_materials_admin: Advanced materials administration system (v1.0)
 * Metal alloys, ceramics, composites, smart materials, biomaterials
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

#define MAX_METAL      16
#define MAX_CERAMIC    14
#define MAX_COMPOSITE  12
#define MAX_SMART      10
#define MAX_BIO        10

typedef struct {
    int    metal_id;
    int    metal_type;
    int    metal_category;
    int    superalloy;
    int    titanium_alloy;
    int    aluminum_alloy;
    int    year;
    int    active;
} metal_t;

typedef struct {
    int    ceramic_id;
    int    ceramic_type;
    int    ceramic_category;
    int    structural;
    int    functional;
    int    bioceramic;
    int    year;
    int    active;
} ceramic_t;

typedef struct {
    int    composite_id;
    int    composite_type;
    int    composite_category;
    int    carbon_fiber;
    int    glass_fiber;
    int    aramid_fiber;
    int    year;
    int    active;
} composite_t;

typedef struct {
    int    smart_id;
    int    smart_type;
    int    smart_category;
    int    shape_memory;
    int    piezoelectric;
    int    magnetostrict;
    int    year;
    int    active;
} smart_t;

typedef struct {
    int    bio_id;
    int    bio_type;
    int    bio_category;
    int    medical_metal;
    int    biodegradable;
    int    tissue_scaffold;
    int    year;
    int    active;
} bio_t;

typedef struct {
    int    n_metal;
    int    n_ceramic;
    int    n_composite;
    int    n_smart;
    int    n_bio;
    int    total_superalloy;
    int    total_structural;
    int    total_carbon;
    int    total_shape;
    int    total_medical;
} ama_state_t;

static metal_t metals[MAX_METAL];
static ceramic_t ceramics[MAX_CERAMIC];
static composite_t composites[MAX_COMPOSITE];
static smart_t smart_mat[MAX_SMART];
static bio_t bio_mat[MAX_BIO];
static ama_state_t ama;

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

int ama_init(void) {
    if (initialized) return -1;
    ama.n_metal = 0; ama.n_ceramic = 0; ama.n_composite = 0;
    ama.n_smart = 0; ama.n_bio = 0;
    ama.total_superalloy = 0; ama.total_structural = 0;
    ama.total_carbon = 0; ama.total_shape = 0;
    ama.total_medical = 0;
    for (int i = 0; i < MAX_METAL; i++) metals[i].active = 0;
    for (int i = 0; i < MAX_CERAMIC; i++) ceramics[i].active = 0;
    for (int i = 0; i < MAX_COMPOSITE; i++) composites[i].active = 0;
    for (int i = 0; i < MAX_SMART; i++) smart_mat[i].active = 0;
    for (int i = 0; i < MAX_BIO; i++) bio_mat[i].active = 0;
    initialized = 1;
    print_str("[AMA] Advanced materials initialized\n");
    return 0;
}

int ama_metal(int mt_type, int cat, int super_a, int titan, int alum, int year) {
    if (ama.n_metal >= MAX_METAL) return -1;
    metal_t* m = &metals[ama.n_metal];
    m->metal_id = ama.n_metal;
    m->metal_type = mt_type;
    m->metal_category = cat;
    m->superalloy = super_a;
    m->titanium_alloy = titan;
    m->aluminum_alloy = alum;
    m->year = year;
    m->active = 1;
    ama.total_superalloy += super_a;
    ama.n_metal++;
    print_str("[AMA] Metal "); print_int(ama.n_metal - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" sup="); print_int(super_a);
    print_str(" tit="); print_int(titan);
    print_str(" alm="); print_int(alum); print_str("\n");
    return ama.n_metal - 1;
}

int ama_ceramic(int cr_type, int cat, int struct_c, int func_c, int bioc, int year) {
    if (ama.n_ceramic >= MAX_CERAMIC) return -1;
    ceramic_t* c = &ceramics[ama.n_ceramic];
    c->ceramic_id = ama.n_ceramic;
    c->ceramic_type = cr_type;
    c->ceramic_category = cat;
    c->structural = struct_c;
    c->functional = func_c;
    c->bioceramic = bioc;
    c->year = year;
    c->active = 1;
    ama.total_structural += struct_c;
    ama.n_ceramic++;
    print_str("[AMA] Ceramic "); print_int(ama.n_ceramic - 1);
    print_str(" type="); print_int(cr_type);
    print_str(" cat="); print_int(cat);
    print_str(" str="); print_int(struct_c);
    print_str(" fnc="); print_int(func_c);
    print_str(" bio="); print_int(bioc); print_str("\n");
    return ama.n_ceramic - 1;
}

int ama_composite(int cp_type, int cat, int carbon, int glass, int aramid, int year) {
    if (ama.n_composite >= MAX_COMPOSITE) return -1;
    composite_t* c = &composites[ama.n_composite];
    c->composite_id = ama.n_composite;
    c->composite_type = cp_type;
    c->composite_category = cat;
    c->carbon_fiber = carbon;
    c->glass_fiber = glass;
    c->aramid_fiber = aramid;
    c->year = year;
    c->active = 1;
    ama.total_carbon += carbon;
    ama.n_composite++;
    print_str("[AMA] Composite "); print_int(ama.n_composite - 1);
    print_str(" type="); print_int(cp_type);
    print_str(" cat="); print_int(cat);
    print_str(" crb="); print_int(carbon);
    print_str(" gls="); print_int(glass);
    print_str(" arm="); print_int(aramid); print_str("\n");
    return ama.n_composite - 1;
}

int ama_smart(int sm_type, int cat, int shape_m, int piezo, int magneto, int year) {
    if (ama.n_smart >= MAX_SMART) return -1;
    smart_t* s = &smart_mat[ama.n_smart];
    s->smart_id = ama.n_smart;
    s->smart_type = sm_type;
    s->smart_category = cat;
    s->shape_memory = shape_m;
    s->piezoelectric = piezo;
    s->magnetostrict = magneto;
    s->year = year;
    s->active = 1;
    ama.total_shape += shape_m;
    ama.n_smart++;
    print_str("[AMA] Smart "); print_int(ama.n_smart - 1);
    print_str(" type="); print_int(sm_type);
    print_str(" cat="); print_int(cat);
    print_str(" shp="); print_int(shape_m);
    print_str(" pie="); print_int(piezo);
    print_str(" mag="); print_int(magneto); print_str("\n");
    return ama.n_smart - 1;
}

int ama_bio(int bi_type, int cat, int med_m, int biodeg, int scaffold, int year) {
    if (ama.n_bio >= MAX_BIO) return -1;
    bio_t* b = &bio_mat[ama.n_bio];
    b->bio_id = ama.n_bio;
    b->bio_type = bi_type;
    b->bio_category = cat;
    b->medical_metal = med_m;
    b->biodegradable = biodeg;
    b->tissue_scaffold = scaffold;
    b->year = year;
    b->active = 1;
    ama.total_medical += med_m;
    ama.n_bio++;
    print_str("[AMA] Bio "); print_int(ama.n_bio - 1);
    print_str(" type="); print_int(bi_type);
    print_str(" cat="); print_int(cat);
    print_str(" mdc="); print_int(med_m);
    print_str(" bdg="); print_int(biodeg);
    print_str(" scf="); print_int(scaffold); print_str("\n");
    return ama.n_bio - 1;
}

void ama_metal_report(void) {
    print_str("[AMA] Metal report:\n");
    print_str("  Metal alloy categories: "); print_int(ama.n_metal); print_str("\n");
    print_str("  Total superalloys: "); print_int(ama.total_superalloy); print_str("\n");
}

void ama_ceramic_report(void) {
    print_str("[AMA] Ceramic report:\n");
    print_str("  Ceramic categories: "); print_int(ama.n_ceramic); print_str("\n");
    print_str("  Total structural ceramics: "); print_int(ama.total_structural); print_str("\n");
}

void ama_bio_report(void) {
    print_str("[AMA] Bio report:\n");
    print_str("  Composite categories: "); print_int(ama.n_composite); print_str("\n");
    print_str("  Total carbon fiber: "); print_int(ama.total_carbon); print_str("\n");
    print_str("  Smart material categories: "); print_int(ama.n_smart); print_str("\n");
    print_str("  Total shape memory: "); print_int(ama.total_shape); print_str("\n");
    print_str("  Biomaterial categories: "); print_int(ama.n_bio); print_str("\n");
    print_str("  Total medical metal: "); print_int(ama.total_medical); print_str("\n");
}

void ama_print_state(void) {
    print_str("[AMA] Mt="); print_int(ama.n_metal);
    print_str(" Cr="); print_int(ama.n_ceramic);
    print_str(" Cp="); print_int(ama.n_composite);
    print_str(" Sm="); print_int(ama.n_smart);
    print_str(" Bi="); print_int(ama.n_bio);
    print_str("\n");
}

int main(void) {
    print_str("=== Advanced Materials Admin Demo ===\n\n");
    ama_init();

    print_str("Metal alloys...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int sup = 60 + (i * 14);
        int tit = 40 + (i * 10);
        int alm = 25 + (i * 6);
        int year = 2020 + (i % 5);
        ama_metal(type, cat, sup, tit, alm, year);
    }

    print_str("\nCeramics...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int str = 50 + (i * 12);
        int fnc = 35 + (i * 8);
        int bio = 20 + (i * 5);
        int year = 2021 + (i % 4);
        ama_ceramic(type, cat, str, fnc, bio, year);
    }

    print_str("\nComposites...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int crb = 42 + (i * 10);
        int gls = 28 + (i * 7);
        int arm = 18 + (i * 4);
        int year = 2022 + (i % 3);
        ama_composite(type, cat, crb, gls, arm, year);
    }

    print_str("\nSmart materials...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int shp = 35 + (i * 8);
        int pie = 25 + (i * 6);
        int mag = 15 + (i * 3);
        int year = 2023 + (i % 2);
        ama_smart(type, cat, shp, pie, mag, year);
    }

    print_str("\nBiomaterials...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int mdc = 30 + (i * 7);
        int bdg = 20 + (i * 5);
        int scf = 12 + (i * 3);
        int year = 2024;
        ama_bio(type, cat, mdc, bdg, scf, year);
    }

    print_str("\nMetal report...\n");
    ama_metal_report();

    print_str("\nCeramic report...\n");
    ama_ceramic_report();

    print_str("\nBio report...\n");
    ama_bio_report();

    print_str("\nFinal state...\n");
    ama_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
