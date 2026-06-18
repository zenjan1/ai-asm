/* petroleum_engineering_admin: Petroleum engineering administration system (v1.0)
 * Oil/gas exploration, drilling, oilfield development, oil/gas storage, petroleum refining
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

#define MAX_OIL_EX      16
#define MAX_DRILL       14
#define MAX_OIL_DEV     12
#define MAX_OIL_ST      10
#define MAX_PET_RF      10

typedef struct {
    int    oe_id;
    int    oe_type;
    int    oe_category;
    int    seismic_ex;
    int    log_interp;
    int    geo_rec;
    int    year;
    int    active;
} oil_ex_t;

typedef struct {
    int    dr_id;
    int    dr_type;
    int    dr_category;
    int    rot_drill;
    int    dir_drill;
    int    hor_drill;
    int    year;
    int    active;
} drill_t;

typedef struct {
    int    od_id;
    int    od_type;
    int    od_category;
    int    reserv_eng;
    int    prod_eng;
    int    water_fld;
    int    year;
    int    active;
} oil_dev_t;

typedef struct {
    int    os_id;
    int    os_type;
    int    os_category;
    int    pipe_tr;
    int    field_col;
    int    oil_stor;
    int    year;
    int    active;
} oil_st_t;

typedef struct {
    int    rf_id;
    int    rf_type;
    int    rf_category;
    int    atm_dist;
    int    cat_crack;
    int    hydro_crack;
    int    year;
    int    active;
} pet_rf_t;

typedef struct {
    int    n_oil_ex;
    int    n_drill;
    int    n_oil_dev;
    int    n_oil_st;
    int    n_pet_rf;
    int    total_seismic_ex;
    int    total_rot_drill;
    int    total_reserv_eng;
    int    total_pipe_tr;
    int    total_atm_dist;
} pea_state_t;

static oil_ex_t oil_exs[MAX_OIL_EX];
static drill_t drills[MAX_DRILL];
static oil_dev_t oil_devs[MAX_OIL_DEV];
static oil_st_t oil_sts[MAX_OIL_ST];
static pet_rf_t pet_rfs[MAX_PET_RF];
static pea_state_t pea;

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

int pea_init(void) {
    if (initialized) return -1;
    pea.n_oil_ex = 0; pea.n_drill = 0; pea.n_oil_dev = 0;
    pea.n_oil_st = 0; pea.n_pet_rf = 0;
    pea.total_seismic_ex = 0; pea.total_rot_drill = 0;
    pea.total_reserv_eng = 0; pea.total_pipe_tr = 0;
    pea.total_atm_dist = 0;
    for (int i = 0; i < MAX_OIL_EX; i++) oil_exs[i].active = 0;
    for (int i = 0; i < MAX_DRILL; i++) drills[i].active = 0;
    for (int i = 0; i < MAX_OIL_DEV; i++) oil_devs[i].active = 0;
    for (int i = 0; i < MAX_OIL_ST; i++) oil_sts[i].active = 0;
    for (int i = 0; i < MAX_PET_RF; i++) pet_rfs[i].active = 0;
    initialized = 1;
    print_str("[PEA] Petroleum engineering initialized\n");
    return 0;
}

int pea_oil_ex(int ot_type, int cat, int ses, int lgi, int gcr, int year) {
    if (pea.n_oil_ex >= MAX_OIL_EX) return -1;
    oil_ex_t* o = &oil_exs[pea.n_oil_ex];
    o->oe_id = pea.n_oil_ex;
    o->oe_type = ot_type;
    o->oe_category = cat;
    o->seismic_ex = ses;
    o->log_interp = lgi;
    o->geo_rec = gcr;
    o->year = year;
    o->active = 1;
    pea.total_seismic_ex += ses;
    pea.n_oil_ex++;
    print_str("[PEA] Oil ex "); print_int(pea.n_oil_ex - 1);
    print_str(" type="); print_int(ot_type);
    print_str(" cat="); print_int(cat);
    print_str(" ses="); print_int(ses);
    print_str(" lgi="); print_int(lgi);
    print_str(" gcr="); print_int(gcr); print_str("\n");
    return pea.n_oil_ex - 1;
}

int pea_drill(int dt_type, int cat, int rtv, int drv, int hrv, int year) {
    if (pea.n_drill >= MAX_DRILL) return -1;
    drill_t* d = &drills[pea.n_drill];
    d->dr_id = pea.n_drill;
    d->dr_type = dt_type;
    d->dr_category = cat;
    d->rot_drill = rtv;
    d->dir_drill = drv;
    d->hor_drill = hrv;
    d->year = year;
    d->active = 1;
    pea.total_rot_drill += rtv;
    pea.n_drill++;
    print_str("[PEA] Drill "); print_int(pea.n_drill - 1);
    print_str(" type="); print_int(dt_type);
    print_str(" cat="); print_int(cat);
    print_str(" rtv="); print_int(rtv);
    print_str(" drv="); print_int(drv);
    print_str(" hrv="); print_int(hrv); print_str("\n");
    return pea.n_drill - 1;
}

int pea_oil_dev(int ot_type, int cat, int rse, int pre, int wtf, int year) {
    if (pea.n_oil_dev >= MAX_OIL_DEV) return -1;
    oil_dev_t* o = &oil_devs[pea.n_oil_dev];
    o->od_id = pea.n_oil_dev;
    o->od_type = ot_type;
    o->od_category = cat;
    o->reserv_eng = rse;
    o->prod_eng = pre;
    o->water_fld = wtf;
    o->year = year;
    o->active = 1;
    pea.total_reserv_eng += rse;
    pea.n_oil_dev++;
    print_str("[PEA] Oil dev "); print_int(pea.n_oil_dev - 1);
    print_str(" type="); print_int(ot_type);
    print_str(" cat="); print_int(cat);
    print_str(" rse="); print_int(rse);
    print_str(" pre="); print_int(pre);
    print_str(" wtf="); print_int(wtf); print_str("\n");
    return pea.n_oil_dev - 1;
}

int pea_oil_st(int st_type, int cat, int ppt, int fcl, int ost, int year) {
    if (pea.n_oil_st >= MAX_OIL_ST) return -1;
    oil_st_t* o = &oil_sts[pea.n_oil_st];
    o->os_id = pea.n_oil_st;
    o->os_type = st_type;
    o->os_category = cat;
    o->pipe_tr = ppt;
    o->field_col = fcl;
    o->oil_stor = ost;
    o->year = year;
    o->active = 1;
    pea.total_pipe_tr += ppt;
    pea.n_oil_st++;
    print_str("[PEA] Oil st "); print_int(pea.n_oil_st - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" ppt="); print_int(ppt);
    print_str(" fcl="); print_int(fcl);
    print_str(" ost="); print_int(ost); print_str("\n");
    return pea.n_oil_st - 1;
}

int pea_pet_rf(int rt_type, int cat, int atd, int ctc, int hdc, int year) {
    if (pea.n_pet_rf >= MAX_PET_RF) return -1;
    pet_rf_t* p = &pet_rfs[pea.n_pet_rf];
    p->rf_id = pea.n_pet_rf;
    p->rf_type = rt_type;
    p->rf_category = cat;
    p->atm_dist = atd;
    p->cat_crack = ctc;
    p->hydro_crack = hdc;
    p->year = year;
    p->active = 1;
    pea.total_atm_dist += atd;
    pea.n_pet_rf++;
    print_str("[PEA] Pet rf "); print_int(pea.n_pet_rf - 1);
    print_str(" type="); print_int(rt_type);
    print_str(" cat="); print_int(cat);
    print_str(" atd="); print_int(atd);
    print_str(" ctc="); print_int(ctc);
    print_str(" hdc="); print_int(hdc); print_str("\n");
    return pea.n_pet_rf - 1;
}

void pea_oil_ex_report(void) {
    print_str("[PEA] Oil exploration report:\n");
    print_str("  Oil exploration categories: "); print_int(pea.n_oil_ex); print_str("\n");
    print_str("  Total seismic exploration: "); print_int(pea.total_seismic_ex); print_str("\n");
}

void pea_drill_report(void) {
    print_str("[PEA] Drilling report:\n");
    print_str("  Drilling categories: "); print_int(pea.n_drill); print_str("\n");
    print_str("  Total rotary drilling: "); print_int(pea.total_rot_drill); print_str("\n");
}

void pea_full_report(void) {
    print_str("[PEA] Full report:\n");
    print_str("  Oilfield development categories: "); print_int(pea.n_oil_dev); print_str("\n");
    print_str("  Total reservoir engineering: "); print_int(pea.total_reserv_eng); print_str("\n");
    print_str("  Oil/gas storage categories: "); print_int(pea.n_oil_st); print_str("\n");
    print_str("  Total pipeline transport: "); print_int(pea.total_pipe_tr); print_str("\n");
    print_str("  Petroleum refining categories: "); print_int(pea.n_pet_rf); print_str("\n");
    print_str("  Total atmospheric distillation: "); print_int(pea.total_atm_dist); print_str("\n");
}

void pea_print_state(void) {
    print_str("[PEA] Oe="); print_int(pea.n_oil_ex);
    print_str(" Dr="); print_int(pea.n_drill);
    print_str(" Od="); print_int(pea.n_oil_dev);
    print_str(" Os="); print_int(pea.n_oil_st);
    print_str(" Rf="); print_int(pea.n_pet_rf);
    print_str("\n");
}

int main(void) {
    print_str("=== Petroleum Engineering Admin Demo ===\n\n");
    pea_init();

    print_str("Oil exploration...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int ses = 55 + (i * 13);
        int lgi = 40 + (i * 10);
        int gcr = 22 + (i * 5);
        int year = 2020 + (i % 5);
        pea_oil_ex(type, cat, ses, lgi, gcr, year);
    }

    print_str("\nDrilling...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int rtv = 48 + (i * 11);
        int drv = 35 + (i * 8);
        int hrv = 20 + (i * 4);
        int year = 2021 + (i % 4);
        pea_drill(type, cat, rtv, drv, hrv, year);
    }

    print_str("\nOilfield development...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int rse = 42 + (i * 10);
        int pre = 28 + (i * 7);
        int wtf = 18 + (i * 4);
        int year = 2022 + (i % 3);
        pea_oil_dev(type, cat, rse, pre, wtf, year);
    }

    print_str("\nOil/gas storage...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ppt = 35 + (i * 8);
        int fcl = 25 + (i * 6);
        int ost = 15 + (i * 3);
        int year = 2023 + (i % 2);
        pea_oil_st(type, cat, ppt, fcl, ost, year);
    }

    print_str("\nPetroleum refining...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int atd = 30 + (i * 7);
        int ctc = 22 + (i * 5);
        int hdc = 12 + (i * 3);
        int year = 2024;
        pea_pet_rf(type, cat, atd, ctc, hdc, year);
    }

    print_str("\nOil exploration report...\n");
    pea_oil_ex_report();

    print_str("\nDrilling report...\n");
    pea_drill_report();

    print_str("\nFull report...\n");
    pea_full_report();

    print_str("\nFinal state...\n");
    pea_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
