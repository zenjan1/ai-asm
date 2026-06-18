/* tribology_admin: Tribology administration system (v1.0)
 * Friction, wear, lubrication, bearings, surface engineering
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

#define MAX_FRICTION    16
#define MAX_WEAR        14
#define MAX_LUBRIC      12
#define MAX_BEARING     10
#define MAX_SURFACE     10

typedef struct {
    int    fr_id;
    int    fr_type;
    int    fr_category;
    int    sliding;
    int    rolling;
    int    boundary_fr;
    int    year;
    int    active;
} friction_t;

typedef struct {
    int    wr_id;
    int    wr_type;
    int    wr_category;
    int    adhesive;
    int    abrasive;
    int    fatigue_wr;
    int    year;
    int    active;
} wear_t;

typedef struct {
    int    lb_id;
    int    lb_type;
    int    lb_category;
    int    fluid_lb;
    int    boundary_lb;
    int    solid_lb;
    int    year;
    int    active;
} lubrication_t;

typedef struct {
    int    br_id;
    int    br_type;
    int    br_category;
    int    rolling_br;
    int    sliding_br;
    int    magnetic_br;
    int    year;
    int    active;
} bearing_t;

typedef struct {
    int    sf_id;
    int    sf_type;
    int    sf_category;
    int    coating;
    int    plating;
    int    surface_mod;
    int    year;
    int    active;
} surface_t;

typedef struct {
    int    n_friction;
    int    n_wear;
    int    n_lubrication;
    int    n_bearing;
    int    n_surface;
    int    total_sliding;
    int    total_adhesive;
    int    total_fluid_lb;
    int    total_rolling_br;
    int    total_coating;
} tba_state_t;

static friction_t frictions[MAX_FRICTION];
static wear_t wears[MAX_WEAR];
static lubrication_t lubrications[MAX_LUBRIC];
static bearing_t bearings[MAX_BEARING];
static surface_t surfaces[MAX_SURFACE];
static tba_state_t tba;

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

int tba_init(void) {
    if (initialized) return -1;
    tba.n_friction = 0; tba.n_wear = 0; tba.n_lubrication = 0;
    tba.n_bearing = 0; tba.n_surface = 0;
    tba.total_sliding = 0; tba.total_adhesive = 0;
    tba.total_fluid_lb = 0; tba.total_rolling_br = 0;
    tba.total_coating = 0;
    for (int i = 0; i < MAX_FRICTION; i++) frictions[i].active = 0;
    for (int i = 0; i < MAX_WEAR; i++) wears[i].active = 0;
    for (int i = 0; i < MAX_LUBRIC; i++) lubrications[i].active = 0;
    for (int i = 0; i < MAX_BEARING; i++) bearings[i].active = 0;
    for (int i = 0; i < MAX_SURFACE; i++) surfaces[i].active = 0;
    initialized = 1;
    print_str("[TBA] Tribology initialized\n");
    return 0;
}

int tba_friction(int ft_type, int cat, int slid, int roll, int bnd, int year) {
    if (tba.n_friction >= MAX_FRICTION) return -1;
    friction_t* f = &frictions[tba.n_friction];
    f->fr_id = tba.n_friction;
    f->fr_type = ft_type;
    f->fr_category = cat;
    f->sliding = slid;
    f->rolling = roll;
    f->boundary_fr = bnd;
    f->year = year;
    f->active = 1;
    tba.total_sliding += slid;
    tba.n_friction++;
    print_str("[TBA] Friction "); print_int(tba.n_friction - 1);
    print_str(" type="); print_int(ft_type);
    print_str(" cat="); print_int(cat);
    print_str(" sld="); print_int(slid);
    print_str(" rll="); print_int(roll);
    print_str(" bnd="); print_int(bnd); print_str("\n");
    return tba.n_friction - 1;
}

int tba_wear(int wr_type, int cat, int adh, int abr, int ftg, int year) {
    if (tba.n_wear >= MAX_WEAR) return -1;
    wear_t* w = &wears[tba.n_wear];
    w->wr_id = tba.n_wear;
    w->wr_type = wr_type;
    w->wr_category = cat;
    w->adhesive = adh;
    w->abrasive = abr;
    w->fatigue_wr = ftg;
    w->year = year;
    w->active = 1;
    tba.total_adhesive += adh;
    tba.n_wear++;
    print_str("[TBA] Wear "); print_int(tba.n_wear - 1);
    print_str(" type="); print_int(wr_type);
    print_str(" cat="); print_int(cat);
    print_str(" adh="); print_int(adh);
    print_str(" abr="); print_int(abr);
    print_str(" ftg="); print_int(ftg); print_str("\n");
    return tba.n_wear - 1;
}

int tba_lubrication(int lb_type, int cat, int flu, int bnd_l, int sol, int year) {
    if (tba.n_lubrication >= MAX_LUBRIC) return -1;
    lubrication_t* l = &lubrications[tba.n_lubrication];
    l->lb_id = tba.n_lubrication;
    l->lb_type = lb_type;
    l->lb_category = cat;
    l->fluid_lb = flu;
    l->boundary_lb = bnd_l;
    l->solid_lb = sol;
    l->year = year;
    l->active = 1;
    tba.total_fluid_lb += flu;
    tba.n_lubrication++;
    print_str("[TBA] Lubrication "); print_int(tba.n_lubrication - 1);
    print_str(" type="); print_int(lb_type);
    print_str(" cat="); print_int(cat);
    print_str(" flu="); print_int(flu);
    print_str(" bdl="); print_int(bnd_l);
    print_str(" sol="); print_int(sol); print_str("\n");
    return tba.n_lubrication - 1;
}

int tba_bearing(int br_type, int cat, int rll, int sld, int mag, int year) {
    if (tba.n_bearing >= MAX_BEARING) return -1;
    bearing_t* b = &bearings[tba.n_bearing];
    b->br_id = tba.n_bearing;
    b->br_type = br_type;
    b->br_category = cat;
    b->rolling_br = rll;
    b->sliding_br = sld;
    b->magnetic_br = mag;
    b->year = year;
    b->active = 1;
    tba.total_rolling_br += rll;
    tba.n_bearing++;
    print_str("[TBA] Bearing "); print_int(tba.n_bearing - 1);
    print_str(" type="); print_int(br_type);
    print_str(" cat="); print_int(cat);
    print_str(" rll="); print_int(rll);
    print_str(" sld="); print_int(sld);
    print_str(" mag="); print_int(mag); print_str("\n");
    return tba.n_bearing - 1;
}

int tba_surface(int sf_type, int cat, int cot, int plt, int sfc_m, int year) {
    if (tba.n_surface >= MAX_SURFACE) return -1;
    surface_t* s = &surfaces[tba.n_surface];
    s->sf_id = tba.n_surface;
    s->sf_type = sf_type;
    s->sf_category = cat;
    s->coating = cot;
    s->plating = plt;
    s->surface_mod = sfc_m;
    s->year = year;
    s->active = 1;
    tba.total_coating += cot;
    tba.n_surface++;
    print_str("[TBA] Surface "); print_int(tba.n_surface - 1);
    print_str(" type="); print_int(sf_type);
    print_str(" cat="); print_int(cat);
    print_str(" cot="); print_int(cot);
    print_str(" plt="); print_int(plt);
    print_str(" sfm="); print_int(sfc_m); print_str("\n");
    return tba.n_surface - 1;
}

void tba_friction_report(void) {
    print_str("[TBA] Friction report:\n");
    print_str("  Friction categories: "); print_int(tba.n_friction); print_str("\n");
    print_str("  Total sliding: "); print_int(tba.total_sliding); print_str("\n");
}

void tba_wear_report(void) {
    print_str("[TBA] Wear report:\n");
    print_str("  Wear categories: "); print_int(tba.n_wear); print_str("\n");
    print_str("  Total adhesive wear: "); print_int(tba.total_adhesive); print_str("\n");
}

void tba_full_report(void) {
    print_str("[TBA] Full report:\n");
    print_str("  Lubrication categories: "); print_int(tba.n_lubrication); print_str("\n");
    print_str("  Total fluid lubrication: "); print_int(tba.total_fluid_lb); print_str("\n");
    print_str("  Bearing categories: "); print_int(tba.n_bearing); print_str("\n");
    print_str("  Total rolling bearings: "); print_int(tba.total_rolling_br); print_str("\n");
    print_str("  Surface engineering categories: "); print_int(tba.n_surface); print_str("\n");
    print_str("  Total coatings: "); print_int(tba.total_coating); print_str("\n");
}

void tba_print_state(void) {
    print_str("[TBA] Fr="); print_int(tba.n_friction);
    print_str(" Wr="); print_int(tba.n_wear);
    print_str(" Lb="); print_int(tba.n_lubrication);
    print_str(" Br="); print_int(tba.n_bearing);
    print_str(" Sf="); print_int(tba.n_surface);
    print_str("\n");
}

int main(void) {
    print_str("=== Tribology Admin Demo ===\n\n");
    tba_init();

    print_str("Friction...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int slid = 55 + (i * 13);
        int roll = 40 + (i * 10);
        int bnd = 22 + (i * 5);
        int year = 2020 + (i % 5);
        tba_friction(type, cat, slid, roll, bnd, year);
    }

    print_str("\nWear...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int adh = 48 + (i * 11);
        int abr = 35 + (i * 8);
        int ftg = 20 + (i * 4);
        int year = 2021 + (i % 4);
        tba_wear(type, cat, adh, abr, ftg, year);
    }

    print_str("\nLubrication...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int flu = 42 + (i * 10);
        int bdl = 28 + (i * 7);
        int sol = 18 + (i * 4);
        int year = 2022 + (i % 3);
        tba_lubrication(type, cat, flu, bdl, sol, year);
    }

    print_str("\nBearings...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int rll = 35 + (i * 8);
        int sld = 25 + (i * 6);
        int mag = 15 + (i * 3);
        int year = 2023 + (i % 2);
        tba_bearing(type, cat, rll, sld, mag, year);
    }

    print_str("\nSurface engineering...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cot = 30 + (i * 7);
        int plt = 22 + (i * 5);
        int sfm = 12 + (i * 3);
        int year = 2024;
        tba_surface(type, cat, cot, plt, sfm, year);
    }

    print_str("\nFriction report...\n");
    tba_friction_report();

    print_str("\nWear report...\n");
    tba_wear_report();

    print_str("\nFull report...\n");
    tba_full_report();

    print_str("\nFinal state...\n");
    tba_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
