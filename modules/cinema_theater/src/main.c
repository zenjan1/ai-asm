/* cinema_theater: Cinema theater management (v1.0)
 * Screens, movies, showtimes, tickets, concessions, members
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

#define MAX_SCREENS    6
#define MAX_MOVIES     8
#define MAX_SHOWTIMES  14
#define MAX_TICKETS    20
#define MAX_MEMBERS    12

typedef struct {
    int    screen_id;
    int    type;
    int    capacity;
    int    n_shows;
    int    total_attendance;
    double revenue;
    int    active;
} screen_t;

typedef struct {
    int    movie_id;
    int    genre;
    int    duration_min;
    int    rating;
    const char* name;
    int    n_shows;
    double total_revenue;
    int    active;
} movie_t;

typedef struct {
    int    showtime_id;
    int    screen_id;
    int    movie_id;
    int    day;
    int    hour;
    int    is_matinee;
    int    seats_sold;
    int    capacity;
    double revenue;
    int    active;
} showtime_t;

typedef struct {
    int    ticket_id;
    int    showtime_id;
    int    type;
    double price;
    int    qty;
    double total;
    int    member_id;
    int    active;
} ticket_t;

typedef struct {
    int    member_id;
    int    tier;
    int    n_visits;
    int    points;
    double total_spent;
    int    active;
} member_t;

typedef struct {
    int    n_screens;
    int    n_movies;
    int    n_showtimes;
    int    n_tickets;
    int    n_members;
    double ticket_revenue;
    double concession_revenue;
    double total_revenue;
    int    total_attendance;
    int    total_shows;
} ct_state_t;

static screen_t screens[MAX_SCREENS];
static movie_t movies[MAX_MOVIES];
static showtime_t showtimes[MAX_SHOWTIMES];
static ticket_t tickets[MAX_TICKETS];
static member_t members[MAX_MEMBERS];
static ct_state_t ct;

static int initialized = 0;
static double concession_rev = 0.0;

static void print_str(const char* s) { host_print(s); }
static void print_int(int val) {
    char buf[32]; int i = 0;
    if (val < 0) { buf[i++] = '-'; val = -val; }
    if (val == 0) { buf[i++] = '0'; }
    else { int s = i; while (val > 0) { buf[i++] = '0' + (val % 10); val /= 10; }
           int e = i - 1; while (s < e) { char t = buf[s]; buf[s] = buf[e]; buf[e] = t; s++; e--; } }
    buf[i] = '\0'; host_print(buf);
}

int ct_init(void) {
    if (initialized) return -1;
    ct.n_screens = 0; ct.n_movies = 0; ct.n_showtimes = 0;
    ct.n_tickets = 0; ct.n_members = 0;
    ct.ticket_revenue = 0.0; ct.concession_revenue = 0.0;
    ct.total_revenue = 0.0; ct.total_attendance = 0;
    ct.total_shows = 0;
    concession_rev = 0.0;
    for (int i = 0; i < MAX_SCREENS; i++) screens[i].active = 0;
    for (int i = 0; i < MAX_MOVIES; i++) movies[i].active = 0;
    for (int i = 0; i < MAX_SHOWTIMES; i++) showtimes[i].active = 0;
    for (int i = 0; i < MAX_TICKETS; i++) tickets[i].active = 0;
    for (int i = 0; i < MAX_MEMBERS; i++) members[i].active = 0;
    initialized = 1;
    print_str("[CT] Cinema theater initialized\n");
    return 0;
}

int ct_add_screen(int type, int capacity) {
    if (ct.n_screens >= MAX_SCREENS) return -1;
    screen_t* s = &screens[ct.n_screens];
    s->screen_id = ct.n_screens;
    s->type = type;
    s->capacity = capacity;
    s->n_shows = 0;
    s->total_attendance = 0;
    s->revenue = 0.0;
    s->active = 1;
    ct.n_screens++;
    print_str("[CT] Screen "); print_int(ct.n_screens - 1);
    print_str(" type="); print_int(type);
    print_str(" cap="); print_int(capacity); print_str("\n");
    return ct.n_screens - 1;
}

int ct_add_movie(int genre, int duration, int rating) {
    if (ct.n_movies >= MAX_MOVIES) return -1;
    movie_t* m = &movies[ct.n_movies];
    m->movie_id = ct.n_movies;
    m->genre = genre;
    m->duration_min = duration;
    m->rating = rating;
    m->n_shows = 0;
    m->total_revenue = 0.0;
    m->active = 1;
    ct.n_movies++;
    print_str("[CT] Movie "); print_int(ct.n_movies - 1);
    print_str(" genre="); print_int(genre);
    print_str(" dur="); print_int(duration); print_str("min");
    print_str(" rating="); print_int(rating); print_str("\n");
    return ct.n_movies - 1;
}

int ct_add_showtime(int screen_id, int movie_id, int day, int hour) {
    if (ct.n_showtimes >= MAX_SHOWTIMES) return -1;
    if (screen_id >= ct.n_screens || movie_id >= ct.n_movies) return -2;
    showtime_t* sh = &showtimes[ct.n_showtimes];
    sh->showtime_id = ct.n_showtimes;
    sh->screen_id = screen_id;
    sh->movie_id = movie_id;
    sh->day = day;
    sh->hour = hour;
    sh->is_matinee = (hour < 17) ? 1 : 0;
    sh->seats_sold = 0;
    sh->capacity = screens[screen_id].capacity;
    sh->revenue = 0.0;
    sh->active = 1;
    screens[screen_id].n_shows++;
    movies[movie_id].n_shows++;
    ct.total_shows++;
    ct.n_showtimes++;
    print_str("[CT] Showtime "); print_int(ct.n_showtimes - 1);
    print_str(" Sc"); print_int(screen_id);
    print_str(" M"); print_int(movie_id);
    print_str(" day="); print_int(day);
    print_str(" hr="); print_int(hour);
    if (sh->is_matinee) print_str(" [MATINEE]");
    print_str("\n");
    return ct.n_showtimes - 1;
}

int ct_register_member(int tier) {
    if (ct.n_members >= MAX_MEMBERS) return -1;
    member_t* m = &members[ct.n_members];
    m->member_id = ct.n_members;
    m->tier = tier;
    m->n_visits = 0;
    m->points = 0;
    m->total_spent = 0.0;
    m->active = 1;
    ct.n_members++;
    print_str("[CT] Member "); print_int(ct.n_members - 1);
    print_str(" tier="); print_int(tier); print_str("\n");
    return ct.n_members - 1;
}

int ct_sell_ticket(int showtime_id, int type, int qty, int member_id) {
    if (ct.n_tickets >= MAX_TICKETS || showtime_id >= ct.n_showtimes) return -1;
    showtime_t* sh = &showtimes[showtime_id];
    double base_prices[3] = {12.0, 8.0, 9.0};
    double price = base_prices[type < 3 ? type : 0];
    if (sh->is_matinee) price *= 0.75;
    if (member_id >= 0 && member_id < ct.n_members) {
        double tier_disc[4] = {1.0, 0.95, 0.90, 0.85};
        price *= tier_disc[members[member_id].tier < 4 ? members[member_id].tier : 0];
    }
    screen_t* sc = &screens[sh->screen_id];
    if (sh->seats_sold + qty > sh->capacity) {
        qty = sh->capacity - sh->seats_sold;
        if (qty <= 0) return -2;
    }
    ticket_t* t = &tickets[ct.n_tickets];
    t->ticket_id = ct.n_tickets;
    t->showtime_id = showtime_id;
    t->type = type;
    t->price = price;
    t->qty = qty;
    t->total = price * qty;
    t->member_id = member_id;
    t->active = 1;
    sh->seats_sold += qty;
    sh->revenue += t->total;
    sc->total_attendance += qty;
    sc->revenue += t->total;
    movies[sh->movie_id].total_revenue += t->total;
    if (member_id >= 0 && member_id < ct.n_members) {
        members[member_id].n_visits++;
        members[member_id].points += qty * 10;
        members[member_id].total_spent += t->total;
    }
    ct.ticket_revenue += t->total;
    ct.total_attendance += qty;
    double conc = qty * 6.0;
    ct.concession_revenue += conc;
    concession_rev += conc;
    ct.total_revenue += t->total + conc;
    ct.n_tickets++;
    print_str("[CT] Ticket "); print_int(ct.n_tickets - 1);
    print_str(" Sh"); print_int(showtime_id);
    print_str(" type="); print_int(type);
    print_str(" qty="); print_int(qty);
    print_str(" total="); print_int((int)t->total); print_str("\n");
    return ct.n_tickets - 1;
}

void ct_screen_report(void) {
    print_str("[CT] Screen report:\n");
    for (int i = 0; i < ct.n_screens; i++) {
        screen_t* s = &screens[i];
        print_str("  Sc"); print_int(i);
        print_str(" type="); print_int(s->type);
        print_str(" cap="); print_int(s->capacity);
        print_str(" shows="); print_int(s->n_shows);
        print_str(" attend="); print_int(s->total_attendance);
        print_str(" rev="); print_int((int)s->revenue); print_str("\n");
    }
}

void ct_movie_report(void) {
    print_str("[CT] Movie report:\n");
    for (int i = 0; i < ct.n_movies; i++) {
        movie_t* m = &movies[i];
        print_str("  M"); print_int(i);
        print_str(" genre="); print_int(m->genre);
        print_str(" shows="); print_int(m->n_shows);
        print_str(" rev="); print_int((int)m->total_revenue); print_str("\n");
    }
}

void ct_financial_report(void) {
    print_str("[CT] Financial report:\n");
    print_str("  Ticket revenue: "); print_int((int)ct.ticket_revenue); print_str("\n");
    print_str("  Concession revenue: "); print_int((int)ct.concession_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)ct.total_revenue); print_str("\n");
    print_str("  Total attendance: "); print_int(ct.total_attendance); print_str("\n");
    print_str("  Total shows: "); print_int(ct.total_shows); print_str("\n");
    print_str("  Members: "); print_int(ct.n_members); print_str("\n");
}

void ct_print_state(void) {
    print_str("[CT] Screens="); print_int(ct.n_screens);
    print_str(" Movies="); print_int(ct.n_movies);
    print_str(" Shows="); print_int(ct.n_showtimes);
    print_str(" Tickets="); print_int(ct.n_tickets);
    print_str(" Members="); print_int(ct.n_members);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)ct.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Cinema Theater Demo ===\n\n");
    ct_init();

    print_str("Adding screens...\n");
    ct_add_screen(1, 120);
    ct_add_screen(1, 150);
    ct_add_screen(2, 200);
    ct_add_screen(2, 250);
    ct_add_screen(3, 80);
    ct_add_screen(4, 60);

    print_str("\nAdding movies...\n");
    ct_add_movie(1, 120, 85);
    ct_add_movie(1, 135, 90);
    ct_add_movie(2, 110, 78);
    ct_add_movie(2, 105, 82);
    ct_add_movie(3, 95, 88);
    ct_add_movie(3, 100, 75);
    ct_add_movie(4, 90, 92);
    ct_add_movie(4, 115, 80);

    print_str("\nCreating showtimes...\n");
    ct_add_showtime(0, 0, 10, 11);
    ct_add_showtime(0, 1, 10, 14);
    ct_add_showtime(0, 2, 10, 19);
    ct_add_showtime(1, 3, 10, 12);
    ct_add_showtime(1, 4, 10, 15);
    ct_add_showtime(1, 5, 10, 20);
    ct_add_showtime(2, 6, 11, 11);
    ct_add_showtime(2, 7, 11, 14);
    ct_add_showtime(2, 0, 11, 18);
    ct_add_showtime(3, 1, 11, 13);
    ct_add_showtime(3, 2, 11, 16);
    ct_add_showtime(4, 3, 12, 10);
    ct_add_showtime(4, 4, 12, 14);
    ct_add_showtime(5, 5, 12, 19);

    print_str("\nRegistering members...\n");
    for (int i = 0; i < 12; i++) {
        ct_register_member(i % 4);
    }

    print_str("\nSelling tickets...\n");
    ct_sell_ticket(0, 0, 4, 0);
    ct_sell_ticket(1, 0, 2, 1);
    ct_sell_ticket(2, 0, 6, 2);
    ct_sell_ticket(3, 1, 3, 3);
    ct_sell_ticket(4, 0, 5, 4);
    ct_sell_ticket(5, 2, 2, 5);
    ct_sell_ticket(6, 0, 8, 6);
    ct_sell_ticket(7, 0, 3, 7);
    ct_sell_ticket(8, 1, 4, 8);
    ct_sell_ticket(9, 0, 5, 9);
    ct_sell_ticket(10, 2, 2, 10);
    ct_sell_ticket(11, 0, 3, 11);
    ct_sell_ticket(12, 1, 4, 0);
    ct_sell_ticket(13, 0, 2, 1);
    ct_sell_ticket(0, 0, 3, 2);
    ct_sell_ticket(1, 0, 5, 3);
    ct_sell_ticket(2, 1, 2, 4);
    ct_sell_ticket(3, 0, 4, 5);
    ct_sell_ticket(4, 2, 3, 6);
    ct_sell_ticket(5, 0, 2, 7);

    print_str("\nScreen report...\n");
    ct_screen_report();

    print_str("\nMovie report...\n");
    ct_movie_report();

    print_str("\nFinancial report...\n");
    ct_financial_report();

    print_str("\nFinal state...\n");
    ct_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
