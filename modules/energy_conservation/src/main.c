/* energy_conservation: Energy conservation & emission reduction administration system (v1.0)
 * Energy saving, emission reduction, clean production, circular economy, carbon trading
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

#define MAX_ENERGY       16
#define MAX_EMISSION     14
#define MAX_CLEAN        12
#define MAX_CIRCULAR     10
#define MAX_CARBON       10

typedef struct {
    int    energy_id;
    int    energy_type;
    int    sector_id;
    int    target_saving;
    int    actual_saving;
    int    monitoring_points;
    int    year;
    int    active;
} energy_t;

typedef struct {
    int    emission_id;
    int    pollutant_type;
    int    source_category;
    int    emission_volume;
    int    reduction_achieved;
    int    treatment_projects;
    int    year;
    int    active;
} emission_t;

typedef struct {
    int    clean_id;
    int    industry_type;
    int    audit_level;
    int    enterprises_audited;
    int    technologies_promoted;
    int    upgrades_completed;
    int    year;
    int    active;
} clean_t;

typedef struct {
    int    circular_id;
    int    resource_type;
    int    recycling_category;
    int    recycling_volume;
    int    waste_utilization;
    int    renewable_output;
    int    year;
    int    active;
} circular_t;

typedef struct {
    int    carbon_id;
    int    carbon_category;
    int    trading_type;
    int    emissions_monitored;
    int    allowances_allocated;
    int    transactions_completed;
    int    year;
    int    active;
} carbon_t;

typedef struct {
    int    n_energy;
    int    n_emission;
    int    n_clean;
    int    n_circular;
    int    n_carbon;
    int    total_saving;
    int    total_reduction;
    int    total_enterprises;
    int    total_recycling;
    int    total_emissions;
} ec_state_t;

static energy_t energies[MAX_ENERGY];
static emission_t emissions_list[MAX_EMISSION];
static clean_t cleans[MAX_CLEAN];
static circular_t circulars[MAX_CIRCULAR];
static carbon_t carbons[MAX_CARBON];
static ec_state_t ec;

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

int ec_init(void) {
    if (initialized) return -1;
    ec.n_energy = 0; ec.n_emission = 0; ec.n_clean = 0;
    ec.n_circular = 0; ec.n_carbon = 0;
    ec.total_saving = 0; ec.total_reduction = 0;
    ec.total_enterprises = 0; ec.total_recycling = 0;
    ec.total_emissions = 0;
    for (int i = 0; i < MAX_ENERGY; i++) energies[i].active = 0;
    for (int i = 0; i < MAX_EMISSION; i++) emissions_list[i].active = 0;
    for (int i = 0; i < MAX_CLEAN; i++) cleans[i].active = 0;
    for (int i = 0; i < MAX_CIRCULAR; i++) circulars[i].active = 0;
    for (int i = 0; i < MAX_CARBON; i++) carbons[i].active = 0;
    initialized = 1;
    print_str("[EC] Energy conservation initialized\n");
    return 0;
}

int ec_energy(int eng_type, int sector, int target, int actual, int points, int year) {
    if (ec.n_energy >= MAX_ENERGY) return -1;
    energy_t* e = &energies[ec.n_energy];
    e->energy_id = ec.n_energy;
    e->energy_type = eng_type;
    e->sector_id = sector;
    e->target_saving = target;
    e->actual_saving = actual;
    e->monitoring_points = points;
    e->year = year;
    e->active = 1;
    ec.total_saving += actual;
    ec.n_energy++;
    print_str("[EC] Energy "); print_int(ec.n_energy - 1);
    print_str(" type="); print_int(eng_type);
    print_str(" sec="); print_int(sector);
    print_str(" tgt="); print_int(target);
    print_str(" act="); print_int(actual);
    print_str(" pts="); print_int(points); print_str("\n");
    return ec.n_energy - 1;
}

int ec_emission(int poll_type, int source, int volume, int reduction, int projects, int year) {
    if (ec.n_emission >= MAX_EMISSION) return -1;
    emission_t* em = &emissions_list[ec.n_emission];
    em->emission_id = ec.n_emission;
    em->pollutant_type = poll_type;
    em->source_category = source;
    em->emission_volume = volume;
    em->reduction_achieved = reduction;
    em->treatment_projects = projects;
    em->year = year;
    em->active = 1;
    ec.total_reduction += reduction;
    ec.n_emission++;
    print_str("[EC] Emission "); print_int(ec.n_emission - 1);
    print_str(" type="); print_int(poll_type);
    print_str(" src="); print_int(source);
    print_str(" vol="); print_int(volume);
    print_str(" red="); print_int(reduction);
    print_str(" prj="); print_int(projects); print_str("\n");
    return ec.n_emission - 1;
}

int ec_clean(int industry, int audit_lvl, int audited, int tech_promoted, int upgrades, int year) {
    if (ec.n_clean >= MAX_CLEAN) return -1;
    clean_t* c = &cleans[ec.n_clean];
    c->clean_id = ec.n_clean;
    c->industry_type = industry;
    c->audit_level = audit_lvl;
    c->enterprises_audited = audited;
    c->technologies_promoted = tech_promoted;
    c->upgrades_completed = upgrades;
    c->year = year;
    c->active = 1;
    ec.total_enterprises += audited;
    ec.n_clean++;
    print_str("[EC] Clean "); print_int(ec.n_clean - 1);
    print_str(" ind="); print_int(industry);
    print_str(" lvl="); print_int(audit_lvl);
    print_str(" aud="); print_int(audited);
    print_str(" tch="); print_int(tech_promoted);
    print_str(" upg="); print_int(upgrades); print_str("\n");
    return ec.n_clean - 1;
}

int ec_circular(int resource, int recycling_cat, int volume, int utilization, int output, int year) {
    if (ec.n_circular >= MAX_CIRCULAR) return -1;
    circular_t* cr = &circulars[ec.n_circular];
    cr->circular_id = ec.n_circular;
    cr->resource_type = resource;
    cr->recycling_category = recycling_cat;
    cr->recycling_volume = volume;
    cr->waste_utilization = utilization;
    cr->renewable_output = output;
    cr->year = year;
    cr->active = 1;
    ec.total_recycling += volume;
    ec.n_circular++;
    print_str("[EC] Circular "); print_int(ec.n_circular - 1);
    print_str(" rsc="); print_int(resource);
    print_str(" cat="); print_int(recycling_cat);
    print_str(" vol="); print_int(volume);
    print_str(" utl="); print_int(utilization);
    print_str(" out="); print_int(output); print_str("\n");
    return ec.n_circular - 1;
}

int ec_carbon(int category, int trade_type, int monitored, int allocated, int transactions, int year) {
    if (ec.n_carbon >= MAX_CARBON) return -1;
    carbon_t* cb = &carbons[ec.n_carbon];
    cb->carbon_id = ec.n_carbon;
    cb->carbon_category = category;
    cb->trading_type = trade_type;
    cb->emissions_monitored = monitored;
    cb->allowances_allocated = allocated;
    cb->transactions_completed = transactions;
    cb->year = year;
    cb->active = 1;
    ec.total_emissions += monitored;
    ec.n_carbon++;
    print_str("[EC] Carbon "); print_int(ec.n_carbon - 1);
    print_str(" cat="); print_int(category);
    print_str(" type="); print_int(trade_type);
    print_str(" mon="); print_int(monitored);
    print_str(" alc="); print_int(allocated);
    print_str(" trx="); print_int(transactions); print_str("\n");
    return ec.n_carbon - 1;
}

void ec_energy_report(void) {
    print_str("[EC] Energy report:\n");
    print_str("  Energy categories: "); print_int(ec.n_energy); print_str("\n");
    print_str("  Total energy saved: "); print_int(ec.total_saving); print_str("\n");
}

void ec_emission_report(void) {
    print_str("[EC] Emission report:\n");
    print_str("  Emission categories: "); print_int(ec.n_emission); print_str("\n");
    print_str("  Total reduction achieved: "); print_int(ec.total_reduction); print_str("\n");
}

void ec_carbon_report(void) {
    print_str("[EC] Carbon report:\n");
    print_str("  Clean production units: "); print_int(ec.n_clean); print_str("\n");
    print_str("  Total enterprises audited: "); print_int(ec.total_enterprises); print_str("\n");
    print_str("  Circular economy units: "); print_int(ec.n_circular); print_str("\n");
    print_str("  Total recycling volume: "); print_int(ec.total_recycling); print_str("\n");
    print_str("  Carbon trading units: "); print_int(ec.n_carbon); print_str("\n");
    print_str("  Total emissions monitored: "); print_int(ec.total_emissions); print_str("\n");
}

void ec_print_state(void) {
    print_str("[EC] En="); print_int(ec.n_energy);
    print_str(" Em="); print_int(ec.n_emission);
    print_str(" Cl="); print_int(ec.n_clean);
    print_str(" Cr="); print_int(ec.n_circular);
    print_str(" Cb="); print_int(ec.n_carbon);
    print_str("\n");
}

int main(void) {
    print_str("=== Energy Conservation Demo ===\n\n");
    ec_init();

    print_str("Energy saving...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int sec = (i % 6) + 1;
        int tgt = 500 + (i * 100);
        int act = 400 + (i * 80);
        int pts = 50 + (i * 12);
        int year = 2020 + (i % 5);
        ec_energy(type, sec, tgt, act, pts, year);
    }

    print_str("\nEmission reduction...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 5) + 1;
        int src = (i % 4) + 1;
        int vol = 1000 + (i * 200);
        int red = 300 + (i * 60);
        int prj = 10 + (i * 3);
        int year = 2021 + (i % 4);
        ec_emission(type, src, vol, red, prj, year);
    }

    print_str("\nClean production...\n");
    for (int i = 0; i < 12; i++) {
        int ind = (i % 5) + 1;
        int lvl = (i % 3) + 1;
        int aud = 30 + (i * 8);
        int tch = 15 + (i * 4);
        int upg = 10 + (i * 3);
        int year = 2022 + (i % 3);
        ec_clean(ind, lvl, aud, tch, upg, year);
    }

    print_str("\nCircular economy...\n");
    for (int i = 0; i < 10; i++) {
        int rsc = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int vol = 2000 + (i * 500);
        int utl = 1500 + (i * 400);
        int out = 1000 + (i * 300);
        int year = 2023 + (i % 2);
        ec_circular(rsc, cat, vol, utl, out, year);
    }

    print_str("\nCarbon trading...\n");
    for (int i = 0; i < 10; i++) {
        int cat = (i % 4) + 1;
        int type = (i % 3) + 1;
        int mon = 5000 + (i * 1200);
        int alc = 4000 + (i * 1000);
        int trx = 500 + (i * 120);
        int year = 2024;
        ec_carbon(cat, type, mon, alc, trx, year);
    }

    print_str("\nEnergy report...\n");
    ec_energy_report();

    print_str("\nEmission report...\n");
    ec_emission_report();

    print_str("\nCarbon report...\n");
    ec_carbon_report();

    print_str("\nFinal state...\n");
    ec_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
