/* volunteer_service: Volunteer service administration system (v1.0)
 * Volunteer registration, project management, organization, service records, coordination
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

#define MAX_REGISTRATION 16
#define MAX_VS_PROJECT   14
#define MAX_TEAM         12
#define MAX_RECORD       10
#define MAX_COORDINATION 10

typedef struct {
    int    registration_id;
    int    volunteer_category;
    int    skill_level;
    int    volunteers_registered;
    int    training_completed;
    int    certifications_issued;
    int    year;
    int    active;
} registration_t;

typedef struct {
    int    project_id;
    int    project_type;
    int    service_domain;
    int    projects_published;
    int    volunteers_recruited;
    int    project_completed;
    int    year;
    int    active;
} vs_project_t;

typedef struct {
    int    team_id;
    int    team_type;
    int    region_id;
    int    teams_formed;
    int    activities_organized;
    int    awards_given;
    int    year;
    int    active;
} team_t;

typedef struct {
    int    record_id;
    int    record_type;
    int    service_category;
    int    hours_recorded;
    int    evaluations_done;
    int    certificates_issued;
    int    year;
    int    active;
} record_t;

typedef struct {
    int    coordination_id;
    int    coord_type;
    int    region_pair;
    int    needs_matched;
    int    resources_allocated;
    int    regional_events;
    int    year;
    int    active;
} coordination_t;

typedef struct {
    int    n_registration;
    int    n_project;
    int    n_team;
    int    n_record;
    int    n_coordination;
    int    total_volunteers;
    int    total_projects;
    int    total_teams;
    int    total_hours;
    int    total_certificates;
} vs_state_t;

static registration_t registrations[MAX_REGISTRATION];
static vs_project_t vs_projects[MAX_VS_PROJECT];
static team_t teams[MAX_TEAM];
static record_t records[MAX_RECORD];
static coordination_t coordinations[MAX_COORDINATION];
static vs_state_t vs;

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

int vs_init(void) {
    if (initialized) return -1;
    vs.n_registration = 0; vs.n_project = 0; vs.n_team = 0;
    vs.n_record = 0; vs.n_coordination = 0;
    vs.total_volunteers = 0; vs.total_projects = 0;
    vs.total_teams = 0; vs.total_hours = 0;
    vs.total_certificates = 0;
    for (int i = 0; i < MAX_REGISTRATION; i++) registrations[i].active = 0;
    for (int i = 0; i < MAX_VS_PROJECT; i++) vs_projects[i].active = 0;
    for (int i = 0; i < MAX_TEAM; i++) teams[i].active = 0;
    for (int i = 0; i < MAX_RECORD; i++) records[i].active = 0;
    for (int i = 0; i < MAX_COORDINATION; i++) coordinations[i].active = 0;
    initialized = 1;
    print_str("[VS] Volunteer service initialized\n");
    return 0;
}

int vs_registration(int cat, int skill, int registered, int trained, int certs, int year) {
    if (vs.n_registration >= MAX_REGISTRATION) return -1;
    registration_t* r = &registrations[vs.n_registration];
    r->registration_id = vs.n_registration;
    r->volunteer_category = cat;
    r->skill_level = skill;
    r->volunteers_registered = registered;
    r->training_completed = trained;
    r->certifications_issued = certs;
    r->year = year;
    r->active = 1;
    vs.total_volunteers += registered;
    vs.total_certificates += certs;
    vs.n_registration++;
    print_str("[VS] Registration "); print_int(vs.n_registration - 1);
    print_str(" cat="); print_int(cat);
    print_str(" skl="); print_int(skill);
    print_str(" reg="); print_int(registered);
    print_str(" trn="); print_int(trained);
    print_str(" crt="); print_int(certs); print_str("\n");
    return vs.n_registration - 1;
}

int vs_project(int proj_type, int domain, int published, int recruited, int completed, int year) {
    if (vs.n_project >= MAX_VS_PROJECT) return -1;
    vs_project_t* p = &vs_projects[vs.n_project];
    p->project_id = vs.n_project;
    p->project_type = proj_type;
    p->service_domain = domain;
    p->projects_published = published;
    p->volunteers_recruited = recruited;
    p->project_completed = completed;
    p->year = year;
    p->active = 1;
    vs.total_projects += completed;
    vs.n_project++;
    print_str("[VS] Project "); print_int(vs.n_project - 1);
    print_str(" type="); print_int(proj_type);
    print_str(" dom="); print_int(domain);
    print_str(" pub="); print_int(published);
    print_str(" rec="); print_int(recruited);
    print_str(" cmp="); print_int(completed); print_str("\n");
    return vs.n_project - 1;
}

int vs_team(int team_type, int region, int formed, int activities, int awards, int year) {
    if (vs.n_team >= MAX_TEAM) return -1;
    team_t* t = &teams[vs.n_team];
    t->team_id = vs.n_team;
    t->team_type = team_type;
    t->region_id = region;
    t->teams_formed = formed;
    t->activities_organized = activities;
    t->awards_given = awards;
    t->year = year;
    t->active = 1;
    vs.total_teams += formed;
    vs.n_team++;
    print_str("[VS] Team "); print_int(vs.n_team - 1);
    print_str(" type="); print_int(team_type);
    print_str(" rgn="); print_int(region);
    print_str(" frm="); print_int(formed);
    print_str(" act="); print_int(activities);
    print_str(" awd="); print_int(awards); print_str("\n");
    return vs.n_team - 1;
}

int vs_record(int rec_type, int svc_cat, int hours, int evaluations, int certs, int year) {
    if (vs.n_record >= MAX_RECORD) return -1;
    record_t* r = &records[vs.n_record];
    r->record_id = vs.n_record;
    r->record_type = rec_type;
    r->service_category = svc_cat;
    r->hours_recorded = hours;
    r->evaluations_done = evaluations;
    r->certificates_issued = certs;
    r->year = year;
    r->active = 1;
    vs.total_hours += hours;
    vs.total_certificates += certs;
    vs.n_record++;
    print_str("[VS] Record "); print_int(vs.n_record - 1);
    print_str(" type="); print_int(rec_type);
    print_str(" cat="); print_int(svc_cat);
    print_str(" hrs="); print_int(hours);
    print_str(" evl="); print_int(evaluations);
    print_str(" crt="); print_int(certs); print_str("\n");
    return vs.n_record - 1;
}

int vs_coordination(int coord_type, int region_pair, int matched, int resources, int events, int year) {
    if (vs.n_coordination >= MAX_COORDINATION) return -1;
    coordination_t* c = &coordinations[vs.n_coordination];
    c->coordination_id = vs.n_coordination;
    c->coord_type = coord_type;
    c->region_pair = region_pair;
    c->needs_matched = matched;
    c->resources_allocated = resources;
    c->regional_events = events;
    c->year = year;
    c->active = 1;
    vs.n_coordination++;
    print_str("[VS] Coordination "); print_int(vs.n_coordination - 1);
    print_str(" type="); print_int(coord_type);
    print_str(" pr="); print_int(region_pair);
    print_str(" mtc="); print_int(matched);
    print_str(" rsc="); print_int(resources);
    print_str(" evt="); print_int(events); print_str("\n");
    return vs.n_coordination - 1;
}

void vs_registration_report(void) {
    print_str("[VS] Registration report:\n");
    print_str("  Volunteer categories: "); print_int(vs.n_registration); print_str("\n");
    print_str("  Total volunteers: "); print_int(vs.total_volunteers); print_str("\n");
    print_str("  Total certificates: "); print_int(vs.total_certificates); print_str("\n");
}

void vs_project_report(void) {
    print_str("[VS] Project report:\n");
    print_str("  Project types: "); print_int(vs.n_project); print_str("\n");
    print_str("  Total projects completed: "); print_int(vs.total_projects); print_str("\n");
}

void vs_coordination_report(void) {
    print_str("[VS] Coordination report:\n");
    print_str("  Teams formed: "); print_int(vs.n_team); print_str("\n");
    print_str("  Total teams: "); print_int(vs.total_teams); print_str("\n");
    print_str("  Service records: "); print_int(vs.n_record); print_str("\n");
    print_str("  Total hours: "); print_int(vs.total_hours); print_str("\n");
    print_str("  Coordination units: "); print_int(vs.n_coordination); print_str("\n");
}

void vs_print_state(void) {
    print_str("[VS] Rg="); print_int(vs.n_registration);
    print_str(" Pj="); print_int(vs.n_project);
    print_str(" Tm="); print_int(vs.n_team);
    print_str(" Rc="); print_int(vs.n_record);
    print_str(" Cd="); print_int(vs.n_coordination);
    print_str("\n");
}

int main(void) {
    print_str("=== Volunteer Service Demo ===\n\n");
    vs_init();

    print_str("Volunteer registration...\n");
    for (int i = 0; i < 16; i++) {
        int cat = (i % 5) + 1;
        int skl = (i % 4) + 1;
        int reg = 500 + (i * 100);
        int trn = 300 + (i * 60);
        int crt = 200 + (i * 40);
        int year = 2020 + (i % 5);
        vs_registration(cat, skl, reg, trn, crt, year);
    }

    print_str("\nProject management...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 5) + 1;
        int dom = (i % 4) + 1;
        int pub = 20 + (i * 5);
        int rec = 100 + (i * 25);
        int cmp = 15 + (i * 4);
        int year = 2021 + (i % 4);
        vs_project(type, dom, pub, rec, cmp, year);
    }

    print_str("\nTeam organization...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int rgn = (i % 8) + 1;
        int frm = 10 + (i * 3);
        int act = 30 + (i * 8);
        int awd = 5 + (i * 2);
        int year = 2022 + (i % 3);
        vs_team(type, rgn, frm, act, awd, year);
    }

    print_str("\nService records...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int hrs = 5000 + (i * 1000);
        int evl = 500 + (i * 100);
        int crt = 200 + (i * 50);
        int year = 2023 + (i % 2);
        vs_record(type, cat, hrs, evl, crt, year);
    }

    print_str("\nResource coordination...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int pr = 100 + (i * 11);
        int mtc = 50 + (i * 12);
        int rsc = 30 + (i * 8);
        int evt = 10 + (i * 3);
        int year = 2024;
        vs_coordination(type, pr, mtc, rsc, evt, year);
    }

    print_str("\nRegistration report...\n");
    vs_registration_report();

    print_str("\nProject report...\n");
    vs_project_report();

    print_str("\nCoordination report...\n");
    vs_coordination_report();

    print_str("\nFinal state...\n");
    vs_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
