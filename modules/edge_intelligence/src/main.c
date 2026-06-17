/* edge_intelligence: Edge AI inference platform (v1.0)
 * Model compression, inference optimization, federated learning, TinyML, scheduling
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

#define MAX_LAYERS  16
#define MAX_NODES   8
#define MAX_MODELS  4
#define MAX_DATASET 32

#define QUANT_FP32  0x00
#define QUANT_INT8  0x01
#define QUANT_INT4  0x02
#define QUANT_BIN   0x03

#define PRUNE_NONE   0x00
#define PRUNE_STRUCT 0x01
#define PRUNE_UNSTR  0x02

#define FL_AVG    0x01
#define FL_PROX   0x02
#define FL_PRIV   0x03

#define SCHED_ROUND   0x01
#define SCHED_ENERGY 0x02
#define SCHED_LATENCY 0x03

typedef struct {
    int    in_dim;
    int    out_dim;
    int    kernel;
    int    stride;
    int    params;
    int    flops;
    int    quant;
    double prune_ratio;
    double weight_scale;
    int    weight_zero;
    int    active;
} layer_t;

typedef struct {
    int    n_layers;
    layer_t layers[MAX_LAYERS];
    int    total_params;
    int    total_flops;
    double model_size_kb;
    double accuracy;
    double latency_ms;
    double energy_mj;
    int    quant_type;
    int    prune_type;
    int    active;
} model_t;

typedef struct {
    int    n_models;
    model_t models[MAX_MODELS];
    int    total_params;
    double total_size_kb;
    double avg_accuracy;
    double avg_latency_ms;
    double avg_energy_mj;
    int    compression_ratio;
} edge_model_t;

typedef struct {
    int    n_nodes;
    int    n_rounds;
    int    fl_method;
    int    local_epochs;
    double learning_rate;
    double batch_size;
    double epsilon;
    int    privacy_budget;
    int    total_samples;
    double global_loss;
    double global_accuracy;
    int    converged;
    int    active;
} federated_t;

typedef struct {
    int    n_layers;
    int    n_neurons;
    int    n_params;
    double activation_threshold;
    double decay_rate;
    double spike_count;
    double inference_ms;
    double energy_uj;
    int    active;
} tinymodel_t;

typedef struct {
    int    method;
    int    n_tasks;
    int    n_nodes;
    double latency_budget_ms;
    double energy_budget_mj;
    double bandwidth_mbps;
    int    offloaded;
    int    local;
    double total_latency;
    double total_energy;
    int    active;
} scheduler_t;

typedef struct {
    double data[MAX_DATASET];
    int    labels[MAX_DATASET];
    int    n_samples;
    int    n_features;
    int    active;
} dataset_t;

typedef struct {
    int    n_models;
    int    n_nodes;
    int    n_fl_rounds;
    int    compression_applied;
    double avg_accuracy;
    double avg_latency_ms;
    double avg_energy_mj;
    int    fl_converged;
    int    schedule_count;
} edge_state_t;

static edge_model_t edge_model;
static federated_t fed;
static tinymodel_t tiny;
static scheduler_t sched;
static dataset_t ds;
static edge_state_t es;

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
static double dabs(double x) { return x < 0 ? -x : x; }
static double dsqrt(double x) {
    if (x <= 0.0) return 0.0;
    double g = x, p = x / g;
    for (int i = 0; i < 30; i++) { g = (g + p) * 0.5; p = x / g; }
    return g;
}
static double dexp(double x) {
    if (x > 20.0) return 5e8;
    if (x < -20.0) return 0.0;
    double sum = 1.0, term = 1.0;
    for (int i = 1; i < 30; i++) { term *= x / i; sum += term; }
    return sum;
}
static int drand_int(int seed) {
    int h = seed ^ 0x5DEECE66D;
    h = (h * 1103515245 + 12345) & 0x7FFFFFFF;
    return (h & 0xFFFF) / 65536;
}

int ei_init(void) {
    if (initialized) return -1;
    es.n_models = 0; es.n_nodes = 0; es.n_fl_rounds = 0;
    es.compression_applied = 0; es.avg_accuracy = 0.0;
    es.avg_latency_ms = 0.0; es.avg_energy_mj = 0.0;
    es.fl_converged = 0; es.schedule_count = 0;
    edge_model.n_models = 0; edge_model.total_params = 0;
    edge_model.total_size_kb = 0.0; edge_model.avg_accuracy = 0.0;
    edge_model.avg_latency_ms = 0.0; edge_model.avg_energy_mj = 0.0;
    edge_model.compression_ratio = 1;
    fed.n_nodes = 0; fed.n_rounds = 0; fed.fl_method = 0;
    fed.global_loss = 1.0; fed.global_accuracy = 0.0;
    fed.converged = 0; fed.active = 0;
    tiny.n_layers = 0; tiny.n_neurons = 0; tiny.n_params = 0;
    tiny.active = 0;
    sched.method = 0; sched.n_tasks = 0; sched.n_nodes = 0;
    sched.active = 0;
    ds.n_samples = 0; ds.n_features = 0; ds.active = 0;
    for (int i = 0; i < MAX_MODELS; i++) edge_model.models[i].active = 0;
    initialized = 1;
    print_str("[EI] Edge intelligence platform initialized\n");
    return 0;
}

int ei_create_model(int n_layers) {
    if (edge_model.n_models >= MAX_MODELS) return -1;
    model_t* m = &edge_model.models[edge_model.n_models];
    m->n_layers = n_layers < MAX_LAYERS ? n_layers : MAX_LAYERS;
    m->total_params = 0; m->total_flops = 0;
    m->model_size_kb = 0.0; m->accuracy = 0.0;
    m->latency_ms = 0.0; m->energy_mj = 0.0;
    m->quant_type = QUANT_FP32; m->prune_type = PRUNE_NONE;
    m->active = 1;
    int id = edge_model.n_models++;
    es.n_models++;
    return id;
}

void ei_add_conv_layer(int model_id, int in_c, int out_c, int kernel, int stride) {
    if (model_id >= edge_model.n_models) return;
    model_t* m = &edge_model.models[model_id];
    if (m->n_layers >= MAX_LAYERS) return;
    layer_t* l = &m->layers[m->n_layers];
    l->in_dim = in_c; l->out_dim = out_c;
    l->kernel = kernel; l->stride = stride;
    l->params = in_c * out_c * kernel * kernel;
    l->flops = l->params * 2 * (32 / stride) * (32 / stride);
    l->quant = QUANT_FP32; l->prune_ratio = 0.0;
    l->weight_scale = 1.0; l->weight_zero = 0; l->active = 1;
    m->total_params += l->params;
    m->total_flops += l->flops;
    m->model_size_kb += l->params * 4.0 / 1024.0;
    print_str("[EI] Conv layer: "); print_int(in_c); print_str("->"); print_int(out_c);
    print_str(" k="); print_int(kernel); print_str(" params="); print_int(l->params); print_str("\n");
    m->n_layers++;
}

void ei_add_dense_layer(int model_id, int in_dim, int out_dim) {
    if (model_id >= edge_model.n_models) return;
    model_t* m = &edge_model.models[model_id];
    if (m->n_layers >= MAX_LAYERS) return;
    layer_t* l = &m->layers[m->n_layers];
    l->in_dim = in_dim; l->out_dim = out_dim;
    l->kernel = 1; l->stride = 1;
    l->params = in_dim * out_dim;
    l->flops = l->params * 2;
    l->quant = QUANT_FP32; l->prune_ratio = 0.0;
    l->weight_scale = 1.0; l->weight_zero = 0; l->active = 1;
    m->total_params += l->params;
    m->total_flops += l->flops;
    m->model_size_kb += l->params * 4.0 / 1024.0;
    print_str("[EI] Dense layer: "); print_int(in_dim); print_str("->"); print_int(out_dim);
    print_str(" params="); print_int(l->params); print_str("\n");
    m->n_layers++;
}

void ei_quantize(int model_id, int quant_type) {
    if (model_id >= edge_model.n_models) return;
    model_t* m = &edge_model.models[model_id];
    m->quant_type = quant_type;
    double ratio = 1.0;
    if (quant_type == QUANT_INT8) { ratio = 4.0; m->model_size_kb /= 4.0; }
    else if (quant_type == QUANT_INT4) { ratio = 8.0; m->model_size_kb /= 8.0; }
    else if (quant_type == QUANT_BIN) { ratio = 32.0; m->model_size_kb /= 32.0; }
    for (int i = 0; i < m->n_layers; i++) {
        m->layers[i].quant = quant_type;
        m->layers[i].weight_scale = 1.0 / ratio;
    }
    m->accuracy *= (1.0 - 0.02 * (ratio > 4.0 ? 4.0 : ratio));
    m->latency_ms = m->total_flops / (1e6 / ratio);
    m->energy_mj = m->latency_ms * 0.5 / ratio;
    es.compression_applied++;
    print_str("[EI] Quantized to ");
    if (quant_type == QUANT_INT8) print_str("INT8");
    else if (quant_type == QUANT_INT4) print_str("INT4");
    else if (quant_type == QUANT_BIN) print_str("Binary");
    else print_str("FP32");
    print_str(" ratio="); print_int((int)ratio); print_str("x size=");
    print_int((int)m->model_size_kb); print_str("KB\n");
}

void ei_prune(int model_id, int prune_type, double ratio) {
    if (model_id >= edge_model.n_models) return;
    model_t* m = &edge_model.models[model_id];
    m->prune_type = prune_type;
    double r = ratio < 0.9 ? ratio : 0.9;
    for (int i = 0; i < m->n_layers; i++) {
        m->layers[i].prune_ratio = r;
        int orig_params = m->layers[i].params;
        m->layers[i].params = (int)(orig_params * (1.0 - r));
        m->layers[i].weight_zero = (int)(orig_params * r);
    }
    int new_total = 0;
    for (int i = 0; i < m->n_layers; i++) new_total += m->layers[i].params;
    m->total_params = new_total;
    m->total_flops = (int)(m->total_flops * (1.0 - r));
    m->model_size_kb *= (1.0 - r);
    m->accuracy *= (1.0 - 0.03 * r * 10.0);
    m->latency_ms *= (1.0 - r * 0.5);
    m->energy_mj *= (1.0 - r * 0.5);
    es.compression_applied++;
    print_str("[EI] Pruned: method="); print_int(prune_type);
    print_str(" ratio="); print_int((int)(ratio * 100));
    print_str("% params="); print_int(m->total_params);
    print_str(" acc="); print_int((int)(m->accuracy * 100)); print_str("%\n");
}

void ei_knowledge_distill(int teacher_id, int student_id) {
    if (teacher_id >= edge_model.n_models || student_id >= edge_model.n_models) return;
    model_t* t = &edge_model.models[teacher_id];
    model_t* s = &edge_model.models[student_id];
    s->accuracy = t->accuracy * 0.95;
    s->latency_ms = s->latency_ms * 0.8;
    print_str("[EI] Knowledge distillation: teacher acc=");
    print_int((int)(t->accuracy * 100)); print_str("% -> student acc=");
    print_int((int)(s->accuracy * 100)); print_str("%\n");
}

void ei_compute_inference(int model_id) {
    if (model_id >= edge_model.n_models) return;
    model_t* m = &edge_model.models[model_id];
    if (m->latency_ms < 0.001) m->latency_ms = m->total_flops / 1e6;
    if (m->energy_mj < 0.001) m->energy_mj = m->latency_ms * 0.5;
    if (m->accuracy < 0.01) m->accuracy = 0.95 - m->n_layers * 0.01;
    print_str("[EI] Inference model "); print_int(model_id);
    print_str(": params="); print_int(m->total_params);
    print_str(" size="); print_int((int)m->model_size_kb); print_str("KB");
    print_str(" latency="); print_int((int)(m->latency_ms * 1000)); print_str("ms");
    print_str(" energy="); print_int((int)(m->energy_mj * 1000)); print_str("mJ");
    print_str(" acc="); print_int((int)(m->accuracy * 100)); print_str("%\n");
}

void ei_federated_init(int n_nodes, int method, double lr, int local_epochs) {
    fed.n_nodes = n_nodes; fed.fl_method = method;
    fed.learning_rate = lr; fed.local_epochs = local_epochs;
    fed.n_rounds = 0; fed.global_loss = 1.0;
    fed.global_accuracy = 0.1; fed.converged = 0;
    fed.epsilon = 1.0; fed.privacy_budget = 10;
    fed.total_samples = 0; fed.active = 1;
    fed.batch_size = 32;
    print_str("[EI] Federated learning: nodes="); print_int(n_nodes);
    print_str(" method="); print_int(method);
    print_str(" lr="); print_int((int)(lr * 10000)); print_str("\n");
}

void ei_federated_round(void) {
    if (!fed.active) return;
    fed.n_rounds++;
    double local_loss[8];
    double local_acc[8];
    for (int i = 0; i < fed.n_nodes && i < MAX_NODES; i++) {
        local_loss[i] = fed.global_loss * (0.9 + drand_int(i + fed.n_rounds) * 0.2);
        local_acc[i] = fed.global_accuracy + (1.0 - fed.global_accuracy) * 0.1;
        for (int e = 0; e < fed.local_epochs; e++) {
            local_loss[i] *= (1.0 - fed.learning_rate * 0.5);
            local_acc[i] += (1.0 - local_acc[i]) * fed.learning_rate * 0.3;
        }
    }
    double avg_loss = 0.0, avg_acc = 0.0;
    for (int i = 0; i < fed.n_nodes && i < MAX_NODES; i++) {
        avg_loss += local_loss[i]; avg_acc += local_acc[i];
    }
    fed.global_loss = avg_loss / fed.n_nodes;
    fed.global_accuracy = avg_acc / fed.n_nodes;
    if (fed.fl_method == FL_PROX) fed.global_loss *= 0.98;
    if (fed.fl_method == FL_PRIV) fed.privacy_budget--;
    es.n_fl_rounds++;
    print_str("[EI] FL round "); print_int(fed.n_rounds);
    print_str(": loss="); print_int((int)(fed.global_loss * 10000));
    print_str("/10000 acc="); print_int((int)(fed.global_accuracy * 100)); print_str("%");
    if (fed.fl_method == FL_PRIV) { print_str(" eps="); print_int(fed.privacy_budget); }
    print_str("\n");
    if (fed.global_loss < 0.05 || fed.n_rounds > 50) {
        fed.converged = 1;
        es.fl_converged = 1;
    }
}

void ei_create_tinymodel(int n_layers, int n_neurons) {
    tiny.n_layers = n_layers; tiny.n_neurons = n_neurons;
    tiny.n_params = n_layers * n_neurons * n_neurons;
    tiny.activation_threshold = 0.5;
    tiny.decay_rate = 0.9;
    tiny.spike_count = 0.0;
    tiny.inference_ms = tiny.n_params * 0.00001;
    tiny.energy_uj = tiny.n_params * 0.001;
    tiny.active = 1;
    print_str("[EI] TinyML model: layers="); print_int(n_layers);
    print_str(" neurons="); print_int(n_neurons);
    print_str(" params="); print_int(tiny.n_params);
    print_str(" energy="); print_int((int)(tiny.energy_uj * 1000)); print_str("uJ\n");
}

void ei_tinyml_inference(void) {
    if (!tiny.active) return;
    double spikes = tiny.n_neurons * 0.3;
    tiny.spike_count += spikes;
    tiny.inference_ms = tiny.n_params * 0.00001 * (spikes / tiny.n_neurons);
    tiny.energy_uj = spikes * 0.01;
    print_str("[EI] TinyML inference: spikes="); print_int((int)spikes);
    print_str(" time="); print_int((int)(tiny.inference_ms * 1000)); print_str("us");
    print_str(" energy="); print_int((int)(tiny.energy_uj * 1000)); print_str("uJ\n");
}

void ei_schedule_tasks(int method, int n_tasks) {
    sched.method = method; sched.n_tasks = n_tasks;
    sched.n_nodes = fed.n_nodes > 0 ? fed.n_nodes : 4;
    sched.latency_budget_ms = 100.0;
    sched.energy_budget_mj = 50.0;
    sched.bandwidth_mbps = 10.0;
    sched.offloaded = 0; sched.local = 0;
    sched.total_latency = 0.0; sched.total_energy = 0.0;
    for (int t = 0; t < n_tasks; t++) {
        double task_flops = 1e5 + t * 5e4;
        double local_lat = task_flops / 1e6;
        double offload_lat = task_flops / 1e7 + 5.0;
        double local_energy = local_lat * 10.0;
        double offload_energy = task_flops / 1e7 * 2.0 + 1.0;
        if (method == SCHED_LATENCY) {
            if (offload_lat < local_lat) { sched.offloaded++; sched.total_latency += offload_lat; sched.total_energy += offload_energy; }
            else { sched.local++; sched.total_latency += local_lat; sched.total_energy += local_energy; }
        } else if (method == SCHED_ENERGY) {
            if (offload_energy < local_energy) { sched.offloaded++; sched.total_latency += offload_lat; sched.total_energy += offload_energy; }
            else { sched.local++; sched.total_latency += local_lat; sched.total_energy += local_energy; }
        } else {
            sched.local++;
            sched.total_latency += local_lat;
            sched.total_energy += local_energy;
        }
    }
    sched.active = 1;
    es.schedule_count++;
    print_str("[EI] Schedule: method="); print_int(method);
    print_str(" tasks="); print_int(n_tasks);
    print_str(" offload="); print_int(sched.offloaded);
    print_str(" local="); print_int(sched.local);
    print_str(" lat="); print_int((int)(sched.total_latency * 1000)); print_str("ms");
    print_str(" energy="); print_int((int)(sched.total_energy * 100)); print_str("mJ\n");
}

void ei_print_state(void) {
    print_str("[EI] Models="); print_int(es.n_models);
    print_str(" Compress="); print_int(es.compression_applied);
    print_str(" FL_rounds="); print_int(es.n_fl_rounds);
    print_str(" FL_conv="); print_int(es.fl_converged);
    print_str(" Schedules="); print_int(es.schedule_count);
    print_str("\n");
}

int main(void) {
    print_str("=== Edge Intelligence Platform Demo ===\n\n");
    ei_init();

    print_str("Creating CNN model...\n");
    int m1 = ei_create_model(6);
    ei_add_conv_layer(m1, 3, 16, 3, 1);
    ei_add_conv_layer(m1, 16, 32, 3, 2);
    ei_add_conv_layer(m1, 32, 64, 3, 2);
    ei_add_dense_layer(m1, 64 * 8 * 8, 128);
    ei_add_dense_layer(m1, 128, 64);
    ei_add_dense_layer(m1, 64, 10);
    ei_compute_inference(m1);

    print_str("\nQuantizing to INT8...\n");
    ei_quantize(m1, QUANT_INT8);
    ei_compute_inference(m1);

    print_str("\nQuantizing to INT4...\n");
    int m2 = ei_create_model(6);
    ei_add_conv_layer(m2, 3, 16, 3, 1);
    ei_add_conv_layer(m2, 16, 32, 3, 2);
    ei_add_conv_layer(m2, 32, 64, 3, 2);
    ei_add_dense_layer(m2, 64 * 8 * 8, 128);
    ei_add_dense_layer(m2, 128, 64);
    ei_add_dense_layer(m2, 64, 10);
    ei_quantize(m2, QUANT_INT4);
    ei_compute_inference(m2);

    print_str("\nUnstructured pruning (50%)...\n");
    ei_prune(m1, PRUNE_UNSTR, 0.5);
    ei_compute_inference(m1);

    print_str("\nStructured pruning (70%)...\n");
    ei_prune(m2, PRUNE_STRUCT, 0.7);
    ei_compute_inference(m2);

    print_str("\nKnowledge distillation...\n");
    ei_knowledge_distill(m1, m2);

    print_str("\nFederated learning (FedAvg)...\n");
    ei_federated_init(4, FL_AVG, 0.01, 5);
    for (int i = 0; i < 10; i++) ei_federated_round();

    print_str("\nFederated learning (FedProx)...\n");
    ei_federated_init(6, FL_PROX, 0.005, 3);
    for (int i = 0; i < 8; i++) ei_federated_round();

    print_str("\nFederated learning (differential privacy)...\n");
    ei_federated_init(4, FL_PRIV, 0.01, 5);
    for (int i = 0; i < 8; i++) ei_federated_round();

    print_str("\nTinyML model...\n");
    ei_create_tinymodel(3, 16);
    for (int i = 0; i < 5; i++) ei_tinyml_inference();

    print_str("\nTask scheduling (round-robin)...\n");
    ei_schedule_tasks(SCHED_ROUND, 8);

    print_str("\nTask scheduling (latency-opt)...\n");
    ei_schedule_tasks(SCHED_LATENCY, 8);

    print_str("\nTask scheduling (energy-opt)...\n");
    ei_schedule_tasks(SCHED_ENERGY, 8);

    ei_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
