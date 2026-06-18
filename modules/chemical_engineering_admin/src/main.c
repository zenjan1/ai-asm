/* chemical_engineering_admin: Chemical engineering administration system (v1.0)
 * Reaction engineering, separation engineering, thermodynamics, transport processes, process design
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

#define MAX_REACT_EN    16
#define MAX_SEP_EN      14
#define MAX_THERM       12
#define MAX_TRANSP      10
#define MAX_PROC_DES    10

typedef struct {
    int    re_id;
    int    re_type;
    int    re_category;
    int    reactor_ds;
    int    cat_reaction;
    int    poly_reaction;
    int    year;
    int    active;
} react_en_t;

typedef struct {
    int    se_id;
    int    se_type;
    int    se_category;
    int    dist_sep;
    int    extrac_sep;
    int    memb_sep;
    int    year;
    int    active;
} sep_en_t;

typedef struct {
    int    th_id;
    int    th_type;
    int    th_category;
    int    phase_eq;
    int    chem_eq;
    int    energy_bal;
    int    year;
    int    active;
} therm_t;

typedef struct {
    int    tp_id;
    int    tp_type;
    int    tp_category;
    int    mom_trans;
    int    heat_trans;
    int    mass_trans;
    int    year;
    int    active;
} transp_t;

typedef struct {
    int    pd_id;
    int    pd_type;
    int    pd_category;
    int    flow_ds;
    int    equip_sel;
    int    pipe_ds;
    int    year;
    int    active;
} proc_des_t;

typedef struct {
    int    n_react_en;
    int    n_sep_en;
    int    n_therm;
    int    n_transp;
    int    n_proc_des;
    int    total_reactor_ds;
    int    total_dist_sep;
    int    total_phase_eq;
    int    total_mom_trans;
    int    total_flow_ds;
} cea_state_t;

static react_en_t react_ens[MAX_REACT_EN];
static sep_en_t sep_ens[MAX_SEP_EN];
static therm_t therms[MAX_THERM];
static transp_t transps[MAX_TRANSP];
static proc_des_t proc_dess[MAX_PROC_DES];
static cea_state_t cea;

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

int cea_init(void) {
    if (initialized) return -1;
    cea.n_react_en = 0; cea.n_sep_en = 0; cea.n_therm = 0;
    cea.n_transp = 0; cea.n_proc_des = 0;
    cea.total_reactor_ds = 0; cea.total_dist_sep = 0;
    cea.total_phase_eq = 0; cea.total_mom_trans = 0;
    cea.total_flow_ds = 0;
    for (int i = 0; i < MAX_REACT_EN; i++) react_ens[i].active = 0;
    for (int i = 0; i < MAX_SEP_EN; i++) sep_ens[i].active = 0;
    for (int i = 0; i < MAX_THERM; i++) therms[i].active = 0;
    for (int i = 0; i < MAX_TRANSP; i++) transps[i].active = 0;
    for (int i = 0; i < MAX_PROC_DES; i++) proc_dess[i].active = 0;
    initialized = 1;
    print_str("[CEA] Chemical engineering initialized\n");
    return 0;
}

int cea_react_en(int rt_type, int cat, int rds, int ctr, int ptr, int year) {
    if (cea.n_react_en >= MAX_REACT_EN) return -1;
    react_en_t* r = &react_ens[cea.n_react_en];
    r->re_id = cea.n_react_en;
    r->re_type = rt_type;
    r->re_category = cat;
    r->reactor_ds = rds;
    r->cat_reaction = ctr;
    r->poly_reaction = ptr;
    r->year = year;
    r->active = 1;
    cea.total_reactor_ds += rds;
    cea.n_react_en++;
    print_str("[CEA] React en "); print_int(cea.n_react_en - 1);
    print_str(" type="); print_int(rt_type);
    print_str(" cat="); print_int(cat);
    print_str(" rds="); print_int(rds);
    print_str(" ctr="); print_int(ctr);
    print_str(" ptr="); print_int(ptr); print_str("\n");
    return cea.n_react_en - 1;
}

int cea_sep_en(int st_type, int cat, int dst, int ext, int mmb, int year) {
    if (cea.n_sep_en >= MAX_SEP_EN) return -1;
    sep_en_t* s = &sep_ens[cea.n_sep_en];
    s->se_id = cea.n_sep_en;
    s->se_type = st_type;
    s->se_category = cat;
    s->dist_sep = dst;
    s->extrac_sep = ext;
    s->memb_sep = mmb;
    s->year = year;
    s->active = 1;
    cea.total_dist_sep += dst;
    cea.n_sep_en++;
    print_str("[CEA] Sep en "); print_int(cea.n_sep_en - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" dst="); print_int(dst);
    print_str(" ext="); print_int(ext);
    print_str(" mmb="); print_int(mmb); print_str("\n");
    return cea.n_sep_en - 1;
}

int cea_therm(int tt_type, int cat, int phe, int che, int enb, int year) {
    if (cea.n_therm >= MAX_THERM) return -1;
    therm_t* t = &therms[cea.n_therm];
    t->th_id = cea.n_therm;
    t->th_type = tt_type;
    t->th_category = cat;
    t->phase_eq = phe;
    t->chem_eq = che;
    t->energy_bal = enb;
    t->year = year;
    t->active = 1;
    cea.total_phase_eq += phe;
    cea.n_therm++;
    print_str("[CEA] Therm "); print_int(cea.n_therm - 1);
    print_str(" type="); print_int(tt_type);
    print_str(" cat="); print_int(cat);
    print_str(" phe="); print_int(phe);
    print_str(" che="); print_int(che);
    print_str(" enb="); print_int(enb); print_str("\n");
    return cea.n_therm - 1;
}

int cea_transp(int tt_type, int cat, int mmt, int htt, int mst, int year) {
    if (cea.n_transp >= MAX_TRANSP) return -1;
    transp_t* t = &transps[cea.n_transp];
    t->tp_id = cea.n_transp;
    t->tp_type = tt_type;
    t->tp_category = cat;
    t->mom_trans = mmt;
    t->heat_trans = htt;
    t->mass_trans = mst;
    t->year = year;
    t->active = 1;
    cea.total_mom_trans += mmt;
    cea.n_transp++;
    print_str("[CEA] Transp "); print_int(cea.n_transp - 1);
    print_str(" type="); print_int(tt_type);
    print_str(" cat="); print_int(cat);
    print_str(" mmt="); print_int(mmt);
    print_str(" htt="); print_int(htt);
    print_str(" mst="); print_int(mst); print_str("\n");
    return cea.n_transp - 1;
}

int cea_proc_des(int pt_type, int cat, int flw, int eqs, int pip, int year) {
    if (cea.n_proc_des >= MAX_PROC_DES) return -1;
    proc_des_t* p = &proc_dess[cea.n_proc_des];
    p->pd_id = cea.n_proc_des;
    p->pd_type = pt_type;
    p->pd_category = cat;
    p->flow_ds = flw;
    p->equip_sel = eqs;
    p->pipe_ds = pip;
    p->year = year;
    p->active = 1;
    cea.total_flow_ds += flw;
    cea.n_proc_des++;
    print_str("[CEA] Proc des "); print_int(cea.n_proc_des - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" flw="); print_int(flw);
    print_str(" eqs="); print_int(eqs);
    print_str(" pip="); print_int(pip); print_str("\n");
    return cea.n_proc_des - 1;
}

void cea_react_en_report(void) {
    print_str("[CEA] Reaction engineering report:\n");
    print_str("  Reaction engineering categories: "); print_int(cea.n_react_en); print_str("\n");
    print_str("  Total reactor design: "); print_int(cea.total_reactor_ds); print_str("\n");
}

void cea_sep_en_report(void) {
    print_str("[CEA] Separation engineering report:\n");
    print_str("  Separation engineering categories: "); print_int(cea.n_sep_en); print_str("\n");
    print_str("  Total distillation separation: "); print_int(cea.total_dist_sep); print_str("\n");
}

void cea_full_report(void) {
    print_str("[CEA] Full report:\n");
    print_str("  Thermodynamics categories: "); print_int(cea.n_therm); print_str("\n");
    print_str("  Total phase equilibrium: "); print_int(cea.total_phase_eq); print_str("\n");
    print_str("  Transport processes categories: "); print_int(cea.n_transp); print_str("\n");
    print_str("  Total momentum transfer: "); print_int(cea.total_mom_trans); print_str("\n");
    print_str("  Process design categories: "); print_int(cea.n_proc_des); print_str("\n");
    print_str("  Total flow design: "); print_int(cea.total_flow_ds); print_str("\n");
}

void cea_print_state(void) {
    print_str("[CEA] Re="); print_int(cea.n_react_en);
    print_str(" Se="); print_int(cea.n_sep_en);
    print_str(" Th="); print_int(cea.n_therm);
    print_str(" Tr="); print_int(cea.n_transp);
    print_str(" Pd="); print_int(cea.n_proc_des);
    print_str("\n");
}

int main(void) {
    print_str("=== Chemical Engineering Admin Demo ===\n\n");
    cea_init();

    print_str("Reaction engineering...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int rds = 55 + (i * 13);
        int ctr = 40 + (i * 10);
        int ptr = 22 + (i * 5);
        int year = 2020 + (i % 5);
        cea_react_en(type, cat, rds, ctr, ptr, year);
    }

    print_str("\nSeparation engineering...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int dst = 48 + (i * 11);
        int ext = 35 + (i * 8);
        int mmb = 20 + (i * 4);
        int year = 2021 + (i % 4);
        cea_sep_en(type, cat, dst, ext, mmb, year);
    }

    print_str("\nThermodynamics...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int phe = 42 + (i * 10);
        int che = 28 + (i * 7);
        int enb = 18 + (i * 4);
        int year = 2022 + (i % 3);
        cea_therm(type, cat, phe, che, enb, year);
    }

    print_str("\nTransport processes...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int mmt = 35 + (i * 8);
        int htt = 25 + (i * 6);
        int mst = 15 + (i * 3);
        int year = 2023 + (i % 2);
        cea_transp(type, cat, mmt, htt, mst, year);
    }

    print_str("\nProcess design...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int flw = 30 + (i * 7);
        int eqs = 22 + (i * 5);
        int pip = 12 + (i * 3);
        int year = 2024;
        cea_proc_des(type, cat, flw, eqs, pip, year);
    }

    print_str("\nReaction engineering report...\n");
    cea_react_en_report();

    print_str("\nSeparation engineering report...\n");
    cea_sep_en_report();

    print_str("\nFull report...\n");
    cea_full_report();

    print_str("\nFinal state...\n");
    cea_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
