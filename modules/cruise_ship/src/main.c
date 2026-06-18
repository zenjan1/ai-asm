/* cruise_ship: Cruise ship management (v1.0)
 * Cabins, itineraries, passengers, crew, facilities, revenue
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

#define MAX_CABINS     14
#define MAX_ITINERARIES 8
#define MAX_PASSENGERS 16
#define MAX_CREW       12
#define MAX_FACILITIES 10

typedef struct {
    int    cabin_id;
    int    type;
    int    deck;
    double nightly_rate;
    int    occupied;
    int    n_bookings;
    double revenue;
    int    active;
} cabin_t;

typedef struct {
    int    itin_id;
    int    n_ports;
    int    duration_days;
    double base_price;
    int    n_bookings;
    double revenue;
    int    active;
} itinerary_t;

typedef struct {
    int    pass_id;
    int    cabin_id;
    int    itin_id;
    int    n_guests;
    double onboard_spent;
    double excursion_spent;
    double tips;
    int    active;
} passenger_t;

typedef struct {
    int    crew_id;
    int    dept;
    int    rank;
    double monthly_salary;
    int    n_months;
    double total_paid;
    double rating;
    int    active;
} crew_t;

typedef struct {
    int    fac_id;
    int    type;
    int    capacity;
    double daily_revenue;
    int    n_visits;
    double total_revenue;
    int    active;
} facility_t;

typedef struct {
    int    n_cabins;
    int    n_itineraries;
    int    n_passengers;
    int    n_crew;
    int    n_facilities;
    double cabin_revenue;
    double onboard_revenue;
    double excursion_revenue;
    double tips_revenue;
    double total_revenue;
    double crew_costs;
    double ops_costs;
    int    total_guests;
} cs_state_t;

static cabin_t cabins[MAX_CABINS];
static itinerary_t itineraries[MAX_ITINERARIES];
static passenger_t passengers[MAX_PASSENGERS];
static crew_t crew[MAX_CREW];
static facility_t facilities[MAX_FACILITIES];
static cs_state_t cs;

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

int cs_init(void) {
    if (initialized) return -1;
    cs.n_cabins = 0; cs.n_itineraries = 0; cs.n_passengers = 0;
    cs.n_crew = 0; cs.n_facilities = 0;
    cs.cabin_revenue = 0.0; cs.onboard_revenue = 0.0;
    cs.excursion_revenue = 0.0; cs.tips_revenue = 0.0;
    cs.total_revenue = 0.0; cs.crew_costs = 0.0;
    cs.ops_costs = 0.0; cs.total_guests = 0;
    for (int i = 0; i < MAX_CABINS; i++) cabins[i].active = 0;
    for (int i = 0; i < MAX_ITINERARIES; i++) itineraries[i].active = 0;
    for (int i = 0; i < MAX_PASSENGERS; i++) passengers[i].active = 0;
    for (int i = 0; i < MAX_CREW; i++) crew[i].active = 0;
    for (int i = 0; i < MAX_FACILITIES; i++) facilities[i].active = 0;
    initialized = 1;
    print_str("[CS] Cruise ship initialized\n");
    return 0;
}

int cs_add_cabin(int type, int deck, double rate) {
    if (cs.n_cabins >= MAX_CABINS) return -1;
    cabin_t* c = &cabins[cs.n_cabins];
    c->cabin_id = cs.n_cabins;
    c->type = type;
    c->deck = deck;
    c->nightly_rate = rate;
    c->occupied = 0;
    c->n_bookings = 0;
    c->revenue = 0.0;
    c->active = 1;
    cs.n_cabins++;
    print_str("[CS] Cabin "); print_int(cs.n_cabins - 1);
    print_str(" type="); print_int(type);
    print_str(" D"); print_int(deck);
    print_str(" $"); print_int((int)rate); print_str("/nt\n");
    return cs.n_cabins - 1;
}

int cs_add_itinerary(int n_ports, int days, double price) {
    if (cs.n_itineraries >= MAX_ITINERARIES) return -1;
    itinerary_t* it = &itineraries[cs.n_itineraries];
    it->itin_id = cs.n_itineraries;
    it->n_ports = n_ports;
    it->duration_days = days;
    it->base_price = price;
    it->n_bookings = 0;
    it->revenue = 0.0;
    it->active = 1;
    cs.n_itineraries++;
    print_str("[CS] Itin "); print_int(cs.n_itineraries - 1);
    print_str(" ports="); print_int(n_ports);
    print_str(" days="); print_int(days);
    print_str(" $"); print_int((int)price); print_str("\n");
    return cs.n_itineraries - 1;
}

int cs_add_crew(int dept, int rank, double salary) {
    if (cs.n_crew >= MAX_CREW) return -1;
    crew_t* cr = &crew[cs.n_crew];
    cr->crew_id = cs.n_crew;
    cr->dept = dept;
    cr->rank = rank;
    cr->monthly_salary = salary;
    cr->n_months = 0;
    cr->total_paid = 0.0;
    cr->rating = 4.0;
    cr->active = 1;
    cs.n_crew++;
    print_str("[CS] Crew "); print_int(cs.n_crew - 1);
    print_str(" dept="); print_int(dept);
    print_str(" rank="); print_int(rank);
    print_str(" $"); print_int((int)salary); print_str("/mo\n");
    return cs.n_crew - 1;
}

int cs_add_facility(int type, int capacity, double daily_rev) {
    if (cs.n_facilities >= MAX_FACILITIES) return -1;
    facility_t* f = &facilities[cs.n_facilities];
    f->fac_id = cs.n_facilities;
    f->type = type;
    f->capacity = capacity;
    f->daily_revenue = daily_rev;
    f->n_visits = 0;
    f->total_revenue = 0.0;
    f->active = 1;
    cs.n_facilities++;
    print_str("[CS] Facility "); print_int(cs.n_facilities - 1);
    print_str(" type="); print_int(type);
    print_str(" cap="); print_int(capacity);
    print_str(" $"); print_int((int)daily_rev); print_str("/day\n");
    return cs.n_facilities - 1;
}

int cs_book_passenger(int cabin_id, int itin_id, int n_guests) {
    if (cs.n_passengers >= MAX_PASSENGERS) return -1;
    if (cabin_id >= cs.n_cabins || itin_id >= cs.n_itineraries) return -2;
    passenger_t* p = &passengers[cs.n_passengers];
    p->pass_id = cs.n_passengers;
    p->cabin_id = cabin_id;
    p->itin_id = itin_id;
    p->n_guests = n_guests;
    p->onboard_spent = 0.0;
    p->excursion_spent = 0.0;
    p->tips = 0.0;
    p->active = 1;
    cabin_t* c = &cabins[cabin_id];
    itinerary_t* it = &itineraries[itin_id];
    double cabin_rev = c->nightly_rate * it->duration_days;
    c->occupied = 1;
    c->n_bookings++;
    c->revenue += cabin_rev;
    it->n_bookings++;
    it->revenue += it->base_price * n_guests;
    cs.cabin_revenue += cabin_rev + it->base_price * n_guests;
    cs.total_revenue += cabin_rev + it->base_price * n_guests;
    cs.total_guests += n_guests;
    cs.n_passengers++;
    print_str("[CS] Pass "); print_int(cs.n_passengers - 1);
    print_str(" Cb"); print_int(cabin_id);
    print_str(" It"); print_int(itin_id);
    print_str(" guests="); print_int(n_guests);
    print_str(" $"); print_int((int)(cabin_rev + it->base_price * n_guests)); print_str("\n");
    return cs.n_passengers - 1;
}

int cs_onboard_spending(int pass_id, double amount) {
    if (pass_id >= cs.n_passengers) return -1;
    passengers[pass_id].onboard_spent += amount;
    cs.onboard_revenue += amount;
    cs.total_revenue += amount;
    print_str("[CS] Pass"); print_int(pass_id);
    print_str(" onboard $"); print_int((int)amount); print_str("\n");
    return 0;
}

int cs_excursion_spending(int pass_id, double amount) {
    if (pass_id >= cs.n_passengers) return -1;
    passengers[pass_id].excursion_spent += amount;
    cs.excursion_revenue += amount;
    cs.total_revenue += amount;
    print_str("[CS] Pass"); print_int(pass_id);
    print_str(" excursion $"); print_int((int)amount); print_str("\n");
    return 0;
}

int cs_add_tips(int pass_id, double amount) {
    if (pass_id >= cs.n_passengers) return -1;
    passengers[pass_id].tips += amount;
    cs.tips_revenue += amount;
    cs.total_revenue += amount;
    print_str("[CS] Pass"); print_int(pass_id);
    print_str(" tips $"); print_int((int)amount); print_str("\n");
    return 0;
}

int cs_facility_visit(int fac_id) {
    if (fac_id >= cs.n_facilities) return -1;
    facility_t* f = &facilities[fac_id];
    f->n_visits++;
    f->total_revenue += f->daily_revenue;
    cs.total_revenue += f->daily_revenue;
    cs.onboard_revenue += f->daily_revenue;
    return 0;
}

int cs_pay_crew(int crew_id, int months) {
    if (crew_id >= cs.n_crew) return -1;
    crew_t* cr = &crew[crew_id];
    cr->n_months += months;
    double pay = cr->monthly_salary * months;
    cr->total_paid += pay;
    cs.crew_costs += pay;
    print_str("[CS] Cr"); print_int(crew_id);
    print_str(" paid $"); print_int((int)pay); print_str("\n");
    return 0;
}

int cs_add_ops_cost(double amount) {
    cs.ops_costs += amount;
    print_str("[CS] Ops cost $"); print_int((int)amount); print_str("\n");
    return 0;
}

void cs_cabin_report(void) {
    print_str("[CS] Cabin report:\n");
    for (int i = 0; i < cs.n_cabins; i++) {
        cabin_t* c = &cabins[i];
        print_str("  Cb"); print_int(i);
        print_str(" type="); print_int(c->type);
        print_str(" D"); print_int(c->deck);
        print_str(" bookings="); print_int(c->n_bookings);
        print_str(" rev="); print_int((int)c->revenue);
        print_str(" occ="); print_int(c->occupied); print_str("\n");
    }
}

void cs_financial_report(void) {
    print_str("[CS] Financial report:\n");
    print_str("  Cabin revenue: "); print_int((int)cs.cabin_revenue); print_str("\n");
    print_str("  Onboard revenue: "); print_int((int)cs.onboard_revenue); print_str("\n");
    print_str("  Excursion revenue: "); print_int((int)cs.excursion_revenue); print_str("\n");
    print_str("  Tips revenue: "); print_int((int)cs.tips_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)cs.total_revenue); print_str("\n");
    print_str("  Crew costs: "); print_int((int)cs.crew_costs); print_str("\n");
    print_str("  Ops costs: "); print_int((int)cs.ops_costs); print_str("\n");
    double total_costs = cs.crew_costs + cs.ops_costs;
    double profit = cs.total_revenue - total_costs;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    print_str("  Total guests: "); print_int(cs.total_guests); print_str("\n");
}

void cs_print_state(void) {
    print_str("[CS] Cabins="); print_int(cs.n_cabins);
    print_str(" Itins="); print_int(cs.n_itineraries);
    print_str(" Pass="); print_int(cs.n_passengers);
    print_str(" Crew="); print_int(cs.n_crew);
    print_str(" Fac="); print_int(cs.n_facilities);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)cs.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Cruise Ship Demo ===\n\n");
    cs_init();

    print_str("Adding cabins...\n");
    cs_add_cabin(1, 1, 120.0);
    cs_add_cabin(1, 2, 130.0);
    cs_add_cabin(2, 3, 200.0);
    cs_add_cabin(2, 4, 220.0);
    cs_add_cabin(3, 5, 350.0);
    cs_add_cabin(3, 6, 380.0);
    cs_add_cabin(4, 7, 600.0);
    cs_add_cabin(4, 8, 650.0);
    cs_add_cabin(1, 1, 125.0);
    cs_add_cabin(2, 3, 210.0);
    cs_add_cabin(3, 5, 360.0);
    cs_add_cabin(4, 7, 620.0);
    cs_add_cabin(1, 2, 135.0);
    cs_add_cabin(2, 4, 230.0);

    print_str("\nAdding itineraries...\n");
    cs_add_itinerary(3, 4, 800.0);
    cs_add_itinerary(4, 7, 1500.0);
    cs_add_itinerary(5, 10, 2200.0);
    cs_add_itinerary(6, 14, 3500.0);
    cs_add_itinerary(3, 5, 1000.0);
    cs_add_itinerary(4, 7, 1600.0);
    cs_add_itinerary(5, 10, 2400.0);
    cs_add_itinerary(7, 21, 6000.0);

    print_str("\nAdding crew...\n");
    cs_add_crew(1, 5, 8000.0);
    cs_add_crew(1, 4, 5000.0);
    cs_add_crew(2, 3, 3500.0);
    cs_add_crew(2, 3, 3200.0);
    cs_add_crew(3, 4, 4500.0);
    cs_add_crew(3, 3, 3000.0);
    cs_add_crew(4, 3, 2800.0);
    cs_add_crew(4, 2, 2200.0);
    cs_add_crew(5, 3, 3500.0);
    cs_add_crew(5, 2, 2500.0);
    cs_add_crew(1, 4, 5500.0);
    cs_add_crew(2, 3, 3300.0);

    print_str("\nAdding facilities...\n");
    cs_add_facility(1, 300, 5000.0);
    cs_add_facility(2, 200, 3500.0);
    cs_add_facility(3, 150, 2500.0);
    cs_add_facility(4, 100, 2000.0);
    cs_add_facility(5, 80, 1800.0);
    cs_add_facility(6, 50, 1500.0);
    cs_add_facility(1, 250, 4500.0);
    cs_add_facility(2, 180, 3000.0);
    cs_add_facility(7, 60, 1200.0);
    cs_add_facility(8, 40, 1000.0);

    print_str("\nBooking passengers...\n");
    for (int i = 0; i < 16; i++) {
        cs_book_passenger(i % 14, i % 8, 1 + (i % 3));
    }

    print_str("\nOnboard spending...\n");
    for (int i = 0; i < 16; i++) {
        cs_onboard_spending(i, 200.0 + i * 50.0);
    }

    print_str("\nExcursion spending...\n");
    for (int i = 0; i < 16; i++) {
        cs_excursion_spending(i, 300.0 + i * 75.0);
    }

    print_str("\nTips...\n");
    for (int i = 0; i < 16; i++) {
        cs_add_tips(i, 150.0 + i * 25.0);
    }

    print_str("\nFacility visits...\n");
    for (int i = 0; i < 30; i++) {
        cs_facility_visit(i % 10);
    }

    print_str("\nPaying crew...\n");
    for (int i = 0; i < 12; i++) {
        cs_pay_crew(i, 6);
    }

    print_str("\nOps costs...\n");
    cs_add_ops_cost(500000.0);
    cs_add_ops_cost(350000.0);
    cs_add_ops_cost(250000.0);
    cs_add_ops_cost(180000.0);

    print_str("\nCabin report...\n");
    cs_cabin_report();

    print_str("\nFinancial report...\n");
    cs_financial_report();

    print_str("\nFinal state...\n");
    cs_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
