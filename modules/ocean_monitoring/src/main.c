/* ocean_monitoring: Ocean monitoring system (v1.0)
 * Water quality, marine ecology, tides/currents, pollution, deep sea exploration
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
#define MAX_SENSORS    16
#define MAX_SPECIES    8
#define MAX_POLLUTION  8
#define MAX_DEPTH      8

typedef struct {
    double temperature;
    double salinity;
    double ph;
    double dissolved_oxygen;
    double turbidity;
    double chlorophyll;
    double x, y;
    int    station_id;
    int    active;
} station_t;

typedef struct {
    int    type;
    double density;
    double biomass;
    double diversity_index;
    double health_score;
    int    location_x;
    int    location_y;
    int    id;
    int    active;
} species_t;

typedef struct {
    double tide_height;
    double tide_period;
    double current_speed;
    double current_direction;
    double wave_height;
    double wave_period;
    int    station_id;
    int    active;
} tide_current_t;

typedef struct {
    int    type;
    double concentration;
    double threshold;
    double spread_radius;
    int    severity;
    int    detected;
    int    id;
    int    active;
} pollution_t;

typedef struct {
    double depth;
    double pressure;
    double temperature;
    int    terrain_type;
    double biodiversity;
    int    explored;
    int    id;
    int    active;
} deep_sea_t;

typedef struct {
    int    n_stations;
    int    n_sensors;
    int    n_species;
    int    n_pollution;
    int    n_deep_sea;
    double avg_temp;
    double avg_ph;
    double avg_do;
    int    pollution_alerts;
} om_state_t;

static station_t stations[MAX_STATIONS];
static species_t species[MAX_SPECIES];
static tide_current_t tides[MAX_STATIONS];
static pollution_t pollutions[MAX_POLLUTION];
static deep_sea_t deep_seas[MAX_DEPTH];
static om_state_t om;

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

int om_init(void) {
    if (initialized) return -1;
    om.n_stations = 0; om.n_sensors = 0; om.n_species = 0;
    om.n_pollution = 0; om.n_deep_sea = 0;
    om.avg_temp = 0.0; om.avg_ph = 0.0; om.avg_do = 0.0;
    om.pollution_alerts = 0;
    for (int i = 0; i < MAX_STATIONS; i++) stations[i].active = 0;
    for (int i = 0; i < MAX_SPECIES; i++) species[i].active = 0;
    for (int i = 0; i < MAX_STATIONS; i++) tides[i].active = 0;
    for (int i = 0; i < MAX_POLLUTION; i++) pollutions[i].active = 0;
    for (int i = 0; i < MAX_DEPTH; i++) deep_seas[i].active = 0;
    initialized = 1;
    print_str("[OM] Ocean monitoring initialized\n");
    return 0;
}

int om_add_station(double temp, double salinity, double ph, double x, double y) {
    if (om.n_stations >= MAX_STATIONS) return -1;
    station_t* s = &stations[om.n_stations];
    s->temperature = temp; s->salinity = salinity; s->ph = ph;
    s->dissolved_oxygen = 7.0 + (double)(om.n_stations % 4) * 0.5;
    s->turbidity = 2.0 + (double)(om.n_stations % 5) * 1.0;
    s->chlorophyll = 1.5 + (double)(om.n_stations % 3) * 0.5;
    s->x = x; s->y = y;
    s->station_id = om.n_stations; s->active = 1;
    om.n_stations++;
    print_str("[OM] Station "); print_int(om.n_stations - 1);
    print_str(" T="); print_int((int)(temp * 10)); print_str("C");
    print_str(" S="); print_int((int)(salinity * 10)); print_str("ppt");
    print_str(" pH="); print_int((int)(ph * 10));
    print_str(" DO="); print_int((int)(s->dissolved_oxygen * 10)); print_str("mg/L\n");
    return om.n_stations - 1;
}

void om_water_quality_analysis(void) {
    double sum_temp = 0.0, sum_ph = 0.0, sum_do = 0.0;
    for (int i = 0; i < om.n_stations; i++) {
        sum_temp += stations[i].temperature;
        sum_ph += stations[i].ph;
        sum_do += stations[i].dissolved_oxygen;
    }
    om.avg_temp = sum_temp / (om.n_stations + 1e-10);
    om.avg_ph = sum_ph / (om.n_stations + 1e-10);
    om.avg_do = sum_do / (om.n_stations + 1e-10);
    print_str("[OM] Water quality analysis:\n");
    print_str("  Avg T="); print_int((int)(om.avg_temp * 10)); print_str("C");
    print_str(" pH="); print_int((int)(om.avg_ph * 10));
    print_str(" DO="); print_int((int)(om.avg_do * 10)); print_str("mg/L\n");
    for (int i = 0; i < om.n_stations; i++) {
        int issues = 0;
        if (stations[i].ph < 7.5 || stations[i].ph > 8.5) issues++;
        if (stations[i].dissolved_oxygen < 6.0) issues++;
        if (stations[i].turbidity > 5.0) issues++;
        print_str("  Station "); print_int(i);
        print_str(" issues="); print_int(issues); print_str("\n");
    }
}

int om_add_species(int type, double density) {
    if (om.n_species >= MAX_SPECIES) return -1;
    species_t* sp = &species[om.n_species];
    sp->type = type; sp->density = density;
    sp->biomass = density * 100.0;
    sp->diversity_index = 2.5 + (double)(om.n_species % 4) * 0.3;
    sp->health_score = 80.0 + (double)(om.n_species % 3) * 5.0;
    sp->location_x = om.n_species * 20;
    sp->location_y = (om.n_species % 3) * 15;
    sp->id = om.n_species; sp->active = 1;
    om.n_species++;
    print_str("[OM] Species "); print_int(om.n_species - 1);
    print_str(" type="); print_int(type);
    print_str(" density="); print_int((int)density);
    print_str(" health="); print_int((int)sp->health_score); print_str("%\n");
    return om.n_species - 1;
}

void om_marine_ecology_assessment(void) {
    double total_biomass = 0.0;
    double avg_health = 0.0;
    for (int i = 0; i < om.n_species; i++) {
        total_biomass += species[i].biomass;
        avg_health += species[i].health_score;
    }
    avg_health /= (om.n_species + 1e-10);
    print_str("[OM] Marine ecology: biomass="); print_int((int)total_biomass);
    print_str(" health="); print_int((int)avg_health); print_str("%\n");
    for (int i = 0; i < om.n_species; i++) {
        print_str("  Species "); print_int(i);
        print_str(" type="); print_int(species[i].type);
        print_str(" biomass="); print_int((int)species[i].biomass);
        print_str(" diversity="); print_int((int)(species[i].diversity_index * 10)); print_str("\n");
    }
}

void om_tide_current_prediction(int station_id) {
    if (station_id >= om.n_stations) return;
    tide_current_t* t = &tides[station_id];
    t->tide_height = 1.5 + dsin(2.0 * 3.14159 * station_id / 12.0);
    t->tide_period = 12.42;
    t->current_speed = 0.5 + (double)(station_id % 4) * 0.2;
    t->current_direction = 45.0 + station_id * 30.0;
    t->wave_height = 1.0 + (double)(station_id % 3) * 0.5;
    t->wave_period = 8.0 + (double)(station_id % 4);
    t->station_id = station_id; t->active = 1;
    print_str("[OM] Tide prediction station "); print_int(station_id);
    print_str(" height="); print_int((int)(t->tide_height * 100)); print_str("cm");
    print_str(" current="); print_int((int)(t->current_speed * 100)); print_str("m/s");
    print_str(" dir="); print_int((int)t->current_direction); print_str("deg\n");
}

int om_detect_pollution(int type, double concentration, double threshold) {
    if (om.n_pollution >= MAX_POLLUTION) return -1;
    pollution_t* p = &pollutions[om.n_pollution];
    p->type = type; p->concentration = concentration;
    p->threshold = threshold;
    p->spread_radius = concentration * 0.1;
    p->severity = concentration > threshold * 2.0 ? 3 :
                  (concentration > threshold ? 2 : 1);
    p->detected = concentration > threshold ? 1 : 0;
    if (p->detected) om.pollution_alerts++;
    p->id = om.n_pollution; p->active = 1;
    om.n_pollution++;
    print_str("[OM] Pollution "); print_int(om.n_pollution - 1);
    print_str(" type="); print_int(type);
    print_str(" conc="); print_int((int)concentration);
    print_str(" thresh="); print_int((int)threshold);
    print_str(" severity="); print_int(p->severity);
    print_str(" detected="); print_int(p->detected); print_str("\n");
    return om.n_pollution - 1;
}

int om_deep_sea_exploration(double depth, int terrain_type) {
    if (om.n_deep_sea >= MAX_DEPTH) return -1;
    deep_sea_t* d = &deep_seas[om.n_deep_sea];
    d->depth = depth;
    d->pressure = depth * 0.1;
    d->temperature = 4.0 - depth * 0.001;
    d->terrain_type = terrain_type;
    d->biodiversity = 20.0 - depth * 0.005;
    if (d->biodiversity < 0) d->biodiversity = 0;
    d->explored = 1;
    d->id = om.n_deep_sea; d->active = 1;
    om.n_deep_sea++;
    print_str("[OM] Deep sea "); print_int(om.n_deep_sea - 1);
    print_str(" depth="); print_int((int)depth); print_str("m");
    print_str(" P="); print_int((int)d->pressure); print_str("atm");
    print_str(" T="); print_int((int)(d->temperature * 10)); print_str("C");
    print_str(" bio="); print_int((int)d->biodiversity); print_str("\n");
    return om.n_deep_sea - 1;
}

void om_print_state(void) {
    print_str("[OM] Stations="); print_int(om.n_stations);
    print_str(" Species="); print_int(om.n_species);
    print_str(" Pollution="); print_int(om.n_pollution);
    print_str(" DeepSea="); print_int(om.n_deep_sea);
    print_str(" Alerts="); print_int(om.pollution_alerts); print_str("\n");
}

int main(void) {
    print_str("=== Ocean Monitoring System Demo ===\n\n");
    om_init();

    print_str("Adding stations...\n");
    om_add_station(18.5, 35.0, 8.1, 0.0, 0.0);
    om_add_station(19.2, 34.5, 8.0, 10.0, 5.0);
    om_add_station(17.8, 35.5, 8.2, 20.0, 0.0);
    om_add_station(20.1, 34.0, 7.9, 10.0, 10.0);
    om_add_station(18.0, 35.2, 8.3, 0.0, 10.0);

    print_str("\nWater quality analysis...\n");
    om_water_quality_analysis();

    print_str("\nMarine species...\n");
    om_add_species(1, 50.0);
    om_add_species(2, 30.0);
    om_add_species(3, 80.0);
    om_add_species(4, 25.0);
    om_add_species(5, 45.0);

    print_str("\nMarine ecology assessment...\n");
    om_marine_ecology_assessment();

    print_str("\nTide and current prediction...\n");
    for (int i = 0; i < om.n_stations; i++) om_tide_current_prediction(i);

    print_str("\nPollution detection...\n");
    om_detect_pollution(1, 0.5, 1.0);
    om_detect_pollution(2, 2.5, 1.0);
    om_detect_pollution(3, 0.8, 0.5);
    om_detect_pollution(4, 3.0, 1.5);

    print_str("\nDeep sea exploration...\n");
    om_deep_sea_exploration(1000.0, 1);
    om_deep_sea_exploration(2000.0, 2);
    om_deep_sea_exploration(3000.0, 3);
    om_deep_sea_exploration(5000.0, 1);

    om_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
