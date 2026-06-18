/* bus_company: Bus transportation management (v1.0)
 * Routes, fleet, drivers, tickets, passengers
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

#define MAX_ROUTES     14
#define MAX_BUSES      18
#define MAX_DRIVERS    16
#define MAX_TICKETS    12
#define MAX_PASSENGERS 10

typedef struct {
    int    route_id;
    int    zone;
    int    length_km;
    int    stops;
    int    frequency_min;
    int    operating_hours;
    int    year_started;
    int    active;
} route_t;

typedef struct {
    int    bus_id;
    int    route_id;
    int    type;
    int    capacity;
    int    status;
    int    mileage;
    int    year_built;
    int    active;
} bus_t;

typedef struct {
    int    driver_id;
    int    route_id;
    int    license_class;
    int    years_exp;
    int    shift;
    int    rating;
    int    incidents;
    int    active;
} driver_t;

typedef struct {
    int    ticket_id;
    int    type;
    int    zone;
    int    price;
    int    sold;
    int    revenue;
    int    year;
    int    month;
    int    active;
} ticket_t;

typedef struct {
    int    pax_id;
    int    route_id;
    int    zone;
    int    daily_ridership;
    int    peak_hour;
    int    satisfaction;
    int    year;
    int    active;
} passenger_t;

typedef struct {
    int    n_routes;
    int    n_buses;
    int    n_drivers;
    int    n_tickets;
    int    n_passengers;
    int    total_route_km;
    int    total_capacity;
    int    total_revenue;
    int    total_ridership;
    int    total_incidents;
} bc_state_t;

static route_t routes[MAX_ROUTES];
static bus_t buses[MAX_BUSES];
static driver_t drivers[MAX_DRIVERS];
static ticket_t tickets[MAX_TICKETS];
static passenger_t passengers[MAX_PASSENGERS];
static bc_state_t bc;

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

int bc_init(void) {
    if (initialized) return -1;
    bc.n_routes = 0; bc.n_buses = 0; bc.n_drivers = 0;
    bc.n_tickets = 0; bc.n_passengers = 0;
    bc.total_route_km = 0; bc.total_capacity = 0;
    bc.total_revenue = 0; bc.total_ridership = 0;
    bc.total_incidents = 0;
    for (int i = 0; i < MAX_ROUTES; i++) routes[i].active = 0;
    for (int i = 0; i < MAX_BUSES; i++) buses[i].active = 0;
    for (int i = 0; i < MAX_DRIVERS; i++) drivers[i].active = 0;
    for (int i = 0; i < MAX_TICKETS; i++) tickets[i].active = 0;
    for (int i = 0; i < MAX_PASSENGERS; i++) passengers[i].active = 0;
    initialized = 1;
    print_str("[BC] Bus company initialized\n");
    return 0;
}

int bc_add_route(int zone, int length, int stops, int frequency, int hours, int year) {
    if (bc.n_routes >= MAX_ROUTES) return -1;
    route_t* r = &routes[bc.n_routes];
    r->route_id = bc.n_routes;
    r->zone = zone;
    r->length_km = length;
    r->stops = stops;
    r->frequency_min = frequency;
    r->operating_hours = hours;
    r->year_started = year;
    r->active = 1;
    bc.total_route_km += length;
    bc.n_routes++;
    print_str("[BC] Route "); print_int(bc.n_routes - 1);
    print_str(" zone="); print_int(zone);
    print_str(" len="); print_int(length); print_str("km");
    print_str(" stops="); print_int(stops);
    print_str(" freq="); print_int(frequency); print_str("min\n");
    return bc.n_routes - 1;
}

int bc_add_bus(int route_id, int type, int capacity, int year) {
    if (bc.n_buses >= MAX_BUSES) return -1;
    bus_t* b = &buses[bc.n_buses];
    b->bus_id = bc.n_buses;
    b->route_id = route_id;
    b->type = type;
    b->capacity = capacity;
    b->status = 1;
    b->mileage = 0;
    b->year_built = year;
    b->active = 1;
    bc.total_capacity += capacity;
    bc.n_buses++;
    print_str("[BC] Bus "); print_int(bc.n_buses - 1);
    print_str(" route="); print_int(route_id);
    print_str(" type="); print_int(type);
    print_str(" cap="); print_int(capacity);
    print_str(" "); print_int(year); print_str("\n");
    return bc.n_buses - 1;
}

int bc_add_driver(int route_id, int license, int years, int shift, int rating) {
    if (bc.n_drivers >= MAX_DRIVERS) return -1;
    driver_t* d = &drivers[bc.n_drivers];
    d->driver_id = bc.n_drivers;
    d->route_id = route_id;
    d->license_class = license;
    d->years_exp = years;
    d->shift = shift;
    d->rating = rating;
    d->incidents = 0;
    d->active = 1;
    bc.n_drivers++;
    print_str("[BC] Driver "); print_int(bc.n_drivers - 1);
    print_str(" route="); print_int(route_id);
    print_str(" lic="); print_int(license);
    print_str(" yrs="); print_int(years);
    print_str(" rating="); print_int(rating); print_str("\n");
    return bc.n_drivers - 1;
}

int bc_record_incident(int driver_id) {
    if (driver_id >= bc.n_drivers) return -1;
    drivers[driver_id].incidents++;
    bc.total_incidents++;
    print_str("[BC] Incident D"); print_int(driver_id); print_str("\n");
    return 0;
}

int bc_add_ticket(int type, int zone, int price, int sold, int year, int month) {
    if (bc.n_tickets >= MAX_TICKETS) return -1;
    ticket_t* t = &tickets[bc.n_tickets];
    t->ticket_id = bc.n_tickets;
    t->type = type;
    t->zone = zone;
    t->price = price;
    t->sold = sold;
    t->revenue = price * sold;
    t->year = year;
    t->month = month;
    t->active = 1;
    bc.total_revenue += t->revenue;
    bc.n_tickets++;
    print_str("[BC] Ticket "); print_int(bc.n_tickets - 1);
    print_str(" type="); print_int(type);
    print_str(" zone="); print_int(zone);
    print_str(" price=$"); print_int(price);
    print_str(" sold="); print_int(sold);
    print_str(" rev=$"); print_int(t->revenue); print_str("\n");
    return bc.n_tickets - 1;
}

int bc_add_passenger(int route_id, int zone, int daily, int peak, int satisfaction, int year) {
    if (bc.n_passengers >= MAX_PASSENGERS) return -1;
    passenger_t* p = &passengers[bc.n_passengers];
    p->pax_id = bc.n_passengers;
    p->route_id = route_id;
    p->zone = zone;
    p->daily_ridership = daily;
    p->peak_hour = peak;
    p->satisfaction = satisfaction;
    p->year = year;
    p->active = 1;
    bc.total_ridership += daily;
    bc.n_passengers++;
    print_str("[BC] Pax "); print_int(bc.n_passengers - 1);
    print_str(" route="); print_int(route_id);
    print_str(" zone="); print_int(zone);
    print_str(" daily="); print_int(daily);
    print_str(" sat="); print_int(satisfaction); print_str("%\n");
    return bc.n_passengers - 1;
}

void bc_route_report(void) {
    print_str("[BC] Route report:\n");
    print_str("  Routes: "); print_int(bc.n_routes); print_str("\n");
    print_str("  Total route km: "); print_int(bc.total_route_km); print_str("\n");
    print_str("  Buses: "); print_int(bc.n_buses); print_str("\n");
    print_str("  Total capacity: "); print_int(bc.total_capacity); print_str("\n");
}

void bc_revenue_report(void) {
    print_str("[BC] Revenue report:\n");
    print_str("  Total revenue: $"); print_int(bc.total_revenue); print_str("\n");
    print_str("  Daily ridership: "); print_int(bc.total_ridership); print_str("\n");
}

void bc_safety_report(void) {
    print_str("[BC] Safety report:\n");
    print_str("  Drivers: "); print_int(bc.n_drivers); print_str("\n");
    print_str("  Incidents: "); print_int(bc.total_incidents); print_str("\n");
}

void bc_print_state(void) {
    print_str("[BC] Rt="); print_int(bc.n_routes);
    print_str(" Bu="); print_int(bc.n_buses);
    print_str(" Dr="); print_int(bc.n_drivers);
    print_str(" Tk="); print_int(bc.n_tickets);
    print_str(" Px="); print_int(bc.n_passengers);
    print_str("\n");
}

int main(void) {
    print_str("=== Bus Company Demo ===\n\n");
    bc_init();

    print_str("Adding routes...\n");
    for (int i = 0; i < 14; i++) {
        int zone = (i % 4) + 1;
        int length = 10 + (i * 5);
        int stops = 10 + (i * 3);
        int freq = 10 + (i % 20);
        int hours = 16 + (i % 8);
        int year = 2010 + (i % 10);
        bc_add_route(zone, length, stops, freq, hours, year);
    }

    print_str("\nAdding buses...\n");
    for (int i = 0; i < 18; i++) {
        int route = i % 14;
        int type = (i % 3) + 1;
        int cap = 40 + (i * 5);
        int year = 2015 + (i % 8);
        bc_add_bus(route, type, cap, year);
    }

    print_str("\nAdding drivers...\n");
    for (int i = 0; i < 16; i++) {
        int route = i % 14;
        int lic = (i % 3) + 1;
        int yrs = 1 + (i % 20);
        int shift = (i % 3) + 1;
        int rating = 70 + (i % 30);
        bc_add_driver(route, lic, yrs, shift, rating);
    }

    print_str("\nRecording incidents...\n");
    for (int i = 0; i < 5; i++) {
        bc_record_incident(i * 3);
    }

    print_str("\nAdding tickets...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 3) + 1;
        int zone = (i % 4) + 1;
        int price = 2 + (i % 5);
        int sold = 5000 + (i * 1000);
        int year = 2024;
        int month = 1 + (i % 12);
        bc_add_ticket(type, zone, price, sold, year, month);
    }

    print_str("\nAdding passenger data...\n");
    for (int i = 0; i < 10; i++) {
        int route = i % 14;
        int zone = (i % 4) + 1;
        int daily = 2000 + (i * 500);
        int peak = 7 + (i % 10);
        int sat = 75 + (i % 25);
        int year = 2024;
        bc_add_passenger(route, zone, daily, peak, sat, year);
    }

    print_str("\nRoute report...\n");
    bc_route_report();

    print_str("\nRevenue report...\n");
    bc_revenue_report();

    print_str("\nSafety report...\n");
    bc_safety_report();

    print_str("\nFinal state...\n");
    bc_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
