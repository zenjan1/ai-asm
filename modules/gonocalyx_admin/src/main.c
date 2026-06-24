/* gonocalyx_admin: Gonocalyx ornamental berry plantation administration (v1.0)
 * Gonocalyx planting, growth monitoring, harvest, packaging, market distribution
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);

#define N 16

/* Growth stages */
#define GON_STAGE_SEEDLING    1
#define GON_STAGE_VEGETATIVE  2
#define GON_STAGE_FLOWERING   3
#define GON_STAGE_FRUITING    4

/* Packaging types */
#define GON_PKG_FRESH    1
#define GON_PKG_DRIED    2
#define GON_PKG_FROZEN   3
#define GON_PKG_PREMIUM  4

typedef struct {
    int id;
    int variety;       /* cultivar type 1-8 */
    int stage;         /* growth stage */
    int moisture;      /* soil moisture % */
    int ph;            /* soil pH x10 */
    int pest_score;    /* pest pressure 0-100 */
    int yield_kg;      /* harvest yield */
    int harvest_week;  /* scheduled harvest week */
    int active;
} gon_t;

typedef struct {
    int n_plant;       /* planting records */
    int n_monitor;     /* monitoring records */
    int n_harvest;     /* harvest records */
    int n_pkg;         /* packaging records */
    int n_market;      /* market records */
    int t_yield;       /* total yield */
    int t_revenue;     /* total revenue */
    int avg_moisture;
    int avg_ph;
    int pest_alerts;
} gon_state_t;

static gon_t gon_plants[N];
static gon_t gon_monitors[N-2];
static gon_t gon_harvests[N-4];
static gon_t gon_packages[N-6];
static gon_t gon_markets[N-6];
static gon_state_t st;
static int init;

static void ps(const char *s) { host_print(s); }
static void pi(int v) {
    char b[32]; int i = 0;
    if (v < 0) { b[i++] = '-'; v = -v; }
    if (v == 0) { b[i++] = '0'; }
    else { int s = i; while (v > 0) { b[i++] = '0' + (v % 10); v /= 10; } int e = i - 1; while (s < e) { char t = b[s]; b[s] = b[e]; b[e] = t; s++; e--; } }
    b[i] = '\0'; host_print(b);
}

static int add(gon_t *a, int *cnt, int *sum, int mx, int variety, int stage, int moisture, int ph, int pest, int yld, int week) {
    if (*cnt >= mx) return -1;
    gon_t *x = &a[*cnt];
    x->id = *cnt; x->variety = variety; x->stage = stage;
    x->moisture = moisture; x->ph = ph; x->pest_score = pest;
    x->yield_kg = yld; x->harvest_week = week; x->active = 1;
    *sum += yld; (*cnt)++;
    ps("[GON] Record "); pi(*cnt - 1);
    ps(" var="); pi(variety); ps(" stg="); pi(stage);
    ps(" moi="); pi(moisture); ps(" ph="); pi(ph);
    ps(" pest="); pi(pest); ps(" yld="); pi(yld); ps("\n");
    return *cnt - 1;
}

int gon_init(void) {
    if (init) return -1;
    st.n_plant = 0; st.n_monitor = 0; st.n_harvest = 0;
    st.n_pkg = 0; st.n_market = 0; st.t_yield = 0;
    st.t_revenue = 0; st.avg_moisture = 0; st.avg_ph = 0;
    st.pest_alerts = 0;
    for (int i = 0; i < N; i++) gon_plants[i].active = 0;
    for (int i = 0; i < N - 2; i++) gon_monitors[i].active = 0;
    for (int i = 0; i < N - 4; i++) gon_harvests[i].active = 0;
    for (int i = 0; i < N - 6; i++) gon_packages[i].active = 0;
    for (int i = 0; i < N - 6; i++) gon_markets[i].active = 0;
    init = 1;
    ps("[GON] Gonocalyx plantation initialized\n");
    return 0;
}

int gon_planting(int variety, int moisture, int ph, int week) {
    return add(gon_plants, &st.n_plant, &st.t_yield, N, variety, GON_STAGE_SEEDLING, moisture, ph, 0, 0, week);
}

int gon_growth_monitor(int variety, int stage, int moisture, int ph, int pest, int week) {
    if (pest > 70) st.pest_alerts++;
    st.avg_moisture += moisture;
    st.avg_ph += ph;
    return add(gon_monitors, &st.n_monitor, &st.t_yield, N - 2, variety, stage, moisture, ph, pest, 0, week);
}

int gon_harvest(int variety, int stage, int moisture, int ph, int pest, int yld, int week) {
    return add(gon_harvests, &st.n_harvest, &st.t_yield, N - 4, variety, stage, moisture, ph, pest, yld, week);
}

int gon_packaging(int variety, int pkg_type, int yld, int week) {
    int price = 0;
    if (pkg_type == GON_PKG_FRESH) price = yld * 8;
    else if (pkg_type == GON_PKG_DRIED) price = yld * 15;
    else if (pkg_type == GON_PKG_FROZEN) price = yld * 12;
    else if (pkg_type == GON_PKG_PREMIUM) price = yld * 25;
    st.t_revenue += price;
    return add(gon_packages, &st.n_pkg, &st.t_yield, N - 6, variety, pkg_type, 0, 0, 0, yld, week);
}

int gon_market(int variety, int channel, int qty, int price, int week) {
    st.t_revenue += qty * price;
    return add(gon_markets, &st.n_market, &st.t_revenue, N - 6, variety, channel, qty, price, 0, qty * price, week);
}

void gon_report(void) {
    ps("[GON] Plants: "); pi(st.n_plant);
    ps(" Monitor: "); pi(st.n_monitor);
    ps(" Harvest: "); pi(st.n_harvest);
    ps(" Pkg: "); pi(st.n_pkg);
    ps(" Market: "); pi(st.n_market);
    ps("\n[GON] Total yield: "); pi(st.t_yield); ps(" kg");
    ps("\n[GON] Total revenue: "); pi(st.t_revenue); ps(" USD");
    ps("\n[GON] Pest alerts: "); pi(st.pest_alerts);
    ps("\n");
}

void gon_state(void) {
    ps("[GON] Plant="); pi(st.n_plant);
    ps(" Mon="); pi(st.n_monitor);
    ps(" Harv="); pi(st.n_harvest);
    ps(" Pkg="); pi(st.n_pkg);
    ps(" Mkt="); pi(st.n_market);
    ps("\n");
}

int main(void) {
    ps("=== Gonocalyx Plantation Admin Demo ===\n\n");
    gon_init();

    ps("Planting cycle...\n");
    for (int i = 0; i < N; i++) {
        int variety = (i % 5) + 1;
        int moisture = 55 + (i * 3) % 25;
        int ph = 58 + (i * 2) % 18;  /* pH 5.8-7.5 */
        int week = 2020 + (i % 5);
        gon_planting(variety, moisture, ph, week);
    }

    ps("\nGrowth monitoring...\n");
    for (int i = 0; i < N - 2; i++) {
        int variety = (i % 5) + 1;
        int stage = (i % 4) + 1;
        int moisture = 60 + (i * 2) % 20;
        int ph = 60 + (i * 2) % 15;
        int pest = (i * 7) % 100;
        int week = 2021 + (i % 4);
        gon_growth_monitor(variety, stage, moisture, ph, pest, week);
    }

    ps("\nHarvest...\n");
    for (int i = 0; i < N - 4; i++) {
        int variety = (i % 5) + 1;
        int stage = GON_STAGE_FRUITING;
        int moisture = 65 + (i % 15);
        int ph = 62 + (i % 12);
        int pest = (i * 5) % 50;
        int yld = 120 + (i * 17);
        int week = 2022 + (i % 3);
        gon_harvest(variety, stage, moisture, ph, pest, yld, week);
    }

    ps("\nPackaging...\n");
    for (int i = 0; i < N - 6; i++) {
        int variety = (i % 4) + 1;
        int pkg = (i % 4) + 1;
        int yld = 80 + (i * 13);
        int week = 2023 + (i % 2);
        gon_packaging(variety, pkg, yld, week);
    }

    ps("\nMarket distribution...\n");
    for (int i = 0; i < N - 6; i++) {
        int variety = (i % 4) + 1;
        int channel = (i % 3) + 1;  /* retail/wholesale/export */
        int qty = 40 + (i * 8);
        int price = 10 + (i % 15);
        int week = 2024;
        gon_market(variety, channel, qty, price, week);
    }

    ps("\n");
    gon_report();
    gon_state();
    ps("\n=== Demo Complete ===\n");
    return 0;
}
