/* ferrite_admin: Ferrite administration system (v1.0)
 * Soft ferrite, hard ferrite,旋磁 ferrite, square ferrite, applications
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

#define MAX_SOFT_F     16
#define MAX_HARD_F     14
#define MAX_SPIN_F     12
#define MAX_SQ_F       10
#define MAX_APP_F      10

typedef struct {
    int    soft_id;
    int    soft_type;
    int    soft_category;
    int    mnzn_fer;
    int    nizin_fer;
    int    mgzn_fer;
    int    year;
    int    active;
} soft_f_t;

typedef struct {
    int    hard_id;
    int    hard_type;
    int    hard_category;
    int    ba_fer;
    int    sr_fer;
    int    pb_fer;
    int    year;
    int    active;
} hard_f_t;

typedef struct {
    int    spin_id;
    int    spin_type;
    int    spin_category;
    int    yig;
    int    gig;
    int    euig;
    int    year;
    int    active;
} spin_f_t;

typedef struct {
    int    sq_id;
    int    sq_type;
    int    sq_category;
    int    mgmn_fer;
    int    limn_fer;
    int    cofe_fer;
    int    year;
    int    active;
} sq_f_t;

typedef struct {
    int    app_id;
    int    app_type;
    int    app_category;
    int    inductor;
    int    transformer;
    int    electromagnet;
    int    year;
    int    active;
} app_f_t;

typedef struct {
    int    n_soft_f;
    int    n_hard_f;
    int    n_spin_f;
    int    n_sq_f;
    int    n_app_f;
    int    total_mnzn;
    int    total_ba;
    int    total_yig;
    int    total_mgmn;
    int    total_inductor;
} fa_state_t;

static soft_f_t soft_fs[MAX_SOFT_F];
static hard_f_t hard_fs[MAX_HARD_F];
static spin_f_t spin_fs[MAX_SPIN_F];
static sq_f_t sq_fs[MAX_SQ_F];
static app_f_t app_fs[MAX_APP_F];
static fa_state_t fa;

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

int fa_init(void) {
    if (initialized) return -1;
    fa.n_soft_f = 0; fa.n_hard_f = 0; fa.n_spin_f = 0;
    fa.n_sq_f = 0; fa.n_app_f = 0;
    fa.total_mnzn = 0; fa.total_ba = 0;
    fa.total_yig = 0; fa.total_mgmn = 0;
    fa.total_inductor = 0;
    for (int i = 0; i < MAX_SOFT_F; i++) soft_fs[i].active = 0;
    for (int i = 0; i < MAX_HARD_F; i++) hard_fs[i].active = 0;
    for (int i = 0; i < MAX_SPIN_F; i++) spin_fs[i].active = 0;
    for (int i = 0; i < MAX_SQ_F; i++) sq_fs[i].active = 0;
    for (int i = 0; i < MAX_APP_F; i++) app_fs[i].active = 0;
    initialized = 1;
    print_str("[FA] Ferrite initialized\n");
    return 0;
}

int fa_soft_f(int sf_type, int cat, int mnzn, int nizin, int mgzn, int year) {
    if (fa.n_soft_f >= MAX_SOFT_F) return -1;
    soft_f_t* s = &soft_fs[fa.n_soft_f];
    s->soft_id = fa.n_soft_f;
    s->soft_type = sf_type;
    s->soft_category = cat;
    s->mnzn_fer = mnzn;
    s->nizin_fer = nizin;
    s->mgzn_fer = mgzn;
    s->year = year;
    s->active = 1;
    fa.total_mnzn += mnzn;
    fa.n_soft_f++;
    print_str("[FA] Soft ferrite "); print_int(fa.n_soft_f - 1);
    print_str(" type="); print_int(sf_type);
    print_str(" cat="); print_int(cat);
    print_str(" mnz="); print_int(mnzn);
    print_str(" niz="); print_int(nizin);
    print_str(" mgz="); print_int(mgzn); print_str("\n");
    return fa.n_soft_f - 1;
}

int fa_hard_f(int hf_type, int cat, int ba_f, int sr_f, int pb_f, int year) {
    if (fa.n_hard_f >= MAX_HARD_F) return -1;
    hard_f_t* h = &hard_fs[fa.n_hard_f];
    h->hard_id = fa.n_hard_f;
    h->hard_type = hf_type;
    h->hard_category = cat;
    h->ba_fer = ba_f;
    h->sr_fer = sr_f;
    h->pb_fer = pb_f;
    h->year = year;
    h->active = 1;
    fa.total_ba += ba_f;
    fa.n_hard_f++;
    print_str("[FA] Hard ferrite "); print_int(fa.n_hard_f - 1);
    print_str(" type="); print_int(hf_type);
    print_str(" cat="); print_int(cat);
    print_str(" ba="); print_int(ba_f);
    print_str(" sr="); print_int(sr_f);
    print_str(" pb="); print_int(pb_f); print_str("\n");
    return fa.n_hard_f - 1;
}

int fa_spin_f(int sp_type, int cat, int yig_v, int gig_v, int euig_v, int year) {
    if (fa.n_spin_f >= MAX_SPIN_F) return -1;
    spin_f_t* s = &spin_fs[fa.n_spin_f];
    s->spin_id = fa.n_spin_f;
    s->spin_type = sp_type;
    s->spin_category = cat;
    s->yig = yig_v;
    s->gig = gig_v;
    s->euig = euig_v;
    s->year = year;
    s->active = 1;
    fa.total_yig += yig_v;
    fa.n_spin_f++;
    print_str("[FA] Spin ferrite "); print_int(fa.n_spin_f - 1);
    print_str(" type="); print_int(sp_type);
    print_str(" cat="); print_int(cat);
    print_str(" yig="); print_int(yig_v);
    print_str(" gig="); print_int(gig_v);
    print_str(" eui="); print_int(euig_v); print_str("\n");
    return fa.n_spin_f - 1;
}

int fa_sq_f(int sq_type, int cat, int mgmn, int limn, int cofe, int year) {
    if (fa.n_sq_f >= MAX_SQ_F) return -1;
    sq_f_t* s = &sq_fs[fa.n_sq_f];
    s->sq_id = fa.n_sq_f;
    s->sq_type = sq_type;
    s->sq_category = cat;
    s->mgmn_fer = mgmn;
    s->limn_fer = limn;
    s->cofe_fer = cofe;
    s->year = year;
    s->active = 1;
    fa.total_mgmn += mgmn;
    fa.n_sq_f++;
    print_str("[FA] Square ferrite "); print_int(fa.n_sq_f - 1);
    print_str(" type="); print_int(sq_type);
    print_str(" cat="); print_int(cat);
    print_str(" mgm="); print_int(mgmn);
    print_str(" lmn="); print_int(limn);
    print_str(" cfe="); print_int(cofe); print_str("\n");
    return fa.n_sq_f - 1;
}

int fa_app_f(int af_type, int cat, int induc, int trans, int electrom, int year) {
    if (fa.n_app_f >= MAX_APP_F) return -1;
    app_f_t* a = &app_fs[fa.n_app_f];
    a->app_id = fa.n_app_f;
    a->app_type = af_type;
    a->app_category = cat;
    a->inductor = induc;
    a->transformer = trans;
    a->electromagnet = electrom;
    a->year = year;
    a->active = 1;
    fa.total_inductor += induc;
    fa.n_app_f++;
    print_str("[FA] App "); print_int(fa.n_app_f - 1);
    print_str(" type="); print_int(af_type);
    print_str(" cat="); print_int(cat);
    print_str(" ind="); print_int(induc);
    print_str(" trs="); print_int(trans);
    print_str(" elm="); print_int(electrom); print_str("\n");
    return fa.n_app_f - 1;
}

void fa_soft_report(void) {
    print_str("[FA] Soft report:\n");
    print_str("  Soft ferrite categories: "); print_int(fa.n_soft_f); print_str("\n");
    print_str("  Total MnZn ferrite: "); print_int(fa.total_mnzn); print_str("\n");
}

void fa_hard_report(void) {
    print_str("[FA] Hard report:\n");
    print_str("  Hard ferrite categories: "); print_int(fa.n_hard_f); print_str("\n");
    print_str("  Total Ba ferrite: "); print_int(fa.total_ba); print_str("\n");
}

void fa_app_report(void) {
    print_str("[FA] App report:\n");
    print_str("  Spin ferrite categories: "); print_int(fa.n_spin_f); print_str("\n");
    print_str("  Total YIG: "); print_int(fa.total_yig); print_str("\n");
    print_str("  Square ferrite categories: "); print_int(fa.n_sq_f); print_str("\n");
    print_str("  Total MgMn ferrite: "); print_int(fa.total_mgmn); print_str("\n");
    print_str("  Ferrite application categories: "); print_int(fa.n_app_f); print_str("\n");
    print_str("  Total inductors: "); print_int(fa.total_inductor); print_str("\n");
}

void fa_print_state(void) {
    print_str("[FA] Sf="); print_int(fa.n_soft_f);
    print_str(" Hd="); print_int(fa.n_hard_f);
    print_str(" Sp="); print_int(fa.n_spin_f);
    print_str(" Sq="); print_int(fa.n_sq_f);
    print_str(" Ap="); print_int(fa.n_app_f);
    print_str("\n");
}

int main(void) {
    print_str("=== Ferrite Admin Demo ===\n\n");
    fa_init();

    print_str("Soft ferrite...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int mnz = 58 + (i * 14);
        int niz = 40 + (i * 10);
        int mgz = 25 + (i * 6);
        int year = 2020 + (i % 5);
        fa_soft_f(type, cat, mnz, niz, mgz, year);
    }

    print_str("\nHard ferrite...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ba = 50 + (i * 12);
        int sr = 35 + (i * 8);
        int pb = 22 + (i * 5);
        int year = 2021 + (i % 4);
        fa_hard_f(type, cat, ba, sr, pb, year);
    }

    print_str("\nSpin ferrite...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int yig = 45 + (i * 11);
        int gig = 30 + (i * 7);
        int eui = 18 + (i * 4);
        int year = 2022 + (i % 3);
        fa_spin_f(type, cat, yig, gig, eui, year);
    }

    print_str("\nSquare ferrite...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int mgm = 38 + (i * 9);
        int lmn = 25 + (i * 6);
        int cfe = 15 + (i * 3);
        int year = 2023 + (i % 2);
        fa_sq_f(type, cat, mgm, lmn, cfe, year);
    }

    print_str("\nFerrite applications...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ind = 32 + (i * 8);
        int trs = 22 + (i * 5);
        int elm = 12 + (i * 3);
        int year = 2024;
        fa_app_f(type, cat, ind, trs, elm, year);
    }

    print_str("\nSoft report...\n");
    fa_soft_report();

    print_str("\nHard report...\n");
    fa_hard_report();

    print_str("\nApp report...\n");
    fa_app_report();

    print_str("\nFinal state...\n");
    fa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
