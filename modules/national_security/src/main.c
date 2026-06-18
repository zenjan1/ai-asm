/* national_security: National security administration system (v1.0)
 * Intelligence, counter-espionage, national security, cyber security, coordination
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

#define MAX_INTELLIGENCE 16
#define MAX_COUNTERESP   14
#define MAX_SECURITY     12
#define MAX_CYBER        10
#define MAX_COORDINATION 10

typedef struct {
    int    intelligence_id;
    int    intel_type;
    int    source_type;
    int    intelligence_gathered;
    int    reports_produced;
    int    threats_identified;
    int    year;
    int    active;
} intelligence_t;

typedef struct {
    int    counteresp_id;
    int    case_type;
    int    threat_level;
    int    investigations;
    int    cases_resolved;
    int    prevention_ops;
    int    year;
    int    active;
} counteresp_t;

typedef struct {
    int    security_id;
    int    security_domain;
    int    risk_level;
    int    assessments_done;
    int    warnings_issued;
    int    responses_mobilized;
    int    year;
    int    active;
} security_t;

typedef struct {
    int    cyber_id;
    int    cyber_domain;
    int    threat_type;
    int    incidents_detected;
    int    attacks_blocked;
    int    systems_protected;
    int    year;
    int    active;
} cyber_t;

typedef struct {
    int    coordination_id;
    int    coord_type;
    int    partner_level;
    int    meetings_held;
    int    agreements_signed;
    int    joint_operations;
    int    year;
    int    active;
} coordination_t;

typedef struct {
    int    n_intelligence;
    int    n_counteresp;
    int    n_security;
    int    n_cyber;
    int    n_coordination;
    int    total_intelligence;
    int    total_cases;
    int    total_assessments;
    int    total_incidents;
    int    total_operations;
} nsec_state_t;

static intelligence_t intelligences[MAX_INTELLIGENCE];
static counteresp_t counteresps[MAX_COUNTERESP];
static security_t securities[MAX_SECURITY];
static cyber_t cybers[MAX_CYBER];
static coordination_t coordinations[MAX_COORDINATION];
static nsec_state_t nsec;

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

int nsec_init(void) {
    if (initialized) return -1;
    nsec.n_intelligence = 0; nsec.n_counteresp = 0; nsec.n_security = 0;
    nsec.n_cyber = 0; nsec.n_coordination = 0;
    nsec.total_intelligence = 0; nsec.total_cases = 0;
    nsec.total_assessments = 0; nsec.total_incidents = 0;
    nsec.total_operations = 0;
    for (int i = 0; i < MAX_INTELLIGENCE; i++) intelligences[i].active = 0;
    for (int i = 0; i < MAX_COUNTERESP; i++) counteresps[i].active = 0;
    for (int i = 0; i < MAX_SECURITY; i++) securities[i].active = 0;
    for (int i = 0; i < MAX_CYBER; i++) cybers[i].active = 0;
    for (int i = 0; i < MAX_COORDINATION; i++) coordinations[i].active = 0;
    initialized = 1;
    print_str("[NSEC] National security initialized\n");
    return 0;
}

int nsec_intelligence(int intel_type, int source, int gathered, int reports, int threats, int year) {
    if (nsec.n_intelligence >= MAX_INTELLIGENCE) return -1;
    intelligence_t* intel = &intelligences[nsec.n_intelligence];
    intel->intelligence_id = nsec.n_intelligence;
    intel->intel_type = intel_type;
    intel->source_type = source;
    intel->intelligence_gathered = gathered;
    intel->reports_produced = reports;
    intel->threats_identified = threats;
    intel->year = year;
    intel->active = 1;
    nsec.total_intelligence += gathered;
    nsec.n_intelligence++;
    print_str("[NSEC] Intelligence "); print_int(nsec.n_intelligence - 1);
    print_str(" type="); print_int(intel_type);
    print_str(" src="); print_int(source);
    print_str(" gth="); print_int(gathered);
    print_str(" rpt="); print_int(reports);
    print_str(" thr="); print_int(threats); print_str("\n");
    return nsec.n_intelligence - 1;
}

int nsec_counteresp(int case_type, int threat_lvl, int investigations, int resolved, int prevention, int year) {
    if (nsec.n_counteresp >= MAX_COUNTERESP) return -1;
    counteresp_t* c = &counteresps[nsec.n_counteresp];
    c->counteresp_id = nsec.n_counteresp;
    c->case_type = case_type;
    c->threat_level = threat_lvl;
    c->investigations = investigations;
    c->cases_resolved = resolved;
    c->prevention_ops = prevention;
    c->year = year;
    c->active = 1;
    nsec.total_cases += resolved;
    nsec.n_counteresp++;
    print_str("[NSEC] CounterEsp "); print_int(nsec.n_counteresp - 1);
    print_str(" case="); print_int(case_type);
    print_str(" thr="); print_int(threat_lvl);
    print_str(" inv="); print_int(investigations);
    print_str(" rsl="); print_int(resolved);
    print_str(" prv="); print_int(prevention); print_str("\n");
    return nsec.n_counteresp - 1;
}

int nsec_security(int domain, int risk, int assessments, int warnings, int responses, int year) {
    if (nsec.n_security >= MAX_SECURITY) return -1;
    security_t* s = &securities[nsec.n_security];
    s->security_id = nsec.n_security;
    s->security_domain = domain;
    s->risk_level = risk;
    s->assessments_done = assessments;
    s->warnings_issued = warnings;
    s->responses_mobilized = responses;
    s->year = year;
    s->active = 1;
    nsec.total_assessments += assessments;
    nsec.n_security++;
    print_str("[NSEC] Security "); print_int(nsec.n_security - 1);
    print_str(" dom="); print_int(domain);
    print_str(" risk="); print_int(risk);
    print_str(" ass="); print_int(assessments);
    print_str(" wrn="); print_int(warnings);
    print_str(" rsp="); print_int(responses); print_str("\n");
    return nsec.n_security - 1;
}

int nsec_cyber(int domain, int threat_type, int detected, int blocked, int protected_cnt, int year) {
    if (nsec.n_cyber >= MAX_CYBER) return -1;
    cyber_t* cy = &cybers[nsec.n_cyber];
    cy->cyber_id = nsec.n_cyber;
    cy->cyber_domain = domain;
    cy->threat_type = threat_type;
    cy->incidents_detected = detected;
    cy->attacks_blocked = blocked;
    cy->systems_protected = protected_cnt;
    cy->year = year;
    cy->active = 1;
    nsec.total_incidents += detected;
    nsec.n_cyber++;
    print_str("[NSEC] Cyber "); print_int(nsec.n_cyber - 1);
    print_str(" dom="); print_int(domain);
    print_str(" type="); print_int(threat_type);
    print_str(" det="); print_int(detected);
    print_str(" blk="); print_int(blocked);
    print_str(" prt="); print_int(protected_cnt); print_str("\n");
    return nsec.n_cyber - 1;
}

int nsec_coordination(int coord_type, int partner, int meetings, int agreements, int joint_ops, int year) {
    if (nsec.n_coordination >= MAX_COORDINATION) return -1;
    coordination_t* co = &coordinations[nsec.n_coordination];
    co->coordination_id = nsec.n_coordination;
    co->coord_type = coord_type;
    co->partner_level = partner;
    co->meetings_held = meetings;
    co->agreements_signed = agreements;
    co->joint_operations = joint_ops;
    co->year = year;
    co->active = 1;
    nsec.total_operations += joint_ops;
    nsec.n_coordination++;
    print_str("[NSEC] Coordination "); print_int(nsec.n_coordination - 1);
    print_str(" type="); print_int(coord_type);
    print_str(" ptr="); print_int(partner);
    print_str(" mtg="); print_int(meetings);
    print_str(" agr="); print_int(agreements);
    print_str(" ops="); print_int(joint_ops); print_str("\n");
    return nsec.n_coordination - 1;
}

void nsec_intelligence_report(void) {
    print_str("[NSEC] Intelligence report:\n");
    print_str("  Intelligence sources: "); print_int(nsec.n_intelligence); print_str("\n");
    print_str("  Total intelligence gathered: "); print_int(nsec.total_intelligence); print_str("\n");
}

void nsec_counteresp_report(void) {
    print_str("[NSEC] Counter-espionage report:\n");
    print_str("  Counter-espionage units: "); print_int(nsec.n_counteresp); print_str("\n");
    print_str("  Total cases resolved: "); print_int(nsec.total_cases); print_str("\n");
}

void nsec_security_report(void) {
    print_str("[NSEC] Security report:\n");
    print_str("  Security domains: "); print_int(nsec.n_security); print_str("\n");
    print_str("  Total assessments: "); print_int(nsec.total_assessments); print_str("\n");
    print_str("  Cyber units: "); print_int(nsec.n_cyber); print_str("\n");
    print_str("  Total incidents detected: "); print_int(nsec.total_incidents); print_str("\n");
    print_str("  Coordination partners: "); print_int(nsec.n_coordination); print_str("\n");
    print_str("  Total joint operations: "); print_int(nsec.total_operations); print_str("\n");
}

void nsec_print_state(void) {
    print_str("[NSEC] In="); print_int(nsec.n_intelligence);
    print_str(" Ce="); print_int(nsec.n_counteresp);
    print_str(" Sc="); print_int(nsec.n_security);
    print_str(" Cy="); print_int(nsec.n_cyber);
    print_str(" Co="); print_int(nsec.n_coordination);
    print_str("\n");
}

int main(void) {
    print_str("=== National Security Demo ===\n\n");
    nsec_init();

    print_str("Intelligence gathering...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int src = (i % 4) + 1;
        int gth = 50 + (i * 15);
        int rpt = 20 + (i * 5);
        int thr = 5 + (i % 8);
        int year = 2020 + (i % 5);
        nsec_intelligence(type, src, gth, rpt, thr, year);
    }

    print_str("\nCounter-espionage...\n");
    for (int i = 0; i < 14; i++) {
        int ctype = (i % 4) + 1;
        int thr = (i % 5) + 1;
        int inv = 10 + (i * 3);
        int rsl = 8 + (i * 2);
        int prv = 5 + (i * 2);
        int year = 2021 + (i % 4);
        nsec_counteresp(ctype, thr, inv, rsl, prv, year);
    }

    print_str("\nNational security...\n");
    for (int i = 0; i < 12; i++) {
        int dom = (i % 5) + 1;
        int risk = (i % 4) + 1;
        int ass = 20 + (i * 5);
        int wrn = 8 + (i * 2);
        int rsp = 3 + (i % 5);
        int year = 2022 + (i % 3);
        nsec_security(dom, risk, ass, wrn, rsp, year);
    }

    print_str("\nCyber security...\n");
    for (int i = 0; i < 10; i++) {
        int dom = (i % 4) + 1;
        int type = (i % 5) + 1;
        int det = 100 + (i * 30);
        int blk = 80 + (i * 25);
        int prt = 50 + (i * 10);
        int year = 2023 + (i % 2);
        nsec_cyber(dom, type, det, blk, prt, year);
    }

    print_str("\nSecurity coordination...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int ptr = (i % 5) + 1;
        int mtg = 15 + (i * 3);
        int agr = 5 + (i * 2);
        int ops = 3 + (i % 4);
        int year = 2024;
        nsec_coordination(type, ptr, mtg, agr, ops, year);
    }

    print_str("\nIntelligence report...\n");
    nsec_intelligence_report();

    print_str("\nCounter-espionage report...\n");
    nsec_counteresp_report();

    print_str("\nSecurity report...\n");
    nsec_security_report();

    print_str("\nFinal state...\n");
    nsec_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
