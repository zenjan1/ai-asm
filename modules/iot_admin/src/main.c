/* iot_admin: IoT administration system (v1.0)
 * Perception layer, network layer, platform layer, application layer, security layer
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

#define MAX_PERCEPTION   16
#define MAX_NETWORK      14
#define MAX_PLATFORM     12
#define MAX_APPLICATION  10
#define MAX_SECURITY     10

typedef struct {
    int    perception_id;
    int    perception_type;
    int    perception_category;
    int    sensor;
    int    rfid;
    int    camera;
    int    year;
    int    active;
} perception_t;

typedef struct {
    int    network_id;
    int    network_type;
    int    network_category;
    int    protocol;
    int    gateway;
    int    data_transfer;
    int    year;
    int    active;
} network_t;

typedef struct {
    int    platform_id;
    int    platform_type;
    int    platform_category;
    int    device_manage;
    int    data_manage;
    int    app_enable;
    int    year;
    int    active;
} platform_t;

typedef struct {
    int    app_id;
    int    app_type;
    int    app_category;
    int    smart_home;
    int    smart_industry;
    int    smart_agriculture;
    int    year;
    int    active;
} app_t;

typedef struct {
    int    security_id;
    int    security_type;
    int    security_category;
    int    device_auth;
    int    data_encrypt;
    int    security_audit;
    int    year;
    int    active;
} security_t;

typedef struct {
    int    n_perception;
    int    n_network;
    int    n_platform;
    int    n_app;
    int    n_security;
    int    total_sensor;
    int    total_protocol;
    int    total_device;
    int    total_home;
    int    total_auth;
} iot_state_t;

static perception_t perceptions[MAX_PERCEPTION];
static network_t networks[MAX_NETWORK];
static platform_t platforms[MAX_PLATFORM];
static app_t apps[MAX_APPLICATION];
static security_t securities[MAX_SECURITY];
static iot_state_t iot;

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

int iot_init(void) {
    if (initialized) return -1;
    iot.n_perception = 0; iot.n_network = 0; iot.n_platform = 0;
    iot.n_app = 0; iot.n_security = 0;
    iot.total_sensor = 0; iot.total_protocol = 0;
    iot.total_device = 0; iot.total_home = 0;
    iot.total_auth = 0;
    for (int i = 0; i < MAX_PERCEPTION; i++) perceptions[i].active = 0;
    for (int i = 0; i < MAX_NETWORK; i++) networks[i].active = 0;
    for (int i = 0; i < MAX_PLATFORM; i++) platforms[i].active = 0;
    for (int i = 0; i < MAX_APPLICATION; i++) apps[i].active = 0;
    for (int i = 0; i < MAX_SECURITY; i++) securities[i].active = 0;
    initialized = 1;
    print_str("[IOT] IoT initialized\n");
    return 0;
}

int iot_perception(int pct_type, int cat, int sensor, int rfid, int camera, int year) {
    if (iot.n_perception >= MAX_PERCEPTION) return -1;
    perception_t* p = &perceptions[iot.n_perception];
    p->perception_id = iot.n_perception;
    p->perception_type = pct_type;
    p->perception_category = cat;
    p->sensor = sensor;
    p->rfid = rfid;
    p->camera = camera;
    p->year = year;
    p->active = 1;
    iot.total_sensor += sensor;
    iot.n_perception++;
    print_str("[IOT] Perception "); print_int(iot.n_perception - 1);
    print_str(" type="); print_int(pct_type);
    print_str(" cat="); print_int(cat);
    print_str(" sns="); print_int(sensor);
    print_str(" rfd="); print_int(rfid);
    print_str(" cam="); print_int(camera); print_str("\n");
    return iot.n_perception - 1;
}

int iot_network(int ntw_type, int cat, int protocol, int gateway, int transfer, int year) {
    if (iot.n_network >= MAX_NETWORK) return -1;
    network_t* n = &networks[iot.n_network];
    n->network_id = iot.n_network;
    n->network_type = ntw_type;
    n->network_category = cat;
    n->protocol = protocol;
    n->gateway = gateway;
    n->data_transfer = transfer;
    n->year = year;
    n->active = 1;
    iot.total_protocol += protocol;
    iot.n_network++;
    print_str("[IOT] Network "); print_int(iot.n_network - 1);
    print_str(" type="); print_int(ntw_type);
    print_str(" cat="); print_int(cat);
    print_str(" prt="); print_int(protocol);
    print_str(" gtw="); print_int(gateway);
    print_str(" trf="); print_int(transfer); print_str("\n");
    return iot.n_network - 1;
}

int iot_platform(int plt_type, int cat, int device, int data, int enable, int year) {
    if (iot.n_platform >= MAX_PLATFORM) return -1;
    platform_t* p = &platforms[iot.n_platform];
    p->platform_id = iot.n_platform;
    p->platform_type = plt_type;
    p->platform_category = cat;
    p->device_manage = device;
    p->data_manage = data;
    p->app_enable = enable;
    p->year = year;
    p->active = 1;
    iot.total_device += device;
    iot.n_platform++;
    print_str("[IOT] Platform "); print_int(iot.n_platform - 1);
    print_str(" type="); print_int(plt_type);
    print_str(" cat="); print_int(cat);
    print_str(" dvc="); print_int(device);
    print_str(" dat="); print_int(data);
    print_str(" ena="); print_int(enable); print_str("\n");
    return iot.n_platform - 1;
}

int iot_app(int ap_type, int cat, int home, int industry, int agriculture, int year) {
    if (iot.n_app >= MAX_APPLICATION) return -1;
    app_t* a = &apps[iot.n_app];
    a->app_id = iot.n_app;
    a->app_type = ap_type;
    a->app_category = cat;
    a->smart_home = home;
    a->smart_industry = industry;
    a->smart_agriculture = agriculture;
    a->year = year;
    a->active = 1;
    iot.total_home += home;
    iot.n_app++;
    print_str("[IOT] App "); print_int(iot.n_app - 1);
    print_str(" type="); print_int(ap_type);
    print_str(" cat="); print_int(cat);
    print_str(" hom="); print_int(home);
    print_str(" ind="); print_int(industry);
    print_str(" agr="); print_int(agriculture); print_str("\n");
    return iot.n_app - 1;
}

int iot_security(int sec_type, int cat, int auth, int encrypt, int audit, int year) {
    if (iot.n_security >= MAX_SECURITY) return -1;
    security_t* s = &securities[iot.n_security];
    s->security_id = iot.n_security;
    s->security_type = sec_type;
    s->security_category = cat;
    s->device_auth = auth;
    s->data_encrypt = encrypt;
    s->security_audit = audit;
    s->year = year;
    s->active = 1;
    iot.total_auth += auth;
    iot.n_security++;
    print_str("[IOT] Security "); print_int(iot.n_security - 1);
    print_str(" type="); print_int(sec_type);
    print_str(" cat="); print_int(cat);
    print_str(" auth="); print_int(auth);
    print_str(" enc="); print_int(encrypt);
    print_str(" aud="); print_int(audit); print_str("\n");
    return iot.n_security - 1;
}

void iot_perception_report(void) {
    print_str("[IOT] Perception report:\n");
    print_str("  Perception categories: "); print_int(iot.n_perception); print_str("\n");
    print_str("  Total sensors: "); print_int(iot.total_sensor); print_str("\n");
}

void iot_network_report(void) {
    print_str("[IOT] Network report:\n");
    print_str("  Network categories: "); print_int(iot.n_network); print_str("\n");
    print_str("  Total protocols: "); print_int(iot.total_protocol); print_str("\n");
}

void iot_app_report(void) {
    print_str("[IOT] App report:\n");
    print_str("  Platform categories: "); print_int(iot.n_platform); print_str("\n");
    print_str("  Total device manage: "); print_int(iot.total_device); print_str("\n");
    print_str("  App categories: "); print_int(iot.n_app); print_str("\n");
    print_str("  Total smart home: "); print_int(iot.total_home); print_str("\n");
    print_str("  Security categories: "); print_int(iot.n_security); print_str("\n");
    print_str("  Total device auth: "); print_int(iot.total_auth); print_str("\n");
}

void iot_print_state(void) {
    print_str("[IOT] Pc="); print_int(iot.n_perception);
    print_str(" Nw="); print_int(iot.n_network);
    print_str(" Pl="); print_int(iot.n_platform);
    print_str(" Ap="); print_int(iot.n_app);
    print_str(" Sc="); print_int(iot.n_security);
    print_str("\n");
}

int main(void) {
    print_str("=== IoT Admin Demo ===\n\n");
    iot_init();

    print_str("Perception layer...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int sns = 50 + (i * 12);
        int rfd = 30 + (i * 8);
        int cam = 20 + (i * 5);
        int year = 2020 + (i % 5);
        iot_perception(type, cat, sns, rfd, cam, year);
    }

    print_str("\nNetwork layer...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int prt = 40 + (i * 10);
        int gtw = 25 + (i * 6);
        int trf = 60 + (i * 15);
        int year = 2021 + (i % 4);
        iot_network(type, cat, prt, gtw, trf, year);
    }

    print_str("\nPlatform layer...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int dvc = 35 + (i * 9);
        int dat = 25 + (i * 6);
        int ena = 18 + (i * 4);
        int year = 2022 + (i % 3);
        iot_platform(type, cat, dvc, dat, ena, year);
    }

    print_str("\nApplication layer...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int hom = 30 + (i * 8);
        int ind = 20 + (i * 5);
        int agr = 15 + (i * 4);
        int year = 2023 + (i % 2);
        iot_app(type, cat, hom, ind, agr, year);
    }

    print_str("\nSecurity layer...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int auth = 25 + (i * 6);
        int enc = 20 + (i * 5);
        int aud = 15 + (i * 4);
        int year = 2024;
        iot_security(type, cat, auth, enc, aud, year);
    }

    print_str("\nPerception report...\n");
    iot_perception_report();

    print_str("\nNetwork report...\n");
    iot_network_report();

    print_str("\nApp report...\n");
    iot_app_report();

    print_str("\nFinal state...\n");
    iot_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
