/* indigofera_admin: Indigofera Dye Plant administration (v1.0)
 * Field cultivation, leaf harvest, indican tracking, fermentation, dye production
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);

#define N 16

/* Textile types */
#define INDIGO_TEXTILE_COTTON    1
#define INDIGO_TEXTILE_SILK      2
#define INDIGO_TEXTILE_WOOL      3
#define INDIGO_TEXTILE_LINEN     4

/* Dye recipes */
#define INDIGO_RECIPE_TRADITIONAL 1
#define INDIGO_RECIPE_JAPANESE    2
#define INDIGO_RECIPE_INDIAN      3
#define INDIGO_RECIPE_MODERN      4

typedef struct {
    int id;
    int plot_id;
    int growth_day;
    int leaf_kg;
    int indican_ppm;
    int ferment_day;
    int dye_cake_g;
    int color_intensity;
    int textile_type;
    int recipe_id;
    int active;
} indigo_t;

typedef struct {
    int n_plot;
    int n_harvest;
    int n_indican;
    int n_ferment;
    int n_dye_cake;
    int n_color;
    int n_textile;
    int total_leaf;
    int total_indican;
    int total_dye_cake;
    int avg_color;
} indigo_state_t;

static indigo_t indigo_plots[N];
static indigo_t indigo_harvest_recs[N - 2];
static indigo_t indigo_indican_recs[N - 4];
static indigo_t indigo_ferment_recs[N - 6];
static indigo_t indigo_dye_cake_recs[N - 6];
static indigo_t indigo_color_recs[N - 8];
static indigo_t indigo_textile_recs[N - 8];
static indigo_state_t st;
static int init;

static void ps(const char *s) { host_print(s); }
static void pi(int v) {
    char b[32]; int i = 0;
    if (v < 0) { b[i++] = '-'; v = -v; }
    if (v == 0) { b[i++] = '0'; }
    else { int s = i; while (v > 0) { b[i++] = '0' + (v % 10); v /= 10; } int e = i - 1; while (s < e) { char t = b[s]; b[s] = b[e]; b[e] = t; s++; e--; } }
    b[i] = '\0'; host_print(b);
}

static int add_indigo(indigo_t *a, int *cnt, int *sum, int mx, int plot, int growth, int leaf, int indican, int ferment, int dye_cake, int color, int textile, int recipe) {
    if (*cnt >= mx) return -1;
    indigo_t *x = &a[*cnt];
    x->id = *cnt; x->plot_id = plot; x->growth_day = growth;
    x->leaf_kg = leaf; x->indican_ppm = indican; x->ferment_day = ferment;
    x->dye_cake_g = dye_cake; x->color_intensity = color;
    x->textile_type = textile; x->recipe_id = recipe; x->active = 1;
    *sum += leaf; (*cnt)++;
    ps("[INDIGO] Record "); pi(*cnt - 1);
    ps(" plot="); pi(plot); ps(" growth="); pi(growth);
    ps(" leaf="); pi(leaf); ps(" indican="); pi(indican);
    ps(" ferment="); pi(ferment); ps(" dye_cake="); pi(dye_cake); ps("\n");
    return *cnt - 1;
}

int indigo_init(void) {
    if (init) return -1;
    st.n_plot = 0; st.n_harvest = 0; st.n_indican = 0;
    st.n_ferment = 0; st.n_dye_cake = 0; st.n_color = 0;
    st.n_textile = 0; st.total_leaf = 0; st.total_indican = 0;
    st.total_dye_cake = 0; st.avg_color = 0;
    for (int i = 0; i < N; i++) indigo_plots[i].active = 0;
    for (int i = 0; i < N - 2; i++) indigo_harvest_recs[i].active = 0;
    for (int i = 0; i < N - 4; i++) indigo_indican_recs[i].active = 0;
    for (int i = 0; i < N - 6; i++) indigo_ferment_recs[i].active = 0;
    for (int i = 0; i < N - 6; i++) indigo_dye_cake_recs[i].active = 0;
    for (int i = 0; i < N - 8; i++) indigo_color_recs[i].active = 0;
    for (int i = 0; i < N - 8; i++) indigo_textile_recs[i].active = 0;
    init = 1;
    ps("[INDIGO] Indigofera admin initialized\n");
    return 0;
}

int indigo_plot_establish(int plot_id, int growth_day) {
    return add_indigo(indigo_plots, &st.n_plot, &st.total_leaf, N,
                      plot_id, growth_day, 0, 0, 0, 0, 0, 0, 0);
}

int indigo_leaf_harvest(int plot_id, int leaf_kg) {
    st.total_leaf += leaf_kg;
    return add_indigo(indigo_harvest_recs, &st.n_harvest, &st.total_leaf, N - 2,
                      plot_id, 0, leaf_kg, 0, 0, 0, 0, 0, 0);
}

int indigo_indican_track(int plot_id, int indican_ppm) {
    st.total_indican += indican_ppm;
    return add_indigo(indigo_indican_recs, &st.n_indican, &st.total_indican, N - 4,
                      plot_id, 0, 0, indican_ppm, 0, 0, 0, 0, 0);
}

int indigo_ferment(int plot_id, int ferment_day) {
    return add_indigo(indigo_ferment_recs, &st.n_ferment, &st.n_ferment, N - 6,
                      plot_id, 0, 0, 0, ferment_day, 0, 0, 0, 0);
}

int indigo_dye_cake_produce(int plot_id, int dye_cake_g) {
    st.total_dye_cake += dye_cake_g;
    return add_indigo(indigo_dye_cake_recs, &st.n_dye_cake, &st.total_dye_cake, N - 6,
                      plot_id, 0, 0, 0, 0, dye_cake_g, 0, 0, 0);
}

int indigo_color_measure(int plot_id, int color_intensity) {
    st.avg_color += color_intensity;
    return add_indigo(indigo_color_recs, &st.n_color, &st.avg_color, N - 8,
                      plot_id, 0, 0, 0, 0, 0, color_intensity, 0, 0);
}

int indigo_textile_apply(int plot_id, int textile_type, int recipe_id) {
    return add_indigo(indigo_textile_recs, &st.n_textile, &st.n_textile, N - 8,
                      plot_id, 0, 0, 0, 0, 0, 0, textile_type, recipe_id);
}

void indigo_report(void) {
    ps("[INDIGO] Plots: "); pi(st.n_plot);
    ps(" Harvest: "); pi(st.n_harvest);
    ps(" Indican: "); pi(st.n_indican);
    ps(" Ferment: "); pi(st.n_ferment);
    ps(" Dye cake: "); pi(st.n_dye_cake);
    ps(" Color: "); pi(st.n_color);
    ps(" Textile: "); pi(st.n_textile);
    ps("\n[INDIGO] Total leaf: "); pi(st.total_leaf); ps(" kg");
    ps("\n[INDIGO] Total indican: "); pi(st.total_indican); ps(" ppm");
    ps("\n[INDIGO] Total dye cake: "); pi(st.total_dye_cake); ps(" g");
    ps("\n[INDIGO] Avg color: "); pi(st.avg_color); ps(" intensity");
    ps("\n");
}

void indigo_state(void) {
    ps("[INDIGO] Pl="); pi(st.n_plot);
    ps(" Harv="); pi(st.n_harvest);
    ps(" Ind="); pi(st.n_indican);
    ps(" Frm="); pi(st.n_ferment);
    ps(" Dye="); pi(st.n_dye_cake);
    ps(" Clr="); pi(st.n_color);
    ps(" Txt="); pi(st.n_textile);
    ps("\n");
}

int main(void) {
    ps("=== Indigofera Dye Plant Admin Demo ===\n\n");
    indigo_init();

    ps("Plot establishment...\n");
    for (int i = 0; i < N; i++) {
        int plot = 400 + i;
        int growth = 30 + (i * 5);
        indigo_plot_establish(plot, growth);
    }

    ps("\nLeaf harvest cycles...\n");
    for (int i = 0; i < N - 2; i++) {
        int plot = 400 + i;
        int leaf = 4 + (i * 2) % 8;
        indigo_leaf_harvest(plot, leaf);
    }

    ps("\nIndican content tracking...\n");
    for (int i = 0; i < N - 4; i++) {
        int plot = 400 + i;
        int indican = 150 + (i * 19) % 250;
        indigo_indican_track(plot, indican);
    }

    ps("\nFermentation process...\n");
    for (int i = 0; i < N - 6; i++) {
        int plot = 400 + i;
        int ferment = 3 + (i % 5);
        indigo_ferment(plot, ferment);
    }

    ps("\nDye cake production...\n");
    for (int i = 0; i < N - 6; i++) {
        int plot = 400 + i;
        int dye_cake = 50 + (i * 15) % 100;
        indigo_dye_cake_produce(plot, dye_cake);
    }

    ps("\nColor intensity measurement...\n");
    for (int i = 0; i < N - 8; i++) {
        int plot = 400 + i;
        int color = 60 + (i * 7) % 40;
        indigo_color_measure(plot, color);
    }

    ps("\nTextile application...\n");
    for (int i = 0; i < N - 8; i++) {
        int plot = 400 + i;
        int textile = (i % 4) + 1;
        int recipe = (i % 4) + 1;
        indigo_textile_apply(plot, textile, recipe);
    }

    ps("\n");
    indigo_report();
    indigo_state();
    ps("\n=== Demo Complete ===\n");
    return 0;
}
