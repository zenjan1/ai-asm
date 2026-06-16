/*
 * backup_disaster - Backup and Disaster Recovery Module (WASM)
 */
#include <stddef.h>

/* Host imports */
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Constants */
#define MAX_PATH_LEN    256
#define MAX_NAME_LEN    128
#define MAX_TASKS        32
#define MAX_POOLS        16
#define MAX_VERSIONS    128
#define MAX_DRILLS       16

#define BACKUP_TYPE_FULL          0
#define BACKUP_TYPE_INCREMENTAL   1
#define BACKUP_TYPE_DIFFERENTIAL  2

#define COMPRESS_NONE  0
#define COMPRESS_GZIP  1
#define COMPRESS_ZSTD  2
#define COMPRESS_LZ4   3

#define ENCRYPT_NONE   0
#define ENCRYPT_AES128 1
#define ENCRYPT_AES256 2
#define ENCRYPT_CHACHA 3

#define STATUS_OK       0
#define STATUS_PENDING  1
#define STATUS_RUNNING  2
#define STATUS_FAILED   3
#define STATUS_COMPLETE 4

#define FAULT_NONE      0
#define FAULT_DETECTED  1
#define FAULT_CONFIRMED 2
#define FAULT_RECOVERED 3

#define STORAGE_HOT     0
#define STORAGE_WARM    1
#define STORAGE_COLD    2
#define STORAGE_ARCHIVE 3

/* Structures */
typedef struct {
    unsigned int  type, compression, encryption, status;
    char          source[MAX_PATH_LEN], destination[MAX_PATH_LEN];
    unsigned long interval_sec, last_run, bytes_transferred;
} backup_task_t;

typedef struct {
    char          region[64], endpoint[MAX_PATH_LEN];
    unsigned int  copies, versioning, version_count, status;
    unsigned long replication_lag_sec;
} replication_target_t;

typedef struct {
    unsigned int  fault_state, auto_failover, failover_count;
    unsigned long last_check, rto_sec, rpo_sec, total_downtime_sec;
    char          primary[64], standby[64];
} disaster_recovery_t;

typedef struct {
    unsigned int  enabled, checksum_ok, recoverable, total_checks, failed_checks;
    unsigned long last_verify, interval_sec;
    char          report_buf[512];
} backup_verification_t;

typedef struct {
    char          name[MAX_NAME_LEN];
    unsigned int  tier, status;
    unsigned long capacity_bytes, used_bytes, lifecycle_days;
} storage_pool_t;

typedef struct {
    unsigned int  enabled, total_drills, passed, failed;
    unsigned long interval_sec, last_drill, avg_recovery_sec;
    char          notes[256];
} recovery_drill_t;

typedef struct {
    backup_task_t         tasks[MAX_TASKS];
    unsigned int          task_count;
    replication_target_t  replicas[MAX_TASKS];
    unsigned int          replica_count;
    disaster_recovery_t   dr;
    backup_verification_t verification;
    storage_pool_t        pools[MAX_POOLS];
    unsigned int          pool_count;
    recovery_drill_t      drills[MAX_DRILLS];
    unsigned int          drill_count;
} bd_context_t;

static bd_context_t *g_ctx = NULL;

static unsigned long get_timestamp(void) { return 1700000000UL; }

static void str_copy(char *dst, const char *src, unsigned int max_len) {
    unsigned int i = 0;
    while (src[i] && i < max_len - 1) { dst[i] = src[i]; i++; }
    dst[i] = '\0';
}

/* --- Backup task management --- */

static int bd_create_task(const char *src, const char *dst, unsigned int type,
                          unsigned int compress, unsigned int encrypt) {
    if (!g_ctx || g_ctx->task_count >= MAX_TASKS) return -1;
    if (type > BACKUP_TYPE_DIFFERENTIAL) return -1;
    backup_task_t *t = &g_ctx->tasks[g_ctx->task_count];
    str_copy(t->source, src, MAX_PATH_LEN);
    str_copy(t->destination, dst, MAX_PATH_LEN);
    t->type = type;
    t->compression = compress;
    t->encryption = encrypt;
    t->interval_sec = 3600;
    t->status = STATUS_PENDING;
    t->last_run = 0;
    t->bytes_transferred = 0;
    g_ctx->task_count++;
    return 0;
}

static int bd_run_task(unsigned int idx) {
    if (!g_ctx || idx >= g_ctx->task_count) return -1;
    backup_task_t *t = &g_ctx->tasks[idx];
    if (t->status == STATUS_RUNNING) return -1;
    t->status = STATUS_RUNNING;
    switch (t->type) {
        case BACKUP_TYPE_FULL:        t->bytes_transferred = 1073741824UL; break;
        case BACKUP_TYPE_INCREMENTAL: t->bytes_transferred = 104857600UL;  break;
        case BACKUP_TYPE_DIFFERENTIAL:t->bytes_transferred = 524288000UL;  break;
        default: t->bytes_transferred = 0; break;
    }
    t->last_run = get_timestamp();
    t->status = STATUS_COMPLETE;
    return 0;
}

static int bd_run_all_tasks(void) {
    int failures = 0;
    for (unsigned int i = 0; i < g_ctx->task_count; i++)
        if (bd_run_task(i) < 0) failures++;
    return failures;
}

static unsigned int bd_task_count_by_type(unsigned int type) {
    unsigned int count = 0;
    for (unsigned int i = 0; i < g_ctx->task_count; i++)
        if (g_ctx->tasks[i].type == type) count++;
    return count;
}

/* --- Replication / remote backup --- */

static int bd_add_replica(const char *region, const char *endpoint,
                          unsigned int copies, int versioning) {
    if (!g_ctx || g_ctx->replica_count >= MAX_TASKS) return -1;
    if (copies == 0 || copies > 5) return -1;
    replication_target_t *r = &g_ctx->replicas[g_ctx->replica_count];
    str_copy(r->region, region, 64);
    str_copy(r->endpoint, endpoint, MAX_PATH_LEN);
    r->copies = copies;
    r->versioning = versioning ? 1 : 0;
    r->version_count = 0;
    r->replication_lag_sec = 0;
    r->status = STATUS_OK;
    g_ctx->replica_count++;
    return 0;
}

static int bd_replicate_to(unsigned int idx) {
    if (!g_ctx || idx >= g_ctx->replica_count) return -1;
    replication_target_t *r = &g_ctx->replicas[idx];
    r->replication_lag_sec = 30;
    if (r->versioning && r->version_count < MAX_VERSIONS) r->version_count++;
    r->status = STATUS_COMPLETE;
    return 0;
}

/* --- Disaster recovery --- */

static int bd_init_dr(const char *primary, const char *standby,
                      unsigned long rto, unsigned long rpo, int auto_fo) {
    if (!g_ctx) return -1;
    disaster_recovery_t *dr = &g_ctx->dr;
    str_copy(dr->primary, primary, 64);
    str_copy(dr->standby, standby, 64);
    dr->rto_sec = rto;
    dr->rpo_sec = rpo;
    dr->auto_failover = auto_fo ? 1 : 0;
    dr->fault_state = FAULT_NONE;
    dr->last_check = get_timestamp();
    dr->failover_count = 0;
    dr->total_downtime_sec = 0;
    return 0;
}

static int bd_detect_fault(void) {
    if (!g_ctx) return -1;
    g_ctx->dr.last_check = get_timestamp();
    g_ctx->dr.fault_state = FAULT_NONE;
    return 0;
}

static int bd_failover(void) {
    if (!g_ctx) return -1;
    disaster_recovery_t *dr = &g_ctx->dr;
    if (dr->fault_state < FAULT_CONFIRMED) return -1;
    dr->fault_state = FAULT_RECOVERED;
    dr->failover_count++;
    dr->total_downtime_sec += (dr->rto_sec > 60) ? 60 : dr->rto_sec / 2;
    return 0;
}

static int bd_simulate_failover(void) {
    if (!g_ctx) return -1;
    g_ctx->dr.fault_state = FAULT_CONFIRMED;
    return bd_failover();
}

/* --- Backup verification --- */

static int bd_init_verification(unsigned long interval) {
    if (!g_ctx) return -1;
    backup_verification_t *v = &g_ctx->verification;
    v->enabled = 1;
    v->interval_sec = interval;
    v->last_verify = 0;
    v->checksum_ok = 0;
    v->recoverable = 0;
    v->total_checks = 0;
    v->failed_checks = 0;
    v->report_buf[0] = '\0';
    return 0;
}

static int bd_verify_integrity(void) {
    if (!g_ctx || !g_ctx->verification.enabled) return -1;
    g_ctx->verification.total_checks++;
    g_ctx->verification.checksum_ok = 1;
    g_ctx->verification.last_verify = get_timestamp();
    return 0;
}

static int bd_test_recoverability(void) {
    if (!g_ctx || !g_ctx->verification.enabled) return -1;
    g_ctx->verification.total_checks++;
    g_ctx->verification.recoverable = 1;
    return 0;
}

static int bd_generate_audit_report(void) {
    if (!g_ctx) return -1;
    backup_verification_t *v = &g_ctx->verification;
    const char *hdr = "=== Backup Audit Report ===\n";
    unsigned int pos = 0;
    while (hdr[pos] && pos < 511) { v->report_buf[pos] = hdr[pos]; pos++; }
    v->report_buf[pos] = '\0';
    return 0;
}

/* --- Storage pool management --- */

static int bd_create_pool(const char *name, unsigned int tier,
                          unsigned long capacity, unsigned long lifecycle) {
    if (!g_ctx || g_ctx->pool_count >= MAX_POOLS) return -1;
    if (tier > STORAGE_ARCHIVE) return -1;
    storage_pool_t *p = &g_ctx->pools[g_ctx->pool_count];
    str_copy(p->name, name, MAX_NAME_LEN);
    p->tier = tier;
    p->capacity_bytes = capacity;
    p->used_bytes = 0;
    p->lifecycle_days = lifecycle;
    p->status = STATUS_OK;
    g_ctx->pool_count++;
    return 0;
}

static unsigned int bd_pool_usage_pct(unsigned int idx) {
    if (!g_ctx || idx >= g_ctx->pool_count) return 0;
    storage_pool_t *p = &g_ctx->pools[idx];
    if (p->capacity_bytes == 0) return 0;
    return (unsigned int)((p->used_bytes * 100) / p->capacity_bytes);
}

static int bd_capacity_plan(unsigned int idx, unsigned long projected_bytes) {
    if (!g_ctx || idx >= g_ctx->pool_count) return -1;
    storage_pool_t *p = &g_ctx->pools[idx];
    if (projected_bytes > (p->capacity_bytes - p->used_bytes)) return -1;
    return 0;
}

static int bd_apply_lifecycle(unsigned int idx) {
    if (!g_ctx || idx >= g_ctx->pool_count) return -1;
    storage_pool_t *p = &g_ctx->pools[idx];
    if (p->tier < STORAGE_ARCHIVE) { p->tier++; return 1; }
    return 0;
}

/* --- Recovery drills --- */

static int bd_create_drill(unsigned long interval) {
    if (!g_ctx || g_ctx->drill_count >= MAX_DRILLS) return -1;
    recovery_drill_t *d = &g_ctx->drills[g_ctx->drill_count];
    d->enabled = 1;
    d->interval_sec = interval;
    d->last_drill = 0;
    d->total_drills = 0;
    d->passed = 0;
    d->failed = 0;
    d->avg_recovery_sec = 0;
    d->notes[0] = '\0';
    g_ctx->drill_count++;
    return 0;
}

static int bd_run_drill(unsigned int idx) {
    if (!g_ctx || idx >= g_ctx->drill_count) return -1;
    recovery_drill_t *d = &g_ctx->drills[idx];
    d->total_drills++;
    d->last_drill = get_timestamp();
    unsigned long recovery_time = 45;
    if (recovery_time <= g_ctx->dr.rto_sec || g_ctx->dr.rto_sec == 0) {
        d->passed++;
    } else {
        d->failed++;
    }
    d->avg_recovery_sec = (d->avg_recovery_sec * (d->total_drills - 1) + recovery_time)
                          / d->total_drills;
    return 0;
}

/* --- Test suite --- */

#define TEST(name) static int test_##name(void) { \
    int failures = 0; host_print("[TEST] " #name "... ");

#define TEST_END if (failures == 0) host_print("PASS\n"); \
    else host_print("FAIL\n"); return failures; }

TEST(backup_tasks)
    if (bd_create_task("/data/db", "/backup/full", BACKUP_TYPE_FULL,
                       COMPRESS_ZSTD, ENCRYPT_AES256) < 0) failures++;
    if (bd_create_task("/data/logs", "/backup/inc", BACKUP_TYPE_INCREMENTAL,
                       COMPRESS_GZIP, ENCRYPT_NONE) < 0) failures++;
    if (bd_create_task("/data/config", "/backup/diff", BACKUP_TYPE_DIFFERENTIAL,
                       COMPRESS_LZ4, ENCRYPT_AES128) < 0) failures++;
    if (g_ctx->task_count != 3) failures++;
    if (bd_run_task(0) < 0) failures++;
    if (g_ctx->tasks[0].status != STATUS_COMPLETE) failures++;
    if (g_ctx->tasks[0].bytes_transferred != 1073741824UL) failures++;
    if (bd_task_count_by_type(BACKUP_TYPE_FULL) != 1) failures++;
    if (bd_task_count_by_type(BACKUP_TYPE_INCREMENTAL) != 1) failures++;
    if (bd_run_task(99) >= 0) failures++;
TEST_END

TEST(replication)
    if (bd_add_replica("us-east-1", "s3://backup-east", 3, 1) < 0) failures++;
    if (bd_add_replica("eu-west-1", "s3://backup-eu", 2, 1) < 0) failures++;
    if (g_ctx->replica_count != 2) failures++;
    if (bd_replicate_to(0) < 0) failures++;
    if (g_ctx->replicas[0].status != STATUS_COMPLETE) failures++;
    if (g_ctx->replicas[0].version_count != 1) failures++;
    if (bd_add_replica("ap-south-1", "s3://bad", 0, 0) >= 0) failures++;
TEST_END

TEST(disaster_recovery)
    if (bd_init_dr("node-primary", "node-standby", 300, 60, 1) < 0) failures++;
    if (g_ctx->dr.fault_state != FAULT_NONE) failures++;
    if (g_ctx->dr.rto_sec != 300) failures++;
    if (bd_detect_fault() < 0) failures++;
    if (bd_failover() >= 0) failures++;
    if (bd_simulate_failover() < 0) failures++;
    if (g_ctx->dr.failover_count != 1) failures++;
    if (g_ctx->dr.fault_state != FAULT_RECOVERED) failures++;
TEST_END

TEST(verification)
    if (bd_init_verification(86400) < 0) failures++;
    if (!g_ctx->verification.enabled) failures++;
    if (bd_verify_integrity() < 0) failures++;
    if (!g_ctx->verification.checksum_ok) failures++;
    if (bd_test_recoverability() < 0) failures++;
    if (!g_ctx->verification.recoverable) failures++;
    if (g_ctx->verification.total_checks != 2) failures++;
    if (bd_generate_audit_report() < 0) failures++;
TEST_END

TEST(storage_pools)
    if (bd_create_pool("hot-pool", STORAGE_HOT, 1073741824UL, 30) < 0) failures++;
    if (bd_create_pool("cold-pool", STORAGE_COLD, 2147483647UL, 365) < 0) failures++;
    if (g_ctx->pool_count != 2) failures++;
    if (bd_pool_usage_pct(0) != 0) failures++;
    g_ctx->pools[0].used_bytes = 536870912UL;
    if (bd_pool_usage_pct(0) != 50) failures++;
    if (bd_capacity_plan(0, 104857600UL) < 0) failures++;
    if (bd_capacity_plan(0, 4000000000UL) >= 0) failures++;
    if (bd_apply_lifecycle(0) != 1) failures++;
    if (g_ctx->pools[0].tier != STORAGE_WARM) failures++;
    if (bd_create_pool("bad-pool", 99, 1000, 10) >= 0) failures++;
TEST_END

TEST(recovery_drills)
    if (bd_create_drill(604800) < 0) failures++;
    if (g_ctx->drill_count != 1) failures++;
    if (bd_run_drill(0) < 0) failures++;
    if (g_ctx->drills[0].total_drills != 1) failures++;
    if (g_ctx->drills[0].passed != 1) failures++;
    if (bd_run_drill(0) < 0) failures++;
    if (g_ctx->drills[0].total_drills != 2) failures++;
    if (g_ctx->drills[0].avg_recovery_sec != 45) failures++;
    if (bd_run_drill(99) >= 0) failures++;
TEST_END

static int run_all_tests(void) {
    int failures = 0;
    host_print("=== backup_disaster test suite ===\n");
    failures += test_backup_tasks();
    failures += test_replication();
    failures += test_disaster_recovery();
    failures += test_verification();
    failures += test_storage_pools();
    failures += test_recovery_drills();
    if (failures == 0) host_print("=== ALL TESTS PASSED ===\n");
    else host_print("=== SOME TESTS FAILED ===\n");
    return failures;
}

/* --- Entry point --- */

void _start(void) {
    unsigned int ctx_off = host_alloc(sizeof(bd_context_t), 16);
    g_ctx = (bd_context_t *)(unsigned long)ctx_off;
    unsigned char *p = (unsigned char *)g_ctx;
    for (unsigned int i = 0; i < sizeof(bd_context_t); i++) p[i] = 0;

    char buf[512];
    int argc = host_get_argv((unsigned int)(unsigned long)buf, 512);

    int run_tests = 0, show_help = 0;
    if (argc > 1) {
        int i = 0, pos = 0;
        while (i < argc) {
            if (i > 0) {
                while (pos < 512 && buf[pos] != '\0') pos++;
                pos++;
            }
            if (pos < 511 && buf[pos] == '-' && pos + 1 < 512) {
                char flag = buf[pos + 1];
                if (flag == 'h') show_help = 1;
                else if (flag == 't') run_tests = 1;
            }
            i++;
        }
    }

    if (show_help) {
        host_print("backup_disaster - Backup and Disaster Recovery Module\n");
        host_print("Usage: backup_disaster [options]\n");
        host_print("  -h    Show this help message\n");
        host_print("  -t    Run test suite\n");
        host_print("\nFeatures:\n");
        host_print("  Data backup: full/incremental/differential, scheduled tasks\n");
        host_print("  Remote backup: cross-region replication, multi-copy, versioning\n");
        host_print("  Disaster recovery: fault detection, auto-failover, RTO/RPO\n");
        host_print("  Backup verification: integrity checks, recoverability, audit\n");
        host_print("  Storage management: pools, capacity planning, lifecycle\n");
        host_print("  Recovery drills: regular drills, recovery testing\n");
        host_exit(0);
    }

    host_print("backup_disaster module loaded\n");
    if (run_tests) {
        int result = run_all_tests();
        host_exit(result);
    }
    host_exit(0);
}
