/* big_data_admin: Big data administration system (v1.0)
 * Data collection, data storage, data analysis, data governance, data application
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

#define MAX_COLLECTION   16
#define MAX_STORAGE      14
#define MAX_ANALYSIS     12
#define MAX_GOVERNANCE   10
#define MAX_APPLICATION  10

typedef struct {
    int    collection_id;
    int    collection_type;
    int    source_category;
    int    data_source_access;
    int    data_cleansing;
    int    data_integration;
    int    year;
    int    active;
} collection_t;

typedef struct {
    int    storage_id;
    int    storage_type;
    int    storage_category;
    int    distributed_storage;
    int    data_warehouse;
    int    data_lake;
    int    year;
    int    active;
} storage_t;

typedef struct {
    int    analysis_id;
    int    analysis_type;
    int    analysis_category;
    int    data_mining;
    int    data_visualization;
    int    intelligent_analysis;
    int    year;
    int    active;
} analysis_t;

typedef struct {
    int    governance_id;
    int    governance_type;
    int    governance_category;
    int    quality_management;
    int    data_security;
    int    data_standards;
    int    year;
    int    active;
} governance_t;

typedef struct {
    int    app_id;
    int    app_type;
    int    app_category;
    int    data_sharing;
    int    data_trading;
    int    data_services;
    int    year;
    int    active;
} app_t;

typedef struct {
    int    n_collection;
    int    n_storage;
    int    n_analysis;
    int    n_governance;
    int    n_app;
    int    total_access;
    int    total_distributed;
    int    total_mining;
    int    total_quality;
    int    total_sharing;
} bda_state_t;

static collection_t collections[MAX_COLLECTION];
static storage_t storages[MAX_STORAGE];
static analysis_t analyses[MAX_ANALYSIS];
static governance_t governances[MAX_GOVERNANCE];
static app_t apps[MAX_APPLICATION];
static bda_state_t bda;

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

int bda_init(void) {
    if (initialized) return -1;
    bda.n_collection = 0; bda.n_storage = 0; bda.n_analysis = 0;
    bda.n_governance = 0; bda.n_app = 0;
    bda.total_access = 0; bda.total_distributed = 0;
    bda.total_mining = 0; bda.total_quality = 0;
    bda.total_sharing = 0;
    for (int i = 0; i < MAX_COLLECTION; i++) collections[i].active = 0;
    for (int i = 0; i < MAX_STORAGE; i++) storages[i].active = 0;
    for (int i = 0; i < MAX_ANALYSIS; i++) analyses[i].active = 0;
    for (int i = 0; i < MAX_GOVERNANCE; i++) governances[i].active = 0;
    for (int i = 0; i < MAX_APPLICATION; i++) apps[i].active = 0;
    initialized = 1;
    print_str("[BDA] Big data initialized\n");
    return 0;
}

int bda_collection(int clt_type, int cat, int access, int cleansing, int integration, int year) {
    if (bda.n_collection >= MAX_COLLECTION) return -1;
    collection_t* c = &collections[bda.n_collection];
    c->collection_id = bda.n_collection;
    c->collection_type = clt_type;
    c->source_category = cat;
    c->data_source_access = access;
    c->data_cleansing = cleansing;
    c->data_integration = integration;
    c->year = year;
    c->active = 1;
    bda.total_access += access;
    bda.n_collection++;
    print_str("[BDA] Collection "); print_int(bda.n_collection - 1);
    print_str(" type="); print_int(clt_type);
    print_str(" cat="); print_int(cat);
    print_str(" acc="); print_int(access);
    print_str(" cln="); print_int(cleansing);
    print_str(" int="); print_int(integration); print_str("\n");
    return bda.n_collection - 1;
}

int bda_storage(int stg_type, int cat, int distributed, int warehouse, int lake, int year) {
    if (bda.n_storage >= MAX_STORAGE) return -1;
    storage_t* s = &storages[bda.n_storage];
    s->storage_id = bda.n_storage;
    s->storage_type = stg_type;
    s->storage_category = cat;
    s->distributed_storage = distributed;
    s->data_warehouse = warehouse;
    s->data_lake = lake;
    s->year = year;
    s->active = 1;
    bda.total_distributed += distributed;
    bda.n_storage++;
    print_str("[BDA] Storage "); print_int(bda.n_storage - 1);
    print_str(" type="); print_int(stg_type);
    print_str(" cat="); print_int(cat);
    print_str(" dst="); print_int(distributed);
    print_str(" whs="); print_int(warehouse);
    print_str(" lke="); print_int(lake); print_str("\n");
    return bda.n_storage - 1;
}

int bda_analysis(int ans_type, int cat, int mining, int visualization, int intelligent, int year) {
    if (bda.n_analysis >= MAX_ANALYSIS) return -1;
    analysis_t* a = &analyses[bda.n_analysis];
    a->analysis_id = bda.n_analysis;
    a->analysis_type = ans_type;
    a->analysis_category = cat;
    a->data_mining = mining;
    a->data_visualization = visualization;
    a->intelligent_analysis = intelligent;
    a->year = year;
    a->active = 1;
    bda.total_mining += mining;
    bda.n_analysis++;
    print_str("[BDA] Analysis "); print_int(bda.n_analysis - 1);
    print_str(" type="); print_int(ans_type);
    print_str(" cat="); print_int(cat);
    print_str(" mng="); print_int(mining);
    print_str(" vsl="); print_int(visualization);
    print_str(" int="); print_int(intelligent); print_str("\n");
    return bda.n_analysis - 1;
}

int bda_governance(int gov_type, int cat, int quality, int security, int standards, int year) {
    if (bda.n_governance >= MAX_GOVERNANCE) return -1;
    governance_t* g = &governances[bda.n_governance];
    g->governance_id = bda.n_governance;
    g->governance_type = gov_type;
    g->governance_category = cat;
    g->quality_management = quality;
    g->data_security = security;
    g->data_standards = standards;
    g->year = year;
    g->active = 1;
    bda.total_quality += quality;
    bda.n_governance++;
    print_str("[BDA] Governance "); print_int(bda.n_governance - 1);
    print_str(" type="); print_int(gov_type);
    print_str(" cat="); print_int(cat);
    print_str(" qlt="); print_int(quality);
    print_str(" sec="); print_int(security);
    print_str(" std="); print_int(standards); print_str("\n");
    return bda.n_governance - 1;
}

int bda_app(int ap_type, int cat, int sharing, int trading, int services, int year) {
    if (bda.n_app >= MAX_APPLICATION) return -1;
    app_t* a = &apps[bda.n_app];
    a->app_id = bda.n_app;
    a->app_type = ap_type;
    a->app_category = cat;
    a->data_sharing = sharing;
    a->data_trading = trading;
    a->data_services = services;
    a->year = year;
    a->active = 1;
    bda.total_sharing += sharing;
    bda.n_app++;
    print_str("[BDA] App "); print_int(bda.n_app - 1);
    print_str(" type="); print_int(ap_type);
    print_str(" cat="); print_int(cat);
    print_str(" shr="); print_int(sharing);
    print_str(" trd="); print_int(trading);
    print_str(" svc="); print_int(services); print_str("\n");
    return bda.n_app - 1;
}

void bda_collection_report(void) {
    print_str("[BDA] Collection report:\n");
    print_str("  Data collection categories: "); print_int(bda.n_collection); print_str("\n");
    print_str("  Total data source access: "); print_int(bda.total_access); print_str("\n");
}

void bda_storage_report(void) {
    print_str("[BDA] Storage report:\n");
    print_str("  Data storage categories: "); print_int(bda.n_storage); print_str("\n");
    print_str("  Total distributed storage: "); print_int(bda.total_distributed); print_str("\n");
}

void bda_app_report(void) {
    print_str("[BDA] App report:\n");
    print_str("  Analysis categories: "); print_int(bda.n_analysis); print_str("\n");
    print_str("  Total data mining: "); print_int(bda.total_mining); print_str("\n");
    print_str("  Governance categories: "); print_int(bda.n_governance); print_str("\n");
    print_str("  Total quality management: "); print_int(bda.total_quality); print_str("\n");
    print_str("  App categories: "); print_int(bda.n_app); print_str("\n");
    print_str("  Total data sharing: "); print_int(bda.total_sharing); print_str("\n");
}

void bda_print_state(void) {
    print_str("[BDA] Cl="); print_int(bda.n_collection);
    print_str(" St="); print_int(bda.n_storage);
    print_str(" An="); print_int(bda.n_analysis);
    print_str(" Gv="); print_int(bda.n_governance);
    print_str(" Ap="); print_int(bda.n_app);
    print_str("\n");
}

int main(void) {
    print_str("=== Big Data Admin Demo ===\n\n");
    bda_init();

    print_str("Data collection...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int acc = 100 + (i * 25);
        int cln = 80 + (i * 20);
        int intl = 60 + (i * 15);
        int year = 2020 + (i % 5);
        bda_collection(type, cat, acc, cln, intl, year);
    }

    print_str("\nData storage...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int dst = 50 + (i * 12);
        int whs = 30 + (i * 8);
        int lke = 20 + (i * 5);
        int year = 2021 + (i % 4);
        bda_storage(type, cat, dst, whs, lke, year);
    }

    print_str("\nData analysis...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int mng = 40 + (i * 10);
        int vsl = 25 + (i * 6);
        int intl = 20 + (i * 5);
        int year = 2022 + (i % 3);
        bda_analysis(type, cat, mng, vsl, intl, year);
    }

    print_str("\nData governance...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int qlt = 30 + (i * 8);
        int sec = 20 + (i * 5);
        int std = 15 + (i * 4);
        int year = 2023 + (i % 2);
        bda_governance(type, cat, qlt, sec, std, year);
    }

    print_str("\nData application...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int shr = 35 + (i * 9);
        int trd = 15 + (i * 4);
        int svc = 25 + (i * 6);
        int year = 2024;
        bda_app(type, cat, shr, trd, svc, year);
    }

    print_str("\nCollection report...\n");
    bda_collection_report();

    print_str("\nStorage report...\n");
    bda_storage_report();

    print_str("\nApp report...\n");
    bda_app_report();

    print_str("\nFinal state...\n");
    bda_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
