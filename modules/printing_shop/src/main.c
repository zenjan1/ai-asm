/* printing_shop: Print shop management (v1.0)
 * Services, materials, orders, designers, equipment, customers
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

#define MAX_SERVICES   10
#define MAX_MATERIALS  8
#define MAX_ORDERS     18
#define MAX_DESIGNERS  6
#define MAX_EQUIPMENT  6
#define MAX_CUSTOMERS  12

typedef struct {
    int    service_id;
    int    type;
    double price_per_unit;
    double cost_per_unit;
    int    n_printed;
    double revenue;
    int    active;
} service_t;

typedef struct {
    int    material_id;
    int    type;
    double cost_per_sheet;
    int    stock;
    int    n_used;
    double total_cost;
    int    active;
} material_t;

typedef struct {
    int    order_id;
    int    customer_id;
    int    service_id;
    int    material_id;
    int    qty;
    int    designer_id;
    double print_cost;
    double design_fee;
    double total;
    int    rush;
    int    status;
    int    day;
    int    active;
} order_t;

typedef struct {
    int    designer_id;
    int    skill;
    int    n_jobs;
    double total_earned;
    int    rating;
    int    active;
} designer_t;

typedef struct {
    int    equip_id;
    int    type;
    int    status;
    int    n_jobs;
    int    active;
} equipment_t;

typedef struct {
    int    customer_id;
    int    type;
    int    n_orders;
    double total_spent;
    int    active;
} customer_t;

typedef struct {
    int    n_services;
    int    n_materials;
    int    n_orders;
    int    n_designers;
    int    n_equipment;
    int    n_customers;
    double print_revenue;
    double design_revenue;
    double rush_revenue;
    double total_revenue;
    double material_cost;
} ps_state_t;

static service_t services[MAX_SERVICES];
static material_t materials[MAX_MATERIALS];
static order_t orders[MAX_ORDERS];
static designer_t designers[MAX_DESIGNERS];
static equipment_t equipment[MAX_EQUIPMENT];
static customer_t customers[MAX_CUSTOMERS];
static ps_state_t ps;

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

int ps_init(void) {
    if (initialized) return -1;
    ps.n_services = 0; ps.n_materials = 0; ps.n_orders = 0;
    ps.n_designers = 0; ps.n_equipment = 0; ps.n_customers = 0;
    ps.print_revenue = 0.0; ps.design_revenue = 0.0;
    ps.rush_revenue = 0.0; ps.total_revenue = 0.0;
    ps.material_cost = 0.0;
    for (int i = 0; i < MAX_SERVICES; i++) services[i].active = 0;
    for (int i = 0; i < MAX_MATERIALS; i++) materials[i].active = 0;
    for (int i = 0; i < MAX_ORDERS; i++) orders[i].active = 0;
    for (int i = 0; i < MAX_DESIGNERS; i++) designers[i].active = 0;
    for (int i = 0; i < MAX_EQUIPMENT; i++) equipment[i].active = 0;
    for (int i = 0; i < MAX_CUSTOMERS; i++) customers[i].active = 0;
    initialized = 1;
    print_str("[PS] Printing shop initialized\n");
    return 0;
}

int ps_add_service(int type, double price, double cost) {
    if (ps.n_services >= MAX_SERVICES) return -1;
    service_t* s = &services[ps.n_services];
    s->service_id = ps.n_services;
    s->type = type;
    s->price_per_unit = price;
    s->cost_per_unit = cost;
    s->n_printed = 0;
    s->revenue = 0.0;
    s->active = 1;
    ps.n_services++;
    print_str("[PS] Service "); print_int(ps.n_services - 1);
    print_str(" type="); print_int(type);
    print_str(" price="); print_int((int)price); print_str("/unit\n");
    return ps.n_services - 1;
}

int ps_add_material(int type, double cost, int stock) {
    if (ps.n_materials >= MAX_MATERIALS) return -1;
    material_t* m = &materials[ps.n_materials];
    m->material_id = ps.n_materials;
    m->type = type;
    m->cost_per_sheet = cost;
    m->stock = stock;
    m->n_used = 0;
    m->total_cost = 0.0;
    m->active = 1;
    ps.n_materials++;
    print_str("[PS] Material "); print_int(ps.n_materials - 1);
    print_str(" type="); print_int(type);
    print_str(" cost="); print_int((int)cost);
    print_str(" stock="); print_int(stock); print_str("\n");
    return ps.n_materials - 1;
}

int ps_add_designer(int skill, int rating) {
    if (ps.n_designers >= MAX_DESIGNERS) return -1;
    designer_t* d = &designers[ps.n_designers];
    d->designer_id = ps.n_designers;
    d->skill = skill;
    d->n_jobs = 0;
    d->total_earned = 0.0;
    d->rating = rating;
    d->active = 1;
    ps.n_designers++;
    print_str("[PS] Designer "); print_int(ps.n_designers - 1);
    print_str(" skill="); print_int(skill);
    print_str(" rating="); print_int(rating); print_str("\n");
    return ps.n_designers - 1;
}

int ps_add_equipment(int type) {
    if (ps.n_equipment >= MAX_EQUIPMENT) return -1;
    equipment_t* e = &equipment[ps.n_equipment];
    e->equip_id = ps.n_equipment;
    e->type = type;
    e->status = 1;
    e->n_jobs = 0;
    e->active = 1;
    ps.n_equipment++;
    print_str("[PS] Equipment "); print_int(ps.n_equipment - 1);
    print_str(" type="); print_int(type); print_str("\n");
    return ps.n_equipment - 1;
}

int ps_register_customer(int type) {
    if (ps.n_customers >= MAX_CUSTOMERS) return -1;
    customer_t* c = &customers[ps.n_customers];
    c->customer_id = ps.n_customers;
    c->type = type;
    c->n_orders = 0;
    c->total_spent = 0.0;
    c->active = 1;
    ps.n_customers++;
    print_str("[PS] Customer "); print_int(ps.n_customers - 1);
    print_str(" type="); print_int(type); print_str("\n");
    return ps.n_customers - 1;
}

int ps_create_order(int customer_id, int service_id, int material_id, int qty, int designer_id, int rush, int day) {
    if (ps.n_orders >= MAX_ORDERS) return -1;
    if (customer_id >= ps.n_customers || service_id >= ps.n_services) return -2;
    service_t* s = &services[service_id];
    order_t* o = &orders[ps.n_orders];
    o->order_id = ps.n_orders;
    o->customer_id = customer_id;
    o->service_id = service_id;
    o->material_id = material_id;
    o->qty = qty;
    o->designer_id = designer_id;
    o->print_cost = s->price_per_unit * qty;
    o->design_fee = 0.0;
    if (designer_id >= 0 && designer_id < ps.n_designers) {
        double rates[3] = {30.0, 50.0, 80.0};
        int skill = designers[designer_id].skill;
        o->design_fee = rates[skill < 3 ? skill : 0];
        designers[designer_id].n_jobs++;
        designers[designer_id].total_earned += o->design_fee * 0.5;
        ps.design_revenue += o->design_fee;
    }
    if (material_id >= 0 && material_id < ps.n_materials) {
        material_t* m = &materials[material_id];
        if (m->stock >= qty) {
            m->stock -= qty;
            m->n_used += qty;
            double cost = m->cost_per_sheet * qty;
            m->total_cost += cost;
            ps.material_cost += cost;
        }
    }
    double rush_fee = rush ? o->print_cost * 0.25 : 0.0;
    o->rush = rush;
    o->total = o->print_cost + o->design_fee + rush_fee;
    o->status = 3;
    o->day = day;
    o->active = 1;
    s->n_printed += qty;
    s->revenue += o->print_cost;
    customers[customer_id].n_orders++;
    customers[customer_id].total_spent += o->total;
    ps.print_revenue += o->print_cost;
    ps.rush_revenue += rush_fee;
    ps.total_revenue += o->total;
    ps.n_orders++;
    print_str("[PS] Order "); print_int(ps.n_orders - 1);
    print_str(" C"); print_int(customer_id);
    print_str(" S"); print_int(service_id);
    print_str(" qty="); print_int(qty);
    print_str(" total="); print_int((int)o->total);
    if (rush) print_str(" [RUSH]");
    print_str("\n");
    return ps.n_orders - 1;
}

void ps_service_report(void) {
    print_str("[PS] Service report:\n");
    for (int i = 0; i < ps.n_services; i++) {
        service_t* s = &services[i];
        print_str("  S"); print_int(i);
        print_str(" type="); print_int(s->type);
        print_str(" printed="); print_int(s->n_printed);
        print_str(" rev="); print_int((int)s->revenue); print_str("\n");
    }
}

void ps_designer_report(void) {
    print_str("[PS] Designer report:\n");
    for (int i = 0; i < ps.n_designers; i++) {
        designer_t* d = &designers[i];
        print_str("  D"); print_int(i);
        print_str(" skill="); print_int(d->skill);
        print_str(" jobs="); print_int(d->n_jobs);
        print_str(" earned="); print_int((int)d->total_earned);
        print_str(" rating="); print_int(d->rating); print_str("\n");
    }
}

void ps_financial_report(void) {
    print_str("[PS] Financial report:\n");
    print_str("  Print revenue: "); print_int((int)ps.print_revenue); print_str("\n");
    print_str("  Design revenue: "); print_int((int)ps.design_revenue); print_str("\n");
    print_str("  Rush revenue: "); print_int((int)ps.rush_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)ps.total_revenue); print_str("\n");
    print_str("  Material cost: "); print_int((int)ps.material_cost); print_str("\n");
    double profit = ps.total_revenue - ps.material_cost;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
}

void ps_print_state(void) {
    print_str("[PS] Services="); print_int(ps.n_services);
    print_str(" Materials="); print_int(ps.n_materials);
    print_str(" Orders="); print_int(ps.n_orders);
    print_str(" Designers="); print_int(ps.n_designers);
    print_str(" Equipment="); print_int(ps.n_equipment);
    print_str(" Customers="); print_int(ps.n_customers);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)ps.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Printing Shop Demo ===\n\n");
    ps_init();

    print_str("Adding services...\n");
    ps_add_service(1, 0.15, 0.05);
    ps_add_service(2, 0.25, 0.08);
    ps_add_service(3, 0.50, 0.15);
    ps_add_service(4, 1.00, 0.30);
    ps_add_service(5, 2.00, 0.60);
    ps_add_service(1, 0.20, 0.07);
    ps_add_service(2, 0.30, 0.10);
    ps_add_service(3, 0.60, 0.18);
    ps_add_service(4, 1.20, 0.35);
    ps_add_service(5, 2.50, 0.75);

    print_str("\nAdding materials...\n");
    ps_add_material(1, 0.03, 5000);
    ps_add_material(1, 0.05, 3000);
    ps_add_material(2, 0.08, 2000);
    ps_add_material(2, 0.12, 1500);
    ps_add_material(3, 0.15, 1000);
    ps_add_material(3, 0.20, 800);
    ps_add_material(4, 0.25, 500);
    ps_add_material(4, 0.35, 300);

    print_str("\nAdding designers...\n");
    ps_add_designer(1, 88);
    ps_add_designer(2, 92);
    ps_add_designer(1, 85);
    ps_add_designer(2, 90);
    ps_add_designer(0, 82);
    ps_add_designer(0, 87);

    print_str("\nAdding equipment...\n");
    ps_add_equipment(1);
    ps_add_equipment(1);
    ps_add_equipment(2);
    ps_add_equipment(2);
    ps_add_equipment(3);
    ps_add_equipment(3);

    print_str("\nRegistering customers...\n");
    for (int i = 0; i < 10; i++) {
        ps_register_customer(i % 3);
    }

    print_str("\nCreating orders...\n");
    ps_create_order(0, 0, 0, 500, 0, 0, 10);
    ps_create_order(1, 1, 1, 300, 1, 1, 11);
    ps_create_order(2, 2, 2, 200, 2, 0, 12);
    ps_create_order(3, 3, 3, 100, 3, 1, 13);
    ps_create_order(4, 4, 4, 50, -1, 0, 14);
    ps_create_order(5, 5, 5, 150, 4, 0, 15);
    ps_create_order(6, 6, 0, 400, -1, 1, 16);
    ps_create_order(7, 7, 1, 250, 5, 0, 17);
    ps_create_order(8, 8, 2, 180, 0, 1, 18);
    ps_create_order(9, 9, 3, 80, 1, 0, 19);
    ps_create_order(0, 0, 4, 600, -1, 0, 20);
    ps_create_order(1, 1, 5, 350, 2, 1, 21);
    ps_create_order(2, 2, 6, 220, -1, 0, 22);
    ps_create_order(3, 3, 7, 120, 3, 0, 23);
    ps_create_order(4, 4, 0, 75, 4, 1, 24);
    ps_create_order(5, 5, 1, 170, -1, 0, 25);
    ps_create_order(6, 6, 2, 280, 5, 1, 26);
    ps_create_order(7, 7, 3, 140, 0, 0, 27);

    print_str("\nService report...\n");
    ps_service_report();

    print_str("\nDesigner report...\n");
    ps_designer_report();

    print_str("\nFinancial report...\n");
    ps_financial_report();

    print_str("\nFinal state...\n");
    ps_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
