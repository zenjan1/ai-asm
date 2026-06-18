/* theoretical_mechanics_admin: Theoretical mechanics administration (v1.0)
 * Statics, kinematics, particle dynamics, rigid body dynamics, analytical mechanics
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

#define MAX_STATICS     16
#define MAX_KINEMAT     14
#define MAX_PARTICLE    12
#define MAX_RIGID       10
#define MAX_ANALYTIC    10

typedef struct {
    int    st_id;
    int    st_type;
    int    st_category;
    int    force_comp;
    int    moment;
    int    equilibrium;
    int    year;
    int    active;
} statics_t;

typedef struct {
    int    kn_id;
    int    kn_type;
    int    kn_category;
    int    point_motion;
    int    rigid_motion;
    int    vel_comp;
    int    year;
    int    active;
} kinematic_t;

typedef struct {
    int    pd_id;
    int    pd_type;
    int    pd_category;
    int    newton_law;
    int    momentum_th;
    int    energy_th;
    int    year;
    int    active;
} particle_dyn_t;

typedef struct {
    int    rb_id;
    int    rb_type;
    int    rb_category;
    int    moment_inertia;
    int    angular_mom;
    int    d_alembert;
    int    year;
    int    active;
} rigid_body_t;

typedef struct {
    int    an_id;
    int    an_type;
    int    an_category;
    int    virtual_disp;
    int    lagrange_eq;
    int    hamilton_pr;
    int    year;
    int    active;
} analytic_mech_t;

typedef struct {
    int    n_statics;
    int    n_kinematic;
    int    n_particle_dyn;
    int    n_rigid_body;
    int    n_analytic;
    int    total_force_comp;
    int    total_point_motion;
    int    total_newton_law;
    int    total_moment_inertia;
    int    total_virtual_disp;
} tma_state_t;

static statics_t statics_list[MAX_STATICS];
static kinematic_t kinematics[MAX_KINEMAT];
static particle_dyn_t particle_dyns[MAX_PARTICLE];
static rigid_body_t rigid_bodies[MAX_RIGID];
static analytic_mech_t analytic_mechs[MAX_ANALYTIC];
static tma_state_t tma;

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

int tma_init(void) {
    if (initialized) return -1;
    tma.n_statics = 0; tma.n_kinematic = 0; tma.n_particle_dyn = 0;
    tma.n_rigid_body = 0; tma.n_analytic = 0;
    tma.total_force_comp = 0; tma.total_point_motion = 0;
    tma.total_newton_law = 0; tma.total_moment_inertia = 0;
    tma.total_virtual_disp = 0;
    for (int i = 0; i < MAX_STATICS; i++) statics_list[i].active = 0;
    for (int i = 0; i < MAX_KINEMAT; i++) kinematics[i].active = 0;
    for (int i = 0; i < MAX_PARTICLE; i++) particle_dyns[i].active = 0;
    for (int i = 0; i < MAX_RIGID; i++) rigid_bodies[i].active = 0;
    for (int i = 0; i < MAX_ANALYTIC; i++) analytic_mechs[i].active = 0;
    initialized = 1;
    print_str("[TMA] Theoretical mechanics initialized\n");
    return 0;
}

int tma_statics(int st_type, int cat, int frc, int mom, int eql, int year) {
    if (tma.n_statics >= MAX_STATICS) return -1;
    statics_t* s = &statics_list[tma.n_statics];
    s->st_id = tma.n_statics;
    s->st_type = st_type;
    s->st_category = cat;
    s->force_comp = frc;
    s->moment = mom;
    s->equilibrium = eql;
    s->year = year;
    s->active = 1;
    tma.total_force_comp += frc;
    tma.n_statics++;
    print_str("[TMA] Statics "); print_int(tma.n_statics - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" frc="); print_int(frc);
    print_str(" mom="); print_int(mom);
    print_str(" eql="); print_int(eql); print_str("\n");
    return tma.n_statics - 1;
}

int tma_kinematic(int kn_type, int cat, int pnt, int rgd, int vel, int year) {
    if (tma.n_kinematic >= MAX_KINEMAT) return -1;
    kinematic_t* k = &kinematics[tma.n_kinematic];
    k->kn_id = tma.n_kinematic;
    k->kn_type = kn_type;
    k->kn_category = cat;
    k->point_motion = pnt;
    k->rigid_motion = rgd;
    k->vel_comp = vel;
    k->year = year;
    k->active = 1;
    tma.total_point_motion += pnt;
    tma.n_kinematic++;
    print_str("[TMA] Kinematic "); print_int(tma.n_kinematic - 1);
    print_str(" type="); print_int(kn_type);
    print_str(" cat="); print_int(cat);
    print_str(" pnt="); print_int(pnt);
    print_str(" rgd="); print_int(rgd);
    print_str(" vel="); print_int(vel); print_str("\n");
    return tma.n_kinematic - 1;
}

int tma_particle_dyn(int pd_type, int cat, int nwt, int mmt, int eng, int year) {
    if (tma.n_particle_dyn >= MAX_PARTICLE) return -1;
    particle_dyn_t* p = &particle_dyns[tma.n_particle_dyn];
    p->pd_id = tma.n_particle_dyn;
    p->pd_type = pd_type;
    p->pd_category = cat;
    p->newton_law = nwt;
    p->momentum_th = mmt;
    p->energy_th = eng;
    p->year = year;
    p->active = 1;
    tma.total_newton_law += nwt;
    tma.n_particle_dyn++;
    print_str("[TMA] Particle dyn "); print_int(tma.n_particle_dyn - 1);
    print_str(" type="); print_int(pd_type);
    print_str(" cat="); print_int(cat);
    print_str(" nwt="); print_int(nwt);
    print_str(" mmt="); print_int(mmt);
    print_str(" eng="); print_int(eng); print_str("\n");
    return tma.n_particle_dyn - 1;
}

int tma_rigid_body(int rb_type, int cat, int moi, int ang_m, int dal, int year) {
    if (tma.n_rigid_body >= MAX_RIGID) return -1;
    rigid_body_t* r = &rigid_bodies[tma.n_rigid_body];
    r->rb_id = tma.n_rigid_body;
    r->rb_type = rb_type;
    r->rb_category = cat;
    r->moment_inertia = moi;
    r->angular_mom = ang_m;
    r->d_alembert = dal;
    r->year = year;
    r->active = 1;
    tma.total_moment_inertia += moi;
    tma.n_rigid_body++;
    print_str("[TMA] Rigid body "); print_int(tma.n_rigid_body - 1);
    print_str(" type="); print_int(rb_type);
    print_str(" cat="); print_int(cat);
    print_str(" moi="); print_int(moi);
    print_str(" ang="); print_int(ang_m);
    print_str(" dal="); print_int(dal); print_str("\n");
    return tma.n_rigid_body - 1;
}

int tma_analytic(int an_type, int cat, int vd, int lag, int ham, int year) {
    if (tma.n_analytic >= MAX_ANALYTIC) return -1;
    analytic_mech_t* a = &analytic_mechs[tma.n_analytic];
    a->an_id = tma.n_analytic;
    a->an_type = an_type;
    a->an_category = cat;
    a->virtual_disp = vd;
    a->lagrange_eq = lag;
    a->hamilton_pr = ham;
    a->year = year;
    a->active = 1;
    tma.total_virtual_disp += vd;
    tma.n_analytic++;
    print_str("[TMA] Analytic "); print_int(tma.n_analytic - 1);
    print_str(" type="); print_int(an_type);
    print_str(" cat="); print_int(cat);
    print_str(" vdp="); print_int(vd);
    print_str(" lag="); print_int(lag);
    print_str(" ham="); print_int(ham); print_str("\n");
    return tma.n_analytic - 1;
}

void tma_statics_report(void) {
    print_str("[TMA] Statics report:\n");
    print_str("  Statics categories: "); print_int(tma.n_statics); print_str("\n");
    print_str("  Total force composition: "); print_int(tma.total_force_comp); print_str("\n");
}

void tma_kinematic_report(void) {
    print_str("[TMA] Kinematic report:\n");
    print_str("  Kinematic categories: "); print_int(tma.n_kinematic); print_str("\n");
    print_str("  Total point motion: "); print_int(tma.total_point_motion); print_str("\n");
}

void tma_full_report(void) {
    print_str("[TMA] Full report:\n");
    print_str("  Particle dynamics categories: "); print_int(tma.n_particle_dyn); print_str("\n");
    print_str("  Total Newton's law: "); print_int(tma.total_newton_law); print_str("\n");
    print_str("  Rigid body dynamics categories: "); print_int(tma.n_rigid_body); print_str("\n");
    print_str("  Total moment of inertia: "); print_int(tma.total_moment_inertia); print_str("\n");
    print_str("  Analytical mechanics categories: "); print_int(tma.n_analytic); print_str("\n");
    print_str("  Total virtual displacement: "); print_int(tma.total_virtual_disp); print_str("\n");
}

void tma_print_state(void) {
    print_str("[TMA] St="); print_int(tma.n_statics);
    print_str(" Kn="); print_int(tma.n_kinematic);
    print_str(" Pd="); print_int(tma.n_particle_dyn);
    print_str(" Rb="); print_int(tma.n_rigid_body);
    print_str(" An="); print_int(tma.n_analytic);
    print_str("\n");
}

int main(void) {
    print_str("=== Theoretical Mechanics Admin Demo ===\n\n");
    tma_init();

    print_str("Statics...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int frc = 55 + (i * 13);
        int mom = 40 + (i * 10);
        int eql = 22 + (i * 5);
        int year = 2020 + (i % 5);
        tma_statics(type, cat, frc, mom, eql, year);
    }

    print_str("\nKinematics...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pnt = 48 + (i * 11);
        int rgd = 35 + (i * 8);
        int vel = 20 + (i * 4);
        int year = 2021 + (i % 4);
        tma_kinematic(type, cat, pnt, rgd, vel, year);
    }

    print_str("\nParticle dynamics...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int nwt = 42 + (i * 10);
        int mmt = 28 + (i * 7);
        int eng = 18 + (i * 4);
        int year = 2022 + (i % 3);
        tma_particle_dyn(type, cat, nwt, mmt, eng, year);
    }

    print_str("\nRigid body dynamics...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int moi = 35 + (i * 8);
        int ang_m = 25 + (i * 6);
        int dal = 15 + (i * 3);
        int year = 2023 + (i % 2);
        tma_rigid_body(type, cat, moi, ang_m, dal, year);
    }

    print_str("\nAnalytical mechanics...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int vd = 30 + (i * 7);
        int lag = 22 + (i * 5);
        int ham = 12 + (i * 3);
        int year = 2024;
        tma_analytic(type, cat, vd, lag, ham, year);
    }

    print_str("\nStatics report...\n");
    tma_statics_report();

    print_str("\nKinematic report...\n");
    tma_kinematic_report();

    print_str("\nFull report...\n");
    tma_full_report();

    print_str("\nFinal state...\n");
    tma_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
