/* beauty_salon: Beauty salon management (v1.0)
 * Clients, services, appointments, inventory, analytics
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

#define MAX_CLIENTS    14
#define MAX_STYLISTS   8
#define MAX_SERVICES   12
#define MAX_APPOINTMENTS 16
#define MAX_PRODUCTS   10

typedef struct {
    int    client_id;
    int    tier;
    int    n_visits;
    double total_spent;
    int    last_visit_day;
    int    preference;
    int    active;
} client_t;

typedef struct {
    int    stylist_id;
    int    specialty;
    int    rating;
    int    n_appointments;
    double revenue;
    int    active;
} stylist_t;

typedef struct {
    int    service_id;
    int    category;
    double price;
    double cost;
    int    duration_min;
    int    n_performed;
    double revenue;
    int    active;
} service_t;

typedef struct {
    int    appt_id;
    int    client_id;
    int    stylist_id;
    int    service_id;
    int    day;
    int    slot;
    int    status;
    int    active;
} appointment_t;

typedef struct {
    int    product_id;
    int    type;
    int    stock;
    double unit_cost;
    double price;
    int    n_sold;
    double revenue;
    int    active;
} product_t;

typedef struct {
    int    n_clients;
    int    n_stylists;
    int    n_services;
    int    n_appts;
    int    n_products;
    double total_revenue;
    double service_revenue;
    double product_revenue;
    double total_costs;
    int    total_appointments;
    int    completed_appts;
} bs_state_t;

static client_t clients[MAX_CLIENTS];
static stylist_t stylists[MAX_STYLISTS];
static service_t services[MAX_SERVICES];
static appointment_t appointments[MAX_APPOINTMENTS];
static product_t products[MAX_PRODUCTS];
static bs_state_t bs;

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

int bs_init(void) {
    if (initialized) return -1;
    bs.n_clients = 0; bs.n_stylists = 0; bs.n_services = 0;
    bs.n_appts = 0; bs.n_products = 0;
    bs.total_revenue = 0.0; bs.service_revenue = 0.0;
    bs.product_revenue = 0.0; bs.total_costs = 0.0;
    bs.total_appointments = 0; bs.completed_appts = 0;
    for (int i = 0; i < MAX_CLIENTS; i++) clients[i].active = 0;
    for (int i = 0; i < MAX_STYLISTS; i++) stylists[i].active = 0;
    for (int i = 0; i < MAX_SERVICES; i++) services[i].active = 0;
    for (int i = 0; i < MAX_APPOINTMENTS; i++) appointments[i].active = 0;
    for (int i = 0; i < MAX_PRODUCTS; i++) products[i].active = 0;
    initialized = 1;
    print_str("[BS] Beauty salon initialized\n");
    return 0;
}

int bs_add_stylist(int specialty, int rating) {
    if (bs.n_stylists >= MAX_STYLISTS) return -1;
    stylist_t* s = &stylists[bs.n_stylists];
    s->stylist_id = bs.n_stylists;
    s->specialty = specialty;
    s->rating = rating;
    s->n_appointments = 0;
    s->revenue = 0.0;
    s->active = 1;
    bs.n_stylists++;
    print_str("[BS] Stylist "); print_int(bs.n_stylists - 1);
    print_str(" specialty="); print_int(specialty);
    print_str(" rating="); print_int(rating); print_str("\n");
    return bs.n_stylists - 1;
}

int bs_register_client(int tier, int preference) {
    if (bs.n_clients >= MAX_CLIENTS) return -1;
    client_t* c = &clients[bs.n_clients];
    c->client_id = bs.n_clients;
    c->tier = tier;
    c->n_visits = 0;
    c->total_spent = 0.0;
    c->last_visit_day = 0;
    c->preference = preference;
    c->active = 1;
    bs.n_clients++;
    print_str("[BS] Client "); print_int(bs.n_clients - 1);
    print_str(" tier="); print_int(tier);
    print_str(" pref="); print_int(preference); print_str("\n");
    return bs.n_clients - 1;
}

int bs_add_service(int category, double price, double cost, int duration) {
    if (bs.n_services >= MAX_SERVICES) return -1;
    service_t* s = &services[bs.n_services];
    s->service_id = bs.n_services;
    s->category = category;
    s->price = price;
    s->cost = cost;
    s->duration_min = duration;
    s->n_performed = 0;
    s->revenue = 0.0;
    s->active = 1;
    bs.n_services++;
    print_str("[BS] Service "); print_int(bs.n_services - 1);
    print_str(" cat="); print_int(category);
    print_str(" price="); print_int((int)price);
    print_str(" dur="); print_int(duration); print_str("min\n");
    return bs.n_services - 1;
}

int bs_add_product(int type, int stock, double cost, double price) {
    if (bs.n_products >= MAX_PRODUCTS) return -1;
    product_t* p = &products[bs.n_products];
    p->product_id = bs.n_products;
    p->type = type;
    p->stock = stock;
    p->unit_cost = cost;
    p->price = price;
    p->n_sold = 0;
    p->revenue = 0.0;
    p->active = 1;
    bs.n_products++;
    print_str("[BS] Product "); print_int(bs.n_products - 1);
    print_str(" type="); print_int(type);
    print_str(" stock="); print_int(stock);
    print_str(" price="); print_int((int)price); print_str("\n");
    return bs.n_products - 1;
}

int bs_make_appointment(int client_id, int stylist_id, int service_id, int day, int slot) {
    if (bs.n_appts >= MAX_APPOINTMENTS) return -1;
    if (client_id >= bs.n_clients || stylist_id >= bs.n_stylists) return -2;
    if (service_id >= bs.n_services) return -3;
    appointment_t* a = &appointments[bs.n_appts];
    a->appt_id = bs.n_appts;
    a->client_id = client_id;
    a->stylist_id = stylist_id;
    a->service_id = service_id;
    a->day = day;
    a->slot = slot;
    a->status = 1;
    a->active = 1;
    bs.n_appts++;
    bs.total_appointments++;
    stylists[stylist_id].n_appointments++;
    print_str("[BS] Appt "); print_int(bs.n_appts - 1);
    print_str(" C"); print_int(client_id);
    print_str(" S"); print_int(stylist_id);
    print_str(" SV"); print_int(service_id);
    print_str(" day="); print_int(day);
    print_str(" slot="); print_int(slot); print_str("\n");
    return bs.n_appts - 1;
}

void bs_complete_appointment(int appt_id) {
    if (appt_id >= bs.n_appts) return;
    appointment_t* a = &appointments[appt_id];
    a->status = 3;
    service_t* sv = &services[a->service_id];
    double discount = (clients[a->client_id].tier >= 2) ? 0.9 : 1.0;
    double price = sv->price * discount;
    sv->n_performed++;
    sv->revenue += price;
    clients[a->client_id].n_visits++;
    clients[a->client_id].total_spent += price;
    clients[a->client_id].last_visit_day = a->day;
    stylists[a->stylist_id].revenue += price;
    bs.service_revenue += price;
    bs.total_revenue += price;
    bs.total_costs += sv->cost;
    bs.completed_appts++;
    print_str("[BS] Appt "); print_int(appt_id);
    print_str(" completed C"); print_int(a->client_id);
    print_str(" S"); print_int(a->stylist_id);
    print_str(" price="); print_int((int)price); print_str("\n");
}

int bs_sell_product(int product_id, int client_id) {
    if (product_id >= bs.n_products || client_id >= bs.n_clients) return -1;
    product_t* p = &products[product_id];
    if (p->stock <= 0) return -2;
    p->stock--;
    p->n_sold++;
    p->revenue += p->price;
    clients[client_id].total_spent += p->price;
    bs.product_revenue += p->price;
    bs.total_revenue += p->price;
    bs.total_costs += p->unit_cost;
    print_str("[BS] Sold P"); print_int(product_id);
    print_str(" to C"); print_int(client_id);
    print_str(" price="); print_int((int)p->price);
    print_str(" stock="); print_int(p->stock); print_str("\n");
    return 0;
}

int bs_restock_product(int product_id, int qty) {
    if (product_id >= bs.n_products) return -1;
    product_t* p = &products[product_id];
    p->stock += qty;
    bs.total_costs += p->unit_cost * qty;
    print_str("[BS] Restocked P"); print_int(product_id);
    print_str(" qty="); print_int(qty);
    print_str(" stock="); print_int(p->stock); print_str("\n");
    return 0;
}

void bs_stylist_performance(void) {
    print_str("[BS] Stylist performance:\n");
    for (int i = 0; i < bs.n_stylists; i++) {
        stylist_t* s = &stylists[i];
        print_str("  S"); print_int(i);
        print_str(" specialty="); print_int(s->specialty);
        print_str(" rating="); print_int(s->rating);
        print_str(" appts="); print_int(s->n_appointments);
        print_str(" rev="); print_int((int)s->revenue); print_str("\n");
    }
}

void bs_service_ranking(void) {
    print_str("[BS] Service ranking:\n");
    for (int i = 0; i < bs.n_services; i++) {
        service_t* s = &services[i];
        print_str("  SV"); print_int(i);
        print_str(" cat="); print_int(s->category);
        print_str(" performed="); print_int(s->n_performed);
        print_str(" rev="); print_int((int)s->revenue); print_str("\n");
    }
}

void bs_financial_report(void) {
    print_str("[BS] Financial report:\n");
    print_str("  Service revenue: "); print_int((int)bs.service_revenue); print_str("\n");
    print_str("  Product revenue: "); print_int((int)bs.product_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)bs.total_revenue); print_str("\n");
    print_str("  Total costs: "); print_int((int)bs.total_costs); print_str("\n");
    double profit = bs.total_revenue - bs.total_costs;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    double margin = (bs.total_revenue > 0) ? profit / bs.total_revenue * 100.0 : 0.0;
    print_str("  Margin: "); print_int((int)margin); print_str("%\n");
    print_str("  Completed appts: "); print_int(bs.completed_appts); print_str("\n");
}

void bs_print_state(void) {
    print_str("[BS] Clients="); print_int(bs.n_clients);
    print_str(" Stylists="); print_int(bs.n_stylists);
    print_str(" Services="); print_int(bs.n_services);
    print_str(" Appts="); print_int(bs.n_appts);
    print_str(" Products="); print_int(bs.n_products);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)bs.total_revenue);
    print_str(" Profit: "); print_int((int)(bs.total_revenue - bs.total_costs)); print_str("\n");
}

int main(void) {
    print_str("=== Beauty Salon Demo ===\n\n");
    bs_init();

    print_str("Adding stylists...\n");
    bs_add_stylist(1, 92);
    bs_add_stylist(2, 88);
    bs_add_stylist(3, 90);
    bs_add_stylist(1, 85);
    bs_add_stylist(2, 95);
    bs_add_stylist(3, 87);

    print_str("\nRegistering clients...\n");
    bs_register_client(0, 1);
    bs_register_client(1, 2);
    bs_register_client(2, 3);
    bs_register_client(0, 1);
    bs_register_client(1, 3);
    bs_register_client(2, 2);
    bs_register_client(3, 1);
    bs_register_client(0, 3);
    bs_register_client(1, 2);
    bs_register_client(2, 1);

    print_str("\nAdding services...\n");
    bs_add_service(1, 45.0, 12.0, 60);
    bs_add_service(1, 35.0, 8.0, 45);
    bs_add_service(2, 80.0, 25.0, 90);
    bs_add_service(2, 60.0, 18.0, 60);
    bs_add_service(3, 25.0, 5.0, 30);
    bs_add_service(3, 40.0, 10.0, 45);
    bs_add_service(4, 120.0, 40.0, 120);
    bs_add_service(1, 55.0, 15.0, 75);
    bs_add_service(2, 90.0, 30.0, 90);

    print_str("\nAdding products...\n");
    bs_add_product(1, 20, 8.0, 18.0);
    bs_add_product(2, 15, 12.0, 28.0);
    bs_add_product(3, 25, 5.0, 12.0);
    bs_add_product(1, 18, 10.0, 22.0);
    bs_add_product(4, 10, 15.0, 35.0);
    bs_add_product(2, 22, 8.0, 20.0);

    print_str("\nMaking appointments...\n");
    bs_make_appointment(0, 0, 0, 10, 1);
    bs_make_appointment(0, 0, 4, 10, 2);
    bs_make_appointment(1, 1, 1, 10, 1);
    bs_make_appointment(1, 1, 2, 10, 2);
    bs_make_appointment(2, 2, 6, 11, 1);
    bs_make_appointment(2, 2, 3, 11, 2);
    bs_make_appointment(3, 3, 0, 11, 1);
    bs_make_appointment(3, 3, 5, 11, 2);
    bs_make_appointment(4, 4, 2, 12, 1);
    bs_make_appointment(4, 4, 1, 12, 2);
    bs_make_appointment(5, 5, 3, 12, 1);
    bs_make_appointment(5, 5, 7, 12, 2);
    bs_make_appointment(6, 0, 8, 13, 1);
    bs_make_appointment(7, 1, 0, 13, 1);
    bs_make_appointment(8, 2, 4, 13, 2);
    bs_make_appointment(9, 3, 1, 14, 1);

    print_str("\nCompleting appointments...\n");
    bs_complete_appointment(0);
    bs_complete_appointment(1);
    bs_complete_appointment(2);
    bs_complete_appointment(3);
    bs_complete_appointment(4);
    bs_complete_appointment(5);
    bs_complete_appointment(6);
    bs_complete_appointment(7);
    bs_complete_appointment(8);
    bs_complete_appointment(9);
    bs_complete_appointment(10);
    bs_complete_appointment(11);
    bs_complete_appointment(12);
    bs_complete_appointment(13);
    bs_complete_appointment(14);
    bs_complete_appointment(15);

    print_str("\nSelling products...\n");
    bs_sell_product(0, 0);
    bs_sell_product(1, 2);
    bs_sell_product(2, 4);
    bs_sell_product(3, 6);
    bs_sell_product(4, 7);
    bs_sell_product(5, 9);
    bs_sell_product(0, 1);
    bs_sell_product(1, 3);
    bs_sell_product(2, 5);

    print_str("\nRestocking...\n");
    bs_restock_product(0, 10);
    bs_restock_product(1, 8);
    bs_restock_product(2, 12);
    bs_restock_product(4, 5);

    print_str("\nStylist performance...\n");
    bs_stylist_performance();

    print_str("\nService ranking...\n");
    bs_service_ranking();

    print_str("\nFinancial report...\n");
    bs_financial_report();

    print_str("\nFinal state...\n");
    bs_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
