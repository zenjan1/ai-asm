/* zoo_aquarium: Zoo and aquarium management (v1.0)
 * Animals, exhibits, feeding, tickets, visitors, veterinary
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

#define MAX_ANIMALS    14
#define MAX_EXHIBITS   8
#define MAX_FEEDINGS   16
#define MAX_TICKETS    18
#define MAX_VISITORS   12
#define MAX_VET_CHECKS 10

typedef struct {
    int    animal_id;
    int    species;
    int    exhibit_id;
    int    age;
    double weight_kg;
    int    health;
    int    n_feedings;
    double food_cost;
    int    active;
} animal_t;

typedef struct {
    int    exhibit_id;
    int    type;
    int    capacity;
    int    n_animals;
    int    n_visitors;
    double maint_cost;
    double revenue;
    int    active;
} exhibit_t;

typedef struct {
    int    feeding_id;
    int    animal_id;
    int    feed_type;
    double amount_kg;
    double cost;
    int    day;
    int    active;
} feeding_t;

typedef struct {
    int    ticket_id;
    int    visitor_id;
    int    type;
    double price;
    int    guided;
    int    day;
    int    active;
} ticket_t;

typedef struct {
    int    visitor_id;
    int    age_group;
    int    n_visits;
    double total_spent;
    int    favorite_exhibit;
    int    active;
} visitor_t;

typedef struct {
    int    vet_id;
    int    animal_id;
    int    type;
    double cost;
    int    day;
    int    completed;
    int    active;
} vet_check_t;

typedef struct {
    int    n_animals;
    int    n_exhibits;
    int    n_feedings;
    int    n_tickets;
    int    n_visitors;
    int    n_vet;
    double ticket_revenue;
    double giftshop_revenue;
    double total_revenue;
    double food_costs;
    double vet_costs;
    double maint_costs;
    int    total_feedings;
} za_state_t;

static animal_t animals[MAX_ANIMALS];
static exhibit_t exhibits[MAX_EXHIBITS];
static feeding_t feedings[MAX_FEEDINGS];
static ticket_t tickets[MAX_TICKETS];
static visitor_t visitors[MAX_VISITORS];
static vet_check_t vet_checks[MAX_VET_CHECKS];
static za_state_t za;

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

int za_init(void) {
    if (initialized) return -1;
    za.n_animals = 0; za.n_exhibits = 0; za.n_feedings = 0;
    za.n_tickets = 0; za.n_visitors = 0; za.n_vet = 0;
    za.ticket_revenue = 0.0; za.giftshop_revenue = 0.0;
    za.total_revenue = 0.0; za.food_costs = 0.0;
    za.vet_costs = 0.0; za.maint_costs = 0.0;
    za.total_feedings = 0;
    for (int i = 0; i < MAX_ANIMALS; i++) animals[i].active = 0;
    for (int i = 0; i < MAX_EXHIBITS; i++) exhibits[i].active = 0;
    for (int i = 0; i < MAX_FEEDINGS; i++) feedings[i].active = 0;
    for (int i = 0; i < MAX_TICKETS; i++) tickets[i].active = 0;
    for (int i = 0; i < MAX_VISITORS; i++) visitors[i].active = 0;
    for (int i = 0; i < MAX_VET_CHECKS; i++) vet_checks[i].active = 0;
    initialized = 1;
    print_str("[ZA] Zoo initialized\n");
    return 0;
}

int za_add_exhibit(int type, int capacity, double maint) {
    if (za.n_exhibits >= MAX_EXHIBITS) return -1;
    exhibit_t* e = &exhibits[za.n_exhibits];
    e->exhibit_id = za.n_exhibits;
    e->type = type;
    e->capacity = capacity;
    e->n_animals = 0;
    e->n_visitors = 0;
    e->maint_cost = maint;
    e->revenue = 0.0;
    e->active = 1;
    za.maint_costs += maint;
    za.n_exhibits++;
    print_str("[ZA] Exhibit "); print_int(za.n_exhibits - 1);
    print_str(" type="); print_int(type);
    print_str(" cap="); print_int(capacity);
    print_str(" maint="); print_int((int)maint); print_str("\n");
    return za.n_exhibits - 1;
}

int za_add_animal(int species, int exhibit_id, int age, double weight) {
    if (za.n_animals >= MAX_ANIMALS || exhibit_id >= za.n_exhibits) return -1;
    animal_t* a = &animals[za.n_animals];
    a->animal_id = za.n_animals;
    a->species = species;
    a->exhibit_id = exhibit_id;
    a->age = age;
    a->weight_kg = weight;
    a->health = 100;
    a->n_feedings = 0;
    a->food_cost = 0.0;
    a->active = 1;
    exhibits[exhibit_id].n_animals++;
    za.n_animals++;
    print_str("[ZA] Animal "); print_int(za.n_animals - 1);
    print_str(" sp="); print_int(species);
    print_str(" Ex"); print_int(exhibit_id);
    print_str(" age="); print_int(age);
    print_str(" wt="); print_int((int)weight); print_str("kg\n");
    return za.n_animals - 1;
}

int za_register_visitor(int age_group) {
    if (za.n_visitors >= MAX_VISITORS) return -1;
    visitor_t* v = &visitors[za.n_visitors];
    v->visitor_id = za.n_visitors;
    v->age_group = age_group;
    v->n_visits = 0;
    v->total_spent = 0.0;
    v->favorite_exhibit = -1;
    v->active = 1;
    za.n_visitors++;
    print_str("[ZA] Visitor "); print_int(za.n_visitors - 1);
    print_str(" age="); print_int(age_group); print_str("\n");
    return za.n_visitors - 1;
}

int z_sell_ticket(int visitor_id, int type, int guided, int day) {
    if (za.n_tickets >= MAX_TICKETS || visitor_id >= za.n_visitors) return -1;
    double prices[4] = {25.0, 18.0, 12.0, 60.0};
    double price = prices[type < 4 ? type : 0];
    visitor_t* v = &visitors[visitor_id];
    if (v->age_group == 1) price *= 0.75;
    if (v->age_group == 3) price *= 0.50;
    if (guided) price += 10.0;
    ticket_t* t = &tickets[za.n_tickets];
    t->ticket_id = za.n_tickets;
    t->visitor_id = visitor_id;
    t->type = type;
    t->price = price;
    t->guided = guided;
    t->day = day;
    t->active = 1;
    v->n_visits++;
    v->total_spent += price;
    za.ticket_revenue += price;
    za.total_revenue += price;
    za.n_tickets++;
    print_str("[ZA] Ticket "); print_int(za.n_tickets - 1);
    print_str(" V"); print_int(visitor_id);
    print_str(" type="); print_int(type);
    print_str(" price="); print_int((int)price);
    if (guided) print_str(" [GUIDED]");
    print_str("\n");
    return za.n_tickets - 1;
}

int za_feed_animal(int animal_id, int feed_type, double amount, double cost_per_kg, int day) {
    if (za.n_feedings >= MAX_FEEDINGS || animal_id >= za.n_animals) return -1;
    feeding_t* f = &feedings[za.n_feedings];
    f->feeding_id = za.n_feedings;
    f->animal_id = animal_id;
    f->feed_type = feed_type;
    f->amount_kg = amount;
    f->cost = amount * cost_per_kg;
    f->day = day;
    f->active = 1;
    animals[animal_id].n_feedings++;
    animals[animal_id].food_cost += f->cost;
    za.food_costs += f->cost;
    za.total_feedings++;
    za.n_feedings++;
    print_str("[ZA] Feed A"); print_int(animal_id);
    print_str(" type="); print_int(feed_type);
    print_str(" amt="); print_int((int)amount); print_str("kg");
    print_str(" cost="); print_int((int)f->cost);
    print_str(" day="); print_int(day); print_str("\n");
    return za.n_feedings - 1;
}

int za_visit_exhibit(int exhibit_id, int visitor_id) {
    if (exhibit_id >= za.n_exhibits || visitor_id >= za.n_visitors) return -1;
    exhibits[exhibit_id].n_visitors++;
    visitors[visitor_id].favorite_exhibit = exhibit_id;
    print_str("[ZA] V"); print_int(visitor_id);
    print_str(" visited Ex"); print_int(exhibit_id); print_str("\n");
    return 0;
}

int za_vet_check(int animal_id, int type, double cost, int day) {
    if (za.n_vet >= MAX_VET_CHECKS || animal_id >= za.n_animals) return -1;
    vet_check_t* vc = &vet_checks[za.n_vet];
    vc->vet_id = za.n_vet;
    vc->animal_id = animal_id;
    vc->type = type;
    vc->cost = cost;
    vc->day = day;
    vc->completed = 0;
    vc->active = 1;
    za.vet_costs += cost;
    za.n_vet++;
    print_str("[ZA] Vet A"); print_int(animal_id);
    print_str(" type="); print_int(type);
    print_str(" cost="); print_int((int)cost);
    print_str(" day="); print_int(day); print_str("\n");
    return za.n_vet - 1;
}

void za_complete_vet(int vet_id) {
    if (vet_id >= za.n_vet) return;
    vet_checks[vet_id].completed = 1;
    animals[vet_checks[vet_id].animal_id].health += 5;
    print_str("[ZA] Vet "); print_int(vet_id); print_str(" completed\n");
}

int za_giftshop_sale(int visitor_id, double amount) {
    if (visitor_id >= za.n_visitors) return -1;
    visitors[visitor_id].total_spent += amount;
    za.giftshop_revenue += amount;
    za.total_revenue += amount;
    print_str("[ZA] Giftshop V"); print_int(visitor_id);
    print_str(" amount="); print_int((int)amount); print_str("\n");
    return 0;
}

void za_animal_report(void) {
    print_str("[ZA] Animal report:\n");
    for (int i = 0; i < za.n_animals; i++) {
        animal_t* a = &animals[i];
        print_str("  A"); print_int(i);
        print_str(" sp="); print_int(a->species);
        print_str(" Ex"); print_int(a->exhibit_id);
        print_str(" age="); print_int(a->age);
        print_str(" wt="); print_int((int)a->weight_kg); print_str("kg");
        print_str(" health="); print_int(a->health);
        print_str(" feeds="); print_int(a->n_feedings); print_str("\n");
    }
}

void za_financial_report(void) {
    print_str("[ZA] Financial report:\n");
    print_str("  Tickets: "); print_int((int)za.ticket_revenue); print_str("\n");
    print_str("  Giftshop: "); print_int((int)za.giftshop_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)za.total_revenue); print_str("\n");
    print_str("  Food costs: "); print_int((int)za.food_costs); print_str("\n");
    print_str("  Vet costs: "); print_int((int)za.vet_costs); print_str("\n");
    print_str("  Maintenance: "); print_int((int)za.maint_costs); print_str("\n");
    double total_costs = za.food_costs + za.vet_costs + za.maint_costs;
    double profit = za.total_revenue - total_costs;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    print_str("  Animals: "); print_int(za.n_animals); print_str("\n");
    print_str("  Feedings: "); print_int(za.total_feedings); print_str("\n");
}

void za_print_state(void) {
    print_str("[ZA] Animals="); print_int(za.n_animals);
    print_str(" Exhibits="); print_int(za.n_exhibits);
    print_str(" Feedings="); print_int(za.n_feedings);
    print_str(" Tickets="); print_int(za.n_tickets);
    print_str(" Visitors="); print_int(za.n_visitors);
    print_str(" Vet="); print_int(za.n_vet);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)za.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Zoo Aquarium Demo ===\n\n");
    za_init();

    print_str("Adding exhibits...\n");
    za_add_exhibit(1, 20, 2000.0);
    za_add_exhibit(1, 15, 1500.0);
    za_add_exhibit(2, 30, 3000.0);
    za_add_exhibit(2, 25, 2500.0);
    za_add_exhibit(3, 40, 1800.0);
    za_add_exhibit(3, 35, 1600.0);
    za_add_exhibit(4, 10, 2200.0);
    za_add_exhibit(4, 12, 1900.0);

    print_str("\nAdding animals...\n");
    za_add_animal(1, 0, 8, 200.0);
    za_add_animal(1, 0, 5, 180.0);
    za_add_animal(2, 1, 12, 150.0);
    za_add_animal(2, 1, 3, 90.0);
    za_add_animal(3, 2, 15, 500.0);
    za_add_animal(3, 2, 7, 350.0);
    za_add_animal(4, 3, 20, 80.0);
    za_add_animal(4, 3, 10, 60.0);
    za_add_animal(5, 4, 6, 300.0);
    za_add_animal(5, 4, 4, 250.0);
    za_add_animal(6, 5, 25, 120.0);
    za_add_animal(6, 5, 9, 100.0);
    za_add_animal(7, 6, 3, 15.0);
    za_add_animal(7, 7, 2, 10.0);

    print_str("\nRegistering visitors...\n");
    for (int i = 0; i < 12; i++) {
        za_register_visitor(i % 4);
    }

    print_str("\nSelling tickets...\n");
    for (int i = 0; i < 12; i++) {
        z_sell_ticket(i, i % 4, i % 3 == 0, 10 + i / 4);
    }
    z_sell_ticket(0, 0, 1, 13);
    z_sell_ticket(1, 1, 0, 13);
    z_sell_ticket(2, 2, 1, 14);
    z_sell_ticket(3, 3, 0, 14);
    z_sell_ticket(4, 0, 0, 14);
    z_sell_ticket(5, 1, 1, 15);

    print_str("\nFeeding animals...\n");
    za_feed_animal(0, 1, 20.0, 3.0, 10);
    za_feed_animal(1, 1, 15.0, 3.0, 10);
    za_feed_animal(2, 2, 10.0, 5.0, 10);
    za_feed_animal(3, 2, 8.0, 5.0, 10);
    za_feed_animal(4, 3, 30.0, 2.5, 11);
    za_feed_animal(5, 3, 25.0, 2.5, 11);
    za_feed_animal(6, 1, 12.0, 4.0, 11);
    za_feed_animal(7, 1, 10.0, 4.0, 11);
    za_feed_animal(8, 4, 20.0, 3.5, 12);
    za_feed_animal(9, 4, 18.0, 3.5, 12);
    za_feed_animal(10, 2, 15.0, 4.5, 12);
    za_feed_animal(11, 2, 12.0, 4.5, 12);
    za_feed_animal(12, 5, 3.0, 8.0, 13);
    za_feed_animal(13, 5, 2.0, 8.0, 13);
    za_feed_animal(0, 1, 22.0, 3.0, 13);
    za_feed_animal(4, 3, 28.0, 2.5, 13);

    print_str("\nVisiting exhibits...\n");
    for (int e = 0; e < 8; e++) {
        za_visit_exhibit(e, e % 12);
    }
    za_visit_exhibit(0, 5);
    za_visit_exhibit(2, 7);
    za_visit_exhibit(4, 9);
    za_visit_exhibit(6, 11);

    print_str("\nVet checks...\n");
    za_vet_check(0, 1, 300.0, 15);
    za_vet_check(2, 1, 250.0, 16);
    za_vet_check(4, 2, 500.0, 17);
    za_vet_check(6, 1, 280.0, 18);
    za_vet_check(8, 2, 450.0, 19);
    za_vet_check(10, 1, 320.0, 20);
    za_vet_check(12, 1, 200.0, 21);
    za_complete_vet(0);
    za_complete_vet(1);
    za_complete_vet(2);
    za_complete_vet(3);
    za_complete_vet(4);

    print_str("\nGiftshop sales...\n");
    za_giftshop_sale(0, 20.0);
    za_giftshop_sale(2, 35.0);
    za_giftshop_sale(4, 15.0);
    za_giftshop_sale(6, 28.0);
    za_giftshop_sale(8, 22.0);
    za_giftshop_sale(10, 18.0);

    print_str("\nAnimal report...\n");
    za_animal_report();

    print_str("\nFinancial report...\n");
    za_financial_report();

    print_str("\nFinal state...\n");
    za_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
