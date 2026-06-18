/* electrical_engineering_admin: Electrical engineering administration (v1.0)
 * Electric machines, power electronics, high voltage, power systems, control
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

#define MAX_MACHINE     16
#define MAX_POWER_EL    14
#define MAX_HIGH_V      12
#define MAX_POWER_SYS   10
#define MAX_ELEC_CTRL   10

typedef struct {
    int    mc_id;
    int    mc_type;
    int    mc_category;
    int    transformer;
    int    async_motor;
    int    sync_motor;
    int    year;
    int    active;
} machine_t;

typedef struct {
    int    pe_id;
    int    pe_type;
    int    pe_category;
    int    rectifier;
    int    inverter;
    int    chopper;
    int    year;
    int    active;
} power_el_t;

typedef struct {
    int    hv_id;
    int    hv_type;
    int    hv_category;
    int    insul_test;
    int    overvolt_prot;
    int    grounding;
    int    year;
    int    active;
} high_v_t;

typedef struct {
    int    ps_id;
    int    ps_type;
    int    ps_category;
    int    load_flow;
    int    short_circuit;
    int    stability;
    int    year;
    int    active;
} power_sys_t;

typedef struct {
    int    ec_id;
    int    ec_type;
    int    ec_category;
    int    plc_ctrl;
    int    relay_prot;
    int    auto_dev;
    int    year;
    int    active;
} elec_ctrl_t;

typedef struct {
    int    n_machine;
    int    n_power_el;
    int    n_high_v;
    int    n_power_sys;
    int    n_elec_ctrl;
    int    total_transformer;
    int    total_rectifier;
    int    total_insul_test;
    int    total_load_flow;
    int    total_plc_ctrl;
} eea_state_t;

static machine_t machines[MAX_MACHINE];
static power_el_t power_els[MAX_POWER_EL];
static high_v_t high_vs[MAX_HIGH_V];
static power_sys_t power_syss[MAX_POWER_SYS];
static elec_ctrl_t elec_ctrls[MAX_ELEC_CTRL];
static eea_state_t eea;

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

int eea_init(void) {
    if (initialized) return -1;
    eea.n_machine = 0; eea.n_power_el = 0; eea.n_high_v = 0;
    eea.n_power_sys = 0; eea.n_elec_ctrl = 0;
    eea.total_transformer = 0; eea.total_rectifier = 0;
    eea.total_insul_test = 0; eea.total_load_flow = 0;
    eea.total_plc_ctrl = 0;
    for (int i = 0; i < MAX_MACHINE; i++) machines[i].active = 0;
    for (int i = 0; i < MAX_POWER_EL; i++) power_els[i].active = 0;
    for (int i = 0; i < MAX_HIGH_V; i++) high_vs[i].active = 0;
    for (int i = 0; i < MAX_POWER_SYS; i++) power_syss[i].active = 0;
    for (int i = 0; i < MAX_ELEC_CTRL; i++) elec_ctrls[i].active = 0;
    initialized = 1;
    print_str("[EEA] Electrical engineering initialized\n");
    return 0;
}

int eea_machine(int mt_type, int cat, int trf, int async_m, int sync_m, int year) {
    if (eea.n_machine >= MAX_MACHINE) return -1;
    machine_t* m = &machines[eea.n_machine];
    m->mc_id = eea.n_machine;
    m->mc_type = mt_type;
    m->mc_category = cat;
    m->transformer = trf;
    m->async_motor = async_m;
    m->sync_motor = sync_m;
    m->year = year;
    m->active = 1;
    eea.total_transformer += trf;
    eea.n_machine++;
    print_str("[EEA] Machine "); print_int(eea.n_machine - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" trf="); print_int(trf);
    print_str(" asy="); print_int(async_m);
    print_str(" syn="); print_int(sync_m); print_str("\n");
    return eea.n_machine - 1;
}

int eea_power_el(int pt_type, int cat, int rct, int inv, int chp, int year) {
    if (eea.n_power_el >= MAX_POWER_EL) return -1;
    power_el_t* p = &power_els[eea.n_power_el];
    p->pe_id = eea.n_power_el;
    p->pe_type = pt_type;
    p->pe_category = cat;
    p->rectifier = rct;
    p->inverter = inv;
    p->chopper = chp;
    p->year = year;
    p->active = 1;
    eea.total_rectifier += rct;
    eea.n_power_el++;
    print_str("[EEA] Power el "); print_int(eea.n_power_el - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" rct="); print_int(rct);
    print_str(" inv="); print_int(inv);
    print_str(" chp="); print_int(chp); print_str("\n");
    return eea.n_power_el - 1;
}

int eea_high_v(int ht_type, int cat, int ist, int ovp, int grd, int year) {
    if (eea.n_high_v >= MAX_HIGH_V) return -1;
    high_v_t* h = &high_vs[eea.n_high_v];
    h->hv_id = eea.n_high_v;
    h->hv_type = ht_type;
    h->hv_category = cat;
    h->insul_test = ist;
    h->overvolt_prot = ovp;
    h->grounding = grd;
    h->year = year;
    h->active = 1;
    eea.total_insul_test += ist;
    eea.n_high_v++;
    print_str("[EEA] High V "); print_int(eea.n_high_v - 1);
    print_str(" type="); print_int(ht_type);
    print_str(" cat="); print_int(cat);
    print_str(" ist="); print_int(ist);
    print_str(" ovp="); print_int(ovp);
    print_str(" grd="); print_int(grd); print_str("\n");
    return eea.n_high_v - 1;
}

int eea_power_sys(int ps_type, int cat, int ldf, int shc, int stb, int year) {
    if (eea.n_power_sys >= MAX_POWER_SYS) return -1;
    power_sys_t* p = &power_syss[eea.n_power_sys];
    p->ps_id = eea.n_power_sys;
    p->ps_type = ps_type;
    p->ps_category = cat;
    p->load_flow = ldf;
    p->short_circuit = shc;
    p->stability = stb;
    p->year = year;
    p->active = 1;
    eea.total_load_flow += ldf;
    eea.n_power_sys++;
    print_str("[EEA] Power sys "); print_int(eea.n_power_sys - 1);
    print_str(" type="); print_int(ps_type);
    print_str(" cat="); print_int(cat);
    print_str(" ldf="); print_int(ldf);
    print_str(" shc="); print_int(shc);
    print_str(" stb="); print_int(stb); print_str("\n");
    return eea.n_power_sys - 1;
}

int eea_elec_ctrl(int et_type, int cat, int plc_v, int rlp, int aud, int year) {
    if (eea.n_elec_ctrl >= MAX_ELEC_CTRL) return -1;
    elec_ctrl_t* e = &elec_ctrls[eea.n_elec_ctrl];
    e->ec_id = eea.n_elec_ctrl;
    e->ec_type = et_type;
    e->ec_category = cat;
    e->plc_ctrl = plc_v;
    e->relay_prot = rlp;
    e->auto_dev = aud;
    e->year = year;
    e->active = 1;
    eea.total_plc_ctrl += plc_v;
    eea.n_elec_ctrl++;
    print_str("[EEA] Elec ctrl "); print_int(eea.n_elec_ctrl - 1);
    print_str(" type="); print_int(et_type);
    print_str(" cat="); print_int(cat);
    print_str(" plc="); print_int(plc_v);
    print_str(" rlp="); print_int(rlp);
    print_str(" aud="); print_int(aud); print_str("\n");
    return eea.n_elec_ctrl - 1;
}

void eea_machine_report(void) {
    print_str("[EEA] Machine report:\n");
    print_str("  Electric machine categories: "); print_int(eea.n_machine); print_str("\n");
    print_str("  Total transformers: "); print_int(eea.total_transformer); print_str("\n");
}

void eea_power_el_report(void) {
    print_str("[EEA] Power electronics report:\n");
    print_str("  Power electronics categories: "); print_int(eea.n_power_el); print_str("\n");
    print_str("  Total rectifiers: "); print_int(eea.total_rectifier); print_str("\n");
}

void eea_full_report(void) {
    print_str("[EEA] Full report:\n");
    print_str("  High voltage categories: "); print_int(eea.n_high_v); print_str("\n");
    print_str("  Total insulation test: "); print_int(eea.total_insul_test); print_str("\n");
    print_str("  Power system categories: "); print_int(eea.n_power_sys); print_str("\n");
    print_str("  Total load flow: "); print_int(eea.total_load_flow); print_str("\n");
    print_str("  Electrical control categories: "); print_int(eea.n_elec_ctrl); print_str("\n");
    print_str("  Total PLC control: "); print_int(eea.total_plc_ctrl); print_str("\n");
}

void eea_print_state(void) {
    print_str("[EEA] Mc="); print_int(eea.n_machine);
    print_str(" Pe="); print_int(eea.n_power_el);
    print_str(" Hv="); print_int(eea.n_high_v);
    print_str(" Ps="); print_int(eea.n_power_sys);
    print_str(" Ec="); print_int(eea.n_elec_ctrl);
    print_str("\n");
}

int main(void) {
    print_str("=== Electrical Engineering Admin Demo ===\n\n");
    eea_init();

    print_str("Electric machines...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int trf = 55 + (i * 13);
        int async_m = 40 + (i * 10);
        int sync_m = 22 + (i * 5);
        int year = 2020 + (i % 5);
        eea_machine(type, cat, trf, async_m, sync_m, year);
    }

    print_str("\nPower electronics...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int rct = 48 + (i * 11);
        int inv = 35 + (i * 8);
        int chp = 20 + (i * 4);
        int year = 2021 + (i % 4);
        eea_power_el(type, cat, rct, inv, chp, year);
    }

    print_str("\nHigh voltage...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ist = 42 + (i * 10);
        int ovp = 28 + (i * 7);
        int grd = 18 + (i * 4);
        int year = 2022 + (i % 3);
        eea_high_v(type, cat, ist, ovp, grd, year);
    }

    print_str("\nPower systems...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ldf = 35 + (i * 8);
        int shc = 25 + (i * 6);
        int stb = 15 + (i * 3);
        int year = 2023 + (i % 2);
        eea_power_sys(type, cat, ldf, shc, stb, year);
    }

    print_str("\nElectrical control...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int plc_v = 30 + (i * 7);
        int rlp = 22 + (i * 5);
        int aud = 12 + (i * 3);
        int year = 2024;
        eea_elec_ctrl(type, cat, plc_v, rlp, aud, year);
    }

    print_str("\nMachine report...\n");
    eea_machine_report();

    print_str("\nPower electronics report...\n");
    eea_power_el_report();

    print_str("\nFull report...\n");
    eea_full_report();

    print_str("\nFinal state...\n");
    eea_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
