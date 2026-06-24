/* lathyrus_admin: Lathyrus sweet pea administration (v1.0)
 * Vine inventory, flower tracking, fragrance measurement, support structures
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);

#define N 16

/* Support types */
#define LATH_SUP_TRELLIS     1
#define LATH_SUP_ARBOR       2
#define LATH_SUP_FENCE       3
#define LATH_SUP_NET         4

/* Propagation methods */
#define LATH_PROP_SEED       1
#define LATH_PROP_CUTTING    2
#define LATH_PROP_LAYER      3

/* Fragrance levels */
#define LATH_FRAG_NONE       0
#define LATH_FRAG_LOW        1
#define LATH_FRAG_MED        2
#define LATH_FRAG_HIGH       3

typedef struct {
    int id;
    int plant_id;
    int age_weeks;
    int flower_count;
    int fragrance_level;
    int support_type;
    int deadhead_freq;
    int seed_count;
    int variety_id;
    int propagate_method;
    int active;
} lathyrus_t;

typedef struct {
    int n_plant;
    int n_flower;
    int n_fragrance;
    int n_support;
    int n_deadhead;
    int n_seed;
    int n_variety;
    int total_flower;
    int total_seed;
    int avg_fragrance;
} lathyrus_state_t;

static lathyrus_t lathyrus_plants[N];
static lathyrus_t lathyrus_flower_recs[N - 2];
static lathyrus_t lathyrus_fragrance_recs[N - 4];
static lathyrus_t lathyrus_support_recs[N - 6];
static lathyrus_t lathyrus_deadhead_recs[N - 6];
static lathyrus_t lathyrus_seed_recs[N - 8];
static lathyrus_t lathyrus_variety_recs[N - 8];
static lathyrus_state_t st;
static int init;

static void ps(const char *s) { host_print(s); }
static void pi(int v) {
    char b[32]; int i = 0;
    if (v < 0) { b[i++] = '-'; v = -v; }
    if (v == 0) { b[i++] = '0'; }
    else { int s = i; while (v > 0) { b[i++] = '0' + (v % 10); v /= 10; } int e = i - 1; while (s < e) { char t = b[s]; b[s] = b[e]; b[e] = t; s++; e--; } }
    b[i] = '\0'; host_print(b);
}

static int add_lathyrus(lathyrus_t *a, int *cnt, int *sum, int mx, int plant, int age, int flower, int frag, int support, int deadhead, int seed, int variety, int prop) {
    if (*cnt >= mx) return -1;
    lathyrus_t *x = &a[*cnt];
    x->id = *cnt; x->plant_id = plant; x->age_weeks = age;
    x->flower_count = flower; x->fragrance_level = frag; x->support_type = support;
    x->deadhead_freq = deadhead; x->seed_count = seed;
    x->variety_id = variety; x->propagate_method = prop; x->active = 1;
    *sum += flower; (*cnt)++;
    ps("[LATHYRUS] Record "); pi(*cnt - 1);
    ps(" plant="); pi(plant); ps(" age="); pi(age);
    ps(" flower="); pi(flower); ps(" frag="); pi(frag);
    ps(" support="); pi(support); ps(" seed="); pi(seed); ps("\n");
    return *cnt - 1;
}

int lathyrus_init(void) {
    if (init) return -1;
    st.n_plant = 0; st.n_flower = 0; st.n_fragrance = 0;
    st.n_support = 0; st.n_deadhead = 0; st.n_seed = 0;
    st.n_variety = 0; st.total_flower = 0; st.total_seed = 0;
    st.avg_fragrance = 0;
    for (int i = 0; i < N; i++) lathyrus_plants[i].active = 0;
    for (int i = 0; i < N - 2; i++) lathyrus_flower_recs[i].active = 0;
    for (int i = 0; i < N - 4; i++) lathyrus_fragrance_recs[i].active = 0;
    for (int i = 0; i < N - 6; i++) lathyrus_support_recs[i].active = 0;
    for (int i = 0; i < N - 6; i++) lathyrus_deadhead_recs[i].active = 0;
    for (int i = 0; i < N - 8; i++) lathyrus_seed_recs[i].active = 0;
    for (int i = 0; i < N - 8; i++) lathyrus_variety_recs[i].active = 0;
    init = 1;
    ps("[LATHYRUS] Lathyrus admin initialized\n");
    return 0;
}

int lathyrus_plant_inventory(int plant_id, int age_weeks) {
    return add_lathyrus(lathyrus_plants, &st.n_plant, &st.total_flower, N,
                        plant_id, age_weeks, 0, 0, 0, 0, 0, 0, 0);
}

int lathyrus_flower_track(int plant_id, int flower_count) {
    st.total_flower += flower_count;
    return add_lathyrus(lathyrus_flower_recs, &st.n_flower, &st.total_flower, N - 2,
                        plant_id, 0, flower_count, 0, 0, 0, 0, 0, 0);
}

int lathyrus_fragrance_measure(int plant_id, int fragrance_level) {
    st.avg_fragrance += fragrance_level;
    return add_lathyrus(lathyrus_fragrance_recs, &st.n_fragrance, &st.avg_fragrance, N - 4,
                        plant_id, 0, 0, fragrance_level, 0, 0, 0, 0, 0);
}

int lathyrus_support_install(int plant_id, int support_type) {
    return add_lathyrus(lathyrus_support_recs, &st.n_support, &st.n_support, N - 6,
                        plant_id, 0, 0, 0, support_type, 0, 0, 0, 0);
}

int lathyrus_deadhead_schedule(int plant_id, int deadhead_freq) {
    return add_lathyrus(lathyrus_deadhead_recs, &st.n_deadhead, &st.n_deadhead, N - 6,
                        plant_id, 0, 0, 0, 0, deadhead_freq, 0, 0, 0);
}

int lathyrus_seed_collect(int plant_id, int seed_count) {
    st.total_seed += seed_count;
    return add_lathyrus(lathyrus_seed_recs, &st.n_seed, &st.total_seed, N - 8,
                        plant_id, 0, 0, 0, 0, 0, seed_count, 0, 0);
}

int lathyrus_variety_catalog(int plant_id, int variety_id, int propagate_method) {
    return add_lathyrus(lathyrus_variety_recs, &st.n_variety, &st.n_variety, N - 8,
                        plant_id, 0, 0, 0, 0, 0, 0, variety_id, propagate_method);
}

void lathyrus_report(void) {
    ps("[LATHYRUS] Plants: "); pi(st.n_plant);
    ps(" Flower: "); pi(st.n_flower);
    ps(" Fragrance: "); pi(st.n_fragrance);
    ps(" Support: "); pi(st.n_support);
    ps(" Deadhead: "); pi(st.n_deadhead);
    ps(" Seed: "); pi(st.n_seed);
    ps(" Variety: "); pi(st.n_variety);
    ps("\n[LATHYRUS] Total flower: "); pi(st.total_flower); ps(" blooms");
    ps("\n[LATHYRUS] Total seed: "); pi(st.total_seed); ps(" seeds");
    ps("\n[LATHYRUS] Avg fragrance: "); pi(st.avg_fragrance); ps(" level");
    ps("\n");
}

void lathyrus_state(void) {
    ps("[LATHYRUS] Pl="); pi(st.n_plant);
    ps(" Flw="); pi(st.n_flower);
    ps(" Frq="); pi(st.n_fragrance);
    ps(" Sup="); pi(st.n_support);
    ps(" Ded="); pi(st.n_deadhead);
    ps(" Sd="); pi(st.n_seed);
    ps(" Var="); pi(st.n_variety);
    ps("\n");
}

int main(void) {
    ps("=== Lathyrus Sweet Pea Admin Demo ===\n\n");
    lathyrus_init();

    ps("Plant inventory...\n");
    for (int i = 0; i < N; i++) {
        int plant = 900 + i;
        int age = 4 + (i * 2) % 16;
        lathyrus_plant_inventory(plant, age);
    }

    ps("\nFlower tracking...\n");
    for (int i = 0; i < N - 2; i++) {
        int plant = 900 + i;
        int flower = 5 + (i * 3) % 15;
        lathyrus_flower_track(plant, flower);
    }

    ps("\nFragrance measurement...\n");
    for (int i = 0; i < N - 4; i++) {
        int plant = 900 + i;
        int frag = (i % 4);
        lathyrus_fragrance_measure(plant, frag);
    }

    ps("\nSupport structure installation...\n");
    for (int i = 0; i < N - 6; i++) {
        int plant = 900 + i;
        int support = (i % 4) + 1;
        lathyrus_support_install(plant, support);
    }

    ps("\nDeadheading schedule...\n");
    for (int i = 0; i < N - 6; i++) {
        int plant = 900 + i;
        int freq = 3 + (i % 7);
        lathyrus_deadhead_schedule(plant, freq);
    }

    ps("\nSeed collection...\n");
    for (int i = 0; i < N - 8; i++) {
        int plant = 900 + i;
        int seed = 10 + (i * 4) % 30;
        lathyrus_seed_collect(plant, seed);
    }

    ps("\nVariety catalog...\n");
    for (int i = 0; i < N - 8; i++) {
        int plant = 900 + i;
        int variety = 1 + (i % 12);
        int prop = (i % 3) + 1;
        lathyrus_variety_catalog(plant, variety, prop);
    }

    ps("\n");
    lathyrus_report();
    lathyrus_state();
    ps("\n=== Demo Complete ===\n");
    return 0;
}
