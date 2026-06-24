/* goodyera_admin: Goodyera jewel orchid conservation administration (v1.0)
 * Goodyera species catalog, propagation, environment control, compound analysis, export certification
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);

#define N 16

/* Propagation methods */
#define GOO_PROP_SEEDLING     1
#define GOO_PROP_MICROPROP    2
#define GOO_PROP_DIVISION     3

/* Conservation status (IUCN) */
#define GOO_CONS_LC   1   /* Least Concern */
#define GOO_CONS_NT   2   /* Near Threatened */
#define GOO_CONS_VU   3   /* Vulnerable */
#define GOO_CONS_EN   4   /* Endangered */
#define GOO_CONS_CR   5   /* Critically Endangered */

typedef struct {
    int id;
    int species;       /* species ID 1-12 */
    int prop_method;   /* propagation method */
    int humidity;      /* humidity % */
    int temp;          /* temperature x10 (C) */
    int shade_pct;     /* shade percentage */
    int compound_ppm;  /* goodyosin content ppm */
    int cons_status;   /* IUCN conservation status */
    int export_ok;     /* export compliance flag */
    int active;
} goo_t;

typedef struct {
    int n_species;     /* registered species */
    int n_prop;        /* propagation batches */
    int n_env;         /* environment records */
    int n_compound;    /* compound analyses */
    int n_export;      /* export certifications */
    int t_seedlings;   /* total seedlings produced */
    int t_compound;    /* total compound yield */
    int t_export;      /* total export value */
    int endangered;    /* endangered species count */
    int alerts;        /* environment alerts */
} goo_state_t;

static goo_t goo_species[N];
static goo_t goo_props[N - 2];
static goo_t goo_envs[N - 4];
static goo_t goo_compounds[N - 6];
static goo_t goo_exports[N - 6];
static goo_state_t st;
static int init;

static void ps(const char *s) { host_print(s); }
static void pi(int v) {
    char b[32]; int i = 0;
    if (v < 0) { b[i++] = '-'; v = -v; }
    if (v == 0) { b[i++] = '0'; }
    else { int s = i; while (v > 0) { b[i++] = '0' + (v % 10); v /= 10; } int e = i - 1; while (s < e) { char t = b[s]; b[s] = b[e]; b[e] = t; s++; e--; } }
    b[i] = '\0'; host_print(b);
}

static int add(goo_t *a, int *cnt, int *sum, int mx, int species, int prop, int humidity, int temp, int shade, int compound, int status, int export_ok) {
    if (*cnt >= mx) return -1;
    goo_t *x = &a[*cnt];
    x->id = *cnt; x->species = species; x->prop_method = prop;
    x->humidity = humidity; x->temp = temp; x->shade_pct = shade;
    x->compound_ppm = compound; x->cons_status = status;
    x->export_ok = export_ok; x->active = 1;
    *sum += compound; (*cnt)++;
    ps("[GOO] Record "); pi(*cnt - 1);
    ps(" sp="); pi(species); ps(" prop="); pi(prop);
    ps(" hum="); pi(humidity); ps(" temp="); pi(temp);
    ps(" shade="); pi(shade); ps(" cmpd="); pi(compound); ps("\n");
    return *cnt - 1;
}

int goo_init(void) {
    if (init) return -1;
    st.n_species = 0; st.n_prop = 0; st.n_env = 0;
    st.n_compound = 0; st.n_export = 0; st.t_seedlings = 0;
    st.t_compound = 0; st.t_export = 0; st.endangered = 0;
    st.alerts = 0;
    for (int i = 0; i < N; i++) goo_species[i].active = 0;
    for (int i = 0; i < N - 2; i++) goo_props[i].active = 0;
    for (int i = 0; i < N - 4; i++) goo_envs[i].active = 0;
    for (int i = 0; i < N - 6; i++) goo_compounds[i].active = 0;
    for (int i = 0; i < N - 6; i++) goo_exports[i].active = 0;
    init = 1;
    ps("[GOO] Goodyera jewel orchid admin initialized\n");
    return 0;
}

int goo_species_register(int species, int cons_status, int humidity, int temp, int shade) {
    if (cons_status >= GOO_CONS_VU) st.endangered++;
    return add(goo_species, &st.n_species, &st.t_compound, N,
               species, 0, humidity, temp, shade, 0, cons_status, 0);
}

int goo_propagate(int species, int prop_method, int count) {
    st.t_seedlings += count;
    return add(goo_props, &st.n_prop, &st.t_seedlings, N - 2,
               species, prop_method, 70, 220, 60, 0, 0, 0);
}

int goo_environment_control(int species, int humidity, int temp, int shade) {
    /* Alert if out of optimal range */
    if (humidity < 60 || humidity > 85) st.alerts++;
    if (temp < 180 || temp > 280) st.alerts++;  /* 18-28C */
    return add(goo_envs, &st.n_env, &st.alerts, N - 4,
               species, 0, humidity, temp, shade, 0, 0, 0);
}

int goo_compound_analyze(int species, int compound_ppm) {
    st.t_compound += compound_ppm;
    return add(goo_compounds, &st.n_compound, &st.t_compound, N - 6,
               species, 0, 0, 0, 0, compound_ppm, 0, 0);
}

int goo_export_certify(int species, int qty, int value, int cons_status) {
    /* Only LC/NT species can be exported */
    int export_ok = (cons_status <= GOO_CONS_NT) ? 1 : 0;
    if (export_ok) st.t_export += value;
    return add(goo_exports, &st.n_export, &st.t_export, N - 6,
               species, 0, 0, 0, 0, qty, cons_status, export_ok);
}

void goo_report(void) {
    ps("[GOO] Species: "); pi(st.n_species);
    ps(" Prop: "); pi(st.n_prop);
    ps(" Env: "); pi(st.n_env);
    ps(" Compound: "); pi(st.n_compound);
    ps(" Export: "); pi(st.n_export);
    ps("\n[GOO] Total seedlings: "); pi(st.t_seedlings);
    ps("\n[GOO] Total compound: "); pi(st.t_compound); ps(" ppm");
    ps("\n[GOO] Total export: "); pi(st.t_export); ps(" USD");
    ps("\n[GOO] Endangered: "); pi(st.endangered);
    ps(" Alerts: "); pi(st.alerts);
    ps("\n");
}

void goo_state(void) {
    ps("[GOO] Sp="); pi(st.n_species);
    ps(" Prop="); pi(st.n_prop);
    ps(" Env="); pi(st.n_env);
    ps(" Cmpd="); pi(st.n_compound);
    ps(" Exp="); pi(st.n_export);
    ps("\n");
}

int main(void) {
    ps("=== Goodyera Jewel Orchid Admin Demo ===\n\n");
    goo_init();

    ps("Species registration...\n");
    for (int i = 0; i < N; i++) {
        int species = (i % 8) + 1;
        int cons = (i % 5) + 1;           /* LC to CR */
        int humidity = 65 + (i * 2) % 20;
        int temp = 200 + (i * 5) % 80;    /* 20-28C x10 */
        int shade = 50 + (i * 3) % 30;
        goo_species_register(species, cons, humidity, temp, shade);
    }

    ps("\nPropagation batches...\n");
    for (int i = 0; i < N - 2; i++) {
        int species = (i % 8) + 1;
        int method = (i % 3) + 1;          /* seedling/microprop/division */
        int count = 20 + (i * 7);
        goo_propagate(species, method, count);
    }

    ps("\nEnvironment control cycles...\n");
    for (int i = 0; i < N - 4; i++) {
        int species = (i % 8) + 1;
        int humidity = 60 + (i * 3) % 30;
        int temp = 180 + (i * 7) % 100;   /* 18-28C x10 */
        int shade = 55 + (i * 4) % 25;
        goo_environment_control(species, humidity, temp, shade);
    }

    ps("\nCompound analysis...\n");
    for (int i = 0; i < N - 6; i++) {
        int species = (i % 8) + 1;
        int compound = 50 + (i * 17) % 200;
        goo_compound_analyze(species, compound);
    }

    ps("\nExport certification...\n");
    for (int i = 0; i < N - 6; i++) {
        int species = (i % 8) + 1;
        int qty = 10 + (i * 5);
        int value = 100 + (i * 30);
        int cons = (i % 3) + 1;            /* mostly LC/NT for export */
        goo_export_certify(species, qty, value, cons);
    }

    ps("\n");
    goo_report();
    goo_state();
    ps("\n=== Demo Complete ===\n");
    return 0;
}
