/* planner: Task planning and decomposition agent (v9.1) */

__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int off, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_STEPS 32
#define MAX_PLAN_NAME 64
#define MAX_DESC_LEN 128

typedef struct {
    int id;
    int priority;
    char description[MAX_DESC_LEN];
} step_t;

typedef struct {
    int id;
    char name[MAX_PLAN_NAME];
    int step_count;
    step_t steps[MAX_STEPS];
    int status; /* 0=pending, 1=active, 2=complete */
} plan_t;

static unsigned int heap_pos = 65536;
static plan_t plans[16];
static int plan_count = 0;
static int next_plan_id = 0;

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
    for (unsigned int i = 0; i < len; i++)
        d[i] = src[i];
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

static int my_strncmp(const char *a, const char *b, unsigned int n)
{
    for (unsigned int i = 0; i < n; i++) {
        if (!a[i] || !b[i]) return (a[i] != b[i]);
        if (a[i] != b[i]) return 1;
    }
    return 0;
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

static int create_plan(const char *name)
{
    if (plan_count >= 16) { print_str("planner: max plans reached\n"); return -1; }
    plan_t *p = &plans[plan_count];
    p->id = next_plan_id++;
    p->step_count = 0;
    p->status = 0;
    copy_string(p->name, name, MAX_PLAN_NAME);
    plan_count++;
    print_str("planner: created plan ");
    print_str(name);
    print_str(" (id=");
    print_int(p->id);
    print_str(")\n");
    return p->id;
}

static int add_step(int plan_id, const char *desc, int priority)
{
    for (int i = 0; i < plan_count; i++) {
        if (plans[i].id == plan_id) {
            if (plans[i].step_count >= MAX_STEPS) return -1;
            step_t *s = &plans[i].steps[plans[i].step_count];
            s->id = plans[i].step_count;
            s->priority = priority;
            copy_string(s->description, desc, MAX_DESC_LEN);
            plans[i].step_count++;
            return s->id;
        }
    }
    return -1;
}

static void show_help(void)
{
    print_str("planner: Task planning and decomposition (v9.1)\n");
    print_str("Usage: planner [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -t             Test mode\n");
    print_str("  -l             List plans\n");
}

static void show_info(void)
{
    print_str("planner: info:\n");
    print_str("planner: max plans: 16\n");
    print_str("planner: max steps per plan: 32\n");
    print_str("planner: info display complete\n");
}

static void show_status(void)
{
    print_str("planner: status:\n");
    print_str("planner: active plans: ");
    print_int(plan_count);
    print_str("\n");
    print_str("planner: status check complete\n");
}

static void list_plans(int verbose)
{
    print_str("planner: plans (");
    print_int(plan_count);
    print_str("/16)\n");
    for (int i = 0; i < plan_count; i++) {
        print_str("planner:   id=");
        print_int(plans[i].id);
        print_str(" name=");
        print_str(plans[i].name);
        print_str(" steps=");
        print_int(plans[i].step_count);
        if (verbose) {
            for (int j = 0; j < plans[i].step_count; j++) {
                print_str("planner:     step ");
                print_int(plans[i].steps[j].id);
                print_str(": ");
                print_str(plans[i].steps[j].description);
                print_str("\n");
            }
        }
    }
}

static void test_mode(int verbose)
{
    print_str("planner: testing\n");

    int p1 = create_plan("build_feature");
    if (p1 >= 0) {
        add_step(p1, "analyze requirements", 1);
        add_step(p1, "design API", 2);
        add_step(p1, "implement code", 3);
        add_step(p1, "write tests", 4);
        print_str("planner: create plan + steps: OK\n");
    }

    int p2 = create_plan("refactor_module");
    if (p2 >= 0) {
        add_step(p2, "identify dependencies", 1);
        add_step(p2, "extract interfaces", 2);
        print_str("planner: second plan: OK\n");
    }

    if (verbose) list_plans(1);
    print_str("planner: test complete\n");
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
    if (list) { list_plans(verbose); host_exit(0); }

    print_str("planner: usage: planner [-t] [-l] [-v] [-i] [-s]\n");
    host_exit(0);
}
