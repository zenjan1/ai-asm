/* wedding_planner: Wedding planning management (v1.0)
 * Couples, venues, vendors, guests, budget, timeline
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

#define MAX_COUPLES    8
#define MAX_VENUES     6
#define MAX_VENDORS    12
#define MAX_GUESTS     20
#define MAX_TASKS      14

typedef struct {
    int    couple_id;
    int    style;
    double budget;
    int    wedding_day;
    int    n_guests;
    double spent;
    int    active;
} couple_t;

typedef struct {
    int    venue_id;
    int    type;
    int    capacity;
    double rental_fee;
    int    booked_day;
    int    couple_id;
    int    active;
} venue_t;

typedef struct {
    int    vendor_id;
    int    type;
    double price;
    int    rating;
    int    couple_id;
    int    confirmed;
    int    active;
} vendor_t;

typedef struct {
    int    guest_id;
    int    couple_id;
    int    side;
    int    rsvp;
    int    table;
    double meal_cost;
    int    active;
} guest_t;

typedef struct {
    int    task_id;
    int    couple_id;
    int    type;
    int    due_day;
    int    done;
    int    priority;
    int    active;
} task_t;

typedef struct {
    int    n_couples;
    int    n_venues;
    int    n_vendors;
    int    n_guests;
    int    n_tasks;
    double total_budget;
    double total_spent;
    int    total_guests;
    int    confirmed_guests;
} wp_state_t;

static couple_t couples[MAX_COUPLES];
static venue_t venues[MAX_VENUES];
static vendor_t vendors[MAX_VENDORS];
static guest_t guests[MAX_GUESTS];
static task_t tasks[MAX_TASKS];
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
    wp.n_couples = 0; wp.n_venues = 0; wp.n_vendors = 0;
    wp.n_guests = 0; wp.n_tasks = 0;
    wp.total_budget = 0.0; wp.total_spent = 0.0;
    wp.total_guests = 0; wp.confirmed_guests = 0;
    for (int i = 0; i < MAX_COUPLES; i++) couples[i].active = 0;
    for (int i = 0; i < MAX_VENUES; i++) venues[i].active = 0;
    for (int i = 0; i < MAX_VENDORS; i++) vendors[i].active = 0;
    for (int i = 0; i < MAX_GUESTS; i++) guests[i].active = 0;
    for (int i = 0; i < MAX_TASKS; i++) tasks[i].active = 0;
    initialized = 1;
    print_str("[WP] Wedding planner initialized\n");
    return 0;
}

int wp_add_couple(int style, double budget, int wedding_day) {
    if (wp.n_couples >= MAX_COUPLES) return -1;
    couple_t* c = &couples[wp.n_couples];
    c->couple_id = wp.n_couples;
    c->style = style;
    c->budget = budget;
    c->wedding_day = wedding_day;
    c->n_guests = 0;
    c->spent = 0.0;
    c->active = 1;
    wp.n_couples++;
    wp.total_budget += budget;
    print_str("[WP] Couple "); print_int(wp.n_couples - 1);
    print_str(" style="); print_int(style);
    print_str(" budget="); print_int((int)budget);
    print_str(" day="); print_int(wedding_day); print_str("\n");
    return wp.n_couples - 1;
}

int wp_add_venue(int type, int capacity, double fee) {
    if (wp.n_venues >= MAX_VENUES) return -1;
    venue_t* v = &venues[wp.n_venues];
    v->venue_id = wp.n_venues;
    v->type = type;
    v->capacity = capacity;
    v->rental_fee = fee;
    v->booked_day = 0;
    v->couple_id = -1;
    v->active = 1;
    wp.n_venues++;
    print_str("[WP] Venue "); print_int(wp.n_venues - 1);
    print_str(" type="); print_int(type);
    print_str(" cap="); print_int(capacity);
    print_str(" fee="); print_int((int)fee); print_str("\n");
    return wp.n_venues - 1;
}

int wp_add_vendor(int type, double price, int rating) {
    if (wp.n_vendors >= MAX_VENDORS) return -1;
    vendor_t* v = &vendors[wp.n_vendors];
    v->vendor_id = wp.n_vendors;
    v->type = type;
    v->price = price;
    v->rating = rating;
    v->couple_id = -1;
    v->confirmed = 0;
    v->active = 1;
    wp.n_vendors++;
    print_str("[WP] Vendor "); print_int(wp.n_vendors - 1);
    print_str(" type="); print_int(type);
    print_str(" price="); print_int((int)price);
    print_str(" rating="); print_int(rating); print_str("\n");
    return wp.n_vendors - 1;
}

int wp_book_venue(int couple_id, int venue_id) {
    if (couple_id >= wp.n_couples || venue_id >= wp.n_venues) return -1;
    venue_t* v = &venues[venue_id];
    if (v->couple_id >= 0) return -2;
    v->couple_id = couple_id;
    v->booked_day = couples[couple_id].wedding_day;
    couples[couple_id].spent += v->rental_fee;
    wp.total_spent += v->rental_fee;
    print_str("[WP] Venue "); print_int(venue_id);
    print_str(" booked by C"); print_int(couple_id);
    print_str(" fee="); print_int((int)v->rental_fee); print_str("\n");
    return 0;
}

int wp_hire_vendor(int couple_id, int vendor_id) {
    if (couple_id >= wp.n_couples || vendor_id >= wp.n_vendors) return -1;
    vendor_t* v = &vendors[vendor_id];
    if (v->couple_id >= 0) return -2;
    v->couple_id = couple_id;
    v->confirmed = 1;
    couples[couple_id].spent += v->price;
    wp.total_spent += v->price;
    print_str("[WP] Vendor "); print_int(vendor_id);
    print_str(" hired by C"); print_int(couple_id);
    print_str(" fee="); print_int((int)v->price); print_str("\n");
    return 0;
}

int wp_add_guest(int couple_id, int side, double meal_cost) {
    if (wp.n_guests >= MAX_GUESTS || couple_id >= wp.n_couples) return -1;
    guest_t* g = &guests[wp.n_guests];
    g->guest_id = wp.n_guests;
    g->couple_id = couple_id;
    g->side = side;
    g->rsvp = 0;
    g->table = 0;
    g->meal_cost = meal_cost;
    g->active = 1;
    couples[couple_id].n_guests++;
    wp.total_guests++;
    wp.total_spent += meal_cost;
    couples[couple_id].spent += meal_cost;
    wp.n_guests++;
    print_str("[WP] Guest "); print_int(wp.n_guests - 1);
    print_str(" C"); print_int(couple_id);
    print_str(" side="); print_int(side);
    print_str(" meal="); print_int((int)meal_cost); print_str("\n");
    return wp.n_guests - 1;
}

void wp_rsvp(int guest_id, int confirmed) {
    if (guest_id >= wp.n_guests) return;
    guests[guest_id].rsvp = confirmed;
    if (confirmed) wp.confirmed_guests++;
    print_str("[WP] Guest "); print_int(guest_id);
    print_str(confirmed ? " confirmed\n" : " declined\n");
}

void wp_assign_table(int guest_id, int table) {
    if (guest_id >= wp.n_guests) return;
    guests[guest_id].table = table;
    print_str("[WP] Guest "); print_int(guest_id);
    print_str(" table="); print_int(table); print_str("\n");
}

int wp_add_task(int couple_id, int type, int due_day, int priority) {
    if (wp.n_tasks >= MAX_TASKS || couple_id >= wp.n_couples) return -1;
    task_t* t = &tasks[wp.n_tasks];
    t->task_id = wp.n_tasks;
    t->couple_id = couple_id;
    t->type = type;
    t->due_day = due_day;
    t->done = 0;
    t->priority = priority;
    t->active = 1;
    wp.n_tasks++;
    print_str("[WP] Task "); print_int(wp.n_tasks - 1);
    print_str(" C"); print_int(couple_id);
    print_str(" type="); print_int(type);
    print_str(" due="); print_int(due_day);
    print_str(" pri="); print_int(priority); print_str("\n");
    return wp.n_tasks - 1;
}

void wp_complete_task(int task_id) {
    if (task_id >= wp.n_tasks) return;
    tasks[task_id].done = 1;
    print_str("[WP] Task "); print_int(task_id); print_str(" completed\n");
}

void wp_guest_report(void) {
    print_str("[WP] Guest report:\n");
    for (int c = 0; c < wp.n_couples; c++) {
        int total = 0, confirmed = 0;
        for (int i = 0; i < wp.n_guests; i++) {
            if (guests[i].couple_id == c) {
                total++;
                if (guests[i].rsvp) confirmed++;
            }
        }
        print_str("  Couple "); print_int(c);
        print_str(" total="); print_int(total);
        print_str(" confirmed="); print_int(confirmed); print_str("\n");
    }
}

void wp_budget_report(void) {
    print_str("[WP] Budget report:\n");
    for (int i = 0; i < wp.n_couples; i++) {
        couple_t* c = &couples[i];
        double remaining = c->budget - c->spent;
        print_str("  C"); print_int(i);
        print_str(" budget="); print_int((int)c->budget);
        print_str(" spent="); print_int((int)c->spent);
        print_str(" remain="); print_int((int)remaining);
        print_str(" guests="); print_int(c->n_guests); print_str("\n");
    }
    double total_remaining = wp.total_budget - wp.total_spent;
    print_str("  Total budget: "); print_int((int)wp.total_budget); print_str("\n");
    print_str("  Total spent: "); print_int((int)wp.total_spent); print_str("\n");
    print_str("  Total remaining: "); print_int((int)total_remaining); print_str("\n");
}

void wp_task_report(void) {
    print_str("[WP] Task report:\n");
    int done = 0;
    for (int i = 0; i < wp.n_tasks; i++) {
        if (tasks[i].done) done++;
    }
    print_str("  Total tasks: "); print_int(wp.n_tasks); print_str("\n");
    print_str("  Completed: "); print_int(done); print_str("\n");
    print_str("  Pending: "); print_int(wp.n_tasks - done); print_str("\n");
}

void wp_print_state(void) {
    print_str("[WP] Couples="); print_int(wp.n_couples);
    print_str(" Venues="); print_int(wp.n_venues);
    print_str(" Vendors="); print_int(wp.n_vendors);
    print_str(" Guests="); print_int(wp.n_guests);
    print_str(" Tasks="); print_int(wp.n_tasks);
    print_str("\n");
    print_str("  Total spent: "); print_int((int)wp.total_spent); print_str("\n");
}

int main(void) {
    print_str("=== Wedding Planner Demo ===\n\n");
    wp_init();

    print_str("Adding couples...\n");
    wp_add_couple(1, 30000.0, 60);
    wp_add_couple(2, 50000.0, 75);
    wp_add_couple(3, 20000.0, 90);
    wp_add_couple(1, 40000.0, 105);
    wp_add_couple(2, 25000.0, 120);
    wp_add_couple(3, 35000.0, 135);

    print_str("\nAdding venues...\n");
    wp_add_venue(1, 100, 5000.0);
    wp_add_venue(1, 200, 8000.0);
    wp_add_venue(2, 80, 3500.0);
    wp_add_venue(2, 150, 6000.0);
    wp_add_venue(3, 60, 2500.0);
    wp_add_venue(3, 120, 4500.0);

    print_str("\nAdding vendors...\n");
    wp_add_vendor(1, 2500.0, 92);
    wp_add_vendor(2, 1800.0, 88);
    wp_add_vendor(3, 3000.0, 95);
    wp_add_vendor(4, 1200.0, 90);
    wp_add_vendor(5, 800.0, 85);
    wp_add_vendor(6, 1500.0, 93);
    wp_add_vendor(1, 2200.0, 87);
    wp_add_vendor(2, 1600.0, 91);
    wp_add_vendor(3, 2800.0, 89);
    wp_add_vendor(4, 1000.0, 86);
    wp_add_vendor(5, 700.0, 84);
    wp_add_vendor(6, 1400.0, 94);

    print_str("\nBooking venues...\n");
    wp_book_venue(0, 0);
    wp_book_venue(1, 1);
    wp_book_venue(2, 2);
    wp_book_venue(3, 3);
    wp_book_venue(4, 4);
    wp_book_venue(5, 5);

    print_str("\nHiring vendors...\n");
    wp_hire_vendor(0, 0);
    wp_hire_vendor(0, 3);
    wp_hire_vendor(0, 5);
    wp_hire_vendor(1, 1);
    wp_hire_vendor(1, 4);
    wp_hire_vendor(1, 6);
    wp_hire_vendor(2, 2);
    wp_hire_vendor(2, 5);
    wp_hire_vendor(2, 8);
    wp_hire_vendor(3, 0);
    wp_hire_vendor(3, 7);
    wp_hire_vendor(3, 10);
    wp_hire_vendor(4, 3);
    wp_hire_vendor(4, 9);
    wp_hire_vendor(5, 1);
    wp_hire_vendor(5, 6);
    wp_hire_vendor(5, 11);

    print_str("\nAdding guests...\n");
    for (int c = 0; c < 6; c++) {
        for (int g = 0; g < 3; g++) {
            int side = g < 2 ? 1 : 2;
            double meal = 50.0 + g * 25.0;
            wp_add_guest(c, side, meal);
        }
    }

    print_str("\nRSVP responses...\n");
    for (int i = 0; i < wp.n_guests; i++) {
        wp_rsvp(i, (i % 5 != 0) ? 1 : 0);
    }

    print_str("\nAssigning tables...\n");
    for (int i = 0; i < wp.n_guests; i++) {
        if (guests[i].rsvp) {
            wp_assign_table(i, (i / 4) + 1);
        }
    }

    print_str("\nAdding tasks...\n");
    wp_add_task(0, 1, 30, 3);
    wp_add_task(0, 2, 40, 2);
    wp_add_task(0, 3, 50, 1);
    wp_add_task(1, 1, 45, 3);
    wp_add_task(1, 2, 55, 2);
    wp_add_task(1, 3, 65, 1);
    wp_add_task(2, 1, 60, 3);
    wp_add_task(2, 2, 70, 2);
    wp_add_task(3, 1, 75, 3);
    wp_add_task(3, 2, 85, 2);
    wp_add_task(4, 1, 90, 3);
    wp_add_task(4, 2, 100, 2);
    wp_add_task(5, 1, 105, 3);
    wp_add_task(5, 2, 115, 2);

    print_str("\nCompleting tasks...\n");
    for (int i = 0; i < 10; i++) {
        wp_complete_task(i);
    }

    print_str("\nGuest report...\n");
    wp_guest_report();

    print_str("\nBudget report...\n");
    wp_budget_report();

    print_str("\nTask report...\n");
    wp_task_report();

    print_str("\nFinal state...\n");
    wp_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
