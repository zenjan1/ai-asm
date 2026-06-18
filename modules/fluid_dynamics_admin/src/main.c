/* fluid_dynamics_admin: Fluid dynamics administration system (v1.0)
 * Fluid flow, fluid machinery, fluid mechanics, fluid control, fluid applications
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

#define MAX_FLOW       16
#define MAX_MACHINERY  14
#define MAX_MECHANICS  12
#define MAX_CONTROL    10
#define MAX_APP        10

typedef struct {
    int    flow_id;
    int    flow_type;
    int    flow_category;
    int    laminar;
    int    turbulent;
    int    transitional;
    int    year;
    int    active;
} flow_t;

typedef struct {
    int    mach_id;
    int    mach_type;
    int    mach_category;
    int    pump;
    int    fan;
    int    compressor;
    int    year;
    int    active;
} machinery_t;

typedef struct {
    int    mech_id;
    int    mech_type;
    int    mech_category;
    int    bernoulli;
    int    navier_stokes;
    int    boundary_layer;
    int    year;
    int    active;
} mechanics_t;

typedef struct {
    int    ctrl_id;
    int    ctrl_type;
    int    ctrl_category;
    int    valve;
    int    pipeline;
    int    flow_ctrl;
    int    year;
    int    active;
} control_t;

typedef struct {
    int    app_id;
    int    app_type;
    int    app_category;
    int    hydraulic_trans;
    int    hydraulic_coupling;
    int    torque_conv;
    int    year;
    int    active;
} app_t;

typedef struct {
    int    n_flow;
    int    n_machinery;
    int    n_mechanics;
    int    n_control;
    int    n_app;
    int    total_laminar;
    int    total_pump;
    int    total_bernoulli;
    int    total_valve;
    int    total_hyd_trans;
} fda_state_t;

static flow_t flows[MAX_FLOW];
static machinery_t machineries[MAX_MACHINERY];
static mechanics_t mechanics_list[MAX_MECHANICS];
static control_t controls[MAX_CONTROL];
static app_t apps[MAX_APP];
static fda_state_t fda;

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

int fda_init(void) {
    if (initialized) return -1;
    fda.n_flow = 0; fda.n_machinery = 0; fda.n_mechanics = 0;
    fda.n_control = 0; fda.n_app = 0;
    fda.total_laminar = 0; fda.total_pump = 0;
    fda.total_bernoulli = 0; fda.total_valve = 0;
    fda.total_hyd_trans = 0;
    for (int i = 0; i < MAX_FLOW; i++) flows[i].active = 0;
    for (int i = 0; i < MAX_MACHINERY; i++) machineries[i].active = 0;
    for (int i = 0; i < MAX_MECHANICS; i++) mechanics_list[i].active = 0;
    for (int i = 0; i < MAX_CONTROL; i++) controls[i].active = 0;
    for (int i = 0; i < MAX_APP; i++) apps[i].active = 0;
    initialized = 1;
    print_str("[FDA] Fluid dynamics initialized\n");
    return 0;
}

int fda_flow(int fl_type, int cat, int lam, int turb, int trans, int year) {
    if (fda.n_flow >= MAX_FLOW) return -1;
    flow_t* f = &flows[fda.n_flow];
    f->flow_id = fda.n_flow;
    f->flow_type = fl_type;
    f->flow_category = cat;
    f->laminar = lam;
    f->turbulent = turb;
    f->transitional = trans;
    f->year = year;
    f->active = 1;
    fda.total_laminar += lam;
    fda.n_flow++;
    print_str("[FDA] Flow "); print_int(fda.n_flow - 1);
    print_str(" type="); print_int(fl_type);
    print_str(" cat="); print_int(cat);
    print_str(" lam="); print_int(lam);
    print_str(" trb="); print_int(turb);
    print_str(" trn="); print_int(trans); print_str("\n");
    return fda.n_flow - 1;
}

int fda_machinery(int mc_type, int cat, int pmp, int fn, int comp, int year) {
    if (fda.n_machinery >= MAX_MACHINERY) return -1;
    machinery_t* m = &machineries[fda.n_machinery];
    m->mach_id = fda.n_machinery;
    m->mach_type = mc_type;
    m->mach_category = cat;
    m->pump = pmp;
    m->fan = fn;
    m->compressor = comp;
    m->year = year;
    m->active = 1;
    fda.total_pump += pmp;
    fda.n_machinery++;
    print_str("[FDA] Machinery "); print_int(fda.n_machinery - 1);
    print_str(" type="); print_int(mc_type);
    print_str(" cat="); print_int(cat);
    print_str(" pmp="); print_int(pmp);
    print_str(" fan="); print_int(fn);
    print_str(" cmp="); print_int(comp); print_str("\n");
    return fda.n_machinery - 1;
}

int fda_mechanics(int me_type, int cat, int bern, int nav, int bnd, int year) {
    if (fda.n_mechanics >= MAX_MECHANICS) return -1;
    mechanics_t* m = &mechanics_list[fda.n_mechanics];
    m->mech_id = fda.n_mechanics;
    m->mech_type = me_type;
    m->mech_category = cat;
    m->bernoulli = bern;
    m->navier_stokes = nav;
    m->boundary_layer = bnd;
    m->year = year;
    m->active = 1;
    fda.total_bernoulli += bern;
    fda.n_mechanics++;
    print_str("[FDA] Mechanics "); print_int(fda.n_mechanics - 1);
    print_str(" type="); print_int(me_type);
    print_str(" cat="); print_int(cat);
    print_str(" brn="); print_int(bern);
    print_str(" nav="); print_int(nav);
    print_str(" bnd="); print_int(bnd); print_str("\n");
    return fda.n_mechanics - 1;
}

int fda_control(int ct_type, int cat, int vlv, int pipe, int flc, int year) {
    if (fda.n_control >= MAX_CONTROL) return -1;
    control_t* c = &controls[fda.n_control];
    c->ctrl_id = fda.n_control;
    c->ctrl_type = ct_type;
    c->ctrl_category = cat;
    c->valve = vlv;
    c->pipeline = pipe;
    c->flow_ctrl = flc;
    c->year = year;
    c->active = 1;
    fda.total_valve += vlv;
    fda.n_control++;
    print_str("[FDA] Control "); print_int(fda.n_control - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" vlv="); print_int(vlv);
    print_str(" pip="); print_int(pipe);
    print_str(" flc="); print_int(flc); print_str("\n");
    return fda.n_control - 1;
}

int fda_app(int ap_type, int cat, int hyd_tr, int hyd_cp, int trq, int year) {
    if (fda.n_app >= MAX_APP) return -1;
    app_t* a = &apps[fda.n_app];
    a->app_id = fda.n_app;
    a->app_type = ap_type;
    a->app_category = cat;
    a->hydraulic_trans = hyd_tr;
    a->hydraulic_coupling = hyd_cp;
    a->torque_conv = trq;
    a->year = year;
    a->active = 1;
    fda.total_hyd_trans += hyd_tr;
    fda.n_app++;
    print_str("[FDA] App "); print_int(fda.n_app - 1);
    print_str(" type="); print_int(ap_type);
    print_str(" cat="); print_int(cat);
    print_str(" htr="); print_int(hyd_tr);
    print_str(" hcp="); print_int(hyd_cp);
    print_str(" tqc="); print_int(trq); print_str("\n");
    return fda.n_app - 1;
}

void fda_flow_report(void) {
    print_str("[FDA] Flow report:\n");
    print_str("  Fluid flow categories: "); print_int(fda.n_flow); print_str("\n");
    print_str("  Total laminar flow: "); print_int(fda.total_laminar); print_str("\n");
}

void fda_machinery_report(void) {
    print_str("[FDA] Machinery report:\n");
    print_str("  Fluid machinery categories: "); print_int(fda.n_machinery); print_str("\n");
    print_str("  Total pumps: "); print_int(fda.total_pump); print_str("\n");
}

void fda_app_report(void) {
    print_str("[FDA] App report:\n");
    print_str("  Fluid mechanics categories: "); print_int(fda.n_mechanics); print_str("\n");
    print_str("  Total Bernoulli: "); print_int(fda.total_bernoulli); print_str("\n");
    print_str("  Fluid control categories: "); print_int(fda.n_control); print_str("\n");
    print_str("  Total valves: "); print_int(fda.total_valve); print_str("\n");
    print_str("  Fluid application categories: "); print_int(fda.n_app); print_str("\n");
    print_str("  Total hydraulic transmission: "); print_int(fda.total_hyd_trans); print_str("\n");
}

void fda_print_state(void) {
    print_str("[FDA] Fl="); print_int(fda.n_flow);
    print_str(" Mc="); print_int(fda.n_machinery);
    print_str(" Me="); print_int(fda.n_mechanics);
    print_str(" Ct="); print_int(fda.n_control);
    print_str(" Ap="); print_int(fda.n_app);
    print_str("\n");
}

int main(void) {
    print_str("=== Fluid Dynamics Admin Demo ===\n\n");
    fda_init();

    print_str("Fluid flow...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int lam = 55 + (i * 13);
        int trb = 40 + (i * 10);
        int trn = 22 + (i * 5);
        int year = 2020 + (i % 5);
        fda_flow(type, cat, lam, trb, trn, year);
    }

    print_str("\nFluid machinery...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pmp = 48 + (i * 12);
        int fn = 35 + (i * 8);
        int cmp = 20 + (i * 5);
        int year = 2021 + (i % 4);
        fda_machinery(type, cat, pmp, fn, cmp, year);
    }

    print_str("\nFluid mechanics...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int brn = 42 + (i * 10);
        int nav = 28 + (i * 7);
        int bnd = 18 + (i * 4);
        int year = 2022 + (i % 3);
        fda_mechanics(type, cat, brn, nav, bnd, year);
    }

    print_str("\nFluid control...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int vlv = 35 + (i * 8);
        int pip = 25 + (i * 6);
        int flc = 15 + (i * 3);
        int year = 2023 + (i % 2);
        fda_control(type, cat, vlv, pip, flc, year);
    }

    print_str("\nFluid applications...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int htr = 30 + (i * 7);
        int hcp = 22 + (i * 5);
        int tqc = 12 + (i * 3);
        int year = 2024;
        fda_app(type, cat, htr, hcp, tqc, year);
    }

    print_str("\nFlow report...\n");
    fda_flow_report();

    print_str("\nMachinery report...\n");
    fda_machinery_report();

    print_str("\nApp report...\n");
    fda_app_report();

    print_str("\nFinal state...\n");
    fda_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
