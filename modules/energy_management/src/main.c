/* energy_management: Energy management system (v1.0)
 * Consumption monitoring, optimization, distributed energy, carbon, efficiency
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

#define MAX_METERS     16
#define MAX_ZONES      8
#define MAX_PERIODS    24
#define MAX_GEN        8
#define MAX_STORAGE    4
#define MAX_CARBON     8

#define METER_ELEC     0x01
#define METER_GAS      0x02
#define METER_WATER    0x03
#define METER_HEAT     0x04
#define METER_STEAM    0x05

#define OPT_PEAK_SHAVE 0x01
#define OPT_DEMAND_RSP 0x02
#define OPT_ECO_DISP   0x03

#define GEN_PV         0x01
#define GEN_WIND       0x02
#define GEN_CHP        0x03
#define GEN_FUELCELL   0x04

typedef struct {
    int    type;
    double power;
    double energy_total;
    double energy_peak;
    double energy_offpeak;
    double cost;
    double baseline;
    int    zone_id;
    int    id;
    int    active;
} meter_t;

typedef struct {
    double load[MAX_PERIODS];
    double forecast[MAX_PERIODS];
    double peak;
    double avg;
    double lf;
    int    zone_id;
    int    active;
} zone_t;

typedef struct {
    int    type;
    double capacity;
    double output;
    double efficiency;
    double cost_kwh;
    int    id;
    int    active;
} generator_t;

typedef struct {
    double capacity;
    double soc;
    double charge_power;
    double discharge_power;
    double efficiency;
    double cycles;
    int    id;
    int    active;
} storage_t;

typedef struct {
    double scope1;
    double scope2;
    double scope3;
    double total;
    double offset;
    double net;
    double price;
    int    id;
    int    active;
} carbon_t;

typedef struct {
    int    n_meters;
    int    n_zones;
    int    n_gen;
    int    n_storage;
    int    n_carbon;
    double total_power;
    double total_energy;
    double total_cost;
    double peak_demand;
    double pue;
    double carbon_total;
    int    optimization_count;
} em_state_t;

static meter_t meters[MAX_METERS];
static zone_t zones[MAX_ZONES];
static generator_t generators[MAX_GEN];
static storage_t storages[MAX_STORAGE];
static carbon_t carbons[MAX_CARBON];
static em_state_t em;

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
static double dsqrt(double x) {
    if (x <= 0.0) return 0.0;
    double g = x, p = x / g;
    for (int i = 0; i < 30; i++) { g = (g + p) * 0.5; p = x / g; }
    return g;
}
static double dsin(double x) {
    double sum = x, term = x, x2 = x * x;
    for (int i = 1; i < 15; i++) { term *= -x2 / ((2*i)*(2*i+1)); sum += term; }
    return sum;
}

int em_init(void) {
    if (initialized) return -1;
    em.n_meters = 0; em.n_zones = 0; em.n_gen = 0;
    em.n_storage = 0; em.n_carbon = 0;
    em.total_power = 0.0; em.total_energy = 0.0; em.total_cost = 0.0;
    em.peak_demand = 0.0; em.pue = 1.5; em.carbon_total = 0.0;
    em.optimization_count = 0;
    for (int i = 0; i < MAX_METERS; i++) meters[i].active = 0;
    for (int i = 0; i < MAX_ZONES; i++) zones[i].active = 0;
    for (int i = 0; i < MAX_GEN; i++) generators[i].active = 0;
    for (int i = 0; i < MAX_STORAGE; i++) storages[i].active = 0;
    for (int i = 0; i < MAX_CARBON; i++) carbons[i].active = 0;
    initialized = 1;
    print_str("[EM] Energy management initialized\n");
    return 0;
}

int em_add_meter(int type, double baseline, int zone_id) {
    if (em.n_meters >= MAX_METERS) return -1;
    meter_t* m = &meters[em.n_meters];
    m->type = type; m->baseline = baseline;
    m->power = baseline * (0.7 + 0.3 * (double)(em.n_meters % 3) / 2.0);
    m->energy_total = m->power * 24.0;
    m->energy_peak = m->energy_total * 0.6;
    m->energy_offpeak = m->energy_total * 0.4;
    double peak_rate = 0.15, offpeak_rate = 0.08;
    m->cost = m->energy_peak * peak_rate + m->energy_offpeak * offpeak_rate;
    m->zone_id = zone_id;
    m->id = em.n_meters; m->active = 1;
    em.n_meters++;
    print_str("[EM] Meter "); print_int(em.n_meters - 1);
    print_str(" type="); print_int(type);
    print_str(" power="); print_int((int)m->power); print_str("kW");
    print_str(" cost="); print_int((int)(m->cost * 100)); print_str("c\n");
    return em.n_meters - 1;
}

int em_add_zone(double peak_load) {
    if (em.n_zones >= MAX_ZONES) return -1;
    zone_t* z = &zones[em.n_zones];
    z->peak = peak_load;
    double sum = 0.0;
    for (int i = 0; i < MAX_PERIODS; i++) {
        double factor = 0.6 + 0.4 * dsin(3.14159 * (double)i / 24.0);
        z->load[i] = peak_load * factor;
        z->forecast[i] = z->load[i] * 1.02;
        sum += z->load[i];
    }
    z->avg = sum / MAX_PERIODS;
    z->lf = z->avg / (z->peak + 1e-10);
    z->zone_id = em.n_zones; z->active = 1;
    em.n_zones++;
    print_str("[EM] Zone "); print_int(em.n_zones - 1);
    print_str(" peak="); print_int((int)peak_load); print_str("kW");
    print_str(" avg="); print_int((int)z->avg); print_str("kW");
    print_str(" LF="); print_int((int)(z->lf * 100)); print_str("%\n");
    return em.n_zones - 1;
}

int em_add_generator(int type, double capacity, double cost_kwh) {
    if (em.n_gen >= MAX_GEN) return -1;
    generator_t* g = &generators[em.n_gen];
    g->type = type; g->capacity = capacity;
    g->output = capacity * 0.8;
    g->efficiency = type == GEN_PV ? 0.18 : (type == GEN_WIND ? 0.35 : 0.45);
    g->cost_kwh = cost_kwh;
    g->id = em.n_gen; g->active = 1;
    em.n_gen++;
    print_str("[EM] Generator "); print_int(em.n_gen - 1);
    print_str(" type="); print_int(type);
    print_str(" cap="); print_int((int)capacity); print_str("kW");
    print_str(" eff="); print_int((int)(g->efficiency * 100)); print_str("%\n");
    return em.n_gen - 1;
}

int em_add_storage(double capacity, double soc) {
    if (em.n_storage >= MAX_STORAGE) return -1;
    storage_t* s = &storages[em.n_storage];
    s->capacity = capacity; s->soc = soc;
    s->charge_power = capacity * 0.2;
    s->discharge_power = capacity * 0.25;
    s->efficiency = 0.92; s->cycles = 0;
    s->id = em.n_storage; s->active = 1;
    em.n_storage++;
    print_str("[EM] Storage "); print_int(em.n_storage - 1);
    print_str(" cap="); print_int((int)capacity); print_str("kWh");
    print_str(" SOC="); print_int((int)soc); print_str("%\n");
    return em.n_storage - 1;
}

void em_consumption_analysis(void) {
    double total_power = 0.0, total_energy = 0.0, total_cost = 0.0;
    for (int i = 0; i < em.n_meters; i++) {
        total_power += meters[i].power;
        total_energy += meters[i].energy_total;
        total_cost += meters[i].cost;
    }
    em.total_power = total_power;
    em.total_energy = total_energy;
    em.total_cost = total_cost;
    em.peak_demand = total_power * 1.2;
    em.pue = 1.0 + 0.5 * total_power / (em.total_energy / 24.0 + 1e-10);
    if (em.pue < 1.1) em.pue = 1.1;
    print_str("[EM] Consumption analysis:\n");
    print_str("  Total power: "); print_int((int)total_power); print_str("kW\n");
    print_str("  Total energy: "); print_int((int)total_energy); print_str("kWh\n");
    print_str("  Total cost: "); print_int((int)(total_cost * 100)); print_str("c\n");
    print_str("  Peak demand: "); print_int((int)em.peak_demand); print_str("kW\n");
    print_str("  PUE: "); print_int((int)(em.pue * 100)); print_str("/100\n");
}

void em_optimization(int method) {
    double saved = 0.0;
    if (method == OPT_PEAK_SHAVE) {
        for (int i = 0; i < em.n_meters; i++) {
            if (meters[i].energy_peak > meters[i].energy_offpeak) {
                saved += (meters[i].energy_peak - meters[i].energy_offpeak) * 0.1;
            }
        }
        print_str("[EM] Peak shaving: saved="); print_int((int)saved); print_str("kWh\n");
    } else if (method == OPT_DEMAND_RSP) {
        for (int i = 0; i < em.n_zones; i++) {
            saved += zones[i].peak * 0.15;
        }
        print_str("[EM] Demand response: shed="); print_int((int)saved); print_str("kW\n");
    } else if (method == OPT_ECO_DISP) {
        double min_cost = 1e10; int best_gen = 0;
        for (int i = 0; i < em.n_gen; i++) {
            double c = generators[i].cost_kwh / (generators[i].efficiency + 1e-10);
            if (c < min_cost) { min_cost = c; best_gen = i; }
        }
        saved = em.total_cost * 0.1;
        print_str("[EM] Economic dispatch: best gen="); print_int(best_gen);
        print_str(" saved="); print_int((int)(saved * 100)); print_str("c\n");
    }
    em.total_cost -= saved;
    em.optimization_count++;
}

void em_distributed_dispatch(void) {
    double total_gen = 0.0;
    for (int i = 0; i < em.n_gen; i++) total_gen += generators[i].output;
    double total_store = 0.0;
    for (int i = 0; i < em.n_storage; i++) {
        storages[i].charge_power = total_gen > em.total_power ?
            (total_gen - em.total_power) * 0.5 : 0.0;
        storages[i].discharge_power = total_gen < em.total_power ?
            (em.total_power - total_gen) * 0.3 : 0.0;
        total_store += storages[i].charge_power + storages[i].discharge_power;
    }
    print_str("[EM] Distributed dispatch:\n");
    print_str("  Total gen: "); print_int((int)total_gen); print_str("kW\n");
    print_str("  Total load: "); print_int((int)em.total_power); print_str("kW\n");
    print_str("  Storage: "); print_int((int)total_store); print_str("kW\n");
    for (int i = 0; i < em.n_storage; i++) {
        print_str("  Storage "); print_int(i);
        print_str(" SOC="); print_int((int)storages[i].soc);
        print_str("% ch="); print_int((int)storages[i].charge_power);
        print_str("kW dis="); print_int((int)storages[i].discharge_power);
        print_str("kW\n");
    }
}

int em_add_carbon(double scope1, double scope2, double price) {
    if (em.n_carbon >= MAX_CARBON) return -1;
    carbon_t* c = &carbons[em.n_carbon];
    c->scope1 = scope1; c->scope2 = scope2;
    c->scope3 = scope1 * 0.3;
    c->total = c->scope1 + c->scope2 + c->scope3;
    c->offset = c->total * 0.2;
    c->net = c->total - c->offset;
    c->price = price;
    c->id = em.n_carbon; c->active = 1;
    em.n_carbon++;
    print_str("[EM] Carbon "); print_int(em.n_carbon - 1);
    print_str(" S1="); print_int((int)scope1);
    print_str(" S2="); print_int((int)scope2);
    print_str(" total="); print_int((int)c->total);
    print_str(" net="); print_int((int)c->net); print_str("tCO2\n");
    return em.n_carbon - 1;
}

void em_carbon_trading(void) {
    double total_net = 0.0, total_cost = 0.0;
    for (int i = 0; i < em.n_carbon; i++) {
        total_net += carbons[i].net;
        total_cost += carbons[i].net * carbons[i].price;
    }
    em.carbon_total = total_net;
    print_str("[EM] Carbon trading:\n");
    print_str("  Total net: "); print_int((int)total_net); print_str("tCO2\n");
    print_str("  Trading cost: "); print_int((int)total_cost); print_str("USD\n");
}

void em_efficiency_evaluation(void) {
    double target = em.total_power * 0.9;
    double savings = em.total_power - target;
    print_str("[EM] Efficiency evaluation:\n");
    print_str("  Current: "); print_int((int)em.total_power); print_str("kW\n");
    print_str("  Target: "); print_int((int)target); print_str("kW\n");
    print_str("  Savings: "); print_int((int)savings); print_str("kW\n");
    print_str("  PUE: "); print_int((int)(em.pue * 100)); print_str("/100\n");
    print_str("  Score: "); print_int(em.pue < 1.4 ? 90 : (em.pue < 1.6 ? 75 : 60)); print_str("/100\n");
}

void em_print_state(void) {
    print_str("[EM] Meters="); print_int(em.n_meters);
    print_str(" Zones="); print_int(em.n_zones);
    print_str(" Gen="); print_int(em.n_gen);
    print_str(" Stor="); print_int(em.n_storage);
    print_str(" P="); print_int((int)em.total_power); print_str("kW");
    print_str(" Cost="); print_int((int)(em.total_cost * 100)); print_str("c");
    print_str(" CO2="); print_int((int)em.carbon_total); print_str("t\n");
}

int main(void) {
    print_str("=== Energy Management System Demo ===\n\n");
    em_init();

    print_str("Adding zones...\n");
    em_add_zone(500.0);
    em_add_zone(300.0);
    em_add_zone(400.0);

    print_str("\nAdding meters...\n");
    em_add_meter(METER_ELEC, 200.0, 0);
    em_add_meter(METER_ELEC, 150.0, 1);
    em_add_meter(METER_GAS, 80.0, 0);
    em_add_meter(METER_WATER, 50.0, 2);
    em_add_meter(METER_HEAT, 120.0, 1);
    em_add_meter(METER_ELEC, 100.0, 2);

    print_str("\nConsumption analysis...\n");
    em_consumption_analysis();

    print_str("\nAdding generators...\n");
    em_add_generator(GEN_PV, 200.0, 0.05);
    em_add_generator(GEN_WIND, 150.0, 0.06);
    em_add_generator(GEN_CHP, 300.0, 0.08);

    print_str("\nAdding storage...\n");
    em_add_storage(500.0, 60.0);
    em_add_storage(300.0, 40.0);

    print_str("\nOptimization (peak shaving)...\n");
    em_optimization(OPT_PEAK_SHAVE);

    print_str("\nOptimization (demand response)...\n");
    em_optimization(OPT_DEMAND_RSP);

    print_str("\nOptimization (economic dispatch)...\n");
    em_optimization(OPT_ECO_DISP);

    print_str("\nDistributed dispatch...\n");
    em_distributed_dispatch();

    print_str("\nCarbon management...\n");
    em_add_carbon(500.0, 800.0, 25.0);
    em_add_carbon(200.0, 400.0, 28.0);
    em_carbon_trading();

    print_str("\nEfficiency evaluation...\n");
    em_efficiency_evaluation();

    em_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
