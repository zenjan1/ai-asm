/* gas_company: Gas utility management (v1.0)
 * Gas sources, pipeline network, customers, safety, billing
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

#define MAX_SOURCES    8
#define MAX_STATIONS   10
#define MAX_PIPES      16
#define MAX_CUSTOMERS  18
#define MAX_ALERTS     12

typedef struct {
    int    source_id;
    int    type;
    int    capacity;
    int    pressure;
    int    quality;
    int    year;
    int    active;
} source_t;

typedef struct {
    int    station_id;
    int    type;
    int    zone;
    int    pressure_in;
    int    pressure_out;
    int    capacity;
    int    flow;
    int    staff;
    int    active;
} station_t;

typedef struct {
    int    pipe_id;
    int    from_st;
    int    to_st;
    int    pressure_class;
    int    diameter_mm;
    int    length_km;
    int    flow;
    int    status;
    int    active;
} pipe_t;

typedef struct {
    int    cust_id;
    int    type;
    int    zone;
    int    meter_id;
    int    consumption_m3;
    int    bill;
    int    paid;
    int    active;
} customer_t;

typedef struct {
    int    alert_id;
    int    location_id;
    int    type;
    int    severity;
    int    pressure;
    int    threshold;
    int    hour;
    int    resolved;
    int    active;
} alert_t;

typedef struct {
    int    n_sources;
    int    n_stations;
    int    n_pipes;
    int    n_customers;
    int    n_alerts;
    int    total_supply;
    int    total_consumption;
    int    total_revenue;
    int    total_unpaid;
    int    total_alerts;
    int    total_resolved;
    int    natural_gas;
    int    lpg;
    int    coal_gas;
} gc_state_t;

static source_t sources[MAX_SOURCES];
static station_t stations[MAX_STATIONS];
static pipe_t pipes[MAX_PIPES];
static customer_t customers[MAX_CUSTOMERS];
static alert_t alerts[MAX_ALERTS];
static gc_state_t gc;

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

int gc_init(void) {
    if (initialized) return -1;
    gc.n_sources = 0; gc.n_stations = 0; gc.n_pipes = 0;
    gc.n_customers = 0; gc.n_alerts = 0;
    gc.total_supply = 0; gc.total_consumption = 0;
    gc.total_revenue = 0; gc.total_unpaid = 0;
    gc.total_alerts = 0; gc.total_resolved = 0;
    gc.natural_gas = 0; gc.lpg = 0; gc.coal_gas = 0;
    for (int i = 0; i < MAX_SOURCES; i++) sources[i].active = 0;
    for (int i = 0; i < MAX_STATIONS; i++) stations[i].active = 0;
    for (int i = 0; i < MAX_PIPES; i++) pipes[i].active = 0;
    for (int i = 0; i < MAX_CUSTOMERS; i++) customers[i].active = 0;
    for (int i = 0; i < MAX_ALERTS; i++) alerts[i].active = 0;
    initialized = 1;
    print_str("[GC] Gas company initialized\n");
    return 0;
}

int gc_add_source(int type, int capacity, int pressure, int quality, int year) {
    if (gc.n_sources >= MAX_SOURCES) return -1;
    source_t* s = &sources[gc.n_sources];
    s->source_id = gc.n_sources;
    s->type = type;
    s->capacity = capacity;
    s->pressure = pressure;
    s->quality = quality;
    s->year = year;
    s->active = 1;
    if (type == 1) gc.natural_gas++;
    else if (type == 2) gc.lpg++;
    else gc.coal_gas++;
    gc.total_supply += capacity;
    gc.n_sources++;
    print_str("[GC] Source "); print_int(gc.n_sources - 1);
    print_str(" type="); print_int(type);
    print_str(" cap="); print_int(capacity);
    print_str(" pres="); print_int(pressure);
    print_str(" qual="); print_int(quality); print_str("\n");
    return gc.n_sources - 1;
}

int gc_add_station(int type, int zone, int pressure_in, int pressure_out, int capacity, int staff) {
    if (gc.n_stations >= MAX_STATIONS) return -1;
    station_t* st = &stations[gc.n_stations];
    st->station_id = gc.n_stations;
    st->type = type;
    st->zone = zone;
    st->pressure_in = pressure_in;
    st->pressure_out = pressure_out;
    st->capacity = capacity;
    st->flow = 0;
    st->staff = staff;
    st->active = 1;
    gc.n_stations++;
    print_str("[GC] Station "); print_int(gc.n_stations - 1);
    print_str(" type="); print_int(type);
    print_str(" zone="); print_int(zone);
    print_str(" pres="); print_int(pressure_in); print_str("->"); print_int(pressure_out);
    print_str(" cap="); print_int(capacity); print_str("\n");
    return gc.n_stations - 1;
}

int gc_add_pipe(int from, int to, int pressure_class, int diameter, int length) {
    if (gc.n_pipes >= MAX_PIPES || from >= gc.n_stations || to >= gc.n_stations) return -1;
    pipe_t* p = &pipes[gc.n_pipes];
    p->pipe_id = gc.n_pipes;
    p->from_st = from;
    p->to_st = to;
    p->pressure_class = pressure_class;
    p->diameter_mm = diameter;
    p->length_km = length;
    p->flow = 0;
    p->status = 1;
    p->active = 1;
    gc.n_pipes++;
    print_str("[GC] Pipe "); print_int(gc.n_pipes - 1);
    print_str(" St"); print_int(from); print_str("->St"); print_int(to);
    print_str(" cls="); print_int(pressure_class);
    print_str(" mm="); print_int(diameter);
    print_str(" km="); print_int(length); print_str("\n");
    return gc.n_pipes - 1;
}

int gc_flow_gas(int station_id, int flow) {
    if (station_id >= gc.n_stations) return -1;
    stations[station_id].flow += flow;
    print_str("[GC] Flow St"); print_int(station_id);
    print_str(" flow="); print_int(flow); print_str("\n");
    return 0;
}

int gc_add_customer(int type, int zone, int meter, int consumption) {
    if (gc.n_customers >= MAX_CUSTOMERS) return -1;
    customer_t* c = &customers[gc.n_customers];
    c->cust_id = gc.n_customers;
    c->type = type;
    c->zone = zone;
    c->meter_id = meter;
    c->consumption_m3 = consumption;
    int price = (type == 1) ? 3 : (type == 2 ? 4 : 5);
    c->bill = consumption * price;
    c->paid = 0;
    c->active = 1;
    gc.total_unpaid += c->bill;
    gc.total_consumption += consumption;
    gc.n_customers++;
    print_str("[GC] Cust "); print_int(gc.n_customers - 1);
    print_str(" type="); print_int(type);
    print_str(" zone="); print_int(zone);
    print_str(" m3="); print_int(consumption);
    print_str(" bill=$"); print_int(c->bill); print_str("\n");
    return gc.n_customers - 1;
}

int gc_pay_bill(int cust_id) {
    if (cust_id >= gc.n_customers) return -1;
    if (!customers[cust_id].paid) {
        gc.total_revenue += customers[cust_id].bill;
        gc.total_unpaid -= customers[cust_id].bill;
        customers[cust_id].paid = 1;
    }
    print_str("[GC] Pay C"); print_int(cust_id);
    print_str(" $"); print_int(customers[cust_id].bill); print_str("\n");
    return 0;
}

int gc_record_alert(int location, int type, int severity, int pressure, int threshold, int hour) {
    if (gc.n_alerts >= MAX_ALERTS) return -1;
    alert_t* a = &alerts[gc.n_alerts];
    a->alert_id = gc.n_alerts;
    a->location_id = location;
    a->type = type;
    a->severity = severity;
    a->pressure = pressure;
    a->threshold = threshold;
    a->hour = hour;
    a->resolved = 0;
    a->active = 1;
    gc.total_alerts++;
    gc.n_alerts++;
    print_str("[GC] Alert "); print_int(gc.n_alerts - 1);
    print_str(" loc="); print_int(location);
    print_str(" type="); print_int(type);
    print_str(" sev="); print_int(severity);
    print_str(" pres="); print_int(pressure); print_str("\n");
    return gc.n_alerts - 1;
}

int gc_resolve_alert(int alert_id) {
    if (alert_id >= gc.n_alerts) return -1;
    alerts[alert_id].resolved = 1;
    gc.total_resolved++;
    print_str("[GC] Resolve Al"); print_int(alert_id); print_str("\n");
    return 0;
}

void gc_source_report(void) {
    print_str("[GC] Source report:\n");
    print_str("  Natural gas: "); print_int(gc.natural_gas);
    print_str(" LPG: "); print_int(gc.lpg);
    print_str(" Coal gas: "); print_int(gc.coal_gas); print_str("\n");
    print_str("  Total supply: "); print_int(gc.total_supply); print_str("\n");
}

void gc_network_report(void) {
    print_str("[GC] Network report:\n");
    print_str("  Stations: "); print_int(gc.n_stations); print_str("\n");
    print_str("  Pipes: "); print_int(gc.n_pipes); print_str("\n");
    for (int i = 0; i < gc.n_stations; i++) {
        station_t* st = &stations[i];
        print_str("  St"); print_int(i);
        print_str(" type="); print_int(st->type);
        print_str(" zone="); print_int(st->zone);
        print_str(" flow="); print_int(st->flow);
        print_str("/"); print_int(st->capacity); print_str("\n");
    }
}

void gc_revenue_report(void) {
    print_str("[GC] Revenue report:\n");
    print_str("  Customers: "); print_int(gc.n_customers); print_str("\n");
    print_str("  Consumption: "); print_int(gc.total_consumption); print_str(" m3\n");
    print_str("  Revenue: "); print_int(gc.total_revenue); print_str("\n");
    print_str("  Unpaid: "); print_int(gc.total_unpaid); print_str("\n");
    print_str("  Alerts: "); print_int(gc.total_alerts);
    print_str(" resolved="); print_int(gc.total_resolved); print_str("\n");
}

void gc_print_state(void) {
    print_str("[GC] Src="); print_int(gc.n_sources);
    print_str(" Stn="); print_int(gc.n_stations);
    print_str(" Pipe="); print_int(gc.n_pipes);
    print_str(" Cust="); print_int(gc.n_customers);
    print_str(" Alert="); print_int(gc.n_alerts);
    print_str("\n");
    print_str("  Revenue: "); print_int(gc.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Gas Company Demo ===\n\n");
    gc_init();

    print_str("Adding gas sources...\n");
    gc_add_source(1, 50000, 80, 95, 2010);
    gc_add_source(1, 40000, 75, 93, 2012);
    gc_add_source(2, 30000, 60, 90, 2015);
    gc_add_source(2, 25000, 55, 88, 2018);
    gc_add_source(3, 20000, 40, 85, 2008);
    gc_add_source(1, 45000, 78, 94, 2014);
    gc_add_source(3, 18000, 38, 82, 2011);
    gc_add_source(2, 28000, 58, 89, 2016);

    print_str("\nAdding regulation stations...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 3) + 1;
        int zone = (i % 4) + 1;
        int pressure_in = 80 - (i * 5);
        int pressure_out = 20 - (i * 2);
        int capacity = 5000 + (i * 500);
        int staff = 5 + (i % 5);
        gc_add_station(type, zone, pressure_in, pressure_out, capacity, staff);
    }

    print_str("\nAdding pipelines...\n");
    for (int i = 0; i < 16; i++) {
        int from = i % 10;
        int to = (i + 3) % 10;
        int pressure_class = (i % 3) + 1;
        int diameter = 100 + (i * 50);
        int length = 5 + (i * 3);
        gc_add_pipe(from, to, pressure_class, diameter, length);
    }

    print_str("\nFlowing gas...\n");
    for (int i = 0; i < 10; i++) {
        int flow = 500 + (i * 200);
        gc_flow_gas(i, flow);
    }

    print_str("\nAdding customers...\n");
    for (int i = 0; i < 18; i++) {
        int type = (i % 3) + 1;
        int zone = (i % 4) + 1;
        int meter = 20000 + i * 100;
        int consumption = 50 + (i * 30);
        gc_add_customer(type, zone, meter, consumption);
    }

    print_str("\nCollecting payments...\n");
    for (int i = 0; i < 18; i++) {
        gc_pay_bill(i);
    }

    print_str("\nRecording alerts...\n");
    for (int i = 0; i < 12; i++) {
        int location = i % 10;
        int type = (i % 3) + 1;
        int severity = (i % 4) + 1;
        int pressure = 40 + (i * 5);
        int threshold = 60;
        int hour = 8 + (i % 12);
        gc_record_alert(location, type, severity, pressure, threshold, hour);
    }

    print_str("\nResolving alerts...\n");
    for (int i = 0; i < 12; i++) {
        gc_resolve_alert(i);
    }

    print_str("\nSource report...\n");
    gc_source_report();

    print_str("\nNetwork report...\n");
    gc_network_report();

    print_str("\nRevenue report...\n");
    gc_revenue_report();

    print_str("\nFinal state...\n");
    gc_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
