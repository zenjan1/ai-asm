/* sports_league: Sports league management (v1.0)
 * Teams, players, matches, standings, tickets, revenue
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

#define MAX_TEAMS    10
#define MAX_PLAYERS  16
#define MAX_MATCHES  14
#define MAX_TICKETS  18

typedef struct {
    int    team_id;
    int    wins;
    int    losses;
    int    draws;
    int    goals_for;
    int    goals_against;
    int    points;
    double revenue;
    int    active;
} team_t;

typedef struct {
    int    player_id;
    int    team_id;
    int    number;
    int    position;
    int    goals;
    int    assists;
    int    yellow_cards;
    int    red_cards;
    int    games_played;
    int    active;
} player_t;

typedef struct {
    int    match_id;
    int    home_team;
    int    away_team;
    int    home_score;
    int    away_score;
    int    day;
    int    attendance;
    int    played;
    int    active;
} match_t;

typedef struct {
    int    ticket_id;
    int    match_id;
    int    section;
    double price;
    int    sold;
    int    active;
} ticket_t;

typedef struct {
    int    n_teams;
    int    n_players;
    int    n_matches;
    int    n_tickets;
    double ticket_revenue;
    double sponsor_revenue;
    double broadcast_revenue;
    double total_revenue;
    double total_costs;
    int    total_attendance;
} sl_state_t;

static team_t teams[MAX_TEAMS];
static player_t players[MAX_PLAYERS];
static match_t matches[MAX_MATCHES];
static ticket_t tickets[MAX_TICKETS];
static sl_state_t sl;

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

int sl_init(void) {
    if (initialized) return -1;
    sl.n_teams = 0; sl.n_players = 0; sl.n_matches = 0; sl.n_tickets = 0;
    sl.ticket_revenue = 0.0; sl.sponsor_revenue = 0.0;
    sl.broadcast_revenue = 0.0; sl.total_revenue = 0.0;
    sl.total_costs = 0.0; sl.total_attendance = 0;
    for (int i = 0; i < MAX_TEAMS; i++) teams[i].active = 0;
    for (int i = 0; i < MAX_PLAYERS; i++) players[i].active = 0;
    for (int i = 0; i < MAX_MATCHES; i++) matches[i].active = 0;
    for (int i = 0; i < MAX_TICKETS; i++) tickets[i].active = 0;
    initialized = 1;
    print_str("[SL] League initialized\n");
    return 0;
}

int sl_add_team(void) {
    if (sl.n_teams >= MAX_TEAMS) return -1;
    team_t* t = &teams[sl.n_teams];
    t->team_id = sl.n_teams;
    t->wins = 0; t->losses = 0; t->draws = 0;
    t->goals_for = 0; t->goals_against = 0;
    t->points = 0; t->revenue = 0.0;
    t->active = 1;
    sl.n_teams++;
    print_str("[SL] Team "); print_int(sl.n_teams - 1); print_str(" added\n");
    return sl.n_teams - 1;
}

int sl_add_player(int team_id, int number, int position) {
    if (sl.n_players >= MAX_PLAYERS || team_id >= sl.n_teams) return -1;
    player_t* p = &players[sl.n_players];
    p->player_id = sl.n_players;
    p->team_id = team_id;
    p->number = number;
    p->position = position;
    p->goals = 0; p->assists = 0;
    p->yellow_cards = 0; p->red_cards = 0;
    p->games_played = 0;
    p->active = 1;
    sl.n_players++;
    print_str("[SL] Player "); print_int(sl.n_players - 1);
    print_str(" T"); print_int(team_id);
    print_str(" #"); print_int(number);
    print_str(" pos="); print_int(position); print_str("\n");
    return sl.n_players - 1;
}

int sl_schedule_match(int home, int away, int day) {
    if (sl.n_matches >= MAX_MATCHES) return -1;
    if (home >= sl.n_teams || away >= sl.n_teams) return -2;
    match_t* m = &matches[sl.n_matches];
    m->match_id = sl.n_matches;
    m->home_team = home;
    m->away_team = away;
    m->home_score = 0; m->away_score = 0;
    m->day = day;
    m->attendance = 0;
    m->played = 0;
    m->active = 1;
    sl.n_matches++;
    print_str("[SL] Match "); print_int(sl.n_matches - 1);
    print_str(" T"); print_int(home);
    print_str(" vs T"); print_int(away);
    print_str(" day="); print_int(day); print_str("\n");
    return sl.n_matches - 1;
}

int sl_play_match(int match_id, int home_score, int away_score, int attendance) {
    if (match_id >= sl.n_matches) return -1;
    match_t* m = &matches[match_id];
    if (m->played) return -2;
    m->home_score = home_score;
    m->away_score = away_score;
    m->attendance = attendance;
    m->played = 1;
    team_t* home = &teams[m->home_team];
    team_t* away = &teams[m->away_team];
    home->goals_for += home_score;
    home->goals_against += away_score;
    away->goals_for += away_score;
    away->goals_against += home_score;
    if (home_score > away_score) {
        home->wins++; home->points += 3;
        away->losses++;
    } else if (home_score < away_score) {
        away->wins++; away->points += 3;
        home->losses++;
    } else {
        home->draws++; home->points += 1;
        away->draws++; away->points += 1;
    }
    sl.total_attendance += attendance;
    print_str("[SL] Match "); print_int(match_id);
    print_str(" T"); print_int(m->home_team);
    print_str(" "); print_int(home_score);
    print_str("-"); print_int(away_score);
    print_str(" T"); print_int(m->away_team);
    print_str(" att="); print_int(attendance); print_str("\n");
    return 0;
}

int sl_add_player_stats(int player_id, int goals, int assists, int yellow, int red) {
    if (player_id >= sl.n_players) return -1;
    player_t* p = &players[player_id];
    p->goals += goals;
    p->assists += assists;
    p->yellow_cards += yellow;
    p->red_cards += red;
    p->games_played++;
    return 0;
}

int sl_sell_ticket(int match_id, int section, double price) {
    if (sl.n_tickets >= MAX_TICKETS || match_id >= sl.n_matches) return -1;
    ticket_t* t = &tickets[sl.n_tickets];
    t->ticket_id = sl.n_tickets;
    t->match_id = match_id;
    t->section = section;
    t->price = price;
    t->sold = 1;
    t->active = 1;
    sl.ticket_revenue += price;
    sl.total_revenue += price;
    teams[matches[match_id].home_team].revenue += price * 0.6;
    teams[matches[match_id].away_team].revenue += price * 0.4;
    sl.n_tickets++;
    print_str("[SL] Ticket "); print_int(sl.n_tickets - 1);
    print_str(" M"); print_int(match_id);
    print_str(" sec="); print_int(section);
    print_str(" $"); print_int((int)price); print_str("\n");
    return sl.n_tickets - 1;
}

int sl_add_sponsorship(double amount) {
    sl.sponsor_revenue += amount;
    sl.total_revenue += amount;
    print_str("[SL] Sponsorship +"); print_int((int)amount); print_str("\n");
    return 0;
}

int sl_add_broadcast(double amount) {
    sl.broadcast_revenue += amount;
    sl.total_revenue += amount;
    print_str("[SL] Broadcast +"); print_int((int)amount); print_str("\n");
    return 0;
}

void sl_standings_report(void) {
    print_str("[SL] Standings:\n");
    print_str("  T   W  L  D  GF GA Pts\n");
    for (int i = 0; i < sl.n_teams; i++) {
        team_t* t = &teams[i];
        print_str("  "); print_int(i);
        print_str("  "); print_int(t->wins);
        print_str("  "); print_int(t->losses);
        print_str("  "); print_int(t->draws);
        print_str("  "); print_int(t->goals_for);
        print_str(" "); print_int(t->goals_against);
        print_str(" "); print_int(t->points); print_str("\n");
    }
}

void sl_financial_report(void) {
    print_str("[SL] Financial report:\n");
    print_str("  Ticket revenue: "); print_int((int)sl.ticket_revenue); print_str("\n");
    print_str("  Sponsor revenue: "); print_int((int)sl.sponsor_revenue); print_str("\n");
    print_str("  Broadcast revenue: "); print_int((int)sl.broadcast_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)sl.total_revenue); print_str("\n");
    print_str("  Costs: "); print_int((int)sl.total_costs); print_str("\n");
    double profit = sl.total_revenue - sl.total_costs;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    print_str("  Total attendance: "); print_int(sl.total_attendance); print_str("\n");
}

void sl_print_state(void) {
    print_str("[SL] Teams="); print_int(sl.n_teams);
    print_str(" Players="); print_int(sl.n_players);
    print_str(" Matches="); print_int(sl.n_matches);
    print_str(" Tickets="); print_int(sl.n_tickets);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)sl.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Sports League Demo ===\n\n");
    sl_init();

    print_str("Adding teams...\n");
    for (int i = 0; i < 10; i++) sl_add_team();

    print_str("\nAdding players...\n");
    sl_add_player(0, 10, 1);
    sl_add_player(0, 7, 2);
    sl_add_player(1, 9, 1);
    sl_add_player(1, 11, 2);
    sl_add_player(2, 8, 1);
    sl_add_player(2, 6, 3);
    sl_add_player(3, 10, 1);
    sl_add_player(3, 5, 2);
    sl_add_player(4, 7, 1);
    sl_add_player(4, 9, 2);
    sl_add_player(5, 11, 1);
    sl_add_player(5, 8, 3);
    sl_add_player(6, 10, 1);
    sl_add_player(7, 6, 2);
    sl_add_player(8, 9, 1);
    sl_add_player(9, 7, 1);

    print_str("\nScheduling matches...\n");
    sl_schedule_match(0, 1, 1);
    sl_schedule_match(2, 3, 1);
    sl_schedule_match(4, 5, 2);
    sl_schedule_match(6, 7, 2);
    sl_schedule_match(8, 9, 3);
    sl_schedule_match(1, 2, 4);
    sl_schedule_match(3, 4, 5);
    sl_schedule_match(5, 6, 6);
    sl_schedule_match(7, 8, 7);
    sl_schedule_match(9, 0, 8);
    sl_schedule_match(0, 3, 9);
    sl_schedule_match(1, 5, 10);
    sl_schedule_match(2, 7, 11);
    sl_schedule_match(4, 9, 12);

    print_str("\nPlaying matches...\n");
    sl_play_match(0, 3, 1, 25000);
    sl_play_match(1, 2, 2, 18000);
    sl_play_match(2, 1, 0, 22000);
    sl_play_match(3, 0, 1, 15000);
    sl_play_match(4, 4, 2, 20000);
    sl_play_match(5, 1, 3, 19000);
    sl_play_match(6, 2, 1, 21000);
    sl_play_match(7, 3, 0, 17000);
    sl_play_match(8, 0, 2, 16000);
    sl_play_match(9, 1, 1, 24000);
    sl_play_match(10, 2, 0, 23000);
    sl_play_match(11, 1, 2, 20000);
    sl_play_match(12, 3, 1, 18000);
    sl_play_match(13, 0, 0, 22000);

    print_str("\nAdding player stats...\n");
    sl_add_player_stats(0, 2, 1, 0, 0);
    sl_add_player_stats(1, 1, 0, 1, 0);
    sl_add_player_stats(2, 1, 1, 0, 0);
    sl_add_player_stats(3, 0, 2, 0, 0);
    sl_add_player_stats(4, 1, 0, 0, 0);
    sl_add_player_stats(5, 0, 0, 1, 0);
    sl_add_player_stats(6, 2, 0, 1, 0);
    sl_add_player_stats(7, 0, 1, 0, 0);
    sl_add_player_stats(8, 1, 1, 0, 0);
    sl_add_player_stats(9, 0, 0, 0, 1);
    sl_add_player_stats(10, 3, 0, 0, 0);
    sl_add_player_stats(11, 0, 1, 1, 0);
    sl_add_player_stats(12, 1, 0, 0, 0);
    sl_add_player_stats(13, 0, 0, 1, 0);
    sl_add_player_stats(14, 2, 1, 0, 0);
    sl_add_player_stats(15, 1, 0, 0, 0);

    print_str("\nSelling tickets...\n");
    for (int i = 0; i < 18; i++) {
        int match_id = i % 14;
        int section = (i % 3) + 1;
        double price = 50.0 + section * 25.0;
        sl_sell_ticket(match_id, section, price);
    }

    print_str("\nSponsorship & broadcast...\n");
    sl_add_sponsorship(500000.0);
    sl_add_sponsorship(300000.0);
    sl_add_broadcast(800000.0);
    sl_add_broadcast(600000.0);

    print_str("\nStandings report...\n");
    sl_standings_report();

    print_str("\nFinancial report...\n");
    sl_financial_report();

    print_str("\nFinal state...\n");
    sl_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
