/* http_client: HTTP/HTTPS client for API calls (v9.3) */

__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int off, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("net_connect")))
extern int host_net_connect(unsigned int host_ptr, unsigned int host_len);

__attribute__((import_module("host"), import_name("net_send")))
extern int host_net_send(unsigned int fd, unsigned int buf_ptr, unsigned int buf_len);

__attribute__((import_module("host"), import_name("net_recv")))
extern int host_net_recv(unsigned int fd, unsigned int buf_ptr, unsigned int buf_len);

__attribute__((import_module("host"), import_name("net_close")))
extern void host_net_close(unsigned int fd);

#define HTTP_MAX_URL     256
#define HTTP_MAX_BODY    4096
#define HTTP_MAX_RESP    8192
#define HTTP_MAX_HDRS  1024

#define API_CLAUDE  0
#define API_OPENAI  1
#define API_GEMINI  2

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

static const char *api_name(int t)
{
    switch (t) {
        case API_CLAUDE: return "Claude";
        case API_OPENAI: return "OpenAI";
        case API_GEMINI: return "Gemini";
        default: return "Unknown";
    }
}

static const char *api_host(int t)
{
    switch (t) {
        case API_CLAUDE: return "api.anthropic.com";
        case API_OPENAI: return "api.openai.com";
        case API_GEMINI: return "generativelanguage.googleapis.com";
        default: return "";
    }
}

/* Build JSON request body for Claude API */
static void build_claude_request(char *buf, const char *prompt, unsigned int buf_size)
{
    unsigned int pos = 0;
    const char *prefix = "{\"model\":\"claude-sonnet-4-20250514\",\"max_tokens\":1024,\"messages\":[{\"role\":\"user\",\"content\":\"";
    unsigned int plen = my_strlen(prefix);
    if (plen < buf_size) {
        for (unsigned int i = 0; i < plen; i++) buf[pos++] = prefix[i];
    }
    unsigned int prompt_len = my_strlen(prompt);
    for (unsigned int i = 0; i < prompt_len && pos < buf_size - 3; i++)
        buf[pos++] = prompt[i];
    const char *suffix = "\"}]}";
    unsigned int slen = my_strlen(suffix);
    for (unsigned int i = 0; i < slen && pos < buf_size; i++) buf[pos++] = suffix[i];
    buf[pos] = 0;
}

/* Build JSON request body for OpenAI API */
static void build_openai_request(char *buf, const char *prompt, unsigned int buf_size)
{
    unsigned int pos = 0;
    const char *prefix = "{\"model\":\"gpt-4o\",\"max_tokens\":1024,\"messages\":[{\"role\":\"user\",\"content\":\"";
    unsigned int plen = my_strlen(prefix);
    if (plen < buf_size) {
        for (unsigned int i = 0; i < plen; i++) buf[pos++] = prefix[i];
    }
    unsigned int prompt_len = my_strlen(prompt);
    for (unsigned int i = 0; i < prompt_len && pos < buf_size - 3; i++)
        buf[pos++] = prompt[i];
    const char *suffix = "\"}]}";
    unsigned int slen = my_strlen(suffix);
    for (unsigned int i = 0; i < slen && pos < buf_size; i++) buf[pos++] = suffix[i];
    buf[pos] = 0;
}

/* Build JSON request body for Gemini API */
static void build_gemini_request(char *buf, const char *prompt, unsigned int buf_size)
{
    unsigned int pos = 0;
    const char *prefix = "{\"contents\":[{\"parts\":[{\"text\":\"";
    unsigned int plen = my_strlen(prefix);
    if (plen < buf_size) {
        for (unsigned int i = 0; i < plen; i++) buf[pos++] = prefix[i];
    }
    unsigned int prompt_len = my_strlen(prompt);
    for (unsigned int i = 0; i < prompt_len && pos < buf_size - 3; i++)
        buf[pos++] = prompt[i];
    const char *suffix = "\"}]}";
    unsigned int slen = my_strlen(suffix);
    for (unsigned int i = 0; i < slen && pos < buf_size; i++) buf[pos++] = suffix[i];
    buf[pos] = 0;
}

/* Parse JSON response to extract content field */
static int json_parse_field(const char *json, const char *key, char *value, unsigned int max_len)
{
    unsigned int key_len = my_strlen(key);
    unsigned int json_len = my_strlen(json);

    for (unsigned int i = 0; i < json_len - key_len; i++) {
        if (my_strncmp(&json[i], key, key_len) == 0) {
            /* Found key, skip colon and quote */
            unsigned int j = i + key_len;
            while (j < json_len && (json[j] == ':' || json[j] == '"' || json[j] == ' ')) j++;
            unsigned int vpos = 0;
            while (j < json_len && json[j] != '"' && json[j] != '}' && vpos < max_len - 1) {
                value[vpos++] = json[j++];
            }
            value[vpos] = 0;
            return 1;
        }
    }
    return 0;
}

static void call_api(int api_type, const char *prompt, int verbose)
{
    print_str("http_client: calling ");
    print_str(api_name(api_type));
    print_str(" API\n");

    const char *host = api_host(api_type);
    unsigned int hlen = my_strlen(host);
    unsigned int hbuf = alloc(hlen + 1);
    copy_to_mem(host, hbuf, hlen + 1);

    if (verbose) {
        print_str("http_client: connecting to ");
        print_str(host);
        print_str("\n");
    }

    int fd = host_net_connect(hbuf, hlen);
    if (fd < 0) {
        print_str("http_client: connection failed\n");
        return;
    }
    if (verbose) {
        print_str("http_client: connected (fd=");
        print_int(fd);
        print_str(")\n");
    }

    /* Build request */
    unsigned int req_buf = alloc(HTTP_MAX_BODY);
    unsigned int resp_buf = alloc(HTTP_MAX_RESP);

    switch (api_type) {
        case API_CLAUDE: build_claude_request((char *)req_buf, prompt, HTTP_MAX_BODY); break;
        case API_OPENAI: build_openai_request((char *)req_buf, prompt, HTTP_MAX_BODY); break;
        case API_GEMINI: build_gemini_request((char *)req_buf, prompt, HTTP_MAX_BODY); break;
    }

    if (verbose) {
        print_str("http_client: sending request\n");
    }

    int sent = host_net_send(fd, req_buf, HTTP_MAX_BODY);
    if (verbose) {
        print_str("http_client: sent ");
        print_int(sent);
        print_str(" bytes\n");
    }

    int recv = host_net_recv(fd, resp_buf, HTTP_MAX_RESP);
    if (verbose) {
        print_str("http_client: received ");
        print_int(recv);
        print_str(" bytes\n");
    }

    /* Parse response */
    char content[512];
    if (json_parse_field((char *)resp_buf, "content", content, 512)) {
        print_str("http_client: response: ");
        print_str(content);
        print_str("\n");
    } else {
        print_str("http_client: response received\n");
    }

    host_net_close(fd);
    print_str("http_client: connection closed\n");
}

static void show_help(void)
{
    print_str("http_client: HTTP/HTTPS API client (v9.3)\n");
    print_str("Usage: http_client [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -a API         API type (0=Claude, 1=OpenAI, 2=Gemini)\n");
    print_str("  -p PROMPT      Query prompt\n");
    print_str("  -t             Test mode\n");
    print_str("\n");
    print_str("Supported: Claude, OpenAI, Gemini APIs\n");
}

static void show_info(void)
{
    print_str("http_client: info:\n");
    print_str("http_client: max URL: 256\n");
    print_str("http_client: max body: 4096\n");
    print_str("http_client: max response: 8192\n");
    print_str("http_client: APIs: Claude, OpenAI, Gemini\n");
    print_str("http_client: info display complete\n");
}

static void show_status(void)
{
    print_str("http_client: status: ready\n");
    print_str("http_client: status check complete\n");
}

static void test_mode(int verbose)
{
    print_str("http_client: testing\n");

    if (verbose) {
        print_str("http_client: testing JSON parsing\n");
    }

    /* Test JSON parsing */
    const char *test_json = "{\"content\":\"Hello from API\",\"status\":\"ok\"}";
    char result[64];
    if (json_parse_field(test_json, "content", result, 64)) {
        if (my_strcmp(result, "Hello from API") == 0)
            print_str("http_client: json parse: OK\n");
        else
            print_str("http_client: json parse: FAIL (got ");
    } else {
        print_str("http_client: json parse: FAIL\n");
    }

    /* Test request building */
    char buf[512];
    build_claude_request(buf, "test prompt", 512);
    if (my_strncmp(buf, "{\"model\":", 9) == 0)
        print_str("http_client: claude build: OK\n");
    else
        print_str("http_client: claude build: FAIL\n");

    build_openai_request(buf, "test", 512);
    if (my_strncmp(buf, "{\"model\":", 9) == 0)
        print_str("http_client: openai build: OK\n");
    else
        print_str("http_client: openai build: FAIL\n");

    build_gemini_request(buf, "test", 512);
    if (my_strncmp(buf, "{\"contents\":", 12) == 0)
        print_str("http_client: gemini build: OK\n");
    else
        print_str("http_client: gemini build: FAIL\n");

    print_str("http_client: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    host_get_argv(buf, 512);

    int help = 0, info = 0, verbose = 0, status = 0, test = 0;
    int api_type = API_CLAUDE;
    const char *prompt = 0;

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
        else if (my_strcmp(arg, "-a") == 0) {
            pos++;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512) api_type = argv_ptr[pos] - '0';
        } else if (my_strcmp(arg, "-p") == 0) {
            pos++;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512) prompt = &argv_ptr[pos];
        }
        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    if (help) { show_help(); host_exit(0); }
    if (info) { show_info(); host_exit(0); }
    if (status) { show_status(); host_exit(0); }
    if (test) { test_mode(verbose); host_exit(0); }

    if (prompt) {
        call_api(api_type, prompt, verbose);
    } else {
        print_str("http_client: usage: http_client [-a API] -p PROMPT [-v] [-i] [-s] [-t]\n");
    }

    host_exit(0);
}
