/* semiconductor_simulation: Semiconductor device modeling (v1.0)
 * Band structure, carrier transport, PN junction, MOSFET, optoelectronics
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

#define MAX_REGIONS  8
#define MAX_POINTS   64
#define MAX_BANDS    16
#define MAX_LAYERS   16

#define MAT_SI    0x01
#define MAT_GASI  0x02
#define MAT_GAP   0x03
#define MAT_GAN   0x04
#define MAT_SIC   0x05
#define MAT_INP   0x06
#define MAT_GE    0x07

#define DOP_NONE  0x00
#define DOP_P     0x01
#define DOP_N     0x02

#define DEV_PN    0x01
#define DEV_MOSFET 0x02
#define DEV_BJT   0x03
#define DEV_SOLAR 0x04
#define DEV_LED   0x05
#define DEV_LASER 0x06

#define TB_S      0x01
#define TB_SP3    0x02
#define KP_2BAND  0x03
#define KP_4BAND  0x04

typedef struct {
    int    type;
    double Eg;
    double chi;
    double eps;
    double Nc;
    double Nv;
    double mu_n;
    double mu_p;
    double ni;
    double m_eff_n;
    double m_eff_p;
    int    doping;
    double Na, Nd;
    double Eg300;
    double alpha_T;
    double beta_T;
} semi_material_t;

typedef struct {
    int    dev_type;
    int    mat_type;
    double W_depletion;
    double V_bi;
    double V_breakdown;
    double J_s;
    double C_j;
    double R_series;
    double ideality;
    double T;
    int    computed;
} pn_junction_t;

typedef struct {
    double L_gate;
    double t_ox;
    double V_th;
    double V_ds;
    double V_gs;
    double I_ds;
    double g_m;
    double subthreshold_slope;
    double DIBL;
    double mu_eff;
    double C_ox;
    int    computed;
} mosfet_t;

typedef struct {
    double k[MAX_BANDS];
    double E[MAX_BANDS][MAX_POINTS];
    int    band_count;
    int    k_count;
    double m_eff_cb;
    double m_eff_hh;
    double m_eff_lh;
    int    method;
    int    computed;
} band_structure_t;

typedef struct {
    double n[MAX_POINTS];
    double p[MAX_POINTS];
    double Jn[MAX_POINTS];
    double Jp[MAX_POINTS];
    double E_field[MAX_POINTS];
    int    points;
    double V_applied;
    int    computed;
} transport_t;

typedef struct {
    double J_sc;
    double V_oc;
    double FF;
    double efficiency;
    double spectrum_absorbed;
    double quantum_eff;
    int    computed;
} solar_cell_t;

typedef struct {
    double photon_energy;
    double wavelength;
    double gain;
    double threshold_current;
    double slope_efficiency;
    double output_power;
    int    computed;
} led_laser_t;

static semi_material_t mat_db[8];
static pn_junction_t pn;
static mosfet_t mos;
static band_structure_t bands;
static transport_t transport;
static solar_cell_t solar;
static led_laser_t opto;

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
static double dexp(double x) {
    if (x > 20.0) return 5e8;
    if (x < -20.0) return 0.0;
    double sum = 1.0, term = 1.0;
    for (int i = 1; i < 30; i++) { term *= x / i; sum += term; }
    return sum;
}
static double dlog(double x) {
    if (x <= 0.0) return -100.0;
    double y = (x - 1.0) / (x + 1.0);
    double y2 = y * y;
    double sum = 0.0, term = y;
    for (int i = 0; i < 20; i++) {
        sum += term / (2 * i + 1);
        term *= y2;
    }
    return 2.0 * sum;
}

int semi_init(void) {
    if (initialized) return -1;
    mat_db[0].type = MAT_SI;   mat_db[0].Eg = 1.12;  mat_db[0].chi = 4.05;
    mat_db[0].eps = 11.9; mat_db[0].Nc = 2.8e19; mat_db[0].Nv = 1.04e19;
    mat_db[0].mu_n = 1400.0; mat_db[0].mu_p = 450.0; mat_db[0].ni = 1.5e10;
    mat_db[0].m_eff_n = 0.26; mat_db[0].m_eff_p = 0.39;
    mat_db[0].doping = DOP_NONE; mat_db[0].Na = 0; mat_db[0].Nd = 0;
    mat_db[0].Eg300 = 1.12; mat_db[0].alpha_T = 4.73e-4; mat_db[0].beta_T = 636.0;

    mat_db[1].type = MAT_GASI; mat_db[1].Eg = 1.42;  mat_db[1].chi = 4.07;
    mat_db[1].eps = 12.9; mat_db[1].Nc = 4.7e17; mat_db[1].Nv = 7.0e18;
    mat_db[1].mu_n = 8500.0; mat_db[1].mu_p = 400.0; mat_db[1].ni = 2.1e6;
    mat_db[1].m_eff_n = 0.067; mat_db[1].m_eff_p = 0.48;
    mat_db[1].doping = DOP_NONE; mat_db[1].Na = 0; mat_db[1].Nd = 0;
    mat_db[1].Eg300 = 1.42; mat_db[1].alpha_T = 5.41e-4; mat_db[1].beta_T = 204.0;

    mat_db[2].type = MAT_GAP;  mat_db[2].Eg = 2.26;  mat_db[2].chi = 3.80;
    mat_db[2].eps = 11.1; mat_db[2].Nc = 2.1e18; mat_db[2].Nv = 1.5e19;
    mat_db[2].mu_n = 300.0; mat_db[2].mu_p = 150.0; mat_db[2].ni = 1.0;
    mat_db[2].m_eff_n = 0.15; mat_db[2].m_eff_p = 0.60;
    mat_db[2].doping = DOP_NONE; mat_db[2].Na = 0; mat_db[2].Nd = 0;
    mat_db[2].Eg300 = 2.26; mat_db[2].alpha_T = 5.0e-4; mat_db[2].beta_T = 300.0;

    mat_db[3].type = MAT_GAN;  mat_db[3].Eg = 3.40;  mat_db[3].chi = 4.10;
    mat_db[3].eps = 9.5; mat_db[3].Nc = 2.2e18; mat_db[3].Nv = 3.2e19;
    mat_db[3].mu_n = 1500.0; mat_db[3].mu_p = 30.0; mat_db[3].ni = 1.9e-10;
    mat_db[3].m_eff_n = 0.20; mat_db[3].m_eff_p = 1.40;
    mat_db[3].doping = DOP_NONE; mat_db[3].Na = 0; mat_db[3].Nd = 0;
    mat_db[3].Eg300 = 3.40; mat_db[3].alpha_T = 7.0e-4; mat_db[3].beta_T = 600.0;

    mat_db[4].type = MAT_SIC;  mat_db[4].Eg = 3.26;  mat_db[4].chi = 3.60;
    mat_db[4].eps = 10.0; mat_db[4].Nc = 1.5e20; mat_db[4].Nv = 2.0e20;
    mat_db[4].mu_n = 900.0; mat_db[4].mu_p = 115.0; mat_db[4].ni = 6.7e-4;
    mat_db[4].m_eff_n = 0.37; mat_db[4].m_eff_p = 2.0;
    mat_db[4].doping = DOP_NONE; mat_db[4].Na = 0; mat_db[4].Nd = 0;
    mat_db[4].Eg300 = 3.26; mat_db[4].alpha_T = 4.0e-4; mat_db[4].beta_T = 800.0;

    pn.computed = 0; mos.computed = 0; bands.computed = 0;
    transport.computed = 0; solar.computed = 0; opto.computed = 0;
    pn.T = 300.0;
    initialized = 1;
    print_str("[SEMI] Semiconductor module initialized\n");
    return 0;
}

double semi_bandgap_T(int mat_id, double T) {
    semi_material_t* m = &mat_db[mat_id];
    return m->Eg300 - m->alpha_T * (T * T) / (T + m->beta_T);
}

void semi_compute_bands(int mat_id, int method) {
    bands.method = method;
    bands.band_count = 4;
    bands.k_count = 16;
    double Eg = mat_db[mat_id].Eg;
    double me_n = mat_db[mat_id].m_eff_n;
    double me_h = mat_db[mat_id].m_eff_p;
    for (int ik = 0; ik < bands.k_count; ik++) {
        double k = (double)ik / (bands.k_count - 1) * 0.5;
        bands.k[ik] = k;
        if (method == TB_S) {
            double Ec = Eg + 3.0 * (1.0 - dcos(k * 3.14159)) / (2.0 * me_n);
            double Ev = -1.5 * (1.0 - dcos(k * 3.14159)) / (2.0 * me_h);
            bands.E[0][ik] = Ec;
            bands.E[1][ik] = Ec + 0.5;
            bands.E[2][ik] = Ev;
            bands.E[3][ik] = Ev - 0.3;
        } else {
            double k2 = k * k;
            double Ec = Eg + k2 / me_n;
            double Ehh = -k2 / me_h;
            double Elh = -k2 / (me_h * 0.5);
            bands.E[0][ik] = Ec;
            bands.E[1][ik] = Ec + 0.3;
            bands.E[2][ik] = Ehh;
            bands.E[3][ik] = Elh;
        }
    }
    double dE = bands.E[0][1] - bands.E[0][0];
    double dk = bands.k[1] - bands.k[0];
    double hbar = 1.054e-34;
    double q = 1.602e-19;
    double m0 = 9.109e-31;
    bands.m_eff_cb = hbar * hbar / (dE / dk / dk * q * m0 + 1e-30);
    double dE2 = bands.E[2][0] - bands.E[2][1];
    bands.m_eff_hh = hbar * hbar / (dabs(dE2) / dk / dk * q * m0 + 1e-30);
    bands.computed = 1;
    print_str("[SEMI] Band structure (");
    if (method == TB_S) print_str("tight-binding s");
    else print_str("k.p 2-band");
    print_str(") mat="); print_int(mat_id); print_str("\n");
    print_str("  m*_CB="); print_int((int)(bands.m_eff_cb * 1000));
    print_str(" m*_HH="); print_int((int)(bands.m_eff_hh * 1000)); print_str("\n");
}

void semi_print_bands(void) {
    print_str("[SEMI] E(k) dispersion:\n");
    print_str("  k/A-1   CB    CB+1   VB    LH\n");
    for (int ik = 0; ik < bands.k_count; ik += 3) {
        print_str("  "); print_int((int)(bands.k[ik] * 100));
        for (int b = 0; b < 4; b++) {
            print_str(" "); print_int((int)(bands.E[b][ik] * 100));
        }
        print_str("\n");
    }
}

void semi_pn_junction(int mat_id, double Na, double Nd, double T) {
    semi_material_t* m = &mat_db[mat_id];
    pn.mat_type = mat_id;
    pn.dev_type = DEV_PN;
    pn.T = T;
    double Eg_T = semi_bandgap_T(mat_id, T);
    double ni_T = m->ni * dexp(-Eg_T * 0.5 * 1.602e-19 / (1.38e-23 * T)
                               + Eg_T * 0.5 * 1.602e-19 / (1.38e-23 * 300.0));
    if (dabs(ni_T) < 1e-30) ni_T = m->ni;
    double Vt = 1.38e-23 * T / 1.602e-19;
    pn.V_bi = Vt * dlog(Na * Nd / (ni_T * ni_T + 1e-30));
    if (pn.V_bi > Eg_T) pn.V_bi = Eg_T;
    double eps_s = m->eps * 8.854e-12;
    pn.W_depletion = dsqrt(2.0 * eps_s * pn.V_bi * (Na + Nd) / (1.602e-19 * Na * Nd + 1e-30));
    double Dn_default = m->mu_n * Vt;
    double Dp_default = m->mu_p * Vt;
    double Ln = dsqrt(Dn_default * 1e-6);
    double Lp = dsqrt(Dp_default * 1e-6);
    pn.J_s = 1.602e-19 * ni_T * ni_T * (Dn_default / (Ln + 1e-10) + Dp_default / (Lp + 1e-10));
    pn.V_breakdown = 50.0 + Eg_T * 20.0;
    pn.C_j = eps_s / (pn.W_depletion + 1e-10);
    pn.R_series = 0.01;
    pn.ideality = 1.2;
    pn.computed = 1;
    print_str("[SEMI] PN junction (mat="); print_int(mat_id);
    print_str(" T="); print_int((int)T); print_str("K):\n");
    print_str("  Vbi="); print_int((int)(pn.V_bi * 1000)); print_str("mV");
    print_str(" W="); print_int((int)(pn.W_depletion * 1e6)); print_str("um");
    print_str(" Vbr="); print_int((int)pn.V_breakdown); print_str("V\n");
}

void semi_mosfet(int mat_id, double L_gate, double t_ox, double Na, double V_ds) {
    semi_material_t* m = &mat_db[mat_id];
    mos.L_gate = L_gate;
    mos.t_ox = t_ox;
    mos.V_ds = V_ds;
    double eps_ox = 3.9 * 8.854e-12;
    mos.C_ox = eps_ox / t_ox;
    double Vt = 1.38e-23 * 300.0 / 1.602e-19;
    double phi_f = Vt * dlog(Na / (m->ni + 1e-30));
    double eps_s = m->eps * 8.854e-12;
    double V_dep = dsqrt(2.0 * eps_s * 1.602e-19 * Na * phi_f * 2.0) / mos.C_ox;
    mos.V_th = m->chi + m->Eg * 0.5 + phi_f + V_dep;
    mos.mu_eff = m->mu_n * 0.6;
    mos.V_gs = mos.V_th + 1.0;
    double Vov = mos.V_gs - mos.V_th;
    if (Vov < 0) Vov = 0;
    if (mos.V_ds > Vov) {
        mos.I_ds = 0.5 * mos.mu_eff * mos.C_ox / L_gate * Vov * Vov;
    } else {
        mos.I_ds = mos.mu_eff * mos.C_ox / L_gate * (Vov * mos.V_ds - 0.5 * mos.V_ds * mos.V_ds);
    }
    mos.g_m = mos.mu_eff * mos.C_ox / L_gate * (Vov > mos.V_ds ? mos.V_ds : Vov);
    mos.subthreshold_slope = 80.0 + t_ox * 1e9 * 0.5;
    mos.DIBL = 50.0 / (L_gate * 1e9);
    mos.computed = 1;
    print_str("[SEMI] MOSFET (mat="); print_int(mat_id);
    print_str(" L="); print_int((int)(L_gate * 1e9)); print_str("nm");
    print_str(" tox="); print_int((int)(t_ox * 1e9)); print_str("nm):\n");
    print_str("  Vth="); print_int((int)(mos.V_th * 1000)); print_str("mV");
    print_str(" Ids="); print_int((int)(mos.I_ds * 1e6)); print_str("uA");
    print_str(" gm="); print_int((int)(mos.g_m * 1e6)); print_str("uS\n");
    print_str("  SS="); print_int((int)mos.subthreshold_slope); print_str("mV/dec");
    print_str(" DIBL="); print_int((int)mos.DIBL); print_str("mV/V\n");
}

void semi_drift_diffusion(int mat_id, double V_applied) {
    semi_material_t* m = &mat_db[mat_id];
    transport.points = 16;
    transport.V_applied = V_applied;
    double Vt = 1.38e-23 * 300.0 / 1.602e-19;
    double L = 1e-4;
    double dx = L / transport.points;
    for (int i = 0; i < transport.points; i++) {
        double x = (double)i / (transport.points - 1);
        double phi = V_applied * x;
        double n = m->Nd > 0 ? m->Nd * dexp(-phi / Vt) : m->ni;
        double p = m->Na > 0 ? m->Na * dexp(phi / Vt) : m->ni;
        transport.n[i] = n;
        transport.p[i] = p;
        double E = V_applied / L;
        transport.E_field[i] = E;
        transport.Jn[i] = 1.602e-19 * m->mu_n * n * E;
        transport.Jp[i] = 1.602e-19 * m->mu_p * p * E;
    }
    transport.computed = 1;
    print_str("[SEMI] Drift-diffusion (V=");
    print_int((int)(V_applied * 1000)); print_str("mV):\n");
    print_str("  x     n/cm3    p/cm3    Jn     Jp\n");
    for (int i = 0; i < transport.points; i += 4) {
        print_str("  "); print_int((int)((double)i / transport.points * 100));
        print_str("%  "); print_int((int)(transport.n[i] / 1e10));
        print_str("E10 "); print_int((int)(transport.p[i] / 1e10));
        print_str("E10 "); print_int((int)(transport.Jn[i] * 1e4));
        print_str(" "); print_int((int)(transport.Jp[i] * 1e4)); print_str("\n");
    }
}

void semi_solar_cell(int mat_id, double illumination) {
    semi_material_t* m = &mat_db[mat_id];
    solar.quantum_eff = 0.85;
    double Eg = m->Eg;
    double P_in = illumination * 100.0;
    double N_photons = P_in / (Eg * 1.602e-19 + 1e-30);
    solar.J_sc = 1.602e-19 * N_photons * solar.quantum_eff * 0.01;
    solar.V_oc = Eg - 0.3;
    solar.FF = 0.75 - Eg * 0.05;
    solar.efficiency = solar.J_sc * solar.V_oc * solar.FF / (P_in + 1e-10);
    solar.spectrum_absorbed = illumination * (1.0 - Eg / 3.0);
    solar.computed = 1;
    print_str("[SEMI] Solar cell (mat="); print_int(mat_id);
    print_str(" Eg="); print_int((int)(Eg * 1000)); print_str("meV):\n");
    print_str("  Jsc="); print_int((int)(solar.J_sc * 1000)); print_str("mA/cm2");
    print_str(" Voc="); print_int((int)(solar.V_oc * 1000)); print_str("mV");
    print_str(" FF="); print_int((int)(solar.FF * 100)); print_str("%\n");
    print_str("  Efficiency="); print_int((int)(solar.efficiency * 100));
    print_str("%\n");
}

void semi_led(int mat_id, double current_density) {
    semi_material_t* m = &mat_db[mat_id];
    opto.photon_energy = m->Eg;
    opto.wavelength = 1.24 / m->Eg;
    opto.gain = current_density * 0.01;
    opto.threshold_current = 100.0 + m->Eg * 50.0;
    double I_above = current_density - opto.threshold_current;
    if (I_above < 0) I_above = 0;
    opto.slope_efficiency = 0.5 * m->Eg;
    opto.output_power = opto.slope_efficiency * I_above;
    opto.computed = 1;
    print_str("[SEMI] LED/Laser (mat="); print_int(mat_id);
    print_str(" lambda="); print_int((int)(opto.wavelength * 1000)); print_str("nm):\n");
    print_str("  Ith="); print_int((int)opto.threshold_current); print_str("A/cm2");
    print_str(" P="); print_int((int)(opto.output_power * 1000)); print_str("mW/cm2\n");
}

void semi_print_state(void) {
    print_str("[SEMI] PN="); print_int(pn.computed);
    print_str(" MOS="); print_int(mos.computed);
    print_str(" Bands="); print_int(bands.computed);
    print_str(" Transport="); print_int(transport.computed);
    print_str(" Solar="); print_int(solar.computed);
    print_str(" LED="); print_int(opto.computed);
    print_str("\n");
}

int main(void) {
    print_str("=== Semiconductor Device Simulation Demo ===\n\n");
    semi_init();

    print_str("Silicon band structure (tight-binding)...\n");
    semi_compute_bands(0, TB_S);
    semi_print_bands();

    print_str("\nGaAs band structure (k.p)...\n");
    semi_compute_bands(1, KP_2BAND);
    semi_print_bands();

    print_str("\nSi PN junction at 300K...\n");
    semi_pn_junction(0, 1e17, 1e16, 300.0);

    print_str("\nSi PN junction at 400K...\n");
    semi_pn_junction(0, 1e17, 1e16, 400.0);

    print_str("\nSi MOSFET (45nm)...\n");
    semi_mosfet(0, 45e-9, 1.5e-9, 1e17, 1.0);

    print_str("\nSi MOSFET (7nm)...\n");
    semi_mosfet(0, 7e-9, 1.0e-9, 1e18, 0.7);

    print_str("\nDrift-diffusion in Si...\n");
    semi_drift_diffusion(0, 0.5);

    print_str("\nGaAs solar cell...\n");
    semi_solar_cell(1, 100.0);

    print_str("\nSi solar cell...\n");
    semi_solar_cell(0, 100.0);

    print_str("\nGaN LED...\n");
    semi_led(3, 500.0);

    print_str("\nGaAs LED...\n");
    semi_led(1, 200.0);

    semi_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
