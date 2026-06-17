/* computational_fluid_dynamics: CFD solver (v1.0)
 * Navier-Stokes, turbulence, multiphase, mesh, boundary conditions, visualization
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

#define MAX_CELLS_X   32
#define MAX_CELLS_Y   32
#define MAX_CELLS_Z   8
#define MAX_PARTICLES 128
#define MAX_STREAMLINES 16

#define SOLVER_FVM 0x01
#define SOLVER_FEM 0x02
#define SOLVER_FDM 0x03
#define SOLVER_SPECTRAL 0x04

#define TURB_NONE    0x00
#define TURB_RANS    0x01
#define TURB_LES     0x02
#define TURB_DNS     0x03
#define TURB_KEPS    0x04

#define MULT_NONE    0x00
#define MULT_VOF     0x01
#define MULT_LEVELSET 0x02
#define MULT_PHASEFIELD 0x03

#define MESH_STRUCT 0x01
#define MESH_UNSTRUCT 0x02
#define MESH_ADAPTIVE 0x03

#define BC_NOSLIP    0x01
#define BC_PERIODIC  0x02
#define BC_SYMMETRY  0x03
#define BC_INLET     0x04
#define BC_OUTLET    0x05

typedef struct {
    double u, v, w;
    double p;
    double rho;
    double mu;
    double t;
    int    type;
} cell_t;

typedef struct {
    double x, y, z;
    double u, v, w;
    double life;
    int    phase;
    int    active;
} particle_t;

typedef struct {
    double points[MAX_STREAMLINES][64][3];
    int    length[64];
    int    count;
} streamline_t;

typedef struct {
    int    nx, ny, nz;
    double dx, dy, dz;
    double dt;
    double viscosity;
    double density;
    double reynolds;
    double courant;
    int    solver;
    int    turb_model;
    int    multiphase;
    int    mesh_type;
    int    step_count;
    double max_velocity;
    double avg_pressure;
    double divergence;
} cfd_state_t;

static cell_t cells[MAX_CELLS_X][MAX_CELLS_Y][MAX_CELLS_Z];
static particle_t particles[MAX_PARTICLES];
static streamline_t streamlines;
static cfd_state_t cfd;

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
static double dabs(double x) { return x < 0 ? -x : x; }
static double dsqrt(double x) {
    if (x <= 0.0) return 0.0;
    double g = x, p = x / g;
    for (int i = 0; i < 30; i++) { g = (g + p) * 0.5; p = x / g; }
    return g;
}

int cfd_init(int solver, int turb, int mesh) {
    if (initialized) return -1;
    cfd.nx = 8; cfd.ny = 8; cfd.nz = 2;
    cfd.dx = 0.1; cfd.dy = 0.1; cfd.dz = 0.1;
    cfd.dt = 0.001; cfd.viscosity = 0.001; cfd.density = 1.0;
    cfd.reynolds = 1000.0; cfd.courant = 0.0;
    cfd.solver = solver; cfd.turb_model = turb; cfd.mesh_type = mesh;
    cfd.multiphase = MULT_NONE; cfd.step_count = 0;
    cfd.max_velocity = 0.0; cfd.avg_pressure = 0.0; cfd.divergence = 0.0;
    for (int x = 0; x < MAX_CELLS_X; x++)
        for (int y = 0; y < MAX_CELLS_Y; y++)
            for (int z = 0; z < MAX_CELLS_Z; z++) {
                cells[x][y][z].u = 0.0; cells[x][y][z].v = 0.0; cells[x][y][z].w = 0.0;
                cells[x][y][z].p = 0.0; cells[x][y][z].rho = 1.0;
                cells[x][y][z].mu = 0.001; cells[x][y][z].t = 300.0;
                cells[x][y][z].type = 0;
            }
    for (int i = 0; i < MAX_PARTICLES; i++) {
        particles[i].active = 0; particles[i].phase = 0;
    }
    streamlines.count = 0;
    initialized = 1;
    print_str("[CFD] Initialized: solver="); print_int(solver);
    print_str(" turb="); print_int(turb);
    print_str(" mesh="); print_int(mesh); print_str("\n");
    return 0;
}

int cfd_set_inlet_velocity(double u, double v, double w) {
    for (int y = 0; y < cfd.ny; y++)
        for (int z = 0; z < cfd.nz; z++) {
            cells[0][y][z].u = u; cells[0][y][z].v = v; cells[0][y][z].w = w;
            cells[0][y][z].type = BC_INLET;
        }
    return 0;
}

int cfd_set_outlet_pressure(double p) {
    for (int y = 0; y < cfd.ny; y++)
        for (int z = 0; z < cfd.nz; z++) {
            cells[cfd.nx-1][y][z].p = p;
            cells[cfd.nx-1][y][z].type = BC_OUTLET;
        }
    return 0;
}

int cfd_set_wall(int x, int y, int z) {
    if (x >= cfd.nx || y >= cfd.ny || z >= cfd.nz) return -1;
    cells[x][y][z].type = BC_NOSLIP;
    cells[x][y][z].u = 0.0; cells[x][y][z].v = 0.0; cells[x][y][z].w = 0.0;
    return 0;
}

int cfd_set_periodic(int axis) {
    (void)axis;
    for (int x = 0; x < cfd.nx; x++)
        for (int y = 0; y < cfd.ny; y++)
            for (int z = 0; z < cfd.nz; z++) {
                if (cells[x][y][z].type == 0) cells[x][y][z].type = BC_PERIODIC;
            }
    return 0;
}

static void apply_boundary(void) {
    for (int y = 0; y < cfd.ny; y++)
        for (int z = 0; z < cfd.nz; z++) {
            cells[0][y][z].u = cells[1][y][z].u;
            cells[cfd.nx-1][y][z].p = cells[cfd.nx-2][y][z].p;
        }
    for (int x = 0; x < cfd.nx; x++) {
        for (int z = 0; z < cfd.nz; z++) {
            cells[x][0][z].v = -cells[x][1][z].v;
            cells[x][cfd.ny-1][z].v = -cells[x][cfd.ny-2][z].v;
        }
    }
}

static double calc_courant(void) {
    double max_c = 0.0;
    for (int x = 0; x < cfd.nx; x++)
        for (int y = 0; y < cfd.ny; y++)
            for (int z = 0; z < cfd.nz; z++) {
                double v = dsqrt(cells[x][y][z].u*cells[x][y][z].u +
                               cells[x][y][z].v*cells[x][y][z].v +
                               cells[x][y][z].w*cells[x][y][z].w);
                double c = v * cfd.dt / cfd.dx;
                if (c > max_c) max_c = c;
            }
    return max_c;
}

static void diffuse_fvm(void) {
    for (int x = 1; x < cfd.nx-1; x++)
        for (int y = 1; y < cfd.ny-1; y++)
            for (int z = 0; z < cfd.nz; z++) {
                double lap_u = (cells[x+1][y][z].u - 2.0*cells[x][y][z].u + cells[x-1][y][z].u) / (cfd.dx*cfd.dx)
                             + (cells[x][y+1][z].u - 2.0*cells[x][y][z].u + cells[x][y-1][z].u) / (cfd.dy*cfd.dy);
                double lap_v = (cells[x+1][y][z].v - 2.0*cells[x][y][z].v + cells[x-1][y][z].v) / (cfd.dx*cfd.dx)
                             + (cells[x][y+1][z].v - 2.0*cells[x][y][z].v + cells[x][y-1][z].v) / (cfd.dy*cfd.dy);
                cells[x][y][z].u += cfd.viscosity * lap_u * cfd.dt;
                cells[x][y][z].v += cfd.viscosity * lap_v * cfd.dt;
            }
}

static void advect_fvm(void) {
    for (int x = 1; x < cfd.nx-1; x++)
        for (int y = 1; y < cfd.ny-1; y++)
            for (int z = 0; z < cfd.nz; z++) {
                double du_dx = (cells[x+1][y][z].u - cells[x-1][y][z].u) / (2.0*cfd.dx);
                double du_dy = (cells[x][y+1][z].u - cells[x][y-1][z].u) / (2.0*cfd.dy);
                double dv_dx = (cells[x+1][y][z].v - cells[x-1][y][z].v) / (2.0*cfd.dx);
                double dv_dy = (cells[x][y+1][z].v - cells[x][y-1][z].v) / (2.0*cfd.dy);
                cells[x][y][z].u -= (cells[x][y][z].u * du_dx + cells[x][y][z].v * du_dy) * cfd.dt;
                cells[x][y][z].v -= (cells[x][y][z].u * dv_dx + cells[x][y][z].v * dv_dy) * cfd.dt;
            }
}

static void project_pressure(void) {
    for (int x = 1; x < cfd.nx-1; x++)
        for (int y = 1; y < cfd.ny-1; y++)
            for (int z = 0; z < cfd.nz; z++) {
                double div = (cells[x+1][y][z].u - cells[x-1][y][z].u) / (2.0*cfd.dx)
                           + (cells[x][y+1][z].v - cells[x][y-1][z].v) / (2.0*cfd.dy);
                cells[x][y][z].p += -cfd.density * div * cfd.dx * cfd.dx / cfd.dt;
                cells[x][y][z].u -= cfd.dt / cfd.density * (cells[x+1][y][z].p - cells[x-1][y][z].p) / (2.0*cfd.dx);
                cells[x][y][z].v -= cfd.dt / cfd.density * (cells[x][y+1][z].p - cells[x][y-1][z].p) / (2.0*cfd.dy);
            }
}

static void apply_rans_turbulence(void) {
    double c_mu = 0.09;
    for (int x = 1; x < cfd.nx-1; x++)
        for (int y = 1; y < cfd.ny-1; y++)
            for (int z = 0; z < cfd.nz; z++) {
                double s2 = 2.0 * ((cells[x+1][y][z].u - cells[x-1][y][z].u) / (2.0*cfd.dx))
                          * ((cells[x+1][y][z].u - cells[x-1][y][z].u) / (2.0*cfd.dx));
                double mu_t = cfd.density * c_mu * c_mu * 100.0 * 100.0 / (cfd.viscosity + 1e-10);
                double mu_eff = cfd.viscosity + mu_t;
                cells[x][y][z].mu = mu_eff;
                double damp = 1.0 / (1.0 + 0.01 * s2);
                cells[x][y][z].u *= (1.0 - 0.001 * damp);
                cells[x][y][z].v *= (1.0 - 0.001 * damp);
            }
}

static void apply_les_turbulence(void) {
    double cs = 0.1;
    double delta = cfd.dx;
    for (int x = 1; x < cfd.nx-1; x++)
        for (int y = 1; y < cfd.ny-1; y++)
            for (int z = 0; z < cfd.nz; z++) {
                double s11 = (cells[x+1][y][z].u - cells[x-1][y][z].u) / (2.0*cfd.dx);
                double s22 = (cells[x][y+1][z].v - cells[x][y-1][z].v) / (2.0*cfd.dy);
                double s12 = 0.5 * ((cells[x+1][y][z].v - cells[x-1][y][z].v) / (2.0*cfd.dx)
                                  + (cells[x][y+1][z].u - cells[x][y-1][z].u) / (2.0*cfd.dy));
                double s_mag = dsqrt(2.0*(s11*s11 + s22*s22 + 2.0*s12*s12));
                double nu_t = cs*cs * delta*delta * s_mag;
                cells[x][y][z].mu = cfd.viscosity + cfd.density * nu_t;
            }
}

static void update_vof(void) {
    for (int x = 1; x < cfd.nx-1; x++)
        for (int y = 1; y < cfd.ny-1; y++)
            for (int z = 0; z < cfd.nz; z++) {
                double df_dx = (cells[x+1][y][z].rho - cells[x-1][y][z].rho) / (2.0*cfd.dx);
                double df_dy = (cells[x][y+1][z].rho - cells[x][y-1][z].rho) / (2.0*cfd.dy);
                cells[x][y][z].rho -= (cells[x][y][z].u * df_dx + cells[x][y][z].v * df_dy) * cfd.dt;
                if (cells[x][y][z].rho < 0.1) cells[x][y][z].rho = 0.1;
                if (cells[x][y][z].rho > 2.0) cells[x][y][z].rho = 2.0;
            }
}

static void update_particles(void) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (!particles[i].active) continue;
        int px = (int)(particles[i].x / cfd.dx);
        int py = (int)(particles[i].y / cfd.dy);
        if (px < 0 || px >= cfd.nx || py < 0 || py >= cfd.ny) { particles[i].active = 0; continue; }
        double fu = cells[px][py][0].u;
        double fv = cells[px][py][0].v;
        particles[i].u += (fu - particles[i].u) * 0.1;
        particles[i].v += (fv - particles[i].v) * 0.1;
        particles[i].x += particles[i].u * cfd.dt;
        particles[i].y += particles[i].v * cfd.dt;
        particles[i].life -= cfd.dt;
        if (particles[i].life < 0.0) particles[i].active = 0;
    }
}

static double calc_divergence(void) {
    double total = 0.0;
    for (int x = 1; x < cfd.nx-1; x++)
        for (int y = 1; y < cfd.ny-1; y++)
            for (int z = 0; z < cfd.nz; z++) {
                double div = (cells[x+1][y][z].u - cells[x-1][y][z].u) / (2.0*cfd.dx)
                           + (cells[x][y+1][z].v - cells[x][y-1][z].v) / (2.0*cfd.dy);
                total += dabs(div);
            }
    return total;
}

int cfd_step(void) {
    if (!initialized) return -1;
    if (cfd.solver == SOLVER_FVM || cfd.solver == SOLVER_FDM) {
        diffuse_fvm(); advect_fvm(); project_pressure();
    }
    apply_boundary();
    if (cfd.turb_model == TURB_RANS || cfd.turb_model == TURB_KEPS) apply_rans_turbulence();
    else if (cfd.turb_model == TURB_LES) apply_les_turbulence();
    if (cfd.multiphase == MULT_VOF) update_vof();
    update_particles();
    cfd.courant = calc_courant();
    cfd.divergence = calc_divergence();
    cfd.max_velocity = 0.0; cfd.avg_pressure = 0.0;
    int cnt = 0;
    for (int x = 0; x < cfd.nx; x++)
        for (int y = 0; y < cfd.ny; y++)
            for (int z = 0; z < cfd.nz; z++) {
                double v = dsqrt(cells[x][y][z].u*cells[x][y][z].u +
                               cells[x][y][z].v*cells[x][y][z].v);
                if (v > cfd.max_velocity) cfd.max_velocity = v;
                cfd.avg_pressure += cells[x][y][z].p;
                cnt++;
            }
    if (cnt > 0) cfd.avg_pressure /= cnt;
    cfd.step_count++;
    return 0;
}

int cfd_set_turbulence(int m) { cfd.turb_model = m; return 0; }
int cfd_set_multiphase(int m) { cfd.multiphase = m; return 0; }
int cfd_set_viscosity(double v) { cfd.viscosity = v; return 0; }
int cfd_set_dt(double dt) { cfd.dt = dt; return 0; }

int cfd_add_particle(double x, double y, double z, int phase) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (!particles[i].active) {
            particles[i].x = x; particles[i].y = y; particles[i].z = z;
            particles[i].u = 0.0; particles[i].v = 0.0; particles[i].w = 0.0;
            particles[i].life = 10.0; particles[i].phase = phase;
            particles[i].active = 1;
            return i;
        }
    }
    return -1;
}

void cfd_print_state(void) {
    print_str("[CFD] Step="); print_int(cfd.step_count);
    print_str(" Courant="); print_int((int)(cfd.courant * 1000));
    print_str(" Vmax="); print_int((int)(cfd.max_velocity * 1000));
    print_str(" Pavg="); print_int((int)(cfd.avg_pressure * 1000));
    print_str(" Div="); print_int((int)(cfd.divergence * 10000));
    print_str("\n");
}

void cfd_print_field(void) {
    print_str("[CFD] Velocity field (y=4):\n");
    for (int y = 3; y < 5 && y < cfd.ny; y++) {
        for (int x = 0; x < cfd.nx; x++) {
            int v = (int)(dsqrt(cells[x][y][0].u*cells[x][y][0].u +
                               cells[x][y][0].v*cells[x][y][0].v) * 100);
            print_str(" "); print_int(v);
        }
        print_str("\n");
    }
}

int main(void) {
    print_str("=== CFD Solver Demo ===\n\n");
    cfd_init(SOLVER_FVM, TURB_RANS, MESH_STRUCT);
    cfd_set_viscosity(0.01); cfd_set_dt(0.005);
    cfd_set_inlet_velocity(1.0, 0.0, 0.0);
    cfd_set_outlet_pressure(0.0);
    for (int y = 0; y < cfd.ny; y++) cfd_set_wall(4, y, 0);
    for (int i = 0; i < 8; i++) cfd_add_particle(0.5, 1.0 + i * 0.8, 0.0, 0);
    print_str("\nRunning 20 FVM steps with RANS turbulence...\n");
    for (int i = 0; i < 20; i++) { cfd_step(); if (i % 5 == 4) cfd_print_state(); }
    cfd_print_field();
    print_str("\nSwitching to LES turbulence...\n");
    cfd_set_turbulence(TURB_LES);
    for (int i = 0; i < 10; i++) cfd_step();
    cfd_print_state();
    print_str("\nEnabling VOF multiphase...\n");
    cfd_set_multiphase(MULT_VOF);
    for (int i = 0; i < 10; i++) cfd_step();
    cfd_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
