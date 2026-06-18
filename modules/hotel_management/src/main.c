/* hotel_management: Hotel management system (v1.0)
 * Room management, reservations, check-in/out, guest services, revenue management
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

#define MAX_ROOMS       20
#define MAX_GUESTS      12
#define MAX_RESERVATIONS 14
#define MAX_SERVICES    10
#define MAX_REQUESTS    12

typedef struct {
    int    room_id;
    int    floor;
    int    type;
    double rate;
    int    capacity;
    int    status;
    int    clean_status;
    int    active;
} room_t;

typedef struct {
    int    guest_id;
    int    room_id;
    int    nights;
    int    checkin_day;
    int    checkout_day;
    double total_bill;
    double deposit;
    int    status;
    int    active;
} reservation_t;

typedef struct {
    int    checkin_id;
    int    guest_id;
    int    room_id;
    int    checkin_day;
    int    checkout_day;
    double room_charge;
    double extras;
    double total;
    int    status;
    int    active;
} checkin_t;

typedef struct {
    int    service_id;
    int    type;
    double price;
    int    n_ordered;
    double revenue;
    int    active;
} service_t;

typedef struct {
    int    request_id;
    int    guest_id;
    int    room_id;
    int    type;
    int    status;
    int    active;
} request_t;

typedef struct {
    int    n_rooms;
    int    n_guests;
    int    n_reservations;
    int    n_checkins;
    int    n_services;
    int    n_requests;
    double total_revenue;
    double room_revenue;
    double extras_revenue;
    int    rooms_occupied;
    int    rooms_available;
    int    reservations_pending;
} hm_state_t;

static room_t rooms[MAX_ROOMS];
static reservation_t reservations[MAX_RESERVATIONS];
static checkin_t checkins[MAX_RESERVATIONS];
static service_t services[MAX_SERVICES];
static request_t requests[MAX_REQUESTS];
static hm_state_t hm;

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

int hm_init(void) {
    if (initialized) return -1;
    hm.n_rooms = 0; hm.n_guests = 0; hm.n_reservations = 0;
    hm.n_checkins = 0; hm.n_services = 0; hm.n_requests = 0;
    hm.total_revenue = 0.0; hm.room_revenue = 0.0; hm.extras_revenue = 0.0;
    hm.rooms_occupied = 0; hm.rooms_available = 0; hm.reservations_pending = 0;
    for (int i = 0; i < MAX_ROOMS; i++) rooms[i].active = 0;
    for (int i = 0; i < MAX_RESERVATIONS; i++) reservations[i].active = 0;
    for (int i = 0; i < MAX_RESERVATIONS; i++) checkins[i].active = 0;
    for (int i = 0; i < MAX_SERVICES; i++) services[i].active = 0;
    for (int i = 0; i < MAX_REQUESTS; i++) requests[i].active = 0;
    initialized = 1;
    print_str("[HM] Hotel management initialized\n");
    return 0;
}

int hm_add_room(int floor, int type, double rate, int capacity) {
    if (hm.n_rooms >= MAX_ROOMS) return -1;
    room_t* r = &rooms[hm.n_rooms];
    r->room_id = hm.n_rooms;
    r->floor = floor;
    r->type = type;
    r->rate = rate;
    r->capacity = capacity;
    r->status = 0;
    r->clean_status = 1;
    r->active = 1;
    hm.n_rooms++;
    hm.rooms_available++;
    print_str("[HM] Room "); print_int(hm.n_rooms - 1);
    print_str(" F"); print_int(floor);
    print_str(" type="); print_int(type);
    print_str(" rate="); print_int((int)rate);
    print_str(" cap="); print_int(capacity); print_str("\n");
    return hm.n_rooms - 1;
}

int hm_add_service(int type, double price) {
    if (hm.n_services >= MAX_SERVICES) return -1;
    service_t* s = &services[hm.n_services];
    s->service_id = hm.n_services;
    s->type = type;
    s->price = price;
    s->n_ordered = 0;
    s->revenue = 0.0;
    s->active = 1;
    hm.n_services++;
    print_str("[HM] Service "); print_int(hm.n_services - 1);
    print_str(" type="); print_int(type);
    print_str(" price="); print_int((int)price); print_str("\n");
    return hm.n_services - 1;
}

int hm_make_reservation(int room_id, int nights, int checkin_day) {
    if (hm.n_reservations >= MAX_RESERVATIONS || room_id >= hm.n_rooms) return -1;
    if (rooms[room_id].status != 0) return -2;
    reservation_t* r = &reservations[hm.n_reservations];
    r->guest_id = hm.n_guests++;
    r->room_id = room_id;
    r->nights = nights;
    r->checkin_day = checkin_day;
    r->checkout_day = checkin_day + nights;
    r->total_bill = rooms[room_id].rate * nights;
    r->deposit = r->total_bill * 0.3;
    r->status = 1;
    r->active = 1;
    hm.n_reservations++;
    hm.reservations_pending++;
    rooms[room_id].status = 1;
    hm.rooms_available--;
    print_str("[HM] Reservation "); print_int(hm.n_reservations - 1);
    print_str(" R"); print_int(room_id);
    print_str(" nights="); print_int(nights);
    print_str(" day "); print_int(checkin_day); print_str("-"); print_int(r->checkout_day);
    print_str(" bill="); print_int((int)r->total_bill);
    print_str(" dep="); print_int((int)r->deposit); print_str("\n");
    return hm.n_reservations - 1;
}

int hm_checkin(int reservation_id, int actual_day) {
    if (hm.n_checkins >= MAX_RESERVATIONS || reservation_id >= hm.n_reservations) return -1;
    reservation_t* r = &reservations[reservation_id];
    if (r->status != 1) return -2;
    checkin_t* c = &checkins[hm.n_checkins];
    c->checkin_id = hm.n_checkins;
    c->guest_id = r->guest_id;
    c->room_id = r->room_id;
    c->checkin_day = actual_day;
    c->checkout_day = r->checkout_day;
    c->room_charge = rooms[r->room_id].rate * r->nights;
    c->extras = 0.0;
    c->total = c->room_charge;
    c->status = 1;
    c->active = 1;
    hm.n_checkins++;
    hm.reservations_pending--;
    r->status = 2;
    rooms[r->room_id].status = 2;
    rooms[r->room_id].clean_status = 0;
    hm.rooms_occupied++;
    print_str("[HM] Check-in Res"); print_int(reservation_id);
    print_str(" G"); print_int(c->guest_id);
    print_str(" R"); print_int(c->room_id);
    print_str(" day "); print_int(actual_day); print_str("\n");
    return hm.n_checkins - 1;
}

int hm_order_service(int checkin_id, int service_id) {
    if (checkin_id >= hm.n_checkins || service_id >= hm.n_services) return -1;
    checkin_t* c = &checkins[checkin_id];
    service_t* s = &services[service_id];
    c->extras += s->price;
    c->total += s->price;
    s->n_ordered++;
    s->revenue += s->price;
    hm.extras_revenue += s->price;
    print_str("[HM] Service S"); print_int(service_id);
    print_str(" ordered by C"); print_int(checkin_id);
    print_str(" price="); print_int((int)s->price); print_str("\n");
    return 0;
}

int hm_submit_request(int guest_id, int room_id, int type) {
    if (hm.n_requests >= MAX_REQUESTS) return -1;
    request_t* r = &requests[hm.n_requests];
    r->request_id = hm.n_requests;
    r->guest_id = guest_id;
    r->room_id = room_id;
    r->type = type;
    r->status = 1;
    r->active = 1;
    hm.n_requests++;
    print_str("[HM] Request "); print_int(hm.n_requests - 1);
    print_str(" G"); print_int(guest_id);
    print_str(" R"); print_int(room_id);
    print_str(" type="); print_int(type); print_str("\n");
    return hm.n_requests - 1;
}

void hm_fulfill_request(int request_id) {
    if (request_id >= hm.n_requests) return;
    requests[request_id].status = 2;
    print_str("[HM] Request "); print_int(request_id); print_str(" fulfilled\n");
}

void hm_checkout(int checkin_id) {
    if (checkin_id >= hm.n_checkins) return;
    checkin_t* c = &checkins[checkin_id];
    c->status = 2;
    rooms[c->room_id].status = 3;
    rooms[c->room_id].clean_status = 0;
    hm.rooms_occupied--;
    hm.rooms_available++;
    hm.total_revenue += c->total;
    hm.room_revenue += c->room_charge;
    print_str("[HM] Check-out C"); print_int(checkin_id);
    print_str(" G"); print_int(c->guest_id);
    print_str(" R"); print_int(c->room_id);
    print_str(" room="); print_int((int)c->room_charge);
    print_str(" extras="); print_int((int)c->extras);
    print_str(" total="); print_int((int)c->total); print_str("\n");
}

void hm_clean_room(int room_id) {
    if (room_id >= hm.n_rooms) return;
    rooms[room_id].clean_status = 1;
    if (rooms[room_id].status == 3) {
        rooms[room_id].status = 0;
    }
    print_str("[HM] Room "); print_int(room_id); print_str(" cleaned\n");
}

void hm_revenue_analysis(void) {
    print_str("[HM] Revenue analysis:\n");
    int occupied = 0, available = 0;
    double total_rate = 0.0;
    for (int i = 0; i < hm.n_rooms; i++) {
        if (rooms[i].status == 2) occupied++;
        else if (rooms[i].status == 0) available++;
        total_rate += rooms[i].rate;
    }
    double occ_rate = (hm.n_rooms > 0) ? (double)occupied / hm.n_rooms * 100.0 : 0.0;
    double revpar = hm.room_revenue / hm.n_rooms;
    double adr = (hm.n_checkins > 0) ? hm.room_revenue / hm.n_checkins : 0.0;
    print_str("  Total rooms: "); print_int(hm.n_rooms); print_str("\n");
    print_str("  Occupied: "); print_int(occupied); print_str("\n");
    print_str("  Available: "); print_int(available); print_str("\n");
    print_str("  Occupancy: "); print_int((int)occ_rate); print_str("%\n");
    print_str("  ADR: "); print_int((int)adr); print_str("\n");
    print_str("  RevPAR: "); print_int((int)revpar); print_str("\n");
    print_str("  Room revenue: "); print_int((int)hm.room_revenue); print_str("\n");
    print_str("  Extras revenue: "); print_int((int)hm.extras_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)hm.total_revenue); print_str("\n");
}

void hm_print_state(void) {
    print_str("[HM] Rooms="); print_int(hm.n_rooms);
    print_str(" Res="); print_int(hm.n_reservations);
    print_str(" Checkins="); print_int(hm.n_checkins);
    print_str(" Services="); print_int(hm.n_services);
    print_str(" Requests="); print_int(hm.n_requests);
    print_str("\n");
    print_str("  Occupied: "); print_int(hm.rooms_occupied); print_str("\n");
    print_str("  Available: "); print_int(hm.rooms_available); print_str("\n");
    print_str("  Pending res: "); print_int(hm.reservations_pending); print_str("\n");
    print_str("  Revenue: "); print_int((int)hm.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Hotel Management System Demo ===\n\n");
    hm_init();

    print_str("Adding rooms...\n");
    hm_add_room(1, 1, 150.0, 2);
    hm_add_room(1, 1, 160.0, 2);
    hm_add_room(1, 2, 200.0, 3);
    hm_add_room(2, 1, 170.0, 2);
    hm_add_room(2, 2, 220.0, 3);
    hm_add_room(2, 3, 350.0, 4);
    hm_add_room(3, 2, 240.0, 3);
    hm_add_room(3, 3, 400.0, 4);
    hm_add_room(3, 3, 450.0, 4);
    hm_add_room(4, 4, 800.0, 6);

    print_str("\nAdding services...\n");
    hm_add_service(1, 25.0);
    hm_add_service(2, 40.0);
    hm_add_service(3, 15.0);
    hm_add_service(4, 60.0);
    hm_add_service(5, 80.0);

    print_str("\nMaking reservations...\n");
    hm_make_reservation(0, 3, 10);
    hm_make_reservation(2, 2, 11);
    hm_make_reservation(5, 4, 12);
    hm_make_reservation(3, 1, 13);
    hm_make_reservation(7, 3, 14);
    hm_make_reservation(9, 5, 15);
    hm_make_reservation(1, 2, 16);

    print_str("\nCheck-ins...\n");
    hm_checkin(0, 10);
    hm_checkin(1, 11);
    hm_checkin(2, 12);
    hm_checkin(3, 13);
    hm_checkin(4, 14);

    print_str("\nOrdering services...\n");
    hm_order_service(0, 0);
    hm_order_service(0, 2);
    hm_order_service(1, 1);
    hm_order_service(2, 3);
    hm_order_service(2, 4);
    hm_order_service(3, 0);
    hm_order_service(4, 2);
    hm_order_service(4, 1);

    print_str("\nGuest requests...\n");
    hm_submit_request(0, 0, 1);
    hm_submit_request(1, 2, 2);
    hm_submit_request(2, 5, 1);
    hm_submit_request(3, 3, 3);
    hm_submit_request(4, 7, 2);

    print_str("\nFulfilling requests...\n");
    hm_fulfill_request(0);
    hm_fulfill_request(1);
    hm_fulfill_request(2);
    hm_fulfill_request(3);

    print_str("\nCheck-outs...\n");
    hm_checkout(0);
    hm_checkout(1);
    hm_checkout(3);

    print_str("\nCleaning rooms...\n");
    hm_clean_room(0);
    hm_clean_room(2);
    hm_clean_room(3);

    print_str("\nRevenue analysis...\n");
    hm_revenue_analysis();

    print_str("\nFinal state...\n");
    hm_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
