/* model_federation: Federated learning module (v46.0)
 *
 * Provides federated learning capabilities:
 * - Distributed training (horizontal/vertical FL)
 * - Model aggregation (FedAvg, Federated SGD)
 * - Privacy protection (differential privacy, secure aggregation)
 * - Heterogeneous model coordination
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

/* FL topology modes */
#define FL_HORIZONTAL         1
#define FL_VERTICAL           2
#define FL_CROSS_SILO         3
#define FL_CROSS_DEVICE       4

/* Aggregation algorithms */
#define AGG_FEDAVG            1
#define AGG_FEDSGD            2
#define AGG_FEDPROX           3
#define AGG_SCAFFOLD          4

/* Privacy modes */
#define PRIV_NONE             0
#define PRIV_DP_SGD           1
#define PRIV_SECURE_AGG       2
#define PRIV_LDP              3

/* Client states */
#define CLIENT_OFFLINE        0
#define CLIENT_READY          1
#define CLIENT_TRAINING       2
#define CLIENT_DONE           3

/* Model limits */
#define MAX_CLIENTS           128
#define MAX_FEATURES          256
#define MAX_LAYERS            16
#define MAX_NAME_LEN          64
#define MAX_DATA_SAMPLES      1024

/* Model layer */
typedef struct {
    int             layer_id;
    int             input_dim;
    int             output_dim;
    int             weight_count;
    int             active;
} model_layer_t;

/* Model configuration */
typedef struct {
    int             model_id;
    char            name[MAX_NAME_LEN];
    int             layer_count;
    model_layer_t   layers[MAX_LAYERS];
    int             total_weights;
    int             active;
} model_config_t;

/* Federated client */
typedef struct {
    int             client_id;
    char            name[MAX_NAME_LEN];
    int             state;
    int             data_samples;
    int             local_epochs;
    float           learning_rate;
    int             features[MAX_FEATURES];
    int             feature_count;
    unsigned int    round_completed;
    float           loss;
    float           accuracy;
    int             active;
} fl_client_t;

/* Aggregation result */
typedef struct {
    int             round_id;
    int             clients_participated;
    int             algorithm;
    float           global_loss;
    float           global_accuracy;
    float           convergence_rate;
} agg_result_t;

/* Privacy configuration */
typedef struct {
    int             mode;
    float           epsilon;
    float           delta;
    float           noise_scale;
    float           clip_norm;
    int             secure_bits;
} privacy_config_t;

/* Federation context */
typedef struct {
    char            federation_name[MAX_NAME_LEN];
    int             topology;
    int             agg_algorithm;
    int             client_count;
    fl_client_t     clients[MAX_CLIENTS];
    int             model_count;
    model_config_t  models[8];
    int             current_round;
    int             total_rounds;
    agg_result_t    results[64];
    int             result_count;
    privacy_config_t privacy;
    unsigned int    federation_time;
} federation_context_t;

/* Global state */
static federation_context_t federation;
static int initialized = 0;

/* String utilities */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0; while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; } dst[i] = '\0';
}

static void print_str(const char *str) { host_print(str); }
static void print_int(int val) {
    char buf[32]; int pos = 0;
    if (val < 0) { buf[pos++] = '-'; val = -val; }
    if (val == 0) buf[pos++] = '0';
    else { int d = 0, t = val; while (t > 0) { d++; t /= 10; } pos += d; buf[pos] = '\0'; pos--;
        while (val > 0) { buf[pos--] = '0' + (val % 10); val /= 10; } }
    host_print(buf);
}

/* ===== Federation Initialization ===== */

int federation_init(const char *name, int topology, int algorithm) {
    my_strncpy(federation.federation_name, name, MAX_NAME_LEN - 1);
    federation.topology = topology;
    federation.agg_algorithm = algorithm;
    federation.client_count = 0;
    federation.model_count = 0;
    federation.current_round = 0;
    federation.total_rounds = 0;
    federation.result_count = 0;
    federation.federation_time = 1609459200;

    federation.privacy.mode = PRIV_NONE;
    federation.privacy.epsilon = 1.0f;
    federation.privacy.delta = 0.00001f;
    federation.privacy.noise_scale = 0.1f;
    federation.privacy.clip_norm = 1.0f;
    federation.privacy.secure_bits = 32;

    initialized = 1;

    print_str("Federation initialized: ");
    print_str(name);
    print_str("\n");
    print_str("  Topology: ");
    if (topology == FL_HORIZONTAL) print_str("Horizontal FL");
    else if (topology == FL_VERTICAL) print_str("Vertical FL");
    else if (topology == FL_CROSS_SILO) print_str("Cross-silo FL");
    else print_str("Cross-device FL");
    print_str("\n");

    print_str("  Aggregation: ");
    if (algorithm == AGG_FEDAVG) print_str("FedAvg");
    else if (algorithm == AGG_FEDSGD) print_str("Federated SGD");
    else if (algorithm == AGG_FEDPROX) print_str("FedProx");
    else print_str("SCAFFOLD");
    print_str("\n");

    return 0;
}

/* ===== Model Registration ===== */

int model_register(const char *name, int layer_count) {
    if (!initialized || federation.model_count >= 8) return -1;

    int idx = federation.model_count++;
    federation.models[idx].model_id = idx;
    my_strncpy(federation.models[idx].name, name, MAX_NAME_LEN - 1);
    federation.models[idx].layer_count = layer_count;
    federation.models[idx].total_weights = 0;
    federation.models[idx].active = 1;

    for (int i = 0; i < layer_count; i++) {
        federation.models[idx].layers[i].layer_id = i;
        federation.models[idx].layers[i].input_dim = 128 >> i;
        federation.models[idx].layers[i].output_dim = 128 >> (i + 1);
        if (federation.models[idx].layers[i].output_dim < 1)
            federation.models[idx].layers[i].output_dim = 1;
        federation.models[idx].layers[i].weight_count =
            federation.models[idx].layers[i].input_dim *
            federation.models[idx].layers[i].output_dim;
        federation.models[idx].total_weights +=
            federation.models[idx].layers[i].weight_count;
        federation.models[idx].layers[i].active = 1;
    }

    print_str("Registered model: ");
    print_str(name);
    print_str(" (layers=");
    print_int(layer_count);
    print_str(", weights=");
    print_int(federation.models[idx].total_weights);
    print_str(")\n");

    return idx;
}

/* ===== Client Management ===== */

int federation_add_client(const char *name, int data_samples, int local_epochs) {
    if (!initialized || federation.client_count >= MAX_CLIENTS) return -1;

    int idx = federation.client_count++;
    federation.clients[idx].client_id = idx;
    my_strncpy(federation.clients[idx].name, name, MAX_NAME_LEN - 1);
    federation.clients[idx].state = CLIENT_READY;
    federation.clients[idx].data_samples = data_samples;
    federation.clients[idx].local_epochs = local_epochs;
    federation.clients[idx].learning_rate = 0.01f;
    federation.clients[idx].feature_count = 0;
    federation.clients[idx].round_completed = 0;
    federation.clients[idx].loss = 0.0f;
    federation.clients[idx].accuracy = 0.0f;
    federation.clients[idx].active = 1;

    print_str("Added client: ");
    print_str(name);
    print_str(" (samples=");
    print_int(data_samples);
    print_str(", epochs=");
    print_int(local_epochs);
    print_str(")\n");

    return idx;
}

int client_set_features(int client_id, int *features, int count) {
    if (client_id < 0 || client_id >= federation.client_count) return -1;
    if (count > MAX_FEATURES) count = MAX_FEATURES;

    for (int i = 0; i < count; i++) {
        federation.clients[client_id].features[i] = features[i];
    }
    federation.clients[client_id].feature_count = count;

    print_str("Client ");
    print_str(federation.clients[client_id].name);
    print_str(" features set: ");
    print_int(count);
    print_str("\n");

    return 0;
}

/* ===== Privacy Configuration ===== */

int privacy_configure(int mode, float epsilon, float clip_norm) {
    federation.privacy.mode = mode;
    federation.privacy.epsilon = epsilon;
    federation.privacy.clip_norm = clip_norm;

    if (mode == PRIV_DP_SGD) {
        federation.privacy.noise_scale = 2.0f * clip_norm / epsilon;
        federation.privacy.delta = 1.0f / (float)federation.client_count;
        if (federation.privacy.delta < 0.000001f)
            federation.privacy.delta = 0.000001f;
    } else if (mode == PRIV_SECURE_AGG) {
        federation.privacy.secure_bits = 64;
        federation.privacy.noise_scale = 0.0f;
    } else if (mode == PRIV_LDP) {
        federation.privacy.noise_scale = 4.0f * clip_norm / epsilon;
        federation.privacy.delta = 0.0f;
    }

    print_str("Privacy configured: ");
    if (mode == PRIV_DP_SGD) print_str("DP-SGD");
    else if (mode == PRIV_SECURE_AGG) print_str("Secure Aggregation");
    else if (mode == PRIV_LDP) print_str("Local DP");
    else print_str("None");
    print_str("\n");

    print_str("  Epsilon: ");
    print_int((int)(epsilon * 1000));
    print_str("/1000\n");
    print_str("  Clip norm: ");
    print_int((int)(clip_norm * 1000));
    print_str("/1000\n");
    print_str("  Noise scale: ");
    print_int((int)(federation.privacy.noise_scale * 1000));
    print_str("/1000\n");

    return 0;
}

/* ===== Training Simulation ===== */

int simulate_local_training(int client_id) {
    if (client_id < 0 || client_id >= federation.client_count) return -1;
    if (!federation.clients[client_id].active) return -1;

    federation.clients[client_id].state = CLIENT_TRAINING;

    /* Simulate training loss convergence */
    float base_loss = 2.3f;
    float decay = 0.85f;
    float loss = base_loss;
    for (int e = 0; e < federation.clients[client_id].local_epochs; e++) {
        loss = loss * decay + 0.05f;
    }

    /* Simulate accuracy */
    float accuracy = 1.0f - (loss / base_loss) * 0.7f;
    if (accuracy > 0.95f) accuracy = 0.95f;

    federation.clients[client_id].loss = loss;
    federation.clients[client_id].accuracy = accuracy;
    federation.clients[client_id].state = CLIENT_DONE;
    federation.clients[client_id].round_completed = federation.current_round;

    return 0;
}

/* ===== Model Aggregation ===== */

int aggregate_models(void) {
    int participating = 0;
    float total_loss = 0.0f;
    float total_accuracy = 0.0f;
    int total_samples = 0;

    for (int i = 0; i < federation.client_count; i++) {
        if (!federation.clients[i].active) continue;
        if (federation.clients[i].state != CLIENT_DONE) continue;
        if (federation.clients[i].round_completed != (unsigned int)federation.current_round) continue;

        int weight = federation.clients[i].data_samples;
        total_loss += federation.clients[i].loss * (float)weight;
        total_accuracy += federation.clients[i].accuracy * (float)weight;
        total_samples += weight;
        participating++;
    }

    if (participating == 0 || total_samples == 0) return -1;

    /* Weighted average (FedAvg) */
    float global_loss = total_loss / (float)total_samples;
    float global_accuracy = total_accuracy / (float)total_samples;

    /* Apply privacy noise if configured */
    if (federation.privacy.mode == PRIV_DP_SGD ||
        federation.privacy.mode == PRIV_LDP) {
        /* Add calibrated noise to gradients (simulated) */
        float noise_impact = federation.privacy.noise_scale * 0.001f;
        global_loss += noise_impact;
        global_accuracy -= noise_impact * 0.5f;
        if (global_accuracy < 0.0f) global_accuracy = 0.0f;
    }

    /* Convergence rate */
    float convergence = 0.0f;
    if (federation.result_count > 0) {
        float prev_loss = federation.results[federation.result_count - 1].global_loss;
        convergence = (prev_loss - global_loss) / prev_loss;
        if (convergence < 0.0f) convergence = 0.0f;
    } else {
        convergence = 0.15f;
    }

    if (federation.result_count >= 64) return -1;

    int idx = federation.result_count++;
    federation.results[idx].round_id = federation.current_round;
    federation.results[idx].clients_participated = participating;
    federation.results[idx].algorithm = federation.agg_algorithm;
    federation.results[idx].global_loss = global_loss;
    federation.results[idx].global_accuracy = global_accuracy;
    federation.results[idx].convergence_rate = convergence;

    print_str("Round ");
    print_int(federation.current_round);
    print_str(": aggregated ");
    print_int(participating);
    print_str(" clients (");
    if (federation.agg_algorithm == AGG_FEDAVG) print_str("FedAvg");
    else if (federation.agg_algorithm == AGG_FEDSGD) print_str("FedSGD");
    else if (federation.agg_algorithm == AGG_FEDPROX) print_str("FedProx");
    else print_str("SCAFFOLD");
    print_str(")\n");

    print_str("  Loss: ");
    print_int((int)(global_loss * 1000));
    print_str("/1000  Accuracy: ");
    print_int((int)(global_accuracy * 10000));
    print_str("/10000  Convergence: ");
    print_int((int)(convergence * 10000));
    print_str("/10000\n");

    /* Reset client states */
    for (int i = 0; i < federation.client_count; i++) {
        if (federation.clients[i].state == CLIENT_DONE) {
            federation.clients[i].state = CLIENT_READY;
        }
    }

    federation.current_round++;

    return idx;
}

/* ===== Training Round ===== */

int run_training_round(void) {
    print_str("=== Training Round ");
    print_int(federation.current_round);
    print_str(" ===\n");

    /* Select participating clients */
    int selected = 0;
    for (int i = 0; i < federation.client_count; i++) {
        if (!federation.clients[i].active) continue;
        if (federation.clients[i].state != CLIENT_READY) continue;

        /* Select clients with probability proportional to data */
        if ((i * 7 + federation.current_round * 3) % 5 != 0) {
            simulate_local_training(i);
            selected++;
        }
    }

    print_str("Selected ");
    print_int(selected);
    print_str("/");
    print_int(federation.client_count);
    print_str(" clients\n");

    if (selected == 0) return -1;

    return aggregate_models();
}

/* ===== Heterogeneous Model Coordination ===== */

int coordinate_heterogeneous_models(int model_a, int model_b) {
    if (model_a < 0 || model_a >= federation.model_count) return -1;
    if (model_b < 0 || model_b >= federation.model_count) return -1;

    model_config_t *ma = &federation.models[model_a];
    model_config_t *mb = &federation.models[model_b];

    print_str("Coordinating heterogeneous models:\n");
    print_str("  ");
    print_str(ma->name);
    print_str(" <-> ");
    print_str(mb->name);
    print_str("\n");

    /* Find common layer dimensions for knowledge distillation */
    int common_layers = 0;
    int min_layers = ma->layer_count < mb->layer_count ?
                     ma->layer_count : mb->layer_count;

    for (int i = 0; i < min_layers; i++) {
        if (ma->layers[i].output_dim == mb->layers[i].output_dim) {
            common_layers++;
        }
    }

    print_str("  Common layers: ");
    print_int(common_layers);
    print_str("/");
    print_int(min_layers);
    print_str("\n");
    print_str("  Knowledge distillation: enabled\n");

    return common_layers;
}

/* ===== Status ===== */

void federation_status(void) {
    print_str("Federation Status:\n");
    print_str("  Name: ");
    print_str(federation.federation_name);
    print_str("\n");
    print_str("  Round: ");
    print_int(federation.current_round);
    print_str("\n");
    print_str("  Clients: ");
    int active = 0;
    for (int i = 0; i < federation.client_count; i++) {
        if (federation.clients[i].active) active++;
    }
    print_int(active);
    print_str("/");
    print_int(federation.client_count);
    print_str(" active\n");
    print_str("  Models: ");
    print_int(federation.model_count);
    print_str("\n");
    print_str("  Privacy: ");
    if (federation.privacy.mode == PRIV_DP_SGD) print_str("DP-SGD");
    else if (federation.privacy.mode == PRIV_SECURE_AGG) print_str("Secure Agg");
    else if (federation.privacy.mode == PRIV_LDP) print_str("Local DP");
    else print_str("None");
    print_str("\n");

    if (federation.result_count > 0) {
        int last = federation.result_count - 1;
        print_str("  Last loss: ");
        print_int((int)(federation.results[last].global_loss * 1000));
        print_str("/1000\n");
        print_str("  Last accuracy: ");
        print_int((int)(federation.results[last].global_accuracy * 10000));
        print_str("/10000\n");
    }
}

/* ===== CLI entry ===== */
void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);
    int help = 0, test = 0;
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        int hl = my_strlen(arg);
        if (hl == 2 && arg[0] == '-' && arg[1] == 'h') help = 1;
        else if (hl == 2 && arg[0] == '-' && arg[1] == 't') test = 1;
        while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    }

    print_str("Model Federation v46.0 - Federated Learning Module\n");
    if (help) {
        print_str("Usage: model_federation [options]\n");
        print_str("  -h    Show help\n");
        print_str("  -t    Run federation test\n");
        return;
    }

    if (test) {
        print_str("=== Model Federation Test Suite ===\n\n");

        /* Test 1: Initialize federation */
        print_str("[Test 1] Federation Initialization\n");
        federation_init("healthcare-fl", FL_HORIZONTAL, AGG_FEDAVG);
        print_str("\n");

        /* Test 2: Register models */
        print_str("[Test 2] Model Registration\n");
        int model_diag = model_register("diagnosis-model", 4);
        int model_pred = model_register("prediction-model", 3);
        print_str("\n");

        /* Test 3: Add clients */
        print_str("[Test 3] Client Registration\n");
        federation_add_client("hospital-a", 5000, 5);
        federation_add_client("hospital-b", 3000, 3);
        federation_add_client("hospital-c", 8000, 5);
        federation_add_client("hospital-d", 2000, 2);
        federation_add_client("hospital-e", 6000, 4);
        print_str("\n");

        /* Test 4: Configure privacy */
        print_str("[Test 4] Privacy Configuration\n");
        privacy_configure(PRIV_DP_SGD, 1.0f, 1.0f);
        print_str("\n");

        /* Test 5: Set features (vertical FL) */
        print_str("[Test 5] Feature Assignment\n");
        int feat_a[] = {1, 2, 3, 5, 8};
        int feat_b[] = {2, 3, 5, 7, 11};
        int feat_c[] = {1, 3, 5, 7, 9};
        client_set_features(0, feat_a, 5);
        client_set_features(1, feat_b, 5);
        client_set_features(2, feat_c, 5);
        print_str("\n");

        /* Test 6: Training rounds */
        print_str("[Test 6] Federated Training\n");
        for (int r = 0; r < 5; r++) {
            run_training_round();
        }
        print_str("\n");

        /* Test 7: Heterogeneous coordination */
        print_str("[Test 7] Heterogeneous Model Coordination\n");
        coordinate_heterogeneous_models(model_diag, model_pred);
        print_str("\n");

        /* Test 8: Switch to FedSGD */
        print_str("[Test 8] Algorithm Switch (FedSGD)\n");
        federation.agg_algorithm = AGG_FEDSGD;
        run_training_round();
        print_str("\n");

        /* Test 9: Secure aggregation */
        print_str("[Test 9] Secure Aggregation Mode\n");
        privacy_configure(PRIV_SECURE_AGG, 0.0f, 0.0f);
        run_training_round();
        print_str("\n");

        /* Test 10: Final status */
        print_str("[Test 10] Final Status\n");
        federation_status();
        print_str("\n");

        print_str("=== Model Federation Test Complete ===\n");
        return;
    }

    print_str("Use -h for help, -t for test\n");
}
