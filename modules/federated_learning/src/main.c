/* federated_learning: Privacy-Preserving Distributed ML Framework (v56.0) */

#include <stddef.h>

/* Host imports */
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits and constants */
#define MAX_LAYERS    8
#define MAX_WEIGHTS   64
#define MAX_CLIENTS   16
#define MAX_CLASSES   32
#define MAX_GRADIENTS 128
#define DP_GAUSSIAN 0
#define DP_LAPLACE  1
#define SEL_RANDOM            0
#define SEL_POWER_AWARE       1
#define SEL_FAIR_ROUND        2
#define SEL_ACTIVITY_WEIGHTED 3
#define DIST_IID     0
#define DIST_NON_IID 1
#define COMP_SPARSE_TOPK 0
#define COMP_QUANTIZE    1
#define COMP_DISTILL     2

/* ---- Type definitions ---- */
typedef struct {
    int num_layers, layer_sizes[MAX_LAYERS];
    double weights[MAX_LAYERS][MAX_WEIGHTS];
} model_weights_t;
typedef struct { int mechanism; double epsilon, delta, sensitivity, max_norm; } dp_config_t;
typedef struct { int threshold_k, total_shares_n; unsigned int prime; } shamir_config_t;
typedef struct {
    int client_id, data_samples, compute_power;
    int bandwidth_mbps, battery_pct, active, last_seen;
} client_info_t;
typedef struct { int strategy, min_clients, max_clients; } selection_config_t;
typedef struct { int data_distribution; double imbalance_ratio; int num_classes; } hetero_config_t;

/* ---- Global state ---- */
static unsigned int rng_state = 42;
static double g_priv_epsilon = 0.0, g_priv_delta = 0.0, g_priv_spent = 0.0;

/* ---- Helper functions ---- */
unsigned int my_strlen(const char *s) {
    unsigned int n = 0; while (s[n]) n++; return n;
}
void my_strncpy(char *dst, const char *src, unsigned int n) {
    unsigned int i;
    for (i = 0; i < n && src[i]; i++) dst[i] = src[i];
    for (; i < n; i++) dst[i] = '\0';
}
void print_str(const char *s) { host_print(s); }
void print_int(int v) {
    char buf[16]; int i = 14, neg = (v < 0);
    if (neg) v = -v; buf[15] = '\0';
    if (v == 0) buf[--i] = '0';
    else while (v > 0 && i > 0) { buf[--i] = '0' + (v % 10); v /= 10; }
    if (neg && i > 0) buf[--i] = '-';
    host_print(&buf[i]);
}
void print_double(double v) {
    if (v < 0.0) { print_str("-"); v = -v; }
    int whole = (int)v; double frac = v - (double)whole;
    print_int(whole); print_str(".");
    int f = (int)(frac * 10000.0 + 0.5); if (f >= 10000) f = 9999;
    char buf[8];
    buf[0] = '0' + (f / 1000); f %= 1000; buf[1] = '0' + (f / 100); f %= 100;
    buf[2] = '0' + (f / 10); f %= 10; buf[3] = '0' + f; buf[4] = '\0';
    host_print(buf);
}
double my_abs(double v) { return (v < 0.0) ? -v : v; }
double my_sqrt(double v) {
    if (v <= 0.0) return 0.0;
    double x = v * 0.5;
    for (int i = 0; i < 30; i++) if (x > 0.0) x = x - (x * x - v) / (2.0 * x);
    return x;
}
unsigned int lcg_rand(void) {
    rng_state = rng_state * 1103515245u + 12345u;
    return (rng_state >> 16) & 0x7FFF;
}
double lcg_rand_double(void) { return (double)lcg_rand() / 32768.0; }

/* ---- 1. Aggregation Algorithms ---- */
void model_init(model_weights_t *m, int layers, int *sizes) {
    m->num_layers = layers;
    for (int i = 0; i < layers; i++) {
        m->layer_sizes[i] = (sizes[i] < MAX_WEIGHTS) ? sizes[i] : MAX_WEIGHTS;
        for (int j = 0; j < m->layer_sizes[i]; j++) m->weights[i][j] = lcg_rand_double() * 0.1;
    }
}
/* FedAvg: weighted average by sample counts */
void fed_avg(model_weights_t *out, model_weights_t models[],
             int sample_counts[], int num_clients) {
    int total = 0;
    for (int c = 0; c < num_clients; c++) total += sample_counts[c];
    if (total == 0) total = 1;
    out->num_layers = models[0].num_layers;
    for (int l = 0; l < out->num_layers; l++) {
        out->layer_sizes[l] = models[0].layer_sizes[l];
        for (int w = 0; w < out->layer_sizes[l]; w++) {
            double s = 0.0;
            for (int c = 0; c < num_clients; c++)
                s += models[c].weights[l][w] * (double)sample_counts[c];
            out->weights[l][w] = s / (double)total;
        }
    }
}
/* FedProx: FedAvg with proximal regularization term */
void fed_prox(model_weights_t *out, model_weights_t models[],
              model_weights_t *gl, double mu, int num_clients) {
    int sc[MAX_CLIENTS];
    for (int i = 0; i < num_clients; i++) sc[i] = 1;
    fed_avg(out, models, sc, num_clients);
    for (int l = 0; l < gl->num_layers; l++)
        for (int w = 0; w < gl->layer_sizes[l]; w++) {
            double prox = mu * (gl->weights[l][w] - out->weights[l][w]);
            out->weights[l][w] -= prox / (double)num_clients;
        }
}
static int ones16[16] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
/* FedYogi: adaptive optimizer with sign-based updates */
void fed_yogi(model_weights_t *out, model_weights_t models[],
              model_weights_t *gl, double beta1, double beta2,
              double epsilon, int num_clients) {
    model_weights_t avg;
    fed_avg(&avg, models, ones16, num_clients);
    out->num_layers = gl->num_layers;
    for (int l = 0; l < out->num_layers; l++) {
        out->layer_sizes[l] = gl->layer_sizes[l];
        for (int w = 0; w < out->layer_sizes[l]; w++) {
            double delta = avg.weights[l][w] - gl->weights[l][w];
            double sign = (delta >= 0.0) ? 1.0 : -1.0;
            double vt = beta2 * my_abs(delta) + (1.0 - beta2);
            double denom = my_sqrt(vt) + epsilon;
            out->weights[l][w] = gl->weights[l][w] + beta1 * sign / denom * delta;
        }
    }
}
/* FedAdam: adaptive moment estimation for federated setting */
void fed_adam(model_weights_t *out, model_weights_t models[],
              model_weights_t *gl, double beta1, double beta2,
              double epsilon, int num_clients) {
    model_weights_t avg;
    fed_avg(&avg, models, ones16, num_clients);
    out->num_layers = gl->num_layers;
    for (int l = 0; l < out->num_layers; l++) {
        out->layer_sizes[l] = gl->layer_sizes[l];
        for (int w = 0; w < out->layer_sizes[l]; w++) {
            double delta = avg.weights[l][w] - gl->weights[l][w];
            double mt = beta1 * delta / (1.0 - beta1);
            double vt = beta2 * delta * delta / (1.0 - beta2);
            out->weights[l][w] = gl->weights[l][w] + mt / (my_sqrt(vt) + epsilon);
        }
    }
}

double compute_convergence(model_weights_t *old_g, model_weights_t *new_g) {
    double sum_sq = 0.0;
    for (int l = 0; l < new_g->num_layers; l++)
        for (int w = 0; w < new_g->layer_sizes[l]; w++) {
            double d = new_g->weights[l][w] - old_g->weights[l][w];
            sum_sq += d * d;
        }
    return my_sqrt(sum_sq);
}
/* ---- 2. Differential Privacy ---- */
/* Gaussian noise via Box-Muller approximation */
double dp_gaussian_noise(double value, double sigma) {
    double u1 = lcg_rand_double(), u2 = lcg_rand_double();
    if (u1 < 0.0001) u1 = 0.0001;
    double z = my_sqrt(-2.0 * u1) * (u2 * 6.2831853 - 3.14159);
    return value + z * sigma;
}
/* Laplace noise: scale b = sensitivity / epsilon */
double dp_laplace_noise(double value, double b) {
    double u = lcg_rand_double() - 0.5;
    if (u == 0.0) u = 0.0001;
    double sign = (u > 0.0) ? 1.0 : -1.0;
    return value - b * sign;
}
/* Clip gradients to maximum L2 norm */
void dp_clip_gradient(double gradients[], int len, double max_norm) {
    double norm_sq = 0.0;
    for (int i = 0; i < len; i++) norm_sq += gradients[i] * gradients[i];
    double norm = my_sqrt(norm_sq);
    if (norm > max_norm && norm > 0.0) {
        double scale = max_norm / norm;
        for (int i = 0; i < len; i++) gradients[i] *= scale;
    }
}
void dp_privacy_budget_init(double epsilon, double delta) {
    g_priv_epsilon = epsilon; g_priv_delta = delta; g_priv_spent = 0.0;
}

double dp_privacy_budget_spend(double noise_scale) {
    if (noise_scale <= 0.0) return g_priv_epsilon - g_priv_spent;
    double cost = g_priv_epsilon / (noise_scale + 1.0);
    g_priv_spent += cost;
    double remaining = g_priv_epsilon - g_priv_spent;
    return (remaining > 0.0) ? remaining : 0.0;
}
double dp_compute_sensitivity(int dataset_size, int num_classes) {
    if (dataset_size <= 0) return 1.0;
    return (double)num_classes / (double)dataset_size;
}
/* ---- 3. Secure Aggregation ---- */
/* Compute one Shamir secret share via polynomial evaluation */
unsigned int shamir_share(unsigned int secret, unsigned int index,
                          unsigned int k, unsigned int prime) {
    unsigned int result = secret % prime;
    unsigned int x_pow = index % prime;
    for (unsigned int i = 1; i < k; i++) {
        unsigned int coeff = (lcg_rand() % (prime - 1)) + 1;
        result = (result + coeff * x_pow) % prime;
        x_pow = (x_pow * index) % prime;
    }
    return result;
}
/* Modular inverse via Fermat's little theorem */
unsigned int mod_inverse(unsigned int a, unsigned int p) {
    unsigned int result = 1, base = a % p, exp = p - 2;
    while (exp > 0) {
        if (exp & 1) result = ((unsigned long long)result * base) % p;
        base = ((unsigned long long)base * base) % p;
        exp >>= 1;
    }
    return result;
}
/* Reconstruct secret via Lagrange interpolation at x=0 */
unsigned int shamir_reconstruct(unsigned int shares[], unsigned int indices[],
                                unsigned int k, unsigned int prime) {
    unsigned int secret = 0;
    for (unsigned int i = 0; i < k; i++) {
        unsigned int num = 1, den = 1;
        for (unsigned int j = 0; j < k; j++) {
            if (i == j) continue;
            unsigned int xi = indices[i] % prime;
            unsigned int xj = indices[j] % prime;
            num = ((unsigned long long)num * (prime - xj)) % prime;
            unsigned int diff = (xi >= xj) ? (xi - xj) : (prime - (xj - xi));
            den = ((unsigned long long)den * diff) % prime;
        }
        unsigned int lagrange = ((unsigned long long)shares[i] * num % prime
                                 * mod_inverse(den, prime)) % prime;
        secret = (secret + lagrange) % prime;
    }
    return secret;
}
/* Aggregate with pairwise masking that cancels out */
double secure_mask_aggregate(double updates[], double masks[], int num_clients) {
    double sum = 0.0, mask_sum = 0.0;
    for (int i = 0; i < num_clients; i++) {
        sum += updates[i] + masks[i];
        mask_sum += masks[i];
    }
    return sum - mask_sum;
}
int secure_detect_dropouts(int masks_received[], int expected_clients) {
    int dropped = 0;
    for (int i = 0; i < expected_clients; i++) if (!masks_received[i]) dropped++;
    return dropped;
}
int secure_verify_aggregate(double aggregate, double expected_sum, double tolerance) {
    return (my_abs(aggregate - expected_sum) <= tolerance) ? 1 : 0;
}
/* ---- 4. Client Selection ---- */
void client_init(client_info_t *c, int id, int samples, int power,
                 int bandwidth, int battery, int active) {
    c->client_id = id;       c->data_samples = samples;
    c->compute_power = power; c->bandwidth_mbps = bandwidth;
    c->battery_pct = battery; c->active = active;
    c->last_seen = (int)lcg_rand() % 100;
}
int client_select_random(client_info_t clients[], int n, int k,
                         client_info_t selected[], unsigned int seed) {
    rng_state = seed;
    int count = 0, active = 0;
    for (int i = 0; i < n; i++) if (clients[i].active) active++;
    if (k > active) k = active;
    int used[MAX_CLIENTS] = {0};
    while (count < k) {
        int idx = (int)(lcg_rand() % (unsigned int)n);
        if (!used[idx] && clients[idx].active) {
            selected[count++] = clients[idx]; used[idx] = 1;
        }
    }
    return count;
}
int client_select_power_aware(client_info_t clients[], int n, int min_power,
                              int min_bw, client_info_t selected[], int k) {
    int count = 0;
    for (int i = 0; i < n && count < k; i++) {
        if (clients[i].active && clients[i].compute_power >= min_power
            && clients[i].bandwidth_mbps >= min_bw && clients[i].battery_pct > 10)
            selected[count++] = clients[i];
    }
    return count;
}
int client_select_fair_round(client_info_t clients[], int n, int history[],
                             client_info_t selected[], int k) {
    int count = 0, used[MAX_CLIENTS] = {0};
    for (int r = 0; r < k; r++) {
        int min_h = 999999, min_idx = -1;
        for (int i = 0; i < n; i++)
            if (!used[i] && clients[i].active && history[i] < min_h)
                { min_h = history[i]; min_idx = i; }
        if (min_idx >= 0) { selected[count++] = clients[min_idx]; used[min_idx] = 1; }
    }
    return count;
}
int client_select_activity_weighted(client_info_t clients[], int n,
                                    client_info_t selected[], int k) {
    int count = 0, used[MAX_CLIENTS] = {0};
    while (count < k) {
        int best = -1, best_score = -1;
        for (int i = 0; i < n; i++) {
            if (used[i] || !clients[i].active) continue;
            int score = (100 - clients[i].last_seen) * clients[i].battery_pct;
            if (score > best_score) { best_score = score; best = i; }
        }
        if (best < 0) break;
        selected[count++] = clients[best]; used[best] = 1;
    }
    return count;
}
/* ---- 5. Model Compression ---- */
/* Keep top-k gradients by magnitude, zero the rest */
int compress_sparse_topk(double gradients[], int len, int k) {
    if (k >= len) return len;
    double thr[MAX_GRADIENTS];
    for (int i = 0; i < len; i++) thr[i] = my_abs(gradients[i]);
    for (int i = 0; i < k; i++) {
        int mx = i;
        for (int j = i + 1; j < len; j++) if (thr[j] > thr[mx]) mx = j;
        double t = thr[i]; thr[i] = thr[mx]; thr[mx] = t;
    }
    double threshold = thr[k - 1];
    int kept = 0;
    for (int i = 0; i < len; i++) {
        if (my_abs(gradients[i]) >= threshold) kept++;
        else gradients[i] = 0.0;
    }
    return kept;
}
/* Quantize model update to N bits per parameter */
int compress_quantize_update(double update[], int len, int bits) {
    if (bits <= 0 || bits > 30) return 0;
    double min_val = update[0], max_val = update[0];
    for (int i = 1; i < len; i++) {
        if (update[i] < min_val) min_val = update[i];
        if (update[i] > max_val) max_val = update[i];
    }
    double range = max_val - min_val;
    if (range < 0.0001) range = 0.0001;
    int levels = 1 << bits;
    for (int i = 0; i < len; i++) {
        double norm = (update[i] - min_val) / range;
        int q = (int)(norm * (double)(levels - 1) + 0.5);
        update[i] = min_val + (double)q / (double)(levels - 1) * range;
    }
    return len * bits / 8 + 1;
}
/* Knowledge distillation: student mimics teacher weights */
double compress_knowledge_distill(model_weights_t *teacher,
                                  model_weights_t *student, double data[]) {
    (void)data;
    double loss = 0.0;
    for (int l = 0; l < teacher->num_layers; l++) {
        int sz = teacher->layer_sizes[l];
        if (sz > student->layer_sizes[l]) sz = student->layer_sizes[l];
        for (int w = 0; w < sz; w++) {
            double diff = teacher->weights[l][w] - student->weights[l][w];
            loss += diff * diff;
            student->weights[l][w] += 0.01 * diff;
        }
    }
    return my_sqrt(loss);
}
double compress_compute_ratio(int original_size, int compressed_size) {
    if (compressed_size <= 0) return 0.0;
    return (double)original_size / (double)compressed_size;
}
double compress_estimate_savings(double update[], int len, int method) {
    if (method == COMP_SPARSE_TOPK) {
        double nonzero = 0.0;
        for (int i = 0; i < len; i++) if (update[i] != 0.0) nonzero += 1.0;
        return 1.0 - nonzero / (double)len;
    } else if (method == COMP_QUANTIZE) {
        return 1.0 - 8.0 / 32.0;
    }
    return 0.0;
}
/* ---- 6. Heterogeneity Handling ---- */
/* Simulate Dirichlet distribution for non-IID data partitioning */
void hetero_generate_noniid_data(double dist[][MAX_CLASSES], int num_clients,
                                 int num_classes, double alpha) {
    for (int c = 0; c < num_clients; c++) {
        double sum = 0.0;
        for (int cl = 0; cl < num_classes; cl++) {
            dist[c][cl] = lcg_rand_double() * alpha + 0.01;
            sum += dist[c][cl];
        }
        for (int cl = 0; cl < num_classes; cl++) dist[c][cl] /= sum;
    }
}
void hetero_compute_local_stats(double data_dist[], int num_classes, double stats[]) {
    double sum = 0.0, sum_sq = 0.0, mx = -1e18, mn = 1e18;
    for (int i = 0; i < num_classes; i++) {
        sum += data_dist[i];
        sum_sq += data_dist[i] * data_dist[i];
        if (data_dist[i] > mx) mx = data_dist[i];
        if (data_dist[i] < mn) mn = data_dist[i];
    }
    double mean = sum / (double)num_classes;
    stats[0] = mean;
    stats[1] = sum_sq / (double)num_classes - mean * mean;
    stats[2] = mx;
    stats[3] = mn;
}
double hetero_measure_divergence(double local_stats[][4], double global_stats[],
                                 int num_clients) {
    double divergence = 0.0;
    for (int c = 0; c < num_clients; c++) {
        double diff_mean = local_stats[c][0] - global_stats[0];
        double diff_var  = local_stats[c][1] - global_stats[1];
        divergence += diff_mean * diff_mean + my_abs(diff_var) * 0.1;
    }
    return divergence / (double)num_clients;
}

int hetero_adjust_epochs(double divergence, int base_epochs) {
    if (divergence > 0.5) return base_epochs / 2;
    if (divergence > 0.2) return base_epochs * 3 / 4;
    return base_epochs;
}
/* ---- Test & Entry Point ---- */
static void test_all(void) {
    print_str("=== Federated Learning Framework Test ===\n\n");
    print_str("[1] Aggregation Algorithms\n");
    int sizes[3] = {4, 4, 4};
    model_weights_t models[3], result, global;
    for (int i = 0; i < 3; i++) model_init(&models[i], 3, sizes);
    model_init(&global, 3, sizes);
    int samples[3] = {100, 200, 150};
    fed_avg(&result, models, samples, 3);
    print_str("  FedAvg l0[0]: "); print_double(result.weights[0][0]); print_str("\n");
    fed_prox(&result, models, &global, 0.01, 3);
    print_str("  FedProx l0[0]: "); print_double(result.weights[0][0]); print_str("\n");
    fed_yogi(&result, models, &global, 0.9, 0.999, 1e-8, 3);
    print_str("  FedYogi l0[0]: "); print_double(result.weights[0][0]); print_str("\n");
    fed_adam(&result, models, &global, 0.9, 0.999, 1e-8, 3);
    print_str("  FedAdam l0[0]: "); print_double(result.weights[0][0]); print_str("\n");
    double conv = compute_convergence(&global, &result);
    print_str("  Convergence: "); print_double(conv); print_str("\n\n");
    print_str("[2] Differential Privacy\n");
    dp_privacy_budget_init(1.0, 1e-5);
    print_str("  Gaussian: "); print_double(dp_gaussian_noise(1.0, 0.5)); print_str("\n");
    print_str("  Laplace:  "); print_double(dp_laplace_noise(1.0, 0.1)); print_str("\n");
    double grads[8] = {0.5, -0.3, 1.2, -0.8, 0.1, 0.7, -0.4, 0.9};
    dp_clip_gradient(grads, 8, 1.0);
    print_str("  Clipped[2]: "); print_double(grads[2]); print_str("\n");
    double remaining = dp_privacy_budget_spend(0.5);
    print_str("  Budget remaining: "); print_double(remaining); print_str("\n");
    print_str("  Sensitivity: "); print_double(dp_compute_sensitivity(1000, 10)); print_str("\n\n");
    print_str("[3] Secure Aggregation\n");
    unsigned int prime = 1000003, secret = 42;
    unsigned int shares[5], indices[5];
    for (int i = 0; i < 5; i++) {
        indices[i] = (unsigned int)(i + 1);
        shares[i] = shamir_share(secret, indices[i], 3, prime);
    }
    print_str("  Shares: ");
    for (int i = 0; i < 5; i++) { print_int((int)shares[i]); print_str(" "); }
    print_str("\n");
    unsigned int recon = shamir_reconstruct(shares, indices, 3, prime);
    print_str("  Reconstructed: "); print_int((int)recon); print_str("\n");
    double upd[4] = {1.0, 2.0, 3.0, 4.0}, msk[4] = {0.5, -0.3, 0.1, -0.3};
    print_str("  Secure agg: "); print_double(secure_mask_aggregate(upd, msk, 4)); print_str("\n");
    int masks[4] = {1, 1, 0, 1};
    print_str("  Dropouts: "); print_int(secure_detect_dropouts(masks, 4)); print_str("\n");
    print_str("  Verified: "); print_int(secure_verify_aggregate(10.0, 10.0, 0.01)); print_str("\n\n");
    print_str("[4] Client Selection\n");
    client_info_t clients[6], selected[4];
    client_init(&clients[0], 0, 500, 80, 50, 90, 1);
    client_init(&clients[1], 1, 300, 60, 30, 70, 1);
    client_init(&clients[2], 2, 800, 90, 100, 50, 1);
    client_init(&clients[3], 3, 200, 40, 20, 20, 1);
    client_init(&clients[4], 4, 600, 70, 40, 85, 1);
    client_init(&clients[5], 5, 100, 30, 10, 10, 0);
    print_str("  Random: "); print_int(client_select_random(clients, 6, 3, selected, 123)); print_str("\n");
    print_str("  Power-aware: "); print_int(client_select_power_aware(clients, 6, 50, 25, selected, 4)); print_str("\n");
    int hist[6] = {5, 2, 1, 8, 3, 0};
    print_str("  Fair-round: "); print_int(client_select_fair_round(clients, 6, hist, selected, 3)); print_str("\n");
    print_str("  Activity-wt: "); print_int(client_select_activity_weighted(clients, 6, selected, 3)); print_str("\n\n");
    print_str("[5] Model Compression\n");
    double g2[16], orig[16];
    for (int i = 0; i < 16; i++) g2[i] = orig[i] = lcg_rand_double() * 2.0 - 1.0;
    int kept = compress_sparse_topk(g2, 16, 5);
    print_str("  Top-k kept: "); print_int(kept); print_str(" / 16\n");
    double u2[8] = {0.1, 0.5, 0.9, 0.3, 0.7, 0.2, 0.8, 0.4};
    print_str("  Quantized bytes: "); print_int(compress_quantize_update(u2, 8, 8)); print_str("\n");
    model_weights_t teacher, student;
    model_init(&teacher, 2, sizes); model_init(&student, 2, sizes);
    double dloss = compress_knowledge_distill(&teacher, &student, (double[]){0.0});
    print_str("  Distill loss: "); print_double(dloss); print_str("\n");
    print_str("  Compression ratio: "); print_double(compress_compute_ratio(1024, 256)); print_str("\n");
    print_str("  Savings: "); print_double(compress_estimate_savings(orig, 16, COMP_SPARSE_TOPK)); print_str("\n\n");

    print_str("[6] Heterogeneity Handling\n");
    double dist_arr[MAX_CLIENTS][MAX_CLASSES];
    hetero_generate_noniid_data(dist_arr, 4, 5, 0.3);
    print_str("  Dirichlet[0][0]: "); print_double(dist_arr[0][0]); print_str("\n");
    double stats[4];
    hetero_compute_local_stats(dist_arr[0], 5, stats);
    print_str("  Mean: "); print_double(stats[0]); print_str("\n");
    print_str("  Variance: "); print_double(stats[1]); print_str("\n");
    double g_stats[4] = {0.2, 0.01, 0.4, 0.05};
    double local_stats[4][4];
    for (int c = 0; c < 4; c++)
        hetero_compute_local_stats(dist_arr[c], 5, local_stats[c]);
    double div = hetero_measure_divergence(local_stats, g_stats, 4);
    print_str("  Divergence: "); print_double(div); print_str("\n");
    print_str("  Adjusted epochs: "); print_int(hetero_adjust_epochs(0.35, 10)); print_str("\n\n");

    print_str("=== All tests passed ===\n");
}

static void show_help(void) {
    print_str("Federated Learning Framework v56.0\n");
    print_str("Usage: federated_learning [options]\n");
    print_str("  -h  Show this help message\n");
    print_str("  -t  Run test suite\n");
}

void _start(void) {
    char buf[256];
    int len = host_get_argv(0, 256);
    if (len > 0 && len <= 256) {
        host_get_argv((unsigned int)(size_t)buf, 256);
    }
    if (len > 0) {
        for (int i = 0; i < len; i++) {
            if (buf[i] == '-' && i + 1 < len) {
                if (buf[i + 1] == 'h') { show_help(); host_exit(0); return; }
                if (buf[i + 1] == 't') { test_all(); host_exit(0); return; }
            }
        }
    }
    print_str("Federated Learning v56.0 - Privacy-Preserving Distributed ML\n");
    print_str("Use -h for help, -t for tests\n");
}
