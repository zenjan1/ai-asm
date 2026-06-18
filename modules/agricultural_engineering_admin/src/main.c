/* agricultural_engineering_admin: Agricultural engineering administration system (v1.0)
 * Agricultural mechanization, electrification, water/soil, buildings, processing
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

#define MAX_AG_MECH     16
#define MAX_AG_ELEC     14
#define MAX_AG_WS       12
#define MAX_AG_BLD      10
#define MAX_AG_PROC     10

typedef struct {
    int    am_id;
    int    am_type;
    int    am_category;
    int    tractor;
    int    harvester;
    int    planter;
    int    year;
    int    active;
} ag_mech_t;

typedef struct {
    int    ae_id;
    int    ae_type;
    int    ae_category;
    int    ag_power;
    int    ag_auto;
    int    precision_ag;
    int    year;
    int    active;
} ag_elec_t;

typedef struct {
    int    ws_id;
    int    ws_type;
    int    ws_category;
    int    field_irr;
    int    field_drn;
    int    water_cons;
    int    year;
    int    active;
} ag_ws_t;

typedef struct {
    int    ab_id;
    int    ab_type;
    int    ab_category;
    int    ag_build;
    int    greenhouse;
    int    cold_chain;
    int    year;
    int    active;
} ag_bld_t;

typedef struct {
    int    ap_id;
    int    ap_type;
    int    ap_category;
    int    grain_proc;
    int    fruit_proc;
    int    livestock_proc;
    int    year;
    int    active;
} ag_proc_t;

typedef struct {
    int    n_ag_mech;
    int    n_ag_elec;
    int    n_ag_ws;
    int    n_ag_bld;
    int    n_ag_proc;
    int    total_tractor;
    int    total_ag_power;
    int    total_field_irr;
    int    total_ag_build;
    int    total_grain_proc;
} ag_state_t;

static ag_mech_t ag_mechs[MAX_AG_MECH];
static ag_elec_t ag_elecs[MAX_AG_ELEC];
static ag_ws_t ag_wss[MAX_AG_WS];
static ag_bld_t ag_blds[MAX_AG_BLD];
static ag_proc_t ag_procs[MAX_AG_PROC];
static ag_state_t ags;

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

int ags_init(void) {
    if (initialized) return -1;
    ags.n_ag_mech = 0; ags.n_ag_elec = 0; ags.n_ag_ws = 0;
    ags.n_ag_bld = 0; ags.n_ag_proc = 0;
    ags.total_tractor = 0; ags.total_ag_power = 0;
    ags.total_field_irr = 0; ags.total_ag_build = 0;
    ags.total_grain_proc = 0;
    for (int i = 0; i < MAX_AG_MECH; i++) ag_mechs[i].active = 0;
    for (int i = 0; i < MAX_AG_ELEC; i++) ag_elecs[i].active = 0;
    for (int i = 0; i < MAX_AG_WS; i++) ag_wss[i].active = 0;
    for (int i = 0; i < MAX_AG_BLD; i++) ag_blds[i].active = 0;
    for (int i = 0; i < MAX_AG_PROC; i++) ag_procs[i].active = 0;
    initialized = 1;
    print_str("[AGS] Agricultural engineering initialized\n");
    return 0;
}

int ags_ag_mech(int mt_type, int cat, int trc, int hrv, int plt, int year) {
    if (ags.n_ag_mech >= MAX_AG_MECH) return -1;
    ag_mech_t* a = &ag_mechs[ags.n_ag_mech];
    a->am_id = ags.n_ag_mech;
    a->am_type = mt_type;
    a->am_category = cat;
    a->tractor = trc;
    a->harvester = hrv;
    a->planter = plt;
    a->year = year;
    a->active = 1;
    ags.total_tractor += trc;
    ags.n_ag_mech++;
    print_str("[AGS] Ag mech "); print_int(ags.n_ag_mech - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" trc="); print_int(trc);
    print_str(" hrv="); print_int(hrv);
    print_str(" plt="); print_int(plt); print_str("\n");
    return ags.n_ag_mech - 1;
}

int ags_ag_elec(int et_type, int cat, int agp, int agt, int prg, int year) {
    if (ags.n_ag_elec >= MAX_AG_ELEC) return -1;
    ag_elec_t* a = &ag_elecs[ags.n_ag_elec];
    a->ae_id = ags.n_ag_elec;
    a->ae_type = et_type;
    a->ae_category = cat;
    a->ag_power = agp;
    a->ag_auto = agt;
    a->precision_ag = prg;
    a->year = year;
    a->active = 1;
    ags.total_ag_power += agp;
    ags.n_ag_elec++;
    print_str("[AGS] Ag elec "); print_int(ags.n_ag_elec - 1);
    print_str(" type="); print_int(et_type);
    print_str(" cat="); print_int(cat);
    print_str(" agp="); print_int(agp);
    print_str(" agt="); print_int(agt);
    print_str(" prg="); print_int(prg); print_str("\n");
    return ags.n_ag_elec - 1;
}

int ags_ag_ws(int wt_type, int cat, int fir, int fdr, int wcn, int year) {
    if (ags.n_ag_ws >= MAX_AG_WS) return -1;
    ag_ws_t* a = &ag_wss[ags.n_ag_ws];
    a->ws_id = ags.n_ag_ws;
    a->ws_type = wt_type;
    a->ws_category = cat;
    a->field_irr = fir;
    a->field_drn = fdr;
    a->water_cons = wcn;
    a->year = year;
    a->active = 1;
    ags.total_field_irr += fir;
    ags.n_ag_ws++;
    print_str("[AGS] Ag ws "); print_int(ags.n_ag_ws - 1);
    print_str(" type="); print_int(wt_type);
    print_str(" cat="); print_int(cat);
    print_str(" fir="); print_int(fir);
    print_str(" fdr="); print_int(fdr);
    print_str(" wcn="); print_int(wcn); print_str("\n");
    return ags.n_ag_ws - 1;
}

int ags_ag_bld(int bt_type, int cat, int agb, int grh, int clc, int year) {
    if (ags.n_ag_bld >= MAX_AG_BLD) return -1;
    ag_bld_t* a = &ag_blds[ags.n_ag_bld];
    a->ab_id = ags.n_ag_bld;
    a->ab_type = bt_type;
    a->ab_category = cat;
    a->ag_build = agb;
    a->greenhouse = grh;
    a->cold_chain = clc;
    a->year = year;
    a->active = 1;
    ags.total_ag_build += agb;
    ags.n_ag_bld++;
    print_str("[AGS] Ag bld "); print_int(ags.n_ag_bld - 1);
    print_str(" type="); print_int(bt_type);
    print_str(" cat="); print_int(cat);
    print_str(" agb="); print_int(agb);
    print_str(" grh="); print_int(grh);
    print_str(" clc="); print_int(clc); print_str("\n");
    return ags.n_ag_bld - 1;
}

int ags_ag_proc(int pt_type, int cat, int grp, int frp, int lvp, int year) {
    if (ags.n_ag_proc >= MAX_AG_PROC) return -1;
    ag_proc_t* a = &ag_procs[ags.n_ag_proc];
    a->ap_id = ags.n_ag_proc;
    a->ap_type = pt_type;
    a->ap_category = cat;
    a->grain_proc = grp;
    a->fruit_proc = frp;
    a->livestock_proc = lvp;
    a->year = year;
    a->active = 1;
    ags.total_grain_proc += grp;
    ags.n_ag_proc++;
    print_str("[AGS] Ag proc "); print_int(ags.n_ag_proc - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" grp="); print_int(grp);
    print_str(" frp="); print_int(frp);
    print_str(" lvp="); print_int(lvp); print_str("\n");
    return ags.n_ag_proc - 1;
}

void ags_ag_mech_report(void) {
    print_str("[AGS] Agricultural mech report:\n");
    print_str("  Mechanization categories: "); print_int(ags.n_ag_mech); print_str("\n");
    print_str("  Total tractors: "); print_int(ags.total_tractor); print_str("\n");
}

void ags_ag_elec_report(void) {
    print_str("[AGS] Agricultural elec report:\n");
    print_str("  Electrification categories: "); print_int(ags.n_ag_elec); print_str("\n");
    print_str("  Total ag power: "); print_int(ags.total_ag_power); print_str("\n");
}

void ags_full_report(void) {
    print_str("[AGS] Full report:\n");
    print_str("  Water/soil categories: "); print_int(ags.n_ag_ws); print_str("\n");
    print_str("  Total field irrigation: "); print_int(ags.total_field_irr); print_str("\n");
    print_str("  Building categories: "); print_int(ags.n_ag_bld); print_str("\n");
    print_str("  Total ag buildings: "); print_int(ags.total_ag_build); print_str("\n");
    print_str("  Processing categories: "); print_int(ags.n_ag_proc); print_str("\n");
    print_str("  Total grain processing: "); print_int(ags.total_grain_proc); print_str("\n");
}

void ags_print_state(void) {
    print_str("[AGS] M="); print_int(ags.n_ag_mech);
    print_str(" E="); print_int(ags.n_ag_elec);
    print_str(" W="); print_int(ags.n_ag_ws);
    print_str(" B="); print_int(ags.n_ag_bld);
    print_str(" P="); print_int(ags.n_ag_proc);
    print_str("\n");
}

int main(void) {
    print_str("=== Agricultural Engineering Admin Demo ===\n\n");
    ags_init();

    print_str("Mechanization...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int trc = 55 + (i * 13);
        int hrv = 40 + (i * 10);
        int plt = 22 + (i * 5);
        int year = 2020 + (i % 5);
        ags_ag_mech(type, cat, trc, hrv, plt, year);
    }

    print_str("\nElectrification...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int agp = 48 + (i * 11);
        int agt = 35 + (i * 8);
        int prg = 20 + (i * 4);
        int year = 2021 + (i % 4);
        ags_ag_elec(type, cat, agp, agt, prg, year);
    }

    print_str("\nWater/soil...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int fir = 42 + (i * 10);
        int fdr = 28 + (i * 7);
        int wcn = 18 + (i * 4);
        int year = 2022 + (i % 3);
        ags_ag_ws(type, cat, fir, fdr, wcn, year);
    }

    print_str("\nBuildings...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int agb = 35 + (i * 8);
        int grh = 25 + (i * 6);
        int clc = 15 + (i * 3);
        int year = 2023 + (i % 2);
        ags_ag_bld(type, cat, agb, grh, clc, year);
    }

    print_str("\nProcessing...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int grp = 30 + (i * 7);
        int frp = 22 + (i * 5);
        int lvp = 12 + (i * 3);
        int year = 2024;
        ags_ag_proc(type, cat, grp, frp, lvp, year);
    }

    print_str("\nMechanization report...\n");
    ags_ag_mech_report();

    print_str("\nElectrification report...\n");
    ags_ag_elec_report();

    print_str("\nFull report...\n");
    ags_full_report();

    print_str("\nFinal state...\n");
    ags_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
