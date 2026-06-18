/* hong_kong_macao: Hong Kong & Macao affairs administration system (v1.0)
 * Liaison, policy research, economic cooperation, cultural exchange, resident services
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

#define MAX_LIAISON      16
#define MAX_POLICY       14
#define MAX_ECONOMIC     12
#define MAX_CULTURAL     10
#define MAX_RESIDENT     10

typedef struct {
    int    liaison_id;
    int    office_type;
    int    region_id;
    int    personnel;
    int    meetings_held;
    int    cooperation_projects;
    int    year;
    int    active;
} liaison_t;

typedef struct {
    int    policy_id;
    int    policy_area;
    int    research_type;
    int    reports_produced;
    int    recommendations;
    int    implemented;
    int    year;
    int    active;
} policy_t;

typedef struct {
    int    economic_id;
    int    sector;
    int    cooperation_type;
    int    investment_amount;
    int    trade_volume;
    int    projects_signed;
    int    year;
    int    active;
} economic_t;

typedef struct {
    int    cultural_id;
    int    activity_type;
    int    region_id;
    int    events_held;
    int    participants;
    int    youth_exchanges;
    int    year;
    int    active;
} cultural_t;

typedef struct {
    int    resident_id;
    int    service_type;
    int    document_type;
    int    applications;
    int    approved;
    int    processed;
    int    year;
    int    active;
} resident_t;

typedef struct {
    int    n_liaison;
    int    n_policy;
    int    n_economic;
    int    n_cultural;
    int    n_resident;
    int    total_personnel;
    int    total_investment;
    int    total_trade;
    int    total_participants;
    int    total_applications;
} hm_state_t;

static liaison_t liaisons[MAX_LIAISON];
static policy_t policies[MAX_POLICY];
static economic_t economics[MAX_ECONOMIC];
static cultural_t cultural[MAX_CULTURAL];
static resident_t residents[MAX_RESIDENT];
static hm_state_t hm;

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

int hm_init(void) {
    if (initialized) return -1;
    hm.n_liaison = 0; hm.n_policy = 0; hm.n_economic = 0;
    hm.n_cultural = 0; hm.n_resident = 0;
    hm.total_personnel = 0; hm.total_investment = 0;
    hm.total_trade = 0; hm.total_participants = 0;
    hm.total_applications = 0;
    for (int i = 0; i < MAX_LIAISON; i++) liaisons[i].active = 0;
    for (int i = 0; i < MAX_POLICY; i++) policies[i].active = 0;
    for (int i = 0; i < MAX_ECONOMIC; i++) economics[i].active = 0;
    for (int i = 0; i < MAX_CULTURAL; i++) cultural[i].active = 0;
    for (int i = 0; i < MAX_RESIDENT; i++) residents[i].active = 0;
    initialized = 1;
    print_str("[HM] HK/Macao admin initialized\n");
    return 0;
}

int hm_liaison(int office, int region, int personnel, int meetings, int projects, int year) {
    if (hm.n_liaison >= MAX_LIAISON) return -1;
    liaison_t* l = &liaisons[hm.n_liaison];
    l->liaison_id = hm.n_liaison;
    l->office_type = office;
    l->region_id = region;
    l->personnel = personnel;
    l->meetings_held = meetings;
    l->cooperation_projects = projects;
    l->year = year;
    l->active = 1;
    hm.total_personnel += personnel;
    hm.n_liaison++;
    print_str("[HM] Liaison "); print_int(hm.n_liaison - 1);
    print_str(" off="); print_int(office);
    print_str(" rgn="); print_int(region);
    print_str(" per="); print_int(personnel);
    print_str(" mtg="); print_int(meetings);
    print_str(" prj="); print_int(projects); print_str("\n");
    return hm.n_liaison - 1;
}

int hm_policy(int area, int research, int reports, int recommendations, int implemented, int year) {
    if (hm.n_policy >= MAX_POLICY) return -1;
    policy_t* p = &policies[hm.n_policy];
    p->policy_id = hm.n_policy;
    p->policy_area = area;
    p->research_type = research;
    p->reports_produced = reports;
    p->recommendations = recommendations;
    p->implemented = implemented;
    p->year = year;
    p->active = 1;
    hm.n_policy++;
    print_str("[HM] Policy "); print_int(hm.n_policy - 1);
    print_str(" area="); print_int(area);
    print_str(" type="); print_int(research);
    print_str(" rpt="); print_int(reports);
    print_str(" rec="); print_int(recommendations);
    print_str(" imp="); print_int(implemented); print_str("\n");
    return hm.n_policy - 1;
}

int hm_economic(int sector, int coop_type, int investment, int trade, int projects, int year) {
    if (hm.n_economic >= MAX_ECONOMIC) return -1;
    economic_t* e = &economics[hm.n_economic];
    e->economic_id = hm.n_economic;
    e->sector = sector;
    e->cooperation_type = coop_type;
    e->investment_amount = investment;
    e->trade_volume = trade;
    e->projects_signed = projects;
    e->year = year;
    e->active = 1;
    hm.total_investment += investment;
    hm.total_trade += trade;
    hm.n_economic++;
    print_str("[HM] Economic "); print_int(hm.n_economic - 1);
    print_str(" sec="); print_int(sector);
    print_str(" type="); print_int(coop_type);
    print_str(" inv=$"); print_int(investment);
    print_str(" trd=$"); print_int(trade);
    print_str(" prj="); print_int(projects); print_str("\n");
    return hm.n_economic - 1;
}

int hm_cultural(int act_type, int region, int events, int participants, int youth, int year) {
    if (hm.n_cultural >= MAX_CULTURAL) return -1;
    cultural_t* c = &cultural[hm.n_cultural];
    c->cultural_id = hm.n_cultural;
    c->activity_type = act_type;
    c->region_id = region;
    c->events_held = events;
    c->participants = participants;
    c->youth_exchanges = youth;
    c->year = year;
    c->active = 1;
    hm.total_participants += participants;
    hm.n_cultural++;
    print_str("[HM] Cultural "); print_int(hm.n_cultural - 1);
    print_str(" type="); print_int(act_type);
    print_str(" rgn="); print_int(region);
    print_str(" evt="); print_int(events);
    print_str(" par="); print_int(participants);
    print_str(" yth="); print_int(youth); print_str("\n");
    return hm.n_cultural - 1;
}

int hm_resident(int svc_type, int doc_type, int applications, int approved, int processed, int year) {
    if (hm.n_resident >= MAX_RESIDENT) return -1;
    resident_t* r = &residents[hm.n_resident];
    r->resident_id = hm.n_resident;
    r->service_type = svc_type;
    r->document_type = doc_type;
    r->applications = applications;
    r->approved = approved;
    r->processed = processed;
    r->year = year;
    r->active = 1;
    hm.total_applications += applications;
    hm.n_resident++;
    print_str("[HM] Resident "); print_int(hm.n_resident - 1);
    print_str(" svc="); print_int(svc_type);
    print_str(" doc="); print_int(doc_type);
    print_str(" app="); print_int(applications);
    print_str(" apr="); print_int(approved);
    print_str(" prc="); print_int(processed); print_str("\n");
    return hm.n_resident - 1;
}

void hm_liaison_report(void) {
    print_str("[HM] Liaison report:\n");
    print_str("  Liaison offices: "); print_int(hm.n_liaison); print_str("\n");
    print_str("  Total personnel: "); print_int(hm.total_personnel); print_str("\n");
}

void hm_economic_report(void) {
    print_str("[HM] Economic report:\n");
    print_str("  Economic sectors: "); print_int(hm.n_economic); print_str("\n");
    print_str("  Total investment: $"); print_int(hm.total_investment); print_str("\n");
    print_str("  Total trade: $"); print_int(hm.total_trade); print_str("\n");
}

void hm_cultural_report(void) {
    print_str("[HM] Cultural report:\n");
    print_str("  Policy studies: "); print_int(hm.n_policy); print_str("\n");
    print_str("  Cultural events: "); print_int(hm.n_cultural); print_str("\n");
    print_str("  Total participants: "); print_int(hm.total_participants); print_str("\n");
    print_str("  Resident services: "); print_int(hm.n_resident); print_str("\n");
    print_str("  Total applications: "); print_int(hm.total_applications); print_str("\n");
}

void hm_print_state(void) {
    print_str("[HM] Ln="); print_int(hm.n_liaison);
    print_str(" Pl="); print_int(hm.n_policy);
    print_str(" Ec="); print_int(hm.n_economic);
    print_str(" Cl="); print_int(hm.n_cultural);
    print_str(" Rs="); print_int(hm.n_resident);
    print_str("\n");
}

int main(void) {
    print_str("=== HK/Macao Admin Demo ===\n\n");
    hm_init();

    print_str("Liaison offices...\n");
    for (int i = 0; i < 16; i++) {
        int off = (i % 3) + 1;
        int rgn = (i % 5) + 1;
        int per = 20 + (i * 5);
        int mtg = 10 + (i * 3);
        int prj = 5 + (i * 2);
        int year = 2020 + (i % 5);
        hm_liaison(off, rgn, per, mtg, prj, year);
    }

    print_str("\nPolicy research...\n");
    for (int i = 0; i < 14; i++) {
        int area = (i % 6) + 1;
        int type = (i % 4) + 1;
        int rpt = 10 + (i * 3);
        int rec = rpt / 2;
        int imp = rec - (i % 3);
        int year = 2021 + (i % 4);
        hm_policy(area, type, rpt, rec, imp, year);
    }

    print_str("\nEconomic cooperation...\n");
    for (int i = 0; i < 12; i++) {
        int sec = (i % 5) + 1;
        int type = (i % 4) + 1;
        int inv = 5000000 + (i * 2000000);
        int trd = 20000000 + (i * 5000000);
        int prj = 10 + (i * 3);
        int year = 2022 + (i % 3);
        hm_economic(sec, type, inv, trd, prj, year);
    }

    print_str("\nCultural exchange...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 5) + 1;
        int rgn = (i % 4) + 1;
        int evt = 5 + (i * 2);
        int par = 200 + (i * 50);
        int yth = 50 + (i * 10);
        int year = 2023 + (i % 2);
        hm_cultural(type, rgn, evt, par, yth, year);
    }

    print_str("\nResident services...\n");
    for (int i = 0; i < 10; i++) {
        int svc = (i % 4) + 1;
        int doc = (i % 5) + 1;
        int app = 1000 + (i * 200);
        int apr = app - (i * 20);
        int prc = apr + (i * 10);
        int year = 2024;
        hm_resident(svc, doc, app, apr, prc, year);
    }

    print_str("\nLiaison report...\n");
    hm_liaison_report();

    print_str("\nEconomic report...\n");
    hm_economic_report();

    print_str("\nCultural report...\n");
    hm_cultural_report();

    print_str("\nFinal state...\n");
    hm_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
