/* real_estate: Real estate management system (v1.0)
 * Property listing, sales management, leasing, market analysis, appraisal
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

#define MAX_PROPERTIES  16
#define MAX_CLIENTS     12
#define MAX_LISTINGS    12
#define MAX_LEASES      10
#define MAX_VIEWINGS    12
#define MAX_APPRAISALS  8

typedef struct {
    int    property_id;
    int    type;
    int    area_sqm;
    int    bedrooms;
    int    bathrooms;
    double price;
    int    year_built;
    int    status;
    int    active;
} property_t;

typedef struct {
    int    client_id;
    int    type;
    double budget;
    int    min_area;
    int    preferences;
    int    n_viewings;
    int    status;
    int    active;
} client_t;

typedef struct {
    int    listing_id;
    int    property_id;
    double asking_price;
    int    days_on_market;
    int    n_views;
    int    n_inquiries;
    int    status;
    int    active;
} listing_t;

typedef struct {
    int    lease_id;
    int    property_id;
    int    tenant_id;
    double monthly_rent;
    int    deposit;
    int    term_months;
    int    start_month;
    int    status;
    int    active;
} lease_t;

typedef struct {
    int    viewing_id;
    int    property_id;
    int    client_id;
    int    day;
    int    duration_min;
    int    feedback_score;
    int    active;
} viewing_t;

typedef struct {
    int    appraisal_id;
    int    property_id;
    double market_value;
    double assessed_value;
    int    confidence;
    int    active;
} appraisal_t;

typedef struct {
    int    n_properties;
    int    n_clients;
    int    n_listings;
    int    n_leases;
    int    n_viewings;
    int    n_appraisals;
    double total_sales_volume;
    double total_rental_income;
    double total_commission;
    int    properties_sold;
    int    properties_leased;
} re_state_t;

static property_t properties[MAX_PROPERTIES];
static client_t clients[MAX_CLIENTS];
static listing_t listings[MAX_LISTINGS];
static lease_t leases[MAX_LEASES];
static viewing_t viewings[MAX_VIEWINGS];
static appraisal_t appraisals[MAX_APPRAISALS];
static re_state_t re;

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

int re_init(void) {
    if (initialized) return -1;
    re.n_properties = 0; re.n_clients = 0; re.n_listings = 0;
    re.n_leases = 0; re.n_viewings = 0; re.n_appraisals = 0;
    re.total_sales_volume = 0.0; re.total_rental_income = 0.0;
    re.total_commission = 0.0; re.properties_sold = 0; re.properties_leased = 0;
    for (int i = 0; i < MAX_PROPERTIES; i++) properties[i].active = 0;
    for (int i = 0; i < MAX_CLIENTS; i++) clients[i].active = 0;
    for (int i = 0; i < MAX_LISTINGS; i++) listings[i].active = 0;
    for (int i = 0; i < MAX_LEASES; i++) leases[i].active = 0;
    for (int i = 0; i < MAX_VIEWINGS; i++) viewings[i].active = 0;
    for (int i = 0; i < MAX_APPRAISALS; i++) appraisals[i].active = 0;
    initialized = 1;
    print_str("[RE] Real estate initialized\n");
    return 0;
}

int re_add_property(int type, int area, int beds, int baths, double price, int year) {
    if (re.n_properties >= MAX_PROPERTIES) return -1;
    property_t* p = &properties[re.n_properties];
    p->property_id = re.n_properties;
    p->type = type;
    p->area_sqm = area;
    p->bedrooms = beds;
    p->bathrooms = baths;
    p->price = price;
    p->year_built = year;
    p->status = 1;
    p->active = 1;
    re.n_properties++;
    print_str("[RE] Property "); print_int(re.n_properties - 1);
    print_str(" type="); print_int(type);
    print_str(" "); print_int(area); print_str("sqm");
    print_str(" "); print_int(beds); print_str("BR/"); print_int(baths); print_str("BA");
    print_str(" price="); print_int((int)price);
    print_str(" year="); print_int(year); print_str("\n");
    return re.n_properties - 1;
}

int re_register_client(int type, double budget, int min_area) {
    if (re.n_clients >= MAX_CLIENTS) return -1;
    client_t* c = &clients[re.n_clients];
    c->client_id = re.n_clients;
    c->type = type;
    c->budget = budget;
    c->min_area = min_area;
    c->preferences = 0;
    c->n_viewings = 0;
    c->status = 1;
    c->active = 1;
    re.n_clients++;
    print_str("[RE] Client "); print_int(re.n_clients - 1);
    print_str(" type="); print_int(type);
    print_str(" budget="); print_int((int)budget);
    print_str(" min="); print_int(min_area); print_str("sqm\n");
    return re.n_clients - 1;
}

int re_create_listing(int property_id, double asking_price) {
    if (re.n_listings >= MAX_LISTINGS || property_id >= re.n_properties) return -1;
    listing_t* l = &listings[re.n_listings];
    l->listing_id = re.n_listings;
    l->property_id = property_id;
    l->asking_price = asking_price;
    l->days_on_market = 0;
    l->n_views = 0;
    l->n_inquiries = 0;
    l->status = 1;
    l->active = 1;
    re.n_listings++;
    properties[property_id].status = 2;
    print_str("[RE] Listing "); print_int(re.n_listings - 1);
    print_str(" P"); print_int(property_id);
    print_str(" asking="); print_int((int)asking_price); print_str("\n");
    return re.n_listings - 1;
}

void re_listing_activity(int listing_id, int views, int inquiries) {
    if (listing_id >= re.n_listings) return;
    listings[listing_id].n_views += views;
    listings[listing_id].n_inquiries += inquiries;
    listings[listing_id].days_on_market += 7;
    print_str("[RE] Listing "); print_int(listing_id);
    print_str(" views="); print_int(listings[listing_id].n_views);
    print_str(" inquiries="); print_int(listings[listing_id].n_inquiries);
    print_str(" days="); print_int(listings[listing_id].days_on_market); print_str("\n");
}

int re_schedule_viewing(int property_id, int client_id, int day) {
    if (re.n_viewings >= MAX_VIEWINGS) return -1;
    if (property_id >= re.n_properties || client_id >= re.n_clients) return -1;
    viewing_t* v = &viewings[re.n_viewings];
    v->viewing_id = re.n_viewings;
    v->property_id = property_id;
    v->client_id = client_id;
    v->day = day;
    v->duration_min = 30;
    v->feedback_score = 0;
    v->active = 1;
    re.n_viewings++;
    clients[client_id].n_viewings++;
    print_str("[RE] Viewing "); print_int(re.n_viewings - 1);
    print_str(" P"); print_int(property_id);
    print_str(" C"); print_int(client_id);
    print_str(" day="); print_int(day); print_str("\n");
    return re.n_viewings - 1;
}

void re_viewing_feedback(int viewing_id, int score) {
    if (viewing_id >= re.n_viewings) return;
    viewings[viewing_id].feedback_score = score;
    print_str("[RE] Viewing "); print_int(viewing_id);
    print_str(" feedback="); print_int(score); print_str("/10\n");
}

int re_close_sale(int listing_id, int client_id, double sale_price) {
    if (listing_id >= re.n_listings || client_id >= re.n_clients) return -1;
    listing_t* l = &listings[listing_id];
    l->status = 3;
    properties[l->property_id].status = 3;
    clients[client_id].status = 3;
    double commission = sale_price * 0.025;
    re.total_sales_volume += sale_price;
    re.total_commission += commission;
    re.properties_sold++;
    print_str("[RE] SALE CLOSED L"); print_int(listing_id);
    print_str(" C"); print_int(client_id);
    print_str(" price="); print_int((int)sale_price);
    print_str(" commission="); print_int((int)commission); print_str("\n");
    return 0;
}

int re_create_lease(int property_id, int tenant_id, double rent, int deposit, int term) {
    if (re.n_leases >= MAX_LEASES) return -1;
    if (property_id >= re.n_properties || tenant_id >= re.n_clients) return -1;
    lease_t* ls = &leases[re.n_leases];
    ls->lease_id = re.n_leases;
    ls->property_id = property_id;
    ls->tenant_id = tenant_id;
    ls->monthly_rent = rent;
    ls->deposit = deposit;
    ls->term_months = term;
    ls->start_month = 1;
    ls->status = 1;
    ls->active = 1;
    re.n_leases++;
    re.total_rental_income += rent * term;
    re.properties_leased++;
    properties[property_id].status = 4;
    print_str("[RE] Lease "); print_int(re.n_leases - 1);
    print_str(" P"); print_int(property_id);
    print_str(" T"); print_int(tenant_id);
    print_str(" rent="); print_int((int)rent); print_str("/mo");
    print_str(" dep="); print_int(deposit);
    print_str(" term="); print_int(term); print_str("mo\n");
    return re.n_leases - 1;
}

int re_appraise_property(int property_id) {
    if (re.n_appraisals >= MAX_APPRAISALS || property_id >= re.n_properties) return -1;
    appraisal_t* a = &appraisals[re.n_appraisals];
    a->appraisal_id = re.n_appraisals;
    a->property_id = property_id;
    property_t* p = &properties[property_id];
    double base = (double)p->area_sqm * 5000.0;
    double age_factor = 1.0 - (2024 - p->year_built) * 0.005;
    double room_factor = 1.0 + (double)(p->bedrooms + p->bathrooms) * 0.05;
    a->market_value = base * age_factor * room_factor;
    a->assessed_value = a->market_value * 0.95;
    a->confidence = 85 + (re.n_appraisals * 3) % 10;
    a->active = 1;
    re.n_appraisals++;
    print_str("[RE] Appraisal "); print_int(re.n_appraisals - 1);
    print_str(" P"); print_int(property_id);
    print_str(" market="); print_int((int)a->market_value);
    print_str(" assessed="); print_int((int)a->assessed_value);
    print_str(" conf="); print_int(a->confidence); print_str("%\n");
    return re.n_appraisals - 1;
}

void re_market_analysis(void) {
    print_str("[RE] Market analysis:\n");
    double total_area = 0.0, total_price = 0.0;
    int active_count = 0;
    for (int i = 0; i < re.n_properties; i++) {
        if (properties[i].status != 3) {
            total_area += properties[i].area_sqm;
            total_price += properties[i].price;
            active_count++;
        }
    }
    double avg_price_sqm = (total_area > 0) ? total_price / total_area : 0.0;
    print_str("  Active properties: "); print_int(active_count); print_str("\n");
    print_str("  Avg price/sqm: "); print_int((int)avg_price_sqm); print_str("\n");
    print_str("  Properties sold: "); print_int(re.properties_sold); print_str("\n");
    print_str("  Properties leased: "); print_int(re.properties_leased); print_str("\n");
    print_str("  Sales volume: "); print_int((int)re.total_sales_volume); print_str("\n");
    print_str("  Rental income: "); print_int((int)re.total_rental_income); print_str("\n");
    print_str("  Total commission: "); print_int((int)re.total_commission); print_str("\n");
}

void re_print_state(void) {
    print_str("[RE] Properties="); print_int(re.n_properties);
    print_str(" Clients="); print_int(re.n_clients);
    print_str(" Listings="); print_int(re.n_listings);
    print_str(" Leases="); print_int(re.n_leases);
    print_str(" Viewings="); print_int(re.n_viewings);
    print_str(" Appraisals="); print_int(re.n_appraisals);
    print_str("\n");
    print_str("  Sales volume: "); print_int((int)re.total_sales_volume); print_str("\n");
    print_str("  Rental income: "); print_int((int)re.total_rental_income); print_str("\n");
    print_str("  Commission: "); print_int((int)re.total_commission); print_str("\n");
}

int main(void) {
    print_str("=== Real Estate Management System Demo ===\n\n");
    re_init();

    print_str("Adding properties...\n");
    re_add_property(1, 85, 2, 1, 350000.0, 2015);
    re_add_property(1, 120, 3, 2, 520000.0, 2018);
    re_add_property(2, 200, 4, 3, 880000.0, 2020);
    re_add_property(1, 60, 1, 1, 250000.0, 2010);
    re_add_property(3, 150, 0, 2, 1200000.0, 2019);
    re_add_property(1, 95, 2, 2, 420000.0, 2016);
    re_add_property(2, 180, 3, 2, 750000.0, 2017);
    re_add_property(1, 75, 2, 1, 310000.0, 2012);
    re_add_property(3, 300, 0, 4, 2500000.0, 2021);
    re_add_property(1, 110, 3, 2, 480000.0, 2019);

    print_str("\nRegistering clients...\n");
    re_register_client(1, 400000.0, 70);
    re_register_client(1, 600000.0, 100);
    re_register_client(2, 300000.0, 50);
    re_register_client(1, 900000.0, 150);
    re_register_client(2, 500000.0, 80);

    print_str("\nCreating listings...\n");
    re_create_listing(0, 360000.0);
    re_create_listing(1, 530000.0);
    re_create_listing(2, 900000.0);
    re_create_listing(3, 255000.0);
    re_create_listing(5, 430000.0);
    re_create_listing(6, 770000.0);

    print_str("\nListing activity...\n");
    re_listing_activity(0, 45, 8);
    re_listing_activity(1, 32, 5);
    re_listing_activity(2, 28, 4);
    re_listing_activity(3, 56, 12);
    re_listing_activity(4, 21, 3);
    re_listing_activity(5, 38, 7);

    print_str("\nScheduling viewings...\n");
    re_schedule_viewing(0, 0, 5);
    re_schedule_viewing(1, 1, 5);
    re_schedule_viewing(3, 2, 6);
    re_schedule_viewing(0, 3, 7);
    re_schedule_viewing(2, 3, 8);
    re_schedule_viewing(5, 4, 8);
    re_schedule_viewing(6, 1, 9);

    print_str("\nViewing feedback...\n");
    re_viewing_feedback(0, 8);
    re_viewing_feedback(1, 9);
    re_viewing_feedback(2, 6);
    re_viewing_feedback(3, 7);
    re_viewing_feedback(4, 8);
    re_viewing_feedback(5, 5);
    re_viewing_feedback(6, 9);

    print_str("\nClosing sales...\n");
    re_close_sale(0, 0, 355000.0);
    re_close_sale(1, 1, 525000.0);
    re_close_sale(3, 2, 248000.0);
    re_close_sale(5, 4, 760000.0);

    print_str("\nCreating leases...\n");
    re_create_lease(4, 0, 8000.0, 16000, 24);
    re_create_lease(7, 1, 5500.0, 11000, 12);
    re_create_lease(8, 3, 15000.0, 30000, 36);
    re_create_lease(9, 4, 6500.0, 13000, 18);

    print_str("\nProperty appraisals...\n");
    re_appraise_property(0);
    re_appraise_property(2);
    re_appraise_property(4);
    re_appraise_property(6);
    re_appraise_property(8);

    print_str("\nMarket analysis...\n");
    re_market_analysis();

    print_str("\nFinal state...\n");
    re_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
