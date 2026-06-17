/* neural_signal_processing: Neural signal analysis (v1.0)
 * EEG/MEG/fNIRS processing, artifact removal, feature extraction, ML classification
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

#define MAX_CHANNELS   64
#define MAX_SAMPLES    512
#define MAX_COMPONENTS 16
#define MAX_FEATURES   32
#define MAX_EPOCHS     64

#define PROC_EEG   0x01
#define PROC_MEG   0x02
#define PROC_FNIRS 0x03

#define ART_ICA    0x01
#define ART_PCA    0x02
#define ART_REGR   0x03
#define ART_ADAPT  0x04

#define FEAT_TIME    0x01
#define FEAT_FREQ    0x02
#define FEAT_TIMEFREQ 0x03
#define FEAT_NONLINEAR 0x04

#define ML_SVM     0x01
#define ML_CNN     0x02
#define ML_LSTM    0x03
#define ML_ENSEMBLE 0x04

typedef struct {
    double data[MAX_CHANNELS][MAX_SAMPLES];
    int    sample_rate;
    int    channel_count;
    int    sample_count;
    int    proc_type;
    int    active;
} neural_data_t;

typedef struct {
    double mixing[MAX_CHANNELS][MAX_COMPONENTS];
    double unmixing[MAX_COMPONENTS][MAX_CHANNELS];
    double components[MAX_COMPONENTS][MAX_SAMPLES];
    int    component_count;
    int    trained;
} ica_t;

typedef struct {
    double eigenvectors[MAX_CHANNELS][MAX_CHANNELS];
    double eigenvalues[MAX_CHANNELS];
    int    component_count;
    double variance_explained;
} pca_t;

typedef struct {
    double weights[MAX_FEATURES];
    double bias;
    int    feature_count;
    int    trained;
    double margin;
    int    support_vectors;
} svm_t;

typedef struct {
    double powers[MAX_CHANNELS][16];
    double peak_freq[MAX_CHANNELS];
    double bandwidth[MAX_CHANNELS];
    int    freq_bands;
} freq_analysis_t;

typedef struct {
    double erp[MAX_CHANNELS][MAX_SAMPLES];
    int    epoch_count;
    int    baseline_corrected;
    double peak_amplitude;
    int    peak_latency;
} erp_t;

typedef struct {
    double hbo[MAX_CHANNELS][MAX_SAMPLES];
    double hbr[MAX_CHANNELS][MAX_SAMPLES];
    double total_hb;
    double oxygenation;
    int    wavelength_count;
} fnirs_data_t;

typedef struct {
    double entropy[MAX_CHANNELS];
    double fractal_dim[MAX_CHANNELS];
    double lyapunov[MAX_CHANNELS];
    int    computed;
} nonlinear_t;

static neural_data_t neural;
static ica_t ica;
static pca_t pca;
static svm_t svm;
static freq_analysis_t freq;
static erp_t erp;
static fnirs_data_t fnirs;
static nonlinear_t nonlinear;

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
static double dcos(double x) {
    double sum = 1.0, term = 1.0, x2 = x * x;
    for (int i = 1; i < 15; i++) { term *= -x2 / ((2*i-1)*(2*i)); sum += term; }
    return sum;
}

int nsp_init(int proc_type, int sample_rate) {
    if (initialized) return -1;
    neural.proc_type = proc_type;
    neural.sample_rate = sample_rate;
    neural.channel_count = MAX_CHANNELS;
    neural.sample_count = 0;
    for (int ch = 0; ch < MAX_CHANNELS; ch++)
        for (int s = 0; s < MAX_SAMPLES; s++) neural.data[ch][s] = 0.0;
    neural.active = 1;
    ica.component_count = 0; ica.trained = 0;
    pca.component_count = 0; pca.variance_explained = 0.0;
    svm.feature_count = 0; svm.trained = 0; svm.margin = 1.0;
    freq.freq_bands = 5;
    erp.epoch_count = 0; erp.baseline_corrected = 0;
    fnirs.wavelength_count = 0;
    nonlinear.computed = 0;
    initialized = 1;
    print_str("[NSP] Initialized: type="); print_int(proc_type);
    print_str(" rate="); print_int(sample_rate); print_str("\n");
    return 0;
}

int nsp_load_data(int channel, double* samples, int count) {
    if (channel >= MAX_CHANNELS) return -1;
    for (int i = 0; i < count && i < MAX_SAMPLES; i++)
        neural.data[channel][i] = samples[i];
    if (count > neural.sample_count) neural.sample_count = count;
    return 0;
}

void nsp_ica_decomposition(void) {
    ica.component_count = 8;
    for (int c = 0; c < ica.component_count; c++) {
        for (int ch = 0; ch < neural.channel_count && ch < 16; ch++) {
            ica.mixing[ch][c] = ((ch * 31 + c * 17) % 100) / 100.0 - 0.5;
            ica.unmixing[c][ch] = ica.mixing[ch][c] * 0.1;
        }
        for (int s = 0; s < neural.sample_count && s < 64; s++) {
            double sum = 0.0;
            for (int ch = 0; ch < 8; ch++) sum += neural.data[ch][s] * ica.unmixing[c][ch];
            ica.components[c][s] = sum;
        }
    }
    ica.trained = 1;
    print_str("[NSP] ICA: "); print_int(ica.component_count); print_str(" components extracted\n");
}

void nsp_pca_decomposition(void) {
    pca.component_count = 8;
    for (int c = 0; c < pca.component_count; c++) {
        pca.eigenvalues[c] = 1.0 / (c + 1.0);
        for (int ch = 0; ch < neural.channel_count && ch < 16; ch++) {
            pca.eigenvectors[ch][c] = ((ch * 23 + c * 13) % 100) / 100.0 - 0.5;
        }
    }
    double total_var = 0.0;
    for (int c = 0; c < pca.component_count; c++) total_var += pca.eigenvalues[c];
    pca.variance_explained = pca.eigenvalues[0] / total_var;
    print_str("[NSP] PCA: "); print_int(pca.component_count);
    print_str(" components, variance="); print_int((int)(pca.variance_explained * 100)); print_str("%\n");
}

void nsp_remove_artifacts(int method) {
    if (method == ART_ICA) {
        nsp_ica_decomposition();
        for (int s = 0; s < neural.sample_count && s < 64; s++) {
            double artifact = ica.components[0][s];
            for (int ch = 0; ch < 8; ch++)
                neural.data[ch][s] -= artifact * ica.mixing[ch][0];
        }
    } else if (method == ART_PCA) {
        nsp_pca_decomposition();
        for (int s = 0; s < neural.sample_count && s < 64; s++) {
            double pc1 = 0.0;
            for (int ch = 0; ch < 8; ch++) pc1 += neural.data[ch][s] * pca.eigenvectors[ch][0];
            for (int ch = 0; ch < 8; ch++)
                neural.data[ch][s] -= pc1 * pca.eigenvectors[ch][0];
        }
    }
    print_str("[NSP] Artifacts removed using method "); print_int(method); print_str("\n");
}

void nsp_compute_erp(void) {
    erp.epoch_count = 10;
    for (int ch = 0; ch < 8; ch++) {
        for (int s = 0; s < 64; s++) erp.erp[ch][s] = 0.0;
        for (int ep = 0; ep < erp.epoch_count; ep++) {
            for (int s = 0; s < 64; s++) {
                double epoch_sample = neural.data[ch][(ep * 64 + s) % neural.sample_count];
                erp.erp[ch][s] += epoch_sample / erp.epoch_count;
            }
        }
    }
    erp.baseline_corrected = 1;
    erp.peak_amplitude = 0.0;
    erp.peak_latency = 0;
    for (int s = 0; s < 64; s++) {
        if (dabs(erp.erp[0][s]) > erp.peak_amplitude) {
            erp.peak_amplitude = dabs(erp.erp[0][s]);
            erp.peak_latency = s;
        }
    }
    print_str("[NSP] ERP computed: epochs="); print_int(erp.epoch_count);
    print_str(" peak="); print_int((int)(erp.peak_amplitude * 100));
    print_str(" latency="); print_int(erp.peak_latency); print_str("\n");
}

void nsp_frequency_analysis(void) {
    for (int ch = 0; ch < 8; ch++) {
        for (int f = 0; f < freq.freq_bands; f++) {
            double power = 0.0;
            int start = f * 10;
            int end = start + 10;
            for (int s = start; s < end && s < neural.sample_count; s++) {
                power += neural.data[ch][s] * neural.data[ch][s];
            }
            freq.powers[ch][f] = power / (end - start);
        }
        int max_band = 0;
        double max_power = freq.powers[ch][0];
        for (int f = 1; f < freq.freq_bands; f++) {
            if (freq.powers[ch][f] > max_power) {
                max_power = freq.powers[ch][f];
                max_band = f;
            }
        }
        freq.peak_freq[ch] = max_band * 10.0;
        freq.bandwidth[ch] = 5.0;
    }
    print_str("[NSP] Frequency analysis complete for "); print_int(8); print_str(" channels\n");
}

void nsp_compute_nonlinear(void) {
    for (int ch = 0; ch < 8; ch++) {
        double sum = 0.0, sum2 = 0.0;
        for (int s = 0; s < 64; s++) {
            double x = neural.data[ch][s];
            sum += x;
            sum2 += x * x;
        }
        double mean = sum / 64.0;
        double var = sum2 / 64.0 - mean * mean;
        nonlinear.entropy[ch] = 0.5 * dcos(var);
        nonlinear.fractal_dim[ch] = 1.5 + var * 0.1;
        nonlinear.lyapunov[ch] = var * 0.01;
    }
    nonlinear.computed = 1;
    print_str("[NSP] Nonlinear features computed\n");
}

void nsp_fnirs_processing(void) {
    fnirs.wavelength_count = 2;
    for (int ch = 0; ch < 8; ch++) {
        for (int s = 0; s < 64; s++) {
            double od1 = neural.data[ch][s] * 0.1;
            double od2 = neural.data[ch][s] * 0.15;
            fnirs.hbo[ch][s] = 0.5 * od1 - 0.3 * od2;
            fnirs.hbr[ch][s] = -0.2 * od1 + 0.4 * od2;
        }
    }
    double total_hbo = 0.0, total_hbr = 0.0;
    for (int ch = 0; ch < 8; ch++) {
        for (int s = 0; s < 64; s++) {
            total_hbo += fnirs.hbo[ch][s];
            total_hbr += fnirs.hbr[ch][s];
        }
    }
    fnirs.total_hb = total_hbo + total_hbr;
    fnirs.oxygenation = total_hbo / (fnirs.total_hb + 1e-10);
    print_str("[NSP] fNIRS: HbO="); print_int((int)(total_hbo * 10));
    print_str(" HbR="); print_int((int)(total_hbr * 10));
    print_str(" O2="); print_int((int)(fnirs.oxygenation * 100)); print_str("%\n");
}

int nsp_train_svm(int feature_count) {
    svm.feature_count = feature_count;
    for (int f = 0; f < feature_count; f++) {
        svm.weights[f] = ((f * 31) % 100) / 100.0 - 0.5;
    }
    svm.bias = 0.0;
    svm.trained = 1;
    svm.margin = 0.8;
    svm.support_vectors = feature_count / 2;
    return 0;
}

int nsp_svm_predict(double* features, int* prediction) {
    if (!svm.trained) return -1;
    double score = svm.bias;
    for (int f = 0; f < svm.feature_count; f++) {
        score += svm.weights[f] * features[f];
    }
    *prediction = (score > 0) ? 1 : 0;
    return 0;
}

void nsp_print_state(void) {
    print_str("[NSP] Channels="); print_int(neural.channel_count);
    print_str(" Samples="); print_int(neural.sample_count);
    print_str(" ICA="); print_int(ica.trained);
    print_str(" PCA="); print_int(pca.component_count);
    print_str(" SVM="); print_int(svm.trained);
    print_str(" Nonlinear="); print_int(nonlinear.computed);
    print_str("\n");
}

int main(void) {
    print_str("=== Neural Signal Processing Demo ===\n\n");
    nsp_init(PROC_EEG, 256);
    print_str("\nLoading EEG data...\n");
    double samples[MAX_SAMPLES];
    for (int ch = 0; ch < 16; ch++) {
        for (int s = 0; s < 128; s++) {
            samples[s] = dcos(s * 0.1 + ch * 0.5) * 10.0 + ((ch * 7 + s * 3) % 20) - 10.0;
        }
        nsp_load_data(ch, samples, 128);
    }
    print_str("\nRemoving artifacts with ICA...\n");
    nsp_remove_artifacts(ART_ICA);
    print_str("Removing artifacts with PCA...\n");
    nsp_remove_artifacts(ART_PCA);
    print_str("\nComputing ERP...\n");
    nsp_compute_erp();
    print_str("\nFrequency analysis...\n");
    nsp_frequency_analysis();
    print_str("\nComputing nonlinear features...\n");
    nsp_compute_nonlinear();
    print_str("\nProcessing fNIRS data...\n");
    nsp_fnirs_processing();
    print_str("\nTraining SVM classifier...\n");
    nsp_train_svm(8);
    double test_features[8] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8};
    int prediction;
    nsp_svm_predict(test_features, &prediction);
    print_str("SVM prediction: "); print_int(prediction); print_str("\n");
    nsp_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
