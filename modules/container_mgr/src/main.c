/* container_mgr: WASM container lifecycle manager (v12.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);
extern int container_create(const char *name, const char *image, int mem_limit);
extern int container_start(int container_id);
extern int container_stop(int container_id);
extern int container_destroy(int container_id);
extern int container_exec(int container_id, const char *module_path);
extern int container_get_status(int container_id, unsigned int buf);
extern int container_list(unsigned int buf, int max_count);
extern int container_init(void);

/* Limits */
#define MAX_CONTAINERS 32
#define CONTAINER_NAME_LEN 64
#define CONTAINER_IMAGE_LEN 128

/* Container states */
#define STATUS_CREATED    0
#define STATUS_RUNNING    1
#define STATUS_STOPPED    2
#define STATUS_PAUSED     3
#define STATUS_DESTROYED  4

/* Container entry */
typedef struct {
    int   container_id;
    char  name[CONTAINER_NAME_LEN];
    char  image[CONTAINER_IMAGE_LEN];
    int   status;
    int   memory_limit;
    int   cpu_quota;
    int   network_enabled;
    char  ip_addr[16];
    int   start_count;
} container_t;

static container_t containers[MAX_CONTAINERS];
static int container_mgr_count = 0;
static int container_mgr_initialized = 0;

/* String utilities */
static int my_strlen(const char *s) {
    int len = 0;
    while (s[len]) len++;
    return len;
}

static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) {
        if (*a != *b) return *a - *b;
        a++; b++;
    }
    return *a - *b;
}

static void my_strcpy(char *dst, const char *src) {
    while (*src) *dst++ = *src++;
    *dst = '\0';
}

static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0;
    while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; }
    dst[i] = '\0';
}

static int my_strncmp(const char *a, const char *b, int n) {
    for (int i = 0; i < n; i++) {
        if (!a[i] || !b[i]) return (a[i] ? 1 : 0) - (b[i] ? 1 : 0);
        if (a[i] != b[i]) return a[i] - b[i];
    }
    return 0;
}

/* Print utilities */
static void print_str(const char *str) { host_print(str); }
static void print_int(int val) {
    char buf[32];
    int pos = 0;
    if (val < 0) { buf[pos++] = '-'; val = -val; }
    if (val == 0) buf[pos++] = '0';
    else {
        int d = 0, t = val;
        while (t > 0) { d++; t /= 10; }
        pos += d;
        buf[pos] = '\0'; pos--;
        while (val > 0) { buf[pos--] = '0' + (val % 10); val /= 10; }
    }
    host_print(buf);
}

/* Get state name */
static const char *state_name(int state) {
    if (state == STATUS_CREATED) return "CREATED";
    if (state == STATUS_RUNNING) return "RUNNING";
    if (state == STATUS_STOPPED) return "STOPPED";
    if (state == STATUS_PAUSED) return "PAUSED";
    if (state == STATUS_DESTROYED) return "DESTROYED";
    return "UNKNOWN";
}

/* Create a new container
   name: container name, image: WASM module path, mem_limit: MB
   Returns: container_id or -1 */
int container_mgr_create(const char *name, const char *image, int mem_limit) {
    if (!container_mgr_initialized) {
        container_init();
        container_mgr_initialized = 1;
    }
    if (container_mgr_count >= MAX_CONTAINERS) {
        print_str("[container_mgr] max containers reached\n");
        return -1;
    }

    int id = container_create(name, image, mem_limit);
    if (id < 0) return -1;

    if (id < MAX_CONTAINERS) {
        containers[id].container_id = id;
        my_strncpy(containers[id].name, name, CONTAINER_NAME_LEN - 1);
        my_strncpy(containers[id].image, image, CONTAINER_IMAGE_LEN - 1);
        containers[id].status = STATUS_CREATED;
        containers[id].memory_limit = mem_limit;
        containers[id].cpu_quota = 1000;
        containers[id].network_enabled = 0;
        containers[id].start_count = 0;
        container_mgr_count++;
    }

    print_str("[container_mgr] created container: ");
    print_str(name);
    print_str(" (id=");
    print_int(id);
    print_str(", mem=");
    print_int(mem_limit);
    print_str("MB)\n");

    return id;
}

/* Start a container
   container_id: container to start
   Returns: 0=success, -1=failure */
int container_mgr_start(int container_id) {
    int rc = container_start(container_id);
    if (rc == 0 && container_id < MAX_CONTAINERS) {
        containers[container_id].status = STATUS_RUNNING;
        containers[container_id].start_count++;
    }
    print_str("[container_mgr] started container ");
    print_int(container_id);
    print_str(rc == 0 ? "\n" : " FAILED\n");
    return rc;
}

/* Stop a container
   container_id: container to stop
   Returns: 0=success, -1=failure */
int container_mgr_stop(int container_id) {
    int rc = container_stop(container_id);
    if (rc == 0 && container_id < MAX_CONTAINERS) {
        containers[container_id].status = STATUS_STOPPED;
    }
    print_str("[container_mgr] stopped container ");
    print_int(container_id);
    print_str("\n");
    return rc;
}

/* Execute a command inside a container
   container_id: target container, cmd: module path
   Returns: 0=success, -1=failure */
int container_mgr_exec(int container_id, const char *cmd) {
    int rc = container_exec(container_id, cmd);
    print_str("[container_mgr] exec in container ");
    print_int(container_id);
    print_str(": ");
    print_str(cmd);
    print_str(rc == 0 ? " OK\n" : " FAILED\n");
    return rc;
}

/* Get container logs
   container_id: container, buf: output buffer, len: max length
   Returns: bytes written */
int container_mgr_logs(int container_id, char *buf, int len) {
    if (container_id >= MAX_CONTAINERS) return -1;

    int pos = 0;
    buf[0] = '\0';

    /* Simulate logs */
    const char *name = containers[container_id].name;
    const char *state = state_name(containers[container_id].status);

    /* Build log string manually */
    char tmp[128];
    int tp = 0;
    tmp[tp++] = '['; tmp[tp++] = 'c'; tmp[tp++] = 't'; tmp[tp++] = 'r';
    tmp[tp++] = ']'; tmp[tp++] = ' ';
    for (int i = 0; name[i] && tp < 60; i++) tmp[tp++] = name[i];
    tmp[tp++] = ' ';
    tmp[tp++] = 's'; tmp[tp++] = 't'; tmp[tp++] = 'a'; tmp[tp++] = 't';
    tmp[tp++] = 'e'; tmp[tp++] = '=';
    for (int i = 0; state[i] && tp < 80; i++) tmp[tp++] = state[i];
    tmp[tp++] = '\n';
    tmp[tp] = '\0';

    my_strncpy(buf, tmp, len);
    return tp;
}

/* Get container metrics
   container_id: container
   Returns: 0=success, -1=failure */
int container_mgr_get_metrics(int container_id) {
    if (container_id >= MAX_CONTAINERS) return -1;

    print_str("[metrics] container ");
    print_int(container_id);
    print_str(" (");
    print_str(containers[container_id].name);
    print_str(")\n");
    print_str("  status: ");
    print_str(state_name(containers[container_id].status));
    print_str("\n  memory_limit: ");
    print_int(containers[container_id].memory_limit);
    print_str("MB\n  cpu_quota: ");
    print_int(containers[container_id].cpu_quota);
    print_str(" ticks\n  starts: ");
    print_int(containers[container_id].start_count);
    print_str("\n");

    return 0;
}

/* List all containers */
void container_mgr_list(void) {
    print_str("=== Containers ===\n");
    if (container_mgr_count == 0) {
        print_str("No containers running\n");
        return;
    }

    for (int i = 0; i < MAX_CONTAINERS; i++) {
        if (containers[i].status == STATUS_DESTROYED) continue;
        if (!containers[i].name[0]) continue;

        print_str("  ");
        print_int(i);
        print_str("\t");
        print_str(containers[i].name);
        print_str("\t[");
        print_str(state_name(containers[i].status));
        print_str("] mem=");
        print_int(containers[i].memory_limit);
        print_str("MB\n");
    }
}

/* ===== CLI entry ===== */
void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);

    int help = 0, test = 0, list = 0;
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;
    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0) help = 1;
        else if (my_strcmp(arg, "-t") == 0 || my_strcmp(arg, "--test") == 0) test = 1;
        else if (my_strcmp(arg, "-l") == 0 || my_strcmp(arg, "--list") == 0) list = 1;
        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    print_str("Container Manager v12.0 - WASM Container Lifecycle\n");

    if (help) {
        print_str("Usage: container_mgr [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run container test\n");
        print_str("  -l, --list    List containers\n");
        return;
    }

    if (test) {
        print_str("=== Container Manager Test ===\n");

        print_str("\n--- Create Containers ---\n");
        int c1 = container_mgr_create("web-server", "modules/httpd/httpd.wasm", 8);
        int c2 = container_mgr_create("api-gateway", "modules/api/api.wasm", 16);
        int c3 = container_mgr_create("worker", "modules/worker/worker.wasm", 4);

        print_str("\n--- Start Containers ---\n");
        container_mgr_start(c1);
        container_mgr_start(c2);
        container_mgr_start(c3);

        print_str("\n--- Get Metrics ---\n");
        container_mgr_get_metrics(c1);
        container_mgr_get_metrics(c2);

        print_str("\n--- List Containers ---\n");
        container_mgr_list();

        print_str("\n--- Exec in Container ---\n");
        container_mgr_exec(c1, "modules/cut_v7/cut_v7.wasm");

        print_str("\n--- Stop Containers ---\n");
        container_mgr_stop(c1);
        container_mgr_stop(c2);

        print_str("\n--- Final List ---\n");
        container_mgr_list();

        print_str("\n=== Test Complete ===\n");
        return;
    }

    if (list) {
        container_mgr_list();
        return;
    }

    print_str("Use -h for help, -t for test, -l for list\n");
}
