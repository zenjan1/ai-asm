/* restaurant_chain: Restaurant chain management (v1.0)
 * Multi-store ops, menu management, ordering, inventory, staff scheduling
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

#define MAX_STORES    6
#define MAX_STAFF     12
#define MAX_MENU_ITEMS 14
#define MAX_ORDERS    18
#define MAX_ITEMS_PER_ORDER 8
#define MAX_INGREDIENTS 12

typedef struct {
    int    store_id;
    int    zone;
    int    capacity;
    int    tables;
    int    n_staff;
    double daily_revenue;
    int    active;
} store_t;

typedef struct {
    int    staff_id;
    int    store_id;
    int    role;
    int    shift;
    int    hours_worked;
    double wage;
    int    attendance;
    int    active;
} staff_t;

typedef struct {
    int    item_id;
    int    category;
    double price;
    double cost;
    int    n_ordered;
    double revenue;
    int    active;
} menu_item_t;

typedef struct {
    int    order_id;
    int    store_id;
    int    table_num;
    int    n_items;
    int    items[MAX_ITEMS_PER_ORDER];
    int    qty[MAX_ITEMS_PER_ORDER];
    double total;
    int    type;
    int    status;
    int    active;
} order_t;

typedef struct {
    int    ingredient_id;
    int    type;
    int    stock;
    double unit_cost;
    int    reorder_level;
    int    n_purchases;
    double purchase_cost;
    int    active;
} ingredient_t;

typedef struct {
    int    n_stores;
    int    n_staff;
    int    n_items;
    int    n_orders;
    int    n_ingredients;
    double total_revenue;
    double total_costs;
    double total_wages;
    int    total_orders;
    int    dine_in_orders;
    int    takeaway_orders;
} rc_state_t;

static store_t stores[MAX_STORES];
static staff_t staff[MAX_STAFF];
static menu_item_t menu_items[MAX_MENU_ITEMS];
static order_t orders[MAX_ORDERS];
static ingredient_t ingredients[MAX_INGREDIENTS];
static rc_state_t rc;

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

int rc_init(void) {
    if (initialized) return -1;
    rc.n_stores = 0; rc.n_staff = 0; rc.n_items = 0;
    rc.n_orders = 0; rc.n_ingredients = 0;
    rc.total_revenue = 0.0; rc.total_costs = 0.0;
    rc.total_wages = 0.0; rc.total_orders = 0;
    rc.dine_in_orders = 0; rc.takeaway_orders = 0;
    for (int i = 0; i < MAX_STORES; i++) stores[i].active = 0;
    for (int i = 0; i < MAX_STAFF; i++) staff[i].active = 0;
    for (int i = 0; i < MAX_MENU_ITEMS; i++) menu_items[i].active = 0;
    for (int i = 0; i < MAX_ORDERS; i++) orders[i].active = 0;
    for (int i = 0; i < MAX_INGREDIENTS; i++) ingredients[i].active = 0;
    initialized = 1;
    print_str("[RC] Restaurant chain initialized\n");
    return 0;
}

int rc_add_store(int zone, int capacity, int tables) {
    if (rc.n_stores >= MAX_STORES) return -1;
    store_t* s = &stores[rc.n_stores];
    s->store_id = rc.n_stores;
    s->zone = zone;
    s->capacity = capacity;
    s->tables = tables;
    s->n_staff = 0;
    s->daily_revenue = 0.0;
    s->active = 1;
    rc.n_stores++;
    print_str("[RC] Store "); print_int(rc.n_stores - 1);
    print_str(" zone="); print_int(zone);
    print_str(" cap="); print_int(capacity);
    print_str(" tables="); print_int(tables); print_str("\n");
    return rc.n_stores - 1;
}

int rc_add_staff(int store_id, int role, int shift, double wage) {
    if (rc.n_staff >= MAX_STAFF || store_id >= rc.n_stores) return -1;
    staff_t* s = &staff[rc.n_staff];
    s->staff_id = rc.n_staff;
    s->store_id = store_id;
    s->role = role;
    s->shift = shift;
    s->hours_worked = 0;
    s->wage = wage;
    s->attendance = 0;
    s->active = 1;
    stores[store_id].n_staff++;
    rc.n_staff++;
    print_str("[RC] Staff "); print_int(rc.n_staff - 1);
    print_str(" S"); print_int(store_id);
    print_str(" role="); print_int(role);
    print_str(" shift="); print_int(shift);
    print_str(" wage="); print_int((int)wage); print_str("/hr\n");
    return rc.n_staff - 1;
}

int rc_add_menu_item(int category, double price, double cost) {
    if (rc.n_items >= MAX_MENU_ITEMS) return -1;
    menu_item_t* m = &menu_items[rc.n_items];
    m->item_id = rc.n_items;
    m->category = category;
    m->price = price;
    m->cost = cost;
    m->n_ordered = 0;
    m->revenue = 0.0;
    m->active = 1;
    rc.n_items++;
    print_str("[RC] Menu "); print_int(rc.n_items - 1);
    print_str(" cat="); print_int(category);
    print_str(" price="); print_int((int)price);
    print_str(" cost="); print_int((int)cost); print_str("\n");
    return rc.n_items - 1;
}

int rc_add_ingredient(int type, int stock, double unit_cost, int reorder) {
    if (rc.n_ingredients >= MAX_INGREDIENTS) return -1;
    ingredient_t* ing = &ingredients[rc.n_ingredients];
    ing->ingredient_id = rc.n_ingredients;
    ing->type = type;
    ing->stock = stock;
    ing->unit_cost = unit_cost;
    ing->reorder_level = reorder;
    ing->n_purchases = 0;
    ing->purchase_cost = 0.0;
    ing->active = 1;
    rc.n_ingredients++;
    print_str("[RC] Ingredient "); print_int(rc.n_ingredients - 1);
    print_str(" type="); print_int(type);
    print_str(" stock="); print_int(stock);
    print_str(" cost="); print_int((int)(unit_cost * 10)); print_str("\n");
    return rc.n_ingredients - 1;
}

int rc_create_order(int store_id, int table, int type) {
    if (rc.n_orders >= MAX_ORDERS || store_id >= rc.n_stores) return -1;
    order_t* o = &orders[rc.n_orders];
    o->order_id = rc.n_orders;
    o->store_id = store_id;
    o->table_num = table;
    o->n_items = 0;
    for (int i = 0; i < MAX_ITEMS_PER_ORDER; i++) {
        o->items[i] = -1;
        o->qty[i] = 0;
    }
    o->total = 0.0;
    o->type = type;
    o->status = 1;
    o->active = 1;
    rc.n_orders++;
    rc.total_orders++;
    if (type == 1) rc.dine_in_orders++;
    else rc.takeaway_orders++;
    print_str("[RC] Order "); print_int(rc.n_orders - 1);
    print_str(" S"); print_int(store_id);
    print_str(" T"); print_int(table);
    if (type == 1) print_str(" dine-in");
    else print_str(" takeaway");
    print_str("\n");
    return rc.n_orders - 1;
}

int rc_add_item_to_order(int order_id, int menu_item_id, int qty) {
    if (order_id >= rc.n_orders || menu_item_id >= rc.n_items) return -1;
    order_t* o = &orders[order_id];
    if (o->n_items >= MAX_ITEMS_PER_ORDER) return -2;
    menu_item_t* m = &menu_items[menu_item_id];
    o->items[o->n_items] = menu_item_id;
    o->qty[o->n_items] = qty;
    o->total += m->price * qty;
    m->n_ordered += qty;
    m->revenue += m->price * qty;
    o->n_items++;
    print_str("[RC] Added M"); print_int(menu_item_id);
    print_str(" x"); print_int(qty);
    print_str(" to O"); print_int(order_id);
    print_str(" total="); print_int((int)o->total); print_str("\n");
    return 0;
}

void rc_complete_order(int order_id) {
    if (order_id >= rc.n_orders) return;
    order_t* o = &orders[order_id];
    o->status = 3;
    stores[o->store_id].daily_revenue += o->total;
    rc.total_revenue += o->total;
    for (int i = 0; i < o->n_items; i++) {
        int mid = o->items[i];
        rc.total_costs += menu_items[mid].cost * o->qty[i];
    }
    print_str("[RC] Order "); print_int(order_id);
    print_str(" completed total="); print_int((int)o->total); print_str("\n");
}

void rc_staff_clock_in(int staff_id, int hours) {
    if (staff_id >= rc.n_staff) return;
    staff[staff_id].hours_worked += hours;
    staff[staff_id].attendance++;
    double pay = staff[staff_id].wage * hours;
    rc.total_wages += pay;
    print_str("[RC] Staff "); print_int(staff_id);
    print_str(" clocked "); print_int(hours); print_str("hr");
    print_str(" pay="); print_int((int)pay); print_str("\n");
}

int rc_purchase_ingredient(int ingredient_id, int qty) {
    if (ingredient_id >= rc.n_ingredients) return -1;
    ingredient_t* ing = &ingredients[ingredient_id];
    ing->stock += qty;
    ing->n_purchases++;
    double cost = ing->unit_cost * qty;
    ing->purchase_cost += cost;
    rc.total_costs += cost;
    print_str("[RC] Purchased I"); print_int(ingredient_id);
    print_str(" qty="); print_int(qty);
    print_str(" cost="); print_int((int)cost);
    print_str(" stock="); print_int(ing->stock); print_str("\n");
    return 0;
}

void rc_inventory_check(void) {
    print_str("[RC] Inventory check:\n");
    for (int i = 0; i < rc.n_ingredients; i++) {
        ingredient_t* ing = &ingredients[i];
        print_str("  I"); print_int(i);
        print_str(" type="); print_int(ing->type);
        print_str(" stock="); print_int(ing->stock);
        if (ing->stock <= ing->reorder_level) {
            print_str(" [LOW STOCK]");
        }
        print_str(" purchases="); print_int(ing->n_purchases);
        print_str(" cost="); print_int((int)ing->purchase_cost); print_str("\n");
    }
}

void rc_store_report(void) {
    print_str("[RC] Store report:\n");
    for (int i = 0; i < rc.n_stores; i++) {
        store_t* s = &stores[i];
        print_str("  S"); print_int(i);
        print_str(" zone="); print_int(s->zone);
        print_str(" staff="); print_int(s->n_staff);
        print_str(" revenue="); print_int((int)s->daily_revenue); print_str("\n");
    }
}

void rc_financial_summary(void) {
    print_str("[RC] Financial summary:\n");
    print_str("  Total revenue: "); print_int((int)rc.total_revenue); print_str("\n");
    print_str("  Total costs: "); print_int((int)rc.total_costs); print_str("\n");
    print_str("  Total wages: "); print_int((int)rc.total_wages); print_str("\n");
    double profit = rc.total_revenue - rc.total_costs - rc.total_wages;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    double margin = (rc.total_revenue > 0) ? profit / rc.total_revenue * 100.0 : 0.0;
    print_str("  Margin: "); print_int((int)margin); print_str("%\n");
    print_str("  Dine-in: "); print_int(rc.dine_in_orders); print_str("\n");
    print_str("  Takeaway: "); print_int(rc.takeaway_orders); print_str("\n");
}

void rc_print_state(void) {
    print_str("[RC] Stores="); print_int(rc.n_stores);
    print_str(" Staff="); print_int(rc.n_staff);
    print_str(" Menu="); print_int(rc.n_items);
    print_str(" Orders="); print_int(rc.n_orders);
    print_str(" Ingredients="); print_int(rc.n_ingredients);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)rc.total_revenue);
    print_str(" Profit margin: ");
    double profit = rc.total_revenue - rc.total_costs - rc.total_wages;
    double margin = (rc.total_revenue > 0) ? profit / rc.total_revenue * 100.0 : 0.0;
    print_int((int)margin); print_str("%\n");
}

int main(void) {
    print_str("=== Restaurant Chain Demo ===\n\n");
    rc_init();

    print_str("Adding stores...\n");
    rc_add_store(1, 80, 12);
    rc_add_store(1, 60, 10);
    rc_add_store(2, 100, 15);
    rc_add_store(2, 70, 10);
    rc_add_store(3, 90, 14);

    print_str("\nAdding staff...\n");
    rc_add_staff(0, 1, 1, 15.0);
    rc_add_staff(0, 1, 2, 15.0);
    rc_add_staff(0, 2, 1, 12.0);
    rc_add_staff(1, 1, 1, 14.0);
    rc_add_staff(1, 2, 2, 11.0);
    rc_add_staff(2, 1, 1, 16.0);
    rc_add_staff(2, 1, 2, 16.0);
    rc_add_staff(2, 2, 1, 13.0);
    rc_add_staff(3, 1, 1, 15.0);
    rc_add_staff(3, 2, 2, 12.0);
    rc_add_staff(4, 1, 1, 15.0);
    rc_add_staff(4, 2, 1, 12.0);

    print_str("\nAdding menu items...\n");
    rc_add_menu_item(1, 12.0, 4.0);
    rc_add_menu_item(1, 15.0, 5.0);
    rc_add_menu_item(1, 18.0, 6.0);
    rc_add_menu_item(2, 8.0, 2.5);
    rc_add_menu_item(2, 10.0, 3.0);
    rc_add_menu_item(3, 6.0, 1.5);
    rc_add_menu_item(3, 5.0, 1.0);
    rc_add_menu_item(3, 7.0, 2.0);
    rc_add_menu_item(4, 4.0, 1.0);
    rc_add_menu_item(4, 5.0, 1.5);
    rc_add_menu_item(4, 3.5, 0.8);
    rc_add_menu_item(1, 25.0, 8.0);
    rc_add_menu_item(2, 9.0, 3.0);
    rc_add_menu_item(3, 4.5, 1.2);

    print_str("\nAdding ingredients...\n");
    rc_add_ingredient(1, 100, 2.0, 30);
    rc_add_ingredient(2, 80, 3.5, 25);
    rc_add_ingredient(3, 60, 1.5, 20);
    rc_add_ingredient(4, 50, 4.0, 15);
    rc_add_ingredient(5, 120, 1.0, 40);
    rc_add_ingredient(1, 90, 2.5, 30);
    rc_add_ingredient(2, 70, 3.0, 20);
    rc_add_ingredient(3, 45, 1.8, 15);

    print_str("\nCreating orders...\n");
    int o0 = rc_create_order(0, 1, 1);
    int o1 = rc_create_order(0, 3, 1);
    int o2 = rc_create_order(0, 5, 2);
    int o3 = rc_create_order(1, 2, 1);
    int o4 = rc_create_order(1, 4, 2);
    int o5 = rc_create_order(2, 1, 1);
    int o6 = rc_create_order(2, 6, 1);
    int o7 = rc_create_order(2, 8, 2);
    int o8 = rc_create_order(3, 3, 1);
    int o9 = rc_create_order(3, 7, 2);
    int o10 = rc_create_order(4, 2, 1);
    int o11 = rc_create_order(4, 9, 1);

    print_str("\nAdding items to orders...\n");
    rc_add_item_to_order(o0, 0, 2);
    rc_add_item_to_order(o0, 5, 1);
    rc_add_item_to_order(o0, 8, 2);
    rc_add_item_to_order(o1, 11, 1);
    rc_add_item_to_order(o1, 3, 2);
    rc_add_item_to_order(o1, 9, 1);
    rc_add_item_to_order(o2, 1, 1);
    rc_add_item_to_order(o2, 6, 2);
    rc_add_item_to_order(o3, 0, 1);
    rc_add_item_to_order(o3, 4, 2);
    rc_add_item_to_order(o3, 12, 1);
    rc_add_item_to_order(o4, 2, 2);
    rc_add_item_to_order(o4, 7, 1);
    rc_add_item_to_order(o5, 11, 2);
    rc_add_item_to_order(o5, 3, 3);
    rc_add_item_to_order(o6, 0, 3);
    rc_add_item_to_order(o6, 1, 2);
    rc_add_item_to_order(o6, 10, 2);
    rc_add_item_to_order(o7, 5, 2);
    rc_add_item_to_order(o7, 13, 3);
    rc_add_item_to_order(o8, 1, 1);
    rc_add_item_to_order(o8, 9, 2);
    rc_add_item_to_order(o9, 2, 1);
    rc_add_item_to_order(o9, 4, 1);
    rc_add_item_to_order(o10, 0, 2);
    rc_add_item_to_order(o10, 3, 1);
    rc_add_item_to_order(o11, 11, 1);
    rc_add_item_to_order(o11, 8, 3);

    print_str("\nCompleting orders...\n");
    rc_complete_order(o0);
    rc_complete_order(o1);
    rc_complete_order(o2);
    rc_complete_order(o3);
    rc_complete_order(o4);
    rc_complete_order(o5);
    rc_complete_order(o6);
    rc_complete_order(o7);
    rc_complete_order(o8);
    rc_complete_order(o9);
    rc_complete_order(o10);
    rc_complete_order(o11);

    print_str("\nStaff clock-in...\n");
    rc_staff_clock_in(0, 8);
    rc_staff_clock_in(1, 8);
    rc_staff_clock_in(2, 6);
    rc_staff_clock_in(3, 8);
    rc_staff_clock_in(4, 6);
    rc_staff_clock_in(5, 8);
    rc_staff_clock_in(6, 8);
    rc_staff_clock_in(7, 6);
    rc_staff_clock_in(8, 8);
    rc_staff_clock_in(9, 6);
    rc_staff_clock_in(10, 8);
    rc_staff_clock_in(11, 6);

    print_str("\nPurchasing ingredients...\n");
    rc_purchase_ingredient(0, 50);
    rc_purchase_ingredient(1, 40);
    rc_purchase_ingredient(2, 30);
    rc_purchase_ingredient(3, 25);
    rc_purchase_ingredient(4, 60);
    rc_purchase_ingredient(5, 30);
    rc_purchase_ingredient(6, 40);
    rc_purchase_ingredient(7, 20);

    print_str("\nInventory check...\n");
    rc_inventory_check();

    print_str("\nStore report...\n");
    rc_store_report();

    print_str("\nFinancial summary...\n");
    rc_financial_summary();

    print_str("\nFinal state...\n");
    rc_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
