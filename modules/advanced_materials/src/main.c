/* advanced_materials: Materials science computation (v1.0)
 * Crystal structures, phase diagrams, composite mechanics, nanomaterials
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

#define MAX_MATERIALS  32
#define MAX_PHASES     8
#define MAX_COMPS      4
#define MAX_LAYERS     16
#define MAX_NANO       16

#define CRYSTAL_CUBIC    0x01
#define CRYSTAL_HEX      0x02
#define CRYSTAL_TETRAG   0x03
#define CRYSTAL_ORTHORH  0x04
#define CRYSTAL_MONOCL   0x05
#define CRYSTAL_TRICLIN  0x06
#define CRYSTAL_TRIGONAL 0x07

#define MAT_METAL    0x01
#define MAT_CERAMIC  0x02
#define MAT_POLYMER  0x03
#define MAT_COMPOSITE 0x04
#define MAT_NANO     0x05
#define MAT_SEMI     0x06

#define NANO_DOT     0x01
#define NANO_WIRE    0x02
#define NANO_TUBE    0x03
#define NANO_SHEET   0x04

typedef struct {
    char   name[32];
    int    type;
    int    crystal;
    double E;
    double nu;
    double rho;
    double sigma_y;
    double k_thermal;
    double sigma_elec;
    double chi_mag;
    double a, b, c;
    double alpha_ang, beta_ang, gamma_ang;
    double T_melt;
    int    active;
} material_t;

typedef struct {
    double composition[MAX_PHASES];
    double fraction[MAX_PHASES];
    int    phase_count;
    double temperature;
    double pressure;
    int    phase_ids[MAX_PHASES];
} phase_state_t;

typedef struct {
    int    layer_count;
    double thickness[MAX_LAYERS];
    double E1[MAX_LAYERS], E2[MAX_LAYERS];
    double nu12[MAX_LAYERS];
    double G12[MAX_LAYERS];
    double angle[MAX_LAYERS];
    double A[3][3], B[3][3], D[3][3];
    double sigma_fail;
} laminate_t;

typedef struct {
    int    type;
    double size;
    double bandgap;
    double exciton_energy;
    double surface_area;
    double quantum_yield;
    int    active;
} nanomaterial_t;

typedef struct {
    int    mat_count;
    int    nano_count;
    double total_volume;
    double avg_density;
    double avg_E;
    double avg_k;
    int    phase_calc_done;
    int    laminate_calc_done;
} mat_state_t;

static material_t materials[MAX_MATERIALS];
static phase_state_t phases;
static laminate_t laminate;
static nanomaterial_t nanos[MAX_NANO];
static mat_state_t mat;

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
static double dcos(double x) {
    double sum = 1.0, term = 1.0, x2 = x * x;
    for (int i = 1; i < 15; i++) { term *= -x2 / ((2*i-1)*(2*i)); sum += term; }
    return sum;
}

int mat_init(void) {
    if (initialized) return -1;
    mat.mat_count = 0; mat.nano_count = 0;
    mat.total_volume = 0.0; mat.avg_density = 0.0;
    mat.avg_E = 0.0; mat.avg_k = 0.0;
    mat.phase_calc_done = 0; mat.laminate_calc_done = 0;
    for (int i = 0; i < MAX_MATERIALS; i++) materials[i].active = 0;
    for (int i = 0; i < MAX_NANO; i++) nanos[i].active = 0;
    laminate.layer_count = 0;
    phases.phase_count = 0;
    initialized = 1;
    print_str("[MAT] Materials module initialized\n");
    return 0;
}

int mat_add_material(const char* name, int type, int crystal,
                     double E, double nu, double rho, double sigma_y,
                     double k_th, double sig_e, double chi,
                     double a, double b, double c, double Tm) {
    if (mat.mat_count >= MAX_MATERIALS) return -1;
    material_t* m = &materials[mat.mat_count];
    int len = 0;
    while (len < 31 && name[len]) { m->name[len] = name[len]; len++; }
    m->name[len] = '\0';
    m->type = type; m->crystal = crystal;
    m->E = E; m->nu = nu; m->rho = rho; m->sigma_y = sigma_y;
    m->k_thermal = k_th; m->sigma_elec = sig_e; m->chi_mag = chi;
    m->a = a; m->b = b; m->c = c;
    m->alpha_ang = 90.0; m->beta_ang = 90.0; m->gamma_ang = 90.0;
    if (crystal == CRYSTAL_HEX) { m->gamma_ang = 120.0; }
    if (crystal == CRYSTAL_MONOCL) { m->beta_ang = 97.0; }
    m->T_melt = Tm;
    m->active = 1;
    print_str("[MAT] Added: "); print_str(name);
    print_str(" E="); print_int((int)(E/1e9)); print_str("GPa");
    print_str(" rho="); print_int((int)rho); print_str("kg/m3\n");
    return mat.mat_count++;
}

void mat_compute_properties(void) {
    double sum_rho = 0.0, sum_E = 0.0, sum_k = 0.0;
    int cnt = 0;
    for (int i = 0; i < mat.mat_count; i++) {
        if (!materials[i].active) continue;
        material_t* m = &materials[i];
        double V = m->a * m->b * m->c * 1e-30;
        mat.total_volume += V;
        sum_rho += m->rho;
        sum_E += m->E;
        sum_k += m->k_thermal;
        cnt++;
    }
    if (cnt > 0) {
        mat.avg_density = sum_rho / cnt;
        mat.avg_E = sum_E / cnt;
        mat.avg_k = sum_k / cnt;
    }
    print_str("[MAT] Avg properties: rho="); print_int((int)mat.avg_density);
    print_str(" E="); print_int((int)(mat.avg_E/1e9)); print_str("GPa");
    print_str(" k="); print_int((int)mat.avg_k); print_str("W/mK\n");
}

void mat_phase_diagram(double T_start, double T_end, double comp_A) {
    phases.temperature = (T_start + T_end) * 0.5;
    phases.pressure = 1.0;
    phases.phase_count = 0;
    if (comp_A < 0.3) {
        phases.phase_ids[0] = 0; phases.fraction[0] = 1.0 - comp_A / 0.3;
        phases.phase_ids[1] = 1; phases.fraction[1] = comp_A / 0.3;
        phases.phase_count = 2;
        if (phases.temperature > 1500.0) {
            phases.phase_count = 1; phases.fraction[0] = 1.0;
            phases.phase_ids[0] = 2;
        }
    } else if (comp_A < 0.7) {
        phases.phase_ids[0] = 1; phases.fraction[0] = 0.6;
        phases.phase_ids[1] = 2; phases.fraction[1] = 0.4;
        phases.phase_count = 2;
        if (phases.temperature > 1800.0) {
            phases.phase_count = 1; phases.fraction[0] = 1.0;
            phases.phase_ids[0] = 2;
        }
    } else {
        phases.phase_ids[0] = 2; phases.fraction[0] = (comp_A - 0.7) / 0.3;
        phases.phase_ids[1] = 3; phases.fraction[1] = 1.0 - (comp_A - 0.7) / 0.3;
        phases.phase_count = 2;
        if (phases.temperature > 1200.0) {
            phases.phase_count = 1; phases.fraction[0] = 1.0;
            phases.phase_ids[0] = 2;
        }
    }
    phases.composition[0] = comp_A;
    mat.phase_calc_done = 1;
    print_str("[MAT] Phase diagram at T="); print_int((int)phases.temperature);
    print_str("K comp="); print_int((int)(comp_A * 100)); print_str("%\n");
    for (int i = 0; i < phases.phase_count; i++) {
        print_str("  Phase "); print_int(phases.phase_ids[i]);
        print_str(": fraction="); print_int((int)(phases.fraction[i] * 100));
        print_str("%\n");
    }
}

double mat_rule_of_mixtures(double V_f, double E_f, double E_m) {
    return V_f * E_f + (1.0 - V_f) * E_m;
}

double mat_inverse_rom(double V_f, double E_f, double E_m) {
    double vf = V_f > 0.001 ? V_f : 0.001;
    double vm = 1.0 - V_f;
    return 1.0 / (vf / E_f + vm / E_m);
}

void mat_laminate_analysis(void) {
    if (laminate.layer_count == 0) return;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) {
            laminate.A[i][j] = 0.0;
            laminate.B[i][j] = 0.0;
            laminate.D[i][j] = 0.0;
        }
    double z_total = 0.0;
    for (int l = 0; l < laminate.layer_count; l++) z_total += laminate.thickness[l];
    double z = 0.0;
    for (int l = 0; l < laminate.layer_count; l++) {
        double t = laminate.thickness[l];
        double z_bottom = z;
        double z_top = z + t;
        double theta = laminate.angle[l] * 3.14159 / 180.0;
        double c = dcos(theta), s = dcos(3.14159 / 2.0 - theta);
        double Q11 = laminate.E1[l] / (1.0 - laminate.nu12[l] * laminate.nu12[l]);
        double Q12 = laminate.nu12[l] * Q11;
        double Q22 = laminate.E2[l] / (1.0 - laminate.nu12[l] * laminate.nu12[l]);
        double Q66 = laminate.G12[l];
        double A11 = Q11*c*c*c*c + 2.0*(Q12+2.0*Q66)*s*s*c*c + Q22*s*s*s*s;
        double A12 = (Q11+Q22-4.0*Q66)*s*s*c*c + Q12*(s*s*s*s+c*c*c*c);
        double A22 = Q11*s*s*s*s + 2.0*(Q12+2.0*Q66)*s*s*c*c + Q22*c*c*c*c;
        laminate.A[0][0] += A11 * t;
        laminate.A[0][1] += A12 * t;
        laminate.A[1][1] += A22 * t;
        double z_mid = (z_bottom + z_top) * 0.5;
        laminate.B[0][0] += A11 * t * z_mid;
        laminate.D[0][0] += A11 * (z_top*z_top*z_top - z_bottom*z_bottom*z_bottom) / 3.0;
        z = z_top;
    }
    double Ex = (laminate.A[0][0] * laminate.A[1][1] - laminate.A[0][1] * laminate.A[0][1])
                / (z_total * laminate.A[1][1]);
    laminate.sigma_fail = 500.0 + Ex * 0.001;
    mat.laminate_calc_done = 1;
    print_str("[MAT] Laminate ("); print_int(laminate.layer_count); print_str(" layers):\n");
    print_str("  A11="); print_int((int)laminate.A[0][0]);
    print_str(" A12="); print_int((int)laminate.A[0][1]);
    print_str(" A22="); print_int((int)laminate.A[1][1]);
    print_str("\n  Ex="); print_int((int)(Ex/1e9)); print_str("GPa");
    print_str(" sigma_fail="); print_int((int)laminate.sigma_fail); print_str("MPa\n");
}

int mat_add_laminate_layer(double t, double E1, double E2, double nu12, double G12, double angle) {
    if (laminate.layer_count >= MAX_LAYERS) return -1;
    int l = laminate.layer_count;
    laminate.thickness[l] = t;
    laminate.E1[l] = E1; laminate.E2[l] = E2;
    laminate.nu12[l] = nu12; laminate.G12[l] = G12;
    laminate.angle[l] = angle;
    laminate.layer_count++;
    return 0;
}

int mat_add_nanomaterial(int type, double size) {
    if (mat.nano_count >= MAX_NANO) return -1;
    nanomaterial_t* n = &nanos[mat.nano_count];
    n->type = type;
    n->size = size;
    if (type == NANO_DOT) {
        n->bandgap = 1.5 + 2.0 / (size + 0.1);
        n->exciton_energy = n->bandgap + 0.1;
        n->surface_area = 3.14159 * size * size;
        n->quantum_yield = 0.8 - size * 0.01;
    } else if (type == NANO_WIRE) {
        n->bandgap = 1.2 + 0.5 / (size + 0.1);
        n->exciton_energy = n->bandgap + 0.05;
        n->surface_area = 2.0 * 3.14159 * size * 10.0;
        n->quantum_yield = 0.7;
    } else if (type == NANO_TUBE) {
        n->bandgap = 0.5 + 0.3 / (size + 0.1);
        n->exciton_energy = n->bandgap;
        n->surface_area = 2.0 * 3.14159 * size * 100.0;
        n->quantum_yield = 0.5;
    } else {
        n->bandgap = 0.0;
        n->exciton_energy = 0.05;
        n->surface_area = size * size * 2.0;
        n->quantum_yield = 0.3;
    }
    n->active = 1;
    print_str("[MAT] Nano "); print_int(type);
    print_str(" size="); print_int((int)(size * 100)); print_str("nm");
    print_str(" Eg="); print_int((int)(n->bandgap * 100)); print_str("meV\n");
    return mat.nano_count++;
}

void mat_print_state(void) {
    print_str("[MAT] Materials="); print_int(mat.mat_count);
    print_str(" Nano="); print_int(mat.nano_count);
    print_str(" Phases="); print_int(phases.phase_count);
    print_str(" Layers="); print_int(laminate.layer_count);
    print_str(" rho="); print_int((int)mat.avg_density);
    print_str(" E="); print_int((int)(mat.avg_E/1e9));
    print_str("GPa\n");
}

int main(void) {
    print_str("=== Advanced Materials Science Demo ===\n\n");
    mat_init();

    print_str("Adding materials database...\n");
    mat_add_material("Steel-304", MAT_METAL, CRYSTAL_CUBIC,
                     200e9, 0.30, 7900.0, 215e6, 16.0, 1.4e6, 0.001,
                     3.6e-10, 3.6e-10, 3.6e-10, 1700.0);
    mat_add_material("Al-6061", MAT_METAL, CRYSTAL_CUBIC,
                     69e9, 0.33, 2700.0, 280e6, 170.0, 2.5e7, 0.00002,
                     4.05e-10, 4.05e-10, 4.05e-10, 930.0);
    mat_add_material("SiC", MAT_CERAMIC, CRYSTAL_HEX,
                     450e9, 0.17, 3200.0, 3000e6, 120.0, 0.001, 0.00001,
                     3.08e-10, 3.08e-10, 15.1e-10, 3100.0);
    mat_add_material("Ti-6Al-4V", MAT_METAL, CRYSTAL_HEX,
                     114e9, 0.34, 4430.0, 880e6, 7.2, 1e6, 0.0001,
                     2.95e-10, 2.95e-10, 4.68e-10, 1900.0);
    mat_add_material("Carbon-epoxy", MAT_COMPOSITE, CRYSTAL_TRICLIN,
                     135e9, 0.30, 1600.0, 1500e6, 0.5, 0.01, 0.0,
                     5.0e-10, 5.0e-10, 5.0e-10, 450.0);

    print_str("\nComputing average properties...\n");
    mat_compute_properties();

    print_str("\nPhase diagram analysis...\n");
    mat_phase_diagram(300.0, 2000.0, 0.2);
    mat_phase_diagram(300.0, 2000.0, 0.5);
    mat_phase_diagram(300.0, 2000.0, 0.8);

    print_str("\nLaminate composite analysis...\n");
    mat_add_laminate_layer(0.125e-3, 135e9, 10e9, 0.30, 5.0e9, 0.0);
    mat_add_laminate_layer(0.125e-3, 135e9, 10e9, 0.30, 5.0e9, 45.0);
    mat_add_laminate_layer(0.125e-3, 135e9, 10e9, 0.30, 5.0e9, -45.0);
    mat_add_laminate_layer(0.125e-3, 135e9, 10e9, 0.30, 5.0e9, 90.0);
    mat_add_laminate_layer(0.125e-3, 135e9, 10e9, 0.30, 5.0e9, 90.0);
    mat_add_laminate_layer(0.125e-3, 135e9, 10e9, 0.30, 5.0e9, -45.0);
    mat_add_laminate_layer(0.125e-3, 135e9, 10e9, 0.30, 5.0e9, 45.0);
    mat_add_laminate_layer(0.125e-3, 135e9, 10e9, 0.30, 5.0e9, 0.0);
    mat_laminate_analysis();

    print_str("\nNanomaterial characterization...\n");
    mat_add_nanomaterial(NANO_DOT, 3.0);
    mat_add_nanomaterial(NANO_DOT, 5.0);
    mat_add_nanomaterial(NANO_WIRE, 10.0);
    mat_add_nanomaterial(NANO_TUBE, 2.0);
    mat_add_nanomaterial(NANO_SHEET, 100.0);

    print_str("\nRule of mixtures (carbon fiber 60%):\n");
    double E_rom = mat_rule_of_mixtures(0.6, 230e9, 3.5e9);
    double E_irom = mat_inverse_rom(0.6, 230e9, 3.5e9);
    print_str("  Voigt (upper bound): "); print_int((int)(E_rom/1e9)); print_str("GPa\n");
    print_str("  Reuss (lower bound): "); print_int((int)(E_irom/1e9)); print_str("GPa\n");

    mat_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
