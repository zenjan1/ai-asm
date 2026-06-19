/* health_economics_admin: Health economics administration system (v1.0)
 * Health economics, health management, health insurance, pharmaceutical economics, health services
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

#define MAX_HEA_ECON   16
#define MAX_HEA_MAN    14
#define MAX_HEA_INS    12
#define MAX_PHA_ECON   10
#define MAX_HEA_SER    10

typedef struct {
    int    he_id;
    int    he_type;
    int    he_category;
    int    hea_exp;
    int    hea_res;
    int    hea_all;
    int    hea_ben;
    int    year;
    int    active;
} hea_econ_t;

typedef struct {
    int    hm_id;
    int    hm_type;
    int    hm_category;
    int    hos_man;
    int    pub_man;
    int    hea_hr;
    int    hea_it;
    int    year;
    int    active;
} hea_man_t;

typedef struct {
    int    hi_id;
    int    hi_type;
    int    hi_category;
    int    ins_sys;
    int    ins_fnd;
    int    ins_pay;
    int    dis_ins;
    int    year;
    int    active;
} hea_ins_t;

typedef struct {
    int    pe_id;
    int    pe_type;
    int    pe_category;
    int    phr_eco;
    int    phr_mkt;
    int    phr_pri;
    int    phr_inn;
    int    year;
    int    active;
} pha_econ_t;

typedef struct {
    int    hs_id;
    int    hs_type;
    int    hs_category;
    int    hea_mgm;
    int    hea_pro;
    int    hea_ind;
    int    hea_big;
    int    year;
    int    active;
} hea_ser_t;

typedef struct {
    int    n_hea_econ;
    int    n_hea_man;
    int    n_hea_ins;
    int    n_pha_econ;
    int    n_hea_ser;
    int    total_hea_exp;
    int    total_hos_man;
    int    total_ins_sys;
    int    total_phr_eco;
    int    total_hea_mgm;
} hea_state_t;

static hea_econ_t hea_econs[MAX_HEA_ECON];
static hea_man_t  hea_mans[MAX_HEA_MAN];
static hea_ins_t  hea_inss[MAX_HEA_INS];
static pha_econ_t pha_econs[MAX_PHA_ECON];
static hea_ser_t  hea_sers[MAX_HEA_SER];
static hea_state_t hea;

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

int hea_init(void) {
    if (initialized) return -1;
    hea.n_hea_econ = 0; hea.n_hea_man = 0; hea.n_hea_ins = 0;
    hea.n_pha_econ = 0; hea.n_hea_ser = 0;
    hea.total_hea_exp = 0; hea.total_hos_man = 0;
    hea.total_ins_sys = 0; hea.total_phr_eco = 0;
    hea.total_hea_mgm = 0;
    for (int i = 0; i < MAX_HEA_ECON; i++) hea_econs[i].active = 0;
    for (int i = 0; i < MAX_HEA_MAN; i++) hea_mans[i].active = 0;
    for (int i = 0; i < MAX_HEA_INS; i++) hea_inss[i].active = 0;
    for (int i = 0; i < MAX_PHA_ECON; i++) pha_econs[i].active = 0;
    for (int i = 0; i < MAX_HEA_SER; i++) hea_sers[i].active = 0;
    initialized = 1;
    print_str("[HEA] Health economics initialized\n");
    return 0;
}

int hea_hea_econ(int he_type, int cat, int exp, int res, int all, int ben, int year) {
    if (hea.n_hea_econ >= MAX_HEA_ECON) return -1;
    hea_econ_t* h = &hea_econs[hea.n_hea_econ];
    h->he_id = hea.n_hea_econ;
    h->he_type = he_type;
    h->he_category = cat;
    h->hea_exp = exp;
    h->hea_res = res;
    h->hea_all = all;
    h->hea_ben = ben;
    h->year = year;
    h->active = 1;
    hea.total_hea_exp += exp;
    hea.n_hea_econ++;
    print_str("[HEA] Hea econ "); print_int(hea.n_hea_econ - 1);
    print_str(" type="); print_int(he_type);
    print_str(" cat="); print_int(cat);
    print_str(" exp="); print_int(exp);
    print_str(" res="); print_int(res);
    print_str(" all="); print_int(all);
    print_str(" ben="); print_int(ben); print_str("\n");
    return hea.n_hea_econ - 1;
}

int hea_hea_man(int hm_type, int cat, int hman, int pman, int hhr, int hit, int year) {
    if (hea.n_hea_man >= MAX_HEA_MAN) return -1;
    hea_man_t* m = &hea_mans[hea.n_hea_man];
    m->hm_id = hea.n_hea_man;
    m->hm_type = hm_type;
    m->hm_category = cat;
    m->hos_man = hman;
    m->pub_man = pman;
    m->hea_hr = hhr;
    m->hea_it = hit;
    m->year = year;
    m->active = 1;
    hea.total_hos_man += hman;
    hea.n_hea_man++;
    print_str("[HEA] Hea man "); print_int(hea.n_hea_man - 1);
    print_str(" type="); print_int(hm_type);
    print_str(" cat="); print_int(cat);
    print_str(" hman="); print_int(hman);
    print_str(" pman="); print_int(pman);
    print_str(" hhr="); print_int(hhr);
    print_str(" hit="); print_int(hit); print_str("\n");
    return hea.n_hea_man - 1;
}

int hea_hea_ins(int hi_type, int cat, int isys, int ifnd, int ipay, int dins, int year) {
    if (hea.n_hea_ins >= MAX_HEA_INS) return -1;
    hea_ins_t* ins = &hea_inss[hea.n_hea_ins];
    ins->hi_id = hea.n_hea_ins;
    ins->hi_type = hi_type;
    ins->hi_category = cat;
    ins->ins_sys = isys;
    ins->ins_fnd = ifnd;
    ins->ins_pay = ipay;
    ins->dis_ins = dins;
    ins->year = year;
    ins->active = 1;
    hea.total_ins_sys += isys;
    hea.n_hea_ins++;
    print_str("[HEA] Hea ins "); print_int(hea.n_hea_ins - 1);
    print_str(" type="); print_int(hi_type);
    print_str(" cat="); print_int(cat);
    print_str(" isys="); print_int(isys);
    print_str(" ifnd="); print_int(ifnd);
    print_str(" ipay="); print_int(ipay);
    print_str(" dins="); print_int(dins); print_str("\n");
    return hea.n_hea_ins - 1;
}

int hea_pha_econ(int pe_type, int cat, int peco, int pmkt, int ppri, int pinn, int year) {
    if (hea.n_pha_econ >= MAX_PHA_ECON) return -1;
    pha_econ_t* p = &pha_econs[hea.n_pha_econ];
    p->pe_id = hea.n_pha_econ;
    p->pe_type = pe_type;
    p->pe_category = cat;
    p->phr_eco = peco;
    p->phr_mkt = pmkt;
    p->phr_pri = ppri;
    p->phr_inn = pinn;
    p->year = year;
    p->active = 1;
    hea.total_phr_eco += peco;
    hea.n_pha_econ++;
    print_str("[HEA] Pha econ "); print_int(hea.n_pha_econ - 1);
    print_str(" type="); print_int(pe_type);
    print_str(" cat="); print_int(cat);
    print_str(" peco="); print_int(peco);
    print_str(" pmkt="); print_int(pmkt);
    print_str(" ppri="); print_int(ppri);
    print_str(" pinn="); print_int(pinn); print_str("\n");
    return hea.n_pha_econ - 1;
}

int hea_hea_ser(int hs_type, int cat, int hmgm, int hpro, int hind, int hbig, int year) {
    if (hea.n_hea_ser >= MAX_HEA_SER) return -1;
    hea_ser_t* s = &hea_sers[hea.n_hea_ser];
    s->hs_id = hea.n_hea_ser;
    s->hs_type = hs_type;
    s->hs_category = cat;
    s->hea_mgm = hmgm;
    s->hea_pro = hpro;
    s->hea_ind = hind;
    s->hea_big = hbig;
    s->year = year;
    s->active = 1;
    hea.total_hea_mgm += hmgm;
    hea.n_hea_ser++;
    print_str("[HEA] Hea ser "); print_int(hea.n_hea_ser - 1);
    print_str(" type="); print_int(hs_type);
    print_str(" cat="); print_int(cat);
    print_str(" hmgm="); print_int(hmgm);
    print_str(" hpro="); print_int(hpro);
    print_str(" hind="); print_int(hind);
    print_str(" hbig="); print_int(hbig); print_str("\n");
    return hea.n_hea_ser - 1;
}

void hea_econ_report(void) {
    print_str("[HEA] Health economics report:\n");
    print_str("  Economics categories: "); print_int(hea.n_hea_econ); print_str("\n");
    print_str("  Total health expenditure: "); print_int(hea.total_hea_exp); print_str("\n");
}

void hea_management_report(void) {
    print_str("[HEA] Health management report:\n");
    print_str("  Management categories: "); print_int(hea.n_hea_man); print_str("\n");
    print_str("  Total hospital management: "); print_int(hea.total_hos_man); print_str("\n");
}

void hea_full_report(void) {
    print_str("[HEA] Full report:\n");
    print_str("  Insurance categories: "); print_int(hea.n_hea_ins); print_str("\n");
    print_str("  Total insurance system: "); print_int(hea.total_ins_sys); print_str("\n");
    print_str("  Pharma categories: "); print_int(hea.n_pha_econ); print_str("\n");
    print_str("  Total pharma economics: "); print_int(hea.total_phr_eco); print_str("\n");
    print_str("  Service categories: "); print_int(hea.n_hea_ser); print_str("\n");
    print_str("  Total health management: "); print_int(hea.total_hea_mgm); print_str("\n");
}

void hea_print_state(void) {
    print_str("[HEA] He="); print_int(hea.n_hea_econ);
    print_str(" Hm="); print_int(hea.n_hea_man);
    print_str(" Hi="); print_int(hea.n_hea_ins);
    print_str(" Pe="); print_int(hea.n_pha_econ);
    print_str(" Hs="); print_int(hea.n_hea_ser);
    print_str("\n");
}

int main(void) {
    print_str("=== Health Economics Admin Demo ===\n\n");
    hea_init();

    print_str("Health economics...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int exp = 58 + (i * 13);
        int res = 42 + (i * 10);
        int all = 24 + (i * 6);
        int ben = 16 + (i * 4);
        int year = 2020 + (i % 5);
        hea_hea_econ(type, cat, exp, res, all, ben, year);
    }

    print_str("\nHealth management...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int hman = 46 + (i * 11);
        int pman = 33 + (i * 8);
        int hhr = 21 + (i * 5);
        int hit = 13 + (i * 3);
        int year = 2021 + (i % 4);
        hea_hea_man(type, cat, hman, pman, hhr, hit, year);
    }

    print_str("\nHealth insurance...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int isys = 40 + (i * 10);
        int ifnd = 28 + (i * 7);
        int ipay = 17 + (i * 4);
        int dins = 11 + (i * 3);
        int year = 2022 + (i % 3);
        hea_hea_ins(type, cat, isys, ifnd, ipay, dins, year);
    }

    print_str("\nPharmaceutical economics...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int peco = 36 + (i * 9);
        int pmkt = 26 + (i * 7);
        int ppri = 15 + (i * 4);
        int pinn = 10 + (i * 3);
        int year = 2023 + (i % 2);
        hea_pha_econ(type, cat, peco, pmkt, ppri, pinn, year);
    }

    print_str("\nHealth services...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int hmgm = 34 + (i * 8);
        int hpro = 25 + (i * 6);
        int hind = 14 + (i * 3);
        int hbig = 9 + (i * 2);
        int year = 2024;
        hea_hea_ser(type, cat, hmgm, hpro, hind, hbig, year);
    }

    print_str("\nEconomics report...\n");
    hea_econ_report();

    print_str("\nManagement report...\n");
    hea_management_report();

    print_str("\nFull report...\n");
    hea_full_report();

    print_str("\nFinal state...\n");
    hea_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
