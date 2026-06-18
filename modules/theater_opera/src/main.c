/* theater_opera: Theater and opera house management (v1.0)
 * Shows, performers, tickets, stage, productions, revenue
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

#define MAX_SHOWS       10
#define MAX_PERFORMERS  14
#define MAX_TICKETS     20
#define MAX_PRODUCTIONS 6
#define MAX_CREW        10
#define MAX_REHEARSALS  12

typedef struct {
    int    show_id;
    int    type;
    int    production_id;
    int    day;
    int    hour;
    int    capacity;
    int    seats_sold;
    double revenue;
    int    active;
} show_t;

typedef struct {
    int    performer_id;
    int    role;
    int    production_id;
    int    n_performances;
    double fee_per_show;
    double total_earned;
    int    rating;
    int    active;
} performer_t;

typedef struct {
    int    ticket_id;
    int    show_id;
    int    seat_class;
    double price;
    int    qty;
    double total;
    int    active;
} ticket_t;

typedef struct {
    int    prod_id;
    int    type;
    int    director_id;
    int    n_performers;
    int    n_rehearsals;
    int    n_shows;
    double budget;
    double spent;
    double revenue;
    int    active;
} production_t;

typedef struct {
    int    crew_id;
    int    role;
    int    production_id;
    int    n_shifts;
    int    total_hours;
    double hourly_rate;
    double total_paid;
    int    active;
} crew_t;

typedef struct {
    int    rehearsal_id;
    int    production_id;
    int    day;
    int    hours;
    int    n_performers;
    double cost;
    int    active;
} rehearsal_t;

typedef struct {
    int    n_shows;
    int    n_performers;
    int    n_tickets;
    int    n_productions;
    int    n_crew;
    int    n_rehearsals;
    double box_office;
    double sponsorship;
    double merch_revenue;
    double total_revenue;
    double performer_costs;
    double crew_costs;
    double production_costs;
    int    total_attendance;
} to_state_t;

static show_t shows[MAX_SHOWS];
static performer_t performers[MAX_PERFORMERS];
static ticket_t tickets[MAX_TICKETS];
static production_t productions[MAX_PRODUCTIONS];
static crew_t crew[MAX_CREW];
static rehearsal_t rehearsals[MAX_REHEARSALS];
static to_state_t ts;

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

int ts_init(void) {
    if (initialized) return -1;
    ts.n_shows = 0; ts.n_performers = 0; ts.n_tickets = 0;
    ts.n_productions = 0; ts.n_crew = 0; ts.n_rehearsals = 0;
    ts.box_office = 0.0; ts.sponsorship = 0.0;
    ts.merch_revenue = 0.0; ts.total_revenue = 0.0;
    ts.performer_costs = 0.0; ts.crew_costs = 0.0;
    ts.production_costs = 0.0; ts.total_attendance = 0;
    for (int i = 0; i < MAX_SHOWS; i++) shows[i].active = 0;
    for (int i = 0; i < MAX_PERFORMERS; i++) performers[i].active = 0;
    for (int i = 0; i < MAX_TICKETS; i++) tickets[i].active = 0;
    for (int i = 0; i < MAX_PRODUCTIONS; i++) productions[i].active = 0;
    for (int i = 0; i < MAX_CREW; i++) crew[i].active = 0;
    for (int i = 0; i < MAX_REHEARSALS; i++) rehearsals[i].active = 0;
    initialized = 1;
    print_str("[TO] Theater initialized\n");
    return 0;
}

int ts_add_production(int type, int director_id, double budget) {
    if (ts.n_productions >= MAX_PRODUCTIONS) return -1;
    production_t* p = &productions[ts.n_productions];
    p->prod_id = ts.n_productions;
    p->type = type;
    p->director_id = director_id;
    p->n_performers = 0;
    p->n_rehearsals = 0;
    p->n_shows = 0;
    p->budget = budget;
    p->spent = 0.0;
    p->revenue = 0.0;
    p->active = 1;
    ts.n_productions++;
    print_str("[TO] Production "); print_int(ts.n_productions - 1);
    print_str(" type="); print_int(type);
    print_str(" dir="); print_int(director_id);
    print_str(" budget="); print_int((int)budget); print_str("\n");
    return ts.n_productions - 1;
}

int ts_add_performer(int role, int prod_id, double fee, int rating) {
    if (ts.n_performers >= MAX_PERFORMERS || prod_id >= ts.n_productions) return -1;
    performer_t* p = &performers[ts.n_performers];
    p->performer_id = ts.n_performers;
    p->role = role;
    p->production_id = prod_id;
    p->n_performances = 0;
    p->fee_per_show = fee;
    p->total_earned = 0.0;
    p->rating = rating;
    p->active = 1;
    productions[prod_id].n_performers++;
    ts.n_performers++;
    print_str("[TO] Performer "); print_int(ts.n_performers - 1);
    print_str(" role="); print_int(role);
    print_str(" P"); print_int(prod_id);
    print_str(" fee="); print_int((int)fee);
    print_str(" rating="); print_int(rating); print_str("\n");
    return ts.n_performers - 1;
}

int ts_add_crew(int role, int prod_id, double rate) {
    if (ts.n_crew >= MAX_CREW || prod_id >= ts.n_productions) return -1;
    crew_t* c = &crew[ts.n_crew];
    c->crew_id = ts.n_crew;
    c->role = role;
    c->production_id = prod_id;
    c->n_shifts = 0;
    c->total_hours = 0;
    c->hourly_rate = rate;
    c->total_paid = 0.0;
    c->active = 1;
    ts.n_crew++;
    print_str("[TO] Crew "); print_int(ts.n_crew - 1);
    print_str(" role="); print_int(role);
    print_str(" P"); print_int(prod_id);
    print_str(" rate="); print_int((int)rate); print_str("/hr\n");
    return ts.n_crew - 1;
}

int ts_add_show(int type, int prod_id, int day, int hour, int capacity) {
    if (ts.n_shows >= MAX_SHOWS || prod_id >= ts.n_productions) return -1;
    show_t* s = &shows[ts.n_shows];
    s->show_id = ts.n_shows;
    s->type = type;
    s->production_id = prod_id;
    s->day = day;
    s->hour = hour;
    s->capacity = capacity;
    s->seats_sold = 0;
    s->revenue = 0.0;
    s->active = 1;
    productions[prod_id].n_shows++;
    ts.n_shows++;
    print_str("[TO] Show "); print_int(ts.n_shows - 1);
    print_str(" type="); print_int(type);
    print_str(" P"); print_int(prod_id);
    print_str(" day="); print_int(day);
    print_str(" hr="); print_int(hour);
    print_str(" cap="); print_int(capacity); print_str("\n");
    return ts.n_shows - 1;
}

int ts_schedule_rehearsal(int prod_id, int day, int hours) {
    if (ts.n_rehearsals >= MAX_REHEARSALS || prod_id >= ts.n_productions) return -1;
    rehearsal_t* r = &rehearsals[ts.n_rehearsals];
    r->rehearsal_id = ts.n_rehearsals;
    r->production_id = prod_id;
    r->day = day;
    r->hours = hours;
    r->n_performers = productions[prod_id].n_performers;
    r->cost = hours * 100.0;
    r->active = 1;
    productions[prod_id].n_rehearsals++;
    productions[prod_id].spent += r->cost;
    ts.production_costs += r->cost;
    ts.n_rehearsals++;
    print_str("[TO] Rehearsal "); print_int(ts.n_rehearsals - 1);
    print_str(" P"); print_int(prod_id);
    print_str(" day="); print_int(day);
    print_str(" hrs="); print_int(hours);
    print_str(" cost="); print_int((int)r->cost); print_str("\n");
    return ts.n_rehearsals - 1;
}

int ts_sell_ticket(int show_id, int seat_class, int qty) {
    if (ts.n_tickets >= MAX_TICKETS || show_id >= ts.n_shows) return -1;
    show_t* s = &shows[show_id];
    double prices[4] = {120.0, 80.0, 50.0, 30.0};
    double price = prices[seat_class < 4 ? seat_class : 0];
    if (s->seats_sold + qty > s->capacity) {
        qty = s->capacity - s->seats_sold;
        if (qty <= 0) return -2;
    }
    ticket_t* t = &tickets[ts.n_tickets];
    t->ticket_id = ts.n_tickets;
    t->show_id = show_id;
    t->seat_class = seat_class;
    t->price = price;
    t->qty = qty;
    t->total = price * qty;
    t->active = 1;
    s->seats_sold += qty;
    s->revenue += t->total;
    productions[s->production_id].revenue += t->total;
    ts.box_office += t->total;
    ts.total_revenue += t->total;
    ts.total_attendance += qty;
    ts.n_tickets++;
    print_str("[TO] Ticket "); print_int(ts.n_tickets - 1);
    print_str(" Sh"); print_int(show_id);
    print_str(" class="); print_int(seat_class);
    print_str(" qty="); print_int(qty);
    print_str(" total="); print_int((int)t->total); print_str("\n");
    return ts.n_tickets - 1;
}

int ts_crew_shift(int crew_id, int hours) {
    if (crew_id >= ts.n_crew) return -1;
    crew_t* c = &crew[crew_id];
    c->n_shifts++;
    c->total_hours += hours;
    double pay = c->hourly_rate * hours;
    c->total_paid += pay;
    ts.crew_costs += pay;
    print_str("[TO] Crew "); print_int(crew_id);
    print_str(" hrs="); print_int(hours);
    print_str(" pay="); print_int((int)pay); print_str("\n");
    return 0;
}

int ts_perform(int show_id) {
    if (show_id >= ts.n_shows) return -1;
    show_t* s = &shows[show_id];
    int prod_id = s->production_id;
    for (int i = 0; i < ts.n_performers; i++) {
        if (performers[i].production_id == prod_id) {
            performers[i].n_performances++;
            performers[i].total_earned += performers[i].fee_per_show;
            ts.performer_costs += performers[i].fee_per_show;
        }
    }
    print_str("[TO] Show "); print_int(show_id);
    print_str(" performed attendance="); print_int(s->seats_sold); print_str("\n");
    return 0;
}

int ts_add_sponsorship(double amount) {
    ts.sponsorship += amount;
    ts.total_revenue += amount;
    print_str("[TO] Sponsorship amount="); print_int((int)amount); print_str("\n");
    return 0;
}

int ts_merch_sale(double amount) {
    ts.merch_revenue += amount;
    ts.total_revenue += amount;
    print_str("[TO] Merch sale amount="); print_int((int)amount); print_str("\n");
    return 0;
}

void ts_performer_report(void) {
    print_str("[TO] Performer report:\n");
    for (int i = 0; i < ts.n_performers; i++) {
        performer_t* p = &performers[i];
        print_str("  Pf"); print_int(i);
        print_str(" role="); print_int(p->role);
        print_str(" P"); print_int(p->production_id);
        print_str(" shows="); print_int(p->n_performances);
        print_str(" earned="); print_int((int)p->total_earned);
        print_str(" rating="); print_int(p->rating); print_str("\n");
    }
}

void ts_show_report(void) {
    print_str("[TO] Show report:\n");
    for (int i = 0; i < ts.n_shows; i++) {
        show_t* s = &shows[i];
        print_str("  Sh"); print_int(i);
        print_str(" type="); print_int(s->type);
        print_str(" P"); print_int(s->production_id);
        print_str(" sold="); print_int(s->seats_sold);
        print_str("/"); print_int(s->capacity);
        print_str(" rev="); print_int((int)s->revenue); print_str("\n");
    }
}

void ts_financial_report(void) {
    print_str("[TO] Financial report:\n");
    print_str("  Box office: "); print_int((int)ts.box_office); print_str("\n");
    print_str("  Sponsorship: "); print_int((int)ts.sponsorship); print_str("\n");
    print_str("  Merch: "); print_int((int)ts.merch_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)ts.total_revenue); print_str("\n");
    print_str("  Performer costs: "); print_int((int)ts.performer_costs); print_str("\n");
    print_str("  Crew costs: "); print_int((int)ts.crew_costs); print_str("\n");
    print_str("  Production costs: "); print_int((int)ts.production_costs); print_str("\n");
    double total_costs = ts.performer_costs + ts.crew_costs + ts.production_costs;
    double profit = ts.total_revenue - total_costs;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    print_str("  Total attendance: "); print_int(ts.total_attendance); print_str("\n");
}

void ts_print_state(void) {
    print_str("[TO] Shows="); print_int(ts.n_shows);
    print_str(" Performers="); print_int(ts.n_performers);
    print_str(" Tickets="); print_int(ts.n_tickets);
    print_str(" Prods="); print_int(ts.n_productions);
    print_str(" Crew="); print_int(ts.n_crew);
    print_str(" Rehearsals="); print_int(ts.n_rehearsals);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)ts.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Theater Opera Demo ===\n\n");
    ts_init();

    print_str("Adding productions...\n");
    ts_add_production(1, 0, 50000.0);
    ts_add_production(2, 1, 80000.0);
    ts_add_production(3, 0, 60000.0);
    ts_add_production(4, 1, 40000.0);
    ts_add_production(1, 0, 55000.0);
    ts_add_production(2, 1, 70000.0);

    print_str("\nAdding performers...\n");
    ts_add_performer(1, 0, 500.0, 95);
    ts_add_performer(2, 0, 300.0, 88);
    ts_add_performer(3, 0, 200.0, 85);
    ts_add_performer(1, 1, 600.0, 92);
    ts_add_performer(2, 1, 350.0, 90);
    ts_add_performer(3, 1, 250.0, 87);
    ts_add_performer(1, 2, 550.0, 94);
    ts_add_performer(2, 2, 320.0, 89);
    ts_add_performer(1, 3, 450.0, 91);
    ts_add_performer(2, 3, 280.0, 86);
    ts_add_performer(3, 3, 180.0, 83);
    ts_add_performer(1, 4, 520.0, 93);
    ts_add_performer(2, 4, 310.0, 88);
    ts_add_performer(1, 5, 580.0, 96);

    print_str("\nAdding crew...\n");
    ts_add_crew(1, 0, 25.0);
    ts_add_crew(1, 0, 22.0);
    ts_add_crew(2, 1, 30.0);
    ts_add_crew(2, 1, 28.0);
    ts_add_crew(3, 2, 20.0);
    ts_add_crew(3, 2, 18.0);
    ts_add_crew(4, 3, 35.0);
    ts_add_crew(4, 3, 32.0);
    ts_add_crew(1, 4, 24.0);
    ts_add_crew(2, 5, 29.0);

    print_str("\nAdding shows...\n");
    ts_add_show(1, 0, 15, 19, 500);
    ts_add_show(1, 0, 16, 19, 500);
    ts_add_show(2, 1, 17, 20, 400);
    ts_add_show(2, 1, 18, 20, 400);
    ts_add_show(3, 2, 19, 19, 450);
    ts_add_show(3, 2, 20, 19, 450);
    ts_add_show(4, 3, 21, 18, 350);
    ts_add_show(4, 3, 22, 18, 350);
    ts_add_show(1, 4, 23, 19, 500);
    ts_add_show(2, 5, 24, 20, 400);

    print_str("\nScheduling rehearsals...\n");
    ts_schedule_rehearsal(0, 10, 4);
    ts_schedule_rehearsal(0, 11, 4);
    ts_schedule_rehearsal(0, 12, 3);
    ts_schedule_rehearsal(1, 10, 5);
    ts_schedule_rehearsal(1, 11, 5);
    ts_schedule_rehearsal(2, 13, 4);
    ts_schedule_rehearsal(2, 14, 4);
    ts_schedule_rehearsal(3, 15, 3);
    ts_schedule_rehearsal(3, 16, 3);
    ts_schedule_rehearsal(4, 17, 4);
    ts_schedule_rehearsal(5, 18, 5);
    ts_schedule_rehearsal(5, 19, 4);

    print_str("\nSelling tickets...\n");
    ts_sell_ticket(0, 0, 5);
    ts_sell_ticket(0, 1, 10);
    ts_sell_ticket(0, 2, 20);
    ts_sell_ticket(1, 0, 8);
    ts_sell_ticket(1, 1, 15);
    ts_sell_ticket(2, 0, 4);
    ts_sell_ticket(2, 1, 12);
    ts_sell_ticket(2, 2, 25);
    ts_sell_ticket(3, 0, 6);
    ts_sell_ticket(3, 1, 10);
    ts_sell_ticket(4, 1, 8);
    ts_sell_ticket(4, 2, 30);
    ts_sell_ticket(5, 0, 5);
    ts_sell_ticket(5, 2, 20);
    ts_sell_ticket(6, 1, 15);
    ts_sell_ticket(6, 2, 25);
    ts_sell_ticket(7, 0, 3);
    ts_sell_ticket(7, 1, 12);
    ts_sell_ticket(8, 2, 35);
    ts_sell_ticket(9, 1, 10);

    print_str("\nPerforming shows...\n");
    for (int i = 0; i < 10; i++) {
        ts_perform(i);
    }

    print_str("\nCrew shifts...\n");
    for (int i = 0; i < 10; i++) {
        ts_crew_shift(i, 6);
    }

    print_str("\nSponsorship...\n");
    ts_add_sponsorship(10000.0);
    ts_add_sponsorship(15000.0);
    ts_add_sponsorship(8000.0);

    print_str("\nMerch sales...\n");
    ts_merch_sale(2500.0);
    ts_merch_sale(3200.0);
    ts_merch_sale(1800.0);
    ts_merch_sale(4100.0);

    print_str("\nPerformer report...\n");
    ts_performer_report();

    print_str("\nShow report...\n");
    ts_show_report();

    print_str("\nFinancial report...\n");
    ts_financial_report();

    print_str("\nFinal state...\n");
    ts_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
