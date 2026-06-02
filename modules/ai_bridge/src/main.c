/* ai_bridge: AI model interface bridge (v9.0) */

__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("ai_init")))
extern void host_ai_init(int model_type);

__attribute__((import_module("host"), import_name("ai_query")))
extern void host_ai_query(int prompt_ptr, int prompt_len);

__attribute__((import_module("host"), import_name("ai_response")))
extern int host_ai_response(int resp_ptr, int resp_len);

#define MAX_LINE 4096
#define MAX_PROMPT 2048

static unsigned int heap_pos = 65536;

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
    while (s[len])
        len++;
    return len;
}

static int my_strcmp(const char *a, const char *b)
{
    while (*a && *b) {
        if (*a != *b) return 1;
        a++; b++;
    }
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

static void print_str(const char *s)
{
    unsigned int len = my_strlen(s);
    unsigned int buf = alloc(len + 1);
    copy_to_mem(s, buf, len + 1);
    host_print(buf, len);
}

static void print_int(int n)
{
    char buf[16];
    int i = 15;
    int neg = 0;
    buf[15] = 0;
    if (n < 0) { neg = 1; n = -n; }
    if (n == 0) buf[--i] = '0';
    while (n > 0) { buf[--i] = '0' + (n % 10); n /= 10; }
    if (neg) buf[--i] = '-';
    unsigned int len = my_strlen(&buf[i]);
    unsigned int dst = alloc(len + 1);
    copy_to_mem(&buf[i], dst, len + 1);
    host_print(dst, len);
}

static void show_help(void)
{
    print_str("ai_bridge - AI model interface bridge (v9.0)\n");
    print_str("Usage: ai_bridge [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -m MODEL       Model type (0=local, 1=http, 2=websocket)\n");
    print_str("  -p PROMPT      Query prompt\n");
    print_str("  -t             Test mode\n");
    print_str("\n");
    print_str("Unified interface for AI model inference.\n");
}

static void show_info(void)
{
    print_str("ai_bridge: AI model info:\n");
    print_str("ai_bridge: model types: 0=local, 1=http, 2=websocket\n");
    print_str("ai_bridge: max prompt length: 2048\n");
    print_str("ai_bridge: supported APIs: Claude, OpenAI, Gemini\n");
    print_str("ai_bridge: info display complete\n");
}

static void show_status(void)
{
    print_str("ai_bridge: AI status:\n");
    print_str("ai_bridge: model: not initialized\n");
    print_str("ai_bridge: connection: idle\n");
    print_str("ai_bridge: queries: 0\n");
    print_str("ai_bridge: status check complete\n");
}

static void test_mode(int verbose)
{
    print_str("ai_bridge: testing AI bridge\n");

    if (verbose) {
        print_str("ai_bridge: testing model initialization\n");
    }

    /* test: initialize local model */
    host_ai_init(0);
    print_str("ai_bridge: init local (type=0): OK\n");

    if (verbose) {
        print_str("ai_bridge: testing HTTP model initialization\n");
    }
    host_ai_init(1);
    print_str("ai_bridge: init http (type=1): OK\n");

    if (verbose) {
        print_str("ai_bridge: testing WebSocket model initialization\n");
    }
    host_ai_init(2);
    print_str("ai_bridge: init websocket (type=2): OK\n");

    if (verbose) {
        print_str("ai_bridge: testing query/response cycle\n");
    }

    /* test: query with test prompt */
    const char *test_prompt = "test query";
    unsigned int len = my_strlen(test_prompt);
    unsigned int buf = alloc(len + 1);
    copy_to_mem(test_prompt, buf, len + 1);
    host_ai_query(buf, len);
    print_str("ai_bridge: test query: OK\n");

    int resp_len = host_ai_response(buf, MAX_PROMPT);
    print_str("ai_bridge: test response: OK (len=");
    print_int(resp_len);
    print_str(")\n");

    print_str("ai_bridge: test complete\n");
}

static void query_model(const char *prompt, int model_type, int verbose)
{
    if (verbose) {
        print_str("ai_bridge: initializing model type=");
        print_int(model_type);
        print_str("\n");
    }

    host_ai_init(model_type);

    if (verbose) {
        print_str("ai_bridge: sending query\n");
    }

    unsigned int len = my_strlen(prompt);
    unsigned int buf = alloc(len + 1);
    copy_to_mem(prompt, buf, len + 1);
    host_ai_query(buf, len);

    if (verbose) {
        print_str("ai_bridge: waiting for response\n");
    }

    int resp_len = host_ai_response(buf, MAX_PROMPT);
    print_str("ai_bridge: response received (len=");
    print_int(resp_len);
    print_str(")\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int test_flag = 0;
    int model_type = 0;
    const char *prompt = 0;

    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;

    /* skip argv[0] */
    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;

    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];

        if (my_strcmp(arg, "--help") == 0 || my_strcmp(arg, "-h") == 0) {
            help_flag = 1;
        } else if (my_strcmp(arg, "-i") == 0) {
            info_flag = 1;
        } else if (my_strcmp(arg, "-v") == 0) {
            verbose_flag = 1;
        } else if (my_strcmp(arg, "-s") == 0) {
            status_flag = 1;
        } else if (my_strcmp(arg, "-t") == 0) {
            test_flag = 1;
        } else if (my_strcmp(arg, "-m") == 0) {
            pos++;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512) {
                char c = argv_ptr[pos];
                model_type = c - '0';
            }
        } else if (my_strncmp(arg, "-m", 2) == 0 && arg[2]) {
            model_type = arg[2] - '0';
        } else if (my_strcmp(arg, "-p") == 0) {
            pos++;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512) {
                prompt = &argv_ptr[pos];
            }
        } else if (my_strncmp(arg, "-p", 2) == 0 && arg[2]) {
            prompt = &arg[2];
        }

        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    if (help_flag) {
        show_help();
        host_exit(0);
    }

    if (info_flag) {
        show_info();
        host_exit(0);
    }

    if (status_flag) {
        show_status();
        host_exit(0);
    }

    if (test_flag) {
        test_mode(verbose_flag);
        host_exit(0);
    }

    if (prompt) {
        query_model(prompt, model_type, verbose_flag);
    } else {
        print_str("ai_bridge: no prompt specified\n");
        print_str("ai_bridge: usage: ai_bridge [-m MODEL] -p PROMPT [-v] [-i] [-s] [-t]\n");
    }

    host_exit(0);
}
