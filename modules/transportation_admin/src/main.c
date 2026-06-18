/* transportation_admin: Transportation administration system (v1.0)
 * Traffic engineering, transport engineering, traffic info, rail transit, water transport
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

#define MAX_TRA_ENG     16
#define MAX_TRN_ENG     14
#define MAX_TRA_INF     12
#define MAX_RAI_TRA     10
#define MAX_WAT_TRA     10

typedef struct {
    int    te_id;
    int    te_type;
    int    te_category;
    int    tra_pla;
    int    tra_des;
    int    tra_man;
    int    tra_saf;
    int    year;
    int    active;
} tra_eng_t;

typedef struct {
    int    tr_id;
    int    tr_type;
    int    tr_category;
    int    hig_tra;
    int    rai_tra;
    int    air_tra;
    int    pip_tra;
    int    year;
    int    active;
} trn_eng_t;

typedef struct {
    int    ti_id;
    int    ti_type;
    int    ti_category;
    int    int_tra;
    int    tra_con;
    int    tra_det;
    int    tra_sim;
    int    year;
    int    active;
} tra_inf_t;

typedef struct {
    int    rt_id;
    int    rt_type;
    int    rt_category;
    int    urb_rail;
    int    hig_spd;
    int    mag_lev;
    int    lig_rail;
    int    year;
    int    active;
} rai_tra_t;

typedef struct {
    int    wt_id;
    int    wt_type;
    int    wt_category;
    int    por_eng;
    int    wat_eng;
    int    shi_eng;
    int    wat_man;
    int    year;
    int    active;
} wat_tra_t;

typedef struct {
    int    n_tra_eng;
    int    n_trn_eng;
    int    n_tra_inf;
    int    n_rai_tra;
    int    n_wat_tra;
    int    total_tra_pla;
    int    total_hig_tra;
    int    total_int_tra;
    int    total_urb_rail;
    int    total_por_eng;
} tpa_state_t;

static tra_eng_t tra_engs[MAX_TRA_ENG];
static trn_eng_t trn_engs[MAX_TRN_ENG];
static tra_inf_t tra_infs[MAX_TRA_INF];
static rai_tra_t rai_tras[MAX_RAI_TRA];
static wat_tra_t wat_tras[MAX_WAT_TRA];
static tpa_state_t tpa;

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

int tpa_init(void) {
    if (initialized) return -1;
    tpa.n_tra_eng = 0; tpa.n_trn_eng = 0; tpa.n_tra_inf = 0;
    tpa.n_rai_tra = 0; tpa.n_wat_tra = 0;
    tpa.total_tra_pla = 0; tpa.total_hig_tra = 0;
    tpa.total_int_tra = 0; tpa.total_urb_rail = 0;
    tpa.total_por_eng = 0;
    for (int i = 0; i < MAX_TRA_ENG; i++) tra_engs[i].active = 0;
    for (int i = 0; i < MAX_TRN_ENG; i++) trn_engs[i].active = 0;
    for (int i = 0; i < MAX_TRA_INF; i++) tra_infs[i].active = 0;
    for (int i = 0; i < MAX_RAI_TRA; i++) rai_tras[i].active = 0;
    for (int i = 0; i < MAX_WAT_TRA; i++) wat_tras[i].active = 0;
    initialized = 1;
    print_str("[TPA] Transportation initialized\n");
    return 0;
}

int tpa_tra_eng(int tt_type, int cat, int tpl, int tde, int tma, int tsa, int year) {
    if (tpa.n_tra_eng >= MAX_TRA_ENG) return -1;
    tra_eng_t* t = &tra_engs[tpa.n_tra_eng];
    t->te_id = tpa.n_tra_eng;
    t->te_type = tt_type;
    t->te_category = cat;
    t->tra_pla = tpl;
    t->tra_des = tde;
    t->tra_man = tma;
    t->tra_saf = tsa;
    t->year = year;
    t->active = 1;
    tpa.total_tra_pla += tpl;
    tpa.n_tra_eng++;
    print_str("[TPA] Tra eng "); print_int(tpa.n_tra_eng - 1);
    print_str(" type="); print_int(tt_type);
    print_str(" cat="); print_int(cat);
    print_str(" tpl="); print_int(tpl);
    print_str(" tde="); print_int(tde);
    print_str(" tma="); print_int(tma);
    print_str(" tsa="); print_int(tsa); print_str("\n");
    return tpa.n_tra_eng - 1;
}

int tpa_trn_eng(int tt_type, int cat, int htr, int rtr, int atr, int ptr, int year) {
    if (tpa.n_trn_eng >= MAX_TRN_ENG) return -1;
    trn_eng_t* t = &trn_engs[tpa.n_trn_eng];
    t->tr_id = tpa.n_trn_eng;
    t->tr_type = tt_type;
    t->tr_category = cat;
    t->hig_tra = htr;
    t->rai_tra = rtr;
    t->air_tra = atr;
    t->pip_tra = ptr;
    t->year = year;
    t->active = 1;
    tpa.total_hig_tra += htr;
    tpa.n_trn_eng++;
    print_str("[TPA] Trn eng "); print_int(tpa.n_trn_eng - 1);
    print_str(" type="); print_int(tt_type);
    print_str(" cat="); print_int(cat);
    print_str(" htr="); print_int(htr);
    print_str(" rtr="); print_int(rtr);
    print_str(" atr="); print_int(atr);
    print_str(" ptr="); print_int(ptr); print_str("\n");
    return tpa.n_trn_eng - 1;
}

int tpa_tra_inf(int it_type, int cat, int itr, int tco, int tde, int tsi, int year) {
    if (tpa.n_tra_inf >= MAX_TRA_INF) return -1;
    tra_inf_t* t = &tra_infs[tpa.n_tra_inf];
    t->ti_id = tpa.n_tra_inf;
    t->ti_type = it_type;
    t->ti_category = cat;
    t->int_tra = itr;
    t->tra_con = tco;
    t->tra_det = tde;
    t->tra_sim = tsi;
    t->year = year;
    t->active = 1;
    tpa.total_int_tra += itr;
    tpa.n_tra_inf++;
    print_str("[TPA] Tra inf "); print_int(tpa.n_tra_inf - 1);
    print_str(" type="); print_int(it_type);
    print_str(" cat="); print_int(cat);
    print_str(" itr="); print_int(itr);
    print_str(" tco="); print_int(tco);
    print_str(" tde="); print_int(tde);
    print_str(" tsi="); print_int(tsi); print_str("\n");
    return tpa.n_tra_inf - 1;
}

int tpa_rai_tra(int rt_type, int cat, int ura, int hsp, int mgl, int lra, int year) {
    if (tpa.n_rai_tra >= MAX_RAI_TRA) return -1;
    rai_tra_t* r = &rai_tras[tpa.n_rai_tra];
    r->rt_id = tpa.n_rai_tra;
    r->rt_type = rt_type;
    r->rt_category = cat;
    r->urb_rail = ura;
    r->hig_spd = hsp;
    r->mag_lev = mgl;
    r->lig_rail = lra;
    r->year = year;
    r->active = 1;
    tpa.total_urb_rail += ura;
    tpa.n_rai_tra++;
    print_str("[TPA] Rai tra "); print_int(tpa.n_rai_tra - 1);
    print_str(" type="); print_int(rt_type);
    print_str(" cat="); print_int(cat);
    print_str(" ura="); print_int(ura);
    print_str(" hsp="); print_int(hsp);
    print_str(" mgl="); print_int(mgl);
    print_str(" lra="); print_int(lra); print_str("\n");
    return tpa.n_rai_tra - 1;
}

int tpa_wat_tra(int wt_type, int cat, int pen, int wen, int shn, int wma, int year) {
    if (tpa.n_wat_tra >= MAX_WAT_TRA) return -1;
    wat_tra_t* w = &wat_tras[tpa.n_wat_tra];
    w->wt_id = tpa.n_wat_tra;
    w->wt_type = wt_type;
    w->wt_category = cat;
    w->por_eng = pen;
    w->wat_eng = wen;
    w->shi_eng = shn;
    w->wat_man = wma;
    w->year = year;
    w->active = 1;
    tpa.total_por_eng += pen;
    tpa.n_wat_tra++;
    print_str("[TPA] Wat tra "); print_int(tpa.n_wat_tra - 1);
    print_str(" type="); print_int(wt_type);
    print_str(" cat="); print_int(cat);
    print_str(" pen="); print_int(pen);
    print_str(" wen="); print_int(wen);
    print_str(" shn="); print_int(shn);
    print_str(" wma="); print_int(wma); print_str("\n");
    return tpa.n_wat_tra - 1;
}

void tpa_engineering_report(void) {
    print_str("[TPA] Traffic engineering report:\n");
    print_str("  Engineering categories: "); print_int(tpa.n_tra_eng); print_str("\n");
    print_str("  Total traffic planning: "); print_int(tpa.total_tra_pla); print_str("\n");
}

void tpa_transport_report(void) {
    print_str("[TPA] Transport report:\n");
    print_str("  Transport categories: "); print_int(tpa.n_trn_eng); print_str("\n");
    print_str("  Total highway transport: "); print_int(tpa.total_hig_tra); print_str("\n");
}

void tpa_full_report(void) {
    print_str("[TPA] Full report:\n");
    print_str("  Traffic info categories: "); print_int(tpa.n_tra_inf); print_str("\n");
    print_str("  Total intelligent transport: "); print_int(tpa.total_int_tra); print_str("\n");
    print_str("  Rail categories: "); print_int(tpa.n_rai_tra); print_str("\n");
    print_str("  Total urban rail: "); print_int(tpa.total_urb_rail); print_str("\n");
    print_str("  Water categories: "); print_int(tpa.n_wat_tra); print_str("\n");
    print_str("  Total port engineering: "); print_int(tpa.total_por_eng); print_str("\n");
}

void tpa_print_state(void) {
    print_str("[TPA] Te="); print_int(tpa.n_tra_eng);
    print_str(" Tr="); print_int(tpa.n_trn_eng);
    print_str(" Ti="); print_int(tpa.n_tra_inf);
    print_str(" Rt="); print_int(tpa.n_rai_tra);
    print_str(" Wt="); print_int(tpa.n_wat_tra);
    print_str("\n");
}

int main(void) {
    print_str("=== Transportation Admin Demo ===\n\n");
    tpa_init();

    print_str("Traffic engineering...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int tpl = 55 + (i * 13);
        int tde = 40 + (i * 10);
        int tma = 22 + (i * 5);
        int tsa = 15 + (i * 3);
        int year = 2020 + (i % 5);
        tpa_tra_eng(type, cat, tpl, tde, tma, tsa, year);
    }

    print_str("\nTransport...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int htr = 48 + (i * 11);
        int rtr = 35 + (i * 8);
        int atr = 20 + (i * 4);
        int ptr = 12 + (i * 3);
        int year = 2021 + (i % 4);
        tpa_trn_eng(type, cat, htr, rtr, atr, ptr, year);
    }

    print_str("\nTraffic info...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int itr = 42 + (i * 10);
        int tco = 28 + (i * 7);
        int tde = 18 + (i * 4);
        int tsi = 10 + (i * 2);
        int year = 2022 + (i % 3);
        tpa_tra_inf(type, cat, itr, tco, tde, tsi, year);
    }

    print_str("\nRail transit...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ura = 35 + (i * 8);
        int hsp = 25 + (i * 6);
        int mgl = 15 + (i * 3);
        int lra = 10 + (i * 2);
        int year = 2023 + (i % 2);
        tpa_rai_tra(type, cat, ura, hsp, mgl, lra, year);
    }

    print_str("\nWater transport...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pen = 30 + (i * 7);
        int wen = 22 + (i * 5);
        int shn = 12 + (i * 3);
        int wma = 8 + (i * 2);
        int year = 2024;
        tpa_wat_tra(type, cat, pen, wen, shn, wma, year);
    }

    print_str("\nEngineering report...\n");
    tpa_engineering_report();

    print_str("\nTransport report...\n");
    tpa_transport_report();

    print_str("\nFull report...\n");
    tpa_full_report();

    print_str("\nFinal state...\n");
    tpa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
