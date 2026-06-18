/* metamaterial_admin: Metamaterial administration system (v1.0)
 * EM metamaterials, acoustic, mechanical, thermal, smart metamaterials
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

#define MAX_EM         16
#define MAX_ACOUSTIC   14
#define MAX_MECH       12
#define MAX_THERMAL    10
#define MAX_SMART      10

typedef struct {
    int    em_id;
    int    em_type;
    int    em_category;
    int    neg_refract;
    int    cloaking;
    int    perfect_absorb;
    int    year;
    int    active;
} em_t;

typedef struct {
    int    ac_id;
    int    ac_type;
    int    ac_category;
    int    phononic;
    int    ac_cloak;
    int    ac_superlens;
    int    year;
    int    active;
} acoustic_t;

typedef struct {
    int    me_id;
    int    me_type;
    int    me_category;
    int    neg_poisson;
    int    programmable;
    int    ultra_light;
    int    year;
    int    active;
} mech_t;

typedef struct {
    int    th_id;
    int    th_type;
    int    th_category;
    int    th_cloak;
    int    th_concent;
    int    th_diode;
    int    year;
    int    active;
} thermal_t;

typedef struct {
    int    sm_id;
    int    sm_type;
    int    sm_category;
    int    tunable;
    int    adaptive;
    int    four_d_print;
    int    year;
    int    active;
} smart_t;

typedef struct {
    int    n_em;
    int    n_acoustic;
    int    n_mech;
    int    n_thermal;
    int    n_smart;
    int    total_neg_refract;
    int    total_phononic;
    int    total_neg_poisson;
    int    total_th_cloak;
    int    total_tunable;
} mms_state_t;

static em_t ems[MAX_EM];
static acoustic_t acoustics[MAX_ACOUSTIC];
static mech_t mechs[MAX_MECH];
static thermal_t thermals[MAX_THERMAL];
static smart_t smarts[MAX_SMART];
static mms_state_t mms;

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

int mms_init(void) {
    if (initialized) return -1;
    mms.n_em = 0; mms.n_acoustic = 0; mms.n_mech = 0;
    mms.n_thermal = 0; mms.n_smart = 0;
    mms.total_neg_refract = 0; mms.total_phononic = 0;
    mms.total_neg_poisson = 0; mms.total_th_cloak = 0;
    mms.total_tunable = 0;
    for (int i = 0; i < MAX_EM; i++) ems[i].active = 0;
    for (int i = 0; i < MAX_ACOUSTIC; i++) acoustics[i].active = 0;
    for (int i = 0; i < MAX_MECH; i++) mechs[i].active = 0;
    for (int i = 0; i < MAX_THERMAL; i++) thermals[i].active = 0;
    for (int i = 0; i < MAX_SMART; i++) smarts[i].active = 0;
    initialized = 1;
    print_str("[MMS] Metamaterial initialized\n");
    return 0;
}

int mms_em(int e_type, int cat, int neg_r, int cloak, int absorb_v, int year) {
    if (mms.n_em >= MAX_EM) return -1;
    em_t* e = &ems[mms.n_em];
    e->em_id = mms.n_em;
    e->em_type = e_type;
    e->em_category = cat;
    e->neg_refract = neg_r;
    e->cloaking = cloak;
    e->perfect_absorb = absorb_v;
    e->year = year;
    e->active = 1;
    mms.total_neg_refract += neg_r;
    mms.n_em++;
    print_str("[MMS] EM "); print_int(mms.n_em - 1);
    print_str(" type="); print_int(e_type);
    print_str(" cat="); print_int(cat);
    print_str(" ngr="); print_int(neg_r);
    print_str(" clk="); print_int(cloak);
    print_str(" abs="); print_int(absorb_v); print_str("\n");
    return mms.n_em - 1;
}

int mms_acoustic(int a_type, int cat, int phono, int aclk, int asupe, int year) {
    if (mms.n_acoustic >= MAX_ACOUSTIC) return -1;
    acoustic_t* a = &acoustics[mms.n_acoustic];
    a->ac_id = mms.n_acoustic;
    a->ac_type = a_type;
    a->ac_category = cat;
    a->phononic = phono;
    a->ac_cloak = aclk;
    a->ac_superlens = asupe;
    a->year = year;
    a->active = 1;
    mms.total_phononic += phono;
    mms.n_acoustic++;
    print_str("[MMS] Acoustic "); print_int(mms.n_acoustic - 1);
    print_str(" type="); print_int(a_type);
    print_str(" cat="); print_int(cat);
    print_str(" phn="); print_int(phono);
    print_str(" ack="); print_int(aclk);
    print_str(" asl="); print_int(asupe); print_str("\n");
    return mms.n_acoustic - 1;
}

int mms_mech(int m_type, int cat, int neg_p, int programm, int ulight, int year) {
    if (mms.n_mech >= MAX_MECH) return -1;
    mech_t* m = &mechs[mms.n_mech];
    m->me_id = mms.n_mech;
    m->me_type = m_type;
    m->me_category = cat;
    m->neg_poisson = neg_p;
    m->programmable = programm;
    m->ultra_light = ulight;
    m->year = year;
    m->active = 1;
    mms.total_neg_poisson += neg_p;
    mms.n_mech++;
    print_str("[MMS] Mech "); print_int(mms.n_mech - 1);
    print_str(" type="); print_int(m_type);
    print_str(" cat="); print_int(cat);
    print_str(" nps="); print_int(neg_p);
    print_str(" prg="); print_int(programm);
    print_str(" ult="); print_int(ulight); print_str("\n");
    return mms.n_mech - 1;
}

int mms_thermal(int t_type, int cat, int tclk, int tconc, int tdiode, int year) {
    if (mms.n_thermal >= MAX_THERMAL) return -1;
    thermal_t* t = &thermals[mms.n_thermal];
    t->th_id = mms.n_thermal;
    t->th_type = t_type;
    t->th_category = cat;
    t->th_cloak = tclk;
    t->th_concent = tconc;
    t->th_diode = tdiode;
    t->year = year;
    t->active = 1;
    mms.total_th_cloak += tclk;
    mms.n_thermal++;
    print_str("[MMS] Thermal "); print_int(mms.n_thermal - 1);
    print_str(" type="); print_int(t_type);
    print_str(" cat="); print_int(cat);
    print_str(" tck="); print_int(tclk);
    print_str(" tcn="); print_int(tconc);
    print_str(" tdi="); print_int(tdiode); print_str("\n");
    return mms.n_thermal - 1;
}

int mms_smart(int s_type, int cat, int tunab, int adapt_v, int fourd, int year) {
    if (mms.n_smart >= MAX_SMART) return -1;
    smart_t* s = &smarts[mms.n_smart];
    s->sm_id = mms.n_smart;
    s->sm_type = s_type;
    s->sm_category = cat;
    s->tunable = tunab;
    s->adaptive = adapt_v;
    s->four_d_print = fourd;
    s->year = year;
    s->active = 1;
    mms.total_tunable += tunab;
    mms.n_smart++;
    print_str("[MMS] Smart "); print_int(mms.n_smart - 1);
    print_str(" type="); print_int(s_type);
    print_str(" cat="); print_int(cat);
    print_str(" tun="); print_int(tunab);
    print_str(" adp="); print_int(adapt_v);
    print_str(" frd="); print_int(fourd); print_str("\n");
    return mms.n_smart - 1;
}

void mms_em_report(void) {
    print_str("[MMS] EM report:\n");
    print_str("  EM metamaterial categories: "); print_int(mms.n_em); print_str("\n");
    print_str("  Total neg refractive index: "); print_int(mms.total_neg_refract); print_str("\n");
}

void mms_acoustic_report(void) {
    print_str("[MMS] Acoustic report:\n");
    print_str("  Acoustic metamaterial categories: "); print_int(mms.n_acoustic); print_str("\n");
    print_str("  Total phononic crystals: "); print_int(mms.total_phononic); print_str("\n");
}

void mms_smart_report(void) {
    print_str("[MMS] Smart report:\n");
    print_str("  Mechanical metamaterial categories: "); print_int(mms.n_mech); print_str("\n");
    print_str("  Total neg Poisson ratio: "); print_int(mms.total_neg_poisson); print_str("\n");
    print_str("  Thermal metamaterial categories: "); print_int(mms.n_thermal); print_str("\n");
    print_str("  Total thermal cloaking: "); print_int(mms.total_th_cloak); print_str("\n");
    print_str("  Smart metamaterial categories: "); print_int(mms.n_smart); print_str("\n");
    print_str("  Total tunable metamaterial: "); print_int(mms.total_tunable); print_str("\n");
}

void mms_print_state(void) {
    print_str("[MMS] Em="); print_int(mms.n_em);
    print_str(" Ac="); print_int(mms.n_acoustic);
    print_str(" Me="); print_int(mms.n_mech);
    print_str(" Th="); print_int(mms.n_thermal);
    print_str(" Sm="); print_int(mms.n_smart);
    print_str("\n");
}

int main(void) {
    print_str("=== Metamaterial Admin Demo ===\n\n");
    mms_init();

    print_str("EM metamaterials...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int ngr = 55 + (i * 13);
        int clk = 40 + (i * 10);
        int abs = 22 + (i * 5);
        int year = 2020 + (i % 5);
        mms_em(type, cat, ngr, clk, abs, year);
    }

    print_str("\nAcoustic metamaterials...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int phn = 48 + (i * 12);
        int ack = 35 + (i * 8);
        int asl = 20 + (i * 5);
        int year = 2021 + (i % 4);
        mms_acoustic(type, cat, phn, ack, asl, year);
    }

    print_str("\nMechanical metamaterials...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int nps = 42 + (i * 10);
        int prg = 28 + (i * 7);
        int ult = 18 + (i * 4);
        int year = 2022 + (i % 3);
        mms_mech(type, cat, nps, prg, ult, year);
    }

    print_str("\nThermal metamaterials...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int tck = 35 + (i * 8);
        int tcn = 25 + (i * 6);
        int tdi = 15 + (i * 3);
        int year = 2023 + (i % 2);
        mms_thermal(type, cat, tck, tcn, tdi, year);
    }

    print_str("\nSmart metamaterials...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int tun = 30 + (i * 7);
        int adp = 22 + (i * 5);
        int frd = 12 + (i * 3);
        int year = 2024;
        mms_smart(type, cat, tun, adp, frd, year);
    }

    print_str("\nEM report...\n");
    mms_em_report();

    print_str("\nAcoustic report...\n");
    mms_acoustic_report();

    print_str("\nSmart report...\n");
    mms_smart_report();

    print_str("\nFinal state...\n");
    mms_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
