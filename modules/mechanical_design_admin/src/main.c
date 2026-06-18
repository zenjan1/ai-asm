/* mechanical_design_admin: Mechanical design administration (v1.0)
 * Connections, transmissions, shaft components, springs, lubrication/seals
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

#define MAX_CONN        16
#define MAX_TRANSM      14
#define MAX_SHAFT       12
#define MAX_SPRING      10
#define MAX_LUBE_SEAL   10

typedef struct {
    int    cn_id;
    int    cn_type;
    int    cn_category;
    int    bolt_conn;
    int    key_conn;
    int    pin_conn;
    int    year;
    int    active;
} conn_t;

typedef struct {
    int    tr_id;
    int    tr_type;
    int    tr_category;
    int    belt_drive;
    int    chain_drive;
    int    gear_drive;
    int    year;
    int    active;
} transm_t;

typedef struct {
    int    sh_id;
    int    sh_type;
    int    sh_category;
    int    shaft_des;
    int    bearing_sel;
    int    coupling;
    int    year;
    int    active;
} shaft_t;

typedef struct {
    int    sp_id;
    int    sp_type;
    int    sp_category;
    int    spring_des;
    int    damper;
    int    elastic_cpl;
    int    year;
    int    active;
} spring_t;

typedef struct {
    int    ls_id;
    int    ls_type;
    int    ls_category;
    int    lube_sys;
    int    seal_dev;
    int    mech_seal;
    int    year;
    int    active;
} lube_seal_t;

typedef struct {
    int    n_conn;
    int    n_transm;
    int    n_shaft;
    int    n_spring;
    int    n_lube_seal;
    int    total_bolt_conn;
    int    total_belt_drive;
    int    total_shaft_des;
    int    total_spring_des;
    int    total_lube_sys;
} mda_state_t;

static conn_t conns[MAX_CONN];
static transm_t transms[MAX_TRANSM];
static shaft_t shafts[MAX_SHAFT];
static spring_t springs[MAX_SPRING];
static lube_seal_t lube_seals[MAX_LUBE_SEAL];
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
    mda.n_conn = 0; mda.n_transm = 0; mda.n_shaft = 0;
    mda.n_spring = 0; mda.n_lube_seal = 0;
    mda.total_bolt_conn = 0; mda.total_belt_drive = 0;
    mda.total_shaft_des = 0; mda.total_spring_des = 0;
    mda.total_lube_sys = 0;
    for (int i = 0; i < MAX_CONN; i++) conns[i].active = 0;
    for (int i = 0; i < MAX_TRANSM; i++) transms[i].active = 0;
    for (int i = 0; i < MAX_SHAFT; i++) shafts[i].active = 0;
    for (int i = 0; i < MAX_SPRING; i++) springs[i].active = 0;
    for (int i = 0; i < MAX_LUBE_SEAL; i++) lube_seals[i].active = 0;
    initialized = 1;
    print_str("[MDA] Mechanical design initialized\n");
    return 0;
}

int mda_conn(int ct_type, int cat, int blt, int key, int pin, int year) {
    if (mda.n_conn >= MAX_CONN) return -1;
    conn_t* c = &conns[mda.n_conn];
    c->cn_id = mda.n_conn;
    c->cn_type = ct_type;
    c->cn_category = cat;
    c->bolt_conn = blt;
    c->key_conn = key;
    c->pin_conn = pin;
    c->year = year;
    c->active = 1;
    mda.total_bolt_conn += blt;
    mda.n_conn++;
    print_str("[MDA] Conn "); print_int(mda.n_conn - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" blt="); print_int(blt);
    print_str(" key="); print_int(key);
    print_str(" pin="); print_int(pin); print_str("\n");
    return mda.n_conn - 1;
}

int mda_transm(int tt_type, int cat, int blt_d, int chn, int grd, int year) {
    if (mda.n_transm >= MAX_TRANSM) return -1;
    transm_t* t = &transms[mda.n_transm];
    t->tr_id = mda.n_transm;
    t->tr_type = tt_type;
    t->tr_category = cat;
    t->belt_drive = blt_d;
    t->chain_drive = chn;
    t->gear_drive = grd;
    t->year = year;
    t->active = 1;
    mda.total_belt_drive += blt_d;
    mda.n_transm++;
    print_str("[MDA] Transm "); print_int(mda.n_transm - 1);
    print_str(" type="); print_int(tt_type);
    print_str(" cat="); print_int(cat);
    print_str(" bld="); print_int(blt_d);
    print_str(" chn="); print_int(chn);
    print_str(" grd="); print_int(grd); print_str("\n");
    return mda.n_transm - 1;
}

int mda_shaft(int st_type, int cat, int shd, int brg, int cpl, int year) {
    if (mda.n_shaft >= MAX_SHAFT) return -1;
    shaft_t* s = &shafts[mda.n_shaft];
    s->sh_id = mda.n_shaft;
    s->sh_type = st_type;
    s->sh_category = cat;
    s->shaft_des = shd;
    s->bearing_sel = brg;
    s->coupling = cpl;
    s->year = year;
    s->active = 1;
    mda.total_shaft_des += shd;
    mda.n_shaft++;
    print_str("[MDA] Shaft "); print_int(mda.n_shaft - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" shd="); print_int(shd);
    print_str(" brg="); print_int(brg);
    print_str(" cpl="); print_int(cpl); print_str("\n");
    return mda.n_shaft - 1;
}

int mda_spring(int sp_type, int cat, int spr, int dmp, int elc, int year) {
    if (mda.n_spring >= MAX_SPRING) return -1;
    spring_t* s = &springs[mda.n_spring];
    s->sp_id = mda.n_spring;
    s->sp_type = sp_type;
    s->sp_category = cat;
    s->spring_des = spr;
    s->damper = dmp;
    s->elastic_cpl = elc;
    s->year = year;
    s->active = 1;
    mda.total_spring_des += spr;
    mda.n_spring++;
    print_str("[MDA] Spring "); print_int(mda.n_spring - 1);
    print_str(" type="); print_int(sp_type);
    print_str(" cat="); print_int(cat);
    print_str(" spr="); print_int(spr);
    print_str(" dmp="); print_int(dmp);
    print_str(" elc="); print_int(elc); print_str("\n");
    return mda.n_spring - 1;
}

int mda_lube_seal(int lt_type, int cat, int lbs, int sld, int msl, int year) {
    if (mda.n_lube_seal >= MAX_LUBE_SEAL) return -1;
    lube_seal_t* l = &lube_seals[mda.n_lube_seal];
    l->ls_id = mda.n_lube_seal;
    l->ls_type = lt_type;
    l->ls_category = cat;
    l->lube_sys = lbs;
    l->seal_dev = sld;
    l->mech_seal = msl;
    l->year = year;
    l->active = 1;
    mda.total_lube_sys += lbs;
    mda.n_lube_seal++;
    print_str("[MDA] Lube seal "); print_int(mda.n_lube_seal - 1);
    print_str(" type="); print_int(lt_type);
    print_str(" cat="); print_int(cat);
    print_str(" lbs="); print_int(lbs);
    print_str(" sld="); print_int(sld);
    print_str(" msl="); print_int(msl); print_str("\n");
    return mda.n_lube_seal - 1;
}

void mda_conn_report(void) {
    print_str("[MDA] Connection report:\n");
    print_str("  Connection categories: "); print_int(mda.n_conn); print_str("\n");
    print_str("  Total bolt connections: "); print_int(mda.total_bolt_conn); print_str("\n");
}

void mda_transm_report(void) {
    print_str("[MDA] Transmission report:\n");
    print_str("  Transmission categories: "); print_int(mda.n_transm); print_str("\n");
    print_str("  Total belt drive: "); print_int(mda.total_belt_drive); print_str("\n");
}

void mda_full_report(void) {
    print_str("[MDA] Full report:\n");
    print_str("  Shaft component categories: "); print_int(mda.n_shaft); print_str("\n");
    print_str("  Total shaft design: "); print_int(mda.total_shaft_des); print_str("\n");
    print_str("  Spring categories: "); print_int(mda.n_spring); print_str("\n");
    print_str("  Total spring design: "); print_int(mda.total_spring_des); print_str("\n");
    print_str("  Lubrication/seal categories: "); print_int(mda.n_lube_seal); print_str("\n");
    print_str("  Total lube systems: "); print_int(mda.total_lube_sys); print_str("\n");
}

void mda_print_state(void) {
    print_str("[MDA] Cn="); print_int(mda.n_conn);
    print_str(" Tr="); print_int(mda.n_transm);
    print_str(" Sh="); print_int(mda.n_shaft);
    print_str(" Sp="); print_int(mda.n_spring);
    print_str(" Ls="); print_int(mda.n_lube_seal);
    print_str("\n");
}

int main(void) {
    print_str("=== Mechanical Design Admin Demo ===\n\n");
    mda_init();

    print_str("Connections...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int blt = 55 + (i * 13);
        int key = 40 + (i * 10);
        int pin = 22 + (i * 5);
        int year = 2020 + (i % 5);
        mda_conn(type, cat, blt, key, pin, year);
    }

    print_str("\nTransmissions...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int bld = 48 + (i * 11);
        int chn = 35 + (i * 8);
        int grd = 20 + (i * 4);
        int year = 2021 + (i % 4);
        mda_transm(type, cat, bld, chn, grd, year);
    }

    print_str("\nShaft components...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int shd = 42 + (i * 10);
        int brg = 28 + (i * 7);
        int cpl = 18 + (i * 4);
        int year = 2022 + (i % 3);
        mda_shaft(type, cat, shd, brg, cpl, year);
    }

    print_str("\nSprings...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int spr = 35 + (i * 8);
        int dmp = 25 + (i * 6);
        int elc = 15 + (i * 3);
        int year = 2023 + (i % 2);
        mda_spring(type, cat, spr, dmp, elc, year);
    }

    print_str("\nLubrication/seals...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int lbs = 30 + (i * 7);
        int sld = 22 + (i * 5);
        int msl = 12 + (i * 3);
        int year = 2024;
        mda_lube_seal(type, cat, lbs, sld, msl, year);
    }

    print_str("\nConnection report...\n");
    mda_conn_report();

    print_str("\nTransmission report...\n");
    mda_transm_report();

    print_str("\nFull report...\n");
    mda_full_report();

    print_str("\nFinal state...\n");
    mda_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
