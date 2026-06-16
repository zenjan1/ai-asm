/* edge_ai_runtime: Edge AI Inference Runtime (v56.0)
 * Optimized inference engine for AArch64 WebAssembly-native OS
 * Features: quantization, operator fusion, heterogeneous dispatch,
 *           dynamic batching, model hot-update, A/B testing
 */

#include <stddef.h>

/* ── Host imports ────────────────────────────────────────────────── */
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* ── Constants ───────────────────────────────────────────────────── */
#define MAX_DIMS          4
#define MAX_NODES         32
#define MAX_EDGES         64
#define MAX_DEVICES        8
#define MAX_CAPS          16
#define MAX_QUEUE         64
#define MAX_VERSIONS       8
#define MAX_METRICS      256
#define MAX_SAMPLES       64
#define MAX_PATH          64
#define MAX_NAME          32
#define MAX_OPS            8

/* Quantization modes */
#define QUANT_INT8_SYM     0
#define QUANT_INT8_ASYM    1
#define QUANT_FP16         2

/* Calibration methods */
#define CALIB_MIN_MAX      0
#define CALIB_PERCENTILE   1
#define CALIB_KL_DIV       2

/* Data types */
#define DTYPE_FP32         0
#define DTYPE_FP16         1
#define DTYPE_INT8         2

/* Operator types */
#define OP_CONV2D          0
#define OP_BATCHNORM       1
#define OP_RELU            2
#define OP_MATMUL          3
#define OP_ADD             4
#define OP_POOLING         5
#define OP_SOFTMAX         6
#define OP_FUSED           7

/* Device types */
#define DEV_CPU            0
#define DEV_GPU            1
#define DEV_NPU            2

/* Scheduler modes */
#define SCHED_ROUND_ROBIN  0
#define SCHED_LOAD_BALANCE 1
#define SCHED_PREFERENCE   2

/* Model status */
#define MSTAT_LOADING      0
#define MSTAT_ACTIVE       1
#define MSTAT_STANDBY      2
#define MSTAT_ERROR        3

/* A/B split modes */
#define SPLIT_PERCENTAGE   0
#define SPLIT_USER_HASH    1

/* ── Struct definitions ──────────────────────────────────────────── */

typedef struct {
    int mode;               /* QUANT_INT8_SYM / ASYM / FP16 */
    int calibration_method; /* CALIB_MIN_MAX / PERCENTILE / KL_DIV */
} quant_config_t;

typedef struct {
    int shape[MAX_DIMS];
    int data_type;          /* DTYPE_FP32 / FP16 / INT8 */
    float *data;
    float scale;
    int zero_point;
    int num_elements;
} tensor_t;

typedef struct {
    int type;               /* OP_* */
    int inputs[MAX_OPS];
    int outputs[MAX_OPS];
    int num_inputs;
    int num_outputs;
    float params[8];
    int fused;
    char name[MAX_NAME];
} op_node_t;

typedef struct {
    int from;
    int to;
} edge_t;

typedef struct {
    op_node_t nodes[MAX_NODES];
    edge_t edges[MAX_EDGES];
    int num_nodes;
    int num_edges;
} fusion_graph_t;

typedef struct {
    int id;
    int type;               /* DEV_CPU / GPU / NPU */
    int capabilities[MAX_CAPS];
    int num_caps;
    int memory_mb;
    int utilization_pct;
    int total_ops;
    int total_latency_ms;
    int total_power_mw;
} device_t;

typedef struct {
    device_t devices[MAX_DEVICES];
    int num_devices;
    int scheduler_mode;
    int rr_index;
} dispatch_context_t;

typedef struct {
    int max_batch_size;
    int timeout_ms;
    int queue_capacity;
} batch_config_t;

typedef struct {
    int request_id;
    int model_id;
    float *input;
    int input_size;
    int enqueue_time_ms;
    int status;             /* 0=waiting 1=batched 2=done */
} inference_request_t;

typedef struct {
    inference_request_t requests[MAX_QUEUE];
    int count;
    int capacity;
    int head;
    int tail;
    batch_config_t config;
    int total_served;
    int total_wait_ms;
} request_queue_t;

typedef struct {
    int major;
    int minor;
    int patch;
    char path[MAX_PATH];
    int load_time_ms;
    int status;             /* MSTAT_* */
} model_version_t;

typedef struct {
    model_version_t models[MAX_VERSIONS];
    int version_count;
    int active_index;
    int previous_index;
} model_registry_t;

typedef struct {
    int variant;            /* 0=A 1=B */
    int latency_ms;
    float accuracy;
    int success;
} ab_metric_t;

typedef struct {
    int split_mode;
    int traffic_split_a_pct;
    int metrics_window_ms;
} ab_config_t;

typedef struct {
    char name[MAX_NAME];
    int model_a;
    int model_b;
    ab_config_t config;
    ab_metric_t metrics_a[MAX_METRICS];
    ab_metric_t metrics_b[MAX_METRICS];
    int count_a;
    int count_b;
} ab_test_t;

/* ── Helper functions ────────────────────────────────────────────── */

static int my_strlen(const char *s) {
    int len = 0;
    while (s[len]) len++;
    return len;
}

static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0;
    while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; }
    dst[i] = '\0';
}

static void print_str(const char *s) { host_print(s); }

static void print_int(int val) {
    char buf[32];
    int pos = 0;
    if (val < 0) { buf[pos++] = '-'; val = -val; }
    if (val == 0) { buf[pos++] = '0'; }
    else {
        int d = 0, t = val;
        while (t > 0) { d++; t /= 10; }
        pos += d;
        buf[pos] = '\0';
        pos--;
        while (val > 0) { buf[pos--] = '0' + (val % 10); val /= 10; }
    }
    host_print(buf);
}

static void print_double(double val) {
    int whole = (int)val;
    double frac_part = val - (double)whole;
    if (frac_part < 0) frac_part = -frac_part;
    int frac = (int)(frac_part * 10000.0);
    print_int(whole);
    print_str(".");
    /* Print fractional part with leading zeros */
    char fbuf[8];
    int fp = 4;
    fbuf[fp] = '\0';
    if (frac == 0) { print_str("0000"); return; }
    while (frac > 0 && fp > 0) { fbuf[--fp] = '0' + (frac % 10); frac /= 10; }
    while (fp > 0) { fbuf[--fp] = '0'; }
    print_str(fbuf);
}

static double my_abs(double x) { return x < 0.0 ? -x : x; }

static double my_sqrt(double x) {
    if (x <= 0.0) return 0.0;
    double guess = x * 0.5;
    for (int i = 0; i < 40; i++) {
        guess = (guess + x / guess) * 0.5;
    }
    return guess;
}

/* ── 1. Model Quantization ───────────────────────────────────────── */

static void quant_calibrate(tensor_t *tensor, float *samples, int num_samples,
                            int method) {
    if (num_samples <= 0) return;
    float min_val = samples[0];
    float max_val = samples[0];
    for (int i = 1; i < num_samples; i++) {
        if (samples[i] < min_val) min_val = samples[i];
        if (samples[i] > max_val) max_val = samples[i];
    }
    /* Percentile: trim 1% from each side (approximation) */
    if (method == CALIB_PERCENTILE && num_samples > 4) {
        /* Simple bubble-sort partial for min/max trim */
        float range = max_val - min_val;
        min_val = min_val + range * 0.01f;
        max_val = max_val - range * 0.01f;
    }
    /* KL divergence: adjust range for minimal divergence */
    if (method == CALIB_KL_DIV && num_samples > 2) {
        float range = max_val - min_val;
        min_val = min_val + range * 0.005f;
        max_val = max_val - range * 0.005f;
    }
    float scale = (max_val - min_val) / 255.0f;
    if (scale < 0.000001f) scale = 0.000001f;
    int zero_point = (int)(-min_val / scale);
    if (zero_point < -128) zero_point = -128;
    if (zero_point > 127) zero_point = 127;
    tensor->scale = scale;
    tensor->zero_point = zero_point;
}

static void quant_apply(tensor_t *tensor, quant_config_t *config) {
    if (!tensor || !tensor->data) return;
    int n = tensor->num_elements;
    if (config->mode == QUANT_INT8_SYM || config->mode == QUANT_INT8_ASYM) {
        tensor->data_type = DTYPE_INT8;
        for (int i = 0; i < n; i++) {
            int q = (int)(tensor->data[i] / tensor->scale) + tensor->zero_point;
            if (q < -128) q = -128;
            if (q > 127) q = 127;
            tensor->data[i] = (float)q;
        }
    } else if (config->mode == QUANT_FP16) {
        tensor->data_type = DTYPE_FP16;
        /* Simulate FP16 rounding by truncating mantissa precision */
        for (int i = 0; i < n; i++) {
            float v = tensor->data[i];
            int bits;
            __builtin_memcpy(&bits, &v, 4);
            bits = bits & 0xFFFF0000; /* zero lower 16 bits */
            __builtin_memcpy(&v, &bits, 4);
            tensor->data[i] = v;
        }
    }
}

static void quant_dequantize(tensor_t *tensor) {
    if (!tensor || !tensor->data) return;
    int n = tensor->num_elements;
    for (int i = 0; i < n; i++) {
        tensor->data[i] = (tensor->data[i] - (float)tensor->zero_point) * tensor->scale;
    }
    tensor->data_type = DTYPE_FP32;
}

static void quant_compute_error(tensor_t *original, tensor_t *quantized) {
    if (!original || !quantized) return;
    int n = original->num_elements;
    if (n <= 0) return;
    double mse_sum = 0.0;
    double sig_sum = 0.0;
    for (int i = 0; i < n; i++) {
        double diff = (double)original->data[i] - (double)quantized->data[i];
        mse_sum += diff * diff;
        sig_sum += (double)original->data[i] * (double)original->data[i];
    }
    double mse = mse_sum / (double)n;
    double snr = 0.0;
    if (mse > 0.0000001) snr = 10.0 * (my_sqrt(sig_sum / mse));
    print_str("  Quantization error: MSE=");
    print_double(mse);
    print_str(" SNR=");
    print_double(snr);
    print_str("\n");
}

/* ── 2. Operator Fusion ─────────────────────────────────────────── */

static fusion_graph_t *fusion_create_graph(void) {
    unsigned int sz = sizeof(fusion_graph_t);
    unsigned int mem = host_alloc(sz, 8);
    fusion_graph_t *g = (fusion_graph_t *)mem;
    g->num_nodes = 0;
    g->num_edges = 0;
    return g;
}

static int fusion_add_node(fusion_graph_t *g, int type, const char *name) {
    if (g->num_nodes >= MAX_NODES) return -1;
    int idx = g->num_nodes;
    g->nodes[idx].type = type;
    g->nodes[idx].fused = 0;
    g->nodes[idx].num_inputs = 0;
    g->nodes[idx].num_outputs = 0;
    for (int i = 0; i < 8; i++) g->nodes[idx].params[i] = 0.0f;
    my_strncpy(g->nodes[idx].name, name, MAX_NAME - 1);
    g->num_nodes++;
    return idx;
}

static void fusion_add_edge(fusion_graph_t *g, int from, int to) {
    if (g->num_edges >= MAX_EDGES) return;
    g->edges[g->num_edges].from = from;
    g->edges[g->num_edges].to = to;
    g->num_edges++;
}

static int fusion_detect_patterns(fusion_graph_t *g) {
    int patterns_found = 0;
    for (int i = 0; i < g->num_edges; i++) {
        int a = g->edges[i].from;
        int b = g->edges[i].to;
        /* Pattern 1: Conv2D -> BatchNorm */
        if (g->nodes[a].type == OP_CONV2D && g->nodes[b].type == OP_BATCHNORM) {
            /* Check for ReLU after BN */
            for (int j = 0; j < g->num_edges; j++) {
                if (g->edges[j].from == b && g->nodes[g->edges[j].to].type == OP_RELU) {
                    g->nodes[a].fused = 1;
                    g->nodes[b].fused = 1;
                    g->nodes[g->edges[j].to].fused = 1;
                    print_str("  Pattern: Conv2D+BatchNorm+ReLU -> fused\n");
                    patterns_found++;
                }
            }
        }
        /* Pattern 2: MatMul -> Add */
        if (g->nodes[a].type == OP_MATMUL && g->nodes[b].type == OP_ADD) {
            g->nodes[a].fused = 1;
            g->nodes[b].fused = 1;
            print_str("  Pattern: MatMul+Add -> fused\n");
            patterns_found++;
        }
    }
    return patterns_found;
}

static int fusion_apply(fusion_graph_t *g) {
    int fused_count = 0;
    int write = 0;
    for (int i = 0; i < g->num_nodes; i++) {
        if (g->nodes[i].fused) {
            fused_count++;
        } else {
            if (write != i) g->nodes[write] = g->nodes[i];
            write++;
        }
    }
    /* Insert fused representative nodes */
    for (int i = 0; i < g->num_nodes; i++) {
        if (g->nodes[i].fused) {
            int idx = write++;
            g->nodes[idx].type = OP_FUSED;
            g->nodes[idx].fused = 0;
            my_strncpy(g->nodes[idx].name, "fused_op", MAX_NAME - 1);
            break; /* one fused representative */
        }
    }
    g->num_nodes = write;
    return fused_count;
}

static void fusion_optimize(fusion_graph_t *g) {
    print_str("  Detecting fusible patterns...\n");
    int pats = fusion_detect_patterns(g);
    print_str("  Patterns found: ");
    print_int(pats);
    print_str("\n");
    if (pats > 0) {
        int fused = fusion_apply(g);
        print_str("  Fused nodes: ");
        print_int(fused);
        print_str(", remaining: ");
        print_int(g->num_nodes);
        print_str("\n");
    }
}

/* ── 3. Heterogeneous Dispatch ───────────────────────────────────── */

static dispatch_context_t *dispatch_init(void) {
    unsigned int mem = host_alloc(sizeof(dispatch_context_t), 8);
    dispatch_context_t *ctx = (dispatch_context_t *)mem;
    ctx->num_devices = 0;
    ctx->scheduler_mode = SCHED_LOAD_BALANCE;
    ctx->rr_index = 0;

    /* Register simulated devices */
    /* CPU */
    ctx->devices[0].id = 0;
    ctx->devices[0].type = DEV_CPU;
    ctx->devices[0].memory_mb = 8192;
    ctx->devices[0].utilization_pct = 35;
    ctx->devices[0].num_caps = 2;
    ctx->devices[0].capabilities[0] = OP_CONV2D;
    ctx->devices[0].capabilities[1] = OP_MATMUL;
    ctx->devices[0].total_ops = 0;
    ctx->devices[0].total_latency_ms = 0;
    ctx->devices[0].total_power_mw = 0;

    /* GPU */
    ctx->devices[1].id = 1;
    ctx->devices[1].type = DEV_GPU;
    ctx->devices[1].memory_mb = 4096;
    ctx->devices[1].utilization_pct = 60;
    ctx->devices[1].num_caps = 4;
    ctx->devices[1].capabilities[0] = OP_CONV2D;
    ctx->devices[1].capabilities[1] = OP_MATMUL;
    ctx->devices[1].capabilities[2] = OP_POOLING;
    ctx->devices[1].capabilities[3] = OP_SOFTMAX;
    ctx->devices[1].total_ops = 0;
    ctx->devices[1].total_latency_ms = 0;
    ctx->devices[1].total_power_mw = 0;

    /* NPU */
    ctx->devices[2].id = 2;
    ctx->devices[2].type = DEV_NPU;
    ctx->devices[2].memory_mb = 2048;
    ctx->devices[2].utilization_pct = 20;
    ctx->devices[2].num_caps = 3;
    ctx->devices[2].capabilities[0] = OP_CONV2D;
    ctx->devices[2].capabilities[1] = OP_BATCHNORM;
    ctx->devices[2].capabilities[2] = OP_RELU;
    ctx->devices[2].total_ops = 0;
    ctx->devices[2].total_latency_ms = 0;
    ctx->devices[2].total_power_mw = 0;

    ctx->num_devices = 3;
    return ctx;
}

static dispatch_context_t *dispatch_create_context(void) {
    return dispatch_init();
}

static const char *device_type_name(int type) {
    if (type == DEV_CPU) return "CPU";
    if (type == DEV_GPU) return "GPU";
    if (type == DEV_NPU) return "NPU";
    return "???";
}

static int dispatch_select_device(dispatch_context_t *ctx, int op_type) {
    if (ctx->num_devices == 0) return -1;

    if (ctx->scheduler_mode == SCHED_ROUND_ROBIN) {
        int idx = ctx->rr_index;
        ctx->rr_index = (ctx->rr_index + 1) % ctx->num_devices;
        return idx;
    }

    /* LOAD_BALANCE / PREFERENCE: pick device with lowest utilization that supports op */
    int best = -1;
    int best_util = 101;
    for (int i = 0; i < ctx->num_devices; i++) {
        if (ctx->devices[i].utilization_pct >= 95) continue;
        int supports = 0;
        for (int c = 0; c < ctx->devices[i].num_caps; c++) {
            if (ctx->devices[i].capabilities[c] == op_type) { supports = 1; break; }
        }
        if (supports && ctx->devices[i].utilization_pct < best_util) {
            best = i;
            best_util = ctx->devices[i].utilization_pct;
        }
    }
    if (best < 0) best = 0; /* fallback to device 0 */
    return best;
}

static int dispatch_execute(dispatch_context_t *ctx, int node_idx, int dev_idx) {
    if (dev_idx < 0 || dev_idx >= ctx->num_devices) return -1;
    device_t *dev = &ctx->devices[dev_idx];
    /* Simulate latency based on device type and utilization */
    int base_latency = 10;
    if (dev->type == DEV_GPU) base_latency = 5;
    if (dev->type == DEV_NPU) base_latency = 3;
    int latency = base_latency + (dev->utilization_pct / 10);
    int power = 500;
    if (dev->type == DEV_GPU) power = 2000;
    if (dev->type == DEV_NPU) power = 300;
    dev->total_ops++;
    dev->total_latency_ms += latency;
    dev->total_power_mw += power;
    return latency;
}

static void dispatch_update_stats(dispatch_context_t *ctx, int dev_idx,
                                  int latency_ms, int power_mw) {
    if (dev_idx < 0 || dev_idx >= ctx->num_devices) return;
    ctx->devices[dev_idx].total_latency_ms += latency_ms;
    ctx->devices[dev_idx].total_power_mw += power_mw;
    ctx->devices[dev_idx].total_ops++;
}

/* ── 4. Dynamic Batching ────────────────────────────────────────── */

static request_queue_t *batch_queue_init(batch_config_t *config) {
    unsigned int mem = host_alloc(sizeof(request_queue_t), 8);
    request_queue_t *q = (request_queue_t *)mem;
    q->count = 0;
    q->capacity = config->queue_capacity;
    q->head = 0;
    q->tail = 0;
    q->total_served = 0;
    q->total_wait_ms = 0;
    q->config.max_batch_size = config->max_batch_size;
    q->config.timeout_ms = config->timeout_ms;
    q->config.queue_capacity = config->queue_capacity;
    return q;
}

static int batch_enqueue(request_queue_t *q, inference_request_t *req) {
    if (q->count >= q->capacity) return -1;
    q->requests[q->tail] = *req;
    q->requests[q->tail].status = 0;
    q->tail = (q->tail + 1) % q->capacity;
    q->count++;
    return 0;
}

static int batch_dequeue_batch(request_queue_t *q, int *batch_indices, int max_batch) {
    int collected = 0;
    int limit = q->count;
    if (limit > max_batch) limit = max_batch;
    int idx = q->head;
    for (int i = 0; i < limit && collected < q->config.max_batch_size; i++) {
        if (q->requests[idx].status == 0) {
            batch_indices[collected] = idx;
            q->requests[idx].status = 1;
            collected++;
        }
        idx = (idx + 1) % q->capacity;
    }
    return collected;
}

static int batch_process_batch(request_queue_t *q, int *batch_indices, int batch_size) {
    if (batch_size <= 0) return 0;
    /* Simulate batched inference: latency scales sub-linearly */
    int simulated_latency = 8 + batch_size * 2;
    for (int i = 0; i < batch_size; i++) {
        int idx = batch_indices[i];
        q->requests[idx].status = 2; /* done */
        int wait = simulated_latency;
        q->total_wait_ms += wait;
        q->total_served++;
    }
    /* Advance head past completed requests */
    while (q->count > 0 && q->requests[q->head].status == 2) {
        q->head = (q->head + 1) % q->capacity;
        q->count--;
    }
    return simulated_latency;
}

static void batch_compute_stats(request_queue_t *q) {
    print_str("  Queue stats: pending=");
    print_int(q->count);
    print_str(" served=");
    print_int(q->total_served);
    if (q->total_served > 0) {
        int avg_wait = q->total_wait_ms / q->total_served;
        print_str(" avg_wait=");
        print_int(avg_wait);
        print_str("ms throughput=");
        double tput = (double)q->total_served * 1000.0 /
                      (double)(q->total_wait_ms > 0 ? q->total_wait_ms : 1);
        print_double(tput);
        print_str(" req/s\n");
    } else {
        print_str("\n");
    }
}

/* ── 5. Model Hot Update ────────────────────────────────────────── */

static model_registry_t *registry_init(void) {
    unsigned int mem = host_alloc(sizeof(model_registry_t), 8);
    model_registry_t *reg = (model_registry_t *)mem;
    reg->version_count = 0;
    reg->active_index = -1;
    reg->previous_index = -1;
    return reg;
}

static int registry_load_version(model_registry_t *reg, int major, int minor,
                                 int patch, const char *path) {
    if (reg->version_count >= MAX_VERSIONS) return -1;
    int idx = reg->version_count;
    reg->models[idx].major = major;
    reg->models[idx].minor = minor;
    reg->models[idx].patch = patch;
    my_strncpy(reg->models[idx].path, path, MAX_PATH - 1);
    reg->models[idx].load_time_ms = 120 + idx * 15; /* simulated */
    reg->models[idx].status = MSTAT_STANDBY;
    reg->version_count++;
    print_str("  Loaded model v");
    print_int(major); print_str("."); print_int(minor); print_str("."); print_int(patch);
    print_str(" ("); print_int(reg->models[idx].load_time_ms);
    print_str("ms) from "); print_str(path); print_str("\n");
    return idx;
}

static int registry_activate_version(model_registry_t *reg, int version_idx) {
    if (version_idx < 0 || version_idx >= reg->version_count) return -1;
    int prev = reg->active_index;
    /* Mark old active as standby */
    if (prev >= 0) reg->models[prev].status = MSTAT_STANDBY;
    reg->models[version_idx].status = MSTAT_ACTIVE;
    reg->previous_index = prev;
    reg->active_index = version_idx;
    print_str("  Activated v");
    print_int(reg->models[version_idx].major); print_str(".");
    print_int(reg->models[version_idx].minor); print_str(".");
    print_int(reg->models[version_idx].patch);
    print_str("\n");
    return 0;
}

static int registry_rollback(model_registry_t *reg) {
    if (reg->previous_index < 0) {
        print_str("  No previous version to rollback to\n");
        return -1;
    }
    print_str("  Rolling back to previous version...\n");
    return registry_activate_version(reg, reg->previous_index);
}

static void registry_list_versions(model_registry_t *reg) {
    for (int i = 0; i < reg->version_count; i++) {
        print_str("  ["); print_int(i); print_str("] v");
        print_int(reg->models[i].major); print_str(".");
        print_int(reg->models[i].minor); print_str(".");
        print_int(reg->models[i].patch);
        print_str(" status=");
        if (reg->models[i].status == MSTAT_ACTIVE) print_str("ACTIVE");
        else if (reg->models[i].status == MSTAT_STANDBY) print_str("STANDBY");
        else if (reg->models[i].status == MSTAT_LOADING) print_str("LOADING");
        else print_str("ERROR");
        print_str("\n");
    }
}

/* ── 6. A/B Testing ─────────────────────────────────────────────── */

static ab_test_t *ab_create(const char *name, int model_a, int model_b,
                            ab_config_t *config) {
    unsigned int mem = host_alloc(sizeof(ab_test_t), 8);
    ab_test_t *t = (ab_test_t *)mem;
    my_strncpy(t->name, name, MAX_NAME - 1);
    t->model_a = model_a;
    t->model_b = model_b;
    t->config.split_mode = config->split_mode;
    t->config.traffic_split_a_pct = config->traffic_split_a_pct;
    t->config.metrics_window_ms = config->metrics_window_ms;
    t->count_a = 0;
    t->count_b = 0;
    return t;
}

static int ab_assign(ab_test_t *t, int user_id) {
    if (t->config.split_mode == SPLIT_PERCENTAGE) {
        /* Simple hash: user_id mod 100 < threshold -> A */
        int bucket = user_id % 100;
        return (bucket < t->config.traffic_split_a_pct) ? 0 : 1;
    }
    /* USER_HASH: hash user_id */
    int hash = ((user_id * 2654435761u) >> 16) & 0x7FFFFFFF;
    int bucket = hash % 100;
    return (bucket < t->config.traffic_split_a_pct) ? 0 : 1;
}

static void ab_record_metric(ab_test_t *t, int variant, int latency_ms,
                             float accuracy, int success) {
    ab_metric_t m;
    m.variant = variant;
    m.latency_ms = latency_ms;
    m.accuracy = accuracy;
    m.success = success;
    if (variant == 0 && t->count_a < MAX_METRICS) {
        t->metrics_a[t->count_a++] = m;
    } else if (variant == 1 && t->count_b < MAX_METRICS) {
        t->metrics_b[t->count_b++] = m;
    }
}

static void ab_compute_significance(ab_test_t *t) {
    /* Compute mean accuracy for each variant */
    double mean_a = 0.0, mean_b = 0.0;
    if (t->count_a > 0) {
        for (int i = 0; i < t->count_a; i++) mean_a += (double)t->metrics_a[i].accuracy;
        mean_a /= (double)t->count_a;
    }
    if (t->count_b > 0) {
        for (int i = 0; i < t->count_b; i++) mean_b += (double)t->metrics_b[i].accuracy;
        mean_b /= (double)t->count_b;
    }
    /* Compute variance for t-test approximation */
    double var_a = 0.0, var_b = 0.0;
    if (t->count_a > 1) {
        for (int i = 0; i < t->count_a; i++) {
            double d = (double)t->metrics_a[i].accuracy - mean_a;
            var_a += d * d;
        }
        var_a /= (double)(t->count_a - 1);
    }
    if (t->count_b > 1) {
        for (int i = 0; i < t->count_b; i++) {
            double d = (double)t->metrics_b[i].accuracy - mean_b;
            var_b += d * d;
        }
        var_b /= (double)(t->count_b - 1);
    }
    /* t-statistic: (mean_a - mean_b) / sqrt(var_a/n_a + var_b/n_b) */
    double denom = 0.0;
    if (t->count_a > 0) denom += var_a / (double)t->count_a;
    if (t->count_b > 0) denom += var_b / (double)t->count_b;
    double t_stat = 0.0;
    if (denom > 0.0000001) t_stat = (mean_a - mean_b) / my_sqrt(denom);
    /* Approximate p-value: |t| > 1.96 => significant at 95% */
    int significant = (my_abs(t_stat) > 1.96) ? 1 : 0;
    print_str("  Mean accuracy A=");
    print_double(mean_a);
    print_str(" B=");
    print_double(mean_b);
    print_str("\n  t-statistic=");
    print_double(t_stat);
    print_str(significant ? " (significant)\n" : " (not significant)\n");
}

static void ab_report(ab_test_t *t) {
    print_str("  A/B Test Report: ");
    print_str(t->name);
    print_str("\n");
    print_str("    Model A samples: ");
    print_int(t->count_a);
    print_str("  Model B samples: ");
    print_int(t->count_b);
    print_str("\n");
    /* Compute avg latency per variant */
    int lat_a = 0, lat_b = 0;
    int succ_a = 0, succ_b = 0;
    if (t->count_a > 0) {
        for (int i = 0; i < t->count_a; i++) {
            lat_a += t->metrics_a[i].latency_ms;
            succ_a += t->metrics_a[i].success;
        }
        lat_a /= t->count_a;
    }
    if (t->count_b > 0) {
        for (int i = 0; i < t->count_b; i++) {
            lat_b += t->metrics_b[i].latency_ms;
            succ_b += t->metrics_b[i].success;
        }
        lat_b /= t->count_b;
    }
    print_str("    Avg latency A="); print_int(lat_a);
    print_str("ms  B="); print_int(lat_b); print_str("ms\n");
    print_str("    Success rate A="); print_int(succ_a);
    print_str("/"); print_int(t->count_a);
    print_str("  B="); print_int(succ_b);
    print_str("/"); print_int(t->count_b); print_str("\n");
    ab_compute_significance(t);
    /* Recommend winner */
    double acc_a = 0.0, acc_b = 0.0;
    if (t->count_a > 0) {
        for (int i = 0; i < t->count_a; i++) acc_a += (double)t->metrics_a[i].accuracy;
        acc_a /= (double)t->count_a;
    }
    if (t->count_b > 0) {
        for (int i = 0; i < t->count_b; i++) acc_b += (double)t->metrics_b[i].accuracy;
        acc_b /= (double)t->count_b;
    }
    if (acc_a > acc_b) print_str("    Recommendation: Deploy Model A\n");
    else if (acc_b > acc_a) print_str("    Recommendation: Deploy Model B\n");
    else print_str("    Recommendation: Inconclusive, continue testing\n");
}

/* ── 7. Entry point ─────────────────────────────────────────────── */

static void test_quantization(void) {
    print_str("\n--- Quantization ---\n");
    unsigned int mem = host_alloc(16 * sizeof(float), 16);
    tensor_t orig;
    orig.shape[0] = 2; orig.shape[1] = 2; orig.shape[2] = 2; orig.shape[3] = 2;
    orig.data_type = DTYPE_FP32;
    orig.data = (float *)mem;
    orig.num_elements = 16;
    orig.scale = 0.0f;
    orig.zero_point = 0;

    /* Fill with sample values */
    for (int i = 0; i < 16; i++) orig.data[i] = (float)(i * 10 - 80) * 0.01f;

    float samples[8];
    for (int i = 0; i < 8; i++) samples[i] = (float)(i * 10 - 40) * 0.01f;

    quant_config_t cfg;
    cfg.mode = QUANT_INT8_SYM;
    cfg.calibration_method = CALIB_MIN_MAX;

    quant_calibrate(&orig, samples, 8, CALIB_MIN_MAX);
    print_str("  Scale="); print_double((double)orig.scale);
    print_str(" ZP="); print_int(orig.zero_point); print_str("\n");

    /* Save original values for error comparison */
    unsigned int mem2 = host_alloc(16 * sizeof(float), 16);
    tensor_t backup;
    backup.data = (float *)mem2;
    backup.num_elements = 16;
    for (int i = 0; i < 16; i++) backup.data[i] = orig.data[i];

    quant_apply(&orig, &cfg);
    print_str("  Quantized to INT8\n");
    quant_dequantize(&orig);
    print_str("  Dequantized back to FP32\n");
    quant_compute_error(&backup, &orig);
}

static void test_fusion(void) {
    print_str("\n--- Operator Fusion ---\n");
    fusion_graph_t *g = fusion_create_graph();
    int n0 = fusion_add_node(g, OP_CONV2D, "conv1");
    int n1 = fusion_add_node(g, OP_BATCHNORM, "bn1");
    int n2 = fusion_add_node(g, OP_RELU, "relu1");
    int n3 = fusion_add_node(g, OP_MATMUL, "matmul1");
    int n4 = fusion_add_node(g, OP_ADD, "bias_add");
    int n5 = fusion_add_node(g, OP_SOFTMAX, "softmax");
    fusion_add_edge(g, n0, n1);
    fusion_add_edge(g, n1, n2);
    fusion_add_edge(g, n2, n3);
    fusion_add_edge(g, n3, n4);
    fusion_add_edge(g, n4, n5);
    print_str("  Graph: "); print_int(g->num_nodes); print_str(" nodes, ");
    print_int(g->num_edges); print_str(" edges\n");
    fusion_optimize(g);
}

static void test_dispatch(void) {
    print_str("\n--- Heterogeneous Dispatch ---\n");
    dispatch_context_t *ctx = dispatch_create_context();
    print_str("  Devices: "); print_int(ctx->num_devices); print_str("\n");
    for (int i = 0; i < ctx->num_devices; i++) {
        print_str("    ["); print_int(i); print_str("] ");
        print_str(device_type_name(ctx->devices[i].type));
        print_str(" mem="); print_int(ctx->devices[i].memory_mb);
        print_str("MB util="); print_int(ctx->devices[i].utilization_pct);
        print_str("%\n");
    }
    /* Dispatch various ops */
    int ops[] = { OP_CONV2D, OP_MATMUL, OP_RELU, OP_SOFTMAX };
    for (int i = 0; i < 4; i++) {
        int dev = dispatch_select_device(ctx, ops[i]);
        int lat = dispatch_execute(ctx, i, dev);
        print_str("  Op "); print_int(ops[i]);
        print_str(" -> device "); print_int(dev);
        print_str(" ("); print_str(device_type_name(ctx->devices[dev].type));
        print_str(") latency="); print_int(lat); print_str("ms\n");
    }
    dispatch_update_stats(ctx, 0, 15, 800);
}

static void test_batching(void) {
    print_str("\n--- Dynamic Batching ---\n");
    batch_config_t bcfg;
    bcfg.max_batch_size = 4;
    bcfg.timeout_ms = 50;
    bcfg.queue_capacity = MAX_QUEUE;
    request_queue_t *q = batch_queue_init(&bcfg);

    /* Enqueue several requests */
    for (int i = 0; i < 7; i++) {
        inference_request_t req;
        req.request_id = 1000 + i;
        req.model_id = 1;
        req.input = NULL;
        req.input_size = 224;
        req.enqueue_time_ms = i * 5;
        req.status = 0;
        batch_enqueue(q, &req);
    }
    print_str("  Enqueued 7 requests\n");

    /* Form and process batches */
    int batch_idx[MAX_QUEUE];
    int bs = batch_dequeue_batch(q, batch_idx, 4);
    print_str("  Batch 1 size: "); print_int(bs); print_str("\n");
    int lat = batch_process_batch(q, batch_idx, bs);
    print_str("  Batch 1 latency: "); print_int(lat); print_str("ms\n");

    bs = batch_dequeue_batch(q, batch_idx, 4);
    print_str("  Batch 2 size: "); print_int(bs); print_str("\n");
    lat = batch_process_batch(q, batch_idx, bs);
    print_str("  Batch 2 latency: "); print_int(lat); print_str("ms\n");

    batch_compute_stats(q);
}

static void test_hot_update(void) {
    print_str("\n--- Model Hot Update ---\n");
    model_registry_t *reg = registry_init();
    int v0 = registry_load_version(reg, 1, 0, 0, "/models/resnet_v1.onnx");
    int v1 = registry_load_version(reg, 1, 1, 0, "/models/resnet_v1.1.onnx");
    int v2 = registry_load_version(reg, 2, 0, 0, "/models/resnet_v2.onnx");
    registry_activate_version(reg, v0);
    print_str("  Versions:\n");
    registry_list_versions(reg);
    print_str("  Switching to v2.0.0...\n");
    registry_activate_version(reg, v2);
    registry_list_versions(reg);
    print_str("  Rollback triggered!\n");
    registry_rollback(reg);
    registry_list_versions(reg);
    (void)v1;
}

static void test_ab_testing(void) {
    print_str("\n--- A/B Testing ---\n");
    ab_config_t acfg;
    acfg.split_mode = SPLIT_PERCENTAGE;
    acfg.traffic_split_a_pct = 50;
    acfg.metrics_window_ms = 60000;
    ab_test_t *t = ab_create("resnet_accuracy_test", 1, 2, &acfg);

    /* Simulate traffic: model A is slightly better */
    for (int uid = 0; uid < 40; uid++) {
        int variant = ab_assign(t, uid);
        int lat = variant == 0 ? 12 : 8;
        float acc = variant == 0 ? 0.92f : 0.87f;
        int succ = (uid % 10 != 0) ? 1 : 0; /* 90% success */
        ab_record_metric(t, variant, lat, acc, succ);
    }
    print_str("  Assigned 40 users\n");
    print_str("    Variant A: "); print_int(t->count_a);
    print_str("  Variant B: "); print_int(t->count_b); print_str("\n");
    ab_report(t);
}

void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);

    int help = 0, test = 0;
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;
    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        int is_h = (arg[0] == '-' && arg[1] == 'h' && arg[2] == '\0');
        int is_help = (arg[0] == '-' && arg[1] == '-' && arg[2] == 'h' &&
                       arg[3] == 'e' && arg[4] == 'l' && arg[5] == 'p' && arg[6] == '\0');
        int is_t = (arg[0] == '-' && arg[1] == 't' && arg[2] == '\0');
        int is_test = (arg[0] == '-' && arg[1] == '-' && arg[2] == 't' &&
                       arg[3] == 'e' && arg[4] == 's' && arg[5] == 't' && arg[6] == '\0');
        if (is_h || is_help) help = 1;
        if (is_t || is_test) test = 1;
        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    print_str("Edge AI Runtime v56.0 - Optimized Inference Engine\n");

    if (help) {
        print_str("Usage: edge_ai_runtime [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run comprehensive demo of all subsystems\n");
        return;
    }

    if (test) {
        print_str("=== Edge AI Runtime Comprehensive Test ===\n");
        test_quantization();
        test_fusion();
        test_dispatch();
        test_batching();
        test_hot_update();
        test_ab_testing();
        print_str("\n=== All Subsystem Tests Complete ===\n");
        return;
    }

    print_str("Use -h for help, -t for comprehensive test\n");
}
