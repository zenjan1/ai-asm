/* precision_agriculture: Precision agriculture system (v1.0)
 * Soil monitoring, crop health, irrigation control, yield prediction, VRA
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

#define MAX_FIELDS     8
#define MAX_ZONES      16
#define MAX_SENSORS    16
#define MAX_CROPS      8

#define IRRIG_DRIP     0x01
#define IRRIG_SPRINK   0x02
#define IRRIG_FLOOD    0x03

typedef struct {
    double moisture;
    double ph;
    double nitrogen;
    double phosphorus;
    double potassium;
    double temperature;
    double ec;
    int    zone_id;
    int    id;
    int    active;
} soil_sensor_t;

typedef struct {
    double ndvi;
    double ndre;
    double cvri;
    double health_score;
    int    growth_stage;
    int    pest_detected;
    int    zone_id;
    int    id;
    int    active;
} crop_health_t;

typedef struct {
    double water_needed;
    double water_applied;
    double efficiency;
    int    method;
    int    scheduled;
    int    zone_id;
    int    id;
    int    active;
} irrigation_t;

typedef struct {
    double area_ha;
    double yield_estimate;
    double yield_actual;
    double planting_density;
    int    crop_type;
    int    season;
    int    field_id;
    int    id;
    int    active;
} yield_t;

typedef struct {
    double rate_n;
    double rate_p;
    double rate_k;
    double rate_pesticide;
    double cost;
    int    zone_id;
    int    id;
    int    active;
} vra_prescription_t;

typedef struct {
    int    n_fields;
    int    n_zones;
    int    n_sensors;
    int    n_crops;
    double total_area;
    double avg_moisture;
    double avg_ndvi;
    double total_yield;
    int    irrigation_count;
} pa_state_t;

static soil_sensor_t soil_sensors[MAX_SENSORS];
static crop_health_t crop_health[MAX_ZONES];
static irrigation_t irrigations[MAX_ZONES];
static yield_t yields[MAX_FIELDS];
static vra_prescription_t vra[MAX_ZONES];
static pa_state_t pa;

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

int pa_init(void) {
    if (initialized) return -1;
    pa.n_fields = 0; pa.n_zones = 0; pa.n_sensors = 0;
    pa.n_crops = 0; pa.total_area = 0.0;
    pa.avg_moisture = 0.0; pa.avg_ndvi = 0.0;
    pa.total_yield = 0.0; pa.irrigation_count = 0;
    for (int i = 0; i < MAX_SENSORS; i++) soil_sensors[i].active = 0;
    for (int i = 0; i < MAX_ZONES; i++) { crop_health[i].active = 0; irrigations[i].active = 0; vra[i].active = 0; }
    for (int i = 0; i < MAX_FIELDS; i++) yields[i].active = 0;
    initialized = 1;
    print_str("[PA] Precision agriculture initialized\n");
    return 0;
}

int pa_add_field(double area, int crop_type) {
    if (pa.n_fields >= MAX_FIELDS) return -1;
    yield_t* y = &yields[pa.n_fields];
    y->area_ha = area; y->crop_type = crop_type;
    y->planting_density = 50000.0 + (double)(crop_type % 3) * 10000.0;
    y->yield_estimate = area * 5.0;
    y->yield_actual = 0.0; y->season = 1;
    y->field_id = pa.n_fields; y->id = pa.n_fields; y->active = 1;
    pa.n_fields++;
    pa.total_area += area;
    print_str("[PA] Field "); print_int(pa.n_fields - 1);
    print_str(" area="); print_int((int)(area * 100)); print_str("ha");
    print_str(" crop="); print_int(crop_type);
    print_str(" density="); print_int((int)y->planting_density); print_str("/ha\n");
    return pa.n_fields - 1;
}

int pa_add_soil_sensor(int zone_id) {
    if (pa.n_sensors >= MAX_SENSORS) return -1;
    soil_sensor_t* s = &soil_sensors[pa.n_sensors];
    s->moisture = 40.0 + (double)(pa.n_sensors % 4) * 10.0;
    s->ph = 6.0 + (double)(pa.n_sensors % 3) * 0.5;
    s->nitrogen = 30.0 + (double)(pa.n_sensors % 5) * 10.0;
    s->phosphorus = 20.0 + (double)(pa.n_sensors % 4) * 5.0;
    s->potassium = 150.0 + (double)(pa.n_sensors % 3) * 30.0;
    s->temperature = 18.0 + (double)(pa.n_sensors % 5) * 2.0;
    s->ec = 1.2 + (double)(pa.n_sensors % 4) * 0.3;
    s->zone_id = zone_id; s->id = pa.n_sensors; s->active = 1;
    pa.n_sensors++;
    print_str("[PA] Soil sensor "); print_int(pa.n_sensors - 1);
    print_str(" zone="); print_int(zone_id);
    print_str(" moist="); print_int((int)s->moisture); print_str("%");
    print_str(" pH="); print_int((int)(s->ph * 10));
    print_str(" N="); print_int((int)s->nitrogen); print_str("ppm\n");
    return pa.n_sensors - 1;
}

void pa_crop_health_assessment(int zone_id) {
    if (zone_id >= pa.n_zones && zone_id >= pa.n_sensors) return;
    crop_health_t* ch = &crop_health[zone_id];
    ch->ndvi = 0.5 + (double)(zone_id % 4) * 0.1;
    ch->ndre = 0.4 + (double)(zone_id % 3) * 0.08;
    ch->cvri = 0.3 + (double)(zone_id % 5) * 0.05;
    ch->health_score = ch->ndvi * 100.0;
    ch->growth_stage = 2 + zone_id % 3;
    ch->pest_detected = (zone_id % 5 == 0) ? 1 : 0;
    ch->zone_id = zone_id; ch->id = zone_id; ch->active = 1;
    if (zone_id >= pa.n_zones) pa.n_zones = zone_id + 1;
    print_str("[PA] Crop health zone "); print_int(zone_id);
    print_str(" NDVI="); print_int((int)(ch->ndvi * 100));
    print_str(" health="); print_int((int)ch->health_score);
    print_str(" stage="); print_int(ch->growth_stage);
    print_str(" pest="); print_int(ch->pest_detected); print_str("\n");
}

void pa_irrigation_schedule(int zone_id, int method) {
    if (zone_id >= pa.n_zones) return;
    irrigation_t* irr = &irrigations[zone_id];
    irr->method = method;
    irr->water_needed = 20.0 + (double)(zone_id % 4) * 5.0;
    irr->water_applied = irr->water_needed * 0.9;
    irr->efficiency = method == IRRIG_DRIP ? 0.95 : (method == IRRIG_SPRINK ? 0.80 : 0.65);
    irr->scheduled = 1;
    irr->zone_id = zone_id; irr->id = zone_id; irr->active = 1;
    pa.irrigation_count++;
    print_str("[PA] Irrigation zone "); print_int(zone_id);
    print_str(" method="); print_int(method);
    print_str(" need="); print_int((int)irr->water_needed); print_str("mm");
    print_str(" applied="); print_int((int)irr->water_applied); print_str("mm");
    print_str(" eff="); print_int((int)(irr->efficiency * 100)); print_str("%\n");
}

void pa_yield_prediction(int field_id) {
    if (field_id >= pa.n_fields) return;
    yield_t* y = &yields[field_id];
    double base = y->area_ha * 5.0;
    double factor = 1.0;
    for (int i = 0; i < pa.n_sensors; i++) {
        if (soil_sensors[i].moisture < 30.0) factor *= 0.95;
        if (soil_sensors[i].nitrogen < 25.0) factor *= 0.97;
    }
    y->yield_estimate = base * factor;
    y->yield_actual = y->yield_estimate * 0.95;
    pa.total_yield += y->yield_actual;
    print_str("[PA] Yield prediction field "); print_int(field_id);
    print_str(" area="); print_int((int)y->area_ha); print_str("ha");
    print_str(" est="); print_int((int)y->yield_estimate); print_str("t");
    print_str(" actual="); print_int((int)y->yield_actual); print_str("t\n");
}

void pa_variable_rate_application(int zone_id) {
    if (zone_id >= pa.n_zones) return;
    vra_prescription_t* v = &vra[zone_id];
    v->rate_n = 100.0 + (double)(zone_id % 4) * 20.0;
    v->rate_p = 50.0 + (double)(zone_id % 3) * 10.0;
    v->rate_k = 80.0 + (double)(zone_id % 5) * 15.0;
    v->rate_pesticide = crop_health[zone_id].pest_detected ? 2.0 : 0.5;
    v->cost = v->rate_n * 0.5 + v->rate_p * 0.8 + v->rate_k * 0.3 + v->rate_pesticide * 20.0;
    v->zone_id = zone_id; v->id = zone_id; v->active = 1;
    print_str("[PA] VRA zone "); print_int(zone_id);
    print_str(" N="); print_int((int)v->rate_n);
    print_str(" P="); print_int((int)v->rate_p);
    print_str(" K="); print_int((int)v->rate_k);
    print_str(" pest="); print_int((int)v->rate_pesticide);
    print_str(" cost="); print_int((int)v->cost); print_str("USD/ha\n");
}

void pa_soil_analysis(void) {
    double sum_moist = 0.0, sum_ndvi = 0.0;
    int n = 0;
    for (int i = 0; i < pa.n_sensors; i++) {
        sum_moist += soil_sensors[i].moisture;
        n++;
    }
    pa.avg_moisture = sum_moist / (n + 1e-10);
    for (int i = 0; i < pa.n_zones; i++) {
        if (crop_health[i].active) sum_ndvi += crop_health[i].ndvi;
    }
    pa.avg_ndvi = sum_ndvi / (pa.n_zones + 1e-10);
    print_str("[PA] Soil analysis: avg_moisture="); print_int((int)pa.avg_moisture);
    print_str("% avg_NDVI="); print_int((int)(pa.avg_ndvi * 100)); print_str("\n");
    for (int i = 0; i < pa.n_sensors; i++) {
        print_str("  Sensor "); print_int(i);
        print_str(" pH="); print_int((int)(soil_sensors[i].ph * 10));
        print_str(" N="); print_int((int)soil_sensors[i].nitrogen);
        print_str(" P="); print_int((int)soil_sensors[i].phosphorus);
        print_str(" K="); print_int((int)soil_sensors[i].potassium); print_str("\n");
    }
}

void pa_print_state(void) {
    print_str("[PA] Fields="); print_int(pa.n_fields);
    print_str(" Zones="); print_int(pa.n_zones);
    print_str(" Sensors="); print_int(pa.n_sensors);
    print_str(" Area="); print_int((int)pa.total_area); print_str("ha");
    print_str(" Yield="); print_int((int)pa.total_yield); print_str("t\n");
}

int main(void) {
    print_str("=== Precision Agriculture Demo ===\n\n");
    pa_init();

    print_str("Adding fields...\n");
    pa_add_field(50.0, 1);
    pa_add_field(30.0, 2);
    pa_add_field(40.0, 1);
    pa_add_field(25.0, 3);

    print_str("\nAdding soil sensors...\n");
    for (int i = 0; i < 8; i++) pa_add_soil_sensor(i);

    print_str("\nCrop health assessment...\n");
    for (int i = 0; i < 8; i++) pa_crop_health_assessment(i);

    print_str("\nIrrigation scheduling...\n");
    pa_irrigation_schedule(0, IRRIG_DRIP);
    pa_irrigation_schedule(1, IRRIG_SPRINK);
    pa_irrigation_schedule(2, IRRIG_DRIP);
    pa_irrigation_schedule(3, IRRIG_FLOOD);
    pa_irrigation_schedule(4, IRRIG_DRIP);

    print_str("\nSoil analysis...\n");
    pa_soil_analysis();

    print_str("\nYield prediction...\n");
    for (int i = 0; i < pa.n_fields; i++) pa_yield_prediction(i);

    print_str("\nVariable rate application...\n");
    for (int i = 0; i < 8; i++) pa_variable_rate_application(i);

    pa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
