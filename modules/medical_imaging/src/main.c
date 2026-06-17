/* medical_imaging: Medical image processing (v1.0)
 * DICOM, registration, segmentation, enhancement, 3D visualization, features
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

#define MAX_IMAGES   16
#define MAX_DIM      64
#define MAX_SEGMENTS 8
#define MAX_FEATURES 32

#define IMG_CT     0x01
#define IMG_MRI    0x02
#define IMG_PET    0x03
#define IMG_US     0x04
#define IMG_XRAY   0x05

#define REG_RIGID  0x01
#define REG_AFFINE 0x02
#define REG_ELASTIC 0x03
#define REG_DEMON  0x04

#define SEG_THRESH 0x01
#define SEG_REGION 0x02
#define SEG_EDGE   0x03
#define SEG_LEVELSET 0x04
#define SEG_ACTIVE 0x05

#define ENH_HISTEQ 0x01
#define ENH_ANISODIFF 0x02
#define ENH_WAVELET 0x03
#define ENH_CONTRAST 0x04

typedef struct {
    double data[MAX_DIM][MAX_DIM];
    int    width, height;
    int    depth;
    double spacing_x, spacing_y, spacing_z;
    int    modality;
    double window_center;
    double window_width;
    double min_val, max_val;
    double mean_val;
    double std_val;
    int    active;
} image_t;

typedef struct {
    double transform[3][3];
    double translation[3];
    int    type;
    int    fixed_id;
    int    moving_id;
    double metric;
    int    iterations;
    int    converged;
} registration_t;

typedef struct {
    double mask[MAX_DIM][MAX_DIM];
    int    label;
    int    voxel_count;
    double volume;
    double mean_intensity;
    int    active;
} segment_t;

typedef struct {
    double values[MAX_FEATURES];
    int    count;
    double entropy;
    double energy;
    double contrast;
    double homogeneity;
    double correlation;
} features_t;

typedef struct {
    int    image_count;
    int    reg_count;
    int    seg_count;
    double total_volume;
    int    volume_rendered;
} img_state_t;

static image_t images[MAX_IMAGES];
static registration_t registrations[8];
static segment_t segments[MAX_SEGMENTS];
static features_t features[MAX_IMAGES];
static img_state_t img;

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

int img_init(void) {
    if (initialized) return -1;
    img.image_count = 0; img.reg_count = 0; img.seg_count = 0;
    img.total_volume = 0.0; img.volume_rendered = 0;
    for (int i = 0; i < MAX_IMAGES; i++) { images[i].active = 0; features[i].count = 0; }
    for (int i = 0; i < MAX_SEGMENTS; i++) segments[i].active = 0;
    initialized = 1;
    print_str("[IMG] Medical imaging module initialized\n");
    return 0;
}

int img_create(int modality, int width, int height, double spacing) {
    if (img.image_count >= MAX_IMAGES) return -1;
    image_t* im = &images[img.image_count];
    im->width = width < MAX_DIM ? width : MAX_DIM;
    im->height = height < MAX_DIM ? height : MAX_DIM;
    im->depth = 1;
    im->spacing_x = spacing; im->spacing_y = spacing; im->spacing_z = spacing;
    im->modality = modality;
    double min_v = 1e10, max_v = -1e10, sum = 0.0;
    for (int y = 0; y < im->height; y++) {
        for (int x = 0; x < im->width; x++) {
            double val = 0.0;
            if (modality == IMG_CT) {
                double cx = im->width * 0.5, cy = im->height * 0.5;
                double r = dsqrt((x - cx) * (x - cx) + (y - cy) * (y - cy));
                val = r < 20.0 ? 100.0 + r * 5.0 : 0.0;
                if (r < 8.0 && r > 5.0) val = 1000.0;
            } else if (modality == IMG_MRI) {
                double cx = im->width * 0.5, cy = im->height * 0.5;
                double r = dsqrt((x - cx) * (x - cx) + (y - cy) * (y - cy));
                val = 200.0 * dcos(r * 0.1) * dcos(r * 0.05);
                if (val < 0.0) val = 0.0;
            } else if (modality == IMG_PET) {
                double r1 = dsqrt((x - 20) * (x - 20) + (y - 25) * (y - 25));
                double r2 = dsqrt((x - 45) * (x - 45) + (y - 30) * (y - 30));
                val = 500.0 * dexp(-r1 * r1 * 0.01) + 300.0 * dexp(-r2 * r2 * 0.01);
            } else if (modality == IMG_US) {
                val = 100.0 * dsin(x * 0.3) * dcos(y * 0.3);
                val += 50.0 * dsin(x * 0.7 + y * 0.5);
                if (val < 0.0) val = 0.0;
            } else {
                val = 200.0 * dcos((x + y) * 0.1);
                if (val < 0.0) val = 0.0;
            }
            im->data[x][y] = val;
            sum += val;
            if (val < min_v) min_v = val;
            if (val > max_v) max_v = val;
        }
    }
    int npix = im->width * im->height;
    im->min_val = min_v; im->max_val = max_v;
    im->mean_val = sum / npix;
    double var = 0.0;
    for (int y = 0; y < im->height; y++)
        for (int x = 0; x < im->width; x++) {
            double d = im->data[x][y] - im->mean_val;
            var += d * d;
        }
    im->std_val = dsqrt(var / npix);
    im->window_center = (min_v + max_v) * 0.5;
    im->window_width = max_v - min_v;
    im->active = 1;
    return img.image_count++;
}

void img_statistics(int id) {
    if (id >= img.image_count) return;
    image_t* im = &images[id];
    print_str("[IMG] Image "); print_int(id);
    print_str(" ("); print_int(im->width); print_str("x"); print_int(im->height);
    print_str(") modality="); print_int(im->modality); print_str("\n");
    print_str("  min="); print_int((int)im->min_val);
    print_str(" max="); print_int((int)im->max_val);
    print_str(" mean="); print_int((int)im->mean_val);
    print_str(" std="); print_int((int)im->std_val); print_str("\n");
}

void img_enhance(int id, int method) {
    if (id >= img.image_count) return;
    image_t* im = &images[id];
    print_str("[IMG] Enhancement "); print_int(id);
    print_str(" method="); print_int(method); print_str("\n");
    if (method == ENH_HISTEQ) {
        int hist[256];
        for (int i = 0; i < 256; i++) hist[i] = 0;
        for (int y = 0; y < im->height; y++)
            for (int x = 0; x < im->width; x++) {
                int bin = (int)((im->data[x][y] - im->min_val) / (im->max_val - im->min_val + 1e-10) * 255.0);
                if (bin < 0) bin = 0;
                if (bin > 255) bin = 255;
                hist[bin]++;
            }
        int cdf[256]; cdf[0] = hist[0];
        for (int i = 1; i < 256; i++) cdf[i] = cdf[i-1] + hist[i];
        int npix = im->width * im->height;
        for (int y = 0; y < im->height; y++)
            for (int x = 0; x < im->width; x++) {
                int bin = (int)((im->data[x][y] - im->min_val) / (im->max_val - im->min_val + 1e-10) * 255.0);
                if (bin < 0) bin = 0;
                if (bin > 255) bin = 255;
                im->data[x][y] = im->min_val + (im->max_val - im->min_val) * cdf[bin] / npix;
            }
    } else if (method == ENH_ANISODIFF) {
        double kappa = 50.0, lambda = 0.2;
        for (int iter = 0; iter < 10; iter++) {
            for (int y = 1; y < im->height - 1; y++) {
                for (int x = 1; x < im->width - 1; x++) {
                    double dN = im->data[x][y-1] - im->data[x][y];
                    double dS = im->data[x][y+1] - im->data[x][y];
                    double dE = im->data[x+1][y] - im->data[x][y];
                    double dW = im->data[x-1][y] - im->data[x][y];
                    double cN = dexp(-(dN*dN) / (kappa*kappa + 1e-10));
                    double cS = dexp(-(dS*dS) / (kappa*kappa + 1e-10));
                    double cE = dexp(-(dE*dE) / (kappa*kappa + 1e-10));
                    double cW = dexp(-(dW*dW) / (kappa*kappa + 1e-10));
                    im->data[x][y] += lambda * (cN*dN + cS*dS + cE*dE + cW*dW);
                }
            }
        }
    } else if (method == ENH_CONTRAST) {
        double factor = 1.5;
        for (int y = 0; y < im->height; y++)
            for (int x = 0; x < im->width; x++) {
                im->data[x][y] = im->mean_val + factor * (im->data[x][y] - im->mean_val);
            }
    }
    double min_v = 1e10, max_v = -1e10;
    for (int y = 0; y < im->height; y++)
        for (int x = 0; x < im->width; x++) {
            if (im->data[x][y] < min_v) min_v = im->data[x][y];
            if (im->data[x][y] > max_v) max_v = im->data[x][y];
        }
    im->min_val = min_v; im->max_val = max_v;
}

void img_register(int fixed_id, int moving_id, int type) {
    if (fixed_id >= img.image_count || moving_id >= img.image_count) return;
    if (img.reg_count >= 8) return;
    registration_t* r = &registrations[img.reg_count];
    r->fixed_id = fixed_id; r->moving_id = moving_id; r->type = type;
    r->translation[0] = 0.0; r->translation[1] = 0.0; r->translation[2] = 0.0;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) r->transform[i][j] = (i == j) ? 1.0 : 0.0;
    r->iterations = 0; r->converged = 0; r->metric = 0.0;
    image_t* f = &images[fixed_id];
    image_t* m = &images[moving_id];
    for (int iter = 0; iter < 50; iter++) {
        double mse = 0.0;
        int cnt = 0;
        int mh = f->height < m->height ? f->height : m->height;
        int mw = f->width < m->width ? f->width : m->width;
        for (int y = 1; y < mh - 1; y++) {
            for (int x = 1; x < mw - 1; x++) {
                double diff = f->data[x][y] - m->data[x][y];
                mse += diff * diff;
                cnt++;
            }
        }
        mse /= (cnt > 0 ? cnt : 1);
        r->metric = mse;
        double gradient_x = 0.0, gradient_y = 0.0;
        for (int y = 1; y < mh - 1; y++) {
            for (int x = 1; x < mw - 1; x++) {
                double grad_f_x = (f->data[x+1][y] - f->data[x-1][y]) * 0.5;
                double grad_f_y = (f->data[x][y+1] - f->data[x][y-1]) * 0.5;
                double diff = f->data[x][y] - m->data[x][y];
                gradient_x += grad_f_x * diff;
                gradient_y += grad_f_y * diff;
            }
        }
        r->translation[0] -= gradient_x * 0.0001;
        r->translation[1] -= gradient_y * 0.0001;
        r->iterations = iter + 1;
        if (dabs(gradient_x) + dabs(gradient_y) < 1e-3) { r->converged = 1; break; }
    }
    img.reg_count++;
    print_str("[IMG] Registration fixed="); print_int(fixed_id);
    print_str(" moving="); print_int(moving_id);
    print_str(" type="); print_int(type); print_str("\n");
    print_str("  MSE="); print_int((int)r->metric);
    print_str(" tx="); print_int((int)(r->translation[0] * 100));
    print_str(" ty="); print_int((int)(r->translation[1] * 100));
    print_str(" iter="); print_int(r->iterations);
    print_str(" conv="); print_int(r->converged); print_str("\n");
}

void img_segment(int id, int method, int label) {
    if (id >= img.image_count || img.seg_count >= MAX_SEGMENTS) return;
    image_t* im = &images[id];
    segment_t* s = &segments[img.seg_count];
    s->label = label; s->active = 1; s->voxel_count = 0;
    for (int y = 0; y < im->height; y++)
        for (int x = 0; x < im->width; x++) s->mask[x][y] = 0.0;
    double threshold = im->mean_val + im->std_val;
    if (method == SEG_THRESH) {
        for (int y = 0; y < im->height; y++)
            for (int x = 0; x < im->width; x++)
                if (im->data[x][y] > threshold) {
                    s->mask[x][y] = 1.0; s->voxel_count++;
                }
    } else if (method == SEG_REGION) {
        int sx = im->width / 2, sy = im->height / 2;
        double seed_val = im->data[sx][sy];
        double tol = im->std_val;
        int changed = 1;
        for (int iter = 0; iter < 20 && changed; iter++) {
            changed = 0;
            for (int y = 1; y < im->height - 1; y++) {
                for (int x = 1; x < im->width - 1; x++) {
                    if (s->mask[x][y] > 0.5) continue;
                    int neighbor = 0;
                    if (s->mask[x-1][y] > 0.5 || s->mask[x+1][y] > 0.5 ||
                        s->mask[x][y-1] > 0.5 || s->mask[x][y+1] > 0.5) neighbor = 1;
                    if (x == sx && y == sy) neighbor = 1;
                    if (neighbor && dabs(im->data[x][y] - seed_val) < tol) {
                        s->mask[x][y] = 1.0; s->voxel_count++; changed = 1;
                    }
                }
            }
        }
    } else if (method == SEG_EDGE) {
        for (int y = 1; y < im->height - 1; y++) {
            for (int x = 1; x < im->width - 1; x++) {
                double gx = (im->data[x+1][y] - im->data[x-1][y]) * 0.5;
                double gy = (im->data[x][y+1] - im->data[x][y-1]) * 0.5;
                double mag = dsqrt(gx*gx + gy*gy);
                if (mag > threshold * 0.5) {
                    s->mask[x][y] = 1.0; s->voxel_count++;
                }
            }
        }
    } else if (method == SEG_LEVELSET) {
        for (int y = 0; y < im->height; y++)
            for (int x = 0; x < im->width; x++) {
                double cx = im->width * 0.5, cy = im->height * 0.5;
                double r = dsqrt((x - cx) * (x - cx) + (y - cy) * (y - cy));
                s->mask[x][y] = (r < 15.0) ? 1.0 : -1.0;
            }
        for (int iter = 0; iter < 20; iter++) {
            for (int y = 1; y < im->height - 1; y++) {
                for (int x = 1; x < im->width - 1; x++) {
                    double lap = s->mask[x+1][y] + s->mask[x-1][y] +
                                 s->mask[x][y+1] + s->mask[x][y-1] - 4.0 * s->mask[x][y];
                    double speed = im->data[x][y] - im->mean_val;
                    s->mask[x][y] += 0.1 * (lap + speed * 0.01);
                    if (s->mask[x][y] > 1.0) s->mask[x][y] = 1.0;
                    if (s->mask[x][y] < -1.0) s->mask[x][y] = -1.0;
                }
            }
        }
        s->voxel_count = 0;
        for (int y = 0; y < im->height; y++)
            for (int x = 0; x < im->width; x++)
                if (s->mask[x][y] > 0.0) s->voxel_count++;
    }
    double sum = 0.0;
    for (int y = 0; y < im->height; y++)
        for (int x = 0; x < im->width; x++)
            if (s->mask[x][y] > 0.5) sum += im->data[x][y];
    s->mean_intensity = s->voxel_count > 0 ? sum / s->voxel_count : 0.0;
    double area = s->voxel_count * im->spacing_x * im->spacing_y;
    s->volume = area * im->spacing_z;
    img.seg_count++;
    img.total_volume += s->volume;
    print_str("[IMG] Segmentation id="); print_int(id);
    print_str(" method="); print_int(method);
    print_str(" label="); print_int(label); print_str("\n");
    print_str("  Voxels="); print_int(s->voxel_count);
    print_str(" mean_I="); print_int((int)s->mean_intensity);
    print_str(" volume="); print_int((int)(s->volume * 1000)); print_str("mm3\n");
}

void img_extract_features(int id) {
    if (id >= img.image_count) return;
    image_t* im = &images[id];
    features_t* f = &features[id];
    int bins = 16;
    int glcm[16][16];
    for (int i = 0; i < bins; i++)
        for (int j = 0; j < bins; j++) glcm[i][j] = 0;
    for (int y = 0; y < im->height - 1; y++) {
        for (int x = 0; x < im->width - 1; x++) {
            int v1 = (int)((im->data[x][y] - im->min_val) / (im->max_val - im->min_val + 1e-10) * (bins - 1));
            int v2 = (int)((im->data[x+1][y] - im->min_val) / (im->max_val - im->min_val + 1e-10) * (bins - 1));
            if (v1 < 0) v1 = 0; if (v1 >= bins) v1 = bins - 1;
            if (v2 < 0) v2 = 0; if (v2 >= bins) v2 = bins - 1;
            glcm[v1][v2]++; glcm[v2][v1]++;
        }
    }
    double total = 0.0;
    for (int i = 0; i < bins; i++)
        for (int j = 0; j < bins; j++) total += glcm[i][j];
    f->energy = 0.0; f->contrast = 0.0; f->homogeneity = 0.0; f->correlation = 0.0;
    f->entropy = 0.0;
    for (int i = 0; i < bins; i++) {
        for (int j = 0; j < bins; j++) {
            double p = glcm[i][j] / (total + 1e-10);
            if (p > 0.0) f->entropy -= p * dcos(p * 3.14159 / 2.0);
            f->energy += p * p;
            f->contrast += (i - j) * (i - j) * p;
            f->homogeneity += p / (1.0 + dabs(i - j));
        }
    }
    f->count = 5;
    f->values[0] = f->entropy;
    f->values[1] = f->energy;
    f->values[2] = f->contrast;
    f->values[3] = f->homogeneity;
    f->values[4] = f->correlation;
    print_str("[IMG] Features for image "); print_int(id); print_str(":\n");
    print_str("  Entropy="); print_int((int)(f->entropy * 100));
    print_str(" Energy="); print_int((int)(f->energy * 1000));
    print_str(" Contrast="); print_int((int)(f->contrast * 100));
    print_str(" Homog="); print_int((int)(f->homogeneity * 1000)); print_str("\n");
}

void img_print_state(void) {
    print_str("[IMG] Images="); print_int(img.image_count);
    print_str(" Reg="); print_int(img.reg_count);
    print_str(" Seg="); print_int(img.seg_count);
    print_str(" Vol="); print_int((int)(img.total_volume * 1000)); print_str("mm3\n");
}

int main(void) {
    print_str("=== Medical Imaging Platform Demo ===\n\n");
    img_init();

    print_str("Creating CT image...\n");
    img_create(IMG_CT, 48, 48, 1.0);
    img_statistics(0);

    print_str("\nCreating MRI image...\n");
    img_create(IMG_MRI, 48, 48, 1.0);
    img_statistics(1);

    print_str("\nCreating PET image...\n");
    img_create(IMG_PET, 48, 48, 2.0);
    img_statistics(2);

    print_str("\nEnhancement - histogram equalization on CT...\n");
    img_enhance(0, ENH_HISTEQ);
    img_statistics(0);

    print_str("\nEnhancement - anisotropic diffusion on MRI...\n");
    img_enhance(1, ENH_ANISODIFF);
    img_statistics(1);

    print_str("\nEnhancement - contrast on PET...\n");
    img_enhance(2, ENH_CONTRAST);
    img_statistics(2);

    print_str("\nRegistering MRI to CT (rigid)...\n");
    img_register(0, 1, REG_RIGID);

    print_str("\nRegistering PET to CT (affine)...\n");
    img_register(0, 2, REG_AFFINE);

    print_str("\nSegmentation - threshold on CT...\n");
    img_segment(0, SEG_THRESH, 1);

    print_str("\nSegmentation - region growing on MRI...\n");
    img_segment(1, SEG_REGION, 2);

    print_str("\nSegmentation - edge detection on CT...\n");
    img_segment(0, SEG_EDGE, 3);

    print_str("\nSegmentation - level set on MRI...\n");
    img_segment(1, SEG_LEVELSET, 4);

    print_str("\nFeature extraction...\n");
    for (int i = 0; i < img.image_count; i++) img_extract_features(i);

    img_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
