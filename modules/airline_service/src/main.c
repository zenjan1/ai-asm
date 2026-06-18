/* airline_service: Airline service system (v1.0)
 * Flight management, ticketing, check-in, onboard services, operations monitoring
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

#define MAX_FLIGHTS   10
#define MAX_TICKETS   20
#define MAX_PASSENGERS 16
#define MAX_CREW      12
#define MAX_ANOMALIES 8

typedef struct {
    int    flight_id;
    int    route_from;
    int    route_to;
    int    scheduled_hour;
    int    actual_hour;
    int    capacity;
    int    booked;
    int    status;
    int    active;
} flight_t;

typedef struct {
    int    ticket_id;
    int    flight_id;
    int    passenger_id;
    double price;
    int    seat_class;
    int    seat_num;
    int    checked_in;
    int    active;
} ticket_t;

typedef struct {
    int    passenger_id;
    int    tier;
    int    miles;
    int    flights_flown;
    int    active;
} passenger_t;

typedef struct {
    int    crew_id;
    int    role;
    int    flight_id;
    int    hours_flown;
    int    active;
} crew_t;

typedef struct {
    int    anomaly_id;
    int    flight_id;
    int    type;
    int    severity;
    int    resolved;
    int    active;
} anomaly_t;

typedef struct {
    int    n_flights;
    int    n_tickets;
    int    n_passengers;
    int    n_crew;
    int    n_anomalies;
    double total_revenue;
    double total_miles;
    int    total_checked_in;
    int    on_time_flights;
    int    delayed_flights;
    int    cancelled_flights;
} as_state_t;

static flight_t flights[MAX_FLIGHTS];
static ticket_t tickets[MAX_TICKETS];
static passenger_t passengers[MAX_PASSENGERS];
static crew_t crew[MAX_CREW];
static anomaly_t anomalies[MAX_ANOMALIES];
static as_state_t as;

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

int as_init(void) {
    if (initialized) return -1;
    as.n_flights = 0; as.n_tickets = 0; as.n_passengers = 0;
    as.n_crew = 0; as.n_anomalies = 0;
    as.total_revenue = 0.0; as.total_miles = 0.0;
    as.total_checked_in = 0; as.on_time_flights = 0;
    as.delayed_flights = 0; as.cancelled_flights = 0;
    for (int i = 0; i < MAX_FLIGHTS; i++) flights[i].active = 0;
    for (int i = 0; i < MAX_TICKETS; i++) tickets[i].active = 0;
    for (int i = 0; i < MAX_PASSENGERS; i++) passengers[i].active = 0;
    for (int i = 0; i < MAX_CREW; i++) crew[i].active = 0;
    for (int i = 0; i < MAX_ANOMALIES; i++) anomalies[i].active = 0;
    initialized = 1;
    print_str("[AS] Airline service initialized\n");
    return 0;
}

int as_create_flight(int from, int to, int hour, int capacity) {
    if (as.n_flights >= MAX_FLIGHTS) return -1;
    flight_t* f = &flights[as.n_flights];
    f->flight_id = as.n_flights;
    f->route_from = from;
    f->route_to = to;
    f->scheduled_hour = hour;
    f->actual_hour = hour;
    f->capacity = capacity;
    f->booked = 0;
    f->status = 1;
    f->active = 1;
    as.n_flights++;
    print_str("[AS] Flight "); print_int(as.n_flights - 1);
    print_str(" R"); print_int(from); print_str("->R"); print_int(to);
    print_str(" "); print_int(hour); print_str(":00");
    print_str(" cap="); print_int(capacity); print_str("\n");
    return as.n_flights - 1;
}

int as_register_passenger(int tier) {
    if (as.n_passengers >= MAX_PASSENGERS) return -1;
    passenger_t* p = &passengers[as.n_passengers];
    p->passenger_id = as.n_passengers;
    p->tier = tier;
    p->miles = tier * 5000;
    p->flights_flown = 0;
    p->active = 1;
    as.n_passengers++;
    print_str("[AS] Passenger "); print_int(as.n_passengers - 1);
    print_str(" tier="); print_int(tier);
    print_str(" miles="); print_int(p->miles); print_str("\n");
    return as.n_passengers - 1;
}

int as_add_crew(int role) {
    if (as.n_crew >= MAX_CREW) return -1;
    crew_t* c = &crew[as.n_crew];
    c->crew_id = as.n_crew;
    c->role = role;
    c->flight_id = -1;
    c->hours_flown = 0;
    c->active = 1;
    as.n_crew++;
    print_str("[AS] Crew "); print_int(as.n_crew - 1);
    print_str(" role="); print_int(role); print_str("\n");
    return as.n_crew - 1;
}

int as_sell_ticket(int flight_id, int passenger_id, int seat_class) {
    if (as.n_tickets >= MAX_TICKETS) return -1;
    if (flight_id >= as.n_flights || passenger_id >= as.n_passengers) return -1;
    flight_t* f = &flights[flight_id];
    if (f->booked >= f->capacity) return -2;
    double base_prices[3] = {200.0, 500.0, 1200.0};
    double price = base_prices[seat_class < 3 ? seat_class : 0];
    if (passengers[passenger_id].tier >= 2) price *= 0.9;
    ticket_t* t = &tickets[as.n_tickets];
    t->ticket_id = as.n_tickets;
    t->flight_id = flight_id;
    t->passenger_id = passenger_id;
    t->price = price;
    t->seat_class = seat_class;
    t->seat_num = f->booked + 1;
    t->checked_in = 0;
    t->active = 1;
    f->booked++;
    as.n_tickets++;
    as.total_revenue += price;
    print_str("[AS] Ticket "); print_int(as.n_tickets - 1);
    print_str(" F"); print_int(flight_id);
    print_str(" P"); print_int(passenger_id);
    print_str(" class="); print_int(seat_class);
    print_str(" seat="); print_int(t->seat_num);
    print_str(" price="); print_int((int)price); print_str("\n");
    return as.n_tickets - 1;
}

void as_check_in(int ticket_id) {
    if (ticket_id >= as.n_tickets) return;
    ticket_t* t = &tickets[ticket_id];
    t->checked_in = 1;
    as.total_checked_in++;
    print_str("[AS] Check-in T"); print_int(ticket_id);
    print_str(" P"); print_int(t->passenger_id);
    print_str(" F"); print_int(t->flight_id);
    print_str(" seat="); print_int(t->seat_num);
    print_str(" class="); print_int(t->seat_class); print_str("\n");
}

void as_assign_crew(int crew_id, int flight_id) {
    if (crew_id >= as.n_crew || flight_id >= as.n_flights) return;
    crew[crew_id].flight_id = flight_id;
    print_str("[AS] Crew "); print_int(crew_id);
    print_str(" assigned to F"); print_int(flight_id); print_str("\n");
}

void as_depart_flight(int flight_id) {
    if (flight_id >= as.n_flights) return;
    flight_t* f = &flights[flight_id];
    f->actual_hour = f->scheduled_hour + ((flight_id * 7) % 3);
    f->status = 2;
    int delay = f->actual_hour - f->scheduled_hour;
    if (delay == 0) {
        as.on_time_flights++;
        print_str("[AS] Flight "); print_int(flight_id); print_str(" departed ON TIME\n");
    } else if (delay <= 1) {
        as.on_time_flights++;
        print_str("[AS] Flight "); print_int(flight_id);
        print_str(" departed delayed "); print_int(delay); print_str("h\n");
    } else {
        as.delayed_flights++;
        print_str("[AS] Flight "); print_int(flight_id);
        print_str(" SIGNIFICANTLY delayed "); print_int(delay); print_str("h\n");
    }
}

void as_complete_flight(int flight_id) {
    if (flight_id >= as.n_flights) return;
    flight_t* f = &flights[flight_id];
    f->status = 3;
    int route_distance = 500 + (flight_id * 137) % 2000;
    for (int i = 0; i < as.n_tickets; i++) {
        if (tickets[i].flight_id == flight_id && tickets[i].checked_in) {
            int pax = tickets[i].passenger_id;
            passengers[pax].miles += route_distance;
            passengers[pax].flights_flown++;
            as.total_miles += route_distance;
        }
    }
    for (int i = 0; i < as.n_crew; i++) {
        if (crew[i].flight_id == flight_id) {
            crew[i].hours_flown += route_distance / 500;
            crew[i].flight_id = -1;
        }
    }
    print_str("[AS] Flight "); print_int(flight_id); print_str(" completed");
    print_str(" distance="); print_int(route_distance); print_str("km\n");
}

int as_report_anomaly(int flight_id, int type, int severity) {
    if (as.n_anomalies >= MAX_ANOMALIES || flight_id >= as.n_flights) return -1;
    anomaly_t* a = &anomalies[as.n_anomalies];
    a->anomaly_id = as.n_anomalies;
    a->flight_id = flight_id;
    a->type = type;
    a->severity = severity;
    a->resolved = 0;
    a->active = 1;
    as.n_anomalies++;
    print_str("[AS] ANOMALY "); print_int(as.n_anomalies - 1);
    print_str(" F"); print_int(flight_id);
    print_str(" type="); print_int(type);
    print_str(" sev="); print_int(severity); print_str("\n");
    return as.n_anomalies - 1;
}

void as_resolve_anomaly(int anomaly_id) {
    if (anomaly_id >= as.n_anomalies) return;
    anomalies[anomaly_id].resolved = 1;
    print_str("[AS] Anomaly "); print_int(anomaly_id); print_str(" resolved\n");
}

void as_operations_report(void) {
    print_str("[AS] Operations report:\n");
    print_str("  Flights: "); print_int(as.n_flights); print_str("\n");
    print_str("  On time: "); print_int(as.on_time_flights); print_str("\n");
    print_str("  Delayed: "); print_int(as.delayed_flights); print_str("\n");
    print_str("  Cancelled: "); print_int(as.cancelled_flights); print_str("\n");
    double otp = (as.n_flights > 0) ? (double)as.on_time_flights / as.n_flights * 100.0 : 0.0;
    print_str("  OTP: "); print_int((int)otp); print_str("%\n");
    int crew_util = 0, crew_count = 0;
    for (int i = 0; i < as.n_crew; i++) {
        crew_util += crew[i].hours_flown;
        crew_count++;
    }
    print_str("  Avg crew hours: "); print_int(crew_count > 0 ? crew_util / crew_count : 0); print_str("\n");
    print_str("  Unresolved anomalies: ");
    int unresolved = 0;
    for (int i = 0; i < as.n_anomalies; i++) {
        if (!anomalies[i].resolved) unresolved++;
    }
    print_int(unresolved); print_str("\n");
}

void as_print_state(void) {
    print_str("[AS] Flights="); print_int(as.n_flights);
    print_str(" Tickets="); print_int(as.n_tickets);
    print_str(" Pax="); print_int(as.n_passengers);
    print_str(" Crew="); print_int(as.n_crew);
    print_str(" Anomalies="); print_int(as.n_anomalies);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)as.total_revenue); print_str("\n");
    print_str("  Miles awarded: "); print_int((int)as.total_miles); print_str("\n");
    print_str("  Checked in: "); print_int(as.total_checked_in); print_str("\n");
    for (int i = 0; i < as.n_flights; i++) {
        print_str("  Flight "); print_int(i);
        print_str(" booked="); print_int(flights[i].booked);
        print_str("/"); print_int(flights[i].capacity);
        print_str(" status="); print_int(flights[i].status); print_str("\n");
    }
}

int main(void) {
    print_str("=== Airline Service System Demo ===\n\n");
    as_init();

    print_str("Creating flights...\n");
    as_create_flight(1, 2, 8, 150);
    as_create_flight(2, 1, 10, 150);
    as_create_flight(1, 3, 12, 200);
    as_create_flight(3, 1, 14, 200);
    as_create_flight(2, 3, 9, 120);
    as_create_flight(3, 2, 16, 120);
    as_create_flight(1, 4, 7, 180);
    as_create_flight(4, 1, 20, 180);

    print_str("\nRegistering passengers...\n");
    as_register_passenger(0);
    as_register_passenger(1);
    as_register_passenger(2);
    as_register_passenger(3);
    as_register_passenger(1);
    as_register_passenger(0);
    as_register_passenger(2);
    as_register_passenger(1);
    as_register_passenger(0);
    as_register_passenger(3);

    print_str("\nAdding crew...\n");
    as_add_crew(1);
    as_add_crew(1);
    as_add_crew(2);
    as_add_crew(2);
    as_add_crew(3);
    as_add_crew(3);
    as_add_crew(2);
    as_add_crew(1);

    print_str("\nSelling tickets...\n");
    as_sell_ticket(0, 0, 0);
    as_sell_ticket(0, 1, 1);
    as_sell_ticket(0, 2, 2);
    as_sell_ticket(0, 3, 0);
    as_sell_ticket(1, 4, 1);
    as_sell_ticket(1, 5, 0);
    as_sell_ticket(2, 6, 2);
    as_sell_ticket(2, 7, 0);
    as_sell_ticket(3, 8, 1);
    as_sell_ticket(4, 9, 0);
    as_sell_ticket(5, 0, 0);
    as_sell_ticket(6, 1, 1);
    as_sell_ticket(6, 3, 2);
    as_sell_ticket(7, 5, 0);
    as_sell_ticket(7, 7, 1);

    print_str("\nChecking in passengers...\n");
    as_check_in(0);
    as_check_in(1);
    as_check_in(2);
    as_check_in(3);
    as_check_in(4);
    as_check_in(6);
    as_check_in(7);
    as_check_in(8);
    as_check_in(9);
    as_check_in(12);
    as_check_in(13);

    print_str("\nAssigning crew...\n");
    as_assign_crew(0, 0);
    as_assign_crew(1, 0);
    as_assign_crew(2, 0);
    as_assign_crew(3, 1);
    as_assign_crew(4, 1);
    as_assign_crew(5, 2);
    as_assign_crew(6, 3);
    as_assign_crew(7, 4);

    print_str("\nFlight departures...\n");
    as_depart_flight(0);
    as_depart_flight(1);
    as_depart_flight(2);
    as_depart_flight(3);
    as_depart_flight(4);

    print_str("\nFlight completions...\n");
    as_complete_flight(0);
    as_complete_flight(1);
    as_complete_flight(2);

    print_str("\nReporting anomalies...\n");
    as_report_anomaly(3, 1, 2);
    as_report_anomaly(4, 2, 3);
    as_report_anomaly(3, 3, 1);

    print_str("\nResolving anomalies...\n");
    as_resolve_anomaly(0);
    as_resolve_anomaly(2);

    print_str("\nOperations report...\n");
    as_operations_report();

    print_str("\nFinal state...\n");
    as_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
