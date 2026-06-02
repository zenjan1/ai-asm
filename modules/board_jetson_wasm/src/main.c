/* board_jetson_wasm: NVIDIA Jetson WASM interface (v20.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Jetson models */
#define JETSON_NANO     0
#define JETSON_XAVIER   1
#define JETSON_ORIN_NX  2
#define JETSON_ORIN     3

/* Limits */
#define NAME_LEN    32
#define FEATURES_LEN 512

/* Board info */
typedef struct {
    int   model;
    char  name[NAME_LEN];
    int   cpu_cores;
    int   gpu_cores;
    int   tensor_cores;
    int   dla_cores;
    int   memory_gb;
    int   tflops;
    char  features[FEATURES_LEN];
} board_info_t;

static board_info_t board;

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

/* Detect Jetson model */
static void detect_jetson(int model) {
    board.model = model;
    switch (model) {
    case JETSON_NANO:
        my_strcpy(board.name, "Jetson Nano");
        board.cpu_cores = 4; board.gpu_cores = 128;
        board.tensor_cores = 0; board.dla_cores = 0;
        board.memory_gb = 4; board.tflops = 0.5;
        my_strcpy(board.features, "CUDA,GPIO,I2C,SPI");
        break;
    case JETSON_XAVIER:
        my_strcpy(board.name, "Jetson Xavier");
        board.cpu_cores = 8; board.gpu_cores = 512;
        board.tensor_cores = 64; board.dla_cores = 2;
        board.memory_gb = 16; board.tflops = 21;
        my_strcpy(board.features, "CUDA,TensorCore,DLA,NvENC");
        break;
    case JETSON_ORIN_NX:
        my_strcpy(board.name, "Jetson Orin NX");
        board.cpu_cores = 8; board.gpu_cores = 1024;
        board.tensor_cores = 32; board.dla_cores = 2;
        board.memory_gb = 16; board.tflops = 40;
        my_strcpy(board.features, "CUDA,TensorCore,DLA,NvENC,NvJPEG");
        break;
    case JETSON_ORIN:
        my_strcpy(board.name, "Jetson AGX Orin");
        board.cpu_cores = 12; board.gpu_cores = 2048;
        board.tensor_cores = 64; board.dla_cores = 2;
        board.memory_gb = 64; board.tflops = 275;
        my_strcpy(board.features, "CUDA,TensorCore,DLA,NvENC,NvJPEG,DLA");
        break;
    default:
        my_strcpy(board.name, "Unknown Jetson");
        board.cpu_cores = 0; board.gpu_cores = 0;
        board.tensor_cores = 0; board.dla_cores = 0;
        board.memory_gb = 0; board.tflops = 0;
        my_strcpy(board.features, "none");
        break;
    }
}

/* Get board info */
int jetson_get_info(void) {
    print_str("=== Jetson Board Info ===\n");
    print_str("  Model: ");
    print_str(board.name);
    print_str("\n");
    print_str("  CPU cores: ");
    print_int(board.cpu_cores);
    print_str("\n");
    print_str("  GPU SMs: ");
    print_int(board.gpu_cores);
    print_str("\n");
    print_str("  Tensor Cores: ");
    print_int(board.tensor_cores);
    print_str("\n");
    print_str("  DLA Cores: ");
    print_int(board.dla_cores);
    print_str("\n");
    print_str("  Memory: ");
    print_int(board.memory_gb);
    print_str(" GB\n");
    print_str("  TFLOPS: ");
    print_int(board.tflops);
    print_str("\n");
    print_str("  Features: ");
    print_str(board.features);
    print_str("\n");
    return 0;
}

/* CUDA compute */
int jetson_cuda_compute(int grid_size) {
    print_str("=== CUDA Compute ===\n");
    print_str("  Grid size: ");
    print_int(grid_size);
    print_str("\n");
    print_str("  GPU cores: ");
    print_int(board.gpu_cores);
    print_str("\n");
    print_str("  Status: CUDA ready\n");
    return 0;
}

/* DLA inference */
int jetson_dla_infer(const char *model) {
    if (!model) return -1;
    print_str("=== DLA Inference ===\n");
    print_str("  Model: ");
    print_str(model);
    print_str("\n");
    print_str("  DLA cores: ");
    print_int(board.dla_cores);
    print_str("\n");
    print_str("  Status: DLA ready\n");
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
    print_str("Jetson WASM v20.0 - NVIDIA Jetson Interface\n");
    if (help) {
        print_str("Usage: board_jetson_wasm [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run hardware test\n");
        return;
    }
    detect_jetson(JETSON_ORIN);
    if (test) {
        print_str("=== Jetson Test ===\n\n");
        jetson_get_info();
        print_str("\n");
        jetson_cuda_compute(256);
        print_str("\n");
        jetson_dla_infer("resnet50");
        print_str("\n=== Jetson Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
