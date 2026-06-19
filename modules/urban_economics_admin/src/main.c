/* urban_economics_admin: Urban economics administration system (v1.0)
 * Urban economics, land economics, housing economics, planning economics, regional development
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

#define MAX_URB_ECON   16
#define MAX_LAN_ECON   14
#define MAX_HOU_ECON   12
#define MAX_PLA_ECON   10
#define MAX_REG_DEV    10

typedef struct {
    int    ue_id;
    int    ue_type;
    int    ue_category;
    int    urb_eco;
    int    urb_urb;
    int    urb_grw;
    int    urb_scl;
    int    year;
    int    active;
} urb_econ_t;

typedef struct {
    int    le_id;
    int    le_type;
    int    le_category;
    int    lnd_eco;
    int    lnd_mkt;
    int    lnd_use;
    int    lnd_pri;
    int    year;
    int    active;
} lan_econ_t;

typedef struct {
    int    he_id;
    int    he_type;
    int    he_category;
    int    hou_mkt;
    int    hou_pol;
    int    hou_sup;
    int    hou_fin;
    int    year;
    int    active;
} hou_econ_t;

typedef struct {
    int    pe_id;
    int    pe_type;
    int    pe_category;
    int    pln_eco;
    int    inf_eco;
    int    pub_svc;
    int    tra_eco;
    int    year;
    int    active;
} pla_econ_t;

typedef struct {
    int    rd_id;
    int    rd_type;
    int    rd_category;
    int    reg_eco;
    int    urb_rur;
    int    reg_cor;
    int    urb_grp;
    int    year;
    int    active;
} reg_dev_t;

typedef struct {
    int    n_urb_econ;
    int    n_lan_econ;
    int    n_hou_econ;
    int    n_pla_econ;
    int    n_reg_dev;
    int    total_urb_eco;
    int    total_lnd_eco;
    int    total_hou_mkt;
    int    total_pln_eco;
    int    total_reg_eco;
} ue_state_t;

static urb_econ_t urb_econs[MAX_URB_ECON];
static lan_econ_t lan_econs[MAX_LAN_ECON];
static hou_econ_t hou_econs[MAX_HOU_ECON];
static pla_econ_t pla_econs[MAX_PLA_ECON];
static reg_dev_t  reg_devs[MAX_REG_DEV];
static ue_state_t ue;

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

int ue_init(void) {
    if (initialized) return -1;
    ue.n_urb_econ = 0; ue.n_lan_econ = 0; ue.n_hou_econ = 0;
    ue.n_pla_econ = 0; ue.n_reg_dev = 0;
    ue.total_urb_eco = 0; ue.total_lnd_eco = 0;
    ue.total_hou_mkt = 0; ue.total_pln_eco = 0;
    ue.total_reg_eco = 0;
    for (int i = 0; i < MAX_URB_ECON; i++) urb_econs[i].active = 0;
    for (int i = 0; i < MAX_LAN_ECON; i++) lan_econs[i].active = 0;
    for (int i = 0; i < MAX_HOU_ECON; i++) hou_econs[i].active = 0;
    for (int i = 0; i < MAX_PLA_ECON; i++) pla_econs[i].active = 0;
    for (int i = 0; i < MAX_REG_DEV; i++) reg_devs[i].active = 0;
    initialized = 1;
    print_str("[UE] Urban economics initialized\n");
    return 0;
}

int ue_urb_econ(int ue_type, int cat, int eco, int urb, int grw, int scl, int year) {
    if (ue.n_urb_econ >= MAX_URB_ECON) return -1;
    urb_econ_t* u = &urb_econs[ue.n_urb_econ];
    u->ue_id = ue.n_urb_econ; u->ue_type = ue_type; u->ue_category = cat;
    u->urb_eco = eco; u->urb_urb = urb; u->urb_grw = grw; u->urb_scl = scl;
    u->year = year; u->active = 1;
    ue.total_urb_eco += eco; ue.n_urb_econ++;
    print_str("[UE] Urb econ "); print_int(ue.n_urb_econ - 1);
    print_str(" type="); print_int(ue_type); print_str(" cat="); print_int(cat);
    print_str(" eco="); print_int(eco); print_str(" urb="); print_int(urb);
    print_str(" grw="); print_int(grw); print_str(" scl="); print_int(scl); print_str("\n");
    return ue.n_urb_econ - 1;
}

int ue_lan_econ(int le_type, int cat, int eco, int mkt, int use, int pri, int year) {
    if (ue.n_lan_econ >= MAX_LAN_ECON) return -1;
    lan_econ_t* l = &lan_econs[ue.n_lan_econ];
    l->le_id = ue.n_lan_econ; l->le_type = le_type; l->le_category = cat;
    l->lnd_eco = eco; l->lnd_mkt = mkt; l->lnd_use = use; l->lnd_pri = pri;
    l->year = year; l->active = 1;
    ue.total_lnd_eco += eco; ue.n_lan_econ++;
    print_str("[UE] Lan econ "); print_int(ue.n_lan_econ - 1);
    print_str(" type="); print_int(le_type); print_str(" cat="); print_int(cat);
    print_str(" eco="); print_int(eco); print_str(" mkt="); print_int(mkt);
    print_str(" use="); print_int(use); print_str(" pri="); print_int(pri); print_str("\n");
    return ue.n_lan_econ - 1;
}

int ue_hou_econ(int he_type, int cat, int hmkt, int hpol, int hsup, int hfin, int year) {
    if (ue.n_hou_econ >= MAX_HOU_ECON) return -1;
    hou_econ_t* h = &hou_econs[ue.n_hou_econ];
    h->he_id = ue.n_hou_econ; h->he_type = he_type; h->he_category = cat;
    h->hou_mkt = hmkt; h->hou_pol = hpol; h->hou_sup = hsup; h->hou_fin = hfin;
    h->year = year; h->active = 1;
    ue.total_hou_mkt += hmkt; ue.n_hou_econ++;
    print_str("[UE] Hou econ "); print_int(ue.n_hou_econ - 1);
    print_str(" type="); print_int(he_type); print_str(" cat="); print_int(cat);
    print_str(" hmkt="); print_int(hmkt); print_str(" hpol="); print_int(hpol);
    print_str(" hsup="); print_int(hsup); print_str(" hfin="); print_int(hfin); print_str("\n");
    return ue.n_hou_econ - 1;
}

int ue_pla_econ(int pe_type, int cat, int eco, int inf, int pub, int tra, int year) {
    if (ue.n_pla_econ >= MAX_PLA_ECON) return -1;
    pla_econ_t* p = &pla_econs[ue.n_pla_econ];
    p->pe_id = ue.n_pla_econ; p->pe_type = pe_type; p->pe_category = cat;
    p->pln_eco = eco; p->inf_eco = inf; p->pub_svc = pub; p->tra_eco = tra;
    p->year = year; p->active = 1;
    ue.total_pln_eco += eco; ue.n_pla_econ++;
    print_str("[UE] Pla econ "); print_int(ue.n_pla_econ - 1);
    print_str(" type="); print_int(pe_type); print_str(" cat="); print_int(cat);
    print_str(" eco="); print_int(eco); print_str(" inf="); print_int(inf);
    print_str(" pub="); print_int(pub); print_str(" tra="); print_int(tra); print_str("\n");
    return ue.n_pla_econ - 1;
}

int ue_reg_dev(int rd_type, int cat, int eco, int rur, int cor, int grp, int year) {
    if (ue.n_reg_dev >= MAX_REG_DEV) return -1;
    reg_dev_t* r = &reg_devs[ue.n_reg_dev];
    r->rd_id = ue.n_reg_dev; r->rd_type = rd_type; r->rd_category = cat;
    r->reg_eco = eco; r->urb_rur = rur; r->reg_cor = cor; r->urb_grp = grp;
    r->year = year; r->active = 1;
    ue.total_reg_eco += eco; ue.n_reg_dev++;
    print_str("[UE] Reg dev "); print_int(ue.n_reg_dev - 1);
    print_str(" type="); print_int(rd_type); print_str(" cat="); print_int(cat);
    print_str(" eco="); print_int(eco); print_str(" rur="); print_int(rur);
    print_str(" cor="); print_int(cor); print_str(" grp="); print_int(grp); print_str("\n");
    return ue.n_reg_dev - 1;
}

void ue_urban_report(void) {
    print_str("[UE] Urban economics report:\n");
    print_str("  Urban categories: "); print_int(ue.n_urb_econ); print_str("\n");
    print_str("  Total urban economics: "); print_int(ue.total_urb_eco); print_str("\n");
}

void ue_land_report(void) {
    print_str("[UE] Land economics report:\n");
    print_str("  Land categories: "); print_int(ue.n_lan_econ); print_str("\n");
    print_str("  Total land economics: "); print_int(ue.total_lnd_eco); print_str("\n");
}

void ue_full_report(void) {
    print_str("[UE] Full report:\n");
    print_str("  Housing categories: "); print_int(ue.n_hou_econ); print_str("\n");
    print_str("  Total housing market: "); print_int(ue.total_hou_mkt); print_str("\n");
    print_str("  Planning categories: "); print_int(ue.n_pla_econ); print_str("\n");
    print_str("  Total planning economics: "); print_int(ue.total_pln_eco); print_str("\n");
    print_str("  Regional categories: "); print_int(ue.n_reg_dev); print_str("\n");
    print_str("  Total regional economics: "); print_int(ue.total_reg_eco); print_str("\n");
}

void ue_print_state(void) {
    print_str("[UE] Ue="); print_int(ue.n_urb_econ);
    print_str(" Le="); print_int(ue.n_lan_econ);
    print_str(" He="); print_int(ue.n_hou_econ);
    print_str(" Pe="); print_int(ue.n_pla_econ);
    print_str(" Rd="); print_int(ue.n_reg_dev);
    print_str("\n");
}

int main(void) {
    print_str("=== Urban Economics Admin Demo ===\n\n");
    ue_init();

    print_str("Urban economics...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1; int cat = (i % 4) + 1;
        int eco = 66 + (i * 15); int urb = 50 + (i * 12);
        int grw = 28 + (i * 7); int scl = 20 + (i * 5);
        int year = 2020 + (i % 5);
        ue_urb_econ(type, cat, eco, urb, grw, scl, year);
    }

    print_str("\nLand economics...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1; int cat = (i % 5) + 1;
        int eco = 56 + (i * 13); int mkt = 42 + (i * 10);
        int use = 24 + (i * 6); int pri = 16 + (i * 4);
        int year = 2021 + (i % 4);
        ue_lan_econ(type, cat, eco, mkt, use, pri, year);
    }

    print_str("\nHousing economics...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1; int cat = (i % 5) + 1;
        int hmkt = 48 + (i * 11); int hpol = 34 + (i * 8);
        int hsup = 22 + (i * 5); int hfin = 14 + (i * 3);
        int year = 2022 + (i % 3);
        ue_hou_econ(type, cat, hmkt, hpol, hsup, hfin, year);
    }

    print_str("\nPlanning economics...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1; int cat = (i % 5) + 1;
        int eco = 42 + (i * 9); int inf = 30 + (i * 7);
        int pub = 18 + (i * 4); int tra = 12 + (i * 3);
        int year = 2023 + (i % 2);
        ue_pla_econ(type, cat, eco, inf, pub, tra, year);
    }

    print_str("\nRegional development...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1; int cat = (i % 5) + 1;
        int eco = 38 + (i * 8); int rur = 28 + (i * 6);
        int cor = 16 + (i * 4); int grp = 11 + (i * 2);
        int year = 2024;
        ue_reg_dev(type, cat, eco, rur, cor, grp, year);
    }

    print_str("\nUrban report...\n"); ue_urban_report();
    print_str("\nLand report...\n"); ue_land_report();
    print_str("\nFull report...\n"); ue_full_report();
    print_str("\nFinal state...\n"); ue_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
