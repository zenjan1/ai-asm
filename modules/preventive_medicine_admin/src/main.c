/* preventive_medicine_admin: Preventive medicine administration system (v1.0)
 * Epidemiology, health statistics, environmental health, occupational health, nutrition
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

#define MAX_EPIDEMIO    16
#define Health_STAT   14
#define ENV_HEALTH    12
#define OCC_HEALTH    10
#define NUTRITION     10

typedef struct {
    int    ep_id;
    int    ep_type;
    int    ep_category;
    int    inf_epi;
    int    chr_epi;
    int    epi_meth;
    int    dis_surv;
    int    year;
    int    active;
} epidemio_t;

typedef struct {
    int    hs_id;
    int    hs_type;
    int    hs_category;
    int    stat_meth;
    int    data_ana;
    int    hlth_ind;
    int    stat_rep;
    int    year;
    int    active;
} health_stat_t;

typedef struct {
    int    eh_id;
    int    eh_type;
    int    eh_category;
    int    air_env;
    int    water_env;
    int    soil_env;
    int    indoor_env;
    int    year;
    int    active;
} env_health_t;

typedef struct {
    int    oh_id;
    int    oh_type;
    int    oh_category;
    int    occ_haz;
    int    occ_dis;
    int    labor_hlth;
    int    hlth_mon;
    int    year;
    int    active;
} occ_health_t;

typedef struct {
    int    nt_id;
    int    nt_type;
    int    nt_category;
    int    nut_base;
    int    food_hyg;
    int    diet_gui;
    int    nut_mon;
    int    year;
    int    active;
} nutrition_t;

typedef struct {
    int    n_epidemio;
    int    n_health_stat;
    int    n_env_health;
    int    n_occ_health;
    int    n_nutrition;
    int    total_inf_epi;
    int    total_stat_meth;
    int    total_air_env;
    int    total_occ_haz;
    int    total_nut_base;
} pma_state_t;

static epidemio_t epidemios[MAX_EPIDEMIO];
static health_stat_t health_stats[Health_STAT];
static env_health_t env_healths[ENV_HEALTH];
static occ_health_t occ_healths[OCC_HEALTH];
static nutrition_t nutritionss[NUTRITION];
static pma_state_t pma;

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

int pma_init(void) {
    if (initialized) return -1;
    pma.n_epidemio = 0; pma.n_health_stat = 0; pma.n_env_health = 0;
    pma.n_occ_health = 0; pma.n_nutrition = 0;
    pma.total_inf_epi = 0; pma.total_stat_meth = 0;
    pma.total_air_env = 0; pma.total_occ_haz = 0;
    pma.total_nut_base = 0;
    for (int i = 0; i < MAX_EPIDEMIO; i++) epidemios[i].active = 0;
    for (int i = 0; i < Health_STAT; i++) health_stats[i].active = 0;
    for (int i = 0; i < ENV_HEALTH; i++) env_healths[i].active = 0;
    for (int i = 0; i < OCC_HEALTH; i++) occ_healths[i].active = 0;
    for (int i = 0; i < NUTRITION; i++) nutritionss[i].active = 0;
    initialized = 1;
    print_str("[PMA] Preventive medicine initialized\n");
    return 0;
}

int pma_epidemio(int et_type, int cat, int ife, int cre, int epm, int dsv, int year) {
    if (pma.n_epidemio >= MAX_EPIDEMIO) return -1;
    epidemio_t* e = &epidemios[pma.n_epidemio];
    e->ep_id = pma.n_epidemio;
    e->ep_type = et_type;
    e->ep_category = cat;
    e->inf_epi = ife;
    e->chr_epi = cre;
    e->epi_meth = epm;
    e->dis_surv = dsv;
    e->year = year;
    e->active = 1;
    pma.total_inf_epi += ife;
    pma.n_epidemio++;
    print_str("[PMA] Epidemio "); print_int(pma.n_epidemio - 1);
    print_str(" type="); print_int(et_type);
    print_str(" cat="); print_int(cat);
    print_str(" ife="); print_int(ife);
    print_str(" cre="); print_int(cre);
    print_str(" epm="); print_int(epm);
    print_str(" dsv="); print_int(dsv); print_str("\n");
    return pma.n_epidemio - 1;
}

int pma_health_stat(int st_type, int cat, int stm, int dan, int hli, int str, int year) {
    if (pma.n_health_stat >= Health_STAT) return -1;
    health_stat_t* h = &health_stats[pma.n_health_stat];
    h->hs_id = pma.n_health_stat;
    h->hs_type = st_type;
    h->hs_category = cat;
    h->stat_meth = stm;
    h->data_ana = dan;
    h->hlth_ind = hli;
    h->stat_rep = str;
    h->year = year;
    h->active = 1;
    pma.total_stat_meth += stm;
    pma.n_health_stat++;
    print_str("[PMA] Health stat "); print_int(pma.n_health_stat - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" stm="); print_int(stm);
    print_str(" dan="); print_int(dan);
    print_str(" hli="); print_int(hli);
    print_str(" str="); print_int(str); print_str("\n");
    return pma.n_health_stat - 1;
}

int pma_env_health(int ht_type, int cat, int aren, int wten, int slen, int iden, int year) {
    if (pma.n_env_health >= ENV_HEALTH) return -1;
    env_health_t* e = &env_healths[pma.n_env_health];
    e->eh_id = pma.n_env_health;
    e->eh_type = ht_type;
    e->eh_category = cat;
    e->air_env = aren;
    e->water_env = wten;
    e->soil_env = slen;
    e->indoor_env = iden;
    e->year = year;
    e->active = 1;
    pma.total_air_env += aren;
    pma.n_env_health++;
    print_str("[PMA] Env health "); print_int(pma.n_env_health - 1);
    print_str(" type="); print_int(ht_type);
    print_str(" cat="); print_int(cat);
    print_str(" aren="); print_int(aren);
    print_str(" wten="); print_int(wten);
    print_str(" slen="); print_int(slen);
    print_str(" iden="); print_int(iden); print_str("\n");
    return pma.n_env_health - 1;
}

int pma_occ_health(int ot_type, int cat, int och, int ocd, int lbh, int hmn, int year) {
    if (pma.n_occ_health >= OCC_HEALTH) return -1;
    occ_health_t* o = &occ_healths[pma.n_occ_health];
    o->oh_id = pma.n_occ_health;
    o->oh_type = ot_type;
    o->oh_category = cat;
    o->occ_haz = och;
    o->occ_dis = ocd;
    o->labor_hlth = lbh;
    o->hlth_mon = hmn;
    o->year = year;
    o->active = 1;
    pma.total_occ_haz += och;
    pma.n_occ_health++;
    print_str("[PMA] Occ health "); print_int(pma.n_occ_health - 1);
    print_str(" type="); print_int(ot_type);
    print_str(" cat="); print_int(cat);
    print_str(" och="); print_int(och);
    print_str(" ocd="); print_int(ocd);
    print_str(" lbh="); print_int(lbh);
    print_str(" hmn="); print_int(hmn); print_str("\n");
    return pma.n_occ_health - 1;
}

int pma_nutrition(int nt_type, int cat, int nub, int fhy, int dgu, int num, int year) {
    if (pma.n_nutrition >= NUTRITION) return -1;
    nutrition_t* n = &nutritionss[pma.n_nutrition];
    n->nt_id = pma.n_nutrition;
    n->nt_type = nt_type;
    n->nt_category = cat;
    n->nut_base = nub;
    n->food_hyg = fhy;
    n->diet_gui = dgu;
    n->nut_mon = num;
    n->year = year;
    n->active = 1;
    pma.total_nut_base += nub;
    pma.n_nutrition++;
    print_str("[PMA] Nutrition "); print_int(pma.n_nutrition - 1);
    print_str(" type="); print_int(nt_type);
    print_str(" cat="); print_int(cat);
    print_str(" nub="); print_int(nub);
    print_str(" fhy="); print_int(fhy);
    print_str(" dgu="); print_int(dgu);
    print_str(" num="); print_int(num); print_str("\n");
    return pma.n_nutrition - 1;
}

void pma_epi_report(void) {
    print_str("[PMA] Epidemiology report:\n");
    print_str("  Epidemiology categories: "); print_int(pma.n_epidemio); print_str("\n");
    print_str("  Total infectious epidemiology: "); print_int(pma.total_inf_epi); print_str("\n");
}

void pma_stat_report(void) {
    print_str("[PMA] Health statistics report:\n");
    print_str("  Statistics categories: "); print_int(pma.n_health_stat); print_str("\n");
    print_str("  Total statistical methods: "); print_int(pma.total_stat_meth); print_str("\n");
}

void pma_full_report(void) {
    print_str("[PMA] Full report:\n");
    print_str("  Environmental health categories: "); print_int(pma.n_env_health); print_str("\n");
    print_str("  Total air environment: "); print_int(pma.total_air_env); print_str("\n");
    print_str("  Occupational health categories: "); print_int(pma.n_occ_health); print_str("\n");
    print_str("  Total occupational hazards: "); print_int(pma.total_occ_haz); print_str("\n");
    print_str("  Nutrition categories: "); print_int(pma.n_nutrition); print_str("\n");
    print_str("  Total nutrition base: "); print_int(pma.total_nut_base); print_str("\n");
}

void pma_print_state(void) {
    print_str("[PMA] Ep="); print_int(pma.n_epidemio);
    print_str(" Hs="); print_int(pma.n_health_stat);
    print_str(" Eh="); print_int(pma.n_env_health);
    print_str(" Oh="); print_int(pma.n_occ_health);
    print_str(" Nt="); print_int(pma.n_nutrition);
    print_str("\n");
}

int main(void) {
    print_str("=== Preventive Medicine Admin Demo ===\n\n");
    pma_init();

    print_str("Epidemiology...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int ife = 55 + (i * 13);
        int cre = 40 + (i * 10);
        int epm = 22 + (i * 5);
        int dsv = 15 + (i * 3);
        int year = 2020 + (i % 5);
        pma_epidemio(type, cat, ife, cre, epm, dsv, year);
    }

    print_str("\nHealth statistics...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int stm = 48 + (i * 11);
        int dan = 35 + (i * 8);
        int hli = 20 + (i * 4);
        int str = 12 + (i * 3);
        int year = 2021 + (i % 4);
        pma_health_stat(type, cat, stm, dan, hli, str, year);
    }

    print_str("\nEnvironmental health...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int aren = 42 + (i * 10);
        int wten = 28 + (i * 7);
        int slen = 18 + (i * 4);
        int iden = 10 + (i * 2);
        int year = 2022 + (i % 3);
        pma_env_health(type, cat, aren, wten, slen, iden, year);
    }

    print_str("\nOccupational health...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int och = 35 + (i * 8);
        int ocd = 25 + (i * 6);
        int lbh = 15 + (i * 3);
        int hmn = 10 + (i * 2);
        int year = 2023 + (i % 2);
        pma_occ_health(type, cat, och, ocd, lbh, hmn, year);
    }

    print_str("\nNutrition...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int nub = 30 + (i * 7);
        int fhy = 22 + (i * 5);
        int dgu = 12 + (i * 3);
        int num = 8 + (i * 2);
        int year = 2024;
        pma_nutrition(type, cat, nub, fhy, dgu, num, year);
    }

    print_str("\nEpidemiology report...\n");
    pma_epi_report();

    print_str("\nStatistics report...\n");
    pma_stat_report();

    print_str("\nFull report...\n");
    pma_full_report();

    print_str("\nFinal state...\n");
    pma_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
