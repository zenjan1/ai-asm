/* sports_industry_admin: Sports industry administration system (v1.0)
 * Sports economics, sports management, sports marketing, sports events, sports media
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

#define MAX_SPO_ECON   16
#define MAX_SPO_MAN    14
#define MAX_SPO_MKT    12
#define MAX_SPO_EVT    10
#define MAX_SPO_MED    10

typedef struct {
    int    se_id;
    int    se_type;
    int    se_category;
    int    spo_econ;
    int    spo_mar;
    int    spo_con;
    int    spo_inv;
    int    year;
    int    active;
} spo_econ_t;

typedef struct {
    int    sm_id;
    int    sm_type;
    int    sm_category;
    int    org_man;
    int    evt_man;
    int    ven_man;
    int    spo_hr;
    int    year;
    int    active;
} spo_man_t;

typedef struct {
    int    mk_id;
    int    mk_type;
    int    mk_category;
    int    spo_bra;
    int    spo_spo;
    int    spo_med;
    int    spo_age;
    int    year;
    int    active;
} spo_mkt_t;

typedef struct {
    int    ev_id;
    int    ev_type;
    int    ev_category;
    int    evt_pla;
    int    evt_opr;
    int    evt_bro;
    int    evt_spo2;
    int    year;
    int    active;
} spo_evt_t;

typedef struct {
    int    me_id;
    int    me_type;
    int    me_category;
    int    spo_new;
    int    spo_bro2;
    int    spo_self;
    int    spo_dat;
    int    year;
    int    active;
} spo_med_t;

typedef struct {
    int    n_spo_econ;
    int    n_spo_man;
    int    n_spo_mkt;
    int    n_spo_evt;
    int    n_spo_med;
    int    total_spo_econ;
    int    total_org_man;
    int    total_spo_bra;
    int    total_evt_pla;
    int    total_spo_new;
} sia_state_t;

static spo_econ_t spo_econs[MAX_SPO_ECON];
static spo_man_t spo_mans[MAX_SPO_MAN];
static spo_mkt_t spo_mkts[MAX_SPO_MKT];
static spo_evt_t spo_evts[MAX_SPO_EVT];
static spo_med_t spo_meds[MAX_SPO_MED];
static sia_state_t sia;

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

int sia_init(void) {
    if (initialized) return -1;
    sia.n_spo_econ = 0; sia.n_spo_man = 0; sia.n_spo_mkt = 0;
    sia.n_spo_evt = 0; sia.n_spo_med = 0;
    sia.total_spo_econ = 0; sia.total_org_man = 0;
    sia.total_spo_bra = 0; sia.total_evt_pla = 0;
    sia.total_spo_new = 0;
    for (int i = 0; i < MAX_SPO_ECON; i++) spo_econs[i].active = 0;
    for (int i = 0; i < MAX_SPO_MAN; i++) spo_mans[i].active = 0;
    for (int i = 0; i < MAX_SPO_MKT; i++) spo_mkts[i].active = 0;
    for (int i = 0; i < MAX_SPO_EVT; i++) spo_evts[i].active = 0;
    for (int i = 0; i < MAX_SPO_MED; i++) spo_meds[i].active = 0;
    initialized = 1;
    print_str("[SIA] Sports industry initialized\n");
    return 0;
}

int sia_spo_econ(int st_type, int cat, int secon, int smar, int scon, int sinv, int year) {
    if (sia.n_spo_econ >= MAX_SPO_ECON) return -1;
    spo_econ_t* s = &spo_econs[sia.n_spo_econ];
    s->se_id = sia.n_spo_econ;
    s->se_type = st_type;
    s->se_category = cat;
    s->spo_econ = secon;
    s->spo_mar = smar;
    s->spo_con = scon;
    s->spo_inv = sinv;
    s->year = year;
    s->active = 1;
    sia.total_spo_econ += secon;
    sia.n_spo_econ++;
    print_str("[SIA] Spo econ "); print_int(sia.n_spo_econ - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" secon="); print_int(secon);
    print_str(" smar="); print_int(smar);
    print_str(" scon="); print_int(scon);
    print_str(" sinv="); print_int(sinv); print_str("\n");
    return sia.n_spo_econ - 1;
}

int sia_spo_man(int mt_type, int cat, int oman, int eman, int vman, int shr, int year) {
    if (sia.n_spo_man >= MAX_SPO_MAN) return -1;
    spo_man_t* m = &spo_mans[sia.n_spo_man];
    m->sm_id = sia.n_spo_man;
    m->sm_type = mt_type;
    m->sm_category = cat;
    m->org_man = oman;
    m->evt_man = eman;
    m->ven_man = vman;
    m->spo_hr = shr;
    m->year = year;
    m->active = 1;
    sia.total_org_man += oman;
    sia.n_spo_man++;
    print_str("[SIA] Spo man "); print_int(sia.n_spo_man - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" oman="); print_int(oman);
    print_str(" eman="); print_int(eman);
    print_str(" vman="); print_int(vman);
    print_str(" shr="); print_int(shr); print_str("\n");
    return sia.n_spo_man - 1;
}

int sia_spo_mkt(int kt_type, int cat, int sbra, int sspo, int smed, int sage, int year) {
    if (sia.n_spo_mkt >= MAX_SPO_MKT) return -1;
    spo_mkt_t* m = &spo_mkts[sia.n_spo_mkt];
    m->mk_id = sia.n_spo_mkt;
    m->mk_type = kt_type;
    m->mk_category = cat;
    m->spo_bra = sbra;
    m->spo_spo = sspo;
    m->spo_med = smed;
    m->spo_age = sage;
    m->year = year;
    m->active = 1;
    sia.total_spo_bra += sbra;
    sia.n_spo_mkt++;
    print_str("[SIA] Spo mkt "); print_int(sia.n_spo_mkt - 1);
    print_str(" type="); print_int(kt_type);
    print_str(" cat="); print_int(cat);
    print_str(" sbra="); print_int(sbra);
    print_str(" sspo="); print_int(sspo);
    print_str(" smed="); print_int(smed);
    print_str(" sage="); print_int(sage); print_str("\n");
    return sia.n_spo_mkt - 1;
}

int sia_spo_evt(int et_type, int cat, int epla, int eopr, int ebro, int espo, int year) {
    if (sia.n_spo_evt >= MAX_SPO_EVT) return -1;
    spo_evt_t* e = &spo_evts[sia.n_spo_evt];
    e->ev_id = sia.n_spo_evt;
    e->ev_type = et_type;
    e->ev_category = cat;
    e->evt_pla = epla;
    e->evt_opr = eopr;
    e->evt_bro = ebro;
    e->evt_spo2 = espo;
    e->year = year;
    e->active = 1;
    sia.total_evt_pla += epla;
    sia.n_spo_evt++;
    print_str("[SIA] Spo evt "); print_int(sia.n_spo_evt - 1);
    print_str(" type="); print_int(et_type);
    print_str(" cat="); print_int(cat);
    print_str(" epla="); print_int(epla);
    print_str(" eopr="); print_int(eopr);
    print_str(" ebro="); print_int(ebro);
    print_str(" espo="); print_int(espo); print_str("\n");
    return sia.n_spo_evt - 1;
}

int sia_spo_med(int md_type, int cat, int snew, int sbro, int sself, int sdat, int year) {
    if (sia.n_spo_med >= MAX_SPO_MED) return -1;
    spo_med_t* m = &spo_meds[sia.n_spo_med];
    m->me_id = sia.n_spo_med;
    m->me_type = md_type;
    m->me_category = cat;
    m->spo_new = snew;
    m->spo_bro2 = sbro;
    m->spo_self = sself;
    m->spo_dat = sdat;
    m->year = year;
    m->active = 1;
    sia.total_spo_new += snew;
    sia.n_spo_med++;
    print_str("[SIA] Spo med "); print_int(sia.n_spo_med - 1);
    print_str(" type="); print_int(md_type);
    print_str(" cat="); print_int(cat);
    print_str(" snew="); print_int(snew);
    print_str(" sbro="); print_int(sbro);
    print_str(" sself="); print_int(sself);
    print_str(" sdat="); print_int(sdat); print_str("\n");
    return sia.n_spo_med - 1;
}

void sia_economics_report(void) {
    print_str("[SIA] Sports economics report:\n");
    print_str("  Economics categories: "); print_int(sia.n_spo_econ); print_str("\n");
    print_str("  Total sports economics: "); print_int(sia.total_spo_econ); print_str("\n");
}

void sia_management_report(void) {
    print_str("[SIA] Sports management report:\n");
    print_str("  Management categories: "); print_int(sia.n_spo_man); print_str("\n");
    print_str("  Total org management: "); print_int(sia.total_org_man); print_str("\n");
}

void sia_full_report(void) {
    print_str("[SIA] Full report:\n");
    print_str("  Marketing categories: "); print_int(sia.n_spo_mkt); print_str("\n");
    print_str("  Total sports brands: "); print_int(sia.total_spo_bra); print_str("\n");
    print_str("  Events categories: "); print_int(sia.n_spo_evt); print_str("\n");
    print_str("  Total event planning: "); print_int(sia.total_evt_pla); print_str("\n");
    print_str("  Media categories: "); print_int(sia.n_spo_med); print_str("\n");
    print_str("  Total sports news: "); print_int(sia.total_spo_new); print_str("\n");
}

void sia_print_state(void) {
    print_str("[SIA] Se="); print_int(sia.n_spo_econ);
    print_str(" Sm="); print_int(sia.n_spo_man);
    print_str(" Mk="); print_int(sia.n_spo_mkt);
    print_str(" Ev="); print_int(sia.n_spo_evt);
    print_str(" Me="); print_int(sia.n_spo_med);
    print_str("\n");
}

int main(void) {
    print_str("=== Sports Industry Admin Demo ===\n\n");
    sia_init();

    print_str("Sports economics...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int secon = 55 + (i * 13);
        int smar = 40 + (i * 10);
        int scon = 22 + (i * 5);
        int sinv = 15 + (i * 3);
        int year = 2020 + (i % 5);
        sia_spo_econ(type, cat, secon, smar, scon, sinv, year);
    }

    print_str("\nSports management...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int oman = 48 + (i * 11);
        int eman = 35 + (i * 8);
        int vman = 20 + (i * 4);
        int shr = 12 + (i * 3);
        int year = 2021 + (i % 4);
        sia_spo_man(type, cat, oman, eman, vman, shr, year);
    }

    print_str("\nSports marketing...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int sbra = 42 + (i * 10);
        int sspo = 28 + (i * 7);
        int smed = 18 + (i * 4);
        int sage = 10 + (i * 2);
        int year = 2022 + (i % 3);
        sia_spo_mkt(type, cat, sbra, sspo, smed, sage, year);
    }

    print_str("\nSports events...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int epla = 35 + (i * 8);
        int eopr = 25 + (i * 6);
        int ebro = 15 + (i * 3);
        int espo = 10 + (i * 2);
        int year = 2023 + (i % 2);
        sia_spo_evt(type, cat, epla, eopr, ebro, espo, year);
    }

    print_str("\nSports media...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int snew = 30 + (i * 7);
        int sbro = 22 + (i * 5);
        int sself = 12 + (i * 3);
        int sdat = 8 + (i * 2);
        int year = 2024;
        sia_spo_med(type, cat, snew, sbro, sself, sdat, year);
    }

    print_str("\nEconomics report...\n");
    sia_economics_report();

    print_str("\nManagement report...\n");
    sia_management_report();

    print_str("\nFull report...\n");
    sia_full_report();

    print_str("\nFinal state...\n");
    sia_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
