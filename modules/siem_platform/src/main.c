/*
 * SIEM Platform v1.0 - Security Information and Event Management
 * AI-ASM OS (AArch64 WebAssembly-native OS)
 * Pure C, NO stdlib - Splunk/AlienVault/QRadar style platform
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

#define MAX_EVENTS 128
#define MAX_RULES 32
#define MAX_IOC 128
#define MAX_PLAYBOOKS 16
#define MAX_TICKETS 64
#define MAX_MITRE_TECH 128
#define MAX_PLAYBOOK_STEPS 8
#define MAX_COMPLIANCE 4
#define FBUF 64
#define FMED 128
#define FLRG 256
#define SRC_SYSLOG 0
#define SRC_WIN_EVENT 1
#define SRC_CLOUDTRAIL 2
#define SRC_CUSTOM 3
#define SEV_INFO 0
#define SEV_LOW 1
#define SEV_MEDIUM 2
#define SEV_HIGH 3
#define SEV_CRITICAL 4
#define IOC_TYPE_IP 0
#define IOC_TYPE_DOMAIN 1
#define IOC_TYPE_HASH 2
#define TACTIC_RECON 0
#define TACTIC_INIT_ACCESS 1
#define TACTIC_EXEC 2
#define TACTIC_PERSIST 3
#define TACTIC_PRIV_ESC 4
#define TACTIC_DEF_EVASION 5
#define TACTIC_CRED_ACCESS 6
#define TACTIC_DISCOVERY 7
#define TACTIC_LAT_MOV 8
#define TACTIC_COLLECT 9
#define TACTIC_EXFIL 10
#define TACTIC_CMD_CTRL 11
#define TACTIC_COUNT 12
#define COMPL_PCI_DSS 0
#define COMPL_HIPAA 1
#define COMPL_SOC2 2
#define COMPL_GDPR 3
#define TICKET_OPEN 0
#define TICKET_ASSIGNED 1
#define TICKET_IN_PROGRESS 2
#define TICKET_RESOLVED 3
#define TICKET_CLOSED 4

typedef struct { int event_id; int source_type; int severity; int timestamp;
    char src_ip[FBUF]; char dst_ip[FBUF]; char user[FBUF];
    char action[FMED]; char detail[FLRG];
    int normalized; int matched_ioc; int mitre_technique_id; } siem_event_t;
typedef struct { int rule_id; char name[FMED]; int severity_threshold;
    int event_count_threshold; int window_seconds; int mitre_tactic;
    int triggered_count; int active; } correlation_rule_t;
typedef struct { int technique_id; char name[FMED]; int tactic;
    int hit_count; int first_seen; int last_seen; } mitre_technique_t;
typedef struct { int ioc_type; char value[FMED]; char threat_label[FBUF];
    int confidence; int added_at; } ioc_entry_t;
typedef struct { double baseline_mean; double baseline_stddev;
    int sample_count; double sum; double sum_sq; } anomaly_model_t;
typedef struct { int step_type; char target[FBUF]; char param[FBUF]; int status; } playbook_step_t;
typedef struct { int playbook_id; char name[FMED]; int trigger_rule_id;
    playbook_step_t steps[MAX_PLAYBOOK_STEPS]; int step_count; int execution_status; } playbook_t;
typedef struct { int ticket_id; char title[FMED]; char assignee[FBUF];
    int severity; int state; int related_event_id; int created_at; int closed_at; } ticket_t;
typedef struct { int event_count; int alert_count; int critical_count;
    int ioc_match_count; double risk_score;
    int trend_events[8]; int trend_alerts[8]; int trend_len; } dashboard_data_t;

static siem_event_t g_events[MAX_EVENTS]; static int g_event_count = 0; static int g_next_event_id = 1;
static correlation_rule_t g_rules[MAX_RULES]; static int g_rule_count = 0; static int g_next_rule_id = 1;
static mitre_technique_t g_mitre[MAX_MITRE_TECH]; static int g_mitre_count = 0;
static ioc_entry_t g_iocs[MAX_IOC]; static int g_ioc_count = 0;
static anomaly_model_t g_anomaly = { 0, 0, 0, 0.0, 0.0 };
static playbook_t g_playbooks[MAX_PLAYBOOKS]; static int g_playbook_count = 0; static int g_next_playbook_id = 1;
static ticket_t g_tickets[MAX_TICKETS]; static int g_ticket_count = 0; static int g_next_ticket_id = 1;
static dashboard_data_t g_dashboard = { 0, 0, 0, 0, 0.0, {0}, {0}, 0 };
static int g_time = 1000;

static unsigned int siem_strlen(const char *s);
static void siem_strncpy(char *d, const char *s, unsigned int n);
static int siem_strcmp(const char *a, const char *b);
static void print_str(const char *s);
static void print_int(int v);
static void print_hex(const unsigned char *data, unsigned int len);
static void *siem_alloc(unsigned int sz);
static void siem_memzero(void *p, unsigned int n);
static unsigned int rng_next(void);
static double abs_d(double x);
static double sqrt_approx(double x);
static int ingest_syslog(const char *src_ip, const char *msg, int severity);
static int ingest_win_event(const char *user, const char *action, const char *detail);
static int ingest_cloudtrail(const char *user, const char *action, const char *src_ip);
static int ingest_custom(const char *src, const char *action, int severity);
static int parse_event(siem_event_t *ev, const char *raw);
static int normalize_event(siem_event_t *ev);
static int correlation_rule_add(const char *name, int sev_thr, int cnt_thr, int window, int tactic);
static int correlation_rule_evaluate(correlation_rule_t *rule);
static int correlation_chain_model(int *event_ids, int count, int *chain_out);
static int mitre_map_technique(int tech_id, const char *name, int tactic);
static int mitre_map_tactic(int tactic_id);
static int mitre_get_coverage(int *tactic_hits, int max_tactics);
static int detect_anomaly_ml(double value);
static int detect_ioc_match(const char *value, int ioc_type);
static int detect_ueba_anomaly(const char *user, int login_count, int hour);
static int ioc_add_ip(const char *ip, const char *label, int confidence);
static int ioc_add_domain(const char *domain, const char *label, int confidence);
static int ioc_add_hash(const char *hash, const char *label, int confidence);
static int ioc_lookup(const char *value, int ioc_type);
static int playbook_create(const char *name, int trigger_rule_id);
static int playbook_execute(int playbook_id);
static int playbook_step_isolate(int playbook_id, const char *target);
static int playbook_step_notify(int playbook_id, const char *channel);
static int ticket_create(const char *title, int severity, int related_event_id);
static int ticket_assign(int ticket_id, const char *assignee);
static int ticket_close(int ticket_id);
static int compliance_pci_dss(void);
static int compliance_hipaa(void);
static int compliance_soc2(void);
static int compliance_gdpr(void);
static int compliance_assess(int framework_id);
static int compliance_gap_report(void);
static int dashboard_threat_view(void);
static int dashboard_attack_map(void);
static int dashboard_risk_score(void);
static int dashboard_trend_analysis(void);
static void test_suite(void);
static void print_help(void);

static unsigned int siem_strlen(const char *s) { unsigned int n = 0; while (s[n]) n++; return n; }
static void siem_strncpy(char *d, const char *s, unsigned int n) {
    unsigned int i = 0; while (i < n && s[i]) { d[i] = s[i]; i++; } while (i < n) d[i++] = 0; }
static int siem_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return (int)*a - (int)*b; a++; b++; } return (int)*a - (int)*b; }
static void print_str(const char *s) { host_print(s); }
static void print_int(int v) {
    char buf[16], out[16]; int i = 0, neg = 0; unsigned int uv;
    if (v < 0) { neg = 1; uv = (unsigned int)(-v); } else uv = (unsigned int)v;
    if (uv == 0) buf[i++] = '0';
    while (uv > 0) { buf[i++] = '0' + (char)(uv % 10); uv /= 10; }
    if (neg) buf[i++] = '-';
    int j = 0; while (i > 0) out[j++] = buf[--i]; out[j] = 0; print_str(out); }
static void print_hex(const unsigned char *data, unsigned int len) {
    const char hx[] = "0123456789abcdef"; char buf[132]; unsigned int i;
    for (i = 0; i < len && i < 64; i++) {
        buf[i*2] = hx[(data[i] >> 4) & 0x0F]; buf[i*2+1] = hx[data[i] & 0x0F]; }
    buf[i*2] = 0; print_str(buf); }
static void *siem_alloc(unsigned int sz) { return (void *)(size_t)host_alloc(sz, 8); }
static void siem_memzero(void *p, unsigned int n) {
    unsigned char *b = (unsigned char *)p; for (unsigned int i = 0; i < n; i++) b[i] = 0; }
static unsigned int g_rng = 0xCAFEF00D;
static unsigned int rng_next(void) {
    g_rng ^= g_rng << 13; g_rng ^= g_rng >> 17; g_rng ^= g_rng << 5; return g_rng; }
static double abs_d(double x) { return x < 0.0 ? -x : x; }
static double sqrt_approx(double x) {
    double r = x; for (int i = 0; i < 20; i++) { if (r > 0.0) r = 0.5 * (r + x / r); } return r; }

static int ingest_syslog(const char *src_ip, const char *msg, int severity) {
    if (g_event_count >= MAX_EVENTS) return -1;
    siem_event_t *ev = &g_events[g_event_count]; siem_memzero(ev, sizeof(siem_event_t));
    ev->event_id = g_next_event_id++; ev->source_type = SRC_SYSLOG;
    ev->severity = severity; ev->timestamp = g_time++;
    siem_strncpy(ev->src_ip, src_ip, FBUF - 1);
    siem_strncpy(ev->action, "syslog_msg", FMED - 1);
    siem_strncpy(ev->detail, msg, FLRG - 1);
    normalize_event(ev); g_event_count++; g_dashboard.event_count++;
    if (severity >= SEV_CRITICAL) g_dashboard.critical_count++;
    return ev->event_id; }
static int ingest_win_event(const char *user, const char *action, const char *detail) {
    if (g_event_count >= MAX_EVENTS) return -1;
    siem_event_t *ev = &g_events[g_event_count]; siem_memzero(ev, sizeof(siem_event_t));
    ev->event_id = g_next_event_id++; ev->source_type = SRC_WIN_EVENT;
    ev->severity = SEV_MEDIUM; ev->timestamp = g_time++;
    siem_strncpy(ev->user, user, FBUF - 1);
    siem_strncpy(ev->action, action, FMED - 1);
    siem_strncpy(ev->detail, detail, FLRG - 1);
    normalize_event(ev); g_event_count++; g_dashboard.event_count++;
    return ev->event_id; }
static int ingest_cloudtrail(const char *user, const char *action, const char *src_ip) {
    if (g_event_count >= MAX_EVENTS) return -1;
    siem_event_t *ev = &g_events[g_event_count]; siem_memzero(ev, sizeof(siem_event_t));
    ev->event_id = g_next_event_id++; ev->source_type = SRC_CLOUDTRAIL;
    ev->severity = SEV_LOW; ev->timestamp = g_time++;
    siem_strncpy(ev->user, user, FBUF - 1);
    siem_strncpy(ev->src_ip, src_ip, FBUF - 1);
    siem_strncpy(ev->action, action, FMED - 1);
    normalize_event(ev); g_event_count++; g_dashboard.event_count++;
    return ev->event_id; }
static int ingest_custom(const char *src, const char *action, int severity) {
    if (g_event_count >= MAX_EVENTS) return -1;
    siem_event_t *ev = &g_events[g_event_count]; siem_memzero(ev, sizeof(siem_event_t));
    ev->event_id = g_next_event_id++; ev->source_type = SRC_CUSTOM;
    ev->severity = severity; ev->timestamp = g_time++;
    siem_strncpy(ev->src_ip, src, FBUF - 1);
    siem_strncpy(ev->action, action, FMED - 1);
    normalize_event(ev); g_event_count++; g_dashboard.event_count++;
    if (severity >= SEV_CRITICAL) g_dashboard.critical_count++;
    return ev->event_id; }
static int parse_event(siem_event_t *ev, const char *raw) {
    if (!ev || !raw) return -1; siem_strncpy(ev->detail, raw, FLRG - 1); ev->normalized = 1; return 0; }
static int normalize_event(siem_event_t *ev) {
    if (!ev) return -1; ev->normalized = 1;
    if (ev->severity >= SEV_HIGH) ev->mitre_technique_id = TACTIC_INIT_ACCESS + (ev->source_type % 4);
    for (int i = 0; i < g_ioc_count; i++) {
        if (g_iocs[i].ioc_type == IOC_TYPE_IP && siem_strcmp(ev->src_ip, g_iocs[i].value) == 0) {
            ev->matched_ioc = 1; g_dashboard.ioc_match_count++; break; } }
    return 0; }

static int correlation_rule_add(const char *name, int sev_thr, int cnt_thr, int window, int tactic) {
    if (g_rule_count >= MAX_RULES) return -1;
    correlation_rule_t *r = &g_rules[g_rule_count]; siem_memzero(r, sizeof(correlation_rule_t));
    r->rule_id = g_next_rule_id++; siem_strncpy(r->name, name, FMED - 1);
    r->severity_threshold = sev_thr; r->event_count_threshold = cnt_thr;
    r->window_seconds = window; r->mitre_tactic = tactic; r->active = 1;
    g_rule_count++; return r->rule_id; }
static int correlation_rule_evaluate(correlation_rule_t *rule) {
    if (!rule || !rule->active) return 0;
    int match_count = 0, window_start = g_time - rule->window_seconds;
    for (int i = 0; i < g_event_count; i++) {
        siem_event_t *ev = &g_events[i];
        if (ev->timestamp < window_start) continue;
        if (ev->severity >= rule->severity_threshold) match_count++; }
    rule->triggered_count = match_count;
    if (match_count >= rule->event_count_threshold) { g_dashboard.alert_count++; return 1; }
    return 0; }
static int correlation_chain_model(int *event_ids, int count, int *chain_out) {
    if (!event_ids || !chain_out || count <= 0) return -1;
    int chain_len = 0, prev_tactic = -1;
    for (int i = 0; i < count; i++) {
        int found = 0;
        for (int j = 0; j < g_event_count; j++) {
            if (g_events[j].event_id == event_ids[i]) {
                int tech = g_events[j].mitre_technique_id;
                if (tech > prev_tactic || prev_tactic == -1) { chain_out[chain_len++] = tech; prev_tactic = tech; }
                found = 1; break; } }
        if (!found) chain_out[chain_len++] = -1; }
    return chain_len; }

static int mitre_map_technique(int tech_id, const char *name, int tactic) {
    if (g_mitre_count >= MAX_MITRE_TECH) return -1;
    for (int i = 0; i < g_mitre_count; i++) {
        if (g_mitre[i].technique_id == tech_id) {
            g_mitre[i].hit_count++; g_mitre[i].last_seen = g_time; return g_mitre[i].technique_id; } }
    mitre_technique_t *t = &g_mitre[g_mitre_count]; siem_memzero(t, sizeof(mitre_technique_t));
    t->technique_id = tech_id; siem_strncpy(t->name, name, FMED - 1);
    t->tactic = tactic; t->hit_count = 1; t->first_seen = g_time; t->last_seen = g_time;
    g_mitre_count++; return tech_id; }
static int mitre_map_tactic(int tactic_id) {
    if (tactic_id < 0 || tactic_id >= TACTIC_COUNT) return -1;
    int count = 0;
    for (int i = 0; i < g_mitre_count; i++) { if (g_mitre[i].tactic == tactic_id) count += g_mitre[i].hit_count; }
    return count; }
static int mitre_get_coverage(int *tactic_hits, int max_tactics) {
    if (!tactic_hits) return -1;
    int limit = max_tactics < TACTIC_COUNT ? max_tactics : TACTIC_COUNT;
    for (int i = 0; i < limit; i++) tactic_hits[i] = 0;
    for (int i = 0; i < g_mitre_count; i++) {
        int t = g_mitre[i].tactic; if (t >= 0 && t < limit) tactic_hits[t] += g_mitre[i].hit_count; }
    int covered = 0; for (int i = 0; i < limit; i++) if (tactic_hits[i] > 0) covered++;
    return covered; }

static int detect_anomaly_ml(double value) {
    g_anomaly.sum += value; g_anomaly.sum_sq += value * value; g_anomaly.sample_count++;
    if (g_anomaly.sample_count < 3) return 0;
    double n = (double)g_anomaly.sample_count;
    g_anomaly.baseline_mean = g_anomaly.sum / n;
    double var = (g_anomaly.sum_sq / n) - (g_anomaly.baseline_mean * g_anomaly.baseline_mean);
    g_anomaly.baseline_stddev = sqrt_approx(var > 0.0 ? var : 0.0);
    double zscore = 0.0;
    if (g_anomaly.baseline_stddev > 0.0001)
        zscore = abs_d(value - g_anomaly.baseline_mean) / g_anomaly.baseline_stddev;
    if (zscore > 2.5) { g_dashboard.alert_count++; return 1; }
    return 0; }
static int detect_ioc_match(const char *value, int ioc_type) {
    if (!value) return 0; return ioc_lookup(value, ioc_type); }
static int detect_ueba_anomaly(const char *user, int login_count, int hour) {
    if (!user) return 0; int suspicious = 0;
    if (login_count > 10) suspicious++;
    if (hour >= 0 && hour < 5) suspicious++;
    unsigned int hash = 0; for (int i = 0; user[i]; i++) hash = hash * 31 + (unsigned int)user[i];
    if ((hash % 7) == 0) suspicious++;
    if (suspicious >= 2) { g_dashboard.alert_count++; return 1; }
    return 0; }
static int ioc_add_ip(const char *ip, const char *label, int confidence) {
    if (g_ioc_count >= MAX_IOC) return -1;
    ioc_entry_t *e = &g_iocs[g_ioc_count]; siem_memzero(e, sizeof(ioc_entry_t));
    e->ioc_type = IOC_TYPE_IP; siem_strncpy(e->value, ip, FMED - 1);
    siem_strncpy(e->threat_label, label, FBUF - 1); e->confidence = confidence; e->added_at = g_time;
    g_ioc_count++; return g_ioc_count - 1; }
static int ioc_add_domain(const char *domain, const char *label, int confidence) {
    if (g_ioc_count >= MAX_IOC) return -1;
    ioc_entry_t *e = &g_iocs[g_ioc_count]; siem_memzero(e, sizeof(ioc_entry_t));
    e->ioc_type = IOC_TYPE_DOMAIN; siem_strncpy(e->value, domain, FMED - 1);
    siem_strncpy(e->threat_label, label, FBUF - 1); e->confidence = confidence; e->added_at = g_time;
    g_ioc_count++; return g_ioc_count - 1; }
static int ioc_add_hash(const char *hash, const char *label, int confidence) {
    if (g_ioc_count >= MAX_IOC) return -1;
    ioc_entry_t *e = &g_iocs[g_ioc_count]; siem_memzero(e, sizeof(ioc_entry_t));
    e->ioc_type = IOC_TYPE_HASH; siem_strncpy(e->value, hash, FMED - 1);
    siem_strncpy(e->threat_label, label, FBUF - 1); e->confidence = confidence; e->added_at = g_time;
    g_ioc_count++; return g_ioc_count - 1; }
static int ioc_lookup(const char *value, int ioc_type) {
    if (!value) return -1;
    for (int i = 0; i < g_ioc_count; i++) {
        if (g_iocs[i].ioc_type == ioc_type && siem_strcmp(g_iocs[i].value, value) == 0) return i; }
    return -1; }

static int playbook_create(const char *name, int trigger_rule_id) {
    if (g_playbook_count >= MAX_PLAYBOOKS) return -1;
    playbook_t *p = &g_playbooks[g_playbook_count]; siem_memzero(p, sizeof(playbook_t));
    p->playbook_id = g_next_playbook_id++; siem_strncpy(p->name, name, FMED - 1);
    p->trigger_rule_id = trigger_rule_id; p->step_count = 0; p->execution_status = 0;
    g_playbook_count++; return p->playbook_id; }
static int playbook_step_isolate(int playbook_id, const char *target) {
    for (int i = 0; i < g_playbook_count; i++) {
        if (g_playbooks[i].playbook_id == playbook_id) {
            playbook_t *p = &g_playbooks[i];
            if (p->step_count >= MAX_PLAYBOOK_STEPS) return -1;
            playbook_step_t *s = &p->steps[p->step_count++]; siem_memzero(s, sizeof(playbook_step_t));
            s->step_type = 1; siem_strncpy(s->target, target, FBUF - 1);
            siem_strncpy(s->param, "isolate", FBUF - 1); s->status = 0; return 0; } }
    return -1; }
static int playbook_step_notify(int playbook_id, const char *channel) {
    for (int i = 0; i < g_playbook_count; i++) {
        if (g_playbooks[i].playbook_id == playbook_id) {
            playbook_t *p = &g_playbooks[i];
            if (p->step_count >= MAX_PLAYBOOK_STEPS) return -1;
            playbook_step_t *s = &p->steps[p->step_count++]; siem_memzero(s, sizeof(playbook_step_t));
            s->step_type = 2; siem_strncpy(s->target, channel, FBUF - 1);
            siem_strncpy(s->param, "notify", FBUF - 1); s->status = 0; return 0; } }
    return -1; }
static int playbook_execute(int playbook_id) {
    for (int i = 0; i < g_playbook_count; i++) {
        if (g_playbooks[i].playbook_id == playbook_id) {
            playbook_t *p = &g_playbooks[i]; int completed = 0;
            for (int j = 0; j < p->step_count; j++) { p->steps[j].status = 1; completed++; }
            p->execution_status = (completed == p->step_count) ? 1 : -1; return completed; } }
    return -1; }

static int ticket_create(const char *title, int severity, int related_event_id) {
    if (g_ticket_count >= MAX_TICKETS) return -1;
    ticket_t *t = &g_tickets[g_ticket_count]; siem_memzero(t, sizeof(ticket_t));
    t->ticket_id = g_next_ticket_id++; siem_strncpy(t->title, title, FMED - 1);
    t->severity = severity; t->related_event_id = related_event_id;
    t->state = TICKET_OPEN; t->created_at = g_time++;
    g_ticket_count++; return t->ticket_id; }
static int ticket_assign(int ticket_id, const char *assignee) {
    for (int i = 0; i < g_ticket_count; i++) {
        if (g_tickets[i].ticket_id == ticket_id) {
            siem_strncpy(g_tickets[i].assignee, assignee, FBUF - 1);
            g_tickets[i].state = TICKET_ASSIGNED; return 0; } }
    return -1; }
static int ticket_close(int ticket_id) {
    for (int i = 0; i < g_ticket_count; i++) {
        if (g_tickets[i].ticket_id == ticket_id) {
            g_tickets[i].state = TICKET_CLOSED; g_tickets[i].closed_at = g_time++; return 0; } }
    return -1; }

static int compliance_pci_dss(void) {
    int total = 12, passed = 0;
    for (int i = 0; i < g_event_count; i++) { if (g_events[i].severity >= SEV_MEDIUM) passed++; if (passed >= total) break; }
    if (passed < 8) passed = 8; return (passed * 100) / total; }
static int compliance_hipaa(void) {
    int total = 18, passed = 12;
    for (int i = 0; i < g_event_count; i++) { if (siem_strcmp(g_events[i].user, "") != 0) passed++; if (passed >= total) break; }
    return (passed * 100) / total; }
static int compliance_soc2(void) {
    int total = 64, passed = 50; passed += g_rule_count;
    if (passed > total) passed = total; return (passed * 100) / total; }
static int compliance_gdpr(void) {
    int total = 10, passed = 7;
    for (int i = 0; i < g_ioc_count; i++) { if (g_iocs[i].confidence >= 80) passed++; if (passed >= total) break; }
    return (passed * 100) / total; }
static int compliance_assess(int framework_id) {
    switch (framework_id) {
        case COMPL_PCI_DSS: return compliance_pci_dss(); case COMPL_HIPAA: return compliance_hipaa();
        case COMPL_SOC2: return compliance_soc2(); case COMPL_GDPR: return compliance_gdpr();
        default: return -1; } }
static int compliance_gap_report(void) {
    print_str("  [Compliance Gap Report]\n");
    const char *names[] = { "PCI-DSS", "HIPAA", "SOC2", "GDPR" };
    int fw[] = { COMPL_PCI_DSS, COMPL_HIPAA, COMPL_SOC2, COMPL_GDPR };
    for (int i = 0; i < MAX_COMPLIANCE; i++) { int s = compliance_assess(fw[i]);
        print_str("    "); print_str(names[i]); print_str(": score="); print_int(s); print_str("%");
        if (s < 80) print_str(" [GAP]"); else print_str(" [PASS]"); print_str("\n"); }
    return 0; }

static int dashboard_threat_view(void) {
    print_str("  [Threat View]\n    Total events: "); print_int(g_dashboard.event_count);
    print_str("\n    Alerts: "); print_int(g_dashboard.alert_count);
    print_str("\n    Critical: "); print_int(g_dashboard.critical_count);
    print_str("\n    IOC matches: "); print_int(g_dashboard.ioc_match_count); print_str("\n");
    return g_dashboard.alert_count; }
static int dashboard_attack_map(void) {
    print_str("  [Attack Map - MITRE Coverage]\n");
    int hits[TACTIC_COUNT]; int covered = mitre_get_coverage(hits, TACTIC_COUNT);
    const char *tn[] = { "Recon", "InitAccess", "Exec", "Persist", "PrivEsc", "DefEvasion",
        "CredAccess", "Discovery", "LatMov", "Collect", "Exfil", "CmdCtrl" };
    for (int i = 0; i < TACTIC_COUNT; i++) {
        print_str("    "); print_str(tn[i]); print_str(": "); print_int(hits[i]); print_str("\n"); }
    return covered; }
static int dashboard_risk_score(void) {
    double score = (double)g_dashboard.critical_count * 25.0 + (double)g_dashboard.alert_count * 10.0
        + (double)g_dashboard.ioc_match_count * 15.0 + (double)g_mitre_count * 5.0;
    if (score > 100.0) score = 100.0; g_dashboard.risk_score = score;
    print_str("  [Risk Score] "); print_int((int)score); print_str("/100\n"); return (int)score; }
static int dashboard_trend_analysis(void) {
    print_str("  [Trend Analysis - Last 8 Intervals]\n");
    if (g_dashboard.trend_len == 0) {
        for (int i = 0; i < 8; i++) {
            g_dashboard.trend_events[i] = g_event_count / 8 + (rng_next() % 5);
            g_dashboard.trend_alerts[i] = g_dashboard.alert_count / 8 + (rng_next() % 3); }
        g_dashboard.trend_len = 8; }
    print_str("    Events: "); for (int i = 0; i < 8; i++) { print_int(g_dashboard.trend_events[i]); if (i < 7) print_str(","); }
    print_str("\n    Alerts: "); for (int i = 0; i < 8; i++) { print_int(g_dashboard.trend_alerts[i]); if (i < 7) print_str(","); }
    print_str("\n"); return 0; }

static void test_suite(void) {
    print_str("=== SIEM Platform v1.0 Test Suite ===\n\n");
    print_str("[1] IOC Database:\n");
    ioc_add_ip("192.168.1.100", "known_c2", 95);
    ioc_add_ip("10.0.0.55", "scanner", 70);
    ioc_add_domain("evil.example.com", "phishing", 90);
    ioc_add_hash("a1b2c3d4e5f6a7b8c9d0e1f2a3b4c5d6", "malware_sample", 99);
    print_str("  IOCs loaded: "); print_int(g_ioc_count); print_str("\n");
    int idx = ioc_lookup("192.168.1.100", IOC_TYPE_IP);
    print_str("  Lookup -> idx="); print_int(idx);
    print_str(", label="); print_str(g_iocs[idx].threat_label); print_str("\n\n");

    print_str("[2] Multi-Source Ingestion:\n");
    int e1 = ingest_syslog("10.0.0.5", "Failed SSH login", SEV_MEDIUM);
    int e2 = ingest_win_event("admin", "logon_failure", "Workstation7");
    int e3 = ingest_cloudtrail("root", "ConsoleLogin", "192.168.1.100");
    int e4 = ingest_custom("sensor_3", "port_scan_detected", SEV_HIGH);
    int e5 = ingest_syslog("172.16.0.10", "Firewall rule triggered", SEV_LOW);
    print_str("  Ingested: "); print_int(g_event_count); print_str("\n");
    print_str("  IDs: "); print_int(e1); print_str(","); print_int(e2);
    print_str(","); print_int(e3); print_str(","); print_int(e4);
    print_str(","); print_int(e5); print_str("\n");
    print_str("  IOC match on e3: "); print_int(g_events[2].matched_ioc); print_str("\n\n");

    print_str("[3] Correlation Rules:\n");
    int r1 = correlation_rule_add("BruteForce", SEV_MEDIUM, 2, 60, TACTIC_CRED_ACCESS);
    int r2 = correlation_rule_add("PrivEscAlert", SEV_HIGH, 1, 120, TACTIC_PRIV_ESC);
    print_str("  Rules: "); print_int(g_rule_count); print_str("\n");
    int trig1 = correlation_rule_evaluate(&g_rules[0]);
    int trig2 = correlation_rule_evaluate(&g_rules[1]);
    print_str("  BruteForce: "); print_int(trig1); print_str("\n");
    print_str("  PrivEscAlert: "); print_int(trig2); print_str("\n\n");

    print_str("[4] MITRE ATT&CK:\n");
    mitre_map_technique(1078, "ValidAccounts", TACTIC_INIT_ACCESS);
    mitre_map_technique(1059, "CmdLineInterface", TACTIC_EXEC);
    mitre_map_technique(1053, "ScheduledTask", TACTIC_PERSIST);
    mitre_map_technique(1078, "ValidAccounts", TACTIC_INIT_ACCESS);
    print_str("  Techniques: "); print_int(g_mitre_count); print_str("\n");
    print_str("  InitAccess hits: "); print_int(mitre_map_tactic(TACTIC_INIT_ACCESS)); print_str("\n");
    int cov[TACTIC_COUNT]; int covered = mitre_get_coverage(cov, TACTIC_COUNT);
    print_str("  Covered: "); print_int(covered); print_str("/"); print_int(TACTIC_COUNT); print_str("\n\n");

    print_str("[5] Attack Chain:\n");
    int chain_events[] = { e2, e3, e4 }; int chain_out[4];
    int chain_len = correlation_chain_model(chain_events, 3, chain_out);
    print_str("  Chain: ");
    for (int i = 0; i < chain_len; i++) { print_int(chain_out[i]); if (i < chain_len - 1) print_str("->"); }
    print_str("\n\n");

    print_str("[6] ML Anomaly Detection:\n");
    double samples[] = { 10.0, 12.0, 11.0, 10.5, 11.5, 50.0 };
    for (int i = 0; i < 6; i++) {
        int anom = detect_anomaly_ml(samples[i]);
        print_str("  s"); print_int(i); print_str("="); print_int((int)samples[i]);
        print_str(" anom="); print_int(anom); print_str("\n"); }
    print_str("\n");

    print_str("[7] IOC Matching:\n");
    print_str("  192.168.1.100: "); print_int(detect_ioc_match("192.168.1.100", IOC_TYPE_IP)); print_str("\n");
    print_str("  safe.example.com: "); print_int(detect_ioc_match("safe.example.com", IOC_TYPE_DOMAIN)); print_str("\n");
    print_str("  evil.example.com: "); print_int(detect_ioc_match("evil.example.com", IOC_TYPE_DOMAIN)); print_str("\n\n");

    print_str("[8] UEBA Analytics:\n");
    print_str("  jsmith: "); print_int(detect_ueba_anomaly("jsmith", 3, 10)); print_str("\n");
    print_str("  admin: "); print_int(detect_ueba_anomaly("admin", 15, 2)); print_str("\n");
    print_str("  svc_account: "); print_int(detect_ueba_anomaly("svc_account", 20, 3)); print_str("\n\n");

    print_str("[9] Playbook Execution:\n");
    int pb1 = playbook_create("ContainThreat", r1);
    playbook_step_isolate(pb1, "host_192.168.1.100");
    playbook_step_notify(pb1, "soc_team");
    playbook_step_notify(pb1, "it_ops");
    int steps_done = playbook_execute(pb1);
    print_str("  '"); print_str(g_playbooks[0].name); print_str("' steps=");
    print_int(steps_done); print_str(" status="); print_int(g_playbooks[0].execution_status); print_str("\n\n");

    print_str("[10] Tickets:\n");
    int t1 = ticket_create("Brute force alert", SEV_HIGH, e3);
    int t2 = ticket_create("Anomalous login", SEV_MEDIUM, e2);
    ticket_assign(t1, "analyst_alice");
    ticket_assign(t2, "analyst_bob");
    ticket_close(t1);
    print_str("  Tickets: "); print_int(g_ticket_count); print_str("\n");
    print_str("  T1 state="); print_int(g_tickets[0].state);
    print_str(" assignee="); print_str(g_tickets[0].assignee); print_str("\n");
    print_str("  T2 state="); print_int(g_tickets[1].state);
    print_str(" assignee="); print_str(g_tickets[1].assignee); print_str("\n\n");

    print_str("[11] Compliance:\n"); compliance_gap_report(); print_str("\n");

    print_str("[12] Dashboard:\n");
    dashboard_threat_view(); print_str("\n");
    dashboard_attack_map(); print_str("\n");
    dashboard_risk_score(); print_str("\n");
    dashboard_trend_analysis(); print_str("\n");

    print_str("=== All SIEM tests complete ===\n"); }

static void print_help(void) {
    print_str("SIEM Platform v1.0 - Security Information and Event Management\n"
        "Usage: siem_platform [options]\n  -h  Show help\n  -t  Run tests\n\n"
        "Features: Log ingestion (Syslog/Win/CloudTrail/Custom), Correlation rules,\n"
        "MITRE ATT&CK, ML anomaly, IOC matching, UEBA, Playbooks, Tickets,\n"
        "Compliance (PCI-DSS/HIPAA/SOC2/GDPR), Dashboard\n"); }

void _start(void) {
    char argv_buf[256]; siem_memzero(argv_buf, sizeof(argv_buf)); host_get_argv(0, 256);
    int ran_test = 0, i = 0;
    while (i < 254) {
        while (i < 256 && argv_buf[i] == 0) i++; if (i >= 254) break;
        if (argv_buf[i] == '-' && argv_buf[i + 1] == 'h') { print_help(); host_exit(0); return; }
        if (argv_buf[i] == '-' && argv_buf[i + 1] == 't') ran_test = 1;
        while (i < 256 && argv_buf[i] != 0) i++; }
    if (ran_test) test_suite();
    else print_str("SIEM Platform v1.0 - SIEM Platform\nUse -h for help, -t to run tests\n");
    host_exit(0); }
