/* human_resource_admin: Human resource administration system (v1.0)
 * HR planning, recruitment, training, performance management, compensation
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

#define MAX_HR_PLN     16
#define MAX_REC_CFG    14
#define MAX_TRN_DEV    12
#define MAX_PER_MAN    10
#define MAX_COM_BEN    10

typedef struct {
    int    hp_id;
    int    hp_type;
    int    hp_category;
    int    org_des;
    int    pos_ana;
    int    man_for;
    int    est_man;
    int    year;
    int    active;
} hr_pln_t;

typedef struct {
    int    rc_id;
    int    rc_type;
    int    rc_category;
    int    rec_cha;
    int    sel_met;
    int    per_all;
    int    entry_man;
    int    year;
    int    active;
} rec_cfg_t;

typedef struct {
    int    td_id;
    int    td_type;
    int    td_category;
    int    tra_sys;
    int    car_dev;
    int    abi_eva;
    int    lea_man;
    int    year;
    int    active;
} trn_dev_t;

typedef struct {
    int    pm_id;
    int    pm_type;
    int    pm_category;
    int    per_app;
    int    obj_man;
    int    per_fed;
    int    inc_man;
    int    year;
    int    active;
} per_man_t;

typedef struct {
    int    cb_id;
    int    cb_type;
    int    cb_category;
    int    com_des;
    int    ben_man;
    int    equ_inc;
    int    emp_car;
    int    year;
    int    active;
} com_ben_t;

typedef struct {
    int    n_hr_pln;
    int    n_rec_cfg;
    int    n_trn_dev;
    int    n_per_man;
    int    n_com_ben;
    int    total_org_des;
    int    total_rec_cha;
    int    total_tra_sys;
    int    total_per_app;
    int    total_com_des;
} hra_state_t;

static hr_pln_t hr_plns[MAX_HR_PLN];
static rec_cfg_t rec_cfgs[MAX_REC_CFG];
static trn_dev_t trn_devs[MAX_TRN_DEV];
static per_man_t per_mans[MAX_PER_MAN];
static com_ben_t com_bens[MAX_COM_BEN];
static hra_state_t hra;

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

int hra_init(void) {
    if (initialized) return -1;
    hra.n_hr_pln = 0; hra.n_rec_cfg = 0; hra.n_trn_dev = 0;
    hra.n_per_man = 0; hra.n_com_ben = 0;
    hra.total_org_des = 0; hra.total_rec_cha = 0;
    hra.total_tra_sys = 0; hra.total_per_app = 0;
    hra.total_com_des = 0;
    for (int i = 0; i < MAX_HR_PLN; i++) hr_plns[i].active = 0;
    for (int i = 0; i < MAX_REC_CFG; i++) rec_cfgs[i].active = 0;
    for (int i = 0; i < MAX_TRN_DEV; i++) trn_devs[i].active = 0;
    for (int i = 0; i < MAX_PER_MAN; i++) per_mans[i].active = 0;
    for (int i = 0; i < MAX_COM_BEN; i++) com_bens[i].active = 0;
    initialized = 1;
    print_str("[HRA] Human resource initialized\n");
    return 0;
}

int hra_hr_pln(int ht_type, int cat, int odes, int pana, int mfor, int eman, int year) {
    if (hra.n_hr_pln >= MAX_HR_PLN) return -1;
    hr_pln_t* h = &hr_plns[hra.n_hr_pln];
    h->hp_id = hra.n_hr_pln;
    h->hp_type = ht_type;
    h->hp_category = cat;
    h->org_des = odes;
    h->pos_ana = pana;
    h->man_for = mfor;
    h->est_man = eman;
    h->year = year;
    h->active = 1;
    hra.total_org_des += odes;
    hra.n_hr_pln++;
    print_str("[HRA] Hr pln "); print_int(hra.n_hr_pln - 1);
    print_str(" type="); print_int(ht_type);
    print_str(" cat="); print_int(cat);
    print_str(" odes="); print_int(odes);
    print_str(" pana="); print_int(pana);
    print_str(" mfor="); print_int(mfor);
    print_str(" eman="); print_int(eman); print_str("\n");
    return hra.n_hr_pln - 1;
}

int hra_rec_cfg(int rt_type, int cat, int rcha, int smet, int pall, int eman2, int year) {
    if (hra.n_rec_cfg >= MAX_REC_CFG) return -1;
    rec_cfg_t* r = &rec_cfgs[hra.n_rec_cfg];
    r->rc_id = hra.n_rec_cfg;
    r->rc_type = rt_type;
    r->rc_category = cat;
    r->rec_cha = rcha;
    r->sel_met = smet;
    r->per_all = pall;
    r->entry_man = eman2;
    r->year = year;
    r->active = 1;
    hra.total_rec_cha += rcha;
    hra.n_rec_cfg++;
    print_str("[HRA] Rec cfg "); print_int(hra.n_rec_cfg - 1);
    print_str(" type="); print_int(rt_type);
    print_str(" cat="); print_int(cat);
    print_str(" rcha="); print_int(rcha);
    print_str(" smet="); print_int(smet);
    print_str(" pall="); print_int(pall);
    print_str(" eman2="); print_int(eman2); print_str("\n");
    return hra.n_rec_cfg - 1;
}

int hra_trn_dev(int tt_type, int cat, int tsys, int cdev, int aeva, int lman, int year) {
    if (hra.n_trn_dev >= MAX_TRN_DEV) return -1;
    trn_dev_t* t = &trn_devs[hra.n_trn_dev];
    t->td_id = hra.n_trn_dev;
    t->td_type = tt_type;
    t->td_category = cat;
    t->tra_sys = tsys;
    t->car_dev = cdev;
    t->abi_eva = aeva;
    t->lea_man = lman;
    t->year = year;
    t->active = 1;
    hra.total_tra_sys += tsys;
    hra.n_trn_dev++;
    print_str("[HRA] Trn dev "); print_int(hra.n_trn_dev - 1);
    print_str(" type="); print_int(tt_type);
    print_str(" cat="); print_int(cat);
    print_str(" tsys="); print_int(tsys);
    print_str(" cdev="); print_int(cdev);
    print_str(" aeva="); print_int(aeva);
    print_str(" lman="); print_int(lman); print_str("\n");
    return hra.n_trn_dev - 1;
}

int hra_per_man(int pt_type, int cat, int papp, int oman, int pfed, int iman, int year) {
    if (hra.n_per_man >= MAX_PER_MAN) return -1;
    per_man_t* p = &per_mans[hra.n_per_man];
    p->pm_id = hra.n_per_man;
    p->pm_type = pt_type;
    p->pm_category = cat;
    p->per_app = papp;
    p->obj_man = oman;
    p->per_fed = pfed;
    p->inc_man = iman;
    p->year = year;
    p->active = 1;
    hra.total_per_app += papp;
    hra.n_per_man++;
    print_str("[HRA] Per man "); print_int(hra.n_per_man - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" papp="); print_int(papp);
    print_str(" oman="); print_int(oman);
    print_str(" pfed="); print_int(pfed);
    print_str(" iman="); print_int(iman); print_str("\n");
    return hra.n_per_man - 1;
}

int hra_com_ben(int ct_type, int cat, int cdes, int bman, int einc, int ecar, int year) {
    if (hra.n_com_ben >= MAX_COM_BEN) return -1;
    com_ben_t* c = &com_bens[hra.n_com_ben];
    c->cb_id = hra.n_com_ben;
    c->cb_type = ct_type;
    c->cb_category = cat;
    c->com_des = cdes;
    c->ben_man = bman;
    c->equ_inc = einc;
    c->emp_car = ecar;
    c->year = year;
    c->active = 1;
    hra.total_com_des += cdes;
    hra.n_com_ben++;
    print_str("[HRA] Com ben "); print_int(hra.n_com_ben - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" cdes="); print_int(cdes);
    print_str(" bman="); print_int(bman);
    print_str(" einc="); print_int(einc);
    print_str(" ecar="); print_int(ecar); print_str("\n");
    return hra.n_com_ben - 1;
}

void hra_planning_report(void) {
    print_str("[HRA] HR planning report:\n");
    print_str("  Planning categories: "); print_int(hra.n_hr_pln); print_str("\n");
    print_str("  Total org design: "); print_int(hra.total_org_des); print_str("\n");
}

void hra_recruitment_report(void) {
    print_str("[HRA] Recruitment report:\n");
    print_str("  Recruitment categories: "); print_int(hra.n_rec_cfg); print_str("\n");
    print_str("  Total recruitment channels: "); print_int(hra.total_rec_cha); print_str("\n");
}

void hra_full_report(void) {
    print_str("[HRA] Full report:\n");
    print_str("  Training categories: "); print_int(hra.n_trn_dev); print_str("\n");
    print_str("  Total training systems: "); print_int(hra.total_tra_sys); print_str("\n");
    print_str("  Performance categories: "); print_int(hra.n_per_man); print_str("\n");
    print_str("  Total performance appraisal: "); print_int(hra.total_per_app); print_str("\n");
    print_str("  Compensation categories: "); print_int(hra.n_com_ben); print_str("\n");
    print_str("  Total compensation design: "); print_int(hra.total_com_des); print_str("\n");
}

void hra_print_state(void) {
    print_str("[HRA] Hp="); print_int(hra.n_hr_pln);
    print_str(" Rc="); print_int(hra.n_rec_cfg);
    print_str(" Td="); print_int(hra.n_trn_dev);
    print_str(" Pm="); print_int(hra.n_per_man);
    print_str(" Cb="); print_int(hra.n_com_ben);
    print_str("\n");
}

int main(void) {
    print_str("=== Human Resource Admin Demo ===\n\n");
    hra_init();

    print_str("HR planning...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int odes = 55 + (i * 13);
        int pana = 40 + (i * 10);
        int mfor = 22 + (i * 5);
        int eman = 15 + (i * 3);
        int year = 2020 + (i % 5);
        hra_hr_pln(type, cat, odes, pana, mfor, eman, year);
    }

    print_str("\nRecruitment...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int rcha = 48 + (i * 11);
        int smet = 35 + (i * 8);
        int pall = 20 + (i * 4);
        int eman2 = 12 + (i * 3);
        int year = 2021 + (i % 4);
        hra_rec_cfg(type, cat, rcha, smet, pall, eman2, year);
    }

    print_str("\nTraining...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int tsys = 42 + (i * 10);
        int cdev = 28 + (i * 7);
        int aeva = 18 + (i * 4);
        int lman = 10 + (i * 2);
        int year = 2022 + (i % 3);
        hra_trn_dev(type, cat, tsys, cdev, aeva, lman, year);
    }

    print_str("\nPerformance...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int papp = 35 + (i * 8);
        int oman = 25 + (i * 6);
        int pfed = 15 + (i * 3);
        int iman = 10 + (i * 2);
        int year = 2023 + (i % 2);
        hra_per_man(type, cat, papp, oman, pfed, iman, year);
    }

    print_str("\nCompensation...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cdes = 30 + (i * 7);
        int bman = 22 + (i * 5);
        int einc = 12 + (i * 3);
        int ecar = 8 + (i * 2);
        int year = 2024;
        hra_com_ben(type, cat, cdes, bman, einc, ecar, year);
    }

    print_str("\nPlanning report...\n");
    hra_planning_report();

    print_str("\nRecruitment report...\n");
    hra_recruitment_report();

    print_str("\nFull report...\n");
    hra_full_report();

    print_str("\nFinal state...\n");
    hra_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
