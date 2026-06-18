/* robotics_admin: Robotics administration system (v1.0)
 * Industrial robots, service robots, special robots, core components, integrated applications
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

#define MAX_INDUSTRIAL   16
#define MAX_SERVICE      14
#define MAX_SPECIAL      12
#define MAX_COMPONENT    10
#define MAX_INTEGRATE    10

typedef struct {
    int    industrial_id;
    int    industrial_type;
    int    industrial_category;
    int    welding;
    int    assembly;
    int    handling;
    int    year;
    int    active;
} industrial_t;

typedef struct {
    int    service_id;
    int    service_type;
    int    service_category;
    int    medical;
    int    education;
    int    household;
    int    year;
    int    active;
} service_t;

typedef struct {
    int    special_id;
    int    special_type;
    int    special_category;
    int    rescue;
    int    detection;
    int    military;
    int    year;
    int    active;
} special_t;

typedef struct {
    int    component_id;
    int    component_type;
    int    component_category;
    int    reducer;
    int    servo_motor;
    int    controller;
    int    year;
    int    active;
} component_t;

typedef struct {
    int    integrate_id;
    int    integrate_type;
    int    integrate_category;
    int    system_integrate;
    int    process_plan;
    int    maintenance;
    int    year;
    int    active;
} integrate_t;

typedef struct {
    int    n_industrial;
    int    n_service;
    int    n_special;
    int    n_component;
    int    n_integrate;
    int    total_welding;
    int    total_medical;
    int    total_rescue;
    int    total_reducer;
    int    total_system;
} rba_state_t;

static industrial_t industrials[MAX_INDUSTRIAL];
static service_t services[MAX_SERVICE];
static special_t specials[MAX_SPECIAL];
static component_t components[MAX_COMPONENT];
static integrate_t integrates[MAX_INTEGRATE];
static rba_state_t rba;

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

int rba_init(void) {
    if (initialized) return -1;
    rba.n_industrial = 0; rba.n_service = 0; rba.n_special = 0;
    rba.n_component = 0; rba.n_integrate = 0;
    rba.total_welding = 0; rba.total_medical = 0;
    rba.total_rescue = 0; rba.total_reducer = 0;
    rba.total_system = 0;
    for (int i = 0; i < MAX_INDUSTRIAL; i++) industrials[i].active = 0;
    for (int i = 0; i < MAX_SERVICE; i++) services[i].active = 0;
    for (int i = 0; i < MAX_SPECIAL; i++) specials[i].active = 0;
    for (int i = 0; i < MAX_COMPONENT; i++) components[i].active = 0;
    for (int i = 0; i < MAX_INTEGRATE; i++) integrates[i].active = 0;
    initialized = 1;
    print_str("[RBA] Robotics initialized\n");
    return 0;
}

int rba_industrial(int ind_type, int cat, int weld, int asm_cnt, int handle, int year) {
    if (rba.n_industrial >= MAX_INDUSTRIAL) return -1;
    industrial_t* ind = &industrials[rba.n_industrial];
    ind->industrial_id = rba.n_industrial;
    ind->industrial_type = ind_type;
    ind->industrial_category = cat;
    ind->welding = weld;
    ind->assembly = asm_cnt;
    ind->handling = handle;
    ind->year = year;
    ind->active = 1;
    rba.total_welding += weld;
    rba.n_industrial++;
    print_str("[RBA] Industrial "); print_int(rba.n_industrial - 1);
    print_str(" type="); print_int(ind_type);
    print_str(" cat="); print_int(cat);
    print_str(" wld="); print_int(weld);
    print_str(" asm="); print_int(asm_cnt);
    print_str(" hdl="); print_int(handle); print_str("\n");
    return rba.n_industrial - 1;
}

int rba_service(int svc_type, int cat, int med, int edu, int house, int year) {
    if (rba.n_service >= MAX_SERVICE) return -1;
    service_t* s = &services[rba.n_service];
    s->service_id = rba.n_service;
    s->service_type = svc_type;
    s->service_category = cat;
    s->medical = med;
    s->education = edu;
    s->household = house;
    s->year = year;
    s->active = 1;
    rba.total_medical += med;
    rba.n_service++;
    print_str("[RBA] Service "); print_int(rba.n_service - 1);
    print_str(" type="); print_int(svc_type);
    print_str(" cat="); print_int(cat);
    print_str(" med="); print_int(med);
    print_str(" edu="); print_int(edu);
    print_str(" hsh="); print_int(house); print_str("\n");
    return rba.n_service - 1;
}

int rba_special(int spc_type, int cat, int resc, int detect, int mil, int year) {
    if (rba.n_special >= MAX_SPECIAL) return -1;
    special_t* sp = &specials[rba.n_special];
    sp->special_id = rba.n_special;
    sp->special_type = spc_type;
    sp->special_category = cat;
    sp->rescue = resc;
    sp->detection = detect;
    sp->military = mil;
    sp->year = year;
    sp->active = 1;
    rba.total_rescue += resc;
    rba.n_special++;
    print_str("[RBA] Special "); print_int(rba.n_special - 1);
    print_str(" type="); print_int(spc_type);
    print_str(" cat="); print_int(cat);
    print_str(" rsc="); print_int(resc);
    print_str(" det="); print_int(detect);
    print_str(" mil="); print_int(mil); print_str("\n");
    return rba.n_special - 1;
}

int rba_component(int cmp_type, int cat, int red, int servo, int ctrl, int year) {
    if (rba.n_component >= MAX_COMPONENT) return -1;
    component_t* c = &components[rba.n_component];
    c->component_id = rba.n_component;
    c->component_type = cmp_type;
    c->component_category = cat;
    c->reducer = red;
    c->servo_motor = servo;
    c->controller = ctrl;
    c->year = year;
    c->active = 1;
    rba.total_reducer += red;
    rba.n_component++;
    print_str("[RBA] Component "); print_int(rba.n_component - 1);
    print_str(" type="); print_int(cmp_type);
    print_str(" cat="); print_int(cat);
    print_str(" red="); print_int(red);
    print_str(" srv="); print_int(servo);
    print_str(" ctl="); print_int(ctrl); print_str("\n");
    return rba.n_component - 1;
}

int rba_integrate(int int_type, int cat, int sys_int, int process, int maint, int year) {
    if (rba.n_integrate >= MAX_INTEGRATE) return -1;
    integrate_t* it = &integrates[rba.n_integrate];
    it->integrate_id = rba.n_integrate;
    it->integrate_type = int_type;
    it->integrate_category = cat;
    it->system_integrate = sys_int;
    it->process_plan = process;
    it->maintenance = maint;
    it->year = year;
    it->active = 1;
    rba.total_system += sys_int;
    rba.n_integrate++;
    print_str("[RBA] Integrate "); print_int(rba.n_integrate - 1);
    print_str(" type="); print_int(int_type);
    print_str(" cat="); print_int(cat);
    print_str(" sys="); print_int(sys_int);
    print_str(" prc="); print_int(process);
    print_str(" mnt="); print_int(maint); print_str("\n");
    return rba.n_integrate - 1;
}

void rba_industrial_report(void) {
    print_str("[RBA] Industrial report:\n");
    print_str("  Industrial robot categories: "); print_int(rba.n_industrial); print_str("\n");
    print_str("  Total welding robots: "); print_int(rba.total_welding); print_str("\n");
}

void rba_service_report(void) {
    print_str("[RBA] Service report:\n");
    print_str("  Service robot categories: "); print_int(rba.n_service); print_str("\n");
    print_str("  Total medical robots: "); print_int(rba.total_medical); print_str("\n");
}

void rba_integrate_report(void) {
    print_str("[RBA] Integrate report:\n");
    print_str("  Special robot categories: "); print_int(rba.n_special); print_str("\n");
    print_str("  Total rescue robots: "); print_int(rba.total_rescue); print_str("\n");
    print_str("  Component categories: "); print_int(rba.n_component); print_str("\n");
    print_str("  Total reducers: "); print_int(rba.total_reducer); print_str("\n");
    print_str("  Integrate categories: "); print_int(rba.n_integrate); print_str("\n");
    print_str("  Total system integrate: "); print_int(rba.total_system); print_str("\n");
}

void rba_print_state(void) {
    print_str("[RBA] In="); print_int(rba.n_industrial);
    print_str(" Sv="); print_int(rba.n_service);
    print_str(" Sp="); print_int(rba.n_special);
    print_str(" Cp="); print_int(rba.n_component);
    print_str(" It="); print_int(rba.n_integrate);
    print_str("\n");
}

int main(void) {
    print_str("=== Robotics Admin Demo ===\n\n");
    rba_init();

    print_str("Industrial robots...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int wld = 35 + (i * 9);
        int asm_c = 25 + (i * 6);
        int hdl = 18 + (i * 4);
        int year = 2020 + (i % 5);
        rba_industrial(type, cat, wld, asm_c, hdl, year);
    }

    print_str("\nService robots...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int med = 40 + (i * 10);
        int edu = 30 + (i * 7);
        int hsh = 20 + (i * 5);
        int year = 2021 + (i % 4);
        rba_service(type, cat, med, edu, hsh, year);
    }

    print_str("\nSpecial robots...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int rsc = 25 + (i * 6);
        int det = 18 + (i * 4);
        int mil = 12 + (i * 3);
        int year = 2022 + (i % 3);
        rba_special(type, cat, rsc, det, mil, year);
    }

    print_str("\nCore components...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int red = 30 + (i * 7);
        int srv = 22 + (i * 5);
        int ctl = 15 + (i * 3);
        int year = 2023 + (i % 2);
        rba_component(type, cat, red, srv, ctl, year);
    }

    print_str("\nIntegrated applications...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int sys_i = 20 + (i * 5);
        int prc = 15 + (i * 3);
        int mnt = 10 + (i * 2);
        int year = 2024;
        rba_integrate(type, cat, sys_i, prc, mnt, year);
    }

    print_str("\nIndustrial report...\n");
    rba_industrial_report();

    print_str("\nService report...\n");
    rba_service_report();

    print_str("\nIntegrate report...\n");
    rba_integrate_report();

    print_str("\nFinal state...\n");
    rba_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
