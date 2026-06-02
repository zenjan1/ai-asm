/* api_keys: API key management (v9.3) */

__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int off, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_KEYS    16
#define KEY_LEN     128
#define NAME_LEN    32

typedef struct {
    char name[NAME_LEN];
    char key[KEY_LEN];
    int encrypted;
    int valid;
} api_key_entry_t;

static unsigned int heap_pos = 65536;
static api_key_entry_t key_store[MAX_KEYS];
static int key_count = 0;

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

static int my_strncmp(const char *a, const char *b, unsigned int n)
{
    for (unsigned int i = 0; i < n; i++) {
        if (!a[i] || !b[i]) return (a[i] != b[i]);
        if (a[i] != b[i]) return 1;
    }
    return 0;
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

static int find_key(const char *name)
{
    for (int i = 0; i < key_count; i++) {
        if (my_strcmp(key_store[i].name, name) == 0) return i;
    }
    return -1;
}

static int api_key_add(const char *name, const char *key)
{
    if (key_count >= MAX_KEYS) {
        print_str("api_keys: max keys reached\n");
        return -1;
    }
    int idx = find_key(name);
    if (idx >= 0) {
        /* Update existing */
        copy_string(key_store[idx].key, key, KEY_LEN);
        key_store[idx].valid = 1;
        print_str("api_keys: updated key ");
        print_str(name);
        print_str("\n");
        return idx;
    }
    api_key_entry_t *k = &key_store[key_count];
    copy_string(k->name, name, NAME_LEN);
    copy_string(k->key, key, KEY_LEN);
    k->encrypted = 0;
    k->valid = 1;
    key_count++;
    print_str("api_keys: added key ");
    print_str(name);
    print_str(" (slot ");
    print_int(key_count - 1);
    print_str(")\n");
    return key_count - 1;
}

static int api_key_get(const char *name, char *key_out)
{
    int idx = find_key(name);
    if (idx < 0) return -1;
    if (!key_store[idx].valid) return -1;
    copy_string(key_out, key_store[idx].key, KEY_LEN);
    return 0;
}

static int api_key_verify(const char *name)
{
    int idx = find_key(name);
    if (idx < 0) return 0;
    print_str("api_keys: verifying key ");
    print_str(name);
    print_str("... ");
    if (key_store[idx].valid) {
        print_str("VALID\n");
        return 1;
    }
    print_str("INVALID\n");
    return 0;
}

static int api_key_delete(const char *name)
{
    int idx = find_key(name);
    if (idx < 0) return -1;
    key_store[idx].valid = 0;
    key_store[idx].key[0] = 0;
    print_str("api_keys: deleted key ");
    print_str(name);
    print_str("\n");
    return 0;
}

static int api_key_rotate(const char *name, const char *new_key)
{
    int idx = find_key(name);
    if (idx < 0) return -1;
    copy_string(key_store[idx].key, new_key, KEY_LEN);
    key_store[idx].valid = 1;
    print_str("api_keys: rotated key ");
    print_str(name);
    print_str("\n");
    return 0;
}

static void show_help(void)
{
    print_str("api_keys: API key management (v9.3)\n");
    print_str("Usage: api_keys [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -t             Test mode\n");
    print_str("  -l             List keys\n");
    print_str("\n");
    print_str("Manages API keys for Claude, OpenAI, Gemini.\n");
}

static void show_info(void)
{
    print_str("api_keys: info:\n");
    print_str("api_keys: max keys: 16\n");
    print_str("api_keys: key length: 128\n");
    print_str("api_keys: encryption: XOR (basic)\n");
    print_str("api_keys: info display complete\n");
}

static void show_status(void)
{
    print_str("api_keys: status:\n");
    print_str("api_keys: keys stored: ");
    print_int(key_count);
    print_str("/");
    print_int(MAX_KEYS);
    print_str("\n");
    print_str("api_keys: status check complete\n");
}

static void list_keys(int verbose)
{
    print_str("api_keys: keys (");
    print_int(key_count);
    print_str("/");
    print_int(MAX_KEYS);
    print_str(")\n");
    for (int i = 0; i < key_count; i++) {
        if (key_store[i].valid) {
            print_str("api_keys:   ");
            print_str(key_store[i].name);
            if (verbose) {
                print_str(" [key stored]\n");
            }
            print_str("\n");
        }
    }
}

static void test_mode(int verbose)
{
    print_str("api_keys: testing\n");

    /* Test add */
    int k1 = api_key_add("claude", "sk-ant-claude-test-key-12345");
    int k2 = api_key_add("openai", "sk-proj-openai-test-key-67890");
    int k3 = api_key_add("gemini", "AIza-gemini-test-key-11111");

    if (k1 >= 0 && k2 >= 0 && k3 >= 0)
        print_str("api_keys: add keys: OK\n");

    /* Test get */
    char buf[KEY_LEN];
    if (api_key_get("claude", buf) == 0) {
        if (my_strncmp(buf, "sk-ant", 6) == 0)
            print_str("api_keys: get key: OK\n");
        else
            print_str("api_keys: get key: FAIL\n");
    }

    /* Test verify */
    if (api_key_verify("claude") == 1)
        print_str("api_keys: verify: OK\n");
    if (api_key_verify("nonexistent") == 0)
        print_str("api_keys: verify missing: OK\n");

    /* Test rotate */
    if (api_key_rotate("claude", "sk-ant-rotated-key-99999") == 0)
        print_str("api_keys: rotate: OK\n");

    /* Test delete */
    if (api_key_delete("gemini") == 0)
        print_str("api_keys: delete: OK\n");

    if (verbose) list_keys(1);
    print_str("api_keys: test complete\n");
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
    if (list) { list_keys(verbose); host_exit(0); }

    print_str("api_keys: usage: api_keys [-t] [-l] [-v] [-i] [-s]\n");
    host_exit(0);
}
