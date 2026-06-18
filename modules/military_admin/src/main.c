/* military_admin: Military science administration system (v1.0)
 * Military thought, strategy, operations, logistics, political work
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

#define MAX_MIL_TH      16
#define MAX_MIL_STR     14
#define MAX_MIL_CAMP    12
#define MAX_MIL_LOG     10
#define MAX_MIL_POL     10

typedef struct {
    int    mt_id;
    int    mt_type;
    int    mt_category;
    int    mil_pri;
    int    strat_th;
    int    tact_th;
    int    mil_phi;
    int    year;
    int    active;
} mil_th_t;

typedef struct {
    int    ms_id;
    int    ms_type;
    int    ms_category;
    int    str_th;
    int    str_plan;
    int    str_eva;
    int    str_sup;
    int    year;
    int    active;
} mil_str_t;

typedef struct {
    int    mc_id;
    int    mc_type;
    int    mc_category;
    int    camp_th;
    int    camp_cmd;
    int    camp_sup;
    int    camp_eva;
    int    year;
    int    active;
} mil_camp_t;

typedef struct {
    int    lg_id;
    int    lg_type;
    int    lg_category;
    int    log_sup;
    int    equip;
    int    supply;
    int    mil_tra;
    int    year;
    int    active;
} mil_log_t;

typedef struct {
    int    mp_id;
    int    mp_type;
    int    mp_category;
    int    pol_pri;
    int    org_con;
    int    prop_work;
    int    sec_work;
    int    year;
    int    active;
} mil_pol_t;

typedef struct {
    int    n_mil_th;
    int    n_mil_str;
    int    n_mil_camp;
    int    n_mil_log;
    int    n_mil_pol;
    int    total_mil_pri;
    int    total_str_th;
    int    total_camp_th;
    int    total_log_sup;
    int    total_pol_pri;
} mla_state_t;

static mil_th_t mil_ths[MAX_MIL_TH];
static mil_str_t mil_strs[MAX_MIL_STR];
static mil_camp_t mil_camps[MAX_MIL_CAMP];
static mil_log_t mil_logs[MAX_MIL_LOG];
static mil_pol_t mil_pols[MAX_MIL_POL];
static mla_state_t mla;

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

int mla_init(void) {
    if (initialized) return -1;
    mla.n_mil_th = 0; mla.n_mil_str = 0; mla.n_mil_camp = 0;
    mla.n_mil_log = 0; mla.n_mil_pol = 0;
    mla.total_mil_pri = 0; mla.total_str_th = 0;
    mla.total_camp_th = 0; mla.total_log_sup = 0;
    mla.total_pol_pri = 0;
    for (int i = 0; i < MAX_MIL_TH; i++) mil_ths[i].active = 0;
    for (int i = 0; i < MAX_MIL_STR; i++) mil_strs[i].active = 0;
    for (int i = 0; i < MAX_MIL_CAMP; i++) mil_camps[i].active = 0;
    for (int i = 0; i < MAX_MIL_LOG; i++) mil_logs[i].active = 0;
    for (int i = 0; i < MAX_MIL_POL; i++) mil_pols[i].active = 0;
    initialized = 1;
    print_str("[MLA] Military initialized\n");
    return 0;
}

int mla_mil_th(int mt_type, int cat, int mpr, int sth, int tth, int mph, int year) {
    if (mla.n_mil_th >= MAX_MIL_TH) return -1;
    mil_th_t* m = &mil_ths[mla.n_mil_th];
    m->mt_id = mla.n_mil_th;
    m->mt_type = mt_type;
    m->mt_category = cat;
    m->mil_pri = mpr;
    m->strat_th = sth;
    m->tact_th = tth;
    m->mil_phi = mph;
    m->year = year;
    m->active = 1;
    mla.total_mil_pri += mpr;
    mla.n_mil_th++;
    print_str("[MLA] Mil th "); print_int(mla.n_mil_th - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" mpr="); print_int(mpr);
    print_str(" sth="); print_int(sth);
    print_str(" tth="); print_int(tth);
    print_str(" mph="); print_int(mph); print_str("\n");
    return mla.n_mil_th - 1;
}

int mla_mil_str(int st_type, int cat, int sth, int spl, int sev, int ssu, int year) {
    if (mla.n_mil_str >= MAX_MIL_STR) return -1;
    mil_str_t* s = &mil_strs[mla.n_mil_str];
    s->ms_id = mla.n_mil_str;
    s->ms_type = st_type;
    s->ms_category = cat;
    s->str_th = sth;
    s->str_plan = spl;
    s->str_eva = sev;
    s->str_sup = ssu;
    s->year = year;
    s->active = 1;
    mla.total_str_th += sth;
    mla.n_mil_str++;
    print_str("[MLA] Mil str "); print_int(mla.n_mil_str - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" sth="); print_int(sth);
    print_str(" spl="); print_int(spl);
    print_str(" sev="); print_int(sev);
    print_str(" ssu="); print_int(ssu); print_str("\n");
    return mla.n_mil_str - 1;
}

int mla_mil_camp(int ct_type, int cat, int cth, int ccm, int csu, int cev, int year) {
    if (mla.n_mil_camp >= MAX_MIL_CAMP) return -1;
    mil_camp_t* c = &mil_camps[mla.n_mil_camp];
    c->mc_id = mla.n_mil_camp;
    c->mc_type = ct_type;
    c->mc_category = cat;
    c->camp_th = cth;
    c->camp_cmd = ccm;
    c->camp_sup = csu;
    c->camp_eva = cev;
    c->year = year;
    c->active = 1;
    mla.total_camp_th += cth;
    mla.n_mil_camp++;
    print_str("[MLA] Mil camp "); print_int(mla.n_mil_camp - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" cth="); print_int(cth);
    print_str(" ccm="); print_int(ccm);
    print_str(" csu="); print_int(csu);
    print_str(" cev="); print_int(cev); print_str("\n");
    return mla.n_mil_camp - 1;
}

int mla_mil_log(int lt_type, int cat, int lgs, int equ, int sup, int mtr, int year) {
    if (mla.n_mil_log >= MAX_MIL_LOG) return -1;
    mil_log_t* l = &mil_logs[mla.n_mil_log];
    l->lg_id = mla.n_mil_log;
    l->lg_type = lt_type;
    l->lg_category = cat;
    l->log_sup = lgs;
    l->equip = equ;
    l->supply = sup;
    l->mil_tra = mtr;
    l->year = year;
    l->active = 1;
    mla.total_log_sup += lgs;
    mla.n_mil_log++;
    print_str("[MLA] Mil log "); print_int(mla.n_mil_log - 1);
    print_str(" type="); print_int(lt_type);
    print_str(" cat="); print_int(cat);
    print_str(" lgs="); print_int(lgs);
    print_str(" equ="); print_int(equ);
    print_str(" sup="); print_int(sup);
    print_str(" mtr="); print_int(mtr); print_str("\n");
    return mla.n_mil_log - 1;
}

int mla_mil_pol(int pt_type, int cat, int ppr, int ocn, int prp, int scw, int year) {
    if (mla.n_mil_pol >= MAX_MIL_POL) return -1;
    mil_pol_t* p = &mil_pols[mla.n_mil_pol];
    p->mp_id = mla.n_mil_pol;
    p->mp_type = pt_type;
    p->mp_category = cat;
    p->pol_pri = ppr;
    p->org_con = ocn;
    p->prop_work = prp;
    p->sec_work = scw;
    p->year = year;
    p->active = 1;
    mla.total_pol_pri += ppr;
    mla.n_mil_pol++;
    print_str("[MLA] Mil pol "); print_int(mla.n_mil_pol - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" ppr="); print_int(ppr);
    print_str(" ocn="); print_int(ocn);
    print_str(" prp="); print_int(prp);
    print_str(" scw="); print_int(scw); print_str("\n");
    return mla.n_mil_pol - 1;
}

void mil_th_report(void) {
    print_str("[MLA] Military thought report:\n");
    print_str("  Thought categories: "); print_int(mla.n_mil_th); print_str("\n");
    print_str("  Total military principles: "); print_int(mla.total_mil_pri); print_str("\n");
}

void mil_str_report(void) {
    print_str("[MLA] Military strategy report:\n");
    print_str("  Strategy categories: "); print_int(mla.n_mil_str); print_str("\n");
    print_str("  Total strategy theory: "); print_int(mla.total_str_th); print_str("\n");
}

void mla_full_report(void) {
    print_str("[MLA] Full report:\n");
    print_str("  Operations categories: "); print_int(mla.n_mil_camp); print_str("\n");
    print_str("  Total operations theory: "); print_int(mla.total_camp_th); print_str("\n");
    print_str("  Logistics categories: "); print_int(mla.n_mil_log); print_str("\n");
    print_str("  Total logistics support: "); print_int(mla.total_log_sup); print_str("\n");
    print_str("  Political work categories: "); print_int(mla.n_mil_pol); print_str("\n");
    print_str("  Total political principles: "); print_int(mla.total_pol_pri); print_str("\n");
}

void mla_print_state(void) {
    print_str("[MLA] Mt="); print_int(mla.n_mil_th);
    print_str(" Ms="); print_int(mla.n_mil_str);
    print_str(" Mc="); print_int(mla.n_mil_camp);
    print_str(" Lg="); print_int(mla.n_mil_log);
    print_str(" Mp="); print_int(mla.n_mil_pol);
    print_str("\n");
}

int main(void) {
    print_str("=== Military Admin Demo ===\n\n");
    mla_init();

    print_str("Military thought...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int mpr = 55 + (i * 13);
        int sth = 40 + (i * 10);
        int tth = 22 + (i * 5);
        int mph = 15 + (i * 3);
        int year = 2020 + (i % 5);
        mla_mil_th(type, cat, mpr, sth, tth, mph, year);
    }

    print_str("\nMilitary strategy...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int sth = 48 + (i * 11);
        int spl = 35 + (i * 8);
        int sev = 20 + (i * 4);
        int ssu = 12 + (i * 3);
        int year = 2021 + (i % 4);
        mla_mil_str(type, cat, sth, spl, sev, ssu, year);
    }

    print_str("\nMilitary operations...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cth = 42 + (i * 10);
        int ccm = 28 + (i * 7);
        int csu = 18 + (i * 4);
        int cev = 10 + (i * 2);
        int year = 2022 + (i % 3);
        mla_mil_camp(type, cat, cth, ccm, csu, cev, year);
    }

    print_str("\nMilitary logistics...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int lgs = 35 + (i * 8);
        int equ = 25 + (i * 6);
        int sup = 15 + (i * 3);
        int mtr = 10 + (i * 2);
        int year = 2023 + (i % 2);
        mla_mil_log(type, cat, lgs, equ, sup, mtr, year);
    }

    print_str("\nPolitical work...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ppr = 30 + (i * 7);
        int ocn = 22 + (i * 5);
        int prp = 12 + (i * 3);
        int scw = 8 + (i * 2);
        int year = 2024;
        mla_mil_pol(type, cat, ppr, ocn, prp, scw, year);
    }

    print_str("\nThought report...\n");
    mil_th_report();

    print_str("\nStrategy report...\n");
    mil_str_report();

    print_str("\nFull report...\n");
    mla_full_report();

    print_str("\nFinal state...\n");
    mla_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
