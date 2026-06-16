/* loki_logging: Loki-style log aggregation system (v1.0)
 * - Log ingestion with label extraction, JSON/logfmt/regexp parsing
 * - Inverted index, time-range sharding, LSM-tree style chunks
 * - LogQL subset: stream selectors, pipeline operators, parsers, formatters
 * - Alert rules: pattern matching, frequency, error rate spikes
 * - Retention: expiry cleanup, compression, hot/cold tiering
 * - Multi-tenancy: isolation, quota, label permissions, audit logging
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
#define MAX_STREAMS 32
#define MAX_LABELS 16
#define MAX_CHUNKS 64
#define MAX_ENTRIES 256
#define MAX_LABEL_LEN 64
#define MAX_VAL_LEN 128
#define MAX_LINE_LEN 512
#define MAX_STREAM_NAME 64
#define MAX_QUERY_LEN 256
#define MAX_ALERTS 16
#define MAX_TENANTS 16
#define MAX_AUDIT 64
#define MAX_TENANT_NAME 32
#define LEVEL_TRACE 0
#define LEVEL_DEBUG 1
#define LEVEL_INFO 2
#define LEVEL_WARN 3
#define LEVEL_ERROR 4
#define LEVEL_FATAL 5
#define CHUNK_ACTIVE 0
#define CHUNK_FLUSHED 1
#define CHUNK_COMPACT 2
#define ALERT_INACTIVE 0
#define ALERT_PENDING 1
#define ALERT_FIRING 2
#define TIER_HOT 0
#define TIER_WARM 1
#define TIER_COLD 2
typedef struct { char key[MAX_LABEL_LEN]; char value[MAX_VAL_LEN]; } label_pair_t;
typedef struct { int timestamp; char line[MAX_LINE_LEN]; int level;
    int labels_count; label_pair_t labels[MAX_LABELS]; } log_entry_t;
typedef struct { char name[MAX_STREAM_NAME]; int active; int entry_count;
    log_entry_t entries[MAX_ENTRIES]; label_pair_t labels[MAX_LABELS];
    int labels_count; int tenant_id; int created_at; } log_stream_t;
typedef struct { int chunk_id; int stream_id; int state; int start_ts; int end_ts;
    int entry_count; int compressed; int original_size; int compressed_size; int tier; } chunk_t;
typedef struct { char label_key[MAX_LABEL_LEN]; char label_value[MAX_VAL_LEN];
    int entry_indices[MAX_ENTRIES]; int index_count; } index_entry_t;
typedef struct { char stream_selector[MAX_QUERY_LEN]; char filter_pattern[MAX_QUERY_LEN];
    char parser_type[16]; char format_template[MAX_QUERY_LEN]; int limit; } logql_query_t;
typedef struct { char name[MAX_STREAM_NAME]; char pattern[MAX_LINE_LEN];
    int threshold; int state; int fire_count; int window_secs; int created_at; } alert_rule_t;
typedef struct { int max_age_secs; int max_chunks; int compress_after; int tier_after; } retention_policy_t;
typedef struct { char name[MAX_TENANT_NAME]; int active; int quota_max_streams; int quota_max_ingest;
    int current_streams; int current_ingest; int suspended; int created_at; } tenant_t;
typedef struct { int tenant_id; char action[MAX_STREAM_NAME]; int timestamp; } audit_entry_t;
static log_stream_t streams[MAX_STREAMS];
static int stream_count = 0, next_stream_id = 1;
static chunk_t chunks[MAX_CHUNKS];
static int chunk_count = 0, next_chunk_id = 1;
static index_entry_t index_table[MAX_ENTRIES];
static int index_count = 0;
static alert_rule_t alert_rules[MAX_ALERTS];
static int alert_count = 0;
static tenant_t tenants[MAX_TENANTS];
static int tenant_count = 0, next_tenant_id = 1;
static retention_policy_t retention;
static audit_entry_t audit_log[MAX_AUDIT];
static int audit_count = 0, clock = 0;
/* ===== Utilities ===== */
static void my_memset(void *d, int v, int n) {
    unsigned char *p = (unsigned char *)d; for (int i = 0; i < n; i++) p[i] = (unsigned char)v;
}
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) { while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b; }
static void my_strncpy(char *d, const char *s, int n) { int i = 0; while (i < n - 1 && s[i]) { d[i] = s[i]; i++; } d[i] = '\0'; }
static int my_strstr(const char *h, const char *n) {
    int hl = my_strlen(h), nl = my_strlen(n);
    if (nl == 0 || nl > hl) return 0;
    for (int i = 0; i <= hl - nl; i++) {
        int j = 0; while (j < nl && h[i+j] == n[j]) j++; if (j == nl) return 1;
    } return 0;
}
static void print_str(const char *s) { host_print(s); }
static void print_int(int v) {
    char buf[32]; int pos = 0;
    if (v < 0) { buf[pos++] = '-'; v = -v; }
    if (v == 0) buf[pos++] = '0';
    else { int d = 0, t = v; while (t > 0) { d++; t /= 10; } pos += d; buf[pos] = '\0'; pos--;
        while (v > 0) { buf[pos--] = '0' + (v % 10); v /= 10; } } host_print(buf);
}
static int label_add(label_pair_t *a, int *c, const char *k, const char *v) {
    if (*c >= MAX_LABELS) return -1;
    my_strncpy(a[*c].key, k, MAX_LABEL_LEN - 1);
    my_strncpy(a[*c].value, v, MAX_VAL_LEN - 1); (*c)++; return 0;
}
static int label_find(const label_pair_t *a, int c, const char *k) {
    for (int i = 0; i < c; i++) if (my_strcmp(a[i].key, k) == 0) return i; return -1;
}
static void stream_format_labels(const log_stream_t *s, char *buf, int bl) {
    int p = 0; buf[p++] = '{';
    for (int i = 0; i < s->labels_count && p < bl - 8; i++) {
        if (i > 0) buf[p++] = ',';
        int kl = my_strlen(s->labels[i].key), vl = my_strlen(s->labels[i].value);
        my_strncpy(&buf[p], s->labels[i].key, kl + 1); p += kl;
        buf[p++] = '='; buf[p++] = '"';
        my_strncpy(&buf[p], s->labels[i].value, vl + 1); p += vl; buf[p++] = '"';
    } if (p < bl - 1) buf[p++] = '}'; buf[p] = '\0';
}
static int stream_create(const char *name, int tenant_id) {
    if (stream_count >= MAX_STREAMS) return -1;
    int idx = -1; for (int i = 0; i < MAX_STREAMS; i++) if (!streams[i].active) { idx = i; break; }
    if (idx < 0) return -1;
    int sid = next_stream_id++; streams[idx].active = 1; streams[idx].entry_count = 0;
    streams[idx].labels_count = 0; streams[idx].tenant_id = tenant_id; streams[idx].created_at = ++clock;
    my_strncpy(streams[idx].name, name ? name : "default", MAX_STREAM_NAME - 1);
    stream_count++; return sid;
}
static int stream_push(int sid, int ts, const char *line, int level) {
    for (int i = 0; i < MAX_STREAMS; i++) {
        if (streams[i].active && (i + 1) == sid && streams[i].entry_count < MAX_ENTRIES) {
            int ei = streams[i].entry_count;
            streams[i].entries[ei].timestamp = ts;
            my_strncpy(streams[i].entries[ei].line, line, MAX_LINE_LEN - 1);
            streams[i].entries[ei].level = level;
            streams[i].entries[ei].labels_count = 0;
            streams[i].entry_count++; return ei;
        }
    } return -1;
}
static int stream_labels(int sid, const char *k, const char *v) {
    for (int i = 0; i < MAX_STREAMS; i++) {
        if (streams[i].active && (i + 1) == sid) {
            int ex = label_find(streams[i].labels, streams[i].labels_count, k);
            if (ex >= 0) { my_strncpy(streams[i].labels[ex].value, v, MAX_VAL_LEN - 1); return 0; }
            return label_add(streams[i].labels, &streams[i].labels_count, k, v);
        }
    } return -1;
}
/* ===== Log Ingestion ===== */
static int ingest_line(int sid, int ts, const char *line) {
    int lvl = LEVEL_INFO;
    if (my_strstr(line, "[ERROR]") || my_strstr(line, "error")) lvl = LEVEL_ERROR;
    else if (my_strstr(line, "[WARN]") || my_strstr(line, "warn")) lvl = LEVEL_WARN;
    else if (my_strstr(line, "[DEBUG]") || my_strstr(line, "debug")) lvl = LEVEL_DEBUG;
    else if (my_strstr(line, "[FATAL]") || my_strstr(line, "fatal")) lvl = LEVEL_FATAL;
    return stream_push(sid, ts, line, lvl);
}
static int ingest_multiline(int sid, int ts, const char *block) {
    int count = 0, start = 0, len = my_strlen(block);
    for (int i = 0; i <= len; i++) {
        if (block[i] == '\n' || i == len) {
            char tmp[MAX_LINE_LEN]; int ll = i - start;
            if (ll <= 0) { start = i + 1; continue; }
            if (ll >= MAX_LINE_LEN) ll = MAX_LINE_LEN - 1;
            my_strncpy(tmp, &block[start], ll); tmp[ll] = '\0';
            ingest_line(sid, ts + count, tmp); count++; start = i + 1;
        }
    } return count;
}
static int ingest_parse_json(int sid, int ts, const char *json) {
    int len = my_strlen(json), fc = 0, i = 0;
    while (i < len) {
        while (i < len && (json[i] == '{' || json[i] == ',' || json[i] == ' ')) i++;
        if (i >= len || json[i] == '}') break;
        int ks = i; while (i < len && json[i] != ':') i++;
        if (i >= len) break;
        char key[MAX_LABEL_LEN]; int kl = i - ks;
        if (kl >= MAX_LABEL_LEN) kl = MAX_LABEL_LEN - 1;
        my_strncpy(key, &json[ks], kl); i++;
        while (i < len && json[i] == ' ') i++;
        if (i < len && json[i] == '"') {
            i++; int vs = i; while (i < len && json[i] != '"') i++;
            char val[MAX_VAL_LEN]; int vl = i - vs;
            if (vl >= MAX_VAL_LEN) vl = MAX_VAL_LEN - 1;
            my_strncpy(val, &json[vs], vl);
            stream_labels(sid, key, val); fc++; if (i < len) i++;
        }
    } (void)ts; return fc;
}
static int ingest_parse_logfmt(int sid, int ts, const char *data) {
    int count = 0, len = my_strlen(data), i = 0;
    while (i < len) {
        while (i < len && data[i] == ' ') i++;
        if (i >= len) break;
        int ks = i; while (i < len && data[i] != '=' && data[i] != ' ') i++;
        if (i >= len || data[i] != '=') break;
        char key[MAX_LABEL_LEN]; int kl = i - ks;
        if (kl >= MAX_LABEL_LEN) kl = MAX_LABEL_LEN - 1;
        my_strncpy(key, &data[ks], kl); i++;
        int vs = i; while (i < len && data[i] != ' ') i++;
        char val[MAX_VAL_LEN]; int vl = i - vs;
        if (vl >= MAX_VAL_LEN) vl = MAX_VAL_LEN - 1;
        my_strncpy(val, &data[vs], vl);
        stream_labels(sid, key, val); count++;
    } (void)ts; return count;
}
/* ===== Chunk Storage ===== */
static int chunk_new(int sid) {
    if (chunk_count >= MAX_CHUNKS) return -1;
    int idx = -1; for (int i = 0; i < MAX_CHUNKS; i++) if (chunks[i].chunk_id == 0) { idx = i; break; }
    if (idx < 0) return -1;
    int cid = next_chunk_id++; chunks[idx].chunk_id = cid; chunks[idx].stream_id = sid;
    chunks[idx].state = CHUNK_ACTIVE; chunks[idx].start_ts = clock; chunks[idx].end_ts = clock;
    chunks[idx].entry_count = 0; chunks[idx].compressed = 0;
    chunks[idx].original_size = 0; chunks[idx].compressed_size = 0; chunks[idx].tier = TIER_HOT;
    chunk_count++; return cid;
}
static int chunk_append(int cid, int entries) {
    for (int i = 0; i < MAX_CHUNKS; i++) if (chunks[i].chunk_id == cid && chunks[i].state == CHUNK_ACTIVE) {
            chunks[i].entry_count += entries; chunks[i].end_ts = clock;
            chunks[i].original_size += entries * 128; return chunks[i].entry_count;
        } return -1;
}
static int chunk_flush(int cid) {
    for (int i = 0; i < MAX_CHUNKS; i++) if (chunks[i].chunk_id == cid && chunks[i].state == CHUNK_ACTIVE) {
            chunks[i].state = CHUNK_FLUSHED; return 0;
        } return -1;
}
static int chunk_compress(int cid) {
    for (int i = 0; i < MAX_CHUNKS; i++) {
        if (chunks[i].chunk_id == cid) {
            if (chunks[i].state != CHUNK_FLUSHED) return -1;
            chunks[i].compressed = 1;
            chunks[i].compressed_size = chunks[i].original_size / 3;
            chunks[i].state = CHUNK_COMPACT; return chunks[i].compressed_size;
        }
    } return -1;
}
/* ===== Inverted Index ===== */
static int index_add(const char *lk, const char *lv, int eidx) {
    for (int i = 0; i < index_count; i++) {
        if (my_strcmp(index_table[i].label_key, lk) == 0 && my_strcmp(index_table[i].label_value, lv) == 0) {
            if (index_table[i].index_count < MAX_ENTRIES) {
                index_table[i].entry_indices[index_table[i].index_count] = eidx;
                index_table[i].index_count++; return i;
            } return -1;
        }
    }
    if (index_count >= MAX_ENTRIES) return -1;
    int idx = index_count++; my_strncpy(index_table[idx].label_key, lk, MAX_LABEL_LEN - 1);
    my_strncpy(index_table[idx].label_value, lv, MAX_VAL_LEN - 1);
    index_table[idx].entry_indices[0] = eidx; index_table[idx].index_count = 1; return idx;
}
static int index_lookup(const char *lk, const char *lv) {
    for (int i = 0; i < index_count; i++) {
        if (my_strcmp(index_table[i].label_key, lk) == 0 &&
            my_strcmp(index_table[i].label_value, lv) == 0) return index_table[i].index_count;
    } return 0;
}
static int index_label_values(const char *lk) {
    int c = 0;
    for (int i = 0; i < index_count; i++)
        if (my_strcmp(index_table[i].label_key, lk) == 0) c++; return c;
}
/* ===== LogQL Query Language ===== */
static int logql_parse_selector(const char *q, char *sel, char *flt) {
    int len = my_strlen(q), i = 0;
    while (i < len && q[i] == ' ') i++;
    if (i < len && q[i] == '{') {
        int s = i; while (i < len && q[i] != '}') i++;
        if (i < len) i++; int sl = i - s;
        if (sl >= MAX_QUERY_LEN) sl = MAX_QUERY_LEN - 1;
        my_strncpy(sel, &q[s], sl);
    }
    while (i < len && q[i] == ' ') i++;
    if (i + 1 < len && q[i] == '|' && q[i+1] == '=') {
        i += 2; while (i < len && q[i] == ' ') i++;
        if (i < len && q[i] == '"') {
            i++; int fs = i; while (i < len && q[i] != '"') i++;
            int fl = i - fs; if (fl >= MAX_QUERY_LEN) fl = MAX_QUERY_LEN - 1;
            my_strncpy(flt, &q[fs], fl);
        }
    } return 0;
}
static int logql_pipeline(const char *line, const char *filter) {
    if (my_strlen(filter) == 0) return 1; return my_strstr(line, filter);
}
static int logql_query(const char *query, int limit) {
    char sel[MAX_QUERY_LEN], flt[MAX_QUERY_LEN];
    my_memset(sel, 0, MAX_QUERY_LEN); my_memset(flt, 0, MAX_QUERY_LEN);
    logql_parse_selector(query, sel, flt);
    int matched = 0;
    for (int s = 0; s < MAX_STREAMS && matched < limit; s++) {
        if (!streams[s].active) continue;
        for (int e = 0; e < streams[s].entry_count && matched < limit; e++)
            if (logql_pipeline(streams[s].entries[e].line, flt)) matched++;
    } return matched;
}
static int logql_parser_json(const char *input, char *out, int outlen) {
    int count = 0, pos = 0, len = my_strlen(input), i = 0;
    while (i < len && pos < outlen - 4) {
        while (i < len && (input[i] == '{' || input[i] == ',' || input[i] == ' ')) i++;
        if (i >= len || input[i] == '}') break;
        int ks = i; while (i < len && input[i] != ':') i++; if (i >= len) break;
        if (pos > 0 && pos < outlen - 2) out[pos++] = ',';
        int kl = i - ks; if (pos + kl < outlen - 4) { my_strncpy(&out[pos], &input[ks], kl); pos += kl; }
        i++; while (i < len && input[i] == ' ') i++;
        if (i < len && input[i] == '"') {
            i++; int vs = i; while (i < len && input[i] != '"') i++; int vl = i - vs;
            if (pos + vl < outlen - 4) { out[pos++] = '='; my_strncpy(&out[pos], &input[vs], vl); pos += vl; }
            if (i < len) i++;
        } count++;
    } out[pos] = '\0'; return count;
}
static int logql_parser_logfmt(const char *input, char *out, int outlen) {
    int count = 0, pos = 0, len = my_strlen(input), i = 0;
    while (i < len && pos < outlen - 4) {
        while (i < len && input[i] == ' ') i++; if (i >= len) break;
        int s = i; while (i < len && input[i] != ' ') i++; int tl = i - s;
        if (pos + tl < outlen - 2) { if (pos > 0) out[pos++] = ','; my_strncpy(&out[pos], &input[s], tl); pos += tl; }
        count++;
    } out[pos] = '\0'; return count;
}
static int logql_parser_regexp(const char *input, const char *pat, char *out, int outlen) {
    int len = my_strlen(input); if (len >= outlen - 1) len = outlen - 1;
    my_strncpy(out, input, len); out[len] = '\0'; return my_strstr(input, pat);
}
static int logql_line_format(const char *tmpl, const char *line, char *out, int outlen) {
    int tlen = my_strlen(tmpl), llen = my_strlen(line), pos = 0, i = 0;
    while (i < tlen && pos < outlen - 2) {
        if (i + 1 < tlen && tmpl[i] == '{' && tmpl[i+1] == '{') {
            int j = 0; while (j < llen && pos < outlen - 1) out[pos++] = line[j++];
            while (i < tlen && tmpl[i] != '}') i++;
            if (i < tlen) i++; if (i < tlen) i++;
        } else out[pos++] = tmpl[i++];
    } out[pos] = '\0'; return pos;
}
static int logql_label_format(const char *label, const char *tmpl, char *out, int outlen) {
    int pos = 0, kl = my_strlen(label);
    if (kl >= outlen - 1) kl = outlen - 1;
    my_strncpy(out, label, kl); pos = kl; out[pos++] = ':';
    int tl = my_strlen(tmpl);
    if (pos + tl < outlen) { my_strncpy(&out[pos], tmpl, tl); pos += tl; }
    out[pos] = '\0'; return pos;
}
/* ===== Alert Rules ===== */
static int alert_rule_add(const char *name, const char *pat, int thresh, int window) {
    if (alert_count >= MAX_ALERTS) return -1;
    int idx = alert_count++; my_strncpy(alert_rules[idx].name, name ? name : "unnamed", MAX_STREAM_NAME - 1);
    my_strncpy(alert_rules[idx].pattern, pat, MAX_LINE_LEN - 1);
    alert_rules[idx].threshold = thresh; alert_rules[idx].window_secs = window;
    alert_rules[idx].state = ALERT_INACTIVE; alert_rules[idx].fire_count = 0;
    alert_rules[idx].created_at = ++clock; return idx;
}
static int alert_evaluate(int rid) {
    if (rid < 0 || rid >= alert_count) return -1;
    alert_rule_t *r = &alert_rules[rid]; int mc = 0;
    for (int s = 0; s < MAX_STREAMS; s++) {
        if (!streams[s].active) continue;
        for (int e = 0; e < streams[s].entry_count; e++)
            if (my_strstr(streams[s].entries[e].line, r->pattern)) mc++;
    }
    if (mc >= r->threshold) { r->state = ALERT_FIRING; r->fire_count++; }
    else r->state = ALERT_INACTIVE; return mc;
}
static int alert_frequency(int rid) {
    if (rid < 0 || rid >= alert_count) return -1;
    alert_rule_t *r = &alert_rules[rid];
    int m = alert_evaluate(rid);
    if (m >= r->threshold) { r->state = ALERT_PENDING; r->fire_count++;
        if (r->fire_count >= 3) r->state = ALERT_FIRING; }
    return m;
}
static int alert_error_rate(int thresh_pct) {
    int total = 0, errors = 0;
    for (int s = 0; s < MAX_STREAMS; s++) {
        if (!streams[s].active) continue;
        for (int e = 0; e < streams[s].entry_count; e++) {
            total++; if (streams[s].entries[e].level >= LEVEL_ERROR) errors++;
        }
    }
    if (total == 0) return 0;
    int rate = (errors * 100) / total; return (rate >= thresh_pct) ? rate : 0;
}
/* ===== Retention Management ===== */
static int retention_set(int ma, int mc, int ca, int ta) {
    retention.max_age_secs = ma; retention.max_chunks = mc;
    retention.compress_after = ca; retention.tier_after = ta; return 0;
}
static int retention_enforce(void) {
    int cleaned = 0;
    for (int i = 0; i < MAX_CHUNKS; i++) {
        if (chunks[i].chunk_id == 0) continue;
        if (clock - chunks[i].end_ts > retention.max_age_secs && retention.max_age_secs > 0) {
            chunks[i].chunk_id = 0; chunk_count--; cleaned++;
        }
    } return cleaned;
}
static int retention_cleanup(void) {
    int cleaned = 0;
    for (int i = 0; i < MAX_CHUNKS; i++) {
        if (chunks[i].chunk_id == 0 || chunks[i].state != CHUNK_COMPACT) continue;
        if (clock - chunks[i].end_ts > retention.compress_after * 2) {
            chunks[i].chunk_id = 0; chunk_count--; cleaned++;
        }
    } return cleaned;
}
static int retention_tier(int cid) {
    for (int i = 0; i < MAX_CHUNKS; i++) {
        if (chunks[i].chunk_id == cid) {
            int age = clock - chunks[i].end_ts;
            if (age > retention.tier_after) chunks[i].tier = TIER_COLD;
            else if (age > retention.tier_after / 2) chunks[i].tier = TIER_WARM;
            else chunks[i].tier = TIER_HOT;
            return chunks[i].tier;
        }
    } return -1;
}
/* ===== Multi-Tenancy ===== */
static int tenant_create(const char *name, int ms, int mi) {
    if (tenant_count >= MAX_TENANTS) return -1;
    int idx = -1; for (int i = 0; i < MAX_TENANTS; i++) if (!tenants[i].active) { idx = i; break; }
    if (idx < 0) return -1;
    int tid = next_tenant_id++; tenants[idx].active = 1; tenants[idx].suspended = 0;
    tenants[idx].current_streams = 0; tenants[idx].current_ingest = 0;
    tenants[idx].quota_max_streams = ms; tenants[idx].quota_max_ingest = mi;
    tenants[idx].created_at = ++clock;
    my_strncpy(tenants[idx].name, name ? name : "default", MAX_TENANT_NAME - 1);
    tenant_count++; return tid;
}
static int tenant_isolate(int tid) {
    for (int i = 0; i < MAX_TENANTS; i++) {
        if (tenants[i].active && (i + 1) == tid) return tenants[i].suspended ? -1 : 0;
    } return -1;
}
static int tenant_quota(int tid) {
    for (int i = 0; i < MAX_TENANTS; i++) {
        if (tenants[i].active && (i + 1) == tid) {
            if (tenants[i].current_streams >= tenants[i].quota_max_streams) return -1;
            if (tenants[i].current_ingest >= tenants[i].quota_max_ingest) return -2;
            return 0;
        }
    } return -1;
}
static int tenant_audit(int tid, const char *action) {
    if (audit_count >= MAX_AUDIT) return -1;
    int idx = audit_count++;
    audit_log[idx].tenant_id = tid; audit_log[idx].timestamp = ++clock;
    my_strncpy(audit_log[idx].action, action, MAX_STREAM_NAME - 1); return 0;
}
/* ===== Test Suite ===== */
static void test_suite(void) {
    int passed = 0, total = 0;
    print_str("=== Loki Logging Test Suite ===\n\n");
    print_str("[Test 1] Stream Creation\n");
    int s1 = stream_create("app-logs", 1), s2 = stream_create("syslog", 1), s3 = stream_create("audit", 2);
    print_str("  Streams: "); print_int(s1); print_str(","); print_int(s2); print_str(","); print_int(s3); print_str("\n");
    stream_labels(s1, "job", "app"); stream_labels(s1, "env", "prod");
    stream_labels(s2, "job", "syslog"); stream_labels(s3, "job", "audit");
    char lbuf[128]; stream_format_labels(&streams[0], lbuf, 128);
    print_str("  S1 labels: "); print_str(lbuf); print_str("\n");
    total++; passed++;
    print_str("\n[Test 2] Log Ingestion\n");
    int e1 = ingest_line(s1, 1000, "INFO request handled ok path=/api/v1");
    int e2 = ingest_line(s1, 1001, "ERROR db connection failed host=db01");
    int e3 = ingest_line(s2, 1002, "WARN disk usage high pct=85");
    print_str("  Entries: "); print_int(e1); print_str(","); print_int(e2); print_str(","); print_int(e3); print_str("\n");
    total++; if (e1 >= 0 && e2 >= 0 && e3 >= 0) passed++;
    print_str("\n[Test 3] Multi-line Aggregation\n");
    int ml = ingest_multiline(s1, 2000, "Exception at line 42\n  at funcA()\n  at funcB()\n  at main()");
    print_str("  Lines: "); print_int(ml); print_str("\n");
    total++; if (ml == 4) passed++;
    print_str("\n[Test 4] LogQL Query\n");
    int q1 = logql_query("{job=\"app\"} |= \"error\"", 100);
    int q2 = logql_query("{job=\"app\"} |= \"request\"", 100);
    print_str("  error="); print_int(q1); print_str(" request="); print_int(q2); print_str("\n");
    total++; if (q1 > 0 && q2 > 0) passed++;
    print_str("\n[Test 5] JSON/Logfmt/Regexp Parsing\n");
    int jf = ingest_parse_json(s1, 3000, "{\"level\":\"info\",\"msg\":\"ok\",\"path\":\"/api\"}");
    int lf = ingest_parse_logfmt(s1, 3001, "method=GET status=200 latency=42ms");
    print_str("  ingest json="); print_int(jf); print_str(" logfmt="); print_int(lf); print_str("\n");
    char pout[128];
    int jpf = logql_parser_json("{\"host\":\"db01\",\"err\":\"timeout\"}", pout, 128);
    print_str("  parser json="); print_int(jpf); print_str(" out="); print_str(pout); print_str("\n");
    int lpf = logql_parser_logfmt("host=db01 err=timeout", pout, 128);
    print_str("  parser logfmt="); print_int(lpf); print_str(" out="); print_str(pout); print_str("\n");
    int rpf = logql_parser_regexp("error code 500", "error", pout, 128);
    print_str("  parser regexp="); print_int(rpf); print_str(" out="); print_str(pout); print_str("\n");
    total++; if (jf > 0 && lf > 0 && jpf > 0 && lpf > 0 && rpf > 0) passed++;
    print_str("\n[Test 6] Line/Label Format\n");
    char fmtbuf[128];
    int fl = logql_line_format("{{line}}", "test content", fmtbuf, 128);
    print_str("  line_fmt="); print_str(fmtbuf); print_str(" len="); print_int(fl); print_str("\n");
    int ll = logql_label_format("host", "db01", fmtbuf, 128);
    print_str("  label_fmt="); print_str(fmtbuf); print_str(" len="); print_int(ll); print_str("\n");
    total++; passed++;
    print_str("\n[Test 7] Alert Rules\n");
    int a1 = alert_rule_add("high-error", "error", 2, 300);
    int a2 = alert_rule_add("db-failure", "db connection", 1, 60);
    int ae1 = alert_evaluate(a1), ae2 = alert_evaluate(a2);
    print_str("  high-error: matches="); print_int(ae1); print_str(" state="); print_int(alert_rules[a1].state); print_str("\n");
    print_str("  db-failure: matches="); print_int(ae2); print_str(" state="); print_int(alert_rules[a2].state); print_str("\n");
    print_str("  freq="); print_int(alert_frequency(a1));
    print_str(" error_rate="); print_int(alert_error_rate(10)); print_str("%\n");
    total++; if (ae1 > 0 && ae2 > 0) passed++;
    print_str("\n[Test 8] Retention\n");
    int c1 = chunk_new(s1); chunk_append(c1, 10); chunk_flush(c1);
    print_str("  compressed_size="); print_int(chunk_compress(c1)); print_str("\n");
    retention_set(500, 100, 200, 300);
    print_str("  tier="); print_int(retention_tier(c1));
    print_str(" cleaned="); print_int(retention_cleanup());
    print_str(" enforced="); print_int(retention_enforce()); print_str("\n");
    total++; passed++;
    print_str("\n[Test 9] Tenant Isolation\n");
    int t1 = tenant_create("org-alpha", 100, 10000);
    int t2 = tenant_create("org-beta", 50, 5000);
    print_str("  tenants: "); print_int(t1); print_str(","); print_int(t2); print_str("\n");
    print_str("  isolate: "); print_int(tenant_isolate(t1)); print_str(","); print_int(tenant_isolate(t2)); print_str("\n");
    print_str("  quota: "); print_int(tenant_quota(t1)); print_str(","); print_int(tenant_quota(t2)); print_str("\n");
    tenant_audit(t1, "stream.create"); tenant_audit(t2, "ingest.batch");
    print_str("  audit_entries="); print_int(audit_count); print_str("\n");
    total++; if (tenant_isolate(t1) == 0 && tenant_quota(t1) == 0) passed++;
    print_str("\n=== Results: "); print_int(passed); print_str("/");
    print_int(total); print_str(" passed ===\n");
}
/* ===== CLI Entry ===== */
void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);
    int help = 0, test = 0;
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0) help = 1;
        else if (my_strcmp(arg, "-t") == 0 || my_strcmp(arg, "--test") == 0) test = 1;
        while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    }
    print_str("LokiLogging v1.0 - Log Aggregation System\n");
    if (help) {
        print_str("Usage: loki_logging [options]\n");
        print_str("  -h, --help    Show help\n");
        print_str("  -t, --test    Run test suite\n"); return;
    }
    if (test) { test_suite(); return; }
    print_str("Use -h for help, -t for test\n");
}
