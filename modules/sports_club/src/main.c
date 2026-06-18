/* sports_club: Sports club management (v1.0)
 * Memberships, courts, tournaments, coaches, analytics
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

#define MAX_MEMBERS    14
#define MAX_COURTS     8
#define MAX_COACHES    6
#define MAX_BOOKINGS   16
#define MAX_TEAMS      8
#define MAX_MATCHES    10

typedef struct {
    int    member_id;
    int    tier;
    int    join_day;
    double monthly_fee;
    int    points;
    int    n_bookings;
    double total_paid;
    int    active;
} member_t;

typedef struct {
    int    court_id;
    int    type;
    int    capacity;
    int    n_bookings;
    int    maintenance_day;
    double hourly_rate;
    int    status;
    int    active;
} court_t;

typedef struct {
    int    coach_id;
    int    specialty;
    int    rating;
    int    n_students;
    int    n_sessions;
    double hourly_rate;
    double revenue;
    int    active;
} coach_t;

typedef struct {
    int    booking_id;
    int    member_id;
    int    court_id;
    int    day;
    int    hour;
    int    duration_hr;
    double fee;
    int    status;
    int    active;
} booking_t;

typedef struct {
    int    team_id;
    int    sport;
    int    n_players;
    int    n_wins;
    int    n_losses;
    int    points;
    int    active;
} team_t;

typedef struct {
    int    match_id;
    int    team_a;
    int    team_b;
    int    court_id;
    int    day;
    int    score_a;
    int    score_b;
    int    status;
    int    active;
} match_t;

typedef struct {
    int    n_members;
    int    n_courts;
    int    n_coaches;
    int    n_bookings;
    int    n_teams;
    int    n_matches;
    double total_revenue;
    double court_revenue;
    double coaching_revenue;
    int    total_bookings_ever;
} sc_state_t;

static member_t members[MAX_MEMBERS];
static court_t courts[MAX_COURTS];
static coach_t coaches[MAX_COACHES];
static booking_t bookings[MAX_BOOKINGS];
static team_t teams[MAX_TEAMS];
static match_t matches[MAX_MATCHES];
static sc_state_t sc;

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

int sc_init(void) {
    if (initialized) return -1;
    sc.n_members = 0; sc.n_courts = 0; sc.n_coaches = 0;
    sc.n_bookings = 0; sc.n_teams = 0; sc.n_matches = 0;
    sc.total_revenue = 0.0; sc.court_revenue = 0.0;
    sc.coaching_revenue = 0.0; sc.total_bookings_ever = 0;
    for (int i = 0; i < MAX_MEMBERS; i++) members[i].active = 0;
    for (int i = 0; i < MAX_COURTS; i++) courts[i].active = 0;
    for (int i = 0; i < MAX_COACHES; i++) coaches[i].active = 0;
    for (int i = 0; i < MAX_BOOKINGS; i++) bookings[i].active = 0;
    for (int i = 0; i < MAX_TEAMS; i++) teams[i].active = 0;
    for (int i = 0; i < MAX_MATCHES; i++) matches[i].active = 0;
    initialized = 1;
    print_str("[SC] Sports club initialized\n");
    return 0;
}

int sc_add_court(int type, int capacity, double rate) {
    if (sc.n_courts >= MAX_COURTS) return -1;
    court_t* c = &courts[sc.n_courts];
    c->court_id = sc.n_courts;
    c->type = type;
    c->capacity = capacity;
    c->n_bookings = 0;
    c->maintenance_day = 0;
    c->hourly_rate = rate;
    c->status = 1;
    c->active = 1;
    sc.n_courts++;
    print_str("[SC] Court "); print_int(sc.n_courts - 1);
    print_str(" type="); print_int(type);
    print_str(" cap="); print_int(capacity);
    print_str(" rate="); print_int((int)rate); print_str("/hr\n");
    return sc.n_courts - 1;
}

int sc_register_member(int tier, int join_day) {
    if (sc.n_members >= MAX_MEMBERS) return -1;
    member_t* m = &members[sc.n_members];
    m->member_id = sc.n_members;
    m->tier = tier;
    m->join_day = join_day;
    double fees[4] = {39.99, 59.99, 89.99, 149.99};
    m->monthly_fee = fees[tier < 4 ? tier : 0];
    m->points = 0;
    m->n_bookings = 0;
    m->total_paid = 0.0;
    m->active = 1;
    sc.n_members++;
    print_str("[SC] Member "); print_int(sc.n_members - 1);
    print_str(" tier="); print_int(tier);
    print_str(" fee="); print_int((int)m->monthly_fee); print_str("/mo\n");
    return sc.n_members - 1;
}

int sc_add_coach(int specialty, int rating, double rate) {
    if (sc.n_coaches >= MAX_COACHES) return -1;
    coach_t* c = &coaches[sc.n_coaches];
    c->coach_id = sc.n_coaches;
    c->specialty = specialty;
    c->rating = rating;
    c->n_students = 0;
    c->n_sessions = 0;
    c->hourly_rate = rate;
    c->revenue = 0.0;
    c->active = 1;
    sc.n_coaches++;
    print_str("[SC] Coach "); print_int(sc.n_coaches - 1);
    print_str(" specialty="); print_int(specialty);
    print_str(" rating="); print_int(rating);
    print_str(" rate="); print_int((int)rate); print_str("/hr\n");
    return sc.n_coaches - 1;
}

int sc_add_team(int sport) {
    if (sc.n_teams >= MAX_TEAMS) return -1;
    team_t* t = &teams[sc.n_teams];
    t->team_id = sc.n_teams;
    t->sport = sport;
    t->n_players = 0;
    t->n_wins = 0;
    t->n_losses = 0;
    t->points = 0;
    t->active = 1;
    sc.n_teams++;
    print_str("[SC] Team "); print_int(sc.n_teams - 1);
    print_str(" sport="); print_int(sport); print_str("\n");
    return sc.n_teams - 1;
}

int sc_add_player_to_team(int team_id) {
    if (team_id >= sc.n_teams) return -1;
    teams[team_id].n_players++;
    print_str("[SC] Player added to T"); print_int(team_id);
    print_str(" (now "); print_int(teams[team_id].n_players); print_str(" players)\n");
    return 0;
}

int sc_book_court(int member_id, int court_id, int day, int hour, int duration) {
    if (sc.n_bookings >= MAX_BOOKINGS) return -1;
    if (member_id >= sc.n_members || court_id >= sc.n_courts) return -2;
    court_t* c = &courts[court_id];
    if (c->status != 1) return -3;
    booking_t* b = &bookings[sc.n_bookings];
    b->booking_id = sc.n_bookings;
    b->member_id = member_id;
    b->court_id = court_id;
    b->day = day;
    b->hour = hour;
    b->duration_hr = duration;
    b->fee = c->hourly_rate * duration;
    b->status = 1;
    b->active = 1;
    c->n_bookings++;
    members[member_id].n_bookings++;
    sc.n_bookings++;
    sc.total_bookings_ever++;
    print_str("[SC] Booking "); print_int(sc.n_bookings - 1);
    print_str(" M"); print_int(member_id);
    print_str(" C"); print_int(court_id);
    print_str(" day="); print_int(day);
    print_str(" hr="); print_int(hour);
    print_str(" dur="); print_int(duration); print_str("hr");
    print_str(" fee="); print_int((int)b->fee); print_str("\n");
    return sc.n_bookings - 1;
}

void sc_pay_booking(int booking_id) {
    if (booking_id >= sc.n_bookings) return;
    booking_t* b = &bookings[booking_id];
    b->status = 3;
    members[b->member_id].total_paid += b->fee;
    members[b->member_id].points += (int)b->fee;
    sc.court_revenue += b->fee;
    sc.total_revenue += b->fee;
    print_str("[SC] Booking "); print_int(booking_id);
    print_str(" paid M"); print_int(b->member_id);
    print_str(" fee="); print_int((int)b->fee); print_str("\n");
}

int sc_coaching_session(int coach_id, int member_id, int hours) {
    if (coach_id >= sc.n_coaches || member_id >= sc.n_members) return -1;
    coach_t* c = &coaches[coach_id];
    double fee = c->hourly_rate * hours;
    c->n_sessions++;
    c->n_students++;
    c->revenue += fee;
    members[member_id].total_paid += fee;
    sc.coaching_revenue += fee;
    sc.total_revenue += fee;
    print_str("[SC] Coaching "); print_int(coach_id);
    print_str(" -> M"); print_int(member_id);
    print_str(" "); print_int(hours); print_str("hr");
    print_str(" fee="); print_int((int)fee); print_str("\n");
    return 0;
}

int sc_schedule_match(int team_a, int team_b, int court_id, int day) {
    if (sc.n_matches >= MAX_MATCHES) return -1;
    if (team_a >= sc.n_teams || team_b >= sc.n_teams) return -2;
    if (court_id >= sc.n_courts) return -3;
    match_t* m = &matches[sc.n_matches];
    m->match_id = sc.n_matches;
    m->team_a = team_a;
    m->team_b = team_b;
    m->court_id = court_id;
    m->day = day;
    m->score_a = 0;
    m->score_b = 0;
    m->status = 1;
    m->active = 1;
    sc.n_matches++;
    print_str("[SC] Match "); print_int(sc.n_matches - 1);
    print_str(" T"); print_int(team_a); print_str(" vs T"); print_int(team_b);
    print_str(" C"); print_int(court_id);
    print_str(" day="); print_int(day); print_str("\n");
    return sc.n_matches - 1;
}

void sc_play_match(int match_id, int score_a, int score_b) {
    if (match_id >= sc.n_matches) return;
    match_t* m = &matches[match_id];
    m->score_a = score_a;
    m->score_b = score_b;
    m->status = 3;
    if (score_a > score_b) {
        teams[m->team_a].n_wins++;
        teams[m->team_b].n_losses++;
        teams[m->team_a].points += 3;
    } else if (score_b > score_a) {
        teams[m->team_b].n_wins++;
        teams[m->team_a].n_losses++;
        teams[m->team_b].points += 3;
    } else {
        teams[m->team_a].points += 1;
        teams[m->team_b].points += 1;
    }
    print_str("[SC] Match "); print_int(match_id);
    print_str(" T"); print_int(m->team_a); print_str(" "); print_int(score_a);
    print_str(" - "); print_int(score_b); print_str(" T"); print_int(m->team_b);
    print_str("\n");
}

void sc_maintain_court(int court_id, int day) {
    if (court_id >= sc.n_courts) return;
    courts[court_id].maintenance_day = day;
    courts[court_id].status = 2;
    print_str("[SC] Court "); print_int(court_id);
    print_str(" maintenance day="); print_int(day); print_str("\n");
}

void sc_complete_maintenance(int court_id) {
    if (court_id >= sc.n_courts) return;
    courts[court_id].status = 1;
    print_str("[SC] Court "); print_int(court_id); print_str(" maintenance complete\n");
}

void sc_standings(void) {
    print_str("[SC] Team standings:\n");
    for (int i = 0; i < sc.n_teams; i++) {
        team_t* t = &teams[i];
        print_str("  T"); print_int(i);
        print_str(" sport="); print_int(t->sport);
        print_str(" W"); print_int(t->n_wins);
        print_str(" L"); print_int(t->n_losses);
        print_str(" pts="); print_int(t->points); print_str("\n");
    }
}

void sc_court_utilization(void) {
    print_str("[SC] Court utilization:\n");
    for (int i = 0; i < sc.n_courts; i++) {
        court_t* c = &courts[i];
        print_str("  C"); print_int(i);
        print_str(" type="); print_int(c->type);
        print_str(" bookings="); print_int(c->n_bookings);
        print_str(" status="); print_int(c->status); print_str("\n");
    }
}

void sc_financial_report(void) {
    print_str("[SC] Financial report:\n");
    print_str("  Court revenue: "); print_int((int)sc.court_revenue); print_str("\n");
    print_str("  Coaching revenue: "); print_int((int)sc.coaching_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)sc.total_revenue); print_str("\n");
    print_str("  Total bookings: "); print_int(sc.total_bookings_ever); print_str("\n");
}

void sc_print_state(void) {
    print_str("[SC] Members="); print_int(sc.n_members);
    print_str(" Courts="); print_int(sc.n_courts);
    print_str(" Coaches="); print_int(sc.n_coaches);
    print_str(" Bookings="); print_int(sc.n_bookings);
    print_str(" Teams="); print_int(sc.n_teams);
    print_str(" Matches="); print_int(sc.n_matches);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)sc.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Sports Club Demo ===\n\n");
    sc_init();

    print_str("Adding courts...\n");
    sc_add_court(1, 4, 25.0);
    sc_add_court(1, 4, 25.0);
    sc_add_court(2, 6, 30.0);
    sc_add_court(2, 6, 30.0);
    sc_add_court(3, 2, 20.0);
    sc_add_court(3, 2, 20.0);
    sc_add_court(1, 4, 25.0);
    sc_add_court(2, 6, 30.0);

    print_str("\nRegistering members...\n");
    sc_register_member(0, 1);
    sc_register_member(1, 1);
    sc_register_member(2, 2);
    sc_register_member(3, 1);
    sc_register_member(1, 3);
    sc_register_member(0, 2);
    sc_register_member(2, 1);
    sc_register_member(3, 4);
    sc_register_member(0, 2);
    sc_register_member(1, 1);

    print_str("\nAdding coaches...\n");
    sc_add_coach(1, 90, 40.0);
    sc_add_coach(2, 85, 35.0);
    sc_add_coach(3, 92, 45.0);
    sc_add_coach(1, 88, 38.0);
    sc_add_coach(2, 80, 30.0);

    print_str("\nAdding teams...\n");
    sc_add_team(1);
    sc_add_team(1);
    sc_add_team(2);
    sc_add_team(2);
    sc_add_team(3);
    sc_add_team(3);

    print_str("\nAdding players to teams...\n");
    sc_add_player_to_team(0);
    sc_add_player_to_team(0);
    sc_add_player_to_team(0);
    sc_add_player_to_team(1);
    sc_add_player_to_team(1);
    sc_add_player_to_team(1);
    sc_add_player_to_team(2);
    sc_add_player_to_team(2);
    sc_add_player_to_team(3);
    sc_add_player_to_team(3);
    sc_add_player_to_team(4);
    sc_add_player_to_team(4);
    sc_add_player_to_team(5);
    sc_add_player_to_team(5);

    print_str("\nBooking courts...\n");
    sc_book_court(0, 0, 10, 9, 2);
    sc_book_court(1, 1, 10, 10, 1);
    sc_book_court(2, 2, 10, 14, 2);
    sc_book_court(3, 3, 11, 9, 1);
    sc_book_court(4, 4, 11, 11, 2);
    sc_book_court(5, 5, 11, 15, 1);
    sc_book_court(6, 6, 12, 10, 2);
    sc_book_court(7, 7, 12, 14, 1);
    sc_book_court(8, 0, 13, 9, 2);
    sc_book_court(9, 1, 13, 11, 1);
    sc_book_court(0, 2, 13, 15, 2);
    sc_book_court(1, 3, 14, 10, 1);
    sc_book_court(2, 4, 14, 14, 2);
    sc_book_court(3, 5, 14, 9, 1);
    sc_book_court(4, 6, 15, 11, 2);
    sc_book_court(5, 7, 15, 15, 1);

    print_str("\nPaying bookings...\n");
    for (int i = 0; i < sc.n_bookings; i++) {
        sc_pay_booking(i);
    }

    print_str("\nCoaching sessions...\n");
    sc_coaching_session(0, 0, 2);
    sc_coaching_session(1, 1, 1);
    sc_coaching_session(2, 2, 2);
    sc_coaching_session(3, 3, 1);
    sc_coaching_session(4, 4, 2);
    sc_coaching_session(0, 5, 1);
    sc_coaching_session(1, 6, 2);
    sc_coaching_session(2, 7, 1);

    print_str("\nScheduling matches...\n");
    sc_schedule_match(0, 1, 0, 20);
    sc_schedule_match(2, 3, 2, 20);
    sc_schedule_match(4, 5, 4, 21);
    sc_schedule_match(0, 2, 0, 22);
    sc_schedule_match(1, 3, 2, 22);
    sc_schedule_match(4, 0, 4, 23);
    sc_schedule_match(1, 5, 6, 23);
    sc_schedule_match(2, 4, 2, 24);

    print_str("\nPlaying matches...\n");
    sc_play_match(0, 21, 18);
    sc_play_match(1, 15, 21);
    sc_play_match(2, 11, 9);
    sc_play_match(3, 21, 21);
    sc_play_match(4, 18, 21);
    sc_play_match(5, 21, 15);
    sc_play_match(6, 21, 19);
    sc_play_match(7, 12, 15);

    print_str("\nCourt maintenance...\n");
    sc_maintain_court(0, 25);
    sc_maintain_court(2, 26);
    sc_maintain_court(4, 27);
    sc_complete_maintenance(0);
    sc_complete_maintenance(2);
    sc_complete_maintenance(4);

    print_str("\nTeam standings...\n");
    sc_standings();

    print_str("\nCourt utilization...\n");
    sc_court_utilization();

    print_str("\nFinancial report...\n");
    sc_financial_report();

    print_str("\nFinal state...\n");
    sc_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
