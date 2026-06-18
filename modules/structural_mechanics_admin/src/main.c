/* structural_mechanics_admin: Structural mechanics administration (v1.0)
 * Trusses, frames, arches, influence lines, structural dynamics
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

#define MAX_TRUSS       16
#define MAX_FRAME       14
#define MAX_ARCH        12
#define MAX_INFLUENCE   10
#define MAX_STRUCT_DYN  10

typedef struct {
    int    tr_id;
    int    tr_type;
    int    tr_category;
    int    joint_method;
    int    section_method;
    int    zero_force;
    int    year;
    int    active;
} truss_t;

typedef struct {
    int    fr_id;
    int    fr_type;
    int    fr_category;
    int    moment_diag;
    int    shear_diag;
    int    axial_diag;
    int    year;
    int    active;
} frame_t;

typedef struct {
    int    ar_id;
    int    ar_type;
    int    ar_category;
    int    three_hinge;
    int    tie_arch;
    int    fixed_arch;
    int    year;
    int    active;
} arch_t;

typedef struct {
    int    il_id;
    int    il_type;
    int    il_category;
    int    static_method;
    int    kinematic_method;
    int    critical_load;
    int    year;
    int    active;
} influence_t;

typedef struct {
    int    sd_id;
    int    sd_type;
    int    sd_category;
    int    single_dof;
    int    multi_dof;
    int    mode_superp;
    int    year;
    int    active;
} struct_dyn_t;

typedef struct {
    int    n_truss;
    int    n_frame;
    int    n_arch;
    int    n_influence;
    int    n_struct_dyn;
    int    total_joint_method;
    int    total_moment_diag;
    int    total_three_hinge;
    int    total_static_method;
    int    total_single_dof;
} sma_state_t;

static truss_t trusses[MAX_TRUSS];
static frame_t frames[MAX_FRAME];
static arch_t arches[MAX_ARCH];
static influence_t influences[MAX_INFLUENCE];
static struct_dyn_t struct_dyns[MAX_STRUCT_DYN];
static sma_state_t sma;

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

int sma_init(void) {
    if (initialized) return -1;
    sma.n_truss = 0; sma.n_frame = 0; sma.n_arch = 0;
    sma.n_influence = 0; sma.n_struct_dyn = 0;
    sma.total_joint_method = 0; sma.total_moment_diag = 0;
    sma.total_three_hinge = 0; sma.total_static_method = 0;
    sma.total_single_dof = 0;
    for (int i = 0; i < MAX_TRUSS; i++) trusses[i].active = 0;
    for (int i = 0; i < MAX_FRAME; i++) frames[i].active = 0;
    for (int i = 0; i < MAX_ARCH; i++) arches[i].active = 0;
    for (int i = 0; i < MAX_INFLUENCE; i++) influences[i].active = 0;
    for (int i = 0; i < MAX_STRUCT_DYN; i++) struct_dyns[i].active = 0;
    initialized = 1;
    print_str("[SMA] Structural mechanics initialized\n");
    return 0;
}

int sma_truss(int ts_type, int cat, int jnt, int sec, int zer, int year) {
    if (sma.n_truss >= MAX_TRUSS) return -1;
    truss_t* t = &trusses[sma.n_truss];
    t->tr_id = sma.n_truss;
    t->tr_type = ts_type;
    t->tr_category = cat;
    t->joint_method = jnt;
    t->section_method = sec;
    t->zero_force = zer;
    t->year = year;
    t->active = 1;
    sma.total_joint_method += jnt;
    sma.n_truss++;
    print_str("[SMA] Truss "); print_int(sma.n_truss - 1);
    print_str(" type="); print_int(ts_type);
    print_str(" cat="); print_int(cat);
    print_str(" jnt="); print_int(jnt);
    print_str(" sec="); print_int(sec);
    print_str(" zer="); print_int(zer); print_str("\n");
    return sma.n_truss - 1;
}

int sma_frame(int ft_type, int cat, int mdt, int shd, int axd, int year) {
    if (sma.n_frame >= MAX_FRAME) return -1;
    frame_t* f = &frames[sma.n_frame];
    f->fr_id = sma.n_frame;
    f->fr_type = ft_type;
    f->fr_category = cat;
    f->moment_diag = mdt;
    f->shear_diag = shd;
    f->axial_diag = axd;
    f->year = year;
    f->active = 1;
    sma.total_moment_diag += mdt;
    sma.n_frame++;
    print_str("[SMA] Frame "); print_int(sma.n_frame - 1);
    print_str(" type="); print_int(ft_type);
    print_str(" cat="); print_int(cat);
    print_str(" mdt="); print_int(mdt);
    print_str(" shd="); print_int(shd);
    print_str(" axd="); print_int(axd); print_str("\n");
    return sma.n_frame - 1;
}

int sma_arch(int ar_type, int cat, int thh, int tar, int fxa, int year) {
    if (sma.n_arch >= MAX_ARCH) return -1;
    arch_t* a = &arches[sma.n_arch];
    a->ar_id = sma.n_arch;
    a->ar_type = ar_type;
    a->ar_category = cat;
    a->three_hinge = thh;
    a->tie_arch = tar;
    a->fixed_arch = fxa;
    a->year = year;
    a->active = 1;
    sma.total_three_hinge += thh;
    sma.n_arch++;
    print_str("[SMA] Arch "); print_int(sma.n_arch - 1);
    print_str(" type="); print_int(ar_type);
    print_str(" cat="); print_int(cat);
    print_str(" thh="); print_int(thh);
    print_str(" tar="); print_int(tar);
    print_str(" fxa="); print_int(fxa); print_str("\n");
    return sma.n_arch - 1;
}

int sma_influence(int il_type, int cat, int stm, int knm, int crt, int year) {
    if (sma.n_influence >= MAX_INFLUENCE) return -1;
    influence_t* s = &influences[sma.n_influence];
    s->il_id = sma.n_influence;
    s->il_type = il_type;
    s->il_category = cat;
    s->static_method = stm;
    s->kinematic_method = knm;
    s->critical_load = crt;
    s->year = year;
    s->active = 1;
    sma.total_static_method += stm;
    sma.n_influence++;
    print_str("[SMA] Influence "); print_int(sma.n_influence - 1);
    print_str(" type="); print_int(il_type);
    print_str(" cat="); print_int(cat);
    print_str(" stm="); print_int(stm);
    print_str(" knm="); print_int(knm);
    print_str(" crt="); print_int(crt); print_str("\n");
    return sma.n_influence - 1;
}

int sma_struct_dyn(int sd_type, int cat, int sgd, int mlt, int mds, int year) {
    if (sma.n_struct_dyn >= MAX_STRUCT_DYN) return -1;
    struct_dyn_t* d = &struct_dyns[sma.n_struct_dyn];
    d->sd_id = sma.n_struct_dyn;
    d->sd_type = sd_type;
    d->sd_category = cat;
    d->single_dof = sgd;
    d->multi_dof = mlt;
    d->mode_superp = mds;
    d->year = year;
    d->active = 1;
    sma.total_single_dof += sgd;
    sma.n_struct_dyn++;
    print_str("[SMA] Struct dyn "); print_int(sma.n_struct_dyn - 1);
    print_str(" type="); print_int(sd_type);
    print_str(" cat="); print_int(cat);
    print_str(" sgd="); print_int(sgd);
    print_str(" mlt="); print_int(mlt);
    print_str(" mds="); print_int(mds); print_str("\n");
    return sma.n_struct_dyn - 1;
}

void sma_truss_report(void) {
    print_str("[SMA] Truss report:\n");
    print_str("  Truss categories: "); print_int(sma.n_truss); print_str("\n");
    print_str("  Total joint method: "); print_int(sma.total_joint_method); print_str("\n");
}

void sma_frame_report(void) {
    print_str("[SMA] Frame report:\n");
    print_str("  Frame categories: "); print_int(sma.n_frame); print_str("\n");
    print_str("  Total moment diagrams: "); print_int(sma.total_moment_diag); print_str("\n");
}

void sma_full_report(void) {
    print_str("[SMA] Full report:\n");
    print_str("  Arch categories: "); print_int(sma.n_arch); print_str("\n");
    print_str("  Total three-hinge: "); print_int(sma.total_three_hinge); print_str("\n");
    print_str("  Influence line categories: "); print_int(sma.n_influence); print_str("\n");
    print_str("  Total static method: "); print_int(sma.total_static_method); print_str("\n");
    print_str("  Structural dynamics categories: "); print_int(sma.n_struct_dyn); print_str("\n");
    print_str("  Total single DOF: "); print_int(sma.total_single_dof); print_str("\n");
}

void sma_print_state(void) {
    print_str("[SMA] Tr="); print_int(sma.n_truss);
    print_str(" Fr="); print_int(sma.n_frame);
    print_str(" Ar="); print_int(sma.n_arch);
    print_str(" Il="); print_int(sma.n_influence);
    print_str(" Sd="); print_int(sma.n_struct_dyn);
    print_str("\n");
}

int main(void) {
    print_str("=== Structural Mechanics Admin Demo ===\n\n");
    sma_init();

    print_str("Trusses...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int jnt = 55 + (i * 13);
        int sec = 40 + (i * 10);
        int zer = 22 + (i * 5);
        int year = 2020 + (i % 5);
        sma_truss(type, cat, jnt, sec, zer, year);
    }

    print_str("\nFrames...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int mdt = 48 + (i * 11);
        int shd = 35 + (i * 8);
        int axd = 20 + (i * 4);
        int year = 2021 + (i % 4);
        sma_frame(type, cat, mdt, shd, axd, year);
    }

    print_str("\nArches...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int thh = 42 + (i * 10);
        int tar = 28 + (i * 7);
        int fxa = 18 + (i * 4);
        int year = 2022 + (i % 3);
        sma_arch(type, cat, thh, tar, fxa, year);
    }

    print_str("\nInfluence lines...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int stm = 35 + (i * 8);
        int knm = 25 + (i * 6);
        int crt = 15 + (i * 3);
        int year = 2023 + (i % 2);
        sma_influence(type, cat, stm, knm, crt, year);
    }

    print_str("\nStructural dynamics...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int sgd = 30 + (i * 7);
        int mlt = 22 + (i * 5);
        int mds = 12 + (i * 3);
        int year = 2024;
        sma_struct_dyn(type, cat, sgd, mlt, mds, year);
    }

    print_str("\nTruss report...\n");
    sma_truss_report();

    print_str("\nFrame report...\n");
    sma_frame_report();

    print_str("\nFull report...\n");
    sma_full_report();

    print_str("\nFinal state...\n");
    sma_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
