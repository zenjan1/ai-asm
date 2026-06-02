/* whisper_api: OpenAI Whisper speech recognition (v39.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_SEGMENTS    64
#define MAX_TEXT        2048
#define MAX_AUDIO       4096
#define MAX_MODEL       64
#define MAX_LANG        16
#define MAX_WORD        64

/* Whisper segment */
typedef struct {
    int   id;
    float start;   /* seconds * 100 */
    float end;     /* seconds * 100 */
    char  text[MAX_TEXT];
    int   no_speech_prob;  /* * 10000 */
} whisper_segment_t;

/* Whisper config */
typedef struct {
    char  model[MAX_MODEL];
    char  language[MAX_LANG];
    int   response_format; /* 0=json, 1=text, 2=srt, 3=vtt */
    int   timestamp_granularity; /* 0=segment, 1=word */
    char  prompt[MAX_TEXT];
    int   temperature; /* * 100 */
} whisper_config_t;

/* Whisper response */
typedef struct {
    char  text[MAX_TEXT];
    whisper_segment_t segments[MAX_SEGMENTS];
    int   segment_count;
    char  language_detected[MAX_LANG];
    float duration;    /* seconds * 100 */
    int   tokens_used;
} whisper_response_t;

static whisper_config_t global_config;
static whisper_response_t last_response;
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
static void print_float(int val, int dec) {
    char buf[32]; int pos = 0, frac = 1;
    for (int i = 0; i < dec; i++) frac *= 10;
    int ip = val / frac, rem = val % frac;
    if (ip == 0) buf[pos++] = '0';
    else { int dd = 0, t = ip; while (t > 0) { dd++; t /= 10; } pos += dd; buf[pos] = '\0'; pos--;
        while (ip > 0) { buf[pos--] = '0' + (ip % 10); ip /= 10; } }
    buf[pos++] = '.';
    int fp = pos + dec - 1; buf[fp + 1] = '\0';
    for (int i = 0; i < dec; i++) { buf[fp - i] = '0' + (rem % 10); rem /= 10; }
    host_print(buf);
}

/* Initialize config */
int wa_init(const char *model, const char *language, int response_format) {
    my_strncpy(global_config.model, model ? model : "whisper-1", MAX_MODEL - 1);
    my_strncpy(global_config.language, language ? language : "en", MAX_LANG - 1);
    global_config.response_format = response_format;
    global_config.timestamp_granularity = 0;
    global_config.prompt[0] = '\0';
    global_config.temperature = 0;
    return 0;
}

/* Transcribe audio (simulated) */
int wa_transcribe(const char *audio_data, whisper_response_t *resp) {
    request_count++;
    int audio_len = my_strlen(audio_data);
    if (audio_len == 0) return -1;
    /* Simulate transcription based on audio content */
    resp->segment_count = 0;
    resp->tokens_used = audio_len / 4 + 1;
    total_tokens += resp->tokens_used;
    my_strncpy(resp->language_detected, global_config.language, MAX_LANG - 1);
    resp->duration = (audio_len * 10) / 100; /* simulate duration */
    /* Generate simulated segments */
    const char *simulated_texts[4] = {
        "Welcome to the presentation on artificial intelligence and machine learning.",
        "Today we will explore the latest developments in natural language processing.",
        "Neural networks have revolutionized the way we approach complex problems.",
        "The future of AI holds tremendous promise for solving real-world challenges."
    };
    int num_seg = audio_len > 100 ? 4 : audio_len > 50 ? 2 : 1;
    if (num_seg > MAX_SEGMENTS) num_seg = MAX_SEGMENTS;
    float step = (float)resp->duration / num_seg;
    for (int i = 0; i < num_seg; i++) {
        int si = i % 4;
        my_strncpy(resp->segments[i].text, simulated_texts[si], MAX_TEXT - 1);
        resp->segments[i].id = i;
        resp->segments[i].start = (int)(step * i * 100);
        resp->segments[i].end = (int)(step * (i + 1) * 100);
        resp->segments[i].no_speech_prob = 500; /* 0.05 */
        /* Append to full text */
        int len = my_strlen(resp->text);
        if (len > 0) resp->text[len++] = ' ';
        my_strncpy(&resp->text[len], resp->segments[i].text, MAX_TEXT - len - 1);
    }
    resp->segment_count = num_seg;
    return resp->segment_count;
}

/* Translate audio to English (simulated) */
int wa_translate(const char *audio_data, whisper_response_t *resp) {
    my_strncpy(global_config.language, "en", MAX_LANG - 1);
    int rc = wa_transcribe(audio_data, resp);
    if (rc > 0) {
        /* Mark as translated */
        for (int i = 0; i < resp->segment_count; i++) {
            resp->segments[i].no_speech_prob = 200; /* lower = more confident */
        }
    }
    return rc;
}

/* Detect language (simulated) */
int wa_detect_language(const char *audio_data) {
    /* Simple heuristic: return configured language */
    my_strncpy(last_response.language_detected, global_config.language, MAX_LANG - 1);
    int score = 9500; /* 0.95 confidence */
    return score;
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
    print_str("Whisper API v39.0 - Speech Recognition\n");
    if (help) { print_str("Usage: whisper_api [options]\n  -h, --help    Show help\n  -t, --test    Run test\n"); return; }
    if (test) {
        print_str("=== Whisper API Test ===\n\n");
        wa_init("whisper-1", "en", 0);
        print_str("  Model: "); print_str(global_config.model); print_str("\n");
        print_str("  Language: "); print_str(global_config.language); print_str("\n\n");
        /* Transcribe */
        const char *audio = "simulated_audio_data_for_testing_purposes_with_speech_recognition_capabilities";
        int segs = wa_transcribe(audio, &last_response);
        print_str("Transcription:\n");
        print_str("  Duration: "); print_float((int)last_response.duration, 2); print_str("s\n");
        print_str("  Language: "); print_str(last_response.language_detected); print_str("\n");
        print_str("  Segments: "); print_int(segs); print_str("\n\n");
        for (int i = 0; i < segs; i++) {
            print_str("  ["); print_float((int)last_response.segments[i].start, 2);
            print_str("-"); print_float((int)last_response.segments[i].end, 2);
            print_str("] "); print_str(last_response.segments[i].text); print_str("\n");
        }
        print_str("\n  Tokens: "); print_int(last_response.tokens_used); print_str("\n");
        /* Translate */
        const char *foreign = "foreign_language_audio_data_for_translation_testing";
        whisper_response_t translated;
        int tseg = wa_translate(foreign, &translated);
        print_str("\nTranslation:\n");
        print_str("  Segments: "); print_int(tseg); print_str("\n");
        print_str("  Full text: "); print_str(translated.text); print_str("\n");
        /* Language detection */
        int lang_score = wa_detect_language(audio);
        print_str("\nLanguage detection confidence: "); print_float(lang_score, 4); print_str("\n");
        print_str("\n=== Whisper API Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
