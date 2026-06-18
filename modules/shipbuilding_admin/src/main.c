/* shipbuilding_admin: Shipbuilding administration system (v1.0)
 * Ship construction, welding technology, outfitting, launching, testing
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

#define MAX_CONSTRUCT   16
#define MAX_WELDING     14
#define MAX_OUTFIT      12
#define MAX_LAUNCH      10
#define MAX_TESTING     10

typedef struct {
    int    cn_id;
    int    cn_type;
    int    cn_category;
    int    lofting;
    int    cutting;
    int    assembly;
    int    year;
    int    active;
} construct_t;

typedef struct {
    int    wd_id;
    int    wd_type;
    int    wd_category;
    int    manual_arc;
    int    gas_shield;
    int    submerged_arc;
    int    year;
    int    active;
} welding_t;

typedef struct {
    int    of_id;
    int    of_type;
    int    of_category;
    int    hull_outfit;
    int    machine_outfit;
    int    electric_outfit;
    int    year;
    int    active;
} outfit_t;

typedef struct {
    int    ln_id;
    int    ln_type;
    int    ln_category;
    int    gravity_launch;
    int    floating_launch;
    int    mechanized;
    int    year;
    int    active;
} launch_t;

typedef struct {
    int    ts_id;
    int    ts_type;
    int    ts_category;
    int    inclining_test;
    int    sea_trial;
    int    mooring_test;
    int    year;
    int    active;
} testing_t;

typedef struct {
    int    n_construct;
    int    n_welding;
    int    n_outfit;
    int    n_launch;
    int    n_testing;
    int    total_lofting;
    int    total_manual_arc;
    int    total_hull_outfit;
    int    total_gravity_launch;
    int    total_inclining_test;
} sba_state_t;

static construct_t constructs[MAX_CONSTRUCT];
static welding_t weldings[MAX_WELDING];
static outfit_t outfits[MAX_OUTFIT];
static launch_t launches[MAX_LAUNCH];
static testing_t testings[MAX_TESTING];
static sba_state_t sba;

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

int sba_init(void) {
    if (initialized) return -1;
    sba.n_construct = 0; sba.n_welding = 0; sba.n_outfit = 0;
    sba.n_launch = 0; sba.n_testing = 0;
    sba.total_lofting = 0; sba.total_manual_arc = 0;
    sba.total_hull_outfit = 0; sba.total_gravity_launch = 0;
    sba.total_inclining_test = 0;
    for (int i = 0; i < MAX_CONSTRUCT; i++) constructs[i].active = 0;
    for (int i = 0; i < MAX_WELDING; i++) weldings[i].active = 0;
    for (int i = 0; i < MAX_OUTFIT; i++) outfits[i].active = 0;
    for (int i = 0; i < MAX_LAUNCH; i++) launches[i].active = 0;
    for (int i = 0; i < MAX_TESTING; i++) testings[i].active = 0;
    initialized = 1;
    print_str("[SBA] Shipbuilding initialized\n");
    return 0;
}

int sba_construct(int ct_type, int cat, int lft, int cut, int asm_v, int year) {
    if (sba.n_construct >= MAX_CONSTRUCT) return -1;
    construct_t* c = &constructs[sba.n_construct];
    c->cn_id = sba.n_construct;
    c->cn_type = ct_type;
    c->cn_category = cat;
    c->lofting = lft;
    c->cutting = cut;
    c->assembly = asm_v;
    c->year = year;
    c->active = 1;
    sba.total_lofting += lft;
    sba.n_construct++;
    print_str("[SBA] Construct "); print_int(sba.n_construct - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" lft="); print_int(lft);
    print_str(" cut="); print_int(cut);
    print_str(" asm="); print_int(asm_v); print_str("\n");
    return sba.n_construct - 1;
}

int sba_welding(int wd_type, int cat, int mna, int gsh, int sar, int year) {
    if (sba.n_welding >= MAX_WELDING) return -1;
    welding_t* w = &weldings[sba.n_welding];
    w->wd_id = sba.n_welding;
    w->wd_type = wd_type;
    w->wd_category = cat;
    w->manual_arc = mna;
    w->gas_shield = gsh;
    w->submerged_arc = sar;
    w->year = year;
    w->active = 1;
    sba.total_manual_arc += mna;
    sba.n_welding++;
    print_str("[SBA] Welding "); print_int(sba.n_welding - 1);
    print_str(" type="); print_int(wd_type);
    print_str(" cat="); print_int(cat);
    print_str(" mna="); print_int(mna);
    print_str(" gsh="); print_int(gsh);
    print_str(" sar="); print_int(sar); print_str("\n");
    return sba.n_welding - 1;
}

int sba_outfit(int of_type, int cat, int hlo, int mch, int elc, int year) {
    if (sba.n_outfit >= MAX_OUTFIT) return -1;
    outfit_t* o = &outfits[sba.n_outfit];
    o->of_id = sba.n_outfit;
    o->of_type = of_type;
    o->of_category = cat;
    o->hull_outfit = hlo;
    o->machine_outfit = mch;
    o->electric_outfit = elc;
    o->year = year;
    o->active = 1;
    sba.total_hull_outfit += hlo;
    sba.n_outfit++;
    print_str("[SBA] Outfit "); print_int(sba.n_outfit - 1);
    print_str(" type="); print_int(of_type);
    print_str(" cat="); print_int(cat);
    print_str(" hlo="); print_int(hlo);
    print_str(" mch="); print_int(mch);
    print_str(" elc="); print_int(elc); print_str("\n");
    return sba.n_outfit - 1;
}

int sba_launch(int ln_type, int cat, int grv, int flt, int mch_l, int year) {
    if (sba.n_launch >= MAX_LAUNCH) return -1;
    launch_t* l = &launches[sba.n_launch];
    l->ln_id = sba.n_launch;
    l->ln_type = ln_type;
    l->ln_category = cat;
    l->gravity_launch = grv;
    l->floating_launch = flt;
    l->mechanized = mch_l;
    l->year = year;
    l->active = 1;
    sba.total_gravity_launch += grv;
    sba.n_launch++;
    print_str("[SBA] Launch "); print_int(sba.n_launch - 1);
    print_str(" type="); print_int(ln_type);
    print_str(" cat="); print_int(cat);
    print_str(" grv="); print_int(grv);
    print_str(" flt="); print_int(flt);
    print_str(" mch="); print_int(mch_l); print_str("\n");
    return sba.n_launch - 1;
}

int sba_testing(int ts_type, int cat, int inc, int sea, int mor, int year) {
    if (sba.n_testing >= MAX_TESTING) return -1;
    testing_t* t = &testings[sba.n_testing];
    t->ts_id = sba.n_testing;
    t->ts_type = ts_type;
    t->ts_category = cat;
    t->inclining_test = inc;
    t->sea_trial = sea;
    t->mooring_test = mor;
    t->year = year;
    t->active = 1;
    sba.total_inclining_test += inc;
    sba.n_testing++;
    print_str("[SBA] Testing "); print_int(sba.n_testing - 1);
    print_str(" type="); print_int(ts_type);
    print_str(" cat="); print_int(cat);
    print_str(" inc="); print_int(inc);
    print_str(" sea="); print_int(sea);
    print_str(" mor="); print_int(mor); print_str("\n");
    return sba.n_testing - 1;
}

void sba_construct_report(void) {
    print_str("[SBA] Construction report:\n");
    print_str("  Ship construction categories: "); print_int(sba.n_construct); print_str("\n");
    print_str("  Total lofting: "); print_int(sba.total_lofting); print_str("\n");
}

void sba_welding_report(void) {
    print_str("[SBA] Welding report:\n");
    print_str("  Welding technology categories: "); print_int(sba.n_welding); print_str("\n");
    print_str("  Total manual arc welding: "); print_int(sba.total_manual_arc); print_str("\n");
}

void sba_full_report(void) {
    print_str("[SBA] Full report:\n");
    print_str("  Outfitting categories: "); print_int(sba.n_outfit); print_str("\n");
    print_str("  Total hull outfitting: "); print_int(sba.total_hull_outfit); print_str("\n");
    print_str("  Launching categories: "); print_int(sba.n_launch); print_str("\n");
    print_str("  Total gravity launch: "); print_int(sba.total_gravity_launch); print_str("\n");
    print_str("  Testing categories: "); print_int(sba.n_testing); print_str("\n");
    print_str("  Total inclining test: "); print_int(sba.total_inclining_test); print_str("\n");
}

void sba_print_state(void) {
    print_str("[SBA] Cn="); print_int(sba.n_construct);
    print_str(" Wd="); print_int(sba.n_welding);
    print_str(" Of="); print_int(sba.n_outfit);
    print_str(" Ln="); print_int(sba.n_launch);
    print_str(" Ts="); print_int(sba.n_testing);
    print_str("\n");
}

int main(void) {
    print_str("=== Shipbuilding Admin Demo ===\n\n");
    sba_init();

    print_str("Ship construction...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int lft = 55 + (i * 13);
        int cut = 40 + (i * 10);
        int asm_v = 22 + (i * 5);
        int year = 2020 + (i % 5);
        sba_construct(type, cat, lft, cut, asm_v, year);
    }

    print_str("\nWelding technology...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int mna = 48 + (i * 11);
        int gsh = 35 + (i * 8);
        int sar = 20 + (i * 4);
        int year = 2021 + (i % 4);
        sba_welding(type, cat, mna, gsh, sar, year);
    }

    print_str("\nOutfitting...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int hlo = 42 + (i * 10);
        int mch = 28 + (i * 7);
        int elc = 18 + (i * 4);
        int year = 2022 + (i % 3);
        sba_outfit(type, cat, hlo, mch, elc, year);
    }

    print_str("\nLaunching...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int grv = 35 + (i * 8);
        int flt = 25 + (i * 6);
        int mch_l = 15 + (i * 3);
        int year = 2023 + (i % 2);
        sba_launch(type, cat, grv, flt, mch_l, year);
    }

    print_str("\nTesting...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int inc = 30 + (i * 7);
        int sea = 22 + (i * 5);
        int mor = 12 + (i * 3);
        int year = 2024;
        sba_testing(type, cat, inc, sea, mor, year);
    }

    print_str("\nConstruction report...\n");
    sba_construct_report();

    print_str("\nWelding report...\n");
    sba_welding_report();

    print_str("\nFull report...\n");
    sba_full_report();

    print_str("\nFinal state...\n");
    sba_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
