/* winery: Winery management (v1.0)
 * Vineyards, production, tasting, sales, tours, revenue
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

#define MAX_VINEYARDS 8
#define MAX_WINES    12
#define MAX_TASTINGS 10
#define MAX_TOURS    8
#define MAX_MEMBERS  14

typedef struct {
    int    vineyard_id;
    int    grape_type;
    int    acres;
    int    yield_tons;
    int    year;
    double quality;
    int    active;
} vineyard_t;

typedef struct {
    int    wine_id;
    int    vineyard_id;
    int    type;
    int    vintage;
    int    bottles;
    double price;
    int    sold;
    double revenue;
    double rating;
    int    active;
} wine_t;

typedef struct {
    int    tasting_id;
    int    n_wines;
    double price;
    int    n_guests;
    double revenue;
    int    day;
    int    active;
} tasting_t;

typedef struct {
    int    tour_id;
    int    type;
    int    capacity;
    double price;
    int    n_guests;
    double revenue;
    int    day;
    int    active;
} tour_t;

typedef struct {
    int    member_id;
    int    tier;
    int    n_bottles;
    double total_spent;
    int    points;
    int    active;
} member_t;

typedef struct {
    int    n_vineyards;
    int    n_wines;
    int    n_tastings;
    int    n_tours;
    int    n_members;
    double wine_revenue;
    double tasting_revenue;
    double tour_revenue;
    double event_revenue;
    double total_revenue;
    double total_costs;
    int    total_bottles_sold;
} wr_state_t;

static vineyard_t vineyards[MAX_VINEYARDS];
static wine_t wines[MAX_WINES];
static tasting_t tastings[MAX_TASTINGS];
static tour_t tours[MAX_TOURS];
static member_t members[MAX_MEMBERS];
static wr_state_t wr;

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

int wr_init(void) {
    if (initialized) return -1;
    wr.n_vineyards = 0; wr.n_wines = 0; wr.n_tastings = 0;
    wr.n_tours = 0; wr.n_members = 0;
    wr.wine_revenue = 0.0; wr.tasting_revenue = 0.0;
    wr.tour_revenue = 0.0; wr.event_revenue = 0.0;
    wr.total_revenue = 0.0; wr.total_costs = 0.0;
    wr.total_bottles_sold = 0;
    for (int i = 0; i < MAX_VINEYARDS; i++) vineyards[i].active = 0;
    for (int i = 0; i < MAX_WINES; i++) wines[i].active = 0;
    for (int i = 0; i < MAX_TASTINGS; i++) tastings[i].active = 0;
    for (int i = 0; i < MAX_TOURS; i++) tours[i].active = 0;
    for (int i = 0; i < MAX_MEMBERS; i++) members[i].active = 0;
    initialized = 1;
    print_str("[WR] Winery initialized\n");
    return 0;
}

int wr_add_vineyard(int grape_type, int acres, int yield_tons, int year, double quality) {
    if (wr.n_vineyards >= MAX_VINEYARDS) return -1;
    vineyard_t* v = &vineyards[wr.n_vineyards];
    v->vineyard_id = wr.n_vineyards;
    v->grape_type = grape_type;
    v->acres = acres;
    v->yield_tons = yield_tons;
    v->year = year;
    v->quality = quality;
    v->active = 1;
    wr.n_vineyards++;
    print_str("[WR] Vineyard "); print_int(wr.n_vineyards - 1);
    print_str(" grape="); print_int(grape_type);
    print_str(" acres="); print_int(acres);
    print_str(" yield="); print_int(yield_tons); print_str("t\n");
    return wr.n_vineyards - 1;
}

int wr_add_wine(int vineyard_id, int type, int vintage, int bottles, double price, double rating) {
    if (wr.n_wines >= MAX_WINES || vineyard_id >= wr.n_vineyards) return -1;
    wine_t* w = &wines[wr.n_wines];
    w->wine_id = wr.n_wines;
    w->vineyard_id = vineyard_id;
    w->type = type;
    w->vintage = vintage;
    w->bottles = bottles;
    w->price = price;
    w->sold = 0;
    w->revenue = 0.0;
    w->rating = rating;
    w->active = 1;
    wr.n_wines++;
    print_str("[WR] Wine "); print_int(wr.n_wines - 1);
    print_str(" V"); print_int(vineyard_id);
    print_str(" type="); print_int(type);
    print_str(" vintage="); print_int(vintage);
    print_str(" bottles="); print_int(bottles);
    print_str(" $"); print_int((int)price); print_str("\n");
    return wr.n_wines - 1;
}

int wr_add_tasting(int n_wines, double price, int n_guests, int day) {
    if (wr.n_tastings >= MAX_TASTINGS) return -1;
    tasting_t* t = &tastings[wr.n_tastings];
    t->tasting_id = wr.n_tastings;
    t->n_wines = n_wines;
    t->price = price;
    t->n_guests = n_guests;
    t->revenue = price * n_guests;
    t->day = day;
    t->active = 1;
    wr.tasting_revenue += t->revenue;
    wr.total_revenue += t->revenue;
    wr.n_tastings++;
    print_str("[WR] Tasting "); print_int(wr.n_tastings - 1);
    print_str(" wines="); print_int(n_wines);
    print_str(" guests="); print_int(n_guests);
    print_str(" $"); print_int((int)t->revenue); print_str("\n");
    return wr.n_tastings - 1;
}

int wr_add_tour(int type, int capacity, double price, int n_guests, int day) {
    if (wr.n_tours >= MAX_TOURS) return -1;
    tour_t* tr = &tours[wr.n_tours];
    tr->tour_id = wr.n_tours;
    tr->type = type;
    tr->capacity = capacity;
    tr->price = price;
    tr->n_guests = n_guests;
    tr->revenue = price * n_guests;
    tr->day = day;
    tr->active = 1;
    wr.tour_revenue += tr->revenue;
    wr.total_revenue += tr->revenue;
    wr.n_tours++;
    print_str("[WR] Tour "); print_int(wr.n_tours - 1);
    print_str(" type="); print_int(type);
    print_str(" guests="); print_int(n_guests);
    print_str(" $"); print_int((int)tr->revenue); print_str("\n");
    return wr.n_tours - 1;
}

int wr_register_member(int tier) {
    if (wr.n_members >= MAX_MEMBERS) return -1;
    member_t* m = &members[wr.n_members];
    m->member_id = wr.n_members;
    m->tier = tier;
    m->n_bottles = 0;
    m->total_spent = 0.0;
    m->points = 0;
    m->active = 1;
    wr.n_members++;
    print_str("[WR] Member "); print_int(wr.n_members - 1);
    print_str(" tier="); print_int(tier); print_str("\n");
    return wr.n_members - 1;
}

int wr_sell_wine(int wine_id, int member_id, int bottles) {
    if (wine_id >= wr.n_wines || member_id >= wr.n_members) return -1;
    wine_t* w = &wines[wine_id];
    if (w->bottles - w->sold < bottles) return -2;
    double rev = w->price * bottles;
    w->sold += bottles;
    w->revenue += rev;
    members[member_id].n_bottles += bottles;
    members[member_id].total_spent += rev;
    members[member_id].points += (int)(rev / 10);
    wr.wine_revenue += rev;
    wr.total_revenue += rev;
    wr.total_bottles_sold += bottles;
    print_str("[WR] Sell W"); print_int(wine_id);
    print_str(" Me"); print_int(member_id);
    print_str(" bottles="); print_int(bottles);
    print_str(" $"); print_int((int)rev); print_str("\n");
    return 0;
}

int wr_add_event(double revenue) {
    wr.event_revenue += revenue;
    wr.total_revenue += revenue;
    print_str("[WR] Event +$"); print_int((int)revenue); print_str("\n");
    return 0;
}

void wr_wine_report(void) {
    print_str("[WR] Wine report:\n");
    for (int i = 0; i < wr.n_wines; i++) {
        wine_t* w = &wines[i];
        print_str("  W"); print_int(i);
        print_str(" V"); print_int(w->vineyard_id);
        print_str(" type="); print_int(w->type);
        print_str(" vintage="); print_int(w->vintage);
        print_str(" sold="); print_int(w->sold);
        print_str("/"); print_int(w->bottles);
        print_str(" rev="); print_int((int)w->revenue);
        print_str(" rating="); print_int((int)(w->rating * 10)); print_str("\n");
    }
}

void wr_financial_report(void) {
    print_str("[WR] Financial report:\n");
    print_str("  Wine revenue: "); print_int((int)wr.wine_revenue); print_str("\n");
    print_str("  Tasting revenue: "); print_int((int)wr.tasting_revenue); print_str("\n");
    print_str("  Tour revenue: "); print_int((int)wr.tour_revenue); print_str("\n");
    print_str("  Event revenue: "); print_int((int)wr.event_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)wr.total_revenue); print_str("\n");
    print_str("  Costs: "); print_int((int)wr.total_costs); print_str("\n");
    double profit = wr.total_revenue - wr.total_costs;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    print_str("  Bottles sold: "); print_int(wr.total_bottles_sold); print_str("\n");
}

void wr_print_state(void) {
    print_str("[WR] Vineyards="); print_int(wr.n_vineyards);
    print_str(" Wines="); print_int(wr.n_wines);
    print_str(" Tastings="); print_int(wr.n_tastings);
    print_str(" Tours="); print_int(wr.n_tours);
    print_str(" Members="); print_int(wr.n_members);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)wr.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Winery Demo ===\n\n");
    wr_init();

    print_str("Adding vineyards...\n");
    wr_add_vineyard(1, 20, 80, 2020, 8.5);
    wr_add_vineyard(1, 15, 60, 2021, 8.8);
    wr_add_vineyard(2, 25, 100, 2019, 9.0);
    wr_add_vineyard(2, 18, 70, 2020, 8.6);
    wr_add_vineyard(3, 12, 45, 2021, 9.2);
    wr_add_vineyard(3, 10, 40, 2022, 8.9);
    wr_add_vineyard(4, 30, 120, 2018, 9.5);
    wr_add_vineyard(4, 22, 85, 2020, 9.1);

    print_str("\nAdding wines...\n");
    wr_add_wine(0, 1, 2020, 5000, 45.0, 8.5);
    wr_add_wine(0, 1, 2021, 6000, 48.0, 8.8);
    wr_add_wine(1, 1, 2021, 4500, 52.0, 9.0);
    wr_add_wine(2, 2, 2019, 8000, 65.0, 9.2);
    wr_add_wine(2, 2, 2020, 7500, 68.0, 9.4);
    wr_add_wine(3, 2, 2020, 5500, 55.0, 8.8);
    wr_add_wine(4, 3, 2021, 3000, 85.0, 9.5);
    wr_add_wine(4, 3, 2022, 3500, 90.0, 9.6);
    wr_add_wine(5, 3, 2022, 2800, 80.0, 9.1);
    wr_add_wine(6, 4, 2018, 10000, 120.0, 9.8);
    wr_add_wine(6, 4, 2020, 9000, 110.0, 9.5);
    wr_add_wine(7, 4, 2020, 7000, 95.0, 9.3);

    print_str("\nScheduling tastings...\n");
    wr_add_tasting(4, 35.0, 20, 10);
    wr_add_tasting(5, 40.0, 18, 11);
    wr_add_tasting(3, 30.0, 25, 12);
    wr_add_tasting(6, 50.0, 15, 13);
    wr_add_tasting(4, 35.0, 22, 14);
    wr_add_tasting(5, 42.0, 16, 15);
    wr_add_tasting(3, 28.0, 28, 16);
    wr_add_tasting(6, 55.0, 12, 17);
    wr_add_tasting(4, 38.0, 20, 18);
    wr_add_tasting(5, 45.0, 14, 19);

    print_str("\nScheduling tours...\n");
    wr_add_tour(1, 15, 25.0, 12, 10);
    wr_add_tour(1, 15, 25.0, 14, 11);
    wr_add_tour(2, 10, 45.0, 8, 12);
    wr_add_tour(2, 10, 45.0, 9, 13);
    wr_add_tour(3, 8, 75.0, 6, 14);
    wr_add_tour(1, 15, 25.0, 13, 15);
    wr_add_tour(3, 8, 80.0, 7, 16);
    wr_add_tour(2, 10, 50.0, 10, 17);

    print_str("\nRegistering members...\n");
    for (int i = 0; i < 14; i++) {
        wr_register_member((i % 3) + 1);
    }

    print_str("\nSelling wine...\n");
    for (int i = 0; i < 12; i++) {
        wr_sell_wine(i, i % 14, 50 + (i * 20));
    }

    print_str("\nEvents...\n");
    wr_add_event(15000.0);
    wr_add_event(12000.0);
    wr_add_event(18000.0);
    wr_add_event(10000.0);
    wr_add_event(20000.0);

    print_str("\nWine report...\n");
    wr_wine_report();

    print_str("\nFinancial report...\n");
    wr_financial_report();

    print_str("\nFinal state...\n");
    wr_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
