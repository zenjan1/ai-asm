/* legal_compliance: Legal compliance system (v1.0)
 * Contract management, regulatory compliance, IP management, litigation, audit
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

#define MAX_CONTRACTS   12
#define MAX_REGULATIONS 8
#define MAX_IP_ASSETS   10
#define MAX_CASES       6
#define MAX_AUDITS      8
#define MAX_LOG_ENTRIES 20

typedef struct {
    int    contract_id;
    int    type;
    int    party_count;
    double value;
    int    start_day;
    int    end_day;
    int    status;
    int    approval_stage;
    int    active;
} contract_t;

typedef struct {
    int    reg_id;
    int    category;
    int    severity;
    int    compliance_score;
    int    last_check_day;
    int    next_check_day;
    int    violations;
    int    active;
} regulation_t;

typedef struct {
    int    ip_id;
    int    type;
    int    registration_day;
    int    expiry_day;
    double value;
    int    status;
    int    active;
} ip_asset_t;

typedef struct {
    int    case_id;
    int    type;
    int    priority;
    double amount_claimed;
    int    status;
    int    evidence_count;
    int    active;
} case_t;

typedef struct {
    int    audit_id;
    int    type;
    int    day;
    int    findings;
    int    resolved;
    int    severity;
    int    active;
} audit_t;

typedef struct {
    int    log_id;
    int    entity_type;
    int    entity_id;
    int    action;
    int    day;
    int    user_id;
    int    active;
} log_entry_t;

typedef struct {
    int    n_contracts;
    int    n_regulations;
    int    n_ip_assets;
    int    n_cases;
    int    n_audits;
    int    n_logs;
    double total_contract_value;
    double total_ip_value;
    int    total_violations;
    int    compliance_rate;
} lc_state_t;

static contract_t contracts[MAX_CONTRACTS];
static regulation_t regulations[MAX_REGULATIONS];
static ip_asset_t ip_assets[MAX_IP_ASSETS];
static case_t cases[MAX_CASES];
static audit_t audits[MAX_AUDITS];
static log_entry_t logs[MAX_LOG_ENTRIES];
static lc_state_t lc;

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

int lc_init(void) {
    if (initialized) return -1;
    lc.n_contracts = 0; lc.n_regulations = 0; lc.n_ip_assets = 0;
    lc.n_cases = 0; lc.n_audits = 0; lc.n_logs = 0;
    lc.total_contract_value = 0.0; lc.total_ip_value = 0.0;
    lc.total_violations = 0; lc.compliance_rate = 100;
    for (int i = 0; i < MAX_CONTRACTS; i++) contracts[i].active = 0;
    for (int i = 0; i < MAX_REGULATIONS; i++) regulations[i].active = 0;
    for (int i = 0; i < MAX_IP_ASSETS; i++) ip_assets[i].active = 0;
    for (int i = 0; i < MAX_CASES; i++) cases[i].active = 0;
    for (int i = 0; i < MAX_AUDITS; i++) audits[i].active = 0;
    for (int i = 0; i < MAX_LOG_ENTRIES; i++) logs[i].active = 0;
    initialized = 1;
    print_str("[LC] Legal compliance initialized\n");
    return 0;
}

static void log_action(int entity_type, int entity_id, int action, int day, int user_id) {
    if (lc.n_logs >= MAX_LOG_ENTRIES) return;
    log_entry_t* l = &logs[lc.n_logs];
    l->log_id = lc.n_logs;
    l->entity_type = entity_type;
    l->entity_id = entity_id;
    l->action = action;
    l->day = day;
    l->user_id = user_id;
    l->active = 1;
    lc.n_logs++;
}

int lc_create_contract(int type, int parties, double value, int duration_days) {
    if (lc.n_contracts >= MAX_CONTRACTS) return -1;
    contract_t* c = &contracts[lc.n_contracts];
    c->contract_id = lc.n_contracts;
    c->type = type;
    c->party_count = parties;
    c->value = value;
    c->start_day = 1;
    c->end_day = 1 + duration_days;
    c->status = 1;
    c->approval_stage = 1;
    c->active = 1;
    lc.n_contracts++;
    lc.total_contract_value += value;
    log_action(1, lc.n_contracts - 1, 1, 1, 0);
    print_str("[LC] Contract "); print_int(lc.n_contracts - 1);
    print_str(" type="); print_int(type);
    print_str(" parties="); print_int(parties);
    print_str(" value="); print_int((int)value);
    print_str(" days="); print_int(duration_days); print_str("\n");
    return lc.n_contracts - 1;
}

void lc_advance_approval(int contract_id) {
    if (contract_id >= lc.n_contracts) return;
    contract_t* c = &contracts[contract_id];
    if (c->approval_stage < 4) {
        c->approval_stage++;
        log_action(1, contract_id, 2, c->approval_stage, 0);
        print_str("[LC] Contract "); print_int(contract_id);
        print_str(" approval stage "); print_int(c->approval_stage); print_str("/4\n");
        if (c->approval_stage == 4) {
            c->status = 2;
            print_str("[LC] Contract "); print_int(contract_id); print_str(" FULLY APPROVED\n");
        }
    }
}

int lc_check_regulation(int category, int severity) {
    if (lc.n_regulations >= MAX_REGULATIONS) return -1;
    regulation_t* r = &regulations[lc.n_regulations];
    r->reg_id = lc.n_regulations;
    r->category = category;
    r->severity = severity;
    r->compliance_score = 85 + (lc.n_regulations * 3) % 15;
    r->last_check_day = 1;
    r->next_check_day = 30;
    r->violations = 0;
    r->active = 1;
    lc.n_regulations++;
    log_action(2, lc.n_regulations - 1, 1, 1, 0);
    print_str("[LC] Regulation "); print_int(lc.n_regulations - 1);
    print_str(" cat="); print_int(category);
    print_str(" sev="); print_int(severity);
    print_str(" score="); print_int(r->compliance_score); print_str("\n");
    return lc.n_regulations - 1;
}

void lc_record_violation(int reg_id) {
    if (reg_id >= lc.n_regulations) return;
    regulations[reg_id].violations++;
    lc.total_violations++;
    log_action(2, reg_id, 3, regulations[reg_id].last_check_day, 0);
    print_str("[LC] VIOLATION reg="); print_int(reg_id);
    print_str(" total="); print_int(regulations[reg_id].violations); print_str("\n");
}

int lc_register_ip(int type, int duration_years, double value) {
    if (lc.n_ip_assets >= MAX_IP_ASSETS) return -1;
    ip_asset_t* ip = &ip_assets[lc.n_ip_assets];
    ip->ip_id = lc.n_ip_assets;
    ip->type = type;
    ip->registration_day = 1;
    ip->expiry_day = 1 + duration_years * 365;
    ip->value = value;
    ip->status = 1;
    ip->active = 1;
    lc.n_ip_assets++;
    lc.total_ip_value += value;
    log_action(3, lc.n_ip_assets - 1, 1, 1, 0);
    print_str("[LC] IP "); print_int(lc.n_ip_assets - 1);
    print_str(" type="); print_int(type);
    print_str(" years="); print_int(duration_years);
    print_str(" value="); print_int((int)value); print_str("\n");
    return lc.n_ip_assets - 1;
}

int lc_open_case(int type, int priority, double amount) {
    if (lc.n_cases >= MAX_CASES) return -1;
    case_t* c = &cases[lc.n_cases];
    c->case_id = lc.n_cases;
    c->type = type;
    c->priority = priority;
    c->amount_claimed = amount;
    c->status = 1;
    c->evidence_count = 0;
    c->active = 1;
    lc.n_cases++;
    log_action(4, lc.n_cases - 1, 1, 1, 0);
    print_str("[LC] Case "); print_int(lc.n_cases - 1);
    print_str(" type="); print_int(type);
    print_str(" P"); print_int(priority);
    print_str(" amount="); print_int((int)amount); print_str("\n");
    return lc.n_cases - 1;
}

void lc_add_evidence(int case_id) {
    if (case_id >= lc.n_cases) return;
    cases[case_id].evidence_count++;
    log_action(4, case_id, 2, 1, 0);
    print_str("[LC] Case "); print_int(case_id);
    print_str(" +evidence (total: "); print_int(cases[case_id].evidence_count); print_str(")\n");
}

void lc_resolve_case(int case_id) {
    if (case_id >= lc.n_cases) return;
    cases[case_id].status = 3;
    log_action(4, case_id, 3, 1, 0);
    print_str("[LC] Case "); print_int(case_id); print_str(" RESOLVED\n");
}

int lc_audit(int type, int severity) {
    if (lc.n_audits >= MAX_AUDITS) return -1;
    audit_t* a = &audits[lc.n_audits];
    a->audit_id = lc.n_audits;
    a->type = type;
    a->day = 1;
    a->findings = 2 + (lc.n_audits * 3) % 5;
    a->resolved = 0;
    a->severity = severity;
    a->active = 1;
    lc.n_audits++;
    log_action(5, lc.n_audits - 1, 1, 1, 0);
    print_str("[LC] Audit "); print_int(lc.n_audits - 1);
    print_str(" type="); print_int(type);
    print_str(" sev="); print_int(severity);
    print_str(" findings="); print_int(a->findings); print_str("\n");
    return lc.n_audits - 1;
}

void lc_resolve_findings(int audit_id, int count) {
    if (audit_id >= lc.n_audits) return;
    audits[audit_id].resolved += count;
    log_action(5, audit_id, 2, 1, 0);
    print_str("[LC] Audit "); print_int(audit_id);
    print_str(" resolved "); print_int(count); print_str(" findings (total: ");
    print_int(audits[audit_id].resolved); print_str("/"); print_int(audits[audit_id].findings); print_str(")\n");
}

void lc_compliance_summary(void) {
    print_str("[LC] Compliance summary:\n");
    int total_score = 0;
    for (int i = 0; i < lc.n_regulations; i++) {
        total_score += regulations[i].compliance_score;
    }
    lc.compliance_rate = (lc.n_regulations > 0) ? total_score / lc.n_regulations : 100;
    print_str("  Regulations: "); print_int(lc.n_regulations); print_str("\n");
    print_str("  Compliance rate: "); print_int(lc.compliance_rate); print_str("%\n");
    print_str("  Total violations: "); print_int(lc.total_violations); print_str("\n");
    int active_contracts = 0;
    for (int i = 0; i < lc.n_contracts; i++) {
        if (contracts[i].status == 2) active_contracts++;
    }
    print_str("  Active contracts: "); print_int(active_contracts); print_str("\n");
    print_str("  Total contract value: "); print_int((int)lc.total_contract_value); print_str("\n");
    print_str("  IP assets: "); print_int(lc.n_ip_assets); print_str("\n");
    print_str("  IP value: "); print_int((int)lc.total_ip_value); print_str("\n");
    print_str("  Open cases: "); print_int(lc.n_cases); print_str("\n");
    print_str("  Audit log entries: "); print_int(lc.n_logs); print_str("\n");
}

void lc_print_state(void) {
    print_str("[LC] Contracts="); print_int(lc.n_contracts);
    print_str(" Regs="); print_int(lc.n_regulations);
    print_str(" IP="); print_int(lc.n_ip_assets);
    print_str(" Cases="); print_int(lc.n_cases);
    print_str(" Audits="); print_int(lc.n_audits);
    print_str(" Logs="); print_int(lc.n_logs);
    print_str("\n");
    print_str("  Compliance: "); print_int(lc.compliance_rate); print_str("%\n");
    print_str("  Violations: "); print_int(lc.total_violations); print_str("\n");
}

int main(void) {
    print_str("=== Legal Compliance System Demo ===\n\n");
    lc_init();

    print_str("Creating contracts...\n");
    lc_create_contract(1, 2, 100000.0, 365);
    lc_create_contract(2, 3, 250000.0, 730);
    lc_create_contract(1, 2, 75000.0, 180);
    lc_create_contract(3, 2, 500000.0, 1095);
    lc_create_contract(2, 4, 180000.0, 540);

    print_str("\nAdvancing contract approvals...\n");
    lc_advance_approval(0);
    lc_advance_approval(0);
    lc_advance_approval(0);
    lc_advance_approval(0);
    lc_advance_approval(1);
    lc_advance_approval(1);
    lc_advance_approval(2);
    lc_advance_approval(2);
    lc_advance_approval(2);
    lc_advance_approval(2);

    print_str("\nChecking regulations...\n");
    lc_check_regulation(1, 3);
    lc_check_regulation(2, 2);
    lc_check_regulation(3, 1);
    lc_check_regulation(4, 3);
    lc_check_regulation(5, 2);

    print_str("\nRecording violations...\n");
    lc_record_violation(1);
    lc_record_violation(3);
    lc_record_violation(3);

    print_str("\nRegistering IP assets...\n");
    lc_register_ip(1, 20, 500000.0);
    lc_register_ip(2, 10, 150000.0);
    lc_register_ip(3, 5, 80000.0);
    lc_register_ip(1, 15, 350000.0);
    lc_register_ip(2, 10, 120000.0);

    print_str("\nOpening cases...\n");
    lc_open_case(1, 2, 50000.0);
    lc_open_case(2, 3, 200000.0);
    lc_open_case(1, 1, 25000.0);

    print_str("\nAdding evidence...\n");
    lc_add_evidence(0);
    lc_add_evidence(0);
    lc_add_evidence(0);
    lc_add_evidence(1);
    lc_add_evidence(1);
    lc_add_evidence(1);
    lc_add_evidence(1);
    lc_add_evidence(2);

    print_str("\nResolving cases...\n");
    lc_resolve_case(0);
    lc_resolve_case(2);

    print_str("\nConducting audits...\n");
    lc_audit(1, 2);
    lc_audit(2, 3);
    lc_audit(1, 1);
    lc_audit(3, 2);

    print_str("\nResolving findings...\n");
    lc_resolve_findings(0, 2);
    lc_resolve_findings(0, 1);
    lc_resolve_findings(1, 3);
    lc_resolve_findings(2, 2);
    lc_resolve_findings(3, 1);

    print_str("\nCompliance summary...\n");
    lc_compliance_summary();

    print_str("\nFinal state...\n");
    lc_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
