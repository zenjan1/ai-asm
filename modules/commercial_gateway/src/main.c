/* commercial_gateway: Commercial API gateway (v15.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Billing models */
#define BILLING_FREE      0
#define BILLING_FIXED     1
#define BILLING_PAY_PER_USE 2

/* Rate limit levels */
#define RATE_UNLIMITED    0
#define RATE_LOW          10
#define RATE_MEDIUM       100
#define RATE_HIGH         1000

/* Limits */
#define MAX_APIS        32
#define API_NAME_LEN    64
#define MODEL_NAME_LEN  32

/* API statistics */
typedef struct {
    int   api_id;
    char  name[API_NAME_LEN];
    int   call_count;
    int   success_count;
    int   error_count;
    int   avg_latency_ms;
    int   quota_used;
    int   quota_limit;
    int   rate_limit;
    int   billing_model;
    int   price_per_call;
} api_stats_t;

static api_stats_t apis[MAX_APIS];
static int api_count = 0;
static int next_api_id = 1;

/* Global billing stats */
static int total_revenue = 0;
static int total_calls_today = 0;

/* String utilities */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
}
static void my_strcpy(char *dst, const char *src) { while (*src) *dst++ = *src++; *dst = '\0'; }
static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0; while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; } dst[i] = '\0';
}
static int my_strncmp(const char *a, const char *b, int n) {
    for (int i = 0; i < n; i++) {
        if (!a[i] || !b[i]) return (a[i] ? 1 : 0) - (b[i] ? 1 : 0);
        if (a[i] != b[i]) return a[i] - b[i];
    } return 0;
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

/* Get billing model name */
static const char *billing_name(int model) {
    if (model == BILLING_FREE) return "free";
    if (model == BILLING_FIXED) return "fixed";
    if (model == BILLING_PAY_PER_USE) return "pay-per-use";
    return "unknown";
}

/* Register an API endpoint */
static int register_api(const char *name, int quota, int billing, int price) {
    if (api_count >= MAX_APIS) return -1;
    int idx = api_count;
    apis[idx].api_id = next_api_id;
    my_strncpy(apis[idx].name, name, API_NAME_LEN - 1);
    apis[idx].call_count = 0;
    apis[idx].success_count = 0;
    apis[idx].error_count = 0;
    apis[idx].avg_latency_ms = 0;
    apis[idx].quota_used = 0;
    apis[idx].quota_limit = quota;
    apis[idx].rate_limit = RATE_UNLIMITED;
    apis[idx].billing_model = billing;
    apis[idx].price_per_call = price;
    api_count++;
    int id = next_api_id++;
    print_str("[gateway] registered API: ");
    print_str(name);
    print_str(" (quota=");
    print_int(quota);
    print_str(", billing=");
    print_str(billing_name(billing));
    print_str(")\n");
    return id;
}

/* Track an API call */
int gateway_track_call(const char *api_name) {
    for (int i = 0; i < api_count; i++) {
        if (my_strcmp(apis[i].name, api_name) == 0) {
            apis[i].call_count++;
            apis[i].quota_used++;
            total_calls_today++;
            /* Simulate latency */
            int lat = 5 + (apis[i].call_count % 50);
            apis[i].avg_latency_ms = (apis[i].avg_latency_ms + lat) / 2;
            /* Track revenue for pay-per-use */
            if (apis[i].billing_model == BILLING_PAY_PER_USE) {
                total_revenue += apis[i].price_per_call;
            }
            print_str("[gateway] call tracked: ");
            print_str(api_name);
            print_str(" (#");
            print_int(apis[i].call_count);
            print_str(")\n");
            return 0;
        }
    }
    print_str("[gateway] API not found: ");
    print_str(api_name);
    print_str("\n");
    return -1;
}

/* Check quota for an API */
int gateway_check_quota(const char *api_name) {
    for (int i = 0; i < api_count; i++) {
        if (my_strcmp(apis[i].name, api_name) == 0) {
            if (apis[i].quota_limit > 0 && apis[i].quota_used >= apis[i].quota_limit) {
                print_str("[gateway] quota exceeded: ");
                print_str(api_name);
                print_str(" (");
                print_int(apis[i].quota_used);
                print_str("/");
                print_int(apis[i].quota_limit);
                print_str(")\n");
                return 0;
            }
            print_str("[gateway] quota OK: ");
            print_str(api_name);
            print_str(" (");
            print_int(apis[i].quota_used);
            print_str("/");
            print_int(apis[i].quota_limit);
            print_str(")\n");
            return 1;
        }
    }
    return -1;
}

/* Get usage report */
int gateway_get_usage_report(int month) {
    print_str("=== Usage Report (month ");
    print_int(month);
    print_str(") ===\n");
    print_str("Total API calls today: ");
    print_int(total_calls_today);
    print_str("\nTotal revenue: $");
    print_int(total_revenue);
    print_str("\n");
    for (int i = 0; i < api_count; i++) {
        print_str("  ");
        print_str(apis[i].name);
        print_str(": calls=");
        print_int(apis[i].call_count);
        print_str(" success=");
        print_int(apis[i].success_count);
        print_str(" errors=");
        print_int(apis[i].error_count);
        print_str(" avg_lat=");
        print_int(apis[i].avg_latency_ms);
        print_str("ms quota=");
        print_int(apis[i].quota_used);
        print_str("/");
        print_int(apis[i].quota_limit);
        print_str("\n");
    }
    return 0;
}

/* Calculate billing amount */
int gateway_calculate_billing(int billing_id) {
    print_str("=== Billing Calculation ===\n");
    int total = 0;
    for (int i = 0; i < api_count; i++) {
        if (apis[i].billing_model == BILLING_PAY_PER_USE) {
            int cost = apis[i].call_count * apis[i].price_per_call;
            print_str("  ");
            print_str(apis[i].name);
            print_str(": ");
            print_int(apis[i].call_count);
            print_str(" calls x $");
            print_int(apis[i].price_per_call);
            print_str(" = $");
            print_int(cost);
            print_str("\n");
            total += cost;
        } else if (apis[i].billing_model == BILLING_FIXED) {
            print_str("  ");
            print_str(apis[i].name);
            print_str(": fixed monthly fee\n");
            total += 100;
        } else {
            print_str("  ");
            print_str(apis[i].name);
            print_str(": free\n");
        }
    }
    print_str("Total: $");
    print_int(total);
    print_str("\n");
    return total;
}

/* Apply rate limit */
int gateway_apply_rate_limit(const char *api_name, int rate) {
    for (int i = 0; i < api_count; i++) {
        if (my_strcmp(apis[i].name, api_name) == 0) {
            apis[i].rate_limit = rate;
            print_str("[gateway] rate limit applied: ");
            print_str(api_name);
            print_str(" = ");
            print_int(rate);
            print_str(" req/min\n");
            return 0;
        }
    }
    return -1;
}

/* ===== CLI entry ===== */
void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);
    int help = 0, test = 0, report = 0;
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0) help = 1;
        else if (my_strcmp(arg, "-t") == 0 || my_strcmp(arg, "--test") == 0) test = 1;
        else if (my_strcmp(arg, "-r") == 0 || my_strcmp(arg, "--report") == 0) report = 1;
        while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    }
    print_str("Commercial Gateway v15.0 - API Gateway & Billing\n");
    if (help) {
        print_str("Usage: commercial_gateway [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run gateway test\n");
        print_str("  -r, --report  Show usage report\n");
        return;
    }
    if (test) {
        print_str("=== Commercial Gateway Test ===\n");
        print_str("\n--- Register APIs ---\n");
        register_api("ai_query", 1000, BILLING_PAY_PER_USE, 5);
        register_api("ai_stream", 500, BILLING_PAY_PER_USE, 10);
        register_api("health_check", 0, BILLING_FREE, 0);
        register_api("premium_analytics", 100, BILLING_FIXED, 0);

        print_str("\n--- Track Calls ---\n");
        gateway_track_call("ai_query");
        gateway_track_call("ai_query");
        gateway_track_call("ai_query");
        gateway_track_call("ai_stream");
        gateway_track_call("ai_stream");
        gateway_track_call("health_check");

        print_str("\n--- Check Quotas ---\n");
        gateway_check_quota("ai_query");
        gateway_check_quota("ai_stream");

        print_str("\n--- Apply Rate Limits ---\n");
        gateway_apply_rate_limit("ai_query", 100);
        gateway_apply_rate_limit("ai_stream", 50);

        print_str("\n--- Billing ---\n");
        gateway_calculate_billing(1);

        print_str("\n=== Test Complete ===\n");
        return;
    }
    if (report) { gateway_get_usage_report(1); return; }
    print_str("Use -h for help, -t for test, -r for report\n");
}
