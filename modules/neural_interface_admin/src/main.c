/* neural_interface_admin: Neural interface administration system (v1.0)
 * Neural acquisition, neural stimulation, neural feedback, rehabilitation, enhancement
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

#define MAX_ACQUIRE    16
#define MAX_STIMULATE  14
#define MAX_FEEDBACK   12
#define MAX_REHAB      10
#define MAX_ENHANCE    10

typedef struct {
    int    acquire_id;
    int    acquire_type;
    int    acquire_category;
    int    eeg_signal;
    int    emg_signal;
    int    eye_track;
    int    year;
    int    active;
} acquire_t;

typedef struct {
    int    stim_id;
    int    stim_type;
    int    stim_category;
    int    tms_stim;
    int    tES_stim;
    int    dbs_stim;
    int    year;
    int    active;
} stimulate_t;

typedef struct {
    int    feedback_id;
    int    feedback_type;
    int    feedback_category;
    int    realtime_eeg;
    int    neuro_modulate;
    int    cognitive_train;
    int    year;
    int    active;
} feedback_t;

typedef struct {
    int    rehab_id;
    int    rehab_type;
    int    rehab_category;
    int    motor_rehab;
    int    cognitive_rehab;
    int    language_rehab;
    int    year;
    int    active;
} rehab_t;

typedef struct {
    int    enhance_id;
    int    enhance_type;
    int    enhance_category;
    int    attention_enh;
    int    memory_enh;
    int    learning_enh;
    int    year;
    int    active;
} enhance_t;

typedef struct {
    int    n_acquire;
    int    n_stimulate;
    int    n_feedback;
    int    n_rehab;
    int    n_enhance;
    int    total_eeg;
    int    total_tms;
    int    total_realtime;
    int    total_motor;
    int    total_attention;
} nia_state_t;

static acquire_t acquires[MAX_ACQUIRE];
static stimulate_t stimulates[MAX_STIMULATE];
static feedback_t feedbacks[MAX_FEEDBACK];
static rehab_t rehabs[MAX_REHAB];
static enhance_t enhances[MAX_ENHANCE];
static nia_state_t nia;

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

int nia_init(void) {
    if (initialized) return -1;
    nia.n_acquire = 0; nia.n_stimulate = 0; nia.n_feedback = 0;
    nia.n_rehab = 0; nia.n_enhance = 0;
    nia.total_eeg = 0; nia.total_tms = 0;
    nia.total_realtime = 0; nia.total_motor = 0;
    nia.total_attention = 0;
    for (int i = 0; i < MAX_ACQUIRE; i++) acquires[i].active = 0;
    for (int i = 0; i < MAX_STIMULATE; i++) stimulates[i].active = 0;
    for (int i = 0; i < MAX_FEEDBACK; i++) feedbacks[i].active = 0;
    for (int i = 0; i < MAX_REHAB; i++) rehabs[i].active = 0;
    for (int i = 0; i < MAX_ENHANCE; i++) enhances[i].active = 0;
    initialized = 1;
    print_str("[NIA] Neural interface initialized\n");
    return 0;
}

int nia_acquire(int ac_type, int cat, int eeg_s, int emg_s, int eye_t, int year) {
    if (nia.n_acquire >= MAX_ACQUIRE) return -1;
    acquire_t* a = &acquires[nia.n_acquire];
    a->acquire_id = nia.n_acquire;
    a->acquire_type = ac_type;
    a->acquire_category = cat;
    a->eeg_signal = eeg_s;
    a->emg_signal = emg_s;
    a->eye_track = eye_t;
    a->year = year;
    a->active = 1;
    nia.total_eeg += eeg_s;
    nia.n_acquire++;
    print_str("[NIA] Acquire "); print_int(nia.n_acquire - 1);
    print_str(" type="); print_int(ac_type);
    print_str(" cat="); print_int(cat);
    print_str(" eeg="); print_int(eeg_s);
    print_str(" emg="); print_int(emg_s);
    print_str(" eye="); print_int(eye_t); print_str("\n");
    return nia.n_acquire - 1;
}

int nia_stimulate(int st_type, int cat, int tms_v, int tes_v, int dbs_v, int year) {
    if (nia.n_stimulate >= MAX_STIMULATE) return -1;
    stimulate_t* s = &stimulates[nia.n_stimulate];
    s->stim_id = nia.n_stimulate;
    s->stim_type = st_type;
    s->stim_category = cat;
    s->tms_stim = tms_v;
    s->tES_stim = tes_v;
    s->dbs_stim = dbs_v;
    s->year = year;
    s->active = 1;
    nia.total_tms += tms_v;
    nia.n_stimulate++;
    print_str("[NIA] Stimulate "); print_int(nia.n_stimulate - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" tms="); print_int(tms_v);
    print_str(" tes="); print_int(tes_v);
    print_str(" dbs="); print_int(dbs_v); print_str("\n");
    return nia.n_stimulate - 1;
}

int nia_feedback(int fb_type, int cat, int realtime_e, int neuro_m, int cog_t, int year) {
    if (nia.n_feedback >= MAX_FEEDBACK) return -1;
    feedback_t* f = &feedbacks[nia.n_feedback];
    f->feedback_id = nia.n_feedback;
    f->feedback_type = fb_type;
    f->feedback_category = cat;
    f->realtime_eeg = realtime_e;
    f->neuro_modulate = neuro_m;
    f->cognitive_train = cog_t;
    f->year = year;
    f->active = 1;
    nia.total_realtime += realtime_e;
    nia.n_feedback++;
    print_str("[NIA] Feedback "); print_int(nia.n_feedback - 1);
    print_str(" type="); print_int(fb_type);
    print_str(" cat="); print_int(cat);
    print_str(" rte="); print_int(realtime_e);
    print_str(" nrm="); print_int(neuro_m);
    print_str(" cgt="); print_int(cog_t); print_str("\n");
    return nia.n_feedback - 1;
}

int nia_rehab(int rh_type, int cat, int motor_r, int cog_r, int lang_r, int year) {
    if (nia.n_rehab >= MAX_REHAB) return -1;
    rehab_t* r = &rehabs[nia.n_rehab];
    r->rehab_id = nia.n_rehab;
    r->rehab_type = rh_type;
    r->rehab_category = cat;
    r->motor_rehab = motor_r;
    r->cognitive_rehab = cog_r;
    r->language_rehab = lang_r;
    r->year = year;
    r->active = 1;
    nia.total_motor += motor_r;
    nia.n_rehab++;
    print_str("[NIA] Rehab "); print_int(nia.n_rehab - 1);
    print_str(" type="); print_int(rh_type);
    print_str(" cat="); print_int(cat);
    print_str(" mtr="); print_int(motor_r);
    print_str(" cgr="); print_int(cog_r);
    print_str(" lgr="); print_int(lang_r); print_str("\n");
    return nia.n_rehab - 1;
}

int nia_enhance(int eh_type, int cat, int attn_e, int mem_e, int lrn_e, int year) {
    if (nia.n_enhance >= MAX_ENHANCE) return -1;
    enhance_t* e = &enhances[nia.n_enhance];
    e->enhance_id = nia.n_enhance;
    e->enhance_type = eh_type;
    e->enhance_category = cat;
    e->attention_enh = attn_e;
    e->memory_enh = mem_e;
    e->learning_enh = lrn_e;
    e->year = year;
    e->active = 1;
    nia.total_attention += attn_e;
    nia.n_enhance++;
    print_str("[NIA] Enhance "); print_int(nia.n_enhance - 1);
    print_str(" type="); print_int(eh_type);
    print_str(" cat="); print_int(cat);
    print_str(" atn="); print_int(attn_e);
    print_str(" mem="); print_int(mem_e);
    print_str(" lrn="); print_int(lrn_e); print_str("\n");
    return nia.n_enhance - 1;
}

void nia_acquire_report(void) {
    print_str("[NIA] Acquire report:\n");
    print_str("  Neural acquisition categories: "); print_int(nia.n_acquire); print_str("\n");
    print_str("  Total EEG signals: "); print_int(nia.total_eeg); print_str("\n");
}

void nia_stimulate_report(void) {
    print_str("[NIA] Stimulate report:\n");
    print_str("  Neural stimulation categories: "); print_int(nia.n_stimulate); print_str("\n");
    print_str("  Total TMS stimulation: "); print_int(nia.total_tms); print_str("\n");
}

void nia_enhance_report(void) {
    print_str("[NIA] Enhance report:\n");
    print_str("  Neural feedback categories: "); print_int(nia.n_feedback); print_str("\n");
    print_str("  Total realtime EEG: "); print_int(nia.total_realtime); print_str("\n");
    print_str("  Neural rehab categories: "); print_int(nia.n_rehab); print_str("\n");
    print_str("  Total motor rehab: "); print_int(nia.total_motor); print_str("\n");
    print_str("  Neural enhancement categories: "); print_int(nia.n_enhance); print_str("\n");
    print_str("  Total attention enhancement: "); print_int(nia.total_attention); print_str("\n");
}

void nia_print_state(void) {
    print_str("[NIA] Ac="); print_int(nia.n_acquire);
    print_str(" St="); print_int(nia.n_stimulate);
    print_str(" Fb="); print_int(nia.n_feedback);
    print_str(" Rh="); print_int(nia.n_rehab);
    print_str(" Eh="); print_int(nia.n_enhance);
    print_str("\n");
}

int main(void) {
    print_str("=== Neural Interface Admin Demo ===\n\n");
    nia_init();

    print_str("Neural acquisition...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int eeg = 60 + (i * 14);
        int emg = 42 + (i * 10);
        int eye = 25 + (i * 6);
        int year = 2020 + (i % 5);
        nia_acquire(type, cat, eeg, emg, eye, year);
    }

    print_str("\nNeural stimulation...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int tms = 50 + (i * 12);
        int tes = 35 + (i * 8);
        int dbs = 22 + (i * 5);
        int year = 2021 + (i % 4);
        nia_stimulate(type, cat, tms, tes, dbs, year);
    }

    print_str("\nNeural feedback...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int rte = 45 + (i * 11);
        int nrm = 30 + (i * 7);
        int cgt = 18 + (i * 4);
        int year = 2022 + (i % 3);
        nia_feedback(type, cat, rte, nrm, cgt, year);
    }

    print_str("\nNeural rehabilitation...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int mtr = 38 + (i * 9);
        int cgr = 25 + (i * 6);
        int lgr = 15 + (i * 3);
        int year = 2023 + (i % 2);
        nia_rehab(type, cat, mtr, cgr, lgr, year);
    }

    print_str("\nNeural enhancement...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int atn = 32 + (i * 8);
        int mem = 22 + (i * 5);
        int lrn = 12 + (i * 3);
        int year = 2024;
        nia_enhance(type, cat, atn, mem, lrn, year);
    }

    print_str("\nAcquire report...\n");
    nia_acquire_report();

    print_str("\nStimulate report...\n");
    nia_stimulate_report();

    print_str("\nEnhance report...\n");
    nia_enhance_report();

    print_str("\nFinal state...\n");
    nia_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
