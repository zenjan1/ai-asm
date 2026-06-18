/* rural_affairs: Rural affairs administration system (v1.0)
 * Rural construction, agricultural production, farmer income, rural governance, rural reform
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

#define MAX_CONSTRUCTION 16
#define MAX_PRODUCTION   14
#define MAX_INCOME       12
#define MAX_GOVERNANCE   10
#define MAX_REFORM       10

typedef struct {
    int    construction_id;
    int    construction_type;
    int    rural_area;
    int    new_villages;
    int    infrastructure_projects;
    int    living_environment;
    int    year;
    int    active;
} construction_t;

typedef struct {
    int    production_id;
    int    production_type;
    int    farm_category;
    int    grain_output;
    int    cash_crops;
    int    livestock;
    int    year;
    int    active;
} production_t;

typedef struct {
    int    income_id;
    int    income_type;
    int    farmer_category;
    int    employment;
    int    income_growth;
    int    training_sessions;
    int    year;
    int    active;
} income_t;

typedef struct {
    int    governance_id;
    int    governance_type;
    int    village_category;
    int    village_autonomy;
    int    public_disclosure;
    int    party_building;
    int    year;
    int    active;
} governance_t;

typedef struct {
    int    reform_id;
    int    reform_type;
    int    reform_area;
    int    land_transfers;
    int    property_rights;
    int    cooperatives;
    int    year;
    int    active;
} reform_t;

typedef struct {
    int    n_construction;
    int    n_production;
    int    n_income;
    int    n_governance;
    int    n_reform;
    int    total_villages;
    int    total_grain;
    int    total_employment;
    int    total_autonomy;
    int    total_transfers;
} ra_state_t;

static construction_t constructions[MAX_CONSTRUCTION];
static production_t productions[MAX_PRODUCTION];
static income_t incomes[MAX_INCOME];
static governance_t governances[MAX_GOVERNANCE];
static reform_t reforms[MAX_REFORM];
static ra_state_t ra;

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

int ra_init(void) {
    if (initialized) return -1;
    ra.n_construction = 0; ra.n_production = 0; ra.n_income = 0;
    ra.n_governance = 0; ra.n_reform = 0;
    ra.total_villages = 0; ra.total_grain = 0;
    ra.total_employment = 0; ra.total_autonomy = 0;
    ra.total_transfers = 0;
    for (int i = 0; i < MAX_CONSTRUCTION; i++) constructions[i].active = 0;
    for (int i = 0; i < MAX_PRODUCTION; i++) productions[i].active = 0;
    for (int i = 0; i < MAX_INCOME; i++) incomes[i].active = 0;
    for (int i = 0; i < MAX_GOVERNANCE; i++) governances[i].active = 0;
    for (int i = 0; i < MAX_REFORM; i++) reforms[i].active = 0;
    initialized = 1;
    print_str("[RA] Rural affairs initialized\n");
    return 0;
}

int ra_construction(int con_type, int area, int villages, int infra, int living, int year) {
    if (ra.n_construction >= MAX_CONSTRUCTION) return -1;
    construction_t* c = &constructions[ra.n_construction];
    c->construction_id = ra.n_construction;
    c->construction_type = con_type;
    c->rural_area = area;
    c->new_villages = villages;
    c->infrastructure_projects = infra;
    c->living_environment = living;
    c->year = year;
    c->active = 1;
    ra.total_villages += villages;
    ra.n_construction++;
    print_str("[RA] Construction "); print_int(ra.n_construction - 1);
    print_str(" type="); print_int(con_type);
    print_str(" area="); print_int(area);
    print_str(" vlg="); print_int(villages);
    print_str(" inf="); print_int(infra);
    print_str(" liv="); print_int(living); print_str("\n");
    return ra.n_construction - 1;
}

int ra_production(int prod_type, int cat, int grain, int cash, int livestock, int year) {
    if (ra.n_production >= MAX_PRODUCTION) return -1;
    production_t* p = &productions[ra.n_production];
    p->production_id = ra.n_production;
    p->production_type = prod_type;
    p->farm_category = cat;
    p->grain_output = grain;
    p->cash_crops = cash;
    p->livestock = livestock;
    p->year = year;
    p->active = 1;
    ra.total_grain += grain;
    ra.n_production++;
    print_str("[RA] Production "); print_int(ra.n_production - 1);
    print_str(" type="); print_int(prod_type);
    print_str(" cat="); print_int(cat);
    print_str(" grn="); print_int(grain);
    print_str(" csh="); print_int(cash);
    print_str(" lvk="); print_int(livestock); print_str("\n");
    return ra.n_production - 1;
}

int ra_income(int inc_type, int cat, int employment, int growth, int training, int year) {
    if (ra.n_income >= MAX_INCOME) return -1;
    income_t* inc = &incomes[ra.n_income];
    inc->income_id = ra.n_income;
    inc->income_type = inc_type;
    inc->farmer_category = cat;
    inc->employment = employment;
    inc->income_growth = growth;
    inc->training_sessions = training;
    inc->year = year;
    inc->active = 1;
    ra.total_employment += employment;
    ra.n_income++;
    print_str("[RA] Income "); print_int(ra.n_income - 1);
    print_str(" type="); print_int(inc_type);
    print_str(" cat="); print_int(cat);
    print_str(" emp="); print_int(employment);
    print_str(" grw="); print_int(growth);
    print_str(" trn="); print_int(training); print_str("\n");
    return ra.n_income - 1;
}

int ra_governance(int gov_type, int cat, int autonomy, int disclosure, int party, int year) {
    if (ra.n_governance >= MAX_GOVERNANCE) return -1;
    governance_t* g = &governances[ra.n_governance];
    g->governance_id = ra.n_governance;
    g->governance_type = gov_type;
    g->village_category = cat;
    g->village_autonomy = autonomy;
    g->public_disclosure = disclosure;
    g->party_building = party;
    g->year = year;
    g->active = 1;
    ra.total_autonomy += autonomy;
    ra.n_governance++;
    print_str("[RA] Governance "); print_int(ra.n_governance - 1);
    print_str(" type="); print_int(gov_type);
    print_str(" cat="); print_int(cat);
    print_str(" aut="); print_int(autonomy);
    print_str(" dsc="); print_int(disclosure);
    print_str(" pty="); print_int(party); print_str("\n");
    return ra.n_governance - 1;
}

int ra_reform(int ref_type, int area, int transfers, int rights, int cooperatives, int year) {
    if (ra.n_reform >= MAX_REFORM) return -1;
    reform_t* r = &reforms[ra.n_reform];
    r->reform_id = ra.n_reform;
    r->reform_type = ref_type;
    r->reform_area = area;
    r->land_transfers = transfers;
    r->property_rights = rights;
    r->cooperatives = cooperatives;
    r->year = year;
    r->active = 1;
    ra.total_transfers += transfers;
    ra.n_reform++;
    print_str("[RA] Reform "); print_int(ra.n_reform - 1);
    print_str(" type="); print_int(ref_type);
    print_str(" area="); print_int(area);
    print_str(" trf="); print_int(transfers);
    print_str(" rgt="); print_int(rights);
    print_str(" cop="); print_int(cooperatives); print_str("\n");
    return ra.n_reform - 1;
}

void ra_construction_report(void) {
    print_str("[RA] Construction report:\n");
    print_str("  Construction categories: "); print_int(ra.n_construction); print_str("\n");
    print_str("  Total new villages: "); print_int(ra.total_villages); print_str("\n");
}

void ra_production_report(void) {
    print_str("[RA] Production report:\n");
    print_str("  Production categories: "); print_int(ra.n_production); print_str("\n");
    print_str("  Total grain output: "); print_int(ra.total_grain); print_str("\n");
}

void ra_reform_report(void) {
    print_str("[RA] Reform report:\n");
    print_str("  Income categories: "); print_int(ra.n_income); print_str("\n");
    print_str("  Total employment: "); print_int(ra.total_employment); print_str("\n");
    print_str("  Governance categories: "); print_int(ra.n_governance); print_str("\n");
    print_str("  Total village autonomy: "); print_int(ra.total_autonomy); print_str("\n");
    print_str("  Reform categories: "); print_int(ra.n_reform); print_str("\n");
    print_str("  Total land transfers: "); print_int(ra.total_transfers); print_str("\n");
}

void ra_print_state(void) {
    print_str("[RA] Cn="); print_int(ra.n_construction);
    print_str(" Pd="); print_int(ra.n_production);
    print_str(" In="); print_int(ra.n_income);
    print_str(" Gv="); print_int(ra.n_governance);
    print_str(" Rf="); print_int(ra.n_reform);
    print_str("\n");
}

int main(void) {
    print_str("=== Rural Affairs Demo ===\n\n");
    ra_init();

    print_str("Rural construction...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int area = (i % 6) + 1;
        int vlg = 20 + (i * 5);
        int inf = 30 + (i * 8);
        int liv = 15 + (i * 4);
        int year = 2020 + (i % 5);
        ra_construction(type, area, vlg, inf, liv, year);
    }

    print_str("\nAgricultural production...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int grn = 500 + (i * 120);
        int csh = 200 + (i * 50);
        int lvk = 100 + (i * 25);
        int year = 2021 + (i % 4);
        ra_production(type, cat, grn, csh, lvk, year);
    }

    print_str("\nFarmer income...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int emp = 80 + (i * 20);
        int grw = 50 + (i * 12);
        int trn = 20 + (i * 5);
        int year = 2022 + (i % 3);
        ra_income(type, cat, emp, grw, trn, year);
    }

    print_str("\nRural governance...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int aut = 30 + (i * 8);
        int dsc = 25 + (i * 6);
        int pty = 15 + (i * 4);
        int year = 2023 + (i % 2);
        ra_governance(type, cat, aut, dsc, pty, year);
    }

    print_str("\nRural reform...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int area = (i % 5) + 1;
        int trf = 40 + (i * 10);
        int rgt = 20 + (i * 5);
        int cop = 10 + (i * 3);
        int year = 2024;
        ra_reform(type, area, trf, rgt, cop, year);
    }

    print_str("\nConstruction report...\n");
    ra_construction_report();

    print_str("\nProduction report...\n");
    ra_production_report();

    print_str("\nReform report...\n");
    ra_reform_report();

    print_str("\nFinal state...\n");
    ra_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
