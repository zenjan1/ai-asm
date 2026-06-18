/* communications_admin: Communications administration system (v1.0)
 * Comm theory, wireless comm, fiber optic comm, comm networks, comm systems
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

#define MAX_COM_TH     16
#define MAX_WIR_COM    14
#define MAX_FIB_COM    12
#define MAX_COM_NET    10
#define MAX_COM_SYS    10

typedef struct {
    int    ct_id;
    int    ct_type;
    int    ct_category;
    int    sig_sys;
    int    mod_dem;
    int    cha_cod;
    int    syn_tec;
    int    year;
    int    active;
} com_th_t;

typedef struct {
    int    wc_id;
    int    wc_type;
    int    wc_category;
    int    mob_com;
    int    sat_com;
    int    mic_com;
    int    ant_tec;
    int    year;
    int    active;
} wir_com_t;

typedef struct {
    int    fc_id;
    int    fc_type;
    int    fc_category;
    int    fib_tra;
    int    opt_net;
    int    opt_swi;
    int    opt_acc;
    int    year;
    int    active;
} fib_com_t;

typedef struct {
    int    cn_id;
    int    cn_type;
    int    cn_category;
    int    tel_net;
    int    int_tec;
    int    iot_sys;
    int    five_g;
    int    year;
    int    active;
} com_net_t;

typedef struct {
    int    cs_id;
    int    cs_type;
    int    cs_category;
    int    sat_nav;
    int    rad_sys;
    int    tel_con;
    int    sdr_sys;
    int    year;
    int    active;
} com_sys_t;

typedef struct {
    int    n_com_th;
    int    n_wir_com;
    int    n_fib_com;
    int    n_com_net;
    int    n_com_sys;
    int    total_sig_sys;
    int    total_mob_com;
    int    total_fib_tra;
    int    total_tel_net;
    int    total_sat_nav;
} cma_state_t;

static com_th_t com_ths[MAX_COM_TH];
static wir_com_t wir_coms[MAX_WIR_COM];
static fib_com_t fib_coms[MAX_FIB_COM];
static com_net_t com_nets[MAX_COM_NET];
static com_sys_t com_syss[MAX_COM_SYS];
static cma_state_t cma;

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

int cma_init(void) {
    if (initialized) return -1;
    cma.n_com_th = 0; cma.n_wir_com = 0; cma.n_fib_com = 0;
    cma.n_com_net = 0; cma.n_com_sys = 0;
    cma.total_sig_sys = 0; cma.total_mob_com = 0;
    cma.total_fib_tra = 0; cma.total_tel_net = 0;
    cma.total_sat_nav = 0;
    for (int i = 0; i < MAX_COM_TH; i++) com_ths[i].active = 0;
    for (int i = 0; i < MAX_WIR_COM; i++) wir_coms[i].active = 0;
    for (int i = 0; i < MAX_FIB_COM; i++) fib_coms[i].active = 0;
    for (int i = 0; i < MAX_COM_NET; i++) com_nets[i].active = 0;
    for (int i = 0; i < MAX_COM_SYS; i++) com_syss[i].active = 0;
    initialized = 1;
    print_str("[CMA] Communications initialized\n");
    return 0;
}

int cma_com_th(int ct_type, int cat, int ssc, int mdm, int ccd, int stc, int year) {
    if (cma.n_com_th >= MAX_COM_TH) return -1;
    com_th_t* c = &com_ths[cma.n_com_th];
    c->ct_id = cma.n_com_th;
    c->ct_type = ct_type;
    c->ct_category = cat;
    c->sig_sys = ssc;
    c->mod_dem = mdm;
    c->cha_cod = ccd;
    c->syn_tec = stc;
    c->year = year;
    c->active = 1;
    cma.total_sig_sys += ssc;
    cma.n_com_th++;
    print_str("[CMA] Com th "); print_int(cma.n_com_th - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" ssc="); print_int(ssc);
    print_str(" mdm="); print_int(mdm);
    print_str(" ccd="); print_int(ccd);
    print_str(" stc="); print_int(stc); print_str("\n");
    return cma.n_com_th - 1;
}

int cma_wir_com(int wt_type, int cat, int mbc, int stc, int mcc, int anc, int year) {
    if (cma.n_wir_com >= MAX_WIR_COM) return -1;
    wir_com_t* w = &wir_coms[cma.n_wir_com];
    w->wc_id = cma.n_wir_com;
    w->wc_type = wt_type;
    w->wc_category = cat;
    w->mob_com = mbc;
    w->sat_com = stc;
    w->mic_com = mcc;
    w->ant_tec = anc;
    w->year = year;
    w->active = 1;
    cma.total_mob_com += mbc;
    cma.n_wir_com++;
    print_str("[CMA] Wir com "); print_int(cma.n_wir_com - 1);
    print_str(" type="); print_int(wt_type);
    print_str(" cat="); print_int(cat);
    print_str(" mbc="); print_int(mbc);
    print_str(" stc="); print_int(stc);
    print_str(" mcc="); print_int(mcc);
    print_str(" anc="); print_int(anc); print_str("\n");
    return cma.n_wir_com - 1;
}

int cma_fib_com(int ft_type, int cat, int fbr, int onw, int osw, int oac, int year) {
    if (cma.n_fib_com >= MAX_FIB_COM) return -1;
    fib_com_t* f = &fib_coms[cma.n_fib_com];
    f->fc_id = cma.n_fib_com;
    f->fc_type = ft_type;
    f->fc_category = cat;
    f->fib_tra = fbr;
    f->opt_net = onw;
    f->opt_swi = osw;
    f->opt_acc = oac;
    f->year = year;
    f->active = 1;
    cma.total_fib_tra += fbr;
    cma.n_fib_com++;
    print_str("[CMA] Fib com "); print_int(cma.n_fib_com - 1);
    print_str(" type="); print_int(ft_type);
    print_str(" cat="); print_int(cat);
    print_str(" fbr="); print_int(fbr);
    print_str(" onw="); print_int(onw);
    print_str(" osw="); print_int(osw);
    print_str(" oac="); print_int(oac); print_str("\n");
    return cma.n_fib_com - 1;
}

int cma_com_net(int nt_type, int cat, int tln, int itc, int iot, int fg, int year) {
    if (cma.n_com_net >= MAX_COM_NET) return -1;
    com_net_t* n = &com_nets[cma.n_com_net];
    n->cn_id = cma.n_com_net;
    n->cn_type = nt_type;
    n->cn_category = cat;
    n->tel_net = tln;
    n->int_tec = itc;
    n->iot_sys = iot;
    n->five_g = fg;
    n->year = year;
    n->active = 1;
    cma.total_tel_net += tln;
    cma.n_com_net++;
    print_str("[CMA] Com net "); print_int(cma.n_com_net - 1);
    print_str(" type="); print_int(nt_type);
    print_str(" cat="); print_int(cat);
    print_str(" tln="); print_int(tln);
    print_str(" itc="); print_int(itc);
    print_str(" iot="); print_int(iot);
    print_str(" fg="); print_int(fg); print_str("\n");
    return cma.n_com_net - 1;
}

int cma_com_sys(int st_type, int cat, int snv, int rds, int tlc, int sdr, int year) {
    if (cma.n_com_sys >= MAX_COM_SYS) return -1;
    com_sys_t* s = &com_syss[cma.n_com_sys];
    s->cs_id = cma.n_com_sys;
    s->cs_type = st_type;
    s->cs_category = cat;
    s->sat_nav = snv;
    s->rad_sys = rds;
    s->tel_con = tlc;
    s->sdr_sys = sdr;
    s->year = year;
    s->active = 1;
    cma.total_sat_nav += snv;
    cma.n_com_sys++;
    print_str("[CMA] Com sys "); print_int(cma.n_com_sys - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" snv="); print_int(snv);
    print_str(" rds="); print_int(rds);
    print_str(" tlc="); print_int(tlc);
    print_str(" sdr="); print_int(sdr); print_str("\n");
    return cma.n_com_sys - 1;
}

void cma_theory_report(void) {
    print_str("[CMA] Comm theory report:\n");
    print_str("  Theory categories: "); print_int(cma.n_com_th); print_str("\n");
    print_str("  Total signal systems: "); print_int(cma.total_sig_sys); print_str("\n");
}

void cma_wireless_report(void) {
    print_str("[CMA] Wireless comm report:\n");
    print_str("  Wireless categories: "); print_int(cma.n_wir_com); print_str("\n");
    print_str("  Total mobile comm: "); print_int(cma.total_mob_com); print_str("\n");
}

void cma_full_report(void) {
    print_str("[CMA] Full report:\n");
    print_str("  Fiber optic categories: "); print_int(cma.n_fib_com); print_str("\n");
    print_str("  Total fiber transmission: "); print_int(cma.total_fib_tra); print_str("\n");
    print_str("  Network categories: "); print_int(cma.n_com_net); print_str("\n");
    print_str("  Total telecom networks: "); print_int(cma.total_tel_net); print_str("\n");
    print_str("  System categories: "); print_int(cma.n_com_sys); print_str("\n");
    print_str("  Total satellite navigation: "); print_int(cma.total_sat_nav); print_str("\n");
}

void cma_print_state(void) {
    print_str("[CMA] Ct="); print_int(cma.n_com_th);
    print_str(" Wc="); print_int(cma.n_wir_com);
    print_str(" Fc="); print_int(cma.n_fib_com);
    print_str(" Nt="); print_int(cma.n_com_net);
    print_str(" Cs="); print_int(cma.n_com_sys);
    print_str("\n");
}

int main(void) {
    print_str("=== Communications Admin Demo ===\n\n");
    cma_init();

    print_str("Comm theory...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int ssc = 55 + (i * 13);
        int mdm = 40 + (i * 10);
        int ccd = 22 + (i * 5);
        int stc = 15 + (i * 3);
        int year = 2020 + (i % 5);
        cma_com_th(type, cat, ssc, mdm, ccd, stc, year);
    }

    print_str("\nWireless comm...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int mbc = 48 + (i * 11);
        int stc = 35 + (i * 8);
        int mcc = 20 + (i * 4);
        int anc = 12 + (i * 3);
        int year = 2021 + (i % 4);
        cma_wir_com(type, cat, mbc, stc, mcc, anc, year);
    }

    print_str("\nFiber optic comm...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int fbr = 42 + (i * 10);
        int onw = 28 + (i * 7);
        int osw = 18 + (i * 4);
        int oac = 10 + (i * 2);
        int year = 2022 + (i % 3);
        cma_fib_com(type, cat, fbr, onw, osw, oac, year);
    }

    print_str("\nComm networks...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int tln = 35 + (i * 8);
        int itc = 25 + (i * 6);
        int iot = 15 + (i * 3);
        int fg = 10 + (i * 2);
        int year = 2023 + (i % 2);
        cma_com_net(type, cat, tln, itc, iot, fg, year);
    }

    print_str("\nComm systems...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int snv = 30 + (i * 7);
        int rds = 22 + (i * 5);
        int tlc = 12 + (i * 3);
        int sdr = 8 + (i * 2);
        int year = 2024;
        cma_com_sys(type, cat, snv, rds, tlc, sdr, year);
    }

    print_str("\nTheory report...\n");
    cma_theory_report();

    print_str("\nWireless report...\n");
    cma_wireless_report();

    print_str("\nFull report...\n");
    cma_full_report();

    print_str("\nFinal state...\n");
    cma_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
