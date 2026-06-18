/* chemical_admin: Chemical engineering administration system (v1.0)
 * Chemical engineering, chemical process, bioengineering, materials, environmental
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

#define MAX_CHM_ENG     16
#define MAX_CHM_PRO     14
#define MAX_BIO_ENG     12
#define MAX_MAT_ENG     10
#define MAX_ENV_ENG     10

typedef struct {
    int    ce_id;
    int    ce_type;
    int    ce_category;
    int    chm_pri;
    int    chm_the;
    int    rea_eng;
    int    sep_eng;
    int    year;
    int    active;
} chm_eng_t;

typedef struct {
    int    cp_id;
    int    cp_type;
    int    cp_category;
    int    bas_org;
    int    poly_chem;
    int    fin_chem;
    int    ele_chem;
    int    year;
    int    active;
} chm_pro_t;

typedef struct {
    int    be_id;
    int    be_type;
    int    be_category;
    int    bio_chem;
    int    fer_eng;
    int    enz_eng;
    int    cel_eng;
    int    year;
    int    active;
} bio_eng_t;

typedef struct {
    int    mt_id;
    int    mt_type;
    int    mt_category;
    int    met_mat;
    int    ino_mat;
    int    poly_mat;
    int    com_mat;
    int    year;
    int    active;
} mat_eng_t;

typedef struct {
    int    ev_id;
    int    ev_type;
    int    ev_category;
    int    wat_con;
    int    air_con;
    int    sol_dis;
    int    env_mon;
    int    year;
    int    active;
} env_eng_t;

typedef struct {
    int    n_chm_eng;
    int    n_chm_pro;
    int    n_bio_eng;
    int    n_mat_eng;
    int    n_env_eng;
    int    total_chm_pri;
    int    total_bas_org;
    int    total_bio_chem;
    int    total_met_mat;
    int    total_wat_con;
} cha_state_t;

static chm_eng_t chm_engs[MAX_CHM_ENG];
static chm_pro_t chm_pros[MAX_CHM_PRO];
static bio_eng_t bio_engs[MAX_BIO_ENG];
static mat_eng_t mat_engs[MAX_MAT_ENG];
static env_eng_t env_engs[MAX_ENV_ENG];
static cha_state_t cha;

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

int cha_init(void) {
    if (initialized) return -1;
    cha.n_chm_eng = 0; cha.n_chm_pro = 0; cha.n_bio_eng = 0;
    cha.n_mat_eng = 0; cha.n_env_eng = 0;
    cha.total_chm_pri = 0; cha.total_bas_org = 0;
    cha.total_bio_chem = 0; cha.total_met_mat = 0;
    cha.total_wat_con = 0;
    for (int i = 0; i < MAX_CHM_ENG; i++) chm_engs[i].active = 0;
    for (int i = 0; i < MAX_CHM_PRO; i++) chm_pros[i].active = 0;
    for (int i = 0; i < MAX_BIO_ENG; i++) bio_engs[i].active = 0;
    for (int i = 0; i < MAX_MAT_ENG; i++) mat_engs[i].active = 0;
    for (int i = 0; i < MAX_ENV_ENG; i++) env_engs[i].active = 0;
    initialized = 1;
    print_str("[CHA] Chemical initialized\n");
    return 0;
}

int cha_chm_eng(int ct_type, int cat, int cpr, int cte, int ren, int sen, int year) {
    if (cha.n_chm_eng >= MAX_CHM_ENG) return -1;
    chm_eng_t* c = &chm_engs[cha.n_chm_eng];
    c->ce_id = cha.n_chm_eng;
    c->ce_type = ct_type;
    c->ce_category = cat;
    c->chm_pri = cpr;
    c->chm_the = cte;
    c->rea_eng = ren;
    c->sep_eng = sen;
    c->year = year;
    c->active = 1;
    cha.total_chm_pri += cpr;
    cha.n_chm_eng++;
    print_str("[CHA] Chm eng "); print_int(cha.n_chm_eng - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" cpr="); print_int(cpr);
    print_str(" cte="); print_int(cte);
    print_str(" ren="); print_int(ren);
    print_str(" sen="); print_int(sen); print_str("\n");
    return cha.n_chm_eng - 1;
}

int cha_chm_pro(int pt_type, int cat, int bor, int pol, int fnc, int elc, int year) {
    if (cha.n_chm_pro >= MAX_CHM_PRO) return -1;
    chm_pro_t* p = &chm_pros[cha.n_chm_pro];
    p->cp_id = cha.n_chm_pro;
    p->cp_type = pt_type;
    p->cp_category = cat;
    p->bas_org = bor;
    p->poly_chem = pol;
    p->fin_chem = fnc;
    p->ele_chem = elc;
    p->year = year;
    p->active = 1;
    cha.total_bas_org += bor;
    cha.n_chm_pro++;
    print_str("[CHA] Chm pro "); print_int(cha.n_chm_pro - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" bor="); print_int(bor);
    print_str(" pol="); print_int(pol);
    print_str(" fnc="); print_int(fnc);
    print_str(" elc="); print_int(elc); print_str("\n");
    return cha.n_chm_pro - 1;
}

int cha_bio_eng(int bt_type, int cat, int bce, int fen, int ezn, int cel, int year) {
    if (cha.n_bio_eng >= MAX_BIO_ENG) return -1;
    bio_eng_t* b = &bio_engs[cha.n_bio_eng];
    b->be_id = cha.n_bio_eng;
    b->be_type = bt_type;
    b->be_category = cat;
    b->bio_chem = bce;
    b->fer_eng = fen;
    b->enz_eng = ezn;
    b->cel_eng = cel;
    b->year = year;
    b->active = 1;
    cha.total_bio_chem += bce;
    cha.n_bio_eng++;
    print_str("[CHA] Bio eng "); print_int(cha.n_bio_eng - 1);
    print_str(" type="); print_int(bt_type);
    print_str(" cat="); print_int(cat);
    print_str(" bce="); print_int(bce);
    print_str(" fen="); print_int(fen);
    print_str(" ezn="); print_int(ezn);
    print_str(" cel="); print_int(cel); print_str("\n");
    return cha.n_bio_eng - 1;
}

int cha_mat_eng(int mt_type, int cat, int mtl, int inm, int ply, int cmt, int year) {
    if (cha.n_mat_eng >= MAX_MAT_ENG) return -1;
    mat_eng_t* m = &mat_engs[cha.n_mat_eng];
    m->mt_id = cha.n_mat_eng;
    m->mt_type = mt_type;
    m->mt_category = cat;
    m->met_mat = mtl;
    m->ino_mat = inm;
    m->poly_mat = ply;
    m->com_mat = cmt;
    m->year = year;
    m->active = 1;
    cha.total_met_mat += mtl;
    cha.n_mat_eng++;
    print_str("[CHA] Mat eng "); print_int(cha.n_mat_eng - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" mtl="); print_int(mtl);
    print_str(" inm="); print_int(inm);
    print_str(" ply="); print_int(ply);
    print_str(" cmt="); print_int(cmt); print_str("\n");
    return cha.n_mat_eng - 1;
}

int cha_env_eng(int et_type, int cat, int wtc, int arc, int sld, int evm, int year) {
    if (cha.n_env_eng >= MAX_ENV_ENG) return -1;
    env_eng_t* e = &env_engs[cha.n_env_eng];
    e->ev_id = cha.n_env_eng;
    e->ev_type = et_type;
    e->ev_category = cat;
    e->wat_con = wtc;
    e->air_con = arc;
    e->sol_dis = sld;
    e->env_mon = evm;
    e->year = year;
    e->active = 1;
    cha.total_wat_con += wtc;
    cha.n_env_eng++;
    print_str("[CHA] Env eng "); print_int(cha.n_env_eng - 1);
    print_str(" type="); print_int(et_type);
    print_str(" cat="); print_int(cat);
    print_str(" wtc="); print_int(wtc);
    print_str(" arc="); print_int(arc);
    print_str(" sld="); print_int(sld);
    print_str(" evm="); print_int(evm); print_str("\n");
    return cha.n_env_eng - 1;
}

void cha_engineering_report(void) {
    print_str("[CHA] Chemical engineering report:\n");
    print_str("  Engineering categories: "); print_int(cha.n_chm_eng); print_str("\n");
    print_str("  Total chemical principles: "); print_int(cha.total_chm_pri); print_str("\n");
}

void cha_process_report(void) {
    print_str("[CHA] Chemical process report:\n");
    print_str("  Process categories: "); print_int(cha.n_chm_pro); print_str("\n");
    print_str("  Total basic organic: "); print_int(cha.total_bas_org); print_str("\n");
}

void cha_full_report(void) {
    print_str("[CHA] Full report:\n");
    print_str("  Bio categories: "); print_int(cha.n_bio_eng); print_str("\n");
    print_str("  Total biochemistry: "); print_int(cha.total_bio_chem); print_str("\n");
    print_str("  Materials categories: "); print_int(cha.n_mat_eng); print_str("\n");
    print_str("  Total metal materials: "); print_int(cha.total_met_mat); print_str("\n");
    print_str("  Environmental categories: "); print_int(cha.n_env_eng); print_str("\n");
    print_str("  Total water control: "); print_int(cha.total_wat_con); print_str("\n");
}

void cha_print_state(void) {
    print_str("[CHA] Ce="); print_int(cha.n_chm_eng);
    print_str(" Cp="); print_int(cha.n_chm_pro);
    print_str(" Be="); print_int(cha.n_bio_eng);
    print_str(" Mt="); print_int(cha.n_mat_eng);
    print_str(" Ev="); print_int(cha.n_env_eng);
    print_str("\n");
}

int main(void) {
    print_str("=== Chemical Admin Demo ===\n\n");
    cha_init();

    print_str("Chemical engineering...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int cpr = 55 + (i * 13);
        int cte = 40 + (i * 10);
        int ren = 22 + (i * 5);
        int sen = 15 + (i * 3);
        int year = 2020 + (i % 5);
        cha_chm_eng(type, cat, cpr, cte, ren, sen, year);
    }

    print_str("\nChemical process...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int bor = 48 + (i * 11);
        int pol = 35 + (i * 8);
        int fnc = 20 + (i * 4);
        int elc = 12 + (i * 3);
        int year = 2021 + (i % 4);
        cha_chm_pro(type, cat, bor, pol, fnc, elc, year);
    }

    print_str("\nBioengineering...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int bce = 42 + (i * 10);
        int fen = 28 + (i * 7);
        int ezn = 18 + (i * 4);
        int cel = 10 + (i * 2);
        int year = 2022 + (i % 3);
        cha_bio_eng(type, cat, bce, fen, ezn, cel, year);
    }

    print_str("\nMaterials...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int mtl = 35 + (i * 8);
        int inm = 25 + (i * 6);
        int ply = 15 + (i * 3);
        int cmt = 10 + (i * 2);
        int year = 2023 + (i % 2);
        cha_mat_eng(type, cat, mtl, inm, ply, cmt, year);
    }

    print_str("\nEnvironmental...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int wtc = 30 + (i * 7);
        int arc = 22 + (i * 5);
        int sld = 12 + (i * 3);
        int evm = 8 + (i * 2);
        int year = 2024;
        cha_env_eng(type, cat, wtc, arc, sld, evm, year);
    }

    print_str("\nEngineering report...\n");
    cha_engineering_report();

    print_str("\nProcess report...\n");
    cha_process_report();

    print_str("\nFull report...\n");
    cha_full_report();

    print_str("\nFinal state...\n");
    cha_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
