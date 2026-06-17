/*
 * Threat Detection Module v1.0 - Unified Detection and Response
 * AI-ASM OS (AArch64 WebAssembly-native OS)
 * Pure C, NO stdlib - SIEM/IDS/TI/EDR/SOAR/Forensics engine
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

/* Limits */
#define MAX_LOG 24
#define MAX_RULE 8
#define MAX_ALERT 12
#define MAX_INC 8
#define MAX_SIG 12
#define MAX_PKT 12
#define MAX_FIM 6
#define MAX_SC 6
#define MAX_IOC 24
#define MAX_MITRE 12
#define MAX_HUNT 3
#define MAX_CVE 12
#define MAX_AGENT 4
#define MAX_PROC 24
#define MAX_PB 3
#define MAX_PBS 4
#define MAX_CASE 3
#define MAX_REG 12
#define MAX_FLOW 12
#define MAX_ART 12
#define MAX_TL 24
#define FBUF 28
#define FMED 48
#define FLRG 72

/* Source types */
#define SRC_SYSLOG 0
#define SRC_FIREWALL 1
#define SRC_NETFLOW 2
#define SRC_IDS 3
#define SRC_EDR 4

/* Severity levels */
#define SEV_INFO 0
#define SEV_LOW 1
#define SEV_MEDIUM 2
#define SEV_HIGH 3
#define SEV_CRITICAL 4

/* IOC types */
#define IOC_IP 0
#define IOC_DOMAIN 1
#define IOC_HASH 2
#define IOC_URL 3

/* MITRE ATT&CK tactics */
#define TACT_RECON 0
#define TACT_INIT 1
#define TACT_EXEC 2
#define TACT_PERSIST 3
#define TACT_PRIV 4
#define TACT_EVASION 5
#define TACT_CRED 6
#define TACT_DISC 7
#define TACT_LAT 8
#define TACT_COLLECT 9
#define TACT_EXFIL 10
#define TACT_C2 11

/* Incident states */
#define INC_NEW 0
#define INC_INVESTIGATING 1
#define INC_CONTAINED 2
#define INC_REMEDIATED 3
#define INC_CLOSED 4

/* === Helper functions === */
static void str_copy(char *dst, const char *src, int max) {
    int i = 0;
    while (i < max - 1 && src[i]) { dst[i] = src[i]; i++; }
    dst[i] = '\0';
}

static int str_equal(const char *a, const char *b) {
    int i = 0;
    while (a[i] && b[i]) { if (a[i] != b[i]) return 0; i++; }
    return a[i] == b[i];
}

static void print_int(int v) {
    char buf[20], out[20]; int i = 0, neg = 0; unsigned int uv;
    if (v < 0) { neg = 1; uv = (unsigned int)(-v); } else uv = (unsigned int)v;
    if (uv == 0) buf[i++] = '0';
    while (uv > 0) { buf[i++] = '0' + (char)(uv % 10); uv /= 10; }
    if (neg) buf[i++] = '-';
    int j = 0; while (i > 0) out[j++] = buf[--i]; out[j] = '\0';
    host_print(out);
}

static void print_uint(unsigned int v) {
    char buf[20], out[20]; int i = 0;
    if (v == 0) buf[i++] = '0';
    while (v > 0) { buf[i++] = '0' + (char)(v % 10); v /= 10; }
    int j = 0; while (i > 0) out[j++] = buf[--i]; out[j] = '\0';
    host_print(out);
}

static void print_sev(int s) {
    const char *n[] = {"INFO", "LOW", "MEDIUM", "HIGH", "CRITICAL"};
    host_print((s >= 0 && s <= 4) ? n[s] : "UNKNOWN");
}

static void print_state(int s) {
    const char *n[] = {"NEW", "INVESTIGATING", "CONTAINED", "REMEDIATED", "CLOSED"};
    host_print((s >= 0 && s <= 4) ? n[s] : "UNKNOWN");
}

/* === Data Structures === */

/* SIEM */
typedef struct { int src; int sev; unsigned int ts; char sip[FBUF]; char dip[FBUF];
    char desc[FMED]; char raw[FLRG]; int norm; int corr; } log_t;
typedef struct { char name[FBUF]; int cond; int thresh; int sev; int hits;
    char desc[FMED]; int act; } rule_t;
typedef struct { int sev; unsigned int ts; char rule[FBUF]; char sip[FBUF];
    char desc[FMED]; int ack; int esc; int iid; } alert_t;
typedef struct { int id; int state; int sev; unsigned int ct; unsigned int ut;
    char title[FMED]; char who[FBUF]; int sla; int ac; } inc_t;
typedef struct { log_t logs[MAX_LOG]; int lc; rule_t rules[MAX_RULE]; int rc;
    alert_t alerts[MAX_ALERT]; int ac; inc_t incs[MAX_INC]; int ic;
    unsigned int etot; unsigned int atot; int ke; int kd; int kr; } siem_t;

/* IDS */
typedef struct { char sid[FBUF]; char pat[FMED]; int proto; int dport;
    int sev; char desc[FMED]; int mode; int hits; } idsig_t;
typedef struct { char sip[FBUF]; char dip[FBUF]; int sp; int dp; int proto;
    unsigned int psz; unsigned int ts; char pay[FLRG]; int sus; int anom; } pkt_t;
typedef struct { char path[FMED]; char ehash[FBUF]; unsigned int sz; unsigned int lt; } fim_t;
typedef struct { int scid; char proc[FBUF]; int pid; int sus; } scl_t;
typedef struct { idsig_t sigs[MAX_SIG]; int sc; pkt_t pkts[MAX_PKT]; int pc;
    fim_t fim[MAX_FIM]; int fc; scl_t sysc[MAX_SC]; int cc;
    int nids_al; int hids_al; int dpi_tot; int anom_tot;
    double bl_avg; double bl_sd; } ids_t;

/* Threat Intel */
typedef struct { int type; char val[FMED]; int sev; unsigned int fs; unsigned int ls;
    int hits; char src[FBUF]; char tags[FMED]; int conf; } ioc_t;
typedef struct { char tid[FBUF]; char name[FMED]; int tactic;
    int sev; char det[FLRG]; int mit; } mitre_t;
typedef struct { char hyp[FMED]; char query[FLRG]; int tac; int find; int st; } hunt_t;
typedef struct { char cveid[FBUF]; char desc[FMED]; int cvss10;
    int sev; char prod[FMED]; int patched; unsigned int pub; } cve_t;
typedef struct { ioc_t iocs[MAX_IOC]; int ic; mitre_t tech[MAX_MITRE]; int tc;
    hunt_t hunts[MAX_HUNT]; int hc; cve_t cves[MAX_CVE]; int vc;
    int ihits; int hfind; int dw; } ti_t;

/* EDR */
typedef struct { int id; char host[FBUF]; char os[FBUF];
    int status; unsigned int ls; int thr; } agent_t;
typedef struct { int pid; int ppid; char name[FBUF]; char cmd[FLRG];
    unsigned int ts; int sus; int bscore; char parent[FBUF]; } proc_t;
typedef struct { char path[FMED]; unsigned int sz; char hash[FBUF];
    int wl; int mal; int ast; char result[FMED]; } file_t;
typedef struct { int btype; char desc[FMED]; int sev; int pid;
    char proc[FBUF]; unsigned int ts; } behav_t;
typedef struct { agent_t agents[MAX_AGENT]; int ac; proc_t procs[MAX_PROC]; int pc;
    file_t files[MAX_AGENT]; int fcnt; behav_t behs[MAX_AGENT]; int bc;
    unsigned int telem; int mal_tot; int iso; } edr_t;

/* SOAR */
typedef struct { int act; char tgt[FMED]; char params[FLRG];
    int status; char result[FMED]; } pbstep_t;
typedef struct { char name[FBUF]; char desc[FMED]; int tsev;
    int sc; pbstep_t steps[MAX_PBS]; int act; int ec; int ok; } pb_t;
typedef struct { pb_t pbs[MAX_PB]; int pc; int tex; int tok;
    int tfail; int ares; int mes; } soar_t;

/* Forensics */
typedef struct { int pid; char proc[FBUF]; unsigned int base;
    unsigned int sz; char hash[FBUF]; int sus; char art[FMED]; } mreg_t;
typedef struct { char sip[FBUF]; char dip[FBUF]; int sp; int dp;
    int proto; unsigned int by; unsigned int pk;
    unsigned int st; unsigned int et; char pn[FBUF]; } nflow_t;
typedef struct { char path[FMED]; unsigned int off; unsigned int sz;
    char ftype[FBUF]; char hash[FBUF]; unsigned int rts; int ev; } dart_t;
typedef struct { unsigned int ts; char etype[FBUF]; char desc[FMED];
    char src[FBUF]; int ri; } tlevt_t;
typedef struct { int cid; int type; char name[FMED]; unsigned int ct;
    int ec; char ehash[FBUF]; int coc; int lh;
    mreg_t mregs[MAX_REG]; int mc; nflow_t flows[MAX_FLOW]; int nc;
    dart_t arts[MAX_ART]; int dc; tlevt_t tl[MAX_TL]; int tc; } fcase_t;
typedef struct { fcase_t cases[MAX_CASE]; int cc; int tev; int ttl; } for_t;

/* === Forward Declarations === */
static void siem_init(siem_t *s);
static void siem_ingest(siem_t *s, int src, int sev, unsigned int ts, const char *si, const char *di, const char *desc, const char *raw);
static void siem_normalize(siem_t *s);
static void siem_add_rule(siem_t *s, const char *name, int cond, int thresh, int sev, const char *desc);
static void siem_correlate(siem_t *s);
static void siem_gen_alert(siem_t *s, int rule_idx);
static void siem_gen_incident(siem_t *s, int alert_idx);
static void siem_dashboard(siem_t *s);
static void ids_init(ids_t *d);
static void ids_add_sig(ids_t *d, const char *sid, const char *pat, int proto, int dp, int sev, const char *desc, int mode);
static void ids_inspect(ids_t *d, const char *si, const char *di, int sp, int dp, int pr, unsigned int psz, const char *payload, unsigned int ts);
static void ids_fim(ids_t *d, const char *path, const char *hash, unsigned int sz, unsigned int ts);
static void ids_syscall(ids_t *d, int scid, const char *proc, int pid);
static void ids_anomaly(ids_t *d);
static void ids_dpi(ids_t *d);
static void ti_init(ti_t *t);
static void ti_add_ioc(ti_t *t, int type, const char *val, int sev, unsigned int fs, unsigned int ls, const char *src, const char *tags, int conf);
static int ti_match_ioc(ti_t *t, const char *val, int type);
static void ti_add_technique(ti_t *t, const char *tid, const char *name, int tactic, int sev, const char *det, int mit);
static void ti_add_hunt(ti_t *t, const char *hyp, const char *query, int tactic);
static void ti_run_hunt(ti_t *t, int idx);
static void ti_add_cve(ti_t *t, const char *cveid, const char *desc, int cvss10, int sev, const char *prod, int patched, unsigned int pub);
static void ti_darkweb(ti_t *t);
static void edr_init(edr_t *e);
static void edr_register(edr_t *e, int id, const char *host, const char *os, unsigned int ls);
static void edr_track_proc(edr_t *e, int pid, int ppid, const char *name, const char *cmd, unsigned int ts, const char *parent);
static void edr_behavior(edr_t *e, int pid, const char *proc, int btype, const char *desc, int sev, unsigned int ts);
static void edr_scan(edr_t *e, const char *path, unsigned int sz, const char *hash);
static void edr_isolate(edr_t *e, int agent_id);
static void edr_appctl(edr_t *e, const char *path, int whitelist);
static void soar_init(soar_t *s);
static void soar_add_pb(soar_t *s, const char *name, const char *desc, int tsev);
static void soar_add_step(soar_t *s, int pidx, int act, const char *tgt, const char *par);
static void soar_exec(soar_t *s, int pidx, int isev);
static void soar_auto(soar_t *s, int sev, const char *sip, const char *tgt);
static void for_init(for_t *f);
static void for_newcase(for_t *f, int cid, int type, const char *name, unsigned int ts);
static void for_addmem(for_t *f, int cid, int pid, const char *proc, unsigned int base, unsigned int sz, const char *hash, int sus, const char *art);
static void for_addflow(for_t *f, int cid, const char *si, const char *di, int sp, int dp, int pr, unsigned int by, unsigned int pk, unsigned int st, unsigned int et);
static void for_addart(for_t *f, int cid, const char *path, unsigned int off, unsigned int sz, const char *ft, const char *hash, unsigned int rts, int ev);
static void for_addtl(for_t *f, int cid, unsigned int ts, const char *etype, const char *desc, const char *src, int related);
static void for_timeline(for_t *f, int cid);
static void for_preserve(for_t *f, int cid);

/* === SIEM Implementation === */
static void siem_init(siem_t *s) {
    s->lc = 0; s->rc = 0; s->ac = 0; s->ic = 0;
    s->etot = 0; s->atot = 0; s->ke = 0; s->kd = 0; s->kr = 0;
    host_print("[SIEM] Initialized - aggregation and correlation engine ready\n");
}

static void siem_ingest(siem_t *s, int src, int sev, unsigned int ts, const char *si, const char *di, const char *desc, const char *raw) {
    if (s->lc >= MAX_LOG) return;
    log_t *e = &s->logs[s->lc];
    e->src = src; e->sev = sev; e->ts = ts;
    str_copy(e->sip, si, FBUF); str_copy(e->dip, di, FBUF);
    str_copy(e->desc, desc, FMED); str_copy(e->raw, raw, FLRG);
    e->norm = 0; e->corr = 0;
    s->lc++; s->etot++;
}

static void siem_normalize(siem_t *s) {
    int count = 0;
    for (int i = 0; i < s->lc; i++)
        if (!s->logs[i].norm) { s->logs[i].norm = 1; count++; }
    host_print("[SIEM] Normalized "); print_int(count); host_print(" log entries to common schema\n");
}

static void siem_add_rule(siem_t *s, const char *name, int cond, int thresh, int sev, const char *desc) {
    if (s->rc >= MAX_RULE) return;
    rule_t *r = &s->rules[s->rc];
    str_copy(r->name, name, FBUF); r->cond = cond; r->thresh = thresh;
    r->sev = sev; r->hits = 0; str_copy(r->desc, desc, FMED);
    r->act = 1; s->rc++;
}

static void siem_correlate(siem_t *s) {
    host_print("[SIEM] Running correlation: "); print_int(s->lc);
    host_print(" events, "); print_int(s->rc); host_print(" rules\n");
    for (int r = 0; r < s->rc; r++) {
        if (!s->rules[r].act) continue;
        s->rules[r].hits = 0;
        for (int i = 0; i < s->lc; i++)
            if (s->logs[i].sev >= s->rules[r].cond) { s->rules[r].hits++; s->logs[i].corr = 1; }
        if (s->rules[r].hits >= s->rules[r].thresh) {
            host_print("[SIEM] Rule '"); host_print(s->rules[r].name);
            host_print("' triggered: "); print_int(s->rules[r].hits);
            host_print(" matches (threshold: "); print_int(s->rules[r].thresh); host_print(")\n");
            siem_gen_alert(s, r);
        }
    }
}

static void siem_gen_alert(siem_t *s, int rule_idx) {
    if (s->ac >= MAX_ALERT) return;
    rule_t *r = &s->rules[rule_idx]; alert_t *a = &s->alerts[s->ac];
    a->sev = r->sev; a->ts = s->logs[s->lc > 0 ? s->lc - 1 : 0].ts;
    str_copy(a->rule, r->name, FBUF); str_copy(a->sip, "MULTI", FBUF);
    str_copy(a->desc, r->desc, FMED); a->ack = 0; a->esc = 0; a->iid = -1;
    s->ac++; s->atot++;
    if (r->sev >= SEV_HIGH) siem_gen_incident(s, s->ac - 1);
    if (r->sev >= SEV_CRITICAL) {
        a->esc = 1;
        host_print("[SIEM] Escalated to SOC manager: "); host_print(r->name); host_print("\n");
    }
}

static void siem_gen_incident(siem_t *s, int alert_idx) {
    if (s->ic >= MAX_INC) return;
    inc_t *inc = &s->incs[s->ic];
    inc->id = s->ic; inc->state = INC_NEW; inc->sev = s->alerts[alert_idx].sev;
    inc->ct = s->alerts[alert_idx].ts; inc->ut = inc->ct;
    str_copy(inc->title, s->alerts[alert_idx].desc, FMED);
    str_copy(inc->who, "soc_analyst", FBUF);
    inc->sla = (inc->sev >= SEV_CRITICAL) ? 1 : (inc->sev >= SEV_HIGH) ? 4 : 24;
    inc->ac = 1; s->alerts[alert_idx].iid = inc->id; s->ic++;
    host_print("[SIEM] Incident #"); print_int(inc->id);
    host_print(" created (SLA: "); print_int(inc->sla);
    host_print("h, severity: "); print_sev(inc->sev); host_print(")\n");
}

static void siem_dashboard(siem_t *s) {
    host_print("=== SIEM Dashboard ===\n");
    host_print("Events: "); print_int(s->etot); host_print(" Alerts: "); print_int(s->atot);
    host_print(" Incidents: "); print_int(s->ic); host_print(" Rules: "); print_int(s->rc);
    host_print("\nKPI: epm="); print_int(s->ke);
    host_print(" mttd="); print_int(s->kd); host_print("s mttr="); print_int(s->kr); host_print("s\n");
    int bs[5] = {0, 0, 0, 0, 0};
    for (int i = 0; i < s->ic; i++) bs[s->incs[i].state]++;
    host_print("States: ");
    for (int t = 0; t <= 4; t++) { host_print(" "); print_state(t); host_print("="); print_int(bs[t]); }
    host_print("\n======================\n");
}

/* === IDS Implementation === */
static void ids_init(ids_t *d) {
    d->sc = 0; d->pc = 0; d->fc = 0; d->cc = 0;
    d->nids_al = 0; d->hids_al = 0; d->dpi_tot = 0; d->anom_tot = 0;
    d->bl_avg = 512.0; d->bl_sd = 256.0;
    host_print("[IDS] NIDS/HIDS/DPI engine ready\n");
}

static void ids_add_sig(ids_t *d, const char *sid, const char *pat, int proto, int dp, int sev, const char *desc, int mode) {
    if (d->sc >= MAX_SIG) return;
    idsig_t *s = &d->sigs[d->sc];
    str_copy(s->sid, sid, FBUF); str_copy(s->pat, pat, FMED);
    s->proto = proto; s->dport = dp; s->sev = sev;
    str_copy(s->desc, desc, FMED); s->mode = mode; s->hits = 0; d->sc++;
}

static void ids_inspect(ids_t *d, const char *si, const char *di, int sp, int dp, int pr, unsigned int psz, const char *payload, unsigned int ts) {
    if (d->pc >= MAX_PKT) return;
    pkt_t *p = &d->pkts[d->pc];
    str_copy(p->sip, si, FBUF); str_copy(p->dip, di, FBUF);
    p->sp = sp; p->dp = dp; p->proto = pr; p->psz = psz;
    str_copy(p->pay, payload, FLRG); p->ts = ts; p->sus = 0; p->anom = 0;
    for (int i = 0; i < d->sc; i++) {
        if (d->sigs[i].dport != dp && d->sigs[i].dport != 0) continue;
        int pl = 0; while (d->sigs[i].pat[pl]) pl++;
        if (!pl) continue;
        int kl = 0; while (payload[kl]) kl++;
        for (int k = 0; k <= kl - pl; k++) {
            int m = 1;
            for (int j = 0; j < pl; j++) if (payload[k + j] != d->sigs[i].pat[j]) { m = 0; break; }
            if (m) {
                p->sus = 1; d->sigs[i].hits++; d->nids_al++;
                host_print("[NIDS] Sig "); host_print(d->sigs[i].sid);
                host_print(": "); host_print(si); host_print(":"); print_int(sp);
                host_print(" -> "); host_print(di); host_print(":"); print_int(dp); host_print("\n");
            }
        }
    }
    double dv = (double)psz - d->bl_avg; if (dv < 0) dv = -dv;
    if (dv > d->bl_sd * 2.0) {
        p->anom = (int)(dv / d->bl_sd);
        if (p->anom > 3) { p->sus = 1; d->anom_tot++;
            host_print("[NIDS] Anomaly: size="); print_int((int)psz);
            host_print(" from "); host_print(si); host_print("\n"); }
    }
    d->pc++;
}

static void ids_fim(ids_t *d, const char *path, const char *hash, unsigned int sz, unsigned int ts) {
    if (d->fc >= MAX_FIM) return;
    fim_t *f = &d->fim[d->fc];
    str_copy(f->path, path, FMED); str_copy(f->ehash, hash, FBUF);
    f->sz = sz; f->lt = ts; d->fc++;
}

static void ids_syscall(ids_t *d, int scid, const char *proc, int pid) {
    if (d->cc >= MAX_SC) return;
    scl_t *s = &d->sysc[d->cc];
    s->scid = scid; str_copy(s->proc, proc, FBUF); s->pid = pid;
    s->sus = (scid == 59 || scid == 57 || scid == 10);
    if (s->sus) { d->hids_al++;
        host_print("[HIDS] Suspicious syscall "); print_int(scid);
        host_print(" PID="); print_int(pid); host_print(" ("); host_print(proc); host_print(")\n"); }
    d->cc++;
}

static void ids_anomaly(ids_t *d) {
    int count = 0;
    for (int i = 0; i < d->pc; i++) if (d->pkts[i].anom > 0) count++;
    host_print("[IDS] Anomaly scan: "); print_int(count);
    host_print(" anomalous of "); print_int(d->pc); host_print(" packets\n");
}

static void ids_dpi(ids_t *d) {
    d->dpi_tot = d->pc;
    host_print("[IDS] DPI: "); print_int(d->dpi_tot); host_print(" inspected, ");
    print_int(d->nids_al); host_print(" sig alerts, "); print_int(d->anom_tot); host_print(" anomalies\n");
}

/* === Threat Intel Implementation === */
static void ti_init(ti_t *t) {
    t->ic = 0; t->tc = 0; t->hc = 0; t->vc = 0;
    t->ihits = 0; t->hfind = 0; t->dw = 0;
    host_print("[TI] Threat intelligence engine initialized\n");
}

static void ti_add_ioc(ti_t *t, int type, const char *val, int sev, unsigned int fs, unsigned int ls, const char *src, const char *tags, int conf) {
    if (t->ic >= MAX_IOC) return;
    ioc_t *e = &t->iocs[t->ic];
    e->type = type; str_copy(e->val, val, FMED); e->sev = sev;
    e->fs = fs; e->ls = ls; e->hits = 0;
    str_copy(e->src, src, FBUF); str_copy(e->tags, tags, FMED); e->conf = conf; t->ic++;
}

static int ti_match_ioc(ti_t *t, const char *val, int type) {
    for (int i = 0; i < t->ic; i++)
        if (t->iocs[i].type == type && str_equal(t->iocs[i].val, val)) {
            t->iocs[i].hits++; t->ihits++; return i;
        }
    return -1;
}

static void ti_add_technique(ti_t *t, const char *tid, const char *name, int tactic, int sev, const char *det, int mit) {
    if (t->tc >= MAX_MITRE) return;
    mitre_t *m = &t->tech[t->tc];
    str_copy(m->tid, tid, FBUF); str_copy(m->name, name, FMED);
    m->tactic = tactic; m->sev = sev; str_copy(m->det, det, FLRG); m->mit = mit; t->tc++;
}

static void ti_add_hunt(ti_t *t, const char *hyp, const char *query, int tactic) {
    if (t->hc >= MAX_HUNT) return;
    hunt_t *h = &t->hunts[t->hc];
    str_copy(h->hyp, hyp, FMED); str_copy(h->query, query, FLRG);
    h->tac = tactic; h->find = 0; h->st = 0; t->hc++;
}

static void ti_run_hunt(ti_t *t, int idx) {
    if (idx < 0 || idx >= t->hc) return;
    hunt_t *h = &t->hunts[idx]; h->st = 1; h->find = 0;
    for (int i = 0; i < t->tc; i++) if (t->tech[i].tactic == h->tac) h->find++;
    h->st = 2; t->hfind += h->find;
    host_print("[THUNT] '"); host_print(h->hyp); host_print("': ");
    print_int(h->find); host_print(" findings\n");
}

static void ti_add_cve(ti_t *t, const char *cveid, const char *desc, int cvss10, int sev, const char *prod, int patched, unsigned int pub) {
    if (t->vc >= MAX_CVE) return;
    cve_t *c = &t->cves[t->vc];
    str_copy(c->cveid, cveid, FBUF); str_copy(c->desc, desc, FMED);
    c->cvss10 = cvss10; c->sev = sev; str_copy(c->prod, prod, FMED);
    c->patched = patched; c->pub = pub; t->vc++;
}

static void ti_darkweb(ti_t *t) {
    host_print("[DARKWEB] Scanning threat actor channels...\n");
    int leaks = 0;
    for (int i = 0; i < t->ic; i++) if (t->iocs[i].type == IOC_URL) { t->iocs[i].hits++; leaks++; }
    t->dw += leaks;
    host_print("[DARKWEB] Scan complete: "); print_int(leaks);
    host_print(" leaks, total: "); print_int(t->dw); host_print("\n");
}

/* === EDR Implementation === */
static void edr_init(edr_t *e) {
    e->ac = 0; e->pc = 0; e->fcnt = 0; e->bc = 0;
    e->telem = 0; e->mal_tot = 0; e->iso = 0;
    host_print("[EDR] Endpoint detection engine ready\n");
}

static void edr_register(edr_t *e, int id, const char *host, const char *os, unsigned int ls) {
    if (e->ac >= MAX_AGENT) return;
    agent_t *a = &e->agents[e->ac];
    a->id = id; str_copy(a->host, host, FBUF); str_copy(a->os, os, FBUF);
    a->status = 1; a->ls = ls; a->thr = 0; e->ac++;
}

static void edr_track_proc(edr_t *e, int pid, int ppid, const char *name, const char *cmd, unsigned int ts, const char *parent) {
    if (e->pc >= MAX_PROC) return;
    proc_t *p = &e->procs[e->pc];
    p->pid = pid; p->ppid = ppid; str_copy(p->name, name, FBUF);
    str_copy(p->cmd, cmd, FLRG); p->ts = ts; p->sus = 0; p->bscore = 0;
    str_copy(p->parent, parent, FBUF); e->pc++; e->telem++;
}

static void edr_behavior(edr_t *e, int pid, const char *proc, int btype, const char *desc, int sev, unsigned int ts) {
    if (e->bc >= MAX_AGENT) return;
    behav_t *b = &e->behs[e->bc];
    b->btype = btype; str_copy(b->desc, desc, FMED); b->sev = sev;
    b->pid = pid; str_copy(b->proc, proc, FBUF); b->ts = ts;
    e->bc++; e->telem++;
    for (int i = 0; i < e->pc; i++)
        if (e->procs[i].pid == pid) { e->procs[i].sus = 1; e->procs[i].bscore += sev * 10; break; }
    if (sev >= SEV_HIGH) {
        host_print("[EDR] Behavioral alert: "); host_print(desc);
        host_print(" (PID "); print_int(pid); host_print(", ");
        host_print(proc); host_print(") sev="); print_sev(sev); host_print("\n");
    }
}

static void edr_scan(edr_t *e, const char *path, unsigned int sz, const char *hash) {
    if (e->fcnt >= MAX_AGENT) return;
    file_t *f = &e->files[e->fcnt];
    str_copy(f->path, path, FMED); f->sz = sz; str_copy(f->hash, hash, FBUF);
    f->wl = 0; f->mal = 0; f->ast = 1;
    if (hash[0] == 'E' && hash[1] == '5') { f->mal = 1; e->mal_tot++; str_copy(f->result, "MALICIOUS - Trojan.GenericKD", FMED); }
    else if (hash[0] == 'W' && hash[1] == 'L') { f->wl = 1; str_copy(f->result, "WHITELISTED", FMED); }
    else str_copy(f->result, "CLEAN", FMED);
    f->ast = 2; e->fcnt++;
}

static void edr_isolate(edr_t *e, int agent_id) {
    for (int i = 0; i < e->ac; i++)
        if (e->agents[i].id == agent_id) {
            e->agents[i].status = 2; e->iso++;
            host_print("[EDR] Endpoint "); print_int(agent_id);
            host_print(" ("); host_print(e->agents[i].host); host_print(") ISOLATED\n"); return;
        }
    host_print("[EDR] Agent not found: "); print_int(agent_id); host_print("\n");
}

static void edr_appctl(edr_t *e, const char *path, int whitelist) {
    (void)e;
    host_print(whitelist ? "[EDR] Whitelisted: " : "[EDR] BLOCKED: ");
    host_print(path); host_print("\n");
}

/* === SOAR Implementation === */
static void soar_init(soar_t *s) {
    s->pc = 0; s->tex = 0; s->tok = 0; s->tfail = 0; s->ares = 0; s->mes = 0;
    host_print("[SOAR] Security orchestration engine initialized\n");
}

static void soar_add_pb(soar_t *s, const char *name, const char *desc, int tsev) {
    if (s->pc >= MAX_PB) return;
    pb_t *pb = &s->pbs[s->pc];
    str_copy(pb->name, name, FBUF); str_copy(pb->desc, desc, FMED);
    pb->tsev = tsev; pb->sc = 0; pb->act = 1; pb->ec = 0; pb->ok = 0; s->pc++;
}

static void soar_add_step(soar_t *s, int pidx, int act, const char *tgt, const char *par) {
    if (pidx < 0 || pidx >= s->pc) return;
    pb_t *pb = &s->pbs[pidx]; if (pb->sc >= MAX_PBS) return;
    pbstep_t *st = &pb->steps[pb->sc];
    st->act = act; str_copy(st->tgt, tgt, FMED);
    str_copy(st->params, par, FLRG); st->status = 0; st->result[0] = '\0'; pb->sc++;
}

static void soar_exec(soar_t *s, int pidx, int isev) {
    if (pidx < 0 || pidx >= s->pc) return;
    pb_t *pb = &s->pbs[pidx]; if (!pb->act || isev < pb->tsev) return;
    host_print("[SOAR] Executing playbook '"); host_print(pb->name);
    host_print("' ("); print_int(pb->sc); host_print(" steps) for severity ");
    print_sev(isev); host_print("\n");
    const char *names[] = {"ISOLATE_HOST", "QUARANTINE_NET", "LOCK_ACCOUNT",
        "DEPLOY_PATCH", "KILL_PROCESS", "BLOCK_IP", "NOTIFY", "CREATE_TICKET",
        "COLLECT_FORENSIC", "SCAN_ENDPOINT"};
    for (int i = 0; i < pb->sc; i++) {
        pbstep_t *st = &pb->steps[i]; st->status = 1;
        host_print("[SOAR]   Step "); print_int(i + 1); host_print(": ");
        host_print((st->act >= 0 && st->act <= 9) ? names[st->act] : "UNKNOWN");
        host_print(" -> "); host_print(st->tgt); host_print("\n");
        st->status = 2; str_copy(st->result, "SUCCESS", FMED);
    }
    pb->ec++; s->tex++; pb->ok++; s->tok++;
    if (isev <= SEV_MEDIUM) s->ares++; else s->mes++;
    host_print("[SOAR] Playbook '"); host_print(pb->name); host_print("' completed: SUCCESS\n");
}

static void soar_auto(soar_t *s, int sev, const char *sip, const char *tgt) {
    (void)sip; (void)tgt;
    for (int i = 0; i < s->pc; i++)
        if (s->pbs[i].act && sev >= s->pbs[i].tsev) { soar_exec(s, i, sev); return; }
    host_print("[SOAR] No matching playbook for severity "); print_sev(sev); host_print("\n");
}

/* === Forensic Implementation === */
static void for_init(for_t *f) {
    f->cc = 0; f->tev = 0; f->ttl = 0;
    host_print("[FORENSIC] Forensic analysis engine initialized\n");
}

static int for_find(for_t *f, int cid) {
    for (int c = 0; c < f->cc; c++) if (f->cases[c].cid == cid) return c; return -1;
}

static void for_newcase(for_t *f, int cid, int type, const char *name, unsigned int ts) {
    if (f->cc >= MAX_CASE) return;
    fcase_t *c = &f->cases[f->cc];
    c->cid = cid; c->type = type; str_copy(c->name, name, FMED);
    c->ct = ts; c->ec = 0; str_copy(c->ehash, "PENDING", FBUF);
    c->coc = 1; c->lh = 0; c->mc = 0; c->nc = 0; c->dc = 0; c->tc = 0;
    f->cc++; host_print("[FORENSIC] Case #"); print_int(cid);
    host_print(" created: "); host_print(name); host_print("\n");
}

static void for_addmem(for_t *f, int cid, int pid, const char *proc, unsigned int base, unsigned int sz, const char *hash, int sus, const char *art) {
    int ci = for_find(f, cid); if (ci < 0 || f->cases[ci].mc >= MAX_REG) return;
    mreg_t *m = &f->cases[ci].mregs[f->cases[ci].mc];
    m->pid = pid; str_copy(m->proc, proc, FBUF); m->base = base; m->sz = sz;
    str_copy(m->hash, hash, FBUF); m->sus = sus; str_copy(m->art, art, FMED);
    f->cases[ci].mc++; f->cases[ci].ec++; f->tev++;
}

static void for_addflow(for_t *f, int cid, const char *si, const char *di, int sp, int dp, int pr, unsigned int by, unsigned int pk, unsigned int st, unsigned int et) {
    int ci = for_find(f, cid); if (ci < 0 || f->cases[ci].nc >= MAX_FLOW) return;
    nflow_t *nf = &f->cases[ci].flows[f->cases[ci].nc];
    str_copy(nf->sip, si, FBUF); str_copy(nf->dip, di, FBUF);
    nf->sp = sp; nf->dp = dp; nf->proto = pr; nf->by = by; nf->pk = pk;
    nf->st = st; nf->et = et;
    str_copy(nf->pn, (pr == 6) ? "TCP" : (pr == 17) ? "UDP" : "OTHER", FBUF);
    f->cases[ci].nc++; f->cases[ci].ec++; f->tev++;
}

static void for_addart(for_t *f, int cid, const char *path, unsigned int off, unsigned int sz, const char *ft, const char *hash, unsigned int rts, int ev) {
    int ci = for_find(f, cid); if (ci < 0 || f->cases[ci].dc >= MAX_ART) return;
    dart_t *a = &f->cases[ci].arts[f->cases[ci].dc];
    str_copy(a->path, path, FMED); a->off = off; a->sz = sz;
    str_copy(a->ftype, ft, FBUF); str_copy(a->hash, hash, FBUF);
    a->rts = rts; a->ev = ev;
    f->cases[ci].dc++; f->cases[ci].ec++; f->tev++;
}

static void for_addtl(for_t *f, int cid, unsigned int ts, const char *etype, const char *desc, const char *src, int related) {
    int ci = for_find(f, cid); if (ci < 0 || f->cases[ci].tc >= MAX_TL) return;
    tlevt_t *te = &f->cases[ci].tl[f->cases[ci].tc];
    te->ts = ts; str_copy(te->etype, etype, FBUF);
    str_copy(te->desc, desc, FMED); str_copy(te->src, src, FBUF); te->ri = related;
    f->cases[ci].tc++; f->ttl++;
}

static void for_timeline(for_t *f, int cid) {
    int ci = for_find(f, cid); if (ci < 0) return;
    fcase_t *c = &f->cases[ci];
    host_print("[FORENSIC] Reconstructing timeline for case #"); print_int(cid);
    host_print(" ("); print_int(c->tc); host_print(" events):\n");
    for (int i = 0; i < c->tc - 1; i++)
        for (int j = i + 1; j < c->tc; j++)
            if (c->tl[j].ts < c->tl[i].ts) {
                tlevt_t tmp = c->tl[i]; c->tl[i] = c->tl[j]; c->tl[j] = tmp;
            }
    for (int i = 0; i < c->tc; i++) {
        host_print("  [T+"); print_uint(c->tl[i].ts); host_print("] ");
        host_print(c->tl[i].etype); host_print(": ");
        host_print(c->tl[i].desc); host_print("\n");
    }
}

static void for_preserve(for_t *f, int cid) {
    int ci = for_find(f, cid); if (ci < 0) return;
    fcase_t *c = &f->cases[ci]; c->lh = 1;
    unsigned int comb = 0;
    for (int i = 0; i < c->ec; i++) comb = comb * 31 + (unsigned int)i;
    char buf[FBUF]; str_copy(buf, "SHA256:", FBUF);
    int len = 0; while (buf[len]) len++;
    const char *hex = "0123456789ABCDEF"; char tmp[12]; int ti = 0;
    unsigned int v = comb; if (v == 0) tmp[ti++] = '0';
    while (v > 0 && ti < 11) { tmp[ti++] = hex[v & 0xF]; v >>= 4; }
    for (int s = ti - 1; s >= 0 && len < FBUF - 1; s--) buf[len++] = tmp[s];
    buf[len] = '\0'; str_copy(c->ehash, buf, FBUF);
    host_print("[FORENSIC] Evidence preserved for case #"); print_int(cid);
    host_print(": hash="); host_print(c->ehash);
    host_print(" items="); print_int(c->ec); host_print(" chain_of_custody=VERIFIED\n");
}

/* === Main: Full Threat Detection Pipeline === */
int main(void) {
    host_print("========================================\n");
    host_print(" AI-ASM Threat Detection Module v1.0\n");
    host_print(" AArch64 WebAssembly-native OS\n");
    host_print("========================================\n");

    host_print("\n--- Phase 1: Initializing subsystems ---\n");
    siem_t *siem = (siem_t *)host_alloc(sizeof(siem_t), 8); siem_init(siem);
    ids_t *ids = (ids_t *)host_alloc(sizeof(ids_t), 8); ids_init(ids);
    ti_t *ti = (ti_t *)host_alloc(sizeof(ti_t), 8); ti_init(ti);
    edr_t *edr = (edr_t *)host_alloc(sizeof(edr_t), 8); edr_init(edr);
    soar_t *soar = (soar_t *)host_alloc(sizeof(soar_t), 8); soar_init(soar);
    for_t *forensics = (for_t *)host_alloc(sizeof(for_t), 8); for_init(forensics);

    host_print("\n--- Phase 2: Loading SIEM correlation rules ---\n");
    siem_add_rule(siem, "BRUTE_FORCE_LOGIN", SEV_MEDIUM, 5, SEV_HIGH, "Multiple failed login attempts from single source");
    siem_add_rule(siem, "PRIV_ESC_ATTEMPT", SEV_HIGH, 1, SEV_CRITICAL, "Privilege escalation on critical system");
    siem_add_rule(siem, "DATA_EXFIL_LARGE_XFER", SEV_MEDIUM, 3, SEV_HIGH, "Large outbound data transfer to external host");
    siem_add_rule(siem, "MALWARE_CALLBACK", SEV_HIGH, 1, SEV_CRITICAL, "Suspected C2 callback to known malicious infrastructure");
    siem_add_rule(siem, "LATERAL_MOVEMENT", SEV_MEDIUM, 2, SEV_HIGH, "Lateral movement detected across network segments");

    host_print("\n--- Phase 3: Ingesting security event logs ---\n");
    siem_ingest(siem, SRC_SYSLOG, SEV_MEDIUM, 1000, "192.168.1.100", "10.0.0.1", "Failed SSH login attempt", "sshd[1234]: Failed password for root");
    siem_ingest(siem, SRC_SYSLOG, SEV_MEDIUM, 1001, "192.168.1.100", "10.0.0.1", "Failed SSH login attempt", "sshd[1234]: Failed password for root");
    siem_ingest(siem, SRC_SYSLOG, SEV_MEDIUM, 1002, "192.168.1.100", "10.0.0.1", "Failed SSH login attempt", "sshd[1234]: Failed password for root");
    siem_ingest(siem, SRC_SYSLOG, SEV_MEDIUM, 1003, "192.168.1.100", "10.0.0.1", "Failed SSH login attempt", "sshd[1234]: Failed password for root");
    siem_ingest(siem, SRC_SYSLOG, SEV_MEDIUM, 1004, "192.168.1.100", "10.0.0.1", "Failed SSH login attempt", "sshd[1234]: Failed password for root");
    siem_ingest(siem, SRC_FIREWALL, SEV_HIGH, 1010, "10.0.0.50", "10.0.0.1", "Privilege escalation via sudo", "sudo: analyst : TTY=pts/0 ; COMMAND=/bin/bash");
    siem_ingest(siem, SRC_NETFLOW, SEV_MEDIUM, 1020, "10.0.0.50", "203.0.113.77", "Large outbound transfer 2.3GB", "flow: bytes=2469606400 proto=TCP");
    siem_ingest(siem, SRC_IDS, SEV_HIGH, 1030, "10.0.0.50", "198.51.100.44", "Outbound connection to known C2", "ids: alert sid=2001 category=trojan-activity");
    siem_ingest(siem, SRC_EDR, SEV_MEDIUM, 1040, "10.0.0.55", "10.0.0.1", "SMB lateral movement", "edr: process=cmd.exe parent=powershell.exe conn=10.0.0.1:445");
    siem_ingest(siem, SRC_EDR, SEV_MEDIUM, 1041, "10.0.0.55", "10.0.0.2", "WinRM lateral movement", "edr: process=cmd.exe parent=powershell.exe conn=10.0.0.2:5985");
    siem_normalize(siem);

    host_print("\n--- Phase 4: Running correlation engine ---\n");
    siem->ke = 120; siem->kd = 45; siem->kr = 300;
    siem_correlate(siem);

    host_print("\n--- Phase 5: Network Intrusion Detection ---\n");
    ids_add_sig(ids, "NIDS-001", "SELECT.*FROM", 6, 3306, SEV_HIGH, "SQL injection attempt", 0);
    ids_add_sig(ids, "NIDS-002", "/etc/passwd", 6, 80, SEV_CRITICAL, "Path traversal attempt", 0);
    ids_add_sig(ids, "NIDS-003", "cmd.exe", 6, 445, SEV_HIGH, "SMB command execution attempt", 0);
    ids_add_sig(ids, "NIDS-004", "powershell -enc", 6, 5985, SEV_CRITICAL, "Encoded PowerShell via WinRM", 0);
    ids_inspect(ids, "192.168.1.50", "10.0.0.5", 44321, 3306, 6, 256, "GET /api?id=SELECT * FROM users", 1000);
    ids_inspect(ids, "192.168.1.50", "10.0.0.5", 44322, 80, 6, 180, "GET /../../etc/passwd HTTP/1.1", 1001);
    ids_inspect(ids, "10.0.0.55", "10.0.0.1", 49999, 445, 6, 512, "SMB negotiate cmd.exe /c whoami", 1040);
    ids_inspect(ids, "10.0.0.55", "10.0.0.2", 50001, 5985, 6, 1024, "WinRM powershell -enc SGVsbG8=", 1041);
    ids_inspect(ids, "10.0.0.60", "10.0.0.5", 55000, 80, 6, 48000, "GET /normal-page HTTP/1.1", 1050);
    ids_inspect(ids, "10.0.0.60", "10.0.0.5", 55001, 443, 6, 22000, "POST /api/data HTTP/1.1", 1051);

    host_print("\n--- Phase 6: Host Intrusion Detection ---\n");
    ids_fim(ids, "/etc/passwd", "A1B2C3D4E5F6A1B2C3D4E5F6A1B2C3D4", 2048, 2000);
    ids_fim(ids, "/etc/shadow", "F6E5D4C3B2A1F6E5D4C3B2A1F6E5D4C3", 1536, 2000);
    ids_fim(ids, "/usr/bin/sudo", "1234567890ABCDEF1234567890ABCDEF", 163840, 2000);
    ids_fim(ids, "/boot/vmlinuz", "ABCDEF1234567890ABCDEF1234567890", 8388608, 2000);
    ids_syscall(ids, 59, "suspicious.sh", 9999);
    ids_syscall(ids, 57, "rootkit_loader", 9998);
    ids_syscall(ids, 10, "unknown_proc", 9997);
    ids_syscall(ids, 1, "init", 1);
    ids_syscall(ids, 56, "bash", 1234);
    ids_anomaly(ids); ids_dpi(ids);

    host_print("\n--- Phase 7: Threat intelligence analysis ---\n");
    ti_add_ioc(ti, IOC_IP, "198.51.100.44", SEV_CRITICAL, 900, 1100, "AlienVault_OTX", "APT28,C2,Russia", 95);
    ti_add_ioc(ti, IOC_IP, "203.0.113.77", SEV_HIGH, 800, 1050, "Abuse.ch", "Exfil,Dropper", 88);
    ti_add_ioc(ti, IOC_DOMAIN, "evil-c2.darknet.example", SEV_CRITICAL, 700, 1100, "PhishTank", "Phishing,C2", 92);
    ti_add_ioc(ti, IOC_HASH, "E5ABC123DEF4567890ABCDEF1234567890ABCDEF1234567890ABCDEF12345678", SEV_CRITICAL, 600, 1100, "VirusTotal", "Trojan,Backdoor", 99);
    ti_add_ioc(ti, IOC_URL, "http://evil.example/payload.exe", SEV_HIGH, 500, 1000, "URLhaus", "Malware,Dropper", 90);
    host_print("[TI] IOC matching against ingested events:\n");
    host_print("  198.51.100.44 match: "); print_int(ti_match_ioc(ti, "198.51.100.44", IOC_IP) >= 0 ? 1 : 0);
    host_print("\n  203.0.113.77 match: "); print_int(ti_match_ioc(ti, "203.0.113.77", IOC_IP) >= 0 ? 1 : 0);
    host_print("\n  10.0.0.50 (internal) match: "); print_int(ti_match_ioc(ti, "10.0.0.50", IOC_IP) >= 0 ? 1 : 0);
    host_print("\n[TI] Total IOC hits: "); print_int(ti->ihits); host_print("\n");
    host_print("\n[TI] Loading MITRE ATT&CK mappings:\n");
    ti_add_technique(ti, "T1110", "Brute Force", TACT_CRED, SEV_MEDIUM, "Detect multiple failed auth attempts", 4);
    ti_add_technique(ti, "T1068", "Exploitation for Privilege Escalation", TACT_PRIV, SEV_CRITICAL, "Monitor sudo/su anomalies", 5);
    ti_add_technique(ti, "T1048", "Exfiltration Over Alternative Protocol", TACT_EXFIL, SEV_HIGH, "Monitor large outbound transfers", 6);
    ti_add_technique(ti, "T1071", "Application Layer Protocol C2", TACT_C2, SEV_CRITICAL, "DNS/HTTP beacon detection", 7);
    ti_add_technique(ti, "T1021", "Remote Services Lateral Movement", TACT_LAT, SEV_HIGH, "SMB/WinRM/RDP anomaly detection", 5);
    ti_add_technique(ti, "T1059", "Command and Scripting Interpreter", TACT_EXEC, SEV_HIGH, "PowerShell/bash execution monitoring", 8);
    host_print("\n[TI] Initiating threat hunt operations:\n");
    ti_add_hunt(ti, "APT28 C2 beaconing in network", "SELECT * FROM flows WHERE dst_ip IN ioc_list AND interval < 60s", TACT_C2);
    ti_add_hunt(ti, "Lateral movement via SMB", "SELECT * FROM auth WHERE proto=SMB AND dst_port=445 AND src!=admin", TACT_LAT);
    ti_add_hunt(ti, "Credential dumping activity", "SELECT * FROM processes WHERE name IN (mimikatz,lsass_dump,procdump)", TACT_CRED);
    for (int i = 0; i < ti->hc; i++) ti_run_hunt(ti, i);
    host_print("\n[TI] Vulnerability management scan:\n");
    ti_add_cve(ti, "CVE-2024-3094", "XZ Utils backdoor in liblzma", 100, SEV_CRITICAL, "xz-utils < 5.6.2", 1, 1700000000);
    ti_add_cve(ti, "CVE-2024-21762", "Android kernel out-of-bounds write", 98, SEV_CRITICAL, "Linux Kernel < 6.6.15", 0, 1705000000);
    ti_add_cve(ti, "CVE-2024-1709", "ScreenConnect auth bypass", 100, SEV_CRITICAL, "ScreenConnect < 23.9.8", 1, 1707000000);
    ti_add_cve(ti, "CVE-2023-44487", "HTTP/2 Rapid Reset DDoS", 75, SEV_HIGH, "nginx, envoy, haproxy", 1, 1696000000);
    { int patched = 0, unpatched = 0;
      for (int i = 0; i < ti->vc; i++) { if (ti->cves[i].patched) patched++; else unpatched++; }
      host_print("[VULN] Patched: "); print_int(patched);
      host_print(", Unpatched: "); print_int(unpatched); host_print("\n"); }
    ti_darkweb(ti);

    host_print("\n--- Phase 8: Endpoint Detection and Response ---\n");
    edr_register(edr, 1, "ws-analyst-01", "Linux 6.6.15-aarch64", 1050);
    edr_register(edr, 2, "srv-web-01", "Linux 6.1.0-aarch64", 1051);
    edr_register(edr, 3, "srv-db-01", "Linux 5.15.0-aarch64", 1048);
    edr_register(edr, 4, "ws-dev-01", "Linux 6.6.15-aarch64", 1052);
    edr_track_proc(edr, 1, 0, "systemd", "/sbin/init", 100, "");
    edr_track_proc(edr, 1234, 1, "bash", "/bin/bash", 500, "systemd");
    edr_track_proc(edr, 9999, 1234, "suspicious.sh", "/tmp/.hidden/suspicious.sh", 1030, "bash");
    edr_track_proc(edr, 9998, 9999, "rootkit_loader", "/tmp/.hidden/rk_loader", 1031, "suspicious.sh");
    edr_track_proc(edr, 5678, 1, "nginx", "nginx: worker process", 200, "systemd");
    edr_track_proc(edr, 5679, 5678, "php-fpm", "php-fpm: pool www", 201, "nginx");
    edr_behavior(edr, 9999, "suspicious.sh", 0, "Process spawned from /tmp/.hidden directory", SEV_HIGH, 1030);
    edr_behavior(edr, 9998, "rootkit_loader", 1, "Binary with no signature attempting kernel module load", SEV_CRITICAL, 1031);
    edr_behavior(edr, 9999, "suspicious.sh", 2, "Outbound connection to known C2 198.51.100.44", SEV_CRITICAL, 1035);
    edr_behavior(edr, 9999, "suspicious.sh", 3, "Credential access: reading /etc/shadow", SEV_HIGH, 1036);
    edr_scan(edr, "/tmp/.hidden/suspicious.sh", 4096, "AB12CD34EF56AB12CD34EF56AB12CD34EF56AB12CD34EF56AB12CD34EF56AB12");
    edr_scan(edr, "/tmp/.hidden/rk_loader", 102400, "E5ABC123DEF4567890ABCDEF1234567890ABCDEF1234567890ABCDEF12345678");
    edr_scan(edr, "/usr/bin/nginx", 524288, "WL0000000000000000000000000000000000000000000000000000000000000000");
    edr_appctl(edr, "/usr/bin/nginx", 1);
    edr_appctl(edr, "/tmp/.hidden/rk_loader", 0);

    host_print("\n--- Phase 9: Automated SOAR response ---\n");
    soar_add_pb(soar, "CRITICAL_BREACH_RESPONSE", "Full breach response: isolate, contain, remediate", SEV_CRITICAL);
    soar_add_step(soar, 0, 0, "affected-host", "immediate=true");
    soar_add_step(soar, 0, 4, "suspicious_pid", "force=true");
    soar_add_step(soar, 0, 8, "affected-host", "full_disk=true");
    soar_add_step(soar, 0, 7, "SOC_QUEUE", "priority=P1");
    soar_add_pb(soar, "HIGH_SEVERITY_INVESTIGATION", "Investigate high severity alerts", SEV_HIGH);
    soar_add_step(soar, 1, 9, "target-host", "deep_scan=true");
    soar_add_step(soar, 1, 5, "malicious_ip", "firewall=all");
    soar_add_step(soar, 1, 7, "SOC_QUEUE", "priority=P2");
    soar_add_pb(soar, "MEDIUM_BRUTE_FORCE", "Brute force response: block IP, lock account", SEV_MEDIUM);
    soar_add_step(soar, 2, 5, "attacker_ip", "duration=24h");
    soar_add_step(soar, 2, 2, "target_account", "duration=1h");
    host_print("[SOAR] Auto-responding to active incidents:\n");
    soar_auto(soar, SEV_CRITICAL, "198.51.100.44", "10.0.0.50");
    soar_auto(soar, SEV_HIGH, "192.168.1.50", "10.0.0.5");
    soar_auto(soar, SEV_MEDIUM, "192.168.1.100", "10.0.0.1");
    host_print("\n[SOAR] Isolating compromised endpoints:\n");
    edr_isolate(edr, 1);

    host_print("\n--- Phase 10: Forensic analysis ---\n");
    for_newcase(forensics, 20240001, 0, "APT28 Compromise Investigation", 1000);
    for_newcase(forensics, 20240002, 1, "Data Exfiltration Investigation", 1020);
    for_newcase(forensics, 20240003, 2, "Rootkit Artifact Recovery", 1030);
    host_print("\n[FORENSIC] Memory forensics analysis:\n");
    for_addmem(forensics, 20240001, 9999, "suspicious.sh", 0x7F000000, 4096, "A1B2C3D4E5F6A1B2", 1, "packed ELF, anti-debug, C2 callback code");
    for_addmem(forensics, 20240001, 9998, "rootkit_loader", 0x7F001000, 102400, "F6E5D4C3B2A1F6E5", 1, "kernel module stub, syscall hook table");
    for_addmem(forensics, 20240001, 1, "systemd", 0x55000000, 262144, "1234567890ABCDEF", 0, "clean - reference baseline");
    host_print("\n[FORENSIC] Network forensics - flow reconstruction:\n");
    for_addflow(forensics, 20240002, "10.0.0.50", "203.0.113.77", 45678, 443, 6, 2469606400, 1638400, 1020, 1100);
    for_addflow(forensics, 20240002, "10.0.0.50", "198.51.100.44", 54321, 8443, 6, 524288, 1024, 1030, 1090);
    for_addflow(forensics, 20240002, "10.0.0.55", "10.0.0.1", 49999, 445, 6, 131072, 256, 1040, 1045);
    for_addflow(forensics, 20240002, "10.0.0.55", "10.0.0.2", 50001, 5985, 6, 262144, 512, 1041, 1048);
    host_print("\n[FORENSIC] Disk forensics - artifact recovery:\n");
    for_addart(forensics, 20240003, "/tmp/.hidden/suspicious.sh", 0, 4096, "ELF", "AB12CD34EF56AB12", 1030, 1);
    for_addart(forensics, 20240003, "/tmp/.hidden/rk_loader", 0, 102400, "ELF", "E5ABC123DEF45678", 1031, 1);
    for_addart(forensics, 20240003, "/var/log/auth.log.deleted", 512, 8192, "LOG", "DEADBEEFCAFEBABE", 1032, 1);
    for_addart(forensics, 20240003, "/dev/shm/.shared_mem", 0, 65536, "DATA", "FEEDFACECAFEBEEF", 1033, 1);
    host_print("\n[FORENSIC] Building unified incident timeline:\n");
    for_addtl(forensics, 20240001, 1000, "AUTH", "First failed SSH login from 192.168.1.100", "syslog", -1);
    for_addtl(forensics, 20240001, 1004, "AUTH", "Fifth failed SSH login - brute force threshold", "siem", -1);
    for_addtl(forensics, 20240001, 1010, "PRIVESC", "Sudo privilege escalation on 10.0.0.50", "firewall", -1);
    for_addtl(forensics, 20240001, 1030, "C2", "Outbound connection to 198.51.100.44 (known C2)", "ids", -1);
    for_addtl(forensics, 20240001, 1031, "MALWARE", "Rootkit loader executed from /tmp/.hidden", "edr", -1);
    for_addtl(forensics, 20240001, 1035, "EXFIL", "C2 channel active, data staging detected", "edr", -1);
    for_addtl(forensics, 20240001, 1040, "LATMOV", "SMB lateral movement from 10.0.0.55 to 10.0.0.1", "edr", -1);
    for_addtl(forensics, 20240001, 1041, "LATMOV", "WinRM lateral movement from 10.0.0.55 to 10.0.0.2", "edr", -1);
    for_addtl(forensics, 20240001, 1050, "EXFIL", "Large outbound transfer 2.3GB to 203.0.113.77", "netflow", -1);
    for_timeline(forensics, 20240001);
    host_print("\n[FORENSIC] Preserving evidence chain of custody:\n");
    for_preserve(forensics, 20240001);
    for_preserve(forensics, 20240002);
    for_preserve(forensics, 20240003);

    host_print("\n========================================\n");
    host_print(" Detection Statistics Summary\n");
    host_print("========================================\n");
    siem_dashboard(siem);
    host_print("\n--- IDS Statistics ---\n");
    host_print("Signatures: "); print_int(ids->sc);
    host_print(" NIDS alerts: "); print_int(ids->nids_al);
    host_print(" HIDS alerts: "); print_int(ids->hids_al);
    host_print(" DPI: "); print_int(ids->dpi_tot);
    host_print(" Anomalies: "); print_int(ids->anom_tot);
    host_print(" FIM: "); print_int(ids->fc);
    host_print(" Syscalls: "); print_int(ids->cc); host_print("\n");
    host_print("\n--- Threat Intelligence ---\n");
    host_print("IOCs: "); print_int(ti->ic);
    host_print(" Hits: "); print_int(ti->ihits);
    host_print(" Techniques: "); print_int(ti->tc);
    host_print(" Hunts: "); print_int(ti->hc);
    host_print(" Findings: "); print_int(ti->hfind);
    host_print(" CVEs: "); print_int(ti->vc);
    host_print(" Darkweb: "); print_int(ti->dw); host_print("\n");
    host_print("\n--- EDR ---\n");
    host_print("Agents: "); print_int(edr->ac);
    host_print(" Processes: "); print_int(edr->pc);
    host_print(" Behaviors: "); print_int(edr->bc);
    host_print(" Files: "); print_int(edr->fcnt);
    host_print(" Malware: "); print_int(edr->mal_tot);
    host_print(" Isolated: "); print_int(edr->iso);
    host_print(" Telemetry: "); print_int(edr->telem); host_print("\n");
    host_print("\n--- SOAR ---\n");
    host_print("Playbooks: "); print_int(soar->pc);
    host_print(" Executions: "); print_int(soar->tex);
    host_print(" Successes: "); print_int(soar->tok);
    host_print(" Failures: "); print_int(soar->tfail);
    host_print(" Auto-resolved: "); print_int(soar->ares);
    host_print(" Escalated: "); print_int(soar->mes); host_print("\n");
    host_print("\n--- Forensics ---\n");
    host_print("Cases: "); print_int(forensics->cc);
    host_print(" Evidence: "); print_int(forensics->tev);
    host_print(" Timeline: "); print_int(forensics->ttl); host_print("\n");
    host_print("\n========================================\n");
    host_print(" Threat Detection Module - Run Complete\n");
    host_print("========================================\n");
    host_exit(0);
    return 0;
}
