/* synthetic_biology_admin: Synthetic biology administration system (v1.0)
 * Biological parts, gene circuits, metabolic engineering, cell factories, biosafety
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

#define MAX_PARTS        16
#define MAX_CIRCUITS     14
#define MAX_METABOLIC    12
#define MAX_FACTORY      10
#define MAX_BIOSAFETY    10

typedef struct {
    int    parts_id;
    int    parts_type;
    int    parts_category;
    int    promoter;
    int    terminator;
    int    rbs;
    int    year;
    int    active;
} parts_t;

typedef struct {
    int    circuit_id;
    int    circuit_type;
    int    circuit_category;
    int    logic_gate;
    int    oscillator;
    int    sensor;
    int    year;
    int    active;
} circuit_t;

typedef struct {
    int    metabolic_id;
    int    metabolic_type;
    int    metabolic_category;
    int    pathway_design;
    int    flux_analysis;
    int    metabolic_opt;
    int    year;
    int    active;
} metabolic_t;

typedef struct {
    int    factory_id;
    int    factory_type;
    int    factory_category;
    int    microbial;
    int    plant;
    int    animal;
    int    year;
    int    active;
} factory_t;

typedef struct {
    int    biosafety_id;
    int    biosafety_type;
    int    biosafety_category;
    int    safety_assess;
    int    protection;
    int    ethics_review;
    int    year;
    int    active;
} biosafety_t;

typedef struct {
    int    n_parts;
    int    n_circuits;
    int    n_metabolic;
    int    n_factory;
    int    n_biosafety;
    int    total_promoter;
    int    total_logic;
    int    total_pathway;
    int    total_microbial;
    int    total_safety;
} sba_state_t;

static parts_t parts_list[MAX_PARTS];
static circuit_t circuits[MAX_CIRCUITS];
static metabolic_t metabolics[MAX_METABOLIC];
static factory_t factories[MAX_FACTORY];
static biosafety_t biosafeties[MAX_BIOSAFETY];
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
    sba.n_parts = 0; sba.n_circuits = 0; sba.n_metabolic = 0;
    sba.n_factory = 0; sba.n_biosafety = 0;
    sba.total_promoter = 0; sba.total_logic = 0;
    sba.total_pathway = 0; sba.total_microbial = 0;
    sba.total_safety = 0;
    for (int i = 0; i < MAX_PARTS; i++) parts_list[i].active = 0;
    for (int i = 0; i < MAX_CIRCUITS; i++) circuits[i].active = 0;
    for (int i = 0; i < MAX_METABOLIC; i++) metabolics[i].active = 0;
    for (int i = 0; i < MAX_FACTORY; i++) factories[i].active = 0;
    for (int i = 0; i < MAX_BIOSAFETY; i++) biosafeties[i].active = 0;
    initialized = 1;
    print_str("[SBA] Synthetic biology initialized\n");
    return 0;
}

int sba_parts(int prt_type, int cat, int promo, int term, int ribo, int year) {
    if (sba.n_parts >= MAX_PARTS) return -1;
    parts_t* p = &parts_list[sba.n_parts];
    p->parts_id = sba.n_parts;
    p->parts_type = prt_type;
    p->parts_category = cat;
    p->promoter = promo;
    p->terminator = term;
    p->rbs = ribo;
    p->year = year;
    p->active = 1;
    sba.total_promoter += promo;
    sba.n_parts++;
    print_str("[SBA] Parts "); print_int(sba.n_parts - 1);
    print_str(" type="); print_int(prt_type);
    print_str(" cat="); print_int(cat);
    print_str(" prm="); print_int(promo);
    print_str(" trm="); print_int(term);
    print_str(" rbs="); print_int(ribo); print_str("\n");
    return sba.n_parts - 1;
}

int sba_circuits(int crct_type, int cat, int logic, int osc, int sensor, int year) {
    if (sba.n_circuits >= MAX_CIRCUITS) return -1;
    circuit_t* c = &circuits[sba.n_circuits];
    c->circuit_id = sba.n_circuits;
    c->circuit_type = crct_type;
    c->circuit_category = cat;
    c->logic_gate = logic;
    c->oscillator = osc;
    c->sensor = sensor;
    c->year = year;
    c->active = 1;
    sba.total_logic += logic;
    sba.n_circuits++;
    print_str("[SBA] Circuit "); print_int(sba.n_circuits - 1);
    print_str(" type="); print_int(crct_type);
    print_str(" cat="); print_int(cat);
    print_str(" lgc="); print_int(logic);
    print_str(" osc="); print_int(osc);
    print_str(" sns="); print_int(sensor); print_str("\n");
    return sba.n_circuits - 1;
}

int sba_metabolic(int mtc_type, int cat, int pathway, int flux, int optimize, int year) {
    if (sba.n_metabolic >= MAX_METABOLIC) return -1;
    metabolic_t* m = &metabolics[sba.n_metabolic];
    m->metabolic_id = sba.n_metabolic;
    m->metabolic_type = mtc_type;
    m->metabolic_category = cat;
    m->pathway_design = pathway;
    m->flux_analysis = flux;
    m->metabolic_opt = optimize;
    m->year = year;
    m->active = 1;
    sba.total_pathway += pathway;
    sba.n_metabolic++;
    print_str("[SBA] Metabolic "); print_int(sba.n_metabolic - 1);
    print_str(" type="); print_int(mtc_type);
    print_str(" cat="); print_int(cat);
    print_str(" pth="); print_int(pathway);
    print_str(" flx="); print_int(flux);
    print_str(" opt="); print_int(optimize); print_str("\n");
    return sba.n_metabolic - 1;
}

int sba_factory(int fct_type, int cat, int microb, int plant_c, int animal_c, int year) {
    if (sba.n_factory >= MAX_FACTORY) return -1;
    factory_t* f = &factories[sba.n_factory];
    f->factory_id = sba.n_factory;
    f->factory_type = fct_type;
    f->factory_category = cat;
    f->microbial = microb;
    f->plant = plant_c;
    f->animal = animal_c;
    f->year = year;
    f->active = 1;
    sba.total_microbial += microb;
    sba.n_factory++;
    print_str("[SBA] Factory "); print_int(sba.n_factory - 1);
    print_str(" type="); print_int(fct_type);
    print_str(" cat="); print_int(cat);
    print_str(" mcr="); print_int(microb);
    print_str(" plt="); print_int(plant_c);
    print_str(" anm="); print_int(animal_c); print_str("\n");
    return sba.n_factory - 1;
}

int sba_biosafety(int bio_type, int cat, int assess, int protect, int ethics, int year) {
    if (sba.n_biosafety >= MAX_BIOSAFETY) return -1;
    biosafety_t* b = &biosafeties[sba.n_biosafety];
    b->biosafety_id = sba.n_biosafety;
    b->biosafety_type = bio_type;
    b->biosafety_category = cat;
    b->safety_assess = assess;
    b->protection = protect;
    b->ethics_review = ethics;
    b->year = year;
    b->active = 1;
    sba.total_safety += assess;
    sba.n_biosafety++;
    print_str("[SBA] Biosafety "); print_int(sba.n_biosafety - 1);
    print_str(" type="); print_int(bio_type);
    print_str(" cat="); print_int(cat);
    print_str(" ass="); print_int(assess);
    print_str(" prt="); print_int(protect);
    print_str(" eth="); print_int(ethics); print_str("\n");
    return sba.n_biosafety - 1;
}

void sba_parts_report(void) {
    print_str("[SBA] Parts report:\n");
    print_str("  Biological parts categories: "); print_int(sba.n_parts); print_str("\n");
    print_str("  Total promoters: "); print_int(sba.total_promoter); print_str("\n");
}

void sba_circuits_report(void) {
    print_str("[SBA] Circuits report:\n");
    print_str("  Gene circuit categories: "); print_int(sba.n_circuits); print_str("\n");
    print_str("  Total logic gates: "); print_int(sba.total_logic); print_str("\n");
}

void sba_factory_report(void) {
    print_str("[SBA] Factory report:\n");
    print_str("  Metabolic engineering categories: "); print_int(sba.n_metabolic); print_str("\n");
    print_str("  Total pathway design: "); print_int(sba.total_pathway); print_str("\n");
    print_str("  Cell factory categories: "); print_int(sba.n_factory); print_str("\n");
    print_str("  Total microbial factories: "); print_int(sba.total_microbial); print_str("\n");
    print_str("  Biosafety categories: "); print_int(sba.n_biosafety); print_str("\n");
    print_str("  Total safety assessment: "); print_int(sba.total_safety); print_str("\n");
}

void sba_print_state(void) {
    print_str("[SBA] Pt="); print_int(sba.n_parts);
    print_str(" Cr="); print_int(sba.n_circuits);
    print_str(" Mt="); print_int(sba.n_metabolic);
    print_str(" Fc="); print_int(sba.n_factory);
    print_str(" Bs="); print_int(sba.n_biosafety);
    print_str("\n");
}

int main(void) {
    print_str("=== Synthetic Biology Admin Demo ===\n\n");
    sba_init();

    print_str("Biological parts...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int prm = 65 + (i * 16);
        int trm = 42 + (i * 10);
        int rbs = 28 + (i * 7);
        int year = 2020 + (i % 5);
        sba_parts(type, cat, prm, trm, rbs, year);
    }

    print_str("\nGene circuits...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int lgc = 50 + (i * 12);
        int osc = 35 + (i * 8);
        int sns = 22 + (i * 5);
        int year = 2021 + (i % 4);
        sba_circuits(type, cat, lgc, osc, sns, year);
    }

    print_str("\nMetabolic engineering...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pth = 40 + (i * 10);
        int flx = 28 + (i * 7);
        int opt = 18 + (i * 4);
        int year = 2022 + (i % 3);
        sba_metabolic(type, cat, pth, flx, opt, year);
    }

    print_str("\nCell factories...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int mcr = 35 + (i * 8);
        int plt = 25 + (i * 6);
        int anm = 18 + (i * 4);
        int year = 2023 + (i % 2);
        sba_factory(type, cat, mcr, plt, anm, year);
    }

    print_str("\nBiosafety...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ass = 28 + (i * 7);
        int prt = 20 + (i * 5);
        int eth = 12 + (i * 3);
        int year = 2024;
        sba_biosafety(type, cat, ass, prt, eth, year);
    }

    print_str("\nParts report...\n");
    sba_parts_report();

    print_str("\nCircuits report...\n");
    sba_circuits_report();

    print_str("\nFactory report...\n");
    sba_factory_report();

    print_str("\nFinal state...\n");
    sba_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
