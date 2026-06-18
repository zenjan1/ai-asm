/* farm_ranch: Farm and ranch management (v1.0)
 * Crops, livestock, land, equipment, workers, revenue
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

#define MAX_CROPS      10
#define MAX_LIVESTOCK  12
#define MAX_LAND       8
#define MAX_EQUIPMENT  8
#define MAX_WORKERS    10
#define MAX_TASKS      16

typedef struct {
    int    crop_id;
    int    type;
    int    land_id;
    double area_acres;
    double yield_per_acre;
    double price_per_unit;
    double seed_cost;
    double fertilizer_cost;
    int    planted_day;
    int    harvested;
    double revenue;
    int    active;
} crop_t;

typedef struct {
    int    animal_id;
    int    type;
    int    age_months;
    double weight_kg;
    int    health;
    double feed_cost;
    double product_value;
    int    n_products;
    int    active;
} livestock_t;

typedef struct {
    int    land_id;
    int    soil_type;
    double area_acres;
    int    irrigation;
    int    crop_id;
    double value;
    int    active;
} land_t;

typedef struct {
    int    equip_id;
    int    type;
    int    status;
    int    n_uses;
    double fuel_cost;
    double maint_cost;
    int    active;
} equipment_t;

typedef struct {
    int    worker_id;
    int    skill;
    int    n_tasks;
    int    total_hours;
    double hourly_wage;
    double total_paid;
    int    active;
} worker_t;

typedef struct {
    int    task_id;
    int    worker_id;
    int    type;
    int    target_id;
    int    hours;
    int    day;
    int    active;
} task_t;

typedef struct {
    int    n_crops;
    int    n_livestock;
    int    n_land;
    int    n_equipment;
    int    n_workers;
    int    n_tasks;
    double crop_revenue;
    double livestock_revenue;
    double total_revenue;
    double seed_costs;
    double fertilizer_costs;
    double feed_costs;
    double fuel_costs;
    double wage_costs;
    double maint_costs;
} fr_state_t;

static crop_t crops[MAX_CROPS];
static livestock_t livestock[MAX_LIVESTOCK];
static land_t land[MAX_LAND];
static equipment_t equipment[MAX_EQUIPMENT];
static worker_t workers[MAX_WORKERS];
static task_t tasks[MAX_TASKS];
static fr_state_t fr;

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

int fr_init(void) {
    if (initialized) return -1;
    fr.n_crops = 0; fr.n_livestock = 0; fr.n_land = 0;
    fr.n_equipment = 0; fr.n_workers = 0; fr.n_tasks = 0;
    fr.crop_revenue = 0.0; fr.livestock_revenue = 0.0;
    fr.total_revenue = 0.0; fr.seed_costs = 0.0;
    fr.fertilizer_costs = 0.0; fr.feed_costs = 0.0;
    fr.fuel_costs = 0.0; fr.wage_costs = 0.0;
    fr.maint_costs = 0.0;
    for (int i = 0; i < MAX_CROPS; i++) crops[i].active = 0;
    for (int i = 0; i < MAX_LIVESTOCK; i++) livestock[i].active = 0;
    for (int i = 0; i < MAX_LAND; i++) land[i].active = 0;
    for (int i = 0; i < MAX_EQUIPMENT; i++) equipment[i].active = 0;
    for (int i = 0; i < MAX_WORKERS; i++) workers[i].active = 0;
    for (int i = 0; i < MAX_TASKS; i++) tasks[i].active = 0;
    initialized = 1;
    print_str("[FR] Farm initialized\n");
    return 0;
}

int fr_add_land(int soil_type, double area, int irrigation) {
    if (fr.n_land >= MAX_LAND) return -1;
    land_t* l = &land[fr.n_land];
    l->land_id = fr.n_land;
    l->soil_type = soil_type;
    l->area_acres = area;
    l->irrigation = irrigation;
    l->crop_id = -1;
    l->value = area * 5000.0;
    l->active = 1;
    fr.n_land++;
    print_str("[FR] Land "); print_int(fr.n_land - 1);
    print_str(" soil="); print_int(soil_type);
    print_str(" area="); print_int((int)area); print_str("ac");
    if (irrigation) print_str(" [IRR]");
    print_str("\n");
    return fr.n_land - 1;
}

int fr_plant_crop(int type, int land_id, double area, double yield_per, double price, double seed, double fert) {
    if (fr.n_crops >= MAX_CROPS || land_id >= fr.n_land) return -1;
    crop_t* c = &crops[fr.n_crops];
    c->crop_id = fr.n_crops;
    c->type = type;
    c->land_id = land_id;
    c->area_acres = area;
    c->yield_per_acre = yield_per;
    c->price_per_unit = price;
    c->seed_cost = seed * area;
    c->fertilizer_cost = fert * area;
    c->planted_day = 10;
    c->harvested = 0;
    c->revenue = 0.0;
    c->active = 1;
    land[land_id].crop_id = fr.n_crops;
    fr.seed_costs += c->seed_cost;
    fr.fertilizer_costs += c->fertilizer_cost;
    fr.n_crops++;
    print_str("[FR] Crop "); print_int(fr.n_crops - 1);
    print_str(" type="); print_int(type);
    print_str(" L"); print_int(land_id);
    print_str(" area="); print_int((int)area); print_str("ac");
    print_str(" yield="); print_int((int)yield_per); print_str("/ac\n");
    return fr.n_crops - 1;
}

int fr_add_livestock(int type, int age, double weight, double feed, double prod_val) {
    if (fr.n_livestock >= MAX_LIVESTOCK) return -1;
    livestock_t* a = &livestock[fr.n_livestock];
    a->animal_id = fr.n_livestock;
    a->type = type;
    a->age_months = age;
    a->weight_kg = weight;
    a->health = 100;
    a->feed_cost = feed;
    a->product_value = prod_val;
    a->n_products = 0;
    a->active = 1;
    fr.feed_costs += feed;
    fr.n_livestock++;
    print_str("[FR] Livestock "); print_int(fr.n_livestock - 1);
    print_str(" type="); print_int(type);
    print_str(" age="); print_int(age); print_str("mo");
    print_str(" wt="); print_int((int)weight); print_str("kg\n");
    return fr.n_livestock - 1;
}

int fr_add_equipment(int type, double fuel, double maint) {
    if (fr.n_equipment >= MAX_EQUIPMENT) return -1;
    equipment_t* e = &equipment[fr.n_equipment];
    e->equip_id = fr.n_equipment;
    e->type = type;
    e->status = 1;
    e->n_uses = 0;
    e->fuel_cost = fuel;
    e->maint_cost = maint;
    e->active = 1;
    fr.n_equipment++;
    print_str("[FR] Equipment "); print_int(fr.n_equipment - 1);
    print_str(" type="); print_int(type); print_str("\n");
    return fr.n_equipment - 1;
}

int fr_add_worker(int skill, double wage) {
    if (fr.n_workers >= MAX_WORKERS) return -1;
    worker_t* w = &workers[fr.n_workers];
    w->worker_id = fr.n_workers;
    w->skill = skill;
    w->n_tasks = 0;
    w->total_hours = 0;
    w->hourly_wage = wage;
    w->total_paid = 0.0;
    w->active = 1;
    fr.n_workers++;
    print_str("[FR] Worker "); print_int(fr.n_workers - 1);
    print_str(" skill="); print_int(skill);
    print_str(" wage="); print_int((int)wage); print_str("/hr\n");
    return fr.n_workers - 1;
}

int fr_harvest_crop(int crop_id) {
    if (crop_id >= fr.n_crops) return -1;
    crop_t* c = &crops[crop_id];
    if (c->harvested) return -2;
    double total_yield = c->area_acres * c->yield_per_acre;
    c->revenue = total_yield * c->price_per_unit;
    c->harvested = 1;
    fr.crop_revenue += c->revenue;
    fr.total_revenue += c->revenue;
    print_str("[FR] Harvested C"); print_int(crop_id);
    print_str(" yield="); print_int((int)total_yield);
    print_str(" rev="); print_int((int)c->revenue); print_str("\n");
    return 0;
}

int fr_produce(int animal_id) {
    if (animal_id >= fr.n_livestock) return -1;
    livestock_t* a = &livestock[animal_id];
    a->n_products++;
    double rev = a->product_value;
    fr.livestock_revenue += rev;
    fr.total_revenue += rev;
    print_str("[FR] A"); print_int(animal_id);
    print_str(" produced val="); print_int((int)rev); print_str("\n");
    return 0;
}

int fr_assign_task(int worker_id, int type, int target_id, int hours, int day) {
    if (fr.n_tasks >= MAX_TASKS || worker_id >= fr.n_workers) return -1;
    task_t* t = &tasks[fr.n_tasks];
    t->task_id = fr.n_tasks;
    t->worker_id = worker_id;
    t->type = type;
    t->target_id = target_id;
    t->hours = hours;
    t->day = day;
    t->active = 1;
    double pay = workers[worker_id].hourly_wage * hours;
    workers[worker_id].n_tasks++;
    workers[worker_id].total_hours += hours;
    workers[worker_id].total_paid += pay;
    fr.wage_costs += pay;
    fr.n_tasks++;
    print_str("[FR] Task "); print_int(fr.n_tasks - 1);
    print_str(" W"); print_int(worker_id);
    print_str(" type="); print_int(type);
    print_str(" hrs="); print_int(hours);
    print_str(" pay="); print_int((int)pay); print_str("\n");
    return fr.n_tasks - 1;
}

int fr_use_equipment(int equip_id, int hours) {
    if (equip_id >= fr.n_equipment) return -1;
    equipment_t* e = &equipment[equip_id];
    e->n_uses++;
    double fuel = e->fuel_cost * hours;
    fr.fuel_costs += fuel;
    print_str("[FR] E"); print_int(equip_id);
    print_str(" used hrs="); print_int(hours);
    print_str(" fuel="); print_int((int)fuel); print_str("\n");
    return 0;
}

int fr_maintain_equipment(int equip_id) {
    if (equip_id >= fr.n_equipment) return -1;
    equipment_t* e = &equipment[equip_id];
    fr.maint_costs += e->maint_cost;
    print_str("[FR] E"); print_int(equip_id);
    print_str(" maintained cost="); print_int((int)e->maint_cost); print_str("\n");
    return 0;
}

void fr_crop_report(void) {
    print_str("[FR] Crop report:\n");
    for (int i = 0; i < fr.n_crops; i++) {
        crop_t* c = &crops[i];
        print_str("  C"); print_int(i);
        print_str(" type="); print_int(c->type);
        print_str(" L"); print_int(c->land_id);
        print_str(" area="); print_int((int)c->area_acres); print_str("ac");
        print_str(" rev="); print_int((int)c->revenue);
        if (c->harvested) print_str(" [HARVESTED]");
        print_str("\n");
    }
}

void fr_financial_report(void) {
    print_str("[FR] Financial report:\n");
    print_str("  Crop revenue: "); print_int((int)fr.crop_revenue); print_str("\n");
    print_str("  Livestock revenue: "); print_int((int)fr.livestock_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)fr.total_revenue); print_str("\n");
    double total_costs = fr.seed_costs + fr.fertilizer_costs + fr.feed_costs +
                         fr.fuel_costs + fr.wage_costs + fr.maint_costs;
    print_str("  Seed: "); print_int((int)fr.seed_costs); print_str("\n");
    print_str("  Fertilizer: "); print_int((int)fr.fertilizer_costs); print_str("\n");
    print_str("  Feed: "); print_int((int)fr.feed_costs); print_str("\n");
    print_str("  Fuel: "); print_int((int)fr.fuel_costs); print_str("\n");
    print_str("  Wages: "); print_int((int)fr.wage_costs); print_str("\n");
    print_str("  Maintenance: "); print_int((int)fr.maint_costs); print_str("\n");
    print_str("  Total costs: "); print_int((int)total_costs); print_str("\n");
    double profit = fr.total_revenue - total_costs;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
}

void fr_print_state(void) {
    print_str("[FR] Crops="); print_int(fr.n_crops);
    print_str(" Animals="); print_int(fr.n_livestock);
    print_str(" Land="); print_int(fr.n_land);
    print_str(" Equip="); print_int(fr.n_equipment);
    print_str(" Workers="); print_int(fr.n_workers);
    print_str(" Tasks="); print_int(fr.n_tasks);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)fr.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Farm Ranch Demo ===\n\n");
    fr_init();

    print_str("Adding land...\n");
    fr_add_land(1, 50.0, 1);
    fr_add_land(1, 40.0, 1);
    fr_add_land(2, 60.0, 0);
    fr_add_land(2, 35.0, 1);
    fr_add_land(3, 45.0, 0);
    fr_add_land(1, 30.0, 1);
    fr_add_land(3, 55.0, 0);
    fr_add_land(2, 25.0, 1);

    print_str("\nPlanting crops...\n");
    fr_plant_crop(1, 0, 20.0, 150.0, 5.0, 30.0, 15.0);
    fr_plant_crop(1, 1, 18.0, 140.0, 5.0, 28.0, 14.0);
    fr_plant_crop(2, 2, 25.0, 200.0, 3.0, 20.0, 10.0);
    fr_plant_crop(2, 3, 15.0, 180.0, 3.0, 18.0, 9.0);
    fr_plant_crop(3, 4, 10.0, 500.0, 8.0, 50.0, 25.0);
    fr_plant_crop(3, 5, 12.0, 450.0, 8.0, 45.0, 22.0);
    fr_plant_crop(4, 6, 30.0, 100.0, 12.0, 40.0, 20.0);
    fr_plant_crop(4, 7, 20.0, 120.0, 12.0, 35.0, 18.0);
    fr_plant_crop(1, 0, 15.0, 160.0, 5.5, 32.0, 16.0);
    fr_plant_crop(2, 2, 22.0, 190.0, 3.5, 22.0, 11.0);

    print_str("\nAdding livestock...\n");
    fr_add_livestock(1, 24, 500.0, 8.0, 5.0);
    fr_add_livestock(1, 18, 450.0, 7.0, 5.0);
    fr_add_livestock(1, 30, 550.0, 9.0, 5.0);
    fr_add_livestock(2, 12, 80.0, 3.0, 2.0);
    fr_add_livestock(2, 10, 70.0, 2.5, 2.0);
    fr_add_livestock(2, 14, 85.0, 3.5, 2.0);
    fr_add_livestock(3, 8, 120.0, 5.0, 0.0);
    fr_add_livestock(3, 6, 100.0, 4.0, 0.0);
    fr_add_livestock(4, 3, 2.5, 0.5, 0.3);
    fr_add_livestock(4, 4, 3.0, 0.6, 0.3);
    fr_add_livestock(4, 2, 2.0, 0.4, 0.3);
    fr_add_livestock(5, 36, 500.0, 10.0, 0.0);

    print_str("\nAdding equipment...\n");
    fr_add_equipment(1, 15.0, 500.0);
    fr_add_equipment(1, 12.0, 400.0);
    fr_add_equipment(2, 20.0, 800.0);
    fr_add_equipment(2, 18.0, 700.0);
    fr_add_equipment(3, 8.0, 300.0);
    fr_add_equipment(3, 10.0, 350.0);
    fr_add_equipment(4, 5.0, 200.0);
    fr_add_equipment(4, 6.0, 250.0);

    print_str("\nAdding workers...\n");
    fr_add_worker(1, 18.0);
    fr_add_worker(1, 16.0);
    fr_add_worker(2, 20.0);
    fr_add_worker(2, 19.0);
    fr_add_worker(3, 15.0);
    fr_add_worker(3, 14.0);
    fr_add_worker(1, 17.0);
    fr_add_worker(2, 21.0);
    fr_add_worker(3, 16.0);
    fr_add_worker(1, 18.0);

    print_str("\nAssigning tasks...\n");
    fr_assign_task(0, 1, 0, 8, 10);
    fr_assign_task(1, 1, 1, 8, 10);
    fr_assign_task(2, 2, 2, 8, 10);
    fr_assign_task(3, 2, 3, 8, 11);
    fr_assign_task(4, 3, 0, 6, 11);
    fr_assign_task(5, 3, 1, 6, 11);
    fr_assign_task(6, 1, 4, 8, 12);
    fr_assign_task(7, 2, 5, 8, 12);
    fr_assign_task(8, 3, 6, 6, 12);
    fr_assign_task(9, 1, 7, 8, 13);
    fr_assign_task(0, 2, 8, 6, 13);
    fr_assign_task(1, 3, 9, 6, 13);
    fr_assign_task(2, 1, 0, 8, 14);
    fr_assign_task(3, 2, 2, 8, 14);
    fr_assign_task(4, 3, 4, 6, 14);
    fr_assign_task(5, 1, 6, 8, 15);

    print_str("\nHarvesting crops...\n");
    for (int i = 0; i < 10; i++) {
        fr_harvest_crop(i);
    }

    print_str("\nLivestock production...\n");
    for (int p = 0; p < 3; p++) {
        for (int i = 0; i < 12; i++) {
            if (livestock[i].product_value > 0)
                fr_produce(i);
        }
    }

    print_str("\nEquipment usage...\n");
    fr_use_equipment(0, 6);
    fr_use_equipment(1, 5);
    fr_use_equipment(2, 8);
    fr_use_equipment(3, 7);
    fr_use_equipment(4, 4);
    fr_use_equipment(5, 5);
    fr_use_equipment(6, 3);
    fr_use_equipment(7, 4);

    print_str("\nEquipment maintenance...\n");
    for (int i = 0; i < 8; i++) {
        fr_maintain_equipment(i);
    }

    print_str("\nCrop report...\n");
    fr_crop_report();

    print_str("\nFinancial report...\n");
    fr_financial_report();

    print_str("\nFinal state...\n");
    fr_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
