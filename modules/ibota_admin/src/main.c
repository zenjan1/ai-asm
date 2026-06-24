/* ibota_admin: Ibota Wax Privet medicinal & ornamental administration (v1.0)
 * Shrub inventory, leaf harvest, wax production, medicinal extracts
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);

#define N 16

/* Propagation types */
#define IBOTA_PROP_SEED     1
#define IBOTA_PROP_CUTTING  2
#define IBOTA_PROP_LAYER    3
#define IBOTA_PROP_DIVISION 4

/* Landscape uses */
#define IBOTA_LAND_HEDGE    1
#define IBOTA_LAND_SCREEN   2
#define IBOTA_LAND_SPECIMEN 3
#define IBOTA_LAND_BONSAI   4

/* Pest levels */
#define IBOTA_PEST_NONE     0
#define IBOTA_PEST_LOW      1
#define IBOTA_PEST_MED      2
#define IBOTA_PEST_HIGH     3

typedef struct {
    int id;
    int plant_id;
    int age_years;
    int leaf_kg;
    int wax_kg;
    int extract_conc;
    int prune_month;
    int pest_level;
    int propagate_type;
    int landscape_use;
    int active;
} ibota_t;

typedef struct {
    int n_plant;
    int n_harvest;
    int n_wax;
    int n_medicinal;
    int n_prune;
    int n_pest;
    int n_propagate;
    int total_leaf;
    int total_wax;
    int total_extract;
} ibota_state_t;

static ibota_t ibota_plants[N];
static ibota_t ibota_harvest_recs[N - 2];
static ibota_t ibota_wax_recs[N - 4];
static ibota_t ibota_medicinal_recs[N - 6];
static ibota_t ibota_prune_recs[N - 6];
static ibota_t ibota_pest_recs[N - 8];
static ibota_t ibota_propagate_recs[N - 8];
static ibota_state_t st;
static int init;

static void ps(const char *s) { host_print(s); }
static void pi(int v) {
    char b[32]; int i = 0;
    if (v < 0) { b[i++] = '-'; v = -v; }
    if (v == 0) { b[i++] = '0'; }
    else { int s = i; while (v > 0) { b[i++] = '0' + (v % 10); v /= 10; } int e = i - 1; while (s < e) { char t = b[s]; b[s] = b[e]; b[e] = t; s++; e--; } }
    b[i] = '\0'; host_print(b);
}

static int add_ibota(ibota_t *a, int *cnt, int *sum, int mx, int plant, int age, int leaf, int wax, int extract, int prune, int pest, int prop, int land) {
    if (*cnt >= mx) return -1;
    ibota_t *x = &a[*cnt];
    x->id = *cnt; x->plant_id = plant; x->age_years = age;
    x->leaf_kg = leaf; x->wax_kg = wax; x->extract_conc = extract;
    x->prune_month = prune; x->pest_level = pest;
    x->propagate_type = prop; x->landscape_use = land; x->active = 1;
    *sum += leaf; (*cnt)++;
    ps("[IBOTA] Record "); pi(*cnt - 1);
    ps(" plant="); pi(plant); ps(" age="); pi(age);
    ps(" leaf="); pi(leaf); ps(" wax="); pi(wax);
    ps(" extract="); pi(extract); ps(" prune="); pi(prune); ps("\n");
    return *cnt - 1;
}

int ibota_init(void) {
    if (init) return -1;
    st.n_plant = 0; st.n_harvest = 0; st.n_wax = 0;
    st.n_medicinal = 0; st.n_prune = 0; st.n_pest = 0;
    st.n_propagate = 0; st.total_leaf = 0; st.total_wax = 0;
    st.total_extract = 0;
    for (int i = 0; i < N; i++) ibota_plants[i].active = 0;
    for (int i = 0; i < N - 2; i++) ibota_harvest_recs[i].active = 0;
    for (int i = 0; i < N - 4; i++) ibota_wax_recs[i].active = 0;
    for (int i = 0; i < N - 6; i++) ibota_medicinal_recs[i].active = 0;
    for (int i = 0; i < N - 6; i++) ibota_prune_recs[i].active = 0;
    for (int i = 0; i < N - 8; i++) ibota_pest_recs[i].active = 0;
    for (int i = 0; i < N - 8; i++) ibota_propagate_recs[i].active = 0;
    init = 1;
    ps("[IBOTA] Ibota admin initialized\n");
    return 0;
}

int ibota_plant_inventory(int plant_id, int age_years) {
    return add_ibota(ibota_plants, &st.n_plant, &st.total_leaf, N,
                     plant_id, age_years, 0, 0, 0, 0, 0, 0, 0);
}

int ibota_leaf_harvest(int plant_id, int leaf_kg) {
    st.total_leaf += leaf_kg;
    return add_ibota(ibota_harvest_recs, &st.n_harvest, &st.total_leaf, N - 2,
                     plant_id, 0, leaf_kg, 0, 0, 0, 0, 0, 0);
}

int ibota_wax_extract(int plant_id, int wax_kg) {
    st.total_wax += wax_kg;
    return add_ibota(ibota_wax_recs, &st.n_wax, &st.total_wax, N - 4,
                     plant_id, 0, 0, wax_kg, 0, 0, 0, 0, 0);
}

int ibota_medicinal_prep(int plant_id, int extract_conc) {
    st.total_extract += extract_conc;
    return add_ibota(ibota_medicinal_recs, &st.n_medicinal, &st.total_extract, N - 6,
                     plant_id, 0, 0, 0, extract_conc, 0, 0, 0, 0);
}

int ibota_prune_schedule(int plant_id, int prune_month) {
    return add_ibota(ibota_prune_recs, &st.n_prune, &st.n_prune, N - 6,
                     plant_id, 0, 0, 0, 0, prune_month, 0, 0, 0);
}

int ibota_pest_manage(int plant_id, int pest_level) {
    return add_ibota(ibota_pest_recs, &st.n_pest, &st.n_pest, N - 8,
                     plant_id, 0, 0, 0, 0, 0, pest_level, 0, 0);
}

int ibota_propagate(int plant_id, int prop_type, int landscape_use) {
    return add_ibota(ibota_propagate_recs, &st.n_propagate, &st.n_propagate, N - 8,
                     plant_id, 0, 0, 0, 0, 0, 0, prop_type, landscape_use);
}

void ibota_report(void) {
    ps("[IBOTA] Plants: "); pi(st.n_plant);
    ps(" Harvest: "); pi(st.n_harvest);
    ps(" Wax: "); pi(st.n_wax);
    ps(" Med: "); pi(st.n_medicinal);
    ps(" Prune: "); pi(st.n_prune);
    ps(" Pest: "); pi(st.n_pest);
    ps(" Prop: "); pi(st.n_propagate);
    ps("\n[IBOTA] Total leaf: "); pi(st.total_leaf); ps(" kg");
    ps("\n[IBOTA] Total wax: "); pi(st.total_wax); ps(" kg");
    ps("\n[IBOTA] Total extract: "); pi(st.total_extract); ps(" units");
    ps("\n");
}

void ibota_state(void) {
    ps("[IBOTA] Pl="); pi(st.n_plant);
    ps(" Harv="); pi(st.n_harvest);
    ps(" Wax="); pi(st.n_wax);
    ps(" Med="); pi(st.n_medicinal);
    ps(" Prn="); pi(st.n_prune);
    ps(" Pst="); pi(st.n_pest);
    ps(" Prp="); pi(st.n_propagate);
    ps("\n");
}

int main(void) {
    ps("=== Ibota Wax Privet Admin Demo ===\n\n");
    ibota_init();

    ps("Plant inventory...\n");
    for (int i = 0; i < N; i++) {
        int plant = 200 + i;
        int age = 2 + (i % 8);
        ibota_plant_inventory(plant, age);
    }

    ps("\nLeaf harvest cycles...\n");
    for (int i = 0; i < N - 2; i++) {
        int plant = 200 + i;
        int leaf = 3 + (i * 2) % 8;
        ibota_leaf_harvest(plant, leaf);
    }

    ps("\nWax extraction...\n");
    for (int i = 0; i < N - 4; i++) {
        int plant = 200 + i;
        int wax = 1 + (i * 3) % 5;
        ibota_wax_extract(plant, wax);
    }

    ps("\nMedicinal preparation...\n");
    for (int i = 0; i < N - 6; i++) {
        int plant = 200 + i;
        int extract = 10 + (i * 5);
        ibota_medicinal_prep(plant, extract);
    }

    ps("\nPruning schedule...\n");
    for (int i = 0; i < N - 6; i++) {
        int plant = 200 + i;
        int month = 3 + (i % 6);
        ibota_prune_schedule(plant, month);
    }

    ps("\nPest management...\n");
    for (int i = 0; i < N - 8; i++) {
        int plant = 200 + i;
        int pest = i % 4;
        ibota_pest_manage(plant, pest);
    }

    ps("\nPropagation records...\n");
    for (int i = 0; i < N - 8; i++) {
        int plant = 200 + i;
        int prop = (i % 4) + 1;
        int land = (i % 4) + 1;
        ibota_propagate(plant, prop, land);
    }

    ps("\n");
    ibota_report();
    ibota_state();
    ps("\n=== Demo Complete ===\n");
    return 0;
}
