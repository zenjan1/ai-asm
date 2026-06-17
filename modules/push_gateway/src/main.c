/* push_gateway: Unified push gateway with multi-vendor routing (v1.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* ─── Constants ─── */
#define MAX_VENDORS    8
#define MAX_DEVICES   32
#define MAX_ALIASES   16
#define MAX_TAGS      16
#define MAX_CHANNELS  16
#define MAX_MSGS      32
#define MAX_QUEUE     64
#define MAX_STATS     16
#define MAX_RULES     16
#define MAX_BUTTONS    4
#define MAX_MEDIA      4
#define MAX_NAME      32
#define MAX_TOKEN     64
#define MAX_BODY     256
#define MAX_TAG       24
#define MAX_REGION    16
#define BUF_SIZE    4096
/* Vendor IDs */
#define VENDOR_APNS    0
#define VENDOR_FCM     1
#define VENDOR_HUAWEI  2
#define VENDOR_XIAOMI  3
#define VENDOR_OPPO    4
#define VENDOR_VIVO    5
#define VENDOR_NONE   99
/* Message types */
#define MSG_NOTIFY      0
#define MSG_TRANSPARENT 1
#define MSG_RICH_MEDIA  2
#define MSG_INTERACTIVE 3
/* Media types */
#define MEDIA_IMAGE  0
#define MEDIA_AUDIO  1
#define MEDIA_VIDEO  2
/* Priority levels */
#define PRI_LOW      0
#define PRI_NORMAL   1
#define PRI_HIGH     2
#define PRI_URGENT   3
/* Network types */
#define NET_WIFI     0
#define NET_4G       1
#define NET_5G       2
#define NET_OFFLINE  3
/* Routing strategies */
#define ROUTE_AUTO      0
#define ROUTE_COST_OPT  1
#define ROUTE_SPEED_OPT 2
#define ROUTE_RELIABLE  3
/* Compliance actions */
#define COMPL_ALLOW  0
#define COMPL_BLOCK  1
#define COMPL_DEFER  2
/* Delivery status */
#define DEL_PENDING  0
#define DEL_SENT     1
#define DEL_ARRIVED  2
#define DEL_CLICKED  3
#define DEL_FAILED   4
#define DEL_EXPIRED  5

/* ─── Typedefs ─── */
typedef struct { int id; char name[MAX_NAME]; int enabled;
    int qps_limit; int qps_cur; unsigned int last_tick;
    unsigned long sent; unsigned long arrived; unsigned long failed;
    int cost_per_1k; int avg_latency_ms; int reliability; } vendor_t;

typedef struct { char dev_id[MAX_TOKEN]; int vendor; char token[MAX_TOKEN];
    char alias[MAX_NAME]; int active; int net_type;
    char region[MAX_REGION]; unsigned int reg_at; } device_t;

typedef struct { char alias[MAX_NAME]; char dev_ids[MAX_DEVICES][MAX_TOKEN];
    int dev_n; int active; } alias_map_t;

typedef struct { char name[MAX_TAG]; char dev_ids[MAX_DEVICES][MAX_TOKEN];
    int dev_n; } tag_t;

typedef struct { char name[MAX_NAME]; int vendor_filter;
    int enabled; int pri; } channel_t;

typedef struct { char url[MAX_BODY]; int type; int idx; } media_t;

typedef struct { char label[MAX_NAME]; char action[MAX_BODY];
    int idx; } button_t;

typedef struct { unsigned long id; int type; char title[MAX_NAME];
    char body[MAX_BODY]; char channel[MAX_NAME];
    int pri; int vendor; char target_alias[MAX_NAME];
    char target_tag[MAX_TAG]; int media_n; media_t media[MAX_MEDIA];
    int btn_n; button_t btns[MAX_BUTTONS];
    unsigned int created; unsigned int expire;
    int status; int delivered; int clicked; } push_msg_t;

typedef struct { push_msg_t msg; int target_dev; int vendor_used;
    int status; unsigned int sent_at; unsigned int arrived_at;
    unsigned int clicked_at; int retries; } delivery_t;

typedef struct { char vendor[MAX_NAME]; unsigned long total;
    unsigned long arrived; unsigned long clicked; unsigned long failed;
    int avg_latency; } vendor_stats_t;

typedef struct { int vendor; int min_pri; char regions[MAX_NAME];
    int active; } route_rule_t;

typedef struct { char dev_id[MAX_TOKEN]; unsigned int count;
    unsigned int window_start; int blocked;
    unsigned int unsub_at; int opted_out; } compliance_t;

/* ─── Globals ─── */
static unsigned int clk = 0;
static int vendor_n = 0, dev_n = 0, alias_n = 0, tag_n = 0;
static int chan_n = 0, msg_n = 0, queue_n = 0, stats_n = 0;
static int rule_n = 0, compl_n = 0;
static int route_strategy = ROUTE_AUTO;
static unsigned long total_pushed = 0, total_arrived = 0;
static unsigned long total_clicked = 0, total_failed = 0;
static unsigned long msg_id_seq = 1000;

static vendor_t vendors[MAX_VENDORS];
static device_t devices[MAX_DEVICES];
static alias_map_t aliases[MAX_ALIASES];
static tag_t tags[MAX_TAGS];
static channel_t channels[MAX_CHANNELS];
static push_msg_t messages[MAX_MSGS];
static delivery_t queue[MAX_QUEUE];
static vendor_stats_t vstats[MAX_STATS];
static route_rule_t rules[MAX_RULES];
static compliance_t compliances[MAX_DEVICES];

/* ─── Forward declarations ─── */
static void log_str(const char *s);
static void log_num(int n);
static void log_ulong(unsigned long n);
static int find_vendor(const char *name);
static int find_device(const char *dev_id);
static int find_channel(const char *name);
static int find_compliance(const char *dev_id);
static int hash_str(const char *s);
static int check_rate_limit(vendor_t *v);
static int check_compliance(const char *dev_id, int channel_idx);
static int select_vendor(int pri, const char *region);
static void enqueue_delivery(int msg_idx, int dev_idx, int vendor_id);
static void process_queue(void);
static void update_stats(int vendor_id, int status);

/* ─── Utility functions ─── */
static void log_str(const char *s) { host_print(s); }

static void log_num(int n)
{
    char buf[16];
    int i = 14, neg = 0;
    unsigned int u;
    buf[15] = '\0';
    if (n < 0) { neg = 1; u = (unsigned int)(-(n + 1)) + 1U; }
    else { u = (unsigned int)n; }
    if (u == 0) { buf[--i] = '0'; }
    else { while (u > 0 && i > 0) { buf[--i] = '0' + (char)(u % 10); u /= 10; } }
    if (neg && i > 0) buf[--i] = '-';
    host_print(&buf[i]);
}

static void log_ulong(unsigned long n)
{
    char buf[24];
    int i = 23;
    buf[23] = '\0';
    if (n == 0) { buf[--i] = '0'; }
    else { while (n > 0 && i > 0) { buf[--i] = '0' + (char)(n % 10); n /= 10; } }
    host_print(&buf[i]);
}

static int hash_str(const char *s)
{
    int h = 5381;
    while (*s) { h = ((h << 5) + h) + (int)*s; s++; }
    return h & 0x7FFFFFFF;
}

static int str_eq(const char *a, const char *b)
{
    while (*a && *b) { if (*a != *b) return 0; a++; b++; }
    return (*a == *b) ? 1 : 0;
}

static void str_copy(char *dst, const char *src, int max)
{
    int i = 0;
    while (i < max - 1 && src[i]) { dst[i] = src[i]; i++; }
    dst[i] = '\0';
}

static void mem_zero(void *p, int sz)
{
    char *c = (char *)p;
    int i;
    for (i = 0; i < sz; i++) c[i] = '\0';
}

/* ─── Vendor management ─── */
static int find_vendor(const char *name)
{
    int i;
    for (i = 0; i < vendor_n; i++) {
        if (str_eq(vendors[i].name, name)) return i;
    }
    return -1;
}

int vendor_register(const char *name, int qps_limit, int cost_per_1k, int reliability)
{
    if (vendor_n >= MAX_VENDORS) { log_str("vendor limit\n"); return -1; }
    if (find_vendor(name) >= 0) { log_str("vendor exists\n"); return -1; }
    vendor_t *v = &vendors[vendor_n];
    mem_zero(v, sizeof(vendor_t));
    str_copy(v->name, name, MAX_NAME);
    v->id = vendor_n;
    v->enabled = 1;
    v->qps_limit = qps_limit;
    v->qps_cur = 0;
    v->last_tick = clk;
    v->cost_per_1k = cost_per_1k;
    v->avg_latency_ms = 200;
    v->reliability = reliability;
    log_str("vendor registered: "); log_str(name);
    log_str(" qps="); log_num(qps_limit);
    log_str(" rel="); log_num(reliability); log_str("\n");
    return vendor_n++;
}

int vendor_set_enabled(const char *name, int enabled)
{
    int idx = find_vendor(name);
    if (idx < 0) return -1;
    vendors[idx].enabled = enabled;
    log_str("vendor "); log_str(name);
    log_str(enabled ? " enabled\n" : " disabled\n");
    return 0;
}

/* ─── Device management ─── */
static int find_device(const char *dev_id)
{
    int i;
    for (i = 0; i < dev_n; i++) {
        if (str_eq(devices[i].dev_id, dev_id)) return i;
    }
    return -1;
}

int device_register(const char *dev_id, int vendor, const char *token,
                    const char *region)
{
    if (dev_n >= MAX_DEVICES) { log_str("device limit\n"); return -1; }
    if (find_device(dev_id) >= 0) { log_str("device exists\n"); return -1; }
    device_t *d = &devices[dev_n];
    mem_zero(d, sizeof(device_t));
    str_copy(d->dev_id, dev_id, MAX_TOKEN);
    d->vendor = vendor;
    str_copy(d->token, token, MAX_TOKEN);
    str_copy(d->region, region, MAX_REGION);
    d->active = 1;
    d->net_type = NET_WIFI;
    d->reg_at = clk;
    log_str("device registered: "); log_str(dev_id);
    log_str(" vendor="); log_num(vendor); log_str("\n");
    return dev_n++;
}

int device_set_network(const char *dev_id, int net_type)
{
    int idx = find_device(dev_id);
    if (idx < 0) return -1;
    devices[idx].net_type = net_type;
    return 0;
}

int device_set_active(const char *dev_id, int active)
{
    int idx = find_device(dev_id);
    if (idx < 0) return -1;
    devices[idx].active = active;
    log_str("device "); log_str(dev_id);
    log_str(active ? " active\n" : " inactive\n");
    return 0;
}

/* ─── Alias management ─── */
int alias_create(const char *alias_name)
{
    if (alias_n >= MAX_ALIASES) return -1;
    alias_map_t *a = &aliases[alias_n];
    mem_zero(a, sizeof(alias_map_t));
    str_copy(a->alias, alias_name, MAX_NAME);
    a->dev_n = 0;
    a->active = 1;
    log_str("alias created: "); log_str(alias_name); log_str("\n");
    return alias_n++;
}

int alias_bind(const char *alias_name, const char *dev_id)
{
    int i;
    int ai = -1;
    for (i = 0; i < alias_n; i++) {
        if (str_eq(aliases[i].alias, alias_name)) { ai = i; break; }
    }
    if (ai < 0) { ai = alias_create(alias_name); }
    if (ai < 0) return -1;
    alias_map_t *a = &aliases[ai];
    if (a->dev_n >= MAX_DEVICES) return -1;
    if (find_device(dev_id) < 0) { log_str("dev not found\n"); return -1; }
    str_copy(a->dev_ids[a->dev_n], dev_id, MAX_TOKEN);
    a->dev_n++;
    log_str("alias bind: "); log_str(alias_name);
    log_str(" <- "); log_str(dev_id); log_str("\n");
    return 0;
}

/* ─── Tag management ─── */
int tag_create(const char *tag_name)
{
    if (tag_n >= MAX_TAGS) return -1;
    tag_t *t = &tags[tag_n];
    mem_zero(t, sizeof(tag_t));
    str_copy(t->name, tag_name, MAX_TAG);
    t->dev_n = 0;
    log_str("tag created: "); log_str(tag_name); log_str("\n");
    return tag_n++;
}

int tag_add_device(const char *tag_name, const char *dev_id)
{
    int i, ti = -1;
    for (i = 0; i < tag_n; i++) {
        if (str_eq(tags[i].name, tag_name)) { ti = i; break; }
    }
    if (ti < 0) { ti = tag_create(tag_name); }
    if (ti < 0) return -1;
    tag_t *t = &tags[ti];
    if (t->dev_n >= MAX_DEVICES) return -1;
    str_copy(t->dev_ids[t->dev_n], dev_id, MAX_TOKEN);
    t->dev_n++;
    return 0;
}

/* ─── Channel management ─── */
static int find_channel(const char *name)
{
    int i;
    for (i = 0; i < chan_n; i++) {
        if (str_eq(channels[i].name, name)) return i;
    }
    return -1;
}

int channel_create(const char *name, int vendor_filter, int pri)
{
    if (chan_n >= MAX_CHANNELS) return -1;
    if (find_channel(name) >= 0) return -1;
    channel_t *c = &channels[chan_n];
    mem_zero(c, sizeof(channel_t));
    str_copy(c->name, name, MAX_NAME);
    c->vendor_filter = vendor_filter;
    c->enabled = 1;
    c->pri = pri;
    log_str("channel: "); log_str(name);
    log_str(" pri="); log_num(pri); log_str("\n");
    return chan_n++;
}

/* ─── Routing rules ─── */
int route_add_rule(int vendor, int min_pri, const char *regions)
{
    if (rule_n >= MAX_RULES) return -1;
    route_rule_t *r = &rules[rule_n];
    mem_zero(r, sizeof(route_rule_t));
    r->vendor = vendor;
    r->min_pri = min_pri;
    str_copy(r->regions, regions, MAX_NAME);
    r->active = 1;
    log_str("route rule added: vendor="); log_num(vendor);
    log_str(" pri>="); log_num(min_pri); log_str("\n");
    return rule_n++;
}

int route_set_strategy(int strategy)
{
    route_strategy = strategy;
    log_str("route strategy="); log_num(strategy); log_str("\n");
    return 0;
}

static int select_vendor(int pri, const char *region)
{
    int i, best = -1, best_score = -1;
    for (i = 0; i < rule_n; i++) {
        route_rule_t *r = &rules[i];
        if (!r->active) continue;
        if (pri < r->min_pri) continue;
        if (r->regions[0] != '\0' && !str_eq(r->regions, region)) continue;
        if (r->vendor >= 0 && r->vendor < vendor_n && vendors[r->vendor].enabled) {
            return r->vendor;
        }
    }
    for (i = 0; i < vendor_n; i++) {
        if (!vendors[i].enabled) continue;
        int score = 0;
        if (route_strategy == ROUTE_COST_OPT) {
            score = 1000 - vendors[i].cost_per_1k;
        } else if (route_strategy == ROUTE_SPEED_OPT) {
            score = 1000 - vendors[i].avg_latency_ms;
        } else if (route_strategy == ROUTE_RELIABLE) {
            score = vendors[i].reliability;
        } else {
            score = vendors[i].reliability - vendors[i].cost_per_1k / 10;
        }
        if (score > best_score) { best_score = score; best = i; }
    }
    return best;
}

/* ─── Compliance management ─── */
static int find_compliance(const char *dev_id)
{
    int i;
    for (i = 0; i < compl_n; i++) {
        if (str_eq(compliances[i].dev_id, dev_id)) return i;
    }
    return -1;
}

int compliance_opt_out(const char *dev_id)
{
    int idx = find_compliance(dev_id);
    if (idx < 0) {
        if (compl_n >= MAX_DEVICES) return -1;
        idx = compl_n++;
        mem_zero(&compliances[idx], sizeof(compliance_t));
        str_copy(compliances[idx].dev_id, dev_id, MAX_TOKEN);
    }
    compliances[idx].opted_out = 1;
    compliances[idx].unsub_at = clk;
    log_str("compliance: opt-out "); log_str(dev_id); log_str("\n");
    return 0;
}

int compliance_opt_in(const char *dev_id)
{
    int idx = find_compliance(dev_id);
    if (idx < 0) return -1;
    compliances[idx].opted_out = 0;
    log_str("compliance: opt-in "); log_str(dev_id); log_str("\n");
    return 0;
}

static int check_compliance(const char *dev_id, int ch_idx)
{
    int ci = find_compliance(dev_id);
    if (ci >= 0 && compliances[ci].opted_out) return COMPL_BLOCK;
    if (ch_idx >= 0 && ch_idx < chan_n && !channels[ch_idx].enabled) return COMPL_BLOCK;
    (void)ch_idx;
    return COMPL_ALLOW;
}

static int check_rate_limit(vendor_t *v)
{
    if (clk != v->last_tick) { v->qps_cur = 0; v->last_tick = clk; }
    if (v->qps_cur >= v->qps_limit) return 0;
    v->qps_cur++;
    return 1;
}

/* ─── Message construction ─── */
static unsigned long next_msg_id(void) { return msg_id_seq++; }

int push_create_notification(const char *title, const char *body,
                             const char *channel, int pri)
{
    if (msg_n >= MAX_MSGS) return -1;
    push_msg_t *m = &messages[msg_n];
    mem_zero(m, sizeof(push_msg_t));
    m->id = next_msg_id();
    m->type = MSG_NOTIFY;
    str_copy(m->title, title, MAX_NAME);
    str_copy(m->body, body, MAX_BODY);
    str_copy(m->channel, channel, MAX_NAME);
    m->pri = pri;
    m->vendor = VENDOR_NONE;
    m->created = clk;
    m->expire = clk + 86400;
    m->status = DEL_PENDING;
    log_str("msg notification #"); log_ulong(m->id);
    log_str(" \""); log_str(title); log_str("\"\n");
    return msg_n++;
}

int push_create_transparent(const char *payload, const char *channel, int pri)
{
    if (msg_n >= MAX_MSGS) return -1;
    push_msg_t *m = &messages[msg_n];
    mem_zero(m, sizeof(push_msg_t));
    m->id = next_msg_id();
    m->type = MSG_TRANSPARENT;
    str_copy(m->body, payload, MAX_BODY);
    str_copy(m->channel, channel, MAX_NAME);
    m->pri = pri;
    m->vendor = VENDOR_NONE;
    m->created = clk;
    m->expire = clk + 86400;
    m->status = DEL_PENDING;
    log_str("msg transparent #"); log_ulong(m->id); log_str("\n");
    return msg_n++;
}

int push_create_rich_media(const char *title, const char *body,
                           const char *media_url, int media_type)
{
    if (msg_n >= MAX_MSGS) return -1;
    push_msg_t *m = &messages[msg_n];
    mem_zero(m, sizeof(push_msg_t));
    m->id = next_msg_id();
    m->type = MSG_RICH_MEDIA;
    str_copy(m->title, title, MAX_NAME);
    str_copy(m->body, body, MAX_BODY);
    m->media[0].idx = 0;
    m->media[0].type = media_type;
    str_copy(m->media[0].url, media_url, MAX_BODY);
    m->media_n = 1;
    m->pri = PRI_NORMAL;
    m->vendor = VENDOR_NONE;
    m->created = clk;
    m->expire = clk + 86400;
    m->status = DEL_PENDING;
    log_str("msg rich #"); log_ulong(m->id);
    log_str(" media="); log_str(media_url); log_str("\n");
    return msg_n++;
}

int push_create_interactive(const char *title, const char *body,
                            const char *btn_label, const char *btn_action)
{
    if (msg_n >= MAX_MSGS) return -1;
    push_msg_t *m = &messages[msg_n];
    mem_zero(m, sizeof(push_msg_t));
    m->id = next_msg_id();
    m->type = MSG_INTERACTIVE;
    str_copy(m->title, title, MAX_NAME);
    str_copy(m->body, body, MAX_BODY);
    m->btns[0].idx = 0;
    str_copy(m->btns[0].label, btn_label, MAX_NAME);
    str_copy(m->btns[0].action, btn_action, MAX_BODY);
    m->btn_n = 1;
    m->pri = PRI_HIGH;
    m->vendor = VENDOR_NONE;
    m->created = clk;
    m->expire = clk + 43200;
    m->status = DEL_PENDING;
    log_str("msg interactive #"); log_ulong(m->id);
    log_str(" btn=\""); log_str(btn_label); log_str("\"\n");
    return msg_n++;
}

int push_add_button(int msg_idx, const char *label, const char *action)
{
    if (msg_idx < 0 || msg_idx >= msg_n) return -1;
    push_msg_t *m = &messages[msg_idx];
    if (m->btn_n >= MAX_BUTTONS) return -1;
    button_t *b = &m->btns[m->btn_n];
    b->idx = m->btn_n;
    str_copy(b->label, label, MAX_NAME);
    str_copy(b->action, action, MAX_BODY);
    m->btn_n++;
    return 0;
}

int push_add_media(int msg_idx, const char *url, int media_type)
{
    if (msg_idx < 0 || msg_idx >= msg_n) return -1;
    push_msg_t *m = &messages[msg_idx];
    if (m->media_n >= MAX_MEDIA) return -1;
    media_t *md = &m->media[m->media_n];
    md->idx = m->media_n;
    md->type = media_type;
    str_copy(md->url, url, MAX_BODY);
    m->media_n++;
    return 0;
}

/* ─── Delivery queue ─── */
static void enqueue_delivery(int msg_idx, int dev_idx, int vendor_id)
{
    if (queue_n >= MAX_QUEUE) return;
    delivery_t *d = &queue[queue_n];
    mem_zero(d, sizeof(delivery_t));
    d->msg = messages[msg_idx];
    d->target_dev = dev_idx;
    d->vendor_used = vendor_id;
    d->status = DEL_PENDING;
    d->sent_at = 0;
    d->retries = 0;
    queue_n++;
}

static void update_stats(int vendor_id, int status)
{
    int i, si = -1;
    for (i = 0; i < stats_n; i++) {
        if (str_eq(vstats[i].vendor, vendors[vendor_id].name)) { si = i; break; }
    }
    if (si < 0 && stats_n < MAX_STATS) {
        si = stats_n++;
        mem_zero(&vstats[si], sizeof(vendor_stats_t));
        str_copy(vstats[si].vendor, vendors[vendor_id].name, MAX_NAME);
    }
    if (si < 0) return;
    vstats[si].total++;
    if (status == DEL_ARRIVED) vstats[si].arrived++;
    if (status == DEL_CLICKED) vstats[si].clicked++;
    if (status == DEL_FAILED) vstats[si].failed++;
}

static void process_queue(void)
{
    int i;
    int processed = 0;
    for (i = 0; i < queue_n; i++) {
        delivery_t *d = &queue[i];
        if (d->status != DEL_PENDING) continue;
        if (d->vendor_used < 0 || d->vendor_used >= vendor_n) {
            d->status = DEL_FAILED; total_failed++; continue;
        }
        vendor_t *v = &vendors[d->vendor_used];
        if (!v->enabled) { d->status = DEL_FAILED; total_failed++; continue; }
        if (!check_rate_limit(v)) {
            d->retries++;
            if (d->retries > 3) { d->status = DEL_FAILED; total_failed++; }
            continue;
        }
        d->status = DEL_SENT;
        d->sent_at = clk;
        v->sent++;
        total_pushed++;
        if (d->target_dev >= 0 && d->target_dev < dev_n) {
            d->status = DEL_ARRIVED;
            d->arrived_at = clk + (unsigned int)(v->avg_latency_ms);
            v->arrived++;
            total_arrived++;
        }
        update_stats(d->vendor_used, d->status);
        processed++;
    }
    if (processed > 0) {
        log_str("queue processed: "); log_num(processed); log_str("\n");
    }
}

/* ─── Unified push API ─── */
int push_send_to_device(int msg_idx, const char *dev_id)
{
    if (msg_idx < 0 || msg_idx >= msg_n) return -1;
    int di = find_device(dev_id);
    if (di < 0) { log_str("push: dev not found\n"); return -1; }
    if (!devices[di].active) { log_str("push: dev inactive\n"); return -1; }
    int ch_idx = find_channel(messages[msg_idx].channel);
    int comp = check_compliance(dev_id, ch_idx);
    if (comp == COMPL_BLOCK) { log_str("push: blocked\n"); return -1; }
    int vid = select_vendor(messages[msg_idx].pri, devices[di].region);
    if (vid < 0) { log_str("push: no vendor\n"); return -1; }
    messages[msg_idx].vendor = vid;
    enqueue_delivery(msg_idx, di, vid);
    log_str("push -> "); log_str(dev_id);
    log_str(" via "); log_str(vendors[vid].name); log_str("\n");
    return 0;
}

int push_send_to_alias(int msg_idx, const char *alias_name)
{
    int i, sent = 0;
    for (i = 0; i < alias_n; i++) {
        if (!str_eq(aliases[i].alias, alias_name)) continue;
        alias_map_t *a = &aliases[i];
        int j;
        for (j = 0; j < a->dev_n; j++) {
            if (push_send_to_device(msg_idx, a->dev_ids[j]) == 0) sent++;
        }
    }
    log_str("push alias "); log_str(alias_name);
    log_str(" sent="); log_num(sent); log_str("\n");
    return sent;
}

int push_send_to_tag(int msg_idx, const char *tag_name)
{
    int i, sent = 0;
    for (i = 0; i < tag_n; i++) {
        if (!str_eq(tags[i].name, tag_name)) continue;
        tag_t *t = &tags[i];
        int j;
        for (j = 0; j < t->dev_n; j++) {
            if (push_send_to_device(msg_idx, t->dev_ids[j]) == 0) sent++;
        }
    }
    log_str("push tag "); log_str(tag_name);
    log_str(" sent="); log_num(sent); log_str("\n");
    return sent;
}

int push_send_broadcast(int msg_idx)
{
    int i, sent = 0;
    for (i = 0; i < dev_n; i++) {
        if (!devices[i].active) continue;
        if (push_send_to_device(msg_idx, devices[i].dev_id) == 0) sent++;
    }
    log_str("broadcast sent="); log_num(sent); log_str("\n");
    return sent;
}

/* ─── Delivery tracking ─── */
int push_mark_clicked(const char *dev_id, unsigned long msg_id)
{
    int i;
    for (i = 0; i < queue_n; i++) {
        delivery_t *d = &queue[i];
        if (d->msg.id != msg_id) continue;
        if (d->target_dev >= 0 && d->target_dev < dev_n &&
            str_eq(devices[d->target_dev].dev_id, dev_id)) {
            d->status = DEL_CLICKED;
            d->clicked_at = clk;
            d->msg.clicked++;
            total_clicked++;
            if (d->vendor_used >= 0 && d->vendor_used < vendor_n) {
                vendors[d->vendor_used].arrived++;
                update_stats(d->vendor_used, DEL_CLICKED);
            }
            log_str("clicked: dev="); log_str(dev_id);
            log_str(" msg="); log_ulong(msg_id); log_str("\n");
            return 0;
        }
    }
    return -1;
}

/* ─── Statistics ─── */
int stats_get_overview(void)
{
    log_str("=== Push Statistics ===\n");
    log_str("total_pushed="); log_ulong(total_pushed); log_str("\n");
    log_str("total_arrived="); log_ulong(total_arrived); log_str("\n");
    log_str("total_clicked="); log_ulong(total_clicked); log_str("\n");
    log_str("total_failed="); log_ulong(total_failed); log_str("\n");
    unsigned long arr_rate = 0;
    if (total_pushed > 0) arr_rate = (total_arrived * 100) / total_pushed;
    log_str("arrival_rate="); log_ulong(arr_rate); log_str("%\n");
    unsigned long clk_rate = 0;
    if (total_arrived > 0) clk_rate = (total_clicked * 100) / total_arrived;
    log_str("click_rate="); log_ulong(clk_rate); log_str("%\n");
    return 0;
}

int stats_get_vendor(const char *name)
{
    int i;
    for (i = 0; i < stats_n; i++) {
        if (!str_eq(vstats[i].vendor, name)) continue;
        log_str("vendor stats: "); log_str(name); log_str("\n");
        log_str("  total="); log_ulong(vstats[i].total); log_str("\n");
        log_str("  arrived="); log_ulong(vstats[i].arrived); log_str("\n");
        log_str("  clicked="); log_ulong(vstats[i].clicked); log_str("\n");
        log_str("  failed="); log_ulong(vstats[i].failed); log_str("\n");
        return 0;
    }
    log_str("vendor stats not found\n");
    return -1;
}

int stats_compare_vendors(void)
{
    int i;
    log_str("=== Vendor Comparison ===\n");
    for (i = 0; i < vendor_n; i++) {
        vendor_t *v = &vendors[i];
        log_str(v->name); log_str(": sent="); log_ulong(v->sent);
        log_str(" arr="); log_ulong(v->arrived);
        log_str(" fail="); log_ulong(v->failed);
        log_str(" lat="); log_num(v->avg_latency_ms);
        log_str("ms cost="); log_num(v->cost_per_1k); log_str("\n");
    }
    return 0;
}

int stats_funnel(void)
{
    log_str("=== Funnel Analysis ===\n");
    log_str("stage1_created="); log_ulong((unsigned long)msg_n); log_str("\n");
    log_str("stage2_sent="); log_ulong(total_pushed); log_str("\n");
    log_str("stage3_arrived="); log_ulong(total_arrived); log_str("\n");
    log_str("stage4_clicked="); log_ulong(total_clicked); log_str("\n");
    if (msg_n > 0) {
        unsigned long r1 = (total_pushed * 100) / (unsigned long)msg_n;
        unsigned long r2 = (total_arrived * 100) / (unsigned long)msg_n;
        unsigned long r3 = (total_clicked * 100) / (unsigned long)msg_n;
        log_str("conv_send="); log_ulong(r1); log_str("%\n");
        log_str("conv_arrive="); log_ulong(r2); log_str("%\n");
        log_str("conv_click="); log_ulong(r3); log_str("%\n");
    }
    return 0;
}

/* ─── Offline cache management ─── */
int push_retry_failed(void)
{
    int i, retried = 0;
    for (i = 0; i < queue_n; i++) {
        delivery_t *d = &queue[i];
        if (d->status != DEL_FAILED) continue;
        if (d->retries >= 3) continue;
        int vid = select_vendor(d->msg.pri,
            (d->target_dev >= 0 && d->target_dev < dev_n) ?
            devices[d->target_dev].region : "");
        if (vid < 0) continue;
        d->vendor_used = vid;
        d->status = DEL_PENDING;
        d->retries++;
        retried++;
    }
    if (retried > 0) {
        log_str("retried: "); log_num(retried); log_str("\n");
        process_queue();
    }
    return retried;
}

/* ─── Tick (simulate time advance) ─── */
int tick(unsigned int dt)
{
    clk += dt;
    int i;
    for (i = 0; i < vendor_n; i++) { vendors[i].qps_cur = 0; }
    return 0;
}

/* ─── Status queries ─── */
int status_queue(void)
{
    int pend = 0, sent = 0, arr = 0, fail = 0, clkd = 0;
    int i;
    for (i = 0; i < queue_n; i++) {
        switch (queue[i].status) {
        case DEL_PENDING: pend++; break;
        case DEL_SENT:    sent++; break;
        case DEL_ARRIVED: arr++;  break;
        case DEL_FAILED:  fail++; break;
        case DEL_CLICKED: clkd++; break;
        default: break;
        }
    }
    log_str("queue: pend="); log_num(pend);
    log_str(" sent="); log_num(sent);
    log_str(" arr="); log_num(arr);
    log_str(" fail="); log_num(fail);
    log_str(" click="); log_num(clkd); log_str("\n");
    return 0;
}

/* ─── Exported: init ─── */
__attribute__((export_name("init")))
int wasm_init(void)
{
    log_str("push_gateway v1.0 init\n");
    mem_zero(vendors, sizeof(vendors));
    mem_zero(devices, sizeof(devices));
    mem_zero(aliases, sizeof(aliases));
    mem_zero(tags, sizeof(tags));
    mem_zero(channels, sizeof(channels));
    mem_zero(messages, sizeof(messages));
    mem_zero(queue, sizeof(queue));
    mem_zero(vstats, sizeof(vstats));
    mem_zero(rules, sizeof(rules));
    mem_zero(compliances, sizeof(compliances));
    return 0;
}

/* ─── Main: demo scenario ─── */
int main(void)
{
    wasm_init();

    /* Register push vendors */
    int apns = vendor_register("APNS", 500, 8, 95);
    int fcm  = vendor_register("FCM", 1000, 5, 92);
    int hw   = vendor_register("Huawei", 800, 6, 88);
    int xm   = vendor_register("Xiaomi", 600, 4, 85);
    int oppo = vendor_register("OPPO", 400, 7, 82);
    int vivo = vendor_register("Vivo", 400, 7, 80);
    (void)apns; (void)fcm; (void)hw; (void)xm; (void)oppo; (void)vivo;

    /* Setup channels */
    channel_create("marketing", VENDOR_NONE, PRI_NORMAL);
    channel_create("transaction", VENDOR_NONE, PRI_HIGH);
    channel_create("system", VENDOR_NONE, PRI_URGENT);

    /* Routing rules: urgent messages go to APNS */
    route_add_rule(VENDOR_APNS, PRI_URGENT, "");
    route_add_rule(VENDOR_FCM, PRI_HIGH, "");
    route_set_strategy(ROUTE_AUTO);

    /* Register devices */
    device_register("dev_001", VENDOR_APNS, "apns_token_001", "US");
    device_register("dev_002", VENDOR_FCM, "fcm_token_002", "EU");
    device_register("dev_003", VENDOR_HUAWEI, "hw_token_003", "CN");
    device_register("dev_004", VENDOR_XIAOMI, "xm_token_004", "CN");
    device_register("dev_005", VENDOR_OPPO, "oppo_token_005", "IN");
    device_register("dev_006", VENDOR_FCM, "fcm_token_006", "US");

    /* Alias: user1 has two devices */
    alias_create("user1");
    alias_bind("user1", "dev_001");
    alias_bind("user1", "dev_002");

    /* Tags: premium users */
    tag_create("premium");
    tag_add_device("premium", "dev_001");
    tag_add_device("premium", "dev_003");
    tag_create("beta");
    tag_add_device("beta", "dev_005");
    tag_add_device("beta", "dev_006");

    /* Compliance: dev_005 opts out */
    compliance_opt_out("dev_005");

    /* Create messages of each type */
    int m1 = push_create_notification("Welcome", "Hello from AI-ASM!",
                                       "system", PRI_URGENT);
    int m2 = push_create_transparent("{\"sync\":true}", "transaction", PRI_HIGH);
    int m3 = push_create_rich_media("Sale!", "50% off today",
                                     "https://img.example.com/sale.png", MEDIA_IMAGE);
    int m4 = push_create_interactive("Update", "New version available",
                                      "Install", "app://update");
    push_add_button(m4, "Later", "app://dismiss");
    push_add_media(m3, "https://img.example.com/banner.png", MEDIA_IMAGE);

    /* Send to individual devices */
    push_send_to_device(m1, "dev_001");
    push_send_to_device(m1, "dev_003");
    push_send_to_device(m2, "dev_002");
    push_send_to_device(m2, "dev_006");

    /* Send to alias (multi-device) */
    push_send_to_alias(m3, "user1");

    /* Send to tag */
    push_send_to_tag(m4, "premium");

    /* Broadcast */
    push_send_broadcast(m1);

    /* Process delivery queue */
    process_queue();

    /* Simulate clicks */
    push_mark_clicked("dev_001", messages[m1].id);
    push_mark_clicked("dev_003", messages[m1].id);

    /* Advance time and retry */
    tick(1000);
    push_retry_failed();

    /* Statistics */
    stats_get_overview();
    stats_compare_vendors();
    stats_funnel();
    status_queue();

    /* Vendor management */
    vendor_set_enabled("Vivo", 0);
    device_set_active("dev_004", 0);
    compliance_opt_in("dev_005");
    device_set_network("dev_002", NET_5G);

    log_str("push_gateway demo complete\n");
    host_exit(0);
    return 0;
}
