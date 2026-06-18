/* youth_affairs: Youth affairs management (v1.0)
 * Youth development, volunteering, rights protection, organizations, education
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

#define MAX_DEVELOP      16
#define MAX_VOLUNTEER    14
#define MAX_RIGHTS       12
#define MAX_ORG          10
#define MAX_IDEATION     10

typedef struct {
    int    develop_id;
    int    youth_id;
    int    program_type;
    int    training_hours;
    int    skills_acquired;
    int    employment_status;
    int    income;
    int    year;
    int    active;
} develop_t;

typedef struct {
    int    volunteer_id;
    int    person_id;
    int    project_id;
    int    service_hours;
    int    category;
    int    points;
    int    recognition;
    int    year;
    int    active;
} volunteer_t;

typedef struct {
    int    rights_id;
    int    youth_id;
    int    case_type;
    int    counselor_id;
    int    sessions;
    int    resolved;
    int    year;
    int    status;
    int    active;
} rights_t;

typedef struct {
    int    org_id;
    int    type;
    int    region_id;
    int    members;
    int    activities;
    int    funding;
    int    year;
    int    active;
} org_t;

typedef struct {
    int    idea_id;
    int    youth_id;
    int    type;
    int    participants;
    int    topics_covered;
    int    satisfaction;
    int    year;
    int    active;
} ideation_t;

typedef struct {
    int    n_develop;
    int    n_volunteer;
    int    n_rights;
    int    n_org;
    int    n_idea;
    int    total_training;
    int    total_hours;
    int    total_points;
    int    total_resolved;
    int    total_members;
} ya_state_t;

static develop_t develops[MAX_DEVELOP];
static volunteer_t volunteers[MAX_VOLUNTEER];
static rights_t rights_cases[MAX_RIGHTS];
static org_t organizations[MAX_ORG];
static ideation_t ideations[MAX_IDEATION];
static ya_state_t ya;

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

int ya_init(void) {
    if (initialized) return -1;
    ya.n_develop = 0; ya.n_volunteer = 0; ya.n_rights = 0;
    ya.n_org = 0; ya.n_idea = 0;
    ya.total_training = 0; ya.total_hours = 0;
    ya.total_points = 0; ya.total_resolved = 0;
    ya.total_members = 0;
    for (int i = 0; i < MAX_DEVELOP; i++) develops[i].active = 0;
    for (int i = 0; i < MAX_VOLUNTEER; i++) volunteers[i].active = 0;
    for (int i = 0; i < MAX_RIGHTS; i++) rights_cases[i].active = 0;
    for (int i = 0; i < MAX_ORG; i++) organizations[i].active = 0;
    for (int i = 0; i < MAX_IDEATION; i++) ideations[i].active = 0;
    initialized = 1;
    print_str("[YA] Youth affairs initialized\n");
    return 0;
}

int ya_develop(int youth, int program, int training, int skills, int employment, int income, int year) {
    if (ya.n_develop >= MAX_DEVELOP) return -1;
    develop_t* d = &develops[ya.n_develop];
    d->develop_id = ya.n_develop;
    d->youth_id = youth;
    d->program_type = program;
    d->training_hours = training;
    d->skills_acquired = skills;
    d->employment_status = employment;
    d->income = income;
    d->year = year;
    d->active = 1;
    ya.total_training += training;
    ya.n_develop++;
    print_str("[YA] Develop "); print_int(ya.n_develop - 1);
    print_str(" yth="); print_int(youth);
    print_str(" prg="); print_int(program);
    print_str(" trn="); print_int(training); print_str("h");
    print_str(" skl="); print_int(skills);
    print_str(" emp="); print_int(employment);
    print_str(" inc=$"); print_int(income); print_str("\n");
    return ya.n_develop - 1;
}

int ya_volunteer(int person, int project, int hours, int category, int points, int recognition, int year) {
    if (ya.n_volunteer >= MAX_VOLUNTEER) return -1;
    volunteer_t* v = &volunteers[ya.n_volunteer];
    v->volunteer_id = ya.n_volunteer;
    v->person_id = person;
    v->project_id = project;
    v->service_hours = hours;
    v->category = category;
    v->points = points;
    v->recognition = recognition;
    v->year = year;
    v->active = 1;
    ya.total_hours += hours;
    ya.total_points += points;
    ya.n_volunteer++;
    print_str("[YA] Volunteer "); print_int(ya.n_volunteer - 1);
    print_str(" per="); print_int(person);
    print_str(" prj="); print_int(project);
    print_str(" hrs="); print_int(hours);
    print_str(" cat="); print_int(category);
    print_str(" pts="); print_int(points); print_str("\n");
    return ya.n_volunteer - 1;
}

int ya_rights(int youth, int case_type, int counselor, int sessions, int resolved, int year) {
    if (ya.n_rights >= MAX_RIGHTS) return -1;
    rights_t* r = &rights_cases[ya.n_rights];
    r->rights_id = ya.n_rights;
    r->youth_id = youth;
    r->case_type = case_type;
    r->counselor_id = counselor;
    r->sessions = sessions;
    r->resolved = resolved;
    r->year = year;
    r->status = 1;
    r->active = 1;
    if (resolved) ya.total_resolved++;
    ya.n_rights++;
    print_str("[YA] Rights "); print_int(ya.n_rights - 1);
    print_str(" yth="); print_int(youth);
    print_str(" type="); print_int(case_type);
    print_str(" cns="); print_int(counselor);
    print_str(" ses="); print_int(sessions);
    print_str(" res="); print_int(resolved); print_str("\n");
    return ya.n_rights - 1;
}

int ya_org(int type, int region, int members, int activities, int funding, int year) {
    if (ya.n_org >= MAX_ORG) return -1;
    org_t* o = &organizations[ya.n_org];
    o->org_id = ya.n_org;
    o->type = type;
    o->region_id = region;
    o->members = members;
    o->activities = activities;
    o->funding = funding;
    o->year = year;
    o->active = 1;
    ya.total_members += members;
    ya.n_org++;
    print_str("[YA] Org "); print_int(ya.n_org - 1);
    print_str(" type="); print_int(type);
    print_str(" reg="); print_int(region);
    print_str(" mem="); print_int(members);
    print_str(" act="); print_int(activities);
    print_str(" fnd=$"); print_int(funding); print_str("\n");
    return ya.n_org - 1;
}

int ya_ideation(int youth, int type, int participants, int topics, int satisfaction, int year) {
    if (ya.n_idea >= MAX_IDEATION) return -1;
    ideation_t* id = &ideations[ya.n_idea];
    id->idea_id = ya.n_idea;
    id->youth_id = youth;
    id->type = type;
    id->participants = participants;
    id->topics_covered = topics;
    id->satisfaction = satisfaction;
    id->year = year;
    id->active = 1;
    ya.n_idea++;
    print_str("[YA] Ideation "); print_int(ya.n_idea - 1);
    print_str(" yth="); print_int(youth);
    print_str(" type="); print_int(type);
    print_str(" ptc="); print_int(participants);
    print_str(" top="); print_int(topics);
    print_str(" sat="); print_int(satisfaction); print_str("\n");
    return ya.n_idea - 1;
}

void ya_develop_report(void) {
    print_str("[YA] Development report:\n");
    print_str("  Youth in programs: "); print_int(ya.n_develop); print_str("\n");
    print_str("  Total training hours: "); print_int(ya.total_training); print_str("\n");
}

void ya_volunteer_report(void) {
    print_str("[YA] Volunteer report:\n");
    print_str("  Volunteers: "); print_int(ya.n_volunteer); print_str("\n");
    print_str("  Total service hours: "); print_int(ya.total_hours); print_str("\n");
    print_str("  Total points: "); print_int(ya.total_points); print_str("\n");
}

void ya_org_report(void) {
    print_str("[YA] Organization report:\n");
    print_str("  Organizations: "); print_int(ya.n_org); print_str("\n");
    print_str("  Total members: "); print_int(ya.total_members); print_str("\n");
    print_str("  Rights cases: "); print_int(ya.n_rights); print_str("\n");
    print_str("  Resolved: "); print_int(ya.total_resolved); print_str("\n");
    print_str("  Education sessions: "); print_int(ya.n_idea); print_str("\n");
}

void ya_print_state(void) {
    print_str("[YA] Dv="); print_int(ya.n_develop);
    print_str(" Vl="); print_int(ya.n_volunteer);
    print_str(" Rt="); print_int(ya.n_rights);
    print_str(" Og="); print_int(ya.n_org);
    print_str(" Id="); print_int(ya.n_idea);
    print_str("\n");
}

int main(void) {
    print_str("=== Youth Affairs Demo ===\n\n");
    ya_init();

    print_str("Youth development...\n");
    for (int i = 0; i < 16; i++) {
        int yth = 1000 + (i * 13);
        int prg = (i % 5) + 1;
        int trn = 20 + (i * 10);
        int skl = 1 + (i % 6);
        int emp = (i % 3) + 1;
        int inc = 2000 + (i * 300);
        int year = 2021 + (i % 4);
        ya_develop(yth, prg, trn, skl, emp, inc, year);
    }

    print_str("\nVolunteer services...\n");
    for (int i = 0; i < 14; i++) {
        int per = 2000 + (i * 11);
        int prj = 100 + (i * 5);
        int hrs = 10 + (i * 8);
        int cat = (i % 4) + 1;
        int pts = hrs * 2;
        int rec = (i % 4 == 0) ? 1 : 0;
        int year = 2022 + (i % 3);
        ya_volunteer(per, prj, hrs, cat, pts, rec, year);
    }

    print_str("\nRights protection...\n");
    for (int i = 0; i < 12; i++) {
        int yth = 3000 + (i * 17);
        int ctype = (i % 4) + 1;
        int cns = 400 + (i % 8);
        int ses = 1 + (i % 6);
        int res = (i % 3 == 0) ? 0 : 1;
        int year = 2023 + (i % 2);
        ya_rights(yth, ctype, cns, ses, res, year);
    }

    print_str("\nYouth organizations...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 3) + 1;
        int reg = (i % 6) + 1;
        int mem = 20 + (i * 15);
        int act = 5 + (i * 3);
        int fnd = 10000 + (i * 5000);
        int year = 2024;
        ya_org(type, reg, mem, act, fnd, year);
    }

    print_str("\nIdeological education...\n");
    for (int i = 0; i < 10; i++) {
        int yth = 5000 + (i * 7);
        int type = (i % 4) + 1;
        int ptc = 30 + (i * 20);
        int top = 3 + (i % 5);
        int sat = 70 + (i * 3);
        int year = 2024;
        ya_ideation(yth, type, ptc, top, sat, year);
    }

    print_str("\nDevelopment report...\n");
    ya_develop_report();

    print_str("\nVolunteer report...\n");
    ya_volunteer_report();

    print_str("\nOrganization report...\n");
    ya_org_report();

    print_str("\nFinal state...\n");
    ya_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
