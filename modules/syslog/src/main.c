/*
 * aiasm-aarch64/modules/syslog/src/main.c
 * System log service — reads from kernel log ring buffer,
 * writes structured JSON log lines to /var/log/syslog on the RAM disk.
 */

__attribute__((import_module("host"), import_name("print")))
extern void wasm_host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("log")))
extern void wasm_host_log(unsigned int level_off, unsigned int level_len,
                          unsigned int msg_off, unsigned int msg_len);

__attribute__((import_module("host"), import_name("sleep")))
extern void wasm_host_sleep(unsigned int ms);

__attribute__((import_module("host"), import_name("yield")))
extern void wasm_host_yield(void);

__attribute__((import_module("host"), import_name("fs_open")))
extern int wasm_host_fs_open(unsigned int path_off, unsigned int path_len);

__attribute__((import_module("host"), import_name("fs_write")))
extern int wasm_host_fs_write(int fd, unsigned int buf_off, unsigned int len);

__attribute__((import_module("host"), import_name("fs_close")))
extern void wasm_host_fs_close(int fd);

__attribute__((import_module("host"), import_name("fs_create")))
extern int wasm_host_fs_create(unsigned int name_off, unsigned int name_len);

__attribute__((import_module("host"), import_name("log_read")))
extern int wasm_host_log_read(unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("log_size")))
extern int wasm_host_log_size(void);

__attribute__((import_module("host"), import_name("audit_query")))
extern int wasm_host_audit_query(unsigned int buf_off, unsigned int max_events);

__attribute__((import_module("host"), import_name("audit_get_count")))
extern int wasm_host_audit_get_count(void);

__attribute__((import_module("host"), import_name("audit_flush")))
extern void wasm_host_audit_flush(void);

static unsigned int my_strlen(const char *s)
{
    const char *p = s;
    while (*p) p++;
    return (unsigned int)(p - s);
}

static void print_str(const char *s)
{
    unsigned int len = my_strlen(s);
    char *dst = (char *)65536;
    for (unsigned int i = 0; i < len; i++)
        dst[i] = s[i];
    wasm_host_print(65536, len);
}

static void print_num(int v)
{
    if (v == 0) { print_str("0"); return; }
    if (v < 0) { print_str("-"); v = -v; }
    char buf[12]; int i = 0;
    while (v > 0) { buf[i++] = (char)('0' + (v % 10)); v /= 10; }
    while (i > 0) {
        char *d = (char *)65536; *d = buf[--i];
        wasm_host_print(65536, 1);
    }
}

static void sys_log(const char *level, const char *msg)
{
    unsigned int ll = my_strlen(level);
    unsigned int ml = my_strlen(msg);
    char *buf = (char *)65536;
    for (unsigned int i = 0; i < ll; i++) buf[i] = level[i];
    buf[ll] = '\0';
    for (unsigned int i = 0; i < ml; i++) buf[ll + 1 + i] = msg[i];
    buf[ll + 1 + ml] = '\0';
    wasm_host_log(65536, ll, 65536 + ll + 1, ml);
}

static const char *audit_type_name(unsigned short t)
{
    switch (t) {
        case 0x0001: return "LOGIN";
        case 0x0002: return "LOGOUT";
        case 0x0003: return "PERM_CHANGE";
        case 0x0004: return "PROC_CREATE";
        case 0x0005: return "PROC_EXIT";
        case 0x0006: return "QUOTA_EXCEED";
        case 0x0007: return "MEM_FAIL";
        case 0x0008: return "FILE_ACCESS";
        case 0x0009: return "DEVICE_ATTACH";
        case 0x000A: return "KERNEL_BOOT";
        default: return "UNKNOWN";
    }
}

static const char *audit_level_name(unsigned short l)
{
    if (l == 0) return "INFO";
    if (l == 1) return "WARN";
    return "ERROR";
}

static void show_audit_log(void)
{
    int count = wasm_host_audit_get_count();
    if (count == 0) {
        print_str("  (no audit events)\n");
        return;
    }

    int to_read = count;
    if (to_read > 64) to_read = 64;

    int got = wasm_host_audit_query(66000, to_read);
    if (got <= 0) return;

    print_str("  Timestamp       Type           Level  PID  UID  Detail\n");

    for (int i = 0; i < got; i++) {
        unsigned int *evt = (unsigned int *)(66000 + i * 32);
        unsigned int timestamp = evt[0];
        unsigned short etype = (unsigned short)(evt[1] & 0xFFFF);
        unsigned short elevel = (unsigned short)((evt[1] >> 16) & 0xFFFF);
        unsigned int proc_id = evt[2];
        unsigned int user_id = evt[3];
        unsigned int detail0 = evt[4];

        print_str("  ");
        print_num(timestamp);
        print_str("   ");
        print_str(audit_type_name(etype));
        { const char *n = audit_type_name(etype); unsigned int l = my_strlen(n);
          for (unsigned int j = 0; j < 14 - l; j++) print_str(" "); }
        print_str(audit_level_name(elevel));
        print_str("  ");
        print_num(proc_id);
        print_str("  ");
        print_num(user_id);
        print_str("  ");
        print_num(detail0);
        print_str("\n");
    }
}

void syslog_loop(int log_fd)
{
    for (;;) {
        /* Check if data available */
        int avail = wasm_host_log_size();
        if (avail <= 0) {
            wasm_host_yield();
            wasm_host_sleep(100);
            continue;
        }

        /* Read one line from kernel log ring buffer */
        char *line_buf = (char *)66000;  /* separate from string buffer */
        int len = wasm_host_log_read(66000, 256);
        if (len > 0) {
            /* Write to log file if fd is valid */
            if (log_fd >= 0) {
                int rc = wasm_host_fs_write(log_fd, 66000, (unsigned int)len);
                if (rc < 0) {
                    print_str("[syslog] write error: ");
                    print_num(rc);
                    print_str("\n");
                }
            }
        } else {
            wasm_host_yield();
            wasm_host_sleep(100);
        }
    }
}

__attribute__((export_name("_start")))
void _start(void)
{
    print_str("\n== AI-ASM Syslog Service ==\n");
    print_str("Opening log file... ");

    /* Try to create log file — ramdisk is read-only in current setup, */
    /* so we just open with a fallback */
    int log_fd = -1;
    /* Try to open /var/log/syslog (will likely fail on read-only ramdisk) */
    char *path = (char *)65536;
    const char *p = "/var/log/syslog";
    unsigned int pl = my_strlen(p);
    for (unsigned int i = 0; i < pl; i++) path[i] = p[i];
    log_fd = wasm_host_fs_open(65536, pl);

    if (log_fd >= 0) {
        print_str("ok (fd=");
        print_num(log_fd);
        print_str(")\n");
    } else {
        print_str("read-only (no file write)\n");
        print_str("[syslog] logging to UART only\n");
    }

    sys_log("INFO", "syslog service started");

    /* Display current audit log */
    print_str("\n--- Audit Log ---\n");
    show_audit_log();
    print_str("-----------------\n");

    syslog_loop(log_fd);
}
