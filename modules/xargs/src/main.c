/*
 * aiasm-aarch64/modules/xargs/src/main.c
 * xargs WASM module for AI-ASM AArch64 v32.0
 *
 * Usage: xargs [COMMAND [ARGS...]]
 * Reads stdin, appends each whitespace-separated token as argument to COMMAND.
 * Default command: echo.
 *
 * Designed for use in shell pipes: ls | xargs wc -l
 */

__attribute__((import_module("host"), import_name("print")))
extern void wasm_host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("getc")))
extern int wasm_host_getc(void);

__attribute__((import_module("host"), import_name("exit")))
extern void wasm_host_exit(int code);

__attribute__((import_module("host"), import_name("alloc")))
extern void *wasm_host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("yield")))
extern void wasm_host_yield(void);

__attribute__((import_module("host"), import_name("get_argv")))
extern int wasm_host_get_argv(unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("set_argv")))
extern void wasm_host_set_argv(unsigned int buf_off, unsigned int buf_len);

__attribute__((import_module("host"), import_name("spawn_redirect")))
extern int wasm_host_spawn_redirect(unsigned int name_off, unsigned int name_len,
                                     int stdin_fd, int stdout_fd);

__attribute__((import_module("host"), import_name("pipe_create")))
extern int wasm_host_pipe_create(unsigned int rfd_off, unsigned int wfd_off);

__attribute__((import_module("host"), import_name("pipe_read")))
extern int wasm_host_pipe_read(int fd, unsigned int buf_off, unsigned int len);

__attribute__((import_module("host"), import_name("pipe_close")))
extern void wasm_host_pipe_close(int fd);

__attribute__((import_module("host"), import_name("proc_get_status")))
extern int wasm_host_proc_get_status(int pid);

/* -------------------------------------------------------------------------- */
/* WASM memory allocator (bump)                                               */
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
/* Constants                                                                  */
/* -------------------------------------------------------------------------- */

#define MAX_ARGV 128
#define MAX_ARGS 32
#define MAX_ARG_LEN 64

/* -------------------------------------------------------------------------- */
/* Helpers                                                                    */
/* -------------------------------------------------------------------------- */

static void print_str(const char *s, unsigned int len)
{
    unsigned int offset = alloc(len);
    char *dst = (char *)offset;
    for (unsigned int i = 0; i < len; i++)
        dst[i] = s[i];
    wasm_host_print(offset, len);
}

/* -------------------------------------------------------------------------- */
/* Main                                                                       */
/* -------------------------------------------------------------------------- */

__attribute__((export_name("_start")))
void _start(void)
{
    /* Get argv from parent */
    unsigned int argv_off = alloc(MAX_ARGV);
    int argv_len = wasm_host_get_argv(argv_off, MAX_ARGV - 1);
    if (argv_len < 0) argv_len = 0;

    const char *argv = (const char *)argv_off;
    int i = 0;

    /* Skip module name */
    while (i < argv_len && argv[i] != ' ') i++;
    while (i < argv_len && argv[i] == ' ') i++;

    /* Parse initial command and args from argv */
    char cmd_args[MAX_ARGS][MAX_ARG_LEN];
    unsigned int num_args = 0;

    while (i < argv_len && num_args < MAX_ARGS) {
        unsigned int arg_start = 0;
        while (i < argv_len && argv[i] == ' ') i++;
        if (i >= argv_len) break;
        arg_start = i;
        while (i < argv_len && argv[i] != ' ') i++;
        unsigned int arg_len = (unsigned int)(i - arg_start);
        if (arg_len >= MAX_ARG_LEN) arg_len = MAX_ARG_LEN - 1;
        for (unsigned int j = 0; j < arg_len; j++)
            cmd_args[num_args][j] = argv[arg_start + j];
        cmd_args[num_args][arg_len] = '\0';
        num_args++;
    }

    /* If no command specified, default to "echo" */
    if (num_args == 0) {
        cmd_args[0][0] = 'e'; cmd_args[0][1] = 'c';
        cmd_args[0][2] = 'h'; cmd_args[0][3] = 'o';
        cmd_args[0][4] = '\0';
        num_args = 1;
    }

    /* Read stdin, collect arguments */
    char stdin_arg[MAX_ARG_LEN];
    unsigned int stdin_arg_len = 0;
    unsigned int total_stdin_args = 0;

    for (;;) {
        int c = wasm_host_getc();

        if (c == -1) {
            /* EOF: finalize last argument */
            if (stdin_arg_len > 0 && num_args < MAX_ARGS) {
                stdin_arg[stdin_arg_len] = '\0';
                for (unsigned int j = 0; j < stdin_arg_len && j < MAX_ARG_LEN - 1; j++)
                    cmd_args[num_args][j] = stdin_arg[j];
                cmd_args[num_args][stdin_arg_len] = '\0';
                num_args++;
                total_stdin_args++;
            }
            break;
        }
        if (c == 0) {
            wasm_host_yield();
            continue;
        }

        char ch = (char)c;

        if (ch == ' ' || ch == '\n' || ch == '\t') {
            /* Whitespace: finalize current argument */
            if (stdin_arg_len > 0 && num_args < MAX_ARGS) {
                stdin_arg[stdin_arg_len] = '\0';
                for (unsigned int j = 0; j < stdin_arg_len && j < MAX_ARG_LEN - 1; j++)
                    cmd_args[num_args][j] = stdin_arg[j];
                cmd_args[num_args][stdin_arg_len] = '\0';
                num_args++;
                total_stdin_args++;
                stdin_arg_len = 0;
            }
        } else {
            if (stdin_arg_len < MAX_ARG_LEN - 1)
                stdin_arg[stdin_arg_len++] = ch;
        }

        wasm_host_yield();
    }

    /* Build argv string for child process */
    unsigned int child_argv_off = alloc(MAX_ARGV);
    char *child_argv = (char *)child_argv_off;
    unsigned int pos = 0;

    for (unsigned int j = 0; j < num_args; j++) {
        if (j > 0) {
            if (pos < MAX_ARGV - 1) child_argv[pos++] = ' ';
        }
        unsigned int k = 0;
        while (cmd_args[j][k] && pos < MAX_ARGV - 1) {
            child_argv[pos++] = cmd_args[j][k];
            k++;
        }
    }
    child_argv[pos] = '\0';

    /* Create pipe for capturing stdout */
    unsigned int pipe_rfd_off = alloc(4);
    unsigned int pipe_wfd_off = alloc(4);
    int rc = wasm_host_pipe_create(pipe_rfd_off, pipe_wfd_off);
    int pipe_rfd = *(int *)pipe_rfd_off;
    int pipe_wfd = *(int *)pipe_wfd_off;

    /* Set child argv and spawn */
    wasm_host_set_argv(child_argv_off, pos);

    /* Spawn with -1 for stdin (no redirect), pipe_wfd for stdout */
    int spawn_rc = wasm_host_spawn_redirect(child_argv_off, pos, -1, pipe_wfd);

    if (spawn_rc < 0) {
        /* Spawn failed, just echo the args */
        print_str(child_argv, pos);
        unsigned int no = alloc(1);
        *(char *)no = '\n';
        wasm_host_print(no, 1);
    } else {
        /* Read from pipe and output */
        unsigned int read_buf_off = alloc(4096);
        char *read_buf = (char *)read_buf_off;

        for (;;) {
            int n = wasm_host_pipe_read(pipe_rfd, read_buf_off, 4096);
            if (n > 0) {
                print_str(read_buf, (unsigned int)n);
            } else if (n == 0) {
                /* EOF from pipe */
                break;
            } else {
                wasm_host_yield();
            }
        }

        /* Wait for process to exit */
        int pid = spawn_rc;
        for (;;) {
            int status = wasm_host_proc_get_status(pid);
            if (status < 0) break; /* exited */
            wasm_host_yield();
        }
    }

    /* Close pipe fds */
    wasm_host_pipe_close(pipe_rfd);
    wasm_host_pipe_close(pipe_wfd);

    wasm_host_exit(0);
}
