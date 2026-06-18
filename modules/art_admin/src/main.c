/* art_admin: Art administration system (v1.0)
 * Art theory, music, fine arts, drama/film, dance
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

#define MAX_ART_TH      16
#define MAX_MUSIC       14
#define MAX_FINE_ART    12
#define MAX_DRAMA       10
#define MAX_DANCE       10

typedef struct {
    int    at_id;
    int    at_type;
    int    at_category;
    int    art_pri;
    int    aesth;
    int    art_cri;
    int    art_his;
    int    year;
    int    active;
} art_th_t;

typedef struct {
    int    mu_id;
    int    mu_type;
    int    mu_category;
    int    mus_th;
    int    compo;
    int    mus_perf;
    int    mus_edu;
    int    year;
    int    active;
} music_t;

typedef struct {
    int    fa_id;
    int    fa_type;
    int    fa_category;
    int    paint;
    int    sculp;
    int    calli;
    int    art_des;
    int    year;
    int    active;
} fine_art_t;

typedef struct {
    int    dr_id;
    int    dr_type;
    int    dr_category;
    int    drama;
    int    film;
    int    broad_art;
    int    anim;
    int    year;
    int    active;
} drama_t;

typedef struct {
    int    da_id;
    int    da_type;
    int    da_category;
    int    dan_th;
    int    dan_perf;
    int    dan_chor;
    int    dan_edu;
    int    year;
    int    active;
} dance_t;

typedef struct {
    int    n_art_th;
    int    n_music;
    int    n_fine_art;
    int    n_drama;
    int    n_dance;
    int    total_art_pri;
    int    total_mus_th;
    int    total_paint;
    int    total_drama;
    int    total_dan_th;
} aa_state_t;

static art_th_t art_ths[MAX_ART_TH];
static music_t musics[MAX_MUSIC];
static fine_art_t fine_arts[MAX_FINE_ART];
static drama_t dramas[MAX_DRAMA];
static dance_t dances[MAX_DANCE];
static aa_state_t aa;

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

int aa_init(void) {
    if (initialized) return -1;
    aa.n_art_th = 0; aa.n_music = 0; aa.n_fine_art = 0;
    aa.n_drama = 0; aa.n_dance = 0;
    aa.total_art_pri = 0; aa.total_mus_th = 0;
    aa.total_paint = 0; aa.total_drama = 0;
    aa.total_dan_th = 0;
    for (int i = 0; i < MAX_ART_TH; i++) art_ths[i].active = 0;
    for (int i = 0; i < MAX_MUSIC; i++) musics[i].active = 0;
    for (int i = 0; i < MAX_FINE_ART; i++) fine_arts[i].active = 0;
    for (int i = 0; i < MAX_DRAMA; i++) dramas[i].active = 0;
    for (int i = 0; i < MAX_DANCE; i++) dances[i].active = 0;
    initialized = 1;
    print_str("[AA] Art initialized\n");
    return 0;
}

int aa_art_th(int at_type, int cat, int apr, int ast, int acr, int ahs, int year) {
    if (aa.n_art_th >= MAX_ART_TH) return -1;
    art_th_t* a = &art_ths[aa.n_art_th];
    a->at_id = aa.n_art_th;
    a->at_type = at_type;
    a->at_category = cat;
    a->art_pri = apr;
    a->aesth = ast;
    a->art_cri = acr;
    a->art_his = ahs;
    a->year = year;
    a->active = 1;
    aa.total_art_pri += apr;
    aa.n_art_th++;
    print_str("[AA] Art th "); print_int(aa.n_art_th - 1);
    print_str(" type="); print_int(at_type);
    print_str(" cat="); print_int(cat);
    print_str(" apr="); print_int(apr);
    print_str(" ast="); print_int(ast);
    print_str(" acr="); print_int(acr);
    print_str(" ahs="); print_int(ahs); print_str("\n");
    return aa.n_art_th - 1;
}

int aa_music(int mt_type, int cat, int mth, int cmp, int mpr, int med, int year) {
    if (aa.n_music >= MAX_MUSIC) return -1;
    music_t* m = &musics[aa.n_music];
    m->mu_id = aa.n_music;
    m->mu_type = mt_type;
    m->mu_category = cat;
    m->mus_th = mth;
    m->compo = cmp;
    m->mus_perf = mpr;
    m->mus_edu = med;
    m->year = year;
    m->active = 1;
    aa.total_mus_th += mth;
    aa.n_music++;
    print_str("[AA] Music "); print_int(aa.n_music - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" mth="); print_int(mth);
    print_str(" cmp="); print_int(cmp);
    print_str(" mpr="); print_int(mpr);
    print_str(" med="); print_int(med); print_str("\n");
    return aa.n_music - 1;
}

int aa_fine_art(int ft_type, int cat, int pnt, int scl, int cal, int ads, int year) {
    if (aa.n_fine_art >= MAX_FINE_ART) return -1;
    fine_art_t* f = &fine_arts[aa.n_fine_art];
    f->fa_id = aa.n_fine_art;
    f->fa_type = ft_type;
    f->fa_category = cat;
    f->paint = pnt;
    f->sculp = scl;
    f->calli = cal;
    f->art_des = ads;
    f->year = year;
    f->active = 1;
    aa.total_paint += pnt;
    aa.n_fine_art++;
    print_str("[AA] Fine art "); print_int(aa.n_fine_art - 1);
    print_str(" type="); print_int(ft_type);
    print_str(" cat="); print_int(cat);
    print_str(" pnt="); print_int(pnt);
    print_str(" scl="); print_int(scl);
    print_str(" cal="); print_int(cal);
    print_str(" ads="); print_int(ads); print_str("\n");
    return aa.n_fine_art - 1;
}

int aa_drama(int dt_type, int cat, int drm, int flm, int brd, int ani, int year) {
    if (aa.n_drama >= MAX_DRAMA) return -1;
    drama_t* d = &dramas[aa.n_drama];
    d->dr_id = aa.n_drama;
    d->dr_type = dt_type;
    d->dr_category = cat;
    d->drama = drm;
    d->film = flm;
    d->broad_art = brd;
    d->anim = ani;
    d->year = year;
    d->active = 1;
    aa.total_drama += drm;
    aa.n_drama++;
    print_str("[AA] Drama "); print_int(aa.n_drama - 1);
    print_str(" type="); print_int(dt_type);
    print_str(" cat="); print_int(cat);
    print_str(" drm="); print_int(drm);
    print_str(" flm="); print_int(flm);
    print_str(" brd="); print_int(brd);
    print_str(" ani="); print_int(ani); print_str("\n");
    return aa.n_drama - 1;
}

int aa_dance(int dt_type, int cat, int dth, int dpr, int dch, int ded, int year) {
    if (aa.n_dance >= MAX_DANCE) return -1;
    dance_t* d = &dances[aa.n_dance];
    d->da_id = aa.n_dance;
    d->da_type = dt_type;
    d->da_category = cat;
    d->dan_th = dth;
    d->dan_perf = dpr;
    d->dan_chor = dch;
    d->dan_edu = ded;
    d->year = year;
    d->active = 1;
    aa.total_dan_th += dth;
    aa.n_dance++;
    print_str("[AA] Dance "); print_int(aa.n_dance - 1);
    print_str(" type="); print_int(dt_type);
    print_str(" cat="); print_int(cat);
    print_str(" dth="); print_int(dth);
    print_str(" dpr="); print_int(dpr);
    print_str(" dch="); print_int(dch);
    print_str(" ded="); print_int(ded); print_str("\n");
    return aa.n_dance - 1;
}

void aa_theory_report(void) {
    print_str("[AA] Art theory report:\n");
    print_str("  Theory categories: "); print_int(aa.n_art_th); print_str("\n");
    print_str("  Total art principles: "); print_int(aa.total_art_pri); print_str("\n");
}

void aa_music_report(void) {
    print_str("[AA] Music report:\n");
    print_str("  Music categories: "); print_int(aa.n_music); print_str("\n");
    print_str("  Total music theory: "); print_int(aa.total_mus_th); print_str("\n");
}

void aa_full_report(void) {
    print_str("[AA] Full report:\n");
    print_str("  Fine art categories: "); print_int(aa.n_fine_art); print_str("\n");
    print_str("  Total painting: "); print_int(aa.total_paint); print_str("\n");
    print_str("  Drama categories: "); print_int(aa.n_drama); print_str("\n");
    print_str("  Total drama: "); print_int(aa.total_drama); print_str("\n");
    print_str("  Dance categories: "); print_int(aa.n_dance); print_str("\n");
    print_str("  Total dance theory: "); print_int(aa.total_dan_th); print_str("\n");
}

void aa_print_state(void) {
    print_str("[AA] At="); print_int(aa.n_art_th);
    print_str(" Mu="); print_int(aa.n_music);
    print_str(" Fa="); print_int(aa.n_fine_art);
    print_str(" Dr="); print_int(aa.n_drama);
    print_str(" Da="); print_int(aa.n_dance);
    print_str("\n");
}

int main(void) {
    print_str("=== Art Admin Demo ===\n\n");
    aa_init();

    print_str("Art theory...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int apr = 55 + (i * 13);
        int ast = 40 + (i * 10);
        int acr = 22 + (i * 5);
        int ahs = 15 + (i * 3);
        int year = 2020 + (i % 5);
        aa_art_th(type, cat, apr, ast, acr, ahs, year);
    }

    print_str("\nMusic...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int mth = 48 + (i * 11);
        int cmp = 35 + (i * 8);
        int mpr = 20 + (i * 4);
        int med = 12 + (i * 3);
        int year = 2021 + (i % 4);
        aa_music(type, cat, mth, cmp, mpr, med, year);
    }

    print_str("\nFine arts...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pnt = 42 + (i * 10);
        int scl = 28 + (i * 7);
        int cal = 18 + (i * 4);
        int ads = 10 + (i * 2);
        int year = 2022 + (i % 3);
        aa_fine_art(type, cat, pnt, scl, cal, ads, year);
    }

    print_str("\nDrama & film...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int drm = 35 + (i * 8);
        int flm = 25 + (i * 6);
        int brd = 15 + (i * 3);
        int ani = 10 + (i * 2);
        int year = 2023 + (i % 2);
        aa_drama(type, cat, drm, flm, brd, ani, year);
    }

    print_str("\nDance...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int dth = 30 + (i * 7);
        int dpr = 22 + (i * 5);
        int dch = 12 + (i * 3);
        int ded = 8 + (i * 2);
        int year = 2024;
        aa_dance(type, cat, dth, dpr, dch, ded, year);
    }

    print_str("\nTheory report...\n");
    aa_theory_report();

    print_str("\nMusic report...\n");
    aa_music_report();

    print_str("\nFull report...\n");
    aa_full_report();

    print_str("\nFinal state...\n");
    aa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
