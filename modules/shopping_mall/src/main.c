/* shopping_mall: Shopping mall management (v1.0)
 * Tenants, floors, leases, marketing, facilities, revenue
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

#define MAX_TENANTS    14
#define MAX_FLOORS     5
#define MAX_LEASES     12
#define MAX_FACILITIES 8
#define MAX_PROMOTIONS 10

typedef struct {
    int    tenant_id;
    int    type;
    int    floor_id;
    int    sqft;
    double rent_per_sqft;
    int    lease_id;
    double monthly_sales;
    double total_rent_paid;
    int    active;
} tenant_t;

typedef struct {
    int    floor_id;
    int    zone;
    int    total_sqft;
    int    occupied_sqft;
    int    n_tenants;
    int    foot_traffic;
    double revenue;
    int    active;
} floor_t;

typedef struct {
    int    lease_id;
    int    tenant_id;
    int    start_month;
    int    duration_months;
    double monthly_rent;
    double deposit;
    int    status;
    int    active;
} lease_t;

typedef struct {
    int    facility_id;
    int    type;
    int    capacity;
    int    status;
    double maint_cost;
    int    n_uses;
    int    active;
} facility_t;

typedef struct {
    int    promo_id;
    int    type;
    double cost;
    int    n_participants;
    double revenue_generated;
    int    day;
    int    active;
} promotion_t;

typedef struct {
    int    n_tenants;
    int    n_floors;
    int    n_leases;
    int    n_facilities;
    int    n_promos;
    double rent_revenue;
    double parking_revenue;
    double ad_revenue;
    double promo_revenue;
    double total_revenue;
    double total_costs;
    int    total_foot_traffic;
    int    occupancy_pct;
} sm_state_t;

static tenant_t tenants[MAX_TENANTS];
static floor_t floors[MAX_FLOORS];
static lease_t leases[MAX_LEASES];
static facility_t facilities[MAX_FACILITIES];
static promotion_t promotions[MAX_PROMOTIONS];
static sm_state_t sm;

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

int sm_init(void) {
    if (initialized) return -1;
    sm.n_tenants = 0; sm.n_floors = 0; sm.n_leases = 0;
    sm.n_facilities = 0; sm.n_promos = 0;
    sm.rent_revenue = 0.0; sm.parking_revenue = 0.0;
    sm.ad_revenue = 0.0; sm.promo_revenue = 0.0;
    sm.total_revenue = 0.0; sm.total_costs = 0.0;
    sm.total_foot_traffic = 0; sm.occupancy_pct = 0;
    for (int i = 0; i < MAX_TENANTS; i++) tenants[i].active = 0;
    for (int i = 0; i < MAX_FLOORS; i++) floors[i].active = 0;
    for (int i = 0; i < MAX_LEASES; i++) leases[i].active = 0;
    for (int i = 0; i < MAX_FACILITIES; i++) facilities[i].active = 0;
    for (int i = 0; i < MAX_PROMOTIONS; i++) promotions[i].active = 0;
    initialized = 1;
    print_str("[SM] Mall initialized\n");
    return 0;
}

int sm_add_floor(int zone, int total_sqft) {
    if (sm.n_floors >= MAX_FLOORS) return -1;
    floor_t* f = &floors[sm.n_floors];
    f->floor_id = sm.n_floors;
    f->zone = zone;
    f->total_sqft = total_sqft;
    f->occupied_sqft = 0;
    f->n_tenants = 0;
    f->foot_traffic = 0;
    f->revenue = 0.0;
    f->active = 1;
    sm.n_floors++;
    print_str("[SM] Floor "); print_int(sm.n_floors - 1);
    print_str(" zone="); print_int(zone);
    print_str(" sqft="); print_int(total_sqft); print_str("\n");
    return sm.n_floors - 1;
}

int sm_add_facility(int type, int capacity, double maint) {
    if (sm.n_facilities >= MAX_FACILITIES) return -1;
    facility_t* fc = &facilities[sm.n_facilities];
    fc->facility_id = sm.n_facilities;
    fc->type = type;
    fc->capacity = capacity;
    fc->status = 1;
    fc->maint_cost = maint;
    fc->n_uses = 0;
    fc->active = 1;
    sm.total_costs += maint;
    sm.n_facilities++;
    print_str("[SM] Facility "); print_int(sm.n_facilities - 1);
    print_str(" type="); print_int(type);
    print_str(" cap="); print_int(capacity);
    print_str(" maint="); print_int((int)maint); print_str("\n");
    return sm.n_facilities - 1;
}

int sm_add_tenant(int type, int floor_id, int sqft, double rent_per_sqft) {
    if (sm.n_tenants >= MAX_TENANTS || floor_id >= sm.n_floors) return -1;
    tenant_t* t = &tenants[sm.n_tenants];
    t->tenant_id = sm.n_tenants;
    t->type = type;
    t->floor_id = floor_id;
    t->sqft = sqft;
    t->rent_per_sqft = rent_per_sqft;
    t->lease_id = -1;
    t->monthly_sales = 0.0;
    t->total_rent_paid = 0.0;
    t->active = 1;
    floors[floor_id].n_tenants++;
    floors[floor_id].occupied_sqft += sqft;
    sm.n_tenants++;
    print_str("[SM] Tenant "); print_int(sm.n_tenants - 1);
    print_str(" type="); print_int(type);
    print_str(" F"); print_int(floor_id);
    print_str(" sqft="); print_int(sqft);
    print_str(" rent="); print_int((int)rent_per_sqft); print_str("/sqft\n");
    return sm.n_tenants - 1;
}

int sm_create_lease(int tenant_id, int start, int duration) {
    if (sm.n_leases >= MAX_LEASES || tenant_id >= sm.n_tenants) return -1;
    tenant_t* t = &tenants[tenant_id];
    lease_t* l = &leases[sm.n_leases];
    l->lease_id = sm.n_leases;
    l->tenant_id = tenant_id;
    l->start_month = start;
    l->duration_months = duration;
    l->monthly_rent = t->sqft * t->rent_per_sqft;
    l->deposit = l->monthly_rent;
    l->status = 1;
    l->active = 1;
    t->lease_id = sm.n_leases;
    t->total_rent_paid += l->deposit + l->monthly_rent * duration;
    sm.rent_revenue += l->deposit + l->monthly_rent * duration;
    sm.total_revenue += l->deposit + l->monthly_rent * duration;
    floors[t->floor_id].revenue += l->deposit + l->monthly_rent * duration;
    sm.n_leases++;
    print_str("[SM] Lease "); print_int(sm.n_leases - 1);
    print_str(" T"); print_int(tenant_id);
    print_str(" start="); print_int(start);
    print_str(" dur="); print_int(duration); print_str("mo");
    print_str(" rent="); print_int((int)l->monthly_rent);
    print_str(" dep="); print_int((int)l->deposit); print_str("\n");
    return sm.n_leases - 1;
}

int sm_add_promotion(int type, double cost, double revenue, int day) {
    if (sm.n_promos >= MAX_PROMOTIONS) return -1;
    promotion_t* p = &promotions[sm.n_promos];
    p->promo_id = sm.n_promos;
    p->type = type;
    p->cost = cost;
    p->n_participants = 0;
    p->revenue_generated = revenue;
    p->day = day;
    p->active = 1;
    sm.promo_revenue += revenue;
    sm.total_revenue += revenue;
    sm.total_costs += cost;
    sm.n_promos++;
    print_str("[SM] Promo "); print_int(sm.n_promos - 1);
    print_str(" type="); print_int(type);
    print_str(" cost="); print_int((int)cost);
    print_str(" rev="); print_int((int)revenue);
    print_str(" day="); print_int(day); print_str("\n");
    return sm.n_promos - 1;
}

int sm_add_foot_traffic(int floor_id, int visitors) {
    if (floor_id >= sm.n_floors) return -1;
    floors[floor_id].foot_traffic += visitors;
    sm.total_foot_traffic += visitors;
    print_str("[SM] F"); print_int(floor_id);
    print_str(" traffic +"); print_int(visitors); print_str("\n");
    return 0;
}

int sm_parking_income(double amount) {
    sm.parking_revenue += amount;
    sm.total_revenue += amount;
    print_str("[SM] Parking income="); print_int((int)amount); print_str("\n");
    return 0;
}

int sm_ad_income(double amount) {
    sm.ad_revenue += amount;
    sm.total_revenue += amount;
    print_str("[SM] Ad income="); print_int((int)amount); print_str("\n");
    return 0;
}

int sm_record_tenant_sales(int tenant_id, double sales) {
    if (tenant_id >= sm.n_tenants) return -1;
    tenants[tenant_id].monthly_sales += sales;
    print_str("[SM] T"); print_int(tenant_id);
    print_str(" sales="); print_int((int)sales); print_str("\n");
    return 0;
}

void sm_floor_report(void) {
    print_str("[SM] Floor report:\n");
    int total_cap = 0, total_occ = 0;
    for (int i = 0; i < sm.n_floors; i++) {
        floor_t* f = &floors[i];
        print_str("  F"); print_int(i);
        print_str(" zone="); print_int(f->zone);
        print_str(" occ="); print_int(f->occupied_sqft);
        print_str("/"); print_int(f->total_sqft);
        print_str(" tenants="); print_int(f->n_tenants);
        print_str(" traffic="); print_int(f->foot_traffic);
        print_str(" rev="); print_int((int)f->revenue); print_str("\n");
        total_cap += f->total_sqft;
        total_occ += f->occupied_sqft;
    }
    sm.occupancy_pct = (total_cap > 0) ? (total_occ * 100 / total_cap) : 0;
    print_str("  Overall occupancy: "); print_int(sm.occupancy_pct); print_str("%\n");
}

void sm_financial_report(void) {
    print_str("[SM] Financial report:\n");
    print_str("  Rent revenue: "); print_int((int)sm.rent_revenue); print_str("\n");
    print_str("  Parking: "); print_int((int)sm.parking_revenue); print_str("\n");
    print_str("  Advertising: "); print_int((int)sm.ad_revenue); print_str("\n");
    print_str("  Promotions: "); print_int((int)sm.promo_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)sm.total_revenue); print_str("\n");
    print_str("  Costs: "); print_int((int)sm.total_costs); print_str("\n");
    double profit = sm.total_revenue - sm.total_costs;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    print_str("  Total traffic: "); print_int(sm.total_foot_traffic); print_str("\n");
}

void sm_print_state(void) {
    print_str("[SM] Tenants="); print_int(sm.n_tenants);
    print_str(" Floors="); print_int(sm.n_floors);
    print_str(" Leases="); print_int(sm.n_leases);
    print_str(" Facilities="); print_int(sm.n_facilities);
    print_str(" Promos="); print_int(sm.n_promos);
    print_str("\n");
    print_str("  Occupancy: "); print_int(sm.occupancy_pct); print_str("%\n");
    print_str("  Revenue: "); print_int((int)sm.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Shopping Mall Demo ===\n\n");
    sm_init();

    print_str("Adding floors...\n");
    sm_add_floor(1, 50000);
    sm_add_floor(2, 45000);
    sm_add_floor(3, 40000);
    sm_add_floor(1, 55000);
    sm_add_floor(2, 35000);

    print_str("\nAdding facilities...\n");
    sm_add_facility(1, 500, 3000.0);
    sm_add_facility(1, 400, 2500.0);
    sm_add_facility(2, 200, 1500.0);
    sm_add_facility(2, 150, 1200.0);
    sm_add_facility(3, 100, 800.0);
    sm_add_facility(3, 80, 600.0);
    sm_add_facility(4, 300, 2000.0);
    sm_add_facility(4, 250, 1800.0);

    print_str("\nAdding tenants...\n");
    sm_add_tenant(1, 0, 2000, 3.5);
    sm_add_tenant(1, 0, 1500, 3.0);
    sm_add_tenant(2, 1, 3000, 4.0);
    sm_add_tenant(2, 1, 2500, 3.5);
    sm_add_tenant(3, 2, 1800, 2.5);
    sm_add_tenant(3, 2, 2200, 3.0);
    sm_add_tenant(4, 3, 4000, 4.5);
    sm_add_tenant(4, 3, 3500, 4.0);
    sm_add_tenant(1, 4, 1200, 2.8);
    sm_add_tenant(2, 4, 1600, 3.2);
    sm_add_tenant(3, 0, 2800, 3.5);
    sm_add_tenant(4, 1, 3200, 4.0);
    sm_add_tenant(1, 2, 1400, 2.5);
    sm_add_tenant(2, 3, 2600, 3.8);

    print_str("\nCreating leases...\n");
    for (int i = 0; i < 12; i++) {
        sm_create_lease(i, 1, 12 + (i % 3) * 12);
    }

    print_str("\nAdding promotions...\n");
    sm_add_promotion(1, 5000.0, 25000.0, 10);
    sm_add_promotion(2, 3000.0, 18000.0, 12);
    sm_add_promotion(3, 8000.0, 40000.0, 15);
    sm_add_promotion(1, 4000.0, 22000.0, 18);
    sm_add_promotion(2, 6000.0, 35000.0, 20);
    sm_add_promotion(4, 2000.0, 12000.0, 22);
    sm_add_promotion(3, 7000.0, 38000.0, 25);
    sm_add_promotion(1, 4500.0, 24000.0, 28);

    print_str("\nFoot traffic...\n");
    sm_add_foot_traffic(0, 5000);
    sm_add_foot_traffic(1, 4500);
    sm_add_foot_traffic(2, 3800);
    sm_add_foot_traffic(3, 5500);
    sm_add_foot_traffic(4, 3200);
    sm_add_foot_traffic(0, 4800);
    sm_add_foot_traffic(1, 4200);
    sm_add_foot_traffic(2, 3500);
    sm_add_foot_traffic(3, 5200);
    sm_add_foot_traffic(4, 3000);

    print_str("\nParking income...\n");
    sm_parking_income(8000.0);
    sm_parking_income(7500.0);
    sm_parking_income(9000.0);
    sm_parking_income(8500.0);

    print_str("\nAdvertising income...\n");
    sm_ad_income(3000.0);
    sm_ad_income(4500.0);
    sm_ad_income(2500.0);
    sm_ad_income(5000.0);

    print_str("\nTenant sales...\n");
    for (int i = 0; i < 14; i++) {
        sm_record_tenant_sales(i, 15000.0 + i * 2000.0);
    }

    print_str("\nFloor report...\n");
    sm_floor_report();

    print_str("\nFinancial report...\n");
    sm_financial_report();

    print_str("\nFinal state...\n");
    sm_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
