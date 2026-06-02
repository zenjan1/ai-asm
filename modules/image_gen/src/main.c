/* image_gen: DALL-E image generation (v39.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_PROMPT      1000
#define MAX_IMAGES      4
#define MAX_URL         512
#define MAX_MODEL       32
#define MAX_SIZE        32
#define MAX_STYLE       16
#define MAX_FORMAT      16
#define MAX_REVISED     1024

/* Models */
#define MODEL_DALLE3   0
#define MODEL_DALLE2   1
#define MODEL_SD       2

/* Image config */
typedef struct {
    char  model[MAX_MODEL];
    char  size[MAX_SIZE];
    char  quality[MAX_STYLE];    /* standard or hd */
    char  style[MAX_STYLE];      /* vivid or natural */
    char  response_format[MAX_FORMAT];  /* url or b64_json */
    int   n;                     /* number of images */
} image_gen_config_t;

/* Image generation request */
typedef struct {
    char  prompt[MAX_PROMPT];
    char  model[MAX_MODEL];
    int   n;
    char  size[MAX_SIZE];
    char  quality[MAX_STYLE];
    char  style[MAX_STYLE];
    char  response_format[MAX_FORMAT];
} image_gen_request_t;

/* Image result */
typedef struct {
    char  url[MAX_URL];
    int   is_base64;
    char  revised_prompt[MAX_REVISED];
} image_gen_result_t;

/* Image response */
typedef struct {
    image_gen_result_t images[MAX_IMAGES];
    int   count;
    int   created_at;
} image_gen_response_t;

static image_gen_config_t global_config;
static int request_count = 0;

/* String utilities */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
}
static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0; while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; } dst[i] = '\0';
}
static int my_strstr(const char *haystack, const char *needle) {
    int hl = my_strlen(haystack), nl = my_strlen(needle);
    if (nl == 0 || nl > hl) return 0;
    for (int i = 0; i <= hl - nl; i++) {
        int j = 0; while (j < nl && haystack[i+j] == needle[j]) j++;
        if (j == nl) return 1;
    }
    return 0;
}

static void print_str(const char *str) { host_print(str); }
static void print_int(int val) {
    char buf[32]; int pos = 0;
    if (val < 0) { buf[pos++] = '-'; val = -val; }
    if (val == 0) buf[pos++] = '0';
    else { int d = 0, t = val; while (t > 0) { d++; t /= 10; } pos += d; buf[pos] = '\0'; pos--;
        while (val > 0) { buf[pos--] = '0' + (val % 10); val /= 10; } }
    host_print(buf);
}

/* Initialize */
int ig_init(const char *model, const char *size, const char *quality, const char *style) {
    my_strncpy(global_config.model, model ? model : "dall-e-3", MAX_MODEL - 1);
    my_strncpy(global_config.size, size ? size : "1024x1024", MAX_SIZE - 1);
    my_strncpy(global_config.quality, quality ? quality : "standard", MAX_STYLE - 1);
    my_strncpy(global_config.style, style ? style : "vivid", MAX_STYLE - 1);
    my_strncpy(global_config.response_format, "url", MAX_FORMAT - 1);
    global_config.n = 1;
    return 0;
}

/* Generate image (simulated) */
int ig_generate(const char *prompt, image_gen_response_t *resp) {
    request_count++;
    int prompt_len = my_strlen(prompt);
    if (prompt_len == 0) return -1;
    int n = global_config.n > MAX_IMAGES ? MAX_IMAGES : (global_config.n > 0 ? global_config.n : 1);
    resp->count = n;
    resp->created_at = request_count * 1000;
    for (int i = 0; i < n; i++) {
        /* Simulate URL */
        int offset = 0;
        my_strncpy(resp->images[i].url, "https://oaidalleapi.example.com/v1/images/gen_", MAX_URL - 1);
        my_strncpy(&resp->images[i].url[my_strlen(resp->images[i].url)], "abcdef", MAX_URL - my_strlen(resp->images[i].url) - 1);
        char idx_buf[4];
        idx_buf[0] = '0' + i; idx_buf[1] = '\0';
        my_strncpy(&resp->images[i].url[my_strlen(resp->images[i].url)], idx_buf, MAX_URL - my_strlen(resp->images[i].url) - 1);
        resp->images[i].is_base64 = 0;
        /* Revised prompt */
        my_strncpy(resp->images[i].revised_prompt, prompt, MAX_REVISED - 1);
    }
    return n;
}

/* Edit image (simulated) */
int ig_edit(const char *image, const char *mask, const char *prompt, image_gen_response_t *resp) {
    request_count++;
    resp->count = 1;
    my_strncpy(resp->images[0].url, "https://oaidalleapi.example.com/v1/images/edit_001", MAX_URL - 1);
    resp->images[0].is_base64 = 0;
    my_strncpy(resp->images[0].revised_prompt, prompt, MAX_REVISED - 1);
    return 1;
}

/* Variation (simulated) */
int ig_variation(const char *image, image_gen_response_t *resp) {
    request_count++;
    resp->count = 2;
    my_strncpy(resp->images[0].url, "https://oaidalleapi.example.com/v1/images/var_001", MAX_URL - 1);
    my_strncpy(resp->images[1].url, "https://oaidalleapi.example.com/v1/images/var_002", MAX_URL - 1);
    resp->images[0].is_base64 = resp->images[1].is_base64 = 0;
    return 2;
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
    print_str("Image Gen v39.0 - DALL-E Image Generation\n");
    if (help) { print_str("Usage: image_gen [options]\n  -h, --help    Show help\n  -t, --test    Run test\n"); return; }
    if (test) {
        print_str("=== Image Gen Test ===\n\n");
        ig_init("dall-e-3", "1024x1024", "hd", "vivid");
        print_str("  Model: "); print_str(global_config.model); print_str("\n");
        print_str("  Size: "); print_str(global_config.size); print_str("\n");
        print_str("  Quality: "); print_str(global_config.quality); print_str("\n");
        print_str("  Style: "); print_str(global_config.style); print_str("\n\n");
        /* Generate */
        const char *prompt = "A futuristic cityscape with flying cars and neon lights at sunset, photorealistic style";
        image_gen_response_t resp;
        int count = ig_generate(prompt, &resp);
        print_str("Generation ("); print_int(count); print_str(" images):\n");
        for (int i = 0; i < count; i++) {
            print_str("  ["); print_int(i + 1); print_str("] "); print_str(resp.images[i].url); print_str("\n");
        }
        print_str("\n");
        /* Edit */
        image_gen_response_t edit_resp;
        ig_edit("https://example.com/original.jpg", "mask.png", "Add more trees to the background", &edit_resp);
        print_str("Edit:\n");
        print_str("  URL: "); print_str(edit_resp.images[0].url); print_str("\n\n");
        /* Variation */
        image_gen_response_t var_resp;
        ig_variation("https://example.com/image.jpg", &var_resp);
        print_str("Variations:\n");
        for (int i = 0; i < var_resp.count; i++) {
            print_str("  ["); print_int(i + 1); print_str("] "); print_str(var_resp.images[i].url); print_str("\n");
        }
        print_str("\n=== Image Gen Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
