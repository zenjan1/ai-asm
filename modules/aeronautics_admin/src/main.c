/* aeronautics_admin: Aeronautics administration system (v1.0)
 * Flight principles, aircraft design, structures, engines, flight mechanics
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

#define MAX_FLIGHT_PR   16
#define MAX_AIRCRAFT_DS 14
#define MAX_AIRCRAFT_ST 12
#define MAX_ENGINE      10
#define MAX_FLIGHT_MC   10

typedef struct {
    int    fp_id;
    int    fp_type;
    int    fp_category;
    int    lift;
    int    drag;
    int    thrust;
    int    year;
    int    active;
} flight_pr_t;

typedef struct {
    int    ad_id;
    int    ad_type;
    int    ad_category;
    int    overall_ds;
    int    aero_ds;
    int    struct_ds;
    int    year;
    int    active;
} aircraft_ds_t;

typedef struct {
    int    ast_id;
    int    ast_type;
    int    ast_category;
    int    fuselage;
    int    wing;
    int    empennage;
    int    year;
    int    active;
} aircraft_st_t;

typedef struct {
    int    en_id;
    int    en_type;
    int    en_category;
    int    piston_en;
    int    turbojet;
    int    turbofan;
    int    year;
    int    active;
} engine_t;

typedef struct {
    int    fmc_id;
    int    fmc_type;
    int    fmc_category;
    int    level_flight;
    int    maneuver;
    int    stability;
    int    year;
    int    active;
} flight_mc_t;

typedef struct {
    int    n_flight_pr;
    int    n_aircraft_ds;
    int    n_aircraft_st;
    int    n_engine;
    int    n_flight_mc;
    int    total_lift;
    int    total_overall_ds;
    int    total_fuselage;
    int    total_piston_en;
    int    total_level_flight;
} ana_state_t;

static flight_pr_t flight_prs[MAX_FLIGHT_PR];
static aircraft_ds_t aircraft_dss[MAX_AIRCRAFT_DS];
static aircraft_st_t aircraft_sts[MAX_AIRCRAFT_ST];
static engine_t engines[MAX_ENGINE];
static flight_mc_t flight_mcs[MAX_FLIGHT_MC];
static ana_state_t ana;

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

int ana_init(void) {
    if (initialized) return -1;
    ana.n_flight_pr = 0; ana.n_aircraft_ds = 0; ana.n_aircraft_st = 0;
    ana.n_engine = 0; ana.n_flight_mc = 0;
    ana.total_lift = 0; ana.total_overall_ds = 0;
    ana.total_fuselage = 0; ana.total_piston_en = 0;
    ana.total_level_flight = 0;
    for (int i = 0; i < MAX_FLIGHT_PR; i++) flight_prs[i].active = 0;
    for (int i = 0; i < MAX_AIRCRAFT_DS; i++) aircraft_dss[i].active = 0;
    for (int i = 0; i < MAX_AIRCRAFT_ST; i++) aircraft_sts[i].active = 0;
    for (int i = 0; i < MAX_ENGINE; i++) engines[i].active = 0;
    for (int i = 0; i < MAX_FLIGHT_MC; i++) flight_mcs[i].active = 0;
    initialized = 1;
    print_str("[ANA] Aeronautics initialized\n");
    return 0;
}

int ana_flight_pr(int fp_type, int cat, int lft, int drg, int thr, int year) {
    if (ana.n_flight_pr >= MAX_FLIGHT_PR) return -1;
    flight_pr_t* f = &flight_prs[ana.n_flight_pr];
    f->fp_id = ana.n_flight_pr;
    f->fp_type = fp_type;
    f->fp_category = cat;
    f->lift = lft;
    f->drag = drg;
    f->thrust = thr;
    f->year = year;
    f->active = 1;
    ana.total_lift += lft;
    ana.n_flight_pr++;
    print_str("[ANA] Flight pr "); print_int(ana.n_flight_pr - 1);
    print_str(" type="); print_int(fp_type);
    print_str(" cat="); print_int(cat);
    print_str(" lft="); print_int(lft);
    print_str(" drg="); print_int(drg);
    print_str(" thr="); print_int(thr); print_str("\n");
    return ana.n_flight_pr - 1;
}

int ana_aircraft_ds(int ad_type, int cat, int ov, int ar, int sr, int year) {
    if (ana.n_aircraft_ds >= MAX_AIRCRAFT_DS) return -1;
    aircraft_ds_t* a = &aircraft_dss[ana.n_aircraft_ds];
    a->ad_id = ana.n_aircraft_ds;
    a->ad_type = ad_type;
    a->ad_category = cat;
    a->overall_ds = ov;
    a->aero_ds = ar;
    a->struct_ds = sr;
    a->year = year;
    a->active = 1;
    ana.total_overall_ds += ov;
    ana.n_aircraft_ds++;
    print_str("[ANA] Aircraft ds "); print_int(ana.n_aircraft_ds - 1);
    print_str(" type="); print_int(ad_type);
    print_str(" cat="); print_int(cat);
    print_str(" ov="); print_int(ov);
    print_str(" ar="); print_int(ar);
    print_str(" sr="); print_int(sr); print_str("\n");
    return ana.n_aircraft_ds - 1;
}

int ana_aircraft_st(int at_type, int cat, int fus, int wng, int emp, int year) {
    if (ana.n_aircraft_st >= MAX_AIRCRAFT_ST) return -1;
    aircraft_st_t* a = &aircraft_sts[ana.n_aircraft_st];
    a->ast_id = ana.n_aircraft_st;
    a->ast_type = at_type;
    a->ast_category = cat;
    a->fuselage = fus;
    a->wing = wng;
    a->empennage = emp;
    a->year = year;
    a->active = 1;
    ana.total_fuselage += fus;
    ana.n_aircraft_st++;
    print_str("[ANA] Aircraft st "); print_int(ana.n_aircraft_st - 1);
    print_str(" type="); print_int(at_type);
    print_str(" cat="); print_int(cat);
    print_str(" fus="); print_int(fus);
    print_str(" wng="); print_int(wng);
    print_str(" emp="); print_int(emp); print_str("\n");
    return ana.n_aircraft_st - 1;
}

int ana_engine(int en_type, int cat, int pst, int trj, int tfn, int year) {
    if (ana.n_engine >= MAX_ENGINE) return -1;
    engine_t* e = &engines[ana.n_engine];
    e->en_id = ana.n_engine;
    e->en_type = en_type;
    e->en_category = cat;
    e->piston_en = pst;
    e->turbojet = trj;
    e->turbofan = tfn;
    e->year = year;
    e->active = 1;
    ana.total_piston_en += pst;
    ana.n_engine++;
    print_str("[ANA] Engine "); print_int(ana.n_engine - 1);
    print_str(" type="); print_int(en_type);
    print_str(" cat="); print_int(cat);
    print_str(" pst="); print_int(pst);
    print_str(" trj="); print_int(trj);
    print_str(" tfn="); print_int(tfn); print_str("\n");
    return ana.n_engine - 1;
}

int ana_flight_mc(int ft_type, int cat, int lvf, int mnu, int stb, int year) {
    if (ana.n_flight_mc >= MAX_FLIGHT_MC) return -1;
    flight_mc_t* f = &flight_mcs[ana.n_flight_mc];
    f->fmc_id = ana.n_flight_mc;
    f->fmc_type = ft_type;
    f->fmc_category = cat;
    f->level_flight = lvf;
    f->maneuver = mnu;
    f->stability = stb;
    f->year = year;
    f->active = 1;
    ana.total_level_flight += lvf;
    ana.n_flight_mc++;
    print_str("[ANA] Flight mc "); print_int(ana.n_flight_mc - 1);
    print_str(" type="); print_int(ft_type);
    print_str(" cat="); print_int(cat);
    print_str(" lvf="); print_int(lvf);
    print_str(" mnu="); print_int(mnu);
    print_str(" stb="); print_int(stb); print_str("\n");
    return ana.n_flight_mc - 1;
}

void ana_flight_pr_report(void) {
    print_str("[ANA] Flight principles report:\n");
    print_str("  Flight principles categories: "); print_int(ana.n_flight_pr); print_str("\n");
    print_str("  Total lift: "); print_int(ana.total_lift); print_str("\n");
}

void ana_aircraft_ds_report(void) {
    print_str("[ANA] Aircraft design report:\n");
    print_str("  Aircraft design categories: "); print_int(ana.n_aircraft_ds); print_str("\n");
    print_str("  Total overall design: "); print_int(ana.total_overall_ds); print_str("\n");
}

void ana_full_report(void) {
    print_str("[ANA] Full report:\n");
    print_str("  Aircraft structure categories: "); print_int(ana.n_aircraft_st); print_str("\n");
    print_str("  Total fuselage: "); print_int(ana.total_fuselage); print_str("\n");
    print_str("  Engine categories: "); print_int(ana.n_engine); print_str("\n");
    print_str("  Total piston engines: "); print_int(ana.total_piston_en); print_str("\n");
    print_str("  Flight mechanics categories: "); print_int(ana.n_flight_mc); print_str("\n");
    print_str("  Total level flight: "); print_int(ana.total_level_flight); print_str("\n");
}

void ana_print_state(void) {
    print_str("[ANA] Fp="); print_int(ana.n_flight_pr);
    print_str(" Ad="); print_int(ana.n_aircraft_ds);
    print_str(" As="); print_int(ana.n_aircraft_st);
    print_str(" En="); print_int(ana.n_engine);
    print_str(" Fm="); print_int(ana.n_flight_mc);
    print_str("\n");
}

int main(void) {
    print_str("=== Aeronautics Admin Demo ===\n\n");
    ana_init();

    print_str("Flight principles...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int lft = 55 + (i * 13);
        int drg = 40 + (i * 10);
        int thr = 22 + (i * 5);
        int year = 2020 + (i % 5);
        ana_flight_pr(type, cat, lft, drg, thr, year);
    }

    print_str("\nAircraft design...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ov = 48 + (i * 11);
        int ar = 35 + (i * 8);
        int sr = 20 + (i * 4);
        int year = 2021 + (i % 4);
        ana_aircraft_ds(type, cat, ov, ar, sr, year);
    }

    print_str("\nAircraft structures...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int fus = 42 + (i * 10);
        int wng = 28 + (i * 7);
        int emp = 18 + (i * 4);
        int year = 2022 + (i % 3);
        ana_aircraft_st(type, cat, fus, wng, emp, year);
    }

    print_str("\nEngines...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pst = 35 + (i * 8);
        int trj = 25 + (i * 6);
        int tfn = 15 + (i * 3);
        int year = 2023 + (i % 2);
        ana_engine(type, cat, pst, trj, tfn, year);
    }

    print_str("\nFlight mechanics...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int lvf = 30 + (i * 7);
        int mnu = 22 + (i * 5);
        int stb = 12 + (i * 3);
        int year = 2024;
        ana_flight_mc(type, cat, lvf, mnu, stb, year);
    }

    print_str("\nFlight principles report...\n");
    ana_flight_pr_report();

    print_str("\nAircraft design report...\n");
    ana_aircraft_ds_report();

    print_str("\nFull report...\n");
    ana_full_report();

    print_str("\nFinal state...\n");
    ana_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
