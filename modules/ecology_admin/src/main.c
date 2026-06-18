/* ecology_admin: Ecology administration system (v1.0)
 * Individual ecology, population ecology, community ecology, ecosystem, applied ecology
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

#define MAX_IND_ECO     16
#define MAX_POP_ECO     14
#define MAX_COMMUN_ECO  12
#define MAX_ECO_SYS     10
#define MAX_APPL_ECO    10

typedef struct {
    int    ie_id;
    int    ie_type;
    int    ie_category;
    int    phy_eco;
    int    beh_eco;
    int    ada_eco;
    int    eco_niche;
    int    year;
    int    active;
} ind_eco_t;

typedef struct {
    int    pe_id;
    int    pe_type;
    int    pe_category;
    int    pop_dyn;
    int    pop_reg;
    int    life_his;
    int    intra_rel;
    int    year;
    int    active;
} pop_eco_t;

typedef struct {
    int    ce_id;
    int    ce_type;
    int    ce_category;
    int    com_str;
    int    com_suc;
    int    bio_div;
    int    inter_rel;
    int    year;
    int    active;
} commun_eco_t;

typedef struct {
    int    es_id;
    int    es_type;
    int    es_category;
    int    eng_flow;
    int    mat_cyc;
    int    eco_svc;
    int    eco_mod;
    int    year;
    int    active;
} eco_sys_t;

typedef struct {
    int    ae_id;
    int    ae_type;
    int    ae_category;
    int    res_eco;
    int    con_bio;
    int    urb_eco;
    int    land_eco;
    int    year;
    int    active;
} appl_eco_t;

typedef struct {
    int    n_ind_eco;
    int    n_pop_eco;
    int    n_commun_eco;
    int    n_eco_sys;
    int    n_appl_eco;
    int    total_phy_eco;
    int    total_pop_dyn;
    int    total_com_str;
    int    total_eng_flow;
    int    total_res_eco;
} ela_state_t;

static ind_eco_t ind_ecos[MAX_IND_ECO];
static pop_eco_t pop_ecos[MAX_POP_ECO];
static commun_eco_t commun_ecos[MAX_COMMUN_ECO];
static eco_sys_t eco_syss[MAX_ECO_SYS];
static appl_eco_t appl_ecos[MAX_APPL_ECO];
static ela_state_t ela;

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

int ela_init(void) {
    if (initialized) return -1;
    ela.n_ind_eco = 0; ela.n_pop_eco = 0; ela.n_commun_eco = 0;
    ela.n_eco_sys = 0; ela.n_appl_eco = 0;
    ela.total_phy_eco = 0; ela.total_pop_dyn = 0;
    ela.total_com_str = 0; ela.total_eng_flow = 0;
    ela.total_res_eco = 0;
    for (int i = 0; i < MAX_IND_ECO; i++) ind_ecos[i].active = 0;
    for (int i = 0; i < MAX_POP_ECO; i++) pop_ecos[i].active = 0;
    for (int i = 0; i < MAX_COMMUN_ECO; i++) commun_ecos[i].active = 0;
    for (int i = 0; i < MAX_ECO_SYS; i++) eco_syss[i].active = 0;
    for (int i = 0; i < MAX_APPL_ECO; i++) appl_ecos[i].active = 0;
    initialized = 1;
    print_str("[ELA] Ecology initialized\n");
    return 0;
}

int ela_ind_eco(int it_type, int cat, int phe, int bhe, int ade, int nich, int year) {
    if (ela.n_ind_eco >= MAX_IND_ECO) return -1;
    ind_eco_t* n = &ind_ecos[ela.n_ind_eco];
    n->ie_id = ela.n_ind_eco;
    n->ie_type = it_type;
    n->ie_category = cat;
    n->phy_eco = phe;
    n->beh_eco = bhe;
    n->ada_eco = ade;
    n->eco_niche = nich;
    n->year = year;
    n->active = 1;
    ela.total_phy_eco += phe;
    ela.n_ind_eco++;
    print_str("[ELA] Ind eco "); print_int(ela.n_ind_eco - 1);
    print_str(" type="); print_int(it_type);
    print_str(" cat="); print_int(cat);
    print_str(" phe="); print_int(phe);
    print_str(" bhe="); print_int(bhe);
    print_str(" ade="); print_int(ade);
    print_str(" nich="); print_int(nich); print_str("\n");
    return ela.n_ind_eco - 1;
}

int ela_pop_eco(int pt_type, int cat, int pdy, int pre, int lhf, int inr, int year) {
    if (ela.n_pop_eco >= MAX_POP_ECO) return -1;
    pop_eco_t* p = &pop_ecos[ela.n_pop_eco];
    p->pe_id = ela.n_pop_eco;
    p->pe_type = pt_type;
    p->pe_category = cat;
    p->pop_dyn = pdy;
    p->pop_reg = pre;
    p->life_his = lhf;
    p->intra_rel = inr;
    p->year = year;
    p->active = 1;
    ela.total_pop_dyn += pdy;
    ela.n_pop_eco++;
    print_str("[ELA] Pop eco "); print_int(ela.n_pop_eco - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" pdy="); print_int(pdy);
    print_str(" pre="); print_int(pre);
    print_str(" lhf="); print_int(lhf);
    print_str(" inr="); print_int(inr); print_str("\n");
    return ela.n_pop_eco - 1;
}

int ela_commun_eco(int ct_type, int cat, int cstr, int csuc, int bdiv, int inr, int year) {
    if (ela.n_commun_eco >= MAX_COMMUN_ECO) return -1;
    commun_eco_t* c = &commun_ecos[ela.n_commun_eco];
    c->ce_id = ela.n_commun_eco;
    c->ce_type = ct_type;
    c->ce_category = cat;
    c->com_str = cstr;
    c->com_suc = csuc;
    c->bio_div = bdiv;
    c->inter_rel = inr;
    c->year = year;
    c->active = 1;
    ela.total_com_str += cstr;
    ela.n_commun_eco++;
    print_str("[ELA] Commun eco "); print_int(ela.n_commun_eco - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" cstr="); print_int(cstr);
    print_str(" csuc="); print_int(csuc);
    print_str(" bdiv="); print_int(bdiv);
    print_str(" inr="); print_int(inr); print_str("\n");
    return ela.n_commun_eco - 1;
}

int ela_eco_sys(int et_type, int cat, int efl, int mcy, int esvc, int emod, int year) {
    if (ela.n_eco_sys >= MAX_ECO_SYS) return -1;
    eco_sys_t* e = &eco_syss[ela.n_eco_sys];
    e->es_id = ela.n_eco_sys;
    e->es_type = et_type;
    e->es_category = cat;
    e->eng_flow = efl;
    e->mat_cyc = mcy;
    e->eco_svc = esvc;
    e->eco_mod = emod;
    e->year = year;
    e->active = 1;
    ela.total_eng_flow += efl;
    ela.n_eco_sys++;
    print_str("[ELA] Eco sys "); print_int(ela.n_eco_sys - 1);
    print_str(" type="); print_int(et_type);
    print_str(" cat="); print_int(cat);
    print_str(" efl="); print_int(efl);
    print_str(" mcy="); print_int(mcy);
    print_str(" esvc="); print_int(esvc);
    print_str(" emod="); print_int(emod); print_str("\n");
    return ela.n_eco_sys - 1;
}

int ela_appl_eco(int at_type, int cat, int rec, int cbio, int urb, int land, int year) {
    if (ela.n_appl_eco >= MAX_APPL_ECO) return -1;
    appl_eco_t* a = &appl_ecos[ela.n_appl_eco];
    a->ae_id = ela.n_appl_eco;
    a->ae_type = at_type;
    a->ae_category = cat;
    a->res_eco = rec;
    a->con_bio = cbio;
    a->urb_eco = urb;
    a->land_eco = land;
    a->year = year;
    a->active = 1;
    ela.total_res_eco += rec;
    ela.n_appl_eco++;
    print_str("[ELA] Appl eco "); print_int(ela.n_appl_eco - 1);
    print_str(" type="); print_int(at_type);
    print_str(" cat="); print_int(cat);
    print_str(" rec="); print_int(rec);
    print_str(" cbio="); print_int(cbio);
    print_str(" urb="); print_int(urb);
    print_str(" land="); print_int(land); print_str("\n");
    return ela.n_appl_eco - 1;
}

void ela_ind_report(void) {
    print_str("[ELA] Individual ecology report:\n");
    print_str("  Individual categories: "); print_int(ela.n_ind_eco); print_str("\n");
    print_str("  Total physiological ecology: "); print_int(ela.total_phy_eco); print_str("\n");
}

void ela_pop_report(void) {
    print_str("[ELA] Population ecology report:\n");
    print_str("  Population categories: "); print_int(ela.n_pop_eco); print_str("\n");
    print_str("  Total population dynamics: "); print_int(ela.total_pop_dyn); print_str("\n");
}

void ela_full_report(void) {
    print_str("[ELA] Full report:\n");
    print_str("  Community categories: "); print_int(ela.n_commun_eco); print_str("\n");
    print_str("  Total community structure: "); print_int(ela.total_com_str); print_str("\n");
    print_str("  Ecosystem categories: "); print_int(ela.n_eco_sys); print_str("\n");
    print_str("  Total energy flow: "); print_int(ela.total_eng_flow); print_str("\n");
    print_str("  Applied categories: "); print_int(ela.n_appl_eco); print_str("\n");
    print_str("  Total restoration ecology: "); print_int(ela.total_res_eco); print_str("\n");
}

void ela_print_state(void) {
    print_str("[ELA] Ie="); print_int(ela.n_ind_eco);
    print_str(" Pe="); print_int(ela.n_pop_eco);
    print_str(" Ce="); print_int(ela.n_commun_eco);
    print_str(" Es="); print_int(ela.n_eco_sys);
    print_str(" Ae="); print_int(ela.n_appl_eco);
    print_str("\n");
}

int main(void) {
    print_str("=== Ecology Admin Demo ===\n\n");
    ela_init();

    print_str("Individual ecology...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int phe = 55 + (i * 13);
        int bhe = 40 + (i * 10);
        int ade = 22 + (i * 5);
        int nich = 15 + (i * 3);
        int year = 2020 + (i % 5);
        ela_ind_eco(type, cat, phe, bhe, ade, nich, year);
    }

    print_str("\nPopulation ecology...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pdy = 48 + (i * 11);
        int pre = 35 + (i * 8);
        int lhf = 20 + (i * 4);
        int inr = 12 + (i * 3);
        int year = 2021 + (i % 4);
        ela_pop_eco(type, cat, pdy, pre, lhf, inr, year);
    }

    print_str("\nCommunity ecology...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cstr = 42 + (i * 10);
        int csuc = 28 + (i * 7);
        int bdiv = 18 + (i * 4);
        int inr = 10 + (i * 2);
        int year = 2022 + (i % 3);
        ela_commun_eco(type, cat, cstr, csuc, bdiv, inr, year);
    }

    print_str("\nEcosystem ecology...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int efl = 35 + (i * 8);
        int mcy = 25 + (i * 6);
        int esvc = 15 + (i * 3);
        int emod = 10 + (i * 2);
        int year = 2023 + (i % 2);
        ela_eco_sys(type, cat, efl, mcy, esvc, emod, year);
    }

    print_str("\nApplied ecology...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int rec = 30 + (i * 7);
        int cbio = 22 + (i * 5);
        int urb = 12 + (i * 3);
        int land = 8 + (i * 2);
        int year = 2024;
        ela_appl_eco(type, cat, rec, cbio, urb, land, year);
    }

    print_str("\nIndividual report...\n");
    ela_ind_report();

    print_str("\nPopulation report...\n");
    ela_pop_report();

    print_str("\nFull report...\n");
    ela_full_report();

    print_str("\nFinal state...\n");
    ela_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
