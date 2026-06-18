/* food_security: Food security administration system (v1.0)
 * Grain management, quality supervision, storage logistics, emergency, market regulation
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

#define MAX_GRAIN        16
#define MAX_QUALITY      14
#define MAX_STORAGE      12
#define MAX_EMERGENCY    10
#define MAX_MARKET       10

typedef struct {
    int    grain_id;
    int    grain_type;
    int    production_region;
    int    production_volume;
    int    procurement_volume;
    int    reserve_volume;
    int    year;
    int    active;
} grain_t;

typedef struct {
    int    quality_id;
    int    inspection_type;
    int    food_category;
    int    samples_tested;
    int    pass_rate;
    int    traceability_records;
    int    year;
    int    active;
} quality_t;

typedef struct {
    int    storage_id;
    int    storage_type;
    int    facility_type;
    int    warehouses_operated;
    int    storage_capacity;
    int    deliveries_completed;
    int    year;
    int    active;
} storage_t;

typedef struct {
    int    emergency_id;
    int    emergency_type;
    int    response_level;
    int    reserves_deployed;
    int    allocations_made;
    int    beneficiaries;
    int    year;
    int    active;
} emergency_t;

typedef struct {
    int    market_id;
    int    market_type;
    int    commodity_category;
    int    price_monitors;
    int    regulations_enforced;
    int    import_export_volume;
    int    year;
    int    active;
} market_t;

typedef struct {
    int    n_grain;
    int    n_quality;
    int    n_storage;
    int    n_emergency;
    int    n_market;
    int    total_production;
    int    total_procurement;
    int    total_samples;
    int    total_capacity;
    int    total_reserves;
} fs_state_t;

static grain_t grains[MAX_GRAIN];
static quality_t qualities[MAX_QUALITY];
static storage_t storages[MAX_STORAGE];
static emergency_t emergencies[MAX_EMERGENCY];
static market_t markets[MAX_MARKET];
static fs_state_t fs;

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

int fs_init(void) {
    if (initialized) return -1;
    fs.n_grain = 0; fs.n_quality = 0; fs.n_storage = 0;
    fs.n_emergency = 0; fs.n_market = 0;
    fs.total_production = 0; fs.total_procurement = 0;
    fs.total_samples = 0; fs.total_capacity = 0;
    fs.total_reserves = 0;
    for (int i = 0; i < MAX_GRAIN; i++) grains[i].active = 0;
    for (int i = 0; i < MAX_QUALITY; i++) qualities[i].active = 0;
    for (int i = 0; i < MAX_STORAGE; i++) storages[i].active = 0;
    for (int i = 0; i < MAX_EMERGENCY; i++) emergencies[i].active = 0;
    for (int i = 0; i < MAX_MARKET; i++) markets[i].active = 0;
    initialized = 1;
    print_str("[FS] Food security initialized\n");
    return 0;
}

int fs_grain(int grain_type, int region, int production, int procurement, int reserve, int year) {
    if (fs.n_grain >= MAX_GRAIN) return -1;
    grain_t* g = &grains[fs.n_grain];
    g->grain_id = fs.n_grain;
    g->grain_type = grain_type;
    g->production_region = region;
    g->production_volume = production;
    g->procurement_volume = procurement;
    g->reserve_volume = reserve;
    g->year = year;
    g->active = 1;
    fs.total_production += production;
    fs.total_procurement += procurement;
    fs.total_reserves += reserve;
    fs.n_grain++;
    print_str("[FS] Grain "); print_int(fs.n_grain - 1);
    print_str(" type="); print_int(grain_type);
    print_str(" rgn="); print_int(region);
    print_str(" prd="); print_int(production);
    print_str(" prc="); print_int(procurement);
    print_str(" rsv="); print_int(reserve); print_str("\n");
    return fs.n_grain - 1;
}

int fs_quality(int insp_type, int food_cat, int samples, int pass_rate_val, int trace, int year) {
    if (fs.n_quality >= MAX_QUALITY) return -1;
    quality_t* q = &qualities[fs.n_quality];
    q->quality_id = fs.n_quality;
    q->inspection_type = insp_type;
    q->food_category = food_cat;
    q->samples_tested = samples;
    q->pass_rate = pass_rate_val;
    q->traceability_records = trace;
    q->year = year;
    q->active = 1;
    fs.total_samples += samples;
    fs.n_quality++;
    print_str("[FS] Quality "); print_int(fs.n_quality - 1);
    print_str(" type="); print_int(insp_type);
    print_str(" cat="); print_int(food_cat);
    print_str(" spl="); print_int(samples);
    print_str(" rate="); print_int(pass_rate_val);
    print_str(" trc="); print_int(trace); print_str("\n");
    return fs.n_quality - 1;
}

int fs_storage(int str_type, int facility, int warehouses_cnt, int capacity, int deliveries, int year) {
    if (fs.n_storage >= MAX_STORAGE) return -1;
    storage_t* s = &storages[fs.n_storage];
    s->storage_id = fs.n_storage;
    s->storage_type = str_type;
    s->facility_type = facility;
    s->warehouses_operated = warehouses_cnt;
    s->storage_capacity = capacity;
    s->deliveries_completed = deliveries;
    s->year = year;
    s->active = 1;
    fs.total_capacity += capacity;
    fs.n_storage++;
    print_str("[FS] Storage "); print_int(fs.n_storage - 1);
    print_str(" type="); print_int(str_type);
    print_str(" fac="); print_int(facility);
    print_str(" whs="); print_int(warehouses_cnt);
    print_str(" cap="); print_int(capacity);
    print_str(" dlv="); print_int(deliveries); print_str("\n");
    return fs.n_storage - 1;
}

int fs_emergency(int emg_type, int level, int deployed, int allocated, int beneficiaries_cnt, int year) {
    if (fs.n_emergency >= MAX_EMERGENCY) return -1;
    emergency_t* e = &emergencies[fs.n_emergency];
    e->emergency_id = fs.n_emergency;
    e->emergency_type = emg_type;
    e->response_level = level;
    e->reserves_deployed = deployed;
    e->allocations_made = allocated;
    e->beneficiaries = beneficiaries_cnt;
    e->year = year;
    e->active = 1;
    fs.n_emergency++;
    print_str("[FS] Emergency "); print_int(fs.n_emergency - 1);
    print_str(" type="); print_int(emg_type);
    print_str(" lvl="); print_int(level);
    print_str(" dep="); print_int(deployed);
    print_str(" alc="); print_int(allocated);
    print_str(" ben="); print_int(beneficiaries_cnt); print_str("\n");
    return fs.n_emergency - 1;
}

int fs_market(int mkt_type, int commodity, int monitors, int regulations, int import_export, int year) {
    if (fs.n_market >= MAX_MARKET) return -1;
    market_t* m = &markets[fs.n_market];
    m->market_id = fs.n_market;
    m->market_type = mkt_type;
    m->commodity_category = commodity;
    m->price_monitors = monitors;
    m->regulations_enforced = regulations;
    m->import_export_volume = import_export;
    m->year = year;
    m->active = 1;
    fs.n_market++;
    print_str("[FS] Market "); print_int(fs.n_market - 1);
    print_str(" type="); print_int(mkt_type);
    print_str(" cmd="); print_int(commodity);
    print_str(" mon="); print_int(monitors);
    print_str(" reg="); print_int(regulations);
    print_str(" imp="); print_int(import_export); print_str("\n");
    return fs.n_market - 1;
}

void fs_grain_report(void) {
    print_str("[FS] Grain report:\n");
    print_str("  Grain categories: "); print_int(fs.n_grain); print_str("\n");
    print_str("  Total production: "); print_int(fs.total_production); print_str("\n");
    print_str("  Total procurement: "); print_int(fs.total_procurement); print_str("\n");
}

void fs_quality_report(void) {
    print_str("[FS] Quality report:\n");
    print_str("  Quality categories: "); print_int(fs.n_quality); print_str("\n");
    print_str("  Total samples tested: "); print_int(fs.total_samples); print_str("\n");
}

void fs_market_report(void) {
    print_str("[FS] Market report:\n");
    print_str("  Storage units: "); print_int(fs.n_storage); print_str("\n");
    print_str("  Total capacity: "); print_int(fs.total_capacity); print_str("\n");
    print_str("  Emergency responses: "); print_int(fs.n_emergency); print_str("\n");
    print_str("  Total reserves deployed: "); print_int(fs.total_reserves); print_str("\n");
    print_str("  Market categories: "); print_int(fs.n_market); print_str("\n");
}

void fs_print_state(void) {
    print_str("[FS] Gr="); print_int(fs.n_grain);
    print_str(" Qu="); print_int(fs.n_quality);
    print_str(" St="); print_int(fs.n_storage);
    print_str(" Em="); print_int(fs.n_emergency);
    print_str(" Mk="); print_int(fs.n_market);
    print_str("\n");
}

int main(void) {
    print_str("=== Food Security Demo ===\n\n");
    fs_init();

    print_str("Grain management...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int rgn = (i % 8) + 1;
        int prd = 10000 + (i * 2000);
        int prc = 8000 + (i * 1500);
        int rsv = 5000 + (i * 1000);
        int year = 2020 + (i % 5);
        fs_grain(type, rgn, prd, prc, rsv, year);
    }

    print_str("\nQuality supervision...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 6) + 1;
        int spl = 500 + (i * 100);
        int rate = 90 + (i % 10);
        int trc = 1000 + (i * 200);
        int year = 2021 + (i % 4);
        fs_quality(type, cat, spl, rate, trc, year);
    }

    print_str("\nStorage logistics...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int fac = (i % 3) + 1;
        int whs = 30 + (i * 8);
        int cap = 2000 + (i * 500);
        int dlv = 500 + (i * 120);
        int year = 2022 + (i % 3);
        fs_storage(type, fac, whs, cap, dlv, year);
    }

    print_str("\nEmergency response...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int lvl = (i % 4) + 1;
        int dep = 1000 + (i * 250);
        int alc = 500 + (i * 120);
        int ben = 10000 + (i * 2500);
        int year = 2023 + (i % 2);
        fs_emergency(type, lvl, dep, alc, ben, year);
    }

    print_str("\nMarket regulation...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cmd = (i % 5) + 1;
        int mon = 100 + (i * 25);
        int reg = 50 + (i * 12);
        int imp = 3000 + (i * 700);
        int year = 2024;
        fs_market(type, cmd, mon, reg, imp, year);
    }

    print_str("\nGrain report...\n");
    fs_grain_report();

    print_str("\nQuality report...\n");
    fs_quality_report();

    print_str("\nMarket report...\n");
    fs_market_report();

    print_str("\nFinal state...\n");
    fs_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
