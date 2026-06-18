/* water_park: Water park management (v1.0)
 * Rides, locker rooms, food, lifeguards, tickets, revenue
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

#define MAX_RIDES     10
#define MAX_LOCKERS   8
#define MAX_FOOD      8
#define MAX_GUARDS    10
#define MAX_TICKETS   16

typedef struct {
    int    ride_id;
    int    type;
    int    capacity;
    int    height_req;
    int    status;
    int    n_riders;
    double maint_cost;
    int    active;
} ride_t;

typedef struct {
    int    locker_id;
    int    size;
    double daily_rate;
    int    rented;
    double revenue;
    int    active;
} locker_t;

typedef struct {
    int    food_id;
    int    type;
    double price;
    double cost;
    int    n_sold;
    double revenue;
    int    active;
} food_t;

typedef struct {
    int    guard_id;
    int    zone;
    double hourly_rate;
    int    hours;
    double total_pay;
    int    n_rescues;
    int    active;
} guard_t;

typedef struct {
    int    ticket_id;
    int    type;
    double price;
    int    day;
    int    active;
} ticket_t;

typedef struct {
    int    n_rides;
    int    n_lockers;
    int    n_food;
    int    n_guards;
    int    n_tickets;
    double ticket_revenue;
    double food_revenue;
    double locker_revenue;
    double merch_revenue;
    double total_revenue;
    double total_costs;
    int    total_visitors;
    int    total_riders;
} wp_state_t;

static ride_t rides[MAX_RIDES];
static locker_t lockers[MAX_LOCKERS];
static food_t foods[MAX_FOOD];
static guard_t guards[MAX_GUARDS];
static ticket_t tickets[MAX_TICKETS];
static wp_state_t wp;

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

int wp_init(void) {
    if (initialized) return -1;
    wp.n_rides = 0; wp.n_lockers = 0; wp.n_food = 0;
    wp.n_guards = 0; wp.n_tickets = 0;
    wp.ticket_revenue = 0.0; wp.food_revenue = 0.0;
    wp.locker_revenue = 0.0; wp.merch_revenue = 0.0;
    wp.total_revenue = 0.0; wp.total_costs = 0.0;
    wp.total_visitors = 0; wp.total_riders = 0;
    for (int i = 0; i < MAX_RIDES; i++) rides[i].active = 0;
    for (int i = 0; i < MAX_LOCKERS; i++) lockers[i].active = 0;
    for (int i = 0; i < MAX_FOOD; i++) foods[i].active = 0;
    for (int i = 0; i < MAX_GUARDS; i++) guards[i].active = 0;
    for (int i = 0; i < MAX_TICKETS; i++) tickets[i].active = 0;
    initialized = 1;
    print_str("[WP] Water park initialized\n");
    return 0;
}

int wp_add_ride(int type, int capacity, int height_req) {
    if (wp.n_rides >= MAX_RIDES) return -1;
    ride_t* r = &rides[wp.n_rides];
    r->ride_id = wp.n_rides;
    r->type = type;
    r->capacity = capacity;
    r->height_req = height_req;
    r->status = 1;
    r->n_riders = 0;
    r->maint_cost = 0.0;
    r->active = 1;
    wp.n_rides++;
    print_str("[WP] Ride "); print_int(wp.n_rides - 1);
    print_str(" type="); print_int(type);
    print_str(" cap="); print_int(capacity);
    print_str(" ht="); print_int(height_req); print_str("cm\n");
    return wp.n_rides - 1;
}

int wp_add_locker(int size, double rate) {
    if (wp.n_lockers >= MAX_LOCKERS) return -1;
    locker_t* l = &lockers[wp.n_lockers];
    l->locker_id = wp.n_lockers;
    l->size = size;
    l->daily_rate = rate;
    l->rented = 0;
    l->revenue = 0.0;
    l->active = 1;
    wp.n_lockers++;
    print_str("[WP] Locker "); print_int(wp.n_lockers - 1);
    print_str(" sz="); print_int(size);
    print_str(" $"); print_int((int)rate); print_str("/day\n");
    return wp.n_lockers - 1;
}

int wp_add_food(int type, double price, double cost) {
    if (wp.n_food >= MAX_FOOD) return -1;
    food_t* f = &foods[wp.n_food];
    f->food_id = wp.n_food;
    f->type = type;
    f->price = price;
    f->cost = cost;
    f->n_sold = 0;
    f->revenue = 0.0;
    f->active = 1;
    wp.n_food++;
    print_str("[WP] Food "); print_int(wp.n_food - 1);
    print_str(" type="); print_int(type);
    print_str(" $"); print_int((int)price); print_str("\n");
    return wp.n_food - 1;
}

int wp_add_guard(int zone, double rate, int hours) {
    if (wp.n_guards >= MAX_GUARDS) return -1;
    guard_t* g = &guards[wp.n_guards];
    g->guard_id = wp.n_guards;
    g->zone = zone;
    g->hourly_rate = rate;
    g->hours = hours;
    g->total_pay = rate * hours;
    g->n_rescues = 0;
    g->active = 1;
    wp.total_costs += g->total_pay;
    wp.n_guards++;
    print_str("[WP] Guard "); print_int(wp.n_guards - 1);
    print_str(" Z"); print_int(zone);
    print_str(" $"); print_int((int)rate); print_str("/hr");
    print_str(" hrs="); print_int(hours); print_str("\n");
    return wp.n_guards - 1;
}

int wp_sell_ticket(int type, double price, int day) {
    if (wp.n_tickets >= MAX_TICKETS) return -1;
    ticket_t* t = &tickets[wp.n_tickets];
    t->ticket_id = wp.n_tickets;
    t->type = type;
    t->price = price;
    t->day = day;
    t->active = 1;
    wp.ticket_revenue += price;
    wp.total_revenue += price;
    wp.total_visitors++;
    wp.n_tickets++;
    print_str("[WP] Ticket "); print_int(wp.n_tickets - 1);
    print_str(" type="); print_int(type);
    print_str(" $"); print_int((int)price);
    print_str(" D"); print_int(day); print_str("\n");
    return wp.n_tickets - 1;
}

int wp_ride_riders(int ride_id, int count) {
    if (ride_id >= wp.n_rides) return -1;
    rides[ride_id].n_riders += count;
    wp.total_riders += count;
    print_str("[WP] R"); print_int(ride_id);
    print_str(" riders +"); print_int(count); print_str("\n");
    return 0;
}

int wp_rent_locker(int locker_id) {
    if (locker_id >= wp.n_lockers) return -1;
    locker_t* l = &lockers[locker_id];
    l->rented = 1;
    l->revenue += l->daily_rate;
    wp.locker_revenue += l->daily_rate;
    wp.total_revenue += l->daily_rate;
    print_str("[WP] Locker "); print_int(locker_id);
    print_str(" rented $"); print_int((int)l->daily_rate); print_str("\n");
    return 0;
}

int wp_sell_food(int food_id, int count) {
    if (food_id >= wp.n_food) return -1;
    food_t* f = &foods[food_id];
    f->n_sold += count;
    double rev = f->price * count;
    double cost = f->cost * count;
    f->revenue += rev;
    wp.food_revenue += rev;
    wp.total_revenue += rev;
    wp.total_costs += cost;
    print_str("[WP] F"); print_int(food_id);
    print_str(" sold "); print_int(count);
    print_str(" $"); print_int((int)rev); print_str("\n");
    return 0;
}

int wp_add_merch(double amount) {
    wp.merch_revenue += amount;
    wp.total_revenue += amount;
    print_str("[WP] Merch +$"); print_int((int)amount); print_str("\n");
    return 0;
}

int wp_guard_rescue(int guard_id) {
    if (guard_id >= wp.n_guards) return -1;
    guards[guard_id].n_rescues++;
    print_str("[WP] G"); print_int(guard_id);
    print_str(" rescue #"); print_int(guards[guard_id].n_rescues); print_str("\n");
    return 0;
}

void wp_ride_report(void) {
    print_str("[WP] Ride report:\n");
    for (int i = 0; i < wp.n_rides; i++) {
        ride_t* r = &rides[i];
        print_str("  R"); print_int(i);
        print_str(" type="); print_int(r->type);
        print_str(" cap="); print_int(r->capacity);
        print_str(" riders="); print_int(r->n_riders);
        print_str(" status="); print_int(r->status); print_str("\n");
    }
}

void wp_financial_report(void) {
    print_str("[WP] Financial report:\n");
    print_str("  Ticket revenue: "); print_int((int)wp.ticket_revenue); print_str("\n");
    print_str("  Food revenue: "); print_int((int)wp.food_revenue); print_str("\n");
    print_str("  Locker revenue: "); print_int((int)wp.locker_revenue); print_str("\n");
    print_str("  Merch revenue: "); print_int((int)wp.merch_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)wp.total_revenue); print_str("\n");
    print_str("  Costs: "); print_int((int)wp.total_costs); print_str("\n");
    double profit = wp.total_revenue - wp.total_costs;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    print_str("  Visitors: "); print_int(wp.total_visitors);
    print_str(" Riders: "); print_int(wp.total_riders); print_str("\n");
}

void wp_print_state(void) {
    print_str("[WP] Rides="); print_int(wp.n_rides);
    print_str(" Lockers="); print_int(wp.n_lockers);
    print_str(" Food="); print_int(wp.n_food);
    print_str(" Guards="); print_int(wp.n_guards);
    print_str(" Tickets="); print_int(wp.n_tickets);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)wp.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Water Park Demo ===\n\n");
    wp_init();

    print_str("Adding rides...\n");
    wp_add_ride(1, 4, 120);
    wp_add_ride(1, 2, 140);
    wp_add_ride(2, 6, 110);
    wp_add_ride(2, 8, 100);
    wp_add_ride(3, 10, 0);
    wp_add_ride(3, 12, 0);
    wp_add_ride(4, 4, 130);
    wp_add_ride(1, 3, 125);
    wp_add_ride(4, 6, 115);
    wp_add_ride(2, 5, 105);

    print_str("\nAdding lockers...\n");
    wp_add_locker(1, 8.0);
    wp_add_locker(1, 8.0);
    wp_add_locker(2, 12.0);
    wp_add_locker(2, 12.0);
    wp_add_locker(3, 18.0);
    wp_add_locker(3, 18.0);
    wp_add_locker(1, 8.0);
    wp_add_locker(2, 12.0);

    print_str("\nAdding food...\n");
    wp_add_food(1, 8.0, 3.0);
    wp_add_food(1, 10.0, 4.0);
    wp_add_food(2, 5.0, 1.5);
    wp_add_food(2, 6.0, 2.0);
    wp_add_food(3, 4.0, 1.0);
    wp_add_food(3, 5.0, 1.5);
    wp_add_food(4, 12.0, 5.0);
    wp_add_food(4, 15.0, 6.0);

    print_str("\nAdding lifeguards...\n");
    wp_add_guard(1, 20.0, 8);
    wp_add_guard(1, 20.0, 8);
    wp_add_guard(2, 20.0, 8);
    wp_add_guard(2, 20.0, 8);
    wp_add_guard(3, 22.0, 8);
    wp_add_guard(3, 22.0, 8);
    wp_add_guard(4, 20.0, 8);
    wp_add_guard(4, 20.0, 8);
    wp_add_guard(1, 25.0, 6);
    wp_add_guard(2, 25.0, 6);

    print_str("\nSelling tickets...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 4) + 1;
        double price;
        if (type == 1) price = 45.0;
        else if (type == 2) price = 30.0;
        else if (type == 3) price = 120.0;
        else price = 35.0;
        wp_sell_ticket(type, price, (i % 3) + 1);
    }

    print_str("\nRide activity...\n");
    for (int i = 0; i < 10; i++) {
        wp_ride_riders(i, 100 + i * 20);
    }

    print_str("\nRenting lockers...\n");
    for (int i = 0; i < 8; i++) {
        wp_rent_locker(i);
    }

    print_str("\nSelling food...\n");
    wp_sell_food(0, 150);
    wp_sell_food(1, 120);
    wp_sell_food(2, 200);
    wp_sell_food(3, 180);
    wp_sell_food(4, 250);
    wp_sell_food(5, 220);
    wp_sell_food(6, 80);
    wp_sell_food(7, 60);

    print_str("\nMerchandise...\n");
    wp_add_merch(5000.0);
    wp_add_merch(3500.0);
    wp_add_merch(4200.0);

    print_str("\nGuard rescues...\n");
    wp_guard_rescue(0);
    wp_guard_rescue(2);
    wp_guard_rescue(4);
    wp_guard_rescue(6);
    wp_guard_rescue(0);
    wp_guard_rescue(8);

    print_str("\nRide report...\n");
    wp_ride_report();

    print_str("\nFinancial report...\n");
    wp_financial_report();

    print_str("\nFinal state...\n");
    wp_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
