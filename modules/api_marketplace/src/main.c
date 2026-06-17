/*
 * api_marketplace.wasm - AI-ASM OS API Developer Portal & Marketplace
 *
 * API catalog, developer management, subscriptions, analytics,
 * monetization, and community features.
 * Target: wasm32-unknown-unknown (no-std, host imports)
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

#define MAX_CAT 32
#define MAX_API 256
#define MAX_DEV 512
#define MAX_APP 1024
#define MAX_KEY 2048
#define MAX_OA  128
#define MAX_SUB 2048
#define MAX_PLAN 16
#define MAX_BILL 4096
#define MAX_CMT  4096
#define MAX_ISS  1024
#define MAX_ANA  32768
#define MAX_TAGS 32
#define MAX_VER  8
#define MAX_SAMP 4
#define NAME_LEN 128
#define DESC_LEN 512
#define URL_LEN  256
#define KEY_LEN  64
#define BILLING_PPU  0
#define BILLING_MON  1
#define BILLING_TIER 2
#define PLAN_FREE    0
#define PLAN_PRO     1
#define PLAN_ENT     2
#define ST_PENDING   0
#define ST_APPROVED  1
#define ST_REJECTED  2

typedef struct { unsigned int id; char name[NAME_LEN]; unsigned int api_count, active; } category_t;
typedef struct { unsigned int id; char name[NAME_LEN]; unsigned int active; } developer_t;
typedef struct { unsigned int id, api_id, dev_id; unsigned int score; char review[DESC_LEN]; unsigned int ts; } rating_t;
typedef struct { unsigned int id, api_id, dev_id; char content[DESC_LEN]; unsigned int ts; } comment_t;
typedef struct { unsigned int id, api_id, dev_id; char title[NAME_LEN], desc[DESC_LEN]; unsigned int sev, resolved, ts; } issue_t;
typedef struct { unsigned int id; char name[NAME_LEN], desc[DESC_LEN]; unsigned int cat_id; unsigned int tags[MAX_TAGS], tag_ct; unsigned int ver_ct; unsigned int major[MAX_VER], minor[MAX_VER]; char spec[DESC_LEN]; char samples[MAX_SAMP][DESC_LEN]; unsigned int samp_ct, dev_id, active, created; } api_entry_t;
typedef struct { unsigned int id, app_id, dev_id; char cid[KEY_LEN], csec[KEY_LEN], redirect[URL_LEN], scope[DESC_LEN]; unsigned int ts, active; } oauth_app_t;
typedef struct { unsigned int id, app_id, dev_id; char name[NAME_LEN], redirect[URL_LEN]; unsigned int ts, active; } app_t;
typedef struct { unsigned int id; char key[KEY_LEN]; unsigned int app_id, dev_id, api_id, created, expires, revoked; } api_key_t;
typedef struct { unsigned int id; char name[NAME_LEN]; unsigned int plan_type, rate_limit, price_cents, billing; float tier_thresh[4], tier_price[4]; unsigned int tier_ct, active; } plan_t;
typedef struct { unsigned int id, dev_id, api_id, plan_id, status, approval, req_ts, app_ts, calls, quota, active; } subscription_t;
typedef struct { unsigned int api_id; unsigned int calls, errors; unsigned long long latency_sum; } analytics_rec_t;
typedef struct { unsigned int id, dev_id, amount, plan_id, sub_id, p_start, p_end, billing, calls_in_p, paid, ts; float unit_price; } bill_t;
typedef struct { unsigned int dev_id, api_id; char practice[DESC_LEN]; unsigned int ts; } best_practice_t;
typedef struct { unsigned int id, api_id, ver_idx; char msg[DESC_LEN]; unsigned int ts; } notification_t;

static category_t     g_cat[MAX_CAT];        static unsigned int g_cat_n;
static api_entry_t    g_api[MAX_API];        static unsigned int g_api_n;
static developer_t    g_dev[MAX_DEV];        static unsigned int g_dev_n;
static app_t          g_app[MAX_APP];        static unsigned int g_app_n;
static api_key_t      g_keys[MAX_KEY];       static unsigned int g_key_n;
static oauth_app_t    g_oa[MAX_OA];          static unsigned int g_oa_n;
static plan_t         g_plan[MAX_PLAN];      static unsigned int g_plan_n;
static subscription_t g_sub[MAX_SUB];        static unsigned int g_sub_n;
static analytics_rec_t g_ana[MAX_ANA];       static unsigned int g_ana_n;
static bill_t         g_bill[MAX_BILL];      static unsigned int g_bill_n;
static comment_t      g_cmt[MAX_CMT];        static unsigned int g_cmt_n;
static rating_t       g_rat[MAX_CMT];        static unsigned int g_rat_n;
static issue_t        g_iss[MAX_ISS];        static unsigned int g_iss_n;
static best_practice_t g_bp[MAX_CMT];        static unsigned int g_bp_n;
static notification_t g_notif[MAX_CMT];      static unsigned int g_notif_n;
static unsigned int   g_rev_dev_bps = 7000, g_rev_plat_bps = 3000, g_next_id;

static void *mem_alloc(unsigned int sz) {
    unsigned int p = host_alloc(sz, 8);
    if (!p) { host_print("[ERROR] alloc failed\n"); host_exit(1); }
    return (void *)p;
}
static unsigned int next_id(void) { return ++g_next_id; }

static char g_uitoa_buf[16];
static const char *uitoa(unsigned int n) {
    int i = 0;
    if (n == 0) { g_uitoa_buf[0] = '0'; g_uitoa_buf[1] = '\0'; return g_uitoa_buf; }
    char tmp[16]; int tc = 0;
    while (n > 0) { tmp[tc++] = '0' + (n % 10); n /= 10; }
    while (tc > 0) g_uitoa_buf[i++] = tmp[--tc];
    g_uitoa_buf[i] = '\0';
    return g_uitoa_buf;
}
static void print_uint(unsigned int n) { host_print(uitoa(n)); }
static void copy_str(char *d, const char *s, unsigned int mx) {
    unsigned int i = 0;
    while (i < mx - 1 && s[i]) { d[i] = s[i]; i++; }
    d[i] = '\0';
}
static int str_eq(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return 0; a++; b++; }
    return *a == *b;
}
static int starts_with(const char *s, const char *p) {
    while (*p) { if (*s != *p) return 0; s++; p++; }
    return 1;
}
static unsigned int parse_uint(const char *s) {
    unsigned int v = 0;
    while (*s >= '0' && *s <= '9') { v = v * 10 + (unsigned int)(*s - '0'); s++; }
    return v;
}
static void sys_init(void) {
    g_cat_n = g_api_n = g_dev_n = g_app_n = g_key_n = g_oa_n = 0;
    g_plan_n = g_sub_n = g_ana_n = g_bill_n = g_cmt_n = g_rat_n = 0;
    g_iss_n = g_bp_n = g_notif_n = g_next_id = 0;
    g_rev_dev_bps = 7000; g_rev_plat_bps = 3000;
    host_print("[INIT] API Marketplace module initialized\n");
}

/* ===== Forward Declarations ===== */
static void handle_command(const char *cmd);
static void cmd_register_api(const char *a);
static void cmd_list_apis(void);
static void cmd_add_category(const char *a);
static void cmd_add_tag(const char *a);
static void cmd_add_version(void);
static void cmd_add_sample(const char *a);
static void cmd_register_dev(const char *a);
static void cmd_create_app(const char *a);
static void cmd_generate_apikey(void);
static void cmd_revoke_apikey(const char *a);
static void cmd_create_oauth(const char *a);
static void cmd_list_devs(void);
static void cmd_subscribe(void);
static void cmd_approve_sub(unsigned int id);
static void cmd_reject_sub(unsigned int id);
static void cmd_create_plan(const char *a);
static void cmd_list_plans(void);
static void cmd_check_quota(unsigned int id);
static void cmd_record_call(unsigned int api_id, unsigned int lat, unsigned int err);
static void cmd_get_api_stats(unsigned int api_id);
static void cmd_get_top_apis(unsigned int ct);
static void cmd_get_dev_ranking(unsigned int ct);
static void cmd_gen_bill(unsigned int sub_id, unsigned int ps, unsigned int pe);
static void cmd_pay_bill(unsigned int bill_id);
static void cmd_set_rev_share(unsigned int dev, unsigned int plat);
static void cmd_add_comment(const char *a);
static void cmd_add_rating(const char *a);
static void cmd_report_issue(const char *a);
static void cmd_share_bp(const char *a);
static void cmd_send_notif(unsigned int api_id, unsigned int vi, const char *msg);
static void cmd_list_comments(unsigned int api_id);
static void cmd_list_ratings(unsigned int api_id);
static subscription_t *find_sub(unsigned int id);
static unsigned int compute_total_calls(unsigned int api_id);
static unsigned int calc_bill_amount(unsigned int sub_id, unsigned int calls);
static float compute_avg_rating(unsigned int api_id);

/* ===== API Catalog ===== */
static void cmd_register_api(const char *a) {
    if (g_api_n >= MAX_API) { host_print("[ERROR] max APIs\n"); return; }
    api_entry_t *e = &g_api[g_api_n];
    e->id = next_id(); copy_str(e->name, a, NAME_LEN); copy_str(e->desc, "New API", DESC_LEN);
    e->cat_id = 0; e->tag_ct = 0; e->ver_ct = 0; e->samp_ct = 0;
    e->dev_id = 0; e->active = 1; e->created = 1000000; copy_str(e->spec, "{}", DESC_LEN);
    g_api_n++; host_print("[CATALOG] API registered: "); host_print(e->name); host_print("\n");
}
static void cmd_list_apis(void) {
    host_print("[CATALOG] APIs:\n");
    for (unsigned int i = 0; i < g_api_n; i++)
        if (g_api[i].active) {
            host_print("  #"); print_uint(g_api[i].id); host_print(" "); host_print(g_api[i].name);
            host_print(" ver="); print_uint(g_api[i].ver_ct);
            host_print(" samp="); print_uint(g_api[i].samp_ct); host_print("\n");
        }
    host_print("  Total: "); print_uint(g_api_n); host_print("\n");
}
static void cmd_add_category(const char *a) {
    if (g_cat_n >= MAX_CAT) { host_print("[ERROR] max categories\n"); return; }
    category_t *c = &g_cat[g_cat_n]; c->id = next_id(); copy_str(c->name, a, NAME_LEN);
    c->api_count = 0; c->active = 1; g_cat_n++;
    host_print("[CATALOG] Category: "); host_print(c->name); host_print("\n");
}
static void cmd_add_tag(const char *a) {
    host_print("[CATALOG] Tag added: "); host_print(a); host_print("\n");
}
static void cmd_add_version(void) {
    if (!g_api_n) { host_print("[ERROR] no APIs\n"); return; }
    api_entry_t *e = &g_api[g_api_n - 1];
    if (e->ver_ct >= MAX_VER) { host_print("[ERROR] max versions\n"); return; }
    e->major[e->ver_ct] = 1; e->minor[e->ver_ct] = 0; e->ver_ct++;
    host_print("[CATALOG] v1.0 added to "); host_print(e->name); host_print("\n");
}
static void cmd_add_sample(const char *a) {
    if (!g_api_n) { host_print("[ERROR] no APIs\n"); return; }
    api_entry_t *e = &g_api[g_api_n - 1];
    if (e->samp_ct >= MAX_SAMP) { host_print("[ERROR] max samples\n"); return; }
    copy_str(e->samples[e->samp_ct++], a, DESC_LEN);
    host_print("[CATALOG] Sample added to "); host_print(e->name); host_print("\n");
}

/* ===== Developer Management ===== */
static void cmd_register_dev(const char *a) {
    if (g_dev_n >= MAX_DEV) { host_print("[ERROR] max devs\n"); return; }
    developer_t *d = &g_dev[g_dev_n]; d->id = next_id(); copy_str(d->name, a, NAME_LEN);
    d->active = 1; g_dev_n++;
    host_print("[DEV] Registered: "); host_print(d->name); host_print("\n");
}
static void cmd_create_app(const char *a) {
    if (g_app_n >= MAX_APP) { host_print("[ERROR] max apps\n"); return; }
    app_t *ap = &g_app[g_app_n]; ap->id = next_id(); copy_str(ap->name, a, NAME_LEN);
    ap->dev_id = g_dev_n ? g_dev[g_dev_n-1].id : 0;
    copy_str(ap->redirect, "https://localhost/cb", URL_LEN);
    ap->ts = 1000000; ap->active = 1; g_app_n++;
    host_print("[DEV] App: "); host_print(ap->name); host_print("\n");
}
static void cmd_generate_apikey(void) {
    if (g_key_n >= MAX_KEY) { host_print("[ERROR] max keys\n"); return; }
    api_key_t *k = &g_keys[g_key_n]; k->id = next_id();
    copy_str(k->key, "ak_0000000000000000000000000000DEAD", KEY_LEN);
    k->app_id = g_app_n ? g_app[g_app_n-1].id : 0;
    k->dev_id = g_dev_n ? g_dev[g_dev_n-1].id : 0;
    k->api_id = 0; k->created = 1000000; k->expires = 2000000; k->revoked = 0; g_key_n++;
    host_print("[DEV] API Key: "); host_print(k->key); host_print("\n");
}
static void cmd_revoke_apikey(const char *a) {
    unsigned int kid = parse_uint(a);
    for (unsigned int i = 0; i < g_key_n; i++)
        if (g_keys[i].id == kid) { g_keys[i].revoked = 1; host_print("[DEV] Key revoked\n"); return; }
    host_print("[ERROR] key not found\n");
}
static void cmd_create_oauth(const char *a) {
    (void)a;
    if (g_oa_n >= MAX_OA) { host_print("[ERROR] max oauth\n"); return; }
    oauth_app_t *o = &g_oa[g_oa_n]; o->id = next_id();
    o->app_id = g_app_n ? g_app[g_app_n-1].id : 0;
    copy_str(o->cid, "cid_AABBCCDD00112233", KEY_LEN);
    copy_str(o->csec, "sec_FF00FF00FF00FF00EE", KEY_LEN);
    copy_str(o->redirect, "https://localhost/cb", URL_LEN);
    copy_str(o->scope, "read write", DESC_LEN);
    o->ts = 1000000; o->active = 1; g_oa_n++;
    host_print("[DEV] OAuth app, client_id: "); host_print(o->cid); host_print("\n");
}
static void cmd_list_devs(void) {
    host_print("[DEV] Developers:\n");
    for (unsigned int i = 0; i < g_dev_n; i++)
        if (g_dev[i].active) { host_print("  #"); print_uint(g_dev[i].id); host_print(" "); host_print(g_dev[i].name); host_print("\n"); }
}

/* ===== Subscription & Approval ===== */
static subscription_t *find_sub(unsigned int id) {
    for (unsigned int i = 0; i < g_sub_n; i++) if (g_sub[i].id == id) return &g_sub[i];
    return (subscription_t *)0;
}
static void cmd_subscribe(void) {
    if (g_sub_n >= MAX_SUB) { host_print("[ERROR] max subs\n"); return; }
    subscription_t *s = &g_sub[g_sub_n]; s->id = next_id();
    s->dev_id = g_dev_n ? g_dev[g_dev_n-1].id : 0;
    s->api_id = g_api_n ? g_api[g_api_n-1].id : 0;
    s->plan_id = g_plan_n ? g_plan[0].id : 0;
    s->status = ST_PENDING; s->approval = 0; s->req_ts = 1000000; s->app_ts = 0;
    s->calls = 0; s->quota = 5000; s->active = 1;
    s->status = ST_APPROVED; s->app_ts = 1000001; /* auto-approve */
    g_sub_n++;
    host_print("[SUB] Auto-approved, id: "); print_uint(s->id); host_print("\n");
}
static void cmd_approve_sub(unsigned int id) {
    subscription_t *s = find_sub(id);
    if (!s) { host_print("[ERROR] sub not found\n"); return; }
    s->status = ST_APPROVED; s->app_ts = 1000100;
    host_print("[SUB] Approved #"); print_uint(id); host_print("\n");
}
static void cmd_reject_sub(unsigned int id) {
    subscription_t *s = find_sub(id);
    if (!s) { host_print("[ERROR] sub not found\n"); return; }
    s->status = ST_REJECTED; host_print("[SUB] Rejected #"); print_uint(id); host_print("\n");
}
static void cmd_create_plan(const char *a) {
    if (g_plan_n >= MAX_PLAN) { host_print("[ERROR] max plans\n"); return; }
    plan_t *p = &g_plan[g_plan_n]; p->id = next_id(); copy_str(p->name, a, NAME_LEN);
    p->plan_type = PLAN_PRO; p->rate_limit = 5000; p->price_cents = 999;
    p->billing = BILLING_MON; p->tier_ct = 0; p->active = 1; g_plan_n++;
    host_print("[PLAN] Created: "); host_print(p->name); host_print("\n");
}
static void cmd_list_plans(void) {
    host_print("[PLAN] Plans:\n");
    for (unsigned int i = 0; i < g_plan_n; i++)
        if (g_plan[i].active) {
            host_print("  #"); print_uint(g_plan[i].id); host_print(" "); host_print(g_plan[i].name);
            host_print(" $"); print_uint(g_plan[i].price_cents); host_print("c\n");
        }
}
static void cmd_check_quota(unsigned int id) {
    subscription_t *s = find_sub(id);
    if (!s) { host_print("[ERROR] sub not found\n"); return; }
    host_print("[QUOTA] Sub #"); print_uint(id); host_print(": ");
    print_uint(s->calls); host_print("/"); print_uint(s->quota); host_print("\n");
}

/* ===== Usage Analytics ===== */
static void cmd_record_call(unsigned int api_id, unsigned int lat, unsigned int err) {
    if (g_ana_n >= MAX_ANA) { host_print("[ERROR] analytics full\n"); return; }
    /* Aggregate into existing record if same api_id, else new */
    for (unsigned int i = 0; i < g_ana_n; i++) {
        if (g_ana[i].api_id == api_id) {
            g_ana[i].calls++; if (err) g_ana[i].errors++;
            g_ana[i].latency_sum += lat; return;
        }
    }
    g_ana[g_ana_n].api_id = api_id; g_ana[g_ana_n].calls = 1;
    g_ana[g_ana_n].errors = err; g_ana[g_ana_n].latency_sum = lat; g_ana_n++;
    for (unsigned int i = 0; i < g_sub_n; i++)
        if (g_sub[i].api_id == api_id && g_sub[i].status == ST_APPROVED) { g_sub[i].calls++; break; }
}
static unsigned int compute_total_calls(unsigned int api_id) {
    unsigned int t = 0;
    for (unsigned int i = 0; i < g_ana_n; i++) if (g_ana[i].api_id == api_id) t += g_ana[i].calls;
    return t;
}
static void cmd_get_api_stats(unsigned int api_id) {
    unsigned int total = compute_total_calls(api_id), errs = 0;
    unsigned long long lat_sum = 0;
    for (unsigned int i = 0; i < g_ana_n; i++)
        if (g_ana[i].api_id == api_id) { errs += g_ana[i].errors; lat_sum += g_ana[i].latency_sum; }
    host_print("[STATS] API #"); print_uint(api_id); host_print(": calls=");
    print_uint(total); host_print(" errors="); print_uint(errs);
    host_print(" avg_lat="); if (total) print_uint((unsigned int)(lat_sum / total)); else host_print("0");
    host_print("ms\n");
}
static void cmd_get_top_apis(unsigned int ct) {
    host_print("[STATS] Top APIs:\n"); unsigned int shown = 0;
    for (unsigned int i = 0; i < g_api_n && shown < ct; i++) {
        unsigned int t = compute_total_calls(g_api[i].id);
        if (t > 0) { host_print("  "); host_print(g_api[i].name); host_print(" calls="); print_uint(t); host_print("\n"); shown++; }
    }
}
static void cmd_get_dev_ranking(unsigned int ct) {
    host_print("[STATS] Dev ranking:\n"); unsigned int shown = 0;
    for (unsigned int i = 0; i < g_dev_n && shown < ct; i++, shown++)
        { host_print("  "); host_print(g_dev[i].name); host_print("\n"); }
}

/* ===== Monetization ===== */
static unsigned int calc_bill_amount(unsigned int sub_id, unsigned int calls) {
    subscription_t *s = find_sub(sub_id);
    if (!s) return 0;
    for (unsigned int i = 0; i < g_plan_n; i++) {
        if (g_plan[i].id != s->plan_id) continue;
        if (g_plan[i].billing == BILLING_MON) return g_plan[i].price_cents;
        if (g_plan[i].billing == BILLING_PPU) return calls;
        if (g_plan[i].billing == BILLING_TIER) {
            unsigned int amt = 0, rem = calls;
            for (unsigned int t = 0; t < g_plan[i].tier_ct && rem > 0; t++) {
                unsigned int use = rem < (unsigned int)g_plan[i].tier_thresh[t] ? rem : (unsigned int)g_plan[i].tier_thresh[t];
                amt += (unsigned int)(use * g_plan[i].tier_price[t]); rem -= use;
            }
            return amt;
        }
    }
    return 0;
}
static void cmd_gen_bill(unsigned int sub_id, unsigned int ps, unsigned int pe) {
    if (g_bill_n >= MAX_BILL) { host_print("[ERROR] max bills\n"); return; }
    subscription_t *s = find_sub(sub_id);
    if (!s) { host_print("[ERROR] sub not found\n"); return; }
    bill_t *b = &g_bill[g_bill_n]; b->id = next_id();
    b->dev_id = s->dev_id; b->sub_id = sub_id; b->plan_id = s->plan_id;
    b->p_start = ps; b->p_end = pe; b->calls_in_p = s->calls;
    b->billing = BILLING_MON; b->unit_price = 0.01f;
    b->amount = calc_bill_amount(sub_id, s->calls); b->paid = 0; b->ts = 1000000;
    g_bill_n++;
    host_print("[BILL] Generated #"); print_uint(b->id); host_print(" amt=");
    print_uint(b->amount); host_print("c\n");
}
static void cmd_pay_bill(unsigned int bill_id) {
    for (unsigned int i = 0; i < g_bill_n; i++)
        if (g_bill[i].id == bill_id) { g_bill[i].paid = 1; host_print("[BILL] Paid #"); print_uint(bill_id); host_print("\n"); return; }
    host_print("[ERROR] bill not found\n");
}
static void cmd_set_rev_share(unsigned int dev, unsigned int plat) {
    if (dev + plat != 10000) { host_print("[ERROR] must total 10000 bps\n"); return; }
    g_rev_dev_bps = dev; g_rev_plat_bps = plat;
    host_print("[BILL] Rev share: dev="); print_uint(dev); host_print(" plat="); print_uint(plat); host_print(" bps\n");
}

/* ===== Community ===== */
static void cmd_add_comment(const char *a) {
    if (g_cmt_n >= MAX_CMT) { host_print("[ERROR] max comments\n"); return; }
    comment_t *c = &g_cmt[g_cmt_n]; c->id = next_id();
    c->api_id = g_api_n ? g_api[g_api_n-1].id : 0;
    c->dev_id = g_dev_n ? g_dev[g_dev_n-1].id : 0;
    copy_str(c->content, a, DESC_LEN); c->ts = 1000000; g_cmt_n++;
    host_print("[COMMUNITY] Comment added\n");
}
static void cmd_add_rating(const char *a) {
    if (g_rat_n >= MAX_CMT) { host_print("[ERROR] max ratings\n"); return; }
    unsigned int sc = parse_uint(a); if (sc > 5) sc = 5; if (!sc) sc = 1;
    rating_t *r = &g_rat[g_rat_n]; r->id = next_id();
    r->api_id = g_api_n ? g_api[g_api_n-1].id : 0;
    r->dev_id = g_dev_n ? g_dev[g_dev_n-1].id : 0;
    r->score = sc; copy_str(r->review, "", DESC_LEN); r->ts = 1000000; g_rat_n++;
    host_print("[COMMUNITY] Rating: "); print_uint(sc); host_print("/5\n");
}
static void cmd_report_issue(const char *a) {
    if (g_iss_n >= MAX_ISS) { host_print("[ERROR] max issues\n"); return; }
    issue_t *is = &g_iss[g_iss_n]; is->id = next_id();
    is->api_id = g_api_n ? g_api[g_api_n-1].id : 0;
    is->dev_id = g_dev_n ? g_dev[g_dev_n-1].id : 0;
    copy_str(is->title, a, NAME_LEN); copy_str(is->desc, "Issue reported", DESC_LEN);
    is->sev = 1; is->resolved = 0; is->ts = 1000000; g_iss_n++;
    host_print("[COMMUNITY] Issue: "); host_print(is->title); host_print("\n");
}
static void cmd_share_bp(const char *a) {
    if (g_bp_n >= MAX_CMT) { host_print("[ERROR] max bps\n"); return; }
    best_practice_t *bp = &g_bp[g_bp_n];
    bp->dev_id = g_dev_n ? g_dev[g_dev_n-1].id : 0;
    bp->api_id = g_api_n ? g_api[g_api_n-1].id : 0;
    copy_str(bp->practice, a, DESC_LEN); bp->ts = 1000000; g_bp_n++;
    host_print("[COMMUNITY] Best practice shared\n");
}
static void cmd_send_notif(unsigned int api_id, unsigned int vi, const char *msg) {
    if (g_notif_n >= MAX_CMT) { host_print("[ERROR] max notifs\n"); return; }
    notification_t *n = &g_notif[g_notif_n]; n->id = next_id();
    n->api_id = api_id; n->ver_idx = vi; copy_str(n->msg, msg, DESC_LEN); n->ts = 1000000; g_notif_n++;
    host_print("[COMMUNITY] Notification for API #"); print_uint(api_id); host_print("\n");
}
static void cmd_list_comments(unsigned int api_id) {
    host_print("[COMMUNITY] Comments for API #"); print_uint(api_id); host_print(":\n");
    for (unsigned int i = 0; i < g_cmt_n; i++)
        if (g_cmt[i].api_id == api_id) { host_print("  - "); host_print(g_cmt[i].content); host_print("\n"); }
}
static void cmd_list_ratings(unsigned int api_id) {
    host_print("[COMMUNITY] Ratings for API #"); print_uint(api_id); host_print(":\n");
    for (unsigned int i = 0; i < g_rat_n; i++)
        if (g_rat[i].api_id == api_id) { host_print("  - "); print_uint(g_rat[i].score); host_print("/5\n"); }
}
static float compute_avg_rating(unsigned int api_id) {
    unsigned int sum = 0, ct = 0;
    for (unsigned int i = 0; i < g_rat_n; i++)
        if (g_rat[i].api_id == api_id) { sum += g_rat[i].score; ct++; }
    return ct ? (float)sum / (float)ct : 0.0f;
}

/* ===== Command Router ===== */
static void handle_command(const char *cmd) {
    if (starts_with(cmd, "register_api "))  { cmd_register_api(cmd + 13); return; }
    if (str_eq(cmd, "list_apis"))          { cmd_list_apis(); return; }
    if (starts_with(cmd, "add_cat "))       { cmd_add_category(cmd + 8); return; }
    if (starts_with(cmd, "add_tag "))       { cmd_add_tag(cmd + 8); return; }
    if (str_eq(cmd, "add_ver"))             { cmd_add_version(); return; }
    if (starts_with(cmd, "add_sample "))    { cmd_add_sample(cmd + 11); return; }
    if (starts_with(cmd, "reg_dev "))       { cmd_register_dev(cmd + 8); return; }
    if (starts_with(cmd, "create_app "))    { cmd_create_app(cmd + 11); return; }
    if (str_eq(cmd, "gen_key"))             { cmd_generate_apikey(); return; }
    if (starts_with(cmd, "revoke_key "))    { cmd_revoke_apikey(cmd + 11); return; }
    if (str_eq(cmd, "create_oauth"))        { cmd_create_oauth(cmd); return; }
    if (str_eq(cmd, "list_devs"))           { cmd_list_devs(); return; }
    if (str_eq(cmd, "subscribe"))           { cmd_subscribe(); return; }
    if (starts_with(cmd, "approve "))       { cmd_approve_sub(parse_uint(cmd + 8)); return; }
    if (starts_with(cmd, "reject "))        { cmd_reject_sub(parse_uint(cmd + 7)); return; }
    if (starts_with(cmd, "create_plan "))   { cmd_create_plan(cmd + 12); return; }
    if (str_eq(cmd, "list_plans"))          { cmd_list_plans(); return; }
    if (starts_with(cmd, "quota "))         { cmd_check_quota(parse_uint(cmd + 6)); return; }
    if (str_eq(cmd, "record_call"))         { cmd_record_call(1, 150, 0); return; }
    if (starts_with(cmd, "stats "))         { cmd_get_api_stats(parse_uint(cmd + 6)); return; }
    if (starts_with(cmd, "top "))           { cmd_get_top_apis(parse_uint(cmd + 4)); return; }
    if (starts_with(cmd, "rank "))          { cmd_get_dev_ranking(parse_uint(cmd + 5)); return; }
    if (starts_with(cmd, "bill "))          { cmd_gen_bill(parse_uint(cmd + 5), 1000000, 2000000); return; }
    if (starts_with(cmd, "pay "))           { cmd_pay_bill(parse_uint(cmd + 4)); return; }
    if (starts_with(cmd, "comment "))       { cmd_add_comment(cmd + 8); return; }
    if (starts_with(cmd, "rate "))          { cmd_add_rating(cmd + 5); return; }
    if (starts_with(cmd, "issue "))         { cmd_report_issue(cmd + 6); return; }
    if (starts_with(cmd, "bp "))            { cmd_share_bp(cmd + 3); return; }
    if (str_eq(cmd, "rev_share"))           { cmd_set_rev_share(7000, 3000); return; }
    if (str_eq(cmd, "help")) {
        host_print("[HELP] register_api list_apis add_cat add_tag add_ver add_sample\n");
        host_print("  reg_dev create_app gen_key revoke_key create_oauth list_devs\n");
        host_print("  subscribe approve reject create_plan list_plans quota\n");
        host_print("  record_call stats top rank bill pay comment rate issue bp rev_share\n");
        return;
    }
    host_print("[ERROR] Unknown: "); host_print(cmd); host_print("\n");
}

/* ===== Entry Point ===== */
__attribute__((export_name("main")))
int main(void) {
    sys_init();
    char *buf = (char *)mem_alloc(4096);
    int alen = host_get_argv(0, 4096);
    if (alen > 0 && alen < 4096) {
        host_get_argv((unsigned int)(size_t)buf, 4096);
        host_print("[MARKET] Cmd: "); host_print(buf); host_print("\n");
        handle_command(buf);
        return 0;
    }
    host_print("[MARKET] No command. Type 'help'. Running demo...\n");
    cmd_add_category("AI/ML"); cmd_add_category("Data");
    cmd_add_tag("nlp"); cmd_add_tag("vision");
    cmd_register_api("TextSummarizer"); cmd_add_version(); cmd_add_sample("POST /v1/summarize");
    cmd_register_dev("Alice"); cmd_create_app("MyApp"); cmd_generate_apikey(); cmd_create_oauth("");
    cmd_create_plan("ProPlan"); cmd_subscribe(); cmd_check_quota(2);
    cmd_record_call(1, 120, 0); cmd_record_call(1, 200, 0); cmd_record_call(1, 95, 1);
    cmd_get_api_stats(1); cmd_get_top_apis(3); cmd_get_dev_ranking(3);
    cmd_set_rev_share(7000, 3000); cmd_gen_bill(2, 1000000, 2000000); cmd_pay_bill(1);
    cmd_add_comment("Great API!"); cmd_add_rating("5");
    cmd_report_issue("Rate limit issues"); cmd_share_bp("Cache batch responses");
    cmd_send_notif(1, 0, "v1.1.0 released!"); cmd_list_comments(1); cmd_list_ratings(1);
    (void)compute_avg_rating(1);
    host_print("\n[MARKET] Done.\n");
    return 0;
}
