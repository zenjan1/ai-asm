/* license_manager: License management for commercial support (v15.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* License types */
#define LICENSE_FREE      0
#define LICENSE_BASIC     1
#define LICENSE_PRO       2
#define LICENSE_ENTERPRISE 3
#define LICENSE_EDUCATION 4

/* Feature flags */
#define FEATURE_MAX_AGENTS     0x01
#define FEATURE_MAX_CONTAINERS 0x02
#define FEATURE_AI_ROUTING     0x04
#define FEATURE_EDGE_AI        0x08
#define FEATURE_PRIORITY_SUP   0x10
#define FEATURE_SLA            0x20

/* Limits */
#define MAX_FEATURES      32
#define MAX_LICENSES      8
#define SERIAL_KEY_LEN    64
#define OWNER_LEN         128
#define EMAIL_LEN         64
#define FEATURE_NAME_LEN  64

/* License info */
typedef struct {
    int   license_id;
    char  serial_key[SERIAL_KEY_LEN];
    int   type;
    char  owner[OWNER_LEN];
    char  email[EMAIL_LEN];
    int   expire_date;      /* days from epoch */
    int   features_enabled;
    int   max_agents;
    int   max_containers;
    int   support_level;
    int   active;
} license_t;

/* Feature definition */
typedef struct {
    int   feature_id;
    char  name[FEATURE_NAME_LEN];
    int   min_license;
    int   enabled;
} feature_t;

static license_t licenses[MAX_LICENSES];
static feature_t features[MAX_FEATURES];
static int license_count = 0;
static int feature_count = 0;
static int active_license = -1;

/* String utilities */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
}
static void my_strcpy(char *dst, const char *src) { while (*src) *dst++ = *src++; *dst = '\0'; }
static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0; while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; } dst[i] = '\0';
}
static int my_strncmp(const char *a, const char *b, int n) {
    for (int i = 0; i < n; i++) {
        if (!a[i] || !b[i]) return (a[i] ? 1 : 0) - (b[i] ? 1 : 0);
        if (a[i] != b[i]) return a[i] - b[i];
    } return 0;
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

/* Get license type name */
static const char *license_type_name(int type) {
    if (type == LICENSE_FREE) return "free";
    if (type == LICENSE_BASIC) return "basic";
    if (type == LICENSE_PRO) return "pro";
    if (type == LICENSE_ENTERPRISE) return "enterprise";
    if (type == LICENSE_EDUCATION) return "education";
    return "unknown";
}

/* Get license level from serial key (simulated validation) */
static int serial_to_license_type(const char *key) {
    if (my_strncmp(key, "ENT-", 4) == 0) return LICENSE_ENTERPRISE;
    if (my_strncmp(key, "PRO-", 4) == 0) return LICENSE_PRO;
    if (my_strncmp(key, "BSC-", 4) == 0) return LICENSE_BASIC;
    if (my_strncmp(key, "EDU-", 4) == 0) return LICENSE_EDUCATION;
    return LICENSE_FREE;
}

/* Get features enabled for license type */
static int features_for_type(int type) {
    int f = 0;
    if (type >= LICENSE_BASIC)     f |= FEATURE_MAX_AGENTS;
    if (type >= LICENSE_PRO)       f |= FEATURE_MAX_CONTAINERS | FEATURE_AI_ROUTING | FEATURE_EDGE_AI;
    if (type >= LICENSE_ENTERPRISE) f |= FEATURE_PRIORITY_SUP | FEATURE_SLA;
    return f;
}

/* Get max agents for license type */
static int max_agents_for_type(int type) {
    if (type == LICENSE_FREE) return 1;
    if (type == LICENSE_BASIC) return 4;
    if (type == LICENSE_PRO) return 16;
    if (type == LICENSE_ENTERPRISE) return 256;
    if (type == LICENSE_EDUCATION) return 8;
    return 1;
}

/* Validate and activate a license */
int license_validate(const char *serial_key) {
    if (!serial_key) return -1;
    print_str("[license] validating: ");
    print_str(serial_key);
    print_str("\n");

    int type = serial_to_license_type(serial_key);
    print_str("[license] type: ");
    print_str(license_type_name(type));
    print_str("\n");

    /* Check if already registered */
    for (int i = 0; i < license_count; i++) {
        if (my_strcmp(licenses[i].serial_key, serial_key) == 0) {
            print_str("[license] license already registered\n");
            return 0;
        }
    }

    if (license_count >= MAX_LICENSES) {
        print_str("[license] max licenses reached\n");
        return -1;
    }

    /* Register license */
    int idx = license_count;
    licenses[idx].license_id = idx + 1;
    my_strncpy(licenses[idx].serial_key, serial_key, SERIAL_KEY_LEN - 1);
    licenses[idx].type = type;
    licenses[idx].features_enabled = features_for_type(type);
    licenses[idx].max_agents = max_agents_for_type(type);
    licenses[idx].max_containers = type >= LICENSE_PRO ? 32 : (type >= LICENSE_BASIC ? 8 : 2);
    licenses[idx].support_level = type;
    licenses[idx].active = 1;
    licenses[idx].expire_date = 365; /* 1 year default */
    my_strcpy(licenses[idx].owner, "registered_user");
    my_strcpy(licenses[idx].email, "user@example.com");

    license_count++;
    active_license = idx;

    print_str("[license] validated successfully\n");
    return 0;
}

/* Get license info */
int license_get_info(license_t *info) {
    if (!info || active_license < 0) return -1;
    info->license_id = licenses[active_license].license_id;
    my_strcpy(info->serial_key, licenses[active_license].serial_key);
    info->type = licenses[active_license].type;
    my_strcpy(info->owner, licenses[active_license].owner);
    my_strcpy(info->email, licenses[active_license].email);
    info->expire_date = licenses[active_license].expire_date;
    info->features_enabled = licenses[active_license].features_enabled;
    info->max_agents = licenses[active_license].max_agents;
    info->max_containers = licenses[active_license].max_containers;
    info->support_level = licenses[active_license].support_level;
    return 0;
}

/* Check if a feature is available */
int license_check_feature(const char *feature_name) {
    if (active_license < 0) return 0;
    int feat_mask = licenses[active_license].features_enabled;

    if (my_strcmp(feature_name, "max_agents") == 0) return (feat_mask & FEATURE_MAX_AGENTS) ? 1 : 0;
    if (my_strcmp(feature_name, "max_containers") == 0) return (feat_mask & FEATURE_MAX_CONTAINERS) ? 1 : 0;
    if (my_strcmp(feature_name, "ai_routing") == 0) return (feat_mask & FEATURE_AI_ROUTING) ? 1 : 0;
    if (my_strcmp(feature_name, "edge_ai") == 0) return (feat_mask & FEATURE_EDGE_AI) ? 1 : 0;
    if (my_strcmp(feature_name, "priority_support") == 0) return (feat_mask & FEATURE_PRIORITY_SUP) ? 1 : 0;
    if (my_strcmp(feature_name, "sla") == 0) return (feat_mask & FEATURE_SLA) ? 1 : 0;

    print_str("[license] unknown feature: ");
    print_str(feature_name);
    print_str("\n");
    return 0;
}

/* Upgrade to a new license */
int license_upgrade(const char *new_serial_key) {
    if (!new_serial_key) return -1;
    print_str("[license] upgrading to: ");
    print_str(new_serial_key);
    print_str("\n");
    return license_validate(new_serial_key);
}

/* Get remaining days */
int license_get_remaining_days(void) {
    if (active_license < 0) return 0;
    int remaining = licenses[active_license].expire_date;
    print_str("[license] remaining days: ");
    print_int(remaining);
    print_str("\n");
    return remaining;
}

/* Expiration warning */
int license_expire_warning(void) {
    if (active_license < 0) return 0;
    int remaining = licenses[active_license].expire_date;
    if (remaining <= 30) {
        print_str("[license] WARNING: license expires in ");
        print_int(remaining);
        print_str(" days\n");
        return 1;
    }
    print_str("[license] license valid\n");
    return 0;
}

/* Register a feature */
static void register_feature(const char *name, int min_lic) {
    if (feature_count >= MAX_FEATURES) return;
    int idx = feature_count;
    features[idx].feature_id = idx + 1;
    my_strncpy(features[idx].name, name, FEATURE_NAME_LEN - 1);
    features[idx].min_license = min_lic;
    features[idx].enabled = 0;
    feature_count++;
}

/* Print license info */
void license_display(void) {
    print_str("=== License Information ===\n");
    if (active_license < 0) {
        print_str("  No active license (FREE)\n");
        print_str("  Max agents: 1\n");
        print_str("  Max containers: 2\n");
        return;
    }
    print_str("  Type: ");
    print_str(license_type_name(licenses[active_license].type));
    print_str("\n");
    print_str("  Serial: ");
    print_str(licenses[active_license].serial_key);
    print_str("\n");
    print_str("  Owner: ");
    print_str(licenses[active_license].owner);
    print_str("\n");
    print_str("  Max agents: ");
    print_int(licenses[active_license].max_agents);
    print_str("\n");
    print_str("  Max containers: ");
    print_int(licenses[active_license].max_containers);
    print_str("\n");
    print_str("  Support level: ");
    print_int(licenses[active_license].support_level);
    print_str("\n");
}

/* ===== CLI entry ===== */
void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);
    int help = 0, test = 0, info = 0;
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0) help = 1;
        else if (my_strcmp(arg, "-t") == 0 || my_strcmp(arg, "--test") == 0) test = 1;
        else if (my_strcmp(arg, "-i") == 0 || my_strcmp(arg, "--info") == 0) info = 1;
        while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    }
    print_str("License Manager v15.0 - Commercial Licensing\n");
    if (help) {
        print_str("Usage: license_manager [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run license test\n");
        print_str("  -i, --info    Show license info\n");
        return;
    }
    if (test) {
        print_str("=== License Manager Test ===\n");

        /* Register features */
        register_feature("max_agents", LICENSE_BASIC);
        register_feature("max_containers", LICENSE_PRO);
        register_feature("ai_routing", LICENSE_PRO);
        register_feature("edge_ai", LICENSE_PRO);
        register_feature("priority_support", LICENSE_ENTERPRISE);
        register_feature("sla", LICENSE_ENTERPRISE);

        print_str("\n--- Free License (default) ---\n");
        license_display();

        print_str("\n--- Validate PRO License ---\n");
        license_validate("PRO-XXXX-YYYY-ZZZZ");
        license_display();

        print_str("\n--- Check Features ---\n");
        print_str("  max_agents: ");
        print_int(license_check_feature("max_agents"));
        print_str("\n");
        print_str("  max_containers: ");
        print_int(license_check_feature("max_containers"));
        print_str("\n");
        print_str("  ai_routing: ");
        print_int(license_check_feature("ai_routing"));
        print_str("\n");
        print_str("  sla: ");
        print_int(license_check_feature("sla"));
        print_str("\n");

        print_str("\n--- Expiry Check ---\n");
        license_get_remaining_days();
        license_expire_warning();

        print_str("\n--- Upgrade to Enterprise ---\n");
        license_upgrade("ENT-AAAA-BBBB-CCCC");
        license_display();

        print_str("\n--- Check Enterprise Features ---\n");
        print_str("  priority_support: ");
        print_int(license_check_feature("priority_support"));
        print_str("\n");
        print_str("  sla: ");
        print_int(license_check_feature("sla"));
        print_str("\n");

        print_str("\n=== Test Complete ===\n");
        return;
    }
    if (info) { license_display(); return; }
    print_str("Use -h for help, -t for test, -i for info\n");
}
