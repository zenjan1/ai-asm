/* statistics_admin: Statistics administration system (v1.0)
 * Economic statistics, population, social stats, data analysis, enforcement
 */
#include <stddef.h>

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_ECONOMIC     16
#define MAX_POPULATION   14
#define MAX_SOCIAL       12
#define MAX_ANALYSIS     10
#define MAX_ENFORCE      10

typedef struct {
    int    economic_id;
    int    region_id;
    int    indicator_type;
    int    value;
    int    growth_rate;
    int    contribution;
    int    year;
    int    active;
} economic_t;

typedef struct {
    int    population_id;
    int    region_id;
    int    survey_type;
    int    total_population;
    int    birth_rate;
    int    death_rate;
    int    year;
    int    active;
} population_t;

typedef struct {
    int    social_id;
    int    domain_id;
    int    social_type;
    int    sample_size;
    int    coverage_rate;
    int    satisfaction;
    int    year;
    int    active;
} social_t;

typedef struct {
    int    analysis_id;
    int    dataset_id;
    int    analysis_type;
    int    data_points;
    int    accuracy;
    int    processing_time;
    int    year;
    int    active;
} analysis_t;

typedef struct {
    int    enforce_id;
    int    case_id;
    int    violation_type;
    int    entities_involved;
    int    data_corrected;
    int    penalty;
    int    year;
    int    active;
} enforce_t;

typedef struct {
    int    n_economic;
    int    n_population;
    int    n_social;
    int    n_analysis;
    int    n_enforce;
    int    total_gdp;
    int    total_population;
    int    total_samples;
    int    total_data_points;
    int    total_penalty;
} sta_state_t;

static economic_t economics[MAX_ECONOMIC];
static population_t populations[MAX_POPULATION];
static social_t socials[MAX_SOCIAL];
static analysis_t analyses[MAX_ANALYSIS];
static enforce_t enforces[MAX_ENFORCE];
static sta_state_t sta;

static int initialized = 0;

static void print_str(const char* s) { host_print(s); }
static void print_int(int val) {
    char buf[32]; int i = 0;
    if (val < 0) { buf[i++] = '-'; val = -val; }
    if (val == 0) { buf[i++] = '0'; }
    else { int s = i; while (val > 0) { buf[i++] = '0' + (val % 10); val /= 10; }
           int e = i - 1; while (s < e) { char t = buf[s]; buf[s] = buf[e]; buf[e] = t; s++; e--; } }
    buf[i] = '\0'; host_print(buf);
}

int sta_init(void) {
    if (initialized) return -1;
    sta.n_economic = 0; sta.n_population = 0; sta.n_social = 0;
    sta.n_analysis = 0; sta.n_enforce = 0;
    sta.total_gdp = 0; sta.total_population = 0;
    sta.total_samples = 0; sta.total_data_points = 0;
    sta.total_penalty = 0;
    for (int i = 0; i < MAX_ECONOMIC; i++) economics[i].active = 0;
    for (int i = 0; i < MAX_POPULATION; i++) populations[i].active = 0;
    for (int i = 0; i < MAX_SOCIAL; i++) socials[i].active = 0;
    for (int i = 0; i < MAX_ANALYSIS; i++) analyses[i].active = 0;
    for (int i = 0; i < MAX_ENFORCE; i++) enforces[i].active = 0;
    initialized = 1;
    print_str("[STA] Statistics admin initialized\n");
    return 0;
}

int sta_economic(int region, int indicator_type, int value, int growth, int contribution, int year) {
    if (sta.n_economic >= MAX_ECONOMIC) return -1;
    economic_t* e = &economics[sta.n_economic];
    e->economic_id = sta.n_economic;
    e->region_id = region;
    e->indicator_type = indicator_type;
    e->value = value;
    e->growth_rate = growth;
    e->contribution = contribution;
    e->year = year;
    e->active = 1;
    sta.total_gdp += value;
    sta.n_economic++;
    print_str("[STA] Economic "); print_int(sta.n_economic - 1);
    print_str(" rgn="); print_int(region);
    print_str(" type="); print_int(indicator_type);
    print_str(" val=$"); print_int(value);
    print_str(" grw="); print_int(growth); print_str("%");
    print_str(" cntr="); print_int(contribution); print_str("%\n");
    return sta.n_economic - 1;
}

int sta_population(int region, int survey_type, int total_pop, int birth_rate, int death_rate, int year) {
    if (sta.n_population >= MAX_POPULATION) return -1;
    population_t* p = &populations[sta.n_population];
    p->population_id = sta.n_population;
    p->region_id = region;
    p->survey_type = survey_type;
    p->total_population = total_pop;
    p->birth_rate = birth_rate;
    p->death_rate = death_rate;
    p->year = year;
    p->active = 1;
    sta.total_population += total_pop;
    sta.n_population++;
    print_str("[STA] Population "); print_int(sta.n_population - 1);
    print_str(" rgn="); print_int(region);
    print_str(" type="); print_int(survey_type);
    print_str(" pop="); print_int(total_pop);
    print_str(" brth="); print_int(birth_rate);
    print_str(" dth="); print_int(death_rate); print_str("\n");
    return sta.n_population - 1;
}

int sta_social(int domain, int social_type, int sample, int coverage, int satisfaction, int year) {
    if (sta.n_social >= MAX_SOCIAL) return -1;
    social_t* s = &socials[sta.n_social];
    s->social_id = sta.n_social;
    s->domain_id = domain;
    s->social_type = social_type;
    s->sample_size = sample;
    s->coverage_rate = coverage;
    s->satisfaction = satisfaction;
    s->year = year;
    s->active = 1;
    sta.total_samples += sample;
    sta.n_social++;
    print_str("[STA] Social "); print_int(sta.n_social - 1);
    print_str(" dom="); print_int(domain);
    print_str(" type="); print_int(social_type);
    print_str(" smp="); print_int(sample);
    print_str(" cov="); print_int(coverage); print_str("%");
    print_str(" sat="); print_int(satisfaction); print_str("\n");
    return sta.n_social - 1;
}

int sta_analysis(int dataset, int analysis_type, int data_points, int accuracy, int processing_time, int year) {
    if (sta.n_analysis >= MAX_ANALYSIS) return -1;
    analysis_t* a = &analyses[sta.n_analysis];
    a->analysis_id = sta.n_analysis;
    a->dataset_id = dataset;
    a->analysis_type = analysis_type;
    a->data_points = data_points;
    a->accuracy = accuracy;
    a->processing_time = processing_time;
    a->year = year;
    a->active = 1;
    sta.total_data_points += data_points;
    sta.n_analysis++;
    print_str("[STA] Analysis "); print_int(sta.n_analysis - 1);
    print_str(" dts="); print_int(dataset);
    print_str(" type="); print_int(analysis_type);
    print_str(" pts="); print_int(data_points);
    print_str(" acc="); print_int(accuracy); print_str("%");
    print_str(" tm="); print_int(processing_time); print_str("s\n");
    return sta.n_analysis - 1;
}

int sta_enforce(int case_id, int violation_type, int entities, int corrected, int penalty, int year) {
    if (sta.n_enforce >= MAX_ENFORCE) return -1;
    enforce_t* ef = &enforces[sta.n_enforce];
    ef->enforce_id = sta.n_enforce;
    ef->case_id = case_id;
    ef->violation_type = violation_type;
    ef->entities_involved = entities;
    ef->data_corrected = corrected;
    ef->penalty = penalty;
    ef->year = year;
    ef->active = 1;
    sta.total_penalty += penalty;
    sta.n_enforce++;
    print_str("[STA] Enforce "); print_int(sta.n_enforce - 1);
    print_str(" cas="); print_int(case_id);
    print_str(" type="); print_int(violation_type);
    print_str(" ent="); print_int(entities);
    print_str(" crt="); print_int(corrected);
    print_str(" pen=$"); print_int(penalty); print_str("\n");
    return sta.n_enforce - 1;
}

void sta_economic_report(void) {
    print_str("[STA] Economic report:\n");
    print_str("  Indicators: "); print_int(sta.n_economic); print_str("\n");
    print_str("  Total GDP: $"); print_int(sta.total_gdp); print_str("\n");
}

void sta_population_report(void) {
    print_str("[STA] Population report:\n");
    print_str("  Surveys: "); print_int(sta.n_population); print_str("\n");
    print_str("  Total population: "); print_int(sta.total_population); print_str("\n");
}

void sta_analysis_report(void) {
    print_str("[STA] Analysis report:\n");
    print_str("  Social surveys: "); print_int(sta.n_social); print_str("\n");
    print_str("  Total samples: "); print_int(sta.total_samples); print_str("\n");
    print_str("  Data analyses: "); print_int(sta.n_analysis); print_str("\n");
    print_str("  Total data points: "); print_int(sta.total_data_points); print_str("\n");
    print_str("  Enforcement cases: "); print_int(sta.n_enforce); print_str("\n");
    print_str("  Total penalties: $"); print_int(sta.total_penalty); print_str("\n");
}

void sta_print_state(void) {
    print_str("[STA] Ec="); print_int(sta.n_economic);
    print_str(" Pp="); print_int(sta.n_population);
    print_str(" Sc="); print_int(sta.n_social);
    print_str(" An="); print_int(sta.n_analysis);
    print_str(" En="); print_int(sta.n_enforce);
    print_str("\n");
}

int main(void) {
    print_str("=== Statistics Admin Demo ===\n\n");
    sta_init();

    print_str("Economic statistics...\n");
    for (int i = 0; i < 16; i++) {
        int rgn = (i % 8) + 1;
        int type = (i % 5) + 1;
        int val = 1000000 + (i * 500000);
        int grw = 3 + (i % 8);
        int cntr = 10 + (i * 3);
        int year = 2020 + (i % 5);
        sta_economic(rgn, type, val, grw, cntr, year);
    }

    print_str("\nPopulation statistics...\n");
    for (int i = 0; i < 14; i++) {
        int rgn = (i % 8) + 1;
        int type = (i % 3) + 1;
        int pop = 500000 + (i * 100000);
        int brth = 10 + (i % 8);
        int dth = 5 + (i % 6);
        int year = 2021 + (i % 4);
        sta_population(rgn, type, pop, brth, dth, year);
    }

    print_str("\nSocial statistics...\n");
    for (int i = 0; i < 12; i++) {
        int dom = (i % 6) + 1;
        int type = (i % 4) + 1;
        int smp = 10000 + (i * 5000);
        int cov = 80 + (i % 15);
        int sat = 70 + (i * 2);
        int year = 2022 + (i % 3);
        sta_social(dom, type, smp, cov, sat, year);
    }

    print_str("\nData analysis...\n");
    for (int i = 0; i < 10; i++) {
        int dts = 100 + (i * 10);
        int type = (i % 3) + 1;
        int pts = 100000 + (i * 50000);
        int acc = 90 + (i % 8);
        int tm = 10 + (i * 5);
        int year = 2023 + (i % 2);
        sta_analysis(dts, type, pts, acc, tm, year);
    }

    print_str("\nStatistical enforcement...\n");
    for (int i = 0; i < 10; i++) {
        int cas = 200 + (i * 11);
        int type = (i % 4) + 1;
        int ent = 5 + (i * 2);
        int crt = ent - (i % 3);
        int pen = 10000 + (i * 5000);
        int year = 2024;
        sta_enforce(cas, type, ent, crt, pen, year);
    }

    print_str("\nEconomic report...\n");
    sta_economic_report();

    print_str("\nPopulation report...\n");
    sta_population_report();

    print_str("\nAnalysis report...\n");
    sta_analysis_report();

    print_str("\nFinal state...\n");
    sta_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
