/* arcade_game: Arcade game management (v1.0)
 * Machines, tokens, prizes, maintenance, events, revenue
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

#define MAX_MACHINES   12
#define MAX_PRIZES     10
#define MAX_MEMBERS    8
#define MAX_MAINT      8
#define MAX_EVENTS     6
#define MAX_PLAYS      16

typedef struct {
    int    machine_id;
    int    type;
    int    coin_cost;
    int    total_coins;
    int    total_plays;
    int    status;
    double maint_cost;
    int    active;
} machine_t;

typedef struct {
    int    prize_id;
    int    type;
    int    token_cost;
    int    stock;
    int    n_redeemed;
    double unit_cost;
    int    active;
} prize_t;

typedef struct {
    int    member_id;
    int    tokens;
    int    points;
    int    total_spent;
    int    n_visits;
    int    active;
} member_t;

typedef struct {
    int    maint_id;
    int    machine_id;
    double cost;
    int    day;
    int    active;
} maint_t;

typedef struct {
    int    event_id;
    int    type;
    double entry_fee;
    double prize_pool;
    int    n_participants;
    int    day;
    int    active;
} event_t;

typedef struct {
    int    play_id;
    int    machine_id;
    int    member_id;
    int    coins;
    int    score;
    int    day;
    int    active;
} play_t;

typedef struct {
    int    n_machines;
    int    n_prizes;
    int    n_members;
    int    n_maint;
    int    n_events;
    int    n_plays;
    double coin_revenue;
    double member_revenue;
    double event_revenue;
    double total_revenue;
    double total_costs;
    int    total_coins;
    int    total_plays;
} ag_state_t;

static machine_t machines[MAX_MACHINES];
static prize_t prizes[MAX_PRIZES];
static member_t members[MAX_MEMBERS];
static maint_t maints[MAX_MAINT];
static event_t events[MAX_EVENTS];
static play_t plays[MAX_PLAYS];
static ag_state_t ag;

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

int ag_init(void) {
    if (initialized) return -1;
    ag.n_machines = 0; ag.n_prizes = 0; ag.n_members = 0;
    ag.n_maint = 0; ag.n_events = 0; ag.n_plays = 0;
    ag.coin_revenue = 0.0; ag.member_revenue = 0.0;
    ag.event_revenue = 0.0; ag.total_revenue = 0.0;
    ag.total_costs = 0.0; ag.total_coins = 0; ag.total_plays = 0;
    for (int i = 0; i < MAX_MACHINES; i++) machines[i].active = 0;
    for (int i = 0; i < MAX_PRIZES; i++) prizes[i].active = 0;
    for (int i = 0; i < MAX_MEMBERS; i++) members[i].active = 0;
    for (int i = 0; i < MAX_MAINT; i++) maints[i].active = 0;
    for (int i = 0; i < MAX_EVENTS; i++) events[i].active = 0;
    for (int i = 0; i < MAX_PLAYS; i++) plays[i].active = 0;
    initialized = 1;
    print_str("[AG] Arcade initialized\n");
    return 0;
}

int ag_add_machine(int type, int coin_cost) {
    if (ag.n_machines >= MAX_MACHINES) return -1;
    machine_t* m = &machines[ag.n_machines];
    m->machine_id = ag.n_machines;
    m->type = type;
    m->coin_cost = coin_cost;
    m->total_coins = 0;
    m->total_plays = 0;
    m->status = 1;
    m->maint_cost = 0.0;
    m->active = 1;
    ag.n_machines++;
    print_str("[AG] Machine "); print_int(ag.n_machines - 1);
    print_str(" type="); print_int(type);
    print_str(" coin="); print_int(coin_cost); print_str("\n");
    return ag.n_machines - 1;
}

int ag_add_prize(int type, int token_cost, int stock, double unit_cost) {
    if (ag.n_prizes >= MAX_PRIZES) return -1;
    prize_t* p = &prizes[ag.n_prizes];
    p->prize_id = ag.n_prizes;
    p->type = type;
    p->token_cost = token_cost;
    p->stock = stock;
    p->n_redeemed = 0;
    p->unit_cost = unit_cost;
    p->active = 1;
    ag.n_prizes++;
    print_str("[AG] Prize "); print_int(ag.n_prizes - 1);
    print_str(" type="); print_int(type);
    print_str(" tokens="); print_int(token_cost);
    print_str(" stock="); print_int(stock); print_str("\n");
    return ag.n_prizes - 1;
}

int ag_register_member(int tokens) {
    if (ag.n_members >= MAX_MEMBERS) return -1;
    member_t* m = &members[ag.n_members];
    m->member_id = ag.n_members;
    m->tokens = tokens;
    m->points = 0;
    m->total_spent = tokens;
    m->n_visits = 0;
    m->active = 1;
    double revenue = tokens * 0.25;
    ag.member_revenue += revenue;
    ag.total_revenue += revenue;
    ag.n_members++;
    print_str("[AG] Member "); print_int(ag.n_members - 1);
    print_str(" tokens="); print_int(tokens); print_str("\n");
    return ag.n_members - 1;
}

int ag_play_machine(int machine_id, int member_id, int score, int day) {
    if (ag.n_plays >= MAX_PLAYS) return -1;
    if (machine_id >= ag.n_machines || member_id >= ag.n_members) return -2;
    machine_t* m = &machines[machine_id];
    if (m->status != 1) return -3;
    member_t* mem = &members[member_id];
    if (mem->tokens < m->coin_cost) return -4;
    play_t* p = &plays[ag.n_plays];
    p->play_id = ag.n_plays;
    p->machine_id = machine_id;
    p->member_id = member_id;
    p->coins = m->coin_cost;
    p->score = score;
    p->day = day;
    p->active = 1;
    mem->tokens -= m->coin_cost;
    mem->points += score / 100;
    mem->n_visits++;
    m->total_coins += m->coin_cost;
    m->total_plays++;
    ag.total_coins += m->coin_cost;
    ag.total_plays++;
    ag.n_plays++;
    print_str("[AG] Play "); print_int(ag.n_plays - 1);
    print_str(" M"); print_int(machine_id);
    print_str(" Me"); print_int(member_id);
    print_str(" score="); print_int(score); print_str("\n");
    return ag.n_plays - 1;
}

int ag_walk_in_play(int machine_id, int coins, int score, int day) {
    if (machine_id >= ag.n_machines) return -1;
    machine_t* m = &machines[machine_id];
    if (m->status != 1) return -2;
    m->total_coins += coins;
    m->total_plays++;
    ag.total_coins += coins;
    ag.total_plays++;
    double revenue = coins * 0.25;
    ag.coin_revenue += revenue;
    ag.total_revenue += revenue;
    print_str("[AG] Walk-in M"); print_int(machine_id);
    print_str(" coins="); print_int(coins);
    print_str(" score="); print_int(score); print_str("\n");
    return 0;
}

int ag_redeem_prize(int prize_id, int member_id) {
    if (prize_id >= ag.n_prizes || member_id >= ag.n_members) return -1;
    prize_t* p = &prizes[prize_id];
    member_t* m = &members[member_id];
    if (p->stock <= 0) return -2;
    if (m->tokens < p->token_cost) return -3;
    p->stock--;
    p->n_redeemed++;
    m->tokens -= p->token_cost;
    ag.total_costs += p->unit_cost;
    print_str("[AG] Redeem Pr"); print_int(prize_id);
    print_str(" Me"); print_int(member_id);
    print_str(" tokens="); print_int(p->token_cost); print_str("\n");
    return 0;
}

int ag_schedule_maint(int machine_id, double cost, int day) {
    if (ag.n_maint >= MAX_MAINT || machine_id >= ag.n_machines) return -1;
    maint_t* mt = &maints[ag.n_maint];
    mt->maint_id = ag.n_maint;
    mt->machine_id = machine_id;
    mt->cost = cost;
    mt->day = day;
    mt->active = 1;
    machines[machine_id].status = 2;
    machines[machine_id].maint_cost += cost;
    ag.total_costs += cost;
    ag.n_maint++;
    print_str("[AG] Maint M"); print_int(machine_id);
    print_str(" cost="); print_int((int)cost); print_str("\n");
    return ag.n_maint - 1;
}

int ag_complete_maint(int machine_id) {
    if (machine_id >= ag.n_machines) return -1;
    machines[machine_id].status = 1;
    print_str("[AG] M"); print_int(machine_id); print_str(" maint done\n");
    return 0;
}

int ag_add_event(int type, double entry, double prize_pool, int day) {
    if (ag.n_events >= MAX_EVENTS) return -1;
    event_t* e = &events[ag.n_events];
    e->event_id = ag.n_events;
    e->type = type;
    e->entry_fee = entry;
    e->prize_pool = prize_pool;
    e->n_participants = 0;
    e->day = day;
    e->active = 1;
    ag.n_events++;
    print_str("[AG] Event "); print_int(ag.n_events - 1);
    print_str(" type="); print_int(type);
    print_str(" entry="); print_int((int)entry);
    print_str(" prize="); print_int((int)prize_pool); print_str("\n");
    return ag.n_events - 1;
}

int ag_event_register(int event_id, int n_participants) {
    if (event_id >= ag.n_events) return -1;
    event_t* e = &events[event_id];
    e->n_participants += n_participants;
    double revenue = e->entry_fee * n_participants;
    ag.event_revenue += revenue;
    ag.total_revenue += revenue;
    ag.total_costs += e->prize_pool;
    print_str("[AG] Event "); print_int(event_id);
    print_str(" +"); print_int(n_participants);
    print_str(" participants rev="); print_int((int)revenue); print_str("\n");
    return 0;
}

void ag_machine_report(void) {
    print_str("[AG] Machine report:\n");
    for (int i = 0; i < ag.n_machines; i++) {
        machine_t* m = &machines[i];
        print_str("  M"); print_int(i);
        print_str(" type="); print_int(m->type);
        print_str(" coins="); print_int(m->total_coins);
        print_str(" plays="); print_int(m->total_plays);
        print_str(" status="); print_int(m->status);
        print_str(" maint$="); print_int((int)m->maint_cost); print_str("\n");
    }
}

void ag_financial_report(void) {
    print_str("[AG] Financial report:\n");
    print_str("  Coin revenue: "); print_int((int)ag.coin_revenue); print_str("\n");
    print_str("  Member revenue: "); print_int((int)ag.member_revenue); print_str("\n");
    print_str("  Event revenue: "); print_int((int)ag.event_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)ag.total_revenue); print_str("\n");
    print_str("  Costs: "); print_int((int)ag.total_costs); print_str("\n");
    double profit = ag.total_revenue - ag.total_costs;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    print_str("  Total coins: "); print_int(ag.total_coins); print_str("\n");
    print_str("  Total plays: "); print_int(ag.total_plays); print_str("\n");
}

void ag_print_state(void) {
    print_str("[AG] Machines="); print_int(ag.n_machines);
    print_str(" Prizes="); print_int(ag.n_prizes);
    print_str(" Members="); print_int(ag.n_members);
    print_str(" Events="); print_int(ag.n_events);
    print_str(" Plays="); print_int(ag.n_plays);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)ag.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Arcade Game Demo ===\n\n");
    ag_init();

    print_str("Adding machines...\n");
    ag_add_machine(1, 2);
    ag_add_machine(1, 3);
    ag_add_machine(2, 4);
    ag_add_machine(2, 3);
    ag_add_machine(3, 2);
    ag_add_machine(3, 5);
    ag_add_machine(4, 3);
    ag_add_machine(4, 2);
    ag_add_machine(5, 4);
    ag_add_machine(5, 3);
    ag_add_machine(1, 2);
    ag_add_machine(2, 4);

    print_str("\nAdding prizes...\n");
    ag_add_prize(1, 10, 50, 2.0);
    ag_add_prize(1, 15, 40, 3.0);
    ag_add_prize(2, 25, 30, 5.0);
    ag_add_prize(2, 30, 25, 6.0);
    ag_add_prize(3, 50, 15, 10.0);
    ag_add_prize(3, 60, 12, 12.0);
    ag_add_prize(4, 100, 5, 20.0);
    ag_add_prize(4, 120, 4, 25.0);
    ag_add_prize(1, 8, 60, 1.5);
    ag_add_prize(2, 20, 35, 4.0);

    print_str("\nRegistering members...\n");
    ag_register_member(50);
    ag_register_member(100);
    ag_register_member(75);
    ag_register_member(40);
    ag_register_member(60);
    ag_register_member(80);
    ag_register_member(30);
    ag_register_member(90);

    print_str("\nMember plays...\n");
    ag_play_machine(0, 0, 5000, 10);
    ag_play_machine(1, 1, 8000, 10);
    ag_play_machine(2, 2, 12000, 10);
    ag_play_machine(3, 3, 6000, 11);
    ag_play_machine(4, 4, 4500, 11);
    ag_play_machine(5, 5, 15000, 11);
    ag_play_machine(6, 6, 7500, 12);
    ag_play_machine(7, 7, 9000, 12);
    ag_play_machine(8, 0, 11000, 12);
    ag_play_machine(9, 1, 13000, 13);
    ag_play_machine(10, 2, 5500, 13);
    ag_play_machine(11, 3, 14000, 13);
    ag_play_machine(0, 4, 6500, 14);
    ag_play_machine(2, 5, 16000, 14);
    ag_play_machine(4, 6, 3000, 14);
    ag_play_machine(6, 7, 8500, 15);

    print_str("\nWalk-in plays...\n");
    ag_walk_in_play(0, 10, 4000, 10);
    ag_walk_in_play(1, 15, 7000, 11);
    ag_walk_in_play(2, 20, 10000, 12);
    ag_walk_in_play(3, 12, 5500, 13);
    ag_walk_in_play(4, 8, 3500, 14);
    ag_walk_in_play(5, 25, 18000, 15);
    ag_walk_in_play(6, 10, 6000, 15);
    ag_walk_in_play(7, 14, 8000, 16);

    print_str("\nRedeeming prizes...\n");
    ag_redeem_prize(0, 0);
    ag_redeem_prize(2, 1);
    ag_redeem_prize(4, 2);
    ag_redeem_prize(8, 3);
    ag_redeem_prize(1, 4);
    ag_redeem_prize(3, 5);
    ag_redeem_prize(5, 6);
    ag_redeem_prize(7, 7);
    ag_redeem_prize(0, 1);
    ag_redeem_prize(9, 2);

    print_str("\nMaintenance...\n");
    ag_schedule_maint(0, 150.0, 14);
    ag_schedule_maint(3, 200.0, 15);
    ag_schedule_maint(6, 180.0, 15);
    ag_complete_maint(0);
    ag_complete_maint(3);
    ag_complete_maint(6);

    print_str("\nEvents...\n");
    ag_add_event(1, 10.0, 200.0, 15);
    ag_add_event(2, 15.0, 350.0, 16);
    ag_add_event(1, 10.0, 250.0, 17);
    ag_add_event(3, 20.0, 500.0, 18);
    ag_add_event(2, 15.0, 300.0, 19);
    ag_add_event(1, 10.0, 180.0, 20);
    ag_event_register(0, 12);
    ag_event_register(1, 16);
    ag_event_register(2, 10);
    ag_event_register(3, 8);
    ag_event_register(4, 14);
    ag_event_register(5, 11);

    print_str("\nMachine report...\n");
    ag_machine_report();

    print_str("\nFinancial report...\n");
    ag_financial_report();

    print_str("\nFinal state...\n");
    ag_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
