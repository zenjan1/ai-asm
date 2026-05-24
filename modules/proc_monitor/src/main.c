/*
 * aiasm-aarch64/modules/proc_monitor/src/main.c
 * Process monitoring service — detects crashed/exited processes and auto-restarts them.
 *
 * Runs as a persistent WASM module spawned by init.
 * Every ~1 second, iterates all WASM modules, checks for EXITED state,
 * and restarts them.
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

__attribute__((import_module("host"), import_name("proc_list_next")))
extern int proc_list_next(void);

__attribute__((import_module("host"), import_name("proc_get_status")))
extern int proc_get_status(int pid);

__attribute__((import_module("host"), import_name("proc_restart")))
extern int proc_restart(int pid);

__attribute__((import_module("host"), import_name("proc_kill")))
extern void proc_kill(int pid);

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
    if (v == 0) {
        print_str("0");
        return;
    }
    if (v < 0) {
        print_str("-");
        v = -v;
    }
    char buf[12];
    int i = 0;
    while (v > 0) {
        buf[i++] = (char)('0' + (v % 10));
        v /= 10;
    }
    while (i > 0) {
        char *d = (char *)65536;
        *d = buf[--i];
        wasm_host_print(65536, 1);
    }
}

static void mon_log(const char *level, const char *msg)
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

/* Module status codes (matches module_state_t in kernel) */
#define MOD_EXITED  4

void monitor_loop(void)
{
    for (;;) {
        int pid = proc_list_next();  /* returns -1 when done iterating */

        while (pid >= 0) {
            int status = proc_get_status(pid);
            if (status == MOD_EXITED) {
                print_str("[proc_monitor] restarting module pid=");
                print_num(pid);
                print_str("\n");

                int rc = proc_restart(pid);
                if (rc >= 0) {
                    mon_log("INFO", "process restarted");
                } else {
                    mon_log("ERROR", "restart failed");
                }
            }

            pid = proc_list_next();
        }

        /* Yield and sleep ~1 second before next scan */
        wasm_host_yield();
        wasm_host_sleep(1000);
    }
}

__attribute__((export_name("_start")))
void _start(void)
{
    print_str("\n== AI-ASM Process Monitor ==\n");
    print_str("Monitoring started.\n");
    mon_log("INFO", "proc_monitor started");

    monitor_loop();
}
