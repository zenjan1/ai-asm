/* food_delivery: Food delivery management (v1.0)
 * Restaurants, orders, riders, customers, zones, analytics
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

#define MAX_RESTAURANTS 10
#define MAX_ORDERS      18
#define MAX_RIDERS      10
#define MAX_CUSTOMERS   14
#define MAX_ITEMS       12

typedef struct {
    int    restaurant_id;
    int    cuisine;
    int    rating;
    int    n_orders;
    double total_revenue;
    int    zone;
    int    active;
} restaurant_t;

typedef struct {
    int    item_id;
    int    restaurant_id;
    double price;
    int    n_ordered;
    double revenue;
    int    active;
} item_t;

typedef struct {
    int    order_id;
    int    customer_id;
    int    restaurant_id;
    int    rider_id;
    double subtotal;
    double delivery_fee;
    double total;
    int    status;
    int    delivery_time;
    int    active;
} order_t;

typedef struct {
    int    rider_id;
    int    zone;
    int    n_deliveries;
    double total_earned;
    double rating;
    int    status;
    int    active;
} rider_t;

typedef struct {
    int    customer_id;
    int    zone;
    int    n_orders;
    double total_spent;
    double rating_given;
    int    active;
} customer_t;

typedef struct {
    int    n_restaurants;
    int    n_orders;
    int    n_riders;
    int    n_customers;
    int    n_items;
    double order_revenue;
    double delivery_revenue;
    double rider_payout;
    double total_revenue;
    int    total_deliveries;
    int    on_time_deliveries;
} fd_state_t;

static restaurant_t restaurants[MAX_RESTAURANTS];
static item_t items[MAX_ITEMS];
static order_t orders[MAX_ORDERS];
static rider_t riders[MAX_RIDERS];
static customer_t customers[MAX_CUSTOMERS];
static fd_state_t fd;

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

int fd_init(void) {
    if (initialized) return -1;
    fd.n_restaurants = 0; fd.n_orders = 0; fd.n_riders = 0;
    fd.n_customers = 0; fd.n_items = 0;
    fd.order_revenue = 0.0; fd.delivery_revenue = 0.0;
    fd.rider_payout = 0.0; fd.total_revenue = 0.0;
    fd.total_deliveries = 0; fd.on_time_deliveries = 0;
    for (int i = 0; i < MAX_RESTAURANTS; i++) restaurants[i].active = 0;
    for (int i = 0; i < MAX_ORDERS; i++) orders[i].active = 0;
    for (int i = 0; i < MAX_RIDERS; i++) riders[i].active = 0;
    for (int i = 0; i < MAX_CUSTOMERS; i++) customers[i].active = 0;
    for (int i = 0; i < MAX_ITEMS; i++) items[i].active = 0;
    initialized = 1;
    print_str("[FD] Food delivery initialized\n");
    return 0;
}

int fd_add_restaurant(int cuisine, int rating, int zone) {
    if (fd.n_restaurants >= MAX_RESTAURANTS) return -1;
    restaurant_t* r = &restaurants[fd.n_restaurants];
    r->restaurant_id = fd.n_restaurants;
    r->cuisine = cuisine;
    r->rating = rating;
    r->n_orders = 0;
    r->total_revenue = 0.0;
    r->zone = zone;
    r->active = 1;
    fd.n_restaurants++;
    print_str("[FD] Restaurant "); print_int(fd.n_restaurants - 1);
    print_str(" cuisine="); print_int(cuisine);
    print_str(" rating="); print_int(rating);
    print_str(" zone="); print_int(zone); print_str("\n");
    return fd.n_restaurants - 1;
}

int fd_add_item(int restaurant_id, double price) {
    if (fd.n_items >= MAX_ITEMS || restaurant_id >= fd.n_restaurants) return -1;
    item_t* it = &items[fd.n_items];
    it->item_id = fd.n_items;
    it->restaurant_id = restaurant_id;
    it->price = price;
    it->n_ordered = 0;
    it->revenue = 0.0;
    it->active = 1;
    fd.n_items++;
    print_str("[FD] Item "); print_int(fd.n_items - 1);
    print_str(" R"); print_int(restaurant_id);
    print_str(" price="); print_int((int)price); print_str("\n");
    return fd.n_items - 1;
}

int fd_register_rider(int zone, double rating) {
    if (fd.n_riders >= MAX_RIDERS) return -1;
    rider_t* r = &riders[fd.n_riders];
    r->rider_id = fd.n_riders;
    r->zone = zone;
    r->n_deliveries = 0;
    r->total_earned = 0.0;
    r->rating = rating;
    r->status = 1;
    r->active = 1;
    fd.n_riders++;
    print_str("[FD] Rider "); print_int(fd.n_riders - 1);
    print_str(" zone="); print_int(zone);
    print_str(" rating="); print_int((int)rating); print_str("\n");
    return fd.n_riders - 1;
}

int fd_register_customer(int zone) {
    if (fd.n_customers >= MAX_CUSTOMERS) return -1;
    customer_t* c = &customers[fd.n_customers];
    c->customer_id = fd.n_customers;
    c->zone = zone;
    c->n_orders = 0;
    c->total_spent = 0.0;
    c->rating_given = 0.0;
    c->active = 1;
    fd.n_customers++;
    print_str("[FD] Customer "); print_int(fd.n_customers - 1);
    print_str(" zone="); print_int(zone); print_str("\n");
    return fd.n_customers - 1;
}

int fd_place_order(int customer_id, int restaurant_id, int rider_id, int item_id, int qty, int delivery_time) {
    if (fd.n_orders >= MAX_ORDERS) return -1;
    if (customer_id >= fd.n_customers || restaurant_id >= fd.n_restaurants) return -2;
    if (rider_id >= fd.n_riders || item_id >= fd.n_items) return -3;
    item_t* it = &items[item_id];
    order_t* o = &orders[fd.n_orders];
    o->order_id = fd.n_orders;
    o->customer_id = customer_id;
    o->restaurant_id = restaurant_id;
    o->rider_id = rider_id;
    o->subtotal = it->price * qty;
    o->delivery_fee = 5.0;
    o->total = o->subtotal + o->delivery_fee;
    o->status = 3;
    o->delivery_time = delivery_time;
    o->active = 1;
    it->n_ordered += qty;
    it->revenue += o->subtotal;
    restaurants[restaurant_id].n_orders++;
    restaurants[restaurant_id].total_revenue += o->subtotal;
    customers[customer_id].n_orders++;
    customers[customer_id].total_spent += o->total;
    riders[rider_id].n_deliveries++;
    double payout = o->delivery_fee * 0.7;
    riders[rider_id].total_earned += payout;
    fd.order_revenue += o->subtotal;
    fd.delivery_revenue += o->delivery_fee;
    fd.rider_payout += payout;
    fd.total_revenue += o->total;
    fd.total_deliveries++;
    if (delivery_time <= 30) fd.on_time_deliveries++;
    fd.n_orders++;
    print_str("[FD] Order "); print_int(fd.n_orders - 1);
    print_str(" C"); print_int(customer_id);
    print_str(" R"); print_int(restaurant_id);
    print_str(" Rd"); print_int(rider_id);
    print_str(" I"); print_int(item_id);
    print_str(" qty="); print_int(qty);
    print_str(" total="); print_int((int)o->total);
    print_str(" time="); print_int(delivery_time); print_str("min\n");
    return fd.n_orders - 1;
}

void fd_restaurant_report(void) {
    print_str("[FD] Restaurant report:\n");
    for (int i = 0; i < fd.n_restaurants; i++) {
        restaurant_t* r = &restaurants[i];
        print_str("  R"); print_int(i);
        print_str(" cuisine="); print_int(r->cuisine);
        print_str(" rating="); print_int(r->rating);
        print_str(" orders="); print_int(r->n_orders);
        print_str(" rev="); print_int((int)r->total_revenue); print_str("\n");
    }
}

void fd_rider_report(void) {
    print_str("[FD] Rider report:\n");
    for (int i = 0; i < fd.n_riders; i++) {
        rider_t* r = &riders[i];
        print_str("  Rd"); print_int(i);
        print_str(" zone="); print_int(r->zone);
        print_str(" deliveries="); print_int(r->n_deliveries);
        print_str(" earned="); print_int((int)r->total_earned);
        print_str(" rating="); print_int((int)r->rating); print_str("\n");
    }
}

void fd_delivery_stats(void) {
    print_str("[FD] Delivery stats:\n");
    int total = fd.total_deliveries;
    int on_time = fd.on_time_deliveries;
    double rate = (total > 0) ? (double)on_time / total * 100.0 : 0.0;
    print_str("  Total deliveries: "); print_int(total); print_str("\n");
    print_str("  On-time: "); print_int(on_time); print_str("\n");
    print_str("  On-time rate: "); print_int((int)rate); print_str("%\n");
}

void fd_financial_report(void) {
    print_str("[FD] Financial report:\n");
    print_str("  Order revenue: "); print_int((int)fd.order_revenue); print_str("\n");
    print_str("  Delivery revenue: "); print_int((int)fd.delivery_revenue); print_str("\n");
    print_str("  Rider payout: "); print_int((int)fd.rider_payout); print_str("\n");
    print_str("  Total revenue: "); print_int((int)fd.total_revenue); print_str("\n");
    double net = fd.delivery_revenue - fd.rider_payout;
    print_str("  Net delivery profit: "); print_int((int)net); print_str("\n");
}

void fd_print_state(void) {
    print_str("[FD] Restaurants="); print_int(fd.n_restaurants);
    print_str(" Orders="); print_int(fd.n_orders);
    print_str(" Riders="); print_int(fd.n_riders);
    print_str(" Customers="); print_int(fd.n_customers);
    print_str(" Items="); print_int(fd.n_items);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)fd.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Food Delivery Demo ===\n\n");
    fd_init();

    print_str("Adding restaurants...\n");
    fd_add_restaurant(1, 88, 1);
    fd_add_restaurant(2, 92, 1);
    fd_add_restaurant(3, 85, 2);
    fd_add_restaurant(1, 90, 2);
    fd_add_restaurant(4, 87, 3);
    fd_add_restaurant(2, 91, 3);
    fd_add_restaurant(3, 86, 4);
    fd_add_restaurant(1, 93, 4);
    fd_add_restaurant(4, 89, 5);
    fd_add_restaurant(2, 94, 5);

    print_str("\nAdding menu items...\n");
    fd_add_item(0, 12.99);
    fd_add_item(1, 15.50);
    fd_add_item(2, 9.99);
    fd_add_item(3, 18.00);
    fd_add_item(4, 11.50);
    fd_add_item(5, 14.00);
    fd_add_item(6, 10.99);
    fd_add_item(7, 16.50);
    fd_add_item(8, 13.00);
    fd_add_item(9, 17.50);
    fd_add_item(0, 8.99);
    fd_add_item(1, 12.00);

    print_str("\nRegistering riders...\n");
    for (int i = 0; i < 8; i++) {
        fd_register_rider((i % 5) + 1, 80 + i * 2);
    }

    print_str("\nRegistering customers...\n");
    for (int i = 0; i < 12; i++) {
        fd_register_customer((i % 5) + 1);
    }

    print_str("\nPlacing orders...\n");
    fd_place_order(0, 0, 0, 0, 2, 25);
    fd_place_order(1, 1, 1, 1, 1, 20);
    fd_place_order(2, 2, 2, 2, 3, 35);
    fd_place_order(3, 3, 3, 3, 1, 28);
    fd_place_order(4, 4, 4, 4, 2, 22);
    fd_place_order(5, 5, 5, 5, 1, 30);
    fd_place_order(6, 6, 6, 6, 2, 18);
    fd_place_order(7, 7, 7, 7, 3, 32);
    fd_place_order(8, 8, 0, 8, 1, 26);
    fd_place_order(9, 9, 1, 9, 2, 24);
    fd_place_order(10, 0, 2, 10, 3, 38);
    fd_place_order(11, 1, 3, 11, 1, 21);
    fd_place_order(0, 2, 4, 0, 2, 29);
    fd_place_order(1, 3, 5, 1, 1, 33);
    fd_place_order(2, 4, 6, 2, 2, 19);
    fd_place_order(3, 5, 7, 3, 3, 27);
    fd_place_order(4, 6, 0, 4, 1, 31);
    fd_place_order(5, 7, 1, 5, 2, 23);

    print_str("\nRestaurant report...\n");
    fd_restaurant_report();

    print_str("\nRider report...\n");
    fd_rider_report();

    print_str("\nDelivery stats...\n");
    fd_delivery_stats();

    print_str("\nFinancial report...\n");
    fd_financial_report();

    print_str("\nFinal state...\n");
    fd_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
