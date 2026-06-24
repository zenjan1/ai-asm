/* jacaranda_admin: Jacaranda flowering tree administration (v1.0)
 * Tree inventory, growth tracking, bloom monitoring, seed collection
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);

#define N 16

/* Pest levels */
#define JACA_PEST_NONE     0
#define JACA_PEST_LOW      1
#define JACA_PEST_MED      2
#define JACA_PEST_HIGH     3

/* Landscape zones */
#define JACA_ZONE_STREET   1
#define JACA_ZONE_PARK     2
#define JACA_ZONE_GARDEN   3
#define JACA_ZONE_AVENUE   4

/* Wood quality grades */
#define JACA_WOOD_STANDARD 1
#define JACA_WOOD_SELECT   2
#define JACA_WOOD_PREMIUM  3

typedef struct {
    int id;
    int tree_id;
    int age_years;
    int height_m;
    int bloom_count;
    int seed_count;
    int prune_month;
    int pest_level;
    int landscape_zone;
    int wood_quality;
    int active;
} jacaranda_t;

typedef struct {
    int n_tree;
    int n_growth;
    int n_bloom;
    int n_seed;
    int n_prune;
    int n_pest;
    int n_landscape;
    int total_height;
    int total_bloom;
    int total_seed;
} jacaranda_state_t;

static jacaranda_t jacaranda_trees[N];
static jacaranda_t jacaranda_growth_recs[N - 2];
static jacaranda_t jacaranda_bloom_recs[N - 4];
static jacaranda_t jacaranda_seed_recs[N - 6];
static jacaranda_t jacaranda_prune_recs[N - 6];
static jacaranda_t jacaranda_pest_recs[N - 8];
static jacaranda_t jacaranda_landscape_recs[N - 8];
static jacaranda_state_t st;
static int init;

static void ps(const char *s) { host_print(s); }
static void pi(int v) {
    char b[32]; int i = 0;
    if (v < 0) { b[i++] = '-'; v = -v; }
    if (v == 0) { b[i++] = '0'; }
    else { int s = i; while (v > 0) { b[i++] = '0' + (v % 10); v /= 10; } int e = i - 1; while (s < e) { char t = b[s]; b[s] = b[e]; b[e] = t; s++; e--; } }
    b[i] = '\0'; host_print(b);
}

static int add_jacaranda(jacaranda_t *a, int *cnt, int *sum, int mx, int tree, int age, int height, int bloom, int seed, int prune, int pest, int zone, int wood) {
    if (*cnt >= mx) return -1;
    jacaranda_t *x = &a[*cnt];
    x->id = *cnt; x->tree_id = tree; x->age_years = age;
    x->height_m = height; x->bloom_count = bloom; x->seed_count = seed;
    x->prune_month = prune; x->pest_level = pest;
    x->landscape_zone = zone; x->wood_quality = wood; x->active = 1;
    *sum += height; (*cnt)++;
    ps("[JACARANDA] Record "); pi(*cnt - 1);
    ps(" tree="); pi(tree); ps(" age="); pi(age);
    ps(" height="); pi(height); ps(" bloom="); pi(bloom);
    ps(" seed="); pi(seed); ps(" zone="); pi(zone); ps("\n");
    return *cnt - 1;
}

int jacaranda_init(void) {
    if (init) return -1;
    st.n_tree = 0; st.n_growth = 0; st.n_bloom = 0;
    st.n_seed = 0; st.n_prune = 0; st.n_pest = 0;
    st.n_landscape = 0; st.total_height = 0; st.total_bloom = 0;
    st.total_seed = 0;
    for (int i = 0; i < N; i++) jacaranda_trees[i].active = 0;
    for (int i = 0; i < N - 2; i++) jacaranda_growth_recs[i].active = 0;
    for (int i = 0; i < N - 4; i++) jacaranda_bloom_recs[i].active = 0;
    for (int i = 0; i < N - 6; i++) jacaranda_seed_recs[i].active = 0;
    for (int i = 0; i < N - 6; i++) jacaranda_prune_recs[i].active = 0;
    for (int i = 0; i < N - 8; i++) jacaranda_pest_recs[i].active = 0;
    for (int i = 0; i < N - 8; i++) jacaranda_landscape_recs[i].active = 0;
    init = 1;
    ps("[JACARANDA] Jacaranda admin initialized\n");
    return 0;
}

int jacaranda_tree_inventory(int tree_id, int age_years) {
    return add_jacaranda(jacaranda_trees, &st.n_tree, &st.total_height, N,
                         tree_id, age_years, 0, 0, 0, 0, 0, 0, 0);
}

int jacaranda_growth_track(int tree_id, int height_m) {
    st.total_height += height_m;
    return add_jacaranda(jacaranda_growth_recs, &st.n_growth, &st.total_height, N - 2,
                         tree_id, 0, height_m, 0, 0, 0, 0, 0, 0);
}

int jacaranda_bloom_monitor(int tree_id, int bloom_count) {
    st.total_bloom += bloom_count;
    return add_jacaranda(jacaranda_bloom_recs, &st.n_bloom, &st.total_bloom, N - 4,
                         tree_id, 0, 0, bloom_count, 0, 0, 0, 0, 0);
}

int jacaranda_seed_collect(int tree_id, int seed_count) {
    st.total_seed += seed_count;
    return add_jacaranda(jacaranda_seed_recs, &st.n_seed, &st.total_seed, N - 6,
                         tree_id, 0, 0, 0, seed_count, 0, 0, 0, 0);
}

int jacaranda_prune_schedule(int tree_id, int prune_month) {
    return add_jacaranda(jacaranda_prune_recs, &st.n_prune, &st.n_prune, N - 6,
                         tree_id, 0, 0, 0, 0, prune_month, 0, 0, 0);
}

int jacaranda_pest_manage(int tree_id, int pest_level) {
    return add_jacaranda(jacaranda_pest_recs, &st.n_pest, &st.n_pest, N - 8,
                         tree_id, 0, 0, 0, 0, 0, pest_level, 0, 0);
}

int jacaranda_landscape_plant(int tree_id, int zone, int wood_quality) {
    return add_jacaranda(jacaranda_landscape_recs, &st.n_landscape, &st.n_landscape, N - 8,
                         tree_id, 0, 0, 0, 0, 0, 0, zone, wood_quality);
}

void jacaranda_report(void) {
    ps("[JACARANDA] Trees: "); pi(st.n_tree);
    ps(" Growth: "); pi(st.n_growth);
    ps(" Bloom: "); pi(st.n_bloom);
    ps(" Seed: "); pi(st.n_seed);
    ps(" Prune: "); pi(st.n_prune);
    ps(" Pest: "); pi(st.n_pest);
    ps(" Landscape: "); pi(st.n_landscape);
    ps("\n[JACARANDA] Total height: "); pi(st.total_height); ps(" m");
    ps("\n[JACARANDA] Total bloom: "); pi(st.total_bloom); ps(" flowers");
    ps("\n[JACARANDA] Total seed: "); pi(st.total_seed); ps(" seeds");
    ps("\n");
}

void jacaranda_state(void) {
    ps("[JACARANDA] Tr="); pi(st.n_tree);
    ps(" Grw="); pi(st.n_growth);
    ps(" Blm="); pi(st.n_bloom);
    ps(" Sd="); pi(st.n_seed);
    ps(" Prn="); pi(st.n_prune);
    ps(" Pst="); pi(st.n_pest);
    ps(" Lnd="); pi(st.n_landscape);
    ps("\n");
}

int main(void) {
    ps("=== Jacaranda Flowering Tree Admin Demo ===\n\n");
    jacaranda_init();

    ps("Tree inventory...\n");
    for (int i = 0; i < N; i++) {
        int tree = 500 + i;
        int age = 3 + (i % 10);
        jacaranda_tree_inventory(tree, age);
    }

    ps("\nGrowth tracking...\n");
    for (int i = 0; i < N - 2; i++) {
        int tree = 500 + i;
        int height = 2 + (i * 3) % 8;
        jacaranda_growth_track(tree, height);
    }

    ps("\nBloom monitoring...\n");
    for (int i = 0; i < N - 4; i++) {
        int tree = 500 + i;
        int bloom = 50 + (i * 17) % 100;
        jacaranda_bloom_monitor(tree, bloom);
    }

    ps("\nSeed collection...\n");
    for (int i = 0; i < N - 6; i++) {
        int tree = 500 + i;
        int seed = 20 + (i * 7) % 40;
        jacaranda_seed_collect(tree, seed);
    }

    ps("\nPruning schedule...\n");
    for (int i = 0; i < N - 6; i++) {
        int tree = 500 + i;
        int month = 1 + (i % 12);
        jacaranda_prune_schedule(tree, month);
    }

    ps("\nPest management...\n");
    for (int i = 0; i < N - 8; i++) {
        int tree = 500 + i;
        int pest = i % 4;
        jacaranda_pest_manage(tree, pest);
    }

    ps("\nLandscape planting...\n");
    for (int i = 0; i < N - 8; i++) {
        int tree = 500 + i;
        int zone = (i % 4) + 1;
        int wood = (i % 3) + 1;
        jacaranda_landscape_plant(tree, zone, wood);
    }

    ps("\n");
    jacaranda_report();
    jacaranda_state();
    ps("\n=== Demo Complete ===\n");
    return 0;
}
