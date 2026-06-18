/* nanotechnology_admin: Nanotechnology administration system (v1.0)
 * Nanomaterials, nanodevices, nanobiotech, nanoenergy, nanosafety
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

#define MAX_MATERIAL   16
#define MAX_DEVICE     14
#define MAX_NANOBIO    12
#define MAX_ENERGY     10
#define MAX_SAFETY     10

typedef struct {
    int    material_id;
    int    material_type;
    int    material_category;
    int    nanoparticle;
    int    nanowire;
    int    nanofilm;
    int    year;
    int    active;
} material_t;

typedef struct {
    int    device_id;
    int    device_type;
    int    device_category;
    int    nano_sensor;
    int    nano_actuator;
    int    nano_motor;
    int    year;
    int    active;
} device_t;

typedef struct {
    int    nanobio_id;
    int    nanobio_type;
    int    nanobio_category;
    int    nano_drug;
    int    nano_diagnose;
    int    nano_imaging;
    int    year;
    int    active;
} nanobio_t;

typedef struct {
    int    energy_id;
    int    energy_type;
    int    energy_category;
    int    nano_solar;
    int    nano_battery;
    int    nano_capacitor;
    int    year;
    int    active;
} energy_t;

typedef struct {
    int    safety_id;
    int    safety_type;
    int    safety_category;
    int    nano_toxic;
    int    nano_protect;
    int    nano_monitor;
    int    year;
    int    active;
} safety_t;

typedef struct {
    int    n_material;
    int    n_device;
    int    n_nanobio;
    int    n_energy;
    int    n_safety;
    int    total_particle;
    int    total_sensor;
    int    total_drug;
    int    total_solar;
    int    total_toxic;
} nta_state_t;

static material_t materials[MAX_MATERIAL];
static device_t devices[MAX_DEVICE];
static nanobio_t nanobios[MAX_NANOBIO];
static energy_t energies[MAX_ENERGY];
static safety_t safeties[MAX_SAFETY];
static nta_state_t nta;

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

int nta_init(void) {
    if (initialized) return -1;
    nta.n_material = 0; nta.n_device = 0; nta.n_nanobio = 0;
    nta.n_energy = 0; nta.n_safety = 0;
    nta.total_particle = 0; nta.total_sensor = 0;
    nta.total_drug = 0; nta.total_solar = 0;
    nta.total_toxic = 0;
    for (int i = 0; i < MAX_MATERIAL; i++) materials[i].active = 0;
    for (int i = 0; i < MAX_DEVICE; i++) devices[i].active = 0;
    for (int i = 0; i < MAX_NANOBIO; i++) nanobios[i].active = 0;
    for (int i = 0; i < MAX_ENERGY; i++) energies[i].active = 0;
    for (int i = 0; i < MAX_SAFETY; i++) safeties[i].active = 0;
    initialized = 1;
    print_str("[NTA] Nanotechnology initialized\n");
    return 0;
}

int nta_material(int mat_type, int cat, int particle, int wire, int film, int year) {
    if (nta.n_material >= MAX_MATERIAL) return -1;
    material_t* m = &materials[nta.n_material];
    m->material_id = nta.n_material;
    m->material_type = mat_type;
    m->material_category = cat;
    m->nanoparticle = particle;
    m->nanowire = wire;
    m->nanofilm = film;
    m->year = year;
    m->active = 1;
    nta.total_particle += particle;
    nta.n_material++;
    print_str("[NTA] Material "); print_int(nta.n_material - 1);
    print_str(" type="); print_int(mat_type);
    print_str(" cat="); print_int(cat);
    print_str(" prt="); print_int(particle);
    print_str(" wir="); print_int(wire);
    print_str(" flm="); print_int(film); print_str("\n");
    return nta.n_material - 1;
}

int nta_device(int dev_type, int cat, int sensor, int actuator, int motor, int year) {
    if (nta.n_device >= MAX_DEVICE) return -1;
    device_t* d = &devices[nta.n_device];
    d->device_id = nta.n_device;
    d->device_type = dev_type;
    d->device_category = cat;
    d->nano_sensor = sensor;
    d->nano_actuator = actuator;
    d->nano_motor = motor;
    d->year = year;
    d->active = 1;
    nta.total_sensor += sensor;
    nta.n_device++;
    print_str("[NTA] Device "); print_int(nta.n_device - 1);
    print_str(" type="); print_int(dev_type);
    print_str(" cat="); print_int(cat);
    print_str(" sns="); print_int(sensor);
    print_str(" act="); print_int(actuator);
    print_str(" mtr="); print_int(motor); print_str("\n");
    return nta.n_device - 1;
}

int nta_nanobio(int nb_type, int cat, int drug, int diagnose, int imaging, int year) {
    if (nta.n_nanobio >= MAX_NANOBIO) return -1;
    nanobio_t* n = &nanobios[nta.n_nanobio];
    n->nanobio_id = nta.n_nanobio;
    n->nanobio_type = nb_type;
    n->nanobio_category = cat;
    n->nano_drug = drug;
    n->nano_diagnose = diagnose;
    n->nano_imaging = imaging;
    n->year = year;
    n->active = 1;
    nta.total_drug += drug;
    nta.n_nanobio++;
    print_str("[NTA] Nanobio "); print_int(nta.n_nanobio - 1);
    print_str(" type="); print_int(nb_type);
    print_str(" cat="); print_int(cat);
    print_str(" drg="); print_int(drug);
    print_str(" dgn="); print_int(diagnose);
    print_str(" img="); print_int(imaging); print_str("\n");
    return nta.n_nanobio - 1;
}

int nta_energy(int en_type, int cat, int solar, int battery, int cap, int year) {
    if (nta.n_energy >= MAX_ENERGY) return -1;
    energy_t* e = &energies[nta.n_energy];
    e->energy_id = nta.n_energy;
    e->energy_type = en_type;
    e->energy_category = cat;
    e->nano_solar = solar;
    e->nano_battery = battery;
    e->nano_capacitor = cap;
    e->year = year;
    e->active = 1;
    nta.total_solar += solar;
    nta.n_energy++;
    print_str("[NTA] Energy "); print_int(nta.n_energy - 1);
    print_str(" type="); print_int(en_type);
    print_str(" cat="); print_int(cat);
    print_str(" slr="); print_int(solar);
    print_str(" bat="); print_int(battery);
    print_str(" cap="); print_int(cap); print_str("\n");
    return nta.n_energy - 1;
}

int nta_safety(int sf_type, int cat, int toxic, int protect, int monitor, int year) {
    if (nta.n_safety >= MAX_SAFETY) return -1;
    safety_t* s = &safeties[nta.n_safety];
    s->safety_id = nta.n_safety;
    s->safety_type = sf_type;
    s->safety_category = cat;
    s->nano_toxic = toxic;
    s->nano_protect = protect;
    s->nano_monitor = monitor;
    s->year = year;
    s->active = 1;
    nta.total_toxic += toxic;
    nta.n_safety++;
    print_str("[NTA] Safety "); print_int(nta.n_safety - 1);
    print_str(" type="); print_int(sf_type);
    print_str(" cat="); print_int(cat);
    print_str(" txc="); print_int(toxic);
    print_str(" prt="); print_int(protect);
    print_str(" mon="); print_int(monitor); print_str("\n");
    return nta.n_safety - 1;
}

void nta_material_report(void) {
    print_str("[NTA] Material report:\n");
    print_str("  Nanomaterial categories: "); print_int(nta.n_material); print_str("\n");
    print_str("  Total nanoparticles: "); print_int(nta.total_particle); print_str("\n");
}

void nta_device_report(void) {
    print_str("[NTA] Device report:\n");
    print_str("  Nanodevice categories: "); print_int(nta.n_device); print_str("\n");
    print_str("  Total nano sensors: "); print_int(nta.total_sensor); print_str("\n");
}

void nta_safety_report(void) {
    print_str("[NTA] Safety report:\n");
    print_str("  Nanobio categories: "); print_int(nta.n_nanobio); print_str("\n");
    print_str("  Total nano drugs: "); print_int(nta.total_drug); print_str("\n");
    print_str("  Nanoenergy categories: "); print_int(nta.n_energy); print_str("\n");
    print_str("  Total nano solar: "); print_int(nta.total_solar); print_str("\n");
    print_str("  Nanosafety categories: "); print_int(nta.n_safety); print_str("\n");
    print_str("  Total nano toxic: "); print_int(nta.total_toxic); print_str("\n");
}

void nta_print_state(void) {
    print_str("[NTA] Mt="); print_int(nta.n_material);
    print_str(" Dv="); print_int(nta.n_device);
    print_str(" Nb="); print_int(nta.n_nanobio);
    print_str(" En="); print_int(nta.n_energy);
    print_str(" Sf="); print_int(nta.n_safety);
    print_str("\n");
}

int main(void) {
    print_str("=== Nanotechnology Admin Demo ===\n\n");
    nta_init();

    print_str("Nanomaterials...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int prt = 60 + (i * 15);
        int wir = 40 + (i * 10);
        int flm = 25 + (i * 6);
        int year = 2020 + (i % 5);
        nta_material(type, cat, prt, wir, flm, year);
    }

    print_str("\nNanodevices...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int sns = 45 + (i * 11);
        int act = 30 + (i * 7);
        int mtr = 20 + (i * 5);
        int year = 2021 + (i % 4);
        nta_device(type, cat, sns, act, mtr, year);
    }

    print_str("\nNanobiotech...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int drg = 38 + (i * 9);
        int dgn = 25 + (i * 6);
        int img = 18 + (i * 4);
        int year = 2022 + (i % 3);
        nta_nanobio(type, cat, drg, dgn, img, year);
    }

    print_str("\nNanoenergy...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int slr = 32 + (i * 8);
        int bat = 22 + (i * 5);
        int cap_c = 15 + (i * 3);
        int year = 2023 + (i % 2);
        nta_energy(type, cat, slr, bat, cap_c, year);
    }

    print_str("\nNanosafety...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int txc = 25 + (i * 6);
        int prt = 18 + (i * 4);
        int mon = 12 + (i * 3);
        int year = 2024;
        nta_safety(type, cat, txc, prt, mon, year);
    }

    print_str("\nMaterial report...\n");
    nta_material_report();

    print_str("\nDevice report...\n");
    nta_device_report();

    print_str("\nSafety report...\n");
    nta_safety_report();

    print_str("\nFinal state...\n");
    nta_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
