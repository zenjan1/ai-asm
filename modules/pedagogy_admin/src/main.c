/* pedagogy_admin: Pedagogy administration system (v1.0)
 * Pedagogy principles, curriculum, preschool, higher education, edu tech
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

#define MAX_PED_PRI     16
#define MAX_CUR_INS     14
#define MAX_PRE_SCH     12
#define MAX_HIGH_ED     10
#define MAX_EDU_TEC     10

typedef struct {
    int    pp_id;
    int    pp_type;
    int    pp_category;
    int    ped_phi;
    int    ped_soc;
    int    ped_psy;
    int    ped_his;
    int    year;
    int    active;
} ped_pri_t;

typedef struct {
    int    ci_id;
    int    ci_type;
    int    ci_category;
    int    cur_th;
    int    ins_des;
    int    tea_eva;
    int    tea_tec;
    int    year;
    int    active;
} cur_ins_t;

typedef struct {
    int    ps_id;
    int    ps_type;
    int    ps_category;
    int    child_psy;
    int    child_edu;
    int    kin_mgmt;
    int    play_th;
    int    year;
    int    active;
} pre_sch_t;

typedef struct {
    int    he_id;
    int    he_type;
    int    he_category;
    int    uni_mgmt;
    int    sub_con;
    int    grad_edu;
    int    acad_eva;
    int    year;
    int    active;
} high_ed_t;

typedef struct {
    int    et_id;
    int    et_type;
    int    et_category;
    int    dis_edu;
    int    multi_edu;
    int    online;
    int    smart_edu;
    int    year;
    int    active;
} edu_tec_t;

typedef struct {
    int    n_ped_pri;
    int    n_cur_ins;
    int    n_pre_sch;
    int    n_high_ed;
    int    n_edu_tec;
    int    total_ped_phi;
    int    total_cur_th;
    int    total_child_psy;
    int    total_uni_mgmt;
    int    total_dis_edu;
} pda_state_t;

static ped_pri_t ped_pris[MAX_PED_PRI];
static cur_ins_t cur_inss[MAX_CUR_INS];
static pre_sch_t pre_schs[MAX_PRE_SCH];
static high_ed_t high_eds[MAX_HIGH_ED];
static edu_tec_t edu_tecs[MAX_EDU_TEC];
static pda_state_t pda;

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

int pda_init(void) {
    if (initialized) return -1;
    pda.n_ped_pri = 0; pda.n_cur_ins = 0; pda.n_pre_sch = 0;
    pda.n_high_ed = 0; pda.n_edu_tec = 0;
    pda.total_ped_phi = 0; pda.total_cur_th = 0;
    pda.total_child_psy = 0; pda.total_uni_mgmt = 0;
    pda.total_dis_edu = 0;
    for (int i = 0; i < MAX_PED_PRI; i++) ped_pris[i].active = 0;
    for (int i = 0; i < MAX_CUR_INS; i++) cur_inss[i].active = 0;
    for (int i = 0; i < MAX_PRE_SCH; i++) pre_schs[i].active = 0;
    for (int i = 0; i < MAX_HIGH_ED; i++) high_eds[i].active = 0;
    for (int i = 0; i < MAX_EDU_TEC; i++) edu_tecs[i].active = 0;
    initialized = 1;
    print_str("[PDA] Pedagogy initialized\n");
    return 0;
}

int pda_ped_pri(int pt_type, int cat, int pph, int psc, int pps, int phs, int year) {
    if (pda.n_ped_pri >= MAX_PED_PRI) return -1;
    ped_pri_t* p = &ped_pris[pda.n_ped_pri];
    p->pp_id = pda.n_ped_pri;
    p->pp_type = pt_type;
    p->pp_category = cat;
    p->ped_phi = pph;
    p->ped_soc = psc;
    p->ped_psy = pps;
    p->ped_his = phs;
    p->year = year;
    p->active = 1;
    pda.total_ped_phi += pph;
    pda.n_ped_pri++;
    print_str("[PDA] Ped pri "); print_int(pda.n_ped_pri - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" pph="); print_int(pph);
    print_str(" psc="); print_int(psc);
    print_str(" pps="); print_int(pps);
    print_str(" phs="); print_int(phs); print_str("\n");
    return pda.n_ped_pri - 1;
}

int pda_cur_ins(int ct_type, int cat, int cth, int ids, int tev, int ttc, int year) {
    if (pda.n_cur_ins >= MAX_CUR_INS) return -1;
    cur_ins_t* c = &cur_inss[pda.n_cur_ins];
    c->ci_id = pda.n_cur_ins;
    c->ci_type = ct_type;
    c->ci_category = cat;
    c->cur_th = cth;
    c->ins_des = ids;
    c->tea_eva = tev;
    c->tea_tec = ttc;
    c->year = year;
    c->active = 1;
    pda.total_cur_th += cth;
    pda.n_cur_ins++;
    print_str("[PDA] Cur ins "); print_int(pda.n_cur_ins - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" cth="); print_int(cth);
    print_str(" ids="); print_int(ids);
    print_str(" tev="); print_int(tev);
    print_str(" ttc="); print_int(ttc); print_str("\n");
    return pda.n_cur_ins - 1;
}

int pda_pre_sch(int pt_type, int cat, int cps, int ced, int kmg, int pth, int year) {
    if (pda.n_pre_sch >= MAX_PRE_SCH) return -1;
    pre_sch_t* p = &pre_schs[pda.n_pre_sch];
    p->ps_id = pda.n_pre_sch;
    p->ps_type = pt_type;
    p->ps_category = cat;
    p->child_psy = cps;
    p->child_edu = ced;
    p->kin_mgmt = kmg;
    p->play_th = pth;
    p->year = year;
    p->active = 1;
    pda.total_child_psy += cps;
    pda.n_pre_sch++;
    print_str("[PDA] Pre sch "); print_int(pda.n_pre_sch - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" cps="); print_int(cps);
    print_str(" ced="); print_int(ced);
    print_str(" kmg="); print_int(kmg);
    print_str(" pth="); print_int(pth); print_str("\n");
    return pda.n_pre_sch - 1;
}

int pda_high_ed(int ht_type, int cat, int umg, int sbc, int ged, int ace, int year) {
    if (pda.n_high_ed >= MAX_HIGH_ED) return -1;
    high_ed_t* h = &high_eds[pda.n_high_ed];
    h->he_id = pda.n_high_ed;
    h->he_type = ht_type;
    h->he_category = cat;
    h->uni_mgmt = umg;
    h->sub_con = sbc;
    h->grad_edu = ged;
    h->acad_eva = ace;
    h->year = year;
    h->active = 1;
    pda.total_uni_mgmt += umg;
    pda.n_high_ed++;
    print_str("[PDA] High ed "); print_int(pda.n_high_ed - 1);
    print_str(" type="); print_int(ht_type);
    print_str(" cat="); print_int(cat);
    print_str(" umg="); print_int(umg);
    print_str(" sbc="); print_int(sbc);
    print_str(" ged="); print_int(ged);
    print_str(" ace="); print_int(ace); print_str("\n");
    return pda.n_high_ed - 1;
}

int pda_edu_tec(int et_type, int cat, int ded, int mld, int ond, int sed, int year) {
    if (pda.n_edu_tec >= MAX_EDU_TEC) return -1;
    edu_tec_t* t = &edu_tecs[pda.n_edu_tec];
    t->et_id = pda.n_edu_tec;
    t->et_type = et_type;
    t->et_category = cat;
    t->dis_edu = ded;
    t->multi_edu = mld;
    t->online = ond;
    t->smart_edu = sed;
    t->year = year;
    t->active = 1;
    pda.total_dis_edu += ded;
    pda.n_edu_tec++;
    print_str("[PDA] Edu tec "); print_int(pda.n_edu_tec - 1);
    print_str(" type="); print_int(et_type);
    print_str(" cat="); print_int(cat);
    print_str(" ded="); print_int(ded);
    print_str(" mld="); print_int(mld);
    print_str(" ond="); print_int(ond);
    print_str(" sed="); print_int(sed); print_str("\n");
    return pda.n_edu_tec - 1;
}

void pda_principle_report(void) {
    print_str("[PDA] Pedagogy principles report:\n");
    print_str("  Principle categories: "); print_int(pda.n_ped_pri); print_str("\n");
    print_str("  Total pedagogy philosophy: "); print_int(pda.total_ped_phi); print_str("\n");
}

void pda_curriculum_report(void) {
    print_str("[PDA] Curriculum & instruction report:\n");
    print_str("  Curriculum categories: "); print_int(pda.n_cur_ins); print_str("\n");
    print_str("  Total curriculum theory: "); print_int(pda.total_cur_th); print_str("\n");
}

void pda_full_report(void) {
    print_str("[PDA] Full report:\n");
    print_str("  Preschool categories: "); print_int(pda.n_pre_sch); print_str("\n");
    print_str("  Total child psychology: "); print_int(pda.total_child_psy); print_str("\n");
    print_str("  Higher education categories: "); print_int(pda.n_high_ed); print_str("\n");
    print_str("  Total university management: "); print_int(pda.total_uni_mgmt); print_str("\n");
    print_str("  Edu tech categories: "); print_int(pda.n_edu_tec); print_str("\n");
    print_str("  Total distance education: "); print_int(pda.total_dis_edu); print_str("\n");
}

void pda_print_state(void) {
    print_str("[PDA] Pp="); print_int(pda.n_ped_pri);
    print_str(" Ci="); print_int(pda.n_cur_ins);
    print_str(" Ps="); print_int(pda.n_pre_sch);
    print_str(" He="); print_int(pda.n_high_ed);
    print_str(" Et="); print_int(pda.n_edu_tec);
    print_str("\n");
}

int main(void) {
    print_str("=== Pedagogy Admin Demo ===\n\n");
    pda_init();

    print_str("Pedagogy principles...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int pph = 55 + (i * 13);
        int psc = 40 + (i * 10);
        int pps = 22 + (i * 5);
        int phs = 15 + (i * 3);
        int year = 2020 + (i % 5);
        pda_ped_pri(type, cat, pph, psc, pps, phs, year);
    }

    print_str("\nCurriculum & instruction...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cth = 48 + (i * 11);
        int ids = 35 + (i * 8);
        int tev = 20 + (i * 4);
        int ttc = 12 + (i * 3);
        int year = 2021 + (i % 4);
        pda_cur_ins(type, cat, cth, ids, tev, ttc, year);
    }

    print_str("\nPreschool education...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cps = 42 + (i * 10);
        int ced = 28 + (i * 7);
        int kmg = 18 + (i * 4);
        int pth = 10 + (i * 2);
        int year = 2022 + (i % 3);
        pda_pre_sch(type, cat, cps, ced, kmg, pth, year);
    }

    print_str("\nHigher education...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int umg = 35 + (i * 8);
        int sbc = 25 + (i * 6);
        int ged = 15 + (i * 3);
        int ace = 10 + (i * 2);
        int year = 2023 + (i % 2);
        pda_high_ed(type, cat, umg, sbc, ged, ace, year);
    }

    print_str("\nEducation technology...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ded = 30 + (i * 7);
        int mld = 22 + (i * 5);
        int ond = 12 + (i * 3);
        int sed = 8 + (i * 2);
        int year = 2024;
        pda_edu_tec(type, cat, ded, mld, ond, sed, year);
    }

    print_str("\nPrinciple report...\n");
    pda_principle_report();

    print_str("\nCurriculum report...\n");
    pda_curriculum_report();

    print_str("\nFull report...\n");
    pda_full_report();

    print_str("\nFinal state...\n");
    pda_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
