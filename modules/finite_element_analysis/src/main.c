/* finite_element_analysis: Structural FEA solver (v1.0)
 * Elements, materials, nonlinear/dynamic analysis, thermal, solvers
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

#define MAX_NODES     64
#define MAX_ELEMENTS  64
#define MAX_LOADS     32
#define MAX_MATS      8
#define MAX_MODES     16

#define ELEM_TRUSS   0x01
#define ELEM_BEAM    0x02
#define ELEM_SHELL   0x03
#define ELEM_SOLID   0x04
#define ELEM_CONTACT 0x05

#define MAT_ELASTIC    0x01
#define MAT_ELASTOPLASTIC 0x02
#define MAT_VISCOELASTIC  0x03
#define MAT_HYPERELASTIC  0x04
#define MAT_COMPOSITE     0x05

#define NL_NONE      0x00
#define NL_GEOMETRIC 0x01
#define NL_MATERIAL  0x02
#define NL_CONTACT   0x04

#define DYN_STATIC   0x00
#define DYN_MODAL    0x01
#define DYN_HARMONIC 0x02
#define DYN_TRANSIENT 0x03

#define THERM_NONE  0x00
#define THERM_STEADY 0x01
#define THERM_TRANSIENT 0x02

#define SOLV_DIRECT  0x01
#define SOLV_ITERATIVE 0x02
#define SOLV_PARALLEL  0x03

typedef struct {
    double x, y, z;
    double ux, uy, uz;
    double fx, fy, fz;
    int    constrained;
    int    active;
} node_t;

typedef struct {
    int    nodes[8];
    int    node_count;
    int    type;
    int    mat_id;
    double length;
    double area;
    double inertia;
    double stiffness;
    int    active;
} element_t;

typedef struct {
    double E;
    double nu;
    double rho;
    double sigma_y;
    double alpha;
    double conductivity;
    double specific_heat;
    int    type;
    char   name[32];
} material_t;

typedef struct {
    int    node;
    double fx, fy, fz;
    int    active;
} load_t;

typedef struct {
    double freq;
    double amplitude;
    int    node;
    int    active;
} harmonic_load_t;

typedef struct {
    double values[MAX_MODES];
    int    count;
} mode_shapes_t;

typedef struct {
    int    node_count;
    int    elem_count;
    int    load_count;
    int    mat_count;
    int    nl_flags;
    int    dyn_type;
    int    therm_type;
    int    solver_type;
    double max_displacement;
    double max_stress;
    double max_strain;
    double total_mass;
    double total_energy;
    double time;
    int    step_count;
    int    converged;
} fea_state_t;

static node_t nodes[MAX_NODES];
static element_t elements[MAX_ELEMENTS];
static material_t materials[MAX_MATS];
static load_t loads[MAX_LOADS];
static harmonic_load_t harmonic_loads[MAX_LOADS];
static mode_shapes_t modes;
static fea_state_t fea;

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

int fea_init(int solver, int dyn, int therm) {
    if (initialized) return -1;
    fea.node_count = 0; fea.elem_count = 0; fea.load_count = 0; fea.mat_count = 0;
    fea.nl_flags = NL_NONE; fea.dyn_type = dyn; fea.therm_type = therm;
    fea.solver_type = solver;
    fea.max_displacement = 0.0; fea.max_stress = 0.0; fea.max_strain = 0.0;
    fea.total_mass = 0.0; fea.total_energy = 0.0;
    fea.time = 0.0; fea.step_count = 0; fea.converged = 0;
    for (int i = 0; i < MAX_NODES; i++) { nodes[i].active = 0; nodes[i].constrained = 0; }
    for (int i = 0; i < MAX_ELEMENTS; i++) elements[i].active = 0;
    for (int i = 0; i < MAX_LOADS; i++) { loads[i].active = 0; harmonic_loads[i].active = 0; }
    for (int i = 0; i < MAX_MODES; i++) modes.values[i] = 0.0;
    modes.count = 0;
    initialized = 1;
    print_str("[FEA] Initialized: solver="); print_int(solver);
    print_str(" dyn="); print_int(dyn);
    print_str(" therm="); print_int(therm); print_str("\n");
    return 0;
}

int fea_add_material(double E, double nu, double rho, double sigma_y, int type) {
    if (fea.mat_count >= MAX_MATS) return -1;
    material_t* m = &materials[fea.mat_count];
    m->E = E; m->nu = nu; m->rho = rho; m->sigma_y = sigma_y;
    m->alpha = 1.2e-5; m->conductivity = 50.0; m->specific_heat = 500.0;
    m->type = type;
    return fea.mat_count++;
}

int fea_add_node(double x, double y, double z) {
    if (fea.node_count >= MAX_NODES) return -1;
    node_t* n = &nodes[fea.node_count];
    n->x = x; n->y = y; n->z = z;
    n->ux = 0.0; n->uy = 0.0; n->uz = 0.0;
    n->fx = 0.0; n->fy = 0.0; n->fz = 0.0;
    n->constrained = 0; n->active = 1;
    return fea.node_count++;
}

int fea_constrain_node(int node, int fix_x, int fix_y, int fix_z) {
    if (node < 0 || node >= fea.node_count) return -1;
    nodes[node].constrained = (fix_x ? 1 : 0) | (fix_y ? 2 : 0) | (fix_z ? 4 : 0);
    return 0;
}

int fea_add_element(int type, int n0, int n1, int mat_id, double area, double I) {
    if (fea.elem_count >= MAX_ELEMENTS) return -1;
    if (n0 >= fea.node_count || n1 >= fea.node_count) return -1;
    element_t* e = &elements[fea.elem_count];
    e->nodes[0] = n0; e->nodes[1] = n1; e->node_count = 2;
    e->type = type; e->mat_id = mat_id;
    e->area = area; e->inertia = I;
    double dx = nodes[n1].x - nodes[n0].x;
    double dy = nodes[n1].y - nodes[n0].y;
    double dz = nodes[n1].z - nodes[n0].z;
    e->length = dsqrt(dx*dx + dy*dy + dz*dz);
    double E = materials[mat_id].E;
    if (type == ELEM_TRUSS) e->stiffness = E * area / e->length;
    else if (type == ELEM_BEAM) e->stiffness = 12.0 * E * I / (e->length * e->length * e->length);
    else e->stiffness = E * area / e->length;
    e->active = 1;
    return fea.elem_count++;
}

int fea_add_load(int node, double fx, double fy, double fz) {
    if (fea.load_count >= MAX_LOADS) return -1;
    load_t* l = &loads[fea.load_count];
    l->node = node; l->fx = fx; l->fy = fy; l->fz = fz; l->active = 1;
    return fea.load_count++;
}

int fea_set_nonlinear(int flags) { fea.nl_flags = flags; return 0; }
int fea_set_dynamic(int type) { fea.dyn_type = type; return 0; }
int fea_set_thermal(int type) { fea.therm_type = type; return 0; }

static void compute_stiffness_matrix(void) {
    for (int i = 0; i < fea.node_count; i++) {
        nodes[i].fx = 0.0; nodes[i].fy = 0.0; nodes[i].fz = 0.0;
    }
    for (int e = 0; e < fea.elem_count; e++) {
        if (!elements[e].active) continue;
        int n0 = elements[e].nodes[0], n1 = elements[e].nodes[1];
        double k = elements[e].stiffness;
        double du = nodes[n1].ux - nodes[n0].ux;
        double dv = nodes[n1].uy - nodes[n0].uy;
        double dw = nodes[n1].uz - nodes[n0].uz;
        nodes[n0].fx += k * du; nodes[n0].fy += k * dv; nodes[n0].fz += k * dw;
        nodes[n1].fx -= k * du; nodes[n1].fy -= k * dv; nodes[n1].fz -= k * dw;
    }
    if (fea.nl_flags & NL_GEOMETRIC) {
        for (int i = 0; i < fea.node_count; i++) {
            double u2 = nodes[i].ux*nodes[i].ux + nodes[i].uy*nodes[i].uy + nodes[i].uz*nodes[i].uz;
            double geom_factor = 1.0 + 0.01 * u2;
            nodes[i].fx *= geom_factor;
            nodes[i].fy *= geom_factor;
        }
    }
    if (fea.nl_flags & NL_MATERIAL) {
        for (int e = 0; e < fea.elem_count; e++) {
            if (!elements[e].active) continue;
            int n0 = elements[e].nodes[0], n1 = elements[e].nodes[1];
            double du = nodes[n1].ux - nodes[n0].ux;
            double strain = du / elements[e].length;
            material_t* m = &materials[elements[e].mat_id];
            if (m->type == MAT_ELASTOPLASTIC && dabs(strain * m->E) > m->sigma_y) {
                double kp = 0.01;
                double plastic_strain = dabs(strain) - m->sigma_y / m->E;
                double stress = m->sigma_y + kp * plastic_strain;
                double factor = stress / (dabs(strain) * m->E + 1e-10);
                double k_new = elements[e].stiffness * factor;
                nodes[n0].fx = k_new * du;
                nodes[n1].fx = -k_new * du;
            }
        }
    }
}

static void solve_static_direct(void) {
    compute_stiffness_matrix();
    for (int l = 0; l < fea.load_count; l++) {
        if (!loads[l].active) continue;
        int n = loads[l].node;
        nodes[n].fx += loads[l].fx;
        nodes[n].fy += loads[l].fy;
        nodes[n].fz += loads[l].fz;
    }
    for (int iter = 0; iter < 50; iter++) {
        double max_change = 0.0;
        for (int e = 0; e < fea.elem_count; e++) {
            if (!elements[e].active) continue;
            int n0 = elements[e].nodes[0], n1 = elements[e].nodes[1];
            double k = elements[e].stiffness;
            double du = (nodes[n1].fx - nodes[n0].fx) / (2.0 * k + 1e-10);
            if (!(nodes[n0].constrained & 1)) {
                nodes[n0].ux += du * 0.1;
                if (dabs(du) > max_change) max_change = dabs(du);
            }
            if (!(nodes[n1].constrained & 1)) {
                nodes[n1].ux -= du * 0.1;
                if (dabs(du) > max_change) max_change = dabs(du);
            }
        }
        if (max_change < 1e-8) { fea.converged = 1; break; }
        compute_stiffness_matrix();
    }
}

static void solve_iterative_cg(void) {
    compute_stiffness_matrix();
    for (int l = 0; l < fea.load_count; l++) {
        if (!loads[l].active) continue;
        int n = loads[l].node;
        nodes[n].fx += loads[l].fx;
        nodes[n].fy += loads[l].fy;
    }
    for (int iter = 0; iter < 100; iter++) {
        double alpha = 0.01 / (1.0 + 0.001 * iter);
        for (int e = 0; e < fea.elem_count; e++) {
            if (!elements[e].active) continue;
            int n0 = elements[e].nodes[0], n1 = elements[e].nodes[1];
            double k = elements[e].stiffness;
            double r = nodes[n1].fx - nodes[n0].fx - k * (nodes[n1].ux - nodes[n0].ux);
            if (!(nodes[n0].constrained & 1)) nodes[n0].ux += alpha * r;
            if (!(nodes[n1].constrained & 1)) nodes[n1].ux -= alpha * r;
        }
        double res = 0.0;
        for (int i = 0; i < fea.node_count; i++)
            if (!nodes[i].constrained) res += nodes[i].fx * nodes[i].fx;
        if (res < 1e-10) { fea.converged = 1; break; }
    }
}

static void compute_modal(void) {
    modes.count = 0;
    for (int i = 0; i < MAX_MODES && i < fea.elem_count; i++) {
        double k = elements[i].stiffness;
        double m = materials[elements[i].mat_id].rho * elements[i].area * elements[i].length;
        if (m < 1e-10) m = 1e-10;
        modes.values[i] = dsqrt(k / m) / (2.0 * 3.14159);
        modes.count++;
    }
}

static void compute_harmonic(double freq) {
    double omega = 2.0 * 3.14159 * freq;
    for (int i = 0; i < fea.node_count; i++) {
        double m_eff = 1.0;
        for (int e = 0; e < fea.elem_count; e++) {
            if (!elements[e].active) continue;
            if (elements[e].nodes[0] == i || elements[e].nodes[1] == i) {
                m_eff += materials[elements[e].mat_id].rho * elements[e].area * elements[e].length * 0.5;
            }
        }
        double k_eff = 0.0;
        for (int e = 0; e < fea.elem_count; e++) {
            if (!elements[e].active) continue;
            if (elements[e].nodes[0] == i || elements[e].nodes[1] == i)
                k_eff += elements[e].stiffness * 0.5;
        }
        double denom = k_eff - m_eff * omega * omega;
        if (dabs(denom) > 1e-10) nodes[i].ux += 1.0 / denom;
    }
}

static void compute_transient(double dt) {
    double beta = 0.25, gamma = 0.5;
    compute_stiffness_matrix();
    for (int l = 0; l < fea.load_count; l++) {
        if (!loads[l].active) continue;
        int n = loads[l].node;
        nodes[n].fx += loads[l].fx * (1.0 + 0.1 * fea.time);
    }
    for (int i = 0; i < fea.node_count; i++) {
        if (nodes[i].constrained) continue;
        double m_eff = 1.0;
        double k_eff = 0.0;
        for (int e = 0; e < fea.elem_count; e++) {
            if (!elements[e].active) continue;
            if (elements[e].nodes[0] == i || elements[e].nodes[1] == i) {
                m_eff += materials[elements[e].mat_id].rho * elements[e].area * elements[e].length * 0.5;
                k_eff += elements[e].stiffness * 0.5;
            }
        }
        double a = m_eff / (beta * dt * dt) + k_eff;
        double b = nodes[i].fx + m_eff * (nodes[i].ux / (beta * dt * dt));
        if (dabs(a) > 1e-10) nodes[i].ux = b / a;
    }
    fea.time += dt;
}

static void compute_thermal(void) {
    if (fea.therm_type == THERM_STEADY) {
        for (int e = 0; e < fea.elem_count; e++) {
            if (!elements[e].active) continue;
            int n0 = elements[e].nodes[0], n1 = elements[e].nodes[1];
            double k = materials[elements[e].mat_id].conductivity * elements[e].area / elements[e].length;
            double t_diff = nodes[n1].uz - nodes[n0].uz;
            nodes[n0].uz += k * t_diff * 0.01;
            nodes[n1].uz -= k * t_diff * 0.01;
        }
    } else if (fea.therm_type == THERM_TRANSIENT) {
        double dt = 0.01;
        for (int e = 0; e < fea.elem_count; e++) {
            if (!elements[e].active) continue;
            int n0 = elements[e].nodes[0], n1 = elements[e].nodes[1];
            double k = materials[elements[e].mat_id].conductivity;
            double c = materials[elements[e].mat_id].specific_heat;
            double m = materials[elements[e].mat_id].rho * elements[e].area * elements[e].length;
            double alpha = k * dt / (m * c * elements[e].length * elements[e].length + 1e-10);
            double t_diff = nodes[n1].uz - nodes[n0].uz;
            nodes[n0].uz += alpha * t_diff;
            nodes[n1].uz -= alpha * t_diff;
        }
    }
}

static void update_results(void) {
    fea.max_displacement = 0.0; fea.max_stress = 0.0; fea.max_strain = 0.0;
    fea.total_mass = 0.0; fea.total_energy = 0.0;
    for (int i = 0; i < fea.node_count; i++) {
        double d = dsqrt(nodes[i].ux*nodes[i].ux + nodes[i].uy*nodes[i].uy + nodes[i].uz*nodes[i].uz);
        if (d > fea.max_displacement) fea.max_displacement = d;
    }
    for (int e = 0; e < fea.elem_count; e++) {
        if (!elements[e].active) continue;
        int n0 = elements[e].nodes[0], n1 = elements[e].nodes[1];
        double strain = dabs(nodes[n1].ux - nodes[n0].ux) / elements[e].length;
        double stress = strain * materials[elements[e].mat_id].E;
        if (stress > fea.max_stress) fea.max_stress = stress;
        if (strain > fea.max_strain) fea.max_strain = strain;
        fea.total_mass += materials[elements[e].mat_id].rho * elements[e].area * elements[e].length;
        fea.total_energy += 0.5 * elements[e].stiffness * strain * strain * elements[e].length;
    }
}

int fea_step(void) {
    if (!initialized) return -1;
    if (fea.dyn_type == DYN_STATIC) {
        if (fea.solver_type == SOLV_DIRECT) solve_static_direct();
        else solve_iterative_cg();
    } else if (fea.dyn_type == DYN_MODAL) {
        solve_static_direct(); compute_modal();
    } else if (fea.dyn_type == DYN_HARMONIC) {
        compute_harmonic(10.0);
    } else if (fea.dyn_type == DYN_TRANSIENT) {
        compute_transient(0.01);
    }
    if (fea.therm_type != THERM_NONE) compute_thermal();
    update_results();
    fea.step_count++;
    return 0;
}

void fea_print_state(void) {
    print_str("[FEA] Step="); print_int(fea.step_count);
    print_str(" Umax="); print_int((int)(fea.max_displacement * 10000));
    print_str(" Smax="); print_int((int)(fea.max_stress));
    print_str(" Mass="); print_int((int)(fea.total_mass * 100));
    print_str(" E="); print_int((int)(fea.total_energy * 1000));
    print_str(" conv="); print_int(fea.converged);
    print_str("\n");
}

void fea_print_modes(void) {
    print_str("[FEA] Natural frequencies (Hz):\n");
    for (int i = 0; i < 8 && i < modes.count; i++) {
        print_str("  Mode "); print_int(i+1); print_str(": ");
        print_int((int)(modes.values[i] * 100));
        print_str(" Hz\n");
    }
}

int main(void) {
    print_str("=== FEA Solver Demo ===\n\n");
    fea_init(SOLV_DIRECT, DYN_STATIC, THERM_NONE);
    int steel = fea_add_material(210e9, 0.3, 7800.0, 250e6, MAT_ELASTIC);
    int aluminum = fea_add_material(70e9, 0.33, 2700.0, 280e6, MAT_ELASTOPLASTIC);
    int n0 = fea_add_node(0.0, 0.0, 0.0);
    int n1 = fea_add_node(1.0, 0.0, 0.0);
    int n2 = fea_add_node(2.0, 0.0, 0.0);
    int n3 = fea_add_node(3.0, 0.0, 0.0);
    int n4 = fea_add_node(4.0, 0.0, 0.0);
    fea_constrain_node(n0, 1, 1, 1);
    fea_constrain_node(n4, 1, 1, 1);
    fea_add_element(ELEM_BEAM, n0, n1, steel, 0.01, 8.3e-6);
    fea_add_element(ELEM_BEAM, n1, n2, steel, 0.01, 8.3e-6);
    fea_add_element(ELEM_BEAM, n2, n3, aluminum, 0.01, 8.3e-6);
    fea_add_element(ELEM_BEAM, n3, n4, aluminum, 0.01, 8.3e-6);
    fea_add_load(n2, 0.0, -1000.0, 0.0);
    print_str("Static analysis with direct solver:\n");
    for (int i = 0; i < 10; i++) { fea_step(); fea_print_state(); }
    print_str("\nModal analysis:\n");
    fea_set_dynamic(DYN_MODAL);
    for (int i = 0; i < 5; i++) fea_step();
    fea_print_modes();
    print_str("\nTransient dynamic:\n");
    fea_set_dynamic(DYN_TRANSIENT);
    for (int i = 0; i < 10; i++) { fea_step(); fea_print_state(); }
    print_str("\nWith thermal coupling:\n");
    fea_set_thermal(THERM_TRANSIENT);
    nodes[n2].uz = 500.0;
    for (int i = 0; i < 10; i++) fea_step();
    fea_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
