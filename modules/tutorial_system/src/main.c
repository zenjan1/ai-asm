/* tutorial_system: Interactive WASM development tutorials (v32.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_TUTORIALS  32
#define MAX_LESSONS    16
#define MAX_QUESTIONS  8
#define MAX_NAME       64
#define MAX_DESC       128
#define MAX_CODE       256

/* Difficulty levels */
#define DIFF_BEGINNER   0
#define DIFF_INTERMEDIATE 1
#define DIFF_ADVANCED   2

/* Tutorial lesson */
typedef struct {
    char title[MAX_NAME];
    char description[MAX_DESC];
    char code_example[MAX_CODE];
    int  difficulty;
    int  completed;
} lesson_t;

/* Tutorial entry */
typedef struct {
    int   tut_id;
    char  name[MAX_NAME];
    char  description[MAX_DESC];
    int   difficulty;
    int   lesson_count;
    lesson_t lessons[MAX_LESSONS];
    int   progress;       /* percentage */
    int   certificate;    /* earned cert? */
} tutorial_t;

/* User progress */
static int user_progress[MAX_TUTORIALS];  /* lesson index per tutorial */
static int total_completed = 0;

static tutorial_t tutorials[MAX_TUTORIALS];
static int tut_count = 0;
static int next_tut_id = 1;

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

static const char *diff_name(int d) {
    if (d == DIFF_BEGINNER) return "beginner";
    if (d == DIFF_INTERMEDIATE) return "intermediate";
    return "advanced";
}

/* Create tutorial */
int ts_create(const char *name, const char *desc, int difficulty) {
    if (tut_count >= MAX_TUTORIALS) return -1;
    int idx = tut_count;
    tutorials[idx].tut_id = next_tut_id++;
    my_strncpy(tutorials[idx].name, name, MAX_NAME - 1);
    my_strncpy(tutorials[idx].description, desc, MAX_DESC - 1);
    tutorials[idx].difficulty = difficulty;
    tutorials[idx].lesson_count = 0;
    tutorials[idx].progress = 0;
    tutorials[idx].certificate = 0;
    user_progress[idx] = 0;
    tut_count++;
    return tutorials[idx].tut_id;
}

/* Add lesson to tutorial */
int ts_add_lesson(int tut_id, const char *title, const char *desc, const char *code, int diff) {
    for (int i = 0; i < tut_count; i++) {
        if (tutorials[i].tut_id == tut_id) {
            int j = tutorials[i].lesson_count;
            if (j >= MAX_LESSONS) return -1;
            my_strncpy(tutorials[i].lessons[j].title, title, MAX_NAME - 1);
            my_strncpy(tutorials[i].lessons[j].description, desc, MAX_DESC - 1);
            my_strncpy(tutorials[i].lessons[j].code_example, code, MAX_CODE - 1);
            tutorials[i].lessons[j].difficulty = diff;
            tutorials[i].lessons[j].completed = 0;
            tutorials[i].lesson_count++;
            return j;
        }
    }
    return -1;
}

/* Start tutorial */
int ts_start(int tut_id) {
    for (int i = 0; i < tut_count; i++) {
        if (tutorials[i].tut_id == tut_id) {
            print_str("  Starting: ");
            print_str(tutorials[i].name);
            print_str(" [");
            print_str(diff_name(tutorials[i].difficulty));
            print_str("]\n");
            print_str("  Lessons: ");
            print_int(tutorials[i].lesson_count);
            print_str("\n");
            return tut_id;
        }
    }
    return -1;
}

/* Complete a lesson */
int ts_complete_lesson(int tut_id, int lesson_idx) {
    for (int i = 0; i < tut_count; i++) {
        if (tutorials[i].tut_id == tut_id && lesson_idx < tutorials[i].lesson_count) {
            if (!tutorials[i].lessons[lesson_idx].completed) {
                tutorials[i].lessons[lesson_idx].completed = 1;
                total_completed++;
                tutorials[i].progress = ((lesson_idx + 1) * 100) / tutorials[i].lesson_count;
                print_str("  Completed: ");
                print_str(tutorials[i].lessons[lesson_idx].title);
                print_str(" (progress: ");
                print_int(tutorials[i].progress);
                print_str("%)\n");
            }
            if (tutorials[i].progress >= 100 && !tutorials[i].certificate) {
                tutorials[i].certificate = 1;
                print_str("  Certificate earned: ");
                print_str(tutorials[i].name);
                print_str("\n");
            }
            return tutorials[i].progress;
        }
    }
    return -1;
}

/* List all tutorials */
int ts_list(void) {
    print_str("  Tutorial Catalog\n");
    print_str("  =================================================\n");
    for (int i = 0; i < tut_count; i++) {
        print_str("  [");
        print_str(diff_name(tutorials[i].difficulty));
        print_str("] ");
        print_str(tutorials[i].name);
        print_str(" (");
        print_int(tutorials[i].lesson_count);
        print_str(" lessons, ");
        print_int(tutorials[i].progress);
        print_str("%)");
        if (tutorials[i].certificate) print_str(" [CERT]");
        print_str("\n");
    }
    return tut_count;
}

/* Show progress */
int ts_progress(int tut_id) {
    for (int i = 0; i < tut_count; i++) {
        if (tutorials[i].tut_id == tut_id) {
            print_str("  Tutorial: ");
            print_str(tutorials[i].name);
            print_str("\n");
            print_str("  Progress: ");
            print_int(tutorials[i].progress);
            print_str("%\n");
            for (int j = 0; j < tutorials[i].lesson_count; j++) {
                print_str("    ");
                if (tutorials[i].lessons[j].completed) print_str("[x] ");
                else print_str("[ ] ");
                print_str(tutorials[i].lessons[j].title);
                print_str("\n");
            }
            if (tutorials[i].certificate) print_str("  Certificate: EARNED\n");
            return tutorials[i].progress;
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
    print_str("Tutorial System v32.0 - Interactive WASM Development Tutorials\n");
    if (help) {
        print_str("Usage: tutorial_system [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run tutorial test\n");
        return;
    }
    if (test) {
        print_str("=== Tutorial System Test ===\n\n");
        /* Create tutorials */
        int t1 = ts_create("WASM Basics", "Introduction to WebAssembly", DIFF_BEGINNER);
        int t2 = ts_create("Host Functions", "Using host_print, host_alloc, etc.", DIFF_BEGINNER);
        int t3 = ts_create("IPC Programming", "Pipes, message queues, shared memory", DIFF_INTERMEDIATE);
        int t4 = ts_create("Agent Development", "Building AI agents", DIFF_ADVANCED);
        print_str("  Created: "); print_int(tut_count); print_str(" tutorials\n\n");
        /* Add lessons */
        ts_add_lesson(t1, "Hello World", "Your first WASM module", "void _start() { host_print(\"hello\\n\"); }", DIFF_BEGINNER);
        ts_add_lesson(t1, "CLI Parsing", "Command-line argument handling", "host_get_argv(buf, 512);", DIFF_BEGINNER);
        ts_add_lesson(t1, "String Utils", "String operations in WASM", "my_strlen, my_strcmp", DIFF_BEGINNER);
        ts_add_lesson(t2, "host_print", "Printing to console", "host_print(str);", DIFF_BEGINNER);
        ts_add_lesson(t2, "host_alloc", "Memory allocation", "host_alloc(size, align);", DIFF_BEGINNER);
        ts_add_lesson(t3, "Pipes", "Pipe creation and I/O", "host_pipe_create(id, flags);", DIFF_INTERMEDIATE);
        ts_add_lesson(t4, "Agent Runtime", "Creating agents", "agent_create(\"name\");", DIFF_ADVANCED);
        print_str("\n");
        /* Start and complete */
        ts_start(t1);
        ts_complete_lesson(t1, 0);
        ts_complete_lesson(t1, 1);
        ts_complete_lesson(t1, 2);
        print_str("\n");
        /* Progress */
        ts_progress(t1);
        print_str("\n");
        /* List all */
        ts_list();
        print_str("\n=== Tutorial System Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
