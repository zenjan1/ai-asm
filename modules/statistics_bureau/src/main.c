/* statistics_bureau: Statistics management (v1.0)
 * Census, economic, social, agricultural statistics, surveys
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

#define MAX_POPULATION 18
#define MAX_ECONOMIC   14
#define MAX_SOCIAL     12
#define MAX_AGRICULTURE 10
#define MAX_SURVEYS    12

typedef struct {
    int    pop_id;
    int    zone;
    int    total;
    int    male;
    int    female;
    int    urban;
    int    rural;
    int    year;
    int    active;
} population_t;

typedef struct {
    int    econ_id;
    int    zone;
    int    gdp;
    int    primary;
    int    secondary;
    int    tertiary;
    int    growth_rate;
    int    year;
    int    active;
} economic_t;

typedef struct {
    int    soc_id;
    int    zone;
    int    employed;
    int    unemployed;
    int    avg_income;
    int    avg_consumption;
    int    poverty_rate;
    int    year;
    int    active;
} social_t;

typedef struct {
    int    agri_id;
    int    zone;
    int    grain_tons;
    int    meat_tons;
    int    milk_tons;
    int    farmland_ha;
    int    yield_per_ha;
    int    year;
    int    active;
} agriculture_t;

typedef struct {
    int    survey_id;
    int    type;
    int    zone;
    int    sample_size;
    int    response_rate;
    int    data_points;
    int    completed;
    int    year;
    int    active;
} survey_t;

typedef struct {
    int    n_population;
    int    n_economic;
    int    n_social;
    int    n_agriculture;
    int    n_surveys;
    int    total_population;
    int    total_gdp;
    int    total_employed;
    int    total_unemployed;
    int    total_grain;
    int    total_farmland;
    int    total_surveys_done;
} sb_state_t;

static population_t populations[MAX_POPULATION];
static economic_t economics[MAX_ECONOMIC];
static social_t socials[MAX_SOCIAL];
static agriculture_t agricultures[MAX_AGRICULTURE];
static survey_t surveys[MAX_SURVEYS];
static sb_state_t sb;

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

int sb_init(void) {
    if (initialized) return -1;
    sb.n_population = 0; sb.n_economic = 0; sb.n_social = 0;
    sb.n_agriculture = 0; sb.n_surveys = 0;
    sb.total_population = 0; sb.total_gdp = 0;
    sb.total_employed = 0; sb.total_unemployed = 0;
    sb.total_grain = 0; sb.total_farmland = 0;
    sb.total_surveys_done = 0;
    for (int i = 0; i < MAX_POPULATION; i++) populations[i].active = 0;
    for (int i = 0; i < MAX_ECONOMIC; i++) economics[i].active = 0;
    for (int i = 0; i < MAX_SOCIAL; i++) socials[i].active = 0;
    for (int i = 0; i < MAX_AGRICULTURE; i++) agricultures[i].active = 0;
    for (int i = 0; i < MAX_SURVEYS; i++) surveys[i].active = 0;
    initialized = 1;
    print_str("[SB] Statistics bureau initialized\n");
    return 0;
}

int sb_census(int zone, int total, int male, int female, int urban, int rural, int year) {
    if (sb.n_population >= MAX_POPULATION) return -1;
    population_t* p = &populations[sb.n_population];
    p->pop_id = sb.n_population;
    p->zone = zone;
    p->total = total;
    p->male = male;
    p->female = female;
    p->urban = urban;
    p->rural = rural;
    p->year = year;
    p->active = 1;
    sb.total_population += total;
    sb.n_population++;
    print_str("[SB] Census "); print_int(sb.n_population - 1);
    print_str(" zone="); print_int(zone);
    print_str(" total="); print_int(total);
    print_str(" M="); print_int(male);
    print_str(" F="); print_int(female);
    print_str(" U="); print_int(urban);
    print_str(" R="); print_int(rural);
    print_str(" "); print_int(year); print_str("\n");
    return sb.n_population - 1;
}

int sb_economic(int zone, int gdp, int primary, int secondary, int tertiary, int growth, int year) {
    if (sb.n_economic >= MAX_ECONOMIC) return -1;
    economic_t* e = &economics[sb.n_economic];
    e->econ_id = sb.n_economic;
    e->zone = zone;
    e->gdp = gdp;
    e->primary = primary;
    e->secondary = secondary;
    e->tertiary = tertiary;
    e->growth_rate = growth;
    e->year = year;
    e->active = 1;
    sb.total_gdp += gdp;
    sb.n_economic++;
    print_str("[SB] Econ "); print_int(sb.n_economic - 1);
    print_str(" zone="); print_int(zone);
    print_str(" gdp="); print_int(gdp);
    print_str(" growth="); print_int(growth); print_str("%\n");
    return sb.n_economic - 1;
}

int sb_social(int zone, int employed, int unemployed, int income, int consumption, int poverty, int year) {
    if (sb.n_social >= MAX_SOCIAL) return -1;
    social_t* s = &socials[sb.n_social];
    s->soc_id = sb.n_social;
    s->zone = zone;
    s->employed = employed;
    s->unemployed = unemployed;
    s->avg_income = income;
    s->avg_consumption = consumption;
    s->poverty_rate = poverty;
    s->year = year;
    s->active = 1;
    sb.total_employed += employed;
    sb.total_unemployed += unemployed;
    sb.n_social++;
    print_str("[SB] Social "); print_int(sb.n_social - 1);
    print_str(" zone="); print_int(zone);
    print_str(" emp="); print_int(employed);
    print_str(" unemp="); print_int(unemployed);
    print_str(" inc=$"); print_int(income);
    print_str(" pov="); print_int(poverty); print_str("%\n");
    return sb.n_social - 1;
}

int sb_agriculture(int zone, int grain, int meat, int milk, int farmland, int yield, int year) {
    if (sb.n_agriculture >= MAX_AGRICULTURE) return -1;
    agriculture_t* a = &agricultures[sb.n_agriculture];
    a->agri_id = sb.n_agriculture;
    a->zone = zone;
    a->grain_tons = grain;
    a->meat_tons = meat;
    a->milk_tons = milk;
    a->farmland_ha = farmland;
    a->yield_per_ha = yield;
    a->year = year;
    a->active = 1;
    sb.total_grain += grain;
    sb.total_farmland += farmland;
    sb.n_agriculture++;
    print_str("[SB] Agri "); print_int(sb.n_agriculture - 1);
    print_str(" zone="); print_int(zone);
    print_str(" grain="); print_int(grain);
    print_str(" meat="); print_int(meat);
    print_str(" farm="); print_int(farmland); print_str("ha\n");
    return sb.n_agriculture - 1;
}

int sb_add_survey(int type, int zone, int sample_size, int year) {
    if (sb.n_surveys >= MAX_SURVEYS) return -1;
    survey_t* sv = &surveys[sb.n_surveys];
    sv->survey_id = sb.n_surveys;
    sv->type = type;
    sv->zone = zone;
    sv->sample_size = sample_size;
    sv->response_rate = 0;
    sv->data_points = 0;
    sv->completed = 0;
    sv->year = year;
    sv->active = 1;
    sb.n_surveys++;
    print_str("[SB] Survey "); print_int(sb.n_surveys - 1);
    print_str(" type="); print_int(type);
    print_str(" zone="); print_int(zone);
    print_str(" sample="); print_int(sample_size);
    print_str(" "); print_int(year); print_str("\n");
    return sb.n_surveys - 1;
}

int sb_collect_data(int survey_id, int response_rate, int data_points) {
    if (survey_id >= sb.n_surveys) return -1;
    surveys[survey_id].response_rate = response_rate;
    surveys[survey_id].data_points = data_points;
    print_str("[SB] Collect Sv"); print_int(survey_id);
    print_str(" resp="); print_int(response_rate); print_str("%");
    print_str(" pts="); print_int(data_points); print_str("\n");
    return 0;
}

int sb_complete_survey(int survey_id) {
    if (survey_id >= sb.n_surveys) return -1;
    surveys[survey_id].completed = 1;
    sb.total_surveys_done++;
    print_str("[SB] Complete Sv"); print_int(survey_id); print_str("\n");
    return 0;
}

void sb_population_report(void) {
    print_str("[SB] Population report:\n");
    print_str("  Total population: "); print_int(sb.total_population); print_str("\n");
    for (int i = 0; i < sb.n_population; i++) {
        population_t* p = &populations[i];
        print_str("  Z"); print_int(p->zone);
        print_str(" pop="); print_int(p->total);
        print_str(" M="); print_int(p->male);
        print_str(" F="); print_int(p->female); print_str("\n");
    }
}

void sb_economic_report(void) {
    print_str("[SB] Economic report:\n");
    print_str("  Total GDP: "); print_int(sb.total_gdp); print_str("\n");
    print_str("  Regions: "); print_int(sb.n_economic); print_str("\n");
}

void sb_social_report(void) {
    print_str("[SB] Social report:\n");
    print_str("  Total employed: "); print_int(sb.total_employed); print_str("\n");
    print_str("  Total unemployed: "); print_int(sb.total_unemployed); print_str("\n");
}

void sb_agriculture_report(void) {
    print_str("[SB] Agriculture report:\n");
    print_str("  Total grain: "); print_int(sb.total_grain); print_str(" tons\n");
    print_str("  Total farmland: "); print_int(sb.total_farmland); print_str(" ha\n");
}

void sb_survey_report(void) {
    print_str("[SB] Survey report:\n");
    print_str("  Total surveys: "); print_int(sb.n_surveys); print_str("\n");
    print_str("  Completed: "); print_int(sb.total_surveys_done); print_str("\n");
}

void sb_print_state(void) {
    print_str("[SB] Pop="); print_int(sb.n_population);
    print_str(" Econ="); print_int(sb.n_economic);
    print_str(" Soc="); print_int(sb.n_social);
    print_str(" Agri="); print_int(sb.n_agriculture);
    print_str(" Surv="); print_int(sb.n_surveys);
    print_str("\n");
}

int main(void) {
    print_str("=== Statistics Bureau Demo ===\n\n");
    sb_init();

    print_str("Census data...\n");
    for (int i = 0; i < 18; i++) {
        int zone = (i % 4) + 1;
        int total = 50000 + (i * 10000);
        int male = total / 2 + (i * 100);
        int female = total - male;
        int urban = total * 7 / 10;
        int rural = total - urban;
        int year = 2020 + (i % 5);
        sb_census(zone, total, male, female, urban, rural, year);
    }

    print_str("\nEconomic data...\n");
    for (int i = 0; i < 14; i++) {
        int zone = (i % 4) + 1;
        int gdp = 1000000 + (i * 200000);
        int primary = gdp / 10;
        int secondary = gdp * 4 / 10;
        int tertiary = gdp - primary - secondary;
        int growth = 3 + (i % 5);
        int year = 2020 + (i % 5);
        sb_economic(zone, gdp, primary, secondary, tertiary, growth, year);
    }

    print_str("\nSocial data...\n");
    for (int i = 0; i < 12; i++) {
        int zone = (i % 4) + 1;
        int employed = 30000 + (i * 5000);
        int unemployed = 2000 + (i * 300);
        int income = 3000 + (i * 500);
        int consumption = income * 7 / 10;
        int poverty = 10 - (i % 5);
        int year = 2024;
        sb_social(zone, employed, unemployed, income, consumption, poverty, year);
    }

    print_str("\nAgriculture data...\n");
    for (int i = 0; i < 10; i++) {
        int zone = (i % 4) + 1;
        int grain = 100000 + (i * 30000);
        int meat = 20000 + (i * 5000);
        int milk = 30000 + (i * 8000);
        int farmland = 5000 + (i * 1000);
        int yield = grain / farmland;
        int year = 2024;
        sb_agriculture(zone, grain, meat, milk, farmland, yield, year);
    }

    print_str("\nAdding surveys...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int zone = (i % 4) + 1;
        int sample = 1000 + (i * 500);
        int year = 2024;
        sb_add_survey(type, zone, sample, year);
    }

    print_str("\nCollecting data...\n");
    for (int i = 0; i < 12; i++) {
        int rate = 70 + (i % 25);
        int pts = 5000 + (i * 1000);
        sb_collect_data(i, rate, pts);
    }

    print_str("\nCompleting surveys...\n");
    for (int i = 0; i < 12; i++) {
        sb_complete_survey(i);
    }

    print_str("\nPopulation report...\n");
    sb_population_report();

    print_str("\nEconomic report...\n");
    sb_economic_report();

    print_str("\nSocial report...\n");
    sb_social_report();

    print_str("\nAgriculture report...\n");
    sb_agriculture_report();

    print_str("\nSurvey report...\n");
    sb_survey_report();

    print_str("\nFinal state...\n");
    sb_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
