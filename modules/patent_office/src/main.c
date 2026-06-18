/* patent_office: Patent office management (v1.0)
 * Patent applications, examinations, maintenance, search, statistics
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

#define MAX_APPS         18
#define MAX_EXAMS        14
#define MAX_MAINT        12
#define MAX_SEARCHES     10
#define MAX_STATS        10

typedef struct {
    int    app_id;
    int    type;
    int    field;
    int    applicant_id;
    int    claims;
    int    pages;
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
    int    citations;
    int    months_taken;
    int    year;
    int    active;
} examination_t;

typedef struct {
    int    maint_id;
    int    patent_id;
    int    fee_type;
    int    amount;
    int    year_paid;
    int    due_year;
    int    status;
    int    active;
} maintenance_t;

typedef struct {
    int    search_id;
    int    type;
    int    field;
    int    results;
    int    citations_found;
    int    prior_art;
    int    year;
    int    active;
} search_t;

typedef struct {
    int    stat_id;
    int    field;
    int    filed;
    int    granted;
    int    rejected;
    int    pending;
    int    expired;
    int    year;
    int    active;
} statistic_t;

typedef struct {
    int    n_apps;
    int    n_exams;
    int    n_maint;
    int    n_searches;
    int    n_stats;
    int    total_filed;
    int    total_granted;
    int    total_rejected;
    int    total_fees;
    int    total_searches;
} po_state_t;

static application_t applications[MAX_APPS];
static examination_t examinations[MAX_EXAMS];
static maintenance_t maintenance[MAX_MAINT];
static search_t searches[MAX_SEARCHES];
static statistic_t statistics[MAX_STATS];
static po_state_t po;

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

int po_init(void) {
    if (initialized) return -1;
    po.n_apps = 0; po.n_exams = 0; po.n_maint = 0;
    po.n_searches = 0; po.n_stats = 0;
    po.total_filed = 0; po.total_granted = 0;
    po.total_rejected = 0; po.total_fees = 0;
    po.total_searches = 0;
    for (int i = 0; i < MAX_APPS; i++) applications[i].active = 0;
    for (int i = 0; i < MAX_EXAMS; i++) examinations[i].active = 0;
    for (int i = 0; i < MAX_MAINT; i++) maintenance[i].active = 0;
    for (int i = 0; i < MAX_SEARCHES; i++) searches[i].active = 0;
    for (int i = 0; i < MAX_STATS; i++) statistics[i].active = 0;
    initialized = 1;
    print_str("[PO] Patent office initialized\n");
    return 0;
}

int po_file_application(int type, int field, int applicant, int claims, int pages, int year) {
    if (po.n_apps >= MAX_APPS) return -1;
    application_t* a = &applications[po.n_apps];
    a->app_id = po.n_apps;
    a->type = type;
    a->field = field;
    a->applicant_id = applicant;
    a->claims = claims;
    a->pages = pages;
    a->year_filed = year;
    a->status = 1;
    a->active = 1;
    po.total_filed++;
    po.n_apps++;
    print_str("[PO] Application "); print_int(po.n_apps - 1);
    print_str(" type="); print_int(type);
    print_str(" field="); print_int(field);
    print_str(" app="); print_int(applicant);
    print_str(" cl="); print_int(claims);
    print_str(" pg="); print_int(pages);
    print_str(" yr="); print_int(year); print_str("\n");
    return po.n_apps - 1;
}

int po_examine(int app_id, int examiner, int exam_type, int result, int citations, int months, int year) {
    if (po.n_exams >= MAX_EXAMS) return -1;
    examination_t* e = &examinations[po.n_exams];
    e->exam_id = po.n_exams;
    e->app_id = app_id;
    e->examiner_id = examiner;
    e->exam_type = exam_type;
    e->result = result;
    e->citations = citations;
    e->months_taken = months;
    e->year = year;
    e->active = 1;
    if (result == 1) po.total_granted++;
    else if (result == 2) po.total_rejected++;
    po.n_exams++;
    print_str("[PO] Exam "); print_int(po.n_exams - 1);
    print_str(" app="); print_int(app_id);
    print_str(" ex="); print_int(examiner);
    print_str(" type="); print_int(exam_type);
    print_str(" res="); print_int(result);
    print_str(" cit="); print_int(citations);
    print_str(" mo="); print_int(months); print_str("\n");
    return po.n_exams - 1;
}

int po_pay_fee(int patent_id, int fee_type, int amount, int year_paid, int due_year) {
    if (po.n_maint >= MAX_MAINT) return -1;
    maintenance_t* m = &maintenance[po.n_maint];
    m->maint_id = po.n_maint;
    m->patent_id = patent_id;
    m->fee_type = fee_type;
    m->amount = amount;
    m->year_paid = year_paid;
    m->due_year = due_year;
    m->status = 1;
    m->active = 1;
    po.total_fees += amount;
    po.n_maint++;
    print_str("[PO] Fee "); print_int(po.n_maint - 1);
    print_str(" pat="); print_int(patent_id);
    print_str(" type="); print_int(fee_type);
    print_str(" amt=$"); print_int(amount);
    print_str(" paid="); print_int(year_paid); print_str("\n");
    return po.n_maint - 1;
}

int po_search(int type, int field, int results, int citations, int prior_art, int year) {
    if (po.n_searches >= MAX_SEARCHES) return -1;
    search_t* s = &searches[po.n_searches];
    s->search_id = po.n_searches;
    s->type = type;
    s->field = field;
    s->results = results;
    s->citations_found = citations;
    s->prior_art = prior_art;
    s->year = year;
    s->active = 1;
    po.total_searches++;
    po.n_searches++;
    print_str("[PO] Search "); print_int(po.n_searches - 1);
    print_str(" type="); print_int(type);
    print_str(" field="); print_int(field);
    print_str(" res="); print_int(results);
    print_str(" cit="); print_int(citations);
    print_str(" pa="); print_int(prior_art); print_str("\n");
    return po.n_searches - 1;
}

int po_record_stat(int field, int filed, int granted, int rejected, int pending, int expired, int year) {
    if (po.n_stats >= MAX_STATS) return -1;
    statistic_t* st = &statistics[po.n_stats];
    st->stat_id = po.n_stats;
    st->field = field;
    st->filed = filed;
    st->granted = granted;
    st->rejected = rejected;
    st->pending = pending;
    st->expired = expired;
    st->year = year;
    st->active = 1;
    po.n_stats++;
    print_str("[PO] Stat "); print_int(po.n_stats - 1);
    print_str(" fld="); print_int(field);
    print_str(" fl="); print_int(filed);
    print_str(" gr="); print_int(granted);
    print_str(" rj="); print_int(rejected);
    print_str(" yr="); print_int(year); print_str("\n");
    return po.n_stats - 1;
}

void po_application_report(void) {
    print_str("[PO] Application report:\n");
    print_str("  Applications: "); print_int(po.n_apps); print_str("\n");
    print_str("  Total filed: "); print_int(po.total_filed); print_str("\n");
    print_str("  Granted: "); print_int(po.total_granted); print_str("\n");
    print_str("  Rejected: "); print_int(po.total_rejected); print_str("\n");
}

void po_examination_report(void) {
    print_str("[PO] Examination report:\n");
    print_str("  Examinations: "); print_int(po.n_exams); print_str("\n");
    int total_months = 0;
    for (int i = 0; i < po.n_exams; i++) total_months += examinations[i].months_taken;
    int avg = po.n_exams > 0 ? total_months / po.n_exams : 0;
    print_str("  Avg months: "); print_int(avg); print_str("\n");
}

void po_revenue_report(void) {
    print_str("[PO] Revenue report:\n");
    print_str("  Fees collected: "); print_int(po.n_maint); print_str("\n");
    print_str("  Total revenue: $"); print_int(po.total_fees); print_str("\n");
    print_str("  Searches: "); print_int(po.total_searches); print_str("\n");
}

void po_print_state(void) {
    print_str("[PO] Ap="); print_int(po.n_apps);
    print_str(" Ex="); print_int(po.n_exams);
    print_str(" Mt="); print_int(po.n_maint);
    print_str(" Sr="); print_int(po.n_searches);
    print_str(" St="); print_int(po.n_stats);
    print_str("\n");
}

int main(void) {
    print_str("=== Patent Office Demo ===\n\n");
    po_init();

    print_str("Filing applications...\n");
    for (int i = 0; i < 18; i++) {
        int type = (i % 3) + 1;
        int field = (i % 8) + 1;
        int app = 1000 + (i * 7);
        int claims = 5 + (i % 15);
        int pages = 10 + (i * 3);
        int year = 2018 + (i % 7);
        po_file_application(type, field, app, claims, pages, year);
    }

    print_str("\nExamining applications...\n");
    for (int i = 0; i < 14; i++) {
        int app = i % 18;
        int examiner = 200 + (i % 8);
        int type = (i % 2) + 1;
        int result = (i % 3);
        int cit = 2 + (i % 10);
        int months = 6 + (i % 18);
        int year = 2020 + (i % 5);
        po_examine(app, examiner, type, result, cit, months, year);
    }

    print_str("\nPaying maintenance fees...\n");
    for (int i = 0; i < 12; i++) {
        int pat = i % 14;
        int fee_type = (i % 3) + 1;
        int amount = 500 + (i * 200);
        int paid = 2022 + (i % 3);
        int due = paid + 1;
        po_pay_fee(pat, fee_type, amount, paid, due);
    }

    print_str("\nConducting searches...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 3) + 1;
        int field = (i % 8) + 1;
        int res = 5 + (i * 3);
        int cit = 1 + (i % 5);
        int pa = i % 3;
        int year = 2023 + (i % 2);
        po_search(type, field, res, cit, pa, year);
    }

    print_str("\nRecording statistics...\n");
    for (int i = 0; i < 10; i++) {
        int field = (i % 8) + 1;
        int filed = 100 + (i * 50);
        int granted = 60 + (i * 30);
        int rejected = 10 + (i * 5);
        int pending = 30 + (i * 15);
        int expired = 5 + (i * 3);
        int year = 2020 + (i % 5);
        po_record_stat(field, filed, granted, rejected, pending, expired, year);
    }

    print_str("\nApplication report...\n");
    po_application_report();

    print_str("\nExamination report...\n");
    po_examination_report();

    print_str("\nRevenue report...\n");
    po_revenue_report();

    print_str("\nFinal state...\n");
    po_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
