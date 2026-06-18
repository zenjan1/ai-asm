/* food_science_engineering_admin: Food science and engineering administration system (v1.0)
 * Food processing, preservation, safety, nutrition, food engineering
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

#define MAX_FOOD_PR      16
#define MAX_FOOD_PS      14
#define MAX_FOOD_SF      12
#define MAX_FOOD_NT      10
#define MAX_FOOD_EN      10

typedef struct {
    int    fp_id;
    int    fp_type;
    int    fp_category;
    int    grain_proc;
    int    oil_proc;
    int    dairy_proc;
    int    year;
    int    active;
} food_pr_t;

typedef struct {
    int    fs_id;
    int    fs_type;
    int    fs_category;
    int    cold_stor;
    int    canning;
    int    drying;
    int    year;
    int    active;
} food_ps_t;

typedef struct {
    int    sf_id;
    int    sf_type;
    int    sf_category;
    int    food_test;
    int    food_micr;
    int    food_add;
    int    year;
    int    active;
} food_sf_t;

typedef struct {
    int    nt_id;
    int    nt_type;
    int    nt_category;
    int    nutr_an;
    int    func_food;
    int    nutr_fort;
    int    year;
    int    active;
} food_nt_t;

typedef struct {
    int    fe_id;
    int    fe_type;
    int    fe_category;
    int    food_mach;
    int    food_pack;
    int    food_fac;
    int    year;
    int    active;
} food_en_t;

typedef struct {
    int    n_food_pr;
    int    n_food_ps;
    int    n_food_sf;
    int    n_food_nt;
    int    n_food_en;
    int    total_grain_proc;
    int    total_cold_stor;
    int    total_food_test;
    int    total_nutr_an;
    int    total_food_mach;
} fsea_state_t;

static food_pr_t food_prs[MAX_FOOD_PR];
static food_ps_t food_pss[MAX_FOOD_PS];
static food_sf_t food_sfs[MAX_FOOD_SF];
static food_nt_t food_nts[MAX_FOOD_NT];
static food_en_t food_ens[MAX_FOOD_EN];
static fsea_state_t fsea;

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

int fsea_init(void) {
    if (initialized) return -1;
    fsea.n_food_pr = 0; fsea.n_food_ps = 0; fsea.n_food_sf = 0;
    fsea.n_food_nt = 0; fsea.n_food_en = 0;
    fsea.total_grain_proc = 0; fsea.total_cold_stor = 0;
    fsea.total_food_test = 0; fsea.total_nutr_an = 0;
    fsea.total_food_mach = 0;
    for (int i = 0; i < MAX_FOOD_PR; i++) food_prs[i].active = 0;
    for (int i = 0; i < MAX_FOOD_PS; i++) food_pss[i].active = 0;
    for (int i = 0; i < MAX_FOOD_SF; i++) food_sfs[i].active = 0;
    for (int i = 0; i < MAX_FOOD_NT; i++) food_nts[i].active = 0;
    for (int i = 0; i < MAX_FOOD_EN; i++) food_ens[i].active = 0;
    initialized = 1;
    print_str("[FSEA] Food science initialized\n");
    return 0;
}

int fsea_food_pr(int ft_type, int cat, int grp, int olp, int drp, int year) {
    if (fsea.n_food_pr >= MAX_FOOD_PR) return -1;
    food_pr_t* f = &food_prs[fsea.n_food_pr];
    f->fp_id = fsea.n_food_pr;
    f->fp_type = ft_type;
    f->fp_category = cat;
    f->grain_proc = grp;
    f->oil_proc = olp;
    f->dairy_proc = drp;
    f->year = year;
    f->active = 1;
    fsea.total_grain_proc += grp;
    fsea.n_food_pr++;
    print_str("[FSEA] Food pr "); print_int(fsea.n_food_pr - 1);
    print_str(" type="); print_int(ft_type);
    print_str(" cat="); print_int(cat);
    print_str(" grp="); print_int(grp);
    print_str(" olp="); print_int(olp);
    print_str(" drp="); print_int(drp); print_str("\n");
    return fsea.n_food_pr - 1;
}

int fsea_food_ps(int pt_type, int cat, int cds, int cnn, int drg, int year) {
    if (fsea.n_food_ps >= MAX_FOOD_PS) return -1;
    food_ps_t* f = &food_pss[fsea.n_food_ps];
    f->fs_id = fsea.n_food_ps;
    f->fs_type = pt_type;
    f->fs_category = cat;
    f->cold_stor = cds;
    f->canning = cnn;
    f->drying = drg;
    f->year = year;
    f->active = 1;
    fsea.total_cold_stor += cds;
    fsea.n_food_ps++;
    print_str("[FSEA] Food ps "); print_int(fsea.n_food_ps - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" cds="); print_int(cds);
    print_str(" cnn="); print_int(cnn);
    print_str(" drg="); print_int(drg); print_str("\n");
    return fsea.n_food_ps - 1;
}

int fsea_food_sf(int st_type, int cat, int fdt, int fmc, int fad, int year) {
    if (fsea.n_food_sf >= MAX_FOOD_SF) return -1;
    food_sf_t* s = &food_sfs[fsea.n_food_sf];
    s->sf_id = fsea.n_food_sf;
    s->sf_type = st_type;
    s->sf_category = cat;
    s->food_test = fdt;
    s->food_micr = fmc;
    s->food_add = fad;
    s->year = year;
    s->active = 1;
    fsea.total_food_test += fdt;
    fsea.n_food_sf++;
    print_str("[FSEA] Food sf "); print_int(fsea.n_food_sf - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" fdt="); print_int(fdt);
    print_str(" fmc="); print_int(fmc);
    print_str(" fad="); print_int(fad); print_str("\n");
    return fsea.n_food_sf - 1;
}

int fsea_food_nt(int nt_type, int cat, int nta, int fcf, int ntf, int year) {
    if (fsea.n_food_nt >= MAX_FOOD_NT) return -1;
    food_nt_t* n = &food_nts[fsea.n_food_nt];
    n->nt_id = fsea.n_food_nt;
    n->nt_type = nt_type;
    n->nt_category = cat;
    n->nutr_an = nta;
    n->func_food = fcf;
    n->nutr_fort = ntf;
    n->year = year;
    n->active = 1;
    fsea.total_nutr_an += nta;
    fsea.n_food_nt++;
    print_str("[FSEA] Food nt "); print_int(fsea.n_food_nt - 1);
    print_str(" type="); print_int(nt_type);
    print_str(" cat="); print_int(cat);
    print_str(" nta="); print_int(nta);
    print_str(" fcf="); print_int(fcf);
    print_str(" ntf="); print_int(ntf); print_str("\n");
    return fsea.n_food_nt - 1;
}

int fsea_food_en(int ft_type, int cat, int fmc_v, int fpk, int ffc, int year) {
    if (fsea.n_food_en >= MAX_FOOD_EN) return -1;
    food_en_t* f = &food_ens[fsea.n_food_en];
    f->fe_id = fsea.n_food_en;
    f->fe_type = ft_type;
    f->fe_category = cat;
    f->food_mach = fmc_v;
    f->food_pack = fpk;
    f->food_fac = ffc;
    f->year = year;
    f->active = 1;
    fsea.total_food_mach += fmc_v;
    fsea.n_food_en++;
    print_str("[FSEA] Food en "); print_int(fsea.n_food_en - 1);
    print_str(" type="); print_int(ft_type);
    print_str(" cat="); print_int(cat);
    print_str(" fmc="); print_int(fmc_v);
    print_str(" fpk="); print_int(fpk);
    print_str(" ffc="); print_int(ffc); print_str("\n");
    return fsea.n_food_en - 1;
}

void fsea_food_pr_report(void) {
    print_str("[FSEA] Food processing report:\n");
    print_str("  Food processing categories: "); print_int(fsea.n_food_pr); print_str("\n");
    print_str("  Total grain processing: "); print_int(fsea.total_grain_proc); print_str("\n");
}

void fsea_food_ps_report(void) {
    print_str("[FSEA] Food preservation report:\n");
    print_str("  Food preservation categories: "); print_int(fsea.n_food_ps); print_str("\n");
    print_str("  Total cold storage: "); print_int(fsea.total_cold_stor); print_str("\n");
}

void fsea_full_report(void) {
    print_str("[FSEA] Full report:\n");
    print_str("  Food safety categories: "); print_int(fsea.n_food_sf); print_str("\n");
    print_str("  Total food testing: "); print_int(fsea.total_food_test); print_str("\n");
    print_str("  Food nutrition categories: "); print_int(fsea.n_food_nt); print_str("\n");
    print_str("  Total nutrition analysis: "); print_int(fsea.total_nutr_an); print_str("\n");
    print_str("  Food engineering categories: "); print_int(fsea.n_food_en); print_str("\n");
    print_str("  Total food machinery: "); print_int(fsea.total_food_mach); print_str("\n");
}

void fsea_print_state(void) {
    print_str("[FSEA] Fp="); print_int(fsea.n_food_pr);
    print_str(" Ps="); print_int(fsea.n_food_ps);
    print_str(" Sf="); print_int(fsea.n_food_sf);
    print_str(" Nt="); print_int(fsea.n_food_nt);
    print_str(" Fe="); print_int(fsea.n_food_en);
    print_str("\n");
}

int main(void) {
    print_str("=== Food Science Engineering Admin Demo ===\n\n");
    fsea_init();

    print_str("Food processing...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int grp = 55 + (i * 13);
        int olp = 40 + (i * 10);
        int drp = 22 + (i * 5);
        int year = 2020 + (i % 5);
        fsea_food_pr(type, cat, grp, olp, drp, year);
    }

    print_str("\nFood preservation...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cds = 48 + (i * 11);
        int cnn = 35 + (i * 8);
        int drg = 20 + (i * 4);
        int year = 2021 + (i % 4);
        fsea_food_ps(type, cat, cds, cnn, drg, year);
    }

    print_str("\nFood safety...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int fdt = 42 + (i * 10);
        int fmc = 28 + (i * 7);
        int fad = 18 + (i * 4);
        int year = 2022 + (i % 3);
        fsea_food_sf(type, cat, fdt, fmc, fad, year);
    }

    print_str("\nFood nutrition...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int nta = 35 + (i * 8);
        int fcf = 25 + (i * 6);
        int ntf = 15 + (i * 3);
        int year = 2023 + (i % 2);
        fsea_food_nt(type, cat, nta, fcf, ntf, year);
    }

    print_str("\nFood engineering...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int fmc_v = 30 + (i * 7);
        int fpk = 22 + (i * 5);
        int ffc = 12 + (i * 3);
        int year = 2024;
        fsea_food_en(type, cat, fmc_v, fpk, ffc, year);
    }

    print_str("\nFood processing report...\n");
    fsea_food_pr_report();

    print_str("\nFood preservation report...\n");
    fsea_food_ps_report();

    print_str("\nFull report...\n");
    fsea_full_report();

    print_str("\nFinal state...\n");
    fsea_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
