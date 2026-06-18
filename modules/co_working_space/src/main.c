/* co_working_space: Shared office management (v1.0)
 * Spaces, members, bookings, services, events, analytics
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

#define MAX_SPACES    16
#define MAX_MEMBERS   14
#define MAX_BOOKINGS  18
#define MAX_EVENTS    10
#define MAX_SERVICES  8

typedef struct {
    int    space_id;
    int    type;
    int    capacity;
    double hourly_rate;
    int    status;
    int    n_bookings;
    double revenue;
    int    active;
} space_t;

typedef struct {
    int    member_id;
    int    plan;
    int    join_day;
    double monthly_fee;
    int    n_bookings;
    double total_paid;
    int    active;
} member_t;

typedef struct {
    int    booking_id;
    int    member_id;
    int    space_id;
    int    day;
    int    start_hr;
    int    duration;
    double fee;
    int    status;
    int    active;
} booking_t;

typedef struct {
    int    event_id;
    int    type;
    int    space_id;
    int    day;
    int    capacity;
    int    n_attendees;
    double fee;
    double revenue;
    int    active;
} event_t;

typedef struct {
    int    service_id;
    int    type;
    double price;
    int    n_used;
    double revenue;
    int    active;
} service_t;

typedef struct {
    int    n_spaces;
    int    n_members;
    int    n_bookings;
    int    n_events;
    int    n_services;
    double space_revenue;
    double event_revenue;
    double service_revenue;
    double membership_revenue;
    double total_revenue;
} cw_state_t;

static space_t spaces[MAX_SPACES];
static member_t members[MAX_MEMBERS];
static booking_t bookings[MAX_BOOKINGS];
static event_t events[MAX_EVENTS];
static service_t services[MAX_SERVICES];
static cw_state_t cw;

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

int cw_init(void) {
    if (initialized) return -1;
    cw.n_spaces = 0; cw.n_members = 0; cw.n_bookings = 0;
    cw.n_events = 0; cw.n_services = 0;
    cw.space_revenue = 0.0; cw.event_revenue = 0.0;
    cw.service_revenue = 0.0; cw.membership_revenue = 0.0;
    cw.total_revenue = 0.0;
    for (int i = 0; i < MAX_SPACES; i++) spaces[i].active = 0;
    for (int i = 0; i < MAX_MEMBERS; i++) members[i].active = 0;
    for (int i = 0; i < MAX_BOOKINGS; i++) bookings[i].active = 0;
    for (int i = 0; i < MAX_EVENTS; i++) events[i].active = 0;
    for (int i = 0; i < MAX_SERVICES; i++) services[i].active = 0;
    initialized = 1;
    print_str("[CW] Co-working space initialized\n");
    return 0;
}

int cw_add_space(int type, int capacity, double rate) {
    if (cw.n_spaces >= MAX_SPACES) return -1;
    space_t* s = &spaces[cw.n_spaces];
    s->space_id = cw.n_spaces;
    s->type = type;
    s->capacity = capacity;
    s->hourly_rate = rate;
    s->status = 1;
    s->n_bookings = 0;
    s->revenue = 0.0;
    s->active = 1;
    cw.n_spaces++;
    print_str("[CW] Space "); print_int(cw.n_spaces - 1);
    print_str(" type="); print_int(type);
    print_str(" cap="); print_int(capacity);
    print_str(" rate="); print_int((int)rate); print_str("/hr\n");
    return cw.n_spaces - 1;
}

int cw_register_member(int plan, int join_day) {
    if (cw.n_members >= MAX_MEMBERS) return -1;
    member_t* m = &members[cw.n_members];
    m->member_id = cw.n_members;
    m->plan = plan;
    m->join_day = join_day;
    double fees[4] = {15.0, 199.0, 499.0, 899.0};
    m->monthly_fee = fees[plan < 4 ? plan : 0];
    m->n_bookings = 0;
    m->total_paid = m->monthly_fee;
    m->active = 1;
    cw.membership_revenue += m->monthly_fee;
    cw.total_revenue += m->monthly_fee;
    cw.n_members++;
    print_str("[CW] Member "); print_int(cw.n_members - 1);
    print_str(" plan="); print_int(plan);
    print_str(" fee="); print_int((int)m->monthly_fee); print_str("/mo\n");
    return cw.n_members - 1;
}

int cw_add_service(int type, double price) {
    if (cw.n_services >= MAX_SERVICES) return -1;
    service_t* s = &services[cw.n_services];
    s->service_id = cw.n_services;
    s->type = type;
    s->price = price;
    s->n_used = 0;
    s->revenue = 0.0;
    s->active = 1;
    cw.n_services++;
    print_str("[CW] Service "); print_int(cw.n_services - 1);
    print_str(" type="); print_int(type);
    print_str(" price="); print_int((int)price); print_str("\n");
    return cw.n_services - 1;
}

int cw_book_space(int member_id, int space_id, int day, int start_hr, int duration) {
    if (cw.n_bookings >= MAX_BOOKINGS) return -1;
    if (member_id >= cw.n_members || space_id >= cw.n_spaces) return -2;
    space_t* s = &spaces[space_id];
    if (s->status != 1) return -3;
    booking_t* b = &bookings[cw.n_bookings];
    b->booking_id = cw.n_bookings;
    b->member_id = member_id;
    b->space_id = space_id;
    b->day = day;
    b->start_hr = start_hr;
    b->duration = duration;
    double plan_discount[4] = {1.0, 0.9, 0.75, 0.5};
    b->fee = s->hourly_rate * duration * plan_discount[members[member_id].plan < 4 ? members[member_id].plan : 0];
    b->status = 1;
    b->active = 1;
    s->n_bookings++;
    s->revenue += b->fee;
    members[member_id].n_bookings++;
    members[member_id].total_paid += b->fee;
    cw.space_revenue += b->fee;
    cw.total_revenue += b->fee;
    cw.n_bookings++;
    print_str("[CW] Booking "); print_int(cw.n_bookings - 1);
    print_str(" M"); print_int(member_id);
    print_str(" S"); print_int(space_id);
    print_str(" day="); print_int(day);
    print_str(" hr="); print_int(start_hr);
    print_str(" dur="); print_int(duration);
    print_str(" fee="); print_int((int)b->fee); print_str("\n");
    return cw.n_bookings - 1;
}

int cw_use_service(int service_id, int member_id) {
    if (service_id >= cw.n_services || member_id >= cw.n_members) return -1;
    service_t* s = &services[service_id];
    s->n_used++;
    s->revenue += s->price;
    members[member_id].total_paid += s->price;
    cw.service_revenue += s->price;
    cw.total_revenue += s->price;
    print_str("[CW] Service S"); print_int(service_id);
    print_str(" used by M"); print_int(member_id);
    print_str(" fee="); print_int((int)s->price); print_str("\n");
    return 0;
}

int cw_create_event(int type, int space_id, int day, int capacity, double fee) {
    if (cw.n_events >= MAX_EVENTS || space_id >= cw.n_spaces) return -1;
    event_t* e = &events[cw.n_events];
    e->event_id = cw.n_events;
    e->type = type;
    e->space_id = space_id;
    e->day = day;
    e->capacity = capacity;
    e->n_attendees = 0;
    e->fee = fee;
    e->revenue = 0.0;
    e->active = 1;
    cw.n_events++;
    print_str("[CW] Event "); print_int(cw.n_events - 1);
    print_str(" type="); print_int(type);
    print_str(" S"); print_int(space_id);
    print_str(" day="); print_int(day);
    print_str(" cap="); print_int(capacity);
    print_str(" fee="); print_int((int)fee); print_str("\n");
    return cw.n_events - 1;
}

int cw_attend_event(int event_id, int n_people) {
    if (event_id >= cw.n_events) return -1;
    event_t* e = &events[event_id];
    int can_attend = e->capacity - e->n_attendees;
    if (n_people > can_attend) n_people = can_attend;
    e->n_attendees += n_people;
    double revenue = e->fee * n_people;
    e->revenue += revenue;
    cw.event_revenue += revenue;
    cw.total_revenue += revenue;
    print_str("[CW] Event E"); print_int(event_id);
    print_str(" +"); print_int(n_people); print_str(" attendees");
    print_str(" revenue="); print_int((int)revenue); print_str("\n");
    return 0;
}

void cw_space_utilization(void) {
    print_str("[CW] Space utilization:\n");
    for (int i = 0; i < cw.n_spaces; i++) {
        space_t* s = &spaces[i];
        print_str("  S"); print_int(i);
        print_str(" type="); print_int(s->type);
        print_str(" cap="); print_int(s->capacity);
        print_str(" bookings="); print_int(s->n_bookings);
        print_str(" rev="); print_int((int)s->revenue); print_str("\n");
    }
}

void cw_financial_report(void) {
    print_str("[CW] Financial report:\n");
    print_str("  Membership revenue: "); print_int((int)cw.membership_revenue); print_str("\n");
    print_str("  Space revenue: "); print_int((int)cw.space_revenue); print_str("\n");
    print_str("  Event revenue: "); print_int((int)cw.event_revenue); print_str("\n");
    print_str("  Service revenue: "); print_int((int)cw.service_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)cw.total_revenue); print_str("\n");
    print_str("  Members: "); print_int(cw.n_members); print_str("\n");
}

void cw_print_state(void) {
    print_str("[CW] Spaces="); print_int(cw.n_spaces);
    print_str(" Members="); print_int(cw.n_members);
    print_str(" Bookings="); print_int(cw.n_bookings);
    print_str(" Events="); print_int(cw.n_events);
    print_str(" Services="); print_int(cw.n_services);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)cw.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Co-Working Space Demo ===\n\n");
    cw_init();

    print_str("Adding spaces...\n");
    cw_add_space(1, 1, 8.0);
    cw_add_space(1, 1, 8.0);
    cw_add_space(1, 1, 8.0);
    cw_add_space(1, 1, 8.0);
    cw_add_space(2, 1, 15.0);
    cw_add_space(2, 1, 15.0);
    cw_add_space(2, 1, 15.0);
    cw_add_space(3, 6, 45.0);
    cw_add_space(3, 8, 55.0);
    cw_add_space(3, 10, 65.0);
    cw_add_space(4, 1, 5.0);
    cw_add_space(4, 1, 5.0);
    cw_add_space(4, 2, 8.0);
    cw_add_space(4, 2, 8.0);
    cw_add_space(1, 1, 8.0);
    cw_add_space(2, 1, 15.0);

    print_str("\nRegistering members...\n");
    for (int i = 0; i < 12; i++) {
        cw_register_member(i % 4, (i % 3) + 1);
    }

    print_str("\nAdding services...\n");
    cw_add_service(1, 2.0);
    cw_add_service(2, 5.0);
    cw_add_service(3, 10.0);
    cw_add_service(4, 3.0);
    cw_add_service(1, 2.0);
    cw_add_service(2, 5.0);
    cw_add_service(3, 10.0);
    cw_add_service(4, 3.0);

    print_str("\nBooking spaces...\n");
    cw_book_space(0, 0, 10, 9, 4);
    cw_book_space(1, 4, 10, 10, 3);
    cw_book_space(2, 7, 10, 14, 2);
    cw_book_space(3, 1, 11, 9, 5);
    cw_book_space(4, 5, 11, 11, 2);
    cw_book_space(5, 8, 11, 14, 3);
    cw_book_space(6, 2, 12, 9, 4);
    cw_book_space(7, 9, 12, 10, 2);
    cw_book_space(8, 3, 13, 9, 3);
    cw_book_space(9, 6, 13, 11, 4);
    cw_book_space(10, 10, 13, 14, 2);
    cw_book_space(11, 12, 14, 9, 5);
    cw_book_space(0, 13, 14, 10, 3);
    cw_book_space(1, 14, 14, 14, 2);
    cw_book_space(2, 15, 15, 9, 4);
    cw_book_space(3, 0, 15, 11, 3);
    cw_book_space(4, 4, 15, 14, 2);
    cw_book_space(5, 7, 16, 10, 3);

    print_str("\nUsing services...\n");
    cw_use_service(0, 0);
    cw_use_service(1, 1);
    cw_use_service(2, 2);
    cw_use_service(3, 3);
    cw_use_service(4, 4);
    cw_use_service(5, 5);
    cw_use_service(6, 6);
    cw_use_service(7, 7);
    cw_use_service(0, 8);
    cw_use_service(1, 9);
    cw_use_service(2, 10);
    cw_use_service(3, 11);

    print_str("\nCreating events...\n");
    cw_create_event(1, 7, 20, 30, 25.0);
    cw_create_event(2, 8, 22, 50, 15.0);
    cw_create_event(3, 9, 25, 40, 35.0);
    cw_create_event(1, 7, 27, 25, 20.0);
    cw_create_event(2, 8, 30, 60, 10.0);

    print_str("\nEvent attendance...\n");
    cw_attend_event(0, 20);
    cw_attend_event(1, 35);
    cw_attend_event(2, 30);
    cw_attend_event(3, 18);
    cw_attend_event(4, 45);
    cw_attend_event(0, 8);
    cw_attend_event(1, 12);

    print_str("\nSpace utilization...\n");
    cw_space_utilization();

    print_str("\nFinancial report...\n");
    cw_financial_report();

    print_str("\nFinal state...\n");
    cw_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
