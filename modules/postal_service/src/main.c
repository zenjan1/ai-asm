/* postal_service: Postal service management (v1.0)
 * Mail, parcels, offices, transport, philately, financial services
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

#define MAX_ITEMS      20
#define MAX_OFFICES    12
#define MAX_ROUTES     10
#define MAX_STAMPS     14
#define MAX_FINANCIAL  12

typedef struct {
    int    item_id;
    int    type;
    int    from_office;
    int    to_office;
    int    weight_g;
    int    postage;
    int    status;
    int    day;
    int    active;
} item_t;

typedef struct {
    int    office_id;
    int    type;
    int    zone;
    int    staff;
    int    daily_volume;
    int    revenue;
    int    active;
} office_t;

typedef struct {
    int    route_id;
    int    type;
    int    from_office;
    int    to_office;
    int    distance_km;
    int    duration_hours;
    int    frequency;
    int    vehicle_id;
    int    active;
} route_t;

typedef struct {
    int    stamp_id;
    int    series;
    int    denomination;
    int    quantity;
    int    issued_year;
    int    sold;
    int    active;
} stamp_t;

typedef struct {
    int    fin_id;
    int    customer_id;
    int    type;
    int    amount;
    int    balance;
    int    year;
    int    month;
    int    active;
} financial_t;

typedef struct {
    int    n_items;
    int    n_offices;
    int    n_routes;
    int    n_stamps;
    int    n_financial;
    int    total_letters;
    int    total_parcels;
    int    total_express;
    int    total_volume;
    int    total_postage;
    int    total_revenue;
    int    total_financial;
    int    total_stamps_sold;
} ps_state_t;

static item_t items[MAX_ITEMS];
static office_t offices[MAX_OFFICES];
static route_t routes[MAX_ROUTES];
static stamp_t stamps[MAX_STAMPS];
static financial_t financials[MAX_FINANCIAL];
static ps_state_t ps;

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

int ps_init(void) {
    if (initialized) return -1;
    ps.n_items = 0; ps.n_offices = 0; ps.n_routes = 0;
    ps.n_stamps = 0; ps.n_financial = 0;
    ps.total_letters = 0; ps.total_parcels = 0; ps.total_express = 0;
    ps.total_volume = 0; ps.total_postage = 0; ps.total_revenue = 0;
    ps.total_financial = 0; ps.total_stamps_sold = 0;
    for (int i = 0; i < MAX_ITEMS; i++) items[i].active = 0;
    for (int i = 0; i < MAX_OFFICES; i++) offices[i].active = 0;
    for (int i = 0; i < MAX_ROUTES; i++) routes[i].active = 0;
    for (int i = 0; i < MAX_STAMPS; i++) stamps[i].active = 0;
    for (int i = 0; i < MAX_FINANCIAL; i++) financials[i].active = 0;
    initialized = 1;
    print_str("[PS] Postal service initialized\n");
    return 0;
}

int ps_add_office(int type, int zone, int staff) {
    if (ps.n_offices >= MAX_OFFICES) return -1;
    office_t* o = &offices[ps.n_offices];
    o->office_id = ps.n_offices;
    o->type = type;
    o->zone = zone;
    o->staff = staff;
    o->daily_volume = 0;
    o->revenue = 0;
    o->active = 1;
    ps.n_offices++;
    print_str("[PS] Office "); print_int(ps.n_offices - 1);
    print_str(" type="); print_int(type);
    print_str(" zone="); print_int(zone);
    print_str(" staff="); print_int(staff); print_str("\n");
    return ps.n_offices - 1;
}

int ps_add_route(int type, int from, int to, int distance, int duration, int frequency, int vehicle) {
    if (ps.n_routes >= MAX_ROUTES || from >= ps.n_offices || to >= ps.n_offices) return -1;
    route_t* r = &routes[ps.n_routes];
    r->route_id = ps.n_routes;
    r->type = type;
    r->from_office = from;
    r->to_office = to;
    r->distance_km = distance;
    r->duration_hours = duration;
    r->frequency = frequency;
    r->vehicle_id = vehicle;
    r->active = 1;
    ps.n_routes++;
    print_str("[PS] Route "); print_int(ps.n_routes - 1);
    print_str(" O"); print_int(from); print_str("->O"); print_int(to);
    print_str(" type="); print_int(type);
    print_str(" km="); print_int(distance);
    print_str(" hrs="); print_int(duration); print_str("\n");
    return ps.n_routes - 1;
}

int ps_accept_item(int type, int from, int to, int weight, int day) {
    if (ps.n_items >= MAX_ITEMS || from >= ps.n_offices || to >= ps.n_offices) return -1;
    item_t* it = &items[ps.n_items];
    it->item_id = ps.n_items;
    it->type = type;
    it->from_office = from;
    it->to_office = to;
    it->weight_g = weight;
    int postage = 5 + weight / 100;
    if (type == 3) postage *= 3;
    else if (type == 2) postage *= 2;
    it->postage = postage;
    it->status = 1;
    it->day = day;
    it->active = 1;
    if (type == 1) ps.total_letters++;
    else if (type == 2) ps.total_parcels++;
    else ps.total_express++;
    ps.total_volume++;
    ps.total_postage += postage;
    ps.total_revenue += postage;
    offices[from].daily_volume++;
    offices[from].revenue += postage;
    ps.n_items++;
    print_str("[PS] Item "); print_int(ps.n_items - 1);
    print_str(" type="); print_int(type);
    print_str(" O"); print_int(from); print_str("->O"); print_int(to);
    print_str(" g="); print_int(weight);
    print_str(" $"); print_int(postage); print_str("\n");
    return ps.n_items - 1;
}

int ps_process_item(int item_id, int status) {
    if (item_id >= ps.n_items) return -1;
    items[item_id].status = status;
    print_str("[PS] Process I"); print_int(item_id);
    print_str(" st="); print_int(status); print_str("\n");
    return 0;
}

int ps_deliver_item(int item_id) {
    if (item_id >= ps.n_items) return -1;
    items[item_id].status = 4;
    print_str("[PS] Deliver I"); print_int(item_id); print_str("\n");
    return 0;
}

int ps_issue_stamp(int series, int denomination, int quantity, int year) {
    if (ps.n_stamps >= MAX_STAMPS) return -1;
    stamp_t* s = &stamps[ps.n_stamps];
    s->stamp_id = ps.n_stamps;
    s->series = series;
    s->denomination = denomination;
    s->quantity = quantity;
    s->issued_year = year;
    s->sold = 0;
    s->active = 1;
    ps.n_stamps++;
    print_str("[PS] Stamp "); print_int(ps.n_stamps - 1);
    print_str(" ser="); print_int(series);
    print_str(" denom="); print_int(denomination);
    print_str(" qty="); print_int(quantity);
    print_str(" "); print_int(year); print_str("\n");
    return ps.n_stamps - 1;
}

int ps_sell_stamp(int stamp_id, int qty) {
    if (stamp_id >= ps.n_stamps) return -1;
    stamps[stamp_id].sold += qty;
    ps.total_stamps_sold += qty;
    ps.total_revenue += stamps[stamp_id].denomination * qty;
    print_str("[PS] Sell S"); print_int(stamp_id);
    print_str(" qty="); print_int(qty); print_str("\n");
    return 0;
}

int ps_financial_op(int customer, int type, int amount, int year, int month) {
    if (ps.n_financial >= MAX_FINANCIAL) return -1;
    financial_t* f = &financials[ps.n_financial];
    f->fin_id = ps.n_financial;
    f->customer_id = customer;
    f->type = type;
    f->amount = amount;
    f->balance = amount;
    f->year = year;
    f->month = month;
    f->active = 1;
    ps.total_financial += amount;
    ps.n_financial++;
    print_str("[PS] Fin "); print_int(ps.n_financial - 1);
    print_str(" cust="); print_int(customer);
    print_str(" type="); print_int(type);
    print_str(" $"); print_int(amount);
    print_str(" "); print_int(year); print_str("-"); print_int(month); print_str("\n");
    return ps.n_financial - 1;
}

void ps_volume_report(void) {
    print_str("[PS] Volume report:\n");
    print_str("  Letters: "); print_int(ps.total_letters); print_str("\n");
    print_str("  Parcels: "); print_int(ps.total_parcels); print_str("\n");
    print_str("  Express: "); print_int(ps.total_express); print_str("\n");
    print_str("  Total: "); print_int(ps.total_volume); print_str("\n");
    print_str("  Postage: "); print_int(ps.total_postage); print_str("\n");
}

void ps_office_report(void) {
    print_str("[PS] Office report:\n");
    for (int i = 0; i < ps.n_offices; i++) {
        office_t* o = &offices[i];
        print_str("  O"); print_int(i);
        print_str(" type="); print_int(o->type);
        print_str(" zone="); print_int(o->zone);
        print_str(" vol="); print_int(o->daily_volume);
        print_str(" rev=$"); print_int(o->revenue);
        print_str(" staff="); print_int(o->staff); print_str("\n");
    }
}

void ps_stamp_report(void) {
    print_str("[PS] Stamp report:\n");
    print_str("  Series: "); print_int(ps.n_stamps); print_str("\n");
    print_str("  Total sold: "); print_int(ps.total_stamps_sold); print_str("\n");
}

void ps_financial_report(void) {
    print_str("[PS] Financial report:\n");
    print_str("  Transactions: "); print_int(ps.n_financial); print_str("\n");
    print_str("  Total amount: "); print_int(ps.total_financial); print_str("\n");
}

void ps_print_state(void) {
    print_str("[PS] Items="); print_int(ps.n_items);
    print_str(" Off="); print_int(ps.n_offices);
    print_str(" Routes="); print_int(ps.n_routes);
    print_str(" Stamps="); print_int(ps.n_stamps);
    print_str(" Fin="); print_int(ps.n_financial);
    print_str("\n");
    print_str("  Revenue: "); print_int(ps.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Postal Service Demo ===\n\n");
    ps_init();

    print_str("Adding offices...\n");
    ps_add_office(1, 1, 15);
    ps_add_office(1, 2, 12);
    ps_add_office(2, 1, 8);
    ps_add_office(2, 3, 6);
    ps_add_office(1, 4, 10);
    ps_add_office(3, 2, 5);
    ps_add_office(1, 3, 14);
    ps_add_office(2, 4, 7);
    ps_add_office(3, 1, 6);
    ps_add_office(1, 2, 11);
    ps_add_office(2, 3, 9);
    ps_add_office(3, 4, 5);

    print_str("\nAdding routes...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 3) + 1;
        int from = i % 12;
        int to = (i + 4) % 12;
        int distance = 50 + (i * 30);
        int duration = 2 + (i % 6);
        int frequency = 1 + (i % 3);
        int vehicle = 100 + i;
        ps_add_route(type, from, to, distance, duration, frequency, vehicle);
    }

    print_str("\nAccepting items...\n");
    for (int i = 0; i < 20; i++) {
        int type = (i % 3) + 1;
        int from = i % 12;
        int to = (i + 5) % 12;
        int weight = 50 + (i * 100);
        int day = 1 + (i % 28);
        ps_accept_item(type, from, to, weight, day);
    }

    print_str("\nProcessing items...\n");
    for (int i = 0; i < 20; i++) {
        ps_process_item(i, 2);
    }

    print_str("\nTransporting items...\n");
    for (int i = 0; i < 20; i++) {
        ps_process_item(i, 3);
    }

    print_str("\nDelivering items...\n");
    for (int i = 0; i < 20; i++) {
        ps_deliver_item(i);
    }

    print_str("\nIssuing stamps...\n");
    for (int i = 0; i < 14; i++) {
        int series = 2020 + (i % 6);
        int denomination = 1 + (i % 10);
        int quantity = 100000 + (i * 50000);
        int year = 2020 + (i % 6);
        ps_issue_stamp(series, denomination, quantity, year);
    }

    print_str("\nSelling stamps...\n");
    for (int i = 0; i < 14; i++) {
        int qty = 1000 + (i * 500);
        ps_sell_stamp(i, qty);
    }

    print_str("\nFinancial operations...\n");
    for (int i = 0; i < 12; i++) {
        int customer = 5000 + i;
        int type = (i % 3) + 1;
        int amount = 1000 + (i * 5000);
        int year = 2024;
        int month = 1 + (i % 12);
        ps_financial_op(customer, type, amount, year, month);
    }

    print_str("\nVolume report...\n");
    ps_volume_report();

    print_str("\nOffice report...\n");
    ps_office_report();

    print_str("\nStamp report...\n");
    ps_stamp_report();

    print_str("\nFinancial report...\n");
    ps_financial_report();

    print_str("\nFinal state...\n");
    ps_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
