/* architecture_admin: Architecture administration system (v1.0)
 * Architecture, urban planning, landscape, architectural tech, historic preservation
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

#define MAX_ARCHI       16
#define MAX_URBAN_PL    14
#define MAX_LANDSC      12
#define MAX_ARCHI_TE    10
#define MAX_HIST_PR     10

typedef struct {
    int    ar_id;
    int    ar_type;
    int    ar_category;
    int    archi_des;
    int    archi_his;
    int    archi_tech;
    int    urb_plan;
    int    year;
    int    active;
} archi_t;

typedef struct {
    int    up_id;
    int    up_type;
    int    up_category;
    int    urb_des;
    int    reg_plan;
    int    vil_plan;
    int    tra_plan;
    int    year;
    int    active;
} urban_pl_t;

typedef struct {
    int    ls_id;
    int    ls_type;
    int    ls_category;
    int    gar_des;
    int    land_des;
    int    plant_cfg;
    int    land_eng;
    int    year;
    int    active;
} landsc_t;

typedef struct {
    int    at_id;
    int    at_type;
    int    at_category;
    int    arc_str;
    int    arc_phy;
    int    arc_mat;
    int    arc_equ;
    int    year;
    int    active;
} archi_te_t;

typedef struct {
    int    hp_id;
    int    hp_type;
    int    hp_category;
    int    arc_her;
    int    cul_pre;
    int    anc_res;
    int    his_blk;
    int    year;
    int    active;
} hist_pr_t;

typedef struct {
    int    n_archi;
    int    n_urban_pl;
    int    n_landsc;
    int    n_archi_te;
    int    n_hist_pr;
    int    total_archi_des;
    int    total_urb_des;
    int    total_gar_des;
    int    total_arc_str;
    int    total_arc_her;
} ara_state_t;

static archi_t archis[MAX_ARCHI];
static urban_pl_t urban_pls[MAX_URBAN_PL];
static landsc_t landss[MAX_LANDSC];
static archi_te_t archi_tes[MAX_ARCHI_TE];
static hist_pr_t hist_prs[MAX_HIST_PR];
static ara_state_t ara;

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

int ara_init(void) {
    if (initialized) return -1;
    ara.n_archi = 0; ara.n_urban_pl = 0; ara.n_landsc = 0;
    ara.n_archi_te = 0; ara.n_hist_pr = 0;
    ara.total_archi_des = 0; ara.total_urb_des = 0;
    ara.total_gar_des = 0; ara.total_arc_str = 0;
    ara.total_arc_her = 0;
    for (int i = 0; i < MAX_ARCHI; i++) archis[i].active = 0;
    for (int i = 0; i < MAX_URBAN_PL; i++) urban_pls[i].active = 0;
    for (int i = 0; i < MAX_LANDSC; i++) landss[i].active = 0;
    for (int i = 0; i < MAX_ARCHI_TE; i++) archi_tes[i].active = 0;
    for (int i = 0; i < MAX_HIST_PR; i++) hist_prs[i].active = 0;
    initialized = 1;
    print_str("[ARA] Architecture initialized\n");
    return 0;
}

int ara_archi(int at_type, int cat, int ads, int ahs, int atc, int upl, int year) {
    if (ara.n_archi >= MAX_ARCHI) return -1;
    archi_t* a = &archis[ara.n_archi];
    a->ar_id = ara.n_archi;
    a->ar_type = at_type;
    a->ar_category = cat;
    a->archi_des = ads;
    a->archi_his = ahs;
    a->archi_tech = atc;
    a->urb_plan = upl;
    a->year = year;
    a->active = 1;
    ara.total_archi_des += ads;
    ara.n_archi++;
    print_str("[ARA] Archi "); print_int(ara.n_archi - 1);
    print_str(" type="); print_int(at_type);
    print_str(" cat="); print_int(cat);
    print_str(" ads="); print_int(ads);
    print_str(" ahs="); print_int(ahs);
    print_str(" atc="); print_int(atc);
    print_str(" upl="); print_int(upl); print_str("\n");
    return ara.n_archi - 1;
}

int ara_urban_pl(int ut_type, int cat, int uds, int rpl, int vpl, int tpl, int year) {
    if (ara.n_urban_pl >= MAX_URBAN_PL) return -1;
    urban_pl_t* u = &urban_pls[ara.n_urban_pl];
    u->up_id = ara.n_urban_pl;
    u->up_type = ut_type;
    u->up_category = cat;
    u->urb_des = uds;
    u->reg_plan = rpl;
    u->vil_plan = vpl;
    u->tra_plan = tpl;
    u->year = year;
    u->active = 1;
    ara.total_urb_des += uds;
    ara.n_urban_pl++;
    print_str("[ARA] Urban pl "); print_int(ara.n_urban_pl - 1);
    print_str(" type="); print_int(ut_type);
    print_str(" cat="); print_int(cat);
    print_str(" uds="); print_int(uds);
    print_str(" rpl="); print_int(rpl);
    print_str(" vpl="); print_int(vpl);
    print_str(" tpl="); print_int(tpl); print_str("\n");
    return ara.n_urban_pl - 1;
}

int ara_landsc(int lt_type, int cat, int gds, int lds, int pcfg, int len, int year) {
    if (ara.n_landsc >= MAX_LANDSC) return -1;
    landsc_t* l = &landss[ara.n_landsc];
    l->ls_id = ara.n_landsc;
    l->ls_type = lt_type;
    l->ls_category = cat;
    l->gar_des = gds;
    l->land_des = lds;
    l->plant_cfg = pcfg;
    l->land_eng = len;
    l->year = year;
    l->active = 1;
    ara.total_gar_des += gds;
    ara.n_landsc++;
    print_str("[ARA] Landsc "); print_int(ara.n_landsc - 1);
    print_str(" type="); print_int(lt_type);
    print_str(" cat="); print_int(cat);
    print_str(" gds="); print_int(gds);
    print_str(" lds="); print_int(lds);
    print_str(" pcfg="); print_int(pcfg);
    print_str(" len="); print_int(len); print_str("\n");
    return ara.n_landsc - 1;
}

int ara_archi_te(int tt_type, int cat, int astr, int aph, int amt, int aeq, int year) {
    if (ara.n_archi_te >= MAX_ARCHI_TE) return -1;
    archi_te_t* t = &archi_tes[ara.n_archi_te];
    t->at_id = ara.n_archi_te;
    t->at_type = tt_type;
    t->at_category = cat;
    t->arc_str = astr;
    t->arc_phy = aph;
    t->arc_mat = amt;
    t->arc_equ = aeq;
    t->year = year;
    t->active = 1;
    ara.total_arc_str += astr;
    ara.n_archi_te++;
    print_str("[ARA] Archi te "); print_int(ara.n_archi_te - 1);
    print_str(" type="); print_int(tt_type);
    print_str(" cat="); print_int(cat);
    print_str(" astr="); print_int(astr);
    print_str(" aph="); print_int(aph);
    print_str(" amt="); print_int(amt);
    print_str(" aeq="); print_int(aeq); print_str("\n");
    return ara.n_archi_te - 1;
}

int ara_hist_pr(int ht_type, int cat, int ahr, int cul, int anc, int hbl, int year) {
    if (ara.n_hist_pr >= MAX_HIST_PR) return -1;
    hist_pr_t* h = &hist_prs[ara.n_hist_pr];
    h->hp_id = ara.n_hist_pr;
    h->hp_type = ht_type;
    h->hp_category = cat;
    h->arc_her = ahr;
    h->cul_pre = cul;
    h->anc_res = anc;
    h->his_blk = hbl;
    h->year = year;
    h->active = 1;
    ara.total_arc_her += ahr;
    ara.n_hist_pr++;
    print_str("[ARA] Hist pr "); print_int(ara.n_hist_pr - 1);
    print_str(" type="); print_int(ht_type);
    print_str(" cat="); print_int(cat);
    print_str(" ahr="); print_int(ahr);
    print_str(" cul="); print_int(cul);
    print_str(" anc="); print_int(anc);
    print_str(" hbl="); print_int(hbl); print_str("\n");
    return ara.n_hist_pr - 1;
}

void ara_archi_report(void) {
    print_str("[ARA] Architecture report:\n");
    print_str("  Architecture categories: "); print_int(ara.n_archi); print_str("\n");
    print_str("  Total architecture design: "); print_int(ara.total_archi_des); print_str("\n");
}

void ara_urban_report(void) {
    print_str("[ARA] Urban planning report:\n");
    print_str("  Urban planning categories: "); print_int(ara.n_urban_pl); print_str("\n");
    print_str("  Total urban design: "); print_int(ara.total_urb_des); print_str("\n");
}

void ara_full_report(void) {
    print_str("[ARA] Full report:\n");
    print_str("  Landscape categories: "); print_int(ara.n_landsc); print_str("\n");
    print_str("  Total garden design: "); print_int(ara.total_gar_des); print_str("\n");
    print_str("  Archi tech categories: "); print_int(ara.n_archi_te); print_str("\n");
    print_str("  Total archi structure: "); print_int(ara.total_arc_str); print_str("\n");
    print_str("  Historic preservation categories: "); print_int(ara.n_hist_pr); print_str("\n");
    print_str("  Total archi heritage: "); print_int(ara.total_arc_her); print_str("\n");
}

void ara_print_state(void) {
    print_str("[ARA] Ar="); print_int(ara.n_archi);
    print_str(" Up="); print_int(ara.n_urban_pl);
    print_str(" Ls="); print_int(ara.n_landsc);
    print_str(" At="); print_int(ara.n_archi_te);
    print_str(" Hp="); print_int(ara.n_hist_pr);
    print_str("\n");
}

int main(void) {
    print_str("=== Architecture Admin Demo ===\n\n");
    ara_init();

    print_str("Architecture...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int ads = 55 + (i * 13);
        int ahs = 40 + (i * 10);
        int atc = 22 + (i * 5);
        int upl = 15 + (i * 3);
        int year = 2020 + (i % 5);
        ara_archi(type, cat, ads, ahs, atc, upl, year);
    }

    print_str("\nUrban planning...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int uds = 48 + (i * 11);
        int rpl = 35 + (i * 8);
        int vpl = 20 + (i * 4);
        int tpl = 12 + (i * 3);
        int year = 2021 + (i % 4);
        ara_urban_pl(type, cat, uds, rpl, vpl, tpl, year);
    }

    print_str("\nLandscape...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int gds = 42 + (i * 10);
        int lds = 28 + (i * 7);
        int pcfg = 18 + (i * 4);
        int len = 10 + (i * 2);
        int year = 2022 + (i % 3);
        ara_landsc(type, cat, gds, lds, pcfg, len, year);
    }

    print_str("\nArchitectural tech...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int astr = 35 + (i * 8);
        int aph = 25 + (i * 6);
        int amt = 15 + (i * 3);
        int aeq = 10 + (i * 2);
        int year = 2023 + (i % 2);
        ara_archi_te(type, cat, astr, aph, amt, aeq, year);
    }

    print_str("\nHistoric preservation...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ahr = 30 + (i * 7);
        int cul = 22 + (i * 5);
        int anc = 12 + (i * 3);
        int hbl = 8 + (i * 2);
        int year = 2024;
        ara_hist_pr(type, cat, ahr, cul, anc, hbl, year);
    }

    print_str("\nArchitecture report...\n");
    ara_archi_report();

    print_str("\nUrban report...\n");
    ara_urban_report();

    print_str("\nFull report...\n");
    ara_full_report();

    print_str("\nFinal state...\n");
    ara_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
