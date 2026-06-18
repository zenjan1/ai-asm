/* consumer_protection: Consumer rights protection system (v1.0)
 * Complaints, market supervision, rights protection, credit, education
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

#define MAX_COMPLAINT    16
#define MAX_MARKET       14
#define MAX_RIGHTS       12
#define MAX_CREDIT       10
#define MAX_EDUCATION    10

typedef struct {
    int    complaint_id;
    int    consumer_id;
    int    business_id;
    int    category;
    int    amount;
    int    priority;
    int    resolved;
    int    year;
    int    active;
} complaint_t;

typedef struct {
    int    market_id;
    int    vendor_id;
    int    type;
    int    sample_count;
    int    pass_count;
    int    violation_type;
    int    penalty;
    int    year;
    int    active;
} market_t;

typedef struct {
    int    rights_id;
    int    case_id;
    int    type;
    int    claimant_id;
    int    compensation;
    int    mediation_success;
    int    year;
    int    status;
    int    active;
} rights_t;

typedef struct {
    int    credit_id;
    int    business_id;
    int    score;
    int    violations;
    int    blacklist;
    int    warnings_issued;
    int    year;
    int    active;
} credit_t;

typedef struct {
    int    edu_id;
    int    type;
    int    audience;
    int    reach;
    int    satisfaction;
    int    budget;
    int    year;
    int    active;
} education_t;

typedef struct {
    int    n_complaint;
    int    n_market;
    int    n_rights;
    int    n_credit;
    int    n_education;
    int    total_compensation;
    int    total_resolved;
    int    total_penalty;
    int    total_reach;
    int    total_warnings;
} cp_state_t;

static complaint_t complaints[MAX_COMPLAINT];
static market_t markets[MAX_MARKET];
static rights_t rights_cases[MAX_RIGHTS];
static credit_t credits[MAX_CREDIT];
static education_t educations[MAX_EDUCATION];
static cp_state_t cp;

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

int cp_init(void) {
    if (initialized) return -1;
    cp.n_complaint = 0; cp.n_market = 0; cp.n_rights = 0;
    cp.n_credit = 0; cp.n_education = 0;
    cp.total_compensation = 0; cp.total_resolved = 0;
    cp.total_penalty = 0; cp.total_reach = 0;
    cp.total_warnings = 0;
    for (int i = 0; i < MAX_COMPLAINT; i++) complaints[i].active = 0;
    for (int i = 0; i < MAX_MARKET; i++) markets[i].active = 0;
    for (int i = 0; i < MAX_RIGHTS; i++) rights_cases[i].active = 0;
    for (int i = 0; i < MAX_CREDIT; i++) credits[i].active = 0;
    for (int i = 0; i < MAX_EDUCATION; i++) educations[i].active = 0;
    initialized = 1;
    print_str("[CP] Consumer protection initialized\n");
    return 0;
}

int cp_complain(int consumer, int business, int category, int amount, int priority, int resolved, int year) {
    if (cp.n_complaint >= MAX_COMPLAINT) return -1;
    complaint_t* c = &complaints[cp.n_complaint];
    c->complaint_id = cp.n_complaint;
    c->consumer_id = consumer;
    c->business_id = business;
    c->category = category;
    c->amount = amount;
    c->priority = priority;
    c->resolved = resolved;
    c->year = year;
    c->active = 1;
    if (resolved) cp.total_resolved++;
    cp.n_complaint++;
    print_str("[CP] Complaint "); print_int(cp.n_complaint - 1);
    print_str(" con="); print_int(consumer);
    print_str(" biz="); print_int(business);
    print_str(" cat="); print_int(category);
    print_str(" amt=$"); print_int(amount);
    print_str(" pri="); print_int(priority); print_str("\n");
    return cp.n_complaint - 1;
}

int cp_inspect(int vendor, int type, int samples, int pass, int violation, int penalty, int year) {
    if (cp.n_market >= MAX_MARKET) return -1;
    market_t* m = &markets[cp.n_market];
    m->market_id = cp.n_market;
    m->vendor_id = vendor;
    m->type = type;
    m->sample_count = samples;
    m->pass_count = pass;
    m->violation_type = violation;
    m->penalty = penalty;
    m->year = year;
    m->active = 1;
    cp.total_penalty += penalty;
    cp.n_market++;
    print_str("[CP] Market "); print_int(cp.n_market - 1);
    print_str(" vnd="); print_int(vendor);
    print_str(" type="); print_int(type);
    print_str(" smp="); print_int(samples);
    print_str(" pass="); print_int(pass);
    print_str(" pen=$"); print_int(penalty); print_str("\n");
    return cp.n_market - 1;
}

int cp_protect(int caseno, int type, int claimant, int compensation, int mediation, int year) {
    if (cp.n_rights >= MAX_RIGHTS) return -1;
    rights_t* r = &rights_cases[cp.n_rights];
    r->rights_id = cp.n_rights;
    r->case_id = caseno;
    r->type = type;
    r->claimant_id = claimant;
    r->compensation = compensation;
    r->mediation_success = mediation;
    r->year = year;
    r->status = 1;
    r->active = 1;
    cp.total_compensation += compensation;
    cp.n_rights++;
    print_str("[CP] Rights "); print_int(cp.n_rights - 1);
    print_str(" cas="); print_int(caseno);
    print_str(" type="); print_int(type);
    print_str(" clm="); print_int(claimant);
    print_str(" cmp=$"); print_int(compensation);
    print_str(" med="); print_int(mediation); print_str("\n");
    return cp.n_rights - 1;
}

int cp_add_credit(int business, int score, int violations, int blacklist, int warnings, int year) {
    if (cp.n_credit >= MAX_CREDIT) return -1;
    credit_t* cr = &credits[cp.n_credit];
    cr->credit_id = cp.n_credit;
    cr->business_id = business;
    cr->score = score;
    cr->violations = violations;
    cr->blacklist = blacklist;
    cr->warnings_issued = warnings;
    cr->year = year;
    cr->active = 1;
    cp.total_warnings += warnings;
    cp.n_credit++;
    print_str("[CP] Credit "); print_int(cp.n_credit - 1);
    print_str(" biz="); print_int(business);
    print_str(" score="); print_int(score);
    print_str(" viol="); print_int(violations);
    print_str(" bl="); print_int(blacklist);
    print_str(" wrn="); print_int(warnings); print_str("\n");
    return cp.n_credit - 1;
}

int cp_educate(int type, int audience, int reach, int satisfaction, int budget, int year) {
    if (cp.n_education >= MAX_EDUCATION) return -1;
    education_t* e = &educations[cp.n_education];
    e->edu_id = cp.n_education;
    e->type = type;
    e->audience = audience;
    e->reach = reach;
    e->satisfaction = satisfaction;
    e->budget = budget;
    e->year = year;
    e->active = 1;
    cp.total_reach += reach;
    cp.n_education++;
    print_str("[CP] Education "); print_int(cp.n_education - 1);
    print_str(" type="); print_int(type);
    print_str(" aud="); print_int(audience);
    print_str(" rch="); print_int(reach);
    print_str(" sat="); print_int(satisfaction);
    print_str(" bgt=$"); print_int(budget); print_str("\n");
    return cp.n_education - 1;
}

void cp_complaint_report(void) {
    print_str("[CP] Complaint report:\n");
    print_str("  Complaints: "); print_int(cp.n_complaint); print_str("\n");
    print_str("  Resolved: "); print_int(cp.total_resolved); print_str("\n");
}

void cp_market_report(void) {
    print_str("[CP] Market report:\n");
    print_str("  Inspections: "); print_int(cp.n_market); print_str("\n");
    print_str("  Total penalties: $"); print_int(cp.total_penalty); print_str("\n");
}

void cp_rights_report(void) {
    print_str("[CP] Rights report:\n");
    print_str("  Cases: "); print_int(cp.n_rights); print_str("\n");
    print_str("  Total compensation: $"); print_int(cp.total_compensation); print_str("\n");
    print_str("  Credit records: "); print_int(cp.n_credit); print_str("\n");
    print_str("  Total warnings: "); print_int(cp.total_warnings); print_str("\n");
    print_str("  Education reach: "); print_int(cp.total_reach); print_str("\n");
}

void cp_print_state(void) {
    print_str("[CP] Cm="); print_int(cp.n_complaint);
    print_str(" Mk="); print_int(cp.n_market);
    print_str(" Rt="); print_int(cp.n_rights);
    print_str(" Cr="); print_int(cp.n_credit);
    print_str(" Ed="); print_int(cp.n_education);
    print_str("\n");
}

int main(void) {
    print_str("=== Consumer Protection Demo ===\n\n");
    cp_init();

    print_str("Processing complaints...\n");
    for (int i = 0; i < 16; i++) {
        int con = 1000 + (i * 13);
        int biz = 2000 + (i * 7);
        int cat = (i % 5) + 1;
        int amt = 100 + (i * 200);
        int pri = (i % 4) + 1;
        int res = (i % 3 == 0) ? 0 : 1;
        int year = 2022 + (i % 3);
        cp_complain(con, biz, cat, amt, pri, res, year);
    }

    print_str("\nMarket supervision...\n");
    for (int i = 0; i < 14; i++) {
        int vnd = 3000 + (i * 11);
        int type = (i % 4) + 1;
        int smp = 10 + (i * 5);
        int pass = smp - (i % 4);
        int viol = (i % 3) + 1;
        int pen = 500 + (i * 300);
        int year = 2023 + (i % 2);
        cp_inspect(vnd, type, smp, pass, viol, pen, year);
    }

    print_str("\nRights protection...\n");
    for (int i = 0; i < 12; i++) {
        int cas = 4000 + (i * 17);
        int type = (i % 4) + 1;
        int clm = 5000 + (i * 13);
        int cmp = 1000 + (i * 500);
        int med = (i % 3 == 0) ? 0 : 1;
        int year = 2023 + (i % 2);
        cp_protect(cas, type, clm, cmp, med, year);
    }

    print_str("\nCredit management...\n");
    for (int i = 0; i < 10; i++) {
        int biz = 6000 + (i * 7);
        int score = 60 + (i * 4);
        int viol = i % 5;
        int bl = (i % 5 == 0) ? 1 : 0;
        int wrn = 1 + (i % 3);
        int year = 2024;
        cp_add_credit(biz, score, viol, bl, wrn, year);
    }

    print_str("\nConsumer education...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 3) + 1;
        int aud = (i % 4) + 1;
        int rch = 500 + (i * 200);
        int sat = 70 + (i * 3);
        int bgt = 10000 + (i * 5000);
        int year = 2024;
        cp_educate(type, aud, rch, sat, bgt, year);
    }

    print_str("\nComplaint report...\n");
    cp_complaint_report();

    print_str("\nMarket report...\n");
    cp_market_report();

    print_str("\nRights report...\n");
    cp_rights_report();

    print_str("\nFinal state...\n");
    cp_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
