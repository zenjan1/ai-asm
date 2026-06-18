/* mechanical_admin: Mechanical engineering administration system (v1.0)
 * Mechanical design, manufacturing, power engineering, vehicle engineering, aerospace
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

#define MAX_MECH_DES    16
#define MAX_MANU_ENG    14
#define MAX_POW_ENG     12
#define MAX_VEH_ENG     10
#define MAX_AER_ENG     10

typedef struct {
    int    md_id;
    int    md_type;
    int    md_category;
    int    mec_pri;
    int    mec_par;
    int    mec_vib;
    int    mec_opt;
    int    year;
    int    active;
} mech_des_t;

typedef struct {
    int    mf_id;
    int    mf_type;
    int    mf_category;
    int    man_pro;
    int    cnc_tec;
    int    int_man;
    int    pre_mach;
    int    year;
    int    active;
} manu_eng_t;

typedef struct {
    int    pe_id;
    int    pe_type;
    int    pe_category;
    int    ic_eng;
    int    stu_tur;
    int    boi_sys;
    int    the_eng;
    int    year;
    int    active;
} pow_eng_t;

typedef struct {
    int    ve_id;
    int    ve_type;
    int    ve_category;
    int    veh_des;
    int    veh_the;
    int    new_ene;
    int    int_con;
    int    year;
    int    active;
} veh_eng_t;

typedef struct {
    int    ae_id;
    int    ae_type;
    int    ae_category;
    int    fly_des;
    int    pro_the;
    int    aer_str;
    int    fly_con;
    int    year;
    int    active;
} aer_eng_t;

typedef struct {
    int    n_mech_des;
    int    n_manu_eng;
    int    n_pow_eng;
    int    n_veh_eng;
    int    n_aer_eng;
    int    total_mec_pri;
    int    total_man_pro;
    int    total_ic_eng;
    int    total_veh_des;
    int    total_fly_des;
} mca_state_t;

static mech_des_t mech_dess[MAX_MECH_DES];
static manu_eng_t manu_engs[MAX_MANU_ENG];
static pow_eng_t pow_engs[MAX_POW_ENG];
static veh_eng_t veh_engs[MAX_VEH_ENG];
static aer_eng_t aer_engs[MAX_AER_ENG];
static mca_state_t mca;

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

int mca_init(void) {
    if (initialized) return -1;
    mca.n_mech_des = 0; mca.n_manu_eng = 0; mca.n_pow_eng = 0;
    mca.n_veh_eng = 0; mca.n_aer_eng = 0;
    mca.total_mec_pri = 0; mca.total_man_pro = 0;
    mca.total_ic_eng = 0; mca.total_veh_des = 0;
    mca.total_fly_des = 0;
    for (int i = 0; i < MAX_MECH_DES; i++) mech_dess[i].active = 0;
    for (int i = 0; i < MAX_MANU_ENG; i++) manu_engs[i].active = 0;
    for (int i = 0; i < MAX_POW_ENG; i++) pow_engs[i].active = 0;
    for (int i = 0; i < MAX_VEH_ENG; i++) veh_engs[i].active = 0;
    for (int i = 0; i < MAX_AER_ENG; i++) aer_engs[i].active = 0;
    initialized = 1;
    print_str("[MCA] Mechanical initialized\n");
    return 0;
}

int mca_mech_des(int mt_type, int cat, int mpr, int mpa, int mvb, int mop, int year) {
    if (mca.n_mech_des >= MAX_MECH_DES) return -1;
    mech_des_t* m = &mech_dess[mca.n_mech_des];
    m->md_id = mca.n_mech_des;
    m->md_type = mt_type;
    m->md_category = cat;
    m->mec_pri = mpr;
    m->mec_par = mpa;
    m->mec_vib = mvb;
    m->mec_opt = mop;
    m->year = year;
    m->active = 1;
    mca.total_mec_pri += mpr;
    mca.n_mech_des++;
    print_str("[MCA] Mech des "); print_int(mca.n_mech_des - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" mpr="); print_int(mpr);
    print_str(" mpa="); print_int(mpa);
    print_str(" mvb="); print_int(mvb);
    print_str(" mop="); print_int(mop); print_str("\n");
    return mca.n_mech_des - 1;
}

int mca_manu_eng(int mt_type, int cat, int mpr, int cnt, int imn, int pcm, int year) {
    if (mca.n_manu_eng >= MAX_MANU_ENG) return -1;
    manu_eng_t* m = &manu_engs[mca.n_manu_eng];
    m->mf_id = mca.n_manu_eng;
    m->mf_type = mt_type;
    m->mf_category = cat;
    m->man_pro = mpr;
    m->cnc_tec = cnt;
    m->int_man = imn;
    m->pre_mach = pcm;
    m->year = year;
    m->active = 1;
    mca.total_man_pro += mpr;
    mca.n_manu_eng++;
    print_str("[MCA] Manu eng "); print_int(mca.n_manu_eng - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" mpr="); print_int(mpr);
    print_str(" cnt="); print_int(cnt);
    print_str(" imn="); print_int(imn);
    print_str(" pcm="); print_int(pcm); print_str("\n");
    return mca.n_manu_eng - 1;
}

int mca_pow_eng(int pt_type, int cat, int ice, int stt, int bos, int the, int year) {
    if (mca.n_pow_eng >= MAX_POW_ENG) return -1;
    pow_eng_t* p = &pow_engs[mca.n_pow_eng];
    p->pe_id = mca.n_pow_eng;
    p->pe_type = pt_type;
    p->pe_category = cat;
    p->ic_eng = ice;
    p->stu_tur = stt;
    p->boi_sys = bos;
    p->the_eng = the;
    p->year = year;
    p->active = 1;
    mca.total_ic_eng += ice;
    mca.n_pow_eng++;
    print_str("[MCA] Pow eng "); print_int(mca.n_pow_eng - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" ice="); print_int(ice);
    print_str(" stt="); print_int(stt);
    print_str(" bos="); print_int(bos);
    print_str(" the="); print_int(the); print_str("\n");
    return mca.n_pow_eng - 1;
}

int mca_veh_eng(int vt_type, int cat, int vds, int vth, int nen, int icn, int year) {
    if (mca.n_veh_eng >= MAX_VEH_ENG) return -1;
    veh_eng_t* v = &veh_engs[mca.n_veh_eng];
    v->ve_id = mca.n_veh_eng;
    v->ve_type = vt_type;
    v->ve_category = cat;
    v->veh_des = vds;
    v->veh_the = vth;
    v->new_ene = nen;
    v->int_con = icn;
    v->year = year;
    v->active = 1;
    mca.total_veh_des += vds;
    mca.n_veh_eng++;
    print_str("[MCA] Veh eng "); print_int(mca.n_veh_eng - 1);
    print_str(" type="); print_int(vt_type);
    print_str(" cat="); print_int(cat);
    print_str(" vds="); print_int(vds);
    print_str(" vth="); print_int(vth);
    print_str(" nen="); print_int(nen);
    print_str(" icn="); print_int(icn); print_str("\n");
    return mca.n_veh_eng - 1;
}

int mca_aer_eng(int at_type, int cat, int fds, int pth, int ast, int fcn, int year) {
    if (mca.n_aer_eng >= MAX_AER_ENG) return -1;
    aer_eng_t* a = &aer_engs[mca.n_aer_eng];
    a->ae_id = mca.n_aer_eng;
    a->ae_type = at_type;
    a->ae_category = cat;
    a->fly_des = fds;
    a->pro_the = pth;
    a->aer_str = ast;
    a->fly_con = fcn;
    a->year = year;
    a->active = 1;
    mca.total_fly_des += fds;
    mca.n_aer_eng++;
    print_str("[MCA] Aer eng "); print_int(mca.n_aer_eng - 1);
    print_str(" type="); print_int(at_type);
    print_str(" cat="); print_int(cat);
    print_str(" fds="); print_int(fds);
    print_str(" pth="); print_int(pth);
    print_str(" ast="); print_int(ast);
    print_str(" fcn="); print_int(fcn); print_str("\n");
    return mca.n_aer_eng - 1;
}

void mca_design_report(void) {
    print_str("[MCA] Mechanical design report:\n");
    print_str("  Design categories: "); print_int(mca.n_mech_des); print_str("\n");
    print_str("  Total mechanical principles: "); print_int(mca.total_mec_pri); print_str("\n");
}

void mca_manu_report(void) {
    print_str("[MCA] Manufacturing report:\n");
    print_str("  Manufacturing categories: "); print_int(mca.n_manu_eng); print_str("\n");
    print_str("  Total manufacturing processes: "); print_int(mca.total_man_pro); print_str("\n");
}

void mca_full_report(void) {
    print_str("[MCA] Full report:\n");
    print_str("  Power categories: "); print_int(mca.n_pow_eng); print_str("\n");
    print_str("  Total IC engines: "); print_int(mca.total_ic_eng); print_str("\n");
    print_str("  Vehicle categories: "); print_int(mca.n_veh_eng); print_str("\n");
    print_str("  Total vehicle designs: "); print_int(mca.total_veh_des); print_str("\n");
    print_str("  Aerospace categories: "); print_int(mca.n_aer_eng); print_str("\n");
    print_str("  Total flying designs: "); print_int(mca.total_fly_des); print_str("\n");
}

void mca_print_state(void) {
    print_str("[MCA] Md="); print_int(mca.n_mech_des);
    print_str(" Mf="); print_int(mca.n_manu_eng);
    print_str(" Pe="); print_int(mca.n_pow_eng);
    print_str(" Ve="); print_int(mca.n_veh_eng);
    print_str(" Ae="); print_int(mca.n_aer_eng);
    print_str("\n");
}

int main(void) {
    print_str("=== Mechanical Admin Demo ===\n\n");
    mca_init();

    print_str("Mechanical design...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int mpr = 55 + (i * 13);
        int mpa = 40 + (i * 10);
        int mvb = 22 + (i * 5);
        int mop = 15 + (i * 3);
        int year = 2020 + (i % 5);
        mca_mech_des(type, cat, mpr, mpa, mvb, mop, year);
    }

    print_str("\nManufacturing...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int mpr = 48 + (i * 11);
        int cnt = 35 + (i * 8);
        int imn = 20 + (i * 4);
        int pcm = 12 + (i * 3);
        int year = 2021 + (i % 4);
        mca_manu_eng(type, cat, mpr, cnt, imn, pcm, year);
    }

    print_str("\nPower engineering...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ice = 42 + (i * 10);
        int stt = 28 + (i * 7);
        int bos = 18 + (i * 4);
        int the = 10 + (i * 2);
        int year = 2022 + (i % 3);
        mca_pow_eng(type, cat, ice, stt, bos, the, year);
    }

    print_str("\nVehicle engineering...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int vds = 35 + (i * 8);
        int vth = 25 + (i * 6);
        int nen = 15 + (i * 3);
        int icn = 10 + (i * 2);
        int year = 2023 + (i % 2);
        mca_veh_eng(type, cat, vds, vth, nen, icn, year);
    }

    print_str("\nAerospace...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int fds = 30 + (i * 7);
        int pth = 22 + (i * 5);
        int ast = 12 + (i * 3);
        int fcn = 8 + (i * 2);
        int year = 2024;
        mca_aer_eng(type, cat, fds, pth, ast, fcn, year);
    }

    print_str("\nDesign report...\n");
    mca_design_report();

    print_str("\nManufacturing report...\n");
    mca_manu_report();

    print_str("\nFull report...\n");
    mca_full_report();

    print_str("\nFinal state...\n");
    mca_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
