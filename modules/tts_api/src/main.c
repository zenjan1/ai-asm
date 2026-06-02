/* tts_api: Text-to-Speech synthesis (v39.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_TEXT        4096
#define MAX_AUDIO       8192
#define MAX_MODEL       64
#define MAX_VOICE       32
#define MAX_VOICES      6
#define MAX_FORMAT      8

/* TTS voices */
#define VOICE_ALLOY    0
#define VOICE_ECHO     1
#define VOICE_FABLE    2
#define VOICE_ONYX     3
#define VOICE_NOVA     4
#define VOICE_SHIMMER  5

/* TTS config */
typedef struct {
    char  model[MAX_MODEL];       /* tts-1 or tts-1-hd */
    char  voice[MAX_VOICE];
    float speed;                   /* * 100, range 25-400 */
    int   response_format;         /* 0=mp3, 1=opus, 2=aac, 3=flac, 4=wav */
    int   stream;                  /* streaming enabled */
} tts_config_t;

/* TTS request */
typedef struct {
    char  input[MAX_TEXT];
    char  model[MAX_MODEL];
    char  voice[MAX_VOICE];
    int   speed;    /* * 100 */
    int   format;
} tts_request_t;

/* TTS response */
typedef struct {
    char  audio_data[MAX_AUDIO];
    int   audio_len;
    int   duration_ms;
    char  format[MAX_FORMAT];
    char  voice_used[MAX_VOICE];
    int   characters;
    int   cost_cents;
} tts_response_t;

static tts_config_t global_config;
static tts_response_t last_response;
static int request_count = 0;
static int total_chars = 0;

/* Voice info */
typedef struct {
    const char *name;
    const char *gender;
    const char *accent;
    const char *style;
} tts_voice_info_t;

static const tts_voice_info_t voice_infos[MAX_VOICES] = {
    {"alloy",   "neutral", "american", "balanced"},
    {"echo",    "male",    "american", "professional"},
    {"fable",   "male",    "british",  "expressive"},
    {"onyx",    "male",    "american", "deep"},
    {"nova",    "female",  "american", "warm"},
    {"shimmer", "female",  "american", "bright"}
};

/* String utilities */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
}
static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0; while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; } dst[i] = '\0';
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

static const char *format_name(int f) {
    if (f == 0) return "mp3";
    if (f == 1) return "opus";
    if (f == 2) return "aac";
    if (f == 3) return "flac";
    return "wav";
}

/* Initialize config */
int tts_init(const char *model, const char *voice, float speed, int format) {
    my_strncpy(global_config.model, model ? model : "tts-1", MAX_MODEL - 1);
    my_strncpy(global_config.voice, voice ? voice : "alloy", MAX_VOICE - 1);
    global_config.speed = speed;
    global_config.response_format = format;
    global_config.stream = 0;
    return 0;
}

/* Synthesize speech (simulated) */
int tts_synthesize(const char *text, tts_response_t *resp) {
    request_count++;
    int text_len = my_strlen(text);
    if (text_len == 0 || text_len >= MAX_TEXT) return -1;
    /* Estimate duration: ~150 words/min = ~750 chars/min = ~12.5 chars/sec */
    int duration = text_len * 1000 / 12; /* ms */
    int speed_factor = global_config.speed > 0 ? global_config.speed : 100;
    duration = duration * 100 / speed_factor;
    resp->duration_ms = duration;
    resp->characters = text_len;
    my_strncpy(resp->voice_used, global_config.voice, MAX_VOICE - 1);
    my_strncpy(resp->format, format_name(global_config.response_format), MAX_FORMAT - 1);
    /* Simulate audio data */
    int audio_len = text_len * 2; /* simulated */
    if (audio_len > MAX_AUDIO) audio_len = MAX_AUDIO;
    for (int i = 0; i < audio_len; i++) {
        resp->audio_data[i] = text[i % text_len];
    }
    resp->audio_data[audio_len] = '\0';
    resp->audio_len = audio_len;
    /* Cost: $0.015/1K chars for tts-1, $0.03/1K for tts-1-hd */
    int is_hd = my_strstr(global_config.model, "hd") ? 1 : 0;
    resp->cost_cents = (text_len * (is_hd ? 3 : 15)) / 1000;
    if (resp->cost_cents < 1) resp->cost_cents = 1;
    total_chars += text_len;
    return duration;
}

/* List available voices */
int tts_list_voices(void) {
    print_str("  Available Voices\n");
    print_str("  ==========================================================\n");
    for (int i = 0; i < MAX_VOICES; i++) {
        print_str("  "); print_str(voice_infos[i].name);
        print_str(" ("); print_str(voice_infos[i].gender);
        print_str(", "); print_str(voice_infos[i].accent);
        print_str(", "); print_str(voice_infos[i].style);
        print_str(")\n");
    }
    return MAX_VOICES;
}

/* Estimate duration */
int tts_estimate_duration(const char *text) {
    int text_len = my_strlen(text);
    int duration = text_len * 1000 / 12;
    int speed_factor = global_config.speed > 0 ? global_config.speed : 100;
    return duration * 100 / speed_factor;
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
    print_str("TTS API v39.0 - Text-to-Speech Synthesis\n");
    if (help) { print_str("Usage: tts_api [options]\n  -h, --help    Show help\n  -t, --test    Run test\n"); return; }
    if (test) {
        print_str("=== TTS API Test ===\n\n");
        tts_init("tts-1", "alloy", 100, 0);
        print_str("  Model: "); print_str(global_config.model); print_str("\n");
        print_str("  Voice: "); print_str(global_config.voice); print_str("\n");
        print_str("  Speed: "); print_int(global_config.speed); print_str("%\n");
        print_str("  Format: "); print_str(format_name(global_config.response_format)); print_str("\n\n");
        /* List voices */
        tts_list_voices();
        print_str("\n");
        /* Synthesize */
        const char *text = "Hello, this is a test of the text to speech synthesis system. It demonstrates how the TTS API converts written text into spoken audio.";
        int duration = tts_synthesize(text, &last_response);
        print_str("Synthesis result:\n");
        print_str("  Duration: "); print_int(duration); print_str("ms\n");
        print_str("  Characters: "); print_int(last_response.characters); print_str("\n");
        print_str("  Voice used: "); print_str(last_response.voice_used); print_str("\n");
        print_str("  Format: "); print_str(last_response.format); print_str("\n");
        print_str("  Audio length: "); print_int(last_response.audio_len); print_str("\n");
        print_str("  Cost: $0."); print_int(last_response.cost_cents); print_str("\n");
        /* Estimate */
        const char *short_text = "Quick test.";
        int est = tts_estimate_duration(short_text);
        print_str("\nEstimate for '"); print_str(short_text); print_str("': ");
        print_int(est); print_str("ms\n");
        print_str("\n=== TTS API Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
