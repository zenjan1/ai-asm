/* zero_trust_network: Continuous verification & zero-trust security (v1.0) */

#include <stddef.h>

/* Host function imports */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* ========================================================================
 * Constants & Limits
 * ======================================================================== */

#define MAX_IDENTITIES    32
#define MAX_SEGMENTS      24
#define MAX_PERMISSIONS   64
#define MAX_DEVICES       32
#define MAX_BEHAVIORS     48
#define MAX_AUDIT_LOGS    128
#define MAX_POLICIES      16
#define NAME_LEN          32
#define PATTERN_LEN       48
#define POLICY_LEN        64
#define HASH_LEN          16
#define MAX_WORKLOADS     32

/* Identity types */
#define IDENTITY_DEVICE   0
#define IDENTITY_USER     1
#define IDENTITY_APP      2
#define IDENTITY_SERVICE  3

/* Auth methods */
#define AUTH_MFA          0
#define AUTH_CERT         1
#define AUTH_TOKEN        2
#define AUTH_BIOMETRIC    3

/* Segment policy actions */
#define SEG_ACTION_ALLOW   0
#define SEG_ACTION_DENY    1
#define SEG_ACTION_LOG     2
#define SEG_ACTION_QUARANTINE 3

/* Permission levels */
#define PERM_NONE    0
#define PERM_READ    1
#define PERM_WRITE   2
#define PERM_EXEC    3
#define PERM_ADMIN   4

/* Device compliance states */
#define COMPLIANT       0
#define NON_COMPLIANT   1
#define DEGRADED        2
#define UNKNOWN         3

/* Risk levels */
#define RISK_LOW      0
#define RISK_MEDIUM   1
#define RISK_HIGH     2
#define RISK_CRITICAL 3

/* Audit action types */
#define AUDIT_ACCESS      0
#define AUDIT_GRANT       1
#define AUDIT_REVOKE      2
#define AUDIT_ANOMALY     3
#define AUDIT_POLICY      4
#define AUDIT_HEALTH      5
#define AUDIT_SEGMENT     6
#define AUDIT_FORENSIC    7

/* ========================================================================
 * Data Structures
 * ======================================================================== */

/* Identity record for continuous verification */
typedef struct {
    int   id;
    int   type;                /* device/user/app/service */
    char  name[NAME_LEN];
    int   auth_method;
    int   auth_valid;          /* 1=valid, 0=expired */
    int   last_verify;         /* timestamp of last verification */
    int   trust_score;         /* 0-100 continuous trust */
    int   session_active;
} identity_t;

/* Micro-segment definition */
typedef struct {
    int   seg_id;
    char  name[NAME_LEN];
    int   policy_action;
    char  policy[POLICY_LEN];
    int   workload_count;
    int   isolated;            /* 1=isolated, 0=connected */
    int   enforcement_active;
} segment_t;

/* Least-privilege permission record */
typedef struct {
    int   perm_id;
    int   identity_id;
    char  resource[NAME_LEN];
    int   level;               /* read/write/exec/admin */
    int   dynamic;             /* 1=dynamic grant, 0=static */
    int   ttl;                 /* time-to-live in ticks */
    int   active;
    int   grant_time;
} permission_t;

/* Device trust record */
typedef struct {
    int   device_id;
    char  name[NAME_LEN];
    int   health_status;       /* 0=healthy, 1=degraded, 2=critical */
    int   compliance;
    int   trust_score;         /* 0-100 */
    int   os_patched;
    int   firewall_active;
    int   encryption_active;
    int   last_check;
} device_trust_t;

/* Behavior analysis record */
typedef struct {
    int   record_id;
    int   identity_id;
    char  pattern[PATTERN_LEN];
    int   anomaly_detected;
    int   risk_level;
    int   baseline_deviation;   /* percentage deviation from baseline */
    int   response_action;     /* 0=none, 1=alert, 2=block, 3=quarantine */
    int   timestamp;
} behavior_record_t;

/* Audit log entry */
typedef struct {
    int   log_id;
    int   action_type;
    int   subject_id;
    char  description[POLICY_LEN];
    int   severity;            /* risk level */
    int   result;              /* 0=success, -1=denied */
    int   timestamp;
} audit_log_entry_t;

/* Workload isolation record */
typedef struct {
    int   workload_id;
    char  name[NAME_LEN];
    int   segment_id;
    int   isolated;
    int   policy_enforced;
    char  fingerprint[HASH_LEN];
} workload_t;

/* ========================================================================
 * Global State
 * ======================================================================== */

static identity_t identities[MAX_IDENTITIES];
static int identity_count = 0;
static int next_identity_id = 1;

static segment_t segments[MAX_SEGMENTS];
static int segment_count = 0;
static int next_segment_id = 1;

static permission_t permissions[MAX_PERMISSIONS];
static int permission_count = 0;
static int next_permission_id = 1;

static device_trust_t devices[MAX_DEVICES];
static int device_count = 0;
static int next_device_id = 1;

static behavior_record_t behaviors[MAX_BEHAVIORS];
static int behavior_count = 0;
static int next_behavior_id = 1;

static audit_log_entry_t audit_logs[MAX_AUDIT_LOGS];
static int audit_count = 0;
static int next_audit_id = 1;

static workload_t workloads[MAX_WORKLOADS];
static int workload_count = 0;
static int next_workload_id = 1;

static int tick_counter = 0;

/* ========================================================================
 * String & Print Utilities
 * ======================================================================== */

static int my_strlen(const char *s) {
    int l = 0; while (s[l]) l++; return l;
}

static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; }
    return *a - *b;
}

static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0;
    while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; }
    dst[i] = '\0';
}

static void print_str(const char *str) { host_print(str); }

static void print_int(int val) {
    char buf[32]; int pos = 0;
    if (val < 0) { buf[pos++] = '-'; val = -val; }
    if (val == 0) buf[pos++] = '0';
    else {
        int d = 0, t = val;
        while (t > 0) { d++; t /= 10; }
        pos += d; buf[pos] = '\0'; pos--;
        while (val > 0) { buf[pos--] = '0' + (val % 10); val /= 10; }
    }
    host_print(buf);
}

static const char *identity_type_name(int t) {
    if (t == IDENTITY_DEVICE) return "DEVICE";
    if (t == IDENTITY_USER) return "USER";
    if (t == IDENTITY_APP) return "APP";
    return "SERVICE";
}

static const char *auth_method_name(int m) {
    if (m == AUTH_MFA) return "MFA";
    if (m == AUTH_CERT) return "CERT";
    if (m == AUTH_TOKEN) return "TOKEN";
    return "BIOMETRIC";
}

static const char *perm_level_name(int l) {
    if (l == PERM_NONE) return "NONE";
    if (l == PERM_READ) return "READ";
    if (l == PERM_WRITE) return "WRITE";
    if (l == PERM_EXEC) return "EXEC";
    return "ADMIN";
}

static const char *risk_level_name(int r) {
    if (r == RISK_LOW) return "LOW";
    if (r == RISK_MEDIUM) return "MEDIUM";
    if (r == RISK_HIGH) return "HIGH";
    return "CRITICAL";
}

static const char *audit_action_name(int a) {
    if (a == AUDIT_ACCESS) return "ACCESS";
    if (a == AUDIT_GRANT) return "GRANT";
    if (a == AUDIT_REVOKE) return "REVOKE";
    if (a == AUDIT_ANOMALY) return "ANOMALY";
    if (a == AUDIT_POLICY) return "POLICY";
    if (a == AUDIT_HEALTH) return "HEALTH";
    if (a == AUDIT_SEGMENT) return "SEGMENT";
    return "FORENSIC";
}

/* ========================================================================
 * Audit Logging (used by all features)
 * ======================================================================== */

static void audit_log(int action, int subject_id, const char *desc, int severity, int result) {
    if (audit_count >= MAX_AUDIT_LOGS) return;
    int idx = audit_count;
    audit_logs[idx].log_id = next_audit_id++;
    audit_logs[idx].action_type = action;
    audit_logs[idx].subject_id = subject_id;
    my_strncpy(audit_logs[idx].description, desc, POLICY_LEN - 1);
    audit_logs[idx].severity = severity;
    audit_logs[idx].result = result;
    audit_logs[idx].timestamp = tick_counter++;
}

/* ========================================================================
 * Feature 1: Continuous Verification (device/user/app identity auth)
 * ======================================================================== */

int zt_register_identity(const char *name, int type, int auth_method) {
    if (identity_count >= MAX_IDENTITIES) return -1;
    int idx = identity_count;
    identities[idx].id = next_identity_id++;
    identities[idx].type = type;
    my_strncpy(identities[idx].name, name, NAME_LEN - 1);
    identities[idx].auth_method = auth_method;
    identities[idx].auth_valid = 1;
    identities[idx].last_verify = tick_counter;
    identities[idx].trust_score = 80;
    identities[idx].session_active = 1;
    identity_count++;
    audit_log(AUDIT_ACCESS, identities[idx].id, "identity registered", RISK_LOW, 0);
    return identities[idx].id;
}

int zt_verify_identity(int id) {
    for (int i = 0; i < identity_count; i++) {
        if (identities[i].id != id) continue;
        identities[i].last_verify = tick_counter;
        /* Simulate verification: auth tokens expire after 10 ticks */
        if (identities[i].auth_method == AUTH_TOKEN &&
            (tick_counter - identities[i].last_verify) > 10) {
            identities[i].auth_valid = 0;
            audit_log(AUDIT_ACCESS, id, "token expired - re-auth required", RISK_MEDIUM, -1);
            return -1;
        }
        /* MFA always passes verification */
        identities[i].auth_valid = 1;
        audit_log(AUDIT_ACCESS, id, "identity verified", RISK_LOW, 0);
        return identities[i].trust_score;
    }
    return -1;
}

int zt_continuous_verify_all(void) {
    int verified = 0;
    print_str("  --- Continuous Verification ---\n");
    for (int i = 0; i < identity_count; i++) {
        int score = zt_verify_identity(identities[i].id);
        print_str("  ");
        print_str(identities[i].name);
        print_str(" (");
        print_str(identity_type_name(identities[i].type));
        print_str("/");
        print_str(auth_method_name(identities[i].auth_method));
        print_str("): trust=");
        print_int(score);
        print_str("  ");
        if (score >= 0) { print_str("VERIFIED\n"); verified++; }
        else { print_str("DENIED\n"); }
    }
    print_str("  Verified: ");
    print_int(verified);
    print_str("/");
    print_int(identity_count);
    print_str("\n");
    return verified;
}

/* ========================================================================
 * Feature 2: Micro-Segmentation (network segmentation, isolation, policy)
 * ======================================================================== */

int zt_create_segment(const char *name, int policy_action, const char *policy) {
    if (segment_count >= MAX_SEGMENTS) return -1;
    int idx = segment_count;
    segments[idx].seg_id = next_segment_id++;
    my_strncpy(segments[idx].name, name, NAME_LEN - 1);
    segments[idx].policy_action = policy_action;
    my_strncpy(segments[idx].policy, policy, POLICY_LEN - 1);
    segments[idx].workload_count = 0;
    segments[idx].isolated = 0;
    segments[idx].enforcement_active = 1;
    segment_count++;
    audit_log(AUDIT_SEGMENT, segments[idx].seg_id, "segment created", RISK_LOW, 0);
    return segments[idx].seg_id;
}

int zt_isolate_workload(int workload_id, int segment_id) {
    for (int i = 0; i < workload_count; i++) {
        if (workloads[i].workload_id != workload_id) continue;
        workloads[i].segment_id = segment_id;
        workloads[i].isolated = 1;
        workloads[i].policy_enforced = 1;
        /* Update segment workload count */
        for (int j = 0; j < segment_count; j++) {
            if (segments[j].seg_id == segment_id) segments[j].workload_count++;
        }
        audit_log(AUDIT_SEGMENT, workload_id, "workload isolated into segment", RISK_MEDIUM, 0);
        return 0;
    }
    return -1;
}

int zt_enforce_policy(int segment_id) {
    for (int i = 0; i < segment_count; i++) {
        if (segments[i].seg_id != segment_id) continue;
        segments[i].enforcement_active = 1;
        print_str("  Enforcing policy on segment '");
        print_str(segments[i].name);
        print_str("': ");
        print_str(segments[i].policy);
        print_str("\n");
        if (segments[i].policy_action == SEG_ACTION_ALLOW)
            print_str("  Action: ALLOW all traffic within segment\n");
        else if (segments[i].policy_action == SEG_ACTION_DENY)
            print_str("  Action: DENY cross-segment traffic\n");
        else if (segments[i].policy_action == SEG_ACTION_LOG)
            print_str("  Action: LOG all segment activity\n");
        else
            print_str("  Action: QUARANTINE non-compliant traffic\n");
        audit_log(AUDIT_POLICY, segment_id, "policy enforced", RISK_LOW, 0);
        return 0;
    }
    return -1;
}

int zt_register_workload(const char *name) {
    if (workload_count >= MAX_WORKLOADS) return -1;
    int idx = workload_count;
    workloads[idx].workload_id = next_workload_id++;
    my_strncpy(workloads[idx].name, name, NAME_LEN - 1);
    workloads[idx].segment_id = -1;
    workloads[idx].isolated = 0;
    workloads[idx].policy_enforced = 0;
    my_strncpy(workloads[idx].fingerprint, "fp-00000000", HASH_LEN - 1);
    workload_count++;
    return workloads[idx].workload_id;
}

/* ========================================================================
 * Feature 3: Least Privilege (dynamic grant, on-demand, revocation)
 * ======================================================================== */

int zt_grant_permission(int identity_id, const char *resource, int level, int ttl) {
    if (permission_count >= MAX_PERMISSIONS) return -1;
    int idx = permission_count;
    permissions[idx].perm_id = next_permission_id++;
    permissions[idx].identity_id = identity_id;
    my_strncpy(permissions[idx].resource, resource, NAME_LEN - 1);
    permissions[idx].level = level;
    permissions[idx].dynamic = (ttl > 0) ? 1 : 0;
    permissions[idx].ttl = ttl;
    permissions[idx].active = 1;
    permissions[idx].grant_time = tick_counter;
    permission_count++;
    audit_log(AUDIT_GRANT, identity_id, "permission granted", RISK_LOW, 0);
    return permissions[idx].perm_id;
}

int zt_request_on_demand(int identity_id, const char *resource, int level) {
    /* On-demand access: grant with short TTL */
    print_str("  On-demand request: identity #");
    print_int(identity_id);
    print_str(" -> ");
    print_str(resource);
    print_str(" (");
    print_str(perm_level_name(level));
    print_str(")\n");
    /* Check identity trust score before granting */
    for (int i = 0; i < identity_count; i++) {
        if (identities[i].id != identity_id) continue;
        if (identities[i].trust_score < 50) {
            print_str("  DENIED: trust score too low (");
            print_int(identities[i].trust_score);
            print_str(")\n");
            audit_log(AUDIT_GRANT, identity_id, "on-demand denied: low trust", RISK_HIGH, -1);
            return -1;
        }
    }
    int perm_id = zt_grant_permission(identity_id, resource, level, 5);
    print_str("  GRANTED: on-demand access for 5 ticks (perm #");
    print_int(perm_id);
    print_str(")\n");
    return perm_id;
}

int zt_revoke_permission(int perm_id) {
    for (int i = 0; i < permission_count; i++) {
        if (permissions[i].perm_id != perm_id) continue;
        permissions[i].active = 0;
        print_str("  Permission #");
        print_int(perm_id);
        print_str(" revoked (was ");
        print_str(perm_level_name(permissions[i].level));
        print_str(" on ");
        print_str(permissions[i].resource);
        print_str(")\n");
        audit_log(AUDIT_REVOKE, permissions[i].identity_id, "permission revoked", RISK_LOW, 0);
        return 0;
    }
    return -1;
}

int zt_expire_permissions(void) {
    int expired = 0;
    for (int i = 0; i < permission_count; i++) {
        if (!permissions[i].active) continue;
        if (!permissions[i].dynamic) continue;
        if ((tick_counter - permissions[i].grant_time) >= permissions[i].ttl) {
            permissions[i].active = 0;
            audit_log(AUDIT_REVOKE, permissions[i].identity_id, "permission auto-expired", RISK_LOW, 0);
            expired++;
        }
    }
    return expired;
}

/* ========================================================================
 * Feature 4: Device Trust (health checks, compliance, trust scoring)
 * ======================================================================== */

int zt_register_device(const char *name, int os_patched, int firewall, int encryption) {
    if (device_count >= MAX_DEVICES) return -1;
    int idx = device_count;
    devices[idx].device_id = next_device_id++;
    my_strncpy(devices[idx].name, name, NAME_LEN - 1);
    devices[idx].os_patched = os_patched;
    devices[idx].firewall_active = firewall;
    devices[idx].encryption_active = encryption;
    devices[idx].last_check = tick_counter;
    device_count++;
    return devices[idx].device_id;
}

int zt_health_check(int device_id) {
    for (int i = 0; i < device_count; i++) {
        if (devices[i].device_id != device_id) continue;
        devices[i].last_check = tick_counter;
        int score = 100;
        if (!devices[i].os_patched) score -= 30;
        if (!devices[i].firewall_active) score -= 25;
        if (!devices[i].encryption_active) score -= 25;
        devices[i].trust_score = (score < 0) ? 0 : score;
        if (score >= 80) devices[i].health_status = 0;       /* healthy */
        else if (score >= 50) devices[i].health_status = 1;  /* degraded */
        else devices[i].health_status = 2;                    /* critical */
        audit_log(AUDIT_HEALTH, device_id, "health check performed", RISK_LOW, 0);
        return devices[i].trust_score;
    }
    return -1;
}

int zt_compliance_assess(int device_id) {
    for (int i = 0; i < device_count; i++) {
        if (devices[i].device_id != device_id) continue;
        int compliant = 1;
        print_str("  Compliance assessment for '");
        print_str(devices[i].name);
        print_str("':\n");
        print_str("    OS patched:     ");
        if (devices[i].os_patched) print_str("PASS\n");
        else { print_str("FAIL\n"); compliant = 0; }
        print_str("    Firewall:       ");
        if (devices[i].firewall_active) print_str("PASS\n");
        else { print_str("FAIL\n"); compliant = 0; }
        print_str("    Encryption:     ");
        if (devices[i].encryption_active) print_str("PASS\n");
        else { print_str("FAIL\n"); compliant = 0; }
        devices[i].compliance = compliant ? COMPLIANT : NON_COMPLIANT;
        print_str("    Status: ");
        if (compliant) print_str("COMPLIANT\n");
        else print_str("NON-COMPLIANT\n");
        audit_log(AUDIT_HEALTH, device_id, "compliance assessed", RISK_LOW,
                  compliant ? 0 : -1);
        return compliant;
    }
    return -1;
}

int zt_compute_trust_scores(void) {
    print_str("  --- Device Trust Scores ---\n");
    for (int i = 0; i < device_count; i++) {
        int score = zt_health_check(devices[i].device_id);
        print_str("  ");
        print_str(devices[i].name);
        print_str(": trust=");
        print_int(score);
        print_str("  health=");
        if (devices[i].health_status == 0) print_str("HEALTHY");
        else if (devices[i].health_status == 1) print_str("DEGRADED");
        else print_str("CRITICAL");
        print_str("\n");
        /* Update associated identity trust score */
        for (int j = 0; j < identity_count; j++) {
            if (identities[j].type == IDENTITY_DEVICE &&
                identities[j].id == devices[i].device_id) {
                identities[j].trust_score = score;
            }
        }
    }
    return device_count;
}

/* ========================================================================
 * Feature 5: Behavior Analysis (anomaly detection, risk, adaptive response)
 * ======================================================================== */

int zt_record_behavior(int identity_id, const char *pattern, int deviation) {
    if (behavior_count >= MAX_BEHAVIORS) return -1;
    int idx = behavior_count;
    behaviors[idx].record_id = next_behavior_id++;
    behaviors[idx].identity_id = identity_id;
    my_strncpy(behaviors[idx].pattern, pattern, PATTERN_LEN - 1);
    behaviors[idx].baseline_deviation = deviation;
    behaviors[idx].timestamp = tick_counter;
    /* Anomaly detection: flag if deviation > 30% */
    if (deviation > 30) {
        behaviors[idx].anomaly_detected = 1;
        if (deviation > 70) behaviors[idx].risk_level = RISK_CRITICAL;
        else if (deviation > 50) behaviors[idx].risk_level = RISK_HIGH;
        else behaviors[idx].risk_level = RISK_MEDIUM;
        audit_log(AUDIT_ANOMALY, identity_id, "behavior anomaly detected",
                  behaviors[idx].risk_level, -1);
    } else {
        behaviors[idx].anomaly_detected = 0;
        behaviors[idx].risk_level = RISK_LOW;
    }
    behaviors[idx].response_action = 0;
    behavior_count++;
    return behaviors[idx].record_id;
}

int zt_assess_risk(int identity_id) {
    int total_risk = 0;
    int count = 0;
    for (int i = 0; i < behavior_count; i++) {
        if (behaviors[i].identity_id != identity_id) continue;
        total_risk += behaviors[i].risk_level;
        count++;
    }
    if (count == 0) return RISK_LOW;
    int avg = total_risk / count;
    return (avg > RISK_CRITICAL) ? RISK_CRITICAL : avg;
}

int zt_adaptive_response(int record_id) {
    for (int i = 0; i < behavior_count; i++) {
        if (behaviors[i].record_id != record_id) continue;
        if (!behaviors[i].anomaly_detected) {
            print_str("  No anomaly on record #");
            print_int(record_id);
            print_str(" - no action needed\n");
            return 0;
        }
        /* Adaptive response based on risk level */
        if (behaviors[i].risk_level == RISK_CRITICAL) {
            behaviors[i].response_action = 3; /* quarantine */
            print_str("  CRITICAL anomaly #");
            print_int(record_id);
            print_str(" -> QUARANTINE identity #");
            print_int(behaviors[i].identity_id);
            print_str("\n");
            /* Revoke all active permissions for this identity */
            for (int j = 0; j < permission_count; j++) {
                if (permissions[j].identity_id == behaviors[i].identity_id &&
                    permissions[j].active) {
                    permissions[j].active = 0;
                }
            }
            audit_log(AUDIT_ANOMALY, behaviors[i].identity_id,
                      "CRITICAL: identity quarantined", RISK_CRITICAL, 0);
        } else if (behaviors[i].risk_level == RISK_HIGH) {
            behaviors[i].response_action = 2; /* block */
            print_str("  HIGH anomaly #");
            print_int(record_id);
            print_str(" -> BLOCK identity #");
            print_int(behaviors[i].identity_id);
            print_str("\n");
            audit_log(AUDIT_ANOMALY, behaviors[i].identity_id,
                      "HIGH: identity blocked", RISK_HIGH, 0);
        } else {
            behaviors[i].response_action = 1; /* alert */
            print_str("  MEDIUM anomaly #");
            print_int(record_id);
            print_str(" -> ALERT issued for identity #");
            print_int(behaviors[i].identity_id);
            print_str("\n");
            audit_log(AUDIT_ANOMALY, behaviors[i].identity_id,
                      "MEDIUM: alert issued", RISK_MEDIUM, 0);
        }
        return behaviors[i].response_action;
    }
    return -1;
}

/* ========================================================================
 * Feature 6: Security Audit (full-chain logs, compliance, forensics)
 * ======================================================================== */

int zt_audit_report(void) {
    print_str("  === Full-Chain Audit Log ===\n");
    print_str("  ID   Action    Subject  Severity  Result  Description\n");
    print_str("  -----------------------------------------------------------------------\n");
    for (int i = 0; i < audit_count; i++) {
        print_str("  ");
        print_int(audit_logs[i].log_id);
        print_str("  ");
        print_str(audit_action_name(audit_logs[i].action_type));
        /* padding */
        int alen = my_strlen(audit_action_name(audit_logs[i].action_type));
        for (int p = 0; p < 10 - alen; p++) print_str(" ");
        print_int(audit_logs[i].subject_id);
        print_str("  ");
        print_str(risk_level_name(audit_logs[i].severity));
        print_str("    ");
        if (audit_logs[i].result == 0) print_str("OK   ");
        else print_str("FAIL ");
        print_str("  ");
        print_str(audit_logs[i].description);
        print_str("\n");
    }
    print_str("  Total entries: ");
    print_int(audit_count);
    print_str("\n");
    return audit_count;
}

int zt_compliance_report(void) {
    print_str("  === Compliance Report ===\n");
    /* Device compliance summary */
    int compliant_count = 0;
    for (int i = 0; i < device_count; i++) {
        if (devices[i].compliance == COMPLIANT) compliant_count++;
    }
    print_str("  Devices compliant: ");
    print_int(compliant_count);
    print_str("/");
    print_int(device_count);
    print_str("\n");
    /* Active permissions summary */
    int active_perms = 0;
    for (int i = 0; i < permission_count; i++) {
        if (permissions[i].active) active_perms++;
    }
    print_str("  Active permissions: ");
    print_int(active_perms);
    print_str("/");
    print_int(permission_count);
    print_str("\n");
    /* Identity verification status */
    int verified = 0;
    for (int i = 0; i < identity_count; i++) {
        if (identities[i].auth_valid && identities[i].session_active) verified++;
    }
    print_str("  Verified identities: ");
    print_int(verified);
    print_str("/");
    print_int(identity_count);
    print_str("\n");
    /* Anomaly summary */
    int anomalies = 0;
    for (int i = 0; i < behavior_count; i++) {
        if (behaviors[i].anomaly_detected) anomalies++;
    }
    print_str("  Anomalies detected: ");
    print_int(anomalies);
    print_str("\n");
    /* Overall compliance verdict */
    int overall_ok = (compliant_count == device_count) && (anomalies == 0);
    print_str("  Overall: ");
    if (overall_ok) print_str("COMPLIANT\n");
    else print_str("NON-COMPLIANT\n");
    audit_log(AUDIT_POLICY, 0, "compliance report generated", RISK_LOW, 0);
    return overall_ok ? 1 : 0;
}

int zt_forensics_trace(int identity_id) {
    print_str("  === Forensic Trace for Identity #");
    print_int(identity_id);
    print_str(" ===\n");
    int events = 0;
    /* Scan audit log for this identity */
    for (int i = 0; i < audit_count; i++) {
        if (audit_logs[i].subject_id != identity_id) continue;
        print_str("  [tick ");
        print_int(audit_logs[i].timestamp);
        print_str("] ");
        print_str(audit_action_name(audit_logs[i].action_type));
        print_str(" - ");
        print_str(audit_logs[i].description);
        print_str(" (");
        if (audit_logs[i].result == 0) print_str("OK");
        else print_str("FAIL");
        print_str(")\n");
        events++;
    }
    /* Scan behavior records */
    for (int i = 0; i < behavior_count; i++) {
        if (behaviors[i].identity_id != identity_id) continue;
        print_str("  [tick ");
        print_int(behaviors[i].timestamp);
        print_str("] BEHAVIOR ");
        print_str(behaviors[i].pattern);
        print_str(" deviation=");
        print_int(behaviors[i].baseline_deviation);
        print_str("%\n");
        events++;
    }
    print_str("  Total forensic events: ");
    print_int(events);
    print_str("\n");
    audit_log(AUDIT_FORENSIC, identity_id, "forensic trace generated", RISK_MEDIUM, 0);
    return events;
}

/* ========================================================================
 * CLI Entry Point
 * ======================================================================== */

void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);
    int help = 0, test = 0;
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    /* Skip first arg (program name) */
    while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0) help = 1;
        else if (my_strcmp(arg, "-t") == 0 || my_strcmp(arg, "--test") == 0) test = 1;
        while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    }

    print_str("Zero Trust Network v1.0 - Continuous Verification & Least Privilege\n");

    if (help) {
        print_str("Usage: zero_trust_network [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run full zero-trust security test\n");
        print_str("\nFeatures:\n");
        print_str("  1. Continuous Verification - device/user/app identity auth\n");
        print_str("  2. Micro-Segmentation      - network segmentation & isolation\n");
        print_str("  3. Least Privilege          - dynamic permission management\n");
        print_str("  4. Device Trust             - health checks & trust scoring\n");
        print_str("  5. Behavior Analysis        - anomaly detection & adaptive response\n");
        print_str("  6. Security Audit           - full-chain logs & forensics\n");
        return;
    }

    if (test) {
        print_str("=== Zero Trust Network Test ===\n\n");

        /* --- Feature 4: Register devices and assess trust --- */
        print_str("[1] Device Trust Assessment\n");
        int dev1 = zt_register_device("workstation-01", 1, 1, 1);
        int dev2 = zt_register_device("laptop-02", 1, 1, 0);
        int dev3 = zt_register_device("server-03", 0, 1, 1);
        zt_compute_trust_scores();
        print_str("\n");
        zt_compliance_assess(dev1);
        zt_compliance_assess(dev2);
        zt_compliance_assess(dev3);
        print_str("\n");

        /* --- Feature 1: Register identities & continuous verification --- */
        print_str("[2] Continuous Verification\n");
        int user1 = zt_register_identity("admin-alice", IDENTITY_USER, AUTH_MFA);
        int app1 = zt_register_identity("api-gateway", IDENTITY_APP, AUTH_CERT);
        int svc1 = zt_register_identity("backup-svc", IDENTITY_SERVICE, AUTH_TOKEN);
        zt_continuous_verify_all();
        print_str("\n");

        /* --- Feature 2: Micro-segmentation --- */
        print_str("[3] Micro-Segmentation\n");
        int seg_prod = zt_create_segment("production", SEG_ACTION_DENY,
                                         "deny cross-zone traffic");
        int seg_dev = zt_create_segment("development", SEG_ACTION_ALLOW,
                                        "allow intra-zone traffic");
        int wl1 = zt_register_workload("web-server");
        int wl2 = zt_register_workload("db-server");
        int wl3 = zt_register_workload("dev-app");
        zt_isolate_workload(wl1, seg_prod);
        zt_isolate_workload(wl2, seg_prod);
        zt_isolate_workload(wl3, seg_dev);
        zt_enforce_policy(seg_prod);
        zt_enforce_policy(seg_dev);
        print_str("\n");

        /* --- Feature 3: Least privilege --- */
        print_str("[4] Least Privilege\n");
        zt_grant_permission(user1, "admin-panel", PERM_ADMIN, 0);
        zt_grant_permission(app1, "api-endpoint", PERM_READ, 0);
        zt_request_on_demand(svc1, "temp-storage", PERM_WRITE);
        int temp_perm = zt_request_on_demand(user1, "debug-console", PERM_EXEC);
        print_str("  Active permissions: ");
        print_int(permission_count);
        print_str("\n");
        zt_revoke_permission(temp_perm);
        print_str("\n");

        /* --- Feature 5: Behavior analysis --- */
        print_str("[5] Behavior Analysis\n");
        int b1 = zt_record_behavior(user1, "normal-login-pattern", 5);
        int b2 = zt_record_behavior(app1, "unusual-api-volume", 45);
        int b3 = zt_record_behavior(svc1, "data-access-spike", 75);
        (void)b1;
        print_str("  Risk for identity #");
        print_int(app1);
        print_str(": ");
        print_str(risk_level_name(zt_assess_risk(app1)));
        print_str("\n");
        print_str("  Risk for identity #");
        print_int(svc1);
        print_str(": ");
        print_str(risk_level_name(zt_assess_risk(svc1)));
        print_str("\n");
        zt_adaptive_response(b2);
        zt_adaptive_response(b3);
        print_str("\n");

        /* --- Feature 6: Security audit --- */
        print_str("[6] Security Audit\n");
        zt_audit_report();
        print_str("\n");
        zt_compliance_report();
        print_str("\n");
        zt_forensics_trace(svc1);
        print_str("\n");

        /* --- Expire dynamic permissions --- */
        /* Advance tick counter to trigger expiry */
        for (int i = 0; i < 10; i++) tick_counter++;
        int expired = zt_expire_permissions();
        print_str("  Expired permissions: ");
        print_int(expired);
        print_str("\n");

        print_str("\n=== Zero Trust Network Test Complete ===\n");
        return;
    }

    print_str("Use -h for help, -t for test\n");
}
