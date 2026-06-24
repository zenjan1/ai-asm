/* justicia_admin: Justicia shrimp plant administration (v1.0)
 * Plant inventory, bract tracking, flower monitoring, propagation
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);

#define N 16

/* Bract colors */
#define JUST_BRACT_RED     1
#define JUST_BRACT_ORANGE  2
#define JUST_BRACT_PINK    3
#define JUST_BRACT_WHITE   4

/* Propagation types */
#define JUST_PROP_CUTTING  1
#define JUST_PROP_DIVISION 2
#define JUST_PROP_LAYER    3

/* Light levels */
#define JUST_LIGHT_LOW     1
#define JUST_LIGHT_MED     2
#define JUST_LIGHT_HIGH    3
#define JUST_LIGHT_FULL    4

/* Placement */
#define JUST_PLACE_INDOOR  1
#define JUST_PLACE_OUTDOOR 2
#define JUST_PLACE_GREENHOUSE 3

typedef struct {
    int id;
    int plant_id;
    int age_months;
    int bract_color;
    int flower_count;
    int propagate_type;
    int water_freq;
    int light_level;
    int placement;
    int care_month;
    int active;
} justicia_t;

typedef struct {
    int n_plant;
    int n_bract;
    int n_flower;
    int n_propagate;
    int n_water;
    int n_light;
    int n_care;
    int total_flower;
    int avg_age;
} justicia_state_t;

static justicia_t justicia_plants[N];
static justicia_t justicia_bract_recs[N - 2];
static justicia_t justicia_flower_recs[N - 4];
static justicia_t justicia_propagate_recs[N - 6];
static justicia_t justicia_water_recs[N - 6];
static justicia_t justicia_light_recs[N - 8];
static justicia_t justicia_care_recs[N - 8];
static justicia_state_t st;
static int init;

static void ps(const char *s) { host_print(s); }
static void pi(int v) {
    char b[32]; int i = 0;
    if (v < 0) { b[i++] = '-'; v = -v; }
    if (v == 0) { b[i++] = '0'; }
    else { int s = i; while (v > 0) { b[i++] = '0' + (v % 10); v /= 10; } int e = i - 1; while (s < e) { char t = b[s]; b[s] = b[e]; b[e] = t; s++; e--; } }
    b[i] = '\0'; host_print(b);
}

static int add_justicia(justicia_t *a, int *cnt, int *sum, int mx, int plant, int age, int bract, int flower, int prop, int water, int light, int place, int care) {
    if (*cnt >= mx) return -1;
    justicia_t *x = &a[*cnt];
    x->id = *cnt; x->plant_id = plant; x->age_months = age;
    x->bract_color = bract; x->flower_count = flower; x->propagate_type = prop;
    x->water_freq = water; x->light_level = light;
    x->placement = place; x->care_month = care; x->active = 1;
    *sum += flower; (*cnt)++;
    ps("[JUSTICIA] Record "); pi(*cnt - 1);
    ps(" plant="); pi(plant); ps(" age="); pi(age);
    ps(" bract="); pi(bract); ps(" flower="); pi(flower);
    ps(" prop="); pi(prop); ps(" light="); pi(light); ps("\n");
    return *cnt - 1;
}

int justicia_init(void) {
    if (init) return -1;
    st.n_plant = 0; st.n_bract = 0; st.n_flower = 0;
    st.n_propagate = 0; st.n_water = 0; st.n_light = 0;
    st.n_care = 0; st.total_flower = 0; st.avg_age = 0;
    for (int i = 0; i < N; i++) justicia_plants[i].active = 0;
    for (int i = 0; i < N - 2; i++) justicia_bract_recs[i].active = 0;
    for (int i = 0; i < N - 4; i++) justicia_flower_recs[i].active = 0;
    for (int i = 0; i < N - 6; i++) justicia_propagate_recs[i].active = 0;
    for (int i = 0; i < N - 6; i++) justicia_water_recs[i].active = 0;
    for (int i = 0; i < N - 8; i++) justicia_light_recs[i].active = 0;
    for (int i = 0; i < N - 8; i++) justicia_care_recs[i].active = 0;
    init = 1;
    ps("[JUSTICIA] Justicia admin initialized\n");
    return 0;
}

int justicia_plant_inventory(int plant_id, int age_months) {
    return add_justicia(justicia_plants, &st.n_plant, &st.avg_age, N,
                        plant_id, age_months, 0, 0, 0, 0, 0, 0, 0);
}

int justicia_bract_track(int plant_id, int bract_color) {
    return add_justicia(justicia_bract_recs, &st.n_bract, &st.n_bract, N - 2,
                        plant_id, 0, bract_color, 0, 0, 0, 0, 0, 0);
}

int justicia_flower_monitor(int plant_id, int flower_count) {
    st.total_flower += flower_count;
    return add_justicia(justicia_flower_recs, &st.n_flower, &st.total_flower, N - 4,
                        plant_id, 0, 0, flower_count, 0, 0, 0, 0, 0);
}

int justicia_propagate(int plant_id, int propagate_type) {
    return add_justicia(justicia_propagate_recs, &st.n_propagate, &st.n_propagate, N - 6,
                        plant_id, 0, 0, 0, propagate_type, 0, 0, 0, 0);
}

int justicia_water_schedule(int plant_id, int water_freq) {
    return add_justicia(justicia_water_recs, &st.n_water, &st.n_water, N - 6,
                        plant_id, 0, 0, 0, 0, water_freq, 0, 0, 0);
}

int justicia_light_track(int plant_id, int light_level) {
    return add_justicia(justicia_light_recs, &st.n_light, &st.n_light, N - 8,
                        plant_id, 0, 0, 0, 0, 0, light_level, 0, 0);
}

int justicia_seasonal_care(int plant_id, int care_month, int placement) {
    return add_justicia(justicia_care_recs, &st.n_care, &st.n_care, N - 8,
                        plant_id, 0, 0, 0, 0, 0, 0, placement, care_month);
}

void justicia_report(void) {
    ps("[JUSTICIA] Plants: "); pi(st.n_plant);
    ps(" Bract: "); pi(st.n_bract);
    ps(" Flower: "); pi(st.n_flower);
    ps(" Propagate: "); pi(st.n_propagate);
    ps(" Water: "); pi(st.n_water);
    ps(" Light: "); pi(st.n_light);
    ps(" Care: "); pi(st.n_care);
    ps("\n[JUSTICIA] Total flower: "); pi(st.total_flower); ps(" blooms");
    ps("\n[JUSTICIA] Avg age: "); pi(st.avg_age); ps(" months");
    ps("\n");
}

void justicia_state(void) {
    ps("[JUSTICIA] Pl="); pi(st.n_plant);
    ps(" Brc="); pi(st.n_bract);
    ps(" Flw="); pi(st.n_flower);
    ps(" Prp="); pi(st.n_propagate);
    ps(" Wtr="); pi(st.n_water);
    ps(" Lgt="); pi(st.n_light);
    ps(" Cre="); pi(st.n_care);
    ps("\n");
}

int main(void) {
    ps("=== Justicia Shrimp Plant Admin Demo ===\n\n");
    justicia_init();

    ps("Plant inventory...\n");
    for (int i = 0; i < N; i++) {
        int plant = 600 + i;
        int age = 6 + (i * 3) % 24;
        justicia_plant_inventory(plant, age);
    }

    ps("\nBract color tracking...\n");
    for (int i = 0; i < N - 2; i++) {
        int plant = 600 + i;
        int bract = (i % 4) + 1;
        justicia_bract_track(plant, bract);
    }

    ps("\nFlower monitoring...\n");
    for (int i = 0; i < N - 4; i++) {
        int plant = 600 + i;
        int flower = 5 + (i * 2) % 15;
        justicia_flower_monitor(plant, flower);
    }

    ps("\nPropagation records...\n");
    for (int i = 0; i < N - 6; i++) {
        int plant = 600 + i;
        int prop = (i % 3) + 1;
        justicia_propagate(plant, prop);
    }

    ps("\nWatering schedule...\n");
    for (int i = 0; i < N - 6; i++) {
        int plant = 600 + i;
        int water = 2 + (i % 5);
        justicia_water_schedule(plant, water);
    }

    ps("\nLight exposure tracking...\n");
    for (int i = 0; i < N - 8; i++) {
        int plant = 600 + i;
        int light = (i % 4) + 1;
        justicia_light_track(plant, light);
    }

    ps("\nSeasonal care calendar...\n");
    for (int i = 0; i < N - 8; i++) {
        int plant = 600 + i;
        int care = 1 + (i % 12);
        int place = (i % 3) + 1;
        justicia_seasonal_care(plant, care, place);
    }

    ps("\n");
    justicia_report();
    justicia_state();
    ps("\n=== Demo Complete ===\n");
    return 0;
}
