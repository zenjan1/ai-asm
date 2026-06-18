/* forestry: Forestry management system (v1.0)
 * Forest resources, afforestation, forest protection, industry, tenure reform
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

#define MAX_RESOURCE     16
#define MAX_AFFOREST     14
#define MAX_PROTECT      12
#define MAX_INDUSTRY     10
#define MAX_TENURE       10

typedef struct {
    int    resource_id;
    int    forest_type;
    int    region_id;
    int    area_hectares;
    int    volume_m3;
    int    coverage_pct;
    int    year;
    int    active;
} resource_t;

typedef struct {
    int    afforest_id;
    int    project_type;
    int    region_id;
    int    area_planted;
    int    trees_planted;
    int    survival_rate;
    int    investment;
    int    year;
    int    active;
} afforest_t;

typedef struct {
    int    protect_id;
    int    type;
    int    forest_id;
    int    area_affected;
    int    response_team;
    int    damage;
    int    year;
    int    status;
    int    active;
} protect_t;

typedef struct {
    int    industry_id;
    int    category;
    int    company_id;
    int    output_value;
    int    employment;
    int    export_value;
    int    year;
    int    active;
} industry_t;

typedef struct {
    int    tenure_id;
    int    parcel_id;
    int    holder_id;
    int    area_hectares;
    int    transfer_value;
    int    mortgage_amount;
    int    year;
    int    active;
} tenure_t;

typedef struct {
    int    n_resource;
    int    n_afforest;
    int    n_protect;
    int    n_industry;
    int    n_tenure;
    int    total_area;
    int    total_volume;
    int    total_planted;
    int    total_output;
    int    total_mortgage;
} fo_state_t;

static resource_t resources[MAX_RESOURCE];
static afforest_t afforestations[MAX_AFFOREST];
static protect_t protections[MAX_PROTECT];
static industry_t industries[MAX_INDUSTRY];
static tenure_t tenures[MAX_TENURE];
static fo_state_t fo;

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

int fo_init(void) {
    if (initialized) return -1;
    fo.n_resource = 0; fo.n_afforest = 0; fo.n_protect = 0;
    fo.n_industry = 0; fo.n_tenure = 0;
    fo.total_area = 0; fo.total_volume = 0;
    fo.total_planted = 0; fo.total_output = 0;
    fo.total_mortgage = 0;
    for (int i = 0; i < MAX_RESOURCE; i++) resources[i].active = 0;
    for (int i = 0; i < MAX_AFFOREST; i++) afforestations[i].active = 0;
    for (int i = 0; i < MAX_PROTECT; i++) protections[i].active = 0;
    for (int i = 0; i < MAX_INDUSTRY; i++) industries[i].active = 0;
    for (int i = 0; i < MAX_TENURE; i++) tenures[i].active = 0;
    initialized = 1;
    print_str("[FO] Forestry initialized\n");
    return 0;
}

int fo_resource(int forest_type, int region, int area, int volume, int coverage, int year) {
    if (fo.n_resource >= MAX_RESOURCE) return -1;
    resource_t* r = &resources[fo.n_resource];
    r->resource_id = fo.n_resource;
    r->forest_type = forest_type;
    r->region_id = region;
    r->area_hectares = area;
    r->volume_m3 = volume;
    r->coverage_pct = coverage;
    r->year = year;
    r->active = 1;
    fo.total_area += area;
    fo.total_volume += volume;
    fo.n_resource++;
    print_str("[FO] Resource "); print_int(fo.n_resource - 1);
    print_str(" type="); print_int(forest_type);
    print_str(" reg="); print_int(region);
    print_str(" area="); print_int(area); print_str("ha");
    print_str(" vol="); print_int(volume); print_str("m3");
    print_str(" cov="); print_int(coverage); print_str("%\n");
    return fo.n_resource - 1;
}

int fo_afforest(int project, int region, int area, int trees, int survival, int investment, int year) {
    if (fo.n_afforest >= MAX_AFFOREST) return -1;
    afforest_t* a = &afforestations[fo.n_afforest];
    a->afforest_id = fo.n_afforest;
    a->project_type = project;
    a->region_id = region;
    a->area_planted = area;
    a->trees_planted = trees;
    a->survival_rate = survival;
    a->investment = investment;
    a->year = year;
    a->active = 1;
    fo.total_planted += trees;
    fo.n_afforest++;
    print_str("[FO] Afforest "); print_int(fo.n_afforest - 1);
    print_str(" prj="); print_int(project);
    print_str(" reg="); print_int(region);
    print_str(" area="); print_int(area); print_str("ha");
    print_str(" trees="); print_int(trees);
    print_str(" srv="); print_int(survival); print_str("%\n");
    return fo.n_afforest - 1;
}

int fo_protect(int type, int forest, int affected, int team, int damage, int year) {
    if (fo.n_protect >= MAX_PROTECT) return -1;
    protect_t* p = &protections[fo.n_protect];
    p->protect_id = fo.n_protect;
    p->type = type;
    p->forest_id = forest;
    p->area_affected = affected;
    p->response_team = team;
    p->damage = damage;
    p->year = year;
    p->status = 1;
    p->active = 1;
    fo.n_protect++;
    print_str("[FO] Protect "); print_int(fo.n_protect - 1);
    print_str(" type="); print_int(type);
    print_str(" frs="); print_int(forest);
    print_str(" aff="); print_int(affected); print_str("ha");
    print_str(" team="); print_int(team);
    print_str(" dmg=$"); print_int(damage); print_str("\n");
    return fo.n_protect - 1;
}

int fo_industry(int category, int company, int output, int employment, int export_val, int year) {
    if (fo.n_industry >= MAX_INDUSTRY) return -1;
    industry_t* ind = &industries[fo.n_industry];
    ind->industry_id = fo.n_industry;
    ind->category = category;
    ind->company_id = company;
    ind->output_value = output;
    ind->employment = employment;
    ind->export_value = export_val;
    ind->year = year;
    ind->active = 1;
    fo.total_output += output;
    fo.n_industry++;
    print_str("[FO] Industry "); print_int(fo.n_industry - 1);
    print_str(" cat="); print_int(category);
    print_str(" co="); print_int(company);
    print_str(" out=$"); print_int(output);
    print_str(" emp="); print_int(employment);
    print_str(" exp=$"); print_int(export_val); print_str("\n");
    return fo.n_industry - 1;
}

int fo_tenure(int parcel, int holder, int area, int transfer, int mortgage, int year) {
    if (fo.n_tenure >= MAX_TENURE) return -1;
    tenure_t* t = &tenures[fo.n_tenure];
    t->tenure_id = fo.n_tenure;
    t->parcel_id = parcel;
    t->holder_id = holder;
    t->area_hectares = area;
    t->transfer_value = transfer;
    t->mortgage_amount = mortgage;
    t->year = year;
    t->active = 1;
    fo.total_mortgage += mortgage;
    fo.n_tenure++;
    print_str("[FO] Tenure "); print_int(fo.n_tenure - 1);
    print_str(" prc="); print_int(parcel);
    print_str(" hld="); print_int(holder);
    print_str(" area="); print_int(area); print_str("ha");
    print_str(" trf=$"); print_int(transfer);
    print_str(" mrt=$"); print_int(mortgage); print_str("\n");
    return fo.n_tenure - 1;
}

void fo_resource_report(void) {
    print_str("[FO] Resource report:\n");
    print_str("  Forest areas: "); print_int(fo.n_resource); print_str("\n");
    print_str("  Total area: "); print_int(fo.total_area); print_str(" ha\n");
    print_str("  Total volume: "); print_int(fo.total_volume); print_str(" m3\n");
}

void fo_afforest_report(void) {
    print_str("[FO] Afforestation report:\n");
    print_str("  Projects: "); print_int(fo.n_afforest); print_str("\n");
    print_str("  Total trees planted: "); print_int(fo.total_planted); print_str("\n");
}

void fo_industry_report(void) {
    print_str("[FO] Industry report:\n");
    print_str("  Companies: "); print_int(fo.n_industry); print_str("\n");
    print_str("  Total output: $"); print_int(fo.total_output); print_str("\n");
    print_str("  Protection cases: "); print_int(fo.n_protect); print_str("\n");
    print_str("  Tenure records: "); print_int(fo.n_tenure); print_str("\n");
    print_str("  Total mortgage: $"); print_int(fo.total_mortgage); print_str("\n");
}

void fo_print_state(void) {
    print_str("[FO] Rs="); print_int(fo.n_resource);
    print_str(" Af="); print_int(fo.n_afforest);
    print_str(" Pt="); print_int(fo.n_protect);
    print_str(" In="); print_int(fo.n_industry);
    print_str(" Tn="); print_int(fo.n_tenure);
    print_str("\n");
}

int main(void) {
    print_str("=== Forestry Demo ===\n\n");
    fo_init();

    print_str("Forest resources...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 4) + 1;
        int reg = (i % 6) + 1;
        int area = 500 + (i * 200);
        int vol = 10000 + (i * 5000);
        int cov = 40 + (i * 3);
        int year = 2020 + (i % 5);
        fo_resource(type, reg, area, vol, cov, year);
    }

    print_str("\nAfforestation...\n");
    for (int i = 0; i < 14; i++) {
        int prj = (i % 4) + 1;
        int reg = (i % 6) + 1;
        int area = 50 + (i * 30);
        int trees = 5000 + (i * 3000);
        int srv = 75 + (i * 2);
        int inv = 100000 + (i * 50000);
        int year = 2021 + (i % 4);
        fo_afforest(prj, reg, area, trees, srv, inv, year);
    }

    print_str("\nForest protection...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 3) + 1;
        int frs = 100 + (i * 10);
        int aff = 10 + (i * 5);
        int team = 5 + (i * 2);
        int dmg = 5000 + (i * 3000);
        int year = 2022 + (i % 3);
        fo_protect(type, frs, aff, team, dmg, year);
    }

    print_str("\nForestry industry...\n");
    for (int i = 0; i < 10; i++) {
        int cat = (i % 4) + 1;
        int co = 2000 + (i * 7);
        int out = 500000 + (i * 200000);
        int emp = 50 + (i * 20);
        int exp = 100000 + (i * 50000);
        int year = 2023 + (i % 2);
        fo_industry(cat, co, out, emp, exp, year);
    }

    print_str("\nTenure reform...\n");
    for (int i = 0; i < 10; i++) {
        int prc = 300 + (i * 20);
        int hld = 4000 + (i * 13);
        int area = 10 + (i * 5);
        int trf = 20000 + (i * 10000);
        int mrt = (i % 3 == 0) ? 50000 + (i * 20000) : 0;
        int year = 2024;
        fo_tenure(prc, hld, area, trf, mrt, year);
    }

    print_str("\nResource report...\n");
    fo_resource_report();

    print_str("\nAfforestation report...\n");
    fo_afforest_report();

    print_str("\nIndustry report...\n");
    fo_industry_report();

    print_str("\nFinal state...\n");
    fo_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
