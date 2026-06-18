/* bakery_chain: Bakery chain management (v1.0)
 * Stores, production, ingredients, orders, members, revenue
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

#define MAX_STORES    8
#define MAX_PRODUCTS  14
#define MAX_INGREDIENTS 10
#define MAX_ORDERS    16
#define MAX_MEMBERS   12

typedef struct {
    int    store_id;
    int    location;
    int    n_employees;
    double daily_revenue;
    double rent;
    int    total_sales;
    int    active;
} store_t;

typedef struct {
    int    prod_id;
    int    type;
    double price;
    double cost;
    int    produced;
    int    sold;
    double revenue;
    int    active;
} product_t;

typedef struct {
    int    ing_id;
    int    type;
    int    stock_lbs;
    double unit_cost;
    int    reorder_level;
    int    n_used;
    double total_cost;
    int    active;
} ingredient_t;

typedef struct {
    int    order_id;
    int    store_id;
    int    type;
    int    qty;
    double total;
    int    day;
    int    active;
} order_t;

typedef struct {
    int    member_id;
    int    points;
    double total_spent;
    int    n_visits;
    int    birthday_month;
    int    active;
} member_t;

typedef struct {
    int    n_stores;
    int    n_products;
    int    n_ingredients;
    int    n_orders;
    int    n_members;
    double retail_revenue;
    double wholesale_revenue;
    double custom_revenue;
    double total_revenue;
    double ingredient_costs;
    double labor_costs;
    double rent_costs;
    int    total_items_sold;
} bc_state_t;

static store_t stores[MAX_STORES];
static product_t products[MAX_PRODUCTS];
static ingredient_t ingredients[MAX_INGREDIENTS];
static order_t orders[MAX_ORDERS];
static member_t members[MAX_MEMBERS];
static bc_state_t bc;

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

int bc_init(void) {
    if (initialized) return -1;
    bc.n_stores = 0; bc.n_products = 0; bc.n_ingredients = 0;
    bc.n_orders = 0; bc.n_members = 0;
    bc.retail_revenue = 0.0; bc.wholesale_revenue = 0.0;
    bc.custom_revenue = 0.0; bc.total_revenue = 0.0;
    bc.ingredient_costs = 0.0; bc.labor_costs = 0.0;
    bc.rent_costs = 0.0; bc.total_items_sold = 0;
    for (int i = 0; i < MAX_STORES; i++) stores[i].active = 0;
    for (int i = 0; i < MAX_PRODUCTS; i++) products[i].active = 0;
    for (int i = 0; i < MAX_INGREDIENTS; i++) ingredients[i].active = 0;
    for (int i = 0; i < MAX_ORDERS; i++) orders[i].active = 0;
    for (int i = 0; i < MAX_MEMBERS; i++) members[i].active = 0;
    initialized = 1;
    print_str("[BC] Bakery initialized\n");
    return 0;
}

int bc_add_store(int location, int n_emp, double daily_rev, double rent) {
    if (bc.n_stores >= MAX_STORES) return -1;
    store_t* s = &stores[bc.n_stores];
    s->store_id = bc.n_stores;
    s->location = location;
    s->n_employees = n_emp;
    s->daily_revenue = daily_rev;
    s->rent = rent;
    s->total_sales = 0;
    s->active = 1;
    bc.rent_costs += rent;
    bc.labor_costs += n_emp * 120.0;
    bc.n_stores++;
    print_str("[BC] Store "); print_int(bc.n_stores - 1);
    print_str(" loc="); print_int(location);
    print_str(" emp="); print_int(n_emp);
    print_str(" $"); print_int((int)daily_rev); print_str("/day\n");
    return bc.n_stores - 1;
}

int bc_add_product(int type, double price, double cost) {
    if (bc.n_products >= MAX_PRODUCTS) return -1;
    product_t* p = &products[bc.n_products];
    p->prod_id = bc.n_products;
    p->type = type;
    p->price = price;
    p->cost = cost;
    p->produced = 0;
    p->sold = 0;
    p->revenue = 0.0;
    p->active = 1;
    bc.n_products++;
    print_str("[BC] Product "); print_int(bc.n_products - 1);
    print_str(" type="); print_int(type);
    print_str(" $"); print_int((int)price); print_str("\n");
    return bc.n_products - 1;
}

int bc_add_ingredient(int type, int stock, double cost, int reorder) {
    if (bc.n_ingredients >= MAX_INGREDIENTS) return -1;
    ingredient_t* ig = &ingredients[bc.n_ingredients];
    ig->ing_id = bc.n_ingredients;
    ig->type = type;
    ig->stock_lbs = stock;
    ig->unit_cost = cost;
    ig->reorder_level = reorder;
    ig->n_used = 0;
    ig->total_cost = 0.0;
    ig->active = 1;
    bc.n_ingredients++;
    print_str("[BC] Ingredient "); print_int(bc.n_ingredients - 1);
    print_str(" type="); print_int(type);
    print_str(" stock="); print_int(stock); print_str("lb");
    print_str(" $"); print_int((int)cost); print_str("/lb\n");
    return bc.n_ingredients - 1;
}

int bc_produce(int prod_id, int qty) {
    if (prod_id >= bc.n_products) return -1;
    product_t* p = &products[prod_id];
    p->produced += qty;
    print_str("[BC] Produce Pr"); print_int(prod_id);
    print_str(" qty="); print_int(qty); print_str("\n");
    return 0;
}

int bc_use_ingredient(int ing_id, int lbs) {
    if (ing_id >= bc.n_ingredients) return -1;
    ingredient_t* ig = &ingredients[ing_id];
    if (ig->stock_lbs < lbs) return -2;
    ig->stock_lbs -= lbs;
    ig->n_used += lbs;
    double cost = ig->unit_cost * lbs;
    ig->total_cost += cost;
    bc.ingredient_costs += cost;
    print_str("[BC] Use Ing"); print_int(ing_id);
    print_str(" lbs="); print_int(lbs);
    print_str(" $"); print_int((int)cost); print_str("\n");
    return 0;
}

int bc_create_order(int store_id, int type, int qty, int day) {
    if (bc.n_orders >= MAX_ORDERS || store_id >= bc.n_stores) return -1;
    order_t* o = &orders[bc.n_orders];
    o->order_id = bc.n_orders;
    o->store_id = store_id;
    o->type = type;
    o->qty = qty;
    o->total = 0.0;
    o->day = day;
    o->active = 1;
    bc.n_orders++;
    print_str("[BC] Order "); print_int(bc.n_orders - 1);
    print_str(" S"); print_int(store_id);
    print_str(" type="); print_int(type);
    print_str(" qty="); print_int(qty); print_str("\n");
    return bc.n_orders - 1;
}

int bc_retail_sale(int prod_id, int store_id, int qty) {
    if (prod_id >= bc.n_products || store_id >= bc.n_stores) return -1;
    product_t* p = &products[prod_id];
    if (p->produced - p->sold < qty) return -2;
    double rev = p->price * qty;
    p->sold += qty;
    p->revenue += rev;
    stores[store_id].total_sales += qty;
    bc.retail_revenue += rev;
    bc.total_revenue += rev;
    bc.total_items_sold += qty;
    print_str("[BC] Retail Pr"); print_int(prod_id);
    print_str(" S"); print_int(store_id);
    print_str(" qty="); print_int(qty);
    print_str(" $"); print_int((int)rev); print_str("\n");
    return 0;
}

int bc_wholesale(int prod_id, int qty, double unit_price) {
    if (prod_id >= bc.n_products) return -1;
    product_t* p = &products[prod_id];
    if (p->produced - p->sold < qty) return -2;
    double rev = unit_price * qty;
    p->sold += qty;
    p->revenue += rev;
    bc.wholesale_revenue += rev;
    bc.total_revenue += rev;
    bc.total_items_sold += qty;
    print_str("[BC] Wholesale Pr"); print_int(prod_id);
    print_str(" qty="); print_int(qty);
    print_str(" $"); print_int((int)rev); print_str("\n");
    return 0;
}

int bc_custom_order(int prod_id, int qty, double total) {
    if (prod_id >= bc.n_products) return -1;
    product_t* p = &products[prod_id];
    p->sold += qty;
    p->revenue += total;
    bc.custom_revenue += total;
    bc.total_revenue += total;
    bc.total_items_sold += qty;
    print_str("[BC] Custom Pr"); print_int(prod_id);
    print_str(" qty="); print_int(qty);
    print_str(" $"); print_int((int)total); print_str("\n");
    return 0;
}

int bc_register_member(int birthday_month) {
    if (bc.n_members >= MAX_MEMBERS) return -1;
    member_t* m = &members[bc.n_members];
    m->member_id = bc.n_members;
    m->points = 0;
    m->total_spent = 0.0;
    m->n_visits = 0;
    m->birthday_month = birthday_month;
    m->active = 1;
    bc.n_members++;
    print_str("[BC] Member "); print_int(bc.n_members - 1);
    print_str(" bday="); print_int(birthday_month); print_str("\n");
    return bc.n_members - 1;
}

int bc_member_purchase(int member_id, double amount) {
    if (member_id >= bc.n_members) return -1;
    member_t* m = &members[member_id];
    m->total_spent += amount;
    m->points += (int)(amount / 10);
    m->n_visits++;
    print_str("[BC] Member"); print_int(member_id);
    print_str(" spent $"); print_int((int)amount); print_str("\n");
    return 0;
}

void bc_store_report(void) {
    print_str("[BC] Store report:\n");
    for (int i = 0; i < bc.n_stores; i++) {
        store_t* s = &stores[i];
        print_str("  S"); print_int(i);
        print_str(" loc="); print_int(s->location);
        print_str(" emp="); print_int(s->n_employees);
        print_str(" sales="); print_int(s->total_sales);
        print_str(" rev="); print_int((int)s->daily_revenue); print_str("/day\n");
    }
}

void bc_financial_report(void) {
    print_str("[BC] Financial report:\n");
    print_str("  Retail revenue: "); print_int((int)bc.retail_revenue); print_str("\n");
    print_str("  Wholesale revenue: "); print_int((int)bc.wholesale_revenue); print_str("\n");
    print_str("  Custom revenue: "); print_int((int)bc.custom_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)bc.total_revenue); print_str("\n");
    print_str("  Ingredient costs: "); print_int((int)bc.ingredient_costs); print_str("\n");
    print_str("  Labor costs: "); print_int((int)bc.labor_costs); print_str("\n");
    print_str("  Rent costs: "); print_int((int)bc.rent_costs); print_str("\n");
    double total_costs = bc.ingredient_costs + bc.labor_costs + bc.rent_costs;
    double profit = bc.total_revenue - total_costs;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    print_str("  Items sold: "); print_int(bc.total_items_sold); print_str("\n");
}

void bc_print_state(void) {
    print_str("[BC] Stores="); print_int(bc.n_stores);
    print_str(" Products="); print_int(bc.n_products);
    print_str(" Orders="); print_int(bc.n_orders);
    print_str(" Members="); print_int(bc.n_members);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)bc.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Bakery Chain Demo ===\n\n");
    bc_init();

    print_str("Adding stores...\n");
    bc_add_store(1, 8, 2500.0, 5000.0);
    bc_add_store(2, 6, 2000.0, 4000.0);
    bc_add_store(3, 10, 3000.0, 6000.0);
    bc_add_store(1, 7, 2200.0, 4500.0);
    bc_add_store(2, 9, 2800.0, 5500.0);
    bc_add_store(3, 6, 1800.0, 3500.0);
    bc_add_store(1, 8, 2600.0, 5200.0);
    bc_add_store(2, 7, 2100.0, 4200.0);

    print_str("\nAdding products...\n");
    bc_add_product(1, 4.5, 1.5);
    bc_add_product(1, 5.0, 1.8);
    bc_add_product(2, 25.0, 8.0);
    bc_add_product(2, 35.0, 12.0);
    bc_add_product(3, 3.5, 1.2);
    bc_add_product(3, 4.0, 1.5);
    bc_add_product(4, 6.0, 2.0);
    bc_add_product(4, 7.5, 2.5);
    bc_add_product(1, 5.5, 2.0);
    bc_add_product(2, 30.0, 10.0);
    bc_add_product(3, 3.0, 1.0);
    bc_add_product(4, 8.0, 3.0);
    bc_add_product(1, 4.0, 1.4);
    bc_add_product(2, 28.0, 9.0);

    print_str("\nAdding ingredients...\n");
    bc_add_ingredient(1, 500, 0.8, 100);
    bc_add_ingredient(2, 300, 1.5, 50);
    bc_add_ingredient(3, 200, 4.0, 40);
    bc_add_ingredient(4, 100, 3.5, 20);
    bc_add_ingredient(5, 150, 2.0, 30);
    bc_add_ingredient(6, 80, 5.0, 15);
    bc_add_ingredient(1, 400, 0.75, 80);
    bc_add_ingredient(2, 250, 1.4, 45);
    bc_add_ingredient(3, 180, 3.8, 35);
    bc_add_ingredient(4, 90, 3.2, 18);

    print_str("\nProducing...\n");
    for (int i = 0; i < 14; i++) {
        bc_produce(i, 100 + (i * 10));
    }

    print_str("\nUsing ingredients...\n");
    for (int i = 0; i < 10; i++) {
        bc_use_ingredient(i, 20 + (i * 5));
    }

    print_str("\nCreating orders...\n");
    for (int i = 0; i < 16; i++) {
        bc_create_order(i % 8, (i % 4) + 1, 10 + (i * 2), 10 + (i % 5));
    }

    print_str("\nRetail sales...\n");
    for (int i = 0; i < 14; i++) {
        bc_retail_sale(i, i % 8, 40 + (i * 5));
    }

    print_str("\nWholesale...\n");
    bc_wholesale(0, 200, 3.0);
    bc_wholesale(1, 180, 3.5);
    bc_wholesale(4, 250, 2.5);
    bc_wholesale(5, 220, 3.0);
    bc_wholesale(10, 300, 2.0);
    bc_wholesale(11, 150, 6.0);

    print_str("\nCustom orders...\n");
    bc_custom_order(2, 5, 150.0);
    bc_custom_order(3, 3, 120.0);
    bc_custom_order(9, 4, 140.0);
    bc_custom_order(13, 6, 180.0);

    print_str("\nRegistering members...\n");
    for (int i = 0; i < 12; i++) {
        bc_register_member((i % 12) + 1);
    }

    print_str("\nMember purchases...\n");
    for (int i = 0; i < 12; i++) {
        bc_member_purchase(i, 50.0 + i * 15.0);
    }

    print_str("\nStore report...\n");
    bc_store_report();

    print_str("\nFinancial report...\n");
    bc_financial_report();

    print_str("\nFinal state...\n");
    bc_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
