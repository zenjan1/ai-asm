/* permanent_magnet_admin: Permanent magnet administration system (v1.0)
 * NdFeB, SmCo, AlNiCo, ferrite PM, PM applications
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

#define MAX_NDFEB      16
#define MAX_SMCO       14
#define MAX_ALNICO     12
#define MAX_FERRITE_PM 10
#define MAX_APP_PM     10

typedef struct {
    int    ndfeb_id;
    int    ndfeb_type;
    int    ndfeb_category;
    int    sintered;
    int    bonded;
    int    hot_press;
    int    year;
    int    active;
} ndfeb_t;

typedef struct {
    int    smco_id;
    int    smco_type;
    int    smco_category;
    int    smco5;
    int    sm2co17;
    int    smco_custom;
    int    year;
    int    active;
} smco_t;

typedef struct {
    int    alnico_id;
    int    alnico_type;
    int    alnico_category;
    int    cast_alnico;
    int    sinter_alnico;
    int    bonded_alnico;
    int    year;
    int    active;
} alnico_t;

typedef struct {
    int    fpm_id;
    int    fpm_type;
    int    fpm_category;
    int    aniso_fer;
    int    iso_fer;
    int    custom_fer;
    int    year;
    int    active;
} fpm_t;

typedef struct {
    int    app_id;
    int    app_type;
    int    app_category;
    int    motor_magnet;
    int    speaker_magnet;
    int    mri_magnet;
    int    year;
    int    active;
} app_pm_t;

typedef struct {
    int    n_ndfeb;
    int    n_smco;
    int    n_alnico;
    int    n_fpm;
    int    n_app_pm;
    int    total_sintered;
    int    total_smco5;
    int    total_cast;
    int    total_aniso;
    int    total_motor;
} pma_state_t;

static ndfeb_t ndfebs[MAX_NDFEB];
static smco_t smcos[MAX_SMCO];
static alnico_t alnicos[MAX_ALNICO];
static fpm_t fpms[MAX_FERRITE_PM];
static app_pm_t app_pms[MAX_APP_PM];
static pma_state_t pma;

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

int pma_init(void) {
    if (initialized) return -1;
    pma.n_ndfeb = 0; pma.n_smco = 0; pma.n_alnico = 0;
    pma.n_fpm = 0; pma.n_app_pm = 0;
    pma.total_sintered = 0; pma.total_smco5 = 0;
    pma.total_cast = 0; pma.total_aniso = 0;
    pma.total_motor = 0;
    for (int i = 0; i < MAX_NDFEB; i++) ndfebs[i].active = 0;
    for (int i = 0; i < MAX_SMCO; i++) smcos[i].active = 0;
    for (int i = 0; i < MAX_ALNICO; i++) alnicos[i].active = 0;
    for (int i = 0; i < MAX_FERRITE_PM; i++) fpms[i].active = 0;
    for (int i = 0; i < MAX_APP_PM; i++) app_pms[i].active = 0;
    initialized = 1;
    print_str("[PMA] Permanent magnet initialized\n");
    return 0;
}

int pma_ndfeb(int nd_type, int cat, int sint, int bond, int hpress, int year) {
    if (pma.n_ndfeb >= MAX_NDFEB) return -1;
    ndfeb_t* n = &ndfebs[pma.n_ndfeb];
    n->ndfeb_id = pma.n_ndfeb;
    n->ndfeb_type = nd_type;
    n->ndfeb_category = cat;
    n->sintered = sint;
    n->bonded = bond;
    n->hot_press = hpress;
    n->year = year;
    n->active = 1;
    pma.total_sintered += sint;
    pma.n_ndfeb++;
    print_str("[PMA] NdFeB "); print_int(pma.n_ndfeb - 1);
    print_str(" type="); print_int(nd_type);
    print_str(" cat="); print_int(cat);
    print_str(" sin="); print_int(sint);
    print_str(" bnd="); print_int(bond);
    print_str(" hpr="); print_int(hpress); print_str("\n");
    return pma.n_ndfeb - 1;
}

int pma_smco(int sc_type, int cat, int sm5, int sm17, int custom, int year) {
    if (pma.n_smco >= MAX_SMCO) return -1;
    smco_t* s = &smcos[pma.n_smco];
    s->smco_id = pma.n_smco;
    s->smco_type = sc_type;
    s->smco_category = cat;
    s->smco5 = sm5;
    s->sm2co17 = sm17;
    s->smco_custom = custom;
    s->year = year;
    s->active = 1;
    pma.total_smco5 += sm5;
    pma.n_smco++;
    print_str("[PMA] SmCo "); print_int(pma.n_smco - 1);
    print_str(" type="); print_int(sc_type);
    print_str(" cat="); print_int(cat);
    print_str(" sm5="); print_int(sm5);
    print_str(" s17="); print_int(sm17);
    print_str(" cst="); print_int(custom); print_str("\n");
    return pma.n_smco - 1;
}

int pma_alnico(int al_type, int cat, int cast_v, int sint_v, int bond_v, int year) {
    if (pma.n_alnico >= MAX_ALNICO) return -1;
    alnico_t* a = &alnicos[pma.n_alnico];
    a->alnico_id = pma.n_alnico;
    a->alnico_type = al_type;
    a->alnico_category = cat;
    a->cast_alnico = cast_v;
    a->sinter_alnico = sint_v;
    a->bonded_alnico = bond_v;
    a->year = year;
    a->active = 1;
    pma.total_cast += cast_v;
    pma.n_alnico++;
    print_str("[PMA] AlNiCo "); print_int(pma.n_alnico - 1);
    print_str(" type="); print_int(al_type);
    print_str(" cat="); print_int(cat);
    print_str(" cst="); print_int(cast_v);
    print_str(" sin="); print_int(sint_v);
    print_str(" bnd="); print_int(bond_v); print_str("\n");
    return pma.n_alnico - 1;
}

int pma_fpm(int fp_type, int cat, int aniso, int iso_v, int custom_f, int year) {
    if (pma.n_fpm >= MAX_FERRITE_PM) return -1;
    fpm_t* f = &fpms[pma.n_fpm];
    f->fpm_id = pma.n_fpm;
    f->fpm_type = fp_type;
    f->fpm_category = cat;
    f->aniso_fer = aniso;
    f->iso_fer = iso_v;
    f->custom_fer = custom_f;
    f->year = year;
    f->active = 1;
    pma.total_aniso += aniso;
    pma.n_fpm++;
    print_str("[PMA] Ferrite PM "); print_int(pma.n_fpm - 1);
    print_str(" type="); print_int(fp_type);
    print_str(" cat="); print_int(cat);
    print_str(" ans="); print_int(aniso);
    print_str(" iso="); print_int(iso_v);
    print_str(" cfm="); print_int(custom_f); print_str("\n");
    return pma.n_fpm - 1;
}

int pma_app_pm(int ap_type, int cat, int motor_v, int speaker, int mri, int year) {
    if (pma.n_app_pm >= MAX_APP_PM) return -1;
    app_pm_t* a = &app_pms[pma.n_app_pm];
    a->app_id = pma.n_app_pm;
    a->app_type = ap_type;
    a->app_category = cat;
    a->motor_magnet = motor_v;
    a->speaker_magnet = speaker;
    a->mri_magnet = mri;
    a->year = year;
    a->active = 1;
    pma.total_motor += motor_v;
    pma.n_app_pm++;
    print_str("[PMA] App "); print_int(pma.n_app_pm - 1);
    print_str(" type="); print_int(ap_type);
    print_str(" cat="); print_int(cat);
    print_str(" mtr="); print_int(motor_v);
    print_str(" spk="); print_int(speaker);
    print_str(" mri="); print_int(mri); print_str("\n");
    return pma.n_app_pm - 1;
}

void pma_ndfeb_report(void) {
    print_str("[PMA] NdFeB report:\n");
    print_str("  NdFeB PM categories: "); print_int(pma.n_ndfeb); print_str("\n");
    print_str("  Total sintered NdFeB: "); print_int(pma.total_sintered); print_str("\n");
}

void pma_smco_report(void) {
    print_str("[PMA] SmCo report:\n");
    print_str("  SmCo PM categories: "); print_int(pma.n_smco); print_str("\n");
    print_str("  Total SmCo5: "); print_int(pma.total_smco5); print_str("\n");
}

void pma_app_report(void) {
    print_str("[PMA] App report:\n");
    print_str("  AlNiCo PM categories: "); print_int(pma.n_alnico); print_str("\n");
    print_str("  Total cast AlNiCo: "); print_int(pma.total_cast); print_str("\n");
    print_str("  Ferrite PM categories: "); print_int(pma.n_fpm); print_str("\n");
    print_str("  Total anisotropic ferrite: "); print_int(pma.total_aniso); print_str("\n");
    print_str("  PM application categories: "); print_int(pma.n_app_pm); print_str("\n");
    print_str("  Total motor magnets: "); print_int(pma.total_motor); print_str("\n");
}

void pma_print_state(void) {
    print_str("[PMA] Nd="); print_int(pma.n_ndfeb);
    print_str(" Sm="); print_int(pma.n_smco);
    print_str(" Al="); print_int(pma.n_alnico);
    print_str(" Fp="); print_int(pma.n_fpm);
    print_str(" Ap="); print_int(pma.n_app_pm);
    print_str("\n");
}

int main(void) {
    print_str("=== Permanent Magnet Admin Demo ===\n\n");
    pma_init();

    print_str("NdFeB magnets...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int sin = 60 + (i * 14);
        int bnd = 42 + (i * 10);
        int hpr = 25 + (i * 6);
        int year = 2020 + (i % 5);
        pma_ndfeb(type, cat, sin, bnd, hpr, year);
    }

    print_str("\nSmCo magnets...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int sm5 = 50 + (i * 12);
        int s17 = 35 + (i * 8);
        int cst = 22 + (i * 5);
        int year = 2021 + (i % 4);
        pma_smco(type, cat, sm5, s17, cst, year);
    }

    print_str("\nAlNiCo magnets...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cts = 45 + (i * 11);
        int sin = 30 + (i * 7);
        int bnd = 18 + (i * 4);
        int year = 2022 + (i % 3);
        pma_alnico(type, cat, cts, sin, bnd, year);
    }

    print_str("\nFerrite PM...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ans = 38 + (i * 9);
        int iso = 25 + (i * 6);
        int cfm = 15 + (i * 3);
        int year = 2023 + (i % 2);
        pma_fpm(type, cat, ans, iso, cfm, year);
    }

    print_str("\nPM applications...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int mtr = 32 + (i * 8);
        int spk = 22 + (i * 5);
        int mri = 12 + (i * 3);
        int year = 2024;
        pma_app_pm(type, cat, mtr, spk, mri, year);
    }

    print_str("\nNdFeB report...\n");
    pma_ndfeb_report();

    print_str("\nSmCo report...\n");
    pma_smco_report();

    print_str("\nApp report...\n");
    pma_app_report();

    print_str("\nFinal state...\n");
    pma_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
