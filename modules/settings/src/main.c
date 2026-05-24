/*
 * aiasm-aarch64/modules/settings/src/main.c
 * System settings — display and modify system configuration.
 * Terminal-based settings panel.
 */

__attribute__((import_module("host"), import_name("print")))
extern void wasm_host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("log")))
extern void wasm_host_log(unsigned int level_off, unsigned int level_len,
                          unsigned int msg_off, unsigned int msg_len);

__attribute__((import_module("host"), import_name("getc")))
extern int wasm_host_getc(void);

__attribute__((import_module("host"), import_name("yield")))
extern void wasm_host_yield(void);

__attribute__((import_module("host"), import_name("sleep")))
extern void wasm_host_sleep(unsigned int ms);

__attribute__((import_module("host"), import_name("proc_list_next")))
extern int proc_list_next(void);

__attribute__((import_module("host"), import_name("proc_get_status")))
extern int proc_get_status(int pid);

__attribute__((import_module("host"), import_name("quota_get")))
extern int quota_get(int pid, unsigned int buf_off);

__attribute__((import_module("host"), import_name("perm_get_level")))
extern int perm_get_level(void);

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

static void settings_log(const char *msg)
{
    unsigned int ll = my_strlen("INFO");
    unsigned int ml = my_strlen(msg);
    char *buf = (char *)65536;
    buf[0] = 'I'; buf[1] = 'N'; buf[2] = 'F'; buf[3] = 'O'; buf[4] = '\0';
    for (unsigned int i = 0; i < ml; i++) buf[5 + i] = msg[i];
    buf[5 + ml] = '\0';
    wasm_host_log(65536, 4, 65536 + 5, ml);
}

static void show_system_info(void)
{
    print_str("\n--- System Information ---\n");
    print_str("  OS:     AI-ASM AArch64 v4.0\n");
    print_str("  Arch:   AArch64 (ARMv8-A)\n");
    print_str("  Kernel: Pure assembly, 6.87MB\n");
    print_str("  WASM:   Wasm3 runtime, multi-module\n");
    print_str("  GUI:    Framebuffer + double buffering\n");
    print_str("  Net:    VirtIO-Net + TCP/IP stack\n");
    print_str("  FS:     VirtIO-Block + FAT32 + RAM disk\n");
    print_str("  IPC:    Pipes, message queues, signals\n");
    print_str("--------------------------\n");
}

static void show_processes(void)
{
    print_str("\n--- Running Processes ---\n");
    int pid = proc_list_next();
    int count = 0;
    while (pid >= 0) {
        int status = proc_get_status(pid);
        print_str("  PID ");
        /* print pid */
        { int v = pid; char tmp[8]; int ti = 0;
          if (v == 0) tmp[ti++] = '0';
          else { while (v > 0) { tmp[ti++] = (char)('0' + (v % 10)); v /= 10; } }
          char *d = (char *)65536;
          for (int j = ti - 1; j >= 0; j--) { *d = tmp[j]; wasm_host_print(65536, 1); } }
        print_str(" status=");
        /* print status */
        { int v = status; char tmp[8]; int ti = 0;
          if (v == 0) tmp[ti++] = '0';
          else { while (v > 0) { tmp[ti++] = (char)('0' + (v % 10)); v /= 10; } }
          char *d = (char *)65536;
          for (int j = ti - 1; j >= 0; j--) { *d = tmp[j]; wasm_host_print(65536, 1); } }
        print_str("\n");
        count++;
        pid = proc_list_next();
    }
    if (count == 0) print_str("  (no processes found)\n");
    print_str("---------------------------\n");
}

static void print_u32(unsigned int v)
{
    char tmp[12]; int ti = 0;
    if (v == 0) tmp[ti++] = '0';
    else { while (v > 0) { tmp[ti++] = (char)('0' + (v % 10)); v /= 10; } }
    for (int j = ti - 1; j >= 0; j--) { char c = tmp[j]; wasm_host_print(65536, 1); ((char*)65536)[0] = c; }
}

static void show_quotas(void)
{
    unsigned int buf[6];
    print_str("\n--- Resource Quotas ---\n");
    print_str("  PID  CPU Limit    CPU Used   Mem Limit    Mem Used   FD\n");

    int pid = proc_list_next();
    while (pid >= 0) {
        print_str("  ");
        { int v = pid; char tmp[8]; int ti = 0;
          if (v == 0) tmp[ti++] = '0';
          else { while (v > 0) { tmp[ti++] = (char)('0' + (v % 10)); v /= 10; } }
          char *d = (char *)65536;
          for (int j = ti - 1; j >= 0; j--) { d[0] = tmp[j]; wasm_host_print(65536, 1); } }

        quota_get(pid, (unsigned int)buf);

        /* CPU limit */
        print_str("  ");
        if (buf[0] == 0) { print_str("unlimited"); }
        else { print_u32(buf[1]); }

        /* CPU used */
        print_str("  ");
        print_u32(buf[2]);

        /* Mem limit */
        print_str("  ");
        if (buf[3] == 0) { print_str("unlimited"); }
        else { print_u32(buf[4]); }

        /* Mem used */
        print_str("  ");
        print_u32(buf[5]);

        /* FD used/limit */
        print_str("  ");
        /* skip for now, buf has fd at offset 16 */

        print_str("\n");
        pid = proc_list_next();
    }
    print_str("-------------------------\n");
}

static void show_menu(void)
{
    print_str("\n=== System Settings ===\n");
    print_str("  i - System Info\n");
    print_str("  p - Process List\n");
    print_str("  q - Resource Quotas\n");
    print_str("  l - Change log level\n");
    print_str("  x - Quit\n");
    print_str("Enter choice: ");
}

static void change_log_level(void)
{
    print_str("\nLog levels: 0=DEBUG, 1=INFO, 2=WARN, 3=ERROR\n");
    print_str("Current: 1 (INFO) - change not yet supported\n");
    settings_log("log level change attempted (not implemented)");
}

__attribute__((export_name("_start")))
void _start(void)
{
    print_str("\n=== AI-ASM System Settings ===\n");
    settings_log("settings module started");

    show_menu();

    for (;;) {
        int ch = wasm_host_getc();
        if (ch > 0) {
            char c = (char)ch;
            switch (c) {
                case 'i': case 'I':
                    show_system_info();
                    break;
                case 'p': case 'P':
                    show_processes();
                    break;
                case 'q': case 'Q':
                    show_quotas();
                    break;
                case 'l': case 'L':
                    change_log_level();
                    break;
                case 'x': case 'X':
                    print_str("Exiting settings.\n");
                    settings_log("settings module exited");
                    return;
                case '\n': case '\r':
                    break;
                default:
                    print_str("Unknown option. ");
                    break;
            }
            show_menu();
        }
        wasm_host_yield();
        wasm_host_sleep(50);
    }
}
