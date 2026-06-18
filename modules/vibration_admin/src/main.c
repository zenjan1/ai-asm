/* vibration_admin: Vibration engineering administration system (v1.0)
 * Vibration theory, isolation, modal analysis, control, damping
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

#define MAX_VIBRATION   16
#define MAX_ISOLATION   14
#define MAX_MODAL       12
#define MAX_CTRL_VIB    10
#define MAX_DAMPING     10

typedef struct {
    int    vb_id;
    int    vb_type;
    int    vb_category;
    int    free_vb;
    int    forced_vb;
    int    self_excited;
    int    year;
    int    active;
} vibration_t;

typedef struct {
    int    iso_id;
    int    iso_type;
    int    iso_category;
    int    active_iso;
    int    passive_iso;
    int    hybrid_iso;
    int    year;
    int    active;
} isolation_t;

typedef struct {
    int    md_id;
    int    md_type;
    int    md_category;
    int    modal_ident;
    int    modal_super;
    int    freq_resp;
    int    year;
    int    active;
} modal_t;

typedef struct {
    int    ctv_id;
    int    ctv_type;
    int    ctv_category;
    int    tmd;
    int    base_iso;
    int    energy_diss;
    int    year;
    int    active;
} control_vib_t;

typedef struct {
    int    dmp_id;
    int    dmp_type;
    int    dmp_category;
    int    damping_mat;
    int    dyn_absorber;
    int    impact_dmp;
    int    year;
    int    active;
} damping_t;

typedef struct {
    int    n_vibration;
    int    n_isolation;
    int    n_modal;
    int    n_ctrl_vib;
    int    n_damping;
    int    total_free_vb;
    int    total_active_iso;
    int    total_modal_ident;
    int    total_tmd;
    int    total_damping_mat;
} vba_state_t;

static vibration_t vibrations[MAX_VIBRATION];
static isolation_t isolations[MAX_ISOLATION];
static modal_t modals[MAX_MODAL];
static control_vib_t ctrl_vibs[MAX_CTRL_VIB];
static damping_t dampings[MAX_DAMPING];
static vba_state_t vba;

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

int vba_init(void) {
    if (initialized) return -1;
    vba.n_vibration = 0; vba.n_isolation = 0; vba.n_modal = 0;
    vba.n_ctrl_vib = 0; vba.n_damping = 0;
    vba.total_free_vb = 0; vba.total_active_iso = 0;
    vba.total_modal_ident = 0; vba.total_tmd = 0;
    vba.total_damping_mat = 0;
    for (int i = 0; i < MAX_VIBRATION; i++) vibrations[i].active = 0;
    for (int i = 0; i < MAX_ISOLATION; i++) isolations[i].active = 0;
    for (int i = 0; i < MAX_MODAL; i++) modals[i].active = 0;
    for (int i = 0; i < MAX_CTRL_VIB; i++) ctrl_vibs[i].active = 0;
    for (int i = 0; i < MAX_DAMPING; i++) dampings[i].active = 0;
    initialized = 1;
    print_str("[VBA] Vibration initialized\n");
    return 0;
}

int vba_vibration(int vt_type, int cat, int fre, int frc, int sel, int year) {
    if (vba.n_vibration >= MAX_VIBRATION) return -1;
    vibration_t* v = &vibrations[vba.n_vibration];
    v->vb_id = vba.n_vibration;
    v->vb_type = vt_type;
    v->vb_category = cat;
    v->free_vb = fre;
    v->forced_vb = frc;
    v->self_excited = sel;
    v->year = year;
    v->active = 1;
    vba.total_free_vb += fre;
    vba.n_vibration++;
    print_str("[VBA] Vibration "); print_int(vba.n_vibration - 1);
    print_str(" type="); print_int(vt_type);
    print_str(" cat="); print_int(cat);
    print_str(" fre="); print_int(fre);
    print_str(" frc="); print_int(frc);
    print_str(" sel="); print_int(sel); print_str("\n");
    return vba.n_vibration - 1;
}

int vba_isolation(int it_type, int cat, int act, int pas, int hyb, int year) {
    if (vba.n_isolation >= MAX_ISOLATION) return -1;
    isolation_t* s = &isolations[vba.n_isolation];
    s->iso_id = vba.n_isolation;
    s->iso_type = it_type;
    s->iso_category = cat;
    s->active_iso = act;
    s->passive_iso = pas;
    s->hybrid_iso = hyb;
    s->year = year;
    s->active = 1;
    vba.total_active_iso += act;
    vba.n_isolation++;
    print_str("[VBA] Isolation "); print_int(vba.n_isolation - 1);
    print_str(" type="); print_int(it_type);
    print_str(" cat="); print_int(cat);
    print_str(" act="); print_int(act);
    print_str(" pas="); print_int(pas);
    print_str(" hyb="); print_int(hyb); print_str("\n");
    return vba.n_isolation - 1;
}

int vba_modal(int mt_type, int cat, int idn, int sup, int frq, int year) {
    if (vba.n_modal >= MAX_MODAL) return -1;
    modal_t* m = &modals[vba.n_modal];
    m->md_id = vba.n_modal;
    m->md_type = mt_type;
    m->md_category = cat;
    m->modal_ident = idn;
    m->modal_super = sup;
    m->freq_resp = frq;
    m->year = year;
    m->active = 1;
    vba.total_modal_ident += idn;
    vba.n_modal++;
    print_str("[VBA] Modal "); print_int(vba.n_modal - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" idn="); print_int(idn);
    print_str(" sup="); print_int(sup);
    print_str(" frq="); print_int(frq); print_str("\n");
    return vba.n_modal - 1;
}

int vba_ctrl_vib(int cv_type, int cat, int tmd_v, int bis, int edp, int year) {
    if (vba.n_ctrl_vib >= MAX_CTRL_VIB) return -1;
    control_vib_t* c = &ctrl_vibs[vba.n_ctrl_vib];
    c->ctv_id = vba.n_ctrl_vib;
    c->ctv_type = cv_type;
    c->ctv_category = cat;
    c->tmd = tmd_v;
    c->base_iso = bis;
    c->energy_diss = edp;
    c->year = year;
    c->active = 1;
    vba.total_tmd += tmd_v;
    vba.n_ctrl_vib++;
    print_str("[VBA] Ctrl vib "); print_int(vba.n_ctrl_vib - 1);
    print_str(" type="); print_int(cv_type);
    print_str(" cat="); print_int(cat);
    print_str(" tmd="); print_int(tmd_v);
    print_str(" bis="); print_int(bis);
    print_str(" edp="); print_int(edp); print_str("\n");
    return vba.n_ctrl_vib - 1;
}

int vba_damping(int dt_type, int cat, int dmp_m, int dyn_a, int imp_d, int year) {
    if (vba.n_damping >= MAX_DAMPING) return -1;
    damping_t* d = &dampings[vba.n_damping];
    d->dmp_id = vba.n_damping;
    d->dmp_type = dt_type;
    d->dmp_category = cat;
    d->damping_mat = dmp_m;
    d->dyn_absorber = dyn_a;
    d->impact_dmp = imp_d;
    d->year = year;
    d->active = 1;
    vba.total_damping_mat += dmp_m;
    vba.n_damping++;
    print_str("[VBA] Damping "); print_int(vba.n_damping - 1);
    print_str(" type="); print_int(dt_type);
    print_str(" cat="); print_int(cat);
    print_str(" dmp="); print_int(dmp_m);
    print_str(" dyn="); print_int(dyn_a);
    print_str(" imp="); print_int(imp_d); print_str("\n");
    return vba.n_damping - 1;
}

void vba_vibration_report(void) {
    print_str("[VBA] Vibration report:\n");
    print_str("  Vibration categories: "); print_int(vba.n_vibration); print_str("\n");
    print_str("  Total free vibration: "); print_int(vba.total_free_vb); print_str("\n");
}

void vba_isolation_report(void) {
    print_str("[VBA] Isolation report:\n");
    print_str("  Isolation categories: "); print_int(vba.n_isolation); print_str("\n");
    print_str("  Total active isolation: "); print_int(vba.total_active_iso); print_str("\n");
}

void vba_full_report(void) {
    print_str("[VBA] Full report:\n");
    print_str("  Modal analysis categories: "); print_int(vba.n_modal); print_str("\n");
    print_str("  Total modal identification: "); print_int(vba.total_modal_ident); print_str("\n");
    print_str("  Vibration control categories: "); print_int(vba.n_ctrl_vib); print_str("\n");
    print_str("  Total TMD: "); print_int(vba.total_tmd); print_str("\n");
    print_str("  Damping categories: "); print_int(vba.n_damping); print_str("\n");
    print_str("  Total damping material: "); print_int(vba.total_damping_mat); print_str("\n");
}

void vba_print_state(void) {
    print_str("[VBA] Vb="); print_int(vba.n_vibration);
    print_str(" Is="); print_int(vba.n_isolation);
    print_str(" Md="); print_int(vba.n_modal);
    print_str(" Cv="); print_int(vba.n_ctrl_vib);
    print_str(" Dp="); print_int(vba.n_damping);
    print_str("\n");
}

int main(void) {
    print_str("=== Vibration Admin Demo ===\n\n");
    vba_init();

    print_str("Vibration...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int fre = 55 + (i * 13);
        int frc = 40 + (i * 10);
        int sel = 22 + (i * 5);
        int year = 2020 + (i % 5);
        vba_vibration(type, cat, fre, frc, sel, year);
    }

    print_str("\nIsolation...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int act = 48 + (i * 11);
        int pas = 35 + (i * 8);
        int hyb = 20 + (i * 4);
        int year = 2021 + (i % 4);
        vba_isolation(type, cat, act, pas, hyb, year);
    }

    print_str("\nModal analysis...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int idn = 42 + (i * 10);
        int sup = 28 + (i * 7);
        int frq = 18 + (i * 4);
        int year = 2022 + (i % 3);
        vba_modal(type, cat, idn, sup, frq, year);
    }

    print_str("\nVibration control...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int tmd_v = 35 + (i * 8);
        int bis = 25 + (i * 6);
        int edp = 15 + (i * 3);
        int year = 2023 + (i % 2);
        vba_ctrl_vib(type, cat, tmd_v, bis, edp, year);
    }

    print_str("\nDamping...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int dmp_m = 30 + (i * 7);
        int dyn_a = 22 + (i * 5);
        int imp_d = 12 + (i * 3);
        int year = 2024;
        vba_damping(type, cat, dmp_m, dyn_a, imp_d, year);
    }

    print_str("\nVibration report...\n");
    vba_vibration_report();

    print_str("\nIsolation report...\n");
    vba_isolation_report();

    print_str("\nFull report...\n");
    vba_full_report();

    print_str("\nFinal state...\n");
    vba_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
