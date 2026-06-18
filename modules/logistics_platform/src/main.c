/* logistics_platform: Logistics and delivery platform (v1.0)
 * Warehouse management, fleet tracking, shipping, delivery routing, inventory
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

#define MAX_WAREHOUSES  6
#define MAX_LOCATIONS   20
#define MAX_VEHICLES    10
#define MAX_DRIVERS     10
#define MAX_ORDERS      16
#define MAX_SHIPMENTS   16
#define MAX_DELIVERIES  14

typedef struct {
    int    warehouse_id;
    int    zone;
    int    capacity;
    int    used;
    int    n_items;
    double total_value;
    int    active;
} warehouse_t;

typedef struct {
    int    location_id;
    int    warehouse_id;
    int    type;
    int    capacity;
    int    used;
    int    active;
} location_t;

typedef struct {
    int    vehicle_id;
    int    type;
    int    capacity;
    double fuel_consumption;
    int    mileage;
    int    trips;
    int    status;
    int    active;
} vehicle_t;

typedef struct {
    int    driver_id;
    int    vehicle_id;
    int    license_class;
    int    deliveries_done;
    double rating;
    int    available;
    int    active;
} driver_t;

typedef struct {
    int    order_id;
    int    from_warehouse;
    int    to_warehouse;
    int    items;
    double weight;
    double value;
    int    status;
    int    priority;
    int    active;
} order_t;

typedef struct {
    int    shipment_id;
    int    order_id;
    int    vehicle_id;
    int    driver_id;
    double distance;
    double cost;
    int    status;
    int    active;
} shipment_t;

typedef struct {
    int    delivery_id;
    int    shipment_id;
    int    driver_id;
    int    from_loc;
    int    to_loc;
    int    status;
    int    day;
    int    active;
} delivery_t;

typedef struct {
    int    n_warehouses;
    int    n_locations;
    int    n_vehicles;
    int    n_drivers;
    int    n_orders;
    int    n_shipments;
    int    n_deliveries;
    double total_cost;
    double total_distance;
    double total_revenue;
    int    completed_deliveries;
    int    on_time_deliveries;
} log_state_t;

static warehouse_t warehouses[MAX_WAREHOUSES];
static location_t locations[MAX_LOCATIONS];
static vehicle_t vehicles[MAX_VEHICLES];
static driver_t drivers[MAX_DRIVERS];
static order_t orders[MAX_ORDERS];
static shipment_t shipments[MAX_SHIPMENTS];
static delivery_t deliveries[MAX_DELIVERIES];
static log_state_t ls;

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

int log_init(void) {
    if (initialized) return -1;
    ls.n_warehouses = 0; ls.n_locations = 0; ls.n_vehicles = 0;
    ls.n_drivers = 0; ls.n_orders = 0;
    ls.n_shipments = 0; ls.n_deliveries = 0;
    ls.total_cost = 0.0; ls.total_distance = 0.0;
    ls.total_revenue = 0.0;
    ls.completed_deliveries = 0; ls.on_time_deliveries = 0;
    for (int i = 0; i < MAX_WAREHOUSES; i++) warehouses[i].active = 0;
    for (int i = 0; i < MAX_LOCATIONS; i++) locations[i].active = 0;
    for (int i = 0; i < MAX_VEHICLES; i++) vehicles[i].active = 0;
    for (int i = 0; i < MAX_DRIVERS; i++) drivers[i].active = 0;
    for (int i = 0; i < MAX_ORDERS; i++) orders[i].active = 0;
    for (int i = 0; i < MAX_SHIPMENTS; i++) shipments[i].active = 0;
    for (int i = 0; i < MAX_DELIVERIES; i++) deliveries[i].active = 0;
    initialized = 1;
    print_str("[LOG] Logistics platform initialized\n");
    return 0;
}

int log_add_warehouse(int zone, int capacity) {
    if (ls.n_warehouses >= MAX_WAREHOUSES) return -1;
    warehouse_t* w = &warehouses[ls.n_warehouses];
    w->warehouse_id = ls.n_warehouses;
    w->zone = zone;
    w->capacity = capacity;
    w->used = 0;
    w->n_items = 0;
    w->total_value = 0.0;
    w->active = 1;
    ls.n_warehouses++;
    print_str("[LOG] Warehouse W"); print_int(ls.n_warehouses - 1);
    print_str(" zone="); print_int(zone);
    print_str(" cap="); print_int(capacity); print_str("\n");
    return ls.n_warehouses - 1;
}

int log_add_location(int warehouse_id, int type, int capacity) {
    if (ls.n_locations >= MAX_LOCATIONS || warehouse_id >= ls.n_warehouses) return -1;
    location_t* l = &locations[ls.n_locations];
    l->location_id = ls.n_locations;
    l->warehouse_id = warehouse_id;
    l->type = type;
    l->capacity = capacity;
    l->used = 0;
    l->active = 1;
    ls.n_locations++;
    print_str("[LOG] Location L"); print_int(ls.n_locations - 1);
    print_str(" W"); print_int(warehouse_id);
    print_str(" type="); print_int(type);
    print_str(" cap="); print_int(capacity); print_str("\n");
    return ls.n_locations - 1;
}

int log_add_vehicle(int type, int capacity, double fuel) {
    if (ls.n_vehicles >= MAX_VEHICLES) return -1;
    vehicle_t* v = &vehicles[ls.n_vehicles];
    v->vehicle_id = ls.n_vehicles;
    v->type = type;
    v->capacity = capacity;
    v->fuel_consumption = fuel;
    v->mileage = 0;
    v->trips = 0;
    v->status = 1;
    v->active = 1;
    ls.n_vehicles++;
    print_str("[LOG] Vehicle V"); print_int(ls.n_vehicles - 1);
    print_str(" type="); print_int(type);
    print_str(" cap="); print_int(capacity);
    print_str(" fuel="); print_int((int)(fuel * 10)); print_str("\n");
    return ls.n_vehicles - 1;
}

int log_add_driver(int vehicle_id, int license) {
    if (ls.n_drivers >= MAX_DRIVERS || vehicle_id >= ls.n_vehicles) return -1;
    driver_t* d = &drivers[ls.n_drivers];
    d->driver_id = ls.n_drivers;
    d->vehicle_id = vehicle_id;
    d->license_class = license;
    d->deliveries_done = 0;
    d->rating = 85.0;
    d->available = 1;
    d->active = 1;
    ls.n_drivers++;
    print_str("[LOG] Driver DR"); print_int(ls.n_drivers - 1);
    print_str(" V"); print_int(vehicle_id);
    print_str(" license="); print_int(license); print_str("\n");
    return ls.n_drivers - 1;
}

int log_create_order(int from_wh, int to_wh, int items, double weight, double value, int priority) {
    if (ls.n_orders >= MAX_ORDERS) return -1;
    if (from_wh >= ls.n_warehouses || to_wh >= ls.n_warehouses) return -2;
    order_t* o = &orders[ls.n_orders];
    o->order_id = ls.n_orders;
    o->from_warehouse = from_wh;
    o->to_warehouse = to_wh;
    o->items = items;
    o->weight = weight;
    o->value = value;
    o->status = 1;
    o->priority = priority;
    o->active = 1;
    warehouses[from_wh].n_items++;
    warehouses[from_wh].total_value += value;
    ls.n_orders++;
    print_str("[LOG] Order O"); print_int(ls.n_orders - 1);
    print_str(" W"); print_int(from_wh); print_str("->W"); print_int(to_wh);
    print_str(" items="); print_int(items);
    print_str(" wt="); print_int((int)weight);
    print_str(" val="); print_int((int)value);
    print_str(" pri="); print_int(priority); print_str("\n");
    return ls.n_orders - 1;
}

int log_create_shipment(int order_id, int vehicle_id, int driver_id, double distance) {
    if (ls.n_shipments >= MAX_SHIPMENTS) return -1;
    if (order_id >= ls.n_orders || vehicle_id >= ls.n_vehicles) return -2;
    if (driver_id >= ls.n_drivers) return -3;
    order_t* o = &orders[order_id];
    vehicle_t* v = &vehicles[vehicle_id];
    double cost = distance * v->fuel_consumption + o->weight * 0.5;
    shipment_t* s = &shipments[ls.n_shipments];
    s->shipment_id = ls.n_shipments;
    s->order_id = order_id;
    s->vehicle_id = vehicle_id;
    s->driver_id = driver_id;
    s->distance = distance;
    s->cost = cost;
    s->status = 1;
    s->active = 1;
    o->status = 2;
    drivers[driver_id].available = 0;
    v->trips++;
    v->mileage += (int)distance;
    ls.total_cost += cost;
    ls.total_distance += distance;
    ls.n_shipments++;
    print_str("[LOG] Shipment S"); print_int(ls.n_shipments - 1);
    print_str(" O"); print_int(order_id);
    print_str(" V"); print_int(vehicle_id);
    print_str(" DR"); print_int(driver_id);
    print_str(" dist="); print_int((int)distance);
    print_str(" cost="); print_int((int)cost); print_str("\n");
    return ls.n_shipments - 1;
}

int log_create_delivery(int shipment_id, int driver_id, int from_loc, int to_loc, int day) {
    if (ls.n_deliveries >= MAX_DELIVERIES) return -1;
    if (shipment_id >= ls.n_shipments || driver_id >= ls.n_drivers) return -2;
    delivery_t* d = &deliveries[ls.n_deliveries];
    d->delivery_id = ls.n_deliveries;
    d->shipment_id = shipment_id;
    d->driver_id = driver_id;
    d->from_loc = from_loc;
    d->to_loc = to_loc;
    d->status = 1;
    d->day = day;
    d->active = 1;
    ls.n_deliveries++;
    print_str("[LOG] Delivery D"); print_int(ls.n_deliveries - 1);
    print_str(" S"); print_int(shipment_id);
    print_str(" DR"); print_int(driver_id);
    print_str(" L"); print_int(from_loc); print_str("->L"); print_int(to_loc);
    print_str(" day="); print_int(day); print_str("\n");
    return ls.n_deliveries - 1;
}

void log_complete_delivery(int delivery_id, int on_time) {
    if (delivery_id >= ls.n_deliveries) return;
    delivery_t* d = &deliveries[delivery_id];
    d->status = 3;
    drivers[d->driver_id].deliveries_done++;
    drivers[d->driver_id].available = 1;
    int oid = shipments[d->shipment_id].order_id;
    orders[oid].status = 3;
    int to_wh = orders[oid].to_warehouse;
    warehouses[to_wh].n_items += orders[oid].items;
    warehouses[to_wh].total_value += orders[oid].value;
    ls.completed_deliveries++;
    if (on_time) {
        ls.on_time_deliveries++;
        print_str("[LOG] Delivery D"); print_int(delivery_id);
        print_str(" completed ON TIME\n");
    } else {
        print_str("[LOG] Delivery D"); print_int(delivery_id);
        print_str(" completed LATE\n");
    }
}

void log_receive_order(int order_id) {
    if (order_id >= ls.n_orders) return;
    order_t* o = &orders[order_id];
    o->status = 4;
    ls.total_revenue += o->value;
    print_str("[LOG] Order O"); print_int(order_id);
    print_str(" received val="); print_int((int)o->value); print_str("\n");
}

void log_inventory_check(void) {
    print_str("[LOG] Inventory check:\n");
    for (int i = 0; i < ls.n_warehouses; i++) {
        warehouse_t* w = &warehouses[i];
        int util = (w->capacity > 0) ? (w->used * 100) / w->capacity : 0;
        print_str("  W"); print_int(i);
        print_str(" zone="); print_int(w->zone);
        print_str(" items="); print_int(w->n_items);
        print_str(" util="); print_int(util);
        print_str("% val="); print_int((int)w->total_value); print_str("\n");
    }
}

void log_fleet_report(void) {
    print_str("[LOG] Fleet report:\n");
    print_str("  Vehicles: "); print_int(ls.n_vehicles); print_str("\n");
    print_str("  Drivers: "); print_int(ls.n_drivers); print_str("\n");
    print_str("  Total distance: "); print_int((int)ls.total_distance); print_str("km\n");
    print_str("  Total cost: "); print_int((int)ls.total_cost); print_str("\n");
    double cost_per_km = (ls.total_distance > 0) ? ls.total_cost / ls.total_distance : 0.0;
    print_str("  Cost/km: "); print_int((int)(cost_per_km * 100)); print_str("\n");
    for (int i = 0; i < ls.n_vehicles; i++) {
        vehicle_t* v = &vehicles[i];
        print_str("  V"); print_int(i);
        print_str(" type="); print_int(v->type);
        print_str(" trips="); print_int(v->trips);
        print_str(" km="); print_int(v->mileage); print_str("\n");
    }
}

void log_delivery_performance(void) {
    print_str("[LOG] Delivery performance:\n");
    double otp = (ls.completed_deliveries > 0) ?
        (double)ls.on_time_deliveries / ls.completed_deliveries * 100.0 : 0.0;
    print_str("  Completed: "); print_int(ls.completed_deliveries); print_str("\n");
    print_str("  On time: "); print_int(ls.on_time_deliveries); print_str("\n");
    print_str("  Late: "); print_int(ls.completed_deliveries - ls.on_time_deliveries); print_str("\n");
    print_str("  OTP rate: "); print_int((int)otp); print_str("%\n");
    print_str("  Total revenue: "); print_int((int)ls.total_revenue); print_str("\n");
    for (int i = 0; i < ls.n_drivers; i++) {
        driver_t* d = &drivers[i];
        print_str("  Driver "); print_int(i);
        print_str(" deliveries="); print_int(d->deliveries_done);
        print_str(" avail="); print_int(d->available); print_str("\n");
    }
}

void log_print_state(void) {
    print_str("[LOG] Warehouses="); print_int(ls.n_warehouses);
    print_str(" Vehicles="); print_int(ls.n_vehicles);
    print_str(" Drivers="); print_int(ls.n_drivers);
    print_str(" Orders="); print_int(ls.n_orders);
    print_str(" Shipments="); print_int(ls.n_shipments);
    print_str(" Deliveries="); print_int(ls.n_deliveries);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)ls.total_revenue);
    print_str(" Cost: "); print_int((int)ls.total_cost); print_str("\n");
}

int main(void) {
    print_str("=== Logistics Platform Demo ===\n\n");
    log_init();

    print_str("Adding warehouses...\n");
    log_add_warehouse(1, 500);
    log_add_warehouse(1, 300);
    log_add_warehouse(2, 400);
    log_add_warehouse(2, 250);
    log_add_warehouse(3, 600);

    print_str("\nAdding storage locations...\n");
    log_add_location(0, 1, 100);
    log_add_location(0, 2, 80);
    log_add_location(1, 1, 60);
    log_add_location(2, 1, 90);
    log_add_location(2, 2, 70);
    log_add_location(3, 1, 50);
    log_add_location(4, 1, 120);
    log_add_location(4, 2, 100);

    print_str("\nAdding vehicles...\n");
    log_add_vehicle(1, 5000, 12.0);
    log_add_vehicle(1, 3000, 10.0);
    log_add_vehicle(2, 8000, 18.0);
    log_add_vehicle(2, 6000, 15.0);
    log_add_vehicle(3, 15000, 25.0);
    log_add_vehicle(1, 4000, 11.0);

    print_str("\nAdding drivers...\n");
    log_add_driver(0, 2);
    log_add_driver(1, 1);
    log_add_driver(2, 3);
    log_add_driver(3, 2);
    log_add_driver(4, 3);
    log_add_driver(5, 1);
    log_add_driver(0, 2);
    log_add_driver(2, 3);

    print_str("\nCreating orders...\n");
    log_create_order(0, 2, 20, 500.0, 5000.0, 1);
    log_create_order(1, 3, 15, 300.0, 3500.0, 2);
    log_create_order(0, 4, 30, 800.0, 8000.0, 1);
    log_create_order(2, 1, 10, 200.0, 2000.0, 3);
    log_create_order(3, 4, 25, 600.0, 6500.0, 1);
    log_create_order(4, 0, 18, 400.0, 4000.0, 2);
    log_create_order(1, 2, 12, 250.0, 2800.0, 2);
    log_create_order(0, 3, 22, 550.0, 5500.0, 1);

    print_str("\nCreating shipments...\n");
    log_create_shipment(0, 0, 0, 150.0);
    log_create_shipment(1, 1, 1, 120.0);
    log_create_shipment(2, 2, 2, 200.0);
    log_create_shipment(3, 3, 3, 100.0);
    log_create_shipment(4, 4, 4, 250.0);
    log_create_shipment(5, 5, 5, 180.0);
    log_create_shipment(6, 0, 6, 130.0);
    log_create_shipment(7, 1, 7, 160.0);

    print_str("\nCreating deliveries...\n");
    log_create_delivery(0, 0, 0, 4, 10);
    log_create_delivery(1, 1, 2, 6, 10);
    log_create_delivery(2, 2, 0, 12, 11);
    log_create_delivery(3, 3, 5, 2, 11);
    log_create_delivery(4, 4, 9, 14, 12);
    log_create_delivery(5, 5, 13, 0, 12);
    log_create_delivery(6, 6, 2, 4, 13);
    log_create_delivery(7, 7, 1, 6, 13);

    print_str("\nCompleting deliveries...\n");
    log_complete_delivery(0, 1);
    log_complete_delivery(1, 1);
    log_complete_delivery(2, 0);
    log_complete_delivery(3, 1);
    log_complete_delivery(4, 1);
    log_complete_delivery(5, 0);
    log_complete_delivery(6, 1);

    print_str("\nReceiving orders...\n");
    log_receive_order(0);
    log_receive_order(1);
    log_receive_order(2);
    log_receive_order(3);
    log_receive_order(4);
    log_receive_order(5);
    log_receive_order(6);

    print_str("\nInventory check...\n");
    log_inventory_check();

    print_str("\nFleet report...\n");
    log_fleet_report();

    print_str("\nDelivery performance...\n");
    log_delivery_performance();

    print_str("\nFinal state...\n");
    log_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
