/* tourism_admin: Tourism administration system (v1.0)
 * Tourism resources, travel agencies, attractions, hotel rating, promotion
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

#define MAX_RESOURCE     16
#define MAX_AGENCY       14
#define MAX_ATTRACTION   12
#define MAX_HOTEL        10
#define MAX_PROMOTION    10

typedef struct {
    int    resource_id;
    int    region_id;
    int    resource_type;
    int    rating;
    int    annual_visitors;
    int    revenue;
    int    year;
    int    active;
} resource_t;

typedef struct {
    int    agency_id;
    int    operator_id;
    int    license_type;
    int    tours_operated;
    int    tourists_served;
    int    satisfaction;
    int    year;
    int    active;
} agency_t;

typedef struct {
    int    attraction_id;
    int    location_id;
    int    attraction_type;
    int    daily_capacity;
    int    daily_visitors;
    int    ticket_price;
    int    year;
    int    active;
} attraction_t;

typedef struct {
    int    hotel_id;
    int    property_id;
    int    star_rating;
    int    rooms;
    int    occupancy_rate;
    int    avg_rate;
    int    year;
    int    active;
} hotel_t;

typedef struct {
    int    promotion_id;
    int    campaign_id;
    int    promotion_type;
    int    budget;
    int    reach;
    int    conversion_rate;
    int    year;
    int    active;
} promotion_t;

typedef struct {
    int    n_resource;
    int    n_agency;
    int    n_attraction;
    int    n_hotel;
    int    n_promotion;
    int    total_visitors;
    int    total_revenue;
    int    total_tours;
    int    total_rooms;
    int    total_budget;
} trg_state_t;

static resource_t resources[MAX_RESOURCE];
static agency_t agencies[MAX_AGENCY];
static attraction_t attractions[MAX_ATTRACTION];
static hotel_t hotels[MAX_HOTEL];
static promotion_t promotions[MAX_PROMOTION];
static trg_state_t trg;

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

int trg_init(void) {
    if (initialized) return -1;
    trg.n_resource = 0; trg.n_agency = 0; trg.n_attraction = 0;
    trg.n_hotel = 0; trg.n_promotion = 0;
    trg.total_visitors = 0; trg.total_revenue = 0;
    trg.total_tours = 0; trg.total_rooms = 0;
    trg.total_budget = 0;
    for (int i = 0; i < MAX_RESOURCE; i++) resources[i].active = 0;
    for (int i = 0; i < MAX_AGENCY; i++) agencies[i].active = 0;
    for (int i = 0; i < MAX_ATTRACTION; i++) attractions[i].active = 0;
    for (int i = 0; i < MAX_HOTEL; i++) hotels[i].active = 0;
    for (int i = 0; i < MAX_PROMOTION; i++) promotions[i].active = 0;
    initialized = 1;
    print_str("[TRG] Tourism admin initialized\n");
    return 0;
}

int trg_resource(int region, int resource_type, int rating, int visitors, int revenue, int year) {
    if (trg.n_resource >= MAX_RESOURCE) return -1;
    resource_t* r = &resources[trg.n_resource];
    r->resource_id = trg.n_resource;
    r->region_id = region;
    r->resource_type = resource_type;
    r->rating = rating;
    r->annual_visitors = visitors;
    r->revenue = revenue;
    r->year = year;
    r->active = 1;
    trg.total_visitors += visitors;
    trg.total_revenue += revenue;
    trg.n_resource++;
    print_str("[TRG] Resource "); print_int(trg.n_resource - 1);
    print_str(" rgn="); print_int(region);
    print_str(" type="); print_int(resource_type);
    print_str(" rat="); print_int(rating);
    print_str(" vst="); print_int(visitors);
    print_str(" rev=$"); print_int(revenue); print_str("\n");
    return trg.n_resource - 1;
}

int trg_agency(int operator_id, int license_type, int tours, int tourists, int satisfaction, int year) {
    if (trg.n_agency >= MAX_AGENCY) return -1;
    agency_t* a = &agencies[trg.n_agency];
    a->agency_id = trg.n_agency;
    a->operator_id = operator_id;
    a->license_type = license_type;
    a->tours_operated = tours;
    a->tourists_served = tourists;
    a->satisfaction = satisfaction;
    a->year = year;
    a->active = 1;
    trg.total_tours += tours;
    trg.n_agency++;
    print_str("[TRG] Agency "); print_int(trg.n_agency - 1);
    print_str(" opr="); print_int(operator_id);
    print_str(" lic="); print_int(license_type);
    print_str(" trs="); print_int(tours);
    print_str(" tst="); print_int(tourists);
    print_str(" sat="); print_int(satisfaction); print_str("\n");
    return trg.n_agency - 1;
}

int trg_attraction(int location, int attraction_type, int capacity, int visitors, int price, int year) {
    if (trg.n_attraction >= MAX_ATTRACTION) return -1;
    attraction_t* at = &attractions[trg.n_attraction];
    at->attraction_id = trg.n_attraction;
    at->location_id = location;
    at->attraction_type = attraction_type;
    at->daily_capacity = capacity;
    at->daily_visitors = visitors;
    at->ticket_price = price;
    at->year = year;
    at->active = 1;
    trg.total_visitors += visitors * 365;
    trg.n_attraction++;
    print_str("[TRG] Attraction "); print_int(trg.n_attraction - 1);
    print_str(" loc="); print_int(location);
    print_str(" type="); print_int(attraction_type);
    print_str(" cap="); print_int(capacity);
    print_str(" vst="); print_int(visitors);
    print_str(" prc=$"); print_int(price); print_str("\n");
    return trg.n_attraction - 1;
}

int trg_hotel(int property, int star_rating, int rooms, int occupancy, int avg_rate, int year) {
    if (trg.n_hotel >= MAX_HOTEL) return -1;
    hotel_t* h = &hotels[trg.n_hotel];
    h->hotel_id = trg.n_hotel;
    h->property_id = property;
    h->star_rating = star_rating;
    h->rooms = rooms;
    h->occupancy_rate = occupancy;
    h->avg_rate = avg_rate;
    h->year = year;
    h->active = 1;
    trg.total_rooms += rooms;
    trg.n_hotel++;
    print_str("[TRG] Hotel "); print_int(trg.n_hotel - 1);
    print_str(" prp="); print_int(property);
    print_str(" str="); print_int(star_rating);
    print_str(" rms="); print_int(rooms);
    print_str(" occ="); print_int(occupancy); print_str("%");
    print_str(" rate=$"); print_int(avg_rate); print_str("\n");
    return trg.n_hotel - 1;
}

int trg_promotion(int campaign, int promotion_type, int budget, int reach, int conversion, int year) {
    if (trg.n_promotion >= MAX_PROMOTION) return -1;
    promotion_t* p = &promotions[trg.n_promotion];
    p->promotion_id = trg.n_promotion;
    p->campaign_id = campaign;
    p->promotion_type = promotion_type;
    p->budget = budget;
    p->reach = reach;
    p->conversion_rate = conversion;
    p->year = year;
    p->active = 1;
    trg.total_budget += budget;
    trg.n_promotion++;
    print_str("[TRG] Promotion "); print_int(trg.n_promotion - 1);
    print_str(" cmp="); print_int(campaign);
    print_str(" type="); print_int(promotion_type);
    print_str(" bgt=$"); print_int(budget);
    print_str(" rch="); print_int(reach);
    print_str(" cnv="); print_int(conversion); print_str("%\n");
    return trg.n_promotion - 1;
}

void trg_resource_report(void) {
    print_str("[TRG] Resource report:\n");
    print_str("  Tourism resources: "); print_int(trg.n_resource); print_str("\n");
    print_str("  Total visitors: "); print_int(trg.total_visitors); print_str("\n");
    print_str("  Total revenue: $"); print_int(trg.total_revenue); print_str("\n");
}

void trg_agency_report(void) {
    print_str("[TRG] Agency report:\n");
    print_str("  Travel agencies: "); print_int(trg.n_agency); print_str("\n");
    print_str("  Total tours: "); print_int(trg.total_tours); print_str("\n");
}

void trg_promotion_report(void) {
    print_str("[TRG] Promotion report:\n");
    print_str("  Attractions: "); print_int(trg.n_attraction); print_str("\n");
    print_str("  Hotels: "); print_int(trg.n_hotel); print_str("\n");
    print_str("  Total rooms: "); print_int(trg.total_rooms); print_str("\n");
    print_str("  Promotion campaigns: "); print_int(trg.n_promotion); print_str("\n");
    print_str("  Total budget: $"); print_int(trg.total_budget); print_str("\n");
}

void trg_print_state(void) {
    print_str("[TRG] Rs="); print_int(trg.n_resource);
    print_str(" Ag="); print_int(trg.n_agency);
    print_str(" At="); print_int(trg.n_attraction);
    print_str(" Ht="); print_int(trg.n_hotel);
    print_str(" Pr="); print_int(trg.n_promotion);
    print_str("\n");
}

int main(void) {
    print_str("=== Tourism Admin Demo ===\n\n");
    trg_init();

    print_str("Tourism resources...\n");
    for (int i = 0; i < 16; i++) {
        int rgn = (i % 8) + 1;
        int type = (i % 5) + 1;
        int rat = (i % 5) + 1;
        int vst = 100000 + (i * 50000);
        int rev = 5000000 + (i * 2000000);
        int year = 2020 + (i % 5);
        trg_resource(rgn, type, rat, vst, rev, year);
    }

    print_str("\nTravel agencies...\n");
    for (int i = 0; i < 14; i++) {
        int opr = 1000 + (i * 11);
        int lic = (i % 3) + 1;
        int trs = 50 + (i * 15);
        int tst = trs * 30;
        int sat = 75 + (i % 20);
        int year = 2021 + (i % 4);
        trg_agency(opr, lic, trs, tst, sat, year);
    }

    print_str("\nTourist attractions...\n");
    for (int i = 0; i < 12; i++) {
        int loc = 200 + (i * 10);
        int type = (i % 4) + 1;
        int cap = 500 + (i * 200);
        int vst = cap - (i * 50);
        int prc = 50 + (i * 15);
        int year = 2022 + (i % 3);
        trg_attraction(loc, type, cap, vst, prc, year);
    }

    print_str("\nHotels...\n");
    for (int i = 0; i < 10; i++) {
        int prp = 300 + (i * 13);
        int str = (i % 5) + 1;
        int rms = 50 + (i * 20);
        int occ = 60 + (i * 3);
        int rate = 200 + (i * 50);
        int year = 2023 + (i % 2);
        trg_hotel(prp, str, rms, occ, rate, year);
    }

    print_str("\nPromotions...\n");
    for (int i = 0; i < 10; i++) {
        int cmp = 400 + (i * 17);
        int type = (i % 4) + 1;
        int bgt = 100000 + (i * 50000);
        int rch = 1000000 + (i * 500000);
        int cnv = 3 + (i % 8);
        int year = 2024;
        trg_promotion(cmp, type, bgt, rch, cnv, year);
    }

    print_str("\nResource report...\n");
    trg_resource_report();

    print_str("\nAgency report...\n");
    trg_agency_report();

    print_str("\nPromotion report...\n");
    trg_promotion_report();

    print_str("\nFinal state...\n");
    trg_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
