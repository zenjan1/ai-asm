/* civil_admin: Civil engineering administration system (v1.0)
 * Structural, hydraulic, transportation, municipal, construction engineering
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

#define MAX_STR_ENG     16
#define MAX_HYD_ENG     14
#define MAX_TRA_ENG     12
#define MAX_MUN_ENG     10
#define MAX_CON_ENG     10

typedef struct {
    int    se_id;
    int    se_type;
    int    se_category;
    int    bld_str;
    int    bri_eng;
    int    geo_tec;
    int    ase_eng;
    int    year;
    int    active;
} str_eng_t;

typedef struct {
    int    he_id;
    int    he_type;
    int    he_category;
    int    hyd_hyd;
    int    hyd_mec;
    int    hyd_hbu;
    int    irr_dra;
    int    year;
    int    active;
} hyd_eng_t;

typedef struct {
    int    te_id;
    int    te_type;
    int    te_category;
    int    roa_eng;
    int    rai_eng;
    int    air_eng;
    int    tun_eng;
    int    year;
    int    active;
} tra_eng_t;

typedef struct {
    int    me_id;
    int    me_type;
    int    me_category;
    int    wat_sup;
    int    urb_heat;
    int    gas_eng;
    int    env_san;
    int    year;
    int    active;
} mun_eng_t;

typedef struct {
    int    ce_id;
    int    ce_type;
    int    ce_category;
    int    bld_des;
    int    bld_tec;
    int    bld_eco;
    int    bld_man;
    int    year;
    int    active;
} con_eng_t;

typedef struct {
    int    n_str_eng;
    int    n_hyd_eng;
    int    n_tra_eng;
    int    n_mun_eng;
    int    n_con_eng;
    int    total_bld_str;
    int    total_hyd_hyd;
    int    total_roa_eng;
    int    total_wat_sup;
    int    total_bld_des;
} cia_state_t;

static str_eng_t str_engs[MAX_STR_ENG];
static hyd_eng_t hyd_engs[MAX_HYD_ENG];
static tra_eng_t tra_engs[MAX_TRA_ENG];
static mun_eng_t mun_engs[MAX_MUN_ENG];
static con_eng_t con_engs[MAX_CON_ENG];
static cia_state_t cia;

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

int cia_init(void) {
    if (initialized) return -1;
    cia.n_str_eng = 0; cia.n_hyd_eng = 0; cia.n_tra_eng = 0;
    cia.n_mun_eng = 0; cia.n_con_eng = 0;
    cia.total_bld_str = 0; cia.total_hyd_hyd = 0;
    cia.total_roa_eng = 0; cia.total_wat_sup = 0;
    cia.total_bld_des = 0;
    for (int i = 0; i < MAX_STR_ENG; i++) str_engs[i].active = 0;
    for (int i = 0; i < MAX_HYD_ENG; i++) hyd_engs[i].active = 0;
    for (int i = 0; i < MAX_TRA_ENG; i++) tra_engs[i].active = 0;
    for (int i = 0; i < MAX_MUN_ENG; i++) mun_engs[i].active = 0;
    for (int i = 0; i < MAX_CON_ENG; i++) con_engs[i].active = 0;
    initialized = 1;
    print_str("[CIA] Civil initialized\n");
    return 0;
}

int cia_str_eng(int st_type, int cat, int bsr, int ber, int gte, int aee, int year) {
    if (cia.n_str_eng >= MAX_STR_ENG) return -1;
    str_eng_t* s = &str_engs[cia.n_str_eng];
    s->se_id = cia.n_str_eng;
    s->se_type = st_type;
    s->se_category = cat;
    s->bld_str = bsr;
    s->bri_eng = ber;
    s->geo_tec = gte;
    s->ase_eng = aee;
    s->year = year;
    s->active = 1;
    cia.total_bld_str += bsr;
    cia.n_str_eng++;
    print_str("[CIA] Str eng "); print_int(cia.n_str_eng - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" bsr="); print_int(bsr);
    print_str(" ber="); print_int(ber);
    print_str(" gte="); print_int(gte);
    print_str(" aee="); print_int(aee); print_str("\n");
    return cia.n_str_eng - 1;
}

int cia_hyd_eng(int ht_type, int cat, int hhd, int hmc, int hhu, int idr, int year) {
    if (cia.n_hyd_eng >= MAX_HYD_ENG) return -1;
    hyd_eng_t* h = &hyd_engs[cia.n_hyd_eng];
    h->he_id = cia.n_hyd_eng;
    h->he_type = ht_type;
    h->he_category = cat;
    h->hyd_hyd = hhd;
    h->hyd_mec = hmc;
    h->hyd_hbu = hhu;
    h->irr_dra = idr;
    h->year = year;
    h->active = 1;
    cia.total_hyd_hyd += hhd;
    cia.n_hyd_eng++;
    print_str("[CIA] Hyd eng "); print_int(cia.n_hyd_eng - 1);
    print_str(" type="); print_int(ht_type);
    print_str(" cat="); print_int(cat);
    print_str(" hhd="); print_int(hhd);
    print_str(" hmc="); print_int(hmc);
    print_str(" hhu="); print_int(hhu);
    print_str(" idr="); print_int(idr); print_str("\n");
    return cia.n_hyd_eng - 1;
}

int cia_tra_eng(int tt_type, int cat, int ren, int raen, int aen, int ten, int year) {
    if (cia.n_tra_eng >= MAX_TRA_ENG) return -1;
    tra_eng_t* t = &tra_engs[cia.n_tra_eng];
    t->te_id = cia.n_tra_eng;
    t->te_type = tt_type;
    t->te_category = cat;
    t->roa_eng = ren;
    t->rai_eng = raen;
    t->air_eng = aen;
    t->tun_eng = ten;
    t->year = year;
    t->active = 1;
    cia.total_roa_eng += ren;
    cia.n_tra_eng++;
    print_str("[CIA] Tra eng "); print_int(cia.n_tra_eng - 1);
    print_str(" type="); print_int(tt_type);
    print_str(" cat="); print_int(cat);
    print_str(" ren="); print_int(ren);
    print_str(" raen="); print_int(raen);
    print_str(" aen="); print_int(aen);
    print_str(" ten="); print_int(ten); print_str("\n");
    return cia.n_tra_eng - 1;
}

int cia_mun_eng(int mt_type, int cat, int wts, int uht, int gen, int esn, int year) {
    if (cia.n_mun_eng >= MAX_MUN_ENG) return -1;
    mun_eng_t* m = &mun_engs[cia.n_mun_eng];
    m->me_id = cia.n_mun_eng;
    m->me_type = mt_type;
    m->me_category = cat;
    m->wat_sup = wts;
    m->urb_heat = uht;
    m->gas_eng = gen;
    m->env_san = esn;
    m->year = year;
    m->active = 1;
    cia.total_wat_sup += wts;
    cia.n_mun_eng++;
    print_str("[CIA] Mun eng "); print_int(cia.n_mun_eng - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" wts="); print_int(wts);
    print_str(" uht="); print_int(uht);
    print_str(" gen="); print_int(gen);
    print_str(" esn="); print_int(esn); print_str("\n");
    return cia.n_mun_eng - 1;
}

int cia_con_eng(int ct_type, int cat, int bds, int bte, int bec, int bma, int year) {
    if (cia.n_con_eng >= MAX_CON_ENG) return -1;
    con_eng_t* c = &con_engs[cia.n_con_eng];
    c->ce_id = cia.n_con_eng;
    c->ce_type = ct_type;
    c->ce_category = cat;
    c->bld_des = bds;
    c->bld_tec = bte;
    c->bld_eco = bec;
    c->bld_man = bma;
    c->year = year;
    c->active = 1;
    cia.total_bld_des += bds;
    cia.n_con_eng++;
    print_str("[CIA] Con eng "); print_int(cia.n_con_eng - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" bds="); print_int(bds);
    print_str(" bte="); print_int(bte);
    print_str(" bec="); print_int(bec);
    print_str(" bma="); print_int(bma); print_str("\n");
    return cia.n_con_eng - 1;
}

void cia_structural_report(void) {
    print_str("[CIA] Structural report:\n");
    print_str("  Structural categories: "); print_int(cia.n_str_eng); print_str("\n");
    print_str("  Total building structures: "); print_int(cia.total_bld_str); print_str("\n");
}

void cia_hydraulic_report(void) {
    print_str("[CIA] Hydraulic report:\n");
    print_str("  Hydraulic categories: "); print_int(cia.n_hyd_eng); print_str("\n");
    print_str("  Total hydrology studies: "); print_int(cia.total_hyd_hyd); print_str("\n");
}

void cia_full_report(void) {
    print_str("[CIA] Full report:\n");
    print_str("  Transportation categories: "); print_int(cia.n_tra_eng); print_str("\n");
    print_str("  Total road engineering: "); print_int(cia.total_roa_eng); print_str("\n");
    print_str("  Municipal categories: "); print_int(cia.n_mun_eng); print_str("\n");
    print_str("  Total water supply: "); print_int(cia.total_wat_sup); print_str("\n");
    print_str("  Construction categories: "); print_int(cia.n_con_eng); print_str("\n");
    print_str("  Total building design: "); print_int(cia.total_bld_des); print_str("\n");
}

void cia_print_state(void) {
    print_str("[CIA] Se="); print_int(cia.n_str_eng);
    print_str(" He="); print_int(cia.n_hyd_eng);
    print_str(" Te="); print_int(cia.n_tra_eng);
    print_str(" Me="); print_int(cia.n_mun_eng);
    print_str(" Ce="); print_int(cia.n_con_eng);
    print_str("\n");
}

int main(void) {
    print_str("=== Civil Admin Demo ===\n\n");
    cia_init();

    print_str("Structural...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int bsr = 55 + (i * 13);
        int ber = 40 + (i * 10);
        int gte = 22 + (i * 5);
        int aee = 15 + (i * 3);
        int year = 2020 + (i % 5);
        cia_str_eng(type, cat, bsr, ber, gte, aee, year);
    }

    print_str("\nHydraulic...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int hhd = 48 + (i * 11);
        int hmc = 35 + (i * 8);
        int hhu = 20 + (i * 4);
        int idr = 12 + (i * 3);
        int year = 2021 + (i % 4);
        cia_hyd_eng(type, cat, hhd, hmc, hhu, idr, year);
    }

    print_str("\nTransportation...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ren = 42 + (i * 10);
        int raen = 28 + (i * 7);
        int aen = 18 + (i * 4);
        int ten = 10 + (i * 2);
        int year = 2022 + (i % 3);
        cia_tra_eng(type, cat, ren, raen, aen, ten, year);
    }

    print_str("\nMunicipal...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int wts = 35 + (i * 8);
        int uht = 25 + (i * 6);
        int gen = 15 + (i * 3);
        int esn = 10 + (i * 2);
        int year = 2023 + (i % 2);
        cia_mun_eng(type, cat, wts, uht, gen, esn, year);
    }

    print_str("\nConstruction...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int bds = 30 + (i * 7);
        int bte = 22 + (i * 5);
        int bec = 12 + (i * 3);
        int bma = 8 + (i * 2);
        int year = 2024;
        cia_con_eng(type, cat, bds, bte, bec, bma, year);
    }

    print_str("\nStructural report...\n");
    cia_structural_report();

    print_str("\nHydraulic report...\n");
    cia_hydraulic_report();

    print_str("\nFull report...\n");
    cia_full_report();

    print_str("\nFinal state...\n");
    cia_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
