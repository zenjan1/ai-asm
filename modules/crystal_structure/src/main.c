/* crystal_structure: Crystallography analysis (v1.0)
 * XRD, lattice parameters, symmetry operations, reciprocal space, defects
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

#define MAX_ATOMS    64
#define MAX_PEAKS    32
#define MAX_SYMMOPS  24
#define MAX_KPOINTS  16
#define MAX_DEFECTS  16

#define LATTICE_CUBIC_P  0x01
#define LATTICE_CUBIC_I  0x02
#define LATTICE_CUBIC_F  0x03
#define LATTICE_HEX      0x04
#define LATTICE_TETRAG   0x05
#define LATTICE_ORTHORH  0x06

#define ATOM_C   0x01
#define ATOM_SI  0x02
#define ATOM_FE  0x03
#define ATOM_CU  0x04
#define ATOM_AL  0x05
#define ATOM_O   0x06
#define ATOM_N   0x07
#define ATOM_TI  0x08
#define ATOM_AU  0x09

#define DEF_VACANCY     0x01
#define DEF_INTERSTITIAL 0x02
#define DEF_SUBSTITUTION 0x03
#define DEF_DISLOCATION 0x04
#define DEF_GRAINBOUND  0x05
#define DEF_STACKFAULT  0x06

#define WAVELENGTH_CUKA  1.5406
#define WAVELENGTH_MOKA  0.7107
#define WAVELENGTH_SYNC  1.0000

typedef struct {
    double x, y, z;
    int    type;
    char   element[4];
    double occupancy;
    double bfactor;
    int    active;
} atom_t;

typedef struct {
    double a, b, c;
    double alpha, beta, gamma;
    int    lattice_type;
    double volume;
    double density;
    int    atom_count;
    double total_mass;
} unit_cell_t;

typedef struct {
    double two_theta;
    double intensity;
    int    h, k, l;
    double d_spacing;
    int    active;
} xrd_peak_t;

typedef struct {
    double matrix[3][3];
    double trace;
    int    order;
    int    type;
} symop_t;

typedef struct {
    double kx, ky, kz;
    double energy;
    double weight;
} kpoint_t;

typedef struct {
    int    type;
    int    atom_site;
    int    new_atom_type;
    double formation_energy;
    double displacement[3];
    int    active;
} defect_t;

typedef struct {
    int    atom_count;
    int    peak_count;
    int    symop_count;
    int    kpoint_count;
    int    defect_count;
    double max_intensity;
    double resolution;
    int    cell_computed;
} xtal_state_t;

static atom_t atoms[MAX_ATOMS];
static unit_cell_t cell;
static xrd_peak_t peaks[MAX_PEAKS];
static symop_t symops[MAX_SYMMOPS];
static kpoint_t kpoints[MAX_KPOINTS];
static defect_t defects[MAX_DEFECTS];
static xtal_state_t xtal;

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
static double dsin(double x) {
    double sum = x, term = x, x2 = x * x;
    for (int i = 1; i < 15; i++) { term *= -x2 / ((2*i)*(2*i+1)); sum += term; }
    return sum;
}

int xtal_init(void) {
    if (initialized) return -1;
    xtal.atom_count = 0; xtal.peak_count = 0; xtal.symop_count = 0;
    xtal.kpoint_count = 0; xtal.defect_count = 0;
    xtal.max_intensity = 0.0; xtal.resolution = 0.0;
    xtal.cell_computed = 0;
    for (int i = 0; i < MAX_ATOMS; i++) atoms[i].active = 0;
    for (int i = 0; i < MAX_DEFECTS; i++) defects[i].active = 0;
    cell.atom_count = 0; cell.volume = 0.0;
    initialized = 1;
    print_str("[XTAL] Crystallography module initialized\n");
    return 0;
}

int xtal_set_cell(double a, double b, double c, double alpha, double beta, double gamma, int type) {
    cell.a = a; cell.b = b; cell.c = c;
    cell.alpha = alpha; cell.beta = beta; cell.gamma = gamma;
    cell.lattice_type = type;
    double ca = dcos(alpha * 3.14159 / 180.0);
    double cb = dcos(beta * 3.14159 / 180.0);
    double cg = dcos(gamma * 3.14159 / 180.0);
    cell.volume = a * b * c * dsqrt(1.0 - ca*ca - cb*cb - cg*cg + 2.0*ca*cb*cg);
    xtal.cell_computed = 1;
    print_str("[XTAL] Unit cell: a="); print_int((int)(a * 1e10));
    print_str("E-10m V="); print_int((int)(cell.volume * 1e30));
    print_str("E-30m3\n");
    return 0;
}

int xtal_add_atom(double x, double y, double z, int type, const char* element) {
    if (xtal.atom_count >= MAX_ATOMS) return -1;
    atom_t* a = &atoms[xtal.atom_count];
    a->x = x; a->y = y; a->z = z;
    a->type = type;
    int i = 0;
    while (i < 3 && element[i]) { a->element[i] = element[i]; i++; }
    a->element[i] = '\0';
    a->occupancy = 1.0;
    a->bfactor = 0.5;
    a->active = 1;
    return xtal.atom_count++;
}

void xtal_compute_density(void) {
    double total_mass = 0.0;
    for (int i = 0; i < xtal.atom_count; i++) {
        double aw = 12.0;
        if (atoms[i].type == ATOM_SI) aw = 28.09;
        else if (atoms[i].type == ATOM_FE) aw = 55.85;
        else if (atoms[i].type == ATOM_CU) aw = 63.55;
        else if (atoms[i].type == ATOM_AL) aw = 26.98;
        else if (atoms[i].type == ATOM_O) aw = 16.00;
        else if (atoms[i].type == ATOM_TI) aw = 47.87;
        else if (atoms[i].type == ATOM_AU) aw = 196.97;
        total_mass += aw * atoms[i].occupancy;
    }
    double NA = 6.022e23;
    double V_cm3 = cell.volume * 1e-24;
    cell.density = total_mass / (NA * V_cm3);
    cell.total_mass = total_mass;
    cell.atom_count = xtal.atom_count;
    print_str("[XTAL] Density: "); print_int((int)(cell.density * 1000));
    print_str(" kg/m3 ("); print_int((int)cell.density); print_str(" g/cm3)\n");
}

void xtal_compute_xrd(double wavelength, int max_2theta) {
    xtal.peak_count = 0;
    xtal.max_intensity = 0.0;
    double theta_max = max_2theta * 3.14159 / 360.0;
    for (int h = 0; h <= 4 && xtal.peak_count < MAX_PEAKS; h++) {
        for (int k = 0; k <= 4 && xtal.peak_count < MAX_PEAKS; k++) {
            for (int l = 0; l <= 4 && xtal.peak_count < MAX_PEAKS; l++) {
                if (h == 0 && k == 0 && l == 0) continue;
                double sa = dcos(cell.alpha * 3.14159 / 180.0);
                double sb = dcos(cell.beta  * 3.14159 / 180.0);
                double sg = dcos(cell.gamma * 3.14159 / 180.0);
                double inv_d2 = (h*h / (cell.a*cell.a) * (1.0 - sa*sa)
                               + k*k / (cell.b*cell.b) * (1.0 - sb*sb)
                               + l*l / (cell.c*cell.c) * (1.0 - sg*sg)) * 1e20;
                if (inv_d2 <= 0.0) continue;
                double d = 1.0 / dsqrt(inv_d2);
                double sin_theta = wavelength / (2.0 * d);
                if (dabs(sin_theta) > 1.0) continue;
                double theta = 0.0;
                double g = sin_theta, p = sin_theta;
                for (int i = 0; i < 20; i++) { g = (g + p) * 0.5; p = sin_theta / g; }
                theta = g;
                double two_theta = 2.0 * theta * 180.0 / 3.14159;
                if (two_theta > max_2theta) continue;
                double F = 0.0;
                for (int i = 0; i < xtal.atom_count; i++) {
                    double phase = 2.0 * 3.14159 * (h * atoms[i].x + k * atoms[i].y + l * atoms[i].z);
                    F += atoms[i].occupancy * dcos(phase);
                }
                double intensity = F * F;
                xrd_peak_t* pk = &peaks[xtal.peak_count];
                pk->two_theta = two_theta;
                pk->intensity = intensity;
                pk->h = h; pk->k = k; pk->l = l;
                pk->d_spacing = d;
                pk->active = 1;
                if (intensity > xtal.max_intensity) xtal.max_intensity = intensity;
                xtal.peak_count++;
            }
        }
    }
    print_str("[XTAL] XRD pattern: "); print_int(xtal.peak_count);
    print_str(" peaks (lambda="); print_int((int)(wavelength * 10000));
    print_str("E-4A)\n");
}

void xtal_print_xrd(void) {
    for (int i = 0; i < xtal.peak_count && i < 8; i++) {
        xrd_peak_t* pk = &peaks[i];
        double rel_int = xtal.max_intensity > 0.0 ? pk->intensity / xtal.max_intensity * 100.0 : 0.0;
        print_str("  ("); print_int(pk->h);
        print_int(pk->k); print_int(pk->l);
        print_str(") 2theta="); print_int((int)(pk->two_theta * 100));
        print_str(" d="); print_int((int)(pk->d_spacing * 1e10));
        print_str("E-2nm I="); print_int((int)rel_int); print_str("%\n");
    }
}

void xtal_generate_symmetry(void) {
    xtal.symop_count = 0;
    double identity[3][3] = {{1,0,0},{0,1,0},{0,0,1}};
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            symops[0].matrix[i][j] = identity[i][j];
    symops[0].trace = 3.0; symops[0].order = 1; symops[0].type = 0;
    xtal.symop_count++;
    if (cell.lattice_type == LATTICE_CUBIC_P || cell.lattice_type == LATTICE_CUBIC_F ||
        cell.lattice_type == LATTICE_CUBIC_I) {
        double inv4[3][3] = {{-1,0,0},{0,-1,0},{0,0,-1}};
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                symops[xtal.symop_count].matrix[i][j] = inv4[i][j];
        symops[xtal.symop_count].trace = -3.0;
        symops[xtal.symop_count].order = 2;
        symops[xtal.symop_count].type = 1;
        xtal.symop_count++;
        double c4z[3][3] = {{0,-1,0},{1,0,0},{0,0,1}};
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                symops[xtal.symop_count].matrix[i][j] = c4z[i][j];
        symops[xtal.symop_count].trace = 1.0;
        symops[xtal.symop_count].order = 4;
        symops[xtal.symop_count].type = 2;
        xtal.symop_count++;
        double c3[3][3] = {{0,0,1},{1,0,0},{0,1,0}};
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                symops[xtal.symop_count].matrix[i][j] = c3[i][j];
        symops[xtal.symop_count].trace = 0.0;
        symops[xtal.symop_count].order = 3;
        symops[xtal.symop_count].type = 3;
        xtal.symop_count++;
        double c2z[3][3] = {{-1,0,0},{0,-1,0},{0,0,1}};
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                symops[xtal.symop_count].matrix[i][j] = c2z[i][j];
        symops[xtal.symop_count].trace = -1.0;
        symops[xtal.symop_count].order = 2;
        symops[xtal.symop_count].type = 4;
        xtal.symop_count++;
    }
    print_str("[XTAL] Symmetry operations: "); print_int(xtal.symop_count); print_str("\n");
}

void xtal_generate_kpoints(int nk) {
    xtal.kpoint_count = 0;
    for (int i = 0; i < nk && xtal.kpoint_count < MAX_KPOINTS; i++) {
        for (int j = 0; j < nk && xtal.kpoint_count < MAX_KPOINTS; j++) {
            for (int k = 0; k < nk && xtal.kpoint_count < MAX_KPOINTS; k++) {
                kpoints[xtal.kpoint_count].kx = (double)i / nk * 0.5;
                kpoints[xtal.kpoint_count].ky = (double)j / nk * 0.5;
                kpoints[xtal.kpoint_count].kz = (double)k / nk * 0.5;
                double k2 = kpoints[xtal.kpoint_count].kx * kpoints[xtal.kpoint_count].kx
                          + kpoints[xtal.kpoint_count].ky * kpoints[xtal.kpoint_count].ky
                          + kpoints[xtal.kpoint_count].kz * kpoints[xtal.kpoint_count].kz;
                kpoints[xtal.kpoint_count].energy = dsqrt(k2) * 10.0;
                kpoints[xtal.kpoint_count].weight = 1.0 / (nk * nk * nk);
                xtal.kpoint_count++;
            }
        }
    }
    print_str("[XTAL] k-point mesh: "); print_int(nk); print_str("x"); print_int(nk);
    print_str("x"); print_int(nk); print_str(" = "); print_int(xtal.kpoint_count);
    print_str(" points\n");
}

int xtal_add_defect(int type, int atom_site, int new_type) {
    if (xtal.defect_count >= MAX_DEFECTS) return -1;
    defect_t* d = &defects[xtal.defect_count];
    d->type = type;
    d->atom_site = atom_site;
    d->new_atom_type = new_type;
    if (type == DEF_VACANCY) d->formation_energy = 2.5;
    else if (type == DEF_INTERSTITIAL) d->formation_energy = 3.5;
    else if (type == DEF_SUBSTITUTION) d->formation_energy = 1.5;
    else if (type == DEF_DISLOCATION) d->formation_energy = 5.0;
    else if (type == DEF_GRAINBOUND) d->formation_energy = 0.8;
    else d->formation_energy = 1.0;
    d->displacement[0] = 0.01;
    d->displacement[1] = 0.0;
    d->displacement[2] = 0.0;
    d->active = 1;
    return xtal.defect_count++;
}

double xtal_bond_length(int i, int j) {
    if (i >= xtal.atom_count || j >= xtal.atom_count) return 0.0;
    double dx = (atoms[i].x - atoms[j].x) * cell.a;
    double dy = (atoms[i].y - atoms[j].y) * cell.b;
    double dz = (atoms[i].z - atoms[j].z) * cell.c;
    return dsqrt(dx*dx + dy*dy + dz*dz);
}

void xtal_coordination_analysis(void) {
    double cutoff = 3.0e-10;
    print_str("[XTAL] Coordination analysis (cutoff="); print_int((int)(cutoff * 1e10));
    print_str("E-10m):\n");
    for (int i = 0; i < xtal.atom_count && i < 4; i++) {
        int coord = 0;
        for (int j = 0; j < xtal.atom_count; j++) {
            if (i == j) continue;
            double d = xtal_bond_length(i, j);
            if (d < cutoff && d > 0.5e-10) coord++;
        }
        print_str("  Atom "); print_int(i); print_str(" ("); print_str(atoms[i].element);
        print_str("): CN="); print_int(coord); print_str("\n");
    }
}

void xtal_print_state(void) {
    print_str("[XTAL] Atoms="); print_int(xtal.atom_count);
    print_str(" Peaks="); print_int(xtal.peak_count);
    print_str(" SymOps="); print_int(xtal.symop_count);
    print_str(" kPts="); print_int(xtal.kpoint_count);
    print_str(" Defs="); print_int(xtal.defect_count);
    print_str("\n");
}

int main(void) {
    print_str("=== Crystal Structure Analysis Demo ===\n\n");
    xtal_init();

    print_str("Setting up FCC Aluminum unit cell...\n");
    xtal_set_cell(4.05e-10, 4.05e-10, 4.05e-10, 90.0, 90.0, 90.0, LATTICE_CUBIC_F);
    xtal_add_atom(0.0, 0.0, 0.0, ATOM_AL, "Al");
    xtal_add_atom(0.5, 0.5, 0.0, ATOM_AL, "Al");
    xtal_add_atom(0.5, 0.0, 0.5, ATOM_AL, "Al");
    xtal_add_atom(0.0, 0.5, 0.5, ATOM_AL, "Al");
    xtal_compute_density();

    print_str("\nXRD pattern (Cu Ka)...\n");
    xtal_compute_xrd(WAVELENGTH_CUKA, 90);
    xtal_print_xrd();

    print_str("\nSymmetry operations...\n");
    xtal_generate_symmetry();

    print_str("\nk-point generation...\n");
    xtal_generate_kpoints(4);

    print_str("\nBond length analysis...\n");
    for (int i = 0; i < xtal.atom_count && i < 3; i++) {
        for (int j = i + 1; j < xtal.atom_count && j < 4; j++) {
            double d = xtal_bond_length(i, j);
            print_str("  d("); print_int(i); print_str(","); print_int(j);
            print_str(")="); print_int((int)(d * 1e10)); print_str("E-10m\n");
        }
    }

    print_str("\nCoordination analysis...\n");
    xtal_coordination_analysis();

    print_str("\nDefect creation...\n");
    xtal_add_defect(DEF_VACANCY, 0, 0);
    xtal_add_defect(DEF_INTERSTITIAL, -1, ATOM_AL);
    xtal_add_defect(DEF_SUBSTITUTION, 1, ATOM_CU);
    for (int i = 0; i < xtal.defect_count; i++) {
        print_str("  Defect "); print_int(i+1);
        print_str(": type="); print_int(defects[i].type);
        print_str(" Ef="); print_int((int)(defects[i].formation_energy * 100));
        print_str("meV\n");
    }

    print_str("\nSetting up BCC Iron cell...\n");
    xtal_set_cell(2.87e-10, 2.87e-10, 2.87e-10, 90.0, 90.0, 90.0, LATTICE_CUBIC_I);
    xtal_add_atom(0.0, 0.0, 0.0, ATOM_FE, "Fe");
    xtal_add_atom(0.5, 0.5, 0.5, ATOM_FE, "Fe");
    xtal_compute_density();
    xtal_compute_xrd(WAVELENGTH_CUKA, 120);
    xtal_print_xrd();

    xtal_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
