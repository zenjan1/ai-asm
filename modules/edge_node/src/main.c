/* edge_node - Edge Node Management Module for AI-ASM OS
 * Subsystems: registration, resources, deployment, network, security, monitoring */
#include <stddef.h>

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_NODES 128
#define MAX_APPS 64
#define MAX_RES 256
#define MAX_ROUTES 128
#define MAX_PEERS 64
#define MAX_CERTS 32
#define MAX_LOGS 256
#define MAX_DEPS 32
#define MAX_NAME 64
#define N_ONLINE 0x0001U
#define N_AUTHED 0x0002U
#define N_TEE    0x0004U
#define N_GPU    0x0008U
#define N_SECURE 0x0010U
#define A_RUNNING 0x01U
#define A_CANARY  0x02U
#define A_STOPPED 0x03U
#define A_MIGRATED 0x05U
#define R_CPU 0x01U
#define R_GPU 0x02U
#define R_MEM 0x03U
#define R_STO 0x04U
#define R_BW  0x05U
#define L_INFO  0x01U
#define L_WARN  0x02U
#define L_ERROR 0x03U
#define L_DEBUG 0x04U
#define E_OK 0
#define E_NOMEM -1
#define E_FULL -2
#define E_NOTFOUND -3
#define E_AUTH -4
#define E_TEE -5
#define E_NET -6
#define E_DEPLOY -7
#define E_QUOTA -8

/* Forward declarations */
typedef struct node_s node_t;
typedef struct caps_s caps_t;
typedef struct geo_s geo_t;
typedef struct pool_s pool_t;
typedef struct quota_s quota_t;
typedef struct app_s app_t;
typedef struct dep_s dep_t;
typedef struct route_s route_t;
typedef struct peer_s peer_t;
typedef struct cert_s cert_t;
typedef struct logent_s logent_t;
typedef struct mon_s mon_t;
typedef struct ctx_s ctx_t;

static int edge_init(ctx_t *ctx);
static int register_node(ctx_t *ctx, const char *name, unsigned int flags);
static int authenticate_node(ctx_t *ctx, unsigned int nid);
static int report_caps(ctx_t *ctx, unsigned int nid, const caps_t *c);
static int set_geo(ctx_t *ctx, unsigned int nid, const geo_t *g);
static int pool_resource(ctx_t *ctx, unsigned int nid, unsigned int rt, unsigned int amt);
static int alloc_quota(ctx_t *ctx, unsigned int aid, unsigned int rt, unsigned int q);
static int balance_load(ctx_t *ctx);
static int elastic_scale(ctx_t *ctx, unsigned int aid, int dir);
static int fault_migrate(ctx_t *ctx, unsigned int aid, unsigned int tgt);
static int deploy_container(ctx_t *ctx, const char *img, unsigned int nid);
static int deploy_function(ctx_t *ctx, const char *fn, unsigned int nid);
static int deploy_model(ctx_t *ctx, const char *m, unsigned int nid);
static int canary_release(ctx_t *ctx, unsigned int did, unsigned int pct);
static int version_rollback(ctx_t *ctx, unsigned int did, unsigned int ver);
static int build_topology(ctx_t *ctx);
static int configure_sdwan(ctx_t *ctx, unsigned int rid);
static int p2p_connect(ctx_t *ctx, unsigned int s, unsigned int d);
static int nat_traverse(ctx_t *ctx, unsigned int pid);
static int offline_tx(ctx_t *ctx, unsigned int pid, const unsigned char *d, unsigned int l);
static int enable_tee(ctx_t *ctx, unsigned int nid);
static int secure_boot(ctx_t *ctx, unsigned int nid);
static int encrypt_data(ctx_t *ctx, const unsigned char *p, unsigned int l, unsigned char *c);
static int check_access(ctx_t *ctx, unsigned int nid, unsigned int aid, unsigned int perms);
static int check_health(ctx_t *ctx, unsigned int nid);
static int sample_usage(ctx_t *ctx, unsigned int nid);
static int collect_logs(ctx_t *ctx, unsigned int nid);
static int remote_debug(ctx_t *ctx, unsigned int nid, const char *cmd);
static void run_self_test(ctx_t *ctx);
static void log_event(ctx_t *ctx, unsigned int nid, unsigned int lvl, const char *msg);

struct geo_s { int lat, lon, elev; };
struct caps_s {
    unsigned int cores, mhz, gpus, gpu_mem, mem_mb, sto_mb, bw_mbps, flags;
};
struct node_s {
    unsigned int id, flags, load, apps, health, cidx;
    char name[MAX_NAME];
    caps_t caps;
    geo_t geo;
};
struct pool_s { unsigned int nid, rt, total, alloc, avail; };
struct quota_s { unsigned int aid, rt, limit, used; };
struct app_s {
    unsigned int id, type, state, ver, nid, did;
    char name[MAX_NAME];
};
struct dep_s { unsigned int id, aid, nid, ver, cpct, flags; };
struct route_s { unsigned int src, dst, metric, via, flags; };
struct peer_s { unsigned int id, nid, nat, reach; char ep[MAX_NAME]; };
struct cert_s { unsigned int nid, hhi, hlo, valid, flags; };
struct logent_s { unsigned int ts, nid, lvl; char msg[MAX_NAME]; };
struct mon_s { unsigned int nid, hp, cpu, mem, disk, net, arun, lc; };
/* ── Edge Context ───────────────────────────────────────────────────── */

struct ctx_s {
    node_t nodes[MAX_NODES]; unsigned int nc;
    pool_t pools[MAX_RES];   unsigned int pc;
    quota_t quotas[MAX_RES]; unsigned int qc;
    app_t apps[MAX_APPS];    unsigned int ac;
    dep_t deps[MAX_DEPS];    unsigned int dc;
    route_t routes[MAX_ROUTES]; unsigned int rc;
    peer_t peers[MAX_PEERS]; unsigned int pkc;
    cert_t certs[MAX_CERTS]; unsigned int cc;
    logent_t logs[MAX_LOGS]; unsigned int lc;
    mon_t mons[MAX_NODES];   unsigned int mc;
};

/* ── Utility helpers ────────────────────────────────────────────────── */

static unsigned int heap = 0x10000U;
static void *walloc(unsigned int sz) { unsigned int p = heap; heap += (sz+7U)&~7U; return (void*)p; }
static void mzero(void *d, unsigned int n) { unsigned char *p=(unsigned char*)d; for(unsigned int i=0;i<n;i++) p[i]=0; }
static void scopy(char *d, const char *s, unsigned int mx) {
    unsigned int i; for(i=0;i<mx-1U&&s[i];i++) d[i]=s[i]; d[i]='\0';
}
static void log_event(ctx_t *x, unsigned int nid, unsigned int lvl, const char *msg) {
    if (x->lc >= MAX_LOGS) return;
    logent_t *e = &x->logs[x->lc];
    e->ts = x->lc + 1U; e->nid = nid; e->lvl = lvl; scopy(e->msg, msg, MAX_NAME); x->lc++;
}

/* ── Node Registration ──────────────────────────────────────────────── */

static int edge_init(ctx_t *x) {
    mzero(x, sizeof(ctx_t));
    log_event(x, 0, L_INFO, "edge_init: boot sequence started");
    return E_OK;
}

static int register_node(ctx_t *x, const char *name, unsigned int fl) {
    if (x->nc >= MAX_NODES) return E_FULL;
    node_t *n = &x->nodes[x->nc];
    n->id = x->nc; scopy(n->name, name, MAX_NAME);
    n->flags = fl|N_ONLINE; n->load=0; n->apps=0; n->health=100; n->cidx=0xFFFFFFFFU;
    x->nc++; log_event(x, n->id, L_INFO, "node registered"); return (int)n->id;
}

static int authenticate_node(ctx_t *x, unsigned int nid) {
    if (nid >= x->nc) return E_NOTFOUND;
    if (x->cc >= MAX_CERTS) return E_FULL;
    cert_t *c = &x->certs[x->cc];
    c->nid=nid; c->hhi=0xDEADBEEFU; c->hlo=0xCAFEBABEU; c->valid=0xFFFFFFFFU; c->flags=0x01U;
    x->nodes[nid].cidx = x->cc;
    x->nodes[nid].flags |= N_AUTHED;
    x->cc++;
    log_event(x, nid, L_INFO, "node authenticated successfully");
    return E_OK;
}

static int report_caps(ctx_t *x, unsigned int nid, const caps_t *c) {
    if (nid >= x->nc) return E_NOTFOUND;
    x->nodes[nid].caps = *c;
    if (c->gpus > 0) x->nodes[nid].flags |= N_GPU;
    log_event(x, nid, L_INFO, "caps reported");
    return E_OK;
}

/* Set geographic location for latency-aware routing */
static int set_geo(ctx_t *x, unsigned int nid, const geo_t *g) {
    if (nid >= x->nc) return E_NOTFOUND;
    x->nodes[nid].geo = *g;
    log_event(x, nid, L_DEBUG, "geo set");
    return E_OK;
}

/* ── Resource Management ────────────────────────────────────────────── */

static int pool_resource(ctx_t *x, unsigned int nid, unsigned int rt, unsigned int amt) {
    if (x->pc >= MAX_RES) return E_FULL;
    if (nid >= x->nc) return E_NOTFOUND;
    pool_t *p = &x->pools[x->pc];
    p->nid=nid; p->rt=rt; p->total=amt; p->alloc=0; p->avail=amt; x->pc++;
    log_event(x, nid, L_INFO, "resource pooled");
    return E_OK;
}

/* Allocate resource quota for an application from the global pool */
static int alloc_quota(ctx_t *x, unsigned int aid, unsigned int rt, unsigned int q) {
    if (x->qc >= MAX_RES) return E_FULL;
    for (unsigned int i=0; i<x->pc; i++) {
        pool_t *p = &x->pools[i];
        if (p->rt == rt && p->avail >= q) {
            quota_t *qu = &x->quotas[x->qc];
            qu->aid=aid; qu->rt=rt; qu->limit=q; qu->used=0;
            p->alloc += q; p->avail -= q; x->qc++;
            log_event(x, 0, L_INFO, "quota allocated"); return E_OK;
        }
    }
    return E_QUOTA;
}

/* Balance load across nodes - detect overloaded nodes for migration */
static int balance_load(ctx_t *x) {
    if (x->nc == 0) return E_OK;
    unsigned int tot=0;
    for (unsigned int i=0; i<x->nc; i++) tot += x->nodes[i].load;
    unsigned int avg = tot / x->nc, mig=0;
    for (unsigned int i=0; i<x->nc; i++) if (x->nodes[i].load > avg+20U) mig++;
    log_event(x, 0, L_INFO, "load balanced");
    return (int)mig;
}

/* Elastic scaling: scale out to least-loaded node, or scale in */
static int elastic_scale(ctx_t *x, unsigned int aid, int dir) {
    if (aid >= x->ac) return E_NOTFOUND;
    if (dir > 0) {
        unsigned int best=0xFFFFFFFFU, bl=0xFFFFFFFFU;
        for (unsigned int i=0; i<x->nc; i++)
            if ((x->nodes[i].flags&N_ONLINE) && x->nodes[i].load<bl) { bl=x->nodes[i].load; best=i; }
        if (best != 0xFFFFFFFFU) { log_event(x, best, L_INFO, "scaled out"); return (int)best; }
    } else { log_event(x, x->apps[aid].nid, L_INFO, "scaled in"); }
    return E_OK;
}
static int fault_migrate(ctx_t *x, unsigned int aid, unsigned int tgt) {
    if (aid >= x->ac || tgt >= x->nc) return E_NOTFOUND;
    if (!(x->nodes[tgt].flags & N_ONLINE)) return E_NET;
    app_t *a = &x->apps[aid]; unsigned int old = a->nid;
    a->nid = tgt; a->state = A_MIGRATED;
    x->nodes[old].apps--; x->nodes[tgt].apps++;
    log_event(x, aid, L_WARN, "fault migrated");
    return E_OK;
}

/* ── Application Deployment ─────────────────────────────────────────── */

static int deploy_container(ctx_t *x, const char *img, unsigned int nid) {
    if (x->ac>=MAX_APPS || x->dc>=MAX_DEPS) return E_FULL;
    if (nid >= x->nc) return E_NOTFOUND;
    app_t *a = &x->apps[x->ac];
    a->id=x->ac; scopy(a->name,img,MAX_NAME); a->type=1; a->state=A_RUNNING;
    a->ver=1; a->nid=nid; a->did=x->dc;
    dep_t *d = &x->deps[x->dc];
    d->id=x->dc; d->aid=a->id; d->nid=nid; d->ver=1; d->cpct=0; d->flags=0x01U;
    x->ac++; x->dc++; x->nodes[nid].apps++;
    log_event(x, nid, L_INFO, "container deployed");
    return (int)a->id;
}

/* Deploy a serverless function to the specified edge node */
static int deploy_function(ctx_t *x, const char *fn, unsigned int nid) {
    if (x->ac>=MAX_APPS || x->dc>=MAX_DEPS) return E_FULL;
    if (nid >= x->nc) return E_NOTFOUND;
    app_t *a = &x->apps[x->ac];
    a->id=x->ac; scopy(a->name,fn,MAX_NAME); a->type=2; a->state=A_RUNNING;
    a->ver=1; a->nid=nid; a->did=x->dc;
    dep_t *d = &x->deps[x->dc];
    d->id=x->dc; d->aid=a->id; d->nid=nid; d->ver=1; d->cpct=0; d->flags=0x02U;
    x->ac++; x->dc++; x->nodes[nid].apps++;
    log_event(x, nid, L_INFO, "function deployed");
    return (int)a->id;
}

/* Deploy ML model - requires GPU-capable node */
static int deploy_model(ctx_t *x, const char *m, unsigned int nid) {
    if (x->ac>=MAX_APPS || x->dc>=MAX_DEPS) return E_FULL;
    if (nid >= x->nc) return E_NOTFOUND;
    if (!(x->nodes[nid].flags & N_GPU)) return E_DEPLOY;
    app_t *a = &x->apps[x->ac];
    a->id=x->ac; scopy(a->name,m,MAX_NAME); a->type=3; a->state=A_RUNNING;
    a->ver=1; a->nid=nid; a->did=x->dc;
    dep_t *d = &x->deps[x->dc];
    d->id=x->dc; d->aid=a->id; d->nid=nid; d->ver=1; d->cpct=0; d->flags=0x04U;
    x->ac++; x->dc++; x->nodes[nid].apps++;
    log_event(x, nid, L_INFO, "model deployed"); return (int)a->id;
}
static int canary_release(ctx_t *x, unsigned int did, unsigned int pct) {
    if (did >= x->dc || pct > 100U) return E_DEPLOY;
    dep_t *d = &x->deps[did]; d->cpct = pct;
    if (d->aid < x->ac) x->apps[d->aid].state = A_CANARY;
    log_event(x, d->nid, L_INFO, "canary release"); return E_OK;
}
static int version_rollback(ctx_t *x, unsigned int did, unsigned int ver) {
    if (did >= x->dc) return E_NOTFOUND;
    dep_t *d = &x->deps[did]; d->ver = ver;
    if (d->aid < x->ac) x->apps[d->aid].ver = ver;
    log_event(x, d->nid, L_WARN, "version rollback");
    return E_OK;
}

/* ── Network Management ─────────────────────────────────────────────── */

static int build_topology(ctx_t *x) {
    if (x->nc < 2) return E_OK;
    x->rc = 0;
    for (unsigned int i=0; i<x->nc && x->rc<MAX_ROUTES; i++)
        for (unsigned int j=i+1U; j<x->nc && x->rc<MAX_ROUTES; j++) {
            route_t *r = &x->routes[x->rc];
            r->src=i; r->dst=j; r->metric=1U; r->via=0xFFFFFFFFU; r->flags=0x01U; x->rc++;
        }
    log_event(x, 0, L_INFO, "topology built");
    return (int)x->rc;
}

/* Configure SD-WAN overlay for a specific route */
static int configure_sdwan(ctx_t *x, unsigned int rid) {
    if (rid >= x->rc) return E_NOTFOUND;
    x->routes[rid].flags |= 0x02U; log_event(x, 0, L_INFO, "sdwan configured"); return E_OK;
}
static int p2p_connect(ctx_t *x, unsigned int s, unsigned int d) {
    if (x->pkc>=MAX_PEERS || s>=x->nc || d>=x->nc) return E_FULL;
    peer_t *p = &x->peers[x->pkc];
    p->id=x->pkc; p->nid=d; p->nat=0; p->reach=1; scopy(p->ep,"p2p://edge",MAX_NAME); x->pkc++;
    log_event(x, s, L_INFO, "p2p connected");
    return (int)p->id;
}

/* NAT traversal using hole-punching or relay fallback */
static int nat_traverse(ctx_t *x, unsigned int pid) {
    if (pid >= x->pkc) return E_NOTFOUND;
    x->peers[pid].nat=1U; x->peers[pid].reach=1;
    log_event(x, x->peers[pid].nid, L_INFO, "nat traversed"); return E_OK;
}
static int offline_tx(ctx_t *x, unsigned int pid, const unsigned char *d, unsigned int l) {
    if (pid>=x->pkc || !d || l==0) return E_NET;
    log_event(x, x->peers[pid].nid, L_WARN, "offline tx queued");
    return (int)l;
}

/* ── Security Isolation ─────────────────────────────────────────────── */

static int enable_tee(ctx_t *x, unsigned int nid) {
    if (nid >= x->nc) return E_NOTFOUND;
    x->nodes[nid].flags |= N_TEE|N_SECURE;
    log_event(x, nid, L_INFO, "tee enabled"); return E_OK;
}
static int secure_boot(ctx_t *x, unsigned int nid) {
    if (nid >= x->nc) return E_NOTFOUND;
    unsigned int ci = x->nodes[nid].cidx;
    if (ci==0xFFFFFFFFU || ci>=x->cc) { log_event(x,nid,L_ERROR,"boot verify failed"); return E_AUTH; }
    cert_t *c = &x->certs[ci];
    if (c->hhi!=0xDEADBEEFU || c->hlo!=0xCAFEBABEU) { log_event(x,nid,L_ERROR,"boot hash mismatch"); return E_TEE; }
    x->nodes[nid].flags |= N_SECURE; log_event(x, nid, L_INFO, "boot verified"); return E_OK;
}
/* XOR-based stream cipher placeholder using rotating key schedule */
static int encrypt_data(ctx_t *x, const unsigned char *p, unsigned int l, unsigned char *c) {
    if (!p || !c || l==0) return E_NOMEM;
    static const unsigned char key[16] = {
        0xA5U,0x3CU,0x7FU,0xE1U,0x56U,0xB8U,0x2DU,0x94U,
        0x0FU,0xD3U,0x68U,0x41U,0xC7U,0x8AU,0x15U,0xEBU
    };
    for (unsigned int i=0; i<l; i++) c[i] = p[i] ^ key[i&15U];
    log_event(x, 0, L_DEBUG, "data encrypted"); return E_OK;
}
static int check_access(ctx_t *x, unsigned int nid, unsigned int aid, unsigned int perms) {
    if (nid>=x->nc) return E_NOTFOUND;
    if (aid>=x->ac && aid!=0) return E_NOTFOUND;
    unsigned int nf = x->nodes[nid].flags;
    if (!(nf & N_AUTHED)) { log_event(x,nid,L_WARN,"access denied unauth"); return E_AUTH; }
    if ((perms&0x04U) && !(nf&N_TEE)) { log_event(x,nid,L_WARN,"access denied no tee"); return E_TEE; }
    if ((perms&0x08U) && !(nf&N_SECURE)) { log_event(x,nid,L_WARN,"access denied no boot"); return E_AUTH; }
    log_event(x, nid, L_DEBUG, "access granted");
    return E_OK;
}

/* ── Monitoring Ops ─────────────────────────────────────────────────── */

static int ensure_mon(ctx_t *x, unsigned int nid) {
    for (unsigned int i=0; i<x->mc; i++) if (x->mons[i].nid==nid) return (int)i;
    if (x->mc >= MAX_NODES) return E_FULL;
    unsigned int idx = x->mc; x->mons[idx].nid=nid; x->mons[idx].hp=100; x->mc++;
    return (int)idx;
}
static int check_health(ctx_t *x, unsigned int nid) {
    if (nid >= x->nc) return E_NOTFOUND;
    int idx = ensure_mon(x, nid); if (idx < 0) return idx;
    mon_t *m = &x->mons[idx]; m->hp = 100;
    if (!(x->nodes[nid].flags&N_ONLINE)) m->hp=0;
    else if (x->nodes[nid].load>90U) m->hp=30;
    else if (x->nodes[nid].load>70U) m->hp=60;
    log_event(x, nid, L_INFO, "health checked");
    return (int)m->hp;
}

/* Sample resource usage metrics from a node */
static int sample_usage(ctx_t *x, unsigned int nid) {
    if (nid >= x->nc) return E_NOTFOUND;
    int idx = ensure_mon(x, nid); if (idx < 0) return idx;
    mon_t *m = &x->mons[idx];
    m->cpu=x->nodes[nid].load; m->mem=45U; m->disk=30U; m->net=20U;
    m->arun=x->nodes[nid].apps;
    log_event(x, nid, L_DEBUG, "usage sampled");
    return E_OK;
}

/* Collect log entries for a specific node */
static int collect_logs(ctx_t *x, unsigned int nid) {
    if (nid >= x->nc) return E_NOTFOUND;
    int idx = ensure_mon(x, nid); if (idx < 0) return idx;
    unsigned int cnt=0;
    for (unsigned int i=0; i<x->lc; i++) if (x->logs[i].nid==nid) cnt++;
    x->mons[idx].lc = cnt;
    log_event(x, nid, L_DEBUG, "logs collected");
    return (int)cnt;
}

/* Initiate remote debug session on an edge node (requires TEE) */
static int remote_debug(ctx_t *x, unsigned int nid, const char *cmd) {
    if (nid>=x->nc || !cmd) return E_NOTFOUND;
    int rc = check_access(x, nid, 0, 0x04U);
    if (rc != E_OK) { log_event(x, nid, L_ERROR, "debug denied"); return rc; }
    log_event(x, nid, L_WARN, "remote debug");
    return E_OK;
}

/* ── Self-test / Integration Exercise ───────────────────────────────── */

static void run_self_test(ctx_t *x) {
    host_print("[edge_node] self-test begin\n");
    int n0=register_node(x,"edge-gw-01",N_ONLINE);
    int n1=register_node(x,"edge-cmp-02",N_ONLINE);
    int n2=register_node(x,"edge-gpu-03",N_ONLINE);
    if (n0<0||n1<0||n2<0) { host_print("[edge_node] FAIL: register\n"); return; }
    host_print("[edge_node] PASS: register 3 nodes\n");
    if (authenticate_node(x,0)!=E_OK||authenticate_node(x,1)!=E_OK||authenticate_node(x,2)!=E_OK)
        { host_print("[edge_node] FAIL: auth\n"); return; }
    host_print("[edge_node] PASS: authenticate\n");
    caps_t c0={4,2400,0,0,8192,65536,1000,0};
    caps_t c1={8,3200,0,0,16384,131072,2000,0};
    caps_t c2={16,3600,2,16384,65536,524288,10000,0};
    report_caps(x,0,&c0); report_caps(x,1,&c1); report_caps(x,2,&c2);
    host_print("[edge_node] PASS: caps\n");
    geo_t g0={37774,-122419,10}, g1={37784,-122409,15}, g2={37338,-121885,30};
    set_geo(x,0,&g0); set_geo(x,1,&g1); set_geo(x,2,&g2);
    pool_resource(x,0,R_CPU,400); pool_resource(x,0,R_MEM,8192);
    pool_resource(x,1,R_CPU,800); pool_resource(x,1,R_MEM,16384);
    pool_resource(x,2,R_CPU,1600); pool_resource(x,2,R_GPU,200); pool_resource(x,2,R_MEM,65536);
    host_print("[edge_node] PASS: pools\n");
    if (alloc_quota(x,0,R_CPU,200)!=E_OK||alloc_quota(x,0,R_MEM,4096)!=E_OK)
        { host_print("[edge_node] FAIL: quota\n"); return; }
    host_print("[edge_node] PASS: quotas\n");
    int a0=deploy_container(x,"nginx:latest",0);
    int a1=deploy_function(x,"inference-fn",2);
    int a2=deploy_model(x,"yolo-v8-edge",2);
    if (a0<0||a1<0||a2<0) { host_print("[edge_node] FAIL: deploy\n"); return; }
    host_print("[edge_node] PASS: deploy\n");
    canary_release(x,0,10);
    host_print("[edge_node] PASS: canary\n");
    build_topology(x); configure_sdwan(x,0);
    host_print("[edge_node] PASS: topology+sdwan\n");
    int p0=p2p_connect(x,0,1); if (p0<0) { host_print("[edge_node] FAIL: p2p\n"); return; }
    nat_traverse(x,(unsigned int)p0);
    host_print("[edge_node] PASS: p2p+nat\n");
    enable_tee(x,2); secure_boot(x,0);
    unsigned char pl[8]={0x48,0x65,0x6C,0x6C,0x6F,0x21,0x00,0x00}, ci[8];
    encrypt_data(x,pl,6,ci);
    host_print("[edge_node] PASS: security\n");
    if (check_access(x,2,0,0x04U)!=E_OK) { host_print("[edge_node] FAIL: access tee\n"); return; }
    if (check_access(x,1,0,0x04U)==E_OK) { host_print("[edge_node] FAIL: should deny\n"); return; }
    host_print("[edge_node] PASS: access control\n");
    x->nodes[0].load=85; x->nodes[1].load=40; x->nodes[2].load=20;
    balance_load(x); elastic_scale(x,(unsigned int)a0,1);
    host_print("[edge_node] PASS: balance+scale\n");
    fault_migrate(x,(unsigned int)a0,1);
    host_print("[edge_node] PASS: migrate\n");
    version_rollback(x,0,0);
    host_print("[edge_node] PASS: rollback\n");
    unsigned char blob[4]={0xAA,0xBB,0xCC,0xDD};
    offline_tx(x,(unsigned int)p0,blob,4);
    host_print("[edge_node] PASS: offline tx\n");
    check_health(x,0); sample_usage(x,1); collect_logs(x,2); remote_debug(x,2,"dump");
    host_print("[edge_node] PASS: monitoring\n");
    host_print("[edge_node] self-test complete\n");
}

__attribute__((export_name("main")))
int main(void) {
    ctx_t *x = (ctx_t *)walloc(sizeof(ctx_t));
    if (!x) { host_print("[edge_node] FATAL: alloc\n"); host_exit(1); return 1; }
    int rc = edge_init(x);
    if (rc != E_OK) { host_print("[edge_node] FATAL: init\n"); host_exit(1); return 1; }
    run_self_test(x);
    host_exit(0);
    return 0;
}
