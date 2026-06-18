/* land_resources: Land resources management (v1.0)
 * Land registration, planning, farmland protection, market, cadastre
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

#define MAX_REGISTR      18
#define MAX_PLANS        14
#define MAX_FARMLAND     12
#define MAX_MARKET       10
#define MAX_CADASTRE     10

typedef struct {
    int    reg_id;
    int    type;
    int    owner_id;
    int    area_hectares;
    int    land_use;
    int    location_zone;
    int    year_registered;
    int    status;
    int    active;
} registration_t;

typedef struct {
    int    plan_id;
    int    type;
    int    zone;
    int    area_hectares;
    int    purpose;
    int    approved_year;
    int    status;
    int    active;
} plan_t;

typedef struct {
    int    farm_id;
    int    zone;
    int    area_hectares;
    int    quality;
    int    crop_type;
    int    yield_tons;
    int    year;
    int    status;
    int    active;
} farmland_t;

typedef struct {
    int    market_id;
    int    transaction_type;
    int    seller_id;
    int    buyer_id;
    int    area_hectares;
    int    price;
    int    year;
    int    status;
    int    active;
} market_t;

typedef struct {
    int    cadastre_id;
    int    parcel_id;
    int    survey_area;
    int    boundaries;
    int    disputes;
    int    last_updated;
    int    year;
    int    active;
} cadastre_t;

typedef struct {
    int    n_registr;
    int    n_plans;
    int    n_farmland;
    int    n_market;
    int    n_cadastre;
    int    total_area;
    int    total_farmland;
    int    total_transactions;
    int    total_value;
    int    total_disputes;
} lr_state_t;

static registration_t registrations[MAX_REGISTR];
static plan_t plans[MAX_PLANS];
static farmland_t farmlands[MAX_FARMLAND];
static market_t markets[MAX_MARKET];
static cadastre_t cadastres[MAX_CADASTRE];
static lr_state_t lr;

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

int lr_init(void) {
    if (initialized) return -1;
    lr.n_registr = 0; lr.n_plans = 0; lr.n_farmland = 0;
    lr.n_market = 0; lr.n_cadastre = 0;
    lr.total_area = 0; lr.total_farmland = 0;
    lr.total_transactions = 0; lr.total_value = 0;
    lr.total_disputes = 0;
    for (int i = 0; i < MAX_REGISTR; i++) registrations[i].active = 0;
    for (int i = 0; i < MAX_PLANS; i++) plans[i].active = 0;
    for (int i = 0; i < MAX_FARMLAND; i++) farmlands[i].active = 0;
    for (int i = 0; i < MAX_MARKET; i++) markets[i].active = 0;
    for (int i = 0; i < MAX_CADASTRE; i++) cadastres[i].active = 0;
    initialized = 1;
    print_str("[LR] Land resources initialized\n");
    return 0;
}

int lr_register(int type, int owner, int area, int use, int zone, int year) {
    if (lr.n_registr >= MAX_REGISTR) return -1;
    registration_t* r = &registrations[lr.n_registr];
    r->reg_id = lr.n_registr;
    r->type = type;
    r->owner_id = owner;
    r->area_hectares = area;
    r->land_use = use;
    r->location_zone = zone;
    r->year_registered = year;
    r->status = 1;
    r->active = 1;
    lr.total_area += area;
    lr.n_registr++;
    print_str("[LR] Registr "); print_int(lr.n_registr - 1);
    print_str(" type="); print_int(type);
    print_str(" own="); print_int(owner);
    print_str(" area="); print_int(area); print_str("ha");
    print_str(" use="); print_int(use);
    print_str(" zone="); print_int(zone); print_str("\n");
    return lr.n_registr - 1;
}

int lr_plan(int type, int zone, int area, int purpose, int year) {
    if (lr.n_plans >= MAX_PLANS) return -1;
    plan_t* p = &plans[lr.n_plans];
    p->plan_id = lr.n_plans;
    p->type = type;
    p->zone = zone;
    p->area_hectares = area;
    p->purpose = purpose;
    p->approved_year = year;
    p->status = 1;
    p->active = 1;
    lr.n_plans++;
    print_str("[LR] Plan "); print_int(lr.n_plans - 1);
    print_str(" type="); print_int(type);
    print_str(" zone="); print_int(zone);
    print_str(" area="); print_int(area); print_str("ha");
    print_str(" purp="); print_int(purpose);
    print_str(" yr="); print_int(year); print_str("\n");
    return lr.n_plans - 1;
}

int lr_protect_farmland(int zone, int area, int quality, int crop, int yield_val, int year) {
    if (lr.n_farmland >= MAX_FARMLAND) return -1;
    farmland_t* f = &farmlands[lr.n_farmland];
    f->farm_id = lr.n_farmland;
    f->zone = zone;
    f->area_hectares = area;
    f->quality = quality;
    f->crop_type = crop;
    f->yield_tons = yield_val;
    f->year = year;
    f->status = 1;
    f->active = 1;
    lr.total_farmland += area;
    lr.n_farmland++;
    print_str("[LR] Farm "); print_int(lr.n_farmland - 1);
    print_str(" zone="); print_int(zone);
    print_str(" area="); print_int(area); print_str("ha");
    print_str(" qual="); print_int(quality);
    print_str(" crop="); print_int(crop);
    print_str(" yld="); print_int(yield_val); print_str("t\n");
    return lr.n_farmland - 1;
}

int lr_transact(int type, int seller, int buyer, int area, int price, int year) {
    if (lr.n_market >= MAX_MARKET) return -1;
    market_t* m = &markets[lr.n_market];
    m->market_id = lr.n_market;
    m->transaction_type = type;
    m->seller_id = seller;
    m->buyer_id = buyer;
    m->area_hectares = area;
    m->price = price;
    m->year = year;
    m->status = 1;
    m->active = 1;
    lr.total_transactions++;
    lr.total_value += price;
    lr.n_market++;
    print_str("[LR] Market "); print_int(lr.n_market - 1);
    print_str(" type="); print_int(type);
    print_str(" sell="); print_int(seller);
    print_str(" buy="); print_int(buyer);
    print_str(" area="); print_int(area); print_str("ha");
    print_str(" price=$"); print_int(price); print_str("\n");
    return lr.n_market - 1;
}

int lr_cadastre(int parcel, int area, int boundaries, int disputes, int updated, int year) {
    if (lr.n_cadastre >= MAX_CADASTRE) return -1;
    cadastre_t* c = &cadastres[lr.n_cadastre];
    c->cadastre_id = lr.n_cadastre;
    c->parcel_id = parcel;
    c->survey_area = area;
    c->boundaries = boundaries;
    c->disputes = disputes;
    c->last_updated = updated;
    c->year = year;
    c->active = 1;
    lr.total_disputes += disputes;
    lr.n_cadastre++;
    print_str("[LR] Cadastre "); print_int(lr.n_cadastre - 1);
    print_str(" parcel="); print_int(parcel);
    print_str(" area="); print_int(area);
    print_str(" bound="); print_int(boundaries);
    print_str(" disp="); print_int(disputes); print_str("\n");
    return lr.n_cadastre - 1;
}

void lr_registration_report(void) {
    print_str("[LR] Registration report:\n");
    print_str("  Registrations: "); print_int(lr.n_registr); print_str("\n");
    print_str("  Total area: "); print_int(lr.total_area); print_str(" ha\n");
    print_str("  Plans: "); print_int(lr.n_plans); print_str("\n");
}

void lr_farmland_report(void) {
    print_str("[LR] Farmland report:\n");
    print_str("  Protected areas: "); print_int(lr.n_farmland); print_str("\n");
    print_str("  Total farmland: "); print_int(lr.total_farmland); print_str(" ha\n");
}

void lr_market_report(void) {
    print_str("[LR] Market report:\n");
    print_str("  Transactions: "); print_int(lr.total_transactions); print_str("\n");
    print_str("  Total value: $"); print_int(lr.total_value); print_str("\n");
    print_str("  Disputes: "); print_int(lr.total_disputes); print_str("\n");
}

void lr_print_state(void) {
    print_str("[LR] Rg="); print_int(lr.n_registr);
    print_str(" Pl="); print_int(lr.n_plans);
    print_str(" Fm="); print_int(lr.n_farmland);
    print_str(" Mk="); print_int(lr.n_market);
    print_str(" Cd="); print_int(lr.n_cadastre);
    print_str("\n");
}

int main(void) {
    print_str("=== Land Resources Demo ===\n\n");
    lr_init();

    print_str("Registering land...\n");
    for (int i = 0; i < 18; i++) {
        int type = (i % 3) + 1;
        int owner = 2000 + (i * 13);
        int area = 5 + (i * 3);
        int use = (i % 6) + 1;
        int zone = (i % 8) + 1;
        int year = 2015 + (i % 10);
        lr_register(type, owner, area, use, zone, year);
    }

    print_str("\nCreating plans...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 3) + 1;
        int zone = (i % 8) + 1;
        int area = 50 + (i * 20);
        int purpose = (i % 5) + 1;
        int year = 2018 + (i % 6);
        lr_plan(type, zone, area, purpose, year);
    }

    print_str("\nProtecting farmland...\n");
    for (int i = 0; i < 12; i++) {
        int zone = (i % 8) + 1;
        int area = 20 + (i * 10);
        int quality = (i % 4) + 1;
        int crop = (i % 5) + 1;
        int yield_val = 100 + (i * 50);
        int year = 2020 + (i % 5);
        lr_protect_farmland(zone, area, quality, crop, yield_val, year);
    }

    print_str("\nLand transactions...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 3) + 1;
        int seller = 3000 + (i * 7);
        int buyer = 4000 + (i * 11);
        int area = 2 + (i * 2);
        int price = 100000 + (i * 50000);
        int year = 2022 + (i % 3);
        lr_transact(type, seller, buyer, area, price, year);
    }

    print_str("\nCadastre surveys...\n");
    for (int i = 0; i < 10; i++) {
        int parcel = 5000 + (i * 100);
        int area = 10 + (i * 5);
        int bound = 4 + (i % 4);
        int disp = i % 3;
        int upd = 20230101 + (i * 30);
        int year = 2023 + (i % 2);
        lr_cadastre(parcel, area, bound, disp, upd, year);
    }

    print_str("\nRegistration report...\n");
    lr_registration_report();

    print_str("\nFarmland report...\n");
    lr_farmland_report();

    print_str("\nMarket report...\n");
    lr_market_report();

    print_str("\nFinal state...\n");
    lr_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
