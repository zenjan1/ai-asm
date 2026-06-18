/* smart_city: Smart city management system (v1.0)
 * Traffic, environment, public services, infrastructure, city brain
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

#define MAX_INTERSECTIONS 8
#define MAX_SENSORS       16
#define MAX_SERVICES      8
#define MAX_INFRA         8

#define TRAFFIC_ADAPTIVE  0x01
#define TRAFFIC_FIXED     0x02
#define TRAFFIC_COORDINATED 0x03

#define ENV_AIR    0x01
#define ENV_NOISE  0x02
#define ENV_WATER  0x03
#define ENV_WEATHER 0x04

typedef struct {
    double x, y;
    double flow_ns, flow_ew;
    double green_ns, green_ew;
    double cycle;
    double congestion;
    int    id;
    int    active;
} intersection_t;

typedef struct {
    int    type;
    double value;
    double threshold;
    double x, y;
    int    id;
    int    alarm;
    int    active;
} env_sensor_t;

typedef struct {
    int    type;
    int    status;
    double efficiency;
    int    id;
    int    active;
} service_t;

typedef struct {
    int    type;
    double health;
    double load;
    double age;
    int    id;
    int    active;
} infrastructure_t;

typedef struct {
    int    n_intersections;
    int    n_sensors;
    int    n_services;
    int    n_infra;
    double avg_congestion;
    double avg_aqi;
    double avg_noise;
    int    alarm_count;
    int    incidents;
} city_state_t;

static intersection_t intersections[MAX_INTERSECTIONS];
static env_sensor_t sensors[MAX_SENSORS];
static service_t services[MAX_SERVICES];
static infrastructure_t infra[MAX_INFRA];
static city_state_t city;

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

int city_init(void) {
    if (initialized) return -1;
    city.n_intersections = 0; city.n_sensors = 0;
    city.n_services = 0; city.n_infra = 0;
    city.avg_congestion = 0.0; city.avg_aqi = 50.0;
    city.avg_noise = 50.0; city.alarm_count = 0; city.incidents = 0;
    for (int i = 0; i < MAX_INTERSECTIONS; i++) intersections[i].active = 0;
    for (int i = 0; i < MAX_SENSORS; i++) sensors[i].active = 0;
    for (int i = 0; i < MAX_SERVICES; i++) services[i].active = 0;
    for (int i = 0; i < MAX_INFRA; i++) infra[i].active = 0;
    initialized = 1;
    print_str("[CITY] Smart city initialized\n");
    return 0;
}

int city_add_intersection(double x, double y, double flow_ns, double flow_ew) {
    if (city.n_intersections >= MAX_INTERSECTIONS) return -1;
    intersection_t* isect = &intersections[city.n_intersections];
    isect->x = x; isect->y = y;
    isect->flow_ns = flow_ns; isect->flow_ew = flow_ew;
    isect->green_ns = 30.0; isect->green_ew = 30.0;
    isect->cycle = 60.0; isect->congestion = 0.0;
    isect->id = city.n_intersections; isect->active = 1;
    city.n_intersections++;
    print_str("[CITY] Intersection "); print_int(city.n_intersections - 1);
    print_str(" flow_ns="); print_int((int)flow_ns);
    print_str(" flow_ew="); print_int((int)flow_ew); print_str("\n");
    return city.n_intersections - 1;
}

void city_traffic_control(int method) {
    double total_cong = 0.0;
    for (int i = 0; i < city.n_intersections; i++) {
        intersection_t* isect = &intersections[i];
        double total_flow = isect->flow_ns + isect->flow_ew;
        if (method == TRAFFIC_ADAPTIVE) {
            double ratio = isect->flow_ns / (total_flow + 1e-10);
            isect->green_ns = isect->cycle * ratio;
            isect->green_ew = isect->cycle * (1.0 - ratio);
        } else if (method == TRAFFIC_COORDINATED) {
            isect->green_ns = 35.0 + i * 2.0;
            isect->green_ew = 25.0 - i * 2.0;
        }
        isect->congestion = (total_flow - 1000.0) / 1000.0 * 100.0;
        if (isect->congestion < 0) isect->congestion = 0;
        if (isect->congestion > 100) isect->congestion = 100;
        total_cong += isect->congestion;
    }
    city.avg_congestion = total_cong / (city.n_intersections + 1e-10);
    print_str("[CITY] Traffic control method="); print_int(method);
    print_str(" avg_congestion="); print_int((int)city.avg_congestion); print_str("%\n");
    for (int i = 0; i < city.n_intersections; i++) {
        print_str("  Int "); print_int(i);
        print_str(" green_ns="); print_int((int)intersections[i].green_ns);
        print_str("s cong="); print_int((int)intersections[i].congestion); print_str("%\n");
    }
}

int city_add_sensor(int type, double value, double threshold, double x, double y) {
    if (city.n_sensors >= MAX_SENSORS) return -1;
    env_sensor_t* s = &sensors[city.n_sensors];
    s->type = type; s->value = value; s->threshold = threshold;
    s->x = x; s->y = y; s->id = city.n_sensors;
    s->alarm = value > threshold ? 1 : 0;
    s->active = 1;
    city.n_sensors++;
    if (s->alarm) city.alarm_count++;
    print_str("[CITY] Sensor type="); print_int(type);
    print_str(" val="); print_int((int)(value * 10));
    print_str(" thresh="); print_int((int)(threshold * 10));
    print_str(" alarm="); print_int(s->alarm); print_str("\n");
    return city.n_sensors - 1;
}

void city_monitor_environment(void) {
    double aqi_sum = 0.0, noise_sum = 0.0;
    int aqi_n = 0, noise_n = 0;
    for (int i = 0; i < city.n_sensors; i++) {
        if (!sensors[i].active) continue;
        if (sensors[i].type == ENV_AIR) { aqi_sum += sensors[i].value; aqi_n++; }
        else if (sensors[i].type == ENV_NOISE) { noise_sum += sensors[i].value; noise_n++; }
    }
    city.avg_aqi = aqi_sum / (aqi_n + 1e-10);
    city.avg_noise = noise_sum / (noise_n + 1e-10);
    print_str("[CITY] Environment: AQI="); print_int((int)city.avg_aqi);
    print_str(" noise="); print_int((int)city.avg_noise); print_str("dB\n");
}

int city_add_service(int type) {
    if (city.n_services >= MAX_SERVICES) return -1;
    service_t* sv = &services[city.n_services];
    sv->type = type; sv->status = 1; sv->efficiency = 90.0;
    sv->id = city.n_services; sv->active = 1;
    city.n_services++;
    print_str("[CITY] Service type="); print_int(type);
    print_str(" status="); print_int(sv->status);
    print_str(" eff="); print_int((int)sv->efficiency); print_str("%\n");
    return city.n_services - 1;
}

int city_add_infrastructure(int type, double health) {
    if (city.n_infra >= MAX_INFRA) return -1;
    infrastructure_t* inf = &infra[city.n_infra];
    inf->type = type; inf->health = health;
    inf->load = 50.0; inf->age = 10.0;
    inf->id = city.n_infra; inf->active = 1;
    city.n_infra++;
    print_str("[CITY] Infrastructure type="); print_int(type);
    print_str(" health="); print_int((int)health); print_str("%\n");
    return city.n_infra - 1;
}

void city_brain_fusion(void) {
    print_str("[CITY] City brain fusion:\n");
    print_str("  Intersections: "); print_int(city.n_intersections);
    print_str(" congestion="); print_int((int)city.avg_congestion); print_str("%\n");
    print_str("  Env sensors: "); print_int(city.n_sensors);
    print_str(" AQI="); print_int((int)city.avg_aqi);
    print_str(" noise="); print_int((int)city.avg_noise); print_str("dB\n");
    print_str("  Services: "); print_int(city.n_services);
    print_str("  Infrastructure: "); print_int(city.n_infra); print_str("\n");
    print_str("  Alarms: "); print_int(city.alarm_count);
    print_str("  Incidents: "); print_int(city.incidents); print_str("\n");
}

void city_emergency_response(int severity) {
    city.incidents++;
    print_str("[CITY] Emergency response: severity="); print_int(severity); print_str("\n");
    print_str("  Dispatching units...\n");
    print_str("  Coordinating traffic signals...\n");
    print_str("  Notifying services...\n");
}

void city_print_state(void) {
    print_str("[CITY] Int="); print_int(city.n_intersections);
    print_str(" Sensors="); print_int(city.n_sensors);
    print_str(" Services="); print_int(city.n_services);
    print_str(" Infra="); print_int(city.n_infra);
    print_str(" Cong="); print_int((int)city.avg_congestion);
    print_str("% AQI="); print_int((int)city.avg_aqi);
    print_str(" Alarms="); print_int(city.alarm_count); print_str("\n");
}

int main(void) {
    print_str("=== Smart City System Demo ===\n\n");
    city_init();

    print_str("Adding intersections...\n");
    city_add_intersection(0.0, 0.0, 500.0, 300.0);
    city_add_intersection(1.0, 0.0, 400.0, 600.0);
    city_add_intersection(0.0, 1.0, 700.0, 200.0);
    city_add_intersection(1.0, 1.0, 300.0, 400.0);

    print_str("\nTraffic control (fixed time)...\n");
    city_traffic_control(TRAFFIC_FIXED);

    print_str("\nTraffic control (adaptive)...\n");
    city_traffic_control(TRAFFIC_ADAPTIVE);

    print_str("\nTraffic control (coordinated)...\n");
    city_traffic_control(TRAFFIC_COORDINATED);

    print_str("\nEnvironmental sensors...\n");
    city_add_sensor(ENV_AIR, 45.0, 100.0, 0.0, 0.0);
    city_add_sensor(ENV_AIR, 78.0, 100.0, 1.0, 0.0);
    city_add_sensor(ENV_AIR, 120.0, 100.0, 0.5, 0.5);
    city_add_sensor(ENV_NOISE, 55.0, 70.0, 0.0, 0.5);
    city_add_sensor(ENV_NOISE, 72.0, 70.0, 1.0, 0.5);
    city_add_sensor(ENV_WATER, 7.5, 8.0, 0.5, 0.0);
    city_add_sensor(ENV_WEATHER, 25.0, 40.0, 0.0, 1.0);

    print_str("\nEnvironment monitoring...\n");
    city_monitor_environment();

    print_str("\nPublic services...\n");
    city_add_service(1);
    city_add_service(2);
    city_add_service(3);
    city_add_service(4);

    print_str("\nInfrastructure...\n");
    city_add_infrastructure(1, 92.0);
    city_add_infrastructure(2, 78.0);
    city_add_infrastructure(3, 85.0);
    city_add_infrastructure(4, 65.0);

    print_str("\nCity brain fusion...\n");
    city_brain_fusion();

    print_str("\nEmergency response (severity 3)...\n");
    city_emergency_response(3);

    city_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
