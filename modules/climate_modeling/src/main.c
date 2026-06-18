/* climate_modeling: Climate modeling system (v1.0)
 * Weather prediction, climate trends, extreme events, emissions, impact assessment
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

#define MAX_STATIONS   8
#define MAX_PERIODS    12
#define MAX_EVENTS     8
#define MAX_EMISSIONS  8
#define MAX_IMPACTS    8

typedef struct {
    double temperature;
    double precipitation;
    double humidity;
    double wind_speed;
    double pressure;
    double x, y;
    int    station_id;
    int    active;
} weather_station_t;

typedef struct {
    double temp_anomaly[MAX_PERIODS];
    double precip_anomaly[MAX_PERIODS];
    double co2_level[MAX_PERIODS];
    double trend_temp;
    double trend_precip;
    int    n_periods;
    int    active;
} climate_trend_t;

typedef struct {
    int    event_type;
    double severity;
    double probability;
    double affected_area;
    int    duration_days;
    int    warning_level;
    int    id;
    int    active;
} extreme_event_t;

typedef struct {
    double co2_emission;
    double ch4_emission;
    double n2o_emission;
    double total_co2e;
    double reduction_target;
    double reduction_achieved;
    int    sector;
    int    id;
    int    active;
} emission_t;

typedef struct {
    int    impact_type;
    double magnitude;
    double confidence;
    double affected_area;
    int    time_horizon;
    int    id;
    int    active;
} impact_t;

typedef struct {
    int    n_stations;
    int    n_trends;
    int    n_events;
    int    n_emissions;
    int    n_impacts;
    double avg_temp;
    double avg_precip;
    double total_emission;
    int    warning_count;
} cm_state_t;

static weather_station_t stations[MAX_STATIONS];
static climate_trend_t trends;
static extreme_event_t events[MAX_EVENTS];
static emission_t emissions[MAX_EMISSIONS];
static impact_t impacts[MAX_IMPACTS];
static cm_state_t cm;

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
static double dabs(double x) { return x < 0 ? -x : x; }
static double dsin(double x) {
    double sum = x, term = x, x2 = x * x;
    for (int i = 1; i < 15; i++) { term *= -x2 / ((2*i)*(2*i+1)); sum += term; }
    return sum;
}

int cm_init(void) {
    if (initialized) return -1;
    cm.n_stations = 0; cm.n_trends = 0; cm.n_events = 0;
    cm.n_emissions = 0; cm.n_impacts = 0;
    cm.avg_temp = 0.0; cm.avg_precip = 0.0;
    cm.total_emission = 0.0; cm.warning_count = 0;
    for (int i = 0; i < MAX_STATIONS; i++) stations[i].active = 0;
    for (int i = 0; i < MAX_EVENTS; i++) events[i].active = 0;
    for (int i = 0; i < MAX_EMISSIONS; i++) emissions[i].active = 0;
    for (int i = 0; i < MAX_IMPACTS; i++) impacts[i].active = 0;
    trends.active = 0;
    initialized = 1;
    print_str("[CM] Climate modeling initialized\n");
    return 0;
}

int cm_add_station(double temp, double precip, double humidity, double x, double y) {
    if (cm.n_stations >= MAX_STATIONS) return -1;
    weather_station_t* s = &stations[cm.n_stations];
    s->temperature = temp; s->precipitation = precip;
    s->humidity = humidity;
    s->wind_speed = 5.0 + (double)(cm.n_stations % 4) * 2.0;
    s->pressure = 1013.0 + (double)(cm.n_stations % 3) * 2.0;
    s->x = x; s->y = y;
    s->station_id = cm.n_stations; s->active = 1;
    cm.n_stations++;
    print_str("[CM] Station "); print_int(cm.n_stations - 1);
    print_str(" T="); print_int((int)(temp * 10)); print_str("C");
    print_str(" P="); print_int((int)precip); print_str("mm");
    print_str(" RH="); print_int((int)humidity); print_str("%");
    print_str(" wind="); print_int((int)s->wind_speed); print_str("m/s\n");
    return cm.n_stations - 1;
}

void cm_weather_forecast(int station_id, int days) {
    if (station_id >= cm.n_stations) return;
    weather_station_t* s = &stations[station_id];
    print_str("[CM] Weather forecast station "); print_int(station_id);
    print_str(" ("); print_int(days); print_str(" days):\n");
    for (int i = 0; i < days && i < 7; i++) {
        double temp_forecast = s->temperature + dsin(2.0 * 3.14159 * i / 7.0) * 3.0;
        double precip_forecast = s->precipitation * (0.8 + (double)(i % 3) * 0.2);
        print_str("  Day "); print_int(i + 1);
        print_str(" T="); print_int((int)(temp_forecast * 10)); print_str("C");
        print_str(" P="); print_int((int)precip_forecast); print_str("mm\n");
    }
}

void cm_climate_trend_analysis(void) {
    trends.n_periods = MAX_PERIODS;
    double sum_temp = 0.0, sum_precip = 0.0;
    for (int i = 0; i < MAX_PERIODS; i++) {
        trends.temp_anomaly[i] = 0.5 + (double)i * 0.1;
        trends.precip_anomaly[i] = -20.0 + (double)(i % 4) * 10.0;
        trends.co2_level[i] = 400.0 + (double)i * 2.5;
        sum_temp += trends.temp_anomaly[i];
        sum_precip += trends.precip_anomaly[i];
    }
    trends.trend_temp = sum_temp / MAX_PERIODS;
    trends.trend_precip = sum_precip / MAX_PERIODS;
    trends.active = 1;
    cm.n_trends = 1;
    print_str("[CM] Climate trend analysis:\n");
    print_str("  Temp trend: +"); print_int((int)(trends.trend_temp * 10)); print_str("C/decade\n");
    print_str("  Precip trend: "); print_int((int)trends.trend_precip); print_str("mm/decade\n");
    print_str("  CO2 trend: +"); print_int((int)(trends.co2_level[MAX_PERIODS - 1] - trends.co2_level[0])); print_str("ppm\n");
}

int cm_predict_extreme_event(int type, double severity, double probability) {
    if (cm.n_events >= MAX_EVENTS) return -1;
    extreme_event_t* e = &events[cm.n_events];
    e->event_type = type; e->severity = severity;
    e->probability = probability;
    e->affected_area = severity * 100.0;
    e->duration_days = (int)(severity * 3.0);
    e->warning_level = probability > 0.7 ? 3 : (probability > 0.4 ? 2 : 1);
    if (e->warning_level >= 2) cm.warning_count++;
    e->id = cm.n_events; e->active = 1;
    cm.n_events++;
    print_str("[CM] Extreme event "); print_int(cm.n_events - 1);
    print_str(" type="); print_int(type);
    print_str(" severity="); print_int((int)(severity * 10));
    print_str(" prob="); print_int((int)(probability * 100)); print_str("%");
    print_str(" warning="); print_int(e->warning_level); print_str("\n");
    return cm.n_events - 1;
}

int cm_add_emission(int sector, double co2, double ch4, double n2o) {
    if (cm.n_emissions >= MAX_EMISSIONS) return -1;
    emission_t* e = &emissions[cm.n_emissions];
    e->sector = sector;
    e->co2_emission = co2;
    e->ch4_emission = ch4;
    e->n2o_emission = n2o;
    e->total_co2e = co2 + ch4 * 25.0 + n2o * 298.0;
    e->reduction_target = e->total_co2e * 0.3;
    e->reduction_achieved = e->total_co2e * 0.15;
    e->id = cm.n_emissions; e->active = 1;
    cm.n_emissions++;
    cm.total_emission += e->total_co2e;
    print_str("[CM] Emission sector "); print_int(sector);
    print_str(" CO2="); print_int((int)co2);
    print_str(" CH4="); print_int((int)ch4);
    print_str(" N2O="); print_int((int)n2o);
    print_str(" total="); print_int((int)e->total_co2e); print_str("tCO2e\n");
    return cm.n_emissions - 1;
}

void cm_emission_tracking(void) {
    print_str("[CM] Emission tracking:\n");
    print_str("  Total emissions: "); print_int((int)cm.total_emission); print_str("tCO2e\n");
    double total_target = 0.0, total_achieved = 0.0;
    for (int i = 0; i < cm.n_emissions; i++) {
        total_target += emissions[i].reduction_target;
        total_achieved += emissions[i].reduction_achieved;
    }
    print_str("  Reduction target: "); print_int((int)total_target); print_str("tCO2e\n");
    print_str("  Reduction achieved: "); print_int((int)total_achieved); print_str("tCO2e\n");
    print_str("  Progress: "); print_int((int)(total_achieved * 100.0 / (total_target + 1e-10))); print_str("%\n");
}

int cm_assess_impact(int type, double magnitude, int time_horizon) {
    if (cm.n_impacts >= MAX_IMPACTS) return -1;
    impact_t* imp = &impacts[cm.n_impacts];
    imp->impact_type = type;
    imp->magnitude = magnitude;
    imp->confidence = 70.0 + (double)(cm.n_impacts % 4) * 5.0;
    imp->affected_area = magnitude * 50.0;
    imp->time_horizon = time_horizon;
    imp->id = cm.n_impacts; imp->active = 1;
    cm.n_impacts++;
    print_str("[CM] Impact assessment "); print_int(cm.n_impacts - 1);
    print_str(" type="); print_int(type);
    print_str(" magnitude="); print_int((int)(magnitude * 10));
    print_str(" confidence="); print_int((int)imp->confidence);
    print_str(" horizon="); print_int(time_horizon); print_str("yr\n");
    return cm.n_impacts - 1;
}

void cm_impact_summary(void) {
    print_str("[CM] Impact summary:\n");
    for (int i = 0; i < cm.n_impacts; i++) {
        print_str("  Impact "); print_int(i);
        print_str(" type="); print_int(impacts[i].impact_type);
        print_str(" mag="); print_int((int)(impacts[i].magnitude * 10));
        print_str(" area="); print_int((int)impacts[i].affected_area); print_str("km2\n");
    }
}

void cm_print_state(void) {
    double sum_temp = 0.0, sum_precip = 0.0;
    for (int i = 0; i < cm.n_stations; i++) {
        sum_temp += stations[i].temperature;
        sum_precip += stations[i].precipitation;
    }
    cm.avg_temp = sum_temp / (cm.n_stations + 1e-10);
    cm.avg_precip = sum_precip / (cm.n_stations + 1e-10);
    print_str("[CM] Stations="); print_int(cm.n_stations);
    print_str(" Events="); print_int(cm.n_events);
    print_str(" Emissions="); print_int(cm.n_emissions);
    print_str(" Impacts="); print_int(cm.n_impacts);
    print_str(" AvgT="); print_int((int)(cm.avg_temp * 10)); print_str("C");
    print_str(" Warnings="); print_int(cm.warning_count); print_str("\n");
}

int main(void) {
    print_str("=== Climate Modeling System Demo ===\n\n");
    cm_init();

    print_str("Adding weather stations...\n");
    cm_add_station(15.0, 80.0, 65.0, 0.0, 0.0);
    cm_add_station(18.5, 120.0, 70.0, 10.0, 5.0);
    cm_add_station(12.0, 60.0, 75.0, 20.0, 0.0);
    cm_add_station(22.0, 150.0, 80.0, 10.0, 10.0);
    cm_add_station(10.0, 50.0, 60.0, 0.0, 10.0);

    print_str("\nWeather forecasts...\n");
    cm_weather_forecast(0, 5);
    cm_weather_forecast(1, 7);
    cm_weather_forecast(3, 3);

    print_str("\nClimate trend analysis...\n");
    cm_climate_trend_analysis();

    print_str("\nExtreme event prediction...\n");
    cm_predict_extreme_event(1, 0.7, 0.6);
    cm_predict_extreme_event(2, 0.9, 0.4);
    cm_predict_extreme_event(3, 0.5, 0.8);
    cm_predict_extreme_event(4, 0.8, 0.5);

    print_str("\nEmission tracking...\n");
    cm_add_emission(1, 5000.0, 200.0, 10.0);
    cm_add_emission(2, 3000.0, 150.0, 8.0);
    cm_add_emission(3, 2000.0, 100.0, 5.0);
    cm_add_emission(4, 4000.0, 180.0, 12.0);
    cm_emission_tracking();

    print_str("\nImpact assessments...\n");
    cm_assess_impact(1, 0.8, 20);
    cm_assess_impact(2, 0.6, 30);
    cm_assess_impact(3, 0.9, 50);
    cm_assess_impact(4, 0.7, 40);
    cm_impact_summary();

    cm_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
