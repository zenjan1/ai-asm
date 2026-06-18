/* catering_service: Catering management (v1.0)
 * Menus, events, clients, staff, ingredients, equipment, finance
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

#define MAX_MENUS     12
#define MAX_EVENTS    14
#define MAX_CLIENTS   10
#define MAX_STAFF     10
#define MAX_INGREDIENTS 14

typedef struct {
    int    menu_id;
    int    type;
    double price_per_person;
    double cost_per_person;
    int    n_items;
    int    n_booked;
    double revenue;
    int    active;
} menu_t;

typedef struct {
    int    event_id;
    int    client_id;
    int    menu_id;
    int    day;
    int    n_guests;
    double total;
    int    status;
    int    active;
} event_t;

typedef struct {
    int    client_id;
    int    type;
    int    n_events;
    double total_spent;
    int    active;
} client_t;

typedef struct {
    int    staff_id;
    int    role;
    int    n_assignments;
    double total_earned;
    int    rating;
    int    active;
} staff_t;

typedef struct {
    int    ingredient_id;
    int    type;
    double cost_per_kg;
    int    stock_kg;
    int    n_used;
    double total_cost;
    int    active;
} ingredient_t;

typedef struct {
    int    n_menus;
    int    n_events;
    int    n_clients;
    int    n_staff;
    int    n_ingredients;
    double event_revenue;
    double ingredient_cost;
    double staff_cost;
    double total_revenue;
    double total_cost;
    int    total_guests;
} cs_state_t;

static menu_t menus[MAX_MENUS];
static event_t events[MAX_EVENTS];
static client_t clients[MAX_CLIENTS];
static staff_t staff_arr[MAX_STAFF];
static ingredient_t ingredients[MAX_INGREDIENTS];
static cs_state_t cs;

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

int cs_init(void) {
    if (initialized) return -1;
    cs.n_menus = 0; cs.n_events = 0; cs.n_clients = 0;
    cs.n_staff = 0; cs.n_ingredients = 0;
    cs.event_revenue = 0.0; cs.ingredient_cost = 0.0;
    cs.staff_cost = 0.0; cs.total_revenue = 0.0;
    cs.total_cost = 0.0; cs.total_guests = 0;
    for (int i = 0; i < MAX_MENUS; i++) menus[i].active = 0;
    for (int i = 0; i < MAX_EVENTS; i++) events[i].active = 0;
    for (int i = 0; i < MAX_CLIENTS; i++) clients[i].active = 0;
    for (int i = 0; i < MAX_STAFF; i++) staff_arr[i].active = 0;
    for (int i = 0; i < MAX_INGREDIENTS; i++) ingredients[i].active = 0;
    initialized = 1;
    print_str("[CS] Catering service initialized\n");
    return 0;
}

int cs_add_menu(int type, double price, double cost, int n_items) {
    if (cs.n_menus >= MAX_MENUS) return -1;
    menu_t* m = &menus[cs.n_menus];
    m->menu_id = cs.n_menus;
    m->type = type;
    m->price_per_person = price;
    m->cost_per_person = cost;
    m->n_items = n_items;
    m->n_booked = 0;
    m->revenue = 0.0;
    m->active = 1;
    cs.n_menus++;
    print_str("[CS] Menu "); print_int(cs.n_menus - 1);
    print_str(" type="); print_int(type);
    print_str(" price/pp="); print_int((int)price);
    print_str(" cost/pp="); print_int((int)cost); print_str("\n");
    return cs.n_menus - 1;
}

int cs_register_client(int type) {
    if (cs.n_clients >= MAX_CLIENTS) return -1;
    client_t* c = &clients[cs.n_clients];
    c->client_id = cs.n_clients;
    c->type = type;
    c->n_events = 0;
    c->total_spent = 0.0;
    c->active = 1;
    cs.n_clients++;
    print_str("[CS] Client "); print_int(cs.n_clients - 1);
    print_str(" type="); print_int(type); print_str("\n");
    return cs.n_clients - 1;
}

int cs_add_staff(int role, int rating) {
    if (cs.n_staff >= MAX_STAFF) return -1;
    staff_t* s = &staff_arr[cs.n_staff];
    s->staff_id = cs.n_staff;
    s->role = role;
    s->n_assignments = 0;
    s->total_earned = 0.0;
    s->rating = rating;
    s->active = 1;
    cs.n_staff++;
    print_str("[CS] Staff "); print_int(cs.n_staff - 1);
    print_str(" role="); print_int(role);
    print_str(" rating="); print_int(rating); print_str("\n");
    return cs.n_staff - 1;
}

int cs_add_ingredient(int type, double cost, int stock) {
    if (cs.n_ingredients >= MAX_INGREDIENTS) return -1;
    ingredient_t* ig = &ingredients[cs.n_ingredients];
    ig->ingredient_id = cs.n_ingredients;
    ig->type = type;
    ig->cost_per_kg = cost;
    ig->stock_kg = stock;
    ig->n_used = 0;
    ig->total_cost = 0.0;
    ig->active = 1;
    cs.n_ingredients++;
    print_str("[CS] Ingredient "); print_int(cs.n_ingredients - 1);
    print_str(" type="); print_int(type);
    print_str(" cost/kg="); print_int((int)cost);
    print_str(" stock="); print_int(stock); print_str("kg\n");
    return cs.n_ingredients - 1;
}

int cs_book_event(int client_id, int menu_id, int day, int n_guests) {
    if (cs.n_events >= MAX_EVENTS) return -1;
    if (client_id >= cs.n_clients || menu_id >= cs.n_menus) return -2;
    menu_t* m = &menus[menu_id];
    event_t* e = &events[cs.n_events];
    e->event_id = cs.n_events;
    e->client_id = client_id;
    e->menu_id = menu_id;
    e->day = day;
    e->n_guests = n_guests;
    double discount = (clients[client_id].type >= 2) ? 0.9 : 1.0;
    e->total = m->price_per_person * n_guests * discount;
    e->status = 1;
    e->active = 1;
    m->n_booked++;
    m->revenue += e->total;
    clients[client_id].n_events++;
    clients[client_id].total_spent += e->total;
    cs.event_revenue += e->total;
    cs.total_revenue += e->total;
    cs.total_guests += n_guests;
    cs.n_events++;
    print_str("[CS] Event "); print_int(cs.n_events - 1);
    print_str(" Cl"); print_int(client_id);
    print_str(" M"); print_int(menu_id);
    print_str(" day="); print_int(day);
    print_str(" guests="); print_int(n_guests);
    print_str(" total="); print_int((int)e->total); print_str("\n");
    return cs.n_events - 1;
}

int cs_assign_staff(int event_id, int staff_id) {
    if (event_id >= cs.n_events || staff_id >= cs.n_staff) return -1;
    staff_arr[staff_id].n_assignments++;
    double pay = 50.0;
    staff_arr[staff_id].total_earned += pay;
    cs.staff_cost += pay;
    cs.total_cost += pay;
    print_str("[CS] Staff St"); print_int(staff_id);
    print_str(" assigned to E"); print_int(event_id);
    print_str(" pay="); print_int((int)pay); print_str("\n");
    return 0;
}

int cs_use_ingredient(int ingredient_id, int kg, int event_id) {
    if (ingredient_id >= cs.n_ingredients || event_id >= cs.n_events) return -1;
    ingredient_t* ig = &ingredients[ingredient_id];
    if (ig->stock_kg < kg) return -2;
    ig->stock_kg -= kg;
    ig->n_used++;
    double cost = ig->cost_per_kg * kg;
    ig->total_cost += cost;
    cs.ingredient_cost += cost;
    cs.total_cost += cost;
    print_str("[CS] Ingredient Ig"); print_int(ingredient_id);
    print_str(" used "); print_int(kg); print_str("kg for E"); print_int(event_id);
    print_str(" cost="); print_int((int)cost); print_str("\n");
    return 0;
}

void cs_menu_report(void) {
    print_str("[CS] Menu report:\n");
    for (int i = 0; i < cs.n_menus; i++) {
        menu_t* m = &menus[i];
        print_str("  M"); print_int(i);
        print_str(" type="); print_int(m->type);
        print_str(" price/pp="); print_int((int)m->price_per_person);
        print_str(" booked="); print_int(m->n_booked);
        print_str(" rev="); print_int((int)m->revenue); print_str("\n");
    }
}

void cs_staff_report(void) {
    print_str("[CS] Staff report:\n");
    for (int i = 0; i < cs.n_staff; i++) {
        staff_t* s = &staff_arr[i];
        print_str("  St"); print_int(i);
        print_str(" role="); print_int(s->role);
        print_str(" assign="); print_int(s->n_assignments);
        print_str(" earned="); print_int((int)s->total_earned);
        print_str(" rating="); print_int(s->rating); print_str("\n");
    }
}

void cs_financial_report(void) {
    print_str("[CS] Financial report:\n");
    print_str("  Event revenue: "); print_int((int)cs.event_revenue); print_str("\n");
    print_str("  Ingredient cost: "); print_int((int)cs.ingredient_cost); print_str("\n");
    print_str("  Staff cost: "); print_int((int)cs.staff_cost); print_str("\n");
    print_str("  Total cost: "); print_int((int)cs.total_cost); print_str("\n");
    double profit = cs.total_revenue - cs.total_cost;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    print_str("  Total guests served: "); print_int(cs.total_guests); print_str("\n");
}

void cs_print_state(void) {
    print_str("[CS] Menus="); print_int(cs.n_menus);
    print_str(" Events="); print_int(cs.n_events);
    print_str(" Clients="); print_int(cs.n_clients);
    print_str(" Staff="); print_int(cs.n_staff);
    print_str(" Ingredients="); print_int(cs.n_ingredients);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)cs.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Catering Service Demo ===\n\n");
    cs_init();

    print_str("Adding menus...\n");
    cs_add_menu(1, 45.0, 18.0, 5);
    cs_add_menu(2, 65.0, 25.0, 7);
    cs_add_menu(3, 85.0, 35.0, 10);
    cs_add_menu(1, 35.0, 14.0, 4);
    cs_add_menu(2, 55.0, 22.0, 6);
    cs_add_menu(3, 95.0, 40.0, 12);
    cs_add_menu(1, 40.0, 16.0, 5);
    cs_add_menu(2, 70.0, 28.0, 8);
    cs_add_menu(3, 110.0, 45.0, 14);
    cs_add_menu(1, 50.0, 20.0, 6);
    cs_add_menu(2, 60.0, 24.0, 7);
    cs_add_menu(3, 80.0, 32.0, 9);

    print_str("\nRegistering clients...\n");
    for (int i = 0; i < 8; i++) {
        cs_register_client(i % 3);
    }

    print_str("\nAdding staff...\n");
    cs_add_staff(1, 92);
    cs_add_staff(1, 88);
    cs_add_staff(2, 90);
    cs_add_staff(2, 85);
    cs_add_staff(3, 87);
    cs_add_staff(3, 91);
    cs_add_staff(1, 93);
    cs_add_staff(2, 86);
    cs_add_staff(3, 89);
    cs_add_staff(1, 94);

    print_str("\nAdding ingredients...\n");
    cs_add_ingredient(1, 8.0, 200);
    cs_add_ingredient(1, 12.0, 150);
    cs_add_ingredient(2, 5.0, 300);
    cs_add_ingredient(2, 7.0, 250);
    cs_add_ingredient(3, 15.0, 100);
    cs_add_ingredient(3, 20.0, 80);
    cs_add_ingredient(4, 3.0, 400);
    cs_add_ingredient(4, 4.0, 350);
    cs_add_ingredient(1, 10.0, 180);
    cs_add_ingredient(2, 6.0, 280);
    cs_add_ingredient(3, 18.0, 90);
    cs_add_ingredient(4, 5.0, 320);
    cs_add_ingredient(1, 9.0, 220);
    cs_add_ingredient(2, 8.0, 260);

    print_str("\nBooking events...\n");
    cs_book_event(0, 0, 20, 50);
    cs_book_event(1, 1, 22, 80);
    cs_book_event(2, 2, 25, 120);
    cs_book_event(3, 3, 27, 40);
    cs_book_event(4, 4, 30, 100);
    cs_book_event(5, 5, 32, 60);
    cs_book_event(6, 6, 35, 75);
    cs_book_event(7, 7, 37, 90);
    cs_book_event(0, 8, 40, 150);
    cs_book_event(1, 9, 42, 55);
    cs_book_event(2, 10, 45, 70);
    cs_book_event(3, 11, 47, 85);
    cs_book_event(4, 0, 50, 45);
    cs_book_event(5, 1, 52, 95);

    print_str("\nAssigning staff...\n");
    for (int i = 0; i < 14; i++) {
        cs_assign_staff(i, i % cs.n_staff);
    }
    cs_assign_staff(0, 1);
    cs_assign_staff(2, 3);
    cs_assign_staff(4, 5);
    cs_assign_staff(6, 7);

    print_str("\nUsing ingredients...\n");
    cs_use_ingredient(0, 20, 0);
    cs_use_ingredient(1, 30, 1);
    cs_use_ingredient(2, 40, 2);
    cs_use_ingredient(3, 25, 3);
    cs_use_ingredient(4, 15, 4);
    cs_use_ingredient(5, 10, 5);
    cs_use_ingredient(6, 35, 6);
    cs_use_ingredient(7, 28, 7);
    cs_use_ingredient(8, 22, 8);
    cs_use_ingredient(9, 18, 9);
    cs_use_ingredient(10, 12, 10);
    cs_use_ingredient(11, 30, 11);
    cs_use_ingredient(12, 20, 12);
    cs_use_ingredient(13, 25, 13);

    print_str("\nMenu report...\n");
    cs_menu_report();

    print_str("\nStaff report...\n");
    cs_staff_report();

    print_str("\nFinancial report...\n");
    cs_financial_report();

    print_str("\nFinal state...\n");
    cs_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
