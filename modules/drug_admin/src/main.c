/* drug_admin: Drug administration system (v1.0)
 * Drug registration, production, distribution, medical devices, cosmetics
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

#define MAX_REGISTER     16
#define MAX_PRODUCE      14
#define MAX_DISTRIBUTE   12
#define MAX_DEVICE       10
#define MAX_COSMETIC     10

typedef struct {
    int    register_id;
    int    drug_id;
    int    drug_type;
    int    applicant_id;
    int    approval_status;
    int    clinical_trials;
    int    year;
    int    active;
} register_t;

typedef struct {
    int    produce_id;
    int    manufacturer_id;
    int    drug_id;
    int    gmp_certified;
    int    batch_count;
    int    inspection_pass;
    int    year;
    int    active;
} produce_t;

typedef struct {
    int    distribute_id;
    int    distributor_id;
    int    drug_id;
    int    gsp_certified;
    int    distribution_volume;
    int    traceability;
    int    year;
    int    active;
} distribute_t;

typedef struct {
    int    device_id;
    int    device_type;
    int    manufacturer_id;
    int    registration;
    int    production_license;
    int    usage_monitoring;
    int    year;
    int    active;
} device_t;

typedef struct {
    int    cosmetic_id;
    int    product_id;
    int    manufacturer_id;
    int    product_license;
    int    safety_score;
    int    market_status;
    int    year;
    int    active;
} cosmetic_t;

typedef struct {
    int    n_register;
    int    n_produce;
    int    n_distribute;
    int    n_device;
    int    n_cosmetic;
    int    total_approved;
    int    total_batches;
    int    total_volume;
    int    total_devices;
    int    total_cosmetics;
} da_state_t;

static register_t registers[MAX_REGISTER];
static produce_t produces[MAX_PRODUCE];
static distribute_t distributes[MAX_DISTRIBUTE];
static device_t devices[MAX_DEVICE];
static cosmetic_t cosmetics[MAX_COSMETIC];
static da_state_t da;

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

int da_init(void) {
    if (initialized) return -1;
    da.n_register = 0; da.n_produce = 0; da.n_distribute = 0;
    da.n_device = 0; da.n_cosmetic = 0;
    da.total_approved = 0; da.total_batches = 0;
    da.total_volume = 0; da.total_devices = 0;
    da.total_cosmetics = 0;
    for (int i = 0; i < MAX_REGISTER; i++) registers[i].active = 0;
    for (int i = 0; i < MAX_PRODUCE; i++) produces[i].active = 0;
    for (int i = 0; i < MAX_DISTRIBUTE; i++) distributes[i].active = 0;
    for (int i = 0; i < MAX_DEVICE; i++) devices[i].active = 0;
    for (int i = 0; i < MAX_COSMETIC; i++) cosmetics[i].active = 0;
    initialized = 1;
    print_str("[DA] Drug admin initialized\n");
    return 0;
}

int da_register(int drug, int drug_type, int applicant, int status, int trials, int year) {
    if (da.n_register >= MAX_REGISTER) return -1;
    register_t* r = &registers[da.n_register];
    r->register_id = da.n_register;
    r->drug_id = drug;
    r->drug_type = drug_type;
    r->applicant_id = applicant;
    r->approval_status = status;
    r->clinical_trials = trials;
    r->year = year;
    r->active = 1;
    if (status == 2) da.total_approved++;
    da.n_register++;
    print_str("[DA] Register "); print_int(da.n_register - 1);
    print_str(" drg="); print_int(drug);
    print_str(" type="); print_int(drug_type);
    print_str(" app="); print_int(applicant);
    print_str(" st="); print_int(status);
    print_str(" trl="); print_int(trials); print_str("\n");
    return da.n_register - 1;
}

int da_produce(int manufacturer, int drug, int gmp, int batches, int pass, int year) {
    if (da.n_produce >= MAX_PRODUCE) return -1;
    produce_t* p = &produces[da.n_produce];
    p->produce_id = da.n_produce;
    p->manufacturer_id = manufacturer;
    p->drug_id = drug;
    p->gmp_certified = gmp;
    p->batch_count = batches;
    p->inspection_pass = pass;
    p->year = year;
    p->active = 1;
    da.total_batches += batches;
    da.n_produce++;
    print_str("[DA] Produce "); print_int(da.n_produce - 1);
    print_str(" mfr="); print_int(manufacturer);
    print_str(" drg="); print_int(drug);
    print_str(" gmp="); print_int(gmp);
    print_str(" bat="); print_int(batches);
    print_str(" pass="); print_int(pass); print_str("\n");
    return da.n_produce - 1;
}

int da_distribute(int distributor, int drug, int gsp, int volume, int traceability, int year) {
    if (da.n_distribute >= MAX_DISTRIBUTE) return -1;
    distribute_t* d = &distributes[da.n_distribute];
    d->distribute_id = da.n_distribute;
    d->distributor_id = distributor;
    d->drug_id = drug;
    d->gsp_certified = gsp;
    d->distribution_volume = volume;
    d->traceability = traceability;
    d->year = year;
    d->active = 1;
    da.total_volume += volume;
    da.n_distribute++;
    print_str("[DA] Distribute "); print_int(da.n_distribute - 1);
    print_str(" dst="); print_int(distributor);
    print_str(" drg="); print_int(drug);
    print_str(" gsp="); print_int(gsp);
    print_str(" vol="); print_int(volume);
    print_str(" trc="); print_int(traceability); print_str("\n");
    return da.n_distribute - 1;
}

int da_device(int device_type, int manufacturer, int registration, int license, int monitoring, int year) {
    if (da.n_device >= MAX_DEVICE) return -1;
    device_t* dv = &devices[da.n_device];
    dv->device_id = da.n_device;
    dv->device_type = device_type;
    dv->manufacturer_id = manufacturer;
    dv->registration = registration;
    dv->production_license = license;
    dv->usage_monitoring = monitoring;
    dv->year = year;
    dv->active = 1;
    da.total_devices++;
    da.n_device++;
    print_str("[DA] Device "); print_int(da.n_device - 1);
    print_str(" type="); print_int(device_type);
    print_str(" mfr="); print_int(manufacturer);
    print_str(" reg="); print_int(registration);
    print_str(" lic="); print_int(license);
    print_str(" mon="); print_int(monitoring); print_str("\n");
    return da.n_device - 1;
}

int da_cosmetic(int product, int manufacturer, int license, int safety, int market_status, int year) {
    if (da.n_cosmetic >= MAX_COSMETIC) return -1;
    cosmetic_t* c = &cosmetics[da.n_cosmetic];
    c->cosmetic_id = da.n_cosmetic;
    c->product_id = product;
    c->manufacturer_id = manufacturer;
    c->product_license = license;
    c->safety_score = safety;
    c->market_status = market_status;
    c->year = year;
    c->active = 1;
    da.total_cosmetics++;
    da.n_cosmetic++;
    print_str("[DA] Cosmetic "); print_int(da.n_cosmetic - 1);
    print_str(" prd="); print_int(product);
    print_str(" mfr="); print_int(manufacturer);
    print_str(" lic="); print_int(license);
    print_str(" saf="); print_int(safety);
    print_str(" mkt="); print_int(market_status); print_str("\n");
    return da.n_cosmetic - 1;
}

void da_register_report(void) {
    print_str("[DA] Registration report:\n");
    print_str("  Drug registrations: "); print_int(da.n_register); print_str("\n");
    print_str("  Total approved: "); print_int(da.total_approved); print_str("\n");
}

void da_produce_report(void) {
    print_str("[DA] Production report:\n");
    print_str("  Manufacturers: "); print_int(da.n_produce); print_str("\n");
    print_str("  Total batches: "); print_int(da.total_batches); print_str("\n");
}

void da_distribute_report(void) {
    print_str("[DA] Distribution report:\n");
    print_str("  Distributors: "); print_int(da.n_distribute); print_str("\n");
    print_str("  Total volume: "); print_int(da.total_volume); print_str("\n");
    print_str("  Medical devices: "); print_int(da.n_device); print_str("\n");
    print_str("  Cosmetics: "); print_int(da.n_cosmetic); print_str("\n");
}

void da_print_state(void) {
    print_str("[DA] Rg="); print_int(da.n_register);
    print_str(" Pr="); print_int(da.n_produce);
    print_str(" Ds="); print_int(da.n_distribute);
    print_str(" Dv="); print_int(da.n_device);
    print_str(" Cs="); print_int(da.n_cosmetic);
    print_str("\n");
}

int main(void) {
    print_str("=== Drug Admin Demo ===\n\n");
    da_init();

    print_str("Drug registration...\n");
    for (int i = 0; i < 16; i++) {
        int drg = 1000 + (i * 13);
        int type = (i % 5) + 1;
        int app = 2000 + (i * 7);
        int st = (i % 3) + 1;
        int trl = (i % 4) + 1;
        int year = 2020 + (i % 5);
        da_register(drg, type, app, st, trl, year);
    }

    print_str("\nDrug production...\n");
    for (int i = 0; i < 14; i++) {
        int mfr = 3000 + (i * 11);
        int drg = 4000 + (i * 7);
        int gmp = (i % 3 == 0) ? 0 : 1;
        int bat = 50 + (i * 20);
        int pass = bat - (i % 5);
        int year = 2021 + (i % 4);
        da_produce(mfr, drg, gmp, bat, pass, year);
    }

    print_str("\nDrug distribution...\n");
    for (int i = 0; i < 12; i++) {
        int dst = 5000 + (i * 17);
        int drg = 6000 + (i * 13);
        int gsp = (i % 3 == 0) ? 0 : 1;
        int vol = 1000 + (i * 500);
        int trc = (i % 4 == 0) ? 0 : 1;
        int year = 2022 + (i % 3);
        da_distribute(dst, drg, gsp, vol, trc, year);
    }

    print_str("\nMedical devices...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int mfr = 7000 + (i * 7);
        int reg = (i % 3 == 0) ? 0 : 1;
        int lic = (i % 4 == 0) ? 0 : 1;
        int mon = (i % 3 == 0) ? 1 : 0;
        int year = 2023 + (i % 2);
        da_device(type, mfr, reg, lic, mon, year);
    }

    print_str("\nCosmetics...\n");
    for (int i = 0; i < 10; i++) {
        int prd = 8000 + (i * 11);
        int mfr = 9000 + (i * 13);
        int lic = (i % 3 == 0) ? 0 : 1;
        int saf = 70 + (i * 3);
        int mkt = (i % 4) + 1;
        int year = 2024;
        da_cosmetic(prd, mfr, lic, saf, mkt, year);
    }

    print_str("\nRegistration report...\n");
    da_register_report();

    print_str("\nProduction report...\n");
    da_produce_report();

    print_str("\nDistribution report...\n");
    da_distribute_report();

    print_str("\nFinal state...\n");
    da_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
