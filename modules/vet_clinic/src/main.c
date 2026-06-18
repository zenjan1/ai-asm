/* vet_clinic: Veterinary clinic management (v1.0)
 * Pets, owners, treatments, medications, surgeries, revenue
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

#define MAX_PETS       14
#define MAX_OWNERS     10
#define MAX_TREATMENTS 16
#define MAX_MEDICATIONS 12
#define MAX_SURGERIES  10
#define MAX_BOARDING   8

typedef struct {
    int    pet_id;
    int    owner_id;
    int    species;
    int    breed;
    int    age_months;
    double weight_kg;
    int    vaccinated;
    double total_spent;
    int    n_visits;
    int    active;
} pet_t;

typedef struct {
    int    owner_id;
    int    n_pets;
    double total_spent;
    int    membership;
    int    active;
} owner_t;

typedef struct {
    int    treatment_id;
    int    pet_id;
    int    type;
    double fee;
    double cost;
    int    day;
    int    active;
} treatment_t;

typedef struct {
    int    med_id;
    int    type;
    int    stock;
    double unit_cost;
    double unit_price;
    int    n_sold;
    double revenue;
    int    active;
} medication_t;

typedef struct {
    int    surgery_id;
    int    pet_id;
    int    type;
    double fee;
    double cost;
    int    duration_min;
    int    day;
    int    active;
} surgery_t;

typedef struct {
    int    board_id;
    int    pet_id;
    int    days;
    double daily_rate;
    double total;
    int    active;
} boarding_t;

typedef struct {
    int    n_pets;
    int    n_owners;
    int    n_treatments;
    int    n_medications;
    int    n_surgeries;
    int    n_boarding;
    double treatment_revenue;
    double surgery_revenue;
    double medication_revenue;
    double boarding_revenue;
    double total_revenue;
    double total_costs;
} vet_state_t;

static pet_t pets[MAX_PETS];
static owner_t owners[MAX_OWNERS];
static treatment_t treatments[MAX_TREATMENTS];
static medication_t medications[MAX_MEDICATIONS];
static surgery_t surgeries[MAX_SURGERIES];
static boarding_t boarding[MAX_BOARDING];
static vet_state_t vet;

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

int vet_init(void) {
    if (initialized) return -1;
    vet.n_pets = 0; vet.n_owners = 0; vet.n_treatments = 0;
    vet.n_medications = 0; vet.n_surgeries = 0; vet.n_boarding = 0;
    vet.treatment_revenue = 0.0; vet.surgery_revenue = 0.0;
    vet.medication_revenue = 0.0; vet.boarding_revenue = 0.0;
    vet.total_revenue = 0.0; vet.total_costs = 0.0;
    for (int i = 0; i < MAX_PETS; i++) pets[i].active = 0;
    for (int i = 0; i < MAX_OWNERS; i++) owners[i].active = 0;
    for (int i = 0; i < MAX_TREATMENTS; i++) treatments[i].active = 0;
    for (int i = 0; i < MAX_MEDICATIONS; i++) medications[i].active = 0;
    for (int i = 0; i < MAX_SURGERIES; i++) surgeries[i].active = 0;
    for (int i = 0; i < MAX_BOARDING; i++) boarding[i].active = 0;
    initialized = 1;
    print_str("[VET] Clinic initialized\n");
    return 0;
}

int vet_register_owner(int membership) {
    if (vet.n_owners >= MAX_OWNERS) return -1;
    owner_t* o = &owners[vet.n_owners];
    o->owner_id = vet.n_owners;
    o->n_pets = 0;
    o->total_spent = 0.0;
    o->membership = membership;
    o->active = 1;
    vet.n_owners++;
    print_str("[VET] Owner "); print_int(vet.n_owners - 1);
    print_str(" member="); print_int(membership); print_str("\n");
    return vet.n_owners - 1;
}

int vet_register_pet(int owner_id, int species, int breed, int age_months, double weight) {
    if (vet.n_pets >= MAX_PETS || owner_id >= vet.n_owners) return -1;
    pet_t* p = &pets[vet.n_pets];
    p->pet_id = vet.n_pets;
    p->owner_id = owner_id;
    p->species = species;
    p->breed = breed;
    p->age_months = age_months;
    p->weight_kg = weight;
    p->vaccinated = 0;
    p->total_spent = 0.0;
    p->n_visits = 0;
    p->active = 1;
    owners[owner_id].n_pets++;
    vet.n_pets++;
    print_str("[VET] Pet "); print_int(vet.n_pets - 1);
    print_str(" Ow"); print_int(owner_id);
    print_str(" sp="); print_int(species);
    print_str(" br="); print_int(breed);
    print_str(" wt="); print_int((int)weight); print_str("kg\n");
    return vet.n_pets - 1;
}

int vet_add_medication(int type, int stock, double cost, double price) {
    if (vet.n_medications >= MAX_MEDICATIONS) return -1;
    medication_t* m = &medications[vet.n_medications];
    m->med_id = vet.n_medications;
    m->type = type;
    m->stock = stock;
    m->unit_cost = cost;
    m->unit_price = price;
    m->n_sold = 0;
    m->revenue = 0.0;
    m->active = 1;
    vet.n_medications++;
    print_str("[VET] Med "); print_int(vet.n_medications - 1);
    print_str(" type="); print_int(type);
    print_str(" stock="); print_int(stock);
    print_str(" price="); print_int((int)price); print_str("\n");
    return vet.n_medications - 1;
}

int vet_treatment(int pet_id, int type, double fee, double cost, int day) {
    if (vet.n_treatments >= MAX_TREATMENTS || pet_id >= vet.n_pets) return -1;
    treatment_t* t = &treatments[vet.n_treatments];
    t->treatment_id = vet.n_treatments;
    t->pet_id = pet_id;
    t->type = type;
    t->fee = fee;
    t->cost = cost;
    t->day = day;
    t->active = 1;
    pets[pet_id].total_spent += fee;
    pets[pet_id].n_visits++;
    owners[pets[pet_id].owner_id].total_spent += fee;
    vet.treatment_revenue += fee;
    vet.total_revenue += fee;
    vet.total_costs += cost;
    vet.n_treatments++;
    print_str("[VET] Treat "); print_int(vet.n_treatments - 1);
    print_str(" P"); print_int(pet_id);
    print_str(" type="); print_int(type);
    print_str(" fee="); print_int((int)fee); print_str("\n");
    return vet.n_treatments - 1;
}

int vet_vaccinate(int pet_id, int med_id, double fee) {
    if (pet_id >= vet.n_pets || med_id >= vet.n_medications) return -1;
    medication_t* m = &medications[med_id];
    if (m->stock <= 0) return -2;
    m->stock--;
    m->n_sold++;
    m->revenue += m->unit_price;
    pets[pet_id].vaccinated = 1;
    pets[pet_id].total_spent += fee;
    pets[pet_id].n_visits++;
    owners[pets[pet_id].owner_id].total_spent += fee;
    vet.medication_revenue += m->unit_price;
    vet.treatment_revenue += fee;
    vet.total_revenue += fee + m->unit_price;
    vet.total_costs += m->unit_cost;
    print_str("[VET] Vaccinate P"); print_int(pet_id);
    print_str(" M"); print_int(med_id);
    print_str(" fee="); print_int((int)fee); print_str("\n");
    return 0;
}

int vet_surgery(int pet_id, int type, double fee, double cost, int duration, int day) {
    if (vet.n_surgeries >= MAX_SURGERIES || pet_id >= vet.n_pets) return -1;
    surgery_t* s = &surgeries[vet.n_surgeries];
    s->surgery_id = vet.n_surgeries;
    s->pet_id = pet_id;
    s->type = type;
    s->fee = fee;
    s->cost = cost;
    s->duration_min = duration;
    s->day = day;
    s->active = 1;
    pets[pet_id].total_spent += fee;
    pets[pet_id].n_visits++;
    owners[pets[pet_id].owner_id].total_spent += fee;
    vet.surgery_revenue += fee;
    vet.total_revenue += fee;
    vet.total_costs += cost;
    vet.n_surgeries++;
    print_str("[VET] Surgery "); print_int(vet.n_surgeries - 1);
    print_str(" P"); print_int(pet_id);
    print_str(" type="); print_int(type);
    print_str(" dur="); print_int(duration); print_str("min");
    print_str(" fee="); print_int((int)fee); print_str("\n");
    return vet.n_surgeries - 1;
}

int vet_board(int pet_id, int days, double daily_rate) {
    if (vet.n_boarding >= MAX_BOARDING || pet_id >= vet.n_pets) return -1;
    boarding_t* b = &boarding[vet.n_boarding];
    b->board_id = vet.n_boarding;
    b->pet_id = pet_id;
    b->days = days;
    b->daily_rate = daily_rate;
    b->total = days * daily_rate;
    b->active = 1;
    pets[pet_id].total_spent += b->total;
    owners[pets[pet_id].owner_id].total_spent += b->total;
    vet.boarding_revenue += b->total;
    vet.total_revenue += b->total;
    vet.n_boarding++;
    print_str("[VET] Board P"); print_int(pet_id);
    print_str(" days="); print_int(days);
    print_str(" total="); print_int((int)b->total); print_str("\n");
    return vet.n_boarding - 1;
}

void vet_pet_report(void) {
    print_str("[VET] Pet report:\n");
    for (int i = 0; i < vet.n_pets; i++) {
        pet_t* p = &pets[i];
        print_str("  P"); print_int(i);
        print_str(" Ow"); print_int(p->owner_id);
        print_str(" sp="); print_int(p->species);
        print_str(" wt="); print_int((int)p->weight_kg); print_str("kg");
        print_str(" vacc="); print_int(p->vaccinated);
        print_str(" visits="); print_int(p->n_visits);
        print_str(" spent="); print_int((int)p->total_spent); print_str("\n");
    }
}

void vet_financial_report(void) {
    print_str("[VET] Financial report:\n");
    print_str("  Treatment revenue: "); print_int((int)vet.treatment_revenue); print_str("\n");
    print_str("  Surgery revenue: "); print_int((int)vet.surgery_revenue); print_str("\n");
    print_str("  Medication revenue: "); print_int((int)vet.medication_revenue); print_str("\n");
    print_str("  Boarding revenue: "); print_int((int)vet.boarding_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)vet.total_revenue); print_str("\n");
    print_str("  Costs: "); print_int((int)vet.total_costs); print_str("\n");
    double profit = vet.total_revenue - vet.total_costs;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    print_str("  Pets: "); print_int(vet.n_pets);
    print_str(" Owners: "); print_int(vet.n_owners); print_str("\n");
}

void vet_print_state(void) {
    print_str("[VET] Pets="); print_int(vet.n_pets);
    print_str(" Owners="); print_int(vet.n_owners);
    print_str(" Treat="); print_int(vet.n_treatments);
    print_str(" Surg="); print_int(vet.n_surgeries);
    print_str(" Med="); print_int(vet.n_medications);
    print_str(" Board="); print_int(vet.n_boarding);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)vet.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Veterinary Clinic Demo ===\n\n");
    vet_init();

    print_str("Registering owners...\n");
    vet_register_owner(1);
    vet_register_owner(0);
    vet_register_owner(1);
    vet_register_owner(0);
    vet_register_owner(1);
    vet_register_owner(0);
    vet_register_owner(1);
    vet_register_owner(0);
    vet_register_owner(1);
    vet_register_owner(0);

    print_str("\nRegistering pets...\n");
    vet_register_pet(0, 1, 1, 24, 12.0);
    vet_register_pet(0, 1, 2, 36, 8.5);
    vet_register_pet(1, 2, 3, 12, 4.0);
    vet_register_pet(2, 1, 1, 18, 15.0);
    vet_register_pet(3, 2, 4, 6, 3.5);
    vet_register_pet(4, 1, 5, 48, 20.0);
    vet_register_pet(4, 3, 6, 12, 0.5);
    vet_register_pet(5, 1, 2, 30, 10.0);
    vet_register_pet(6, 4, 7, 24, 2.0);
    vet_register_pet(7, 1, 1, 8, 6.0);
    vet_register_pet(8, 2, 3, 18, 5.0);
    vet_register_pet(8, 1, 8, 60, 25.0);
    vet_register_pet(9, 1, 2, 15, 7.0);
    vet_register_pet(9, 3, 9, 24, 0.3);

    print_str("\nAdding medications...\n");
    vet_add_medication(1, 50, 5.0, 15.0);
    vet_add_medication(1, 40, 8.0, 20.0);
    vet_add_medication(2, 30, 12.0, 30.0);
    vet_add_medication(2, 25, 15.0, 35.0);
    vet_add_medication(3, 60, 3.0, 10.0);
    vet_add_medication(3, 45, 6.0, 18.0);
    vet_add_medication(4, 20, 25.0, 55.0);
    vet_add_medication(4, 15, 30.0, 65.0);
    vet_add_medication(5, 35, 10.0, 25.0);
    vet_add_medication(5, 20, 20.0, 45.0);
    vet_add_medication(1, 50, 4.0, 12.0);
    vet_add_medication(2, 40, 7.0, 22.0);

    print_str("\nTreatments...\n");
    vet_treatment(0, 1, 80.0, 25.0, 10);
    vet_treatment(1, 1, 75.0, 20.0, 10);
    vet_treatment(2, 2, 120.0, 40.0, 11);
    vet_treatment(3, 1, 85.0, 25.0, 11);
    vet_treatment(4, 3, 150.0, 50.0, 12);
    vet_treatment(5, 2, 110.0, 35.0, 12);
    vet_treatment(6, 1, 70.0, 18.0, 13);
    vet_treatment(7, 3, 160.0, 55.0, 13);
    vet_treatment(8, 2, 125.0, 40.0, 14);
    vet_treatment(9, 1, 80.0, 22.0, 14);
    vet_treatment(10, 2, 115.0, 38.0, 15);
    vet_treatment(11, 3, 170.0, 60.0, 15);
    vet_treatment(12, 1, 78.0, 20.0, 16);
    vet_treatment(13, 2, 130.0, 42.0, 16);
    vet_treatment(0, 1, 82.0, 24.0, 17);
    vet_treatment(3, 2, 118.0, 38.0, 17);

    print_str("\nVaccinations...\n");
    vet_vaccinate(0, 0, 45.0);
    vet_vaccinate(1, 0, 45.0);
    vet_vaccinate(2, 1, 55.0);
    vet_vaccinate(3, 0, 45.0);
    vet_vaccinate(4, 2, 65.0);
    vet_vaccinate(5, 1, 55.0);
    vet_vaccinate(7, 0, 45.0);
    vet_vaccinate(8, 2, 65.0);
    vet_vaccinate(9, 0, 45.0);
    vet_vaccinate(11, 1, 55.0);

    print_str("\nSurgeries...\n");
    vet_surgery(0, 1, 800.0, 300.0, 90, 12);
    vet_surgery(3, 2, 1200.0, 450.0, 120, 13);
    vet_surgery(5, 1, 750.0, 280.0, 75, 14);
    vet_surgery(7, 3, 1500.0, 600.0, 150, 15);
    vet_surgery(9, 2, 950.0, 350.0, 100, 16);
    vet_surgery(11, 1, 700.0, 260.0, 60, 17);
    vet_surgery(2, 1, 650.0, 240.0, 55, 17);
    vet_surgery(4, 3, 1800.0, 700.0, 180, 18);

    print_str("\nBoarding...\n");
    vet_board(0, 5, 40.0);
    vet_board(3, 7, 45.0);
    vet_board(5, 3, 50.0);
    vet_board(7, 10, 40.0);
    vet_board(9, 4, 35.0);
    vet_board(11, 6, 55.0);
    vet_board(1, 3, 38.0);
    vet_board(8, 5, 42.0);

    print_str("\nPet report...\n");
    vet_pet_report();

    print_str("\nFinancial report...\n");
    vet_financial_report();

    print_str("\nFinal state...\n");
    vet_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
