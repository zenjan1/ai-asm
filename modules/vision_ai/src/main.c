/* vision_ai: Visual AI module (v16.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Vision tasks */
#define VISION_CLASSIFY   0
#define VISION_DETECT     1
#define VISION_SEGMENT    2
#define VISION_TRACK      3
#define VISION_OCR        4
#define VISION_FACE       5

/* Image formats */
#define FORMAT_RGB   0
#define FORMAT_RGBA  1
#define FORMAT_GRAY  2

/* Limits */
#define MAX_VISION_MODELS  8
#define MAX_DETECTIONS    32
#define MAX_CAMERAS        4
#define LABEL_LEN         64
#define FEATURE_LEN       256
#define MODEL_PATH_LEN    128
#define TEXT_LEN          512

/* Detection result */
typedef struct {
    int   object_id;
    char  label[LABEL_LEN];
    int   confidence;    /* 0-100 */
    int   x, y, w, h;    /* bounding box */
    char  features[FEATURE_LEN];
} detection_t;

/* Vision model */
typedef struct {
    int   model_id;
    char  name[MODEL_PATH_LEN];
    int   task_type;
    int   loaded;
    int   inference_count;
    int   avg_latency_ms;
} vision_model_t;

static vision_model_t models[MAX_VISION_MODELS];
static int model_count = 0;
static int next_model_id = 1;
static int total_frames_processed = 0;

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

/* Get task name */
static const char *task_name(int task) {
    if (task == VISION_CLASSIFY) return "classify";
    if (task == VISION_DETECT) return "detect";
    if (task == VISION_SEGMENT) return "segment";
    if (task == VISION_TRACK) return "track";
    if (task == VISION_OCR) return "ocr";
    if (task == VISION_FACE) return "face";
    return "unknown";
}

/* Load a vision model */
int vision_load_model(const char *model_path) {
    if (!model_path || model_count >= MAX_VISION_MODELS) return -1;
    int idx = model_count;
    models[idx].model_id = next_model_id;
    my_strncpy(models[idx].name, model_path, MODEL_PATH_LEN - 1);
    models[idx].task_type = VISION_DETECT;
    models[idx].loaded = 1;
    models[idx].inference_count = 0;
    models[idx].avg_latency_ms = 0;

    /* Determine task from model name */
    if (my_strncmp(model_path, "classify", 8) == 0) models[idx].task_type = VISION_CLASSIFY;
    else if (my_strncmp(model_path, "detect", 6) == 0) models[idx].task_type = VISION_DETECT;
    else if (my_strncmp(model_path, "segment", 7) == 0) models[idx].task_type = VISION_SEGMENT;
    else if (my_strncmp(model_path, "ocr", 3) == 0) models[idx].task_type = VISION_OCR;
    else if (my_strncmp(model_path, "face", 4) == 0) models[idx].task_type = VISION_FACE;

    int id = next_model_id++;
    model_count++;
    print_str("[vision] loaded: ");
    print_str(model_path);
    print_str(" (task=");
    print_str(task_name(models[idx].task_type));
    print_str(")\n");
    return id;
}

/* Classify an image */
int vision_classify(const char *image_desc, char *label, int *confidence) {
    if (!image_desc || !label) return -1;
    print_str("[vision] classify: ");
    print_str(image_desc);
    print_str("\n");

    /* Simulated classification */
    if (my_strncmp(image_desc, "cat", 3) == 0) { my_strcpy(label, "cat"); *confidence = 95; }
    else if (my_strncmp(image_desc, "dog", 3) == 0) { my_strcpy(label, "dog"); *confidence = 92; }
    else if (my_strncmp(image_desc, "car", 3) == 0) { my_strcpy(label, "car"); *confidence = 88; }
    else if (my_strncmp(image_desc, "person", 6) == 0) { my_strcpy(label, "person"); *confidence = 97; }
    else if (my_strncmp(image_desc, "tree", 4) == 0) { my_strcpy(label, "tree"); *confidence = 85; }
    else { my_strcpy(label, "unknown"); *confidence = 30; }

    print_str("  result: ");
    print_str(label);
    print_str(" confidence=");
    print_int(*confidence);
    print_str("%\n");
    total_frames_processed++;
    return 0;
}

/* Detect objects in image */
int vision_detect_objects(const char *image_desc, detection_t *results, int *count) {
    if (!results || !count) return -1;
    print_str("[vision] detect: ");
    print_str(image_desc);
    print_str("\n");

    *count = 0;
    /* Simulated detection */
    if (my_strncmp(image_desc, "street", 6) == 0) {
        my_strcpy(results[0].label, "car"); results[0].confidence = 94; results[0].x = 10; results[0].y = 50; results[0].w = 200; results[0].h = 150;
        (*count)++;
        my_strcpy(results[1].label, "person"); results[1].confidence = 89; results[1].x = 300; results[1].y = 80; results[1].w = 60; results[1].h = 180;
        (*count)++;
        my_strcpy(results[2].label, "traffic_light"); results[2].confidence = 91; results[2].x = 500; results[2].y = 10; results[2].w = 40; results[2].h = 100;
        (*count)++;
    } else if (my_strncmp(image_desc, "indoor", 6) == 0) {
        my_strcpy(results[0].label, "person"); results[0].confidence = 96; results[0].x = 100; results[0].y = 50; results[0].w = 120; results[0].h = 300;
        (*count)++;
        my_strcpy(results[1].label, "chair"); results[1].confidence = 82; results[1].x = 300; results[1].y = 200; results[1].w = 80; results[1].h = 100;
        (*count)++;
    }

    print_str("  detected ");
    print_int(*count);
    print_str(" objects\n");
    for (int i = 0; i < *count; i++) {
        print_str("    [");
        print_str(results[i].label);
        print_str("] conf=");
        print_int(results[i].confidence);
        print_str(" box=(");
        print_int(results[i].x);
        print_str(",");
        print_int(results[i].y);
        print_str(",");
        print_int(results[i].w);
        print_str(",");
        print_int(results[i].h);
        print_str(")\n");
    }
    total_frames_processed++;
    return 0;
}

/* OCR text recognition */
int vision_ocr(const char *image_desc, char *text) {
    if (!text) return -1;
    print_str("[vision] OCR: ");
    print_str(image_desc);
    print_str("\n");

    /* Simulated OCR */
    if (my_strncmp(image_desc, "document", 8) == 0) {
        my_strcpy(text, "AI-ASM OS Documentation\nVersion 16.0");
    } else if (my_strncmp(image_desc, "sign", 4) == 0) {
        my_strcpy(text, "STOP");
    } else if (my_strncmp(image_desc, "license_plate", 13) == 0) {
        my_strcpy(text, "ABC-1234");
    } else {
        my_strcpy(text, "text not recognized");
    }
    print_str("  text: ");
    print_str(text);
    print_str("\n");
    total_frames_processed++;
    return 0;
}

/* Process an image */
int vision_process_image(const char *image_desc, int task) {
    char label[LABEL_LEN];
    int confidence = 0;
    detection_t detections[MAX_DETECTIONS];
    int det_count = 0;
    char ocr_text[TEXT_LEN];

    if (task == VISION_CLASSIFY) return vision_classify(image_desc, label, &confidence);
    if (task == VISION_DETECT) return vision_detect_objects(image_desc, detections, &det_count);
    if (task == VISION_OCR) return vision_ocr(image_desc, ocr_text);

    print_str("[vision] process (task=");
    print_str(task_name(task));
    print_str("): ");
    print_str(image_desc);
    print_str("\n");
    total_frames_processed++;
    return 0;
}

/* Stream processing */
int vision_stream_process(int camera_id, int task) {
    print_str("[vision] stream: camera=");
    print_int(camera_id);
    print_str(" task=");
    print_str(task_name(task));
    print_str("\n");
    print_str("  Processing frames...\n");
    print_str("  Frame 1: processed\n");
    print_str("  Frame 2: processed\n");
    print_str("  Frame 3: processed\n");
    total_frames_processed += 3;
    print_str("  Total frames: ");
    print_int(total_frames_processed);
    print_str("\n");
    return 0;
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
    print_str("Vision AI v16.0 - Computer Vision\n");
    if (help) {
        print_str("Usage: vision_ai [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run vision test\n");
        return;
    }
    if (test) {
        print_str("=== Vision AI Test ===\n");
        print_str("\n--- Load Models ---\n");
        vision_load_model("/models/detect_yolo.onnx");
        vision_load_model("/models/classify_resnet.tflite");
        vision_load_model("/models/ocr_recognizer.onnx");
        vision_load_model("/models/face_detect.onnx");

        print_str("\n--- Classification ---\n");
        char label[LABEL_LEN]; int conf;
        vision_classify("cat", label, &conf);
        vision_classify("car", label, &conf);
        vision_classify("person", label, &conf);

        print_str("\n--- Object Detection ---\n");
        detection_t dets[MAX_DETECTIONS]; int dc;
        vision_detect_objects("street scene", dets, &dc);
        vision_detect_objects("indoor room", dets, &dc);

        print_str("\n--- OCR ---\n");
        char text[TEXT_LEN];
        vision_ocr("document page", text);
        vision_ocr("license plate", text);

        print_str("\n--- Stream Processing ---\n");
        vision_stream_process(0, VISION_DETECT);

        print_str("\n=== Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
