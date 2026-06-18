/* sports_authority: Sports management (v1.0)
 * Venues, events, athletes, associations, fitness
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
#define MAX_EVENTS     16
#define MAX_ATHLETES   18
#define MAX_ASSOC      10
#define MAX_FACILITIES 14

typedef struct {
    int    venue_id;
    int    type;
    int    zone;
    int    capacity;
    int    events_held;
    int    staff;
    int    revenue;
    int    active;
} venue_t;

typedef struct {
    int    event_id;
    int    type;
    int    venue_id;
    int    n_athletes;
    int    n_spectators;
    int    duration_days;
    int    revenue;
    int    year;
    int    active;
} event_t;

typedef struct {
    int    athlete_id;
    int    sport;
    int    level;
    int    age;
    int    n_competitions;
    int    medals;
    int    best_score;
    int    training_hours;
    int    active;
} athlete_t;

typedef struct {
    int    assoc_id;
    int    sport;
    int    n_members;
    int    n_events;
    int    revenue;
    int    year_established;
    int    active;
} association_t;

typedef struct {
    int    facility_id;
    int    type;
    int    zone;
    int    capacity;
    int    users;
    int    revenue;
    int    staff;
    int    active;
} facility_t;

typedef struct {
    int    n_venues;
    int    n_events;
    int    n_athletes;
    int    n_associations;
    int    n_facilities;
    int    total_events;
    int    total_athletes_registered;
    int    total_medals;
    int    total_spectators;
    int    total_revenue;
    int    total_training_hours;
} sa_state_t;

static venue_t venues[MAX_VENUES];
static event_t events[MAX_EVENTS];
static athlete_t athletes[MAX_ATHLETES];
static association_t associations[MAX_ASSOC];
static facility_t facilities[MAX_FACILITIES];
static sa_state_t sa;

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

int sa_init(void) {
    if (initialized) return -1;
    sa.n_venues = 0; sa.n_events = 0; sa.n_athletes = 0;
    sa.n_associations = 0; sa.n_facilities = 0;
    sa.total_events = 0; sa.total_athletes_registered = 0;
    sa.total_medals = 0; sa.total_spectators = 0;
    sa.total_revenue = 0; sa.total_training_hours = 0;
    for (int i = 0; i < MAX_VENUES; i++) venues[i].active = 0;
    for (int i = 0; i < MAX_EVENTS; i++) events[i].active = 0;
    for (int i = 0; i < MAX_ATHLETES; i++) athletes[i].active = 0;
    for (int i = 0; i < MAX_ASSOC; i++) associations[i].active = 0;
    for (int i = 0; i < MAX_FACILITIES; i++) facilities[i].active = 0;
    initialized = 1;
    print_str("[SA] Sports authority initialized\n");
    return 0;
}

int sa_add_venue(int type, int zone, int capacity, int staff) {
    if (sa.n_venues >= MAX_VENUES) return -1;
    venue_t* v = &venues[sa.n_venues];
    v->venue_id = sa.n_venues;
    v->type = type;
    v->zone = zone;
    v->capacity = capacity;
    v->events_held = 0;
    v->staff = staff;
    v->revenue = 0;
    v->active = 1;
    sa.n_venues++;
    print_str("[SA] Venue "); print_int(sa.n_venues - 1);
    print_str(" type="); print_int(type);
    print_str(" zone="); print_int(zone);
    print_str(" cap="); print_int(capacity);
    print_str(" staff="); print_int(staff); print_str("\n");
    return sa.n_venues - 1;
}

int sa_register_athlete(int sport, int level, int age) {
    if (sa.n_athletes >= MAX_ATHLETES) return -1;
    athlete_t* a = &athletes[sa.n_athletes];
    a->athlete_id = sa.n_athletes;
    a->sport = sport;
    a->level = level;
    a->age = age;
    a->n_competitions = 0;
    a->medals = 0;
    a->best_score = 0;
    a->training_hours = 0;
    a->active = 1;
    sa.total_athletes_registered++;
    sa.n_athletes++;
    print_str("[SA] Athlete "); print_int(sa.n_athletes - 1);
    print_str(" sport="); print_int(sport);
    print_str(" lvl="); print_int(level);
    print_str(" age="); print_int(age); print_str("\n");
    return sa.n_athletes - 1;
}

int sa_train(int athlete_id, int hours) {
    if (athlete_id >= sa.n_athletes) return -1;
    athletes[athlete_id].training_hours += hours;
    sa.total_training_hours += hours;
    print_str("[SA] Train A"); print_int(athlete_id);
    print_str(" +"); print_int(hours); print_str("hrs\n");
    return 0;
}

int sa_hold_event(int type, int venue, int n_athletes, int n_spectators, int duration, int revenue, int year) {
    if (sa.n_events >= MAX_EVENTS || venue >= sa.n_venues) return -1;
    event_t* e = &events[sa.n_events];
    e->event_id = sa.n_events;
    e->type = type;
    e->venue_id = venue;
    e->n_athletes = n_athletes;
    e->n_spectators = n_spectators;
    e->duration_days = duration;
    e->revenue = revenue;
    e->year = year;
    e->active = 1;
    venues[venue].events_held++;
    venues[venue].revenue += revenue;
    sa.total_events++;
    sa.total_spectators += n_spectators;
    sa.total_revenue += revenue;
    sa.n_events++;
    print_str("[SA] Event "); print_int(sa.n_events - 1);
    print_str(" type="); print_int(type);
    print_str(" V"); print_int(venue);
    print_str(" ath="); print_int(n_athletes);
    print_str(" spec="); print_int(n_spectators);
    print_str(" days="); print_int(duration);
    print_str(" $"); print_int(revenue); print_str("\n");
    return sa.n_events - 1;
}

int sa_compete(int athlete_id, int score, int medal) {
    if (athlete_id >= sa.n_athletes) return -1;
    athletes[athlete_id].n_competitions++;
    if (score > athletes[athlete_id].best_score) {
        athletes[athlete_id].best_score = score;
    }
    if (medal > 0) {
        athletes[athlete_id].medals += medal;
        sa.total_medals += medal;
    }
    print_str("[SA] Compete A"); print_int(athlete_id);
    print_str(" score="); print_int(score);
    print_str(" medal="); print_int(medal); print_str("\n");
    return 0;
}

int sa_add_association(int sport, int n_members, int year) {
    if (sa.n_associations >= MAX_ASSOC) return -1;
    association_t* a = &associations[sa.n_associations];
    a->assoc_id = sa.n_associations;
    a->sport = sport;
    a->n_members = n_members;
    a->n_events = 0;
    a->revenue = 0;
    a->year_established = year;
    a->active = 1;
    sa.n_associations++;
    print_str("[SA] Assoc "); print_int(sa.n_associations - 1);
    print_str(" sport="); print_int(sport);
    print_str(" mem="); print_int(n_members);
    print_str(" yr="); print_int(year); print_str("\n");
    return sa.n_associations - 1;
}

int sa_add_facility(int type, int zone, int capacity, int staff) {
    if (sa.n_facilities >= MAX_FACILITIES) return -1;
    facility_t* f = &facilities[sa.n_facilities];
    f->facility_id = sa.n_facilities;
    f->type = type;
    f->zone = zone;
    f->capacity = capacity;
    f->users = 0;
    f->revenue = 0;
    f->staff = staff;
    f->active = 1;
    sa.n_facilities++;
    print_str("[SA] Facility "); print_int(sa.n_facilities - 1);
    print_str(" type="); print_int(type);
    print_str(" zone="); print_int(zone);
    print_str(" cap="); print_int(capacity);
    print_str(" staff="); print_int(staff); print_str("\n");
    return sa.n_facilities - 1;
}

int sa_record_users(int facility_id, int users, int revenue) {
    if (facility_id >= sa.n_facilities) return -1;
    facilities[facility_id].users += users;
    facilities[facility_id].revenue += revenue;
    sa.total_revenue += revenue;
    print_str("[SA] Users F"); print_int(facility_id);
    print_str(" +"); print_int(users);
    print_str(" $"); print_int(revenue); print_str("\n");
    return 0;
}

void sa_venue_report(void) {
    print_str("[SA] Venues:\n");
    for (int i = 0; i < sa.n_venues; i++) {
        venue_t* v = &venues[i];
        print_str("  V"); print_int(i);
        print_str(" type="); print_int(v->type);
        print_str(" events="); print_int(v->events_held);
        print_str("/"); print_int(v->capacity);
        print_str(" rev=$"); print_int(v->revenue); print_str("\n");
    }
}

void sa_athlete_report(void) {
    print_str("[SA] Athletes: "); print_int(sa.n_athletes); print_str("\n");
    print_str("  Total medals: "); print_int(sa.total_medals); print_str("\n");
    print_str("  Total training hrs: "); print_int(sa.total_training_hours); print_str("\n");
}

void sa_event_report(void) {
    print_str("[SA] Events: "); print_int(sa.total_events); print_str("\n");
    print_str("  Total spectators: "); print_int(sa.total_spectators); print_str("\n");
    print_str("  Total revenue: "); print_int(sa.total_revenue); print_str("\n");
}

void sa_print_state(void) {
    print_str("[SA] Venue="); print_int(sa.n_venues);
    print_str(" Event="); print_int(sa.n_events);
    print_str(" Ath="); print_int(sa.n_athletes);
    print_str(" Assoc="); print_int(sa.n_associations);
    print_str(" Fac="); print_int(sa.n_facilities);
    print_str("\n");
}

int main(void) {
    print_str("=== Sports Authority Demo ===\n\n");
    sa_init();

    print_str("Adding venues...\n");
    sa_add_venue(1, 1, 20000, 50);
    sa_add_venue(1, 2, 15000, 40);
    sa_add_venue(2, 1, 5000, 25);
    sa_add_venue(2, 3, 6000, 28);
    sa_add_venue(3, 2, 3000, 20);
    sa_add_venue(3, 4, 2500, 18);
    sa_add_venue(4, 1, 8000, 35);
    sa_add_venue(4, 3, 7000, 30);
    sa_add_venue(1, 4, 12000, 45);
    sa_add_venue(2, 2, 4000, 22);
    sa_add_venue(3, 3, 3500, 19);
    sa_add_venue(4, 4, 5500, 27);

    print_str("\nRegistering athletes...\n");
    for (int i = 0; i < 18; i++) {
        int sport = (i % 8) + 1;
        int level = (i % 4) + 1;
        int age = 18 + (i % 15);
        sa_register_athlete(sport, level, age);
    }

    print_str("\nTraining athletes...\n");
    for (int i = 0; i < 18; i++) {
        int hours = 20 + (i * 10);
        sa_train(i, hours);
    }

    print_str("\nHolding events...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 4) + 1;
        int venue = i % 12;
        int n_athletes = 50 + (i * 10);
        int n_spectators = 1000 + (i * 500);
        int duration = 1 + (i % 5);
        int revenue = 20000 + (i * 10000);
        int year = 2024;
        sa_hold_event(type, venue, n_athletes, n_spectators, duration, revenue, year);
    }

    print_str("\nCompetitions...\n");
    for (int i = 0; i < 36; i++) {
        int athlete = i % 18;
        int score = 50 + (i * 5);
        int medal = (i % 3);
        sa_compete(athlete, score, medal);
    }

    print_str("\nAdding associations...\n");
    for (int i = 0; i < 10; i++) {
        int sport = i + 1;
        int n_members = 100 + (i * 50);
        int year = 1990 + (i * 3);
        sa_add_association(sport, n_members, year);
    }

    print_str("\nAdding fitness facilities...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int zone = (i % 4) + 1;
        int capacity = 100 + (i * 30);
        int staff = 3 + (i % 5);
        sa_add_facility(type, zone, capacity, staff);
    }

    print_str("\nRecording facility usage...\n");
    for (int i = 0; i < 14; i++) {
        int users = 50 + (i * 20);
        int revenue = users * 10;
        sa_record_users(i, users, revenue);
    }

    print_str("\nVenue report...\n");
    sa_venue_report();

    print_str("\nAthlete report...\n");
    sa_athlete_report();

    print_str("\nEvent report...\n");
    sa_event_report();

    print_str("\nFinal state...\n");
    sa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
