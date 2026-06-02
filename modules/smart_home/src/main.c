/* smart_home: Smart home scenario adaptation (v14.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Device types */
#define HOME_LIGHT     0
#define HOME_AC        1
#define HOME_HEATER    2
#define HOME_CAMERA    3
#define HOME_LOCK      4
#define HOME_SENSOR    5

/* Device states */
#define HOME_ON        1
#define HOME_OFF       0
#define HOME_STANDBY   2

/* Limits */
#define MAX_HOME_DEVICES  32
#define MAX_HOME_RULES    32
#define MAX_SCHEDULES     64
#define DEVICE_NAME_LEN   32
#define SCHEDULE_LEN      128
#define TRIGGER_LEN       128
#define ACTION_LEN        128

/* Home device */
typedef struct {
    int   device_id;
    char  name[DEVICE_NAME_LEN];
    int   type;
    int   room_id;
    int   status;
    int   power_consumption;
    char  schedule[SCHEDULE_LEN];
    int   value;  /* dim level, temp, etc */
} home_device_t;

/* Home rule */
typedef struct {
    int   rule_id;
    char  trigger[TRIGGER_LEN];
    char  action[ACTION_LEN];
    int   priority;
    int   enabled;
    int   trigger_count;
} home_rule_t;

/* Schedule entry */
typedef struct {
    int   schedule_id;
    int   device_id;
    char  cron_expr[64];  /* simplified: "HH:MM" */
    int   action;
    int   value;
    int   active;
} home_schedule_t;

static home_device_t    devices[MAX_HOME_DEVICES];
static home_rule_t      rules[MAX_HOME_RULES];
static home_schedule_t  schedules[MAX_SCHEDULES];
static int device_count = 0;
static int rule_count = 0;
static int schedule_count = 0;
static int next_device_id = 1;
static int next_rule_id = 1;
static int next_schedule_id = 1;

/* Energy tracking */
static int total_energy_wh = 0;
static int device_energy[MAX_HOME_DEVICES];

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

/* Get device type name */
static const char *device_type_name(int type) {
    if (type == HOME_LIGHT) return "light";
    if (type == HOME_AC) return "ac";
    if (type == HOME_HEATER) return "heater";
    if (type == HOME_CAMERA) return "camera";
    if (type == HOME_LOCK) return "lock";
    if (type == HOME_SENSOR) return "sensor";
    return "unknown";
}

/* Register a home device */
int home_register_device(home_device_t *device) {
    if (!device || device_count >= MAX_HOME_DEVICES) {
        print_str("[smart_home] cannot register device\n");
        return -1;
    }
    int idx = device_count;
    devices[idx].device_id = next_device_id;
    my_strncpy(devices[idx].name, device->name, DEVICE_NAME_LEN - 1);
    devices[idx].type = device->type;
    devices[idx].room_id = device->room_id;
    devices[idx].status = HOME_OFF;
    devices[idx].power_consumption = device->power_consumption;
    devices[idx].schedule[0] = '\0';
    devices[idx].value = 0;
    device_energy[idx] = 0;
    device_count++;
    int id = next_device_id++;
    print_str("[smart_home] registered: ");
    print_str(device->name);
    print_str(" (type=");
    print_str(device_type_name(device->type));
    print_str(", id=");
    print_int(id);
    print_str(", room=");
    print_int(device->room_id);
    print_str(")\n");
    return id;
}

/* Control a home device */
int home_control_device(int device_id, int status, int value) {
    for (int i = 0; i < device_count; i++) {
        if (devices[i].device_id == device_id) {
            devices[i].status = status;
            devices[i].value = value;
            /* Track energy */
            if (status == HOME_ON) {
                device_energy[i] += devices[i].power_consumption;
                total_energy_wh += devices[i].power_consumption;
            }
            print_str("[smart_home] controlled ");
            print_str(devices[i].name);
            print_str(" -> status=");
            print_int(status);
            print_str(" value=");
            print_int(value);
            print_str("\n");
            return 0;
        }
    }
    print_str("[smart_home] device not found: ");
    print_int(device_id);
    print_str("\n");
    return -1;
}

/* Define a home automation rule */
int home_define_rule(home_rule_t *rule) {
    if (!rule || rule_count >= MAX_HOME_RULES) return -1;
    int idx = rule_count;
    rules[idx].rule_id = next_rule_id;
    my_strncpy(rules[idx].trigger, rule->trigger, TRIGGER_LEN - 1);
    my_strncpy(rules[idx].action, rule->action, ACTION_LEN - 1);
    rules[idx].priority = rule->priority;
    rules[idx].enabled = 1;
    rules[idx].trigger_count = 0;
    rule_count++;
    int id = next_rule_id++;
    print_str("[smart_home] rule: ");
    print_str(rule->trigger);
    print_str(" -> ");
    print_str(rule->action);
    print_str(" (pri=");
    print_int(rule->priority);
    print_str(")\n");
    return id;
}

/* Apply a scene (predefined set of actions) */
int home_apply_scene(const char *scene_name) {
    print_str("[smart_home] applying scene: ");
    print_str(scene_name);
    print_str("\n");

    if (my_strcmp(scene_name, "morning") == 0) {
        /* Turn on lights, open curtains, set AC */
        for (int i = 0; i < device_count; i++) {
            if (devices[i].type == HOME_LIGHT) home_control_device(devices[i].device_id, HOME_ON, 80);
            if (devices[i].type == HOME_AC) home_control_device(devices[i].device_id, HOME_ON, 24);
        }
    } else if (my_strcmp(scene_name, "night") == 0) {
        /* Turn off lights, lock doors, set AC */
        for (int i = 0; i < device_count; i++) {
            if (devices[i].type == HOME_LIGHT) home_control_device(devices[i].device_id, HOME_OFF, 0);
            if (devices[i].type == HOME_LOCK) home_control_device(devices[i].device_id, HOME_ON, 1);
            if (devices[i].type == HOME_AC) home_control_device(devices[i].device_id, HOME_ON, 26);
        }
    } else if (my_strcmp(scene_name, "away") == 0) {
        /* Turn everything off, lock doors, enable cameras */
        for (int i = 0; i < device_count; i++) {
            if (devices[i].type == HOME_LIGHT || devices[i].type == HOME_AC || devices[i].type == HOME_HEATER)
                home_control_device(devices[i].device_id, HOME_OFF, 0);
            if (devices[i].type == HOME_LOCK) home_control_device(devices[i].device_id, HOME_ON, 1);
            if (devices[i].type == HOME_CAMERA) home_control_device(devices[i].device_id, HOME_ON, 1);
        }
    } else if (my_strcmp(scene_name, "movie") == 0) {
        /* Dim lights, close curtains */
        for (int i = 0; i < device_count; i++) {
            if (devices[i].type == HOME_LIGHT) home_control_device(devices[i].device_id, HOME_ON, 20);
        }
    } else {
        print_str("[smart_home] unknown scene: ");
        print_str(scene_name);
        print_str("\n");
        return -1;
    }

    return 0;
}

/* Get energy report */
int home_get_energy_report(void) {
    print_str("=== Energy Report ===\n");
    print_str("Total consumption: ");
    print_int(total_energy_wh);
    print_str(" Wh\n");
    for (int i = 0; i < device_count; i++) {
        if (device_energy[i] > 0) {
            print_str("  ");
            print_str(devices[i].name);
            print_str(": ");
            print_int(device_energy[i]);
            print_str(" Wh\n");
        }
    }
    return total_energy_wh;
}

/* Schedule a task for a device */
int home_schedule_task(int device_id, const char *cron, int action) {
    if (schedule_count >= MAX_SCHEDULES) return -1;
    int idx = schedule_count;
    schedules[idx].schedule_id = next_schedule_id;
    schedules[idx].device_id = device_id;
    my_strncpy(schedules[idx].cron_expr, cron, 63);
    schedules[idx].action = action;
    schedules[idx].value = 0;
    schedules[idx].active = 1;
    schedule_count++;
    int id = next_schedule_id++;
    print_str("[smart_home] scheduled device ");
    print_int(device_id);
    print_str(" at ");
    print_str(cron);
    print_str(" action=");
    print_int(action);
    print_str(" (id=");
    print_int(id);
    print_str(")\n");
    return id;
}

/* Print device list */
void home_list_devices(void) {
    print_str("=== Smart Home Devices ===\n");
    print_str("ID\tNAME\t\tTYPE\tROOM\tSTATUS\tPOWER(W)\n");
    for (int i = 0; i < device_count; i++) {
        print_str("  ");
        print_int(devices[i].device_id);
        print_str("\t");
        print_str(devices[i].name);
        print_str("\t");
        print_str(device_type_name(devices[i].type));
        print_str("\t");
        print_int(devices[i].room_id);
        print_str("\t");
        print_int(devices[i].status);
        print_str("\t");
        print_int(devices[i].power_consumption);
        print_str("\n");
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
    print_str("Smart Home v14.0 - Home Automation Scenario\n");
    if (help) {
        print_str("Usage: smart_home [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run home test\n");
        print_str("  -l, --list    List devices\n");
        return;
    }
    if (test) {
        print_str("=== Smart Home Test ===\n");
        print_str("\n--- Register Devices ---\n");
        home_device_t d1; my_strcpy(d1.name, "living_room_light"); d1.type = HOME_LIGHT; d1.room_id = 1; d1.power_consumption = 10;
        home_register_device(&d1);
        home_device_t d2; my_strcpy(d2.name, "bedroom_ac"); d2.type = HOME_AC; d2.room_id = 2; d2.power_consumption = 1500;
        home_register_device(&d2);
        home_device_t d3; my_strcpy(d3.name, "front_door_lock"); d3.type = HOME_LOCK; d3.room_id = 0; d3.power_consumption = 5;
        home_register_device(&d3);
        home_device_t d4; my_strcpy(d4.name, "kitchen_heater"); d4.type = HOME_HEATER; d4.room_id = 3; d4.power_consumption = 2000;
        home_register_device(&d4);
        home_device_t d5; my_strcpy(d5.name, "hallway_camera"); d5.type = HOME_CAMERA; d5.room_id = 0; d5.power_consumption = 8;
        home_register_device(&d5);

        print_str("\n--- Define Rules ---\n");
        home_rule_t r1; my_strcpy(r1.trigger, "motion_detected"); my_strcpy(r1.action, "turn_on_hallway_light"); r1.priority = 5;
        home_define_rule(&r1);
        home_rule_t r2; my_strcpy(r2.trigger, "temp>28"); my_strcpy(r2.action, "set_ac_temp=22"); r2.priority = 8;
        home_define_rule(&r2);
        home_rule_t r3; my_strcpy(r3.trigger, "time=22:00"); my_strcpy(r3.action, "night_scene"); r3.priority = 3;
        home_define_rule(&r3);

        print_str("\n--- Scenes ---\n");
        home_apply_scene("morning");
        home_apply_scene("night");
        home_apply_scene("away");
        home_apply_scene("movie");

        print_str("\n--- Schedule ---\n");
        home_schedule_task(1, "06:00", HOME_ON);
        home_schedule_task(2, "07:00", HOME_ON);

        print_str("\n--- Energy Report ---\n");
        home_get_energy_report();

        print_str("\n--- Device List ---\n");
        home_list_devices();

        print_str("\n=== Test Complete ===\n");
        return;
    }
    if (list) { home_list_devices(); return; }
    print_str("Use -h for help, -t for test, -l for list\n");
}
