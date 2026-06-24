/* gyrostachys_admin: Gyrostachys orchid research cultivation administration (v1.0)
 * Taxonomy, propagation, in-vitro culture, mycorrhiza, pollination, DNA barcoding
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);

#define N 16

/* Propagation stages */
#define GYRO_PROP_FLASK      1
#define GYRO_PROP_DEFLASKED  2
#define GYRO_PROP_POTTED     3
#define GYRO_PROP_ACCLIMATED 4

/* Culture media types */
#define GYRO_MEDIA_MS     1   /* Murashige & Skoog */
#define GYRO_MEDIA_VW     2   /* Vacin & Went */
#define GYRO_MEDIA_KC     3   /* Knudson C */
#define GYRO_MEDIA_HYPO   4   /* Hypo-red */

typedef struct {
    int id;
    int taxon;            /* taxon ID 1-12 */
    int prop_stage;       /* propagation stage */
    int culture_week;     /* weeks in culture */
    int media_type;       /* culture media */
    int myco_colonized;   /* mycorrhizal colonization % */
    int poll_success;     /* pollination success % */
    int chrom_count;      /* chromosome count */
    int barcode_id;       /* DNA barcode registry ID */
    int active;
} gyro_t;

typedef struct {
    int n_taxon;
    int n_prop;
    int n_culture;
    int n_myco;
    int n_pollination;
    int n_barcode;
    int total_plants;
    int avg_colonization;
    int avg_poll_success;
    int flask_count;
    int barcode_registered;
} gyro_state_t;

static gyro_t gyro_taxa[N];
static gyro_t gyro_prop_recs[N - 2];
static gyro_t gyro_cultures[N - 4];
static gyro_t gyro_myco_recs[N - 6];
static gyro_t gyro_poll_recs[N - 6];
static gyro_t gyro_barcodes[N - 8];
static gyro_state_t st;
static int init;

static void ps(const char *s) { host_print(s); }
static void pi(int v) {
    char b[32]; int i = 0;
    if (v < 0) { b[i++] = '-'; v = -v; }
    if (v == 0) { b[i++] = '0'; }
    else { int s = i; while (v > 0) { b[i++] = '0' + (v % 10); v /= 10; } int e = i - 1; while (s < e) { char t = b[s]; b[s] = b[e]; b[e] = t; s++; e--; } }
    b[i] = '\0'; host_print(b);
}

static int add_gyro(gyro_t *a, int *cnt, int *sum, int mx, int taxon, int prop, int week, int media, int myco, int poll, int chrom, int barcode) {
    if (*cnt >= mx) return -1;
    gyro_t *x = &a[*cnt];
    x->id = *cnt; x->taxon = taxon; x->prop_stage = prop;
    x->culture_week = week; x->media_type = media;
    x->myco_colonized = myco; x->poll_success = poll;
    x->chrom_count = chrom; x->barcode_id = barcode; x->active = 1;
    *sum += week; (*cnt)++;
    ps("[GYRO] Record "); pi(*cnt - 1);
    ps(" tax="); pi(taxon); ps(" prop="); pi(prop);
    ps(" wk="); pi(week); ps(" med="); pi(media);
    ps(" myco="); pi(myco); ps(" poll="); pi(poll); ps("\n");
    return *cnt - 1;
}

int gyro_init(void) {
    if (init) return -1;
    st.n_taxon = 0; st.n_prop = 0; st.n_culture = 0;
    st.n_myco = 0; st.n_pollination = 0; st.n_barcode = 0;
    st.total_plants = 0; st.avg_colonization = 0;
    st.avg_poll_success = 0; st.flask_count = 0;
    st.barcode_registered = 0;
    for (int i = 0; i < N; i++) gyro_taxa[i].active = 0;
    for (int i = 0; i < N - 2; i++) gyro_prop_recs[i].active = 0;
    for (int i = 0; i < N - 4; i++) gyro_cultures[i].active = 0;
    for (int i = 0; i < N - 6; i++) gyro_myco_recs[i].active = 0;
    for (int i = 0; i < N - 6; i++) gyro_poll_recs[i].active = 0;
    for (int i = 0; i < N - 8; i++) gyro_barcodes[i].active = 0;
    init = 1;
    ps("[GYRO] Gyrostachys research initialized\n");
    return 0;
}

int gyro_taxon_register(int taxon, int chrom_count, int barcode_id) {
    st.total_plants++;
    st.barcode_registered++;
    return add_gyro(gyro_taxa, &st.n_taxon, &st.total_plants, N,
                    taxon, 0, 0, 0, 0, 0, chrom_count, barcode_id);
}

int gyro_propagate(int taxon, int prop_stage, int count) {
    st.total_plants += count;
    if (prop_stage == GYRO_PROP_FLASK) st.flask_count += count;
    return add_gyro(gyro_prop_recs, &st.n_prop, &st.total_plants, N - 2,
                    taxon, prop_stage, 0, 0, 0, 0, 0, 0);
}

int gyro_culture_monitor(int taxon, int week, int media) {
    return add_gyro(gyro_cultures, &st.n_culture, &st.n_culture, N - 4,
                    taxon, 0, week, media, 0, 0, 0, 0);
}

int gyro_mycorrhiza_check(int taxon, int colonization_pct) {
    st.avg_colonization += colonization_pct;
    return add_gyro(gyro_myco_recs, &st.n_myco, &st.avg_colonization, N - 6,
                    taxon, 0, 0, 0, colonization_pct, 0, 0, 0);
}

int gyro_pollinate(int taxon, int success_pct) {
    st.avg_poll_success += success_pct;
    return add_gyro(gyro_poll_recs, &st.n_pollination, &st.avg_poll_success, N - 6,
                    taxon, 0, 0, 0, 0, success_pct, 0, 0);
}

int gyro_barcode(int taxon, int barcode_id) {
    st.barcode_registered++;
    return add_gyro(gyro_barcodes, &st.n_barcode, &st.barcode_registered, N - 8,
                    taxon, 0, 0, 0, 0, 0, 0, barcode_id);
}

void gyro_report(void) {
    ps("[GYRO] Taxon: "); pi(st.n_taxon);
    ps(" Prop: "); pi(st.n_prop);
    ps(" Culture: "); pi(st.n_culture);
    ps(" Myco: "); pi(st.n_myco);
    ps(" Poll: "); pi(st.n_pollination);
    ps(" Barcode: "); pi(st.n_barcode);
    ps("\n[GYRO] Total plants: "); pi(st.total_plants);
    ps(" Flask: "); pi(st.flask_count);
    ps("\n[GYRO] Avg myco: "); pi(st.avg_colonization); ps(" %");
    ps("\n[GYRO] Avg poll success: "); pi(st.avg_poll_success); ps(" %");
    ps("\n[GYRO] Barcodes: "); pi(st.barcode_registered);
    ps("\n");
}

void gyro_state(void) {
    ps("[GYRO] Tax="); pi(st.n_taxon);
    ps(" Prop="); pi(st.n_prop);
    ps(" Cult="); pi(st.n_culture);
    ps(" Myco="); pi(st.n_myco);
    ps(" Poll="); pi(st.n_pollination);
    ps(" Bar="); pi(st.n_barcode);
    ps("\n");
}

int main(void) {
    ps("=== Gyrostachys Orchid Research Demo ===\n\n");
    gyro_init();

    ps("Taxon registration...\n");
    for (int i = 0; i < N; i++) {
        int taxon = (i % 8) + 1;
        int chrom = 20 + (i * 2) % 20;     /* 2n = 20-38 */
        int barcode = 5000 + i * 100;
        gyro_taxon_register(taxon, chrom, barcode);
    }

    ps("\nPropagation pipeline...\n");
    for (int i = 0; i < N - 2; i++) {
        int taxon = (i % 8) + 1;
        int stage = (i % 4) + 1;
        int count = 5 + (i * 3);
        gyro_propagate(taxon, stage, count);
    }

    ps("\nCulture monitoring...\n");
    for (int i = 0; i < N - 4; i++) {
        int taxon = (i % 8) + 1;
        int week = 2 + (i % 8);
        int media = (i % 4) + 1;
        gyro_culture_monitor(taxon, week, media);
    }

    ps("\nMycorrhiza inoculation checks...\n");
    for (int i = 0; i < N - 6; i++) {
        int taxon = (i % 8) + 1;
        int colonization = 30 + (i * 8) % 60;
        gyro_mycorrhiza_check(taxon, colonization);
    }

    ps("\nPollination trials...\n");
    for (int i = 0; i < N - 6; i++) {
        int taxon = (i % 8) + 1;
        int success = 20 + (i * 7) % 70;
        gyro_pollinate(taxon, success);
    }

    ps("\nDNA barcoding...\n");
    for (int i = 0; i < N - 8; i++) {
        int taxon = (i % 8) + 1;
        int barcode = 6000 + i * 100;
        gyro_barcode(taxon, barcode);
    }

    ps("\n");
    gyro_report();
    gyro_state();
    ps("\n=== Demo Complete ===\n");
    return 0;
}
