/* multi_factor_auth: Multi-factor authentication system (v34.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_SESSIONS   64
#define MAX_FACTORS    16
#define MAX_LOGS       128
#define MAX_NAME       64
#define MAX_TOKEN      32

/* Factor types */
#define FACTOR_KNOWLEDGE   0  /* Password/PIN */
#define FACTOR_POSSESSION  1  /* TOTP/HOTP token */
#define FACTOR_BIOMETRIC   2  /* Fingerprint/Face (reserved) */
#define FACTOR_HARDWARE    3  /* Hardware token (reserved) */

/* Factor status */
#define STATUS_INCOMPLETE  0
#define STATUS_IN_PROGRESS 1
#define STATUS_VERIFIED    2
#define STATUS_FAILED      3
#define STATUS_LOCKED      4

/* Log result */
#define LOG_SUCCESS    0
#define LOG_FAILURE    1

/* Auth session */
typedef struct {
    int   session_id;
    char  user_id[MAX_NAME];
    int   factors_completed;
    int   total_factors;
    int   risk_score;        /* 0-100, higher=riskier */
    int   status;
    int   timestamp;
    char  token[MAX_TOKEN];
} auth_session_t;

/* Factor config */
typedef struct {
    int   factor_id;
    int   type;
    int   required;          /* 1=mandatory, 0=optional */
    int   weight;            /* contribution to auth score */
    int   enabled;
} factor_config_t;

/* Auth log */
typedef struct {
    int   log_id;
    int   session_id;
    int   factor_type;
    int   result;
    int   timestamp;
} auth_log_t;

static auth_session_t sessions[MAX_SESSIONS];
static int session_count = 0;
static int next_session_id = 1;

static factor_config_t factors[MAX_FACTORS];
static int factor_count = 0;
static int next_factor_id = 1;

static auth_log_t logs[MAX_LOGS];
static int log_count = 0;
static int next_log_id = 1;

/* String utilities */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
}
static int my_strncmp(const char *a, const char *b, int n) {
    for (int i = 0; i < n; i++) {
        if (!a[i] || !b[i] || a[i] != b[i]) return (a[i] ? a[i] : 0) - (b[i] ? b[i] : 0);
    } return 0;
}
static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0; while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; } dst[i] = '\0';
}

/* Print utilities */
static void print_str(const char *str) { host_print(str); }
static void print_int(int val) {
    char buf[32]; int pos = 0;
    if (val < 0) { buf[pos++] = '-'; val = -val; }
    if (val == 0) buf[pos++] = '0';
    else { int d = 0, t = val; while (t > 0) { d++; t /= 10; } pos += d; buf[pos] = '\0'; pos--;
        while (val > 0) { buf[pos--] = '0' + (val % 10); val /= 10; } }
    host_print(buf);
}

static const char *factor_type_name(int t) {
    if (t == FACTOR_KNOWLEDGE) return "Knowledge (PIN)";
    if (t == FACTOR_POSSESSION) return "Possession (TOTP)";
    if (t == FACTOR_BIOMETRIC) return "Biometric";
    return "Hardware Token";
}

static const char *status_name(int s) {
    if (s == STATUS_INCOMPLETE) return "incomplete";
    if (s == STATUS_IN_PROGRESS) return "in progress";
    if (s == STATUS_VERIFIED) return "verified";
    if (s == STATUS_FAILED) return "failed";
    return "locked";
}

/* Register authentication factor */
int mfa_register_factor(int type, int required, int weight) {
    if (factor_count >= MAX_FACTORS) return -1;
    int idx = factor_count;
    factors[idx].factor_id = next_factor_id++;
    factors[idx].type = type;
    factors[idx].required = required;
    factors[idx].weight = weight;
    factors[idx].enabled = 1;
    factor_count++;
    return factors[idx].factor_id;
}

/* Create auth session */
int mfa_create_session(const char *user_id) {
    if (session_count >= MAX_SESSIONS) return -1;
    int idx = session_count;
    sessions[idx].session_id = next_session_id++;
    my_strncpy(sessions[idx].user_id, user_id, MAX_NAME - 1);
    sessions[idx].factors_completed = 0;
    sessions[idx].total_factors = 0;
    sessions[idx].risk_score = 50;  /* Default medium risk */
    sessions[idx].status = STATUS_INCOMPLETE;
    sessions[idx].timestamp = session_count + 1;
    my_strncpy(sessions[idx].token, "NONE", MAX_TOKEN - 1);
    /* Count enabled factors */
    for (int i = 0; i < factor_count; i++) {
        if (factors[i].enabled) sessions[idx].total_factors++;
    }
    return sessions[idx].session_id;
}

/* Submit factor verification */
int mfa_submit_factor(int session_id, int factor_type, const char *token) {
    for (int i = 0; i < session_count; i++) {
        if (sessions[i].session_id == session_id) {
            if (sessions[i].status == STATUS_LOCKED) return -1;
            if (sessions[i].status == STATUS_VERIFIED) return -1;
            sessions[i].status = STATUS_IN_PROGRESS;
            /* Simulate verification: accept if token is not empty */
            int verified = my_strlen(token) > 0 ? 1 : 0;
            /* Log the attempt */
            if (log_count < MAX_LOGS) {
                int lidx = log_count;
                logs[lidx].log_id = next_log_id++;
                logs[lidx].session_id = session_id;
                logs[lidx].factor_type = factor_type;
                logs[lidx].result = verified ? LOG_SUCCESS : LOG_FAILURE;
                logs[lidx].timestamp = log_count + 1;
                log_count++;
            }
            if (verified) {
                sessions[i].factors_completed++;
                sessions[i].risk_score -= 15;  /* Each factor reduces risk */
                if (sessions[i].risk_score < 0) sessions[i].risk_score = 0;
                my_strncpy(sessions[i].token, token, MAX_TOKEN - 1);
                /* Check if all required factors completed */
                int all_done = 1;
                for (int j = 0; j < factor_count; j++) {
                    if (factors[j].required && factor_type != factors[j].type) {
                        /* Check if this factor type was already done */
                        int found = 0;
                        for (int k = 0; k < log_count; k++) {
                            if (logs[k].session_id == session_id &&
                                logs[k].factor_type == factors[j].type &&
                                logs[k].result == LOG_SUCCESS) {
                                found = 1; break;
                            }
                        }
                        if (!found) all_done = 0;
                    }
                }
                /* Also check submitted factor */
                int submitted_done = 0;
                for (int k = 0; k < log_count; k++) {
                    if (logs[k].session_id == session_id &&
                        logs[k].factor_type == factor_type &&
                        logs[k].result == LOG_SUCCESS) {
                        submitted_done = 1; break;
                    }
                }
                if (!submitted_done) all_done = 0;

                if (all_done) {
                    sessions[i].status = STATUS_VERIFIED;
                    print_str("    Authentication VERIFIED\n");
                } else {
                    print_str("    Factor accepted, ");
                    print_int(sessions[i].total_factors - sessions[i].factors_completed);
                    print_str(" factors remaining\n");
                }
            } else {
                sessions[i].risk_score += 20;
                if (sessions[i].risk_score > 100) sessions[i].risk_score = 100;
                if (sessions[i].risk_score >= 80) {
                    sessions[i].status = STATUS_LOCKED;
                    print_str("    Account LOCKED (high risk)\n");
                } else {
                    print_str("    Factor rejected, risk increased\n");
                }
            }
            return 0;
        }
    }
    return -1;
}

/* Session listing */
int mfa_list_sessions(void) {
    print_str("  Authentication Sessions\n");
    print_str("  =====================================================================\n");
    print_str("  ID  User            Factors  Risk  Status        Token\n");
    print_str("  ---------------------------------------------------------------------\n");
    for (int i = 0; i < session_count; i++) {
        print_int(sessions[i].session_id);
        print_str("  ");
        print_str(sessions[i].user_id);
        int len = my_strlen(sessions[i].user_id);
        for (int p = 0; p < 16 - len; p++) print_str(" ");
        print_int(sessions[i].factors_completed);
        print_str("/");
        print_int(sessions[i].total_factors);
        print_str("     ");
        print_int(sessions[i].risk_score);
        print_str("   ");
        print_str(status_name(sessions[i].status));
        if (sessions[i].status == STATUS_VERIFIED) print_str("   ");
        else if (sessions[i].status == STATUS_IN_PROGRESS) print_str("  ");
        else print_str("       ");
        print_str(sessions[i].token);
        print_str("\n");
    }
    return session_count;
}

/* Factor config listing */
int mfa_list_factors(void) {
    print_str("  Authentication Factors\n");
    print_str("  =====================================================================\n");
    print_str("  ID  Type               Required  Weight  Enabled\n");
    print_str("  ---------------------------------------------------------------------\n");
    for (int i = 0; i < factor_count; i++) {
        print_int(factors[i].factor_id);
        print_str("  ");
        print_str(factor_type_name(factors[i].type));
        int len = my_strlen(factor_type_name(factors[i].type));
        for (int p = 0; p < 19 - len; p++) print_str(" ");
        if (factors[i].required) print_str("YES       ");
        else print_str("NO        ");
        print_int(factors[i].weight);
        print_str("      ");
        if (factors[i].enabled) print_str("YES");
        else print_str("NO");
        print_str("\n");
    }
    return factor_count;
}

/* Auth log */
int mfa_show_log(void) {
    print_str("  Authentication Log\n");
    print_str("  =====================================================================\n");
    int success = 0, failure = 0;
    for (int i = 0; i < log_count; i++) {
        print_str("  #");
        print_int(logs[i].log_id);
        print_str(" Session #");
        print_int(logs[i].session_id);
        print_str(" ");
        print_str(factor_type_name(logs[i].factor_type));
        int len = my_strlen(factor_type_name(logs[i].factor_type));
        for (int p = 0; p < 22 - len; p++) print_str(" ");
        if (logs[i].result == LOG_SUCCESS) {
            print_str("SUCCESS");
            success++;
        } else {
            print_str("FAILURE");
            failure++;
        }
        print_str("\n");
    }
    print_str("  Total: "); print_int(log_count); print_str(" (");
    print_int(success); print_str(" success, ");
    print_int(failure); print_str(" failed)\n");
    return log_count;
}

/* ===== CLI entry ===== */
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
    print_str("Multi-Factor Auth v34.0 - Multi-Factor Authentication System\n");
    if (help) {
        print_str("Usage: multi_factor_auth [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run MFA test\n");
        return;
    }
    if (test) {
        print_str("=== Multi-Factor Auth Test ===\n\n");
        /* Register factors */
        mfa_register_factor(FACTOR_KNOWLEDGE, 1, 30);
        mfa_register_factor(FACTOR_POSSESSION, 1, 35);
        mfa_register_factor(FACTOR_BIOMETRIC, 0, 20);
        mfa_register_factor(FACTOR_HARDWARE, 0, 15);
        print_str("  Factors registered: "); print_int(factor_count); print_str("\n\n");
        /* List factors */
        mfa_list_factors();
        print_str("\n");
        /* Create session */
        int s1 = mfa_create_session("admin_user");
        int s2 = mfa_create_session("dev_operator");
        print_str("  Sessions created: "); print_int(session_count); print_str("\n\n");
        /* Admin: complete all required factors */
        print_str("Admin authentication:\n");
        mfa_submit_factor(s1, FACTOR_KNOWLEDGE, "T0TP-123456");
        mfa_submit_factor(s1, FACTOR_POSSESSION, "PIN-9876");
        print_str("\n");
        /* Operator: partial (one factor only) */
        print_str("Operator authentication:\n");
        mfa_submit_factor(s2, FACTOR_KNOWLEDGE, "T0TP-654321");
        print_str("\n");
        /* Failed attempt */
        print_str("Failed attempt:\n");
        int s3 = mfa_create_session("attacker");
        mfa_submit_factor(s3, FACTOR_KNOWLEDGE, "");  /* Empty token = fail */
        mfa_submit_factor(s3, FACTOR_KNOWLEDGE, "");
        mfa_submit_factor(s3, FACTOR_KNOWLEDGE, "");
        print_str("\n");
        /* Sessions */
        mfa_list_sessions();
        print_str("\n");
        /* Auth log */
        mfa_show_log();
        print_str("\n=== Multi-Factor Auth Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
