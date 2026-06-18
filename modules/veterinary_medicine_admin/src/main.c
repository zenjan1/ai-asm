/* veterinary_medicine_admin: Veterinary medicine administration system (v1.0)
 * Anatomy, physiology, pathology, pharmacology, clinical science
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

#define MAX_VET_ANAT    16
#define MAX_VET_PHYS    14
#define MAX_VET_PATH    12
#define MAX_VET_PHARM   10
#define MAX_VET_CLIN    10

typedef struct {
    int    va_id;
    int    va_type;
    int    va_category;
    int    livestock_anat;
    int    poultry_anat;
    int    comp_anat;
    int    hist_emb;
    int    year;
    int    active;
} vet_anat_t;

typedef struct {
    int    vp_id;
    int    vp_type;
    int    vp_category;
    int    livestock_phys;
    int    poultry_phys;
    int    neuro_phys;
    int    endo_phys;
    int    year;
    int    active;
} vet_phys_t;

typedef struct {
    int    vpt_id;
    int    vpt_type;
    int    vpt_category;
    int    path_base;
    int    clin_path;
    int    immu_path;
    int    infect_path;
    int    year;
    int    active;
} vet_path_t;

typedef struct {
    int    vph_id;
    int    vph_type;
    int    vph_category;
    int    drug_meta;
    int    drug_eff;
    int    antibiotic;
    int    vet_prep;
    int    year;
    int    active;
} vet_pharm_t;

typedef struct {
    int    vc_id;
    int    vc_type;
    int    vc_category;
    int    intern_med;
    int    surgery;
    int    obstet;
    int    infect_dis;
    int    year;
    int    active;
} vet_clin_t;

typedef struct {
    int    n_vet_anat;
    int    n_vet_phys;
    int    n_vet_path;
    int    n_vet_pharm;
    int    n_vet_clin;
    int    total_livestock_anat;
    int    total_livestock_phys;
    int    total_path_base;
    int    total_drug_meta;
    int    total_intern_med;
} vma_state_t;

static vet_anat_t vet_anats[MAX_VET_ANAT];
static vet_phys_t vet_physs[MAX_VET_PHYS];
static vet_path_t vet_paths[MAX_VET_PATH];
static vet_pharm_t vet_pharms[MAX_VET_PHARM];
static vet_clin_t vet_clins[MAX_VET_CLIN];
static vma_state_t vma;

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

int vma_init(void) {
    if (initialized) return -1;
    vma.n_vet_anat = 0; vma.n_vet_phys = 0; vma.n_vet_path = 0;
    vma.n_vet_pharm = 0; vma.n_vet_clin = 0;
    vma.total_livestock_anat = 0; vma.total_livestock_phys = 0;
    vma.total_path_base = 0; vma.total_drug_meta = 0;
    vma.total_intern_med = 0;
    for (int i = 0; i < MAX_VET_ANAT; i++) vet_anats[i].active = 0;
    for (int i = 0; i < MAX_VET_PHYS; i++) vet_physs[i].active = 0;
    for (int i = 0; i < MAX_VET_PATH; i++) vet_paths[i].active = 0;
    for (int i = 0; i < MAX_VET_PHARM; i++) vet_pharms[i].active = 0;
    for (int i = 0; i < MAX_VET_CLIN; i++) vet_clins[i].active = 0;
    initialized = 1;
    print_str("[VMA] Veterinary medicine initialized\n");
    return 0;
}

int vma_vet_anat(int at_type, int cat, int lsk, int plt, int cmp, int hem, int year) {
    if (vma.n_vet_anat >= MAX_VET_ANAT) return -1;
    vet_anat_t* v = &vet_anats[vma.n_vet_anat];
    v->va_id = vma.n_vet_anat;
    v->va_type = at_type;
    v->va_category = cat;
    v->livestock_anat = lsk;
    v->poultry_anat = plt;
    v->comp_anat = cmp;
    v->hist_emb = hem;
    v->year = year;
    v->active = 1;
    vma.total_livestock_anat += lsk;
    vma.n_vet_anat++;
    print_str("[VMA] Vet anat "); print_int(vma.n_vet_anat - 1);
    print_str(" type="); print_int(at_type);
    print_str(" cat="); print_int(cat);
    print_str(" lsk="); print_int(lsk);
    print_str(" plt="); print_int(plt);
    print_str(" cmp="); print_int(cmp);
    print_str(" hem="); print_int(hem); print_str("\n");
    return vma.n_vet_anat - 1;
}

int vma_vet_phys(int pt_type, int cat, int lsk, int plt, int nro, int end, int year) {
    if (vma.n_vet_phys >= MAX_VET_PHYS) return -1;
    vet_phys_t* v = &vet_physs[vma.n_vet_phys];
    v->vp_id = vma.n_vet_phys;
    v->vp_type = pt_type;
    v->vp_category = cat;
    v->livestock_phys = lsk;
    v->poultry_phys = plt;
    v->neuro_phys = nro;
    v->endo_phys = end;
    v->year = year;
    v->active = 1;
    vma.total_livestock_phys += lsk;
    vma.n_vet_phys++;
    print_str("[VMA] Vet phys "); print_int(vma.n_vet_phys - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" lsk="); print_int(lsk);
    print_str(" plt="); print_int(plt);
    print_str(" nro="); print_int(nro);
    print_str(" end="); print_int(end); print_str("\n");
    return vma.n_vet_phys - 1;
}

int vma_vet_path(int pt_type, int cat, int pbs, int cpt, int imp, int ifp, int year) {
    if (vma.n_vet_path >= MAX_VET_PATH) return -1;
    vet_path_t* v = &vet_paths[vma.n_vet_path];
    v->vpt_id = vma.n_vet_path;
    v->vpt_type = pt_type;
    v->vpt_category = cat;
    v->path_base = pbs;
    v->clin_path = cpt;
    v->immu_path = imp;
    v->infect_path = ifp;
    v->year = year;
    v->active = 1;
    vma.total_path_base += pbs;
    vma.n_vet_path++;
    print_str("[VMA] Vet path "); print_int(vma.n_vet_path - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" pbs="); print_int(pbs);
    print_str(" cpt="); print_int(cpt);
    print_str(" imp="); print_int(imp);
    print_str(" ifp="); print_int(ifp); print_str("\n");
    return vma.n_vet_path - 1;
}

int vma_vet_pharm(int ht_type, int cat, int dma, int dfe, int abt, int vtp, int year) {
    if (vma.n_vet_pharm >= MAX_VET_PHARM) return -1;
    vet_pharm_t* v = &vet_pharms[vma.n_vet_pharm];
    v->vph_id = vma.n_vet_pharm;
    v->vph_type = ht_type;
    v->vph_category = cat;
    v->drug_meta = dma;
    v->drug_eff = dfe;
    v->antibiotic = abt;
    v->vet_prep = vtp;
    v->year = year;
    v->active = 1;
    vma.total_drug_meta += dma;
    vma.n_vet_pharm++;
    print_str("[VMA] Vet pharm "); print_int(vma.n_vet_pharm - 1);
    print_str(" type="); print_int(ht_type);
    print_str(" cat="); print_int(cat);
    print_str(" dma="); print_int(dma);
    print_str(" dfe="); print_int(dfe);
    print_str(" abt="); print_int(abt);
    print_str(" vtp="); print_int(vtp); print_str("\n");
    return vma.n_vet_pharm - 1;
}

int vma_vet_clin(int ct_type, int cat, int imd, int srg, int obt, int ifd, int year) {
    if (vma.n_vet_clin >= MAX_VET_CLIN) return -1;
    vet_clin_t* v = &vet_clins[vma.n_vet_clin];
    v->vc_id = vma.n_vet_clin;
    v->vc_type = ct_type;
    v->vc_category = cat;
    v->intern_med = imd;
    v->surgery = srg;
    v->obstet = obt;
    v->infect_dis = ifd;
    v->year = year;
    v->active = 1;
    vma.total_intern_med += imd;
    vma.n_vet_clin++;
    print_str("[VMA] Vet clin "); print_int(vma.n_vet_clin - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" imd="); print_int(imd);
    print_str(" srg="); print_int(srg);
    print_str(" obt="); print_int(obt);
    print_str(" ifd="); print_int(ifd); print_str("\n");
    return vma.n_vet_clin - 1;
}

void vma_anat_report(void) {
    print_str("[VMA] Anatomy report:\n");
    print_str("  Anatomy categories: "); print_int(vma.n_vet_anat); print_str("\n");
    print_str("  Total livestock anatomy: "); print_int(vma.total_livestock_anat); print_str("\n");
}

void vma_phys_report(void) {
    print_str("[VMA] Physiology report:\n");
    print_str("  Physiology categories: "); print_int(vma.n_vet_phys); print_str("\n");
    print_str("  Total livestock physiology: "); print_int(vma.total_livestock_phys); print_str("\n");
}

void vma_full_report(void) {
    print_str("[VMA] Full report:\n");
    print_str("  Pathology categories: "); print_int(vma.n_vet_path); print_str("\n");
    print_str("  Total pathology base: "); print_int(vma.total_path_base); print_str("\n");
    print_str("  Pharmacology categories: "); print_int(vma.n_vet_pharm); print_str("\n");
    print_str("  Total drug metabolism: "); print_int(vma.total_drug_meta); print_str("\n");
    print_str("  Clinical categories: "); print_int(vma.n_vet_clin); print_str("\n");
    print_str("  Total internal medicine: "); print_int(vma.total_intern_med); print_str("\n");
}

void vma_print_state(void) {
    print_str("[VMA] An="); print_int(vma.n_vet_anat);
    print_str(" Ph="); print_int(vma.n_vet_phys);
    print_str(" Pt="); print_int(vma.n_vet_path);
    print_str(" Pm="); print_int(vma.n_vet_pharm);
    print_str(" Cl="); print_int(vma.n_vet_clin);
    print_str("\n");
}

int main(void) {
    print_str("=== Veterinary Medicine Admin Demo ===\n\n");
    vma_init();

    print_str("Anatomy...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int lsk = 55 + (i * 13);
        int plt = 40 + (i * 10);
        int cmp = 22 + (i * 5);
        int hem = 15 + (i * 3);
        int year = 2020 + (i % 5);
        vma_vet_anat(type, cat, lsk, plt, cmp, hem, year);
    }

    print_str("\nPhysiology...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int lsk = 48 + (i * 11);
        int plt = 35 + (i * 8);
        int nro = 20 + (i * 4);
        int end = 12 + (i * 3);
        int year = 2021 + (i % 4);
        vma_vet_phys(type, cat, lsk, plt, nro, end, year);
    }

    print_str("\nPathology...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pbs = 42 + (i * 10);
        int cpt = 28 + (i * 7);
        int imp = 18 + (i * 4);
        int ifp = 10 + (i * 2);
        int year = 2022 + (i % 3);
        vma_vet_path(type, cat, pbs, cpt, imp, ifp, year);
    }

    print_str("\nPharmacology...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int dma = 35 + (i * 8);
        int dfe = 25 + (i * 6);
        int abt = 15 + (i * 3);
        int vtp = 10 + (i * 2);
        int year = 2023 + (i % 2);
        vma_vet_pharm(type, cat, dma, dfe, abt, vtp, year);
    }

    print_str("\nClinical...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int imd = 30 + (i * 7);
        int srg = 22 + (i * 5);
        int obt = 12 + (i * 3);
        int ifd = 8 + (i * 2);
        int year = 2024;
        vma_vet_clin(type, cat, imd, srg, obt, ifd, year);
    }

    print_str("\nAnatomy report...\n");
    vma_anat_report();

    print_str("\nPhysiology report...\n");
    vma_phys_report();

    print_str("\nFull report...\n");
    vma_full_report();

    print_str("\nFinal state...\n");
    vma_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
