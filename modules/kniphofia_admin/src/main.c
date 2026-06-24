/* kniphofia_admin: Kniphofia red hot poker administration (v1.0)
 * Clump inventory, spike tracking, color catalog, pollinator records
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);

#define N 16

/* Color varieties */
#define KNIP_RED           1
#define KNIP_ORANGE        2
#define KNIP_YELLOW        3
#define KNIP_BICOLOR       4

/* Drought ratings */
#define KNIP_DROUGHT_LOW   1
#define KNIP_DROUGHT_MED   2
#define KNIP_DROUGHT_HIGH  3

/* Design zones */
#define KNIP_ZONE_BORDER   1
#define KNIP_ZONE_BED      2
#define KNIP_ZONE_ROCK     3
#define KNIP_ZONE_WILDFLOWER 4

typedef struct {
    int id;
    int clump_id;
    int age_years;
    int spike_count;
    int color_variety;
    int divide_year;
    int drought_rating;
    int pollinator_count;
    int design_zone;
    int cut_flower_kg;
    int active;
} kniphofia_t;

typedef struct {
    int n_clump;
    int n_spike;
    int n_color;
    int n_divide;
    int n_drought;
    int n_pollinator;
    int n_cut;
    int total_spike;
    int total_pollinator;
    int total_cut;
} kniphofia_state_t;

static kniphofia_t kniphofia_clumps[N];
static kniphofia_t kniphofia_spike_recs[N - 2];
static kniphofia_t kniphofia_color_recs[N - 4];
static kniphofia_t kniphofia_divide_recs[N - 6];
static kniphofia_t kniphofia_drought_recs[N - 6];
static kniphofia_t kniphofia_pollinator_recs[N - 8];
static kniphofia_t kniphofia_cut_recs[N - 8];
static kniphofia_state_t st;
static int init;

static void ps(const char *s) { host_print(s); }
static void pi(int v) {
    char b[32]; int i = 0;
    if (v < 0) { b[i++] = '-'; v = -v; }
    if (v == 0) { b[i++] = '0'; }
    else { int s = i; while (v > 0) { b[i++] = '0' + (v % 10); v /= 10; } int e = i - 1; while (s < e) { char t = b[s]; b[s] = b[e]; b[e] = t; s++; e--; } }
    b[i] = '\0'; host_print(b);
}

static int add_kniphofia(kniphofia_t *a, int *cnt, int *sum, int mx, int clump, int age, int spike, int color, int divide, int drought, int poll, int zone, int cut) {
    if (*cnt >= mx) return -1;
    kniphofia_t *x = &a[*cnt];
    x->id = *cnt; x->clump_id = clump; x->age_years = age;
    x->spike_count = spike; x->color_variety = color; x->divide_year = divide;
    x->drought_rating = drought; x->pollinator_count = poll;
    x->design_zone = zone; x->cut_flower_kg = cut; x->active = 1;
    *sum += spike; (*cnt)++;
    ps("[KNIPHOFIA] Record "); pi(*cnt - 1);
    ps(" clump="); pi(clump); ps(" age="); pi(age);
    ps(" spike="); pi(spike); ps(" color="); pi(color);
    ps(" drought="); pi(drought); ps(" poll="); pi(poll); ps("\n");
    return *cnt - 1;
}

int kniphofia_init(void) {
    if (init) return -1;
    st.n_clump = 0; st.n_spike = 0; st.n_color = 0;
    st.n_divide = 0; st.n_drought = 0; st.n_pollinator = 0;
    st.n_cut = 0; st.total_spike = 0; st.total_pollinator = 0;
    st.total_cut = 0;
    for (int i = 0; i < N; i++) kniphofia_clumps[i].active = 0;
    for (int i = 0; i < N - 2; i++) kniphofia_spike_recs[i].active = 0;
    for (int i = 0; i < N - 4; i++) kniphofia_color_recs[i].active = 0;
    for (int i = 0; i < N - 6; i++) kniphofia_divide_recs[i].active = 0;
    for (int i = 0; i < N - 6; i++) kniphofia_drought_recs[i].active = 0;
    for (int i = 0; i < N - 8; i++) kniphofia_pollinator_recs[i].active = 0;
    for (int i = 0; i < N - 8; i++) kniphofia_cut_recs[i].active = 0;
    init = 1;
    ps("[KNIPHOFIA] Kniphofia admin initialized\n");
    return 0;
}

int kniphofia_clump_inventory(int clump_id, int age_years) {
    return add_kniphofia(kniphofia_clumps, &st.n_clump, &st.total_spike, N,
                         clump_id, age_years, 0, 0, 0, 0, 0, 0, 0);
}

int kniphofia_spike_track(int clump_id, int spike_count) {
    st.total_spike += spike_count;
    return add_kniphofia(kniphofia_spike_recs, &st.n_spike, &st.total_spike, N - 2,
                         clump_id, 0, spike_count, 0, 0, 0, 0, 0, 0);
}

int kniphofia_color_catalog(int clump_id, int color_variety) {
    return add_kniphofia(kniphofia_color_recs, &st.n_color, &st.n_color, N - 4,
                         clump_id, 0, 0, color_variety, 0, 0, 0, 0, 0);
}

int kniphofia_divide_propagate(int clump_id, int divide_year) {
    return add_kniphofia(kniphofia_divide_recs, &st.n_divide, &st.n_divide, N - 6,
                         clump_id, 0, 0, 0, divide_year, 0, 0, 0, 0);
}

int kniphofia_drought_monitor(int clump_id, int drought_rating) {
    return add_kniphofia(kniphofia_drought_recs, &st.n_drought, &st.n_drought, N - 6,
                         clump_id, 0, 0, 0, 0, drought_rating, 0, 0, 0);
}

int kniphofia_pollinator_track(int clump_id, int pollinator_count) {
    st.total_pollinator += pollinator_count;
    return add_kniphofia(kniphofia_pollinator_recs, &st.n_pollinator, &st.total_pollinator, N - 8,
                         clump_id, 0, 0, 0, 0, 0, pollinator_count, 0, 0);
}

int kniphofia_cut_flower_harvest(int clump_id, int cut_flower_kg, int design_zone) {
    st.total_cut += cut_flower_kg;
    return add_kniphofia(kniphofia_cut_recs, &st.n_cut, &st.total_cut, N - 8,
                         clump_id, 0, 0, 0, 0, 0, 0, design_zone, cut_flower_kg);
}

void kniphofia_report(void) {
    ps("[KNIPHOFIA] Clumps: "); pi(st.n_clump);
    ps(" Spike: "); pi(st.n_spike);
    ps(" Color: "); pi(st.n_color);
    ps(" Divide: "); pi(st.n_divide);
    ps(" Drought: "); pi(st.n_drought);
    ps(" Pollinator: "); pi(st.n_pollinator);
    ps(" Cut: "); pi(st.n_cut);
    ps("\n[KNIPHOFIA] Total spike: "); pi(st.total_spike); ps(" spikes");
    ps("\n[KNIPHOFIA] Total pollinator: "); pi(st.total_pollinator); ps(" visits");
    ps("\n[KNIPHOFIA] Total cut: "); pi(st.total_cut); ps(" kg");
    ps("\n");
}

void kniphofia_state(void) {
    ps("[KNIPHOFIA] Cl="); pi(st.n_clump);
    ps(" Spk="); pi(st.n_spike);
    ps(" Col="); pi(st.n_color);
    ps(" Div="); pi(st.n_divide);
    ps(" Dro="); pi(st.n_drought);
    ps(" Pol="); pi(st.n_pollinator);
    ps(" Cut="); pi(st.n_cut);
    ps("\n");
}

int main(void) {
    ps("=== Kniphofia Red Hot Poker Admin Demo ===\n\n");
    kniphofia_init();

    ps("Clump inventory...\n");
    for (int i = 0; i < N; i++) {
        int clump = 700 + i;
        int age = 2 + (i % 8);
        kniphofia_clump_inventory(clump, age);
    }

    ps("\nSpike tracking...\n");
    for (int i = 0; i < N - 2; i++) {
        int clump = 700 + i;
        int spike = 3 + (i * 2) % 10;
        kniphofia_spike_track(clump, spike);
    }

    ps("\nColor variety catalog...\n");
    for (int i = 0; i < N - 4; i++) {
        int clump = 700 + i;
        int color = (i % 4) + 1;
        kniphofia_color_catalog(clump, color);
    }

    ps("\nDivision propagation...\n");
    for (int i = 0; i < N - 6; i++) {
        int clump = 700 + i;
        int year = 2024 + (i % 3);
        kniphofia_divide_propagate(clump, year);
    }

    ps("\nDrought monitoring...\n");
    for (int i = 0; i < N - 6; i++) {
        int clump = 700 + i;
        int drought = (i % 3) + 1;
        kniphofia_drought_monitor(clump, drought);
    }

    ps("\nPollinator tracking...\n");
    for (int i = 0; i < N - 8; i++) {
        int clump = 700 + i;
        int poll = 10 + (i * 5) % 30;
        kniphofia_pollinator_track(clump, poll);
    }

    ps("\nCut flower harvest...\n");
    for (int i = 0; i < N - 8; i++) {
        int clump = 700 + i;
        int cut = 1 + (i * 3) % 5;
        int zone = (i % 4) + 1;
        kniphofia_cut_flower_harvest(clump, cut, zone);
    }

    ps("\n");
    kniphofia_report();
    kniphofia_state();
    ps("\n=== Demo Complete ===\n");
    return 0;
}
