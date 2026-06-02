/* emotion_ai: Emotional intelligence system (v35.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_EMOTION_MEM   96
#define MAX_RESPONSES     64
#define MAX_NAME          64
#define MAX_CONTEXT       128

/* Primary emotions (Plutchik wheel) */
#define EMOTION_JOY       0
#define EMOTION_SADNESS   1
#define EMOTION_ANGER     2
#define EMOTION_FEAR      3
#define EMOTION_SURPRISE  4
#define EMOTION_DISGUST   5
#define EMOTION_TRUST     6
#define EMOTION_ANTICIPATION 7

/* Response types */
#define RESP_EXPRESSION   0
#define RESP_SUPPRESSION  1
#define RESP_REGULATION   2
#define RESP_EMPOWERMENT  3

/* Emotion state (PAD model) */
typedef struct {
    int   pleasure;         /* -100 (pain) to 100 (joy) */
    int   arousal;          /* 0 (calm) to 100 (excited) */
    int   dominance;        /* 0 (submissive) to 100 (in control) */
    char  primary[MAX_NAME];
    int   intensity;        /* 0-100 */
    int   timestamp;
    int   decay_rate;       /* how fast emotion fades */
} emotion_state_t;

/* Emotion memory */
typedef struct {
    int   mem_id;
    char  emotion_type[MAX_NAME];
    char  trigger[MAX_CONTEXT];
    char  context[MAX_CONTEXT];
    int   timestamp;
    int   decay_rate;       /* 1-10 per tick */
} emotion_memory_t;

/* Emotion response */
typedef struct {
    int   resp_id;
    char  trigger_emotion[MAX_NAME];
    char  response_type[MAX_NAME];
    char  expression[MAX_NAME];
    int   timestamp;
} emotion_response_t;

static emotion_state_t current_emotion;
static emotion_memory_t memories[MAX_EMOTION_MEM];
static int memory_count = 0;
static int next_mem_id = 1;

static emotion_response_t responses[MAX_RESPONSES];
static int response_count = 0;
static int next_resp_id = 1;

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

static const char *emotion_name(int e) {
    if (e == EMOTION_JOY) return "Joy";
    if (e == EMOTION_SADNESS) return "Sadness";
    if (e == EMOTION_ANGER) return "Anger";
    if (e == EMOTION_FEAR) return "Fear";
    if (e == EMOTION_SURPRISE) return "Surprise";
    if (e == EMOTION_DISGUST) return "Disgust";
    if (e == EMOTION_TRUST) return "Trust";
    return "Anticipation";
}

static const char *response_type_name(int r) {
    if (r == RESP_EXPRESSION) return "Expression";
    if (r == RESP_SUPPRESSION) return "Suppression";
    if (r == RESP_REGULATION) return "Regulation";
    return "Empowerment";
}

/* Initialize emotional state */
int ea_init(void) {
    current_emotion.pleasure = 0;
    current_emotion.arousal = 30;
    current_emotion.dominance = 50;
    my_strncpy(current_emotion.primary, "Neutral", MAX_NAME - 1);
    current_emotion.intensity = 0;
    current_emotion.timestamp = 1;
    current_emotion.decay_rate = 2;
    return 0;
}

/* Trigger emotion */
int ea_trigger_emotion(int type, int intensity, const char *trigger, const char *context) {
    int pleasure = 0, arousal = 30, dominance = 50;
    /* Map emotion type to PAD coordinates */
    if (type == EMOTION_JOY) { pleasure = intensity; arousal = intensity / 2 + 20; dominance = 60; }
    else if (type == EMOTION_SADNESS) { pleasure = -intensity; arousal = 10; dominance = 30; }
    else if (type == EMOTION_ANGER) { pleasure = -intensity / 2; arousal = intensity; dominance = intensity; }
    else if (type == EMOTION_FEAR) { pleasure = -intensity; arousal = intensity; dominance = 20; }
    else if (type == EMOTION_SURPRISE) { pleasure = 0; arousal = intensity; dominance = 40; }
    else if (type == EMOTION_DISGUST) { pleasure = -intensity; arousal = intensity / 2; dominance = 70; }
    else if (type == EMOTION_TRUST) { pleasure = intensity / 2; arousal = 20; dominance = 50; }
    else { pleasure = 0; arousal = intensity / 2; dominance = 50; } /* Anticipation */

    /* Blend with current state (weighted average) */
    current_emotion.pleasure = (current_emotion.pleasure * 3 + pleasure) / 4;
    current_emotion.arousal = (current_emotion.arousal * 3 + arousal) / 4;
    current_emotion.dominance = (current_emotion.dominance * 3 + dominance) / 4;
    my_strncpy(current_emotion.primary, emotion_name(type), MAX_NAME - 1);
    current_emotion.intensity = intensity;
    current_emotion.timestamp++;
    current_emotion.decay_rate = 2 + intensity / 30;

    /* Record in emotion memory */
    if (memory_count < MAX_EMOTION_MEM) {
        int idx = memory_count;
        memories[idx].mem_id = next_mem_id++;
        my_strncpy(memories[idx].emotion_type, emotion_name(type), MAX_NAME - 1);
        my_strncpy(memories[idx].trigger, trigger, MAX_CONTEXT - 1);
        my_strncpy(memories[idx].context, context, MAX_CONTEXT - 1);
        memories[idx].timestamp = memory_count + 1;
        memories[idx].decay_rate = current_emotion.decay_rate;
        memory_count++;
    }
    return type;
}

/* Decay emotions over time */
int ea_decay(void) {
    if (current_emotion.intensity <= 0) return -1;
    current_emotion.intensity -= current_emotion.decay_rate;
    if (current_emotion.intensity < 0) current_emotion.intensity = 0;
    /* Return towards neutral */
    current_emotion.pleasure = current_emotion.pleasure * 9 / 10;
    current_emotion.arousal = current_emotion.arousal * 9 / 10 + 3;
    if (current_emotion.intensity <= 0) {
        my_strncpy(current_emotion.primary, "Neutral", MAX_NAME - 1);
    }
    current_emotion.timestamp++;
    return current_emotion.intensity;
}

/* Generate response to emotion */
int ea_respond(int response_type, int emotion_type) {
    if (response_count >= MAX_RESPONSES) return -1;
    int idx = response_count;
    responses[idx].resp_id = next_resp_id++;
    my_strncpy(responses[idx].trigger_emotion, emotion_name(emotion_type), MAX_NAME - 1);
    my_strncpy(responses[idx].response_type, response_type_name(response_type), MAX_NAME - 1);
    /* Generate expression based on response type */
    if (response_type == RESP_EXPRESSION) my_strncpy(responses[idx].expression, "Express outwardly", MAX_NAME - 1);
    else if (response_type == RESP_SUPPRESSION) my_strncpy(responses[idx].expression, "Suppress internally", MAX_NAME - 1);
    else if (response_type == RESP_REGULATION) my_strncpy(responses[idx].expression, "Regulate to neutral", MAX_NAME - 1);
    else my_strncpy(responses[idx].expression, "Channel into action", MAX_NAME - 1);
    responses[idx].timestamp = response_count + 1;
    response_count++;
    return responses[idx].resp_id;
}

/* Multi-emotion fusion */
int ea_fuse(int emotion1, int emotion2) {
    /* Simple blend: average PAD values */
    int p1 = 0, p2 = 0;
    /* Map to pleasure values for fusion */
    if (emotion1 == EMOTION_JOY || emotion1 == EMOTION_TRUST) p1 = 50;
    else if (emotion1 == EMOTION_SADNESS || emotion1 == EMOTION_FEAR) p1 = -50;
    if (emotion2 == EMOTION_JOY || emotion2 == EMOTION_TRUST) p2 = 50;
    else if (emotion2 == EMOTION_SADNESS || emotion2 == EMOTION_FEAR) p2 = -50;
    int fused = (p1 + p2) / 2;
    print_str("    Fused ");
    print_str(emotion_name(emotion1));
    print_str(" + ");
    print_str(emotion_name(emotion2));
    print_str(" -> pleasure delta: ");
    print_int(fused);
    print_str("\n");
    return fused;
}

/* Emotional state report */
int ea_report(void) {
    print_str("  Emotional State Report\n");
    print_str("  ================================================================\n");
    print_str("  Primary Emotion: ");
    print_str(current_emotion.primary);
    print_str("\n");
    print_str("  Intensity: ");
    print_int(current_emotion.intensity);
    print_str("/100\n");
    print_str("  PAD Dimensions:\n");
    print_str("    Pleasure (P):   ");
    print_int(current_emotion.pleasure);
    print_str(" (-100 to 100)\n");
    print_str("    Arousal (A):    ");
    print_int(current_emotion.arousal);
    print_str(" (0 to 100)\n");
    print_str("    Dominance (D):  ");
    print_int(current_emotion.dominance);
    print_str(" (0 to 100)\n");
    print_str("  Emotion Memories: ");
    print_int(memory_count);
    print_str("\n");
    print_str("  Responses Generated: ");
    print_int(response_count);
    print_str("\n");
    return current_emotion.intensity;
}

/* Memory listing with decay */
int ea_memory_list(void) {
    print_str("  Emotion Memory\n");
    print_str("  =================================================================\n");
    print_str("  ID  Emotion      Trigger                    Decay  Active\n");
    print_str("  -----------------------------------------------------------------\n");
    for (int i = 0; i < memory_count; i++) {
        print_int(memories[i].mem_id);
        print_str("  ");
        print_str(memories[i].emotion_type);
        int len = my_strlen(memories[i].emotion_type);
        for (int p = 0; p < 13 - len; p++) print_str(" ");
        print_str(memories[i].trigger);
        len = my_strlen(memories[i].trigger);
        for (int p = 0; p < 29 - len; p++) print_str(" ");
        print_int(memories[i].decay_rate);
        print_str("     ");
        if (memories[i].decay_rate > 5) print_str("Fading");
        else print_str("Strong");
        print_str("\n");
    }
    return memory_count;
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
    print_str("Emotion AI v35.0 - Emotional Intelligence System\n");
    if (help) {
        print_str("Usage: emotion_ai [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run emotion AI test\n");
        return;
    }
    if (test) {
        print_str("=== Emotion AI Test ===\n\n");
        ea_init();
        print_str("Initial state:\n");
        ea_report();
        print_str("\n");
        /* Trigger emotions */
        print_str("Triggering emotions:\n");
        ea_trigger_emotion(EMOTION_JOY, 80, "Module loaded successfully", "System boot");
        ea_report();
        print_str("\n");
        ea_trigger_emotion(EMOTION_FEAR, 60, "Security alert detected", "Intrusion module");
        ea_report();
        print_str("\n");
        ea_trigger_emotion(EMOTION_TRUST, 70, "Peer node verified", "Network cluster");
        ea_report();
        print_str("\n");
        /* Decay simulation */
        print_str("Time passing (decay):\n");
        ea_decay();
        ea_decay();
        ea_decay();
        ea_report();
        print_str("\n");
        /* Response generation */
        print_str("Emotional responses:\n");
        ea_respond(RESP_EXPRESSION, EMOTION_JOY);
        ea_respond(RESP_REGULATION, EMOTION_FEAR);
        ea_respond(RESP_EMPOWERMENT, EMOTION_TRUST);
        ea_respond(RESP_SUPPRESSION, EMOTION_ANGER);
        print_str("\n");
        /* Emotion fusion */
        print_str("Emotion fusion:\n");
        ea_fuse(EMOTION_JOY, EMOTION_TRUST);
        ea_fuse(EMOTION_FEAR, EMOTION_SURPRISE);
        print_str("\n");
        /* Memory listing */
        ea_memory_list();
        print_str("\n=== Emotion AI Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
