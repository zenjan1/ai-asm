/* edge_computing_admin: Edge computing administration system (v1.0)
 * Edge nodes, edge resources, edge applications, edge security, edge collaboration
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

#define MAX_NODE         16
#define MAX_RESOURCE     14
#define MAX_APPLICATION  12
#define MAX_SECURITY     10
#define MAX_COLLAB       10

typedef struct {
    int    node_id;
    int    node_type;
    int    node_category;
    int    edge_server;
    int    edge_gateway;
    int    edge_cache;
    int    year;
    int    active;
} node_t;

typedef struct {
    int    resource_id;
    int    resource_type;
    int    resource_category;
    int    compute_schedule;
    int    storage_manage;
    int    bandwidth_alloc;
    int    year;
    int    active;
} resource_t;

typedef struct {
    int    app_id;
    int    app_type;
    int    app_category;
    int    app_deploy;
    int    app_migrate;
    int    app_monitor;
    int    year;
    int    active;
} app_t;

typedef struct {
    int    security_id;
    int    security_type;
    int    security_category;
    int    node_auth;
    int    data_encrypt;
    int    access_control;
    int    year;
    int    active;
} security_t;

typedef struct {
    int    collab_id;
    int    collab_type;
    int    collab_category;
    int    cloud_edge;
    int    edge_edge;
    int    device_edge;
    int    year;
    int    active;
} collab_t;

typedef struct {
    int    n_node;
    int    n_resource;
    int    n_app;
    int    n_security;
    int    n_collab;
    int    total_server;
    int    total_compute;
    int    total_deploy;
    int    total_auth;
    int    total_cloud_edge;
} eca_state_t;

static node_t nodes[MAX_NODE];
static resource_t resources[MAX_RESOURCE];
static app_t apps[MAX_APPLICATION];
static security_t securities[MAX_SECURITY];
static collab_t collabs[MAX_COLLAB];
static eca_state_t eca;

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

int eca_init(void) {
    if (initialized) return -1;
    eca.n_node = 0; eca.n_resource = 0; eca.n_app = 0;
    eca.n_security = 0; eca.n_collab = 0;
    eca.total_server = 0; eca.total_compute = 0;
    eca.total_deploy = 0; eca.total_auth = 0;
    eca.total_cloud_edge = 0;
    for (int i = 0; i < MAX_NODE; i++) nodes[i].active = 0;
    for (int i = 0; i < MAX_RESOURCE; i++) resources[i].active = 0;
    for (int i = 0; i < MAX_APPLICATION; i++) apps[i].active = 0;
    for (int i = 0; i < MAX_SECURITY; i++) securities[i].active = 0;
    for (int i = 0; i < MAX_COLLAB; i++) collabs[i].active = 0;
    initialized = 1;
    print_str("[ECA] Edge computing initialized\n");
    return 0;
}

int eca_node(int nd_type, int cat, int server, int gateway, int cache, int year) {
    if (eca.n_node >= MAX_NODE) return -1;
    node_t* n = &nodes[eca.n_node];
    n->node_id = eca.n_node;
    n->node_type = nd_type;
    n->node_category = cat;
    n->edge_server = server;
    n->edge_gateway = gateway;
    n->edge_cache = cache;
    n->year = year;
    n->active = 1;
    eca.total_server += server;
    eca.n_node++;
    print_str("[ECA] Node "); print_int(eca.n_node - 1);
    print_str(" type="); print_int(nd_type);
    print_str(" cat="); print_int(cat);
    print_str(" srv="); print_int(server);
    print_str(" gtw="); print_int(gateway);
    print_str(" cch="); print_int(cache); print_str("\n");
    return eca.n_node - 1;
}

int eca_resource(int rsc_type, int cat, int compute, int storage, int bandwidth, int year) {
    if (eca.n_resource >= MAX_RESOURCE) return -1;
    resource_t* r = &resources[eca.n_resource];
    r->resource_id = eca.n_resource;
    r->resource_type = rsc_type;
    r->resource_category = cat;
    r->compute_schedule = compute;
    r->storage_manage = storage;
    r->bandwidth_alloc = bandwidth;
    r->year = year;
    r->active = 1;
    eca.total_compute += compute;
    eca.n_resource++;
    print_str("[ECA] Resource "); print_int(eca.n_resource - 1);
    print_str(" type="); print_int(rsc_type);
    print_str(" cat="); print_int(cat);
    print_str(" cmp="); print_int(compute);
    print_str(" stg="); print_int(storage);
    print_str(" bwd="); print_int(bandwidth); print_str("\n");
    return eca.n_resource - 1;
}

int eca_app(int ap_type, int cat, int deploy, int migrate, int monitor, int year) {
    if (eca.n_app >= MAX_APPLICATION) return -1;
    app_t* a = &apps[eca.n_app];
    a->app_id = eca.n_app;
    a->app_type = ap_type;
    a->app_category = cat;
    a->app_deploy = deploy;
    a->app_migrate = migrate;
    a->app_monitor = monitor;
    a->year = year;
    a->active = 1;
    eca.total_deploy += deploy;
    eca.n_app++;
    print_str("[ECA] App "); print_int(eca.n_app - 1);
    print_str(" type="); print_int(ap_type);
    print_str(" cat="); print_int(cat);
    print_str(" dpl="); print_int(deploy);
    print_str(" mgr="); print_int(migrate);
    print_str(" mon="); print_int(monitor); print_str("\n");
    return eca.n_app - 1;
}

int eca_security(int sec_type, int cat, int auth, int encrypt, int access, int year) {
    if (eca.n_security >= MAX_SECURITY) return -1;
    security_t* s = &securities[eca.n_security];
    s->security_id = eca.n_security;
    s->security_type = sec_type;
    s->security_category = cat;
    s->node_auth = auth;
    s->data_encrypt = encrypt;
    s->access_control = access;
    s->year = year;
    s->active = 1;
    eca.total_auth += auth;
    eca.n_security++;
    print_str("[ECA] Security "); print_int(eca.n_security - 1);
    print_str(" type="); print_int(sec_type);
    print_str(" cat="); print_int(cat);
    print_str(" auth="); print_int(auth);
    print_str(" enc="); print_int(encrypt);
    print_str(" acc="); print_int(access); print_str("\n");
    return eca.n_security - 1;
}

int eca_collab(int col_type, int cat, int cloud_edge, int edge_edge, int device_edge, int year) {
    if (eca.n_collab >= MAX_COLLAB) return -1;
    collab_t* c = &collabs[eca.n_collab];
    c->collab_id = eca.n_collab;
    c->collab_type = col_type;
    c->collab_category = cat;
    c->cloud_edge = cloud_edge;
    c->edge_edge = edge_edge;
    c->device_edge = device_edge;
    c->year = year;
    c->active = 1;
    eca.total_cloud_edge += cloud_edge;
    eca.n_collab++;
    print_str("[ECA] Collab "); print_int(eca.n_collab - 1);
    print_str(" type="); print_int(col_type);
    print_str(" cat="); print_int(cat);
    print_str(" ce="); print_int(cloud_edge);
    print_str(" ee="); print_int(edge_edge);
    print_str(" de="); print_int(device_edge); print_str("\n");
    return eca.n_collab - 1;
}

void eca_node_report(void) {
    print_str("[ECA] Node report:\n");
    print_str("  Edge node categories: "); print_int(eca.n_node); print_str("\n");
    print_str("  Total edge servers: "); print_int(eca.total_server); print_str("\n");
}

void eca_resource_report(void) {
    print_str("[ECA] Resource report:\n");
    print_str("  Edge resource categories: "); print_int(eca.n_resource); print_str("\n");
    print_str("  Total compute schedule: "); print_int(eca.total_compute); print_str("\n");
}

void eca_collab_report(void) {
    print_str("[ECA] Collab report:\n");
    print_str("  App categories: "); print_int(eca.n_app); print_str("\n");
    print_str("  Total app deploy: "); print_int(eca.total_deploy); print_str("\n");
    print_str("  Security categories: "); print_int(eca.n_security); print_str("\n");
    print_str("  Total node auth: "); print_int(eca.total_auth); print_str("\n");
    print_str("  Collab categories: "); print_int(eca.n_collab); print_str("\n");
    print_str("  Total cloud-edge: "); print_int(eca.total_cloud_edge); print_str("\n");
}

void eca_print_state(void) {
    print_str("[ECA] Nd="); print_int(eca.n_node);
    print_str(" Rs="); print_int(eca.n_resource);
    print_str(" Ap="); print_int(eca.n_app);
    print_str(" Sc="); print_int(eca.n_security);
    print_str(" Cl="); print_int(eca.n_collab);
    print_str("\n");
}

int main(void) {
    print_str("=== Edge Computing Admin Demo ===\n\n");
    eca_init();

    print_str("Edge nodes...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int srv = 40 + (i * 10);
        int gtw = 25 + (i * 6);
        int cch = 15 + (i * 4);
        int year = 2020 + (i % 5);
        eca_node(type, cat, srv, gtw, cch, year);
    }

    print_str("\nEdge resources...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cmp = 60 + (i * 15);
        int stg = 80 + (i * 20);
        int bwd = 40 + (i * 10);
        int year = 2021 + (i % 4);
        eca_resource(type, cat, cmp, stg, bwd, year);
    }

    print_str("\nEdge applications...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int dpl = 30 + (i * 8);
        int mgr = 20 + (i * 5);
        int mon = 15 + (i * 4);
        int year = 2022 + (i % 3);
        eca_app(type, cat, dpl, mgr, mon, year);
    }

    print_str("\nEdge security...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int auth = 25 + (i * 6);
        int enc = 20 + (i * 5);
        int acc = 15 + (i * 4);
        int year = 2023 + (i % 2);
        eca_security(type, cat, auth, enc, acc, year);
    }

    print_str("\nEdge collaboration...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ce = 35 + (i * 9);
        int ee = 25 + (i * 6);
        int de = 18 + (i * 4);
        int year = 2024;
        eca_collab(type, cat, ce, ee, de, year);
    }

    print_str("\nNode report...\n");
    eca_node_report();

    print_str("\nResource report...\n");
    eca_resource_report();

    print_str("\nCollab report...\n");
    eca_collab_report();

    print_str("\nFinal state...\n");
    eca_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
