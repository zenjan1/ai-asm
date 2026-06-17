/*
 * BCI Interface Module - AI-ASM OS (AArch64 WebAssembly-native)
 * Version: BCI Interface v55.0 - Brain-Computer Interface Applications
 * No standard library - uses only host imports and manual implementations.
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

#define PI 3.14159265358979323846
#define MAX_CH 64
#define MAX_SAMP 2048
#define BUF_SZ 256
#define MAX_SESSIONS 32
#define MAX_ANNOT 128
#define MAX_USERS 16
#define VERSION "BCI Interface v55.0 - Brain-Computer Interface Applications"

/* ── Utility Functions ── */
static unsigned int my_strlen(const char *s) { unsigned int l = 0; while (s[l]) l++; return l; }
static void my_strncpy(char *d, const char *s, unsigned int n) {
    unsigned int i = 0; while (i < n && s[i]) { d[i] = s[i]; i++; } while (i < n) { d[i] = '\0'; i++; }
}
static void print_str(const char *s) { host_print(s); }
static void print_int(int v) {
    char b[16]; int i = 0;
    if (v < 0) { host_print("-"); v = -v; } if (v == 0) { host_print("0"); return; }
    while (v > 0 && i < 15) { b[i++] = '0' + (v % 10); v /= 10; }
    char o[16]; for (int j = 0; j < i; j++) o[j] = b[i - 1 - j]; o[i] = '\0'; host_print(o);
}
static void print_double(double v) {
    if (v < 0) { host_print("-"); v = -v; }
    int w = (int)v; double f = v - (double)w; print_int(w); host_print(".");
    for (int i = 0; i < 4; i++) { f *= 10.0; int d = (int)f; char c[2] = { (char)('0' + d), '\0' }; host_print(c); f -= (double)d; }
}
static double my_abs(double x) { return x < 0 ? -x : x; }
static double my_sqrt(double x) {
    if (x <= 0) return 0.0; double g = x / 2.0;
    for (int i = 0; i < 30; i++) g = (g + x / g) / 2.0; return g;
}
static double my_sin(double x) {
    while (x > PI) x -= 2.0 * PI; while (x < -PI) x += 2.0 * PI;
    double t = x, s = x;
    for (int i = 1; i <= 10; i++) { t *= -x * x / ((2.0 * i) * (2.0 * i + 1.0)); s += t; } return s;
}
static double my_cos(double x) { return my_sin(x + PI / 2.0); }
static unsigned int lcg_state = 54321;
static double srand_val(void) {
    lcg_state = lcg_state * 1103515245 + 12345;
    return (double)((lcg_state >> 16) & 0x7FFF) / 32767.0;
}
static int clamp_int(int v, int lo, int hi) { return (v < lo) ? lo : (v > hi) ? hi : v; }
static double clamp_d(double v, double lo, double hi) { return (v < lo) ? lo : (v > hi) ? hi : v; }

/* ── Data Structures ── */
typedef struct { double alpha, beta, theta, delta, gamma; } band_power_t;
typedef struct { int x, y; double vel_x, vel_y; int click_state; } cursor_state_t;
typedef struct { char buffer[64]; int pos; int commit_count; } typing_state_t;
typedef struct { int device_id; int power; int brightness; double temperature; } device_state_t;
typedef struct { double heading; double speed; int mode; double waypoint_x; double waypoint_y; } wheelchair_state_t;
typedef struct { int patient_id; int phase; double motor_score; double cognitive_score; int sessions_completed; } rehab_state_t;
typedef struct { int game_id; int level; double score; double reaction_ms; int accuracy; } game_state_t;
typedef struct { int task_type; int difficulty; double mem_score; double learn_rate; double focus_score; } cognitive_state_t;
typedef struct { int alert_level; double fatigue_score; double attention_score; double stress_score; } safety_state_t;
typedef struct { int session_id; int user_id; int duration_sec; int status; double quality; } session_info_t;
typedef struct { int label; int timestamp; int channel; double confidence; } annotation_t;
typedef struct { int model_id; int type; int epoch; double loss; double accuracy; int trained; } model_info_t;
typedef struct { int user_id; char key[16]; int perm_level; int active; double entropy; } privacy_entry_t;

/* ── Global State ── */
static band_power_t g_bands = { 0 };
static cursor_state_t g_cursor = { 0, 0, 0.0, 0.0, 0 };
static typing_state_t g_typing = { {0}, 0, 0 };
static device_state_t g_devices[16];
static wheelchair_state_t g_wheelchair = { 0.0, 0.0, 0, 0.0, 0.0 };
static rehab_state_t g_rehab = { 0, 0, 0.0, 0.0, 0 };
static game_state_t g_games[8];
static cognitive_state_t g_cognitive = { 0, 1, 0.0, 0.0, 0.0 };
static safety_state_t g_safety = { 0, 0.0, 100.0, 0.0 };
static session_info_t g_sessions[MAX_SESSIONS];
static annotation_t g_annotations[MAX_ANNOT];
static model_info_t g_models[8];
static privacy_entry_t g_privacy[MAX_USERS];
static int g_n_sessions = 0;
static int g_n_annotations = 0;
static int g_n_models = 0;

/* ── 1. Signal Processing Core ── */
static band_power_t compute_band_power(double d[], int len, int sr) {
    band_power_t bp = { 0 };
    int n = 1; while (n < len && n < 512) n <<= 1;
    double re[512], im[512];
    for (int i = 0; i < n; i++) { re[i] = (i < len) ? d[i] : 0; im[i] = 0; }
    for (int i = 1, j = 0; i < n; i++) {
        int b = n >> 1; while (j & b) { j ^= b; b >>= 1; } j ^= b;
        if (i < j) { double t = re[i]; re[i] = re[j]; re[j] = t; t = im[i]; im[i] = im[j]; im[j] = t; }
    }
    for (int l = 2; l <= n; l <<= 1) {
        double a = -2.0 * PI / (double)l, wr = my_cos(a), wi = my_sin(a);
        for (int i = 0; i < n; i += l) {
            double cr = 1.0, ci = 0;
            for (int j = 0; j < l / 2; j++) {
                double ur = re[i + j], ui = im[i + j];
                double vr = re[i + j + l / 2] * cr - im[i + j + l / 2] * ci;
                double vi = re[i + j + l / 2] * ci + im[i + j + l / 2] * cr;
                re[i + j] = ur + vr; im[i + j] = ui + vi;
                re[i + j + l / 2] = ur - vr; im[i + j + l / 2] = ui - vi;
                double nr = cr * wr - ci * wi; ci = cr * wi + ci * wr; cr = nr;
            }
        }
    }
    double fr = (double)sr / (double)n;
    for (int i = 1; i < n / 2; i++) {
        double p = re[i] * re[i] + im[i] * im[i], fq = (double)i * fr;
        double total = bp.delta + bp.theta + bp.alpha + bp.beta + bp.gamma + p;
        if (fq >= 0.5 && fq < 4.0) bp.delta += p;
        else if (fq >= 4.0 && fq < 8.0) bp.theta += p;
        else if (fq >= 8.0 && fq < 13.0) bp.alpha += p;
        else if (fq >= 13.0 && fq < 30.0) bp.beta += p;
        else if (fq >= 30.0 && fq < 50.0) bp.gamma += p;
        (void)total;
    }
    return bp;
}
static void apply_bandpass(double d[], int len, double lo, double hi, int sr) {
    double rc_l = 1.0 / (2.0 * PI * lo), dt = 1.0 / (double)sr, a_l = rc_l / (rc_l + dt);
    double rc_h = 1.0 / (2.0 * PI * hi), a_h = dt / (rc_h + dt);
    double pi2 = d[0], po2 = d[0];
    for (int i = 0; i < len; i++) { double o = a_h * (po2 + d[i] - pi2); pi2 = d[i]; po2 = o; d[i] = o; }
    double p = d[0];
    for (int i = 0; i < len; i++) { double o = p + a_l * (d[i] - p); p = o; d[i] = o; }
}
static double compute_correlation(double a[], double b[], int len) {
    double sa = 0, sb = 0;
    for (int i = 0; i < len; i++) { sa += a[i]; sb += b[i]; }
    double ma = sa / (double)len, mb = sb / (double)len;
    double sxy = 0, sxx = 0, syy = 0;
    for (int i = 0; i < len; i++) { double da = a[i] - ma, db = b[i] - mb; sxy += da * db; sxx += da * da; syy += db * db; }
    double dn = my_sqrt(sxx * syy); return (dn > 0) ? sxy / dn : 0;
}

/* ── 2. Control Applications ── */
static void cursor_init(int screen_w, int screen_h) {
    g_cursor.x = screen_w / 2; g_cursor.y = screen_h / 2;
    g_cursor.vel_x = 0; g_cursor.vel_y = 0; g_cursor.click_state = 0;
    print_str("[CTRL] Cursor init at "); print_int(g_cursor.x); print_str("x"); print_int(g_cursor.y); print_str("\n");
}
static void cursor_update(double alpha_ratio, double beta_ratio, int screen_w, int screen_h) {
    double dx = (beta_ratio - 0.5) * 10.0, dy = (alpha_ratio - 0.5) * 10.0;
    g_cursor.vel_x = g_cursor.vel_x * 0.7 + dx * 0.3;
    g_cursor.vel_y = g_cursor.vel_y * 0.7 + dy * 0.3;
    g_cursor.x = clamp_int(g_cursor.x + (int)g_cursor.vel_x, 0, screen_w - 1);
    g_cursor.y = clamp_int(g_cursor.y + (int)g_cursor.vel_y, 0, screen_h - 1);
    print_str("[CTRL] Cursor at "); print_int(g_cursor.x); print_str(","); print_int(g_cursor.y); print_str("\n");
}
static int typing_input(double p300_scores[], int n_chars, char *out) {
    int best = 0; double best_s = p300_scores[0];
    for (int i = 1; i < n_chars; i++) if (p300_scores[i] > best_s) { best_s = p300_scores[i]; best = i; }
    char c = (char)('A' + best); out[0] = c; out[1] = '\0';
    if (g_typing.pos < 63) { g_typing.buffer[g_typing.pos++] = c; g_typing.buffer[g_typing.pos] = '\0'; }
    g_typing.commit_count++;
    print_str("[CTRL] Typed: "); char s[2] = { c, '\0' }; print_str(s);
    print_str(" buffer="); print_str(g_typing.buffer); print_str("\n"); return best;
}
static void device_control(int dev_id, int cmd, double value) {
    if (dev_id < 0 || dev_id >= 16) return;
    if (cmd == 0) g_devices[dev_id].power = (value > 0.5) ? 1 : 0;
    else if (cmd == 1) g_devices[dev_id].brightness = clamp_int((int)value, 0, 100);
    else if (cmd == 2) g_devices[dev_id].temperature = clamp_d(value, 16.0, 30.0);
    print_str("[CTRL] Device "); print_int(dev_id); print_str(" cmd="); print_int(cmd);
    print_str(" val="); print_double(value); print_str("\n");
}
static void smart_home_command(int room_id, int action, double intensity) {
    const char *rooms[] = { "LIVING", "BEDROOM", "KITCHEN", "BATHROOM", "OFFICE" };
    const char *actions[] = { "LIGHTS", "TEMP", "BLINDS", "MUSIC", "SECURITY" };
    print_str("[CTRL] SmartHome "); print_str(rooms[room_id % 5]); print_str(" ");
    print_str(actions[action % 5]); print_str(" intensity="); print_double(intensity); print_str("\n");
}
static void wheelchair_navigate(double heading_intent, double speed_intent, double obstacle_dist) {
    g_wheelchair.heading = heading_intent * 360.0;
    g_wheelchair.speed = clamp_d(speed_intent * 5.0, 0.0, 5.0);
    if (obstacle_dist < 0.5) { g_wheelchair.speed *= 0.2; g_wheelchair.mode = 2; }
    else if (obstacle_dist < 2.0) { g_wheelchair.speed *= 0.5; g_wheelchair.mode = 1; }
    else { g_wheelchair.mode = 0; }
    print_str("[CTRL] Wheelchair hdg="); print_double(g_wheelchair.heading);
    print_str(" spd="); print_double(g_wheelchair.speed);
    print_str(" mode="); print_int(g_wheelchair.mode); print_str("\n");
}

/* ── 3. Medical Rehabilitation ── */
static void rehab_stroke_session(double motor_signal[], int len, int target_angle) {
    double mean = 0; for (int i = 0; i < len; i++) mean += motor_signal[i]; mean /= (double)len;
    double effort = my_abs(mean) / 100.0 * 100.0;
    g_rehab.motor_score = g_rehab.motor_score * 0.9 + effort * 0.1;
    int success = (effort > (double)target_angle * 0.8) ? 1 : 0;
    g_rehab.phase = success ? 2 : 1;
    print_str("[MED] Stroke: effort="); print_double(effort);
    print_str(" target="); print_int(target_angle);
    print_str(success ? " SUCCESS" : " RETRY"); print_str("\n");
}
static int rehab_epilepsy_predict(double eeg_features[], int n_feat, double threshold) {
    double risk = 0; for (int i = 0; i < n_feat; i++) risk += eeg_features[i] * (0.5 + srand_val() * 0.5);
    risk /= (double)n_feat;
    int warning = (risk > threshold) ? 1 : 0;
    print_str("[MED] Epilepsy risk="); print_double(risk);
    print_str(warning ? " WARNING!" : " normal"); print_str("\n"); return warning;
}
static double rehab_depression_treatment(double alpha_asymmetry, double session_count) {
    double improvement = alpha_asymmetry * 0.3 + my_sqrt(session_count) * 2.0;
    improvement = clamp_d(improvement, 0.0, 100.0);
    print_str("[MED] Depression tx: asymm="); print_double(alpha_asymmetry);
    print_str(" sessions="); print_double(session_count);
    print_str(" improvement="); print_double(improvement); print_str("\n"); return improvement;
}
static void rehab_pain_management(double pain_signal, int stimulation_level) {
    double relief = pain_signal * (1.0 - (double)stimulation_level / 10.0);
    relief = clamp_d(relief, 0.0, 100.0);
    print_str("[MED] Pain: signal="); print_double(pain_signal);
    print_str(" stim="); print_int(stimulation_level);
    print_str(" relief="); print_double(100.0 - relief); print_str("%\n");
}
static double rehab_sleep_improvement(double delta_power, double sleep_duration_hr) {
    double quality = delta_power * 10.0 + sleep_duration_hr * 5.0;
    quality = clamp_d(quality, 0.0, 100.0);
    print_str("[MED] Sleep: delta="); print_double(delta_power);
    print_str(" dur="); print_double(sleep_duration_hr);
    print_str(" quality="); print_double(quality); print_str("\n"); return quality;
}

/* ── 4. Gaming Interaction ── */
static void game_mind_control(int game_id, double concentration, double move_dir) {
    if (game_id < 0 || game_id >= 8) return;
    g_games[game_id].game_id = game_id;
    g_games[game_id].score += concentration * 10.0;
    g_games[game_id].level = (int)(g_games[game_id].score / 100.0) + 1;
    print_str("[GAME] Mind ctrl id="); print_int(game_id);
    print_str(" conc="); print_double(concentration);
    print_str(" dir="); print_double(move_dir);
    print_str(" score="); print_double(g_games[game_id].score); print_str("\n");
}
static int game_attention_task(int target, int response, double time_ms) {
    int correct = (target == response) ? 1 : 0;
    g_cognitive.focus_score = g_cognitive.focus_score * 0.8 + (correct ? 100.0 : 0.0) * 0.2;
    g_games[0].reaction_ms = time_ms;
    g_games[0].accuracy = correct ? 100 : 0;
    print_str("[GAME] Attention: "); print_str(correct ? "CORRECT" : "MISS");
    print_str(" rt="); print_double(time_ms); print_str("ms\n"); return correct;
}
static void game_emotion_interaction(double valence, double arousal) {
    const char *states[] = { "CALM", "HAPPY", "EXCITED", "TENSE", "SAD", "FOCUSED" };
    int idx = 0;
    if (valence > 0.5 && arousal < 0.5) idx = 1;
    else if (valence > 0.5 && arousal >= 0.5) idx = 2;
    else if (valence < 0 && arousal >= 0.5) idx = 3;
    else if (valence < 0 && arousal < 0.5) idx = 4;
    else if (valence >= 0 && arousal >= 0.3) idx = 5;
    print_str("[GAME] Emotion: val="); print_double(valence);
    print_str(" aro="); print_double(arousal);
    print_str(" state="); print_str(states[idx]); print_str("\n");
}
static void game_multiplayer_sync(int player_id, double intent[], int n_players) {
    print_str("[GAME] Multiplayer sync: player="); print_int(player_id);
    print_str(" n="); print_int(n_players);
    for (int i = 0; i < n_players && i < 4; i++) {
        print_str(" p"); print_int(i); print_str("="); print_double(intent[i]);
    }
    print_str("\n");
}

/* ── 5. Cognitive Enhancement ── */
static void cog_memory_enhancement(int n_items, double recall_accuracy) {
    g_cognitive.task_type = 0;
    g_cognitive.mem_score = g_cognitive.mem_score * 0.85 + recall_accuracy * 0.15;
    print_str("[COG] Memory: items="); print_int(n_items);
    print_str(" recall="); print_double(recall_accuracy);
    print_str(" score="); print_double(g_cognitive.mem_score); print_str("\n");
}
static double cog_learning_acceleration(double difficulty, double success_rate, int trial) {
    g_cognitive.task_type = 1;
    double adapt = difficulty * (0.8 + success_rate * 0.4);
    g_cognitive.learn_rate = g_cognitive.learn_rate * 0.9 + adapt * 0.1;
    print_str("[COG] Learning: diff="); print_double(difficulty);
    print_str(" success="); print_double(success_rate);
    print_str(" trial="); print_int(trial);
    print_str(" rate="); print_double(g_cognitive.learn_rate); print_str("\n"); return adapt;
}
static double cog_attention_training(double focus_signal[], int len) {
    g_cognitive.task_type = 2;
    double mean = 0; for (int i = 0; i < len; i++) mean += focus_signal[i]; mean /= (double)len;
    double var = 0; for (int i = 0; i < len; i++) { double d = focus_signal[i] - mean; var += d * d; }
    var /= (double)len;
    double stability = 100.0 - my_sqrt(var) * 10.0;
    stability = clamp_d(stability, 0.0, 100.0);
    g_cognitive.focus_score = g_cognitive.focus_score * 0.85 + stability * 0.15;
    print_str("[COG] Attention train: stability="); print_double(stability);
    print_str(" focus="); print_double(g_cognitive.focus_score); print_str("\n"); return stability;
}
static double cog_creativity_stimulation(double divergent_score, double novelty) {
    g_cognitive.task_type = 3;
    double creativity = divergent_score * 0.6 + novelty * 0.4;
    creativity = clamp_d(creativity, 0.0, 100.0);
    print_str("[COG] Creativity: div="); print_double(divergent_score);
    print_str(" nov="); print_double(novelty);
    print_str(" score="); print_double(creativity); print_str("\n"); return creativity;
}

/* ── 6. Safety Protection ── */
static int safety_driving_fatigue(double blink_rate, double eye_closure, double head_nod) {
    g_safety.fatigue_score = blink_rate * 30.0 + eye_closure * 40.0 + head_nod * 30.0;
    g_safety.fatigue_score = clamp_d(g_safety.fatigue_score, 0.0, 100.0);
    if (g_safety.fatigue_score > 70.0) g_safety.alert_level = 3;
    else if (g_safety.fatigue_score > 40.0) g_safety.alert_level = 2;
    else if (g_safety.fatigue_score > 20.0) g_safety.alert_level = 1;
    else g_safety.alert_level = 0;
    print_str("[SAFE] Driving: fatigue="); print_double(g_safety.fatigue_score);
    print_str(" alert="); print_int(g_safety.alert_level); print_str("\n");
    return g_safety.alert_level;
}
static int safety_attention_monitor(double attention_level, double duration_min) {
    g_safety.attention_score = attention_level;
    int degrade = 0;
    if (attention_level < 30.0 || duration_min > 45.0) { degrade = 1; g_safety.alert_level = 2; }
    else if (attention_level < 50.0) { g_safety.alert_level = 1; }
    else { g_safety.alert_level = 0; }
    print_str("[SAFE] Attention: lvl="); print_double(attention_level);
    print_str(" dur="); print_double(duration_min);
    print_str(degrade ? " DEGRADED" : " OK"); print_str("\n"); return degrade;
}
static int safety_emotional_warning(double stress_index, double heart_variability) {
    g_safety.stress_score = stress_index * 60.0 + (100.0 - heart_variability) * 0.4;
    g_safety.stress_score = clamp_d(g_safety.stress_score, 0.0, 100.0);
    int warning = 0;
    if (g_safety.stress_score > 70.0) { warning = 2; print_str("[SAFE] Emotion: HIGH STRESS\n"); }
    else if (g_safety.stress_score > 40.0) { warning = 1; print_str("[SAFE] Emotion: ELEVATED\n"); }
    else { print_str("[SAFE] Emotion: NORMAL\n"); }
    return warning;
}
static int safety_anomaly_detection(double signal[], int len, double baseline_mean, double baseline_std) {
    int anomalies = 0;
    double threshold = baseline_mean + 3.0 * baseline_std;
    for (int i = 0; i < len; i++) {
        if (my_abs(signal[i] - baseline_mean) > threshold) anomalies++;
    }
    int alert = (anomalies > len / 10) ? 1 : 0;
    print_str("[SAFE] Anomaly: "); print_int(anomalies); print_str("/"); print_int(len);
    print_str(alert ? " ALERT\n" : " normal\n"); return alert;
}

/* ── 7. Data Management ── */
static int session_create(int user_id, int duration_sec) {
    if (g_n_sessions >= MAX_SESSIONS) return -1;
    int id = g_n_sessions++;
    g_sessions[id].session_id = id;
    g_sessions[id].user_id = user_id;
    g_sessions[id].duration_sec = duration_sec;
    g_sessions[id].status = 1;
    g_sessions[id].quality = 0.0;
    print_str("[DATA] Session "); print_int(id); print_str(" created user="); print_int(user_id);
    print_str(" dur="); print_int(duration_sec); print_str("s\n"); return id;
}
static void session_complete(int sid, double quality) {
    if (sid < 0 || sid >= g_n_sessions) return;
    g_sessions[sid].status = 2;
    g_sessions[sid].quality = clamp_d(quality, 0.0, 100.0);
    print_str("[DATA] Session "); print_int(sid); print_str(" completed q="); print_double(quality); print_str("\n");
}
static int annotate_data(int label, int timestamp, int channel, double confidence) {
    if (g_n_annotations >= MAX_ANNOT) return -1;
    int id = g_n_annotations++;
    g_annotations[id].label = label;
    g_annotations[id].timestamp = timestamp;
    g_annotations[id].channel = channel;
    g_annotations[id].confidence = clamp_d(confidence, 0.0, 1.0);
    print_str("[DATA] Annotation "); print_int(id); print_str(" label="); print_int(label);
    print_str(" ch="); print_int(channel); print_str(" conf="); print_double(confidence); print_str("\n"); return id;
}
static int model_train(int type, double data[], int n_samples, int epochs) {
    if (g_n_models >= 8) return -1;
    int mid = g_n_models++;
    g_models[mid].model_id = mid;
    g_models[mid].type = type;
    g_models[mid].epoch = 0;
    g_models[mid].loss = 1.0;
    g_models[mid].accuracy = 0.0;
    g_models[mid].trained = 0;
    for (int e = 0; e < epochs; e++) {
        g_models[mid].loss *= (0.95 - srand_val() * 0.05);
        g_models[mid].accuracy += (1.0 - g_models[mid].accuracy) * (0.1 + srand_val() * 0.05);
        g_models[mid].epoch = e + 1;
    }
    g_models[mid].trained = 1;
    print_str("[DATA] Model "); print_int(mid); print_str(" type="); print_int(type);
    print_str(" epochs="); print_int(epochs);
    print_str(" loss="); print_double(g_models[mid].loss);
    print_str(" acc="); print_double(g_models[mid].accuracy * 100.0); print_str("%\n"); return mid;
}
static void effect_evaluate(int session_ids[], int n_sess) {
    double total_q = 0;
    for (int i = 0; i < n_sess; i++) {
        int s = session_ids[i];
        if (s >= 0 && s < g_n_sessions) total_q += g_sessions[s].quality;
    }
    double avg = (n_sess > 0) ? total_q / (double)n_sess : 0;
    print_str("[DATA] Evaluation: "); print_int(n_sess);
    print_str(" sessions, avg_quality="); print_double(avg); print_str("\n");
}
static void privacy_protect(int user_id, const char *key, int perm_level) {
    if (user_id < 0 || user_id >= MAX_USERS) return;
    g_privacy[user_id].user_id = user_id;
    my_strncpy(g_privacy[user_id].key, key, 15);
    g_privacy[user_id].perm_level = perm_level;
    g_privacy[user_id].active = 1;
    g_privacy[user_id].entropy = 4.5 + srand_val() * 2.5;
    print_str("[DATA] Privacy: user="); print_int(user_id);
    print_str(" perm="); print_int(perm_level);
    print_str(" entropy="); print_double(g_privacy[user_id].entropy); print_str("\n");
}

/* ── 8. Entry Point ── */
static void print_help(void) {
    print_str(VERSION);
    print_str("\n\nUsage: bci_interface [options]\n");
    print_str("  -h    Show help\n  -t    Run test/demo\n\n");
    print_str("Modules: Control Apps, Medical Rehab, Gaming, Cognitive,\n");
    print_str("  Safety Protection, Data Management\n");
}
static void run_test(void) {
    print_str("=== "); print_str(VERSION); print_str(" ===\n\n");
    /* Signal Processing */
    print_str("--- Signal Processing ---\n");
    double raw[MAX_SAMP];
    for (int i = 0; i < MAX_SAMP; i++)
        raw[i] = 20.0 * my_sin(2.0 * PI * 10.0 * (double)i / 256.0)
               + 8.0 * my_sin(2.0 * PI * 20.0 * (double)i / 256.0)
               + (srand_val() - 0.5) * 6.0;
    apply_bandpass(raw, 1024, 0.5, 50.0, 256);
    print_str("  Bandpass applied\n");
    band_power_t bp = compute_band_power(raw, 1024, 256);
    print_str("  Bands: a="); print_double(bp.alpha); print_str(" b="); print_double(bp.beta);
    print_str(" th="); print_double(bp.theta); print_str(" d="); print_double(bp.delta);
    print_str(" g="); print_double(bp.gamma); print_str("\n");
    /* Control Apps */
    print_str("\n--- Control Applications ---\n");
    cursor_init(1920, 1080);
    cursor_update(0.6, 0.7, 1920, 1080);
    char tbuf[2]; double p300_sc[] = { 0.2, 0.8, 0.3, 0.1, 0.5, 0.9, 0.4, 0.6, 0.7, 0.15 };
    typing_input(p300_sc, 10, tbuf);
    for (int i = 0; i < 4; i++) { g_devices[i].device_id = i; g_devices[i].power = 0; }
    device_control(0, 0, 1.0); device_control(0, 1, 75.0); device_control(1, 2, 22.0);
    smart_home_command(0, 0, 0.8); smart_home_command(2, 2, 0.5);
    wheelchair_navigate(0.25, 0.6, 3.0); wheelchair_navigate(0.5, 0.8, 0.3);
    /* Medical Rehab */
    print_str("\n--- Medical Rehabilitation ---\n");
    double motor_sig[64]; for (int i = 0; i < 64; i++) motor_sig[i] = srand_val() * 80.0;
    g_rehab.patient_id = 1001;
    rehab_stroke_session(motor_sig, 64, 45);
    double eeg_feat[8]; for (int i = 0; i < 8; i++) eeg_feat[i] = srand_val() * 2.0;
    rehab_epilepsy_predict(eeg_feat, 8, 1.2);
    rehab_depression_treatment(0.3, 12.0);
    rehab_pain_management(65.0, 7);
    rehab_sleep_improvement(2.5, 7.5);
    /* Gaming */
    print_str("\n--- Gaming Interaction ---\n");
    game_mind_control(0, 0.75, 0.3);
    game_mind_control(1, 0.60, -0.5);
    game_attention_task(3, 3, 320.0);
    game_attention_task(5, 2, 450.0);
    game_emotion_interaction(0.7, 0.4);
    game_emotion_interaction(-0.3, 0.8);
    double intents[4] = { 0.5, 0.8, 0.3, 0.9 };
    game_multiplayer_sync(0, intents, 4);
    /* Cognitive Enhancement */
    print_str("\n--- Cognitive Enhancement ---\n");
    cog_memory_enhancement(7, 85.0);
    cog_memory_enhancement(9, 78.0);
    cog_learning_acceleration(0.7, 0.85, 15);
    cog_learning_acceleration(0.8, 0.90, 16);
    double focus_sig[32]; for (int i = 0; i < 32; i++) focus_sig[i] = 60.0 + srand_val() * 20.0;
    cog_attention_training(focus_sig, 32);
    cog_creativity_stimulation(72.0, 65.0);
    /* Safety Protection */
    print_str("\n--- Safety Protection ---\n");
    safety_driving_fatigue(0.4, 0.6, 0.3);
    safety_driving_fatigue(0.8, 0.9, 0.7);
    safety_attention_monitor(65.0, 30.0);
    safety_attention_monitor(25.0, 50.0);
    safety_emotional_warning(0.5, 70.0);
    safety_emotional_warning(0.9, 30.0);
    double anom_sig[32]; for (int i = 0; i < 32; i++) anom_sig[i] = 50.0 + (srand_val() - 0.5) * 10.0;
    anom_sig[5] = 120.0; anom_sig[20] = -30.0;
    safety_anomaly_detection(anom_sig, 32, 50.0, 5.0);
    /* Data Management */
    print_str("\n--- Data Management ---\n");
    int s1 = session_create(1, 1800);
    int s2 = session_create(1, 2400);
    int s3 = session_create(2, 1200);
    session_complete(s1, 88.5);
    session_complete(s2, 92.0);
    session_complete(s3, 76.5);
    annotate_data(1, 1000, 0, 0.95);
    annotate_data(2, 2000, 3, 0.87);
    annotate_data(1, 3000, 5, 0.92);
    double train_data[64]; for (int i = 0; i < 64; i++) train_data[i] = srand_val() * 10.0;
    model_train(0, train_data, 64, 50);
    model_train(1, train_data, 64, 30);
    int eval_ids[] = { 0, 1, 2 };
    effect_evaluate(eval_ids, 3);
    privacy_protect(0, "aes256key1", 3);
    privacy_protect(1, "rsa4096key", 2);
    print_str("\n=== All BCI Interface tests completed ===\n");
}
void _start(void) {
    char ab[BUF_SZ];
    int al = host_get_argv(0, BUF_SZ);
    int tm = 0, hm = 0;
    if (al > 0) {
        host_get_argv(0, BUF_SZ);
        for (int i = 0; i < al; i++) {
            if (ab[i] == '-' && i + 1 < al) {
                if (ab[i + 1] == 't') tm = 1; if (ab[i + 1] == 'h') hm = 1;
            }
        }
    }
    if (hm) { print_help(); host_exit(0); }
    if (tm) { run_test(); host_exit(0); }
    print_str(VERSION); print_str("\nUse -h for help, -t for test\n");
    host_exit(0);
}
