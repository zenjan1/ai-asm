#include <stddef.h>

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_DEVICES 128
#define MAX_RULES 64
#define MAX_TOPICS 256
#define STATUS_OK 0
#define STATUS_ERR -1

#define PROTO_MQTT 0x01
#define PROTO_COAP 0x02
#define PROTO_HTTP 0x03
#define PROTO_LORAWAN 0x04
#define PROTO_NBIOT 0x05
#define PROTO_ZIGBEE 0x06

#define AUTH_X509 0x01
#define AUTH_PSK 0x02
#define AUTH_JWT 0x03

typedef struct {
    int device_id;
    unsigned int name_hash;
    int protocol;
    int auth_method;
    int connected;
    float last_telemetry;
    int firmware_version;
    int active;
} Device;

typedef struct {
    int rule_id;
    int device_id;
    int condition_type;
    float threshold;
    int action_type;
    int active;
} Rule;

typedef struct {
    int device_id;
    float desired_state;
    float reported_state;
    int version;
    int synced;
} DeviceShadow;

typedef struct {
    Device devices[MAX_DEVICES];
    int device_count;
    Rule rules[MAX_RULES];
    int rule_count;
    DeviceShadow shadows[MAX_DEVICES];
    int topic_count;
    unsigned int total_messages;
} IoTPlatform;

static IoTPlatform* platform = NULL;

static void print_str(const char* s) { host_print(s); }

static void print_int(int val) {
    char buf[32];
    int i = 0;
    if (val < 0) { buf[i++] = '-'; val = -val; }
    if (val == 0) { buf[i++] = '0'; }
    else {
        int start = i;
        while (val > 0) { buf[i++] = '0' + (val % 10); val /= 10; }
        int end = i - 1;
        while (start < end) { char t = buf[start]; buf[start] = buf[end]; buf[end] = t; start++; end--; }
    }
    buf[i] = '\0';
    host_print(buf);
}

static void print_hex(unsigned int val) {
    char buf[16];
    int i = 0;
    buf[i++] = '0'; buf[i++] = 'x';
    if (val == 0) { buf[i++] = '0'; }
    else {
        int start = i;
        while (val > 0) {
            int d = val & 0xF;
            buf[i++] = (d < 10) ? ('0' + d) : ('A' + d - 10);
            val >>= 4;
        }
        int end = i - 1;
        while (start < end) { char t = buf[start]; buf[start] = buf[end]; buf[end] = t; start++; end--; }
    }
    buf[i] = '\0';
    host_print(buf);
}

static unsigned int hash_name(const char* name) {
    unsigned int h = 0x811C9DC5;
    while (*name) { h ^= (unsigned int)*name++; h *= 0x01000193; }
    return h;
}

int iot_init(void) {
    platform = (IoTPlatform*)host_alloc(sizeof(IoTPlatform), 4);
    if (!platform) return STATUS_ERR;
    platform->device_count = 0;
    platform->rule_count = 0;
    platform->topic_count = 0;
    platform->total_messages = 0;
    print_str("[IoT] Platform initialized\n");
    return STATUS_OK;
}

int iot_register_device(const char* name, int protocol, int auth) {
    if (platform->device_count >= MAX_DEVICES) return STATUS_ERR;
    int id = platform->device_count++;
    Device* d = &platform->devices[id];
    d->device_id = id;
    d->name_hash = hash_name(name);
    d->protocol = protocol;
    d->auth_method = auth;
    d->connected = 0;
    d->last_telemetry = 0.0f;
    d->firmware_version = 0x0100;
    d->active = 1;
    DeviceShadow* s = &platform->shadows[id];
    s->device_id = id;
    s->desired_state = 0.0f;
    s->reported_state = 0.0f;
    s->version = 1;
    s->synced = 1;
    print_str("[IoT] Registered device ");
    print_int(id);
    print_str(" proto=");
    print_hex((unsigned int)protocol);
    print_str(" auth=");
    print_hex((unsigned int)auth);
    print_str("\n");
    return id;
}

int iot_connect_device(int device_id) {
    if (device_id < 0 || device_id >= platform->device_count) return STATUS_ERR;
    Device* d = &platform->devices[device_id];
    if (!d->active) return STATUS_ERR;
    d->connected = 1;
    print_str("[IoT] Device ");
    print_int(device_id);
    print_str(" connected\n");
    return STATUS_OK;
}

int iot_send_telemetry(int device_id, float value) {
    if (device_id < 0 || device_id >= platform->device_count) return STATUS_ERR;
    Device* d = &platform->devices[device_id];
    if (!d->connected) return STATUS_ERR;
    d->last_telemetry = value;
    platform->shadows[device_id].reported_state = value;
    platform->total_messages++;
    return STATUS_OK;
}

int iot_add_rule(int device_id, int condition, float threshold, int action) {
    if (platform->rule_count >= MAX_RULES) return STATUS_ERR;
    int id = platform->rule_count++;
    Rule* r = &platform->rules[id];
    r->rule_id = id;
    r->device_id = device_id;
    r->condition_type = condition;
    r->threshold = threshold;
    r->action_type = action;
    r->active = 1;
    print_str("[IoT] Added rule ");
    print_int(id);
    print_str(" for device ");
    print_int(device_id);
    print_str("\n");
    return id;
}

void iot_evaluate_rules(void) {
    int triggered = 0;
    for (int i = 0; i < platform->rule_count; i++) {
        Rule* r = &platform->rules[i];
        if (!r->active) continue;
        Device* d = &platform->devices[r->device_id];
        if (!d->connected) continue;
        float val = d->last_telemetry;
        int fire = 0;
        if (r->condition_type == 0 && val > r->threshold) fire = 1;
        if (r->condition_type == 1 && val < r->threshold) fire = 1;
        if (fire) triggered++;
    }
    print_str("[IoT] Rule evaluation: ");
    print_int(triggered);
    print_str(" triggered\n");
}

int iot_update_desired_state(int device_id, float desired) {
    if (device_id < 0 || device_id >= platform->device_count) return STATUS_ERR;
    DeviceShadow* s = &platform->shadows[device_id];
    s->desired_state = desired;
    s->version++;
    s->synced = 0;
    print_str("[IoT] Updated desired state for device ");
    print_int(device_id);
    print_str(" to ");
    print_int((int)desired);
    print_str("\n");
    return STATUS_OK;
}

int iot_sync_shadow(int device_id) {
    if (device_id < 0 || device_id >= platform->device_count) return STATUS_ERR;
    DeviceShadow* s = &platform->shadows[device_id];
    s->reported_state = s->desired_state;
    s->synced = 1;
    return STATUS_OK;
}

int iot_firmware_update(int device_id, int new_version) {
    if (device_id < 0 || device_id >= platform->device_count) return STATUS_ERR;
    Device* d = &platform->devices[device_id];
    if (!d->connected) return STATUS_ERR;
    int old = d->firmware_version;
    d->firmware_version = new_version;
    print_str("[IoT] Firmware update device ");
    print_int(device_id);
    print_str(": ");
    print_hex((unsigned int)old);
    print_str(" -> ");
    print_hex((unsigned int)new_version);
    print_str("\n");
    return STATUS_OK;
}

void iot_print_platform_stats(void) {
    print_str("[IoT] Platform Statistics:\n");
    print_str("  Devices: ");
    print_int(platform->device_count);
    print_str("\n");
    print_str("  Connected: ");
    int conn = 0;
    for (int i = 0; i < platform->device_count; i++) {
        if (platform->devices[i].connected) conn++;
    }
    print_int(conn);
    print_str("\n");
    print_str("  Rules: ");
    print_int(platform->rule_count);
    print_str("\n");
    print_str("  Messages: ");
    print_int((int)platform->total_messages);
    print_str("\n");
}

void iot_print_device(int device_id) {
    if (device_id < 0 || device_id >= platform->device_count) return;
    Device* d = &platform->devices[device_id];
    print_str("Device ");
    print_int(device_id);
    print_str(": proto=");
    print_hex((unsigned int)d->protocol);
    print_str(" conn=");
    print_int(d->connected);
    print_str(" fw=");
    print_hex((unsigned int)d->firmware_version);
    print_str(" telemetry=");
    print_int((int)d->last_telemetry);
    print_str("\n");
}

int main(void) {
    print_str("=== IoT Platform Demo ===\n\n");
    iot_init();
    int d1 = iot_register_device("temp_sensor_01", PROTO_MQTT, AUTH_X509);
    int d2 = iot_register_device("humidity_01", PROTO_COAP, AUTH_PSK);
    int d3 = iot_register_device("gateway_01", PROTO_HTTP, AUTH_JWT);
    int d4 = iot_register_device("lora_node_01", PROTO_LORAWAN, AUTH_PSK);
    iot_connect_device(d1);
    iot_connect_device(d2);
    iot_connect_device(d3);
    iot_connect_device(d4);
    print_str("\nDevice states:\n");
    iot_print_device(d1);
    iot_print_device(d2);
    iot_print_device(d3);
    iot_print_device(d4);
    iot_add_rule(d1, 0, 30.0f, 1);
    iot_add_rule(d2, 1, 20.0f, 2);
    iot_send_telemetry(d1, 25.5f);
    iot_send_telemetry(d2, 45.0f);
    iot_send_telemetry(d3, 100.0f);
    iot_send_telemetry(d4, 12.3f);
    print_str("\nEvaluating rules...\n");
    iot_evaluate_rules();
    iot_update_desired_state(d1, 22.0f);
    iot_update_desired_state(d2, 50.0f);
    iot_sync_shadow(d1);
    iot_sync_shadow(d2);
    iot_firmware_update(d1, 0x0200);
    iot_firmware_update(d3, 0x0105);
    iot_print_platform_stats();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
