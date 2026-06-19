/* industrial_economics_admin: Industrial economics administration system (v1.0)
 * Industrial organization, industrial development, enterprise economics, industrial policy, technology economics
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

#define MAX_IND_ORG   16
#define MAX_IND_DEV   14
#define MAX_ENT_ECON  12
#define MAX_IND_POL   10
#define MAX_TEC_ECON  10

typedef struct {
    int    io_id;
    int    io_type;
    int    io_category;
    int    mkt_str;
    int    com_pol;
    int    ant_mon;
    int    mkt_con;
    int    year;
    int    active;
} ind_org_t;

typedef struct {
    int    id_id;
    int    id_type;
    int    id_category;
    int    ind_str;
    int    ind_lnk;
    int    ind_chn;
    int    ind_cls;
    int    year;
    int    active;
} ind_dev_t;

typedef struct {
    int    ee_id;
    int    ee_type;
    int    ee_category;
    int    ent_the;
    int    ent_gov;
    int    ent_str;
    int    ent_inn;
    int    year;
    int    active;
} ent_econ_t;

typedef struct {
    int    ip_id;
    int    ip_type;
    int    ip_category;
    int    pol_mak;
    int    pol_sup;
    int    pol_adj;
    int    pol_sec;
    int    year;
    int    active;
} ind_pol_t;

typedef struct {
    int    te_id;
    int    te_type;
    int    te_category;
    int    tec_pro;
    int    tec_dif;
    int    rnd_eco;
    int    ipr_eco;
    int    year;
    int    active;
} tec_econ_t;

typedef struct {
    int    n_ind_org;
    int    n_ind_dev;
    int    n_ent_econ;
    int    n_ind_pol;
    int    n_tec_econ;
    int    total_mkt_str;
    int    total_ind_str;
    int    total_ent_the;
    int    total_pol_mak;
    int    total_tec_pro;
} ie_state_t;

static ind_org_t ind_orgs[MAX_IND_ORG];
static ind_dev_t ind_devs[MAX_IND_DEV];
static ent_econ_t ent_econs[MAX_ENT_ECON];
static ind_pol_t ind_pols[MAX_IND_POL];
static tec_econ_t tec_econs[MAX_TEC_ECON];
static ie_state_t ie;

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

int ie_init(void) {
    if (initialized) return -1;
    ie.n_ind_org = 0; ie.n_ind_dev = 0; ie.n_ent_econ = 0;
    ie.n_ind_pol = 0; ie.n_tec_econ = 0;
    ie.total_mkt_str = 0; ie.total_ind_str = 0;
    ie.total_ent_the = 0; ie.total_pol_mak = 0;
    ie.total_tec_pro = 0;
    for (int i = 0; i < MAX_IND_ORG; i++) ind_orgs[i].active = 0;
    for (int i = 0; i < MAX_IND_DEV; i++) ind_devs[i].active = 0;
    for (int i = 0; i < MAX_ENT_ECON; i++) ent_econs[i].active = 0;
    for (int i = 0; i < MAX_IND_POL; i++) ind_pols[i].active = 0;
    for (int i = 0; i < MAX_TEC_ECON; i++) tec_econs[i].active = 0;
    initialized = 1;
    print_str("[IE] Industrial economics initialized\n");
    return 0;
}

int ie_ind_org(int io_type, int cat, int mstr, int cpol, int amon, int mcon, int year) {
    if (ie.n_ind_org >= MAX_IND_ORG) return -1;
    ind_org_t* o = &ind_orgs[ie.n_ind_org];
    o->io_id = ie.n_ind_org; o->io_type = io_type; o->io_category = cat;
    o->mkt_str = mstr; o->com_pol = cpol; o->ant_mon = amon; o->mkt_con = mcon;
    o->year = year; o->active = 1;
    ie.total_mkt_str += mstr; ie.n_ind_org++;
    print_str("[IE] Ind org "); print_int(ie.n_ind_org - 1);
    print_str(" type="); print_int(io_type); print_str(" cat="); print_int(cat);
    print_str(" mstr="); print_int(mstr); print_str(" cpol="); print_int(cpol);
    print_str(" amon="); print_int(amon); print_str(" mcon="); print_int(mcon); print_str("\n");
    return ie.n_ind_org - 1;
}

int ie_ind_dev(int id_type, int cat, int istr, int ilnk, int ichn, int icls, int year) {
    if (ie.n_ind_dev >= MAX_IND_DEV) return -1;
    ind_dev_t* d = &ind_devs[ie.n_ind_dev];
    d->id_id = ie.n_ind_dev; d->id_type = id_type; d->id_category = cat;
    d->ind_str = istr; d->ind_lnk = ilnk; d->ind_chn = ichn; d->ind_cls = icls;
    d->year = year; d->active = 1;
    ie.total_ind_str += istr; ie.n_ind_dev++;
    print_str("[IE] Ind dev "); print_int(ie.n_ind_dev - 1);
    print_str(" type="); print_int(id_type); print_str(" cat="); print_int(cat);
    print_str(" istr="); print_int(istr); print_str(" ilnk="); print_int(ilnk);
    print_str(" ichn="); print_int(ichn); print_str(" icls="); print_int(icls); print_str("\n");
    return ie.n_ind_dev - 1;
}

int ie_ent_econ(int ee_type, int cat, int ethe, int egov, int estr, int einn, int year) {
    if (ie.n_ent_econ >= MAX_ENT_ECON) return -1;
    ent_econ_t* e = &ent_econs[ie.n_ent_econ];
    e->ee_id = ie.n_ent_econ; e->ee_type = ee_type; e->ee_category = cat;
    e->ent_the = ethe; e->ent_gov = egov; e->ent_str = estr; e->ent_inn = einn;
    e->year = year; e->active = 1;
    ie.total_ent_the += ethe; ie.n_ent_econ++;
    print_str("[IE] Ent econ "); print_int(ie.n_ent_econ - 1);
    print_str(" type="); print_int(ee_type); print_str(" cat="); print_int(cat);
    print_str(" ethe="); print_int(ethe); print_str(" egov="); print_int(egov);
    print_str(" estr="); print_int(estr); print_str(" einn="); print_int(einn); print_str("\n");
    return ie.n_ent_econ - 1;
}

int ie_ind_pol(int ip_type, int cat, int pmak, int psup, int padj, int psec, int year) {
    if (ie.n_ind_pol >= MAX_IND_POL) return -1;
    ind_pol_t* p = &ind_pols[ie.n_ind_pol];
    p->ip_id = ie.n_ind_pol; p->ip_type = ip_type; p->ip_category = cat;
    p->pol_mak = pmak; p->pol_sup = psup; p->pol_adj = padj; p->pol_sec = psec;
    p->year = year; p->active = 1;
    ie.total_pol_mak += pmak; ie.n_ind_pol++;
    print_str("[IE] Ind pol "); print_int(ie.n_ind_pol - 1);
    print_str(" type="); print_int(ip_type); print_str(" cat="); print_int(cat);
    print_str(" pmak="); print_int(pmak); print_str(" psup="); print_int(psup);
    print_str(" padj="); print_int(padj); print_str(" psec="); print_int(psec); print_str("\n");
    return ie.n_ind_pol - 1;
}

int ie_tec_econ(int te_type, int cat, int tpro, int tdif, int rndeco, int ipeco, int year) {
    if (ie.n_tec_econ >= MAX_TEC_ECON) return -1;
    tec_econ_t* t = &tec_econs[ie.n_tec_econ];
    t->te_id = ie.n_tec_econ; t->te_type = te_type; t->te_category = cat;
    t->tec_pro = tpro; t->tec_dif = tdif; t->rnd_eco = rndeco; t->ipr_eco = ipeco;
    t->year = year; t->active = 1;
    ie.total_tec_pro += tpro; ie.n_tec_econ++;
    print_str("[IE] Tec econ "); print_int(ie.n_tec_econ - 1);
    print_str(" type="); print_int(te_type); print_str(" cat="); print_int(cat);
    print_str(" tpro="); print_int(tpro); print_str(" tdif="); print_int(tdif);
    print_str(" rndeco="); print_int(rndeco); print_str(" ipeco="); print_int(ipeco); print_str("\n");
    return ie.n_tec_econ - 1;
}

void ie_org_report(void) {
    print_str("[IE] Industrial organization report:\n");
    print_str("  Organization categories: "); print_int(ie.n_ind_org); print_str("\n");
    print_str("  Total market structure: "); print_int(ie.total_mkt_str); print_str("\n");
}

void ie_dev_report(void) {
    print_str("[IE] Industrial development report:\n");
    print_str("  Development categories: "); print_int(ie.n_ind_dev); print_str("\n");
    print_str("  Total industrial structure: "); print_int(ie.total_ind_str); print_str("\n");
}

void ie_full_report(void) {
    print_str("[IE] Full report:\n");
    print_str("  Enterprise categories: "); print_int(ie.n_ent_econ); print_str("\n");
    print_str("  Total enterprise theory: "); print_int(ie.total_ent_the); print_str("\n");
    print_str("  Policy categories: "); print_int(ie.n_ind_pol); print_str("\n");
    print_str("  Total policy making: "); print_int(ie.total_pol_mak); print_str("\n");
    print_str("  Technology categories: "); print_int(ie.n_tec_econ); print_str("\n");
    print_str("  Total tech progress: "); print_int(ie.total_tec_pro); print_str("\n");
}

void ie_print_state(void) {
    print_str("[IE] Io="); print_int(ie.n_ind_org);
    print_str(" Id="); print_int(ie.n_ind_dev);
    print_str(" Ee="); print_int(ie.n_ent_econ);
    print_str(" Ip="); print_int(ie.n_ind_pol);
    print_str(" Te="); print_int(ie.n_tec_econ);
    print_str("\n");
}

int main(void) {
    print_str("=== Industrial Economics Admin Demo ===\n\n");
    ie_init();

    print_str("Industrial organization...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1; int cat = (i % 4) + 1;
        int mstr = 64 + (i * 14); int cpol = 48 + (i * 11);
        int amon = 26 + (i * 6); int mcon = 19 + (i * 4);
        int year = 2020 + (i % 5);
        ie_ind_org(type, cat, mstr, cpol, amon, mcon, year);
    }

    print_str("\nIndustrial development...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1; int cat = (i % 5) + 1;
        int istr = 54 + (i * 12); int ilnk = 40 + (i * 9);
        int ichn = 23 + (i * 5); int icls = 15 + (i * 3);
        int year = 2021 + (i % 4);
        ie_ind_dev(type, cat, istr, ilnk, ichn, icls, year);
    }

    print_str("\nEnterprise economics...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1; int cat = (i % 5) + 1;
        int ethe = 46 + (i * 11); int egov = 32 + (i * 8);
        int estr = 21 + (i * 5); int einn = 13 + (i * 3);
        int year = 2022 + (i % 3);
        ie_ent_econ(type, cat, ethe, egov, estr, einn, year);
    }

    print_str("\nIndustrial policy...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1; int cat = (i % 5) + 1;
        int pmak = 40 + (i * 9); int psup = 29 + (i * 7);
        int padj = 17 + (i * 4); int psec = 12 + (i * 3);
        int year = 2023 + (i % 2);
        ie_ind_pol(type, cat, pmak, psup, padj, psec, year);
    }

    print_str("\nTechnology economics...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1; int cat = (i % 5) + 1;
        int tpro = 36 + (i * 8); int tdif = 27 + (i * 6);
        int rndeco = 16 + (i * 4); int ipeco = 10 + (i * 2);
        int year = 2024;
        ie_tec_econ(type, cat, tpro, tdif, rndeco, ipeco, year);
    }

    print_str("\nOrganization report...\n"); ie_org_report();
    print_str("\nDevelopment report...\n"); ie_dev_report();
    print_str("\nFull report...\n"); ie_full_report();
    print_str("\nFinal state...\n"); ie_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
