/* museum_gallery: Museum and gallery management (v1.0)
 * Exhibits, halls, tickets, events, visitors, revenue
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

#define MAX_EXHIBITS   12
#define MAX_HALLS      6
#define MAX_TICKETS    18
#define MAX_EVENTS     8
#define MAX_VISITORS   14

typedef struct {
    int    exhibit_id;
    int    type;
    int    hall_id;
    int    year;
    double value;
    double insurance;
    int    views;
    int    active;
} exhibit_t;

typedef struct {
    int    hall_id;
    int    type;
    int    capacity;
    int    temperature;
    int    humidity;
    int    n_exhibits;
    int    n_visitors;
    double revenue;
    int    active;
} hall_t;

typedef struct {
    int    ticket_id;
    int    visitor_id;
    int    type;
    double price;
    int    guided_tour;
    int    audio_guide;
    int    day;
    int    active;
} ticket_t;

typedef struct {
    int    event_id;
    int    type;
    int    hall_id;
    int    capacity;
    int    n_attendees;
    double fee;
    double revenue;
    int    day;
    int    active;
} event_t;

typedef struct {
    int    visitor_id;
    int    age_group;
    int    n_visits;
    int    member;
    double total_spent;
    int    satisfaction;
    int    active;
} visitor_t;

typedef struct {
    int    n_exhibits;
    int    n_halls;
    int    n_tickets;
    int    n_events;
    int    n_visitors;
    double ticket_revenue;
    double event_revenue;
    double giftshop_revenue;
    double donation_revenue;
    double total_revenue;
    double total_costs;
    int    total_views;
} mg_state_t;

static exhibit_t exhibits[MAX_EXHIBITS];
static hall_t halls[MAX_HALLS];
static ticket_t tickets[MAX_TICKETS];
static event_t events[MAX_EVENTS];
static visitor_t visitors[MAX_VISITORS];
static mg_state_t mg;

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

int mg_init(void) {
    if (initialized) return -1;
    mg.n_exhibits = 0; mg.n_halls = 0; mg.n_tickets = 0;
    mg.n_events = 0; mg.n_visitors = 0;
    mg.ticket_revenue = 0.0; mg.event_revenue = 0.0;
    mg.giftshop_revenue = 0.0; mg.donation_revenue = 0.0;
    mg.total_revenue = 0.0; mg.total_costs = 0.0;
    mg.total_views = 0;
    for (int i = 0; i < MAX_EXHIBITS; i++) exhibits[i].active = 0;
    for (int i = 0; i < MAX_HALLS; i++) halls[i].active = 0;
    for (int i = 0; i < MAX_TICKETS; i++) tickets[i].active = 0;
    for (int i = 0; i < MAX_EVENTS; i++) events[i].active = 0;
    for (int i = 0; i < MAX_VISITORS; i++) visitors[i].active = 0;
    initialized = 1;
    print_str("[MG] Museum initialized\n");
    return 0;
}

int mg_add_hall(int type, int capacity, int temp, int humidity) {
    if (mg.n_halls >= MAX_HALLS) return -1;
    hall_t* h = &halls[mg.n_halls];
    h->hall_id = mg.n_halls;
    h->type = type;
    h->capacity = capacity;
    h->temperature = temp;
    h->humidity = humidity;
    h->n_exhibits = 0;
    h->n_visitors = 0;
    h->revenue = 0.0;
    h->active = 1;
    mg.n_halls++;
    print_str("[MG] Hall "); print_int(mg.n_halls - 1);
    print_str(" type="); print_int(type);
    print_str(" cap="); print_int(capacity);
    print_str(" temp="); print_int(temp); print_str("C\n");
    return mg.n_halls - 1;
}

int mg_add_exhibit(int type, int hall_id, int year, double value) {
    if (mg.n_exhibits >= MAX_EXHIBITS || hall_id >= mg.n_halls) return -1;
    exhibit_t* e = &exhibits[mg.n_exhibits];
    e->exhibit_id = mg.n_exhibits;
    e->type = type;
    e->hall_id = hall_id;
    e->year = year;
    e->value = value;
    e->insurance = value * 0.02;
    e->views = 0;
    e->active = 1;
    halls[hall_id].n_exhibits++;
    mg.total_costs += e->insurance;
    mg.n_exhibits++;
    print_str("[MG] Exhibit "); print_int(mg.n_exhibits - 1);
    print_str(" type="); print_int(type);
    print_str(" H"); print_int(hall_id);
    print_str(" year="); print_int(year);
    print_str(" val="); print_int((int)value); print_str("\n");
    return mg.n_exhibits - 1;
}

int mg_register_visitor(int age_group, int member) {
    if (mg.n_visitors >= MAX_VISITORS) return -1;
    visitor_t* v = &visitors[mg.n_visitors];
    v->visitor_id = mg.n_visitors;
    v->age_group = age_group;
    v->n_visits = 0;
    v->member = member;
    v->total_spent = 0.0;
    v->satisfaction = 85;
    v->active = 1;
    mg.n_visitors++;
    print_str("[MG] Visitor "); print_int(mg.n_visitors - 1);
    print_str(" age="); print_int(age_group);
    if (member) print_str(" [MEMBER]");
    print_str("\n");
    return mg.n_visitors - 1;
}

int mg_add_event(int type, int hall_id, int capacity, double fee, int day) {
    if (mg.n_events >= MAX_EVENTS || hall_id >= mg.n_halls) return -1;
    event_t* ev = &events[mg.n_events];
    ev->event_id = mg.n_events;
    ev->type = type;
    ev->hall_id = hall_id;
    ev->capacity = capacity;
    ev->n_attendees = 0;
    ev->fee = fee;
    ev->revenue = 0.0;
    ev->day = day;
    ev->active = 1;
    mg.n_events++;
    print_str("[MG] Event "); print_int(mg.n_events - 1);
    print_str(" type="); print_int(type);
    print_str(" H"); print_int(hall_id);
    print_str(" cap="); print_int(capacity);
    print_str(" fee="); print_int((int)fee);
    print_str(" day="); print_int(day); print_str("\n");
    return mg.n_events - 1;
}

int mg_sell_ticket(int visitor_id, int type, int guided, int audio, int day) {
    if (mg.n_tickets >= MAX_TICKETS || visitor_id >= mg.n_visitors) return -1;
    double prices[4] = {15.0, 10.0, 8.0, 12.0};
    double price = prices[type < 4 ? type : 0];
    visitor_t* v = &visitors[visitor_id];
    if (v->member) price *= 0.7;
    if (guided) price += 8.0;
    if (audio) price += 5.0;
    ticket_t* t = &tickets[mg.n_tickets];
    t->ticket_id = mg.n_tickets;
    t->visitor_id = visitor_id;
    t->type = type;
    t->price = price;
    t->guided_tour = guided;
    t->audio_guide = audio;
    t->day = day;
    t->active = 1;
    v->n_visits++;
    v->total_spent += price;
    mg.ticket_revenue += price;
    mg.total_revenue += price;
    mg.n_tickets++;
    print_str("[MG] Ticket "); print_int(mg.n_tickets - 1);
    print_str(" V"); print_int(visitor_id);
    print_str(" type="); print_int(type);
    print_str(" price="); print_int((int)price);
    if (guided) print_str(" [GUIDED]");
    if (audio) print_str(" [AUDIO]");
    print_str("\n");
    return mg.n_tickets - 1;
}

int mg_attend_event(int event_id, int visitor_id) {
    if (event_id >= mg.n_events || visitor_id >= mg.n_visitors) return -1;
    event_t* ev = &events[event_id];
    if (ev->n_attendees >= ev->capacity) return -2;
    ev->n_attendees++;
    ev->revenue += ev->fee;
    visitors[visitor_id].total_spent += ev->fee;
    mg.event_revenue += ev->fee;
    mg.total_revenue += ev->fee;
    print_str("[MG] V"); print_int(visitor_id);
    print_str(" attended E"); print_int(event_id);
    print_str(" fee="); print_int((int)ev->fee); print_str("\n");
    return 0;
}

int mg_view_exhibit(int exhibit_id, int visitor_id) {
    if (exhibit_id >= mg.n_exhibits || visitor_id >= mg.n_visitors) return -1;
    exhibits[exhibit_id].views++;
    halls[exhibits[exhibit_id].hall_id].n_visitors++;
    mg.total_views++;
    print_str("[MG] V"); print_int(visitor_id);
    print_str(" viewed Ex"); print_int(exhibit_id); print_str("\n");
    return 0;
}

int mg_giftshop_sale(int visitor_id, double amount) {
    if (visitor_id >= mg.n_visitors) return -1;
    visitors[visitor_id].total_spent += amount;
    mg.giftshop_revenue += amount;
    mg.total_revenue += amount;
    print_str("[MG] Giftshop V"); print_int(visitor_id);
    print_str(" amount="); print_int((int)amount); print_str("\n");
    return 0;
}

int mg_donation(double amount) {
    mg.donation_revenue += amount;
    mg.total_revenue += amount;
    print_str("[MG] Donation amount="); print_int((int)amount); print_str("\n");
    return 0;
}

void mg_exhibit_report(void) {
    print_str("[MG] Exhibit report:\n");
    for (int i = 0; i < mg.n_exhibits; i++) {
        exhibit_t* e = &exhibits[i];
        print_str("  Ex"); print_int(i);
        print_str(" type="); print_int(e->type);
        print_str(" H"); print_int(e->hall_id);
        print_str(" year="); print_int(e->year);
        print_str(" views="); print_int(e->views); print_str("\n");
    }
}

void mg_financial_report(void) {
    print_str("[MG] Financial report:\n");
    print_str("  Tickets: "); print_int((int)mg.ticket_revenue); print_str("\n");
    print_str("  Events: "); print_int((int)mg.event_revenue); print_str("\n");
    print_str("  Giftshop: "); print_int((int)mg.giftshop_revenue); print_str("\n");
    print_str("  Donations: "); print_int((int)mg.donation_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)mg.total_revenue); print_str("\n");
    print_str("  Costs (insurance): "); print_int((int)mg.total_costs); print_str("\n");
    double net = mg.total_revenue - mg.total_costs;
    print_str("  Net: "); print_int((int)net); print_str("\n");
    print_str("  Total views: "); print_int(mg.total_views); print_str("\n");
}

void mg_print_state(void) {
    print_str("[MG] Exhibits="); print_int(mg.n_exhibits);
    print_str(" Halls="); print_int(mg.n_halls);
    print_str(" Tickets="); print_int(mg.n_tickets);
    print_str(" Events="); print_int(mg.n_events);
    print_str(" Visitors="); print_int(mg.n_visitors);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)mg.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Museum Gallery Demo ===\n\n");
    mg_init();

    print_str("Adding halls...\n");
    mg_add_hall(1, 50, 21, 45);
    mg_add_hall(2, 40, 20, 50);
    mg_add_hall(3, 30, 22, 40);
    mg_add_hall(4, 60, 19, 55);
    mg_add_hall(1, 45, 21, 45);
    mg_add_hall(2, 35, 20, 50);

    print_str("\nAdding exhibits...\n");
    mg_add_exhibit(1, 0, 1850, 50000.0);
    mg_add_exhibit(1, 0, 1920, 35000.0);
    mg_add_exhibit(2, 1, 1600, 120000.0);
    mg_add_exhibit(2, 1, 1750, 80000.0);
    mg_add_exhibit(3, 2, 1950, 25000.0);
    mg_add_exhibit(3, 2, 1970, 18000.0);
    mg_add_exhibit(4, 3, 2000, 45000.0);
    mg_add_exhibit(4, 3, 2010, 30000.0);
    mg_add_exhibit(1, 4, 1880, 60000.0);
    mg_add_exhibit(2, 5, 1700, 95000.0);
    mg_add_exhibit(3, 0, 1960, 22000.0);
    mg_add_exhibit(4, 1, 2015, 40000.0);

    print_str("\nRegistering visitors...\n");
    for (int i = 0; i < 14; i++) {
        mg_register_visitor(i % 4, i % 3 == 0);
    }

    print_str("\nAdding events...\n");
    mg_add_event(1, 0, 30, 25.0, 15);
    mg_add_event(2, 1, 20, 35.0, 16);
    mg_add_event(3, 2, 15, 15.0, 17);
    mg_add_event(1, 3, 40, 30.0, 18);
    mg_add_event(4, 4, 25, 20.0, 19);
    mg_add_event(2, 5, 18, 40.0, 20);
    mg_add_event(3, 0, 12, 18.0, 21);
    mg_add_event(1, 1, 35, 28.0, 22);

    print_str("\nSelling tickets...\n");
    for (int i = 0; i < 14; i++) {
        mg_sell_ticket(i, i % 4, i % 3 == 0, i % 2 == 0, 10 + i / 5);
    }
    mg_sell_ticket(0, 0, 1, 0, 13);
    mg_sell_ticket(1, 1, 0, 1, 13);
    mg_sell_ticket(2, 2, 1, 1, 14);
    mg_sell_ticket(3, 3, 0, 0, 14);

    print_str("\nViewing exhibits...\n");
    for (int e = 0; e < 12; e++) {
        mg_view_exhibit(e, e % 14);
    }
    mg_view_exhibit(0, 5);
    mg_view_exhibit(2, 7);
    mg_view_exhibit(4, 9);
    mg_view_exhibit(6, 11);
    mg_view_exhibit(8, 13);
    mg_view_exhibit(10, 1);

    print_str("\nAttending events...\n");
    mg_attend_event(0, 0);
    mg_attend_event(0, 1);
    mg_attend_event(0, 2);
    mg_attend_event(1, 3);
    mg_attend_event(1, 4);
    mg_attend_event(2, 5);
    mg_attend_event(2, 6);
    mg_attend_event(3, 7);
    mg_attend_event(3, 8);
    mg_attend_event(4, 9);
    mg_attend_event(4, 10);
    mg_attend_event(5, 11);
    mg_attend_event(6, 12);
    mg_attend_event(7, 13);

    print_str("\nGiftshop sales...\n");
    mg_giftshop_sale(0, 15.0);
    mg_giftshop_sale(2, 25.0);
    mg_giftshop_sale(4, 12.0);
    mg_giftshop_sale(6, 30.0);
    mg_giftshop_sale(8, 18.0);
    mg_giftshop_sale(10, 22.0);
    mg_giftshop_sale(12, 20.0);

    print_str("\nDonations...\n");
    mg_donation(500.0);
    mg_donation(1000.0);
    mg_donation(250.0);
    mg_donation(750.0);

    print_str("\nExhibit report...\n");
    mg_exhibit_report();

    print_str("\nFinancial report...\n");
    mg_financial_report();

    print_str("\nFinal state...\n");
    mg_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
