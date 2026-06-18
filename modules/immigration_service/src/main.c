/* immigration_service: Immigration management (v1.0)
 * Visas, entry control, residence permits, citizenship, compliance
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

#define MAX_VISAS       20
#define MAX_ENTRIES     16
#define MAX_RESIDENCES  12
#define MAX_CITIZEN     8
#define MAX_DEPORTS     6
#define MAX_BORDERS     10

typedef struct {
    int    visa_id;
    int    applicant_id;
    int    type;
    int    country;
    int    duration_days;
    int    issued_year;
    int    issued_month;
    int    expires_year;
    int    status;
    int    active;
} visa_t;

typedef struct {
    int    entry_id;
    int    person_id;
    int    visa_id;
    int    port_id;
    int    type;
    int    year;
    int    month;
    int    day;
    int    result;
    int    active;
} entry_t;

typedef struct {
    int    resid_id;
    int    person_id;
    int    type;
    int    country;
    int    issued_year;
    int    expires_year;
    int    conditions;
    int    status;
    int    active;
} residence_t;

typedef struct {
    int    citizen_id;
    int    person_id;
    int    origin_country;
    int    year;
    int    approved;
    int    ceremony_done;
    int    active;
} citizen_t;

typedef struct {
    int    deport_id;
    int    person_id;
    int    reason;
    int    origin_country;
    int    year;
    int    executed;
    int    active;
} deport_t;

typedef struct {
    int    border_id;
    int    port_type;
    int    zone;
    int    n_entries;
    int    n_exits;
    int    n_flagged;
    int    staff;
    int    active;
} border_t;

typedef struct {
    int    n_visas;
    int    n_entries;
    int    n_residences;
    int    n_citizens;
    int    n_deports;
    int    n_borders;
    int    total_visas_issued;
    int    total_entries;
    int    total_exits;
    int    total_citizens_granted;
    int    total_deported;
    int    total_flagged;
    int    tourist_visa;
    int    work_visa;
    int    student_visa;
    int    pr_visa;
} is_state_t;

static visa_t visas[MAX_VISAS];
static entry_t entries[MAX_ENTRIES];
static residence_t residences[MAX_RESIDENCES];
static citizen_t citizens[MAX_CITIZEN];
static deport_t deportations[MAX_DEPORTS];
static border_t borders[MAX_BORDERS];
static is_state_t is;

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

int is_init(void) {
    if (initialized) return -1;
    is.n_visas = 0; is.n_entries = 0; is.n_residences = 0;
    is.n_citizens = 0; is.n_deports = 0; is.n_borders = 0;
    is.total_visas_issued = 0; is.total_entries = 0;
    is.total_exits = 0; is.total_citizens_granted = 0;
    is.total_deported = 0; is.total_flagged = 0;
    is.tourist_visa = 0; is.work_visa = 0;
    is.student_visa = 0; is.pr_visa = 0;
    for (int i = 0; i < MAX_VISAS; i++) visas[i].active = 0;
    for (int i = 0; i < MAX_ENTRIES; i++) entries[i].active = 0;
    for (int i = 0; i < MAX_RESIDENCES; i++) residences[i].active = 0;
    for (int i = 0; i < MAX_CITIZEN; i++) citizens[i].active = 0;
    for (int i = 0; i < MAX_DEPORTS; i++) deportations[i].active = 0;
    for (int i = 0; i < MAX_BORDERS; i++) borders[i].active = 0;
    initialized = 1;
    print_str("[IS] Immigration service initialized\n");
    return 0;
}

int is_add_border(int port_type, int zone, int staff) {
    if (is.n_borders >= MAX_BORDERS) return -1;
    border_t* b = &borders[is.n_borders];
    b->border_id = is.n_borders;
    b->port_type = port_type;
    b->zone = zone;
    b->n_entries = 0;
    b->n_exits = 0;
    b->n_flagged = 0;
    b->staff = staff;
    b->active = 1;
    is.n_borders++;
    print_str("[IS] Border "); print_int(is.n_borders - 1);
    print_str(" type="); print_int(port_type);
    print_str(" zone="); print_int(zone);
    print_str(" staff="); print_int(staff); print_str("\n");
    return is.n_borders - 1;
}

int is_issue_visa(int applicant, int type, int country, int duration, int year, int month) {
    if (is.n_visas >= MAX_VISAS) return -1;
    visa_t* v = &visas[is.n_visas];
    v->visa_id = is.n_visas;
    v->applicant_id = applicant;
    v->type = type;
    v->country = country;
    v->duration_days = duration;
    v->issued_year = year;
    v->issued_month = month;
    v->expires_year = year + (duration / 365) + 1;
    v->status = 1;
    v->active = 1;
    is.total_visas_issued++;
    if (type == 1) is.tourist_visa++;
    else if (type == 2) is.work_visa++;
    else if (type == 3) is.student_visa++;
    else is.pr_visa++;
    is.n_visas++;
    print_str("[IS] Visa "); print_int(is.n_visas - 1);
    print_str(" P"); print_int(applicant);
    print_str(" type="); print_int(type);
    print_str(" cty="); print_int(country);
    print_str(" days="); print_int(duration);
    print_str(" "); print_int(year); print_str("-"); print_int(month); print_str("\n");
    return is.n_visas - 1;
}

int is_process_entry(int person, int visa_id, int port, int type, int year, int month, int day) {
    if (is.n_entries >= MAX_ENTRIES || visa_id >= is.n_visas) return -1;
    entry_t* e = &entries[is.n_entries];
    e->entry_id = is.n_entries;
    e->person_id = person;
    e->visa_id = visa_id;
    e->port_id = port;
    e->type = type;
    e->year = year;
    e->month = month;
    e->day = day;
    e->result = 1;
    e->active = 1;
    borders[port].n_entries++;
    is.total_entries++;
    is.n_entries++;
    print_str("[IS] Entry "); print_int(is.n_entries - 1);
    print_str(" P"); print_int(person);
    print_str(" V"); print_int(visa_id);
    print_str(" port="); print_int(port);
    print_str(" type="); print_int(type);
    print_str(" "); print_int(year); print_str("-"); print_int(month); print_str("-"); print_int(day); print_str("\n");
    return is.n_entries - 1;
}

int is_process_exit(int person, int port) {
    if (port >= is.n_borders) return -1;
    borders[port].n_exits++;
    is.total_exits++;
    print_str("[IS] Exit P"); print_int(person);
    print_str(" port="); print_int(port); print_str("\n");
    return 0;
}

int is_flag_entry(int entry_id) {
    if (entry_id >= is.n_entries) return -1;
    int port = entries[entry_id].port_id;
    borders[port].n_flagged++;
    is.total_flagged++;
    print_str("[IS] Flag E"); print_int(entry_id);
    print_str(" port="); print_int(port); print_str("\n");
    return 0;
}

int is_grant_residence(int person, int type, int country, int issued, int expires, int conditions) {
    if (is.n_residences >= MAX_RESIDENCES) return -1;
    residence_t* r = &residences[is.n_residences];
    r->resid_id = is.n_residences;
    r->person_id = person;
    r->type = type;
    r->country = country;
    r->issued_year = issued;
    r->expires_year = expires;
    r->conditions = conditions;
    r->status = 1;
    r->active = 1;
    is.n_residences++;
    print_str("[IS] Residence "); print_int(is.n_residences - 1);
    print_str(" P"); print_int(person);
    print_str(" type="); print_int(type);
    print_str(" cty="); print_int(country);
    print_str(" "); print_int(issued); print_str("-"); print_int(expires); print_str("\n");
    return is.n_residences - 1;
}

int is_extend_residence(int resid_id, int new_expiry) {
    if (resid_id >= is.n_residences) return -1;
    residences[resid_id].expires_year = new_expiry;
    print_str("[IS] Extend R"); print_int(resid_id);
    print_str(" exp="); print_int(new_expiry); print_str("\n");
    return 0;
}

int is_apply_citizenship(int person, int origin, int year) {
    if (is.n_citizens >= MAX_CITIZEN) return -1;
    citizen_t* c = &citizens[is.n_citizens];
    c->citizen_id = is.n_citizens;
    c->person_id = person;
    c->origin_country = origin;
    c->year = year;
    c->approved = 0;
    c->ceremony_done = 0;
    c->active = 1;
    is.n_citizens++;
    print_str("[IS] Citizen app "); print_int(is.n_citizens - 1);
    print_str(" P"); print_int(person);
    print_str(" from="); print_int(origin);
    print_str(" "); print_int(year); print_str("\n");
    return is.n_citizens - 1;
}

int is_approve_citizenship(int citizen_id) {
    if (citizen_id >= is.n_citizens) return -1;
    citizens[citizen_id].approved = 1;
    citizens[citizen_id].ceremony_done = 1;
    is.total_citizens_granted++;
    print_str("[IS] Approve C"); print_int(citizen_id); print_str("\n");
    return 0;
}

int is_deport(int person, int reason, int origin, int year) {
    if (is.n_deports >= MAX_DEPORTS) return -1;
    deport_t* d = &deportations[is.n_deports];
    d->deport_id = is.n_deports;
    d->person_id = person;
    d->reason = reason;
    d->origin_country = origin;
    d->year = year;
    d->executed = 0;
    d->active = 1;
    is.total_deported++;
    is.n_deports++;
    print_str("[IS] Deport "); print_int(is.n_deports - 1);
    print_str(" P"); print_int(person);
    print_str(" reason="); print_int(reason);
    print_str(" to="); print_int(origin);
    print_str(" "); print_int(year); print_str("\n");
    return is.n_deports - 1;
}

int is_execute_deport(int deport_id) {
    if (deport_id >= is.n_deports) return -1;
    deportations[deport_id].executed = 1;
    print_str("[IS] Exec deport "); print_int(deport_id); print_str("\n");
    return 0;
}

void is_visa_report(void) {
    print_str("[IS] Visa report: "); print_int(is.n_visas); print_str("\n");
    print_str("  Tourist: "); print_int(is.tourist_visa);
    print_str(" Work: "); print_int(is.work_visa);
    print_str(" Student: "); print_int(is.student_visa);
    print_str(" PR: "); print_int(is.pr_visa); print_str("\n");
    for (int i = 0; i < is.n_visas; i++) {
        visa_t* v = &visas[i];
        print_str("  V"); print_int(i);
        print_str(" P"); print_int(v->applicant_id);
        print_str(" type="); print_int(v->type);
        print_str(" cty="); print_int(v->country);
        print_str(" days="); print_int(v->duration_days);
        print_str(" st="); print_int(v->status); print_str("\n");
    }
}

void is_border_report(void) {
    print_str("[IS] Border report:\n");
    for (int i = 0; i < is.n_borders; i++) {
        border_t* b = &borders[i];
        print_str("  B"); print_int(i);
        print_str(" type="); print_int(b->port_type);
        print_str(" zone="); print_int(b->zone);
        print_str(" in="); print_int(b->n_entries);
        print_str(" out="); print_int(b->n_exits);
        print_str(" flag="); print_int(b->n_flagged);
        print_str(" staff="); print_int(b->staff); print_str("\n");
    }
    print_str("  Total entries: "); print_int(is.total_entries);
    print_str(" exits: "); print_int(is.total_exits);
    print_str(" flagged: "); print_int(is.total_flagged); print_str("\n");
}

void is_compliance_report(void) {
    print_str("[IS] Compliance report:\n");
    print_str("  Residents: "); print_int(is.n_residences); print_str("\n");
    print_str("  Citizens granted: "); print_int(is.total_citizens_granted); print_str("\n");
    print_str("  Deportations: "); print_int(is.total_deported); print_str("\n");
}

void is_print_state(void) {
    print_str("[IS] Visas="); print_int(is.n_visas);
    print_str(" Entries="); print_int(is.n_entries);
    print_str(" Resid="); print_int(is.n_residences);
    print_str(" Citizens="); print_int(is.n_citizens);
    print_str(" Borders="); print_int(is.n_borders);
    print_str(" Deports="); print_int(is.n_deports);
    print_str("\n");
}

int main(void) {
    print_str("=== Immigration Service Demo ===\n\n");
    is_init();

    print_str("Adding border ports...\n");
    is_add_border(1, 1, 20);
    is_add_border(1, 2, 15);
    is_add_border(2, 1, 25);
    is_add_border(2, 3, 18);
    is_add_border(3, 2, 12);
    is_add_border(3, 4, 10);
    is_add_border(1, 3, 16);
    is_add_border(2, 4, 14);
    is_add_border(3, 1, 22);
    is_add_border(1, 4, 13);

    print_str("\nIssuing visas...\n");
    for (int i = 0; i < 20; i++) {
        int applicant = i + 100;
        int type = (i % 4) + 1;
        int country = (i % 8) + 1;
        int duration = 30 + (i * 30);
        int year = 2023 + (i % 3);
        int month = 1 + (i % 12);
        is_issue_visa(applicant, type, country, duration, year, month);
    }

    print_str("\nProcessing entries...\n");
    for (int i = 0; i < 16; i++) {
        int person = 100 + (i % 20);
        int visa = i % 20;
        int port = i % 10;
        int type = (i % 2) + 1;
        int year = 2024;
        int month = 1 + (i % 12);
        int day = 1 + (i % 28);
        is_process_entry(person, visa, port, type, year, month, day);
    }

    print_str("\nFlagging suspicious entries...\n");
    is_flag_entry(3);
    is_flag_entry(7);
    is_flag_entry(11);

    print_str("\nProcessing exits...\n");
    for (int i = 0; i < 8; i++) {
        int person = 100 + (i % 20);
        int port = i % 10;
        is_process_exit(person, port);
    }

    print_str("\nGranting residences...\n");
    for (int i = 0; i < 12; i++) {
        int person = 200 + i;
        int type = (i % 3) + 1;
        int country = (i % 6) + 1;
        int issued = 2022 + (i % 3);
        int expires = issued + 3;
        int conditions = (i % 4) + 1;
        is_grant_residence(person, type, country, issued, expires, conditions);
    }

    print_str("\nExtending residences...\n");
    for (int i = 0; i < 6; i++) {
        is_extend_residence(i, 2030 + i);
    }

    print_str("\nCitizenship applications...\n");
    for (int i = 0; i < 8; i++) {
        int person = 300 + i;
        int origin = (i % 5) + 1;
        int year = 2024;
        is_apply_citizenship(person, origin, year);
    }

    print_str("\nApproving citizenships...\n");
    for (int i = 0; i < 8; i++) {
        is_approve_citizenship(i);
    }

    print_str("\nDeportations...\n");
    is_deport(400, 1, 3, 2024);
    is_deport(401, 2, 5, 2024);
    is_deport(402, 1, 7, 2024);
    is_deport(403, 3, 2, 2024);
    is_deport(404, 2, 4, 2024);
    is_deport(405, 1, 6, 2024);

    print_str("\nExecuting deportations...\n");
    for (int i = 0; i < 6; i++) {
        is_execute_deport(i);
    }

    print_str("\nVisa report...\n");
    is_visa_report();

    print_str("\nBorder report...\n");
    is_border_report();

    print_str("\nCompliance report...\n");
    is_compliance_report();

    print_str("\nFinal state...\n");
    is_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
