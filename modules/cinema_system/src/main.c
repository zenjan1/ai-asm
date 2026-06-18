/* cinema_system: Cinema management system (v1.0)
 * Screens, scheduling, ticketing, concessions, analytics
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

#define MAX_SCREENS   6
#define MAX_MOVIES    10
#define MAX_SHOWINGS  16
#define MAX_TICKETS   20
#define MAX_CONCESSIONS 12
#define MAX_MEMBERS   10

typedef struct {
    int    screen_id;
    int    capacity;
    int    type;
    int    n_showings;
    int    total_sold;
    double revenue;
    int    active;
} screen_t;

typedef struct {
    int    movie_id;
    int    genre;
    int    duration_min;
    double rating;
    int    n_showings;
    int    total_tickets;
    double box_office;
    int    active;
} movie_t;

typedef struct {
    int    showing_id;
    int    screen_id;
    int    movie_id;
    int    day;
    int    time_slot;
    double price;
    int    seats_sold;
    int    active;
} showing_t;

typedef struct {
    int    ticket_id;
    int    showing_id;
    int    seat_num;
    double price;
    int    member_id;
    int    active;
} ticket_t;

typedef struct {
    int    concession_id;
    int    type;
    double price;
    double cost;
    int    stock;
    int    n_sold;
    double revenue;
    int    active;
} concession_t;

typedef struct {
    int    member_id;
    int    tier;
    int    n_tickets;
    double total_spent;
    int    points;
    int    active;
} member_t;

typedef struct {
    int    n_screens;
    int    n_movies;
    int    n_showings;
    int    n_tickets;
    int    n_concessions;
    int    n_members;
    double box_office;
    double concession_revenue;
    double total_revenue;
    double total_costs;
    int    total_attendees;
} cs_state_t;

static screen_t screens[MAX_SCREENS];
static movie_t movies[MAX_MOVIES];
static showing_t showings[MAX_SHOWINGS];
static ticket_t tickets[MAX_TICKETS];
static concession_t concessions[MAX_CONCESSIONS];
static member_t members[MAX_MEMBERS];
static cs_state_t cs;

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

int cs_init(void) {
    if (initialized) return -1;
    cs.n_screens = 0; cs.n_movies = 0; cs.n_showings = 0;
    cs.n_tickets = 0; cs.n_concessions = 0; cs.n_members = 0;
    cs.box_office = 0.0; cs.concession_revenue = 0.0;
    cs.total_revenue = 0.0; cs.total_costs = 0.0;
    cs.total_attendees = 0;
    for (int i = 0; i < MAX_SCREENS; i++) screens[i].active = 0;
    for (int i = 0; i < MAX_MOVIES; i++) movies[i].active = 0;
    for (int i = 0; i < MAX_SHOWINGS; i++) showings[i].active = 0;
    for (int i = 0; i < MAX_TICKETS; i++) tickets[i].active = 0;
    for (int i = 0; i < MAX_CONCESSIONS; i++) concessions[i].active = 0;
    for (int i = 0; i < MAX_MEMBERS; i++) members[i].active = 0;
    initialized = 1;
    print_str("[CS] Cinema system initialized\n");
    return 0;
}

int cs_add_screen(int capacity, int type) {
    if (cs.n_screens >= MAX_SCREENS) return -1;
    screen_t* s = &screens[cs.n_screens];
    s->screen_id = cs.n_screens;
    s->capacity = capacity;
    s->type = type;
    s->n_showings = 0;
    s->total_sold = 0;
    s->revenue = 0.0;
    s->active = 1;
    cs.n_screens++;
    print_str("[CS] Screen "); print_int(cs.n_screens - 1);
    print_str(" cap="); print_int(capacity);
    print_str(" type="); print_int(type); print_str("\n");
    return cs.n_screens - 1;
}

int cs_add_movie(int genre, int duration, double rating) {
    if (cs.n_movies >= MAX_MOVIES) return -1;
    movie_t* m = &movies[cs.n_movies];
    m->movie_id = cs.n_movies;
    m->genre = genre;
    m->duration_min = duration;
    m->rating = rating;
    m->n_showings = 0;
    m->total_tickets = 0;
    m->box_office = 0.0;
    m->active = 1;
    cs.n_movies++;
    print_str("[CS] Movie "); print_int(cs.n_movies - 1);
    print_str(" genre="); print_int(genre);
    print_str(" dur="); print_int(duration); print_str("min");
    print_str(" rating="); print_int((int)(rating * 10)); print_str("\n");
    return cs.n_movies - 1;
}

int cs_schedule_showing(int screen_id, int movie_id, int day, int slot, double price) {
    if (cs.n_showings >= MAX_SHOWINGS) return -1;
    if (screen_id >= cs.n_screens || movie_id >= cs.n_movies) return -2;
    showing_t* sh = &showings[cs.n_showings];
    sh->showing_id = cs.n_showings;
    sh->screen_id = screen_id;
    sh->movie_id = movie_id;
    sh->day = day;
    sh->time_slot = slot;
    sh->price = price;
    sh->seats_sold = 0;
    sh->active = 1;
    screens[screen_id].n_showings++;
    movies[movie_id].n_showings++;
    cs.n_showings++;
    print_str("[CS] Showing "); print_int(cs.n_showings - 1);
    print_str(" SC"); print_int(screen_id);
    print_str(" M"); print_int(movie_id);
    print_str(" day="); print_int(day);
    print_str(" slot="); print_int(slot);
    print_str(" price="); print_int((int)price); print_str("\n");
    return cs.n_showings - 1;
}

int cs_register_member(int tier) {
    if (cs.n_members >= MAX_MEMBERS) return -1;
    member_t* m = &members[cs.n_members];
    m->member_id = cs.n_members;
    m->tier = tier;
    m->n_tickets = 0;
    m->total_spent = 0.0;
    m->points = 0;
    m->active = 1;
    cs.n_members++;
    print_str("[CS] Member "); print_int(cs.n_members - 1);
    print_str(" tier="); print_int(tier); print_str("\n");
    return cs.n_members - 1;
}

int cs_add_concession(int type, double price, double cost, int stock) {
    if (cs.n_concessions >= MAX_CONCESSIONS) return -1;
    concession_t* c = &concessions[cs.n_concessions];
    c->concession_id = cs.n_concessions;
    c->type = type;
    c->price = price;
    c->cost = cost;
    c->stock = stock;
    c->n_sold = 0;
    c->revenue = 0.0;
    c->active = 1;
    cs.n_concessions++;
    print_str("[CS] Concession "); print_int(cs.n_concessions - 1);
    print_str(" type="); print_int(type);
    print_str(" price="); print_int((int)price);
    print_str(" stock="); print_int(stock); print_str("\n");
    return cs.n_concessions - 1;
}

int cs_sell_ticket(int showing_id, int member_id) {
    if (cs.n_tickets >= MAX_TICKETS || showing_id >= cs.n_showings) return -1;
    showing_t* sh = &showings[showing_id];
    screen_t* sc = &screens[sh->screen_id];
    if (sh->seats_sold >= sc->capacity) return -2;
    ticket_t* t = &tickets[cs.n_tickets];
    t->ticket_id = cs.n_tickets;
    t->showing_id = showing_id;
    t->seat_num = sh->seats_sold + 1;
    double price = sh->price;
    if (member_id >= 0 && member_id < cs.n_members) {
        t->member_id = member_id;
        if (members[member_id].tier >= 2) price *= 0.85;
        members[member_id].n_tickets++;
        members[member_id].total_spent += price;
        members[member_id].points += (int)price;
    } else {
        t->member_id = -1;
    }
    t->price = price;
    t->active = 1;
    sh->seats_sold++;
    sc->total_sold++;
    sc->revenue += price;
    movies[sh->movie_id].total_tickets++;
    movies[sh->movie_id].box_office += price;
    cs.box_office += price;
    cs.total_revenue += price;
    cs.total_attendees++;
    cs.n_tickets++;
    print_str("[CS] Ticket "); print_int(cs.n_tickets - 1);
    print_str(" SH"); print_int(showing_id);
    print_str(" seat="); print_int(t->seat_num);
    print_str(" price="); print_int((int)price);
    if (t->member_id >= 0) { print_str(" M"); print_int(t->member_id); }
    print_str("\n");
    return cs.n_tickets - 1;
}

int cs_sell_concession(int concession_id, int member_id) {
    if (concession_id >= cs.n_concessions) return -1;
    concession_t* c = &concessions[concession_id];
    if (c->stock <= 0) return -2;
    c->stock--;
    c->n_sold++;
    c->revenue += c->price;
    cs.concession_revenue += c->price;
    cs.total_revenue += c->price;
    cs.total_costs += c->cost;
    if (member_id >= 0 && member_id < cs.n_members) {
        members[member_id].total_spent += c->price;
        members[member_id].points += (int)(c->price / 2);
    }
    print_str("[CS] Concession C"); print_int(concession_id);
    print_str(" sold price="); print_int((int)c->price);
    print_str(" stock="); print_int(c->stock); print_str("\n");
    return 0;
}

void cs_complete_showing(int showing_id) {
    if (showing_id >= cs.n_showings) return;
    showing_t* sh = &showings[showing_id];
    screen_t* sc = &screens[sh->screen_id];
    double occ = (sc->capacity > 0) ? (double)sh->seats_sold / sc->capacity * 100.0 : 0.0;
    print_str("[CS] Showing "); print_int(showing_id);
    print_str(" complete");
    print_str(" sold="); print_int(sh->seats_sold);
    print_str("/"); print_int(sc->capacity);
    print_str(" occ="); print_int((int)occ); print_str("%\n");
}

void cs_box_office_report(void) {
    print_str("[CS] Box office report:\n");
    print_str("  Total tickets: "); print_int(cs.n_tickets); print_str("\n");
    print_str("  Box office: "); print_int((int)cs.box_office); print_str("\n");
    print_str("  Total attendees: "); print_int(cs.total_attendees); print_str("\n");
    for (int i = 0; i < cs.n_movies; i++) {
        movie_t* m = &movies[i];
        print_str("  M"); print_int(i);
        print_str(" genre="); print_int(m->genre);
        print_str(" tickets="); print_int(m->total_tickets);
        print_str(" box="); print_int((int)m->box_office); print_str("\n");
    }
}

void cs_concession_report(void) {
    print_str("[CS] Concession report:\n");
    print_str("  Total revenue: "); print_int((int)cs.concession_revenue); print_str("\n");
    for (int i = 0; i < cs.n_concessions; i++) {
        concession_t* c = &concessions[i];
        print_str("  C"); print_int(i);
        print_str(" type="); print_int(c->type);
        print_str(" sold="); print_int(c->n_sold);
        print_str(" rev="); print_int((int)c->revenue); print_str("\n");
    }
}

void cs_financial_summary(void) {
    print_str("[CS] Financial summary:\n");
    print_str("  Box office: "); print_int((int)cs.box_office); print_str("\n");
    print_str("  Concessions: "); print_int((int)cs.concession_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)cs.total_revenue); print_str("\n");
    print_str("  Total costs: "); print_int((int)cs.total_costs); print_str("\n");
    double profit = cs.total_revenue - cs.total_costs;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    double margin = (cs.total_revenue > 0) ? profit / cs.total_revenue * 100.0 : 0.0;
    print_str("  Margin: "); print_int((int)margin); print_str("%\n");
}

void cs_print_state(void) {
    print_str("[CS] Screens="); print_int(cs.n_screens);
    print_str(" Movies="); print_int(cs.n_movies);
    print_str(" Showings="); print_int(cs.n_showings);
    print_str(" Tickets="); print_int(cs.n_tickets);
    print_str(" Concessions="); print_int(cs.n_concessions);
    print_str(" Members="); print_int(cs.n_members);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)cs.total_revenue);
    print_str(" Attendees: "); print_int(cs.total_attendees); print_str("\n");
}

int main(void) {
    print_str("=== Cinema System Demo ===\n\n");
    cs_init();

    print_str("Adding screens...\n");
    cs_add_screen(120, 1);
    cs_add_screen(80, 1);
    cs_add_screen(200, 2);
    cs_add_screen(60, 1);
    cs_add_screen(150, 2);
    cs_add_screen(100, 1);

    print_str("\nAdding movies...\n");
    cs_add_movie(1, 130, 8.5);
    cs_add_movie(2, 110, 7.8);
    cs_add_movie(3, 95, 8.2);
    cs_add_movie(1, 140, 9.0);
    cs_add_movie(4, 100, 7.5);
    cs_add_movie(2, 120, 8.8);
    cs_add_movie(3, 105, 7.9);
    cs_add_movie(1, 150, 8.6);

    print_str("\nRegistering members...\n");
    cs_register_member(0);
    cs_register_member(1);
    cs_register_member(2);
    cs_register_member(0);
    cs_register_member(1);
    cs_register_member(2);
    cs_register_member(3);
    cs_register_member(0);

    print_str("\nAdding concessions...\n");
    cs_add_concession(1, 8.0, 2.0, 100);
    cs_add_concession(2, 6.0, 1.5, 80);
    cs_add_concession(3, 10.0, 3.0, 60);
    cs_add_concession(1, 12.0, 4.0, 50);
    cs_add_concession(4, 5.0, 1.0, 120);
    cs_add_concession(2, 7.0, 2.0, 90);
    cs_add_concession(3, 15.0, 5.0, 40);

    print_str("\nScheduling showings...\n");
    cs_schedule_showing(0, 0, 15, 1, 12.0);
    cs_schedule_showing(0, 3, 15, 3, 15.0);
    cs_schedule_showing(1, 1, 15, 2, 10.0);
    cs_schedule_showing(1, 5, 15, 4, 12.0);
    cs_schedule_showing(2, 2, 16, 1, 10.0);
    cs_schedule_showing(2, 7, 16, 3, 15.0);
    cs_schedule_showing(3, 4, 16, 2, 10.0);
    cs_schedule_showing(3, 6, 16, 4, 10.0);
    cs_schedule_showing(4, 0, 17, 1, 15.0);
    cs_schedule_showing(4, 3, 17, 3, 18.0);
    cs_schedule_showing(5, 1, 17, 2, 12.0);
    cs_schedule_showing(5, 5, 17, 4, 12.0);
    cs_schedule_showing(0, 2, 18, 1, 12.0);
    cs_schedule_showing(1, 4, 18, 2, 10.0);
    cs_schedule_showing(2, 7, 18, 3, 15.0);
    cs_schedule_showing(3, 6, 18, 4, 10.0);

    print_str("\nSelling tickets...\n");
    cs_sell_ticket(0, -1);
    cs_sell_ticket(0, 0);
    cs_sell_ticket(0, 1);
    cs_sell_ticket(1, 2);
    cs_sell_ticket(1, 6);
    cs_sell_ticket(2, -1);
    cs_sell_ticket(2, 3);
    cs_sell_ticket(3, 4);
    cs_sell_ticket(3, 5);
    cs_sell_ticket(4, -1);
    cs_sell_ticket(4, 0);
    cs_sell_ticket(5, 1);
    cs_sell_ticket(5, 2);
    cs_sell_ticket(6, 6);
    cs_sell_ticket(7, -1);
    cs_sell_ticket(7, 3);
    cs_sell_ticket(8, 7);
    cs_sell_ticket(9, -1);
    cs_sell_ticket(9, 6);
    cs_sell_ticket(10, -1);
    cs_sell_ticket(11, 4);
    cs_sell_ticket(11, 5);
    cs_sell_ticket(12, -1);
    cs_sell_ticket(13, 0);
    cs_sell_ticket(14, 1);
    cs_sell_ticket(15, 2);

    print_str("\nSelling concessions...\n");
    cs_sell_concession(0, 0);
    cs_sell_concession(1, 1);
    cs_sell_concession(2, 2);
    cs_sell_concession(3, 6);
    cs_sell_concession(4, 3);
    cs_sell_concession(5, 4);
    cs_sell_concession(0, 5);
    cs_sell_concession(1, 7);
    cs_sell_concession(6, -1);
    cs_sell_concession(2, -1);
    cs_sell_concession(3, 0);
    cs_sell_concession(4, 1);

    print_str("\nCompleting showings...\n");
    cs_complete_showing(0);
    cs_complete_showing(1);
    cs_complete_showing(2);
    cs_complete_showing(3);
    cs_complete_showing(4);
    cs_complete_showing(5);
    cs_complete_showing(6);
    cs_complete_showing(7);
    cs_complete_showing(8);
    cs_complete_showing(9);
    cs_complete_showing(10);
    cs_complete_showing(11);
    cs_complete_showing(12);
    cs_complete_showing(13);
    cs_complete_showing(14);
    cs_complete_showing(15);

    print_str("\nBox office report...\n");
    cs_box_office_report();

    print_str("\nConcession report...\n");
    cs_concession_report();

    print_str("\nFinancial summary...\n");
    cs_financial_summary();

    print_str("\nFinal state...\n");
    cs_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
