/* livestock_management: Livestock management system (v1.0)
 * Animal tracking, health monitoring, feed optimization, breeding, pasture management
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

#define MAX_ANIMALS   16
#define MAX_FEEDS     8
#define MAX_PASTURES  8
#define MAX_BREEDINGS 8

#define SPECIES_CATTLE 0x01
#define SPECIES_SHEEP  0x02
#define SPECIES_GOAT   0x03
#define SPECIES_PIG    0x04

typedef struct {
    int    id;
    int    species;
    double weight;
    double age_months;
    double body_condition;
    double temperature;
    double activity_level;
    double rumination_min;
    int    location_x;
    int    location_y;
    int    in_heat;
    int    pregnant;
    int    health_status;
    int    active;
} animal_t;

typedef struct {
    double energy;
    double protein;
    double fiber;
    double calcium;
    double phosphorus;
    double cost_per_kg;
    int    feed_type;
    int    id;
    int    active;
} feed_t;

typedef struct {
    double area_ha;
    double forage_kg;
    double utilization;
    double carrying_capacity;
    double rest_days;
    int    pasture_id;
    int    active;
} pasture_t;

typedef struct {
    int    sire_id;
    int    dam_id;
    int    breeding_date;
    int    expected_calving;
    int    success;
    int    id;
    int    active;
} breeding_t;

typedef struct {
    int    n_animals;
    int    n_feeds;
    int    n_pastures;
    int    n_breedings;
    double total_weight;
    double avg_bcs;
    double avg_temperature;
    int    health_alerts;
    int    pregnant_count;
} lm_state_t;

static animal_t animals[MAX_ANIMALS];
static feed_t feeds[MAX_FEEDS];
static pasture_t pastures[MAX_PASTURES];
static breeding_t breedings[MAX_BREEDINGS];
static lm_state_t lm;

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

int lm_init(void) {
    if (initialized) return -1;
    lm.n_animals = 0; lm.n_feeds = 0;
    lm.n_pastures = 0; lm.n_breedings = 0;
    lm.total_weight = 0.0; lm.avg_bcs = 0.0;
    lm.avg_temperature = 0.0; lm.health_alerts = 0;
    lm.pregnant_count = 0;
    for (int i = 0; i < MAX_ANIMALS; i++) animals[i].active = 0;
    for (int i = 0; i < MAX_FEEDS; i++) feeds[i].active = 0;
    for (int i = 0; i < MAX_PASTURES; i++) pastures[i].active = 0;
    for (int i = 0; i < MAX_BREEDINGS; i++) breedings[i].active = 0;
    initialized = 1;
    print_str("[LM] Livestock management initialized\n");
    return 0;
}

int lm_add_animal(int species, double weight, double age_months) {
    if (lm.n_animals >= MAX_ANIMALS) return -1;
    animal_t* a = &animals[lm.n_animals];
    a->species = species; a->weight = weight;
    a->age_months = age_months;
    a->body_condition = 3.0 + (double)(lm.n_animals % 3) * 0.5;
    a->temperature = 38.5 + (double)(lm.n_animals % 4) * 0.3;
    a->activity_level = 70.0 + (double)(lm.n_animals % 5) * 5.0;
    a->rumination_min = 400.0 + (double)(lm.n_animals % 6) * 30.0;
    a->location_x = lm.n_animals * 10;
    a->location_y = (lm.n_animals % 3) * 15;
    a->in_heat = 0; a->pregnant = 0;
    a->health_status = (a->temperature > 39.5) ? 2 : 1;
    if (a->health_status == 2) lm.health_alerts++;
    a->id = lm.n_animals; a->active = 1;
    lm.n_animals++;
    lm.total_weight += weight;
    if (a->temperature > 39.5) lm.health_alerts++;
    print_str("[LM] Animal "); print_int(lm.n_animals - 1);
    print_str(" sp="); print_int(species);
    print_str(" wt="); print_int((int)weight); print_str("kg");
    print_str(" age="); print_int((int)age_months); print_str("mo");
    print_str(" BCS="); print_int((int)(a->body_condition * 10));
    print_str(" T="); print_int((int)(a->temperature * 10)); print_str("C\n");
    return lm.n_animals - 1;
}

void lm_animal_tracking(void) {
    print_str("[LM] Animal tracking:\n");
    for (int i = 0; i < lm.n_animals; i++) {
        animal_t* a = &animals[i];
        print_str("  Animal "); print_int(i);
        print_str(" pos=("); print_int(a->location_x); print_str(","); print_int(a->location_y);
        print_str(") activity="); print_int((int)a->activity_level);
        print_str("% rumination="); print_int((int)a->rumination_min); print_str("min\n");
    }
}

void lm_health_monitoring(void) {
    double sum_temp = 0.0, sum_bcs = 0.0;
    int alerts = 0;
    for (int i = 0; i < lm.n_animals; i++) {
        animal_t* a = &animals[i];
        sum_temp += a->temperature;
        sum_bcs += a->body_condition;
        if (a->temperature > 39.5 || a->temperature < 38.0) alerts++;
        if (a->body_condition < 2.5 || a->body_condition > 4.0) alerts++;
        if (a->rumination_min < 350) alerts++;
        if (a->activity_level < 50) alerts++;
    }
    lm.avg_temperature = sum_temp / (lm.n_animals + 1e-10);
    lm.avg_bcs = sum_bcs / (lm.n_animals + 1e-10);
    lm.health_alerts = alerts;
    print_str("[LM] Health monitoring: avg_T="); print_int((int)(lm.avg_temperature * 10));
    print_str("C avg_BCS="); print_int((int)(lm.avg_bcs * 10));
    print_str(" alerts="); print_int(alerts); print_str("\n");
    for (int i = 0; i < lm.n_animals; i++) {
        if (animals[i].temperature > 39.5) {
            print_str("  FEVER: Animal "); print_int(i);
            print_str(" T="); print_int((int)(animals[i].temperature * 10)); print_str("C\n");
        }
    }
}

int lm_add_feed(double energy, double protein, double cost) {
    if (lm.n_feeds >= MAX_FEEDS) return -1;
    feed_t* f = &feeds[lm.n_feeds];
    f->energy = energy; f->protein = protein;
    f->fiber = 15.0 + (double)(lm.n_feeds % 3) * 5.0;
    f->calcium = 0.3 + (double)(lm.n_feeds % 4) * 0.1;
    f->phosphorus = 0.2 + (double)(lm.n_feeds % 3) * 0.05;
    f->cost_per_kg = cost;
    f->feed_type = lm.n_feeds + 1;
    f->id = lm.n_feeds; f->active = 1;
    lm.n_feeds++;
    print_str("[LM] Feed "); print_int(lm.n_feeds - 1);
    print_str(" ME="); print_int((int)energy);
    print_str(" CP="); print_int((int)protein);
    print_str(" cost="); print_int((int)(cost * 100)); print_str("c/kg\n");
    return lm.n_feeds - 1;
}

void lm_feed_optimization(int animal_id) {
    if (animal_id >= lm.n_animals) return;
    animal_t* a = &animals[animal_id];
    double dm_intake = a->weight * 0.025;
    double energy_req = a->weight * 0.1;
    double protein_req = a->weight * 0.012;
    if (a->pregnant) { energy_req *= 1.2; protein_req *= 1.3; }
    double best_cost = 1e10; int best_feed = 0;
    for (int i = 0; i < lm.n_feeds; i++) {
        double cost = (energy_req / (feeds[i].energy + 1e-10)) * feeds[i].cost_per_kg;
        if (cost < best_cost) { best_cost = cost; best_feed = i; }
    }
    print_str("[LM] Feed optimization animal "); print_int(animal_id);
    print_str(" DMI="); print_int((int)dm_intake); print_str("kg");
    print_str(" best_feed="); print_int(best_feed);
    print_str(" cost="); print_int((int)best_cost); print_str("USD/day\n");
}

int lm_add_pasture(double area, double forage) {
    if (lm.n_pastures >= MAX_PASTURES) return -1;
    pasture_t* p = &pastures[lm.n_pastures];
    p->area_ha = area; p->forage_kg = forage;
    p->utilization = 60.0;
    p->carrying_capacity = forage * area * 0.01;
    p->rest_days = 30;
    p->pasture_id = lm.n_pastures; p->active = 1;
    lm.n_pastures++;
    print_str("[LM] Pasture "); print_int(lm.n_pastures - 1);
    print_str(" area="); print_int((int)area); print_str("ha");
    print_str(" forage="); print_int((int)forage); print_str("kg/ha");
    print_str(" capacity="); print_int((int)p->carrying_capacity); print_str("AU\n");
    return lm.n_pastures - 1;
}

void lm_pasture_rotation(void) {
    print_str("[LM] Pasture rotation:\n");
    for (int i = 0; i < lm.n_pastures; i++) {
        pasture_t* p = &pastures[i];
        int graze_days = (int)(p->forage_kg * p->area_ha * p->utilization / 100.0 / (lm.n_animals * 10.0 + 1e-10));
        print_str("  Pasture "); print_int(i);
        print_str(" rest="); print_int(p->rest_days); print_str("d");
        print_str(" graze="); print_int(graze_days); print_str("d\n");
        p->rest_days -= graze_days;
        if (p->rest_days < 0) p->rest_days = 0;
    }
}

int lm_add_breeding(int sire_id, int dam_id) {
    if (lm.n_breedings >= MAX_BREEDINGS) return -1;
    if (sire_id >= lm.n_animals || dam_id >= lm.n_animals) return -1;
    breeding_t* b = &breedings[lm.n_breedings];
    b->sire_id = sire_id; b->dam_id = dam_id;
    b->breeding_date = 0;
    b->expected_calving = 283;
    b->success = (animals[dam_id].body_condition > 2.5) ? 1 : 0;
    if (b->success) {
        animals[dam_id].pregnant = 1;
        lm.pregnant_count++;
    }
    b->id = lm.n_breedings; b->active = 1;
    lm.n_breedings++;
    print_str("[LM] Breeding "); print_int(lm.n_breedings - 1);
    print_str(" sire="); print_int(sire_id);
    print_str(" dam="); print_int(dam_id);
    print_str(" success="); print_int(b->success); print_str("\n");
    return lm.n_breedings - 1;
}

void lm_print_state(void) {
    print_str("[LM] Animals="); print_int(lm.n_animals);
    print_str(" Feeds="); print_int(lm.n_feeds);
    print_str(" Pastures="); print_int(lm.n_pastures);
    print_str(" Breedings="); print_int(lm.n_breedings);
    print_str(" Wt="); print_int((int)lm.total_weight); print_str("kg");
    print_str(" Preg="); print_int(lm.pregnant_count);
    print_str(" Alerts="); print_int(lm.health_alerts); print_str("\n");
}

int main(void) {
    print_str("=== Livestock Management Demo ===\n\n");
    lm_init();

    print_str("Adding animals...\n");
    lm_add_animal(SPECIES_CATTLE, 550.0, 36.0);
    lm_add_animal(SPECIES_CATTLE, 480.0, 24.0);
    lm_add_animal(SPECIES_CATTLE, 620.0, 48.0);
    lm_add_animal(SPECIES_CATTLE, 510.0, 30.0);
    lm_add_animal(SPECIES_SHEEP, 65.0, 18.0);
    lm_add_animal(SPECIES_SHEEP, 58.0, 12.0);
    lm_add_animal(SPECIES_GOAT, 45.0, 15.0);
    lm_add_animal(SPECIES_CATTLE, 580.0, 42.0);

    print_str("\nAnimal tracking...\n");
    lm_animal_tracking();

    print_str("\nHealth monitoring...\n");
    lm_health_monitoring();

    print_str("\nAdding feeds...\n");
    lm_add_feed(11.0, 16.0, 0.30);
    lm_add_feed(10.5, 14.0, 0.25);
    lm_add_feed(9.0, 10.0, 0.15);
    lm_add_feed(12.0, 18.0, 0.40);

    print_str("\nFeed optimization...\n");
    for (int i = 0; i < lm.n_animals; i++) lm_feed_optimization(i);

    print_str("\nAdding pastures...\n");
    lm_add_pasture(20.0, 3000.0);
    lm_add_pasture(15.0, 2500.0);
    lm_add_pasture(25.0, 3500.0);

    print_str("\nPasture rotation...\n");
    lm_pasture_rotation();

    print_str("\nBreeding management...\n");
    lm_add_breeding(2, 1);
    lm_add_breeding(2, 3);
    lm_add_breeding(2, 7);

    lm_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
