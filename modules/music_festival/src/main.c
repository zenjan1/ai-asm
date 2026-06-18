/* music_festival: Music festival management (v1.0)
 * Stages, artists, tickets, vendors, security, revenue
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

#define MAX_STAGES    6
#define MAX_ARTISTS   14
#define MAX_TICKETS   20
#define MAX_VENDORS   10
#define MAX_SECURITY  8
#define MAX_SETS      16

typedef struct {
    int    stage_id;
    int    type;
    int    capacity;
    double sound_budget;
    int    n_sets;
    double revenue;
    int    active;
} stage_t;

typedef struct {
    int    artist_id;
    int    type;
    int    genre;
    double fee;
    double travel_cost;
    double lodging_cost;
    int    stage_id;
    int    day;
    int    duration_min;
    int    n_attendees;
    int    active;
} artist_t;

typedef struct {
    int    ticket_id;
    int    type;
    double price;
    int    day;
    int    active;
} ticket_t;

typedef struct {
    int    vendor_id;
    int    type;
    double booth_fee;
    double sales;
    double commission_pct;
    int    active;
} vendor_t;

typedef struct {
    int    sec_id;
    int    zone;
    int    n_guards;
    double hourly_rate;
    int    hours;
    double total_cost;
    int    active;
} security_t;

typedef struct {
    int    set_id;
    int    artist_id;
    int    stage_id;
    int    day;
    int    start_hour;
    int    duration_min;
    int    n_attendees;
    int    active;
} set_t;

typedef struct {
    int    n_stages;
    int    n_artists;
    int    n_tickets;
    int    n_vendors;
    int    n_security;
    int    n_sets;
    double ticket_revenue;
    double vendor_commission;
    double sponsorship;
    double total_revenue;
    double artist_costs;
    double security_costs;
    double stage_costs;
    double ops_costs;
    int    total_attendance;
} mf_state_t;

static stage_t stages[MAX_STAGES];
static artist_t artists[MAX_ARTISTS];
static ticket_t tickets[MAX_TICKETS];
static vendor_t vendors[MAX_VENDORS];
static security_t security[MAX_SECURITY];
static set_t sets[MAX_SETS];
static mf_state_t mf;

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

int mf_init(void) {
    if (initialized) return -1;
    mf.n_stages = 0; mf.n_artists = 0; mf.n_tickets = 0;
    mf.n_vendors = 0; mf.n_security = 0; mf.n_sets = 0;
    mf.ticket_revenue = 0.0; mf.vendor_commission = 0.0;
    mf.sponsorship = 0.0; mf.total_revenue = 0.0;
    mf.artist_costs = 0.0; mf.security_costs = 0.0;
    mf.stage_costs = 0.0; mf.ops_costs = 0.0;
    mf.total_attendance = 0;
    for (int i = 0; i < MAX_STAGES; i++) stages[i].active = 0;
    for (int i = 0; i < MAX_ARTISTS; i++) artists[i].active = 0;
    for (int i = 0; i < MAX_TICKETS; i++) tickets[i].active = 0;
    for (int i = 0; i < MAX_VENDORS; i++) vendors[i].active = 0;
    for (int i = 0; i < MAX_SECURITY; i++) security[i].active = 0;
    for (int i = 0; i < MAX_SETS; i++) sets[i].active = 0;
    initialized = 1;
    print_str("[MF] Festival initialized\n");
    return 0;
}

int mf_add_stage(int type, int capacity, double sound_budget) {
    if (mf.n_stages >= MAX_STAGES) return -1;
    stage_t* s = &stages[mf.n_stages];
    s->stage_id = mf.n_stages;
    s->type = type;
    s->capacity = capacity;
    s->sound_budget = sound_budget;
    s->n_sets = 0;
    s->revenue = 0.0;
    s->active = 1;
    mf.stage_costs += sound_budget;
    mf.total_revenue -= sound_budget;
    mf.n_stages++;
    print_str("[MF] Stage "); print_int(mf.n_stages - 1);
    print_str(" type="); print_int(type);
    print_str(" cap="); print_int(capacity);
    print_str(" sound$="); print_int((int)sound_budget); print_str("\n");
    return mf.n_stages - 1;
}

int mf_book_artist(int type, int genre, double fee, double travel, double lodging) {
    if (mf.n_artists >= MAX_ARTISTS) return -1;
    artist_t* a = &artists[mf.n_artists];
    a->artist_id = mf.n_artists;
    a->type = type;
    a->genre = genre;
    a->fee = fee;
    a->travel_cost = travel;
    a->lodging_cost = lodging;
    a->stage_id = -1;
    a->day = 0;
    a->duration_min = 0;
    a->n_attendees = 0;
    a->active = 1;
    double total_cost = fee + travel + lodging;
    mf.artist_costs += total_cost;
    mf.n_artists++;
    print_str("[MF] Artist "); print_int(mf.n_artists - 1);
    print_str(" type="); print_int(type);
    print_str(" genre="); print_int(genre);
    print_str(" fee="); print_int((int)fee); print_str("\n");
    return mf.n_artists - 1;
}

int mf_add_ticket(int type, double price, int day) {
    if (mf.n_tickets >= MAX_TICKETS) return -1;
    ticket_t* t = &tickets[mf.n_tickets];
    t->ticket_id = mf.n_tickets;
    t->type = type;
    t->price = price;
    t->day = day;
    t->active = 1;
    mf.ticket_revenue += price;
    mf.total_revenue += price;
    mf.total_attendance++;
    mf.n_tickets++;
    print_str("[MF] Ticket "); print_int(mf.n_tickets - 1);
    print_str(" type="); print_int(type);
    print_str(" price="); print_int((int)price);
    print_str(" day="); print_int(day); print_str("\n");
    return mf.n_tickets - 1;
}

int mf_add_vendor(int type, double booth_fee, double sales, double commission) {
    if (mf.n_vendors >= MAX_VENDORS) return -1;
    vendor_t* v = &vendors[mf.n_vendors];
    v->vendor_id = mf.n_vendors;
    v->type = type;
    v->booth_fee = booth_fee;
    v->sales = sales;
    v->commission_pct = commission;
    v->active = 1;
    double commission_amt = sales * commission / 100.0;
    mf.vendor_commission += commission_amt;
    mf.total_revenue += booth_fee + commission_amt;
    mf.n_vendors++;
    print_str("[MF] Vendor "); print_int(mf.n_vendors - 1);
    print_str(" type="); print_int(type);
    print_str(" booth="); print_int((int)booth_fee);
    print_str(" comm="); print_int((int)commission_amt); print_str("\n");
    return mf.n_vendors - 1;
}

int mf_add_security(int zone, int guards, double rate, int hours) {
    if (mf.n_security >= MAX_SECURITY) return -1;
    security_t* s = &security[mf.n_security];
    s->sec_id = mf.n_security;
    s->zone = zone;
    s->n_guards = guards;
    s->hourly_rate = rate;
    s->hours = hours;
    s->total_cost = guards * rate * hours;
    s->active = 1;
    mf.security_costs += s->total_cost;
    mf.n_security++;
    print_str("[MF] Security Z"); print_int(zone);
    print_str(" guards="); print_int(guards);
    print_str(" hrs="); print_int(hours);
    print_str(" cost="); print_int((int)s->total_cost); print_str("\n");
    return mf.n_security - 1;
}

int mf_schedule_set(int artist_id, int stage_id, int day, int start_hour, int duration) {
    if (mf.n_sets >= MAX_SETS) return -1;
    if (artist_id >= mf.n_artists || stage_id >= mf.n_stages) return -2;
    set_t* s = &sets[mf.n_sets];
    s->set_id = mf.n_sets;
    s->artist_id = artist_id;
    s->stage_id = stage_id;
    s->day = day;
    s->start_hour = start_hour;
    s->duration_min = duration;
    s->n_attendees = 0;
    s->active = 1;
    artist_t* a = &artists[artist_id];
    a->stage_id = stage_id;
    a->day = day;
    a->duration_min = duration;
    stage_t* st = &stages[stage_id];
    st->n_sets++;
    mf.n_sets++;
    print_str("[MF] Set "); print_int(mf.n_sets - 1);
    print_str(" Ar"); print_int(artist_id);
    print_str(" St"); print_int(stage_id);
    print_str(" D"); print_int(day);
    print_str(" @"); print_int(start_hour); print_str("h");
    print_str(" dur="); print_int(duration); print_str("min\n");
    return mf.n_sets - 1;
}

int mf_set_attendance(int set_id, int attendees) {
    if (set_id >= mf.n_sets) return -1;
    sets[set_id].n_attendees = attendees;
    artists[sets[set_id].artist_id].n_attendees += attendees;
    stages[sets[set_id].stage_id].revenue += attendees * 5;
    return 0;
}

int mf_add_sponsorship(double amount) {
    mf.sponsorship += amount;
    mf.total_revenue += amount;
    print_str("[MF] Sponsorship +"); print_int((int)amount); print_str("\n");
    return 0;
}

int mf_add_ops_cost(double amount) {
    mf.ops_costs += amount;
    print_str("[MF] Ops cost +"); print_int((int)amount); print_str("\n");
    return 0;
}

void mf_schedule_report(void) {
    print_str("[MF] Schedule report:\n");
    for (int i = 0; i < mf.n_sets; i++) {
        set_t* s = &sets[i];
        print_str("  Set "); print_int(i);
        print_str(" Ar"); print_int(s->artist_id);
        print_str(" St"); print_int(s->stage_id);
        print_str(" D"); print_int(s->day);
        print_str(" @"); print_int(s->start_hour); print_str("h");
        print_str(" dur="); print_int(s->duration_min); print_str("min");
        print_str(" att="); print_int(s->n_attendees); print_str("\n");
    }
}

void mf_financial_report(void) {
    print_str("[MF] Financial report:\n");
    print_str("  Ticket revenue: "); print_int((int)mf.ticket_revenue); print_str("\n");
    print_str("  Vendor commission: "); print_int((int)mf.vendor_commission); print_str("\n");
    print_str("  Sponsorship: "); print_int((int)mf.sponsorship); print_str("\n");
    print_str("  Total revenue: "); print_int((int)mf.total_revenue); print_str("\n");
    print_str("  Artist costs: "); print_int((int)mf.artist_costs); print_str("\n");
    print_str("  Security costs: "); print_int((int)mf.security_costs); print_str("\n");
    print_str("  Stage costs: "); print_int((int)mf.stage_costs); print_str("\n");
    print_str("  Ops costs: "); print_int((int)mf.ops_costs); print_str("\n");
    double total_costs = mf.artist_costs + mf.security_costs + mf.stage_costs + mf.ops_costs;
    double profit = mf.total_revenue - total_costs;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    print_str("  Total attendance: "); print_int(mf.total_attendance); print_str("\n");
}

void mf_print_state(void) {
    print_str("[MF] Stages="); print_int(mf.n_stages);
    print_str(" Artists="); print_int(mf.n_artists);
    print_str(" Tickets="); print_int(mf.n_tickets);
    print_str(" Vendors="); print_int(mf.n_vendors);
    print_str(" Security="); print_int(mf.n_security);
    print_str(" Sets="); print_int(mf.n_sets);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)mf.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Music Festival Demo ===\n\n");
    mf_init();

    print_str("Adding stages...\n");
    mf_add_stage(1, 20000, 150000.0);
    mf_add_stage(2, 8000, 80000.0);
    mf_add_stage(3, 5000, 60000.0);
    mf_add_stage(4, 3000, 45000.0);
    mf_add_stage(1, 15000, 120000.0);
    mf_add_stage(2, 6000, 70000.0);

    print_str("\nBooking artists...\n");
    mf_book_artist(1, 1, 500000.0, 20000.0, 15000.0);
    mf_book_artist(1, 2, 350000.0, 15000.0, 12000.0);
    mf_book_artist(2, 1, 200000.0, 10000.0, 8000.0);
    mf_book_artist(2, 3, 180000.0, 8000.0, 7000.0);
    mf_book_artist(3, 2, 150000.0, 5000.0, 5000.0);
    mf_book_artist(3, 4, 120000.0, 4000.0, 4000.0);
    mf_book_artist(1, 1, 400000.0, 18000.0, 14000.0);
    mf_book_artist(2, 3, 220000.0, 12000.0, 9000.0);
    mf_book_artist(3, 2, 130000.0, 5000.0, 4500.0);
    mf_book_artist(1, 4, 300000.0, 14000.0, 11000.0);
    mf_book_artist(2, 1, 170000.0, 7000.0, 6000.0);
    mf_book_artist(3, 2, 100000.0, 3000.0, 3500.0);
    mf_book_artist(1, 3, 450000.0, 16000.0, 13000.0);
    mf_book_artist(2, 4, 160000.0, 6000.0, 5500.0);

    print_str("\nScheduling sets...\n");
    mf_schedule_set(0, 0, 1, 20, 90);
    mf_schedule_set(2, 1, 1, 18, 60);
    mf_schedule_set(4, 2, 1, 16, 60);
    mf_schedule_set(6, 0, 1, 22, 75);
    mf_schedule_set(1, 0, 2, 20, 90);
    mf_schedule_set(3, 1, 2, 18, 60);
    mf_schedule_set(5, 3, 2, 15, 45);
    mf_schedule_set(7, 4, 2, 21, 75);
    mf_schedule_set(8, 2, 3, 16, 60);
    mf_schedule_set(9, 4, 3, 20, 90);
    mf_schedule_set(10, 1, 3, 18, 60);
    mf_schedule_set(11, 3, 3, 14, 45);
    mf_schedule_set(12, 0, 3, 22, 80);
    mf_schedule_set(13, 5, 3, 17, 60);
    mf_schedule_set(0, 0, 4, 21, 120);
    mf_schedule_set(6, 4, 4, 19, 90);

    print_str("\nSetting attendance...\n");
    mf_set_attendance(0, 18000);
    mf_set_attendance(1, 7000);
    mf_set_attendance(2, 4500);
    mf_set_attendance(3, 16000);
    mf_set_attendance(4, 19000);
    mf_set_attendance(5, 6500);
    mf_set_attendance(6, 2800);
    mf_set_attendance(7, 13000);
    mf_set_attendance(8, 4200);
    mf_set_attendance(9, 14000);
    mf_set_attendance(10, 7500);
    mf_set_attendance(11, 2500);
    mf_set_attendance(12, 17000);
    mf_set_attendance(13, 5500);
    mf_set_attendance(14, 20000);
    mf_set_attendance(15, 14500);

    print_str("\nSelling tickets...\n");
    for (int i = 0; i < 20; i++) {
        int type = (i % 4) + 1;
        double price;
        if (type == 1) price = 150.0;
        else if (type == 2) price = 350.0;
        else if (type == 3) price = 500.0;
        else price = 80.0;
        int day = (i % 4) + 1;
        mf_add_ticket(type, price, day);
    }

    print_str("\nAdding vendors...\n");
    mf_add_vendor(1, 2000.0, 25000.0, 10.0);
    mf_add_vendor(1, 1800.0, 22000.0, 10.0);
    mf_add_vendor(2, 2500.0, 40000.0, 12.0);
    mf_add_vendor(2, 2200.0, 35000.0, 12.0);
    mf_add_vendor(3, 1500.0, 15000.0, 8.0);
    mf_add_vendor(3, 1200.0, 12000.0, 8.0);
    mf_add_vendor(4, 3000.0, 50000.0, 15.0);
    mf_add_vendor(1, 1800.0, 20000.0, 10.0);
    mf_add_vendor(2, 2000.0, 30000.0, 12.0);
    mf_add_vendor(4, 2500.0, 45000.0, 15.0);

    print_str("\nSecurity deployment...\n");
    mf_add_security(1, 20, 25.0, 48);
    mf_add_security(2, 12, 25.0, 48);
    mf_add_security(3, 8, 25.0, 48);
    mf_add_security(4, 10, 25.0, 48);
    mf_add_security(1, 15, 30.0, 36);
    mf_add_security(2, 10, 30.0, 36);
    mf_add_security(3, 6, 30.0, 36);
    mf_add_security(4, 8, 30.0, 36);

    print_str("\nSponsorships...\n");
    mf_add_sponsorship(500000.0);
    mf_add_sponsorship(300000.0);
    mf_add_sponsorship(200000.0);

    print_str("\nOps costs...\n");
    mf_add_ops_cost(100000.0);
    mf_add_ops_cost(75000.0);
    mf_add_ops_cost(50000.0);

    print_str("\nSchedule report...\n");
    mf_schedule_report();

    print_str("\nFinancial report...\n");
    mf_financial_report();

    print_str("\nFinal state...\n");
    mf_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
