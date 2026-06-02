/* pod_mgr: Kubernetes-style Pod manager (v12.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Pod states */
#define POD_PENDING    0
#define POD_RUNNING    1
#define POD_SUCCEEDED  2
#define POD_FAILED     3
#define POD_DELETED    4

/* Restart policies */
#define RESTART_ALWAYS     0
#define RESTART_ON_FAILURE 1
#define RESTART_NEVER      2

/* Limits */
#define MAX_PODS        16
#define MAX_POD_CONTAINERS 8
#define POD_NAME_LEN    64
#define NS_LEN          32
#define LABELS_LEN      256
#define MANIFEST_LEN    512

/* Pod definition */
typedef struct {
    int   pod_id;
    char  name[POD_NAME_LEN];
    char  namespace[NS_LEN];
    int   containers[MAX_POD_CONTAINERS];
    int   container_count;
    int   status;
    char  labels[LABELS_LEN];
    int   restart_policy;
    int   restart_count;
    int   created_at;
} pod_t;

static pod_t pods[MAX_PODS];
static int pod_count = 0;
static int next_pod_id = 1;

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

/* Create a pod from a manifest
   name: pod name, manifest: YAML-style definition
   Returns: pod_id or -1 */
int pod_create(const char *name, const char *manifest) {
    if (pod_count >= MAX_PODS) {
        print_str("[pod_mgr] max pods reached\n");
        return -1;
    }

    int idx = pod_count;
    pods[idx].pod_id = next_pod_id;
    my_strncpy(pods[idx].name, name, POD_NAME_LEN - 1);
    my_strcpy(pods[idx].namespace, "default");
    pods[idx].container_count = 0;
    pods[idx].status = POD_PENDING;
    pods[idx].labels[0] = '\0';
    pods[idx].restart_policy = RESTART_ALWAYS;
    pods[idx].restart_count = 0;
    pods[idx].created_at = 0;

    /* Parse manifest for containers (simple format: "img1,img2,img3") */
    if (manifest && manifest[0]) {
        int cc = 0;
        int img_start = 0;
        for (int i = 0; manifest[i] && cc < MAX_POD_CONTAINERS; i++) {
            if (manifest[i] == ',' || manifest[i] == '|') {
                /* Create container from image */
                char img[128];
                int len = i - img_start;
                if (len > 0 && len < 127) {
                    int j = 0;
                    while (j < len) { img[j] = manifest[img_start + j]; j++; }
                    img[len] = '\0';
                    pods[idx].containers[cc] = cc; /* simplified */
                    pods[idx].container_count++;
                    cc++;
                }
                img_start = i + 1;
            }
        }
        /* Last image */
        if (img_start < my_strlen(manifest) && cc < MAX_POD_CONTAINERS) {
            pods[idx].containers[cc] = cc;
            pods[idx].container_count++;
        }
    }

    /* Parse labels from manifest (after ':') */
    int mlen = my_strlen(manifest);
    for (int i = 0; i < mlen; i++) {
        if (manifest[i] == ':') {
            my_strncpy(pods[idx].labels, manifest + i + 1, LABELS_LEN - 1);
            break;
        }
    }

    pod_count++;
    int id = next_pod_id++;

    print_str("[pod_mgr] created pod: ");
    print_str(name);
    print_str(" (id=");
    print_int(id);
    print_str(", containers=");
    print_int(pods[idx].container_count);
    print_str(")\n");

    return id;
}

/* Start a pod
   pod_id: pod to start
   Returns: 0=success, -1=failure */
int pod_start(int pod_id) {
    for (int i = 0; i < pod_count; i++) {
        if (pods[i].pod_id == pod_id) {
            if (pods[i].status == POD_PENDING || pods[i].status == POD_FAILED) {
                pods[i].status = POD_RUNNING;
                print_str("[pod_mgr] started pod: ");
                print_str(pods[i].name);
                print_str("\n");
                return 0;
            }
            print_str("[pod_mgr] cannot start pod (state=");
            print_int(pods[i].status);
            print_str(")\n");
            return -1;
        }
    }
    print_str("[pod_mgr] pod not found: ");
    print_int(pod_id);
    print_str("\n");
    return -1;
}

/* Delete a pod
   pod_id: pod to delete
   Returns: 0=success, -1=failure */
int pod_delete(int pod_id) {
    for (int i = 0; i < pod_count; i++) {
        if (pods[i].pod_id == pod_id) {
            pods[i].status = POD_DELETED;
            print_str("[pod_mgr] deleted pod: ");
            print_str(pods[i].name);
            print_str("\n");
            return 0;
        }
    }
    return -1;
}

/* Get pod status
   pod_id: pod to query
   Returns: status code */
int pod_get_status(int pod_id) {
    for (int i = 0; i < pod_count; i++) {
        if (pods[i].pod_id == pod_id) {
            return pods[i].status;
        }
    }
    return -1;
}

/* Scale a pod's containers
   pod_id: pod, replicas: number of container replicas
   Returns: 0=success, -1=failure */
int pod_scale(int pod_id, int replicas) {
    for (int i = 0; i < pod_count; i++) {
        if (pods[i].pod_id == pod_id) {
            print_str("[pod_mgr] scaling pod: ");
            print_str(pods[i].name);
            print_str(" to ");
            print_int(replicas);
            print_str(" replicas\n");
            return 0;
        }
    }
    return -1;
}

/* Execute command in a pod container
   pod_id: pod, container: container name, cmd: command
   Returns: 0=success, -1=failure */
int pod_exec(int pod_id, const char *container, const char *cmd) {
    for (int i = 0; i < pod_count; i++) {
        if (pods[i].pod_id == pod_id) {
            print_str("[pod_mgr] exec '");
            print_str(cmd);
            print_str("' in container '");
            print_str(container);
            print_str("' of pod '");
            print_str(pods[i].name);
            print_str("'\n");
            return 0;
        }
    }
    return -1;
}

/* List all pods */
void pod_list(void) {
    print_str("=== Pods ===\n");
    print_str("NAME\t\tNAMESPACE\tSTATUS\tCONTAINERS\n");

    for (int i = 0; i < pod_count; i++) {
        if (pods[i].status == POD_DELETED) continue;
        print_str("  ");
        print_str(pods[i].name);
        print_str("\t");
        print_str(pods[i].namespace);
        print_str("\t");
        const char *state = pods[i].status == POD_PENDING ? "Pending" :
                           pods[i].status == POD_RUNNING ? "Running" :
                           pods[i].status == POD_SUCCEEDED ? "Succeeded" :
                           pods[i].status == POD_FAILED ? "Failed" : "Deleted";
        print_str(state);
        print_str("\t");
        print_int(pods[i].container_count);
        print_str("\n");
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

    print_str("Pod Manager v12.0 - Kubernetes-style Pod Abstraction\n");

    if (help) {
        print_str("Usage: pod_mgr [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run pod test\n");
        print_str("  -l, --list    List pods\n");
        return;
    }

    if (test) {
        print_str("=== Pod Manager Test ===\n");

        print_str("\n--- Create Pods ---\n");
        int p1 = pod_create("web-pod", "nginx,sidecar-proxy:app=web,tier=frontend");
        int p2 = pod_create("worker-pod", "python-worker:app=worker,tier=backend");
        int p3 = pod_create("redis-pod", "redis:app=cache,tier=data");

        print_str("\n--- Start Pods ---\n");
        pod_start(p1);
        pod_start(p2);
        pod_start(p3);

        print_str("\n--- Get Status ---\n");
        int s1 = pod_get_status(p1);
        print_str("web-pod status: ");
        print_int(s1);
        print_str("\n");

        print_str("\n--- Scale ---\n");
        pod_scale(p2, 3);

        print_str("\n--- Exec ---\n");
        pod_exec(p1, "nginx", "curl localhost:80");

        print_str("\n--- List Pods ---\n");
        pod_list();

        print_str("\n--- Delete Pod ---\n");
        pod_delete(p3);

        print_str("\n--- Final List ---\n");
        pod_list();

        print_str("\n=== Test Complete ===\n");
        return;
    }

    if (list) {
        pod_list();
        return;
    }

    print_str("Use -h for help, -t for test, -l for list\n");
}
