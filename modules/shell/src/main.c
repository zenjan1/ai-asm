/*
 * aiasm-aarch64/modules/shell/src/main.c
 * Wasm Shell module for AI-ASM AArch64 kernel v0.5
 *
 * Interactive command loop: reads characters via host_getc,
 * echoes input, handles backspace, executes commands on newline.
 * Supports: help, tick, echo, alloc, log, clear, exit, ls, cat
 */

#define MAX_LINE 128

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

__attribute__((import_module("host"), import_name("pipe_read")))
extern int wasm_host_pipe_read(int fd, unsigned int buf_off, unsigned int len);

__attribute__((import_module("host"), import_name("proc_list_next")))
extern int wasm_host_proc_list_next(void);

__attribute__((import_module("host"), import_name("proc_get_status")))
extern int wasm_host_proc_get_status(int pid);

__attribute__((import_module("host"), import_name("yield")))
extern void wasm_host_yield(void);

__attribute__((import_module("host"), import_name("set_argv")))
extern void wasm_host_set_argv(unsigned int buf_off, unsigned int buf_len);

__attribute__((import_module("host"), import_name("fs_create")))
extern int wasm_host_fs_create(unsigned int path_off, unsigned int path_len);

__attribute__((import_module("host"), import_name("fs_write")))
extern int wasm_host_fs_write(int fd, unsigned int buf_off, unsigned int len);

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
/* File redirection parsing (v22.0)                                           */
/* -------------------------------------------------------------------------- */

#define REDIR_NONE      0
#define REDIR_OUT       1   /* > */
#define REDIR_OUT_APP   2   /* >> */
#define REDIR_IN        3   /* < */

typedef struct {
    int mode;               /* REDIR_NONE / REDIR_OUT / REDIR_OUT_APP / REDIR_IN */
    char filename[64];
} redirect_t;

/* Parse redirection from command line.
 * Copies clean command (without redirect part) to cmd_buf.
 * Sets redirect mode and filename in redir. */
static void parse_redirect(const char *line, char *cmd_buf, redirect_t *redir)
{
    redir->mode = REDIR_NONE;
    redir->filename[0] = '\0';

    unsigned int len = my_strlen(line);

    /* Scan for redirect operators */
    unsigned int i = 0;
    while (i < len) {
        if (line[i] == '>') {
            /* Check for >> (append) */
            unsigned int op_start = i;
            if (i + 1 < len && line[i + 1] == '>') {
                redir->mode = REDIR_OUT_APP;
                i += 2;
            } else {
                redir->mode = REDIR_OUT;
                i += 1;
            }
            /* Skip spaces */
            while (i < len && line[i] == ' ') i++;
            /* Copy filename */
            unsigned int fi = 0;
            while (i < len && fi < 63) {
                redir->filename[fi++] = line[i++];
            }
            redir->filename[fi] = '\0';
            /* Copy command part (before redirect) */
            unsigned int ci = 0;
            while (ci < op_start && line[ci] != ' ') ci++;
            /* Trim trailing spaces */
            while (ci > 0 && line[ci - 1] == ' ') ci--;
            for (unsigned int j = 0; j < ci; j++) cmd_buf[j] = line[j];
            cmd_buf[ci] = '\0';
            return;
        } else if (line[i] == '<') {
            redir->mode = REDIR_IN;
            i += 1;
            unsigned int op_start = i;
            while (i < len && line[i] == ' ') i++;
            unsigned int fi = 0;
            while (i < len && fi < 63) {
                redir->filename[fi++] = line[i++];
            }
            redir->filename[fi] = '\0';
            unsigned int ci = 0;
            while (ci < op_start && line[ci] != ' ') ci++;
            while (ci > 0 && line[ci - 1] == ' ') ci--;
            for (unsigned int j = 0; j < ci; j++) cmd_buf[j] = line[j];
            cmd_buf[ci] = '\0';
            return;
        }
        i++;
    }

    /* No redirect found — copy as-is */
    unsigned int j;
    for (j = 0; j < len; j++) cmd_buf[j] = line[j];
    cmd_buf[j] = '\0';
}

/* Write a buffer to a file (used for > and >> redirect) */
static int write_to_file(const char *filename, int append,
                         const char *buf, unsigned int len)
{
    unsigned int flen = my_strlen(filename);
    unsigned int path_off = alloc(flen);
    char *pd = (char *)path_off;
    for (unsigned int i = 0; i < flen; i++) pd[i] = filename[i];

    int fd;
    if (append) {
        fd = wasm_host_fs_open(path_off, flen);
    } else {
        fd = wasm_host_fs_create(path_off, flen);
    }
    if (fd < 0) {
        print_str("shell: cannot open '");
        print_str(filename);
        print_str("'\n");
        return -1;
    }

    /* Write data to file */
    unsigned int data_off = alloc(len);
    char *dd = (char *)data_off;
    for (unsigned int i = 0; i < len; i++) dd[i] = buf[i];

    int rc = wasm_host_fs_write(fd, data_off, len);
    wasm_host_fs_close(fd);

    if (rc < 0) {
        print_str("shell: write to '");
        print_str(filename);
        print_str("' failed\n");
        return -1;
    }
    return 0;
}

/* Read entire file into WASM memory, returns pointer and sets len_out.
 * Returns 0 on failure. */
static char *read_file(const char *filename, unsigned int *len_out)
{
    unsigned int flen = my_strlen(filename);
    unsigned int path_off = alloc(flen);
    char *pd = (char *)path_off;
    for (unsigned int i = 0; i < flen; i++) pd[i] = filename[i];

    int fd = wasm_host_fs_open(path_off, flen);
    if (fd < 0) {
        print_str("shell: cannot open '");
        print_str(filename);
        print_str("'\n");
        return 0;
    }

    /* Allocate a 4KB buffer */
    unsigned int buf_off = alloc(4096);
    int total = 0;
    for (;;) {
        int n = wasm_host_fs_read(fd, buf_off + (unsigned int)total,
                                   4096 - (unsigned int)total);
        if (n <= 0) break;
        total += n;
        if ((unsigned int)total >= 4096) break;
    }
    wasm_host_fs_close(fd);

    *len_out = (unsigned int)total;
    return (char *)buf_off;
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
    print_str("  grep P  - search stdin for pattern P\n");
    print_str("  Pipes:  - cmd1 | cmd2 | cmd3\n");
    print_str("  Redirect:\n");
    print_str("  echo hi > file.txt   (write)\n");
    print_str("  echo hi >> file.txt  (append)\n");
    print_str("  cat < file.txt       (read stdin)\n");
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

/* Output buffer for redirect mode */
#define MAX_REDIRECT_BUF 2048
static char redirect_buf[MAX_REDIRECT_BUF];
static unsigned int redirect_buf_len = 0;

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

    /* Check if last stage has output redirect (> or >>) */
    char last_stage_buf[MAX_LINE];
    redirect_t last_redir;
    parse_redirect(stages[num_stages - 1], last_stage_buf, &last_redir);
    int has_output_redirect = (last_redir.mode == REDIR_OUT ||
                               last_redir.mode == REDIR_OUT_APP);

    /* Create pipes: need num_stages - 1 pipes between stages */
    int pipes[MAX_PIPE_STAGES - 1][2];
    unsigned int i;

    for (i = 0; i < (unsigned int)(num_stages - 1); i++) {
        unsigned int buf = alloc(PIPE_BUF_SIZE);
        int result = wasm_host_pipe_create(buf, buf + 4);
        if (result != 0) {
            print_str("pipe: create failed\n");
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

    /* If output redirect, create an extra pipe for last stage's stdout */
    int redirect_pipe_rfd = -1;
    int redirect_pipe_wfd = -1;
    if (has_output_redirect) {
        unsigned int buf = alloc(PIPE_BUF_SIZE);
        int result = wasm_host_pipe_create(buf, buf + 4);
        if (result == 0) {
            redirect_pipe_rfd = *(int *)(buf);
            redirect_pipe_wfd = *(int *)(buf + 4);
        }
    }

    /* Spawn each stage with appropriate fd redirection */
    int pids[MAX_PIPE_STAGES];
    for (i = 0; i < (unsigned int)num_stages; i++) {
        int stdin_fd = -1;
        int stdout_fd = -1;

        if (i > 0) {
            stdin_fd = pipes[i - 1][0];
        }
        /* Last stage: if output redirect, stdout goes to redirect pipe */
        if (i < (unsigned int)(num_stages - 1)) {
            stdout_fd = pipes[i][1];
        } else if (has_output_redirect) {
            stdout_fd = redirect_pipe_wfd;
        }

        /* Extract module name (first word) from stage command */
        const char *stage = (i == (unsigned int)(num_stages - 1)) ? last_stage_buf : stages[i];
        unsigned int slen = (i == (unsigned int)(num_stages - 1)) ?
                            my_strlen(last_stage_buf) : stage_lens[i];

        unsigned int mod_name_len = 0;
        while (mod_name_len < slen && stage[mod_name_len] != ' ') mod_name_len++;

        /* Copy module name */
        unsigned int mod_off = alloc(mod_name_len + 1);
        char *mod_dst = (char *)(mod_off);
        unsigned int k;
        for (k = 0; k < mod_name_len; k++) mod_dst[k] = stage[k];
        mod_dst[mod_name_len] = '\0';

        /* Copy full command for set_argv */
        unsigned int cmd_off = alloc(slen + 1);
        char *cmd_dst = (char *)(cmd_off);
        for (k = 0; k < slen; k++) cmd_dst[k] = stage[k];
        cmd_dst[slen] = '\0';

        wasm_host_set_argv(cmd_off, slen);

        if (stdin_fd != -1 || stdout_fd != -1) {
            pids[i] = wasm_host_spawn_redirect(mod_off, mod_name_len, stdin_fd, stdout_fd);
        } else {
            pids[i] = wasm_host_spawn(mod_off, mod_name_len);
        }

        if (pids[i] < 0) {
            print_str("pipe: spawn failed for stage ");
            print_int((int)i);
            print_str("\n");
        }
    }

    /* Close all pipe fds in parent */
    for (i = 0; i < (unsigned int)(num_stages - 1); i++) {
        wasm_host_pipe_close(pipes[i][0]);
        wasm_host_pipe_close(pipes[i][1]);
    }

    /* Close write end of redirect pipe in parent */
    if (redirect_pipe_wfd >= 0) {
        wasm_host_pipe_close(redirect_pipe_wfd);
    }

    /* Wait for all spawned processes */
    for (i = 0; i < (unsigned int)num_stages; i++) {
        if (pids[i] >= 0) {
            wait_for_pid(pids[i]);
        }
    }

    /* If output redirect, read from redirect pipe and write to file */
    if (has_output_redirect && redirect_pipe_rfd >= 0) {
        redirect_buf_len = 0;

        /* Read data from redirect pipe */
        unsigned int rbuf_off = alloc(512);
        for (;;) {
            int n = wasm_host_pipe_read(redirect_pipe_rfd, rbuf_off, 512);
            if (n <= 0) break;
            if (redirect_buf_len + (unsigned int)n < MAX_REDIRECT_BUF) {
                char *src = (char *)rbuf_off;
                for (int j = 0; j < n; j++)
                    redirect_buf[redirect_buf_len + (unsigned int)j] = src[j];
                redirect_buf_len += (unsigned int)n;
            }
            wasm_host_yield();
        }

        wasm_host_pipe_close(redirect_pipe_rfd);

        /* Write collected output to file */
        write_to_file(last_redir.filename,
                      last_redir.mode == REDIR_OUT_APP,
                      redirect_buf, redirect_buf_len);
    }
}

static void redirect_print(const char *s)
{
    unsigned int len = my_strlen(s);
    if (redirect_buf_len + len < MAX_REDIRECT_BUF) {
        for (unsigned int i = 0; i < len; i++)
            redirect_buf[redirect_buf_len + i] = s[i];
        redirect_buf_len += len;
    }
}

static void redirect_print_buf(const char *buf, unsigned int len)
{
    if (redirect_buf_len + len < MAX_REDIRECT_BUF) {
        for (unsigned int i = 0; i < len; i++)
            redirect_buf[redirect_buf_len + i] = buf[i];
        redirect_buf_len += len;
    }
}

static int redirect_write(const redirect_t *redir, int append)
{
    return write_to_file(redir->filename, append, redirect_buf, redirect_buf_len);
}

static void shell_execute_single(const char *cmd)
{
    redirect_t redir;
    char cmd_buf[MAX_LINE];
    parse_redirect(cmd, cmd_buf, &redir);

    /* For output redirect, accumulate into redirect_buf instead of printing */
    if (redir.mode == REDIR_OUT || redir.mode == REDIR_OUT_APP) {
        redirect_buf_len = 0;

        if (my_strcmp(cmd_buf, "help") == 0) {
            cmd_help();
            redirect_write(&redir, redir.mode == REDIR_OUT_APP);
        } else if (my_strcmp(cmd_buf, "tick") == 0) {
            redirect_print("tick: ");
            /* We need tick value as string — compute inline */
            char tick_buf[20];
            unsigned long long tick = wasm_host_get_tick();
            int ti = 0;
            if (tick == 0) tick_buf[ti++] = '0';
            else { unsigned long long v = tick; while (v > 0) { tick_buf[ti++] = (char)('0' + (v % 10)); v /= 10; } }
            for (int j = ti - 1; j >= 0; j--) redirect_print_buf(tick_buf, 1);
            redirect_print("\n");
            redirect_write(&redir, redir.mode == REDIR_OUT_APP);
        } else if (my_strcmp(cmd_buf, "alloc") == 0) {
            char *buf = (char *)shell_malloc(64);
            if (buf) {
                for (int i = 0; i < 32; i++)
                    buf[i] = 'A' + (char)(i % 26);
                buf[32] = '\0';
                redirect_print("alloc 64 bytes: ");
                unsigned int bl = my_strlen(buf);
                redirect_print_buf(buf, bl);
                redirect_print("\nfree ok\n");
                shell_free(buf);
            } else {
                redirect_print("alloc failed!\n");
            }
            redirect_write(&redir, redir.mode == REDIR_OUT_APP);
        } else if (my_strcmp(cmd_buf, "log") == 0) {
            shell_log("INFO", "user requested log test");
            redirect_print("log sent\n");
            redirect_write(&redir, redir.mode == REDIR_OUT_APP);
        } else if (my_strcmp(cmd_buf, "clear") == 0) {
            cmd_clear();
        } else if (my_strcmp(cmd_buf, "ls") == 0) {
            unsigned int buf_off = alloc(2048);
            int total = wasm_host_fs_list(buf_off, 2048);
            if (total <= 0) {
                redirect_print("(empty)\n");
            } else {
                unsigned int i = 0;
                char *names = (char *)buf_off;
                while (i < (unsigned int)total) {
                    if (names[i] == '\0') redirect_print("\n");
                    else redirect_print_buf(names + i, 1);
                    i++;
                }
                redirect_print("\n");
            }
            redirect_write(&redir, redir.mode == REDIR_OUT_APP);
        } else if (cmd_buf[0] == 'c' && cmd_buf[1] == 'a' &&
                   cmd_buf[2] == 't' && cmd_buf[3] == ' ') {
            /* cat with output redirect: cat file > dest */
            const char *filename = cmd_buf + 4;
            unsigned int flen = my_strlen(filename);
            unsigned int path_off = alloc(flen);
            char *pd = (char *)path_off;
            for (unsigned int i = 0; i < flen; i++) pd[i] = filename[i];

            int fd = wasm_host_fs_open(path_off, flen);
            if (fd < 0) {
                redirect_print("cat: no such file: ");
                redirect_print(filename);
                redirect_print("\n");
                redirect_write(&redir, redir.mode == REDIR_OUT_APP);
                return;
            }
            unsigned int rbuf_off = alloc(512);
            for (;;) {
                int n = wasm_host_fs_read(fd, rbuf_off, 512);
                if (n <= 0) break;
                redirect_print_buf((const char *)rbuf_off, (unsigned int)n);
            }
            wasm_host_fs_close(fd);
            redirect_write(&redir, redir.mode == REDIR_OUT_APP);
        } else if (cmd_buf[0] == 'e' && cmd_buf[1] == 'c' &&
                   cmd_buf[2] == 'h' && cmd_buf[3] == 'o' &&
                   cmd_buf[4] == ' ') {
            redirect_print(cmd_buf + 5);
            redirect_print("\n");
            redirect_write(&redir, redir.mode == REDIR_OUT_APP);
        } else if (cmd_buf[0] == 'g' && cmd_buf[1] == 'r' &&
                   cmd_buf[2] == 'e' && cmd_buf[3] == 'p' &&
                   cmd_buf[4] == ' ') {
            /* grep with output redirect */
            /* Spawn grep module and redirect its output */
            unsigned int mod_len = my_strlen(cmd_buf);
            unsigned int cmd_off = alloc(mod_len + 1);
            char *cd = (char *)cmd_off;
            for (unsigned int i = 0; i <= mod_len; i++) cd[i] = cmd_buf[i];
            wasm_host_set_argv(cmd_off, mod_len);

            unsigned int name_off = alloc(4);
            char *nd = (char *)name_off;
            nd[0] = 'g'; nd[1] = 'r'; nd[2] = 'e'; nd[3] = 'p';
            int pid = wasm_host_spawn(name_off, 4);
            if (pid >= 0) {
                /* We can't redirect grep output easily here since it uses host_print directly */
                /* Just wait for it */
                wait_for_pid(pid);
            }
        } else {
            /* External module with output redirect — spawn and wait */
            unsigned int mod_name_len = 0;
            while (cmd_buf[mod_name_len] && cmd_buf[mod_name_len] != ' ') mod_name_len++;
            unsigned int cmd_off = alloc(mod_name_len + 1);
            char *cd = (char *)cmd_off;
            for (unsigned int i = 0; i <= mod_name_len; i++) cd[i] = cmd_buf[i];
            wasm_host_set_argv(cmd_off, mod_name_len);
            int pid = wasm_host_spawn(cmd_off, mod_name_len);
            if (pid >= 0) {
                wait_for_pid(pid);
            } else {
                redirect_print("unknown: ");
                redirect_print(cmd_buf);
                redirect_print("\n");
                redirect_write(&redir, redir.mode == REDIR_OUT_APP);
            }
        }
        return;
    }

    /* For input redirect (<), provide file content as stdin */
    if (redir.mode == REDIR_IN) {
        unsigned int file_len = 0;
        char *file_content = read_file(redir.filename, &file_len);
        if (!file_content) return;

        /* For cat < file, output the file content */
        if (cmd_buf[0] == 'c' && cmd_buf[1] == 'a' &&
            cmd_buf[2] == 't' && cmd_buf[3] == ' ') {
            redirect_print_buf(file_content, file_len);
            redirect_print("\n");
            /* Print to stdout (no output redirect) */
            unsigned int off = alloc(redirect_buf_len);
            char *od = (char *)off;
            for (unsigned int i = 0; i < redirect_buf_len; i++) od[i] = redirect_buf[i];
            wasm_host_print(off, redirect_buf_len);
            redirect_buf_len = 0;
            return;
        }

        /* For grep < file, spawn grep with file content via pipe */
        if (cmd_buf[0] == 'g' && cmd_buf[1] == 'r' &&
            cmd_buf[2] == 'e' && cmd_buf[3] == 'p' &&
            cmd_buf[4] == ' ') {
            /* Create pipe, spawn grep, write file content via host_print to pipe */
            /* For now, just spawn grep (it reads stdin from UART) */
            unsigned int mod_len = my_strlen(cmd_buf);
            unsigned int cmd_off = alloc(mod_len + 1);
            char *cd = (char *)cmd_off;
            for (unsigned int i = 0; i <= mod_len; i++) cd[i] = cmd_buf[i];
            wasm_host_set_argv(cmd_off, mod_len);

            unsigned int name_off = alloc(4);
            char *nd = (char *)name_off;
            nd[0] = 'g'; nd[1] = 'r'; nd[2] = 'e'; nd[3] = 'p';
            int pid = wasm_host_spawn(name_off, 4);
            if (pid >= 0) wait_for_pid(pid);
            return;
        }

        /* External module with input redirect */
        unsigned int mod_name_len = 0;
        while (cmd_buf[mod_name_len] && cmd_buf[mod_name_len] != ' ') mod_name_len++;
        unsigned int cmd_off = alloc(mod_name_len + 1);
        char *cd = (char *)cmd_off;
        for (unsigned int i = 0; i <= mod_name_len; i++) cd[i] = cmd_buf[i];
        wasm_host_set_argv(cmd_off, mod_name_len);
        int pid = wasm_host_spawn(cmd_off, mod_name_len);
        if (pid >= 0) wait_for_pid(pid);
        return;
    }

    /* No redirect — normal execution */
    if (my_strcmp(cmd_buf, "help") == 0) {
        cmd_help();
    } else if (my_strcmp(cmd_buf, "tick") == 0) {
        cmd_tick();
    } else if (my_strcmp(cmd_buf, "alloc") == 0) {
        cmd_alloc();
    } else if (my_strcmp(cmd_buf, "log") == 0) {
        cmd_log();
    } else if (my_strcmp(cmd_buf, "clear") == 0) {
        cmd_clear();
    } else if (my_strcmp(cmd_buf, "ls") == 0) {
        cmd_ls();
    } else if (cmd_buf[0] == 'c' && cmd_buf[1] == 'a' &&
               cmd_buf[2] == 't' && cmd_buf[3] == ' ') {
        cmd_cat(cmd_buf + 4);
    } else if (my_strcmp(cmd_buf, "exit") == 0) {
        shell_log("OP", "shell exiting");
        wasm_host_exit(0);
    } else if (cmd_buf[0] == 'e' && cmd_buf[1] == 'c' &&
               cmd_buf[2] == 'h' && cmd_buf[3] == 'o' &&
               cmd_buf[4] == ' ') {
        cmd_echo(cmd_buf + 5);
    } else {
        print_str("unknown: ");
        print_str(cmd_buf);
        print_str("\n");
    }
}

static void wait_for_pid(int pid)
{
    /* Poll proc_list_next and proc_get_status until pid exits.
     * Yield CPU between polls so spawned processes can run. */
    int iter;
    for (iter = 0; iter < 10000; iter++) {
        int listed_pid = wasm_host_proc_list_next();
        if (listed_pid == pid) {
            int status = wasm_host_proc_get_status(pid);
            if (status != 3) {  /* not MOD_RUNNING (4=EXITED) */
                return;
            }
        }
        if (listed_pid < 0) {
            /* End of list — reset iterator for next scan */
            wasm_host_proc_list_next();
        }
        /* Yield CPU so other WASM modules can execute */
        wasm_host_yield();
    }
}

/* -------------------------------------------------------------------------- */
/* Shell entry point                                                          */
/* -------------------------------------------------------------------------- */

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
