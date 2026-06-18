/* hydraulic_admin: Hydraulic administration system (v1.0)
 * Hydraulic pumps, motors, cylinders, valves, systems
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

#define MAX_PUMP       16
#define MAX_MOTOR      14
#define MAX_CYLINDER   12
#define MAX_VALVE      10
#define MAX_SYSTEM     10

typedef struct {
    int    pump_id;
    int    pump_type;
    int    pump_category;
    int    gear_pump;
    int    vane_pump;
    int    piston_pump;
    int    year;
    int    active;
} pump_t;

typedef struct {
    int    motor_id;
    int    motor_type;
    int    motor_category;
    int    gear_motor;
    int    vane_motor;
    int    piston_motor;
    int    year;
    int    active;
} motor_t;

typedef struct {
    int    cyl_id;
    int    cyl_type;
    int    cyl_category;
    int    single_act;
    int    double_act;
    int    telescopic;
    int    year;
    int    active;
} cylinder_t;

typedef struct {
    int    vlv_id;
    int    vlv_type;
    int    vlv_category;
    int    direction_v;
    int    pressure_v;
    int    flow_v;
    int    year;
    int    active;
} valve_t;

typedef struct {
    int    sys_id;
    int    sys_type;
    int    sys_category;
    int    open_sys;
    int    closed_sys;
    int    servo_sys;
    int    year;
    int    active;
} system_t;

typedef struct {
    int    n_pump;
    int    n_motor;
    int    n_cylinder;
    int    n_valve;
    int    n_system;
    int    total_gear_pump;
    int    total_gear_motor;
    int    total_single_act;
    int    total_direction;
    int    total_open;
} hya_state_t;

static pump_t pumps[MAX_PUMP];
static motor_t motors[MAX_MOTOR];
static cylinder_t cylinders[MAX_CYLINDER];
static valve_t valves[MAX_VALVE];
static system_t systems[MAX_SYSTEM];
static hya_state_t hya;

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

int hya_init(void) {
    if (initialized) return -1;
    hya.n_pump = 0; hya.n_motor = 0; hya.n_cylinder = 0;
    hya.n_valve = 0; hya.n_system = 0;
    hya.total_gear_pump = 0; hya.total_gear_motor = 0;
    hya.total_single_act = 0; hya.total_direction = 0;
    hya.total_open = 0;
    for (int i = 0; i < MAX_PUMP; i++) pumps[i].active = 0;
    for (int i = 0; i < MAX_MOTOR; i++) motors[i].active = 0;
    for (int i = 0; i < MAX_CYLINDER; i++) cylinders[i].active = 0;
    for (int i = 0; i < MAX_VALVE; i++) valves[i].active = 0;
    for (int i = 0; i < MAX_SYSTEM; i++) systems[i].active = 0;
    initialized = 1;
    print_str("[HYA] Hydraulic initialized\n");
    return 0;
}

int hya_pump(int pm_type, int cat, int gear, int vane, int piston, int year) {
    if (hya.n_pump >= MAX_PUMP) return -1;
    pump_t* p = &pumps[hya.n_pump];
    p->pump_id = hya.n_pump;
    p->pump_type = pm_type;
    p->pump_category = cat;
    p->gear_pump = gear;
    p->vane_pump = vane;
    p->piston_pump = piston;
    p->year = year;
    p->active = 1;
    hya.total_gear_pump += gear;
    hya.n_pump++;
    print_str("[HYA] Pump "); print_int(hya.n_pump - 1);
    print_str(" type="); print_int(pm_type);
    print_str(" cat="); print_int(cat);
    print_str(" gr="); print_int(gear);
    print_str(" vn="); print_int(vane);
    print_str(" ps="); print_int(piston); print_str("\n");
    return hya.n_pump - 1;
}

int hya_motor(int mt_type, int cat, int gear_m, int vane_m, int piston_m, int year) {
    if (hya.n_motor >= MAX_MOTOR) return -1;
    motor_t* m = &motors[hya.n_motor];
    m->motor_id = hya.n_motor;
    m->motor_type = mt_type;
    m->motor_category = cat;
    m->gear_motor = gear_m;
    m->vane_motor = vane_m;
    m->piston_motor = piston_m;
    m->year = year;
    m->active = 1;
    hya.total_gear_motor += gear_m;
    hya.n_motor++;
    print_str("[HYA] Motor "); print_int(hya.n_motor - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" gr="); print_int(gear_m);
    print_str(" vn="); print_int(vane_m);
    print_str(" ps="); print_int(piston_m); print_str("\n");
    return hya.n_motor - 1;
}

int hya_cylinder(int cy_type, int cat, int single_a, int double_a, int teles, int year) {
    if (hya.n_cylinder >= MAX_CYLINDER) return -1;
    cylinder_t* c = &cylinders[hya.n_cylinder];
    c->cyl_id = hya.n_cylinder;
    c->cyl_type = cy_type;
    c->cyl_category = cat;
    c->single_act = single_a;
    c->double_act = double_a;
    c->telescopic = teles;
    c->year = year;
    c->active = 1;
    hya.total_single_act += single_a;
    hya.n_cylinder++;
    print_str("[HYA] Cylinder "); print_int(hya.n_cylinder - 1);
    print_str(" type="); print_int(cy_type);
    print_str(" cat="); print_int(cat);
    print_str(" sgl="); print_int(single_a);
    print_str(" dbl="); print_int(double_a);
    print_str(" tls="); print_int(teles); print_str("\n");
    return hya.n_cylinder - 1;
}

int hya_valve(int vl_type, int cat, int dir_v, int prs_v, int flw_v, int year) {
    if (hya.n_valve >= MAX_VALVE) return -1;
    valve_t* v = &valves[hya.n_valve];
    v->vlv_id = hya.n_valve;
    v->vlv_type = vl_type;
    v->vlv_category = cat;
    v->direction_v = dir_v;
    v->pressure_v = prs_v;
    v->flow_v = flw_v;
    v->year = year;
    v->active = 1;
    hya.total_direction += dir_v;
    hya.n_valve++;
    print_str("[HYA] Valve "); print_int(hya.n_valve - 1);
    print_str(" type="); print_int(vl_type);
    print_str(" cat="); print_int(cat);
    print_str(" dir="); print_int(dir_v);
    print_str(" prs="); print_int(prs_v);
    print_str(" flw="); print_int(flw_v); print_str("\n");
    return hya.n_valve - 1;
}

int hya_system(int sy_type, int cat, int open_s, int closed_s, int servo_s, int year) {
    if (hya.n_system >= MAX_SYSTEM) return -1;
    system_t* s = &systems[hya.n_system];
    s->sys_id = hya.n_system;
    s->sys_type = sy_type;
    s->sys_category = cat;
    s->open_sys = open_s;
    s->closed_sys = closed_s;
    s->servo_sys = servo_s;
    s->year = year;
    s->active = 1;
    hya.total_open += open_s;
    hya.n_system++;
    print_str("[HYA] System "); print_int(hya.n_system - 1);
    print_str(" type="); print_int(sy_type);
    print_str(" cat="); print_int(cat);
    print_str(" opn="); print_int(open_s);
    print_str(" cls="); print_int(closed_s);
    print_str(" srv="); print_int(servo_s); print_str("\n");
    return hya.n_system - 1;
}

void hya_pump_report(void) {
    print_str("[HYA] Pump report:\n");
    print_str("  Hydraulic pump categories: "); print_int(hya.n_pump); print_str("\n");
    print_str("  Total gear pumps: "); print_int(hya.total_gear_pump); print_str("\n");
}

void hya_motor_report(void) {
    print_str("[HYA] Motor report:\n");
    print_str("  Hydraulic motor categories: "); print_int(hya.n_motor); print_str("\n");
    print_str("  Total gear motors: "); print_int(hya.total_gear_motor); print_str("\n");
}

void hya_system_report(void) {
    print_str("[HYA] System report:\n");
    print_str("  Hydraulic cylinder categories: "); print_int(hya.n_cylinder); print_str("\n");
    print_str("  Total single-acting: "); print_int(hya.total_single_act); print_str("\n");
    print_str("  Hydraulic valve categories: "); print_int(hya.n_valve); print_str("\n");
    print_str("  Total direction valves: "); print_int(hya.total_direction); print_str("\n");
    print_str("  Hydraulic system categories: "); print_int(hya.n_system); print_str("\n");
    print_str("  Total open systems: "); print_int(hya.total_open); print_str("\n");
}

void hya_print_state(void) {
    print_str("[HYA] Pm="); print_int(hya.n_pump);
    print_str(" Mt="); print_int(hya.n_motor);
    print_str(" Cy="); print_int(hya.n_cylinder);
    print_str(" Vl="); print_int(hya.n_valve);
    print_str(" Sy="); print_int(hya.n_system);
    print_str("\n");
}

int main(void) {
    print_str("=== Hydraulic Admin Demo ===\n\n");
    hya_init();

    print_str("Hydraulic pumps...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int gr = 58 + (i * 14);
        int vn = 40 + (i * 10);
        int ps = 25 + (i * 6);
        int year = 2020 + (i % 5);
        hya_pump(type, cat, gr, vn, ps, year);
    }

    print_str("\nHydraulic motors...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int gr = 50 + (i * 12);
        int vn = 35 + (i * 8);
        int ps = 22 + (i * 5);
        int year = 2021 + (i % 4);
        hya_motor(type, cat, gr, vn, ps, year);
    }

    print_str("\nHydraulic cylinders...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int sgl = 45 + (i * 11);
        int dbl = 30 + (i * 7);
        int tls = 18 + (i * 4);
        int year = 2022 + (i % 3);
        hya_cylinder(type, cat, sgl, dbl, tls, year);
    }

    print_str("\nHydraulic valves...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int dir = 38 + (i * 9);
        int prs = 25 + (i * 6);
        int flw = 15 + (i * 3);
        int year = 2023 + (i % 2);
        hya_valve(type, cat, dir, prs, flw, year);
    }

    print_str("\nHydraulic systems...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int opn = 32 + (i * 8);
        int cls = 22 + (i * 5);
        int srv = 12 + (i * 3);
        int year = 2024;
        hya_system(type, cat, opn, cls, srv, year);
    }

    print_str("\nPump report...\n");
    hya_pump_report();

    print_str("\nMotor report...\n");
    hya_motor_report();

    print_str("\nSystem report...\n");
    hya_system_report();

    print_str("\nFinal state...\n");
    hya_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
