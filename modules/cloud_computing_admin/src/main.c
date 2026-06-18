/* cloud_computing_admin: Cloud computing administration system (v1.0)
 * Cloud services, cloud resources, cloud platforms, cloud applications, cloud ecosystem
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

#define MAX_SERVICE      16
#define MAX_RESOURCE     14
#define MAX_PLATFORM     12
#define MAX_APPLICATION  10
#define MAX_ECOSYSTEM    10

typedef struct {
    int    service_id;
    int    service_type;
    int    service_model;
    int    public_cloud;
    int    private_cloud;
    int    hybrid_cloud;
    int    year;
    int    active;
} service_t;

typedef struct {
    int    resource_id;
    int    resource_type;
    int    resource_category;
    int    compute_resources;
    int    storage_resources;
    int    network_resources;
    int    year;
    int    active;
} resource_t;

typedef struct {
    int    platform_id;
    int    platform_type;
    int    platform_category;
    int    platform_construction;
    int    platform_operations;
    int    platform_security;
    int    year;
    int    active;
} platform_t;

typedef struct {
    int    app_id;
    int    app_type;
    int    app_category;
    int    app_development;
    int    app_deployment;
    int    app_monitoring;
    int    year;
    int    active;
} app_t;

typedef struct {
    int    ecosystem_id;
    int    ecosystem_type;
    int    ecosystem_category;
    int    cloud_providers;
    int    service_catalog;
    int    billing_settlement;
    int    year;
    int    active;
} ecosystem_t;

typedef struct {
    int    n_service;
    int    n_resource;
    int    n_platform;
    int    n_app;
    int    n_ecosystem;
    int    total_public;
    int    total_compute;
    int    total_construction;
    int    total_development;
    int    total_providers;
} cca_state_t;

static service_t services[MAX_SERVICE];
static resource_t resources[MAX_RESOURCE];
static platform_t platforms[MAX_PLATFORM];
static app_t apps[MAX_APPLICATION];
static ecosystem_t ecosystems[MAX_ECOSYSTEM];
static cca_state_t cca;

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

int cca_init(void) {
    if (initialized) return -1;
    cca.n_service = 0; cca.n_resource = 0; cca.n_platform = 0;
    cca.n_app = 0; cca.n_ecosystem = 0;
    cca.total_public = 0; cca.total_compute = 0;
    cca.total_construction = 0; cca.total_development = 0;
    cca.total_providers = 0;
    for (int i = 0; i < MAX_SERVICE; i++) services[i].active = 0;
    for (int i = 0; i < MAX_RESOURCE; i++) resources[i].active = 0;
    for (int i = 0; i < MAX_PLATFORM; i++) platforms[i].active = 0;
    for (int i = 0; i < MAX_APPLICATION; i++) apps[i].active = 0;
    for (int i = 0; i < MAX_ECOSYSTEM; i++) ecosystems[i].active = 0;
    initialized = 1;
    print_str("[CCA] Cloud computing initialized\n");
    return 0;
}

int cca_service(int svc_type, int model, int public_cnt, int private_cnt, int hybrid, int year) {
    if (cca.n_service >= MAX_SERVICE) return -1;
    service_t* s = &services[cca.n_service];
    s->service_id = cca.n_service;
    s->service_type = svc_type;
    s->service_model = model;
    s->public_cloud = public_cnt;
    s->private_cloud = private_cnt;
    s->hybrid_cloud = hybrid;
    s->year = year;
    s->active = 1;
    cca.total_public += public_cnt;
    cca.n_service++;
    print_str("[CCA] Service "); print_int(cca.n_service - 1);
    print_str(" type="); print_int(svc_type);
    print_str(" mdl="); print_int(model);
    print_str(" pub="); print_int(public_cnt);
    print_str(" prv="); print_int(private_cnt);
    print_str(" hyb="); print_int(hybrid); print_str("\n");
    return cca.n_service - 1;
}

int cca_resource(int rsc_type, int cat, int compute, int storage, int network, int year) {
    if (cca.n_resource >= MAX_RESOURCE) return -1;
    resource_t* r = &resources[cca.n_resource];
    r->resource_id = cca.n_resource;
    r->resource_type = rsc_type;
    r->resource_category = cat;
    r->compute_resources = compute;
    r->storage_resources = storage;
    r->network_resources = network;
    r->year = year;
    r->active = 1;
    cca.total_compute += compute;
    cca.n_resource++;
    print_str("[CCA] Resource "); print_int(cca.n_resource - 1);
    print_str(" type="); print_int(rsc_type);
    print_str(" cat="); print_int(cat);
    print_str(" cmp="); print_int(compute);
    print_str(" stg="); print_int(storage);
    print_str(" ntw="); print_int(network); print_str("\n");
    return cca.n_resource - 1;
}

int cca_platform(int plt_type, int cat, int construction, int operations, int security, int year) {
    if (cca.n_platform >= MAX_PLATFORM) return -1;
    platform_t* p = &platforms[cca.n_platform];
    p->platform_id = cca.n_platform;
    p->platform_type = plt_type;
    p->platform_category = cat;
    p->platform_construction = construction;
    p->platform_operations = operations;
    p->platform_security = security;
    p->year = year;
    p->active = 1;
    cca.total_construction += construction;
    cca.n_platform++;
    print_str("[CCA] Platform "); print_int(cca.n_platform - 1);
    print_str(" type="); print_int(plt_type);
    print_str(" cat="); print_int(cat);
    print_str(" cst="); print_int(construction);
    print_str(" ops="); print_int(operations);
    print_str(" sec="); print_int(security); print_str("\n");
    return cca.n_platform - 1;
}

int cca_app(int ap_type, int cat, int development, int deployment, int monitoring, int year) {
    if (cca.n_app >= MAX_APPLICATION) return -1;
    app_t* a = &apps[cca.n_app];
    a->app_id = cca.n_app;
    a->app_type = ap_type;
    a->app_category = cat;
    a->app_development = development;
    a->app_deployment = deployment;
    a->app_monitoring = monitoring;
    a->year = year;
    a->active = 1;
    cca.total_development += development;
    cca.n_app++;
    print_str("[CCA] App "); print_int(cca.n_app - 1);
    print_str(" type="); print_int(ap_type);
    print_str(" cat="); print_int(cat);
    print_str(" dvp="); print_int(development);
    print_str(" dpl="); print_int(deployment);
    print_str(" mon="); print_int(monitoring); print_str("\n");
    return cca.n_app - 1;
}

int cca_ecosystem(int eco_type, int cat, int providers, int catalog, int billing, int year) {
    if (cca.n_ecosystem >= MAX_ECOSYSTEM) return -1;
    ecosystem_t* e = &ecosystems[cca.n_ecosystem];
    e->ecosystem_id = cca.n_ecosystem;
    e->ecosystem_type = eco_type;
    e->ecosystem_category = cat;
    e->cloud_providers = providers;
    e->service_catalog = catalog;
    e->billing_settlement = billing;
    e->year = year;
    e->active = 1;
    cca.total_providers += providers;
    cca.n_ecosystem++;
    print_str("[CCA] Ecosystem "); print_int(cca.n_ecosystem - 1);
    print_str(" type="); print_int(eco_type);
    print_str(" cat="); print_int(cat);
    print_str(" prd="); print_int(providers);
    print_str(" ctl="); print_int(catalog);
    print_str(" bil="); print_int(billing); print_str("\n");
    return cca.n_ecosystem - 1;
}

void cca_service_report(void) {
    print_str("[CCA] Service report:\n");
    print_str("  Cloud service categories: "); print_int(cca.n_service); print_str("\n");
    print_str("  Total public cloud: "); print_int(cca.total_public); print_str("\n");
}

void cca_resource_report(void) {
    print_str("[CCA] Resource report:\n");
    print_str("  Cloud resource categories: "); print_int(cca.n_resource); print_str("\n");
    print_str("  Total compute resources: "); print_int(cca.total_compute); print_str("\n");
}

void cca_ecosystem_report(void) {
    print_str("[CCA] Ecosystem report:\n");
    print_str("  Platform categories: "); print_int(cca.n_platform); print_str("\n");
    print_str("  Total platform construction: "); print_int(cca.total_construction); print_str("\n");
    print_str("  App categories: "); print_int(cca.n_app); print_str("\n");
    print_str("  Total app development: "); print_int(cca.total_development); print_str("\n");
    print_str("  Ecosystem categories: "); print_int(cca.n_ecosystem); print_str("\n");
    print_str("  Total cloud providers: "); print_int(cca.total_providers); print_str("\n");
}

void cca_print_state(void) {
    print_str("[CCA] Sv="); print_int(cca.n_service);
    print_str(" Rs="); print_int(cca.n_resource);
    print_str(" Pl="); print_int(cca.n_platform);
    print_str(" Ap="); print_int(cca.n_app);
    print_str(" Ec="); print_int(cca.n_ecosystem);
    print_str("\n");
}

int main(void) {
    print_str("=== Cloud Computing Admin Demo ===\n\n");
    cca_init();

    print_str("Cloud services...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int model = (i % 3) + 1;
        int pub = 50 + (i * 12);
        int prv = 30 + (i * 8);
        int hyb = 20 + (i * 5);
        int year = 2020 + (i % 5);
        cca_service(type, model, pub, prv, hyb, year);
    }

    print_str("\nCloud resources...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cmp = 100 + (i * 25);
        int stg = 200 + (i * 50);
        int ntw = 80 + (i * 20);
        int year = 2021 + (i % 4);
        cca_resource(type, cat, cmp, stg, ntw, year);
    }

    print_str("\nCloud platforms...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cst = 15 + (i * 4);
        int ops = 40 + (i * 10);
        int sec = 25 + (i * 6);
        int year = 2022 + (i % 3);
        cca_platform(type, cat, cst, ops, sec, year);
    }

    print_str("\nCloud applications...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int dvp = 30 + (i * 8);
        int dpl = 20 + (i * 5);
        int mon = 15 + (i * 4);
        int year = 2023 + (i % 2);
        cca_app(type, cat, dvp, dpl, mon, year);
    }

    print_str("\nCloud ecosystem...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int prd = 10 + (i * 3);
        int ctl = 20 + (i * 5);
        int bil = 15 + (i * 4);
        int year = 2024;
        cca_ecosystem(type, cat, prd, ctl, bil, year);
    }

    print_str("\nService report...\n");
    cca_service_report();

    print_str("\nResource report...\n");
    cca_resource_report();

    print_str("\nEcosystem report...\n");
    cca_ecosystem_report();

    print_str("\nFinal state...\n");
    cca_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
