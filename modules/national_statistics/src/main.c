/* national_statistics: National statistics administration system (v1.0)
 * Statistical management, census data, economic statistics, social statistics, publishing
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

#define MAX_STATISTICS   16
#define MAX_CENSUS       14
#define MAX_ECONOMIC     12
#define MAX_SOCIAL       10
#define MAX_PUBLISHING   10

typedef struct {
    int    statistics_id;
    int    survey_type;
    int    data_category;
    int    surveys_conducted;
    int    data_points;
    int    reports_generated;
    int    year;
    int    active;
} statistics_t;

typedef struct {
    int    census_id;
    int    census_type;
    int    region_id;
    int    population_counted;
    int    households_surveyed;
    int    sampling_rate;
    int    year;
    int    active;
} census_t;

typedef struct {
    int    economic_id;
    int    sector_type;
    int    indicator_type;
    int    gdp_contribution;
    int    trade_volume;
    int    growth_rate;
    int    year;
    int    active;
} economic_t;

typedef struct {
    int    social_id;
    int    social_field;
    int    metric_type;
    int    institutions_count;
    int    personnel_count;
    int    coverage_rate;
    int    year;
    int    active;
} social_t;

typedef struct {
    int    publishing_id;
    int    publication_type;
    int    data_domain;
    int    publications_released;
    int    data_entries;
    int    downloads;
    int    year;
    int    active;
} publishing_t;

typedef struct {
    int    n_statistics;
    int    n_census;
    int    n_economic;
    int    n_social;
    int    n_publishing;
    int    total_surveys;
    int    total_population;
    int    total_gdp;
    int    total_institutions;
    int    total_publications;
} ns_state_t;

static statistics_t statistics_list[MAX_STATISTICS];
static census_t census_list[MAX_CENSUS];
static economic_t economic_list[MAX_ECONOMIC];
static social_t social_list[MAX_SOCIAL];
static publishing_t publishing_list[MAX_PUBLISHING];
static ns_state_t ns;

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

int ns_init(void) {
    if (initialized) return -1;
    ns.n_statistics = 0; ns.n_census = 0; ns.n_economic = 0;
    ns.n_social = 0; ns.n_publishing = 0;
    ns.total_surveys = 0; ns.total_population = 0;
    ns.total_gdp = 0; ns.total_institutions = 0;
    ns.total_publications = 0;
    for (int i = 0; i < MAX_STATISTICS; i++) statistics_list[i].active = 0;
    for (int i = 0; i < MAX_CENSUS; i++) census_list[i].active = 0;
    for (int i = 0; i < MAX_ECONOMIC; i++) economic_list[i].active = 0;
    for (int i = 0; i < MAX_SOCIAL; i++) social_list[i].active = 0;
    for (int i = 0; i < MAX_PUBLISHING; i++) publishing_list[i].active = 0;
    initialized = 1;
    print_str("[NS] National statistics initialized\n");
    return 0;
}

int ns_statistics(int survey_type, int data_cat, int surveys, int data_pts, int reports, int year) {
    if (ns.n_statistics >= MAX_STATISTICS) return -1;
    statistics_t* s = &statistics_list[ns.n_statistics];
    s->statistics_id = ns.n_statistics;
    s->survey_type = survey_type;
    s->data_category = data_cat;
    s->surveys_conducted = surveys;
    s->data_points = data_pts;
    s->reports_generated = reports;
    s->year = year;
    s->active = 1;
    ns.total_surveys += surveys;
    ns.n_statistics++;
    print_str("[NS] Statistics "); print_int(ns.n_statistics - 1);
    print_str(" type="); print_int(survey_type);
    print_str(" cat="); print_int(data_cat);
    print_str(" srv="); print_int(surveys);
    print_str(" dpt="); print_int(data_pts);
    print_str(" rpt="); print_int(reports); print_str("\n");
    return ns.n_statistics - 1;
}

int ns_census(int census_type, int region, int population, int households, int sampling, int year) {
    if (ns.n_census >= MAX_CENSUS) return -1;
    census_t* c = &census_list[ns.n_census];
    c->census_id = ns.n_census;
    c->census_type = census_type;
    c->region_id = region;
    c->population_counted = population;
    c->households_surveyed = households;
    c->sampling_rate = sampling;
    c->year = year;
    c->active = 1;
    ns.total_population += population;
    ns.n_census++;
    print_str("[NS] Census "); print_int(ns.n_census - 1);
    print_str(" type="); print_int(census_type);
    print_str(" rgn="); print_int(region);
    print_str(" pop="); print_int(population);
    print_str(" hhd="); print_int(households);
    print_str(" spl="); print_int(sampling); print_str("\n");
    return ns.n_census - 1;
}

int ns_economic(int sector, int indicator, int gdp, int trade, int growth, int year) {
    if (ns.n_economic >= MAX_ECONOMIC) return -1;
    economic_t* e = &economic_list[ns.n_economic];
    e->economic_id = ns.n_economic;
    e->sector_type = sector;
    e->indicator_type = indicator;
    e->gdp_contribution = gdp;
    e->trade_volume = trade;
    e->growth_rate = growth;
    e->year = year;
    e->active = 1;
    ns.total_gdp += gdp;
    ns.n_economic++;
    print_str("[NS] Economic "); print_int(ns.n_economic - 1);
    print_str(" sector="); print_int(sector);
    print_str(" ind="); print_int(indicator);
    print_str(" gdp="); print_int(gdp);
    print_str(" trd="); print_int(trade);
    print_str(" grw="); print_int(growth); print_str("\n");
    return ns.n_economic - 1;
}

int ns_social(int field, int metric, int institutions, int personnel, int coverage, int year) {
    if (ns.n_social >= MAX_SOCIAL) return -1;
    social_t* s = &social_list[ns.n_social];
    s->social_id = ns.n_social;
    s->social_field = field;
    s->metric_type = metric;
    s->institutions_count = institutions;
    s->personnel_count = personnel;
    s->coverage_rate = coverage;
    s->year = year;
    s->active = 1;
    ns.total_institutions += institutions;
    ns.n_social++;
    print_str("[NS] Social "); print_int(ns.n_social - 1);
    print_str(" fld="); print_int(field);
    print_str(" mtr="); print_int(metric);
    print_str(" inst="); print_int(institutions);
    print_str(" pers="); print_int(personnel);
    print_str(" cov="); print_int(coverage); print_str("\n");
    return ns.n_social - 1;
}

int ns_publishing(int pub_type, int domain, int released, int entries, int dloads, int year) {
    if (ns.n_publishing >= MAX_PUBLISHING) return -1;
    publishing_t* p = &publishing_list[ns.n_publishing];
    p->publishing_id = ns.n_publishing;
    p->publication_type = pub_type;
    p->data_domain = domain;
    p->publications_released = released;
    p->data_entries = entries;
    p->downloads = dloads;
    p->year = year;
    p->active = 1;
    ns.total_publications += released;
    ns.n_publishing++;
    print_str("[NS] Publishing "); print_int(ns.n_publishing - 1);
    print_str(" type="); print_int(pub_type);
    print_str(" dom="); print_int(domain);
    print_str(" rel="); print_int(released);
    print_str(" ent="); print_int(entries);
    print_str(" dld="); print_int(dloads); print_str("\n");
    return ns.n_publishing - 1;
}

void ns_statistics_report(void) {
    print_str("[NS] Statistics report:\n");
    print_str("  Survey categories: "); print_int(ns.n_statistics); print_str("\n");
    print_str("  Total surveys: "); print_int(ns.total_surveys); print_str("\n");
}

void ns_census_report(void) {
    print_str("[NS] Census report:\n");
    print_str("  Census regions: "); print_int(ns.n_census); print_str("\n");
    print_str("  Total population counted: "); print_int(ns.total_population); print_str("\n");
}

void ns_economic_report(void) {
    print_str("[NS] Economic report:\n");
    print_str("  Economic sectors: "); print_int(ns.n_economic); print_str("\n");
    print_str("  Total GDP contribution: "); print_int(ns.total_gdp); print_str("\n");
    print_str("  Social institutions: "); print_int(ns.total_institutions); print_str("\n");
    print_str("  Publications released: "); print_int(ns.total_publications); print_str("\n");
}

void ns_print_state(void) {
    print_str("[NS] St="); print_int(ns.n_statistics);
    print_str(" Cn="); print_int(ns.n_census);
    print_str(" Ec="); print_int(ns.n_economic);
    print_str(" Sc="); print_int(ns.n_social);
    print_str(" Pb="); print_int(ns.n_publishing);
    print_str("\n");
}

int main(void) {
    print_str("=== National Statistics Demo ===\n\n");
    ns_init();

    print_str("Statistical surveys...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 6) + 1;
        int srv = 100 + (i * 30);
        int dpt = 10000 + (i * 2000);
        int rpt = 20 + (i * 5);
        int year = 2020 + (i % 5);
        ns_statistics(type, cat, srv, dpt, rpt, year);
    }

    print_str("\nCensus data...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int rgn = (i % 8) + 1;
        int pop = 500000 + (i * 100000);
        int hhd = 100000 + (i * 25000);
        int spl = 5 + (i % 10);
        int year = 2020 + (i % 5);
        ns_census(type, rgn, pop, hhd, spl, year);
    }

    print_str("\nEconomic statistics...\n");
    for (int i = 0; i < 12; i++) {
        int sector = (i % 5) + 1;
        int ind = (i % 4) + 1;
        int gdp = 5000 + (i * 1000);
        int trd = 2000 + (i * 500);
        int grw = 3 + (i % 8);
        int year = 2021 + (i % 4);
        ns_economic(sector, ind, gdp, trd, grw, year);
    }

    print_str("\nSocial statistics...\n");
    for (int i = 0; i < 10; i++) {
        int fld = (i % 5) + 1;
        int mtr = (i % 4) + 1;
        int inst = 200 + (i * 50);
        int pers = 5000 + (i * 1000);
        int cov = 70 + (i * 3);
        int year = 2023 + (i % 2);
        ns_social(fld, mtr, inst, pers, cov, year);
    }

    print_str("\nStatistical publishing...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int dom = (i % 5) + 1;
        int rel = 30 + (i * 8);
        int ent = 5000 + (i * 1000);
        int dld = 1000 + (i * 300);
        int year = 2024;
        ns_publishing(type, dom, rel, ent, dld, year);
    }

    print_str("\nStatistics report...\n");
    ns_statistics_report();

    print_str("\nCensus report...\n");
    ns_census_report();

    print_str("\nEconomic report...\n");
    ns_economic_report();

    print_str("\nFinal state...\n");
    ns_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
