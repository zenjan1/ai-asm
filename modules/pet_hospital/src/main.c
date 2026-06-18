/* pet_hospital: Pet hospital management (v1.0)
 * Pet records, appointments, treatments, pharmacy, billing
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
#define MAX_VETS       8
#define MAX_APPOINTMENTS 16
#define MAX_TREATMENTS 12
#define MAX_MEDICINES  10
#define MAX_BILLS      14

typedef struct {
    int    pet_id;
    int    species;
    int    breed;
    int    age_months;
    double weight_kg;
    int    vaccinated;
    int    n_visits;
    double total_spent;
    int    active;
} pet_t;

typedef struct {
    int    vet_id;
    int    specialty;
    int    rating;
    int    n_appointments;
    int    n_treatments;
    double revenue;
    int    active;
} vet_t;

typedef struct {
    int    appt_id;
    int    pet_id;
    int    vet_id;
    int    type;
    int    day;
    int    status;
    int    active;
} appointment_t;

typedef struct {
    int    treatment_id;
    int    appt_id;
    int    pet_id;
    int    vet_id;
    int    type;
    double cost;
    int    day;
    int    active;
} treatment_t;

typedef struct {
    int    medicine_id;
    int    type;
    double price;
    double cost;
    int    stock;
    int    n_dispensed;
    double revenue;
    int    active;
} medicine_t;

typedef struct {
    int    bill_id;
    int    pet_id;
    int    appt_id;
    double consult_fee;
    double treatment_cost;
    double medicine_cost;
    double total;
    int    paid;
    int    active;
} bill_t;

typedef struct {
    int    n_pets;
    int    n_vets;
    int    n_appts;
    int    n_treatments;
    int    n_medicines;
    int    n_bills;
    double total_revenue;
    double consult_revenue;
    double treatment_revenue;
    double medicine_revenue;
    double total_costs;
    int    total_visits;
} ph_state_t;

static pet_t pets[MAX_PETS];
static vet_t vets[MAX_VETS];
static appointment_t appointments[MAX_APPOINTMENTS];
static treatment_t treatments[MAX_TREATMENTS];
static medicine_t medicines[MAX_MEDICINES];
static bill_t bills[MAX_BILLS];
static ph_state_t ph;

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

int ph_init(void) {
    if (initialized) return -1;
    ph.n_pets = 0; ph.n_vets = 0; ph.n_appts = 0;
    ph.n_treatments = 0; ph.n_medicines = 0; ph.n_bills = 0;
    ph.total_revenue = 0.0; ph.consult_revenue = 0.0;
    ph.treatment_revenue = 0.0; ph.medicine_revenue = 0.0;
    ph.total_costs = 0.0; ph.total_visits = 0;
    for (int i = 0; i < MAX_PETS; i++) pets[i].active = 0;
    for (int i = 0; i < MAX_VETS; i++) vets[i].active = 0;
    for (int i = 0; i < MAX_APPOINTMENTS; i++) appointments[i].active = 0;
    for (int i = 0; i < MAX_TREATMENTS; i++) treatments[i].active = 0;
    for (int i = 0; i < MAX_MEDICINES; i++) medicines[i].active = 0;
    for (int i = 0; i < MAX_BILLS; i++) bills[i].active = 0;
    initialized = 1;
    print_str("[PH] Pet hospital initialized\n");
    return 0;
}

int ph_register_pet(int species, int breed, int age, double weight) {
    if (ph.n_pets >= MAX_PETS) return -1;
    pet_t* p = &pets[ph.n_pets];
    p->pet_id = ph.n_pets;
    p->species = species;
    p->breed = breed;
    p->age_months = age;
    p->weight_kg = weight;
    p->vaccinated = 0;
    p->n_visits = 0;
    p->total_spent = 0.0;
    p->active = 1;
    ph.n_pets++;
    print_str("[PH] Pet "); print_int(ph.n_pets - 1);
    print_str(" species="); print_int(species);
    print_str(" breed="); print_int(breed);
    print_str(" age="); print_int(age); print_str("mo");
    print_str(" wt="); print_int((int)weight); print_str("kg\n");
    return ph.n_pets - 1;
}

int ph_add_vet(int specialty, int rating) {
    if (ph.n_vets >= MAX_VETS) return -1;
    vet_t* v = &vets[ph.n_vets];
    v->vet_id = ph.n_vets;
    v->specialty = specialty;
    v->rating = rating;
    v->n_appointments = 0;
    v->n_treatments = 0;
    v->revenue = 0.0;
    v->active = 1;
    ph.n_vets++;
    print_str("[PH] Vet "); print_int(ph.n_vets - 1);
    print_str(" specialty="); print_int(specialty);
    print_str(" rating="); print_int(rating); print_str("\n");
    return ph.n_vets - 1;
}

int ph_add_medicine(int type, double price, double cost, int stock) {
    if (ph.n_medicines >= MAX_MEDICINES) return -1;
    medicine_t* m = &medicines[ph.n_medicines];
    m->medicine_id = ph.n_medicines;
    m->type = type;
    m->price = price;
    m->cost = cost;
    m->stock = stock;
    m->n_dispensed = 0;
    m->revenue = 0.0;
    m->active = 1;
    ph.n_medicines++;
    print_str("[PH] Medicine "); print_int(ph.n_medicines - 1);
    print_str(" type="); print_int(type);
    print_str(" price="); print_int((int)price);
    print_str(" stock="); print_int(stock); print_str("\n");
    return ph.n_medicines - 1;
}

int ph_make_appointment(int pet_id, int vet_id, int type, int day) {
    if (ph.n_appts >= MAX_APPOINTMENTS) return -1;
    if (pet_id >= ph.n_pets || vet_id >= ph.n_vets) return -2;
    appointment_t* a = &appointments[ph.n_appts];
    a->appt_id = ph.n_appts;
    a->pet_id = pet_id;
    a->vet_id = vet_id;
    a->type = type;
    a->day = day;
    a->status = 1;
    a->active = 1;
    vets[vet_id].n_appointments++;
    ph.n_appts++;
    print_str("[PH] Appt "); print_int(ph.n_appts - 1);
    print_str(" P"); print_int(pet_id);
    print_str(" V"); print_int(vet_id);
    print_str(" type="); print_int(type);
    print_str(" day="); print_int(day); print_str("\n");
    return ph.n_appts - 1;
}

int ph_perform_treatment(int appt_id, int type, double cost) {
    if (ph.n_treatments >= MAX_TREATMENTS || appt_id >= ph.n_appts) return -1;
    appointment_t* a = &appointments[appt_id];
    treatment_t* t = &treatments[ph.n_treatments];
    t->treatment_id = ph.n_treatments;
    t->appt_id = appt_id;
    t->pet_id = a->pet_id;
    t->vet_id = a->vet_id;
    t->type = type;
    t->cost = cost;
    t->day = a->day;
    t->active = 1;
    vets[a->vet_id].n_treatments++;
    vets[a->vet_id].revenue += cost;
    ph.treatment_revenue += cost;
    ph.total_revenue += cost;
    ph.n_treatments++;
    print_str("[PH] Treatment "); print_int(ph.n_treatments - 1);
    print_str(" A"); print_int(appt_id);
    print_str(" type="); print_int(type);
    print_str(" cost="); print_int((int)cost); print_str("\n");
    return ph.n_treatments - 1;
}

int ph_dispense_medicine(int medicine_id, int appt_id, int qty) {
    if (medicine_id >= ph.n_medicines || appt_id >= ph.n_appts) return -1;
    medicine_t* m = &medicines[medicine_id];
    if (m->stock < qty) return -2;
    m->stock -= qty;
    m->n_dispensed += qty;
    double revenue = m->price * qty;
    m->revenue += revenue;
    ph.medicine_revenue += revenue;
    ph.total_revenue += revenue;
    ph.total_costs += m->cost * qty;
    print_str("[PH] Dispensed M"); print_int(medicine_id);
    print_str(" qty="); print_int(qty);
    print_str(" revenue="); print_int((int)revenue);
    print_str(" stock="); print_int(m->stock); print_str("\n");
    return 0;
}

void ph_vaccinate(int pet_id) {
    if (pet_id >= ph.n_pets) return;
    pets[pet_id].vaccinated = 1;
    print_str("[PH] Pet "); print_int(pet_id); print_str(" vaccinated\n");
}

void ph_complete_appointment(int appt_id, double consult_fee) {
    if (appt_id >= ph.n_appts) return;
    appointment_t* a = &appointments[appt_id];
    a->status = 3;
    pets[a->pet_id].n_visits++;
    pets[a->pet_id].total_spent += consult_fee;
    vets[a->vet_id].revenue += consult_fee;
    ph.consult_revenue += consult_fee;
    ph.total_revenue += consult_fee;
    ph.total_visits++;
    print_str("[PH] Appt "); print_int(appt_id);
    print_str(" completed P"); print_int(a->pet_id);
    print_str(" V"); print_int(a->vet_id);
    print_str(" consult="); print_int((int)consult_fee); print_str("\n");
}

int ph_generate_bill(int appt_id) {
    if (ph.n_bills >= MAX_BILLS || appt_id >= ph.n_appts) return -1;
    appointment_t* a = &appointments[appt_id];
    double treat_cost = 0.0;
    for (int i = 0; i < ph.n_treatments; i++) {
        if (treatments[i].appt_id == appt_id) {
            treat_cost += treatments[i].cost;
        }
    }
    bill_t* b = &bills[ph.n_bills];
    b->bill_id = ph.n_bills;
    b->pet_id = a->pet_id;
    b->appt_id = appt_id;
    b->consult_fee = 50.0;
    b->treatment_cost = treat_cost;
    b->medicine_cost = 0.0;
    b->total = b->consult_fee + b->treatment_cost;
    b->paid = 0;
    b->active = 1;
    ph.n_bills++;
    print_str("[PH] Bill "); print_int(ph.n_bills - 1);
    print_str(" P"); print_int(a->pet_id);
    print_str(" consult="); print_int((int)b->consult_fee);
    print_str(" treat="); print_int((int)b->treatment_cost);
    print_str(" total="); print_int((int)b->total); print_str("\n");
    return ph.n_bills - 1;
}

void ph_pay_bill(int bill_id) {
    if (bill_id >= ph.n_bills) return;
    bills[bill_id].paid = 1;
    pets[bills[bill_id].pet_id].total_spent += bills[bill_id].total;
    print_str("[PH] Bill "); print_int(bill_id); print_str(" paid\n");
}

void ph_vet_performance(void) {
    print_str("[PH] Vet performance:\n");
    for (int i = 0; i < ph.n_vets; i++) {
        vet_t* v = &vets[i];
        print_str("  V"); print_int(i);
        print_str(" specialty="); print_int(v->specialty);
        print_str(" rating="); print_int(v->rating);
        print_str(" appts="); print_int(v->n_appointments);
        print_str(" treats="); print_int(v->n_treatments);
        print_str(" rev="); print_int((int)v->revenue); print_str("\n");
    }
}

void ph_financial_report(void) {
    print_str("[PH] Financial report:\n");
    print_str("  Consult revenue: "); print_int((int)ph.consult_revenue); print_str("\n");
    print_str("  Treatment revenue: "); print_int((int)ph.treatment_revenue); print_str("\n");
    print_str("  Medicine revenue: "); print_int((int)ph.medicine_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)ph.total_revenue); print_str("\n");
    print_str("  Total costs: "); print_int((int)ph.total_costs); print_str("\n");
    double profit = ph.total_revenue - ph.total_costs;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    print_str("  Total visits: "); print_int(ph.total_visits); print_str("\n");
}

void ph_print_state(void) {
    print_str("[PH] Pets="); print_int(ph.n_pets);
    print_str(" Vets="); print_int(ph.n_vets);
    print_str(" Appts="); print_int(ph.n_appts);
    print_str(" Treatments="); print_int(ph.n_treatments);
    print_str(" Medicines="); print_int(ph.n_medicines);
    print_str(" Bills="); print_int(ph.n_bills);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)ph.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Pet Hospital Demo ===\n\n");
    ph_init();

    print_str("Registering pets...\n");
    ph_register_pet(1, 1, 24, 8.5);
    ph_register_pet(1, 2, 36, 12.0);
    ph_register_pet(2, 1, 18, 4.2);
    ph_register_pet(2, 2, 48, 5.5);
    ph_register_pet(1, 3, 12, 6.8);
    ph_register_pet(3, 1, 30, 0.3);
    ph_register_pet(1, 1, 60, 15.0);
    ph_register_pet(2, 3, 24, 3.8);
    ph_register_pet(1, 2, 8, 3.2);
    ph_register_pet(2, 1, 42, 4.5);

    print_str("\nAdding vets...\n");
    ph_add_vet(1, 92);
    ph_add_vet(2, 88);
    ph_add_vet(3, 90);
    ph_add_vet(1, 85);
    ph_add_vet(2, 95);

    print_str("\nAdding medicines...\n");
    ph_add_medicine(1, 15.0, 5.0, 50);
    ph_add_medicine(2, 25.0, 8.0, 40);
    ph_add_medicine(3, 12.0, 3.0, 60);
    ph_add_medicine(4, 35.0, 12.0, 30);
    ph_add_medicine(1, 18.0, 6.0, 45);
    ph_add_medicine(2, 20.0, 7.0, 55);

    print_str("\nMaking appointments...\n");
    ph_make_appointment(0, 0, 1, 10);
    ph_make_appointment(1, 1, 2, 10);
    ph_make_appointment(2, 2, 1, 11);
    ph_make_appointment(3, 3, 3, 11);
    ph_make_appointment(4, 4, 1, 12);
    ph_make_appointment(5, 0, 2, 12);
    ph_make_appointment(6, 1, 1, 13);
    ph_make_appointment(7, 2, 3, 13);
    ph_make_appointment(8, 3, 1, 14);
    ph_make_appointment(9, 4, 2, 14);
    ph_make_appointment(0, 0, 3, 15);
    ph_make_appointment(1, 1, 1, 15);
    ph_make_appointment(2, 2, 2, 16);
    ph_make_appointment(3, 3, 1, 16);
    ph_make_appointment(4, 4, 3, 17);
    ph_make_appointment(5, 0, 1, 17);

    print_str("\nPerforming treatments...\n");
    ph_perform_treatment(0, 1, 80.0);
    ph_perform_treatment(1, 2, 120.0);
    ph_perform_treatment(2, 1, 60.0);
    ph_perform_treatment(3, 3, 200.0);
    ph_perform_treatment(4, 1, 75.0);
    ph_perform_treatment(5, 2, 150.0);
    ph_perform_treatment(6, 1, 90.0);
    ph_perform_treatment(7, 3, 180.0);
    ph_perform_treatment(8, 1, 65.0);
    ph_perform_treatment(9, 2, 110.0);
    ph_perform_treatment(10, 1, 85.0);
    ph_perform_treatment(11, 1, 70.0);

    print_str("\nDispensing medicines...\n");
    ph_dispense_medicine(0, 0, 2);
    ph_dispense_medicine(1, 1, 1);
    ph_dispense_medicine(2, 2, 3);
    ph_dispense_medicine(3, 3, 1);
    ph_dispense_medicine(4, 4, 2);
    ph_dispense_medicine(5, 5, 1);
    ph_dispense_medicine(0, 6, 2);
    ph_dispense_medicine(1, 7, 1);
    ph_dispense_medicine(2, 8, 2);
    ph_dispense_medicine(3, 9, 1);
    ph_dispense_medicine(4, 10, 1);
    ph_dispense_medicine(5, 11, 2);

    print_str("\nVaccinating pets...\n");
    ph_vaccinate(0);
    ph_vaccinate(1);
    ph_vaccinate(3);
    ph_vaccinate(4);
    ph_vaccinate(6);
    ph_vaccinate(7);
    ph_vaccinate(9);

    print_str("\nCompleting appointments...\n");
    for (int i = 0; i < ph.n_appts; i++) {
        ph_complete_appointment(i, 50.0);
    }

    print_str("\nGenerating bills...\n");
    for (int i = 0; i < ph.n_appts; i++) {
        ph_generate_bill(i);
    }

    print_str("\nPaying bills...\n");
    for (int i = 0; i < ph.n_bills; i++) {
        ph_pay_bill(i);
    }

    print_str("\nVet performance...\n");
    ph_vet_performance();

    print_str("\nFinancial report...\n");
    ph_financial_report();

    print_str("\nFinal state...\n");
    ph_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
