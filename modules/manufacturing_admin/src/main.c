/* manufacturing_admin: Manufacturing engineering administration (v1.0)
 * Manufacturing processes, CNC, industrial robots, FMS, smart manufacturing
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

#define MAX_PROCESS     16
#define MAX_CNC         14
#define MAX_ROBOT       12
#define MAX_FMS         10
#define MAX_SMART_MF    10

typedef struct {
    int    pr_id;
    int    pr_type;
    int    pr_category;
    int    casting;
    int    forging;
    int    welding_pr;
    int    year;
    int    active;
} process_t;

typedef struct {
    int    cn_id;
    int    cn_type;
    int    cn_category;
    int    cnc_lathe;
    int    cnc_mill;
    int    machining_ctr;
    int    year;
    int    active;
} cnc_t;

typedef struct {
    int    rb_id;
    int    rb_type;
    int    rb_category;
    int    cartesian;
    int    articulated;
    int    scara;
    int    year;
    int    active;
} robot_t;

typedef struct {
    int    fm_id;
    int    fm_type;
    int    fm_category;
    int    fms_sys;
    int    agv;
    int    auto_wh;
    int    year;
    int    active;
} fms_t;

typedef struct {
    int    sm_id;
    int    sm_type;
    int    sm_category;
    int    digital_twin;
    int    industrial_iot;
    int    smart_factory;
    int    year;
    int    active;
} smart_mf_t;

typedef struct {
    int    n_process;
    int    n_cnc;
    int    n_robot;
    int    n_fms;
    int    n_smart_mf;
    int    total_casting;
    int    total_cnc_lathe;
    int    total_cartesian;
    int    total_fms_sys;
    int    total_digital_twin;
} mfg_state_t;

static process_t processes[MAX_PROCESS];
static cnc_t cncs[MAX_CNC];
static robot_t robots[MAX_ROBOT];
static fms_t fmss[MAX_FMS];
static smart_mf_t smart_mfs[MAX_SMART_MF];
static mfg_state_t mfg;

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

int mfg_init(void) {
    if (initialized) return -1;
    mfg.n_process = 0; mfg.n_cnc = 0; mfg.n_robot = 0;
    mfg.n_fms = 0; mfg.n_smart_mf = 0;
    mfg.total_casting = 0; mfg.total_cnc_lathe = 0;
    mfg.total_cartesian = 0; mfg.total_fms_sys = 0;
    mfg.total_digital_twin = 0;
    for (int i = 0; i < MAX_PROCESS; i++) processes[i].active = 0;
    for (int i = 0; i < MAX_CNC; i++) cncs[i].active = 0;
    for (int i = 0; i < MAX_ROBOT; i++) robots[i].active = 0;
    for (int i = 0; i < MAX_FMS; i++) fmss[i].active = 0;
    for (int i = 0; i < MAX_SMART_MF; i++) smart_mfs[i].active = 0;
    initialized = 1;
    print_str("[MFG] Manufacturing initialized\n");
    return 0;
}

int mfg_process(int pt_type, int cat, int cst, int frg, int wld, int year) {
    if (mfg.n_process >= MAX_PROCESS) return -1;
    process_t* p = &processes[mfg.n_process];
    p->pr_id = mfg.n_process;
    p->pr_type = pt_type;
    p->pr_category = cat;
    p->casting = cst;
    p->forging = frg;
    p->welding_pr = wld;
    p->year = year;
    p->active = 1;
    mfg.total_casting += cst;
    mfg.n_process++;
    print_str("[MFG] Process "); print_int(mfg.n_process - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" cst="); print_int(cst);
    print_str(" frg="); print_int(frg);
    print_str(" wld="); print_int(wld); print_str("\n");
    return mfg.n_process - 1;
}

int mfg_cnc(int ct_type, int cat, int lat, int mil, int mch, int year) {
    if (mfg.n_cnc >= MAX_CNC) return -1;
    cnc_t* c = &cncs[mfg.n_cnc];
    c->cn_id = mfg.n_cnc;
    c->cn_type = ct_type;
    c->cn_category = cat;
    c->cnc_lathe = lat;
    c->cnc_mill = mil;
    c->machining_ctr = mch;
    c->year = year;
    c->active = 1;
    mfg.total_cnc_lathe += lat;
    mfg.n_cnc++;
    print_str("[MFG] CNC "); print_int(mfg.n_cnc - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" lat="); print_int(lat);
    print_str(" mil="); print_int(mil);
    print_str(" mch="); print_int(mch); print_str("\n");
    return mfg.n_cnc - 1;
}

int mfg_robot(int rt_type, int cat, int crt, int art, int sca, int year) {
    if (mfg.n_robot >= MAX_ROBOT) return -1;
    robot_t* r = &robots[mfg.n_robot];
    r->rb_id = mfg.n_robot;
    r->rb_type = rt_type;
    r->rb_category = cat;
    r->cartesian = crt;
    r->articulated = art;
    r->scara = sca;
    r->year = year;
    r->active = 1;
    mfg.total_cartesian += crt;
    mfg.n_robot++;
    print_str("[MFG] Robot "); print_int(mfg.n_robot - 1);
    print_str(" type="); print_int(rt_type);
    print_str(" cat="); print_int(cat);
    print_str(" crt="); print_int(crt);
    print_str(" art="); print_int(art);
    print_str(" sca="); print_int(sca); print_str("\n");
    return mfg.n_robot - 1;
}

int mfg_fms(int ft_type, int cat, int fms_s, int agv_v, int aw, int year) {
    if (mfg.n_fms >= MAX_FMS) return -1;
    fms_t* f = &fmss[mfg.n_fms];
    f->fm_id = mfg.n_fms;
    f->fm_type = ft_type;
    f->fm_category = cat;
    f->fms_sys = fms_s;
    f->agv = agv_v;
    f->auto_wh = aw;
    f->year = year;
    f->active = 1;
    mfg.total_fms_sys += fms_s;
    mfg.n_fms++;
    print_str("[MFG] FMS "); print_int(mfg.n_fms - 1);
    print_str(" type="); print_int(ft_type);
    print_str(" cat="); print_int(cat);
    print_str(" fms="); print_int(fms_s);
    print_str(" agv="); print_int(agv_v);
    print_str(" aw="); print_int(aw); print_str("\n");
    return mfg.n_fms - 1;
}

int mfg_smart_mf(int st_type, int cat, int dgt, int iot, int sf, int year) {
    if (mfg.n_smart_mf >= MAX_SMART_MF) return -1;
    smart_mf_t* s = &smart_mfs[mfg.n_smart_mf];
    s->sm_id = mfg.n_smart_mf;
    s->sm_type = st_type;
    s->sm_category = cat;
    s->digital_twin = dgt;
    s->industrial_iot = iot;
    s->smart_factory = sf;
    s->year = year;
    s->active = 1;
    mfg.total_digital_twin += dgt;
    mfg.n_smart_mf++;
    print_str("[MFG] Smart mfg "); print_int(mfg.n_smart_mf - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" dgt="); print_int(dgt);
    print_str(" iot="); print_int(iot);
    print_str(" sf="); print_int(sf); print_str("\n");
    return mfg.n_smart_mf - 1;
}

void mfg_process_report(void) {
    print_str("[MFG] Process report:\n");
    print_str("  Manufacturing processes: "); print_int(mfg.n_process); print_str("\n");
    print_str("  Total casting: "); print_int(mfg.total_casting); print_str("\n");
}

void mfg_cnc_report(void) {
    print_str("[MFG] CNC report:\n");
    print_str("  CNC categories: "); print_int(mfg.n_cnc); print_str("\n");
    print_str("  Total CNC lathe: "); print_int(mfg.total_cnc_lathe); print_str("\n");
}

void mfg_full_report(void) {
    print_str("[MFG] Full report:\n");
    print_str("  Industrial robot categories: "); print_int(mfg.n_robot); print_str("\n");
    print_str("  Total cartesian: "); print_int(mfg.total_cartesian); print_str("\n");
    print_str("  FMS categories: "); print_int(mfg.n_fms); print_str("\n");
    print_str("  Total FMS systems: "); print_int(mfg.total_fms_sys); print_str("\n");
    print_str("  Smart manufacturing categories: "); print_int(mfg.n_smart_mf); print_str("\n");
    print_str("  Total digital twin: "); print_int(mfg.total_digital_twin); print_str("\n");
}

void mfg_print_state(void) {
    print_str("[MFG] Pr="); print_int(mfg.n_process);
    print_str(" Cn="); print_int(mfg.n_cnc);
    print_str(" Rb="); print_int(mfg.n_robot);
    print_str(" Fm="); print_int(mfg.n_fms);
    print_str(" Sm="); print_int(mfg.n_smart_mf);
    print_str("\n");
}

int main(void) {
    print_str("=== Manufacturing Admin Demo ===\n\n");
    mfg_init();

    print_str("Manufacturing processes...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int cst = 55 + (i * 13);
        int frg = 40 + (i * 10);
        int wld = 22 + (i * 5);
        int year = 2020 + (i % 5);
        mfg_process(type, cat, cst, frg, wld, year);
    }

    print_str("\nCNC...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int lat = 48 + (i * 11);
        int mil = 35 + (i * 8);
        int mch = 20 + (i * 4);
        int year = 2021 + (i % 4);
        mfg_cnc(type, cat, lat, mil, mch, year);
    }

    print_str("\nIndustrial robots...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int crt = 42 + (i * 10);
        int art = 28 + (i * 7);
        int sca = 18 + (i * 4);
        int year = 2022 + (i % 3);
        mfg_robot(type, cat, crt, art, sca, year);
    }

    print_str("\nFMS...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int fms_s = 35 + (i * 8);
        int agv_v = 25 + (i * 6);
        int aw = 15 + (i * 3);
        int year = 2023 + (i % 2);
        mfg_fms(type, cat, fms_s, agv_v, aw, year);
    }

    print_str("\nSmart manufacturing...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int dgt = 30 + (i * 7);
        int iot = 22 + (i * 5);
        int sf = 12 + (i * 3);
        int year = 2024;
        mfg_smart_mf(type, cat, dgt, iot, sf, year);
    }

    print_str("\nProcess report...\n");
    mfg_process_report();

    print_str("\nCNC report...\n");
    mfg_cnc_report();

    print_str("\nFull report...\n");
    mfg_full_report();

    print_str("\nFinal state...\n");
    mfg_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
