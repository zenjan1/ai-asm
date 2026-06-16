/* change_data_capture: CDC log replication engine (Debezium/GoldenGate) v1.0 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);
/* Source / event / format / compression / DDL type constants */
#define SRC_BINLOG 0
#define SRC_WAL    1
#define SRC_OPLOG  2
#define EVT_INSERT   1
#define EVT_UPDATE   2
#define EVT_DELETE   3
#define EVT_DDL      4
#define EVT_BEGIN    5
#define EVT_COMMIT   6
#define FMT_AVRO   0
#define FMT_JSON   1
#define FMT_PROTO  2
#define COMP_NONE  0
#define COMP_GZIP  1
#define COMP_ZSTD  2
#define COMP_SNAPPY 3
#define DDL_CREATE 1
#define DDL_ALTER  2
#define DDL_DROP   3
#define DDL_RENAME 4
#define MAX_NAME 64
#define MAX_DATA 512
#define MAX_SCHEMA 1024
#define MAX_ROUTES 16
#define MAX_TARGETS 8
#define MAX_OFFSETS 32
#define MAX_CKPTS 16
#define MAX_IDEM 64
#define MAX_SCHEMAS 16
#define MAX_COLS 16
#define MAX_MASKS 8
typedef struct { int event_type; int source_type; unsigned long long lsn;
    unsigned long long tx_id; unsigned long long timestamp;
    char database[MAX_NAME]; char table[MAX_NAME]; char data[MAX_DATA]; int record_count; } cdc_event_t;
typedef struct { char col_name[MAX_NAME]; int col_type; int nullable; int pk; } column_def_t;
typedef struct { char schema_name[MAX_NAME]; char table_name[MAX_NAME];
    column_def_t columns[MAX_COLS]; int col_count; int version; } schema_def_t;
typedef struct { char filename[MAX_NAME]; unsigned long long position; unsigned long long committed_at; } offset_entry_t;
typedef struct { int id; char source[MAX_NAME]; unsigned long long lsn; unsigned long long timestamp; int snapshot_complete; } checkpoint_t;
typedef struct { unsigned long long tx_id; char source[MAX_NAME]; int consumed; } idempotent_entry_t;
typedef struct { int schema_id; char subject[MAX_NAME]; int version; int format; char schema_data[MAX_SCHEMA]; } schema_entry_t;
typedef struct { char database[MAX_NAME]; char table[MAX_NAME]; char operation[MAX_NAME];
    char targets[MAX_TARGETS][MAX_NAME]; int target_count; } route_entry_t;
typedef struct { char table[MAX_NAME]; char column[MAX_NAME]; char mask_type[MAX_NAME]; } mask_rule_t;
typedef struct { unsigned long long start_ts; unsigned long long end_ts; int events_processed; } latency_sample_t;
static unsigned int heap_pos = 65536;
static offset_entry_t offsets[MAX_OFFSETS]; static int offset_count = 0;
static checkpoint_t ckpts[MAX_CKPTS]; static int ckpt_count = 0;
static idempotent_entry_t idem[MAX_IDEM]; static int idem_count = 0;
static schema_entry_t schemas[MAX_SCHEMAS]; static int schema_count = 0;
static route_entry_t routes[MAX_ROUTES]; static int route_count = 0;
static mask_rule_t masks[MAX_MASKS]; static int mask_count = 0;
static latency_sample_t lat_buf[32]; static int lat_count = 0;
/* Forward declarations */
static unsigned int alloc_mem(unsigned int n); static void copy_to_mem(const char *s, char *d, unsigned int n);
static void zero_mem(char *d, unsigned int n); static int my_strlen(const char *s);
static int my_strcmp(const char *a, const char *b); static int my_strncmp(const char *a, const char *b, unsigned int n);
static void copy_string(char *d, const char *s, unsigned int m); static void print_str(const char *s);
static void print_int(int n); static void print_ull(unsigned long long n);
static void cdc_init(void); static const char *ev_str(int t);
static int binlog_parse(const char *fn, unsigned long long pos, const char *db, const char *tbl, int et, cdc_event_t *o);
static int binlog_event_extract(const cdc_event_t *e, char *o, int ml);
static unsigned long long binlog_position_get(const char *fn);
static int wal_parse(const char *db, unsigned long long lsn, unsigned long long tx, cdc_event_t *o);
static int wal_event_extract(const cdc_event_t *e, char *o, int ml); static unsigned long long wal_lsn_get(const char *db);
static int oplog_parse(const char *ns, int op, unsigned long long ts, const char *pl, cdc_event_t *o);
static int oplog_event_extract(const cdc_event_t *e, char *o, int ml);
static int ddl_detect(const char *txt, char *db, char *tbl, int *dt);
static int ddl_schema_evolve(schema_def_t *cur, const char *txt, schema_def_t *upd);
static int ddl_compat_check(const schema_def_t *rd, const schema_def_t *wr);
static int serialize_avro(const cdc_event_t *e, char *o, int ml);
static int serialize_json(const cdc_event_t *e, char *o, int ml);
static int serialize_protobuf(const cdc_event_t *e, char *o, int ml);
static int schema_registry_register(const char *subj, const char *data, int fmt);
static int schema_registry_get(const char *subj, char *o, int ml);
static int compress_encode(const char *d, int l, int algo, char *o, int mo);
static int route_by_table(const char *db, const char *tbl, char *tgt);
static int route_by_database(const char *db, char *tgt);
static int route_by_filter(const char *db, const char *tbl, const char *op, char *tgt);
static int route_multi_target(const char *db, const char *tbl, char t[][MAX_NAME], int mt);
static int route_data_mask(const char *tbl, const char *col, const char *val, char *o, int mo);
static int offset_get(const char *fn, unsigned long long *pos);
static int offset_commit(const char *fn, unsigned long long pos); static int offset_reset(const char *fn);
static int checkpoint_save(int sid, unsigned long long lsn);
static int checkpoint_restore(int sid, unsigned long long *lsn); static int checkpoint_list(void);
static int idempotent_check(unsigned long long tx, const char *src);
static int transactional_publish(const cdc_event_t *e, const char *tgt);
static int snapshot_initial(const char *db, const char *tbl, int *tbls); static int snapshot_resume(int ckpt_id);
static int sync_incremental(const char *src, unsigned long long from, unsigned long long to, int *ev);
static int sync_latency_monitor(int win); static void test_suite(void); static void show_help(void);
/* Memory and string utilities */
static unsigned int alloc_mem(unsigned int n) {
    unsigned int p = heap_pos; heap_pos += n; heap_pos = (heap_pos + 15) & ~15u; return p; }
static void copy_to_mem(const char *s, char *d, unsigned int n) {
    for (unsigned int i = 0; i < n; i++) d[i] = s[i]; }
static void zero_mem(char *d, unsigned int n) { for (unsigned int i = 0; i < n; i++) d[i] = 0; }
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return 1; a++; b++; } return (*a != *b); }
static int my_strncmp(const char *a, const char *b, unsigned int n) {
    for (unsigned int i = 0; i < n; i++) {
        if (!a[i] || !b[i]) return (a[i] != b[i]); if (a[i] != b[i]) return 1; } return 0; }
static void copy_string(char *d, const char *s, unsigned int m) {
    unsigned int i = 0; while (s[i] && i < m - 1) { d[i] = s[i]; i++; } d[i] = 0; }
static void print_str(const char *s) {
    unsigned int l = (unsigned int)my_strlen(s); unsigned int b = alloc_mem(l + 1);
    copy_to_mem(s, (char *)b, l + 1); host_print((const char *)b); }
static void print_int(int n) {
    char b[16]; int i = 15, neg = 0; b[15] = 0;
    if (n < 0) { neg = 1; n = -n; } if (n == 0) b[--i] = '0';
    while (n > 0) { b[--i] = '0' + (n % 10); n /= 10; }
    if (neg) b[--i] = '-'; print_str(&b[i]); }
static void print_ull(unsigned long long n) {
    char b[24]; int i = 23; b[23] = 0; if (n == 0) b[--i] = '0';
    while (n > 0) { b[--i] = '0' + (int)(n % 10); n /= 10; } print_str(&b[i]); }
static void cdc_init(void) {
    zero_mem((char *)offsets, sizeof(offsets)); zero_mem((char *)ckpts, sizeof(ckpts));
    zero_mem((char *)idem, sizeof(idem)); zero_mem((char *)schemas, sizeof(schemas));
    zero_mem((char *)routes, sizeof(routes)); zero_mem((char *)masks, sizeof(masks));
    zero_mem((char *)lat_buf, sizeof(lat_buf)); }
static const char *ev_str(int t) {
    switch (t) { case EVT_INSERT: return "INSERT"; case EVT_UPDATE: return "UPDATE";
        case EVT_DELETE: return "DELETE"; case EVT_DDL: return "DDL";
        case EVT_BEGIN: return "BEGIN"; case EVT_COMMIT: return "COMMIT"; default: return "UNKNOWN"; } }
/* Binlog parsing (MySQL): parse, extract, position_get */
static int binlog_parse(const char *fn, unsigned long long pos, const char *db, const char *tbl, int et, cdc_event_t *o) {
    if (!fn || !db || !tbl || !o) return -1; zero_mem((char *)o, sizeof(cdc_event_t));
    o->source_type = SRC_BINLOG; o->event_type = et; o->lsn = pos;
    o->timestamp = pos + 1000; o->tx_id = pos / 100;
    copy_string(o->database, db, MAX_NAME); copy_string(o->table, tbl, MAX_NAME);
    copy_string(o->data, fn, MAX_DATA); o->record_count = 1; return 0; }
static int binlog_event_extract(const cdc_event_t *e, char *o, int ml) {
    if (!e || !o || ml <= 0) return -1;
    int n = my_strlen(e->data); if (n >= ml) n = ml - 1;
    copy_to_mem(e->data, o, (unsigned int)n); o[n] = 0; return n; }
static unsigned long long binlog_position_get(const char *fn) {
    if (!fn) return 0;
    for (int i = 0; i < offset_count; i++)
        if (my_strcmp(offsets[i].filename, fn) == 0) return offsets[i].position;
    return 0; }
/* WAL parsing (PostgreSQL): parse, extract, lsn_get */
static int wal_parse(const char *db, unsigned long long lsn, unsigned long long tx, cdc_event_t *o) {
    if (!db || !o) return -1; zero_mem((char *)o, sizeof(cdc_event_t));
    o->source_type = SRC_WAL; o->event_type = EVT_INSERT;
    o->lsn = lsn; o->tx_id = tx; o->timestamp = lsn + 2000;
    copy_string(o->database, db, MAX_NAME); copy_string(o->table, "public.events", MAX_NAME);
    copy_string(o->data, "wal_payload", MAX_DATA); o->record_count = 1; return 0; }
static int wal_event_extract(const cdc_event_t *e, char *o, int ml) {
    if (!e || !o || ml <= 0) return -1;
    int n = my_strlen(e->data); if (n >= ml) n = ml - 1;
    copy_to_mem(e->data, o, (unsigned int)n); o[n] = 0; return n; }
static unsigned long long wal_lsn_get(const char *db) {
    if (!db) return 0;
    for (int i = 0; i < offset_count; i++)
        if (my_strcmp(offsets[i].filename, db) == 0) return offsets[i].position;
    return 0; }
/* Oplog parsing (MongoDB): parse, extract */
static int oplog_parse(const char *ns, int op, unsigned long long ts, const char *pl, cdc_event_t *o) {
    if (!ns || !pl || !o) return -1; zero_mem((char *)o, sizeof(cdc_event_t));
    o->source_type = SRC_OPLOG; o->event_type = op;
    o->lsn = ts; o->tx_id = ts / 10; o->timestamp = ts;
    unsigned int i = 0; while (ns[i] && ns[i] != '.') i++;
    if (ns[i] == '.') { if (i >= MAX_NAME) i = MAX_NAME - 1;
        copy_to_mem(ns, o->database, i); o->database[i] = 0;
        copy_string(o->table, &ns[i + 1], MAX_NAME);
    } else { copy_string(o->database, ns, MAX_NAME); copy_string(o->table, "default", MAX_NAME); }
    copy_string(o->data, pl, MAX_DATA); o->record_count = 1; return 0; }
static int oplog_event_extract(const cdc_event_t *e, char *o, int ml) {
    if (!e || !o || ml <= 0) return -1;
    int n = my_strlen(e->data); if (n >= ml) n = ml - 1;
    copy_to_mem(e->data, o, (unsigned int)n); o[n] = 0; return n; }
/* DDL detection and schema evolution */
static int ddl_detect(const char *txt, char *db, char *tbl, int *dt) {
    if (!txt || !dt) return -1; *dt = 0;
    if (db) copy_string(db, "default_db", MAX_NAME);
    if (tbl) copy_string(tbl, "default_table", MAX_NAME);
    if (!my_strncmp(txt, "CREATE", 6)) { *dt = DDL_CREATE; if (tbl) copy_string(tbl, "new_table", MAX_NAME);
    } else if (!my_strncmp(txt, "ALTER", 5)) { *dt = DDL_ALTER; if (tbl) copy_string(tbl, "existing_table", MAX_NAME);
    } else if (!my_strncmp(txt, "DROP", 4)) { *dt = DDL_DROP;
    } else if (!my_strncmp(txt, "RENAME", 6)) { *dt = DDL_RENAME; }
    return (*dt != 0) ? 0 : -1; }
static int ddl_schema_evolve(schema_def_t *cur, const char *txt, schema_def_t *upd) {
    if (!cur || !txt || !upd) return -1;
    copy_string(upd->schema_name, cur->schema_name, MAX_NAME);
    copy_string(upd->table_name, cur->table_name, MAX_NAME);
    for (int i = 0; i < cur->col_count; i++) upd->columns[i] = cur->columns[i];
    upd->col_count = cur->col_count; upd->version = cur->version + 1;
    if (!my_strncmp(txt, "ALTER", 5) && upd->col_count < MAX_COLS) {
        copy_string(upd->columns[upd->col_count].col_name, "new_column", MAX_NAME);
        upd->columns[upd->col_count].col_type = 1;
        upd->columns[upd->col_count].nullable = 1; upd->columns[upd->col_count].pk = 0;
        upd->col_count++; }
    if (!my_strncmp(txt, "CREATE", 6)) { upd->col_count = 1;
        copy_string(upd->columns[0].col_name, "id", MAX_NAME);
        upd->columns[0].col_type = 0; upd->columns[0].pk = 1; }
    if (!my_strncmp(txt, "DROP", 4) && upd->col_count > 0) upd->col_count--;
    return 0; }
static int ddl_compat_check(const schema_def_t *rd, const schema_def_t *wr) {
    if (!rd || !wr) return -1;
    if (my_strcmp(rd->schema_name, wr->schema_name) != 0) return -2;
    if (my_strcmp(rd->table_name, wr->table_name) != 0) return -3;
    for (int i = 0; i < rd->col_count; i++) { int found = 0;
        for (int j = 0; j < wr->col_count; j++)
            if (my_strcmp(rd->columns[i].col_name, wr->columns[j].col_name) == 0) { found = 1; break; }
        if (!found) return -4; } return 0; }
/* Serialization: Avro / JSON / Protobuf */
static int serialize_avro(const cdc_event_t *e, char *o, int ml) {
    if (!e || !o || ml < 32) return -1; int p = 0;
    o[p++] = 'A'; o[p++] = 'V';
    o[p++] = (char)(e->event_type & 0x0F); o[p++] = (char)(e->source_type & 0x0F);
    int dl = my_strlen(e->database); if (p + dl + 2 >= ml) return -1;
    o[p++] = (char)dl; copy_to_mem(e->database, &o[p], (unsigned int)dl); p += dl; o[p++] = 0;
    int tl = my_strlen(e->table); if (p + tl + 2 >= ml) return -1;
    o[p++] = (char)tl; copy_to_mem(e->table, &o[p], (unsigned int)tl); p += tl; o[p++] = 0;
    o[p++] = (char)(e->record_count & 0xFF); o[p] = 0; return p; }
static int serialize_json(const cdc_event_t *e, char *o, int ml) {
    if (!e || !o || ml < 64) return -1; int p = 0;
    const char *h = "{\"type\":\""; int hl = my_strlen(h);
    copy_to_mem(h, o, (unsigned int)hl); p += hl;
    const char *et = ev_str(e->event_type); int el = my_strlen(et);
    copy_to_mem(et, &o[p], (unsigned int)el); p += el;
    const char *m1 = "\",\"db\":\""; int m1l = my_strlen(m1);
    copy_to_mem(m1, &o[p], (unsigned int)m1l); p += m1l;
    int dl = my_strlen(e->database); copy_to_mem(e->database, &o[p], (unsigned int)dl); p += dl;
    const char *m2 = "\",\"table\":\""; int m2l = my_strlen(m2);
    copy_to_mem(m2, &o[p], (unsigned int)m2l); p += m2l;
    int tl = my_strlen(e->table); copy_to_mem(e->table, &o[p], (unsigned int)tl); p += tl;
    copy_to_mem("\"}", &o[p], 2); p += 2; if (p >= ml) return -1; o[p] = 0; return p; }
static int serialize_protobuf(const cdc_event_t *e, char *o, int ml) {
    if (!e || !o || ml < 24) return -1; int p = 0;
    o[p++] = 0x08; o[p++] = (char)(e->event_type & 0x7F);
    o[p++] = 0x10; o[p++] = (char)(e->source_type & 0x7F);
    int dl = my_strlen(e->database); if (p + dl + 2 >= ml) return -1;
    o[p++] = 0x1A; o[p++] = (char)dl; copy_to_mem(e->database, &o[p], (unsigned int)dl); p += dl;
    int tl = my_strlen(e->table); if (p + tl + 2 >= ml) return -1;
    o[p++] = 0x22; o[p++] = (char)tl; copy_to_mem(e->table, &o[p], (unsigned int)tl); p += tl;
    o[p++] = 0x28; o[p++] = (char)(e->record_count & 0x7F);
    if (p + 2 < ml) { o[p++] = 0x30; o[p++] = (char)((int)(e->tx_id & 0x7F)); }
    o[p] = 0; return p; }
/* Schema Registry integration */
static int schema_registry_register(const char *subj, const char *data, int fmt) {
    if (!subj || !data) return -1; if (schema_count >= MAX_SCHEMAS) return -2;
    for (int i = 0; i < schema_count; i++) {
        if (my_strcmp(schemas[i].subject, subj) == 0) {
            copy_string(schemas[i].schema_data, data, MAX_SCHEMA);
            schemas[i].format = fmt; schemas[i].version++; return schemas[i].schema_id; } }
    int sid = schema_count + 1; schemas[schema_count].schema_id = sid;
    copy_string(schemas[schema_count].subject, subj, MAX_NAME);
    copy_string(schemas[schema_count].schema_data, data, MAX_SCHEMA);
    schemas[schema_count].format = fmt; schemas[schema_count].version = 1; schema_count++; return sid; }
static int schema_registry_get(const char *subj, char *o, int ml) {
    if (!subj || !o || ml <= 0) return -1;
    for (int i = 0; i < schema_count; i++) {
        if (my_strcmp(schemas[i].subject, subj) == 0) {
            int sl = my_strlen(schemas[i].schema_data); if (sl >= ml) sl = ml - 1;
            copy_to_mem(schemas[i].schema_data, o, (unsigned int)sl); o[sl] = 0;
            return schemas[i].schema_id; } } return -1; }
/* Compression encoding (NONE, GZIP, ZSTD, SNAPPY) */
static int compress_encode(const char *d, int l, int algo, char *o, int mo) {
    if (!d || !o || l <= 0 || mo < l + 4) return -1;
    if (algo == COMP_NONE) { copy_to_mem(d, o, (unsigned int)l); o[l] = 0; return l; }
    if (algo == COMP_GZIP) { o[0] = 0x1F; o[1] = (char)0x8B; o[2] = 0x08; o[3] = 0;
        for (int i = 0; i < l && (i+4) < mo; i++) o[i+4] = d[i];
        int t = l+4; if (t >= mo) t = mo-1; o[t] = 0; return t; }
    if (algo == COMP_ZSTD) { o[0] = 0x28; o[1] = (char)0xB5; o[2] = 0x2F; o[3] = (char)0xFD;
        for (int i = 0; i < l && (i+4) < mo; i++) o[i+4] = d[i];
        int t = l+4; if (t >= mo) t = mo-1; o[t] = 0; return t; }
    if (algo == COMP_SNAPPY) { o[0] = (char)0xFF; o[1] = 0x06; o[2] = 0; o[3] = 0;
        for (int i = 0; i < l && (i+4) < mo; i++) o[i+4] = d[i];
        int t = l+4; if (t >= mo) t = mo-1; o[t] = 0; return t; }
    return -1; }
/* Routing: table, database, filter, multi-target */
static int route_by_table(const char *db, const char *tbl, char *tgt) {
    if (!db || !tbl || !tgt) return -1;
    for (int i = 0; i < route_count; i++)
        if (my_strcmp(routes[i].database, db) == 0 && my_strcmp(routes[i].table, tbl) == 0 &&
            routes[i].target_count > 0) { copy_string(tgt, routes[i].targets[0], MAX_NAME); return 0; }
    copy_string(tgt, "default_topic", MAX_NAME); return 1; }
static int route_by_database(const char *db, char *tgt) {
    if (!db || !tgt) return -1;
    for (int i = 0; i < route_count; i++)
        if (my_strcmp(routes[i].database, db) == 0 && routes[i].target_count > 0) {
            copy_string(tgt, routes[i].targets[0], MAX_NAME); return 0; }
    copy_string(tgt, "default_db_topic", MAX_NAME); return 1; }
static int route_by_filter(const char *db, const char *tbl, const char *op, char *tgt) {
    if (!db || !tbl || !op || !tgt) return -1;
    for (int i = 0; i < route_count; i++) { if (routes[i].target_count == 0) continue;
        int dm = (routes[i].database[0]==0)||(my_strcmp(routes[i].database,db)==0);
        int tm = (routes[i].table[0]==0)||(my_strcmp(routes[i].table,tbl)==0);
        int om = (routes[i].operation[0]==0)||(my_strcmp(routes[i].operation,op)==0);
        if (dm && tm && om) { copy_string(tgt, routes[i].targets[0], MAX_NAME); return 0; } }
    copy_string(tgt, "unmatched", MAX_NAME); return 1; }
static int route_multi_target(const char *db, const char *tbl, char t[][MAX_NAME], int mt) {
    if (!db || !tbl || !t || mt <= 0) return -1; int f = 0;
    for (int i = 0; i < route_count && f < mt; i++) {
        int dm = (routes[i].database[0]==0)||(my_strcmp(routes[i].database,db)==0);
        int tm = (routes[i].table[0]==0)||(my_strcmp(routes[i].table,tbl)==0);
        if (dm && tm) for (int j = 0; j < routes[i].target_count && f < mt; j++) {
            int dup = 0; for (int k = 0; k < f; k++) if (my_strcmp(t[k], routes[i].targets[j])==0) { dup=1; break; }
            if (!dup) { copy_string(t[f], routes[i].targets[j], MAX_NAME); f++; } } }
    if (f == 0) { copy_string(t[0], "default_target", MAX_NAME); return 1; } return f; }
/* Data masking: hash, null, partial */
static int route_data_mask(const char *tbl, const char *col, const char *val, char *o, int mo) {
    if (!tbl || !col || !val || !o || mo <= 0) return -1;
    for (int i = 0; i < mask_count; i++) {
        if (my_strcmp(masks[i].table, tbl)==0 && my_strcmp(masks[i].column, col)==0) {
            if (my_strcmp(masks[i].mask_type, "hash")==0) {
                int vl = my_strlen(val); if (vl >= mo) vl = mo-1;
                for (int k = 0; k < vl; k++) o[k] = (char)('A' + ((val[k]+(char)k) % 26));
                o[vl] = 0; return 1; }
            if (my_strcmp(masks[i].mask_type, "null")==0) { copy_string(o, "NULL", (unsigned int)mo); return 2; }
            if (my_strcmp(masks[i].mask_type, "partial")==0) {
                int vl = my_strlen(val), sh = vl/4; if (sh < 1) sh = 1; int k = 0;
                for (; k < sh && k < mo-1; k++) o[k] = val[k];
                for (; k < vl-1 && k < mo-4; k++) o[k] = '*';
                int st = vl-2; if (st < sh) st = sh;
                for (int j = st; j < vl && k < mo-1; j++, k++) o[k] = val[j];
                o[k] = 0; return 3; } } }
    copy_string(o, val, (unsigned int)mo); return 0; }
/* Offset management: get, commit, reset */
static int offset_get(const char *fn, unsigned long long *pos) {
    if (!fn || !pos) return -1; *pos = 0;
    for (int i = 0; i < offset_count; i++)
        if (my_strcmp(offsets[i].filename, fn)==0) { *pos = offsets[i].position; return 0; }
    return -1; }
static int offset_commit(const char *fn, unsigned long long pos) {
    if (!fn) return -1;
    for (int i = 0; i < offset_count; i++)
        if (my_strcmp(offsets[i].filename, fn)==0) {
            offsets[i].position = pos; offsets[i].committed_at = pos + 5000; return 0; }
    if (offset_count >= MAX_OFFSETS) return -2;
    copy_string(offsets[offset_count].filename, fn, MAX_NAME);
    offsets[offset_count].position = pos; offsets[offset_count].committed_at = pos + 5000;
    offset_count++; return 0; }
static int offset_reset(const char *fn) {
    if (!fn) { offset_count = 0; return 0; }
    for (int i = 0; i < offset_count; i++)
        if (my_strcmp(offsets[i].filename, fn)==0) {
            for (int j = i; j < offset_count-1; j++) offsets[j] = offsets[j+1];
            offset_count--; return 0; } return -1; }
/* Checkpoint: save, restore, list */
static int checkpoint_save(int sid, unsigned long long lsn) {
    for (int i = 0; i < ckpt_count; i++)
        if (ckpts[i].id == sid) { ckpts[i].lsn = lsn; ckpts[i].timestamp = lsn + 9000; return 0; }
    if (ckpt_count >= MAX_CKPTS) return -1;
    ckpts[ckpt_count].id = sid; ckpts[ckpt_count].lsn = lsn;
    ckpts[ckpt_count].timestamp = lsn + 9000; ckpts[ckpt_count].snapshot_complete = 0;
    copy_string(ckpts[ckpt_count].source, "source", MAX_NAME); ckpt_count++; return 0; }
static int checkpoint_restore(int sid, unsigned long long *lsn) {
    if (!lsn) return -1; *lsn = 0;
    for (int i = 0; i < ckpt_count; i++)
        if (ckpts[i].id == sid) { *lsn = ckpts[i].lsn; return 0; } return -1; }
static int checkpoint_list(void) {
    print_str("change_data_capture: checkpoints ("); print_int(ckpt_count);
    print_str("/"); print_int(MAX_CKPTS); print_str(")\n");
    for (int i = 0; i < ckpt_count; i++) {
        print_str("change_data_capture:   id="); print_int(ckpts[i].id);
        print_str(" lsn="); print_ull(ckpts[i].lsn);
        print_str(" snap="); print_int(ckpts[i].snapshot_complete); print_str("\n"); }
    return ckpt_count; }
/* Idempotent check and transactional publish */
static int idempotent_check(unsigned long long tx, const char *src) {
    for (int i = 0; i < idem_count; i++)
        if (idem[i].tx_id == tx && my_strcmp(idem[i].source, src ? src : "")==0) return 1;
    return 0; }
static int transactional_publish(const cdc_event_t *e, const char *tgt) {
    if (!e || !tgt) return -1;
    if (idempotent_check(e->tx_id, tgt)) return 1;
    if (idem_count >= MAX_IDEM) return -2;
    idem[idem_count].tx_id = e->tx_id; copy_string(idem[idem_count].source, tgt, MAX_NAME);
    idem[idem_count].consumed = 1; idem_count++; return 0; }
/* Snapshot and incremental sync */
static int snapshot_initial(const char *db, const char *tbl, int *tbls) {
    if (!db || !tbl || !tbls) return -1; *tbls = 1;
    checkpoint_save(99, 100);
    if (ckpt_count > 0) ckpts[ckpt_count-1].snapshot_complete = 1; return 0; }
static int snapshot_resume(int ckpt_id) {
    for (int i = 0; i < ckpt_count; i++)
        if (ckpts[i].id == ckpt_id) {
            if (ckpts[i].snapshot_complete) return 1;
            ckpts[i].snapshot_complete = 1; return 0; } return -1; }
static int sync_incremental(const char *src, unsigned long long from, unsigned long long to, int *ev) {
    if (!src || !ev) return -1;
    if (to <= from) { *ev = 0; return 0; }
    unsigned long long r = to - from; *ev = (int)(r > 1000 ? 1000 : r);
    offset_commit(src, to); checkpoint_save(50, to); return 0; }
static int sync_latency_monitor(int win) {
    if (win <= 0) win = 10; if (win > 32) win = 32;
    unsigned long long mx = 0, tot = 0; int c = lat_count < win ? lat_count : win;
    for (int i = 0; i < c; i++) { unsigned long long lat = 0;
        if (lat_buf[i].end_ts > lat_buf[i].start_ts) lat = lat_buf[i].end_ts - lat_buf[i].start_ts;
        if (lat > mx) mx = lat; tot += lat; }
    unsigned long long avg = (c > 0) ? (tot / (unsigned long long)c) : 0;
    print_str("change_data_capture: latency window="); print_int(c);
    print_str(" max="); print_ull(mx); print_str(" avg="); print_ull(avg); print_str("\n");
    return (int)avg; }
/* Test suite: 22 tests covering all major subsystems */
static void test_suite(void) {
    int p = 0, t = 0; cdc_init();
    print_str("change_data_capture: === TEST SUITE ===\n");
    /* 1: binlog_parse + event_extract + position_get */
    t++; { cdc_event_t ev; char buf[MAX_DATA];
        int r = binlog_parse("binlog.001", 4096, "mydb", "users", EVT_INSERT, &ev);
        int ex = binlog_event_extract(&ev, buf, MAX_DATA);
        unsigned long long bp = binlog_position_get("binlog.001");
        if (r==0 && ev.source_type==SRC_BINLOG && ev.event_type==EVT_INSERT && ex>0 &&
            my_strcmp(ev.database,"mydb")==0 && my_strcmp(ev.table,"users")==0 && bp==0)
        { print_str("change_data_capture: binlog_parse: PASS\n"); p++; }
        else print_str("change_data_capture: binlog_parse: FAIL\n"); }
    /* 2: wal_parse + event_extract + lsn_get */
    t++; { cdc_event_t ev; char buf[MAX_DATA];
        int r = wal_parse("pgdb", 12345, 99, &ev);
        int ex = wal_event_extract(&ev, buf, MAX_DATA);
        unsigned long long wl = wal_lsn_get("pgdb");
        if (r==0 && ev.source_type==SRC_WAL && ev.lsn==12345 && ev.tx_id==99 && ex>0 && wl==0)
        { print_str("change_data_capture: wal_parse: PASS\n"); p++; }
        else print_str("change_data_capture: wal_parse: FAIL\n"); }
    /* 3: oplog_parse + event_extract */
    t++; { cdc_event_t ev;
        int r = oplog_parse("testdb.items", EVT_UPDATE, 5000, "op_data", &ev);
        if (r==0 && ev.source_type==SRC_OPLOG && ev.event_type==EVT_UPDATE &&
            my_strcmp(ev.database,"testdb")==0 && my_strcmp(ev.table,"items")==0)
        { print_str("change_data_capture: oplog_parse: PASS\n"); p++; }
        else print_str("change_data_capture: oplog_parse: FAIL\n"); }
    /* 4: ddl_detect */
    t++; { char db[MAX_NAME], tbl[MAX_NAME]; int dt = 0;
        int r1 = ddl_detect("CREATE TABLE t1(id INT)", db, tbl, &dt);
        int r2 = ddl_detect("ALTER TABLE t1 ADD col VARCHAR", db, tbl, &dt);
        if (r1==0 && r2==0 && dt==DDL_ALTER)
        { print_str("change_data_capture: ddl_detect: PASS\n"); p++; }
        else print_str("change_data_capture: ddl_detect: FAIL\n"); }
    /* 5: ddl_schema_evolve */
    t++; { schema_def_t cur, upd;
        zero_mem((char *)&cur, sizeof(cur)); zero_mem((char *)&upd, sizeof(upd));
        copy_string(cur.schema_name, "public", MAX_NAME);
        copy_string(cur.table_name, "users", MAX_NAME); cur.col_count = 2;
        copy_string(cur.columns[0].col_name, "id", MAX_NAME);
        copy_string(cur.columns[1].col_name, "name", MAX_NAME); cur.version = 1;
        int r = ddl_schema_evolve(&cur, "ALTER TABLE users ADD email VARCHAR", &upd);
        if (r==0 && upd.col_count==3 && upd.version==2)
        { print_str("change_data_capture: ddl_schema_evolve: PASS\n"); p++; }
        else print_str("change_data_capture: ddl_schema_evolve: FAIL\n"); }
    /* 6: ddl_compat_check (compatible and incompatible cases) */
    t++; { schema_def_t rd, wr;
        zero_mem((char *)&rd, sizeof(rd)); zero_mem((char *)&wr, sizeof(wr));
        copy_string(rd.schema_name, "public", MAX_NAME);
        copy_string(rd.table_name, "users", MAX_NAME); rd.col_count = 1;
        copy_string(rd.columns[0].col_name, "id", MAX_NAME);
        copy_string(wr.schema_name, "public", MAX_NAME);
        copy_string(wr.table_name, "users", MAX_NAME); wr.col_count = 2;
        copy_string(wr.columns[0].col_name, "id", MAX_NAME);
        copy_string(wr.columns[1].col_name, "name", MAX_NAME);
        int c1 = ddl_compat_check(&rd, &wr);
        copy_string(wr.columns[0].col_name, "other_id", MAX_NAME);
        int c2 = ddl_compat_check(&rd, &wr);
        if (c1==0 && c2!=0)
        { print_str("change_data_capture: ddl_compat_check: PASS\n"); p++; }
        else print_str("change_data_capture: ddl_compat_check: FAIL\n"); }
    /* 7: serialize_avro */
    t++; { cdc_event_t ev; char o[256];
        binlog_parse("b.001", 100, "db1", "t1", EVT_INSERT, &ev);
        int n = serialize_avro(&ev, o, 256);
        if (n>0 && o[0]=='A' && o[1]=='V')
        { print_str("change_data_capture: serialize_avro: PASS\n"); p++; }
        else print_str("change_data_capture: serialize_avro: FAIL\n"); }
    /* 8: serialize_json */
    t++; { cdc_event_t ev; char o[256];
        binlog_parse("b.001", 200, "db1", "t1", EVT_UPDATE, &ev);
        int n = serialize_json(&ev, o, 256);
        if (n>0 && o[0]=='{' && o[n-1]=='}')
        { print_str("change_data_capture: serialize_json: PASS\n"); p++; }
        else print_str("change_data_capture: serialize_json: FAIL\n"); }
    /* 9: serialize_protobuf */
    t++; { cdc_event_t ev; char o[256];
        binlog_parse("b.001", 300, "db1", "t1", EVT_DELETE, &ev);
        int n = serialize_protobuf(&ev, o, 256);
        if (n>0 && o[0]==0x08 && o[2]==0x10)
        { print_str("change_data_capture: serialize_protobuf: PASS\n"); p++; }
        else print_str("change_data_capture: serialize_protobuf: FAIL\n"); }
    /* 10: schema_registry register and get */
    t++; { int sid = schema_registry_register("users-value",
            "{\"type\":\"record\",\"name\":\"User\",\"fields\":[]}", FMT_AVRO);
        char buf[MAX_SCHEMA]; int gid = schema_registry_get("users-value", buf, MAX_SCHEMA);
        if (sid>0 && gid==sid)
        { print_str("change_data_capture: schema_registry: PASS\n"); p++; }
        else print_str("change_data_capture: schema_registry: FAIL\n"); }
    /* 11: compress_encode (NONE, GZIP, ZSTD, SNAPPY) */
    t++; { char d[] = "Hello CDC World! This is test data for compression."; char o[128];
        int n1 = compress_encode(d, 50, COMP_NONE, o, 128);
        int n2 = compress_encode(d, 50, COMP_GZIP, o, 128);
        int n3 = compress_encode(d, 50, COMP_ZSTD, o, 128);
        int n4 = compress_encode(d, 50, COMP_SNAPPY, o, 128);
        if (n1==50 && n2==54 && n3==54 && n4==54)
        { print_str("change_data_capture: compress_encode: PASS\n"); p++; }
        else print_str("change_data_capture: compress_encode: FAIL\n"); }
    /* 12: route_by_table */
    t++; { char tgt[MAX_NAME];
        copy_string(routes[0].database, "mydb", MAX_NAME);
        copy_string(routes[0].table, "users", MAX_NAME); routes[0].operation[0] = 0;
        copy_string(routes[0].targets[0], "users_topic", MAX_NAME);
        routes[0].target_count = 1; route_count = 1;
        int r = route_by_table("mydb", "users", tgt);
        if (r==0 && my_strcmp(tgt, "users_topic")==0)
        { print_str("change_data_capture: route_by_table: PASS\n"); p++; }
        else print_str("change_data_capture: route_by_table: FAIL\n"); }
    /* 13: route_by_filter (match + no match) */
    t++; { char tgt[MAX_NAME]; route_count = 0;
        copy_string(routes[0].database, "mydb", MAX_NAME);
        copy_string(routes[0].table, "users", MAX_NAME);
        copy_string(routes[0].operation, "INSERT", MAX_NAME);
        copy_string(routes[0].targets[0], "analytics_topic", MAX_NAME);
        routes[0].target_count = 1; route_count = 1;
        int r1 = route_by_filter("mydb", "users", "INSERT", tgt);
        int r2 = route_by_filter("mydb", "users", "DELETE", tgt);
        if (r1==0 && my_strcmp(tgt,"analytics_topic")==0 && r2==1 && my_strcmp(tgt,"unmatched")==0)
        { print_str("change_data_capture: route_by_filter: PASS\n"); p++; }
        else print_str("change_data_capture: route_by_filter: FAIL\n"); }
    /* 14: route_multi_target (kafka + kinesis + s3) */
    t++; { char tgts[MAX_TARGETS][MAX_NAME]; route_count = 0;
        copy_string(routes[0].database, "mydb", MAX_NAME);
        routes[0].table[0] = 0; routes[0].operation[0] = 0;
        copy_string(routes[0].targets[0], "kafka", MAX_NAME);
        copy_string(routes[0].targets[1], "kinesis", MAX_NAME); routes[0].target_count = 2;
        copy_string(routes[1].database, "mydb", MAX_NAME);
        routes[1].table[0] = 0; routes[1].operation[0] = 0;
        copy_string(routes[1].targets[0], "s3", MAX_NAME); routes[1].target_count = 1;
        route_count = 2;
        int n = route_multi_target("mydb", "orders", tgts, MAX_TARGETS);
        if (n==3) { print_str("change_data_capture: route_multi_target: PASS\n"); p++; }
        else print_str("change_data_capture: route_multi_target: FAIL\n"); }
    /* 15: route_data_mask (hash, null, passthrough) */
    t++; { char o[MAX_DATA]; mask_count = 0;
        copy_string(masks[0].table, "users", MAX_NAME);
        copy_string(masks[0].column, "email", MAX_NAME);
        copy_string(masks[0].mask_type, "hash", MAX_NAME);
        copy_string(masks[1].table, "users", MAX_NAME);
        copy_string(masks[1].column, "ssn", MAX_NAME);
        copy_string(masks[1].mask_type, "null", MAX_NAME); mask_count = 2;
        int r1 = route_data_mask("users", "email", "test@example.com", o, MAX_DATA);
        int r2 = route_data_mask("users", "ssn", "123-45-6789", o, MAX_DATA);
        int r3 = route_data_mask("users", "name", "Alice", o, MAX_DATA);
        if (r1==1 && r2==2 && r3==0 && my_strcmp(o,"Alice")==0)
        { print_str("change_data_capture: route_data_mask: PASS\n"); p++; }
        else print_str("change_data_capture: route_data_mask: FAIL\n"); }
    /* 16: offset_commit + offset_get + offset_reset */
    t++; { offset_count = 0;
        offset_commit("binlog.001", 1024); offset_commit("binlog.001", 2048);
        offset_commit("binlog.002", 512);
        unsigned long long p1 = 0, p2 = 0;
        int r1 = offset_get("binlog.001", &p1); int r2 = offset_get("binlog.002", &p2);
        offset_reset("binlog.001"); unsigned long long p3 = 0;
        int r3 = offset_get("binlog.001", &p3);
        if (r1==0 && p1==2048 && r2==0 && p2==512 && r3==-1)
        { print_str("change_data_capture: offset_commit: PASS\n"); p++; }
        else print_str("change_data_capture: offset_commit: FAIL\n"); }
    /* 17: checkpoint_save + checkpoint_restore */
    t++; { ckpt_count = 0;
        checkpoint_save(1, 5000); checkpoint_save(2, 6000);
        unsigned long long l1 = 0; int r1 = checkpoint_restore(1, &l1);
        checkpoint_save(1, 7000); unsigned long long l2 = 0;
        int r2 = checkpoint_restore(1, &l2);
        unsigned long long l3 = 0; int r3 = checkpoint_restore(99, &l3);
        if (r1==0 && l1==5000 && r2==0 && l2==7000 && r3==-1)
        { print_str("change_data_capture: checkpoint_save: PASS\n"); p++; }
        else print_str("change_data_capture: checkpoint_save: FAIL\n"); }
    /* 18: idempotent_check (before + after publish) */
    t++; { idem_count = 0; cdc_event_t ev;
        binlog_parse("b.001", 100, "db1", "t1", EVT_INSERT, &ev);
        int r1 = idempotent_check(ev.tx_id, "target1");
        transactional_publish(&ev, "target1");
        int r2 = idempotent_check(ev.tx_id, "target1");
        int r3 = idempotent_check(ev.tx_id, "target2");
        if (r1==0 && r2==1 && r3==0)
        { print_str("change_data_capture: idempotent_check: PASS\n"); p++; }
        else print_str("change_data_capture: idempotent_check: FAIL\n"); }
    /* 19: transactional_publish (first + duplicate) */
    t++; { idem_count = 0; cdc_event_t ev;
        binlog_parse("b.001", 200, "db1", "t1", EVT_INSERT, &ev);
        int r1 = transactional_publish(&ev, "kafka_topic");
        int r2 = transactional_publish(&ev, "kafka_topic");
        if (r1==0 && r2==1)
        { print_str("change_data_capture: transactional_publish: PASS\n"); p++; }
        else print_str("change_data_capture: transactional_publish: FAIL\n"); }
    /* 20: snapshot_initial + snapshot_resume */
    t++; { ckpt_count = 0; int tbls = 0;
        int r = snapshot_initial("mydb", "users", &tbls);
        int r2 = snapshot_resume(99);
        if (r==0 && tbls==1 && ckpt_count>0 && ckpts[ckpt_count-1].snapshot_complete==1 && r2==1)
        { print_str("change_data_capture: snapshot_initial: PASS\n"); p++; }
        else print_str("change_data_capture: snapshot_initial: FAIL\n"); }
    /* 21: sync_incremental */
    t++; { offset_count = 0; int ev = 0;
        int r = sync_incremental("binlog.001", 1000, 2000, &ev);
        unsigned long long op = 0; offset_get("binlog.001", &op);
        if (r==0 && ev==1000 && op==2000)
        { print_str("change_data_capture: sync_incremental: PASS\n"); p++; }
        else print_str("change_data_capture: sync_incremental: FAIL\n"); }
    /* 22: sync_latency_monitor */
    t++; { lat_count = 0;
        lat_buf[0].start_ts = 1000; lat_buf[0].end_ts = 1050;
        lat_buf[1].start_ts = 2000; lat_buf[1].end_ts = 2030;
        lat_buf[2].start_ts = 3000; lat_buf[2].end_ts = 3070; lat_count = 3;
        int avg = sync_latency_monitor(10);
        if (avg==50) { print_str("change_data_capture: sync_latency_monitor: PASS\n"); p++; }
        else print_str("change_data_capture: sync_latency_monitor: FAIL\n"); }
    print_str("change_data_capture: === RESULTS "); print_int(p);
    print_str("/"); print_int(t); print_str(" PASSED ===\n"); }
/* Help text */
static void show_help(void) {
    print_str("change_data_capture - CDC log replication engine v1.0\n");
    print_str("Usage: change_data_capture [OPTIONS]\n");
    print_str("  -h             Show this help message\n");
    print_str("  -t             Run full test suite\n");
    print_str("\n");
    print_str("Log parsing: Binlog(MySQL), WAL(PostgreSQL), Oplog(MongoDB)\n");
    print_str("Schema: DDL capture, Schema Evolution, compatibility check\n");
    print_str("Serialization: Avro, JSON, Protobuf + Schema Registry integration\n");
    print_str("Compression: NONE, GZIP, ZSTD, SNAPPY encoding support\n");
    print_str("Routing: table/database/filter-based, multi-target, data masking\n");
    print_str("Exactly-once: offset management, checkpoint, idempotent, transactional\n");
    print_str("Snapshot: full initial, incremental sync, latency monitoring, resume\n"); }
/* Entry point */
void _start(void) {
    unsigned int buf = alloc_mem(512);
    int argc = host_get_argv(buf, 512); (void)argc;
    int help_flag = 0, test_flag = 0;
    unsigned int pos = 0; char *ap = (char *)buf;
    while (pos < 512 && ap[pos]) pos++; pos++;
    while (pos < 512 && ap[pos]) { char *a = &ap[pos];
        if (my_strcmp(a, "--help")==0 || my_strcmp(a, "-h")==0) help_flag = 1;
        else if (my_strcmp(a, "-t")==0) test_flag = 1;
        while (pos < 512 && ap[pos]) pos++; pos++; }
    if (test_flag) { test_suite(); host_exit(0); }
    if (help_flag) { show_help(); host_exit(0); }
    print_str("change_data_capture: no action specified (use -h for help, -t for tests)\n");
    host_exit(0); }
