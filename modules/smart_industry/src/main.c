/* smart_industry: Smart industry scenario adaptation (v14.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Equipment types */
#define IND_MOTOR    0
#define IND_PUMP     1
#define IND_VALVE    2
#define IND_CONVEYOR 3
#define IND_CNC      4
#define IND_ROBOT    5

/* Equipment status */
#define IND_RUNNING    0
#define IND_STOPPED    1
#define IND_MAINTENANCE 2
#define IND_ERROR      3

/* Production line states */
#define LINE_ACTIVE   0
#define LINE_IDLE     1
#define LINE_FAULT    2

/* Limits */
#define MAX_EQUIPMENT    32
#define MAX_METRICS      64
#define MAX_PRODUCTION_LINES 8
#define MAX_ALERTS       32
#define EQUIPMENT_NAME_LEN 64
#define METRIC_NAME_LEN    64
#define ALERT_MSG_LEN      128

/* Industry equipment */
typedef struct {
    int   equipment_id;
    char  name[EQUIPMENT_NAME_LEN];
    int   type;
    int   status;
    int   runtime_hours;
    int   temperature;    /* device temp in C */
    int   vibration_level;/* 0-100 */
    int   efficiency;     /* 0-100 percent */
    int   production_line;
} industry_equipment_t;

/* Industry metric */
typedef struct {
    int   metric_id;
    char  name[METRIC_NAME_LEN];
    int   equipment_id;
    int   value;
    int   threshold_low;
    int   threshold_high;
    int   unit;           /* 0=C, 1=bar, 2=m/s, 3=rpm */
    int   trend;          /* 0=STABLE, 1=UP, 2=DOWN */
} industry_metric_t;

/* Alert record */
typedef struct {
    int   alert_id;
    int   equipment_id;
    char  message[ALERT_MSG_LEN];
    int   severity;       /* 0=INFO, 1=WARNING, 2=CRITICAL */
    int   timestamp;
    int   acknowledged;
} industry_alert_t;

/* Production line */
typedef struct {
    int   line_id;
    int   status;
    int   equipment_ids[8];
    int   equipment_count;
    int   output_count;
    int   target_count;
} industry_line_t;

static industry_equipment_t  equipment[MAX_EQUIPMENT];
static industry_metric_t     metrics[MAX_METRICS];
static industry_alert_t      alerts[MAX_ALERTS];
static industry_line_t       production_lines[MAX_PRODUCTION_LINES];
static int equip_count = 0;
static int metric_count = 0;
static int alert_count = 0;
static int line_count = 0;
static int next_equip_id = 1;
static int next_metric_id = 1;
static int next_alert_id = 1;
static int next_line_id = 1;

/* Production stats */
static int total_output = 0;
static int total_defects = 0;
static int total_runtime_hours = 0;

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

/* Get equipment type name */
static const char *equip_type_name(int type) {
    if (type == IND_MOTOR) return "motor";
    if (type == IND_PUMP) return "pump";
    if (type == IND_VALVE) return "valve";
    if (type == IND_CONVEYOR) return "conveyor";
    if (type == IND_CNC) return "cnc";
    if (type == IND_ROBOT) return "robot";
    return "unknown";
}

/* Get status name */
static const char *status_name(int status) {
    if (status == IND_RUNNING) return "running";
    if (status == IND_STOPPED) return "stopped";
    if (status == IND_MAINTENANCE) return "maintenance";
    if (status == IND_ERROR) return "error";
    return "unknown";
}

/* Register equipment */
int industry_register_equipment(industry_equipment_t *eq) {
    if (!eq || equip_count >= MAX_EQUIPMENT) return -1;
    int idx = equip_count;
    equipment[idx].equipment_id = next_equip_id;
    my_strncpy(equipment[idx].name, eq->name, EQUIPMENT_NAME_LEN - 1);
    equipment[idx].type = eq->type;
    equipment[idx].status = IND_STOPPED;
    equipment[idx].runtime_hours = 0;
    equipment[idx].temperature = 25;
    equipment[idx].vibration_level = 0;
    equipment[idx].efficiency = 100;
    equipment[idx].production_line = eq->production_line;
    int id = next_equip_id++;
    equip_count++;
    print_str("[smart_industry] registered: ");
    print_str(eq->name);
    print_str(" (type=");
    print_str(equip_type_name(eq->type));
    print_str(", id=");
    print_int(id);
    print_str(")\n");
    return id;
}

/* Monitor equipment */
int industry_monitor_equipment(int equipment_id) {
    for (int i = 0; i < equip_count; i++) {
        if (equipment[i].equipment_id == equipment_id) {
            /* Simulate sensor readings */
            if (equipment[i].status == IND_RUNNING) {
                equipment[i].runtime_hours++;
                total_runtime_hours++;
                /* Simulate temperature based on runtime */
                equipment[i].temperature = 25 + (equipment[i].runtime_hours % 60);
                equipment[i].vibration_level = 10 + (equipment[i].runtime_hours % 30);
                equipment[i].efficiency = 100 - (equipment[i].runtime_hours % 15);
            }
            print_str("[smart_industry] monitor ");
            print_str(equipment[i].name);
            print_str(": temp=");
            print_int(equipment[i].temperature);
            print_str("C vib=");
            print_int(equipment[i].vibration_level);
            print_str(" eff=");
            print_int(equipment[i].efficiency);
            print_str("%\n");
            return 0;
        }
    }
    return -1;
}

/* Predictive maintenance check */
int industry_predict_maintenance(int equipment_id) {
    for (int i = 0; i < equip_count; i++) {
        if (equipment[i].equipment_id == equipment_id) {
            int needs_maintenance = 0;
            if (equipment[i].temperature > 70) needs_maintenance = 1;
            if (equipment[i].vibration_level > 50) needs_maintenance = 1;
            if (equipment[i].efficiency < 70) needs_maintenance = 1;
            if (equipment[i].runtime_hours > 1000) needs_maintenance = 1;

            print_str("[smart_industry] predict maintenance for ");
            print_str(equipment[i].name);
            print_str(": ");
            if (needs_maintenance) {
                print_str("MAINTENANCE REQUIRED\n");
                /* Create alert */
                if (alert_count < MAX_ALERTS) {
                    alerts[alert_count].alert_id = next_alert_id++;
                    alerts[alert_count].equipment_id = equipment_id;
                    my_strcpy(alerts[alert_count].message, "predictive maintenance required");
                    alerts[alert_count].severity = 1; /* WARNING */
                    alerts[alert_count].acknowledged = 0;
                    alert_count++;
                }
            } else {
                print_str("OK - no maintenance needed\n");
            }
            return needs_maintenance;
        }
    }
    return -1;
}

/* Detect abnormal conditions */
int industry_detect_abnormal(int equipment_id) {
    for (int i = 0; i < equip_count; i++) {
        if (equipment[i].equipment_id == equipment_id) {
            int abnormal = 0;
            if (equipment[i].temperature > 80) {
                print_str("[smart_industry] ABNORMAL: high temp on ");
                print_str(equipment[i].name);
                print_str(" (");
                print_int(equipment[i].temperature);
                print_str("C)\n");
                abnormal = 1;
            }
            if (equipment[i].vibration_level > 70) {
                print_str("[smart_industry] ABNORMAL: high vibration on ");
                print_str(equipment[i].name);
                print_str("\n");
                abnormal = 1;
            }
            if (equipment[i].efficiency < 50) {
                print_str("[smart_industry] ABNORMAL: low efficiency on ");
                print_str(equipment[i].name);
                print_str(" (");
                print_int(equipment[i].efficiency);
                print_str("%)\n");
                abnormal = 1;
            }
            if (!abnormal) {
                print_str("[smart_industry] no abnormalities on ");
                print_str(equipment[i].name);
                print_str("\n");
            }
            return abnormal;
        }
    }
    return -1;
}

/* Control production line */
int industry_control_production_line(int line_id, int action) {
    for (int l = 0; l < line_count; l++) {
        if (production_lines[l].line_id == line_id) {
            if (action == 0) {
                /* Start line */
                production_lines[l].status = LINE_ACTIVE;
                for (int e = 0; e < production_lines[l].equipment_count; e++) {
                    int eid = production_lines[l].equipment_ids[e];
                    for (int i = 0; i < equip_count; i++) {
                        if (equipment[i].equipment_id == eid) {
                            equipment[i].status = IND_RUNNING;
                        }
                    }
                }
                print_str("[smart_industry] started line ");
                print_int(line_id);
                print_str("\n");
            } else {
                /* Stop line */
                production_lines[l].status = LINE_IDLE;
                for (int e = 0; e < production_lines[l].equipment_count; e++) {
                    int eid = production_lines[l].equipment_ids[e];
                    for (int i = 0; i < equip_count; i++) {
                        if (equipment[i].equipment_id == eid) {
                            equipment[i].status = IND_STOPPED;
                        }
                    }
                }
                print_str("[smart_industry] stopped line ");
                print_int(line_id);
                print_str("\n");
            }
            return 0;
        }
    }
    return -1;
}

/* Optimize process */
int industry_optimize_process(const char *process_name) {
    print_str("[smart_industry] optimizing process: ");
    print_str(process_name);
    print_str("\n");
    /* Simulate optimization */
    for (int i = 0; i < equip_count; i++) {
        if (equipment[i].status == IND_RUNNING) {
            equipment[i].efficiency += 5;
            if (equipment[i].efficiency > 100) equipment[i].efficiency = 100;
        }
    }
    print_str("[smart_industry] optimization complete, efficiency improved\n");
    return 0;
}

/* Get efficiency report */
int industry_get_efficiency_report(void) {
    print_str("=== Efficiency Report ===\n");
    print_str("Total runtime: ");
    print_int(total_runtime_hours);
    print_str(" hours\n");
    print_str("Total output: ");
    print_int(total_output);
    print_str("\nTotal defects: ");
    print_int(total_defects);
    print_str("\n");
    if (total_output > 0) {
        print_str("Yield rate: ");
        print_int(((total_output - total_defects) * 100) / total_output);
        print_str("%\n");
    }
    for (int i = 0; i < equip_count; i++) {
        print_str("  ");
        print_str(equipment[i].name);
        print_str(": eff=");
        print_int(equipment[i].efficiency);
        print_str("% temp=");
        print_int(equipment[i].temperature);
        print_str("C\n");
    }
    return 0;
}

/* List all equipment */
void industry_list_equipment(void) {
    print_str("=== Industrial Equipment ===\n");
    print_str("ID\tNAME\t\tTYPE\tSTATUS\tTEMP\tVIB\tEFF\n");
    for (int i = 0; i < equip_count; i++) {
        print_str("  ");
        print_int(equipment[i].equipment_id);
        print_str("\t");
        print_str(equipment[i].name);
        print_str("\t");
        print_str(equip_type_name(equipment[i].type));
        print_str("\t");
        print_str(status_name(equipment[i].status));
        print_str("\t");
        print_int(equipment[i].temperature);
        print_str("\t");
        print_int(equipment[i].vibration_level);
        print_str("\t");
        print_int(equipment[i].efficiency);
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
    print_str("Smart Industry v14.0 - Industrial Automation\n");
    if (help) {
        print_str("Usage: smart_industry [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run industry test\n");
        print_str("  -l, --list    List equipment\n");
        return;
    }
    if (test) {
        print_str("=== Smart Industry Test ===\n");
        print_str("\n--- Register Equipment ---\n");
        industry_equipment_t e1; my_strcpy(e1.name, "main_motor"); e1.type = IND_MOTOR; e1.production_line = 1;
        industry_register_equipment(&e1);
        industry_equipment_t e2; my_strcpy(e2.name, "coolant_pump"); e2.type = IND_PUMP; e2.production_line = 1;
        industry_register_equipment(&e2);
        industry_equipment_t e3; my_strcpy(e3.name, "cnc_mill_01"); e3.type = IND_CNC; e3.production_line = 1;
        industry_register_equipment(&e3);
        industry_equipment_t e4; my_strcpy(e4.name, "assembly_robot"); e4.type = IND_ROBOT; e4.production_line = 1;
        industry_register_equipment(&e4);
        industry_equipment_t e5; my_strcpy(e5.name, "conveyor_belt"); e5.type = IND_CONVEYOR; e5.production_line = 1;
        industry_register_equipment(&e5);

        print_str("\n--- Control Production Line ---\n");
        industry_line_t line1; line1.line_id = next_line_id;
        line1.status = LINE_IDLE; line1.equipment_count = 5;
        line1.equipment_ids[0] = 1; line1.equipment_ids[1] = 2;
        line1.equipment_ids[2] = 3; line1.equipment_ids[3] = 4;
        line1.equipment_ids[4] = 5; line1.output_count = 0; line1.target_count = 1000;
        line_count++; next_line_id++;
        industry_control_production_line(1, 0); /* start */

        print_str("\n--- Monitor Equipment ---\n");
        industry_monitor_equipment(1);
        industry_monitor_equipment(3);
        industry_monitor_equipment(4);

        print_str("\n--- Detect Abnormal ---\n");
        industry_detect_abnormal(1);
        industry_detect_abnormal(3);

        print_str("\n--- Predictive Maintenance ---\n");
        industry_predict_maintenance(1);
        industry_predict_maintenance(3);

        print_str("\n--- Process Optimization ---\n");
        industry_optimize_process("assembly");

        print_str("\n--- Efficiency Report ---\n");
        industry_get_efficiency_report();

        print_str("\n--- Equipment List ---\n");
        industry_list_equipment();

        print_str("\n--- Stop Line ---\n");
        industry_control_production_line(1, 1);

        print_str("\n=== Test Complete ===\n");
        return;
    }
    if (list) { industry_list_equipment(); return; }
    print_str("Use -h for help, -t for test, -l for list\n");
}
