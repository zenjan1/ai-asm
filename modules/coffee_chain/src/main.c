/* coffee_chain: Coffee chain management (v1.0)
 * Stores, beans, drinks, food, members, revenue
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

#define MAX_STORES   10
#define MAX_BEANS    8
#define MAX_DRINKS   14
#define MAX_FOOD     10
#define MAX_MEMBERS  16

typedef struct {
    int    store_id;
    int    location;
    int    n_employees;
    int    seats;
    double daily_revenue;
    double rent;
    int    total_drinks;
    int    total_food;
    int    active;
} store_t;

typedef struct {
    int    bean_id;
    int    origin;
    int    roast;
    int    stock_lbs;
    double cost_per_lb;
    int    n_used;
    double total_cost;
    int    active;
} bean_t;

typedef struct {
    int    drink_id;
    int    type;
    double price;
    double cost;
    int    beans_oz;
    int    sold;
    double revenue;
    int    active;
} drink_t;

typedef struct {
    int    food_id;
    int    type;
    double price;
    double cost;
    int    stock;
    int    sold;
    double revenue;
    int    active;
} food_t;

typedef struct {
    int    member_id;
    double balance;
    int    points;
    int    free_drinks;
    double total_spent;
    int    n_visits;
    int    active;
} member_t;

typedef struct {
    int    n_stores;
    int    n_beans;
    int    n_drinks;
    int    n_food;
    int    n_members;
    double drink_revenue;
    double food_revenue;
    double merch_revenue;
    double member_loads;
    double total_revenue;
    double bean_costs;
    double labor_costs;
    double rent_costs;
    int    total_drinks_sold;
    int    total_food_sold;
} cc_state_t;

static store_t stores[MAX_STORES];
static bean_t beans[MAX_BEANS];
static drink_t drinks[MAX_DRINKS];
static food_t foods[MAX_FOOD];
static member_t members[MAX_MEMBERS];
static cc_state_t cc;

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

int cc_init(void) {
    if (initialized) return -1;
    cc.n_stores = 0; cc.n_beans = 0; cc.n_drinks = 0;
    cc.n_food = 0; cc.n_members = 0;
    cc.drink_revenue = 0.0; cc.food_revenue = 0.0;
    cc.merch_revenue = 0.0; cc.member_loads = 0.0;
    cc.total_revenue = 0.0; cc.bean_costs = 0.0;
    cc.labor_costs = 0.0; cc.rent_costs = 0.0;
    cc.total_drinks_sold = 0; cc.total_food_sold = 0;
    for (int i = 0; i < MAX_STORES; i++) stores[i].active = 0;
    for (int i = 0; i < MAX_BEANS; i++) beans[i].active = 0;
    for (int i = 0; i < MAX_DRINKS; i++) drinks[i].active = 0;
    for (int i = 0; i < MAX_FOOD; i++) foods[i].active = 0;
    for (int i = 0; i < MAX_MEMBERS; i++) members[i].active = 0;
    initialized = 1;
    print_str("[CC] Coffee chain initialized\n");
    return 0;
}

int cc_add_store(int location, int n_emp, int seats, double daily_rev, double rent) {
    if (cc.n_stores >= MAX_STORES) return -1;
    store_t* s = &stores[cc.n_stores];
    s->store_id = cc.n_stores;
    s->location = location;
    s->n_employees = n_emp;
    s->seats = seats;
    s->daily_revenue = daily_rev;
    s->rent = rent;
    s->total_drinks = 0;
    s->total_food = 0;
    s->active = 1;
    cc.rent_costs += rent;
    cc.labor_costs += n_emp * 100.0;
    cc.n_stores++;
    print_str("[CC] Store "); print_int(cc.n_stores - 1);
    print_str(" loc="); print_int(location);
    print_str(" emp="); print_int(n_emp);
    print_str(" seats="); print_int(seats); print_str("\n");
    return cc.n_stores - 1;
}

int cc_add_bean(int origin, int roast, int stock, double cost) {
    if (cc.n_beans >= MAX_BEANS) return -1;
    bean_t* b = &beans[cc.n_beans];
    b->bean_id = cc.n_beans;
    b->origin = origin;
    b->roast = roast;
    b->stock_lbs = stock;
    b->cost_per_lb = cost;
    b->n_used = 0;
    b->total_cost = 0.0;
    b->active = 1;
    cc.n_beans++;
    print_str("[CC] Bean "); print_int(cc.n_beans - 1);
    print_str(" origin="); print_int(origin);
    print_str(" roast="); print_int(roast);
    print_str(" stock="); print_int(stock); print_str("lb\n");
    return cc.n_beans - 1;
}

int cc_add_drink(int type, double price, double cost, int beans_oz) {
    if (cc.n_drinks >= MAX_DRINKS) return -1;
    drink_t* d = &drinks[cc.n_drinks];
    d->drink_id = cc.n_drinks;
    d->type = type;
    d->price = price;
    d->cost = cost;
    d->beans_oz = beans_oz;
    d->sold = 0;
    d->revenue = 0.0;
    d->active = 1;
    cc.n_drinks++;
    print_str("[CC] Drink "); print_int(cc.n_drinks - 1);
    print_str(" type="); print_int(type);
    print_str(" $"); print_int((int)price); print_str("\n");
    return cc.n_drinks - 1;
}

int cc_add_food(int type, double price, double cost, int stock) {
    if (cc.n_food >= MAX_FOOD) return -1;
    food_t* f = &foods[cc.n_food];
    f->food_id = cc.n_food;
    f->type = type;
    f->price = price;
    f->cost = cost;
    f->stock = stock;
    f->sold = 0;
    f->revenue = 0.0;
    f->active = 1;
    cc.n_food++;
    print_str("[CC] Food "); print_int(cc.n_food - 1);
    print_str(" type="); print_int(type);
    print_str(" $"); print_int((int)price);
    print_str(" stock="); print_int(stock); print_str("\n");
    return cc.n_food - 1;
}

int cc_register_member(double initial_load) {
    if (cc.n_members >= MAX_MEMBERS) return -1;
    member_t* m = &members[cc.n_members];
    m->member_id = cc.n_members;
    m->balance = initial_load;
    m->points = 0;
    m->free_drinks = 0;
    m->total_spent = initial_load;
    m->n_visits = 0;
    m->active = 1;
    cc.member_loads += initial_load;
    cc.total_revenue += initial_load;
    cc.n_members++;
    print_str("[CC] Member "); print_int(cc.n_members - 1);
    print_str(" load=$"); print_int((int)initial_load); print_str("\n");
    return cc.n_members - 1;
}

int cc_sell_drink(int drink_id, int store_id, int member_id) {
    if (drink_id >= cc.n_drinks || store_id >= cc.n_stores) return -1;
    drink_t* d = &drinks[drink_id];
    d->sold++;
    d->revenue += d->price;
    stores[store_id].total_drinks++;
    cc.drink_revenue += d->price;
    cc.total_revenue += d->price;
    cc.total_drinks_sold++;
    int beans_needed = d->beans_oz / 16;
    if (member_id >= 0 && member_id < cc.n_members) {
        member_t* m = &members[member_id];
        m->balance -= d->price;
        m->total_spent += d->price;
        m->points += (int)d->price;
        if (m->points >= 100) {
            m->points -= 100;
            m->free_drinks++;
        }
        m->n_visits++;
    }
    print_str("[CC] Drink Dr"); print_int(drink_id);
    print_str(" S"); print_int(store_id);
    if (member_id >= 0) { print_str(" M"); print_int(member_id); }
    print_str(" $"); print_int((int)d->price); print_str("\n");
    return 0;
}

int cc_sell_food(int food_id, int store_id) {
    if (food_id >= cc.n_food || store_id >= cc.n_stores) return -1;
    food_t* f = &foods[food_id];
    if (f->stock <= 0) return -2;
    f->stock--;
    f->sold++;
    f->revenue += f->price;
    stores[store_id].total_food++;
    cc.food_revenue += f->price;
    cc.total_revenue += f->price;
    cc.total_food_sold++;
    print_str("[CC] Food F"); print_int(food_id);
    print_str(" S"); print_int(store_id);
    print_str(" $"); print_int((int)f->price); print_str("\n");
    return 0;
}

int cc_use_beans(int bean_id, int lbs) {
    if (bean_id >= cc.n_beans) return -1;
    bean_t* b = &beans[bean_id];
    if (b->stock_lbs < lbs) return -2;
    b->stock_lbs -= lbs;
    b->n_used += lbs;
    double cost = b->cost_per_lb * lbs;
    b->total_cost += cost;
    cc.bean_costs += cost;
    print_str("[CC] Use B"); print_int(bean_id);
    print_str(" lbs="); print_int(lbs);
    print_str(" $"); print_int((int)cost); print_str("\n");
    return 0;
}

int cc_member_load(int member_id, double amount) {
    if (member_id >= cc.n_members) return -1;
    member_t* m = &members[member_id];
    m->balance += amount;
    m->total_spent += amount;
    cc.member_loads += amount;
    cc.total_revenue += amount;
    print_str("[CC] M"); print_int(member_id);
    print_str(" load $"); print_int((int)amount); print_str("\n");
    return 0;
}

int cc_add_merch(double amount) {
    cc.merch_revenue += amount;
    cc.total_revenue += amount;
    print_str("[CC] Merch +$"); print_int((int)amount); print_str("\n");
    return 0;
}

void cc_store_report(void) {
    print_str("[CC] Store report:\n");
    for (int i = 0; i < cc.n_stores; i++) {
        store_t* s = &stores[i];
        print_str("  S"); print_int(i);
        print_str(" loc="); print_int(s->location);
        print_str(" emp="); print_int(s->n_employees);
        print_str(" drinks="); print_int(s->total_drinks);
        print_str(" food="); print_int(s->total_food);
        print_str(" rev="); print_int((int)s->daily_revenue); print_str("/day\n");
    }
}

void cc_financial_report(void) {
    print_str("[CC] Financial report:\n");
    print_str("  Drink revenue: "); print_int((int)cc.drink_revenue); print_str("\n");
    print_str("  Food revenue: "); print_int((int)cc.food_revenue); print_str("\n");
    print_str("  Merch revenue: "); print_int((int)cc.merch_revenue); print_str("\n");
    print_str("  Member loads: "); print_int((int)cc.member_loads); print_str("\n");
    print_str("  Total revenue: "); print_int((int)cc.total_revenue); print_str("\n");
    print_str("  Bean costs: "); print_int((int)cc.bean_costs); print_str("\n");
    print_str("  Labor costs: "); print_int((int)cc.labor_costs); print_str("\n");
    print_str("  Rent costs: "); print_int((int)cc.rent_costs); print_str("\n");
    double total_costs = cc.bean_costs + cc.labor_costs + cc.rent_costs;
    double profit = cc.total_revenue - total_costs;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    print_str("  Drinks sold: "); print_int(cc.total_drinks_sold);
    print_str(" Food sold: "); print_int(cc.total_food_sold); print_str("\n");
}

void cc_print_state(void) {
    print_str("[CC] Stores="); print_int(cc.n_stores);
    print_str(" Beans="); print_int(cc.n_beans);
    print_str(" Drinks="); print_int(cc.n_drinks);
    print_str(" Food="); print_int(cc.n_food);
    print_str(" Members="); print_int(cc.n_members);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)cc.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Coffee Chain Demo ===\n\n");
    cc_init();

    print_str("Adding stores...\n");
    cc_add_store(1, 6, 20, 1800.0, 3500.0);
    cc_add_store(2, 5, 15, 1500.0, 3000.0);
    cc_add_store(3, 8, 30, 2200.0, 4500.0);
    cc_add_store(1, 6, 18, 1700.0, 3200.0);
    cc_add_store(2, 7, 25, 2000.0, 4000.0);
    cc_add_store(3, 5, 12, 1400.0, 2800.0);
    cc_add_store(1, 6, 22, 1900.0, 3800.0);
    cc_add_store(2, 5, 16, 1600.0, 3100.0);
    cc_add_store(3, 9, 35, 2500.0, 5000.0);
    cc_add_store(1, 6, 20, 1750.0, 3400.0);

    print_str("\nAdding beans...\n");
    cc_add_bean(1, 1, 200, 12.0);
    cc_add_bean(1, 2, 180, 14.0);
    cc_add_bean(2, 1, 150, 10.0);
    cc_add_bean(2, 2, 160, 11.0);
    cc_add_bean(3, 3, 120, 18.0);
    cc_add_bean(3, 1, 140, 15.0);
    cc_add_bean(4, 2, 100, 20.0);
    cc_add_bean(4, 3, 130, 22.0);

    print_str("\nAdding drinks...\n");
    cc_add_drink(1, 2.5, 0.8, 1);
    cc_add_drink(1, 3.0, 1.0, 1);
    cc_add_drink(2, 4.5, 1.5, 2);
    cc_add_drink(2, 5.0, 1.8, 2);
    cc_add_drink(3, 4.0, 1.2, 1);
    cc_add_drink(3, 4.5, 1.5, 2);
    cc_add_drink(4, 5.5, 2.0, 2);
    cc_add_drink(4, 6.0, 2.2, 3);
    cc_add_drink(5, 3.5, 1.0, 1);
    cc_add_drink(5, 4.0, 1.2, 1);
    cc_add_drink(6, 5.0, 1.8, 2);
    cc_add_drink(6, 5.5, 2.0, 2);
    cc_add_drink(7, 4.5, 1.5, 2);
    cc_add_drink(7, 5.0, 1.8, 2);

    print_str("\nAdding food...\n");
    cc_add_food(1, 3.5, 1.2, 50);
    cc_add_food(1, 4.0, 1.5, 40);
    cc_add_food(2, 5.0, 2.0, 30);
    cc_add_food(2, 6.0, 2.5, 25);
    cc_add_food(3, 4.5, 1.8, 35);
    cc_add_food(3, 5.5, 2.2, 28);
    cc_add_food(4, 3.0, 1.0, 45);
    cc_add_food(4, 3.5, 1.3, 38);
    cc_add_food(5, 7.0, 3.0, 20);
    cc_add_food(5, 8.0, 3.5, 15);

    print_str("\nUsing beans...\n");
    for (int i = 0; i < 8; i++) {
        cc_use_beans(i, 10 + (i * 3));
    }

    print_str("\nRegistering members...\n");
    for (int i = 0; i < 16; i++) {
        cc_register_member(20.0 + i * 5.0);
    }

    print_str("\nSelling drinks...\n");
    for (int i = 0; i < 80; i++) {
        cc_sell_drink(i % 14, i % 10, i % 16);
    }

    print_str("\nSelling food...\n");
    for (int i = 0; i < 40; i++) {
        cc_sell_food(i % 10, i % 10);
    }

    print_str("\nMember loads...\n");
    for (int i = 0; i < 16; i++) {
        cc_member_load(i, 10.0 + i * 3.0);
    }

    print_str("\nMerchandise...\n");
    cc_add_merch(3000.0);
    cc_add_merch(2500.0);
    cc_add_merch(3500.0);
    cc_add_merch(2800.0);
    cc_add_merch(4000.0);

    print_str("\nStore report...\n");
    cc_store_report();

    print_str("\nFinancial report...\n");
    cc_financial_report();

    print_str("\nFinal state...\n");
    cc_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
