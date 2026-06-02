/* debug_tools: WASM debugging and profiling tools (v32.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_BREAKPOINTS  32
#define MAX_MEM_TRACKS   64
#define MAX_PROFILES     16
#define MAX_NAME         64

/* Breakpoint */
typedef struct {
    int   bp_id;
    int   address;
    int   enabled;
    int   hit_count;
} breakpoint_t;

/* Memory tracker */
typedef struct {
    int   alloc_id;
    int   address;
    int   size;
    int   freed;
} mem_track_t;

/* Profile entry */
typedef struct {
    char  name[MAX_NAME];
    int   call_count;
    int   total_time_us;
    int   min_time_us;
    int   max_time_us;
} profile_t;

static breakpoint_t breakpoints[MAX_BREAKPOINTS];
static int bp_count = 0;
static int next_bp_id = 1;

static mem_track_t mem_tracks[MAX_MEM_TRACKS];
static int mem_count = 0;
static int next_alloc_id = 1;
static int total_alloc = 0;
static int total_free = 0;

static profile_t profiles[MAX_PROFILES];
static int profile_count = 0;

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

/* Breakpoint management */
int dt_set_bp(int address) {
    if (bp_count >= MAX_BREAKPOINTS) return -1;
    int idx = bp_count;
    breakpoints[idx].bp_id = next_bp_id++;
    breakpoints[idx].address = address;
    breakpoints[idx].enabled = 1;
    breakpoints[idx].hit_count = 0;
    bp_count++;
    print_str("  Breakpoint set at: 0x");
    print_int(address);
    print_str("\n");
    return breakpoints[idx].bp_id;
}

int dt_clear_bp(int bp_id) {
    for (int i = 0; i < bp_count; i++) {
        if (breakpoints[i].bp_id == bp_id) {
            breakpoints[i].enabled = 0;
            print_str("  Breakpoint cleared: #");
            print_int(bp_id);
            print_str("\n");
            return 0;
        }
    }
    return -1;
}

int dt_hit_bp(int bp_id) {
    for (int i = 0; i < bp_count; i++) {
        if (breakpoints[i].bp_id == bp_id && breakpoints[i].enabled) {
            breakpoints[i].hit_count++;
            return breakpoints[i].hit_count;
        }
    }
    return 0;
}

/* Memory tracking */
int dt_track_alloc(int address, int size) {
    if (mem_count >= MAX_MEM_TRACKS) return -1;
    int idx = mem_count;
    mem_tracks[idx].alloc_id = next_alloc_id++;
    mem_tracks[idx].address = address;
    mem_tracks[idx].size = size;
    mem_tracks[idx].freed = 0;
    mem_count++;
    total_alloc += size;
    return mem_tracks[idx].alloc_id;
}

int dt_track_free(int alloc_id) {
    for (int i = 0; i < mem_count; i++) {
        if (mem_tracks[i].alloc_id == alloc_id && !mem_tracks[i].freed) {
            mem_tracks[i].freed = 1;
            total_free += mem_tracks[i].size;
            return 0;
        }
    }
    return -1;
}

int dt_leak_report(void) {
    int leaks = 0;
    int leaked_bytes = 0;
    print_str("  Memory Leak Report\n");
    print_str("  =================================================\n");
    for (int i = 0; i < mem_count; i++) {
        if (!mem_tracks[i].freed) {
            print_str("  LEAK #");
            print_int(mem_tracks[i].alloc_id);
            print_str(" addr=0x");
            print_int(mem_tracks[i].address);
            print_str(" size=");
            print_int(mem_tracks[i].size);
            print_str(" bytes\n");
            leaks++;
            leaked_bytes += mem_tracks[i].size;
        }
    }
    print_str("  Total leaks: ");
    print_int(leaks);
    print_str(" (");
    print_int(leaked_bytes);
    print_str(" bytes)\n");
    print_str("  Total allocated: ");
    print_int(total_alloc);
    print_str("\n");
    print_str("  Total freed: ");
    print_int(total_free);
    print_str("\n");
    return leaks;
}

/* Profiling */
int dt_profile_start(const char *name) {
    if (profile_count >= MAX_PROFILES) return -1;
    for (int i = 0; i < profile_count; i++) {
        if (my_strcmp(profiles[i].name, name) == 0) {
            return i;  /* Already exists */
        }
    }
    int idx = profile_count;
    my_strncpy(profiles[idx].name, name, MAX_NAME - 1);
    profiles[idx].call_count = 0;
    profiles[idx].total_time_us = 0;
    profiles[idx].min_time_us = 999999;
    profiles[idx].max_time_us = 0;
    profile_count++;
    return idx;
}

int dt_profile_stop(const char *name, int elapsed_us) {
    for (int i = 0; i < profile_count; i++) {
        if (my_strcmp(profiles[i].name, name) == 0) {
            profiles[i].call_count++;
            profiles[i].total_time_us += elapsed_us;
            if (elapsed_us < profiles[i].min_time_us) profiles[i].min_time_us = elapsed_us;
            if (elapsed_us > profiles[i].max_time_us) profiles[i].max_time_us = elapsed_us;
            return 0;
        }
    }
    return -1;
}

int dt_profile_report(void) {
    print_str("  Profile Report\n");
    print_str("  =================================================\n");
    print_str("  Name                 Calls   Total(us)  Avg(us)  Max(us)\n");
    print_str("  -------------------------------------------------\n");
    for (int i = 0; i < profile_count; i++) {
        print_str("  ");
        print_str(profiles[i].name);
        int len = my_strlen(profiles[i].name);
        for (int p = 0; p < 21 - len; p++) print_str(" ");
        print_int(profiles[i].call_count);
        print_str("     ");
        print_int(profiles[i].total_time_us);
        print_str("      ");
        int avg = profiles[i].call_count > 0 ? profiles[i].total_time_us / profiles[i].call_count : 0;
        print_int(avg);
        print_str("      ");
        print_int(profiles[i].max_time_us);
        print_str("\n");
    }
    return profile_count;
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
    print_str("Debug Tools v32.0 - WASM Debugging & Profiling\n");
    if (help) {
        print_str("Usage: debug_tools [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run debug tools test\n");
        return;
    }
    if (test) {
        print_str("=== Debug Tools Test ===\n\n");
        /* Breakpoints */
        print_str("Setting breakpoints:\n");
        int bp1 = dt_set_bp(0x1000);
        int bp2 = dt_set_bp(0x2000);
        int bp3 = dt_set_bp(0x3000);
        print_str("\n");
        /* Hit breakpoints */
        print_str("Hitting breakpoints:\n");
        dt_hit_bp(bp1);
        dt_hit_bp(bp1);
        dt_hit_bp(bp2);
        print_str("\n");
        /* Clear breakpoint */
        dt_clear_bp(bp3);
        print_str("\n");
        /* Memory tracking */
        print_str("Memory tracking:\n");
        int a1 = dt_track_alloc(0x10000, 256);
        int a2 = dt_track_alloc(0x10100, 512);
        int a3 = dt_track_alloc(0x10300, 128);
        int a4 = dt_track_alloc(0x10400, 1024);
        dt_track_free(a1);
        dt_track_free(a3);
        print_str("\n");
        dt_leak_report();
        print_str("\n");
        /* Profiling */
        print_str("Profiling:\n");
        int pi1 = dt_profile_start("host_print");
        int pi2 = dt_profile_start("host_alloc");
        int pi3 = dt_profile_start("fs_read");
        dt_profile_stop("host_print", 12);
        dt_profile_stop("host_print", 8);
        dt_profile_stop("host_print", 15);
        dt_profile_stop("host_alloc", 45);
        dt_profile_stop("host_alloc", 38);
        dt_profile_stop("fs_read", 120);
        print_str("\n");
        dt_profile_report();
        print_str("\n=== Debug Tools Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
