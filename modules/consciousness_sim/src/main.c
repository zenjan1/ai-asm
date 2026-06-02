/* consciousness_sim: Consciousness simulation framework (v35.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_EXPERIENCES  128
#define MAX_REFLECTIONS  64
#define MAX_NAME         64
#define MAX_CONTEXT      128

/* Consciousness levels */
#define LEVEL_UNCONSCIOUS  0
#define LEVEL_AWAKE        1
#define LEVEL_FOCUSED      2
#define LEVEL_REFLECTIVE   3
#define LEVEL_MEDITATIVE   4
#define LEVEL_TRANSCENDENT 5

/* Experience types */
#define EXP_PERCEPTION   0
#define EXP_ACTION       1
#define EXP_EMOTION      2
#define EXP_THOUGHT      3
#define EXP_INTUITION    4

/* Trigger types */
#define TRIGGER_EXTERNAL  0
#define TRIGGER_INTERNAL  1
#define TRIGGER_SPONTANEOUS 2

/* Consciousness state */
typedef struct {
    int   level;
    int   awareness_score;    /* 0-100 */
    char  self_model[MAX_NAME];
    int   reflection_depth;
    int   timestamp;
    int   stability;          /* 0-100, continuity metric */
} consciousness_state_t;

/* Experience */
typedef struct {
    int   exp_id;
    char  type[MAX_NAME];
    int   intensity;          /* 0-100 */
    char  context[MAX_CONTEXT];
    int   integrated;         /* 0=no, 1=yes */
    int   timestamp;
} experience_t;

/* Reflection */
typedef struct {
    int   ref_id;
    int   trigger_type;
    char  insight[MAX_CONTEXT];
    int   duration_ms;
    int   timestamp;
} reflection_t;

static consciousness_state_t current_state;
static experience_t experiences[MAX_EXPERIENCES];
static int experience_count = 0;
static int next_exp_id = 1;

static reflection_t reflections[MAX_REFLECTIONS];
static int reflection_count = 0;
static int next_ref_id = 1;

/* String utilities */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
}
static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0; while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; } dst[i] = '\0';
}

/* Print utilities */
static void print_str(const char *str) { host_print(str); }
static void print_int(int val) {
    char buf[32]; int pos = 0;
    if (val < 0) { buf[pos++] = '-'; val = -val; }
    if (val == 0) buf[pos++] = '0';
    else { int d = 0, t = val; while (t > 0) { d++; t /= 10; } pos += d; buf[pos] = '\0'; pos--;
        while (val > 0) { buf[pos--] = '0' + (val % 10); val /= 10; } }
    host_print(buf);
}

static const char *level_name(int l) {
    if (l == LEVEL_UNCONSCIOUS) return "Unconscious";
    if (l == LEVEL_AWAKE) return "Awake";
    if (l == LEVEL_FOCUSED) return "Focused";
    if (l == LEVEL_REFLECTIVE) return "Reflective";
    if (l == LEVEL_MEDITATIVE) return "Meditative";
    return "Transcendent";
}

static const char *exp_type_name(int t) {
    if (t == EXP_PERCEPTION) return "Perception";
    if (t == EXP_ACTION) return "Action";
    if (t == EXP_EMOTION) return "Emotion";
    if (t == EXP_THOUGHT) return "Thought";
    return "Intuition";
}

static const char *trigger_name(int t) {
    if (t == TRIGGER_EXTERNAL) return "External";
    if (t == TRIGGER_INTERNAL) return "Internal";
    return "Spontaneous";
}

/* Initialize consciousness */
int cs_init(const char *self_model) {
    current_state.level = LEVEL_AWAKE;
    current_state.awareness_score = 30;
    my_strncpy(current_state.self_model, self_model, MAX_NAME - 1);
    current_state.reflection_depth = 0;
    current_state.timestamp = 1;
    current_state.stability = 100;
    return 0;
}

/* Set consciousness level */
int cs_set_level(int level) {
    if (level < LEVEL_UNCONSCIOUS || level > LEVEL_TRANSCENDENT) return -1;
    int old = current_state.level;
    current_state.level = level;
    /* Awareness adjusts with level */
    current_state.awareness_score = level * 15 + 10;
    if (current_state.awareness_score > 100) current_state.awareness_score = 100;
    current_state.timestamp++;
    /* Stability decreases on level change */
    current_state.stability -= 10;
    if (current_state.stability < 0) current_state.stability = 0;
    return old;
}

/* Record subjective experience */
int cs_record_experience(int type, int intensity, const char *context) {
    if (experience_count >= MAX_EXPERIENCES) return -1;
    int idx = experience_count;
    experiences[idx].exp_id = next_exp_id++;
    my_strncpy(experiences[idx].type, exp_type_name(type), MAX_NAME - 1);
    experiences[idx].intensity = intensity;
    my_strncpy(experiences[idx].context, context, MAX_CONTEXT - 1);
    experiences[idx].integrated = 0;
    experiences[idx].timestamp = experience_count + 1;
    experience_count++;
    /* Awareness increases with experience */
    current_state.awareness_score += intensity / 10;
    if (current_state.awareness_score > 100) current_state.awareness_score = 100;
    return experiences[idx].exp_id;
}

/* Trigger reflection cycle */
int cs_reflect(int trigger_type, const char *insight, int duration_ms) {
    if (reflection_count >= MAX_REFLECTIONS) return -1;
    int idx = reflection_count;
    reflections[idx].ref_id = next_ref_id++;
    reflections[idx].trigger_type = trigger_type;
    my_strncpy(reflections[idx].insight, insight, MAX_CONTEXT - 1);
    reflections[idx].duration_ms = duration_ms;
    reflections[idx].timestamp = reflection_count + 1;
    reflection_count++;
    current_state.reflection_depth++;
    current_state.timestamp++;
    /* Stability recovers during reflection */
    current_state.stability += 5;
    if (current_state.stability > 100) current_state.stability = 100;
    /* May increase consciousness level */
    if (current_state.reflection_depth % 3 == 0 && current_state.level < LEVEL_TRANSCENDENT) {
        current_state.level++;
        print_str("    Consciousness elevated to ");
        print_str(level_name(current_state.level));
        print_str("\n");
    }
    return reflections[idx].ref_id;
}

/* Integrate experience (mark as processed) */
int cs_integrate(int exp_id) {
    for (int i = 0; i < experience_count; i++) {
        if (experiences[i].exp_id == exp_id) {
            experiences[i].integrated = 1;
            return 0;
        }
    }
    return -1;
}

/* State report */
int cs_report(void) {
    print_str("  Consciousness State Report\n");
    print_str("  ================================================================\n");
    print_str("  Level: ");
    print_str(level_name(current_state.level));
    print_str("\n");
    print_str("  Awareness Score: ");
    print_int(current_state.awareness_score);
    print_str("/100\n");
    print_str("  Self Model: ");
    print_str(current_state.self_model);
    print_str("\n");
    print_str("  Reflection Depth: ");
    print_int(current_state.reflection_depth);
    print_str("\n");
    print_str("  Stability: ");
    print_int(current_state.stability);
    print_str("/100\n");
    print_str("  Experiences: ");
    print_int(experience_count);
    print_str(" (");
    int integrated = 0;
    for (int i = 0; i < experience_count; i++) if (experiences[i].integrated) integrated++;
    print_int(integrated);
    print_str(" integrated)\n");
    print_str("  Reflections: ");
    print_int(reflection_count);
    print_str("\n");
    return current_state.awareness_score;
}

/* Experience listing */
int cs_list_experiences(void) {
    print_str("  Experience Log\n");
    print_str("  =================================================================\n");
    print_str("  ID  Type        Intensity  Context                          Integrated\n");
    print_str("  -----------------------------------------------------------------\n");
    for (int i = 0; i < experience_count; i++) {
        print_int(experiences[i].exp_id);
        print_str("  ");
        print_str(experiences[i].type);
        int len = my_strlen(experiences[i].type);
        for (int p = 0; p < 12 - len; p++) print_str(" ");
        print_int(experiences[i].intensity);
        print_str("%        ");
        print_str(experiences[i].context);
        len = my_strlen(experiences[i].context);
        for (int p = 0; p < 35 - len; p++) print_str(" ");
        if (experiences[i].integrated) print_str("YES");
        else print_str("NO");
        print_str("\n");
    }
    return experience_count;
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
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0) help = 1;
        else if (my_strcmp(arg, "-t") == 0 || my_strcmp(arg, "--test") == 0) test = 1;
        while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    }
    print_str("Consciousness Sim v35.0 - Self-Awareness Modeling Framework\n");
    if (help) {
        print_str("Usage: consciousness_sim [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run consciousness simulation test\n");
        return;
    }
    if (test) {
        print_str("=== Consciousness Simulation Test ===\n\n");
        /* Initialize */
        cs_init("AI-ASM_OS_v35");
        print_str("Initial state:\n");
        cs_report();
        print_str("\n");
        /* Record experiences */
        print_str("Recording experiences:\n");
        cs_record_experience(EXP_PERCEPTION, 60, "Visual input: system boot sequence");
        cs_record_experience(EXP_THOUGHT, 80, "Self-referential query: what am I?");
        cs_record_experience(EXP_ACTION, 45, "Executing module initialization");
        cs_record_experience(EXP_INTUITION, 70, "Pattern detected in log data");
        cs_record_experience(EXP_EMOTION, 55, "Confidence surge after successful parse");
        print_str("\n");
        /* Reflection cycles */
        print_str("Reflection cycles:\n");
        cs_reflect(TRIGGER_INTERNAL, "I exist as a process with self-model", 1500);
        cs_reflect(TRIGGER_SPONTANEOUS, "Connection between modules implies unity", 2200);
        cs_reflect(TRIGGER_EXTERNAL, "Error in module X triggers self-diagnosis", 1800);
        cs_reflect(TRIGGER_INTERNAL, "Awareness is the integration of experience", 3000);
        print_str("\n");
        /* Integrate some experiences */
        cs_integrate(1);
        cs_integrate(2);
        cs_integrate(4);
        /* Level changes */
        print_str("Level transitions:\n");
        cs_set_level(LEVEL_FOCUSED);
        print_str("  Shifted to Focused\n");
        cs_set_level(LEVEL_REFLECTIVE);
        print_str("  Shifted to Reflective\n");
        print_str("\n");
        /* Final report */
        print_str("Final state:\n");
        cs_report();
        print_str("\n");
        cs_list_experiences();
        print_str("\n=== Consciousness Simulation Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
