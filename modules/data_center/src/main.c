/* data_center: Data center management (v1.0)
 * Servers, cooling, power, network, storage, energy analytics
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

#define MAX_RACKS     12
#define MAX_SERVERS   20
#define MAX_COOLING   8
#define MAX_NETWORK   10
#define MAX_STORAGE   8
#define MAX_ALERTS    16

typedef struct {
    int    rack_id;
    int    capacity_u;
    int    used_u;
    int    power_capacity_kw;
    int    power_used_w;
    int    n_servers;
    int    zone;
    int    active;
} rack_t;

typedef struct {
    int    srv_id;
    int    rack_id;
    int    type;
    int    cpu_cores;
    int    ram_gb;
    int    disk_tb;
    int    power_w;
    int    utilization;
    int    uptime_days;
    double revenue;
    int    active;
} server_t;

typedef struct {
    int    cool_id;
    int    type;
    int    capacity_kw;
    int    load_kw;
    double efficiency;
    int    n_units;
    int    active;
} cooling_t;

typedef struct {
    int    net_id;
    int    type;
    int    bandwidth_gbps;
    int    used_gbps;
    int    n_ports;
    int    active_ports;
    int    active;
} network_t;

typedef struct {
    int    stor_id;
    int    type;
    int    capacity_tb;
    int    used_tb;
    int    iops;
    double cost_per_tb;
    int    active;
} storage_t;

typedef struct {
    int    alert_id;
    int    type;
    int    severity;
    int    source_id;
    int    resolved;
    int    day;
    int    active;
} alert_t;

typedef struct {
    int    n_racks;
    int    n_servers;
    int    n_cooling;
    int    n_network;
    int    n_storage;
    int    n_alerts;
    double compute_revenue;
    double storage_revenue;
    double network_revenue;
    double total_revenue;
    double power_costs;
    double cooling_costs;
    double maintenance_costs;
    int    total_power_kw;
    int    total_cooling_kw;
    double pue;
} dc_state_t;

static rack_t racks[MAX_RACKS];
static server_t servers[MAX_SERVERS];
static cooling_t cooling[MAX_COOLING];
static network_t networks[MAX_NETWORK];
static storage_t storage[MAX_STORAGE];
static alert_t alerts[MAX_ALERTS];
static dc_state_t dc;

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

int dc_init(void) {
    if (initialized) return -1;
    dc.n_racks = 0; dc.n_servers = 0; dc.n_cooling = 0;
    dc.n_network = 0; dc.n_storage = 0; dc.n_alerts = 0;
    dc.compute_revenue = 0.0; dc.storage_revenue = 0.0;
    dc.network_revenue = 0.0; dc.total_revenue = 0.0;
    dc.power_costs = 0.0; dc.cooling_costs = 0.0;
    dc.maintenance_costs = 0.0;
    dc.total_power_kw = 0; dc.total_cooling_kw = 0;
    dc.pue = 1.0;
    for (int i = 0; i < MAX_RACKS; i++) racks[i].active = 0;
    for (int i = 0; i < MAX_SERVERS; i++) servers[i].active = 0;
    for (int i = 0; i < MAX_COOLING; i++) cooling[i].active = 0;
    for (int i = 0; i < MAX_NETWORK; i++) networks[i].active = 0;
    for (int i = 0; i < MAX_STORAGE; i++) storage[i].active = 0;
    for (int i = 0; i < MAX_ALERTS; i++) alerts[i].active = 0;
    initialized = 1;
    print_str("[DC] Data center initialized\n");
    return 0;
}

int dc_add_rack(int capacity_u, int power_kw, int zone) {
    if (dc.n_racks >= MAX_RACKS) return -1;
    rack_t* r = &racks[dc.n_racks];
    r->rack_id = dc.n_racks;
    r->capacity_u = capacity_u;
    r->used_u = 0;
    r->power_capacity_kw = power_kw;
    r->power_used_w = 0;
    r->n_servers = 0;
    r->zone = zone;
    r->active = 1;
    dc.n_racks++;
    print_str("[DC] Rack "); print_int(dc.n_racks - 1);
    print_str(" U="); print_int(capacity_u);
    print_str(" kW="); print_int(power_kw);
    print_str(" zone="); print_int(zone); print_str("\n");
    return dc.n_racks - 1;
}

int dc_add_server(int rack_id, int type, int cores, int ram_gb, int disk_tb, int power_w) {
    if (dc.n_servers >= MAX_SERVERS || rack_id >= dc.n_racks) return -1;
    server_t* s = &servers[dc.n_servers];
    s->srv_id = dc.n_servers;
    s->rack_id = rack_id;
    s->type = type;
    s->cpu_cores = cores;
    s->ram_gb = ram_gb;
    s->disk_tb = disk_tb;
    s->power_w = power_w;
    s->utilization = 0;
    s->uptime_days = 0;
    s->revenue = 0.0;
    s->active = 1;
    int rack_u = 2 + (type % 3);
    racks[rack_id].used_u += rack_u;
    racks[rack_id].power_used_w += power_w;
    racks[rack_id].n_servers++;
    dc.total_power_kw += power_w / 1000;
    dc.n_servers++;
    print_str("[DC] Server "); print_int(dc.n_servers - 1);
    print_str(" R"); print_int(rack_id);
    print_str(" type="); print_int(type);
    print_str(" cores="); print_int(cores);
    print_str(" ram="); print_int(ram_gb); print_str("GB\n");
    return dc.n_servers - 1;
}

int dc_add_cooling(int type, int capacity_kw, int n_units) {
    if (dc.n_cooling >= MAX_COOLING) return -1;
    cooling_t* c = &cooling[dc.n_cooling];
    c->cool_id = dc.n_cooling;
    c->type = type;
    c->capacity_kw = capacity_kw;
    c->load_kw = 0;
    c->efficiency = 0.85 + (type * 0.03);
    c->n_units = n_units;
    c->active = 1;
    dc.n_cooling++;
    print_str("[DC] Cooling "); print_int(dc.n_cooling - 1);
    print_str(" type="); print_int(type);
    print_str(" cap="); print_int(capacity_kw);
    print_str("kW units="); print_int(n_units); print_str("\n");
    return dc.n_cooling - 1;
}

int dc_add_network(int type, int bandwidth_gbps, int n_ports) {
    if (dc.n_network >= MAX_NETWORK) return -1;
    network_t* n = &networks[dc.n_network];
    n->net_id = dc.n_network;
    n->type = type;
    n->bandwidth_gbps = bandwidth_gbps;
    n->used_gbps = 0;
    n->n_ports = n_ports;
    n->active_ports = 0;
    n->active = 1;
    dc.n_network++;
    print_str("[DC] Network "); print_int(dc.n_network - 1);
    print_str(" type="); print_int(type);
    print_str(" Gbps="); print_int(bandwidth_gbps);
    print_str(" ports="); print_int(n_ports); print_str("\n");
    return dc.n_network - 1;
}

int dc_add_storage(int type, int capacity_tb, int iops, double cost) {
    if (dc.n_storage >= MAX_STORAGE) return -1;
    storage_t* st = &storage[dc.n_storage];
    st->stor_id = dc.n_storage;
    st->type = type;
    st->capacity_tb = capacity_tb;
    st->used_tb = 0;
    st->iops = iops;
    st->cost_per_tb = cost;
    st->active = 1;
    dc.n_storage++;
    print_str("[DC] Storage "); print_int(dc.n_storage - 1);
    print_str(" type="); print_int(type);
    print_str(" TB="); print_int(capacity_tb);
    print_str(" IOPS="); print_int(iops); print_str("\n");
    return dc.n_storage - 1;
}

int dc_deploy_workload(int srv_id, int utilization, int hours) {
    if (srv_id >= dc.n_servers) return -1;
    server_t* s = &servers[srv_id];
    s->utilization = utilization;
    s->uptime_days += hours / 24;
    double rev = s->cpu_cores * 5.0 + s->ram_gb * 2.0;
    rev = rev * hours / 24 * utilization / 100;
    s->revenue += rev;
    dc.compute_revenue += rev;
    dc.total_revenue += rev;
    dc.power_costs += s->power_w * hours * 0.12 / 1000.0;
    print_str("[DC] Workload S"); print_int(srv_id);
    print_str(" util="); print_int(utilization);
    print_str("% hrs="); print_int(hours);
    print_str(" $"); print_int((int)rev); print_str("\n");
    return 0;
}

int dc_allocate_storage(int stor_id, int tb) {
    if (stor_id >= dc.n_storage) return -1;
    storage_t* st = &storage[stor_id];
    if (st->used_tb + tb > st->capacity_tb) return -2;
    st->used_tb += tb;
    double cost = st->cost_per_tb * tb;
    double rev = cost * 3.0;
    dc.storage_revenue += rev;
    dc.total_revenue += rev;
    print_str("[DC] Storage St"); print_int(stor_id);
    print_str(" TB="); print_int(tb);
    print_str(" $"); print_int((int)rev); print_str("\n");
    return 0;
}

int dc_connect_network(int net_id, int bandwidth_used) {
    if (net_id >= dc.n_network) return -1;
    network_t* n = &networks[net_id];
    if (n->used_gbps + bandwidth_used > n->bandwidth_gbps) return -2;
    n->used_gbps += bandwidth_used;
    n->active_ports++;
    double rev = bandwidth_used * 50.0;
    dc.network_revenue += rev;
    dc.total_revenue += rev;
    print_str("[DC] Net "); print_int(net_id);
    print_str(" Gbps="); print_int(bandwidth_used);
    print_str(" $"); print_int((int)rev); print_str("\n");
    return 0;
}

int dc_add_alert(int type, int severity, int source_id, int day) {
    if (dc.n_alerts >= MAX_ALERTS) return -1;
    alert_t* a = &alerts[dc.n_alerts];
    a->alert_id = dc.n_alerts;
    a->type = type;
    a->severity = severity;
    a->source_id = source_id;
    a->resolved = 0;
    a->day = day;
    a->active = 1;
    dc.n_alerts++;
    dc.maintenance_costs += severity * 100.0;
    print_str("[DC] Alert "); print_int(dc.n_alerts - 1);
    print_str(" type="); print_int(type);
    print_str(" sev="); print_int(severity);
    print_str(" src="); print_int(source_id); print_str("\n");
    return dc.n_alerts - 1;
}

int dc_resolve_alert(int alert_id) {
    if (alert_id >= dc.n_alerts) return -1;
    alerts[alert_id].resolved = 1;
    print_str("[DC] Resolve A"); print_int(alert_id); print_str("\n");
    return 0;
}

int dc_set_cooling_load(int cool_id, int load_kw) {
    if (cool_id >= dc.n_cooling) return -1;
    cooling_t* c = &cooling[cool_id];
    c->load_kw = load_kw;
    dc.total_cooling_kw += load_kw;
    dc.cooling_costs += load_kw * 0.08;
    print_str("[DC] Cool "); print_int(cool_id);
    print_str(" load="); print_int(load_kw); print_str("kW\n");
    return 0;
}

void dc_infra_report(void) {
    print_str("[DC] Infrastructure report:\n");
    for (int i = 0; i < dc.n_racks; i++) {
        rack_t* r = &racks[i];
        print_str("  R"); print_int(i);
        print_str(" U="); print_int(r->used_u);
        print_str("/"); print_int(r->capacity_u);
        print_str(" srv="); print_int(r->n_servers);
        print_str(" W="); print_int(r->power_used_w); print_str("\n");
    }
    for (int i = 0; i < dc.n_servers; i++) {
        server_t* s = &servers[i];
        print_str("  S"); print_int(i);
        print_str(" R"); print_int(s->rack_id);
        print_str(" type="); print_int(s->type);
        print_str(" util="); print_int(s->utilization);
        print_str("% rev="); print_int((int)s->revenue); print_str("\n");
    }
}

void dc_financial_report(void) {
    print_str("[DC] Financial report:\n");
    print_str("  Compute revenue: "); print_int((int)dc.compute_revenue); print_str("\n");
    print_str("  Storage revenue: "); print_int((int)dc.storage_revenue); print_str("\n");
    print_str("  Network revenue: "); print_int((int)dc.network_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)dc.total_revenue); print_str("\n");
    print_str("  Power costs: "); print_int((int)dc.power_costs); print_str("\n");
    print_str("  Cooling costs: "); print_int((int)dc.cooling_costs); print_str("\n");
    print_str("  Maintenance costs: "); print_int((int)dc.maintenance_costs); print_str("\n");
    double total_costs = dc.power_costs + dc.cooling_costs + dc.maintenance_costs;
    double profit = dc.total_revenue - total_costs;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    print_str("  Power: "); print_int(dc.total_power_kw);
    print_str("kW Cooling: "); print_int(dc.total_cooling_kw);
    print_str("kW Alerts: "); print_int(dc.n_alerts); print_str("\n");
}

void dc_print_state(void) {
    print_str("[DC] Racks="); print_int(dc.n_racks);
    print_str(" Servers="); print_int(dc.n_servers);
    print_str(" Cooling="); print_int(dc.n_cooling);
    print_str(" Net="); print_int(dc.n_network);
    print_str(" Storage="); print_int(dc.n_storage);
    print_str(" Alerts="); print_int(dc.n_alerts);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)dc.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Data Center Demo ===\n\n");
    dc_init();

    print_str("Adding racks...\n");
    dc_add_rack(42, 10, 1);
    dc_add_rack(48, 12, 1);
    dc_add_rack(42, 10, 2);
    dc_add_rack(48, 14, 2);
    dc_add_rack(36, 8, 1);
    dc_add_rack(42, 11, 3);
    dc_add_rack(48, 13, 2);
    dc_add_rack(42, 10, 3);
    dc_add_rack(36, 9, 1);
    dc_add_rack(48, 12, 2);
    dc_add_rack(42, 11, 3);
    dc_add_rack(48, 14, 1);

    print_str("\nAdding servers...\n");
    dc_add_server(0, 1, 16, 64, 2, 500);
    dc_add_server(0, 2, 32, 128, 4, 800);
    dc_add_server(1, 1, 16, 64, 2, 500);
    dc_add_server(1, 3, 64, 256, 8, 1200);
    dc_add_server(2, 2, 32, 128, 4, 800);
    dc_add_server(2, 1, 16, 64, 2, 500);
    dc_add_server(3, 3, 64, 256, 8, 1200);
    dc_add_server(3, 2, 32, 128, 4, 800);
    dc_add_server(4, 1, 8, 32, 1, 350);
    dc_add_server(5, 2, 32, 128, 4, 800);
    dc_add_server(5, 3, 48, 192, 6, 1000);
    dc_add_server(6, 1, 16, 64, 2, 500);
    dc_add_server(6, 2, 32, 128, 4, 800);
    dc_add_server(7, 3, 64, 256, 8, 1200);
    dc_add_server(8, 1, 8, 32, 1, 350);
    dc_add_server(9, 2, 32, 128, 4, 800);
    dc_add_server(9, 1, 16, 64, 2, 500);
    dc_add_server(10, 3, 48, 192, 6, 1000);
    dc_add_server(11, 2, 32, 128, 4, 800);
    dc_add_server(11, 1, 16, 64, 2, 500);

    print_str("\nAdding cooling...\n");
    dc_add_cooling(1, 50, 4);
    dc_add_cooling(2, 80, 3);
    dc_add_cooling(1, 60, 4);
    dc_add_cooling(3, 100, 2);
    dc_add_cooling(2, 70, 3);
    dc_add_cooling(1, 55, 4);
    dc_add_cooling(3, 90, 2);
    dc_add_cooling(2, 65, 3);

    print_str("\nAdding network...\n");
    dc_add_network(1, 100, 48);
    dc_add_network(2, 40, 24);
    dc_add_network(1, 100, 48);
    dc_add_network(3, 25, 12);
    dc_add_network(2, 40, 24);
    dc_add_network(1, 100, 48);
    dc_add_network(3, 25, 12);
    dc_add_network(2, 40, 24);
    dc_add_network(1, 100, 48);
    dc_add_network(3, 25, 12);

    print_str("\nAdding storage...\n");
    dc_add_storage(1, 500, 100000, 25.0);
    dc_add_storage(2, 200, 500000, 80.0);
    dc_add_storage(1, 800, 80000, 20.0);
    dc_add_storage(3, 100, 1000000, 150.0);
    dc_add_storage(2, 300, 400000, 70.0);
    dc_add_storage(1, 600, 90000, 22.0);
    dc_add_storage(3, 150, 800000, 140.0);
    dc_add_storage(2, 250, 450000, 75.0);

    print_str("\nDeploying workloads...\n");
    for (int i = 0; i < 20; i++) {
        dc_deploy_workload(i, 40 + (i * 3), 24 + (i % 12));
    }

    print_str("\nAllocating storage...\n");
    for (int i = 0; i < 8; i++) {
        dc_allocate_storage(i, 50 + (i * 20));
    }

    print_str("\nConnecting network...\n");
    for (int i = 0; i < 10; i++) {
        dc_connect_network(i, 5 + (i * 3));
    }

    print_str("\nSetting cooling loads...\n");
    for (int i = 0; i < 8; i++) {
        dc_set_cooling_load(i, 20 + (i * 5));
    }

    print_str("\nAlerts...\n");
    dc_add_alert(1, 2, 3, 10);
    dc_add_alert(2, 3, 7, 11);
    dc_add_alert(1, 1, 12, 12);
    dc_add_alert(3, 4, 5, 13);
    dc_add_alert(2, 2, 15, 14);
    dc_add_alert(1, 3, 8, 15);
    dc_add_alert(3, 1, 1, 16);
    dc_add_alert(2, 2, 10, 17);
    dc_add_alert(1, 4, 18, 18);
    dc_add_alert(3, 2, 6, 19);
    dc_add_alert(2, 1, 4, 20);
    dc_add_alert(1, 3, 11, 21);
    dc_add_alert(3, 2, 9, 22);
    dc_add_alert(2, 3, 14, 23);
    dc_add_alert(1, 1, 2, 24);
    dc_add_alert(3, 4, 16, 25);

    print_str("\nResolving alerts...\n");
    for (int i = 0; i < 16; i++) {
        dc_resolve_alert(i);
    }

    print_str("\nInfrastructure report...\n");
    dc_infra_report();

    print_str("\nFinancial report...\n");
    dc_financial_report();

    print_str("\nFinal state...\n");
    dc_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
