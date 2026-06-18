/* real_estate_admin: Real estate administration system (v1.0)
 * Real estate development, property management, appraisal, housing policy, market
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

#define MAX_REA_DEV    16
#define MAX_PRO_MAN    14
#define MAX_REA_APP    12
#define MAX_HOU_POL    10
#define MAX_REA_MKT    10

typedef struct {
    int    rd_id;
    int    rd_type;
    int    rd_category;
    int    lan_acq;
    int    pro_pla;
    int    dev_con;
    int    pro_man;
    int    year;
    int    active;
} rea_dev_t;

typedef struct {
    int    pm_id;
    int    pm_type;
    int    pm_category;
    int    pro_ser;
    int    fac_mai;
    int    com_man;
    int    own_rel;
    int    year;
    int    active;
} pro_man_t;

typedef struct {
    int    ra_id;
    int    ra_type;
    int    ra_category;
    int    mkt_ana;
    int    val_app;
    int    inv_ana;
    int    risk_ana;
    int    year;
    int    active;
} rea_app_t;

typedef struct {
    int    hp_id;
    int    hp_type;
    int    hp_category;
    int    saf_hou;
    int    hou_sub;
    int    pro_fun;
    int    hou_fin;
    int    year;
    int    active;
} hou_pol_t;

typedef struct {
    int    rm_id;
    int    rm_type;
    int    rm_category;
    int    mkt_res;
    int    mar_pla;
    int    tra_man;
    int    pro_reg;
    int    year;
    int    active;
} rea_mkt_t;

typedef struct {
    int    n_rea_dev;
    int    n_pro_man;
    int    n_rea_app;
    int    n_hou_pol;
    int    n_rea_mkt;
    int    total_lan_acq;
    int    total_pro_ser;
    int    total_mkt_ana;
    int    total_saf_hou;
    int    total_mkt_res;
} rea_state_t;

static rea_dev_t rea_devs[MAX_REA_DEV];
static pro_man_t pro_mans[MAX_PRO_MAN];
static rea_app_t rea_apps[MAX_REA_APP];
static hou_pol_t hou_pols[MAX_HOU_POL];
static rea_mkt_t rea_mkts[MAX_REA_MKT];
static rea_state_t rea;

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

int rea_init(void) {
    if (initialized) return -1;
    rea.n_rea_dev = 0; rea.n_pro_man = 0; rea.n_rea_app = 0;
    rea.n_hou_pol = 0; rea.n_rea_mkt = 0;
    rea.total_lan_acq = 0; rea.total_pro_ser = 0;
    rea.total_mkt_ana = 0; rea.total_saf_hou = 0;
    rea.total_mkt_res = 0;
    for (int i = 0; i < MAX_REA_DEV; i++) rea_devs[i].active = 0;
    for (int i = 0; i < MAX_PRO_MAN; i++) pro_mans[i].active = 0;
    for (int i = 0; i < MAX_REA_APP; i++) rea_apps[i].active = 0;
    for (int i = 0; i < MAX_HOU_POL; i++) hou_pols[i].active = 0;
    for (int i = 0; i < MAX_REA_MKT; i++) rea_mkts[i].active = 0;
    initialized = 1;
    print_str("[REA] Real estate initialized\n");
    return 0;
}

int rea_rea_dev(int rt_type, int cat, int lacq, int ppla, int dcon, int pman, int year) {
    if (rea.n_rea_dev >= MAX_REA_DEV) return -1;
    rea_dev_t* r = &rea_devs[rea.n_rea_dev];
    r->rd_id = rea.n_rea_dev;
    r->rd_type = rt_type;
    r->rd_category = cat;
    r->lan_acq = lacq;
    r->pro_pla = ppla;
    r->dev_con = dcon;
    r->pro_man = pman;
    r->year = year;
    r->active = 1;
    rea.total_lan_acq += lacq;
    rea.n_rea_dev++;
    print_str("[REA] Rea dev "); print_int(rea.n_rea_dev - 1);
    print_str(" type="); print_int(rt_type);
    print_str(" cat="); print_int(cat);
    print_str(" lacq="); print_int(lacq);
    print_str(" ppla="); print_int(ppla);
    print_str(" dcon="); print_int(dcon);
    print_str(" pman="); print_int(pman); print_str("\n");
    return rea.n_rea_dev - 1;
}

int rea_pro_man(int pt_type, int cat, int pser, int fmai, int cman, int orel, int year) {
    if (rea.n_pro_man >= MAX_PRO_MAN) return -1;
    pro_man_t* p = &pro_mans[rea.n_pro_man];
    p->pm_id = rea.n_pro_man;
    p->pm_type = pt_type;
    p->pm_category = cat;
    p->pro_ser = pser;
    p->fac_mai = fmai;
    p->com_man = cman;
    p->own_rel = orel;
    p->year = year;
    p->active = 1;
    rea.total_pro_ser += pser;
    rea.n_pro_man++;
    print_str("[REA] Pro man "); print_int(rea.n_pro_man - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" pser="); print_int(pser);
    print_str(" fmai="); print_int(fmai);
    print_str(" cman="); print_int(cman);
    print_str(" orel="); print_int(orel); print_str("\n");
    return rea.n_pro_man - 1;
}

int rea_rea_app(int at_type, int cat, int mana, int vapp, int iana, int rana, int year) {
    if (rea.n_rea_app >= MAX_REA_APP) return -1;
    rea_app_t* a = &rea_apps[rea.n_rea_app];
    a->ra_id = rea.n_rea_app;
    a->ra_type = at_type;
    a->ra_category = cat;
    a->mkt_ana = mana;
    a->val_app = vapp;
    a->inv_ana = iana;
    a->risk_ana = rana;
    a->year = year;
    a->active = 1;
    rea.total_mkt_ana += mana;
    rea.n_rea_app++;
    print_str("[REA] Rea app "); print_int(rea.n_rea_app - 1);
    print_str(" type="); print_int(at_type);
    print_str(" cat="); print_int(cat);
    print_str(" mana="); print_int(mana);
    print_str(" vapp="); print_int(vapp);
    print_str(" iana="); print_int(iana);
    print_str(" rana="); print_int(rana); print_str("\n");
    return rea.n_rea_app - 1;
}

int rea_hou_pol(int ht_type, int cat, int shou, int hsub, int pfun, int hfin, int year) {
    if (rea.n_hou_pol >= MAX_HOU_POL) return -1;
    hou_pol_t* h = &hou_pols[rea.n_hou_pol];
    h->hp_id = rea.n_hou_pol;
    h->hp_type = ht_type;
    h->hp_category = cat;
    h->saf_hou = shou;
    h->hou_sub = hsub;
    h->pro_fun = pfun;
    h->hou_fin = hfin;
    h->year = year;
    h->active = 1;
    rea.total_saf_hou += shou;
    rea.n_hou_pol++;
    print_str("[REA] Hou pol "); print_int(rea.n_hou_pol - 1);
    print_str(" type="); print_int(ht_type);
    print_str(" cat="); print_int(cat);
    print_str(" shou="); print_int(shou);
    print_str(" hsub="); print_int(hsub);
    print_str(" pfun="); print_int(pfun);
    print_str(" hfin="); print_int(hfin); print_str("\n");
    return rea.n_hou_pol - 1;
}

int rea_rea_mkt(int mt_type, int cat, int mres, int mpla, int tman, int preg, int year) {
    if (rea.n_rea_mkt >= MAX_REA_MKT) return -1;
    rea_mkt_t* m = &rea_mkts[rea.n_rea_mkt];
    m->rm_id = rea.n_rea_mkt;
    m->rm_type = mt_type;
    m->rm_category = cat;
    m->mkt_res = mres;
    m->mar_pla = mpla;
    m->tra_man = tman;
    m->pro_reg = preg;
    m->year = year;
    m->active = 1;
    rea.total_mkt_res += mres;
    rea.n_rea_mkt++;
    print_str("[REA] Rea mkt "); print_int(rea.n_rea_mkt - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" mres="); print_int(mres);
    print_str(" mpla="); print_int(mpla);
    print_str(" tman="); print_int(tman);
    print_str(" preg="); print_int(preg); print_str("\n");
    return rea.n_rea_mkt - 1;
}

void rea_development_report(void) {
    print_str("[REA] Development report:\n");
    print_str("  Development categories: "); print_int(rea.n_rea_dev); print_str("\n");
    print_str("  Total land acquisition: "); print_int(rea.total_lan_acq); print_str("\n");
}

void rea_property_report(void) {
    print_str("[REA] Property management report:\n");
    print_str("  Property categories: "); print_int(rea.n_pro_man); print_str("\n");
    print_str("  Total property service: "); print_int(rea.total_pro_ser); print_str("\n");
}

void rea_full_report(void) {
    print_str("[REA] Full report:\n");
    print_str("  Appraisal categories: "); print_int(rea.n_rea_app); print_str("\n");
    print_str("  Total market analysis: "); print_int(rea.total_mkt_ana); print_str("\n");
    print_str("  Housing policy categories: "); print_int(rea.n_hou_pol); print_str("\n");
    print_str("  Total safe housing: "); print_int(rea.total_saf_hou); print_str("\n");
    print_str("  Market categories: "); print_int(rea.n_rea_mkt); print_str("\n");
    print_str("  Total market research: "); print_int(rea.total_mkt_res); print_str("\n");
}

void rea_print_state(void) {
    print_str("[REA] Rd="); print_int(rea.n_rea_dev);
    print_str(" Pm="); print_int(rea.n_pro_man);
    print_str(" Ra="); print_int(rea.n_rea_app);
    print_str(" Hp="); print_int(rea.n_hou_pol);
    print_str(" Rm="); print_int(rea.n_rea_mkt);
    print_str("\n");
}

int main(void) {
    print_str("=== Real Estate Admin Demo ===\n\n");
    rea_init();

    print_str("Real estate development...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int lacq = 55 + (i * 13);
        int ppla = 40 + (i * 10);
        int dcon = 22 + (i * 5);
        int pman = 15 + (i * 3);
        int year = 2020 + (i % 5);
        rea_rea_dev(type, cat, lacq, ppla, dcon, pman, year);
    }

    print_str("\nProperty management...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pser = 48 + (i * 11);
        int fmai = 35 + (i * 8);
        int cman = 20 + (i * 4);
        int orel = 12 + (i * 3);
        int year = 2021 + (i % 4);
        rea_pro_man(type, cat, pser, fmai, cman, orel, year);
    }

    print_str("\nReal estate appraisal...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int mana = 42 + (i * 10);
        int vapp = 28 + (i * 7);
        int iana = 18 + (i * 4);
        int rana = 10 + (i * 2);
        int year = 2022 + (i % 3);
        rea_rea_app(type, cat, mana, vapp, iana, rana, year);
    }

    print_str("\nHousing policy...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int shou = 35 + (i * 8);
        int hsub = 25 + (i * 6);
        int pfun = 15 + (i * 3);
        int hfin = 10 + (i * 2);
        int year = 2023 + (i % 2);
        rea_hou_pol(type, cat, shou, hsub, pfun, hfin, year);
    }

    print_str("\nReal estate market...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int mres = 30 + (i * 7);
        int mpla = 22 + (i * 5);
        int tman = 12 + (i * 3);
        int preg = 8 + (i * 2);
        int year = 2024;
        rea_rea_mkt(type, cat, mres, mpla, tman, preg, year);
    }

    print_str("\nDevelopment report...\n");
    rea_development_report();

    print_str("\nProperty report...\n");
    rea_property_report();

    print_str("\nFull report...\n");
    rea_full_report();

    print_str("\nFinal state...\n");
    rea_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
