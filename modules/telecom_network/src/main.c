/* telecom_network: Telecom network management (v1.0)
 * Cell towers, core network, subscribers, billing, spectrum
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

#define MAX_TOWERS    12
#define MAX_CORE      8
#define MAX_SUBS      20
#define MAX_PLANS     10
#define MAX_SPECTRUM  8

typedef struct {
    int    tower_id;
    int    type;
    int    coverage_km;
    int    capacity_users;
    int    active_users;
    int    freq_band;
    double signal_quality;
    int    throughput_mbps;
    double revenue;
    int    active;
} tower_t;

typedef struct {
    int    core_id;
    int    type;
    int    capacity_mbps;
    int    load_mbps;
    int    n_subscribers;
    int    n_sessions;
    int    latency_ms;
    int    active;
} core_t;

typedef struct {
    int    sub_id;
    int    plan_id;
    int    tower_id;
    int    data_used_mb;
    int    call_min;
    int    sms_count;
    double monthly_bill;
    double total_paid;
    int    tenure_months;
    int    active;
} subscriber_t;

typedef struct {
    int    plan_id;
    int    type;
    int    data_gb;
    int    call_min;
    int    sms_limit;
    double price;
    int    n_subscribers;
    double revenue;
    int    active;
} plan_t;

typedef struct {
    int    spec_id;
    int    band;
    int    width_mhz;
    int    power_dbm;
    int    interference;
    int    utilization;
    double cost;
    int    active;
} spectrum_t;

typedef struct {
    int    n_towers;
    int    n_core;
    int    n_subscribers;
    int    n_plans;
    int    n_spectrum;
    double subscription_revenue;
    double call_revenue;
    double data_revenue;
    double roaming_revenue;
    double total_revenue;
    double infrastructure_costs;
    double spectrum_costs;
    double ops_costs;
    int    total_data_gb;
    int    total_call_min;
} tn_state_t;

static tower_t towers[MAX_TOWERS];
static core_t cores[MAX_CORE];
static subscriber_t subscribers[MAX_SUBS];
static plan_t plans[MAX_PLANS];
static spectrum_t spectrum[MAX_SPECTRUM];
static tn_state_t tn;

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

int tn_init(void) {
    if (initialized) return -1;
    tn.n_towers = 0; tn.n_core = 0; tn.n_subscribers = 0;
    tn.n_plans = 0; tn.n_spectrum = 0;
    tn.subscription_revenue = 0.0; tn.call_revenue = 0.0;
    tn.data_revenue = 0.0; tn.roaming_revenue = 0.0;
    tn.total_revenue = 0.0; tn.infrastructure_costs = 0.0;
    tn.spectrum_costs = 0.0; tn.ops_costs = 0.0;
    tn.total_data_gb = 0; tn.total_call_min = 0;
    for (int i = 0; i < MAX_TOWERS; i++) towers[i].active = 0;
    for (int i = 0; i < MAX_CORE; i++) cores[i].active = 0;
    for (int i = 0; i < MAX_SUBS; i++) subscribers[i].active = 0;
    for (int i = 0; i < MAX_PLANS; i++) plans[i].active = 0;
    for (int i = 0; i < MAX_SPECTRUM; i++) spectrum[i].active = 0;
    initialized = 1;
    print_str("[TN] Telecom network initialized\n");
    return 0;
}

int tn_add_tower(int type, int coverage, int capacity, int freq_band) {
    if (tn.n_towers >= MAX_TOWERS) return -1;
    tower_t* t = &towers[tn.n_towers];
    t->tower_id = tn.n_towers;
    t->type = type;
    t->coverage_km = coverage;
    t->capacity_users = capacity;
    t->active_users = 0;
    t->freq_band = freq_band;
    t->signal_quality = 85.0 + (type * 3);
    t->throughput_mbps = 100 + (type * 50);
    t->revenue = 0.0;
    t->active = 1;
    tn.infrastructure_costs += 5000.0 + type * 2000.0;
    tn.n_towers++;
    print_str("[TN] Tower "); print_int(tn.n_towers - 1);
    print_str(" type="); print_int(type);
    print_str(" cov="); print_int(coverage);
    print_str("km cap="); print_int(capacity);
    print_str(" band="); print_int(freq_band); print_str("\n");
    return tn.n_towers - 1;
}

int tn_add_core(int type, int capacity, int latency) {
    if (tn.n_core >= MAX_CORE) return -1;
    core_t* c = &cores[tn.n_core];
    c->core_id = tn.n_core;
    c->type = type;
    c->capacity_mbps = capacity;
    c->load_mbps = 0;
    c->n_subscribers = 0;
    c->n_sessions = 0;
    c->latency_ms = latency;
    c->active = 1;
    tn.infrastructure_costs += 10000.0 + type * 3000.0;
    tn.n_core++;
    print_str("[TN] Core "); print_int(tn.n_core - 1);
    print_str(" type="); print_int(type);
    print_str(" cap="); print_int(capacity);
    print_str("Mbps lat="); print_int(latency); print_str("ms\n");
    return tn.n_core - 1;
}

int tn_add_plan(int type, int data_gb, int call_min, int sms, double price) {
    if (tn.n_plans >= MAX_PLANS) return -1;
    plan_t* p = &plans[tn.n_plans];
    p->plan_id = tn.n_plans;
    p->type = type;
    p->data_gb = data_gb;
    p->call_min = call_min;
    p->sms_limit = sms;
    p->price = price;
    p->n_subscribers = 0;
    p->revenue = 0.0;
    p->active = 1;
    tn.n_plans++;
    print_str("[TN] Plan "); print_int(tn.n_plans - 1);
    print_str(" type="); print_int(type);
    print_str(" GB="); print_int(data_gb);
    print_str(" min="); print_int(call_min);
    print_str(" $"); print_int((int)price); print_str("/mo\n");
    return tn.n_plans - 1;
}

int tn_add_spectrum(int band, int width_mhz, int power_dbm, double cost) {
    if (tn.n_spectrum >= MAX_SPECTRUM) return -1;
    spectrum_t* sp = &spectrum[tn.n_spectrum];
    sp->spec_id = tn.n_spectrum;
    sp->band = band;
    sp->width_mhz = width_mhz;
    sp->power_dbm = power_dbm;
    sp->interference = 0;
    sp->utilization = 0;
    sp->cost = cost;
    sp->active = 1;
    tn.spectrum_costs += cost;
    tn.n_spectrum++;
    print_str("[TN] Spectrum "); print_int(tn.n_spectrum - 1);
    print_str(" band="); print_int(band);
    print_str(" MHz="); print_int(width_mhz);
    print_str(" dBm="); print_int(power_dbm);
    print_str(" $"); print_int((int)cost); print_str("\n");
    return tn.n_spectrum - 1;
}

int tn_register_subscriber(int plan_id, int tower_id, int tenure) {
    if (tn.n_subscribers >= MAX_SUBS) return -1;
    if (plan_id >= tn.n_plans || tower_id >= tn.n_towers) return -2;
    subscriber_t* s = &subscribers[tn.n_subscribers];
    s->sub_id = tn.n_subscribers;
    s->plan_id = plan_id;
    s->tower_id = tower_id;
    s->data_used_mb = 0;
    s->call_min = 0;
    s->sms_count = 0;
    s->monthly_bill = 0.0;
    s->total_paid = 0.0;
    s->tenure_months = tenure;
    s->active = 1;
    towers[tower_id].active_users++;
    plans[plan_id].n_subscribers++;
    cores[0].n_subscribers++;
    tn.n_subscribers++;
    print_str("[TN] Sub "); print_int(tn.n_subscribers - 1);
    print_str(" P"); print_int(plan_id);
    print_str(" T"); print_int(tower_id);
    print_str(" tenure="); print_int(tenure); print_str("mo\n");
    return tn.n_subscribers - 1;
}

int tn_monthly_billing(int sub_id) {
    if (sub_id >= tn.n_subscribers) return -1;
    subscriber_t* s = &subscribers[sub_id];
    plan_t* p = &plans[s->plan_id];
    double bill = p->price;
    int over_data = s->data_used_mb / 1024 - p->data_gb;
    if (over_data > 0) bill += over_data * 10.0;
    int over_calls = s->call_min - p->call_min;
    if (over_calls > 0) bill += over_calls * 0.1;
    s->monthly_bill = bill;
    s->total_paid += bill;
    s->tenure_months++;
    p->revenue += bill;
    tn.subscription_revenue += bill;
    tn.total_revenue += bill;
    print_str("[TN] Bill S"); print_int(sub_id);
    print_str(" $"); print_int((int)bill); print_str("\n");
    return 0;
}

int tn_data_usage(int sub_id, int mb) {
    if (sub_id >= tn.n_subscribers) return -1;
    subscriber_t* s = &subscribers[sub_id];
    s->data_used_mb += mb;
    tn.total_data_gb += mb / 1024;
    double rev = mb * 0.005;
    tn.data_revenue += rev;
    tn.total_revenue += rev;
    tower_t* t = &towers[s->tower_id];
    t->revenue += rev;
    print_str("[TN] Data S"); print_int(sub_id);
    print_str(" MB="); print_int(mb);
    print_str(" $"); print_int((int)rev); print_str("\n");
    return 0;
}

int tn_call_usage(int sub_id, int minutes) {
    if (sub_id >= tn.n_subscribers) return -1;
    subscriber_t* s = &subscribers[sub_id];
    s->call_min += minutes;
    tn.total_call_min += minutes;
    double rev = minutes * 0.05;
    tn.call_revenue += rev;
    tn.total_revenue += rev;
    print_str("[TN] Call S"); print_int(sub_id);
    print_str(" min="); print_int(minutes);
    print_str(" $"); print_int((int)rev); print_str("\n");
    return 0;
}

int tn_sms_usage(int sub_id, int count) {
    if (sub_id >= tn.n_subscribers) return -1;
    subscriber_t* s = &subscribers[sub_id];
    s->sms_count += count;
    double rev = count * 0.01;
    tn.total_revenue += rev;
    print_str("[TN] SMS S"); print_int(sub_id);
    print_str(" n="); print_int(count);
    print_str(" $"); print_int((int)rev); print_str("\n");
    return 0;
}

int tn_add_roaming(int sub_id, double charge) {
    if (sub_id >= tn.n_subscribers) return -1;
    subscribers[sub_id].total_paid += charge;
    tn.roaming_revenue += charge;
    tn.total_revenue += charge;
    print_str("[TN] Roam S"); print_int(sub_id);
    print_str(" $"); print_int((int)charge); print_str("\n");
    return 0;
}

int tn_set_spectrum_load(int spec_id, int utilization, int interference) {
    if (spec_id >= tn.n_spectrum) return -1;
    spectrum_t* sp = &spectrum[spec_id];
    sp->utilization = utilization;
    sp->interference = interference;
    tn.ops_costs += interference * 50.0;
    print_str("[TN] Spec "); print_int(spec_id);
    print_str(" util="); print_int(utilization);
    print_str("% interf="); print_int(interference); print_str("\n");
    return 0;
}

int tn_set_core_load(int core_id, int load_mbps, int sessions) {
    if (core_id >= tn.n_core) return -1;
    core_t* c = &cores[core_id];
    c->load_mbps = load_mbps;
    c->n_sessions = sessions;
    print_str("[TN] Core "); print_int(core_id);
    print_str(" load="); print_int(load_mbps);
    print_str("Mbps sess="); print_int(sessions); print_str("\n");
    return 0;
}

void tn_network_report(void) {
    print_str("[TN] Network report:\n");
    for (int i = 0; i < tn.n_towers; i++) {
        tower_t* t = &towers[i];
        print_str("  T"); print_int(i);
        print_str(" type="); print_int(t->type);
        print_str(" users="); print_int(t->active_users);
        print_str("/"); print_int(t->capacity_users);
        print_str(" mbps="); print_int(t->throughput_mbps);
        print_str(" rev="); print_int((int)t->revenue); print_str("\n");
    }
    for (int i = 0; i < tn.n_core; i++) {
        core_t* c = &cores[i];
        print_str("  C"); print_int(i);
        print_str(" type="); print_int(c->type);
        print_str(" load="); print_int(c->load_mbps);
        print_str("/"); print_int(c->capacity_mbps);
        print_str("Mbps subs="); print_int(c->n_subscribers); print_str("\n");
    }
}

void tn_financial_report(void) {
    print_str("[TN] Financial report:\n");
    print_str("  Subscription rev: "); print_int((int)tn.subscription_revenue); print_str("\n");
    print_str("  Call revenue: "); print_int((int)tn.call_revenue); print_str("\n");
    print_str("  Data revenue: "); print_int((int)tn.data_revenue); print_str("\n");
    print_str("  Roaming revenue: "); print_int((int)tn.roaming_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)tn.total_revenue); print_str("\n");
    print_str("  Infra costs: "); print_int((int)tn.infrastructure_costs); print_str("\n");
    print_str("  Spectrum costs: "); print_int((int)tn.spectrum_costs); print_str("\n");
    print_str("  Ops costs: "); print_int((int)tn.ops_costs); print_str("\n");
    double total_costs = tn.infrastructure_costs + tn.spectrum_costs + tn.ops_costs;
    double profit = tn.total_revenue - total_costs;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    print_str("  Subscribers: "); print_int(tn.n_subscribers);
    print_str(" Data: "); print_int(tn.total_data_gb);
    print_str("GB Calls: "); print_int(tn.total_call_min); print_str("min\n");
}

void tn_print_state(void) {
    print_str("[TN] Towers="); print_int(tn.n_towers);
    print_str(" Core="); print_int(tn.n_core);
    print_str(" Subs="); print_int(tn.n_subscribers);
    print_str(" Plans="); print_int(tn.n_plans);
    print_str(" Spectrum="); print_int(tn.n_spectrum);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)tn.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Telecom Network Demo ===\n\n");
    tn_init();

    print_str("Adding towers...\n");
    tn_add_tower(1, 5, 500, 700);
    tn_add_tower(2, 3, 800, 1800);
    tn_add_tower(1, 8, 400, 800);
    tn_add_tower(3, 2, 1200, 2600);
    tn_add_tower(2, 4, 600, 1900);
    tn_add_tower(1, 6, 450, 700);
    tn_add_tower(3, 1, 1500, 3500);
    tn_add_tower(2, 3, 700, 2100);
    tn_add_tower(1, 7, 350, 800);
    tn_add_tower(3, 2, 1000, 2800);
    tn_add_tower(2, 4, 650, 1800);
    tn_add_tower(1, 5, 500, 900);

    print_str("\nAdding core network...\n");
    tn_add_core(1, 10000, 5);
    tn_add_core(2, 20000, 3);
    tn_add_core(3, 5000, 8);
    tn_add_core(1, 15000, 4);
    tn_add_core(2, 25000, 2);
    tn_add_core(3, 8000, 6);
    tn_add_core(1, 12000, 5);
    tn_add_core(2, 18000, 3);

    print_str("\nAdding plans...\n");
    tn_add_plan(1, 5, 200, 500, 29.0);
    tn_add_plan(1, 10, 500, 1000, 49.0);
    tn_add_plan(2, 20, 1000, 2000, 79.0);
    tn_add_plan(2, 50, 2000, 5000, 99.0);
    tn_add_plan(3, 100, 5000, 10000, 149.0);
    tn_add_plan(1, 3, 100, 200, 19.0);
    tn_add_plan(2, 30, 1500, 3000, 89.0);
    tn_add_plan(3, 200, 10000, 20000, 199.0);
    tn_add_plan(1, 8, 300, 800, 39.0);
    tn_add_plan(2, 40, 1800, 4000, 95.0);

    print_str("\nAdding spectrum...\n");
    tn_add_spectrum(700, 20, 43, 50000.0);
    tn_add_spectrum(800, 15, 40, 40000.0);
    tn_add_spectrum(1800, 25, 38, 60000.0);
    tn_add_spectrum(1900, 20, 36, 55000.0);
    tn_add_spectrum(2100, 30, 35, 70000.0);
    tn_add_spectrum(2600, 40, 33, 80000.0);
    tn_add_spectrum(3500, 50, 30, 90000.0);
    tn_add_spectrum(28000, 100, 25, 120000.0);

    print_str("\nRegistering subscribers...\n");
    for (int i = 0; i < 20; i++) {
        tn_register_subscriber(i % 10, i % 12, 6 + (i % 18));
    }

    print_str("\nData usage...\n");
    for (int i = 0; i < 20; i++) {
        tn_data_usage(i, 500 + (i * 200));
    }

    print_str("\nCall usage...\n");
    for (int i = 0; i < 20; i++) {
        tn_call_usage(i, 100 + (i * 30));
    }

    print_str("\nSMS usage...\n");
    for (int i = 0; i < 20; i++) {
        tn_sms_usage(i, 50 + (i * 20));
    }

    print_str("\nMonthly billing...\n");
    for (int i = 0; i < 20; i++) {
        tn_monthly_billing(i);
    }

    print_str("\nRoaming charges...\n");
    for (int i = 0; i < 8; i++) {
        tn_add_roaming(i, 15.0 + i * 5.0);
    }

    print_str("\nSpectrum loading...\n");
    for (int i = 0; i < 8; i++) {
        tn_set_spectrum_load(i, 40 + (i * 7), 2 + (i % 4));
    }

    print_str("\nCore loading...\n");
    for (int i = 0; i < 8; i++) {
        tn_set_core_load(i, 2000 + (i * 1000), 100 + (i * 50));
    }

    print_str("\nNetwork report...\n");
    tn_network_report();

    print_str("\nFinancial report...\n");
    tn_financial_report();

    print_str("\nFinal state...\n");
    tn_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
