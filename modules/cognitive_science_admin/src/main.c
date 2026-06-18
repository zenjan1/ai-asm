/* cognitive_science_admin: Cognitive science administration system (v1.0)
 * Perception, memory, language, decision making, social cognition
 */
#include <stddef.h>

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_PERCEPTION   16
#define MAX_MEMORY       14
#define MAX_LANGUAGE     12
#define MAX_DECISION     10
#define MAX_SOCIAL       10

typedef struct {
    int    percep_id;
    int    percep_type;
    int    percep_category;
    int    visual_cog;
    int    auditory_cog;
    int    tactile_cog;
    int    year;
    int    active;
} perception_t;

typedef struct {
    int    memory_id;
    int    memory_type;
    int    memory_category;
    int    short_term;
    int    long_term;
    int    working_mem;
    int    year;
    int    active;
} memory_t;

typedef struct {
    int    lang_id;
    int    lang_type;
    int    lang_category;
    int    comprehension;
    int    production;
    int    learning;
    int    year;
    int    active;
} language_t;

typedef struct {
    int    decision_id;
    int    decision_type;
    int    decision_category;
    int    risk_assess;
    int    reward_mech;
    int    choice_pref;
    int    year;
    int    active;
} decision_t;

typedef struct {
    int    social_id;
    int    social_type;
    int    social_category;
    int    emotion_cog;
    int    empathy;
    int    social_reason;
    int    year;
    int    active;
} social_t;

typedef struct {
    int    n_perception;
    int    n_memory;
    int    n_language;
    int    n_decision;
    int    n_social;
    int    total_visual;
    int    total_short;
    int    total_comprehend;
    int    total_risk;
    int    total_emotion;
} csa_state_t;

static perception_t perceptions[MAX_PERCEPTION];
static memory_t memories[MAX_MEMORY];
static language_t languages[MAX_LANGUAGE];
static decision_t decisions[MAX_DECISION];
static social_t socials[MAX_SOCIAL];
static csa_state_t csa;

static int initialized = 0;

static void print_str(const char* s) { host_print(s); }
static void print_int(int val) {
    char buf[32]; int i = 0;
    if (val < 0) { buf[i++] = '-'; val = -val; }
    if (val == 0) { buf[i++] = '0'; }
    else { int s = i; while (val > 0) { buf[i++] = '0' + (val % 10); val /= 10; }
           int e = i - 1; while (s < e) { char t = buf[s]; buf[s] = buf[e]; buf[e] = t; s++; e--; } }
    buf[i] = '\0'; host_print(buf);
}

int csa_init(void) {
    if (initialized) return -1;
    csa.n_perception = 0; csa.n_memory = 0; csa.n_language = 0;
    csa.n_decision = 0; csa.n_social = 0;
    csa.total_visual = 0; csa.total_short = 0;
    csa.total_comprehend = 0; csa.total_risk = 0;
    csa.total_emotion = 0;
    for (int i = 0; i < MAX_PERCEPTION; i++) perceptions[i].active = 0;
    for (int i = 0; i < MAX_MEMORY; i++) memories[i].active = 0;
    for (int i = 0; i < MAX_LANGUAGE; i++) languages[i].active = 0;
    for (int i = 0; i < MAX_DECISION; i++) decisions[i].active = 0;
    for (int i = 0; i < MAX_SOCIAL; i++) socials[i].active = 0;
    initialized = 1;
    print_str("[CSA] Cognitive science initialized\n");
    return 0;
}

int csa_perception(int pc_type, int cat, int visual, int auditory, int tactile, int year) {
    if (csa.n_perception >= MAX_PERCEPTION) return -1;
    perception_t* p = &perceptions[csa.n_perception];
    p->percep_id = csa.n_perception;
    p->percep_type = pc_type;
    p->percep_category = cat;
    p->visual_cog = visual;
    p->auditory_cog = auditory;
    p->tactile_cog = tactile;
    p->year = year;
    p->active = 1;
    csa.total_visual += visual;
    csa.n_perception++;
    print_str("[CSA] Perception "); print_int(csa.n_perception - 1);
    print_str(" type="); print_int(pc_type);
    print_str(" cat="); print_int(cat);
    print_str(" vis="); print_int(visual);
    print_str(" aud="); print_int(auditory);
    print_str(" tac="); print_int(tactile); print_str("\n");
    return csa.n_perception - 1;
}

int csa_memory(int mm_type, int cat, int short_t, int long_t, int work_m, int year) {
    if (csa.n_memory >= MAX_MEMORY) return -1;
    memory_t* m = &memories[csa.n_memory];
    m->memory_id = csa.n_memory;
    m->memory_type = mm_type;
    m->memory_category = cat;
    m->short_term = short_t;
    m->long_term = long_t;
    m->working_mem = work_m;
    m->year = year;
    m->active = 1;
    csa.total_short += short_t;
    csa.n_memory++;
    print_str("[CSA] Memory "); print_int(csa.n_memory - 1);
    print_str(" type="); print_int(mm_type);
    print_str(" cat="); print_int(cat);
    print_str(" sht="); print_int(short_t);
    print_str(" lng="); print_int(long_t);
    print_str(" wrk="); print_int(work_m); print_str("\n");
    return csa.n_memory - 1;
}

int csa_language(int lg_type, int cat, int compre, int prod, int learn, int year) {
    if (csa.n_language >= MAX_LANGUAGE) return -1;
    language_t* l = &languages[csa.n_language];
    l->lang_id = csa.n_language;
    l->lang_type = lg_type;
    l->lang_category = cat;
    l->comprehension = compre;
    l->production = prod;
    l->learning = learn;
    l->year = year;
    l->active = 1;
    csa.total_comprehend += compre;
    csa.n_language++;
    print_str("[CSA] Language "); print_int(csa.n_language - 1);
    print_str(" type="); print_int(lg_type);
    print_str(" cat="); print_int(cat);
    print_str(" cmp="); print_int(compre);
    print_str(" prd="); print_int(prod);
    print_str(" lrn="); print_int(learn); print_str("\n");
    return csa.n_language - 1;
}

int csa_decision(int dc_type, int cat, int risk_a, int reward_m, int choice_p, int year) {
    if (csa.n_decision >= MAX_DECISION) return -1;
    decision_t* d = &decisions[csa.n_decision];
    d->decision_id = csa.n_decision;
    d->decision_type = dc_type;
    d->decision_category = cat;
    d->risk_assess = risk_a;
    d->reward_mech = reward_m;
    d->choice_pref = choice_p;
    d->year = year;
    d->active = 1;
    csa.total_risk += risk_a;
    csa.n_decision++;
    print_str("[CSA] Decision "); print_int(csa.n_decision - 1);
    print_str(" type="); print_int(dc_type);
    print_str(" cat="); print_int(cat);
    print_str(" rsk="); print_int(risk_a);
    print_str(" rwd="); print_int(reward_m);
    print_str(" chp="); print_int(choice_p); print_str("\n");
    return csa.n_decision - 1;
}

int csa_social(int sc_type, int cat, int emot_c, int empat, int soc_r, int year) {
    if (csa.n_social >= MAX_SOCIAL) return -1;
    social_t* s = &socials[csa.n_social];
    s->social_id = csa.n_social;
    s->social_type = sc_type;
    s->social_category = cat;
    s->emotion_cog = emot_c;
    s->empathy = empat;
    s->social_reason = soc_r;
    s->year = year;
    s->active = 1;
    csa.total_emotion += emot_c;
    csa.n_social++;
    print_str("[CSA] Social "); print_int(csa.n_social - 1);
    print_str(" type="); print_int(sc_type);
    print_str(" cat="); print_int(cat);
    print_str(" emt="); print_int(emot_c);
    print_str(" emp="); print_int(empat);
    print_str(" scr="); print_int(soc_r); print_str("\n");
    return csa.n_social - 1;
}

void csa_perception_report(void) {
    print_str("[CSA] Perception report:\n");
    print_str("  Perception cognition categories: "); print_int(csa.n_perception); print_str("\n");
    print_str("  Total visual cognition: "); print_int(csa.total_visual); print_str("\n");
}

void csa_memory_report(void) {
    print_str("[CSA] Memory report:\n");
    print_str("  Memory system categories: "); print_int(csa.n_memory); print_str("\n");
    print_str("  Total short-term memory: "); print_int(csa.total_short); print_str("\n");
}

void csa_social_report(void) {
    print_str("[CSA] Social report:\n");
    print_str("  Language cognition categories: "); print_int(csa.n_language); print_str("\n");
    print_str("  Total comprehension: "); print_int(csa.total_comprehend); print_str("\n");
    print_str("  Decision cognition categories: "); print_int(csa.n_decision); print_str("\n");
    print_str("  Total risk assessment: "); print_int(csa.total_risk); print_str("\n");
    print_str("  Social cognition categories: "); print_int(csa.n_social); print_str("\n");
    print_str("  Total emotion cognition: "); print_int(csa.total_emotion); print_str("\n");
}

void csa_print_state(void) {
    print_str("[CSA] Pc="); print_int(csa.n_perception);
    print_str(" Mm="); print_int(csa.n_memory);
    print_str(" Lg="); print_int(csa.n_language);
    print_str(" Dc="); print_int(csa.n_decision);
    print_str(" Sc="); print_int(csa.n_social);
    print_str("\n");
}

int main(void) {
    print_str("=== Cognitive Science Admin Demo ===\n\n");
    csa_init();

    print_str("Perception cognition...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int vis = 58 + (i * 14);
        int aud = 40 + (i * 10);
        int tac = 25 + (i * 6);
        int year = 2020 + (i % 5);
        csa_perception(type, cat, vis, aud, tac, year);
    }

    print_str("\nMemory systems...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int sht = 50 + (i * 12);
        int lng = 35 + (i * 8);
        int wrk = 22 + (i * 5);
        int year = 2021 + (i % 4);
        csa_memory(type, cat, sht, lng, wrk, year);
    }

    print_str("\nLanguage cognition...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cmp = 45 + (i * 11);
        int prd = 30 + (i * 7);
        int lrn = 18 + (i * 4);
        int year = 2022 + (i % 3);
        csa_language(type, cat, cmp, prd, lrn, year);
    }

    print_str("\nDecision cognition...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int rsk = 38 + (i * 9);
        int rwd = 25 + (i * 6);
        int chp = 15 + (i * 3);
        int year = 2023 + (i % 2);
        csa_decision(type, cat, rsk, rwd, chp, year);
    }

    print_str("\nSocial cognition...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int emt = 32 + (i * 8);
        int emp = 22 + (i * 5);
        int scr = 12 + (i * 3);
        int year = 2024;
        csa_social(type, cat, emt, emp, scr, year);
    }

    print_str("\nPerception report...\n");
    csa_perception_report();

    print_str("\nMemory report...\n");
    csa_memory_report();

    print_str("\nSocial report...\n");
    csa_social_report();

    print_str("\nFinal state...\n");
    csa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
