/* synthetic_diamond_admin: Synthetic diamond administration system (v1.0)
 * HPHT synthesis, CVD synthesis, diamond coating, industrial diamond, gem diamond
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

#define MAX_HPHT       16
#define MAX_CVD        14
#define MAX_COATING    12
#define MAX_INDUST     10
#define MAX_GEM        10

typedef struct {
    int    hpht_id;
    int    hpht_type;
    int    hpht_category;
    int    high_pressure;
    int    high_temp;
    int    catalyst;
    int    year;
    int    active;
} hpht_t;

typedef struct {
    int    cvd_id;
    int    cvd_type;
    int    cvd_category;
    int    plasma_cvd;
    int    hot_filament;
    int    microwave_cvd;
    int    year;
    int    active;
} cvd_t;

typedef struct {
    int    coat_id;
    int    coat_type;
    int    coat_category;
    int    film_coat;
    int    hard_coat;
    int    tool_coat;
    int    year;
    int    active;
} coating_t;

typedef struct {
    int    indust_id;
    int    indust_type;
    int    indust_category;
    int    diamond_tool;
    int    diamond_abras;
    int    diamond_drill;
    int    year;
    int    active;
} indust_t;

typedef struct {
    int    gem_id;
    int    gem_type;
    int    gem_category;
    int    jewelry_gem;
    int    industrial_gem;
    int    optical_gem;
    int    year;
    int    active;
} gem_t;

typedef struct {
    int    n_hpht;
    int    n_cvd;
    int    n_coating;
    int    n_indust;
    int    n_gem;
    int    total_pressure;
    int    total_plasma;
    int    total_film;
    int    total_tool;
    int    total_jewelry;
} sda_state_t;

static hpht_t hphts[MAX_HPHT];
static cvd_t cvds[MAX_CVD];
static coating_t coatings[MAX_COATING];
static indust_t indus[MAX_INDUST];
static gem_t gems[MAX_GEM];
static sda_state_t sda;

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

int sda_init(void) {
    if (initialized) return -1;
    sda.n_hpht = 0; sda.n_cvd = 0; sda.n_coating = 0;
    sda.n_indust = 0; sda.n_gem = 0;
    sda.total_pressure = 0; sda.total_plasma = 0;
    sda.total_film = 0; sda.total_tool = 0;
    sda.total_jewelry = 0;
    for (int i = 0; i < MAX_HPHT; i++) hphts[i].active = 0;
    for (int i = 0; i < MAX_CVD; i++) cvds[i].active = 0;
    for (int i = 0; i < MAX_COATING; i++) coatings[i].active = 0;
    for (int i = 0; i < MAX_INDUST; i++) indus[i].active = 0;
    for (int i = 0; i < MAX_GEM; i++) gems[i].active = 0;
    initialized = 1;
    print_str("[SDA] Synthetic diamond initialized\n");
    return 0;
}

int sda_hpht(int h_type, int cat, int press, int temp_v, int catl, int year) {
    if (sda.n_hpht >= MAX_HPHT) return -1;
    hpht_t* h = &hphts[sda.n_hpht];
    h->hpht_id = sda.n_hpht;
    h->hpht_type = h_type;
    h->hpht_category = cat;
    h->high_pressure = press;
    h->high_temp = temp_v;
    h->catalyst = catl;
    h->year = year;
    h->active = 1;
    sda.total_pressure += press;
    sda.n_hpht++;
    print_str("[SDA] HPHT "); print_int(sda.n_hpht - 1);
    print_str(" type="); print_int(h_type);
    print_str(" cat="); print_int(cat);
    print_str(" prs="); print_int(press);
    print_str(" tmp="); print_int(temp_v);
    print_str(" ctl="); print_int(catl); print_str("\n");
    return sda.n_hpht - 1;
}

int sda_cvd(int c_type, int cat, int plasma, int filament, int micro_cvd, int year) {
    if (sda.n_cvd >= MAX_CVD) return -1;
    cvd_t* c = &cvds[sda.n_cvd];
    c->cvd_id = sda.n_cvd;
    c->cvd_type = c_type;
    c->cvd_category = cat;
    c->plasma_cvd = plasma;
    c->hot_filament = filament;
    c->microwave_cvd = micro_cvd;
    c->year = year;
    c->active = 1;
    sda.total_plasma += plasma;
    sda.n_cvd++;
    print_str("[SDA] CVD "); print_int(sda.n_cvd - 1);
    print_str(" type="); print_int(c_type);
    print_str(" cat="); print_int(cat);
    print_str(" pls="); print_int(plasma);
    print_str(" flm="); print_int(filament);
    print_str(" mcvd="); print_int(micro_cvd); print_str("\n");
    return sda.n_cvd - 1;
}

int sda_coating(int ct_type, int cat, int film_c, int hard_c, int tool_c, int year) {
    if (sda.n_coating >= MAX_COATING) return -1;
    coating_t* c = &coatings[sda.n_coating];
    c->coat_id = sda.n_coating;
    c->coat_type = ct_type;
    c->coat_category = cat;
    c->film_coat = film_c;
    c->hard_coat = hard_c;
    c->tool_coat = tool_c;
    c->year = year;
    c->active = 1;
    sda.total_film += film_c;
    sda.n_coating++;
    print_str("[SDA] Coating "); print_int(sda.n_coating - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" flm="); print_int(film_c);
    print_str(" hrd="); print_int(hard_c);
    print_str(" tls="); print_int(tool_c); print_str("\n");
    return sda.n_coating - 1;
}

int sda_indust(int i_type, int cat, int tool_v, int abras_v, int drill_v, int year) {
    if (sda.n_indust >= MAX_INDUST) return -1;
    indust_t* ind = &indus[sda.n_indust];
    ind->indust_id = sda.n_indust;
    ind->indust_type = i_type;
    ind->indust_category = cat;
    ind->diamond_tool = tool_v;
    ind->diamond_abras = abras_v;
    ind->diamond_drill = drill_v;
    ind->year = year;
    ind->active = 1;
    sda.total_tool += tool_v;
    sda.n_indust++;
    print_str("[SDA] Indust "); print_int(sda.n_indust - 1);
    print_str(" type="); print_int(i_type);
    print_str(" cat="); print_int(cat);
    print_str(" tls="); print_int(tool_v);
    print_str(" abr="); print_int(abras_v);
    print_str(" drl="); print_int(drill_v); print_str("\n");
    return sda.n_indust - 1;
}

int sda_gem(int g_type, int cat, int jewel, int ind_gem, int opt_gem, int year) {
    if (sda.n_gem >= MAX_GEM) return -1;
    gem_t* g = &gems[sda.n_gem];
    g->gem_id = sda.n_gem;
    g->gem_type = g_type;
    g->gem_category = cat;
    g->jewelry_gem = jewel;
    g->industrial_gem = ind_gem;
    g->optical_gem = opt_gem;
    g->year = year;
    g->active = 1;
    sda.total_jewelry += jewel;
    sda.n_gem++;
    print_str("[SDA] Gem "); print_int(sda.n_gem - 1);
    print_str(" type="); print_int(g_type);
    print_str(" cat="); print_int(cat);
    print_str(" jwl="); print_int(jewel);
    print_str(" ind="); print_int(ind_gem);
    print_str(" opt="); print_int(opt_gem); print_str("\n");
    return sda.n_gem - 1;
}

void sda_hpht_report(void) {
    print_str("[SDA] HPHT report:\n");
    print_str("  HPHT synthesis categories: "); print_int(sda.n_hpht); print_str("\n");
    print_str("  Total high pressure: "); print_int(sda.total_pressure); print_str("\n");
}

void sda_cvd_report(void) {
    print_str("[SDA] CVD report:\n");
    print_str("  CVD synthesis categories: "); print_int(sda.n_cvd); print_str("\n");
    print_str("  Total plasma CVD: "); print_int(sda.total_plasma); print_str("\n");
}

void sda_gem_report(void) {
    print_str("[SDA] Gem report:\n");
    print_str("  Diamond coating categories: "); print_int(sda.n_coating); print_str("\n");
    print_str("  Total film coating: "); print_int(sda.total_film); print_str("\n");
    print_str("  Industrial diamond categories: "); print_int(sda.n_indust); print_str("\n");
    print_str("  Total diamond tools: "); print_int(sda.total_tool); print_str("\n");
    print_str("  Gem diamond categories: "); print_int(sda.n_gem); print_str("\n");
    print_str("  Total jewelry diamond: "); print_int(sda.total_jewelry); print_str("\n");
}

void sda_print_state(void) {
    print_str("[SDA] Hp="); print_int(sda.n_hpht);
    print_str(" Cv="); print_int(sda.n_cvd);
    print_str(" Ct="); print_int(sda.n_coating);
    print_str(" In="); print_int(sda.n_indust);
    print_str(" Gm="); print_int(sda.n_gem);
    print_str("\n");
}

int main(void) {
    print_str("=== Synthetic Diamond Admin Demo ===\n\n");
    sda_init();

    print_str("HPHT synthesis...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int prs = 55 + (i * 13);
        int tmp = 40 + (i * 10);
        int ctl = 22 + (i * 5);
        int year = 2020 + (i % 5);
        sda_hpht(type, cat, prs, tmp, ctl, year);
    }

    print_str("\nCVD synthesis...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pls = 48 + (i * 12);
        int flm = 35 + (i * 8);
        int mcvd = 20 + (i * 5);
        int year = 2021 + (i % 4);
        sda_cvd(type, cat, pls, flm, mcvd, year);
    }

    print_str("\nDiamond coating...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int flc = 42 + (i * 10);
        int hrd = 28 + (i * 7);
        int tls = 18 + (i * 4);
        int year = 2022 + (i % 3);
        sda_coating(type, cat, flc, hrd, tls, year);
    }

    print_str("\nIndustrial diamond...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int tl = 35 + (i * 8);
        int abr = 25 + (i * 6);
        int drl = 15 + (i * 3);
        int year = 2023 + (i % 2);
        sda_indust(type, cat, tl, abr, drl, year);
    }

    print_str("\nGem diamond...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int jwl = 30 + (i * 7);
        int igm = 22 + (i * 5);
        int ogm = 12 + (i * 3);
        int year = 2024;
        sda_gem(type, cat, jwl, igm, ogm, year);
    }

    print_str("\nHPHT report...\n");
    sda_hpht_report();

    print_str("\nCVD report...\n");
    sda_cvd_report();

    print_str("\nGem report...\n");
    sda_gem_report();

    print_str("\nFinal state...\n");
    sda_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
