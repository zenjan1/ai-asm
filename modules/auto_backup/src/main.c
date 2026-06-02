/* auto_backup: Automated backup system (v33.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_BACKUPS    64
#define MAX_PATH       128
#define MAX_NAME       64

/* Backup types */
#define BACKUP_FULL    0
#define BACKUP_INCR    1
#define BACKUP_DIFF    2

/* Backup states */
#define BACKUP_PENDING   0
#define BACKUP_RUNNING   1
#define BACKUP_COMPLETE  2
#define BACKUP_FAILED    3

/* Backup schedule */
#define SCHEDULE_NONE    0
#define SCHEDULE_HOURLY  1
#define SCHEDULE_DAILY   2
#define SCHEDULE_WEEKLY  3

/* Backup entry */
typedef struct {
    int   backup_id;
    char  name[MAX_NAME];
    char  path[MAX_PATH];
    int   type;            /* full/incr/diff */
    int   state;
    int   size_kb;
    int   timestamp;
    int   verified;        /* 0=unverified, 1=verified */
    int   parent_id;       /* parent backup for incremental */
} backup_t;

static backup_t backups[MAX_BACKUPS];
static int backup_count = 0;
static int next_backup_id = 1;
static int schedule = SCHEDULE_DAILY;

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

static const char *type_name(int t) {
    if (t == BACKUP_FULL) return "full";
    if (t == BACKUP_INCR) return "incremental";
    return "differential";
}

static const char *state_name(int s) {
    if (s == BACKUP_PENDING) return "pending";
    if (s == BACKUP_RUNNING) return "running";
    if (s == BACKUP_COMPLETE) return "complete";
    return "failed";
}

/* Create backup */
int ab_create(const char *name, const char *path, int type) {
    if (backup_count >= MAX_BACKUPS) return -1;
    int idx = backup_count;
    backups[idx].backup_id = next_backup_id++;
    my_strncpy(backups[idx].name, name, MAX_NAME - 1);
    my_strncpy(backups[idx].path, path, MAX_PATH - 1);
    backups[idx].type = type;
    backups[idx].state = BACKUP_PENDING;
    backups[idx].size_kb = 0;
    backups[idx].timestamp = backup_count + 1;
    backups[idx].verified = 0;
    backups[idx].parent_id = -1;
    /* For incremental, find last full backup */
    if (type == BACKUP_INCR) {
        for (int i = backup_count - 1; i >= 0; i--) {
            if (backups[i].type == BACKUP_FULL && backups[i].state == BACKUP_COMPLETE) {
                backups[idx].parent_id = backups[i].backup_id;
                break;
            }
        }
    }
    backup_count++;
    return backups[idx].backup_id;
}

/* Execute backup */
int ab_execute(int backup_id) {
    for (int i = 0; i < backup_count; i++) {
        if (backups[i].backup_id == backup_id) {
            backups[i].state = BACKUP_RUNNING;
            print_str("  Executing backup: ");
            print_str(backups[i].name);
            print_str(" (");
            print_str(type_name(backups[i].type));
            print_str(")\n");
            /* Simulate backup */
            if (backups[i].type == BACKUP_FULL) {
                backups[i].size_kb = 2048;
                print_str("    Full backup: 2048 KB\n");
            } else if (backups[i].type == BACKUP_INCR) {
                backups[i].size_kb = 256;
                print_str("    Incremental: 256 KB\n");
                if (backups[i].parent_id >= 0) {
                    print_str("    Parent: #");
                    print_int(backups[i].parent_id);
                    print_str("\n");
                }
            } else {
                backups[i].size_kb = 512;
                print_str("    Differential: 512 KB\n");
            }
            print_str("    Compressing...\n");
            print_str("    Writing to: ");
            print_str(backups[i].path);
            print_str("\n");
            backups[i].state = BACKUP_COMPLETE;
            return 0;
        }
    }
    return -1;
}

/* Verify backup */
int ab_verify(int backup_id) {
    for (int i = 0; i < backup_count; i++) {
        if (backups[i].backup_id == backup_id) {
            if (backups[i].state != BACKUP_COMPLETE) return -1;
            backups[i].verified = 1;
            print_str("  Verified backup: ");
            print_str(backups[i].name);
            print_str(" checksum OK\n");
            return 0;
        }
    }
    return -1;
}

/* Restore from backup */
int ab_restore(int backup_id) {
    for (int i = 0; i < backup_count; i++) {
        if (backups[i].backup_id == backup_id) {
            if (backups[i].state != BACKUP_COMPLETE) {
                print_str("  Cannot restore: backup not complete\n");
                return -1;
            }
            print_str("  Restoring from: ");
            print_str(backups[i].name);
            print_str(" (");
            print_int(backups[i].size_kb);
            print_str(" KB)\n");
            print_str("  Decompressing...\n");
            print_str("  Restoring files...\n");
            print_str("  Restore complete\n");
            return 0;
        }
    }
    return -1;
}

/* List backups */
int ab_list(void) {
    print_str("  Backup History\n");
    print_str("  ============================================================\n");
    print_str("  ID  Name               Type          Size(KB)  State      Verified\n");
    print_str("  ------------------------------------------------------------\n");
    int total_kb = 0;
    for (int i = 0; i < backup_count; i++) {
        print_int(backups[i].backup_id);
        print_str("  ");
        print_str(backups[i].name);
        int len = my_strlen(backups[i].name);
        for (int p = 0; p < 19 - len; p++) print_str(" ");
        print_str(type_name(backups[i].type));
        if (backups[i].type == BACKUP_FULL) print_str(" ");
        else if (backups[i].type == BACKUP_INCR) print_str("       ");
        else print_str("          ");
        print_int(backups[i].size_kb);
        print_str("      ");
        print_str(state_name(backups[i].state));
        if (backups[i].state == BACKUP_COMPLETE) print_str("  ");
        else print_str("   ");
        if (backups[i].verified) print_str("YES");
        else print_str("NO");
        print_str("\n");
        total_kb += backups[i].size_kb;
    }
    print_str("  Total storage: ");
    print_int(total_kb);
    print_str(" KB\n");
    return backup_count;
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
    print_str("Auto Backup v33.0 - Automated Backup System\n");
    if (help) {
        print_str("Usage: auto_backup [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run backup test\n");
        return;
    }
    if (test) {
        print_str("=== Auto Backup Test ===\n\n");
        /* Create backups */
        int b1 = ab_create("full_backup_001", "/backup/full_001.tar.gz", BACKUP_FULL);
        int b2 = ab_create("incr_backup_002", "/backup/incr_002.tar.gz", BACKUP_INCR);
        int b3 = ab_create("incr_backup_003", "/backup/incr_003.tar.gz", BACKUP_INCR);
        int b4 = ab_create("diff_backup_004", "/backup/diff_004.tar.gz", BACKUP_DIFF);
        print_str("  Created: "); print_int(backup_count); print_str(" backups\n\n");
        /* Execute all */
        ab_execute(b1); print_str("\n");
        ab_execute(b2); print_str("\n");
        ab_execute(b3); print_str("\n");
        ab_execute(b4); print_str("\n");
        /* Verify */
        ab_verify(b1);
        ab_verify(b2);
        print_str("\n");
        /* List */
        ab_list();
        print_str("\n");
        /* Restore test */
        print_str("Restore test:\n");
        ab_restore(b1);
        print_str("\n=== Auto Backup Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
