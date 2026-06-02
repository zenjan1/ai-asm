/* vision_api: Multi-provider vision analysis (v39.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_IMAGES    4
#define MAX_IMG_URL   1024
#define MAX_PROMPT    512
#define MAX_DESC      2048
#define MAX_MODEL     64
#define MAX_PROVIDER  32
#define MAX_OCR       1024

/* Providers */
#define PROVIDER_OPENAI   0
#define PROVIDER_CLAUDE   1
#define PROVIDER_GEMINI   2

/* Detail levels */
#define DETAIL_LOW    0
#define DETAIL_HIGH   1
#define DETAIL_AUTO   2

/* Vision image */
typedef struct {
    char  url_or_base64[MAX_IMG_URL];
    int   detail;   /* 0=low, 1=high, 2=auto */
    int   is_base64;
} vision_image_t;

/* Vision config */
typedef struct {
    char  provider[MAX_PROVIDER];
    char  model[MAX_MODEL];
    int   max_tokens;
    int   detail_level;
} vision_config_t;

/* Vision request */
typedef struct {
    vision_image_t images[MAX_IMAGES];
    int   image_count;
    char  prompt[MAX_PROMPT];
    int   max_tokens;
} vision_request_t;

/* Vision response */
typedef struct {
    char  description[MAX_DESC];
    int   confidence;    /* * 100 */
    char  provider_used[MAX_PROVIDER];
    int   tokens_used;
    int   latency_ms;
} vision_response_t;

static vision_config_t global_config;
static vision_response_t last_response;
static int request_count = 0;
static int total_tokens = 0;

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
int vi_init(const char *provider, const char *model, int max_tokens) {
    my_strncpy(global_config.provider, provider ? provider : "openai", MAX_PROVIDER - 1);
    my_strncpy(global_config.model, model ? model : "gpt-4-vision-preview", MAX_MODEL - 1);
    global_config.max_tokens = max_tokens > 0 ? max_tokens : 300;
    global_config.detail_level = DETAIL_AUTO;
    return 0;
}

/* Analyze image(s) (simulated) */
int vi_analyze(vision_image_t images[], int count, const char *prompt, vision_response_t *resp) {
    request_count++;
    if (count == 0 || count > MAX_IMAGES) return -1;
    my_strncpy(resp->provider_used, global_config.provider, MAX_PROVIDER - 1);
    /* Simulate analysis based on image count and prompt */
    int prompt_len = my_strlen(prompt);
    resp->tokens_used = prompt_len / 4 + 1 + count * 85;
    resp->latency_ms = 500 + count * 200;
    total_tokens += resp->tokens_used;
    /* Generate simulated description */
    if (count > 1) {
        my_strncpy(resp->description, "The provided images show ", MAX_DESC - 1);
    } else {
        my_strncpy(resp->description, "The image shows ", MAX_DESC - 1);
    }
    int len = my_strlen(resp->description);
    if (my_strstr(prompt, "describe") || my_strstr(prompt, "what")) {
        my_strncpy(&resp->description[len], "a detailed scene with multiple elements including objects, text, and visual patterns that can be identified and analyzed.", MAX_DESC - len - 1);
    } else if (my_strstr(prompt, "count")) {
        my_strncpy(&resp->description[len], "approximately 12 distinct objects visible in the image, arranged in a structured pattern.", MAX_DESC - len - 1);
    } else if (my_strstr(prompt, "color")) {
        my_strncpy(&resp->description[len], "a variety of colors including blues, greens, and warm tones dominating the visual composition.", MAX_DESC - len - 1);
    } else {
        my_strncpy(&resp->description[len], "visual content matching the query with relevant details about the scene composition and key elements.", MAX_DESC - len - 1);
    }
    resp->confidence = 82 + count * 3;
    return resp->tokens_used;
}

/* Describe single image */
int vi_describe(const char *image_url, vision_response_t *resp) {
    vision_image_t img;
    my_strncpy(img.url_or_base64, image_url, MAX_IMG_URL - 1);
    img.detail = DETAIL_AUTO;
    img.is_base64 = 0;
    return vi_analyze(&img, 1, "Describe this image in detail", resp);
}

/* Compare two images */
int vi_compare(const char *image1, const char *image2, vision_response_t *resp) {
    vision_image_t imgs[2];
    my_strncpy(imgs[0].url_or_base64, image1, MAX_IMG_URL - 1);
    my_strncpy(imgs[1].url_or_base64, image2, MAX_IMG_URL - 1);
    imgs[0].detail = imgs[1].detail = DETAIL_AUTO;
    imgs[0].is_base64 = imgs[1].is_base64 = 0;
    return vi_analyze(imgs, 2, "Compare these two images", resp);
}

/* Extract text (OCR) */
int vi_extract_text(const char *image_url, char *ocr_text, int max_len) {
    request_count++;
    vision_image_t img;
    my_strncpy(img.url_or_base64, image_url, MAX_IMG_URL - 1);
    img.detail = DETAIL_HIGH;
    img.is_base64 = 0;
    /* Simulated OCR */
    my_strncpy(ocr_text, "Sample extracted text from image: This is a simulated OCR result demonstrating text extraction capabilities.", max_len - 1);
    int len = my_strlen(ocr_text);
    resp.tokens_used = len / 4 + 1 + 85;
    total_tokens += resp.tokens_used;
    return len;
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
    print_str("Vision API v39.0 - Multi-Provider Vision Analysis\n");
    if (help) { print_str("Usage: vision_api [options]\n  -h, --help    Show help\n  -t, --test    Run test\n"); return; }
    if (test) {
        print_str("=== Vision API Test ===\n\n");
        vi_init("openai", "gpt-4-vision-preview", 300);
        print_str("  Provider: "); print_str(global_config.provider); print_str("\n");
        print_str("  Model: "); print_str(global_config.model); print_str("\n");
        print_str("  Max tokens: "); print_int(global_config.max_tokens); print_str("\n\n");
        /* Describe */
        const char *img1 = "https://example.com/image1.jpg";
        vi_describe(img1, &last_response);
        print_str("Image description:\n");
        print_str("  "); print_str(last_response.description); print_str("\n");
        print_str("  Confidence: "); print_int(last_response.confidence); print_str("%\n");
        print_str("  Tokens: "); print_int(last_response.tokens_used); print_str("\n");
        print_str("  Latency: "); print_int(last_response.latency_ms); print_str("ms\n\n");
        /* Compare */
        const char *img2 = "https://example.com/image2.jpg";
        vision_response_t comparison;
        vi_compare(img1, img2, &comparison);
        print_str("Image comparison:\n");
        print_str("  "); print_str(comparison.description); print_str("\n\n");
        /* OCR */
        char ocr[MAX_OCR];
        int ocr_len = vi_extract_text(img1, ocr, MAX_OCR);
        print_str("OCR extraction:\n");
        print_str("  Length: "); print_int(ocr_len); print_str("\n");
        print_str("  Text: "); print_str(ocr); print_str("\n");
        print_str("\n=== Vision API Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
