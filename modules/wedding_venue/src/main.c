/* wedding_venue: Wedding venue management (v1.0)
 * Venues, couples, services, catering, guests, revenue
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

#define MAX_VENUES   8
#define MAX_COUPLES  12
#define MAX_SERVICES 10
#define MAX_CATERING 8
#define MAX_GUESTS   20

typedef struct {
    int    venue_id;
    int    type;
    int    capacity;
    double base_price;
    int    n_bookings;
    double revenue;
    int    active;
} venue_t;

typedef struct {
    int    couple_id;
    int    venue_id;
    double budget;
    int    wedding_date;
    int    n_guests;
    double total_spent;
    int    active;
} couple_t;

typedef struct {
    int    service_id;
    int    type;
    double price;
    double cost;
    int    couple_id;
    int    booked;
    int    active;
} service_t;

typedef struct {
    int    cater_id;
    int    menu_type;
    double per_person;
    double cost_per_person;
    int    couple_id;
    int    n_tables;
    double total;
    int    active;
} catering_t;

typedef struct {
    int    guest_id;
    int    couple_id;
    int    rsvp;
    int    table_num;
    int    meal_choice;
    int    active;
} guest_t;

typedef struct {
    int    n_venues;
    int    n_couples;
    int    n_services;
    int    n_catering;
    int    n_guests;
    double venue_revenue;
    double service_revenue;
    double catering_revenue;
    double total_revenue;
    double total_costs;
    int    total_guests;
} wv_state_t;

static venue_t venues[MAX_VENUES];
static couple_t couples[MAX_COUPLES];
static service_t services[MAX_SERVICES];
static catering_t catering[MAX_CATERING];
static guest_t guests[MAX_GUESTS];
static wv_state_t wv;

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

int wv_init(void) {
    if (initialized) return -1;
    wv.n_venues = 0; wv.n_couples = 0; wv.n_services = 0;
    wv.n_catering = 0; wv.n_guests = 0;
    wv.venue_revenue = 0.0; wv.service_revenue = 0.0;
    wv.catering_revenue = 0.0; wv.total_revenue = 0.0;
    wv.total_costs = 0.0; wv.total_guests = 0;
    for (int i = 0; i < MAX_VENUES; i++) venues[i].active = 0;
    for (int i = 0; i < MAX_COUPLES; i++) couples[i].active = 0;
    for (int i = 0; i < MAX_SERVICES; i++) services[i].active = 0;
    for (int i = 0; i < MAX_CATERING; i++) catering[i].active = 0;
    for (int i = 0; i < MAX_GUESTS; i++) guests[i].active = 0;
    initialized = 1;
    print_str("[WV] Wedding venue initialized\n");
    return 0;
}

int wv_add_venue(int type, int capacity, double price) {
    if (wv.n_venues >= MAX_VENUES) return -1;
    venue_t* v = &venues[wv.n_venues];
    v->venue_id = wv.n_venues;
    v->type = type;
    v->capacity = capacity;
    v->base_price = price;
    v->n_bookings = 0;
    v->revenue = 0.0;
    v->active = 1;
    wv.n_venues++;
    print_str("[WV] Venue "); print_int(wv.n_venues - 1);
    print_str(" type="); print_int(type);
    print_str(" cap="); print_int(capacity);
    print_str(" price="); print_int((int)price); print_str("\n");
    return wv.n_venues - 1;
}

int wv_book_couple(int venue_id, double budget, int date, int n_guests) {
    if (wv.n_couples >= MAX_COUPLES || venue_id >= wv.n_venues) return -1;
    couple_t* c = &couples[wv.n_couples];
    c->couple_id = wv.n_couples;
    c->venue_id = venue_id;
    c->budget = budget;
    c->wedding_date = date;
    c->n_guests = n_guests;
    c->total_spent = venues[venue_id].base_price;
    c->active = 1;
    venues[venue_id].n_bookings++;
    venues[venue_id].revenue += venues[venue_id].base_price;
    wv.venue_revenue += venues[venue_id].base_price;
    wv.total_revenue += venues[venue_id].base_price;
    wv.n_couples++;
    print_str("[WV] Couple "); print_int(wv.n_couples - 1);
    print_str(" V"); print_int(venue_id);
    print_str(" budget="); print_int((int)budget);
    print_str(" date="); print_int(date);
    print_str(" guests="); print_int(n_guests); print_str("\n");
    return wv.n_couples - 1;
}

int wv_add_service(int type, double price, double cost, int couple_id) {
    if (wv.n_services >= MAX_SERVICES || couple_id >= wv.n_couples) return -1;
    service_t* s = &services[wv.n_services];
    s->service_id = wv.n_services;
    s->type = type;
    s->price = price;
    s->cost = cost;
    s->couple_id = couple_id;
    s->booked = 1;
    s->active = 1;
    couples[couple_id].total_spent += price;
    wv.service_revenue += price;
    wv.total_revenue += price;
    wv.total_costs += cost;
    wv.n_services++;
    print_str("[WV] Service "); print_int(wv.n_services - 1);
    print_str(" type="); print_int(type);
    print_str(" Cu"); print_int(couple_id);
    print_str(" price="); print_int((int)price); print_str("\n");
    return wv.n_services - 1;
}

int wv_add_catering(int menu_type, double per_person, double cost_pp, int couple_id, int n_tables) {
    if (wv.n_catering >= MAX_CATERING || couple_id >= wv.n_couples) return -1;
    catering_t* ct = &catering[wv.n_catering];
    ct->cater_id = wv.n_catering;
    ct->menu_type = menu_type;
    ct->per_person = per_person;
    ct->cost_per_person = cost_pp;
    ct->couple_id = couple_id;
    ct->n_tables = n_tables;
    int ppl = n_tables * 10;
    ct->total = ppl * per_person;
    ct->active = 1;
    couples[couple_id].total_spent += ct->total;
    wv.catering_revenue += ct->total;
    wv.total_revenue += ct->total;
    wv.total_costs += ppl * cost_pp;
    wv.n_catering++;
    print_str("[WV] Catering "); print_int(wv.n_catering - 1);
    print_str(" menu="); print_int(menu_type);
    print_str(" Cu"); print_int(couple_id);
    print_str(" tables="); print_int(n_tables);
    print_str(" total="); print_int((int)ct->total); print_str("\n");
    return wv.n_catering - 1;
}

int wv_add_guest(int couple_id, int rsvp, int table, int meal) {
    if (wv.n_guests >= MAX_GUESTS || couple_id >= wv.n_couples) return -1;
    guest_t* g = &guests[wv.n_guests];
    g->guest_id = wv.n_guests;
    g->couple_id = couple_id;
    g->rsvp = rsvp;
    g->table_num = table;
    g->meal_choice = meal;
    g->active = 1;
    if (rsvp) wv.total_guests++;
    wv.n_guests++;
    print_str("[WV] Guest "); print_int(wv.n_guests - 1);
    print_str(" Cu"); print_int(couple_id);
    print_str(" T"); print_int(table);
    print_str(" rsvp="); print_int(rsvp); print_str("\n");
    return wv.n_guests - 1;
}

void wv_venue_report(void) {
    print_str("[WV] Venue report:\n");
    for (int i = 0; i < wv.n_venues; i++) {
        venue_t* v = &venues[i];
        print_str("  V"); print_int(i);
        print_str(" type="); print_int(v->type);
        print_str(" cap="); print_int(v->capacity);
        print_str(" bookings="); print_int(v->n_bookings);
        print_str(" rev="); print_int((int)v->revenue); print_str("\n");
    }
}

void wv_financial_report(void) {
    print_str("[WV] Financial report:\n");
    print_str("  Venue revenue: "); print_int((int)wv.venue_revenue); print_str("\n");
    print_str("  Service revenue: "); print_int((int)wv.service_revenue); print_str("\n");
    print_str("  Catering revenue: "); print_int((int)wv.catering_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)wv.total_revenue); print_str("\n");
    print_str("  Costs: "); print_int((int)wv.total_costs); print_str("\n");
    double profit = wv.total_revenue - wv.total_costs;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    print_str("  Total guests: "); print_int(wv.total_guests); print_str("\n");
    print_str("  Couples: "); print_int(wv.n_couples); print_str("\n");
}

void wv_print_state(void) {
    print_str("[WV] Venues="); print_int(wv.n_venues);
    print_str(" Couples="); print_int(wv.n_couples);
    print_str(" Services="); print_int(wv.n_services);
    print_str(" Catering="); print_int(wv.n_catering);
    print_str(" Guests="); print_int(wv.n_guests);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)wv.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Wedding Venue Demo ===\n\n");
    wv_init();

    print_str("Adding venues...\n");
    wv_add_venue(1, 200, 15000.0);
    wv_add_venue(1, 150, 12000.0);
    wv_add_venue(2, 100, 8000.0);
    wv_add_venue(2, 80, 7000.0);
    wv_add_venue(3, 60, 10000.0);
    wv_add_venue(4, 120, 9000.0);
    wv_add_venue(1, 250, 18000.0);
    wv_add_venue(3, 50, 8500.0);

    print_str("\nBooking couples...\n");
    wv_book_couple(0, 50000.0, 20260615, 180);
    wv_book_couple(1, 35000.0, 20260622, 120);
    wv_book_couple(2, 25000.0, 20260706, 80);
    wv_book_couple(3, 20000.0, 20260713, 70);
    wv_book_couple(4, 30000.0, 20260720, 55);
    wv_book_couple(5, 28000.0, 20260803, 100);
    wv_book_couple(6, 60000.0, 20260817, 220);
    wv_book_couple(7, 22000.0, 20260824, 45);
    wv_book_couple(0, 45000.0, 20260907, 160);
    wv_book_couple(1, 32000.0, 20260914, 130);
    wv_book_couple(2, 23000.0, 20260928, 75);
    wv_book_couple(3, 18000.0, 20261012, 60);

    print_str("\nAdding services...\n");
    wv_add_service(1, 2500.0, 800.0, 0);
    wv_add_service(2, 3500.0, 1200.0, 0);
    wv_add_service(3, 1500.0, 500.0, 1);
    wv_add_service(1, 2000.0, 700.0, 2);
    wv_add_service(4, 1800.0, 600.0, 3);
    wv_add_service(2, 3000.0, 1000.0, 4);
    wv_add_service(5, 1200.0, 400.0, 5);
    wv_add_service(3, 1600.0, 550.0, 6);
    wv_add_service(1, 2200.0, 750.0, 7);
    wv_add_service(4, 1700.0, 580.0, 8);

    print_str("\nAdding catering...\n");
    wv_add_catering(1, 85.0, 35.0, 0, 18);
    wv_add_catering(2, 75.0, 30.0, 1, 12);
    wv_add_catering(1, 80.0, 32.0, 2, 8);
    wv_add_catering(3, 95.0, 40.0, 3, 7);
    wv_add_catering(2, 70.0, 28.0, 4, 6);
    wv_add_catering(1, 82.0, 33.0, 5, 10);
    wv_add_catering(3, 100.0, 42.0, 6, 22);
    wv_add_catering(2, 68.0, 27.0, 7, 5);
    wv_add_catering(1, 83.0, 34.0, 8, 16);
    wv_add_catering(2, 72.0, 29.0, 9, 13);
    wv_add_catering(3, 90.0, 38.0, 10, 8);
    wv_add_catering(1, 78.0, 31.0, 11, 6);

    print_str("\nAdding guests...\n");
    for (int i = 0; i < 20; i++) {
        wv_add_guest(i % 12, (i % 5 != 0) ? 1 : 0, (i / 3) + 1, (i % 3) + 1);
    }

    print_str("\nVenue report...\n");
    wv_venue_report();

    print_str("\nFinancial report...\n");
    wv_financial_report();

    print_str("\nFinal state...\n");
    wv_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
