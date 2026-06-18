/* amusement_park: Amusement park management (v1.0)
 * Rides, ticketing, visitors, operations, concessions
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

#define MAX_RIDES      10
#define MAX_TICKETS    18
#define MAX_VISITORS   14
#define MAX_MEMBERS    10
#define MAX_SHOPS      8
#define MAX_MAINTENANCE 10

typedef struct {
    int    ride_id;
    int    type;
    int    capacity;
    int    duration_min;
    double price;
    int    status;
    int    n_rides;
    int    maintenance_day;
    int    active;
} ride_t;

typedef struct {
    int    ticket_id;
    int    visitor_id;
    int    type;
    double price;
    int    day;
    int    fast_pass;
    int    active;
} ticket_t;

typedef struct {
    int    visitor_id;
    int    group_size;
    int    n_rides;
    double total_spent;
    int    points;
    int    active;
} visitor_t;

typedef struct {
    int    member_id;
    int    tier;
    int    n_visits;
    double total_paid;
    int    points;
    int    active;
} member_t;

typedef struct {
    int    shop_id;
    int    type;
    double price;
    double cost;
    int    stock;
    int    n_sold;
    double revenue;
    int    active;
} shop_t;

typedef struct {
    int    maint_id;
    int    ride_id;
    int    type;
    int    day;
    double cost;
    int    completed;
    int    active;
} maintenance_t;

typedef struct {
    int    n_rides;
    int    n_tickets;
    int    n_visitors;
    int    n_members;
    int    n_shops;
    int    n_maintenance;
    double ticket_revenue;
    double shop_revenue;
    double total_revenue;
    double total_costs;
    int    total_rides_given;
} ap_state_t;

static ride_t rides[MAX_RIDES];
static ticket_t tickets[MAX_TICKETS];
static visitor_t visitors[MAX_VISITORS];
static member_t members[MAX_MEMBERS];
static shop_t shops[MAX_SHOPS];
static maintenance_t maintenance[MAX_MAINTENANCE];
static ap_state_t ap;

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

int ap_init(void) {
    if (initialized) return -1;
    ap.n_rides = 0; ap.n_tickets = 0; ap.n_visitors = 0;
    ap.n_members = 0; ap.n_shops = 0; ap.n_maintenance = 0;
    ap.ticket_revenue = 0.0; ap.shop_revenue = 0.0;
    ap.total_revenue = 0.0; ap.total_costs = 0.0;
    ap.total_rides_given = 0;
    for (int i = 0; i < MAX_RIDES; i++) rides[i].active = 0;
    for (int i = 0; i < MAX_TICKETS; i++) tickets[i].active = 0;
    for (int i = 0; i < MAX_VISITORS; i++) visitors[i].active = 0;
    for (int i = 0; i < MAX_MEMBERS; i++) members[i].active = 0;
    for (int i = 0; i < MAX_SHOPS; i++) shops[i].active = 0;
    for (int i = 0; i < MAX_MAINTENANCE; i++) maintenance[i].active = 0;
    initialized = 1;
    print_str("[AP] Amusement park initialized\n");
    return 0;
}

int ap_add_ride(int type, int capacity, int duration, double price) {
    if (ap.n_rides >= MAX_RIDES) return -1;
    ride_t* r = &rides[ap.n_rides];
    r->ride_id = ap.n_rides;
    r->type = type;
    r->capacity = capacity;
    r->duration_min = duration;
    r->price = price;
    r->status = 1;
    r->n_rides = 0;
    r->maintenance_day = 0;
    r->active = 1;
    ap.n_rides++;
    print_str("[AP] Ride "); print_int(ap.n_rides - 1);
    print_str(" type="); print_int(type);
    print_str(" cap="); print_int(capacity);
    print_str(" dur="); print_int(duration); print_str("min");
    print_str(" price="); print_int((int)price); print_str("\n");
    return ap.n_rides - 1;
}

int ap_register_member(int tier) {
    if (ap.n_members >= MAX_MEMBERS) return -1;
    member_t* m = &members[ap.n_members];
    m->member_id = ap.n_members;
    m->tier = tier;
    m->n_visits = 0;
    m->total_paid = 0.0;
    m->points = 0;
    m->active = 1;
    ap.n_members++;
    print_str("[AP] Member "); print_int(ap.n_members - 1);
    print_str(" tier="); print_int(tier); print_str("\n");
    return ap.n_members - 1;
}

int ap_register_visitor(int group_size) {
    if (ap.n_visitors >= MAX_VISITORS) return -1;
    visitor_t* v = &visitors[ap.n_visitors];
    v->visitor_id = ap.n_visitors;
    v->group_size = group_size;
    v->n_rides = 0;
    v->total_spent = 0.0;
    v->points = 0;
    v->active = 1;
    ap.n_visitors++;
    print_str("[AP] Visitor "); print_int(ap.n_visitors - 1);
    print_str(" group="); print_int(group_size); print_str("\n");
    return ap.n_visitors - 1;
}

int ap_add_shop(int type, double price, double cost, int stock) {
    if (ap.n_shops >= MAX_SHOPS) return -1;
    shop_t* s = &shops[ap.n_shops];
    s->shop_id = ap.n_shops;
    s->type = type;
    s->price = price;
    s->cost = cost;
    s->stock = stock;
    s->n_sold = 0;
    s->revenue = 0.0;
    s->active = 1;
    ap.n_shops++;
    print_str("[AP] Shop "); print_int(ap.n_shops - 1);
    print_str(" type="); print_int(type);
    print_str(" price="); print_int((int)price);
    print_str(" stock="); print_int(stock); print_str("\n");
    return ap.n_shops - 1;
}

int ap_sell_ticket(int visitor_id, int type, double price, int day, int fast_pass) {
    if (ap.n_tickets >= MAX_TICKETS || visitor_id >= ap.n_visitors) return -1;
    ticket_t* t = &tickets[ap.n_tickets];
    t->ticket_id = ap.n_tickets;
    t->visitor_id = visitor_id;
    t->type = type;
    t->price = price;
    t->day = day;
    t->fast_pass = fast_pass;
    t->active = 1;
    visitors[visitor_id].total_spent += price;
    visitors[visitor_id].points += (int)price;
    ap.ticket_revenue += price;
    ap.total_revenue += price;
    ap.n_tickets++;
    print_str("[AP] Ticket "); print_int(ap.n_tickets - 1);
    print_str(" V"); print_int(visitor_id);
    print_str(" type="); print_int(type);
    print_str(" price="); print_int((int)price);
    if (fast_pass) print_str(" FAST");
    print_str("\n");
    return ap.n_tickets - 1;
}

int ap_ride_ride(int ride_id, int visitor_id) {
    if (ride_id >= ap.n_rides || visitor_id >= ap.n_visitors) return -1;
    ride_t* r = &rides[ride_id];
    if (r->status != 1) return -2;
    r->n_rides++;
    visitors[visitor_id].n_rides++;
    visitors[visitor_id].total_spent += r->price;
    visitors[visitor_id].points += (int)(r->price * 0.5);
    ap.total_revenue += r->price;
    ap.total_rides_given++;
    print_str("[AP] V"); print_int(visitor_id);
    print_str(" rode R"); print_int(ride_id);
    print_str(" price="); print_int((int)r->price); print_str("\n");
    return 0;
}

int ap_sell_item(int shop_id, int visitor_id) {
    if (shop_id >= ap.n_shops || visitor_id >= ap.n_visitors) return -1;
    shop_t* s = &shops[shop_id];
    if (s->stock <= 0) return -2;
    s->stock--;
    s->n_sold++;
    s->revenue += s->price;
    visitors[visitor_id].total_spent += s->price;
    ap.shop_revenue += s->price;
    ap.total_revenue += s->price;
    ap.total_costs += s->cost;
    print_str("[AP] Sold SH"); print_int(shop_id);
    print_str(" to V"); print_int(visitor_id);
    print_str(" price="); print_int((int)s->price);
    print_str(" stock="); print_int(s->stock); print_str("\n");
    return 0;
}

int ap_schedule_maintenance(int ride_id, int type, int day, double cost) {
    if (ap.n_maintenance >= MAX_MAINTENANCE || ride_id >= ap.n_rides) return -1;
    maintenance_t* m = &maintenance[ap.n_maintenance];
    m->maint_id = ap.n_maintenance;
    m->ride_id = ride_id;
    m->type = type;
    m->day = day;
    m->cost = cost;
    m->completed = 0;
    m->active = 1;
    rides[ride_id].status = 2;
    rides[ride_id].maintenance_day = day;
    ap.total_costs += cost;
    ap.n_maintenance++;
    print_str("[AP] Maintenance "); print_int(ap.n_maintenance - 1);
    print_str(" R"); print_int(ride_id);
    print_str(" type="); print_int(type);
    print_str(" day="); print_int(day);
    print_str(" cost="); print_int((int)cost); print_str("\n");
    return ap.n_maintenance - 1;
}

void ap_complete_maintenance(int maint_id) {
    if (maint_id >= ap.n_maintenance) return;
    maintenance[maint_id].completed = 1;
    rides[maintenance[maint_id].ride_id].status = 1;
    print_str("[AP] Maintenance "); print_int(maint_id);
    print_str(" completed R"); print_int(maintenance[maint_id].ride_id); print_str("\n");
}

void ap_ride_stats(void) {
    print_str("[AP] Ride stats:\n");
    for (int i = 0; i < ap.n_rides; i++) {
        ride_t* r = &rides[i];
        print_str("  R"); print_int(i);
        print_str(" type="); print_int(r->type);
        print_str(" rides="); print_int(r->n_rides);
        print_str(" status="); print_int(r->status); print_str("\n");
    }
}

void ap_shop_stats(void) {
    print_str("[AP] Shop stats:\n");
    for (int i = 0; i < ap.n_shops; i++) {
        shop_t* s = &shops[i];
        print_str("  SH"); print_int(i);
        print_str(" type="); print_int(s->type);
        print_str(" sold="); print_int(s->n_sold);
        print_str(" rev="); print_int((int)s->revenue);
        print_str(" stock="); print_int(s->stock); print_str("\n");
    }
}

void ap_financial_report(void) {
    print_str("[AP] Financial report:\n");
    print_str("  Ticket revenue: "); print_int((int)ap.ticket_revenue); print_str("\n");
    print_str("  Shop revenue: "); print_int((int)ap.shop_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)ap.total_revenue); print_str("\n");
    print_str("  Total costs: "); print_int((int)ap.total_costs); print_str("\n");
    double profit = ap.total_revenue - ap.total_costs;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    print_str("  Total rides: "); print_int(ap.total_rides_given); print_str("\n");
}

void ap_print_state(void) {
    print_str("[AP] Rides="); print_int(ap.n_rides);
    print_str(" Tickets="); print_int(ap.n_tickets);
    print_str(" Visitors="); print_int(ap.n_visitors);
    print_str(" Members="); print_int(ap.n_members);
    print_str(" Shops="); print_int(ap.n_shops);
    print_str(" Maint="); print_int(ap.n_maintenance);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)ap.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Amusement Park Demo ===\n\n");
    ap_init();

    print_str("Adding rides...\n");
    ap_add_ride(1, 20, 3, 8.0);
    ap_add_ride(2, 12, 5, 12.0);
    ap_add_ride(3, 8, 2, 6.0);
    ap_add_ride(1, 16, 4, 10.0);
    ap_add_ride(4, 24, 3, 7.0);
    ap_add_ride(2, 10, 6, 15.0);
    ap_add_ride(3, 6, 2, 5.0);
    ap_add_ride(1, 18, 4, 9.0);

    print_str("\nRegistering members...\n");
    ap_register_member(0);
    ap_register_member(1);
    ap_register_member(2);
    ap_register_member(3);
    ap_register_member(1);
    ap_register_member(2);

    print_str("\nRegistering visitors...\n");
    ap_register_visitor(1);
    ap_register_visitor(2);
    ap_register_visitor(3);
    ap_register_visitor(4);
    ap_register_visitor(1);
    ap_register_visitor(2);
    ap_register_visitor(3);
    ap_register_visitor(5);
    ap_register_visitor(1);
    ap_register_visitor(2);

    print_str("\nAdding shops...\n");
    ap_add_shop(1, 5.0, 1.5, 50);
    ap_add_shop(2, 8.0, 2.5, 40);
    ap_add_shop(3, 12.0, 4.0, 30);
    ap_add_shop(1, 6.0, 2.0, 45);
    ap_add_shop(4, 15.0, 5.0, 25);
    ap_add_shop(2, 10.0, 3.0, 35);

    print_str("\nSelling tickets...\n");
    ap_sell_ticket(0, 1, 45.0, 10, 0);
    ap_sell_ticket(1, 1, 45.0, 10, 1);
    ap_sell_ticket(2, 2, 60.0, 10, 0);
    ap_sell_ticket(3, 1, 45.0, 10, 1);
    ap_sell_ticket(4, 2, 60.0, 11, 0);
    ap_sell_ticket(5, 1, 45.0, 11, 0);
    ap_sell_ticket(6, 3, 80.0, 11, 1);
    ap_sell_ticket(7, 2, 60.0, 12, 0);
    ap_sell_ticket(8, 1, 45.0, 12, 0);
    ap_sell_ticket(9, 2, 60.0, 12, 1);
    ap_sell_ticket(0, 1, 45.0, 13, 0);
    ap_sell_ticket(1, 3, 80.0, 13, 1);
    ap_sell_ticket(2, 2, 60.0, 13, 0);
    ap_sell_ticket(3, 1, 45.0, 14, 0);
    ap_sell_ticket(4, 2, 60.0, 14, 0);
    ap_sell_ticket(5, 3, 80.0, 14, 1);
    ap_sell_ticket(6, 1, 45.0, 15, 0);
    ap_sell_ticket(7, 2, 60.0, 15, 0);

    print_str("\nRiding rides...\n");
    ap_ride_ride(0, 0);
    ap_ride_ride(1, 0);
    ap_ride_ride(2, 1);
    ap_ride_ride(3, 1);
    ap_ride_ride(4, 2);
    ap_ride_ride(5, 2);
    ap_ride_ride(0, 3);
    ap_ride_ride(1, 3);
    ap_ride_ride(6, 4);
    ap_ride_ride(7, 4);
    ap_ride_ride(0, 5);
    ap_ride_ride(2, 5);
    ap_ride_ride(3, 6);
    ap_ride_ride(4, 6);
    ap_ride_ride(1, 7);
    ap_ride_ride(5, 7);
    ap_ride_ride(6, 8);
    ap_ride_ride(7, 8);
    ap_ride_ride(0, 9);
    ap_ride_ride(3, 9);

    print_str("\nSelling items...\n");
    ap_sell_item(0, 0);
    ap_sell_item(1, 1);
    ap_sell_item(2, 2);
    ap_sell_item(3, 3);
    ap_sell_item(4, 4);
    ap_sell_item(5, 5);
    ap_sell_item(0, 6);
    ap_sell_item(1, 7);
    ap_sell_item(2, 8);
    ap_sell_item(3, 9);
    ap_sell_item(4, 0);
    ap_sell_item(5, 1);

    print_str("\nScheduling maintenance...\n");
    ap_schedule_maintenance(0, 1, 20, 500.0);
    ap_schedule_maintenance(1, 2, 22, 800.0);
    ap_schedule_maintenance(2, 1, 25, 400.0);
    ap_schedule_maintenance(3, 2, 28, 700.0);

    print_str("\nCompleting maintenance...\n");
    ap_complete_maintenance(0);
    ap_complete_maintenance(1);
    ap_complete_maintenance(2);
    ap_complete_maintenance(3);

    print_str("\nRide stats...\n");
    ap_ride_stats();

    print_str("\nShop stats...\n");
    ap_shop_stats();

    print_str("\nFinancial report...\n");
    ap_financial_report();

    print_str("\nFinal state...\n");
    ap_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
