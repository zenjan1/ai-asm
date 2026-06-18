/* industrial_iot: Industrial IoT platform (v1.0)
 * Protocol stack (MQTT/OPC UA/Modbus), device management, data collection, rules, security
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

#define MAX_DEVICES  16
#define MAX_TOPICS   16
#define MAX_RULES    8
#define MAX_DATA     64
#define MAX_ALERTS   16

#define PROTO_MQTT     0x01
#define PROTO_OPCUA    0x02
#define PROTO_MODBUS   0x03
#define PROTO_PROFINET 0x04
#define PROTO_ETHERCAT 0x05
#define PROTO_BACNET   0x06

#define DEV_SENSOR   0x01
#define DEV_ACTUATOR 0x02
#define DEV_PLC      0x03
#define DEV_GATEWAY  0x04
#define DEV_HMI      0x05

typedef struct {
    int    type;
    char   name[16];
    double value;
    double timestamp;
    int    protocol;
    int    device_id;
    int    quality;
    int    active;
} topic_t;

typedef struct {
    int    dev_type;
    int    protocol;
    double poll_rate;
    int    connected;
    int    firmware_ver;
    int    id;
    double last_seen;
    int    config_ver;
    int    active;
} device_t;

typedef struct {
    double timestamp;
    double value;
    int    device_id;
    int    quality;
    int    active;
} ts_data_t;

typedef struct {
    int    condition;
    int    action;
    double threshold;
    int    triggered;
    int    fired_count;
    int    active;
} rule_t;

typedef struct {
    int    severity;
    int    device_id;
    double timestamp;
    int    rule_id;
    int    acknowledged;
    int    active;
} alert_t;

typedef struct {
    int    n_devices;
    int    n_topics;
    int    n_rules;
    int    n_data;
    int    n_alerts;
    int    protocol_count;
    int    connected_devices;
    double data_rate;
    int    total_messages;
    int    security_level;
} iiot_state_t;

static device_t devices[MAX_DEVICES];
static topic_t topics[MAX_TOPICS];
static ts_data_t ts_data[MAX_DATA];
static rule_t rules[MAX_RULES];
static alert_t alerts[MAX_ALERTS];
static iiot_state_t iiot;

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

int ii_init(void) {
    if (initialized) return -1;
    iiot.n_devices = 0; iiot.n_topics = 0; iiot.n_rules = 0;
    iiot.n_data = 0; iiot.n_alerts = 0; iiot.protocol_count = 0;
    iiot.connected_devices = 0; iiot.data_rate = 0.0;
    iiot.total_messages = 0; iiot.security_level = 0;
    for (int i = 0; i < MAX_DEVICES; i++) devices[i].active = 0;
    for (int i = 0; i < MAX_TOPICS; i++) topics[i].active = 0;
    for (int i = 0; i < MAX_DATA; i++) ts_data[i].active = 0;
    for (int i = 0; i < MAX_RULES; i++) rules[i].active = 0;
    for (int i = 0; i < MAX_ALERTS; i++) alerts[i].active = 0;
    initialized = 1;
    print_str("[IIoT] Industrial IoT platform initialized\n");
    return 0;
}

int ii_register_device(int dev_type, int protocol, double rate) {
    if (iiot.n_devices >= MAX_DEVICES) return -1;
    device_t* d = &devices[iiot.n_devices];
    d->dev_type = dev_type; d->protocol = protocol;
    d->poll_rate = rate; d->connected = 1;
    d->firmware_ver = 100 + iiot.n_devices;
    d->id = iiot.n_devices; d->last_seen = 0.0;
    d->config_ver = 1; d->active = 1;
    iiot.n_devices++;
    iiot.connected_devices++;
    print_str("[IIoT] Device registered: type="); print_int(dev_type);
    print_str(" proto="); print_int(protocol);
    print_str(" rate="); print_int((int)rate); print_str("Hz\n");
    return iiot.n_devices - 1;
}

int ii_publish_topic(int device_id, double value, double timestamp) {
    if (iiot.n_topics >= MAX_TOPICS) return -1;
    topic_t* t = &topics[iiot.n_topics];
    t->device_id = device_id; t->value = value;
    t->timestamp = timestamp; t->protocol = devices[device_id].protocol;
    t->quality = 192; t->active = 1;
    iiot.n_topics++;
    iiot.total_messages++;
    print_str("[IIoT] Published: dev="); print_int(device_id);
    print_str(" val="); print_int((int)(value * 100));
    print_str(" t="); print_int((int)(timestamp * 1000)); print_str("ms\n");
    return iiot.n_topics - 1;
}

void ii_protocol_stack(int protocol) {
    print_str("[IIoT] Protocol stack: ");
    if (protocol == PROTO_MQTT) {
        print_str("MQTT v5.0 - publish/subscribe, QoS 0/1/2, retained, LWT\n");
    } else if (protocol == PROTO_OPCUA) {
        print_str("OPC UA - information model, services, security, subscriptions\n");
    } else if (protocol == PROTO_MODBUS) {
        print_str("Modbus TCP/RTU - registers, coils, discrete inputs\n");
    } else if (protocol == PROTO_PROFINET) {
        print_str("PROFINET - real-time, isochronous, diagnostic\n");
    } else if (protocol == PROTO_ETHERCAT) {
        print_str("EtherCAT - processing on the fly, distributed clocks\n");
    } else if (protocol == PROTO_BACNET) {
        print_str("BACnet - building automation, objects, services\n");
    }
    iiot.protocol_count++;
}

void ii_device_shadow(int device_id) {
    if (device_id >= iiot.n_devices) return;
    device_t* d = &devices[device_id];
    d->last_seen = 1000.0;
    print_str("[IIoT] Device shadow: id="); print_int(device_id);
    print_str(" type="); print_int(d->dev_type);
    print_str(" connected="); print_int(d->connected);
    print_str(" fw="); print_int(d->firmware_ver);
    print_str(" config="); print_int(d->config_ver); print_str("\n");
}

void ii_firmware_ota(int device_id, int new_version) {
    if (device_id >= iiot.n_devices) return;
    device_t* d = &devices[device_id];
    int old_ver = d->firmware_ver;
    d->firmware_ver = new_version;
    d->config_ver++;
    print_str("[IIoT] OTA update: device="); print_int(device_id);
    print_str(" "); print_int(old_ver); print_str(" -> "); print_int(new_version); print_str("\n");
}

void ii_collect_data(int device_id, double value, double timestamp) {
    if (iiot.n_data >= MAX_DATA) return;
    ts_data_t* td = &ts_data[iiot.n_data];
    td->device_id = device_id; td->value = value;
    td->timestamp = timestamp; td->quality = 192;
    td->active = 1;
    iiot.n_data++;
    iiot.data_rate = iiot.n_data / 10.0;
}

void ii_data_summary(void) {
    double min_v = 1e10, max_v = -1e10, sum = 0.0;
    int count = 0;
    for (int i = 0; i < iiot.n_data; i++) {
        if (!ts_data[i].active) continue;
        double v = ts_data[i].value;
        if (v < min_v) min_v = v;
        if (v > max_v) max_v = v;
        sum += v; count++;
    }
    double mean = sum / (count + 1e-10);
    print_str("[IIoT] Data summary: n="); print_int(count);
    print_str(" min="); print_int((int)(min_v * 100));
    print_str(" max="); print_int((int)(max_v * 100));
    print_str(" mean="); print_int((int)(mean * 100));
    print_str(" rate="); print_int((int)(iiot.data_rate * 10)); print_str("Hz\n");
}

int ii_add_rule(int condition, int action, double threshold) {
    if (iiot.n_rules >= MAX_RULES) return -1;
    rule_t* r = &rules[iiot.n_rules];
    r->condition = condition; r->action = action;
    r->threshold = threshold; r->triggered = 0;
    r->fired_count = 0; r->active = 1;
    iiot.n_rules++;
    print_str("[IIoT] Rule added: cond="); print_int(condition);
    print_str(" action="); print_int(action);
    print_str(" thresh="); print_int((int)(threshold * 100)); print_str("\n");
    return iiot.n_rules - 1;
}

void ii_evaluate_rules(void) {
    for (int i = 0; i < iiot.n_rules; i++) {
        if (!rules[i].active) continue;
        int triggered = 0;
        for (int j = 0; j < iiot.n_data; j++) {
            if (!ts_data[j].active) continue;
            if (rules[i].condition == 1 && ts_data[j].value > rules[i].threshold) triggered = 1;
            else if (rules[i].condition == 2 && ts_data[j].value < rules[i].threshold) triggered = 1;
        }
        if (triggered) {
            rules[i].triggered = 1;
            rules[i].fired_count++;
            if (iiot.n_alerts < MAX_ALERTS) {
                alert_t* a = &alerts[iiot.n_alerts];
                a->severity = 2;
                a->device_id = 0;
                a->timestamp = 1000.0;
                a->rule_id = i;
                a->acknowledged = 0;
                a->active = 1;
                iiot.n_alerts++;
            }
        }
    }
    print_str("[IIoT] Rule evaluation: "); print_int(iiot.n_rules);
    print_str(" rules, "); print_int(iiot.n_alerts); print_str(" alerts\n");
}

void ii_security_setup(int level) {
    iiot.security_level = level;
    print_str("[IIoT] Security level="); print_int(level); print_str("\n");
    if (level >= 1) print_str("  Device authentication (X.509 certs)\n");
    if (level >= 2) print_str("  Data encryption (TLS 1.3)\n");
    if (level >= 3) print_str("  Secure boot + access control\n");
    if (level >= 4) print_str("  Audit logging + intrusion detection\n");
}

void ii_print_state(void) {
    print_str("[IIoT] Devices="); print_int(iiot.n_devices);
    print_str(" Topics="); print_int(iiot.n_topics);
    print_str(" Rules="); print_int(iiot.n_rules);
    print_str(" Data="); print_int(iiot.n_data);
    print_str(" Alerts="); print_int(iiot.n_alerts);
    print_str(" Protos="); print_int(iiot.protocol_count);
    print_str(" Sec="); print_int(iiot.security_level);
    print_str("\n");
}

int main(void) {
    print_str("=== Industrial IoT Platform Demo ===\n\n");
    ii_init();

    print_str("Protocol stacks...\n");
    ii_protocol_stack(PROTO_MQTT);
    ii_protocol_stack(PROTO_OPCUA);
    ii_protocol_stack(PROTO_MODBUS);
    ii_protocol_stack(PROTO_PROFINET);
    ii_protocol_stack(PROTO_ETHERCAT);
    ii_protocol_stack(PROTO_BACNET);

    print_str("\nRegistering devices...\n");
    ii_register_device(DEV_SENSOR, PROTO_MQTT, 10.0);
    ii_register_device(DEV_SENSOR, PROTO_MODBUS, 5.0);
    ii_register_device(DEV_ACTUATOR, PROTO_PROFINET, 100.0);
    ii_register_device(DEV_PLC, PROTO_ETHERCAT, 1000.0);
    ii_register_device(DEV_GATEWAY, PROTO_MQTT, 1.0);
    ii_register_device(DEV_HMI, PROTO_OPCUA, 30.0);

    print_str("\nDevice shadows...\n");
    for (int i = 0; i < iiot.n_devices; i++) ii_device_shadow(i);

    print_str("\nOTA firmware update...\n");
    ii_firmware_ota(0, 102);
    ii_firmware_ota(3, 200);

    print_str("\nPublishing topics...\n");
    for (int i = 0; i < 5; i++)
        ii_publish_topic(0, 25.0 + i * 0.5, 100.0 + i * 100.0);

    print_str("\nCollecting time-series data...\n");
    for (int i = 0; i < 20; i++)
        ii_collect_data(i % iiot.n_devices, 20.0 + i * 0.3, i * 50.0);
    ii_data_summary();

    print_str("\nAdding rules...\n");
    ii_add_rule(1, 1, 30.0);
    ii_add_rule(2, 2, 15.0);
    ii_add_rule(1, 3, 28.0);

    print_str("\nEvaluating rules...\n");
    ii_evaluate_rules();

    print_str("\nSecurity setup...\n");
    ii_security_setup(4);

    ii_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
