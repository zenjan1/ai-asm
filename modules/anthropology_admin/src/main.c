/* anthropology_admin: Anthropology administration system (v1.0)
 * Physical anthropology, cultural anthropology, archaeology, linguistics, folklore
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

#define MAX_PHYS_AN     16
#define MAX_CULT_AN     14
#define MAX_ARCHAE      12
#define MAX_LINGUI      10
#define MAX_FOLKLO      10

typedef struct {
    int    pa_id;
    int    pa_type;
    int    pa_category;
    int    hum_evo;
    int    hum_gen;
    int    anthro_mea;
    int    paleo;
    int    year;
    int    active;
} phys_an_t;

typedef struct {
    int    ca_id;
    int    ca_type;
    int    ca_category;
    int    cul_theory;
    int    ethnogr;
    int    cul_chg;
    int    cross_cul;
    int    year;
    int    active;
} cult_an_t;

typedef struct {
    int    ar_id;
    int    ar_type;
    int    ar_category;
    int    arch_theory;
    int    field_arch;
    int    heritage;
    int    museum;
    int    year;
    int    active;
} archae_t;

typedef struct {
    int    li_id;
    int    li_type;
    int    li_category;
    int    phonetics;
    int    grammar;
    int    semantics;
    int    socio_lin;
    int    year;
    int    active;
} lingu_i_t;

typedef struct {
    int    fl_id;
    int    fl_type;
    int    fl_category;
    int    folk_theory;
    int    folk_lit;
    int    folk_art;
    int    folk_belief;
    int    year;
    int    active;
} folklo_t;

typedef struct {
    int    n_phys_an;
    int    n_cult_an;
    int    n_archae;
    int    n_lingu_i;
    int    n_folklo;
    int    total_hum_evo;
    int    total_cul_theory;
    int    total_arch_theory;
    int    total_phonetics;
    int    total_folk_theory;
} ant_state_t;

static phys_an_t phys_ans[MAX_PHYS_AN];
static cult_an_t cult_ans[MAX_CULT_AN];
static archae_t archae_s[MAX_ARCHAE];
static lingu_i_t lingu_is[MAX_LINGUI];
static folklo_t folklos[MAX_FOLKLO];
static ant_state_t ant;

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

int ant_init(void) {
    if (initialized) return -1;
    ant.n_phys_an = 0; ant.n_cult_an = 0; ant.n_archae = 0;
    ant.n_lingu_i = 0; ant.n_folklo = 0;
    ant.total_hum_evo = 0; ant.total_cul_theory = 0;
    ant.total_arch_theory = 0; ant.total_phonetics = 0;
    ant.total_folk_theory = 0;
    for (int i = 0; i < MAX_PHYS_AN; i++) phys_ans[i].active = 0;
    for (int i = 0; i < MAX_CULT_AN; i++) cult_ans[i].active = 0;
    for (int i = 0; i < MAX_ARCHAE; i++) archae_s[i].active = 0;
    for (int i = 0; i < MAX_LINGUI; i++) lingu_is[i].active = 0;
    for (int i = 0; i < MAX_FOLKLO; i++) folklos[i].active = 0;
    initialized = 1;
    print_str("[ANT] Anthropology initialized\n");
    return 0;
}

int ant_phys_an(int pt_type, int cat, int hev, int hgn, int amr, int pal, int year) {
    if (ant.n_phys_an >= MAX_PHYS_AN) return -1;
    phys_an_t* p = &phys_ans[ant.n_phys_an];
    p->pa_id = ant.n_phys_an;
    p->pa_type = pt_type;
    p->pa_category = cat;
    p->hum_evo = hev;
    p->hum_gen = hgn;
    p->anthro_mea = amr;
    p->paleo = pal;
    p->year = year;
    p->active = 1;
    ant.total_hum_evo += hev;
    ant.n_phys_an++;
    print_str("[ANT] Phys an "); print_int(ant.n_phys_an - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" hev="); print_int(hev);
    print_str(" hgn="); print_int(hgn);
    print_str(" amr="); print_int(amr);
    print_str(" pal="); print_int(pal); print_str("\n");
    return ant.n_phys_an - 1;
}

int ant_cult_an(int ct_type, int cat, int cth, int eth, int cch, int crc, int year) {
    if (ant.n_cult_an >= MAX_CULT_AN) return -1;
    cult_an_t* c = &cult_ans[ant.n_cult_an];
    c->ca_id = ant.n_cult_an;
    c->ca_type = ct_type;
    c->ca_category = cat;
    c->cul_theory = cth;
    c->ethnogr = eth;
    c->cul_chg = cch;
    c->cross_cul = crc;
    c->year = year;
    c->active = 1;
    ant.total_cul_theory += cth;
    ant.n_cult_an++;
    print_str("[ANT] Cult an "); print_int(ant.n_cult_an - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" cth="); print_int(cth);
    print_str(" eth="); print_int(eth);
    print_str(" cch="); print_int(cch);
    print_str(" crc="); print_int(crc); print_str("\n");
    return ant.n_cult_an - 1;
}

int ant_archae(int at_type, int cat, int ath, int far, int hrg, int mus, int year) {
    if (ant.n_archae >= MAX_ARCHAE) return -1;
    archae_t* a = &archae_s[ant.n_archae];
    a->ar_id = ant.n_archae;
    a->ar_type = at_type;
    a->ar_category = cat;
    a->arch_theory = ath;
    a->field_arch = far;
    a->heritage = hrg;
    a->museum = mus;
    a->year = year;
    a->active = 1;
    ant.total_arch_theory += ath;
    ant.n_archae++;
    print_str("[ANT] Archae "); print_int(ant.n_archae - 1);
    print_str(" type="); print_int(at_type);
    print_str(" cat="); print_int(cat);
    print_str(" ath="); print_int(ath);
    print_str(" far="); print_int(far);
    print_str(" hrg="); print_int(hrg);
    print_str(" mus="); print_int(mus); print_str("\n");
    return ant.n_archae - 1;
}

int ant_lingu_i(int lt_type, int cat, int phn, int grm, int sem, int sli, int year) {
    if (ant.n_lingu_i >= MAX_LINGUI) return -1;
    lingu_i_t* l = &lingu_is[ant.n_lingu_i];
    l->li_id = ant.n_lingu_i;
    l->li_type = lt_type;
    l->li_category = cat;
    l->phonetics = phn;
    l->grammar = grm;
    l->semantics = sem;
    l->socio_lin = sli;
    l->year = year;
    l->active = 1;
    ant.total_phonetics += phn;
    ant.n_lingu_i++;
    print_str("[ANT] Lingu i "); print_int(ant.n_lingu_i - 1);
    print_str(" type="); print_int(lt_type);
    print_str(" cat="); print_int(cat);
    print_str(" phn="); print_int(phn);
    print_str(" grm="); print_int(grm);
    print_str(" sem="); print_int(sem);
    print_str(" sli="); print_int(sli); print_str("\n");
    return ant.n_lingu_i - 1;
}

int ant_folklo(int ft_type, int cat, int fth, int flt, int fat, int fbf, int year) {
    if (ant.n_folklo >= MAX_FOLKLO) return -1;
    folklo_t* f = &folklos[ant.n_folklo];
    f->fl_id = ant.n_folklo;
    f->fl_type = ft_type;
    f->fl_category = cat;
    f->folk_theory = fth;
    f->folk_lit = flt;
    f->folk_art = fat;
    f->folk_belief = fbf;
    f->year = year;
    f->active = 1;
    ant.total_folk_theory += fth;
    ant.n_folklo++;
    print_str("[ANT] Folklo "); print_int(ant.n_folklo - 1);
    print_str(" type="); print_int(ft_type);
    print_str(" cat="); print_int(cat);
    print_str(" fth="); print_int(fth);
    print_str(" flt="); print_int(flt);
    print_str(" fat="); print_int(fat);
    print_str(" fbf="); print_int(fbf); print_str("\n");
    return ant.n_folklo - 1;
}

void ant_phys_report(void) {
    print_str("[ANT] Physical anthropology report:\n");
    print_str("  Physical categories: "); print_int(ant.n_phys_an); print_str("\n");
    print_str("  Total human evolution: "); print_int(ant.total_hum_evo); print_str("\n");
}

void ant_cult_report(void) {
    print_str("[ANT] Cultural anthropology report:\n");
    print_str("  Cultural categories: "); print_int(ant.n_cult_an); print_str("\n");
    print_str("  Total cultural theory: "); print_int(ant.total_cul_theory); print_str("\n");
}

void ant_full_report(void) {
    print_str("[ANT] Full report:\n");
    print_str("  Archaeology categories: "); print_int(ant.n_archae); print_str("\n");
    print_str("  Total archaeology theory: "); print_int(ant.total_arch_theory); print_str("\n");
    print_str("  Linguistics categories: "); print_int(ant.n_lingu_i); print_str("\n");
    print_str("  Total phonetics: "); print_int(ant.total_phonetics); print_str("\n");
    print_str("  Folklore categories: "); print_int(ant.n_folklo); print_str("\n");
    print_str("  Total folklore theory: "); print_int(ant.total_folk_theory); print_str("\n");
}

void ant_print_state(void) {
    print_str("[ANT] Pa="); print_int(ant.n_phys_an);
    print_str(" Ca="); print_int(ant.n_cult_an);
    print_str(" Ar="); print_int(ant.n_archae);
    print_str(" Li="); print_int(ant.n_lingu_i);
    print_str(" Fl="); print_int(ant.n_folklo);
    print_str("\n");
}

int main(void) {
    print_str("=== Anthropology Admin Demo ===\n\n");
    ant_init();

    print_str("Physical anthropology...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int hev = 55 + (i * 13);
        int hgn = 40 + (i * 10);
        int amr = 22 + (i * 5);
        int pal = 15 + (i * 3);
        int year = 2020 + (i % 5);
        ant_phys_an(type, cat, hev, hgn, amr, pal, year);
    }

    print_str("\nCultural anthropology...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cth = 48 + (i * 11);
        int eth = 35 + (i * 8);
        int cch = 20 + (i * 4);
        int crc = 12 + (i * 3);
        int year = 2021 + (i % 4);
        ant_cult_an(type, cat, cth, eth, cch, crc, year);
    }

    print_str("\nArchaeology...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ath = 42 + (i * 10);
        int far = 28 + (i * 7);
        int hrg = 18 + (i * 4);
        int mus = 10 + (i * 2);
        int year = 2022 + (i % 3);
        ant_archae(type, cat, ath, far, hrg, mus, year);
    }

    print_str("\nLinguistics...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int phn = 35 + (i * 8);
        int grm = 25 + (i * 6);
        int sem = 15 + (i * 3);
        int sli = 10 + (i * 2);
        int year = 2023 + (i % 2);
        ant_lingu_i(type, cat, phn, grm, sem, sli, year);
    }

    print_str("\nFolklore...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int fth = 30 + (i * 7);
        int flt = 22 + (i * 5);
        int fat = 12 + (i * 3);
        int fbf = 8 + (i * 2);
        int year = 2024;
        ant_folklo(type, cat, fth, flt, fat, fbf, year);
    }

    print_str("\nPhysical report...\n");
    ant_phys_report();

    print_str("\nCultural report...\n");
    ant_cult_report();

    print_str("\nFull report...\n");
    ant_full_report();

    print_str("\nFinal state...\n");
    ant_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
