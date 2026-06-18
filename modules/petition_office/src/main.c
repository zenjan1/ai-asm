/* petition_office: Petition and complaint administration system (v1.0)
 * Letters, visits, online complaints, case supervision, statistical analysis
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

#define MAX_LETTER       16
#define MAX_VISIT        14
#define MAX_ONLINE       12
#define MAX_CASE         10
#define MAX_STATISTICS   10

typedef struct {
    int    letter_id;
    int    category;
    int    priority;
    int    letters_received;
    int    processed;
    int    forwarded;
    int    year;
    int    active;
} letter_t;

typedef struct {
    int    visit_id;
    int    visit_type;
    int    region_id;
    int    visitors;
    int    interviews;
    int    counseling;
    int    year;
    int    active;
} visit_t;

typedef struct {
    int    online_id;
    int    platform_type;
    int    channel;
    int    complaints;
    int    responded;
    int    resolved;
    int    year;
    int    active;
} online_t;

typedef struct {
    int    case_id;
    int    case_type;
    int    department_id;
    int    key_cases;
    int    assigned;
    int    closed;
    int    year;
    int    active;
} case_t;

typedef struct {
    int    statistics_id;
    int    region_id;
    int    data_type;
    int    total_petitions;
    int    resolved_cases;
    int    satisfaction_score;
    int    year;
    int    active;
} statistics_t;

typedef struct {
    int    n_letter;
    int    n_visit;
    int    n_online;
    int    n_case;
    int    n_statistics;
    int    total_letters;
    int    total_visitors;
    int    total_complaints;
    int    total_key_cases;
    int    total_resolved;
} po_state_t;

static letter_t letters[MAX_LETTER];
static visit_t visits[MAX_VISIT];
static online_t onlines[MAX_ONLINE];
static case_t cases[MAX_CASE];
static statistics_t statistics[MAX_STATISTICS];
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
    po.n_letter = 0; po.n_visit = 0; po.n_online = 0;
    po.n_case = 0; po.n_statistics = 0;
    po.total_letters = 0; po.total_visitors = 0;
    po.total_complaints = 0; po.total_key_cases = 0;
    po.total_resolved = 0;
    for (int i = 0; i < MAX_LETTER; i++) letters[i].active = 0;
    for (int i = 0; i < MAX_VISIT; i++) visits[i].active = 0;
    for (int i = 0; i < MAX_ONLINE; i++) onlines[i].active = 0;
    for (int i = 0; i < MAX_CASE; i++) cases[i].active = 0;
    for (int i = 0; i < MAX_STATISTICS; i++) statistics[i].active = 0;
    initialized = 1;
    print_str("[PO] Petition office initialized\n");
    return 0;
}

int po_letter(int category, int priority, int received, int processed, int forwarded, int year) {
    if (po.n_letter >= MAX_LETTER) return -1;
    letter_t* l = &letters[po.n_letter];
    l->letter_id = po.n_letter;
    l->category = category;
    l->priority = priority;
    l->letters_received = received;
    l->processed = processed;
    l->forwarded = forwarded;
    l->year = year;
    l->active = 1;
    po.total_letters += received;
    po.n_letter++;
    print_str("[PO] Letter "); print_int(po.n_letter - 1);
    print_str(" cat="); print_int(category);
    print_str(" pri="); print_int(priority);
    print_str(" rcv="); print_int(received);
    print_str(" prc="); print_int(processed);
    print_str(" fwd="); print_int(forwarded); print_str("\n");
    return po.n_letter - 1;
}

int po_visit(int visit_type, int region, int visitors, int interviews, int counseling, int year) {
    if (po.n_visit >= MAX_VISIT) return -1;
    visit_t* v = &visits[po.n_visit];
    v->visit_id = po.n_visit;
    v->visit_type = visit_type;
    v->region_id = region;
    v->visitors = visitors;
    v->interviews = interviews;
    v->counseling = counseling;
    v->year = year;
    v->active = 1;
    po.total_visitors += visitors;
    po.n_visit++;
    print_str("[PO] Visit "); print_int(po.n_visit - 1);
    print_str(" type="); print_int(visit_type);
    print_str(" rgn="); print_int(region);
    print_str(" vis="); print_int(visitors);
    print_str(" int="); print_int(interviews);
    print_str(" cns="); print_int(counseling); print_str("\n");
    return po.n_visit - 1;
}

int po_online(int platform, int channel, int complaints, int responded, int resolved, int year) {
    if (po.n_online >= MAX_ONLINE) return -1;
    online_t* o = &onlines[po.n_online];
    o->online_id = po.n_online;
    o->platform_type = platform;
    o->channel = channel;
    o->complaints = complaints;
    o->responded = responded;
    o->resolved = resolved;
    o->year = year;
    o->active = 1;
    po.total_complaints += complaints;
    po.total_resolved += resolved;
    po.n_online++;
    print_str("[PO] Online "); print_int(po.n_online - 1);
    print_str(" plt="); print_int(platform);
    print_str(" chn="); print_int(channel);
    print_str(" cmp="); print_int(complaints);
    print_str(" rsp="); print_int(responded);
    print_str(" rsl="); print_int(resolved); print_str("\n");
    return po.n_online - 1;
}

int po_case(int case_type, int dept, int key_cases, int assigned, int closed, int year) {
    if (po.n_case >= MAX_CASE) return -1;
    case_t* c = &cases[po.n_case];
    c->case_id = po.n_case;
    c->case_type = case_type;
    c->department_id = dept;
    c->key_cases = key_cases;
    c->assigned = assigned;
    c->closed = closed;
    c->year = year;
    c->active = 1;
    po.total_key_cases += key_cases;
    po.n_case++;
    print_str("[PO] Case "); print_int(po.n_case - 1);
    print_str(" type="); print_int(case_type);
    print_str(" dpt="); print_int(dept);
    print_str(" key="); print_int(key_cases);
    print_str(" asg="); print_int(assigned);
    print_str(" cls="); print_int(closed); print_str("\n");
    return po.n_case - 1;
}

int po_statistics(int region, int data_type, int total, int resolved, int satisfaction, int year) {
    if (po.n_statistics >= MAX_STATISTICS) return -1;
    statistics_t* s = &statistics[po.n_statistics];
    s->statistics_id = po.n_statistics;
    s->region_id = region;
    s->data_type = data_type;
    s->total_petitions = total;
    s->resolved_cases = resolved;
    s->satisfaction_score = satisfaction;
    s->year = year;
    s->active = 1;
    po.n_statistics++;
    print_str("[PO] Statistics "); print_int(po.n_statistics - 1);
    print_str(" rgn="); print_int(region);
    print_str(" type="); print_int(data_type);
    print_str(" tot="); print_int(total);
    print_str(" rsl="); print_int(resolved);
    print_str(" sat="); print_int(satisfaction); print_str("%\n");
    return po.n_statistics - 1;
}

void po_letter_report(void) {
    print_str("[PO] Letter report:\n");
    print_str("  Letter categories: "); print_int(po.n_letter); print_str("\n");
    print_str("  Total letters received: "); print_int(po.total_letters); print_str("\n");
}

void po_visit_report(void) {
    print_str("[PO] Visit report:\n");
    print_str("  Visit regions: "); print_int(po.n_visit); print_str("\n");
    print_str("  Total visitors: "); print_int(po.total_visitors); print_str("\n");
}

void po_case_report(void) {
    print_str("[PO] Case report:\n");
    print_str("  Online platforms: "); print_int(po.n_online); print_str("\n");
    print_str("  Total complaints: "); print_int(po.total_complaints); print_str("\n");
    print_str("  Total resolved: "); print_int(po.total_resolved); print_str("\n");
    print_str("  Key cases: "); print_int(po.n_case); print_str("\n");
    print_str("  Total key cases: "); print_int(po.total_key_cases); print_str("\n");
    print_str("  Statistical regions: "); print_int(po.n_statistics); print_str("\n");
}

void po_print_state(void) {
    print_str("[PO] Lt="); print_int(po.n_letter);
    print_str(" Vs="); print_int(po.n_visit);
    print_str(" On="); print_int(po.n_online);
    print_str(" Cs="); print_int(po.n_case);
    print_str(" St="); print_int(po.n_statistics);
    print_str("\n");
}

int main(void) {
    print_str("=== Petition Office Demo ===\n\n");
    po_init();

    print_str("Letter processing...\n");
    for (int i = 0; i < 16; i++) {
        int cat = (i % 8) + 1;
        int pri = (i % 4) + 1;
        int rcv = 500 + (i * 100);
        int prc = rcv - (i * 10);
        int fwd = prc / 2;
        int year = 2020 + (i % 5);
        po_letter(cat, pri, rcv, prc, fwd, year);
    }

    print_str("\nVisit reception...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 5) + 1;
        int rgn = (i % 10) + 1;
        int vis = 200 + (i * 50);
        int intv = vis - (i * 5);
        int cns = intv / 3;
        int year = 2021 + (i % 4);
        po_visit(type, rgn, vis, intv, cns, year);
    }

    print_str("\nOnline complaints...\n");
    for (int i = 0; i < 12; i++) {
        int plt = (i % 4) + 1;
        int chn = (i % 6) + 1;
        int cmp = 1000 + (i * 200);
        int rsp = cmp - (i * 20);
        int rsl = rsp - (i * 30);
        int year = 2022 + (i % 3);
        po_online(plt, chn, cmp, rsp, rsl, year);
    }

    print_str("\nCase supervision...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 6) + 1;
        int dpt = 100 + (i * 11);
        int key = 20 + (i * 5);
        int asg = key + (i % 3);
        int cls = asg - (i % 4);
        int year = 2023 + (i % 2);
        po_case(type, dpt, key, asg, cls, year);
    }

    print_str("\nStatistical analysis...\n");
    for (int i = 0; i < 10; i++) {
        int rgn = (i % 8) + 1;
        int type = (i % 4) + 1;
        int tot = 2000 + (i * 500);
        int rsl = tot - (i * 100);
        int sat = 80 + (i % 15);
        int year = 2024;
        po_statistics(rgn, type, tot, rsl, sat, year);
    }

    print_str("\nLetter report...\n");
    po_letter_report();

    print_str("\nVisit report...\n");
    po_visit_report();

    print_str("\nCase report...\n");
    po_case_report();

    print_str("\nFinal state...\n");
    po_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
