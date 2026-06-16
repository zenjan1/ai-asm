/* deploy_orchestrator: Deployment orchestration module (v1.0)
 *
 * Provides: CI/CD pipelines, blue-green deployment, rolling updates,
 * config management, status tracking, multi-cluster deployment.
 */

#include <stddef.h>

/* Host functions (provided by WASM runtime) */
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* ── Limits ── */
#define MAX_DEPLOYS     16
#define MAX_ENVS         4
#define MAX_CLUSTERS     8
#define MAX_CONFIGS     32
#define MAX_LOGS        64
#define MAX_NAME        64
#define MAX_VAL        128
#define MAX_LOG        128
#define BATCH_DEFAULT    2

/* ── Deployment strategies ── */
#define STRAT_CICD       0
#define STRAT_BLUE_GREEN 1
#define STRAT_ROLLING    2

/* ── Deployment states ── */
#define ST_PENDING       0
#define ST_BUILDING      1
#define ST_DEPLOYING     2
#define ST_HEALTH_CHK    3
#define ST_ACTIVE        4
#define ST_ROLLBACK      5
#define ST_ROLLED_BACK   6
#define ST_FAILED        7
#define ST_PAUSED        8

/* ── Environments ── */
#define ENV_DEV          0
#define ENV_STAGING      1
#define ENV_PROD         2
#define ENV_CUSTOM       3

/* ── Health status ── */
#define HLTH_UNKNOWN     0
#define HLTH_HEALTHY     1
#define HLTH_DEGRADED    2
#define HLTH_UNHEALTHY   3

/* ── Config types ── */
#define CFG_ENV_VAR      0
#define CFG_SECRET       1
#define CFG_FILE         2

/* ── Log levels ── */
#define LOG_INFO         0
#define LOG_WARN         1
#define LOG_ERROR        2

/* ── Traffic routing modes ── */
#define ROUTE_PRIMARY    0
#define ROUTE_CANARY     1
#define ROUTE_WEIGHTED   2
#define ROUTE_SPLIT      3

/* ══════════════════════ Structs ══════════════════════ */

typedef struct {
    char name[MAX_NAME];
    int  env_type;
    int  active_deploy_id;
    int  healthy;
} env_t;

typedef struct {
    char name[MAX_NAME];
    char region[MAX_NAME];
    int  capacity;
    int  current_load;
    int  healthy;
} cluster_t;

typedef struct {
    char key[MAX_NAME];
    char val[MAX_VAL];
    int  type;
    int  encrypted;
} config_t;

typedef struct {
    int  deploy_id;
    int  level;
    char msg[MAX_LOG];
} log_entry_t;

typedef struct {
    int  id;
    char name[MAX_NAME];
    int  env;
    int  strategy;
    int  state;
    int  version;
    int  batch_size;
    int  batches_total;
    int  batches_done;
    int  health;
    int  active_slot;    /* blue=0, green=1 */
    int  traffic_pct;
    int  cluster_id;
} deploy_t;

/* ══════════════════════ Static State ══════════════════════ */

static env_t        g_envs[MAX_ENVS];
static int          g_env_n       = 0;
static cluster_t    g_clusters[MAX_CLUSTERS];
static int          g_cluster_n   = 0;
static config_t     g_cfgs[MAX_CONFIGS];
static int          g_cfg_n       = 0;
static deploy_t     g_deploys[MAX_DEPLOYS];
static int          g_deploy_n    = 0;
static int          g_next_id     = 1;
static log_entry_t  g_logs[MAX_LOGS];
static int          g_log_n       = 0;

/* ══════════════════════ String Utilities ══════════════════════ */

static int my_strlen(const char *s)
{
    int len = 0;
    while (s[len]) len++;
    return len;
}

static int my_strcmp(const char *a, const char *b)
{
    while (*a && *b) {
        if (*a != *b) return *a - *b;
        a++; b++;
    }
    return *a - *b;
}

static void my_strcpy(char *dst, const char *src)
{
    while (*src) *dst++ = *src++;
    *dst = '\0';
}

static void my_strncpy(char *dst, const char *src, int n)
{
    int i = 0;
    while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; }
    dst[i] = '\0';
}

/* ── Print helpers ── */

static void print_str(const char *s) { host_print(s); }

static void print_int(int val)
{
    char buf[32];
    int pos = 0;
    if (val < 0) { buf[pos++] = '-'; val = -val; }
    if (val == 0) { buf[pos++] = '0'; }
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

/* ══════════════════════ Logging ══════════════════════ */

static void add_log(int deploy_id, int level, const char *msg)
{
    if (g_log_n >= MAX_LOGS) return;
    log_entry_t *e = &g_logs[g_log_n];
    e->deploy_id = deploy_id;
    e->level     = level;
    my_strncpy(e->msg, msg, MAX_LOG - 1);
    g_log_n++;
}

static const char *level_str(int lv)
{
    switch (lv) {
        case LOG_INFO:  return "INFO";
        case LOG_WARN:  return "WARN";
        case LOG_ERROR: return "ERROR";
        default:        return "?";
    }
}

/* ══════════════════════ Name Helpers ══════════════════════ */

static const char *state_name(int s)
{
    switch (s) {
        case ST_PENDING:     return "PENDING";
        case ST_BUILDING:    return "BUILDING";
        case ST_DEPLOYING:   return "DEPLOYING";
        case ST_HEALTH_CHK:  return "HEALTH_CHECK";
        case ST_ACTIVE:      return "ACTIVE";
        case ST_ROLLBACK:    return "ROLLING_BACK";
        case ST_ROLLED_BACK: return "ROLLED_BACK";
        case ST_FAILED:      return "FAILED";
        case ST_PAUSED:      return "PAUSED";
        default:             return "UNKNOWN";
    }
}

static const char *strategy_name(int s)
{
    switch (s) {
        case STRAT_CICD:       return "CI/CD";
        case STRAT_BLUE_GREEN: return "BLUE-GREEN";
        case STRAT_ROLLING:    return "ROLLING";
        default:               return "UNKNOWN";
    }
}

static const char *env_name(int e)
{
    switch (e) {
        case ENV_DEV:     return "dev";
        case ENV_STAGING: return "staging";
        case ENV_PROD:    return "prod";
        case ENV_CUSTOM:  return "custom";
        default:          return "?";
    }
}

static const char *health_name(int h)
{
    switch (h) {
        case HLTH_HEALTHY:   return "HEALTHY";
        case HLTH_DEGRADED:  return "DEGRADED";
        case HLTH_UNHEALTHY: return "UNHEALTHY";
        default:             return "UNKNOWN";
    }
}

/* ══════════════════════ Environment Management ══════════════════════ */

static int register_env(const char *name, int env_type)
{
    if (g_env_n >= MAX_ENVS) {
        print_str("[deploy] env limit reached\n");
        return -1;
    }
    for (int i = 0; i < g_env_n; i++) {
        if (my_strcmp(g_envs[i].name, name) == 0) {
            print_str("[deploy] env already exists: ");
            print_str(name);
            print_str("\n");
            return i;
        }
    }
    env_t *e   = &g_envs[g_env_n];
    my_strncpy(e->name, name, MAX_NAME - 1);
    e->env_type        = env_type;
    e->active_deploy_id = -1;
    e->healthy         = 1;
    int idx = g_env_n++;

    print_str("[deploy] registered env: ");
    print_str(name);
    print_str(" (");
    print_str(env_name(env_type));
    print_str(")\n");
    return idx;
}

static int find_env(const char *name)
{
    for (int i = 0; i < g_env_n; i++) {
        if (my_strcmp(g_envs[i].name, name) == 0) return i;
    }
    return -1;
}

/* ══════════════════════ Cluster Management ══════════════════════ */

static int register_cluster(const char *name, const char *region, int capacity)
{
    if (g_cluster_n >= MAX_CLUSTERS) {
        print_str("[deploy] cluster limit reached\n");
        return -1;
    }
    cluster_t *c = &g_clusters[g_cluster_n];
    my_strncpy(c->name, name, MAX_NAME - 1);
    my_strncpy(c->region, region, MAX_NAME - 1);
    c->capacity     = capacity;
    c->current_load = 0;
    c->healthy      = 1;
    int idx = g_cluster_n++;

    print_str("[deploy] registered cluster: ");
    print_str(name);
    print_str(" region=");
    print_str(region);
    print_str(" capacity=");
    print_int(capacity);
    print_str("\n");
    return idx;
}

static int find_cluster(const char *name)
{
    for (int i = 0; i < g_cluster_n; i++) {
        if (my_strcmp(g_clusters[i].name, name) == 0) return i;
    }
    return -1;
}

/* ══════════════════════ Config Management ══════════════════════ */

static int set_config(const char *key, const char *val, int type, int encrypted)
{
    /* Update existing key */
    for (int i = 0; i < g_cfg_n; i++) {
        if (my_strcmp(g_cfgs[i].key, key) == 0) {
            my_strncpy(g_cfgs[i].val, val, MAX_VAL - 1);
            g_cfgs[i].type      = type;
            g_cfgs[i].encrypted = encrypted;
            print_str("[deploy] updated config: ");
            print_str(key);
            print_str("\n");
            return i;
        }
    }
    if (g_cfg_n >= MAX_CONFIGS) {
        print_str("[deploy] config limit reached\n");
        return -1;
    }
    config_t *c = &g_cfgs[g_cfg_n];
    my_strncpy(c->key, key, MAX_NAME - 1);
    my_strncpy(c->val, val, MAX_VAL - 1);
    c->type      = type;
    c->encrypted = encrypted;
    int idx = g_cfg_n++;

    print_str("[deploy] set config: ");
    print_str(key);
    if (encrypted) print_str(" [encrypted]");
    print_str("\n");
    return idx;
}

static int get_config(const char *key)
{
    for (int i = 0; i < g_cfg_n; i++) {
        if (my_strcmp(g_cfgs[i].key, key) == 0) return i;
    }
    return -1;
}

/* ══════════════════════ Deployment Core ══════════════════════ */

static int create_deploy(const char *name, int env, int strategy,
                         int version, int cluster_id)
{
    if (g_deploy_n >= MAX_DEPLOYS) {
        print_str("[deploy] deploy limit reached\n");
        return -1;
    }
    deploy_t *d = &g_deploys[g_deploy_n];
    d->id            = g_next_id++;
    my_strncpy(d->name, name, MAX_NAME - 1);
    d->env           = env;
    d->strategy      = strategy;
    d->state         = ST_PENDING;
    d->version       = version;
    d->batch_size    = BATCH_DEFAULT;
    d->batches_total = (strategy == STRAT_ROLLING) ? 4 : 1;
    d->batches_done  = 0;
    d->health        = HLTH_UNKNOWN;
    d->active_slot   = 0;
    d->traffic_pct   = 0;
    d->cluster_id    = cluster_id;
    int id = d->id;
    g_deploy_n++;

    add_log(id, LOG_INFO, "deployment created");

    print_str("[deploy] created deployment id=");
    print_int(id);
    print_str(" name=");
    print_str(name);
    print_str(" strategy=");
    print_str(strategy_name(strategy));
    print_str(" env=");
    print_str(env_name(env));
    print_str("\n");
    return id;
}

static deploy_t *find_deploy(int id)
{
    for (int i = 0; i < g_deploy_n; i++) {
        if (g_deploys[i].id == id) return &g_deploys[i];
    }
    return NULL;
}

/* Advance a deployment through its pipeline stages.
   Returns 0 on success, -1 on failure. */
static int advance_deploy(int id)
{
    deploy_t *d = find_deploy(id);
    if (!d) {
        print_str("[deploy] deployment not found: ");
        print_int(id);
        print_str("\n");
        return -1;
    }

    switch (d->state) {
    case ST_PENDING:
        d->state = ST_BUILDING;
        add_log(id, LOG_INFO, "build started");
        print_str("[deploy] id=");
        print_int(id);
        print_str(" -> BUILDING\n");
        break;

    case ST_BUILDING:
        d->state = ST_DEPLOYING;
        add_log(id, LOG_INFO, "build complete, deploying");
        print_str("[deploy] id=");
        print_int(id);
        print_str(" -> DEPLOYING\n");
        break;

    case ST_DEPLOYING:
        if (d->strategy == STRAT_ROLLING) {
            d->batches_done++;
            if (d->batches_done < d->batches_total) {
                add_log(id, LOG_INFO, "batch completed, next batch");
                print_str("[deploy] id=");
                print_int(id);
                print_str(" batch ");
                print_int(d->batches_done);
                print_str("/");
                print_int(d->batches_total);
                print_str(" done\n");
            } else {
                d->state = ST_HEALTH_CHK;
                add_log(id, LOG_INFO, "all batches deployed");
            }
        } else if (d->strategy == STRAT_BLUE_GREEN) {
            d->active_slot = (d->active_slot == 0) ? 1 : 0;
            d->traffic_pct = 100;
            d->state       = ST_HEALTH_CHK;
            add_log(id, LOG_INFO, "switched active slot");
        } else {
            d->state       = ST_HEALTH_CHK;
            d->traffic_pct = 100;
            add_log(id, LOG_INFO, "deploy complete, checking health");
        }
        break;

    case ST_HEALTH_CHK:
        d->health = HLTH_HEALTHY;
        d->state  = ST_ACTIVE;
        add_log(id, LOG_INFO, "health check passed");
        print_str("[deploy] id=");
        print_int(id);
        print_str(" -> ACTIVE (healthy)\n");
        break;

    case ST_ACTIVE:
        print_str("[deploy] id=");
        print_int(id);
        print_str(" already ACTIVE\n");
        break;

    case ST_PAUSED:
        d->state = ST_DEPLOYING;
        add_log(id, LOG_INFO, "resumed from pause");
        print_str("[deploy] id=");
        print_int(id);
        print_str(" resumed\n");
        break;

    default:
        print_str("[deploy] id=");
        print_int(id);
        print_str(" cannot advance from ");
        print_str(state_name(d->state));
        print_str("\n");
        return -1;
    }
    return 0;
}

/* ══════════════════════ Blue-Green ══════════════════════ */

static int rollback_blue_green(int id)
{
    deploy_t *d = find_deploy(id);
    if (!d || d->strategy != STRAT_BLUE_GREEN) {
        print_str("[deploy] rollback: not a blue-green deployment\n");
        return -1;
    }
    d->active_slot = (d->active_slot == 0) ? 1 : 0;
    d->state       = ST_ROLLED_BACK;
    d->traffic_pct = 100;
    add_log(id, LOG_WARN, "blue-green rollback executed");

    print_str("[deploy] id=");
    print_int(id);
    print_str(" rolled back to slot ");
    print_str(d->active_slot == 0 ? "blue" : "green");
    print_str("\n");
    return 0;
}

/* ══════════════════════ Rolling Update ══════════════════════ */

static int pause_rolling(int id)
{
    deploy_t *d = find_deploy(id);
    if (!d || d->strategy != STRAT_ROLLING) {
        print_str("[deploy] pause: not a rolling deployment\n");
        return -1;
    }
    if (d->state != ST_DEPLOYING) {
        print_str("[deploy] pause: not in DEPLOYING state\n");
        return -1;
    }
    d->state = ST_PAUSED;
    add_log(id, LOG_WARN, "rolling update paused");

    print_str("[deploy] id=");
    print_int(id);
    print_str(" paused at batch ");
    print_int(d->batches_done);
    print_str("/");
    print_int(d->batches_total);
    print_str("\n");
    return 0;
}

/* ══════════════════════ Health Check ══════════════════════ */

static int run_health_check(int id)
{
    deploy_t *d = find_deploy(id);
    if (!d) return -1;

    /* Simulate health check: deployment is healthy if cluster is healthy */
    int cluster_ok = 1;
    if (d->cluster_id >= 0 && d->cluster_id < g_cluster_n) {
        cluster_ok = g_clusters[d->cluster_id].healthy;
    }

    if (cluster_ok) {
        d->health = HLTH_HEALTHY;
        add_log(id, LOG_INFO, "health check: HEALTHY");
        print_str("[deploy] id=");
        print_int(id);
        print_str(" health=HEALTHY\n");
    } else {
        d->health = HLTH_UNHEALTHY;
        d->state  = ST_FAILED;
        add_log(id, LOG_ERROR, "health check: UNHEALTHY, cluster down");
        print_str("[deploy] id=");
        print_int(id);
        print_str(" health=UNHEALTHY\n");
    }
    return d->health;
}

/* ══════════════════════ Traffic Routing ══════════════════════ */

static int set_traffic(int id, int mode, int pct)
{
    deploy_t *d = find_deploy(id);
    if (!d) return -1;

    if (pct < 0) pct = 0;
    if (pct > 100) pct = 100;
    d->traffic_pct = pct;

    const char *mode_s;
    switch (mode) {
        case ROUTE_CANARY:   mode_s = "CANARY";   break;
        case ROUTE_WEIGHTED: mode_s = "WEIGHTED"; break;
        case ROUTE_SPLIT:    mode_s = "SPLIT";    break;
        default:             mode_s = "PRIMARY";  break;
    }

    add_log(id, LOG_INFO, "traffic routing updated");
    print_str("[deploy] id=");
    print_int(id);
    print_str(" traffic=");
    print_str(mode_s);
    print_str(" pct=");
    print_int(pct);
    print_str("\n");
    return 0;
}

/* ══════════════════════ Status Display ══════════════════════ */

static void show_deploy_status(int id)
{
    deploy_t *d = find_deploy(id);
    if (!d) {
        print_str("[deploy] not found: ");
        print_int(id);
        print_str("\n");
        return;
    }
    print_str("  deploy id=");
    print_int(d->id);
    print_str(" name=");
    print_str(d->name);
    print_str(" env=");
    print_str(env_name(d->env));
    print_str(" strategy=");
    print_str(strategy_name(d->strategy));
    print_str(" state=");
    print_str(state_name(d->state));
    print_str(" v");
    print_int(d->version);
    print_str(" health=");
    print_str(health_name(d->health));
    print_str(" traffic=");
    print_int(d->traffic_pct);
    print_str("%");
    if (d->strategy == STRAT_ROLLING) {
        print_str(" batch=");
        print_int(d->batches_done);
        print_str("/");
        print_int(d->batches_total);
    }
    if (d->strategy == STRAT_BLUE_GREEN) {
        print_str(" slot=");
        print_str(d->active_slot == 0 ? "blue" : "green");
    }
    print_str("\n");
}

static void show_all_status(void)
{
    print_str("=== Deploy Orchestrator Status ===\n");
    print_str("Environments: ");
    print_int(g_env_n);
    print_str("  Clusters: ");
    print_int(g_cluster_n);
    print_str("  Configs: ");
    print_int(g_cfg_n);
    print_str("  Deployments: ");
    print_int(g_deploy_n);
    print_str("\n");

    if (g_deploy_n > 0) {
        print_str("Deployments:\n");
        for (int i = 0; i < g_deploy_n; i++) {
            show_deploy_status(g_deploys[i].id);
        }
    }
}

static void show_logs(int deploy_id)
{
    print_str("--- Deployment Logs ---\n");
    int found = 0;
    for (int i = 0; i < g_log_n; i++) {
        if (deploy_id < 0 || g_logs[i].deploy_id == deploy_id) {
            print_str("  [");
            print_str(level_str(g_logs[i].level));
            print_str("] deploy=");
            print_int(g_logs[i].deploy_id);
            print_str(": ");
            print_str(g_logs[i].msg);
            print_str("\n");
            found++;
        }
    }
    if (!found) print_str("  (no logs)\n");
}

static void show_configs(void)
{
    print_str("--- Configuration ---\n");
    if (g_cfg_n == 0) { print_str("  (no configs)\n"); return; }
    for (int i = 0; i < g_cfg_n; i++) {
        print_str("  ");
        print_str(g_cfgs[i].key);
        print_str("=");
        if (g_cfgs[i].encrypted) {
            print_str("********");
        } else {
            print_str(g_cfgs[i].val);
        }
        const char *ts;
        switch (g_cfgs[i].type) {
            case CFG_ENV_VAR: ts = "env";   break;
            case CFG_SECRET:  ts = "secret"; break;
            case CFG_FILE:    ts = "file";   break;
            default:          ts = "?";      break;
        }
        print_str(" [");
        print_str(ts);
        print_str("]\n");
    }
}

/* ══════════════════════ Help ══════════════════════ */

static void show_help(void)
{
    print_str("deploy_orchestrator: Deployment orchestration (v1.0)\n");
    print_str("Usage: deploy_orchestrator [OPTIONS]\n");
    print_str("  -h, --help     Show this help\n");
    print_str("  -t, --test     Run test suite\n");
    print_str("  -s, --status   Show deployment status\n");
    print_str("  -c, --configs  Show configuration\n");
    print_str("  -l, --logs     Show deployment logs\n");
    print_str("\n");
    print_str("Strategies: CI/CD, Blue-Green (zero-downtime), Rolling (batch updates)\n");
    print_str("Features: multi-env, multi-cluster, traffic routing, fast rollback\n");
}

/* ══════════════════════ Test Suite ══════════════════════ */

static void test_environments(void)
{
    print_str("\n--- Test: Environments ---\n");
    int dev = register_env("dev", ENV_DEV);
    int stg = register_env("staging", ENV_STAGING);
    int prd = register_env("prod", ENV_PROD);

    if (dev >= 0 && stg >= 0 && prd >= 0) {
        print_str("[test] register envs: OK\n");
    } else {
        print_str("[test] register envs: FAIL\n");
    }

    if (find_env("dev") == dev && find_env("prod") == prd) {
        print_str("[test] find envs: OK\n");
    } else {
        print_str("[test] find envs: FAIL\n");
    }

    /* Duplicate should return existing */
    int dup = register_env("dev", ENV_DEV);
    if (dup == dev) {
        print_str("[test] duplicate env: OK\n");
    }
}

static void test_clusters(void)
{
    print_str("\n--- Test: Clusters ---\n");
    int c1 = register_cluster("us-east-1", "us-east", 100);
    int c2 = register_cluster("eu-west-1", "eu-west", 80);
    int c3 = register_cluster("ap-south-1", "ap-south", 60);

    if (c1 >= 0 && c2 >= 0 && c3 >= 0) {
        print_str("[test] register clusters: OK\n");
    } else {
        print_str("[test] register clusters: FAIL\n");
    }

    if (find_cluster("eu-west-1") == c2) {
        print_str("[test] find cluster: OK\n");
    } else {
        print_str("[test] find cluster: FAIL\n");
    }
}

static void test_configs(void)
{
    print_str("\n--- Test: Configs ---\n");
    int c1 = set_config("DATABASE_URL", "postgres://db:5432/app", CFG_ENV_VAR, 0);
    int c2 = set_config("API_KEY", "sk-secret-key-12345", CFG_SECRET, 1);
    int c3 = set_config("nginx.conf", "/etc/nginx/nginx.conf", CFG_FILE, 0);

    if (c1 >= 0 && c2 >= 0 && c3 >= 0) {
        print_str("[test] set configs: OK\n");
    } else {
        print_str("[test] set configs: FAIL\n");
    }

    if (get_config("API_KEY") == c2) {
        print_str("[test] get config: OK\n");
    }

    /* Update existing */
    set_config("DATABASE_URL", "postgres://db:5432/app_v2", CFG_ENV_VAR, 0);
    if (get_config("DATABASE_URL") == c1) {
        print_str("[test] update config: OK\n");
    }
}

static void test_cicd_pipeline(void)
{
    print_str("\n--- Test: CI/CD Pipeline ---\n");
    int d = create_deploy("web-api", ENV_STAGING, STRAT_CICD, 42, 0);
    if (d < 0) { print_str("[test] create cicd: FAIL\n"); return; }

    advance_deploy(d);  /* PENDING -> BUILDING */
    advance_deploy(d);  /* BUILDING -> DEPLOYING */
    advance_deploy(d);  /* DEPLOYING -> HEALTH_CHK */
    advance_deploy(d);  /* HEALTH_CHK -> ACTIVE */
    run_health_check(d);

    deploy_t *p = find_deploy(d);
    if (p && p->state == ST_ACTIVE && p->health == HLTH_HEALTHY) {
        print_str("[test] cicd pipeline: OK\n");
    } else {
        print_str("[test] cicd pipeline: FAIL\n");
    }
}

static void test_blue_green(void)
{
    print_str("\n--- Test: Blue-Green Deployment ---\n");
    int d = create_deploy("frontend", ENV_PROD, STRAT_BLUE_GREEN, 15, 1);
    if (d < 0) { print_str("[test] create blue-green: FAIL\n"); return; }

    advance_deploy(d);  /* PENDING -> BUILDING */
    advance_deploy(d);  /* BUILDING -> DEPLOYING (switches slot) */
    advance_deploy(d);  /* DEPLOYING -> HEALTH_CHK */
    advance_deploy(d);  /* HEALTH_CHK -> ACTIVE */

    deploy_t *p = find_deploy(d);
    if (p && p->state == ST_ACTIVE && p->active_slot == 1) {
        print_str("[test] blue-green deploy: OK (slot=green)\n");
    } else {
        print_str("[test] blue-green deploy: FAIL\n");
    }

    /* Test rollback */
    rollback_blue_green(d);
    p = find_deploy(d);
    if (p && p->state == ST_ROLLED_BACK && p->active_slot == 0) {
        print_str("[test] blue-green rollback: OK (slot=blue)\n");
    } else {
        print_str("[test] blue-green rollback: FAIL\n");
    }
}

static void test_rolling_update(void)
{
    print_str("\n--- Test: Rolling Update ---\n");
    int d = create_deploy("worker", ENV_PROD, STRAT_ROLLING, 7, 2);
    if (d < 0) { print_str("[test] create rolling: FAIL\n"); return; }

    advance_deploy(d);  /* PENDING -> BUILDING */
    advance_deploy(d);  /* BUILDING -> DEPLOYING */

    /* Advance through batches */
    advance_deploy(d);  /* batch 1/4 */
    advance_deploy(d);  /* batch 2/4 */

    /* Pause */
    pause_rolling(d);
    deploy_t *p = find_deploy(d);
    if (p && p->state == ST_PAUSED) {
        print_str("[test] rolling pause: OK\n");
    } else {
        print_str("[test] rolling pause: FAIL\n");
    }

    /* Resume and complete */
    advance_deploy(d);  /* PAUSED -> DEPLOYING */
    advance_deploy(d);  /* batch 3/4 */
    advance_deploy(d);  /* batch 4/4 -> HEALTH_CHK */
    advance_deploy(d);  /* HEALTH_CHK -> ACTIVE */
    run_health_check(d);

    p = find_deploy(d);
    if (p && p->state == ST_ACTIVE && p->batches_done == p->batches_total) {
        print_str("[test] rolling complete: OK\n");
    } else {
        print_str("[test] rolling complete: FAIL\n");
    }
}

static void test_traffic_routing(void)
{
    print_str("\n--- Test: Traffic Routing ---\n");
    int d = create_deploy("gateway", ENV_PROD, STRAT_CICD, 3, 0);
    if (d < 0) { print_str("[test] create for traffic: FAIL\n"); return; }

    /* Advance to active */
    advance_deploy(d);
    advance_deploy(d);
    advance_deploy(d);
    advance_deploy(d);

    /* Canary: 10% */
    set_traffic(d, ROUTE_CANARY, 10);
    deploy_t *p = find_deploy(d);
    if (p && p->traffic_pct == 10) {
        print_str("[test] canary traffic: OK\n");
    }

    /* Weighted: 50% */
    set_traffic(d, ROUTE_WEIGHTED, 50);
    p = find_deploy(d);
    if (p && p->traffic_pct == 50) {
        print_str("[test] weighted traffic: OK\n");
    }

    /* Full: 100% */
    set_traffic(d, ROUTE_PRIMARY, 100);
    p = find_deploy(d);
    if (p && p->traffic_pct == 100) {
        print_str("[test] full traffic: OK\n");
    }
}

static void test_multi_cluster(void)
{
    print_str("\n--- Test: Multi-Cluster ---\n");
    int d1 = create_deploy("svc-a", ENV_PROD, STRAT_CICD, 10, 0);
    int d2 = create_deploy("svc-b", ENV_PROD, STRAT_CICD, 5, 1);
    int d3 = create_deploy("svc-c", ENV_PROD, STRAT_ROLLING, 3, 2);

    if (d1 >= 0 && d2 >= 0 && d3 >= 0) {
        print_str("[test] multi-cluster deploy: OK\n");
    } else {
        print_str("[test] multi-cluster deploy: FAIL\n");
    }

    /* Verify cluster assignments */
    deploy_t *p1 = find_deploy(d1);
    deploy_t *p2 = find_deploy(d2);
    deploy_t *p3 = find_deploy(d3);
    if (p1 && p1->cluster_id == 0 &&
        p2 && p2->cluster_id == 1 &&
        p3 && p3->cluster_id == 2) {
        print_str("[test] cluster assignment: OK\n");
    } else {
        print_str("[test] cluster assignment: FAIL\n");
    }
}

static void test_log_collection(void)
{
    print_str("\n--- Test: Log Collection ---\n");
    if (g_log_n > 0) {
        print_str("[test] logs collected: ");
        print_int(g_log_n);
        print_str(" entries: OK\n");
    } else {
        print_str("[test] logs collected: FAIL (empty)\n");
    }
}

static void run_tests(void)
{
    print_str("=== Deploy Orchestrator Test Suite ===\n");

    test_environments();
    test_clusters();
    test_configs();
    test_cicd_pipeline();
    test_blue_green();
    test_rolling_update();
    test_traffic_routing();
    test_multi_cluster();
    test_log_collection();

    print_str("\n--- Final Status ---\n");
    show_all_status();
    show_configs();
    show_logs(-1);
    print_str("\n[test] all tests complete\n");
}

/* ══════════════════════ Entry Point ══════════════════════ */

void _start(void)
{
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);

    int help   = 0;
    int test   = 0;
    int status = 0;
    int cfgs   = 0;
    int logs   = 0;

    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;
    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0)    help   = 1;
        else if (my_strcmp(arg, "-t") == 0 || my_strcmp(arg, "--test") == 0)   test   = 1;
        else if (my_strcmp(arg, "-s") == 0 || my_strcmp(arg, "--status") == 0) status = 1;
        else if (my_strcmp(arg, "-c") == 0 || my_strcmp(arg, "--configs") == 0) cfgs   = 1;
        else if (my_strcmp(arg, "-l") == 0 || my_strcmp(arg, "--logs") == 0)   logs   = 1;
        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    if (help)   { show_help(); return; }
    if (test)   { run_tests(); return; }
    if (status) { show_all_status(); return; }
    if (cfgs)   { show_configs(); return; }
    if (logs)   { show_logs(-1); return; }

    print_str("deploy_orchestrator: use -h for help, -t for test\n");
}
