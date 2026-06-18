/* water_resources_engineering_admin: Water resources engineering administration system (v1.0)
 * Hydrology, hydraulic structures, irrigation/drainage, flood control, hydropower
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

#define MAX_HYDROL      16
#define MAX_HYDR_ST     14
#define MAX_IRRI_DR     12
#define MAX_FLOOD_C     10
#define MAX_HYDRO_P     10

typedef struct {
    int    hy_id;
    int    hy_type;
    int    hy_category;
    int    runoff_cal;
    int    flood_cal;
    int    low_cal;
    int    year;
    int    active;
} hydrol_t;

typedef struct {
    int    hs_id;
    int    hs_type;
    int    hs_category;
    int    dam_eng;
    int    sluice_eng;
    int    spillway;
    int    year;
    int    active;
} hydr_st_t;

typedef struct {
    int    ir_id;
    int    ir_type;
    int    ir_category;
    int    irrig_eng;
    int    drain_eng;
    int    water_sav;
    int    year;
    int    active;
} irri_dr_t;

typedef struct {
    int    fc_id;
    int    fc_type;
    int    fc_category;
    int    dike_eng;
    int    flood_sto;
    int    flood_for;
    int    year;
    int    active;
} flood_c_t;

typedef struct {
    int    hp_id;
    int    hp_type;
    int    hp_category;
    int    hydro_pln;
    int    turbine_un;
    int    power_op;
    int    year;
    int    active;
} hydro_p_t;

typedef struct {
    int    n_hydrol;
    int    n_hydr_st;
    int    n_irri_dr;
    int    n_flood_c;
    int    n_hydro_p;
    int    total_runoff_cal;
    int    total_dam_eng;
    int    total_irrig_eng;
    int    total_dike_eng;
    int    total_hydro_pln;
} wra_state_t;

static hydrol_t hydrols[MAX_HYDROL];
static hydr_st_t hydr_sts[MAX_HYDR_ST];
static irri_dr_t irri_drs[MAX_IRRI_DR];
static flood_c_t flood_cs[MAX_FLOOD_C];
static hydro_p_t hydro_ps[MAX_HYDRO_P];
static wra_state_t wra;

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

int wra_init(void) {
    if (initialized) return -1;
    wra.n_hydrol = 0; wra.n_hydr_st = 0; wra.n_irri_dr = 0;
    wra.n_flood_c = 0; wra.n_hydro_p = 0;
    wra.total_runoff_cal = 0; wra.total_dam_eng = 0;
    wra.total_irrig_eng = 0; wra.total_dike_eng = 0;
    wra.total_hydro_pln = 0;
    for (int i = 0; i < MAX_HYDROL; i++) hydrols[i].active = 0;
    for (int i = 0; i < MAX_HYDR_ST; i++) hydr_sts[i].active = 0;
    for (int i = 0; i < MAX_IRRI_DR; i++) irri_drs[i].active = 0;
    for (int i = 0; i < MAX_FLOOD_C; i++) flood_cs[i].active = 0;
    for (int i = 0; i < MAX_HYDRO_P; i++) hydro_ps[i].active = 0;
    initialized = 1;
    print_str("[WRA] Water resources engineering initialized\n");
    return 0;
}

int wra_hydrol(int ht_type, int cat, int rnc, int flc, int lwc, int year) {
    if (wra.n_hydrol >= MAX_HYDROL) return -1;
    hydrol_t* h = &hydrols[wra.n_hydrol];
    h->hy_id = wra.n_hydrol;
    h->hy_type = ht_type;
    h->hy_category = cat;
    h->runoff_cal = rnc;
    h->flood_cal = flc;
    h->low_cal = lwc;
    h->year = year;
    h->active = 1;
    wra.total_runoff_cal += rnc;
    wra.n_hydrol++;
    print_str("[WRA] Hydrol "); print_int(wra.n_hydrol - 1);
    print_str(" type="); print_int(ht_type);
    print_str(" cat="); print_int(cat);
    print_str(" rnc="); print_int(rnc);
    print_str(" flc="); print_int(flc);
    print_str(" lwc="); print_int(lwc); print_str("\n");
    return wra.n_hydrol - 1;
}

int wra_hydr_st(int st_type, int cat, int dam, int slc, int spw, int year) {
    if (wra.n_hydr_st >= MAX_HYDR_ST) return -1;
    hydr_st_t* h = &hydr_sts[wra.n_hydr_st];
    h->hs_id = wra.n_hydr_st;
    h->hs_type = st_type;
    h->hs_category = cat;
    h->dam_eng = dam;
    h->sluice_eng = slc;
    h->spillway = spw;
    h->year = year;
    h->active = 1;
    wra.total_dam_eng += dam;
    wra.n_hydr_st++;
    print_str("[WRA] Hydr st "); print_int(wra.n_hydr_st - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" dam="); print_int(dam);
    print_str(" slc="); print_int(slc);
    print_str(" spw="); print_int(spw); print_str("\n");
    return wra.n_hydr_st - 1;
}

int wra_irri_dr(int it_type, int cat, int ire, int dre, int wsv, int year) {
    if (wra.n_irri_dr >= MAX_IRRI_DR) return -1;
    irri_dr_t* n = &irri_drs[wra.n_irri_dr];
    n->ir_id = wra.n_irri_dr;
    n->ir_type = it_type;
    n->ir_category = cat;
    n->irrig_eng = ire;
    n->drain_eng = dre;
    n->water_sav = wsv;
    n->year = year;
    n->active = 1;
    wra.total_irrig_eng += ire;
    wra.n_irri_dr++;
    print_str("[WRA] Irri dr "); print_int(wra.n_irri_dr - 1);
    print_str(" type="); print_int(it_type);
    print_str(" cat="); print_int(cat);
    print_str(" ire="); print_int(ire);
    print_str(" dre="); print_int(dre);
    print_str(" wsv="); print_int(wsv); print_str("\n");
    return wra.n_irri_dr - 1;
}

int wra_flood_c(int ft_type, int cat, int dke, int fls, int flf, int year) {
    if (wra.n_flood_c >= MAX_FLOOD_C) return -1;
    flood_c_t* f = &flood_cs[wra.n_flood_c];
    f->fc_id = wra.n_flood_c;
    f->fc_type = ft_type;
    f->fc_category = cat;
    f->dike_eng = dke;
    f->flood_sto = fls;
    f->flood_for = flf;
    f->year = year;
    f->active = 1;
    wra.total_dike_eng += dke;
    wra.n_flood_c++;
    print_str("[WRA] Flood c "); print_int(wra.n_flood_c - 1);
    print_str(" type="); print_int(ft_type);
    print_str(" cat="); print_int(cat);
    print_str(" dke="); print_int(dke);
    print_str(" fls="); print_int(fls);
    print_str(" flf="); print_int(flf); print_str("\n");
    return wra.n_flood_c - 1;
}

int wra_hydro_p(int pt_type, int cat, int hpl, int trb, int pow, int year) {
    if (wra.n_hydro_p >= MAX_HYDRO_P) return -1;
    hydro_p_t* h = &hydro_ps[wra.n_hydro_p];
    h->hp_id = wra.n_hydro_p;
    h->hp_type = pt_type;
    h->hp_category = cat;
    h->hydro_pln = hpl;
    h->turbine_un = trb;
    h->power_op = pow;
    h->year = year;
    h->active = 1;
    wra.total_hydro_pln += hpl;
    wra.n_hydro_p++;
    print_str("[WRA] Hydro p "); print_int(wra.n_hydro_p - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" hpl="); print_int(hpl);
    print_str(" trb="); print_int(trb);
    print_str(" pow="); print_int(pow); print_str("\n");
    return wra.n_hydro_p - 1;
}

void wra_hydrol_report(void) {
    print_str("[WRA] Hydrology report:\n");
    print_str("  Hydrology categories: "); print_int(wra.n_hydrol); print_str("\n");
    print_str("  Total runoff calculation: "); print_int(wra.total_runoff_cal); print_str("\n");
}

void wra_hydr_st_report(void) {
    print_str("[WRA] Hydraulic structures report:\n");
    print_str("  Hydraulic structure categories: "); print_int(wra.n_hydr_st); print_str("\n");
    print_str("  Total dam engineering: "); print_int(wra.total_dam_eng); print_str("\n");
}

void wra_full_report(void) {
    print_str("[WRA] Full report:\n");
    print_str("  Irrigation/drainage categories: "); print_int(wra.n_irri_dr); print_str("\n");
    print_str("  Total irrigation engineering: "); print_int(wra.total_irrig_eng); print_str("\n");
    print_str("  Flood control categories: "); print_int(wra.n_flood_c); print_str("\n");
    print_str("  Total dike engineering: "); print_int(wra.total_dike_eng); print_str("\n");
    print_str("  Hydropower categories: "); print_int(wra.n_hydro_p); print_str("\n");
    print_str("  Total hydropower planning: "); print_int(wra.total_hydro_pln); print_str("\n");
}

void wra_print_state(void) {
    print_str("[WRA] Hy="); print_int(wra.n_hydrol);
    print_str(" Hs="); print_int(wra.n_hydr_st);
    print_str(" Ir="); print_int(wra.n_irri_dr);
    print_str(" Fc="); print_int(wra.n_flood_c);
    print_str(" Hp="); print_int(wra.n_hydro_p);
    print_str("\n");
}

int main(void) {
    print_str("=== Water Resources Engineering Admin Demo ===\n\n");
    wra_init();

    print_str("Hydrology...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int rnc = 55 + (i * 13);
        int flc = 40 + (i * 10);
        int lwc = 22 + (i * 5);
        int year = 2020 + (i % 5);
        wra_hydrol(type, cat, rnc, flc, lwc, year);
    }

    print_str("\nHydraulic structures...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int dam = 48 + (i * 11);
        int slc = 35 + (i * 8);
        int spw = 20 + (i * 4);
        int year = 2021 + (i % 4);
        wra_hydr_st(type, cat, dam, slc, spw, year);
    }

    print_str("\nIrrigation/drainage...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ire = 42 + (i * 10);
        int dre = 28 + (i * 7);
        int wsv = 18 + (i * 4);
        int year = 2022 + (i % 3);
        wra_irri_dr(type, cat, ire, dre, wsv, year);
    }

    print_str("\nFlood control...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int dke = 35 + (i * 8);
        int fls = 25 + (i * 6);
        int flf = 15 + (i * 3);
        int year = 2023 + (i % 2);
        wra_flood_c(type, cat, dke, fls, flf, year);
    }

    print_str("\nHydropower...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int hpl = 30 + (i * 7);
        int trb = 22 + (i * 5);
        int pow = 12 + (i * 3);
        int year = 2024;
        wra_hydro_p(type, cat, hpl, trb, pow, year);
    }

    print_str("\nHydrology report...\n");
    wra_hydrol_report();

    print_str("\nHydraulic structures report...\n");
    wra_hydr_st_report();

    print_str("\nFull report...\n");
    wra_full_report();

    print_str("\nFinal state...\n");
    wra_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
