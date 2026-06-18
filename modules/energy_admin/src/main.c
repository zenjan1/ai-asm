/* energy_admin: Energy administration system (v1.0)
 * Energy science, renewable energy, nuclear engineering, fossil energy, new energy
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

#define MAX_ENG_SCI     16
#define MAX_REN_ENG     14
#define MAX_NUC_ENG     12
#define MAX_FOS_ENG     10
#define MAX_NEW_ENG     10

typedef struct {
    int    es_id;
    int    es_type;
    int    es_category;
    int    eng_the;
    int    eng_con;
    int    eng_sto;
    int    eng_eff;
    int    year;
    int    active;
} eng_sci_t;

typedef struct {
    int    re_id;
    int    re_type;
    int    re_category;
    int    sol_ene;
    int    win_ene;
    int    wat_ene;
    int    bio_ene;
    int    year;
    int    active;
} ren_eng_t;

typedef struct {
    int    nu_id;
    int    nu_type;
    int    nu_category;
    int    nuc_phy;
    int    nuc_rea;
    int    nuc_fue;
    int    nuc_saf;
    int    year;
    int    active;
} nuc_eng_t;

typedef struct {
    int    fo_id;
    int    fo_type;
    int    fo_category;
    int    coa_eng;
    int    pet_eng;
    int    nat_gas;
    int    sha_gas;
    int    year;
    int    active;
} fos_eng_t;

typedef struct {
    int    ne_id;
    int    ne_type;
    int    ne_category;
    int    geo_ene;
    int    oce_ene;
    int    hyd_ene;
    int    fue_cell;
    int    year;
    int    active;
} new_eng_t;

typedef struct {
    int    n_eng_sci;
    int    n_ren_eng;
    int    n_nuc_eng;
    int    n_fos_eng;
    int    n_new_eng;
    int    total_eng_the;
    int    total_sol_ene;
    int    total_nuc_phy;
    int    total_coa_eng;
    int    total_geo_ene;
} ena_state_t;

static eng_sci_t eng_scis[MAX_ENG_SCI];
static ren_eng_t ren_engs[MAX_REN_ENG];
static nuc_eng_t nuc_engs[MAX_NUC_ENG];
static fos_eng_t fos_engs[MAX_FOS_ENG];
static new_eng_t new_engs[MAX_NEW_ENG];
static ena_state_t ena;

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

int ena_init(void) {
    if (initialized) return -1;
    ena.n_eng_sci = 0; ena.n_ren_eng = 0; ena.n_nuc_eng = 0;
    ena.n_fos_eng = 0; ena.n_new_eng = 0;
    ena.total_eng_the = 0; ena.total_sol_ene = 0;
    ena.total_nuc_phy = 0; ena.total_coa_eng = 0;
    ena.total_geo_ene = 0;
    for (int i = 0; i < MAX_ENG_SCI; i++) eng_scis[i].active = 0;
    for (int i = 0; i < MAX_REN_ENG; i++) ren_engs[i].active = 0;
    for (int i = 0; i < MAX_NUC_ENG; i++) nuc_engs[i].active = 0;
    for (int i = 0; i < MAX_FOS_ENG; i++) fos_engs[i].active = 0;
    for (int i = 0; i < MAX_NEW_ENG; i++) new_engs[i].active = 0;
    initialized = 1;
    print_str("[ENA] Energy initialized\n");
    return 0;
}

int ena_eng_sci(int st_type, int cat, int eth, int eco, int est, int eef, int year) {
    if (ena.n_eng_sci >= MAX_ENG_SCI) return -1;
    eng_sci_t* e = &eng_scis[ena.n_eng_sci];
    e->es_id = ena.n_eng_sci;
    e->es_type = st_type;
    e->es_category = cat;
    e->eng_the = eth;
    e->eng_con = eco;
    e->eng_sto = est;
    e->eng_eff = eef;
    e->year = year;
    e->active = 1;
    ena.total_eng_the += eth;
    ena.n_eng_sci++;
    print_str("[ENA] Eng sci "); print_int(ena.n_eng_sci - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" eth="); print_int(eth);
    print_str(" eco="); print_int(eco);
    print_str(" est="); print_int(est);
    print_str(" eef="); print_int(eef); print_str("\n");
    return ena.n_eng_sci - 1;
}

int ena_ren_eng(int rt_type, int cat, int sen, int wen, int wat, int ben, int year) {
    if (ena.n_ren_eng >= MAX_REN_ENG) return -1;
    ren_eng_t* r = &ren_engs[ena.n_ren_eng];
    r->re_id = ena.n_ren_eng;
    r->re_type = rt_type;
    r->re_category = cat;
    r->sol_ene = sen;
    r->win_ene = wen;
    r->wat_ene = wat;
    r->bio_ene = ben;
    r->year = year;
    r->active = 1;
    ena.total_sol_ene += sen;
    ena.n_ren_eng++;
    print_str("[ENA] Ren eng "); print_int(ena.n_ren_eng - 1);
    print_str(" type="); print_int(rt_type);
    print_str(" cat="); print_int(cat);
    print_str(" sen="); print_int(sen);
    print_str(" wen="); print_int(wen);
    print_str(" wat="); print_int(wat);
    print_str(" ben="); print_int(ben); print_str("\n");
    return ena.n_ren_eng - 1;
}

int ena_nuc_eng(int nt_type, int cat, int nph, int nre, int nfue, int nsaf, int year) {
    if (ena.n_nuc_eng >= MAX_NUC_ENG) return -1;
    nuc_eng_t* n = &nuc_engs[ena.n_nuc_eng];
    n->nu_id = ena.n_nuc_eng;
    n->nu_type = nt_type;
    n->nu_category = cat;
    n->nuc_phy = nph;
    n->nuc_rea = nre;
    n->nuc_fue = nfue;
    n->nuc_saf = nsaf;
    n->year = year;
    n->active = 1;
    ena.total_nuc_phy += nph;
    ena.n_nuc_eng++;
    print_str("[ENA] Nuc eng "); print_int(ena.n_nuc_eng - 1);
    print_str(" type="); print_int(nt_type);
    print_str(" cat="); print_int(cat);
    print_str(" nph="); print_int(nph);
    print_str(" nre="); print_int(nre);
    print_str(" nfue="); print_int(nfue);
    print_str(" nsaf="); print_int(nsaf); print_str("\n");
    return ena.n_nuc_eng - 1;
}

int ena_fos_eng(int ft_type, int cat, int coa, int pet, int ngas, int shg, int year) {
    if (ena.n_fos_eng >= MAX_FOS_ENG) return -1;
    fos_eng_t* f = &fos_engs[ena.n_fos_eng];
    f->fo_id = ena.n_fos_eng;
    f->fo_type = ft_type;
    f->fo_category = cat;
    f->coa_eng = coa;
    f->pet_eng = pet;
    f->nat_gas = ngas;
    f->sha_gas = shg;
    f->year = year;
    f->active = 1;
    ena.total_coa_eng += coa;
    ena.n_fos_eng++;
    print_str("[ENA] Fos eng "); print_int(ena.n_fos_eng - 1);
    print_str(" type="); print_int(ft_type);
    print_str(" cat="); print_int(cat);
    print_str(" coa="); print_int(coa);
    print_str(" pet="); print_int(pet);
    print_str(" ngas="); print_int(ngas);
    print_str(" shg="); print_int(shg); print_str("\n");
    return ena.n_fos_eng - 1;
}

int ena_new_eng(int nt_type, int cat, int geo, int oce, int hyd, int fce, int year) {
    if (ena.n_new_eng >= MAX_NEW_ENG) return -1;
    new_eng_t* n = &new_engs[ena.n_new_eng];
    n->ne_id = ena.n_new_eng;
    n->ne_type = nt_type;
    n->ne_category = cat;
    n->geo_ene = geo;
    n->oce_ene = oce;
    n->hyd_ene = hyd;
    n->fue_cell = fce;
    n->year = year;
    n->active = 1;
    ena.total_geo_ene += geo;
    ena.n_new_eng++;
    print_str("[ENA] New eng "); print_int(ena.n_new_eng - 1);
    print_str(" type="); print_int(nt_type);
    print_str(" cat="); print_int(cat);
    print_str(" geo="); print_int(geo);
    print_str(" oce="); print_int(oce);
    print_str(" hyd="); print_int(hyd);
    print_str(" fce="); print_int(fce); print_str("\n");
    return ena.n_new_eng - 1;
}

void ena_science_report(void) {
    print_str("[ENA] Energy science report:\n");
    print_str("  Science categories: "); print_int(ena.n_eng_sci); print_str("\n");
    print_str("  Total energy theory: "); print_int(ena.total_eng_the); print_str("\n");
}

void ena_renewable_report(void) {
    print_str("[ENA] Renewable report:\n");
    print_str("  Renewable categories: "); print_int(ena.n_ren_eng); print_str("\n");
    print_str("  Total solar energy: "); print_int(ena.total_sol_ene); print_str("\n");
}

void ena_full_report(void) {
    print_str("[ENA] Full report:\n");
    print_str("  Nuclear categories: "); print_int(ena.n_nuc_eng); print_str("\n");
    print_str("  Total nuclear physics: "); print_int(ena.total_nuc_phy); print_str("\n");
    print_str("  Fossil categories: "); print_int(ena.n_fos_eng); print_str("\n");
    print_str("  Total coal engineering: "); print_int(ena.total_coa_eng); print_str("\n");
    print_str("  New energy categories: "); print_int(ena.n_new_eng); print_str("\n");
    print_str("  Total geothermal energy: "); print_int(ena.total_geo_ene); print_str("\n");
}

void ena_print_state(void) {
    print_str("[ENA] Es="); print_int(ena.n_eng_sci);
    print_str(" Re="); print_int(ena.n_ren_eng);
    print_str(" Nu="); print_int(ena.n_nuc_eng);
    print_str(" Fo="); print_int(ena.n_fos_eng);
    print_str(" Ne="); print_int(ena.n_new_eng);
    print_str("\n");
}

int main(void) {
    print_str("=== Energy Admin Demo ===\n\n");
    ena_init();

    print_str("Energy science...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int eth = 55 + (i * 13);
        int eco = 40 + (i * 10);
        int est = 22 + (i * 5);
        int eef = 15 + (i * 3);
        int year = 2020 + (i % 5);
        ena_eng_sci(type, cat, eth, eco, est, eef, year);
    }

    print_str("\nRenewable...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int sen = 48 + (i * 11);
        int wen = 35 + (i * 8);
        int wat = 20 + (i * 4);
        int ben = 12 + (i * 3);
        int year = 2021 + (i % 4);
        ena_ren_eng(type, cat, sen, wen, wat, ben, year);
    }

    print_str("\nNuclear...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int nph = 42 + (i * 10);
        int nre = 28 + (i * 7);
        int nfue = 18 + (i * 4);
        int nsaf = 10 + (i * 2);
        int year = 2022 + (i % 3);
        ena_nuc_eng(type, cat, nph, nre, nfue, nsaf, year);
    }

    print_str("\nFossil...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int coa = 35 + (i * 8);
        int pet = 25 + (i * 6);
        int ngas = 15 + (i * 3);
        int shg = 10 + (i * 2);
        int year = 2023 + (i % 2);
        ena_fos_eng(type, cat, coa, pet, ngas, shg, year);
    }

    print_str("\nNew energy...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int geo = 30 + (i * 7);
        int oce = 22 + (i * 5);
        int hyd = 12 + (i * 3);
        int fce = 8 + (i * 2);
        int year = 2024;
        ena_new_eng(type, cat, geo, oce, hyd, fce, year);
    }

    print_str("\nScience report...\n");
    ena_science_report();

    print_str("\nRenewable report...\n");
    ena_renewable_report();

    print_str("\nFull report...\n");
    ena_full_report();

    print_str("\nFinal state...\n");
    ena_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
