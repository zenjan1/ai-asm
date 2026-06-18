/* heat_transfer_admin: Heat transfer administration system (v1.0)
 * Conduction, convection, radiation, phase change, enhancement
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

#define MAX_CONDUCT     16
#define MAX_CONVECT     14
#define MAX_RADIATE     12
#define MAX_PHASE_CH    10
#define MAX_ENHANCE     10

typedef struct {
    int    cnd_id;
    int    cnd_type;
    int    cnd_category;
    int    fourier;
    int    steady_st;
    int    unsteady_st;
    int    year;
    int    active;
} conduct_t;

typedef struct {
    int    cnv_id;
    int    cnv_type;
    int    cnv_category;
    int    natural_cv;
    int    forced_cv;
    int    boiling_cv;
    int    year;
    int    active;
} convect_t;

typedef struct {
    int    rad_id;
    int    rad_type;
    int    rad_category;
    int    planck;
    int    stefan_boltz;
    int    year;
    int    active;
} radiate_t;

typedef struct {
    int    phc_id;
    int    phc_type;
    int    phc_category;
    int    condensation;
    int    boiling_pc;
    int    year;
    int    active;
} phase_change_t;

typedef struct {
    int    enh_id;
    int    enh_type;
    int    enh_category;
    int    fin;
    int    heat_pipe;
    int    pcm;
    int    year;
    int    active;
} enhance_t;

typedef struct {
    int    n_conduct;
    int    n_convect;
    int    n_radiate;
    int    n_phase_change;
    int    n_enhance;
    int    total_fourier;
    int    total_natural_cv;
    int    total_stefan;
    int    total_condensation;
    int    total_fin;
} hta_state_t;

static conduct_t conducts[MAX_CONDUCT];
static convect_t convects[MAX_CONVECT];
static radiate_t radiates[MAX_RADIATE];
static phase_change_t phase_changes[MAX_PHASE_CH];
static enhance_t enhances[MAX_ENHANCE];
static hta_state_t hta;

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

int hta_init(void) {
    if (initialized) return -1;
    hta.n_conduct = 0; hta.n_convect = 0; hta.n_radiate = 0;
    hta.n_phase_change = 0; hta.n_enhance = 0;
    hta.total_fourier = 0; hta.total_natural_cv = 0;
    hta.total_stefan = 0; hta.total_condensation = 0;
    hta.total_fin = 0;
    for (int i = 0; i < MAX_CONDUCT; i++) conducts[i].active = 0;
    for (int i = 0; i < MAX_CONVECT; i++) convects[i].active = 0;
    for (int i = 0; i < MAX_RADIATE; i++) radiates[i].active = 0;
    for (int i = 0; i < MAX_PHASE_CH; i++) phase_changes[i].active = 0;
    for (int i = 0; i < MAX_ENHANCE; i++) enhances[i].active = 0;
    initialized = 1;
    print_str("[HTA] Heat transfer initialized\n");
    return 0;
}

int hta_conduct(int ct_type, int cat, int four, int stdy, int unst, int year) {
    if (hta.n_conduct >= MAX_CONDUCT) return -1;
    conduct_t* c = &conducts[hta.n_conduct];
    c->cnd_id = hta.n_conduct;
    c->cnd_type = ct_type;
    c->cnd_category = cat;
    c->fourier = four;
    c->steady_st = stdy;
    c->unsteady_st = unst;
    c->year = year;
    c->active = 1;
    hta.total_fourier += four;
    hta.n_conduct++;
    print_str("[HTA] Conduct "); print_int(hta.n_conduct - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" fur="); print_int(four);
    print_str(" std="); print_int(stdy);
    print_str(" uns="); print_int(unst); print_str("\n");
    return hta.n_conduct - 1;
}

int hta_convect(int cv_type, int cat, int nat, int frc, int bol, int year) {
    if (hta.n_convect >= MAX_CONVECT) return -1;
    convect_t* c = &convects[hta.n_convect];
    c->cnv_id = hta.n_convect;
    c->cnv_type = cv_type;
    c->cnv_category = cat;
    c->natural_cv = nat;
    c->forced_cv = frc;
    c->boiling_cv = bol;
    c->year = year;
    c->active = 1;
    hta.total_natural_cv += nat;
    hta.n_convect++;
    print_str("[HTA] Convect "); print_int(hta.n_convect - 1);
    print_str(" type="); print_int(cv_type);
    print_str(" cat="); print_int(cat);
    print_str(" nat="); print_int(nat);
    print_str(" frc="); print_int(frc);
    print_str(" bol="); print_int(bol); print_str("\n");
    return hta.n_convect - 1;
}

int hta_radiate(int rd_type, int cat, int plnk, int stb, int year) {
    if (hta.n_radiate >= MAX_RADIATE) return -1;
    radiate_t* r = &radiates[hta.n_radiate];
    r->rad_id = hta.n_radiate;
    r->rad_type = rd_type;
    r->rad_category = cat;
    r->planck = plnk;
    r->stefan_boltz = stb;
    r->year = year;
    r->active = 1;
    hta.total_stefan += stb;
    hta.n_radiate++;
    print_str("[HTA] Radiate "); print_int(hta.n_radiate - 1);
    print_str(" type="); print_int(rd_type);
    print_str(" cat="); print_int(cat);
    print_str(" plk="); print_int(plnk);
    print_str(" stb="); print_int(stb); print_str("\n");
    return hta.n_radiate - 1;
}

int hta_phase_change(int pc_type, int cat, int condn, int bol_pc, int year) {
    if (hta.n_phase_change >= MAX_PHASE_CH) return -1;
    phase_change_t* p = &phase_changes[hta.n_phase_change];
    p->phc_id = hta.n_phase_change;
    p->phc_type = pc_type;
    p->phc_category = cat;
    p->condensation = condn;
    p->boiling_pc = bol_pc;
    p->year = year;
    p->active = 1;
    hta.total_condensation += condn;
    hta.n_phase_change++;
    print_str("[HTA] Phase change "); print_int(hta.n_phase_change - 1);
    print_str(" type="); print_int(pc_type);
    print_str(" cat="); print_int(cat);
    print_str(" cnd="); print_int(condn);
    print_str(" blp="); print_int(bol_pc); print_str("\n");
    return hta.n_phase_change - 1;
}

int hta_enhance(int eh_type, int cat, int fn, int hp, int pcm_v, int year) {
    if (hta.n_enhance >= MAX_ENHANCE) return -1;
    enhance_t* e = &enhances[hta.n_enhance];
    e->enh_id = hta.n_enhance;
    e->enh_type = eh_type;
    e->enh_category = cat;
    e->fin = fn;
    e->heat_pipe = hp;
    e->pcm = pcm_v;
    e->year = year;
    e->active = 1;
    hta.total_fin += fn;
    hta.n_enhance++;
    print_str("[HTA] Enhance "); print_int(hta.n_enhance - 1);
    print_str(" type="); print_int(eh_type);
    print_str(" cat="); print_int(cat);
    print_str(" fn="); print_int(fn);
    print_str(" hpp="); print_int(hp);
    print_str(" pcm="); print_int(pcm_v); print_str("\n");
    return hta.n_enhance - 1;
}

void hta_conduct_report(void) {
    print_str("[HTA] Conduction report:\n");
    print_str("  Conduction categories: "); print_int(hta.n_conduct); print_str("\n");
    print_str("  Total Fourier: "); print_int(hta.total_fourier); print_str("\n");
}

void hta_convect_report(void) {
    print_str("[HTA] Convection report:\n");
    print_str("  Convection categories: "); print_int(hta.n_convect); print_str("\n");
    print_str("  Total natural convection: "); print_int(hta.total_natural_cv); print_str("\n");
}

void hta_enhance_report(void) {
    print_str("[HTA] Enhancement report:\n");
    print_str("  Radiation categories: "); print_int(hta.n_radiate); print_str("\n");
    print_str("  Total Stefan-Boltzmann: "); print_int(hta.total_stefan); print_str("\n");
    print_str("  Phase change categories: "); print_int(hta.n_phase_change); print_str("\n");
    print_str("  Total condensation: "); print_int(hta.total_condensation); print_str("\n");
    print_str("  Enhancement categories: "); print_int(hta.n_enhance); print_str("\n");
    print_str("  Total fins: "); print_int(hta.total_fin); print_str("\n");
}

void hta_print_state(void) {
    print_str("[HTA] Cn="); print_int(hta.n_conduct);
    print_str(" Cv="); print_int(hta.n_convect);
    print_str(" Rd="); print_int(hta.n_radiate);
    print_str(" Pc="); print_int(hta.n_phase_change);
    print_str(" Eh="); print_int(hta.n_enhance);
    print_str("\n");
}

int main(void) {
    print_str("=== Heat Transfer Admin Demo ===\n\n");
    hta_init();

    print_str("Conduction...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int fur = 55 + (i * 13);
        int stdy = 40 + (i * 10);
        int uns = 22 + (i * 5);
        int year = 2020 + (i % 5);
        hta_conduct(type, cat, fur, stdy, uns, year);
    }

    print_str("\nConvection...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int nat = 48 + (i * 11);
        int frc = 35 + (i * 8);
        int bol = 20 + (i * 4);
        int year = 2021 + (i % 4);
        hta_convect(type, cat, nat, frc, bol, year);
    }

    print_str("\nRadiation...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int plk = 42 + (i * 10);
        int stb = 28 + (i * 7);
        int year = 2022 + (i % 3);
        hta_radiate(type, cat, plk, stb, year);
    }

    print_str("\nPhase change...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cnd = 35 + (i * 8);
        int blp = 25 + (i * 6);
        int year = 2023 + (i % 2);
        hta_phase_change(type, cat, cnd, blp, year);
    }

    print_str("\nEnhancement...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int fn = 30 + (i * 7);
        int hp = 22 + (i * 5);
        int pcm_v = 12 + (i * 3);
        int year = 2024;
        hta_enhance(type, cat, fn, hp, pcm_v, year);
    }

    print_str("\nConduction report...\n");
    hta_conduct_report();

    print_str("\nConvection report...\n");
    hta_convect_report();

    print_str("\nEnhancement report...\n");
    hta_enhance_report();

    print_str("\nFinal state...\n");
    hta_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
