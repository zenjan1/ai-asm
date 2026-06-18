/* semiconductor_admin: Semiconductor administration system (v1.0)
 * Chip design, wafer manufacturing, packaging testing, equipment materials, industry applications
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

#define MAX_DESIGN       16
#define MAX_WAFER        14
#define MAX_PACKAGE      12
#define MAX_EQUIPMENT    10
#define MAX_INDUSTRY     10

typedef struct {
    int    design_id;
    int    design_type;
    int    design_category;
    int    logic_chip;
    int    memory_chip;
    int    analog_chip;
    int    year;
    int    active;
} design_t;

typedef struct {
    int    wafer_id;
    int    wafer_type;
    int    wafer_category;
    int    lithography;
    int    etching;
    int    deposition;
    int    year;
    int    active;
} wafer_t;

typedef struct {
    int    package_id;
    int    package_type;
    int    package_category;
    int    chip_package;
    int    performance;
    int    reliability;
    int    year;
    int    active;
} package_t;

typedef struct {
    int    equip_id;
    int    equip_type;
    int    equip_category;
    int    litho_machine;
    int    etch_machine;
    int    silicon_material;
    int    year;
    int    active;
} equip_t;

typedef struct {
    int    industry_id;
    int    industry_type;
    int    industry_category;
    int    consumer_elec;
    int    auto_elec;
    int    industrial_ctrl;
    int    year;
    int    active;
} industry_t;

typedef struct {
    int    n_design;
    int    n_wafer;
    int    n_package;
    int    n_equip;
    int    n_industry;
    int    total_logic;
    int    total_litho;
    int    total_chip_pkg;
    int    total_litho_machine;
    int    total_consumer;
} sca_state_t;

static design_t designs[MAX_DESIGN];
static wafer_t wafers[MAX_WAFER];
static package_t packages[MAX_PACKAGE];
static equip_t equips[MAX_EQUIPMENT];
static industry_t industries[MAX_INDUSTRY];
static sca_state_t sca;

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

int sca_init(void) {
    if (initialized) return -1;
    sca.n_design = 0; sca.n_wafer = 0; sca.n_package = 0;
    sca.n_equip = 0; sca.n_industry = 0;
    sca.total_logic = 0; sca.total_litho = 0;
    sca.total_chip_pkg = 0; sca.total_litho_machine = 0;
    sca.total_consumer = 0;
    for (int i = 0; i < MAX_DESIGN; i++) designs[i].active = 0;
    for (int i = 0; i < MAX_WAFER; i++) wafers[i].active = 0;
    for (int i = 0; i < MAX_PACKAGE; i++) packages[i].active = 0;
    for (int i = 0; i < MAX_EQUIPMENT; i++) equips[i].active = 0;
    for (int i = 0; i < MAX_INDUSTRY; i++) industries[i].active = 0;
    initialized = 1;
    print_str("[SCA] Semiconductor initialized\n");
    return 0;
}

int sca_design(int dsn_type, int cat, int logic, int mem, int analog, int year) {
    if (sca.n_design >= MAX_DESIGN) return -1;
    design_t* d = &designs[sca.n_design];
    d->design_id = sca.n_design;
    d->design_type = dsn_type;
    d->design_category = cat;
    d->logic_chip = logic;
    d->memory_chip = mem;
    d->analog_chip = analog;
    d->year = year;
    d->active = 1;
    sca.total_logic += logic;
    sca.n_design++;
    print_str("[SCA] Design "); print_int(sca.n_design - 1);
    print_str(" type="); print_int(dsn_type);
    print_str(" cat="); print_int(cat);
    print_str(" lgc="); print_int(logic);
    print_str(" mem="); print_int(mem);
    print_str(" anl="); print_int(analog); print_str("\n");
    return sca.n_design - 1;
}

int sca_wafer(int wfr_type, int cat, int lith, int etch, int dep, int year) {
    if (sca.n_wafer >= MAX_WAFER) return -1;
    wafer_t* w = &wafers[sca.n_wafer];
    w->wafer_id = sca.n_wafer;
    w->wafer_type = wfr_type;
    w->wafer_category = cat;
    w->lithography = lith;
    w->etching = etch;
    w->deposition = dep;
    w->year = year;
    w->active = 1;
    sca.total_litho += lith;
    sca.n_wafer++;
    print_str("[SCA] Wafer "); print_int(sca.n_wafer - 1);
    print_str(" type="); print_int(wfr_type);
    print_str(" cat="); print_int(cat);
    print_str(" lth="); print_int(lith);
    print_str(" etc="); print_int(etch);
    print_str(" dep="); print_int(dep); print_str("\n");
    return sca.n_wafer - 1;
}

int sca_package(int pkg_type, int cat, int chip_pkg, int perf, int rel, int year) {
    if (sca.n_package >= MAX_PACKAGE) return -1;
    package_t* p = &packages[sca.n_package];
    p->package_id = sca.n_package;
    p->package_type = pkg_type;
    p->package_category = cat;
    p->chip_package = chip_pkg;
    p->performance = perf;
    p->reliability = rel;
    p->year = year;
    p->active = 1;
    sca.total_chip_pkg += chip_pkg;
    sca.n_package++;
    print_str("[SCA] Package "); print_int(sca.n_package - 1);
    print_str(" type="); print_int(pkg_type);
    print_str(" cat="); print_int(cat);
    print_str(" cpk="); print_int(chip_pkg);
    print_str(" pef="); print_int(perf);
    print_str(" rel="); print_int(rel); print_str("\n");
    return sca.n_package - 1;
}

int sca_equipment(int eqp_type, int cat, int litho_m, int etch_m, int silicon, int year) {
    if (sca.n_equip >= MAX_EQUIPMENT) return -1;
    equip_t* e = &equips[sca.n_equip];
    e->equip_id = sca.n_equip;
    e->equip_type = eqp_type;
    e->equip_category = cat;
    e->litho_machine = litho_m;
    e->etch_machine = etch_m;
    e->silicon_material = silicon;
    e->year = year;
    e->active = 1;
    sca.total_litho_machine += litho_m;
    sca.n_equip++;
    print_str("[SCA] Equipment "); print_int(sca.n_equip - 1);
    print_str(" type="); print_int(eqp_type);
    print_str(" cat="); print_int(cat);
    print_str(" ltm="); print_int(litho_m);
    print_str(" etm="); print_int(etch_m);
    print_str(" slc="); print_int(silicon); print_str("\n");
    return sca.n_equip - 1;
}

int sca_industry(int ind_type, int cat, int consumer, int auto_c, int ind_ctrl, int year) {
    if (sca.n_industry >= MAX_INDUSTRY) return -1;
    industry_t* ind = &industries[sca.n_industry];
    ind->industry_id = sca.n_industry;
    ind->industry_type = ind_type;
    ind->industry_category = cat;
    ind->consumer_elec = consumer;
    ind->auto_elec = auto_c;
    ind->industrial_ctrl = ind_ctrl;
    ind->year = year;
    ind->active = 1;
    sca.total_consumer += consumer;
    sca.n_industry++;
    print_str("[SCA] Industry "); print_int(sca.n_industry - 1);
    print_str(" type="); print_int(ind_type);
    print_str(" cat="); print_int(cat);
    print_str(" cne="); print_int(consumer);
    print_str(" ate="); print_int(auto_c);
    print_str(" icl="); print_int(ind_ctrl); print_str("\n");
    return sca.n_industry - 1;
}

void sca_design_report(void) {
    print_str("[SCA] Design report:\n");
    print_str("  Chip design categories: "); print_int(sca.n_design); print_str("\n");
    print_str("  Total logic chips: "); print_int(sca.total_logic); print_str("\n");
}

void sca_wafer_report(void) {
    print_str("[SCA] Wafer report:\n");
    print_str("  Wafer manufacturing categories: "); print_int(sca.n_wafer); print_str("\n");
    print_str("  Total lithography: "); print_int(sca.total_litho); print_str("\n");
}

void sca_industry_report(void) {
    print_str("[SCA] Industry report:\n");
    print_str("  Packaging test categories: "); print_int(sca.n_package); print_str("\n");
    print_str("  Total chip packaging: "); print_int(sca.total_chip_pkg); print_str("\n");
    print_str("  Equipment material categories: "); print_int(sca.n_equip); print_str("\n");
    print_str("  Total lithography machines: "); print_int(sca.total_litho_machine); print_str("\n");
    print_str("  Industry application categories: "); print_int(sca.n_industry); print_str("\n");
    print_str("  Total consumer electronics: "); print_int(sca.total_consumer); print_str("\n");
}

void sca_print_state(void) {
    print_str("[SCA] Ds="); print_int(sca.n_design);
    print_str(" Wf="); print_int(sca.n_wafer);
    print_str(" Pk="); print_int(sca.n_package);
    print_str(" Eq="); print_int(sca.n_equip);
    print_str(" In="); print_int(sca.n_industry);
    print_str("\n");
}

int main(void) {
    print_str("=== Semiconductor Admin Demo ===\n\n");
    sca_init();

    print_str("Chip design...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int lgc = 55 + (i * 14);
        int mem = 40 + (i * 10);
        int anl = 25 + (i * 6);
        int year = 2020 + (i % 5);
        sca_design(type, cat, lgc, mem, anl, year);
    }

    print_str("\nWafer manufacturing...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int lth = 45 + (i * 11);
        int etc = 30 + (i * 7);
        int dep = 20 + (i * 5);
        int year = 2021 + (i % 4);
        sca_wafer(type, cat, lth, etc, dep, year);
    }

    print_str("\nPackaging test...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cpk = 38 + (i * 9);
        int pef = 25 + (i * 6);
        int rel = 18 + (i * 4);
        int year = 2022 + (i % 3);
        sca_package(type, cat, cpk, pef, rel, year);
    }

    print_str("\nEquipment materials...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ltm = 30 + (i * 8);
        int etm = 22 + (i * 5);
        int slc = 15 + (i * 3);
        int year = 2023 + (i % 2);
        sca_equipment(type, cat, ltm, etm, slc, year);
    }

    print_str("\nIndustry applications...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cne = 35 + (i * 9);
        int ate = 25 + (i * 6);
        int icl = 18 + (i * 4);
        int year = 2024;
        sca_industry(type, cat, cne, ate, icl, year);
    }

    print_str("\nDesign report...\n");
    sca_design_report();

    print_str("\nWafer report...\n");
    sca_wafer_report();

    print_str("\nIndustry report...\n");
    sca_industry_report();

    print_str("\nFinal state...\n");
    sca_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
