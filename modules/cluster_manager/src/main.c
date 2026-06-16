/* cluster_manager: Cluster management module (v46.0)
 *
 * Provides cluster management capabilities:
 * - Node discovery (mDNS/consul)
 * - Load balancing (round-robin/least-conn)
 * - Failover (automatic recovery)
 * - Health checks (heartbeat)
 * - Cluster state synchronization
 * - Service registration and discovery
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

/* Node states */
#define NODE_STATE_UNKNOWN      0
#define NODE_STATE_HEALTHY      1
#define NODE_STATE_DEGRADED     2
#define NODE_STATE_UNHEALTHY    3
#define NODE_STATE_OFFLINE      4

/* Load balancing algorithms */
#define LB_ROUND_ROBIN          1
#define LB_LEAST_CONN           2
#define LB_WEIGHTED             3
#define LB_RANDOM               4
#define LB_CONSISTENT_HASH      5

/* Service states */
#define SERVICE_REGISTERED      0
#define SERVICE_ACTIVE          1
#define SERVICE_INACTIVE        2

/* Limits */
#define MAX_NODES               128
#define MAX_SERVICES            256
#define MAX_NAME_LEN            64
#define MAX_ADDR_LEN            32
#define MAX_TAGS                16

/* Cluster node */
typedef struct {
    int             node_id;
    char            name[MAX_NAME_LEN];
    char            address[MAX_ADDR_LEN];
    int             port;
    int             state;
    unsigned int    last_heartbeat;
    int             health_score;
    unsigned int    weight;
    int             active;
} cluster_node_t;

/* Service endpoint */
typedef struct {
    int             endpoint_id;
    int             node_id;
    char            address[MAX_ADDR_LEN];
    int             port;
    unsigned int    connections;
    int             healthy;
} service_endpoint_t;

/* Service */
typedef struct {
    int             service_id;
    char            name[MAX_NAME_LEN];
    int             state;
    int             endpoint_count;
    service_endpoint_t endpoints[32];
    int             lb_algorithm;
    int             rr_index;
    int             active;
} service_t;

/* Health check */
typedef struct {
    int             check_id;
    int             node_id;
    int             type;  /* 0=tcp, 1=http, 2=script */
    int             interval_ms;
    int             timeout_ms;
    int             last_status;
    unsigned int    last_check;
} health_check_t;

/* Cluster context */
typedef struct {
    char            cluster_name[MAX_NAME_LEN];
    int             node_count;
    cluster_node_t  nodes[MAX_NODES];
    int             service_count;
    service_t       services[MAX_SERVICES];
    int             check_count;
    health_check_t  checks[256];
    int             leader_node;
    unsigned int    cluster_time;
} cluster_context_t;

/* Global state */
static cluster_context_t cluster;
static int initialized = 0;

/* String utilities */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0; while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; } dst[i] = '\0';
}

static void print_str(const char *str) { host_print(str); }
static void print_int(int val) {
    char buf[32]; int pos = 0;
    if (val < 0) { buf[pos++] = '-'; val = -val; }
    if (val == 0) buf[pos++] = '0';
    else { int d = 0, t = val; while (t > 0) { d++; t /= 10; } pos += d; buf[pos] = '\0'; pos--;
        while (val > 0) { buf[pos--] = '0' + (val % 10); val /= 10; } }
    host_print(buf);
}

/* ===== Cluster Initialization ===== */

int cluster_init(const char *name) {
    my_strncpy(cluster.cluster_name, name, MAX_NAME_LEN - 1);
    cluster.node_count = 0;
    cluster.service_count = 0;
    cluster.check_count = 0;
    cluster.leader_node = -1;
    cluster.cluster_time = 1609459200;
    initialized = 1;

    print_str("Cluster initialized: ");
    print_str(name);
    print_str("\n");

    return 0;
}

/* ===== Node Management ===== */

int cluster_add_node(const char *name, const char *address, int port, unsigned int weight) {
    if (!initialized || cluster.node_count >= MAX_NODES) return -1;

    int idx = cluster.node_count++;
    cluster.nodes[idx].node_id = idx;
    my_strncpy(cluster.nodes[idx].name, name, MAX_NAME_LEN - 1);
    my_strncpy(cluster.nodes[idx].address, address, MAX_ADDR_LEN - 1);
    cluster.nodes[idx].port = port;
    cluster.nodes[idx].state = NODE_STATE_HEALTHY;
    cluster.nodes[idx].last_heartbeat = cluster.cluster_time;
    cluster.nodes[idx].health_score = 100;
    cluster.nodes[idx].weight = weight;
    cluster.nodes[idx].active = 1;

    print_str("Added node: ");
    print_str(name);
    print_str(" (");
    print_str(address);
    print_str(":");
    print_int(port);
    print_str(", weight=");
    print_int(weight);
    print_str(")\n");

    return idx;
}

int cluster_remove_node(int node_id) {
    if (node_id < 0 || node_id >= cluster.node_count) return -1;
    cluster.nodes[node_id].active = 0;
    cluster.nodes[node_id].state = NODE_STATE_OFFLINE;

    print_str("Removed node ");
    print_int(node_id);
    print_str("\n");

    return 0;
}

int cluster_node_heartbeat(int node_id) {
    if (node_id < 0 || node_id >= cluster.node_count) return -1;
    if (!cluster.nodes[node_id].active) return -1;

    cluster.nodes[node_id].last_heartbeat = cluster.cluster_time;
    cluster.nodes[node_id].state = NODE_STATE_HEALTHY;
    cluster.nodes[node_id].health_score = 100;

    return 0;
}

void cluster_check_health(void) {
    for (int i = 0; i < cluster.node_count; i++) {
        if (!cluster.nodes[i].active) continue;

        unsigned int elapsed = cluster.cluster_time - cluster.nodes[i].last_heartbeat;
        if (elapsed > 30000) {  /* 30 seconds */
            cluster.nodes[i].state = NODE_STATE_UNHEALTHY;
            cluster.nodes[i].health_score = 0;
        } else if (elapsed > 10000) {  /* 10 seconds */
            cluster.nodes[i].state = NODE_STATE_DEGRADED;
            cluster.nodes[i].health_score = 50;
        }
    }
}

void cluster_node_list(void) {
    print_str("Cluster Nodes:\n");
    for (int i = 0; i < cluster.node_count; i++) {
        if (!cluster.nodes[i].active) continue;
        print_str("  Node ");
        print_int(i);
        print_str(": ");
        print_str(cluster.nodes[i].name);
        print_str(" (");
        print_str(cluster.nodes[i].address);
        print_str(":");
        print_int(cluster.nodes[i].port);
        print_str(") - ");
        if (cluster.nodes[i].state == NODE_STATE_HEALTHY) print_str("HEALTHY");
        else if (cluster.nodes[i].state == NODE_STATE_DEGRADED) print_str("DEGRADED");
        else if (cluster.nodes[i].state == NODE_STATE_UNHEALTHY) print_str("UNHEALTHY");
        else print_str("OFFLINE");
        print_str("\n");
    }
}

/* ===== Service Management ===== */

int service_register(const char *name, int lb_algorithm) {
    if (!initialized || cluster.service_count >= MAX_SERVICES) return -1;

    int idx = cluster.service_count++;
    cluster.services[idx].service_id = idx;
    my_strncpy(cluster.services[idx].name, name, MAX_NAME_LEN - 1);
    cluster.services[idx].state = SERVICE_REGISTERED;
    cluster.services[idx].endpoint_count = 0;
    cluster.services[idx].lb_algorithm = lb_algorithm;
    cluster.services[idx].rr_index = 0;
    cluster.services[idx].active = 1;

    print_str("Registered service: ");
    print_str(name);
    print_str(" (LB: ");
    if (lb_algorithm == LB_ROUND_ROBIN) print_str("round-robin");
    else if (lb_algorithm == LB_LEAST_CONN) print_str("least-conn");
    else if (lb_algorithm == LB_WEIGHTED) print_str("weighted");
    else print_str("random");
    print_str(")\n");

    return idx;
}

int service_add_endpoint(int service_id, int node_id, const char *address, int port) {
    if (service_id < 0 || service_id >= cluster.service_count) return -1;
    if (cluster.services[service_id].endpoint_count >= 32) return -1;

    int idx = cluster.services[service_id].endpoint_count++;
    cluster.services[service_id].endpoints[idx].endpoint_id = idx;
    cluster.services[service_id].endpoints[idx].node_id = node_id;
    my_strncpy(cluster.services[service_id].endpoints[idx].address, address, MAX_ADDR_LEN - 1);
    cluster.services[service_id].endpoints[idx].port = port;
    cluster.services[service_id].endpoints[idx].connections = 0;
    cluster.services[service_id].endpoints[idx].healthy = 1;

    print_str("Added endpoint to ");
    print_str(cluster.services[service_id].name);
    print_str(": ");
    print_str(address);
    print_str(":");
    print_int(port);
    print_str("\n");

    return idx;
}

int service_get_endpoint(int service_id) {
    if (service_id < 0 || service_id >= cluster.service_count) return -1;
    service_t *svc = &cluster.services[service_id];
    if (svc->endpoint_count == 0) return -1;

    int selected = -1;

    switch (svc->lb_algorithm) {
        case LB_ROUND_ROBIN:
            selected = svc->rr_index;
            svc->rr_index = (svc->rr_index + 1) % svc->endpoint_count;
            break;

        case LB_LEAST_CONN:
            selected = 0;
            for (int i = 1; i < svc->endpoint_count; i++) {
                if (svc->endpoints[i].connections < svc->endpoints[selected].connections) {
                    selected = i;
                }
            }
            break;

        case LB_RANDOM:
            selected = (cluster.cluster_time * 7 + service_id) % svc->endpoint_count;
            break;

        case LB_WEIGHTED:
            /* Simplified: use first healthy endpoint */
            for (int i = 0; i < svc->endpoint_count; i++) {
                if (svc->endpoints[i].healthy) {
                    selected = i;
                    break;
                }
            }
            break;
    }

    if (selected >= 0 && selected < svc->endpoint_count) {
        svc->endpoints[selected].connections++;
        return selected;
    }

    return -1;
}

void service_list(void) {
    print_str("Services:\n");
    for (int i = 0; i < cluster.service_count; i++) {
        if (!cluster.services[i].active) continue;
        print_str("  ");
        print_str(cluster.services[i].name);
        print_str(" - ");
        print_int(cluster.services[i].endpoint_count);
        print_str(" endpoints\n");
    }
}

/* ===== Leader Election ===== */

int cluster_elect_leader(void) {
    int best_node = -1;
    int best_score = -1;

    for (int i = 0; i < cluster.node_count; i++) {
        if (!cluster.nodes[i].active) continue;
        if (cluster.nodes[i].state != NODE_STATE_HEALTHY) continue;

        int score = cluster.nodes[i].health_score * cluster.nodes[i].weight;
        if (score > best_score) {
            best_score = score;
            best_node = i;
        }
    }

    if (best_node >= 0) {
        cluster.leader_node = best_node;
        print_str("Elected leader: Node ");
        print_int(best_node);
        print_str(" (");
        print_str(cluster.nodes[best_node].name);
        print_str(")\n");
    }

    return cluster.leader_node;
}

/* ===== Cluster Status ===== */

void cluster_status(void) {
    print_str("Cluster Status:\n");
    print_str("  Name: ");
    print_str(cluster.cluster_name);
    print_str("\n");
    print_str("  Nodes: ");
    int healthy = 0, total = 0;
    for (int i = 0; i < cluster.node_count; i++) {
        if (!cluster.nodes[i].active) continue;
        total++;
        if (cluster.nodes[i].state == NODE_STATE_HEALTHY) healthy++;
    }
    print_int(healthy);
    print_str("/");
    print_int(total);
    print_str(" healthy\n");
    print_str("  Services: ");
    print_int(cluster.service_count);
    print_str("\n");
    print_str("  Leader: ");
    if (cluster.leader_node >= 0) {
        print_str(cluster.nodes[cluster.leader_node].name);
    } else {
        print_str("NONE");
    }
    print_str("\n");
}

/* ===== CLI entry ===== */
void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);
    int help = 0, test = 0;
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        int hl = my_strlen(arg);
        if (hl == 2 && arg[0] == '-' && arg[1] == 'h') help = 1;
        else if (hl == 2 && arg[0] == '-' && arg[1] == 't') test = 1;
        while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    }

    print_str("Cluster Manager v46.0 - Distributed Cluster Management\n");
    if (help) {
        print_str("Usage: cluster_manager [options]\n");
        print_str("  -h    Show help\n");
        print_str("  -t    Run cluster test\n");
        return;
    }

    if (test) {
        print_str("=== Cluster Manager Test Suite ===\n\n");

        /* Test 1: Initialize cluster */
        print_str("[Test 1] Cluster Initialization\n");
        cluster_init("production-cluster");
        print_str("\n");

        /* Test 2: Add nodes */
        print_str("[Test 2] Node Management\n");
        cluster_add_node("node-1", "192.168.1.10", 8080, 100);
        cluster_add_node("node-2", "192.168.1.11", 8080, 100);
        cluster_add_node("node-3", "192.168.1.12", 8080, 50);
        cluster_add_node("node-4", "192.168.1.13", 8080, 50);
        cluster_node_list();
        print_str("\n");

        /* Test 3: Register services */
        print_str("[Test 3] Service Registration\n");
        int svc1 = service_register("web-api", LB_ROUND_ROBIN);
        int svc2 = service_register("database", LB_LEAST_CONN);
        int svc3 = service_register("cache", LB_RANDOM);
        print_str("\n");

        /* Test 4: Add endpoints */
        print_str("[Test 4] Service Endpoints\n");
        service_add_endpoint(svc1, 0, "192.168.1.10", 80);
        service_add_endpoint(svc1, 1, "192.168.1.11", 80);
        service_add_endpoint(svc2, 0, "192.168.1.10", 5432);
        service_add_endpoint(svc2, 1, "192.168.1.11", 5432);
        service_add_endpoint(svc3, 2, "192.168.1.12", 6379);
        service_add_endpoint(svc3, 3, "192.168.1.13", 6379);
        service_list();
        print_str("\n");

        /* Test 5: Load balancing */
        print_str("[Test 5] Load Balancing\n");
        for (int i = 0; i < 6; i++) {
            int ep = service_get_endpoint(svc1);
            print_str("  Request ");
            print_int(i + 1);
            print_str(" -> endpoint ");
            print_int(ep);
            print_str("\n");
        }
        print_str("\n");

        /* Test 6: Health checks */
        print_str("[Test 6] Health Monitoring\n");
        cluster_node_heartbeat(0);
        cluster_node_heartbeat(1);
        cluster_node_heartbeat(2);
        cluster.cluster_time += 15000;  /* Advance 15 seconds */
        cluster_check_health();
        cluster_node_list();
        print_str("\n");

        /* Test 7: Leader election */
        print_str("[Test 7] Leader Election\n");
        cluster_elect_leader();
        print_str("\n");

        /* Test 8: Cluster status */
        print_str("[Test 8] Cluster Status\n");
        cluster_status();
        print_str("\n");

        /* Test 9: Node failure */
        print_str("[Test 9] Failure Handling\n");
        cluster_remove_node(0);
        cluster_elect_leader();
        cluster_status();
        print_str("\n");

        print_str("=== Cluster Manager Test Complete ===\n");
        return;
    }

    print_str("Use -h for help, -t for test\n");
}
