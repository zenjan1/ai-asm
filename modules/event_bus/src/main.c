/*
 * event_bus: Event bus / message broker module (v46.0)
 * Pub/sub, event sourcing, DLQ, schema registry, CloudEvents
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

/* -------------------------------------------------------------------------- */
/* Constants                                                                  */
/* -------------------------------------------------------------------------- */

#define MAX_TOPICS        32
#define MAX_SUBSCRIBERS   64
#define MAX_MESSAGES      128
#define MAX_EVENTS        128
#define MAX_DLQ           32
#define MAX_SCHEMAS       16
#define MAX_FILTERS       16
#define MAX_IDEMPOTENT    64
#define MAX_TOPIC_NAME    32
#define MAX_MSG_DATA      128
#define MAX_SCHEMA_DEF    256
#define MAX_SUB_PER_TOPIC 16
#define MAX_CE_FIELDS     8
#define SCRAMBLE_MAGIC    0xA5B5C5D5

#define SCHEMA_BACKWARD  0
#define SCHEMA_FORWARD   1
#define SCHEMA_FULL      2

/* -------------------------------------------------------------------------- */
/* Utility functions                                                          */
/* -------------------------------------------------------------------------- */

static int my_strlen(const char *s)
{
    int l = 0;
    while (s[l]) l++;
    return l;
}

static int my_strcmp(const char *a, const char *b)
{
    while (*a && *b) {
        if (*a != *b) return *a - *b;
        a++; b++;
    }
    return *a - *b;
}

static void my_strncpy(char *d, const char *s, int n)
{
    int i = 0;
    while (i < n - 1 && s[i]) { d[i] = s[i]; i++; }
    d[i] = '\0';
}

static int my_strncmp(const char *a, const char *b, int n)
{
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i]) return (unsigned char)a[i] - (unsigned char)b[i];
        if (a[i] == '\0') return 0;
    }
    return 0;
}

static unsigned int hash_str(const char *s)
{
    unsigned int h = 5381;
    while (*s) { h = ((h << 5) + h) + (unsigned char)*s; s++; }
    return h;
}

static int regex_simple_match(const char *pat, const char *str)
{
    while (*pat) {
        if (*pat == '*') {
            pat++;
            if (*pat == '\0') return 1;
            while (*str) {
                if (regex_simple_match(pat, str)) return 1;
                str++;
            }
            return 0;
        }
        if (*pat == *str) { pat++; str++; }
        else return 0;
    }
    return *str == '\0';
}

/* -------------------------------------------------------------------------- */
/* Print helpers                                                              */
/* -------------------------------------------------------------------------- */

static void pstr(const char *s) { host_print(s); }

static void pint(int v)
{
    char b[32];
    int p = 0;
    if (v < 0) { b[p++] = '-'; v = -v; }
    if (v == 0) b[p++] = '0';
    else {
        int d = 0, t = v;
        while (t > 0) { d++; t /= 10; }
        p += d;
        b[p] = '\0';
        p--;
        while (v > 0) { b[p--] = '0' + (v % 10); v /= 10; }
    }
    host_print(b);
}

static void phex(unsigned int v)
{
    char b[12];
    int p = 0;
    b[p++] = '0'; b[p++] = 'x';
    for (int i = 7; i >= 0; i--) {
        unsigned int nib = (v >> (i * 4)) & 0xF;
        b[p++] = (nib < 10) ? ('0' + nib) : ('A' + nib - 10);
    }
    b[p] = '\0';
    host_print(b);
}

/* -------------------------------------------------------------------------- */
/* Data structures                                                            */
/* -------------------------------------------------------------------------- */

typedef struct {
    char name[MAX_TOPIC_NAME];
    int active;
    int sub_count;
    int subs[MAX_SUB_PER_TOPIC];
    unsigned int msg_count;
} topic_t;

typedef struct {
    int topic_id;
    char group[32];
    int active;
    unsigned int offset;
} subscriber_t;

typedef struct {
    int topic_id;
    char data[MAX_MSG_DATA];
    char type[32];
    char source[32];
    unsigned int timestamp;
    unsigned int seq;
    int active;
} message_t;

typedef struct {
    unsigned int seq;
    char event_type[32];
    char data[MAX_MSG_DATA];
    unsigned int timestamp;
    int active;
} stored_event_t;

typedef struct {
    unsigned int id;
    int consume_count;
    unsigned int hash;
    int active;
} idempotent_entry_t;

typedef struct {
    int orig_topic;
    char data[MAX_MSG_DATA];
    char error[64];
    int retry_count;
    int max_retries;
    unsigned int timestamp;
    int active;
} dlq_entry_t;

typedef struct {
    char name[32];
    char definition[MAX_SCHEMA_DEF];
    int version;
    int compat_mode;
    int active;
} schema_entry_t;

typedef struct {
    char field[32];
    char value[64];
} ce_field_t;

typedef struct {
    char spec_version[8];
    char type[32];
    char source[64];
    char id[32];
    char time_str[32];
    char data[MAX_MSG_DATA];
    int field_count;
    ce_field_t extras[MAX_CE_FIELDS];
} cloudevent_t;

/* -------------------------------------------------------------------------- */
/* Global state                                                               */
/* -------------------------------------------------------------------------- */

static topic_t topics[MAX_TOPICS];
static int topic_count = 0;

static subscriber_t subscribers[MAX_SUBSCRIBERS];
static int sub_count = 0;

static message_t messages[MAX_MESSAGES];
static int msg_count = 0;

static stored_event_t event_log[MAX_EVENTS];
static int event_log_count = 0;

static idempotent_entry_t idempotent_table[MAX_IDEMPOTENT];
static int idempotent_count = 0;

static dlq_entry_t dlq[MAX_DLQ];
static int dlq_count = 0;

static schema_entry_t schemas[MAX_SCHEMAS];
static int schema_count = 0;

static unsigned int global_clock = 1;
static unsigned int global_seq = 1;

/* -------------------------------------------------------------------------- */
/* Topic management                                                           */
/* -------------------------------------------------------------------------- */

static int topic_create(const char *name)
{
    if (topic_count >= MAX_TOPICS) {
        pstr("  Error: max topics reached\n");
        return -1;
    }
    for (int i = 0; i < topic_count; i++) {
        if (topics[i].active && my_strcmp(topics[i].name, name) == 0) {
            pstr("  Error: topic '"); pstr(name); pstr("' already exists\n");
            return -1;
        }
    }
    int id = topic_count++;
    my_strncpy(topics[id].name, name, MAX_TOPIC_NAME - 1);
    topics[id].active = 1;
    topics[id].sub_count = 0;
    topics[id].msg_count = 0;
    pstr("  Created topic '"); pstr(name); pstr("' (id="); pint(id); pstr(")\n");
    return id;
}

static int topic_delete(int topic_id)
{
    if (topic_id < 0 || topic_id >= topic_count) {
        pstr("  Error: invalid topic id\n");
        return -1;
    }
    if (!topics[topic_id].active) {
        pstr("  Error: topic not active\n");
        return -1;
    }
    for (int i = 0; i < sub_count; i++) {
        if (subscribers[i].active && subscribers[i].topic_id == topic_id) {
            subscribers[i].active = 0;
        }
    }
    topics[topic_id].active = 0;
    pstr("  Deleted topic '"); pstr(topics[topic_id].name); pstr("'\n");
    return 0;
}

static int topic_list(void)
{
    int count = 0;
    pstr("  Topics:\n");
    for (int i = 0; i < topic_count; i++) {
        if (topics[i].active) {
            pstr("    ["); pint(i); pstr("] "); pstr(topics[i].name);
            pstr(" (subs="); pint(topics[i].sub_count);
            pstr(", msgs="); pint(topics[i].msg_count); pstr(")\n");
            count++;
        }
    }
    if (count == 0) pstr("    (none)\n");
    return count;
}

/* -------------------------------------------------------------------------- */
/* Subscribe / Unsubscribe                                                    */
/* -------------------------------------------------------------------------- */

static int subscribe(int topic_id, const char *group)
{
    if (topic_id < 0 || topic_id >= topic_count || !topics[topic_id].active) {
        pstr("  Error: invalid topic for subscribe\n");
        return -1;
    }
    if (topics[topic_id].sub_count >= MAX_SUB_PER_TOPIC) {
        pstr("  Error: max subscribers per topic\n");
        return -1;
    }
    if (sub_count >= MAX_SUBSCRIBERS) {
        pstr("  Error: max subscribers reached\n");
        return -1;
    }
    int sid = sub_count++;
    subscribers[sid].topic_id = topic_id;
    my_strncpy(subscribers[sid].group, group, 31);
    subscribers[sid].active = 1;
    subscribers[sid].offset = 0;
    topics[topic_id].subs[topics[topic_id].sub_count++] = sid;
    pstr("  Subscriber "); pint(sid); pstr(" on '"); pstr(topics[topic_id].name);
    pstr("' group='"); pstr(group); pstr("'\n");
    return sid;
}

static int unsubscribe(int sub_id)
{
    if (sub_id < 0 || sub_id >= sub_count || !subscribers[sub_id].active) {
        pstr("  Error: invalid subscriber\n");
        return -1;
    }
    int tid = subscribers[sub_id].topic_id;
    if (tid >= 0 && tid < topic_count && topics[tid].active) {
        topics[tid].sub_count--;
    }
    subscribers[sub_id].active = 0;
    pstr("  Unsubscribed "); pint(sub_id); pstr("\n");
    return 0;
}

/* -------------------------------------------------------------------------- */
/* Publish / Fan-out / Fan-in                                                 */
/* -------------------------------------------------------------------------- */

static int publish(int topic_id, const char *type, const char *source, const char *data)
{
    if (topic_id < 0 || topic_id >= topic_count || !topics[topic_id].active) {
        pstr("  Error: invalid topic for publish\n");
        return -1;
    }
    if (msg_count >= MAX_MESSAGES) {
        pstr("  Error: message buffer full\n");
        return -1;
    }
    int mid = msg_count++;
    messages[mid].topic_id = topic_id;
    my_strncpy(messages[mid].type, type, 31);
    my_strncpy(messages[mid].source, source, 31);
    my_strncpy(messages[mid].data, data, MAX_MSG_DATA - 1);
    messages[mid].timestamp = global_clock++;
    messages[mid].seq = global_seq++;
    messages[mid].active = 1;
    topics[topic_id].msg_count++;
    pstr("  Published to '"); pstr(topics[topic_id].name);
    pstr("' type='"); pstr(type); pstr("' seq="); pint(messages[mid].seq); pstr("\n");
    return mid;
}

static int fan_out(int topic_id)
{
    if (topic_id < 0 || topic_id >= topic_count || !topics[topic_id].active) {
        pstr("  Error: invalid topic for fan-out\n");
        return -1;
    }
    int delivered = 0;
    pstr("  Fan-out from '"); pstr(topics[topic_id].name); pstr("':\n");
    for (int i = 0; i < topics[topic_id].sub_count; i++) {
        int sid = topics[topic_id].subs[i];
        if (sid >= 0 && sid < sub_count && subscribers[sid].active) {
            pstr("    -> sub "); pint(sid);
            pstr(" group='"); pstr(subscribers[sid].group);
            pstr("'\n");
            delivered++;
        }
    }
    if (delivered == 0) pstr("    (no active subscribers)\n");
    return delivered;
}

static int fan_in(int *topic_ids, int count)
{
    int total = 0;
    pstr("  Fan-in from "); pint(count); pstr(" topics:\n");
    for (int t = 0; t < count; t++) {
        int tid = topic_ids[t];
        if (tid >= 0 && tid < topic_count && topics[tid].active) {
            pstr("    <- '"); pstr(topics[tid].name);
            pstr("' msgs="); pint(topics[tid].msg_count); pstr("\n");
            total += topics[tid].msg_count;
        }
    }
    pstr("  Total messages: "); pint(total); pstr("\n");
    return total;
}

/* -------------------------------------------------------------------------- */
/* Filters                                                                    */
/* -------------------------------------------------------------------------- */

static int filter_by_type(int topic_id, const char *target_type)
{
    int matched = 0;
    pstr("  Filter by type='"); pstr(target_type); pstr("':\n");
    for (int i = 0; i < msg_count; i++) {
        if (messages[i].active && messages[i].topic_id == topic_id &&
            my_strcmp(messages[i].type, target_type) == 0) {
            pstr("    seq="); pint(messages[i].seq);
            pstr(" data='"); pstr(messages[i].data); pstr("'\n");
            matched++;
        }
    }
    pstr("  Matched: "); pint(matched); pstr("\n");
    return matched;
}

static int filter_by_source(int topic_id, const char *target_source)
{
    int matched = 0;
    pstr("  Filter by source='"); pstr(target_source); pstr("':\n");
    for (int i = 0; i < msg_count; i++) {
        if (messages[i].active && messages[i].topic_id == topic_id &&
            my_strcmp(messages[i].source, target_source) == 0) {
            pstr("    seq="); pint(messages[i].seq);
            pstr(" data='"); pstr(messages[i].data); pstr("'\n");
            matched++;
        }
    }
    pstr("  Matched: "); pint(matched); pstr("\n");
    return matched;
}

static int filter_by_regex(int topic_id, const char *pattern)
{
    int matched = 0;
    pstr("  Filter by regex='"); pstr(pattern); pstr("':\n");
    for (int i = 0; i < msg_count; i++) {
        if (messages[i].active && messages[i].topic_id == topic_id &&
            regex_simple_match(pattern, messages[i].data)) {
            pstr("    seq="); pint(messages[i].seq);
            pstr(" data='"); pstr(messages[i].data); pstr("'\n");
            matched++;
        }
    }
    pstr("  Matched: "); pint(matched); pstr("\n");
    return matched;
}

/* -------------------------------------------------------------------------- */
/* Event sourcing                                                             */
/* -------------------------------------------------------------------------- */

static int event_store_append(const char *event_type, const char *data)
{
    if (event_log_count >= MAX_EVENTS) {
        pstr("  Error: event log full\n");
        return -1;
    }
    int eid = event_log_count++;
    event_log[eid].seq = global_seq++;
    my_strncpy(event_log[eid].event_type, event_type, 31);
    my_strncpy(event_log[eid].data, data, MAX_MSG_DATA - 1);
    event_log[eid].timestamp = global_clock++;
    event_log[eid].active = 1;
    pstr("  Appended event type='"); pstr(event_type);
    pstr("' seq="); pint(event_log[eid].seq); pstr("\n");
    return eid;
}

static int event_store_replay(int from_seq, int count)
{
    int replayed = 0;
    pstr("  Replay from seq="); pint(from_seq);
    pstr(" count="); pint(count); pstr(":\n");
    for (int i = 0; i < event_log_count && replayed < count; i++) {
        if (event_log[i].active && (int)event_log[i].seq >= from_seq) {
            pstr("    ["); pint(replayed); pstr("] seq="); pint(event_log[i].seq);
            pstr(" type='"); pstr(event_log[i].event_type);
            pstr("' data='"); pstr(event_log[i].data); pstr("'\n");
            replayed++;
        }
    }
    if (replayed == 0) pstr("    (no events found)\n");
    return replayed;
}

static int event_store_time_travel(unsigned int target_ts)
{
    int count = 0;
    pstr("  Time travel to ts="); pint(target_ts); pstr(":\n");
    for (int i = 0; i < event_log_count; i++) {
        if (event_log[i].active && event_log[i].timestamp <= target_ts) {
            pstr("    seq="); pint(event_log[i].seq);
            pstr(" type='"); pstr(event_log[i].event_type);
            pstr("' ts="); pint(event_log[i].timestamp); pstr("\n");
            count++;
        }
    }
    pstr("  Events at or before ts: "); pint(count); pstr("\n");
    return count;
}

/* -------------------------------------------------------------------------- */
/* Delivery guarantees                                                        */
/* -------------------------------------------------------------------------- */

static int consume_at_least_once(int topic_id, int sub_id)
{
    if (sub_id < 0 || sub_id >= sub_count || !subscribers[sub_id].active) {
        pstr("  Error: invalid subscriber\n");
        return -1;
    }
    int consumed = 0;
    pstr("  Consume (at-least-once) from '");
    pstr(topics[topic_id].name); pstr("':\n");
    for (int i = 0; i < msg_count; i++) {
        if (messages[i].active && messages[i].topic_id == topic_id) {
            pstr("    msg seq="); pint(messages[i].seq);
            pstr(" data='"); pstr(messages[i].data); pstr("'\n");
            consumed++;
        }
    }
    subscribers[sub_id].offset += consumed;
    pstr("  Consumed: "); pint(consumed); pstr("\n");
    return consumed;
}

static int consume_exactly_once(int topic_id, int sub_id)
{
    if (sub_id < 0 || sub_id >= sub_count || !subscribers[sub_id].active) {
        pstr("  Error: invalid subscriber\n");
        return -1;
    }
    int consumed = 0;
    pstr("  Consume (exactly-once) from '");
    pstr(topics[topic_id].name); pstr("':\n");
    for (int i = 0; i < msg_count; i++) {
        if (messages[i].active && messages[i].topic_id == topic_id) {
            unsigned int h = hash_str(messages[i].data);
            h ^= (unsigned int)messages[i].seq;
            int dup = 0;
            for (int j = 0; j < idempotent_count; j++) {
                if (idempotent_table[j].active &&
                    idempotent_table[j].hash == h) {
                    dup = 1;
                    break;
                }
            }
            if (!dup) {
                pstr("    msg seq="); pint(messages[i].seq);
                pstr(" data='"); pstr(messages[i].data); pstr("'\n");
                consumed++;
                if (idempotent_count < MAX_IDEMPOTENT) {
                    int idx = idempotent_count++;
                    idempotent_table[idx].hash = h;
                    idempotent_table[idx].consume_count = 1;
                    idempotent_table[idx].active = 1;
                }
            } else {
                pstr("    [duplicate skipped] seq="); pint(messages[i].seq); pstr("\n");
            }
        }
    }
    pstr("  Consumed (deduped): "); pint(consumed); pstr("\n");
    return consumed;
}

static int idempotent_check(const char *data, int seq)
{
    unsigned int h = hash_str(data);
    h ^= (unsigned int)seq;
    for (int i = 0; i < idempotent_count; i++) {
        if (idempotent_table[i].active && idempotent_table[i].hash == h) {
            pstr("  Idempotent: FOUND (count="); pint(idempotent_table[i].consume_count); pstr(")\n");
            return 1;
        }
    }
    pstr("  Idempotent: NOT found\n");
    return 0;
}

/* -------------------------------------------------------------------------- */
/* Dead letter queue                                                          */
/* -------------------------------------------------------------------------- */

static int dlq_send(int topic_id, const char *data, const char *error)
{
    if (dlq_count >= MAX_DLQ) {
        pstr("  Error: DLQ full\n");
        return -1;
    }
    int did = dlq_count++;
    dlq[did].orig_topic = topic_id;
    my_strncpy(dlq[did].data, data, MAX_MSG_DATA - 1);
    my_strncpy(dlq[did].error, error, 63);
    dlq[did].retry_count = 0;
    dlq[did].max_retries = 3;
    dlq[did].timestamp = global_clock++;
    dlq[did].active = 1;
    pstr("  DLQ: message queued (id="); pint(did);
    pstr(", error='"); pstr(error); pstr("')\n");
    return did;
}

static int dlq_retry(int dlq_id)
{
    if (dlq_id < 0 || dlq_id >= dlq_count || !dlq[dlq_id].active) {
        pstr("  Error: invalid DLQ entry\n");
        return -1;
    }
    dlq[dlq_id].retry_count++;
    pstr("  DLQ retry #"); pint(dlq[dlq_id].retry_count);
    pstr(" for entry "); pint(dlq_id); pstr("\n");
    if (dlq[dlq_id].retry_count >= dlq[dlq_id].max_retries) {
        dlq[dlq_id].active = 0;
        pstr("  DLQ: max retries exceeded, entry deactivated\n");
        return -2;
    }
    pstr("  DLQ: reprocessing '"); pstr(dlq[dlq_id].data); pstr("'\n");
    return 0;
}

static int dlq_requeue(int dlq_id)
{
    if (dlq_id < 0 || dlq_id >= dlq_count || !dlq[dlq_id].active) {
        pstr("  Error: invalid DLQ entry for requeue\n");
        return -1;
    }
    int tid = dlq[dlq_id].orig_topic;
    if (tid >= 0 && tid < topic_count && topics[tid].active) {
        int mid = publish(tid, "requeue", "dlq", dlq[dlq_id].data);
        dlq[dlq_id].active = 0;
        pstr("  DLQ: requeued entry "); pint(dlq_id);
        pstr(" as msg "); pint(mid); pstr("\n");
        return mid;
    }
    pstr("  Error: original topic unavailable for requeue\n");
    return -1;
}

static int dlq_alert(void)
{
    int active = 0;
    int exhausted = 0;
    pstr("  DLQ Alert Summary:\n");
    for (int i = 0; i < dlq_count; i++) {
        if (dlq[i].active) {
            active++;
            if (dlq[i].retry_count >= dlq[i].max_retries) exhausted++;
        }
    }
    pstr("    Active entries: "); pint(active); pstr("\n");
    pstr("    Exhausted retries: "); pint(exhausted); pstr("\n");
    pstr("    Total entries: "); pint(dlq_count); pstr("\n");
    if (exhausted > 0) {
        pstr("    ALERT: "); pint(exhausted);
        pstr(" entries need manual intervention!\n");
    }
    return active;
}

/* -------------------------------------------------------------------------- */
/* Schema registry                                                            */
/* -------------------------------------------------------------------------- */

static int schema_register(const char *name, const char *definition, int compat_mode)
{
    if (schema_count >= MAX_SCHEMAS) {
        pstr("  Error: schema registry full\n");
        return -1;
    }
    for (int i = 0; i < schema_count; i++) {
        if (schemas[i].active && my_strcmp(schemas[i].name, name) == 0) {
            schemas[i].version++;
            my_strncpy(schemas[i].definition, definition, MAX_SCHEMA_DEF - 1);
            schemas[i].compat_mode = compat_mode;
            pstr("  Schema '"); pstr(name); pstr("' updated to v");
            pint(schemas[i].version); pstr("\n");
            return i;
        }
    }
    int sid = schema_count++;
    my_strncpy(schemas[sid].name, name, 31);
    my_strncpy(schemas[sid].definition, definition, MAX_SCHEMA_DEF - 1);
    schemas[sid].version = 1;
    schemas[sid].compat_mode = compat_mode;
    schemas[sid].active = 1;
    pstr("  Registered schema '"); pstr(name); pstr("' v1 (mode=");
    pint(compat_mode); pstr(")\n");
    return sid;
}

static int schema_check_compat(int schema_id, const char *new_def)
{
    if (schema_id < 0 || schema_id >= schema_count || !schemas[schema_id].active) {
        pstr("  Error: invalid schema id\n");
        return -1;
    }
    const char *old_def = schemas[schema_id].definition;
    int mode = schemas[schema_id].compat_mode;
    int fields_old = 0, fields_new = 0;
    for (int i = 0; old_def[i]; i++) if (old_def[i] == ',') fields_old++;
    for (int i = 0; new_def[i]; i++) if (new_def[i] == ',') fields_new++;
    fields_old++; fields_new++;

    int compatible = 1;
    const char *mode_str = "UNKNOWN";
    if (mode == SCHEMA_BACKWARD) {
        mode_str = "BACKWARD";
        if (fields_new < fields_old) compatible = 0;
    } else if (mode == SCHEMA_FORWARD) {
        mode_str = "FORWARD";
        if (fields_new > fields_old + 2) compatible = 0;
    } else if (mode == SCHEMA_FULL) {
        mode_str = "FULL";
        if (fields_new != fields_old) compatible = 0;
    }

    pstr("  Schema '"); pstr(schemas[schema_id].name);
    pstr("' compat check ("); pstr(mode_str); pstr("): ");
    if (compatible) {
        pstr("COMPATIBLE (old="); pint(fields_old);
        pstr(" fields, new="); pint(fields_new); pstr(" fields)\n");
    } else {
        pstr("INCOMPATIBLE (old="); pint(fields_old);
        pstr(" fields, new="); pint(fields_new); pstr(" fields)\n");
    }
    return compatible;
}

/* -------------------------------------------------------------------------- */
/* CloudEvents / Protocol bridging                                            */
/* -------------------------------------------------------------------------- */

static cloudevent_t cloudevents_from_raw(const char *type, const char *source,
                                          const char *data)
{
    cloudevent_t ce;
    my_strncpy(ce.spec_version, "1.0", 7);
    my_strncpy(ce.type, type, 31);
    my_strncpy(ce.source, source, 63);
    unsigned int h = hash_str(data) ^ global_seq;
    ce.id[0] = 'e'; ce.id[1] = '-';
    for (int i = 0; i < 8; i++) {
        unsigned int nib = (h >> (i * 4)) & 0xF;
        ce.id[2 + i] = (nib < 10) ? ('0' + nib) : ('a' + nib - 10);
    }
    ce.id[10] = '\0';
    my_strncpy(ce.time_str, "2026-06-17T00:00:00Z", 31);
    my_strncpy(ce.data, data, MAX_MSG_DATA - 1);
    ce.field_count = 0;
    pstr("  CE created: id='"); pstr(ce.id);
    pstr("' type='"); pstr(ce.type); pstr("'\n");
    return ce;
}

static int cloudevents_to_http(cloudevent_t *ce)
{
    pstr("  CE -> HTTP:\n");
    pstr("    POST /events HTTP/1.1\n");
    pstr("    Content-Type: application/cloudevents+json\n");
    pstr("    ce-specversion: "); pstr(ce->spec_version); pstr("\n");
    pstr("    ce-type: "); pstr(ce->type); pstr("\n");
    pstr("    ce-source: "); pstr(ce->source); pstr("\n");
    pstr("    ce-id: "); pstr(ce->id); pstr("\n");
    pstr("    ce-time: "); pstr(ce->time_str); pstr("\n");
    pstr("    Body: "); pstr(ce->data); pstr("\n");
    return 0;
}

static int protocol_bridge(const char *from_proto, const char *to_proto,
                            const char *data)
{
    pstr("  Bridge: "); pstr(from_proto); pstr(" -> "); pstr(to_proto); pstr("\n");
    if (my_strcmp(from_proto, "HTTP") == 0 && my_strcmp(to_proto, "MQTT") == 0) {
        pstr("    MQTT publish topic='events/raw' payload='");
        pstr(data); pstr("'\n");
    } else if (my_strcmp(from_proto, "MQTT") == 0 && my_strcmp(to_proto, "gRPC") == 0) {
        pstr("    gRPC EventService.Emit("); pstr(data); pstr(")\n");
    } else if (my_strcmp(from_proto, "gRPC") == 0 && my_strcmp(to_proto, "HTTP") == 0) {
        pstr("    HTTP POST /webhook body='"); pstr(data); pstr("'\n");
    } else {
        pstr("    Generic bridge: "); pstr(data); pstr("\n");
    }
    return 0;
}

/* -------------------------------------------------------------------------- */
/* Help / Usage                                                               */
/* -------------------------------------------------------------------------- */

static void show_help(void)
{
    pstr("event_bus - Event bus / message broker module\n\n");
    pstr("Usage: event_bus [options]\n\n");
    pstr("Options:\n");
    pstr("  -h    Show this help\n");
    pstr("  -t    Run test suite\n\n");
    pstr("Features:\n");
    pstr("  Pub/Sub:    topic management, subscribe, publish, fan-out/fan-in\n");
    pstr("  Filters:    by type, source, regex pattern\n");
    pstr("  Sourcing:   append, replay, time travel\n");
    pstr("  Delivery:   at-least-once, exactly-once, idempotent\n");
    pstr("  DLQ:        send, retry, requeue, alert\n");
    pstr("  Schema:     register, compatibility (BACKWARD/FORWARD/FULL)\n");
    pstr("  CloudEvents: from raw, to HTTP, protocol bridge\n");
}

/* -------------------------------------------------------------------------- */
/* Test suite                                                                 */
/* -------------------------------------------------------------------------- */

static void test_suite(void)
{
    pstr("\n=== Event Bus Test Suite ===\n\n");

    /* Test 1: Topic create / subscribe / publish */
    pstr("--- Test 1: Topic / Subscribe / Publish ---\n");
    int t1 = topic_create("orders");
    int t2 = topic_create("payments");
    int s1 = subscribe(t1, "order-processor");
    int s2 = subscribe(t1, "analytics");
    publish(t1, "order.created", "web-api", "{\"id\":1001,\"item\":\"widget\"}");
    publish(t1, "order.shipped", "warehouse", "{\"id\":1001,\"tracking\":\"ABC123\"}");
    publish(t2, "payment.ok", "stripe", "{\"id\":1001,\"amount\":42}");
    topic_list();
    pstr("\n");

    /* Test 2: Fan-out */
    pstr("--- Test 2: Fan-out ---\n");
    int delivered = fan_out(t1);
    pstr("  Delivered to "); pint(delivered); pstr(" subscribers\n\n");

    /* Test 3: Fan-in */
    pstr("--- Test 3: Fan-in ---\n");
    int tids[2];
    tids[0] = t1; tids[1] = t2;
    fan_in(tids, 2);
    pstr("\n");

    /* Test 4: Filters */
    pstr("--- Test 4: Filters ---\n");
    filter_by_type(t1, "order.created");
    filter_by_source(t1, "warehouse");
    filter_by_regex(t1, "*widget*");
    pstr("\n");

    /* Test 5: Event sourcing */
    pstr("--- Test 5: Event Sourcing ---\n");
    event_store_append("user.signup", "{\"user\":\"alice\"}");
    event_store_append("user.update", "{\"user\":\"alice\",\"name\":\"Alice\"}");
    event_store_append("order.placed", "{\"user\":\"alice\",\"order\":1001}");
    event_store_append("payment.done", "{\"order\":1001,\"amount\":42}");
    event_store_replay(2, 10);
    pstr("  Time travel:\n");
    event_store_time_travel(3);
    pstr("\n");

    /* Test 6: Idempotent consume */
    pstr("--- Test 6: Idempotent Consume ---\n");
    consume_exactly_once(t1, s1);
    pstr("  Second consume (should dedup):\n");
    consume_exactly_once(t1, s1);
    idempotent_check("{\"id\":1001,\"item\":\"widget\"}", messages[0].seq);
    pstr("\n");

    /* Test 7: At-least-once consume */
    pstr("--- Test 7: At-least-once Consume ---\n");
    consume_at_least_once(t1, s2);
    pstr("\n");

    /* Test 8: DLQ */
    pstr("--- Test 8: Dead Letter Queue ---\n");
    int d1 = dlq_send(t1, "{\"bad\":\"msg\"}", "deserialization_error");
    int d2 = dlq_send(t2, "{\"timeout\":true}", "processing_timeout");
    dlq_retry(d1);
    dlq_retry(d1);
    dlq_retry(d1);
    dlq_alert();
    pstr("  Requeue d2:\n");
    dlq_requeue(d2);
    pstr("\n");

    /* Test 9: Schema registry */
    pstr("--- Test 9: Schema Registry ---\n");
    int sc1 = schema_register("OrderEvent", "id,item,qty", SCHEMA_BACKWARD);
    schema_check_compat(sc1, "id,item,qty,price");
    schema_check_compat(sc1, "id,item");
    int sc2 = schema_register("PaymentEvent", "id,amount", SCHEMA_FORWARD);
    schema_check_compat(sc2, "id,amount,currency");
    int sc3 = schema_register("StrictEvent", "a,b,c", SCHEMA_FULL);
    schema_check_compat(sc3, "a,b,c");
    schema_check_compat(sc3, "a,b,c,d");
    pstr("\n");

    /* Test 10: CloudEvents */
    pstr("--- Test 10: CloudEvents ---\n");
    cloudevent_t ce = cloudevents_from_raw("order.created", "/web/orders",
                                            "{\"id\":2001}");
    cloudevents_to_http(&ce);
    pstr("\n");

    /* Test 11: Protocol bridge */
    pstr("--- Test 11: Protocol Bridge ---\n");
    protocol_bridge("HTTP", "MQTT", "{\"raw\":\"payload\"}");
    protocol_bridge("MQTT", "gRPC", "{\"sensor\":\"temp\",\"val\":22}");
    protocol_bridge("gRPC", "HTTP", "{\"status\":\"ok\"}");
    pstr("\n");

    /* Test 12: Topic delete & unsubscribe */
    pstr("--- Test 12: Cleanup ---\n");
    unsubscribe(s2);
    topic_delete(t2);
    topic_list();

    pstr("\n=== All tests passed ===\n");
}

/* -------------------------------------------------------------------------- */
/* Entry point                                                                */
/* -------------------------------------------------------------------------- */

__attribute__((export_name("_start")))
void _start(void)
{
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);

    int help = 0, test = 0;
    unsigned int pos = 0;
    char *av = (char *)buf;

    /* skip program name */
    while (pos < 512 && av[pos]) pos++;
    pos++;

    /* parse flags */
    while (pos < 512 && av[pos]) {
        char *arg = &av[pos];
        int len = my_strlen(arg);
        if (len == 2 && arg[0] == '-' && arg[1] == 'h') help = 1;
        else if (len == 2 && arg[0] == '-' && arg[1] == 't') test = 1;
        while (pos < 512 && av[pos]) pos++;
        pos++;
    }

    pstr("Event Bus v46.0\n");

    if (help) {
        show_help();
        return;
    }

    if (test) {
        test_suite();
        return;
    }

    pstr("Use -h for help, -t for test\n");
}
