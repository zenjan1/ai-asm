/* radio_television: Radio and television management (v1.0)
 * Programs, transmission, safety, industry, public service
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

#define MAX_PROGRAM      16
#define MAX_TRANSMIT     14
#define MAX_SAFETY       12
#define MAX_INDUSTRY     10
#define MAX_PUBLIC       10

typedef struct {
    int    program_id;
    int    channel_id;
    int    type;
    int    duration_min;
    int    audience;
    int    rating;
    int    reviewed;
    int    year;
    int    active;
} program_t;

typedef struct {
    int    transmit_id;
    int    station_id;
    int    tower_type;
    int    power_kw;
    int    coverage_km;
    int    signal_quality;
    int    year;
    int    active;
} transmit_t;

typedef struct {
    int    safety_id;
    int    station_id;
    int    incident_type;
    int    severity;
    int    downtime_min;
    int    resolved;
    int    year;
    int    status;
    int    active;
} safety_t;

typedef struct {
    int    industry_id;
    int    company_type;
    int    company_id;
    int    subscribers;
    int    revenue;
    int    new_media;
    int    year;
    int    active;
} industry_t;

typedef struct {
    int    public_id;
    int    service_type;
    int    region_id;
    int    households;
    int    coverage_pct;
    int    budget;
    int    year;
    int    active;
} public_svc_t;

typedef struct {
    int    n_program;
    int    n_transmit;
    int    n_safety;
    int    n_industry;
    int    n_public;
    int    total_audience;
    int    total_coverage;
    int    total_incidents;
    int    total_subscribers;
    int    total_households;
} rt_state_t;

static program_t programs[MAX_PROGRAM];
static transmit_t transmits[MAX_TRANSMIT];
static safety_t safeties[MAX_SAFETY];
static industry_t industries[MAX_INDUSTRY];
static public_svc_t public_svcs[MAX_PUBLIC];
static rt_state_t rt;

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

int rt_init(void) {
    if (initialized) return -1;
    rt.n_program = 0; rt.n_transmit = 0; rt.n_safety = 0;
    rt.n_industry = 0; rt.n_public = 0;
    rt.total_audience = 0; rt.total_coverage = 0;
    rt.total_incidents = 0; rt.total_subscribers = 0;
    rt.total_households = 0;
    for (int i = 0; i < MAX_PROGRAM; i++) programs[i].active = 0;
    for (int i = 0; i < MAX_TRANSMIT; i++) transmits[i].active = 0;
    for (int i = 0; i < MAX_SAFETY; i++) safeties[i].active = 0;
    for (int i = 0; i < MAX_INDUSTRY; i++) industries[i].active = 0;
    for (int i = 0; i < MAX_PUBLIC; i++) public_svcs[i].active = 0;
    initialized = 1;
    print_str("[RT] Radio television initialized\n");
    return 0;
}

int rt_program(int channel, int type, int duration, int audience, int rating, int reviewed, int year) {
    if (rt.n_program >= MAX_PROGRAM) return -1;
    program_t* p = &programs[rt.n_program];
    p->program_id = rt.n_program;
    p->channel_id = channel;
    p->type = type;
    p->duration_min = duration;
    p->audience = audience;
    p->rating = rating;
    p->reviewed = reviewed;
    p->year = year;
    p->active = 1;
    rt.total_audience += audience;
    rt.n_program++;
    print_str("[RT] Program "); print_int(rt.n_program - 1);
    print_str(" ch="); print_int(channel);
    print_str(" type="); print_int(type);
    print_str(" dur="); print_int(duration); print_str("min");
    print_str(" aud="); print_int(audience);
    print_str(" rtg="); print_int(rating); print_str("\n");
    return rt.n_program - 1;
}

int rt_transmit(int station, int tower, int power, int coverage, int quality, int year) {
    if (rt.n_transmit >= MAX_TRANSMIT) return -1;
    transmit_t* t = &transmits[rt.n_transmit];
    t->transmit_id = rt.n_transmit;
    t->station_id = station;
    t->tower_type = tower;
    t->power_kw = power;
    t->coverage_km = coverage;
    t->signal_quality = quality;
    t->year = year;
    t->active = 1;
    rt.total_coverage += coverage;
    rt.n_transmit++;
    print_str("[RT] Transmit "); print_int(rt.n_transmit - 1);
    print_str(" stn="); print_int(station);
    print_str(" twr="); print_int(tower);
    print_str(" pwr="); print_int(power); print_str("kW");
    print_str(" cov="); print_int(coverage); print_str("km");
    print_str(" qlt="); print_int(quality); print_str("\n");
    return rt.n_transmit - 1;
}

int rt_safety(int station, int incident, int severity, int downtime, int resolved, int year) {
    if (rt.n_safety >= MAX_SAFETY) return -1;
    safety_t* s = &safeties[rt.n_safety];
    s->safety_id = rt.n_safety;
    s->station_id = station;
    s->incident_type = incident;
    s->severity = severity;
    s->downtime_min = downtime;
    s->resolved = resolved;
    s->year = year;
    s->status = 1;
    s->active = 1;
    rt.total_incidents++;
    rt.n_safety++;
    print_str("[RT] Safety "); print_int(rt.n_safety - 1);
    print_str(" stn="); print_int(station);
    print_str(" inc="); print_int(incident);
    print_str(" sev="); print_int(severity);
    print_str(" dwn="); print_int(downtime); print_str("min");
    print_str(" res="); print_int(resolved); print_str("\n");
    return rt.n_safety - 1;
}

int rt_industry(int company_type, int company, int subscribers, int revenue, int new_media, int year) {
    if (rt.n_industry >= MAX_INDUSTRY) return -1;
    industry_t* ind = &industries[rt.n_industry];
    ind->industry_id = rt.n_industry;
    ind->company_type = company_type;
    ind->company_id = company;
    ind->subscribers = subscribers;
    ind->revenue = revenue;
    ind->new_media = new_media;
    ind->year = year;
    ind->active = 1;
    rt.total_subscribers += subscribers;
    rt.n_industry++;
    print_str("[RT] Industry "); print_int(rt.n_industry - 1);
    print_str(" typ="); print_int(company_type);
    print_str(" co="); print_int(company);
    print_str(" sub="); print_int(subscribers);
    print_str(" rev=$"); print_int(revenue);
    print_str(" nwm="); print_int(new_media); print_str("\n");
    return rt.n_industry - 1;
}

int rt_public(int service, int region, int households, int coverage, int budget, int year) {
    if (rt.n_public >= MAX_PUBLIC) return -1;
    public_svc_t* p = &public_svcs[rt.n_public];
    p->public_id = rt.n_public;
    p->service_type = service;
    p->region_id = region;
    p->households = households;
    p->coverage_pct = coverage;
    p->budget = budget;
    p->year = year;
    p->active = 1;
    rt.total_households += households;
    rt.n_public++;
    print_str("[RT] Public "); print_int(rt.n_public - 1);
    print_str(" svc="); print_int(service);
    print_str(" reg="); print_int(region);
    print_str(" hsh="); print_int(households);
    print_str(" cov="); print_int(coverage); print_str("%");
    print_str(" bgt=$"); print_int(budget); print_str("\n");
    return rt.n_public - 1;
}

void rt_program_report(void) {
    print_str("[RT] Program report:\n");
    print_str("  Programs: "); print_int(rt.n_program); print_str("\n");
    print_str("  Total audience: "); print_int(rt.total_audience); print_str("\n");
}

void rt_transmit_report(void) {
    print_str("[RT] Transmission report:\n");
    print_str("  Stations: "); print_int(rt.n_transmit); print_str("\n");
    print_str("  Total coverage: "); print_int(rt.total_coverage); print_str(" km\n");
}

void rt_industry_report(void) {
    print_str("[RT] Industry report:\n");
    print_str("  Companies: "); print_int(rt.n_industry); print_str("\n");
    print_str("  Total subscribers: "); print_int(rt.total_subscribers); print_str("\n");
    print_str("  Safety incidents: "); print_int(rt.total_incidents); print_str("\n");
    print_str("  Public services: "); print_int(rt.n_public); print_str("\n");
    print_str("  Total households: "); print_int(rt.total_households); print_str("\n");
}

void rt_print_state(void) {
    print_str("[RT] Pr="); print_int(rt.n_program);
    print_str(" Tr="); print_int(rt.n_transmit);
    print_str(" Sf="); print_int(rt.n_safety);
    print_str(" In="); print_int(rt.n_industry);
    print_str(" Pb="); print_int(rt.n_public);
    print_str("\n");
}

int main(void) {
    print_str("=== Radio Television Demo ===\n\n");
    rt_init();

    print_str("Program management...\n");
    for (int i = 0; i < 16; i++) {
        int ch = (i % 8) + 1;
        int type = (i % 5) + 1;
        int dur = 30 + (i * 15);
        int aud = 10000 + (i * 5000);
        int rtg = 50 + (i * 3);
        int rvd = (i % 3 == 0) ? 0 : 1;
        int year = 2021 + (i % 4);
        rt_program(ch, type, dur, aud, rtg, rvd, year);
    }

    print_str("\nTransmission coverage...\n");
    for (int i = 0; i < 14; i++) {
        int stn = 100 + (i * 10);
        int twr = (i % 3) + 1;
        int pwr = 5 + (i * 3);
        int cov = 20 + (i * 10);
        int qlt = 70 + (i * 2);
        int year = 2022 + (i % 3);
        rt_transmit(stn, twr, pwr, cov, qlt, year);
    }

    print_str("\nBroadcast safety...\n");
    for (int i = 0; i < 12; i++) {
        int stn = 200 + (i * 15);
        int inc = (i % 4) + 1;
        int sev = (i % 3) + 1;
        int dwn = 5 + (i * 10);
        int res = (i % 3 == 0) ? 0 : 1;
        int year = 2023 + (i % 2);
        rt_safety(stn, inc, sev, dwn, res, year);
    }

    print_str("\nBroadcast industry...\n");
    for (int i = 0; i < 10; i++) {
        int ctype = (i % 3) + 1;
        int co = 3000 + (i * 7);
        int sub = 5000 + (i * 2000);
        int rev = 500000 + (i * 200000);
        int nwm = (i % 3 == 0) ? 1 : 0;
        int year = 2024;
        rt_industry(ctype, co, sub, rev, nwm, year);
    }

    print_str("\nPublic services...\n");
    for (int i = 0; i < 10; i++) {
        int svc = (i % 3) + 1;
        int reg = (i % 6) + 1;
        int hsh = 1000 + (i * 500);
        int cov = 70 + (i * 3);
        int bgt = 100000 + (i * 50000);
        int year = 2024;
        rt_public(svc, reg, hsh, cov, bgt, year);
    }

    print_str("\nProgram report...\n");
    rt_program_report();

    print_str("\nTransmission report...\n");
    rt_transmit_report();

    print_str("\nIndustry report...\n");
    rt_industry_report();

    print_str("\nFinal state...\n");
    rt_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
