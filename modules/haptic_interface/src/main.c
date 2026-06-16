/*
 * haptic_interface: Haptic Feedback Interface (v54.0)
 * Features: Force feedback, texture simulation, temperature control,
 *           haptic rendering, multimodal sync, haptic effect library
 * Build: clang --target=wasm32-unknown-unknown -nostdlib -Wl,--no-entry \
 *        -Wl,--export-all -o haptic_interface.wasm src/main.c
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

/* ── Constants ── */

#define MAX_EFFECTS 32
#define MAX_WAVEFORM_SAMPLES 256
#define MAX_DEVICES 4
#define MAX_SEQUENCE 64
#define MAX_MATERIALS 16
#define MAX_TOOLS 8
/* Capabilities */
#define CAP_FORCE    0x01
#define CAP_TEXTURE  0x02
#define CAP_THERMAL  0x04
#define CAP_VIBRATE  0x08
#define CAP_ALL      0x0F
/* Effect types */
#define FX_CLICK 0
#define FX_BUZZ  1
#define FX_PULSE 2
#define FX_WAVE  3
#define FX_TEX_CLICK 4
#define FX_CUSTOM 5
/* Texture pattern types */
#define PAT_SMOOTH  0
#define PAT_ROUGH   1
#define PAT_GRANULAR 2
#define PAT_RIBBED  3
#define PAT_STIPPLED 4
/* Thermal modes */
#define THERM_NEUTRAL 0
#define THERM_HEAT    1
#define THERM_COOL    2
/* Tool types */
#define TOOL_SCALPEL 0
#define TOOL_BRUSH   1
#define TOOL_POINTER 2
/* Rendering modes */
#define RENDER_IMPEDANCE  0
#define RENDER_ADMITTANCE 1

/* ── Data structures ── */

typedef struct { double x, y, z; } vec3_t;
typedef struct { vec3_t position, force_output; double temperature;
    int active_effects; unsigned int capabilities; } haptic_device_t;
typedef struct { double stiffness, damping; vec3_t force_vector, contact_point,
    surface_normal; } force_feedback_t;
typedef struct { double roughness, smoothness, stickiness, frequency, amplitude;
    int pattern_type; } texture_profile_t;
typedef struct { double current_temp, target_temp, ramp_rate; int mode; } thermal_state_t;
typedef struct { int type; double amplitude, frequency, duration;
    double attack, decay, sustain, release; } haptic_effect_t;
typedef struct { int sample_count; double samples[MAX_WAVEFORM_SAMPLES];
    int playback_position, loop; } waveform_t;
typedef struct { double visual_latency_ms, audio_latency_ms, haptic_latency_ms,
    sync_offset; } multimodal_sync_t;
typedef struct { double bump_height, ridge_width, hole_depth, edge_sharpness; } haptic_surface_t;
typedef struct { int type; double tip_radius, stiffness, friction; } haptic_tool_t;
typedef struct { int effect_ids[MAX_SEQUENCE], count; double total_duration; } effect_sequence_t;

/* ── Globals ── */

static unsigned int heap_pos = 65536;
static haptic_device_t devices[MAX_DEVICES]; static int device_count = 0;
static haptic_effect_t effects[MAX_EFFECTS]; static int effect_count = 0;
static waveform_t waveforms[MAX_EFFECTS]; static int waveform_count = 0;
static texture_profile_t materials[MAX_MATERIALS]; static int material_count = 0;
static haptic_tool_t tools[MAX_TOOLS]; static int tool_count = 0;
static multimodal_sync_t sync_state;
static force_feedback_t force_state;
static thermal_state_t thermal_state;
static haptic_surface_t surface;
static effect_sequence_t sequence;

/* ── Utility functions ── */

static unsigned int my_strlen(const char *s) { unsigned int l = 0; while (s[l]) l++; return l; }
static void my_strncpy(char *d, const char *s, unsigned int m) {
    unsigned int i = 0; while (s[i] && i < m - 1) { d[i] = s[i]; i++; } d[i] = 0; }
static unsigned int alloc_buf(unsigned int n) {
    unsigned int p = heap_pos; heap_pos += n; heap_pos = (heap_pos + 15) & ~15u; return p; }
static void copy_to_mem(const char *s, unsigned int d, unsigned int l) {
    char *p = (char *)d; for (unsigned int i = 0; i < l; i++) p[i] = s[i]; }
static void print_str(const char *s) {
    unsigned int l = my_strlen(s), b = alloc_buf(l + 1); copy_to_mem(s, b, l + 1); host_print((const char *)b); }
static void print_int(int n) {
    char b[16]; int i = 15, neg = 0; b[15] = 0;
    if (n < 0) { neg = 1; n = -n; } if (n == 0) b[--i] = '0';
    while (n > 0) { b[--i] = '0' + (n % 10); n /= 10; } if (neg) b[--i] = '-';
    print_str(&b[i]); }
static void print_dbl(double v) {
    int w = (int)v, f = (int)((v - (double)w) * 1000.0); if (f < 0) f = -f;
    print_int(w); print_str(".");
    char fb[4]; fb[0]='0'+(f/100); f%=100; fb[1]='0'+(f/10); f%=10; fb[2]='0'+f; fb[3]=0; print_str(fb); }

/* ── Math helpers ── */

static double abs_d(double x) { return x < 0.0 ? -x : x; }
static double clamp_d(double v, double lo, double hi) { return v < lo ? lo : v > hi ? hi : v; }
static double lerp(double a, double b, double t) { return a + (b - a) * clamp_d(t, 0.0, 1.0); }
static double vec3_mag(vec3_t v) { double s = v.x*v.x + v.y*v.y + v.z*v.z;
    if (s <= 0.0) return 0.0; double g = s;
    for (int i = 0; i < 20; i++) g = (g + s / g) * 0.5; return g; }
static vec3_t vec3_scale(vec3_t v, double s) { return (vec3_t){v.x*s, v.y*s, v.z*s}; }
static vec3_t vec3_sub(vec3_t a, vec3_t b) { return (vec3_t){a.x-b.x, a.y-b.y, a.z-b.z}; }
static vec3_t vec3_add(vec3_t a, vec3_t b) { return (vec3_t){a.x+b.x, a.y+b.y, a.z+b.z}; }
static vec3_t vec3_normalize(vec3_t v) { double m = vec3_mag(v);
    if (m < 1e-9) return (vec3_t){0,0,0}; return vec3_scale(v, 1.0 / m); }
static double vec3_dot(vec3_t a, vec3_t b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
static double sin_a(double x) { x = x - 6.28318 * (int)(x / 6.28318);
    double s = x - x*x*x/6.0 + x*x*x*x*x/120.0; return clamp_d(s, -1.0, 1.0); }

/* ── Force Feedback ── */

static void force_init(void) {
    force_state.stiffness = 500.0; force_state.damping = 10.0;
    force_state.force_vector = (vec3_t){0,0,0};
    force_state.contact_point = (vec3_t){0,0,0};
    force_state.surface_normal = (vec3_t){0,1,0}; }

static vec3_t compute_spring_force(vec3_t pos, vec3_t surf_pt, double k) {
    return vec3_scale(vec3_sub(surf_pt, pos), k); }

static vec3_t compute_damping_force(vec3_t vel, double b) {
    return vec3_scale(vel, -b); }

static vec3_t compute_impedance_force(vec3_t pos, vec3_t vel, vec3_t surf_pt) {
    vec3_t spring = compute_spring_force(pos, surf_pt, force_state.stiffness);
    vec3_t damp = compute_damping_force(vel, force_state.damping);
    return vec3_add(spring, damp); }

static vec3_t compute_force_vector(vec3_t pos, vec3_t vel) {
    vec3_t diff = vec3_sub(pos, force_state.contact_point);
    double penetration = vec3_dot(diff, force_state.surface_normal);
    if (penetration > 0.0) {
        vec3_t f = compute_impedance_force(pos, vel, force_state.contact_point);
        force_state.force_vector = f; return f;
    }
    force_state.force_vector = (vec3_t){0,0,0};
    return (vec3_t){0,0,0}; }

static vec3_t clamp_force(vec3_t f, double max_mag) {
    double m = vec3_mag(f);
    if (m > max_mag && m > 1e-9) return vec3_scale(f, max_mag / m); return f; }

static double compute_torque(vec3_t force, vec3_t pivot, vec3_t point) {
    vec3_t r = vec3_sub(point, pivot);
    return r.x * force.y - r.y * force.x; }

/* ── Texture Simulation ── */

static void texture_init_defaults(void) {
    material_count = 0;
    materials[material_count++] = (texture_profile_t){0.1,0.9,0.05,100.0,0.2,PAT_SMOOTH};
    materials[material_count++] = (texture_profile_t){0.8,0.2,0.3,250.0,0.6,PAT_ROUGH};
    materials[material_count++] = (texture_profile_t){0.5,0.5,0.2,350.0,0.4,PAT_GRANULAR};
    materials[material_count++] = (texture_profile_t){0.3,0.6,0.1,150.0,0.3,PAT_RIBBED};
    materials[material_count++] = (texture_profile_t){0.6,0.4,0.4,400.0,0.5,PAT_STIPPLED}; }

static double texture_vibrate(double t, const texture_profile_t *tex) {
    double freq = clamp_d(tex->frequency, 50.0, 500.0);
    double amp = clamp_d(tex->amplitude, 0.0, 1.0);
    double base = sin_a(6.28318 * freq * t) * amp;
    if (tex->pattern_type == PAT_ROUGH)
        base += sin_a(6.28318 * freq * 3.0 * t) * amp * 0.3;
    if (tex->pattern_type == PAT_RIBBED)
        base = (base > 0.0) ? base : -base * 0.5;
    return base * tex->roughness; }

static texture_profile_t blend_textures(const texture_profile_t *a,
                                        const texture_profile_t *b, double t) {
    double tc = clamp_d(t, 0.0, 1.0);
    return (texture_profile_t){lerp(a->roughness,b->roughness,tc),
        lerp(a->smoothness,b->smoothness,tc), lerp(a->stickiness,b->stickiness,tc),
        lerp(a->frequency,b->frequency,tc), lerp(a->amplitude,b->amplitude,tc),
        (tc < 0.5) ? a->pattern_type : b->pattern_type}; }

/* ── Temperature Control ── */

static void thermal_init(void) {
    thermal_state.current_temp = 25.0; thermal_state.target_temp = 25.0;
    thermal_state.ramp_rate = 2.0; thermal_state.mode = THERM_NEUTRAL; }

static double thermal_update(double dt) {
    double min_t = 10.0, max_t = 45.0;
    double diff = thermal_state.target_temp - thermal_state.current_temp;
    double step = thermal_state.ramp_rate * dt;
    if (abs_d(diff) < step)
        thermal_state.current_temp = clamp_d(thermal_state.target_temp, min_t, max_t);
    else
        thermal_state.current_temp += (diff > 0.0 ? step : -step);
    thermal_state.current_temp = clamp_d(thermal_state.current_temp, min_t, max_t);
    return thermal_state.current_temp; }

static void thermal_set_target(double temp) {
    thermal_state.target_temp = clamp_d(temp, 10.0, 45.0);
    if (temp > thermal_state.current_temp) thermal_state.mode = THERM_HEAT;
    else if (temp < thermal_state.current_temp) thermal_state.mode = THERM_COOL;
    else thermal_state.mode = THERM_NEUTRAL; }

static double thermal_gradient(double pos, double start_t, double end_t, double span) {
    if (span < 1e-9) return start_t;
    return lerp(start_t, end_t, clamp_d(pos / span, 0.0, 1.0)); }

/* ── Haptic Rendering ── */

static void surface_init(void) {
    surface.bump_height = 0.5; surface.ridge_width = 2.0;
    surface.hole_depth = 1.0; surface.edge_sharpness = 0.8; }

static void tool_init(void) {
    tool_count = 0;
    tools[tool_count++] = (haptic_tool_t){TOOL_SCALPEL, 0.1, 800.0, 0.1};
    tools[tool_count++] = (haptic_tool_t){TOOL_BRUSH, 5.0, 200.0, 0.6};
    tools[tool_count++] = (haptic_tool_t){TOOL_POINTER, 1.0, 500.0, 0.3}; }

static double detect_bump(vec3_t pos, double bx, double by) {
    double dx = pos.x - bx, dy = pos.y - by, d2 = dx*dx + dy*dy;
    if (d2 > 1.0) return 0.0;
    return surface.bump_height * (1.0 - d2); }

static double detect_ridge(vec3_t pos, double rx) {
    double d = abs_d(pos.x - rx);
    if (d > surface.ridge_width) return 0.0;
    return surface.bump_height * (1.0 - d / surface.ridge_width); }

static double detect_hole(vec3_t pos, double hx, double hy) {
    double dx = pos.x - hx, dy = pos.y - hy, d2 = dx*dx + dy*dy;
    if (d2 > 1.0) return 0.0;
    return -surface.hole_depth * (1.0 - d2); }

static vec3_t render_impedance(vec3_t pos, vec3_t vel, int tool_id) {
    double k = (tool_id >= 0 && tool_id < tool_count) ? tools[tool_id].stiffness : 500.0;
    double b = k * 0.02;
    vec3_t spring = vec3_scale(vec3_sub(force_state.contact_point, pos), k);
    vec3_t damp = vec3_scale(vel, -b);
    return clamp_force(vec3_add(spring, damp), 20.0); }

static vec3_t render_admittance(vec3_t force_in, double mass) {
    if (mass < 0.01) mass = 0.01;
    double dt = 0.001, dt2 = dt * dt;
    return (vec3_t){force_in.x/mass*dt2, force_in.y/mass*dt2, force_in.z/mass*dt2}; }

/* ── Multimodal Feedback ── */

static void sync_init(void) {
    sync_state.visual_latency_ms = 16.0; sync_state.audio_latency_ms = 5.0;
    sync_state.haptic_latency_ms = 1.0; sync_state.sync_offset = 0.0; }

static double sync_compute_offset(void) {
    double max_lat = sync_state.visual_latency_ms;
    if (sync_state.audio_latency_ms > max_lat) max_lat = sync_state.audio_latency_ms;
    if (sync_state.haptic_latency_ms > max_lat) max_lat = sync_state.haptic_latency_ms;
    sync_state.sync_offset = max_lat - sync_state.haptic_latency_ms;
    return sync_state.sync_offset; }

static double cross_modal_map(double intensity, double range_in, double range_out) {
    if (range_in < 1e-9) return 0.0;
    return clamp_d(intensity / range_in, 0.0, 1.0) * range_out; }

static double audio_haptic_coupling(double contact_force, double freq_base) {
    double norm = clamp_d(contact_force / 20.0, 0.0, 1.0);
    return freq_base + norm * 400.0; }

/* ── Haptic Effect Library ── */

static void library_init(void) {
    effect_count = 0;
    effects[effect_count++] = (haptic_effect_t){FX_CLICK,0.8,200.0,0.05,0.01,0.01,0.8,0.02};
    effects[effect_count++] = (haptic_effect_t){FX_BUZZ,0.5,150.0,0.3,0.05,0.05,0.7,0.1};
    effects[effect_count++] = (haptic_effect_t){FX_PULSE,1.0,80.0,0.5,0.1,0.05,0.9,0.2};
    effects[effect_count++] = (haptic_effect_t){FX_WAVE,0.6,30.0,1.0,0.2,0.1,0.8,0.3};
    effects[effect_count++] = (haptic_effect_t){FX_TEX_CLICK,0.4,350.0,0.03,0.005,0.005,0.9,0.01}; }

static double envelope_adsrc(double t, const haptic_effect_t *fx) {
    if (t < 0.0 || t > fx->duration) return 0.0;
    if (t < fx->attack) return fx->amplitude * (t / fx->attack);
    double t2 = t - fx->attack;
    if (t2 < fx->decay)
        return fx->amplitude * (1.0 - (1.0 - fx->sustain) * (t2 / fx->decay));
    double sus_dur = fx->duration - fx->attack - fx->decay - fx->release;
    if (sus_dur < 0.0) sus_dur = 0.0;
    double t3 = t - fx->attack - fx->decay;
    if (t3 < sus_dur) return fx->amplitude * fx->sustain;
    double t4 = t - fx->duration + fx->release;
    if (t4 < 0.0) t4 = 0.0;
    if (fx->release < 1e-9) return 0.0;
    return fx->amplitude * fx->sustain * (1.0 - t4 / fx->release); }

static double play_effect(int eid, double t) {
    if (eid < 0 || eid >= effect_count) return 0.0;
    return envelope_adsrc(t, &effects[eid]); }

static int create_waveform(const double *samples, int count, int loop_flag) {
    if (waveform_count >= MAX_EFFECTS || count > MAX_WAVEFORM_SAMPLES) return -1;
    waveform_t *w = &waveforms[waveform_count];
    w->sample_count = count;
    for (int i = 0; i < count; i++) w->samples[i] = samples[i];
    w->playback_position = 0; w->loop = loop_flag;
    return waveform_count++; }

static double play_waveform(int wid) {
    if (wid < 0 || wid >= waveform_count) return 0.0;
    waveform_t *w = &waveforms[wid];
    if (w->sample_count == 0) return 0.0;
    double val = w->samples[w->playback_position];
    w->playback_position++;
    if (w->playback_position >= w->sample_count) {
        if (w->loop) w->playback_position = 0;
        else w->playback_position = w->sample_count - 1;
    } return val; }

static int sequence_add(int eid) {
    if (sequence.count >= MAX_SEQUENCE || eid < 0 || eid >= effect_count) return -1;
    sequence.effect_ids[sequence.count] = eid;
    sequence.total_duration += effects[eid].duration;
    return ++sequence.count; }

static void sequence_reset(void) { sequence.count = 0; sequence.total_duration = 0.0; }

/* ── Device management ── */

static int device_create(unsigned int caps) {
    if (device_count >= MAX_DEVICES) return -1;
    devices[device_count] = (haptic_device_t){{0,0,0},{0,0,0},25.0,0,caps};
    return device_count++; }

/* ── Help / Info / Status ── */

static void show_help(void) {
    print_str("Haptic Interface v54.0 - Tactile Feedback System\n");
    print_str("Usage: haptic_interface [options]\n");
    print_str("  -h  Show this help\n");
    print_str("  -t  Run comprehensive self-test\n"); }

static void show_status(void) {
    print_str("status: devices="); print_int(device_count);
    print_str(" effects="); print_int(effect_count);
    print_str(" waveforms="); print_int(waveform_count);
    print_str(" materials="); print_int(material_count);
    print_str(" tools="); print_int(tool_count);
    print_str(" seq="); print_int(sequence.count);
    print_str(" sync_off="); print_dbl(sync_state.sync_offset);
    print_str("ms temp="); print_dbl(thermal_state.current_temp);
    print_str("C\n"); }

/* ── Test: Force Feedback ── */

static void test_force_feedback(void) {
    print_str("[force_feedback] test\n");
    force_init();
    vec3_t pos = {0.01, -0.005, 0.0}, vel = {0.1, -0.05, 0.0};
    force_state.contact_point = (vec3_t){0,0,0};
    force_state.surface_normal = (vec3_t){0,1,0};
    vec3_t f = compute_force_vector(pos, vel);
    print_str("  force: ("); print_dbl(f.x); print_str(",");
    print_dbl(f.y); print_str(","); print_dbl(f.z); print_str(")\n");
    vec3_t fc = clamp_force(f, 10.0);
    print_str("  clamped: ("); print_dbl(fc.x); print_str(",");
    print_dbl(fc.y); print_str(","); print_dbl(fc.z); print_str(")\n");
    print_str("  torque: "); print_dbl(compute_torque(f,(vec3_t){0,0,0},pos)); print_str("\n");
    print_str("  OK\n"); }

/* ── Test: Texture Simulation ── */

static void test_texture(void) {
    print_str("[texture] test\n");
    texture_init_defaults();
    print_str("  materials: "); print_int(material_count); print_str("\n");
    print_str("  smooth vib: "); print_dbl(texture_vibrate(0.001, &materials[0])); print_str("\n");
    print_str("  rough vib: "); print_dbl(texture_vibrate(0.001, &materials[1])); print_str("\n");
    texture_profile_t bl = blend_textures(&materials[0], &materials[1], 0.5);
    print_str("  blended rough: "); print_dbl(bl.roughness); print_str("\n");
    print_str("  blended freq: "); print_dbl(bl.frequency); print_str("\n");
    print_str("  OK\n"); }

/* ── Test: Temperature Control ── */

static void test_thermal(void) {
    print_str("[thermal] test\n");
    thermal_init();
    print_str("  initial: "); print_dbl(thermal_state.current_temp); print_str("C\n");
    thermal_set_target(38.0);
    for (int i = 0; i < 10; i++) thermal_update(0.5);
    print_str("  after heat: "); print_dbl(thermal_state.current_temp); print_str("C\n");
    thermal_set_target(15.0);
    for (int i = 0; i < 10; i++) thermal_update(0.5);
    print_str("  after cool: "); print_dbl(thermal_state.current_temp); print_str("C\n");
    print_str("  gradient@0.5: "); print_dbl(thermal_gradient(0.5,20.0,40.0,1.0)); print_str("C\n");
    thermal_set_target(100.0);
    print_str("  clamped target: "); print_dbl(thermal_state.target_temp); print_str("C\n");
    print_str("  OK\n"); }

/* ── Test: Haptic Rendering ── */

static void test_rendering(void) {
    print_str("[rendering] test\n");
    surface_init(); tool_init();
    print_str("  tools: "); print_int(tool_count); print_str("\n");
    vec3_t p = {0.5, 0.5, 0.0};
    print_str("  bump@0.5: "); print_dbl(detect_bump(p, 0.0, 0.0)); print_str("\n");
    print_str("  ridge@0.3: "); print_dbl(detect_ridge(p, 0.3)); print_str("\n");
    print_str("  hole@origin: "); print_dbl(detect_hole(p, 0.0, 0.0)); print_str("\n");
    force_state.contact_point = (vec3_t){0,0,0};
    vec3_t fi = render_impedance(p, (vec3_t){0.1,0,0}, TOOL_POINTER);
    print_str("  impedance: ("); print_dbl(fi.x); print_str(",");
    print_dbl(fi.y); print_str(","); print_dbl(fi.z); print_str(")\n");
    vec3_t disp = render_admittance(fi, 1.0);
    print_str("  admittance disp: "); print_dbl(disp.x); print_str("\n");
    print_str("  OK\n"); }

/* ── Test: Multimodal Feedback ── */

static void test_multimodal(void) {
    print_str("[multimodal] test\n");
    sync_init();
    print_str("  sync offset: "); print_dbl(sync_compute_offset()); print_str("ms\n");
    print_str("  cross_modal: "); print_dbl(cross_modal_map(0.7, 1.0, 5.0)); print_str("\n");
    print_str("  audio-haptic freq: "); print_dbl(audio_haptic_coupling(10.0, 100.0));
    print_str("Hz\n"); print_str("  OK\n"); }

/* ── Test: Haptic Library ── */

static void test_library(void) {
    print_str("[library] test\n");
    library_init();
    print_str("  presets: "); print_int(effect_count); print_str("\n");
    print_str("  click@0.01s: "); print_dbl(play_effect(FX_CLICK, 0.01)); print_str("\n");
    print_str("  pulse@0.2s: "); print_dbl(play_effect(FX_PULSE, 0.2)); print_str("\n");
    double wf[8] = {0.0,0.3,0.6,0.9,1.0,0.9,0.6,0.3};
    int wid = create_waveform(wf, 8, 1);
    print_str("  waveform id: "); print_int(wid); print_str("\n");
    print_str("  waveform[0]: "); print_dbl(play_waveform(wid)); print_str("\n");
    sequence_reset();
    sequence_add(FX_CLICK); sequence_add(FX_BUZZ); sequence_add(FX_PULSE);
    print_str("  sequence: "); print_int(sequence.count);
    print_str(" dur="); print_dbl(sequence.total_duration); print_str("s\n");
    print_str("  OK\n"); }

/* ── Comprehensive test ── */

static void test_mode(void) {
    print_str("haptic_interface: self-test\n");
    test_force_feedback(); test_texture(); test_thermal();
    test_rendering(); test_multimodal(); test_library();
    print_str("[integration] test\n");
    device_count = 0;
    int did = device_create(CAP_ALL);
    print_str("  device: "); print_int(did);
    print_str(" caps="); print_int(devices[did].capabilities); print_str("\n");
    show_status();
    print_str("  OK\n");
    print_str("haptic_interface: done\n"); }

/* ── Entry point ── */

void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    int argc = host_get_argv(buf, 512); (void)argc;
    int help_flag = 0, test_flag = 0;
    char *ap = (char *)buf; unsigned int pos = 0;
    /* Skip argv[0] */
    while (pos < 512 && ap[pos]) pos++; pos++;
    /* Parse arguments */
    while (pos < 512 && ap[pos]) { char *a = &ap[pos];
        if ((a[0]=='-') && (a[1]=='h') && (a[2]==0)) help_flag = 1;
        else if ((a[0]=='-') && (a[1]=='t') && (a[2]==0)) test_flag = 1;
        while (pos < 512 && ap[pos]) pos++; pos++; }
    if (help_flag) { show_help(); host_exit(0); }
    if (test_flag) { test_mode(); host_exit(0); }
    print_str("Haptic Interface v54.0 - Tactile Feedback System\n");
    print_str("No action specified. Use -h for help, -t for test.\n");
    host_exit(0); }
