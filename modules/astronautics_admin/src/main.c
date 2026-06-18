/* astronautics_admin: Astronautics administration system (v1.0)
 * Orbital mechanics, rocket propulsion, satellites, deep space, spacecraft design
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

#define MAX_ORBITAL     16
#define MAX_ROCKET      14
#define MAX_SATELLITE   12
#define MAX_DEEP_SPACE  10
#define MAX_SPACECRAFT  10

typedef struct {
    int    ob_id;
    int    ob_type;
    int    ob_category;
    int    kepler_law;
    int    orbit_trans;
    int    rendezvous;
    int    year;
    int    active;
} orbital_t;

typedef struct {
    int    rk_id;
    int    rk_type;
    int    rk_category;
    int    solid_rp;
    int    liquid_rp;
    int    electric_prop;
    int    year;
    int    active;
} rocket_t;

typedef struct {
    int    st_id;
    int    st_type;
    int    st_category;
    int    comm_sat;
    int    nav_sat;
    int    remote_sat;
    int    year;
    int    active;
} satellite_t;

typedef struct {
    int    ds_id;
    int    ds_type;
    int    ds_category;
    int    planet_exp;
    int    asteroid_exp;
    int    interstellar;
    int    year;
    int    active;
} deep_space_t;

typedef struct {
    int    sc_id;
    int    sc_type;
    int    sc_category;
    int    manned_ship;
    int    space_station;
    int    space_shuttle;
    int    year;
    int    active;
} spacecraft_t;

typedef struct {
    int    n_orbital;
    int    n_rocket;
    int    n_satellite;
    int    n_deep_space;
    int    n_spacecraft;
    int    total_kepler_law;
    int    total_solid_rp;
    int    total_comm_sat;
    int    total_planet_exp;
    int    total_manned_ship;
} asa_state_t;

static orbital_t orbitals[MAX_ORBITAL];
static rocket_t rockets[MAX_ROCKET];
static satellite_t satellites[MAX_SATELLITE];
static deep_space_t deep_spaces[MAX_DEEP_SPACE];
static spacecraft_t spacecrafts[MAX_SPACECRAFT];
static asa_state_t asa;

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

int asa_init(void) {
    if (initialized) return -1;
    asa.n_orbital = 0; asa.n_rocket = 0; asa.n_satellite = 0;
    asa.n_deep_space = 0; asa.n_spacecraft = 0;
    asa.total_kepler_law = 0; asa.total_solid_rp = 0;
    asa.total_comm_sat = 0; asa.total_planet_exp = 0;
    asa.total_manned_ship = 0;
    for (int i = 0; i < MAX_ORBITAL; i++) orbitals[i].active = 0;
    for (int i = 0; i < MAX_ROCKET; i++) rockets[i].active = 0;
    for (int i = 0; i < MAX_SATELLITE; i++) satellites[i].active = 0;
    for (int i = 0; i < MAX_DEEP_SPACE; i++) deep_spaces[i].active = 0;
    for (int i = 0; i < MAX_SPACECRAFT; i++) spacecrafts[i].active = 0;
    initialized = 1;
    print_str("[ASA] Astronautics initialized\n");
    return 0;
}

int asa_orbital(int ob_type, int cat, int kpl, int orb_tr, int rndz, int year) {
    if (asa.n_orbital >= MAX_ORBITAL) return -1;
    orbital_t* o = &orbitals[asa.n_orbital];
    o->ob_id = asa.n_orbital;
    o->ob_type = ob_type;
    o->ob_category = cat;
    o->kepler_law = kpl;
    o->orbit_trans = orb_tr;
    o->rendezvous = rndz;
    o->year = year;
    o->active = 1;
    asa.total_kepler_law += kpl;
    asa.n_orbital++;
    print_str("[ASA] Orbital "); print_int(asa.n_orbital - 1);
    print_str(" type="); print_int(ob_type);
    print_str(" cat="); print_int(cat);
    print_str(" kpl="); print_int(kpl);
    print_str(" obt="); print_int(orb_tr);
    print_str(" rnz="); print_int(rndz); print_str("\n");
    return asa.n_orbital - 1;
}

int asa_rocket(int rk_type, int cat, int sld, int lqd, int elp, int year) {
    if (asa.n_rocket >= MAX_ROCKET) return -1;
    rocket_t* r = &rockets[asa.n_rocket];
    r->rk_id = asa.n_rocket;
    r->rk_type = rk_type;
    r->rk_category = cat;
    r->solid_rp = sld;
    r->liquid_rp = lqd;
    r->electric_prop = elp;
    r->year = year;
    r->active = 1;
    asa.total_solid_rp += sld;
    asa.n_rocket++;
    print_str("[ASA] Rocket "); print_int(asa.n_rocket - 1);
    print_str(" type="); print_int(rk_type);
    print_str(" cat="); print_int(cat);
    print_str(" sld="); print_int(sld);
    print_str(" lqd="); print_int(lqd);
    print_str(" elp="); print_int(elp); print_str("\n");
    return asa.n_rocket - 1;
}

int asa_satellite(int st_type, int cat, int cms, int nvs, int rms, int year) {
    if (asa.n_satellite >= MAX_SATELLITE) return -1;
    satellite_t* s = &satellites[asa.n_satellite];
    s->st_id = asa.n_satellite;
    s->st_type = st_type;
    s->st_category = cat;
    s->comm_sat = cms;
    s->nav_sat = nvs;
    s->remote_sat = rms;
    s->year = year;
    s->active = 1;
    asa.total_comm_sat += cms;
    asa.n_satellite++;
    print_str("[ASA] Satellite "); print_int(asa.n_satellite - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" cms="); print_int(cms);
    print_str(" nvs="); print_int(nvs);
    print_str(" rms="); print_int(rms); print_str("\n");
    return asa.n_satellite - 1;
}

int asa_deep_space(int ds_type, int cat, int ple, int ast, int istr, int year) {
    if (asa.n_deep_space >= MAX_DEEP_SPACE) return -1;
    deep_space_t* d = &deep_spaces[asa.n_deep_space];
    d->ds_id = asa.n_deep_space;
    d->ds_type = ds_type;
    d->ds_category = cat;
    d->planet_exp = ple;
    d->asteroid_exp = ast;
    d->interstellar = istr;
    d->year = year;
    d->active = 1;
    asa.total_planet_exp += ple;
    asa.n_deep_space++;
    print_str("[ASA] Deep space "); print_int(asa.n_deep_space - 1);
    print_str(" type="); print_int(ds_type);
    print_str(" cat="); print_int(cat);
    print_str(" ple="); print_int(ple);
    print_str(" ast="); print_int(ast);
    print_str(" ist="); print_int(istr); print_str("\n");
    return asa.n_deep_space - 1;
}

int asa_spacecraft(int sc_type, int cat, int msh, int stst, int ssh, int year) {
    if (asa.n_spacecraft >= MAX_SPACECRAFT) return -1;
    spacecraft_t* s = &spacecrafts[asa.n_spacecraft];
    s->sc_id = asa.n_spacecraft;
    s->sc_type = sc_type;
    s->sc_category = cat;
    s->manned_ship = msh;
    s->space_station = stst;
    s->space_shuttle = ssh;
    s->year = year;
    s->active = 1;
    asa.total_manned_ship += msh;
    asa.n_spacecraft++;
    print_str("[ASA] Spacecraft "); print_int(asa.n_spacecraft - 1);
    print_str(" type="); print_int(sc_type);
    print_str(" cat="); print_int(cat);
    print_str(" msh="); print_int(msh);
    print_str(" sts="); print_int(stst);
    print_str(" ssh="); print_int(ssh); print_str("\n");
    return asa.n_spacecraft - 1;
}

void asa_orbital_report(void) {
    print_str("[ASA] Orbital report:\n");
    print_str("  Orbital mechanics categories: "); print_int(asa.n_orbital); print_str("\n");
    print_str("  Total Kepler's law: "); print_int(asa.total_kepler_law); print_str("\n");
}

void asa_rocket_report(void) {
    print_str("[ASA] Rocket report:\n");
    print_str("  Rocket propulsion categories: "); print_int(asa.n_rocket); print_str("\n");
    print_str("  Total solid rocket: "); print_int(asa.total_solid_rp); print_str("\n");
}

void asa_full_report(void) {
    print_str("[ASA] Full report:\n");
    print_str("  Satellite categories: "); print_int(asa.n_satellite); print_str("\n");
    print_str("  Total communication sat: "); print_int(asa.total_comm_sat); print_str("\n");
    print_str("  Deep space categories: "); print_int(asa.n_deep_space); print_str("\n");
    print_str("  Total planetary exploration: "); print_int(asa.total_planet_exp); print_str("\n");
    print_str("  Spacecraft categories: "); print_int(asa.n_spacecraft); print_str("\n");
    print_str("  Total manned ship: "); print_int(asa.total_manned_ship); print_str("\n");
}

void asa_print_state(void) {
    print_str("[ASA] Ob="); print_int(asa.n_orbital);
    print_str(" Rk="); print_int(asa.n_rocket);
    print_str(" St="); print_int(asa.n_satellite);
    print_str(" Ds="); print_int(asa.n_deep_space);
    print_str(" Sc="); print_int(asa.n_spacecraft);
    print_str("\n");
}

int main(void) {
    print_str("=== Astronautics Admin Demo ===\n\n");
    asa_init();

    print_str("Orbital mechanics...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int kpl = 55 + (i * 13);
        int orb_tr = 40 + (i * 10);
        int rndz = 22 + (i * 5);
        int year = 2020 + (i % 5);
        asa_orbital(type, cat, kpl, orb_tr, rndz, year);
    }

    print_str("\nRocket propulsion...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int sld = 48 + (i * 11);
        int lqd = 35 + (i * 8);
        int elp = 20 + (i * 4);
        int year = 2021 + (i % 4);
        asa_rocket(type, cat, sld, lqd, elp, year);
    }

    print_str("\nSatellites...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cms = 42 + (i * 10);
        int nvs = 28 + (i * 7);
        int rms = 18 + (i * 4);
        int year = 2022 + (i % 3);
        asa_satellite(type, cat, cms, nvs, rms, year);
    }

    print_str("\nDeep space...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ple = 35 + (i * 8);
        int ast = 25 + (i * 6);
        int istr = 15 + (i * 3);
        int year = 2023 + (i % 2);
        asa_deep_space(type, cat, ple, ast, istr, year);
    }

    print_str("\nSpacecraft...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int msh = 30 + (i * 7);
        int stst = 22 + (i * 5);
        int ssh = 12 + (i * 3);
        int year = 2024;
        asa_spacecraft(type, cat, msh, stst, ssh, year);
    }

    print_str("\nOrbital report...\n");
    asa_orbital_report();

    print_str("\nRocket report...\n");
    asa_rocket_report();

    print_str("\nFull report...\n");
    asa_full_report();

    print_str("\nFinal state...\n");
    asa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
