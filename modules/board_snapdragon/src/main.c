/* board_snapdragon: Qualcomm Snapdragon interface (v20.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Snapdragon models */
#define SD_8CX_GEN1  0
#define SD_8CX_GEN2  1
#define SD_8CX_GEN3  2
#define SD_X_ELITE   3

/* Limits */
#define NAME_LEN    32
#define FEATURES_LEN 512

/* Board info */
typedef struct {
    int   model;
    char  name[NAME_LEN];
    int   kryo_cores;
    int   hexagon_units;
    int   adreno_cores;
    int   qnn_units;
    int   memory_gb;
    int   npu_tops;
    char  features[FEATURES_LEN];
} sd_board_t;

static sd_board_t board;

/* String utilities */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
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

/* Detect Snapdragon model */
static void detect_snapdragon(int model) {
    board.model = model;
    switch (model) {
    case SD_8CX_GEN1:
        my_strcpy(board.name, "SD 8cx Gen1");
        board.kryo_cores = 8; board.hexagon_units = 1;
        board.adreno_cores = 1; board.qnn_units = 1;
        board.memory_gb = 8; board.npu_tops = 2;
        my_strcpy(board.features, "Kryo,Hexagon,Adreno,QNN");
        break;
    case SD_8CX_GEN2:
        my_strcpy(board.name, "SD 8cx Gen2");
        board.kryo_cores = 8; board.hexagon_units = 1;
        board.adreno_cores = 1; board.qnn_units = 2;
        board.memory_gb = 16; board.npu_tops = 4;
        my_strcpy(board.features, "Kryo,Hexagon,Adreno,QNN");
        break;
    case SD_8CX_GEN3:
        my_strcpy(board.name, "SD 8cx Gen3");
        board.kryo_cores = 8; board.hexagon_units = 2;
        board.adreno_cores = 2; board.qnn_units = 4;
        board.memory_gb = 16; board.npu_tops = 11;
        my_strcpy(board.features, "Kryo,Hexagon,Adreno,QNN,5G");
        break;
    case SD_X_ELITE:
        my_strcpy(board.name, "SD X Elite");
        board.kryo_cores = 12; board.hexagon_units = 4;
        board.adreno_cores = 4; board.qnn_units = 8;
        board.memory_gb = 32; board.npu_tops = 45;
        my_strcpy(board.features, "Kryo,Hexagon,Adreno,QNN,5G,LTE");
        break;
    default:
        my_strcpy(board.name, "Unknown Snapdragon");
        board.kryo_cores = 0; board.hexagon_units = 0;
        board.adreno_cores = 0; board.qnn_units = 0;
        board.memory_gb = 0; board.npu_tops = 0;
        my_strcpy(board.features, "none");
        break;
    }
}

/* Get board info */
int sd_get_info(void) {
    print_str("=== Snapdragon Board Info ===\n");
    print_str("  Model: ");
    print_str(board.name);
    print_str("\n");
    print_str("  Kryo cores: ");
    print_int(board.kryo_cores);
    print_str("\n");
    print_str("  Hexagon DSP units: ");
    print_int(board.hexagon_units);
    print_str("\n");
    print_str("  Adreno GPU cores: ");
    print_int(board.adreno_cores);
    print_str("\n");
    print_str("  QNN units: ");
    print_int(board.qnn_units);
    print_str("\n");
    print_str("  Memory: ");
    print_int(board.memory_gb);
    print_str(" GB\n");
    print_str("  NPU TOPS: ");
    print_int(board.npu_tops);
    print_str("\n");
    print_str("  Features: ");
    print_str(board.features);
    print_str("\n");
    return 0;
}

/* Hexagon DSP processing */
int sd_hexagon_process(const char *task) {
    if (!task) return -1;
    print_str("=== Hexagon DSP Processing ===\n");
    print_str("  Task: ");
    print_str(task);
    print_str("\n");
    print_str("  DSP units: ");
    print_int(board.hexagon_units);
    print_str("\n");
    print_str("  Status: DSP ready\n");
    return 0;
}

/* QNN inference */
int sd_qnn_infer(const char *model) {
    if (!model) return -1;
    print_str("=== QNN Inference ===\n");
    print_str("  Model: ");
    print_str(model);
    print_str("\n");
    print_str("  NPU TOPS: ");
    print_int(board.npu_tops);
    print_str("\n");
    print_str("  Status: QNN ready\n");
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
    print_str("Snapdragon v20.0 - Qualcomm Snapdragon Interface\n");
    if (help) {
        print_str("Usage: board_snapdragon [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run hardware test\n");
        return;
    }
    detect_snapdragon(SD_X_ELITE);
    if (test) {
        print_str("=== Snapdragon Test ===\n\n");
        sd_get_info();
        print_str("\n");
        sd_hexagon_process("audio_processing");
        print_str("\n");
        sd_qnn_infer("mobilenet");
        print_str("\n=== Snapdragon Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
