/* prosecution_service: Prosecution service management (v1.0)
 * Case review, legal supervision, public interest, duty crimes, statistics
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

#define MAX_REVIEWS      18
#define MAX_SUPERV       14
#define MAX_PUBLIC       12
#define MAX_DUTY         10
#define MAX_STATS        10

typedef struct {
    int    review_id;
    int    type;
    int    case_id;
    int    prosecutor_id;
    int    suspect_id;
    int    charges;
    int    decision;
    int    year;
    int    active;
} review_t;

typedef struct {
    int    superv_id;
    int    type;
    int    case_id;
    int    target_id;
    int    violations;
    int    corrections;
    int    year;
    int    status;
    int    active;
} supervision_t;

typedef struct {
    int    pi_id;
    int    domain;
    int    respondent_id;
    int    affected;
    int    damages;
    int    result;
    int    year;
    int    status;
    int    active;
} public_interest_t;

typedef struct {
    int    duty_id;
    int    type;
    int    official_id;
    int    amount;
    int    evidence;
    int    result;
    int    year;
    int    status;
    int    active;
} duty_crime_t;

typedef struct {
    int    stat_id;
    int    office_id;
    int    reviewed;
    int    prosecuted;
    int    supervised;
    int    public_int;
    int    duty_crimes;
    int    year;
    int    active;
} stat_t;

typedef struct {
    int    n_reviews;
    int    n_superv;
    int    n_public;
    int    n_duty;
    int    n_stats;
    int    total_reviewed;
    int    total_prosecuted;
    int    total_corrections;
    int    total_damages;
    int    total_duty;
} ps_state_t;

static review_t reviews[MAX_REVIEWS];
static supervision_t supervisions[MAX_SUPERV];
static public_interest_t public_interests[MAX_PUBLIC];
static duty_crime_t duty_crimes[MAX_DUTY];
static stat_t stats[MAX_STATS];
static ps_state_t ps;

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

int ps_init(void) {
    if (initialized) return -1;
    ps.n_reviews = 0; ps.n_superv = 0; ps.n_public = 0;
    ps.n_duty = 0; ps.n_stats = 0;
    ps.total_reviewed = 0; ps.total_prosecuted = 0;
    ps.total_corrections = 0; ps.total_damages = 0;
    ps.total_duty = 0;
    for (int i = 0; i < MAX_REVIEWS; i++) reviews[i].active = 0;
    for (int i = 0; i < MAX_SUPERV; i++) supervisions[i].active = 0;
    for (int i = 0; i < MAX_PUBLIC; i++) public_interests[i].active = 0;
    for (int i = 0; i < MAX_DUTY; i++) duty_crimes[i].active = 0;
    for (int i = 0; i < MAX_STATS; i++) stats[i].active = 0;
    initialized = 1;
    print_str("[PS] Prosecution service initialized\n");
    return 0;
}

int ps_review_case(int type, int case_id, int prosecutor, int suspect, int charges, int decision, int year) {
    if (ps.n_reviews >= MAX_REVIEWS) return -1;
    review_t* r = &reviews[ps.n_reviews];
    r->review_id = ps.n_reviews;
    r->type = type;
    r->case_id = case_id;
    r->prosecutor_id = prosecutor;
    r->suspect_id = suspect;
    r->charges = charges;
    r->decision = decision;
    r->year = year;
    r->active = 1;
    ps.total_reviewed++;
    if (decision == 1) ps.total_prosecuted++;
    ps.n_reviews++;
    print_str("[PS] Review "); print_int(ps.n_reviews - 1);
    print_str(" type="); print_int(type);
    print_str(" case="); print_int(case_id);
    print_str(" pros="); print_int(prosecutor);
    print_str(" chg="); print_int(charges);
    print_str(" dec="); print_int(decision); print_str("\n");
    return ps.n_reviews - 1;
}

int ps_supervise(int type, int case_id, int target, int violations, int corrections, int year) {
    if (ps.n_superv >= MAX_SUPERV) return -1;
    supervision_t* s = &supervisions[ps.n_superv];
    s->superv_id = ps.n_superv;
    s->type = type;
    s->case_id = case_id;
    s->target_id = target;
    s->violations = violations;
    s->corrections = corrections;
    s->year = year;
    s->status = 1;
    s->active = 1;
    ps.total_corrections += corrections;
    ps.n_superv++;
    print_str("[PS] Supervision "); print_int(ps.n_superv - 1);
    print_str(" type="); print_int(type);
    print_str(" case="); print_int(case_id);
    print_str(" tgt="); print_int(target);
    print_str(" viol="); print_int(violations);
    print_str(" corr="); print_int(corrections); print_str("\n");
    return ps.n_superv - 1;
}

int ps_public_interest(int domain, int respondent, int affected, int damages, int result, int year) {
    if (ps.n_public >= MAX_PUBLIC) return -1;
    public_interest_t* p = &public_interests[ps.n_public];
    p->pi_id = ps.n_public;
    p->domain = domain;
    p->respondent_id = respondent;
    p->affected = affected;
    p->damages = damages;
    p->result = result;
    p->year = year;
    p->status = 1;
    p->active = 1;
    ps.total_damages += damages;
    ps.n_public++;
    print_str("[PS] PublicInt "); print_int(ps.n_public - 1);
    print_str(" dom="); print_int(domain);
    print_str(" resp="); print_int(respondent);
    print_str(" aff="); print_int(affected);
    print_str(" dmg=$"); print_int(damages);
    print_str(" res="); print_int(result); print_str("\n");
    return ps.n_public - 1;
}

int ps_duty_crime(int type, int official, int amount, int evidence, int result, int year) {
    if (ps.n_duty >= MAX_DUTY) return -1;
    duty_crime_t* d = &duty_crimes[ps.n_duty];
    d->duty_id = ps.n_duty;
    d->type = type;
    d->official_id = official;
    d->amount = amount;
    d->evidence = evidence;
    d->result = result;
    d->year = year;
    d->status = 1;
    d->active = 1;
    ps.total_duty++;
    ps.n_duty++;
    print_str("[PS] DutyCrime "); print_int(ps.n_duty - 1);
    print_str(" type="); print_int(type);
    print_str(" off="); print_int(official);
    print_str(" amt=$"); print_int(amount);
    print_str(" ev="); print_int(evidence);
    print_str(" res="); print_int(result); print_str("\n");
    return ps.n_duty - 1;
}

int ps_record_stat(int office, int reviewed, int prosecuted, int supervised, int public_int, int duty, int year) {
    if (ps.n_stats >= MAX_STATS) return -1;
    stat_t* s = &stats[ps.n_stats];
    s->stat_id = ps.n_stats;
    s->office_id = office;
    s->reviewed = reviewed;
    s->prosecuted = prosecuted;
    s->supervised = supervised;
    s->public_int = public_int;
    s->duty_crimes = duty;
    s->year = year;
    s->active = 1;
    ps.n_stats++;
    print_str("[PS] Stat "); print_int(ps.n_stats - 1);
    print_str(" off="); print_int(office);
    print_str(" rv="); print_int(reviewed);
    print_str(" pr="); print_int(prosecuted);
    print_str(" sp="); print_int(supervised);
    print_str(" yr="); print_int(year); print_str("\n");
    return ps.n_stats - 1;
}

void ps_review_report(void) {
    print_str("[PS] Review report:\n");
    print_str("  Cases reviewed: "); print_int(ps.total_reviewed); print_str("\n");
    print_str("  Prosecuted: "); print_int(ps.total_prosecuted); print_str("\n");
    print_str("  Supervisions: "); print_int(ps.n_superv); print_str("\n");
}

void ps_public_report(void) {
    print_str("[PS] Public interest report:\n");
    print_str("  Cases: "); print_int(ps.n_public); print_str("\n");
    print_str("  Total damages: $"); print_int(ps.total_damages); print_str("\n");
    print_str("  Corrections: "); print_int(ps.total_corrections); print_str("\n");
}

void ps_duty_report(void) {
    print_str("[PS] Duty crime report:\n");
    print_str("  Cases: "); print_int(ps.total_duty); print_str("\n");
    print_str("  Statistics entries: "); print_int(ps.n_stats); print_str("\n");
}

void ps_print_state(void) {
    print_str("[PS] Rv="); print_int(ps.n_reviews);
    print_str(" Sp="); print_int(ps.n_superv);
    print_str(" Pi="); print_int(ps.n_public);
    print_str(" Du="); print_int(ps.n_duty);
    print_str(" St="); print_int(ps.n_stats);
    print_str("\n");
}

int main(void) {
    print_str("=== Prosecution Service Demo ===\n\n");
    ps_init();

    print_str("Reviewing cases...\n");
    for (int i = 0; i < 18; i++) {
        int type = (i % 3) + 1;
        int case_id = 100 + (i * 7);
        int pros = 500 + (i % 10);
        int suspect = 600 + (i * 3);
        int charges = 1 + (i % 5);
        int decision = (i % 3);
        int year = 2019 + (i % 6);
        ps_review_case(type, case_id, pros, suspect, charges, decision, year);
    }

    print_str("\nLegal supervision...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 3) + 1;
        int case_id = 200 + (i * 5);
        int target = 700 + (i * 4);
        int violations = 1 + (i % 6);
        int corrections = i % 4;
        int year = 2020 + (i % 5);
        ps_supervise(type, case_id, target, violations, corrections, year);
    }

    print_str("\nPublic interest cases...\n");
    for (int i = 0; i < 12; i++) {
        int domain = (i % 4) + 1;
        int resp = 800 + (i * 10);
        int aff = 100 + (i * 50);
        int dmg = 50000 + (i * 20000);
        int result = (i % 3) + 1;
        int year = 2021 + (i % 4);
        ps_public_interest(domain, resp, aff, dmg, result, year);
    }

    print_str("\nDuty crime investigations...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 3) + 1;
        int official = 900 + (i * 5);
        int amount = 100000 + (i * 50000);
        int evidence = 3 + (i % 8);
        int result = (i % 3);
        int year = 2022 + (i % 3);
        ps_duty_crime(type, official, amount, evidence, result, year);
    }

    print_str("\nRecording statistics...\n");
    for (int i = 0; i < 10; i++) {
        int office = (i % 5) + 1;
        int reviewed = 50 + (i * 20);
        int prosecuted = 30 + (i * 15);
        int supervised = 20 + (i * 10);
        int pi = 5 + (i * 3);
        int duty = 2 + (i * 2);
        int year = 2020 + (i % 4);
        ps_record_stat(office, reviewed, prosecuted, supervised, pi, duty, year);
    }

    print_str("\nReview report...\n");
    ps_review_report();

    print_str("\nPublic interest report...\n");
    ps_public_report();

    print_str("\nDuty crime report...\n");
    ps_duty_report();

    print_str("\nFinal state...\n");
    ps_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
