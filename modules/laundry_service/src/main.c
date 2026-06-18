/* laundry_service: Laundry service management (v1.0)
 * Garments, customers, orders, services, equipment, delivery
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

#define MAX_GARMENTS   16
#define MAX_CUSTOMERS  12
#define MAX_ORDERS     18
#define MAX_EQUIPMENT  8
#define MAX_SERVICES   6

typedef struct {
    int    garment_id;
    int    type;
    double base_price;
    int    n_cleaned;
    double revenue;
    int    active;
} garment_t;

typedef struct {
    int    customer_id;
    int    tier;
    int    n_orders;
    double total_spent;
    int    points;
    int    active;
} customer_t;

typedef struct {
    int    order_id;
    int    customer_id;
    int    garment_id;
    int    service_id;
    int    qty;
    double price;
    int    rush;
    int    status;
    int    day;
    int    active;
} order_t;

typedef struct {
    int    equip_id;
    int    type;
    int    capacity;
    int    status;
    int    n_cycles;
    int    active;
} equipment_t;

typedef struct {
    int    service_id;
    int    type;
    double multiplier;
    int    n_used;
    double revenue;
    int    active;
} service_t;

typedef struct {
    int    n_garments;
    int    n_customers;
    int    n_orders;
    int    n_equipment;
    int    n_services;
    double order_revenue;
    double rush_revenue;
    double delivery_revenue;
    double total_revenue;
    int    total_items;
} ls_state_t;

static garment_t garments[MAX_GARMENTS];
static customer_t customers[MAX_CUSTOMERS];
static order_t orders[MAX_ORDERS];
static equipment_t equipment[MAX_EQUIPMENT];
static service_t services[MAX_SERVICES];
static ls_state_t ls;

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

int ls_init(void) {
    if (initialized) return -1;
    ls.n_garments = 0; ls.n_customers = 0; ls.n_orders = 0;
    ls.n_equipment = 0; ls.n_services = 0;
    ls.order_revenue = 0.0; ls.rush_revenue = 0.0;
    ls.delivery_revenue = 0.0; ls.total_revenue = 0.0;
    ls.total_items = 0;
    for (int i = 0; i < MAX_GARMENTS; i++) garments[i].active = 0;
    for (int i = 0; i < MAX_CUSTOMERS; i++) customers[i].active = 0;
    for (int i = 0; i < MAX_ORDERS; i++) orders[i].active = 0;
    for (int i = 0; i < MAX_EQUIPMENT; i++) equipment[i].active = 0;
    for (int i = 0; i < MAX_SERVICES; i++) services[i].active = 0;
    initialized = 1;
    print_str("[LS] Laundry service initialized\n");
    return 0;
}

int ls_add_garment(int type, double price) {
    if (ls.n_garments >= MAX_GARMENTS) return -1;
    garment_t* g = &garments[ls.n_garments];
    g->garment_id = ls.n_garments;
    g->type = type;
    g->base_price = price;
    g->n_cleaned = 0;
    g->revenue = 0.0;
    g->active = 1;
    ls.n_garments++;
    print_str("[LS] Garment "); print_int(ls.n_garments - 1);
    print_str(" type="); print_int(type);
    print_str(" price="); print_int((int)price); print_str("\n");
    return ls.n_garments - 1;
}

int ls_register_customer(int tier) {
    if (ls.n_customers >= MAX_CUSTOMERS) return -1;
    customer_t* c = &customers[ls.n_customers];
    c->customer_id = ls.n_customers;
    c->tier = tier;
    c->n_orders = 0;
    c->total_spent = 0.0;
    c->points = 0;
    c->active = 1;
    ls.n_customers++;
    print_str("[LS] Customer "); print_int(ls.n_customers - 1);
    print_str(" tier="); print_int(tier); print_str("\n");
    return ls.n_customers - 1;
}

int ls_add_equipment(int type, int capacity) {
    if (ls.n_equipment >= MAX_EQUIPMENT) return -1;
    equipment_t* e = &equipment[ls.n_equipment];
    e->equip_id = ls.n_equipment;
    e->type = type;
    e->capacity = capacity;
    e->status = 1;
    e->n_cycles = 0;
    e->active = 1;
    ls.n_equipment++;
    print_str("[LS] Equipment "); print_int(ls.n_equipment - 1);
    print_str(" type="); print_int(type);
    print_str(" cap="); print_int(capacity); print_str("\n");
    return ls.n_equipment - 1;
}

int ls_add_service(int type, double multiplier) {
    if (ls.n_services >= MAX_SERVICES) return -1;
    service_t* s = &services[ls.n_services];
    s->service_id = ls.n_services;
    s->type = type;
    s->multiplier = multiplier;
    s->n_used = 0;
    s->revenue = 0.0;
    s->active = 1;
    ls.n_services++;
    print_str("[LS] Service "); print_int(ls.n_services - 1);
    print_str(" type="); print_int(type);
    print_str(" mult="); print_int((int)(multiplier * 100)); print_str("%\n");
    return ls.n_services - 1;
}

int ls_create_order(int customer_id, int garment_id, int service_id, int qty, int rush, int day) {
    if (ls.n_orders >= MAX_ORDERS) return -1;
    if (customer_id >= ls.n_customers || garment_id >= ls.n_garments) return -2;
    if (service_id >= ls.n_services) return -3;
    garment_t* g = &garments[garment_id];
    service_t* s = &services[service_id];
    order_t* o = &orders[ls.n_orders];
    o->order_id = ls.n_orders;
    o->customer_id = customer_id;
    o->garment_id = garment_id;
    o->service_id = service_id;
    o->qty = qty;
    double tier_discount[4] = {1.0, 0.95, 0.90, 0.85};
    double disc = tier_discount[customers[customer_id].tier < 4 ? customers[customer_id].tier : 0];
    o->price = g->base_price * s->multiplier * qty * disc;
    o->rush = rush;
    o->status = 3;
    o->day = day;
    o->active = 1;
    g->n_cleaned += qty;
    g->revenue += o->price;
    s->n_used++;
    s->revenue += o->price;
    customers[customer_id].n_orders++;
    customers[customer_id].total_spent += o->price;
    customers[customer_id].points += (int)o->price;
    double delivery_fee = 5.0;
    double rush_fee = rush ? 10.0 : 0.0;
    ls.order_revenue += o->price;
    ls.rush_revenue += rush_fee;
    ls.delivery_revenue += delivery_fee;
    ls.total_revenue += o->price + rush_fee + delivery_fee;
    ls.total_items += qty;
    ls.n_orders++;
    print_str("[LS] Order "); print_int(ls.n_orders - 1);
    print_str(" C"); print_int(customer_id);
    print_str(" G"); print_int(garment_id);
    print_str(" S"); print_int(service_id);
    print_str(" qty="); print_int(qty);
    print_str(" total="); print_int((int)o->price);
    if (rush) print_str(" [RUSH]");
    print_str("\n");
    return ls.n_orders - 1;
}

void ls_garment_report(void) {
    print_str("[LS] Garment report:\n");
    for (int i = 0; i < ls.n_garments; i++) {
        garment_t* g = &garments[i];
        print_str("  G"); print_int(i);
        print_str(" type="); print_int(g->type);
        print_str(" cleaned="); print_int(g->n_cleaned);
        print_str(" rev="); print_int((int)g->revenue); print_str("\n");
    }
}

void ls_service_report(void) {
    print_str("[LS] Service report:\n");
    for (int i = 0; i < ls.n_services; i++) {
        service_t* s = &services[i];
        print_str("  S"); print_int(i);
        print_str(" type="); print_int(s->type);
        print_str(" used="); print_int(s->n_used);
        print_str(" rev="); print_int((int)s->revenue); print_str("\n");
    }
}

void ls_financial_report(void) {
    print_str("[LS] Financial report:\n");
    print_str("  Order revenue: "); print_int((int)ls.order_revenue); print_str("\n");
    print_str("  Rush revenue: "); print_int((int)ls.rush_revenue); print_str("\n");
    print_str("  Delivery revenue: "); print_int((int)ls.delivery_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)ls.total_revenue); print_str("\n");
    print_str("  Total items: "); print_int(ls.total_items); print_str("\n");
    print_str("  Customers: "); print_int(ls.n_customers); print_str("\n");
}

void ls_print_state(void) {
    print_str("[LS] Garments="); print_int(ls.n_garments);
    print_str(" Orders="); print_int(ls.n_orders);
    print_str(" Customers="); print_int(ls.n_customers);
    print_str(" Equipment="); print_int(ls.n_equipment);
    print_str(" Services="); print_int(ls.n_services);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)ls.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Laundry Service Demo ===\n\n");
    ls_init();

    print_str("Adding garments...\n");
    ls_add_garment(1, 8.0);
    ls_add_garment(1, 12.0);
    ls_add_garment(2, 15.0);
    ls_add_garment(2, 20.0);
    ls_add_garment(3, 25.0);
    ls_add_garment(3, 35.0);
    ls_add_garment(4, 10.0);
    ls_add_garment(4, 18.0);
    ls_add_garment(1, 6.0);
    ls_add_garment(2, 22.0);
    ls_add_garment(3, 30.0);
    ls_add_garment(4, 14.0);
    ls_add_garment(1, 9.0);
    ls_add_garment(2, 16.0);
    ls_add_garment(3, 28.0);
    ls_add_garment(4, 12.0);

    print_str("\nRegistering customers...\n");
    for (int i = 0; i < 10; i++) {
        ls_register_customer(i % 4);
    }

    print_str("\nAdding equipment...\n");
    ls_add_equipment(1, 20);
    ls_add_equipment(1, 25);
    ls_add_equipment(2, 15);
    ls_add_equipment(2, 18);
    ls_add_equipment(3, 10);
    ls_add_equipment(3, 12);
    ls_add_equipment(4, 8);
    ls_add_equipment(4, 10);

    print_str("\nAdding services...\n");
    ls_add_service(1, 1.0);
    ls_add_service(2, 1.3);
    ls_add_service(3, 1.5);
    ls_add_service(4, 0.8);
    ls_add_service(5, 1.8);
    ls_add_service(6, 2.0);

    print_str("\nCreating orders...\n");
    ls_create_order(0, 0, 0, 5, 0, 10);
    ls_create_order(1, 1, 1, 3, 0, 10);
    ls_create_order(2, 2, 2, 2, 1, 11);
    ls_create_order(3, 3, 3, 4, 0, 11);
    ls_create_order(4, 4, 4, 1, 1, 12);
    ls_create_order(5, 5, 5, 3, 0, 12);
    ls_create_order(6, 6, 0, 6, 0, 13);
    ls_create_order(7, 7, 1, 2, 1, 13);
    ls_create_order(8, 8, 2, 4, 0, 14);
    ls_create_order(9, 9, 3, 3, 0, 14);
    ls_create_order(0, 10, 4, 2, 1, 15);
    ls_create_order(1, 11, 5, 1, 0, 15);
    ls_create_order(2, 12, 0, 5, 0, 16);
    ls_create_order(3, 13, 1, 3, 1, 16);
    ls_create_order(4, 14, 2, 2, 0, 17);
    ls_create_order(5, 15, 3, 4, 0, 17);
    ls_create_order(6, 0, 4, 3, 1, 18);
    ls_create_order(7, 1, 5, 2, 0, 18);

    print_str("\nGarment report...\n");
    ls_garment_report();

    print_str("\nService report...\n");
    ls_service_report();

    print_str("\nFinancial report...\n");
    ls_financial_report();

    print_str("\nFinal state...\n");
    ls_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
