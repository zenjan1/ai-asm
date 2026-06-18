/* industrial_internet: Industrial internet administration system (v1.0)
 * Platform construction, device connection, data application, app ecosystem, security
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

#define MAX_PLATFORM     16
#define MAX_DEVICE       14
#define MAX_DATA         12
#define MAX_APP          10
#define MAX_SECURITY     10

typedef struct {
    int    platform_id;
    int    platform_type;
    int    platform_level;
    int    cross_platform;
    int    industry_platform;
    int    regional_platform;
    int    year;
    int    active;
} platform_t;

typedef struct {
    int    device_id;
    int    device_type;
    int    device_category;
    int    device_access;
    int    protocol_adaptation;
    int    data_collection;
    int    year;
    int    active;
} device_t;

typedef struct {
    int    data_id;
    int    data_type;
    int    data_category;
    int    data_analysis;
    int    data_modeling;
    int    data_optimization;
    int    year;
    int    active;
} data_t;

typedef struct {
    int    app_id;
    int    app_type;
    int    app_category;
    int    industrial_apps;
    int    industrial_saas;
    int    solutions;
    int    year;
    int    active;
} app_t;

typedef struct {
    int    security_id;
    int    security_type;
    int    security_category;
    int    device_security;
    int    network_security;
    int    data_security;
    int    year;
    int    active;
} security_t;

typedef struct {
    int    n_platform;
    int    n_device;
    int    n_data;
    int    n_app;
    int    n_security;
    int    total_cross;
    int    total_access;
    int    total_analysis;
    int    total_apps;
    int    total_device_sec;
} ii_state_t;

static platform_t platforms[MAX_PLATFORM];
static device_t devices[MAX_DEVICE];
static data_t data_list[MAX_DATA];
static app_t apps[MAX_APP];
static security_t securities[MAX_SECURITY];
static ii_state_t ii;

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

int ii_init(void) {
    if (initialized) return -1;
    ii.n_platform = 0; ii.n_device = 0; ii.n_data = 0;
    ii.n_app = 0; ii.n_security = 0;
    ii.total_cross = 0; ii.total_access = 0;
    ii.total_analysis = 0; ii.total_apps = 0;
    ii.total_device_sec = 0;
    for (int i = 0; i < MAX_PLATFORM; i++) platforms[i].active = 0;
    for (int i = 0; i < MAX_DEVICE; i++) devices[i].active = 0;
    for (int i = 0; i < MAX_DATA; i++) data_list[i].active = 0;
    for (int i = 0; i < MAX_APP; i++) apps[i].active = 0;
    for (int i = 0; i < MAX_SECURITY; i++) securities[i].active = 0;
    initialized = 1;
    print_str("[II] Industrial internet initialized\n");
    return 0;
}

int ii_platform(int plt_type, int level, int cross, int industry, int regional, int year) {
    if (ii.n_platform >= MAX_PLATFORM) return -1;
    platform_t* p = &platforms[ii.n_platform];
    p->platform_id = ii.n_platform;
    p->platform_type = plt_type;
    p->platform_level = level;
    p->cross_platform = cross;
    p->industry_platform = industry;
    p->regional_platform = regional;
    p->year = year;
    p->active = 1;
    ii.total_cross += cross;
    ii.n_platform++;
    print_str("[II] Platform "); print_int(ii.n_platform - 1);
    print_str(" type="); print_int(plt_type);
    print_str(" lvl="); print_int(level);
    print_str(" crs="); print_int(cross);
    print_str(" ind="); print_int(industry);
    print_str(" rgn="); print_int(regional); print_str("\n");
    return ii.n_platform - 1;
}

int ii_device(int dev_type, int cat, int access, int protocol, int collection, int year) {
    if (ii.n_device >= MAX_DEVICE) return -1;
    device_t* d = &devices[ii.n_device];
    d->device_id = ii.n_device;
    d->device_type = dev_type;
    d->device_category = cat;
    d->device_access = access;
    d->protocol_adaptation = protocol;
    d->data_collection = collection;
    d->year = year;
    d->active = 1;
    ii.total_access += access;
    ii.n_device++;
    print_str("[II] Device "); print_int(ii.n_device - 1);
    print_str(" type="); print_int(dev_type);
    print_str(" cat="); print_int(cat);
    print_str(" acc="); print_int(access);
    print_str(" prt="); print_int(protocol);
    print_str(" col="); print_int(collection); print_str("\n");
    return ii.n_device - 1;
}

int ii_data(int dat_type, int cat, int analysis, int modeling, int optimization, int year) {
    if (ii.n_data >= MAX_DATA) return -1;
    data_t* dt = &data_list[ii.n_data];
    dt->data_id = ii.n_data;
    dt->data_type = dat_type;
    dt->data_category = cat;
    dt->data_analysis = analysis;
    dt->data_modeling = modeling;
    dt->data_optimization = optimization;
    dt->year = year;
    dt->active = 1;
    ii.total_analysis += analysis;
    ii.n_data++;
    print_str("[II] Data "); print_int(ii.n_data - 1);
    print_str(" type="); print_int(dat_type);
    print_str(" cat="); print_int(cat);
    print_str(" ans="); print_int(analysis);
    print_str(" mdl="); print_int(modeling);
    print_str(" opt="); print_int(optimization); print_str("\n");
    return ii.n_data - 1;
}

int ii_app(int ap_type, int cat, int ind_apps, int saas, int solutions, int year) {
    if (ii.n_app >= MAX_APP) return -1;
    app_t* a = &apps[ii.n_app];
    a->app_id = ii.n_app;
    a->app_type = ap_type;
    a->app_category = cat;
    a->industrial_apps = ind_apps;
    a->industrial_saas = saas;
    a->solutions = solutions;
    a->year = year;
    a->active = 1;
    ii.total_apps += ind_apps;
    ii.n_app++;
    print_str("[II] App "); print_int(ii.n_app - 1);
    print_str(" type="); print_int(ap_type);
    print_str(" cat="); print_int(cat);
    print_str(" iap="); print_int(ind_apps);
    print_str(" sas="); print_int(saas);
    print_str(" sol="); print_int(solutions); print_str("\n");
    return ii.n_app - 1;
}

int ii_security(int scr_type, int cat, int device_sec, int network_sec, int data_sec, int year) {
    if (ii.n_security >= MAX_SECURITY) return -1;
    security_t* s = &securities[ii.n_security];
    s->security_id = ii.n_security;
    s->security_type = scr_type;
    s->security_category = cat;
    s->device_security = device_sec;
    s->network_security = network_sec;
    s->data_security = data_sec;
    s->year = year;
    s->active = 1;
    ii.total_device_sec += device_sec;
    ii.n_security++;
    print_str("[II] Security "); print_int(ii.n_security - 1);
    print_str(" type="); print_int(scr_type);
    print_str(" cat="); print_int(cat);
    print_str(" dvc="); print_int(device_sec);
    print_str(" ntw="); print_int(network_sec);
    print_str(" dat="); print_int(data_sec); print_str("\n");
    return ii.n_security - 1;
}

void ii_platform_report(void) {
    print_str("[II] Platform report:\n");
    print_str("  Platform categories: "); print_int(ii.n_platform); print_str("\n");
    print_str("  Total cross-platform: "); print_int(ii.total_cross); print_str("\n");
}

void ii_device_report(void) {
    print_str("[II] Device report:\n");
    print_str("  Device categories: "); print_int(ii.n_device); print_str("\n");
    print_str("  Total device access: "); print_int(ii.total_access); print_str("\n");
}

void ii_app_report(void) {
    print_str("[II] App report:\n");
    print_str("  Data categories: "); print_int(ii.n_data); print_str("\n");
    print_str("  Total data analysis: "); print_int(ii.total_analysis); print_str("\n");
    print_str("  App categories: "); print_int(ii.n_app); print_str("\n");
    print_str("  Total industrial apps: "); print_int(ii.total_apps); print_str("\n");
    print_str("  Security categories: "); print_int(ii.n_security); print_str("\n");
    print_str("  Total device security: "); print_int(ii.total_device_sec); print_str("\n");
}

void ii_print_state(void) {
    print_str("[II] Pl="); print_int(ii.n_platform);
    print_str(" Dv="); print_int(ii.n_device);
    print_str(" Dt="); print_int(ii.n_data);
    print_str(" Ap="); print_int(ii.n_app);
    print_str(" Sc="); print_int(ii.n_security);
    print_str("\n");
}

int main(void) {
    print_str("=== Industrial Internet Demo ===\n\n");
    ii_init();

    print_str("Platform construction...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int level = (i % 4) + 1;
        int crs = 10 + (i * 3);
        int ind = 15 + (i * 4);
        int rgn = 20 + (i * 5);
        int year = 2020 + (i % 5);
        ii_platform(type, level, crs, ind, rgn, year);
    }

    print_str("\nDevice connection...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int acc = 100 + (i * 25);
        int prt = 80 + (i * 20);
        int col = 60 + (i * 15);
        int year = 2021 + (i % 4);
        ii_device(type, cat, acc, prt, col, year);
    }

    print_str("\nData application...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ans = 50 + (i * 12);
        int mdl = 30 + (i * 8);
        int opt = 20 + (i * 5);
        int year = 2022 + (i % 3);
        ii_data(type, cat, ans, mdl, opt, year);
    }

    print_str("\nApp ecosystem...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int iap = 40 + (i * 10);
        int sas = 25 + (i * 6);
        int sol = 15 + (i * 4);
        int year = 2023 + (i % 2);
        ii_app(type, cat, iap, sas, sol, year);
    }

    print_str("\nSecurity protection...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int dvc = 30 + (i * 8);
        int ntw = 25 + (i * 6);
        int dat = 20 + (i * 5);
        int year = 2024;
        ii_security(type, cat, dvc, ntw, dat, year);
    }

    print_str("\nPlatform report...\n");
    ii_platform_report();

    print_str("\nDevice report...\n");
    ii_device_report();

    print_str("\nApp report...\n");
    ii_app_report();

    print_str("\nFinal state...\n");
    ii_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
