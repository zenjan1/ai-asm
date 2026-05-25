/*
 * aiasm-aarch64/modules/init/src/main.c
 * Init module for AI-ASM AArch64 v0.5
 *
 * System boot sequence:
 * 1. Spawn system services (proc_monitor, syslog)
 * 2. Wait for services to be running
 * 3. Spawn shell as the main interactive process
 * 4. If shell exits, respawn it
 */

__attribute__((import_module("host"), import_name("print")))
extern void wasm_host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("spawn")))
extern int wasm_host_spawn(unsigned int name_off, unsigned int name_len);

__attribute__((import_module("host"), import_name("set_argv")))
extern void wasm_host_set_argv(unsigned int buf_off, unsigned int buf_len);

__attribute__((import_module("host"), import_name("exit")))
extern void wasm_host_exit(int code);

__attribute__((import_module("host"), import_name("log")))
extern void wasm_host_log(unsigned int level_off, unsigned int level_len,
                          unsigned int msg_off, unsigned int msg_len);

__attribute__((import_module("host"), import_name("yield")))
extern void wasm_host_yield(void);

__attribute__((import_module("host"), import_name("proc_list_next")))
extern int wasm_host_proc_list_next(void);

__attribute__((import_module("host"), import_name("proc_get_status")))
extern int wasm_host_proc_get_status(int pid);

/* -------------------------------------------------------------------------- */
/* String and output helpers                                                  */
/* -------------------------------------------------------------------------- */

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

static void print_int(int v)
{
    char buf[12];
    int i = 0;
    if (v == 0) {
        buf[i++] = '0';
    } else {
        int n = v;
        if (n < 0) { print_str("-"); n = -n; }
        while (n > 0) {
            buf[i++] = '0' + (char)(n % 10);
            n /= 10;
        }
    }
    char *dst = (char *)65536;
    for (int j = 0; j < i; j++)
        dst[j] = buf[i - 1 - j];
    wasm_host_print(65536, (unsigned int)i);
}

static void init_log(const char *level, const char *msg)
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

/* -------------------------------------------------------------------------- */
/* Module spawning with argv                                                  */
/* -------------------------------------------------------------------------- */

static int spawn_module(const char *name)
{
    unsigned int len = my_strlen(name);
    char *dst = (char *)65536;
    for (unsigned int i = 0; i < len; i++)
        dst[i] = name[i];
    dst[len] = '\0';
    wasm_host_set_argv(65536, len);
    return wasm_host_spawn(65536, len);
}

/* -------------------------------------------------------------------------- */
/* Process waiting                                                          */
/* -------------------------------------------------------------------------- */

static void wait_for_running(int pid)
{
    /* Wait until process reaches MOD_RUNNING (3) */
    int iter = 0;
    while (iter < 10000) {
        int listed = wasm_host_proc_list_next();
        if (listed == pid) {
            int status = wasm_host_proc_get_status(pid);
            if (status == 3) return;  /* MOD_RUNNING */
        }
        if (listed < 0) wasm_host_proc_list_next();  /* reset iterator */
        wasm_host_yield();
        iter++;
    }
}

static void wait_for_exit(int pid)
{
    /* Wait until process exits (status != MOD_RUNNING) */
    int iter = 0;
    while (iter < 100000) {
        int listed = wasm_host_proc_list_next();
        if (listed == pid) {
            int status = wasm_host_proc_get_status(pid);
            if (status != 3) return;  /* exited */
        }
        if (listed < 0) wasm_host_proc_list_next();  /* reset iterator */
        wasm_host_yield();
        iter++;
    }
}

/* -------------------------------------------------------------------------- */
/* Init entry point                                                           */
/* -------------------------------------------------------------------------- */

__attribute__((export_name("_start")))
void _start(void)
{
    print_str("\n=== AI-ASM AArch64 System Boot ===\n");
    print_str("Starting system services...\n\n");

    /* Step 1: Start process monitor */
    print_str("  [1/2] Starting proc_monitor... ");
    int monitor_pid = spawn_module("proc_monitor");
    if (monitor_pid >= 0) {
        wait_for_running(monitor_pid);
        print_str("ok (pid=");
        print_int(monitor_pid);
        print_str(")\n");
    } else {
        print_str("FAILED\n");
    }

    /* Step 2: Start syslog */
    print_str("  [2/2] Starting syslog... ");
    int syslog_pid = spawn_module("syslog");
    if (syslog_pid >= 0) {
        wait_for_running(syslog_pid);
        print_str("ok (pid=");
        print_int(syslog_pid);
        print_str(")\n");
    } else {
        print_str("FAILED\n");
    }

    /* Main loop: spawn shell and respawn if it exits */
    print_str("\nSystem services started.\n");
    init_log("INFO", "system boot complete, starting shell");

    for (;;) {
        print_str("--- Starting shell ---\n\n");
        int shell_pid = spawn_module("shell");
        if (shell_pid >= 0) {
            /* Wait for shell to exit */
            wait_for_exit(shell_pid);
            print_str("\n--- Shell exited, restarting in 3 seconds ---\n\n");
        } else {
            print_str("Shell spawn failed, retrying...\n\n");
        }
        /* Delay before respawn */
        for (int i = 0; i < 3000; i++) wasm_host_yield();
    }

    wasm_host_exit(0);
}
