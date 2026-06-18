/* exhibition_convention: Exhibition and convention administration system (v1.0)
 * Exhibition management, convention, venue, event, exhibitor service
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

#define MAX_EXHIBITION   16
#define MAX_CONVENTION   14
#define MAX_VENUE        12
#define MAX_EVENT        10
#define MAX_EXHIBITOR    10

typedef struct {
    int    exhibition_id;
    int    exhibition_type;
    int    exhibition_scale;
    int    exhibitions_held;
    int    booths_occupied;
    int    visitors_attracted;
    int    year;
    int    active;
} exhibition_t;

typedef struct {
    int    convention_id;
    int    convention_type;
    int    attendee_category;
    int    conventions_organized;
    int    attendees_hosted;
    int    sessions_delivered;
    int    year;
    int    active;
} convention_t;

typedef struct {
    int    venue_id;
    int    venue_type;
    int    venue_capacity;
    int    venues_operated;
    int    bookings_made;
    int    utilization_rate;
    int    year;
    int    active;
} venue_t;

typedef struct {
    int    event_id;
    int    event_type;
    int    event_scale;
    int    events_planned;
    int    events_executed;
    int    satisfaction_rate;
    int    year;
    int    active;
} event_t;

typedef struct {
    int    exhibitor_id;
    int    exhibitor_type;
    int    industry_sector;
    int    exhibitors_registered;
    int    booths_allocated;
    int    services_provided;
    int    year;
    int    active;
} exhibitor_t;

typedef struct {
    int    n_exhibition;
    int    n_convention;
    int    n_venue;
    int    n_event;
    int    n_exhibitor;
    int    total_booths;
    int    total_visitors;
    int    total_bookings;
    int    total_events;
    int    total_exhibitors;
} ec_state_t;

static exhibition_t exhibitions[MAX_EXHIBITION];
static convention_t conventions[MAX_CONVENTION];
static venue_t venues[MAX_VENUE];
static event_t events[MAX_EVENT];
static exhibitor_t exhibitors[MAX_EXHIBITOR];
static ec_state_t ec;

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

int ec_init(void) {
    if (initialized) return -1;
    ec.n_exhibition = 0; ec.n_convention = 0; ec.n_venue = 0;
    ec.n_event = 0; ec.n_exhibitor = 0;
    ec.total_booths = 0; ec.total_visitors = 0;
    ec.total_bookings = 0; ec.total_events = 0;
    ec.total_exhibitors = 0;
    for (int i = 0; i < MAX_EXHIBITION; i++) exhibitions[i].active = 0;
    for (int i = 0; i < MAX_CONVENTION; i++) conventions[i].active = 0;
    for (int i = 0; i < MAX_VENUE; i++) venues[i].active = 0;
    for (int i = 0; i < MAX_EVENT; i++) events[i].active = 0;
    for (int i = 0; i < MAX_EXHIBITOR; i++) exhibitors[i].active = 0;
    initialized = 1;
    print_str("[EC] Exhibition convention initialized\n");
    return 0;
}

int ec_exhibition(int exh_type, int scale, int held, int booths, int visitors, int year) {
    if (ec.n_exhibition >= MAX_EXHIBITION) return -1;
    exhibition_t* e = &exhibitions[ec.n_exhibition];
    e->exhibition_id = ec.n_exhibition;
    e->exhibition_type = exh_type;
    e->exhibition_scale = scale;
    e->exhibitions_held = held;
    e->booths_occupied = booths;
    e->visitors_attracted = visitors;
    e->year = year;
    e->active = 1;
    ec.total_booths += booths;
    ec.total_visitors += visitors;
    ec.n_exhibition++;
    print_str("[EC] Exhibition "); print_int(ec.n_exhibition - 1);
    print_str(" type="); print_int(exh_type);
    print_str(" scl="); print_int(scale);
    print_str(" hld="); print_int(held);
    print_str(" bth="); print_int(booths);
    print_str(" vst="); print_int(visitors); print_str("\n");
    return ec.n_exhibition - 1;
}

int ec_convention(int cnv_type, int attendee, int organized, int hosted, int sessions, int year) {
    if (ec.n_convention >= MAX_CONVENTION) return -1;
    convention_t* c = &conventions[ec.n_convention];
    c->convention_id = ec.n_convention;
    c->convention_type = cnv_type;
    c->attendee_category = attendee;
    c->conventions_organized = organized;
    c->attendees_hosted = hosted;
    c->sessions_delivered = sessions;
    c->year = year;
    c->active = 1;
    ec.n_convention++;
    print_str("[EC] Convention "); print_int(ec.n_convention - 1);
    print_str(" type="); print_int(cnv_type);
    print_str(" att="); print_int(attendee);
    print_str(" org="); print_int(organized);
    print_str(" hst="); print_int(hosted);
    print_str(" ssn="); print_int(sessions); print_str("\n");
    return ec.n_convention - 1;
}

int ec_venue(int vnu_type, int capacity, int operated, int bookings, int utilization, int year) {
    if (ec.n_venue >= MAX_VENUE) return -1;
    venue_t* v = &venues[ec.n_venue];
    v->venue_id = ec.n_venue;
    v->venue_type = vnu_type;
    v->venue_capacity = capacity;
    v->venues_operated = operated;
    v->bookings_made = bookings;
    v->utilization_rate = utilization;
    v->year = year;
    v->active = 1;
    ec.total_bookings += bookings;
    ec.n_venue++;
    print_str("[EC] Venue "); print_int(ec.n_venue - 1);
    print_str(" type="); print_int(vnu_type);
    print_str(" cap="); print_int(capacity);
    print_str(" opr="); print_int(operated);
    print_str(" bkg="); print_int(bookings);
    print_str(" utl="); print_int(utilization); print_str("\n");
    return ec.n_venue - 1;
}

int ec_event(int evt_type, int scale, int planned, int executed, int satisfaction, int year) {
    if (ec.n_event >= MAX_EVENT) return -1;
    event_t* ev = &events[ec.n_event];
    ev->event_id = ec.n_event;
    ev->event_type = evt_type;
    ev->event_scale = scale;
    ev->events_planned = planned;
    ev->events_executed = executed;
    ev->satisfaction_rate = satisfaction;
    ev->year = year;
    ev->active = 1;
    ec.total_events += executed;
    ec.n_event++;
    print_str("[EC] Event "); print_int(ec.n_event - 1);
    print_str(" type="); print_int(evt_type);
    print_str(" scl="); print_int(scale);
    print_str(" pln="); print_int(planned);
    print_str(" exc="); print_int(executed);
    print_str(" sat="); print_int(satisfaction); print_str("\n");
    return ec.n_event - 1;
}

int ec_exhibitor(int exh_type, int sector, int registered_cnt, int booths_alloc, int services, int year) {
    if (ec.n_exhibitor >= MAX_EXHIBITOR) return -1;
    exhibitor_t* ex = &exhibitors[ec.n_exhibitor];
    ex->exhibitor_id = ec.n_exhibitor;
    ex->exhibitor_type = exh_type;
    ex->industry_sector = sector;
    ex->exhibitors_registered = registered_cnt;
    ex->booths_allocated = booths_alloc;
    ex->services_provided = services;
    ex->year = year;
    ex->active = 1;
    ec.total_exhibitors += registered_cnt;
    ec.n_exhibitor++;
    print_str("[EC] Exhibitor "); print_int(ec.n_exhibitor - 1);
    print_str(" type="); print_int(exh_type);
    print_str(" sec="); print_int(sector);
    print_str(" reg="); print_int(registered_cnt);
    print_str(" bth="); print_int(booths_alloc);
    print_str(" svc="); print_int(services); print_str("\n");
    return ec.n_exhibitor - 1;
}

void ec_exhibition_report(void) {
    print_str("[EC] Exhibition report:\n");
    print_str("  Exhibition categories: "); print_int(ec.n_exhibition); print_str("\n");
    print_str("  Total booths: "); print_int(ec.total_booths); print_str("\n");
    print_str("  Total visitors: "); print_int(ec.total_visitors); print_str("\n");
}

void ec_venue_report(void) {
    print_str("[EC] Venue report:\n");
    print_str("  Convention categories: "); print_int(ec.n_convention); print_str("\n");
    print_str("  Venue categories: "); print_int(ec.n_venue); print_str("\n");
    print_str("  Total bookings: "); print_int(ec.total_bookings); print_str("\n");
}

void ec_exhibitor_report(void) {
    print_str("[EC] Exhibitor report:\n");
    print_str("  Event categories: "); print_int(ec.n_event); print_str("\n");
    print_str("  Total events executed: "); print_int(ec.total_events); print_str("\n");
    print_str("  Exhibitor categories: "); print_int(ec.n_exhibitor); print_str("\n");
    print_str("  Total exhibitors: "); print_int(ec.total_exhibitors); print_str("\n");
}

void ec_print_state(void) {
    print_str("[EC] Ex="); print_int(ec.n_exhibition);
    print_str(" Cn="); print_int(ec.n_convention);
    print_str(" Vn="); print_int(ec.n_venue);
    print_str(" Ev="); print_int(ec.n_event);
    print_str(" Eb="); print_int(ec.n_exhibitor);
    print_str("\n");
}

int main(void) {
    print_str("=== Exhibition Convention Demo ===\n\n");
    ec_init();

    print_str("Exhibition management...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int scl = (i % 4) + 1;
        int hld = 5 + (i * 2);
        int bth = 100 + (i * 30);
        int vst = 5000 + (i * 1500);
        int year = 2020 + (i % 5);
        ec_exhibition(type, scl, hld, bth, vst, year);
    }

    print_str("\nConvention management...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int att = (i % 5) + 1;
        int org = 8 + (i * 2);
        int hst = 500 + (i * 150);
        int ssn = 30 + (i * 8);
        int year = 2021 + (i % 4);
        ec_convention(type, att, org, hst, ssn, year);
    }

    print_str("\nVenue management...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cap = 1000 + (i * 300);
        int opr = 3 + (i % 4);
        int bkg = 50 + (i * 12);
        int utl = 70 + (i % 30);
        int year = 2022 + (i % 3);
        ec_venue(type, cap, opr, bkg, utl, year);
    }

    print_str("\nEvent management...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int scl = (i % 3) + 1;
        int pln = 20 + (i * 5);
        int exc = 18 + (i * 4);
        int sat = 85 + (i % 15);
        int year = 2023 + (i % 2);
        ec_event(type, scl, pln, exc, sat, year);
    }

    print_str("\nExhibitor service...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int sec = (i % 6) + 1;
        int reg = 50 + (i * 15);
        int bth = 80 + (i * 20);
        int svc = 30 + (i * 8);
        int year = 2024;
        ec_exhibitor(type, sec, reg, bth, svc, year);
    }

    print_str("\nExhibition report...\n");
    ec_exhibition_report();

    print_str("\nVenue report...\n");
    ec_venue_report();

    print_str("\nExhibitor report...\n");
    ec_exhibitor_report();

    print_str("\nFinal state...\n");
    ec_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
