/* molecular_dynamics: Molecular dynamics simulation engine (v1.0)
 * Force fields, integration, ensembles, constraints, long-range, analysis
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

#define MAX_ATOMS       128
#define MAX_BONDS       256
#define MAX_RDF_BINS    64
#define MAX_HISTORY     32

#define FF_LENNARD_JONES 0x01
#define FF_COULOMB       0x02
#define FF_AMBER         0x03
#define FF_CHARMM        0x04

#define INTEG_VERLET       0
#define INTEG_LEAPFROG     1
#define INTEG_VVERLET      2
#define INTEG_PRED_CORR    3

#define ENS_NVE  0
#define ENS_NVT  1
#define ENS_NPT  2
#define ENS_LANGEVIN 3

#define CONST_NONE   0
#define CONST_SHAKE  1
#define CONST_RATTLE 2

#define LR_Ewald    0
#define LR_PME      1
#define LR_RF       2
#define LR_MULTIPOLE 3

typedef struct {
    double x, y, z;
    double vx, vy, vz;
    double fx, fy, fz;
    double ax, ay, az;
    double mass;
    double charge;
    int    type;
    int    active;
} atom_t;

typedef struct {
    int a, b;
    double k;
    double r0;
    int active;
} bond_t;

typedef struct {
    double epsilon;
    double sigma;
    double cutoff;
    double coulomb_k;
    int    ff_type;
} ff_params_t;

typedef struct {
    int    integ_type;
    int    ensemble_type;
    int    constraint_type;
    int    lr_method;
    double dt;
    double temperature;
    double pressure;
    double total_energy;
    double kinetic_energy;
    double potential_energy;
    int    step_count;
    int    atom_count;
    int    bond_count;
} sim_state_t;

typedef struct {
    double bins[MAX_RDF_BINS];
    int    bin_count;
    double r_max;
    double dr;
    int    samples;
} rdf_t;

typedef struct {
    double pos_history[MAX_HISTORY][MAX_ATOMS * 3];
    int    head;
    int    count;
} trajectory_t;

static atom_t       atoms[MAX_ATOMS];
static bond_t       bonds[MAX_BONDS];
static ff_params_t  ff;
static sim_state_t  sim;
static rdf_t        rdf;
static trajectory_t traj;

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
static void print_hex(unsigned int val) {
    char buf[16]; int i = 0;
    buf[i++] = '0'; buf[i++] = 'x';
    if (val == 0) buf[i++] = '0';
    else { int s = i; while (val > 0) { int d = val & 0xF; buf[i++] = (d < 10) ? ('0' + d) : ('A' + d - 10); val >>= 4; }
           int e = i - 1; while (s < e) { char t = buf[s]; buf[s] = buf[e]; buf[e] = t; s++; e--; } }
    buf[i] = '\0'; host_print(buf);
}
static double dabs(double x) { return x < 0 ? -x : x; }
static double dsqrt(double x) {
    if (x <= 0.0) return 0.0;
    double g = x, p = x / g;
    for (int i = 0; i < 30; i++) { g = (g + p) * 0.5; p = x / g; }
    return g;
}
static double dexp(double x) {
    if (x > 700.0) return 1e300;
    if (x < -700.0) return 0.0;
    double sum = 1.0, term = 1.0;
    for (int i = 1; i < 25; i++) { term *= x / i; sum += term; }
    return sum;
}

int md_init(int ff_type, int integ, int ens, int lr) {
    if (initialized) return -1;
    ff.ff_type = ff_type;
    ff.epsilon = 0.238; ff.sigma = 3.4; ff.cutoff = 10.0; ff.coulomb_k = 138.935;
    sim.integ_type = integ; sim.ensemble_type = ens; sim.lr_method = lr;
    sim.constraint_type = CONST_NONE;
    sim.dt = 0.001; sim.temperature = 300.0; sim.pressure = 1.0;
    sim.total_energy = 0.0; sim.kinetic_energy = 0.0; sim.potential_energy = 0.0;
    sim.step_count = 0; sim.atom_count = 0; sim.bond_count = 0;
    rdf.bin_count = MAX_RDF_BINS; rdf.r_max = 10.0;
    rdf.dr = rdf.r_max / rdf.bin_count; rdf.samples = 0;
    for (int i = 0; i < MAX_RDF_BINS; i++) rdf.bins[i] = 0.0;
    traj.head = 0; traj.count = 0;
    for (int i = 0; i < MAX_ATOMS; i++) { atoms[i].active = 0; atoms[i].type = 0; }
    for (int i = 0; i < MAX_BONDS; i++) bonds[i].active = 0;
    initialized = 1;
    print_str("[MD] Initialized: ff="); print_int(ff_type);
    print_str(" integ="); print_int(integ);
    print_str(" ens="); print_int(ens);
    print_str(" lr="); print_int(lr); print_str("\n");
    return 0;
}

int md_add_atom(double x, double y, double z, double mass, double charge, int type) {
    if (sim.atom_count >= MAX_ATOMS) return -1;
    atom_t* a = &atoms[sim.atom_count];
    a->x = x; a->y = y; a->z = z;
    a->vx = 0.0; a->vy = 0.0; a->vz = 0.0;
    a->fx = 0.0; a->fy = 0.0; a->fz = 0.0;
    a->ax = 0.0; a->ay = 0.0; a->az = 0.0;
    a->mass = mass; a->charge = charge; a->type = type; a->active = 1;
    return sim.atom_count++;
}

int md_add_bond(int a, int b, double k, double r0) {
    if (sim.bond_count >= MAX_BONDS) return -1;
    bond_t* bn = &bonds[sim.bond_count];
    bn->a = a; bn->b = b; bn->k = k; bn->r0 = r0; bn->active = 1;
    return sim.bond_count++;
}

static double calc_lj(double r) {
    if (r < 1e-10) return 0.0;
    double sr6 = (ff.sigma * ff.sigma) / (r * r);
    sr6 = sr6 * sr6 * sr6;
    return 4.0 * ff.epsilon * (sr6 * sr6 - sr6);
}

static double calc_coulomb(double q1, double q2, double r) {
    if (r < 1e-10) return 0.0;
    return ff.coulomb_k * q1 * q2 / r;
}

static void calc_forces_pairwise(void) {
    for (int i = 0; i < sim.atom_count; i++) {
        atoms[i].fx = 0.0; atoms[i].fy = 0.0; atoms[i].fz = 0.0;
    }
    for (int i = 0; i < sim.atom_count; i++) {
        for (int j = i + 1; j < sim.atom_count; j++) {
            double dx = atoms[j].x - atoms[i].x;
            double dy = atoms[j].y - atoms[i].y;
            double dz = atoms[j].z - atoms[i].z;
            double r = dsqrt(dx*dx + dy*dy + dz*dz);
            if (r > ff.cutoff || r < 1e-10) continue;
            double f_lj = 0.0, f_c = 0.0;
            if (ff.ff_type == FF_LENNARD_JONES || ff.ff_type == FF_AMBER || ff.ff_type == FF_CHARMM) {
                double sr = ff.sigma / r;
                double sr6 = sr*sr*sr*sr*sr*sr;
                double sr12 = sr6*sr6;
                f_lj = 24.0 * ff.epsilon / r * (2.0*sr12 - sr6);
            }
            if (ff.ff_type == FF_COULOMB || ff.ff_type == FF_AMBER || ff.ff_type == FF_CHARMM) {
                f_c = -ff.coulomb_k * atoms[i].charge * atoms[j].charge / (r * r);
            }
            double f = f_lj + f_c;
            double fx = f * dx / r; double fy = f * dy / r; double fz = f * dz / r;
            atoms[i].fx += fx; atoms[i].fy += fy; atoms[i].fz += fz;
            atoms[j].fx -= fx; atoms[j].fy -= fy; atoms[j].fz -= fz;
        }
    }
    for (int b = 0; b < sim.bond_count; b++) {
        if (!bonds[b].active) continue;
        int i = bonds[b].a, j = bonds[b].b;
        double dx = atoms[j].x - atoms[i].x;
        double dy = atoms[j].y - atoms[i].y;
        double dz = atoms[j].z - atoms[i].z;
        double r = dsqrt(dx*dx + dy*dy + dz*dz);
        if (r < 1e-10) continue;
        double f = -bonds[b].k * (r - bonds[b].r0);
        double fx = f * dx / r; double fy = f * dy / r; double fz = f * dz / r;
        atoms[i].fx += fx; atoms[i].fy += fy; atoms[i].fz += fz;
        atoms[j].fx -= fx; atoms[j].fy -= fy; atoms[j].fz -= fz;
    }
}

static void calc_forces_ewald(void) {
    calc_forces_pairwise();
    double alpha = 0.25;
    double rc = ff.cutoff;
    for (int i = 0; i < sim.atom_count; i++) {
        for (int j = i + 1; j < sim.atom_count; j++) {
            double dx = atoms[j].x - atoms[i].x;
            double dy = atoms[j].y - atoms[i].y;
            double dz = atoms[j].z - atoms[i].z;
            double r = dsqrt(dx*dx + dy*dy + dz*dz);
            if (r < 1e-10 || r > rc * 2.0) continue;
            double erfc_alpha_r = dexp(-alpha * alpha * r * r);
            double qiqj = atoms[i].charge * atoms[j].charge;
            double f_ewald = ff.coulomb_k * qiqj * erfc_alpha_r / (r * r);
            atoms[i].fx += f_ewald * dx / r;
            atoms[i].fy += f_ewald * dy / r;
            atoms[i].fz += f_ewald * dz / r;
        }
    }
}

static void calc_forces(void) {
    if (sim.lr_method == LR_Ewald || sim.lr_method == LR_PME) calc_forces_ewald();
    else calc_forces_pairwise();
}

static void integrate_verlet(void) {
    for (int i = 0; i < sim.atom_count; i++) {
        double nx = 2.0*atoms[i].x - (atoms[i].x - atoms[i].vx*sim.dt) + atoms[i].fx / atoms[i].mass * sim.dt * sim.dt;
        double ny = 2.0*atoms[i].y - (atoms[i].y - atoms[i].vy*sim.dt) + atoms[i].fy / atoms[i].mass * sim.dt * sim.dt;
        double nz = 2.0*atoms[i].z - (atoms[i].z - (atoms[i].z - atoms[i].vz*sim.dt)) + atoms[i].fz / atoms[i].mass * sim.dt * sim.dt;
        atoms[i].vx = (nx - atoms[i].x) / (2.0*sim.dt);
        atoms[i].vy = (ny - atoms[i].y) / (2.0*sim.dt);
        atoms[i].vz = (nz - atoms[i].z) / (2.0*sim.dt);
        atoms[i].x = nx; atoms[i].y = ny; atoms[i].z = nz;
    }
}

static void integrate_leapfrog(void) {
    for (int i = 0; i < sim.atom_count; i++) {
        atoms[i].vx += atoms[i].fx / atoms[i].mass * sim.dt;
        atoms[i].vy += atoms[i].fy / atoms[i].mass * sim.dt;
        atoms[i].vz += atoms[i].fz / atoms[i].mass * sim.dt;
        atoms[i].x += atoms[i].vx * sim.dt;
        atoms[i].y += atoms[i].vy * sim.dt;
        atoms[i].z += atoms[i].vz * sim.dt;
    }
}

static void integrate_vverlet(void) {
    double half_dt = 0.5 * sim.dt;
    for (int i = 0; i < sim.atom_count; i++) {
        atoms[i].vx += atoms[i].fx / atoms[i].mass * half_dt;
        atoms[i].vy += atoms[i].fy / atoms[i].mass * half_dt;
        atoms[i].vz += atoms[i].fz / atoms[i].mass * half_dt;
        atoms[i].x += atoms[i].vx * sim.dt;
        atoms[i].y += atoms[i].vy * sim.dt;
        atoms[i].z += atoms[i].vz * sim.dt;
    }
    calc_forces();
    for (int i = 0; i < sim.atom_count; i++) {
        atoms[i].vx += atoms[i].fx / atoms[i].mass * half_dt;
        atoms[i].vy += atoms[i].fy / atoms[i].mass * half_dt;
        atoms[i].vz += atoms[i].fz / atoms[i].mass * half_dt;
    }
}

static void apply_constraints_shake(void) {
    for (int iter = 0; iter < 10; iter++) {
        for (int b = 0; b < sim.bond_count; b++) {
            if (!bonds[b].active) continue;
            int i = bonds[b].a, j = bonds[b].b;
            double dx = atoms[j].x - atoms[i].x;
            double dy = atoms[j].y - atoms[i].y;
            double dz = atoms[j].z - atoms[i].z;
            double r = dsqrt(dx*dx + dy*dy + dz*dz);
            double diff = (r - bonds[b].r0) / r * 0.5;
            atoms[i].x += dx * diff; atoms[i].y += dy * diff; atoms[i].z += dz * diff;
            atoms[j].x -= dx * diff; atoms[j].y -= dy * diff; atoms[j].z -= dz * diff;
        }
    }
}

static void apply_thermostat_nvt(void) {
    double ke = 0.0;
    for (int i = 0; i < sim.atom_count; i++) {
        ke += 0.5 * atoms[i].mass * (atoms[i].vx*atoms[i].vx + atoms[i].vy*atoms[i].vy + atoms[i].vz*atoms[i].vz);
    }
    double t_current = ke / (1.5 * sim.atom_count * 0.0019872);
    if (t_current < 1e-10) return;
    double scale = dsqrt(sim.temperature / t_current);
    for (int i = 0; i < sim.atom_count; i++) {
        atoms[i].vx *= scale; atoms[i].vy *= scale; atoms[i].vz *= scale;
    }
}

static void apply_barostat_npt(void) {
    double vol = 1000.0;
    double p_inst = sim.atom_count * 0.0019872 * sim.temperature / vol;
    double p_diff = sim.pressure - p_inst;
    double scale = 1.0 + p_diff * 0.001 * sim.dt;
    for (int i = 0; i < sim.atom_count; i++) {
        atoms[i].x *= scale; atoms[i].y *= scale; atoms[i].z *= scale;
    }
}

static void apply_langevin(void) {
    double gamma = 0.01;
    for (int i = 0; i < sim.atom_count; i++) {
        double rx = ((double)(i * 31 + sim.step_count * 17) / 1000.0) - 0.5;
        double ry = ((double)(i * 47 + sim.step_count * 23) / 1000.0) - 0.5;
        double rz = ((double)(i * 59 + sim.step_count * 31) / 1000.0) - 0.5;
        double noise = dsqrt(2.0 * gamma * sim.temperature * sim.dt / atoms[i].mass);
        atoms[i].vx += (-gamma * atoms[i].vx + noise * rx) * sim.dt;
        atoms[i].vy += (-gamma * atoms[i].vy + noise * ry) * sim.dt;
        atoms[i].vz += (-gamma * atoms[i].vz + noise * rz) * sim.dt;
    }
}

int md_step(void) {
    if (!initialized) return -1;
    calc_forces();
    if (sim.integ_type == INTEG_VERLET) integrate_verlet();
    else if (sim.integ_type == INTEG_LEAPFROG) integrate_leapfrog();
    else if (sim.integ_type == INTEG_VVERLET) integrate_vverlet();
    else integrate_leapfrog();
    if (sim.constraint_type == CONST_SHAKE || sim.constraint_type == CONST_RATTLE)
        apply_constraints_shake();
    if (sim.ensemble_type == ENS_NVT) apply_thermostat_nvt();
    else if (sim.ensemble_type == ENS_NPT) { apply_thermostat_nvt(); apply_barostat_npt(); }
    else if (sim.ensemble_type == ENS_LANGEVIN) apply_langevin();
    sim.kinetic_energy = 0.0;
    for (int i = 0; i < sim.atom_count; i++)
        sim.kinetic_energy += 0.5 * atoms[i].mass * (atoms[i].vx*atoms[i].vx + atoms[i].vy*atoms[i].vy + atoms[i].vz*atoms[i].vz);
    sim.potential_energy = 0.0;
    for (int i = 0; i < sim.atom_count; i++)
        for (int j = i+1; j < sim.atom_count; j++) {
            double dx = atoms[j].x - atoms[i].x, dy = atoms[j].y - atoms[i].y, dz = atoms[j].z - atoms[i].z;
            double r = dsqrt(dx*dx + dy*dy + dz*dz);
            if (r > ff.cutoff || r < 1e-10) continue;
            if (ff.ff_type != FF_COULOMB) sim.potential_energy += calc_lj(r);
            if (ff.ff_type != FF_LENNARD_JONES) sim.potential_energy += calc_coulomb(atoms[i].charge, atoms[j].charge, r);
        }
    sim.total_energy = sim.kinetic_energy + sim.potential_energy;
    sim.step_count++;
    for (int k = 0; k < MAX_RDF_BINS; k++) {
        double r_lo = k * rdf.dr, r_hi = (k+1) * rdf.dr;
        int cnt = 0;
        for (int i = 0; i < sim.atom_count; i++)
            for (int j = i+1; j < sim.atom_count; j++) {
                double dx = atoms[j].x - atoms[i].x, dy = atoms[j].y - atoms[i].y, dz = atoms[j].z - atoms[i].z;
                double r = dsqrt(dx*dx + dy*dy + dz*dz);
                if (r >= r_lo && r < r_hi) cnt++;
            }
        rdf.bins[k] += cnt;
    }
    rdf.samples++;
    int h = traj.head % MAX_HISTORY;
    for (int i = 0; i < sim.atom_count && i < 8; i++) {
        traj.pos_history[h][i*3+0] = atoms[i].x;
        traj.pos_history[h][i*3+1] = atoms[i].y;
        traj.pos_history[h][i*3+2] = atoms[i].z;
    }
    traj.head++; if (traj.count < MAX_HISTORY) traj.count++;
    return 0;
}

int md_set_ensemble(int ens) { sim.ensemble_type = ens; return 0; }
int md_set_constraints(int c) { sim.constraint_type = c; return 0; }
int md_set_temperature(double t) { sim.temperature = t; return 0; }
int md_set_pressure(double p) { sim.pressure = p; return 0; }
int md_set_lr_method(int m) { sim.lr_method = m; return 0; }
double md_get_temperature(void) {
    return sim.kinetic_energy / (1.5 * sim.atom_count * 0.0019872);
}

void md_print_state(void) {
    print_str("[MD] Step="); print_int(sim.step_count);
    print_str(" Atoms="); print_int(sim.atom_count);
    print_str(" T="); print_int((int)md_get_temperature());
    print_str(" K="); print_int((int)(sim.kinetic_energy * 1000));
    print_str(" U="); print_int((int)(sim.potential_energy * 1000));
    print_str(" E="); print_int((int)(sim.total_energy * 1000));
    print_str("\n");
}

void md_print_rdf(void) {
    print_str("[MD] RDF ("); print_int(rdf.samples); print_str(" samples):\n");
    for (int i = 0; i < 8 && i < rdf.bin_count; i++) {
        print_str("  r="); print_int((int)((i+0.5)*rdf.dr*100));
        print_str(" g(r)="); print_int((int)(rdf.bins[i] / (rdf.samples > 0 ? rdf.samples : 1)));
        print_str("\n");
    }
}

int main(void) {
    print_str("=== Molecular Dynamics Demo ===\n\n");
    md_init(FF_AMBER, INTEG_VVERLET, ENS_NVT, LR_Ewald);
    md_set_temperature(300.0); md_set_pressure(1.0); md_set_constraints(CONST_SHAKE);
    md_add_atom(0.0, 0.0, 0.0, 12.0, -0.4, 1);
    md_add_atom(1.0, 0.0, 0.0, 1.0, 0.2, 2);
    md_add_atom(0.0, 1.0, 0.0, 1.0, 0.2, 2);
    md_add_atom(0.0, 0.0, 1.0, 16.0, -0.5, 3);
    md_add_atom(1.0, 1.0, 0.0, 14.0, 0.1, 4);
    md_add_atom(0.0, 1.0, 1.0, 12.0, 0.0, 1);
    md_add_atom(1.0, 0.0, 1.0, 16.0, -0.3, 3);
    md_add_atom(0.5, 0.5, 0.5, 14.0, 0.15, 4);
    md_add_bond(0, 1, 500.0, 1.0); md_add_bond(0, 2, 500.0, 1.0);
    md_add_bond(3, 4, 400.0, 1.2); md_add_bond(4, 5, 300.0, 1.5);
    print_str("\nRunning 20 MD steps with VVerlet + NVT + SHAKE + Ewald...\n\n");
    for (int i = 0; i < 20; i++) { md_step(); if (i % 5 == 4) md_print_state(); }
    md_print_rdf();
    print_str("\nSwitching to Langevin dynamics...\n");
    md_set_ensemble(ENS_LANGEVIN);
    for (int i = 0; i < 10; i++) md_step();
    md_print_state();
    print_str("\nSwitching to NPT ensemble...\n");
    md_set_ensemble(ENS_NPT);
    for (int i = 0; i < 10; i++) md_step();
    md_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
