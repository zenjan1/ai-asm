/* biology_admin: Biology administration system (v1.0)
 * Botany, zoology, microbiology, genetics, cell biology
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

#define MAX_BOTANY      16
#define MAX_ZOOLOGY     14
#define MAX_MICROBIO    12
#define MAX_GENETICS    10
#define MAX_CELL_BIO    10

typedef struct {
    int    bo_id;
    int    bo_type;
    int    bo_category;
    int    pla_cla;
    int    pla_phy;
    int    pla_eco;
    int    pla_gen;
    int    year;
    int    active;
} botany_t;

typedef struct {
    int    zo_id;
    int    zo_type;
    int    zo_category;
    int    ani_cla;
    int    ani_phy;
    int    ani_beh;
    int    ani_eco;
    int    year;
    int    active;
} zoology_t;

typedef struct {
    int    mi_id;
    int    mi_type;
    int    mi_category;
    int    bac_sci;
    int    vir_sci;
    int    fun_sci;
    int    mic_gen;
    int    year;
    int    active;
} microbio_t;

typedef struct {
    int    ge_id;
    int    ge_type;
    int    ge_category;
    int    cls_gen;
    int    mol_gen;
    int    pop_gen;
    int    epi_gen;
    int    year;
    int    active;
} genetics_t;

typedef struct {
    int    ce_id;
    int    ce_type;
    int    ce_category;
    int    cel_str;
    int    cel_met;
    int    cel_sig;
    int    cel_cyc;
    int    year;
    int    active;
} cell_bio_t;

typedef struct {
    int    n_botany;
    int    n_zoology;
    int    n_microbio;
    int    n_genetics;
    int    n_cell_bio;
    int    total_pla_cla;
    int    total_ani_cla;
    int    total_bac_sci;
    int    total_cls_gen;
    int    total_cel_str;
} bia_state_t;

static botany_t botanys[MAX_BOTANY];
static zoology_t zoologys[MAX_ZOOLOGY];
static microbio_t microbios[MAX_MICROBIO];
static genetics_t geneticss[MAX_GENETICS];
static cell_bio_t cell_bios[MAX_CELL_BIO];
static bia_state_t bia;

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

int bia_init(void) {
    if (initialized) return -1;
    bia.n_botany = 0; bia.n_zoology = 0; bia.n_microbio = 0;
    bia.n_genetics = 0; bia.n_cell_bio = 0;
    bia.total_pla_cla = 0; bia.total_ani_cla = 0;
    bia.total_bac_sci = 0; bia.total_cls_gen = 0;
    bia.total_cel_str = 0;
    for (int i = 0; i < MAX_BOTANY; i++) botanys[i].active = 0;
    for (int i = 0; i < MAX_ZOOLOGY; i++) zoologys[i].active = 0;
    for (int i = 0; i < MAX_MICROBIO; i++) microbios[i].active = 0;
    for (int i = 0; i < MAX_GENETICS; i++) geneticss[i].active = 0;
    for (int i = 0; i < MAX_CELL_BIO; i++) cell_bios[i].active = 0;
    initialized = 1;
    print_str("[BIA] Biology initialized\n");
    return 0;
}

int bia_botany(int bt_type, int cat, int pcl, int pph, int pec, int pgn, int year) {
    if (bia.n_botany >= MAX_BOTANY) return -1;
    botany_t* b = &botanys[bia.n_botany];
    b->bo_id = bia.n_botany;
    b->bo_type = bt_type;
    b->bo_category = cat;
    b->pla_cla = pcl;
    b->pla_phy = pph;
    b->pla_eco = pec;
    b->pla_gen = pgn;
    b->year = year;
    b->active = 1;
    bia.total_pla_cla += pcl;
    bia.n_botany++;
    print_str("[BIA] Botany "); print_int(bia.n_botany - 1);
    print_str(" type="); print_int(bt_type);
    print_str(" cat="); print_int(cat);
    print_str(" pcl="); print_int(pcl);
    print_str(" pph="); print_int(pph);
    print_str(" pec="); print_int(pec);
    print_str(" pgn="); print_int(pgn); print_str("\n");
    return bia.n_botany - 1;
}

int bia_zoology(int zt_type, int cat, int acl, int aph, int abh, int aec, int year) {
    if (bia.n_zoology >= MAX_ZOOLOGY) return -1;
    zoology_t* z = &zoologys[bia.n_zoology];
    z->zo_id = bia.n_zoology;
    z->zo_type = zt_type;
    z->zo_category = cat;
    z->ani_cla = acl;
    z->ani_phy = aph;
    z->ani_beh = abh;
    z->ani_eco = aec;
    z->year = year;
    z->active = 1;
    bia.total_ani_cla += acl;
    bia.n_zoology++;
    print_str("[BIA] Zoology "); print_int(bia.n_zoology - 1);
    print_str(" type="); print_int(zt_type);
    print_str(" cat="); print_int(cat);
    print_str(" acl="); print_int(acl);
    print_str(" aph="); print_int(aph);
    print_str(" abh="); print_int(abh);
    print_str(" aec="); print_int(aec); print_str("\n");
    return bia.n_zoology - 1;
}

int bia_microbio(int mt_type, int cat, int bas, int vis, int fus, int mgn, int year) {
    if (bia.n_microbio >= MAX_MICROBIO) return -1;
    microbio_t* m = &microbios[bia.n_microbio];
    m->mi_id = bia.n_microbio;
    m->mi_type = mt_type;
    m->mi_category = cat;
    m->bac_sci = bas;
    m->vir_sci = vis;
    m->fun_sci = fus;
    m->mic_gen = mgn;
    m->year = year;
    m->active = 1;
    bia.total_bac_sci += bas;
    bia.n_microbio++;
    print_str("[BIA] Microbio "); print_int(bia.n_microbio - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" bas="); print_int(bas);
    print_str(" vis="); print_int(vis);
    print_str(" fus="); print_int(fus);
    print_str(" mgn="); print_int(mgn); print_str("\n");
    return bia.n_microbio - 1;
}

int bia_genetics(int gt_type, int cat, int cgn, int mgn, int pgn, int egn, int year) {
    if (bia.n_genetics >= MAX_GENETICS) return -1;
    genetics_t* g = &geneticss[bia.n_genetics];
    g->ge_id = bia.n_genetics;
    g->ge_type = gt_type;
    g->ge_category = cat;
    g->cls_gen = cgn;
    g->mol_gen = mgn;
    g->pop_gen = pgn;
    g->epi_gen = egn;
    g->year = year;
    g->active = 1;
    bia.total_cls_gen += cgn;
    bia.n_genetics++;
    print_str("[BIA] Genetics "); print_int(bia.n_genetics - 1);
    print_str(" type="); print_int(gt_type);
    print_str(" cat="); print_int(cat);
    print_str(" cgn="); print_int(cgn);
    print_str(" mgn="); print_int(mgn);
    print_str(" pgn="); print_int(pgn);
    print_str(" egn="); print_int(egn); print_str("\n");
    return bia.n_genetics - 1;
}

int bia_cell_bio(int ct_type, int cat, int cst, int cmt, int csg, int ccy, int year) {
    if (bia.n_cell_bio >= MAX_CELL_BIO) return -1;
    cell_bio_t* c = &cell_bios[bia.n_cell_bio];
    c->ce_id = bia.n_cell_bio;
    c->ce_type = ct_type;
    c->ce_category = cat;
    c->cel_str = cst;
    c->cel_met = cmt;
    c->cel_sig = csg;
    c->cel_cyc = ccy;
    c->year = year;
    c->active = 1;
    bia.total_cel_str += cst;
    bia.n_cell_bio++;
    print_str("[BIA] Cell bio "); print_int(bia.n_cell_bio - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" cst="); print_int(cst);
    print_str(" cmt="); print_int(cmt);
    print_str(" csg="); print_int(csg);
    print_str(" ccy="); print_int(ccy); print_str("\n");
    return bia.n_cell_bio - 1;
}

void bia_botany_report(void) {
    print_str("[BIA] Botany report:\n");
    print_str("  Botany categories: "); print_int(bia.n_botany); print_str("\n");
    print_str("  Total plant classification: "); print_int(bia.total_pla_cla); print_str("\n");
}

void bia_zoology_report(void) {
    print_str("[BIA] Zoology report:\n");
    print_str("  Zoology categories: "); print_int(bia.n_zoology); print_str("\n");
    print_str("  Total animal classification: "); print_int(bia.total_ani_cla); print_str("\n");
}

void bia_full_report(void) {
    print_str("[BIA] Full report:\n");
    print_str("  Microbiology categories: "); print_int(bia.n_microbio); print_str("\n");
    print_str("  Total bacteriology: "); print_int(bia.total_bac_sci); print_str("\n");
    print_str("  Genetics categories: "); print_int(bia.n_genetics); print_str("\n");
    print_str("  Total classical genetics: "); print_int(bia.total_cls_gen); print_str("\n");
    print_str("  Cell biology categories: "); print_int(bia.n_cell_bio); print_str("\n");
    print_str("  Total cell structure: "); print_int(bia.total_cel_str); print_str("\n");
}

void bia_print_state(void) {
    print_str("[BIA] Bo="); print_int(bia.n_botany);
    print_str(" Zo="); print_int(bia.n_zoology);
    print_str(" Mi="); print_int(bia.n_microbio);
    print_str(" Ge="); print_int(bia.n_genetics);
    print_str(" Ce="); print_int(bia.n_cell_bio);
    print_str("\n");
}

int main(void) {
    print_str("=== Biology Admin Demo ===\n\n");
    bia_init();

    print_str("Botany...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int pcl = 55 + (i * 13);
        int pph = 40 + (i * 10);
        int pec = 22 + (i * 5);
        int pgn = 15 + (i * 3);
        int year = 2020 + (i % 5);
        bia_botany(type, cat, pcl, pph, pec, pgn, year);
    }

    print_str("\nZoology...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int acl = 48 + (i * 11);
        int aph = 35 + (i * 8);
        int abh = 20 + (i * 4);
        int aec = 12 + (i * 3);
        int year = 2021 + (i % 4);
        bia_zoology(type, cat, acl, aph, abh, aec, year);
    }

    print_str("\nMicrobiology...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int bas = 42 + (i * 10);
        int vis = 28 + (i * 7);
        int fus = 18 + (i * 4);
        int mgn = 10 + (i * 2);
        int year = 2022 + (i % 3);
        bia_microbio(type, cat, bas, vis, fus, mgn, year);
    }

    print_str("\nGenetics...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cgn = 35 + (i * 8);
        int mgn = 25 + (i * 6);
        int pgn = 15 + (i * 3);
        int egn = 10 + (i * 2);
        int year = 2023 + (i % 2);
        bia_genetics(type, cat, cgn, mgn, pgn, egn, year);
    }

    print_str("\nCell biology...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cst = 30 + (i * 7);
        int cmt = 22 + (i * 5);
        int csg = 12 + (i * 3);
        int ccy = 8 + (i * 2);
        int year = 2024;
        bia_cell_bio(type, cat, cst, cmt, csg, ccy, year);
    }

    print_str("\nBotany report...\n");
    bia_botany_report();

    print_str("\nZoology report...\n");
    bia_zoology_report();

    print_str("\nFull report...\n");
    bia_full_report();

    print_str("\nFinal state...\n");
    bia_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
