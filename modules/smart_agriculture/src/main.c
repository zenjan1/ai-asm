/* smart_agriculture: Smart agriculture scenario adaptation (v14.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Device types */
#define AGRI_SPRINKLER   0
#define AGRI_FERTILIZER  1
#define AGRI_SENSOR      2
#define AGRI_DRONE       3
#define AGRI_GREENHOUSE  4

/* Growth stages */
#define STAGE_SEED     0
#define STAGE_GROWING  1
#define STAGE_MATURING 2
#define STAGE_HARVEST  3

/* Limits */
#define MAX_AGRI_DEVICES  32
#define MAX_CROPS         32
#define MAX_ZONES         16
#define DEVICE_NAME_LEN   32
#define CROP_NAME_LEN     32

/* Agriculture device */
typedef struct {
    int   device_id;
    char  name[DEVICE_NAME_LEN];
    int   type;
    int   zone_id;
    int   status;
    int   coverage_area;  /* m^2 */
} agriculture_device_t;

/* Crop monitoring */
typedef struct {
    int   crop_id;
    char  name[CROP_NAME_LEN];
    int   zone_id;
    int   growth_stage;
    int   health_score;    /* 0-100 */
    int   moisture_level;  /* 0-100 */
    int   nutrient_level;  /* 0-100 */
    int   estimated_yield; /* kg per hectare */
} crop_monitor_t;

/* Zone data */
typedef struct {
    int   zone_id;
    int   soil_moisture;   /* 0-100 */
    int   soil_temp;       /* C */
    int   humidity;        /* 0-100 */
    int   light_level;     /* 0-100 */
    int   ph_level;        /* 0-14 */
    int   crop_count;
} zone_data_t;

static agriculture_device_t devices[MAX_AGRI_DEVICES];
static crop_monitor_t crops[MAX_CROPS];
static zone_data_t zones[MAX_ZONES];
static int device_count = 0;
static int crop_count = 0;
static int zone_count = 0;
static int next_device_id = 1;
static int next_crop_id = 1;
static int next_zone_id = 1;

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
    if (type == AGRI_SPRINKLER) return "sprinkler";
    if (type == AGRI_FERTILIZER) return "fertilizer";
    if (type == AGRI_SENSOR) return "sensor";
    if (type == AGRI_DRONE) return "drone";
    if (type == AGRI_GREENHOUSE) return "greenhouse";
    return "unknown";
}

/* Get growth stage name */
static const char *growth_stage_name(int stage) {
    if (stage == STAGE_SEED) return "seed";
    if (stage == STAGE_GROWING) return "growing";
    if (stage == STAGE_MATURING) return "maturing";
    if (stage == STAGE_HARVEST) return "harvest";
    return "unknown";
}

/* Register agriculture device */
int agriculture_register_device(agriculture_device_t *dev) {
    if (!dev || device_count >= MAX_AGRI_DEVICES) return -1;
    int idx = device_count;
    devices[idx].device_id = next_device_id;
    my_strncpy(devices[idx].name, dev->name, DEVICE_NAME_LEN - 1);
    devices[idx].type = dev->type;
    devices[idx].zone_id = dev->zone_id;
    devices[idx].status = 0;
    devices[idx].coverage_area = dev->coverage_area;
    int id = next_device_id++;
    device_count++;
    print_str("[smart_agri] registered: ");
    print_str(dev->name);
    print_str(" (type=");
    print_str(device_type_name(dev->type));
    print_str(", zone=");
    print_int(dev->zone_id);
    print_str(")\n");
    return id;
}

/* Monitor crop */
int agriculture_monitor_crop(int crop_id) {
    for (int i = 0; i < crop_count; i++) {
        if (crops[i].crop_id == crop_id) {
            /* Simulate sensor readings from zone */
            int z = -1;
            for (int j = 0; j < zone_count; j++) {
                if (zones[j].zone_id == crops[i].zone_id) z = j;
            }
            if (z >= 0) {
                crops[i].moisture_level = zones[z].soil_moisture;
                crops[i].health_score = 80 + (zones[z].light_level / 10);
                if (crops[i].health_score > 100) crops[i].health_score = 100;
                crops[i].nutrient_level = zones[z].ph_level * 7;
                if (crops[i].nutrient_level > 100) crops[i].nutrient_level = 100;
            }
            print_str("[smart_agri] crop ");
            print_str(crops[i].name);
            print_str(" stage=");
            print_str(growth_stage_name(crops[i].growth_stage));
            print_str(" health=");
            print_int(crops[i].health_score);
            print_str(" moisture=");
            print_int(crops[i].moisture_level);
            print_str(" nutrient=");
            print_int(crops[i].nutrient_level);
            print_str("\n");
            return 0;
        }
    }
    return -1;
}

/* Auto irrigation */
int agriculture_auto_irrigate(int zone_id) {
    int z = -1;
    for (int i = 0; i < zone_count; i++) {
        if (zones[i].zone_id == zone_id) z = i;
    }
    if (z < 0) return -1;

    print_str("[smart_agri] auto irrigate zone ");
    print_int(zone_id);
    print_str(" (soil moisture=");
    print_int(zones[z].soil_moisture);
    print_str(")\n");

    if (zones[z].soil_moisture < 30) {
        /* Activate sprinklers for this zone */
        for (int i = 0; i < device_count; i++) {
            if (devices[i].type == AGRI_SPRINKLER && devices[i].zone_id == zone_id) {
                devices[i].status = 1;
                print_str("[smart_agri] activated sprinkler: ");
                print_str(devices[i].name);
                print_str("\n");
            }
        }
        zones[z].soil_moisture += 40;
        if (zones[z].soil_moisture > 100) zones[z].soil_moisture = 100;
        print_str("[smart_agri] irrigation complete, moisture now ");
        print_int(zones[z].soil_moisture);
        print_str("\n");
    } else {
        print_str("[smart_agri] no irrigation needed\n");
    }
    return 0;
}

/* Detect disease */
int agriculture_detect_disease(int crop_id) {
    for (int i = 0; i < crop_count; i++) {
        if (crops[i].crop_id == crop_id) {
            int disease_risk = 0;
            if (crops[i].health_score < 60) {
                print_str("[smart_agri] disease risk: ");
                print_str(crops[i].name);
                print_str(" (low health=");
                print_int(crops[i].health_score);
                print_str(")\n");
                disease_risk = 1;
            }
            if (crops[i].moisture_level > 80) {
                print_str("[smart_agri] fungal risk: ");
                print_str(crops[i].name);
                print_str(" (high moisture)\n");
                disease_risk = 1;
            }
            if (!disease_risk) {
                print_str("[smart_agri] no disease detected on ");
                print_str(crops[i].name);
                print_str("\n");
            }
            return disease_risk;
        }
    }
    return -1;
}

/* Optimize fertilizer */
int agriculture_optimize_fertilizer(int crop_id) {
    for (int i = 0; i < crop_count; i++) {
        if (crops[i].crop_id == crop_id) {
            print_str("[smart_agri] optimize fertilizer for ");
            print_str(crops[i].name);
            print_str(" (nutrient=");
            print_int(crops[i].nutrient_level);
            print_str(")\n");
            if (crops[i].nutrient_level < 50) {
                /* Activate fertilizer device */
                for (int j = 0; j < device_count; j++) {
                    if (devices[j].type == AGRI_FERTILIZER && devices[j].zone_id == crops[i].zone_id) {
                        devices[j].status = 1;
                        print_str("[smart_agri] activated fertilizer: ");
                        print_str(devices[j].name);
                        print_str("\n");
                    }
                }
                crops[i].nutrient_level += 20;
                if (crops[i].nutrient_level > 100) crops[i].nutrient_level = 100;
            } else {
                print_str("[smart_agri] nutrient level sufficient\n");
            }
            return 0;
        }
    }
    return -1;
}

/* Predict yield */
int agriculture_predict_yield(int crop_id) {
    for (int i = 0; i < crop_count; i++) {
        if (crops[i].crop_id == crop_id) {
            /* Simple yield prediction based on health and stage */
            int base_yield = 5000; /* kg/hectare */
            int health_factor = crops[i].health_score;
            int stage_factor = (crops[i].growth_stage + 1) * 25;
            int predicted = (base_yield * health_factor * stage_factor) / 10000;
            crops[i].estimated_yield = predicted;
            print_str("[smart_agri] yield prediction for ");
            print_str(crops[i].name);
            print_str(": ");
            print_int(predicted);
            print_str(" kg/hectare\n");
            return predicted;
        }
    }
    return -1;
}

/* Control greenhouse */
int agriculture_control_greenhouse(int greenhouse_id, int temp, int humidity) {
    print_str("[smart_agri] greenhouse ");
    print_int(greenhouse_id);
    print_str(" control: target temp=");
    print_int(temp);
    print_str("C humidity=");
    print_int(humidity);
    print_str("%\n");

    /* Find and adjust greenhouse devices */
    for (int i = 0; i < device_count; i++) {
        if (devices[i].type == AGRI_GREENHOUSE && devices[i].device_id == greenhouse_id) {
            devices[i].status = 1;
            /* Update zone data */
            for (int j = 0; j < zone_count; j++) {
                if (zones[j].zone_id == devices[i].zone_id) {
                    zones[j].soil_temp = temp;
                    zones[j].humidity = humidity;
                }
            }
            print_str("[smart_agri] greenhouse adjusted\n");
            return 0;
        }
    }
    return -1;
}

/* List all devices */
void agriculture_list_devices(void) {
    print_str("=== Agriculture Devices ===\n");
    print_str("ID\tNAME\t\tTYPE\tZONE\tSTATUS\n");
    for (int i = 0; i < device_count; i++) {
        print_str("  ");
        print_int(devices[i].device_id);
        print_str("\t");
        print_str(devices[i].name);
        print_str("\t");
        print_str(device_type_name(devices[i].type));
        print_str("\t");
        print_int(devices[i].zone_id);
        print_str("\t");
        print_int(devices[i].status);
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
    print_str("Smart Agriculture v14.0 - Precision Farming\n");
    if (help) {
        print_str("Usage: smart_agriculture [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run agriculture test\n");
        print_str("  -l, --list    List devices\n");
        return;
    }
    if (test) {
        print_str("=== Smart Agriculture Test ===\n");
        print_str("\n--- Register Devices ---\n");
        agriculture_device_t d1; my_strcpy(d1.name, "zone1_sprinkler"); d1.type = AGRI_SPRINKLER; d1.zone_id = 1; d1.coverage_area = 500;
        agriculture_register_device(&d1);
        agriculture_device_t d2; my_strcpy(d2.name, "zone1_sensor"); d2.type = AGRI_SENSOR; d2.zone_id = 1; d2.coverage_area = 100;
        agriculture_register_device(&d2);
        agriculture_device_t d3; my_strcpy(d3.name, "zone2_fertilizer"); d3.type = AGRI_FERTILIZER; d3.zone_id = 2; d3.coverage_area = 300;
        agriculture_register_device(&d3);
        agriculture_device_t d4; my_strcpy(d4.name, "greenhouse_main"); d4.type = AGRI_GREENHOUSE; d4.zone_id = 3; d4.coverage_area = 1000;
        agriculture_register_device(&d4);
        agriculture_device_t d5; my_strcpy(d5.name, "survey_drone"); d5.type = AGRI_DRONE; d5.zone_id = 0; d5.coverage_area = 5000;
        agriculture_register_device(&d5);

        print_str("\n--- Setup Zones ---\n");
        zones[0].zone_id = 1; zones[0].soil_moisture = 20; zones[0].soil_temp = 25; zones[0].humidity = 60; zones[0].light_level = 80; zones[0].ph_level = 6; zones[0].crop_count = 1;
        zones[1].zone_id = 2; zones[1].soil_moisture = 45; zones[1].soil_temp = 22; zones[1].humidity = 55; zones[1].light_level = 70; zones[1].ph_level = 7; zones[1].crop_count = 1;
        zones[2].zone_id = 3; zones[2].soil_moisture = 50; zones[2].soil_temp = 28; zones[2].humidity = 75; zones[2].light_level = 90; zones[2].ph_level = 6; zones[2].crop_count = 2;
        zone_count = 3;

        print_str("\n--- Register Crops ---\n");
        crops[0].crop_id = next_crop_id++; crops[0].zone_id = 1; my_strcpy(crops[0].name, "tomatoes");
        crops[0].growth_stage = STAGE_GROWING; crop_count++;
        crops[1].crop_id = next_crop_id++; crops[1].zone_id = 2; my_strcpy(crops[1].name, "wheat");
        crops[1].growth_stage = STAGE_MATURING; crop_count++;
        crops[2].crop_id = next_crop_id++; crops[2].zone_id = 3; my_strcpy(crops[2].name, "strawberries");
        crops[2].growth_stage = STAGE_SEED; crop_count++;

        print_str("\n--- Auto Irrigation ---\n");
        agriculture_auto_irrigate(1); /* low moisture, should irrigate */
        agriculture_auto_irrigate(2); /* ok moisture */

        print_str("\n--- Crop Monitoring ---\n");
        agriculture_monitor_crop(1);
        agriculture_monitor_crop(2);
        agriculture_monitor_crop(3);

        print_str("\n--- Disease Detection ---\n");
        agriculture_detect_disease(1);
        agriculture_detect_disease(2);

        print_str("\n--- Fertilizer Optimization ---\n");
        agriculture_optimize_fertilizer(1);
        agriculture_optimize_fertilizer(2);

        print_str("\n--- Yield Prediction ---\n");
        agriculture_predict_yield(1);
        agriculture_predict_yield(2);

        print_str("\n--- Greenhouse Control ---\n");
        agriculture_control_greenhouse(4, 26, 70);

        print_str("\n--- Device List ---\n");
        agriculture_list_devices();

        print_str("\n=== Test Complete ===\n");
        return;
    }
    if (list) { agriculture_list_devices(); return; }
    print_str("Use -h for help, -t for test, -l for list\n");
}
