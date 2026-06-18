/* government_affairs: Government affairs administration system (v1.0)
 * Administrative approval, public service, digital government, licensing, transparency
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

#define MAX_APPROVAL     16
#define MAX_SERVICE      14
#define MAX_DIGITAL      12
#define MAX_LICENSING    10
#define MAX_OPENNESS     10

typedef struct {
    int    approval_id;
    int    approval_type;
    int    service_category;
    int    applications_received;
    int    applications_approved;
    int    processing_time;
    int    year;
    int    active;
} approval_t;

typedef struct {
    int    service_id;
    int    service_type;
    int    service_level;
    int    service_requests;
    int    complaints_handled;
    int    satisfaction_rate;
    int    year;
    int    active;
} service_t;

typedef struct {
    int    digital_id;
    int    platform_type;
    int    service_domain;
    int    online_services;
    int    data_shared;
    int    users_registered;
    int    year;
    int    active;
} digital_t;

typedef struct {
    int    licensing_id;
    int    license_type;
    int    industry_sector;
    int    licenses_issued;
    int    certifications_granted;
    int    annual_reviews;
    int    year;
    int    active;
} licensing_t;

typedef struct {
    int    openness_id;
    int    disclosure_type;
    int    information_category;
    int    policies_published;
    int    public_responses;
    int    participation_events;
    int    year;
    int    active;
} openness_t;

typedef struct {
    int    n_approval;
    int    n_service;
    int    n_digital;
    int    n_licensing;
    int    n_openness;
    int    total_approved;
    int    total_requests;
    int    total_online;
    int    total_licenses;
    int    total_policies;
} ga_state_t;

static approval_t approvals[MAX_APPROVAL];
static service_t services[MAX_SERVICE];
static digital_t digitals[MAX_DIGITAL];
static licensing_t licenses[MAX_LICENSING];
static openness_t opennesses[MAX_OPENNESS];
static ga_state_t ga;

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

int ga_init(void) {
    if (initialized) return -1;
    ga.n_approval = 0; ga.n_service = 0; ga.n_digital = 0;
    ga.n_licensing = 0; ga.n_openness = 0;
    ga.total_approved = 0; ga.total_requests = 0;
    ga.total_online = 0; ga.total_licenses = 0;
    ga.total_policies = 0;
    for (int i = 0; i < MAX_APPROVAL; i++) approvals[i].active = 0;
    for (int i = 0; i < MAX_SERVICE; i++) services[i].active = 0;
    for (int i = 0; i < MAX_DIGITAL; i++) digitals[i].active = 0;
    for (int i = 0; i < MAX_LICENSING; i++) licenses[i].active = 0;
    for (int i = 0; i < MAX_OPENNESS; i++) opennesses[i].active = 0;
    initialized = 1;
    print_str("[GA] Government affairs initialized\n");
    return 0;
}

int ga_approval(int app_type, int svc_cat, int received, int approved_cnt, int proc_time, int year) {
    if (ga.n_approval >= MAX_APPROVAL) return -1;
    approval_t* a = &approvals[ga.n_approval];
    a->approval_id = ga.n_approval;
    a->approval_type = app_type;
    a->service_category = svc_cat;
    a->applications_received = received;
    a->applications_approved = approved_cnt;
    a->processing_time = proc_time;
    a->year = year;
    a->active = 1;
    ga.total_approved += approved_cnt;
    ga.n_approval++;
    print_str("[GA] Approval "); print_int(ga.n_approval - 1);
    print_str(" type="); print_int(app_type);
    print_str(" cat="); print_int(svc_cat);
    print_str(" rcv="); print_int(received);
    print_str(" apr="); print_int(approved_cnt);
    print_str(" tm="); print_int(proc_time); print_str("\n");
    return ga.n_approval - 1;
}

int ga_service(int svc_type, int level, int requests, int complaints, int satisfaction, int year) {
    if (ga.n_service >= MAX_SERVICE) return -1;
    service_t* s = &services[ga.n_service];
    s->service_id = ga.n_service;
    s->service_type = svc_type;
    s->service_level = level;
    s->service_requests = requests;
    s->complaints_handled = complaints;
    s->satisfaction_rate = satisfaction;
    s->year = year;
    s->active = 1;
    ga.total_requests += requests;
    ga.n_service++;
    print_str("[GA] Service "); print_int(ga.n_service - 1);
    print_str(" type="); print_int(svc_type);
    print_str(" lvl="); print_int(level);
    print_str(" req="); print_int(requests);
    print_str(" cmp="); print_int(complaints);
    print_str(" sat="); print_int(satisfaction); print_str("\n");
    return ga.n_service - 1;
}

int ga_digital(int platform, int domain, int online, int shared, int users, int year) {
    if (ga.n_digital >= MAX_DIGITAL) return -1;
    digital_t* d = &digitals[ga.n_digital];
    d->digital_id = ga.n_digital;
    d->platform_type = platform;
    d->service_domain = domain;
    d->online_services = online;
    d->data_shared = shared;
    d->users_registered = users;
    d->year = year;
    d->active = 1;
    ga.total_online += online;
    ga.n_digital++;
    print_str("[GA] Digital "); print_int(ga.n_digital - 1);
    print_str(" plt="); print_int(platform);
    print_str(" dom="); print_int(domain);
    print_str(" onl="); print_int(online);
    print_str(" shr="); print_int(shared);
    print_str(" usr="); print_int(users); print_str("\n");
    return ga.n_digital - 1;
}

int ga_licensing(int lic_type, int sector, int issued, int certs, int reviews, int year) {
    if (ga.n_licensing >= MAX_LICENSING) return -1;
    licensing_t* l = &licenses[ga.n_licensing];
    l->licensing_id = ga.n_licensing;
    l->license_type = lic_type;
    l->industry_sector = sector;
    l->licenses_issued = issued;
    l->certifications_granted = certs;
    l->annual_reviews = reviews;
    l->year = year;
    l->active = 1;
    ga.total_licenses += issued;
    ga.n_licensing++;
    print_str("[GA] Licensing "); print_int(ga.n_licensing - 1);
    print_str(" type="); print_int(lic_type);
    print_str(" sec="); print_int(sector);
    print_str(" iss="); print_int(issued);
    print_str(" crt="); print_int(certs);
    print_str(" rvw="); print_int(reviews); print_str("\n");
    return ga.n_licensing - 1;
}

int ga_openness(int disc_type, int info_cat, int published, int responses, int events, int year) {
    if (ga.n_openness >= MAX_OPENNESS) return -1;
    openness_t* o = &opennesses[ga.n_openness];
    o->openness_id = ga.n_openness;
    o->disclosure_type = disc_type;
    o->information_category = info_cat;
    o->policies_published = published;
    o->public_responses = responses;
    o->participation_events = events;
    o->year = year;
    o->active = 1;
    ga.total_policies += published;
    ga.n_openness++;
    print_str("[GA] Openness "); print_int(ga.n_openness - 1);
    print_str(" type="); print_int(disc_type);
    print_str(" cat="); print_int(info_cat);
    print_str(" pub="); print_int(published);
    print_str(" rsp="); print_int(responses);
    print_str(" evt="); print_int(events); print_str("\n");
    return ga.n_openness - 1;
}

void ga_approval_report(void) {
    print_str("[GA] Approval report:\n");
    print_str("  Approval categories: "); print_int(ga.n_approval); print_str("\n");
    print_str("  Total approved: "); print_int(ga.total_approved); print_str("\n");
}

void ga_service_report(void) {
    print_str("[GA] Service report:\n");
    print_str("  Service types: "); print_int(ga.n_service); print_str("\n");
    print_str("  Total requests: "); print_int(ga.total_requests); print_str("\n");
}

void ga_openness_report(void) {
    print_str("[GA] Openness report:\n");
    print_str("  Digital platforms: "); print_int(ga.n_digital); print_str("\n");
    print_str("  Total online services: "); print_int(ga.total_online); print_str("\n");
    print_str("  License categories: "); print_int(ga.n_licensing); print_str("\n");
    print_str("  Total licenses: "); print_int(ga.total_licenses); print_str("\n");
    print_str("  Openness categories: "); print_int(ga.n_openness); print_str("\n");
    print_str("  Total policies: "); print_int(ga.total_policies); print_str("\n");
}

void ga_print_state(void) {
    print_str("[GA] Ap="); print_int(ga.n_approval);
    print_str(" Sv="); print_int(ga.n_service);
    print_str(" Dg="); print_int(ga.n_digital);
    print_str(" Lc="); print_int(ga.n_licensing);
    print_str(" Op="); print_int(ga.n_openness);
    print_str("\n");
}

int main(void) {
    print_str("=== Government Affairs Demo ===\n\n");
    ga_init();

    print_str("Administrative approvals...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 6) + 1;
        int rcv = 200 + (i * 50);
        int apr = 150 + (i * 40);
        int tm = 5 + (i % 10);
        int year = 2020 + (i % 5);
        ga_approval(type, cat, rcv, apr, tm, year);
    }

    print_str("\nPublic services...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int lvl = (i % 5) + 1;
        int req = 500 + (i * 100);
        int cmp = 20 + (i * 5);
        int sat = 80 + (i % 20);
        int year = 2021 + (i % 4);
        ga_service(type, lvl, req, cmp, sat, year);
    }

    print_str("\nDigital government...\n");
    for (int i = 0; i < 12; i++) {
        int plt = (i % 4) + 1;
        int dom = (i % 5) + 1;
        int onl = 50 + (i * 15);
        int shr = 100 + (i * 30);
        int usr = 10000 + (i * 3000);
        int year = 2022 + (i % 3);
        ga_digital(plt, dom, onl, shr, usr, year);
    }

    print_str("\nLicense management...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 5) + 1;
        int sec = (i % 6) + 1;
        int iss = 80 + (i * 20);
        int crt = 40 + (i * 10);
        int rvw = 30 + (i * 8);
        int year = 2023 + (i % 2);
        ga_licensing(type, sec, iss, crt, rvw, year);
    }

    print_str("\nGovernment openness...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pub = 25 + (i * 6);
        int rsp = 50 + (i * 15);
        int evt = 10 + (i * 3);
        int year = 2024;
        ga_openness(type, cat, pub, rsp, evt, year);
    }

    print_str("\nApproval report...\n");
    ga_approval_report();

    print_str("\nService report...\n");
    ga_service_report();

    print_str("\nOpenness report...\n");
    ga_openness_report();

    print_str("\nFinal state...\n");
    ga_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
