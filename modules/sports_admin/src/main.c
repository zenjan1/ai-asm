/* sports_admin: Sports administration system (v1.0)
 * Sports theory, competitive sports, ethnic sports, leisure sports, sports industry
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

#define MAX_SPO_THE     16
#define MAX_COM_SPO     14
#define MAX_ETH_SPO     12
#define MAX_LEI_SPO     10
#define MAX_SPO_IND     10

typedef struct {
    int    st_id;
    int    st_type;
    int    st_category;
    int    spo_the;
    int    mov_tra;
    int    spo_tea;
    int    soc_spo;
    int    year;
    int    active;
} spo_the_t;

typedef struct {
    int    cs_id;
    int    cs_type;
    int    cs_category;
    int    tra_spo;
    int    bal_spo;
    int    wat_spo;
    int    ico_spo;
    int    year;
    int    active;
} com_spo_t;

typedef struct {
    int    es_id;
    int    es_type;
    int    es_category;
    int    tra_mar;
    int    eth_fol;
    int    hea_spo;
    int    arc_spo;
    int    year;
    int    active;
} eth_spo_t;

typedef struct {
    int    ls_id;
    int    ls_type;
    int    ls_category;
    int    out_spo;
    int    fit_lei;
    int    ext_spo;
    int    spo_tou;
    int    year;
    int    active;
} lei_spo_t;

typedef struct {
    int    si_id;
    int    si_type;
    int    si_category;
    int    spo_man;
    int    spo_mar;
    int    spo_med;
    int    spo_fac;
    int    year;
    int    active;
} spo_ind_t;

typedef struct {
    int    n_spo_the;
    int    n_com_spo;
    int    n_eth_spo;
    int    n_lei_spo;
    int    n_spo_ind;
    int    total_spo_the;
    int    total_tra_spo;
    int    total_tra_mar;
    int    total_out_spo;
    int    total_spo_man;
} spa_state_t;

static spo_the_t spo_thes[MAX_SPO_THE];
static com_spo_t com_spos[MAX_COM_SPO];
static eth_spo_t eth_spos[MAX_ETH_SPO];
static lei_spo_t lei_spos[MAX_LEI_SPO];
static spo_ind_t spo_inds[MAX_SPO_IND];
static spa_state_t spa;

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

int spa_init(void) {
    if (initialized) return -1;
    spa.n_spo_the = 0; spa.n_com_spo = 0; spa.n_eth_spo = 0;
    spa.n_lei_spo = 0; spa.n_spo_ind = 0;
    spa.total_spo_the = 0; spa.total_tra_spo = 0;
    spa.total_tra_mar = 0; spa.total_out_spo = 0;
    spa.total_spo_man = 0;
    for (int i = 0; i < MAX_SPO_THE; i++) spo_thes[i].active = 0;
    for (int i = 0; i < MAX_COM_SPO; i++) com_spos[i].active = 0;
    for (int i = 0; i < MAX_ETH_SPO; i++) eth_spos[i].active = 0;
    for (int i = 0; i < MAX_LEI_SPO; i++) lei_spos[i].active = 0;
    for (int i = 0; i < MAX_SPO_IND; i++) spo_inds[i].active = 0;
    initialized = 1;
    print_str("[SPA] Sports initialized\n");
    return 0;
}

int spa_spo_the(int st_type, int cat, int sth, int mtr, int ste, int ssp, int year) {
    if (spa.n_spo_the >= MAX_SPO_THE) return -1;
    spo_the_t* s = &spo_thes[spa.n_spo_the];
    s->st_id = spa.n_spo_the;
    s->st_type = st_type;
    s->st_category = cat;
    s->spo_the = sth;
    s->mov_tra = mtr;
    s->spo_tea = ste;
    s->soc_spo = ssp;
    s->year = year;
    s->active = 1;
    spa.total_spo_the += sth;
    spa.n_spo_the++;
    print_str("[SPA] Spo the "); print_int(spa.n_spo_the - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" sth="); print_int(sth);
    print_str(" mtr="); print_int(mtr);
    print_str(" ste="); print_int(ste);
    print_str(" ssp="); print_int(ssp); print_str("\n");
    return spa.n_spo_the - 1;
}

int spa_com_spo(int ct_type, int cat, int tsp, int bsp, int wsp, int isp, int year) {
    if (spa.n_com_spo >= MAX_COM_SPO) return -1;
    com_spo_t* c = &com_spos[spa.n_com_spo];
    c->cs_id = spa.n_com_spo;
    c->cs_type = ct_type;
    c->cs_category = cat;
    c->tra_spo = tsp;
    c->bal_spo = bsp;
    c->wat_spo = wsp;
    c->ico_spo = isp;
    c->year = year;
    c->active = 1;
    spa.total_tra_spo += tsp;
    spa.n_com_spo++;
    print_str("[SPA] Com spo "); print_int(spa.n_com_spo - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" tsp="); print_int(tsp);
    print_str(" bsp="); print_int(bsp);
    print_str(" wsp="); print_int(wsp);
    print_str(" isp="); print_int(isp); print_str("\n");
    return spa.n_com_spo - 1;
}

int spa_eth_spo(int et_type, int cat, int tma, int efs, int hsp, int asp, int year) {
    if (spa.n_eth_spo >= MAX_ETH_SPO) return -1;
    eth_spo_t* e = &eth_spos[spa.n_eth_spo];
    e->es_id = spa.n_eth_spo;
    e->es_type = et_type;
    e->es_category = cat;
    e->tra_mar = tma;
    e->eth_fol = efs;
    e->hea_spo = hsp;
    e->arc_spo = asp;
    e->year = year;
    e->active = 1;
    spa.total_tra_mar += tma;
    spa.n_eth_spo++;
    print_str("[SPA] Eth spo "); print_int(spa.n_eth_spo - 1);
    print_str(" type="); print_int(et_type);
    print_str(" cat="); print_int(cat);
    print_str(" tma="); print_int(tma);
    print_str(" efs="); print_int(efs);
    print_str(" hsp="); print_int(hsp);
    print_str(" asp="); print_int(asp); print_str("\n");
    return spa.n_eth_spo - 1;
}

int spa_lei_spo(int lt_type, int cat, int osp, int fle, int esp, int sto, int year) {
    if (spa.n_lei_spo >= MAX_LEI_SPO) return -1;
    lei_spo_t* l = &lei_spos[spa.n_lei_spo];
    l->ls_id = spa.n_lei_spo;
    l->ls_type = lt_type;
    l->ls_category = cat;
    l->out_spo = osp;
    l->fit_lei = fle;
    l->ext_spo = esp;
    l->spo_tou = sto;
    l->year = year;
    l->active = 1;
    spa.total_out_spo += osp;
    spa.n_lei_spo++;
    print_str("[SPA] Lei spo "); print_int(spa.n_lei_spo - 1);
    print_str(" type="); print_int(lt_type);
    print_str(" cat="); print_int(cat);
    print_str(" osp="); print_int(osp);
    print_str(" fle="); print_int(fle);
    print_str(" esp="); print_int(esp);
    print_str(" sto="); print_int(sto); print_str("\n");
    return spa.n_lei_spo - 1;
}

int spa_spo_ind(int it_type, int cat, int sma, int smr, int smd, int sfc, int year) {
    if (spa.n_spo_ind >= MAX_SPO_IND) return -1;
    spo_ind_t* s = &spo_inds[spa.n_spo_ind];
    s->si_id = spa.n_spo_ind;
    s->si_type = it_type;
    s->si_category = cat;
    s->spo_man = sma;
    s->spo_mar = smr;
    s->spo_med = smd;
    s->spo_fac = sfc;
    s->year = year;
    s->active = 1;
    spa.total_spo_man += sma;
    spa.n_spo_ind++;
    print_str("[SPA] Spo ind "); print_int(spa.n_spo_ind - 1);
    print_str(" type="); print_int(it_type);
    print_str(" cat="); print_int(cat);
    print_str(" sma="); print_int(sma);
    print_str(" smr="); print_int(smr);
    print_str(" smd="); print_int(smd);
    print_str(" sfc="); print_int(sfc); print_str("\n");
    return spa.n_spo_ind - 1;
}

void spa_theory_report(void) {
    print_str("[SPA] Sports theory report:\n");
    print_str("  Theory categories: "); print_int(spa.n_spo_the); print_str("\n");
    print_str("  Total sports theory: "); print_int(spa.total_spo_the); print_str("\n");
}

void spa_competitive_report(void) {
    print_str("[SPA] Competitive report:\n");
    print_str("  Competitive categories: "); print_int(spa.n_com_spo); print_str("\n");
    print_str("  Total track sports: "); print_int(spa.total_tra_spo); print_str("\n");
}

void spa_full_report(void) {
    print_str("[SPA] Full report:\n");
    print_str("  Ethnic categories: "); print_int(spa.n_eth_spo); print_str("\n");
    print_str("  Total traditional martial: "); print_int(spa.total_tra_mar); print_str("\n");
    print_str("  Leisure categories: "); print_int(spa.n_lei_spo); print_str("\n");
    print_str("  Total outdoor sports: "); print_int(spa.total_out_spo); print_str("\n");
    print_str("  Industry categories: "); print_int(spa.n_spo_ind); print_str("\n");
    print_str("  Total sports management: "); print_int(spa.total_spo_man); print_str("\n");
}

void spa_print_state(void) {
    print_str("[SPA] St="); print_int(spa.n_spo_the);
    print_str(" Cs="); print_int(spa.n_com_spo);
    print_str(" Es="); print_int(spa.n_eth_spo);
    print_str(" Ls="); print_int(spa.n_lei_spo);
    print_str(" Si="); print_int(spa.n_spo_ind);
    print_str("\n");
}

int main(void) {
    print_str("=== Sports Admin Demo ===\n\n");
    spa_init();

    print_str("Sports theory...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int sth = 55 + (i * 13);
        int mtr = 40 + (i * 10);
        int ste = 22 + (i * 5);
        int ssp = 15 + (i * 3);
        int year = 2020 + (i % 5);
        spa_spo_the(type, cat, sth, mtr, ste, ssp, year);
    }

    print_str("\nCompetitive...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int tsp = 48 + (i * 11);
        int bsp = 35 + (i * 8);
        int wsp = 20 + (i * 4);
        int isp = 12 + (i * 3);
        int year = 2021 + (i % 4);
        spa_com_spo(type, cat, tsp, bsp, wsp, isp, year);
    }

    print_str("\nEthnic...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int tma = 42 + (i * 10);
        int efs = 28 + (i * 7);
        int hsp = 18 + (i * 4);
        int asp = 10 + (i * 2);
        int year = 2022 + (i % 3);
        spa_eth_spo(type, cat, tma, efs, hsp, asp, year);
    }

    print_str("\nLeisure...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int osp = 35 + (i * 8);
        int fle = 25 + (i * 6);
        int esp = 15 + (i * 3);
        int sto = 10 + (i * 2);
        int year = 2023 + (i % 2);
        spa_lei_spo(type, cat, osp, fle, esp, sto, year);
    }

    print_str("\nIndustry...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int sma = 30 + (i * 7);
        int smr = 22 + (i * 5);
        int smd = 12 + (i * 3);
        int sfc = 8 + (i * 2);
        int year = 2024;
        spa_spo_ind(type, cat, sma, smr, smd, sfc, year);
    }

    print_str("\nTheory report...\n");
    spa_theory_report();

    print_str("\nCompetitive report...\n");
    spa_competitive_report();

    print_str("\nFull report...\n");
    spa_full_report();

    print_str("\nFinal state...\n");
    spa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
