/* agent_cluster: Distributed Agent cluster communication (v11.0) */

#include <stddef.h>

/* Host functions (provided by WASM runtime) */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern void host_http_request(const char *method, const char *url, const char *body, char *response, int max_len);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Cluster limits */
#define MAX_NODES 16
#define MAX_TASKS 64
#define MAX_NODE_NAME 32
#define MAX_PAYLOAD 1024

/* Node states */
#define NODE_ONLINE   1
#define NODE_OFFLINE  0
#define NODE_BUSY     2

/* Task states */
#define TASK_PENDING    0
#define TASK_DISPATCHED 1
#define TASK_RUNNING    2
#define TASK_COMPLETED  3
#define TASK_FAILED     4

/* Cluster node entry */
typedef struct {
    int     node_id;
    char    ip_addr[16];
    int     port;
    int     status;
    int     load;
    int     agent_count;
    char    name[MAX_NODE_NAME];
    int     last_heartbeat;
} cluster_node_t;

/* Remote task entry */
typedef struct {
    int     task_id;
    int     source_node;
    int     target_node;
    int     status;
    char    payload[MAX_PAYLOAD];
    char    result[MAX_PAYLOAD];
    int     created_at;
    int     completed_at;
} cluster_task_t;

/* Cluster state */
typedef struct {
    int            local_node_id;
    char           local_ip[16];
    int            port;
    cluster_node_t nodes[MAX_NODES];
    int            node_count;
    cluster_task_t tasks[MAX_TASKS];
    int            task_count;
    int            total_dispatched;
    int            total_completed;
    int            total_failed;
} cluster_state_t;

static cluster_state_t cluster;
static int cluster_initialized = 0;
static int next_task_id = 1;
static int next_node_id = 1;

/* String utilities */
static int my_strlen(const char *s) {
    int len = 0;
    while (s[len]) len++;
    return len;
}

static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) {
        if (*a != *b) return *a - *b;
        a++; b++;
    }
    return *a - *b;
}

static void my_strcpy(char *dst, const char *src) {
    while (*src) *dst++ = *src++;
    *dst = '\0';
}

static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0;
    while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; }
    dst[i] = '\0';
}

/* JSON helpers */
static void json_write_int(char *buf, const char *key, int val) {
    int pos = 0;
    buf[pos++] = '"';
    int i = 0;
    while (key[i]) buf[pos++] = key[i++];
    buf[pos++] = '"';
    buf[pos++] = ':';
    /* simple int to string */
    if (val == 0) { buf[pos++] = '0'; }
    else {
        int tmp = val;
        int start = pos;
        if (tmp < 0) { buf[pos++] = '-'; tmp = -tmp; }
        int digits = 0;
        int t2 = tmp;
        while (t2 > 0) { digits++; t2 /= 10; }
        pos += digits;
        buf[pos] = '\0';
        int p2 = pos - 1;
        while (t2 > 0) { buf[p2--] = '0' + (tmp % 10); tmp /= 10; }
    }
}

/* Print utilities */
static void print_str(const char *str) { host_print(str); }
static void print_int(int val) {
    char buf[32];
    int pos = 0;
    if (val < 0) { buf[pos++] = '-'; val = -val; }
    if (val == 0) buf[pos++] = '0';
    else {
        int digits = 0, t = val;
        while (t > 0) { digits++; t /= 10; }
        pos += digits;
        buf[pos] = '\0';
        pos--;
        while (val > 0) { buf[pos--] = '0' + (val % 10); val /= 10; }
    }
    host_print(buf);
}

/* ===== Node management ===== */

/* Register a new cluster node
   ip: node IP address, port: node port
   Returns: node_id or -1 */
int cluster_register_node(const char *ip, int port) {
    if (!cluster_initialized) {
        for (int i = 0; i < MAX_NODES; i++) cluster.nodes[i].status = NODE_OFFLINE;
        for (int i = 0; i < MAX_TASKS; i++) cluster.tasks[i].status = TASK_PENDING;
        cluster.node_count = 0;
        cluster.task_count = 0;
        cluster.total_dispatched = 0;
        cluster.total_completed = 0;
        cluster.total_failed = 0;
        cluster_initialized = 1;
    }

    if (cluster.node_count >= MAX_NODES) {
        print_str("[cluster] node limit reached\n");
        return -1;
    }

    /* Check duplicate */
    for (int i = 0; i < cluster.node_count; i++) {
        if (cluster.nodes[i].port == port &&
            my_strcmp(cluster.nodes[i].ip_addr, ip) == 0) {
            print_str("[cluster] node already registered\n");
            return cluster.nodes[i].node_id;
        }
    }

    int idx = cluster.node_count;
    cluster.nodes[idx].node_id = next_node_id;
    my_strncpy(cluster.nodes[idx].ip_addr, ip, 15);
    cluster.nodes[idx].port = port;
    cluster.nodes[idx].status = NODE_ONLINE;
    cluster.nodes[idx].load = 0;
    cluster.nodes[idx].agent_count = 0;
    cluster.nodes[idx].last_heartbeat = 0;
    my_strcpy(cluster.nodes[idx].name, "node-");

    /* Append ID to name */
    int nid = next_node_id;
    int base = 6; /* "node-" length */
    if (nid == 0) { cluster.nodes[idx].name[base] = '0'; base++; }
    else {
        int d = 0, t = nid;
        while (t > 0) { d++; t /= 10; }
        int p = base + d;
        cluster.nodes[idx].name[p] = '\0';
        while (nid > 0) { cluster.nodes[idx].name[--p] = '0' + (nid % 10); nid /= 10; }
    }

    cluster.node_count++;
    int id = next_node_id++;

    print_str("[cluster] registered node: ");
    print_int(id);
    print_str(" at ");
    print_str(ip);
    print_str(":");
    print_int(port);
    print_str("\n");

    return id;
}

/* Dispatch a task to a target node
   task_id: local task ID, target_node: destination node ID
   Returns: 0=dispatched, -1=failure */
int cluster_dispatch_task(int task_id, int target_node) {
    if (cluster.task_count >= MAX_TASKS) return -1;

    /* Find target node */
    int node_idx = -1;
    for (int i = 0; i < cluster.node_count; i++) {
        if (cluster.nodes[i].node_id == target_node) {
            node_idx = i;
            break;
        }
    }
    if (node_idx < 0 || cluster.nodes[node_idx].status != NODE_ONLINE) {
        print_str("[cluster] target node not available\n");
        return -1;
    }

    /* Find source task */
    int task_idx = -1;
    for (int i = 0; i < cluster.task_count; i++) {
        if (cluster.tasks[i].task_id == task_id) {
            task_idx = i;
            break;
        }
    }

    int idx = cluster.task_count;
    cluster.tasks[idx].task_id = next_task_id++;
    cluster.tasks[idx].source_node = cluster.local_node_id;
    cluster.tasks[idx].target_node = target_node;
    cluster.tasks[idx].status = TASK_DISPATCHED;

    if (task_idx >= 0) {
        my_strncpy(cluster.tasks[idx].payload,
                    cluster.tasks[task_idx].payload, MAX_PAYLOAD - 1);
        cluster.tasks[task_idx].status = TASK_DISPATCHED;
    }

    cluster.tasks[idx].created_at = 0;
    cluster.tasks[idx].completed_at = 0;

    /* Build HTTP request to remote node */
    char req_buf[512];
    int pos = 0;
    req_buf[pos++] = '{';
    req_buf[pos++] = '"'; req_buf[pos++] = 't'; req_buf[pos++] = 'a';
    req_buf[pos++] = 's'; req_buf[pos++] = 'k'; req_buf[pos++] = '"';
    req_buf[pos++] = ':';

    int tid = task_id;
    if (tid == 0) req_buf[pos++] = '0';
    else {
        int d = 0, t = tid;
        while (t > 0) { d++; t /= 10; }
        pos += d;
        req_buf[pos] = '\0';
        int p2 = pos - 1;
        while (tid > 0) { req_buf[p2--] = '0' + (tid % 10); tid /= 10; }
    }
    req_buf[pos++] = ',';
    req_buf[pos++] = '"'; req_buf[pos++] = 's'; req_buf[pos++] = 'r';
    req_buf[pos++] = 'c'; req_buf[pos++] = '"'; req_buf[pos++] = ':';
    int src = cluster.local_node_id;
    if (src == 0) req_buf[pos++] = '0';
    else {
        int d = 0, t = src;
        while (t > 0) { d++; t /= 10; }
        pos += d;
        req_buf[pos] = '\0';
        int p2 = pos - 1;
        while (src > 0) { req_buf[p2--] = '0' + (src % 10); src /= 10; }
    }
    req_buf[pos++] = '}';
    req_buf[pos] = '\0';

    /* Build URL */
    char url[64];
    int up = 0;
    url[up++] = 'h'; url[up++] = 't'; url[up++] = 't'; url[up++] = 'p';
    url[up++] = ':'; url[up++] = '/'; url[up++] = '/';
    for (int i = 0; cluster.nodes[node_idx].ip_addr[i]; i++)
        url[up++] = cluster.nodes[node_idx].ip_addr[i];
    url[up++] = ':';
    int p = cluster.nodes[node_idx].port;
    if (p == 0) url[up++] = '0';
    else {
        int d = 0, t = p;
        char tmp[8];
        while (t > 0) { tmp[d++] = '0' + (t % 10); t /= 10; }
        while (d > 0) url[up++] = tmp[--d];
    }
    url[up++] = '/'; url[up++] = 'd'; url[up++] = 'i'; url[up++] = 's';
    url[up++] = 'p'; url[up++] = 'a'; url[up++] = 't'; url[up++] = 'c';
    url[up++] = 'h'; url[up++] = '\0';

    char resp_buf[512];
    host_http_request("POST", url, req_buf, resp_buf, 512);

    cluster.total_dispatched++;
    cluster.task_count++;

    cluster.nodes[node_idx].load += 10;

    print_str("[cluster] dispatched task ");
    print_int(task_id);
    print_str(" -> node ");
    print_int(target_node);
    print_str("\n");

    return 0;
}

/* Collect results from a dispatched task
   task_id: cluster task ID
   Returns: 0=collected, -1=failure */
int cluster_collect_results(int task_id) {
    for (int i = 0; i < cluster.task_count; i++) {
        if (cluster.tasks[i].task_id == task_id) {
            if (cluster.tasks[i].status == TASK_COMPLETED) {
                print_str("[cluster] result collected for task ");
                print_int(task_id);
                print_str(": ");
                print_str(cluster.tasks[i].result);
                print_str("\n");
                return 0;
            }

            /* Poll remote node for result */
            int node_idx = -1;
            for (int j = 0; j < cluster.node_count; j++) {
                if (cluster.nodes[j].node_id == cluster.tasks[i].target_node) {
                    node_idx = j;
                    break;
                }
            }
            if (node_idx < 0) return -1;

            char url[64];
            int up = 0;
            url[up++] = 'h'; url[up++] = 't'; url[up++] = 't'; url[up++] = 'p';
            url[up++] = ':'; url[up++] = '/'; url[up++] = '/';
            for (int k = 0; cluster.nodes[node_idx].ip_addr[k]; k++)
                url[up++] = cluster.nodes[node_idx].ip_addr[k];
            url[up++] = ':';
            int pp = cluster.nodes[node_idx].port;
            if (pp == 0) url[up++] = '0';
            else {
                int d = 0, t = pp;
                char tmp[8];
                while (t > 0) { tmp[d++] = '0' + (t % 10); t /= 10; }
                while (d > 0) url[up++] = tmp[--d];
            }
            url[up++] = '/'; url[up++] = 'r'; url[up++] = 'e'; url[up++] = 's';
            url[up++] = 'u'; url[up++] = 'l'; url[up++] = 't'; url[up++] = '/';
            int tid = task_id;
            if (tid == 0) url[up++] = '0';
            else {
                int d = 0, t = tid;
                char tmp[16];
                while (t > 0) { tmp[d++] = '0' + (t % 10); t /= 10; }
                while (d > 0) url[up++] = tmp[--d];
            }
            url[up] = '\0';

            char resp[512];
            host_http_request("GET", url, "", resp, 512);

            if (resp[0] && resp[0] != '{') {
                cluster.tasks[i].status = TASK_COMPLETED;
                my_strncpy(cluster.tasks[i].result, resp, MAX_PAYLOAD - 1);
                cluster.total_completed++;
                cluster.nodes[node_idx].load -= 10;
                return 0;
            }

            print_str("[cluster] task ");
            print_int(task_id);
            print_str(" not yet complete\n");
            return -1;
        }
    }

    print_str("[cluster] task not found\n");
    return -1;
}

/* Check health of all cluster nodes
   Returns: number of healthy nodes */
int cluster_health_check(void) {
    if (!cluster_initialized) return 0;

    int healthy = 0;
    for (int i = 0; i < cluster.node_count; i++) {
        if (cluster.nodes[i].status == NODE_OFFLINE) continue;

        /* Build health check URL */
        char url[64];
        int up = 0;
        url[up++] = 'h'; url[up++] = 't'; url[up++] = 't'; url[up++] = 'p';
        url[up++] = ':'; url[up++] = '/'; url[up++] = '/';
        for (int j = 0; cluster.nodes[i].ip_addr[j]; j++)
            url[up++] = cluster.nodes[i].ip_addr[j];
        url[up++] = ':';
        int p = cluster.nodes[i].port;
        if (p == 0) url[up++] = '0';
        else {
            int d = 0, t = p;
            char tmp[8];
            while (t > 0) { tmp[d++] = '0' + (t % 10); t /= 10; }
            while (d > 0) url[up++] = tmp[--d];
        }
        url[up++] = '/'; url[up++] = 'h'; url[up++] = 'e'; url[up++] = 'a';
        url[up++] = 'l'; url[up++] = 't'; url[up++] = 'h'; url[up++] = '\0';

        char resp[128];
        host_http_request("GET", url, "", resp, 128);

        if (resp[0] && resp[0] == '{') {
            cluster.nodes[i].status = NODE_ONLINE;
            cluster.nodes[i].last_heartbeat++;
            healthy++;
        } else {
            if (cluster.nodes[i].last_heartbeat > 3) {
                cluster.nodes[i].status = NODE_OFFLINE;
                print_str("[cluster] node ");
                print_int(cluster.nodes[i].node_id);
                print_str(" marked OFFLINE\n");
            } else {
                cluster.nodes[i].last_heartbeat++;
                healthy++;
            }
        }
    }

    print_str("[cluster] health check: ");
    print_int(healthy);
    print_str("/");
    print_int(cluster.node_count);
    print_str(" nodes online\n");

    return healthy;
}

/* Rebalance load across cluster nodes
   Returns: 0=success, -1=no rebalance needed */
int cluster_rebalance(void) {
    if (!cluster_initialized) return -1;

    /* Find most and least loaded nodes */
    int max_load = -1, min_load = 999;
    int max_idx = -1, min_idx = -1;

    for (int i = 0; i < cluster.node_count; i++) {
        if (cluster.nodes[i].status != NODE_ONLINE) continue;
        if (cluster.nodes[i].load > max_load) {
            max_load = cluster.nodes[i].load;
            max_idx = i;
        }
        if (cluster.nodes[i].load < min_load) {
            min_load = cluster.nodes[i].load;
            min_idx = i;
        }
    }

    if (max_idx < 0 || min_idx < 0) return -1;
    if (max_load - min_load < 30) {
        print_str("[cluster] no rebalance needed\n");
        return -1;
    }

    print_str("[cluster] rebalancing: moving load from node ");
    print_int(cluster.nodes[max_idx].node_id);
    print_str(" (load=");
    print_int(max_load);
    print_str(") to node ");
    print_int(cluster.nodes[min_idx].node_id);
    print_str(" (load=");
    print_int(min_load);
    print_str(")\n");

    /* Transfer one task from max to min */
    cluster.nodes[max_idx].load -= 10;
    cluster.nodes[min_idx].load += 10;

    return 0;
}

/* Set local node info */
void cluster_set_local(const char *ip, int port, int node_id) {
    cluster.local_node_id = node_id;
    my_strncpy(cluster.local_ip, ip, 15);
    cluster.port = port;
}

/* Get cluster status summary */
void cluster_status(void) {
    print_str("=== Cluster Status ===\n");
    print_str("Local node: ");
    print_int(cluster.local_node_id);
    print_str("\nNodes: ");
    print_int(cluster.node_count);
    print_str("/");
    print_int(MAX_NODES);
    print_str("  Tasks: ");
    print_int(cluster.task_count);
    print_str("/");
    print_int(MAX_TASKS);
    print_str("\nDispatched: ");
    print_int(cluster.total_dispatched);
    print_str("  Completed: ");
    print_int(cluster.total_completed);
    print_str("  Failed: ");
    print_int(cluster.total_failed);
    print_str("\n");

    for (int i = 0; i < cluster.node_count; i++) {
        print_str("  Node ");
        print_int(cluster.nodes[i].node_id);
        print_str(" ");
        print_str(cluster.nodes[i].ip_addr);
        print_str(":");
        print_int(cluster.nodes[i].port);
        print_str(" [");
        print_str(cluster.nodes[i].status == NODE_ONLINE ? "ONLINE" :
                  cluster.nodes[i].status == NODE_BUSY ? "BUSY" : "OFFLINE");
        print_str("] load=");
        print_int(cluster.nodes[i].load);
        print_str(" agents=");
        print_int(cluster.nodes[i].agent_count);
        print_str("\n");
    }
}

/* Command-line entry */
void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);

    int help = 0, info = 0, test = 0, status = 0;
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;
    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0) help = 1;
        else if (my_strcmp(arg, "-i") == 0 || my_strcmp(arg, "--info") == 0) info = 1;
        else if (my_strcmp(arg, "-t") == 0 || my_strcmp(arg, "--test") == 0) test = 1;
        else if (my_strcmp(arg, "-s") == 0 || my_strcmp(arg, "--status") == 0) status = 1;
        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    print_str("Agent Cluster v11.0 - Distributed Coordination\n");

    if (help) {
        print_str("Usage: agent_cluster [options]\n");
        print_str("  -h, --help          Show this help\n");
        print_str("  -i, --info          Show cluster info\n");
        print_str("  -t, --test          Run cluster test\n");
        print_str("  -s, --status        Show cluster status\n");
        return;
    }
    if (info) {
        print_str("Agent Cluster v11.0\n");
        print_str("Max nodes: "); print_int(MAX_NODES); print_str("\n");
        print_str("Max tasks: "); print_int(MAX_TASKS); print_str("\n");
        print_str("Payload: "); print_int(MAX_PAYLOAD); print_str(" bytes\n");
        return;
    }
    if (test) {
        print_str("=== Cluster Test ===\n");
        cluster_set_local("192.168.1.100", 8080, 1);
        print_str("\n--- Register Nodes ---\n");
        int n1 = cluster_register_node("192.168.1.101", 8080);
        int n2 = cluster_register_node("192.168.1.102", 8080);
        int n3 = cluster_register_node("192.168.1.103", 8080);
        print_str("\n--- Health Check ---\n");
        int healthy = cluster_health_check();
        print_str("\n--- Load Test ---\n");
        cluster.nodes[0].load = 60;
        cluster.nodes[1].load = 20;
        int rc = cluster_rebalance();
        print_str("\n--- Final Status ---\n");
        cluster_status();
        return;
    }
    if (status) {
        cluster_status();
        return;
    }
    print_str("Use -h for help, -t for test, -s for status\n");
}
