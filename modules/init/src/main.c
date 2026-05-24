/*
 * aiasm-aarch64/modules/init/src/main.c
 * Init module for AI-ASM AArch64 v0.5
 *
 * Prints welcome banner, spawns the launcher module, and exits.
 * The launcher presents the application menu and spawns apps on demand.
 */

__attribute__((import_module("host"), import_name("print")))
extern void wasm_host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("spawn")))
extern int wasm_host_spawn(unsigned int name_off, unsigned int name_len);

__attribute__((import_module("host"), import_name("exit")))
extern void wasm_host_exit(int code);

__attribute__((import_module("host"), import_name("log")))
extern void wasm_host_log(unsigned int level_off, unsigned int level_len,
                          unsigned int msg_off, unsigned int msg_len);

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

static int spawn_module(const char *name)
{
    unsigned int len = my_strlen(name);
    char *dst = (char *)65536;
    for (unsigned int i = 0; i < len; i++)
        dst[i] = name[i];
    return wasm_host_spawn(65536, len);
}

__attribute__((export_name("_start")))
void _start(void)
{
    print_str("\n== AI-ASM AArch64 v0.5 Init ==\n");
    print_str("Initializing system services...\n\n");

    init_log("INFO", "init module started");

    /* Start process monitor (system service) */
    print_str("Spawning proc_monitor... ");
    int monitor_id = spawn_module("proc_monitor");
    if (monitor_id >= 0) {
        print_str("ok\n");
    } else {
        print_str("FAILED\n");
    }

    /* Start syslog service */
    print_str("Spawning syslog... ");
    int syslog_id = spawn_module("syslog");
    if (syslog_id >= 0) {
        print_str("ok\n");
    } else {
        print_str("FAILED\n");
    }

    print_str("Spawning launcher... ");
    int launcher_id = spawn_module("launcher");
    if (launcher_id >= 0) {
        print_str("ok (id=");
        /* Print launcher_id as decimal */
        if (launcher_id == 0) {
            print_str("0");
        } else {
            char buf[12];
            int i = 0, v = launcher_id;
            while (v > 0) {
                buf[i++] = '0' + (char)(v % 10);
                v /= 10;
            }
            while (i > 0) {
                char c = buf[--i];
                char *d = (char *)65536;
                *d = c;
                wasm_host_print(65536, 1);
            }
        }
        print_str(")\n");
    } else {
        print_str("FAILED\n");
    }

    print_str("\nInit complete.\n");
    init_log("INFO", "init complete");

    wasm_host_exit(0);
}
