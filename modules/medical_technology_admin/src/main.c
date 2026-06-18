/* medical_technology_admin: Medical technology administration system (v1.0)
 * Laboratory, imaging, rehabilitation, dental medicine, optometry
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

#define MAX_MED_LAB     16
#define MAX_MED_IMG     14
#define MAX_REHAB       12
#define MAX_DENTAL      10
#define MAX_OPTOM       10

typedef struct {
    int    ml_id;
    int    ml_type;
    int    ml_category;
    int    clin_lab;
    int    biochem_lab;
    int    immuno_lab;
    int    micro_lab;
    int    year;
    int    active;
} med_lab_t;

typedef struct {
    int    mi_id;
    int    mi_type;
    int    mi_category;
    int    xray_diag;
    int    ct_diag;
    int    mri_diag;
    int    us_diag;
    int    year;
    int    active;
} med_img_t;

typedef struct {
    int    rh_id;
    int    rh_type;
    int    rh_category;
    int    phys_th;
    int    occ_th;
    int    speech_th;
    int    reh_eng;
    int    year;
    int    active;
} rehab_t;

typedef struct {
    int    dt_id;
    int    dt_type;
    int    dt_category;
    int    oral_int;
    int    oral_srg;
    int    oral_pro;
    int    ortho_d;
    int    year;
    int    active;
} dental_t;

typedef struct {
    int    om_id;
    int    om_type;
    int    om_category;
    int    eye_exam;
    int    refrac;
    int    vis_train;
    int    lens_fit;
    int    year;
    int    active;
} optom_t;

typedef struct {
    int    n_med_lab;
    int    n_med_img;
    int    n_rehab;
    int    n_dental;
    int    n_optom;
    int    total_clin_lab;
    int    total_xray_diag;
    int    total_phys_th;
    int    total_oral_int;
    int    total_eye_exam;
} mta_state_t;

static med_lab_t med_labs[MAX_MED_LAB];
static med_img_t med_imgs[MAX_MED_IMG];
static rehab_t rehabs[MAX_REHAB];
static dental_t dentals[MAX_DENTAL];
static optom_t optoms[MAX_OPTOM];
static mta_state_t mta;

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

int mta_init(void) {
    if (initialized) return -1;
    mta.n_med_lab = 0; mta.n_med_img = 0; mta.n_rehab = 0;
    mta.n_dental = 0; mta.n_optom = 0;
    mta.total_clin_lab = 0; mta.total_xray_diag = 0;
    mta.total_phys_th = 0; mta.total_oral_int = 0;
    mta.total_eye_exam = 0;
    for (int i = 0; i < MAX_MED_LAB; i++) med_labs[i].active = 0;
    for (int i = 0; i < MAX_MED_IMG; i++) med_imgs[i].active = 0;
    for (int i = 0; i < MAX_REHAB; i++) rehabs[i].active = 0;
    for (int i = 0; i < MAX_DENTAL; i++) dentals[i].active = 0;
    for (int i = 0; i < MAX_OPTOM; i++) optoms[i].active = 0;
    initialized = 1;
    print_str("[MTA] Medical technology initialized\n");
    return 0;
}

int mta_med_lab(int lt_type, int cat, int clr, int bcl, int iml, int mcl, int year) {
    if (mta.n_med_lab >= MAX_MED_LAB) return -1;
    med_lab_t* m = &med_labs[mta.n_med_lab];
    m->ml_id = mta.n_med_lab;
    m->ml_type = lt_type;
    m->ml_category = cat;
    m->clin_lab = clr;
    m->biochem_lab = bcl;
    m->immuno_lab = iml;
    m->micro_lab = mcl;
    m->year = year;
    m->active = 1;
    mta.total_clin_lab += clr;
    mta.n_med_lab++;
    print_str("[MTA] Med lab "); print_int(mta.n_med_lab - 1);
    print_str(" type="); print_int(lt_type);
    print_str(" cat="); print_int(cat);
    print_str(" clr="); print_int(clr);
    print_str(" bcl="); print_int(bcl);
    print_str(" iml="); print_int(iml);
    print_str(" mcl="); print_int(mcl); print_str("\n");
    return mta.n_med_lab - 1;
}

int mta_med_img(int it_type, int cat, int xrd, int ctd, int mrd, int usd, int year) {
    if (mta.n_med_img >= MAX_MED_IMG) return -1;
    med_img_t* m = &med_imgs[mta.n_med_img];
    m->mi_id = mta.n_med_img;
    m->mi_type = it_type;
    m->mi_category = cat;
    m->xray_diag = xrd;
    m->ct_diag = ctd;
    m->mri_diag = mrd;
    m->us_diag = usd;
    m->year = year;
    m->active = 1;
    mta.total_xray_diag += xrd;
    mta.n_med_img++;
    print_str("[MTA] Med img "); print_int(mta.n_med_img - 1);
    print_str(" type="); print_int(it_type);
    print_str(" cat="); print_int(cat);
    print_str(" xrd="); print_int(xrd);
    print_str(" ctd="); print_int(ctd);
    print_str(" mrd="); print_int(mrd);
    print_str(" usd="); print_int(usd); print_str("\n");
    return mta.n_med_img - 1;
}

int mta_rehab(int rt_type, int cat, int pth, int oct, int spt, int ren, int year) {
    if (mta.n_rehab >= MAX_REHAB) return -1;
    rehab_t* r = &rehabs[mta.n_rehab];
    r->rh_id = mta.n_rehab;
    r->rh_type = rt_type;
    r->rh_category = cat;
    r->phys_th = pth;
    r->occ_th = oct;
    r->speech_th = spt;
    r->reh_eng = ren;
    r->year = year;
    r->active = 1;
    mta.total_phys_th += pth;
    mta.n_rehab++;
    print_str("[MTA] Rehab "); print_int(mta.n_rehab - 1);
    print_str(" type="); print_int(rt_type);
    print_str(" cat="); print_int(cat);
    print_str(" pth="); print_int(pth);
    print_str(" oct="); print_int(oct);
    print_str(" spt="); print_int(spt);
    print_str(" ren="); print_int(ren); print_str("\n");
    return mta.n_rehab - 1;
}

int mta_dental(int dt_type, int cat, int ori, int ors, int orp, int ort, int year) {
    if (mta.n_dental >= MAX_DENTAL) return -1;
    dental_t* d = &dentals[mta.n_dental];
    d->dt_id = mta.n_dental;
    d->dt_type = dt_type;
    d->dt_category = cat;
    d->oral_int = ori;
    d->oral_srg = ors;
    d->oral_pro = orp;
    d->ortho_d = ort;
    d->year = year;
    d->active = 1;
    mta.total_oral_int += ori;
    mta.n_dental++;
    print_str("[MTA] Dental "); print_int(mta.n_dental - 1);
    print_str(" type="); print_int(dt_type);
    print_str(" cat="); print_int(cat);
    print_str(" ori="); print_int(ori);
    print_str(" ors="); print_int(ors);
    print_str(" orp="); print_int(orp);
    print_str(" ort="); print_int(ort); print_str("\n");
    return mta.n_dental - 1;
}

int mta_optom(int ot_type, int cat, int eyx, int rfr, int vst, int lsf, int year) {
    if (mta.n_optom >= MAX_OPTOM) return -1;
    optom_t* o = &optoms[mta.n_optom];
    o->om_id = mta.n_optom;
    o->om_type = ot_type;
    o->om_category = cat;
    o->eye_exam = eyx;
    o->refrac = rfr;
    o->vis_train = vst;
    o->lens_fit = lsf;
    o->year = year;
    o->active = 1;
    mta.total_eye_exam += eyx;
    mta.n_optom++;
    print_str("[MTA] Optom "); print_int(mta.n_optom - 1);
    print_str(" type="); print_int(ot_type);
    print_str(" cat="); print_int(cat);
    print_str(" eyx="); print_int(eyx);
    print_str(" rfr="); print_int(rfr);
    print_str(" vst="); print_int(vst);
    print_str(" lsf="); print_int(lsf); print_str("\n");
    return mta.n_optom - 1;
}

void mta_lab_report(void) {
    print_str("[MTA] Laboratory report:\n");
    print_str("  Laboratory categories: "); print_int(mta.n_med_lab); print_str("\n");
    print_str("  Total clinical lab: "); print_int(mta.total_clin_lab); print_str("\n");
}

void mta_img_report(void) {
    print_str("[MTA] Imaging report:\n");
    print_str("  Imaging categories: "); print_int(mta.n_med_img); print_str("\n");
    print_str("  Total X-ray diagnosis: "); print_int(mta.total_xray_diag); print_str("\n");
}

void mta_full_report(void) {
    print_str("[MTA] Full report:\n");
    print_str("  Rehabilitation categories: "); print_int(mta.n_rehab); print_str("\n");
    print_str("  Total physical therapy: "); print_int(mta.total_phys_th); print_str("\n");
    print_str("  Dental categories: "); print_int(mta.n_dental); print_str("\n");
    print_str("  Total oral internal: "); print_int(mta.total_oral_int); print_str("\n");
    print_str("  Optometry categories: "); print_int(mta.n_optom); print_str("\n");
    print_str("  Total eye exams: "); print_int(mta.total_eye_exam); print_str("\n");
}

void mta_print_state(void) {
    print_str("[MTA] Lb="); print_int(mta.n_med_lab);
    print_str(" Im="); print_int(mta.n_med_img);
    print_str(" Rh="); print_int(mta.n_rehab);
    print_str(" Dt="); print_int(mta.n_dental);
    print_str(" Om="); print_int(mta.n_optom);
    print_str("\n");
}

int main(void) {
    print_str("=== Medical Technology Admin Demo ===\n\n");
    mta_init();

    print_str("Medical laboratory...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int clr = 55 + (i * 13);
        int bcl = 40 + (i * 10);
        int iml = 22 + (i * 5);
        int mcl = 15 + (i * 3);
        int year = 2020 + (i % 5);
        mta_med_lab(type, cat, clr, bcl, iml, mcl, year);
    }

    print_str("\nMedical imaging...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int xrd = 48 + (i * 11);
        int ctd = 35 + (i * 8);
        int mrd = 20 + (i * 4);
        int usd = 12 + (i * 3);
        int year = 2021 + (i % 4);
        mta_med_img(type, cat, xrd, ctd, mrd, usd, year);
    }

    print_str("\nRehabilitation...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pth = 42 + (i * 10);
        int oct = 28 + (i * 7);
        int spt = 18 + (i * 4);
        int ren = 10 + (i * 2);
        int year = 2022 + (i % 3);
        mta_rehab(type, cat, pth, oct, spt, ren, year);
    }

    print_str("\nDental...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ori = 35 + (i * 8);
        int ors = 25 + (i * 6);
        int orp = 15 + (i * 3);
        int ort = 10 + (i * 2);
        int year = 2023 + (i % 2);
        mta_dental(type, cat, ori, ors, orp, ort, year);
    }

    print_str("\nOptometry...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int eyx = 30 + (i * 7);
        int rfr = 22 + (i * 5);
        int vst = 12 + (i * 3);
        int lsf = 8 + (i * 2);
        int year = 2024;
        mta_optom(type, cat, eyx, rfr, vst, lsf, year);
    }

    print_str("\nLaboratory report...\n");
    mta_lab_report();

    print_str("\nImaging report...\n");
    mta_img_report();

    print_str("\nFull report...\n");
    mta_full_report();

    print_str("\nFinal state...\n");
    mta_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
