/* space_exploration_admin: Space exploration administration system (v1.0)
 * Manned spaceflight, deep space exploration, space science, commercial space, international cooperation
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

#define MAX_MANNED       16
#define MAX_DEEP         14
#define MAX_SCIENCE      12
#define MAX_COMMERCIAL   10
#define MAX_COOPERATE    10

typedef struct {
    int    manned_id;
    int    manned_type;
    int    manned_category;
    int    spaceship;
    int    space_station;
    int    spacesuit;
    int    year;
    int    active;
} manned_t;

typedef struct {
    int    deep_id;
    int    deep_type;
    int    deep_category;
    int    moon_explore;
    int    mars_explore;
    int    asteroid;
    int    year;
    int    active;
} deep_t;

typedef struct {
    int    science_id;
    int    science_type;
    int    science_category;
    int    space_astro;
    int    space_physics;
    int    space_life;
    int    year;
    int    active;
} science_t;

typedef struct {
    int    commercial_id;
    int    commercial_type;
    int    commercial_category;
    int    commercial_launch;
    int    commercial_sat;
    int    space_tourism;
    int    year;
    int    active;
} commercial_t;

typedef struct {
    int    cooperate_id;
    int    cooperate_type;
    int    cooperate_category;
    int    iss;
    int    joint_explore;
    int    tech_exchange;
    int    year;
    int    active;
} cooperate_t;

typedef struct {
    int    n_manned;
    int    n_deep;
    int    n_science;
    int    n_commercial;
    int    n_cooperate;
    int    total_spaceship;
    int    total_moon;
    int    total_astro;
    int    total_launch;
    int    total_iss;
} sea_state_t;

static manned_t manned_list[MAX_MANNED];
static deep_t deeps[MAX_DEEP];
static science_t sciences[MAX_SCIENCE];
static commercial_t commercials[MAX_COMMERCIAL];
static cooperate_t cooperates[MAX_COOPERATE];
static sea_state_t sea;

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

int sea_init(void) {
    if (initialized) return -1;
    sea.n_manned = 0; sea.n_deep = 0; sea.n_science = 0;
    sea.n_commercial = 0; sea.n_cooperate = 0;
    sea.total_spaceship = 0; sea.total_moon = 0;
    sea.total_astro = 0; sea.total_launch = 0;
    sea.total_iss = 0;
    for (int i = 0; i < MAX_MANNED; i++) manned_list[i].active = 0;
    for (int i = 0; i < MAX_DEEP; i++) deeps[i].active = 0;
    for (int i = 0; i < MAX_SCIENCE; i++) sciences[i].active = 0;
    for (int i = 0; i < MAX_COMMERCIAL; i++) commercials[i].active = 0;
    for (int i = 0; i < MAX_COOPERATE; i++) cooperates[i].active = 0;
    initialized = 1;
    print_str("[SEA] Space exploration initialized\n");
    return 0;
}

int sea_manned(int mnd_type, int cat, int ship, int station, int suit, int year) {
    if (sea.n_manned >= MAX_MANNED) return -1;
    manned_t* m = &manned_list[sea.n_manned];
    m->manned_id = sea.n_manned;
    m->manned_type = mnd_type;
    m->manned_category = cat;
    m->spaceship = ship;
    m->space_station = station;
    m->spacesuit = suit;
    m->year = year;
    m->active = 1;
    sea.total_spaceship += ship;
    sea.n_manned++;
    print_str("[SEA] Manned "); print_int(sea.n_manned - 1);
    print_str(" type="); print_int(mnd_type);
    print_str(" cat="); print_int(cat);
    print_str(" shp="); print_int(ship);
    print_str(" stn="); print_int(station);
    print_str(" sut="); print_int(suit); print_str("\n");
    return sea.n_manned - 1;
}

int sea_deep(int dp_type, int cat, int moon, int mars, int aster, int year) {
    if (sea.n_deep >= MAX_DEEP) return -1;
    deep_t* d = &deeps[sea.n_deep];
    d->deep_id = sea.n_deep;
    d->deep_type = dp_type;
    d->deep_category = cat;
    d->moon_explore = moon;
    d->mars_explore = mars;
    d->asteroid = aster;
    d->year = year;
    d->active = 1;
    sea.total_moon += moon;
    sea.n_deep++;
    print_str("[SEA] Deep "); print_int(sea.n_deep - 1);
    print_str(" type="); print_int(dp_type);
    print_str(" cat="); print_int(cat);
    print_str(" mnn="); print_int(moon);
    print_str(" mrs="); print_int(mars);
    print_str(" ast="); print_int(aster); print_str("\n");
    return sea.n_deep - 1;
}

int sea_science(int sci_type, int cat, int astro, int physics, int life, int year) {
    if (sea.n_science >= MAX_SCIENCE) return -1;
    science_t* s = &sciences[sea.n_science];
    s->science_id = sea.n_science;
    s->science_type = sci_type;
    s->science_category = cat;
    s->space_astro = astro;
    s->space_physics = physics;
    s->space_life = life;
    s->year = year;
    s->active = 1;
    sea.total_astro += astro;
    sea.n_science++;
    print_str("[SEA] Science "); print_int(sea.n_science - 1);
    print_str(" type="); print_int(sci_type);
    print_str(" cat="); print_int(cat);
    print_str(" astr="); print_int(astro);
    print_str(" phys="); print_int(physics);
    print_str(" life="); print_int(life); print_str("\n");
    return sea.n_science - 1;
}

int sea_commercial(int com_type, int cat, int launch, int sat_cnt, int tourism, int year) {
    if (sea.n_commercial >= MAX_COMMERCIAL) return -1;
    commercial_t* c = &commercials[sea.n_commercial];
    c->commercial_id = sea.n_commercial;
    c->commercial_type = com_type;
    c->commercial_category = cat;
    c->commercial_launch = launch;
    c->commercial_sat = sat_cnt;
    c->space_tourism = tourism;
    c->year = year;
    c->active = 1;
    sea.total_launch += launch;
    sea.n_commercial++;
    print_str("[SEA] Commercial "); print_int(sea.n_commercial - 1);
    print_str(" type="); print_int(com_type);
    print_str(" cat="); print_int(cat);
    print_str(" lch="); print_int(launch);
    print_str(" sat="); print_int(sat_cnt);
    print_str(" trm="); print_int(tourism); print_str("\n");
    return sea.n_commercial - 1;
}

int sea_cooperate(int cop_type, int cat, int iss_cnt, int joint, int tech, int year) {
    if (sea.n_cooperate >= MAX_COOPERATE) return -1;
    cooperate_t* c = &cooperates[sea.n_cooperate];
    c->cooperate_id = sea.n_cooperate;
    c->cooperate_type = cop_type;
    c->cooperate_category = cat;
    c->iss = iss_cnt;
    c->joint_explore = joint;
    c->tech_exchange = tech;
    c->year = year;
    c->active = 1;
    sea.total_iss += iss_cnt;
    sea.n_cooperate++;
    print_str("[SEA] Cooperate "); print_int(sea.n_cooperate - 1);
    print_str(" type="); print_int(cop_type);
    print_str(" cat="); print_int(cat);
    print_str(" iss="); print_int(iss_cnt);
    print_str(" jnt="); print_int(joint);
    print_str(" tch="); print_int(tech); print_str("\n");
    return sea.n_cooperate - 1;
}

void sea_manned_report(void) {
    print_str("[SEA] Manned report:\n");
    print_str("  Manned spaceflight categories: "); print_int(sea.n_manned); print_str("\n");
    print_str("  Total spaceships: "); print_int(sea.total_spaceship); print_str("\n");
}

void sea_deep_report(void) {
    print_str("[SEA] Deep report:\n");
    print_str("  Deep space exploration categories: "); print_int(sea.n_deep); print_str("\n");
    print_str("  Total moon exploration: "); print_int(sea.total_moon); print_str("\n");
}

void sea_cooperate_report(void) {
    print_str("[SEA] Cooperate report:\n");
    print_str("  Space science categories: "); print_int(sea.n_science); print_str("\n");
    print_str("  Total space astronomy: "); print_int(sea.total_astro); print_str("\n");
    print_str("  Commercial space categories: "); print_int(sea.n_commercial); print_str("\n");
    print_str("  Total commercial launch: "); print_int(sea.total_launch); print_str("\n");
    print_str("  International cooperation categories: "); print_int(sea.n_cooperate); print_str("\n");
    print_str("  Total ISS participation: "); print_int(sea.total_iss); print_str("\n");
}

void sea_print_state(void) {
    print_str("[SEA] Mn="); print_int(sea.n_manned);
    print_str(" Dp="); print_int(sea.n_deep);
    print_str(" Sc="); print_int(sea.n_science);
    print_str(" Cm="); print_int(sea.n_commercial);
    print_str(" Cp="); print_int(sea.n_cooperate);
    print_str("\n");
}

int main(void) {
    print_str("=== Space Exploration Admin Demo ===\n\n");
    sea_init();

    print_str("Manned spaceflight...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int shp = 50 + (i * 12);
        int stn = 35 + (i * 8);
        int sut = 22 + (i * 5);
        int year = 2020 + (i % 5);
        sea_manned(type, cat, shp, stn, sut, year);
    }

    print_str("\nDeep space exploration...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int mnn = 40 + (i * 10);
        int mrs = 28 + (i * 7);
        int ast = 18 + (i * 4);
        int year = 2021 + (i % 4);
        sea_deep(type, cat, mnn, mrs, ast, year);
    }

    print_str("\nSpace science...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int astr = 35 + (i * 9);
        int phys = 25 + (i * 6);
        int life = 18 + (i * 4);
        int year = 2022 + (i % 3);
        sea_science(type, cat, astr, phys, life, year);
    }

    print_str("\nCommercial space...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int lch = 30 + (i * 8);
        int sat_c = 22 + (i * 5);
        int trm = 15 + (i * 3);
        int year = 2023 + (i % 2);
        sea_commercial(type, cat, lch, sat_c, trm, year);
    }

    print_str("\nInternational cooperation...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int iss_c = 25 + (i * 6);
        int jnt = 18 + (i * 4);
        int tch = 12 + (i * 3);
        int year = 2024;
        sea_cooperate(type, cat, iss_c, jnt, tch, year);
    }

    print_str("\nManned report...\n");
    sea_manned_report();

    print_str("\nDeep report...\n");
    sea_deep_report();

    print_str("\nCooperate report...\n");
    sea_cooperate_report();

    print_str("\nFinal state...\n");
    sea_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
