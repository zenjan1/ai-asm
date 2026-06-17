/* mri_reconstruction: MRI signal reconstruction (v1.0)
 * k-space, Fourier reconstruction, pulse sequences, relaxation, artifacts
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

#define MAX_DIM    32
#define MAX_COILS  4
#define MAX_PE     64
#define MAX_RO     64
#define MAX_SLICES 8

#define SEQ_SE     0x01
#define SEQ_GRE    0x02
#define SEQ_EPI    0x03
#define SEQ_FSE    0x04

#define KSP_CARTESIAN 0x01
#define KSP_RADIAL    0x02
#define KSP_SPIRAL    0x03

#define PAR_NONE    0x00
#define PAR_SENSE   0x01
#define PAR_GRAPPA  0x02

typedef struct {
    double re[MAX_RO][MAX_PE];
    double im[MAX_RO][MAX_PE];
    int    ro_samples;
    int    pe_lines;
    int    fully_sampled;
    int    acceleration;
} kspace_t;

typedef struct {
    double data[MAX_DIM][MAX_DIM];
    int    width, height;
    double resolution;
    double fov;
    int    active;
} mri_image_t;

typedef struct {
    int    seq_type;
    double TR, TE;
    double flip_angle;
    double BW;
    double T1, T2;
    double Mz, Mxy;
    double SNR;
    double CNR;
    int    computed;
} pulse_sequence_t;

typedef struct {
    double coil_data[MAX_COILS][MAX_DIM][MAX_DIM];
    double sensitivity[MAX_COILS][MAX_DIM][MAX_DIM];
    int    coil_count;
    int    acceleration;
    int    method;
    double g_factor;
    double SNR_loss;
} parallel_imaging_t;

typedef struct {
    double magnitude[MAX_DIM][MAX_DIM];
    double phase[MAX_DIM][MAX_DIM];
    int    width, height;
    int    active;
} complex_image_t;

typedef struct {
    int    seq_count;
    int    kspace_count;
    int    image_count;
    double T1_val, T2_val;
    double motion_x, motion_y;
    int    motion_corrected;
    int    artifact_level;
} mri_state_t;

static kspace_t kspaces[4];
static mri_image_t mri_images[MAX_DIM];
static pulse_sequence_t sequences[4];
static parallel_imaging_t par;
static complex_image_t complex_img;
static mri_state_t mri;

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
static double dsin(double x) {
    double sum = x, term = x, x2 = x * x;
    for (int i = 1; i < 15; i++) { term *= -x2 / ((2*i)*(2*i+1)); sum += term; }
    return sum;
}
static double dexp(double x) {
    if (x > 20.0) return 5e8;
    if (x < -20.0) return 0.0;
    double sum = 1.0, term = 1.0;
    for (int i = 1; i < 30; i++) { term *= x / i; sum += term; }
    return sum;
}

int mri_init(void) {
    if (initialized) return -1;
    mri.seq_count = 0; mri.kspace_count = 0; mri.image_count = 0;
    mri.T1_val = 1000.0; mri.T2_val = 100.0;
    mri.motion_x = 0.0; mri.motion_y = 0.0;
    mri.motion_corrected = 0; mri.artifact_level = 0;
    par.coil_count = 0; par.acceleration = 1; par.method = PAR_NONE;
    par.g_factor = 1.0; par.SNR_loss = 0.0;
    complex_img.active = 0;
    for (int i = 0; i < 4; i++) {
        kspaces[i].ro_samples = 0; kspaces[i].pe_lines = 0;
        kspaces[i].fully_sampled = 1; kspaces[i].acceleration = 1;
    }
    for (int i = 0; i < MAX_DIM; i++) mri_images[i].active = 0;
    initialized = 1;
    print_str("[MRI] MRI reconstruction module initialized\n");
    return 0;
}

int mri_create_kspace(int ro, int pe, int trajectory) {
    if (mri.kspace_count >= 4) return -1;
    kspace_t* k = &kspaces[mri.kspace_count];
    k->ro_samples = ro < MAX_RO ? ro : MAX_RO;
    k->pe_lines = pe < MAX_PE ? pe : MAX_PE;
    k->fully_sampled = 1; k->acceleration = 1;
    double cx = k->ro_samples * 0.5, cy = k->pe_lines * 0.5;
    for (int pe_i = 0; pe_i < k->pe_lines; pe_i++) {
        for (int ro_i = 0; ro_i < k->ro_samples; ro_i++) {
            double kx = (ro_i - cx) / k->ro_samples;
            double ky = (pe_i - cy) / k->pe_lines;
            double kr = dsqrt(kx*kx + ky*ky);
            double signal = dexp(-kr * kr * 20.0) * 100.0;
            signal += 20.0 * dcos(kx * 20.0) * dcos(ky * 20.0);
            k->re[ro_i][pe_i] = signal * dcos(kr * 3.14159 * 2.0);
            k->im[ro_i][pe_i] = signal * dsin(kr * 3.14159 * 2.0);
            if (trajectory == KSP_RADIAL) {
                if (pe_i % 3 != 0) { k->re[ro_i][pe_i] *= 0.1; k->im[ro_i][pe_i] *= 0.1; }
                k->fully_sampled = 0; k->acceleration = 3;
            } else if (trajectory == KSP_SPIRAL) {
                if ((ro_i + pe_i) % 2 != 0) { k->re[ro_i][pe_i] *= 0.3; k->im[ro_i][pe_i] *= 0.3; }
                k->fully_sampled = 0; k->acceleration = 2;
            }
        }
    }
    return mri.kspace_count++;
}

void mri_fft_reconstruct(int kspace_id) {
    if (kspace_id >= mri.kspace_count) return;
    if (mri.image_count >= MAX_DIM) return;
    kspace_t* k = &kspaces[kspace_id];
    mri_image_t* im = &mri_images[mri.image_count];
    im->width = k->ro_samples < MAX_DIM ? k->ro_samples : MAX_DIM;
    im->height = k->pe_lines < MAX_DIM ? k->pe_lines : MAX_DIM;
    im->resolution = 1.0; im->fov = im->width;
    for (int y = 0; y < im->height; y++) {
        for (int x = 0; x < im->width; x++) {
            double sum_re = 0.0, sum_im = 0.0;
            for (int ky = 0; ky < im->height; ky++) {
                for (int kx = 0; kx < im->width; kx++) {
                    double phase_re = dcos(2.0 * 3.14159 * (x * kx + y * ky) / im->width);
                    double phase_im = dsin(2.0 * 3.14159 * (x * kx + y * ky) / im->width);
                    sum_re += k->re[kx][ky] * phase_re - k->im[kx][ky] * phase_im;
                    sum_im += k->re[kx][ky] * phase_im + k->im[kx][ky] * phase_re;
                }
            }
            im->data[x][y] = dsqrt(sum_re * sum_re + sum_im * sum_im) / (im->width * im->height);
        }
    }
    im->active = 1;
    print_str("[MRI] FFT reconstruction from kspace "); print_int(kspace_id);
    print_str(" ("); print_int(im->width); print_str("x"); print_int(im->height); print_str(")\n");
    mri.image_count++;
}

void mri_pulse_sequence(int type, double TR, double TE, double T1, double T2) {
    if (mri.seq_count >= 4) return;
    pulse_sequence_t* s = &sequences[mri.seq_count];
    s->seq_type = type; s->TR = TR; s->TE = TE;
    s->T1 = T1; s->T2 = T2;
    s->flip_angle = (type == SEQ_GRE) ? 30.0 : 90.0;
    s->BW = 1.0 / (TE * 0.001 + 1e-10);
    double M0 = 1.0;
    s->Mz = M0 * (1.0 - dexp(-TR / T1));
    s->Mxy = s->Mz * dsin(s->flip_angle * 3.14159 / 180.0) * dexp(-TE / T2);
    double noise = 0.05;
    s->SNR = dabs(s->Mxy) / noise;
    s->CNR = s->SNR * 0.8;
    s->computed = 1;
    mri.seq_count++;
    print_str("[MRI] Pulse sequence type="); print_int(type);
    print_str(" TR="); print_int((int)TR); print_str("ms TE="); print_int((int)TE); print_str("ms\n");
    print_str("  T1="); print_int((int)T1); print_str("ms T2="); print_int((int)T2); print_str("ms");
    print_str(" Mxy="); print_int((int)(s->Mxy * 1000));
    print_str(" SNR="); print_int((int)(s->SNR * 10)); print_str("\n");
}

void mri_relaxation_curve(double T1, double T2, int n_points) {
    mri.T1_val = T1; mri.T2_val = T2;
    print_str("[MRI] Relaxation curves (T1="); print_int((int)T1);
    print_str(" T2="); print_int((int)T2); print_str("):\n");
    print_str("  t(ms)   Mz       Mxy\n");
    for (int i = 0; i < n_points && i < 10; i++) {
        double t = (i + 1) * 100.0;
        double Mz = 1.0 - dexp(-t / T1);
        double Mxy = dexp(-t / T2);
        print_str("  "); print_int((int)t);
        print_str("   "); print_int((int)(Mz * 1000));
        print_str("   "); print_int((int)(Mxy * 1000)); print_str("\n");
    }
}

void mri_parallel_imaging(int coils, int acceleration, int method) {
    par.coil_count = coils < MAX_COILS ? coils : MAX_COILS;
    par.acceleration = acceleration;
    par.method = method;
    for (int c = 0; c < par.coil_count; c++) {
        for (int y = 0; y < MAX_DIM; y++) {
            for (int x = 0; x < MAX_DIM; x++) {
                double cx = MAX_DIM * 0.5 + (c - 1) * 5.0;
                double cy = MAX_DIM * 0.5;
                double r = dsqrt((x - cx) * (x - cx) + (y - cy) * (y - cy));
                par.sensitivity[c][x][y] = dexp(-r * r * 0.005);
                par.coil_data[c][x][y] = par.sensitivity[c][x][y] * 100.0;
            }
        }
    }
    par.g_factor = 1.0 + (acceleration - 1) * 0.15;
    par.SNR_loss = 1.0 / (dsqrt((double)acceleration) * par.g_factor);
    print_str("[MRI] Parallel imaging coils="); print_int(coils);
    print_str(" R="); print_int(acceleration);
    print_str(" method="); print_int(method); print_str("\n");
    print_str("  g-factor="); print_int((int)(par.g_factor * 1000));
    print_str(" SNR_loss="); print_int((int)(par.SNR_loss * 100)); print_str("%\n");
}

void mri_motion_correction(double mx, double my) {
    mri.motion_x = mx; mri.motion_y = my;
    mri.motion_corrected = 1;
    if (mri.image_count == 0) return;
    mri_image_t* im = &mri_images[mri.image_count - 1];
    int shift_x = (int)mx;
    int shift_y = (int)my;
    double temp[MAX_DIM][MAX_DIM];
    for (int y = 0; y < im->height; y++)
        for (int x = 0; x < im->width; x++) temp[x][y] = 0.0;
    for (int y = 0; y < im->height; y++) {
        for (int x = 0; x < im->width; x++) {
            int sx = x - shift_x, sy = y - shift_y;
            if (sx >= 0 && sx < im->width && sy >= 0 && sy < im->height)
                temp[x][y] = im->data[sx][sy];
        }
    }
    for (int y = 0; y < im->height; y++)
        for (int x = 0; x < im->width; x++)
            im->data[x][y] = temp[x][y];
    print_str("[MRI] Motion correction: dx="); print_int((int)(mx * 10));
    print_str(" dy="); print_int((int)(my * 10)); print_str(" pixels\n");
}

void mri_artifact_assessment(void) {
    mri.artifact_level = 0;
    if (dabs(mri.motion_x) + dabs(mri.motion_y) > 2.0) mri.artifact_level += 2;
    else if (dabs(mri.motion_x) + dabs(mri.motion_y) > 0.5) mri.artifact_level += 1;
    if (par.acceleration > 2) mri.artifact_level += 1;
    print_str("[MRI] Artifact assessment: level="); print_int(mri.artifact_level); print_str("/3\n");
}

void mri_print_state(void) {
    print_str("[MRI] Seqs="); print_int(mri.seq_count);
    print_str(" Ksp="); print_int(mri.kspace_count);
    print_str(" Imgs="); print_int(mri.image_count);
    print_str(" Coils="); print_int(par.coil_count);
    print_str(" Motion_corr="); print_int(mri.motion_corrected);
    print_str("\n");
}

int main(void) {
    print_str("=== MRI Reconstruction Demo ===\n\n");
    mri_init();

    print_str("Creating Cartesian k-space...\n");
    mri_create_kspace(24, 24, KSP_CARTESIAN);

    print_str("\nCreating radial k-space (R=3)...\n");
    mri_create_kspace(24, 24, KSP_RADIAL);

    print_str("\nCreating spiral k-space (R=2)...\n");
    mri_create_kspace(24, 24, KSP_SPIRAL);

    print_str("\nFFT reconstruction from Cartesian...\n");
    mri_fft_reconstruct(0);

    print_str("\nPulse sequences...\n");
    mri_pulse_sequence(SEQ_SE, 2000.0, 80.0, 1000.0, 100.0);
    mri_pulse_sequence(SEQ_GRE, 50.0, 5.0, 1000.0, 100.0);
    mri_pulse_sequence(SEQ_EPI, 2000.0, 50.0, 1000.0, 80.0);

    print_str("\nT1/T2 relaxation curves...\n");
    mri_relaxation_curve(1000.0, 100.0, 10);

    print_str("\nParallel imaging (SENSE R=2)...\n");
    mri_parallel_imaging(4, 2, PAR_SENSE);

    print_str("\nParallel imaging (GRAPPA R=3)...\n");
    mri_parallel_imaging(4, 3, PAR_GRAPPA);

    print_str("\nMotion correction...\n");
    mri_motion_correction(2.5, -1.0);

    print_str("\nArtifact assessment...\n");
    mri_artifact_assessment();

    mri_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
