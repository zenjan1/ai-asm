/* pneumatic_admin: Pneumatic administration system (v1.0)
 * Air supply, actuators, control valves, auxiliaries, pneumatic systems
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

#define MAX_AIR_SUPPLY   16
#define MAX_ACTUATOR     14
#define MAX_CTRL_VALVE   12
#define MAX_AUX          10
#define MAX_PNEU_SYS     10

typedef struct {
    int    air_id;
    int    air_type;
    int    air_category;
    int    compressor;
    int    air_tank;
    int    dryer;
    int    year;
    int    active;
} air_supply_t;

typedef struct {
    int    act_id;
    int    act_type;
    int    act_category;
    int    cylinder;
    int    air_motor;
    int    vacuum_cup;
    int    year;
    int    active;
} actuator_t;

typedef struct {
    int    cv_id;
    int    cv_type;
    int    cv_category;
    int    dir_ctrl;
    int    press_ctrl;
    int    flow_ctrl;
    int    year;
    int    active;
} ctrl_valve_t;

typedef struct {
    int    aux_id;
    int    aux_type;
    int    aux_category;
    int    filter;
    int    oil_mist;
    int    silencer;
    int    year;
    int    active;
} aux_t;

typedef struct {
    int    ps_id;
    int    ps_type;
    int    ps_category;
    int    vacuum_sys;
    int    pneu_trans;
    int    pneu_logic;
    int    year;
    int    active;
} pneu_sys_t;

typedef struct {
    int    n_air;
    int    n_actuator;
    int    n_ctrl_valve;
    int    n_aux;
    int    n_pneu_sys;
    int    total_compressor;
    int    total_cylinder;
    int    total_dir_ctrl;
    int    total_filter;
    int    total_vacuum;
} pna_state_t;

static air_supply_t air_supplies[MAX_AIR_SUPPLY];
static actuator_t actuators[MAX_ACTUATOR];
static ctrl_valve_t ctrl_valves[MAX_CTRL_VALVE];
static aux_t auxs[MAX_AUX];
static pneu_sys_t pneu_systems[MAX_PNEU_SYS];
static pna_state_t pna;

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

int pna_init(void) {
    if (initialized) return -1;
    pna.n_air = 0; pna.n_actuator = 0; pna.n_ctrl_valve = 0;
    pna.n_aux = 0; pna.n_pneu_sys = 0;
    pna.total_compressor = 0; pna.total_cylinder = 0;
    pna.total_dir_ctrl = 0; pna.total_filter = 0;
    pna.total_vacuum = 0;
    for (int i = 0; i < MAX_AIR_SUPPLY; i++) air_supplies[i].active = 0;
    for (int i = 0; i < MAX_ACTUATOR; i++) actuators[i].active = 0;
    for (int i = 0; i < MAX_CTRL_VALVE; i++) ctrl_valves[i].active = 0;
    for (int i = 0; i < MAX_AUX; i++) auxs[i].active = 0;
    for (int i = 0; i < MAX_PNEU_SYS; i++) pneu_systems[i].active = 0;
    initialized = 1;
    print_str("[PNA] Pneumatic initialized\n");
    return 0;
}

int pna_air(int ar_type, int cat, int comp, int tank, int dry, int year) {
    if (pna.n_air >= MAX_AIR_SUPPLY) return -1;
    air_supply_t* a = &air_supplies[pna.n_air];
    a->air_id = pna.n_air;
    a->air_type = ar_type;
    a->air_category = cat;
    a->compressor = comp;
    a->air_tank = tank;
    a->dryer = dry;
    a->year = year;
    a->active = 1;
    pna.total_compressor += comp;
    pna.n_air++;
    print_str("[PNA] Air supply "); print_int(pna.n_air - 1);
    print_str(" type="); print_int(ar_type);
    print_str(" cat="); print_int(cat);
    print_str(" cmp="); print_int(comp);
    print_str(" tnk="); print_int(tank);
    print_str(" dry="); print_int(dry); print_str("\n");
    return pna.n_air - 1;
}

int pna_actuator(int ac_type, int cat, int cyl, int motor_a, int vac_cup, int year) {
    if (pna.n_actuator >= MAX_ACTUATOR) return -1;
    actuator_t* a = &actuators[pna.n_actuator];
    a->act_id = pna.n_actuator;
    a->act_type = ac_type;
    a->act_category = cat;
    a->cylinder = cyl;
    a->air_motor = motor_a;
    a->vacuum_cup = vac_cup;
    a->year = year;
    a->active = 1;
    pna.total_cylinder += cyl;
    pna.n_actuator++;
    print_str("[PNA] Actuator "); print_int(pna.n_actuator - 1);
    print_str(" type="); print_int(ac_type);
    print_str(" cat="); print_int(cat);
    print_str(" cyl="); print_int(cyl);
    print_str(" mtr="); print_int(motor_a);
    print_str(" vcp="); print_int(vac_cup); print_str("\n");
    return pna.n_actuator - 1;
}

int pna_ctrl_valve(int cv_type, int cat, int dir_c, int prs_c, int flw_c, int year) {
    if (pna.n_ctrl_valve >= MAX_CTRL_VALVE) return -1;
    ctrl_valve_t* v = &ctrl_valves[pna.n_ctrl_valve];
    v->cv_id = pna.n_ctrl_valve;
    v->cv_type = cv_type;
    v->cv_category = cat;
    v->dir_ctrl = dir_c;
    v->press_ctrl = prs_c;
    v->flow_ctrl = flw_c;
    v->year = year;
    v->active = 1;
    pna.total_dir_ctrl += dir_c;
    pna.n_ctrl_valve++;
    print_str("[PNA] Ctrl valve "); print_int(pna.n_ctrl_valve - 1);
    print_str(" type="); print_int(cv_type);
    print_str(" cat="); print_int(cat);
    print_str(" dir="); print_int(dir_c);
    print_str(" prs="); print_int(prs_c);
    print_str(" flw="); print_int(flw_c); print_str("\n");
    return pna.n_ctrl_valve - 1;
}

int pna_aux(int ax_type, int cat, int flt, int oil_m, int sil, int year) {
    if (pna.n_aux >= MAX_AUX) return -1;
    aux_t* a = &auxs[pna.n_aux];
    a->aux_id = pna.n_aux;
    a->aux_type = ax_type;
    a->aux_category = cat;
    a->filter = flt;
    a->oil_mist = oil_m;
    a->silencer = sil;
    a->year = year;
    a->active = 1;
    pna.total_filter += flt;
    pna.n_aux++;
    print_str("[PNA] Aux "); print_int(pna.n_aux - 1);
    print_str(" type="); print_int(ax_type);
    print_str(" cat="); print_int(cat);
    print_str(" flt="); print_int(flt);
    print_str(" olm="); print_int(oil_m);
    print_str(" sln="); print_int(sil); print_str("\n");
    return pna.n_aux - 1;
}

int pna_pneu_sys(int ps_type, int cat, int vac_s, int pneu_t, int pneu_l, int year) {
    if (pna.n_pneu_sys >= MAX_PNEU_SYS) return -1;
    pneu_sys_t* p = &pneu_systems[pna.n_pneu_sys];
    p->ps_id = pna.n_pneu_sys;
    p->ps_type = ps_type;
    p->ps_category = cat;
    p->vacuum_sys = vac_s;
    p->pneu_trans = pneu_t;
    p->pneu_logic = pneu_l;
    p->year = year;
    p->active = 1;
    pna.total_vacuum += vac_s;
    pna.n_pneu_sys++;
    print_str("[PNA] Pneumatic sys "); print_int(pna.n_pneu_sys - 1);
    print_str(" type="); print_int(ps_type);
    print_str(" cat="); print_int(cat);
    print_str(" vcs="); print_int(vac_s);
    print_str(" ptr="); print_int(pneu_t);
    print_str(" ptl="); print_int(pneu_l); print_str("\n");
    return pna.n_pneu_sys - 1;
}

void pna_air_report(void) {
    print_str("[PNA] Air report:\n");
    print_str("  Air supply categories: "); print_int(pna.n_air); print_str("\n");
    print_str("  Total compressors: "); print_int(pna.total_compressor); print_str("\n");
}

void pna_actuator_report(void) {
    print_str("[PNA] Actuator report:\n");
    print_str("  Pneumatic actuator categories: "); print_int(pna.n_actuator); print_str("\n");
    print_str("  Total cylinders: "); print_int(pna.total_cylinder); print_str("\n");
}

void pna_pneu_sys_report(void) {
    print_str("[PNA] Pneumatic system report:\n");
    print_str("  Control valve categories: "); print_int(pna.n_ctrl_valve); print_str("\n");
    print_str("  Total direction control: "); print_int(pna.total_dir_ctrl); print_str("\n");
    print_str("  Auxiliary categories: "); print_int(pna.n_aux); print_str("\n");
    print_str("  Total filters: "); print_int(pna.total_filter); print_str("\n");
    print_str("  Pneumatic system categories: "); print_int(pna.n_pneu_sys); print_str("\n");
    print_str("  Total vacuum systems: "); print_int(pna.total_vacuum); print_str("\n");
}

void pna_print_state(void) {
    print_str("[PNA] Ar="); print_int(pna.n_air);
    print_str(" Ac="); print_int(pna.n_actuator);
    print_str(" Cv="); print_int(pna.n_ctrl_valve);
    print_str(" Ax="); print_int(pna.n_aux);
    print_str(" Ps="); print_int(pna.n_pneu_sys);
    print_str("\n");
}

int main(void) {
    print_str("=== Pneumatic Admin Demo ===\n\n");
    pna_init();

    print_str("Air supply...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int cmp = 55 + (i * 13);
        int tnk = 40 + (i * 10);
        int dry = 22 + (i * 5);
        int year = 2020 + (i % 5);
        pna_air(type, cat, cmp, tnk, dry, year);
    }

    print_str("\nActuators...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cyl = 48 + (i * 12);
        int mtr = 35 + (i * 8);
        int vcp = 20 + (i * 5);
        int year = 2021 + (i % 4);
        pna_actuator(type, cat, cyl, mtr, vcp, year);
    }

    print_str("\nControl valves...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int dir = 42 + (i * 10);
        int prs = 28 + (i * 7);
        int flw = 18 + (i * 4);
        int year = 2022 + (i % 3);
        pna_ctrl_valve(type, cat, dir, prs, flw, year);
    }

    print_str("\nAuxiliaries...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int flt = 35 + (i * 8);
        int olm = 25 + (i * 6);
        int sln = 15 + (i * 3);
        int year = 2023 + (i % 2);
        pna_aux(type, cat, flt, olm, sln, year);
    }

    print_str("\nPneumatic systems...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int vcs = 30 + (i * 7);
        int ptr = 22 + (i * 5);
        int ptl = 12 + (i * 3);
        int year = 2024;
        pna_pneu_sys(type, cat, vcs, ptr, ptl, year);
    }

    print_str("\nAir report...\n");
    pna_air_report();

    print_str("\nActuator report...\n");
    pna_actuator_report();

    print_str("\nPneumatic system report...\n");
    pna_pneu_sys_report();

    print_str("\nFinal state...\n");
    pna_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
