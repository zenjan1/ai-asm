/* court_system: Court management system (v1.0)
 * Cases, trials, judges, litigation services, judicial statistics
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

#define MAX_CASES        18
#define MAX_TRIALS       14
#define MAX_JUDGES       12
#define MAX_SERVICES     10
#define MAX_STATS        10

typedef struct {
    int    case_id;
    int    type;
    int    plaintiff_id;
    int    defendant_id;
    int    court_id;
    int    status;
    int    year_filed;
    int    amount;
    int    active;
} case_t;

typedef struct {
    int    trial_id;
    int    case_id;
    int    judge_id;
    int    hearing_date;
    int    duration_days;
    int    result;
    int    appeals;
    int    year;
    int    active;
} trial_t;

typedef struct {
    int    judge_id;
    int    court_id;
    int    rank;
    int    cases_assigned;
    int    cases_decided;
    int    reversal_rate;
    int    years_service;
    int    training_score;
    int    active;
} judge_t;

typedef struct {
    int    service_id;
    int    type;
    int    requester_id;
    int    case_id;
    int    status;
    int    documents;
    int    year;
    int    active;
} service_t;

typedef struct {
    int    stat_id;
    int    court_id;
    int    filed;
    int    closed;
    int    pending;
    int    mediated;
    int    appealed;
    int    year;
    int    active;
} stat_t;

typedef struct {
    int    n_cases;
    int    n_trials;
    int    n_judges;
    int    n_services;
    int    n_stats;
    int    total_closed;
    int    total_mediated;
    int    total_appealed;
    int    total_services;
    int    total_reversals;
} cs_state_t;

static case_t cases[MAX_CASES];
static trial_t trials[MAX_TRIALS];
static judge_t judges[MAX_JUDGES];
static service_t services[MAX_SERVICES];
static stat_t stats[MAX_STATS];
static cs_state_t cs;

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

int cs_init(void) {
    if (initialized) return -1;
    cs.n_cases = 0; cs.n_trials = 0; cs.n_judges = 0;
    cs.n_services = 0; cs.n_stats = 0;
    cs.total_closed = 0; cs.total_mediated = 0;
    cs.total_appealed = 0; cs.total_services = 0;
    cs.total_reversals = 0;
    for (int i = 0; i < MAX_CASES; i++) cases[i].active = 0;
    for (int i = 0; i < MAX_TRIALS; i++) trials[i].active = 0;
    for (int i = 0; i < MAX_JUDGES; i++) judges[i].active = 0;
    for (int i = 0; i < MAX_SERVICES; i++) services[i].active = 0;
    for (int i = 0; i < MAX_STATS; i++) stats[i].active = 0;
    initialized = 1;
    print_str("[CS] Court system initialized\n");
    return 0;
}

int cs_file_case(int type, int plaintiff, int defendant, int court, int amount, int year) {
    if (cs.n_cases >= MAX_CASES) return -1;
    case_t* c = &cases[cs.n_cases];
    c->case_id = cs.n_cases;
    c->type = type;
    c->plaintiff_id = plaintiff;
    c->defendant_id = defendant;
    c->court_id = court;
    c->status = 1;
    c->year_filed = year;
    c->amount = amount;
    c->active = 1;
    cs.n_cases++;
    print_str("[CS] Case "); print_int(cs.n_cases - 1);
    print_str(" type="); print_int(type);
    print_str(" pl="); print_int(plaintiff);
    print_str(" df="); print_int(defendant);
    print_str(" crt="); print_int(court);
    print_str(" amt=$"); print_int(amount);
    print_str(" yr="); print_int(year); print_str("\n");
    return cs.n_cases - 1;
}

int cs_schedule_trial(int case_id, int judge, int hearing, int duration, int result, int appeals, int year) {
    if (cs.n_trials >= MAX_TRIALS) return -1;
    trial_t* t = &trials[cs.n_trials];
    t->trial_id = cs.n_trials;
    t->case_id = case_id;
    t->judge_id = judge;
    t->hearing_date = hearing;
    t->duration_days = duration;
    t->result = result;
    t->appeals = appeals;
    t->year = year;
    t->active = 1;
    cs.total_closed++;
    if (appeals > 0) cs.total_appealed++;
    cs.n_trials++;
    print_str("[CS] Trial "); print_int(cs.n_trials - 1);
    print_str(" case="); print_int(case_id);
    print_str(" jdg="); print_int(judge);
    print_str(" dur="); print_int(duration); print_str("d");
    print_str(" res="); print_int(result);
    print_str(" ap="); print_int(appeals); print_str("\n");
    return cs.n_trials - 1;
}

int cs_add_judge(int court, int rank, int assigned, int decided, int reversal, int years, int training) {
    if (cs.n_judges >= MAX_JUDGES) return -1;
    judge_t* j = &judges[cs.n_judges];
    j->judge_id = cs.n_judges;
    j->court_id = court;
    j->rank = rank;
    j->cases_assigned = assigned;
    j->cases_decided = decided;
    j->reversal_rate = reversal;
    j->years_service = years;
    j->training_score = training;
    j->active = 1;
    cs.total_reversals += reversal;
    cs.n_judges++;
    print_str("[CS] Judge "); print_int(cs.n_judges - 1);
    print_str(" crt="); print_int(court);
    print_str(" rank="); print_int(rank);
    print_str(" asgn="); print_int(assigned);
    print_str(" dec="); print_int(decided);
    print_str(" rv="); print_int(reversal); print_str("%\n");
    return cs.n_judges - 1;
}

int cs_add_service(int type, int requester, int case_id, int status, int documents, int year) {
    if (cs.n_services >= MAX_SERVICES) return -1;
    service_t* s = &services[cs.n_services];
    s->service_id = cs.n_services;
    s->type = type;
    s->requester_id = requester;
    s->case_id = case_id;
    s->status = status;
    s->documents = documents;
    s->year = year;
    s->active = 1;
    cs.total_services++;
    cs.n_services++;
    print_str("[CS] Service "); print_int(cs.n_services - 1);
    print_str(" type="); print_int(type);
    print_str(" req="); print_int(requester);
    print_str(" case="); print_int(case_id);
    print_str(" doc="); print_int(documents); print_str("\n");
    return cs.n_services - 1;
}

int cs_record_stat(int court, int filed, int closed, int pending, int mediated, int appealed, int year) {
    if (cs.n_stats >= MAX_STATS) return -1;
    stat_t* st = &stats[cs.n_stats];
    st->stat_id = cs.n_stats;
    st->court_id = court;
    st->filed = filed;
    st->closed = closed;
    st->pending = pending;
    st->mediated = mediated;
    st->appealed = appealed;
    st->year = year;
    st->active = 1;
    cs.total_closed += closed;
    cs.total_mediated += mediated;
    cs.total_appealed += appealed;
    cs.n_stats++;
    print_str("[CS] Stat "); print_int(cs.n_stats - 1);
    print_str(" crt="); print_int(court);
    print_str(" fl="); print_int(filed);
    print_str(" cl="); print_int(closed);
    print_str(" med="); print_int(mediated);
    print_str(" yr="); print_int(year); print_str("\n");
    return cs.n_stats - 1;
}

void cs_case_report(void) {
    print_str("[CS] Case report:\n");
    print_str("  Cases filed: "); print_int(cs.n_cases); print_str("\n");
    print_str("  Trials: "); print_int(cs.n_trials); print_str("\n");
    print_str("  Total closed: "); print_int(cs.total_closed); print_str("\n");
}

void cs_judge_report(void) {
    print_str("[CS] Judge report:\n");
    print_str("  Judges: "); print_int(cs.n_judges); print_str("\n");
    int avg_rv = cs.n_judges > 0 ? cs.total_reversals / cs.n_judges : 0;
    print_str("  Avg reversal: "); print_int(avg_rv); print_str("%\n");
}

void cs_service_report(void) {
    print_str("[CS] Service report:\n");
    print_str("  Services: "); print_int(cs.total_services); print_str("\n");
    print_str("  Mediated: "); print_int(cs.total_mediated); print_str("\n");
    print_str("  Appealed: "); print_int(cs.total_appealed); print_str("\n");
}

void cs_print_state(void) {
    print_str("[CS] Cs="); print_int(cs.n_cases);
    print_str(" Tr="); print_int(cs.n_trials);
    print_str(" Jd="); print_int(cs.n_judges);
    print_str(" Sv="); print_int(cs.n_services);
    print_str(" St="); print_int(cs.n_stats);
    print_str("\n");
}

int main(void) {
    print_str("=== Court System Demo ===\n\n");
    cs_init();

    print_str("Filing cases...\n");
    for (int i = 0; i < 18; i++) {
        int type = (i % 3) + 1;
        int pl = 1000 + (i * 7);
        int df = 2000 + (i * 5);
        int court = (i % 5) + 1;
        int amt = 10000 + (i * 5000);
        int year = 2019 + (i % 6);
        cs_file_case(type, pl, df, court, amt, year);
    }

    print_str("\nScheduling trials...\n");
    for (int i = 0; i < 14; i++) {
        int cs_id = i % 18;
        int jdg = i % 12;
        int hearing = 20220101 + (i * 30);
        int dur = 1 + (i % 10);
        int res = (i % 3) + 1;
        int ap = i % 3;
        int year = 2022 + (i % 3);
        cs_schedule_trial(cs_id, jdg, hearing, dur, res, ap, year);
    }

    print_str("\nAdding judges...\n");
    for (int i = 0; i < 12; i++) {
        int court = (i % 5) + 1;
        int rank = (i % 4) + 1;
        int asgn = 20 + (i * 5);
        int dec = 15 + (i * 4);
        int rv = 3 + (i % 8);
        int yrs = 5 + (i % 20);
        int train = 70 + (i * 2);
        cs_add_judge(court, rank, asgn, dec, rv, yrs, train);
    }

    print_str("\nProviding services...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int req = 3000 + (i * 11);
        int case_id = i % 18;
        int status = (i % 3) + 1;
        int docs = 2 + (i % 6);
        int year = 2023 + (i % 2);
        cs_add_service(type, req, case_id, status, docs, year);
    }

    print_str("\nRecording statistics...\n");
    for (int i = 0; i < 10; i++) {
        int court = (i % 5) + 1;
        int filed = 200 + (i * 50);
        int closed = 180 + (i * 45);
        int pending = 20 + (i * 5);
        int mediated = 50 + (i * 10);
        int appealed = 10 + (i * 3);
        int year = 2020 + (i % 4);
        cs_record_stat(court, filed, closed, pending, mediated, appealed, year);
    }

    print_str("\nCase report...\n");
    cs_case_report();

    print_str("\nJudge report...\n");
    cs_judge_report();

    print_str("\nService report...\n");
    cs_service_report();

    print_str("\nFinal state...\n");
    cs_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
