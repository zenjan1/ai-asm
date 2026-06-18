/* housing_admin: Housing & construction administration system (v1.0)
 * Housing security, urban construction, real estate, village, building safety
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

#define MAX_HOUSING      16
#define MAX_URBAN        14
#define MAX_REALESTATE   12
#define MAX_VILLAGE      10
#define MAX_BUILDING     10

typedef struct {
    int    housing_id;
    int    project_id;
    int    housing_type;
    int    units;
    int    occupied;
    int    subsidy_amount;
    int    year;
    int    active;
} housing_t;

typedef struct {
    int    urban_id;
    int    infrastructure_id;
    int    project_type;
    int    area_sqm;
    int    investment;
    int    completion_pct;
    int    year;
    int    active;
} urban_t;

typedef struct {
    int    realestate_id;
    int    developer_id;
    int    property_type;
    int    units_sold;
    int    price_per_sqm;
    int    inventory;
    int    year;
    int    active;
} realestate_t;

typedef struct {
    int    village_id;
    int    community_id;
    int    improvement_type;
    int    households;
    int    infrastructure_score;
    int    living_score;
    int    year;
    int    active;
} village_t;

typedef struct {
    int    building_id;
    int    construction_id;
    int    building_type;
    int    floors;
    int    safety_score;
    int    inspection_pass;
    int    year;
    int    active;
} building_t;

typedef struct {
    int    n_housing;
    int    n_urban;
    int    n_realestate;
    int    n_village;
    int    n_building;
    int    total_units;
    int    total_occupied;
    int    total_investment;
    int    total_households;
    int    total_floors;
} hpa_state_t;

static housing_t housings[MAX_HOUSING];
static urban_t urbans[MAX_URBAN];
static realestate_t realestates[MAX_REALESTATE];
static village_t villages[MAX_VILLAGE];
static building_t buildings[MAX_BUILDING];
static hpa_state_t hpa;

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

int hpa_init(void) {
    if (initialized) return -1;
    hpa.n_housing = 0; hpa.n_urban = 0; hpa.n_realestate = 0;
    hpa.n_village = 0; hpa.n_building = 0;
    hpa.total_units = 0; hpa.total_occupied = 0;
    hpa.total_investment = 0; hpa.total_households = 0;
    hpa.total_floors = 0;
    for (int i = 0; i < MAX_HOUSING; i++) housings[i].active = 0;
    for (int i = 0; i < MAX_URBAN; i++) urbans[i].active = 0;
    for (int i = 0; i < MAX_REALESTATE; i++) realestates[i].active = 0;
    for (int i = 0; i < MAX_VILLAGE; i++) villages[i].active = 0;
    for (int i = 0; i < MAX_BUILDING; i++) buildings[i].active = 0;
    initialized = 1;
    print_str("[HPA] Housing admin initialized\n");
    return 0;
}

int hpa_housing(int project, int housing_type, int units, int occupied, int subsidy, int year) {
    if (hpa.n_housing >= MAX_HOUSING) return -1;
    housing_t* h = &housings[hpa.n_housing];
    h->housing_id = hpa.n_housing;
    h->project_id = project;
    h->housing_type = housing_type;
    h->units = units;
    h->occupied = occupied;
    h->subsidy_amount = subsidy;
    h->year = year;
    h->active = 1;
    hpa.total_units += units;
    hpa.total_occupied += occupied;
    hpa.n_housing++;
    print_str("[HPA] Housing "); print_int(hpa.n_housing - 1);
    print_str(" prj="); print_int(project);
    print_str(" type="); print_int(housing_type);
    print_str(" unt="); print_int(units);
    print_str(" occ="); print_int(occupied);
    print_str(" sub=$"); print_int(subsidy); print_str("\n");
    return hpa.n_housing - 1;
}

int hpa_urban(int infra_id, int project_type, int area, int investment, int completion, int year) {
    if (hpa.n_urban >= MAX_URBAN) return -1;
    urban_t* u = &urbans[hpa.n_urban];
    u->urban_id = hpa.n_urban;
    u->infrastructure_id = infra_id;
    u->project_type = project_type;
    u->area_sqm = area;
    u->investment = investment;
    u->completion_pct = completion;
    u->year = year;
    u->active = 1;
    hpa.total_investment += investment;
    hpa.n_urban++;
    print_str("[HPA] Urban "); print_int(hpa.n_urban - 1);
    print_str(" inf="); print_int(infra_id);
    print_str(" type="); print_int(project_type);
    print_str(" area="); print_int(area); print_str("sqm");
    print_str(" inv=$"); print_int(investment);
    print_str(" cmp="); print_int(completion); print_str("%\n");
    return hpa.n_urban - 1;
}

int hpa_realestate(int developer, int property_type, int sold, int price, int inventory, int year) {
    if (hpa.n_realestate >= MAX_REALESTATE) return -1;
    realestate_t* r = &realestates[hpa.n_realestate];
    r->realestate_id = hpa.n_realestate;
    r->developer_id = developer;
    r->property_type = property_type;
    r->units_sold = sold;
    r->price_per_sqm = price;
    r->inventory = inventory;
    r->year = year;
    r->active = 1;
    hpa.n_realestate++;
    print_str("[HPA] RealEstate "); print_int(hpa.n_realestate - 1);
    print_str(" dev="); print_int(developer);
    print_str(" type="); print_int(property_type);
    print_str(" sold="); print_int(sold);
    print_str(" price=$"); print_int(price);
    print_str(" invt="); print_int(inventory); print_str("\n");
    return hpa.n_realestate - 1;
}

int hpa_village(int community, int improvement_type, int households, int infra_score, int living_score, int year) {
    if (hpa.n_village >= MAX_VILLAGE) return -1;
    village_t* v = &villages[hpa.n_village];
    v->village_id = hpa.n_village;
    v->community_id = community;
    v->improvement_type = improvement_type;
    v->households = households;
    v->infrastructure_score = infra_score;
    v->living_score = living_score;
    v->year = year;
    v->active = 1;
    hpa.total_households += households;
    hpa.n_village++;
    print_str("[HPA] Village "); print_int(hpa.n_village - 1);
    print_str(" cmn="); print_int(community);
    print_str(" type="); print_int(improvement_type);
    print_str(" hh="); print_int(households);
    print_str(" inf="); print_int(infra_score);
    print_str(" liv="); print_int(living_score); print_str("\n");
    return hpa.n_village - 1;
}

int hpa_building(int construction, int building_type, int floors, int safety_score, int pass, int year) {
    if (hpa.n_building >= MAX_BUILDING) return -1;
    building_t* b = &buildings[hpa.n_building];
    b->building_id = hpa.n_building;
    b->construction_id = construction;
    b->building_type = building_type;
    b->floors = floors;
    b->safety_score = safety_score;
    b->inspection_pass = pass;
    b->year = year;
    b->active = 1;
    hpa.total_floors += floors;
    hpa.n_building++;
    print_str("[HPA] Building "); print_int(hpa.n_building - 1);
    print_str(" cst="); print_int(construction);
    print_str(" type="); print_int(building_type);
    print_str(" flr="); print_int(floors);
    print_str(" saf="); print_int(safety_score);
    print_str(" pass="); print_int(pass); print_str("\n");
    return hpa.n_building - 1;
}

void hpa_housing_report(void) {
    print_str("[HPA] Housing report:\n");
    print_str("  Projects: "); print_int(hpa.n_housing); print_str("\n");
    print_str("  Total units: "); print_int(hpa.total_units); print_str("\n");
    print_str("  Occupied: "); print_int(hpa.total_occupied); print_str("\n");
}

void hpa_urban_report(void) {
    print_str("[HPA] Urban report:\n");
    print_str("  Infrastructure projects: "); print_int(hpa.n_urban); print_str("\n");
    print_str("  Total investment: $"); print_int(hpa.total_investment); print_str("\n");
}

void hpa_building_report(void) {
    print_str("[HPA] Building report:\n");
    print_str("  Real estate developers: "); print_int(hpa.n_realestate); print_str("\n");
    print_str("  Village communities: "); print_int(hpa.n_village); print_str("\n");
    print_str("  Total households: "); print_int(hpa.total_households); print_str("\n");
    print_str("  Buildings: "); print_int(hpa.n_building); print_str("\n");
    print_str("  Total floors: "); print_int(hpa.total_floors); print_str("\n");
}

void hpa_print_state(void) {
    print_str("[HPA] Hs="); print_int(hpa.n_housing);
    print_str(" Ub="); print_int(hpa.n_urban);
    print_str(" Re="); print_int(hpa.n_realestate);
    print_str(" Vl="); print_int(hpa.n_village);
    print_str(" Bd="); print_int(hpa.n_building);
    print_str("\n");
}

int main(void) {
    print_str("=== Housing Admin Demo ===\n\n");
    hpa_init();

    print_str("Housing security...\n");
    for (int i = 0; i < 16; i++) {
        int prj = 1000 + (i * 13);
        int type = (i % 4) + 1;
        int unt = 100 + (i * 30);
        int occ = unt - (i * 5);
        int sub = 50000 + (i * 10000);
        int year = 2020 + (i % 5);
        hpa_housing(prj, type, unt, occ, sub, year);
    }

    print_str("\nUrban construction...\n");
    for (int i = 0; i < 14; i++) {
        int inf = 2000 + (i * 11);
        int type = (i % 5) + 1;
        int area = 5000 + (i * 2000);
        int inv = 1000000 + (i * 500000);
        int cmp = 50 + (i * 4);
        int year = 2021 + (i % 4);
        hpa_urban(inf, type, area, inv, cmp, year);
    }

    print_str("\nReal estate...\n");
    for (int i = 0; i < 12; i++) {
        int dev = 3000 + (i * 7);
        int type = (i % 3) + 1;
        int sold = 50 + (i * 15);
        int price = 10000 + (i * 2000);
        int invt = 100 - (i * 5);
        int year = 2022 + (i % 3);
        hpa_realestate(dev, type, sold, price, invt, year);
    }

    print_str("\nVillage construction...\n");
    for (int i = 0; i < 10; i++) {
        int cmn = 4000 + (i * 13);
        int type = (i % 4) + 1;
        int hh = 30 + (i * 10);
        int inf = 60 + (i * 3);
        int liv = 65 + (i * 3);
        int year = 2023 + (i % 2);
        hpa_village(cmn, type, hh, inf, liv, year);
    }

    print_str("\nBuilding construction...\n");
    for (int i = 0; i < 10; i++) {
        int cst = 5000 + (i * 17);
        int type = (i % 3) + 1;
        int flr = 5 + (i * 3);
        int saf = 75 + (i * 2);
        int pass = (i % 4 == 0) ? 0 : 1;
        int year = 2024;
        hpa_building(cst, type, flr, saf, pass, year);
    }

    print_str("\nHousing report...\n");
    hpa_housing_report();

    print_str("\nUrban report...\n");
    hpa_urban_report();

    print_str("\nBuilding report...\n");
    hpa_building_report();

    print_str("\nFinal state...\n");
    hpa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
