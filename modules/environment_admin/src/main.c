/* environment_admin: Environment administration system (v1.0)
 * Environmental science, environmental engineering, ecology, management, protection
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

#define MAX_ENV_SCI     16
#define MAX_ENV_ENG     14
#define MAX_ECO_SCI     12
#define MAX_ENV_MAN     10
#define MAX_ENV_PRO     10

typedef struct {
    int    es_id;
    int    es_type;
    int    es_category;
    int    env_che;
    int    env_bio;
    int    env_phy;
    int    env_geo;
    int    year;
    int    active;
} env_sci_t;

typedef struct {
    int    ee_id;
    int    ee_type;
    int    ee_category;
    int    wat_con;
    int    air_con;
    int    sol_dis;
    int    noi_con;
    int    year;
    int    active;
} env_eng_t;

typedef struct {
    int    ec_id;
    int    ec_type;
    int    ec_category;
    int    eco_sys;
    int    eco_pro;
    int    eco_res;
    int    eco_eva;
    int    year;
    int    active;
} eco_sci_t;

typedef struct {
    int    em_id;
    int    em_type;
    int    em_category;
    int    env_pla;
    int    env_mon;
    int    env_eva;
    int    env_law;
    int    year;
    int    active;
} env_man_t;

typedef struct {
    int    ep_id;
    int    ep_type;
    int    ep_category;
    int    pro_equ;
    int    pro_mat;
    int    pro_tec;
    int    pro_ind;
    int    year;
    int    active;
} env_pro_t;

typedef struct {
    int    n_env_sci;
    int    n_env_eng;
    int    n_eco_sci;
    int    n_env_man;
    int    n_env_pro;
    int    total_env_che;
    int    total_wat_con;
    int    total_eco_sys;
    int    total_env_pla;
    int    total_pro_equ;
} ena_state_t;

static env_sci_t env_scis[MAX_ENV_SCI];
static env_eng_t env_engs[MAX_ENV_ENG];
static eco_sci_t eco_scis[MAX_ECO_SCI];
static env_man_t env_mans[MAX_ENV_MAN];
static env_pro_t env_pros[MAX_ENV_PRO];
static ena_state_t ena;

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

int ena_init(void) {
    if (initialized) return -1;
    ena.n_env_sci = 0; ena.n_env_eng = 0; ena.n_eco_sci = 0;
    ena.n_env_man = 0; ena.n_env_pro = 0;
    ena.total_env_che = 0; ena.total_wat_con = 0;
    ena.total_eco_sys = 0; ena.total_env_pla = 0;
    ena.total_pro_equ = 0;
    for (int i = 0; i < MAX_ENV_SCI; i++) env_scis[i].active = 0;
    for (int i = 0; i < MAX_ENV_ENG; i++) env_engs[i].active = 0;
    for (int i = 0; i < MAX_ECO_SCI; i++) eco_scis[i].active = 0;
    for (int i = 0; i < MAX_ENV_MAN; i++) env_mans[i].active = 0;
    for (int i = 0; i < MAX_ENV_PRO; i++) env_pros[i].active = 0;
    initialized = 1;
    print_str("[ENA] Environment initialized\n");
    return 0;
}

int ena_env_sci(int st_type, int cat, int ech, int ebi, int eph, int ege, int year) {
    if (ena.n_env_sci >= MAX_ENV_SCI) return -1;
    env_sci_t* e = &env_scis[ena.n_env_sci];
    e->es_id = ena.n_env_sci;
    e->es_type = st_type;
    e->es_category = cat;
    e->env_che = ech;
    e->env_bio = ebi;
    e->env_phy = eph;
    e->env_geo = ege;
    e->year = year;
    e->active = 1;
    ena.total_env_che += ech;
    ena.n_env_sci++;
    print_str("[ENA] Env sci "); print_int(ena.n_env_sci - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" ech="); print_int(ech);
    print_str(" ebi="); print_int(ebi);
    print_str(" eph="); print_int(eph);
    print_str(" ege="); print_int(ege); print_str("\n");
    return ena.n_env_sci - 1;
}

int ena_env_eng(int et_type, int cat, int wco, int aco, int sdi, int nco, int year) {
    if (ena.n_env_eng >= MAX_ENV_ENG) return -1;
    env_eng_t* e = &env_engs[ena.n_env_eng];
    e->ee_id = ena.n_env_eng;
    e->ee_type = et_type;
    e->ee_category = cat;
    e->wat_con = wco;
    e->air_con = aco;
    e->sol_dis = sdi;
    e->noi_con = nco;
    e->year = year;
    e->active = 1;
    ena.total_wat_con += wco;
    ena.n_env_eng++;
    print_str("[ENA] Env eng "); print_int(ena.n_env_eng - 1);
    print_str(" type="); print_int(et_type);
    print_str(" cat="); print_int(cat);
    print_str(" wco="); print_int(wco);
    print_str(" aco="); print_int(aco);
    print_str(" sdi="); print_int(sdi);
    print_str(" nco="); print_int(nco); print_str("\n");
    return ena.n_env_eng - 1;
}

int ena_eco_sci(int ct_type, int cat, int esy, int epr, int ere, int eev, int year) {
    if (ena.n_eco_sci >= MAX_ECO_SCI) return -1;
    eco_sci_t* c = &eco_scis[ena.n_eco_sci];
    c->ec_id = ena.n_eco_sci;
    c->ec_type = ct_type;
    c->ec_category = cat;
    c->eco_sys = esy;
    c->eco_pro = epr;
    c->eco_res = ere;
    c->eco_eva = eev;
    c->year = year;
    c->active = 1;
    ena.total_eco_sys += esy;
    ena.n_eco_sci++;
    print_str("[ENA] Eco sci "); print_int(ena.n_eco_sci - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" esy="); print_int(esy);
    print_str(" epr="); print_int(epr);
    print_str(" ere="); print_int(ere);
    print_str(" eev="); print_int(eev); print_str("\n");
    return ena.n_eco_sci - 1;
}

int ena_env_man(int mt_type, int cat, int epl, int emo, int eev, int elw, int year) {
    if (ena.n_env_man >= MAX_ENV_MAN) return -1;
    env_man_t* e = &env_mans[ena.n_env_man];
    e->em_id = ena.n_env_man;
    e->em_type = mt_type;
    e->em_category = cat;
    e->env_pla = epl;
    e->env_mon = emo;
    e->env_eva = eev;
    e->env_law = elw;
    e->year = year;
    e->active = 1;
    ena.total_env_pla += epl;
    ena.n_env_man++;
    print_str("[ENA] Env man "); print_int(ena.n_env_man - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" epl="); print_int(epl);
    print_str(" emo="); print_int(emo);
    print_str(" eev="); print_int(eev);
    print_str(" elw="); print_int(elw); print_str("\n");
    return ena.n_env_man - 1;
}

int ena_env_pro(int pt_type, int cat, int peq, int pma, int pte, int pid, int year) {
    if (ena.n_env_pro >= MAX_ENV_PRO) return -1;
    env_pro_t* p = &env_pros[ena.n_env_pro];
    p->ep_id = ena.n_env_pro;
    p->ep_type = pt_type;
    p->ep_category = cat;
    p->pro_equ = peq;
    p->pro_mat = pma;
    p->pro_tec = pte;
    p->pro_ind = pid;
    p->year = year;
    p->active = 1;
    ena.total_pro_equ += peq;
    ena.n_env_pro++;
    print_str("[ENA] Env pro "); print_int(ena.n_env_pro - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" peq="); print_int(peq);
    print_str(" pma="); print_int(pma);
    print_str(" pte="); print_int(pte);
    print_str(" pid="); print_int(pid); print_str("\n");
    return ena.n_env_pro - 1;
}

void ena_science_report(void) {
    print_str("[ENA] Environmental science report:\n");
    print_str("  Science categories: "); print_int(ena.n_env_sci); print_str("\n");
    print_str("  Total environmental chemistry: "); print_int(ena.total_env_che); print_str("\n");
}

void ena_engineering_report(void) {
    print_str("[ENA] Environmental engineering report:\n");
    print_str("  Engineering categories: "); print_int(ena.n_env_eng); print_str("\n");
    print_str("  Total water control: "); print_int(ena.total_wat_con); print_str("\n");
}

void ena_full_report(void) {
    print_str("[ENA] Full report:\n");
    print_str("  Ecology categories: "); print_int(ena.n_eco_sci); print_str("\n");
    print_str("  Total ecosystem: "); print_int(ena.total_eco_sys); print_str("\n");
    print_str("  Management categories: "); print_int(ena.n_env_man); print_str("\n");
    print_str("  Total environmental planning: "); print_int(ena.total_env_pla); print_str("\n");
    print_str("  Protection categories: "); print_int(ena.n_env_pro); print_str("\n");
    print_str("  Total protection equipment: "); print_int(ena.total_pro_equ); print_str("\n");
}

void ena_print_state(void) {
    print_str("[ENA] Es="); print_int(ena.n_env_sci);
    print_str(" Ee="); print_int(ena.n_env_eng);
    print_str(" Ec="); print_int(ena.n_eco_sci);
    print_str(" Em="); print_int(ena.n_env_man);
    print_str(" Ep="); print_int(ena.n_env_pro);
    print_str("\n");
}

int main(void) {
    print_str("=== Environment Admin Demo ===\n\n");
    ena_init();

    print_str("Environmental science...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int ech = 55 + (i * 13);
        int ebi = 40 + (i * 10);
        int eph = 22 + (i * 5);
        int ege = 15 + (i * 3);
        int year = 2020 + (i % 5);
        ena_env_sci(type, cat, ech, ebi, eph, ege, year);
    }

    print_str("\nEnvironmental engineering...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int wco = 48 + (i * 11);
        int aco = 35 + (i * 8);
        int sdi = 20 + (i * 4);
        int nco = 12 + (i * 3);
        int year = 2021 + (i % 4);
        ena_env_eng(type, cat, wco, aco, sdi, nco, year);
    }

    print_str("\nEcology...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int esy = 42 + (i * 10);
        int epr = 28 + (i * 7);
        int ere = 18 + (i * 4);
        int eev = 10 + (i * 2);
        int year = 2022 + (i % 3);
        ena_eco_sci(type, cat, esy, epr, ere, eev, year);
    }

    print_str("\nManagement...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int epl = 35 + (i * 8);
        int emo = 25 + (i * 6);
        int eev = 15 + (i * 3);
        int elw = 10 + (i * 2);
        int year = 2023 + (i % 2);
        ena_env_man(type, cat, epl, emo, eev, elw, year);
    }

    print_str("\nProtection...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int peq = 30 + (i * 7);
        int pma = 22 + (i * 5);
        int pte = 12 + (i * 3);
        int pid = 8 + (i * 2);
        int year = 2024;
        ena_env_pro(type, cat, peq, pma, pte, pid, year);
    }

    print_str("\nScience report...\n");
    ena_science_report();

    print_str("\nEngineering report...\n");
    ena_engineering_report();

    print_str("\nFull report...\n");
    ena_full_report();

    print_str("\nFinal state...\n");
    ena_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
