/* illicium_admin: Illicium Star Anise spice & medicinal administration (v1.0)
 * Tree inventory, fruit harvest, shikimic acid analysis, oil distillation
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);

#define N 16

/* Spice grades */
#define ILLICIUM_GRADE_CULINARY    1
#define ILLICIUM_GRADE_MEDICINAL   2
#define ILLICIUM_GRADE_PHARMACEUTICAL 3

/* Market regions */
#define ILLICIUM_MARKET_ASIA       1
#define ILLICIUM_MARKET_EUROPE     2
#define ILLICIUM_MARKET_AMERICAS   3
#define ILLICIUM_MARKET_LOCAL      4

typedef struct {
    int id;
    int tree_id;
    int age_years;
    int fruit_kg;
    int shikimic_ppm;
    int oil_ml;
    int grade;
    int gcms_ok;
    int organic_cert;
    int market_region;
    int active;
} illicium_t;

typedef struct {
    int n_tree;
    int n_harvest;
    int n_shikimic;
    int n_oil;
    int n_grade;
    int n_qc;
    int n_organic;
    int total_fruit;
    int total_shikimic;
    int total_oil;
    int total_revenue;
} illicium_state_t;

static illicium_t illicium_trees[N];
static illicium_t illicium_harvest_recs[N - 2];
static illicium_t illicium_shikimic_recs[N - 4];
static illicium_t illicium_oil_recs[N - 6];
static illicium_t illicium_grade_recs[N - 6];
static illicium_t illicium_qc_recs[N - 8];
static illicium_t illicium_organic_recs[N - 8];
static illicium_state_t st;
static int init;

static void ps(const char *s) { host_print(s); }
static void pi(int v) {
    char b[32]; int i = 0;
    if (v < 0) { b[i++] = '-'; v = -v; }
    if (v == 0) { b[i++] = '0'; }
    else { int s = i; while (v > 0) { b[i++] = '0' + (v % 10); v /= 10; } int e = i - 1; while (s < e) { char t = b[s]; b[s] = b[e]; b[e] = t; s++; e--; } }
    b[i] = '\0'; host_print(b);
}

static int add_illicium(illicium_t *a, int *cnt, int *sum, int mx, int tree, int age, int fruit, int shikimic, int oil, int grade, int gcms, int organic, int market) {
    if (*cnt >= mx) return -1;
    illicium_t *x = &a[*cnt];
    x->id = *cnt; x->tree_id = tree; x->age_years = age;
    x->fruit_kg = fruit; x->shikimic_ppm = shikimic; x->oil_ml = oil;
    x->grade = grade; x->gcms_ok = gcms;
    x->organic_cert = organic; x->market_region = market; x->active = 1;
    *sum += fruit; (*cnt)++;
    ps("[ILLICIUM] Record "); pi(*cnt - 1);
    ps(" tree="); pi(tree); ps(" age="); pi(age);
    ps(" fruit="); pi(fruit); ps(" shikimic="); pi(shikimic);
    ps(" oil="); pi(oil); ps(" grade="); pi(grade); ps("\n");
    return *cnt - 1;
}

int illicium_init(void) {
    if (init) return -1;
    st.n_tree = 0; st.n_harvest = 0; st.n_shikimic = 0;
    st.n_oil = 0; st.n_grade = 0; st.n_qc = 0;
    st.n_organic = 0; st.total_fruit = 0; st.total_shikimic = 0;
    st.total_oil = 0; st.total_revenue = 0;
    for (int i = 0; i < N; i++) illicium_trees[i].active = 0;
    for (int i = 0; i < N - 2; i++) illicium_harvest_recs[i].active = 0;
    for (int i = 0; i < N - 4; i++) illicium_shikimic_recs[i].active = 0;
    for (int i = 0; i < N - 6; i++) illicium_oil_recs[i].active = 0;
    for (int i = 0; i < N - 6; i++) illicium_grade_recs[i].active = 0;
    for (int i = 0; i < N - 8; i++) illicium_qc_recs[i].active = 0;
    for (int i = 0; i < N - 8; i++) illicium_organic_recs[i].active = 0;
    init = 1;
    ps("[ILLICIUM] Illicium admin initialized\n");
    return 0;
}

int illicium_tree_inventory(int tree_id, int age_years) {
    return add_illicium(illicium_trees, &st.n_tree, &st.total_fruit, N,
                        tree_id, age_years, 0, 0, 0, 0, 0, 0, 0);
}

int illicium_fruit_harvest(int tree_id, int fruit_kg) {
    st.total_fruit += fruit_kg;
    return add_illicium(illicium_harvest_recs, &st.n_harvest, &st.total_fruit, N - 2,
                        tree_id, 0, fruit_kg, 0, 0, 0, 0, 0, 0);
}

int illicium_shikimic_analyze(int tree_id, int shikimic_ppm) {
    st.total_shikimic += shikimic_ppm;
    return add_illicium(illicium_shikimic_recs, &st.n_shikimic, &st.total_shikimic, N - 4,
                        tree_id, 0, 0, shikimic_ppm, 0, 0, 0, 0, 0);
}

int illicium_oil_distill(int tree_id, int oil_ml) {
    st.total_oil += oil_ml;
    return add_illicium(illicium_oil_recs, &st.n_oil, &st.total_oil, N - 6,
                        tree_id, 0, 0, 0, oil_ml, 0, 0, 0, 0);
}

int illicium_grade(int tree_id, int grade, int fruit_kg) {
    int price = 0;
    if (grade == ILLICIUM_GRADE_CULINARY) price = fruit_kg * 12;
    else if (grade == ILLICIUM_GRADE_MEDICINAL) price = fruit_kg * 28;
    else if (grade == ILLICIUM_GRADE_PHARMACEUTICAL) price = fruit_kg * 65;
    st.total_revenue += price;
    return add_illicium(illicium_grade_recs, &st.n_grade, &st.total_revenue, N - 6,
                        tree_id, 0, fruit_kg, 0, 0, grade, 0, 0, 0);
}

int illicium_quality_control(int tree_id, int gcms_ok) {
    return add_illicium(illicium_qc_recs, &st.n_qc, &st.n_qc, N - 8,
                        tree_id, 0, 0, 0, 0, 0, gcms_ok, 0, 0);
}

int illicium_organic_cert(int tree_id, int organic_cert, int market_region) {
    return add_illicium(illicium_organic_recs, &st.n_organic, &st.n_organic, N - 8,
                        tree_id, 0, 0, 0, 0, 0, 0, organic_cert, market_region);
}

void illicium_report(void) {
    ps("[ILLICIUM] Trees: "); pi(st.n_tree);
    ps(" Harvest: "); pi(st.n_harvest);
    ps(" Shikimic: "); pi(st.n_shikimic);
    ps(" Oil: "); pi(st.n_oil);
    ps(" Grade: "); pi(st.n_grade);
    ps(" QC: "); pi(st.n_qc);
    ps(" Organic: "); pi(st.n_organic);
    ps("\n[ILLICIUM] Total fruit: "); pi(st.total_fruit); ps(" kg");
    ps("\n[ILLICIUM] Total shikimic: "); pi(st.total_shikimic); ps(" ppm");
    ps("\n[ILLICIUM] Total oil: "); pi(st.total_oil); ps(" ml");
    ps("\n[ILLICIUM] Revenue: "); pi(st.total_revenue); ps(" USD");
    ps("\n");
}

void illicium_state(void) {
    ps("[ILLICIUM] Tr="); pi(st.n_tree);
    ps(" Harv="); pi(st.n_harvest);
    ps(" Shik="); pi(st.n_shikimic);
    ps(" Oil="); pi(st.n_oil);
    ps(" Grd="); pi(st.n_grade);
    ps(" QC="); pi(st.n_qc);
    ps(" Org="); pi(st.n_organic);
    ps("\n");
}

int main(void) {
    ps("=== Illicium Star Anise Admin Demo ===\n\n");
    illicium_init();

    ps("Tree inventory...\n");
    for (int i = 0; i < N; i++) {
        int tree = 300 + i;
        int age = 4 + (i % 10);
        illicium_tree_inventory(tree, age);
    }

    ps("\nFruit harvest...\n");
    for (int i = 0; i < N - 2; i++) {
        int tree = 300 + i;
        int fruit = 5 + (i * 3) % 12;
        illicium_fruit_harvest(tree, fruit);
    }

    ps("\nShikimic acid analysis...\n");
    for (int i = 0; i < N - 4; i++) {
        int tree = 300 + i;
        int shikimic = 100 + (i * 17) % 200;
        illicium_shikimic_analyze(tree, shikimic);
    }

    ps("\nOil distillation...\n");
    for (int i = 0; i < N - 6; i++) {
        int tree = 300 + i;
        int oil = 10 + (i * 4) % 30;
        illicium_oil_distill(tree, oil);
    }

    ps("\nSpice grading...\n");
    for (int i = 0; i < N - 6; i++) {
        int tree = 300 + i;
        int grade = (i % 3) + 1;
        int fruit = 4 + (i * 2) % 8;
        illicium_grade(tree, grade, fruit);
    }

    ps("\nQuality control (GC-MS)...\n");
    for (int i = 0; i < N - 8; i++) {
        int tree = 300 + i;
        int gcms = (i % 2 == 0) ? 1 : 0;
        illicium_quality_control(tree, gcms);
    }

    ps("\nOrganic certification...\n");
    for (int i = 0; i < N - 8; i++) {
        int tree = 300 + i;
        int organic = (i % 3 == 0) ? 1 : 0;
        int market = (i % 4) + 1;
        illicium_organic_cert(tree, organic, market);
    }

    ps("\n");
    illicium_report();
    illicium_state();
    ps("\n=== Demo Complete ===\n");
    return 0;
}
