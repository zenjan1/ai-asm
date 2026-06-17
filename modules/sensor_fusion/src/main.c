/* sensor_fusion: Multi-sensor fusion platform (v1.0)
 * Kalman filters, sensor models, data association, state estimation, fault detection
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

#define STATE_DIM   4
#define MEAS_DIM    3
#define MAX_SENSORS 6
#define MAX_TRACKS  16
#define MAX_PARTS   20
#define MAX_SIGMA   9
#define HIST_SIZE   32

#define FILT_EKF    0x01
#define FILT_UKF    0x02
#define FILT_PF     0x03
#define FILT_IF     0x04

#define SENS_GPS    0x01
#define SENS_IMU    0x02
#define SENS_LIDAR  0x03
#define SENS_CAM    0x04
#define SENS_RADAR  0x05
#define SENS_BARO   0x06

#define ASSO_NN     0x01
#define ASSO_JC     0x02
#define ASSO_MHT    0x03

#define STAT_POS    0x01
#define STAT_VEL    0x02
#define STAT_ACC    0x03
#define STAT_HEAD   0x04

typedef struct {
    double x[STATE_DIM];
    double P[STATE_DIM][STATE_DIM];
    double F[STATE_DIM][STATE_DIM];
    double Q[STATE_DIM][STATE_DIM];
    double H[MEAS_DIM][STATE_DIM];
    double R[MEAS_DIM][MEAS_DIM];
    int    dim;
    int    meas_dim;
    double dt;
    int    active;
} ekf_state_t;

typedef struct {
    double x[STATE_DIM];
    double Wm[MAX_SIGMA];
    double Wc[MAX_SIGMA];
    double sigma[MAX_SIGMA][STATE_DIM];
    double P[STATE_DIM][STATE_DIM];
    double Q[STATE_DIM][STATE_DIM];
    double R[MEAS_DIM][MEAS_DIM];
    double alpha, beta, kappa;
    double lambda;
    int    dim;
    int    n_sigma;
    int    active;
} ukf_state_t;

typedef struct {
    double x[STATE_DIM];
    double w;
    int    active;
} particle_t;

typedef struct {
    particle_t parts[MAX_PARTS];
    double P[STATE_DIM][STATE_DIM];
    double Q[STATE_DIM][STATE_DIM];
    double R[MEAS_DIM][MEAS_DIM];
    int    n_particles;
    int    dim;
    double N_eff;
    int    active;
} pf_state_t;

typedef struct {
    double info_vec[STATE_DIM];
    double info_mat[STATE_DIM][STATE_DIM];
    double contrib_vec[STATE_DIM];
    double contrib_mat[STATE_DIM][STATE_DIM];
    int    dim;
    int    n_contrib;
    int    active;
} info_filter_t;

typedef struct {
    int    type;
    double range_res;
    double bearing_res;
    double noise_std;
    double rate_hz;
    double latency_ms;
    double bias[STATE_DIM];
    double scale;
    int    fault;
    int    active;
} sensor_model_t;

typedef struct {
    double x[STATE_DIM];
    double vx[STATE_DIM];
    double P[STATE_DIM][STATE_DIM];
    double weight;
    int    id;
    int    age;
    int    n_meas;
    int    active;
} track_t;

typedef struct {
    double timestamp;
    double sensor_time;
    double offset;
    double drift_rate;
    int    sync_count;
    int    active;
} time_sync_t;

typedef struct {
    double hist_x[HIST_SIZE];
    double hist_y[HIST_SIZE];
    double hist_t[HIST_SIZE];
    int    head;
    int    count;
} delay_buffer_t;

typedef struct {
    int    filter_type;
    int    n_sensors;
    int    n_tracks;
    int    assoc_method;
    int    est_type;
    double fusion_rate;
    double total_updates;
    double total_fusions;
    int    fault_count;
    int    sync_active;
} fusion_state_t;

static ekf_state_t ekf;
static ukf_state_t ukf;
static pf_state_t pf;
static info_filter_t ifilt;
static sensor_model_t sensors[MAX_SENSORS];
static track_t tracks[MAX_TRACKS];
static time_sync_t tsync;
static delay_buffer_t dbuf;
static fusion_state_t fus;

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

int sf_init(void) {
    if (initialized) return -1;
    fus.filter_type = FILT_EKF;
    fus.n_sensors = 0;
    fus.n_tracks = 0;
    fus.assoc_method = ASSO_NN;
    fus.est_type = STAT_POS;
    fus.fusion_rate = 10.0;
    fus.total_updates = 0.0;
    fus.total_fusions = 0.0;
    fus.fault_count = 0;
    fus.sync_active = 0;
    for (int i = 0; i < MAX_SENSORS; i++) sensors[i].active = 0;
    for (int i = 0; i < MAX_TRACKS; i++) tracks[i].active = 0;
    for (int i = 0; i < STATE_DIM; i++) {
        ekf.x[i] = 0.0;
        for (int j = 0; j < STATE_DIM; j++) {
            ekf.P[i][j] = (i == j) ? 10.0 : 0.0;
            ekf.F[i][j] = (i == j) ? 1.0 : 0.0;
            ekf.Q[i][j] = (i == j) ? 0.1 : 0.0;
        }
        for (int j = 0; j < MEAS_DIM; j++) {
            ekf.H[j][i] = 0.0;
        }
    }
    ekf.H[0][0] = 1.0; ekf.H[1][1] = 1.0; ekf.H[2][2] = 1.0;
    for (int i = 0; i < MEAS_DIM; i++) {
        for (int j = 0; j < MEAS_DIM; j++)
            ekf.R[i][j] = (i == j) ? 1.0 : 0.0;
    }
    ekf.dim = STATE_DIM; ekf.meas_dim = MEAS_DIM; ekf.dt = 0.1; ekf.active = 1;
    for (int i = 0; i < STATE_DIM; i++) {
        ukf.x[i] = 0.0;
        for (int j = 0; j < STATE_DIM; j++)
            ukf.P[i][j] = (i == j) ? 10.0 : 0.0;
    }
    ukf.alpha = 1e-3; ukf.beta = 2.0; ukf.kappa = 0.0;
    ukf.lambda = ukf.alpha * ukf.alpha * (STATE_DIM + ukf.kappa) - STATE_DIM;
    ukf.dim = STATE_DIM; ukf.n_sigma = 2 * STATE_DIM + 1; ukf.active = 0;
    double total_w0 = ukf.lambda / (STATE_DIM + ukf.lambda);
    double wi = 1.0 / (2.0 * (STATE_DIM + ukf.lambda));
    ukf.Wm[0] = total_w0; ukf.Wc[0] = total_w0 + (1.0 - ukf.alpha * ukf.alpha + ukf.beta);
    for (int i = 1; i < ukf.n_sigma; i++) { ukf.Wm[i] = wi; ukf.Wc[i] = wi; }
    for (int i = 0; i < MAX_PARTS; i++) {
        for (int j = 0; j < STATE_DIM; j++) pf.parts[i].x[j] = 0.0;
        pf.parts[i].w = 1.0 / MAX_PARTS; pf.parts[i].active = 0;
    }
    pf.n_particles = MAX_PARTS; pf.dim = STATE_DIM; pf.N_eff = MAX_PARTS; pf.active = 0;
    for (int i = 0; i < STATE_DIM; i++) {
        ifilt.info_vec[i] = 0.0;
        for (int j = 0; j < STATE_DIM; j++)
            ifilt.info_mat[i][j] = (i == j) ? 0.1 : 0.0;
    }
    ifilt.dim = STATE_DIM; ifilt.n_contrib = 0; ifilt.active = 0;
    tsync.timestamp = 0.0; tsync.sensor_time = 0.0; tsync.offset = 0.0;
    tsync.drift_rate = 0.001; tsync.sync_count = 0; tsync.active = 0;
    dbuf.head = 0; dbuf.count = 0;
    initialized = 1;
    print_str("[SF] Sensor fusion initialized\n");
    return 0;
}

void sf_configure_ekf(double dt, double process_noise, double meas_noise) {
    ekf.dt = dt;
    ekf.F[0][1] = dt; ekf.F[2][3] = dt;
    for (int i = 0; i < STATE_DIM; i++)
        for (int j = 0; j < STATE_DIM; j++)
            ekf.Q[i][j] = (i == j) ? process_noise * dt : 0.0;
    for (int i = 0; i < MEAS_DIM; i++)
        for (int j = 0; j < MEAS_DIM; j++)
            ekf.R[i][j] = (i == j) ? meas_noise : 0.0;
    fus.filter_type = FILT_EKF;
    print_str("[SF] EKF configured dt="); print_int((int)(dt * 1000));
    print_str("ms Q="); print_int((int)(process_noise * 1000));
    print_str(" R="); print_int((int)(meas_noise * 1000)); print_str("\n");
}

void sf_ekf_predict(void) {
    double x_new[STATE_DIM];
    for (int i = 0; i < STATE_DIM; i++) {
        x_new[i] = 0.0;
        for (int j = 0; j < STATE_DIM; j++)
            x_new[i] += ekf.F[i][j] * ekf.x[j];
    }
    for (int i = 0; i < STATE_DIM; i++) ekf.x[i] = x_new[i];
    double P_new[STATE_DIM][STATE_DIM];
    for (int i = 0; i < STATE_DIM; i++)
        for (int j = 0; j < STATE_DIM; j++) {
            P_new[i][j] = 0.0;
            for (int k = 0; k < STATE_DIM; k++)
                for (int l = 0; l < STATE_DIM; l++)
                    P_new[i][j] += ekf.F[i][k] * ekf.P[k][l] * ekf.F[j][l];
            P_new[i][j] += ekf.Q[i][j];
        }
    for (int i = 0; i < STATE_DIM; i++)
        for (int j = 0; j < STATE_DIM; j++)
            ekf.P[i][j] = P_new[i][j];
    fus.total_updates += 1.0;
}

void sf_ekf_update(double z0, double z1, double z2) {
    double z[MEAS_DIM] = {z0, z1, z2};
    double y[MEAS_DIM];
    for (int i = 0; i < MEAS_DIM; i++) {
        y[i] = z[i];
        for (int j = 0; j < STATE_DIM; j++)
            y[i] -= ekf.H[i][j] * ekf.x[j];
    }
    double S[MEAS_DIM][MEAS_DIM];
    for (int i = 0; i < MEAS_DIM; i++)
        for (int j = 0; j < MEAS_DIM; j++) {
            S[i][j] = ekf.R[i][j];
            for (int k = 0; k < STATE_DIM; k++)
                for (int l = 0; l < STATE_DIM; l++)
                    S[i][j] += ekf.H[i][k] * ekf.P[k][l] * ekf.H[j][l];
        }
    double S00 = S[0][0] + 1e-10;
    double S11 = S[1][1] + 1e-10;
    double S22 = S[2][2] + 1e-10;
    double K[STATE_DIM][MEAS_DIM];
    for (int i = 0; i < STATE_DIM; i++) {
        K[i][0] = 0.0; K[i][1] = 0.0; K[i][2] = 0.0;
        for (int j = 0; j < STATE_DIM; j++) {
            K[i][0] += ekf.P[i][j] * ekf.H[0][j] / S00;
            K[i][1] += ekf.P[i][j] * ekf.H[1][j] / S11;
            K[i][2] += ekf.P[i][j] * ekf.H[2][j] / S22;
        }
    }
    for (int i = 0; i < STATE_DIM; i++)
        for (int j = 0; j < MEAS_DIM; j++)
            ekf.x[i] += K[i][j] * y[j];
    double I_KH[STATE_DIM][STATE_DIM];
    for (int i = 0; i < STATE_DIM; i++)
        for (int j = 0; j < STATE_DIM; j++) {
            I_KH[i][j] = (i == j) ? 1.0 : 0.0;
            for (int k = 0; k < MEAS_DIM; k++)
                I_KH[i][j] -= K[i][k] * ekf.H[k][j];
        }
    double P_new[STATE_DIM][STATE_DIM];
    for (int i = 0; i < STATE_DIM; i++)
        for (int j = 0; j < STATE_DIM; j++) {
            P_new[i][j] = 0.0;
            for (int k = 0; k < STATE_DIM; k++)
                P_new[i][j] += I_KH[i][k] * ekf.P[k][j];
        }
    for (int i = 0; i < STATE_DIM; i++)
        for (int j = 0; j < STATE_DIM; j++)
            ekf.P[i][j] = P_new[i][j];
    fus.total_fusions += 1.0;
}

void sf_ukf_init(void) {
    for (int i = 0; i < STATE_DIM; i++)
        for (int j = 0; j < STATE_DIM; j++) {
            ukf.P[i][j] = (i == j) ? 10.0 : 0.0;
            ukf.Q[i][j] = (i == j) ? 0.1 : 0.0;
        }
    for (int i = 0; i < MEAS_DIM; i++)
        for (int j = 0; j < MEAS_DIM; j++)
            ukf.R[i][j] = (i == j) ? 1.0 : 0.0;
    ukf.active = 1;
    fus.filter_type = FILT_UKF;
    print_str("[SF] UKF initialized n_sigma="); print_int(ukf.n_sigma); print_str("\n");
}

void sf_ukf_sigma_points(void) {
    double sq = dsqrt(STATE_DIM + ukf.lambda);
    for (int j = 0; j < STATE_DIM; j++) ukf.sigma[0][j] = ukf.x[j];
    for (int i = 0; i < STATE_DIM; i++) {
        double dp = sq * dsqrt(ukf.P[i][i] + 1e-10);
        for (int j = 0; j < STATE_DIM; j++) ukf.sigma[1 + i][j] = ukf.x[j];
        ukf.sigma[1 + i][i] += dp;
        for (int j = 0; j < STATE_DIM; j++) ukf.sigma[1 + STATE_DIM + i][j] = ukf.x[j];
        ukf.sigma[1 + STATE_DIM + i][i] -= dp;
    }
    print_str("[SF] UKF sigma points generated: "); print_int(ukf.n_sigma); print_str("\n");
}

void sf_ukf_predict(void) {
    double x_pred[STATE_DIM];
    for (int j = 0; j < STATE_DIM; j++) {
        x_pred[j] = 0.0;
        for (int s = 0; s < ukf.n_sigma; s++)
            x_pred[j] += ukf.Wm[s] * ukf.sigma[s][j];
    }
    for (int j = 0; j < STATE_DIM; j++) ukf.x[j] = x_pred[j];
    double P_pred[STATE_DIM][STATE_DIM];
    for (int i = 0; i < STATE_DIM; i++)
        for (int j = 0; j < STATE_DIM; j++) {
            P_pred[i][j] = 0.0;
            for (int s = 0; s < ukf.n_sigma; s++) {
                double di = ukf.sigma[s][i] - x_pred[i];
                double dj = ukf.sigma[s][j] - x_pred[j];
                P_pred[i][j] += ukf.Wc[s] * di * dj;
            }
            P_pred[i][j] += ukf.Q[i][j];
        }
    for (int i = 0; i < STATE_DIM; i++)
        for (int j = 0; j < STATE_DIM; j++)
            ukf.P[i][j] = P_pred[i][j];
    fus.total_updates += 1.0;
}

void sf_pf_init(int n_particles) {
    pf.n_particles = n_particles < MAX_PARTS ? n_particles : MAX_PARTS;
    double inv_n = 1.0 / pf.n_particles;
    for (int i = 0; i < STATE_DIM; i++)
        for (int j = 0; j < STATE_DIM; j++) {
            pf.P[i][j] = (i == j) ? 10.0 : 0.0;
            pf.Q[i][j] = (i == j) ? 0.1 : 0.0;
        }
    for (int p = 0; p < pf.n_particles; p++) {
        for (int j = 0; j < STATE_DIM; j++)
            pf.parts[p].x[j] = ((p * 7 + j * 13) % 100 - 50) * 0.1;
        pf.parts[p].w = inv_n;
        pf.parts[p].active = 1;
    }
    pf.N_eff = pf.n_particles;
    pf.active = 1;
    fus.filter_type = FILT_PF;
    print_str("[SF] Particle filter init n="); print_int(pf.n_particles); print_str("\n");
}

void sf_pf_predict(void) {
    for (int p = 0; p < pf.n_particles; p++) {
        pf.parts[p].x[0] += pf.parts[p].x[1] * 0.1;
        pf.parts[p].x[2] += pf.parts[p].x[3] * 0.1;
        for (int j = 0; j < STATE_DIM; j++)
            pf.parts[p].x[j] += ((p * 3 + j * 5) % 7 - 3) * 0.01;
    }
    fus.total_updates += 1.0;
}

void sf_pf_update(double z0, double z1) {
    double w_sum = 0.0;
    for (int p = 0; p < pf.n_particles; p++) {
        double dx = pf.parts[p].x[0] - z0;
        double dy = pf.parts[p].x[2] - z1;
        double d2 = dx * dx + dy * dy;
        double lik = dexp(-d2 * 0.5);
        pf.parts[p].w *= lik;
        w_sum += pf.parts[p].w;
    }
    if (w_sum > 1e-10)
        for (int p = 0; p < pf.n_particles; p++)
            pf.parts[p].w /= w_sum;
    pf.N_eff = 0.0;
    for (int p = 0; p < pf.n_particles; p++)
        pf.N_eff += pf.parts[p].w * pf.parts[p].w;
    pf.N_eff = 1.0 / (pf.N_eff + 1e-10);
    double mean_x[STATE_DIM];
    for (int j = 0; j < STATE_DIM; j++) {
        mean_x[j] = 0.0;
        for (int p = 0; p < pf.n_particles; p++)
            mean_x[j] += pf.parts[p].w * pf.parts[p].x[j];
    }
    print_str("[SF] PF update N_eff="); print_int((int)pf.N_eff);
    print_str(" x="); print_int((int)(mean_x[0] * 100));
    print_str(" y="); print_int((int)(mean_x[2] * 100)); print_str("\n");
    fus.total_fusions += 1.0;
}

void sf_info_filter_init(void) {
    for (int i = 0; i < STATE_DIM; i++) {
        ifilt.info_vec[i] = 0.0;
        for (int j = 0; j < STATE_DIM; j++)
            ifilt.info_mat[i][j] = (i == j) ? 0.1 : 0.0;
    }
    ifilt.n_contrib = 0; ifilt.active = 1;
    fus.filter_type = FILT_IF;
    print_str("[SF] Information filter initialized\n");
}

void sf_info_filter_update(double z0, double z1) {
    double H0[STATE_DIM] = {1.0, 0.0, 0.0, 0.0};
    double H1[STATE_DIM] = {0.0, 0.0, 1.0, 0.0};
    double R_inv = 1.0;
    for (int i = 0; i < STATE_DIM; i++) {
        ifilt.contrib_vec[i] = H0[i] * R_inv * z0 + H1[i] * R_inv * z1;
        for (int j = 0; j < STATE_DIM; j++)
            ifilt.contrib_mat[i][j] = H0[i] * R_inv * H0[j] + H1[i] * R_inv * H1[j];
    }
    for (int i = 0; i < STATE_DIM; i++) {
        ifilt.info_vec[i] += ifilt.contrib_vec[i];
        for (int j = 0; j < STATE_DIM; j++)
            ifilt.info_mat[i][j] += ifilt.contrib_mat[i][j];
    }
    ifilt.n_contrib++;
    print_str("[SF] Info filter update contrib="); print_int(ifilt.n_contrib); print_str("\n");
}

int sf_add_sensor(int type, double noise, double rate) {
    if (fus.n_sensors >= MAX_SENSORS) return -1;
    sensor_model_t* s = &sensors[fus.n_sensors];
    s->type = type; s->noise_std = noise; s->rate_hz = rate;
    s->latency_ms = 5.0 + fus.n_sensors * 2.0;
    s->scale = 1.0;
    for (int i = 0; i < STATE_DIM; i++) s->bias[i] = 0.0;
    s->fault = 0; s->active = 1;
    if (type == SENS_GPS) { s->range_res = 2.5; s->bearing_res = 0.0; s->bias[0] = 0.5; s->bias[2] = 0.3; }
    else if (type == SENS_IMU) { s->range_res = 0.0; s->bearing_res = 0.0; s->bias[1] = 0.01; }
    else if (type == SENS_LIDAR) { s->range_res = 0.05; s->bearing_res = 0.01; }
    else if (type == SENS_CAM) { s->range_res = 0.0; s->bearing_res = 0.005; }
    else if (type == SENS_RADAR) { s->range_res = 0.3; s->bearing_res = 0.02; }
    else if (type == SENS_BARO) { s->range_res = 0.1; }
    fus.n_sensors++;
    print_str("[SF] Sensor added type="); print_int(type);
    print_str(" noise="); print_int((int)(noise * 1000));
    print_str(" rate="); print_int((int)rate); print_str("Hz\n");
    return fus.n_sensors - 1;
}

void sf_check_sensor_fault(int sensor_id) {
    if (sensor_id >= fus.n_sensors) return;
    sensor_model_t* s = &sensors[sensor_id];
    double fault_metric = dabs(s->bias[0]) + dabs(s->bias[1]) + dabs(s->bias[2]);
    if (fault_metric > 5.0 * s->noise_std) {
        s->fault = 1;
        fus.fault_count++;
        print_str("[SF] FAULT detected sensor "); print_int(sensor_id);
        print_str(" type="); print_int(s->type);
        print_str(" metric="); print_int((int)(fault_metric * 100)); print_str("\n");
    } else {
        print_str("[SF] Sensor "); print_int(sensor_id); print_str(" healthy\n");
    }
}

void sf_time_sync(double sensor_time, double host_time) {
    tsync.sensor_time = sensor_time;
    tsync.timestamp = host_time;
    tsync.offset = host_time - sensor_time;
    if (tsync.sync_count > 0)
        tsync.drift_rate = tsync.offset / (tsync.sync_count + 1e-10);
    tsync.sync_count++;
    tsync.active = 1;
    fus.sync_active = 1;
    print_str("[SF] Time sync offset="); print_int((int)(tsync.offset * 1000));
    print_str("ms drift="); print_int((int)(tsync.drift_rate * 1e6));
    print_str("ppm count="); print_int(tsync.sync_count); print_str("\n");
}

void sf_delay_compensate(double delayed_val, double delay_ms) {
    if (dbuf.count < HIST_SIZE) {
        dbuf.hist_x[dbuf.head] = delayed_val;
        dbuf.hist_t[dbuf.head] = delay_ms;
        dbuf.head = (dbuf.head + 1) % HIST_SIZE;
        dbuf.count++;
    } else {
        dbuf.hist_x[dbuf.head] = delayed_val;
        dbuf.hist_t[dbuf.head] = delay_ms;
        dbuf.head = (dbuf.head + 1) % HIST_SIZE;
    }
    print_str("[SF] Delay compensate val="); print_int((int)(delayed_val * 100));
    print_str(" delay="); print_int((int)delay_ms); print_str("ms buf="); print_int(dbuf.count); print_str("\n");
}

void sf_data_association(int method) {
    fus.assoc_method = method;
    int n_tracks = 0;
    for (int i = 0; i < MAX_TRACKS; i++) if (tracks[i].active) n_tracks++;
    print_str("[SF] Data association method="); print_int(method);
    print_str(" tracks="); print_int(n_tracks); print_str("\n");
    if (method == ASSO_NN) {
        print_str("[SF] Nearest neighbor: min distance matching\n");
    } else if (method == ASSO_JC) {
        print_str("[SF] Joint compatible: gate validation\n");
    } else if (method == ASSO_MHT) {
        print_str("[SF] Multiple hypothesis: tree search\n");
    }
}

int sf_add_track(double x, double y, double vx, double vy) {
    for (int i = 0; i < MAX_TRACKS; i++) {
        if (!tracks[i].active) {
            tracks[i].x[0] = x; tracks[i].x[1] = vx;
            tracks[i].x[2] = y; tracks[i].x[3] = vy;
            tracks[i].vx[0] = vx; tracks[i].vx[1] = 0.0;
            tracks[i].vx[2] = vy; tracks[i].vx[3] = 0.0;
            for (int a = 0; a < STATE_DIM; a++)
                for (int b = 0; b < STATE_DIM; b++)
                    tracks[i].P[a][b] = (a == b) ? 1.0 : 0.0;
            tracks[i].weight = 1.0;
            tracks[i].id = fus.n_tracks;
            tracks[i].age = 0; tracks[i].n_meas = 1;
            tracks[i].active = 1;
            fus.n_tracks++;
            return tracks[i].id;
        }
    }
    return -1;
}

void sf_state_estimate(int type) {
    fus.est_type = type;
    double x, y, vx, vy, heading;
    if (fus.filter_type == FILT_EKF) {
        x = ekf.x[0]; y = ekf.x[2]; vx = ekf.x[1]; vy = ekf.x[3];
    } else if (fus.filter_type == FILT_UKF) {
        x = ukf.x[0]; y = ukf.x[2]; vx = ukf.x[1]; vy = ukf.x[3];
    } else if (fus.filter_type == FILT_PF) {
        x = 0.0; y = 0.0; vx = 0.0; vy = 0.0;
        for (int p = 0; p < pf.n_particles; p++) {
            x += pf.parts[p].w * pf.parts[p].x[0];
            y += pf.parts[p].w * pf.parts[p].x[2];
            vx += pf.parts[p].w * pf.parts[p].x[1];
            vy += pf.parts[p].w * pf.parts[p].x[3];
        }
    } else {
        double P_inv_00 = ifilt.info_mat[0][0] + 1e-10;
        x = ifilt.info_vec[0] / P_inv_00;
        double P_inv_22 = ifilt.info_mat[2][2] + 1e-10;
        y = ifilt.info_vec[2] / P_inv_22;
        vx = 0.0; vy = 0.0;
    }
    heading = 0.0;
    if (dabs(vx) > 0.001 || dabs(vy) > 0.001) heading = 0.0;
    double speed = dsqrt(vx * vx + vy * vy);
    print_str("[SF] State est: type="); print_int(type);
    print_str(" x="); print_int((int)(x * 100));
    print_str(" y="); print_int((int)(y * 100));
    print_str(" vx="); print_int((int)(vx * 100));
    print_str(" vy="); print_int((int)(vy * 100));
    print_str(" spd="); print_int((int)(speed * 100));
    print_str(" head="); print_int((int)(heading * 573)); print_str("\n");
}

void sf_print_status(void) {
    print_str("[SF] Filter="); print_int(fus.filter_type);
    print_str(" Sensors="); print_int(fus.n_sensors);
    print_str(" Tracks="); print_int(fus.n_tracks);
    print_str(" Assoc="); print_int(fus.assoc_method);
    print_str(" Updates="); print_int((int)fus.total_updates);
    print_str(" Fusions="); print_int((int)fus.total_fusions);
    print_str(" Faults="); print_int(fus.fault_count);
    print_str(" Sync="); print_int(fus.sync_active);
    print_str("\n");
}

int main(void) {
    print_str("=== Sensor Fusion Platform Demo ===\n\n");
    sf_init();

    print_str("\nAdding sensors...\n");
    sf_add_sensor(SENS_GPS, 2.5, 10.0);
    sf_add_sensor(SENS_IMU, 0.05, 100.0);
    sf_add_sensor(SENS_LIDAR, 0.1, 20.0);
    sf_add_sensor(SENS_CAM, 0.5, 30.0);
    sf_add_sensor(SENS_RADAR, 0.3, 10.0);
    sf_add_sensor(SENS_BARO, 0.1, 50.0);

    print_str("\nConfiguring EKF...\n");
    sf_configure_ekf(0.1, 0.1, 1.0);

    print_str("\nEKF predict/update cycle...\n");
    for (int i = 0; i < 5; i++) {
        sf_ekf_predict();
        sf_ekf_update(10.0 + i * 0.5, 0.0, 5.0 + i * 0.2);
    }
    print_str("  EKF x="); print_int((int)(ekf.x[0] * 100));
    print_str(" y="); print_int((int)(ekf.x[2] * 100));
    print_str(" P00="); print_int((int)(ekf.P[0][0] * 1000)); print_str("\n");

    print_str("\nUKF initialization and sigma points...\n");
    sf_ukf_init();
    sf_ukf_sigma_points();
    sf_ukf_predict();

    print_str("\nParticle filter...\n");
    sf_pf_init(15);
    for (int i = 0; i < 3; i++) {
        sf_pf_predict();
        sf_pf_update(10.0 + i * 0.3, 5.0 + i * 0.1);
    }

    print_str("\nInformation filter...\n");
    sf_info_filter_init();
    for (int i = 0; i < 5; i++)
        sf_info_filter_update(10.0 + i * 0.2, 5.0 + i * 0.1);

    print_str("\nSensor fault detection...\n");
    for (int i = 0; i < fus.n_sensors; i++) sf_check_sensor_fault(i);
    sensors[0].bias[0] = 15.0;
    sf_check_sensor_fault(0);

    print_str("\nTime synchronization...\n");
    sf_time_sync(100.0, 100.5);
    sf_time_sync(200.0, 200.7);
    sf_time_sync(300.0, 300.9);

    print_str("\nDelay compensation...\n");
    sf_delay_compensate(5.5, 50.0);
    sf_delay_compensate(5.8, 48.0);
    sf_delay_compensate(6.1, 45.0);

    print_str("\nAdding tracks...\n");
    sf_add_track(10.0, 5.0, 2.0, 0.5);
    sf_add_track(20.0, 15.0, -1.0, 1.0);
    sf_add_track(5.0, 25.0, 0.5, -0.5);

    print_str("\nData association methods...\n");
    sf_data_association(ASSO_NN);
    sf_data_association(ASSO_JC);
    sf_data_association(ASSO_MHT);

    print_str("\nState estimation (EKF)...\n");
    sf_state_estimate(STAT_POS);
    sf_state_estimate(STAT_VEL);
    sf_state_estimate(STAT_HEAD);

    sf_print_status();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
