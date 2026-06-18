/* wearable_health: Wearable health tracking system (v1.0)
 * Sensor processing, activity recognition, exercise metrics, sleep analysis, alerts
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

#define MAX_SENSORS    8
#define MAX_ACTIVITIES 8
#define MAX_EXERCISES  8
#define MAX_SLEEP      8
#define MAX_ALERTS     8

#define ACT_WALKING    0x01
#define ACT_RUNNING    0x02
#define ACT_CYCLING    0x03
#define ACT_SWIMMING   0x04
#define ACT_SLEEPING   0x05
#define ACT_IDLE       0x06

#define SLEEP_DEEP     0x01
#define SLEEP_LIGHT    0x02
#define SLEEP_REM      0x03
#define SLEEP_AWAKE    0x04

typedef struct {
    int    type;
    double x, y, z;
    double magnitude;
    double sample_rate;
    int    n_samples;
    int    sensor_id;
    int    active;
} sensor_t;

typedef struct {
    int    activity_type;
    double confidence;
    double duration_min;
    double intensity;
    double calories;
    int    steps;
    int    timestamp;
    int    id;
    int    active;
} activity_t;

typedef struct {
    int    activity_type;
    double distance_km;
    double duration_min;
    double avg_speed;
    double max_speed;
    double calories;
    double avg_hr;
    double max_hr;
    int    steps;
    int    id;
    int    active;
} exercise_t;

typedef struct {
    int    stage;
    double duration_min;
    double quality;
    double start_time;
    int    id;
    int    active;
} sleep_stage_t;

typedef struct {
    double total_sleep;
    double deep_sleep;
    double light_sleep;
    double rem_sleep;
    double awake_time;
    double quality_score;
    int    id;
    int    active;
} sleep_analysis_t;

typedef struct {
    int    alert_type;
    int    severity;
    double value;
    double threshold;
    int    timestamp;
    int    acknowledged;
    int    id;
    int    active;
} alert_t;

typedef struct {
    int    n_sensors;
    int    n_activities;
    int    n_exercises;
    int    n_sleep_stages;
    int    n_alerts;
    int    total_steps;
    double total_calories;
    double total_distance;
    int    sitting_hours;
    int    goal_met;
} wh_state_t;

static sensor_t sensors[MAX_SENSORS];
static activity_t activities[MAX_ACTIVITIES];
static exercise_t exercises[MAX_EXERCISES];
static sleep_stage_t sleep_stages[MAX_SLEEP * 4];
static sleep_analysis_t sleep_analyses[MAX_SLEEP];
static alert_t alerts[MAX_ALERTS];
static wh_state_t wh;

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
static double dsin(double x) {
    double sum = x, term = x, x2 = x * x;
    for (int i = 1; i < 15; i++) { term *= -x2 / ((2*i)*(2*i+1)); sum += term; }
    return sum;
}

int wh_init(void) {
    if (initialized) return -1;
    wh.n_sensors = 0; wh.n_activities = 0;
    wh.n_exercises = 0; wh.n_sleep_stages = 0; wh.n_alerts = 0;
    wh.total_steps = 0; wh.total_calories = 0.0;
    wh.total_distance = 0.0; wh.sitting_hours = 0;
    wh.goal_met = 0;
    for (int i = 0; i < MAX_SENSORS; i++) sensors[i].active = 0;
    for (int i = 0; i < MAX_ACTIVITIES; i++) activities[i].active = 0;
    for (int i = 0; i < MAX_EXERCISES; i++) exercises[i].active = 0;
    for (int i = 0; i < MAX_SLEEP * 4; i++) sleep_stages[i].active = 0;
    for (int i = 0; i < MAX_SLEEP; i++) sleep_analyses[i].active = 0;
    for (int i = 0; i < MAX_ALERTS; i++) alerts[i].active = 0;
    initialized = 1;
    print_str("[WH] Wearable health initialized\n");
    return 0;
}

int wh_add_sensor(int type, double sample_rate) {
    if (wh.n_sensors >= MAX_SENSORS) return -1;
    sensor_t* s = &sensors[wh.n_sensors];
    s->type = type; s->sample_rate = sample_rate;
    s->n_samples = (int)sample_rate * 10;
    s->x = 0.1 * dsin(2.0 * 3.14159 * 2.0);
    s->y = 0.05 * dsin(2.0 * 3.14159 * 3.0);
    s->z = 0.02 * dsin(2.0 * 3.14159 * 5.0);
    s->magnitude = dsqrt(s->x * s->x + s->y * s->y + s->z * s->z);
    s->sensor_id = wh.n_sensors; s->active = 1;
    wh.n_sensors++;
    print_str("[WH] Sensor "); print_int(wh.n_sensors - 1);
    print_str(" type="); print_int(type);
    print_str(" rate="); print_int((int)sample_rate); print_str("Hz");
    print_str(" mag="); print_int((int)(s->magnitude * 1000)); print_str("mg\n");
    return wh.n_sensors - 1;
}

int wh_detect_activity(int activity_type, double confidence) {
    if (wh.n_activities >= MAX_ACTIVITIES) return -1;
    activity_t* a = &activities[wh.n_activities];
    a->activity_type = activity_type;
    a->confidence = confidence;
    a->duration_min = 10.0 + (double)(wh.n_activities % 4) * 5.0;
    a->intensity = activity_type == ACT_RUNNING ? 8.0 :
                   (activity_type == ACT_CYCLING ? 6.0 :
                   (activity_type == ACT_WALKING ? 3.0 : 1.0));
    a->calories = a->duration_min * a->intensity * 0.1;
    a->steps = (int)(a->duration_min * (activity_type == ACT_WALKING ? 100.0 :
                     (activity_type == ACT_RUNNING ? 150.0 : 50.0)));
    a->timestamp = wh.n_activities * 600;
    a->id = wh.n_activities; a->active = 1;
    wh.n_activities++;
    wh.total_steps += a->steps;
    wh.total_calories += a->calories;
    print_str("[WH] Activity "); print_int(wh.n_activities - 1);
    print_str(" type="); print_int(activity_type);
    print_str(" conf="); print_int((int)(confidence * 100)); print_str("%");
    print_str(" dur="); print_int((int)a->duration_min); print_str("min");
    print_str(" cal="); print_int((int)a->calories);
    print_str(" steps="); print_int(a->steps); print_str("\n");
    return wh.n_activities - 1;
}

int wh_add_exercise(int activity_type, double distance, double duration) {
    if (wh.n_exercises >= MAX_EXERCISES) return -1;
    exercise_t* e = &exercises[wh.n_exercises];
    e->activity_type = activity_type;
    e->distance_km = distance;
    e->duration_min = duration;
    e->avg_speed = distance / (duration / 60.0);
    e->max_speed = e->avg_speed * 1.3;
    e->calories = distance * 60.0;
    e->avg_hr = 120.0 + (double)(activity_type % 3) * 10.0;
    e->max_hr = e->avg_hr + 30.0;
    e->steps = (int)(distance * 1300.0);
    e->id = wh.n_exercises; e->active = 1;
    wh.n_exercises++;
    wh.total_distance += distance;
    wh.total_calories += e->calories;
    wh.total_steps += e->steps;
    print_str("[WH] Exercise "); print_int(wh.n_exercises - 1);
    print_str(" type="); print_int(activity_type);
    print_str(" dist="); print_int((int)(distance * 1000)); print_str("m");
    print_str(" dur="); print_int((int)duration); print_str("min");
    print_str(" cal="); print_int((int)e->calories);
    print_str(" HR="); print_int((int)e->avg_hr); print_str("bpm\n");
    return wh.n_exercises - 1;
}

int wh_add_sleep_stage(int stage, double duration) {
    int night = wh.n_sleep_stages / 4;
    if (night >= MAX_SLEEP) return -1;
    sleep_stage_t* ss = &sleep_stages[wh.n_sleep_stages];
    ss->stage = stage;
    ss->duration_min = duration;
    ss->quality = stage == SLEEP_DEEP ? 0.9 :
                  (stage == SLEEP_REM ? 0.8 :
                  (stage == SLEEP_LIGHT ? 0.6 : 0.3));
    ss->start_time = wh.n_sleep_stages * 15.0;
    ss->id = wh.n_sleep_stages; ss->active = 1;
    wh.n_sleep_stages++;
    return wh.n_sleep_stages - 1;
}

void wh_analyze_sleep(int night) {
    if (night >= MAX_SLEEP) return;
    sleep_analysis_t* sa = &sleep_analyses[night];
    int base = night * 4;
    sa->deep_sleep = 0.0; sa->light_sleep = 0.0;
    sa->rem_sleep = 0.0; sa->awake_time = 0.0;
    for (int i = base; i < base + 4 && i < wh.n_sleep_stages; i++) {
        if (sleep_stages[i].stage == SLEEP_DEEP) sa->deep_sleep += sleep_stages[i].duration_min;
        else if (sleep_stages[i].stage == SLEEP_LIGHT) sa->light_sleep += sleep_stages[i].duration_min;
        else if (sleep_stages[i].stage == SLEEP_REM) sa->rem_sleep += sleep_stages[i].duration_min;
        else if (sleep_stages[i].stage == SLEEP_AWAKE) sa->awake_time += sleep_stages[i].duration_min;
    }
    sa->total_sleep = sa->deep_sleep + sa->light_sleep + sa->rem_sleep;
    sa->quality_score = (sa->deep_sleep * 0.9 + sa->rem_sleep * 0.8 +
                         sa->light_sleep * 0.6) / (sa->total_sleep + 1e-10) * 100.0;
    sa->id = night; sa->active = 1;
    print_str("[WH] Sleep night "); print_int(night);
    print_str(" total="); print_int((int)sa->total_sleep); print_str("min");
    print_str(" deep="); print_int((int)sa->deep_sleep);
    print_str(" light="); print_int((int)sa->light_sleep);
    print_str(" REM="); print_int((int)sa->rem_sleep);
    print_str(" awake="); print_int((int)sa->awake_time);
    print_str(" quality="); print_int((int)sa->quality_score); print_str("%\n");
}

int wh_add_alert(int alert_type, int severity, double value, double threshold) {
    if (wh.n_alerts >= MAX_ALERTS) return -1;
    alert_t* a = &alerts[wh.n_alerts];
    a->alert_type = alert_type; a->severity = severity;
    a->value = value; a->threshold = threshold;
    a->timestamp = wh.n_alerts * 1800;
    a->acknowledged = 0;
    a->id = wh.n_alerts; a->active = 1;
    wh.n_alerts++;
    print_str("[WH] Alert "); print_int(wh.n_alerts - 1);
    print_str(" type="); print_int(alert_type);
    print_str(" sev="); print_int(severity);
    print_str(" val="); print_int((int)value);
    print_str(" thresh="); print_int((int)threshold); print_str("\n");
    return wh.n_alerts - 1;
}

void wh_sedentary_check(void) {
    double active_min = 0.0;
    for (int i = 0; i < wh.n_activities; i++) {
        if (activities[i].activity_type != ACT_IDLE &&
            activities[i].activity_type != ACT_SLEEPING) {
            active_min += activities[i].duration_min;
        }
    }
    wh.sitting_hours = (int)((480.0 - active_min) / 60.0);
    if (wh.sitting_hours > 4) {
        wh_add_alert(1, 2, (double)wh.sitting_hours, 4.0);
    }
    print_str("[WH] Sedentary check: active="); print_int((int)active_min); print_str("min");
    print_str(" sitting="); print_int(wh.sitting_hours); print_str("h\n");
}

void wh_goal_tracking(int step_goal, int calorie_goal) {
    wh.goal_met = (wh.total_steps >= step_goal && wh.total_calories >= calorie_goal) ? 1 : 0;
    print_str("[WH] Goal tracking:\n");
    print_str("  Steps: "); print_int(wh.total_steps);
    print_str("/"); print_int(step_goal);
    print_str(" ("); print_int(wh.total_steps * 100 / step_goal); print_str("%)\n");
    print_str("  Calories: "); print_int((int)wh.total_calories);
    print_str("/"); print_int(calorie_goal);
    print_str(" ("); print_int((int)(wh.total_calories * 100.0 / calorie_goal)); print_str("%)\n");
    print_str("  Distance: "); print_int((int)(wh.total_distance * 1000)); print_str("m\n");
    print_str("  Goal met: "); print_int(wh.goal_met); print_str("\n");
}

void wh_print_state(void) {
    print_str("[WH] Sensors="); print_int(wh.n_sensors);
    print_str(" Activities="); print_int(wh.n_activities);
    print_str(" Exercises="); print_int(wh.n_exercises);
    print_str(" Sleep="); print_int(wh.n_sleep_stages);
    print_str(" Steps="); print_int(wh.total_steps);
    print_str(" Cal="); print_int((int)wh.total_calories);
    print_str(" Alerts="); print_int(wh.n_alerts); print_str("\n");
}

int main(void) {
    print_str("=== Wearable Health Tracking Demo ===\n\n");
    wh_init();

    print_str("Adding sensors...\n");
    wh_add_sensor(1, 50.0);
    wh_add_sensor(2, 100.0);
    wh_add_sensor(3, 25.0);
    wh_add_sensor(4, 10.0);

    print_str("\nActivity recognition...\n");
    wh_detect_activity(ACT_WALKING, 0.92);
    wh_detect_activity(ACT_RUNNING, 0.88);
    wh_detect_activity(ACT_CYCLING, 0.85);
    wh_detect_activity(ACT_WALKING, 0.95);
    wh_detect_activity(ACT_IDLE, 0.78);
    wh_detect_activity(ACT_SWIMMING, 0.82);

    print_str("\nExercise tracking...\n");
    wh_add_exercise(ACT_RUNNING, 5.0, 30.0);
    wh_add_exercise(ACT_CYCLING, 15.0, 45.0);
    wh_add_exercise(ACT_WALKING, 3.0, 35.0);
    wh_add_exercise(ACT_SWIMMING, 1.0, 25.0);

    print_str("\nSleep tracking (night 0)...\n");
    wh_add_sleep_stage(SLEEP_LIGHT, 20.0);
    wh_add_sleep_stage(SLEEP_DEEP, 45.0);
    wh_add_sleep_stage(SLEEP_REM, 30.0);
    wh_add_sleep_stage(SLEEP_AWAKE, 5.0);
    wh_analyze_sleep(0);

    print_str("\nSleep tracking (night 1)...\n");
    wh_add_sleep_stage(SLEEP_LIGHT, 15.0);
    wh_add_sleep_stage(SLEEP_DEEP, 60.0);
    wh_add_sleep_stage(SLEEP_REM, 25.0);
    wh_add_sleep_stage(SLEEP_AWAKE, 10.0);
    wh_analyze_sleep(1);

    print_str("\nHealth alerts...\n");
    wh_add_alert(2, 3, 110.0, 100.0);
    wh_add_alert(3, 2, 92.0, 95.0);
    wh_add_alert(4, 1, 37.8, 37.5);

    print_str("\nSedentary check...\n");
    wh_sedentary_check();

    print_str("\nGoal tracking...\n");
    wh_goal_tracking(10000, 500);

    wh_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
