/* automotive_admin: Automotive engineering administration system (v1.0)
 * Vehicle theory, vehicle design, manufacturing, electronics, NEV
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

#define MAX_VEHICLE_TH  16
#define MAX_VEHICLE_DS  14
#define MAX_MFG_AUTO    12
#define MAX_AUTO_EL     10
#define MAX_NEV         10

typedef struct {
    int    vt_id;
    int    vt_type;
    int    vt_category;
    int    vehicle_perf;
    int    braking;
    int    handling;
    int    year;
    int    active;
} vehicle_th_t;

typedef struct {
    int    vd_id;
    int    vd_type;
    int    vd_category;
    int    overall_vd;
    int    chassis_ds;
    int    body_ds;
    int    year;
    int    active;
} vehicle_ds_t;

typedef struct {
    int    ma_id;
    int    ma_type;
    int    ma_category;
    int    stamping;
    int    welding_ma;
    int    painting;
    int    year;
    int    active;
} mfg_auto_t;

typedef struct {
    int    el_id;
    int    el_type;
    int    el_category;
    int    engine_ctrl;
    int    chassis_ctrl;
    int    body_elec;
    int    year;
    int    active;
} auto_elec_t;

typedef struct {
    int    nv_id;
    int    nv_type;
    int    nv_category;
    int    bev;
    int    hev;
    int    fcev;
    int    year;
    int    active;
} nev_t;

typedef struct {
    int    n_vehicle_th;
    int    n_vehicle_ds;
    int    n_mfg_auto;
    int    n_auto_elec;
    int    n_nev;
    int    total_vehicle_perf;
    int    total_overall_vd;
    int    total_stamping;
    int    total_engine_ctrl;
    int    total_bev;
} ata_state_t;

static vehicle_th_t vehicle_ths[MAX_VEHICLE_TH];
static vehicle_ds_t vehicle_dss[MAX_VEHICLE_DS];
static mfg_auto_t mfg_autos[MAX_MFG_AUTO];
static auto_elec_t auto_elecs[MAX_AUTO_EL];
static nev_t nevs[MAX_NEV];
static ata_state_t ata;

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

int ata_init(void) {
    if (initialized) return -1;
    ata.n_vehicle_th = 0; ata.n_vehicle_ds = 0; ata.n_mfg_auto = 0;
    ata.n_auto_elec = 0; ata.n_nev = 0;
    ata.total_vehicle_perf = 0; ata.total_overall_vd = 0;
    ata.total_stamping = 0; ata.total_engine_ctrl = 0;
    ata.total_bev = 0;
    for (int i = 0; i < MAX_VEHICLE_TH; i++) vehicle_ths[i].active = 0;
    for (int i = 0; i < MAX_VEHICLE_DS; i++) vehicle_dss[i].active = 0;
    for (int i = 0; i < MAX_MFG_AUTO; i++) mfg_autos[i].active = 0;
    for (int i = 0; i < MAX_AUTO_EL; i++) auto_elecs[i].active = 0;
    for (int i = 0; i < MAX_NEV; i++) nevs[i].active = 0;
    initialized = 1;
    print_str("[ATA] Automotive initialized\n");
    return 0;
}

int ata_vehicle_th(int vt_type, int cat, int vpf, int brk, int hdl, int year) {
    if (ata.n_vehicle_th >= MAX_VEHICLE_TH) return -1;
    vehicle_th_t* v = &vehicle_ths[ata.n_vehicle_th];
    v->vt_id = ata.n_vehicle_th;
    v->vt_type = vt_type;
    v->vt_category = cat;
    v->vehicle_perf = vpf;
    v->braking = brk;
    v->handling = hdl;
    v->year = year;
    v->active = 1;
    ata.total_vehicle_perf += vpf;
    ata.n_vehicle_th++;
    print_str("[ATA] Vehicle th "); print_int(ata.n_vehicle_th - 1);
    print_str(" type="); print_int(vt_type);
    print_str(" cat="); print_int(cat);
    print_str(" vpf="); print_int(vpf);
    print_str(" brk="); print_int(brk);
    print_str(" hdl="); print_int(hdl); print_str("\n");
    return ata.n_vehicle_th - 1;
}

int ata_vehicle_ds(int vd_type, int cat, int ovr, int chs, int bdy, int year) {
    if (ata.n_vehicle_ds >= MAX_VEHICLE_DS) return -1;
    vehicle_ds_t* v = &vehicle_dss[ata.n_vehicle_ds];
    v->vd_id = ata.n_vehicle_ds;
    v->vd_type = vd_type;
    v->vd_category = cat;
    v->overall_vd = ovr;
    v->chassis_ds = chs;
    v->body_ds = bdy;
    v->year = year;
    v->active = 1;
    ata.total_overall_vd += ovr;
    ata.n_vehicle_ds++;
    print_str("[ATA] Vehicle ds "); print_int(ata.n_vehicle_ds - 1);
    print_str(" type="); print_int(vd_type);
    print_str(" cat="); print_int(cat);
    print_str(" ovr="); print_int(ovr);
    print_str(" chs="); print_int(chs);
    print_str(" bdy="); print_int(bdy); print_str("\n");
    return ata.n_vehicle_ds - 1;
}

int ata_mfg_auto(int mt_type, int cat, int stm, int wld, int pnt, int year) {
    if (ata.n_mfg_auto >= MAX_MFG_AUTO) return -1;
    mfg_auto_t* m = &mfg_autos[ata.n_mfg_auto];
    m->ma_id = ata.n_mfg_auto;
    m->ma_type = mt_type;
    m->ma_category = cat;
    m->stamping = stm;
    m->welding_ma = wld;
    m->painting = pnt;
    m->year = year;
    m->active = 1;
    ata.total_stamping += stm;
    ata.n_mfg_auto++;
    print_str("[ATA] Mfg auto "); print_int(ata.n_mfg_auto - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" stm="); print_int(stm);
    print_str(" wld="); print_int(wld);
    print_str(" pnt="); print_int(pnt); print_str("\n");
    return ata.n_mfg_auto - 1;
}

int ata_auto_elec(int et_type, int cat, int eng, int chs_e, int byd, int year) {
    if (ata.n_auto_elec >= MAX_AUTO_EL) return -1;
    auto_elec_t* a = &auto_elecs[ata.n_auto_elec];
    a->el_id = ata.n_auto_elec;
    a->el_type = et_type;
    a->el_category = cat;
    a->engine_ctrl = eng;
    a->chassis_ctrl = chs_e;
    a->body_elec = byd;
    a->year = year;
    a->active = 1;
    ata.total_engine_ctrl += eng;
    ata.n_auto_elec++;
    print_str("[ATA] Auto elec "); print_int(ata.n_auto_elec - 1);
    print_str(" type="); print_int(et_type);
    print_str(" cat="); print_int(cat);
    print_str(" eng="); print_int(eng);
    print_str(" chs="); print_int(chs_e);
    print_str(" byd="); print_int(byd); print_str("\n");
    return ata.n_auto_elec - 1;
}

int ata_nev(int nt_type, int cat, int bev_v, int hev_v, int fcev_v, int year) {
    if (ata.n_nev >= MAX_NEV) return -1;
    nev_t* n = &nevs[ata.n_nev];
    n->nv_id = ata.n_nev;
    n->nv_type = nt_type;
    n->nv_category = cat;
    n->bev = bev_v;
    n->hev = hev_v;
    n->fcev = fcev_v;
    n->year = year;
    n->active = 1;
    ata.total_bev += bev_v;
    ata.n_nev++;
    print_str("[ATA] NEV "); print_int(ata.n_nev - 1);
    print_str(" type="); print_int(nt_type);
    print_str(" cat="); print_int(cat);
    print_str(" bev="); print_int(bev_v);
    print_str(" hev="); print_int(hev_v);
    print_str(" fcv="); print_int(fcev_v); print_str("\n");
    return ata.n_nev - 1;
}

void ata_vehicle_th_report(void) {
    print_str("[ATA] Vehicle theory report:\n");
    print_str("  Vehicle theory categories: "); print_int(ata.n_vehicle_th); print_str("\n");
    print_str("  Total vehicle performance: "); print_int(ata.total_vehicle_perf); print_str("\n");
}

void ata_vehicle_ds_report(void) {
    print_str("[ATA] Vehicle design report:\n");
    print_str("  Vehicle design categories: "); print_int(ata.n_vehicle_ds); print_str("\n");
    print_str("  Total overall design: "); print_int(ata.total_overall_vd); print_str("\n");
}

void ata_full_report(void) {
    print_str("[ATA] Full report:\n");
    print_str("  Auto manufacturing categories: "); print_int(ata.n_mfg_auto); print_str("\n");
    print_str("  Total stamping: "); print_int(ata.total_stamping); print_str("\n");
    print_str("  Auto electronics categories: "); print_int(ata.n_auto_elec); print_str("\n");
    print_str("  Total engine control: "); print_int(ata.total_engine_ctrl); print_str("\n");
    print_str("  NEV categories: "); print_int(ata.n_nev); print_str("\n");
    print_str("  Total BEV: "); print_int(ata.total_bev); print_str("\n");
}

void ata_print_state(void) {
    print_str("[ATA] Vt="); print_int(ata.n_vehicle_th);
    print_str(" Vd="); print_int(ata.n_vehicle_ds);
    print_str(" Ma="); print_int(ata.n_mfg_auto);
    print_str(" El="); print_int(ata.n_auto_elec);
    print_str(" Nv="); print_int(ata.n_nev);
    print_str("\n");
}

int main(void) {
    print_str("=== Automotive Admin Demo ===\n\n");
    ata_init();

    print_str("Vehicle theory...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int vpf = 55 + (i * 13);
        int brk = 40 + (i * 10);
        int hdl = 22 + (i * 5);
        int year = 2020 + (i % 5);
        ata_vehicle_th(type, cat, vpf, brk, hdl, year);
    }

    print_str("\nVehicle design...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ovr = 48 + (i * 11);
        int chs = 35 + (i * 8);
        int bdy = 20 + (i * 4);
        int year = 2021 + (i % 4);
        ata_vehicle_ds(type, cat, ovr, chs, bdy, year);
    }

    print_str("\nAuto manufacturing...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int stm = 42 + (i * 10);
        int wld = 28 + (i * 7);
        int pnt = 18 + (i * 4);
        int year = 2022 + (i % 3);
        ata_mfg_auto(type, cat, stm, wld, pnt, year);
    }

    print_str("\nAuto electronics...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int eng = 35 + (i * 8);
        int chs_e = 25 + (i * 6);
        int byd = 15 + (i * 3);
        int year = 2023 + (i % 2);
        ata_auto_elec(type, cat, eng, chs_e, byd, year);
    }

    print_str("\nNEV...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int bev_v = 30 + (i * 7);
        int hev_v = 22 + (i * 5);
        int fcev_v = 12 + (i * 3);
        int year = 2024;
        ata_nev(type, cat, bev_v, hev_v, fcev_v, year);
    }

    print_str("\nVehicle theory report...\n");
    ata_vehicle_th_report();

    print_str("\nVehicle design report...\n");
    ata_vehicle_ds_report();

    print_str("\nFull report...\n");
    ata_full_report();

    print_str("\nFinal state...\n");
    ata_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
