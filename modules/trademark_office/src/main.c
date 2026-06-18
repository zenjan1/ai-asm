/* trademark_office: Trademark registration management (v1.0)
 * Applications, examination, maintenance, opposition, search
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
#define MAX_EXAMS        14
#define MAX_MAINT        12
#define MAX_OPP          10
#define MAX_SEARCHES     10

typedef struct {
    int    app_id;
    int    type;
    int    class_id;
    int    owner_id;
    int    elements;
    int    colors;
    int    year_filed;
    int    status;
    int    active;
} application_t;

typedef struct {
    int    exam_id;
    int    app_id;
    int    examiner_id;
    int    exam_type;
    int    result;
    int    conflicts;
    int    months_taken;
    int    year;
    int    active;
} examination_t;

typedef struct {
    int    maint_id;
    int    tm_id;
    int    action_type;
    int    fee;
    int    new_owner;
    int    license_to;
    int    year;
    int    status;
    int    active;
} maint_action_t;

typedef struct {
    int    opp_id;
    int    tm_id;
    int    opposer_id;
    int    grounds;
    int    evidence_items;
    int    result;
    int    year;
    int    status;
    int    active;
} opposition_t;

typedef struct {
    int    search_id;
    int    type;
    int    class_id;
    int    query_terms;
    int    results;
    int    similar_found;
    int    year;
    int    active;
} search_t;

typedef struct {
    int    n_apps;
    int    n_exams;
    int    n_maint;
    int    n_opp;
    int    n_searches;
    int    total_registered;
    int    total_rejected;
    int    total_oppositions;
    int    total_fees;
    int    total_searches;
} to_state_t;

static application_t applications[MAX_APPS];
static examination_t examinations[MAX_EXAMS];
static maint_action_t maint_actions[MAX_MAINT];
static opposition_t oppositions[MAX_OPP];
static search_t searches[MAX_SEARCHES];
static to_state_t to;

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

int to_init(void) {
    if (initialized) return -1;
    to.n_apps = 0; to.n_exams = 0; to.n_maint = 0;
    to.n_opp = 0; to.n_searches = 0;
    to.total_registered = 0; to.total_rejected = 0;
    to.total_oppositions = 0; to.total_fees = 0;
    to.total_searches = 0;
    for (int i = 0; i < MAX_APPS; i++) applications[i].active = 0;
    for (int i = 0; i < MAX_EXAMS; i++) examinations[i].active = 0;
    for (int i = 0; i < MAX_MAINT; i++) maint_actions[i].active = 0;
    for (int i = 0; i < MAX_OPP; i++) oppositions[i].active = 0;
    for (int i = 0; i < MAX_SEARCHES; i++) searches[i].active = 0;
    initialized = 1;
    print_str("[TO] Trademark office initialized\n");
    return 0;
}

int to_file_application(int type, int class_id, int owner, int elements, int colors, int year) {
    if (to.n_apps >= MAX_APPS) return -1;
    application_t* a = &applications[to.n_apps];
    a->app_id = to.n_apps;
    a->type = type;
    a->class_id = class_id;
    a->owner_id = owner;
    a->elements = elements;
    a->colors = colors;
    a->year_filed = year;
    a->status = 1;
    a->active = 1;
    to.n_apps++;
    print_str("[TO] Application "); print_int(to.n_apps - 1);
    print_str(" type="); print_int(type);
    print_str(" cls="); print_int(class_id);
    print_str(" own="); print_int(owner);
    print_str(" elem="); print_int(elements);
    print_str(" clr="); print_int(colors);
    print_str(" yr="); print_int(year); print_str("\n");
    return to.n_apps - 1;
}

int to_examine(int app_id, int examiner, int exam_type, int result, int conflicts, int months, int year) {
    if (to.n_exams >= MAX_EXAMS) return -1;
    examination_t* e = &examinations[to.n_exams];
    e->exam_id = to.n_exams;
    e->app_id = app_id;
    e->examiner_id = examiner;
    e->exam_type = exam_type;
    e->result = result;
    e->conflicts = conflicts;
    e->months_taken = months;
    e->year = year;
    e->active = 1;
    if (result == 1) to.total_registered++;
    else if (result == 2) to.total_rejected++;
    to.n_exams++;
    print_str("[TO] Exam "); print_int(to.n_exams - 1);
    print_str(" app="); print_int(app_id);
    print_str(" ex="); print_int(examiner);
    print_str(" res="); print_int(result);
    print_str(" conf="); print_int(conflicts);
    print_str(" mo="); print_int(months); print_str("\n");
    return to.n_exams - 1;
}

int to_maintain(int tm_id, int action_type, int fee, int new_owner, int license_to, int year) {
    if (to.n_maint >= MAX_MAINT) return -1;
    maint_action_t* m = &maint_actions[to.n_maint];
    m->maint_id = to.n_maint;
    m->tm_id = tm_id;
    m->action_type = action_type;
    m->fee = fee;
    m->new_owner = new_owner;
    m->license_to = license_to;
    m->year = year;
    m->status = 1;
    m->active = 1;
    to.total_fees += fee;
    to.n_maint++;
    print_str("[TO] Maint "); print_int(to.n_maint - 1);
    print_str(" tm="); print_int(tm_id);
    print_str(" act="); print_int(action_type);
    print_str(" fee=$"); print_int(fee);
    print_str(" yr="); print_int(year); print_str("\n");
    return to.n_maint - 1;
}

int to_oppose(int tm_id, int opposer, int grounds, int evidence, int result, int year) {
    if (to.n_opp >= MAX_OPP) return -1;
    opposition_t* o = &oppositions[to.n_opp];
    o->opp_id = to.n_opp;
    o->tm_id = tm_id;
    o->opposer_id = opposer;
    o->grounds = grounds;
    o->evidence_items = evidence;
    o->result = result;
    o->year = year;
    o->status = 1;
    o->active = 1;
    to.total_oppositions++;
    to.n_opp++;
    print_str("[TO] Opposition "); print_int(to.n_opp - 1);
    print_str(" tm="); print_int(tm_id);
    print_str(" by="); print_int(opposer);
    print_str(" grd="); print_int(grounds);
    print_str(" ev="); print_int(evidence);
    print_str(" res="); print_int(result); print_str("\n");
    return to.n_opp - 1;
}

int to_search(int type, int class_id, int terms, int results, int similar, int year) {
    if (to.n_searches >= MAX_SEARCHES) return -1;
    search_t* s = &searches[to.n_searches];
    s->search_id = to.n_searches;
    s->type = type;
    s->class_id = class_id;
    s->query_terms = terms;
    s->results = results;
    s->similar_found = similar;
    s->year = year;
    s->active = 1;
    to.total_searches++;
    to.n_searches++;
    print_str("[TO] Search "); print_int(to.n_searches - 1);
    print_str(" type="); print_int(type);
    print_str(" cls="); print_int(class_id);
    print_str(" terms="); print_int(terms);
    print_str(" res="); print_int(results);
    print_str(" sim="); print_int(similar); print_str("\n");
    return to.n_searches - 1;
}

void to_registration_report(void) {
    print_str("[TO] Registration report:\n");
    print_str("  Applications: "); print_int(to.n_apps); print_str("\n");
    print_str("  Registered: "); print_int(to.total_registered); print_str("\n");
    print_str("  Rejected: "); print_int(to.total_rejected); print_str("\n");
    print_str("  Examinations: "); print_int(to.n_exams); print_str("\n");
}

void to_maintenance_report(void) {
    print_str("[TO] Maintenance report:\n");
    print_str("  Actions: "); print_int(to.n_maint); print_str("\n");
    print_str("  Total fees: $"); print_int(to.total_fees); print_str("\n");
    print_str("  Oppositions: "); print_int(to.total_oppositions); print_str("\n");
}

void to_search_report(void) {
    print_str("[TO] Search report:\n");
    print_str("  Searches: "); print_int(to.total_searches); print_str("\n");
    print_str("  Avg results: ");
    int total_res = 0;
    for (int i = 0; i < to.n_searches; i++) total_res += searches[i].results;
    int avg = to.n_searches > 0 ? total_res / to.n_searches : 0;
    print_int(avg); print_str("\n");
}

void to_print_state(void) {
    print_str("[TO] Ap="); print_int(to.n_apps);
    print_str(" Ex="); print_int(to.n_exams);
    print_str(" Mt="); print_int(to.n_maint);
    print_str(" Op="); print_int(to.n_opp);
    print_str(" Sr="); print_int(to.n_searches);
    print_str("\n");
}

int main(void) {
    print_str("=== Trademark Office Demo ===\n\n");
    to_init();

    print_str("Filing applications...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 3) + 1;
        int cls = (i % 45) + 1;
        int owner = 3000 + (i * 11);
        int elem = 1 + (i % 5);
        int colors = 1 + (i % 4);
        int year = 2018 + (i % 7);
        to_file_application(type, cls, owner, elem, colors, year);
    }

    print_str("\nExamining applications...\n");
    for (int i = 0; i < 14; i++) {
        int app = i % 16;
        int examiner = 400 + (i % 10);
        int type = (i % 2) + 1;
        int result = (i % 3);
        int conf = i % 5;
        int months = 3 + (i % 12);
        int year = 2020 + (i % 5);
        to_examine(app, examiner, type, result, conf, months, year);
    }

    print_str("\nMaintenance actions...\n");
    for (int i = 0; i < 12; i++) {
        int tm = i % 14;
        int action = (i % 4) + 1;
        int fee = 300 + (i * 100);
        int new_owner = (i % 3 == 0) ? 4000 + i : 0;
        int license = (i % 4 == 0) ? 5000 + i : 0;
        int year = 2022 + (i % 3);
        to_maintain(tm, action, fee, new_owner, license, year);
    }

    print_str("\nOppositions...\n");
    for (int i = 0; i < 10; i++) {
        int tm = i % 14;
        int opposer = 6000 + (i * 7);
        int grounds = (i % 4) + 1;
        int evidence = 2 + (i % 8);
        int result = (i % 3);
        int year = 2021 + (i % 4);
        to_oppose(tm, opposer, grounds, evidence, result, year);
    }

    print_str("\nConducting searches...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 3) + 1;
        int cls = (i % 45) + 1;
        int terms = 1 + (i % 5);
        int res = 3 + (i * 2);
        int sim = 1 + (i % 4);
        int year = 2023 + (i % 2);
        to_search(type, cls, terms, res, sim, year);
    }

    print_str("\nRegistration report...\n");
    to_registration_report();

    print_str("\nMaintenance report...\n");
    to_maintenance_report();

    print_str("\nSearch report...\n");
    to_search_report();

    print_str("\nFinal state...\n");
    to_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
