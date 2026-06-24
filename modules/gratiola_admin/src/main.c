/* gratiola_admin: Gratiola hedge hyssop medicinal herb administration (v1.0)
 * Gratiola batch cultivation, alkaloid tracking, pharmaceutical grading, supply chain, compliance
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);

#define N 16

/* Pharmaceutical grades */
#define GRA_GRADE_RAW       1   /* raw herb */
#define GRA_GRADE_PROCESSED 2   /* dried/cured */
#define GRA_GRADE_EXTRACT   3   /* standardized extract */
#define GRA_GRADE_PHARMA    4   /* pharmaceutical grade */

/* Supply chain stages */
#define GRA_SUPPLY_GROWER    1
#define GRA_SUPPLY_PROCESSOR 2
#define GRA_SUPPLY_PHARMA    3
#define GRA_SUPPLY_DISTRIBUT 4

/* Compliance standards */
#define GRA_GMP   1
#define GRA_FDA   2
#define GRA_EMA   3
#define GRA_WHO   4

typedef struct {
    int id;
    int batch;          /* batch number */
    int growth_day;     /* days since planting */
    int alkaloid_ppm;   /* gratiolin/gratiogenin content */
    int grade;          /* pharmaceutical grade */
    int dry_kg;         /* dried weight kg */
    int cure_week;      /* curing duration weeks */
    int supply_stage;   /* supply chain stage */
    int compliance_ok;  /* compliance flag */
    int active;
} gra_t;

typedef struct {
    int n_batch;       /* batch records */
    int n_harvest;     /* harvest records */
    int n_process;     /* processing records */
    int n_grade;       /* grading records */
    int n_supply;      /* supply chain records */
    int n_compliance;  /* compliance records */
    int t_dry_kg;      /* total dried weight */
    int t_alkaloid;    /* total alkaloid content */
    int t_revenue;     /* total revenue */
    int compliance_pass; /* passed compliance checks */
    int compliance_fail;
} gra_state_t;

static gra_t gra_batches[N];
static gra_t gra_harvests[N - 2];
static gra_t gra_proc_recs[N - 4];
static gra_t gra_grades[N - 6];
static gra_t gra_supply[N - 6];
static gra_t gra_comp_recs[N - 8];
static gra_state_t st;
static int init;

static void ps(const char *s) { host_print(s); }
static void pi(int v) {
    char b[32]; int i = 0;
    if (v < 0) { b[i++] = '-'; v = -v; }
    if (v == 0) { b[i++] = '0'; }
    else { int s = i; while (v > 0) { b[i++] = '0' + (v % 10); v /= 10; } int e = i - 1; while (s < e) { char t = b[s]; b[s] = b[e]; b[e] = t; s++; e--; } }
    b[i] = '\0'; host_print(b);
}

static int add(gra_t *a, int *cnt, int *sum, int mx, int batch, int growth, int alkaloid, int grade, int dry_kg, int cure, int supply, int compliance) {
    if (*cnt >= mx) return -1;
    gra_t *x = &a[*cnt];
    x->id = *cnt; x->batch = batch; x->growth_day = growth;
    x->alkaloid_ppm = alkaloid; x->grade = grade;
    x->dry_kg = dry_kg; x->cure_week = cure;
    x->supply_stage = supply; x->compliance_ok = compliance;
    x->active = 1;
    *sum += dry_kg; (*cnt)++;
    ps("[GRA] Record "); pi(*cnt - 1);
    ps(" batch="); pi(batch); ps(" day="); pi(growth);
    ps(" alk="); pi(alkaloid); ps(" grade="); pi(grade);
    ps(" dry="); pi(dry_kg); ps(" cure="); pi(cure); ps("\n");
    return *cnt - 1;
}

int gra_init(void) {
    if (init) return -1;
    st.n_batch = 0; st.n_harvest = 0; st.n_process = 0;
    st.n_grade = 0; st.n_supply = 0; st.n_compliance = 0;
    st.t_dry_kg = 0; st.t_alkaloid = 0; st.t_revenue = 0;
    st.compliance_pass = 0; st.compliance_fail = 0;
    for (int i = 0; i < N; i++) gra_batches[i].active = 0;
    for (int i = 0; i < N - 2; i++) gra_harvests[i].active = 0;
    for (int i = 0; i < N - 4; i++) gra_proc_recs[i].active = 0;
    for (int i = 0; i < N - 6; i++) gra_grades[i].active = 0;
    for (int i = 0; i < N - 6; i++) gra_supply[i].active = 0;
    for (int i = 0; i < N - 8; i++) gra_comp_recs[i].active = 0;
    init = 1;
    ps("[GRA] Gratiola herb admin initialized\n");
    return 0;
}

int gra_batch_plant(int batch, int growth_day, int alkaloid_ppm) {
    return add(gra_batches, &st.n_batch, &st.t_dry_kg, N,
               batch, growth_day, alkaloid_ppm, 0, 0, 0, 0, 0);
}

int gra_harvest(int batch, int growth_day, int alkaloid_ppm, int dry_kg) {
    st.t_dry_kg += dry_kg;
    st.t_alkaloid += alkaloid_ppm;
    return add(gra_harvests, &st.n_harvest, &st.t_dry_kg, N - 2,
               batch, growth_day, alkaloid_ppm, 0, dry_kg, 0, 0, 0);
}

int gra_process(int batch, int alkaloid_ppm, int dry_kg, int cure_week) {
    return add(gra_proc_recs, &st.n_process, &st.t_dry_kg, N - 4,
               batch, 0, alkaloid_ppm, GRA_GRADE_PROCESSED, dry_kg, cure_week, 0, 0);
}

int gra_grade(int batch, int grade, int alkaloid_ppm, int dry_kg) {
    /* Price based on grade */
    int price = 0;
    if (grade == GRA_GRADE_RAW) price = dry_kg * 5;
    else if (grade == GRA_GRADE_PROCESSED) price = dry_kg * 12;
    else if (grade == GRA_GRADE_EXTRACT) price = dry_kg * 30;
    else if (grade == GRA_GRADE_PHARMA) price = dry_kg * 80;
    st.t_revenue += price;
    return add(gra_grades, &st.n_grade, &st.t_revenue, N - 6,
               batch, 0, alkaloid_ppm, grade, dry_kg, 0, 0, 0);
}

int gra_supply_chain(int batch, int supply_stage, int qty, int value) {
    st.t_revenue += value;
    return add(gra_supply, &st.n_supply, &st.t_revenue, N - 6,
               batch, 0, 0, 0, qty, 0, supply_stage, 0);
}

int gra_compliance(int batch, int standard, int alkaloid_ppm, int pass) {
    if (pass) st.compliance_pass++;
    else st.compliance_fail++;
    return add(gra_comp_recs, &st.n_compliance, &st.compliance_pass, N - 8,
               batch, 0, alkaloid_ppm, 0, 0, 0, 0, pass);
}

void gra_report(void) {
    ps("[GRA] Batch: "); pi(st.n_batch);
    ps(" Harvest: "); pi(st.n_harvest);
    ps(" Process: "); pi(st.n_process);
    ps(" Grade: "); pi(st.n_grade);
    ps(" Supply: "); pi(st.n_supply);
    ps(" Compliance: "); pi(st.n_compliance);
    ps("\n[GRA] Total dry: "); pi(st.t_dry_kg); ps(" kg");
    ps("\n[GRA] Total alkaloid: "); pi(st.t_alkaloid); ps(" ppm");
    ps("\n[GRA] Total revenue: "); pi(st.t_revenue); ps(" USD");
    ps("\n[GRA] Compliance pass: "); pi(st.compliance_pass);
    ps(" fail: "); pi(st.compliance_fail);
    ps("\n");
}

void gra_state(void) {
    ps("[GRA] Bat="); pi(st.n_batch);
    ps(" Harv="); pi(st.n_harvest);
    ps(" Proc="); pi(st.n_process);
    ps(" Grd="); pi(st.n_grade);
    ps(" Sup="); pi(st.n_supply);
    ps(" Cmp="); pi(st.n_compliance);
    ps("\n");
}

int main(void) {
    ps("=== Gratiola Herb Admin Demo ===\n\n");
    gra_init();

    ps("Batch planting...\n");
    for (int i = 0; i < N; i++) {
        int batch = 1000 + i;
        int growth_day = 60 + (i * 10);
        int alkaloid = 80 + (i * 13) % 150;
        gra_batch_plant(batch, growth_day, alkaloid);
    }

    ps("\nHarvest cycles...\n");
    for (int i = 0; i < N - 2; i++) {
        int batch = 1000 + i;
        int growth_day = 90 + (i * 7);
        int alkaloid = 100 + (i * 15) % 180;
        int dry_kg = 30 + (i * 8);
        gra_harvest(batch, growth_day, alkaloid, dry_kg);
    }

    ps("\nProcessing...\n");
    for (int i = 0; i < N - 4; i++) {
        int batch = 1000 + i;
        int alkaloid = 120 + (i * 14) % 160;
        int dry_kg = 25 + (i * 6);
        int cure_week = 2 + (i % 4);
        gra_process(batch, alkaloid, dry_kg, cure_week);
    }

    ps("\nPharmaceutical grading...\n");
    for (int i = 0; i < N - 6; i++) {
        int batch = 1000 + i;
        int grade = (i % 4) + 1;
        int alkaloid = 150 + (i * 12) % 200;
        int dry_kg = 20 + (i * 5);
        gra_grade(batch, grade, alkaloid, dry_kg);
    }

    ps("\nSupply chain flow...\n");
    for (int i = 0; i < N - 6; i++) {
        int batch = 1000 + i;
        int stage = (i % 4) + 1;
        int qty = 15 + (i * 4);
        int value = 200 + (i * 40);
        gra_supply_chain(batch, stage, qty, value);
    }

    ps("\nCompliance checks...\n");
    for (int i = 0; i < N - 8; i++) {
        int batch = 1000 + i;
        int standard = (i % 4) + 1;      /* GMP/FDA/EMA/WHO */
        int alkaloid = 160 + (i * 10) % 180;
        int pass = (alkaloid >= 200) ? 1 : 0;
        gra_compliance(batch, standard, alkaloid, pass);
    }

    ps("\n");
    gra_report();
    gra_state();
    ps("\n=== Demo Complete ===\n");
    return 0;
}
