/* arts_admin: Arts administration system (v1.0)
 * Art theory, music, fine arts, drama, dance
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

#define MAX_ART_THE     16
#define MAX_MUS_THE     14
#define MAX_FIN_ART     12
#define MAX_DRA_THE     10
#define MAX_DAN_THE     10

typedef struct {
    int    at_id;
    int    at_type;
    int    at_category;
    int    art_the;
    int    aes_pri;
    int    art_cri;
    int    art_his;
    int    year;
    int    active;
} art_the_t;

typedef struct {
    int    mt_id;
    int    mt_type;
    int    mt_category;
    int    mus_the;
    int    mus_per;
    int    mus_cre;
    int    mus_edu;
    int    year;
    int    active;
} mus_the_t;

typedef struct {
    int    fa_id;
    int    fa_type;
    int    fa_category;
    int    pai_art;
    int    sculp_art;
    int    cal_sea;
    int    des_art;
    int    year;
    int    active;
} fin_art_t;

typedef struct {
    int    dt_id;
    int    dt_type;
    int    dt_category;
    int    dra_the;
    int    ope_art;
    int    tel_art;
    int    dir_art;
    int    year;
    int    active;
} dra_the_t;

typedef struct {
    int    dn_id;
    int    dn_type;
    int    dn_category;
    int    dan_the;
    int    dan_per;
    int    dan_cho;
    int    dan_edu;
    int    year;
    int    active;
} dan_the_t;

typedef struct {
    int    n_art_the;
    int    n_mus_the;
    int    n_fin_art;
    int    n_dra_the;
    int    n_dan_the;
    int    total_art_the;
    int    total_mus_the;
    int    total_pai_art;
    int    total_dra_the;
    int    total_dan_the;
} ara_state_t;

static art_the_t art_thes[MAX_ART_THE];
static mus_the_t mus_thes[MAX_MUS_THE];
static fin_art_t fin_arts[MAX_FIN_ART];
static dra_the_t dra_thes[MAX_DRA_THE];
static dan_the_t dan_thes[MAX_DAN_THE];
static ara_state_t ara;

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

int ara_init(void) {
    if (initialized) return -1;
    ara.n_art_the = 0; ara.n_mus_the = 0; ara.n_fin_art = 0;
    ara.n_dra_the = 0; ara.n_dan_the = 0;
    ara.total_art_the = 0; ara.total_mus_the = 0;
    ara.total_pai_art = 0; ara.total_dra_the = 0;
    ara.total_dan_the = 0;
    for (int i = 0; i < MAX_ART_THE; i++) art_thes[i].active = 0;
    for (int i = 0; i < MAX_MUS_THE; i++) mus_thes[i].active = 0;
    for (int i = 0; i < MAX_FIN_ART; i++) fin_arts[i].active = 0;
    for (int i = 0; i < MAX_DRA_THE; i++) dra_thes[i].active = 0;
    for (int i = 0; i < MAX_DAN_THE; i++) dan_thes[i].active = 0;
    initialized = 1;
    print_str("[ARA] Arts initialized\n");
    return 0;
}

int ara_art_the(int at_type, int cat, int ath, int aep, int acr, int ahi, int year) {
    if (ara.n_art_the >= MAX_ART_THE) return -1;
    art_the_t* a = &art_thes[ara.n_art_the];
    a->at_id = ara.n_art_the;
    a->at_type = at_type;
    a->at_category = cat;
    a->art_the = ath;
    a->aes_pri = aep;
    a->art_cri = acr;
    a->art_his = ahi;
    a->year = year;
    a->active = 1;
    ara.total_art_the += ath;
    ara.n_art_the++;
    print_str("[ARA] Art the "); print_int(ara.n_art_the - 1);
    print_str(" type="); print_int(at_type);
    print_str(" cat="); print_int(cat);
    print_str(" ath="); print_int(ath);
    print_str(" aep="); print_int(aep);
    print_str(" acr="); print_int(acr);
    print_str(" ahi="); print_int(ahi); print_str("\n");
    return ara.n_art_the - 1;
}

int ara_mus_the(int mt_type, int cat, int mth, int mpe, int mcr, int med, int year) {
    if (ara.n_mus_the >= MAX_MUS_THE) return -1;
    mus_the_t* m = &mus_thes[ara.n_mus_the];
    m->mt_id = ara.n_mus_the;
    m->mt_type = mt_type;
    m->mt_category = cat;
    m->mus_the = mth;
    m->mus_per = mpe;
    m->mus_cre = mcr;
    m->mus_edu = med;
    m->year = year;
    m->active = 1;
    ara.total_mus_the += mth;
    ara.n_mus_the++;
    print_str("[ARA] Mus the "); print_int(ara.n_mus_the - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" mth="); print_int(mth);
    print_str(" mpe="); print_int(mpe);
    print_str(" mcr="); print_int(mcr);
    print_str(" med="); print_int(med); print_str("\n");
    return ara.n_mus_the - 1;
}

int ara_fin_art(int ft_type, int cat, int par, int scl, int cas, int dar, int year) {
    if (ara.n_fin_art >= MAX_FIN_ART) return -1;
    fin_art_t* f = &fin_arts[ara.n_fin_art];
    f->fa_id = ara.n_fin_art;
    f->fa_type = ft_type;
    f->fa_category = cat;
    f->pai_art = par;
    f->sculp_art = scl;
    f->cal_sea = cas;
    f->des_art = dar;
    f->year = year;
    f->active = 1;
    ara.total_pai_art += par;
    ara.n_fin_art++;
    print_str("[ARA] Fin art "); print_int(ara.n_fin_art - 1);
    print_str(" type="); print_int(ft_type);
    print_str(" cat="); print_int(cat);
    print_str(" par="); print_int(par);
    print_str(" scl="); print_int(scl);
    print_str(" cas="); print_int(cas);
    print_str(" dar="); print_int(dar); print_str("\n");
    return ara.n_fin_art - 1;
}

int ara_dra_the(int dt_type, int cat, int dth, int oat, int tat, int dir, int year) {
    if (ara.n_dra_the >= MAX_DRA_THE) return -1;
    dra_the_t* d = &dra_thes[ara.n_dra_the];
    d->dt_id = ara.n_dra_the;
    d->dt_type = dt_type;
    d->dt_category = cat;
    d->dra_the = dth;
    d->ope_art = oat;
    d->tel_art = tat;
    d->dir_art = dir;
    d->year = year;
    d->active = 1;
    ara.total_dra_the += dth;
    ara.n_dra_the++;
    print_str("[ARA] Dra the "); print_int(ara.n_dra_the - 1);
    print_str(" type="); print_int(dt_type);
    print_str(" cat="); print_int(cat);
    print_str(" dth="); print_int(dth);
    print_str(" oat="); print_int(oat);
    print_str(" tat="); print_int(tat);
    print_str(" dir="); print_int(dir); print_str("\n");
    return ara.n_dra_the - 1;
}

int ara_dan_the(int dt_type, int cat, int dth, int dpe, int dch, int ded, int year) {
    if (ara.n_dan_the >= MAX_DAN_THE) return -1;
    dan_the_t* d = &dan_thes[ara.n_dan_the];
    d->dn_id = ara.n_dan_the;
    d->dn_type = dt_type;
    d->dn_category = cat;
    d->dan_the = dth;
    d->dan_per = dpe;
    d->dan_cho = dch;
    d->dan_edu = ded;
    d->year = year;
    d->active = 1;
    ara.total_dan_the += dth;
    ara.n_dan_the++;
    print_str("[ARA] Dan the "); print_int(ara.n_dan_the - 1);
    print_str(" type="); print_int(dt_type);
    print_str(" cat="); print_int(cat);
    print_str(" dth="); print_int(dth);
    print_str(" dpe="); print_int(dpe);
    print_str(" dch="); print_int(dch);
    print_str(" ded="); print_int(ded); print_str("\n");
    return ara.n_dan_the - 1;
}

void ara_theory_report(void) {
    print_str("[ARA] Art theory report:\n");
    print_str("  Theory categories: "); print_int(ara.n_art_the); print_str("\n");
    print_str("  Total art theory: "); print_int(ara.total_art_the); print_str("\n");
}

void ara_music_report(void) {
    print_str("[ARA] Music report:\n");
    print_str("  Music categories: "); print_int(ara.n_mus_the); print_str("\n");
    print_str("  Total music theory: "); print_int(ara.total_mus_the); print_str("\n");
}

void ara_full_report(void) {
    print_str("[ARA] Full report:\n");
    print_str("  Fine arts categories: "); print_int(ara.n_fin_art); print_str("\n");
    print_str("  Total painting art: "); print_int(ara.total_pai_art); print_str("\n");
    print_str("  Drama categories: "); print_int(ara.n_dra_the); print_str("\n");
    print_str("  Total drama theory: "); print_int(ara.total_dra_the); print_str("\n");
    print_str("  Dance categories: "); print_int(ara.n_dan_the); print_str("\n");
    print_str("  Total dance theory: "); print_int(ara.total_dan_the); print_str("\n");
}

void ara_print_state(void) {
    print_str("[ARA] At="); print_int(ara.n_art_the);
    print_str(" Mt="); print_int(ara.n_mus_the);
    print_str(" Fa="); print_int(ara.n_fin_art);
    print_str(" Dt="); print_int(ara.n_dra_the);
    print_str(" Dn="); print_int(ara.n_dan_the);
    print_str("\n");
}

int main(void) {
    print_str("=== Arts Admin Demo ===\n\n");
    ara_init();

    print_str("Art theory...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int ath = 55 + (i * 13);
        int aep = 40 + (i * 10);
        int acr = 22 + (i * 5);
        int ahi = 15 + (i * 3);
        int year = 2020 + (i % 5);
        ara_art_the(type, cat, ath, aep, acr, ahi, year);
    }

    print_str("\nMusic...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int mth = 48 + (i * 11);
        int mpe = 35 + (i * 8);
        int mcr = 20 + (i * 4);
        int med = 12 + (i * 3);
        int year = 2021 + (i % 4);
        ara_mus_the(type, cat, mth, mpe, mcr, med, year);
    }

    print_str("\nFine arts...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int par = 42 + (i * 10);
        int scl = 28 + (i * 7);
        int cas = 18 + (i * 4);
        int dar = 10 + (i * 2);
        int year = 2022 + (i % 3);
        ara_fin_art(type, cat, par, scl, cas, dar, year);
    }

    print_str("\nDrama...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int dth = 35 + (i * 8);
        int oat = 25 + (i * 6);
        int tat = 15 + (i * 3);
        int dir = 10 + (i * 2);
        int year = 2023 + (i % 2);
        ara_dra_the(type, cat, dth, oat, tat, dir, year);
    }

    print_str("\nDance...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int dth = 30 + (i * 7);
        int dpe = 22 + (i * 5);
        int dch = 12 + (i * 3);
        int ded = 8 + (i * 2);
        int year = 2024;
        ara_dan_the(type, cat, dth, dpe, dch, ded, year);
    }

    print_str("\nTheory report...\n");
    ara_theory_report();

    print_str("\nMusic report...\n");
    ara_music_report();

    print_str("\nFull report...\n");
    ara_full_report();

    print_str("\nFinal state...\n");
    ara_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
