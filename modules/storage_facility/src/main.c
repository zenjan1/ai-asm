/* storage_facility: Storage facility management (v1.0)
 * Units, rentals, customers, access, security, analytics
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

#define MAX_UNITS     20
#define MAX_RENTALS   16
#define MAX_CUSTOMERS 14
#define MAX_ACCESS    18

typedef struct {
    int    unit_id;
    int    type;
    int    size_sqft;
    double monthly_rate;
    int    status;
    int    n_access;
    double revenue;
    int    active;
} unit_t;

typedef struct {
    int    rental_id;
    int    customer_id;
    int    unit_id;
    int    start_month;
    int    duration_months;
    double monthly_rate;
    double deposit;
    int    status;
    int    active;
} rental_t;

typedef struct {
    int    customer_id;
    int    tier;
    int    n_rentals;
    double total_paid;
    int    active;
} customer_t;

typedef struct {
    int    access_id;
    int    rental_id;
    int    day;
    int    hour;
    int    type;
    int    active;
} access_t;

typedef struct {
    int    n_units;
    int    n_rentals;
    int    n_customers;
    int    n_access;
    double rental_revenue;
    double deposit_revenue;
    double insurance_revenue;
    double total_revenue;
    int    occupied_units;
    int    total_access_count;
} sf_state_t;

static unit_t units[MAX_UNITS];
static rental_t rentals[MAX_RENTALS];
static customer_t customers[MAX_CUSTOMERS];
static access_t access_log[MAX_ACCESS];
static sf_state_t sf;

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

int sf_init(void) {
    if (initialized) return -1;
    sf.n_units = 0; sf.n_rentals = 0; sf.n_customers = 0;
    sf.n_access = 0;
    sf.rental_revenue = 0.0; sf.deposit_revenue = 0.0;
    sf.insurance_revenue = 0.0; sf.total_revenue = 0.0;
    sf.occupied_units = 0; sf.total_access_count = 0;
    for (int i = 0; i < MAX_UNITS; i++) units[i].active = 0;
    for (int i = 0; i < MAX_RENTALS; i++) rentals[i].active = 0;
    for (int i = 0; i < MAX_CUSTOMERS; i++) customers[i].active = 0;
    for (int i = 0; i < MAX_ACCESS; i++) access_log[i].active = 0;
    initialized = 1;
    print_str("[SF] Storage facility initialized\n");
    return 0;
}

int sf_add_unit(int type, int size, double rate) {
    if (sf.n_units >= MAX_UNITS) return -1;
    unit_t* u = &units[sf.n_units];
    u->unit_id = sf.n_units;
    u->type = type;
    u->size_sqft = size;
    u->monthly_rate = rate;
    u->status = 1;
    u->n_access = 0;
    u->revenue = 0.0;
    u->active = 1;
    sf.n_units++;
    print_str("[SF] Unit "); print_int(sf.n_units - 1);
    print_str(" type="); print_int(type);
    print_str(" size="); print_int(size); print_str("sqft");
    print_str(" rate="); print_int((int)rate); print_str("/mo\n");
    return sf.n_units - 1;
}

int sf_register_customer(int tier) {
    if (sf.n_customers >= MAX_CUSTOMERS) return -1;
    customer_t* c = &customers[sf.n_customers];
    c->customer_id = sf.n_customers;
    c->tier = tier;
    c->n_rentals = 0;
    c->total_paid = 0.0;
    c->active = 1;
    sf.n_customers++;
    print_str("[SF] Customer "); print_int(sf.n_customers - 1);
    print_str(" tier="); print_int(tier); print_str("\n");
    return sf.n_customers - 1;
}

int sf_create_rental(int customer_id, int unit_id, int start_month, int duration) {
    if (sf.n_rentals >= MAX_RENTALS) return -1;
    if (customer_id >= sf.n_customers || unit_id >= sf.n_units) return -2;
    unit_t* u = &units[unit_id];
    if (u->status != 1) return -3;
    rental_t* r = &rentals[sf.n_rentals];
    r->rental_id = sf.n_rentals;
    r->customer_id = customer_id;
    r->unit_id = unit_id;
    r->start_month = start_month;
    r->duration_months = duration;
    double tier_disc[4] = {1.0, 0.95, 0.90, 0.85};
    r->monthly_rate = u->monthly_rate * tier_disc[customers[customer_id].tier < 4 ? customers[customer_id].tier : 0];
    r->deposit = r->monthly_rate;
    r->status = 1;
    r->active = 1;
    u->status = 2;
    u->revenue += r->monthly_rate * duration;
    customers[customer_id].n_rentals++;
    customers[customer_id].total_paid += r->deposit + r->monthly_rate * duration;
    sf.rental_revenue += r->monthly_rate * duration;
    sf.deposit_revenue += r->deposit;
    sf.total_revenue += r->deposit + r->monthly_rate * duration;
    sf.occupied_units++;
    sf.n_rentals++;
    print_str("[SF] Rental "); print_int(sf.n_rentals - 1);
    print_str(" C"); print_int(customer_id);
    print_str(" U"); print_int(unit_id);
    print_str(" start="); print_int(start_month);
    print_str(" dur="); print_int(duration); print_str("mo");
    print_str(" rate="); print_int((int)r->monthly_rate);
    print_str(" deposit="); print_int((int)r->deposit); print_str("\n");
    return sf.n_rentals - 1;
}

int sf_add_insurance(int rental_id, double premium) {
    if (rental_id >= sf.n_rentals) return -1;
    sf.insurance_revenue += premium;
    sf.total_revenue += premium;
    customers[rentals[rental_id].customer_id].total_paid += premium;
    print_str("[SF] Insurance R"); print_int(rental_id);
    print_str(" premium="); print_int((int)premium); print_str("\n");
    return 0;
}

int sf_log_access(int rental_id, int day, int hour, int type) {
    if (sf.n_access >= MAX_ACCESS || rental_id >= sf.n_rentals) return -1;
    access_t* a = &access_log[sf.n_access];
    a->access_id = sf.n_access;
    a->rental_id = rental_id;
    a->day = day;
    a->hour = hour;
    a->type = type;
    a->active = 1;
    units[rentals[rental_id].unit_id].n_access++;
    sf.total_access_count++;
    sf.n_access++;
    print_str("[SF] Access "); print_int(sf.n_access - 1);
    print_str(" R"); print_int(rental_id);
    print_str(" day="); print_int(day);
    print_str(" hr="); print_int(hour);
    print_str(" type="); print_int(type); print_str("\n");
    return 0;
}

void sf_unit_report(void) {
    print_str("[SF] Unit report:\n");
    for (int i = 0; i < sf.n_units; i++) {
        unit_t* u = &units[i];
        print_str("  U"); print_int(i);
        print_str(" type="); print_int(u->type);
        print_str(" size="); print_int(u->size_sqft); print_str("sqft");
        print_str(" status="); print_int(u->status);
        print_str(" access="); print_int(u->n_access);
        print_str(" rev="); print_int((int)u->revenue); print_str("\n");
    }
}

void sf_occupancy_report(void) {
    print_str("[SF] Occupancy report:\n");
    int total = sf.n_units;
    int occupied = sf.occupied_units;
    double occ = (total > 0) ? (double)occupied / total * 100.0 : 0.0;
    print_str("  Total units: "); print_int(total); print_str("\n");
    print_str("  Occupied: "); print_int(occupied); print_str("\n");
    print_str("  Available: "); print_int(total - occupied); print_str("\n");
    print_str("  Occupancy: "); print_int((int)occ); print_str("%\n");
    print_str("  Total accesses: "); print_int(sf.total_access_count); print_str("\n");
}

void sf_financial_report(void) {
    print_str("[SF] Financial report:\n");
    print_str("  Rental revenue: "); print_int((int)sf.rental_revenue); print_str("\n");
    print_str("  Deposit revenue: "); print_int((int)sf.deposit_revenue); print_str("\n");
    print_str("  Insurance revenue: "); print_int((int)sf.insurance_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)sf.total_revenue); print_str("\n");
    print_str("  Customers: "); print_int(sf.n_customers); print_str("\n");
    print_str("  Rentals: "); print_int(sf.n_rentals); print_str("\n");
}

void sf_print_state(void) {
    print_str("[SF] Units="); print_int(sf.n_units);
    print_str(" Rentals="); print_int(sf.n_rentals);
    print_str(" Customers="); print_int(sf.n_customers);
    print_str(" Access logs="); print_int(sf.n_access);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)sf.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Storage Facility Demo ===\n\n");
    sf_init();

    print_str("Adding units...\n");
    sf_add_unit(1, 25, 59.0);
    sf_add_unit(1, 25, 59.0);
    sf_add_unit(1, 50, 89.0);
    sf_add_unit(1, 50, 89.0);
    sf_add_unit(2, 100, 149.0);
    sf_add_unit(2, 100, 149.0);
    sf_add_unit(2, 150, 199.0);
    sf_add_unit(2, 150, 199.0);
    sf_add_unit(3, 200, 279.0);
    sf_add_unit(3, 200, 279.0);
    sf_add_unit(3, 300, 379.0);
    sf_add_unit(3, 300, 379.0);
    sf_add_unit(4, 100, 249.0);
    sf_add_unit(4, 150, 329.0);
    sf_add_unit(1, 25, 59.0);
    sf_add_unit(1, 50, 89.0);
    sf_add_unit(2, 100, 149.0);
    sf_add_unit(2, 150, 199.0);
    sf_add_unit(3, 200, 279.0);
    sf_add_unit(3, 300, 379.0);

    print_str("\nRegistering customers...\n");
    for (int i = 0; i < 12; i++) {
        sf_register_customer(i % 4);
    }

    print_str("\nCreating rentals...\n");
    sf_create_rental(0, 0, 1, 6);
    sf_create_rental(1, 2, 1, 12);
    sf_create_rental(2, 4, 2, 6);
    sf_create_rental(3, 6, 2, 12);
    sf_create_rental(4, 8, 3, 6);
    sf_create_rental(5, 10, 3, 12);
    sf_create_rental(6, 12, 4, 6);
    sf_create_rental(7, 13, 4, 12);
    sf_create_rental(8, 1, 5, 6);
    sf_create_rental(9, 3, 5, 12);
    sf_create_rental(10, 5, 6, 6);
    sf_create_rental(11, 7, 6, 12);
    sf_create_rental(0, 14, 7, 6);
    sf_create_rental(1, 15, 7, 12);
    sf_create_rental(2, 16, 8, 6);
    sf_create_rental(3, 17, 8, 12);

    print_str("\nAdding insurance...\n");
    for (int i = 0; i < 12; i++) {
        sf_add_insurance(i, 15.0 + i * 2);
    }

    print_str("\nLogging access...\n");
    sf_log_access(0, 10, 9, 1);
    sf_log_access(1, 10, 11, 1);
    sf_log_access(2, 11, 14, 1);
    sf_log_access(3, 11, 10, 2);
    sf_log_access(4, 12, 15, 1);
    sf_log_access(5, 12, 9, 1);
    sf_log_access(6, 13, 10, 1);
    sf_log_access(7, 13, 13, 2);
    sf_log_access(8, 14, 11, 1);
    sf_log_access(9, 14, 16, 1);
    sf_log_access(10, 15, 10, 1);
    sf_log_access(11, 15, 14, 2);
    sf_log_access(0, 16, 9, 1);
    sf_log_access(1, 16, 11, 1);
    sf_log_access(2, 17, 15, 2);
    sf_log_access(3, 17, 10, 1);
    sf_log_access(4, 18, 13, 1);
    sf_log_access(5, 18, 9, 1);

    print_str("\nUnit report...\n");
    sf_unit_report();

    print_str("\nOccupancy report...\n");
    sf_occupancy_report();

    print_str("\nFinancial report...\n");
    sf_financial_report();

    print_str("\nFinal state...\n");
    sf_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
