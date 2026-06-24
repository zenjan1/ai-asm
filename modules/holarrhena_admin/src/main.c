/* holarrhena_admin: Holarrhena conessi medicinal bark administration (v1.0)
 * Tree inventory, bark harvest, alkaloid profiling, Ayurvedic formulation
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);

#define N 16

/* Drying methods */
#define HOLA_DRY_SUN     1
#define HOLA_DRY_SHADE   2
#define HOLA_DRY_OVEN    3
#define HOLA_DRY_FREEZE  4

/* Pharmaceutical grades */
#define HOLA_GRADE_RAW   1
#define HOLA_GRADE_STD   2   /* standardized */
#define HOLA_GRADE_PHARM 3   /* pharmaceutical */

/* Ayurvedic uses */
#define HOLA_USE_DYSNTERY   1
#define HOLA_USE_FEVER      2
#define HOLA_USE_SKIN       3
#define HOLA_USE_DIGESTIVE  4

typedef struct {
    int id;
    int tree_id;
    int age_years;
    int bark_kg;
    int alkaloid_ppm;    /* conessine content */
    int grade;
    int dry_method;
    int cure_week;
    int ayurvedic_use;
    int replant_year;
    int active;
} hola_t;

typedef struct {
    int n_tree;
    int n_harvest;
    int n_alkaloid;
    int n_dry;
    int n_grade;
    int n_ayurvedic;
    int n_replant;
    int total_bark;
    int avg_alkaloid;
    int total_revenue;
} hola_state_t;

static hola_t hola_trees[N];
static hola_t hola_harvest_recs[N - 2];
static hola_t hola_alkaloid_recs[N - 4];
static hola_t hola_dry_recs[N - 6];
static hola_t hola_grade_recs[N - 6];
static hola_t hola_ayurvedic_recs[N - 8];
static hola_t hola_replant_recs[N - 8];
static hola_state_t st;
static int init;

static void ps(const char *s) { host_print(s); }
static void pi(int v) {
    char b[32]; int i = 0;
    if (v < 0) { b[i++] = '-'; v = -v; }
    if (v == 0) { b[i++] = '0'; }
    else { int s = i; while (v > 0) { b[i++] = '0' + (v % 10); v /= 10; } int e = i - 1; while (s < e) { char t = b[s]; b[s] = b[e]; b[e] = t; s++; e--; } }
    b[i] = '\0'; host_print(b);
}

static int add_hola(hola_t *a, int *cnt, int *sum, int mx, int tree, int age, int bark, int alk, int grade, int dry, int cure, int use, int replant) {
    if (*cnt >= mx) return -1;
    hola_t *x = &a[*cnt];
    x->id = *cnt; x->tree_id = tree; x->age_years = age;
    x->bark_kg = bark; x->alkaloid_ppm = alk; x->grade = grade;
    x->dry_method = dry; x->cure_week = cure;
    x->ayurvedic_use = use; x->replant_year = replant; x->active = 1;
    *sum += bark; (*cnt)++;
    ps("[HOLA] Record "); pi(*cnt - 1);
    ps(" tree="); pi(tree); ps(" age="); pi(age);
    ps(" bark="); pi(bark); ps(" alk="); pi(alk);
    ps(" grade="); pi(grade); ps(" dry="); pi(dry); ps("\n");
    return *cnt - 1;
}

int hola_init(void) {
    if (init) return -1;
    st.n_tree = 0; st.n_harvest = 0; st.n_alkaloid = 0;
    st.n_dry = 0; st.n_grade = 0; st.n_ayurvedic = 0;
    st.n_replant = 0; st.total_bark = 0; st.avg_alkaloid = 0;
    st.total_revenue = 0;
    for (int i = 0; i < N; i++) hola_trees[i].active = 0;
    for (int i = 0; i < N - 2; i++) hola_harvest_recs[i].active = 0;
    for (int i = 0; i < N - 4; i++) hola_alkaloid_recs[i].active = 0;
    for (int i = 0; i < N - 6; i++) hola_dry_recs[i].active = 0;
    for (int i = 0; i < N - 6; i++) hola_grade_recs[i].active = 0;
    for (int i = 0; i < N - 8; i++) hola_ayurvedic_recs[i].active = 0;
    for (int i = 0; i < N - 8; i++) hola_replant_recs[i].active = 0;
    init = 1;
    ps("[HOLA] Holarrhena admin initialized\n");
    return 0;
}

int hola_tree_inventory(int tree_id, int age_years) {
    return add_hola(hola_trees, &st.n_tree, &st.total_bark, N,
                    tree_id, age_years, 0, 0, 0, 0, 0, 0, 0);
}

int hola_bark_harvest(int tree_id, int bark_kg, int alkaloid_ppm) {
    st.total_bark += bark_kg;
    st.avg_alkaloid += alkaloid_ppm;
    return add_hola(hola_harvest_recs, &st.n_harvest, &st.total_bark, N - 2,
                    tree_id, 0, bark_kg, alkaloid_ppm, 0, 0, 0, 0, 0);
}

int hola_alkaloid_profile(int tree_id, int alkaloid_ppm) {
    st.avg_alkaloid += alkaloid_ppm;
    return add_hola(hola_alkaloid_recs, &st.n_alkaloid, &st.avg_alkaloid, N - 4,
                    tree_id, 0, 0, alkaloid_ppm, 0, 0, 0, 0, 0);
}

int hola_drying(int tree_id, int bark_kg, int dry_method, int cure_week) {
    return add_hola(hola_dry_recs, &st.n_dry, &st.total_bark, N - 6,
                    tree_id, 0, bark_kg, 0, 0, dry_method, cure_week, 0, 0);
}

int hola_grade(int tree_id, int grade, int bark_kg) {
    int price = 0;
    if (grade == HOLA_GRADE_RAW) price = bark_kg * 8;
    else if (grade == HOLA_GRADE_STD) price = bark_kg * 18;
    else if (grade == HOLA_GRADE_PHARM) price = bark_kg * 45;
    st.total_revenue += price;
    return add_hola(hola_grade_recs, &st.n_grade, &st.total_revenue, N - 6,
                    tree_id, 0, bark_kg, 0, grade, 0, 0, 0, 0);
}

int hola_ayurvedic_use(int tree_id, int use_type, int qty) {
    return add_hola(hola_ayurvedic_recs, &st.n_ayurvedic, &st.n_ayurvedic, N - 8,
                    tree_id, 0, qty, 0, 0, 0, 0, use_type, 0);
}

int hola_replant(int tree_id, int replant_year) {
    return add_hola(hola_replant_recs, &st.n_replant, &st.n_replant, N - 8,
                    tree_id, 0, 0, 0, 0, 0, 0, 0, replant_year);
}

void hola_report(void) {
    ps("[HOLA] Tree: "); pi(st.n_tree);
    ps(" Harvest: "); pi(st.n_harvest);
    ps(" Alk: "); pi(st.n_alkaloid);
    ps(" Dry: "); pi(st.n_dry);
    ps(" Grade: "); pi(st.n_grade);
    ps(" Ayurv: "); pi(st.n_ayurvedic);
    ps(" Replant: "); pi(st.n_replant);
    ps("\n[HOLA] Total bark: "); pi(st.total_bark); ps(" kg");
    ps("\n[HOLA] Avg alkaloid: "); pi(st.avg_alkaloid); ps(" ppm");
    ps("\n[HOLA] Revenue: "); pi(st.total_revenue); ps(" USD");
    ps("\n");
}

void hola_state(void) {
    ps("[HOLA] Tr="); pi(st.n_tree);
    ps(" Harv="); pi(st.n_harvest);
    ps(" Alk="); pi(st.n_alkaloid);
    ps(" Dry="); pi(st.n_dry);
    ps(" Grd="); pi(st.n_grade);
    ps(" Ayu="); pi(st.n_ayurvedic);
    ps(" Rpl="); pi(st.n_replant);
    ps("\n");
}

int main(void) {
    ps("=== Holarrhena Conessi Admin Demo ===\n\n");
    hola_init();

    ps("Tree inventory...\n");
    for (int i = 0; i < N; i++) {
        int tree = 100 + i;
        int age = 3 + (i % 12);
        hola_tree_inventory(tree, age);
    }

    ps("\nBark harvest cycles...\n");
    for (int i = 0; i < N - 2; i++) {
        int tree = 100 + i;
        int bark = 2 + (i * 3) % 10;
        int alk = 80 + (i * 11) % 120;
        hola_bark_harvest(tree, bark, alk);
    }

    ps("\nAlkaloid profiling...\n");
    for (int i = 0; i < N - 4; i++) {
        int tree = 100 + i;
        int alk = 90 + (i * 13) % 140;
        hola_alkaloid_profile(tree, alk);
    }

    ps("\nDrying methods...\n");
    for (int i = 0; i < N - 6; i++) {
        int tree = 100 + i;
        int bark = 3 + (i * 2) % 8;
        int method = (i % 4) + 1;
        int cure = 1 + (i % 4);
        hola_drying(tree, bark, method, cure);
    }

    ps("\nPharmaceutical grading...\n");
    for (int i = 0; i < N - 6; i++) {
        int tree = 100 + i;
        int grade = (i % 3) + 1;
        int bark = 4 + (i * 2) % 8;
        hola_grade(tree, grade, bark);
    }

    ps("\nAyurvedic formulations...\n");
    for (int i = 0; i < N - 8; i++) {
        int tree = 100 + i;
        int use = (i % 4) + 1;
        int qty = 10 + (i * 5);
        hola_ayurvedic_use(tree, use, qty);
    }

    ps("\nReplanting schedule...\n");
    for (int i = 0; i < N - 8; i++) {
        int tree = 100 + i;
        int year = 2025 + (i % 5);
        hola_replant(tree, year);
    }

    ps("\n");
    hola_report();
    hola_state();
    ps("\n=== Demo Complete ===\n");
    return 0;
}
