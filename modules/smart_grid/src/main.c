/* smart_grid: Smart grid control system (v1.0)
 * Demand response, DERs, microgrids, market mechanisms, security
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

#define MAX_ZONES     16
#define MAX_DERS      32
#define MAX_AGENTS    16
#define MAX_INTERVALS 24
#define MAX_CUSTOMERS 64

#define DR_TOU     0x01
#define DR_RTP     0x02
#define DR_CRITICAL 0x03
#define DR_EMERGENCY 0x04

#define DER_PV     0x01
#define DER_WIND   0x02
#define DER_BATT   0x03
#define DER_EV     0x04
#define DER_DIESEL 0x05
#define DER_FUELCELL 0x06

#define GRID_GRID  0x01
#define GRID_ISLAND 0x02
#define GRID_TRANSITION 0x03

#define MKT_DAYAHEAD 0x01
#define MKT_REALTIME 0x02
#define MKT_CAPACITY 0x03
#define MKT_ANCILLARY 0x04

typedef struct {
    double demand[MAX_INTERVALS];
    double price[MAX_INTERVALS];
    double supply[MAX_INTERVALS];
    double renewable[MAX_INTERVALS];
    double storage_soc[MAX_INTERVALS];
    double cost[MAX_INTERVALS];
    int    interval_count;
    double peak_demand;
    double avg_price;
    double total_revenue;
    int    dr_active;
} zone_t;

typedef struct {
    int    type;
    double capacity;
    double output;
    double soc;
    int    zone;
    double cost_per_kwh;
    double efficiency;
    int    active;
} der_t;

typedef struct {
    double power;
    double price;
    int    bid_type;
    int    zone;
    double cost;
    int    cleared;
    int    active;
} bid_t;

typedef struct {
    double total_generation;
    double total_load;
    double total_renewable;
    double total_storage;
    double net_import;
    double frequency;
    double voltage;
    int    mode;
    int    balanced;
    double carbon_intensity;
} microgrid_t;

typedef struct {
    int    type;
    double amount;
    double price;
    int    zone;
    int    cleared;
} market_clear_t;

typedef struct {
    int    zone_id;
    int    customer_class;
    double baseline;
    double curtailment;
    double incentive;
    int    enrolled;
} customer_t;

typedef struct {
    int    zone_count;
    int    der_count;
    int    customer_count;
    int    market_cleared;
    double system_frequency;
    double total_demand;
    double total_supply;
    double carbon_emissions;
    double system_cost;
    int    security_level;
} grid_state_t;

static zone_t zones[MAX_ZONES];
static der_t ders[MAX_DERS];
static bid_t bids[MAX_AGENTS * 2];
static market_clear_t market[16];
static microgrid_t microgrid;
static customer_t customers[MAX_CUSTOMERS];
static grid_state_t grid;

static int initialized = 0;
static int bid_count = 0;

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

int grid_init(void) {
    if (initialized) return -1;
    grid.zone_count = 0; grid.der_count = 0; grid.customer_count = 0;
    grid.market_cleared = 0; grid.system_frequency = 50.0;
    grid.total_demand = 0.0; grid.total_supply = 0.0;
    grid.carbon_emissions = 0.0; grid.system_cost = 0.0;
    grid.security_level = 3;
    for (int i = 0; i < MAX_ZONES; i++) {
        zones[i].interval_count = 24;
        zones[i].peak_demand = 0.0; zones[i].avg_price = 0.0;
        zones[i].total_revenue = 0.0; zones[i].dr_active = 0;
    }
    for (int i = 0; i < MAX_DERS; i++) ders[i].active = 0;
    for (int i = 0; i < MAX_CUSTOMERS; i++) customers[i].enrolled = 0;
    bid_count = 0;
    microgrid.mode = GRID_GRID; microgrid.balanced = 1;
    initialized = 1;
    print_str("[GRID] Smart grid system initialized\n");
    return 0;
}

int grid_add_zone(double base_demand) {
    if (grid.zone_count >= MAX_ZONES) return -1;
    zone_t* z = &zones[grid.zone_count];
    double daily_pattern[24] = {
        0.6, 0.55, 0.5, 0.5, 0.55, 0.65, 0.8, 0.95, 1.0, 0.95,
        0.9, 0.85, 0.8, 0.85, 0.9, 0.95, 1.0, 1.0, 0.95, 0.9,
        0.85, 0.8, 0.7, 0.65
    };
    for (int h = 0; h < 24; h++) {
        z->demand[h] = base_demand * daily_pattern[h];
        z->price[h] = 30.0 + 50.0 * daily_pattern[h];
        z->supply[h] = z->demand[h] * 1.05;
        z->renewable[h] = base_demand * 0.3 * (0.5 + 0.5 * (daily_pattern[h] > 0.7 ? 1.0 : 0.3));
        z->storage_soc[h] = 50.0 + 30.0 * ((h > 12 && h < 18) ? -1.0 : 0.5);
        z->cost[h] = z->demand[h] * z->price[h];
        if (z->demand[h] > z->peak_demand) z->peak_demand = z->demand[h];
        z->avg_price += z->price[h];
        z->total_revenue += z->cost[h];
    }
    z->avg_price /= 24.0;
    return grid.zone_count++;
}

int grid_add_der(int type, double capacity, int zone, double cost, double eff) {
    if (grid.der_count >= MAX_DERS) return -1;
    der_t* d = &ders[grid.der_count];
    d->type = type; d->capacity = capacity; d->zone = zone;
    d->output = capacity * 0.5;
    d->soc = 50.0;
    d->cost_per_kwh = cost;
    d->efficiency = eff;
    d->active = 1;
    return grid.der_count++;
}

void grid_demand_response(int zone, int dr_type) {
    if (zone >= grid.zone_count) return;
    zone_t* z = &zones[zone];
    z->dr_active = 1;
    print_str("[GRID] DR activated zone="); print_int(zone);
    print_str(" type="); print_int(dr_type); print_str("\n");
    double price_factor = 1.0;
    if (dr_type == DR_TOU) price_factor = 1.5;
    else if (dr_type == DR_RTP) price_factor = 2.0;
    else if (dr_type == DR_CRITICAL) price_factor = 3.0;
    else price_factor = 5.0;
    double reduction = 0.0;
    for (int h = 0; h < 24; h++) {
        double old_price = z->price[h];
        z->price[h] = old_price * price_factor;
        double elasticity = -0.15;
        double pct_change = (z->price[h] - old_price) / (old_price + 1e-10);
        double demand_reduction = z->demand[h] * elasticity * pct_change;
        if (demand_reduction > 0) demand_reduction = 0;
        z->demand[h] += demand_reduction;
        reduction += dabs(demand_reduction);
    }
    print_str("  Demand reduction: "); print_int((int)reduction); print_str(" MWh/day\n");
}

void grid_dispatch_ders(int zone, double target) {
    if (zone >= grid.zone_count) return;
    print_str("[GRID] DER dispatch zone="); print_int(zone);
    print_str(" target="); print_int((int)target); print_str(" MW\n");
    double remaining = target;
    for (int i = 0; i < grid.der_count && remaining > 0.0; i++) {
        if (!ders[i].active || ders[i].zone != zone) continue;
        double dispatch = 0.0;
        if (ders[i].type == DER_PV || ders[i].type == DER_WIND) {
            dispatch = ders[i].capacity * 0.4;
        } else if (ders[i].type == DER_BATT) {
            if (ders[i].soc > 20.0) {
                dispatch = ders[i].capacity * 0.5;
                ders[i].soc -= dispatch * 0.1;
            }
        } else if (ders[i].type == DER_DIESEL || ders[i].type == DER_FUELCELL) {
            dispatch = ders[i].capacity * 0.8;
        }
        if (dispatch > remaining) dispatch = remaining;
        ders[i].output = dispatch;
        remaining -= dispatch;
        print_str("  DER "); print_int(i); print_str(" type="); print_int(ders[i].type);
        print_str(" P="); print_int((int)(dispatch * 1000)); print_str("kW");
        print_str(" SOC="); print_int((int)ders[i].soc); print_str("%\n");
    }
}

void grid_microgrid_operation(int mode) {
    microgrid.mode = mode;
    microgrid.total_generation = 0.0;
    microgrid.total_load = 0.0;
    microgrid.total_renewable = 0.0;
    microgrid.total_storage = 0.0;
    microgrid.net_import = 0.0;
    for (int i = 0; i < grid.der_count; i++) {
        if (!ders[i].active) continue;
        microgrid.total_generation += ders[i].output;
        if (ders[i].type == DER_PV || ders[i].type == DER_WIND)
            microgrid.total_renewable += ders[i].output;
        if (ders[i].type == DER_BATT)
            microgrid.total_storage += ders[i].output;
    }
    for (int z = 0; z < grid.zone_count; z++) {
        for (int h = 12; h < 13 && h < zones[z].interval_count; h++) {
            microgrid.total_load += zones[z].demand[h];
        }
    }
    microgrid.net_import = microgrid.total_load - microgrid.total_generation;
    double P_gen = microgrid.total_generation;
    double P_load = microgrid.total_load > 0 ? microgrid.total_load : 1.0;
    microgrid.balanced = (dabs(P_gen - P_load) / P_load < 0.05) ? 1 : 0;
    microgrid.frequency = 50.0 + (P_gen - P_load) * 0.01;
    microgrid.voltage = 1.0 - dabs(P_gen - P_load) * 0.005;
    microgrid.carbon_intensity = 500.0 - microgrid.total_renewable * 2.0;
    if (microgrid.carbon_intensity < 50.0) microgrid.carbon_intensity = 50.0;
    print_str("[GRID] Microgrid mode=");
    if (mode == GRID_GRID) print_str("grid-connected");
    else if (mode == GRID_ISLAND) print_str("island");
    else print_str("transition");
    print_str("\n");
    print_str("  Gen="); print_int((int)(microgrid.total_generation * 1000)); print_str("kW");
    print_str(" Load="); print_int((int)(microgrid.total_load * 1000)); print_str("kW");
    print_str(" Renew="); print_int((int)(microgrid.total_renewable * 1000)); print_str("kW\n");
    print_str("  f="); print_int((int)(microgrid.frequency * 100)); print_str("0mHz");
    print_str(" V="); print_int((int)(microgrid.voltage * 1000));
    print_str("mV balance="); print_int(microgrid.balanced);
    print_str(" CO2="); print_int((int)microgrid.carbon_intensity); print_str("g/kWh\n");
}

void grid_clear_market(int market_type) {
    grid.market_cleared = 1;
    print_str("[GRID] Market clearing type="); print_int(market_type); print_str("\n");
    int mc = 0;
    for (int z = 0; z < grid.zone_count && mc < 8; z++) {
        double demand = zones[z].demand[12];
        double price = zones[z].price[12];
        market[mc].type = market_type;
        market[mc].amount = demand;
        market[mc].price = price;
        market[mc].zone = z;
        market[mc].cleared = 1;
        mc++;
    }
    grid.total_demand = 0.0;
    grid.total_supply = 0.0;
    for (int z = 0; z < grid.zone_count; z++) {
        grid.total_demand += zones[z].demand[12];
        grid.total_supply += zones[z].supply[12];
    }
    grid.system_cost = 0.0;
    for (int i = 0; i < mc; i++) {
        grid.system_cost += market[i].amount * market[i].price;
    }
    print_str("  Cleared "); print_int(mc); print_str(" zones");
    print_str(" demand="); print_int((int)grid.total_demand);
    print_str("MW supply="); print_int((int)grid.total_supply);
    print_str("MW cost=$"); print_int((int)grid.system_cost); print_str("\n");
}

void grid_enroll_customers(int zone, int class_type, int count) {
    for (int i = 0; i < count && grid.customer_count < MAX_CUSTOMERS; i++) {
        customer_t* c = &customers[grid.customer_count];
        c->zone_id = zone;
        c->customer_class = class_type;
        c->baseline = 1.0 + (grid.customer_count % 10) * 0.5;
        c->curtailment = 0.0;
        c->incentive = 50.0 + class_type * 20.0;
        c->enrolled = 1;
        grid.customer_count++;
    }
    print_str("[GRID] Enrolled "); print_int(count);
    print_str(" customers zone="); print_int(zone);
    print_str(" class="); print_int(class_type); print_str("\n");
}

void grid_security_assessment(void) {
    grid.security_level = 3;
    if (dabs(microgrid.frequency - 50.0) > 0.5) grid.security_level--;
    if (microgrid.voltage < 0.95 || microgrid.voltage > 1.05) grid.security_level--;
    if (!microgrid.balanced) grid.security_level--;
    if (grid.security_level < 0) grid.security_level = 0;
    print_str("[GRID] Security assessment: level="); print_int(grid.security_level);
    print_str("/3\n");
    print_str("  f="); print_int((int)(microgrid.frequency * 100));
    print_str("0mHz V="); print_int((int)(microgrid.voltage * 1000));
    print_str("mV balance="); print_int(microgrid.balanced); print_str("\n");
}

void grid_compute_carbon(void) {
    grid.carbon_emissions = 0.0;
    for (int i = 0; i < grid.der_count; i++) {
        if (!ders[i].active) continue;
        double emission = 0.0;
        if (ders[i].type == DER_PV || ders[i].type == DER_WIND) emission = 0.0;
        else if (ders[i].type == DER_BATT) emission = 0.0;
        else if (ders[i].type == DER_DIESEL) emission = ders[i].output * 0.8;
        else if (ders[i].type == DER_FUELCELL) emission = ders[i].output * 0.3;
        else emission = ders[i].output * 0.5;
        grid.carbon_emissions += emission;
    }
    print_str("[GRID] Carbon emissions: "); print_int((int)grid.carbon_emissions);
    print_str(" kg/h\n");
}

void grid_print_state(void) {
    print_str("[GRID] Zones="); print_int(grid.zone_count);
    print_str(" DERs="); print_int(grid.der_count);
    print_str(" Customers="); print_int(grid.customer_count);
    print_str(" Market="); print_int(grid.market_cleared);
    print_str(" f="); print_int((int)(grid.system_frequency * 100));
    print_str("0mHz Cost=$"); print_int((int)grid.system_cost);
    print_str("\n");
}

int main(void) {
    print_str("=== Smart Grid Control Demo ===\n\n");
    grid_init();

    print_str("Creating grid zones...\n");
    grid_add_zone(100.0);
    grid_add_zone(80.0);
    grid_add_zone(60.0);

    print_str("\nAdding distributed energy resources...\n");
    grid_add_der(DER_PV, 50.0, 0, 20.0, 0.95);
    grid_add_der(DER_WIND, 30.0, 0, 30.0, 0.90);
    grid_add_der(DER_BATT, 20.0, 0, 100.0, 0.92);
    grid_add_der(DER_PV, 40.0, 1, 20.0, 0.95);
    grid_add_der(DER_DIESEL, 25.0, 1, 80.0, 0.35);
    grid_add_der(DER_WIND, 20.0, 2, 35.0, 0.90);
    grid_add_der(DER_FUELCELL, 15.0, 2, 150.0, 0.50);

    print_str("\nEnrolling customers in DR program...\n");
    grid_enroll_customers(0, 1, 20);
    grid_enroll_customers(0, 2, 10);
    grid_enroll_customers(1, 1, 15);
    grid_enroll_customers(2, 3, 8);

    print_str("\nActivating demand response (TOU)...\n");
    grid_demand_response(0, DR_TOU);
    grid_demand_response(1, DR_RTP);

    print_str("\nDispatching DERs zone 0...\n");
    grid_dispatch_ders(0, 80.0);

    print_str("\nMicrogrid operation (grid-connected)...\n");
    grid_microgrid_operation(GRID_GRID);

    print_str("\nTransitioning to island mode...\n");
    grid_microgrid_operation(GRID_ISLAND);

    print_str("\nDay-ahead market clearing...\n");
    grid_clear_market(MKT_DAYAHEAD);

    print_str("\nReal-time market clearing...\n");
    grid_clear_market(MKT_REALTIME);

    print_str("\nSecurity assessment...\n");
    grid_security_assessment();

    print_str("\nCarbon accounting...\n");
    grid_compute_carbon();

    grid_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
