/* casino_resort: Casino resort management (v1.0)
 * Tables, slots, hotel, dining, entertainment, revenue
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

#define MAX_TABLES    10
#define MAX_SLOTS     12
#define MAX_ROOMS     10
#define MAX_RESTAURANTS 8
#define MAX_SHOWS     8
#define MAX_PLAYERS   14

typedef struct {
    int    table_id;
    int    type;
    double min_bet;
    double max_bet;
    int    seats;
    int    n_players;
    double hold_pct;
    double drop;
    double win;
    int    active;
} table_t;

typedef struct {
    int    slot_id;
    int    type;
    double denom;
    double hold_pct;
    double coin_in;
    double jackpot;
    double win;
    int    active;
} slot_t;

typedef struct {
    int    room_id;
    int    type;
    double nightly_rate;
    int    occupied;
    int    n_stays;
    double revenue;
    int    active;
} room_t;

typedef struct {
    int    rest_id;
    int    type;
    double avg_check;
    int    n_guests;
    double revenue;
    double food_cost;
    int    active;
} restaurant_t;

typedef struct {
    int    show_id;
    int    type;
    double ticket_price;
    int    capacity;
    int    n_sold;
    double revenue;
    double production_cost;
    int    active;
} show_t;

typedef struct {
    int    player_id;
    int    type;
    double total_wagered;
    double total_won;
    double comp_points;
    int    active;
} player_t;

typedef struct {
    int    n_tables;
    int    n_slots;
    int    n_rooms;
    int    n_restaurants;
    int    n_shows;
    int    n_players;
    double table_win;
    double slot_win;
    double hotel_revenue;
    double dining_revenue;
    double show_revenue;
    double total_revenue;
    double total_costs;
    int    total_guests;
} cr_state_t;

static table_t tables[MAX_TABLES];
static slot_t slots[MAX_SLOTS];
static room_t rooms[MAX_ROOMS];
static restaurant_t restaurants[MAX_RESTAURANTS];
static show_t shows[MAX_SHOWS];
static player_t players[MAX_PLAYERS];
static cr_state_t cr;

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

int cr_init(void) {
    if (initialized) return -1;
    cr.n_tables = 0; cr.n_slots = 0; cr.n_rooms = 0;
    cr.n_restaurants = 0; cr.n_shows = 0; cr.n_players = 0;
    cr.table_win = 0.0; cr.slot_win = 0.0;
    cr.hotel_revenue = 0.0; cr.dining_revenue = 0.0;
    cr.show_revenue = 0.0; cr.total_revenue = 0.0;
    cr.total_costs = 0.0; cr.total_guests = 0;
    for (int i = 0; i < MAX_TABLES; i++) tables[i].active = 0;
    for (int i = 0; i < MAX_SLOTS; i++) slots[i].active = 0;
    for (int i = 0; i < MAX_ROOMS; i++) rooms[i].active = 0;
    for (int i = 0; i < MAX_RESTAURANTS; i++) restaurants[i].active = 0;
    for (int i = 0; i < MAX_SHOWS; i++) shows[i].active = 0;
    for (int i = 0; i < MAX_PLAYERS; i++) players[i].active = 0;
    initialized = 1;
    print_str("[CR] Casino resort initialized\n");
    return 0;
}

int cr_add_table(int type, double min_bet, double max_bet, int seats, double hold) {
    if (cr.n_tables >= MAX_TABLES) return -1;
    table_t* t = &tables[cr.n_tables];
    t->table_id = cr.n_tables;
    t->type = type;
    t->min_bet = min_bet;
    t->max_bet = max_bet;
    t->seats = seats;
    t->n_players = 0;
    t->hold_pct = hold;
    t->drop = 0.0;
    t->win = 0.0;
    t->active = 1;
    cr.n_tables++;
    print_str("[CR] Table "); print_int(cr.n_tables - 1);
    print_str(" type="); print_int(type);
    print_str(" min="); print_int((int)min_bet);
    print_str(" max="); print_int((int)max_bet);
    print_str(" hold="); print_int((int)(hold * 100)); print_str("%\n");
    return cr.n_tables - 1;
}

int cr_add_slot(int type, double denom, double hold) {
    if (cr.n_slots >= MAX_SLOTS) return -1;
    slot_t* s = &slots[cr.n_slots];
    s->slot_id = cr.n_slots;
    s->type = type;
    s->denom = denom;
    s->hold_pct = hold;
    s->coin_in = 0.0;
    s->jackpot = 1000.0 + type * 500.0;
    s->win = 0.0;
    s->active = 1;
    cr.n_slots++;
    print_str("[CR] Slot "); print_int(cr.n_slots - 1);
    print_str(" type="); print_int(type);
    print_str(" denom="); print_int((int)denom);
    print_str(" hold="); print_int((int)(hold * 100)); print_str("%\n");
    return cr.n_slots - 1;
}

int cr_add_room(int type, double rate) {
    if (cr.n_rooms >= MAX_ROOMS) return -1;
    room_t* r = &rooms[cr.n_rooms];
    r->room_id = cr.n_rooms;
    r->type = type;
    r->nightly_rate = rate;
    r->occupied = 0;
    r->n_stays = 0;
    r->revenue = 0.0;
    r->active = 1;
    cr.n_rooms++;
    print_str("[CR] Room "); print_int(cr.n_rooms - 1);
    print_str(" type="); print_int(type);
    print_str(" $"); print_int((int)rate); print_str("/nt\n");
    return cr.n_rooms - 1;
}

int cr_add_restaurant(int type, double avg_check) {
    if (cr.n_restaurants >= MAX_RESTAURANTS) return -1;
    restaurant_t* r = &restaurants[cr.n_restaurants];
    r->rest_id = cr.n_restaurants;
    r->type = type;
    r->avg_check = avg_check;
    r->n_guests = 0;
    r->revenue = 0.0;
    r->food_cost = 0.0;
    r->active = 1;
    cr.n_restaurants++;
    print_str("[CR] Rest "); print_int(cr.n_restaurants - 1);
    print_str(" type="); print_int(type);
    print_str(" avg="); print_int((int)avg_check); print_str("\n");
    return cr.n_restaurants - 1;
}

int cr_add_show(int type, double price, int capacity, double cost) {
    if (cr.n_shows >= MAX_SHOWS) return -1;
    show_t* s = &shows[cr.n_shows];
    s->show_id = cr.n_shows;
    s->type = type;
    s->ticket_price = price;
    s->capacity = capacity;
    s->n_sold = 0;
    s->revenue = 0.0;
    s->production_cost = cost;
    s->active = 1;
    cr.total_costs += cost;
    cr.n_shows++;
    print_str("[CR] Show "); print_int(cr.n_shows - 1);
    print_str(" type="); print_int(type);
    print_str(" $"); print_int((int)price);
    print_str(" cap="); print_int(capacity); print_str("\n");
    return cr.n_shows - 1;
}

int cr_register_player(int type) {
    if (cr.n_players >= MAX_PLAYERS) return -1;
    player_t* p = &players[cr.n_players];
    p->player_id = cr.n_players;
    p->type = type;
    p->total_wagered = 0.0;
    p->total_won = 0.0;
    p->comp_points = 0.0;
    p->active = 1;
    cr.n_players++;
    print_str("[CR] Player "); print_int(cr.n_players - 1);
    print_str(" type="); print_int(type); print_str("\n");
    return cr.n_players - 1;
}

int cr_table_action(int table_id, int player_id, double drop) {
    if (table_id >= cr.n_tables || player_id >= cr.n_players) return -1;
    table_t* t = &tables[table_id];
    double win = drop * t->hold_pct;
    t->drop += drop;
    t->win += win;
    t->n_players++;
    players[player_id].total_wagered += drop;
    players[player_id].total_won += (drop - win);
    players[player_id].comp_points += drop * 0.01;
    cr.table_win += win;
    cr.total_revenue += win;
    print_str("[CR] Tbl"); print_int(table_id);
    print_str(" Pl"); print_int(player_id);
    print_str(" drop="); print_int((int)drop);
    print_str(" win="); print_int((int)win); print_str("\n");
    return 0;
}

int cr_slot_play(int slot_id, int player_id, double coin_in) {
    if (slot_id >= cr.n_slots || player_id >= cr.n_players) return -1;
    slot_t* s = &slots[slot_id];
    double win = coin_in * s->hold_pct;
    s->coin_in += coin_in;
    s->win += win;
    players[player_id].total_wagered += coin_in;
    players[player_id].total_won += (coin_in - win);
    players[player_id].comp_points += coin_in * 0.005;
    cr.slot_win += win;
    cr.total_revenue += win;
    print_str("[CR] Slot"); print_int(slot_id);
    print_str(" Pl"); print_int(player_id);
    print_str(" in="); print_int((int)coin_in);
    print_str(" win="); print_int((int)win); print_str("\n");
    return 0;
}

int cr_book_room(int room_id, int nights) {
    if (room_id >= cr.n_rooms) return -1;
    room_t* r = &rooms[room_id];
    double total = r->nightly_rate * nights;
    r->occupied = 1;
    r->n_stays++;
    r->revenue += total;
    cr.hotel_revenue += total;
    cr.total_revenue += total;
    cr.total_guests++;
    print_str("[CR] Room "); print_int(room_id);
    print_str(" nights="); print_int(nights);
    print_str(" $"); print_int((int)total); print_str("\n");
    return 0;
}

int cr_dine(int rest_id, int n_guests) {
    if (rest_id >= cr.n_restaurants) return -1;
    restaurant_t* r = &restaurants[rest_id];
    double rev = r->avg_check * n_guests;
    double cost = rev * 0.3;
    r->n_guests += n_guests;
    r->revenue += rev;
    r->food_cost += cost;
    cr.dining_revenue += rev;
    cr.total_revenue += rev;
    cr.total_costs += cost;
    print_str("[CR] R"); print_int(rest_id);
    print_str(" guests="); print_int(n_guests);
    print_str(" $"); print_int((int)rev); print_str("\n");
    return 0;
}

int cr_sell_show_tickets(int show_id, int n_sold) {
    if (show_id >= cr.n_shows) return -1;
    show_t* s = &shows[show_id];
    if (s->n_sold + n_sold > s->capacity) return -2;
    s->n_sold += n_sold;
    double rev = s->ticket_price * n_sold;
    s->revenue += rev;
    cr.show_revenue += rev;
    cr.total_revenue += rev;
    print_str("[CR] Sh"); print_int(show_id);
    print_str(" sold "); print_int(n_sold);
    print_str(" $"); print_int((int)rev); print_str("\n");
    return 0;
}

void cr_gaming_report(void) {
    print_str("[CR] Gaming report:\n");
    print_str("  Tables:\n");
    for (int i = 0; i < cr.n_tables; i++) {
        table_t* t = &tables[i];
        print_str("    Tbl"); print_int(i);
        print_str(" type="); print_int(t->type);
        print_str(" drop="); print_int((int)t->drop);
        print_str(" win="); print_int((int)t->win); print_str("\n");
    }
    print_str("  Slots:\n");
    for (int i = 0; i < cr.n_slots; i++) {
        slot_t* s = &slots[i];
        print_str("    Sl"); print_int(i);
        print_str(" type="); print_int(s->type);
        print_str(" in="); print_int((int)s->coin_in);
        print_str(" win="); print_int((int)s->win); print_str("\n");
    }
}

void cr_financial_report(void) {
    print_str("[CR] Financial report:\n");
    print_str("  Table win: "); print_int((int)cr.table_win); print_str("\n");
    print_str("  Slot win: "); print_int((int)cr.slot_win); print_str("\n");
    print_str("  Hotel: "); print_int((int)cr.hotel_revenue); print_str("\n");
    print_str("  Dining: "); print_int((int)cr.dining_revenue); print_str("\n");
    print_str("  Shows: "); print_int((int)cr.show_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)cr.total_revenue); print_str("\n");
    print_str("  Costs: "); print_int((int)cr.total_costs); print_str("\n");
    double profit = cr.total_revenue - cr.total_costs;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    print_str("  Hotel guests: "); print_int(cr.total_guests); print_str("\n");
}

void cr_print_state(void) {
    print_str("[CR] Tables="); print_int(cr.n_tables);
    print_str(" Slots="); print_int(cr.n_slots);
    print_str(" Rooms="); print_int(cr.n_rooms);
    print_str(" Rest="); print_int(cr.n_restaurants);
    print_str(" Shows="); print_int(cr.n_shows);
    print_str(" Players="); print_int(cr.n_players);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)cr.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Casino Resort Demo ===\n\n");
    cr_init();

    print_str("Adding tables...\n");
    cr_add_table(1, 10.0, 500.0, 7, 0.15);
    cr_add_table(1, 25.0, 1000.0, 7, 0.14);
    cr_add_table(2, 5.0, 200.0, 8, 0.05);
    cr_add_table(2, 10.0, 500.0, 8, 0.05);
    cr_add_table(3, 25.0, 2000.0, 14, 0.12);
    cr_add_table(3, 100.0, 5000.0, 14, 0.11);
    cr_add_table(4, 5.0, 100.0, 10, 0.03);
    cr_add_table(4, 10.0, 200.0, 10, 0.03);
    cr_add_table(1, 50.0, 2000.0, 7, 0.13);
    cr_add_table(3, 50.0, 3000.0, 14, 0.12);

    print_str("\nAdding slots...\n");
    cr_add_slot(1, 0.25, 0.08);
    cr_add_slot(1, 1.0, 0.07);
    cr_add_slot(2, 0.25, 0.10);
    cr_add_slot(2, 1.0, 0.09);
    cr_add_slot(3, 0.25, 0.12);
    cr_add_slot(3, 5.0, 0.08);
    cr_add_slot(4, 1.0, 0.06);
    cr_add_slot(4, 5.0, 0.05);
    cr_add_slot(5, 0.25, 0.11);
    cr_add_slot(5, 1.0, 0.07);
    cr_add_slot(1, 0.25, 0.09);
    cr_add_slot(2, 1.0, 0.08);

    print_str("\nAdding rooms...\n");
    cr_add_room(1, 150.0);
    cr_add_room(1, 180.0);
    cr_add_room(2, 250.0);
    cr_add_room(2, 300.0);
    cr_add_room(3, 500.0);
    cr_add_room(3, 650.0);
    cr_add_room(1, 160.0);
    cr_add_room(2, 280.0);
    cr_add_room(3, 550.0);
    cr_add_room(1, 170.0);

    print_str("\nAdding restaurants...\n");
    cr_add_restaurant(1, 80.0);
    cr_add_restaurant(1, 120.0);
    cr_add_restaurant(2, 35.0);
    cr_add_restaurant(2, 25.0);
    cr_add_restaurant(3, 50.0);
    cr_add_restaurant(3, 40.0);
    cr_add_restaurant(4, 15.0);
    cr_add_restaurant(4, 12.0);

    print_str("\nAdding shows...\n");
    cr_add_show(1, 75.0, 1500, 50000.0);
    cr_add_show(2, 50.0, 800, 30000.0);
    cr_add_show(3, 100.0, 2000, 80000.0);
    cr_add_show(1, 60.0, 1200, 40000.0);
    cr_add_show(4, 40.0, 600, 20000.0);
    cr_add_show(2, 55.0, 900, 35000.0);
    cr_add_show(3, 120.0, 2500, 100000.0);
    cr_add_show(4, 35.0, 500, 15000.0);

    print_str("\nRegistering players...\n");
    for (int i = 0; i < 14; i++) {
        cr_register_player((i % 3) + 1);
    }

    print_str("\nTable action...\n");
    for (int i = 0; i < 14; i++) {
        cr_table_action(i % 10, i, 5000.0 + i * 1000.0);
    }

    print_str("\nSlot play...\n");
    for (int i = 0; i < 14; i++) {
        cr_slot_play(i % 12, i, 2000.0 + i * 500.0);
    }

    print_str("\nBooking rooms...\n");
    for (int i = 0; i < 10; i++) {
        cr_book_room(i, 1 + (i % 4));
    }

    print_str("\nDining...\n");
    cr_dine(0, 80);
    cr_dine(1, 50);
    cr_dine(2, 120);
    cr_dine(3, 150);
    cr_dine(4, 90);
    cr_dine(5, 100);
    cr_dine(6, 200);
    cr_dine(7, 250);

    print_str("\nShow tickets...\n");
    cr_sell_show_tickets(0, 1200);
    cr_sell_show_tickets(1, 600);
    cr_sell_show_tickets(2, 1800);
    cr_sell_show_tickets(3, 1000);
    cr_sell_show_tickets(4, 450);
    cr_sell_show_tickets(5, 700);
    cr_sell_show_tickets(6, 2000);
    cr_sell_show_tickets(7, 400);

    print_str("\nGaming report...\n");
    cr_gaming_report();

    print_str("\nFinancial report...\n");
    cr_financial_report();

    print_str("\nFinal state...\n");
    cr_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
