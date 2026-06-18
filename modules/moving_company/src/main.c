/* moving_company: Moving service management (v1.0)
 * Services, vehicles, crews, orders, items, customers
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

#define MAX_VEHICLES   10
#define MAX_WORKERS    14
#define MAX_ORDERS     16
#define MAX_ITEMS      18
#define MAX_CUSTOMERS  12

typedef struct {
    int    vehicle_id;
    int    type;
    int    capacity_m3;
    int    mileage;
    int    status;
    int    n_jobs;
    double revenue;
    int    active;
} vehicle_t;

typedef struct {
    int    worker_id;
    int    role;
    int    n_jobs;
    int    total_hours;
    double total_earned;
    int    rating;
    int    active;
} worker_t;

typedef struct {
    int    order_id;
    int    customer_id;
    int    service_type;
    int    vehicle_id;
    int    distance_km;
    int    floors;
    double base_fee;
    double extra_fee;
    double total;
    int    status;
    int    day;
    int    active;
} order_t;

typedef struct {
    int    item_id;
    int    order_id;
    int    type;
    double volume;
    double value;
    int    special;
    int    active;
} item_t;

typedef struct {
    int    customer_id;
    int    tier;
    int    n_moves;
    double total_paid;
    int    active;
} customer_t;

typedef struct {
    int    n_vehicles;
    int    n_workers;
    int    n_orders;
    int    n_items;
    int    n_customers;
    double base_revenue;
    double extra_revenue;
    double insurance_revenue;
    double total_revenue;
    int    total_km;
} mc_state_t;

static vehicle_t vehicles[MAX_VEHICLES];
static worker_t workers[MAX_WORKERS];
static order_t orders[MAX_ORDERS];
static item_t items[MAX_ITEMS];
static customer_t customers[MAX_CUSTOMERS];
static mc_state_t mc;

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

int mc_init(void) {
    if (initialized) return -1;
    mc.n_vehicles = 0; mc.n_workers = 0; mc.n_orders = 0;
    mc.n_items = 0; mc.n_customers = 0;
    mc.base_revenue = 0.0; mc.extra_revenue = 0.0;
    mc.insurance_revenue = 0.0; mc.total_revenue = 0.0;
    mc.total_km = 0;
    for (int i = 0; i < MAX_VEHICLES; i++) vehicles[i].active = 0;
    for (int i = 0; i < MAX_WORKERS; i++) workers[i].active = 0;
    for (int i = 0; i < MAX_ORDERS; i++) orders[i].active = 0;
    for (int i = 0; i < MAX_ITEMS; i++) items[i].active = 0;
    for (int i = 0; i < MAX_CUSTOMERS; i++) customers[i].active = 0;
    initialized = 1;
    print_str("[MC] Moving company initialized\n");
    return 0;
}

int mc_add_vehicle(int type, int capacity, int mileage) {
    if (mc.n_vehicles >= MAX_VEHICLES) return -1;
    vehicle_t* v = &vehicles[mc.n_vehicles];
    v->vehicle_id = mc.n_vehicles;
    v->type = type;
    v->capacity_m3 = capacity;
    v->mileage = mileage;
    v->status = 1;
    v->n_jobs = 0;
    v->revenue = 0.0;
    v->active = 1;
    mc.n_vehicles++;
    print_str("[MC] Vehicle "); print_int(mc.n_vehicles - 1);
    print_str(" type="); print_int(type);
    print_str(" cap="); print_int(capacity); print_str("m3");
    print_str(" mi="); print_int(mileage); print_str("\n");
    return mc.n_vehicles - 1;
}

int mc_add_worker(int role, int rating) {
    if (mc.n_workers >= MAX_WORKERS) return -1;
    worker_t* w = &workers[mc.n_workers];
    w->worker_id = mc.n_workers;
    w->role = role;
    w->n_jobs = 0;
    w->total_hours = 0;
    w->total_earned = 0.0;
    w->rating = rating;
    w->active = 1;
    mc.n_workers++;
    print_str("[MC] Worker "); print_int(mc.n_workers - 1);
    print_str(" role="); print_int(role);
    print_str(" rating="); print_int(rating); print_str("\n");
    return mc.n_workers - 1;
}

int mc_register_customer(int tier) {
    if (mc.n_customers >= MAX_CUSTOMERS) return -1;
    customer_t* c = &customers[mc.n_customers];
    c->customer_id = mc.n_customers;
    c->tier = tier;
    c->n_moves = 0;
    c->total_paid = 0.0;
    c->active = 1;
    mc.n_customers++;
    print_str("[MC] Customer "); print_int(mc.n_customers - 1);
    print_str(" tier="); print_int(tier); print_str("\n");
    return mc.n_customers - 1;
}

int mc_create_order(int customer_id, int service_type, int vehicle_id, int distance, int floors, int day) {
    if (mc.n_orders >= MAX_ORDERS) return -1;
    if (customer_id >= mc.n_customers || vehicle_id >= mc.n_vehicles) return -2;
    order_t* o = &orders[mc.n_orders];
    o->order_id = mc.n_orders;
    o->customer_id = customer_id;
    o->service_type = service_type;
    o->vehicle_id = vehicle_id;
    o->distance_km = distance;
    o->floors = floors;
    double base_rates[4] = {150.0, 250.0, 400.0, 800.0};
    o->base_fee = base_rates[service_type < 4 ? service_type : 0];
    double floor_fee = floors * 25.0;
    double dist_fee = distance * 2.5;
    double tier_disc[4] = {1.0, 0.95, 0.90, 0.85};
    o->extra_fee = (floor_fee + dist_fee) * tier_disc[customers[customer_id].tier < 4 ? customers[customer_id].tier : 0];
    o->total = o->base_fee + o->extra_fee;
    o->status = 3;
    o->day = day;
    o->active = 1;
    vehicles[vehicle_id].n_jobs++;
    vehicles[vehicle_id].mileage += distance * 2;
    vehicles[vehicle_id].revenue += o->total;
    customers[customer_id].n_moves++;
    customers[customer_id].total_paid += o->total;
    mc.base_revenue += o->base_fee;
    mc.extra_revenue += o->extra_fee;
    mc.total_revenue += o->total;
    mc.total_km += distance;
    mc.n_orders++;
    print_str("[MC] Order "); print_int(mc.n_orders - 1);
    print_str(" C"); print_int(customer_id);
    print_str(" V"); print_int(vehicle_id);
    print_str(" svc="); print_int(service_type);
    print_str(" dist="); print_int(distance); print_str("km");
    print_str(" floors="); print_int(floors);
    print_str(" total="); print_int((int)o->total); print_str("\n");
    return mc.n_orders - 1;
}

int mc_add_item(int order_id, int type, double volume, double value, int special) {
    if (mc.n_items >= MAX_ITEMS || order_id >= mc.n_orders) return -1;
    item_t* it = &items[mc.n_items];
    it->item_id = mc.n_items;
    it->order_id = order_id;
    it->type = type;
    it->volume = volume;
    it->value = value;
    it->special = special;
    it->active = 1;
    if (special) {
        double ins = value * 0.03;
        mc.insurance_revenue += ins;
        mc.total_revenue += ins;
    }
    mc.n_items++;
    print_str("[MC] Item "); print_int(mc.n_items - 1);
    print_str(" O"); print_int(order_id);
    print_str(" type="); print_int(type);
    print_str(" vol="); print_int((int)volume); print_str("L");
    if (special) { print_str(" [SPECIAL]"); }
    print_str("\n");
    return mc.n_items - 1;
}

int mc_assign_worker(int order_id, int worker_id, int hours) {
    if (order_id >= mc.n_orders || worker_id >= mc.n_workers) return -1;
    workers[worker_id].n_jobs++;
    workers[worker_id].total_hours += hours;
    double pay = hours * 20.0;
    workers[worker_id].total_earned += pay;
    print_str("[MC] Worker W"); print_int(worker_id);
    print_str(" assigned to O"); print_int(order_id);
    print_str(" hrs="); print_int(hours);
    print_str(" pay="); print_int((int)pay); print_str("\n");
    return 0;
}

void mc_vehicle_report(void) {
    print_str("[MC] Vehicle report:\n");
    for (int i = 0; i < mc.n_vehicles; i++) {
        vehicle_t* v = &vehicles[i];
        print_str("  V"); print_int(i);
        print_str(" type="); print_int(v->type);
        print_str(" cap="); print_int(v->capacity_m3); print_str("m3");
        print_str(" jobs="); print_int(v->n_jobs);
        print_str(" mi="); print_int(v->mileage);
        print_str(" rev="); print_int((int)v->revenue); print_str("\n");
    }
}

void mc_worker_report(void) {
    print_str("[MC] Worker report:\n");
    for (int i = 0; i < mc.n_workers; i++) {
        worker_t* w = &workers[i];
        print_str("  W"); print_int(i);
        print_str(" role="); print_int(w->role);
        print_str(" jobs="); print_int(w->n_jobs);
        print_str(" hrs="); print_int(w->total_hours);
        print_str(" earned="); print_int((int)w->total_earned);
        print_str(" rating="); print_int(w->rating); print_str("\n");
    }
}

void mc_financial_report(void) {
    print_str("[MC] Financial report:\n");
    print_str("  Base revenue: "); print_int((int)mc.base_revenue); print_str("\n");
    print_str("  Extra revenue: "); print_int((int)mc.extra_revenue); print_str("\n");
    print_str("  Insurance revenue: "); print_int((int)mc.insurance_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)mc.total_revenue); print_str("\n");
    print_str("  Total km: "); print_int(mc.total_km); print_str("\n");
    print_str("  Total moves: "); print_int(mc.n_orders); print_str("\n");
}

void mc_print_state(void) {
    print_str("[MC] Vehicles="); print_int(mc.n_vehicles);
    print_str(" Workers="); print_int(mc.n_workers);
    print_str(" Orders="); print_int(mc.n_orders);
    print_str(" Items="); print_int(mc.n_items);
    print_str(" Customers="); print_int(mc.n_customers);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)mc.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Moving Company Demo ===\n\n");
    mc_init();

    print_str("Adding vehicles...\n");
    mc_add_vehicle(1, 10, 45000);
    mc_add_vehicle(1, 12, 32000);
    mc_add_vehicle(2, 20, 55000);
    mc_add_vehicle(2, 22, 28000);
    mc_add_vehicle(3, 35, 70000);
    mc_add_vehicle(3, 40, 45000);
    mc_add_vehicle(4, 60, 80000);
    mc_add_vehicle(4, 65, 60000);
    mc_add_vehicle(1, 15, 18000);
    mc_add_vehicle(2, 25, 38000);

    print_str("\nAdding workers...\n");
    mc_add_worker(1, 90);
    mc_add_worker(1, 88);
    mc_add_worker(1, 92);
    mc_add_worker(2, 85);
    mc_add_worker(2, 91);
    mc_add_worker(2, 87);
    mc_add_worker(3, 89);
    mc_add_worker(3, 93);
    mc_add_worker(3, 86);
    mc_add_worker(1, 94);
    mc_add_worker(2, 84);
    mc_add_worker(3, 90);
    mc_add_worker(1, 88);
    mc_add_worker(2, 91);

    print_str("\nRegistering customers...\n");
    for (int i = 0; i < 10; i++) {
        mc_register_customer(i % 4);
    }

    print_str("\nCreating orders...\n");
    mc_create_order(0, 1, 0, 15, 2, 10);
    mc_create_order(1, 2, 2, 25, 3, 11);
    mc_create_order(2, 1, 1, 12, 1, 12);
    mc_create_order(3, 3, 4, 40, 4, 13);
    mc_create_order(4, 1, 3, 18, 2, 14);
    mc_create_order(5, 2, 5, 30, 3, 15);
    mc_create_order(6, 1, 8, 10, 1, 16);
    mc_create_order(7, 3, 6, 50, 5, 17);
    mc_create_order(8, 2, 9, 22, 2, 18);
    mc_create_order(9, 1, 0, 20, 3, 19);
    mc_create_order(0, 2, 2, 35, 4, 20);
    mc_create_order(1, 1, 1, 15, 1, 21);
    mc_create_order(2, 3, 4, 45, 3, 22);
    mc_create_order(3, 1, 3, 28, 2, 23);
    mc_create_order(4, 2, 5, 32, 3, 24);
    mc_create_order(5, 1, 8, 14, 1, 25);

    print_str("\nAdding items...\n");
    mc_add_item(0, 1, 50.0, 1000.0, 0);
    mc_add_item(0, 2, 120.0, 5000.0, 1);
    mc_add_item(1, 1, 80.0, 2000.0, 0);
    mc_add_item(1, 3, 200.0, 8000.0, 1);
    mc_add_item(2, 2, 60.0, 1500.0, 0);
    mc_add_item(3, 1, 100.0, 3000.0, 0);
    mc_add_item(3, 3, 250.0, 12000.0, 1);
    mc_add_item(4, 2, 70.0, 2500.0, 0);
    mc_add_item(5, 1, 90.0, 2000.0, 0);
    mc_add_item(5, 2, 150.0, 6000.0, 1);
    mc_add_item(6, 1, 40.0, 800.0, 0);
    mc_add_item(7, 3, 300.0, 15000.0, 1);
    mc_add_item(8, 2, 110.0, 4000.0, 0);
    mc_add_item(9, 1, 55.0, 1200.0, 0);
    mc_add_item(10, 2, 130.0, 5500.0, 1);
    mc_add_item(11, 1, 45.0, 900.0, 0);
    mc_add_item(12, 3, 220.0, 9000.0, 1);
    mc_add_item(13, 2, 85.0, 3500.0, 0);

    print_str("\nAssigning workers...\n");
    mc_assign_worker(0, 0, 5);
    mc_assign_worker(0, 1, 5);
    mc_assign_worker(1, 2, 6);
    mc_assign_worker(1, 3, 6);
    mc_assign_worker(2, 4, 4);
    mc_assign_worker(3, 5, 7);
    mc_assign_worker(3, 6, 7);
    mc_assign_worker(4, 7, 5);
    mc_assign_worker(5, 8, 6);
    mc_assign_worker(5, 9, 6);
    mc_assign_worker(6, 10, 4);
    mc_assign_worker(7, 11, 8);
    mc_assign_worker(7, 12, 8);
    mc_assign_worker(8, 13, 5);
    mc_assign_worker(9, 0, 5);
    mc_assign_worker(10, 1, 6);

    print_str("\nVehicle report...\n");
    mc_vehicle_report();

    print_str("\nWorker report...\n");
    mc_worker_report();

    print_str("\nFinancial report...\n");
    mc_financial_report();

    print_str("\nFinal state...\n");
    mc_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
