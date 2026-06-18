/* fishery_science_admin: Fishery science administration system (v1.0)
 * Aquaculture, fishery resources, fishing engineering, aquatic processing, equipment
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

#define MAX_AQUA_CV     16
#define MAX_FISH_RES    14
#define MAX_FISH_EN     12
#define MAX_AQ_PROC     10
#define MAX_FISH_EQ     10

typedef struct {
    int    ac_id;
    int    ac_type;
    int    ac_category;
    int    fish_cv;
    int    shrimp_cv;
    int    shell_cv;
    int    year;
    int    active;
} aqua_cv_t;

typedef struct {
    int    fr_id;
    int    fr_type;
    int    fr_category;
    int    res_assess;
    int    res_mgmt;
    int    fish_ecol;
    int    year;
    int    active;
} fish_res_t;

typedef struct {
    int    fe_id;
    int    fe_type;
    int    fe_category;
    int    fish_tech;
    int    fish_gear;
    int    fish_vessel;
    int    year;
    int    active;
} fish_en_t;

typedef struct {
    int    ap_id;
    int    ap_type;
    int    ap_category;
    int    aq_fresh;
    int    aq_proc;
    int    fish_paste;
    int    year;
    int    active;
} aq_proc_t;

typedef struct {
    int    eq_id;
    int    eq_type;
    int    eq_category;
    int    aqua_eq;
    int    fish_eq;
    int    proc_eq;
    int    year;
    int    active;
} fish_eq_t;

typedef struct {
    int    n_aqua_cv;
    int    n_fish_res;
    int    n_fish_en;
    int    n_aq_proc;
    int    n_fish_eq;
    int    total_fish_cv;
    int    total_res_assess;
    int    total_fish_tech;
    int    total_aq_fresh;
    int    total_aqua_eq;
} fsa_state_t;

static aqua_cv_t aqua_cvs[MAX_AQUA_CV];
static fish_res_t fish_ress[MAX_FISH_RES];
static fish_en_t fish_ens[MAX_FISH_EN];
static aq_proc_t aq_procs[MAX_AQ_PROC];
static fish_eq_t fish_eqs[MAX_FISH_EQ];
static fsa_state_t fsa;

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

int fsa_init(void) {
    if (initialized) return -1;
    fsa.n_aqua_cv = 0; fsa.n_fish_res = 0; fsa.n_fish_en = 0;
    fsa.n_aq_proc = 0; fsa.n_fish_eq = 0;
    fsa.total_fish_cv = 0; fsa.total_res_assess = 0;
    fsa.total_fish_tech = 0; fsa.total_aq_fresh = 0;
    fsa.total_aqua_eq = 0;
    for (int i = 0; i < MAX_AQUA_CV; i++) aqua_cvs[i].active = 0;
    for (int i = 0; i < MAX_FISH_RES; i++) fish_ress[i].active = 0;
    for (int i = 0; i < MAX_FISH_EN; i++) fish_ens[i].active = 0;
    for (int i = 0; i < MAX_AQ_PROC; i++) aq_procs[i].active = 0;
    for (int i = 0; i < MAX_FISH_EQ; i++) fish_eqs[i].active = 0;
    initialized = 1;
    print_str("[FSA] Fishery science initialized\n");
    return 0;
}

int fsa_aqua_cv(int at_type, int cat, int fsh, int shp, int shl, int year) {
    if (fsa.n_aqua_cv >= MAX_AQUA_CV) return -1;
    aqua_cv_t* a = &aqua_cvs[fsa.n_aqua_cv];
    a->ac_id = fsa.n_aqua_cv;
    a->ac_type = at_type;
    a->ac_category = cat;
    a->fish_cv = fsh;
    a->shrimp_cv = shp;
    a->shell_cv = shl;
    a->year = year;
    a->active = 1;
    fsa.total_fish_cv += fsh;
    fsa.n_aqua_cv++;
    print_str("[FSA] Aqua cv "); print_int(fsa.n_aqua_cv - 1);
    print_str(" type="); print_int(at_type);
    print_str(" cat="); print_int(cat);
    print_str(" fsh="); print_int(fsh);
    print_str(" shp="); print_int(shp);
    print_str(" shl="); print_int(shl); print_str("\n");
    return fsa.n_aqua_cv - 1;
}

int fsa_fish_res(int rt_type, int cat, int ras, int rmg, int fec, int year) {
    if (fsa.n_fish_res >= MAX_FISH_RES) return -1;
    fish_res_t* f = &fish_ress[fsa.n_fish_res];
    f->fr_id = fsa.n_fish_res;
    f->fr_type = rt_type;
    f->fr_category = cat;
    f->res_assess = ras;
    f->res_mgmt = rmg;
    f->fish_ecol = fec;
    f->year = year;
    f->active = 1;
    fsa.total_res_assess += ras;
    fsa.n_fish_res++;
    print_str("[FSA] Fish res "); print_int(fsa.n_fish_res - 1);
    print_str(" type="); print_int(rt_type);
    print_str(" cat="); print_int(cat);
    print_str(" ras="); print_int(ras);
    print_str(" rmg="); print_int(rmg);
    print_str(" fec="); print_int(fec); print_str("\n");
    return fsa.n_fish_res - 1;
}

int fsa_fish_en(int ft_type, int cat, int fth, int fgr, int fvs, int year) {
    if (fsa.n_fish_en >= MAX_FISH_EN) return -1;
    fish_en_t* f = &fish_ens[fsa.n_fish_en];
    f->fe_id = fsa.n_fish_en;
    f->fe_type = ft_type;
    f->fe_category = cat;
    f->fish_tech = fth;
    f->fish_gear = fgr;
    f->fish_vessel = fvs;
    f->year = year;
    f->active = 1;
    fsa.total_fish_tech += fth;
    fsa.n_fish_en++;
    print_str("[FSA] Fish en "); print_int(fsa.n_fish_en - 1);
    print_str(" type="); print_int(ft_type);
    print_str(" cat="); print_int(cat);
    print_str(" fth="); print_int(fth);
    print_str(" fgr="); print_int(fgr);
    print_str(" fvs="); print_int(fvs); print_str("\n");
    return fsa.n_fish_en - 1;
}

int fsa_aq_proc(int pt_type, int cat, int afr, int aqp, int fsp, int year) {
    if (fsa.n_aq_proc >= MAX_AQ_PROC) return -1;
    aq_proc_t* a = &aq_procs[fsa.n_aq_proc];
    a->ap_id = fsa.n_aq_proc;
    a->ap_type = pt_type;
    a->ap_category = cat;
    a->aq_fresh = afr;
    a->aq_proc = aqp;
    a->fish_paste = fsp;
    a->year = year;
    a->active = 1;
    fsa.total_aq_fresh += afr;
    fsa.n_aq_proc++;
    print_str("[FSA] Aq proc "); print_int(fsa.n_aq_proc - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" afr="); print_int(afr);
    print_str(" aqp="); print_int(aqp);
    print_str(" fsp="); print_int(fsp); print_str("\n");
    return fsa.n_aq_proc - 1;
}

int fsa_fish_eq(int et_type, int cat, int aeq, int feq, int peq, int year) {
    if (fsa.n_fish_eq >= MAX_FISH_EQ) return -1;
    fish_eq_t* f = &fish_eqs[fsa.n_fish_eq];
    f->eq_id = fsa.n_fish_eq;
    f->eq_type = et_type;
    f->eq_category = cat;
    f->aqua_eq = aeq;
    f->fish_eq = feq;
    f->proc_eq = peq;
    f->year = year;
    f->active = 1;
    fsa.total_aqua_eq += aeq;
    fsa.n_fish_eq++;
    print_str("[FSA] Fish eq "); print_int(fsa.n_fish_eq - 1);
    print_str(" type="); print_int(et_type);
    print_str(" cat="); print_int(cat);
    print_str(" aeq="); print_int(aeq);
    print_str(" feq="); print_int(feq);
    print_str(" peq="); print_int(peq); print_str("\n");
    return fsa.n_fish_eq - 1;
}

void fsa_aqua_cv_report(void) {
    print_str("[FSA] Aquaculture report:\n");
    print_str("  Aquaculture categories: "); print_int(fsa.n_aqua_cv); print_str("\n");
    print_str("  Total fish culture: "); print_int(fsa.total_fish_cv); print_str("\n");
}

void fsa_fish_res_report(void) {
    print_str("[FSA] Fishery resources report:\n");
    print_str("  Resources categories: "); print_int(fsa.n_fish_res); print_str("\n");
    print_str("  Total resource assessment: "); print_int(fsa.total_res_assess); print_str("\n");
}

void fsa_full_report(void) {
    print_str("[FSA] Full report:\n");
    print_str("  Fishing engineering categories: "); print_int(fsa.n_fish_en); print_str("\n");
    print_str("  Total fishing tech: "); print_int(fsa.total_fish_tech); print_str("\n");
    print_str("  Aquatic processing categories: "); print_int(fsa.n_aq_proc); print_str("\n");
    print_str("  Total aquatic fresh: "); print_int(fsa.total_aq_fresh); print_str("\n");
    print_str("  Equipment categories: "); print_int(fsa.n_fish_eq); print_str("\n");
    print_str("  Total aquaculture equipment: "); print_int(fsa.total_aqua_eq); print_str("\n");
}

void fsa_print_state(void) {
    print_str("[FSA] Ac="); print_int(fsa.n_aqua_cv);
    print_str(" Fr="); print_int(fsa.n_fish_res);
    print_str(" Fe="); print_int(fsa.n_fish_en);
    print_str(" Ap="); print_int(fsa.n_aq_proc);
    print_str(" Eq="); print_int(fsa.n_fish_eq);
    print_str("\n");
}

int main(void) {
    print_str("=== Fishery Science Admin Demo ===\n\n");
    fsa_init();

    print_str("Aquaculture...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int fsh = 55 + (i * 13);
        int shp = 40 + (i * 10);
        int shl = 22 + (i * 5);
        int year = 2020 + (i % 5);
        fsa_aqua_cv(type, cat, fsh, shp, shl, year);
    }

    print_str("\nFishery resources...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ras = 48 + (i * 11);
        int rmg = 35 + (i * 8);
        int fec = 20 + (i * 4);
        int year = 2021 + (i % 4);
        fsa_fish_res(type, cat, ras, rmg, fec, year);
    }

    print_str("\nFishing engineering...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int fth = 42 + (i * 10);
        int fgr = 28 + (i * 7);
        int fvs = 18 + (i * 4);
        int year = 2022 + (i % 3);
        fsa_fish_en(type, cat, fth, fgr, fvs, year);
    }

    print_str("\nAquatic processing...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int afr = 35 + (i * 8);
        int aqp = 25 + (i * 6);
        int fsp = 15 + (i * 3);
        int year = 2023 + (i % 2);
        fsa_aq_proc(type, cat, afr, aqp, fsp, year);
    }

    print_str("\nEquipment...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int aeq = 30 + (i * 7);
        int feq = 22 + (i * 5);
        int peq = 12 + (i * 3);
        int year = 2024;
        fsa_fish_eq(type, cat, aeq, feq, peq, year);
    }

    print_str("\nAquaculture report...\n");
    fsa_aqua_cv_report();

    print_str("\nResources report...\n");
    fsa_fish_res_report();

    print_str("\nFull report...\n");
    fsa_full_report();

    print_str("\nFinal state...\n");
    fsa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
