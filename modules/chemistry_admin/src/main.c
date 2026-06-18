/* chemistry_admin: Chemistry administration system (v1.0)
 * Inorganic chemistry, organic chemistry, physical chemistry, analytical chemistry, applied chemistry
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

#define MAX_INORG       16
#define MAX_ORG         14
#define MAX_PHYS_C      12
#define MAX_ANAL_C      10
#define MAX_APPL_C      10

typedef struct {
    int    in_id;
    int    in_type;
    int    in_category;
    int    coo_chem;
    int    ele_chem;
    int    ino_syn;
    int    sol_ino;
    int    year;
    int    active;
} inorg_t;

typedef struct {
    int    og_id;
    int    og_type;
    int    og_category;
    int    org_syn;
    int    nat_org;
    int    met_org;
    int    poly_chem;
    int    year;
    int    active;
} org_t;

typedef struct {
    int    pc_id;
    int    pc_type;
    int    pc_category;
    int    chem_th;
    int    chem_kin;
    int    qua_chem;
    int    str_chem;
    int    year;
    int    active;
} phys_c_t;

typedef struct {
    int    ac_id;
    int    ac_type;
    int    ac_category;
    int    qua_ana;
    int    qua_ana_q;
    int    ins_ana;
    int    spec_ana;
    int    year;
    int    active;
} anal_c_t;

typedef struct {
    int    ap_id;
    int    ap_type;
    int    ap_category;
    int    chem_eng;
    int    mat_chem;
    int    env_chem;
    int    ene_chem;
    int    year;
    int    active;
} appl_c_t;

typedef struct {
    int    n_inorg;
    int    n_org;
    int    n_phys_c;
    int    n_anal_c;
    int    n_appl_c;
    int    total_coo_chem;
    int    total_org_syn;
    int    total_chem_th;
    int    total_qua_ana;
    int    total_chem_eng;
} cha_state_t;

static inorg_t inorgs[MAX_INORG];
static org_t orgs[MAX_ORG];
static phys_c_t phys_cs[MAX_PHYS_C];
static anal_c_t anal_cs[MAX_ANAL_C];
static appl_c_t appl_cs[MAX_APPL_C];
static cha_state_t cha;

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

int cha_init(void) {
    if (initialized) return -1;
    cha.n_inorg = 0; cha.n_org = 0; cha.n_phys_c = 0;
    cha.n_anal_c = 0; cha.n_appl_c = 0;
    cha.total_coo_chem = 0; cha.total_org_syn = 0;
    cha.total_chem_th = 0; cha.total_qua_ana = 0;
    cha.total_chem_eng = 0;
    for (int i = 0; i < MAX_INORG; i++) inorgs[i].active = 0;
    for (int i = 0; i < MAX_ORG; i++) orgs[i].active = 0;
    for (int i = 0; i < MAX_PHYS_C; i++) phys_cs[i].active = 0;
    for (int i = 0; i < MAX_ANAL_C; i++) anal_cs[i].active = 0;
    for (int i = 0; i < MAX_APPL_C; i++) appl_cs[i].active = 0;
    initialized = 1;
    print_str("[CHA] Chemistry initialized\n");
    return 0;
}

int cha_inorg(int it_type, int cat, int coh, int ech, int isy, int sio, int year) {
    if (cha.n_inorg >= MAX_INORG) return -1;
    inorg_t* n = &inorgs[cha.n_inorg];
    n->in_id = cha.n_inorg;
    n->in_type = it_type;
    n->in_category = cat;
    n->coo_chem = coh;
    n->ele_chem = ech;
    n->ino_syn = isy;
    n->sol_ino = sio;
    n->year = year;
    n->active = 1;
    cha.total_coo_chem += coh;
    cha.n_inorg++;
    print_str("[CHA] Inorg "); print_int(cha.n_inorg - 1);
    print_str(" type="); print_int(it_type);
    print_str(" cat="); print_int(cat);
    print_str(" coh="); print_int(coh);
    print_str(" ech="); print_int(ech);
    print_str(" isy="); print_int(isy);
    print_str(" sio="); print_int(sio); print_str("\n");
    return cha.n_inorg - 1;
}

int cha_org(int ot_type, int cat, int osy, int nog, int mog, int pch, int year) {
    if (cha.n_org >= MAX_ORG) return -1;
    org_t* o = &orgs[cha.n_org];
    o->og_id = cha.n_org;
    o->og_type = ot_type;
    o->og_category = cat;
    o->org_syn = osy;
    o->nat_org = nog;
    o->met_org = mog;
    o->poly_chem = pch;
    o->year = year;
    o->active = 1;
    cha.total_org_syn += osy;
    cha.n_org++;
    print_str("[CHA] Org "); print_int(cha.n_org - 1);
    print_str(" type="); print_int(ot_type);
    print_str(" cat="); print_int(cat);
    print_str(" osy="); print_int(osy);
    print_str(" nog="); print_int(nog);
    print_str(" mog="); print_int(mog);
    print_str(" pch="); print_int(pch); print_str("\n");
    return cha.n_org - 1;
}

int cha_phys_c(int pt_type, int cat, int cht, int chk, int qch, int sch, int year) {
    if (cha.n_phys_c >= MAX_PHYS_C) return -1;
    phys_c_t* p = &phys_cs[cha.n_phys_c];
    p->pc_id = cha.n_phys_c;
    p->pc_type = pt_type;
    p->pc_category = cat;
    p->chem_th = cht;
    p->chem_kin = chk;
    p->qua_chem = qch;
    p->str_chem = sch;
    p->year = year;
    p->active = 1;
    cha.total_chem_th += cht;
    cha.n_phys_c++;
    print_str("[CHA] Phys c "); print_int(cha.n_phys_c - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" cht="); print_int(cht);
    print_str(" chk="); print_int(chk);
    print_str(" qch="); print_int(qch);
    print_str(" sch="); print_int(sch); print_str("\n");
    return cha.n_phys_c - 1;
}

int cha_anal_c(int at_type, int cat, int qan, int qanq, int ins, int spa, int year) {
    if (cha.n_anal_c >= MAX_ANAL_C) return -1;
    anal_c_t* a = &anal_cs[cha.n_anal_c];
    a->ac_id = cha.n_anal_c;
    a->ac_type = at_type;
    a->ac_category = cat;
    a->qua_ana = qan;
    a->qua_ana_q = qanq;
    a->ins_ana = ins;
    a->spec_ana = spa;
    a->year = year;
    a->active = 1;
    cha.total_qua_ana += qan;
    cha.n_anal_c++;
    print_str("[CHA] Anal c "); print_int(cha.n_anal_c - 1);
    print_str(" type="); print_int(at_type);
    print_str(" cat="); print_int(cat);
    print_str(" qan="); print_int(qan);
    print_str(" qanq="); print_int(qanq);
    print_str(" ins="); print_int(ins);
    print_str(" spa="); print_int(spa); print_str("\n");
    return cha.n_anal_c - 1;
}

int cha_appl_c(int at_type, int cat, int cen, int mch, int ech, int ench, int year) {
    if (cha.n_appl_c >= MAX_APPL_C) return -1;
    appl_c_t* a = &appl_cs[cha.n_appl_c];
    a->ap_id = cha.n_appl_c;
    a->ap_type = at_type;
    a->ap_category = cat;
    a->chem_eng = cen;
    a->mat_chem = mch;
    a->env_chem = ech;
    a->ene_chem = ench;
    a->year = year;
    a->active = 1;
    cha.total_chem_eng += cen;
    cha.n_appl_c++;
    print_str("[CHA] Appl c "); print_int(cha.n_appl_c - 1);
    print_str(" type="); print_int(at_type);
    print_str(" cat="); print_int(cat);
    print_str(" cen="); print_int(cen);
    print_str(" mch="); print_int(mch);
    print_str(" ech="); print_int(ech);
    print_str(" ench="); print_int(ench); print_str("\n");
    return cha.n_appl_c - 1;
}

void cha_inorg_report(void) {
    print_str("[CHA] Inorganic chemistry report:\n");
    print_str("  Inorganic categories: "); print_int(cha.n_inorg); print_str("\n");
    print_str("  Total coordination chemistry: "); print_int(cha.total_coo_chem); print_str("\n");
}

void cha_org_report(void) {
    print_str("[CHA] Organic chemistry report:\n");
    print_str("  Organic categories: "); print_int(cha.n_org); print_str("\n");
    print_str("  Total organic synthesis: "); print_int(cha.total_org_syn); print_str("\n");
}

void cha_full_report(void) {
    print_str("[CHA] Full report:\n");
    print_str("  Physical chemistry categories: "); print_int(cha.n_phys_c); print_str("\n");
    print_str("  Total chem thermodynamics: "); print_int(cha.total_chem_th); print_str("\n");
    print_str("  Analytical chemistry categories: "); print_int(cha.n_anal_c); print_str("\n");
    print_str("  Total qualitative analysis: "); print_int(cha.total_qua_ana); print_str("\n");
    print_str("  Applied chemistry categories: "); print_int(cha.n_appl_c); print_str("\n");
    print_str("  Total chemical engineering: "); print_int(cha.total_chem_eng); print_str("\n");
}

void cha_print_state(void) {
    print_str("[CHA] In="); print_int(cha.n_inorg);
    print_str(" Og="); print_int(cha.n_org);
    print_str(" Pc="); print_int(cha.n_phys_c);
    print_str(" Ac="); print_int(cha.n_anal_c);
    print_str(" Ap="); print_int(cha.n_appl_c);
    print_str("\n");
}

int main(void) {
    print_str("=== Chemistry Admin Demo ===\n\n");
    cha_init();

    print_str("Inorganic chemistry...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int coh = 55 + (i * 13);
        int ech = 40 + (i * 10);
        int isy = 22 + (i * 5);
        int sio = 15 + (i * 3);
        int year = 2020 + (i % 5);
        cha_inorg(type, cat, coh, ech, isy, sio, year);
    }

    print_str("\nOrganic chemistry...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int osy = 48 + (i * 11);
        int nog = 35 + (i * 8);
        int mog = 20 + (i * 4);
        int pch = 12 + (i * 3);
        int year = 2021 + (i % 4);
        cha_org(type, cat, osy, nog, mog, pch, year);
    }

    print_str("\nPhysical chemistry...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cht = 42 + (i * 10);
        int chk = 28 + (i * 7);
        int qch = 18 + (i * 4);
        int sch = 10 + (i * 2);
        int year = 2022 + (i % 3);
        cha_phys_c(type, cat, cht, chk, qch, sch, year);
    }

    print_str("\nAnalytical chemistry...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int qan = 35 + (i * 8);
        int qanq = 25 + (i * 6);
        int ins = 15 + (i * 3);
        int spa = 10 + (i * 2);
        int year = 2023 + (i % 2);
        cha_anal_c(type, cat, qan, qanq, ins, spa, year);
    }

    print_str("\nApplied chemistry...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cen = 30 + (i * 7);
        int mch = 22 + (i * 5);
        int ech = 12 + (i * 3);
        int ench = 8 + (i * 2);
        int year = 2024;
        cha_appl_c(type, cat, cen, mch, ech, ench, year);
    }

    print_str("\nInorganic report...\n");
    cha_inorg_report();

    print_str("\nOrganic report...\n");
    cha_org_report();

    print_str("\nFull report...\n");
    cha_full_report();

    print_str("\nFinal state...\n");
    cha_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
