/* insurance_admin: Insurance administration system (v1.0)
 * Insurance theory, life insurance, property insurance, actuarial, insurance management
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

#define MAX_INS_THE    16
#define MAX_LIF_INS    14
#define MAX_PRO_INS    12
#define MAX_ACT_UAR    10
#define MAX_INS_MAN    10

typedef struct {
    int    it_id;
    int    it_type;
    int    it_category;
    int    ins_pri;
    int    ins_con;
    int    ins_reg;
    int    ins_mar;
    int    year;
    int    active;
} ins_the_t;

typedef struct {
    int    li_id;
    int    li_type;
    int    li_category;
    int    lif_ins;
    int    hea_ins;
    int    acc_ins;
    int    pen_ins;
    int    year;
    int    active;
} lif_ins_t;

typedef struct {
    int    pi_id;
    int    pi_type;
    int    pi_category;
    int    pro_the;
    int    lia_ins;
    int    cre_ins;
    int    re_ins;
    int    year;
    int    active;
} pro_ins_t;

typedef struct {
    int    au_id;
    int    au_type;
    int    au_category;
    int    lif_act;
    int    nlf_act;
    int    res_eva;
    int    risk_ana;
    int    year;
    int    active;
} act_uar_t;

typedef struct {
    int    im_id;
    int    im_type;
    int    im_category;
    int    und_cli;
    int    ins_mar2;
    int    cus_ser;
    int    ins_tec;
    int    year;
    int    active;
} ins_man_t;

typedef struct {
    int    n_ins_the;
    int    n_lif_ins;
    int    n_pro_ins;
    int    n_act_uar;
    int    n_ins_man;
    int    total_ins_pri;
    int    total_lif_ins;
    int    total_pro_the;
    int    total_lif_act;
    int    total_und_cli;
} isa_state_t;

static ins_the_t ins_thes[MAX_INS_THE];
static lif_ins_t lif_inss[MAX_LIF_INS];
static pro_ins_t pro_inss[MAX_PRO_INS];
static act_uar_t act_uars[MAX_ACT_UAR];
static ins_man_t ins_mans[MAX_INS_MAN];
static isa_state_t isa;

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

int isa_init(void) {
    if (initialized) return -1;
    isa.n_ins_the = 0; isa.n_lif_ins = 0; isa.n_pro_ins = 0;
    isa.n_act_uar = 0; isa.n_ins_man = 0;
    isa.total_ins_pri = 0; isa.total_lif_ins = 0;
    isa.total_pro_the = 0; isa.total_lif_act = 0;
    isa.total_und_cli = 0;
    for (int i = 0; i < MAX_INS_THE; i++) ins_thes[i].active = 0;
    for (int i = 0; i < MAX_LIF_INS; i++) lif_inss[i].active = 0;
    for (int i = 0; i < MAX_PRO_INS; i++) pro_inss[i].active = 0;
    for (int i = 0; i < MAX_ACT_UAR; i++) act_uars[i].active = 0;
    for (int i = 0; i < MAX_INS_MAN; i++) ins_mans[i].active = 0;
    initialized = 1;
    print_str("[ISA] Insurance initialized\n");
    return 0;
}

int isa_ins_the(int it_type, int cat, int ipr, int icon, int ireg, int imr, int year) {
    if (isa.n_ins_the >= MAX_INS_THE) return -1;
    ins_the_t* t = &ins_thes[isa.n_ins_the];
    t->it_id = isa.n_ins_the;
    t->it_type = it_type;
    t->it_category = cat;
    t->ins_pri = ipr;
    t->ins_con = icon;
    t->ins_reg = ireg;
    t->ins_mar = imr;
    t->year = year;
    t->active = 1;
    isa.total_ins_pri += ipr;
    isa.n_ins_the++;
    print_str("[ISA] Ins the "); print_int(isa.n_ins_the - 1);
    print_str(" type="); print_int(it_type);
    print_str(" cat="); print_int(cat);
    print_str(" ipr="); print_int(ipr);
    print_str(" icon="); print_int(icon);
    print_str(" ireg="); print_int(ireg);
    print_str(" imr="); print_int(imr); print_str("\n");
    return isa.n_ins_the - 1;
}

int isa_lif_ins(int lt_type, int cat, int lfi, int hei, int aci, int pen, int year) {
    if (isa.n_lif_ins >= MAX_LIF_INS) return -1;
    lif_ins_t* l = &lif_inss[isa.n_lif_ins];
    l->li_id = isa.n_lif_ins;
    l->li_type = lt_type;
    l->li_category = cat;
    l->lif_ins = lfi;
    l->hea_ins = hei;
    l->acc_ins = aci;
    l->pen_ins = pen;
    l->year = year;
    l->active = 1;
    isa.total_lif_ins += lfi;
    isa.n_lif_ins++;
    print_str("[ISA] Lif ins "); print_int(isa.n_lif_ins - 1);
    print_str(" type="); print_int(lt_type);
    print_str(" cat="); print_int(cat);
    print_str(" lfi="); print_int(lfi);
    print_str(" hei="); print_int(hei);
    print_str(" aci="); print_int(aci);
    print_str(" pen="); print_int(pen); print_str("\n");
    return isa.n_lif_ins - 1;
}

int isa_pro_ins(int pt_type, int cat, int pth, int lia, int cre, int rei, int year) {
    if (isa.n_pro_ins >= MAX_PRO_INS) return -1;
    pro_ins_t* p = &pro_inss[isa.n_pro_ins];
    p->pi_id = isa.n_pro_ins;
    p->pi_type = pt_type;
    p->pi_category = cat;
    p->pro_the = pth;
    p->lia_ins = lia;
    p->cre_ins = cre;
    p->re_ins = rei;
    p->year = year;
    p->active = 1;
    isa.total_pro_the += pth;
    isa.n_pro_ins++;
    print_str("[ISA] Pro ins "); print_int(isa.n_pro_ins - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" pth="); print_int(pth);
    print_str(" lia="); print_int(lia);
    print_str(" cre="); print_int(cre);
    print_str(" rei="); print_int(rei); print_str("\n");
    return isa.n_pro_ins - 1;
}

int isa_act_uar(int at_type, int cat, int lfa, int nla, int rev, int rsk, int year) {
    if (isa.n_act_uar >= MAX_ACT_UAR) return -1;
    act_uar_t* a = &act_uars[isa.n_act_uar];
    a->au_id = isa.n_act_uar;
    a->au_type = at_type;
    a->au_category = cat;
    a->lif_act = lfa;
    a->nlf_act = nla;
    a->res_eva = rev;
    a->risk_ana = rsk;
    a->year = year;
    a->active = 1;
    isa.total_lif_act += lfa;
    isa.n_act_uar++;
    print_str("[ISA] Act uar "); print_int(isa.n_act_uar - 1);
    print_str(" type="); print_int(at_type);
    print_str(" cat="); print_int(cat);
    print_str(" lfa="); print_int(lfa);
    print_str(" nla="); print_int(nla);
    print_str(" rev="); print_int(rev);
    print_str(" rsk="); print_int(rsk); print_str("\n");
    return isa.n_act_uar - 1;
}

int isa_ins_man(int mt_type, int cat, int udc, int imr2, int cser, int itc, int year) {
    if (isa.n_ins_man >= MAX_INS_MAN) return -1;
    ins_man_t* m = &ins_mans[isa.n_ins_man];
    m->im_id = isa.n_ins_man;
    m->im_type = mt_type;
    m->im_category = cat;
    m->und_cli = udc;
    m->ins_mar2 = imr2;
    m->cus_ser = cser;
    m->ins_tec = itc;
    m->year = year;
    m->active = 1;
    isa.total_und_cli += udc;
    isa.n_ins_man++;
    print_str("[ISA] Ins man "); print_int(isa.n_ins_man - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" udc="); print_int(udc);
    print_str(" imr2="); print_int(imr2);
    print_str(" cser="); print_int(cser);
    print_str(" itc="); print_int(itc); print_str("\n");
    return isa.n_ins_man - 1;
}

void isa_theory_report(void) {
    print_str("[ISA] Insurance theory report:\n");
    print_str("  Theory categories: "); print_int(isa.n_ins_the); print_str("\n");
    print_str("  Total insurance principles: "); print_int(isa.total_ins_pri); print_str("\n");
}

void isa_life_report(void) {
    print_str("[ISA] Life insurance report:\n");
    print_str("  Life categories: "); print_int(isa.n_lif_ins); print_str("\n");
    print_str("  Total life insurance: "); print_int(isa.total_lif_ins); print_str("\n");
}

void isa_full_report(void) {
    print_str("[ISA] Full report:\n");
    print_str("  Property categories: "); print_int(isa.n_pro_ins); print_str("\n");
    print_str("  Total property theory: "); print_int(isa.total_pro_the); print_str("\n");
    print_str("  Actuarial categories: "); print_int(isa.n_act_uar); print_str("\n");
    print_str("  Total life actuarial: "); print_int(isa.total_lif_act); print_str("\n");
    print_str("  Management categories: "); print_int(isa.n_ins_man); print_str("\n");
    print_str("  Total underwriting claims: "); print_int(isa.total_und_cli); print_str("\n");
}

void isa_print_state(void) {
    print_str("[ISA] It="); print_int(isa.n_ins_the);
    print_str(" Li="); print_int(isa.n_lif_ins);
    print_str(" Pi="); print_int(isa.n_pro_ins);
    print_str(" Au="); print_int(isa.n_act_uar);
    print_str(" Im="); print_int(isa.n_ins_man);
    print_str("\n");
}

int main(void) {
    print_str("=== Insurance Admin Demo ===\n\n");
    isa_init();

    print_str("Insurance theory...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int ipr = 55 + (i * 13);
        int icon = 40 + (i * 10);
        int ireg = 22 + (i * 5);
        int imr = 15 + (i * 3);
        int year = 2020 + (i % 5);
        isa_ins_the(type, cat, ipr, icon, ireg, imr, year);
    }

    print_str("\nLife insurance...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int lfi = 48 + (i * 11);
        int hei = 35 + (i * 8);
        int aci = 20 + (i * 4);
        int pen = 12 + (i * 3);
        int year = 2021 + (i % 4);
        isa_lif_ins(type, cat, lfi, hei, aci, pen, year);
    }

    print_str("\nProperty insurance...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pth = 42 + (i * 10);
        int lia = 28 + (i * 7);
        int cre = 18 + (i * 4);
        int rei = 10 + (i * 2);
        int year = 2022 + (i % 3);
        isa_pro_ins(type, cat, pth, lia, cre, rei, year);
    }

    print_str("\nActuarial...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int lfa = 35 + (i * 8);
        int nla = 25 + (i * 6);
        int rev = 15 + (i * 3);
        int rsk = 10 + (i * 2);
        int year = 2023 + (i % 2);
        isa_act_uar(type, cat, lfa, nla, rev, rsk, year);
    }

    print_str("\nInsurance management...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int udc = 30 + (i * 7);
        int imr2 = 22 + (i * 5);
        int cser = 12 + (i * 3);
        int itc = 8 + (i * 2);
        int year = 2024;
        isa_ins_man(type, cat, udc, imr2, cser, itc, year);
    }

    print_str("\nTheory report...\n");
    isa_theory_report();

    print_str("\nLife report...\n");
    isa_life_report();

    print_str("\nFull report...\n");
    isa_full_report();

    print_str("\nFinal state...\n");
    isa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
