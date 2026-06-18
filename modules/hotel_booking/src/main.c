/* hotel_booking: Hotel reservation management (v1.0)
 * Rooms, bookings, guests, services, billing, analytics
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

#define MAX_ROOMS     24
#define MAX_GUESTS    16
#define MAX_BOOKINGS  18
#define MAX_SERVICES  12
#define MAX_MEMBERS   10

typedef struct {
    int    room_id;
    int    floor;
    int    type;
    int    status;
    double nightly_rate;
    int    guest_id;
    int    n_stays;
    double revenue;
    int    active;
} room_t;

typedef struct {
    int    guest_id;
    int    tier;
    int    n_stays;
    int    points;
    double total_paid;
    int    active;
} guest_t;

typedef struct {
    int    booking_id;
    int    guest_id;
    int    room_id;
    int    checkin_day;
    int    nights;
    double total;
    int    status;
    int    active;
} booking_t;

typedef struct {
    int    service_id;
    int    type;
    double price;
    double cost;
    int    n_ordered;
    double revenue;
    int    active;
} service_t;

typedef struct {
    int    member_id;
    int    tier;
    int    guest_id;
    int    join_day;
    int    points;
    double total_spent;
    int    active;
} member_t;

typedef struct {
    int    n_rooms;
    int    n_guests;
    int    n_bookings;
    int    n_services;
    int    n_members;
    double room_revenue;
    double service_revenue;
    double member_revenue;
    double total_revenue;
    int    total_stays;
    int    occupied_rooms;
} hb_state_t;

static room_t rooms[MAX_ROOMS];
static guest_t guests[MAX_GUESTS];
static booking_t bookings[MAX_BOOKINGS];
static service_t services[MAX_SERVICES];
static member_t members[MAX_MEMBERS];
static hb_state_t hb;

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

int hb_init(void) {
    if (initialized) return -1;
    hb.n_rooms = 0; hb.n_guests = 0; hb.n_bookings = 0;
    hb.n_services = 0; hb.n_members = 0;
    hb.room_revenue = 0.0; hb.service_revenue = 0.0;
    hb.member_revenue = 0.0; hb.total_revenue = 0.0;
    hb.total_stays = 0; hb.occupied_rooms = 0;
    for (int i = 0; i < MAX_ROOMS; i++) rooms[i].active = 0;
    for (int i = 0; i < MAX_GUESTS; i++) guests[i].active = 0;
    for (int i = 0; i < MAX_BOOKINGS; i++) bookings[i].active = 0;
    for (int i = 0; i < MAX_SERVICES; i++) services[i].active = 0;
    for (int i = 0; i < MAX_MEMBERS; i++) members[i].active = 0;
    initialized = 1;
    print_str("[HB] Hotel booking initialized\n");
    return 0;
}

int hb_add_room(int floor, int type, double rate) {
    if (hb.n_rooms >= MAX_ROOMS) return -1;
    room_t* r = &rooms[hb.n_rooms];
    r->room_id = hb.n_rooms;
    r->floor = floor;
    r->type = type;
    r->status = 1;
    r->nightly_rate = rate;
    r->guest_id = -1;
    r->n_stays = 0;
    r->revenue = 0.0;
    r->active = 1;
    hb.n_rooms++;
    print_str("[HB] Room "); print_int(hb.n_rooms - 1);
    print_str(" floor="); print_int(floor);
    print_str(" type="); print_int(type);
    print_str(" rate="); print_int((int)rate); print_str("/night\n");
    return hb.n_rooms - 1;
}

int hb_register_guest(int tier) {
    if (hb.n_guests >= MAX_GUESTS) return -1;
    guest_t* g = &guests[hb.n_guests];
    g->guest_id = hb.n_guests;
    g->tier = tier;
    g->n_stays = 0;
    g->points = 0;
    g->total_paid = 0.0;
    g->active = 1;
    hb.n_guests++;
    print_str("[HB] Guest "); print_int(hb.n_guests - 1);
    print_str(" tier="); print_int(tier); print_str("\n");
    return hb.n_guests - 1;
}

int hb_add_service(int type, double price, double cost) {
    if (hb.n_services >= MAX_SERVICES) return -1;
    service_t* s = &services[hb.n_services];
    s->service_id = hb.n_services;
    s->type = type;
    s->price = price;
    s->cost = cost;
    s->n_ordered = 0;
    s->revenue = 0.0;
    s->active = 1;
    hb.n_services++;
    print_str("[HB] Service "); print_int(hb.n_services - 1);
    print_str(" type="); print_int(type);
    print_str(" price="); print_int((int)price); print_str("\n");
    return hb.n_services - 1;
}

int hb_make_booking(int guest_id, int room_id, int checkin_day, int nights) {
    if (hb.n_bookings >= MAX_BOOKINGS) return -1;
    if (guest_id >= hb.n_guests || room_id >= hb.n_rooms) return -2;
    room_t* r = &rooms[room_id];
    if (r->status != 1) return -3;
    booking_t* b = &bookings[hb.n_bookings];
    b->booking_id = hb.n_bookings;
    b->guest_id = guest_id;
    b->room_id = room_id;
    b->checkin_day = checkin_day;
    b->nights = nights;
    double discount = 1.0;
    if (guests[guest_id].tier >= 2) discount = 0.9;
    if (guests[guest_id].tier >= 3) discount = 0.8;
    b->total = r->nightly_rate * nights * discount;
    b->status = 1;
    b->active = 1;
    hb.n_bookings++;
    print_str("[HB] Booking "); print_int(hb.n_bookings - 1);
    print_str(" G"); print_int(guest_id);
    print_str(" R"); print_int(room_id);
    print_str(" day="); print_int(checkin_day);
    print_str(" nights="); print_int(nights);
    print_str(" total="); print_int((int)b->total); print_str("\n");
    return hb.n_bookings - 1;
}

int hb_checkin(int booking_id) {
    if (booking_id >= hb.n_bookings) return -1;
    booking_t* b = &bookings[booking_id];
    room_t* r = &rooms[b->room_id];
    if (r->status != 1) return -2;
    r->status = 2;
    r->guest_id = b->guest_id;
    b->status = 2;
    hb.occupied_rooms++;
    print_str("[HB] Checkin B"); print_int(booking_id);
    print_str(" G"); print_int(b->guest_id);
    print_str(" R"); print_int(b->room_id); print_str("\n");
    return 0;
}

int hb_checkout(int booking_id) {
    if (booking_id >= hb.n_bookings) return -1;
    booking_t* b = &bookings[booking_id];
    if (b->status != 2) return -2;
    room_t* r = &rooms[b->room_id];
    r->status = 1;
    r->guest_id = -1;
    r->n_stays++;
    r->revenue += b->total;
    b->status = 3;
    guests[b->guest_id].n_stays++;
    guests[b->guest_id].total_paid += b->total;
    guests[b->guest_id].points += (int)b->total;
    hb.room_revenue += b->total;
    hb.total_revenue += b->total;
    hb.total_stays++;
    hb.occupied_rooms--;
    print_str("[HB] Checkout B"); print_int(booking_id);
    print_str(" G"); print_int(b->guest_id);
    print_str(" total="); print_int((int)b->total); print_str("\n");
    return 0;
}

int hb_order_service(int service_id, int booking_id, int qty) {
    if (service_id >= hb.n_services || booking_id >= hb.n_bookings) return -1;
    service_t* s = &services[service_id];
    double revenue = s->price * qty;
    s->n_ordered += qty;
    s->revenue += revenue;
    bookings[booking_id].total += revenue;
    hb.service_revenue += revenue;
    hb.total_revenue += revenue;
    print_str("[HB] Service S"); print_int(service_id);
    print_str(" B"); print_int(booking_id);
    print_str(" qty="); print_int(qty);
    print_str(" charge="); print_int((int)revenue); print_str("\n");
    return 0;
}

int hb_add_member(int tier, int guest_id, int join_day) {
    if (hb.n_members >= MAX_MEMBERS || guest_id >= hb.n_guests) return -1;
    member_t* m = &members[hb.n_members];
    m->member_id = hb.n_members;
    m->tier = tier;
    m->guest_id = guest_id;
    m->join_day = join_day;
    m->points = 0;
    m->total_spent = 0.0;
    m->active = 1;
    guests[guest_id].tier = tier;
    hb.n_members++;
    print_str("[HB] Member "); print_int(hb.n_members - 1);
    print_str(" tier="); print_int(tier);
    print_str(" G"); print_int(guest_id); print_str("\n");
    return hb.n_members - 1;
}

void hb_room_status(void) {
    print_str("[HB] Room status:\n");
    for (int i = 0; i < hb.n_rooms; i++) {
        room_t* r = &rooms[i];
        print_str("  R"); print_int(i);
        print_str(" F"); print_int(r->floor);
        print_str(" type="); print_int(r->type);
        print_str(" status="); print_int(r->status);
        print_str(" stays="); print_int(r->n_stays);
        print_str(" rev="); print_int((int)r->revenue); print_str("\n");
    }
}

void hb_occupancy_report(void) {
    print_str("[HB] Occupancy report:\n");
    int total = hb.n_rooms;
    int occupied = hb.occupied_rooms;
    double occ = (total > 0) ? (double)occupied / total * 100.0 : 0.0;
    print_str("  Total rooms: "); print_int(total); print_str("\n");
    print_str("  Occupied: "); print_int(occupied); print_str("\n");
    print_str("  Available: "); print_int(total - occupied); print_str("\n");
    print_str("  Occupancy: "); print_int((int)occ); print_str("%\n");
    print_str("  Total stays: "); print_int(hb.total_stays); print_str("\n");
}

void hb_financial_report(void) {
    print_str("[HB] Financial report:\n");
    print_str("  Room revenue: "); print_int((int)hb.room_revenue); print_str("\n");
    print_str("  Service revenue: "); print_int((int)hb.service_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)hb.total_revenue); print_str("\n");
    print_str("  Members: "); print_int(hb.n_members); print_str("\n");
    print_str("  Guests: "); print_int(hb.n_guests); print_str("\n");
}

void hb_print_state(void) {
    print_str("[HB] Rooms="); print_int(hb.n_rooms);
    print_str(" Guests="); print_int(hb.n_guests);
    print_str(" Bookings="); print_int(hb.n_bookings);
    print_str(" Services="); print_int(hb.n_services);
    print_str(" Members="); print_int(hb.n_members);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)hb.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Hotel Booking Demo ===\n\n");
    hb_init();

    print_str("Adding rooms...\n");
    for (int f = 1; f <= 4; f++) {
        for (int i = 0; i < 6; i++) {
            double rate = (i < 2) ? 89.0 : (i < 4) ? 149.0 : (i < 5) ? 249.0 : 499.0;
            hb_add_room(f, (i < 2) ? 1 : (i < 4) ? 2 : (i < 5) ? 3 : 4, rate);
        }
    }

    print_str("\nRegistering guests...\n");
    for (int i = 0; i < 12; i++) {
        hb_register_guest(i % 4);
    }

    print_str("\nAdding services...\n");
    hb_add_service(1, 25.0, 8.0);
    hb_add_service(2, 45.0, 15.0);
    hb_add_service(3, 15.0, 4.0);
    hb_add_service(4, 35.0, 10.0);
    hb_add_service(1, 20.0, 6.0);
    hb_add_service(2, 55.0, 18.0);

    print_str("\nAdding members...\n");
    hb_add_member(2, 0, 1);
    hb_add_member(3, 2, 1);
    hb_add_member(2, 4, 5);
    hb_add_member(1, 6, 5);
    hb_add_member(3, 8, 10);

    print_str("\nMaking bookings...\n");
    hb_make_booking(0, 0, 10, 3);
    hb_make_booking(1, 1, 10, 2);
    hb_make_booking(2, 6, 11, 4);
    hb_make_booking(3, 7, 11, 2);
    hb_make_booking(4, 12, 12, 3);
    hb_make_booking(5, 13, 12, 1);
    hb_make_booking(6, 18, 13, 2);
    hb_make_booking(7, 19, 13, 3);
    hb_make_booking(8, 2, 14, 2);
    hb_make_booking(9, 8, 14, 1);
    hb_make_booking(10, 14, 15, 3);
    hb_make_booking(11, 20, 15, 2);
    hb_make_booking(0, 3, 16, 2);
    hb_make_booking(1, 9, 16, 1);
    hb_make_booking(2, 15, 17, 3);
    hb_make_booking(3, 21, 17, 2);
    hb_make_booking(4, 4, 18, 1);
    hb_make_booking(5, 10, 18, 2);

    print_str("\nChecking in...\n");
    for (int i = 0; i < 10; i++) {
        hb_checkin(i);
    }

    print_str("\nOrdering services...\n");
    hb_order_service(0, 0, 2);
    hb_order_service(1, 1, 1);
    hb_order_service(2, 2, 3);
    hb_order_service(3, 3, 1);
    hb_order_service(4, 4, 2);
    hb_order_service(5, 5, 1);
    hb_order_service(0, 6, 1);
    hb_order_service(1, 7, 2);
    hb_order_service(2, 8, 1);
    hb_order_service(3, 9, 2);
    hb_order_service(4, 0, 1);
    hb_order_service(5, 1, 1);

    print_str("\nChecking out...\n");
    for (int i = 0; i < 10; i++) {
        hb_checkout(i);
    }

    print_str("\nRoom status...\n");
    hb_room_status();

    print_str("\nOccupancy report...\n");
    hb_occupancy_report();

    print_str("\nFinancial report...\n");
    hb_financial_report();

    print_str("\nFinal state...\n");
    hb_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
