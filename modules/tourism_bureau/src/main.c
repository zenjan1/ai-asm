/* tourism_bureau: Tourism management (v1.0)
 * Scenic areas, hotels, travel agencies, guides, statistics
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

#define MAX_ATTRACTIONS  12
#define MAX_HOTELS       14
#define MAX_AGENCIES     10
#define MAX_GUIDES       16
#define MAX_BOOKINGS     18

typedef struct {
    int    attr_id;
    int    type;
    int    zone;
    int    capacity;
    int    visitors;
    int    ticket_price;
    int    revenue;
    int    rating;
    int    active;
} attraction_t;

typedef struct {
    int    hotel_id;
    int    star;
    int    zone;
    int    rooms;
    int    occupied;
    int    price;
    int    revenue;
    int    rating;
    int    active;
} hotel_t;

typedef struct {
    int    agency_id;
    int    type;
    int    zone;
    int    staff;
    int    n_tours;
    int    revenue;
    int    rating;
    int    active;
} agency_t;

typedef struct {
    int    guide_id;
    int    language;
    int    level;
    int    tours_done;
    int    rating;
    int    complaints;
    int    trained_hours;
    int    active;
} guide_t;

typedef struct {
    int    booking_id;
    int    hotel_id;
    int    guest_id;
    int    rooms;
    int    nights;
    int    price;
    int    day;
    int    status;
    int    active;
} booking_t;

typedef struct {
    int    n_attractions;
    int    n_hotels;
    int    n_agencies;
    int    n_guides;
    int    n_bookings;
    int    total_visitors;
    int    total_revenue;
    int    total_bookings;
    int    total_tours;
    int    total_complaints;
} tb_state_t;

static attraction_t attractions[MAX_ATTRACTIONS];
static hotel_t hotels[MAX_HOTELS];
static agency_t agencies[MAX_AGENCIES];
static guide_t guides[MAX_GUIDES];
static booking_t bookings[MAX_BOOKINGS];
static tb_state_t tb;

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

int tb_init(void) {
    if (initialized) return -1;
    tb.n_attractions = 0; tb.n_hotels = 0; tb.n_agencies = 0;
    tb.n_guides = 0; tb.n_bookings = 0;
    tb.total_visitors = 0; tb.total_revenue = 0;
    tb.total_bookings = 0; tb.total_tours = 0; tb.total_complaints = 0;
    for (int i = 0; i < MAX_ATTRACTIONS; i++) attractions[i].active = 0;
    for (int i = 0; i < MAX_HOTELS; i++) hotels[i].active = 0;
    for (int i = 0; i < MAX_AGENCIES; i++) agencies[i].active = 0;
    for (int i = 0; i < MAX_GUIDES; i++) guides[i].active = 0;
    for (int i = 0; i < MAX_BOOKINGS; i++) bookings[i].active = 0;
    initialized = 1;
    print_str("[TB] Tourism bureau initialized\n");
    return 0;
}

int tb_add_attraction(int type, int zone, int capacity, int price, int rating) {
    if (tb.n_attractions >= MAX_ATTRACTIONS) return -1;
    attraction_t* a = &attractions[tb.n_attractions];
    a->attr_id = tb.n_attractions;
    a->type = type;
    a->zone = zone;
    a->capacity = capacity;
    a->visitors = 0;
    a->ticket_price = price;
    a->revenue = 0;
    a->rating = rating;
    a->active = 1;
    tb.n_attractions++;
    print_str("[TB] Attract "); print_int(tb.n_attractions - 1);
    print_str(" type="); print_int(type);
    print_str(" zone="); print_int(zone);
    print_str(" cap="); print_int(capacity);
    print_str(" $"); print_int(price); print_str("\n");
    return tb.n_attractions - 1;
}

int tb_record_visitors(int attr_id, int visitors) {
    if (attr_id >= tb.n_attractions) return -1;
    attractions[attr_id].visitors += visitors;
    int revenue = visitors * attractions[attr_id].ticket_price;
    attractions[attr_id].revenue += revenue;
    tb.total_visitors += visitors;
    tb.total_revenue += revenue;
    print_str("[TB] Visit A"); print_int(attr_id);
    print_str(" +"); print_int(visitors);
    print_str(" rev=$"); print_int(revenue); print_str("\n");
    return 0;
}

int tb_add_hotel(int star, int zone, int rooms, int price, int rating) {
    if (tb.n_hotels >= MAX_HOTELS) return -1;
    hotel_t* h = &hotels[tb.n_hotels];
    h->hotel_id = tb.n_hotels;
    h->star = star;
    h->zone = zone;
    h->rooms = rooms;
    h->occupied = 0;
    h->price = price;
    h->revenue = 0;
    h->rating = rating;
    h->active = 1;
    tb.n_hotels++;
    print_str("[TB] Hotel "); print_int(tb.n_hotels - 1);
    print_str(" star="); print_int(star);
    print_str(" zone="); print_int(zone);
    print_str(" rooms="); print_int(rooms);
    print_str(" $"); print_int(price); print_str("\n");
    return tb.n_hotels - 1;
}

int tb_add_agency(int type, int zone, int staff) {
    if (tb.n_agencies >= MAX_AGENCIES) return -1;
    agency_t* ag = &agencies[tb.n_agencies];
    ag->agency_id = tb.n_agencies;
    ag->type = type;
    ag->zone = zone;
    ag->staff = staff;
    ag->n_tours = 0;
    ag->revenue = 0;
    ag->rating = 0;
    ag->active = 1;
    tb.n_agencies++;
    print_str("[TB] Agency "); print_int(tb.n_agencies - 1);
    print_str(" type="); print_int(type);
    print_str(" zone="); print_int(zone);
    print_str(" staff="); print_int(staff); print_str("\n");
    return tb.n_agencies - 1;
}

int tb_add_guide(int language, int level) {
    if (tb.n_guides >= MAX_GUIDES) return -1;
    guide_t* g = &guides[tb.n_guides];
    g->guide_id = tb.n_guides;
    g->language = language;
    g->level = level;
    g->tours_done = 0;
    g->rating = 5;
    g->complaints = 0;
    g->trained_hours = 0;
    g->active = 1;
    tb.n_guides++;
    print_str("[TB] Guide "); print_int(tb.n_guides - 1);
    print_str(" lang="); print_int(language);
    print_str(" lvl="); print_int(level); print_str("\n");
    return tb.n_guides - 1;
}

int tb_train_guide(int guide_id, int hours) {
    if (guide_id >= tb.n_guides) return -1;
    guides[guide_id].trained_hours += hours;
    print_str("[TB] Train G"); print_int(guide_id);
    print_str(" +"); print_int(hours); print_str("hrs\n");
    return 0;
}

int tb_book_hotel(int hotel, int guest, int rooms, int nights, int day) {
    if (tb.n_bookings >= MAX_BOOKINGS || hotel >= tb.n_hotels) return -1;
    booking_t* b = &bookings[tb.n_bookings];
    b->booking_id = tb.n_bookings;
    b->hotel_id = hotel;
    b->guest_id = guest;
    b->rooms = rooms;
    b->nights = nights;
    b->price = rooms * nights * hotels[hotel].price;
    b->day = day;
    b->status = 1;
    b->active = 1;
    hotels[hotel].occupied += rooms;
    hotels[hotel].revenue += b->price;
    tb.total_bookings++;
    tb.total_revenue += b->price;
    tb.n_bookings++;
    print_str("[TB] Book "); print_int(tb.n_bookings - 1);
    print_str(" H"); print_int(hotel);
    print_str(" G"); print_int(guest);
    print_str(" rms="); print_int(rooms);
    print_str(" nights="); print_int(nights);
    print_str(" $"); print_int(b->price); print_str("\n");
    return tb.n_bookings - 1;
}

int tb_checkout(int booking_id) {
    if (booking_id >= tb.n_bookings) return -1;
    int hotel = bookings[booking_id].hotel_id;
    hotels[hotel].occupied -= bookings[booking_id].rooms;
    bookings[booking_id].status = 2;
    print_str("[TB] Checkout B"); print_int(booking_id); print_str("\n");
    return 0;
}

int tb_complaint(int guide_id) {
    if (guide_id >= tb.n_guides) return -1;
    guides[guide_id].complaints++;
    tb.total_complaints++;
    print_str("[TB] Complaint G"); print_int(guide_id); print_str("\n");
    return 0;
}

void tb_attraction_report(void) {
    print_str("[TB] Attractions:\n");
    print_str("  Total visitors: "); print_int(tb.total_visitors); print_str("\n");
    for (int i = 0; i < tb.n_attractions; i++) {
        attraction_t* a = &attractions[i];
        print_str("  A"); print_int(i);
        print_str(" type="); print_int(a->type);
        print_str(" visitors="); print_int(a->visitors);
        print_str("/"); print_int(a->capacity);
        print_str(" rev=$"); print_int(a->revenue); print_str("\n");
    }
}

void tb_hotel_report(void) {
    print_str("[TB] Hotels:\n");
    for (int i = 0; i < tb.n_hotels; i++) {
        hotel_t* h = &hotels[i];
        print_str("  H"); print_int(i);
        print_str(" star="); print_int(h->star);
        print_str(" occ="); print_int(h->occupied);
        print_str("/"); print_int(h->rooms);
        print_str(" rev=$"); print_int(h->revenue); print_str("\n");
    }
}

void tb_revenue_report(void) {
    print_str("[TB] Revenue: "); print_int(tb.total_revenue); print_str("\n");
    print_str("  Bookings: "); print_int(tb.total_bookings); print_str("\n");
    print_str("  Complaints: "); print_int(tb.total_complaints); print_str("\n");
}

void tb_print_state(void) {
    print_str("[TB] Attr="); print_int(tb.n_attractions);
    print_str(" Hotel="); print_int(tb.n_hotels);
    print_str(" Agency="); print_int(tb.n_agencies);
    print_str(" Guide="); print_int(tb.n_guides);
    print_str(" Book="); print_int(tb.n_bookings);
    print_str("\n");
}

int main(void) {
    print_str("=== Tourism Bureau Demo ===\n\n");
    tb_init();

    print_str("Adding attractions...\n");
    tb_add_attraction(1, 1, 5000, 100, 4);
    tb_add_attraction(1, 2, 3000, 80, 5);
    tb_add_attraction(2, 1, 2000, 120, 4);
    tb_add_attraction(2, 3, 4000, 60, 3);
    tb_add_attraction(3, 2, 1500, 150, 5);
    tb_add_attraction(3, 4, 2500, 90, 4);
    tb_add_attraction(1, 3, 3500, 70, 4);
    tb_add_attraction(2, 4, 1800, 110, 3);
    tb_add_attraction(3, 1, 2200, 130, 5);
    tb_add_attraction(1, 4, 2800, 85, 4);
    tb_add_attraction(2, 2, 3200, 95, 4);
    tb_add_attraction(3, 3, 1700, 140, 5);

    print_str("\nRecording visitors...\n");
    for (int i = 0; i < 24; i++) {
        int attr = i % 12;
        int visitors = 200 + (i * 50);
        tb_record_visitors(attr, visitors);
    }

    print_str("\nAdding hotels...\n");
    for (int i = 0; i < 14; i++) {
        int star = 1 + (i % 5);
        int zone = (i % 4) + 1;
        int rooms = 50 + (i * 10);
        int price = 200 + (i * 50);
        int rating = 3 + (i % 3);
        tb_add_hotel(star, zone, rooms, price, rating);
    }

    print_str("\nAdding agencies...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 3) + 1;
        int zone = (i % 4) + 1;
        int staff = 5 + (i % 10);
        tb_add_agency(type, zone, staff);
    }

    print_str("\nAdding guides...\n");
    for (int i = 0; i < 16; i++) {
        int language = (i % 5) + 1;
        int level = (i % 4) + 1;
        tb_add_guide(language, level);
    }

    print_str("\nTraining guides...\n");
    for (int i = 0; i < 16; i++) {
        tb_train_guide(i, 20 + (i * 5));
    }

    print_str("\nBooking hotels...\n");
    for (int i = 0; i < 18; i++) {
        int hotel = i % 14;
        int guest = 1000 + i;
        int rooms = 1 + (i % 3);
        int nights = 1 + (i % 5);
        int day = 1 + (i % 28);
        tb_book_hotel(hotel, guest, rooms, nights, day);
    }

    print_str("\nChecking out...\n");
    for (int i = 0; i < 12; i++) {
        tb_checkout(i);
    }

    print_str("\nFiling complaints...\n");
    tb_complaint(2);
    tb_complaint(5);
    tb_complaint(8);
    tb_complaint(11);

    print_str("\nAttraction report...\n");
    tb_attraction_report();

    print_str("\nHotel report...\n");
    tb_hotel_report();

    print_str("\nRevenue report...\n");
    tb_revenue_report();

    print_str("\nFinal state...\n");
    tb_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
