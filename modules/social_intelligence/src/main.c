/* social_intelligence: Social understanding system (v25.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Social contexts */
#define CONTEXT_PROFESSIONAL 0
#define CONTEXT_PERSONAL    1
#define CONTEXT_EMERGENCY   2
#define CONTEXT_COLLABORATIVE 3

/* Emotion types */
#define EMOTION_NEUTRAL    0
#define EMOTION_POSITIVE   1
#define EMOTION_NEGATIVE   2
#define EMOTION_CONFUSED   3
#define EMOTION_URGENT     4

/* Trust levels */
#define TRUST_NONE     0
#define TRUST_LOW      1
#define TRUST_MEDIUM   2
#define TRUST_HIGH     3
#define TRUST_FULL     4

/* Limits */
#define MAX_CONTEXTS  8
#define MAX_USERS     32
#define NAME_LEN      64
#define DESC_LEN      256

/* User profile */
typedef struct {
    int   user_id;
    char  name[NAME_LEN];
    int   trust_level;
    int   interaction_count;
    int   preferred_context;
    int   last_emotion;
} user_profile_t;

/* Social interaction */
typedef struct {
    int   interaction_id;
    int   user_id;
    int   context;
    int   emotion;
    char  content[DESC_LEN];
    int   resolution;
} social_interaction_t;

static user_profile_t users[MAX_USERS];
static social_interaction_t interactions[MAX_CONTEXTS * 4];
static int user_count = 0;
static int interaction_count = 0;

/* String utilities */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
}
static void my_strcpy(char *dst, const char *src) { while (*src) *dst++ = *src++; *dst = '\0'; }
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

/* Get context name */
static const char *context_name(int ctx) {
    if (ctx == CONTEXT_PROFESSIONAL) return "professional";
    if (ctx == CONTEXT_PERSONAL) return "personal";
    if (ctx == CONTEXT_EMERGENCY) return "emergency";
    if (ctx == CONTEXT_COLLABORATIVE) return "collaborative";
    return "unknown";
}

/* Get emotion name */
static const char *emotion_name(int emo) {
    if (emo == EMOTION_NEUTRAL) return "neutral";
    if (emo == EMOTION_POSITIVE) return "positive";
    if (emo == EMOTION_NEGATIVE) return "negative";
    if (emo == EMOTION_CONFUSED) return "confused";
    if (emo == EMOTION_URGENT) return "urgent";
    return "unknown";
}

/* Get trust name */
static const char *trust_name(int trust) {
    if (trust == TRUST_NONE) return "none";
    if (trust == TRUST_LOW) return "low";
    if (trust == TRUST_MEDIUM) return "medium";
    if (trust == TRUST_HIGH) return "high";
    if (trust == TRUST_FULL) return "full";
    return "unknown";
}

/* Register user */
int si_register_user(const char *name, int preferred_context) {
    if (user_count >= MAX_USERS) return -1;
    int idx = user_count;
    users[idx].user_id = user_count + 1;
    my_strncpy(users[idx].name, name, NAME_LEN - 1);
    users[idx].trust_level = TRUST_NONE;
    users[idx].interaction_count = 0;
    users[idx].preferred_context = preferred_context;
    users[idx].last_emotion = EMOTION_NEUTRAL;
    user_count++;
    return users[idx].user_id;
}

/* Recognize emotional state */
int si_recognize_emotion(int user_id, const char *input) {
    print_str("  Analyzing emotional state...\n");
    if (!input) return EMOTION_NEUTRAL;

    int emotion = EMOTION_NEUTRAL;
    for (int i = 0; i < user_count; i++) {
        if (users[i].user_id == user_id) {
            users[i].last_emotion = emotion;
            users[i].interaction_count++;
            if (users[i].interaction_count > 5 && users[i].trust_level < TRUST_HIGH) {
                users[i].trust_level++;
            }
            print_str("    Detected: ");
            print_str(emotion_name(emotion));
            print_str("\n");
            return emotion;
        }
    }
    return EMOTION_NEUTRAL;
}

/* Establish collaboration protocol */
int si_collaborate(int user_id, const char *task) {
    print_str("=== Collaboration Protocol ===\n");
    print_str("  Task: ");
    if (task) print_str(task);
    print_str("\n");
    for (int i = 0; i < user_count; i++) {
        if (users[i].user_id == user_id) {
            print_str("  Partner: ");
            print_str(users[i].name);
            print_str("\n");
            print_str("  Trust level: ");
            print_str(trust_name(users[i].trust_level));
            print_str("\n");
            print_str("  Context: ");
            print_str(context_name(users[i].preferred_context));
            print_str("\n");
            print_str("  Status: Collaboration established\n");
            return 0;
        }
    }
    return -1;
}

/* Build trust */
int si_build_trust(int user_id) {
    for (int i = 0; i < user_count; i++) {
        if (users[i].user_id == user_id) {
            if (users[i].trust_level < TRUST_FULL) {
                users[i].trust_level++;
                print_str("  Trust increased to: ");
                print_str(trust_name(users[i].trust_level));
                print_str("\n");
            }
            return users[i].trust_level;
        }
    }
    return -1;
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
    print_str("Social Intelligence v25.0\n");
    if (help) {
        print_str("Usage: social_intelligence [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run social test\n");
        return;
    }
    if (test) {
        print_str("=== Social Intelligence Test ===\n\n");
        int u1 = si_register_user("alice", CONTEXT_COLLABORATIVE);
        int u2 = si_register_user("bob", CONTEXT_PROFESSIONAL);
        print_str("Registered 2 users\n\n");
        si_recognize_emotion(u1, "Hello, how are you?");
        si_recognize_emotion(u2, "Please check the system logs");
        print_str("\n");
        si_collaborate(u1, "debug network issue");
        print_str("\n");
        si_build_trust(u1);
        si_build_trust(u2);
        print_str("\n=== Social Intelligence Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
