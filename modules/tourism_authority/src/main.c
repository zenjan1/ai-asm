/* tourism_authority: Tourism administration system (v1.0)
 * Scenic areas, travel agencies, promotion, safety, statistics
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

#define MAX_SCENIC       16
#define MAX_AGENCY       14
#define MAX_PROMOTION    12
#define MAX_SAFETY       10
#define MAX_STAT         10

typedef struct {
    int    scenic_id;
    int    area_type;
    int    region_id;
    int    rating;
    int    visitors;
    int    revenue;
    int    satisfaction;
    int    year;
    int    active;
} scenic_t;

typedef struct {
    int    agency_id;
    int    license_id;
    int    guides;
    int    tours;
    int    tourists;
    int    revenue;
    int    complaints;
    int    year;
    int    active;
} agency_t;

typedef struct {
    int    promo_id;
    int    campaign_type;
    int    destination_id;
    int    reach;
    int    budget;
    int    effect_score;
    int    year;
    int    active;
} promotion_t;

typedef struct {
    int    safety_id;
    int    location_id;
    int    incident_type;
    int    severity;
    int    response_time;
    int    resolved;
    int    year;
    int    status;
    int    active;
} safety_t;

typedef struct {
    int    stat_id;
    int    region_id;
    int    tourists;
    int    domestic;
    int    inbound;
    int    revenue;
    int    avg_stay_days;
    int    year;
    int    active;
} stat_t;

typedef struct {
    int    n_scenic;
    int    n_agency;
    int    n_promo;
    int    n_safety;
    int    n_stat;
    int    total_visitors;
    int    total_revenue;
    int    total_tours;
    int    total_incidents;
    int    total_domestic;
} ta_state_t;

static scenic_t scenics[MAX_SCENIC];
static agency_t agencies[MAX_AGENCY];
static promotion_t promotions[MAX_PROMOTION];
static safety_t safeties[MAX_SAFETY];
static stat_t stats[MAX_STAT];
static ta_state_t ta;

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

int ta_init(void) {
    if (initialized) return -1;
    ta.n_scenic = 0; ta.n_agency = 0; ta.n_promo = 0;
    ta.n_safety = 0; ta.n_stat = 0;
    ta.total_visitors = 0; ta.total_revenue = 0;
    ta.total_tours = 0; ta.total_incidents = 0;
    ta.total_domestic = 0;
    for (int i = 0; i < MAX_SCENIC; i++) scenics[i].active = 0;
    for (int i = 0; i < MAX_AGENCY; i++) agencies[i].active = 0;
    for (int i = 0; i < MAX_PROMOTION; i++) promotions[i].active = 0;
    for (int i = 0; i < MAX_SAFETY; i++) safeties[i].active = 0;
    for (int i = 0; i < MAX_STAT; i++) stats[i].active = 0;
    initialized = 1;
    print_str("[TA] Tourism authority initialized\n");
    return 0;
}

int ta_scenic(int area_type, int region, int rating, int visitors, int revenue, int satisfaction, int year) {
    if (ta.n_scenic >= MAX_SCENIC) return -1;
    scenic_t* s = &scenics[ta.n_scenic];
    s->scenic_id = ta.n_scenic;
    s->area_type = area_type;
    s->region_id = region;
    s->rating = rating;
    s->visitors = visitors;
    s->revenue = revenue;
    s->satisfaction = satisfaction;
    s->year = year;
    s->active = 1;
    ta.total_visitors += visitors;
    ta.total_revenue += revenue;
    ta.n_scenic++;
    print_str("[TA] Scenic "); print_int(ta.n_scenic - 1);
    print_str(" type="); print_int(area_type);
    print_str(" reg="); print_int(region);
    print_str(" rtt="); print_int(rating);
    print_str(" vst="); print_int(visitors);
    print_str(" rev=$"); print_int(revenue); print_str("\n");
    return ta.n_scenic - 1;
}

int ta_agency(int license, int guides, int tours, int tourists, int revenue, int complaints, int year) {
    if (ta.n_agency >= MAX_AGENCY) return -1;
    agency_t* a = &agencies[ta.n_agency];
    a->agency_id = ta.n_agency;
    a->license_id = license;
    a->guides = guides;
    a->tours = tours;
    a->tourists = tourists;
    a->revenue = revenue;
    a->complaints = complaints;
    a->year = year;
    a->active = 1;
    ta.total_tours += tours;
    ta.n_agency++;
    print_str("[TA] Agency "); print_int(ta.n_agency - 1);
    print_str(" lic="); print_int(license);
    print_str(" gds="); print_int(guides);
    print_str(" trs="); print_int(tours);
    print_str(" tst="); print_int(tourists);
    print_str(" rev=$"); print_int(revenue); print_str("\n");
    return ta.n_agency - 1;
}

int ta_promotion(int campaign, int destination, int reach, int budget, int effect, int year) {
    if (ta.n_promo >= MAX_PROMOTION) return -1;
    promotion_t* p = &promotions[ta.n_promo];
    p->promo_id = ta.n_promo;
    p->campaign_type = campaign;
    p->destination_id = destination;
    p->reach = reach;
    p->budget = budget;
    p->effect_score = effect;
    p->year = year;
    p->active = 1;
    ta.n_promo++;
    print_str("[TA] Promo "); print_int(ta.n_promo - 1);
    print_str(" cmp="); print_int(campaign);
    print_str(" dst="); print_int(destination);
    print_str(" rch="); print_int(reach);
    print_str(" bgt=$"); print_int(budget);
    print_str(" eff="); print_int(effect); print_str("\n");
    return ta.n_promo - 1;
}

int ta_safety(int location, int incident, int severity, int response, int resolved, int year) {
    if (ta.n_safety >= MAX_SAFETY) return -1;
    safety_t* s = &safeties[ta.n_safety];
    s->safety_id = ta.n_safety;
    s->location_id = location;
    s->incident_type = incident;
    s->severity = severity;
    s->response_time = response;
    s->resolved = resolved;
    s->year = year;
    s->status = 1;
    s->active = 1;
    ta.total_incidents++;
    ta.n_safety++;
    print_str("[TA] Safety "); print_int(ta.n_safety - 1);
    print_str(" loc="); print_int(location);
    print_str(" inc="); print_int(incident);
    print_str(" sev="); print_int(severity);
    print_str(" rsp="); print_int(response); print_str("min");
    print_str(" res="); print_int(resolved); print_str("\n");
    return ta.n_safety - 1;
}

int ta_stat(int region, int tourists, int domestic, int inbound, int revenue, int stay, int year) {
    if (ta.n_stat >= MAX_STAT) return -1;
    stat_t* s = &stats[ta.n_stat];
    s->stat_id = ta.n_stat;
    s->region_id = region;
    s->tourists = tourists;
    s->domestic = domestic;
    s->inbound = inbound;
    s->revenue = revenue;
    s->avg_stay_days = stay;
    s->year = year;
    s->active = 1;
    ta.total_domestic += domestic;
    ta.n_stat++;
    print_str("[TA] Stat "); print_int(ta.n_stat - 1);
    print_str(" reg="); print_int(region);
    print_str(" tst="); print_int(tourists);
    print_str(" dom="); print_int(domestic);
    print_str(" inb="); print_int(inbound);
    print_str(" rev=$"); print_int(revenue);
    print_str(" sty="); print_int(stay); print_str("d\n");
    return ta.n_stat - 1;
}

void ta_scenic_report(void) {
    print_str("[TA] Scenic report:\n");
    print_str("  Scenic areas: "); print_int(ta.n_scenic); print_str("\n");
    print_str("  Total visitors: "); print_int(ta.total_visitors); print_str("\n");
    print_str("  Total revenue: $"); print_int(ta.total_revenue); print_str("\n");
}

void ta_agency_report(void) {
    print_str("[TA] Agency report:\n");
    print_str("  Travel agencies: "); print_int(ta.n_agency); print_str("\n");
    print_str("  Total tours: "); print_int(ta.total_tours); print_str("\n");
}

void ta_stat_report(void) {
    print_str("[TA] Statistics report:\n");
    print_str("  Promotions: "); print_int(ta.n_promo); print_str("\n");
    print_str("  Safety incidents: "); print_int(ta.total_incidents); print_str("\n");
    print_str("  Statistics regions: "); print_int(ta.n_stat); print_str("\n");
    print_str("  Total domestic tourists: "); print_int(ta.total_domestic); print_str("\n");
}

void ta_print_state(void) {
    print_str("[TA] Sc="); print_int(ta.n_scenic);
    print_str(" Ag="); print_int(ta.n_agency);
    print_str(" Pr="); print_int(ta.n_promo);
    print_str(" Sf="); print_int(ta.n_safety);
    print_str(" St="); print_int(ta.n_stat);
    print_str("\n");
}

int main(void) {
    print_str("=== Tourism Authority Demo ===\n\n");
    ta_init();

    print_str("Scenic areas...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int reg = (i % 8) + 1;
        int rtt = (i % 5) + 1;
        int vst = 10000 + (i * 5000);
        int rev = 500000 + (i * 200000);
        int sat = 70 + (i * 2);
        int year = 2020 + (i % 5);
        ta_scenic(type, reg, rtt, vst, rev, sat, year);
    }

    print_str("\nTravel agencies...\n");
    for (int i = 0; i < 14; i++) {
        int lic = 1000 + (i * 11);
        int gds = 5 + (i * 3);
        int trs = 20 + (i * 10);
        int tst = 200 + (i * 100);
        int rev = 100000 + (i * 50000);
        int cmp = i % 4;
        int year = 2021 + (i % 4);
        ta_agency(lic, gds, trs, tst, rev, cmp, year);
    }

    print_str("\nTourism promotion...\n");
    for (int i = 0; i < 12; i++) {
        int cmp = (i % 4) + 1;
        int dst = (i % 6) + 1;
        int rch = 5000 + (i * 2000);
        int bgt = 50000 + (i * 20000);
        int eff = 60 + (i * 3);
        int year = 2022 + (i % 3);
        ta_promotion(cmp, dst, rch, bgt, eff, year);
    }

    print_str("\nTourism safety...\n");
    for (int i = 0; i < 10; i++) {
        int loc = 200 + (i * 20);
        int inc = (i % 4) + 1;
        int sev = (i % 3) + 1;
        int rsp = 10 + (i * 5);
        int res = (i % 3 == 0) ? 0 : 1;
        int year = 2023 + (i % 2);
        ta_safety(loc, inc, sev, rsp, res, year);
    }

    print_str("\nTourism statistics...\n");
    for (int i = 0; i < 10; i++) {
        int reg = (i % 8) + 1;
        int tst = 50000 + (i * 20000);
        int dom = tst - (i * 5000);
        int inb = i * 5000;
        int rev = 5000000 + (i * 2000000);
        int sty = 2 + (i % 4);
        int year = 2024;
        ta_stat(reg, tst, dom, inb, rev, sty, year);
    }

    print_str("\nScenic report...\n");
    ta_scenic_report();

    print_str("\nAgency report...\n");
    ta_agency_report();

    print_str("\nStatistics report...\n");
    ta_stat_report();

    print_str("\nFinal state...\n");
    ta_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
