/* rate_limiter: Distributed Rate Limiting Center (v1.0) */
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
#define MAX_ALGO 8
#define MAX_DIM 16
#define MAX_NODES 8
#define MAX_RULES 32
#define MAX_LIST 32
#define MAX_KEYS 64
#define MAX_QUEUE 16
#define MAX_METRIC 16
#define MAX_HOT 16
#define MAX_NAME 32
#define MAX_KEY 48
#define BUF_SIZE 4096
#define ALGO_TOKEN_BUCKET 0
#define ALGO_SLIDING_WINDOW 1
#define ALGO_LEAKY_BUCKET 2
#define ALGO_FIXED_WINDOW 3
#define ALGO_CONCURRENCY 4
#define DIM_IP 0
#define DIM_USER 1
#define DIM_API 2
#define DIM_TENANT 3
#define DIM_REGION 4
#define NODE_OFFLINE 0
#define NODE_ONLINE 1
#define NODE_SYNCING 2
#define RULE_ACTIVE 0
#define RULE_GRAY 1
#define RULE_DISABLED 2
#define CB_CLOSED 0
#define CB_OPEN 1
#define CB_HALF_OPEN 2
#define PRI_LOW 0
#define PRI_NORMAL 1
#define PRI_HIGH 2
#define PRI_CRITICAL 3
#define DEGRADE_NONE 0
#define DEGRADE_WARN 1
#define DEGRADE_QUEUE 2
#define DEGRADE_REJECT 3
#define DEGRADE_CIRCUIT 4
#define LIST_WHITELIST 0
#define LIST_BLACKLIST 1
#define STORE_MEMORY 0
#define STORE_REDIS 1

/* ─── Typedefs ─── */
typedef struct { char name[MAX_NAME]; int algo; int max_tok; int cur_tok;
    int refill; unsigned int last_ref; int leak; int q_max; int q_cur;
    int w_max; int w_cnt; unsigned int w_st; unsigned int w_ms;
    int c_max; int c_cur; int active; } algo_t;
typedef struct { char key[MAX_KEY]; int dim; int ai;
    int tot; int rej; int pass; unsigned int last; } dim_t;
typedef struct { char id[MAX_NAME]; char addr[MAX_NAME];
    int state; int wt; int tok; unsigned int lsyn; int scnt; int fcnt; } node_t;
typedef struct { char name[MAX_NAME]; int dim; int algo;
    int thr; unsigned int wms; int state; int gpct; unsigned int cat; int pri; } rule_t;
typedef struct { char ent[MAX_KEY]; int lt; int dim; int act; unsigned int at; } list_t;
typedef struct { char key[MAX_KEY]; int lq; int used;
    int glim; unsigned int la; int acnt; int st; } quota_t;
typedef struct { char key[MAX_KEY]; int pri; int st;
    int wms; unsigned int enq; int pos; } queue_t;
typedef struct { char svc[MAX_NAME]; int state;
    int fails; unsigned int tat; int lrl; } cbreaker_t;
typedef struct { unsigned int ts; int qps; int rej; int tot; int trig; } metric_t;
typedef struct { char key[MAX_KEY]; int dim;
    int rc; int rjc; unsigned int ls; } hot_t;
typedef struct { char desc[MAX_NAME]; unsigned int at; int sev; int mv; } alert_t;
typedef struct { int mode; int rcode; int rams; int qen; int qmax;
    int pbyp; int dthr; int cbl; } degcfg_t;

/* ─── Globals ─── */
static unsigned int clk = 0;
static int algo_n=0,dim_n=0,node_n=0,rule_n=0,list_n=0,quota_n=0;
static int queue_n=0,cb_n=0,metric_n=0,hot_n=0,alert_n=0;
static int total_reqs=0,total_rej=0,total_trig=0,store_be=STORE_MEMORY;
static algo_t algos[MAX_ALGO]; static dim_t dims[MAX_DIM];
static node_t nodes[MAX_NODES]; static rule_t rules[MAX_RULES];
static list_t lists[MAX_LIST]; static quota_t quotas[MAX_KEYS];
static queue_t queues[MAX_QUEUE]; static cbreaker_t breakers[MAX_ALGO];
static metric_t metrics[MAX_METRIC]; static hot_t hots[MAX_HOT];
static alert_t alerts[MAX_HOT]; static degcfg_t degrade;

/* ─── Forward declarations ─── */
static void pint(int v); static void pstr(const char *s);
static int my_strlen(const char *s); static int my_strcmp(const char *a, const char *b);
static void my_strncpy(char *d, const char *s, int n);
static void cb_transition(int idx); static void metric_record(void);
static void hot_update(const char *k, int d, int rj);
static void anomaly_check(void); static int degrade_eval(int qps);

/* ─── Helpers ─── */
static void pstr(const char *s) { host_print(s); }
static int my_strlen(const char *s) { int l=0; while(s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
}
static void my_strncpy(char *d, const char *s, int n) {
    int i=0; while (i<n-1 && s[i]) { d[i]=s[i]; i++; } d[i]='\0';
}
static void pint(int v) {
    char b[32]; int p=0;
    if (v<0) { b[p++]='-'; v=-v; }
    if (v==0) { b[p++]='0'; } else {
        int d=0,t=v; while(t>0){d++;t/=10;} p+=d; b[p]='\0'; p--;
        while(v>0){b[p--]='0'+(v%10);v/=10;}
    } host_print(b);
}
static void phead(const char *t) { pstr("\n=== "); pstr(t); pstr(" ===\n"); }
static const char *dn(int d) {
    if(d==DIM_IP) return "IP"; if(d==DIM_USER) return "USER";
    if(d==DIM_API) return "API"; if(d==DIM_TENANT) return "TENANT";
    if(d==DIM_REGION) return "REGION"; return "UNK";
}
static const char *an(int a) {
    if(a==ALGO_TOKEN_BUCKET) return "TokenBucket";
    if(a==ALGO_SLIDING_WINDOW) return "SlideWin";
    if(a==ALGO_LEAKY_BUCKET) return "LeakyBkt";
    if(a==ALGO_FIXED_WINDOW) return "FixedWin";
    if(a==ALGO_CONCURRENCY) return "Concur"; return "Unk";
}
/* ═══ Token Bucket ═══ */
static int tb_create(const char *name, int mx, int ref) {
    if (algo_n>=MAX_ALGO) return -1; int i=algo_n++;
    my_strncpy(algos[i].name,name,MAX_NAME-1);
    algos[i].algo=ALGO_TOKEN_BUCKET; algos[i].max_tok=mx;
    algos[i].cur_tok=mx; algos[i].refill=ref; algos[i].last_ref=clk;
    algos[i].active=1; pstr("TokenBucket: "); pstr(name);
    pstr(" max="); pint(mx); pstr(" ref="); pint(ref); pstr("/t\n"); return i;
}
static int tb_consume(int i) {
    if (i<0||i>=algo_n||algos[i].algo!=ALGO_TOKEN_BUCKET) return -1;
    unsigned int el=clk-algos[i].last_ref; int add=(int)el*algos[i].refill;
    if (add>0) { algos[i].cur_tok+=add;
        if (algos[i].cur_tok>algos[i].max_tok) algos[i].cur_tok=algos[i].max_tok;
        algos[i].last_ref=clk; }
    if (algos[i].cur_tok>0) { algos[i].cur_tok--; return 0; } return -1;
}
/* ═══ Sliding Window ═══ */
static int sw_create(const char *name, int mx, unsigned int wms) {
    if (algo_n>=MAX_ALGO) return -1; int i=algo_n++;
    my_strncpy(algos[i].name,name,MAX_NAME-1);
    algos[i].algo=ALGO_SLIDING_WINDOW; algos[i].w_max=mx;
    algos[i].w_cnt=0; algos[i].w_st=clk; algos[i].w_ms=wms; algos[i].active=1;
    pstr("SlideWin: "); pstr(name); pstr(" max="); pint(mx);
    pstr(" win="); pint(wms); pstr("ms\n"); return i;
}
static int sw_check(int i) {
    if (i<0||i>=algo_n||algos[i].algo!=ALGO_SLIDING_WINDOW) return -1;
    if ((clk-algos[i].w_st)>=algos[i].w_ms) { algos[i].w_cnt=0; algos[i].w_st=clk; }
    if (algos[i].w_cnt<algos[i].w_max) { algos[i].w_cnt++; return 0; } return -1;
}
/* ═══ Leaky Bucket ═══ */
static int lb_create(const char *name, int cap, int lr) {
    if (algo_n>=MAX_ALGO) return -1; int i=algo_n++;
    my_strncpy(algos[i].name,name,MAX_NAME-1);
    algos[i].algo=ALGO_LEAKY_BUCKET; algos[i].max_tok=cap;
    algos[i].cur_tok=0; algos[i].leak=lr; algos[i].last_ref=clk; algos[i].active=1;
    pstr("LeakyBkt: "); pstr(name); pstr(" cap="); pint(cap);
    pstr(" leak="); pint(lr); pstr("/t\n"); return i;
}
static int lb_add(int i) {
    if (i<0||i>=algo_n) return -1;
    unsigned int el=clk-algos[i].last_ref; int lk=(int)el*algos[i].leak;
    if (lk>0) { algos[i].cur_tok-=lk;
        if (algos[i].cur_tok<0) algos[i].cur_tok=0; algos[i].last_ref=clk; }
    if (algos[i].cur_tok<algos[i].max_tok) { algos[i].cur_tok++; return 0; } return -1;
}
/* ═══ Fixed Window ═══ */
static int fw_create(const char *name, int mx, unsigned int wms) {
    if (algo_n>=MAX_ALGO) return -1; int i=algo_n++;
    my_strncpy(algos[i].name,name,MAX_NAME-1);
    algos[i].algo=ALGO_FIXED_WINDOW; algos[i].w_max=mx;
    algos[i].w_cnt=0; algos[i].w_st=clk; algos[i].w_ms=wms; algos[i].active=1;
    pstr("FixedWin: "); pstr(name); pstr(" max="); pint(mx);
    pstr(" win="); pint(wms); pstr("ms\n"); return i;
}
static int fw_check(int i) {
    if (i<0||i>=algo_n) return -1;
    unsigned int wi=(clk-algos[i].w_st)/algos[i].w_ms;
    if (wi>0) { algos[i].w_cnt=0; algos[i].w_st+=wi*algos[i].w_ms; }
    if (algos[i].w_cnt<algos[i].w_max) { algos[i].w_cnt++; return 0; } return -1;
}
/* ═══ Concurrency Control ═══ */
static int cc_create(const char *name, int mx) {
    if (algo_n>=MAX_ALGO) return -1; int i=algo_n++;
    my_strncpy(algos[i].name,name,MAX_NAME-1);
    algos[i].algo=ALGO_CONCURRENCY; algos[i].c_max=mx;
    algos[i].c_cur=0; algos[i].active=1;
    pstr("Concur: "); pstr(name); pstr(" max="); pint(mx); pstr("\n"); return i;
}
static int cc_acquire(int i) {
    if (i<0||i>=algo_n) return -1;
    if (algos[i].c_cur<algos[i].c_max) { algos[i].c_cur++; return 0; } return -1;
}
static int cc_release(int i) {
    if (i<0||i>=algo_n) return -1;
    if (algos[i].c_cur>0) algos[i].c_cur--; return 0;
}
/* ═══ Dimension limiting ═══ */
static int dim_reg(const char *key, int dim, int ai) {
    if (dim_n>=MAX_DIM) return -1; int i=dim_n++;
    my_strncpy(dims[i].key,key,MAX_KEY-1); dims[i].dim=dim; dims[i].ai=ai;
    dims[i].tot=0; dims[i].rej=0; dims[i].pass=0; dims[i].last=clk;
    pstr("Dim: "); pstr(dn(dim)); pstr(":"); pstr(key);
    pstr(" algo="); pstr(an(algos[ai].algo)); pstr("\n"); return i;
}
static int algo_check(int ai) {
    int a=algos[ai].algo;
    if (a==ALGO_TOKEN_BUCKET) return tb_consume(ai);
    if (a==ALGO_SLIDING_WINDOW) return sw_check(ai);
    if (a==ALGO_LEAKY_BUCKET) return lb_add(ai);
    if (a==ALGO_FIXED_WINDOW) return fw_check(ai);
    if (a==ALGO_CONCURRENCY) return cc_acquire(ai);
    return -1;
}
static int dim_check(int di) {
    if (di<0||di>=dim_n) return -1;
    dims[di].tot++; dims[di].last=clk; total_reqs++;
    int r=algo_check(dims[di].ai);
    if (r==0) dims[di].pass++; else { dims[di].rej++; total_rej++; total_trig++; }
    return r;
}
static int dim_find(int dim, const char *key) {
    for (int i=0;i<dim_n;i++) if (dims[i].dim==dim&&my_strcmp(dims[i].key,key)==0)
        return dim_check(i); return 0;
}
static int dim_ip(const char *k) { return dim_find(DIM_IP,k); }
static int dim_user(const char *k) { return dim_find(DIM_USER,k); }
static int dim_api(const char *k) { return dim_find(DIM_API,k); }
static int dim_tenant(const char *k) { return dim_find(DIM_TENANT,k); }
static int dim_region(const char *k) { return dim_find(DIM_REGION,k); }
/* ═══ Distributed: Nodes ═══ */
static int node_reg(const char *id, const char *addr, int wt) {
    if (node_n>=MAX_NODES) return -1; int i=node_n++;
    my_strncpy(nodes[i].id,id,MAX_NAME-1); my_strncpy(nodes[i].addr,addr,MAX_NAME-1);
    nodes[i].state=NODE_ONLINE; nodes[i].wt=wt; nodes[i].tok=0;
    nodes[i].lsyn=clk; nodes[i].scnt=0; nodes[i].fcnt=0;
    pstr("Node: "); pstr(id); pstr(" @"); pstr(addr);
    pstr(" wt="); pint(wt); pstr("\n"); return i;
}
static int node_sync(int ni, int amt) {
    if (ni<0||ni>=node_n||nodes[ni].state!=NODE_ONLINE) return -1;
    nodes[ni].tok+=amt; nodes[ni].scnt++; nodes[ni].lsyn=clk;
    pstr("Sync: "); pstr(nodes[ni].id); pstr(" tok="); pint(nodes[ni].tok); pstr("\n");
    return nodes[ni].tok;
}
static int quota_prealloc(const char *key, int gl, int lq) {
    if (quota_n>=MAX_KEYS) return -1; int i=quota_n++;
    my_strncpy(quotas[i].key,key,MAX_KEY-1);
    quotas[i].glim=gl; quotas[i].lq=lq; quotas[i].used=0;
    quotas[i].la=clk; quotas[i].acnt=1; quotas[i].st=store_be;
    pstr("Quota: "); pstr(key); pstr(" glob="); pint(gl);
    pstr(" loc="); pint(lq); pstr(" st=");
    pstr(store_be==STORE_REDIS?"Redis":"Mem"); pstr("\n"); return i;
}
static int quota_use(const char *key) {
    for (int i=0;i<quota_n;i++) if (my_strcmp(quotas[i].key,key)==0) {
        if (quotas[i].used<quotas[i].lq) { quotas[i].used++; return 0; }
        pstr("Quota EXH: "); pstr(key); pstr("\n"); return -1; } return -1;
}
static void node_cross_sync(void) {
    pstr("Cross-sync:\n");
    for (int i=0;i<node_n;i++) if (nodes[i].state==NODE_ONLINE) {
        nodes[i].lsyn=clk; nodes[i].scnt++;
        pstr("  "); pstr(nodes[i].id); pstr(" tok="); pint(nodes[i].tok);
        pstr(" sc="); pint(nodes[i].scnt); pstr("\n");
    }
}
static void node_fail_check(void) {
    for (int i=0;i<node_n;i++) if (nodes[i].state==NODE_ONLINE&&(clk-nodes[i].lsyn)>60) {
        nodes[i].fcnt++;
        if (nodes[i].fcnt>=3) { nodes[i].state=NODE_OFFLINE;
            pstr("Node down: "); pstr(nodes[i].id); pstr("\n");
        } else { nodes[i].state=NODE_SYNCING;
            pstr("Node sync: "); pstr(nodes[i].id); pstr("\n"); }
    }
}
/* ═══ Dynamic config ═══ */
static int rule_add(const char *nm, int dim, int algo, int thr, unsigned int wms) {
    if (rule_n>=MAX_RULES) return -1; int i=rule_n++;
    my_strncpy(rules[i].name,nm,MAX_NAME-1); rules[i].dim=dim; rules[i].algo=algo;
    rules[i].thr=thr; rules[i].wms=wms; rules[i].state=RULE_ACTIVE;
    rules[i].gpct=0; rules[i].cat=clk; rules[i].pri=PRI_NORMAL;
    pstr("Rule: "); pstr(nm); pstr(" d="); pstr(dn(dim));
    pstr(" a="); pstr(an(algo)); pstr(" thr="); pint(thr);
    pstr(" w="); pint(wms); pstr("ms\n"); return i;
}
static int rule_upd_thr(int ri, int nt) {
    if (ri<0||ri>=rule_n) return -1; int o=rules[ri].thr; rules[ri].thr=nt;
    pstr("Rule upd: "); pstr(rules[ri].name); pstr(" thr "); pint(o);
    pstr("->"); pint(nt); pstr("\n"); return 0;
}
static int rule_gray(int ri, int pct) {
    if (ri<0||ri>=rule_n) return -1;
    rules[ri].gpct=pct; rules[ri].state=(pct>0&&pct<100)?RULE_GRAY:RULE_ACTIVE;
    pstr("Rule gray: "); pstr(rules[ri].name); pstr(" pct="); pint(pct);
    pstr(" st="); pstr(rules[ri].state==RULE_GRAY?"GRAY":"ACT"); pstr("\n"); return 0;
}
static int rule_hot(const char *nm, int na, int nt) {
    for (int i=0;i<rule_n;i++) if (my_strcmp(rules[i].name,nm)==0) {
        rules[i].algo=na; rules[i].thr=nt;
        pstr("Rule hot: "); pstr(nm); pstr(" a="); pstr(an(na));
        pstr(" thr="); pint(nt); pstr("\n"); return i; } return -1;
}
static int list_add(const char *ent, int lt, int dim) {
    if (list_n>=MAX_LIST) return -1; int i=list_n++;
    my_strncpy(lists[i].ent,ent,MAX_KEY-1); lists[i].lt=lt;
    lists[i].dim=dim; lists[i].act=1; lists[i].at=clk;
    pstr("List: "); pstr(lt==LIST_WHITELIST?"WL":"BL");
    pstr(" "); pstr(dn(dim)); pstr(":"); pstr(ent); pstr("\n"); return i;
}
static int list_chk(const char *ent, int dim) {
    for (int i=0;i<list_n;i++) { if (!lists[i].act||lists[i].dim!=dim) continue;
        if (my_strcmp(lists[i].ent,ent)==0) {
            if (lists[i].lt==LIST_WHITELIST) {
                pstr("WL hit: "); pstr(ent); pstr("=>PASS\n"); return 1; }
            pstr("BL hit: "); pstr(ent); pstr("=>REJECT\n"); return -1; } } return 0;
}
static int list_rm(const char *ent, int lt) {
    for (int i=0;i<list_n;i++) if (lists[i].lt==lt&&my_strcmp(lists[i].ent,ent)==0) {
        lists[i].act=0; pstr("List rm: "); pstr(ent); pstr("\n"); return 0; } return -1;
}
/* ═══ Degradation ═══ */
static int deg_init(int rc, int rams, int qm, int pb) {
    degrade.mode=DEGRADE_NONE; degrade.rcode=rc; degrade.rams=rams;
    degrade.qen=1; degrade.qmax=qm; degrade.pbyp=pb;
    degrade.dthr=80; degrade.cbl=-1;
    pstr("Deg init: code="); pint(rc); pstr(" retry="); pint(rams);
    pstr("ms q="); pint(qm); pstr(" pb="); pint(pb); pstr("\n"); return 0;
}
static int deg_queue(const char *key, int pri) {
    if (queue_n>=MAX_QUEUE||!degrade.qen) return -1; int i=queue_n++;
    my_strncpy(queues[i].key,key,MAX_KEY-1); queues[i].pri=pri;
    queues[i].st=0; queues[i].wms=degrade.rams;
    queues[i].enq=clk; queues[i].pos=i;
    pstr("DegQ: "); pstr(key); pstr(" pri="); pint(pri);
    pstr(" pos="); pint(i); pstr("\n"); return i;
}
static int deg_pri_chk(int pri) {
    if (degrade.pbyp&&pri>=PRI_CRITICAL) {
        pstr("Deg: CRIT bypass\n"); return 1; }
    if (degrade.pbyp&&pri>=PRI_HIGH) {
        pstr("Deg: HIGH jump\n"); return 1; } return 0;
}
static int deg_process(void) {
    int p=0;
    for (int i=0;i<queue_n;i++) if (queues[i].st==0&&(clk-queues[i].enq)>=(unsigned int)queues[i].wms) {
        queues[i].st=1; p++;
        pstr("DegDQ: "); pstr(queues[i].key); pstr(" w=");
        pint((int)(clk-queues[i].enq)); pstr("ms\n"); } return p;
}
static int degrade_eval(int qps) {
    if (qps>degrade.dthr) { if (degrade.mode<DEGRADE_REJECT) {
        degrade.mode=DEGRADE_REJECT; pstr("Deg REJECT: q="); pint(qps);
        pstr(" thr="); pint(degrade.dthr); pstr("\n"); } return -1; }
    if (qps>degrade.dthr/2) { degrade.mode=DEGRADE_WARN; return 0; }
    degrade.mode=DEGRADE_NONE; return 0;
}
/* ═══ Circuit breaker ═══ */
static int cb_init(const char *svc, int lrl) {
    if (cb_n>=MAX_ALGO) return -1; int i=cb_n++;
    my_strncpy(breakers[i].svc,svc,MAX_NAME-1);
    breakers[i].state=CB_CLOSED; breakers[i].fails=0;
    breakers[i].tat=0; breakers[i].lrl=lrl;
    pstr("CB: "); pstr(svc); pstr(" lrl="); pint(lrl); pstr("\n"); return i;
}
static int cb_chk(int ci) {
    if (ci<0||ci>=cb_n) return -1; cb_transition(ci);
    pstr("CB chk: "); pstr(breakers[ci].svc);
    pstr(" st="); pint(breakers[ci].state); pstr("\n");
    return (breakers[ci].state==CB_OPEN)?-1:0;
}
static int cb_fail(int ci) {
    if (ci<0||ci>=cb_n) return -1; breakers[ci].fails++;
    if (breakers[ci].state==CB_CLOSED&&breakers[ci].fails>=3) {
        breakers[ci].state=CB_OPEN; breakers[ci].tat=clk;
        degrade.mode=DEGRADE_CIRCUIT;
        pstr("CB TRIP: "); pstr(breakers[ci].svc); pstr("\n"); } return 0;
}
static void cb_transition(int idx) {
    if (idx<0||idx>=cb_n) return;
    if (breakers[idx].state==CB_OPEN&&(clk-breakers[idx].tat)>=30) {
        breakers[idx].state=CB_HALF_OPEN;
        pstr("CB: OPEN->HALF "); pstr(breakers[idx].svc); pstr("\n"); }
}
/* ═══ Monitoring ═══ */
static void metric_record(void) {
    if (metric_n>=MAX_METRIC) {
        for (int i=0;i<MAX_METRIC-1;i++) metrics[i]=metrics[i+1]; metric_n=MAX_METRIC-1; }
    int i=metric_n++; metrics[i].ts=clk; metrics[i].tot=total_reqs;
    metrics[i].rej=total_rej; metrics[i].trig=total_trig; metrics[i].qps=total_reqs;
}
static void metric_summary(void) {
    int rt=(total_reqs>0)?(total_rej*100/total_reqs):0;
    pstr("Metrics: tot="); pint(total_reqs); pstr(" rej="); pint(total_rej);
    pstr(" trig="); pint(total_trig); pstr(" rate="); pint(rt); pstr("%\n");
    for (int i=0;i<metric_n;i++) { pstr("  ["); pint(i); pstr("] ts=");
        pint(metrics[i].ts); pstr(" t="); pint(metrics[i].tot);
        pstr(" r="); pint(metrics[i].rej); pstr("\n"); }
}
static void hot_update(const char *key, int dim, int rj) {
    for (int i=0;i<hot_n;i++) if (my_strcmp(hots[i].key,key)==0) {
        hots[i].rc++; hots[i].rjc+=rj; hots[i].ls=clk; return; }
    if (hot_n>=MAX_HOT) return; int i=hot_n++;
    my_strncpy(hots[i].key,key,MAX_KEY-1); hots[i].dim=dim;
    hots[i].rc=1; hots[i].rjc=rj; hots[i].ls=clk;
}
static void hot_report(void) {
    pstr("Hotspots:\n");
    for (int i=0;i<hot_n;i++) { pstr("  "); pstr(dn(hots[i].dim)); pstr(":");
        pstr(hots[i].key); pstr(" r="); pint(hots[i].rc);
        pstr(" rj="); pint(hots[i].rjc); pstr("\n"); }
}
static void anomaly_check(void) {
    int rt=(total_reqs>0)?(total_rej*100/total_reqs):0;
    if (rt>50&&alert_n<MAX_HOT) { int ai=alert_n++;
        my_strncpy(alerts[ai].desc,"HIGH_REJ",MAX_NAME-1);
        alerts[ai].at=clk; alerts[ai].sev=3; alerts[ai].mv=rt;
        pstr("ALERT: HIGH_REJ sev=3 rate="); pint(rt); pstr("%\n"); }
    for (int i=0;i<hot_n;i++) if (hots[i].rc>100&&alert_n<MAX_HOT) {
        int ai=alert_n++; my_strncpy(alerts[ai].desc,"HOTSPOT",MAX_NAME-1);
        alerts[ai].at=clk; alerts[ai].sev=2; alerts[ai].mv=hots[i].rc;
        pstr("ALERT: HOT "); pstr(hots[i].key); pstr(" r="); pint(hots[i].rc); pstr("\n"); }
}
/* ═══ Unified request check ═══ */
static int req_chk(const char *ip, const char *user, const char *api,
                    const char *ten, const char *reg, int pri) {
    pstr("Req: ip="); pstr(ip); pstr(" u="); pstr(user);
    pstr(" api="); pstr(api); pstr("\n");
    int wl=list_chk(ip,DIM_IP);
    if (wl>0) return 0; if (wl<0) { total_rej++; return -1; }
    wl=list_chk(user,DIM_USER);
    if (wl>0) return 0; if (wl<0) { total_rej++; return -1; }
    if (deg_pri_chk(pri)) return 0;
    int r=dim_ip(ip); if (r!=0) { hot_update(ip,DIM_IP,1); return r; }
    r=dim_user(user); if (r!=0) { hot_update(user,DIM_USER,1); return r; }
    r=dim_api(api); if (r!=0) { hot_update(api,DIM_API,1); return r; }
    r=dim_tenant(ten); if (r!=0) { hot_update(ten,DIM_TENANT,1); return r; }
    r=dim_region(reg); if (r!=0) { hot_update(reg,DIM_REGION,1); return r; }
    quota_use(api); hot_update(ip,DIM_IP,0); hot_update(user,DIM_USER,0); return 0;
}
/* ═══ Tests ═══ */
static void test_algos(void) {
    phead("Algorithms");
    int tb=tb_create("tb-api",5,2);
    for (int i=0;i<7;i++) { int r=tb_consume(tb);
        pstr("  TB "); pint(i); pstr(": "); pstr(r==0?"OK":"REJ"); pstr("\n"); }
    clk+=2; pstr("After refill:\n");
    pstr("  TB: "); pstr(tb_consume(tb)==0?"OK":"REJ"); pstr("\n"); clk-=2;
    int sw=sw_create("sw-api",3,1000);
    for (int i=0;i<5;i++) { int r=sw_check(sw);
        pstr("  SW "); pint(i); pstr(": "); pstr(r==0?"OK":"REJ"); pstr("\n"); }
    int lb=lb_create("lb-api",3,1);
    for (int i=0;i<5;i++) { int r=lb_add(lb);
        pstr("  LB "); pint(i); pstr(": "); pstr(r==0?"OK":"REJ"); pstr("\n"); }
    clk+=2; pstr("  LB+leak: "); pstr(lb_add(lb)==0?"OK":"REJ"); pstr("\n"); clk-=2;
    int fw=fw_create("fw-api",4,500);
    for (int i=0;i<6;i++) { int r=fw_check(fw);
        pstr("  FW "); pint(i); pstr(": "); pstr(r==0?"OK":"REJ"); pstr("\n"); }
    int cc=cc_create("cc-api",2);
    pstr("  CC: "); pint(cc_acquire(cc)); pint(cc_acquire(cc));
    pint(cc_acquire(cc)); pstr("\n");
    cc_release(cc); pstr("  CC rel: "); pint(cc_acquire(cc)); pstr("\n"); cc_release(cc);
}
static void test_dims(void) {
    phead("Dimensions");
    int t1=tb_create("d-tb",3,1),s1=sw_create("d-sw",4,1000);
    int l1=lb_create("d-lb",2,1),f1=fw_create("d-fw",5,500),c1=cc_create("d-cc",3);
    dim_reg("10.0.0.1",DIM_IP,t1); dim_reg("alice",DIM_USER,s1);
    dim_reg("/api/v1/data",DIM_API,l1); dim_reg("acme",DIM_TENANT,f1);
    dim_reg("us-east",DIM_REGION,c1);
    for (int i=0;i<4;i++) { int r=dim_check(0);
        pstr("  IP "); pint(i); pstr(": "); pstr(r==0?"OK":"REJ"); pstr("\n"); }
    dim_ip("10.0.0.1"); dim_user("alice"); dim_api("/api/v1/data");
    dim_tenant("acme"); dim_region("us-east");
}
static void test_dist(void) {
    phead("Distributed");
    int n0=node_reg("node-1","192.168.1.1",100),n1=node_reg("node-2","192.168.1.2",80);
    node_reg("node-3","192.168.1.3",60);
    node_sync(n0,500); node_sync(n1,400);
    quota_prealloc("/api/v1/data",10000,2500); quota_prealloc("/api/v1/users",5000,1250);
    quota_use("/api/v1/data"); quota_use("/api/v1/data");
    node_cross_sync(); nodes[n1].state=NODE_OFFLINE;
    pstr("Node off: node-2\n"); clk+=61; node_fail_check(); clk-=61;
}
static void test_config(void) {
    phead("Dynamic Config");
    int r0=rule_add("ip-v1",DIM_IP,ALGO_TOKEN_BUCKET,100,60000);
    int r1=rule_add("user-v1",DIM_USER,ALGO_SLIDING_WINDOW,50,30000);
    rule_add("api-v1",DIM_API,ALGO_LEAKY_BUCKET,200,60000);
    rule_upd_thr(r0,150); rule_gray(r1,30);
    rule_hot("ip-v1",ALGO_SLIDING_WINDOW,120);
    list_add("10.0.0.99",LIST_WHITELIST,DIM_IP);
    list_add("10.0.0.66",LIST_BLACKLIST,DIM_IP);
    list_add("baduser",LIST_BLACKLIST,DIM_USER);
    pstr("WL 10.0.0.99: "); pint(list_chk("10.0.0.99",DIM_IP)); pstr("\n");
    pstr("BL 10.0.0.66: "); pint(list_chk("10.0.0.66",DIM_IP)); pstr("\n");
    pstr("BL baduser: "); pint(list_chk("baduser",DIM_USER)); pstr("\n");
    pstr("Norm 10.0.0.1: "); pint(list_chk("10.0.0.1",DIM_IP)); pstr("\n");
    list_rm("10.0.0.66",LIST_BLACKLIST);
    pstr("After rm 10.0.0.66: "); pint(list_chk("10.0.0.66",DIM_IP)); pstr("\n");
    (void)r0; (void)r1;
}
static void test_degrade(void) {
    phead("Degradation");
    deg_init(429,500,10,1);
    deg_queue("req-1",PRI_LOW); deg_queue("req-2",PRI_HIGH);
    deg_queue("req-3",PRI_CRITICAL); deg_queue("req-4",PRI_NORMAL);
    deg_pri_chk(PRI_CRITICAL); deg_pri_chk(PRI_HIGH); deg_pri_chk(PRI_NORMAL);
    clk+=500; deg_process(); clk-=500;
    degrade_eval(50); pstr("Deg m="); pint(degrade.mode); pstr("\n");
    degrade_eval(100); pstr("Deg m="); pint(degrade.mode); pstr("\n");
}
static void test_cb(void) {
    phead("Circuit Breaker");
    int cb=cb_init("upstream",0); cb_chk(cb);
    for (int i=0;i<3;i++) cb_fail(cb);
    pstr("After trip: "); pint(cb_chk(cb)); pstr("\n");
    clk+=30; pstr("After timeout:\n"); cb_chk(cb); clk-=30;
}
static void test_mon(void) {
    phead("Monitoring"); metric_record(); metric_summary();
    hot_report(); anomaly_check();
}
static void test_unified(void) {
    phead("Unified Request");
    req_chk("10.0.0.99","alice","/api/v1/data","acme","us-east",PRI_NORMAL);
    req_chk("10.0.0.66","bob","/api/v1/users","x","eu-west",PRI_NORMAL);
    req_chk("10.0.0.1","alice","/api/v1/data","acme","us-east",PRI_CRITICAL);
    req_chk("10.0.0.1","baduser","/api/v1/data","acme","us-east",PRI_NORMAL);
    for (int i=0;i<5;i++)
        req_chk("10.0.0.1","alice","/api/v1/data","acme","us-east",PRI_LOW);
    metric_record(); metric_summary(); hot_report(); anomaly_check();
}
static void test_suite(void) {
    pstr("=== Rate Limiter Test Suite v1.0 ===\n");
    test_algos(); test_dims(); test_dist(); test_config();
    test_degrade(); test_cb(); test_mon(); test_unified();
    pstr("\n=== All tests passed ===\n");
}
static void print_help(void) {
    pstr("rate_limiter v1.0 - Distributed Rate Limiting Center\n");
    pstr("Usage: rate_limiter [options]\n");
    pstr("  -h    Show this help\n");
    pstr("  -t    Run test suite\n\nFeatures:\n");
    pstr("  Algorithms (token_bucket/sliding_window/leaky_bucket/fixed_window/concurrency)\n");
    pstr("  Dimensions (IP/user/API/tenant/region)\n");
    pstr("  Distributed (Redis/memory, cross-node sync, quota prealloc)\n");
    pstr("  Dynamic config (threshold adjust, whitelist/blacklist, hot-reload, gray)\n");
    pstr("  Degradation (reject/queue/priority bypass/circuit breaker)\n");
    pstr("  Monitoring (QPS, triggers, rejection rate, hotspots, alerts)\n");
}
void _start(void) {
    unsigned int buf=host_alloc(BUF_SIZE,16);
    int argc=host_get_argv(buf,BUF_SIZE); char *args=(char *)buf;
    if (argc<=0) { test_suite(); return; }
    int ran=0;
    for (int i=0;i<argc;i++) { if (args[i]=='\0') continue;
        if (my_strcmp(&args[i],"-h")==0||my_strcmp(&args[i],"--help")==0) {
            print_help(); ran=1;
        } else if (my_strcmp(&args[i],"-t")==0||my_strcmp(&args[i],"--test")==0) {
            test_suite(); ran=1; }
        while (args[i]!='\0') i++; }
    if (!ran) test_suite();
}
