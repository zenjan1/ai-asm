/* creative_engine: Creative generation engine (v17.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Creative types */
#define CREATIVE_TEXT     0
#define CREATIVE_CODE     1
#define CREATIVE_DESIGN   2
#define CREATIVE_SOLUTION 3
#define CREATIVE_ART      4

/* Limits */
#define MAX_OUTPUTS      32
#define MAX_TECHNIQUES   16
#define PROMPT_LEN       256
#define OUTPUT_LEN       2048
#define INSPIRATION_LEN  128
#define TECH_NAME_LEN    64
#define TECH_DESC_LEN    256
#define TECH_USE_LEN     256
#define IDEA_LEN         256
#define DOMAIN_LEN       128

/* Creative output */
typedef struct {
    int   creative_id;
    int   type;
    char  prompt[PROMPT_LEN];
    char  output[OUTPUT_LEN];
    int   novelty_score;
    int   usefulness_score;
    int   surprise_score;
    char  inspiration[INSPIRATION_LEN];
} creative_output_t;

/* Creative technique */
typedef struct {
    int   technique_id;
    char  name[TECH_NAME_LEN];
    char  description[TECH_DESC_LEN];
    int   effectiveness;
    char  use_cases[TECH_USE_LEN];
} creative_technique_t;

static creative_output_t outputs[MAX_OUTPUTS];
static creative_technique_t techniques[MAX_TECHNIQUES];
static int output_count = 0;
static int technique_count = 0;
static int next_creative_id = 1;

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

/* Get creative type name */
static const char *creative_type_name(int type) {
    if (type == CREATIVE_TEXT) return "text";
    if (type == CREATIVE_CODE) return "code";
    if (type == CREATIVE_DESIGN) return "design";
    if (type == CREATIVE_SOLUTION) return "solution";
    if (type == CREATIVE_ART) return "art";
    return "unknown";
}

/* Generate creative output */
int creative_generate(int type, const char *prompt) {
    if (!prompt || output_count >= MAX_OUTPUTS) return -1;
    int idx = output_count;
    outputs[idx].creative_id = next_creative_id;
    outputs[idx].type = type;
    my_strncpy(outputs[idx].prompt, prompt, PROMPT_LEN - 1);
    outputs[idx].novelty_score = 60 + (idx % 30);
    outputs[idx].usefulness_score = 50 + (idx % 40);
    outputs[idx].surprise_score = 40 + (idx % 50);

    /* Simulated generation */
    if (type == CREATIVE_TEXT) my_strcpy(outputs[idx].output, "A tale of innovation and discovery in the digital frontier.");
    else if (type == CREATIVE_CODE) my_strcpy(outputs[idx].output, "def solve(): return creative_approach()");
    else if (type == CREATIVE_DESIGN) my_strcpy(outputs[idx].output, "Minimalist UI with fluid animations and intuitive navigation.");
    else if (type == CREATIVE_SOLUTION) my_strcpy(outputs[idx].output, "Use edge computing to reduce latency by 80%.");
    else if (type == CREATIVE_ART) my_strcpy(outputs[idx].output, "Abstract composition blending technology and nature.");

    my_strcpy(outputs[idx].inspiration, "cross-domain synthesis");
    output_count++;
    int id = next_creative_id++;

    print_str("[creative] generated: type=");
    print_str(creative_type_name(type));
    print_str(" prompt=");
    print_str(prompt);
    print_str("\n  output: ");
    print_str(outputs[idx].output);
    print_str("\n  novelty=");
    print_int(outputs[idx].novelty_score);
    print_str(" usefulness=");
    print_int(outputs[idx].usefulness_score);
    print_str(" surprise=");
    print_int(outputs[idx].surprise_score);
    print_str("\n");
    return id;
}

/* Combine ideas */
int creative_combine_ideas(const char *idea1, const char *idea2) {
    if (!idea1 || !idea2) return -1;
    print_str("[creative] combine: ");
    print_str(idea1);
    print_str(" + ");
    print_str(idea2);
    print_str("\n");
    print_str("  Combined: ");
    print_str(idea1);
    print_str(" integrated with ");
    print_str(idea2);
    print_str("\n");
    print_str("  Novelty: 75 Synergy: high\n");
    return 0;
}

/* Breakthrough thinking */
int creative_breakthrough_thinking(const char *domain) {
    if (!domain) return -1;
    print_str("[creative] breakthrough: domain=");
    print_str(domain);
    print_str("\n");
    print_str("  Step 1: Challenge existing assumptions\n");
    print_str("  Step 2: Look for contradictions\n");
    print_str("  Step 3: Apply lateral thinking\n");
    print_str("  Step 4: Generate radical alternatives\n");
    print_str("  Breakthrough idea generated\n");
    return 0;
}

/* Assess novelty */
int creative_assess_novelty(const char *output_str) {
    if (!output_str) return -1;
    int score = 50;
    /* Simulated assessment */
    if (my_strncmp(output_str, "A tale", 6) == 0) score = 72;
    else if (my_strncmp(output_str, "def solve", 9) == 0) score = 65;
    else score = 55;
    print_str("[creative] novelty score: ");
    print_int(score);
    print_str("/100\n");
    return score;
}

/* Iterate and improve */
int creative_iterate_improve(int creative_id) {
    for (int i = 0; i < output_count; i++) {
        if (outputs[i].creative_id == creative_id) {
            outputs[i].novelty_score += 5;
            outputs[i].usefulness_score += 3;
            if (outputs[i].novelty_score > 100) outputs[i].novelty_score = 100;
            if (outputs[i].usefulness_score > 100) outputs[i].usefulness_score = 100;
            print_str("[creative] improved creative ");
            print_int(creative_id);
            print_str(": novelty=");
            print_int(outputs[i].novelty_score);
            print_str(" usefulness=");
            print_int(outputs[i].usefulness_score);
            print_str("\n");
            return 0;
        }
    }
    return -1;
}

/* Get techniques */
int creative_get_techniques(void) {
    print_str("=== Creative Techniques ===\n");
    for (int i = 0; i < technique_count; i++) {
        print_str("  ");
        print_str(techniques[i].name);
        print_str(": ");
        print_str(techniques[i].description);
        print_str(" (eff=");
        print_int(techniques[i].effectiveness);
        print_str("%)\n");
    }
    return technique_count;
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
    print_str("Creative Engine v17.0 - Creative AI Generation\n");
    if (help) {
        print_str("Usage: creative_engine [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run creative test\n");
        return;
    }
    if (test) {
        print_str("=== Creative Engine Test ===\n");
        print_str("\n--- Register Techniques ---\n");
        techniques[0].technique_id = 1; my_strcpy(techniques[0].name, "brainstorming");
        my_strcpy(techniques[0].description, "Generate many ideas without filtering"); techniques[0].effectiveness = 80;
        my_strcpy(techniques[0].use_cases, "text,design,solution"); technique_count++;
        techniques[1].technique_id = 2; my_strcpy(techniques[1].name, "lateral_thinking");
        my_strcpy(techniques[1].description, "Approach from unexpected angles"); techniques[1].effectiveness = 85;
        my_strcpy(techniques[1].use_cases, "solution,art,code"); technique_count++;
        techniques[2].technique_id = 3; my_strcpy(techniques[2].name, "analogy_mapping");
        my_strcpy(techniques[2].description, "Transfer patterns from other domains"); techniques[2].effectiveness = 75;
        my_strcpy(techniques[2].use_cases, "design,art,text"); technique_count++;

        print_str("\n--- Generate Creatives ---\n");
        creative_generate(CREATIVE_TEXT, "write a story about AI");
        creative_generate(CREATIVE_CODE, "generate elegant sorting algorithm");
        creative_generate(CREATIVE_SOLUTION, "reduce energy consumption in data center");
        creative_generate(CREATIVE_ART, "abstract digital art");

        print_str("\n--- Combine Ideas ---\n");
        creative_combine_ideas("neural networks", "music composition");

        print_str("\n--- Breakthrough Thinking ---\n");
        creative_breakthrough_thinking("software engineering");

        print_str("\n--- Assess Novelty ---\n");
        creative_assess_novelty("A tale of innovation and discovery in the digital frontier.");

        print_str("\n--- Iterate Improvement ---\n");
        creative_iterate_improve(1);

        print_str("\n--- Techniques ---\n");
        creative_get_techniques();

        print_str("\n=== Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
