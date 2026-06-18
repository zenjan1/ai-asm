/* transportation_engineering_admin: Transportation engineering administration system (v1.0)
 * Traffic planning, traffic engineering, railway, aviation, port/shipping
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

#define MAX_TRAFF_PL    16
#define MAX_TRAFF_EN    14
#define MAX_RAILW       12
#define MAX_AVIAT       10
#define MAX_PORT_SH     10

typedef struct {
    int    tp_id;
    int    tp_type;
    int    tp_category;
    int    traffic_sur;
    int    traffic_for;
    int    road_net;
    int    year;
    int    active;
} traff_pl_t;

typedef struct {
    int    te_id;
    int    te_type;
    int    te_category;
    int    traffic_sig;
    int    traffic_sgn;
    int    traffic_mon;
    int    year;
    int    active;
} traff_en_t;

typedef struct {
    int    rw_id;
    int    rw_type;
    int    rw_category;
    int    railway_ln;
    int    railway_yd;
    int    railway_sg;
    int    year;
    int    active;
} railw_t;

typedef struct {
    int    av_id;
    int    av_type;
    int    av_category;
    int    airport_pl;
    int    air_traff;
    int    airline;
    int    year;
    int    active;
} aviat_t;

typedef struct {
    int    ps_id;
    int    ps_type;
    int    ps_category;
    int    port_pl;
    int    channel_eng;
    int    ship_tr;
    int    year;
    int    active;
} port_sh_t;

typedef struct {
    int    n_traff_pl;
    int    n_traff_en;
    int    n_railw;
    int    n_aviat;
    int    n_port_sh;
    int    total_traffic_sur;
    int    total_traffic_sig;
    int    total_railway_ln;
    int    total_airport_pl;
    int    total_port_pl;
} tea_state_t;

static traff_pl_t traff_pls[MAX_TRAFF_PL];
static traff_en_t traff_ens[MAX_TRAFF_EN];
static railw_t railws[MAX_RAILW];
static aviat_t aviats[MAX_AVIAT];
static port_sh_t port_shs[MAX_PORT_SH];
static tea_state_t tea;

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

int tea_init(void) {
    if (initialized) return -1;
    tea.n_traff_pl = 0; tea.n_traff_en = 0; tea.n_railw = 0;
    tea.n_aviat = 0; tea.n_port_sh = 0;
    tea.total_traffic_sur = 0; tea.total_traffic_sig = 0;
    tea.total_railway_ln = 0; tea.total_airport_pl = 0;
    tea.total_port_pl = 0;
    for (int i = 0; i < MAX_TRAFF_PL; i++) traff_pls[i].active = 0;
    for (int i = 0; i < MAX_TRAFF_EN; i++) traff_ens[i].active = 0;
    for (int i = 0; i < MAX_RAILW; i++) railws[i].active = 0;
    for (int i = 0; i < MAX_AVIAT; i++) aviats[i].active = 0;
    for (int i = 0; i < MAX_PORT_SH; i++) port_shs[i].active = 0;
    initialized = 1;
    print_str("[TEA] Transportation engineering initialized\n");
    return 0;
}

int tea_traff_pl(int pt_type, int cat, int trs, int trf, int rdn, int year) {
    if (tea.n_traff_pl >= MAX_TRAFF_PL) return -1;
    traff_pl_t* t = &traff_pls[tea.n_traff_pl];
    t->tp_id = tea.n_traff_pl;
    t->tp_type = pt_type;
    t->tp_category = cat;
    t->traffic_sur = trs;
    t->traffic_for = trf;
    t->road_net = rdn;
    t->year = year;
    t->active = 1;
    tea.total_traffic_sur += trs;
    tea.n_traff_pl++;
    print_str("[TEA] Traff pl "); print_int(tea.n_traff_pl - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" trs="); print_int(trs);
    print_str(" trf="); print_int(trf);
    print_str(" rdn="); print_int(rdn); print_str("\n");
    return tea.n_traff_pl - 1;
}

int tea_traff_en(int tt_type, int cat, int tsg, int tsn, int tmn, int year) {
    if (tea.n_traff_en >= MAX_TRAFF_EN) return -1;
    traff_en_t* t = &traff_ens[tea.n_traff_en];
    t->te_id = tea.n_traff_en;
    t->te_type = tt_type;
    t->te_category = cat;
    t->traffic_sig = tsg;
    t->traffic_sgn = tsn;
    t->traffic_mon = tmn;
    t->year = year;
    t->active = 1;
    tea.total_traffic_sig += tsg;
    tea.n_traff_en++;
    print_str("[TEA] Traff en "); print_int(tea.n_traff_en - 1);
    print_str(" type="); print_int(tt_type);
    print_str(" cat="); print_int(cat);
    print_str(" tsg="); print_int(tsg);
    print_str(" tsn="); print_int(tsn);
    print_str(" tmn="); print_int(tmn); print_str("\n");
    return tea.n_traff_en - 1;
}

int tea_railw(int rt_type, int cat, int rln, int ryn, int rsg, int year) {
    if (tea.n_railw >= MAX_RAILW) return -1;
    railw_t* r = &railws[tea.n_railw];
    r->rw_id = tea.n_railw;
    r->rw_type = rt_type;
    r->rw_category = cat;
    r->railway_ln = rln;
    r->railway_yd = ryn;
    r->railway_sg = rsg;
    r->year = year;
    r->active = 1;
    tea.total_railway_ln += rln;
    tea.n_railw++;
    print_str("[TEA] Railw "); print_int(tea.n_railw - 1);
    print_str(" type="); print_int(rt_type);
    print_str(" cat="); print_int(cat);
    print_str(" rln="); print_int(rln);
    print_str(" ryn="); print_int(ryn);
    print_str(" rsg="); print_int(rsg); print_str("\n");
    return tea.n_railw - 1;
}

int tea_aviat(int at_type, int cat, int arp, int atr, int aln, int year) {
    if (tea.n_aviat >= MAX_AVIAT) return -1;
    aviat_t* a = &aviats[tea.n_aviat];
    a->av_id = tea.n_aviat;
    a->av_type = at_type;
    a->av_category = cat;
    a->airport_pl = arp;
    a->air_traff = atr;
    a->airline = aln;
    a->year = year;
    a->active = 1;
    tea.total_airport_pl += arp;
    tea.n_aviat++;
    print_str("[TEA] Aviat "); print_int(tea.n_aviat - 1);
    print_str(" type="); print_int(at_type);
    print_str(" cat="); print_int(cat);
    print_str(" arp="); print_int(arp);
    print_str(" atr="); print_int(atr);
    print_str(" aln="); print_int(aln); print_str("\n");
    return tea.n_aviat - 1;
}

int tea_port_sh(int pt_type, int cat, int ppl, int che, int str, int year) {
    if (tea.n_port_sh >= MAX_PORT_SH) return -1;
    port_sh_t* p = &port_shs[tea.n_port_sh];
    p->ps_id = tea.n_port_sh;
    p->ps_type = pt_type;
    p->ps_category = cat;
    p->port_pl = ppl;
    p->channel_eng = che;
    p->ship_tr = str;
    p->year = year;
    p->active = 1;
    tea.total_port_pl += ppl;
    tea.n_port_sh++;
    print_str("[TEA] Port sh "); print_int(tea.n_port_sh - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" ppl="); print_int(ppl);
    print_str(" che="); print_int(che);
    print_str(" str="); print_int(str); print_str("\n");
    return tea.n_port_sh - 1;
}

void tea_traff_pl_report(void) {
    print_str("[TEA] Traffic planning report:\n");
    print_str("  Traffic planning categories: "); print_int(tea.n_traff_pl); print_str("\n");
    print_str("  Total traffic survey: "); print_int(tea.total_traffic_sur); print_str("\n");
}

void tea_traff_en_report(void) {
    print_str("[TEA] Traffic engineering report:\n");
    print_str("  Traffic engineering categories: "); print_int(tea.n_traff_en); print_str("\n");
    print_str("  Total traffic signals: "); print_int(tea.total_traffic_sig); print_str("\n");
}

void tea_full_report(void) {
    print_str("[TEA] Full report:\n");
    print_str("  Railway categories: "); print_int(tea.n_railw); print_str("\n");
    print_str("  Total railway line: "); print_int(tea.total_railway_ln); print_str("\n");
    print_str("  Aviation categories: "); print_int(tea.n_aviat); print_str("\n");
    print_str("  Total airport planning: "); print_int(tea.total_airport_pl); print_str("\n");
    print_str("  Port/shipping categories: "); print_int(tea.n_port_sh); print_str("\n");
    print_str("  Total port planning: "); print_int(tea.total_port_pl); print_str("\n");
}

void tea_print_state(void) {
    print_str("[TEA] Tp="); print_int(tea.n_traff_pl);
    print_str(" Te="); print_int(tea.n_traff_en);
    print_str(" Rw="); print_int(tea.n_railw);
    print_str(" Av="); print_int(tea.n_aviat);
    print_str(" Ps="); print_int(tea.n_port_sh);
    print_str("\n");
}

int main(void) {
    print_str("=== Transportation Engineering Admin Demo ===\n\n");
    tea_init();

    print_str("Traffic planning...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int trs = 55 + (i * 13);
        int trf = 40 + (i * 10);
        int rdn = 22 + (i * 5);
        int year = 2020 + (i % 5);
        tea_traff_pl(type, cat, trs, trf, rdn, year);
    }

    print_str("\nTraffic engineering...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int tsg = 48 + (i * 11);
        int tsn = 35 + (i * 8);
        int tmn = 20 + (i * 4);
        int year = 2021 + (i % 4);
        tea_traff_en(type, cat, tsg, tsn, tmn, year);
    }

    print_str("\nRailway...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int rln = 42 + (i * 10);
        int ryn = 28 + (i * 7);
        int rsg = 18 + (i * 4);
        int year = 2022 + (i % 3);
        tea_railw(type, cat, rln, ryn, rsg, year);
    }

    print_str("\nAviation...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int arp = 35 + (i * 8);
        int atr = 25 + (i * 6);
        int aln = 15 + (i * 3);
        int year = 2023 + (i % 2);
        tea_aviat(type, cat, arp, atr, aln, year);
    }

    print_str("\nPort/shipping...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ppl = 30 + (i * 7);
        int che = 22 + (i * 5);
        int str = 12 + (i * 3);
        int year = 2024;
        tea_port_sh(type, cat, ppl, che, str, year);
    }

    print_str("\nTraffic planning report...\n");
    tea_traff_pl_report();

    print_str("\nTraffic engineering report...\n");
    tea_traff_en_report();

    print_str("\nFull report...\n");
    tea_full_report();

    print_str("\nFinal state...\n");
    tea_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
