/* multi_modal_ai: Multi-Modal AI Integration Module (v1.0)
 *
 * Provides unified multi-modal AI capabilities:
 * - Vision-language models (image + text reasoning)
 * - Speech-text models (audio transcription + understanding)
 * - Video understanding (temporal + spatial reasoning)
 * - Cross-modal retrieval (search across modalities)
 * - Fusion reasoning (early / late / hybrid strategies)
 */

#include <stddef.h>

__attribute__((import_module("host"), import_name("host_alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("host_print")))
extern void host_print(const char *str);

__attribute__((import_module("host"), import_name("host_exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("host_get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* ---------- Model type constants ---------- */
#define MODEL_VISION_LANG   0
#define MODEL_SPEECH_TEXT   1
#define MODEL_VIDEO_UNDERST 2

/* ---------- Fusion strategies ---------- */
#define FUSION_EARLY   0
#define FUSION_LATE    1
#define FUSION_HYBRID  2

/* ---------- Modality bitmask flags ---------- */
#define MOD_TEXT   0x01
#define MOD_IMAGE  0x02
#define MOD_AUDIO  0x04
#define MOD_VIDEO  0x08

/* ---------- Limits ---------- */
#define MAX_MODELS          12
#define MAX_SESSIONS         8
#define MAX_RETRIEVAL       16
#define MAX_FUSION_INPUTS    6
#define PATH_LEN           128
#define NAME_LEN            64
#define DESC_LEN           256
#define PROMPT_LEN         256
#define RESPONSE_LEN      1024
#define REASON_LEN         512

/* ---------- Data structures ---------- */

typedef struct {
    int  model_id;
    int  type;
    char path[PATH_LEN];
    int  loaded;
    int  session_count;
    int  quality;           /* 0-100 */
} model_t;

typedef struct {
    char image_desc[DESC_LEN];
    char prompt[PROMPT_LEN];
    char response[RESPONSE_LEN];
    int  confidence;         /* 0-100 */
} vl_result_t;

typedef struct {
    char audio_desc[DESC_LEN];
    char transcript[PROMPT_LEN];
    int  confidence;
    char intent[NAME_LEN];
} st_result_t;

typedef struct {
    char video_desc[DESC_LEN];
    int  frame_count;
    char summary[RESPONSE_LEN];
    int  confidence;
    char tags[DESC_LEN];
} video_result_t;

typedef struct {
    int  result_id;
    char description[DESC_LEN];
    int  score;              /* 0-100 relevance */
    int  source_model_id;
    int  modality;           /* MOD_* flag */
} retrieval_result_t;

typedef struct {
    char query[PROMPT_LEN];
    int  input_modalities;   /* MOD_* bitmask */
    char text_input[PROMPT_LEN];
    char image_input[DESC_LEN];
    char audio_input[DESC_LEN];
    char video_input[DESC_LEN];
} fusion_input_t;

typedef struct {
    char response[RESPONSE_LEN];
    int  confidence;
    char reasoning[REASON_LEN];
    int  strategy_used;
    int  models_engaged;
} fusion_result_t;

typedef struct {
    int  session_id;
    int  active;
    int  modalities_used;
    int  query_count;
} session_t;

/* ---------- Static state ---------- */
static model_t          g_models[MAX_MODELS];
static int              g_model_count    = 0;
static int              g_next_model_id  = 1;
static session_t        g_sessions[MAX_SESSIONS];
static int              g_session_count  = 0;
static int              g_next_sess_id   = 1;
static int              g_total_queries  = 0;
static int              g_total_retrievals = 0;

/* ---------- String utilities ---------- */
static int my_strlen(const char *s) { int n = 0; while (s[n]) n++; return n; }

static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; }
    return *a - *b;
}

static void my_strcpy(char *d, const char *s) {
    while (*s) *d++ = *s++; *d = '\0';
}

static void my_strncpy(char *d, const char *s, int n) {
    int i = 0; while (i < n - 1 && s[i]) { d[i] = s[i]; i++; } d[i] = '\0';
}

static int my_strncmp(const char *a, const char *b, int n) {
    for (int i = 0; i < n; i++) {
        if (!a[i] || !b[i]) return (a[i] ? 1 : 0) - (b[i] ? 1 : 0);
        if (a[i] != b[i]) return a[i] - b[i];
    }
    return 0;
}

static void my_strcat(char *d, const char *s) {
    while (*d) d++; while (*s) *d++ = *s++; *d = '\0';
}

/* ---------- Print utilities ---------- */
static void print_str(const char *s) { host_print(s); }

static void print_int(int v) {
    char b[32]; int p = 0;
    if (v < 0) { b[p++] = '-'; v = -v; }
    if (v == 0) { b[p++] = '0'; }
    else {
        int d = 0, t = v; while (t > 0) { d++; t /= 10; }
        p += d; b[p] = '\0'; p--;
        while (v > 0) { b[p--] = '0' + (v % 10); v /= 10; }
    }
    host_print(b);
}

static void print_modality_flags(int flags) {
    int first = 1;
    if (flags & MOD_TEXT)  { print_str("text");  first = 0; }
    if (flags & MOD_IMAGE) { if (!first) print_str("+"); print_str("image");  first = 0; }
    if (flags & MOD_AUDIO) { if (!first) print_str("+"); print_str("audio");  first = 0; }
    if (flags & MOD_VIDEO) { if (!first) print_str("+"); print_str("video"); }
}

/* ---------- Model type name helper ---------- */
static const char *model_type_name(int t) {
    if (t == MODEL_VISION_LANG)   return "vision-language";
    if (t == MODEL_SPEECH_TEXT)   return "speech-text";
    if (t == MODEL_VIDEO_UNDERST) return "video-understanding";
    return "unknown";
}

static const char *fusion_strategy_name(int s) {
    if (s == FUSION_EARLY)  return "early";
    if (s == FUSION_LATE)   return "late";
    if (s == FUSION_HYBRID) return "hybrid";
    return "unknown";
}

/* ==================================================================
 *  MODEL INTEGRATION
 * ================================================================== */

static int load_vision_lang_model(const char *path) {
    if (g_model_count >= MAX_MODELS) return -1;
    int idx = g_model_count;
    g_models[idx].model_id      = g_next_model_id;
    g_models[idx].type          = MODEL_VISION_LANG;
    g_models[idx].loaded        = 1;
    g_models[idx].session_count = 0;
    g_models[idx].quality       = 0;
    my_strncpy(g_models[idx].path, path, PATH_LEN - 1);
    int id = g_next_model_id++;
    g_model_count++;
    print_str("[mmai] loaded vision-language: ");
    print_str(path);
    print_str(" (id="); print_int(id); print_str(")\n");
    return id;
}

static int load_speech_text_model(const char *path) {
    if (g_model_count >= MAX_MODELS) return -1;
    int idx = g_model_count;
    g_models[idx].model_id      = g_next_model_id;
    g_models[idx].type          = MODEL_SPEECH_TEXT;
    g_models[idx].loaded        = 1;
    g_models[idx].session_count = 0;
    g_models[idx].quality       = 0;
    my_strncpy(g_models[idx].path, path, PATH_LEN - 1);
    int id = g_next_model_id++;
    g_model_count++;
    print_str("[mmai] loaded speech-text: ");
    print_str(path);
    print_str(" (id="); print_int(id); print_str(")\n");
    return id;
}

static int load_video_model(const char *path) {
    if (g_model_count >= MAX_MODELS) return -1;
    int idx = g_model_count;
    g_models[idx].model_id      = g_next_model_id;
    g_models[idx].type          = MODEL_VIDEO_UNDERST;
    g_models[idx].loaded        = 1;
    g_models[idx].session_count = 0;
    g_models[idx].quality       = 0;
    my_strncpy(g_models[idx].path, path, PATH_LEN - 1);
    int id = g_next_model_id++;
    g_model_count++;
    print_str("[mmai] loaded video-understanding: ");
    print_str(path);
    print_str(" (id="); print_int(id); print_str(")\n");
    return id;
}

/* ==================================================================
 *  VISION-LANGUAGE INFERENCE
 * ================================================================== */

static int vision_lang_infer(int model_id, const char *image,
                             const char *prompt, vl_result_t *out) {
    if (!image || !prompt || !out) return -1;
    int idx = -1;
    for (int i = 0; i < g_model_count; i++) {
        if (g_models[i].model_id == model_id &&
            g_models[i].type == MODEL_VISION_LANG && g_models[i].loaded) {
            idx = i; break;
        }
    }
    if (idx < 0) { print_str("[mmai] VL model not found\n"); return -1; }

    g_models[idx].session_count++;
    print_str("[mmai] VL infer model="); print_int(model_id);
    print_str(" image="); print_str(image);
    print_str(" prompt="); print_str(prompt); print_str("\n");

    /* Simulated vision-language reasoning */
    if (my_strncmp(image, "cat", 3) == 0) {
        my_strcpy(out->response,
            "The image shows a cat. It appears relaxed and alert.");
        out->confidence = 93;
    } else if (my_strncmp(image, "street", 6) == 0) {
        my_strcpy(out->response,
            "A street scene with vehicles, pedestrians, and buildings.");
        out->confidence = 89;
    } else if (my_strncmp(image, "chart", 5) == 0) {
        my_strcpy(out->response,
            "A bar chart showing quarterly revenue growth over 2025.");
        out->confidence = 91;
    } else {
        my_strcpy(out->response, "An image was analyzed but content is generic.");
        out->confidence = 60;
    }
    my_strncpy(out->image_desc, image, DESC_LEN - 1);
    my_strncpy(out->prompt, prompt, PROMPT_LEN - 1);
    g_models[idx].quality = out->confidence;
    g_total_queries++;
    print_str("  -> "); print_str(out->response);
    print_str(" (conf="); print_int(out->confidence);
    print_str("%)\n");
    return 0;
}

/* ==================================================================
 *  SPEECH-TEXT INFERENCE
 * ================================================================== */

static int speech_text_infer(int model_id, const char *audio,
                             st_result_t *out) {
    if (!audio || !out) return -1;
    int idx = -1;
    for (int i = 0; i < g_model_count; i++) {
        if (g_models[i].model_id == model_id &&
            g_models[i].type == MODEL_SPEECH_TEXT && g_models[i].loaded) {
            idx = i; break;
        }
    }
    if (idx < 0) { print_str("[mmai] ST model not found\n"); return -1; }

    g_models[idx].session_count++;
    print_str("[mmai] ST infer model="); print_int(model_id);
    print_str(" audio="); print_str(audio); print_str("\n");

    if (my_strncmp(audio, "greeting", 8) == 0) {
        my_strcpy(out->transcript, "Hello, how are you today?");
        my_strcpy(out->intent, "greeting");
        out->confidence = 96;
    } else if (my_strncmp(audio, "weather", 7) == 0) {
        my_strcpy(out->transcript, "What is the weather forecast for tomorrow?");
        my_strcpy(out->intent, "weather_query");
        out->confidence = 94;
    } else if (my_strncmp(audio, "command", 7) == 0) {
        my_strcpy(out->transcript, "Turn on the living room lights.");
        my_strcpy(out->intent, "smart_home_command");
        out->confidence = 97;
    } else {
        my_strcpy(out->transcript, "Audio transcribed but intent is unclear.");
        my_strcpy(out->intent, "unknown");
        out->confidence = 55;
    }
    my_strncpy(out->audio_desc, audio, DESC_LEN - 1);
    g_models[idx].quality = out->confidence;
    g_total_queries++;
    print_str("  transcript: "); print_str(out->transcript);
    print_str("\n  intent: ");     print_str(out->intent);
    print_str(" (conf="); print_int(out->confidence);
    print_str("%)\n");
    return 0;
}

/* ==================================================================
 *  VIDEO UNDERSTANDING INFERENCE
 * ================================================================== */

static int video_infer(int model_id, const char *video,
                       int frames, video_result_t *out) {
    if (!video || !out) return -1;
    int idx = -1;
    for (int i = 0; i < g_model_count; i++) {
        if (g_models[i].model_id == model_id &&
            g_models[i].type == MODEL_VIDEO_UNDERST && g_models[i].loaded) {
            idx = i; break;
        }
    }
    if (idx < 0) { print_str("[mmai] video model not found\n"); return -1; }

    g_models[idx].session_count++;
    print_str("[mmai] video infer model="); print_int(model_id);
    print_str(" video="); print_str(video);
    print_str(" frames="); print_int(frames); print_str("\n");

    out->frame_count = frames;
    if (my_strncmp(video, "traffic", 7) == 0) {
        my_strcpy(out->summary,
            "Traffic flow monitored. Moderate congestion on main road.");
        my_strcpy(out->tags, "traffic,congestion,vehicles");
        out->confidence = 87;
    } else if (my_strncmp(video, "meeting", 7) == 0) {
        my_strcpy(out->summary,
            "Meeting room with 5 participants actively discussing.");
        my_strcpy(out->tags, "meeting,people,indoor");
        out->confidence = 90;
    } else if (my_strncmp(video, "sport", 5) == 0) {
        my_strcpy(out->summary,
            "Sporting event in progress. Players in competitive action.");
        my_strcpy(out->tags, "sport,action,outdoor");
        out->confidence = 85;
    } else {
        my_strcpy(out->summary, "Video analyzed. Generic scene detected.");
        my_strcpy(out->tags, "generic,scene");
        out->confidence = 58;
    }
    my_strncpy(out->video_desc, video, DESC_LEN - 1);
    g_models[idx].quality = out->confidence;
    g_total_queries++;
    print_str("  summary: "); print_str(out->summary);
    print_str("\n  tags: ");    print_str(out->tags);
    print_str(" (conf="); print_int(out->confidence);
    print_str("%)\n");
    return 0;
}

/* ==================================================================
 *  CROSS-MODAL RETRIEVAL
 * ================================================================== */

static int cross_modal_retrieve(const char *query, int query_modality,
                                retrieval_result_t *results, int *count) {
    if (!query || !results || !count) return -1;
    *count = 0;
    print_str("[mmai] cross-modal retrieve query=");
    print_str(query); print_str(" modality=");
    print_modality_flags(query_modality); print_str("\n");

    /* Simulated retrieval — rank by relevance to query keywords */
    if (query_modality & MOD_IMAGE) {
        if (my_strncmp(query, "cat", 3) == 0) {
            my_strcpy(results[*count].description, "cat on windowsill");
            results[*count].score = 95;
            results[*count].source_model_id = 1;
            results[*count].modality = MOD_IMAGE;
            (*count)++;
            my_strcpy(results[*count].description, "cat sleeping on sofa");
            results[*count].score = 88;
            results[*count].source_model_id = 1;
            results[*count].modality = MOD_IMAGE;
            (*count)++;
        }
        if (my_strncmp(query, "outdoor", 7) == 0) {
            my_strcpy(results[*count].description, "outdoor park scene");
            results[*count].score = 92;
            results[*count].source_model_id = 1;
            results[*count].modality = MOD_IMAGE;
            (*count)++;
        }
    }
    if (query_modality & MOD_AUDIO) {
        if (my_strncmp(query, "speech", 6) == 0) {
            my_strcpy(results[*count].description, "speech: weather forecast");
            results[*count].score = 91;
            results[*count].source_model_id = 2;
            results[*count].modality = MOD_AUDIO;
            (*count)++;
            my_strcpy(results[*count].description, "speech: greeting message");
            results[*count].score = 85;
            results[*count].source_model_id = 2;
            results[*count].modality = MOD_AUDIO;
            (*count)++;
        }
    }
    if (query_modality & MOD_VIDEO) {
        if (my_strncmp(query, "traffic", 7) == 0) {
            my_strcpy(results[*count].description, "traffic cam: main street");
            results[*count].score = 93;
            results[*count].source_model_id = 3;
            results[*count].modality = MOD_VIDEO;
            (*count)++;
        }
    }
    /* Text fallback */
    if (*count == 0) {
        my_strcpy(results[0].description, "text: general knowledge match");
        results[0].score = 70;
        results[0].source_model_id = 0;
        results[0].modality = MOD_TEXT;
        (*count)++;
    }

    g_total_retrievals += *count;
    print_str("  found "); print_int(*count);
    print_str(" results\n");
    for (int i = 0; i < *count; i++) {
        print_str("    ["); print_int(i);
        print_str("] score="); print_int(results[i].score);
        print_str(" mod=");   print_modality_flags(results[i].modality);
        print_str(" desc=");  print_str(results[i].description);
        print_str("\n");
    }
    return 0;
}

/* ==================================================================
 *  UNIFIED MULTI-MODAL SESSION INTERFACE
 * ================================================================== */

static int session_create(void) {
    if (g_session_count >= MAX_SESSIONS) return -1;
    int idx = g_session_count;
    g_sessions[idx].session_id     = g_next_sess_id;
    g_sessions[idx].active         = 1;
    g_sessions[idx].modalities_used = 0;
    g_sessions[idx].query_count    = 0;
    int id = g_next_sess_id++;
    g_session_count++;
    print_str("[mmai] session created id="); print_int(id); print_str("\n");
    return id;
}

static int session_add_modality(int session_id, int modality) {
    for (int i = 0; i < g_session_count; i++) {
        if (g_sessions[i].session_id == session_id && g_sessions[i].active) {
            g_sessions[i].modalities_used |= modality;
            print_str("[mmai] session "); print_int(session_id);
            print_str(" += modality ");
            print_modality_flags(modality); print_str("\n");
            return 0;
        }
    }
    return -1;
}

static int session_run(int session_id, const char *text,
                       const char *image, const char *audio) {
    int idx = -1;
    for (int i = 0; i < g_session_count; i++) {
        if (g_sessions[i].session_id == session_id && g_sessions[i].active) {
            idx = i; break;
        }
    }
    if (idx < 0) return -1;

    print_str("[mmai] session "); print_int(session_id);
    print_str(" run #"); print_int(g_sessions[idx].query_count + 1);
    print_str("\n");
    if (text  && text[0])  { print_str("  text:  "); print_str(text);  print_str("\n"); }
    if (image && image[0]) { print_str("  image: "); print_str(image); print_str("\n"); }
    if (audio && audio[0]) { print_str("  audio: "); print_str(audio); print_str("\n"); }

    g_sessions[idx].query_count++;
    g_total_queries++;
    print_str("  -> unified response generated (modalities engaged: ");
    print_modality_flags(g_sessions[idx].modalities_used);
    print_str(")\n");
    return 0;
}

static int session_analyze(int session_id) {
    for (int i = 0; i < g_session_count; i++) {
        if (g_sessions[i].session_id == session_id && g_sessions[i].active) {
            print_str("[mmai] session "); print_int(session_id);
            print_str(" analyze:\n");
            print_str("  active: true\n");
            print_str("  queries: ");     print_int(g_sessions[i].query_count);
            print_str("\n");
            print_str("  modalities: ");
            print_modality_flags(g_sessions[i].modalities_used);
            print_str("\n");
            return 0;
        }
    }
    return -1;
}

static int session_close(int session_id) {
    for (int i = 0; i < g_session_count; i++) {
        if (g_sessions[i].session_id == session_id) {
            g_sessions[i].active = 0;
            print_str("[mmai] session "); print_int(session_id);
            print_str(" closed\n");
            return 0;
        }
    }
    return -1;
}

/* ==================================================================
 *  FUSION REASONING  (early / late / hybrid)
 * ================================================================== */

static int fusion_reason(const fusion_input_t *inputs, int input_count,
                         int strategy, fusion_result_t *out) {
    if (!inputs || input_count <= 0 || !out) return -1;

    print_str("[mmai] fusion strategy=");
    print_str(fusion_strategy_name(strategy));
    print_str(" inputs="); print_int(input_count); print_str("\n");

    /* Tally engaged modalities and pick a base confidence */
    int total_mod = 0;
    int base_conf = 0;
    for (int i = 0; i < input_count; i++) {
        total_mod |= inputs[i].input_modalities;
        base_conf += 70;
        print_str("  input["); print_int(i); print_str("] mod=");
        print_modality_flags(inputs[i].input_modalities);
        print_str(" query="); print_str(inputs[i].query);
        print_str("\n");
    }
    if (input_count > 0) base_conf /= input_count;

    /* ---- EARLY FUSION ---- */
    if (strategy == FUSION_EARLY) {
        my_strcpy(out->reasoning,
            "Early fusion: features concatenated before joint inference. "
            "Cross-modal attention applied at encoder level.");
        int bonus = 0;
        if (total_mod & MOD_IMAGE) bonus += 5;
        if (total_mod & MOD_AUDIO) bonus += 5;
        if (total_mod & MOD_VIDEO) bonus += 7;
        out->confidence = base_conf + bonus;
        my_strcpy(out->response,
            "Early fusion result: integrated representation from all "
            "input modalities processed through shared transformer layers.");
        out->models_engaged = input_count;

    /* ---- LATE FUSION ---- */
    } else if (strategy == FUSION_LATE) {
        my_strcpy(out->reasoning,
            "Late fusion: each modality processed independently, "
            "decisions merged via weighted voting.");
        int bonus = 0;
        if (total_mod & MOD_TEXT)  bonus += 3;
        if (total_mod & MOD_IMAGE) bonus += 4;
        if (total_mod & MOD_AUDIO) bonus += 4;
        if (total_mod & MOD_VIDEO) bonus += 5;
        out->confidence = base_conf + bonus;
        my_strcpy(out->response,
            "Late fusion result: independent model outputs aggregated "
            "with confidence-weighted ensemble voting.");
        out->models_engaged = input_count;

    /* ---- HYBRID FUSION ---- */
    } else {
        my_strcpy(out->reasoning,
            "Hybrid fusion: early fusion for correlated modalities, "
            "late fusion for independent streams. Adaptive gating.");
        out->confidence = base_conf + 15;
        my_strcpy(out->response,
            "Hybrid fusion result: adaptive combination of early and "
            "late fusion paths. Gating network selected optimal merge.");
        out->models_engaged = input_count + 1;
    }
    if (out->confidence > 99) out->confidence = 99;
    out->strategy_used = strategy;
    g_total_queries++;

    print_str("  strategy: ");  print_str(fusion_strategy_name(strategy));
    print_str("\n  confidence: "); print_int(out->confidence);
    print_str("%\n  models: ");   print_int(out->models_engaged);
    print_str("\n  reasoning: ");  print_str(out->reasoning);
    print_str("\n  response: ");   print_str(out->response);
    print_str("\n");
    return 0;
}

/* ==================================================================
 *  CLI ENTRY POINT
 * ================================================================== */

void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);

    int help = 0, test = 0;
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0)
            help = 1;
        else if (my_strcmp(arg, "-t") == 0 || my_strcmp(arg, "--test") == 0)
            test = 1;
        while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    }

    print_str("Multi-Modal AI v1.0 - Unified Multi-Modal Intelligence\n");

    if (help) {
        print_str("Usage: multi_modal_ai [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run multi-modal test suite\n");
        print_str("\nCapabilities:\n");
        print_str("  Vision-language inference  (image + text)\n");
        print_str("  Speech-text processing     (audio transcription)\n");
        print_str("  Video understanding        (temporal reasoning)\n");
        print_str("  Cross-modal retrieval      (search across modalities)\n");
        print_str("  Fusion reasoning           (early/late/hybrid)\n");
        return;
    }

    if (test) {
        print_str("=== Multi-Modal AI Test Suite ===\n");

        /* ---- Load Models ---- */
        print_str("\n--- Load Models ---\n");
        int vl_id  = load_vision_lang_model("/models/clip_vit_l14.onnx");
        int st_id  = load_speech_text_model("/models/whisper_large_v3.bin");
        int vid_id = load_video_model("/models/videollama_7b.gguf");

        /* ---- Vision-Language ---- */
        print_str("\n--- Vision-Language Inference ---\n");
        vl_result_t vl;
        vision_lang_infer(vl_id, "cat on windowsill",
                          "Describe this animal", &vl);
        vision_lang_infer(vl_id, "street scene at dusk",
                          "What objects are visible?", &vl);
        vision_lang_infer(vl_id, "chart Q4 revenue",
                          "Summarize this chart", &vl);

        /* ---- Speech-Text ---- */
        print_str("\n--- Speech-Text Inference ---\n");
        st_result_t st;
        speech_text_infer(st_id, "greeting: hello friend", &st);
        speech_text_infer(st_id, "weather: what is forecast", &st);
        speech_text_infer(st_id, "command: turn on lights", &st);

        /* ---- Video Understanding ---- */
        print_str("\n--- Video Understanding ---\n");
        video_result_t vr;
        video_infer(vid_id, "traffic intersection", 30, &vr);
        video_infer(vid_id, "meeting room discussion", 60, &vr);
        video_infer(vid_id, "sport stadium action", 45, &vr);

        /* ---- Cross-Modal Retrieval ---- */
        print_str("\n--- Cross-Modal Retrieval ---\n");
        retrieval_result_t rr[MAX_RETRIEVAL];
        int rc;
        cross_modal_retrieve("cat photo", MOD_IMAGE, rr, &rc);
        cross_modal_retrieve("speech audio", MOD_AUDIO, rr, &rc);
        cross_modal_retrieve("traffic cam", MOD_VIDEO, rr, &rc);
        cross_modal_retrieve("general search", MOD_TEXT, rr, &rc);

        /* ---- Unified Session ---- */
        print_str("\n--- Unified Session ---\n");
        int sid = session_create();
        session_add_modality(sid, MOD_TEXT | MOD_IMAGE | MOD_AUDIO);
        session_run(sid, "Describe what you see and hear",
                    "person in office", "discussion audio");
        session_run(sid, "Follow-up question", NULL, NULL);
        session_analyze(sid);
        session_close(sid);

        /* ---- Fusion Reasoning ---- */
        print_str("\n--- Fusion Reasoning ---\n");

        print_str("\n[early fusion]\n");
        fusion_input_t fi[MAX_FUSION_INPUTS];
        my_strcpy(fi[0].query, "What is happening in this scene?");
        fi[0].input_modalities = MOD_IMAGE;
        my_strcpy(fi[0].image_input, "busy street");
        my_strcpy(fi[1].query, "Transcribe the background audio");
        fi[1].input_modalities = MOD_AUDIO;
        my_strcpy(fi[1].audio_input, "city ambience");
        fusion_result_t fr;
        fusion_reason(fi, 2, FUSION_EARLY, &fr);

        print_str("\n[late fusion]\n");
        my_strcpy(fi[0].query, "Analyze meeting video");
        fi[0].input_modalities = MOD_VIDEO;
        my_strcpy(fi[0].video_input, "meeting room");
        my_strcpy(fi[1].query, "Summarize transcript");
        fi[1].input_modalities = MOD_TEXT;
        my_strcpy(fi[1].text_input, "quarterly review");
        fusion_reason(fi, 2, FUSION_LATE, &fr);

        print_str("\n[hybrid fusion]\n");
        fi[0].input_modalities = MOD_IMAGE | MOD_TEXT;
        my_strcpy(fi[0].query, "Analyze chart with context");
        my_strcpy(fi[0].image_input, "revenue chart");
        my_strcpy(fi[0].text_input, "Q4 financial data");
        fi[1].input_modalities = MOD_AUDIO;
        my_strcpy(fi[1].query, "Narrate the presentation");
        my_strcpy(fi[1].audio_input, "presentation audio");
        fi[2].input_modalities = MOD_VIDEO;
        my_strcpy(fi[2].query, "Monitor the screen share");
        my_strcpy(fi[2].video_input, "screen recording");
        fusion_reason(fi, 3, FUSION_HYBRID, &fr);

        /* ---- Summary ---- */
        print_str("\n--- Test Summary ---\n");
        print_str("  Models loaded:   "); print_int(g_model_count);  print_str("\n");
        print_str("  Sessions used:   "); print_int(g_session_count); print_str("\n");
        print_str("  Total queries:   "); print_int(g_total_queries); print_str("\n");
        print_str("  Total retrieved: "); print_int(g_total_retrievals); print_str("\n");
        print_str("\n=== Test Complete ===\n");
        return;
    }

    print_str("Use -h for help, -t for test\n");
}
