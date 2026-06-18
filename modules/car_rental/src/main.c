/* car_rental: Car rental management (v1.0)
 * Fleet, bookings, customers, insurance, returns, analytics
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

#define MAX_VEHICLES   16
#define MAX_CUSTOMERS  14
#define MAX_BOOKINGS   18
#define MAX_INSURANCE  10
#define MAX_RETURNS    12

typedef struct {
    int    vehicle_id;
    int    type;
    int    year;
    int    mileage;
    double daily_rate;
    int    status;
    int    n_rentals;
    double revenue;
    int    active;
} vehicle_t;

typedef struct {
    int    customer_id;
    int    license_type;
    int    credit_score;
    int    n_rentals;
    double total_paid;
    int    n_violations;
    int    active;
} customer_t;

typedef struct {
    int    booking_id;
    int    customer_id;
    int    vehicle_id;
    int    start_day;
    int    days;
    double daily_rate;
    double insurance_fee;
    double total;
    int    status;
    int    active;
} booking_t;

typedef struct {
    int    insurance_id;
    int    type;
    double daily_fee;
    double coverage;
    int    active;
} insurance_t;

typedef struct {
    int    return_id;
    int    booking_id;
    int    end_day;
    int    end_mileage;
    int    extra_days;
    double extra_charge;
    double fuel_charge;
    double damage_charge;
    int    active;
} return_t;

typedef struct {
    int    n_vehicles;
    int    n_customers;
    int    n_bookings;
    int    n_insurance;
    int    n_returns;
    double rental_revenue;
    double insurance_revenue;
    double extra_revenue;
    double total_revenue;
    int    total_rental_days;
} cr_state_t;

static vehicle_t vehicles[MAX_VEHICLES];
static customer_t customers[MAX_CUSTOMERS];
static booking_t bookings[MAX_BOOKINGS];
static insurance_t insurances[MAX_INSURANCE];
static return_t returns[MAX_RETURNS];
static cr_state_t cr;

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

int cr_init(void) {
    if (initialized) return -1;
    cr.n_vehicles = 0; cr.n_customers = 0; cr.n_bookings = 0;
    cr.n_insurance = 0; cr.n_returns = 0;
    cr.rental_revenue = 0.0; cr.insurance_revenue = 0.0;
    cr.extra_revenue = 0.0; cr.total_revenue = 0.0;
    cr.total_rental_days = 0;
    for (int i = 0; i < MAX_VEHICLES; i++) vehicles[i].active = 0;
    for (int i = 0; i < MAX_CUSTOMERS; i++) customers[i].active = 0;
    for (int i = 0; i < MAX_BOOKINGS; i++) bookings[i].active = 0;
    for (int i = 0; i < MAX_INSURANCE; i++) insurances[i].active = 0;
    for (int i = 0; i < MAX_RETURNS; i++) returns[i].active = 0;
    initialized = 1;
    print_str("[CR] Car rental initialized\n");
    return 0;
}

int cr_add_vehicle(int type, int year, int mileage, double rate) {
    if (cr.n_vehicles >= MAX_VEHICLES) return -1;
    vehicle_t* v = &vehicles[cr.n_vehicles];
    v->vehicle_id = cr.n_vehicles;
    v->type = type;
    v->year = year;
    v->mileage = mileage;
    v->daily_rate = rate;
    v->status = 1;
    v->n_rentals = 0;
    v->revenue = 0.0;
    v->active = 1;
    cr.n_vehicles++;
    print_str("[CR] Vehicle "); print_int(cr.n_vehicles - 1);
    print_str(" type="); print_int(type);
    print_str(" year="); print_int(year);
    print_str(" rate="); print_int((int)rate); print_str("/day\n");
    return cr.n_vehicles - 1;
}

int cr_register_customer(int license_type, int credit_score) {
    if (cr.n_customers >= MAX_CUSTOMERS) return -1;
    customer_t* c = &customers[cr.n_customers];
    c->customer_id = cr.n_customers;
    c->license_type = license_type;
    c->credit_score = credit_score;
    c->n_rentals = 0;
    c->total_paid = 0.0;
    c->n_violations = 0;
    c->active = 1;
    cr.n_customers++;
    print_str("[CR] Customer "); print_int(cr.n_customers - 1);
    print_str(" license="); print_int(license_type);
    print_str(" credit="); print_int(credit_score); print_str("\n");
    return cr.n_customers - 1;
}

int cr_add_insurance(int type, double fee, double coverage) {
    if (cr.n_insurance >= MAX_INSURANCE) return -1;
    insurance_t* ins = &insurances[cr.n_insurance];
    ins->insurance_id = cr.n_insurance;
    ins->type = type;
    ins->daily_fee = fee;
    ins->coverage = coverage;
    ins->active = 1;
    cr.n_insurance++;
    print_str("[CR] Insurance "); print_int(cr.n_insurance - 1);
    print_str(" type="); print_int(type);
    print_str(" fee="); print_int((int)fee);
    print_str(" coverage="); print_int((int)coverage); print_str("\n");
    return cr.n_insurance - 1;
}

int cr_make_booking(int customer_id, int vehicle_id, int start_day, int days, int ins_type) {
    if (cr.n_bookings >= MAX_BOOKINGS) return -1;
    if (customer_id >= cr.n_customers || vehicle_id >= cr.n_vehicles) return -2;
    vehicle_t* v = &vehicles[vehicle_id];
    if (v->status != 1) return -3;
    booking_t* b = &bookings[cr.n_bookings];
    b->booking_id = cr.n_bookings;
    b->customer_id = customer_id;
    b->vehicle_id = vehicle_id;
    b->start_day = start_day;
    b->days = days;
    b->daily_rate = v->daily_rate;
    double ins_fee = 0.0;
    if (ins_type >= 0 && ins_type < cr.n_insurance) {
        ins_fee = insurances[ins_type].daily_fee * days;
    }
    b->insurance_fee = ins_fee;
    b->total = v->daily_rate * days + ins_fee;
    b->status = 1;
    b->active = 1;
    v->status = 2;
    cr.insurance_revenue += ins_fee;
    cr.total_revenue += b->total;
    cr.n_bookings++;
    print_str("[CR] Booking "); print_int(cr.n_bookings - 1);
    print_str(" C"); print_int(customer_id);
    print_str(" V"); print_int(vehicle_id);
    print_str(" day="); print_int(start_day);
    print_str(" days="); print_int(days);
    print_str(" total="); print_int((int)b->total); print_str("\n");
    return cr.n_bookings - 1;
}

int cr_return_vehicle(int booking_id, int end_day, int end_mileage, double fuel_charge, double damage_charge) {
    if (cr.n_returns >= MAX_RETURNS || booking_id >= cr.n_bookings) return -1;
    booking_t* b = &bookings[booking_id];
    if (b->status != 1) return -2;
    vehicle_t* v = &vehicles[b->vehicle_id];
    return_t* r = &returns[cr.n_returns];
    r->return_id = cr.n_returns;
    r->booking_id = booking_id;
    r->end_day = end_day;
    r->end_mileage = end_mileage;
    r->extra_days = (end_day > b->start_day + b->days) ? (end_day - b->start_day - b->days) : 0;
    r->extra_charge = r->extra_days * v->daily_rate;
    r->fuel_charge = fuel_charge;
    r->damage_charge = damage_charge;
    r->active = 1;
    double extra_total = r->extra_charge + r->fuel_charge + r->damage_charge;
    v->status = 1;
    v->mileage = end_mileage;
    v->n_rentals++;
    v->revenue += b->total + extra_total;
    b->status = 3;
    customers[b->customer_id].n_rentals++;
    customers[b->customer_id].total_paid += b->total + extra_total;
    cr.rental_revenue += b->total;
    cr.extra_revenue += extra_total;
    cr.total_revenue += extra_total;
    cr.total_rental_days += b->days + r->extra_days;
    cr.n_returns++;
    print_str("[CR] Return R"); print_int(cr.n_returns - 1);
    print_str(" B"); print_int(booking_id);
    print_str(" end_day="); print_int(end_day);
    if (r->extra_days > 0) {
        print_str(" extra="); print_int(r->extra_days); print_str("d");
    }
    if (extra_total > 0.0) {
        print_str(" charges="); print_int((int)extra_total);
    }
    print_str("\n");
    return 0;
}

void cr_fleet_status(void) {
    print_str("[CR] Fleet status:\n");
    for (int i = 0; i < cr.n_vehicles; i++) {
        vehicle_t* v = &vehicles[i];
        print_str("  V"); print_int(i);
        print_str(" type="); print_int(v->type);
        print_str(" year="); print_int(v->year);
        print_str(" mi="); print_int(v->mileage);
        print_str(" status="); print_int(v->status);
        print_str(" rentals="); print_int(v->n_rentals);
        print_str(" rev="); print_int((int)v->revenue); print_str("\n");
    }
}

void cr_utilization_report(void) {
    print_str("[CR] Utilization report:\n");
    int total = cr.n_vehicles;
    int rented = 0;
    for (int i = 0; i < total; i++) {
        if (vehicles[i].status == 2) rented++;
    }
    double util = (total > 0) ? (double)rented / total * 100.0 : 0.0;
    print_str("  Total vehicles: "); print_int(total); print_str("\n");
    print_str("  Currently rented: "); print_int(rented); print_str("\n");
    print_str("  Available: "); print_int(total - rented); print_str("\n");
    print_str("  Utilization: "); print_int((int)util); print_str("%\n");
    print_str("  Total rental days: "); print_int(cr.total_rental_days); print_str("\n");
}

void cr_financial_report(void) {
    print_str("[CR] Financial report:\n");
    print_str("  Rental revenue: "); print_int((int)cr.rental_revenue); print_str("\n");
    print_str("  Insurance revenue: "); print_int((int)cr.insurance_revenue); print_str("\n");
    print_str("  Extra charges: "); print_int((int)cr.extra_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)cr.total_revenue); print_str("\n");
    print_str("  Total rentals: "); print_int(cr.n_returns); print_str("\n");
    print_str("  Customers: "); print_int(cr.n_customers); print_str("\n");
}

void cr_print_state(void) {
    print_str("[CR] Vehicles="); print_int(cr.n_vehicles);
    print_str(" Customers="); print_int(cr.n_customers);
    print_str(" Bookings="); print_int(cr.n_bookings);
    print_str(" Returns="); print_int(cr.n_returns);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)cr.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Car Rental Demo ===\n\n");
    cr_init();

    print_str("Adding vehicles...\n");
    cr_add_vehicle(1, 2022, 15000, 45.0);
    cr_add_vehicle(1, 2023, 8000, 48.0);
    cr_add_vehicle(1, 2021, 22000, 42.0);
    cr_add_vehicle(2, 2023, 5000, 65.0);
    cr_add_vehicle(2, 2022, 12000, 62.0);
    cr_add_vehicle(2, 2024, 2000, 68.0);
    cr_add_vehicle(3, 2023, 8000, 85.0);
    cr_add_vehicle(3, 2022, 18000, 80.0);
    cr_add_vehicle(3, 2024, 3000, 90.0);
    cr_add_vehicle(4, 2024, 1000, 150.0);
    cr_add_vehicle(4, 2023, 5000, 140.0);
    cr_add_vehicle(4, 2023, 7000, 145.0);
    cr_add_vehicle(1, 2024, 4000, 50.0);
    cr_add_vehicle(2, 2024, 3000, 70.0);
    cr_add_vehicle(3, 2022, 14000, 82.0);
    cr_add_vehicle(1, 2023, 10000, 46.0);

    print_str("\nRegistering customers...\n");
    for (int i = 0; i < 12; i++) {
        cr_register_customer((i % 3) + 1, 600 + i * 20);
    }

    print_str("\nAdding insurance...\n");
    cr_add_insurance(1, 10.0, 5000.0);
    cr_add_insurance(2, 18.0, 20000.0);
    cr_add_insurance(3, 25.0, 50000.0);

    print_str("\nMaking bookings...\n");
    cr_make_booking(0, 0, 10, 3, 0);
    cr_make_booking(1, 3, 10, 5, 1);
    cr_make_booking(2, 6, 11, 2, 0);
    cr_make_booking(3, 9, 11, 4, 2);
    cr_make_booking(4, 1, 12, 3, 1);
    cr_make_booking(5, 4, 12, 2, 0);
    cr_make_booking(6, 7, 13, 5, 1);
    cr_make_booking(7, 10, 13, 3, 2);
    cr_make_booking(8, 13, 14, 4, 0);
    cr_make_booking(9, 2, 14, 2, 1);
    cr_make_booking(10, 5, 15, 3, 0);
    cr_make_booking(11, 8, 15, 5, 2);
    cr_make_booking(0, 12, 16, 2, 1);
    cr_make_booking(1, 14, 16, 3, 0);
    cr_make_booking(2, 15, 17, 4, 1);
    cr_make_booking(3, 11, 17, 2, 2);
    cr_make_booking(4, 0, 18, 3, 0);
    cr_make_booking(5, 3, 18, 2, 1);

    print_str("\nReturning vehicles...\n");
    cr_return_vehicle(0, 13, 15300, 0.0, 0.0);
    cr_return_vehicle(1, 16, 5400, 15.0, 0.0);
    cr_return_vehicle(2, 14, 8200, 0.0, 50.0);
    cr_return_vehicle(3, 16, 1200, 0.0, 0.0);
    cr_return_vehicle(4, 16, 8500, 10.0, 0.0);
    cr_return_vehicle(5, 15, 12400, 0.0, 0.0);
    cr_return_vehicle(6, 19, 18500, 0.0, 100.0);
    cr_return_vehicle(7, 17, 7200, 0.0, 0.0);
    cr_return_vehicle(8, 19, 3500, 12.0, 0.0);
    cr_return_vehicle(9, 17, 22300, 0.0, 0.0);
    cr_return_vehicle(10, 19, 12600, 0.0, 75.0);
    cr_return_vehicle(11, 21, 18800, 0.0, 0.0);

    print_str("\nFleet status...\n");
    cr_fleet_status();

    print_str("\nUtilization report...\n");
    cr_utilization_report();

    print_str("\nFinancial report...\n");
    cr_financial_report();

    print_str("\nFinal state...\n");
    cr_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
