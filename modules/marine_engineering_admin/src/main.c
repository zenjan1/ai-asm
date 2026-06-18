/* marine_engineering_admin: Marine engineering administration system (v1.0)
 * Ship engineering, ocean platforms, subsea engineering, port engineering, ocean energy
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

#define MAX_SHIP       16
#define MAX_PLATFORM   14
#define MAX_SUBSEA     12
#define MAX_PORT       10
#define MAX_OCEAN_EN   10

typedef struct {
    int    ship_id;
    int    ship_type;
    int    ship_category;
    int    cargo_ship;
    int    passenger_ship;
    int    special_ship;
    int    year;
    int    active;
} ship_t;

typedef struct {
    int    platform_id;
    int    platform_type;
    int    platform_category;
    int    fixed_platform;
    int    floating_platform;
    int    deepsea_platform;
    int    year;
    int    active;
} platform_t;

typedef struct {
    int    subsea_id;
    int    subsea_type;
    int    subsea_category;
    int    subsea_pipeline;
    int    subsea_cable;
    int    subsea_mining;
    int    year;
    int    active;
} subsea_t;

typedef struct {
    int    port_id;
    int    port_type;
    int    port_category;
    int    port_construction;
    int    channel_dredge;
    int    breakwater;
    int    year;
    int    active;
} port_t;

typedef struct {
    int    ocean_en_id;
    int    ocean_en_type;
    int    ocean_en_category;
    int    tidal_energy;
    int    wave_energy;
    int    current_energy;
    int    year;
    int    active;
} ocean_en_t;

typedef struct {
    int    n_ship;
    int    n_platform;
    int    n_subsea;
    int    n_port;
    int    n_ocean_en;
    int    total_cargo;
    int    total_fixed;
    int    total_pipeline;
    int    total_construction;
    int    total_tidal;
} mea_state_t;

static ship_t ships[MAX_SHIP];
static platform_t platforms[MAX_PLATFORM];
static subsea_t subseas[MAX_SUBSEA];
static port_t ports[MAX_PORT];
static ocean_en_t ocean_energies[MAX_OCEAN_EN];
static mea_state_t mea;

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

int mea_init(void) {
    if (initialized) return -1;
    mea.n_ship = 0; mea.n_platform = 0; mea.n_subsea = 0;
    mea.n_port = 0; mea.n_ocean_en = 0;
    mea.total_cargo = 0; mea.total_fixed = 0;
    mea.total_pipeline = 0; mea.total_construction = 0;
    mea.total_tidal = 0;
    for (int i = 0; i < MAX_SHIP; i++) ships[i].active = 0;
    for (int i = 0; i < MAX_PLATFORM; i++) platforms[i].active = 0;
    for (int i = 0; i < MAX_SUBSEA; i++) subseas[i].active = 0;
    for (int i = 0; i < MAX_PORT; i++) ports[i].active = 0;
    for (int i = 0; i < MAX_OCEAN_EN; i++) ocean_energies[i].active = 0;
    initialized = 1;
    print_str("[MEA] Marine engineering initialized\n");
    return 0;
}

int mea_ship(int sh_type, int cat, int cargo, int passgr, int special, int year) {
    if (mea.n_ship >= MAX_SHIP) return -1;
    ship_t* s = &ships[mea.n_ship];
    s->ship_id = mea.n_ship;
    s->ship_type = sh_type;
    s->ship_category = cat;
    s->cargo_ship = cargo;
    s->passenger_ship = passgr;
    s->special_ship = special;
    s->year = year;
    s->active = 1;
    mea.total_cargo += cargo;
    mea.n_ship++;
    print_str("[MEA] Ship "); print_int(mea.n_ship - 1);
    print_str(" type="); print_int(sh_type);
    print_str(" cat="); print_int(cat);
    print_str(" crg="); print_int(cargo);
    print_str(" psg="); print_int(passgr);
    print_str(" spc="); print_int(special); print_str("\n");
    return mea.n_ship - 1;
}

int mea_platform(int pl_type, int cat, int fixed, int floating, int deepsea, int year) {
    if (mea.n_platform >= MAX_PLATFORM) return -1;
    platform_t* p = &platforms[mea.n_platform];
    p->platform_id = mea.n_platform;
    p->platform_type = pl_type;
    p->platform_category = cat;
    p->fixed_platform = fixed;
    p->floating_platform = floating;
    p->deepsea_platform = deepsea;
    p->year = year;
    p->active = 1;
    mea.total_fixed += fixed;
    mea.n_platform++;
    print_str("[MEA] Platform "); print_int(mea.n_platform - 1);
    print_str(" type="); print_int(pl_type);
    print_str(" cat="); print_int(cat);
    print_str(" fix="); print_int(fixed);
    print_str(" flt="); print_int(floating);
    print_str(" dep="); print_int(deepsea); print_str("\n");
    return mea.n_platform - 1;
}

int mea_subsea(int sb_type, int cat, int pipeline, int cable, int mining_c, int year) {
    if (mea.n_subsea >= MAX_SUBSEA) return -1;
    subsea_t* s = &subseas[mea.n_subsea];
    s->subsea_id = mea.n_subsea;
    s->subsea_type = sb_type;
    s->subsea_category = cat;
    s->subsea_pipeline = pipeline;
    s->subsea_cable = cable;
    s->subsea_mining = mining_c;
    s->year = year;
    s->active = 1;
    mea.total_pipeline += pipeline;
    mea.n_subsea++;
    print_str("[MEA] Subsea "); print_int(mea.n_subsea - 1);
    print_str(" type="); print_int(sb_type);
    print_str(" cat="); print_int(cat);
    print_str(" ppl="); print_int(pipeline);
    print_str(" cbl="); print_int(cable);
    print_str(" mng="); print_int(mining_c); print_str("\n");
    return mea.n_subsea - 1;
}

int mea_port(int pt_type, int cat, int construct, int dredge, int break_w, int year) {
    if (mea.n_port >= MAX_PORT) return -1;
    port_t* p = &ports[mea.n_port];
    p->port_id = mea.n_port;
    p->port_type = pt_type;
    p->port_category = cat;
    p->port_construction = construct;
    p->channel_dredge = dredge;
    p->breakwater = break_w;
    p->year = year;
    p->active = 1;
    mea.total_construction += construct;
    mea.n_port++;
    print_str("[MEA] Port "); print_int(mea.n_port - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" cst="); print_int(construct);
    print_str(" drg="); print_int(dredge);
    print_str(" brk="); print_int(break_w); print_str("\n");
    return mea.n_port - 1;
}

int mea_ocean_energy(int oe_type, int cat, int tidal, int wave, int current_c, int year) {
    if (mea.n_ocean_en >= MAX_OCEAN_EN) return -1;
    ocean_en_t* o = &ocean_energies[mea.n_ocean_en];
    o->ocean_en_id = mea.n_ocean_en;
    o->ocean_en_type = oe_type;
    o->ocean_en_category = cat;
    o->tidal_energy = tidal;
    o->wave_energy = wave;
    o->current_energy = current_c;
    o->year = year;
    o->active = 1;
    mea.total_tidal += tidal;
    mea.n_ocean_en++;
    print_str("[MEA] Ocean energy "); print_int(mea.n_ocean_en - 1);
    print_str(" type="); print_int(oe_type);
    print_str(" cat="); print_int(cat);
    print_str(" tdl="); print_int(tidal);
    print_str(" wve="); print_int(wave);
    print_str(" crt="); print_int(current_c); print_str("\n");
    return mea.n_ocean_en - 1;
}

void mea_ship_report(void) {
    print_str("[MEA] Ship report:\n");
    print_str("  Ship engineering categories: "); print_int(mea.n_ship); print_str("\n");
    print_str("  Total cargo ships: "); print_int(mea.total_cargo); print_str("\n");
}

void mea_platform_report(void) {
    print_str("[MEA] Platform report:\n");
    print_str("  Ocean platform categories: "); print_int(mea.n_platform); print_str("\n");
    print_str("  Total fixed platforms: "); print_int(mea.total_fixed); print_str("\n");
}

void mea_ocean_report(void) {
    print_str("[MEA] Ocean report:\n");
    print_str("  Subsea engineering categories: "); print_int(mea.n_subsea); print_str("\n");
    print_str("  Total subsea pipelines: "); print_int(mea.total_pipeline); print_str("\n");
    print_str("  Port engineering categories: "); print_int(mea.n_port); print_str("\n");
    print_str("  Total port construction: "); print_int(mea.total_construction); print_str("\n");
    print_str("  Ocean energy categories: "); print_int(mea.n_ocean_en); print_str("\n");
    print_str("  Total tidal energy: "); print_int(mea.total_tidal); print_str("\n");
}

void mea_print_state(void) {
    print_str("[MEA] Sh="); print_int(mea.n_ship);
    print_str(" Pl="); print_int(mea.n_platform);
    print_str(" Sb="); print_int(mea.n_subsea);
    print_str(" Pt="); print_int(mea.n_port);
    print_str(" Oe="); print_int(mea.n_ocean_en);
    print_str("\n");
}

int main(void) {
    print_str("=== Marine Engineering Admin Demo ===\n\n");
    mea_init();

    print_str("Ship engineering...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int crg = 55 + (i * 13);
        int psg = 38 + (i * 9);
        int spc = 22 + (i * 5);
        int year = 2020 + (i % 5);
        mea_ship(type, cat, crg, psg, spc, year);
    }

    print_str("\nOcean platforms...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int fix = 48 + (i * 11);
        int flt = 32 + (i * 8);
        int dep = 20 + (i * 5);
        int year = 2021 + (i % 4);
        mea_platform(type, cat, fix, flt, dep, year);
    }

    print_str("\nSubsea engineering...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ppl = 40 + (i * 10);
        int cbl = 28 + (i * 7);
        int mng = 18 + (i * 4);
        int year = 2022 + (i % 3);
        mea_subsea(type, cat, ppl, cbl, mng, year);
    }

    print_str("\nPort engineering...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cst = 35 + (i * 8);
        int drg = 25 + (i * 6);
        int brk = 15 + (i * 3);
        int year = 2023 + (i % 2);
        mea_port(type, cat, cst, drg, brk, year);
    }

    print_str("\nOcean energy...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int tdl = 30 + (i * 7);
        int wve = 22 + (i * 5);
        int crt = 12 + (i * 3);
        int year = 2024;
        mea_ocean_energy(type, cat, tdl, wve, crt, year);
    }

    print_str("\nShip report...\n");
    mea_ship_report();

    print_str("\nPlatform report...\n");
    mea_platform_report();

    print_str("\nOcean report...\n");
    mea_ocean_report();

    print_str("\nFinal state...\n");
    mea_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
