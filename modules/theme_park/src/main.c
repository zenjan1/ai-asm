/* theme_park: Theme park management (v1.0)
 * Rides, guests, staff, food, retail, maintenance, revenue
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

#define MAX_RIDES     14
#define MAX_GUESTS    20
#define MAX_STAFF     16
#define MAX_FOOD      10
#define MAX_RETAIL    8
#define MAX_MAINT     12

typedef struct {
    int    ride_id;
    int    type;
    int    capacity;
    int    cycle_min;
    int    height_cm;
    int    riders_today;
    double maintenance_cost;
    int    operational;
    int    active;
} ride_t;

typedef struct {
    int    guest_id;
    int    ticket_type;
    double ticket_price;
    int    fast_pass;
    int    rides_used;
    double food_spent;
    double retail_spent;
    int    visit_hours;
    int    active;
} guest_t;

typedef struct {
    int    staff_id;
    int    role;
    int    zone;
    double hourly_rate;
    int    hours_worked;
    double total_pay;
    int    rating;
    int    active;
} staff_t;

typedef struct {
    int    food_id;
    int    type;
    double price;
    double cost;
    int    sold;
    double revenue;
    int    active;
} food_t;

typedef struct {
    int    retail_id;
    int    type;
    double price;
    double cost;
    int    stock;
    int    sold;
    double revenue;
    int    active;
} retail_t;

typedef struct {
    int    maint_id;
    int    ride_id;
    int    type;
    double cost;
    int    hours;
    int    day;
    int    completed;
    int    active;
} maint_t;

typedef struct {
    int    n_rides;
    int    n_guests;
    int    n_staff;
    int    n_food;
    int    n_retail;
    int    n_maint;
    double ticket_revenue;
    double fast_pass_revenue;
    double food_revenue;
    double retail_revenue;
    double total_revenue;
    double staff_costs;
    double maintenance_costs;
    double ops_costs;
    int    total_riders;
    int    total_rides_given;
} tp_state_t;

static ride_t rides[MAX_RIDES];
static guest_t guests[MAX_GUESTS];
static staff_t staff[MAX_STAFF];
static food_t foods[MAX_FOOD];
static retail_t retail[MAX_RETAIL];
static maint_t maints[MAX_MAINT];
static tp_state_t tp;

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

int tp_init(void) {
    if (initialized) return -1;
    tp.n_rides = 0; tp.n_guests = 0; tp.n_staff = 0;
    tp.n_food = 0; tp.n_retail = 0; tp.n_maint = 0;
    tp.ticket_revenue = 0.0; tp.fast_pass_revenue = 0.0;
    tp.food_revenue = 0.0; tp.retail_revenue = 0.0;
    tp.total_revenue = 0.0; tp.staff_costs = 0.0;
    tp.maintenance_costs = 0.0; tp.ops_costs = 0.0;
    tp.total_riders = 0; tp.total_rides_given = 0;
    for (int i = 0; i < MAX_RIDES; i++) rides[i].active = 0;
    for (int i = 0; i < MAX_GUESTS; i++) guests[i].active = 0;
    for (int i = 0; i < MAX_STAFF; i++) staff[i].active = 0;
    for (int i = 0; i < MAX_FOOD; i++) foods[i].active = 0;
    for (int i = 0; i < MAX_RETAIL; i++) retail[i].active = 0;
    for (int i = 0; i < MAX_MAINT; i++) maints[i].active = 0;
    initialized = 1;
    print_str("[TP] Theme park initialized\n");
    return 0;
}

int tp_add_ride(int type, int capacity, int cycle_min, int height_cm) {
    if (tp.n_rides >= MAX_RIDES) return -1;
    ride_t* r = &rides[tp.n_rides];
    r->ride_id = tp.n_rides;
    r->type = type;
    r->capacity = capacity;
    r->cycle_min = cycle_min;
    r->height_cm = height_cm;
    r->riders_today = 0;
    r->maintenance_cost = 0.0;
    r->operational = 1;
    r->active = 1;
    tp.ops_costs += 200.0;
    tp.n_rides++;
    print_str("[TP] Ride "); print_int(tp.n_rides - 1);
    print_str(" type="); print_int(type);
    print_str(" cap="); print_int(capacity);
    print_str(" min="); print_int(cycle_min);
    print_str(" cm="); print_int(height_cm); print_str("\n");
    return tp.n_rides - 1;
}

int tp_admit_guest(int ticket_type, int fast_pass) {
    if (tp.n_guests >= MAX_GUESTS) return -1;
    guest_t* g = &guests[tp.n_guests];
    g->guest_id = tp.n_guests;
    g->ticket_type = ticket_type;
    g->ticket_price = 50.0 + ticket_type * 25.0;
    g->fast_pass = fast_pass;
    g->rides_used = 0;
    g->food_spent = 0.0;
    g->retail_spent = 0.0;
    g->visit_hours = 0;
    g->active = 1;
    tp.ticket_revenue += g->ticket_price;
    tp.total_revenue += g->ticket_price;
    if (fast_pass) {
        tp.fast_pass_revenue += 30.0;
        tp.total_revenue += 30.0;
    }
    tp.n_guests++;
    print_str("[TP] Guest "); print_int(tp.n_guests - 1);
    print_str(" ticket="); print_int(ticket_type);
    print_str(" fp="); print_int(fast_pass);
    print_str(" $"); print_int((int)g->ticket_price); print_str("\n");
    return tp.n_guests - 1;
}

int tp_add_staff(int role, int zone, double rate) {
    if (tp.n_staff >= MAX_STAFF) return -1;
    staff_t* s = &staff[tp.n_staff];
    s->staff_id = tp.n_staff;
    s->role = role;
    s->zone = zone;
    s->hourly_rate = rate;
    s->hours_worked = 0;
    s->total_pay = 0.0;
    s->rating = 0;
    s->active = 1;
    tp.n_staff++;
    print_str("[TP] Staff "); print_int(tp.n_staff - 1);
    print_str(" role="); print_int(role);
    print_str(" zone="); print_int(zone);
    print_str(" $"); print_int((int)rate); print_str("/hr\n");
    return tp.n_staff - 1;
}

int tp_add_food(int type, double price, double cost) {
    if (tp.n_food >= MAX_FOOD) return -1;
    food_t* f = &foods[tp.n_food];
    f->food_id = tp.n_food;
    f->type = type;
    f->price = price;
    f->cost = cost;
    f->sold = 0;
    f->revenue = 0.0;
    f->active = 1;
    tp.n_food++;
    print_str("[TP] Food "); print_int(tp.n_food - 1);
    print_str(" type="); print_int(type);
    print_str(" $"); print_int((int)price); print_str("\n");
    return tp.n_food - 1;
}

int tp_add_retail(int type, double price, double cost, int stock) {
    if (tp.n_retail >= MAX_RETAIL) return -1;
    retail_t* r = &retail[tp.n_retail];
    r->retail_id = tp.n_retail;
    r->type = type;
    r->price = price;
    r->cost = cost;
    r->stock = stock;
    r->sold = 0;
    r->revenue = 0.0;
    r->active = 1;
    tp.n_retail++;
    print_str("[TP] Retail "); print_int(tp.n_retail - 1);
    print_str(" type="); print_int(type);
    print_str(" $"); print_int((int)price);
    print_str(" stock="); print_int(stock); print_str("\n");
    return tp.n_retail - 1;
}

int tp_ride_cycle(int ride_id, int n_riders) {
    if (ride_id >= tp.n_rides) return -1;
    ride_t* r = &rides[ride_id];
    if (!r->operational) return -2;
    if (n_riders > r->capacity) n_riders = r->capacity;
    r->riders_today += n_riders;
    tp.total_riders += n_riders;
    tp.total_rides_given++;
    double rev = n_riders * 5.0;
    tp.total_revenue += rev;
    print_str("[TP] Ride "); print_int(ride_id);
    print_str(" riders="); print_int(n_riders);
    print_str(" $"); print_int((int)rev); print_str("\n");
    return 0;
}

int tp_guest_ride(int guest_id, int ride_id) {
    if (guest_id >= tp.n_guests || ride_id >= tp.n_rides) return -1;
    guests[guest_id].rides_used++;
    return tp_ride_cycle(ride_id, 1);
}

int tp_guest_food(int guest_id, int food_id) {
    if (guest_id >= tp.n_guests || food_id >= tp.n_food) return -1;
    food_t* f = &foods[food_id];
    f->sold++;
    f->revenue += f->price;
    tp.food_revenue += f->price;
    tp.total_revenue += f->price;
    guests[guest_id].food_spent += f->price;
    print_str("[TP] Food F"); print_int(food_id);
    print_str(" G"); print_int(guest_id);
    print_str(" $"); print_int((int)f->price); print_str("\n");
    return 0;
}

int tp_guest_retail(int guest_id, int retail_id) {
    if (guest_id >= tp.n_guests || retail_id >= tp.n_retail) return -1;
    retail_t* r = &retail[retail_id];
    if (r->stock <= 0) return -2;
    r->stock--;
    r->sold++;
    r->revenue += r->price;
    tp.retail_revenue += r->price;
    tp.total_revenue += r->price;
    guests[guest_id].retail_spent += r->price;
    print_str("[TP] Retail R"); print_int(retail_id);
    print_str(" G"); print_int(guest_id);
    print_str(" $"); print_int((int)r->price); print_str("\n");
    return 0;
}

int tp_staff_work(int staff_id, int hours) {
    if (staff_id >= tp.n_staff) return -1;
    staff_t* s = &staff[staff_id];
    s->hours_worked += hours;
    double pay = hours * s->hourly_rate;
    s->total_pay += pay;
    tp.staff_costs += pay;
    print_str("[TP] Staff "); print_int(staff_id);
    print_str(" hrs="); print_int(hours);
    print_str(" $"); print_int((int)pay); print_str("\n");
    return 0;
}

int tp_schedule_maint(int ride_id, int type, int hours, int day) {
    if (tp.n_maint >= MAX_MAINT || ride_id >= tp.n_rides) return -1;
    maint_t* m = &maints[tp.n_maint];
    m->maint_id = tp.n_maint;
    m->ride_id = ride_id;
    m->type = type;
    m->hours = hours;
    m->cost = hours * 200.0 + type * 300.0;
    m->day = day;
    m->completed = 0;
    m->active = 1;
    tp.n_maint++;
    print_str("[TP] Maint "); print_int(tp.n_maint - 1);
    print_str(" R"); print_int(ride_id);
    print_str(" type="); print_int(type);
    print_str(" hrs="); print_int(hours);
    print_str(" $"); print_int((int)m->cost); print_str("\n");
    return tp.n_maint - 1;
}

int tp_complete_maint(int maint_id) {
    if (maint_id >= tp.n_maint) return -1;
    maint_t* m = &maints[maint_id];
    m->completed = 1;
    tp.maintenance_costs += m->cost;
    rides[m->ride_id].maintenance_cost += m->cost;
    print_str("[TP] Complete M"); print_int(maint_id);
    print_str(" $"); print_int((int)m->cost); print_str("\n");
    return 0;
}

void tp_ride_report(void) {
    print_str("[TP] Ride report:\n");
    for (int i = 0; i < tp.n_rides; i++) {
        ride_t* r = &rides[i];
        print_str("  R"); print_int(i);
        print_str(" type="); print_int(r->type);
        print_str(" cap="); print_int(r->capacity);
        print_str(" riders="); print_int(r->riders_today);
        print_str(" op="); print_int(r->operational); print_str("\n");
    }
}

void tp_financial_report(void) {
    print_str("[TP] Financial report:\n");
    print_str("  Ticket revenue: "); print_int((int)tp.ticket_revenue); print_str("\n");
    print_str("  Fast pass revenue: "); print_int((int)tp.fast_pass_revenue); print_str("\n");
    print_str("  Food revenue: "); print_int((int)tp.food_revenue); print_str("\n");
    print_str("  Retail revenue: "); print_int((int)tp.retail_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)tp.total_revenue); print_str("\n");
    print_str("  Staff costs: "); print_int((int)tp.staff_costs); print_str("\n");
    print_str("  Maintenance: "); print_int((int)tp.maintenance_costs); print_str("\n");
    print_str("  Ops costs: "); print_int((int)tp.ops_costs); print_str("\n");
    double total_costs = tp.staff_costs + tp.maintenance_costs + tp.ops_costs;
    double profit = tp.total_revenue - total_costs;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    print_str("  Guests: "); print_int(tp.n_guests);
    print_str(" Riders: "); print_int(tp.total_riders);
    print_str(" Rides: "); print_int(tp.total_rides_given); print_str("\n");
}

void tp_print_state(void) {
    print_str("[TP] Rides="); print_int(tp.n_rides);
    print_str(" Guests="); print_int(tp.n_guests);
    print_str(" Staff="); print_int(tp.n_staff);
    print_str(" Food="); print_int(tp.n_food);
    print_str(" Retail="); print_int(tp.n_retail);
    print_str(" Maint="); print_int(tp.n_maint);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)tp.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Theme Park Demo ===\n\n");
    tp_init();

    print_str("Adding rides...\n");
    tp_add_ride(1, 24, 3, 130);
    tp_add_ride(1, 20, 4, 140);
    tp_add_ride(2, 16, 5, 120);
    tp_add_ride(2, 32, 3, 110);
    tp_add_ride(3, 12, 6, 100);
    tp_add_ride(3, 8, 8, 150);
    tp_add_ride(4, 24, 4, 0);
    tp_add_ride(4, 36, 3, 0);
    tp_add_ride(5, 20, 5, 120);
    tp_add_ride(5, 16, 6, 130);
    tp_add_ride(6, 40, 2, 0);
    tp_add_ride(6, 30, 3, 100);
    tp_add_ride(1, 22, 3, 125);
    tp_add_ride(2, 18, 5, 115);

    print_str("\nAdding food...\n");
    tp_add_food(1, 8.0, 3.0);
    tp_add_food(1, 10.0, 4.0);
    tp_add_food(2, 6.0, 2.0);
    tp_add_food(2, 7.0, 2.5);
    tp_add_food(3, 12.0, 5.0);
    tp_add_food(3, 15.0, 6.0);
    tp_add_food(4, 5.0, 1.5);
    tp_add_food(4, 4.0, 1.0);
    tp_add_food(5, 9.0, 3.5);
    tp_add_food(5, 11.0, 4.5);

    print_str("\nAdding retail...\n");
    tp_add_retail(1, 15.0, 5.0, 200);
    tp_add_retail(1, 20.0, 7.0, 150);
    tp_add_retail(2, 25.0, 10.0, 100);
    tp_add_retail(2, 35.0, 15.0, 80);
    tp_add_retail(3, 12.0, 4.0, 250);
    tp_add_retail(3, 18.0, 6.0, 180);
    tp_add_retail(4, 30.0, 12.0, 60);
    tp_add_retail(4, 45.0, 20.0, 40);

    print_str("\nAdding staff...\n");
    tp_add_staff(1, 1, 15.0);
    tp_add_staff(1, 2, 15.0);
    tp_add_staff(1, 3, 15.0);
    tp_add_staff(2, 1, 12.0);
    tp_add_staff(2, 2, 12.0);
    tp_add_staff(2, 3, 12.0);
    tp_add_staff(3, 1, 11.0);
    tp_add_staff(3, 2, 11.0);
    tp_add_staff(4, 1, 18.0);
    tp_add_staff(4, 2, 18.0);
    tp_add_staff(5, 1, 25.0);
    tp_add_staff(5, 2, 25.0);
    tp_add_staff(1, 4, 15.0);
    tp_add_staff(2, 4, 12.0);
    tp_add_staff(3, 4, 11.0);
    tp_add_staff(4, 3, 18.0);

    print_str("\nAdmitting guests...\n");
    for (int i = 0; i < 20; i++) {
        tp_admit_guest((i % 4) + 1, i % 3);
    }

    print_str("\nRide cycles...\n");
    for (int i = 0; i < 14; i++) {
        for (int j = 0; j < 8; j++) {
            tp_ride_cycle(i, 10 + (i + j) % 15);
        }
    }

    print_str("\nGuest food purchases...\n");
    for (int i = 0; i < 20; i++) {
        tp_guest_food(i, i % 10);
        tp_guest_food(i, (i + 3) % 10);
    }

    print_str("\nGuest retail purchases...\n");
    for (int i = 0; i < 20; i++) {
        tp_guest_retail(i, i % 8);
    }

    print_str("\nStaff working...\n");
    for (int i = 0; i < 16; i++) {
        tp_staff_work(i, 8);
    }

    print_str("\nScheduling maintenance...\n");
    tp_schedule_maint(0, 1, 4, 30);
    tp_schedule_maint(1, 2, 6, 31);
    tp_schedule_maint(2, 1, 5, 32);
    tp_schedule_maint(3, 3, 8, 33);
    tp_schedule_maint(4, 2, 6, 34);
    tp_schedule_maint(5, 1, 4, 35);
    tp_schedule_maint(6, 3, 10, 36);
    tp_schedule_maint(7, 1, 3, 37);
    tp_schedule_maint(8, 2, 7, 38);
    tp_schedule_maint(9, 1, 5, 39);
    tp_schedule_maint(10, 3, 9, 40);
    tp_schedule_maint(11, 2, 6, 41);

    print_str("\nCompleting maintenance...\n");
    for (int i = 0; i < 12; i++) {
        tp_complete_maint(i);
    }

    print_str("\nRide report...\n");
    tp_ride_report();

    print_str("\nFinancial report...\n");
    tp_financial_report();

    print_str("\nFinal state...\n");
    tp_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
