/* edge_inference_engine: Optimized edge inference engine (v1.0)
 * Features: quantization, pruning, distillation, HW accel, batching, caching
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

/* ========================== CONSTANTS ========================== */

#define QUANT_INT8       0x01
#define QUANT_FP16       0x02
#define QUANT_DYNAMIC    0x04
#define QUANT_STATIC     0x08
#define QUANT_AWARE      0x10

#define PRUNE_STRUCTURED   0x01
#define PRUNE_UNSTRUCTURED 0x02
#define PRUNE_ITERATIVE    0x04

#define DISTILL_FEATURE    0x01
#define DISTILL_ATTENTION  0x02
#define DISTILL_LOGIT      0x04

#define HW_GPU_NVIDIA    0x01
#define HW_GPU_AMD       0x02
#define HW_NPU_EDGETPU   0x04
#define HW_NPU_COREML    0x08
#define HW_DSP           0x10
#define HW_CPU           0x20

#define BATCH_DYNAMIC    0x01
#define BATCH_STATIC     0x02
#define BATCH_STREAMING  0x04
#define BATCH_CONTINUOUS 0x08

#define CACHE_LRU        0x01
#define CACHE_PRELOAD    0x02

#define MODEL_NONE       0
#define MODEL_LOADED     1
#define MODEL_OPTIMIZED  2
#define MODEL_ERROR      3

#define MAX_MODELS       16
#define MAX_CACHE        8
#define MAX_LAYERS       32
#define MAX_BATCH        64
#define MAX_PRUNE_ITER   10
#define NAME_LEN         32
#define MAX_REQUESTS     32

/* ======================== DATA STRUCTURES ======================== */

/* Quantization parameters per layer */
typedef struct {
    int   layer_id;
    int   method;
    int   zero_point;
    int   scale_x256;
    float min_val;
    float max_val;
    float accuracy_loss_pct;
} quant_params_t;

/* Pruning result per layer */
typedef struct {
    int   layer_id;
    int   method;
    int   sparsity_pct;
    float sensitivity;
    int   pruned;
} prune_result_t;

/* Distillation config */
typedef struct {
    int   teacher_model_id;
    int   student_model_id;
    int   mode;
    float temperature;
    float alpha;
    float feature_loss;
    float attention_loss;
} distill_config_t;

/* Hardware accelerator descriptor */
typedef struct {
    int   type;
    char  name[NAME_LEN];
    int   memory_mb;
    int   tflops_x100;
    int   power_mw;
    int   active;
    int   ops_dispatched;
} hw_accel_t;

/* Batch optimizer state */
typedef struct {
    int   mode;
    int   max_batch_size;
    int   current_batch;
    int   pending_count;
    int   total_processed;
    int   avg_latency_us;
    int   queue[MAX_BATCH];
} batch_optimizer_t;

/* LRU cache entry */
typedef struct {
    int   model_id;
    int   version;
    int   size_kb;
    int   last_access;
    int   hit_count;
    int   valid;
} cache_entry_t;

/* Inference model */
typedef struct {
    int   model_id;
    char  name[NAME_LEN];
    int   state;
    int   quant_method;
    int   prune_sparsity;
    int   hw_target;
    int   batch_mode;
    int   layers;
    int   params_million;
    int   latency_us;
    int   throughput_qps;
    int   accuracy_pct;
    int   cache_idx;
    int   version;
    int   inference_count;
} inference_model_t;

/* Inference request */
typedef struct {
    int   request_id;
    int   model_id;
    int   priority;
    int   status;
    int   latency_us;
    int   timestamp;
} inference_request_t;

/* Engine global state */
static inference_model_t  g_models[MAX_MODELS];
static cache_entry_t      g_cache[MAX_CACHE];
static hw_accel_t         g_hw[6];
static batch_optimizer_t  g_batcher;
static inference_request_t g_requests[MAX_REQUESTS];
static int g_model_count   = 0;
static int g_request_count = 0;
static int g_next_id       = 1;
static int g_clock         = 0;
static int g_hw_count      = 0;

/* ======================= STRING UTILITIES ======================= */

static int my_strlen(const char *s) {
    int n = 0;
    while (s[n]) n++;
    return n;
}

static void my_strcpy(char *d, const char *s) {
    while (*s) *d++ = *s++;
    *d = '\0';
}

static void my_strncpy(char *d, const char *s, int n) {
    int i = 0;
    while (i < n - 1 && s[i]) { d[i] = s[i]; i++; }
    d[i] = '\0';
}

static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) {
        if (*a != *b) return *a - *b;
        a++; b++;
    }
    return *a - *b;
}

static void print_str(const char *s) { host_print(s); }

static void print_int(int v) {
    char buf[32];
    int pos = 0;
    if (v < 0) { buf[pos++] = '-'; v = -v; }
    if (v == 0) { buf[pos++] = '0'; }
    else {
        int tmp = v, d = 0;
        while (tmp > 0) { d++; tmp /= 10; }
        pos += d;
        buf[pos] = '\0';
        int p = pos - 1;
        while (v > 0) { buf[p--] = '0' + (v % 10); v /= 10; }
    }
    host_print(buf);
}

/* ===================== QUANTIZATION ENGINE ===================== */

static void quant_compute_params(int layer_id, float min_v, float max_v,
                                 int method, quant_params_t *out) {
    out->layer_id = layer_id;
    out->method = method;
    out->min_val = min_v;
    out->max_val = max_v;

    if (method == QUANT_INT8) {
        float range = max_v - min_v;
        out->scale_x256 = (int)(range * 256.0F / 255.0F);
        out->zero_point = (int)(-128.0F - min_v * 255.0F / range);
        out->accuracy_loss_pct = 0.5F + (float)layer_id * 0.1F;
    } else if (method == QUANT_FP16) {
        out->scale_x256 = 256;
        out->zero_point = 0;
        out->accuracy_loss_pct = 0.1F + (float)layer_id * 0.02F;
    } else {
        out->scale_x256 = 128;
        out->zero_point = 0;
        out->accuracy_loss_pct = 1.0F;
    }
}

static int quantize_model(int model_id, int method) {
    int midx = -1;
    for (int i = 0; i < g_model_count; i++) {
        if (g_models[i].model_id == model_id) { midx = i; break; }
    }
    if (midx < 0 || g_models[midx].state == MODEL_NONE) {
        print_str("[quant] ERROR: model not found\n");
        return -1;
    }

    int layers = g_models[midx].layers;
    if (layers > MAX_LAYERS) layers = MAX_LAYERS;

    float total_loss = 0.0F;
    for (int l = 0; l < layers; l++) {
        quant_params_t qp;
        float min_v = -1.0F - (float)l * 0.05F;
        float max_v =  1.0F + (float)l * 0.05F;
        quant_compute_params(l, min_v, max_v, method, &qp);
        total_loss += qp.accuracy_loss_pct;
    }

    float avg_loss = total_loss / (float)layers;
    g_models[midx].quant_method = method;

    int latency_reduction = (method == QUANT_INT8) ? 45 : 25;
    g_models[midx].latency_us -= g_models[midx].latency_us * latency_reduction / 100;
    if (g_models[midx].latency_us < 100) g_models[midx].latency_us = 100;

    int throughput_gain = (method == QUANT_INT8) ? 80 : 35;
    g_models[midx].throughput_qps += g_models[midx].throughput_qps * throughput_gain / 100;

    g_models[midx].state = MODEL_OPTIMIZED;

    const char *mname = (method == QUANT_INT8) ? "INT8" :
                        (method == QUANT_FP16) ? "FP16" : "DYNAMIC";
    print_str("[quant] model=");
    print_str(g_models[midx].name);
    print_str(" method=");
    print_str(mname);
    print_str(" layers=");
    print_int(layers);
    print_str(" avg_loss=");
    print_int((int)(avg_loss * 100.0F));
    print_str("bps latency=");
    print_int(g_models[midx].latency_us);
    print_str("us\n");

    return 0;
}

/* ====================== PRUNING ENGINE ====================== */

static float compute_layer_sensitivity(int layer_id, int total_layers) {
    float base = 0.1F;
    float depth_factor = (float)layer_id / (float)total_layers;
    return base + depth_factor * 0.8F;
}

static int prune_model(int model_id, int method, int target_sparsity) {
    int midx = -1;
    for (int i = 0; i < g_model_count; i++) {
        if (g_models[i].model_id == model_id) { midx = i; break; }
    }
    if (midx < 0) {
        print_str("[prune] ERROR: model not found\n");
        return -1;
    }

    int layers = g_models[midx].layers;
    if (layers > MAX_LAYERS) layers = MAX_LAYERS;

    int iterations = (method & PRUNE_ITERATIVE) ? 5 : 1;
    if (iterations > MAX_PRUNE_ITER) iterations = MAX_PRUNE_ITER;

    float total_params = (float)g_models[midx].params_million;
    float remaining = total_params;

    for (int iter = 0; iter < iterations; iter++) {
        float iter_sparsity = (float)target_sparsity / 100.0F / (float)iterations;
        for (int l = 0; l < layers; l++) {
            float sens = compute_layer_sensitivity(l, layers);
            float layer_prune = iter_sparsity * (1.0F - sens * 0.5F);
            remaining -= remaining * layer_prune;
        }
    }

    float actual_sparsity = (1.0F - remaining / total_params) * 100.0F;
    g_models[midx].prune_sparsity = (int)actual_sparsity;

    int speedup = (int)(actual_sparsity * 0.7F);
    g_models[midx].latency_us -= g_models[midx].latency_us * speedup / 100;
    if (g_models[midx].latency_us < 50) g_models[midx].latency_us = 50;

    int acc_drop = (int)(actual_sparsity * 0.05F);
    g_models[midx].accuracy_pct -= acc_drop;

    g_models[midx].state = MODEL_OPTIMIZED;

    const char *mname = (method & PRUNE_STRUCTURED) ? "structured" : "unstructured";
    print_str("[prune] model=");
    print_str(g_models[midx].name);
    print_str(" method=");
    print_str(mname);
    print_str(" sparsity=");
    print_int((int)actual_sparsity);
    print_str("% params_remaining=");
    print_int((int)remaining);
    print_str("M speedup=");
    print_int(speedup);
    print_str("%\n");

    return 0;
}

static void sensitivity_analysis(int model_id) {
    int midx = -1;
    for (int i = 0; i < g_model_count; i++) {
        if (g_models[i].model_id == model_id) { midx = i; break; }
    }
    if (midx < 0) return;

    int layers = g_models[midx].layers;
    if (layers > MAX_LAYERS) layers = MAX_LAYERS;

    print_str("[prune] sensitivity analysis for ");
    print_str(g_models[midx].name);
    print_str(":\n");

    for (int l = 0; l < layers; l++) {
        float sens = compute_layer_sensitivity(l, layers);
        print_str("  layer=");
        print_int(l);
        print_str(" sensitivity=");
        print_int((int)(sens * 1000.0F));
        print_str("m\n");
    }
}

/* =================== KNOWLEDGE DISTILLATION =================== */

static int distill_model(int teacher_id, int student_id, int mode,
                         float temperature, float alpha) {
    int tidx = -1, sidx = -1;
    for (int i = 0; i < g_model_count; i++) {
        if (g_models[i].model_id == teacher_id) tidx = i;
        if (g_models[i].model_id == student_id) sidx = i;
    }
    if (tidx < 0 || sidx < 0) {
        print_str("[distill] ERROR: teacher or student not found\n");
        return -1;
    }

    float feature_loss = 0.0F;
    float attention_loss = 0.0F;
    float logit_loss = 0.0F;

    int t_layers = g_models[tidx].layers;
    int s_layers = g_models[sidx].layers;
    if (t_layers > MAX_LAYERS) t_layers = MAX_LAYERS;
    if (s_layers > MAX_LAYERS) s_layers = MAX_LAYERS;

    if (mode & DISTILL_FEATURE) {
        int pairs = (s_layers < t_layers) ? s_layers : t_layers;
        for (int l = 0; l < pairs; l++) {
            float t_scale = 1.0F / (1.0F + (float)l * 0.1F);
            float s_scale = 1.0F / (1.0F + (float)l * 0.15F);
            float diff = t_scale - s_scale;
            feature_loss += diff * diff * alpha;
        }
        feature_loss /= (float)pairs;
    }

    if (mode & DISTILL_ATTENTION) {
        int attn_heads_t = 12;
        int attn_heads_s = 6;
        float head_ratio = (float)attn_heads_s / (float)attn_heads_t;
        attention_loss = (1.0F - head_ratio) * temperature * 0.1F;
    }

    if (mode & DISTILL_LOGIT) {
        float t_temp = 1.0F / temperature;
        logit_loss = (1.0F - t_temp) * alpha * 0.05F;
    }

    float total_loss = feature_loss + attention_loss + logit_loss;

    int acc_gain = (int)((1.0F - total_loss) * 5.0F);
    if (acc_gain < 0) acc_gain = 0;
    g_models[sidx].accuracy_pct += acc_gain;
    if (g_models[sidx].accuracy_pct > g_models[tidx].accuracy_pct) {
        g_models[sidx].accuracy_pct = g_models[tidx].accuracy_pct - 1;
    }

    g_models[sidx].state = MODEL_OPTIMIZED;

    print_str("[distill] teacher=");
    print_str(g_models[tidx].name);
    print_str(" student=");
    print_str(g_models[sidx].name);
    print_str(" temp=");
    print_int((int)(temperature * 10.0F));
    print_str(" loss=");
    print_int((int)(total_loss * 10000.0F));
    print_str(" student_acc=");
    print_int(g_models[sidx].accuracy_pct);
    print_str("%\n");

    return 0;
}

/* ================== HARDWARE ACCELERATION ================== */

static void hw_register(int type, const char *name, int mem_mb,
                        int tflops_x100, int power_mw) {
    if (g_hw_count >= 6) return;
    int idx = g_hw_count;
    g_hw[idx].type = type;
    my_strncpy(g_hw[idx].name, name, NAME_LEN - 1);
    g_hw[idx].memory_mb = mem_mb;
    g_hw[idx].tflops_x100 = tflops_x100;
    g_hw[idx].power_mw = power_mw;
    g_hw[idx].active = 0;
    g_hw[idx].ops_dispatched = 0;
    g_hw_count++;
}

static int hw_select_accel(int model_id) {
    int midx = -1;
    for (int i = 0; i < g_model_count; i++) {
        if (g_models[i].model_id == model_id) { midx = i; break; }
    }
    if (midx < 0) return -1;

    int best = -1;
    int best_score = 0;

    for (int i = 0; i < g_hw_count; i++) {
        int perf = g_hw[i].tflops_x100;
        int mem_fit = (g_hw[i].memory_mb >= g_models[midx].params_million * 4) ? 100 : 0;
        int power_ok = (g_hw[i].power_mw < 15000) ? 50 : 0;
        int score = perf + mem_fit + power_ok;

        if (g_models[midx].hw_target != 0 && g_models[midx].hw_target == g_hw[i].type) {
            score += 200;
        }

        if (score > best_score) {
            best_score = score;
            best = i;
        }
    }

    if (best >= 0) {
        g_hw[best].active = 1;
        g_models[midx].hw_target = g_hw[best].type;

        int speedup = g_hw[best].tflops_x100 / 100;
        if (speedup < 2) speedup = 2;
        g_models[midx].latency_us /= speedup;
        if (g_models[midx].latency_us < 10) g_models[midx].latency_us = 10;

        g_models[midx].throughput_qps *= speedup;

        print_str("[hw] accel=");
        print_str(g_hw[best].name);
        print_str(" model=");
        print_str(g_models[midx].name);
        print_str(" speedup=");
        print_int(speedup);
        print_str("x latency=");
        print_int(g_models[midx].latency_us);
        print_str("us\n");
    }

    return best;
}

static void hw_init_defaults(void) {
    hw_register(HW_GPU_NVIDIA, "GPU-NVIDIA-T4", 16384, 8100, 70000);
    hw_register(HW_GPU_AMD, "GPU-AMD-MI100", 32768, 11500, 250000);
    hw_register(HW_NPU_EDGETPU, "NPU-EdgeTPU", 512, 400, 2000);
    hw_register(HW_NPU_COREML, "NPU-CoreML", 1024, 600, 3000);
    hw_register(HW_DSP, "DSP-Hexagon", 256, 200, 1500);
    hw_register(HW_CPU, "CPU-AARM64", 65536, 150, 5000);
}

/* =================== BATCH OPTIMIZER =================== */

static void batch_init(int mode, int max_size) {
    g_batcher.mode = mode;
    g_batcher.max_batch_size = max_size;
    g_batcher.current_batch = 0;
    g_batcher.pending_count = 0;
    g_batcher.total_processed = 0;
    g_batcher.avg_latency_us = 0;

    const char *mname = (mode == BATCH_DYNAMIC) ? "dynamic" :
                        (mode == BATCH_STATIC) ? "static" :
                        (mode == BATCH_STREAMING) ? "streaming" : "continuous";
    print_str("[batch] initialized mode=");
    print_str(mname);
    print_str(" max_size=");
    print_int(max_size);
    print_str("\n");
}

static int batch_enqueue(int model_id, int priority) {
    if (g_request_count >= MAX_REQUESTS) {
        print_str("[batch] queue full\n");
        return -1;
    }

    int idx = g_request_count;
    g_requests[idx].request_id = g_next_id;
    g_requests[idx].model_id = model_id;
    g_requests[idx].priority = priority;
    g_requests[idx].status = 0;
    g_requests[idx].timestamp = g_clock;
    g_request_count++;

    g_batcher.pending_count++;

    print_str("[batch] enqueued req=");
    print_int(g_next_id);
    print_str(" model=");
    print_int(model_id);
    print_str(" priority=");
    print_int(priority);
    print_str(" pending=");
    print_int(g_batcher.pending_count);
    print_str("\n");

    return g_next_id++;
}

static void batch_sort_by_priority(void) {
    for (int i = 0; i < g_request_count - 1; i++) {
        for (int j = i + 1; j < g_request_count; j++) {
            if (g_requests[j].priority > g_requests[i].priority &&
                g_requests[i].status == 0 && g_requests[j].status == 0) {
                inference_request_t tmp = g_requests[i];
                g_requests[i] = g_requests[j];
                g_requests[j] = tmp;
            }
        }
    }
}

static int batch_process(void) {
    batch_sort_by_priority();

    int processed = 0;
    int batch_count = 0;
    int total_lat = 0;

    for (int i = 0; i < g_request_count && batch_count < g_batcher.max_batch_size; i++) {
        if (g_requests[i].status != 0) continue;

        int midx = -1;
        for (int m = 0; m < g_model_count; m++) {
            if (g_models[m].model_id == g_requests[i].model_id) { midx = m; break; }
        }
        if (midx < 0) { g_requests[i].status = 3; continue; }

        g_requests[i].status = 1;
        g_requests[i].latency_us = g_models[midx].latency_us;
        total_lat += g_requests[i].latency_us;

        g_models[midx].inference_count++;
        g_clock += g_requests[i].latency_us / 1000;

        g_requests[i].status = 2;
        processed++;
        batch_count++;
    }

    if (processed > 0) {
        int new_avg = total_lat / processed;
        if (g_batcher.avg_latency_us == 0) {
            g_batcher.avg_latency_us = new_avg;
        } else {
            g_batcher.avg_latency_us = (g_batcher.avg_latency_us + new_avg) / 2;
        }
    }

    g_batcher.total_processed += processed;
    g_batcher.pending_count -= processed;
    if (g_batcher.pending_count < 0) g_batcher.pending_count = 0;

    print_str("[batch] processed=");
    print_int(processed);
    print_str(" total=");
    print_int(g_batcher.total_processed);
    print_str(" avg_lat=");
    print_int(g_batcher.avg_latency_us);
    print_str("us pending=");
    print_int(g_batcher.pending_count);
    print_str("\n");

    return processed;
}

/* ==================== MODEL CACHE (LRU) ==================== */

static int cache_find(int model_id) {
    for (int i = 0; i < MAX_CACHE; i++) {
        if (g_cache[i].valid && g_cache[i].model_id == model_id) return i;
    }
    return -1;
}

static int cache_evict_lru(void) {
    int lru_idx = -1;
    int oldest = 0x7FFFFFFF;

    for (int i = 0; i < MAX_CACHE; i++) {
        if (!g_cache[i].valid) return i;
        if (g_cache[i].last_access < oldest) {
            oldest = g_cache[i].last_access;
            lru_idx = i;
        }
    }

    if (lru_idx >= 0) {
        print_str("[cache] evicting model=");
        print_int(g_cache[lru_idx].model_id);
        print_str(" version=");
        print_int(g_cache[lru_idx].version);
        print_str("\n");
        g_cache[lru_idx].valid = 0;
    }

    return lru_idx;
}

static int cache_insert(int model_id, int version, int size_kb) {
    int idx = cache_find(model_id);
    if (idx >= 0) {
        g_cache[idx].last_access = g_clock;
        g_cache[idx].hit_count++;
        g_cache[idx].version = version;
        print_str("[cache] hit model=");
        print_int(model_id);
        print_str(" hits=");
        print_int(g_cache[idx].hit_count);
        print_str("\n");
        return idx;
    }

    idx = cache_evict_lru();
    if (idx < 0) {
        print_str("[cache] cache full, cannot insert\n");
        return -1;
    }

    g_cache[idx].model_id = model_id;
    g_cache[idx].version = version;
    g_cache[idx].size_kb = size_kb;
    g_cache[idx].last_access = g_clock;
    g_cache[idx].hit_count = 1;
    g_cache[idx].valid = 1;

    print_str("[cache] insert model=");
    print_int(model_id);
    print_str(" v=");
    print_int(version);
    print_str(" size=");
    print_int(size_kb);
    print_str("KB\n");

    return idx;
}

static void cache_preload(int model_ids[], int count) {
    print_str("[cache] preloading ");
    print_int(count);
    print_str(" models\n");
    for (int i = 0; i < count; i++) {
        for (int m = 0; m < g_model_count; m++) {
            if (g_models[m].model_id == model_ids[i]) {
                int size = g_models[m].params_million * 4;
                cache_insert(model_ids[i], g_models[m].version, size);
                break;
            }
        }
    }
}

static void cache_hot_update(int model_id, int new_version) {
    int idx = cache_find(model_id);
    if (idx < 0) {
        print_str("[cache] model not cached, inserting new version\n");
        for (int m = 0; m < g_model_count; m++) {
            if (g_models[m].model_id == model_id) {
                g_models[m].version = new_version;
                cache_insert(model_id, new_version,
                             g_models[m].params_million * 4);
                return;
            }
        }
        return;
    }

    int old_v = g_cache[idx].version;
    g_cache[idx].version = new_version;
    g_cache[idx].last_access = g_clock;

    for (int m = 0; m < g_model_count; m++) {
        if (g_models[m].model_id == model_id) {
            g_models[m].version = new_version;
            break;
        }
    }

    print_str("[cache] hot update model=");
    print_int(model_id);
    print_str(" v");
    print_int(old_v);
    print_str("->v");
    print_int(new_version);
    print_str("\n");
}

/* ===================== MODEL MANAGEMENT ===================== */

static int register_model(const char *name, int layers, int params_m,
                          int base_latency_us, int base_acc) {
    if (g_model_count >= MAX_MODELS) {
        print_str("[engine] max models reached\n");
        return -1;
    }

    int idx = g_model_count;
    g_models[idx].model_id = g_next_id;
    my_strncpy(g_models[idx].name, name, NAME_LEN - 1);
    g_models[idx].state = MODEL_LOADED;
    g_models[idx].quant_method = 0;
    g_models[idx].prune_sparsity = 0;
    g_models[idx].hw_target = 0;
    g_models[idx].batch_mode = 0;
    g_models[idx].layers = layers;
    g_models[idx].params_million = params_m;
    g_models[idx].latency_us = base_latency_us;
    g_models[idx].throughput_qps = 1000000 / base_latency_us;
    g_models[idx].accuracy_pct = base_acc;
    g_models[idx].cache_idx = -1;
    g_models[idx].version = 1;
    g_models[idx].inference_count = 0;

    g_model_count++;
    int id = g_next_id++;

    cache_insert(id, g_models[idx].version, params_m * 4);

    print_str("[engine] registered model=");
    print_str(name);
    print_str(" id=");
    print_int(id);
    print_str(" layers=");
    print_int(layers);
    print_str(" params=");
    print_int(params_m);
    print_str("M acc=");
    print_int(base_acc);
    print_str("%\n");

    return id;
}

/* ======================== MAIN ======================== */

void main(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);

    int demo = 0, help = 0;
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;
    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "--help") == 0) help = 1;
        else if (my_strcmp(arg, "--demo") == 0) demo = 1;
        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    print_str("=== Edge Inference Engine v1.0 ===\n");

    if (help) {
        print_str("Usage: edge_inference_engine [options]\n");
        print_str("  --help  Show this help\n");
        print_str("  --demo  Run full demo pipeline\n");
        return;
    }

    if (!demo) {
        print_str("Use --demo to run the full pipeline demo\n");
        return;
    }

    /* Phase 1: Hardware initialization */
    print_str("\n--- Phase 1: Hardware Init ---\n");
    hw_init_defaults();

    /* Phase 2: Register models */
    print_str("\n--- Phase 2: Model Registration ---\n");
    int teacher_id = register_model("resnet50-teacher", 53, 25, 8000, 92);
    int student_id = register_model("mobilenet-student", 28, 4, 2500, 78);
    int det_id     = register_model("yolo-nano", 15, 7, 4000, 85);
    int nlp_id     = register_model("bert-tiny", 12, 14, 6000, 80);

    /* Phase 3: Quantization */
    print_str("\n--- Phase 3: Quantization ---\n");
    quantize_model(teacher_id, QUANT_INT8);
    quantize_model(student_id, QUANT_FP16);
    quantize_model(det_id, QUANT_INT8);

    /* Phase 4: Pruning */
    print_str("\n--- Phase 4: Pruning ---\n");
    sensitivity_analysis(det_id);
    prune_model(det_id, PRUNE_STRUCTURED | PRUNE_ITERATIVE, 50);
    prune_model(nlp_id, PRUNE_UNSTRUCTURED, 30);

    /* Phase 5: Knowledge Distillation */
    print_str("\n--- Phase 5: Knowledge Distillation ---\n");
    distill_model(teacher_id, student_id,
                  DISTILL_FEATURE | DISTILL_ATTENTION | DISTILL_LOGIT,
                  4.0F, 0.7F);

    /* Phase 6: Hardware Acceleration */
    print_str("\n--- Phase 6: HW Acceleration ---\n");
    hw_select_accel(teacher_id);
    hw_select_accel(det_id);
    hw_select_accel(nlp_id);

    /* Phase 7: Batch Optimization */
    print_str("\n--- Phase 7: Batch Optimization ---\n");
    batch_init(BATCH_DYNAMIC, 8);

    batch_enqueue(teacher_id, 1);
    batch_enqueue(student_id, 3);
    batch_enqueue(det_id, 5);
    batch_enqueue(nlp_id, 2);
    batch_enqueue(teacher_id, 4);
    batch_enqueue(det_id, 2);
    batch_enqueue(student_id, 1);
    batch_enqueue(nlp_id, 5);

    batch_process();

    batch_enqueue(det_id, 3);
    batch_enqueue(teacher_id, 2);
    batch_process();

    /* Phase 8: Cache Operations */
    print_str("\n--- Phase 8: Cache Management ---\n");
    int preload_ids[3];
    preload_ids[0] = teacher_id;
    preload_ids[1] = det_id;
    preload_ids[2] = nlp_id;
    cache_preload(preload_ids, 3);

    cache_hot_update(student_id, 2);
    cache_hot_update(det_id, 2);

    /* Final stats */
    print_str("\n--- Final Model Stats ---\n");
    for (int i = 0; i < g_model_count; i++) {
        print_str("  ");
        print_str(g_models[i].name);
        print_str(": state=");
        print_int(g_models[i].state);
        print_str(" lat=");
        print_int(g_models[i].latency_us);
        print_str("us qps=");
        print_int(g_models[i].throughput_qps);
        print_str(" acc=");
        print_int(g_models[i].accuracy_pct);
        print_str("% inf=");
        print_int(g_models[i].inference_count);
        print_str(" v=");
        print_int(g_models[i].version);
        print_str("\n");
    }

    print_str("\n--- Batch Stats ---\n");
    print_str("  total_processed=");
    print_int(g_batcher.total_processed);
    print_str(" avg_latency=");
    print_int(g_batcher.avg_latency_us);
    print_str("us pending=");
    print_int(g_batcher.pending_count);
    print_str("\n");

    print_str("\n--- Cache Stats ---\n");
    for (int i = 0; i < MAX_CACHE; i++) {
        if (g_cache[i].valid) {
            print_str("  slot=");
            print_int(i);
            print_str(" model=");
            print_int(g_cache[i].model_id);
            print_str(" v=");
            print_int(g_cache[i].version);
            print_str(" size=");
            print_int(g_cache[i].size_kb);
            print_str("KB hits=");
            print_int(g_cache[i].hit_count);
            print_str("\n");
        }
    }

    print_str("\n=== Edge Inference Engine Complete ===\n");
}
