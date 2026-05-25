/*
 * aiasm-aarch64/modules/shell/src/main.c
 * Wasm Shell module for AI-ASM AArch64 kernel v0.5
 *
 * Interactive command loop: reads characters via host_getc,
 * echoes input, handles backspace, executes commands on newline.
 * Supports: help, tick, echo, alloc, log, clear, exit, ls, cat
 */

/* -------------------------------------------------------------------------- */
/* WASM imports                                                               */
/* -------------------------------------------------------------------------- */

__attribute__((import_module("host"), import_name("print")))
extern void wasm_host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("get_tick")))
extern unsigned long long wasm_host_get_tick(void);

__attribute__((import_module("host"), import_name("exit")))
extern void wasm_host_exit(int code);

__attribute__((import_module("host"), import_name("alloc")))
extern void *wasm_host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("free")))
extern void wasm_host_free(void *ptr);

__attribute__((import_module("host"), import_name("log")))
extern void wasm_host_log(unsigned int level_off, unsigned int level_len,
                          unsigned int msg_off, unsigned int msg_len);

__attribute__((import_module("host"), import_name("getc")))
extern int wasm_host_getc(void);

__attribute__((import_module("host"), import_name("fs_open")))
extern int wasm_host_fs_open(unsigned int path_off, unsigned int path_len);

__attribute__((import_module("host"), import_name("fs_read")))
extern int wasm_host_fs_read(int fd, unsigned int buf_off, unsigned int len);

__attribute__((import_module("host"), import_name("fs_close")))
extern void wasm_host_fs_close(int fd);

__attribute__((import_module("host"), import_name("fs_list")))
extern int wasm_host_fs_list(unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("spawn")))
extern int wasm_host_spawn(unsigned int name_off, unsigned int name_len);

__attribute__((import_module("host"), import_name("spawn_redirect")))
extern int wasm_host_spawn_redirect(unsigned int name_off, unsigned int name_len,
                                     int stdin_fd, int stdout_fd);

__attribute__((import_module("host"), import_name("pipe_create")))
extern int wasm_host_pipe_create(unsigned int rfd_off, unsigned int wfd_off);

__attribute__((import_module("host"), import_name("pipe_close")))
extern void wasm_host_pipe_close(int fd);

__attribute__((import_module("host"), import_name("proc_list_next")))
extern int wasm_host_proc_list_next(void);

__attribute__((import_module("host"), import_name("proc_get_status")))
extern int wasm_host_proc_get_status(int pid);

/* -------------------------------------------------------------------------- */
/* WASM memory allocator (bump, uses host_alloc)                              */
/* -------------------------------------------------------------------------- */

static unsigned int heap_pos = 65536;

static unsigned int alloc(unsigned int n)
{
    unsigned int ptr = heap_pos;
    heap_pos += n;
    heap_pos = (heap_pos + 15) & ~15u;
    return ptr;
}

/* -------------------------------------------------------------------------- */
/* String helpers                                                             */
/* -------------------------------------------------------------------------- */

static unsigned int my_strlen(const char *s)
{
    const char *p = s;
    while (*p) p++;
    return (unsigned int)(p - s);
}

static int my_strcmp(const char *a, const char *b)
{
    while (*a && *b) {
        if (*a != *b) return 1;
        a++; b++;
    }
    return (*a != *b) ? 1 : 0;
}

static void print_str(const char *s)
{
    unsigned int len = my_strlen(s);
    unsigned int offset = alloc(len);
    char *dst = (char *)(offset);
    for (unsigned int i = 0; i < len; i++)
        dst[i] = s[i];
    wasm_host_print(offset, len);
}

static void print_u64(unsigned long long v)
{
    char buf[20];
    int i = 0;
    if (v == 0) {
        buf[i++] = '0';
    } else {
        while (v > 0) {
            buf[i++] = '0' + (char)(v % 10);
            v /= 10;
        }
    }
    unsigned int offset = alloc((unsigned int)i);
    char *dst = (char *)(offset);
    for (int j = 0; j < i; j++)
        dst[j] = buf[i - 1 - j];
    wasm_host_print(offset, (unsigned int)i);
}

static void print_char(char c)
{
    unsigned int offset = alloc(1);
    ((char *)offset)[0] = c;
    wasm_host_print(offset, 1);
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
    unsigned int offset = alloc((unsigned int)i);
    char *dst = (char *)(offset);
    for (int j = 0; j < i; j++)
        dst[j] = buf[i - 1 - j];
    wasm_host_print(offset, (unsigned int)i);
}

/* -------------------------------------------------------------------------- */
/* Logging                                                                    */
/* -------------------------------------------------------------------------- */

static void shell_log(const char *level, const char *msg)
{
    unsigned int ll = my_strlen(level);
    unsigned int ml = my_strlen(msg);
    unsigned int loff = alloc(ll);
    unsigned int moff = alloc(ml);
    char *ld = (char *)(loff);
    char *md = (char *)(moff);
    for (unsigned int i = 0; i < ll; i++) ld[i] = level[i];
    for (unsigned int i = 0; i < ml; i++) md[i] = msg[i];
    wasm_host_log(loff, ll, moff, ml);
}

/* -------------------------------------------------------------------------- */
/* Memory helpers                                                             */
/* -------------------------------------------------------------------------- */

static void *shell_malloc(unsigned int n)
{
    return wasm_host_alloc(n, 16);
}

static void shell_free(void *p)
{
    if (p) wasm_host_free(p);
}

/* -------------------------------------------------------------------------- */
/* Input: blocking read one character from UART                               */
/* -------------------------------------------------------------------------- */

static char read_char(void)
{
    return (char)wasm_host_getc();
}

/* -------------------------------------------------------------------------- */
/* Command execution                                                          */
/* -------------------------------------------------------------------------- */

static void cmd_help(void)
{
    print_str("Commands:\n");
    print_str("  help    - show this help\n");
    print_str("  tick    - print current tick counter\n");
    print_str("  echo X  - echo text back\n");
    print_str("  alloc   - test kernel allocator\n");
    print_str("  log     - test kernel logging\n");
    print_str("  clear   - clear screen\n");
    print_str("  ls      - list files on RAM disk\n");
    print_str("  cat F   - print file content\n");
    print_str("  exit    - exit shell\n");
    print_str("  Pipes:  - cmd1 | cmd2 | cmd3\n");
}

static void cmd_tick(void)
{
    print_str("tick: ");
    print_u64(wasm_host_get_tick());
    print_str("\n");
}

static void cmd_echo(const char *arg)
{
    if (*arg) {
        print_str(arg);
    }
    print_str("\n");
}

static void cmd_alloc(void)
{
    char *buf = (char *)shell_malloc(64);
    if (buf) {
        for (int i = 0; i < 32; i++)
            buf[i] = 'A' + (char)(i % 26);
        buf[32] = '\0';
        print_str("alloc 64 bytes: ");
        print_str(buf);
        print_str("\n");
        shell_free(buf);
        print_str("free ok\n");
    } else {
        print_str("alloc failed!\n");
    }
}

static void cmd_log(void)
{
    shell_log("INFO", "user requested log test");
    print_str("log sent\n");
}

static void cmd_clear(void)
{
    print_str("\033[2J\033[H");
}

static void cmd_ls(void)
{
    /* Allocate a 2KB buffer for file listing */
    unsigned int buf_off = alloc(2048);
    int total = wasm_host_fs_list(buf_off, 2048);
    if (total <= 0) {
        print_str("(empty)\n");
        return;
    }
    /* Print null-separated names as line-separated */
    unsigned int i = 0;
    char *names = (char *)buf_off;
    while (i < (unsigned int)total) {
        if (names[i] == '\0') {
            print_str("\n");
        } else {
            print_char(names[i]);
        }
        i++;
    }
    print_str("\n");
}

static void cmd_cat(const char *filename)
{
    if (!*filename) {
        print_str("cat: no filename\n");
        return;
    }
    unsigned int flen = my_strlen(filename);
    unsigned int path_off = alloc(flen);
    char *pd = (char *)path_off;
    for (unsigned int i = 0; i < flen; i++) pd[i] = filename[i];

    int fd = wasm_host_fs_open(path_off, flen);
    if (fd < 0) {
        print_str("cat: no such file: ");
        print_str(filename);
        print_str("\n");
        return;
    }

    /* Read in chunks */
    unsigned int buf_off = alloc(512);
    for (;;) {
        int n = wasm_host_fs_read(fd, buf_off, 512);
        if (n <= 0) break;
        wasm_host_print(buf_off, (unsigned int)n);
    }
    wasm_host_fs_close(fd);
    print_str("\n");
}

/* -------------------------------------------------------------------------- */
/* Pipe command execution (v15.0)                                             */
/* -------------------------------------------------------------------------- */

#define MAX_PIPE_STAGES 8
#define PIPE_BUF_SIZE 8

/* Forward declarations */
static void shell_execute_single(const char *cmd);
static void wait_for_pid(int pid);

static void shell_execute_pipe(const char *pipeline)
{
    /* Parse stages separated by '|' */
    const char *stages[MAX_PIPE_STAGES];
    unsigned int stage_lens[MAX_PIPE_STAGES];
    int num_stages = 0;

    const char *p = pipeline;
    const char *start = p;
    while (*p && num_stages < MAX_PIPE_STAGES) {
        if (*p == '|') {
            stages[num_stages] = start;
            stage_lens[num_stages] = (unsigned int)(p - start);
            num_stages++;
            p++;
            /* Skip spaces after '|' */
            while (*p == ' ') p++;
            start = p;
        } else {
            p++;
        }
    }
    /* Last stage */
    if (start < p) {
        stages[num_stages] = start;
        stage_lens[num_stages] = (unsigned int)(p - start);
        num_stages++;
    }

    if (num_stages <= 1) {
        /* No pipe, execute single command */
        shell_execute_single(pipeline);
        return;
    }

    /* Create pipes: need num_stages - 1 pipes */
    int pipes[MAX_PIPE_STAGES - 1][2];  /* [pipe_index][0]=read, [1]=write */
    unsigned int i;

    for (i = 0; i < (unsigned int)(num_stages - 1); i++) {
        /* Allocate 8 bytes in WASM memory for rfd and wfd return values */
        unsigned int buf = alloc(PIPE_BUF_SIZE);
        int result = wasm_host_pipe_create(buf, buf + 4);
        if (result != 0) {
            print_str("pipe: create failed\n");
            /* Close any pipes already created */
            while (i > 0) {
                i--;
                wasm_host_pipe_close(pipes[i][0]);
                wasm_host_pipe_close(pipes[i][1]);
            }
            return;
        }
        pipes[i][0] = *(int *)(buf);
        pipes[i][1] = *(int *)(buf + 4);
    }

    /* Spawn each stage with appropriate fd redirection */
    int pids[MAX_PIPE_STAGES];
    for (i = 0; i < (unsigned int)num_stages; i++) {
        int stdin_fd = -1;
        int stdout_fd = -1;

        if (i > 0) {
            stdin_fd = pipes[i - 1][0];  /* read from previous pipe */
        }
        if (i < (unsigned int)(num_stages - 1)) {
            stdout_fd = pipes[i][1];  /* write to next pipe */
        }

        /* Copy stage command to WASM memory */
        unsigned int name_off = alloc(stage_lens[i] + 1);
        char *dst = (char *)(name_off);
        unsigned int j;
        for (j = 0; j < stage_lens[i]; j++) dst[j] = stages[i][j];
        dst[j] = '\0';

        if (stdin_fd != -1 || stdout_fd != -1) {
            pids[i] = wasm_host_spawn_redirect(name_off, stage_lens[i], stdin_fd, stdout_fd);
        } else {
            pids[i] = wasm_host_spawn(name_off, stage_lens[i]);
        }

        if (pids[i] < 0) {
            print_str("pipe: spawn failed for stage ");
            print_int((int)i);
            print_str("\n");
        }
    }

    /* Close all pipe fds in parent (shell) */
    for (i = 0; i < (unsigned int)(num_stages - 1); i++) {
        wasm_host_pipe_close(pipes[i][0]);
        wasm_host_pipe_close(pipes[i][1]);
    }

    /* Wait for all spawned processes to exit */
    for (i = 0; i < (unsigned int)num_stages; i++) {
        if (pids[i] >= 0) {
            wait_for_pid(pids[i]);
        }
    }
}

static void shell_execute_single(const char *cmd)
{
    if (my_strcmp(cmd, "help") == 0) {
        cmd_help();
    } else if (my_strcmp(cmd, "tick") == 0) {
        cmd_tick();
    } else if (my_strcmp(cmd, "alloc") == 0) {
        cmd_alloc();
    } else if (my_strcmp(cmd, "log") == 0) {
        cmd_log();
    } else if (my_strcmp(cmd, "clear") == 0) {
        cmd_clear();
    } else if (my_strcmp(cmd, "ls") == 0) {
        cmd_ls();
    } else if (cmd[0] == 'c' && cmd[1] == 'a' &&
               cmd[2] == 't' && cmd[3] == ' ') {
        cmd_cat(cmd + 4);
    } else if (my_strcmp(cmd, "exit") == 0) {
        shell_log("OP", "shell exiting");
        wasm_host_exit(0);
    } else if (cmd[0] == 'e' && cmd[1] == 'c' &&
               cmd[2] == 'h' && cmd[3] == 'o' &&
               cmd[4] == ' ') {
        cmd_echo(cmd + 5);
    } else {
        print_str("unknown: ");
        print_str(cmd);
        print_str("\n");
    }
}

static void wait_for_pid(int pid)
{
    /* Poll proc_list_next and proc_get_status until pid exits */
    int found = 0;
    int iter;
    for (iter = 0; iter < 100000; iter++) {
        int listed_pid = wasm_host_proc_list_next();
        if (listed_pid == pid) {
            int status = wasm_host_proc_get_status(pid);
            if (status != 3) {  /* not MOD_RUNNING (0=FREE,1=LOADING,2=READY,3=RUNNING,4=EXITED) */
                found = 1;
                break;
            }
        }
        if (listed_pid < 0) {
            /* End of list — process not found or still running, reset scan */
            /* If we've scanned the full list and didn't find our pid exiting, keep polling */
        }
    }
    (void)found;
}

/* -------------------------------------------------------------------------- */
/* Shell entry point                                                          */
/* -------------------------------------------------------------------------- */

#define MAX_LINE 128

__attribute__((export_name("_start")))
void shell_entry(void)
{
    print_str("\n== AI-ASM AArch64 v0.5 Wasm Shell ==\n");
    print_str("Type 'help' for commands.\n\n");

    print_str("tick: ");
    print_u64(wasm_host_get_tick());
    print_str("\n\n");

    shell_log("INFO", "shell started");

    /* Demonstrate alloc/free at startup */
    char *buf = (char *)shell_malloc(32);
    if (buf) {
        for (int i = 0; i < 16; i++)
            buf[i] = 'A' + (char)(i % 26);
        buf[16] = '\0';
        print_str("alloc test: ");
        print_str(buf);
        print_str("\n\n");
        shell_free(buf);
    }

    /* Main interactive loop */
    char line[MAX_LINE];
    unsigned int len = 0;

    for (;;) {
        print_str("wasm> ");
        len = 0;

        for (;;) {
            char c = read_char();

            if (c == '\r' || c == '\n') {
                /* End of line — execute */
                print_str("\n");
                line[len] = '\0';

                if (len > 0) {
                    /* Check for pipe syntax */
                    int has_pipe = 0;
                    unsigned int ki;
                    for (ki = 0; ki < len; ki++) {
                        if (line[ki] == '|') {
                            has_pipe = 1;
                            break;
                        }
                    }

                    if (has_pipe) {
                        shell_execute_pipe(line);
                    } else {
                        shell_execute_single(line);
                    }
                }
                /* Reset for next command */
                len = 0;
                break;
            } else if (c == '\b' || c == 0x7f) {
                /* Backspace */
                if (len > 0) {
                    len--;
                    print_str("\x1b[D\x1b[K");  /* cursor left, erase to end */
                }
            } else if (c >= 0x20 && c < 0x7f) {
                /* Printable character */
                if (len < MAX_LINE - 1) {
                    line[len] = c;
                    len++;
                    print_char(c);
                }
            }
            /* Ignore other control characters */
        }
    }
}
