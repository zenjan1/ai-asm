/* forest_management: Forest management system (v1.0)
 * Forest health, fire warning, wildlife tracking, carbon management, forestry operations
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

#define MAX_ZONES      8
#define MAX_TREES      16
#define MAX_SPECIES    8
#define MAX_ANIMALS    8
#define MAX_FIRES      4

#define TREE_CONIFER   0x01
#define TREE_BROADLEAF 0x02
#define TREE_MIXED     0x03

#define FIRE_LOW       0x01
#define FIRE_MODERATE  0x02
#define FIRE_HIGH      0x03
#define FIRE_EXTREME   0x04

typedef struct {
    double area_ha;
    double ndvi;
    double canopy_cover;
    double biomass;
    double carbon_stock;
    int    tree_density;
    int    zone_id;
    int    active;
} zone_t;

typedef struct {
    int    species;
    double age;
    double height;
    double dbh;
    double health;
    int    zone_id;
    int    id;
    int    active;
} tree_t;

typedef struct {
    int    species_type;
    double population;
    double growth_rate;
    double health_index;
    int    zone_id;
    int    id;
    int    active;
} tree_species_t;

typedef struct {
    int    species;
    double population;
    double territory;
    double health;
    int    migration_status;
    int    zone_id;
    int    id;
    int    active;
} wildlife_t;

typedef struct {
    double risk_level;
    double temperature;
    double humidity;
    double wind_speed;
    int    hotspots;
    int    smoke_detected;
    int    zone_id;
    int    id;
    int    active;
} fire_risk_t;

typedef struct {
    int    n_zones;
    int    n_trees;
    int    n_species;
    int    n_wildlife;
    int    n_fires;
    double total_area;
    double total_carbon;
    double avg_ndvi;
    int    fire_alerts;
} fm_state_t;

static zone_t zones[MAX_ZONES];
static tree_t trees[MAX_TREES];
static tree_species_t tree_species[MAX_SPECIES];
static wildlife_t wildlife[MAX_ANIMALS];
static fire_risk_t fire_risks[MAX_FIRES];
static fm_state_t fm;

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

int fm_init(void) {
    if (initialized) return -1;
    fm.n_zones = 0; fm.n_trees = 0; fm.n_species = 0;
    fm.n_wildlife = 0; fm.n_fires = 0;
    fm.total_area = 0.0; fm.total_carbon = 0.0;
    fm.avg_ndvi = 0.0; fm.fire_alerts = 0;
    for (int i = 0; i < MAX_ZONES; i++) zones[i].active = 0;
    for (int i = 0; i < MAX_TREES; i++) trees[i].active = 0;
    for (int i = 0; i < MAX_SPECIES; i++) tree_species[i].active = 0;
    for (int i = 0; i < MAX_ANIMALS; i++) wildlife[i].active = 0;
    for (int i = 0; i < MAX_FIRES; i++) fire_risks[i].active = 0;
    initialized = 1;
    print_str("[FM] Forest management initialized\n");
    return 0;
}

int fm_add_zone(double area, double ndvi, int tree_density) {
    if (fm.n_zones >= MAX_ZONES) return -1;
    zone_t* z = &zones[fm.n_zones];
    z->area_ha = area; z->ndvi = ndvi;
    z->canopy_cover = 60.0 + ndvi * 30.0;
    z->biomass = area * 150.0;
    z->carbon_stock = z->biomass * 0.5;
    z->tree_density = tree_density;
    z->zone_id = fm.n_zones; z->active = 1;
    fm.n_zones++;
    fm.total_area += area;
    fm.total_carbon += z->carbon_stock;
    print_str("[FM] Zone "); print_int(fm.n_zones - 1);
    print_str(" area="); print_int((int)area); print_str("ha");
    print_str(" NDVI="); print_int((int)(ndvi * 100));
    print_str(" canopy="); print_int((int)z->canopy_cover);
    print_str(" carbon="); print_int((int)z->carbon_stock); print_str("t\n");
    return fm.n_zones - 1;
}

int fm_add_tree(int species, double age, double height, int zone_id) {
    if (fm.n_trees >= MAX_TREES) return -1;
    tree_t* t = &trees[fm.n_trees];
    t->species = species; t->age = age; t->height = height;
    t->dbh = height * 0.1;
    t->health = 85.0 + (double)(fm.n_trees % 4) * 3.0;
    t->zone_id = zone_id;
    t->id = fm.n_trees; t->active = 1;
    fm.n_trees++;
    print_str("[FM] Tree "); print_int(fm.n_trees - 1);
    print_str(" sp="); print_int(species);
    print_str(" age="); print_int((int)age); print_str("yr");
    print_str(" h="); print_int((int)height); print_str("m");
    print_str(" DBH="); print_int((int)(t->dbh * 10)); print_str("cm");
    print_str(" health="); print_int((int)t->health); print_str("%\n");
    return fm.n_trees - 1;
}

int fm_add_species(int type, double population) {
    if (fm.n_species >= MAX_SPECIES) return -1;
    tree_species_t* sp = &tree_species[fm.n_species];
    sp->species_type = type;
    sp->population = population;
    sp->growth_rate = 2.0 + (double)(fm.n_species % 3);
    sp->health_index = 80.0 + (double)(fm.n_species % 4) * 4.0;
    sp->zone_id = fm.n_species % fm.n_zones;
    sp->id = fm.n_species; sp->active = 1;
    fm.n_species++;
    print_str("[FM] Species "); print_int(fm.n_species - 1);
    print_str(" type="); print_int(type);
    print_str(" pop="); print_int((int)population);
    print_str(" growth="); print_int((int)sp->growth_rate);
    print_str(" health="); print_int((int)sp->health_index); print_str("%\n");
    return fm.n_species - 1;
}

void fm_forest_health_assessment(void) {
    double sum_ndvi = 0.0;
    for (int i = 0; i < fm.n_zones; i++) sum_ndvi += zones[i].ndvi;
    fm.avg_ndvi = sum_ndvi / (fm.n_zones + 1e-10);
    print_str("[FM] Forest health: avg_NDVI="); print_int((int)(fm.avg_ndvi * 100));
    print_str(" area="); print_int((int)fm.total_area); print_str("ha");
    print_str(" carbon="); print_int((int)fm.total_carbon); print_str("t\n");
    for (int i = 0; i < fm.n_zones; i++) {
        print_str("  Zone "); print_int(i);
        print_str(" NDVI="); print_int((int)(zones[i].ndvi * 100));
        print_str(" canopy="); print_int((int)zones[i].canopy_cover); print_str("%\n");
    }
}

int fm_add_wildlife(int species, double population, int zone_id) {
    if (fm.n_wildlife >= MAX_ANIMALS) return -1;
    wildlife_t* w = &wildlife[fm.n_wildlife];
    w->species = species; w->population = population;
    w->territory = population * 0.5;
    w->health = 75.0 + (double)(fm.n_wildlife % 4) * 5.0;
    w->migration_status = (fm.n_wildlife % 3 == 0) ? 1 : 0;
    w->zone_id = zone_id;
    w->id = fm.n_wildlife; w->active = 1;
    fm.n_wildlife++;
    print_str("[FM] Wildlife "); print_int(fm.n_wildlife - 1);
    print_str(" sp="); print_int(species);
    print_str(" pop="); print_int((int)population);
    print_str(" territory="); print_int((int)w->territory); print_str("ha");
    print_str(" health="); print_int((int)w->health); print_str("%\n");
    return fm.n_wildlife - 1;
}

int fm_assess_fire_risk(int zone_id, double temp, double humidity, double wind) {
    if (zone_id >= fm.n_zones) return -1;
    if (fm.n_fires >= MAX_FIRES) return -1;
    fire_risk_t* f = &fire_risks[fm.n_fires];
    f->temperature = temp; f->humidity = humidity; f->wind_speed = wind;
    double risk_score = (temp - 20.0) * 0.5 + (100.0 - humidity) * 0.3 + wind * 0.2;
    f->risk_level = risk_score > 30.0 ? FIRE_EXTREME :
                    (risk_score > 20.0 ? FIRE_HIGH :
                    (risk_score > 10.0 ? FIRE_MODERATE : FIRE_LOW));
    f->hotspots = (int)(risk_score / 10.0);
    f->smoke_detected = f->risk_level >= FIRE_HIGH ? 1 : 0;
    if (f->risk_level >= FIRE_HIGH) fm.fire_alerts++;
    f->zone_id = zone_id; f->id = fm.n_fires; f->active = 1;
    fm.n_fires++;
    print_str("[FM] Fire risk zone "); print_int(zone_id);
    print_str(" T="); print_int((int)temp); print_str("C");
    print_str(" RH="); print_int((int)humidity); print_str("%");
    print_str(" wind="); print_int((int)wind); print_str("m/s");
    print_str(" risk="); print_int(f->risk_level); print_str("\n");
    return fm.n_fires - 1;
}

void fm_carbon_sequestration(void) {
    print_str("[FM] Carbon sequestration:\n");
    print_str("  Total area: "); print_int((int)fm.total_area); print_str("ha\n");
    print_str("  Total carbon stock: "); print_int((int)fm.total_carbon); print_str("t\n");
    double annual_seq = fm.total_carbon * 0.03;
    print_str("  Annual sequestration: "); print_int((int)annual_seq); print_str("t/yr\n");
    double carbon_credit = annual_seq * 25.0;
    print_str("  Carbon credit value: "); print_int((int)carbon_credit); print_str("USD/yr\n");
}

void fm_harvest_planning(int zone_id) {
    if (zone_id >= fm.n_zones) return;
    zone_t* z = &zones[zone_id];
    double harvestable = z->biomass * 0.3;
    double timber_volume = harvestable * 0.7;
    double revenue = timber_volume * 80.0;
    print_str("[FM] Harvest plan zone "); print_int(zone_id);
    print_str(" biomass="); print_int((int)z->biomass); print_str("t");
    print_str(" harvestable="); print_int((int)harvestable); print_str("t");
    print_str(" timber="); print_int((int)timber_volume); print_str("m3");
    print_str(" revenue="); print_int((int)revenue); print_str("USD\n");
}

void fm_print_state(void) {
    print_str("[FM] Zones="); print_int(fm.n_zones);
    print_str(" Trees="); print_int(fm.n_trees);
    print_str(" Species="); print_int(fm.n_species);
    print_str(" Wildlife="); print_int(fm.n_wildlife);
    print_str(" FireRisks="); print_int(fm.n_fires);
    print_str(" Alerts="); print_int(fm.fire_alerts); print_str("\n");
}

int main(void) {
    print_str("=== Forest Management System Demo ===\n\n");
    fm_init();

    print_str("Adding zones...\n");
    fm_add_zone(100.0, 0.75, 800);
    fm_add_zone(150.0, 0.68, 600);
    fm_add_zone(80.0, 0.82, 1000);
    fm_add_zone(120.0, 0.71, 750);

    print_str("\nAdding trees...\n");
    fm_add_tree(TREE_CONIFER, 25.0, 18.0, 0);
    fm_add_tree(TREE_BROADLEAF, 15.0, 12.0, 0);
    fm_add_tree(TREE_MIXED, 30.0, 20.0, 1);
    fm_add_tree(TREE_CONIFER, 40.0, 25.0, 1);
    fm_add_tree(TREE_BROADLEAF, 10.0, 8.0, 2);
    fm_add_tree(TREE_CONIFER, 35.0, 22.0, 2);
    fm_add_tree(TREE_MIXED, 20.0, 15.0, 3);
    fm_add_tree(TREE_BROADLEAF, 18.0, 14.0, 3);

    print_str("\nTree species...\n");
    fm_add_species(1, 5000.0);
    fm_add_species(2, 3000.0);
    fm_add_species(3, 2000.0);
    fm_add_species(4, 1500.0);

    print_str("\nForest health assessment...\n");
    fm_forest_health_assessment();

    print_str("\nWildlife tracking...\n");
    fm_add_wildlife(1, 50.0, 0);
    fm_add_wildlife(2, 30.0, 1);
    fm_add_wildlife(3, 80.0, 2);
    fm_add_wildlife(4, 20.0, 3);

    print_str("\nFire risk assessment...\n");
    fm_assess_fire_risk(0, 28.0, 45.0, 15.0);
    fm_assess_fire_risk(1, 32.0, 30.0, 20.0);
    fm_assess_fire_risk(2, 25.0, 60.0, 10.0);
    fm_assess_fire_risk(3, 35.0, 25.0, 25.0);

    print_str("\nCarbon sequestration...\n");
    fm_carbon_sequestration();

    print_str("\nHarvest planning...\n");
    fm_harvest_planning(0);
    fm_harvest_planning(1);
    fm_harvest_planning(2);

    fm_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
