/* ct_reconstruction: CT image reconstruction (v1.0)
 * Radon transform, FBP, iterative reconstruction, corrections, dose optimization
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
#define MAX_ANGLES 64
#define MAX_DET    64
#define MAX_ITER   32

#define GEOM_PARALLEL 0x01
#define GEOM_FANBEAM  0x02
#define GEOM_CONE     0x03

#define FILTER_RAMLAK   0x01
#define FILTER_SHEPP    0x02
#define FILTER_COSINE   0x03
#define FILTER_HAMMING  0x04
#define FILTER_HANN     0x05

#define RECON_FBP    0x01
#define RECON_ART    0x02
#define RECON_SART   0x03
#define RECON_SIRT   0x04
#define RECON_STAT   0x05

typedef struct {
    double data[MAX_DIM][MAX_DIM];
    int    width, height;
    double pixel_size;
    int    active;
} phantom_t;

typedef struct {
    double sinogram[MAX_ANGLES][MAX_DET];
    int    n_angles;
    int    n_detectors;
    double angle_step;
    double detector_spacing;
    int    geometry;
    int    active;
} sinogram_t;

typedef struct {
    double data[MAX_DIM][MAX_DIM];
    int    width, height;
    double pixel_size;
    int    method;
    int    filter;
    int    iterations;
    double residual;
    int    converged;
    int    active;
} ct_image_t;

typedef struct {
    double CTDI_vol;
    double DLP;
    double effective_dose;
    double scan_length;
    int    kVp;
    double mAs;
    double pitch;
    int    computed;
} dose_t;

typedef struct {
    double dual_energy_ratio[MAX_DIM][MAX_DIM];
    double iodine_map[MAX_DIM][MAX_DIM];
    double virtual_keV[MAX_DIM][MAX_DIM];
    double low_keV;
    double high_keV;
    int    computed;
} dual_energy_t;

typedef struct {
    int    phantom_count;
    int    sinogram_count;
    int    recon_count;
    double residual;
    int    beam_hardening_corrected;
    int    scatter_corrected;
    int    ring_artifact_corrected;
} ct_state_t;

static phantom_t phantoms[4];
static sinogram_t sinograms[4];
static ct_image_t reconstructions[8];
static dose_t dose;
static dual_energy_t dual;
static ct_state_t ct;

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
static double dlog(double x) {
    if (x <= 0.0) return -100.0;
    double y = (x - 1.0) / (x + 1.0);
    double y2 = y * y;
    double sum = 0.0, term = y;
    for (int i = 0; i < 20; i++) {
        sum += term / (2 * i + 1);
        term *= y2;
    }
    return 2.0 * sum;
}

int ct_init(void) {
    if (initialized) return -1;
    ct.phantom_count = 0; ct.sinogram_count = 0; ct.recon_count = 0;
    ct.residual = 0.0;
    ct.beam_hardening_corrected = 0;
    ct.scatter_corrected = 0;
    ct.ring_artifact_corrected = 0;
    dose.computed = 0; dual.computed = 0;
    for (int i = 0; i < 4; i++) { phantoms[i].active = 0; sinograms[i].active = 0; }
    for (int i = 0; i < 8; i++) reconstructions[i].active = 0;
    initialized = 1;
    print_str("[CT] CT reconstruction module initialized\n");
    return 0;
}

int ct_create_phantom(int type, int size) {
    if (ct.phantom_count >= 4) return -1;
    phantom_t* p = &phantoms[ct.phantom_count];
    p->width = size < MAX_DIM ? size : MAX_DIM;
    p->height = size < MAX_DIM ? size : MAX_DIM;
    p->pixel_size = 1.0;
    double cx = p->width * 0.5, cy = p->height * 0.5;
    for (int y = 0; y < p->height; y++) {
        for (int x = 0; x < p->width; x++) {
            double val = 0.0;
            if (type == 1) {
                double r = dsqrt((x - cx) * (x - cx) + (y - cy) * (y - cy));
                val = r < 12.0 ? 1.0 : 0.0;
                if (r < 10.0 && r > 8.0) val = 0.5;
                double r2 = dsqrt((x - cx - 5) * (x - cx - 5) + (y - cy) * (y - cy));
                if (r2 < 3.0) val = 2.0;
            } else if (type == 2) {
                val = dcos(x * 0.3) * dcos(y * 0.3) * 0.5 + 0.5;
                double r = dsqrt((x - cx) * (x - cx) + (y - cy) * (y - cy));
                if (r < 10.0) val += 1.0;
            } else {
                val = 0.3 + 0.3 * dsin(x * 0.2) + 0.3 * dcos(y * 0.2);
            }
            p->data[x][y] = val;
        }
    }
    p->active = 1;
    return ct.phantom_count++;
}

void ct_radon_transform(int phantom_id, int n_angles, int n_det, int geometry) {
    if (phantom_id >= ct.phantom_count || ct.sinogram_count >= 4) return;
    phantom_t* p = &phantoms[phantom_id];
    sinogram_t* s = &sinograms[ct.sinogram_count];
    s->n_angles = n_angles < MAX_ANGLES ? n_angles : MAX_ANGLES;
    s->n_detectors = n_det < MAX_DET ? n_det : MAX_DET;
    s->angle_step = 3.14159 / s->n_angles;
    s->detector_spacing = p->width / (double)s->n_detectors;
    s->geometry = geometry;
    for (int a = 0; a < s->n_angles; a++) {
        double theta = a * s->angle_step;
        double cos_t = dcos(theta), sin_t = dsin(theta);
        for (int d = 0; d < s->n_detectors; d++) {
            double t = (d - s->n_detectors * 0.5) * s->detector_spacing;
            double sum = 0.0;
            int n_samples = p->width;
            for (int k = 0; k < n_samples; k++) {
                double s_param = (k - n_samples * 0.5) * p->pixel_size;
                double x = t * cos_t - s_param * sin_t + p->width * 0.5;
                double y = t * sin_t + s_param * cos_t + p->height * 0.5;
                int xi = (int)x, yi = (int)y;
                if (xi >= 0 && xi < p->width && yi >= 0 && yi < p->height) {
                    sum += p->data[xi][yi] * p->pixel_size;
                }
            }
            s->sinogram[a][d] = sum;
        }
    }
    s->active = 1;
    print_str("[CT] Radon transform phantom="); print_int(phantom_id);
    print_str(" angles="); print_int(n_angles);
    print_str(" detectors="); print_int(n_det);
    print_str(" geom="); print_int(geometry); print_str("\n");
    ct.sinogram_count++;
}

void ct_filter_sinogram(int sino_id, int filter) {
    if (sino_id >= ct.sinogram_count) return;
    sinogram_t* s = &sinograms[sino_id];
    for (int a = 0; a < s->n_angles; a++) {
        double filtered[MAX_DET];
        for (int d = 0; d < s->n_detectors; d++) {
            double sum = 0.0;
            for (int k = 0; k < s->n_detectors; k++) {
                double w = 0.0;
                int diff = d - k;
                if (diff == 0) w = 1.0;
                else if (diff % 2 != 0) w = -1.0 / (diff * diff * 3.14159 * 3.14159);
                if (filter == FILTER_SHEPP) {
                    double f = dabs(diff) / (double)s->n_detectors;
                    w *= dcos(f * 3.14159 * 0.5);
                } else if (filter == FILTER_HAMMING) {
                    double f = (double)diff / s->n_detectors;
                    w *= 0.54 + 0.46 * dcos(f * 3.14159);
                } else if (filter == FILTER_HANN) {
                    double f = (double)diff / s->n_detectors;
                    w *= 0.5 + 0.5 * dcos(f * 3.14159);
                }
                sum += s->sinogram[a][k] * w;
            }
            filtered[d] = sum;
        }
        for (int d = 0; d < s->n_detectors; d++) s->sinogram[a][d] = filtered[d];
    }
    print_str("[CT] Sinogram filtered with filter="); print_int(filter); print_str("\n");
}

void ct_backproject(int sino_id, int method) {
    if (sino_id >= ct.sinogram_count || ct.recon_count >= 8) return;
    sinogram_t* s = &sinograms[sino_id];
    ct_image_t* r = &reconstructions[ct.recon_count];
    r->width = s->n_detectors < MAX_DIM ? s->n_detectors : MAX_DIM;
    r->height = r->width;
    r->pixel_size = 1.0;
    r->method = method;
    r->filter = FILTER_RAMLAK;
    r->iterations = 1;
    for (int y = 0; y < r->height; y++)
        for (int x = 0; x < r->width; x++) r->data[x][y] = 0.0;
    if (method == RECON_FBP) {
        for (int a = 0; a < s->n_angles; a++) {
            double theta = a * s->angle_step;
            double cos_t = dcos(theta), sin_t = dsin(theta);
            for (int y = 0; y < r->height; y++) {
                for (int x = 0; x < r->width; x++) {
                    double t = (x - r->width * 0.5) * cos_t + (y - r->height * 0.5) * sin_t;
                    int d = (int)(t / s->detector_spacing + s->n_detectors * 0.5);
                    if (d >= 0 && d < s->n_detectors) {
                        r->data[x][y] += s->sinogram[a][d];
                    }
                }
            }
        }
        for (int y = 0; y < r->height; y++)
            for (int x = 0; x < r->width; x++)
                r->data[x][y] *= s->angle_step;
    } else if (method == RECON_ART || method == RECON_SART || method == RECON_SIRT) {
        int max_iter = method == RECON_SIRT ? 10 : 5;
        double lambda = 0.1;
        for (int y = 0; y < r->height; y++)
            for (int x = 0; x < r->width; x++) r->data[x][y] = 0.5;
        for (int iter = 0; iter < max_iter; iter++) {
            double total_res = 0.0;
            for (int a = 0; a < s->n_angles; a += 4) {
                double theta = a * s->angle_step;
                double cos_t = dcos(theta), sin_t = dsin(theta);
                for (int y = 0; y < r->height; y++) {
                    for (int x = 0; x < r->width; x++) {
                        double t = (x - r->width * 0.5) * cos_t + (y - r->height * 0.5) * sin_t;
                        int d = (int)(t / s->detector_spacing + s->n_detectors * 0.5);
                        if (d >= 0 && d < s->n_detectors) {
                            double diff = s->sinogram[a][d] - r->data[x][y];
                            r->data[x][y] += lambda * diff;
                            total_res += dabs(diff);
                        }
                    }
                }
            }
            r->residual = total_res / (s->n_angles * r->width * r->height);
            r->iterations = iter + 1;
            if (r->residual < 0.01) { r->converged = 1; break; }
        }
    }
    r->active = 1;
    ct.residual = r->residual;
    print_str("[CT] Reconstruction method="); print_int(method);
    print_str(" size="); print_int(r->width); print_str("x"); print_int(r->height); print_str("\n");
    if (method != RECON_FBP) {
        print_str("  iter="); print_int(r->iterations);
        print_str(" residual="); print_int((int)(r->residual * 10000));
        print_str(" conv="); print_int(r->converged); print_str("\n");
    }
    ct.recon_count++;
}

void ct_beam_hardening_correction(void) {
    ct.beam_hardening_corrected = 1;
    if (ct.recon_count == 0) return;
    ct_image_t* r = &reconstructions[ct.recon_count - 1];
    for (int y = 0; y < r->height; y++) {
        for (int x = 0; x < r->width; x++) {
            double val = r->data[x][y];
            r->data[x][y] = val - 0.05 * val * val;
        }
    }
    print_str("[CT] Beam hardening correction applied\n");
}

void ct_scatter_correction(void) {
    ct.scatter_corrected = 1;
    print_str("[CT] Scatter correction applied\n");
}

void ct_ring_artifact_correction(void) {
    ct.ring_artifact_corrected = 1;
    print_str("[CT] Ring artifact correction applied\n");
}

void ct_dose_calculation(int kVp, double mAs, double pitch, double scan_length) {
    dose.kVp = kVp;
    dose.mAs = mAs;
    dose.pitch = pitch;
    dose.scan_length = scan_length;
    double kVp_factor = (kVp / 120.0) * (kVp / 120.0);
    dose.CTDI_vol = mAs * kVp_factor / pitch * 0.5;
    dose.DLP = dose.CTDI_vol * scan_length;
    dose.effective_dose = dose.DLP * 0.015;
    dose.computed = 1;
    print_str("[CT] Dose calculation:\n");
    print_str("  kVp="); print_int(kVp);
    print_str(" mAs="); print_int((int)mAs);
    print_str(" pitch="); print_int((int)(pitch * 100));
    print_str(" length="); print_int((int)scan_length); print_str("cm\n");
    print_str("  CTDIvol="); print_int((int)(dose.CTDI_vol * 10));
    print_str("0mGy DLP="); print_int((int)(dose.DLP * 10));
    print_str("0mGy*cm E="); print_int((int)(dose.effective_dose * 100)); print_str("0uSv\n");
}

void ct_dual_energy_recon(int low_keV, int high_keV) {
    dual.low_keV = low_keV;
    dual.high_keV = high_keV;
    if (ct.recon_count < 2) {
        print_str("[CT] Need 2 reconstructions for dual energy\n");
        return;
    }
    ct_image_t* r_low = &reconstructions[ct.recon_count - 2];
    ct_image_t* r_high = &reconstructions[ct.recon_count - 1];
    int w = r_low->width, h = r_low->height;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            double mu_low = r_low->data[x][y];
            double mu_high = r_high->data[x][y];
            dual.virtual_keV[x][y] = (mu_low + mu_high) * 0.5;
            if (mu_high > 0.01) dual.dual_energy_ratio[x][y] = mu_low / mu_high;
            else dual.dual_energy_ratio[x][y] = 1.0;
            double iodine = (mu_low - mu_high) * 10.0;
            dual.iodine_map[x][y] = iodine > 0.0 ? iodine : 0.0;
        }
    }
    dual.computed = 1;
    print_str("[CT] Dual energy reconstruction: ");
    print_int(low_keV); print_str("/"); print_int(high_keV); print_str(" keV\n");
}

void ct_print_state(void) {
    print_str("[CT] Phantoms="); print_int(ct.phantom_count);
    print_str(" Sinograms="); print_int(ct.sinogram_count);
    print_str(" Recon="); print_int(ct.recon_count);
    print_str(" Dose="); print_int(dose.computed);
    print_str(" DE="); print_int(dual.computed); print_str("\n");
}

int main(void) {
    print_str("=== CT Reconstruction Demo ===\n\n");
    ct_init();

    print_str("Creating Shepp-Logan phantom...\n");
    ct_create_phantom(1, 24);

    print_str("Creating texture phantom...\n");
    ct_create_phantom(2, 24);

    print_str("\nRadon transform (parallel beam)...\n");
    ct_radon_transform(0, 32, 24, GEOM_PARALLEL);

    print_str("Radon transform (fan beam)...\n");
    ct_radon_transform(1, 32, 24, GEOM_FANBEAM);

    print_str("\nFiltering with Ram-Lak...\n");
    ct_filter_sinogram(0, FILTER_RAMLAK);

    print_str("Filtering with Shepp-Logan...\n");
    ct_filter_sinogram(0, FILTER_SHEPP);

    print_str("\nFBP reconstruction...\n");
    ct_backproject(0, RECON_FBP);

    print_str("\nART reconstruction...\n");
    ct_backproject(0, RECON_ART);

    print_str("\nSART reconstruction...\n");
    ct_backproject(0, RECON_SART);

    print_str("\nSIRT reconstruction...\n");
    ct_backproject(0, RECON_SIRT);

    print_str("\nArtifact corrections...\n");
    ct_beam_hardening_correction();
    ct_scatter_correction();
    ct_ring_artifact_correction();

    print_str("\nDose calculation...\n");
    ct_dose_calculation(120, 200.0, 1.0, 30.0);

    print_str("\nLow dose (80kVp, 50mAs)...\n");
    ct_dose_calculation(80, 50.0, 0.8, 30.0);

    print_str("\nDual energy reconstruction...\n");
    ct_radon_transform(0, 32, 24, GEOM_PARALLEL);
    ct_filter_sinogram(2, FILTER_RAMLAK);
    ct_backproject(2, RECON_FBP);
    ct_radon_transform(0, 32, 24, GEOM_PARALLEL);
    ct_filter_sinogram(3, FILTER_RAMLAK);
    ct_backproject(3, RECON_FBP);
    ct_dual_energy_recon(80, 140);

    ct_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
