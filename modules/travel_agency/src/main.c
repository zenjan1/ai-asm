/* travel_agency: Travel agency management system (v1.0)
 * Route management, guide management, bookings, team operations, finance
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

#define MAX_ROUTES    10
#define MAX_GUIDES    8
#define MAX_BOOKINGS  16
#define MAX_TEAMS     8
#define MAX_FEEDBACK  10

typedef struct {
    int    route_id;
    int    type;
    int    duration_days;
    int    max_pax;
    double base_cost;
    double price;
    int    stops;
    int    active;
} route_t;

typedef struct {
    int    guide_id;
    int    language;
    int    rating;
    int    tours_led;
    double total_revenue;
    int    availability;
    int    active;
} guide_t;

typedef struct {
    int    booking_id;
    int    route_id;
    int    pax_count;
    double total_amount;
    double paid;
    int    status;
    int    start_day;
    int    active;
} booking_t;

typedef struct {
    int    team_id;
    int    route_id;
    int    guide_id;
    int    start_day;
    int    end_day;
    int    pax_count;
    int    status;
    int    active;
} team_t;

typedef struct {
    int    fb_id;
    int    booking_id;
    int    rating;
    int    satisfaction;
    int    active;
} feedback_t;

typedef struct {
    int    n_routes;
    int    n_guides;
    int    n_bookings;
    int    n_teams;
    int    n_feedback;
    double total_revenue;
    double total_costs;
    double profit;
    double commission_paid;
} ta_state_t;

static route_t routes[MAX_ROUTES];
static guide_t guides[MAX_GUIDES];
static booking_t bookings[MAX_BOOKINGS];
static team_t teams[MAX_TEAMS];
static feedback_t feedbacks[MAX_FEEDBACK];
static ta_state_t ta;

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
static double dabs(double x) { return x < 0 ? -x : x; }

int ta_init(void) {
    if (initialized) return -1;
    ta.n_routes = 0; ta.n_guides = 0; ta.n_bookings = 0;
    ta.n_teams = 0; ta.n_feedback = 0;
    ta.total_revenue = 0.0; ta.total_costs = 0.0;
    ta.profit = 0.0; ta.commission_paid = 0.0;
    for (int i = 0; i < MAX_ROUTES; i++) routes[i].active = 0;
    for (int i = 0; i < MAX_GUIDES; i++) guides[i].active = 0;
    for (int i = 0; i < MAX_BOOKINGS; i++) bookings[i].active = 0;
    for (int i = 0; i < MAX_TEAMS; i++) teams[i].active = 0;
    for (int i = 0; i < MAX_FEEDBACK; i++) feedbacks[i].active = 0;
    initialized = 1;
    print_str("[TA] Travel agency initialized\n");
    return 0;
}

int ta_create_route(int type, int days, int max_pax, double cost, double price, int stops) {
    if (ta.n_routes >= MAX_ROUTES) return -1;
    route_t* r = &routes[ta.n_routes];
    r->route_id = ta.n_routes;
    r->type = type;
    r->duration_days = days;
    r->max_pax = max_pax;
    r->base_cost = cost;
    r->price = price;
    r->stops = stops;
    r->active = 1;
    ta.n_routes++;
    print_str("[TA] Route "); print_int(ta.n_routes - 1);
    print_str(" type="); print_int(type);
    print_str(" days="); print_int(days);
    print_str(" max="); print_int(max_pax); print_str("pax");
    print_str(" cost="); print_int((int)cost);
    print_str(" price="); print_int((int)price);
    print_str(" stops="); print_int(stops); print_str("\n");
    return ta.n_routes - 1;
}

int ta_add_guide(int language, int rating) {
    if (ta.n_guides >= MAX_GUIDES) return -1;
    guide_t* g = &guides[ta.n_guides];
    g->guide_id = ta.n_guides;
    g->language = language;
    g->rating = rating;
    g->tours_led = 0;
    g->total_revenue = 0.0;
    g->availability = 1;
    g->active = 1;
    ta.n_guides++;
    print_str("[TA] Guide "); print_int(ta.n_guides - 1);
    print_str(" lang="); print_int(language);
    print_str(" rating="); print_int(rating); print_str("\n");
    return ta.n_guides - 1;
}

int ta_make_booking(int route_id, int pax, int start_day) {
    if (ta.n_bookings >= MAX_BOOKINGS || route_id >= ta.n_routes) return -1;
    booking_t* b = &bookings[ta.n_bookings];
    b->booking_id = ta.n_bookings;
    b->route_id = route_id;
    b->pax_count = pax;
    b->total_amount = routes[route_id].price * pax;
    b->paid = b->total_amount * 0.3;
    b->status = 1;
    b->start_day = start_day;
    b->active = 1;
    ta.n_bookings++;
    print_str("[TA] Booking "); print_int(ta.n_bookings - 1);
    print_str(" route="); print_int(route_id);
    print_str(" pax="); print_int(pax);
    print_str(" total="); print_int((int)b->total_amount);
    print_str(" deposit="); print_int((int)b->paid);
    print_str(" day="); print_int(start_day); print_str("\n");
    return ta.n_bookings - 1;
}

void ta_form_team(int route_id, int guide_id, int start_day, int duration, int pax) {
    if (ta.n_teams >= MAX_TEAMS) return;
    if (route_id >= ta.n_routes || guide_id >= ta.n_guides) return;
    team_t* t = &teams[ta.n_teams];
    t->team_id = ta.n_teams;
    t->route_id = route_id;
    t->guide_id = guide_id;
    t->start_day = start_day;
    t->end_day = start_day + duration;
    t->pax_count = pax;
    t->status = 1;
    t->active = 1;
    ta.n_teams++;
    guides[guide_id].tours_led++;
    guides[guide_id].availability = 0;
    print_str("[TA] Team "); print_int(ta.n_teams - 1);
    print_str(" route="); print_int(route_id);
    print_str(" guide="); print_int(guide_id);
    print_str(" day "); print_int(start_day); print_str("-"); print_int(t->end_day);
    print_str(" pax="); print_int(pax); print_str("\n");
}

void ta_collect_payment(int booking_id, double amount) {
    if (booking_id >= ta.n_bookings) return;
    booking_t* b = &bookings[booking_id];
    b->paid += amount;
    ta.total_revenue += amount;
    print_str("[TA] Payment collected B"); print_int(booking_id);
    print_str(" amt="); print_int((int)amount);
    print_str(" paid="); print_int((int)b->paid);
    print_str("/"); print_int((int)b->total_amount); print_str("\n");
    if (b->paid >= b->total_amount) {
        b->status = 3;
        print_str("  -> Fully paid\n");
    }
}

void ta_complete_tour(int team_id) {
    if (team_id >= ta.n_teams) return;
    team_t* t = &teams[team_id];
    t->status = 3;
    guides[t->guide_id].availability = 1;
    double revenue = routes[t->route_id].price * t->pax_count;
    double cost = routes[t->route_id].base_cost * t->pax_count;
    double commission = revenue * 0.1;
    ta.total_revenue += revenue;
    ta.total_costs += cost;
    ta.commission_paid += commission;
    ta.profit = ta.total_revenue - ta.total_costs - ta.commission_paid;
    guides[t->guide_id].total_revenue += revenue;
    print_str("[TA] Tour "); print_int(team_id); print_str(" completed\n");
    print_str("  Revenue: "); print_int((int)revenue); print_str("\n");
    print_str("  Cost: "); print_int((int)cost); print_str("\n");
    print_str("  Commission: "); print_int((int)commission); print_str("\n");
}

int ta_add_feedback(int booking_id, int rating, int satisfaction) {
    if (ta.n_feedback >= MAX_FEEDBACK || booking_id >= ta.n_bookings) return -1;
    feedback_t* f = &feedbacks[ta.n_feedback];
    f->fb_id = ta.n_feedback;
    f->booking_id = booking_id;
    f->rating = rating;
    f->satisfaction = satisfaction;
    f->active = 1;
    ta.n_feedback++;
    print_str("[TA] Feedback "); print_int(ta.n_feedback - 1);
    print_str(" booking="); print_int(booking_id);
    print_str(" rating="); print_int(rating);
    print_str(" satisfaction="); print_int(satisfaction); print_str("/10\n");
    return ta.n_feedback - 1;
}

void ta_guide_performance(void) {
    print_str("[TA] Guide performance:\n");
    for (int i = 0; i < ta.n_guides; i++) {
        guide_t* g = &guides[i];
        int avg_rating = g->tours_led > 0 ? (int)(g->total_revenue / g->tours_led) : 0;
        print_str("  Guide "); print_int(i);
        print_str(" tours="); print_int(g->tours_led);
        print_str(" rating="); print_int(g->rating);
        print_str(" revenue="); print_int((int)g->total_revenue);
        print_str(" avail="); print_int(g->availability); print_str("\n");
    }
}

void ta_financial_summary(void) {
    print_str("[TA] Financial summary:\n");
    print_str("  Total revenue: "); print_int((int)ta.total_revenue); print_str("\n");
    print_str("  Total costs: "); print_int((int)ta.total_costs); print_str("\n");
    print_str("  Commission paid: "); print_int((int)ta.commission_paid); print_str("\n");
    print_str("  Net profit: "); print_int((int)ta.profit); print_str("\n");
    double margin = (ta.total_revenue > 0) ? ta.profit / ta.total_revenue * 100.0 : 0.0;
    print_str("  Profit margin: "); print_int((int)margin); print_str("%\n");
}

void ta_print_state(void) {
    print_str("[TA] Routes="); print_int(ta.n_routes);
    print_str(" Guides="); print_int(ta.n_guides);
    print_str(" Bookings="); print_int(ta.n_bookings);
    print_str(" Teams="); print_int(ta.n_teams);
    print_str(" Feedback="); print_int(ta.n_feedback);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)ta.total_revenue);
    print_str(" Profit: "); print_int((int)ta.profit); print_str("\n");
}

int main(void) {
    print_str("=== Travel Agency Management Demo ===\n\n");
    ta_init();

    print_str("Creating routes...\n");
    ta_create_route(1, 5, 20, 500.0, 800.0, 4);
    ta_create_route(2, 7, 15, 700.0, 1100.0, 6);
    ta_create_route(1, 3, 30, 300.0, 500.0, 3);
    ta_create_route(3, 10, 10, 1200.0, 2000.0, 8);
    ta_create_route(2, 4, 25, 400.0, 650.0, 5);

    print_str("\nAdding guides...\n");
    ta_add_guide(1, 90);
    ta_add_guide(2, 85);
    ta_add_guide(1, 80);
    ta_add_guide(3, 95);
    ta_add_guide(2, 88);

    print_str("\nMaking bookings...\n");
    ta_make_booking(0, 10, 15);
    ta_make_booking(1, 8, 18);
    ta_make_booking(2, 15, 20);
    ta_make_booking(3, 5, 22);
    ta_make_booking(4, 12, 25);
    ta_make_booking(0, 8, 28);

    print_str("\nForming teams...\n");
    ta_form_team(0, 0, 15, 5, 10);
    ta_form_team(1, 1, 18, 7, 8);
    ta_form_team(2, 2, 20, 3, 15);
    ta_form_team(3, 3, 22, 10, 5);

    print_str("\nCollecting payments...\n");
    ta_collect_payment(0, 5000.0);
    ta_collect_payment(0, 3000.0);
    ta_collect_payment(1, 4000.0);
    ta_collect_payment(2, 5000.0);
    ta_collect_payment(2, 2500.0);
    ta_collect_payment(3, 8000.0);
    ta_collect_payment(3, 2000.0);
    ta_collect_payment(4, 5000.0);

    print_str("\nCompleting tours...\n");
    ta_complete_tour(0);
    ta_complete_tour(1);
    ta_complete_tour(2);

    print_str("\nReassigning freed guides...\n");
    ta_form_team(4, 0, 28, 4, 12);

    print_str("\nCollecting feedback...\n");
    ta_add_feedback(0, 9, 8);
    ta_add_feedback(1, 8, 7);
    ta_add_feedback(2, 7, 9);
    ta_add_feedback(3, 10, 10);
    ta_add_feedback(4, 6, 6);

    print_str("\nGuide performance review...\n");
    ta_guide_performance();

    print_str("\nFinancial summary...\n");
    ta_financial_summary();

    print_str("\nFinal state...\n");
    ta_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
