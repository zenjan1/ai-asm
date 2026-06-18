/* auto_repair: Auto repair shop management (v1.0)
 * Vehicles, work orders, mechanics, parts, customers, analytics
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

#define MAX_VEHICLES   14
#define MAX_ORDERS     16
#define MAX_MECHANICS  8
#define MAX_PARTS      12
#define MAX_CUSTOMERS  10

typedef struct {
    int    vehicle_id;
    int    type;
    int    year;
    int    mileage;
    int    customer_id;
    int    n_repairs;
    int    active;
} vehicle_t;

typedef struct {
    int    order_id;
    int    vehicle_id;
    int    mechanic_id;
    int    type;
    int    labor_hours;
    double labor_rate;
    double parts_cost;
    double total;
    int    status;
    int    day;
    int    active;
} order_t;

typedef struct {
    int    mechanic_id;
    int    specialty;
    int    n_jobs;
    int    total_hours;
    double total_earned;
    int    rating;
    int    active;
} mechanic_t;

typedef struct {
    int    part_id;
    int    type;
    double price;
    double cost;
    int    stock;
    int    n_sold;
    double revenue;
    int    active;
} part_t;

typedef struct {
    int    customer_id;
    int    n_vehicles;
    int    n_visits;
    double total_spent;
    int    points;
    int    active;
} customer_t;

typedef struct {
    int    n_vehicles;
    int    n_orders;
    int    n_mechanics;
    int    n_parts;
    int    n_customers;
    double labor_revenue;
    double parts_revenue;
    double total_revenue;
    double total_cost;
    int    total_hours;
} ar_state_t;

static vehicle_t vehicles[MAX_VEHICLES];
static order_t orders[MAX_ORDERS];
static mechanic_t mechanics[MAX_MECHANICS];
static part_t parts[MAX_PARTS];
static customer_t customers[MAX_CUSTOMERS];
static ar_state_t ar;

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

int ar_init(void) {
    if (initialized) return -1;
    ar.n_vehicles = 0; ar.n_orders = 0; ar.n_mechanics = 0;
    ar.n_parts = 0; ar.n_customers = 0;
    ar.labor_revenue = 0.0; ar.parts_revenue = 0.0;
    ar.total_revenue = 0.0; ar.total_cost = 0.0;
    ar.total_hours = 0;
    for (int i = 0; i < MAX_VEHICLES; i++) vehicles[i].active = 0;
    for (int i = 0; i < MAX_ORDERS; i++) orders[i].active = 0;
    for (int i = 0; i < MAX_MECHANICS; i++) mechanics[i].active = 0;
    for (int i = 0; i < MAX_PARTS; i++) parts[i].active = 0;
    for (int i = 0; i < MAX_CUSTOMERS; i++) customers[i].active = 0;
    initialized = 1;
    print_str("[AR] Auto repair initialized\n");
    return 0;
}

int ar_register_customer(void) {
    if (ar.n_customers >= MAX_CUSTOMERS) return -1;
    customer_t* c = &customers[ar.n_customers];
    c->customer_id = ar.n_customers;
    c->n_vehicles = 0;
    c->n_visits = 0;
    c->total_spent = 0.0;
    c->points = 0;
    c->active = 1;
    ar.n_customers++;
    print_str("[AR] Customer "); print_int(ar.n_customers - 1); print_str(" registered\n");
    return ar.n_customers - 1;
}

int ar_add_vehicle(int customer_id, int type, int year, int mileage) {
    if (ar.n_vehicles >= MAX_VEHICLES || customer_id >= ar.n_customers) return -1;
    vehicle_t* v = &vehicles[ar.n_vehicles];
    v->vehicle_id = ar.n_vehicles;
    v->type = type;
    v->year = year;
    v->mileage = mileage;
    v->customer_id = customer_id;
    v->n_repairs = 0;
    v->active = 1;
    customers[customer_id].n_vehicles++;
    ar.n_vehicles++;
    print_str("[AR] Vehicle "); print_int(ar.n_vehicles - 1);
    print_str(" C"); print_int(customer_id);
    print_str(" type="); print_int(type);
    print_str(" year="); print_int(year);
    print_str(" mi="); print_int(mileage); print_str("\n");
    return ar.n_vehicles - 1;
}

int ar_add_mechanic(int specialty, int rating) {
    if (ar.n_mechanics >= MAX_MECHANICS) return -1;
    mechanic_t* m = &mechanics[ar.n_mechanics];
    m->mechanic_id = ar.n_mechanics;
    m->specialty = specialty;
    m->n_jobs = 0;
    m->total_hours = 0;
    m->total_earned = 0.0;
    m->rating = rating;
    m->active = 1;
    ar.n_mechanics++;
    print_str("[AR] Mechanic "); print_int(ar.n_mechanics - 1);
    print_str(" specialty="); print_int(specialty);
    print_str(" rating="); print_int(rating); print_str("\n");
    return ar.n_mechanics - 1;
}

int ar_add_part(int type, double price, double cost, int stock) {
    if (ar.n_parts >= MAX_PARTS) return -1;
    part_t* p = &parts[ar.n_parts];
    p->part_id = ar.n_parts;
    p->type = type;
    p->price = price;
    p->cost = cost;
    p->stock = stock;
    p->n_sold = 0;
    p->revenue = 0.0;
    p->active = 1;
    ar.n_parts++;
    print_str("[AR] Part "); print_int(ar.n_parts - 1);
    print_str(" type="); print_int(type);
    print_str(" price="); print_int((int)price);
    print_str(" stock="); print_int(stock); print_str("\n");
    return ar.n_parts - 1;
}

int ar_create_order(int vehicle_id, int mechanic_id, int type, int labor_hours, int part_id, int part_qty, int day) {
    if (ar.n_orders >= MAX_ORDERS) return -1;
    if (vehicle_id >= ar.n_vehicles || mechanic_id >= ar.n_mechanics) return -2;
    order_t* o = &orders[ar.n_orders];
    o->order_id = ar.n_orders;
    o->vehicle_id = vehicle_id;
    o->mechanic_id = mechanic_id;
    o->type = type;
    o->labor_hours = labor_hours;
    double rates[3] = {45.0, 65.0, 85.0};
    o->labor_rate = rates[type < 3 ? type : 0];
    o->parts_cost = 0.0;
    if (part_id >= 0 && part_id < ar.n_parts && part_qty > 0) {
        part_t* p = &parts[part_id];
        if (p->stock >= part_qty) {
            p->stock -= part_qty;
            p->n_sold += part_qty;
            o->parts_cost = p->price * part_qty;
            p->revenue += o->parts_cost;
            ar.parts_revenue += o->parts_cost;
            ar.total_cost += p->cost * part_qty;
        }
    }
    o->total = o->labor_rate * labor_hours + o->parts_cost;
    o->status = 3;
    o->day = day;
    o->active = 1;
    vehicles[vehicle_id].n_repairs++;
    mechanic_t* m = &mechanics[mechanic_id];
    m->n_jobs++;
    m->total_hours += labor_hours;
    double earn = o->labor_rate * labor_hours * 0.6;
    m->total_earned += earn;
    customer_t* c = &customers[vehicles[vehicle_id].customer_id];
    c->n_visits++;
    c->total_spent += o->total;
    c->points += (int)o->total;
    ar.labor_revenue += o->labor_rate * labor_hours;
    ar.total_revenue += o->total;
    ar.total_hours += labor_hours;
    ar.n_orders++;
    print_str("[AR] Order "); print_int(ar.n_orders - 1);
    print_str(" V"); print_int(vehicle_id);
    print_str(" M"); print_int(mechanic_id);
    print_str(" type="); print_int(type);
    print_str(" hrs="); print_int(labor_hours);
    print_str(" total="); print_int((int)o->total); print_str("\n");
    return ar.n_orders - 1;
}

void ar_mechanic_report(void) {
    print_str("[AR] Mechanic report:\n");
    for (int i = 0; i < ar.n_mechanics; i++) {
        mechanic_t* m = &mechanics[i];
        print_str("  M"); print_int(i);
        print_str(" spec="); print_int(m->specialty);
        print_str(" jobs="); print_int(m->n_jobs);
        print_str(" hrs="); print_int(m->total_hours);
        print_str(" earned="); print_int((int)m->total_earned);
        print_str(" rating="); print_int(m->rating); print_str("\n");
    }
}

void ar_parts_inventory(void) {
    print_str("[AR] Parts inventory:\n");
    for (int i = 0; i < ar.n_parts; i++) {
        part_t* p = &parts[i];
        print_str("  P"); print_int(i);
        print_str(" type="); print_int(p->type);
        print_str(" stock="); print_int(p->stock);
        print_str(" sold="); print_int(p->n_sold);
        print_str(" rev="); print_int((int)p->revenue); print_str("\n");
    }
}

void ar_financial_report(void) {
    print_str("[AR] Financial report:\n");
    double profit = ar.total_revenue - ar.total_cost;
    print_str("  Labor revenue: "); print_int((int)ar.labor_revenue); print_str("\n");
    print_str("  Parts revenue: "); print_int((int)ar.parts_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)ar.total_revenue); print_str("\n");
    print_str("  Parts cost: "); print_int((int)ar.total_cost); print_str("\n");
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    print_str("  Total hours: "); print_int(ar.total_hours); print_str("\n");
}

void ar_print_state(void) {
    print_str("[AR] Vehicles="); print_int(ar.n_vehicles);
    print_str(" Orders="); print_int(ar.n_orders);
    print_str(" Mechanics="); print_int(ar.n_mechanics);
    print_str(" Parts="); print_int(ar.n_parts);
    print_str(" Customers="); print_int(ar.n_customers);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)ar.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Auto Repair Demo ===\n\n");
    ar_init();

    print_str("Registering customers...\n");
    for (int i = 0; i < 8; i++) {
        ar_register_customer();
    }

    print_str("\nAdding vehicles...\n");
    ar_add_vehicle(0, 1, 2020, 45000);
    ar_add_vehicle(0, 2, 2018, 72000);
    ar_add_vehicle(1, 1, 2021, 28000);
    ar_add_vehicle(2, 3, 2019, 55000);
    ar_add_vehicle(3, 2, 2022, 15000);
    ar_add_vehicle(4, 1, 2017, 88000);
    ar_add_vehicle(5, 3, 2020, 42000);
    ar_add_vehicle(5, 2, 2021, 32000);
    ar_add_vehicle(6, 1, 2019, 60000);
    ar_add_vehicle(7, 2, 2023, 8000);
    ar_add_vehicle(7, 3, 2020, 38000);
    ar_add_vehicle(3, 1, 2022, 20000);
    ar_add_vehicle(6, 2, 2018, 65000);
    ar_add_vehicle(4, 3, 2021, 25000);

    print_str("\nAdding mechanics...\n");
    ar_add_mechanic(1, 92);
    ar_add_mechanic(2, 88);
    ar_add_mechanic(3, 95);
    ar_add_mechanic(1, 85);
    ar_add_mechanic(2, 90);
    ar_add_mechanic(3, 87);
    ar_add_mechanic(1, 93);
    ar_add_mechanic(2, 89);

    print_str("\nAdding parts...\n");
    ar_add_part(1, 45.0, 20.0, 30);
    ar_add_part(1, 65.0, 28.0, 25);
    ar_add_part(2, 120.0, 55.0, 15);
    ar_add_part(2, 85.0, 40.0, 20);
    ar_add_part(3, 250.0, 120.0, 10);
    ar_add_part(3, 180.0, 85.0, 12);
    ar_add_part(1, 35.0, 15.0, 40);
    ar_add_part(2, 95.0, 45.0, 18);
    ar_add_part(3, 320.0, 150.0, 8);
    ar_add_part(1, 55.0, 25.0, 28);
    ar_add_part(2, 140.0, 65.0, 14);
    ar_add_part(3, 210.0, 100.0, 11);

    print_str("\nCreating work orders...\n");
    ar_create_order(0, 0, 1, 2, 0, 1, 10);
    ar_create_order(1, 1, 2, 3, 2, 1, 11);
    ar_create_order(2, 2, 1, 1, 1, 2, 12);
    ar_create_order(3, 3, 3, 4, 4, 1, 13);
    ar_create_order(4, 4, 1, 2, 3, 1, 14);
    ar_create_order(5, 5, 2, 3, 5, 1, 15);
    ar_create_order(6, 6, 1, 1, 6, 2, 16);
    ar_create_order(7, 7, 3, 5, 8, 1, 17);
    ar_create_order(8, 0, 2, 2, 7, 1, 18);
    ar_create_order(9, 1, 3, 1, 9, 2, 19);
    ar_create_order(10, 2, 1, 3, 10, 1, 20);
    ar_create_order(11, 3, 2, 2, 11, 1, 21);
    ar_create_order(12, 4, 3, 4, 4, 1, 22);
    ar_create_order(13, 5, 1, 1, 0, 2, 23);
    ar_create_order(0, 6, 2, 3, 2, 1, 24);
    ar_create_order(1, 7, 1, 2, 1, 1, 25);

    print_str("\nMechanic report...\n");
    ar_mechanic_report();

    print_str("\nParts inventory...\n");
    ar_parts_inventory();

    print_str("\nFinancial report...\n");
    ar_financial_report();

    print_str("\nFinal state...\n");
    ar_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
