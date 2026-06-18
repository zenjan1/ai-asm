/* accounting_admin: Accounting administration system (v1.0)
 * Financial accounting, management accounting, auditing, tax accounting, accounting info
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

#define MAX_FIN_ACC    16
#define MAX_MAN_ACC    14
#define MAX_AUD_IT     12
#define MAX_TAX_ACC    10
#define MAX_ACC_INF    10

typedef struct {
    int    fa_id;
    int    fa_type;
    int    fa_category;
    int    fin_the;
    int    fin_rep;
    int    fin_ana;
    int    acc_std;
    int    year;
    int    active;
} fin_acc_t;

typedef struct {
    int    ma_id;
    int    ma_type;
    int    ma_category;
    int    cos_man;
    int    bud_man;
    int    per_eva;
    int    dec_ana;
    int    year;
    int    active;
} man_acc_t;

typedef struct {
    int    au_id;
    int    au_type;
    int    au_category;
    int    aud_the;
    int    int_aud;
    int    ext_aud;
    int    aud_met;
    int    year;
    int    active;
} aud_it_t;

typedef struct {
    int    ta_id;
    int    ta_type;
    int    ta_category;
    int    tax_pla;
    int    tax_dec;
    int    tax_aud;
    int    int_tax;
    int    year;
    int    active;
} tax_acc_t;

typedef struct {
    int    ai_id;
    int    ai_type;
    int    ai_category;
    int    acc_inf;
    int    acc_ele;
    int    dat_ana;
    int    int_fin;
    int    year;
    int    active;
} acc_inf_t;

typedef struct {
    int    n_fin_acc;
    int    n_man_acc;
    int    n_aud_it;
    int    n_tax_acc;
    int    n_acc_inf;
    int    total_fin_the;
    int    total_cos_man;
    int    total_aud_the;
    int    total_tax_pla;
    int    total_acc_inf;
} aca_state_t;

static fin_acc_t fin_accs[MAX_FIN_ACC];
static man_acc_t man_accs[MAX_MAN_ACC];
static aud_it_t aud_its[MAX_AUD_IT];
static tax_acc_t tax_accs[MAX_TAX_ACC];
static acc_inf_t acc_infs[MAX_ACC_INF];
static aca_state_t aca;

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

int aca_init(void) {
    if (initialized) return -1;
    aca.n_fin_acc = 0; aca.n_man_acc = 0; aca.n_aud_it = 0;
    aca.n_tax_acc = 0; aca.n_acc_inf = 0;
    aca.total_fin_the = 0; aca.total_cos_man = 0;
    aca.total_aud_the = 0; aca.total_tax_pla = 0;
    aca.total_acc_inf = 0;
    for (int i = 0; i < MAX_FIN_ACC; i++) fin_accs[i].active = 0;
    for (int i = 0; i < MAX_MAN_ACC; i++) man_accs[i].active = 0;
    for (int i = 0; i < MAX_AUD_IT; i++) aud_its[i].active = 0;
    for (int i = 0; i < MAX_TAX_ACC; i++) tax_accs[i].active = 0;
    for (int i = 0; i < MAX_ACC_INF; i++) acc_infs[i].active = 0;
    initialized = 1;
    print_str("[ACA] Accounting initialized\n");
    return 0;
}

int aca_fin_acc(int ft_type, int cat, int fth, int fre, int fan, int ast, int year) {
    if (aca.n_fin_acc >= MAX_FIN_ACC) return -1;
    fin_acc_t* f = &fin_accs[aca.n_fin_acc];
    f->fa_id = aca.n_fin_acc;
    f->fa_type = ft_type;
    f->fa_category = cat;
    f->fin_the = fth;
    f->fin_rep = fre;
    f->fin_ana = fan;
    f->acc_std = ast;
    f->year = year;
    f->active = 1;
    aca.total_fin_the += fth;
    aca.n_fin_acc++;
    print_str("[ACA] Fin acc "); print_int(aca.n_fin_acc - 1);
    print_str(" type="); print_int(ft_type);
    print_str(" cat="); print_int(cat);
    print_str(" fth="); print_int(fth);
    print_str(" fre="); print_int(fre);
    print_str(" fan="); print_int(fan);
    print_str(" ast="); print_int(ast); print_str("\n");
    return aca.n_fin_acc - 1;
}

int aca_man_acc(int mt_type, int cat, int csm, int bdm, int pev, int dan, int year) {
    if (aca.n_man_acc >= MAX_MAN_ACC) return -1;
    man_acc_t* m = &man_accs[aca.n_man_acc];
    m->ma_id = aca.n_man_acc;
    m->ma_type = mt_type;
    m->ma_category = cat;
    m->cos_man = csm;
    m->bud_man = bdm;
    m->per_eva = pev;
    m->dec_ana = dan;
    m->year = year;
    m->active = 1;
    aca.total_cos_man += csm;
    aca.n_man_acc++;
    print_str("[ACA] Man acc "); print_int(aca.n_man_acc - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" csm="); print_int(csm);
    print_str(" bdm="); print_int(bdm);
    print_str(" pev="); print_int(pev);
    print_str(" dan="); print_int(dan); print_str("\n");
    return aca.n_man_acc - 1;
}

int aca_aud_it(int at_type, int cat, int ath, int iad, int ead, int amth, int year) {
    if (aca.n_aud_it >= MAX_AUD_IT) return -1;
    aud_it_t* a = &aud_its[aca.n_aud_it];
    a->au_id = aca.n_aud_it;
    a->au_type = at_type;
    a->au_category = cat;
    a->aud_the = ath;
    a->int_aud = iad;
    a->ext_aud = ead;
    a->aud_met = amth;
    a->year = year;
    a->active = 1;
    aca.total_aud_the += ath;
    aca.n_aud_it++;
    print_str("[ACA] Aud it "); print_int(aca.n_aud_it - 1);
    print_str(" type="); print_int(at_type);
    print_str(" cat="); print_int(cat);
    print_str(" ath="); print_int(ath);
    print_str(" iad="); print_int(iad);
    print_str(" ead="); print_int(ead);
    print_str(" amth="); print_int(amth); print_str("\n");
    return aca.n_aud_it - 1;
}

int aca_tax_acc(int tt_type, int cat, int tpl, int tdc, int tax, int itx, int year) {
    if (aca.n_tax_acc >= MAX_TAX_ACC) return -1;
    tax_acc_t* t = &tax_accs[aca.n_tax_acc];
    t->ta_id = aca.n_tax_acc;
    t->ta_type = tt_type;
    t->ta_category = cat;
    t->tax_pla = tpl;
    t->tax_dec = tdc;
    t->tax_aud = tax;
    t->int_tax = itx;
    t->year = year;
    t->active = 1;
    aca.total_tax_pla += tpl;
    aca.n_tax_acc++;
    print_str("[ACA] Tax acc "); print_int(aca.n_tax_acc - 1);
    print_str(" type="); print_int(tt_type);
    print_str(" cat="); print_int(cat);
    print_str(" tpl="); print_int(tpl);
    print_str(" tdc="); print_int(tdc);
    print_str(" tax="); print_int(tax);
    print_str(" itx="); print_int(itx); print_str("\n");
    return aca.n_tax_acc - 1;
}

int aca_acc_inf(int it_type, int cat, int aif, int ael, int dan, int ifn, int year) {
    if (aca.n_acc_inf >= MAX_ACC_INF) return -1;
    acc_inf_t* a = &acc_infs[aca.n_acc_inf];
    a->ai_id = aca.n_acc_inf;
    a->ai_type = it_type;
    a->ai_category = cat;
    a->acc_inf = aif;
    a->acc_ele = ael;
    a->dat_ana = dan;
    a->int_fin = ifn;
    a->year = year;
    a->active = 1;
    aca.total_acc_inf += aif;
    aca.n_acc_inf++;
    print_str("[ACA] Acc inf "); print_int(aca.n_acc_inf - 1);
    print_str(" type="); print_int(it_type);
    print_str(" cat="); print_int(cat);
    print_str(" aif="); print_int(aif);
    print_str(" ael="); print_int(ael);
    print_str(" dan="); print_int(dan);
    print_str(" ifn="); print_int(ifn); print_str("\n");
    return aca.n_acc_inf - 1;
}

void aca_financial_report(void) {
    print_str("[ACA] Financial accounting report:\n");
    print_str("  Financial categories: "); print_int(aca.n_fin_acc); print_str("\n");
    print_str("  Total financial theory: "); print_int(aca.total_fin_the); print_str("\n");
}

void aca_management_report(void) {
    print_str("[ACA] Management accounting report:\n");
    print_str("  Management categories: "); print_int(aca.n_man_acc); print_str("\n");
    print_str("  Total cost management: "); print_int(aca.total_cos_man); print_str("\n");
}

void aca_full_report(void) {
    print_str("[ACA] Full report:\n");
    print_str("  Audit categories: "); print_int(aca.n_aud_it); print_str("\n");
    print_str("  Total audit theory: "); print_int(aca.total_aud_the); print_str("\n");
    print_str("  Tax categories: "); print_int(aca.n_tax_acc); print_str("\n");
    print_str("  Total tax planning: "); print_int(aca.total_tax_pla); print_str("\n");
    print_str("  Info categories: "); print_int(aca.n_acc_inf); print_str("\n");
    print_str("  Total accounting info: "); print_int(aca.total_acc_inf); print_str("\n");
}

void aca_print_state(void) {
    print_str("[ACA] Fa="); print_int(aca.n_fin_acc);
    print_str(" Ma="); print_int(aca.n_man_acc);
    print_str(" Au="); print_int(aca.n_aud_it);
    print_str(" Ta="); print_int(aca.n_tax_acc);
    print_str(" Ai="); print_int(aca.n_acc_inf);
    print_str("\n");
}

int main(void) {
    print_str("=== Accounting Admin Demo ===\n\n");
    aca_init();

    print_str("Financial accounting...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int fth = 55 + (i * 13);
        int fre = 40 + (i * 10);
        int fan = 22 + (i * 5);
        int ast = 15 + (i * 3);
        int year = 2020 + (i % 5);
        aca_fin_acc(type, cat, fth, fre, fan, ast, year);
    }

    print_str("\nManagement accounting...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int csm = 48 + (i * 11);
        int bdm = 35 + (i * 8);
        int pev = 20 + (i * 4);
        int dan = 12 + (i * 3);
        int year = 2021 + (i % 4);
        aca_man_acc(type, cat, csm, bdm, pev, dan, year);
    }

    print_str("\nAuditing...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ath = 42 + (i * 10);
        int iad = 28 + (i * 7);
        int ead = 18 + (i * 4);
        int amth = 10 + (i * 2);
        int year = 2022 + (i % 3);
        aca_aud_it(type, cat, ath, iad, ead, amth, year);
    }

    print_str("\nTax accounting...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int tpl = 35 + (i * 8);
        int tdc = 25 + (i * 6);
        int tax = 15 + (i * 3);
        int itx = 10 + (i * 2);
        int year = 2023 + (i % 2);
        aca_tax_acc(type, cat, tpl, tdc, tax, itx, year);
    }

    print_str("\nAccounting info...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int aif = 30 + (i * 7);
        int ael = 22 + (i * 5);
        int dan = 12 + (i * 3);
        int ifn = 8 + (i * 2);
        int year = 2024;
        aca_acc_inf(type, cat, aif, ael, dan, ifn, year);
    }

    print_str("\nFinancial report...\n");
    aca_financial_report();

    print_str("\nManagement report...\n");
    aca_management_report();

    print_str("\nFull report...\n");
    aca_full_report();

    print_str("\nFinal state...\n");
    aca_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
