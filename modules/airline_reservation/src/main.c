/* airline_reservation: Airline booking management (v1.0)
 * Routes, flights, passengers, seats, check-in, analytics
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

#define MAX_ROUTES     10
#define MAX_FLIGHTS    14
#define MAX_PASSENGERS 16
#define MAX_BOOKINGS   18
#define MAX_CREW       8

typedef struct {
    int    route_id;
    int    from_city;
    int    to_city;
    int    distance_km;
    int    duration_hr;
    int    active;
} route_t;

typedef struct {
    int    flight_id;
    int    route_id;
    int    day;
    int    departure_hr;
    int    capacity;
    int    seats_sold;
    double base_price;
    int    status;
    int    active;
} flight_t;

typedef struct {
    int    passenger_id;
    int    tier;
    int    n_flights;
    int    miles;
    double total_paid;
    int    active;
} passenger_t;

typedef struct {
    int    booking_id;
    int    passenger_id;
    int    flight_id;
    int    seat_class;
    double price;
    int    checked_in;
    int    status;
    int    active;
} booking_t;

typedef struct {
    int    crew_id;
    int    role;
    int    flight_id;
    int    rating;
    int    active;
} crew_t;

typedef struct {
    int    n_routes;
    int    n_flights;
    int    n_passengers;
    int    n_bookings;
    int    n_crew;
    double ticket_revenue;
    double baggage_revenue;
    double total_revenue;
    int    total_passengers;
    int    total_flights;
} ar_state_t;

static route_t routes[MAX_ROUTES];
static flight_t flights[MAX_FLIGHTS];
static passenger_t passengers[MAX_PASSENGERS];
static booking_t bookings[MAX_BOOKINGS];
static crew_t crew[MAX_CREW];
static ar_state_t ar;

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

int ar_init(void) {
    if (initialized) return -1;
    ar.n_routes = 0; ar.n_flights = 0; ar.n_passengers = 0;
    ar.n_bookings = 0; ar.n_crew = 0;
    ar.ticket_revenue = 0.0; ar.baggage_revenue = 0.0;
    ar.total_revenue = 0.0; ar.total_passengers = 0;
    ar.total_flights = 0;
    for (int i = 0; i < MAX_ROUTES; i++) routes[i].active = 0;
    for (int i = 0; i < MAX_FLIGHTS; i++) flights[i].active = 0;
    for (int i = 0; i < MAX_PASSENGERS; i++) passengers[i].active = 0;
    for (int i = 0; i < MAX_BOOKINGS; i++) bookings[i].active = 0;
    for (int i = 0; i < MAX_CREW; i++) crew[i].active = 0;
    initialized = 1;
    print_str("[AR] Airline reservation initialized\n");
    return 0;
}

int ar_add_route(int from, int to, int distance, int duration) {
    if (ar.n_routes >= MAX_ROUTES) return -1;
    route_t* r = &routes[ar.n_routes];
    r->route_id = ar.n_routes;
    r->from_city = from;
    r->to_city = to;
    r->distance_km = distance;
    r->duration_hr = duration;
    r->active = 1;
    ar.n_routes++;
    print_str("[AR] Route "); print_int(ar.n_routes - 1);
    print_str(" C"); print_int(from); print_str("->C"); print_int(to);
    print_str(" "); print_int(distance); print_str("km");
    print_str(" "); print_int(duration); print_str("hr\n");
    return ar.n_routes - 1;
}

int ar_add_flight(int route_id, int day, int departure_hr, int capacity, double base_price) {
    if (ar.n_flights >= MAX_FLIGHTS || route_id >= ar.n_routes) return -1;
    flight_t* f = &flights[ar.n_flights];
    f->flight_id = ar.n_flights;
    f->route_id = route_id;
    f->day = day;
    f->departure_hr = departure_hr;
    f->capacity = capacity;
    f->seats_sold = 0;
    f->base_price = base_price;
    f->status = 1;
    f->active = 1;
    ar.n_flights++;
    ar.total_flights++;
    print_str("[AR] Flight "); print_int(ar.n_flights - 1);
    print_str(" R"); print_int(route_id);
    print_str(" day="); print_int(day);
    print_str(" hr="); print_int(departure_hr);
    print_str(" cap="); print_int(capacity);
    print_str(" price="); print_int((int)base_price); print_str("\n");
    return ar.n_flights - 1;
}

int ar_register_passenger(int tier) {
    if (ar.n_passengers >= MAX_PASSENGERS) return -1;
    passenger_t* p = &passengers[ar.n_passengers];
    p->passenger_id = ar.n_passengers;
    p->tier = tier;
    p->n_flights = 0;
    p->miles = 0;
    p->total_paid = 0.0;
    p->active = 1;
    ar.n_passengers++;
    print_str("[AR] Passenger "); print_int(ar.n_passengers - 1);
    print_str(" tier="); print_int(tier); print_str("\n");
    return ar.n_passengers - 1;
}

int ar_add_crew(int role, int flight_id, int rating) {
    if (ar.n_crew >= MAX_CREW || flight_id >= ar.n_flights) return -1;
    crew_t* c = &crew[ar.n_crew];
    c->crew_id = ar.n_crew;
    c->role = role;
    c->flight_id = flight_id;
    c->rating = rating;
    c->active = 1;
    ar.n_crew++;
    print_str("[AR] Crew "); print_int(ar.n_crew - 1);
    print_str(" role="); print_int(role);
    print_str(" F"); print_int(flight_id);
    print_str(" rating="); print_int(rating); print_str("\n");
    return ar.n_crew - 1;
}

int ar_book_flight(int passenger_id, int flight_id, int seat_class) {
    if (ar.n_bookings >= MAX_BOOKINGS) return -1;
    if (passenger_id >= ar.n_passengers || flight_id >= ar.n_flights) return -2;
    flight_t* f = &flights[flight_id];
    if (f->seats_sold >= f->capacity) return -3;
    booking_t* b = &bookings[ar.n_bookings];
    b->booking_id = ar.n_bookings;
    b->passenger_id = passenger_id;
    b->flight_id = flight_id;
    b->seat_class = seat_class;
    double multiplier[3] = {1.0, 1.8, 3.0};
    b->price = f->base_price * multiplier[seat_class < 3 ? seat_class : 0];
    b->checked_in = 0;
    b->status = 1;
    b->active = 1;
    f->seats_sold++;
    passengers[passenger_id].n_flights++;
    passengers[passenger_id].total_paid += b->price;
    passengers[passenger_id].miles += routes[f->route_id].distance_km;
    ar.ticket_revenue += b->price;
    ar.total_revenue += b->price;
    ar.total_passengers++;
    ar.n_bookings++;
    print_str("[AR] Booking "); print_int(ar.n_bookings - 1);
    print_str(" P"); print_int(passenger_id);
    print_str(" F"); print_int(flight_id);
    print_str(" class="); print_int(seat_class);
    print_str(" price="); print_int((int)b->price); print_str("\n");
    return ar.n_bookings - 1;
}

int ar_checkin(int booking_id) {
    if (booking_id >= ar.n_bookings) return -1;
    booking_t* b = &bookings[booking_id];
    if (b->status != 1) return -2;
    b->checked_in = 1;
    b->status = 2;
    print_str("[AR] Checkin B"); print_int(booking_id);
    print_str(" P"); print_int(b->passenger_id);
    print_str(" F"); print_int(b->flight_id); print_str("\n");
    return 0;
}

int ar_add_baggage(int booking_id, int n_bags) {
    if (booking_id >= ar.n_bookings) return -1;
    double fee = 30.0 * n_bags;
    if (passengers[bookings[booking_id].passenger_id].tier >= 2) fee = 0.0;
    ar.baggage_revenue += fee;
    ar.total_revenue += fee;
    print_str("[AR] Baggage B"); print_int(booking_id);
    print_str(" bags="); print_int(n_bags);
    print_str(" fee="); print_int((int)fee); print_str("\n");
    return 0;
}

void ar_flight_status(void) {
    print_str("[AR] Flight status:\n");
    for (int i = 0; i < ar.n_flights; i++) {
        flight_t* f = &flights[i];
        print_str("  F"); print_int(i);
        print_str(" R"); print_int(f->route_id);
        print_str(" day="); print_int(f->day);
        print_str(" hr="); print_int(f->departure_hr);
        print_str(" sold="); print_int(f->seats_sold);
        print_str("/"); print_int(f->capacity);
        print_str(" status="); print_int(f->status); print_str("\n");
    }
}

void ar_route_report(void) {
    print_str("[AR] Route report:\n");
    for (int i = 0; i < ar.n_routes; i++) {
        route_t* r = &routes[i];
        int total_sold = 0;
        for (int j = 0; j < ar.n_flights; j++) {
            if (flights[j].route_id == i) total_sold += flights[j].seats_sold;
        }
        print_str("  R"); print_int(i);
        print_str(" C"); print_int(r->from_city); print_str("->C"); print_int(r->to_city);
        print_str(" "); print_int(r->distance_km); print_str("km");
        print_str(" pax="); print_int(total_sold); print_str("\n");
    }
}

void ar_revenue_report(void) {
    print_str("[AR] Revenue report:\n");
    print_str("  Ticket revenue: "); print_int((int)ar.ticket_revenue); print_str("\n");
    print_str("  Baggage revenue: "); print_int((int)ar.baggage_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)ar.total_revenue); print_str("\n");
    print_str("  Total passengers: "); print_int(ar.total_passengers); print_str("\n");
    print_str("  Total flights: "); print_int(ar.total_flights); print_str("\n");
}

void ar_print_state(void) {
    print_str("[AR] Routes="); print_int(ar.n_routes);
    print_str(" Flights="); print_int(ar.n_flights);
    print_str(" Passengers="); print_int(ar.n_passengers);
    print_str(" Bookings="); print_int(ar.n_bookings);
    print_str(" Crew="); print_int(ar.n_crew);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)ar.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Airline Reservation Demo ===\n\n");
    ar_init();

    print_str("Adding routes...\n");
    ar_add_route(1, 2, 1200, 2);
    ar_add_route(1, 3, 2500, 4);
    ar_add_route(2, 4, 800, 1);
    ar_add_route(3, 5, 3200, 5);
    ar_add_route(1, 5, 4000, 6);
    ar_add_route(2, 3, 1800, 3);
    ar_add_route(4, 5, 2000, 3);
    ar_add_route(1, 4, 1500, 2);
    ar_add_route(3, 4, 1000, 2);
    ar_add_route(2, 5, 3500, 5);

    print_str("\nAdding flights...\n");
    ar_add_flight(0, 10, 8, 120, 199.0);
    ar_add_flight(1, 10, 10, 180, 349.0);
    ar_add_flight(2, 10, 14, 90, 149.0);
    ar_add_flight(3, 11, 9, 200, 449.0);
    ar_add_flight(4, 11, 12, 150, 549.0);
    ar_add_flight(5, 11, 16, 160, 279.0);
    ar_add_flight(6, 12, 8, 100, 299.0);
    ar_add_flight(7, 12, 11, 140, 229.0);
    ar_add_flight(8, 13, 9, 110, 179.0);
    ar_add_flight(0, 13, 14, 120, 199.0);
    ar_add_flight(1, 14, 10, 180, 349.0);
    ar_add_flight(3, 14, 15, 200, 449.0);
    ar_add_flight(5, 15, 8, 160, 279.0);
    ar_add_flight(9, 15, 13, 170, 499.0);

    print_str("\nRegistering passengers...\n");
    for (int i = 0; i < 14; i++) {
        ar_register_passenger(i % 3);
    }

    print_str("\nAdding crew...\n");
    ar_add_crew(1, 0, 92);
    ar_add_crew(2, 0, 88);
    ar_add_crew(1, 1, 90);
    ar_add_crew(2, 2, 85);
    ar_add_crew(1, 3, 95);
    ar_add_crew(2, 4, 87);
    ar_add_crew(1, 5, 91);
    ar_add_crew(2, 6, 89);

    print_str("\nBooking flights...\n");
    ar_book_flight(0, 0, 0);
    ar_book_flight(1, 0, 1);
    ar_book_flight(2, 1, 0);
    ar_book_flight(3, 1, 2);
    ar_book_flight(4, 2, 0);
    ar_book_flight(5, 3, 1);
    ar_book_flight(6, 3, 0);
    ar_book_flight(7, 4, 2);
    ar_book_flight(8, 5, 0);
    ar_book_flight(9, 6, 1);
    ar_book_flight(10, 7, 0);
    ar_book_flight(11, 8, 0);
    ar_book_flight(12, 9, 1);
    ar_book_flight(13, 10, 0);
    ar_book_flight(0, 11, 2);
    ar_book_flight(1, 11, 0);
    ar_book_flight(2, 12, 1);
    ar_book_flight(3, 13, 0);

    print_str("\nChecking in...\n");
    for (int i = 0; i < 14; i++) {
        ar_checkin(i);
    }

    print_str("\nAdding baggage...\n");
    ar_add_baggage(0, 2);
    ar_add_baggage(1, 1);
    ar_add_baggage(2, 0);
    ar_add_baggage(3, 2);
    ar_add_baggage(4, 1);
    ar_add_baggage(5, 2);
    ar_add_baggage(6, 0);
    ar_add_baggage(7, 1);
    ar_add_baggage(8, 2);
    ar_add_baggage(9, 1);
    ar_add_baggage(10, 0);
    ar_add_baggage(11, 2);

    print_str("\nFlight status...\n");
    ar_flight_status();

    print_str("\nRoute report...\n");
    ar_route_report();

    print_str("\nRevenue report...\n");
    ar_revenue_report();

    print_str("\nFinal state...\n");
    ar_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
