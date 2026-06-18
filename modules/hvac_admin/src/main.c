/* hvac_admin: HVAC administration system (v1.0)
 * Heating, ventilation, air conditioning, refrigeration, air purification
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

#define MAX_HEATING     16
#define MAX_VENT        14
#define MAX_AC          12
#define MAX_COOLING     10
#define MAX_PURIFY      10

typedef struct {
    int    ht_id;
    int    ht_type;
    int    ht_category;
    int    hot_water;
    int    steam_ht;
    int    radiant_ht;
    int    year;
    int    active;
} heating_t;

typedef struct {
    int    vt_id;
    int    vt_type;
    int    vt_category;
    int    natural_vt;
    int    mechanical_vt;
    int    emergency_vt;
    int    year;
    int    active;
} vent_t;

typedef struct {
    int    ac_id;
    int    ac_type;
    int    ac_category;
    int    central_ac;
    int    split_ac;
    int    vrf;
    int    year;
    int    active;
} ac_t;

typedef struct {
    int    cl_id;
    int    cl_type;
    int    cl_category;
    int    cold_storage;
    int    process_cl;
    int    comfort_cl;
    int    year;
    int    active;
} cooling_t;

typedef struct {
    int    pf_id;
    int    pf_type;
    int    pf_category;
    int    filter;
    int    dust_collect;
    int    sterilize;
    int    year;
    int    active;
} purify_t;

typedef struct {
    int    n_heating;
    int    n_vent;
    int    n_ac;
    int    n_cooling;
    int    n_purify;
    int    total_hot_water;
    int    total_natural_vt;
    int    total_central_ac;
    int    total_cold_storage;
    int    total_filter;
} hvac_state_t;

static heating_t heatings[MAX_HEATING];
static vent_t vents[MAX_VENT];
static ac_t acs[MAX_AC];
static cooling_t coolings[MAX_COOLING];
static purify_t purifies[MAX_PURIFY];
static hvac_state_t hvac;

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

int hvac_init(void) {
    if (initialized) return -1;
    hvac.n_heating = 0; hvac.n_vent = 0; hvac.n_ac = 0;
    hvac.n_cooling = 0; hvac.n_purify = 0;
    hvac.total_hot_water = 0; hvac.total_natural_vt = 0;
    hvac.total_central_ac = 0; hvac.total_cold_storage = 0;
    hvac.total_filter = 0;
    for (int i = 0; i < MAX_HEATING; i++) heatings[i].active = 0;
    for (int i = 0; i < MAX_VENT; i++) vents[i].active = 0;
    for (int i = 0; i < MAX_AC; i++) acs[i].active = 0;
    for (int i = 0; i < MAX_COOLING; i++) coolings[i].active = 0;
    for (int i = 0; i < MAX_PURIFY; i++) purifies[i].active = 0;
    initialized = 1;
    print_str("[HVAC] HVAC initialized\n");
    return 0;
}

int hvac_heating(int ht_type, int cat, int hw, int stm, int rad, int year) {
    if (hvac.n_heating >= MAX_HEATING) return -1;
    heating_t* h = &heatings[hvac.n_heating];
    h->ht_id = hvac.n_heating;
    h->ht_type = ht_type;
    h->ht_category = cat;
    h->hot_water = hw;
    h->steam_ht = stm;
    h->radiant_ht = rad;
    h->year = year;
    h->active = 1;
    hvac.total_hot_water += hw;
    hvac.n_heating++;
    print_str("[HVAC] Heating "); print_int(hvac.n_heating - 1);
    print_str(" type="); print_int(ht_type);
    print_str(" cat="); print_int(cat);
    print_str(" hw="); print_int(hw);
    print_str(" stm="); print_int(stm);
    print_str(" rad="); print_int(rad); print_str("\n");
    return hvac.n_heating - 1;
}

int hvac_vent(int vn_type, int cat, int nat, int mech, int emrg, int year) {
    if (hvac.n_vent >= MAX_VENT) return -1;
    vent_t* v = &vents[hvac.n_vent];
    v->vt_id = hvac.n_vent;
    v->vt_type = vn_type;
    v->vt_category = cat;
    v->natural_vt = nat;
    v->mechanical_vt = mech;
    v->emergency_vt = emrg;
    v->year = year;
    v->active = 1;
    hvac.total_natural_vt += nat;
    hvac.n_vent++;
    print_str("[HVAC] Vent "); print_int(hvac.n_vent - 1);
    print_str(" type="); print_int(vn_type);
    print_str(" cat="); print_int(cat);
    print_str(" nat="); print_int(nat);
    print_str(" mch="); print_int(mech);
    print_str(" emr="); print_int(emrg); print_str("\n");
    return hvac.n_vent - 1;
}

int hvac_ac(int ac_type, int cat, int cntr, int splt, int vrf_v, int year) {
    if (hvac.n_ac >= MAX_AC) return -1;
    ac_t* a = &acs[hvac.n_ac];
    a->ac_id = hvac.n_ac;
    a->ac_type = ac_type;
    a->ac_category = cat;
    a->central_ac = cntr;
    a->split_ac = splt;
    a->vrf = vrf_v;
    a->year = year;
    a->active = 1;
    hvac.total_central_ac += cntr;
    hvac.n_ac++;
    print_str("[HVAC] AC "); print_int(hvac.n_ac - 1);
    print_str(" type="); print_int(ac_type);
    print_str(" cat="); print_int(cat);
    print_str(" cnr="); print_int(cntr);
    print_str(" spl="); print_int(splt);
    print_str(" vrf="); print_int(vrf_v); print_str("\n");
    return hvac.n_ac - 1;
}

int hvac_cooling(int cl_type, int cat, int cstr, int proc, int cmf, int year) {
    if (hvac.n_cooling >= MAX_COOLING) return -1;
    cooling_t* c = &coolings[hvac.n_cooling];
    c->cl_id = hvac.n_cooling;
    c->cl_type = cl_type;
    c->cl_category = cat;
    c->cold_storage = cstr;
    c->process_cl = proc;
    c->comfort_cl = cmf;
    c->year = year;
    c->active = 1;
    hvac.total_cold_storage += cstr;
    hvac.n_cooling++;
    print_str("[HVAC] Cooling "); print_int(hvac.n_cooling - 1);
    print_str(" type="); print_int(cl_type);
    print_str(" cat="); print_int(cat);
    print_str(" cst="); print_int(cstr);
    print_str(" prc="); print_int(proc);
    print_str(" cmf="); print_int(cmf); print_str("\n");
    return hvac.n_cooling - 1;
}

int hvac_purify(int pr_type, int cat, int flt, int dst, int strl, int year) {
    if (hvac.n_purify >= MAX_PURIFY) return -1;
    purify_t* p = &purifies[hvac.n_purify];
    p->pf_id = hvac.n_purify;
    p->pf_type = pr_type;
    p->pf_category = cat;
    p->filter = flt;
    p->dust_collect = dst;
    p->sterilize = strl;
    p->year = year;
    p->active = 1;
    hvac.total_filter += flt;
    hvac.n_purify++;
    print_str("[HVAC] Purify "); print_int(hvac.n_purify - 1);
    print_str(" type="); print_int(pr_type);
    print_str(" cat="); print_int(cat);
    print_str(" flt="); print_int(flt);
    print_str(" dst="); print_int(dst);
    print_str(" str="); print_int(strl); print_str("\n");
    return hvac.n_purify - 1;
}

void hvac_heating_report(void) {
    print_str("[HVAC] Heating report:\n");
    print_str("  Heating categories: "); print_int(hvac.n_heating); print_str("\n");
    print_str("  Total hot water: "); print_int(hvac.total_hot_water); print_str("\n");
}

void hvac_vent_report(void) {
    print_str("[HVAC] Ventilation report:\n");
    print_str("  Ventilation categories: "); print_int(hvac.n_vent); print_str("\n");
    print_str("  Total natural ventilation: "); print_int(hvac.total_natural_vt); print_str("\n");
}

void hvac_full_report(void) {
    print_str("[HVAC] Full report:\n");
    print_str("  AC categories: "); print_int(hvac.n_ac); print_str("\n");
    print_str("  Total central AC: "); print_int(hvac.total_central_ac); print_str("\n");
    print_str("  Cooling categories: "); print_int(hvac.n_cooling); print_str("\n");
    print_str("  Total cold storage: "); print_int(hvac.total_cold_storage); print_str("\n");
    print_str("  Purification categories: "); print_int(hvac.n_purify); print_str("\n");
    print_str("  Total filters: "); print_int(hvac.total_filter); print_str("\n");
}

void hvac_print_state(void) {
    print_str("[HVAC] Ht="); print_int(hvac.n_heating);
    print_str(" Vt="); print_int(hvac.n_vent);
    print_str(" Ac="); print_int(hvac.n_ac);
    print_str(" Cl="); print_int(hvac.n_cooling);
    print_str(" Pf="); print_int(hvac.n_purify);
    print_str("\n");
}

int main(void) {
    print_str("=== HVAC Admin Demo ===\n\n");
    hvac_init();

    print_str("Heating...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int hw = 55 + (i * 13);
        int stm = 40 + (i * 10);
        int rad = 22 + (i * 5);
        int year = 2020 + (i % 5);
        hvac_heating(type, cat, hw, stm, rad, year);
    }

    print_str("\nVentilation...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int nat = 48 + (i * 11);
        int mech = 35 + (i * 8);
        int emrg = 20 + (i * 4);
        int year = 2021 + (i % 4);
        hvac_vent(type, cat, nat, mech, emrg, year);
    }

    print_str("\nAir conditioning...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cntr = 42 + (i * 10);
        int splt = 28 + (i * 7);
        int vrf_v = 18 + (i * 4);
        int year = 2022 + (i % 3);
        hvac_ac(type, cat, cntr, splt, vrf_v, year);
    }

    print_str("\nCooling...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cstr = 35 + (i * 8);
        int proc = 25 + (i * 6);
        int cmf = 15 + (i * 3);
        int year = 2023 + (i % 2);
        hvac_cooling(type, cat, cstr, proc, cmf, year);
    }

    print_str("\nAir purification...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int flt = 30 + (i * 7);
        int dst = 22 + (i * 5);
        int strl = 12 + (i * 3);
        int year = 2024;
        hvac_purify(type, cat, flt, dst, strl, year);
    }

    print_str("\nHeating report...\n");
    hvac_heating_report();

    print_str("\nVentilation report...\n");
    hvac_vent_report();

    print_str("\nFull report...\n");
    hvac_full_report();

    print_str("\nFinal state...\n");
    hvac_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
