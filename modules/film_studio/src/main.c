/* film_studio: Film studio management (v1.0)
 * Productions, actors, crew, stages, post-production, revenue
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

#define MAX_PRODUCTIONS 10
#define MAX_ACTORS     14
#define MAX_CREW       12
#define MAX_STAGES     8
#define MAX_POST       10

typedef struct {
    int    prod_id;
    int    type;
    double budget;
    int    phase;
    int    n_days;
    double box_office;
    double streaming_rev;
    double total_rev;
    int    active;
} production_t;

typedef struct {
    int    actor_id;
    int    tier;
    double salary;
    int    n_productions;
    double total_earned;
    int    guild;
    int    active;
} actor_t;

typedef struct {
    int    crew_id;
    int    dept;
    int    role;
    double daily_rate;
    int    n_days;
    double total_paid;
    int    active;
} crew_t;

typedef struct {
    int    stage_id;
    int    size;
    double daily_rate;
    int    prod_id;
    int    n_days;
    double revenue;
    int    active;
} stage_t;

typedef struct {
    int    post_id;
    int    prod_id;
    int    type;
    int    n_days;
    double cost;
    int    active;
} post_t;

typedef struct {
    int    n_prods;
    int    n_actors;
    int    n_crew;
    int    n_stages;
    int    n_post;
    double box_office;
    double streaming_rev;
    double prod_costs;
    double crew_costs;
    double stage_costs;
    double post_costs;
    double total_revenue;
    double total_costs;
} fs_state_t;

static production_t prods[MAX_PRODUCTIONS];
static actor_t actors[MAX_ACTORS];
static crew_t crew[MAX_CREW];
static stage_t stages[MAX_STAGES];
static post_t posts[MAX_POST];
static fs_state_t fs;

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

int fs_init(void) {
    if (initialized) return -1;
    fs.n_prods = 0; fs.n_actors = 0; fs.n_crew = 0;
    fs.n_stages = 0; fs.n_post = 0;
    fs.box_office = 0.0; fs.streaming_rev = 0.0;
    fs.prod_costs = 0.0; fs.crew_costs = 0.0;
    fs.stage_costs = 0.0; fs.post_costs = 0.0;
    fs.total_revenue = 0.0; fs.total_costs = 0.0;
    for (int i = 0; i < MAX_PRODUCTIONS; i++) prods[i].active = 0;
    for (int i = 0; i < MAX_ACTORS; i++) actors[i].active = 0;
    for (int i = 0; i < MAX_CREW; i++) crew[i].active = 0;
    for (int i = 0; i < MAX_STAGES; i++) stages[i].active = 0;
    for (int i = 0; i < MAX_POST; i++) posts[i].active = 0;
    initialized = 1;
    print_str("[FS] Studio initialized\n");
    return 0;
}

int fs_add_production(int type, double budget) {
    if (fs.n_prods >= MAX_PRODUCTIONS) return -1;
    production_t* p = &prods[fs.n_prods];
    p->prod_id = fs.n_prods;
    p->type = type;
    p->budget = budget;
    p->phase = 1;
    p->n_days = 0;
    p->box_office = 0.0;
    p->streaming_rev = 0.0;
    p->total_rev = 0.0;
    p->active = 1;
    fs.prod_costs += budget;
    fs.total_costs += budget;
    fs.n_prods++;
    print_str("[FS] Prod "); print_int(fs.n_prods - 1);
    print_str(" type="); print_int(type);
    print_str(" budget="); print_int((int)budget); print_str("\n");
    return fs.n_prods - 1;
}

int fs_add_actor(int tier, double salary, int guild) {
    if (fs.n_actors >= MAX_ACTORS) return -1;
    actor_t* a = &actors[fs.n_actors];
    a->actor_id = fs.n_actors;
    a->tier = tier;
    a->salary = salary;
    a->n_productions = 0;
    a->total_earned = 0.0;
    a->guild = guild;
    a->active = 1;
    fs.n_actors++;
    print_str("[FS] Actor "); print_int(fs.n_actors - 1);
    print_str(" tier="); print_int(tier);
    print_str(" $"); print_int((int)salary);
    if (guild) print_str(" [GUILD]");
    print_str("\n");
    return fs.n_actors - 1;
}

int fs_add_crew(int dept, int role, double rate) {
    if (fs.n_crew >= MAX_CREW) return -1;
    crew_t* cr = &crew[fs.n_crew];
    cr->crew_id = fs.n_crew;
    cr->dept = dept;
    cr->role = role;
    cr->daily_rate = rate;
    cr->n_days = 0;
    cr->total_paid = 0.0;
    cr->active = 1;
    fs.n_crew++;
    print_str("[FS] Crew "); print_int(fs.n_crew - 1);
    print_str(" dept="); print_int(dept);
    print_str(" $"); print_int((int)rate); print_str("/day\n");
    return fs.n_crew - 1;
}

int fs_add_stage(int size, double rate) {
    if (fs.n_stages >= MAX_STAGES) return -1;
    stage_t* s = &stages[fs.n_stages];
    s->stage_id = fs.n_stages;
    s->size = size;
    s->daily_rate = rate;
    s->prod_id = -1;
    s->n_days = 0;
    s->revenue = 0.0;
    s->active = 1;
    fs.n_stages++;
    print_str("[FS] Stage "); print_int(fs.n_stages - 1);
    print_str(" sz="); print_int(size);
    print_str(" $"); print_int((int)rate); print_str("/day\n");
    return fs.n_stages - 1;
}

int fs_cast_actor(int prod_id, int actor_id, int days) {
    if (prod_id >= fs.n_prods || actor_id >= fs.n_actors) return -1;
    actor_t* a = &actors[actor_id];
    double pay = a->salary * days / 100.0;
    a->n_productions++;
    a->total_earned += pay;
    fs.crew_costs += pay;
    fs.total_costs += pay;
    print_str("[FS] Cast Ar"); print_int(actor_id);
    print_str(" in Pr"); print_int(prod_id);
    print_str(" days="); print_int(days);
    print_str(" $"); print_int((int)pay); print_str("\n");
    return 0;
}

int fs_hire_crew(int prod_id, int crew_id, int days) {
    if (prod_id >= fs.n_prods || crew_id >= fs.n_crew) return -1;
    crew_t* cr = &crew[crew_id];
    double pay = cr->daily_rate * days;
    cr->n_days += days;
    cr->total_paid += pay;
    fs.crew_costs += pay;
    fs.total_costs += pay;
    print_str("[FS] Hire Cr"); print_int(crew_id);
    print_str(" Pr"); print_int(prod_id);
    print_str(" days="); print_int(days);
    print_str(" $"); print_int((int)pay); print_str("\n");
    return 0;
}

int fs_book_stage(int prod_id, int stage_id, int days) {
    if (prod_id >= fs.n_prods || stage_id >= fs.n_stages) return -1;
    stage_t* s = &stages[stage_id];
    double rev = s->daily_rate * days;
    s->prod_id = prod_id;
    s->n_days += days;
    s->revenue += rev;
    prods[prod_id].n_days += days;
    fs.stage_costs += rev;
    fs.total_costs += rev;
    print_str("[FS] Book Stg"); print_int(stage_id);
    print_str(" Pr"); print_int(prod_id);
    print_str(" days="); print_int(days);
    print_str(" $"); print_int((int)rev); print_str("\n");
    return 0;
}

int fs_add_post(int prod_id, int type, int days, double cost) {
    if (fs.n_post >= MAX_POST || prod_id >= fs.n_prods) return -1;
    post_t* p = &posts[fs.n_post];
    p->post_id = fs.n_post;
    p->prod_id = prod_id;
    p->type = type;
    p->n_days = days;
    p->cost = cost;
    p->active = 1;
    prods[prod_id].phase = 3;
    fs.post_costs += cost;
    fs.total_costs += cost;
    fs.n_post++;
    print_str("[FS] Post "); print_int(fs.n_post - 1);
    print_str(" Pr"); print_int(prod_id);
    print_str(" type="); print_int(type);
    print_str(" days="); print_int(days);
    print_str(" $"); print_int((int)cost); print_str("\n");
    return fs.n_post - 1;
}

int fs_box_office(int prod_id, double amount) {
    if (prod_id >= fs.n_prods) return -1;
    prods[prod_id].box_office += amount;
    prods[prod_id].total_rev += amount;
    fs.box_office += amount;
    fs.total_revenue += amount;
    print_str("[FS] Pr"); print_int(prod_id);
    print_str(" box $"); print_int((int)amount); print_str("\n");
    return 0;
}

int fs_streaming(int prod_id, double amount) {
    if (prod_id >= fs.n_prods) return -1;
    prods[prod_id].streaming_rev += amount;
    prods[prod_id].total_rev += amount;
    fs.streaming_rev += amount;
    fs.total_revenue += amount;
    print_str("[FS] Pr"); print_int(prod_id);
    print_str(" stream $"); print_int((int)amount); print_str("\n");
    return 0;
}

void fs_production_report(void) {
    print_str("[FS] Production report:\n");
    for (int i = 0; i < fs.n_prods; i++) {
        production_t* p = &prods[i];
        print_str("  Pr"); print_int(i);
        print_str(" type="); print_int(p->type);
        print_str(" budget="); print_int((int)p->budget);
        print_str(" days="); print_int(p->n_days);
        print_str(" box="); print_int((int)p->box_office);
        print_str(" stream="); print_int((int)p->streaming_rev);
        print_str(" phase="); print_int(p->phase); print_str("\n");
    }
}

void fs_financial_report(void) {
    print_str("[FS] Financial report:\n");
    print_str("  Box office: "); print_int((int)fs.box_office); print_str("\n");
    print_str("  Streaming: "); print_int((int)fs.streaming_rev); print_str("\n");
    print_str("  Total revenue: "); print_int((int)fs.total_revenue); print_str("\n");
    print_str("  Prod costs: "); print_int((int)fs.prod_costs); print_str("\n");
    print_str("  Crew costs: "); print_int((int)fs.crew_costs); print_str("\n");
    print_str("  Stage costs: "); print_int((int)fs.stage_costs); print_str("\n");
    print_str("  Post costs: "); print_int((int)fs.post_costs); print_str("\n");
    print_str("  Total costs: "); print_int((int)fs.total_costs); print_str("\n");
    double profit = fs.total_revenue - fs.total_costs;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
}

void fs_print_state(void) {
    print_str("[FS] Prods="); print_int(fs.n_prods);
    print_str(" Actors="); print_int(fs.n_actors);
    print_str(" Crew="); print_int(fs.n_crew);
    print_str(" Stages="); print_int(fs.n_stages);
    print_str(" Post="); print_int(fs.n_post);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)fs.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Film Studio Demo ===\n\n");
    fs_init();

    print_str("Adding productions...\n");
    fs_add_production(1, 5000000.0);
    fs_add_production(1, 8000000.0);
    fs_add_production(2, 3000000.0);
    fs_add_production(2, 4500000.0);
    fs_add_production(3, 12000000.0);
    fs_add_production(3, 15000000.0);
    fs_add_production(1, 6000000.0);
    fs_add_production(2, 3500000.0);
    fs_add_production(3, 20000000.0);
    fs_add_production(1, 7000000.0);

    print_str("\nAdding actors...\n");
    fs_add_actor(1, 500000.0, 1);
    fs_add_actor(1, 450000.0, 1);
    fs_add_actor(2, 250000.0, 1);
    fs_add_actor(2, 200000.0, 1);
    fs_add_actor(3, 100000.0, 0);
    fs_add_actor(3, 80000.0, 0);
    fs_add_actor(1, 480000.0, 1);
    fs_add_actor(2, 220000.0, 1);
    fs_add_actor(3, 90000.0, 0);
    fs_add_actor(1, 520000.0, 1);
    fs_add_actor(2, 240000.0, 1);
    fs_add_actor(3, 110000.0, 0);
    fs_add_actor(1, 460000.0, 1);
    fs_add_actor(2, 210000.0, 1);

    print_str("\nAdding crew...\n");
    fs_add_crew(1, 1, 5000.0);
    fs_add_crew(1, 2, 3000.0);
    fs_add_crew(2, 1, 2500.0);
    fs_add_crew(2, 2, 1800.0);
    fs_add_crew(3, 1, 2000.0);
    fs_add_crew(3, 2, 1500.0);
    fs_add_crew(4, 1, 2200.0);
    fs_add_crew(4, 2, 1600.0);
    fs_add_crew(5, 1, 1800.0);
    fs_add_crew(5, 2, 1200.0);
    fs_add_crew(1, 3, 2800.0);
    fs_add_crew(2, 3, 1600.0);

    print_str("\nAdding stages...\n");
    fs_add_stage(1, 15000.0);
    fs_add_stage(1, 12000.0);
    fs_add_stage(2, 8000.0);
    fs_add_stage(2, 7000.0);
    fs_add_stage(3, 5000.0);
    fs_add_stage(3, 4500.0);
    fs_add_stage(1, 14000.0);
    fs_add_stage(2, 7500.0);

    print_str("\nCasting actors...\n");
    for (int i = 0; i < 14; i++) {
        fs_cast_actor(i % 10, i, 30 + (i % 4) * 10);
    }

    print_str("\nHiring crew...\n");
    for (int i = 0; i < 12; i++) {
        fs_hire_crew(i % 10, i, 40 + (i % 3) * 15);
    }

    print_str("\nBooking stages...\n");
    for (int i = 0; i < 8; i++) {
        fs_book_stage(i % 10, i, 50 + (i % 4) * 20);
    }

    print_str("\nPost-production...\n");
    fs_add_post(0, 1, 30, 500000.0);
    fs_add_post(1, 2, 60, 1200000.0);
    fs_add_post(2, 1, 25, 350000.0);
    fs_add_post(3, 3, 45, 800000.0);
    fs_add_post(4, 2, 90, 2000000.0);
    fs_add_post(5, 2, 100, 2500000.0);
    fs_add_post(6, 1, 35, 600000.0);
    fs_add_post(7, 3, 40, 700000.0);
    fs_add_post(8, 2, 120, 3000000.0);
    fs_add_post(9, 1, 30, 450000.0);

    print_str("\nBox office...\n");
    fs_box_office(0, 25000000.0);
    fs_box_office(1, 45000000.0);
    fs_box_office(2, 18000000.0);
    fs_box_office(3, 30000000.0);
    fs_box_office(4, 80000000.0);
    fs_box_office(5, 120000000.0);
    fs_box_office(6, 35000000.0);
    fs_box_office(7, 22000000.0);
    fs_box_office(8, 150000000.0);
    fs_box_office(9, 40000000.0);

    print_str("\nStreaming revenue...\n");
    fs_streaming(0, 8000000.0);
    fs_streaming(1, 15000000.0);
    fs_streaming(2, 6000000.0);
    fs_streaming(3, 10000000.0);
    fs_streaming(4, 30000000.0);
    fs_streaming(5, 45000000.0);
    fs_streaming(6, 12000000.0);
    fs_streaming(7, 7500000.0);
    fs_streaming(8, 50000000.0);
    fs_streaming(9, 13000000.0);

    print_str("\nProduction report...\n");
    fs_production_report();

    print_str("\nFinancial report...\n");
    fs_financial_report();

    print_str("\nFinal state...\n");
    fs_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
