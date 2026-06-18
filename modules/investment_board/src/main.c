/* investment_board: Investment promotion and management (v1.0)
 * Investment promotion, foreign investment, economic zones, projects, statistics
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

#define MAX_PROMOTION  12
#define MAX_FOREIGN    14
#define MAX_ZONES      10
#define MAX_PROJECTS   16
#define MAX_STATS      10

typedef struct {
    int    promo_id;
    int    type;
    int    zone;
    int    target_amount;
    int    actual_amount;
    int    investors_met;
    int    deals_closed;
    int    year;
    int    active;
} promotion_t;

typedef struct {
    int    foreign_id;
    int    country;
    int    zone;
    int    amount;
    int    sector;
    int    jobs_created;
    int    approved;
    int    year;
    int    active;
} foreign_t;

typedef struct {
    int    zone_id;
    int    type;
    int    region;
    int    area_ha;
    int    companies;
    int    investment;
    int    revenue;
    int    year_established;
    int    active;
} zone_t;

typedef struct {
    int    project_id;
    int    type;
    int    zone;
    int    investor;
    int    amount;
    int    jobs;
    int    status;
    int    year;
    int    active;
} project_t;

typedef struct {
    int    stat_id;
    int    zone;
    int    total_investment;
    int    foreign_investment;
    int    domestic_investment;
    int    jobs_created;
    int    year;
    int    active;
} statistic_t;

typedef struct {
    int    n_promotion;
    int    n_foreign;
    int    n_zones;
    int    n_projects;
    int    n_stats;
    int    total_promoted;
    int    total_foreign;
    int    total_domestic;
    int    total_jobs;
    int    total_zones_revenue;
} ib_state_t;

static promotion_t promotions[MAX_PROMOTION];
static foreign_t foreigns[MAX_FOREIGN];
static zone_t zones[MAX_ZONES];
static project_t projects[MAX_PROJECTS];
static statistic_t statistics[MAX_STATS];
static ib_state_t ib;

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

int ib_init(void) {
    if (initialized) return -1;
    ib.n_promotion = 0; ib.n_foreign = 0; ib.n_zones = 0;
    ib.n_projects = 0; ib.n_stats = 0;
    ib.total_promoted = 0; ib.total_foreign = 0;
    ib.total_domestic = 0; ib.total_jobs = 0;
    ib.total_zones_revenue = 0;
    for (int i = 0; i < MAX_PROMOTION; i++) promotions[i].active = 0;
    for (int i = 0; i < MAX_FOREIGN; i++) foreigns[i].active = 0;
    for (int i = 0; i < MAX_ZONES; i++) zones[i].active = 0;
    for (int i = 0; i < MAX_PROJECTS; i++) projects[i].active = 0;
    for (int i = 0; i < MAX_STATS; i++) statistics[i].active = 0;
    initialized = 1;
    print_str("[IB] Investment board initialized\n");
    return 0;
}

int ib_add_promotion(int type, int zone, int target, int actual, int met, int deals, int year) {
    if (ib.n_promotion >= MAX_PROMOTION) return -1;
    promotion_t* p = &promotions[ib.n_promotion];
    p->promo_id = ib.n_promotion;
    p->type = type;
    p->zone = zone;
    p->target_amount = target;
    p->actual_amount = actual;
    p->investors_met = met;
    p->deals_closed = deals;
    p->year = year;
    p->active = 1;
    ib.total_promoted += actual;
    ib.n_promotion++;
    print_str("[IB] Promo "); print_int(ib.n_promotion - 1);
    print_str(" type="); print_int(type);
    print_str(" zone="); print_int(zone);
    print_str(" act=$"); print_int(actual);
    print_str(" deals="); print_int(deals); print_str("\n");
    return ib.n_promotion - 1;
}

int ib_approve_foreign(int country, int zone, int amount, int sector, int jobs, int year) {
    if (ib.n_foreign >= MAX_FOREIGN) return -1;
    foreign_t* f = &foreigns[ib.n_foreign];
    f->foreign_id = ib.n_foreign;
    f->country = country;
    f->zone = zone;
    f->amount = amount;
    f->sector = sector;
    f->jobs_created = jobs;
    f->approved = 1;
    f->year = year;
    f->active = 1;
    ib.total_foreign += amount;
    ib.total_jobs += jobs;
    ib.n_foreign++;
    print_str("[IB] Foreign "); print_int(ib.n_foreign - 1);
    print_str(" ctry="); print_int(country);
    print_str(" zone="); print_int(zone);
    print_str(" amt=$"); print_int(amount);
    print_str(" jobs="); print_int(jobs); print_str("\n");
    return ib.n_foreign - 1;
}

int ib_establish_zone(int type, int region, int area, int companies, int investment, int year) {
    if (ib.n_zones >= MAX_ZONES) return -1;
    zone_t* z = &zones[ib.n_zones];
    z->zone_id = ib.n_zones;
    z->type = type;
    z->region = region;
    z->area_ha = area;
    z->companies = companies;
    z->investment = investment;
    z->revenue = 0;
    z->year_established = year;
    z->active = 1;
    ib.n_zones++;
    print_str("[IB] Zone "); print_int(ib.n_zones - 1);
    print_str(" type="); print_int(type);
    print_str(" reg="); print_int(region);
    print_str(" area="); print_int(area); print_str("ha");
    print_str(" comp="); print_int(companies); print_str("\n");
    return ib.n_zones - 1;
}

int ib_zone_revenue(int zone_id, int revenue) {
    if (zone_id >= ib.n_zones) return -1;
    zones[zone_id].revenue += revenue;
    ib.total_zones_revenue += revenue;
    print_str("[IB] ZoneRev Z"); print_int(zone_id);
    print_str(" rev=$"); print_int(revenue); print_str("\n");
    return 0;
}

int ib_register_project(int type, int zone, int investor, int amount, int jobs, int year) {
    if (ib.n_projects >= MAX_PROJECTS) return -1;
    project_t* p = &projects[ib.n_projects];
    p->project_id = ib.n_projects;
    p->type = type;
    p->zone = zone;
    p->investor = investor;
    p->amount = amount;
    p->jobs = jobs;
    p->status = 1;
    p->year = year;
    p->active = 1;
    ib.total_domestic += amount;
    ib.total_jobs += jobs;
    ib.n_projects++;
    print_str("[IB] Project "); print_int(ib.n_projects - 1);
    print_str(" type="); print_int(type);
    print_str(" zone="); print_int(zone);
    print_str(" inv="); print_int(investor);
    print_str(" amt=$"); print_int(amount);
    print_str(" jobs="); print_int(jobs); print_str("\n");
    return ib.n_projects - 1;
}

int ib_complete_project(int project_id) {
    if (project_id >= ib.n_projects) return -1;
    projects[project_id].status = 2;
    print_str("[IB] Complete Pj"); print_int(project_id); print_str("\n");
    return 0;
}

int ib_record_stat(int zone, int total_inv, int foreign_inv, int domestic_inv, int jobs, int year) {
    if (ib.n_stats >= MAX_STATS) return -1;
    statistic_t* s = &statistics[ib.n_stats];
    s->stat_id = ib.n_stats;
    s->zone = zone;
    s->total_investment = total_inv;
    s->foreign_investment = foreign_inv;
    s->domestic_investment = domestic_inv;
    s->jobs_created = jobs;
    s->year = year;
    s->active = 1;
    ib.n_stats++;
    print_str("[IB] Stat "); print_int(ib.n_stats - 1);
    print_str(" zone="); print_int(zone);
    print_str(" tot=$"); print_int(total_inv);
    print_str(" for=$"); print_int(foreign_inv);
    print_str(" dom=$"); print_int(domestic_inv); print_str("\n");
    return ib.n_stats - 1;
}

void ib_promotion_report(void) {
    print_str("[IB] Promotion report:\n");
    print_str("  Campaigns: "); print_int(ib.n_promotion); print_str("\n");
    print_str("  Total promoted: $"); print_int(ib.total_promoted); print_str("\n");
}

void ib_foreign_report(void) {
    print_str("[IB] Foreign investment report:\n");
    print_str("  Projects: "); print_int(ib.n_foreign); print_str("\n");
    print_str("  Total foreign: $"); print_int(ib.total_foreign); print_str("\n");
    print_str("  Jobs created: "); print_int(ib.total_jobs); print_str("\n");
}

void ib_zone_report(void) {
    print_str("[IB] Economic zone report:\n");
    print_str("  Zones: "); print_int(ib.n_zones); print_str("\n");
    print_str("  Total revenue: $"); print_int(ib.total_zones_revenue); print_str("\n");
    for (int i = 0; i < ib.n_zones; i++) {
        zone_t* z = &zones[i];
        print_str("  Z"); print_int(i);
        print_str(" T"); print_int(z->type);
        print_str(" comp="); print_int(z->companies);
        print_str(" rev=$"); print_int(z->revenue); print_str("\n");
    }
}

void ib_print_state(void) {
    print_str("[IB] Pr="); print_int(ib.n_promotion);
    print_str(" Fg="); print_int(ib.n_foreign);
    print_str(" Zn="); print_int(ib.n_zones);
    print_str(" Pj="); print_int(ib.n_projects);
    print_str(" St="); print_int(ib.n_stats);
    print_str("\n");
}

int main(void) {
    print_str("=== Investment Board Demo ===\n\n");
    ib_init();

    print_str("Adding promotions...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 3) + 1;
        int zone = (i % 4) + 1;
        int target = 5000000 + (i * 1000000);
        int actual = 4000000 + (i * 800000);
        int met = 20 + (i * 5);
        int deals = 3 + (i % 5);
        int year = 2020 + (i % 5);
        ib_add_promotion(type, zone, target, actual, met, deals, year);
    }

    print_str("\nApproving foreign investments...\n");
    for (int i = 0; i < 14; i++) {
        int country = (i % 8) + 1;
        int zone = (i % 4) + 1;
        int amount = 2000000 + (i * 500000);
        int sector = (i % 5) + 1;
        int jobs = 200 + (i * 100);
        int year = 2020 + (i % 5);
        ib_approve_foreign(country, zone, amount, sector, jobs, year);
    }

    print_str("\nEstablishing zones...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 3) + 1;
        int region = (i % 4) + 1;
        int area = 500 + (i * 200);
        int companies = 20 + (i * 10);
        int investment = 10000000 + (i * 5000000);
        int year = 2010 + (i * 2);
        ib_establish_zone(type, region, area, companies, investment, year);
    }

    print_str("\nGenerating zone revenue...\n");
    for (int i = 0; i < 10; i++) {
        int revenue = 2000000 + (i * 500000);
        ib_zone_revenue(i, revenue);
    }

    print_str("\nRegistering projects...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 4) + 1;
        int zone = (i % 4) + 1;
        int investor = 300 + i;
        int amount = 1000000 + (i * 300000);
        int jobs = 100 + (i * 50);
        int year = 2022 + (i % 3);
        ib_register_project(type, zone, investor, amount, jobs, year);
    }

    print_str("\nCompleting projects...\n");
    for (int i = 0; i < 12; i++) {
        ib_complete_project(i);
    }

    print_str("\nRecording statistics...\n");
    for (int i = 0; i < 10; i++) {
        int zone = (i % 4) + 1;
        int total = 8000000 + (i * 2000000);
        int foreign = 3000000 + (i * 800000);
        int domestic = total - foreign;
        int jobs = 500 + (i * 200);
        int year = 2020 + i;
        ib_record_stat(zone, total, foreign, domestic, jobs, year);
    }

    print_str("\nPromotion report...\n");
    ib_promotion_report();

    print_str("\nForeign investment report...\n");
    ib_foreign_report();

    print_str("\nZone report...\n");
    ib_zone_report();

    print_str("\nFinal state...\n");
    ib_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
