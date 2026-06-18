/* computer_admin: Computer science administration system (v1.0)
 * Architecture, software, networks, AI, computer science theory
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

#define MAX_ARCH      16
#define MAX_SOFT      14
#define MAX_NET       12
#define MAX_AI        10
#define MAX_CS_TH     10

typedef struct {
    int    ar_id;
    int    ar_type;
    int    ar_category;
    int    ins_sys;
    int    mem_sys;
    int    bus_sys;
    int    par_proc;
    int    year;
    int    active;
} arch_t;

typedef struct {
    int    sf_id;
    int    sf_type;
    int    sf_category;
    int    op_sys;
    int    com_pri;
    int    dat_sys;
    int    sof_eng;
    int    year;
    int    active;
} soft_t;

typedef struct {
    int    nt_id;
    int    nt_type;
    int    nt_category;
    int    net_pro;
    int    net_arc;
    int    net_sec;
    int    dis_sys;
    int    year;
    int    active;
} net_t;

typedef struct {
    int    ai_id;
    int    ai_type;
    int    ai_category;
    int    mac_lea;
    int    dee_lea;
    int    nlp_sys;
    int    com_vis;
    int    year;
    int    active;
} ai_t;

typedef struct {
    int    ct_id;
    int    ct_type;
    int    ct_category;
    int    alg_des;
    int    dat_str;
    int    com_the;
    int    for_lan;
    int    year;
    int    active;
} cs_th_t;

typedef struct {
    int    n_arch;
    int    n_soft;
    int    n_net;
    int    n_ai;
    int    n_cs_th;
    int    total_ins_sys;
    int    total_op_sys;
    int    total_net_pro;
    int    total_mac_lea;
    int    total_alg_des;
} cpa_state_t;

static arch_t archs[MAX_ARCH];
static soft_t softs[MAX_SOFT];
static net_t nets[MAX_NET];
static ai_t ais[MAX_AI];
static cs_th_t cs_ths[MAX_CS_TH];
static cpa_state_t cpa;

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

int cpa_init(void) {
    if (initialized) return -1;
    cpa.n_arch = 0; cpa.n_soft = 0; cpa.n_net = 0;
    cpa.n_ai = 0; cpa.n_cs_th = 0;
    cpa.total_ins_sys = 0; cpa.total_op_sys = 0;
    cpa.total_net_pro = 0; cpa.total_mac_lea = 0;
    cpa.total_alg_des = 0;
    for (int i = 0; i < MAX_ARCH; i++) archs[i].active = 0;
    for (int i = 0; i < MAX_SOFT; i++) softs[i].active = 0;
    for (int i = 0; i < MAX_NET; i++) nets[i].active = 0;
    for (int i = 0; i < MAX_AI; i++) ais[i].active = 0;
    for (int i = 0; i < MAX_CS_TH; i++) cs_ths[i].active = 0;
    initialized = 1;
    print_str("[CPA] Computer initialized\n");
    return 0;
}

int cpa_arch(int at_type, int cat, int isy, int msy, int bsy, int ppr, int year) {
    if (cpa.n_arch >= MAX_ARCH) return -1;
    arch_t* a = &archs[cpa.n_arch];
    a->ar_id = cpa.n_arch;
    a->ar_type = at_type;
    a->ar_category = cat;
    a->ins_sys = isy;
    a->mem_sys = msy;
    a->bus_sys = bsy;
    a->par_proc = ppr;
    a->year = year;
    a->active = 1;
    cpa.total_ins_sys += isy;
    cpa.n_arch++;
    print_str("[CPA] Arch "); print_int(cpa.n_arch - 1);
    print_str(" type="); print_int(at_type);
    print_str(" cat="); print_int(cat);
    print_str(" isy="); print_int(isy);
    print_str(" msy="); print_int(msy);
    print_str(" bsy="); print_int(bsy);
    print_str(" ppr="); print_int(ppr); print_str("\n");
    return cpa.n_arch - 1;
}

int cpa_soft(int st_type, int cat, int osy, int cpr, int dsy, int seng, int year) {
    if (cpa.n_soft >= MAX_SOFT) return -1;
    soft_t* s = &softs[cpa.n_soft];
    s->sf_id = cpa.n_soft;
    s->sf_type = st_type;
    s->sf_category = cat;
    s->op_sys = osy;
    s->com_pri = cpr;
    s->dat_sys = dsy;
    s->sof_eng = seng;
    s->year = year;
    s->active = 1;
    cpa.total_op_sys += osy;
    cpa.n_soft++;
    print_str("[CPA] Soft "); print_int(cpa.n_soft - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" osy="); print_int(osy);
    print_str(" cpr="); print_int(cpr);
    print_str(" dsy="); print_int(dsy);
    print_str(" seng="); print_int(seng); print_str("\n");
    return cpa.n_soft - 1;
}

int cpa_net(int nt_type, int cat, int npr, int nrc, int nsc, int dsc, int year) {
    if (cpa.n_net >= MAX_NET) return -1;
    net_t* n = &nets[cpa.n_net];
    n->nt_id = cpa.n_net;
    n->nt_type = nt_type;
    n->nt_category = cat;
    n->net_pro = npr;
    n->net_arc = nrc;
    n->net_sec = nsc;
    n->dis_sys = dsc;
    n->year = year;
    n->active = 1;
    cpa.total_net_pro += npr;
    cpa.n_net++;
    print_str("[CPA] Net "); print_int(cpa.n_net - 1);
    print_str(" type="); print_int(nt_type);
    print_str(" cat="); print_int(cat);
    print_str(" npr="); print_int(npr);
    print_str(" nrc="); print_int(nrc);
    print_str(" nsc="); print_int(nsc);
    print_str(" dsc="); print_int(dsc); print_str("\n");
    return cpa.n_net - 1;
}

int cpa_ai(int at_type, int cat, int ml, int dl, int nlp, int cv, int year) {
    if (cpa.n_ai >= MAX_AI) return -1;
    ai_t* a = &ais[cpa.n_ai];
    a->ai_id = cpa.n_ai;
    a->ai_type = at_type;
    a->ai_category = cat;
    a->mac_lea = ml;
    a->dee_lea = dl;
    a->nlp_sys = nlp;
    a->com_vis = cv;
    a->year = year;
    a->active = 1;
    cpa.total_mac_lea += ml;
    cpa.n_ai++;
    print_str("[CPA] AI "); print_int(cpa.n_ai - 1);
    print_str(" type="); print_int(at_type);
    print_str(" cat="); print_int(cat);
    print_str(" ml="); print_int(ml);
    print_str(" dl="); print_int(dl);
    print_str(" nlp="); print_int(nlp);
    print_str(" cv="); print_int(cv); print_str("\n");
    return cpa.n_ai - 1;
}

int cpa_cs_th(int ct_type, int cat, int ads, int dsc, int cth, int fln, int year) {
    if (cpa.n_cs_th >= MAX_CS_TH) return -1;
    cs_th_t* c = &cs_ths[cpa.n_cs_th];
    c->ct_id = cpa.n_cs_th;
    c->ct_type = ct_type;
    c->ct_category = cat;
    c->alg_des = ads;
    c->dat_str = dsc;
    c->com_the = cth;
    c->for_lan = fln;
    c->year = year;
    c->active = 1;
    cpa.total_alg_des += ads;
    cpa.n_cs_th++;
    print_str("[CPA] CS th "); print_int(cpa.n_cs_th - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" ads="); print_int(ads);
    print_str(" dsc="); print_int(dsc);
    print_str(" cth="); print_int(cth);
    print_str(" fln="); print_int(fln); print_str("\n");
    return cpa.n_cs_th - 1;
}

void cpa_arch_report(void) {
    print_str("[CPA] Architecture report:\n");
    print_str("  Architecture categories: "); print_int(cpa.n_arch); print_str("\n");
    print_str("  Total instruction systems: "); print_int(cpa.total_ins_sys); print_str("\n");
}

void cpa_soft_report(void) {
    print_str("[CPA] Software report:\n");
    print_str("  Software categories: "); print_int(cpa.n_soft); print_str("\n");
    print_str("  Total operating systems: "); print_int(cpa.total_op_sys); print_str("\n");
}

void cpa_full_report(void) {
    print_str("[CPA] Full report:\n");
    print_str("  Network categories: "); print_int(cpa.n_net); print_str("\n");
    print_str("  Total network protocols: "); print_int(cpa.total_net_pro); print_str("\n");
    print_str("  AI categories: "); print_int(cpa.n_ai); print_str("\n");
    print_str("  Total machine learning: "); print_int(cpa.total_mac_lea); print_str("\n");
    print_str("  CS theory categories: "); print_int(cpa.n_cs_th); print_str("\n");
    print_str("  Total algorithm design: "); print_int(cpa.total_alg_des); print_str("\n");
}

void cpa_print_state(void) {
    print_str("[CPA] Ar="); print_int(cpa.n_arch);
    print_str(" Sf="); print_int(cpa.n_soft);
    print_str(" Nt="); print_int(cpa.n_net);
    print_str(" Ai="); print_int(cpa.n_ai);
    print_str(" Ct="); print_int(cpa.n_cs_th);
    print_str("\n");
}

int main(void) {
    print_str("=== Computer Admin Demo ===\n\n");
    cpa_init();

    print_str("Architecture...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int isy = 55 + (i * 13);
        int msy = 40 + (i * 10);
        int bsy = 22 + (i * 5);
        int ppr = 15 + (i * 3);
        int year = 2020 + (i % 5);
        cpa_arch(type, cat, isy, msy, bsy, ppr, year);
    }

    print_str("\nSoftware...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int osy = 48 + (i * 11);
        int cpr = 35 + (i * 8);
        int dsy = 20 + (i * 4);
        int seng = 12 + (i * 3);
        int year = 2021 + (i % 4);
        cpa_soft(type, cat, osy, cpr, dsy, seng, year);
    }

    print_str("\nNetworks...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int npr = 42 + (i * 10);
        int nrc = 28 + (i * 7);
        int nsc = 18 + (i * 4);
        int dsc = 10 + (i * 2);
        int year = 2022 + (i % 3);
        cpa_net(type, cat, npr, nrc, nsc, dsc, year);
    }

    print_str("\nAI...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ml = 35 + (i * 8);
        int dl = 25 + (i * 6);
        int nlp = 15 + (i * 3);
        int cv = 10 + (i * 2);
        int year = 2023 + (i % 2);
        cpa_ai(type, cat, ml, dl, nlp, cv, year);
    }

    print_str("\nCS theory...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ads = 30 + (i * 7);
        int dsc = 22 + (i * 5);
        int cth = 12 + (i * 3);
        int fln = 8 + (i * 2);
        int year = 2024;
        cpa_cs_th(type, cat, ads, dsc, cth, fln, year);
    }

    print_str("\nArchitecture report...\n");
    cpa_arch_report();

    print_str("\nSoftware report...\n");
    cpa_soft_report();

    print_str("\nFull report...\n");
    cpa_full_report();

    print_str("\nFinal state...\n");
    cpa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
