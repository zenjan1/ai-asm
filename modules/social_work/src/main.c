/* social_work: Social work administration system (v1.0)
 * Social worker management, service delivery, project management, organization, volunteer
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

#define MAX_WORKER       16
#define MAX_SERVICE_SW   14
#define MAX_PROJECT      12
#define MAX_ORGANIZATION 10
#define MAX_VOLUNTEER_SW 10

typedef struct {
    int    worker_id;
    int    worker_level;
    int    specialization;
    int    workers_registered;
    int    certifications_issued;
    int    training_hours;
    int    year;
    int    active;
} worker_t;

typedef struct {
    int    service_id;
    int    service_type;
    int    target_group;
    int    cases_handled;
    int    clients_served;
    int    home_visits;
    int    year;
    int    active;
} service_sw_t;

typedef struct {
    int    project_id;
    int    project_type;
    int    service_domain;
    int    projects_approved;
    int    funding_allocated;
    int    beneficiaries;
    int    year;
    int    active;
} project_t;

typedef struct {
    int    org_id;
    int    org_type;
    int    region_id;
    int    organizations_registered;
    int    staff_employed;
    int    evaluation_score;
    int    year;
    int    active;
} organization_t;

typedef struct {
    int    volunteer_id;
    int    volunteer_type;
    int    service_area;
    int    volunteers_recruited;
    int    service_hours;
    int    points_awarded;
    int    year;
    int    active;
} volunteer_sw_t;

typedef struct {
    int    n_worker;
    int    n_service_sw;
    int    n_project;
    int    n_organization;
    int    n_volunteer_sw;
    int    total_workers;
    int    total_cases;
    int    total_beneficiaries;
    int    total_orgs;
    int    total_volunteers;
} sw_state_t;

static worker_t workers[MAX_WORKER];
static service_sw_t services_sw[MAX_SERVICE_SW];
static project_t projects[MAX_PROJECT];
static organization_t organizations[MAX_ORGANIZATION];
static volunteer_sw_t volunteers_sw[MAX_VOLUNTEER_SW];
static sw_state_t sw;

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

int sw_init(void) {
    if (initialized) return -1;
    sw.n_worker = 0; sw.n_service_sw = 0; sw.n_project = 0;
    sw.n_organization = 0; sw.n_volunteer_sw = 0;
    sw.total_workers = 0; sw.total_cases = 0;
    sw.total_beneficiaries = 0; sw.total_orgs = 0;
    sw.total_volunteers = 0;
    for (int i = 0; i < MAX_WORKER; i++) workers[i].active = 0;
    for (int i = 0; i < MAX_SERVICE_SW; i++) services_sw[i].active = 0;
    for (int i = 0; i < MAX_PROJECT; i++) projects[i].active = 0;
    for (int i = 0; i < MAX_ORGANIZATION; i++) organizations[i].active = 0;
    for (int i = 0; i < MAX_VOLUNTEER_SW; i++) volunteers_sw[i].active = 0;
    initialized = 1;
    print_str("[SW] Social work initialized\n");
    return 0;
}

int sw_worker(int level, int spec, int registered, int certs, int hours, int year) {
    if (sw.n_worker >= MAX_WORKER) return -1;
    worker_t* w = &workers[sw.n_worker];
    w->worker_id = sw.n_worker;
    w->worker_level = level;
    w->specialization = spec;
    w->workers_registered = registered;
    w->certifications_issued = certs;
    w->training_hours = hours;
    w->year = year;
    w->active = 1;
    sw.total_workers += registered;
    sw.n_worker++;
    print_str("[SW] Worker "); print_int(sw.n_worker - 1);
    print_str(" lvl="); print_int(level);
    print_str(" spec="); print_int(spec);
    print_str(" reg="); print_int(registered);
    print_str(" crt="); print_int(certs);
    print_str(" hrs="); print_int(hours); print_str("\n");
    return sw.n_worker - 1;
}

int sw_service(int svc_type, int target, int cases, int clients, int visits, int year) {
    if (sw.n_service_sw >= MAX_SERVICE_SW) return -1;
    service_sw_t* s = &services_sw[sw.n_service_sw];
    s->service_id = sw.n_service_sw;
    s->service_type = svc_type;
    s->target_group = target;
    s->cases_handled = cases;
    s->clients_served = clients;
    s->home_visits = visits;
    s->year = year;
    s->active = 1;
    sw.total_cases += cases;
    sw.n_service_sw++;
    print_str("[SW] Service "); print_int(sw.n_service_sw - 1);
    print_str(" type="); print_int(svc_type);
    print_str(" tgt="); print_int(target);
    print_str(" cas="); print_int(cases);
    print_str(" clt="); print_int(clients);
    print_str(" vst="); print_int(visits); print_str("\n");
    return sw.n_service_sw - 1;
}

int sw_project(int proj_type, int domain, int approved, int funding, int beneficiaries_cnt, int year) {
    if (sw.n_project >= MAX_PROJECT) return -1;
    project_t* p = &projects[sw.n_project];
    p->project_id = sw.n_project;
    p->project_type = proj_type;
    p->service_domain = domain;
    p->projects_approved = approved;
    p->funding_allocated = funding;
    p->beneficiaries = beneficiaries_cnt;
    p->year = year;
    p->active = 1;
    sw.total_beneficiaries += beneficiaries_cnt;
    sw.n_project++;
    print_str("[SW] Project "); print_int(sw.n_project - 1);
    print_str(" type="); print_int(proj_type);
    print_str(" dom="); print_int(domain);
    print_str(" apr="); print_int(approved);
    print_str(" fnd="); print_int(funding);
    print_str(" ben="); print_int(beneficiaries_cnt); print_str("\n");
    return sw.n_project - 1;
}

int sw_organization(int org_type, int region, int registered_cnt, int staff, int score, int year) {
    if (sw.n_organization >= MAX_ORGANIZATION) return -1;
    organization_t* o = &organizations[sw.n_organization];
    o->org_id = sw.n_organization;
    o->org_type = org_type;
    o->region_id = region;
    o->organizations_registered = registered_cnt;
    o->staff_employed = staff;
    o->evaluation_score = score;
    o->year = year;
    o->active = 1;
    sw.total_orgs += registered_cnt;
    sw.n_organization++;
    print_str("[SW] Organization "); print_int(sw.n_organization - 1);
    print_str(" type="); print_int(org_type);
    print_str(" rgn="); print_int(region);
    print_str(" reg="); print_int(registered_cnt);
    print_str(" stf="); print_int(staff);
    print_str(" scr="); print_int(score); print_str("\n");
    return sw.n_organization - 1;
}

int sw_volunteer(int vol_type, int area, int recruited, int hours, int points, int year) {
    if (sw.n_volunteer_sw >= MAX_VOLUNTEER_SW) return -1;
    volunteer_sw_t* v = &volunteers_sw[sw.n_volunteer_sw];
    v->volunteer_id = sw.n_volunteer_sw;
    v->volunteer_type = vol_type;
    v->service_area = area;
    v->volunteers_recruited = recruited;
    v->service_hours = hours;
    v->points_awarded = points;
    v->year = year;
    v->active = 1;
    sw.total_volunteers += recruited;
    sw.n_volunteer_sw++;
    print_str("[SW] Volunteer "); print_int(sw.n_volunteer_sw - 1);
    print_str(" type="); print_int(vol_type);
    print_str(" area="); print_int(area);
    print_str(" rec="); print_int(recruited);
    print_str(" hrs="); print_int(hours);
    print_str(" pts="); print_int(points); print_str("\n");
    return sw.n_volunteer_sw - 1;
}

void sw_worker_report(void) {
    print_str("[SW] Worker report:\n");
    print_str("  Worker levels: "); print_int(sw.n_worker); print_str("\n");
    print_str("  Total workers: "); print_int(sw.total_workers); print_str("\n");
}

void sw_service_report(void) {
    print_str("[SW] Service report:\n");
    print_str("  Service types: "); print_int(sw.n_service_sw); print_str("\n");
    print_str("  Total cases: "); print_int(sw.total_cases); print_str("\n");
}

void sw_project_report(void) {
    print_str("[SW] Project report:\n");
    print_str("  Project categories: "); print_int(sw.n_project); print_str("\n");
    print_str("  Total beneficiaries: "); print_int(sw.total_beneficiaries); print_str("\n");
    print_str("  Organizations: "); print_int(sw.n_organization); print_str("\n");
    print_str("  Total orgs registered: "); print_int(sw.total_orgs); print_str("\n");
    print_str("  Volunteer categories: "); print_int(sw.n_volunteer_sw); print_str("\n");
    print_str("  Total volunteers: "); print_int(sw.total_volunteers); print_str("\n");
}

void sw_print_state(void) {
    print_str("[SW] Wk="); print_int(sw.n_worker);
    print_str(" Sv="); print_int(sw.n_service_sw);
    print_str(" Pj="); print_int(sw.n_project);
    print_str(" Og="); print_int(sw.n_organization);
    print_str(" Vl="); print_int(sw.n_volunteer_sw);
    print_str("\n");
}

int main(void) {
    print_str("=== Social Work Demo ===\n\n");
    sw_init();

    print_str("Social worker management...\n");
    for (int i = 0; i < 16; i++) {
        int lvl = (i % 4) + 1;
        int spec = (i % 6) + 1;
        int reg = 200 + (i * 50);
        int crt = 100 + (i * 25);
        int hrs = 40 + (i * 10);
        int year = 2020 + (i % 5);
        sw_worker(lvl, spec, reg, crt, hrs, year);
    }

    print_str("\nService delivery...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 5) + 1;
        int tgt = (i % 4) + 1;
        int cas = 50 + (i * 15);
        int clt = 200 + (i * 50);
        int vst = 100 + (i * 30);
        int year = 2021 + (i % 4);
        sw_service(type, tgt, cas, clt, vst, year);
    }

    print_str("\nProject management...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int dom = (i % 5) + 1;
        int apr = 10 + (i * 3);
        int fnd = 100 + (i * 30);
        int ben = 500 + (i * 150);
        int year = 2022 + (i % 3);
        sw_project(type, dom, apr, fnd, ben, year);
    }

    print_str("\nOrganization management...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int rgn = (i % 8) + 1;
        int reg = 20 + (i * 5);
        int stf = 50 + (i * 15);
        int scr = 75 + (i % 25);
        int year = 2023 + (i % 2);
        sw_organization(type, rgn, reg, stf, scr, year);
    }

    print_str("\nVolunteer service...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int area = (i % 5) + 1;
        int rec = 300 + (i * 80);
        int hrs = 1000 + (i * 300);
        int pts = 50 + (i * 15);
        int year = 2024;
        sw_volunteer(type, area, rec, hrs, pts, year);
    }

    print_str("\nWorker report...\n");
    sw_worker_report();

    print_str("\nService report...\n");
    sw_service_report();

    print_str("\nProject report...\n");
    sw_project_report();

    print_str("\nFinal state...\n");
    sw_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
