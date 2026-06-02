/* executor: Task execution agent (v9.1) */

__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int off, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_TASKS 32
#define MAX_TASK_NAME 64

typedef struct {
    int id;
    char name[MAX_TASK_NAME];
    int status; /* 0=pending, 1=running, 2=done, 3=failed */
    int result;
} task_t;

static unsigned int heap_pos = 65536;
static task_t task_queue[MAX_TASKS];
static int task_count = 0;
static int next_task_id = 0;

static unsigned int alloc(unsigned int n)
{
    unsigned int ptr = heap_pos;
    heap_pos += n;
    heap_pos = (heap_pos + 15) & ~15u;
    return ptr;
}

static void copy_to_mem(const char *src, unsigned int dst, unsigned int len)
{
    char *d = (char *)dst;
    for (unsigned int i = 0; i < len; i++) d[i] = src[i];
}

static unsigned int my_strlen(const char *s)
{
    unsigned int len = 0;
    while (s[len]) len++;
    return len;
}

static int my_strcmp(const char *a, const char *b)
{
    while (*a && *b) { if (*a != *b) return 1; a++; b++; }
    return (*a != *b);
}

static void copy_string(char *dst, const char *src, unsigned int max_len)
{
    unsigned int i = 0;
    while (src[i] && i < max_len - 1) { dst[i] = src[i]; i++; }
    dst[i] = 0;
}

static void print_str(const char *s)
{
    unsigned int len = my_strlen(s);
    unsigned int buf = alloc(len + 1);
    copy_to_mem(s, buf, len + 1);
    host_print(buf, len);
}

static void print_int(int n)
{
    char buf[16]; int i = 15; int neg = 0; buf[15] = 0;
    if (n < 0) { neg = 1; n = -n; }
    if (n == 0) buf[--i] = '0';
    while (n > 0) { buf[--i] = '0' + (n % 10); n /= 10; }
    if (neg) buf[--i] = '-';
    unsigned int len = my_strlen(&buf[i]);
    unsigned int dst = alloc(len + 1);
    copy_to_mem(&buf[i], dst, len + 1);
    host_print(dst, len);
}

static int add_task(const char *name)
{
    if (task_count >= MAX_TASKS) { print_str("executor: queue full\n"); return -1; }
    task_t *t = &task_queue[task_count];
    t->id = next_task_id++;
    t->status = 0;
    t->result = 0;
    copy_string(t->name, name, MAX_TASK_NAME);
    task_count++;
    print_str("executor: queued task ");
    print_str(name);
    print_str(" (id=");
    print_int(t->id);
    print_str(")\n");
    return t->id;
}

static int run_task(int task_id)
{
    for (int i = 0; i < task_count; i++) {
        if (task_queue[i].id == task_id) {
            task_queue[i].status = 1;
            print_str("executor: running task ");
            print_int(task_id);
            print_str("\n");
            task_queue[i].status = 2;
            task_queue[i].result = 0;
            print_str("executor: task ");
            print_int(task_id);
            print_str(" completed\n");
            return 0;
        }
    }
    return -1;
}

static void show_help(void)
{
    print_str("executor: Task execution agent (v9.1)\n");
    print_str("Usage: executor [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -t             Test mode\n");
    print_str("  -l             List tasks\n");
}

static void show_info(void)
{
    print_str("executor: info:\n");
    print_str("executor: max tasks: 32\n");
    print_str("executor: task states: pending, running, done, failed\n");
    print_str("executor: info display complete\n");
}

static void show_status(void)
{
    int pending = 0, running = 0, done = 0, failed = 0;
    for (int i = 0; i < task_count; i++) {
        switch (task_queue[i].status) {
            case 0: pending++; break;
            case 1: running++; break;
            case 2: done++; break;
            case 3: failed++; break;
        }
    }
    print_str("executor: status:\n");
    print_str("executor: total: ");
    print_int(task_count);
    print_str(" pending: ");
    print_int(pending);
    print_str(" running: ");
    print_int(running);
    print_str(" done: ");
    print_int(done);
    print_str(" failed: ");
    print_int(failed);
    print_str("\n");
}

static void list_tasks(void)
{
    print_str("executor: tasks (");
    print_int(task_count);
    print_str("/");
    print_int(MAX_TASKS);
    print_str(")\n");
    for (int i = 0; i < task_count; i++) {
        print_str("executor:   id=");
        print_int(task_queue[i].id);
        print_str(" name=");
        print_str(task_queue[i].name);
        print_str(" status=");
        print_int(task_queue[i].status);
        print_str("\n");
    }
}

static void test_mode(int verbose)
{
    print_str("executor: testing\n");

    int t1 = add_task("compile_module");
    int t2 = add_task("run_tests");
    int t3 = add_task("deploy");

    if (t1 >= 0 && t2 >= 0 && t3 >= 0) {
        print_str("executor: queue tasks: OK\n");
    }

    if (run_task(t1) == 0 && run_task(t2) == 0) {
        print_str("executor: run tasks: OK\n");
    }

    if (verbose) list_tasks();
    print_str("executor: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    host_get_argv(buf, 512);

    int help = 0, info = 0, verbose = 0, status = 0, test = 0, list = 0;

    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;

    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0) help = 1;
        else if (my_strcmp(arg, "-i") == 0) info = 1;
        else if (my_strcmp(arg, "-v") == 0) verbose = 1;
        else if (my_strcmp(arg, "-s") == 0) status = 1;
        else if (my_strcmp(arg, "-t") == 0) test = 1;
        else if (my_strcmp(arg, "-l") == 0) list = 1;
        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    if (help) { show_help(); host_exit(0); }
    if (info) { show_info(); host_exit(0); }
    if (status) { show_status(); host_exit(0); }
    if (test) { test_mode(verbose); host_exit(0); }
    if (list) { list_tasks(); host_exit(0); }

    print_str("executor: usage: executor [-t] [-l] [-v] [-i] [-s]\n");
    host_exit(0);
}
