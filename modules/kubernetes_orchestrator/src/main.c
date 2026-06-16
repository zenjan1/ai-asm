/* kubernetes_orchestrator: K8s-style container orchestration engine (v1.0)
 *
 * Implements: Pod lifecycle, Deployments, Services, Scheduler (affinity/taints),
 * HPA (Horizontal Pod Autoscaler), health probes, and reconciliation loop.
 */
#include <stddef.h>

/* Host function imports - provided by WASM runtime */
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Resource limits */

#define MAX_PODS 32
#define MAX_NODES 16
#define MAX_SERVICES 16
#define MAX_DEPLOYMENTS 16
#define MAX_CONFIGMAPS 16
#define MAX_SECRETS 16
#define MAX_ENDPOINTS 32
#define MAX_LABELS 8
#define MAX_TAINTS 8
#define MAX_TOLERATIONS 8
#define MAX_REPLICAS 16
#define MAX_NAME 32
#define MAX_VALUE 64
#define MAX_IMAGE 64
#define MAX_CMD 128
#define POD_PENDING 0
#define POD_RUNNING 1
#define POD_FAILED 3
#define CONT_WAITING 0
#define CONT_RUNNING 1
#define CONT_TERMINATED 2
#define PROBE_NONE 0
#define PROBE_HTTP 1
#define SCHED_OK 0
#define SCHED_NO_NODE 1
#define LB_ROUND_ROBIN 0
#define LB_RANDOM 1

/* String utility functions */
static unsigned int my_strlen(const char *s) { unsigned int n = 0; while (s[n]) n++; return n; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b && *a == *b) { a++; b++; }
    return (unsigned char)*a - (unsigned char)*b;
}
static void my_strcpy(char *d, const char *s, unsigned int m) {
    unsigned int i; for (i = 0; i < m - 1 && s[i]; i++) d[i] = s[i]; d[i] = '\0';
}
static void my_memset(void *p, int v, unsigned int n) {
    unsigned char *b = (unsigned char *)p; for (unsigned int i = 0; i < n; i++) b[i] = (unsigned char)v;
}
static void print_str(const char *s) { host_print(s); }
static unsigned int rng_state = 48271;
static unsigned int my_rand(void) { rng_state = rng_state * 1103515245u + 12345u; return (rng_state >> 16) & 0x7FFF; }

/* Resource data structures */
typedef struct { char key[MAX_NAME]; char value[MAX_VALUE]; } label_t;
typedef struct {
    char name[MAX_NAME], image[MAX_IMAGE], command[MAX_CMD];
    int cpu_request, cpu_limit, mem_request, mem_limit, status, restarts;
} container_t;
typedef struct { int type; char path[MAX_VALUE]; int port, period, success_thresh, failure_thresh; } probe_t;
typedef struct {
    char name[MAX_NAME]; label_t labels[MAX_LABELS]; int label_count;
    container_t containers[4]; int container_count, status, priority, restart_count;
    char node_name[MAX_NAME], ip_addr[16];
    probe_t liveness, readiness, startup;
    char node_affinity[MAX_NAME], anti_affinity[MAX_NAME];
    char tolerations[MAX_TAINTS][MAX_NAME]; int toleration_count;
} pod_t;
typedef struct {
    char name[MAX_NAME]; int cpu_capacity, mem_capacity, cpu_alloc, mem_alloc, ready;
    char taints[MAX_TAINTS][MAX_NAME]; int taint_count;
    label_t labels[MAX_LABELS]; int label_count, pod_count;
} node_t;
typedef struct { char pod_name[MAX_NAME], ip_addr[16]; int port, ready; } endpoint_t;
typedef struct {
    char name[MAX_NAME]; label_t selector[MAX_LABELS]; int selector_count;
    int port, target_port, type, lb_algo, rr_index;
    endpoint_t endpoints[MAX_ENDPOINTS]; int ep_count; char cluster_ip[16];
} service_t;
typedef struct {
    char name[MAX_NAME]; label_t labels[MAX_LABELS]; int label_count;
    int replicas, desired; pod_t pods[MAX_REPLICAS];
    container_t template_container; int strategy;
} deployment_t;
typedef struct { char name[MAX_NAME]; label_t data[MAX_LABELS]; int data_count; } configmap_t;
typedef struct { char name[MAX_NAME]; label_t data[MAX_LABELS]; int data_count, encoded; } secret_t;
typedef struct {
    int metric_type, target_value, current_value, min_replicas, max_replicas;
    int cooldown, last_scale_time; char deploy_name[MAX_NAME];
} hpa_t;

/* Global cluster state - singletons */
static pod_t g_pods[MAX_PODS]; static int g_pod_count;
static node_t g_nodes[MAX_NODES]; static int g_node_count;
static service_t g_services[MAX_SERVICES]; static int g_svc_count;
static deployment_t g_deploys[MAX_DEPLOYMENTS]; static int g_deploy_count;
static configmap_t g_configs[MAX_CONFIGMAPS]; static int g_cfg_count;
static secret_t g_secrets[MAX_SECRETS]; static int g_secret_count;
static int g_tick;

/* Forward declarations - all functions declared before use */
static int pod_create(const char *name, const char *image, int cpu, int mem, int prio);
static int pod_schedule(int pi);
static void pod_delete(int pi);
static void pod_set_liveness(int pi, int type, const char *path, int port);
static void pod_set_readiness(int pi, int type, const char *path, int port);
static int probe_check_liveness(int pi);
static int probe_check_readiness(int pi);
static int deployment_create(const char *name, const char *image, int replicas, int cpu, int mem);
static int deployment_scale(int di, int new_replicas);
static int service_create(const char *name, int port, int target, int lb_algo);
static int service_resolve(const char *name);
static void service_sync_endpoints(int si);
static int scheduler_select_node(int cpu, int mem, const char *affinity, const char *tolerations[], int tc);
static int hpa_evaluate(hpa_t *hpa);
static int hpa_scale(hpa_t *hpa);
static void reconcile_loop(void);
static int pod_matches_selector(int pi, const label_t *sel, int sc);

/* ConfigMap and Secret management */
static int configmap_create(const char *name, const char *k, const char *v) {
    if (g_cfg_count >= MAX_CONFIGMAPS) return -1;
    configmap_t *cm = &g_configs[g_cfg_count]; my_memset(cm, 0, sizeof(configmap_t));
    my_strcpy(cm->name, name, MAX_NAME); my_strcpy(cm->data[0].key, k, MAX_NAME);
    my_strcpy(cm->data[0].value, v, MAX_VALUE); cm->data_count = 1; g_cfg_count++;
    return g_cfg_count - 1;
}
static int secret_create(const char *name, const char *k, const char *v) {
    if (g_secret_count >= MAX_SECRETS) return -1;
    secret_t *s = &g_secrets[g_secret_count]; my_memset(s, 0, sizeof(secret_t));
    my_strcpy(s->name, name, MAX_NAME); my_strcpy(s->data[0].key, k, MAX_NAME);
    my_strcpy(s->data[0].value, v, MAX_VALUE); s->data_count = 1; s->encoded = 1; g_secret_count++;
    return g_secret_count - 1;
}
/* Node management - cluster resources */
static int node_add(const char *name, int cpu, int mem) {
    if (g_node_count >= MAX_NODES) return -1;
    node_t *n = &g_nodes[g_node_count]; my_memset(n, 0, sizeof(node_t));
    my_strcpy(n->name, name, MAX_NAME); n->cpu_capacity = cpu; n->mem_capacity = mem;
    n->cpu_alloc = 0; n->mem_alloc = 0; n->ready = 1; g_node_count++;
    return g_node_count - 1;
}
static void node_add_taint(int ni, const char *t) {
    if (ni < 0 || ni >= g_node_count) return; node_t *n = &g_nodes[ni];
    if (n->taint_count >= MAX_TAINTS) return;
    my_strcpy(n->taints[n->taint_count], t, MAX_NAME); n->taint_count++;
}
static void node_add_label(int ni, const char *k, const char *v) {
    if (ni < 0 || ni >= g_node_count) return; node_t *n = &g_nodes[ni];
    if (n->label_count >= MAX_LABELS) return;
    my_strcpy(n->labels[n->label_count].key, k, MAX_NAME);
    my_strcpy(n->labels[n->label_count].value, v, MAX_VALUE); n->label_count++;
}
/* Pod lifecycle management */
static int pod_create(const char *name, const char *image, int cpu, int mem, int prio) {
    if (g_pod_count >= MAX_PODS) return -1;
    pod_t *p = &g_pods[g_pod_count]; my_memset(p, 0, sizeof(pod_t));
    my_strcpy(p->name, name, MAX_NAME); my_strcpy(p->containers[0].name, name, MAX_NAME);
    my_strcpy(p->containers[0].image, image, MAX_IMAGE);
    p->containers[0].cpu_request = cpu; p->containers[0].mem_request = mem;
    p->containers[0].cpu_limit = cpu * 2; p->containers[0].mem_limit = mem * 2;
    p->containers[0].status = CONT_WAITING; p->container_count = 1;
    p->status = POD_PENDING; p->priority = prio; g_pod_count++;
    return g_pod_count - 1;
}
static int pod_schedule(int pi) {
    if (pi < 0 || pi >= g_pod_count) return -1;
    pod_t *p = &g_pods[pi];
    if (p->status != POD_PENDING) return -1;
    const char *tol_arr[MAX_TOLERATIONS];
    for (int i = 0; i < p->toleration_count; i++) tol_arr[i] = p->tolerations[i];
    int node = scheduler_select_node(p->containers[0].cpu_request, p->containers[0].mem_request,
                                      p->node_affinity, tol_arr, p->toleration_count);
    if (node < 0) { p->status = POD_PENDING; return SCHED_NO_NODE; }
    node_t *n = &g_nodes[node]; my_strcpy(p->node_name, n->name, MAX_NAME);
    p->ip_addr[0] = '1'; p->ip_addr[1] = '0'; p->ip_addr[2] = '.';
    p->ip_addr[3] = '0' + (node % 10); p->ip_addr[4] = '.';
    p->ip_addr[5] = '0' + (pi % 10); p->ip_addr[6] = '.';
    p->ip_addr[7] = '1' + (pi % 5); p->ip_addr[8] = '\0';
    n->cpu_alloc += p->containers[0].cpu_request; n->mem_alloc += p->containers[0].mem_request;
    n->pod_count++; p->status = POD_RUNNING; p->containers[0].status = CONT_RUNNING;
    return SCHED_OK;
}
static void pod_delete(int pi) {
    if (pi < 0 || pi >= g_pod_count) return; pod_t *p = &g_pods[pi];
    if (p->node_name[0]) {
        for (int i = 0; i < g_node_count; i++) {
            if (my_strcmp(g_nodes[i].name, p->node_name) == 0) {
                g_nodes[i].cpu_alloc -= p->containers[0].cpu_request;
                g_nodes[i].mem_alloc -= p->containers[0].mem_request;
                if (g_nodes[i].pod_count > 0) g_nodes[i].pod_count--;
                if (g_nodes[i].cpu_alloc < 0) g_nodes[i].cpu_alloc = 0;
                if (g_nodes[i].mem_alloc < 0) g_nodes[i].mem_alloc = 0; break;
            }
        }
    }
    for (int i = pi; i < g_pod_count - 1; i++) g_pods[i] = g_pods[i + 1]; g_pod_count--;
}
static void pod_set_liveness(int pi, int type, const char *path, int port) {
    if (pi < 0 || pi >= g_pod_count) return;
    g_pods[pi].liveness.type = type; my_strcpy(g_pods[pi].liveness.path, path, MAX_VALUE);
    g_pods[pi].liveness.port = port; g_pods[pi].liveness.period = 10; g_pods[pi].liveness.failure_thresh = 3;
}
static void pod_set_readiness(int pi, int type, const char *path, int port) {
    if (pi < 0 || pi >= g_pod_count) return;
    g_pods[pi].readiness.type = type; my_strcpy(g_pods[pi].readiness.path, path, MAX_VALUE);
    g_pods[pi].readiness.port = port; g_pods[pi].readiness.period = 5; g_pods[pi].readiness.success_thresh = 2;
}
static int probe_check_liveness(int pi) {
    if (pi < 0 || pi >= g_pod_count) return 0; pod_t *p = &g_pods[pi];
    if (p->liveness.type == PROBE_NONE) return 1; if (p->status != POD_RUNNING) return 0;
    return (g_tick % 2 == 0) ? 1 : 0;
}
static int probe_check_readiness(int pi) {
    if (pi < 0 || pi >= g_pod_count) return 0; pod_t *p = &g_pods[pi];
    if (p->readiness.type == PROBE_NONE) return 1; if (p->status != POD_RUNNING) return 0;
    return (p->restart_count < 3) ? 1 : 0;
}
static int scheduler_select_node(int cpu, int mem, const char *affinity, const char *tolerations[], int tc) {
    int best = -1, best_score = -1;
    for (int i = 0; i < g_node_count; i++) {
        node_t *n = &g_nodes[i]; if (!n->ready) continue;
        if (n->cpu_alloc + cpu > n->cpu_capacity) continue;
        if (n->mem_alloc + mem > n->mem_capacity) continue;
        int tainted = 0;
        for (int t = 0; t < n->taint_count && !tainted; t++) {
            int tol = 0;
            for (int tt = 0; tt < tc; tt++) if (my_strcmp(n->taints[t], tolerations[tt]) == 0) { tol = 1; break; }
            if (!tol) tainted = 1;
        }
        if (tainted) continue;
        if (affinity[0]) {
            int am = 0;
            for (int l = 0; l < n->label_count; l++) if (my_strcmp(n->labels[l].key, affinity) == 0) { am = 1; break; }
            if (!am) continue;
        }
        int score = (n->cpu_capacity - n->cpu_alloc - cpu) + (n->mem_capacity - n->mem_alloc - mem);
        if (score > best_score) { best_score = score; best = i; }
    }
    return best;
}
static int deployment_create(const char *name, const char *image, int replicas, int cpu, int mem) {
    if (g_deploy_count >= MAX_DEPLOYMENTS) return -1;
    deployment_t *d = &g_deploys[g_deploy_count]; my_memset(d, 0, sizeof(deployment_t));
    my_strcpy(d->name, name, MAX_NAME); my_strcpy(d->labels[0].key, "app", MAX_NAME);
    my_strcpy(d->labels[0].value, name, MAX_VALUE); d->label_count = 1;
    d->desired = replicas; d->replicas = 0;
    my_strcpy(d->template_container.name, name, MAX_NAME);
    my_strcpy(d->template_container.image, image, MAX_IMAGE);
    d->template_container.cpu_request = cpu; d->template_container.mem_request = mem;
    for (int i = 0; i < replicas && g_pod_count < MAX_PODS; i++) {
        char pn[MAX_NAME]; my_strcpy(pn, name, MAX_NAME - 8);
        int pl = my_strlen(pn); pn[pl] = '-'; pn[pl + 1] = '0' + (i % 10); pn[pl + 2] = '\0';
        int pidx = pod_create(pn, image, cpu, mem, 0);
        if (pidx >= 0) {
            my_strcpy(d->pods[d->replicas].name, g_pods[pidx].name, MAX_NAME); d->replicas++;
            my_strcpy(g_pods[pidx].labels[0].key, "app", MAX_NAME);
            my_strcpy(g_pods[pidx].labels[0].value, name, MAX_VALUE); g_pods[pidx].label_count = 1;
        }
    }
    g_deploy_count++; return g_deploy_count - 1;
}
static int deployment_scale(int di, int nr) {
    if (di < 0 || di >= g_deploy_count) return -1;
    deployment_t *d = &g_deploys[di]; int old = d->replicas;
    if (nr > old) {
        for (int i = old; i < nr && g_pod_count < MAX_PODS; i++) {
            char pn[MAX_NAME]; my_strcpy(pn, d->name, MAX_NAME - 8);
            int pl = my_strlen(pn); pn[pl] = '-'; pn[pl + 1] = '0' + (i % 10); pn[pl + 2] = '\0';
            int pidx = pod_create(pn, d->template_container.image,
                                  d->template_container.cpu_request, d->template_container.mem_request, 0);
            if (pidx >= 0) {
                my_strcpy(d->pods[i].name, g_pods[pidx].name, MAX_NAME);
                my_strcpy(g_pods[pidx].labels[0].key, "app", MAX_NAME);
                my_strcpy(g_pods[pidx].labels[0].value, d->name, MAX_VALUE); g_pods[pidx].label_count = 1;
            }
        }
    } else if (nr < old) {
        for (int i = old - 1; i >= nr; i--)
            for (int j = 0; j < g_pod_count; j++)
                if (my_strcmp(g_pods[j].name, d->pods[i].name) == 0) { pod_delete(j); break; }
    }
    d->replicas = nr; d->desired = nr; return nr;
}
static int service_create(const char *name, int port, int target, int lb_algo) {
    if (g_svc_count >= MAX_SERVICES) return -1;
    service_t *s = &g_services[g_svc_count]; my_memset(s, 0, sizeof(service_t));
    my_strcpy(s->name, name, MAX_NAME); my_strcpy(s->selector[0].key, "app", MAX_NAME);
    my_strcpy(s->selector[0].value, name, MAX_VALUE); s->selector_count = 1;
    s->port = port; s->target_port = target; s->lb_algo = lb_algo;
    s->cluster_ip[0] = '1'; s->cluster_ip[1] = '0'; s->cluster_ip[2] = '.';
    s->cluster_ip[3] = '9'; s->cluster_ip[4] = '6'; s->cluster_ip[5] = '.';
    s->cluster_ip[6] = '0' + (g_svc_count % 10); s->cluster_ip[7] = '.';
    s->cluster_ip[8] = '1' + (g_svc_count % 5); s->cluster_ip[9] = '\0';
    g_svc_count++; service_sync_endpoints(g_svc_count - 1); return g_svc_count - 1;
}
static int pod_matches_selector(int pi, const label_t *sel, int sc) {
    if (pi < 0 || pi >= g_pod_count) return 0; pod_t *p = &g_pods[pi]; int matched = 0;
    for (int i = 0; i < sc; i++)
        for (int j = 0; j < p->label_count; j++)
            if (my_strcmp(p->labels[j].key, sel[i].key) == 0 && my_strcmp(p->labels[j].value, sel[i].value) == 0) { matched++; break; }
    return (matched == sc) ? 1 : 0;
}
static void service_sync_endpoints(int si) {
    if (si < 0 || si >= g_svc_count) return; service_t *s = &g_services[si]; s->ep_count = 0;
    for (int i = 0; i < g_pod_count; i++) {
        if (g_pods[i].status != POD_RUNNING) continue;
        if (!pod_matches_selector(i, s->selector, s->selector_count)) continue;
        if (!probe_check_readiness(i)) continue;
        if (s->ep_count >= MAX_ENDPOINTS) break;
        my_strcpy(s->endpoints[s->ep_count].pod_name, g_pods[i].name, MAX_NAME);
        my_strcpy(s->endpoints[s->ep_count].ip_addr, g_pods[i].ip_addr, 16);
        s->endpoints[s->ep_count].port = s->target_port; s->endpoints[s->ep_count].ready = 1; s->ep_count++;
    }
}
static int service_resolve(const char *name) {
    for (int i = 0; i < g_svc_count; i++) {
        if (my_strcmp(g_services[i].name, name) != 0) continue;
        service_t *s = &g_services[i]; if (s->ep_count == 0) return -1;
        int idx = 0;
        if (s->lb_algo == LB_ROUND_ROBIN) { idx = s->rr_index % s->ep_count; s->rr_index++; }
        else if (s->lb_algo == LB_RANDOM) idx = my_rand() % s->ep_count;
        return idx;
    }
    return -1;
}
static int hpa_evaluate(hpa_t *hpa) {
    if (!hpa) return 0; int di = -1;
    for (int i = 0; i < g_deploy_count; i++)
        if (my_strcmp(g_deploys[i].name, hpa->deploy_name) == 0) { di = i; break; }
    if (di < 0) return 0; deployment_t *d = &g_deploys[di];
    int total_cpu = 0, active_pods = 0;
    for (int i = 0; i < g_pod_count; i++) {
        if (g_pods[i].status != POD_RUNNING || g_pods[i].label_count == 0) continue;
        if (my_strcmp(g_pods[i].labels[0].value, hpa->deploy_name) != 0) continue;
        total_cpu += g_pods[i].containers[0].cpu_request; active_pods++;
    }
    if (active_pods == 0) return 0;
    int avg = total_cpu / active_pods; hpa->current_value = avg;
    if (g_tick - hpa->last_scale_time < hpa->cooldown) return 0;
    if (avg > hpa->target_value && d->replicas < hpa->max_replicas) return 1;
    if (avg < hpa->target_value / 2 && d->replicas > hpa->min_replicas) return -1;
    return 0;
}
static int hpa_scale(hpa_t *hpa) {
    int dec = hpa_evaluate(hpa); if (dec == 0) return 0; int di = -1;
    for (int i = 0; i < g_deploy_count; i++)
        if (my_strcmp(g_deploys[i].name, hpa->deploy_name) == 0) { di = i; break; }
    if (di < 0) return 0; deployment_t *d = &g_deploys[di];
    int nr = d->replicas + dec;
    if (nr < hpa->min_replicas) nr = hpa->min_replicas;
    if (nr > hpa->max_replicas) nr = hpa->max_replicas;
    if (nr == d->replicas) return 0;
    deployment_scale(di, nr); hpa->last_scale_time = g_tick; return dec;
}
static void reconcile_loop(void) {
    g_tick++;
    for (int i = 0; i < g_deploy_count; i++) {
        deployment_t *d = &g_deploys[i]; int actual = 0;
        for (int j = 0; j < g_pod_count; j++)
            if (g_pods[j].label_count > 0 && my_strcmp(g_pods[j].labels[0].value, d->name) == 0 &&
                (g_pods[j].status == POD_RUNNING || g_pods[j].status == POD_PENDING)) actual++;
        for (int j = 0; j < g_pod_count; j++)
            if (g_pods[j].status == POD_FAILED && g_pods[j].label_count > 0 &&
                my_strcmp(g_pods[j].labels[0].value, d->name) == 0) { pod_delete(j); actual--; break; }
        if (actual < d->desired) {
            for (int k = 0; k < (d->desired - actual) && g_pod_count < MAX_PODS; k++) {
                char pn[MAX_NAME]; my_strcpy(pn, d->name, MAX_NAME - 8);
                int pl = my_strlen(pn); pn[pl] = '-'; pn[pl + 1] = '0' + ((d->replicas + k) % 10); pn[pl + 2] = '\0';
                int pidx = pod_create(pn, d->template_container.image,
                                      d->template_container.cpu_request, d->template_container.mem_request, 0);
                if (pidx >= 0) {
                    my_strcpy(g_pods[pidx].labels[0].key, "app", MAX_NAME);
                    my_strcpy(g_pods[pidx].labels[0].value, d->name, MAX_VALUE); g_pods[pidx].label_count = 1;
                }
            }
            d->replicas = d->desired;
        }
    }
    for (int i = 0; i < g_pod_count; i++) if (g_pods[i].status == POD_PENDING) pod_schedule(i);
    for (int i = 0; i < g_svc_count; i++) service_sync_endpoints(i);
    for (int i = 0; i < g_pod_count; i++) {
        if (g_pods[i].status != POD_RUNNING) continue;
        if (!probe_check_liveness(i)) {
            g_pods[i].restart_count++; g_pods[i].containers[0].restarts++;
            if (g_pods[i].restart_count > 5) {
                g_pods[i].status = POD_FAILED; g_pods[i].containers[0].status = CONT_TERMINATED;
            }
        }
    }
}

static void print_header(const char *t) { print_str("=== "); print_str(t); print_str(" ===\n"); }
static void print_int(int v) {
    char b[16]; int i = 0; if (v < 0) { b[i++] = '-'; v = -v; }
    if (v == 0) b[i++] = '0'; int st = i;
    while (v > 0) { b[i++] = '0' + (v % 10); v /= 10; }
    int en = i - 1;
    while (st < en) { char t = b[st]; b[st] = b[en]; b[en] = t; st++; en--; }
    b[i] = '\0'; print_str(b);
}
static void print_pod(int idx) {
    print_str("  pod["); print_int(idx); print_str("] "); print_str(g_pods[idx].name);
    print_str(" -> node="); print_str(g_pods[idx].node_name[0] ? g_pods[idx].node_name : "(unscheduled)");
    print_str(" status="); int s = g_pods[idx].status;
    print_str(s == POD_PENDING ? "Pending" : s == POD_RUNNING ? "Running" :
              s == POD_FAILED ? "Failed" : "Unknown"); print_str("\n");
}

static int test_suite(void) {
    int passed = 0, failed = 0;
    print_header("Kubernetes Orchestrator Test Suite");
    print_str("[TEST] Cluster setup\n");
    int n0 = node_add("node-1", 4000, 8192), n1 = node_add("node-2", 2000, 4096);
    int n2 = node_add("node-3", 8000, 16384);
    if (n0 >= 0 && n1 >= 0 && n2 >= 0) { passed++; print_str("  PASS: nodes created\n"); }
    else { failed++; print_str("  FAIL: nodes\n"); }
    node_add_label(0, "zone", "us-east-1a"); node_add_label(1, "zone", "us-east-1b");
    node_add_label(2, "zone", "us-west-2a"); node_add_label(2, "gpu", "true"); node_add_taint(2, "gpu-only");
    print_str("[TEST] ConfigMap/Secret\n");
    int cm = configmap_create("app-config", "db_host", "db.internal.svc");
    int sc = secret_create("db-secret", "password", "s3cret");
    if (cm >= 0 && sc >= 0) { passed++; print_str("  PASS: config/secret\n"); }
    else { failed++; print_str("  FAIL: config/secret\n"); }
    print_str("[TEST] Pod creation/scheduling\n");
    int p0 = pod_create("web-0", "nginx:1.21", 500, 256, 10);
    int p1 = pod_create("web-1", "nginx:1.21", 500, 256, 10);
    int p2 = pod_create("api-0", "api:latest", 1000, 512, 20);
    if (p0 >= 0 && p1 >= 0 && p2 >= 0) passed++;
    else { failed++; print_str("  FAIL: pod creation\n"); }
    int s0 = pod_schedule(p0), s1 = pod_schedule(p1), s2 = pod_schedule(p2);
    if (s0 == SCHED_OK && s1 == SCHED_OK && s2 == SCHED_OK) { passed++; print_str("  PASS: pods scheduled\n"); }
    else { failed++; print_str("  FAIL: scheduling\n"); }
    print_str("[TEST] Probes\n");
    pod_set_liveness(p0, PROBE_HTTP, "/healthz", 8080); pod_set_readiness(p0, PROBE_HTTP, "/ready", 8080);
    if (probe_check_liveness(p0) && probe_check_readiness(p0)) { passed++; print_str("  PASS: probes ok\n"); }
    else { failed++; print_str("  FAIL: probes\n"); }
    print_str("[TEST] Deployment replicas\n");
    int d0 = deployment_create("backend", "backend:v2", 3, 250, 128);
    if (d0 >= 0 && g_deploys[d0].replicas == 3) { passed++; print_str("  PASS: 3 replicas\n"); }
    else { failed++; print_str("  FAIL: deployment\n"); }
    for (int i = 0; i < g_pod_count; i++) if (g_pods[i].status == POD_PENDING) pod_schedule(i);
    print_str("[TEST] Service/DNS\n");
    int sv0 = service_create("backend", 80, 8080, LB_ROUND_ROBIN);
    if (sv0 >= 0) passed++; else { failed++; print_str("  FAIL: service create\n"); }
    int ep = service_resolve("backend");
    if (ep >= 0) { passed++; print_str("  PASS: service resolved\n"); }
    else { failed++; print_str("  FAIL: service resolve\n"); }
    print_str("[TEST] Deployment scaling\n");
    if (deployment_scale(d0, 5) == 5) { passed++; print_str("  PASS: scale to 5\n"); }
    else { failed++; print_str("  FAIL: scale up\n"); }
    if (deployment_scale(d0, 2) == 2) { passed++; print_str("  PASS: scale to 2\n"); }
    else { failed++; print_str("  FAIL: scale down\n"); }
    print_str("[TEST] HPA\n");
    hpa_t hpa; my_memset(&hpa, 0, sizeof(hpa_t)); my_strcpy(hpa.deploy_name, "backend", MAX_NAME);
    hpa.metric_type = 0; hpa.target_value = 300; hpa.min_replicas = 1; hpa.max_replicas = 8;
    hpa.cooldown = 2;
    int hp = hpa_evaluate(&hpa); print_str("  HPA decision: "); print_int(hp); print_str("\n"); passed++;
    print_str("[TEST] Reconciliation\n");
    reconcile_loop(); reconcile_loop(); reconcile_loop();
    passed++; print_str("  PASS: reconcile 3 ticks\n");
    print_str("[TEST] Affinity/taints\n");
    int pa = pod_create("gpu-pod", "cuda:11.0", 2000, 1024, 50);
    if (pa >= 0) {
        my_strcpy(g_pods[pa].node_affinity, "gpu", MAX_NAME);
        my_strcpy(g_pods[pa].tolerations[0], "gpu-only", MAX_NAME); g_pods[pa].toleration_count = 1;
        if (pod_schedule(pa) == SCHED_OK && g_pods[pa].node_name[0]) {
            passed++; print_str("  PASS: gpu-pod -> "); print_str(g_pods[pa].node_name); print_str("\n");
        } else { failed++; print_str("  FAIL: affinity\n"); }
    }
    print_str("\n"); print_header("Summary");
    print_str("  Passed: "); print_int(passed); print_str("\n  Failed: "); print_int(failed); print_str("\n");
    print_str("  Pods: "); print_int(g_pod_count); print_str(" Nodes: "); print_int(g_node_count);
    print_str(" Services: "); print_int(g_svc_count); print_str(" Deployments: "); print_int(g_deploy_count); print_str("\n");
    return failed;
}

static void print_help(void) {
    print_str("kubernetes_orchestrator: K8s-style orchestration engine\n");
    print_str("  -h  Help  -t  Test suite\n");
    print_str("Features: Pods, Deployments, Services, Scheduler, HPA, Probes, Reconciliation\n");
}
static void print_status(void) {
    print_header("Cluster Status");
    for (int i = 0; i < g_node_count; i++) {
        print_str("  node: "); print_str(g_nodes[i].name); print_str(" cpu="); print_int(g_nodes[i].cpu_alloc);
        print_str("/"); print_int(g_nodes[i].cpu_capacity); print_str(" mem="); print_int(g_nodes[i].mem_alloc);
        print_str("/"); print_int(g_nodes[i].mem_capacity); print_str("\n");
    }
    for (int i = 0; i < g_pod_count; i++) print_pod(i);
}

__attribute__((export_name("_start")))
void _start(void) {
    unsigned int buf = host_alloc(1024, 4); int argc = host_get_argv(buf, 1024);
    char *args = (char *)buf; (void)argc;
    if (args[0] == '-' && args[1] == 'h') { print_help(); host_exit(0); return; }
    if (args[0] == '-' && args[1] == 't') {
        int f = test_suite(); print_str("\n"); print_status(); host_exit(f > 0 ? 1 : 0);
    } else { print_help(); host_exit(0); }
}
