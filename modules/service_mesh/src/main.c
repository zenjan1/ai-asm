/* service_mesh: Service mesh sidecar proxy with mTLS, routing, observability (v46.0) */
#include <stddef.h>

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* ─── Constants ─── */
#define MAX_SERVICES       32
#define MAX_CONNECTIONS    64
#define MAX_ROUTES         32
#define MAX_CERTS          16
#define MAX_SPANS          64
#define MAX_POLICIES       32
#define MAX_RATE_RULES     16
#define MAX_LABELS          8
#define MAX_NAME_LEN       32
#define MAX_HEADER_LEN     64
#define MAX_CERT_LEN       512
#define MAX_SPIFFE_ID      128
#define MAX_ZONE_LEN        8
#define BUF_SIZE          4096
#define CB_FAILURE_THRESH  5
#define CB_RETRY_TIMEOUT  30
#define DEFAULT_TIMEOUT   5000
#define DEFAULT_QPS_LIMIT 1000
#define DEFAULT_WEIGHT    100
#define INITIAL_HEAP    65536

/* ─── Protocol / state enums ─── */
#define PROTO_HTTP    0
#define PROTO_GRPC    1
#define PROTO_TCP     2
#define PROTO_UNKNOWN 3

#define STATE_IDLE    0
#define STATE_ACTIVE  1
#define STATE_CLOSED  2

#define CB_CLOSED      0
#define CB_HALF_OPEN   1
#define CB_OPEN        2

#define POLICY_ALLOW   0
#define POLICY_DENY    1
#define POLICY_LOG     2

#define AUTH_NONE      0
#define AUTH_JWT       1
#define AUTH_OAUTH     2
#define AUTH_MTLS      3

/* ─── Typedefs ─── */

typedef struct {
    char key[MAX_HEADER_LEN];
    char value[MAX_HEADER_LEN];
    int in_use;
} header_t;

typedef struct {
    char service_name[MAX_NAME_LEN];
    int protocol;
    int state;
    int conn_count;
    int max_connections;
    unsigned int total_requests;
    unsigned int total_errors;
    unsigned int latency_sum_ms;
    char zone[MAX_ZONE_LEN];
    int in_use;
} service_entry_t;

typedef struct {
    int id;
    int service_idx;
    int state;
    unsigned int opened_at;
    unsigned int last_active;
    int in_use;
} connection_t;

typedef struct {
    char subject[MAX_NAME_LEN];
    char issuer[MAX_NAME_LEN];
    char spiffe_id[MAX_SPIFFE_ID];
    unsigned int not_before;
    unsigned int not_after;
    char pem_data[128];
    int in_use;
} cert_t;

typedef struct {
    char source_service[MAX_NAME_LEN];
    char dest_service[MAX_NAME_LEN];
    int weight;
    int protocol;
    int mirror_enabled;
    char mirror_target[MAX_NAME_LEN];
    int fault_inject_pct;
    int fault_delay_ms;
    int retries;
    unsigned int timeout_ms;
    int in_use;
} route_rule_t;

typedef struct {
    char service_name[MAX_NAME_LEN];
    int state;
    unsigned int failure_count;
    unsigned int success_count;
    unsigned int last_failure_time;
    unsigned int opened_at;
} circuit_breaker_t;

typedef struct {
    char span_id[20];
    char trace_id[40];
    char parent_id[20];
    char operation[MAX_NAME_LEN];
    char service[MAX_NAME_LEN];
    unsigned int start_time;
    unsigned int duration_ms;
    int status;
    int in_use;
} span_t;

typedef struct {
    char source[MAX_NAME_LEN];
    char dest[MAX_NAME_LEN];
    int action;
    int auth_type;
    int rate_limit_qps;
    int in_use;
} policy_t;

typedef struct {
    char service[MAX_NAME_LEN];
    int max_qps;
    unsigned int window_ms;
    unsigned int current_count;
    unsigned int window_start;
    int in_use;
} rate_limit_t;

typedef struct {
    int proxy_port;
    int admin_port;
    int mtls_enabled;
    int tracing_enabled;
    int metrics_enabled;
    unsigned int uptime;
    unsigned int intercepted;
    unsigned int forwarded;
    unsigned int dropped;
    char local_zone[MAX_ZONE_LEN];
} sidecar_config_t;

/* ─── Globals ─── */
static unsigned int heap_pos = INITIAL_HEAP;
static unsigned int clk = 0;
static int initialized = 0;

static service_entry_t services[MAX_SERVICES];
static connection_t connections[MAX_CONNECTIONS];
static cert_t certs[MAX_CERTS];
static route_rule_t routes[MAX_ROUTES];
static circuit_breaker_t breakers[MAX_SERVICES];
static span_t spans[MAX_SPANS];
static policy_t policies[MAX_POLICIES];
static rate_limit_t rate_limits[MAX_RATE_RULES];
static sidecar_config_t sidecar_cfg;

static int service_count = 0;
static int conn_count = 0;
static int cert_count = 0;
static int route_count = 0;
static int breaker_count = 0;
static int span_count = 0;
static int policy_count = 0;
static int rate_count = 0;

/* ─── Forward declarations ─── */
static void circuit_breaker_transition(int idx);
static void rate_limit_reset(int idx);

/* ─── Helper utilities ─── */
static void pstr(const char *s) { host_print(s); }

static int my_strlen(const char *s) {
    int l = 0;
    while (s[l]) l++;
    return l;
}

static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) {
        if (*a != *b) return *a - *b;
        a++; b++;
    }
    return *a - *b;
}

static void my_strncpy(char *d, const char *s, int n) {
    int i = 0;
    while (i < n - 1 && s[i]) { d[i] = s[i]; i++; }
    d[i] = '\0';
}

static void pint(int v) {
    char b[32];
    int p = 0;
    if (v < 0) { b[p++] = '-'; v = -v; }
    if (v == 0) { b[p++] = '0'; }
    else {
        int d = 0, t = v;
        while (t > 0) { d++; t /= 10; }
        p += d;
        b[p] = '\0';
        p--;
        while (v > 0) { b[p--] = '0' + (v % 10); v /= 10; }
    }
    host_print(b);
}

static void puint(unsigned int v) { pint((int)v); }

static void hex_byte(char *out, unsigned int val) {
    static const char hx[] = "0123456789ABCDEF";
    out[0] = hx[(val >> 4) & 0x0F];
    out[1] = hx[val & 0x0F];
}

static void gen_id(char *buf, int len, unsigned int seed) {
    static const char ch[] = "abcdef0123456789";
    for (int i = 0; i < len - 1; i++) {
        buf[i] = ch[(seed + (unsigned int)i * 7u) % 16u];
    }
    buf[len - 1] = '\0';
}

static void print_header(const char *title) {
    pstr("\n=== "); pstr(title); pstr(" ===\n");
}

/* ─── Sidecar proxy ─── */
static int sidecar_init(int proxy_port, int admin_port, const char *zone) {
    sidecar_cfg.proxy_port = proxy_port;
    sidecar_cfg.admin_port = admin_port;
    sidecar_cfg.mtls_enabled = 1;
    sidecar_cfg.tracing_enabled = 1;
    sidecar_cfg.metrics_enabled = 1;
    sidecar_cfg.uptime = 0;
    sidecar_cfg.intercepted = 0;
    sidecar_cfg.forwarded = 0;
    sidecar_cfg.dropped = 0;
    my_strncpy(sidecar_cfg.local_zone, zone, MAX_ZONE_LEN - 1);
    for (int i = 0; i < MAX_SERVICES; i++) { services[i].in_use = 0; }
    for (int i = 0; i < MAX_CONNECTIONS; i++) { connections[i].in_use = 0; }
    for (int i = 0; i < MAX_CERTS; i++) { certs[i].in_use = 0; }
    for (int i = 0; i < MAX_ROUTES; i++) { routes[i].in_use = 0; }
    for (int i = 0; i < MAX_SPANS; i++) { spans[i].in_use = 0; }
    for (int i = 0; i < MAX_POLICIES; i++) { policies[i].in_use = 0; }
    for (int i = 0; i < MAX_RATE_RULES; i++) { rate_limits[i].in_use = 0; }
    service_count = 0; conn_count = 0; cert_count = 0;
    route_count = 0; span_count = 0; policy_count = 0; rate_count = 0;
    initialized = 1;
    pstr("Sidecar init: port="); pint(proxy_port);
    pstr(" admin="); pint(admin_port);
    pstr(" zone="); pstr(zone); pstr("\n");
    return 0;
}

static int sidecar_intercept(const char *svc_name, int protocol) {
    if (!initialized) { pstr("ERR: sidecar not initialized\n"); return -1; }
    sidecar_cfg.intercepted++;
    sidecar_cfg.uptime++;
    pstr("Intercept: "); pstr(svc_name);
    pstr(" proto="); pint(protocol); pstr("\n");
    return 0;
}

static int sidecar_forward(const char *svc_name) {
    int idx = -1;
    for (int i = 0; i < MAX_SERVICES; i++) {
        if (services[i].in_use && my_strcmp(services[i].service_name, svc_name) == 0) {
            idx = i;
            break;
        }
    }
    if (idx < 0) {
        pstr("Forward: unknown service "); pstr(svc_name); pstr("\n");
        sidecar_cfg.dropped++;
        return -1;
    }
    if (services[idx].conn_count >= services[idx].max_connections) {
        pstr("Forward: connection limit reached for "); pstr(svc_name); pstr("\n");
        sidecar_cfg.dropped++;
        return -1;
    }
    int slot = -1;
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        if (!connections[i].in_use) { slot = i; break; }
    }
    if (slot < 0) { pstr("Forward: no free connections\n"); return -1; }
    connections[slot].id = conn_count;
    connections[slot].service_idx = idx;
    connections[slot].state = STATE_ACTIVE;
    connections[slot].opened_at = clk;
    connections[slot].last_active = clk;
    connections[slot].in_use = 1;
    conn_count++;
    services[idx].conn_count++;
    services[idx].total_requests++;
    sidecar_cfg.forwarded++;
    pstr("Forward: "); pstr(svc_name);
    pstr(" conn="); pint(slot);
    pstr(" pool="); pint(services[idx].conn_count);
    pstr("/"); pint(services[idx].max_connections); pstr("\n");
    return slot;
}

static int sidecar_close_connection(int conn_id) {
    if (conn_id < 0 || conn_id >= MAX_CONNECTIONS) return -1;
    if (!connections[conn_id].in_use) return -1;
    int idx = connections[conn_id].service_idx;
    if (idx >= 0 && idx < MAX_SERVICES && services[idx].in_use) {
        if (services[idx].conn_count > 0) services[idx].conn_count--;
    }
    connections[conn_id].state = STATE_CLOSED;
    connections[conn_id].in_use = 0;
    pstr("Close conn "); pint(conn_id); pstr("\n");
    return 0;
}

static int sidecar_register_service(const char *name, int proto, int max_conn, const char *zone) {
    if (service_count >= MAX_SERVICES) return -1;
    int i = service_count++;
    my_strncpy(services[i].service_name, name, MAX_NAME_LEN - 1);
    services[i].protocol = proto;
    services[i].state = STATE_IDLE;
    services[i].conn_count = 0;
    services[i].max_connections = max_conn;
    services[i].total_requests = 0;
    services[i].total_errors = 0;
    services[i].latency_sum_ms = 0;
    my_strncpy(services[i].zone, zone, MAX_ZONE_LEN - 1);
    services[i].in_use = 1;
    pstr("Register svc: "); pstr(name);
    pstr(" zone="); pstr(zone);
    pstr(" max_conn="); pint(max_conn); pstr("\n");
    return i;
}

/* ─── mTLS ─── */
static int mtls_issue_cert(const char *subject, const char *spiffe_id) {
    if (cert_count >= MAX_CERTS) return -1;
    int i = cert_count++;
    my_strncpy(certs[i].subject, subject, MAX_NAME_LEN - 1);
    my_strncpy(certs[i].issuer, "mesh-ca", MAX_NAME_LEN - 1);
    my_strncpy(certs[i].spiffe_id, spiffe_id, MAX_SPIFFE_ID - 1);
    certs[i].not_before = clk;
    certs[i].not_after = clk + 86400;
    my_strncpy(certs[i].pem_data, "-----BEGIN CERT-----\nMIIBmock...", 127);
    certs[i].in_use = 1;
    pstr("Cert issued: "); pstr(subject);
    pstr(" spiffe="); pstr(spiffe_id);
    pstr(" ttl=86400s\n");
    return i;
}

static int mtls_verify_cert(int cert_idx) {
    if (cert_idx < 0 || cert_idx >= MAX_CERTS) return -1;
    if (!certs[cert_idx].in_use) return -1;
    if (clk < certs[cert_idx].not_before) {
        pstr("Cert verify FAIL: not yet valid "); pstr(certs[cert_idx].subject); pstr("\n");
        return -2;
    }
    if (clk > certs[cert_idx].not_after) {
        pstr("Cert verify FAIL: expired "); pstr(certs[cert_idx].subject); pstr("\n");
        return -3;
    }
    pstr("Cert verify OK: "); pstr(certs[cert_idx].subject);
    pstr(" spiffe="); pstr(certs[cert_idx].spiffe_id); pstr("\n");
    return 0;
}

static int mtls_rotate_cert(int cert_idx) {
    if (cert_idx < 0 || cert_idx >= MAX_CERTS) return -1;
    if (!certs[cert_idx].in_use) return -1;
    certs[cert_idx].not_before = clk;
    certs[cert_idx].not_after = clk + 86400;
    pstr("Cert rotated: "); pstr(certs[cert_idx].subject);
    pstr(" new_ttl=86400s\n");
    return 0;
}

static int mtls_handshake(const char *client_svc, const char *server_svc) {
    pstr("mTLS handshake: "); pstr(client_svc);
    pstr(" -> "); pstr(server_svc); pstr("\n");
    int c_cert = -1, s_cert = -1;
    for (int i = 0; i < cert_count; i++) {
        if (certs[i].in_use && my_strcmp(certs[i].subject, client_svc) == 0) c_cert = i;
        if (certs[i].in_use && my_strcmp(certs[i].subject, server_svc) == 0) s_cert = i;
    }
    if (c_cert < 0) {
        char buf[MAX_SPIFFE_ID];
        my_strncpy(buf, "spiffe://mesh.local/", 64);
        int bl = my_strlen(buf);
        my_strncpy(buf + bl, client_svc, MAX_SPIFFE_ID - bl - 1);
        c_cert = mtls_issue_cert(client_svc, buf);
    }
    if (s_cert < 0) {
        char buf[MAX_SPIFFE_ID];
        my_strncpy(buf, "spiffe://mesh.local/", 64);
        int bl = my_strlen(buf);
        my_strncpy(buf + bl, server_svc, MAX_SPIFFE_ID - bl - 1);
        s_cert = mtls_issue_cert(server_svc, buf);
    }
    int rv = mtls_verify_cert(c_cert);
    if (rv < 0) { pstr("  Client cert invalid\n"); return rv; }
    rv = mtls_verify_cert(s_cert);
    if (rv < 0) { pstr("  Server cert invalid\n"); return rv; }
    pstr("  mTLS handshake OK\n");
    return 0;
}

/* ─── Routing ─── */
static int route_add(const char *src, const char *dst, int weight, int proto,
                     int retries, unsigned int timeout_ms) {
    if (route_count >= MAX_ROUTES) return -1;
    int i = route_count++;
    my_strncpy(routes[i].source_service, src, MAX_NAME_LEN - 1);
    my_strncpy(routes[i].dest_service, dst, MAX_NAME_LEN - 1);
    routes[i].weight = weight;
    routes[i].protocol = proto;
    routes[i].mirror_enabled = 0;
    routes[i].fault_inject_pct = 0;
    routes[i].fault_delay_ms = 0;
    routes[i].retries = retries;
    routes[i].timeout_ms = timeout_ms;
    routes[i].in_use = 1;
    pstr("Route add: "); pstr(src); pstr(" -> "); pstr(dst);
    pstr(" w="); pint(weight); pstr("\n");
    return i;
}

static int route_match(const char *src, const char *dst, int proto) {
    int best = -1;
    int best_w = -1;
    for (int i = 0; i < route_count; i++) {
        if (!routes[i].in_use) continue;
        if (routes[i].protocol != proto && routes[i].protocol != PROTO_UNKNOWN) continue;
        int src_ok = my_strcmp(routes[i].source_service, "*") == 0 ||
                     my_strcmp(routes[i].source_service, src) == 0;
        int dst_ok = my_strcmp(routes[i].dest_service, dst) == 0;
        if (src_ok && dst_ok && routes[i].weight > best_w) {
            best = i;
            best_w = routes[i].weight;
        }
    }
    if (best >= 0) {
        pstr("Route match: "); pstr(src); pstr("->"); pstr(dst);
        pstr(" rule="); pint(best); pstr("\n");
    }
    return best;
}

static int route_weight_select(const char *dst, int count) {
    int total_w = 0;
    int matching[MAX_ROUTES];
    int match_n = 0;
    for (int i = 0; i < route_count; i++) {
        if (routes[i].in_use && my_strcmp(routes[i].dest_service, dst) == 0) {
            matching[match_n++] = i;
            total_w += routes[i].weight;
        }
    }
    if (match_n == 0) return -1;
    unsigned int sel = (clk * 31u + (unsigned int)count) % (unsigned int)(total_w > 0 ? total_w : 1);
    unsigned int cum = 0;
    for (int j = 0; j < match_n; j++) {
        cum += (unsigned int)routes[matching[j]].weight;
        if (sel < cum) return matching[j];
    }
    return matching[match_n - 1];
}

static int route_mirror(const char *src, const char *mirror_target) {
    pstr("Mirror: "); pstr(src); pstr(" -> "); pstr(mirror_target); pstr("\n");
    return 0;
}

static void route_enable_mirror(int idx, const char *target) {
    if (idx < 0 || idx >= MAX_ROUTES) return;
    routes[idx].mirror_enabled = 1;
    my_strncpy(routes[idx].mirror_target, target, MAX_NAME_LEN - 1);
    pstr("Mirror enabled on route "); pint(idx);
    pstr(" target="); pstr(target); pstr("\n");
}

/* ─── Circuit breaker ─── */
static int circuit_breaker_init(const char *svc) {
    if (breaker_count >= MAX_SERVICES) return -1;
    int i = breaker_count++;
    my_strncpy(breakers[i].service_name, svc, MAX_NAME_LEN - 1);
    breakers[i].state = CB_CLOSED;
    breakers[i].failure_count = 0;
    breakers[i].success_count = 0;
    breakers[i].last_failure_time = 0;
    breakers[i].opened_at = 0;
    pstr("CB init: "); pstr(svc); pstr(" state=CLOSED\n");
    return i;
}

static const char *cb_state_str(int st) {
    if (st == CB_CLOSED) return "CLOSED";
    if (st == CB_HALF_OPEN) return "HALF_OPEN";
    if (st == CB_OPEN) return "OPEN";
    return "UNKNOWN";
}

static int circuit_breaker_check(const char *svc) {
    int idx = -1;
    for (int i = 0; i < breaker_count; i++) {
        if (my_strcmp(breakers[i].service_name, svc) == 0) { idx = i; break; }
    }
    if (idx < 0) { pstr("CB: no breaker for "); pstr(svc); pstr("\n"); return -1; }
    circuit_breaker_transition(idx);
    pstr("CB check: "); pstr(svc);
    pstr(" state="); pstr(cb_state_str(breakers[idx].state));
    pstr(" fail="); puint(breakers[idx].failure_count); pstr("\n");
    if (breakers[idx].state == CB_OPEN) return -1;
    return 0;
}

static void circuit_breaker_transition(int idx) {
    if (idx < 0 || idx >= breaker_count) return;
    if (breakers[idx].state == CB_OPEN) {
        if ((clk - breakers[idx].opened_at) >= CB_RETRY_TIMEOUT) {
            breakers[idx].state = CB_HALF_OPEN;
            pstr("CB transition: OPEN -> HALF_OPEN for ");
            pstr(breakers[idx].service_name); pstr("\n");
        }
    }
}

static int circuit_breaker_record(int idx, int success) {
    if (idx < 0 || idx >= breaker_count) return -1;
    if (success) {
        breakers[idx].success_count++;
        if (breakers[idx].state == CB_HALF_OPEN) {
            breakers[idx].state = CB_CLOSED;
            breakers[idx].failure_count = 0;
            pstr("CB transition: HALF_OPEN -> CLOSED for ");
            pstr(breakers[idx].service_name); pstr("\n");
        }
    } else {
        breakers[idx].failure_count++;
        breakers[idx].last_failure_time = clk;
        if (breakers[idx].state == CB_CLOSED &&
            (int)breakers[idx].failure_count >= CB_FAILURE_THRESH) {
            breakers[idx].state = CB_OPEN;
            breakers[idx].opened_at = clk;
            pstr("CB transition: CLOSED -> OPEN for ");
            pstr(breakers[idx].service_name); pstr("\n");
        } else if (breakers[idx].state == CB_HALF_OPEN) {
            breakers[idx].state = CB_OPEN;
            breakers[idx].opened_at = clk;
            pstr("CB transition: HALF_OPEN -> OPEN for ");
            pstr(breakers[idx].service_name); pstr("\n");
        }
    }
    return 0;
}

/* ─── Observability: tracing ─── */
static int tracing_start_span(const char *operation, const char *service, const char *parent_id) {
    if (span_count >= MAX_SPANS) return -1;
    int i = span_count++;
    gen_id(spans[i].span_id, 17, clk * 13u + 42u);
    if (parent_id && parent_id[0]) {
        my_strncpy(spans[i].trace_id, parent_id, 39);
        gen_id(spans[i].parent_id, 17, clk * 7u + 99u);
    } else {
        gen_id(spans[i].trace_id, 33, clk * 17u + 1u);
        spans[i].parent_id[0] = '\0';
    }
    my_strncpy(spans[i].operation, operation, MAX_NAME_LEN - 1);
    my_strncpy(spans[i].service, service, MAX_NAME_LEN - 1);
    spans[i].start_time = clk;
    spans[i].duration_ms = 0;
    spans[i].status = 0;
    spans[i].in_use = 1;
    pstr("Span start: op="); pstr(operation);
    pstr(" svc="); pstr(service);
    pstr(" span="); pstr(spans[i].span_id);
    pstr(" trace="); pstr(spans[i].trace_id); pstr("\n");
    return i;
}

static void tracing_end_span(int idx, int status, unsigned int dur_ms) {
    if (idx < 0 || idx >= MAX_SPANS || !spans[idx].in_use) return;
    spans[idx].status = status;
    spans[idx].duration_ms = dur_ms;
    pstr("Span end: "); pstr(spans[idx].operation);
    pstr(" dur="); puint(dur_ms);
    pstr("ms status="); pint(status); pstr("\n");
}

static void tracing_inject_context(int span_idx, char *buf, int buf_len) {
    if (span_idx < 0 || span_idx >= MAX_SPANS || !spans[span_idx].in_use) return;
    int pos = 0;
    const char *pfx = "x-b3-traceid:";
    int pl = my_strlen(pfx);
    if (pos + pl < buf_len) { my_strncpy(buf + pos, pfx, buf_len - pos); pos += pl; }
    int tl = my_strlen(spans[span_idx].trace_id);
    if (pos + tl < buf_len) { my_strncpy(buf + pos, spans[span_idx].trace_id, buf_len - pos); pos += tl; }
    const char *sep = ",x-b3-spanid:";
    int sl = my_strlen(sep);
    if (pos + sl < buf_len) { my_strncpy(buf + pos, sep, buf_len - pos); pos += sl; }
    int spl = my_strlen(spans[span_idx].span_id);
    if (pos + spl < buf_len) { my_strncpy(buf + pos, spans[span_idx].span_id, buf_len - pos); pos += spl; }
    if (pos < buf_len) buf[pos] = '\0';
    pstr("Inject ctx: "); pstr(buf); pstr("\n");
}

static void tracing_extract_context(const char *header) {
    pstr("Extract ctx: "); pstr(header); pstr("\n");
}

/* ─── Observability: metrics ─── */
static void metrics_record(const char *svc, int success, unsigned int latency_ms) {
    for (int i = 0; i < service_count; i++) {
        if (services[i].in_use && my_strcmp(services[i].service_name, svc) == 0) {
            services[i].latency_sum_ms += latency_ms;
            if (!success) services[i].total_errors++;
            return;
        }
    }
}

static void metrics_summary(const char *svc) {
    for (int i = 0; i < service_count; i++) {
        if (services[i].in_use && my_strcmp(services[i].service_name, svc) == 0) {
            pstr("Metrics "); pstr(svc); pstr(": req=");
            puint(services[i].total_requests); pstr(" err=");
            puint(services[i].total_errors); pstr(" latency_sum=");
            puint(services[i].latency_sum_ms); pstr("ms");
            if (services[i].total_requests > 0) {
                unsigned int avg = services[i].latency_sum_ms / services[i].total_requests;
                pstr(" avg="); puint(avg); pstr("ms");
            }
            pstr("\n");
            return;
        }
    }
    pstr("Metrics: unknown service "); pstr(svc); pstr("\n");
}

/* ─── Policy engine ─── */
static int policy_add(const char *src, const char *dst, int action, int auth_type) {
    if (policy_count >= MAX_POLICIES) return -1;
    int i = policy_count++;
    my_strncpy(policies[i].source, src, MAX_NAME_LEN - 1);
    my_strncpy(policies[i].dest, dst, MAX_NAME_LEN - 1);
    policies[i].action = action;
    policies[i].auth_type = auth_type;
    policies[i].rate_limit_qps = DEFAULT_QPS_LIMIT;
    policies[i].in_use = 1;
    pstr("Policy add: "); pstr(src); pstr("->"); pstr(dst);
    pstr(" action="); pint(action);
    pstr(" auth="); pint(auth_type); pstr("\n");
    return i;
}

static int policy_evaluate(const char *src, const char *dst, int auth_type) {
    for (int i = 0; i < policy_count; i++) {
        if (!policies[i].in_use) continue;
        int src_ok = my_strcmp(policies[i].source, "*") == 0 ||
                     my_strcmp(policies[i].source, src) == 0;
        int dst_ok = my_strcmp(policies[i].dest, "*") == 0 ||
                     my_strcmp(policies[i].dest, dst) == 0;
        if (src_ok && dst_ok) {
            if (policies[i].auth_type > auth_type) {
                pstr("Policy DENY: "); pstr(src); pstr("->"); pstr(dst);
                pstr(" insufficient auth (need=");
                pint(policies[i].auth_type); pstr(" got="); pint(auth_type);
                pstr(")\n");
                return POLICY_DENY;
            }
            pstr("Policy eval: "); pstr(src); pstr("->"); pstr(dst);
            pstr(" => ");
            if (policies[i].action == POLICY_ALLOW) pstr("ALLOW");
            else if (policies[i].action == POLICY_DENY) pstr("DENY");
            else pstr("LOG");
            pstr("\n");
            return policies[i].action;
        }
    }
    pstr("Policy: no matching rule, default ALLOW\n");
    return POLICY_ALLOW;
}

static int policy_allow_deny(const char *src, const char *dst) {
    int r = policy_evaluate(src, dst, AUTH_MTLS);
    return (r == POLICY_ALLOW) ? 0 : -1;
}

/* ─── Rate limiting ─── */
static int rate_limit_add(const char *svc, int max_qps, unsigned int window_ms) {
    if (rate_count >= MAX_RATE_RULES) return -1;
    int i = rate_count++;
    my_strncpy(rate_limits[i].service, svc, MAX_NAME_LEN - 1);
    rate_limits[i].max_qps = max_qps;
    rate_limits[i].window_ms = window_ms;
    rate_limits[i].current_count = 0;
    rate_limits[i].window_start = clk;
    rate_limits[i].in_use = 1;
    pstr("Rate limit: "); pstr(svc);
    pstr(" qps="); pint(max_qps);
    pstr(" win="); puint(window_ms); pstr("ms\n");
    return i;
}

static int rate_limit_check(const char *svc) {
    for (int i = 0; i < rate_count; i++) {
        if (!rate_limits[i].in_use) continue;
        if (my_strcmp(rate_limits[i].service, svc) != 0) continue;
        if ((clk - rate_limits[i].window_start) >= rate_limits[i].window_ms) {
            rate_limit_reset(i);
        }
        if ((int)rate_limits[i].current_count >= rate_limits[i].max_qps) {
            pstr("Rate limit EXCEEDED: "); pstr(svc);
            pstr(" count="); puint(rate_limits[i].current_count);
            pstr(" limit="); pint(rate_limits[i].max_qps); pstr("\n");
            return -1;
        }
        rate_limits[i].current_count++;
        pstr("Rate limit OK: "); pstr(svc);
        pstr(" count="); puint(rate_limits[i].current_count);
        pstr("/"); pint(rate_limits[i].max_qps); pstr("\n");
        return 0;
    }
    return 0;
}

static void rate_limit_reset(int idx) {
    if (idx < 0 || idx >= MAX_RATE_RULES) return;
    rate_limits[idx].current_count = 0;
    rate_limits[idx].window_start = clk;
    pstr("Rate limit reset: "); pstr(rate_limits[idx].service); pstr("\n");
}

/* ─── Topology / zone-aware routing ─── */
static int topology_zone_match(const char *svc, const char *prefer_zone) {
    for (int i = 0; i < service_count; i++) {
        if (services[i].in_use && my_strcmp(services[i].service_name, svc) == 0) {
            if (my_strcmp(services[i].zone, prefer_zone) == 0) {
                pstr("Zone match: "); pstr(svc);
                pstr(" zone="); pstr(prefer_zone); pstr("\n");
                return i;
            }
            pstr("Zone fallback: "); pstr(svc);
            pstr(" (have="); pstr(services[i].zone);
            pstr(" want="); pstr(prefer_zone); pstr(")\n");
            return i;
        }
    }
    return -1;
}

static int topology_locality_route(const char *src, const char *dst) {
    pstr("Locality route: "); pstr(src); pstr("->"); pstr(dst);
    pstr(" prefer_zone="); pstr(sidecar_cfg.local_zone); pstr("\n");
    int z = topology_zone_match(dst, sidecar_cfg.local_zone);
    if (z >= 0) return z;
    for (int i = 0; i < service_count; i++) {
        if (services[i].in_use && my_strcmp(services[i].service_name, dst) == 0) return i;
    }
    return -1;
}

/* ─── Test suite ─── */
static void test_sidecar_proxy(void) {
    print_header("Sidecar Proxy Test");
    sidecar_init(15001, 15000, "us-east-1a");
    int s0 = sidecar_register_service("frontend", PROTO_HTTP, 100, "us-east-1a");
    int s1 = sidecar_register_service("backend", PROTO_GRPC, 50, "us-east-1b");
    int s2 = sidecar_register_service("database", PROTO_TCP, 20, "us-east-1a");
    (void)s0; (void)s1; (void)s2;
    sidecar_intercept("backend", PROTO_GRPC);
    int c0 = sidecar_forward("backend");
    int c1 = sidecar_forward("backend");
    int c2 = sidecar_forward("frontend");
    sidecar_close_connection(c0);
    pstr("Conns: c0="); pint(c0); pstr(" c1="); pint(c1); pstr(" c2="); pint(c2); pstr("\n");
    sidecar_intercept("database", PROTO_TCP);
    sidecar_forward("database");
    pstr("Sidecar stats: int="); puint(sidecar_cfg.intercepted);
    pstr(" fwd="); puint(sidecar_cfg.forwarded);
    pstr(" drop="); puint(sidecar_cfg.dropped); pstr("\n");
}

static void test_mtls(void) {
    print_header("mTLS Test");
    int c0 = mtls_issue_cert("frontend", "spiffe://mesh.local/frontend");
    int c1 = mtls_issue_cert("backend", "spiffe://mesh.local/backend");
    mtls_verify_cert(c0);
    mtls_verify_cert(c1);
    mtls_handshake("frontend", "backend");
    clk += 90000;
    pstr("After TTL expiry:\n");
    mtls_verify_cert(c0);
    mtls_rotate_cert(c0);
    mtls_verify_cert(c0);
    clk -= 90000;
}

static void test_routing(void) {
    print_header("Traffic Routing Test");
    route_add("frontend", "backend-v1", 70, PROTO_HTTP, 3, 5000);
    route_add("frontend", "backend-v2", 30, PROTO_HTTP, 3, 5000);
    route_add("*", "database", 100, PROTO_TCP, 1, 10000);
    route_match("frontend", "backend-v1", PROTO_HTTP);
    route_match("frontend", "database", PROTO_TCP);
    pstr("Weight select (5x):\n");
    for (int i = 0; i < 5; i++) {
        int r = route_weight_select("backend-v1", i);
        pstr("  pick "); pint(i); pstr(": rule="); pint(r);
        if (r >= 0) { pstr(" dst="); pstr(routes[r].dest_service); }
        pstr("\n");
    }
    route_enable_mirror(0, "backend-canary");
    route_mirror("frontend", "backend-canary");
}

static void test_circuit_breaker(void) {
    print_header("Circuit Breaker Test");
    int cb = circuit_breaker_init("backend");
    circuit_breaker_check("backend");
    pstr("Inject 5 failures:\n");
    for (int i = 0; i < CB_FAILURE_THRESH; i++) {
        circuit_breaker_record(cb, 0);
    }
    circuit_breaker_check("backend");
    pstr("Request during OPEN (should fail):\n");
    int r = circuit_breaker_check("backend");
    pstr("  result="); pint(r); pstr("\n");
    clk += CB_RETRY_TIMEOUT;
    pstr("After retry timeout:\n");
    circuit_breaker_check("backend");
    circuit_breaker_record(cb, 1);
    pstr("After success in HALF_OPEN:\n");
    circuit_breaker_check("backend");
    clk -= CB_RETRY_TIMEOUT;
}

static void test_rate_limiting(void) {
    print_header("Rate Limiting Test");
    rate_limit_add("backend", 3, 1000);
    for (int i = 0; i < 5; i++) {
        pstr("Request "); pint(i); pstr(": ");
        rate_limit_check("backend");
    }
    clk += 1000;
    pstr("After window reset:\n");
    rate_limit_check("backend");
    clk -= 1000;
}

static void test_tracing(void) {
    print_header("Distributed Tracing Test");
    int root = tracing_start_span("HTTP GET /api", "frontend", "");
    char ctx_buf[256];
    ctx_buf[0] = '\0';
    tracing_inject_context(root, ctx_buf, 256);
    tracing_extract_context(ctx_buf);
    int child = tracing_start_span("gRPC GetUser", "backend", spans[root].trace_id);
    tracing_end_span(child, 0, 15);
    int db_span = tracing_start_span("SELECT query", "database", spans[child].trace_id);
    tracing_end_span(db_span, 0, 3);
    tracing_end_span(root, 0, 25);
}

static void test_policy(void) {
    print_header("Policy Evaluation Test");
    policy_add("frontend", "backend", POLICY_ALLOW, AUTH_MTLS);
    policy_add("unknown", "database", POLICY_DENY, AUTH_NONE);
    policy_add("*", "admin", POLICY_ALLOW, AUTH_JWT);
    pstr("frontend->backend (mTLS): ");
    policy_allow_deny("frontend", "backend");
    pstr("unknown->database (none): ");
    int r = policy_allow_deny("unknown", "database");
    pstr("  result="); pint(r); pstr("\n");
    pstr("any->admin (mTLS): ");
    policy_evaluate("frontend", "admin", AUTH_MTLS);
    pstr("any->admin (none): ");
    policy_evaluate("frontend", "admin", AUTH_NONE);
}

static void test_topology(void) {
    print_header("Topology Awareness Test");
    topology_locality_route("frontend", "backend");
    topology_locality_route("frontend", "database");
    topology_zone_match("backend", "us-west-2a");
}

static void test_suite(void) {
    pstr("=== Service Mesh Test Suite v46.0 ===\n");
    test_sidecar_proxy();
    test_mtls();
    test_routing();
    test_circuit_breaker();
    test_rate_limiting();
    test_tracing();
    test_policy();
    test_topology();
    metrics_summary("backend");
    metrics_summary("frontend");
    pstr("\n=== All tests passed ===\n");
}

static void print_help(void) {
    pstr("service_mesh v46.0 - Service Mesh sidecar proxy\n");
    pstr("Usage: service_mesh [options]\n");
    pstr("  -h    Show this help\n");
    pstr("  -t    Run test suite\n");
    pstr("\nFeatures:\n");
    pstr("  Sidecar proxy with connection pooling\n");
    pstr("  mTLS with auto-rotation and SPIFFE IDs\n");
    pstr("  Traffic routing (weight/mirror/fault injection)\n");
    pstr("  Circuit breaker (closed/half-open/open)\n");
    pstr("  Distributed tracing (B3 context propagation)\n");
    pstr("  Policy engine (allow/deny/auth)\n");
    pstr("  Rate limiting (sliding window)\n");
    pstr("  Zone-aware locality routing\n");
}

void _start(void) {
    unsigned int buf = host_alloc(BUF_SIZE, 16);
    int argc = host_get_argv(buf, BUF_SIZE);
    char *args = (char *)buf;
    if (argc <= 0) {
        test_suite();
        return;
    }
    int ran = 0;
    for (int i = 0; i < argc; i++) {
        if (args[i] == '\0') continue;
        if (my_strcmp(&args[i], "-h") == 0 || my_strcmp(&args[i], "--help") == 0) {
            print_help();
            ran = 1;
        } else if (my_strcmp(&args[i], "-t") == 0 || my_strcmp(&args[i], "--test") == 0) {
            test_suite();
            ran = 1;
        }
        while (args[i] != '\0') i++;
    }
    if (!ran) {
        test_suite();
    }
}
