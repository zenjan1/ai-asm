/* meteorological_bureau: Meteorological administration system (v1.0)
 * Weather observation, forecasting, disaster warning, climate services, meteorological technology
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

#define MAX_OBSERVATION  16
#define MAX_FORECAST     14
#define MAX_WARNING      12
#define MAX_CLIMATE      10
#define MAX_TECHNOLOGY   10

typedef struct {
    int    observation_id;
    int    station_type;
    int    region_id;
    int    stations_online;
    int    data_quality_score;
    int    observations_daily;
    int    year;
    int    active;
} observation_t;

typedef struct {
    int    forecast_id;
    int    forecast_type;
    int    lead_time;
    int    accuracy_score;
    int    forecasts_issued;
    int    warnings_corrected;
    int    year;
    int    active;
} forecast_t;

typedef struct {
    int    warning_id;
    int    disaster_type;
    int    severity_level;
    int    warnings_issued;
    int    lead_time_min;
    int    population_warned;
    int    year;
    int    active;
} warning_t;

typedef struct {
    int    climate_id;
    int    monitoring_type;
    int    assessment_period;
    int    indicators_monitored;
    int    anomaly_detected;
    int    adaptation_projects;
    int    year;
    int    active;
} climate_t;

typedef struct {
    int    technology_id;
    int    research_area;
    int    project_type;
    int    projects_funded;
    int    patents_granted;
    int    international_cooperation;
    int    year;
    int    active;
} technology_t;

typedef struct {
    int    n_observation;
    int    n_forecast;
    int    n_warning;
    int    n_climate;
    int    n_technology;
    int    total_stations;
    int    total_forecasts;
    int    total_warnings;
    int    total_indicators;
    int    total_patents;
} mb_state_t;

static observation_t observations[MAX_OBSERVATION];
static forecast_t forecasts[MAX_FORECAST];
static warning_t warnings[MAX_WARNING];
static climate_t climates[MAX_CLIMATE];
static technology_t technologies[MAX_TECHNOLOGY];
static mb_state_t mb;

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

int mb_init(void) {
    if (initialized) return -1;
    mb.n_observation = 0; mb.n_forecast = 0; mb.n_warning = 0;
    mb.n_climate = 0; mb.n_technology = 0;
    mb.total_stations = 0; mb.total_forecasts = 0;
    mb.total_warnings = 0; mb.total_indicators = 0;
    mb.total_patents = 0;
    for (int i = 0; i < MAX_OBSERVATION; i++) observations[i].active = 0;
    for (int i = 0; i < MAX_FORECAST; i++) forecasts[i].active = 0;
    for (int i = 0; i < MAX_WARNING; i++) warnings[i].active = 0;
    for (int i = 0; i < MAX_CLIMATE; i++) climates[i].active = 0;
    for (int i = 0; i < MAX_TECHNOLOGY; i++) technologies[i].active = 0;
    initialized = 1;
    print_str("[MB] Meteorological bureau initialized\n");
    return 0;
}

int mb_observation(int stn_type, int region, int stations, int quality, int daily_obs, int year) {
    if (mb.n_observation >= MAX_OBSERVATION) return -1;
    observation_t* o = &observations[mb.n_observation];
    o->observation_id = mb.n_observation;
    o->station_type = stn_type;
    o->region_id = region;
    o->stations_online = stations;
    o->data_quality_score = quality;
    o->observations_daily = daily_obs;
    o->year = year;
    o->active = 1;
    mb.total_stations += stations;
    mb.n_observation++;
    print_str("[MB] Observation "); print_int(mb.n_observation - 1);
    print_str(" type="); print_int(stn_type);
    print_str(" rgn="); print_int(region);
    print_str(" stn="); print_int(stations);
    print_str(" qual="); print_int(quality);
    print_str(" dly="); print_int(daily_obs); print_str("\n");
    return mb.n_observation - 1;
}

int mb_forecast(int fc_type, int lead, int accuracy, int issued, int corrected, int year) {
    if (mb.n_forecast >= MAX_FORECAST) return -1;
    forecast_t* f = &forecasts[mb.n_forecast];
    f->forecast_id = mb.n_forecast;
    f->forecast_type = fc_type;
    f->lead_time = lead;
    f->accuracy_score = accuracy;
    f->forecasts_issued = issued;
    f->warnings_corrected = corrected;
    f->year = year;
    f->active = 1;
    mb.total_forecasts += issued;
    mb.n_forecast++;
    print_str("[MB] Forecast "); print_int(mb.n_forecast - 1);
    print_str(" type="); print_int(fc_type);
    print_str(" lead="); print_int(lead);
    print_str(" acc="); print_int(accuracy); print_str("%");
    print_str(" iss="); print_int(issued);
    print_str(" cor="); print_int(corrected); print_str("\n");
    return mb.n_forecast - 1;
}

int mb_warning(int dis_type, int severity, int issued, int lead_min, int population, int year) {
    if (mb.n_warning >= MAX_WARNING) return -1;
    warning_t* w = &warnings[mb.n_warning];
    w->warning_id = mb.n_warning;
    w->disaster_type = dis_type;
    w->severity_level = severity;
    w->warnings_issued = issued;
    w->lead_time_min = lead_min;
    w->population_warned = population;
    w->year = year;
    w->active = 1;
    mb.total_warnings += issued;
    mb.n_warning++;
    print_str("[MB] Warning "); print_int(mb.n_warning - 1);
    print_str(" type="); print_int(dis_type);
    print_str(" sev="); print_int(severity);
    print_str(" iss="); print_int(issued);
    print_str(" lead="); print_int(lead_min); print_str("m");
    print_str(" pop="); print_int(population); print_str("\n");
    return mb.n_warning - 1;
}

int mb_climate(int mon_type, int period, int indicators, int anomaly, int adaptation, int year) {
    if (mb.n_climate >= MAX_CLIMATE) return -1;
    climate_t* c = &climates[mb.n_climate];
    c->climate_id = mb.n_climate;
    c->monitoring_type = mon_type;
    c->assessment_period = period;
    c->indicators_monitored = indicators;
    c->anomaly_detected = anomaly;
    c->adaptation_projects = adaptation;
    c->year = year;
    c->active = 1;
    mb.total_indicators += indicators;
    mb.n_climate++;
    print_str("[MB] Climate "); print_int(mb.n_climate - 1);
    print_str(" type="); print_int(mon_type);
    print_str(" per="); print_int(period);
    print_str(" ind="); print_int(indicators);
    print_str(" anom="); print_int(anomaly);
    print_str(" adp="); print_int(adaptation); print_str("\n");
    return mb.n_climate - 1;
}

int mb_technology(int res_area, int proj_type, int funded, int patents, int intl_coop, int year) {
    if (mb.n_technology >= MAX_TECHNOLOGY) return -1;
    technology_t* t = &technologies[mb.n_technology];
    t->technology_id = mb.n_technology;
    t->research_area = res_area;
    t->project_type = proj_type;
    t->projects_funded = funded;
    t->patents_granted = patents;
    t->international_cooperation = intl_coop;
    t->year = year;
    t->active = 1;
    mb.total_patents += patents;
    mb.n_technology++;
    print_str("[MB] Technology "); print_int(mb.n_technology - 1);
    print_str(" area="); print_int(res_area);
    print_str(" type="); print_int(proj_type);
    print_str(" fnd="); print_int(funded);
    print_str(" pat="); print_int(patents);
    print_str(" int="); print_int(intl_coop); print_str("\n");
    return mb.n_technology - 1;
}

void mb_observation_report(void) {
    print_str("[MB] Observation report:\n");
    print_str("  Station networks: "); print_int(mb.n_observation); print_str("\n");
    print_str("  Total stations online: "); print_int(mb.total_stations); print_str("\n");
}

void mb_forecast_report(void) {
    print_str("[MB] Forecast report:\n");
    print_str("  Forecast types: "); print_int(mb.n_forecast); print_str("\n");
    print_str("  Total forecasts issued: "); print_int(mb.total_forecasts); print_str("\n");
}

void mb_warning_report(void) {
    print_str("[MB] Warning report:\n");
    print_str("  Disaster warnings: "); print_int(mb.n_warning); print_str("\n");
    print_str("  Total warnings issued: "); print_int(mb.total_warnings); print_str("\n");
    print_str("  Climate monitoring: "); print_int(mb.n_climate); print_str("\n");
    print_str("  Total indicators: "); print_int(mb.total_indicators); print_str("\n");
    print_str("  Technology projects: "); print_int(mb.n_technology); print_str("\n");
    print_str("  Total patents: "); print_int(mb.total_patents); print_str("\n");
}

void mb_print_state(void) {
    print_str("[MB] Ob="); print_int(mb.n_observation);
    print_str(" Fc="); print_int(mb.n_forecast);
    print_str(" Wn="); print_int(mb.n_warning);
    print_str(" Cl="); print_int(mb.n_climate);
    print_str(" Tc="); print_int(mb.n_technology);
    print_str("\n");
}

int main(void) {
    print_str("=== Meteorological Bureau Demo ===\n\n");
    mb_init();

    print_str("Weather observation...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int rgn = (i % 8) + 1;
        int stn = 50 + (i * 20);
        int qual = 85 + (i % 15);
        int dly = 1000 + (i * 500);
        int year = 2020 + (i % 5);
        mb_observation(type, rgn, stn, qual, dly, year);
    }

    print_str("\nWeather forecasting...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int lead = 1 + (i % 7);
        int acc = 80 + (i * 2);
        int iss = 500 + (i * 100);
        int cor = iss / 20;
        int year = 2021 + (i % 4);
        mb_forecast(type, lead, acc, iss, cor, year);
    }

    print_str("\nDisaster warnings...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 6) + 1;
        int sev = (i % 4) + 1;
        int iss = 100 + (i * 30);
        int lead = 30 + (i * 10);
        int pop = 10000 + (i * 5000);
        int year = 2022 + (i % 3);
        mb_warning(type, sev, iss, lead, pop, year);
    }

    print_str("\nClimate services...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int per = (i % 5) + 1;
        int ind = 20 + (i * 5);
        int anom = 2 + (i % 4);
        int adp = 3 + (i % 5);
        int year = 2023 + (i % 2);
        mb_climate(type, per, ind, anom, adp, year);
    }

    print_str("\nMeteorological technology...\n");
    for (int i = 0; i < 10; i++) {
        int area = (i % 6) + 1;
        int type = (i % 4) + 1;
        int fnd = 10 + (i * 3);
        int pat = 5 + (i * 2);
        int intl = 2 + (i % 4);
        int year = 2024;
        mb_technology(area, type, fnd, pat, intl, year);
    }

    print_str("\nObservation report...\n");
    mb_observation_report();

    print_str("\nForecast report...\n");
    mb_forecast_report();

    print_str("\nWarning report...\n");
    mb_warning_report();

    print_str("\nFinal state...\n");
    mb_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
