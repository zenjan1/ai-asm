/* sensor_hub: Sensor data acquisition and processing (v13.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Edge framework functions */
extern int edge_init(void);
extern int edge_register_device(int type, const char *name, int gpio_pin);
extern int edge_discover_devices(unsigned int buf, int max_count);
extern int edge_config_device(int device_id, int sampling_rate, int resolution);
extern int edge_read_sensor(int device_id);
extern int edge_get_status(int device_id, unsigned int buf);

/* Sensor types */
#define SENSOR_TEMP      0
#define SENSOR_HUMIDITY  1
#define SENSOR_PRESSURE  2
#define SENSOR_LIGHT     3
#define SENSOR_ACCEL     4
#define SENSOR_GYRO      5
#define SENSOR_GPS       6
#define SENSOR_CAMERA    7

/* Device types */
#define EDGE_SENSOR      0
#define EDGE_ACTUATOR    1
#define EDGE_CONTROLLER  2
#define EDGE_GATEWAY     3

/* Limits */
#define MAX_SENSORS 32
#define SENSOR_NAME_LEN 32
#define READINGS_HISTORY 64

/* Sensor info */
typedef struct {
    int   sensor_id;
    int   type;
    char  name[SENSOR_NAME_LEN];
    int   gpio_pin;
    int   sampling_rate;
    int   resolution;
    int   last_value;
    int   timestamp;
    int   min_val;
    int   max_val;
    int   reading_count;
} sensor_t;

/* Reading history for anomaly detection */
typedef struct {
    int   value;
    int   timestamp;
} reading_t;

static sensor_t   sensors[MAX_SENSORS];
static reading_t  history[MAX_SENSORS][READINGS_HISTORY];
static int sensor_count = 0;
static int hub_initialized = 0;

/* String utilities */
static int my_strlen(const char *s) {
    int len = 0;
    while (s[len]) len++;
    return len;
}

static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) {
        if (*a != *b) return *a - *b;
        a++; b++;
    }
    return *a - *b;
}

static void my_strcpy(char *dst, const char *src) {
    while (*src) *dst++ = *src++;
    *dst = '\0';
}

static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0;
    while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; }
    dst[i] = '\0';
}

static int my_strncmp(const char *a, const char *b, int n) {
    for (int i = 0; i < n; i++) {
        if (!a[i] || !b[i]) return (a[i] ? 1 : 0) - (b[i] ? 1 : 0);
        if (a[i] != b[i]) return a[i] - b[i];
    }
    return 0;
}

/* Print utilities */
static void print_str(const char *str) { host_print(str); }
static void print_int(int val) {
    char buf[32];
    int pos = 0;
    if (val < 0) { buf[pos++] = '-'; val = -val; }
    if (val == 0) buf[pos++] = '0';
    else {
        int d = 0, t = val;
        while (t > 0) { d++; t /= 10; }
        pos += d;
        buf[pos] = '\0'; pos--;
        while (val > 0) { buf[pos--] = '0' + (val % 10); val /= 10; }
    }
    host_print(buf);
}

/* Get sensor type name */
static const char *sensor_type_name(int type) {
    if (type == SENSOR_TEMP) return "temperature";
    if (type == SENSOR_HUMIDITY) return "humidity";
    if (type == SENSOR_PRESSURE) return "pressure";
    if (type == SENSOR_LIGHT) return "light";
    if (type == SENSOR_ACCEL) return "accelerometer";
    if (type == SENSOR_GYRO) return "gyroscope";
    if (type == SENSOR_GPS) return "gps";
    if (type == SENSOR_CAMERA) return "camera";
    return "unknown";
}

/* Register a sensor
   type: sensor type, name: sensor name, gpio_pin: GPIO pin
   Returns: sensor_id or -1 */
int sensor_register_sensor(int type, const char *name, int gpio_pin) {
    if (!hub_initialized) {
        edge_init();
        hub_initialized = 1;
    }
    if (sensor_count >= MAX_SENSORS) {
        print_str("[sensor_hub] max sensors reached\n");
        return -1;
    }

    int id = edge_register_device(EDGE_SENSOR, name, gpio_pin);
    if (id < 0) return -1;

    int idx = sensor_count;
    sensors[idx].sensor_id = id;
    sensors[idx].type = type;
    my_strncpy(sensors[idx].name, name, SENSOR_NAME_LEN - 1);
    sensors[idx].gpio_pin = gpio_pin;
    sensors[idx].sampling_rate = 1; /* 1 Hz default */
    sensors[idx].resolution = 10;   /* 10-bit default */
    sensors[idx].last_value = 0;
    sensors[idx].timestamp = 0;
    sensors[idx].min_val = 2147483647;
    sensors[idx].max_val = -2147483647;
    sensors[idx].reading_count = 0;

    /* Clear history */
    for (int i = 0; i < READINGS_HISTORY; i++) {
        history[idx][i].value = 0;
        history[idx][i].timestamp = 0;
    }

    sensor_count++;

    print_str("[sensor_hub] registered: ");
    print_str(name);
    print_str(" (type=");
    print_str(sensor_type_name(type));
    print_str(", id=");
    print_int(id);
    print_str(")\n");

    return id;
}

/* Read a sensor value
   sensor_id: sensor to read
   Returns: sensor value or -1 */
int sensor_read(int sensor_id) {
    int idx = -1;
    for (int i = 0; i < sensor_count; i++) {
        if (sensors[i].sensor_id == sensor_id) {
            idx = i;
            break;
        }
    }
    if (idx < 0) return -1;

    int value = edge_read_sensor(sensor_id);
    if (value < 0) return -1;

    /* Update stats */
    sensors[idx].last_value = value;
    sensors[idx].reading_count++;
    if (value < sensors[idx].min_val) sensors[idx].min_val = value;
    if (value > sensors[idx].max_val) sensors[idx].max_val = value;

    /* Store in history (circular buffer) */
    int hidx = sensors[idx].reading_count % READINGS_HISTORY;
    history[idx][hidx].value = value;
    history[idx][hidx].timestamp = sensors[idx].reading_count;

    return value;
}

/* Batch read sensor values
   sensor_id: sensor, values: output array, count: number of readings
   Returns: number of readings returned */
int sensor_read_batch(int sensor_id, int *values, int count) {
    int idx = -1;
    for (int i = 0; i < sensor_count; i++) {
        if (sensors[i].sensor_id == sensor_id) {
            idx = i;
            break;
        }
    }
    if (idx < 0) return 0;

    int available = sensors[idx].reading_count;
    if (available > count) available = count;
    if (available > READINGS_HISTORY) available = READINGS_HISTORY;

    for (int i = 0; i < available; i++) {
        int hidx = (sensors[idx].reading_count - available + i) % READINGS_HISTORY;
        values[i] = history[idx][hidx].value;
    }

    return available;
}

/* Set sensor sampling rate
   sensor_id: sensor, rate_hz: samples per second
   Returns: 0=success, -1=failure */
int sensor_set_rate(int sensor_id, int rate_hz) {
    for (int i = 0; i < sensor_count; i++) {
        if (sensors[i].sensor_id == sensor_id) {
            sensors[i].sampling_rate = rate_hz;
            edge_config_device(sensor_id, rate_hz, sensors[i].resolution);
            return 0;
        }
    }
    return -1;
}

/* Get sensor metadata
   sensor_id: sensor, info: output buffer
   Returns: 0=success, -1=failure */
int sensor_get_metadata(int sensor_id, int *info) {
    for (int i = 0; i < sensor_count; i++) {
        if (sensors[i].sensor_id == sensor_id) {
            info[0] = sensors[i].type;
            info[1] = sensors[i].sampling_rate;
            info[2] = sensors[i].resolution;
            info[3] = sensors[i].last_value;
            info[4] = sensors[i].min_val;
            info[5] = sensors[i].max_val;
            info[6] = sensors[i].reading_count;
            info[7] = sensors[i].gpio_pin;
            return 0;
        }
    }
    return -1;
}

/* Detect anomaly in sensor readings
   sensor_id: sensor, threshold: deviation threshold
   Returns: 1=anomaly detected, 0=normal, -1=error */
int sensor_detect_anomaly(int sensor_id, int threshold) {
    int idx = -1;
    for (int i = 0; i < sensor_count; i++) {
        if (sensors[i].sensor_id == sensor_id) {
            idx = i;
            break;
        }
    }
    if (idx < 0 || sensors[idx].reading_count < 3) return -1;

    /* Calculate mean and std deviation from history */
    int sum = 0;
    int n = 0;
    for (int i = 0; i < READINGS_HISTORY; i++) {
        if (history[idx][i].timestamp > 0) {
            sum += history[idx][i].value;
            n++;
        }
    }
    if (n < 2) return -1;

    int mean = sum / n;
    int last = sensors[idx].last_value;
    int diff = last - mean;
    if (diff < 0) diff = -diff;

    return (diff > threshold) ? 1 : 0;
}

/* Print sensor dashboard */
void sensor_print_dashboard(void) {
    print_str("=== Sensor Dashboard ===\n");
    print_str("ID\tNAME\t\tTYPE\tVALUE\tMIN\tMAX\tREADINGS\n");

    for (int i = 0; i < sensor_count; i++) {
        print_str("  ");
        print_int(sensors[i].sensor_id);
        print_str("\t");
        print_str(sensors[i].name);
        print_str("\t");
        print_str(sensor_type_name(sensors[i].type));
        print_str("\t");
        print_int(sensors[i].last_value);
        print_str("\t");
        print_int(sensors[i].min_val);
        print_str("\t");
        print_int(sensors[i].max_val);
        print_str("\t");
        print_int(sensors[i].reading_count);
        print_str("\n");
    }
}

/* ===== CLI entry ===== */
void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);

    int help = 0, test = 0, dashboard = 0;
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;
    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0) help = 1;
        else if (my_strcmp(arg, "-t") == 0 || my_strcmp(arg, "--test") == 0) test = 1;
        else if (my_strcmp(arg, "-d") == 0 || my_strcmp(arg, "--dashboard") == 0) dashboard = 1;
        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    print_str("Sensor Hub v13.0 - IoT Sensor Data Acquisition\n");

    if (help) {
        print_str("Usage: sensor_hub [options]\n");
        print_str("  -h, --help        Show this help\n");
        print_str("  -t, --test        Run sensor test\n");
        print_str("  -d, --dashboard   Show sensor dashboard\n");
        return;
    }

    if (test) {
        print_str("=== Sensor Hub Test ===\n");

        /* Register sensors */
        print_str("\n--- Register Sensors ---\n");
        int s1 = sensor_register_sensor(SENSOR_TEMP, "room_temp", 0);
        int s2 = sensor_register_sensor(SENSOR_HUMIDITY, "room_humidity", 0);
        int s3 = sensor_register_sensor(SENSOR_PRESSURE, "barometer", 0);
        int s4 = sensor_register_sensor(SENSOR_LIGHT, "light_sensor", 0);

        /* Set sampling rates */
        print_str("\n--- Set Sampling Rates ---\n");
        sensor_set_rate(s1, 10);
        sensor_set_rate(s2, 5);

        /* Read sensors multiple times */
        print_str("\n--- Reading Sensors ---\n");
        for (int i = 0; i < 5; i++) {
            int v1 = sensor_read(s1);
            int v2 = sensor_read(s2);
            int v3 = sensor_read(s3);
            int v4 = sensor_read(s4);
            print_str("  Read #");
            print_int(i + 1);
            print_str(": temp=");
            print_int(v1);
            print_str("C hum=");
            print_int(v2);
            print_str("% pres=");
            print_int(v3);
            print_str("hPa light=");
            print_int(v4);
            print_str("lux\n");
        }

        /* Get metadata */
        print_str("\n--- Sensor Metadata ---\n");
        unsigned int meta_buf = host_alloc(32, 16);
        int *meta = (int *)meta_buf;
        sensor_get_metadata(s1, meta);
        print_str("  temp: type=");
        print_int(meta[0]);
        print_str(" rate=");
        print_int(meta[1]);
        print_str("Hz res=");
        print_int(meta[2]);
        print_str("-bit readings=");
        print_int(meta[6]);
        print_str("\n");

        /* Anomaly detection */
        print_str("\n--- Anomaly Detection ---\n");
        int anomaly = sensor_detect_anomaly(s1, 10);
        print_str("  temp anomaly: ");
        print_int(anomaly);
        print_str("\n");

        /* Batch read */
        print_str("\n--- Batch Read ---\n");
        unsigned int batch_buf = host_alloc(256, 16);
        int *batch = (int *)batch_buf;
        int count = sensor_read_batch(s1, batch, 5);
        print_str("  Batch: ");
        print_int(count);
        print_str(" readings\n");

        /* Dashboard */
        print_str("\n--- Dashboard ---\n");
        sensor_print_dashboard();

        print_str("\n=== Test Complete ===\n");
        return;
    }

    if (dashboard) {
        sensor_print_dashboard();
        return;
    }

    print_str("Use -h for help, -t for test, -d for dashboard\n");
}
