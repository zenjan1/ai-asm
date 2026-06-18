/* conference_center: Conference center management (v1.0)
 * Rooms, events, clients, services, equipment, revenue
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

#define MAX_ROOMS     8
#define MAX_EVENTS    14
#define MAX_CLIENTS   12
#define MAX_EQUIPMENT 10
#define MAX_SERVICES  10

typedef struct {
    int    room_id;
    int    size;
    int    capacity;
    double hourly_rate;
    int    n_events;
    int    total_hours;
    double revenue;
    int    active;
} room_t;

typedef struct {
    int    event_id;
    int    room_id;
    int    client_id;
    int    type;
    int    n_attendees;
    int    duration_hrs;
    double room_fee;
    double service_fee;
    double equip_fee;
    double total;
    int    day;
    int    active;
} event_t;

typedef struct {
    int    client_id;
    int    type;
    int    n_events;
    double total_spent;
    int    discount_pct;
    int    active;
} client_t;

typedef struct {
    int    equip_id;
    int    type;
    double rental_rate;
    int    available;
    int    n_rentals;
    double revenue;
    int    active;
} equipment_t;

typedef struct {
    int    service_id;
    int    type;
    double price;
    double cost;
    int    n_provided;
    double revenue;
    int    active;
} service_t;

typedef struct {
    int    n_rooms;
    int    n_events;
    int    n_clients;
    int    n_equipment;
    int    n_services;
    double room_revenue;
    double service_revenue;
    double equip_revenue;
    double catering_revenue;
    double total_revenue;
    double total_costs;
    int    total_attendees;
} cc_state_t;

static room_t rooms[MAX_ROOMS];
static event_t events[MAX_EVENTS];
static client_t clients[MAX_CLIENTS];
static equipment_t equip[MAX_EQUIPMENT];
static service_t services[MAX_SERVICES];
static cc_state_t cc;

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

int cc_init(void) {
    if (initialized) return -1;
    cc.n_rooms = 0; cc.n_events = 0; cc.n_clients = 0;
    cc.n_equipment = 0; cc.n_services = 0;
    cc.room_revenue = 0.0; cc.service_revenue = 0.0;
    cc.equip_revenue = 0.0; cc.catering_revenue = 0.0;
    cc.total_revenue = 0.0; cc.total_costs = 0.0;
    cc.total_attendees = 0;
    for (int i = 0; i < MAX_ROOMS; i++) rooms[i].active = 0;
    for (int i = 0; i < MAX_EVENTS; i++) events[i].active = 0;
    for (int i = 0; i < MAX_CLIENTS; i++) clients[i].active = 0;
    for (int i = 0; i < MAX_EQUIPMENT; i++) equip[i].active = 0;
    for (int i = 0; i < MAX_SERVICES; i++) services[i].active = 0;
    initialized = 1;
    print_str("[CC] Conference center initialized\n");
    return 0;
}

int cc_add_room(int size, int capacity, double rate) {
    if (cc.n_rooms >= MAX_ROOMS) return -1;
    room_t* r = &rooms[cc.n_rooms];
    r->room_id = cc.n_rooms;
    r->size = size;
    r->capacity = capacity;
    r->hourly_rate = rate;
    r->n_events = 0;
    r->total_hours = 0;
    r->revenue = 0.0;
    r->active = 1;
    cc.n_rooms++;
    print_str("[CC] Room "); print_int(cc.n_rooms - 1);
    print_str(" size="); print_int(size);
    print_str(" cap="); print_int(capacity);
    print_str(" rate="); print_int((int)rate); print_str("/hr\n");
    return cc.n_rooms - 1;
}

int cc_register_client(int type, int discount) {
    if (cc.n_clients >= MAX_CLIENTS) return -1;
    client_t* c = &clients[cc.n_clients];
    c->client_id = cc.n_clients;
    c->type = type;
    c->n_events = 0;
    c->total_spent = 0.0;
    c->discount_pct = discount;
    c->active = 1;
    cc.n_clients++;
    print_str("[CC] Client "); print_int(cc.n_clients - 1);
    print_str(" type="); print_int(type);
    print_str(" disc="); print_int(discount); print_str("%\n");
    return cc.n_clients - 1;
}

int cc_add_equipment(int type, double rate) {
    if (cc.n_equipment >= MAX_EQUIPMENT) return -1;
    equipment_t* e = &equip[cc.n_equipment];
    e->equip_id = cc.n_equipment;
    e->type = type;
    e->rental_rate = rate;
    e->available = 1;
    e->n_rentals = 0;
    e->revenue = 0.0;
    e->active = 1;
    cc.n_equipment++;
    print_str("[CC] Equip "); print_int(cc.n_equipment - 1);
    print_str(" type="); print_int(type);
    print_str(" rate="); print_int((int)rate); print_str("\n");
    return cc.n_equipment - 1;
}

int cc_add_service(int type, double price, double cost) {
    if (cc.n_services >= MAX_SERVICES) return -1;
    service_t* s = &services[cc.n_services];
    s->service_id = cc.n_services;
    s->type = type;
    s->price = price;
    s->cost = cost;
    s->n_provided = 0;
    s->revenue = 0.0;
    s->active = 1;
    cc.n_services++;
    print_str("[CC] Service "); print_int(cc.n_services - 1);
    print_str(" type="); print_int(type);
    print_str(" price="); print_int((int)price); print_str("\n");
    return cc.n_services - 1;
}

int cc_book_event(int room_id, int client_id, int type, int attendees, int duration, int day) {
    if (cc.n_events >= MAX_EVENTS) return -1;
    if (room_id >= cc.n_rooms || client_id >= cc.n_clients) return -2;
    room_t* r = &rooms[room_id];
    client_t* c = &clients[client_id];
    event_t* ev = &events[cc.n_events];
    ev->event_id = cc.n_events;
    ev->room_id = room_id;
    ev->client_id = client_id;
    ev->type = type;
    ev->n_attendees = attendees;
    ev->duration_hrs = duration;
    double disc = 1.0 - c->discount_pct / 100.0;
    ev->room_fee = r->hourly_rate * duration * disc;
    ev->service_fee = 0.0;
    ev->equip_fee = 0.0;
    ev->total = ev->room_fee;
    ev->day = day;
    ev->active = 1;
    r->n_events++;
    r->total_hours += duration;
    c->n_events++;
    cc.room_revenue += ev->room_fee;
    cc.total_revenue += ev->room_fee;
    cc.total_attendees += attendees;
    cc.n_events++;
    print_str("[CC] Event "); print_int(cc.n_events - 1);
    print_str(" R"); print_int(room_id);
    print_str(" Cl"); print_int(client_id);
    print_str(" type="); print_int(type);
    print_str(" att="); print_int(attendees);
    print_str(" dur="); print_int(duration); print_str("hr");
    print_str(" room$="); print_int((int)ev->room_fee); print_str("\n");
    return cc.n_events - 1;
}

int cc_add_event_service(int event_id, int service_id) {
    if (event_id >= cc.n_events || service_id >= cc.n_services) return -1;
    event_t* ev = &events[event_id];
    service_t* s = &services[service_id];
    ev->service_fee += s->price;
    ev->total += s->price;
    s->n_provided++;
    s->revenue += s->price;
    cc.service_revenue += s->price;
    cc.total_revenue += s->price;
    cc.total_costs += s->cost;
    print_str("[CC] Event "); print_int(event_id);
    print_str(" added Sv"); print_int(service_id);
    print_str(" price="); print_int((int)s->price); print_str("\n");
    return 0;
}

int cc_add_event_equipment(int event_id, int equip_id) {
    if (event_id >= cc.n_events || equip_id >= cc.n_equipment) return -1;
    event_t* ev = &events[event_id];
    equipment_t* e = &equip[equip_id];
    ev->equip_fee += e->rental_rate;
    ev->total += e->rental_rate;
    e->n_rentals++;
    e->revenue += e->rental_rate;
    cc.equip_revenue += e->rental_rate;
    cc.total_revenue += e->rental_rate;
    print_str("[CC] Event "); print_int(event_id);
    print_str(" added Eq"); print_int(equip_id);
    print_str(" rate="); print_int((int)e->rental_rate); print_str("\n");
    return 0;
}

void cc_room_report(void) {
    print_str("[CC] Room report:\n");
    for (int i = 0; i < cc.n_rooms; i++) {
        room_t* r = &rooms[i];
        print_str("  R"); print_int(i);
        print_str(" cap="); print_int(r->capacity);
        print_str(" events="); print_int(r->n_events);
        print_str(" hrs="); print_int(r->total_hours);
        print_str(" rev="); print_int((int)r->revenue); print_str("\n");
    }
}

void cc_financial_report(void) {
    print_str("[CC] Financial report:\n");
    print_str("  Room revenue: "); print_int((int)cc.room_revenue); print_str("\n");
    print_str("  Service revenue: "); print_int((int)cc.service_revenue); print_str("\n");
    print_str("  Equipment revenue: "); print_int((int)cc.equip_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)cc.total_revenue); print_str("\n");
    print_str("  Costs: "); print_int((int)cc.total_costs); print_str("\n");
    double profit = cc.total_revenue - cc.total_costs;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    print_str("  Total attendees: "); print_int(cc.total_attendees); print_str("\n");
    print_str("  Total events: "); print_int(cc.n_events); print_str("\n");
}

void cc_print_state(void) {
    print_str("[CC] Rooms="); print_int(cc.n_rooms);
    print_str(" Events="); print_int(cc.n_events);
    print_str(" Clients="); print_int(cc.n_clients);
    print_str(" Equip="); print_int(cc.n_equipment);
    print_str(" Svcs="); print_int(cc.n_services);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)cc.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Conference Center Demo ===\n\n");
    cc_init();

    print_str("Adding rooms...\n");
    cc_add_room(1, 20, 150.0);
    cc_add_room(1, 30, 200.0);
    cc_add_room(2, 50, 350.0);
    cc_add_room(2, 80, 500.0);
    cc_add_room(3, 150, 800.0);
    cc_add_room(3, 200, 1000.0);
    cc_add_room(1, 15, 120.0);
    cc_add_room(2, 60, 400.0);

    print_str("\nRegistering clients...\n");
    cc_register_client(1, 0);
    cc_register_client(1, 5);
    cc_register_client(2, 10);
    cc_register_client(2, 15);
    cc_register_client(3, 0);
    cc_register_client(3, 5);
    cc_register_client(1, 10);
    cc_register_client(2, 0);
    cc_register_client(3, 15);
    cc_register_client(1, 5);
    cc_register_client(2, 10);
    cc_register_client(3, 0);

    print_str("\nAdding equipment...\n");
    cc_add_equipment(1, 100.0);
    cc_add_equipment(1, 80.0);
    cc_add_equipment(2, 200.0);
    cc_add_equipment(2, 150.0);
    cc_add_equipment(3, 300.0);
    cc_add_equipment(3, 250.0);
    cc_add_equipment(4, 50.0);
    cc_add_equipment(4, 75.0);
    cc_add_equipment(5, 120.0);
    cc_add_equipment(5, 90.0);

    print_str("\nAdding services...\n");
    cc_add_service(1, 500.0, 200.0);
    cc_add_service(1, 800.0, 350.0);
    cc_add_service(2, 300.0, 100.0);
    cc_add_service(2, 450.0, 150.0);
    cc_add_service(3, 1000.0, 400.0);
    cc_add_service(3, 1500.0, 600.0);
    cc_add_service(4, 200.0, 50.0);
    cc_add_service(4, 350.0, 100.0);
    cc_add_service(5, 600.0, 200.0);
    cc_add_service(5, 900.0, 350.0);

    print_str("\nBooking events...\n");
    cc_book_event(0, 0, 1, 15, 4, 10);
    cc_book_event(1, 1, 1, 25, 6, 10);
    cc_book_event(2, 2, 2, 40, 8, 11);
    cc_book_event(3, 3, 2, 70, 8, 11);
    cc_book_event(4, 4, 3, 120, 10, 12);
    cc_book_event(5, 5, 3, 180, 12, 12);
    cc_book_event(6, 6, 1, 12, 3, 13);
    cc_book_event(7, 7, 2, 50, 6, 13);
    cc_book_event(0, 8, 1, 18, 4, 14);
    cc_book_event(1, 9, 2, 28, 5, 14);
    cc_book_event(2, 10, 3, 45, 8, 15);
    cc_book_event(3, 11, 1, 65, 6, 15);
    cc_book_event(4, 0, 2, 130, 8, 16);
    cc_book_event(5, 2, 3, 160, 10, 16);

    print_str("\nAdding services to events...\n");
    cc_add_event_service(0, 0);
    cc_add_event_service(1, 1);
    cc_add_event_service(2, 2);
    cc_add_event_service(3, 3);
    cc_add_event_service(4, 4);
    cc_add_event_service(5, 5);
    cc_add_event_service(6, 6);
    cc_add_event_service(7, 7);
    cc_add_event_service(8, 8);
    cc_add_event_service(9, 9);
    cc_add_event_service(10, 0);
    cc_add_event_service(11, 2);
    cc_add_event_service(12, 4);
    cc_add_event_service(13, 5);

    print_str("\nAdding equipment to events...\n");
    cc_add_event_equipment(0, 0);
    cc_add_event_equipment(1, 1);
    cc_add_event_equipment(2, 2);
    cc_add_event_equipment(3, 3);
    cc_add_event_equipment(4, 4);
    cc_add_event_equipment(5, 5);
    cc_add_event_equipment(6, 6);
    cc_add_event_equipment(7, 7);
    cc_add_event_equipment(8, 8);
    cc_add_event_equipment(9, 9);
    cc_add_event_equipment(10, 0);
    cc_add_event_equipment(11, 1);
    cc_add_event_equipment(12, 2);
    cc_add_event_equipment(13, 3);

    print_str("\nRoom report...\n");
    cc_room_report();

    print_str("\nFinancial report...\n");
    cc_financial_report();

    print_str("\nFinal state...\n");
    cc_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
