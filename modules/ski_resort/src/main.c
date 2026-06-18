/* ski_resort: Ski resort management (v1.0)
 * Slopes, lifts, rentals, lessons, lodging, revenue
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

#define MAX_SLOPES  10
#define MAX_LIFTS   8
#define MAX_RENTALS 14
#define MAX_LESSONS 10
#define MAX_LODGING 8
#define MAX_GUESTS  16

typedef struct {
    int    slope_id;
    int    difficulty;
    int    length_m;
    int    capacity;
    int    status;
    int    n_skiers;
    int    active;
} slope_t;

typedef struct {
    int    lift_id;
    int    type;
    int    capacity_hr;
    double ticket_price;
    int    tickets_sold;
    double revenue;
    int    status;
    int    active;
} lift_t;

typedef struct {
    int    rental_id;
    int    type;
    int    size;
    double daily_rate;
    int    rented;
    double total_revenue;
    int    active;
} rental_t;

typedef struct {
    int    lesson_id;
    int    type;
    int    level;
    double price;
    int    max_students;
    int    n_enrolled;
    double revenue;
    int    active;
} lesson_t;

typedef struct {
    int    lodging_id;
    int    type;
    int    capacity;
    double nightly_rate;
    int    n_bookings;
    double revenue;
    int    active;
} lodging_t;

typedef struct {
    int    guest_id;
    int    n_lift_tickets;
    int    n_rentals;
    int    n_lessons;
    double total_spent;
    int    active;
} guest_t;

typedef struct {
    int    n_slopes;
    int    n_lifts;
    int    n_rentals;
    int    n_lessons;
    int    n_lodging;
    int    n_guests;
    double lift_revenue;
    double rental_revenue;
    double lesson_revenue;
    double lodging_revenue;
    double total_revenue;
    double total_costs;
    int    total_skiers;
} sr_state_t;

static slope_t slopes[MAX_SLOPES];
static lift_t lifts[MAX_LIFTS];
static rental_t rentals[MAX_RENTALS];
static lesson_t lessons[MAX_LESSONS];
static lodging_t lodgings[MAX_LODGING];
static guest_t guests[MAX_GUESTS];
static sr_state_t sr;

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

int sr_init(void) {
    if (initialized) return -1;
    sr.n_slopes = 0; sr.n_lifts = 0; sr.n_rentals = 0;
    sr.n_lessons = 0; sr.n_lodging = 0; sr.n_guests = 0;
    sr.lift_revenue = 0.0; sr.rental_revenue = 0.0;
    sr.lesson_revenue = 0.0; sr.lodging_revenue = 0.0;
    sr.total_revenue = 0.0; sr.total_costs = 0.0;
    sr.total_skiers = 0;
    for (int i = 0; i < MAX_SLOPES; i++) slopes[i].active = 0;
    for (int i = 0; i < MAX_LIFTS; i++) lifts[i].active = 0;
    for (int i = 0; i < MAX_RENTALS; i++) rentals[i].active = 0;
    for (int i = 0; i < MAX_LESSONS; i++) lessons[i].active = 0;
    for (int i = 0; i < MAX_LODGING; i++) lodgings[i].active = 0;
    for (int i = 0; i < MAX_GUESTS; i++) guests[i].active = 0;
    initialized = 1;
    print_str("[SR] Ski resort initialized\n");
    return 0;
}

int sr_add_slope(int difficulty, int length, int capacity) {
    if (sr.n_slopes >= MAX_SLOPES) return -1;
    slope_t* s = &slopes[sr.n_slopes];
    s->slope_id = sr.n_slopes;
    s->difficulty = difficulty;
    s->length_m = length;
    s->capacity = capacity;
    s->status = 1;
    s->n_skiers = 0;
    s->active = 1;
    sr.n_slopes++;
    print_str("[SR] Slope "); print_int(sr.n_slopes - 1);
    print_str(" diff="); print_int(difficulty);
    print_str(" len="); print_int(length); print_str("m");
    print_str(" cap="); print_int(capacity); print_str("\n");
    return sr.n_slopes - 1;
}

int sr_add_lift(int type, int capacity, double price) {
    if (sr.n_lifts >= MAX_LIFTS) return -1;
    lift_t* l = &lifts[sr.n_lifts];
    l->lift_id = sr.n_lifts;
    l->type = type;
    l->capacity_hr = capacity;
    l->ticket_price = price;
    l->tickets_sold = 0;
    l->revenue = 0.0;
    l->status = 1;
    l->active = 1;
    sr.n_lifts++;
    print_str("[SR] Lift "); print_int(sr.n_lifts - 1);
    print_str(" type="); print_int(type);
    print_str(" cap="); print_int(capacity); print_str("/hr");
    print_str(" $"); print_int((int)price); print_str("\n");
    return sr.n_lifts - 1;
}

int sr_add_rental(int type, int size, double rate) {
    if (sr.n_rentals >= MAX_RENTALS) return -1;
    rental_t* r = &rentals[sr.n_rentals];
    r->rental_id = sr.n_rentals;
    r->type = type;
    r->size = size;
    r->daily_rate = rate;
    r->rented = 0;
    r->total_revenue = 0.0;
    r->active = 1;
    sr.n_rentals++;
    print_str("[SR] Rental "); print_int(sr.n_rentals - 1);
    print_str(" type="); print_int(type);
    print_str(" sz="); print_int(size);
    print_str(" $"); print_int((int)rate); print_str("/day\n");
    return sr.n_rentals - 1;
}

int sr_add_lesson(int type, int level, double price, int max_students) {
    if (sr.n_lessons >= MAX_LESSONS) return -1;
    lesson_t* l = &lessons[sr.n_lessons];
    l->lesson_id = sr.n_lessons;
    l->type = type;
    l->level = level;
    l->price = price;
    l->max_students = max_students;
    l->n_enrolled = 0;
    l->revenue = 0.0;
    l->active = 1;
    sr.n_lessons++;
    print_str("[SR] Lesson "); print_int(sr.n_lessons - 1);
    print_str(" type="); print_int(type);
    print_str(" lvl="); print_int(level);
    print_str(" $"); print_int((int)price); print_str("\n");
    return sr.n_lessons - 1;
}

int sr_add_lodging(int type, int capacity, double rate) {
    if (sr.n_lodging >= MAX_LODGING) return -1;
    lodging_t* lg = &lodgings[sr.n_lodging];
    lg->lodging_id = sr.n_lodging;
    lg->type = type;
    lg->capacity = capacity;
    lg->nightly_rate = rate;
    lg->n_bookings = 0;
    lg->revenue = 0.0;
    lg->active = 1;
    sr.n_lodging++;
    print_str("[SR] Lodging "); print_int(sr.n_lodging - 1);
    print_str(" type="); print_int(type);
    print_str(" cap="); print_int(capacity);
    print_str(" $"); print_int((int)rate); print_str("/nt\n");
    return sr.n_lodging - 1;
}

int sr_register_guest(void) {
    if (sr.n_guests >= MAX_GUESTS) return -1;
    guest_t* g = &guests[sr.n_guests];
    g->guest_id = sr.n_guests;
    g->n_lift_tickets = 0;
    g->n_rentals = 0;
    g->n_lessons = 0;
    g->total_spent = 0.0;
    g->active = 1;
    sr.n_guests++;
    print_str("[SR] Guest "); print_int(sr.n_guests - 1); print_str("\n");
    return sr.n_guests - 1;
}

int sr_sell_lift_ticket(int lift_id, int guest_id) {
    if (lift_id >= sr.n_lifts || guest_id >= sr.n_guests) return -1;
    lift_t* l = &lifts[lift_id];
    l->tickets_sold++;
    l->revenue += l->ticket_price;
    guests[guest_id].n_lift_tickets++;
    guests[guest_id].total_spent += l->ticket_price;
    sr.lift_revenue += l->ticket_price;
    sr.total_revenue += l->ticket_price;
    print_str("[SR] L"); print_int(lift_id);
    print_str(" ticket G"); print_int(guest_id);
    print_str(" $"); print_int((int)l->ticket_price); print_str("\n");
    return 0;
}

int sr_rent_equipment(int rental_id, int guest_id, int days) {
    if (rental_id >= sr.n_rentals || guest_id >= sr.n_guests) return -1;
    rental_t* r = &rentals[rental_id];
    double total = r->daily_rate * days;
    r->rented = 1;
    r->total_revenue += total;
    guests[guest_id].n_rentals++;
    guests[guest_id].total_spent += total;
    sr.rental_revenue += total;
    sr.total_revenue += total;
    print_str("[SR] R"); print_int(rental_id);
    print_str(" G"); print_int(guest_id);
    print_str(" days="); print_int(days);
    print_str(" $"); print_int((int)total); print_str("\n");
    return 0;
}

int sr_enroll_lesson(int lesson_id, int guest_id) {
    if (lesson_id >= sr.n_lessons || guest_id >= sr.n_guests) return -1;
    lesson_t* l = &lessons[lesson_id];
    if (l->n_enrolled >= l->max_students) return -2;
    l->n_enrolled++;
    l->revenue += l->price;
    guests[guest_id].n_lessons++;
    guests[guest_id].total_spent += l->price;
    sr.lesson_revenue += l->price;
    sr.total_revenue += l->price;
    print_str("[SR] Lsn"); print_int(lesson_id);
    print_str(" G"); print_int(guest_id);
    print_str(" $"); print_int((int)l->price); print_str("\n");
    return 0;
}

int sr_book_lodging(int lodging_id, int nights) {
    if (lodging_id >= sr.n_lodging) return -1;
    lodging_t* lg = &lodgings[lodging_id];
    double total = lg->nightly_rate * nights;
    lg->n_bookings++;
    lg->revenue += total;
    sr.lodging_revenue += total;
    sr.total_revenue += total;
    print_str("[SR] Lod"); print_int(lodging_id);
    print_str(" nights="); print_int(nights);
    print_str(" $"); print_int((int)total); print_str("\n");
    return 0;
}

int sr_add_skiers(int slope_id, int count) {
    if (slope_id >= sr.n_slopes) return -1;
    slopes[slope_id].n_skiers += count;
    sr.total_skiers += count;
    return 0;
}

void sr_slope_report(void) {
    print_str("[SR] Slope report:\n");
    for (int i = 0; i < sr.n_slopes; i++) {
        slope_t* s = &slopes[i];
        print_str("  S"); print_int(i);
        print_str(" diff="); print_int(s->difficulty);
        print_str(" len="); print_int(s->length_m); print_str("m");
        print_str(" skiers="); print_int(s->n_skiers);
        print_str(" status="); print_int(s->status); print_str("\n");
    }
}

void sr_financial_report(void) {
    print_str("[SR] Financial report:\n");
    print_str("  Lift revenue: "); print_int((int)sr.lift_revenue); print_str("\n");
    print_str("  Rental revenue: "); print_int((int)sr.rental_revenue); print_str("\n");
    print_str("  Lesson revenue: "); print_int((int)sr.lesson_revenue); print_str("\n");
    print_str("  Lodging revenue: "); print_int((int)sr.lodging_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)sr.total_revenue); print_str("\n");
    print_str("  Costs: "); print_int((int)sr.total_costs); print_str("\n");
    double profit = sr.total_revenue - sr.total_costs;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    print_str("  Total skiers: "); print_int(sr.total_skiers); print_str("\n");
}

void sr_print_state(void) {
    print_str("[SR] Slopes="); print_int(sr.n_slopes);
    print_str(" Lifts="); print_int(sr.n_lifts);
    print_str(" Rentals="); print_int(sr.n_rentals);
    print_str(" Lessons="); print_int(sr.n_lessons);
    print_str(" Lodging="); print_int(sr.n_lodging);
    print_str(" Guests="); print_int(sr.n_guests);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)sr.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Ski Resort Demo ===\n\n");
    sr_init();

    print_str("Adding slopes...\n");
    sr_add_slope(1, 800, 200);
    sr_add_slope(1, 600, 150);
    sr_add_slope(2, 1200, 180);
    sr_add_slope(2, 1000, 160);
    sr_add_slope(3, 1500, 100);
    sr_add_slope(3, 1800, 80);
    sr_add_slope(4, 500, 120);
    sr_add_slope(1, 700, 180);
    sr_add_slope(2, 1100, 140);
    sr_add_slope(3, 2000, 60);

    print_str("\nAdding lifts...\n");
    sr_add_lift(1, 2400, 89.0);
    sr_add_lift(1, 2000, 79.0);
    sr_add_lift(2, 1800, 69.0);
    sr_add_lift(2, 1600, 59.0);
    sr_add_lift(3, 3000, 99.0);
    sr_add_lift(1, 2200, 85.0);
    sr_add_lift(2, 1400, 55.0);
    sr_add_lift(3, 2800, 95.0);

    print_str("\nAdding rentals...\n");
    sr_add_rental(1, 160, 45.0);
    sr_add_rental(1, 165, 45.0);
    sr_add_rental(1, 170, 48.0);
    sr_add_rental(2, 8, 25.0);
    sr_add_rental(2, 9, 25.0);
    sr_add_rental(2, 10, 28.0);
    sr_add_rental(3, 54, 15.0);
    sr_add_rental(3, 56, 15.0);
    sr_add_rental(3, 58, 18.0);
    sr_add_rental(4, 150, 12.0);
    sr_add_rental(4, 155, 12.0);
    sr_add_rental(1, 175, 50.0);
    sr_add_rental(2, 11, 30.0);
    sr_add_rental(3, 60, 20.0);

    print_str("\nAdding lessons...\n");
    sr_add_lesson(1, 1, 120.0, 8);
    sr_add_lesson(1, 2, 150.0, 6);
    sr_add_lesson(2, 1, 250.0, 1);
    sr_add_lesson(2, 3, 300.0, 1);
    sr_add_lesson(3, 1, 80.0, 10);
    sr_add_lesson(3, 2, 100.0, 8);
    sr_add_lesson(4, 3, 400.0, 4);
    sr_add_lesson(1, 1, 130.0, 8);
    sr_add_lesson(2, 2, 280.0, 1);
    sr_add_lesson(4, 3, 450.0, 4);

    print_str("\nAdding lodging...\n");
    sr_add_lodging(1, 4, 250.0);
    sr_add_lodging(1, 6, 350.0);
    sr_add_lodging(2, 2, 180.0);
    sr_add_lodging(2, 2, 200.0);
    sr_add_lodging(3, 8, 500.0);
    sr_add_lodging(3, 10, 650.0);
    sr_add_lodging(1, 4, 280.0);
    sr_add_lodging(2, 3, 220.0);

    print_str("\nRegistering guests...\n");
    for (int i = 0; i < 16; i++) sr_register_guest();

    print_str("\nSelling lift tickets...\n");
    for (int i = 0; i < 16; i++) {
        sr_sell_lift_ticket(i % 8, i);
    }

    print_str("\nRenting equipment...\n");
    for (int i = 0; i < 14; i++) {
        sr_rent_equipment(i, i % 16, 1 + (i % 3));
    }

    print_str("\nEnrolling lessons...\n");
    sr_enroll_lesson(0, 0);
    sr_enroll_lesson(0, 1);
    sr_enroll_lesson(0, 2);
    sr_enroll_lesson(1, 3);
    sr_enroll_lesson(1, 4);
    sr_enroll_lesson(2, 5);
    sr_enroll_lesson(3, 6);
    sr_enroll_lesson(4, 7);
    sr_enroll_lesson(4, 8);
    sr_enroll_lesson(5, 9);
    sr_enroll_lesson(6, 10);
    sr_enroll_lesson(7, 11);
    sr_enroll_lesson(8, 12);
    sr_enroll_lesson(9, 13);

    print_str("\nBooking lodging...\n");
    sr_book_lodging(0, 3);
    sr_book_lodging(1, 5);
    sr_book_lodging(2, 2);
    sr_book_lodging(3, 4);
    sr_book_lodging(4, 7);
    sr_book_lodging(5, 3);
    sr_book_lodging(6, 2);
    sr_book_lodging(7, 6);

    print_str("\nAdding skiers to slopes...\n");
    for (int i = 0; i < 10; i++) {
        sr_add_skiers(i, 50 + i * 10);
    }

    print_str("\nSlope report...\n");
    sr_slope_report();

    print_str("\nFinancial report...\n");
    sr_financial_report();

    print_str("\nFinal state...\n");
    sr_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
