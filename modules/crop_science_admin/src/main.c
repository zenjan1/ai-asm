/* crop_science_admin: Crop science administration system (v1.0)
 * Crop genetics/breeding, cultivation, soil science, plant protection, farming systems
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

#define MAX_CR_GENBR    16
#define MAX_CR_CULT     14
#define MAX_SOIL_SC     12
#define MAX_PL_PROT     10
#define MAX_FARM_SYS    10

typedef struct {
    int    cg_id;
    int    cg_type;
    int    cg_category;
    int    crop_gen;
    int    crop_brd;
    int    var_app;
    int    seed_prod;
    int    year;
    int    active;
} cr_genbr_t;

typedef struct {
    int    cc_id;
    int    cc_type;
    int    cc_category;
    int    grain_cul;
    int    cash_cul;
    int    veg_cul;
    int    fruit_cul;
    int    year;
    int    active;
} cr_cult_t;

typedef struct {
    int    ss_id;
    int    ss_type;
    int    ss_category;
    int    soil_phys;
    int    soil_chem;
    int    soil_bio;
    int    soil_fert;
    int    year;
    int    active;
} soil_sc_t;

typedef struct {
    int    pp_id;
    int    pp_type;
    int    pp_category;
    int    plant_path;
    int    ag_insect;
    int    pest_sc;
    int    int_ctrl;
    int    year;
    int    active;
} pl_prot_t;

typedef struct {
    int    fs_id;
    int    fs_type;
    int    fs_category;
    int    plant_sys;
    int    rot_sys;
    int    inter_crop;
    int    cons_till;
    int    year;
    int    active;
} farm_sys_t;

typedef struct {
    int    n_cr_genbr;
    int    n_cr_cult;
    int    n_soil_sc;
    int    n_pl_prot;
    int    n_farm_sys;
    int    total_crop_gen;
    int    total_grain_cul;
    int    total_soil_phys;
    int    total_plant_path;
    int    total_plant_sys;
} csa_state_t;

static cr_genbr_t cr_genbrs[MAX_CR_GENBR];
static cr_cult_t cr_cults[MAX_CR_CULT];
static soil_sc_t soil_scs[MAX_SOIL_SC];
static pl_prot_t pl_prots[MAX_PL_PROT];
static farm_sys_t farm_syss[MAX_FARM_SYS];
static csa_state_t csa;

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

int csa_init(void) {
    if (initialized) return -1;
    csa.n_cr_genbr = 0; csa.n_cr_cult = 0; csa.n_soil_sc = 0;
    csa.n_pl_prot = 0; csa.n_farm_sys = 0;
    csa.total_crop_gen = 0; csa.total_grain_cul = 0;
    csa.total_soil_phys = 0; csa.total_plant_path = 0;
    csa.total_plant_sys = 0;
    for (int i = 0; i < MAX_CR_GENBR; i++) cr_genbrs[i].active = 0;
    for (int i = 0; i < MAX_CR_CULT; i++) cr_cults[i].active = 0;
    for (int i = 0; i < MAX_SOIL_SC; i++) soil_scs[i].active = 0;
    for (int i = 0; i < MAX_PL_PROT; i++) pl_prots[i].active = 0;
    for (int i = 0; i < MAX_FARM_SYS; i++) farm_syss[i].active = 0;
    initialized = 1;
    print_str("[CSA] Crop science initialized\n");
    return 0;
}

int csa_cr_genbr(int gt_type, int cat, int cgn, int cbd, int vap, int spr, int year) {
    if (csa.n_cr_genbr >= MAX_CR_GENBR) return -1;
    cr_genbr_t* c = &cr_genbrs[csa.n_cr_genbr];
    c->cg_id = csa.n_cr_genbr;
    c->cg_type = gt_type;
    c->cg_category = cat;
    c->crop_gen = cgn;
    c->crop_brd = cbd;
    c->var_app = vap;
    c->seed_prod = spr;
    c->year = year;
    c->active = 1;
    csa.total_crop_gen += cgn;
    csa.n_cr_genbr++;
    print_str("[CSA] Cr genbr "); print_int(csa.n_cr_genbr - 1);
    print_str(" type="); print_int(gt_type);
    print_str(" cat="); print_int(cat);
    print_str(" cgn="); print_int(cgn);
    print_str(" cbd="); print_int(cbd);
    print_str(" vap="); print_int(vap);
    print_str(" spr="); print_int(spr); print_str("\n");
    return csa.n_cr_genbr - 1;
}

int csa_cr_cult(int ct_type, int cat, int grc, int csc, int vgc, int frc, int year) {
    if (csa.n_cr_cult >= MAX_CR_CULT) return -1;
    cr_cult_t* c = &cr_cults[csa.n_cr_cult];
    c->cc_id = csa.n_cr_cult;
    c->cc_type = ct_type;
    c->cc_category = cat;
    c->grain_cul = grc;
    c->cash_cul = csc;
    c->veg_cul = vgc;
    c->fruit_cul = frc;
    c->year = year;
    c->active = 1;
    csa.total_grain_cul += grc;
    csa.n_cr_cult++;
    print_str("[CSA] Cr cult "); print_int(csa.n_cr_cult - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" grc="); print_int(grc);
    print_str(" csc="); print_int(csc);
    print_str(" vgc="); print_int(vgc);
    print_str(" frc="); print_int(frc); print_str("\n");
    return csa.n_cr_cult - 1;
}

int csa_soil_sc(int st_type, int cat, int sph, int sch, int sbi, int sft, int year) {
    if (csa.n_soil_sc >= MAX_SOIL_SC) return -1;
    soil_sc_t* s = &soil_scs[csa.n_soil_sc];
    s->ss_id = csa.n_soil_sc;
    s->ss_type = st_type;
    s->ss_category = cat;
    s->soil_phys = sph;
    s->soil_chem = sch;
    s->soil_bio = sbi;
    s->soil_fert = sft;
    s->year = year;
    s->active = 1;
    csa.total_soil_phys += sph;
    csa.n_soil_sc++;
    print_str("[CSA] Soil sc "); print_int(csa.n_soil_sc - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" sph="); print_int(sph);
    print_str(" sch="); print_int(sch);
    print_str(" sbi="); print_int(sbi);
    print_str(" sft="); print_int(sft); print_str("\n");
    return csa.n_soil_sc - 1;
}

int csa_pl_prot(int pt_type, int cat, int pth, int agi, int pst, int icl, int year) {
    if (csa.n_pl_prot >= MAX_PL_PROT) return -1;
    pl_prot_t* p = &pl_prots[csa.n_pl_prot];
    p->pp_id = csa.n_pl_prot;
    p->pp_type = pt_type;
    p->pp_category = cat;
    p->plant_path = pth;
    p->ag_insect = agi;
    p->pest_sc = pst;
    p->int_ctrl = icl;
    p->year = year;
    p->active = 1;
    csa.total_plant_path += pth;
    csa.n_pl_prot++;
    print_str("[CSA] Pl prot "); print_int(csa.n_pl_prot - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" pth="); print_int(pth);
    print_str(" agi="); print_int(agi);
    print_str(" pst="); print_int(pst);
    print_str(" icl="); print_int(icl); print_str("\n");
    return csa.n_pl_prot - 1;
}

int csa_farm_sys(int ft_type, int cat, int pls, int rts, int icr, int cst, int year) {
    if (csa.n_farm_sys >= MAX_FARM_SYS) return -1;
    farm_sys_t* f = &farm_syss[csa.n_farm_sys];
    f->fs_id = csa.n_farm_sys;
    f->fs_type = ft_type;
    f->fs_category = cat;
    f->plant_sys = pls;
    f->rot_sys = rts;
    f->inter_crop = icr;
    f->cons_till = cst;
    f->year = year;
    f->active = 1;
    csa.total_plant_sys += pls;
    csa.n_farm_sys++;
    print_str("[CSA] Farm sys "); print_int(csa.n_farm_sys - 1);
    print_str(" type="); print_int(ft_type);
    print_str(" cat="); print_int(cat);
    print_str(" pls="); print_int(pls);
    print_str(" rts="); print_int(rts);
    print_str(" icr="); print_int(icr);
    print_str(" cst="); print_int(cst); print_str("\n");
    return csa.n_farm_sys - 1;
}

void csa_genbr_report(void) {
    print_str("[CSA] Crop genetics/breeding report:\n");
    print_str("  Genetics/breeding categories: "); print_int(csa.n_cr_genbr); print_str("\n");
    print_str("  Total crop genetics: "); print_int(csa.total_crop_gen); print_str("\n");
}

void csa_cult_report(void) {
    print_str("[CSA] Crop cultivation report:\n");
    print_str("  Cultivation categories: "); print_int(csa.n_cr_cult); print_str("\n");
    print_str("  Total grain cultivation: "); print_int(csa.total_grain_cul); print_str("\n");
}

void csa_full_report(void) {
    print_str("[CSA] Full report:\n");
    print_str("  Soil science categories: "); print_int(csa.n_soil_sc); print_str("\n");
    print_str("  Total soil physics: "); print_int(csa.total_soil_phys); print_str("\n");
    print_str("  Plant protection categories: "); print_int(csa.n_pl_prot); print_str("\n");
    print_str("  Total plant pathology: "); print_int(csa.total_plant_path); print_str("\n");
    print_str("  Farming system categories: "); print_int(csa.n_farm_sys); print_str("\n");
    print_str("  Total planting systems: "); print_int(csa.total_plant_sys); print_str("\n");
}

void csa_print_state(void) {
    print_str("[CSA] Gb="); print_int(csa.n_cr_genbr);
    print_str(" Cu="); print_int(csa.n_cr_cult);
    print_str(" So="); print_int(csa.n_soil_sc);
    print_str(" Pp="); print_int(csa.n_pl_prot);
    print_str(" Fs="); print_int(csa.n_farm_sys);
    print_str("\n");
}

int main(void) {
    print_str("=== Crop Science Admin Demo ===\n\n");
    csa_init();

    print_str("Crop genetics/breeding...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int cgn = 55 + (i * 13);
        int cbd = 40 + (i * 10);
        int vap = 22 + (i * 5);
        int spr = 15 + (i * 3);
        int year = 2020 + (i % 5);
        csa_cr_genbr(type, cat, cgn, cbd, vap, spr, year);
    }

    print_str("\nCrop cultivation...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int grc = 48 + (i * 11);
        int csc = 35 + (i * 8);
        int vgc = 20 + (i * 4);
        int frc = 12 + (i * 3);
        int year = 2021 + (i % 4);
        csa_cr_cult(type, cat, grc, csc, vgc, frc, year);
    }

    print_str("\nSoil science...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int sph = 42 + (i * 10);
        int sch = 28 + (i * 7);
        int sbi = 18 + (i * 4);
        int sft = 10 + (i * 2);
        int year = 2022 + (i % 3);
        csa_soil_sc(type, cat, sph, sch, sbi, sft, year);
    }

    print_str("\nPlant protection...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pth = 35 + (i * 8);
        int agi = 25 + (i * 6);
        int pst = 15 + (i * 3);
        int icl = 10 + (i * 2);
        int year = 2023 + (i % 2);
        csa_pl_prot(type, cat, pth, agi, pst, icl, year);
    }

    print_str("\nFarming systems...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pls = 30 + (i * 7);
        int rts = 22 + (i * 5);
        int icr = 12 + (i * 3);
        int cst = 8 + (i * 2);
        int year = 2024;
        csa_farm_sys(type, cat, pls, rts, icr, cst, year);
    }

    print_str("\nGenetics/breeding report...\n");
    csa_genbr_report();

    print_str("\n Cultivation report...\n");
    csa_cult_report();

    print_str("\nFull report...\n");
    csa_full_report();

    print_str("\nFinal state...\n");
    csa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
