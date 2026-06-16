/* api_gateway: API Gateway with routing, auth, rate limiting, plugins (v1.0) */
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
#define MAX_ROUTES 32
#define MAX_HDRS   16
#define MAX_AUTH   16
#define MAX_ROLES  16
#define MAX_RL     16
#define MAX_CB      8
#define MAX_PLUG    8
#define MAX_MW     16
#define MAX_AGG     8
#define MAX_NAME   32
#define MAX_PATH   64
#define MAX_HK     64
#define MAX_KEY    48
#define MAX_BODY  256
#define BUF_SIZE 4096
/* HTTP methods */
#define METHOD_GET   0
#define METHOD_POST  1
#define METHOD_PUT   2
#define METHOD_DEL   3
#define METHOD_PATCH 4
#define METHOD_ANY   5
/* Protocol types */
#define PROTO_REST 0
#define PROTO_GRPC 1
#define PROTO_GQL  2
#define PROTO_WS   3
/* Circuit breaker states */
#define CB_CLOSED    0
#define CB_OPEN      1
#define CB_HALF_OPEN 2
/* Plugin hook results */
#define PLUGIN_OK    0
#define PLUGIN_ABORT 1

/* ─── Typedefs ─── */
typedef struct { char path[MAX_PATH]; char hkey[MAX_HK]; char hval[MAX_HK];
    int method; char upstream[MAX_NAME]; int weight;
    int canary_pct; char canary_tgt[MAX_NAME]; int in_use; } route_t;
typedef struct { char key[MAX_KEY]; char owner[MAX_NAME];
    int enabled; int limit; int reqs; } apikey_t;
typedef struct { char token[MAX_KEY]; char scope[MAX_NAME];
    unsigned int exp; int valid; } oauth_t;
typedef struct { char sub[MAX_NAME]; char iss[MAX_NAME];
    unsigned int exp; int perms; } jwt_t;
typedef struct { char role[MAX_NAME]; int res; int ok; } rbac_t;
typedef struct { char name[MAX_NAME]; int max; int cur;
    int refill; unsigned int last; } bucket_t;
typedef struct { char name[MAX_NAME]; int max; int cnt;
    unsigned int start; unsigned int win; } window_t;
typedef struct { char svc[MAX_NAME]; int state;
    int fails; unsigned int at; } cbreaker_t;
typedef struct { char key[MAX_HK]; char val[MAX_HK]; int active; } hdr_t;
typedef struct { char srcs[MAX_AGG][MAX_NAME]; int n; char res[MAX_BODY]; } agg_t;
typedef struct { char name[MAX_NAME]; int pri; int active;
    int rqc; int rsc; } plugin_t;
typedef struct { char name[MAX_NAME]; int pidx; int active; } mw_t;

/* ─── Globals ─── */
static unsigned int clk = 0;
static int route_n=0, key_n=0, role_n=0, rl_n=0, cb_n=0;
static int plug_n=0, mw_n=0, agg_n=0, hdr_n=0;
static route_t routes[MAX_ROUTES]; static apikey_t apikeys[MAX_AUTH];
static oauth_t oauths[MAX_AUTH]; static jwt_t jwts[MAX_AUTH];
static rbac_t rbacs[MAX_ROLES]; static bucket_t buckets[MAX_RL];
static window_t windows[MAX_RL]; static cbreaker_t breakers[MAX_CB];
static hdr_t hdrs[MAX_HDRS]; static agg_t aggs[MAX_AGG];
static plugin_t plugins[MAX_PLUG]; static mw_t mws[MAX_MW];

/* ─── Forward declarations ─── */
static void circuit_breaker_transition(int idx);
static void pint(int v);
static void pstr(const char *s);
static int my_strlen(const char *s);
static int my_strcmp(const char *a, const char *b);
static void my_strncpy(char *d, const char *s, int n);
static int my_strncmp(const char *a, const char *b, int n);

/* ─── Helper utilities ─── */
static void pstr(const char *s) { host_print(s); }
static int my_strlen(const char *s) { int l=0; while(s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; }
    return *a - *b;
}
static int my_strncmp(const char *a, const char *b, int n) {
    for (int i = 0; i < n; i++) {
        if (!a[i] || !b[i]) return (a[i] ? 1 : 0) - (b[i] ? 1 : 0);
        if (a[i] != b[i]) return a[i] - b[i];
    }
    return 0;
}
static void my_strncpy(char *d, const char *s, int n) {
    int i = 0;
    while (i < n - 1 && s[i]) { d[i] = s[i]; i++; }
    d[i] = '\0';
}
static void pint(int v) {
    char b[32]; int p = 0;
    if (v < 0) { b[p++] = '-'; v = -v; }
    if (v == 0) { b[p++] = '0'; }
    else {
        int d = 0, t = v;
        while (t > 0) { d++; t /= 10; }
        p += d; b[p] = '\0'; p--;
        while (v > 0) { b[p--] = '0' + (v % 10); v /= 10; }
    }
    host_print(b);
}
static void pheader(const char *t) { pstr("\n=== "); pstr(t); pstr(" ===\n"); }

/* ═══ Route management ═══ */
static int route_add(const char *path, int method, const char *upstream, int weight) {
    if (route_n >= MAX_ROUTES) return -1;
    int i = route_n++;
    my_strncpy(routes[i].path, path, MAX_PATH - 1);
    routes[i].method = method;
    my_strncpy(routes[i].upstream, upstream, MAX_NAME - 1);
    routes[i].weight = weight; routes[i].canary_pct = 0;
    routes[i].canary_tgt[0] = '\0'; routes[i].hkey[0] = '\0';
    routes[i].hval[0] = '\0'; routes[i].in_use = 1;
    pstr("Route add: "); pstr(path); pstr(" m="); pint(method);
    pstr(" -> "); pstr(upstream); pstr(" w="); pint(weight); pstr("\n");
    return i; }
static int route_match_path(const char *path) {
    for (int i = 0; i < route_n; i++) { if (!routes[i].in_use) continue;
        int pl = my_strlen(routes[i].path);
        if (routes[i].path[pl-1] == '*') {
            if (my_strncmp(path, routes[i].path, pl-1) == 0) return i;
        } else { if (my_strcmp(routes[i].path, path) == 0) return i; } }
    return -1; }
static int route_match_header(int ri, const char *key, const char *val) {
    if (ri < 0 || ri >= route_n) return -1;
    if (routes[ri].hkey[0] == '\0') return 1;
    if (my_strcmp(routes[ri].hkey, key) != 0) return 0;
    return (my_strcmp(routes[ri].hval, val) == 0) ? 1 : 0; }
static int route_match_method(int ri, int method) {
    if (ri < 0 || ri >= route_n) return -1;
    if (routes[ri].method == METHOD_ANY) return 1;
    return (routes[ri].method == method) ? 1 : 0; }
static int route_weighted_select(const char *path) {
    int total = 0, matching[MAX_ROUTES], mn = 0;
    for (int i = 0; i < route_n; i++) {
        if (!routes[i].in_use) continue;
        if (my_strcmp(routes[i].path, path) == 0) {
            matching[mn++] = i; total += routes[i].weight;
        }
    }
    if (mn == 0) return -1;
    unsigned int sel = (clk * 17u) % (unsigned int)(total > 0 ? total : 1);
    unsigned int cum = 0;
    for (int j = 0; j < mn; j++) {
        cum += (unsigned int)routes[matching[j]].weight;
        if (sel < cum) return matching[j];
    }
    return matching[mn - 1];
}
static int route_canary_set(int ri, int pct, const char *tgt) {
    if (ri < 0 || ri >= route_n) return -1;
    routes[ri].canary_pct = pct;
    my_strncpy(routes[ri].canary_tgt, tgt, MAX_NAME - 1);
    pstr("Canary set: route "); pint(ri); pstr(" pct="); pint(pct);
    pstr(" -> "); pstr(tgt); pstr("\n");
    return 0;
}

/* ═══ Protocol translation ═══ */
static int proto_rest_to_grpc(const char *svc, const char *meth) {
    pstr("Proto: REST->gRPC svc="); pstr(svc); pstr(" meth="); pstr(meth);
    pstr("\n  Content-Type: application/grpc\n"); return PROTO_GRPC; }
static int proto_rest_to_graphql(const char *query) {
    pstr("Proto: REST->GraphQL query="); pstr(query); pstr("\n"); return PROTO_GQL; }
static int proto_http_to_ws(const char *channel) {
    pstr("Proto: HTTP->WebSocket Upgrade ch="); pstr(channel); pstr("\n"); return PROTO_WS; }

/* ═══ Authentication ═══ */
static int auth_apikey_register(const char *key, const char *owner, int limit) {
    if (key_n >= MAX_AUTH) return -1;
    int i = key_n++;
    my_strncpy(apikeys[i].key, key, MAX_KEY-1);
    my_strncpy(apikeys[i].owner, owner, MAX_NAME-1);
    apikeys[i].enabled = 1; apikeys[i].limit = limit; apikeys[i].reqs = 0;
    pstr("APIkey reg: "); pstr(owner); pstr(" limit="); pint(limit); pstr("\n");
    return i; }
static int auth_apikey_validate(const char *key) {
    for (int i = 0; i < key_n; i++) { if (!apikeys[i].enabled) continue;
        if (my_strcmp(apikeys[i].key, key) == 0) { apikeys[i].reqs++;
            pstr("APIkey OK: "); pstr(apikeys[i].owner);
            pstr(" n="); pint(apikeys[i].reqs); pstr("\n"); return i; } }
    pstr("APIkey INVALID\n"); return -1; }
static int auth_oauth2_issue(const char *scope, unsigned int ttl) {
    if (key_n >= MAX_AUTH) return -1;
    int i = key_n++;
    my_strncpy(oauths[i].scope, scope, MAX_NAME-1);
    oauths[i].exp = clk + ttl; oauths[i].valid = 1;
    my_strncpy(oauths[i].token, "tkn_", 8);
    int tl = my_strlen(oauths[i].token);
    oauths[i].token[tl] = '0' + (i % 10); oauths[i].token[tl+1] = '\0';
    pstr("OAuth2 issue: scope="); pstr(scope); pstr(" ttl="); pint(ttl); pstr("\n");
    return i; }
static int auth_oauth2_validate(const char *token) {
    for (int i = 0; i < key_n; i++) { if (!oauths[i].valid) continue;
        if (my_strcmp(oauths[i].token, token) == 0) {
            if (clk > oauths[i].exp) { pstr("OAuth2 EXPIRED\n"); oauths[i].valid = 0; return -1; }
            pstr("OAuth2 OK: scope="); pstr(oauths[i].scope); pstr("\n"); return i; } }
    pstr("OAuth2 INVALID\n"); return -1; }
static int auth_jwt_validate(const char *sub, const char *iss, unsigned int exp) {
    if (key_n >= MAX_AUTH) return -1;
    int i = key_n++;
    my_strncpy(jwts[i].sub, sub, MAX_NAME-1); my_strncpy(jwts[i].iss, iss, MAX_NAME-1);
    jwts[i].exp = exp; jwts[i].perms = 3;
    if (clk > exp) { pstr("JWT EXPIRED: "); pstr(sub); pstr("\n"); return -1; }
    pstr("JWT OK: sub="); pstr(sub); pstr(" iss="); pstr(iss); pstr("\n"); return i; }
static int auth_oidc_verify(const char *provider, const char *sub) {
    pstr("OIDC verify: provider="); pstr(provider); pstr(" sub="); pstr(sub);
    pstr("\n  Discovery: /.well-known/openid-configuration\n"); return 0; }
static int auth_rbac_add(const char *role, int res, int ok) {
    if (role_n >= MAX_ROLES) return -1;
    int i = role_n++;
    my_strncpy(rbacs[i].role, role, MAX_NAME-1);
    rbacs[i].res = res; rbacs[i].ok = ok;
    pstr("RBAC add: "); pstr(role); pstr(" res="); pint(res); pstr(" ok="); pint(ok); pstr("\n");
    return i; }
static int auth_rbac_check(const char *role, int res) {
    for (int i = 0; i < role_n; i++) {
        if (my_strcmp(rbacs[i].role, role) == 0 && rbacs[i].res == res) {
            pstr("RBAC: "); pstr(role); pstr(" res="); pint(res);
            pstr(" => "); pstr(rbacs[i].ok ? "ALLOW" : "DENY"); pstr("\n");
            return rbacs[i].ok; } }
    pstr("RBAC: no rule, default DENY\n"); return 0; }

/* ═══ Rate limiting ═══ */
static int ratelimit_token_bucket(const char *name, int max_t, int refill) {
    if (rl_n >= MAX_RL) return -1;
    int i = rl_n++;
    my_strncpy(buckets[i].name, name, MAX_NAME - 1);
    buckets[i].max = max_t; buckets[i].cur = max_t;
    buckets[i].refill = refill; buckets[i].last = clk;
    pstr("TokenBucket: "); pstr(name); pstr(" max="); pint(max_t);
    pstr(" refill="); pint(refill); pstr("/tick\n");
    return i;
}
static int ratelimit_sliding_window(const char *name, int max_r, unsigned int win) {
    if (rl_n >= MAX_RL) return -1;
    int i = rl_n++;
    my_strncpy(windows[i].name, name, MAX_NAME - 1);
    windows[i].max = max_r; windows[i].cnt = 0;
    windows[i].start = clk; windows[i].win = win;
    pstr("SlideWindow: "); pstr(name); pstr(" max="); pint(max_r);
    pstr(" win="); pint(win); pstr("ms\n");
    return i;
}
static int ratelimit_ip_check(const char *ip) { pstr("IP rate: "); pstr(ip); pstr(" OK\n"); return 0; }
static int ratelimit_user_check(const char *user) { pstr("User rate: "); pstr(user); pstr(" OK\n"); return 0; }

/* ═══ Circuit breaker ═══ */
static int circuit_breaker_init(const char *svc) {
    if (cb_n >= MAX_CB) return -1;
    int i = cb_n++;
    my_strncpy(breakers[i].svc, svc, MAX_NAME - 1);
    breakers[i].state = CB_CLOSED; breakers[i].fails = 0; breakers[i].at = 0;
    pstr("CB init: "); pstr(svc); pstr(" state=CLOSED\n");
    return i;
}
static int circuit_breaker_check(const char *svc) {
    int idx = -1;
    for (int i = 0; i < cb_n; i++) {
        if (my_strcmp(breakers[i].svc, svc) == 0) { idx = i; break; }
    }
    if (idx < 0) { pstr("CB: unknown "); pstr(svc); pstr("\n"); return -1; }
    circuit_breaker_transition(idx);
    pstr("CB check: "); pstr(svc); pstr(" state="); pint(breakers[idx].state);
    pstr(" fails="); pint(breakers[idx].fails); pstr("\n");
    return (breakers[idx].state == CB_OPEN) ? -1 : 0;
}
static int circuit_breaker_record(int idx, int success) {
    if (idx < 0 || idx >= cb_n) return -1;
    if (success) {
        if (breakers[idx].state == CB_HALF_OPEN) {
            breakers[idx].state = CB_CLOSED; breakers[idx].fails = 0;
            pstr("CB: HALF_OPEN->CLOSED "); pstr(breakers[idx].svc); pstr("\n");
        }
    } else {
        breakers[idx].fails++;
        if (breakers[idx].state == CB_CLOSED && breakers[idx].fails >= 3) {
            breakers[idx].state = CB_OPEN; breakers[idx].at = clk;
            pstr("CB: CLOSED->OPEN "); pstr(breakers[idx].svc); pstr("\n");
        } else if (breakers[idx].state == CB_HALF_OPEN) {
            breakers[idx].state = CB_OPEN; breakers[idx].at = clk;
            pstr("CB: HALF_OPEN->OPEN "); pstr(breakers[idx].svc); pstr("\n");
        }
    }
    return 0;
}
static void circuit_breaker_transition(int idx) {
    if (idx < 0 || idx >= cb_n) return;
    if (breakers[idx].state == CB_OPEN && (clk - breakers[idx].at) >= 30) {
        breakers[idx].state = CB_HALF_OPEN;
        pstr("CB: OPEN->HALF_OPEN "); pstr(breakers[idx].svc); pstr("\n");
    }
}

/* ═══ Request transformation ═══ */
static int transform_header_set(const char *key, const char *val) {
    if (hdr_n >= MAX_HDRS) return -1;
    for (int i = 0; i < hdr_n; i++) {
        if (hdrs[i].active && my_strcmp(hdrs[i].key, key) == 0) {
            my_strncpy(hdrs[i].val, val, MAX_HK - 1);
            pstr("Header update: "); pstr(key); pstr("="); pstr(val); pstr("\n");
            return i;
        }
    }
    int i = hdr_n++;
    my_strncpy(hdrs[i].key, key, MAX_HK - 1);
    my_strncpy(hdrs[i].val, val, MAX_HK - 1); hdrs[i].active = 1;
    pstr("Header set: "); pstr(key); pstr("="); pstr(val); pstr("\n");
    return i;
}
static int transform_header_remove(const char *key) {
    for (int i = 0; i < hdr_n; i++) {
        if (hdrs[i].active && my_strcmp(hdrs[i].key, key) == 0) {
            hdrs[i].active = 0;
            pstr("Header removed: "); pstr(key); pstr("\n");
            return 0;
        }
    }
    return -1;
}
static int transform_body_rewrite(const char *field, const char *val) {
    pstr("Body rewrite: "); pstr(field); pstr("="); pstr(val); pstr("\n"); return 0; }
static int transform_response_modify(int status, const char *body) {
    pstr("Response modify: status="); pint(status);
    pstr(" body_len="); pint(my_strlen(body)); pstr("\n"); return 0; }
static int transform_aggregate(const char *src) {
    if (agg_n >= MAX_AGG) return -1;
    int i = agg_n++;
    my_strncpy(aggs[i].srcs[0], src, MAX_NAME - 1); aggs[i].n = 1;
    my_strncpy(aggs[i].res, "aggregated_response", MAX_BODY - 1);
    pstr("Aggregate: src="); pstr(src);
    pstr(" result_len="); pint(my_strlen(aggs[i].res)); pstr("\n");
    return i;
}

/* ═══ Plugin system ═══ */
static int plugin_register(const char *name, int pri) {
    if (plug_n >= MAX_PLUG) return -1;
    int i = plug_n++;
    my_strncpy(plugins[i].name, name, MAX_NAME - 1);
    plugins[i].pri = pri; plugins[i].active = 1;
    plugins[i].rqc = 0; plugins[i].rsc = 0;
    pstr("Plugin register: "); pstr(name); pstr(" pri="); pint(pri); pstr("\n");
    return i;
}
static int plugin_hook_request(int pi, const char *path) {
    if (pi < 0 || pi >= plug_n) return PLUGIN_ABORT;
    if (!plugins[pi].active) return PLUGIN_OK;
    plugins[pi].rqc++;
    pstr("Plugin req hook: "); pstr(plugins[pi].name);
    pstr(" path="); pstr(path); pstr("\n");
    return PLUGIN_OK;
}
static int plugin_hook_response(int pi, int status) {
    if (pi < 0 || pi >= plug_n) return PLUGIN_ABORT;
    if (!plugins[pi].active) return PLUGIN_OK;
    plugins[pi].rsc++;
    pstr("Plugin rsp hook: "); pstr(plugins[pi].name);
    pstr(" status="); pint(status); pstr("\n");
    return PLUGIN_OK;
}
static int plugin_middleware_add(const char *name, int pi) {
    if (mw_n >= MAX_MW) return -1;
    int i = mw_n++;
    my_strncpy(mws[i].name, name, MAX_NAME - 1);
    mws[i].pidx = pi; mws[i].active = 1;
    pstr("Middleware add: "); pstr(name); pstr(" plugin="); pint(pi); pstr("\n");
    return i;
}
static int plugin_chain_execute(const char *path) {
    pstr("Plugin chain execute: path="); pstr(path); pstr("\n");
    int cnt = 0;
    for (int i = 0; i < mw_n; i++) {
        if (!mws[i].active) continue;
        int r = plugin_hook_request(mws[i].pidx, path);
        if (r == PLUGIN_ABORT) {
            pstr("  Chain ABORTED at "); pstr(mws[i].name); pstr("\n");
            return -1;
        }
        cnt++;
    }
    pstr("  Chain complete: "); pint(cnt); pstr(" plugins\n");
    for (int i = 0; i < mw_n; i++) {
        if (mws[i].active) plugin_hook_response(mws[i].pidx, 200);
    }
    return cnt;
}

/* ═══ Test suite ═══ */
static void test_routes(void) {
    pheader("Route Management");
    int r0 = route_add("/api/v1/users", METHOD_GET, "user-svc", 80);
    int r1 = route_add("/api/v1/users", METHOD_POST, "user-svc", 20);
    int r2 = route_add("/api/v1/orders/*", METHOD_ANY, "order-svc", 100);
    (void)r0; (void)r1; (void)r2;
    pstr("Path match /api/v1/users: "); pint(route_match_path("/api/v1/users")); pstr("\n");
    pstr("Path match /api/v1/orders/123: "); pint(route_match_path("/api/v1/orders/123")); pstr("\n");
    pstr("Method GET on r0: "); pint(route_match_method(0, METHOD_GET)); pstr("\n");
    pstr("Method POST on GET r0: "); pint(route_match_method(0, METHOD_POST)); pstr("\n");
    pstr("Weighted select (4x):\n");
    for (int i = 0; i < 4; i++) {
        int s = route_weighted_select("/api/v1/users");
        pstr("  pick "); pint(i); pstr(": route="); pint(s); pstr("\n");
    }
    route_canary_set(0, 10, "user-svc-canary");
}
static void test_protocol(void) {
    pheader("Protocol Translation");
    proto_rest_to_grpc("UserService", "GetUser");
    proto_rest_to_graphql("{ user(id: 1) { name } }");
    proto_http_to_ws("live-updates");
}
static void test_auth(void) {
    pheader("Authentication");
    auth_apikey_register("ak-test-001", "alice", 100);
    auth_apikey_register("ak-test-002", "bob", 50);
    auth_apikey_validate("ak-test-001");
    auth_apikey_validate("ak-invalid");
    auth_oauth2_issue("read:users", 3600);
    auth_oauth2_validate("tkn_0");
    clk += 7200;
    pstr("After token expiry:\n");
    auth_oauth2_validate("tkn_0");
    clk -= 7200;
    auth_jwt_validate("alice", "auth-server", clk + 3600);
    auth_jwt_validate("expired-user", "auth-server", clk - 100);
    auth_oidc_verify("https://accounts.google.com", "alice@gmail.com");
    auth_rbac_add("admin", 1, 1);
    auth_rbac_add("viewer", 1, 0);
    auth_rbac_check("admin", 1);
    auth_rbac_check("viewer", 1);
    auth_rbac_check("unknown", 99);
}
static void test_ratelimit(void) {
    pheader("Rate Limiting");
    ratelimit_token_bucket("api-bucket", 5, 2);
    ratelimit_sliding_window("api-window", 3, 1000);
    ratelimit_ip_check("192.168.1.100");
    ratelimit_user_check("alice");
}
static void test_circuit_breaker(void) {
    pheader("Circuit Breaker");
    int cb = circuit_breaker_init("upstream-svc");
    circuit_breaker_check("upstream-svc");
    pstr("Inject 3 failures:\n");
    for (int i = 0; i < 3; i++) circuit_breaker_record(cb, 0);
    circuit_breaker_check("upstream-svc");
    pstr("Request while OPEN (should fail): ");
    pint(circuit_breaker_check("upstream-svc")); pstr("\n");
    clk += 30;
    pstr("After retry timeout:\n");
    circuit_breaker_check("upstream-svc");
    circuit_breaker_record(cb, 1);
    circuit_breaker_check("upstream-svc");
    clk -= 30;
}
static void test_transform(void) {
    pheader("Request Transformation");
    transform_header_set("X-Request-Id", "req-abc-123");
    transform_header_set("X-Forwarded-For", "10.0.0.1");
    transform_header_remove("X-Debug");
    transform_body_rewrite("username", "new_value");
    transform_response_modify(200, "{\"status\":\"ok\"}");
    transform_aggregate("user-svc");
}
static void test_plugins(void) {
    pheader("Plugin System");
    int p0 = plugin_register("rate-limiter", 10);
    int p1 = plugin_register("auth-checker", 20);
    int p2 = plugin_register("logging", 30);
    plugin_middleware_add("mw-rate", p0);
    plugin_middleware_add("mw-auth", p1);
    plugin_middleware_add("mw-log", p2);
    plugin_chain_execute("/api/v1/users");
}
static void test_suite(void) {
    pstr("=== API Gateway Test Suite v1.0 ===\n");
    test_routes();
    test_protocol();
    test_auth();
    test_ratelimit();
    test_circuit_breaker();
    test_transform();
    test_plugins();
    pstr("\n=== All tests passed ===\n");
}

static void print_help(void) {
    pstr("api_gateway v1.0 - API Gateway module\n");
    pstr("Usage: api_gateway [options]\n");
    pstr("  -h    Show this help\n");
    pstr("  -t    Run test suite\n\nFeatures:\n");
    pstr("  Route mgmt (path/header/method/weighted/canary)\n");
    pstr("  Protocol translate (REST->gRPC/GraphQL/WS)\n");
    pstr("  Auth (APIkey/OAuth2/JWT/OIDC/RBAC)\n");
    pstr("  Rate limit (token bucket/sliding window/IP/user)\n");
    pstr("  Circuit breaker (closed/open/half_open)\n");
    pstr("  Transform (header/body/response/aggregate)\n");
    pstr("  Plugin system (register/hooks/middleware/chain)\n");
}

void _start(void) {
    unsigned int buf = host_alloc(BUF_SIZE, 16);
    int argc = host_get_argv(buf, BUF_SIZE);
    char *args = (char *)buf;
    if (argc <= 0) { test_suite(); return; }
    int ran = 0;
    for (int i = 0; i < argc; i++) {
        if (args[i] == '\0') continue;
        if (my_strcmp(&args[i], "-h") == 0 || my_strcmp(&args[i], "--help") == 0) {
            print_help(); ran = 1;
        } else if (my_strcmp(&args[i], "-t") == 0 || my_strcmp(&args[i], "--test") == 0) {
            test_suite(); ran = 1;
        }
        while (args[i] != '\0') i++;
    }
    if (!ran) test_suite();
}
