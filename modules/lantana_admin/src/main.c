/* lantana_admin: Lantana shrub verbena administration (v1.0)
 * Shrub inventory, flower cluster tracking, color catalog, invasive management
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);

#define N 16

/* Color varieties */
#define LANT_COLOR_RED_YELLOW 1
#define LANT_COLOR_PINK_RED   2
#define LANT_COLOR_WHITE_PINK 3
#define LANT_COLOR_PURPLE     4

/* Propagation types */
#define LANT_PROP_CUTTING     1
#define LANT_PROP_LAYER       2
#define LANT_PROP_SEED        3

/* Landscape zones */
#define LANT_ZONE_BORDER      1
#define LANT_ZONE_MASS        2
#define LANT_ZONE_CONTAINER   3
#define LANT_ZONE_WILDFLOWER  4

/* Invasive risk levels */
#define LANT_RISK_LOW         1
#define LANT_RISK_MED         2
#define LANT_RISK_HIGH        3

typedef struct {
    int id;
    int plant_id;
    int age_months;
    int cluster_count;
    int color_variety;
    int prune_month;
    int invasive_risk;
    int pollinator_count;
    int landscape_zone;
    int propagate_type;
    int active;
} lantana_t;

typedef struct {
    int n_plant;
    int n_cluster;
    int n_color;
    int n_prune;
    int n_invasive;
    int n_pollinator;
    int n_propagate;
    int total_cluster;
    int total_pollinator;
} lantana_state_t;

static lantana_t lantana_plants[N];
static lantana_t lantana_cluster_recs[N - 2];
static lantana_t lantana_color_recs[N - 4];
static lantana_t lantana_prune_recs[N - 6];
static lantana_t lantana_invasive_recs[N - 6];
static lantana_t lantana_pollinator_recs[N - 8];
static lantana_t lantana_propagate_recs[N - 8];
static lantana_state_t st;
static int init;

static void ps(const char *s) { host_print(s); }
static void pi(int v) {
    char b[32]; int i = 0;
    if (v < 0) { b[i++] = '-'; v = -v; }
    if (v == 0) { b[i++] = '0'; }
    else { int s = i; while (v > 0) { b[i++] = '0' + (v % 10); v /= 10; } int e = i - 1; while (s < e) { char t = b[s]; b[s] = b[e]; b[e] = t; s++; e--; } }
    b[i] = '\0'; host_print(b);
}

static int add_lantana(lantana_t *a, int *cnt, int *sum, int mx, int plant, int age, int cluster, int color, int prune, int risk, int poll, int zone, int prop) {
    if (*cnt >= mx) return -1;
    lantana_t *x = &a[*cnt];
    x->id = *cnt; x->plant_id = plant; x->age_months = age;
    x->cluster_count = cluster; x->color_variety = color; x->prune_month = prune;
    x->invasive_risk = risk; x->pollinator_count = poll;
    x->landscape_zone = zone; x->propagate_type = prop; x->active = 1;
    *sum += cluster; (*cnt)++;
    ps("[LANTANA] Record "); pi(*cnt - 1);
    ps(" plant="); pi(plant); ps(" age="); pi(age);
    ps(" cluster="); pi(cluster); ps(" color="); pi(color);
    ps(" risk="); pi(risk); ps(" poll="); pi(poll); ps("\n");
    return *cnt - 1;
}

int lantana_init(void) {
    if (init) return -1;
    st.n_plant = 0; st.n_cluster = 0; st.n_color = 0;
    st.n_prune = 0; st.n_invasive = 0; st.n_pollinator = 0;
    st.n_propagate = 0; st.total_cluster = 0; st.total_pollinator = 0;
    for (int i = 0; i < N; i++) lantana_plants[i].active = 0;
    for (int i = 0; i < N - 2; i++) lantana_cluster_recs[i].active = 0;
    for (int i = 0; i < N - 4; i++) lantana_color_recs[i].active = 0;
    for (int i = 0; i < N - 6; i++) lantana_prune_recs[i].active = 0;
    for (int i = 0; i < N - 6; i++) lantana_invasive_recs[i].active = 0;
    for (int i = 0; i < N - 8; i++) lantana_pollinator_recs[i].active = 0;
    for (int i = 0; i < N - 8; i++) lantana_propagate_recs[i].active = 0;
    init = 1;
    ps("[LANTANA] Lantana admin initialized\n");
    return 0;
}

int lantana_plant_inventory(int plant_id, int age_months) {
    return add_lantana(lantana_plants, &st.n_plant, &st.total_cluster, N,
                       plant_id, age_months, 0, 0, 0, 0, 0, 0, 0);
}

int lantana_cluster_track(int plant_id, int cluster_count) {
    st.total_cluster += cluster_count;
    return add_lantana(lantana_cluster_recs, &st.n_cluster, &st.total_cluster, N - 2,
                       plant_id, 0, cluster_count, 0, 0, 0, 0, 0, 0);
}

int lantana_color_catalog(int plant_id, int color_variety) {
    return add_lantana(lantana_color_recs, &st.n_color, &st.n_color, N - 4,
                       plant_id, 0, 0, color_variety, 0, 0, 0, 0, 0);
}

int lantana_prune_schedule(int plant_id, int prune_month) {
    return add_lantana(lantana_prune_recs, &st.n_prune, &st.n_prune, N - 6,
                       plant_id, 0, 0, 0, prune_month, 0, 0, 0, 0);
}

int lantana_invasive_manage(int plant_id, int invasive_risk) {
    return add_lantana(lantana_invasive_recs, &st.n_invasive, &st.n_invasive, N - 6,
                       plant_id, 0, 0, 0, 0, invasive_risk, 0, 0, 0);
}

int lantana_pollinator_track(int plant_id, int pollinator_count) {
    st.total_pollinator += pollinator_count;
    return add_lantana(lantana_pollinator_recs, &st.n_pollinator, &st.total_pollinator, N - 8,
                       plant_id, 0, 0, 0, 0, 0, pollinator_count, 0, 0);
}

int lantana_propagate(int plant_id, int propagate_type, int landscape_zone) {
    return add_lantana(lantana_propagate_recs, &st.n_propagate, &st.n_propagate, N - 8,
                       plant_id, 0, 0, 0, 0, 0, 0, landscape_zone, propagate_type);
}

void lantana_report(void) {
    ps("[LANTANA] Plants: "); pi(st.n_plant);
    ps(" Cluster: "); pi(st.n_cluster);
    ps(" Color: "); pi(st.n_color);
    ps(" Prune: "); pi(st.n_prune);
    ps(" Invasive: "); pi(st.n_invasive);
    ps(" Pollinator: "); pi(st.n_pollinator);
    ps(" Propagate: "); pi(st.n_propagate);
    ps("\n[LANTANA] Total cluster: "); pi(st.total_cluster); ps(" clusters");
    ps("\n[LANTANA] Total pollinator: "); pi(st.total_pollinator); ps(" visits");
    ps("\n");
}

void lantana_state(void) {
    ps("[LANTANA] Pl="); pi(st.n_plant);
    ps(" Cls="); pi(st.n_cluster);
    ps(" Col="); pi(st.n_color);
    ps(" Prn="); pi(st.n_prune);
    ps(" Inv="); pi(st.n_invasive);
    ps(" Pol="); pi(st.n_pollinator);
    ps(" Prp="); pi(st.n_propagate);
    ps("\n");
}

int main(void) {
    ps("=== Lantana Shrub Verbena Admin Demo ===\n\n");
    lantana_init();

    ps("Plant inventory...\n");
    for (int i = 0; i < N; i++) {
        int plant = 800 + i;
        int age = 6 + (i * 3) % 24;
        lantana_plant_inventory(plant, age);
    }

    ps("\nFlower cluster tracking...\n");
    for (int i = 0; i < N - 2; i++) {
        int plant = 800 + i;
        int cluster = 10 + (i * 5) % 30;
        lantana_cluster_track(plant, cluster);
    }

    ps("\nColor variety catalog...\n");
    for (int i = 0; i < N - 4; i++) {
        int plant = 800 + i;
        int color = (i % 4) + 1;
        lantana_color_catalog(plant, color);
    }

    ps("\nPruning schedule...\n");
    for (int i = 0; i < N - 6; i++) {
        int plant = 800 + i;
        int month = 3 + (i % 6);
        lantana_prune_schedule(plant, month);
    }

    ps("\nInvasive species management...\n");
    for (int i = 0; i < N - 6; i++) {
        int plant = 800 + i;
        int risk = (i % 3) + 1;
        lantana_invasive_manage(plant, risk);
    }

    ps("\nPollinator tracking...\n");
    for (int i = 0; i < N - 8; i++) {
        int plant = 800 + i;
        int poll = 20 + (i * 7) % 40;
        lantana_pollinator_track(plant, poll);
    }

    ps("\nPropagation records...\n");
    for (int i = 0; i < N - 8; i++) {
        int plant = 800 + i;
        int prop = (i % 3) + 1;
        int zone = (i % 4) + 1;
        lantana_propagate(plant, prop, zone);
    }

    ps("\n");
    lantana_report();
    lantana_state();
    ps("\n=== Demo Complete ===\n");
    return 0;
}
