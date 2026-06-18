/* emergency_supply: Emergency supply administration system (v1.0)
 * Supply reserve, supply distribution, supply guarantee, supply monitoring, supply recovery
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

#define MAX_RESERVE      16
#define MAX_DISTRIBUTION 14
#define MAX_GUARANTEE    12
#define MAX_MONITORING   10
#define MAX_RECOVERY     10

typedef struct {
    int    reserve_id;
    int    reserve_type;
    int    supply_category;
    int    supplies_procured;
    int    inventory_stock;
    int    supplies_allocated;
    int    year;
    int    active;
} reserve_t;

typedef struct {
    int    distribution_id;
    int    distribution_type;
    int    target_area;
    int    supplies_dispatched;
    int    transport_operations;
    int    distribution_points;
    int    year;
    int    active;
} distribution_t;

typedef struct {
    int    guarantee_id;
    int    guarantee_type;
    int    supply_class;
    int    life_supplies;
    int    medical_supplies;
    int    rescue_supplies;
    int    year;
    int    active;
} guarantee_t;

typedef struct {
    int    monitoring_id;
    int    monitoring_type;
    int    monitoring_area;
    int    demand_forecasts;
    int    consumption_tracking;
    int    supply_alerts;
    int    year;
    int    active;
} monitoring_t;

typedef struct {
    int    recovery_id;
    int    recovery_type;
    int    supply_condition;
    int    supplies_recovered;
    int    supplies_scrapped;
    int    supplies_renewed;
    int    year;
    int    active;
} recovery_t;

typedef struct {
    int    n_reserve;
    int    n_distribution;
    int    n_guarantee;
    int    n_monitoring;
    int    n_recovery;
    int    total_procured;
    int    total_dispatched;
    int    total_life;
    int    total_forecasts;
    int    total_recovered;
} es_state_t;

static reserve_t reserves[MAX_RESERVE];
static distribution_t distributions[MAX_DISTRIBUTION];
static guarantee_t guarantees[MAX_GUARANTEE];
static monitoring_t monitorings[MAX_MONITORING];
static recovery_t recoveries[MAX_RECOVERY];
static es_state_t es;

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

int es_init(void) {
    if (initialized) return -1;
    es.n_reserve = 0; es.n_distribution = 0; es.n_guarantee = 0;
    es.n_monitoring = 0; es.n_recovery = 0;
    es.total_procured = 0; es.total_dispatched = 0;
    es.total_life = 0; es.total_forecasts = 0;
    es.total_recovered = 0;
    for (int i = 0; i < MAX_RESERVE; i++) reserves[i].active = 0;
    for (int i = 0; i < MAX_DISTRIBUTION; i++) distributions[i].active = 0;
    for (int i = 0; i < MAX_GUARANTEE; i++) guarantees[i].active = 0;
    for (int i = 0; i < MAX_MONITORING; i++) monitorings[i].active = 0;
    for (int i = 0; i < MAX_RECOVERY; i++) recoveries[i].active = 0;
    initialized = 1;
    print_str("[ES] Emergency supply initialized\n");
    return 0;
}

int es_reserve(int res_type, int cat, int procured, int stock, int allocated, int year) {
    if (es.n_reserve >= MAX_RESERVE) return -1;
    reserve_t* r = &reserves[es.n_reserve];
    r->reserve_id = es.n_reserve;
    r->reserve_type = res_type;
    r->supply_category = cat;
    r->supplies_procured = procured;
    r->inventory_stock = stock;
    r->supplies_allocated = allocated;
    r->year = year;
    r->active = 1;
    es.total_procured += procured;
    es.n_reserve++;
    print_str("[ES] Reserve "); print_int(es.n_reserve - 1);
    print_str(" type="); print_int(res_type);
    print_str(" cat="); print_int(cat);
    print_str(" prc="); print_int(procured);
    print_str(" stk="); print_int(stock);
    print_str(" alc="); print_int(allocated); print_str("\n");
    return es.n_reserve - 1;
}

int es_distribution(int dist_type, int area, int dispatched, int transport, int points, int year) {
    if (es.n_distribution >= MAX_DISTRIBUTION) return -1;
    distribution_t* d = &distributions[es.n_distribution];
    d->distribution_id = es.n_distribution;
    d->distribution_type = dist_type;
    d->target_area = area;
    d->supplies_dispatched = dispatched;
    d->transport_operations = transport;
    d->distribution_points = points;
    d->year = year;
    d->active = 1;
    es.total_dispatched += dispatched;
    es.n_distribution++;
    print_str("[ES] Distribution "); print_int(es.n_distribution - 1);
    print_str(" type="); print_int(dist_type);
    print_str(" area="); print_int(area);
    print_str(" dsp="); print_int(dispatched);
    print_str(" trn="); print_int(transport);
    print_str(" pts="); print_int(points); print_str("\n");
    return es.n_distribution - 1;
}

int es_guarantee(int gur_type, int cls, int life, int medical, int rescue, int year) {
    if (es.n_guarantee >= MAX_GUARANTEE) return -1;
    guarantee_t* g = &guarantees[es.n_guarantee];
    g->guarantee_id = es.n_guarantee;
    g->guarantee_type = gur_type;
    g->supply_class = cls;
    g->life_supplies = life;
    g->medical_supplies = medical;
    g->rescue_supplies = rescue;
    g->year = year;
    g->active = 1;
    es.total_life += life;
    es.n_guarantee++;
    print_str("[ES] Guarantee "); print_int(es.n_guarantee - 1);
    print_str(" type="); print_int(gur_type);
    print_str(" cls="); print_int(cls);
    print_str(" lif="); print_int(life);
    print_str(" med="); print_int(medical);
    print_str(" rsc="); print_int(rescue); print_str("\n");
    return es.n_guarantee - 1;
}

int es_monitoring(int mon_type, int area, int forecasts, int tracking, int alerts, int year) {
    if (es.n_monitoring >= MAX_MONITORING) return -1;
    monitoring_t* m = &monitorings[es.n_monitoring];
    m->monitoring_id = es.n_monitoring;
    m->monitoring_type = mon_type;
    m->monitoring_area = area;
    m->demand_forecasts = forecasts;
    m->consumption_tracking = tracking;
    m->supply_alerts = alerts;
    m->year = year;
    m->active = 1;
    es.total_forecasts += forecasts;
    es.n_monitoring++;
    print_str("[ES] Monitoring "); print_int(es.n_monitoring - 1);
    print_str(" type="); print_int(mon_type);
    print_str(" area="); print_int(area);
    print_str(" frc="); print_int(forecasts);
    print_str(" trk="); print_int(tracking);
    print_str(" alr="); print_int(alerts); print_str("\n");
    return es.n_monitoring - 1;
}

int es_recovery(int rec_type, int condition, int recovered, int scrapped, int renewed, int year) {
    if (es.n_recovery >= MAX_RECOVERY) return -1;
    recovery_t* r = &recoveries[es.n_recovery];
    r->recovery_id = es.n_recovery;
    r->recovery_type = rec_type;
    r->supply_condition = condition;
    r->supplies_recovered = recovered;
    r->supplies_scrapped = scrapped;
    r->supplies_renewed = renewed;
    r->year = year;
    r->active = 1;
    es.total_recovered += recovered;
    es.n_recovery++;
    print_str("[ES] Recovery "); print_int(es.n_recovery - 1);
    print_str(" type="); print_int(rec_type);
    print_str(" cnd="); print_int(condition);
    print_str(" rcv="); print_int(recovered);
    print_str(" scr="); print_int(scrapped);
    print_str(" rnw="); print_int(renewed); print_str("\n");
    return es.n_recovery - 1;
}

void es_reserve_report(void) {
    print_str("[ES] Reserve report:\n");
    print_str("  Reserve categories: "); print_int(es.n_reserve); print_str("\n");
    print_str("  Total supplies procured: "); print_int(es.total_procured); print_str("\n");
}

void es_distribution_report(void) {
    print_str("[ES] Distribution report:\n");
    print_str("  Distribution categories: "); print_int(es.n_distribution); print_str("\n");
    print_str("  Total supplies dispatched: "); print_int(es.total_dispatched); print_str("\n");
}

void es_recovery_report(void) {
    print_str("[ES] Recovery report:\n");
    print_str("  Guarantee categories: "); print_int(es.n_guarantee); print_str("\n");
    print_str("  Total life supplies: "); print_int(es.total_life); print_str("\n");
    print_str("  Monitoring categories: "); print_int(es.n_monitoring); print_str("\n");
    print_str("  Total demand forecasts: "); print_int(es.total_forecasts); print_str("\n");
    print_str("  Recovery categories: "); print_int(es.n_recovery); print_str("\n");
    print_str("  Total supplies recovered: "); print_int(es.total_recovered); print_str("\n");
}

void es_print_state(void) {
    print_str("[ES] Rs="); print_int(es.n_reserve);
    print_str(" Dt="); print_int(es.n_distribution);
    print_str(" Gr="); print_int(es.n_guarantee);
    print_str(" Mn="); print_int(es.n_monitoring);
    print_str(" Rc="); print_int(es.n_recovery);
    print_str("\n");
}

int main(void) {
    print_str("=== Emergency Supply Demo ===\n\n");
    es_init();

    print_str("Supply reserve...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int prc = 500 + (i * 120);
        int stk = 400 + (i * 100);
        int alc = 200 + (i * 50);
        int year = 2020 + (i % 5);
        es_reserve(type, cat, prc, stk, alc, year);
    }

    print_str("\nSupply distribution...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int area = (i % 5) + 1;
        int dsp = 300 + (i * 75);
        int trn = 50 + (i * 12);
        int pts = 20 + (i * 5);
        int year = 2021 + (i % 4);
        es_distribution(type, area, dsp, trn, pts, year);
    }

    print_str("\nSupply guarantee...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cls = (i % 5) + 1;
        int lif = 200 + (i * 50);
        int med = 100 + (i * 25);
        int rsc = 80 + (i * 20);
        int year = 2022 + (i % 3);
        es_guarantee(type, cls, lif, med, rsc, year);
    }

    print_str("\nSupply monitoring...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int area = (i % 5) + 1;
        int frc = 60 + (i * 15);
        int trk = 80 + (i * 20);
        int alr = 10 + (i * 3);
        int year = 2023 + (i % 2);
        es_monitoring(type, area, frc, trk, alr, year);
    }

    print_str("\nSupply recovery...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cnd = (i % 5) + 1;
        int rcv = 100 + (i * 25);
        int scr = 20 + (i * 5);
        int rnw = 30 + (i * 8);
        int year = 2024;
        es_recovery(type, cnd, rcv, scr, rnw, year);
    }

    print_str("\nReserve report...\n");
    es_reserve_report();

    print_str("\nDistribution report...\n");
    es_distribution_report();

    print_str("\nRecovery report...\n");
    es_recovery_report();

    print_str("\nFinal state...\n");
    es_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
