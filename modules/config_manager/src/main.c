/* config_manager: System configuration management (v14.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Config types */
#define CFG_STRING  0
#define CFG_INT     1
#define CFG_BOOL    2
#define CFG_JSON    3

/* Limits */
#define MAX_CONFIG_ITEMS   128
#define MAX_CONFIG_GROUPS  16
#define KEY_LEN            64
#define VALUE_LEN          256
#define TYPE_LEN           16
#define DESC_LEN           128
#define GROUP_NAME_LEN     32

/* Config item */
typedef struct {
    char  key[KEY_LEN];
    char  value[VALUE_LEN];
    char  type[TYPE_LEN];
    char  description[DESC_LEN];
    int   editable;
    char  default_value[VALUE_LEN];
} config_item_t;

/* Config group */
typedef struct {
    char  group_name[GROUP_NAME_LEN];
    config_item_t items[MAX_CONFIG_ITEMS];
    int   item_count;
    int   priority;
} config_group_t;

static config_group_t groups[MAX_CONFIG_GROUPS];
static int group_count = 0;
static int total_items = 0;

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

/* Get type name */
static const char *cfg_type_name(int type) {
    if (type == CFG_STRING) return "string";
    if (type == CFG_INT) return "int";
    if (type == CFG_BOOL) return "bool";
    if (type == CFG_JSON) return "json";
    return "unknown";
}

/* Add a config group */
static int add_group(const char *name, int priority) {
    if (group_count >= MAX_CONFIG_GROUPS) return -1;
    int idx = group_count;
    my_strncpy(groups[idx].group_name, name, GROUP_NAME_LEN - 1);
    groups[idx].item_count = 0;
    groups[idx].priority = priority;
    group_count++;
    return idx;
}

/* Find a config group by name */
static int find_group(const char *name) {
    for (int i = 0; i < group_count; i++) {
        if (my_strcmp(groups[i].group_name, name) == 0) return i;
    }
    return -1;
}

/* Load configuration (simulated defaults) */
int config_load(const char *config_file) {
    print_str("[config] loading: ");
    print_str(config_file);
    print_str("\n");

    /* System group */
    int sys = add_group("system", 1);
    if (sys >= 0) {
        config_item_t *it = &groups[sys].items[groups[sys].item_count];
        my_strcpy(it->key, "system.hostname"); my_strcpy(it->value, "aiasm-edge");
        my_strcpy(it->type, "string"); my_strcpy(it->description, "Device hostname");
        it->editable = 1; my_strcpy(it->default_value, "aiasm-edge");
        groups[sys].item_count++; total_items++;

        it = &groups[sys].items[groups[sys].item_count];
        my_strcpy(it->key, "system.max_processes"); my_strcpy(it->value, "64");
        my_strcpy(it->type, "int"); my_strcpy(it->description, "Max concurrent processes");
        it->editable = 1; my_strcpy(it->default_value, "64");
        groups[sys].item_count++; total_items++;

        it = &groups[sys].items[groups[sys].item_count];
        my_strcpy(it->key, "system.debug_mode"); my_strcpy(it->value, "false");
        my_strcpy(it->type, "bool"); my_strcpy(it->description, "Enable debug logging");
        it->editable = 1; my_strcpy(it->default_value, "false");
        groups[sys].item_count++; total_items++;
    }

    /* Network group */
    int net = add_group("network", 2);
    if (net >= 0) {
        config_item_t *it = &groups[net].items[groups[net].item_count];
        my_strcpy(it->key, "network.ip"); my_strcpy(it->value, "192.168.1.100");
        my_strcpy(it->type, "string"); my_strcpy(it->description, "IP address");
        it->editable = 1; my_strcpy(it->default_value, "192.168.1.100");
        groups[net].item_count++; total_items++;

        it = &groups[net].items[groups[net].item_count];
        my_strcpy(it->key, "network.mqtt_broker"); my_strcpy(it->value, "tcp://localhost:1883");
        my_strcpy(it->type, "string"); my_strcpy(it->description, "MQTT broker URL");
        it->editable = 1; my_strcpy(it->default_value, "tcp://localhost:1883");
        groups[net].item_count++; total_items++;

        it = &groups[net].items[groups[net].item_count];
        my_strcpy(it->key, "network.http_timeout"); my_strcpy(it->value, "5000");
        my_strcpy(it->type, "int"); my_strcpy(it->description, "HTTP timeout (ms)");
        it->editable = 1; my_strcpy(it->default_value, "5000");
        groups[net].item_count++; total_items++;
    }

    /* IoT group */
    int iot = add_group("iot", 3);
    if (iot >= 0) {
        config_item_t *it = &groups[iot].items[groups[iot].item_count];
        my_strcpy(it->key, "iot.sampling_interval"); my_strcpy(it->value, "1000");
        my_strcpy(it->type, "int"); my_strcpy(it->description, "Sensor sampling interval (ms)");
        it->editable = 1; my_strcpy(it->default_value, "1000");
        groups[iot].item_count++; total_items++;

        it = &groups[iot].items[groups[iot].item_count];
        my_strcpy(it->key, "iot.auto_calibrate"); my_strcpy(it->value, "true");
        my_strcpy(it->type, "bool"); my_strcpy(it->description, "Auto-calibrate sensors");
        it->editable = 1; my_strcpy(it->default_value, "true");
        groups[iot].item_count++; total_items++;
    }

    print_str("[config] loaded ");
    print_int(total_items);
    print_str(" items in ");
    print_int(group_count);
    print_str(" groups\n");
    return 0;
}

/* Get a config value by key */
int config_get(const char *key, char *value) {
    if (!key || !value) return -1;
    for (int g = 0; g < group_count; g++) {
        for (int i = 0; i < groups[g].item_count; i++) {
            if (my_strcmp(groups[g].items[i].key, key) == 0) {
                my_strcpy(value, groups[g].items[i].value);
                return 0;
            }
        }
    }
    return -1;
}

/* Set a config value */
int config_set(const char *key, const char *value) {
    if (!key || !value) return -1;
    for (int g = 0; g < group_count; g++) {
        for (int i = 0; i < groups[g].item_count; i++) {
            if (my_strcmp(groups[g].items[i].key, key) == 0) {
                if (!groups[g].items[i].editable) {
                    print_str("[config] key ");
                    print_str(key);
                    print_str(" is not editable\n");
                    return -1;
                }
                my_strcpy(groups[g].items[i].value, value);
                print_str("[config] set ");
                print_str(key);
                print_str(" = ");
                print_str(value);
                print_str("\n");
                return 0;
            }
        }
    }
    print_str("[config] key not found: ");
    print_str(key);
    print_str("\n");
    return -1;
}

/* Validate all config */
int config_validate(void) {
    print_str("=== Config Validation ===\n");
    int errors = 0;
    for (int g = 0; g < group_count; g++) {
        for (int i = 0; i < groups[g].item_count; i++) {
            config_item_t *it = &groups[g].items[i];
            if (my_strcmp(it->type, "int") == 0) {
                /* Check value is numeric */
                int valid = it->value[0] != '\0';
                for (int c = 0; it->value[c]; c++) {
                    if (it->value[c] < '0' || it->value[c] > '9') {
                        if (c == 0 && it->value[c] == '-') continue;
                        valid = 0; break;
                    }
                }
                if (!valid) {
                    print_str("  ERROR: ");
                    print_str(it->key);
                    print_str(" invalid int: ");
                    print_str(it->value);
                    print_str("\n");
                    errors++;
                }
            } else if (my_strcmp(it->type, "bool") == 0) {
                if (my_strcmp(it->value, "true") != 0 && my_strcmp(it->value, "false") != 0) {
                    print_str("  ERROR: ");
                    print_str(it->key);
                    print_str(" invalid bool: ");
                    print_str(it->value);
                    print_str("\n");
                    errors++;
                }
            }
        }
    }
    if (errors == 0) {
        print_str("  All ");
        print_int(total_items);
        print_str(" items valid\n");
    } else {
        print_str("  ");
        print_int(errors);
        print_str(" errors found\n");
    }
    return errors;
}

/* Save configuration (simulated) */
int config_save(const char *config_file) {
    print_str("[config] saving to: ");
    print_str(config_file);
    print_str("\n");
    for (int g = 0; g < group_count; g++) {
        print_str("  [");
        print_str(groups[g].group_name);
        print_str("]\n");
        for (int i = 0; i < groups[g].item_count; i++) {
            print_str("    ");
            print_str(groups[g].items[i].key);
            print_str(" = ");
            print_str(groups[g].items[i].value);
            print_str("\n");
        }
    }
    print_str("[config] saved\n");
    return 0;
}

/* List all config */
void config_list_all(void) {
    print_str("=== Configuration ===\n");
    for (int g = 0; g < group_count; g++) {
        print_str("--- ");
        print_str(groups[g].group_name);
        print_str(" (pri=");
        print_int(groups[g].priority);
        print_str(") ---\n");
        for (int i = 0; i < groups[g].item_count; i++) {
            print_str("  ");
            print_str(groups[g].items[i].key);
            print_str(" [");
            print_str(groups[g].items[i].type);
            print_str("] = ");
            print_str(groups[g].items[i].value);
            print_str("\n    ");
            print_str(groups[g].items[i].description);
            print_str("\n");
        }
    }
}

/* ===== CLI entry ===== */
void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);
    int help = 0, test = 0, list = 0;
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0) help = 1;
        else if (my_strcmp(arg, "-t") == 0 || my_strcmp(arg, "--test") == 0) test = 1;
        else if (my_strcmp(arg, "-l") == 0 || my_strcmp(arg, "--list") == 0) list = 1;
        while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    }
    print_str("Config Manager v14.0 - System Configuration\n");
    if (help) {
        print_str("Usage: config_manager [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run config test\n");
        print_str("  -l, --list    List configuration\n");
        return;
    }
    if (test) {
        print_str("=== Config Manager Test ===\n");
        print_str("\n--- Load Config ---\n");
        config_load("/etc/aiasm/config.yaml");

        print_str("\n--- List All ---\n");
        config_list_all();

        print_str("\n--- Get Values ---\n");
        char val[VALUE_LEN];
        if (config_get("system.hostname", val) == 0) {
            print_str("  system.hostname = ");
            print_str(val);
            print_str("\n");
        }
        if (config_get("network.mqtt_broker", val) == 0) {
            print_str("  network.mqtt_broker = ");
            print_str(val);
            print_str("\n");
        }

        print_str("\n--- Set Values ---\n");
        config_set("system.hostname", "edge-node-01");
        config_set("network.http_timeout", "10000");
        config_set("iot.sampling_interval", "500");

        print_str("\n--- Validate ---\n");
        config_validate();

        print_str("\n--- Save ---\n");
        config_save("/etc/aiasm/config.yaml");

        print_str("\n=== Test Complete ===\n");
        return;
    }
    if (list) { config_list_all(); return; }
    print_str("Use -h for help, -t for test, -l for list\n");
}
