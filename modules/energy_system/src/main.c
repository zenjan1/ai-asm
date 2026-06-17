/* energy_system: Power system analysis (v1.0)
 * Load flow, stability, fault analysis, optimal power flow
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

#define MAX_BUSES    32
#define MAX_LINES    32
#define MAX_GENS     16
#define MAX_LOADS    32

#define BUS_PQ     0x01
#define BUS_PV     0x02
#define BUS_SLACK  0x03

#define LINE_OHL   0x01
#define LINE_CABLE 0x02
#define LINE_XFMR  0x03

#define FAULT_3PH  0x01
#define FAULT_SLG  0x02
#define FAULT_LL   0x03
#define FAULT_DLG  0x04

#define SOLV_NR    0x01
#define SOLV_GS    0x02
#define SOLV_FDPS  0x03
#define SOLV_DC    0x04

typedef struct {
    int    type;
    double V_mag;
    double V_angle;
    double P_gen, Q_gen;
    double P_load, Q_load;
    double P_net, Q_net;
    int    gen_id;
    double V_max, V_min;
    int    active;
} bus_t;

typedef struct {
    int    from, to;
    double R, X, B;
    double rate;
    int    type;
    int    active;
    double P_flow, Q_flow;
    double I_mag;
} line_t;

typedef struct {
    double P_max, Q_max;
    double P_min, Q_min;
    double P_gen, Q_gen;
    double V_set;
    int    bus;
    double cost_a, cost_b, cost_c;
    int    active;
} generator_t;

typedef struct {
    int    bus;
    double P, Q;
    int    priority;
    int    active;
} load_t;

typedef struct {
    double Z_pos[4], Z_neg[4], Z_zero[4];
    double I_fault;
    double V_post[32];
    int    fault_bus;
    int    fault_type;
} fault_result_t;

typedef struct {
    int    bus_count;
    int    line_count;
    int    gen_count;
    int    load_count;
    double base_MVA;
    int    solver;
    double max_mismatch;
    int    iterations;
    int    converged;
    double total_loss_P;
    double total_loss_Q;
    double total_gen_P;
    double total_load_P;
    double total_cost;
    int    lf_computed;
    int    fault_computed;
} sys_state_t;

static bus_t buses[MAX_BUSES];
static line_t lines[MAX_LINES];
static generator_t generators[MAX_GENS];
static load_t loads[MAX_LOADS];
static fault_result_t fault;
static sys_state_t sys;

static int initialized = 0;
static double Y_bus[MAX_BUSES][MAX_BUSES][2];

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

int sys_init(double base_MVA, int solver) {
    if (initialized) return -1;
    sys.bus_count = 0; sys.line_count = 0; sys.gen_count = 0; sys.load_count = 0;
    sys.base_MVA = base_MVA; sys.solver = solver;
    sys.max_mismatch = 0.0; sys.iterations = 0; sys.converged = 0;
    sys.total_loss_P = 0.0; sys.total_loss_Q = 0.0;
    sys.total_gen_P = 0.0; sys.total_load_P = 0.0; sys.total_cost = 0.0;
    sys.lf_computed = 0; sys.fault_computed = 0;
    for (int i = 0; i < MAX_BUSES; i++) { buses[i].active = 0; buses[i].V_mag = 1.0; buses[i].V_angle = 0.0; }
    for (int i = 0; i < MAX_LINES; i++) lines[i].active = 0;
    for (int i = 0; i < MAX_GENS; i++) generators[i].active = 0;
    for (int i = 0; i < MAX_LOADS; i++) loads[i].active = 0;
    for (int i = 0; i < MAX_BUSES; i++)
        for (int j = 0; j < MAX_BUSES; j++) { Y_bus[i][j][0] = 0.0; Y_bus[i][j][1] = 0.0; }
    initialized = 1;
    print_str("[SYS] Power system initialized: base="); print_int((int)base_MVA);
    print_str("MVA solver="); print_int(solver); print_str("\n");
    return 0;
}

int sys_add_bus(int type, double V_set) {
    if (sys.bus_count >= MAX_BUSES) return -1;
    bus_t* b = &buses[sys.bus_count];
    b->type = type;
    b->V_mag = V_set;
    b->V_angle = 0.0;
    b->P_gen = 0.0; b->Q_gen = 0.0;
    b->P_load = 0.0; b->Q_load = 0.0;
    b->P_net = 0.0; b->Q_net = 0.0;
    b->V_max = 1.1; b->V_min = 0.9;
    b->gen_id = -1;
    b->active = 1;
    return sys.bus_count++;
}

int sys_add_line(int from, int to, double R, double X, double B, double rate, int type) {
    if (sys.line_count >= MAX_LINES) return -1;
    line_t* l = &lines[sys.line_count];
    l->from = from; l->to = to;
    l->R = R; l->X = X; l->B = B;
    l->rate = rate; l->type = type;
    l->active = 1;
    l->P_flow = 0.0; l->Q_flow = 0.0; l->I_mag = 0.0;
    return sys.line_count++;
}

int sys_add_generator(int bus, double P_max, double Q_max, double V_set, double ca, double cb, double cc) {
    if (sys.gen_count >= MAX_GENS) return -1;
    generator_t* g = &generators[sys.gen_count];
    g->bus = bus;
    g->P_max = P_max; g->Q_max = Q_max;
    g->P_min = 0.0; g->Q_min = 0.0;
    g->P_gen = P_max * 0.5; g->Q_gen = 0.0;
    g->V_set = V_set;
    g->cost_a = ca; g->cost_b = cb; g->cost_c = cc;
    g->active = 1;
    if (bus >= 0 && bus < sys.bus_count) buses[bus].gen_id = sys.gen_count;
    return sys.gen_count++;
}

int sys_add_load(int bus, double P, double Q, int priority) {
    if (sys.load_count >= MAX_LOADS) return -1;
    load_t* l = &loads[sys.load_count];
    l->bus = bus; l->P = P; l->Q = Q; l->priority = priority;
    l->active = 1;
    return sys.load_count++;
}

void sys_build_admittance(void) {
    for (int i = 0; i < sys.bus_count; i++)
        for (int j = 0; j < sys.bus_count; j++) {
            Y_bus[i][j][0] = 0.0; Y_bus[i][j][1] = 0.0;
        }
    for (int k = 0; k < sys.line_count; k++) {
        if (!lines[k].active) continue;
        int i = lines[k].from, j = lines[k].to;
        double R = lines[k].R, X = lines[k].X, B = lines[k].B;
        double Z2 = R*R + X*X;
        double G = R / Z2, B_line = -X / Z2;
        Y_bus[i][i][0] += G; Y_bus[i][i][1] += B_line + B * 0.5;
        Y_bus[j][j][0] += G; Y_bus[j][j][1] += B_line + B * 0.5;
        Y_bus[i][j][0] -= G; Y_bus[i][j][1] -= B_line;
        Y_bus[j][i][0] -= G; Y_bus[j][i][1] -= B_line;
    }
    print_str("[SYS] Admittance matrix built ("); print_int(sys.bus_count);
    print_str(" buses, "); print_int(sys.line_count); print_str(" lines)\n");
}

void sys_newton_raphson(int max_iter, double tol) {
    sys_build_admittance();
    for (int i = 0; i < sys.bus_count; i++) {
        buses[i].P_net = buses[i].P_gen - buses[i].P_load;
        buses[i].Q_net = buses[i].Q_gen - buses[i].Q_load;
    }
    for (int l = 0; l < sys.load_count; l++) {
        if (!loads[l].active || loads[l].bus >= sys.bus_count) continue;
        buses[loads[l].bus].P_load += loads[l].P;
        buses[loads[l].bus].Q_load += loads[l].Q;
    }
    for (int iter = 0; iter < max_iter; iter++) {
        double max_mm = 0.0;
        for (int i = 0; i < sys.bus_count; i++) {
            if (buses[i].type == BUS_SLACK) continue;
            double P_calc = 0.0, Q_calc = 0.0;
            for (int j = 0; j < sys.bus_count; j++) {
                double Vj = buses[j].V_mag;
                double th_ij = buses[i].V_angle - buses[j].V_angle;
                double Gij = Y_bus[i][j][0], Bij = Y_bus[i][j][1];
                P_calc += Vj * (Gij * dcos(th_ij) + Bij * dsin(th_ij));
                Q_calc += Vj * (Gij * dsin(th_ij) - Bij * dcos(th_ij));
            }
            P_calc *= buses[i].V_mag;
            Q_calc *= buses[i].V_mag;
            double dP = buses[i].P_net - P_calc;
            double dQ = buses[i].Q_net - Q_calc;
            if (dabs(dP) > max_mm) max_mm = dabs(dP);
            if (buses[i].type == BUS_PQ && dabs(dQ) > max_mm) max_mm = dabs(dQ);
            if (buses[i].type == BUS_PQ) {
                buses[i].V_angle += dP * 0.01;
                buses[i].V_mag += dQ * 0.005;
                if (buses[i].V_mag > buses[i].V_max) buses[i].V_mag = buses[i].V_max;
                if (buses[i].V_mag < buses[i].V_min) buses[i].V_mag = buses[i].V_min;
            } else {
                buses[i].V_angle += dP * 0.01;
            }
        }
        sys.max_mismatch = max_mm;
        sys.iterations = iter + 1;
        if (max_mm < tol) { sys.converged = 1; break; }
    }
    sys.lf_computed = 1;
}

void sys_gauss_seidel(int max_iter, double tol) {
    sys_build_admittance();
    for (int l = 0; l < sys.load_count; l++) {
        if (!loads[l].active || loads[l].bus >= sys.bus_count) continue;
        buses[loads[l].bus].P_load += loads[l].P;
        buses[loads[l].bus].Q_load += loads[l].Q;
    }
    for (int i = 0; i < sys.bus_count; i++) {
        buses[i].P_net = buses[i].P_gen - buses[i].P_load;
        buses[i].Q_net = buses[i].Q_gen - buses[i].Q_load;
    }
    for (int iter = 0; iter < max_iter; iter++) {
        double max_mm = 0.0;
        for (int i = 0; i < sys.bus_count; i++) {
            if (buses[i].type == BUS_SLACK) continue;
            double sum_real = 0.0, sum_imag = 0.0;
            for (int j = 0; j < sys.bus_count; j++) {
                if (j == i) continue;
                double Vr = buses[j].V_mag * dcos(buses[j].V_angle);
                double Vi = buses[j].V_mag * dsin(buses[j].V_angle);
                sum_real += Y_bus[i][j][0] * Vr - Y_bus[i][j][1] * Vi;
                sum_imag += Y_bus[i][j][0] * Vi + Y_bus[i][j][1] * Vr;
            }
            double P_sp = buses[i].P_net;
            double Q_sp = buses[i].Q_net;
            double Vi_mag = buses[i].V_mag;
            double I_real = (P_sp * dcos(buses[i].V_angle) + Q_sp * dsin(buses[i].V_angle)) / (Vi_mag + 1e-10);
            double I_imag = (P_sp * dsin(buses[i].V_angle) - Q_sp * dcos(buses[i].V_angle)) / (Vi_mag + 1e-10);
            double new_real = (I_real - sum_real) / (Y_bus[i][i][0] + 1e-10);
            double new_imag = (I_imag - sum_imag) / (Y_bus[i][i][0] + 1e-10);
            double new_mag = dsqrt(new_real * new_real + new_imag * new_imag);
            double new_ang = 0.0;
            if (dabs(new_real) > 1e-10 || dabs(new_imag) > 1e-10) {
                new_ang = new_imag / new_real;
            }
            double dV = dabs(new_mag - buses[i].V_mag);
            if (dV > max_mm) max_mm = dV;
            if (buses[i].type == BUS_PQ) {
                buses[i].V_mag = new_mag * 0.1 + buses[i].V_mag * 0.9;
                buses[i].V_angle = new_ang * 0.1 + buses[i].V_angle * 0.9;
            } else {
                buses[i].V_angle = new_ang * 0.1 + buses[i].V_angle * 0.9;
            }
        }
        sys.max_mismatch = max_mm;
        sys.iterations = iter + 1;
        if (max_mm < tol) { sys.converged = 1; break; }
    }
    sys.lf_computed = 1;
}

void sys_compute_losses(void) {
    sys.total_loss_P = 0.0; sys.total_loss_Q = 0.0;
    sys.total_gen_P = 0.0; sys.total_load_P = 0.0;
    for (int g = 0; g < sys.gen_count; g++) {
        if (!generators[g].active) continue;
        sys.total_gen_P += generators[g].P_gen;
    }
    for (int l = 0; l < sys.load_count; l++) {
        if (!loads[l].active) continue;
        sys.total_load_P += loads[l].P;
    }
    for (int k = 0; k < sys.line_count; k++) {
        if (!lines[k].active) continue;
        int i = lines[k].from, j = lines[k].to;
        double dV = buses[i].V_mag - buses[j].V_mag;
        double dAng = buses[i].V_angle - buses[j].V_angle;
        double Z2 = lines[k].R * lines[k].R + lines[k].X * lines[k].X;
        double P_loss = (dV * dV + dAng * dAng) * lines[k].R / (Z2 + 1e-10);
        double Q_loss = (dV * dV + dAng * dAng) * lines[k].X / (Z2 + 1e-10);
        lines[k].P_flow = P_loss;
        lines[k].Q_flow = Q_loss;
        lines[k].I_mag = dsqrt(P_loss * P_loss + Q_loss * Q_loss) / (buses[i].V_mag + 1e-10);
        sys.total_loss_P += P_loss;
        sys.total_loss_Q += Q_loss;
    }
    sys.total_cost = 0.0;
    for (int g = 0; g < sys.gen_count; g++) {
        if (!generators[g].active) continue;
        double P = generators[g].P_gen;
        sys.total_cost += generators[g].cost_a * P * P + generators[g].cost_b * P + generators[g].cost_c;
    }
}

void sys_fault_analysis(int fault_bus, int fault_type) {
    fault.fault_bus = fault_bus;
    fault.fault_type = fault_type;
    double Zf = 0.01;
    if (fault_type == FAULT_3PH) {
        fault.Z_pos[0] = 0.1; fault.Z_pos[1] = 0.9;
        fault.I_fault = buses[fault_bus].V_mag / (dsqrt(fault.Z_pos[0]*fault.Z_pos[0] + fault.Z_pos[1]*fault.Z_pos[1]) + Zf);
    } else if (fault_type == FAULT_SLG) {
        fault.Z_pos[0] = 0.1; fault.Z_pos[1] = 0.9;
        fault.Z_neg[0] = 0.1; fault.Z_neg[1] = 0.8;
        fault.Z_zero[0] = 0.15; fault.Z_zero[1] = 1.2;
        double Zt_r = fault.Z_pos[0] + fault.Z_neg[0] + fault.Z_zero[0] + Zf;
        double Zt_i = fault.Z_pos[1] + fault.Z_neg[1] + fault.Z_zero[1];
        fault.I_fault = 3.0 * buses[fault_bus].V_mag / (dsqrt(Zt_r*Zt_r + Zt_i*Zt_i) + 1e-10);
    } else if (fault_type == FAULT_LL) {
        fault.Z_pos[0] = 0.1; fault.Z_pos[1] = 0.9;
        fault.Z_neg[0] = 0.1; fault.Z_neg[1] = 0.8;
        double Zt_r = fault.Z_pos[0] + fault.Z_neg[0] + Zf;
        double Zt_i = fault.Z_pos[1] + fault.Z_neg[1];
        fault.I_fault = dsqrt(3.0) * buses[fault_bus].V_mag / (dsqrt(Zt_r*Zt_r + Zt_i*Zt_i) + 1e-10);
    } else {
        fault.I_fault = buses[fault_bus].V_mag * 1.5;
    }
    for (int i = 0; i < sys.bus_count; i++) {
        double dist = dabs(i - fault_bus);
        fault.V_post[i] = buses[i].V_mag * (1.0 - 0.5 / (dist + 1.0));
    }
    sys.fault_computed = 1;
    print_str("[SYS] Fault analysis bus="); print_int(fault_bus);
    print_str(" type="); print_int(fault_type); print_str("\n");
    print_str("  Ifault="); print_int((int)(fault.I_fault * 1000)); print_str("A");
    print_str(" ("); print_int((int)(fault.I_fault * 1000 / sys.base_MVA)); print_str(" pu)\n");
}

void sys_print_bus_results(void) {
    print_str("[SYS] Bus results:\n");
    for (int i = 0; i < sys.bus_count; i++) {
        print_str("  Bus "); print_int(i);
        print_str(": V="); print_int((int)(buses[i].V_mag * 1000));
        print_str("mV ang="); print_int((int)(buses[i].V_angle * 573));
        print_str(" Pnet="); print_int((int)(buses[i].P_net * 100));
        print_str(" type="); print_int(buses[i].type); print_str("\n");
    }
}

void sys_print_line_results(void) {
    print_str("[SYS] Line flows:\n");
    for (int k = 0; k < sys.line_count; k++) {
        if (!lines[k].active) continue;
        print_str("  "); print_int(lines[k].from);
        print_str("->"); print_int(lines[k].to);
        print_str(": P="); print_int((int)(lines[k].P_flow * 100));
        print_str(" Q="); print_int((int)(lines[k].Q_flow * 100));
        print_str(" I="); print_int((int)(lines[k].I_mag * 1000)); print_str("A\n");
    }
}

void sys_print_state(void) {
    print_str("[SYS] Buses="); print_int(sys.bus_count);
    print_str(" Lines="); print_int(sys.line_count);
    print_str(" Gens="); print_int(sys.gen_count);
    print_str(" Loads="); print_int(sys.load_count);
    print_str(" Loss="); print_int((int)(sys.total_loss_P * 100));
    print_str("MW Cost="); print_int((int)sys.total_cost);
    print_str(" iter="); print_int(sys.iterations);
    print_str(" conv="); print_int(sys.converged);
    print_str("\n");
}

int main(void) {
    print_str("=== Power System Analysis Demo ===\n\n");
    sys_init(100.0, SOLV_NR);

    print_str("Building IEEE 14-bus system...\n");
    sys_add_bus(BUS_SLACK, 1.06);
    sys_add_bus(BUS_PQ, 1.04);
    sys_add_bus(BUS_PQ, 1.01);
    sys_add_bus(BUS_PQ, 1.00);
    sys_add_bus(BUS_PQ, 1.00);
    sys_add_bus(BUS_PV, 1.05);
    sys_add_bus(BUS_PQ, 1.02);

    sys_add_line(0, 1, 0.02, 0.06, 0.06, 100.0, LINE_OHL);
    sys_add_line(0, 4, 0.05, 0.20, 0.04, 80.0, LINE_OHL);
    sys_add_line(1, 2, 0.08, 0.24, 0.04, 60.0, LINE_OHL);
    sys_add_line(1, 3, 0.06, 0.18, 0.04, 70.0, LINE_OHL);
    sys_add_line(1, 4, 0.06, 0.20, 0.04, 60.0, LINE_OHL);
    sys_add_line(2, 3, 0.10, 0.30, 0.02, 40.0, LINE_OHL);
    sys_add_line(3, 4, 0.04, 0.12, 0.04, 80.0, LINE_OHL);
    sys_add_line(4, 5, 0.06, 0.20, 0.04, 60.0, LINE_OHL);
    sys_add_line(5, 6, 0.08, 0.24, 0.04, 50.0, LINE_OHL);

    sys_add_generator(0, 100.0, 50.0, 1.06, 10.0, 20.0, 100.0);
    sys_add_generator(5, 60.0, 30.0, 1.05, 15.0, 25.0, 150.0);

    sys_add_load(1, 20.0, 10.0, 1);
    sys_add_load(2, 40.0, 15.0, 2);
    sys_add_load(3, 25.0, 12.0, 1);
    sys_add_load(4, 30.0, 15.0, 2);
    sys_add_load(5, 15.0, 8.0, 1);
    sys_add_load(6, 20.0, 10.0, 2);

    print_str("\nNewton-Raphson load flow...\n");
    sys_newton_raphson(50, 0.001);
    sys_compute_losses();
    sys_print_bus_results();
    sys_print_line_results();

    print_str("\nGauss-Seidel load flow...\n");
    sys.converged = 0;
    sys.iterations = 0;
    sys_gauss_seidel(100, 0.001);
    sys_compute_losses();
    sys_print_bus_results();

    print_str("\n3-phase fault on bus 3...\n");
    sys_fault_analysis(3, FAULT_3PH);

    print_str("\nSingle-line-to-ground fault on bus 5...\n");
    sys_fault_analysis(5, FAULT_SLG);

    print_str("\nLine-to-line fault on bus 2...\n");
    sys_fault_analysis(2, FAULT_LL);

    sys_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
