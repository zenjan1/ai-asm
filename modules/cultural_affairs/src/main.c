/* cultural_affairs: Cultural affairs management (v1.0)
 * Venues, heritage, events, arts funding, exchanges
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

#define MAX_VENUES     12
#define MAX_HERITAGE   14
#define MAX_EVENTS     16
#define MAX_GRANTS     10
#define MAX_EXCHANGES  8

typedef struct {
    int    venue_id;
    int    type;
    int    zone;
    int    capacity;
    int    visitors;
    int    exhibits;
    int    staff;
    int    revenue;
    int    active;
} venue_t;

typedef struct {
    int    heritage_id;
    int    type;
    int    level;
    int    zone;
    int    year_listed;
    int    condition;
    int    funding;
    int    active;
} heritage_t;

typedef struct {
    int    event_id;
    int    type;
    int    venue_id;
    int    attendees;
    int    duration_days;
    int    cost;
    int    revenue;
    int    year;
    int    active;
} event_t;

typedef struct {
    int    grant_id;
    int    artist_id;
    int    type;
    int    amount;
    int    project_year;
    int    completed;
    int    active;
} grant_t;

typedef struct {
    int    exchange_id;
    int    type;
    int    partner_country;
    int    n_artists;
    int    duration_days;
    int    cost;
    int    year;
    int    active;
} exchange_t;

typedef struct {
    int    n_venues;
    int    n_heritage;
    int    n_events;
    int    n_grants;
    int    n_exchanges;
    int    total_visitors;
    int    total_events;
    int    total_attendees;
    int    total_grants;
    int    total_revenue;
    int    total_cost;
} ca_state_t;

static venue_t venues[MAX_VENUES];
static heritage_t heritage[MAX_HERITAGE];
static event_t events[MAX_EVENTS];
static grant_t grants[MAX_GRANTS];
static exchange_t exchanges[MAX_EXCHANGES];
static ca_state_t ca;

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

int ca_init(void) {
    if (initialized) return -1;
    ca.n_venues = 0; ca.n_heritage = 0; ca.n_events = 0;
    ca.n_grants = 0; ca.n_exchanges = 0;
    ca.total_visitors = 0; ca.total_events = 0;
    ca.total_attendees = 0; ca.total_grants = 0;
    ca.total_revenue = 0; ca.total_cost = 0;
    for (int i = 0; i < MAX_VENUES; i++) venues[i].active = 0;
    for (int i = 0; i < MAX_HERITAGE; i++) heritage[i].active = 0;
    for (int i = 0; i < MAX_EVENTS; i++) events[i].active = 0;
    for (int i = 0; i < MAX_GRANTS; i++) grants[i].active = 0;
    for (int i = 0; i < MAX_EXCHANGES; i++) exchanges[i].active = 0;
    initialized = 1;
    print_str("[CA] Cultural affairs initialized\n");
    return 0;
}

int ca_add_venue(int type, int zone, int capacity, int staff) {
    if (ca.n_venues >= MAX_VENUES) return -1;
    venue_t* v = &venues[ca.n_venues];
    v->venue_id = ca.n_venues;
    v->type = type;
    v->zone = zone;
    v->capacity = capacity;
    v->visitors = 0;
    v->exhibits = 0;
    v->staff = staff;
    v->revenue = 0;
    v->active = 1;
    ca.n_venues++;
    print_str("[CA] Venue "); print_int(ca.n_venues - 1);
    print_str(" type="); print_int(type);
    print_str(" zone="); print_int(zone);
    print_str(" cap="); print_int(capacity);
    print_str(" staff="); print_int(staff); print_str("\n");
    return ca.n_venues - 1;
}

int ca_record_visitors(int venue_id, int visitors, int revenue) {
    if (venue_id >= ca.n_venues) return -1;
    venues[venue_id].visitors += visitors;
    venues[venue_id].revenue += revenue;
    ca.total_visitors += visitors;
    ca.total_revenue += revenue;
    print_str("[CA] Visit V"); print_int(venue_id);
    print_str(" +"); print_int(visitors);
    print_str(" $"); print_int(revenue); print_str("\n");
    return 0;
}

int ca_add_exhibit(int venue_id) {
    if (venue_id >= ca.n_venues) return -1;
    venues[venue_id].exhibits++;
    print_str("[CA] Exhibit V"); print_int(venue_id); print_str("\n");
    return 0;
}

int ca_list_heritage(int type, int level, int zone, int year, int condition, int funding) {
    if (ca.n_heritage >= MAX_HERITAGE) return -1;
    heritage_t* h = &heritage[ca.n_heritage];
    h->heritage_id = ca.n_heritage;
    h->type = type;
    h->level = level;
    h->zone = zone;
    h->year_listed = year;
    h->condition = condition;
    h->funding = funding;
    h->active = 1;
    ca.total_cost += funding;
    ca.n_heritage++;
    print_str("[CA] Heritage "); print_int(ca.n_heritage - 1);
    print_str(" type="); print_int(type);
    print_str(" lvl="); print_int(level);
    print_str(" zone="); print_int(zone);
    print_str(" yr="); print_int(year);
    print_str(" fund=$"); print_int(funding); print_str("\n");
    return ca.n_heritage - 1;
}

int ca_hold_event(int type, int venue, int attendees, int duration, int cost, int revenue, int year) {
    if (ca.n_events >= MAX_EVENTS || venue >= ca.n_venues) return -1;
    event_t* e = &events[ca.n_events];
    e->event_id = ca.n_events;
    e->type = type;
    e->venue_id = venue;
    e->attendees = attendees;
    e->duration_days = duration;
    e->cost = cost;
    e->revenue = revenue;
    e->year = year;
    e->active = 1;
    ca.total_events++;
    ca.total_attendees += attendees;
    ca.total_revenue += revenue;
    ca.total_cost += cost;
    ca.n_events++;
    print_str("[CA] Event "); print_int(ca.n_events - 1);
    print_str(" type="); print_int(type);
    print_str(" V"); print_int(venue);
    print_str(" att="); print_int(attendees);
    print_str(" days="); print_int(duration);
    print_str(" $"); print_int(revenue); print_str("\n");
    return ca.n_events - 1;
}

int ca_grant_funding(int artist, int type, int amount, int year) {
    if (ca.n_grants >= MAX_GRANTS) return -1;
    grant_t* g = &grants[ca.n_grants];
    g->grant_id = ca.n_grants;
    g->artist_id = artist;
    g->type = type;
    g->amount = amount;
    g->project_year = year;
    g->completed = 0;
    g->active = 1;
    ca.total_grants += amount;
    ca.total_cost += amount;
    ca.n_grants++;
    print_str("[CA] Grant "); print_int(ca.n_grants - 1);
    print_str(" artist="); print_int(artist);
    print_str(" type="); print_int(type);
    print_str(" $"); print_int(amount);
    print_str(" "); print_int(year); print_str("\n");
    return ca.n_grants - 1;
}

int ca_complete_grant(int grant_id) {
    if (grant_id >= ca.n_grants) return -1;
    grants[grant_id].completed = 1;
    print_str("[CA] Complete G"); print_int(grant_id); print_str("\n");
    return 0;
}

int ca_exchange(int type, int country, int n_artists, int duration, int cost, int year) {
    if (ca.n_exchanges >= MAX_EXCHANGES) return -1;
    exchange_t* ex = &exchanges[ca.n_exchanges];
    ex->exchange_id = ca.n_exchanges;
    ex->type = type;
    ex->partner_country = country;
    ex->n_artists = n_artists;
    ex->duration_days = duration;
    ex->cost = cost;
    ex->year = year;
    ex->active = 1;
    ca.total_cost += cost;
    ca.n_exchanges++;
    print_str("[CA] Exchange "); print_int(ca.n_exchanges - 1);
    print_str(" type="); print_int(type);
    print_str(" cty="); print_int(country);
    print_str(" artists="); print_int(n_artists);
    print_str(" days="); print_int(duration);
    print_str(" $"); print_int(cost); print_str("\n");
    return ca.n_exchanges - 1;
}

void ca_venue_report(void) {
    print_str("[CA] Venues:\n");
    print_str("  Total visitors: "); print_int(ca.total_visitors); print_str("\n");
    for (int i = 0; i < ca.n_venues; i++) {
        venue_t* v = &venues[i];
        print_str("  V"); print_int(i);
        print_str(" type="); print_int(v->type);
        print_str(" visitors="); print_int(v->visitors);
        print_str("/"); print_int(v->capacity);
        print_str(" exh="); print_int(v->exhibits);
        print_str(" rev=$"); print_int(v->revenue); print_str("\n");
    }
}

void ca_heritage_report(void) {
    print_str("[CA] Heritage sites: "); print_int(ca.n_heritage); print_str("\n");
    for (int i = 0; i < ca.n_heritage; i++) {
        heritage_t* h = &heritage[i];
        print_str("  H"); print_int(i);
        print_str(" type="); print_int(h->type);
        print_str(" lvl="); print_int(h->level);
        print_str(" cond="); print_int(h->condition); print_str("\n");
    }
}

void ca_financial_report(void) {
    print_str("[CA] Financial:\n");
    print_str("  Events: "); print_int(ca.total_events); print_str("\n");
    print_str("  Attendees: "); print_int(ca.total_attendees); print_str("\n");
    print_str("  Grants: "); print_int(ca.total_grants); print_str("\n");
    print_str("  Revenue: "); print_int(ca.total_revenue); print_str("\n");
    print_str("  Cost: "); print_int(ca.total_cost); print_str("\n");
}

void ca_print_state(void) {
    print_str("[CA] Venue="); print_int(ca.n_venues);
    print_str(" Herit="); print_int(ca.n_heritage);
    print_str(" Event="); print_int(ca.n_events);
    print_str(" Grant="); print_int(ca.n_grants);
    print_str(" Exch="); print_int(ca.n_exchanges);
    print_str("\n");
}

int main(void) {
    print_str("=== Cultural Affairs Demo ===\n\n");
    ca_init();

    print_str("Adding venues...\n");
    ca_add_venue(1, 1, 1000, 20);
    ca_add_venue(1, 2, 800, 15);
    ca_add_venue(2, 1, 500, 12);
    ca_add_venue(2, 3, 600, 14);
    ca_add_venue(3, 2, 400, 10);
    ca_add_venue(3, 4, 350, 8);
    ca_add_venue(4, 1, 1200, 25);
    ca_add_venue(4, 3, 900, 18);
    ca_add_venue(1, 4, 700, 16);
    ca_add_venue(2, 2, 550, 11);
    ca_add_venue(3, 3, 450, 9);
    ca_add_venue(4, 4, 850, 17);

    print_str("\nRecording visitors...\n");
    for (int i = 0; i < 24; i++) {
        int venue = i % 12;
        int visitors = 100 + (i * 20);
        int revenue = visitors * 10;
        ca_record_visitors(venue, visitors, revenue);
    }

    print_str("\nAdding exhibits...\n");
    for (int i = 0; i < 12; i++) {
        ca_add_exhibit(i);
        ca_add_exhibit(i);
    }

    print_str("\nListing heritage...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 3) + 1;
        int level = (i % 4) + 1;
        int zone = (i % 4) + 1;
        int year = 2000 + (i % 25);
        int condition = 3 + (i % 3);
        int funding = 50000 + (i * 20000);
        ca_list_heritage(type, level, zone, year, condition, funding);
    }

    print_str("\nHolding events...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 4) + 1;
        int venue = i % 12;
        int attendees = 200 + (i * 50);
        int duration = 1 + (i % 7);
        int cost = 10000 + (i * 3000);
        int revenue = 20000 + (i * 5000);
        int year = 2024;
        ca_hold_event(type, venue, attendees, duration, cost, revenue, year);
    }

    print_str("\nGranting funding...\n");
    for (int i = 0; i < 10; i++) {
        int artist = 500 + i;
        int type = (i % 3) + 1;
        int amount = 20000 + (i * 10000);
        int year = 2024;
        ca_grant_funding(artist, type, amount, year);
    }

    print_str("\nCompleting grants...\n");
    for (int i = 0; i < 10; i++) {
        ca_complete_grant(i);
    }

    print_str("\nCultural exchanges...\n");
    for (int i = 0; i < 8; i++) {
        int type = (i % 3) + 1;
        int country = (i % 6) + 1;
        int n_artists = 3 + (i % 5);
        int duration = 7 + (i * 3);
        int cost = 30000 + (i * 10000);
        int year = 2024;
        ca_exchange(type, country, n_artists, duration, cost, year);
    }

    print_str("\nVenue report...\n");
    ca_venue_report();

    print_str("\nHeritage report...\n");
    ca_heritage_report();

    print_str("\nFinancial report...\n");
    ca_financial_report();

    print_str("\nFinal state...\n");
    ca_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
