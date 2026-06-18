/* meteorological_bureau: Weather and climate management (v1.0)
 * Weather stations, forecasts, warnings, climate monitoring
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

#define MAX_STATIONS   14
#define MAX_FORECASTS  12
#define MAX_WARNINGS   16
#define MAX_CLIMATE    10
#define MAX_AVIATION   8

typedef struct {
    int    station_id;
    int    zone;
    int    temperature;
    int    humidity;
    int    pressure;
    int    wind_speed;
    int    rainfall;
    int    year;
    int    month;
    int    day;
    int    hour;
    int    active;
} station_t;

typedef struct {
    int    forecast_id;
    int    zone;
    int    type;
    int    temp_high;
    int    temp_low;
    int    precipitation;
    int    wind;
    int    validity_days;
    int    issued_hour;
    int    active;
} forecast_t;

typedef struct {
    int    warn_id;
    int    zone;
    int    type;
    int    severity;
    int    issued_hour;
    int    expires_hour;
    int    affected;
    int    active;
} warning_t;

typedef struct {
    int    climate_id;
    int    zone;
    int    avg_temp;
    int    avg_rainfall;
    int    anomaly;
    int    year;
    int    active;
} climate_t;

typedef struct {
    int    aviation_id;
    int    airport_id;
    int    visibility;
    int    ceiling;
    int    wind_dir;
    int    wind_speed;
    int    weather;
    int    issued_hour;
    int    active;
} aviation_t;

typedef struct {
    int    n_stations;
    int    n_forecasts;
    int    n_warnings;
    int    n_climate;
    int    n_aviation;
    int    total_readings;
    int    total_warnings_issued;
    int    total_warnings_active;
    int    max_temp;
    int    min_temp;
    int    max_wind;
    int    max_rainfall;
} mb_state_t;

static station_t stations[MAX_STATIONS];
static forecast_t forecasts[MAX_FORECASTS];
static warning_t warnings[MAX_WARNINGS];
static climate_t climates[MAX_CLIMATE];
static aviation_t aviation[MAX_AVIATION];
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
    mb.n_stations = 0; mb.n_forecasts = 0; mb.n_warnings = 0;
    mb.n_climate = 0; mb.n_aviation = 0;
    mb.total_readings = 0; mb.total_warnings_issued = 0;
    mb.total_warnings_active = 0;
    mb.max_temp = -100; mb.min_temp = 100; mb.max_wind = 0; mb.max_rainfall = 0;
    for (int i = 0; i < MAX_STATIONS; i++) stations[i].active = 0;
    for (int i = 0; i < MAX_FORECASTS; i++) forecasts[i].active = 0;
    for (int i = 0; i < MAX_WARNINGS; i++) warnings[i].active = 0;
    for (int i = 0; i < MAX_CLIMATE; i++) climates[i].active = 0;
    for (int i = 0; i < MAX_AVIATION; i++) aviation[i].active = 0;
    initialized = 1;
    print_str("[MB] Meteorological bureau initialized\n");
    return 0;
}

int mb_add_station(int zone) {
    if (mb.n_stations >= MAX_STATIONS) return -1;
    station_t* s = &stations[mb.n_stations];
    s->station_id = mb.n_stations;
    s->zone = zone;
    s->temperature = 0;
    s->humidity = 0;
    s->pressure = 1013;
    s->wind_speed = 0;
    s->rainfall = 0;
    s->year = 2024;
    s->month = 1;
    s->day = 1;
    s->hour = 0;
    s->active = 1;
    mb.n_stations++;
    print_str("[MB] Station "); print_int(mb.n_stations - 1);
    print_str(" zone="); print_int(zone); print_str("\n");
    return mb.n_stations - 1;
}

int mb_record_reading(int station_id, int temp, int humidity, int pressure, int wind, int rain, int hour) {
    if (station_id >= mb.n_stations) return -1;
    station_t* s = &stations[station_id];
    s->temperature = temp;
    s->humidity = humidity;
    s->pressure = pressure;
    s->wind_speed = wind;
    s->rainfall = rain;
    s->hour = hour;
    mb.total_readings++;
    if (temp > mb.max_temp) mb.max_temp = temp;
    if (temp < mb.min_temp) mb.min_temp = temp;
    if (wind > mb.max_wind) mb.max_wind = wind;
    if (rain > mb.max_rainfall) mb.max_rainfall = rain;
    print_str("[MB] St"); print_int(station_id);
    print_str(" T="); print_int(temp); print_str("C");
    print_str(" H="); print_int(humidity); print_str("%");
    print_str(" P="); print_int(pressure);
    print_str(" W="); print_int(wind);
    print_str(" R="); print_int(rain); print_str("mm\n");
    return 0;
}

int mb_issue_forecast(int zone, int type, int high, int low, int precip, int wind, int validity, int hour) {
    if (mb.n_forecasts >= MAX_FORECASTS) return -1;
    forecast_t* f = &forecasts[mb.n_forecasts];
    f->forecast_id = mb.n_forecasts;
    f->zone = zone;
    f->type = type;
    f->temp_high = high;
    f->temp_low = low;
    f->precipitation = precip;
    f->wind = wind;
    f->validity_days = validity;
    f->issued_hour = hour;
    f->active = 1;
    mb.n_forecasts++;
    print_str("[MB] Forecast "); print_int(mb.n_forecasts - 1);
    print_str(" zone="); print_int(zone);
    print_str(" type="); print_int(type);
    print_str(" "); print_int(low); print_str("-"); print_int(high); print_str("C");
    print_str(" P="); print_int(precip); print_str("mm\n");
    return mb.n_forecasts - 1;
}

int mb_issue_warning(int zone, int type, int severity, int issued, int expires, int affected) {
    if (mb.n_warnings >= MAX_WARNINGS) return -1;
    warning_t* w = &warnings[mb.n_warnings];
    w->warn_id = mb.n_warnings;
    w->zone = zone;
    w->type = type;
    w->severity = severity;
    w->issued_hour = issued;
    w->expires_hour = expires;
    w->affected = affected;
    w->active = 1;
    mb.total_warnings_issued++;
    mb.total_warnings_active++;
    mb.n_warnings++;
    print_str("[MB] Warning "); print_int(mb.n_warnings - 1);
    print_str(" zone="); print_int(zone);
    print_str(" type="); print_int(type);
    print_str(" sev="); print_int(severity);
    print_str(" affected="); print_int(affected); print_str("\n");
    return mb.n_warnings - 1;
}

int mb_expire_warning(int warn_id) {
    if (warn_id >= mb.n_warnings) return -1;
    if (warnings[warn_id].active) {
        warnings[warn_id].active = 0;
        mb.total_warnings_active--;
    }
    print_str("[MB] Expire W"); print_int(warn_id); print_str("\n");
    return 0;
}

int mb_record_climate(int zone, int avg_temp, int avg_rain, int anomaly, int year) {
    if (mb.n_climate >= MAX_CLIMATE) return -1;
    climate_t* c = &climates[mb.n_climate];
    c->climate_id = mb.n_climate;
    c->zone = zone;
    c->avg_temp = avg_temp;
    c->avg_rainfall = avg_rain;
    c->anomaly = anomaly;
    c->year = year;
    c->active = 1;
    mb.n_climate++;
    print_str("[MB] Climate "); print_int(mb.n_climate - 1);
    print_str(" zone="); print_int(zone);
    print_str(" T="); print_int(avg_temp); print_str("C");
    print_str(" R="); print_int(avg_rain); print_str("mm");
    print_str(" anom="); print_int(anomaly);
    print_str(" "); print_int(year); print_str("\n");
    return mb.n_climate - 1;
}

int mb_add_aviation(int airport, int vis, int ceiling, int wind_dir, int wind_spd, int weather, int hour) {
    if (mb.n_aviation >= MAX_AVIATION) return -1;
    aviation_t* a = &aviation[mb.n_aviation];
    a->aviation_id = mb.n_aviation;
    a->airport_id = airport;
    a->visibility = vis;
    a->ceiling = ceiling;
    a->wind_dir = wind_dir;
    a->wind_speed = wind_spd;
    a->weather = weather;
    a->issued_hour = hour;
    a->active = 1;
    mb.n_aviation++;
    print_str("[MB] Aviation "); print_int(mb.n_aviation - 1);
    print_str(" ap="); print_int(airport);
    print_str(" vis="); print_int(vis);
    print_str(" ceil="); print_int(ceiling);
    print_str(" W"); print_int(wind_dir); print_str("/"); print_int(wind_spd); print_str("\n");
    return mb.n_aviation - 1;
}

void mb_observation_report(void) {
    print_str("[MB] Observations:\n");
    print_str("  Stations: "); print_int(mb.n_stations); print_str("\n");
    print_str("  Readings: "); print_int(mb.total_readings); print_str("\n");
    print_str("  Max temp: "); print_int(mb.max_temp); print_str("C\n");
    print_str("  Min temp: "); print_int(mb.min_temp); print_str("C\n");
    print_str("  Max wind: "); print_int(mb.max_wind); print_str("km/h\n");
    print_str("  Max rain: "); print_int(mb.max_rainfall); print_str("mm\n");
}

void mb_warning_report(void) {
    print_str("[MB] Warnings:\n");
    print_str("  Issued: "); print_int(mb.total_warnings_issued); print_str("\n");
    print_str("  Active: "); print_int(mb.total_warnings_active); print_str("\n");
}

void mb_print_state(void) {
    print_str("[MB] Stn="); print_int(mb.n_stations);
    print_str(" Fcst="); print_int(mb.n_forecasts);
    print_str(" Warn="); print_int(mb.n_warnings);
    print_str(" Clim="); print_int(mb.n_climate);
    print_str(" Avia="); print_int(mb.n_aviation);
    print_str("\n");
}

int main(void) {
    print_str("=== Meteorological Bureau Demo ===\n\n");
    mb_init();

    print_str("Adding weather stations...\n");
    for (int i = 0; i < 14; i++) {
        mb_add_station((i % 4) + 1);
    }

    print_str("\nRecording readings...\n");
    for (int i = 0; i < 28; i++) {
        int st = i % 14;
        int temp = -5 + (i * 3);
        int humidity = 40 + (i % 50);
        int pressure = 1000 + (i % 30);
        int wind = 5 + (i * 4);
        int rain = i % 20;
        int hour = i % 24;
        mb_record_reading(st, temp, humidity, pressure, wind, rain, hour);
    }

    print_str("\nIssuing forecasts...\n");
    for (int i = 0; i < 12; i++) {
        int zone = (i % 4) + 1;
        int type = (i % 3) + 1;
        int high = 20 + (i * 2);
        int low = high - 10;
        int precip = i * 5;
        int wind = 10 + (i * 3);
        int validity = 1 + (i % 5);
        int hour = i * 2;
        mb_issue_forecast(zone, type, high, low, precip, wind, validity, hour);
    }

    print_str("\nIssuing warnings...\n");
    for (int i = 0; i < 16; i++) {
        int zone = (i % 4) + 1;
        int type = (i % 5) + 1;
        int severity = (i % 4) + 1;
        int issued = 6 + i;
        int expires = issued + 12;
        int affected = 1000 + (i * 500);
        mb_issue_warning(zone, type, severity, issued, expires, affected);
    }

    print_str("\nExpiring warnings...\n");
    for (int i = 0; i < 10; i++) {
        mb_expire_warning(i);
    }

    print_str("\nRecording climate data...\n");
    for (int i = 0; i < 10; i++) {
        int zone = (i % 4) + 1;
        int avg_temp = 10 + (i * 2);
        int avg_rain = 500 + (i * 100);
        int anomaly = -2 + (i % 5);
        int year = 2015 + i;
        mb_record_climate(zone, avg_temp, avg_rain, anomaly, year);
    }

    print_str("\nAdding aviation weather...\n");
    for (int i = 0; i < 8; i++) {
        int airport = 10 + i;
        int vis = 5000 + (i * 1000);
        int ceiling = 2000 + (i * 500);
        int wind_dir = (i * 45) % 360;
        int wind_spd = 10 + (i * 5);
        int weather = (i % 4) + 1;
        int hour = 6 + (i * 2);
        mb_add_aviation(airport, vis, ceiling, wind_dir, wind_spd, weather, hour);
    }

    print_str("\nObservation report...\n");
    mb_observation_report();

    print_str("\nWarning report...\n");
    mb_warning_report();

    print_str("\nFinal state...\n");
    mb_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
