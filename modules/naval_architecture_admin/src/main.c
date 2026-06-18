/* naval_architecture_admin: Naval architecture administration (v1.0)
 * Ship theory, ship design, ship structure, propulsion, maneuvering
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

#define MAX_SHIP_THEORY  16
#define MAX_SHIP_DESIGN  14
#define MAX_SHIP_STRUCT  12
#define MAX_PROPULSION   10
#define MAX_MANEUVER     10

typedef struct {
    int    st_id;
    int    st_type;
    int    st_category;
    int    buoyancy;
    int    stability;
    int    subdivision;
    int    year;
    int    active;
} ship_theory_t;

typedef struct {
    int    sd_id;
    int    sd_type;
    int    sd_category;
    int    overall_sd;
    int    struct_sd;
    int    system_sd;
    int    year;
    int    active;
} ship_design_t;

typedef struct {
    int    ss_id;
    int    ss_type;
    int    ss_category;
    int    hull_struct;
    int    deck_struct;
    int    bulkhead;
    int    year;
    int    active;
} ship_struct_t;

typedef struct {
    int    pr_id;
    int    pr_type;
    int    pr_category;
    int    propeller_th;
    int    prop_char;
    int    prop_eff;
    int    year;
    int    active;
} propulsion_t;

typedef struct {
    int    mn_id;
    int    mn_type;
    int    mn_category;
    int    course_stab;
    int    turning;
    int    stopping;
    int    year;
    int    active;
} maneuver_t;

typedef struct {
    int    n_ship_theory;
    int    n_ship_design;
    int    n_ship_struct;
    int    n_propulsion;
    int    n_maneuver;
    int    total_buoyancy;
    int    total_overall_sd;
    int    total_hull_struct;
    int    total_propeller_th;
    int    total_course_stab;
} naa_state_t;

static ship_theory_t ship_theories[MAX_SHIP_THEORY];
static ship_design_t ship_designs[MAX_SHIP_DESIGN];
static ship_struct_t ship_structs[MAX_SHIP_STRUCT];
static propulsion_t propulsions[MAX_PROPULSION];
static maneuver_t maneuvers[MAX_MANEUVER];
static naa_state_t naa;

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

int naa_init(void) {
    if (initialized) return -1;
    naa.n_ship_theory = 0; naa.n_ship_design = 0; naa.n_ship_struct = 0;
    naa.n_propulsion = 0; naa.n_maneuver = 0;
    naa.total_buoyancy = 0; naa.total_overall_sd = 0;
    naa.total_hull_struct = 0; naa.total_propeller_th = 0;
    naa.total_course_stab = 0;
    for (int i = 0; i < MAX_SHIP_THEORY; i++) ship_theories[i].active = 0;
    for (int i = 0; i < MAX_SHIP_DESIGN; i++) ship_designs[i].active = 0;
    for (int i = 0; i < MAX_SHIP_STRUCT; i++) ship_structs[i].active = 0;
    for (int i = 0; i < MAX_PROPULSION; i++) propulsions[i].active = 0;
    for (int i = 0; i < MAX_MANEUVER; i++) maneuvers[i].active = 0;
    initialized = 1;
    print_str("[NAA] Naval architecture initialized\n");
    return 0;
}

int naa_ship_theory(int st_type, int cat, int buo, int stb, int sbd, int year) {
    if (naa.n_ship_theory >= MAX_SHIP_THEORY) return -1;
    ship_theory_t* s = &ship_theories[naa.n_ship_theory];
    s->st_id = naa.n_ship_theory;
    s->st_type = st_type;
    s->st_category = cat;
    s->buoyancy = buo;
    s->stability = stb;
    s->subdivision = sbd;
    s->year = year;
    s->active = 1;
    naa.total_buoyancy += buo;
    naa.n_ship_theory++;
    print_str("[NAA] Ship theory "); print_int(naa.n_ship_theory - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" buo="); print_int(buo);
    print_str(" stb="); print_int(stb);
    print_str(" sbd="); print_int(sbd); print_str("\n");
    return naa.n_ship_theory - 1;
}

int naa_ship_design(int sd_type, int cat, int ovr, int str, int sys, int year) {
    if (naa.n_ship_design >= MAX_SHIP_DESIGN) return -1;
    ship_design_t* d = &ship_designs[naa.n_ship_design];
    d->sd_id = naa.n_ship_design;
    d->sd_type = sd_type;
    d->sd_category = cat;
    d->overall_sd = ovr;
    d->struct_sd = str;
    d->system_sd = sys;
    d->year = year;
    d->active = 1;
    naa.total_overall_sd += ovr;
    naa.n_ship_design++;
    print_str("[NAA] Ship design "); print_int(naa.n_ship_design - 1);
    print_str(" type="); print_int(sd_type);
    print_str(" cat="); print_int(cat);
    print_str(" ovr="); print_int(ovr);
    print_str(" str="); print_int(str);
    print_str(" sys="); print_int(sys); print_str("\n");
    return naa.n_ship_design - 1;
}

int naa_ship_struct(int ss_type, int cat, int hul, int dck, int blk, int year) {
    if (naa.n_ship_struct >= MAX_SHIP_STRUCT) return -1;
    ship_struct_t* s = &ship_structs[naa.n_ship_struct];
    s->ss_id = naa.n_ship_struct;
    s->ss_type = ss_type;
    s->ss_category = cat;
    s->hull_struct = hul;
    s->deck_struct = dck;
    s->bulkhead = blk;
    s->year = year;
    s->active = 1;
    naa.total_hull_struct += hul;
    naa.n_ship_struct++;
    print_str("[NAA] Ship struct "); print_int(naa.n_ship_struct - 1);
    print_str(" type="); print_int(ss_type);
    print_str(" cat="); print_int(cat);
    print_str(" hul="); print_int(hul);
    print_str(" dck="); print_int(dck);
    print_str(" blk="); print_int(blk); print_str("\n");
    return naa.n_ship_struct - 1;
}

int naa_propulsion(int pr_type, int cat, int pth, int pch, int pef, int year) {
    if (naa.n_propulsion >= MAX_PROPULSION) return -1;
    propulsion_t* p = &propulsions[naa.n_propulsion];
    p->pr_id = naa.n_propulsion;
    p->pr_type = pr_type;
    p->pr_category = cat;
    p->propeller_th = pth;
    p->prop_char = pch;
    p->prop_eff = pef;
    p->year = year;
    p->active = 1;
    naa.total_propeller_th += pth;
    naa.n_propulsion++;
    print_str("[NAA] Propulsion "); print_int(naa.n_propulsion - 1);
    print_str(" type="); print_int(pr_type);
    print_str(" cat="); print_int(cat);
    print_str(" pth="); print_int(pth);
    print_str(" pch="); print_int(pch);
    print_str(" pef="); print_int(pef); print_str("\n");
    return naa.n_propulsion - 1;
}

int naa_maneuver(int mn_type, int cat, int cst, int trn, int stp, int year) {
    if (naa.n_maneuver >= MAX_MANEUVER) return -1;
    maneuver_t* m = &maneuvers[naa.n_maneuver];
    m->mn_id = naa.n_maneuver;
    m->mn_type = mn_type;
    m->mn_category = cat;
    m->course_stab = cst;
    m->turning = trn;
    m->stopping = stp;
    m->year = year;
    m->active = 1;
    naa.total_course_stab += cst;
    naa.n_maneuver++;
    print_str("[NAA] Maneuver "); print_int(naa.n_maneuver - 1);
    print_str(" type="); print_int(mn_type);
    print_str(" cat="); print_int(cat);
    print_str(" cst="); print_int(cst);
    print_str(" trn="); print_int(trn);
    print_str(" stp="); print_int(stp); print_str("\n");
    return naa.n_maneuver - 1;
}

void naa_ship_theory_report(void) {
    print_str("[NAA] Ship theory report:\n");
    print_str("  Ship theory categories: "); print_int(naa.n_ship_theory); print_str("\n");
    print_str("  Total buoyancy: "); print_int(naa.total_buoyancy); print_str("\n");
}

void naa_ship_design_report(void) {
    print_str("[NAA] Ship design report:\n");
    print_str("  Ship design categories: "); print_int(naa.n_ship_design); print_str("\n");
    print_str("  Total overall design: "); print_int(naa.total_overall_sd); print_str("\n");
}

void naa_full_report(void) {
    print_str("[NAA] Full report:\n");
    print_str("  Ship structure categories: "); print_int(naa.n_ship_struct); print_str("\n");
    print_str("  Total hull structure: "); print_int(naa.total_hull_struct); print_str("\n");
    print_str("  Propulsion categories: "); print_int(naa.n_propulsion); print_str("\n");
    print_str("  Total propeller theory: "); print_int(naa.total_propeller_th); print_str("\n");
    print_str("  Maneuvering categories: "); print_int(naa.n_maneuver); print_str("\n");
    print_str("  Total course stability: "); print_int(naa.total_course_stab); print_str("\n");
}

void naa_print_state(void) {
    print_str("[NAA] St="); print_int(naa.n_ship_theory);
    print_str(" Ds="); print_int(naa.n_ship_design);
    print_str(" Ss="); print_int(naa.n_ship_struct);
    print_str(" Pr="); print_int(naa.n_propulsion);
    print_str(" Mn="); print_int(naa.n_maneuver);
    print_str("\n");
}

int main(void) {
    print_str("=== Naval Architecture Admin Demo ===\n\n");
    naa_init();

    print_str("Ship theory...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int buo = 55 + (i * 13);
        int stb = 40 + (i * 10);
        int sbd = 22 + (i * 5);
        int year = 2020 + (i % 5);
        naa_ship_theory(type, cat, buo, stb, sbd, year);
    }

    print_str("\nShip design...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ovr = 48 + (i * 11);
        int str = 35 + (i * 8);
        int sys = 20 + (i * 4);
        int year = 2021 + (i % 4);
        naa_ship_design(type, cat, ovr, str, sys, year);
    }

    print_str("\nShip structure...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int hul = 42 + (i * 10);
        int dck = 28 + (i * 7);
        int blk = 18 + (i * 4);
        int year = 2022 + (i % 3);
        naa_ship_struct(type, cat, hul, dck, blk, year);
    }

    print_str("\nPropulsion...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pth = 35 + (i * 8);
        int pch = 25 + (i * 6);
        int pef = 15 + (i * 3);
        int year = 2023 + (i % 2);
        naa_propulsion(type, cat, pth, pch, pef, year);
    }

    print_str("\nManeuvering...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cst = 30 + (i * 7);
        int trn = 22 + (i * 5);
        int stp = 12 + (i * 3);
        int year = 2024;
        naa_maneuver(type, cat, cst, trn, stp, year);
    }

    print_str("\nShip theory report...\n");
    naa_ship_theory_report();

    print_str("\nShip design report...\n");
    naa_ship_design_report();

    print_str("\nFull report...\n");
    naa_full_report();

    print_str("\nFinal state...\n");
    naa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
