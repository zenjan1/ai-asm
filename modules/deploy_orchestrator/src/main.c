/* deploy_orchestrator: Deployment orchestration module (v1.0)
 * CI/CD, blue-green, rolling updates, config, status, multi-cluster. */
#include <stddef.h>

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_DEPLOYS 16
#define MAX_ENVS     4
#define MAX_CLUSTERS 8
#define MAX_CONFIGS 32
#define MAX_LOGS    64
#define MAX_NAME    64
#define MAX_VAL    128
#define MAX_LOG    128

/* Strategies */
#define STRAT_CICD       0
#define STRAT_BLUE_GREEN 1
#define STRAT_ROLLING    2
/* States */
#define ST_PENDING    0
#define ST_BUILDING   1
#define ST_DEPLOYING  2
#define ST_HEALTH_CHK 3
#define ST_ACTIVE     4
#define ST_ROLLBACK   5
#define ST_ROLLED_BACK 6
#define ST_FAILED     7
#define ST_PAUSED     8
/* Environments */
#define ENV_DEV     0
#define ENV_STAGING 1
#define ENV_PROD    2
#define ENV_CUSTOM  3
/* Health */
#define HLTH_UNKNOWN   0
#define HLTH_HEALTHY   1
#define HLTH_DEGRADED  2
#define HLTH_UNHEALTHY 3
/* Config types */
#define CFG_ENV_VAR 0
#define CFG_SECRET  1
#define CFG_FILE    2
/* Log levels */
#define LOG_INFO  0
#define LOG_WARN  1
#define LOG_ERROR 2
/* Routing modes */
#define ROUTE_PRIMARY 0
#define ROUTE_CANARY  1
#define ROUTE_WEIGHTED 2
#define ROUTE_SPLIT   3

/* ══════ Structs ══════ */
typedef struct { char name[MAX_NAME]; int env_type, active_deploy_id, healthy; } env_t;
typedef struct { char name[MAX_NAME], region[MAX_NAME]; int capacity, current_load, healthy; } cluster_t;
typedef struct { char key[MAX_NAME], val[MAX_VAL]; int type, encrypted; } config_t;
typedef struct { int deploy_id, level; char msg[MAX_LOG]; } log_entry_t;
typedef struct {
    int id; char name[MAX_NAME];
    int env, strategy, state, version, batch_size;
    int batches_total, batches_done, health, active_slot, traffic_pct, cluster_id;
} deploy_t;

/* ══════ Static State ══════ */
static env_t       g_envs[MAX_ENVS];        static int g_env_n = 0;
static cluster_t   g_clusters[MAX_CLUSTERS]; static int g_cl_n = 0;
static config_t    g_cfgs[MAX_CONFIGS];      static int g_cfg_n = 0;
static deploy_t    g_deps[MAX_DEPLOYS];      static int g_dep_n = 0;
static int         g_next_id = 1;
static log_entry_t g_logs[MAX_LOGS];         static int g_log_n = 0;

/* ══════ Utilities ══════ */
static int my_strlen(const char *s) { int n=0; while(s[n]) n++; return n; }
static int my_strcmp(const char *a, const char *b) {
    while(*a&&*b){if(*a!=*b) return *a-*b; a++;b++;} return *a-*b;
}
static void my_strncpy(char *d, const char *s, int n) {
    int i=0; while(i<n-1&&s[i]){d[i]=s[i];i++;} d[i]='\0';
}
static void ps(const char *s) { host_print(s); }
static void pi(int v) {
    char b[32]; int p=0;
    if(v<0){b[p++]='-';v=-v;}
    if(v==0){b[p++]='0';} else {
        int d=0,t=v; while(t>0){d++;t/=10;} p+=d; b[p]='\0'; p--;
        while(v>0){b[p--]='0'+(v%10);v/=10;}
    }
    host_print(b);
}
static void pn(void) { ps("\n"); }
static void pl(const char *s) { ps(s); pn(); }

/* ══════ Logging ══════ */
static void add_log(int did, int lv, const char *msg) {
    if(g_log_n>=MAX_LOGS) return;
    g_logs[g_log_n].deploy_id=did; g_logs[g_log_n].level=lv;
    my_strncpy(g_logs[g_log_n].msg, msg, MAX_LOG-1); g_log_n++;
}
static const char *lv_str(int l){return l==LOG_INFO?"INFO":l==LOG_WARN?"WARN":"ERROR";}

/* ══════ Name Helpers ══════ */
static const char *st_name(int s) {
    switch(s){case ST_PENDING:return"PENDING";case ST_BUILDING:return"BUILDING";
    case ST_DEPLOYING:return"DEPLOYING";case ST_HEALTH_CHK:return"HEALTH_CHECK";
    case ST_ACTIVE:return"ACTIVE";case ST_ROLLBACK:return"ROLLING_BACK";
    case ST_ROLLED_BACK:return"ROLLED_BACK";case ST_FAILED:return"FAILED";
    case ST_PAUSED:return"PAUSED";default:return"UNKNOWN";}
}
static const char *strat_name(int s){return s==STRAT_CICD?"CI/CD":s==STRAT_BLUE_GREEN?"BLUE-GREEN":"ROLLING";}
static const char *env_str(int e){return e==ENV_DEV?"dev":e==ENV_STAGING?"staging":e==ENV_PROD?"prod":"custom";}
static const char *hlth_name(int h){return h==HLTH_HEALTHY?"HEALTHY":h==HLTH_DEGRADED?"DEGRADED":h==HLTH_UNHEALTHY?"UNHEALTHY":"UNKNOWN";}

/* ══════ Environment Management ══════ */
static int register_env(const char *name, int etype) {
    if(g_env_n>=MAX_ENVS){pl("[deploy] env limit reached");return -1;}
    for(int i=0;i<g_env_n;i++) if(!my_strcmp(g_envs[i].name,name)){
        ps("[deploy] env exists: ");ps(name);pn();return i;
    }
    env_t *e=&g_envs[g_env_n]; my_strncpy(e->name,name,MAX_NAME-1);
    e->env_type=etype; e->active_deploy_id=-1; e->healthy=1;
    int idx=g_env_n++;
    ps("[deploy] registered env: ");ps(name);ps(" (");ps(env_str(etype));ps(")");pn();
    return idx;
}
static int find_env(const char *name){for(int i=0;i<g_env_n;i++)if(!my_strcmp(g_envs[i].name,name))return i;return -1;}

/* ══════ Cluster Management ══════ */
static int register_cluster(const char *name, const char *region, int cap) {
    if(g_cl_n>=MAX_CLUSTERS){pl("[deploy] cluster limit reached");return -1;}
    cluster_t *c=&g_clusters[g_cl_n]; my_strncpy(c->name,name,MAX_NAME-1);
    my_strncpy(c->region,region,MAX_NAME-1); c->capacity=cap;c->current_load=0;c->healthy=1;
    int idx=g_cl_n++;
    ps("[deploy] registered cluster: ");ps(name);ps(" region=");ps(region);
    ps(" capacity=");pi(cap);pn(); return idx;
}
static int find_cluster(const char *name){for(int i=0;i<g_cl_n;i++)if(!my_strcmp(g_clusters[i].name,name))return i;return -1;}

/* ══════ Config Management ══════ */
static int set_config(const char *key, const char *val, int type, int enc) {
    for(int i=0;i<g_cfg_n;i++) if(!my_strcmp(g_cfgs[i].key,key)){
        my_strncpy(g_cfgs[i].val,val,MAX_VAL-1);g_cfgs[i].type=type;g_cfgs[i].encrypted=enc;
        ps("[deploy] updated config: ");ps(key);pn();return i;
    }
    if(g_cfg_n>=MAX_CONFIGS){pl("[deploy] config limit reached");return -1;}
    config_t *c=&g_cfgs[g_cfg_n]; my_strncpy(c->key,key,MAX_NAME-1);
    my_strncpy(c->val,val,MAX_VAL-1);c->type=type;c->encrypted=enc;
    int idx=g_cfg_n++;
    ps("[deploy] set config: ");ps(key);if(enc)ps(" [encrypted]");pn(); return idx;
}
static int get_config(const char *key){for(int i=0;i<g_cfg_n;i++)if(!my_strcmp(g_cfgs[i].key,key))return i;return -1;}

/* ══════ Deployment Core ══════ */
static int create_deploy(const char *name, int env, int strat, int ver, int cid) {
    if(g_dep_n>=MAX_DEPLOYS){pl("[deploy] deploy limit reached");return -1;}
    deploy_t *d=&g_deps[g_dep_n]; d->id=g_next_id++;
    my_strncpy(d->name,name,MAX_NAME-1); d->env=env;d->strategy=strat;d->state=ST_PENDING;
    d->version=ver;d->batch_size=2;d->batches_total=(strat==STRAT_ROLLING)?4:1;
    d->batches_done=0;d->health=HLTH_UNKNOWN;d->active_slot=0;d->traffic_pct=0;d->cluster_id=cid;
    int id=d->id; g_dep_n++;
    add_log(id,LOG_INFO,"deployment created");
    ps("[deploy] created id=");pi(id);ps(" name=");ps(name);
    ps(" strat=");ps(strat_name(strat));ps(" env=");ps(env_str(env));pn();
    return id;
}
static deploy_t *find_deploy(int id){for(int i=0;i<g_dep_n;i++)if(g_deps[i].id==id)return&g_deps[i];return NULL;}

/* Advance deployment through pipeline stages */
static int advance_deploy(int id) {
    deploy_t *d=find_deploy(id);
    if(!d){ps("[deploy] not found: ");pi(id);pn();return -1;}
    switch(d->state){
    case ST_PENDING: d->state=ST_BUILDING; add_log(id,LOG_INFO,"build started");
        ps("[deploy] id=");pi(id);ps(" -> BUILDING");pn(); break;
    case ST_BUILDING: d->state=ST_DEPLOYING; add_log(id,LOG_INFO,"build complete, deploying");
        ps("[deploy] id=");pi(id);ps(" -> DEPLOYING");pn(); break;
    case ST_DEPLOYING:
        if(d->strategy==STRAT_ROLLING){
            d->batches_done++;
            if(d->batches_done<d->batches_total){
                add_log(id,LOG_INFO,"batch completed");
                ps("[deploy] id=");pi(id);ps(" batch ");pi(d->batches_done);
                ps("/");pi(d->batches_total);ps(" done");pn();
            } else { d->state=ST_HEALTH_CHK; add_log(id,LOG_INFO,"all batches deployed"); }
        } else if(d->strategy==STRAT_BLUE_GREEN){
            d->active_slot=(d->active_slot==0)?1:0; d->traffic_pct=100;
            d->state=ST_HEALTH_CHK; add_log(id,LOG_INFO,"switched active slot");
        } else { d->state=ST_HEALTH_CHK; d->traffic_pct=100; add_log(id,LOG_INFO,"deploy done"); }
        break;
    case ST_HEALTH_CHK: d->health=HLTH_HEALTHY; d->state=ST_ACTIVE;
        add_log(id,LOG_INFO,"health check passed");
        ps("[deploy] id=");pi(id);ps(" -> ACTIVE (healthy)");pn(); break;
    case ST_ACTIVE: ps("[deploy] id=");pi(id);ps(" already ACTIVE");pn(); break;
    case ST_PAUSED: d->state=ST_DEPLOYING; add_log(id,LOG_INFO,"resumed");
        ps("[deploy] id=");pi(id);ps(" resumed");pn(); break;
    default: ps("[deploy] id=");pi(id);ps(" cannot advance from ");ps(st_name(d->state));pn(); return -1;
    } return 0;
}

/* ══════ Blue-Green Rollback ══════ */
static int rollback_blue_green(int id) {
    deploy_t *d=find_deploy(id);
    if(!d||d->strategy!=STRAT_BLUE_GREEN){pl("[deploy] rollback: not blue-green");return -1;}
    d->active_slot=(d->active_slot==0)?1:0; d->state=ST_ROLLED_BACK; d->traffic_pct=100;
    add_log(id,LOG_WARN,"blue-green rollback executed");
    ps("[deploy] id=");pi(id);ps(" rolled back to slot ");ps(d->active_slot==0?"blue":"green");pn();
    return 0;
}

/* ══════ Rolling Pause ══════ */
static int pause_rolling(int id) {
    deploy_t *d=find_deploy(id);
    if(!d||d->strategy!=STRAT_ROLLING){pl("[deploy] pause: not rolling");return -1;}
    if(d->state!=ST_DEPLOYING){pl("[deploy] pause: not deploying");return -1;}
    d->state=ST_PAUSED; add_log(id,LOG_WARN,"rolling update paused");
    ps("[deploy] id=");pi(id);ps(" paused at batch ");pi(d->batches_done);
    ps("/");pi(d->batches_total);pn(); return 0;
}

/* ══════ Health Check ══════ */
static int run_health_check(int id) {
    deploy_t *d=find_deploy(id); if(!d) return -1;
    int ok=1;
    if(d->cluster_id>=0&&d->cluster_id<g_cl_n) ok=g_clusters[d->cluster_id].healthy;
    if(ok){ d->health=HLTH_HEALTHY; add_log(id,LOG_INFO,"health check: HEALTHY");
        ps("[deploy] id=");pi(id);ps(" health=HEALTHY");pn();
    } else { d->health=HLTH_UNHEALTHY; d->state=ST_FAILED;
        add_log(id,LOG_ERROR,"health check: UNHEALTHY");
        ps("[deploy] id=");pi(id);ps(" health=UNHEALTHY");pn();
    } return d->health;
}

/* ══════ Traffic Routing ══════ */
static int set_traffic(int id, int mode, int pct) {
    deploy_t *d=find_deploy(id); if(!d) return -1;
    if(pct<0)pct=0; if(pct>100)pct=100; d->traffic_pct=pct;
    const char *m=mode==ROUTE_CANARY?"CANARY":mode==ROUTE_WEIGHTED?"WEIGHTED":mode==ROUTE_SPLIT?"SPLIT":"PRIMARY";
    add_log(id,LOG_INFO,"traffic routing updated");
    ps("[deploy] id=");pi(id);ps(" traffic=");ps(m);ps(" pct=");pi(pct);ps("%");pn(); return 0;
}

/* ══════ Status Display ══════ */
static void show_deploy_status(int id) {
    deploy_t *d=find_deploy(id); if(!d){ps("[deploy] not found: ");pi(id);pn();return;}
    ps("  id=");pi(d->id);ps(" name=");ps(d->name);ps(" env=");ps(env_str(d->env));
    ps(" strat=");ps(strat_name(d->strategy));ps(" state=");ps(st_name(d->state));
    ps(" v");pi(d->version);ps(" health=");ps(hlth_name(d->health));
    ps(" traffic=");pi(d->traffic_pct);ps("%");
    if(d->strategy==STRAT_ROLLING){ps(" batch=");pi(d->batches_done);ps("/");pi(d->batches_total);}
    if(d->strategy==STRAT_BLUE_GREEN){ps(" slot=");ps(d->active_slot==0?"blue":"green");}
    pn();
}
static void show_all_status(void) {
    pl("=== Deploy Orchestrator Status ===");
    ps("Envs: ");pi(g_env_n);ps(" Clusters: ");pi(g_cl_n);
    ps(" Configs: ");pi(g_cfg_n);ps(" Deploys: ");pi(g_dep_n);pn();
    for(int i=0;i<g_dep_n;i++) show_deploy_status(g_deps[i].id);
}
static void show_logs(int did) {
    pl("--- Deployment Logs ---"); int found=0;
    for(int i=0;i<g_log_n;i++) if(did<0||g_logs[i].deploy_id==did){
        ps("  [");ps(lv_str(g_logs[i].level));ps("] deploy=");pi(g_logs[i].deploy_id);
        ps(": ");ps(g_logs[i].msg);pn(); found++;
    } if(!found) pl("  (no logs)");
}
static void show_configs(void) {
    pl("--- Configuration ---");
    if(!g_cfg_n){pl("  (no configs)");return;}
    for(int i=0;i<g_cfg_n;i++){
        ps("  ");ps(g_cfgs[i].key);ps("=");
        ps(g_cfgs[i].encrypted?"********":g_cfgs[i].val);
        ps(g_cfgs[i].type==CFG_SECRET?" [secret]":g_cfgs[i].type==CFG_FILE?" [file]":" [env]");pn();
    }
}

/* ══════ Help ══════ */
static void show_help(void) {
    pl("deploy_orchestrator: Deployment orchestration (v1.0)");
    pl("Usage: deploy_orchestrator [OPTIONS]");
    pl("  -h, --help     Show this help");
    pl("  -t, --test     Run test suite");
    pl("  -s, --status   Show deployment status");
    pl("  -c, --configs  Show configuration");
    pl("  -l, --logs     Show deployment logs");
    pl("Strategies: CI/CD, Blue-Green (zero-downtime), Rolling (batch updates)");
}

/* ══════ Test Suite ══════ */
static void test_envs(void) {
    pl("\n--- Test: Environments ---");
    int dev=register_env("dev",ENV_DEV), stg=register_env("staging",ENV_STAGING),
        prd=register_env("prod",ENV_PROD);
    if(dev>=0&&stg>=0&&prd>=0) pl("[test] register envs: OK"); else pl("[test] register envs: FAIL");
    if(find_env("dev")==dev&&find_env("prod")==prd) pl("[test] find envs: OK"); else pl("[test] find envs: FAIL");
    if(register_env("dev",ENV_DEV)==dev) pl("[test] duplicate env: OK");
}
static void test_clusters(void) {
    pl("\n--- Test: Clusters ---");
    int c1=register_cluster("us-east-1","us-east",100),
        c2=register_cluster("eu-west-1","eu-west",80),
        c3=register_cluster("ap-south-1","ap-south",60);
    if(c1>=0&&c2>=0&&c3>=0) pl("[test] register clusters: OK"); else pl("[test] register clusters: FAIL");
    if(find_cluster("eu-west-1")==c2) pl("[test] find cluster: OK"); else pl("[test] find cluster: FAIL");
}
static void test_configs(void) {
    pl("\n--- Test: Configs ---");
    int c1=set_config("DATABASE_URL","postgres://db:5432/app",CFG_ENV_VAR,0),
        c2=set_config("API_KEY","sk-secret-12345",CFG_SECRET,1),
        c3=set_config("nginx.conf","/etc/nginx/nginx.conf",CFG_FILE,0);
    if(c1>=0&&c2>=0&&c3>=0) pl("[test] set configs: OK"); else pl("[test] set configs: FAIL");
    if(get_config("API_KEY")==c2) pl("[test] get config: OK");
    set_config("DATABASE_URL","postgres://db:5432/app_v2",CFG_ENV_VAR,0);
    if(get_config("DATABASE_URL")==c1) pl("[test] update config: OK");
}
static void test_cicd(void) {
    pl("\n--- Test: CI/CD Pipeline ---");
    int d=create_deploy("web-api",ENV_STAGING,STRAT_CICD,42,0);
    if(d<0){pl("[test] create cicd: FAIL");return;}
    advance_deploy(d); advance_deploy(d); advance_deploy(d); advance_deploy(d);
    run_health_check(d);
    deploy_t *p=find_deploy(d);
    if(p&&p->state==ST_ACTIVE&&p->health==HLTH_HEALTHY) pl("[test] cicd pipeline: OK");
    else pl("[test] cicd pipeline: FAIL");
}
static void test_blue_green(void) {
    pl("\n--- Test: Blue-Green Deployment ---");
    int d=create_deploy("frontend",ENV_PROD,STRAT_BLUE_GREEN,15,1);
    if(d<0){pl("[test] create bg: FAIL");return;}
    advance_deploy(d); advance_deploy(d); advance_deploy(d); advance_deploy(d);
    deploy_t *p=find_deploy(d);
    if(p&&p->state==ST_ACTIVE&&p->active_slot==1) pl("[test] blue-green deploy: OK (slot=green)");
    else pl("[test] blue-green deploy: FAIL");
    rollback_blue_green(d); p=find_deploy(d);
    if(p&&p->state==ST_ROLLED_BACK&&p->active_slot==0) pl("[test] blue-green rollback: OK (slot=blue)");
    else pl("[test] blue-green rollback: FAIL");
}
static void test_rolling(void) {
    pl("\n--- Test: Rolling Update ---");
    int d=create_deploy("worker",ENV_PROD,STRAT_ROLLING,7,2);
    if(d<0){pl("[test] create rolling: FAIL");return;}
    advance_deploy(d); advance_deploy(d);
    advance_deploy(d); advance_deploy(d); /* batch 1,2 */
    pause_rolling(d);
    deploy_t *p=find_deploy(d);
    if(p&&p->state==ST_PAUSED) pl("[test] rolling pause: OK"); else pl("[test] rolling pause: FAIL");
    advance_deploy(d); /* resume */ advance_deploy(d); advance_deploy(d); /* batch 3,4 */
    advance_deploy(d); /* health_chk */ advance_deploy(d); /* active */
    run_health_check(d); p=find_deploy(d);
    if(p&&p->state==ST_ACTIVE&&p->batches_done==p->batches_total) pl("[test] rolling complete: OK");
    else pl("[test] rolling complete: FAIL");
}
static void test_traffic(void) {
    pl("\n--- Test: Traffic Routing ---");
    int d=create_deploy("gateway",ENV_PROD,STRAT_CICD,3,0);
    if(d<0){pl("[test] traffic create: FAIL");return;}
    advance_deploy(d);advance_deploy(d);advance_deploy(d);advance_deploy(d);
    set_traffic(d,ROUTE_CANARY,10);
    if(find_deploy(d)->traffic_pct==10) pl("[test] canary traffic: OK");
    set_traffic(d,ROUTE_WEIGHTED,50);
    if(find_deploy(d)->traffic_pct==50) pl("[test] weighted traffic: OK");
    set_traffic(d,ROUTE_PRIMARY,100);
    if(find_deploy(d)->traffic_pct==100) pl("[test] full traffic: OK");
}
static void test_multi_cluster(void) {
    pl("\n--- Test: Multi-Cluster ---");
    int d1=create_deploy("svc-a",ENV_PROD,STRAT_CICD,10,0),
        d2=create_deploy("svc-b",ENV_PROD,STRAT_CICD,5,1),
        d3=create_deploy("svc-c",ENV_PROD,STRAT_ROLLING,3,2);
    if(d1>=0&&d2>=0&&d3>=0) pl("[test] multi-cluster deploy: OK"); else pl("[test] multi-cluster: FAIL");
    deploy_t *p1=find_deploy(d1),*p2=find_deploy(d2),*p3=find_deploy(d3);
    if(p1&&p1->cluster_id==0&&p2&&p2->cluster_id==1&&p3&&p3->cluster_id==2)
        pl("[test] cluster assignment: OK"); else pl("[test] cluster assignment: FAIL");
}
static void run_tests(void) {
    pl("=== Deploy Orchestrator Test Suite ===");
    test_envs(); test_clusters(); test_configs();
    test_cicd(); test_blue_green(); test_rolling();
    test_traffic(); test_multi_cluster();
    pl("\n--- Log Collection ---");
    if(g_log_n>0){ps("[test] logs: ");pi(g_log_n);ps(" entries: OK");pn();}
    else pl("[test] logs: FAIL (empty)");
    pl("\n--- Final Status ---");
    show_all_status(); show_configs(); show_logs(-1);
    pl("\n[test] all tests complete");
}

/* ══════ Entry Point ══════ */
void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);
    int help=0,test=0,status=0,cfgs=0,logs=0;
    unsigned int pos=0; char *av=(char *)buf;
    while(pos<512&&av[pos])pos++; pos++;
    while(pos<512&&av[pos]){
        char *a=&av[pos];
        if(!my_strcmp(a,"-h")||!my_strcmp(a,"--help")) help=1;
        else if(!my_strcmp(a,"-t")||!my_strcmp(a,"--test")) test=1;
        else if(!my_strcmp(a,"-s")||!my_strcmp(a,"--status")) status=1;
        else if(!my_strcmp(a,"-c")||!my_strcmp(a,"--configs")) cfgs=1;
        else if(!my_strcmp(a,"-l")||!my_strcmp(a,"--logs")) logs=1;
        while(pos<512&&av[pos])pos++; pos++;
    }
    if(help){show_help();host_exit(0);} if(test){run_tests();host_exit(0);}
    if(status){show_all_status();host_exit(0);} if(cfgs){show_configs();host_exit(0);}
    if(logs){show_logs(-1);host_exit(0);}
    ps("deploy_orchestrator: use -h for help, -t for test\n");
    host_exit(0);
}
