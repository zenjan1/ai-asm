/* electricity_company: Power utility management (v1.0)
 * Generation, transmission, distribution, customers, load management
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

#define MAX_PLANTS     10
#define MAX_SUBSTN     12
#define MAX_LINES      16
#define MAX_CUSTOMERS  18
#define MAX_FAULTS     14

typedef struct {
    int    plant_id;
    int    type;
    int    zone;
    int    capacity_mw;
    int    output_mw;
    int    fuel_cost;
    int    staff;
    int    active;
} plant_t;

typedef struct {
    int    sub_id;
    int    zone;
    int    voltage_kv;
    int    capacity_mw;
    int    load_mw;
    int    transformers;
    int    status;
    int    active;
} substation_t;

typedef struct {
    int    line_id;
    int    from_sub;
    int    to_sub;
    int    voltage_kv;
    int    length_km;
    int    capacity_mw;
    int    load_mw;
    int    active;
} line_t;

typedef struct {
    int    cust_id;
    int    type;
    int    zone;
    int    meter_id;
    int    consumption_kwh;
    int    bill;
    int    paid;
    int    active;
} customer_t;

typedef struct {
    int    fault_id;
    int    location_id;
    int    type;
    int    severity;
    int    reported_hour;
    int    resolved_hour;
    int    cost;
    int    active;
} fault_t;

typedef struct {
    int    n_plants;
    int    n_substations;
    int    n_lines;
    int    n_customers;
    int    n_faults;
    int    total_generation;
    int    total_load;
    int    total_revenue;
    int    total_unpaid;
    int    total_faults;
    int    total_resolved;
    int    thermal;
    int    hydro;
    int    wind;
    int    solar;
} ec_state_t;

static plant_t plants[MAX_PLANTS];
static substation_t substations[MAX_SUBSTN];
static line_t lines[MAX_LINES];
static customer_t customers[MAX_CUSTOMERS];
static fault_t faults[MAX_FAULTS];
static ec_state_t ec;

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

int ec_init(void) {
    if (initialized) return -1;
    ec.n_plants = 0; ec.n_substations = 0; ec.n_lines = 0;
    ec.n_customers = 0; ec.n_faults = 0;
    ec.total_generation = 0; ec.total_load = 0;
    ec.total_revenue = 0; ec.total_unpaid = 0;
    ec.total_faults = 0; ec.total_resolved = 0;
    ec.thermal = 0; ec.hydro = 0; ec.wind = 0; ec.solar = 0;
    for (int i = 0; i < MAX_PLANTS; i++) plants[i].active = 0;
    for (int i = 0; i < MAX_SUBSTN; i++) substations[i].active = 0;
    for (int i = 0; i < MAX_LINES; i++) lines[i].active = 0;
    for (int i = 0; i < MAX_CUSTOMERS; i++) customers[i].active = 0;
    for (int i = 0; i < MAX_FAULTS; i++) faults[i].active = 0;
    initialized = 1;
    print_str("[EC] Electricity company initialized\n");
    return 0;
}

int ec_add_plant(int type, int zone, int capacity, int fuel_cost, int staff) {
    if (ec.n_plants >= MAX_PLANTS) return -1;
    plant_t* p = &plants[ec.n_plants];
    p->plant_id = ec.n_plants;
    p->type = type;
    p->zone = zone;
    p->capacity_mw = capacity;
    p->output_mw = 0;
    p->fuel_cost = fuel_cost;
    p->staff = staff;
    p->active = 1;
    if (type == 1) ec.thermal++;
    else if (type == 2) ec.hydro++;
    else if (type == 3) ec.wind++;
    else ec.solar++;
    ec.n_plants++;
    print_str("[EC] Plant "); print_int(ec.n_plants - 1);
    print_str(" type="); print_int(type);
    print_str(" zone="); print_int(zone);
    print_str(" cap="); print_int(capacity); print_str("MW\n");
    return ec.n_plants - 1;
}

int ec_generate(int plant_id, int output) {
    if (plant_id >= ec.n_plants) return -1;
    plants[plant_id].output_mw = output;
    ec.total_generation += output;
    print_str("[EC] Generate P"); print_int(plant_id);
    print_str(" out="); print_int(output); print_str("MW\n");
    return 0;
}

int ec_add_substation(int zone, int voltage, int capacity, int transformers) {
    if (ec.n_substations >= MAX_SUBSTN) return -1;
    substation_t* s = &substations[ec.n_substations];
    s->sub_id = ec.n_substations;
    s->zone = zone;
    s->voltage_kv = voltage;
    s->capacity_mw = capacity;
    s->load_mw = 0;
    s->transformers = transformers;
    s->status = 1;
    s->active = 1;
    ec.n_substations++;
    print_str("[EC] Sub "); print_int(ec.n_substations - 1);
    print_str(" zone="); print_int(zone);
    print_str(" kv="); print_int(voltage);
    print_str(" cap="); print_int(capacity);
    print_str(" xfmr="); print_int(transformers); print_str("\n");
    return ec.n_substations - 1;
}

int ec_add_line(int from, int to, int voltage, int length, int capacity) {
    if (ec.n_lines >= MAX_LINES || from >= ec.n_substations || to >= ec.n_substations) return -1;
    line_t* l = &lines[ec.n_lines];
    l->line_id = ec.n_lines;
    l->from_sub = from;
    l->to_sub = to;
    l->voltage_kv = voltage;
    l->length_km = length;
    l->capacity_mw = capacity;
    l->load_mw = 0;
    l->active = 1;
    ec.n_lines++;
    print_str("[EC] Line "); print_int(ec.n_lines - 1);
    print_str(" S"); print_int(from); print_str("->S"); print_int(to);
    print_str(" kv="); print_int(voltage);
    print_str(" km="); print_int(length);
    print_str(" cap="); print_int(capacity); print_str("MW\n");
    return ec.n_lines - 1;
}

int ec_add_customer(int type, int zone, int meter, int consumption) {
    if (ec.n_customers >= MAX_CUSTOMERS) return -1;
    customer_t* c = &customers[ec.n_customers];
    c->cust_id = ec.n_customers;
    c->type = type;
    c->zone = zone;
    c->meter_id = meter;
    c->consumption_kwh = consumption;
    c->bill = consumption / 2;
    c->paid = 0;
    c->active = 1;
    ec.total_unpaid += c->bill;
    ec.n_customers++;
    print_str("[EC] Cust "); print_int(ec.n_customers - 1);
    print_str(" type="); print_int(type);
    print_str(" zone="); print_int(zone);
    print_str(" m="); print_int(meter);
    print_str(" kwh="); print_int(consumption);
    print_str(" bill=$"); print_int(c->bill); print_str("\n");
    return ec.n_customers - 1;
}

int ec_pay_bill(int cust_id) {
    if (cust_id >= ec.n_customers) return -1;
    if (!customers[cust_id].paid) {
        ec.total_revenue += customers[cust_id].bill;
        ec.total_unpaid -= customers[cust_id].bill;
        customers[cust_id].paid = 1;
    }
    print_str("[EC] Pay C"); print_int(cust_id);
    print_str(" $"); print_int(customers[cust_id].bill); print_str("\n");
    return 0;
}

int ec_record_fault(int location, int type, int severity, int hour, int cost) {
    if (ec.n_faults >= MAX_FAULTS) return -1;
    fault_t* f = &faults[ec.n_faults];
    f->fault_id = ec.n_faults;
    f->location_id = location;
    f->type = type;
    f->severity = severity;
    f->reported_hour = hour;
    f->resolved_hour = 0;
    f->cost = cost;
    f->active = 1;
    ec.total_faults++;
    ec.n_faults++;
    print_str("[EC] Fault "); print_int(ec.n_faults - 1);
    print_str(" loc="); print_int(location);
    print_str(" type="); print_int(type);
    print_str(" sev="); print_int(severity);
    print_str(" $"); print_int(cost); print_str("\n");
    return ec.n_faults - 1;
}

int ec_resolve_fault(int fault_id, int hour) {
    if (fault_id >= ec.n_faults) return -1;
    faults[fault_id].resolved_hour = hour;
    ec.total_resolved++;
    print_str("[EC] Resolve F"); print_int(fault_id); print_str("\n");
    return 0;
}

int ec_dispatch_load(int sub_id, int load) {
    if (sub_id >= ec.n_substations) return -1;
    substations[sub_id].load_mw += load;
    ec.total_load += load;
    print_str("[EC] Dispatch S"); print_int(sub_id);
    print_str(" load="); print_int(load); print_str("MW\n");
    return 0;
}

void ec_generation_report(void) {
    print_str("[EC] Generation report:\n");
    print_str("  Thermal: "); print_int(ec.thermal);
    print_str(" Hydro: "); print_int(ec.hydro);
    print_str(" Wind: "); print_int(ec.wind);
    print_str(" Solar: "); print_int(ec.solar); print_str("\n");
    print_str("  Total gen: "); print_int(ec.total_generation); print_str("MW\n");
    for (int i = 0; i < ec.n_plants; i++) {
        plant_t* p = &plants[i];
        print_str("  P"); print_int(i);
        print_str(" type="); print_int(p->type);
        print_str(" out="); print_int(p->output_mw);
        print_str("/"); print_int(p->capacity_mw); print_str("MW\n");
    }
}

void ec_network_report(void) {
    print_str("[EC] Network report:\n");
    print_str("  Substations: "); print_int(ec.n_substations); print_str("\n");
    print_str("  Lines: "); print_int(ec.n_lines); print_str("\n");
    print_str("  Total load: "); print_int(ec.total_load); print_str("MW\n");
}

void ec_revenue_report(void) {
    print_str("[EC] Revenue report:\n");
    print_str("  Customers: "); print_int(ec.n_customers); print_str("\n");
    print_str("  Revenue: "); print_int(ec.total_revenue); print_str("\n");
    print_str("  Unpaid: "); print_int(ec.total_unpaid); print_str("\n");
    print_str("  Faults: "); print_int(ec.total_faults);
    print_str(" resolved="); print_int(ec.total_resolved); print_str("\n");
}

void ec_print_state(void) {
    print_str("[EC] Plants="); print_int(ec.n_plants);
    print_str(" Subs="); print_int(ec.n_substations);
    print_str(" Lines="); print_int(ec.n_lines);
    print_str(" Cust="); print_int(ec.n_customers);
    print_str(" Faults="); print_int(ec.n_faults);
    print_str("\n");
    print_str("  Gen: "); print_int(ec.total_generation);
    print_str("MW Load: "); print_int(ec.total_load); print_str("MW\n");
}

int main(void) {
    print_str("=== Electricity Company Demo ===\n\n");
    ec_init();

    print_str("Adding power plants...\n");
    ec_add_plant(1, 1, 500, 80, 50);
    ec_add_plant(1, 2, 400, 75, 45);
    ec_add_plant(2, 1, 300, 10, 30);
    ec_add_plant(2, 3, 250, 12, 28);
    ec_add_plant(3, 2, 200, 0, 20);
    ec_add_plant(3, 4, 180, 0, 18);
    ec_add_plant(4, 1, 150, 0, 15);
    ec_add_plant(4, 3, 120, 0, 12);
    ec_add_plant(1, 4, 450, 85, 48);
    ec_add_plant(2, 2, 280, 11, 32);

    print_str("\nGenerating power...\n");
    for (int i = 0; i < 10; i++) {
        int output = 100 + (i * 30);
        ec_generate(i, output);
    }

    print_str("\nAdding substations...\n");
    for (int i = 0; i < 12; i++) {
        int zone = (i % 4) + 1;
        int voltage = 110 + (i % 3) * 110;
        int capacity = 100 + (i * 20);
        int transformers = 2 + (i % 3);
        ec_add_substation(zone, voltage, capacity, transformers);
    }

    print_str("\nAdding transmission lines...\n");
    for (int i = 0; i < 16; i++) {
        int from = i % 12;
        int to = (i + 4) % 12;
        int voltage = 220 + (i % 2) * 220;
        int length = 10 + (i * 5);
        int capacity = 50 + (i * 10);
        ec_add_line(from, to, voltage, length, capacity);
    }

    print_str("\nDispatching load...\n");
    for (int i = 0; i < 12; i++) {
        int load = 30 + (i * 10);
        ec_dispatch_load(i, load);
    }

    print_str("\nAdding customers...\n");
    for (int i = 0; i < 18; i++) {
        int type = (i % 3) + 1;
        int zone = (i % 4) + 1;
        int meter = 10000 + i * 100;
        int consumption = 200 + (i * 100);
        ec_add_customer(type, zone, meter, consumption);
    }

    print_str("\nCollecting payments...\n");
    for (int i = 0; i < 18; i++) {
        ec_pay_bill(i);
    }

    print_str("\nRecording faults...\n");
    for (int i = 0; i < 14; i++) {
        int location = i % 12;
        int type = (i % 4) + 1;
        int severity = (i % 3) + 1;
        int hour = 8 + (i % 12);
        int cost = 2000 + (i * 1000);
        ec_record_fault(location, type, severity, hour, cost);
    }

    print_str("\nResolving faults...\n");
    for (int i = 0; i < 14; i++) {
        int hour = 12 + (i % 8);
        ec_resolve_fault(i, hour);
    }

    print_str("\nGeneration report...\n");
    ec_generation_report();

    print_str("\nNetwork report...\n");
    ec_network_report();

    print_str("\nRevenue report...\n");
    ec_revenue_report();

    print_str("\nFinal state...\n");
    ec_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
