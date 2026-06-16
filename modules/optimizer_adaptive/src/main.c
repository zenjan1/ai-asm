/* optimizer_adaptive: WASM Adaptive Optimization Engine (v1.0) */
/* Runtime performance monitoring, auto-tuning, workload identification, dynamic resource allocation */

#include <stddef.h>

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int, unsigned int);

/* ---------- Constants ---------- */

#define MAX_METRICS      64
#define MAX_WORKLOADS    32
#define MAX_NAME         32
#define MAX_RESOURCES    16

#define WL_CPU_BOUND     0
#define WL_IO_BOUND      1
#define WL_MEMORY_BOUND  2
#define WL_MIXED         3
#define WL_INTERACTIVE   4
#define WL_BATCH         5
#define WL_UNKNOWN       6

/* ---------- Data Structures ---------- */

typedef struct {
    int id; char name[MAX_NAME]; int sample_count; int sum; int min_val; int max_val; int last_val;
} perf_metric_t;
typedef struct {
    int id; char name[MAX_NAME]; int type; int cpu_pct; int io_pct; int mem_pct; int confidence;
} workload_t;
typedef struct {
    int jit_threshold; int cache_size_kb; int thread_count;
    int gc_interval_ms; int io_batch_size; int mem_pool_blocks;
} tune_config_t;
typedef struct {
    int id; char name[MAX_NAME]; int total_units; int used_units; int peak_used;
} resource_pool_t;

/* ---------- State ---------- */

static perf_metric_t metrics[MAX_METRICS]; static int metric_count = 0; static int next_metric_id = 1;
static workload_t workloads[MAX_WORKLOADS]; static int workload_count = 0; static int next_workload_id = 1;
static tune_config_t current_config; static int tune_cycles = 0;
static resource_pool_t pools[MAX_RESOURCES]; static int pool_count = 0; static int next_pool_id = 1;

/* ---------- Helpers ---------- */

static int my_strlen(const char *s) { int l=0; while(s[l]) l++; return l; }
static void my_strncpy(char *d, const char *s, int n) { int i=0; while(i<n-1&&s[i]){d[i]=s[i];i++;} d[i]='\0'; }
static void pstr(const char *s) { host_print(s); }
static void pint(int v) {
    char b[32]; int p=0;
    if(v<0){b[p++]='-';v=-v;} if(v==0) b[p++]='0';
    else { int d=0,t=v; while(t>0){d++;t/=10;} p+=d; b[p]='\0'; p--;
        while(v>0){b[p--]='0'+(v%10);v/=10;} }
    host_print(b);
}
static const char *workload_type_name(int t) {
    switch(t) { case WL_CPU_BOUND: return "CPU-BOUND"; case WL_IO_BOUND: return "IO-BOUND";
        case WL_MEMORY_BOUND: return "MEMORY-BOUND"; case WL_MIXED: return "MIXED";
        case WL_INTERACTIVE: return "INTERACTIVE"; case WL_BATCH: return "BATCH";
        default: return "UNKNOWN"; }
}

/* ---------- Performance Monitoring ---------- */

int perf_register(const char *name) {
    if (metric_count >= MAX_METRICS) return -1;
    int idx = metric_count++;
    metrics[idx].id = next_metric_id++;
    my_strncpy(metrics[idx].name, name, MAX_NAME - 1);
    metrics[idx].sample_count = 0; metrics[idx].sum = 0;
    metrics[idx].min_val = 2147483647; metrics[idx].max_val = -2147483647;
    metrics[idx].last_val = 0;
    pstr("  [perf] Registered metric: "); pstr(name); pstr("\n");
    return metrics[idx].id;
}
int perf_record(int metric_id, int value) {
    for (int i = 0; i < MAX_METRICS; i++) {
        if (metrics[i].id == metric_id) {
            metrics[i].sample_count++; metrics[i].sum += value;
            metrics[i].last_val = value;
            if (value < metrics[i].min_val) metrics[i].min_val = value;
            if (value > metrics[i].max_val) metrics[i].max_val = value;
            return 0;
        }
    }
    return -1;
}
int perf_get_avg(int metric_id) {
    for (int i = 0; i < MAX_METRICS; i++)
        if (metrics[i].id == metric_id && metrics[i].sample_count > 0)
            return metrics[i].sum / metrics[i].sample_count;
    return 0;
}
void perf_report(void) {
    pstr("  [perf] Performance Report:\n");
    for (int i = 0; i < metric_count; i++) {
        pstr("    "); pstr(metrics[i].name); pstr(": samples=");
        pint(metrics[i].sample_count); pstr(" avg="); pint(perf_get_avg(metrics[i].id));
        pstr(" min="); pint(metrics[i].min_val); pstr(" max="); pint(metrics[i].max_val);
        pstr(" last="); pint(metrics[i].last_val); pstr("\n");
    }
}

/* ---------- Workload Identification ---------- */

int workload_classify(const char *name, int cpu_pct, int io_pct, int mem_pct) {
    if (workload_count >= MAX_WORKLOADS) return -1;
    int idx = workload_count++;
    workloads[idx].id = next_workload_id++;
    my_strncpy(workloads[idx].name, name, MAX_NAME - 1);
    workloads[idx].cpu_pct = cpu_pct; workloads[idx].io_pct = io_pct;
    workloads[idx].mem_pct = mem_pct;
    int type = WL_UNKNOWN, confidence = 50;
    if (cpu_pct >= 70 && io_pct < 20) { type = WL_CPU_BOUND; confidence = 80 + (cpu_pct - 70) / 3; }
    else if (io_pct >= 60 && cpu_pct < 30) { type = WL_IO_BOUND; confidence = 75 + (io_pct - 60) / 3; }
    else if (mem_pct >= 70) { type = WL_MEMORY_BOUND; confidence = 70 + (mem_pct - 70) / 4; }
    else if (cpu_pct >= 40 && io_pct >= 30) { type = WL_MIXED; confidence = 60; }
    else if (cpu_pct < 20 && io_pct < 20 && mem_pct < 30) { type = WL_INTERACTIVE; confidence = 65; }
    else if (cpu_pct >= 50 && io_pct < 15 && mem_pct < 40) { type = WL_BATCH; confidence = 70; }
    if (confidence > 100) confidence = 100;
    workloads[idx].type = type; workloads[idx].confidence = confidence;
    pstr("  [workload] Classified '"); pstr(name); pstr("' as "); pstr(workload_type_name(type));
    pstr(" (cpu="); pint(cpu_pct); pstr("% io="); pint(io_pct);
    pstr("% mem="); pint(mem_pct); pstr("% conf="); pint(confidence); pstr("%)\n");
    return workloads[idx].id;
}
const char *workload_get_type(int wl_id) {
    for (int i = 0; i < MAX_WORKLOADS; i++)
        if (workloads[i].id == wl_id) return workload_type_name(workloads[i].type);
    return "UNKNOWN";
}

/* ---------- Auto-Tuning ---------- */

void tune_init(void) {
    current_config.jit_threshold = 100;
    current_config.cache_size_kb = 256;
    current_config.thread_count = 4;
    current_config.gc_interval_ms = 500;
    current_config.io_batch_size = 32;
    current_config.mem_pool_blocks = 64;
    pstr("  [tune] Default config: jit_thresh=100 cache=256KB threads=4 gc=500ms io_batch=32 pool=64\n");
}

void tune_apply_workload(int wl_id) {
    for (int i = 0; i < MAX_WORKLOADS; i++) {
        if (workloads[i].id != wl_id) continue;
        int t = workloads[i].type;
        pstr("  [tune] Auto-tuning for "); pstr(workload_type_name(t));
        pstr(" workload '"); pstr(workloads[i].name); pstr("'\n");
        switch (t) {
        case WL_CPU_BOUND:
            current_config.thread_count = 8; current_config.jit_threshold = 50;
            current_config.cache_size_kb = 512;
            pstr("    threads=8 jit_thresh=50 cache=512KB\n"); break;
        case WL_IO_BOUND:
            current_config.io_batch_size = 128; current_config.thread_count = 16;
            current_config.gc_interval_ms = 1000;
            pstr("    io_batch=128 threads=16 gc=1000ms\n"); break;
        case WL_MEMORY_BOUND:
            current_config.cache_size_kb = 1024; current_config.mem_pool_blocks = 256;
            current_config.gc_interval_ms = 250;
            pstr("    cache=1024KB pool=256 gc=250ms\n"); break;
        case WL_MIXED:
            current_config.thread_count = 6; current_config.cache_size_kb = 512;
            current_config.io_batch_size = 64;
            pstr("    threads=6 cache=512KB io_batch=64\n"); break;
        case WL_INTERACTIVE:
            current_config.jit_threshold = 20; current_config.gc_interval_ms = 100;
            current_config.cache_size_kb = 384;
            pstr("    jit_thresh=20 gc=100ms cache=384KB\n"); break;
        case WL_BATCH:
            current_config.thread_count = 12; current_config.io_batch_size = 256;
            current_config.jit_threshold = 200;
            pstr("    threads=12 io_batch=256 jit_thresh=200\n"); break;
        default:
            pstr("    (no changes for unknown workload)\n"); break;
        }
        tune_cycles++;
        return;
    }
    pstr("  [tune] Warning: workload not found\n");
}

void tune_show_config(void) {
    pstr("  [tune] Current configuration:\n");
    pstr("    jit_threshold: "); pint(current_config.jit_threshold); pstr("\n");
    pstr("    cache_size_kb: "); pint(current_config.cache_size_kb); pstr("\n");
    pstr("    thread_count:  "); pint(current_config.thread_count); pstr("\n");
    pstr("    gc_interval_ms:"); pint(current_config.gc_interval_ms); pstr("\n");
    pstr("    io_batch_size: "); pint(current_config.io_batch_size); pstr("\n");
    pstr("    mem_pool_blocks:"); pint(current_config.mem_pool_blocks); pstr("\n");
    pstr("    tune_cycles:   "); pint(tune_cycles); pstr("\n");
}

/* ---------- Dynamic Resource Allocation ---------- */

int resource_create_pool(const char *name, int units) {
    if (pool_count >= MAX_RESOURCES) return -1;
    int idx = pool_count++;
    pools[idx].id = next_pool_id++;
    my_strncpy(pools[idx].name, name, MAX_NAME - 1);
    pools[idx].total_units = units; pools[idx].used_units = 0; pools[idx].peak_used = 0;
    pstr("  [resource] Created pool '"); pstr(name); pstr("' with "); pint(units); pstr(" units\n");
    return pools[idx].id;
}
int resource_alloc(int pool_id, int units) {
    for (int i = 0; i < MAX_RESOURCES; i++) {
        if (pools[i].id != pool_id) continue;
        if (pools[i].used_units + units > pools[i].total_units) {
            pstr("  [resource] WARN: pool '"); pstr(pools[i].name);
            pstr("' insufficient (used="); pint(pools[i].used_units);
            pstr(" req="); pint(units); pstr(" total="); pint(pools[i].total_units); pstr(")\n");
            return -1;
        }
        pools[i].used_units += units;
        if (pools[i].used_units > pools[i].peak_used) pools[i].peak_used = pools[i].used_units;
        return 0;
    }
    return -1;
}
int resource_free(int pool_id, int units) {
    for (int i = 0; i < MAX_RESOURCES; i++) {
        if (pools[i].id != pool_id) continue;
        if (units > pools[i].used_units) {
            pstr("  [resource] WARN: over-free on '"); pstr(pools[i].name); pstr("'\n"); return -1;
        }
        pools[i].used_units -= units; return 0;
    }
    return -1;
}
void resource_scale(int pool_id, int new_total) {
    for (int i = 0; i < MAX_RESOURCES; i++) {
        if (pools[i].id != pool_id) continue;
        if (new_total < pools[i].used_units) {
            pstr("  [resource] WARN: cannot shrink '"); pstr(pools[i].name);
            pstr("' below used ("); pint(pools[i].used_units); pstr(")\n"); return;
        }
        int old = pools[i].total_units; pools[i].total_units = new_total;
        pstr("  [resource] Scaled '"); pstr(pools[i].name);
        pstr("': "); pint(old); pstr(" -> "); pint(new_total); pstr(" units\n"); return;
    }
    pstr("  [resource] WARN: pool not found\n");
}
void resource_report(void) {
    pstr("  [resource] Pool Report:\n");
    for (int i = 0; i < pool_count; i++) {
        int pct = pools[i].total_units > 0 ? (pools[i].used_units * 100) / pools[i].total_units : 0;
        pstr("    "); pstr(pools[i].name); pstr(": used="); pint(pools[i].used_units);
        pstr("/"); pint(pools[i].total_units); pstr(" ("); pint(pct); pstr("%) peak=");
        pint(pools[i].peak_used); pstr("\n");
    }
}

/* ---------- Entry Point ---------- */

void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);

    int help = 0, test = 0;
    unsigned int pos = 0;
    char *av = (char *)buf;
    while (pos < 512 && av[pos]) pos++;
    pos++;
    while (pos < 512 && av[pos]) {
        char *a = &av[pos];
        int l = my_strlen(a);
        if (l == 2 && a[0] == '-' && a[1] == 'h') help = 1;
        else if (l == 2 && a[0] == '-' && a[1] == 't') test = 1;
        while (pos < 512 && av[pos]) pos++;
        pos++;
    }

    pstr("optimizer_adaptive v1.0 - WASM Adaptive Optimization Engine\n");

    if (help) {
        pstr("Usage: optimizer_adaptive [-h|-t]\n");
        pstr("  -h  Show help\n");
        pstr("  -t  Run test mode\n");
        pstr("\nFeatures:\n");
        pstr("  - Runtime performance monitoring\n");
        pstr("  - Auto-tuning (JIT thresholds, cache sizes, thread counts)\n");
        pstr("  - Workload identification (CPU/IO/memory-bound, mixed, interactive, batch)\n");
        pstr("  - Dynamic resource allocation with scaling\n");
        return;
    }

    if (test) {
        pstr("=== optimizer_adaptive Test ===\n\n");

        /* --- Phase 1: Performance Monitoring --- */
        pstr("[Phase 1] Performance Monitoring\n");
        int m_lat = perf_register("request_latency_ms");
        int m_thr = perf_register("throughput_ops");
        int m_mem = perf_register("memory_usage_kb");

        perf_record(m_lat, 45); perf_record(m_lat, 52);
        perf_record(m_lat, 38); perf_record(m_lat, 61); perf_record(m_lat, 44);
        perf_record(m_thr, 1200); perf_record(m_thr, 1350);
        perf_record(m_thr, 1180); perf_record(m_thr, 1400);
        perf_record(m_mem, 2048); perf_record(m_mem, 2100);
        perf_record(m_mem, 2250); perf_record(m_mem, 2180);
        perf_report();
        pstr("\n");

        /* --- Phase 2: Workload Identification --- */
        pstr("[Phase 2] Workload Identification\n");
        int wl1 = workload_classify("api_server", 85, 10, 25);
        int wl2 = workload_classify("db_proxy", 15, 75, 30);
        int wl3 = workload_classify("cache_engine", 20, 15, 80);
        int wl4 = workload_classify("web_gateway", 45, 50, 35);
        int wl5 = workload_classify("cli_tool", 10, 8, 15);
        pstr("  api_server="); pstr(workload_get_type(wl1));
        pstr(" db_proxy="); pstr(workload_get_type(wl2));
        pstr(" cache_engine="); pstr(workload_get_type(wl3));
        pstr(" web_gateway="); pstr(workload_get_type(wl4));
        pstr(" cli_tool="); pstr(workload_get_type(wl5));
        pstr("\n\n");

        /* --- Phase 3: Auto-Tuning --- */
        pstr("[Phase 3] Auto-Tuning\n");
        tune_init();
        pstr("  Applying api_server tuning...\n");
        tune_apply_workload(wl1); tune_show_config();
        pstr("  Applying db_proxy tuning...\n");
        tune_apply_workload(wl2); tune_show_config();
        pstr("  Applying cache_engine tuning...\n");
        tune_apply_workload(wl3); tune_show_config();
        pstr("\n");

        /* --- Phase 4: Dynamic Resource Allocation --- */
        pstr("[Phase 4] Dynamic Resource Allocation\n");
        int p_thr = resource_create_pool("thread_pool", 16);
        int p_mem = resource_create_pool("memory_mb", 512);
        int p_con = resource_create_pool("connections", 100);

        resource_alloc(p_thr, 8); resource_alloc(p_mem, 256);
        resource_alloc(p_con, 50); resource_alloc(p_thr, 4);
        resource_alloc(p_mem, 128);
        pstr("  After initial alloc:\n"); resource_report();

        pstr("  Scaling thread_pool 16->32...\n");
        resource_scale(p_thr, 32); resource_alloc(p_thr, 6);
        resource_report();

        resource_free(p_thr, 4); resource_free(p_mem, 64);
        resource_free(p_con, 20);
        pstr("  After free:\n"); resource_report();

        /* Edge cases */
        pstr("[Edge Cases]\n");
        resource_alloc(p_con, 999);
        resource_scale(p_con, 1);

        /* Summary */
        pstr("\n[Summary]\n");
        pstr("  Metrics: "); pint(metric_count);
        pstr(" Workloads: "); pint(workload_count);
        pstr(" Tune cycles: "); pint(tune_cycles);
        pstr(" Pools: "); pint(pool_count);
        pstr("\n\n=== Test Complete ===\n");
        return;
    }

    pstr("Use -h for help, -t for test\n");
}
