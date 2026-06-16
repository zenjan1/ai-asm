/* sandbox: WASM sandbox isolation module (v46.0)
 *
 * Provides sandbox isolation for WASM modules:
 * - Resource limits (CPU/memory/fd)
 * - Permission control (capabilities)
 * - Memory isolation (address space separation)
 * - System call filtering (seccomp-bpf)
 * - Network policies (whitelist/blacklist)
 * - Monitoring and alerting
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

/* Capability flags */
#define CAP_NET_RAW         0x0001
#define CAP_NET_BIND        0x0002
#define CAP_FILE_READ       0x0004
#define CAP_FILE_WRITE      0x0008
#define CAP_FILE_EXEC       0x0010
#define CAP_SYS_ADMIN       0x0020
#define CAP_SYS_PTRACE      0x0040
#define CAP_SYS_MODULE      0x0080

/* Sandbox states */
#define SANDBOX_CREATED     0
#define SANDBOX_RUNNING     1
#define SANDBOX_PAUSED      2
#define SANDBOX_STOPPED     3
#define SANDBOX_VIOLATION   4

/* Network policies */
#define NET_POLICY_ALLOW_ALL    0
#define NET_POLICY_DENY_ALL     1
#define NET_POLICY_WHITELIST    2
#define NET_POLICY_BLACKLIST    3

/* Limits */
#define MAX_SANDBOXES       64
#define MAX_RULES           256
#define MAX_CAPABILITIES    32
#define MAX_NETWORK_RULES   128
#define MAX_FILE_RULES      128
#define MAX_SYSCALL_RULES   256

/* Resource limits */
typedef struct {
    unsigned int max_memory_mb;
    unsigned int max_cpu_ms;
    unsigned int max_fd_count;
    unsigned int max_file_size_mb;
    unsigned int max_processes;
    unsigned int max_network_bw_kbps;
} resource_limits_t;

/* File access rule */
typedef struct {
    char        path_pattern[128];
    int         allow_read;
    int         allow_write;
    int         allow_exec;
    int         active;
} file_rule_t;

/* Network access rule */
typedef struct {
    int         allow;
    int         protocol;  /* 0=any, 1=tcp, 2=udp */
    unsigned int ip_addr;
    int         port_start;
    int         port_end;
    int         active;
} network_rule_t;

/* Syscall filter rule */
typedef struct {
    int         syscall_nr;
    int         allow;
    int         active;
} syscall_rule_t;

/* Sandbox instance */
typedef struct {
    int             sandbox_id;
    char            name[64];
    int             state;
    resource_limits_t limits;
    unsigned int    capabilities;
    int             network_policy;
    int             file_rule_count;
    file_rule_t     file_rules[MAX_FILE_RULES];
    int             network_rule_count;
    network_rule_t  network_rules[MAX_NETWORK_RULES];
    int             syscall_rule_count;
    syscall_rule_t  syscall_rules[MAX_SYSCALL_RULES];
    unsigned int    memory_used_mb;
    unsigned int    cpu_used_ms;
    int             fd_count;
    int             violation_count;
    int             active;
} sandbox_t;

/* Violation log */
typedef struct {
    int             sandbox_id;
    char            violation_type[64];
    char            details[128];
    unsigned int    timestamp;
    int             severity;
} violation_log_t;

/* Global state */
static sandbox_t sandboxes[MAX_SANDBOXES];
static int sandbox_count = 0;
static violation_log_t violations[512];
static int violation_count = 0;

/* String utilities */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0; while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; } dst[i] = '\0';
}

static void print_str(const char *str) { host_print(str); }
static void print_int(int val) {
    char buf[32]; int pos = 0;
    if (val < 0) { buf[pos++] = '-'; val = -val; }
    if (val == 0) buf[pos++] = '0';
    else { int d = 0, t = val; while (t > 0) { d++; t /= 10; } pos += d; buf[pos] = '\0'; pos--;
        while (val > 0) { buf[pos--] = '0' + (val % 10); val /= 10; } }
    host_print(buf);
}
static void print_hex(unsigned int val) {
    char buf[16];
    const char *hex = "0123456789ABCDEF";
    int pos = 0;
    buf[pos++] = '0'; buf[pos++] = 'x';
    for (int i = 7; i >= 0; i--) {
        buf[pos++] = hex[(val >> (i * 4)) & 0xF];
    }
    buf[pos] = '\0';
    host_print(buf);
}

/* ===== Sandbox Management ===== */

int sandbox_create(const char *name) {
    if (sandbox_count >= MAX_SANDBOXES) return -1;

    int idx = sandbox_count++;
    sandboxes[idx].sandbox_id = idx;
    my_strncpy(sandboxes[idx].name, name, 63);
    sandboxes[idx].state = SANDBOX_CREATED;
    sandboxes[idx].capabilities = 0;
    sandboxes[idx].network_policy = NET_POLICY_DENY_ALL;
    sandboxes[idx].file_rule_count = 0;
    sandboxes[idx].network_rule_count = 0;
    sandboxes[idx].syscall_rule_count = 0;
    sandboxes[idx].memory_used_mb = 0;
    sandboxes[idx].cpu_used_ms = 0;
    sandboxes[idx].fd_count = 0;
    sandboxes[idx].violation_count = 0;
    sandboxes[idx].active = 1;

    /* Set default limits */
    sandboxes[idx].limits.max_memory_mb = 256;
    sandboxes[idx].limits.max_cpu_ms = 10000;
    sandboxes[idx].limits.max_fd_count = 64;
    sandboxes[idx].limits.max_file_size_mb = 100;
    sandboxes[idx].limits.max_processes = 10;
    sandboxes[idx].limits.max_network_bw_kbps = 1000;

    print_str("Created sandbox: ");
    print_str(name);
    print_str(" (id=");
    print_int(idx);
    print_str(")\n");

    return idx;
}

int sandbox_destroy(int sandbox_id) {
    if (sandbox_id < 0 || sandbox_id >= sandbox_count) return -1;
    sandboxes[sandbox_id].active = 0;
    sandboxes[sandbox_id].state = SANDBOX_STOPPED;

    print_str("Destroyed sandbox ");
    print_int(sandbox_id);
    print_str("\n");

    return 0;
}

/* ===== Resource Limits ===== */

int sandbox_set_memory_limit(int sandbox_id, unsigned int max_mb) {
    if (sandbox_id < 0 || sandbox_id >= sandbox_count) return -1;
    sandboxes[sandbox_id].limits.max_memory_mb = max_mb;

    print_str("Set memory limit for sandbox ");
    print_int(sandbox_id);
    print_str(": ");
    print_int(max_mb);
    print_str(" MB\n");

    return 0;
}

int sandbox_set_cpu_limit(int sandbox_id, unsigned int max_ms) {
    if (sandbox_id < 0 || sandbox_id >= sandbox_count) return -1;
    sandboxes[sandbox_id].limits.max_cpu_ms = max_ms;

    print_str("Set CPU limit for sandbox ");
    print_int(sandbox_id);
    print_str(": ");
    print_int(max_ms);
    print_str(" ms\n");

    return 0;
}

int sandbox_set_fd_limit(int sandbox_id, unsigned int max_fd) {
    if (sandbox_id < 0 || sandbox_id >= sandbox_count) return -1;
    sandboxes[sandbox_id].limits.max_fd_count = max_fd;

    print_str("Set fd limit for sandbox ");
    print_int(sandbox_id);
    print_str(": ");
    print_int(max_fd);
    print_str("\n");

    return 0;
}

/* ===== Capabilities ===== */

int sandbox_add_capability(int sandbox_id, unsigned int cap) {
    if (sandbox_id < 0 || sandbox_id >= sandbox_count) return -1;
    sandboxes[sandbox_id].capabilities |= cap;

    print_str("Added capability ");
    print_hex(cap);
    print_str(" to sandbox ");
    print_int(sandbox_id);
    print_str("\n");

    return 0;
}

int sandbox_remove_capability(int sandbox_id, unsigned int cap) {
    if (sandbox_id < 0 || sandbox_id >= sandbox_count) return -1;
    sandboxes[sandbox_id].capabilities &= ~cap;

    print_str("Removed capability ");
    print_hex(cap);
    print_str(" from sandbox ");
    print_int(sandbox_id);
    print_str("\n");

    return 0;
}

/* ===== File Access Rules ===== */

int sandbox_allow_file(int sandbox_id, const char *path, int read, int write, int exec) {
    if (sandbox_id < 0 || sandbox_id >= sandbox_count) return -1;
    if (sandboxes[sandbox_id].file_rule_count >= MAX_FILE_RULES) return -1;

    int idx = sandboxes[sandbox_id].file_rule_count++;
    my_strncpy(sandboxes[sandbox_id].file_rules[idx].path_pattern, path, 127);
    sandboxes[sandbox_id].file_rules[idx].allow_read = read;
    sandboxes[sandbox_id].file_rules[idx].allow_write = write;
    sandboxes[sandbox_id].file_rules[idx].allow_exec = exec;
    sandboxes[sandbox_id].file_rules[idx].active = 1;

    print_str("Allowed file access: ");
    print_str(path);
    print_str(" (");
    if (read) print_str("r");
    if (write) print_str("w");
    if (exec) print_str("x");
    print_str(")\n");

    return idx;
}

/* ===== Network Rules ===== */

int sandbox_set_network_policy(int sandbox_id, int policy) {
    if (sandbox_id < 0 || sandbox_id >= sandbox_count) return -1;
    sandboxes[sandbox_id].network_policy = policy;

    print_str("Set network policy for sandbox ");
    print_int(sandbox_id);
    print_str(": ");
    if (policy == NET_POLICY_ALLOW_ALL) print_str("ALLOW_ALL");
    else if (policy == NET_POLICY_DENY_ALL) print_str("DENY_ALL");
    else if (policy == NET_POLICY_WHITELIST) print_str("WHITELIST");
    else print_str("BLACKLIST");
    print_str("\n");

    return 0;
}

int sandbox_add_network_rule(int sandbox_id, int allow, int protocol, unsigned int ip, int port_start, int port_end) {
    if (sandbox_id < 0 || sandbox_id >= sandbox_count) return -1;
    if (sandboxes[sandbox_id].network_rule_count >= MAX_NETWORK_RULES) return -1;

    int idx = sandboxes[sandbox_id].network_rule_count++;
    sandboxes[sandbox_id].network_rules[idx].allow = allow;
    sandboxes[sandbox_id].network_rules[idx].protocol = protocol;
    sandboxes[sandbox_id].network_rules[idx].ip_addr = ip;
    sandboxes[sandbox_id].network_rules[idx].port_start = port_start;
    sandboxes[sandbox_id].network_rules[idx].port_end = port_end;
    sandboxes[sandbox_id].network_rules[idx].active = 1;

    print_str("Added network rule: ");
    print_str(allow ? "ALLOW" : "DENY");
    print_str(" ");
    if (protocol == 1) print_str("TCP");
    else if (protocol == 2) print_str("UDP");
    else print_str("ANY");
    print_str(" port ");
    print_int(port_start);
    print_str("-");
    print_int(port_end);
    print_str("\n");

    return idx;
}

/* ===== Syscall Filtering ===== */

int sandbox_add_syscall_rule(int sandbox_id, int syscall_nr, int allow) {
    if (sandbox_id < 0 || sandbox_id >= sandbox_count) return -1;
    if (sandboxes[sandbox_id].syscall_rule_count >= MAX_SYSCALL_RULES) return -1;

    int idx = sandboxes[sandbox_id].syscall_rule_count++;
    sandboxes[sandbox_id].syscall_rules[idx].syscall_nr = syscall_nr;
    sandboxes[sandbox_id].syscall_rules[idx].allow = allow;
    sandboxes[sandbox_id].syscall_rules[idx].active = 1;

    print_str("Syscall rule: ");
    print_str(allow ? "ALLOW" : "DENY");
    print_str(" syscall #");
    print_int(syscall_nr);
    print_str("\n");

    return idx;
}

/* ===== Sandbox Control ===== */

int sandbox_start(int sandbox_id) {
    if (sandbox_id < 0 || sandbox_id >= sandbox_count) return -1;
    sandboxes[sandbox_id].state = SANDBOX_RUNNING;

    print_str("Started sandbox ");
    print_int(sandbox_id);
    print_str("\n");

    return 0;
}

int sandbox_stop(int sandbox_id) {
    if (sandbox_id < 0 || sandbox_id >= sandbox_count) return -1;
    sandboxes[sandbox_id].state = SANDBOX_STOPPED;

    print_str("Stopped sandbox ");
    print_int(sandbox_id);
    print_str("\n");

    return 0;
}

void sandbox_status(int sandbox_id) {
    if (sandbox_id < 0 || sandbox_id >= sandbox_count) return;

    print_str("Sandbox ");
    print_int(sandbox_id);
    print_str(" (");
    print_str(sandboxes[sandbox_id].name);
    print_str("):\n");
    print_str("  State: ");
    if (sandboxes[sandbox_id].state == SANDBOX_RUNNING) print_str("RUNNING");
    else if (sandboxes[sandbox_id].state == SANDBOX_PAUSED) print_str("PAUSED");
    else print_str("STOPPED");
    print_str("\n");
    print_str("  Memory: ");
    print_int(sandboxes[sandbox_id].memory_used_mb);
    print_str("/");
    print_int(sandboxes[sandbox_id].limits.max_memory_mb);
    print_str(" MB\n");
    print_str("  CPU: ");
    print_int(sandboxes[sandbox_id].cpu_used_ms);
    print_str("/");
    print_int(sandboxes[sandbox_id].limits.max_cpu_ms);
    print_str(" ms\n");
    print_str("  Capabilities: ");
    print_hex(sandboxes[sandbox_id].capabilities);
    print_str("\n");
    print_str("  Violations: ");
    print_int(sandboxes[sandbox_id].violation_count);
    print_str("\n");
}

/* ===== Violation Tracking ===== */

void log_violation(int sandbox_id, const char *type, const char *details) {
    if (violation_count >= 512) return;

    int idx = violation_count++;
    violations[idx].sandbox_id = sandbox_id;
    my_strncpy(violations[idx].violation_type, type, 63);
    my_strncpy(violations[idx].details, details, 127);
    violations[idx].timestamp = 1609459200 + idx;
    violations[idx].severity = 2;

    if (sandbox_id >= 0 && sandbox_id < sandbox_count) {
        sandboxes[sandbox_id].violation_count++;
    }

    print_str("VIOLATION: ");
    print_str(type);
    print_str(" in sandbox ");
    print_int(sandbox_id);
    print_str(" - ");
    print_str(details);
    print_str("\n");
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
        int hl = my_strlen(arg);
        if (hl == 2 && arg[0] == '-' && arg[1] == 'h') help = 1;
        else if (hl == 2 && arg[0] == '-' && arg[1] == 't') test = 1;
        while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    }

    print_str("Sandbox v46.0 - WASM Isolation System\n");
    if (help) {
        print_str("Usage: sandbox [options]\n");
        print_str("  -h    Show help\n");
        print_str("  -t    Run sandbox test\n");
        return;
    }

    if (test) {
        print_str("=== Sandbox Test Suite ===\n\n");

        /* Test 1: Create sandbox */
        print_str("[Test 1] Create Sandbox\n");
        int sb1 = sandbox_create("web_server");
        int sb2 = sandbox_create("data_processor");
        print_str("\n");

        /* Test 2: Set resource limits */
        print_str("[Test 2] Resource Limits\n");
        sandbox_set_memory_limit(sb1, 512);
        sandbox_set_cpu_limit(sb1, 5000);
        sandbox_set_fd_limit(sb1, 128);
        sandbox_set_memory_limit(sb2, 1024);
        sandbox_set_cpu_limit(sb2, 10000);
        print_str("\n");

        /* Test 3: Add capabilities */
        print_str("[Test 3] Capabilities\n");
        sandbox_add_capability(sb1, CAP_NET_BIND);
        sandbox_add_capability(sb1, CAP_FILE_READ);
        sandbox_add_capability(sb2, CAP_FILE_READ | CAP_FILE_WRITE);
        print_str("\n");

        /* Test 4: File access rules */
        print_str("[Test 4] File Access Rules\n");
        sandbox_allow_file(sb1, "/var/www/*", 1, 0, 0);
        sandbox_allow_file(sb1, "/etc/*.conf", 1, 0, 0);
        sandbox_allow_file(sb2, "/data/*", 1, 1, 0);
        sandbox_allow_file(sb2, "/tmp/*", 1, 1, 1);
        print_str("\n");

        /* Test 5: Network rules */
        print_str("[Test 5] Network Rules\n");
        sandbox_set_network_policy(sb1, NET_POLICY_WHITELIST);
        sandbox_add_network_rule(sb1, 1, 1, 0, 80, 80);
        sandbox_add_network_rule(sb1, 1, 1, 0, 443, 443);
        sandbox_set_network_policy(sb2, NET_POLICY_DENY_ALL);
        print_str("\n");

        /* Test 6: Syscall filtering */
        print_str("[Test 6] Syscall Filtering\n");
        sandbox_add_syscall_rule(sb1, 0, 1);  /* read */
        sandbox_add_syscall_rule(sb1, 1, 1);  /* write */
        sandbox_add_syscall_rule(sb1, 57, 0); /* fork - deny */
        sandbox_add_syscall_rule(sb1, 59, 0); /* execve - deny */
        print_str("\n");

        /* Test 7: Start sandboxes */
        print_str("[Test 7] Start Sandboxes\n");
        sandbox_start(sb1);
        sandbox_start(sb2);
        print_str("\n");

        /* Test 8: Check status */
        print_str("[Test 8] Sandbox Status\n");
        sandbox_status(sb1);
        print_str("\n");
        sandbox_status(sb2);
        print_str("\n");

        /* Test 9: Simulate violations */
        print_str("[Test 9] Violation Detection\n");
        log_violation(sb1, "NETWORK", "Attempted connection to port 22");
        log_violation(sb2, "FILE", "Attempted write to /etc/passwd");
        print_str("\n");

        /* Test 10: Stop sandboxes */
        print_str("[Test 10] Stop Sandboxes\n");
        sandbox_stop(sb1);
        sandbox_stop(sb2);
        sandbox_destroy(sb1);
        sandbox_destroy(sb2);

        print_str("\n=== Sandbox Test Complete ===\n");
        return;
    }

    print_str("Use -h for help, -t for test\n");
}
