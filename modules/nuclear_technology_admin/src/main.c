/* nuclear_technology_admin: Nuclear technology administration system (v1.0)
 * Nuclear power, fuel cycle, nuclear safety, nuclear medicine, nuclear fusion
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

#define MAX_POWER      16
#define MAX_FUEL       14
#define MAX_SAFETY     12
#define MAX_MEDICINE   10
#define MAX_FUSION     10

typedef struct {
    int    power_id;
    int    power_type;
    int    power_category;
    int    pwr_reactor;
    int    bwr_reactor;
    int    hwr_reactor;
    int    year;
    int    active;
} power_t;

typedef struct {
    int    fuel_id;
    int    fuel_type;
    int    fuel_category;
    int    uranium_mine;
    int    enrichment;
    int    fuel_assembly;
    int    year;
    int    active;
} fuel_t;

typedef struct {
    int    safety_id;
    int    safety_type;
    int    safety_category;
    int    radiation_protect;
    int    emergency_resp;
    int    waste_dispose;
    int    year;
    int    active;
} safety_t;

typedef struct {
    int    med_id;
    int    med_type;
    int    med_category;
    int    radio_diagnose;
    int    radio_therapy;
    int    nuclide_drug;
    int    year;
    int    active;
} medicine_t;

typedef struct {
    int    fusion_id;
    int    fusion_type;
    int    fusion_category;
    int    tokamak;
    int    inertial;
    int    magnetic_conf;
    int    year;
    int    active;
} fusion_t;

typedef struct {
    int    n_power;
    int    n_fuel;
    int    n_safety;
    int    n_medicine;
    int    n_fusion;
    int    total_pwr;
    int    total_mine;
    int    total_radiation;
    int    total_diagnose;
    int    total_tokamak;
} nta_state_t;

static power_t powers[MAX_POWER];
static fuel_t fuels[MAX_FUEL];
static safety_t safety_list[MAX_SAFETY];
static medicine_t medicines[MAX_MEDICINE];
static fusion_t fusions[MAX_FUSION];
static nta_state_t ntk;

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

int ntk_init(void) {
    if (initialized) return -1;
    ntk.n_power = 0; ntk.n_fuel = 0; ntk.n_safety = 0;
    ntk.n_medicine = 0; ntk.n_fusion = 0;
    ntk.total_pwr = 0; ntk.total_mine = 0;
    ntk.total_radiation = 0; ntk.total_diagnose = 0;
    ntk.total_tokamak = 0;
    for (int i = 0; i < MAX_POWER; i++) powers[i].active = 0;
    for (int i = 0; i < MAX_FUEL; i++) fuels[i].active = 0;
    for (int i = 0; i < MAX_SAFETY; i++) safety_list[i].active = 0;
    for (int i = 0; i < MAX_MEDICINE; i++) medicines[i].active = 0;
    for (int i = 0; i < MAX_FUSION; i++) fusions[i].active = 0;
    initialized = 1;
    print_str("[NTK] Nuclear technology initialized\n");
    return 0;
}

int ntk_power(int pw_type, int cat, int pwr, int bwr, int hwr, int year) {
    if (ntk.n_power >= MAX_POWER) return -1;
    power_t* p = &powers[ntk.n_power];
    p->power_id = ntk.n_power;
    p->power_type = pw_type;
    p->power_category = cat;
    p->pwr_reactor = pwr;
    p->bwr_reactor = bwr;
    p->hwr_reactor = hwr;
    p->year = year;
    p->active = 1;
    ntk.total_pwr += pwr;
    ntk.n_power++;
    print_str("[NTK] Power "); print_int(ntk.n_power - 1);
    print_str(" type="); print_int(pw_type);
    print_str(" cat="); print_int(cat);
    print_str(" pwr="); print_int(pwr);
    print_str(" bwr="); print_int(bwr);
    print_str(" hwr="); print_int(hwr); print_str("\n");
    return ntk.n_power - 1;
}

int ntk_fuel(int fl_type, int cat, int mine, int enrich, int assembly, int year) {
    if (ntk.n_fuel >= MAX_FUEL) return -1;
    fuel_t* f = &fuels[ntk.n_fuel];
    f->fuel_id = ntk.n_fuel;
    f->fuel_type = fl_type;
    f->fuel_category = cat;
    f->uranium_mine = mine;
    f->enrichment = enrich;
    f->fuel_assembly = assembly;
    f->year = year;
    f->active = 1;
    ntk.total_mine += mine;
    ntk.n_fuel++;
    print_str("[NTK] Fuel "); print_int(ntk.n_fuel - 1);
    print_str(" type="); print_int(fl_type);
    print_str(" cat="); print_int(cat);
    print_str(" mne="); print_int(mine);
    print_str(" enr="); print_int(enrich);
    print_str(" asm="); print_int(assembly); print_str("\n");
    return ntk.n_fuel - 1;
}

int ntk_safety(int sf_type, int cat, int rad_prot, int emergency, int waste_d, int year) {
    if (ntk.n_safety >= MAX_SAFETY) return -1;
    safety_t* s = &safety_list[ntk.n_safety];
    s->safety_id = ntk.n_safety;
    s->safety_type = sf_type;
    s->safety_category = cat;
    s->radiation_protect = rad_prot;
    s->emergency_resp = emergency;
    s->waste_dispose = waste_d;
    s->year = year;
    s->active = 1;
    ntk.total_radiation += rad_prot;
    ntk.n_safety++;
    print_str("[NTK] Safety "); print_int(ntk.n_safety - 1);
    print_str(" type="); print_int(sf_type);
    print_str(" cat="); print_int(cat);
    print_str(" rad="); print_int(rad_prot);
    print_str(" emg="); print_int(emergency);
    print_str(" wst="); print_int(waste_d); print_str("\n");
    return ntk.n_safety - 1;
}

int ntk_medicine(int md_type, int cat, int diagnose, int therapy, int nuclide, int year) {
    if (ntk.n_medicine >= MAX_MEDICINE) return -1;
    medicine_t* m = &medicines[ntk.n_medicine];
    m->med_id = ntk.n_medicine;
    m->med_type = md_type;
    m->med_category = cat;
    m->radio_diagnose = diagnose;
    m->radio_therapy = therapy;
    m->nuclide_drug = nuclide;
    m->year = year;
    m->active = 1;
    ntk.total_diagnose += diagnose;
    ntk.n_medicine++;
    print_str("[NTK] Medicine "); print_int(ntk.n_medicine - 1);
    print_str(" type="); print_int(md_type);
    print_str(" cat="); print_int(cat);
    print_str(" dgn="); print_int(diagnose);
    print_str(" thp="); print_int(therapy);
    print_str(" nuc="); print_int(nuclide); print_str("\n");
    return ntk.n_medicine - 1;
}

int ntk_fusion(int fs_type, int cat, int tokamk, int inertial_c, int mag_conf, int year) {
    if (ntk.n_fusion >= MAX_FUSION) return -1;
    fusion_t* f = &fusions[ntk.n_fusion];
    f->fusion_id = ntk.n_fusion;
    f->fusion_type = fs_type;
    f->fusion_category = cat;
    f->tokamak = tokamk;
    f->inertial = inertial_c;
    f->magnetic_conf = mag_conf;
    f->year = year;
    f->active = 1;
    ntk.total_tokamak += tokamk;
    ntk.n_fusion++;
    print_str("[NTK] Fusion "); print_int(ntk.n_fusion - 1);
    print_str(" type="); print_int(fs_type);
    print_str(" cat="); print_int(cat);
    print_str(" tkk="); print_int(tokamk);
    print_str(" irt="); print_int(inertial_c);
    print_str(" mgn="); print_int(mag_conf); print_str("\n");
    return ntk.n_fusion - 1;
}

void ntk_power_report(void) {
    print_str("[NTK] Power report:\n");
    print_str("  Nuclear power categories: "); print_int(ntk.n_power); print_str("\n");
    print_str("  Total PWR reactors: "); print_int(ntk.total_pwr); print_str("\n");
}

void ntk_fuel_report(void) {
    print_str("[NTK] Fuel report:\n");
    print_str("  Fuel cycle categories: "); print_int(ntk.n_fuel); print_str("\n");
    print_str("  Total uranium mines: "); print_int(ntk.total_mine); print_str("\n");
}

void ntk_fusion_report(void) {
    print_str("[NTK] Fusion report:\n");
    print_str("  Nuclear safety categories: "); print_int(ntk.n_safety); print_str("\n");
    print_str("  Total radiation protection: "); print_int(ntk.total_radiation); print_str("\n");
    print_str("  Nuclear medicine categories: "); print_int(ntk.n_medicine); print_str("\n");
    print_str("  Total radio diagnosis: "); print_int(ntk.total_diagnose); print_str("\n");
    print_str("  Nuclear fusion categories: "); print_int(ntk.n_fusion); print_str("\n");
    print_str("  Total tokamak: "); print_int(ntk.total_tokamak); print_str("\n");
}

void ntk_print_state(void) {
    print_str("[NTK] Pw="); print_int(ntk.n_power);
    print_str(" Fl="); print_int(ntk.n_fuel);
    print_str(" Sf="); print_int(ntk.n_safety);
    print_str(" Md="); print_int(ntk.n_medicine);
    print_str(" Fs="); print_int(ntk.n_fusion);
    print_str("\n");
}

int main(void) {
    print_str("=== Nuclear Technology Admin Demo ===\n\n");
    ntk_init();

    print_str("Nuclear power...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int pwr = 58 + (i * 14);
        int bwr = 42 + (i * 10);
        int hwr = 28 + (i * 6);
        int year = 2020 + (i % 5);
        ntk_power(type, cat, pwr, bwr, hwr, year);
    }

    print_str("\nFuel cycle...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int mne = 48 + (i * 12);
        int enr = 34 + (i * 8);
        int asmb = 22 + (i * 5);
        int year = 2021 + (i % 4);
        ntk_fuel(type, cat, mne, enr, asmb, year);
    }

    print_str("\nNuclear safety...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int rad = 45 + (i * 11);
        int emg = 30 + (i * 7);
        int wst = 20 + (i * 4);
        int year = 2022 + (i % 3);
        ntk_safety(type, cat, rad, emg, wst, year);
    }

    print_str("\nNuclear medicine...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int dgn = 35 + (i * 8);
        int thp = 25 + (i * 6);
        int nuc = 15 + (i * 3);
        int year = 2023 + (i % 2);
        ntk_medicine(type, cat, dgn, thp, nuc, year);
    }

    print_str("\nNuclear fusion...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int tkk = 30 + (i * 7);
        int irt = 22 + (i * 5);
        int mgn = 12 + (i * 3);
        int year = 2024;
        ntk_fusion(type, cat, tkk, irt, mgn, year);
    }

    print_str("\nPower report...\n");
    ntk_power_report();

    print_str("\nFuel report...\n");
    ntk_fuel_report();

    print_str("\nFusion report...\n");
    ntk_fusion_report();

    print_str("\nFinal state...\n");
    ntk_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
