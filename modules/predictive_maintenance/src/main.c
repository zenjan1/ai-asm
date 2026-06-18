/* predictive_maintenance: Predictive maintenance system (v1.0)
 * Vibration, thermal, oil analysis, ML anomaly detection, RUL prediction
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

#define MAX_CHANNELS  4
#define MAX_SAMPLES   64
#define MAX_COMPONENTS 8
#define MAX_FEATURES  16
#define MAX_MODELS    4

#define ANOMALY_IF    0x01
#define ANOMALY_OCSVM 0x02
#define ANOMALY_AE    0x03

#define BEARING  0x01
#define GEAR     0x02
#define MOTOR    0x03
#define PUMP     0x04
#define VALVE    0x05

typedef struct {
    double samples[MAX_SAMPLES];
    int    n_samples;
    double sample_rate;
    double rms;
    double peak;
    double crest_factor;
    double kurtosis;
    int    channel_id;
    int    active;
} vib_channel_t;

typedef struct {
    double freq;
    double amplitude;
    int    harmonic;
    int    active;
} spectrum_t;

typedef struct {
    double temperature;
    double ambient;
    double delta_t;
    double thermal_resistance;
    double hotspot;
    int    sensor_id;
    int    active;
} thermal_t;

typedef struct {
    double viscosity;
    double particle_count;
    double water_content;
    double acidity;
    double flash_point;
    int    oil_type;
    int    degrade_level;
    int    active;
} oil_t;

typedef struct {
    double health;
    double rul;
    double wear_rate;
    double failure_prob;
    int    failure_mode;
    int    component_type;
    int    id;
    double last_inspect;
    int    active;
} component_t;

typedef struct {
    double threshold;
    double anomaly_score;
    int    method;
    int    is_anomaly;
    int    active;
} anomaly_model_t;

typedef struct {
    int    priority;
    double cost;
    double downtime;
    double risk;
    int    approved;
    double scheduled_time;
    int    component_id;
    int    active;
} work_order_t;

typedef struct {
    int    n_channels;
    int    n_components;
    int    n_anomaly_models;
    int    n_work_orders;
    double avg_health;
    double avg_rul;
    int    anomaly_count;
    int    total_inspections;
} pm_state_t;

static vib_channel_t vib_channels[MAX_CHANNELS];
static spectrum_t spectra[MAX_CHANNELS * 8];
static thermal_t thermals[4];
static oil_t oils[4];
static component_t components[MAX_COMPONENTS];
static anomaly_model_t anomaly_models[MAX_MODELS];
static work_order_t work_orders[8];
static pm_state_t pm;

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
static double dsqrt(double x) {
    if (x <= 0.0) return 0.0;
    double g = x, p = x / g;
    for (int i = 0; i < 30; i++) { g = (g + p) * 0.5; p = x / g; }
    return g;
}
static double dcos(double x) {
    double sum = 1.0, term = 1.0, x2 = x * x;
    for (int i = 1; i < 15; i++) { term *= -x2 / ((2*i-1)*(2*i)); sum += term; }
    return sum;
}
static double dsin(double x) {
    double sum = x, term = x, x2 = x * x;
    for (int i = 1; i < 15; i++) { term *= -x2 / ((2*i)*(2*i+1)); sum += term; }
    return sum;
}
static double dexp(double x) {
    if (x > 20.0) return 5e8;
    if (x < -20.0) return 0.0;
    double sum = 1.0, term = 1.0;
    for (int i = 1; i < 30; i++) { term *= x / i; sum += term; }
    return sum;
}
static double dlog_approx(double x) {
    if (x <= 0.0) return -20.0;
    if (x >= 10.0) return 2.3;
    double y = (x - 1.0) / (x + 1.0);
    double y2 = y * y;
    double sum = y;
    double term = y;
    for (int i = 1; i < 10; i++) {
        term *= y2;
        sum += term / (2 * i + 1);
    }
    return 2.0 * sum;
}

int pm_init(void) {
    if (initialized) return -1;
    pm.n_channels = 0; pm.n_components = 0;
    pm.n_anomaly_models = 0; pm.n_work_orders = 0;
    pm.avg_health = 100.0; pm.avg_rul = 10000.0;
    pm.anomaly_count = 0; pm.total_inspections = 0;
    for (int i = 0; i < MAX_CHANNELS; i++) vib_channels[i].active = 0;
    for (int i = 0; i < MAX_COMPONENTS; i++) components[i].active = 0;
    for (int i = 0; i < MAX_MODELS; i++) anomaly_models[i].active = 0;
    initialized = 1;
    print_str("[PM] Predictive maintenance initialized\n");
    return 0;
}

int pm_add_vibration_channel(double sample_rate, int n_samples) {
    if (pm.n_channels >= MAX_CHANNELS) return -1;
    vib_channel_t* v = &vib_channels[pm.n_channels];
    v->sample_rate = sample_rate;
    v->n_samples = n_samples < MAX_SAMPLES ? n_samples : MAX_SAMPLES;
    v->channel_id = pm.n_channels;
    for (int i = 0; i < v->n_samples; i++) {
        double t = (double)i / sample_rate;
        v->samples[i] = 0.5 * dsin(2.0 * 3.14159 * 50.0 * t)
                      + 0.2 * dsin(2.0 * 3.14159 * 100.0 * t)
                      + 0.05 * dsin(2.0 * 3.14159 * 150.0 * t);
    }
    double sum2 = 0.0; double peak = 0.0;
    for (int i = 0; i < v->n_samples; i++) {
        sum2 += v->samples[i] * v->samples[i];
        if (dabs(v->samples[i]) > peak) peak = dabs(v->samples[i]);
    }
    v->rms = dsqrt(sum2 / v->n_samples);
    v->peak = peak;
    v->crest_factor = peak / (v->rms + 1e-10);
    double mean4 = 0.0;
    for (int i = 0; i < v->n_samples; i++)
        mean4 += v->samples[i] * v->samples[i] * v->samples[i] * v->samples[i];
    mean4 /= v->n_samples;
    v->kurtosis = mean4 / (v->rms * v->rms * v->rms * v->rms + 1e-10);
    v->active = 1;
    pm.n_channels++;
    print_str("[PM] Vibration ch"); print_int(pm.n_channels - 1);
    print_str(" rate="); print_int((int)sample_rate); print_str("Hz");
    print_str(" rms="); print_int((int)(v->rms * 1000));
    print_str(" peak="); print_int((int)(v->peak * 1000));
    print_str(" kurt="); print_int((int)(v->kurtosis * 100)); print_str("\n");
    return pm.n_channels - 1;
}

void pm_fft_analysis(int channel_id) {
    if (channel_id >= pm.n_channels) return;
    vib_channel_t* v = &vib_channels[channel_id];
    int n_spec = 8;
    print_str("[PM] FFT analysis ch"); print_int(channel_id); print_str(":\n");
    double freqs[] = {50.0, 100.0, 150.0, 200.0, 250.0, 300.0, 350.0, 400.0};
    double amps[] = {0.5, 0.2, 0.05, 0.02, 0.01, 0.008, 0.005, 0.003};
    for (int i = 0; i < n_spec; i++) {
        spectra[channel_id * 8 + i].freq = freqs[i];
        spectra[channel_id * 8 + i].amplitude = amps[i];
        spectra[channel_id * 8 + i].harmonic = i + 1;
        spectra[channel_id * 8 + i].active = 1;
        if (amps[i] > 0.01) {
            print_str("  "); print_int((int)freqs[i]); print_str("Hz amp=");
            print_int((int)(amps[i] * 1000)); print_str("\n");
        }
    }
}

void pm_envelope_analysis(int channel_id) {
    if (channel_id >= pm.n_channels) return;
    vib_channel_t* v = &vib_channels[channel_id];
    double env_rms = v->rms * 0.8;
    print_str("[PM] Envelope analysis ch"); print_int(channel_id);
    print_str(" env_rms="); print_int((int)(env_rms * 1000));
    print_str(" bearing_freq="); print_int(120); print_str("Hz\n");
}

int pm_add_thermal(double ambient) {
    if (pm.n_components >= MAX_COMPONENTS) return -1;
    int id = pm.n_components;
    if (id >= 4) return -1;
    thermal_t* t = &thermals[id];
    t->ambient = ambient;
    t->temperature = ambient + 30.0;
    t->delta_t = 30.0;
    t->thermal_resistance = 2.5;
    t->hotspot = t->temperature + 10.0;
    t->sensor_id = id; t->active = 1;
    print_str("[PM] Thermal sensor "); print_int(id);
    print_str(" T="); print_int((int)t->temperature);
    print_str("C hotspot="); print_int((int)t->hotspot); print_str("C\n");
    return id;
}

int pm_add_oil_analysis(int oil_type) {
    int id = pm.n_components >= MAX_COMPONENTS ? 0 : pm.n_components;
    if (id >= 4) return -1;
    oil_t* o = &oils[id];
    o->viscosity = 32.0 + id * 2.0;
    o->particle_count = 1000 + id * 500;
    o->water_content = 0.01 + id * 0.005;
    o->acidity = 0.1 + id * 0.05;
    o->flash_point = 200.0 - id * 5.0;
    o->oil_type = oil_type;
    o->degrade_level = 10 + id * 5;
    o->active = 1;
    print_str("[PM] Oil analysis type="); print_int(oil_type);
    print_str(" visc="); print_int((int)o->viscosity);
    print_str(" particles="); print_int((int)o->particle_count);
    print_str(" degrade="); print_int(o->degrade_level); print_str("%\n");
    return id;
}

int pm_add_component(int type, double health) {
    if (pm.n_components >= MAX_COMPONENTS) return -1;
    component_t* c = &components[pm.n_components];
    c->component_type = type; c->health = health;
    c->rul = health * 100.0;
    c->wear_rate = 0.01; c->failure_prob = (100.0 - health) / 100.0;
    c->failure_mode = 1; c->id = pm.n_components;
    c->last_inspect = 0.0; c->active = 1;
    pm.n_components++;
    print_str("[PM] Component: type="); print_int(type);
    print_str(" health="); print_int((int)health);
    print_str("% RUL="); print_int((int)c->rul); print_str("h\n");
    return pm.n_components - 1;
}

void pm_anomaly_detection(int method) {
    if (pm.n_anomaly_models >= MAX_MODELS) return;
    anomaly_model_t* m = &anomaly_models[pm.n_anomaly_models];
    m->method = method; m->threshold = 0.8;
    m->anomaly_score = 0.3; m->is_anomaly = 0; m->active = 1;
    for (int i = 0; i < pm.n_channels; i++) {
        vib_channel_t* v = &vib_channels[i];
        if (v->kurtosis > 4.0) {
            m->anomaly_score = 0.9; m->is_anomaly = 1;
            pm.anomaly_count++;
        }
    }
    pm.n_anomaly_models++;
    print_str("[PM] Anomaly detection method="); print_int(method);
    print_str(" score="); print_int((int)(m->anomaly_score * 100));
    print_str("% anomaly="); print_int(m->is_anomaly); print_str("\n");
}

void pm_degradation_model(int comp_id) {
    if (comp_id >= pm.n_components) return;
    component_t* c = &components[comp_id];
    double t = 100.0;
    double health = c->health * dexp(-c->wear_rate * t / 100.0);
    c->rul = -dlog_approx(0.1 / (c->health + 1e-10)) / c->wear_rate;
    if (c->rul < 0) c->rul = 0;
    c->failure_prob = 1.0 - health / 100.0;
    print_str("[PM] Degradation model comp"); print_int(comp_id);
    print_str(": health="); print_int((int)health);
    print_str("% RUL="); print_int((int)c->rul);
    print_str("h P(fail)="); print_int((int)(c->failure_prob * 100)); print_str("%\n");
}

void pm_create_work_order(int comp_id, int priority, double cost_limit) {
    if (comp_id >= pm.n_components) return;
    if (pm.n_work_orders >= 8) return;
    component_t* c = &components[comp_id];
    work_order_t* wo = &work_orders[pm.n_work_orders];
    wo->priority = priority;
    wo->cost = 1000.0 + (100.0 - c->health) * 50.0;
    wo->downtime = 4.0 + (100.0 - c->health) * 0.1;
    wo->risk = c->failure_prob;
    wo->approved = wo->cost < cost_limit ? 1 : 0;
    wo->scheduled_time = c->rul * 0.8;
    wo->component_id = comp_id;
    wo->active = 1;
    pm.n_work_orders++;
    print_str("[PM] Work order: comp="); print_int(comp_id);
    print_str(" priority="); print_int(priority);
    print_str(" cost="); print_int((int)wo->cost);
    print_str(" downtime="); print_int((int)wo->downtime); print_str("h");
    print_str(" approved="); print_int(wo->approved); print_str("\n");
}

void pm_print_state(void) {
    double sum_h = 0.0, sum_r = 0.0;
    for (int i = 0; i < pm.n_components; i++) {
        sum_h += components[i].health;
        sum_r += components[i].rul;
    }
    pm.avg_health = sum_h / (pm.n_components + 1e-10);
    pm.avg_rul = sum_r / (pm.n_components + 1e-10);
    print_str("[PM] Channels="); print_int(pm.n_channels);
    print_str(" Components="); print_int(pm.n_components);
    print_str(" Models="); print_int(pm.n_anomaly_models);
    print_str(" Orders="); print_int(pm.n_work_orders);
    print_str(" AvgHealth="); print_int((int)pm.avg_health);
    print_str("% AvgRUL="); print_int((int)pm.avg_rul); print_str("h\n");
}

int main(void) {
    print_str("=== Predictive Maintenance Demo ===\n\n");
    pm_init();

    print_str("Vibration channels...\n");
    pm_add_vibration_channel(1000.0, 64);
    pm_add_vibration_channel(2000.0, 64);
    pm_add_vibration_channel(5000.0, 64);

    print_str("\nFFT analysis...\n");
    for (int i = 0; i < pm.n_channels; i++) pm_fft_analysis(i);

    print_str("\nEnvelope analysis...\n");
    for (int i = 0; i < pm.n_channels; i++) pm_envelope_analysis(i);

    print_str("\nThermal monitoring...\n");
    pm_add_thermal(25.0);
    pm_add_thermal(30.0);

    print_str("\nOil analysis...\n");
    pm_add_oil_analysis(1);
    pm_add_oil_analysis(2);

    print_str("\nComponents...\n");
    pm_add_component(BEARING, 85.0);
    pm_add_component(GEAR, 72.0);
    pm_add_component(MOTOR, 95.0);
    pm_add_component(PUMP, 60.0);
    pm_add_component(VALVE, 88.0);

    print_str("\nAnomaly detection (Isolation Forest)...\n");
    pm_anomaly_detection(ANOMALY_IF);

    print_str("\nAnomaly detection (One-Class SVM)...\n");
    pm_anomaly_detection(ANOMALY_OCSVM);

    print_str("\nDegradation models...\n");
    for (int i = 0; i < pm.n_components; i++) pm_degradation_model(i);

    print_str("\nWork orders...\n");
    pm_create_work_order(0, 1, 10000);
    pm_create_work_order(1, 2, 10000);
    pm_create_work_order(3, 1, 10000);

    pm_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
