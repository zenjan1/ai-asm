/* agriculture_admin: Agriculture administration system (v1.0)
 * Crop management, livestock, fishery, agricultural tech, rural development
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

#define MAX_CROP         16
#define MAX_LIVESTOCK    14
#define MAX_FISHERY      12
#define MAX_AGTECH       10
#define MAX_RURAL        10

typedef struct {
    int    crop_id;
    int    farm_id;
    int    crop_type;
    int    area_hectares;
    int    yield_tons;
    int    quality_score;
    int    year;
    int    active;
} crop_t;

typedef struct {
    int    livestock_id;
    int    ranch_id;
    int    animal_type;
    int    head_count;
    int    vet_certified;
    int    disease_free;
    int    year;
    int    active;
} livestock_t;

typedef struct {
    int    fishery_id;
    int    vessel_id;
    int    fishery_type;
    int    catch_tons;
    int    species_count;
    int    sustainability;
    int    year;
    int    active;
} fishery_t;

typedef struct {
    int    agtech_id;
    int    provider_id;
    int    tech_type;
    int    adoption_rate;
    int    efficiency_gain;
    int    cost_saving;
    int    year;
    int    active;
} agtech_t;

typedef struct {
    int    rural_id;
    int    village_id;
    int    project_type;
    int    households;
    int    income_per_capita;
    int    land_transferred;
    int    year;
    int    active;
} rural_t;

typedef struct {
    int    n_crop;
    int    n_livestock;
    int    n_fishery;
    int    n_agtech;
    int    n_rural;
    int    total_yield;
    int    total_head;
    int    total_catch;
    int    total_households;
    int    total_income;
} aa_state_t;

static crop_t crops[MAX_CROP];
static livestock_t livestock_list[MAX_LIVESTOCK];
static fishery_t fisheries[MAX_FISHERY];
static agtech_t agtechs[MAX_AGTECH];
static rural_t rurals[MAX_RURAL];
static aa_state_t aa;

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

int aa_init(void) {
    if (initialized) return -1;
    aa.n_crop = 0; aa.n_livestock = 0; aa.n_fishery = 0;
    aa.n_agtech = 0; aa.n_rural = 0;
    aa.total_yield = 0; aa.total_head = 0;
    aa.total_catch = 0; aa.total_households = 0;
    aa.total_income = 0;
    for (int i = 0; i < MAX_CROP; i++) crops[i].active = 0;
    for (int i = 0; i < MAX_LIVESTOCK; i++) livestock_list[i].active = 0;
    for (int i = 0; i < MAX_FISHERY; i++) fisheries[i].active = 0;
    for (int i = 0; i < MAX_AGTECH; i++) agtechs[i].active = 0;
    for (int i = 0; i < MAX_RURAL; i++) rurals[i].active = 0;
    initialized = 1;
    print_str("[AA] Agriculture admin initialized\n");
    return 0;
}

int aa_crop(int farm, int crop_type, int area, int yield_tons, int quality, int year) {
    if (aa.n_crop >= MAX_CROP) return -1;
    crop_t* c = &crops[aa.n_crop];
    c->crop_id = aa.n_crop;
    c->farm_id = farm;
    c->crop_type = crop_type;
    c->area_hectares = area;
    c->yield_tons = yield_tons;
    c->quality_score = quality;
    c->year = year;
    c->active = 1;
    aa.total_yield += yield_tons;
    aa.n_crop++;
    print_str("[AA] Crop "); print_int(aa.n_crop - 1);
    print_str(" frm="); print_int(farm);
    print_str(" type="); print_int(crop_type);
    print_str(" area="); print_int(area); print_str("ha");
    print_str(" yld="); print_int(yield_tons); print_str("t");
    print_str(" qual="); print_int(quality); print_str("\n");
    return aa.n_crop - 1;
}

int aa_livestock(int ranch, int animal_type, int head_count, int vet_cert, int disease_free, int year) {
    if (aa.n_livestock >= MAX_LIVESTOCK) return -1;
    livestock_t* l = &livestock_list[aa.n_livestock];
    l->livestock_id = aa.n_livestock;
    l->ranch_id = ranch;
    l->animal_type = animal_type;
    l->head_count = head_count;
    l->vet_certified = vet_cert;
    l->disease_free = disease_free;
    l->year = year;
    l->active = 1;
    aa.total_head += head_count;
    aa.n_livestock++;
    print_str("[AA] Livestock "); print_int(aa.n_livestock - 1);
    print_str(" rch="); print_int(ranch);
    print_str(" type="); print_int(animal_type);
    print_str(" head="); print_int(head_count);
    print_str(" vet="); print_int(vet_cert);
    print_str(" dsfree="); print_int(disease_free); print_str("\n");
    return aa.n_livestock - 1;
}

int aa_fishery(int vessel, int fishery_type, int catch_tons, int species, int sustainability, int year) {
    if (aa.n_fishery >= MAX_FISHERY) return -1;
    fishery_t* f = &fisheries[aa.n_fishery];
    f->fishery_id = aa.n_fishery;
    f->vessel_id = vessel;
    f->fishery_type = fishery_type;
    f->catch_tons = catch_tons;
    f->species_count = species;
    f->sustainability = sustainability;
    f->year = year;
    f->active = 1;
    aa.total_catch += catch_tons;
    aa.n_fishery++;
    print_str("[AA] Fishery "); print_int(aa.n_fishery - 1);
    print_str(" vsl="); print_int(vessel);
    print_str(" type="); print_int(fishery_type);
    print_str(" ctch="); print_int(catch_tons); print_str("t");
    print_str(" spc="); print_int(species);
    print_str(" sust="); print_int(sustainability); print_str("\n");
    return aa.n_fishery - 1;
}

int aa_agtech(int provider, int tech_type, int adoption, int efficiency, int cost_saving, int year) {
    if (aa.n_agtech >= MAX_AGTECH) return -1;
    agtech_t* at = &agtechs[aa.n_agtech];
    at->agtech_id = aa.n_agtech;
    at->provider_id = provider;
    at->tech_type = tech_type;
    at->adoption_rate = adoption;
    at->efficiency_gain = efficiency;
    at->cost_saving = cost_saving;
    at->year = year;
    at->active = 1;
    aa.n_agtech++;
    print_str("[AA] AgTech "); print_int(aa.n_agtech - 1);
    print_str(" prv="); print_int(provider);
    print_str(" type="); print_int(tech_type);
    print_str(" adp="); print_int(adoption); print_str("%");
    print_str(" eff="); print_int(efficiency); print_str("%");
    print_str(" sav=$"); print_int(cost_saving); print_str("\n");
    return aa.n_agtech - 1;
}

int aa_rural(int village, int project_type, int households, int income, int land_transferred, int year) {
    if (aa.n_rural >= MAX_RURAL) return -1;
    rural_t* r = &rurals[aa.n_rural];
    r->rural_id = aa.n_rural;
    r->village_id = village;
    r->project_type = project_type;
    r->households = households;
    r->income_per_capita = income;
    r->land_transferred = land_transferred;
    r->year = year;
    r->active = 1;
    aa.total_households += households;
    aa.total_income += income * households;
    aa.n_rural++;
    print_str("[AA] Rural "); print_int(aa.n_rural - 1);
    print_str(" vlg="); print_int(village);
    print_str(" proj="); print_int(project_type);
    print_str(" hh="); print_int(households);
    print_str(" inc=$"); print_int(income);
    print_str(" lnd="); print_int(land_transferred); print_str("ha\n");
    return aa.n_rural - 1;
}

void aa_crop_report(void) {
    print_str("[AA] Crop report:\n");
    print_str("  Farms: "); print_int(aa.n_crop); print_str("\n");
    print_str("  Total yield: "); print_int(aa.total_yield); print_str(" tons\n");
}

void aa_livestock_report(void) {
    print_str("[AA] Livestock report:\n");
    print_str("  Ranches: "); print_int(aa.n_livestock); print_str("\n");
    print_str("  Total head: "); print_int(aa.total_head); print_str("\n");
}

void aa_fishery_report(void) {
    print_str("[AA] Fishery report:\n");
    print_str("  Fishing vessels: "); print_int(aa.n_fishery); print_str("\n");
    print_str("  Total catch: "); print_int(aa.total_catch); print_str(" tons\n");
    print_str("  AgTech providers: "); print_int(aa.n_agtech); print_str("\n");
    print_str("  Rural projects: "); print_int(aa.n_rural); print_str("\n");
    print_str("  Total households: "); print_int(aa.total_households); print_str("\n");
}

void aa_print_state(void) {
    print_str("[AA] Cr="); print_int(aa.n_crop);
    print_str(" Lv="); print_int(aa.n_livestock);
    print_str(" Fs="); print_int(aa.n_fishery);
    print_str(" At="); print_int(aa.n_agtech);
    print_str(" Ru="); print_int(aa.n_rural);
    print_str("\n");
}

int main(void) {
    print_str("=== Agriculture Admin Demo ===\n\n");
    aa_init();

    print_str("Crop management...\n");
    for (int i = 0; i < 16; i++) {
        int frm = 100 + (i * 10);
        int type = (i % 5) + 1;
        int area = 50 + (i * 20);
        int yld = area * 5 + (i * 10);
        int qual = 70 + (i % 25);
        int year = 2020 + (i % 5);
        aa_crop(frm, type, area, yld, qual, year);
    }

    print_str("\nLivestock management...\n");
    for (int i = 0; i < 14; i++) {
        int rch = 200 + (i * 8);
        int type = (i % 4) + 1;
        int head = 100 + (i * 50);
        int vet = (i % 3 == 0) ? 0 : 1;
        int dsfree = (i % 4 == 0) ? 0 : 1;
        int year = 2021 + (i % 4);
        aa_livestock(rch, type, head, vet, dsfree, year);
    }

    print_str("\nFishery management...\n");
    for (int i = 0; i < 12; i++) {
        int vsl = 300 + (i * 5);
        int type = (i % 3) + 1;
        int ctch = 200 + (i * 80);
        int spc = 5 + (i % 10);
        int sust = (i % 3) + 1;
        int year = 2022 + (i % 3);
        aa_fishery(vsl, type, ctch, spc, sust, year);
    }

    print_str("\nAgricultural technology...\n");
    for (int i = 0; i < 10; i++) {
        int prv = 400 + (i * 11);
        int type = (i % 4) + 1;
        int adp = 20 + (i * 7);
        int eff = 10 + (i * 5);
        int sav = 5000 + (i * 2000);
        int year = 2023 + (i % 2);
        aa_agtech(prv, type, adp, eff, sav, year);
    }

    print_str("\nRural development...\n");
    for (int i = 0; i < 10; i++) {
        int vlg = 500 + (i * 7);
        int proj = (i % 4) + 1;
        int hh = 50 + (i * 20);
        int inc = 8000 + (i * 1000);
        int lnd = 10 + (i * 5);
        int year = 2024;
        aa_rural(vlg, proj, hh, inc, lnd, year);
    }

    print_str("\nCrop report...\n");
    aa_crop_report();

    print_str("\nLivestock report...\n");
    aa_livestock_report();

    print_str("\nFishery report...\n");
    aa_fishery_report();

    print_str("\nFinal state...\n");
    aa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
