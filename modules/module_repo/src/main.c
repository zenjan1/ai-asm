/* module_repo: Local module repository service (v31.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_MODULES  256
#define MAX_NAME     64
#define MAX_VERSION  16
#define MAX_DESC     128
#define MAX_CATEGORY 32
#define CACHE_SIZE   128

/* Module entry in repository */
typedef struct {
    int   mod_id;
    char  name[MAX_NAME];
    char  version[MAX_VERSION];
    char  category[MAX_CATEGORY];
    char  description[MAX_DESC];
    int   downloads;
    int   rating;       /* 1-5 stars */
    int   synced;       /* 0=local, 1=synced */
} repo_module_t;

/* Cache entry */
typedef struct {
    char name[MAX_NAME];
    int  last_access;
} cache_entry_t;

static repo_module_t repo_modules[MAX_MODULES];
static int repo_count = 0;
static int next_mod_id = 1;
static cache_entry_t cache[CACHE_SIZE];
static int cache_pos = 0;

/* String utilities */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
}
static int my_strncmp(const char *a, const char *b, int n) {
    for (int i = 0; i < n; i++) {
        if (!a[i] || !b[i] || a[i] != b[i]) return (a[i] ? a[i] : 0) - (b[i] ? b[i] : 0);
    } return 0;
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

/* Register module in repo */
int repo_register(const char *name, const char *version, const char *category, const char *desc) {
    if (repo_count >= MAX_MODULES) return -1;
    int idx = repo_count;
    repo_modules[idx].mod_id = next_mod_id++;
    my_strncpy(repo_modules[idx].name, name, MAX_NAME - 1);
    my_strncpy(repo_modules[idx].version, version, MAX_VERSION - 1);
    my_strncpy(repo_modules[idx].category, category, MAX_CATEGORY - 1);
    my_strncpy(repo_modules[idx].description, desc, MAX_DESC - 1);
    repo_modules[idx].downloads = 0;
    repo_modules[idx].rating = 0;
    repo_modules[idx].synced = 0;
    repo_count++;
    return repo_modules[idx].mod_id;
}

/* Search modules by name pattern */
int repo_search(const char *pattern) {
    print_str("  Search: \"");
    print_str(pattern);
    print_str("\"\n");
    print_str("  Results:\n");
    int found = 0;
    for (int i = 0; i < repo_count; i++) {
        /* Simple substring match */
        int name_len = my_strlen(repo_modules[i].name);
        int pat_len = my_strlen(pattern);
        for (int j = 0; j <= name_len - pat_len; j++) {
            if (my_strncmp(&repo_modules[i].name[j], pattern, pat_len) == 0) {
                print_str("    ");
                print_str(repo_modules[i].name);
                print_str(" v");
                print_str(repo_modules[i].version);
                print_str(" [");
                print_str(repo_modules[i].category);
                print_str("] ");
                print_str(repo_modules[i].description);
                print_str("\n");
                found++;
                break;
            }
        }
    }
    print_str("  Found: ");
    print_int(found);
    print_str(" modules\n");
    return found;
}

/* Compare versions: -1=a<b, 0=a==b, 1=a>b */
static int version_compare(const char *a, const char *b) {
    int va = 0, vb = 0;
    /* Parse major.minor.patch */
    while (*a && *a != '.') { va = va * 10 + (*a - '0'); a++; }
    if (*a == '.') a++;
    while (*a && *a != '.') { va = va * 10 + (*a - '0'); a++; }
    if (*a == '.') a++;
    while (*a) { va = va * 10 + (*a - '0'); a++; }

    while (*b && *b != '.') { vb = vb * 10 + (*b - '0'); b++; }
    if (*b == '.') b++;
    while (*b && *b != '.') { vb = vb * 10 + (*b - '0'); b++; }
    if (*b == '.') b++;
    while (*b) { vb = vb * 10 + (*b - '0'); b++; }

    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* List by category */
int repo_list_category(const char *category) {
    print_str("  Category: ");
    print_str(category);
    print_str("\n");
    int count = 0;
    for (int i = 0; i < repo_count; i++) {
        if (my_strcmp(repo_modules[i].category, category) == 0) {
            print_str("    v");
            print_str(repo_modules[i].version);
            print_str(" ");
            print_str(repo_modules[i].name);
            print_str(" - ");
            print_str(repo_modules[i].description);
            print_str(" (");
            print_int(repo_modules[i].downloads);
            print_str(" downloads)\n");
            count++;
        }
    }
    return count;
}

/* Sync with remote repository (simulated) */
int repo_sync(const char *remote_url) {
    print_str("  Syncing with: ");
    print_str(remote_url);
    print_str("\n");
    int synced = 0;
    for (int i = 0; i < repo_count; i++) {
        if (!repo_modules[i].synced) {
            repo_modules[i].synced = 1;
            synced++;
        }
    }
    print_str("  Synced: ");
    print_int(synced);
    print_str(" modules\n");
    return synced;
}

/* Cache management */
void repo_cache_add(const char *name) {
    int idx = cache_pos % CACHE_SIZE;
    my_strncpy(cache[idx].name, name, MAX_NAME - 1);
    cache[idx].last_access = repo_count;
    cache_pos++;
}

/* Summary statistics */
int repo_stats(void) {
    print_str("  Repository Statistics\n");
    print_str("  Total modules: ");
    print_int(repo_count);
    print_str("\n");
    /* Count categories */
    int pos_count = 0, net_count = 0, ai_count = 0, sys_count = 0, other = 0;
    for (int i = 0; i < repo_count; i++) {
        if (my_strncmp(repo_modules[i].category, "posix", 5) == 0) pos_count++;
        else if (my_strncmp(repo_modules[i].category, "network", 7) == 0) net_count++;
        else if (my_strncmp(repo_modules[i].category, "ai", 2) == 0) ai_count++;
        else if (my_strncmp(repo_modules[i].category, "system", 6) == 0) sys_count++;
        else other++;
    }
    print_str("  POSIX tools: "); print_int(pos_count); print_str("\n");
    print_str("  Network: "); print_int(net_count); print_str("\n");
    print_str("  AI/AGI: "); print_int(ai_count); print_str("\n");
    print_str("  System: "); print_int(sys_count); print_str("\n");
    print_str("  Other: "); print_int(other); print_str("\n");
    int total_dl = 0;
    for (int i = 0; i < repo_count; i++) total_dl += repo_modules[i].downloads;
    print_str("  Total downloads: "); print_int(total_dl); print_str("\n");
    return repo_count;
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
    print_str("Module Repository v31.0 - WASM Module Index & Search\n");
    if (help) {
        print_str("Usage: module_repo [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run repository test\n");
        return;
    }
    if (test) {
        print_str("=== Module Repository Test ===\n\n");
        /* Register modules */
        print_str("Registering modules:\n");
        repo_register("ls", "1.0.0", "posix", "List directory contents");
        repo_register("grep", "1.2.0", "posix", "Pattern matching");
        repo_register("wget", "2.0.0", "network", "HTTP download tool");
        repo_register("ai_bridge", "3.0.0", "ai", "AI model interface");
        repo_register("agent_runtime", "1.5.0", "system", "Agent execution engine");
        print_str("  Registered: "); print_int(repo_count); print_str(" modules\n\n");
        /* Search */
        repo_search("grep");
        print_str("\n");
        repo_search("ai");
        print_str("\n");
        /* Category listing */
        repo_list_category("posix");
        print_str("\n");
        /* Sync */
        repo_sync("https://repo.aiasm.io/v1");
        print_str("\n");
        /* Stats */
        repo_stats();
        print_str("\n=== Module Repository Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
