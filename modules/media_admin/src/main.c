/* media_admin: Media administration system (v1.0)
 * Journalism, communication, advertising, broadcasting, new media
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

#define MAX_JOU_THE     16
#define MAX_COM_THE     14
#define MAX_ADB_THE     12
#define MAX_BRO_BUS     10
#define MAX_NEW_MED     10

typedef struct {
    int    jt_id;
    int    jt_type;
    int    jt_category;
    int    jou_the;
    int    jou_int;
    int    jou_edi;
    int    jou_com;
    int    year;
    int    active;
} jou_the_t;

typedef struct {
    int    ct_id;
    int    ct_type;
    int    ct_category;
    int    com_the;
    int    mas_com;
    int    org_com;
    int    cro_cul;
    int    year;
    int    active;
} com_the_t;

typedef struct {
    int    at_id;
    int    at_type;
    int    at_category;
    int    adb_the;
    int    adb_pla;
    int    adb_cre;
    int    adb_eff;
    int    year;
    int    active;
} adb_the_t;

typedef struct {
    int    bb_id;
    int    bb_type;
    int    bb_category;
    int    rad_bus;
    int    tel_bus;
    int    pro_mak;
    int    med_tec;
    int    year;
    int    active;
} bro_bus_t;

typedef struct {
    int    nm_id;
    int    nm_type;
    int    nm_category;
    int    net_com;
    int    soc_med;
    int    mob_med;
    int    dig_con;
    int    year;
    int    active;
} new_med_t;

typedef struct {
    int    n_jou_the;
    int    n_com_the;
    int    n_adb_the;
    int    n_bro_bus;
    int    n_new_med;
    int    total_jou_the;
    int    total_com_the;
    int    total_adb_the;
    int    total_rad_bus;
    int    total_net_com;
} mda_state_t;

static jou_the_t jou_thes[MAX_JOU_THE];
static com_the_t com_thes[MAX_COM_THE];
static adb_the_t adb_thes[MAX_ADB_THE];
static bro_bus_t bro_buss[MAX_BRO_BUS];
static new_med_t new_meds[MAX_NEW_MED];
static mda_state_t mda;

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

int mda_init(void) {
    if (initialized) return -1;
    mda.n_jou_the = 0; mda.n_com_the = 0; mda.n_adb_the = 0;
    mda.n_bro_bus = 0; mda.n_new_med = 0;
    mda.total_jou_the = 0; mda.total_com_the = 0;
    mda.total_adb_the = 0; mda.total_rad_bus = 0;
    mda.total_net_com = 0;
    for (int i = 0; i < MAX_JOU_THE; i++) jou_thes[i].active = 0;
    for (int i = 0; i < MAX_COM_THE; i++) com_thes[i].active = 0;
    for (int i = 0; i < MAX_ADB_THE; i++) adb_thes[i].active = 0;
    for (int i = 0; i < MAX_BRO_BUS; i++) bro_buss[i].active = 0;
    for (int i = 0; i < MAX_NEW_MED; i++) new_meds[i].active = 0;
    initialized = 1;
    print_str("[MDA] Media initialized\n");
    return 0;
}

int mda_jou_the(int jt_type, int cat, int jth, int jin, int jed, int jco, int year) {
    if (mda.n_jou_the >= MAX_JOU_THE) return -1;
    jou_the_t* j = &jou_thes[mda.n_jou_the];
    j->jt_id = mda.n_jou_the;
    j->jt_type = jt_type;
    j->jt_category = cat;
    j->jou_the = jth;
    j->jou_int = jin;
    j->jou_edi = jed;
    j->jou_com = jco;
    j->year = year;
    j->active = 1;
    mda.total_jou_the += jth;
    mda.n_jou_the++;
    print_str("[MDA] Jou the "); print_int(mda.n_jou_the - 1);
    print_str(" type="); print_int(jt_type);
    print_str(" cat="); print_int(cat);
    print_str(" jth="); print_int(jth);
    print_str(" jin="); print_int(jin);
    print_str(" jed="); print_int(jed);
    print_str(" jco="); print_int(jco); print_str("\n");
    return mda.n_jou_the - 1;
}

int mda_com_the(int ct_type, int cat, int cth, int mco, int oco, int cco, int year) {
    if (mda.n_com_the >= MAX_COM_THE) return -1;
    com_the_t* c = &com_thes[mda.n_com_the];
    c->ct_id = mda.n_com_the;
    c->ct_type = ct_type;
    c->ct_category = cat;
    c->com_the = cth;
    c->mas_com = mco;
    c->org_com = oco;
    c->cro_cul = cco;
    c->year = year;
    c->active = 1;
    mda.total_com_the += cth;
    mda.n_com_the++;
    print_str("[MDA] Com the "); print_int(mda.n_com_the - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" cth="); print_int(cth);
    print_str(" mco="); print_int(mco);
    print_str(" oco="); print_int(oco);
    print_str(" cco="); print_int(cco); print_str("\n");
    return mda.n_com_the - 1;
}

int mda_adb_the(int at_type, int cat, int ath, int apl, int acr, int aef, int year) {
    if (mda.n_adb_the >= MAX_ADB_THE) return -1;
    adb_the_t* a = &adb_thes[mda.n_adb_the];
    a->at_id = mda.n_adb_the;
    a->at_type = at_type;
    a->at_category = cat;
    a->adb_the = ath;
    a->adb_pla = apl;
    a->adb_cre = acr;
    a->adb_eff = aef;
    a->year = year;
    a->active = 1;
    mda.total_adb_the += ath;
    mda.n_adb_the++;
    print_str("[MDA] Adb the "); print_int(mda.n_adb_the - 1);
    print_str(" type="); print_int(at_type);
    print_str(" cat="); print_int(cat);
    print_str(" ath="); print_int(ath);
    print_str(" apl="); print_int(apl);
    print_str(" acr="); print_int(acr);
    print_str(" aef="); print_int(aef); print_str("\n");
    return mda.n_adb_the - 1;
}

int mda_bro_bus(int bt_type, int cat, int rbs, int tbs, int pmk, int mte, int year) {
    if (mda.n_bro_bus >= MAX_BRO_BUS) return -1;
    bro_bus_t* b = &bro_buss[mda.n_bro_bus];
    b->bb_id = mda.n_bro_bus;
    b->bb_type = bt_type;
    b->bb_category = cat;
    b->rad_bus = rbs;
    b->tel_bus = tbs;
    b->pro_mak = pmk;
    b->med_tec = mte;
    b->year = year;
    b->active = 1;
    mda.total_rad_bus += rbs;
    mda.n_bro_bus++;
    print_str("[MDA] Bro bus "); print_int(mda.n_bro_bus - 1);
    print_str(" type="); print_int(bt_type);
    print_str(" cat="); print_int(cat);
    print_str(" rbs="); print_int(rbs);
    print_str(" tbs="); print_int(tbs);
    print_str(" pmk="); print_int(pmk);
    print_str(" mte="); print_int(mte); print_str("\n");
    return mda.n_bro_bus - 1;
}

int mda_new_med(int nt_type, int cat, int nco, int sme, int mme, int dco, int year) {
    if (mda.n_new_med >= MAX_NEW_MED) return -1;
    new_med_t* n = &new_meds[mda.n_new_med];
    n->nm_id = mda.n_new_med;
    n->nm_type = nt_type;
    n->nm_category = cat;
    n->net_com = nco;
    n->soc_med = sme;
    n->mob_med = mme;
    n->dig_con = dco;
    n->year = year;
    n->active = 1;
    mda.total_net_com += nco;
    mda.n_new_med++;
    print_str("[MDA] New med "); print_int(mda.n_new_med - 1);
    print_str(" type="); print_int(nt_type);
    print_str(" cat="); print_int(cat);
    print_str(" nco="); print_int(nco);
    print_str(" sme="); print_int(sme);
    print_str(" mme="); print_int(mme);
    print_str(" dco="); print_int(dco); print_str("\n");
    return mda.n_new_med - 1;
}

void mda_journalism_report(void) {
    print_str("[MDA] Journalism report:\n");
    print_str("  Journalism categories: "); print_int(mda.n_jou_the); print_str("\n");
    print_str("  Total journalism theory: "); print_int(mda.total_jou_the); print_str("\n");
}

void mda_communication_report(void) {
    print_str("[MDA] Communication report:\n");
    print_str("  Communication categories: "); print_int(mda.n_com_the); print_str("\n");
    print_str("  Total communication theory: "); print_int(mda.total_com_the); print_str("\n");
}

void mda_full_report(void) {
    print_str("[MDA] Full report:\n");
    print_str("  Advertising categories: "); print_int(mda.n_adb_the); print_str("\n");
    print_str("  Total advertising theory: "); print_int(mda.total_adb_the); print_str("\n");
    print_str("  Broadcasting categories: "); print_int(mda.n_bro_bus); print_str("\n");
    print_str("  Total radio business: "); print_int(mda.total_rad_bus); print_str("\n");
    print_str("  New media categories: "); print_int(mda.n_new_med); print_str("\n");
    print_str("  Total network communication: "); print_int(mda.total_net_com); print_str("\n");
}

void mda_print_state(void) {
    print_str("[MDA] Jt="); print_int(mda.n_jou_the);
    print_str(" Ct="); print_int(mda.n_com_the);
    print_str(" At="); print_int(mda.n_adb_the);
    print_str(" Bb="); print_int(mda.n_bro_bus);
    print_str(" Nm="); print_int(mda.n_new_med);
    print_str("\n");
}

int main(void) {
    print_str("=== Media Admin Demo ===\n\n");
    mda_init();

    print_str("Journalism...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int jth = 55 + (i * 13);
        int jin = 40 + (i * 10);
        int jed = 22 + (i * 5);
        int jco = 15 + (i * 3);
        int year = 2020 + (i % 5);
        mda_jou_the(type, cat, jth, jin, jed, jco, year);
    }

    print_str("\nCommunication...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cth = 48 + (i * 11);
        int mco = 35 + (i * 8);
        int oco = 20 + (i * 4);
        int cco = 12 + (i * 3);
        int year = 2021 + (i % 4);
        mda_com_the(type, cat, cth, mco, oco, cco, year);
    }

    print_str("\nAdvertising...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ath = 42 + (i * 10);
        int apl = 28 + (i * 7);
        int acr = 18 + (i * 4);
        int aef = 10 + (i * 2);
        int year = 2022 + (i % 3);
        mda_adb_the(type, cat, ath, apl, acr, aef, year);
    }

    print_str("\nBroadcasting...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int rbs = 35 + (i * 8);
        int tbs = 25 + (i * 6);
        int pmk = 15 + (i * 3);
        int mte = 10 + (i * 2);
        int year = 2023 + (i % 2);
        mda_bro_bus(type, cat, rbs, tbs, pmk, mte, year);
    }

    print_str("\nNew media...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int nco = 30 + (i * 7);
        int sme = 22 + (i * 5);
        int mme = 12 + (i * 3);
        int dco = 8 + (i * 2);
        int year = 2024;
        mda_new_med(type, cat, nco, sme, mme, dco, year);
    }

    print_str("\nJournalism report...\n");
    mda_journalism_report();

    print_str("\nCommunication report...\n");
    mda_communication_report();

    print_str("\nFull report...\n");
    mda_full_report();

    print_str("\nFinal state...\n");
    mda_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
