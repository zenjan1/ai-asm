/* event_management: Event management system (v1.0)
 * Venue management, event planning, ticketing, attendee ops, analytics
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

#define MAX_VENUES    8
#define MAX_EVENTS    12
#define MAX_TICKETS   20
#define MAX_ATTENDEES 16
#define MAX_GUESTS    10
#define MAX_FEEDBACK  12

typedef struct {
    int    venue_id;
    int    type;
    int    capacity;
    int    n_seats;
    int    equipment;
    double rental_fee;
    int    bookings;
    int    active;
} venue_t;

typedef struct {
    int    event_id;
    int    venue_id;
    int    type;
    int    day;
    int    duration_hr;
    int    capacity;
    int    sold;
    double revenue;
    double budget;
    int    status;
    int    active;
} event_t;

typedef struct {
    int    ticket_id;
    int    event_id;
    int    attendee_id;
    double price;
    int    seat_num;
    int    checked_in;
    int    active;
} ticket_t;

typedef struct {
    int    attendee_id;
    int    name_hash;
    int    n_events;
    double total_spent;
    int    vip;
    int    active;
} attendee_t;

typedef struct {
    int    guest_id;
    int    event_id;
    int    attendee_id;
    int    role;
    int    fee;
    int    confirmed;
    int    active;
} guest_t;

typedef struct {
    int    fb_id;
    int    event_id;
    int    attendee_id;
    int    rating;
    int    satisfaction;
    int    active;
} feedback_t;

typedef struct {
    int    n_venues;
    int    n_events;
    int    n_tickets;
    int    n_attendees;
    int    n_guests;
    int    n_feedback;
    double total_revenue;
    double total_costs;
    int    total_checked_in;
    int    total_attendees;
} em_state_t;

static venue_t venues[MAX_VENUES];
static event_t events[MAX_EVENTS];
static ticket_t tickets[MAX_TICKETS];
static attendee_t attendees[MAX_ATTENDEES];
static guest_t guests[MAX_GUESTS];
static feedback_t feedbacks[MAX_FEEDBACK];
static em_state_t em;

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

int em_init(void) {
    if (initialized) return -1;
    em.n_venues = 0; em.n_events = 0; em.n_tickets = 0;
    em.n_attendees = 0; em.n_guests = 0; em.n_feedback = 0;
    em.total_revenue = 0.0; em.total_costs = 0.0;
    em.total_checked_in = 0; em.total_attendees = 0;
    for (int i = 0; i < MAX_VENUES; i++) venues[i].active = 0;
    for (int i = 0; i < MAX_EVENTS; i++) events[i].active = 0;
    for (int i = 0; i < MAX_TICKETS; i++) tickets[i].active = 0;
    for (int i = 0; i < MAX_ATTENDEES; i++) attendees[i].active = 0;
    for (int i = 0; i < MAX_GUESTS; i++) guests[i].active = 0;
    for (int i = 0; i < MAX_FEEDBACK; i++) feedbacks[i].active = 0;
    initialized = 1;
    print_str("[EM] Event management initialized\n");
    return 0;
}

int em_add_venue(int type, int capacity, int seats, int equip, double fee) {
    if (em.n_venues >= MAX_VENUES) return -1;
    venue_t* v = &venues[em.n_venues];
    v->venue_id = em.n_venues;
    v->type = type;
    v->capacity = capacity;
    v->n_seats = seats;
    v->equipment = equip;
    v->rental_fee = fee;
    v->bookings = 0;
    v->active = 1;
    em.n_venues++;
    print_str("[EM] Venue "); print_int(em.n_venues - 1);
    print_str(" type="); print_int(type);
    print_str(" cap="); print_int(capacity);
    print_str(" seats="); print_int(seats);
    print_str(" fee="); print_int((int)fee); print_str("\n");
    return em.n_venues - 1;
}

int em_create_event(int venue_id, int type, int day, int duration, double budget) {
    if (em.n_events >= MAX_EVENTS || venue_id >= em.n_venues) return -1;
    event_t* e = &events[em.n_events];
    e->event_id = em.n_events;
    e->venue_id = venue_id;
    e->type = type;
    e->day = day;
    e->duration_hr = duration;
    e->capacity = venues[venue_id].capacity;
    e->sold = 0;
    e->revenue = 0.0;
    e->budget = budget;
    e->status = 1;
    e->active = 1;
    venues[venue_id].bookings++;
    em.total_costs += budget + venues[venue_id].rental_fee;
    em.n_events++;
    print_str("[EM] Event "); print_int(em.n_events - 1);
    print_str(" V"); print_int(venue_id);
    print_str(" type="); print_int(type);
    print_str(" day="); print_int(day);
    print_str(" dur="); print_int(duration); print_str("hr");
    print_str(" budget="); print_int((int)budget); print_str("\n");
    return em.n_events - 1;
}

int em_register_attendee(int name_hash, int vip) {
    if (em.n_attendees >= MAX_ATTENDEES) return -1;
    attendee_t* a = &attendees[em.n_attendees];
    a->attendee_id = em.n_attendees;
    a->name_hash = name_hash;
    a->n_events = 0;
    a->total_spent = 0.0;
    a->vip = vip;
    a->active = 1;
    em.n_attendees++;
    print_str("[EM] Attendee "); print_int(em.n_attendees - 1);
    if (vip) print_str(" [VIP]");
    print_str("\n");
    return em.n_attendees - 1;
}

int em_add_guest(int event_id, int attendee_id, int role, int fee) {
    if (em.n_guests >= MAX_GUESTS) return -1;
    if (event_id >= em.n_events || attendee_id >= em.n_attendees) return -2;
    guest_t* g = &guests[em.n_guests];
    g->guest_id = em.n_guests;
    g->event_id = event_id;
    g->attendee_id = attendee_id;
    g->role = role;
    g->fee = fee;
    g->confirmed = 0;
    g->active = 1;
    em.n_guests++;
    em.total_costs += fee;
    print_str("[EM] Guest "); print_int(em.n_guests - 1);
    print_str(" E"); print_int(event_id);
    print_str(" A"); print_int(attendee_id);
    print_str(" role="); print_int(role);
    print_str(" fee="); print_int(fee); print_str("\n");
    return em.n_guests - 1;
}

void em_confirm_guest(int guest_id) {
    if (guest_id >= em.n_guests) return;
    guests[guest_id].confirmed = 1;
    print_str("[EM] Guest "); print_int(guest_id); print_str(" confirmed\n");
}

int em_sell_ticket(int event_id, int attendee_id, double price) {
    if (em.n_tickets >= MAX_TICKETS) return -1;
    if (event_id >= em.n_events || attendee_id >= em.n_attendees) return -2;
    event_t* e = &events[event_id];
    if (e->sold >= e->capacity) return -3;
    ticket_t* t = &tickets[em.n_tickets];
    t->ticket_id = em.n_tickets;
    t->event_id = event_id;
    t->attendee_id = attendee_id;
    t->price = price;
    t->seat_num = e->sold + 1;
    t->checked_in = 0;
    t->active = 1;
    e->sold++;
    e->revenue += price;
    attendees[attendee_id].n_events++;
    attendees[attendee_id].total_spent += price;
    em.total_revenue += price;
    em.total_attendees++;
    em.n_tickets++;
    print_str("[EM] Ticket "); print_int(em.n_tickets - 1);
    print_str(" E"); print_int(event_id);
    print_str(" A"); print_int(attendee_id);
    print_str(" seat="); print_int(t->seat_num);
    print_str(" price="); print_int((int)price); print_str("\n");
    return em.n_tickets - 1;
}

void em_checkin(int ticket_id) {
    if (ticket_id >= em.n_tickets) return;
    ticket_t* t = &tickets[ticket_id];
    t->checked_in = 1;
    em.total_checked_in++;
    print_str("[EM] Check-in T"); print_int(ticket_id);
    print_str(" A"); print_int(t->attendee_id);
    print_str(" E"); print_int(t->event_id);
    print_str(" seat="); print_int(t->seat_num); print_str("\n");
}

int em_add_feedback(int event_id, int attendee_id, int rating, int satisfaction) {
    if (em.n_feedback >= MAX_FEEDBACK) return -1;
    if (event_id >= em.n_events || attendee_id >= em.n_attendees) return -2;
    feedback_t* f = &feedbacks[em.n_feedback];
    f->fb_id = em.n_feedback;
    f->event_id = event_id;
    f->attendee_id = attendee_id;
    f->rating = rating;
    f->satisfaction = satisfaction;
    f->active = 1;
    em.n_feedback++;
    print_str("[EM] Feedback "); print_int(em.n_feedback - 1);
    print_str(" E"); print_int(event_id);
    print_str(" A"); print_int(attendee_id);
    print_str(" rating="); print_int(rating);
    print_str(" satisfaction="); print_int(satisfaction); print_str("/10\n");
    return em.n_feedback - 1;
}

void em_complete_event(int event_id) {
    if (event_id >= em.n_events) return;
    events[event_id].status = 3;
    print_str("[EM] Event "); print_int(event_id); print_str(" completed\n");
    print_str("  Sold: "); print_int(events[event_id].sold);
    print_str("/"); print_int(events[event_id].capacity); print_str("\n");
    print_str("  Revenue: "); print_int((int)events[event_id].revenue); print_str("\n");
}

void em_venue_report(void) {
    print_str("[EM] Venue report:\n");
    for (int i = 0; i < em.n_venues; i++) {
        venue_t* v = &venues[i];
        print_str("  V"); print_int(i);
        print_str(" type="); print_int(v->type);
        print_str(" bookings="); print_int(v->bookings);
        print_str(" cap="); print_int(v->capacity); print_str("\n");
    }
}

void em_financial_report(void) {
    print_str("[EM] Financial report:\n");
    print_str("  Total revenue: "); print_int((int)em.total_revenue); print_str("\n");
    print_str("  Total costs: "); print_int((int)em.total_costs); print_str("\n");
    double profit = em.total_revenue - em.total_costs;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    double margin = (em.total_revenue > 0) ? profit / em.total_revenue * 100.0 : 0.0;
    print_str("  Margin: "); print_int((int)margin); print_str("%\n");
    print_str("  Check-in rate: ");
    int rate = (em.total_attendees > 0) ? em.total_checked_in * 100 / em.total_attendees : 0;
    print_int(rate); print_str("%\n");
}

void em_print_state(void) {
    print_str("[EM] Venues="); print_int(em.n_venues);
    print_str(" Events="); print_int(em.n_events);
    print_str(" Tickets="); print_int(em.n_tickets);
    print_str(" Attendees="); print_int(em.n_attendees);
    print_str(" Guests="); print_int(em.n_guests);
    print_str(" Feedback="); print_int(em.n_feedback);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)em.total_revenue);
    print_str(" Checked in: "); print_int(em.total_checked_in); print_str("\n");
}

int main(void) {
    print_str("=== Event Management System Demo ===\n\n");
    em_init();

    print_str("Adding venues...\n");
    em_add_venue(1, 200, 200, 3, 500.0);
    em_add_venue(1, 500, 500, 5, 1200.0);
    em_add_venue(2, 100, 100, 2, 300.0);
    em_add_venue(2, 300, 300, 4, 800.0);
    em_add_venue(3, 50, 50, 2, 200.0);

    print_str("\nCreating events...\n");
    em_create_event(0, 1, 15, 3, 2000.0);
    em_create_event(1, 2, 16, 5, 5000.0);
    em_create_event(2, 1, 17, 2, 1500.0);
    em_create_event(3, 3, 18, 4, 3000.0);
    em_create_event(4, 1, 19, 3, 1000.0);
    em_create_event(0, 2, 20, 4, 2500.0);

    print_str("\nRegistering attendees...\n");
    em_register_attendee(1001, 0);
    em_register_attendee(1002, 1);
    em_register_attendee(1003, 0);
    em_register_attendee(1004, 1);
    em_register_attendee(1005, 0);
    em_register_attendee(1006, 0);
    em_register_attendee(1007, 1);
    em_register_attendee(1008, 0);
    em_register_attendee(1009, 1);
    em_register_attendee(1010, 0);
    em_register_attendee(1011, 0);
    em_register_attendee(1012, 1);

    print_str("\nAdding guests...\n");
    em_add_guest(0, 1, 1, 500);
    em_add_guest(0, 3, 2, 300);
    em_add_guest(1, 6, 1, 800);
    em_add_guest(1, 8, 3, 600);
    em_add_guest(2, 9, 2, 400);
    em_add_guest(3, 11, 1, 700);

    print_str("\nConfirming guests...\n");
    em_confirm_guest(0);
    em_confirm_guest(1);
    em_confirm_guest(2);
    em_confirm_guest(3);
    em_confirm_guest(4);
    em_confirm_guest(5);

    print_str("\nSelling tickets...\n");
    em_sell_ticket(0, 0, 50.0);
    em_sell_ticket(0, 2, 50.0);
    em_sell_ticket(0, 4, 75.0);
    em_sell_ticket(0, 5, 50.0);
    em_sell_ticket(1, 1, 100.0);
    em_sell_ticket(1, 3, 100.0);
    em_sell_ticket(1, 6, 150.0);
    em_sell_ticket(1, 7, 100.0);
    em_sell_ticket(1, 8, 150.0);
    em_sell_ticket(2, 0, 40.0);
    em_sell_ticket(2, 9, 40.0);
    em_sell_ticket(3, 1, 80.0);
    em_sell_ticket(3, 10, 80.0);
    em_sell_ticket(3, 11, 120.0);
    em_sell_ticket(4, 2, 45.0);
    em_sell_ticket(4, 4, 45.0);
    em_sell_ticket(4, 5, 65.0);
    em_sell_ticket(5, 3, 70.0);
    em_sell_ticket(5, 6, 70.0);
    em_sell_ticket(5, 7, 70.0);

    print_str("\nChecking in attendees...\n");
    em_checkin(0);
    em_checkin(1);
    em_checkin(2);
    em_checkin(3);
    em_checkin(4);
    em_checkin(5);
    em_checkin(6);
    em_checkin(7);
    em_checkin(8);
    em_checkin(9);
    em_checkin(10);
    em_checkin(11);
    em_checkin(12);
    em_checkin(14);
    em_checkin(15);
    em_checkin(16);
    em_checkin(17);
    em_checkin(18);

    print_str("\nCompleting events...\n");
    em_complete_event(0);
    em_complete_event(1);
    em_complete_event(2);
    em_complete_event(3);
    em_complete_event(4);
    em_complete_event(5);

    print_str("\nCollecting feedback...\n");
    em_add_feedback(0, 0, 8, 8);
    em_add_feedback(0, 2, 9, 9);
    em_add_feedback(1, 1, 9, 9);
    em_add_feedback(1, 3, 10, 10);
    em_add_feedback(2, 9, 7, 7);
    em_add_feedback(3, 11, 8, 8);
    em_add_feedback(4, 4, 9, 9);
    em_add_feedback(5, 6, 7, 7);

    print_str("\nVenue report...\n");
    em_venue_report();

    print_str("\nFinancial report...\n");
    em_financial_report();

    print_str("\nFinal state...\n");
    em_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
