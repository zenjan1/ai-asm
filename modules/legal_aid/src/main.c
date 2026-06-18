/* legal_aid: Legal aid service management (v1.0)
 * Aid applications, lawyers, case tracking, consultations, statistics
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

#define MAX_APPS         16
#define MAX_LAWYERS      14
#define MAX_CASES        18
#define MAX_CONSULTS     12
#define MAX_STATS        10

typedef struct {
    int    app_id;
    int    applicant_id;
    int    income_level;
    int    case_type;
    int    urgency;
    int    status;
    int    year_applied;
    int    assigned_to;
    int    active;
} application_t;

typedef struct {
    int    lawyer_id;
    int    firm_id;
    int    specialty;
    int    cases_assigned;
    int    cases_closed;
    int    hours_worked;
    int    rating;
    int    year_joined;
    int    active;
} lawyer_t;

typedef struct {
    int    case_id;
    int    app_id;
    int    lawyer_id;
    int    type;
    int    hours;
    int    status;
    int    outcome;
    int    year;
    int    active;
} case_t;

typedef struct {
    int    consult_id;
    int    client_id;
    int    lawyer_id;
    int    topic;
    int    duration_min;
    int    resolved;
    int    year;
    int    active;
} consult_t;

typedef struct {
    int    stat_id;
    int    office_id;
    int    apps_received;
    int    apps_approved;
    int    cases_closed;
    int    consults_given;
    int    beneficiaries;
    int    year;
    int    active;
} stat_t;

typedef struct {
    int    n_apps;
    int    n_lawyers;
    int    n_cases;
    int    n_consults;
    int    n_stats;
    int    total_approved;
    int    total_closed;
    int    total_hours;
    int    total_consults;
    int    total_beneficiaries;
} la_state_t;

static application_t applications[MAX_APPS];
static lawyer_t lawyers[MAX_LAWYERS];
static case_t cases[MAX_CASES];
static consult_t consults[MAX_CONSULTS];
static stat_t stats[MAX_STATS];
static la_state_t la;

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

int la_init(void) {
    if (initialized) return -1;
    la.n_apps = 0; la.n_lawyers = 0; la.n_cases = 0;
    la.n_consults = 0; la.n_stats = 0;
    la.total_approved = 0; la.total_closed = 0;
    la.total_hours = 0; la.total_consults = 0;
    la.total_beneficiaries = 0;
    for (int i = 0; i < MAX_APPS; i++) applications[i].active = 0;
    for (int i = 0; i < MAX_LAWYERS; i++) lawyers[i].active = 0;
    for (int i = 0; i < MAX_CASES; i++) cases[i].active = 0;
    for (int i = 0; i < MAX_CONSULTS; i++) consults[i].active = 0;
    for (int i = 0; i < MAX_STATS; i++) stats[i].active = 0;
    initialized = 1;
    print_str("[LA] Legal aid initialized\n");
    return 0;
}

int la_apply(int applicant, int income, int case_type, int urgency, int year) {
    if (la.n_apps >= MAX_APPS) return -1;
    application_t* a = &applications[la.n_apps];
    a->app_id = la.n_apps;
    a->applicant_id = applicant;
    a->income_level = income;
    a->case_type = case_type;
    a->urgency = urgency;
    a->status = 1;
    a->year_applied = year;
    a->assigned_to = -1;
    a->active = 1;
    la.n_apps++;
    print_str("[LA] App "); print_int(la.n_apps - 1);
    print_str(" app="); print_int(applicant);
    print_str(" inc="); print_int(income);
    print_str(" type="); print_int(case_type);
    print_str(" urg="); print_int(urgency);
    print_str(" yr="); print_int(year); print_str("\n");
    return la.n_apps - 1;
}

int la_approve(int app_id, int lawyer) {
    if (app_id >= la.n_apps) return -1;
    applications[app_id].status = 2;
    applications[app_id].assigned_to = lawyer;
    la.total_approved++;
    print_str("[LA] Approved App "); print_int(app_id);
    print_str(" -> Lawyer "); print_int(lawyer); print_str("\n");
    return 0;
}

int la_add_lawyer(int firm, int specialty, int assigned, int closed, int hours, int rating, int year) {
    if (la.n_lawyers >= MAX_LAWYERS) return -1;
    lawyer_t* l = &lawyers[la.n_lawyers];
    l->lawyer_id = la.n_lawyers;
    l->firm_id = firm;
    l->specialty = specialty;
    l->cases_assigned = assigned;
    l->cases_closed = closed;
    l->hours_worked = hours;
    l->rating = rating;
    l->year_joined = year;
    l->active = 1;
    la.n_lawyers++;
    print_str("[LA] Lawyer "); print_int(la.n_lawyers - 1);
    print_str(" firm="); print_int(firm);
    print_str(" spec="); print_int(specialty);
    print_str(" asgn="); print_int(assigned);
    print_str(" cls="); print_int(closed);
    print_str(" hrs="); print_int(hours);
    print_str(" rat="); print_int(rating); print_str("\n");
    return la.n_lawyers - 1;
}

int la_track_case(int app_id, int lawyer, int type, int hours, int status, int outcome, int year) {
    if (la.n_cases >= MAX_CASES) return -1;
    case_t* c = &cases[la.n_cases];
    c->case_id = la.n_cases;
    c->app_id = app_id;
    c->lawyer_id = lawyer;
    c->type = type;
    c->hours = hours;
    c->status = status;
    c->outcome = outcome;
    c->year = year;
    c->active = 1;
    la.total_hours += hours;
    if (status == 3) la.total_closed++;
    la.n_cases++;
    print_str("[LA] Case "); print_int(la.n_cases - 1);
    print_str(" app="); print_int(app_id);
    print_str(" law="); print_int(lawyer);
    print_str(" hrs="); print_int(hours);
    print_str(" st="); print_int(status);
    print_str(" out="); print_int(outcome); print_str("\n");
    return la.n_cases - 1;
}

int la_consult(int client, int lawyer, int topic, int duration, int resolved, int year) {
    if (la.n_consults >= MAX_CONSULTS) return -1;
    consult_t* c = &consults[la.n_consults];
    c->consult_id = la.n_consults;
    c->client_id = client;
    c->lawyer_id = lawyer;
    c->topic = topic;
    c->duration_min = duration;
    c->resolved = resolved;
    c->year = year;
    c->active = 1;
    la.total_consults++;
    la.n_consults++;
    print_str("[LA] Consult "); print_int(la.n_consults - 1);
    print_str(" cli="); print_int(client);
    print_str(" law="); print_int(lawyer);
    print_str(" top="); print_int(topic);
    print_str(" min="); print_int(duration);
    print_str(" res="); print_int(resolved); print_str("\n");
    return la.n_consults - 1;
}

int la_record_stat(int office, int received, int approved, int closed, int consults, int beneficiaries, int year) {
    if (la.n_stats >= MAX_STATS) return -1;
    stat_t* s = &stats[la.n_stats];
    s->stat_id = la.n_stats;
    s->office_id = office;
    s->apps_received = received;
    s->apps_approved = approved;
    s->cases_closed = closed;
    s->consults_given = consults;
    s->beneficiaries = beneficiaries;
    s->year = year;
    s->active = 1;
    la.total_beneficiaries += beneficiaries;
    la.n_stats++;
    print_str("[LA] Stat "); print_int(la.n_stats - 1);
    print_str(" off="); print_int(office);
    print_str(" rcv="); print_int(received);
    print_str(" apr="); print_int(approved);
    print_str(" ben="); print_int(beneficiaries);
    print_str(" yr="); print_int(year); print_str("\n");
    return la.n_stats - 1;
}

void la_application_report(void) {
    print_str("[LA] Application report:\n");
    print_str("  Received: "); print_int(la.n_apps); print_str("\n");
    print_str("  Approved: "); print_int(la.total_approved); print_str("\n");
    print_str("  Lawyers: "); print_int(la.n_lawyers); print_str("\n");
}

void la_case_report(void) {
    print_str("[LA] Case report:\n");
    print_str("  Cases: "); print_int(la.n_cases); print_str("\n");
    print_str("  Closed: "); print_int(la.total_closed); print_str("\n");
    print_str("  Total hours: "); print_int(la.total_hours); print_str("\n");
}

void la_service_report(void) {
    print_str("[LA] Service report:\n");
    print_str("  Consultations: "); print_int(la.total_consults); print_str("\n");
    print_str("  Beneficiaries: "); print_int(la.total_beneficiaries); print_str("\n");
}

void la_print_state(void) {
    print_str("[LA] Ap="); print_int(la.n_apps);
    print_str(" Lw="); print_int(la.n_lawyers);
    print_str(" Cs="); print_int(la.n_cases);
    print_str(" Co="); print_int(la.n_consults);
    print_str(" St="); print_int(la.n_stats);
    print_str("\n");
}

int main(void) {
    print_str("=== Legal Aid Demo ===\n\n");
    la_init();

    print_str("Receiving applications...\n");
    for (int i = 0; i < 16; i++) {
        int applicant = 4000 + (i * 13);
        int income = (i % 5) + 1;
        int type = (i % 4) + 1;
        int urgency = (i % 3) + 1;
        int year = 2019 + (i % 6);
        la_apply(applicant, income, type, urgency, year);
    }

    print_str("\nAdding lawyers...\n");
    for (int i = 0; i < 14; i++) {
        int firm = 100 + (i * 5);
        int spec = (i % 6) + 1;
        int asgn = 5 + (i * 2);
        int cls = 3 + (i * 2);
        int hrs = 100 + (i * 30);
        int rating = 70 + (i * 2);
        int year = 2018 + (i % 6);
        la_add_lawyer(firm, spec, asgn, cls, hrs, rating, year);
    }

    print_str("\nApproving applications...\n");
    for (int i = 0; i < 12; i++) {
        la_approve(i, i % 14);
    }

    print_str("\nTracking cases...\n");
    for (int i = 0; i < 18; i++) {
        int app = i % 16;
        int law = i % 14;
        int type = (i % 4) + 1;
        int hrs = 5 + (i * 3);
        int status = (i % 4) + 1;
        int outcome = (i % 3) + 1;
        int year = 2021 + (i % 4);
        la_track_case(app, law, type, hrs, status, outcome, year);
    }

    print_str("\nProviding consultations...\n");
    for (int i = 0; i < 12; i++) {
        int client = 5000 + (i * 7);
        int lawyer = i % 14;
        int topic = (i % 5) + 1;
        int dur = 30 + (i * 10);
        int resolved = (i % 2);
        int year = 2023 + (i % 2);
        la_consult(client, lawyer, topic, dur, resolved, year);
    }

    print_str("\nRecording statistics...\n");
    for (int i = 0; i < 10; i++) {
        int office = (i % 5) + 1;
        int rcv = 80 + (i * 30);
        int apr = 60 + (i * 25);
        int cls = 40 + (i * 15);
        int cons = 30 + (i * 10);
        int ben = 100 + (i * 40);
        int year = 2020 + (i % 4);
        la_record_stat(office, rcv, apr, cls, cons, ben, year);
    }

    print_str("\nApplication report...\n");
    la_application_report();

    print_str("\nCase report...\n");
    la_case_report();

    print_str("\nService report...\n");
    la_service_report();

    print_str("\nFinal state...\n");
    la_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
