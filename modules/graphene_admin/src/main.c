/* graphene_admin: Graphene administration system (v1.0)
 * Graphene preparation, modification, thin films, composites, applications
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

#define MAX_PREP       16
#define MAX_MODIFY     14
#define MAX_FILM       12
#define MAX_COMPOSITE  10
#define MAX_APP        10

typedef struct {
    int    prep_id;
    int    prep_type;
    int    prep_category;
    int    mechanical;
    int    cvd_prep;
    int    redox_method;
    int    year;
    int    active;
} prep_t;

typedef struct {
    int    mod_id;
    int    mod_type;
    int    mod_category;
    int    graphene_oxide;
    int    reduced_go;
    int    functional;
    int    year;
    int    active;
} modify_t;

typedef struct {
    int    film_id;
    int    film_type;
    int    film_category;
    int    transparent;
    int    barrier_f;
    int    thermal_f;
    int    year;
    int    active;
} film_t;

typedef struct {
    int    comp_id;
    int    comp_type;
    int    comp_category;
    int    polymer_comp;
    int    metal_comp;
    int    ceramic_comp;
    int    year;
    int    active;
} composite_g_t;

typedef struct {
    int    app_id;
    int    app_type;
    int    app_category;
    int    electronic;
    int    energy_stor;
    int    sensor_app;
    int    year;
    int    active;
} app_g_t;

typedef struct {
    int    n_prep;
    int    n_modify;
    int    n_film;
    int    n_composite;
    int    n_app;
    int    total_mechanical;
    int    total_oxide;
    int    total_transparent;
    int    total_polymer;
    int    total_electronic;
} ga_state_t;

static prep_t preps[MAX_PREP];
static modify_t modifies[MAX_MODIFY];
static film_t films[MAX_FILM];
static composite_g_t composites[MAX_COMPOSITE];
static app_g_t apps[MAX_APP];
static ga_state_t ga;

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

int ga_init(void) {
    if (initialized) return -1;
    ga.n_prep = 0; ga.n_modify = 0; ga.n_film = 0;
    ga.n_composite = 0; ga.n_app = 0;
    ga.total_mechanical = 0; ga.total_oxide = 0;
    ga.total_transparent = 0; ga.total_polymer = 0;
    ga.total_electronic = 0;
    for (int i = 0; i < MAX_PREP; i++) preps[i].active = 0;
    for (int i = 0; i < MAX_MODIFY; i++) modifies[i].active = 0;
    for (int i = 0; i < MAX_FILM; i++) films[i].active = 0;
    for (int i = 0; i < MAX_COMPOSITE; i++) composites[i].active = 0;
    for (int i = 0; i < MAX_APP; i++) apps[i].active = 0;
    initialized = 1;
    print_str("[GA] Graphene initialized\n");
    return 0;
}

int ga_prep(int pr_type, int cat, int mech, int cvd_p, int redox, int year) {
    if (ga.n_prep >= MAX_PREP) return -1;
    prep_t* p = &preps[ga.n_prep];
    p->prep_id = ga.n_prep;
    p->prep_type = pr_type;
    p->prep_category = cat;
    p->mechanical = mech;
    p->cvd_prep = cvd_p;
    p->redox_method = redox;
    p->year = year;
    p->active = 1;
    ga.total_mechanical += mech;
    ga.n_prep++;
    print_str("[GA] Prep "); print_int(ga.n_prep - 1);
    print_str(" type="); print_int(pr_type);
    print_str(" cat="); print_int(cat);
    print_str(" mch="); print_int(mech);
    print_str(" cvd="); print_int(cvd_p);
    print_str(" rdx="); print_int(redox); print_str("\n");
    return ga.n_prep - 1;
}

int ga_modify(int md_type, int cat, int go_v, int rgo_v, int func_v, int year) {
    if (ga.n_modify >= MAX_MODIFY) return -1;
    modify_t* m = &modifies[ga.n_modify];
    m->mod_id = ga.n_modify;
    m->mod_type = md_type;
    m->mod_category = cat;
    m->graphene_oxide = go_v;
    m->reduced_go = rgo_v;
    m->functional = func_v;
    m->year = year;
    m->active = 1;
    ga.total_oxide += go_v;
    ga.n_modify++;
    print_str("[GA] Modify "); print_int(ga.n_modify - 1);
    print_str(" type="); print_int(md_type);
    print_str(" cat="); print_int(cat);
    print_str(" gox="); print_int(go_v);
    print_str(" rgo="); print_int(rgo_v);
    print_str(" fnc="); print_int(func_v); print_str("\n");
    return ga.n_modify - 1;
}

int ga_film(int fm_type, int cat, int transp, int barrier, int thermal, int year) {
    if (ga.n_film >= MAX_FILM) return -1;
    film_t* f = &films[ga.n_film];
    f->film_id = ga.n_film;
    f->film_type = fm_type;
    f->film_category = cat;
    f->transparent = transp;
    f->barrier_f = barrier;
    f->thermal_f = thermal;
    f->year = year;
    f->active = 1;
    ga.total_transparent += transp;
    ga.n_film++;
    print_str("[GA] Film "); print_int(ga.n_film - 1);
    print_str(" type="); print_int(fm_type);
    print_str(" cat="); print_int(cat);
    print_str(" trn="); print_int(transp);
    print_str(" brr="); print_int(barrier);
    print_str(" thm="); print_int(thermal); print_str("\n");
    return ga.n_film - 1;
}

int ga_composite(int cp_type, int cat, int polymer, int metal_c, int ceramic_c, int year) {
    if (ga.n_composite >= MAX_COMPOSITE) return -1;
    composite_g_t* c = &composites[ga.n_composite];
    c->comp_id = ga.n_composite;
    c->comp_type = cp_type;
    c->comp_category = cat;
    c->polymer_comp = polymer;
    c->metal_comp = metal_c;
    c->ceramic_comp = ceramic_c;
    c->year = year;
    c->active = 1;
    ga.total_polymer += polymer;
    ga.n_composite++;
    print_str("[GA] Composite "); print_int(ga.n_composite - 1);
    print_str(" type="); print_int(cp_type);
    print_str(" cat="); print_int(cat);
    print_str(" ply="); print_int(polymer);
    print_str(" mtl="); print_int(metal_c);
    print_str(" crm="); print_int(ceramic_c); print_str("\n");
    return ga.n_composite - 1;
}

int ga_app(int ap_type, int cat, int elec, int energy_s, int sensor_a, int year) {
    if (ga.n_app >= MAX_APP) return -1;
    app_g_t* a = &apps[ga.n_app];
    a->app_id = ga.n_app;
    a->app_type = ap_type;
    a->app_category = cat;
    a->electronic = elec;
    a->energy_stor = energy_s;
    a->sensor_app = sensor_a;
    a->year = year;
    a->active = 1;
    ga.total_electronic += elec;
    ga.n_app++;
    print_str("[GA] App "); print_int(ga.n_app - 1);
    print_str(" type="); print_int(ap_type);
    print_str(" cat="); print_int(cat);
    print_str(" elc="); print_int(elec);
    print_str(" nrg="); print_int(energy_s);
    print_str(" snr="); print_int(sensor_a); print_str("\n");
    return ga.n_app - 1;
}

void ga_prep_report(void) {
    print_str("[GA] Prep report:\n");
    print_str("  Graphene prep categories: "); print_int(ga.n_prep); print_str("\n");
    print_str("  Total mechanical exfol: "); print_int(ga.total_mechanical); print_str("\n");
}

void ga_modify_report(void) {
    print_str("[GA] Modify report:\n");
    print_str("  Graphene modification categories: "); print_int(ga.n_modify); print_str("\n");
    print_str("  Total graphene oxide: "); print_int(ga.total_oxide); print_str("\n");
}

void ga_app_report(void) {
    print_str("[GA] App report:\n");
    print_str("  Graphene film categories: "); print_int(ga.n_film); print_str("\n");
    print_str("  Total transparent film: "); print_int(ga.total_transparent); print_str("\n");
    print_str("  Graphene composite categories: "); print_int(ga.n_composite); print_str("\n");
    print_str("  Total polymer composite: "); print_int(ga.total_polymer); print_str("\n");
    print_str("  Graphene application categories: "); print_int(ga.n_app); print_str("\n");
    print_str("  Total electronic devices: "); print_int(ga.total_electronic); print_str("\n");
}

void ga_print_state(void) {
    print_str("[GA] Pr="); print_int(ga.n_prep);
    print_str(" Md="); print_int(ga.n_modify);
    print_str(" Fm="); print_int(ga.n_film);
    print_str(" Cp="); print_int(ga.n_composite);
    print_str(" Ap="); print_int(ga.n_app);
    print_str("\n");
}

int main(void) {
    print_str("=== Graphene Admin Demo ===\n\n");
    ga_init();

    print_str("Graphene preparation...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int mch = 55 + (i * 13);
        int cvd = 40 + (i * 10);
        int rdx = 22 + (i * 5);
        int year = 2020 + (i % 5);
        ga_prep(type, cat, mch, cvd, rdx, year);
    }

    print_str("\nGraphene modification...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int gox = 48 + (i * 12);
        int rgo = 35 + (i * 8);
        int fnc = 20 + (i * 5);
        int year = 2021 + (i % 4);
        ga_modify(type, cat, gox, rgo, fnc, year);
    }

    print_str("\nGraphene films...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int trn = 42 + (i * 10);
        int brr = 28 + (i * 7);
        int thm = 18 + (i * 4);
        int year = 2022 + (i % 3);
        ga_film(type, cat, trn, brr, thm, year);
    }

    print_str("\nGraphene composites...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ply = 35 + (i * 8);
        int mtl = 25 + (i * 6);
        int crm = 15 + (i * 3);
        int year = 2023 + (i % 2);
        ga_composite(type, cat, ply, mtl, crm, year);
    }

    print_str("\nGraphene applications...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int elc = 30 + (i * 7);
        int nrg = 22 + (i * 5);
        int snr = 12 + (i * 3);
        int year = 2024;
        ga_app(type, cat, elc, nrg, snr, year);
    }

    print_str("\nPrep report...\n");
    ga_prep_report();

    print_str("\nModify report...\n");
    ga_modify_report();

    print_str("\nApp report...\n");
    ga_app_report();

    print_str("\nFinal state...\n");
    ga_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
