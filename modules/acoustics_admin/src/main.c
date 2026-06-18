/* acoustics_admin: Acoustics engineering administration system (v1.0)
 * Acoustic fundamentals, noise control, ultrasound, architectural, musical
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

#define MAX_ACOUSTIC    16
#define MAX_NOISE       14
#define MAX_ULTRA       12
#define MAX_ARCH_AC     10
#define MAX_MUSICAL     10

typedef struct {
    int    ac_id;
    int    ac_type;
    int    ac_category;
    int    wave_prop;
    int    sound_speed;
    int    impedance;
    int    year;
    int    active;
} acoustic_t;

typedef struct {
    int    ns_id;
    int    ns_type;
    int    ns_category;
    int    absorption;
    int    sound_insul;
    int    silencer;
    int    year;
    int    active;
} noise_t;

typedef struct {
    int    ut_id;
    int    ut_type;
    int    ut_category;
    int    ultr_detect;
    int    ultr_clean;
    int    ultr_weld;
    int    year;
    int    active;
} ultrasound_t;

typedef struct {
    int    ara_id;
    int    ara_type;
    int    ara_category;
    int    hall_quality;
    int    soundproof;
    int    noise_rating;
    int    year;
    int    active;
} arch_acoustic_t;

typedef struct {
    int    ms_id;
    int    ms_type;
    int    ms_category;
    int    instrument;
    int    room_ac;
    int    electro_ac;
    int    year;
    int    active;
} musical_t;

typedef struct {
    int    n_acoustic;
    int    n_noise;
    int    n_ultrasound;
    int    n_arch_acoustic;
    int    n_musical;
    int    total_wave_prop;
    int    total_absorption;
    int    total_ultr_detect;
    int    total_hall_quality;
    int    total_instrument;
} aba_state_t;

static acoustic_t acoustics[MAX_ACOUSTIC];
static noise_t noises[MAX_NOISE];
static ultrasound_t ultrasounds[MAX_ULTRA];
static arch_acoustic_t arch_acoustics[MAX_ARCH_AC];
static musical_t musicals[MAX_MUSICAL];
static aba_state_t aba;

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

int aba_init(void) {
    if (initialized) return -1;
    aba.n_acoustic = 0; aba.n_noise = 0; aba.n_ultrasound = 0;
    aba.n_arch_acoustic = 0; aba.n_musical = 0;
    aba.total_wave_prop = 0; aba.total_absorption = 0;
    aba.total_ultr_detect = 0; aba.total_hall_quality = 0;
    aba.total_instrument = 0;
    for (int i = 0; i < MAX_ACOUSTIC; i++) acoustics[i].active = 0;
    for (int i = 0; i < MAX_NOISE; i++) noises[i].active = 0;
    for (int i = 0; i < MAX_ULTRA; i++) ultrasounds[i].active = 0;
    for (int i = 0; i < MAX_ARCH_AC; i++) arch_acoustics[i].active = 0;
    for (int i = 0; i < MAX_MUSICAL; i++) musicals[i].active = 0;
    initialized = 1;
    print_str("[ABA] Acoustics initialized\n");
    return 0;
}

int aba_acoustic(int at_type, int cat, int wpr, int spd, int imp, int year) {
    if (aba.n_acoustic >= MAX_ACOUSTIC) return -1;
    acoustic_t* a = &acoustics[aba.n_acoustic];
    a->ac_id = aba.n_acoustic;
    a->ac_type = at_type;
    a->ac_category = cat;
    a->wave_prop = wpr;
    a->sound_speed = spd;
    a->impedance = imp;
    a->year = year;
    a->active = 1;
    aba.total_wave_prop += wpr;
    aba.n_acoustic++;
    print_str("[ABA] Acoustic "); print_int(aba.n_acoustic - 1);
    print_str(" type="); print_int(at_type);
    print_str(" cat="); print_int(cat);
    print_str(" wpr="); print_int(wpr);
    print_str(" spd="); print_int(spd);
    print_str(" imp="); print_int(imp); print_str("\n");
    return aba.n_acoustic - 1;
}

int aba_noise(int nt_type, int cat, int abs, int insl, int sil, int year) {
    if (aba.n_noise >= MAX_NOISE) return -1;
    noise_t* n = &noises[aba.n_noise];
    n->ns_id = aba.n_noise;
    n->ns_type = nt_type;
    n->ns_category = cat;
    n->absorption = abs;
    n->sound_insul = insl;
    n->silencer = sil;
    n->year = year;
    n->active = 1;
    aba.total_absorption += abs;
    aba.n_noise++;
    print_str("[ABA] Noise "); print_int(aba.n_noise - 1);
    print_str(" type="); print_int(nt_type);
    print_str(" cat="); print_int(cat);
    print_str(" abs="); print_int(abs);
    print_str(" ins="); print_int(insl);
    print_str(" sil="); print_int(sil); print_str("\n");
    return aba.n_noise - 1;
}

int aba_ultrasound(int ut_type, int cat, int det, int cln, int wld, int year) {
    if (aba.n_ultrasound >= MAX_ULTRA) return -1;
    ultrasound_t* u = &ultrasounds[aba.n_ultrasound];
    u->ut_id = aba.n_ultrasound;
    u->ut_type = ut_type;
    u->ut_category = cat;
    u->ultr_detect = det;
    u->ultr_clean = cln;
    u->ultr_weld = wld;
    u->year = year;
    u->active = 1;
    aba.total_ultr_detect += det;
    aba.n_ultrasound++;
    print_str("[ABA] Ultrasound "); print_int(aba.n_ultrasound - 1);
    print_str(" type="); print_int(ut_type);
    print_str(" cat="); print_int(cat);
    print_str(" det="); print_int(det);
    print_str(" cln="); print_int(cln);
    print_str(" wld="); print_int(wld); print_str("\n");
    return aba.n_ultrasound - 1;
}

int aba_arch_acoustic(int ar_type, int cat, int hq, int sproof, int nrt, int year) {
    if (aba.n_arch_acoustic >= MAX_ARCH_AC) return -1;
    arch_acoustic_t* a = &arch_acoustics[aba.n_arch_acoustic];
    a->ara_id = aba.n_arch_acoustic;
    a->ara_type = ar_type;
    a->ara_category = cat;
    a->hall_quality = hq;
    a->soundproof = sproof;
    a->noise_rating = nrt;
    a->year = year;
    a->active = 1;
    aba.total_hall_quality += hq;
    aba.n_arch_acoustic++;
    print_str("[ABA] Arch acoustic "); print_int(aba.n_arch_acoustic - 1);
    print_str(" type="); print_int(ar_type);
    print_str(" cat="); print_int(cat);
    print_str(" hlq="); print_int(hq);
    print_str(" spf="); print_int(sproof);
    print_str(" nrt="); print_int(nrt); print_str("\n");
    return aba.n_arch_acoustic - 1;
}

int aba_musical(int mu_type, int cat, int inst, int rom, int elc, int year) {
    if (aba.n_musical >= MAX_MUSICAL) return -1;
    musical_t* m = &musicals[aba.n_musical];
    m->ms_id = aba.n_musical;
    m->ms_type = mu_type;
    m->ms_category = cat;
    m->instrument = inst;
    m->room_ac = rom;
    m->electro_ac = elc;
    m->year = year;
    m->active = 1;
    aba.total_instrument += inst;
    aba.n_musical++;
    print_str("[ABA] Musical "); print_int(aba.n_musical - 1);
    print_str(" type="); print_int(mu_type);
    print_str(" cat="); print_int(cat);
    print_str(" ist="); print_int(inst);
    print_str(" rom="); print_int(rom);
    print_str(" elc="); print_int(elc); print_str("\n");
    return aba.n_musical - 1;
}

void aba_acoustic_report(void) {
    print_str("[ABA] Acoustic report:\n");
    print_str("  Acoustic categories: "); print_int(aba.n_acoustic); print_str("\n");
    print_str("  Total wave propagation: "); print_int(aba.total_wave_prop); print_str("\n");
}

void aba_noise_report(void) {
    print_str("[ABA] Noise report:\n");
    print_str("  Noise control categories: "); print_int(aba.n_noise); print_str("\n");
    print_str("  Total absorption: "); print_int(aba.total_absorption); print_str("\n");
}

void aba_full_report(void) {
    print_str("[ABA] Full report:\n");
    print_str("  Ultrasound categories: "); print_int(aba.n_ultrasound); print_str("\n");
    print_str("  Total ultrasonic detection: "); print_int(aba.total_ultr_detect); print_str("\n");
    print_str("  Architectural acoustic categories: "); print_int(aba.n_arch_acoustic); print_str("\n");
    print_str("  Total hall quality: "); print_int(aba.total_hall_quality); print_str("\n");
    print_str("  Musical acoustic categories: "); print_int(aba.n_musical); print_str("\n");
    print_str("  Total instrument: "); print_int(aba.total_instrument); print_str("\n");
}

void aba_print_state(void) {
    print_str("[ABA] Ac="); print_int(aba.n_acoustic);
    print_str(" Ns="); print_int(aba.n_noise);
    print_str(" Ut="); print_int(aba.n_ultrasound);
    print_str(" Ar="); print_int(aba.n_arch_acoustic);
    print_str(" Ms="); print_int(aba.n_musical);
    print_str("\n");
}

int main(void) {
    print_str("=== Acoustics Admin Demo ===\n\n");
    aba_init();

    print_str("Acoustic fundamentals...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int wpr = 55 + (i * 13);
        int spd = 40 + (i * 10);
        int imp = 22 + (i * 5);
        int year = 2020 + (i % 5);
        aba_acoustic(type, cat, wpr, spd, imp, year);
    }

    print_str("\nNoise control...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int abs = 48 + (i * 11);
        int insl = 35 + (i * 8);
        int sil = 20 + (i * 4);
        int year = 2021 + (i % 4);
        aba_noise(type, cat, abs, insl, sil, year);
    }

    print_str("\nUltrasound...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int det = 42 + (i * 10);
        int cln = 28 + (i * 7);
        int wld = 18 + (i * 4);
        int year = 2022 + (i % 3);
        aba_ultrasound(type, cat, det, cln, wld, year);
    }

    print_str("\nArchitectural acoustics...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int hq = 35 + (i * 8);
        int sproof = 25 + (i * 6);
        int nrt = 15 + (i * 3);
        int year = 2023 + (i % 2);
        aba_arch_acoustic(type, cat, hq, sproof, nrt, year);
    }

    print_str("\nMusical acoustics...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int inst = 30 + (i * 7);
        int rom = 22 + (i * 5);
        int elc = 12 + (i * 3);
        int year = 2024;
        aba_musical(type, cat, inst, rom, elc, year);
    }

    print_str("\nAcoustic report...\n");
    aba_acoustic_report();

    print_str("\nNoise report...\n");
    aba_noise_report();

    print_str("\nFull report...\n");
    aba_full_report();

    print_str("\nFinal state...\n");
    aba_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
