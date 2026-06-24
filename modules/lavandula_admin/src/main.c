/* lavandula_admin: Lavandula lavender administration (v1.0)
 * Plant inventory, flower harvest, oil distillation, product formulation
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);

#define N 16

/* Drying methods */
#define LAV_DRY_AIR        1
#define LAV_DRY_OVEN       2
#define LAV_DRY_FREEZE     3
#define LAV_DRY_DEHYDRATOR 4

/* Product types */
#define LAV_PROD_CULINARY  1
#define LAV_PROD_MEDICINAL 2
#define LAV_PROD_COSMETIC  3
#define LAV_PROD_AROMATHERAPY 4

/* Market regions */
#define LAV_MARKET_LOCAL   1
#define LAV_MARKET_NATIONAL 2
#define LAV_MARKET_EXPORT  3

/* Organic cert levels */
#define LAV_ORG_NONE       0
#define LAV_ORG_PARTIAL    1
#define LAV_ORG_FULL       2

typedef struct {
    int id;
    int plant_id;
    int age_years;
    int flower_kg;
    int oil_ml;
    int linalool_pct;
    int dry_method;
    int product_type;
    int organic_cert;
    int market_region;
    int active;
} lavandula_t;

typedef struct {
    int n_plant;
    int n_harvest;
    int n_oil;
    int n_analyze;
    int n_dry;
    int n_product;
    int n_organic;
    int total_flower;
    int total_oil;
    int avg_linalool;
    int total_revenue;
} lavandula_state_t;

static lavandula_t lavandula_plants[N];
static lavandula_t lavandula_harvest_recs[N - 2];
static lavandula_t lavandula_oil_recs[N - 4];
static lavandula_t lavandula_analyze_recs[N - 6];
static lavandula_t lavandula_dry_recs[N - 6];
static lavandula_t lavandula_product_recs[N - 8];
static lavandula_t lavandula_organic_recs[N - 8];
static lavandula_state_t st;
static int init;

static void ps(const char *s) { host_print(s); }
static void pi(int v) {
    char b[32]; int i = 0;
    if (v < 0) { b[i++] = '-'; v = -v; }
    if (v == 0) { b[i++] = '0'; }
    else { int s = i; while (v > 0) { b[i++] = '0' + (v % 10); v /= 10; } int e = i - 1; while (s < e) { char t = b[s]; b[s] = b[e]; b[e] = t; s++; e--; } }
    b[i] = '\0'; host_print(b);
}

static int add_lavandula(lavandula_t *a, int *cnt, int *sum, int mx, int plant, int age, int flower, int oil, int linalool, int dry, int product, int organic, int market) {
    if (*cnt >= mx) return -1;
    lavandula_t *x = &a[*cnt];
    x->id = *cnt; x->plant_id = plant; x->age_years = age;
    x->flower_kg = flower; x->oil_ml = oil; x->linalool_pct = linalool;
    x->dry_method = dry; x->product_type = product;
    x->organic_cert = organic; x->market_region = market; x->active = 1;
    *sum += flower; (*cnt)++;
    ps("[LAVANDULA] Record "); pi(*cnt - 1);
    ps(" plant="); pi(plant); ps(" age="); pi(age);
    ps(" flower="); pi(flower); ps(" oil="); pi(oil);
    ps(" linalool="); pi(linalool); ps(" dry="); pi(dry); ps("\n");
    return *cnt - 1;
}

int lavandula_init(void) {
    if (init) return -1;
    st.n_plant = 0; st.n_harvest = 0; st.n_oil = 0;
    st.n_analyze = 0; st.n_dry = 0; st.n_product = 0;
    st.n_organic = 0; st.total_flower = 0; st.total_oil = 0;
    st.avg_linalool = 0; st.total_revenue = 0;
    for (int i = 0; i < N; i++) lavandula_plants[i].active = 0;
    for (int i = 0; i < N - 2; i++) lavandula_harvest_recs[i].active = 0;
    for (int i = 0; i < N - 4; i++) lavandula_oil_recs[i].active = 0;
    for (int i = 0; i < N - 6; i++) lavandula_analyze_recs[i].active = 0;
    for (int i = 0; i < N - 6; i++) lavandula_dry_recs[i].active = 0;
    for (int i = 0; i < N - 8; i++) lavandula_product_recs[i].active = 0;
    for (int i = 0; i < N - 8; i++) lavandula_organic_recs[i].active = 0;
    init = 1;
    ps("[LAVANDULA] Lavandula admin initialized\n");
    return 0;
}

int lavandula_plant_inventory(int plant_id, int age_years) {
    return add_lavandula(lavandula_plants, &st.n_plant, &st.total_flower, N,
                         plant_id, age_years, 0, 0, 0, 0, 0, 0, 0);
}

int lavandula_flower_harvest(int plant_id, int flower_kg) {
    st.total_flower += flower_kg;
    return add_lavandula(lavandula_harvest_recs, &st.n_harvest, &st.total_flower, N - 2,
                         plant_id, 0, flower_kg, 0, 0, 0, 0, 0, 0);
}

int lavandula_oil_distill(int plant_id, int oil_ml) {
    st.total_oil += oil_ml;
    return add_lavandula(lavandula_oil_recs, &st.n_oil, &st.total_oil, N - 4,
                         plant_id, 0, 0, oil_ml, 0, 0, 0, 0, 0);
}

int lavandula_oil_analyze(int plant_id, int linalool_pct) {
    st.avg_linalool += linalool_pct;
    return add_lavandula(lavandula_analyze_recs, &st.n_analyze, &st.avg_linalool, N - 6,
                         plant_id, 0, 0, 0, linalool_pct, 0, 0, 0, 0);
}

int lavandula_dry_process(int plant_id, int dry_method) {
    return add_lavandula(lavandula_dry_recs, &st.n_dry, &st.n_dry, N - 6,
                         plant_id, 0, 0, 0, 0, dry_method, 0, 0, 0);
}

int lavandula_product_formulate(int plant_id, int product_type, int market_region) {
    int price = 0;
    if (product_type == LAV_PROD_CULINARY) price = 25;
    else if (product_type == LAV_PROD_MEDICINAL) price = 45;
    else if (product_type == LAV_PROD_COSMETIC) price = 35;
    else price = 30;
    st.total_revenue += price;
    return add_lavandula(lavandula_product_recs, &st.n_product, &st.total_revenue, N - 8,
                         plant_id, 0, 0, 0, 0, 0, product_type, 0, market_region);
}

int lavandula_organic_cert(int plant_id, int organic_cert) {
    return add_lavandula(lavandula_organic_recs, &st.n_organic, &st.n_organic, N - 8,
                         plant_id, 0, 0, 0, 0, 0, 0, organic_cert, 0);
}

void lavandula_report(void) {
    ps("[LAVANDULA] Plants: "); pi(st.n_plant);
    ps(" Harvest: "); pi(st.n_harvest);
    ps(" Oil: "); pi(st.n_oil);
    ps(" Analyze: "); pi(st.n_analyze);
    ps(" Dry: "); pi(st.n_dry);
    ps(" Product: "); pi(st.n_product);
    ps(" Organic: "); pi(st.n_organic);
    ps("\n[LAVANDULA] Total flower: "); pi(st.total_flower); ps(" kg");
    ps("\n[LAVANDULA] Total oil: "); pi(st.total_oil); ps(" ml");
    ps("\n[LAVANDULA] Avg linalool: "); pi(st.avg_linalool); ps(" %");
    ps("\n[LAVANDULA] Revenue: "); pi(st.total_revenue); ps(" USD");
    ps("\n");
}

void lavandula_state(void) {
    ps("[LAVANDULA] Pl="); pi(st.n_plant);
    ps(" Har="); pi(st.n_harvest);
    ps(" Oil="); pi(st.n_oil);
    ps(" Anl="); pi(st.n_analyze);
    ps(" Dry="); pi(st.n_dry);
    ps(" Prd="); pi(st.n_product);
    ps(" Org="); pi(st.n_organic);
    ps("\n");
}

int main(void) {
    ps("=== Lavandula Lavender Admin Demo ===\n\n");
    lavandula_init();

    ps("Plant inventory...\n");
    for (int i = 0; i < N; i++) {
        int plant = 1000 + i;
        int age = 2 + (i % 6);
        lavandula_plant_inventory(plant, age);
    }

    ps("\nFlower harvest...\n");
    for (int i = 0; i < N - 2; i++) {
        int plant = 1000 + i;
        int flower = 3 + (i * 2) % 8;
        lavandula_flower_harvest(plant, flower);
    }

    ps("\nOil distillation...\n");
    for (int i = 0; i < N - 4; i++) {
        int plant = 1000 + i;
        int oil = 50 + (i * 15) % 100;
        lavandula_oil_distill(plant, oil);
    }

    ps("\nOil quality analysis...\n");
    for (int i = 0; i < N - 6; i++) {
        int plant = 1000 + i;
        int linalool = 25 + (i * 5) % 30;
        lavandula_oil_analyze(plant, linalool);
    }

    ps("\nDrying process...\n");
    for (int i = 0; i < N - 6; i++) {
        int plant = 1000 + i;
        int dry = (i % 4) + 1;
        lavandula_dry_process(plant, dry);
    }

    ps("\nProduct formulation...\n");
    for (int i = 0; i < N - 8; i++) {
        int plant = 1000 + i;
        int product = (i % 4) + 1;
        int market = (i % 3) + 1;
        lavandula_product_formulate(plant, product, market);
    }

    ps("\nOrganic certification...\n");
    for (int i = 0; i < N - 8; i++) {
        int plant = 1000 + i;
        int organic = i % 3;
        lavandula_organic_cert(plant, organic);
    }

    ps("\n");
    lavandula_report();
    lavandula_state();
    ps("\n=== Demo Complete ===\n");
    return 0;
}
