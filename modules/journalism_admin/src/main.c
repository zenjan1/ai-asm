/* journalism_admin: Journalism administration system (v1.0)
 * Journalism theory, practice, broadcasting, communication, advertising
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

#define MAX_JOUR_TH     16
#define MAX_JOUR_PR     14
#define MAX_BROAD       12
#define MAX_COMMUN      10
#define MAX_ADVERT      10

typedef struct {
    int    jt_id;
    int    jt_type;
    int    jt_category;
    int    jour_pri;
    int    comm_theory;
    int    media_stu;
    int    opinion;
    int    year;
    int    active;
} jour_th_t;

typedef struct {
    int    jp_id;
    int    jp_type;
    int    jp_category;
    int    jr_interv;
    int    jr_write;
    int    jr_edit;
    int    jr_comment;
    int    year;
    int    active;
} jour_pr_t;

typedef struct {
    int    br_id;
    int    br_type;
    int    br_category;
    int    broad_st;
    int    tv_st;
    int    prog_prod;
    int    anchor;
    int    year;
    int    active;
} broad_t;

typedef struct {
    int    cm_id;
    int    cm_type;
    int    cm_category;
    int    mass_com;
    int    org_com;
    int    cross_com;
    int    new_media;
    int    year;
    int    active;
} commun_t;

typedef struct {
    int    ad_id;
    int    ad_type;
    int    ad_category;
    int    adv_theory;
    int    adv_plan;
    int    adv_creat;
    int    adv_law;
    int    year;
    int    active;
} advert_t;

typedef struct {
    int    n_jour_th;
    int    n_jour_pr;
    int    n_broad;
    int    n_commun;
    int    n_advert;
    int    total_jour_pri;
    int    total_jr_interv;
    int    total_broad_st;
    int    total_mass_com;
    int    total_adv_theory;
} ja_state_t;

static jour_th_t jour_ths[MAX_JOUR_TH];
static jour_pr_t jour_prs[MAX_JOUR_PR];
static broad_t broads[MAX_BROAD];
static commun_t communs[MAX_COMMUN];
static advert_t adverts[MAX_ADVERT];
static ja_state_t ja;

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

int ja_init(void) {
    if (initialized) return -1;
    ja.n_jour_th = 0; ja.n_jour_pr = 0; ja.n_broad = 0;
    ja.n_commun = 0; ja.n_advert = 0;
    ja.total_jour_pri = 0; ja.total_jr_interv = 0;
    ja.total_broad_st = 0; ja.total_mass_com = 0;
    ja.total_adv_theory = 0;
    for (int i = 0; i < MAX_JOUR_TH; i++) jour_ths[i].active = 0;
    for (int i = 0; i < MAX_JOUR_PR; i++) jour_prs[i].active = 0;
    for (int i = 0; i < MAX_BROAD; i++) broads[i].active = 0;
    for (int i = 0; i < MAX_COMMUN; i++) communs[i].active = 0;
    for (int i = 0; i < MAX_ADVERT; i++) adverts[i].active = 0;
    initialized = 1;
    print_str("[JA] Journalism initialized\n");
    return 0;
}

int ja_jour_th(int jt_type, int cat, int jrp, int cmt, int mds, int opn, int year) {
    if (ja.n_jour_th >= MAX_JOUR_TH) return -1;
    jour_th_t* j = &jour_ths[ja.n_jour_th];
    j->jt_id = ja.n_jour_th;
    j->jt_type = jt_type;
    j->jt_category = cat;
    j->jour_pri = jrp;
    j->comm_theory = cmt;
    j->media_stu = mds;
    j->opinion = opn;
    j->year = year;
    j->active = 1;
    ja.total_jour_pri += jrp;
    ja.n_jour_th++;
    print_str("[JA] Jour th "); print_int(ja.n_jour_th - 1);
    print_str(" type="); print_int(jt_type);
    print_str(" cat="); print_int(cat);
    print_str(" jrp="); print_int(jrp);
    print_str(" cmt="); print_int(cmt);
    print_str(" mds="); print_int(mds);
    print_str(" opn="); print_int(opn); print_str("\n");
    return ja.n_jour_th - 1;
}

int ja_jour_pr(int pt_type, int cat, int jrv, int jrw, int jre, int jrc, int year) {
    if (ja.n_jour_pr >= MAX_JOUR_PR) return -1;
    jour_pr_t* j = &jour_prs[ja.n_jour_pr];
    j->jp_id = ja.n_jour_pr;
    j->jp_type = pt_type;
    j->jp_category = cat;
    j->jr_interv = jrv;
    j->jr_write = jrw;
    j->jr_edit = jre;
    j->jr_comment = jrc;
    j->year = year;
    j->active = 1;
    ja.total_jr_interv += jrv;
    ja.n_jour_pr++;
    print_str("[JA] Jour pr "); print_int(ja.n_jour_pr - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" jrv="); print_int(jrv);
    print_str(" jrw="); print_int(jrw);
    print_str(" jre="); print_int(jre);
    print_str(" jrc="); print_int(jrc); print_str("\n");
    return ja.n_jour_pr - 1;
}

int ja_broad(int bt_type, int cat, int brs, int tvs, int prp, int anc, int year) {
    if (ja.n_broad >= MAX_BROAD) return -1;
    broad_t* b = &broads[ja.n_broad];
    b->br_id = ja.n_broad;
    b->br_type = bt_type;
    b->br_category = cat;
    b->broad_st = brs;
    b->tv_st = tvs;
    b->prog_prod = prp;
    b->anchor = anc;
    b->year = year;
    b->active = 1;
    ja.total_broad_st += brs;
    ja.n_broad++;
    print_str("[JA] Broad "); print_int(ja.n_broad - 1);
    print_str(" type="); print_int(bt_type);
    print_str(" cat="); print_int(cat);
    print_str(" brs="); print_int(brs);
    print_str(" tvs="); print_int(tvs);
    print_str(" prp="); print_int(prp);
    print_str(" anc="); print_int(anc); print_str("\n");
    return ja.n_broad - 1;
}

int ja_commun(int ct_type, int cat, int msc, int orc, int crc, int nwm, int year) {
    if (ja.n_commun >= MAX_COMMUN) return -1;
    commun_t* c = &communs[ja.n_commun];
    c->cm_id = ja.n_commun;
    c->cm_type = ct_type;
    c->cm_category = cat;
    c->mass_com = msc;
    c->org_com = orc;
    c->cross_com = crc;
    c->new_media = nwm;
    c->year = year;
    c->active = 1;
    ja.total_mass_com += msc;
    ja.n_commun++;
    print_str("[JA] Commun "); print_int(ja.n_commun - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" msc="); print_int(msc);
    print_str(" orc="); print_int(orc);
    print_str(" crc="); print_int(crc);
    print_str(" nwm="); print_int(nwm); print_str("\n");
    return ja.n_commun - 1;
}

int ja_advert(int at_type, int cat, int avt, int avp, int avc, int avl, int year) {
    if (ja.n_advert >= MAX_ADVERT) return -1;
    advert_t* a = &adverts[ja.n_advert];
    a->ad_id = ja.n_advert;
    a->ad_type = at_type;
    a->ad_category = cat;
    a->adv_theory = avt;
    a->adv_plan = avp;
    a->adv_creat = avc;
    a->adv_law = avl;
    a->year = year;
    a->active = 1;
    ja.total_adv_theory += avt;
    ja.n_advert++;
    print_str("[JA] Advert "); print_int(ja.n_advert - 1);
    print_str(" type="); print_int(at_type);
    print_str(" cat="); print_int(cat);
    print_str(" avt="); print_int(avt);
    print_str(" avp="); print_int(avp);
    print_str(" avc="); print_int(avc);
    print_str(" avl="); print_int(avl); print_str("\n");
    return ja.n_advert - 1;
}

void ja_theory_report(void) {
    print_str("[JA] Journalism theory report:\n");
    print_str("  Theory categories: "); print_int(ja.n_jour_th); print_str("\n");
    print_str("  Total journalism principles: "); print_int(ja.total_jour_pri); print_str("\n");
}

void ja_practice_report(void) {
    print_str("[JA] Journalism practice report:\n");
    print_str("  Practice categories: "); print_int(ja.n_jour_pr); print_str("\n");
    print_str("  Total interview: "); print_int(ja.total_jr_interv); print_str("\n");
}

void ja_full_report(void) {
    print_str("[JA] Full report:\n");
    print_str("  Broadcasting categories: "); print_int(ja.n_broad); print_str("\n");
    print_str("  Total broadcasting study: "); print_int(ja.total_broad_st); print_str("\n");
    print_str("  Communication categories: "); print_int(ja.n_commun); print_str("\n");
    print_str("  Total mass communication: "); print_int(ja.total_mass_com); print_str("\n");
    print_str("  Advertising categories: "); print_int(ja.n_advert); print_str("\n");
    print_str("  Total advertising theory: "); print_int(ja.total_adv_theory); print_str("\n");
}

void ja_print_state(void) {
    print_str("[JA] Th="); print_int(ja.n_jour_th);
    print_str(" Pr="); print_int(ja.n_jour_pr);
    print_str(" Br="); print_int(ja.n_broad);
    print_str(" Cm="); print_int(ja.n_commun);
    print_str(" Ad="); print_int(ja.n_advert);
    print_str("\n");
}

int main(void) {
    print_str("=== Journalism Admin Demo ===\n\n");
    ja_init();

    print_str("Journalism theory...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int jrp = 55 + (i * 13);
        int cmt = 40 + (i * 10);
        int mds = 22 + (i * 5);
        int opn = 15 + (i * 3);
        int year = 2020 + (i % 5);
        ja_jour_th(type, cat, jrp, cmt, mds, opn, year);
    }

    print_str("\nJournalism practice...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int jrv = 48 + (i * 11);
        int jrw = 35 + (i * 8);
        int jre = 20 + (i * 4);
        int jrc = 12 + (i * 3);
        int year = 2021 + (i % 4);
        ja_jour_pr(type, cat, jrv, jrw, jre, jrc, year);
    }

    print_str("\nBroadcasting...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int brs = 42 + (i * 10);
        int tvs = 28 + (i * 7);
        int prp = 18 + (i * 4);
        int anc = 10 + (i * 2);
        int year = 2022 + (i % 3);
        ja_broad(type, cat, brs, tvs, prp, anc, year);
    }

    print_str("\nCommunication...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int msc = 35 + (i * 8);
        int orc = 25 + (i * 6);
        int crc = 15 + (i * 3);
        int nwm = 10 + (i * 2);
        int year = 2023 + (i % 2);
        ja_commun(type, cat, msc, orc, crc, nwm, year);
    }

    print_str("\nAdvertising...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int avt = 30 + (i * 7);
        int avp = 22 + (i * 5);
        int avc = 12 + (i * 3);
        int avl = 8 + (i * 2);
        int year = 2024;
        ja_advert(type, cat, avt, avp, avc, avl, year);
    }

    print_str("\nTheory report...\n");
    ja_theory_report();

    print_str("\nPractice report...\n");
    ja_practice_report();

    print_str("\nFull report...\n");
    ja_full_report();

    print_str("\nFinal state...\n");
    ja_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
