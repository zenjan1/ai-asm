/* pkg_manager: WASM package manager (v31.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_PACKAGES  128
#define MAX_DEPS      16
#define MAX_NAME      64
#define MAX_VERSION   16
#define MAX_DESC      128

/* Package states */
#define PKG_NOT_INSTALLED  0
#define PKG_INSTALLED      1
#define PKG_UPGRADEABLE    2
#define PKG_CONFLICT       3

/* Dependency node */
typedef struct {
    char name[MAX_NAME];
    char version[MAX_VERSION];
} dep_node_t;

/* Package entry */
typedef struct {
    int   pkg_id;
    char  name[MAX_NAME];
    char  version[MAX_VERSION];
    char  description[MAX_DESC];
    int   state;
    int   dep_count;
    dep_node_t deps[MAX_DEPS];
} package_t;

static package_t packages[MAX_PACKAGES];
static int package_count = 0;
static int next_id = 1;

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

/* Find package by name */
static int find_package(const char *name) {
    for (int i = 0; i < package_count; i++) {
        if (my_strcmp(packages[i].name, name) == 0) return i;
    }
    return -1;
}

/* Add package with dependencies */
int pm_add(const char *name, const char *version, const char *desc) {
    if (package_count >= MAX_PACKAGES) return -1;
    int idx = find_package(name);
    if (idx >= 0) {
        /* Update existing */
        my_strncpy(packages[idx].version, version, MAX_VERSION - 1);
        my_strncpy(packages[idx].description, desc, MAX_DESC - 1);
        packages[idx].state = PKG_INSTALLED;
        return packages[idx].pkg_id;
    }
    int i = package_count;
    packages[i].pkg_id = next_id++;
    my_strncpy(packages[i].name, name, MAX_NAME - 1);
    my_strncpy(packages[i].version, version, MAX_VERSION - 1);
    my_strncpy(packages[i].description, desc, MAX_DESC - 1);
    packages[i].state = PKG_NOT_INSTALLED;
    packages[i].dep_count = 0;
    package_count++;
    return packages[i].pkg_id;
}

/* Add dependency */
int pm_add_dep(int pkg_id, const char *dep_name, const char *dep_version) {
    for (int i = 0; i < package_count; i++) {
        if (packages[i].pkg_id == pkg_id) {
            if (packages[i].dep_count >= MAX_DEPS) return -1;
            int j = packages[i].dep_count;
            my_strncpy(packages[i].deps[j].name, dep_name, MAX_NAME - 1);
            my_strncpy(packages[i].deps[j].version, dep_version, MAX_VERSION - 1);
            packages[i].dep_count++;
            return 0;
        }
    }
    return -1;
}

/* Resolve dependencies */
int pm_resolve_deps(int pkg_id) {
    for (int i = 0; i < package_count; i++) {
        if (packages[i].pkg_id == pkg_id) {
            print_str("  Resolving dependencies for: ");
            print_str(packages[i].name);
            print_str("\n");
            for (int j = 0; j < packages[i].dep_count; j++) {
                print_str("    - ");
                print_str(packages[i].deps[j].name);
                print_str(" (");
                print_str(packages[i].deps[j].version);
                int dep_idx = find_package(packages[i].deps[j].name);
                if (dep_idx >= 0 && packages[dep_idx].state == PKG_INSTALLED) {
                    print_str(") satisfied\n");
                } else {
                    print_str(") missing -> auto-install\n");
                }
            }
            return 0;
        }
    }
    return -1;
}

/* Install package */
int pm_install(const char *name, const char *version) {
    int idx = find_package(name);
    if (idx >= 0) {
        if (packages[idx].state == PKG_INSTALLED) {
            print_str("  Already installed: ");
            print_str(name);
            print_str("\n");
            return packages[idx].pkg_id;
        }
        if (version) my_strncpy(packages[idx].version, version, MAX_VERSION - 1);
        pm_resolve_deps(packages[idx].pkg_id);
        packages[idx].state = PKG_INSTALLED;
        print_str("  Installed: ");
        print_str(name);
        print_str(" v");
        print_str(packages[idx].version);
        print_str("\n");
        return packages[idx].pkg_id;
    }
    print_str("  Package not found: ");
    print_str(name);
    print_str("\n");
    return -1;
}

/* Uninstall package */
int pm_uninstall(const char *name) {
    int idx = find_package(name);
    if (idx < 0) return -1;
    /* Check reverse dependencies */
    for (int i = 0; i < package_count; i++) {
        for (int j = 0; j < packages[i].dep_count; j++) {
            if (my_strcmp(packages[i].deps[j].name, name) == 0 &&
                packages[i].state == PKG_INSTALLED) {
                print_str("  Conflict: ");
                print_str(packages[i].name);
                print_str(" depends on ");
                print_str(name);
                print_str("\n");
                packages[idx].state = PKG_CONFLICT;
                return -1;
            }
        }
    }
    packages[idx].state = PKG_NOT_INSTALLED;
    print_str("  Uninstalled: ");
    print_str(name);
    print_str("\n");
    return 0;
}

/* List all packages */
int pm_list(void) {
    print_str("  Package List\n");
    print_str("  =================================================\n");
    print_str("  ID  Name                  Version   Status\n");
    print_str("  -------------------------------------------------\n");
    for (int i = 0; i < package_count; i++) {
        print_int(packages[i].pkg_id);
        print_str("  ");
        print_str(packages[i].name);
        /* Padding */
        int len = my_strlen(packages[i].name);
        for (int p = 0; p < 22 - len; p++) print_str(" ");
        print_str("v");
        print_str(packages[i].version);
        print_str("    ");
        if (packages[i].state == PKG_INSTALLED) print_str("installed");
        else if (packages[i].state == PKG_UPGRADEABLE) print_str("upgradeable");
        else if (packages[i].state == PKG_CONFLICT) print_str("conflict");
        else print_str("available");
        print_str("\n");
    }
    print_str("  Total: ");
    print_int(package_count);
    print_str(" packages\n");
    return package_count;
}

/* Check for upgrades */
int pm_check_upgrades(void) {
    print_str("  Checking for upgrades...\n");
    int upgradable = 0;
    for (int i = 0; i < package_count; i++) {
        if (packages[i].state == PKG_INSTALLED) {
            /* Simulate upgrade check */
            if (packages[i].pkg_id % 3 == 0) {
                packages[i].state = PKG_UPGRADEABLE;
                print_str("  Upgradeable: ");
                print_str(packages[i].name);
                print_str(" v");
                print_str(packages[i].version);
                print_str(" -> v");
                print_int(packages[i].pkg_id + 2);
                print_str(".0.0\n");
                upgradable++;
            }
        }
    }
    print_str("  ");
    print_int(upgradable);
    print_str(" packages can be upgraded\n");
    return upgradable;
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
    print_str("Package Manager v31.0 - WASM Module Package Management\n");
    if (help) {
        print_str("Usage: pkg_manager [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run package manager test\n");
        return;
    }
    if (test) {
        print_str("=== Package Manager Test ===\n\n");
        /* Add packages */
        print_str("Registering packages:\n");
        int p1 = pm_add("ls", "1.0.0", "List directory contents");
        int p2 = pm_add("grep", "1.2.0", "Pattern matching tool");
        int p3 = pm_add("awk", "2.0.0", "Pattern processing language");
        pm_add_dep(p3, "grep", "1.0.0");
        print_str("  Package IDs: ls=");
        print_int(p1);
        print_str(" grep=");
        print_int(p2);
        print_str(" awk=");
        print_int(p3);
        print_str("\n\n");
        /* Install */
        print_str("Installing packages:\n");
        pm_install("ls", "1.0.0");
        pm_install("grep", "1.2.0");
        pm_install("awk", "2.0.0");
        print_str("\n");
        /* List */
        pm_list();
        print_str("\n");
        /* Upgrade check */
        pm_check_upgrades();
        print_str("\n");
        /* Uninstall test */
        print_str("Uninstall test:\n");
        pm_uninstall("grep");
        pm_uninstall("awk");  /* Should show conflict */
        print_str("\n");
        /* Final list */
        pm_list();
        print_str("\n=== Package Manager Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
