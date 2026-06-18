/* quantum_computing_admin: Quantum computing administration system (v1.0)
 * Quantum hardware, quantum software, quantum communication, quantum applications, quantum measurement
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

#define MAX_HARDWARE     16
#define MAX_SOFTWARE     14
#define MAX_COMM         12
#define MAX_APPLICATION  10
#define MAX_MEASURE      10

typedef struct {
    int    hw_id;
    int    hw_type;
    int    hw_category;
    int    superconducting;
    int    ion_trap;
    int    photonic;
    int    year;
    int    active;
} hw_t;

typedef struct {
    int    sw_id;
    int    sw_type;
    int    sw_category;
    int    algorithm;
    int    programming;
    int    simulator;
    int    year;
    int    active;
} sw_t;

typedef struct {
    int    comm_id;
    int    comm_type;
    int    comm_category;
    int    qkd;
    int    teleportation;
    int    quantum_net;
    int    year;
    int    active;
} comm_t;

typedef struct {
    int    app_id;
    int    app_type;
    int    app_category;
    int    quantum_chem;
    int    quantum_opt;
    int    quantum_ml;
    int    year;
    int    active;
} app_t;

typedef struct {
    int    measure_id;
    int    measure_type;
    int    measure_category;
    int    error_correct;
    int    state_prepare;
    int    measurement;
    int    year;
    int    active;
} measure_t;

typedef struct {
    int    n_hw;
    int    n_sw;
    int    n_comm;
    int    n_app;
    int    n_measure;
    int    total_superconducting;
    int    total_algorithm;
    int    total_qkd;
    int    total_chem;
    int    total_error;
} qca_state_t;

static hw_t hardwares[MAX_HARDWARE];
static sw_t softwares[MAX_SOFTWARE];
static comm_t comms[MAX_COMM];
static app_t apps[MAX_APPLICATION];
static measure_t measures[MAX_MEASURE];
static qca_state_t qca;

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

int qca_init(void) {
    if (initialized) return -1;
    qca.n_hw = 0; qca.n_sw = 0; qca.n_comm = 0;
    qca.n_app = 0; qca.n_measure = 0;
    qca.total_superconducting = 0; qca.total_algorithm = 0;
    qca.total_qkd = 0; qca.total_chem = 0;
    qca.total_error = 0;
    for (int i = 0; i < MAX_HARDWARE; i++) hardwares[i].active = 0;
    for (int i = 0; i < MAX_SOFTWARE; i++) softwares[i].active = 0;
    for (int i = 0; i < MAX_COMM; i++) comms[i].active = 0;
    for (int i = 0; i < MAX_APPLICATION; i++) apps[i].active = 0;
    for (int i = 0; i < MAX_MEASURE; i++) measures[i].active = 0;
    initialized = 1;
    print_str("[QCA] Quantum computing initialized\n");
    return 0;
}

int qca_hardware(int hw_type, int cat, int super_c, int ion, int photo, int year) {
    if (qca.n_hw >= MAX_HARDWARE) return -1;
    hw_t* h = &hardwares[qca.n_hw];
    h->hw_id = qca.n_hw;
    h->hw_type = hw_type;
    h->hw_category = cat;
    h->superconducting = super_c;
    h->ion_trap = ion;
    h->photonic = photo;
    h->year = year;
    h->active = 1;
    qca.total_superconducting += super_c;
    qca.n_hw++;
    print_str("[QCA] Hardware "); print_int(qca.n_hw - 1);
    print_str(" type="); print_int(hw_type);
    print_str(" cat="); print_int(cat);
    print_str(" sup="); print_int(super_c);
    print_str(" ion="); print_int(ion);
    print_str(" pho="); print_int(photo); print_str("\n");
    return qca.n_hw - 1;
}

int qca_software(int sw_type, int cat, int algo, int prog, int sim, int year) {
    if (qca.n_sw >= MAX_SOFTWARE) return -1;
    sw_t* s = &softwares[qca.n_sw];
    s->sw_id = qca.n_sw;
    s->sw_type = sw_type;
    s->sw_category = cat;
    s->algorithm = algo;
    s->programming = prog;
    s->simulator = sim;
    s->year = year;
    s->active = 1;
    qca.total_algorithm += algo;
    qca.n_sw++;
    print_str("[QCA] Software "); print_int(qca.n_sw - 1);
    print_str(" type="); print_int(sw_type);
    print_str(" cat="); print_int(cat);
    print_str(" alg="); print_int(algo);
    print_str(" prg="); print_int(prog);
    print_str(" sim="); print_int(sim); print_str("\n");
    return qca.n_sw - 1;
}

int qca_comm(int cm_type, int cat, int qk, int tele, int net_c, int year) {
    if (qca.n_comm >= MAX_COMM) return -1;
    comm_t* c = &comms[qca.n_comm];
    c->comm_id = qca.n_comm;
    c->comm_type = cm_type;
    c->comm_category = cat;
    c->qkd = qk;
    c->teleportation = tele;
    c->quantum_net = net_c;
    c->year = year;
    c->active = 1;
    qca.total_qkd += qk;
    qca.n_comm++;
    print_str("[QCA] Comm "); print_int(qca.n_comm - 1);
    print_str(" type="); print_int(cm_type);
    print_str(" cat="); print_int(cat);
    print_str(" qkd="); print_int(qk);
    print_str(" tel="); print_int(tele);
    print_str(" net="); print_int(net_c); print_str("\n");
    return qca.n_comm - 1;
}

int qca_app(int ap_type, int cat, int chem, int opt_c, int ml_c, int year) {
    if (qca.n_app >= MAX_APPLICATION) return -1;
    app_t* a = &apps[qca.n_app];
    a->app_id = qca.n_app;
    a->app_type = ap_type;
    a->app_category = cat;
    a->quantum_chem = chem;
    a->quantum_opt = opt_c;
    a->quantum_ml = ml_c;
    a->year = year;
    a->active = 1;
    qca.total_chem += chem;
    qca.n_app++;
    print_str("[QCA] App "); print_int(qca.n_app - 1);
    print_str(" type="); print_int(ap_type);
    print_str(" cat="); print_int(cat);
    print_str(" chm="); print_int(chem);
    print_str(" opt="); print_int(opt_c);
    print_str(" mlc="); print_int(ml_c); print_str("\n");
    return qca.n_app - 1;
}

int qca_measure(int ms_type, int cat, int err_c, int state_p, int msrm, int year) {
    if (qca.n_measure >= MAX_MEASURE) return -1;
    measure_t* m = &measures[qca.n_measure];
    m->measure_id = qca.n_measure;
    m->measure_type = ms_type;
    m->measure_category = cat;
    m->error_correct = err_c;
    m->state_prepare = state_p;
    m->measurement = msrm;
    m->year = year;
    m->active = 1;
    qca.total_error += err_c;
    qca.n_measure++;
    print_str("[QCA] Measure "); print_int(qca.n_measure - 1);
    print_str(" type="); print_int(ms_type);
    print_str(" cat="); print_int(cat);
    print_str(" err="); print_int(err_c);
    print_str(" stp="); print_int(state_p);
    print_str(" msr="); print_int(msrm); print_str("\n");
    return qca.n_measure - 1;
}

void qca_hardware_report(void) {
    print_str("[QCA] Hardware report:\n");
    print_str("  Quantum hardware categories: "); print_int(qca.n_hw); print_str("\n");
    print_str("  Total superconducting qubits: "); print_int(qca.total_superconducting); print_str("\n");
}

void qca_software_report(void) {
    print_str("[QCA] Software report:\n");
    print_str("  Quantum software categories: "); print_int(qca.n_sw); print_str("\n");
    print_str("  Total quantum algorithms: "); print_int(qca.total_algorithm); print_str("\n");
}

void qca_measure_report(void) {
    print_str("[QCA] Measure report:\n");
    print_str("  Quantum comm categories: "); print_int(qca.n_comm); print_str("\n");
    print_str("  Total QKD: "); print_int(qca.total_qkd); print_str("\n");
    print_str("  Quantum app categories: "); print_int(qca.n_app); print_str("\n");
    print_str("  Total quantum chem: "); print_int(qca.total_chem); print_str("\n");
    print_str("  Measurement categories: "); print_int(qca.n_measure); print_str("\n");
    print_str("  Total error correction: "); print_int(qca.total_error); print_str("\n");
}

void qca_print_state(void) {
    print_str("[QCA] Hw="); print_int(qca.n_hw);
    print_str(" Sw="); print_int(qca.n_sw);
    print_str(" Cm="); print_int(qca.n_comm);
    print_str(" Ap="); print_int(qca.n_app);
    print_str(" Ms="); print_int(qca.n_measure);
    print_str("\n");
}

int main(void) {
    print_str("=== Quantum Computing Admin Demo ===\n\n");
    qca_init();

    print_str("Quantum hardware...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int sup = 50 + (i * 12);
        int ion = 35 + (i * 8);
        int pho = 22 + (i * 5);
        int year = 2020 + (i % 5);
        qca_hardware(type, cat, sup, ion, pho, year);
    }

    print_str("\nQuantum software...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int alg = 40 + (i * 10);
        int prg = 28 + (i * 7);
        int sim = 18 + (i * 4);
        int year = 2021 + (i % 4);
        qca_software(type, cat, alg, prg, sim, year);
    }

    print_str("\nQuantum communication...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int qk = 45 + (i * 11);
        int tel = 30 + (i * 7);
        int net_c = 20 + (i * 5);
        int year = 2022 + (i % 3);
        qca_comm(type, cat, qk, tel, net_c, year);
    }

    print_str("\nQuantum applications...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int chm = 32 + (i * 8);
        int opt_c = 22 + (i * 5);
        int ml_c = 15 + (i * 3);
        int year = 2023 + (i % 2);
        qca_app(type, cat, chm, opt_c, ml_c, year);
    }

    print_str("\nQuantum measurement...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int err = 28 + (i * 7);
        int stp = 20 + (i * 5);
        int msr = 12 + (i * 3);
        int year = 2024;
        qca_measure(type, cat, err, stp, msr, year);
    }

    print_str("\nHardware report...\n");
    qca_hardware_report();

    print_str("\nSoftware report...\n");
    qca_software_report();

    print_str("\nMeasure report...\n");
    qca_measure_report();

    print_str("\nFinal state...\n");
    qca_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
