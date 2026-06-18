/* architectural_engineering_admin: Architectural engineering administration system (v1.0)
 * Building design, planning, construction, building equipment, architectural construction
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

#define MAX_BUILD_DES   16
#define MAX_PLAN_AR     14
#define MAX_CONST_AR    12
#define MAX_BUILD_EQ    10
#define MAX_ARCH_CO     10

typedef struct {
    int    bd_id;
    int    bd_type;
    int    bd_category;
    int    resident_bld;
    int    public_bld;
    int    industrial_bld;
    int    year;
    int    active;
} build_des_t;

typedef struct {
    int    pa_id;
    int    pa_type;
    int    pa_category;
    int    urban_plan;
    int    resident_plan;
    int    commercial_plan;
    int    year;
    int    active;
} plan_ar_t;

typedef struct {
    int    co_id;
    int    co_type;
    int    co_category;
    int    found_const;
    int    wall_const;
    int    floor_const;
    int    year;
    int    active;
} const_ar_t;

typedef struct {
    int    be_id;
    int    be_type;
    int    be_category;
    int    water_sys;
    int    hvac_sys;
    int    elec_light;
    int    year;
    int    active;
} build_eq_t;

typedef struct {
    int    ac_id;
    int    ac_type;
    int    ac_category;
    int    const_org;
    int    const_tech;
    int    qual_mgr;
    int    year;
    int    active;
} arch_co_t;

typedef struct {
    int    n_build_des;
    int    n_plan_ar;
    int    n_const_ar;
    int    n_build_eq;
    int    n_arch_co;
    int    total_resident_bld;
    int    total_urban_plan;
    int    total_found_const;
    int    total_water_sys;
    int    total_const_org;
} aa_state_t;

static build_des_t build_dess[MAX_BUILD_DES];
static plan_ar_t plan_ars[MAX_PLAN_AR];
static const_ar_t const_ars[MAX_CONST_AR];
static build_eq_t build_eqs[MAX_BUILD_EQ];
static arch_co_t arch_cos[MAX_ARCH_CO];
static aa_state_t aa;

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

int aa_init(void) {
    if (initialized) return -1;
    aa.n_build_des = 0; aa.n_plan_ar = 0; aa.n_const_ar = 0;
    aa.n_build_eq = 0; aa.n_arch_co = 0;
    aa.total_resident_bld = 0; aa.total_urban_plan = 0;
    aa.total_found_const = 0; aa.total_water_sys = 0;
    aa.total_const_org = 0;
    for (int i = 0; i < MAX_BUILD_DES; i++) build_dess[i].active = 0;
    for (int i = 0; i < MAX_PLAN_AR; i++) plan_ars[i].active = 0;
    for (int i = 0; i < MAX_CONST_AR; i++) const_ars[i].active = 0;
    for (int i = 0; i < MAX_BUILD_EQ; i++) build_eqs[i].active = 0;
    for (int i = 0; i < MAX_ARCH_CO; i++) arch_cos[i].active = 0;
    initialized = 1;
    print_str("[AA] Architectural engineering initialized\n");
    return 0;
}

int aa_build_des(int bt_type, int cat, int rsb, int pub, int ind, int year) {
    if (aa.n_build_des >= MAX_BUILD_DES) return -1;
    build_des_t* b = &build_dess[aa.n_build_des];
    b->bd_id = aa.n_build_des;
    b->bd_type = bt_type;
    b->bd_category = cat;
    b->resident_bld = rsb;
    b->public_bld = pub;
    b->industrial_bld = ind;
    b->year = year;
    b->active = 1;
    aa.total_resident_bld += rsb;
    aa.n_build_des++;
    print_str("[AA] Build des "); print_int(aa.n_build_des - 1);
    print_str(" type="); print_int(bt_type);
    print_str(" cat="); print_int(cat);
    print_str(" rsb="); print_int(rsb);
    print_str(" pub="); print_int(pub);
    print_str(" ind="); print_int(ind); print_str("\n");
    return aa.n_build_des - 1;
}

int aa_plan_ar(int pt_type, int cat, int urp, int rsp, int cpl, int year) {
    if (aa.n_plan_ar >= MAX_PLAN_AR) return -1;
    plan_ar_t* p = &plan_ars[aa.n_plan_ar];
    p->pa_id = aa.n_plan_ar;
    p->pa_type = pt_type;
    p->pa_category = cat;
    p->urban_plan = urp;
    p->resident_plan = rsp;
    p->commercial_plan = cpl;
    p->year = year;
    p->active = 1;
    aa.total_urban_plan += urp;
    aa.n_plan_ar++;
    print_str("[AA] Plan ar "); print_int(aa.n_plan_ar - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" urp="); print_int(urp);
    print_str(" rsp="); print_int(rsp);
    print_str(" cpl="); print_int(cpl); print_str("\n");
    return aa.n_plan_ar - 1;
}

int aa_const_ar(int ct_type, int cat, int fdc, int wlc, int flc, int year) {
    if (aa.n_const_ar >= MAX_CONST_AR) return -1;
    const_ar_t* c = &const_ars[aa.n_const_ar];
    c->co_id = aa.n_const_ar;
    c->co_type = ct_type;
    c->co_category = cat;
    c->found_const = fdc;
    c->wall_const = wlc;
    c->floor_const = flc;
    c->year = year;
    c->active = 1;
    aa.total_found_const += fdc;
    aa.n_const_ar++;
    print_str("[AA] Const ar "); print_int(aa.n_const_ar - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" fdc="); print_int(fdc);
    print_str(" wlc="); print_int(wlc);
    print_str(" flc="); print_int(flc); print_str("\n");
    return aa.n_const_ar - 1;
}

int aa_build_eq(int bt_type, int cat, int wts, int hvc, int elt, int year) {
    if (aa.n_build_eq >= MAX_BUILD_EQ) return -1;
    build_eq_t* b = &build_eqs[aa.n_build_eq];
    b->be_id = aa.n_build_eq;
    b->be_type = bt_type;
    b->be_category = cat;
    b->water_sys = wts;
    b->hvac_sys = hvc;
    b->elec_light = elt;
    b->year = year;
    b->active = 1;
    aa.total_water_sys += wts;
    aa.n_build_eq++;
    print_str("[AA] Build eq "); print_int(aa.n_build_eq - 1);
    print_str(" type="); print_int(bt_type);
    print_str(" cat="); print_int(cat);
    print_str(" wts="); print_int(wts);
    print_str(" hvc="); print_int(hvc);
    print_str(" elt="); print_int(elt); print_str("\n");
    return aa.n_build_eq - 1;
}

int aa_arch_co(int at_type, int cat, int cog, int ctt, int qmg, int year) {
    if (aa.n_arch_co >= MAX_ARCH_CO) return -1;
    arch_co_t* a = &arch_cos[aa.n_arch_co];
    a->ac_id = aa.n_arch_co;
    a->ac_type = at_type;
    a->ac_category = cat;
    a->const_org = cog;
    a->const_tech = ctt;
    a->qual_mgr = qmg;
    a->year = year;
    a->active = 1;
    aa.total_const_org += cog;
    aa.n_arch_co++;
    print_str("[AA] Arch co "); print_int(aa.n_arch_co - 1);
    print_str(" type="); print_int(at_type);
    print_str(" cat="); print_int(cat);
    print_str(" cog="); print_int(cog);
    print_str(" ctt="); print_int(ctt);
    print_str(" qmg="); print_int(qmg); print_str("\n");
    return aa.n_arch_co - 1;
}

void aa_build_des_report(void) {
    print_str("[AA] Building design report:\n");
    print_str("  Building design categories: "); print_int(aa.n_build_des); print_str("\n");
    print_str("  Total residential buildings: "); print_int(aa.total_resident_bld); print_str("\n");
}

void aa_plan_ar_report(void) {
    print_str("[AA] Planning report:\n");
    print_str("  Planning categories: "); print_int(aa.n_plan_ar); print_str("\n");
    print_str("  Total urban planning: "); print_int(aa.total_urban_plan); print_str("\n");
}

void aa_full_report(void) {
    print_str("[AA] Full report:\n");
    print_str("  Construction categories: "); print_int(aa.n_const_ar); print_str("\n");
    print_str("  Total foundation construction: "); print_int(aa.total_found_const); print_str("\n");
    print_str("  Building equipment categories: "); print_int(aa.n_build_eq); print_str("\n");
    print_str("  Total water supply system: "); print_int(aa.total_water_sys); print_str("\n");
    print_str("  Architectural construction categories: "); print_int(aa.n_arch_co); print_str("\n");
    print_str("  Total construction organization: "); print_int(aa.total_const_org); print_str("\n");
}

void aa_print_state(void) {
    print_str("[AA] Bd="); print_int(aa.n_build_des);
    print_str(" Pa="); print_int(aa.n_plan_ar);
    print_str(" Co="); print_int(aa.n_const_ar);
    print_str(" Be="); print_int(aa.n_build_eq);
    print_str(" Ac="); print_int(aa.n_arch_co);
    print_str("\n");
}

int main(void) {
    print_str("=== Architectural Engineering Admin Demo ===\n\n");
    aa_init();

    print_str("Building design...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int rsb = 55 + (i * 13);
        int pub = 40 + (i * 10);
        int ind = 22 + (i * 5);
        int year = 2020 + (i % 5);
        aa_build_des(type, cat, rsb, pub, ind, year);
    }

    print_str("\nPlanning...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int urp = 48 + (i * 11);
        int rsp = 35 + (i * 8);
        int cpl = 20 + (i * 4);
        int year = 2021 + (i % 4);
        aa_plan_ar(type, cat, urp, rsp, cpl, year);
    }

    print_str("\nConstruction...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int fdc = 42 + (i * 10);
        int wlc = 28 + (i * 7);
        int flc = 18 + (i * 4);
        int year = 2022 + (i % 3);
        aa_const_ar(type, cat, fdc, wlc, flc, year);
    }

    print_str("\nBuilding equipment...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int wts = 35 + (i * 8);
        int hvc = 25 + (i * 6);
        int elt = 15 + (i * 3);
        int year = 2023 + (i % 2);
        aa_build_eq(type, cat, wts, hvc, elt, year);
    }

    print_str("\nArchitectural construction...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cog = 30 + (i * 7);
        int ctt = 22 + (i * 5);
        int qmg = 12 + (i * 3);
        int year = 2024;
        aa_arch_co(type, cat, cog, ctt, qmg, year);
    }

    print_str("\nBuilding design report...\n");
    aa_build_des_report();

    print_str("\nPlanning report...\n");
    aa_plan_ar_report();

    print_str("\nFull report...\n");
    aa_full_report();

    print_str("\nFinal state...\n");
    aa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
