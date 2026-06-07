/* wasm_thread: Wasm3 multi-threading support module (v46.0)
 *
 * Provides WebAssembly threads proposal support including:
 * - Thread creation and lifecycle management
 * - Mutex/lock synchronization primitives
 * - Condition variables
 * - Thread-local storage abstraction
 * - Thread pool for work distribution
 */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_THREADS       64
#define MAX_THREAD_NAME   32
#define MAX_MSG           256
#define MAX_POOL_SIZE     16
#define MAX_QUEUE_SIZE    128
#define MAX_TLS_KEYS      32

/* Thread states */
#define THREAD_CREATED    0
#define THREAD_RUNNING    1
#define THREAD_BLOCKED    2
#define THREAD_TERMINATED 3
#define THREAD_JOINABLE   4

/* Thread priorities */
#define PRIO_LOW      0
#define PRIO_NORMAL   1
#define PRIO_HIGH     2
#define PRIO_REALTIME 3

/* Mutex types */
#define MUTEX_NORMAL      0
#define MUTEX_RECURSIVE   1
#define MUTEX_ERRORCHECK  2

/* Thread function pointer type (stored as ID for WASM) */
typedef unsigned int thread_func_id;

/* Thread control block */
typedef struct {
    int             thread_id;
    char            name[MAX_THREAD_NAME];
    thread_func_id  entry_func;
    unsigned int    stack_ptr;
    unsigned int    stack_size;
    int             state;
    int             priority;
    int             joinable;
    int             joined;
    unsigned int    result;
    int             parent_id;
    int             tls[MAX_TLS_KEYS];
    int             created_at;
    int             started_at;
    int             terminated_at;
} thread_t;

/* Mutex */
typedef struct {
    int     mutex_id;
    int     type;
    int     locked;
    int     owner_id;
    int     lock_count;       /* for recursive mutexes */
    int     wait_count;
    int     created_at;
} mutex_t;

/* Condition variable */
typedef struct {
    int     cond_id;
    int     wait_count;
    int     signal_count;
    int     created_at;
} cond_t;

/* Thread-local storage key */
typedef struct {
    int     key_id;
    int     allocated;
    void    (*destructor)(void *);
} tls_key_t;

/* Work item for thread pool */
typedef struct {
    thread_func_id  func;
    unsigned int    arg;
    int             status;     /* 0=pending, 1=running, 2=done */
    unsigned int    result;
} work_item_t;

/* Thread pool */
typedef struct {
    int         pool_size;
    int         active_count;
    int         queue_head;
    int         queue_tail;
    int         queue_size;
    work_item_t queue[MAX_QUEUE_SIZE];
    int         completed;
    int         created_at;
} thread_pool_t;

/* Global state */
static thread_t threads[MAX_THREADS];
static mutex_t mutexes[MAX_THREADS];
static cond_t conditions[MAX_THREADS];
static tls_key_t tls_keys[MAX_TLS_KEYS];
static thread_pool_t pool;

static int thread_count = 0;
static int next_thread_id = 1;
static int mutex_count = 0;
static int next_mutex_id = 1;
static int cond_count = 0;
static int next_cond_id = 1;
static int tls_key_count = 0;
static int next_tls_key_id = 1;
static int clock = 0;

/* String utilities */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
}
static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0; while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; } dst[i] = '\0';
}
static int my_strstr(const char *haystack, const char *needle) {
    int hl = my_strlen(haystack), nl = my_strlen(needle);
    if (nl == 0 || nl > hl) return 0;
    for (int i = 0; i <= hl - nl; i++) {
        int j = 0; while (j < nl && haystack[i+j] == needle[j]) j++;
        if (j == nl) return 1;
    }
    return 0;
}

static void print_str(const char *str) { host_print(str); }
static void print_int(int val) {
    char buf[32]; int pos = 0;
    if (val < 0) { buf[pos++] = '-'; val = -val; }
    if (val == 0) buf[pos++] = '0';
    else { int d = 0, t = val; while (t > 0) { d++; t /= 10; } pos += d; buf[pos] = '\0'; pos--;
        while (val > 0) { buf[pos--] = '0' + (val % 10); val /= 10; } }
    host_print(buf);
}

/* ===== Thread Management ===== */

/* Create a new thread */
int thread_create(const char *name, thread_func_id func, unsigned int stack_size, int priority, int joinable) {
    if (thread_count >= MAX_THREADS) return -1;

    int idx = -1;
    for (int i = 0; i < MAX_THREADS; i++) {
        if (threads[i].state == THREAD_TERMINATED || threads[i].thread_id == 0) {
            idx = i; break;
        }
    }
    if (idx < 0) return -1;

    int tid = next_thread_id++;
    threads[idx].thread_id = tid;
    my_strncpy(threads[idx].name, name ? name : "unnamed", MAX_THREAD_NAME - 1);
    threads[idx].entry_func = func;
    threads[idx].stack_size = stack_size > 0 ? stack_size : 4096;
    threads[idx].stack_ptr = host_alloc(threads[idx].stack_size, 16);
    threads[idx].state = THREAD_CREATED;
    threads[idx].priority = priority < 0 ? PRIO_NORMAL : (priority > PRIO_REALTIME ? PRIO_REALTIME : priority);
    threads[idx].joinable = joinable;
    threads[idx].joined = 0;
    threads[idx].result = 0;
    threads[idx].parent_id = 0;
    threads[idx].created_at = ++clock;
    threads[idx].started_at = 0;
    threads[idx].terminated_at = 0;
    for (int k = 0; k < MAX_TLS_KEYS; k++) threads[idx].tls[k] = 0;

    thread_count++;
    return tid;
}

/* Start a thread (transition to running) */
int thread_start(int thread_id) {
    for (int i = 0; i < MAX_THREADS; i++) {
        if (threads[i].thread_id == thread_id) {
            if (threads[i].state != THREAD_CREATED) return -1;
            threads[i].state = THREAD_RUNNING;
            threads[i].started_at = ++clock;
            return 0;
        }
    }
    return -1;
}

/* Get thread by ID */
static thread_t *find_thread(int thread_id) {
    for (int i = 0; i < MAX_THREADS; i++) {
        if (threads[i].thread_id == thread_id) return &threads[i];
    }
    return 0;
}

/* Block a thread */
int thread_block(int thread_id) {
    thread_t *t = find_thread(thread_id);
    if (!t || t->state != THREAD_RUNNING) return -1;
    t->state = THREAD_BLOCKED;
    return 0;
}

/* Unblock a thread */
int thread_unblock(int thread_id) {
    thread_t *t = find_thread(thread_id);
    if (!t || t->state != THREAD_BLOCKED) return -1;
    t->state = THREAD_RUNNING;
    return 0;
}

/* Terminate a thread */
int thread_terminate(int thread_id, unsigned int result) {
    thread_t *t = find_thread(thread_id);
    if (!t) return -1;
    t->state = t->joinable ? THREAD_JOINABLE : THREAD_TERMINATED;
    t->result = result;
    t->terminated_at = ++clock;
    if (!t->joinable) {
        /* Clean up non-joinable thread immediately */
        t->thread_id = 0;
        t->name[0] = '\0';
        thread_count--;
    }
    return 0;
}

/* Join a thread */
int thread_join(int thread_id, unsigned int *result) {
    thread_t *t = find_thread(thread_id);
    if (!t || !t->joinable || t->joined) return -1;
    if (t->state != THREAD_JOINABLE) return -1;
    if (result) *result = t->result;
    t->joined = 1;
    t->state = THREAD_TERMINATED;
    t->thread_id = 0;
    t->name[0] = '\0';
    thread_count--;
    return 0;
}

/* Set thread priority */
int thread_set_priority(int thread_id, int priority) {
    thread_t *t = find_thread(thread_id);
    if (!t) return -1;
    if (priority < PRIO_LOW || priority > PRIO_REALTIME) return -1;
    t->priority = priority;
    return 0;
}

/* Get thread state */
int thread_get_state(int thread_id) {
    thread_t *t = find_thread(thread_id);
    if (!t) return -1;
    return t->state;
}

/* Get thread info */
int thread_get_info(int thread_id, char *name_buf, int *state, int *priority) {
    thread_t *t = find_thread(thread_id);
    if (!t) return -1;
    if (name_buf) my_strncpy(name_buf, t->name, MAX_THREAD_NAME - 1);
    if (state) *state = t->state;
    if (priority) *priority = t->priority;
    return 0;
}

/* List all threads */
int thread_list(void) {
    int count = 0;
    print_str("  Thread List\n");
    print_str("  ==================================================================\n");
    for (int i = 0; i < MAX_THREADS; i++) {
        if (threads[i].thread_id != 0) {
            const char *state_str = "UNKNOWN";
            if (threads[i].state == THREAD_CREATED) state_str = "CREATED";
            else if (threads[i].state == THREAD_RUNNING) state_str = "RUNNING";
            else if (threads[i].state == THREAD_BLOCKED) state_str = "BLOCKED";
            else if (threads[i].state == THREAD_JOINABLE) state_str = "JOINABLE";
            else if (threads[i].state == THREAD_TERMINATED) state_str = "TERMINATED";

            print_str("  TID "); print_int(threads[i].thread_id);
            print_str(" ["); print_str(threads[i].name); print_str("] ");
            print_str(state_str);
            print_str(" prio="); print_int(threads[i].priority);
            print_str(" stack="); print_int(threads[i].stack_size);
            print_str("\n");
            count++;
        }
    }
    print_str("  Total: "); print_int(count); print_str(" threads\n");
    return count;
}

/* ===== Mutex Operations ===== */

/* Create a mutex */
int mutex_create(int type) {
    if (mutex_count >= MAX_THREADS) return -1;
    int idx = -1;
    for (int i = 0; i < MAX_THREADS; i++) {
        if (mutexes[i].mutex_id == 0) { idx = i; break; }
    }
    if (idx < 0) return -1;

    int mid = next_mutex_id++;
    mutexes[idx].mutex_id = mid;
    mutexes[idx].type = type;
    mutexes[idx].locked = 0;
    mutexes[idx].owner_id = 0;
    mutexes[idx].lock_count = 0;
    mutexes[idx].wait_count = 0;
    mutexes[idx].created_at = ++clock;
    mutex_count++;
    return mid;
}

/* Lock a mutex */
int mutex_lock(int mutex_id, int thread_id) {
    for (int i = 0; i < MAX_THREADS; i++) {
        if (mutexes[i].mutex_id == mutex_id) {
            mutex_t *m = &mutexes[i];
            if (m->type == MUTEX_RECURSIVE && m->owner_id == thread_id) {
                m->lock_count++;
                return 0;
            }
            if (m->locked) {
                m->wait_count++;
                thread_block(thread_id);
                return -2; /* would block */
            }
            m->locked = 1;
            m->owner_id = thread_id;
            m->lock_count = 1;
            return 0;
        }
    }
    return -1;
}

/* Unlock a mutex */
int mutex_unlock(int mutex_id, int thread_id) {
    for (int i = 0; i < MAX_THREADS; i++) {
        if (mutexes[i].mutex_id == mutex_id) {
            mutex_t *m = &mutexes[i];
            if (m->owner_id != thread_id) {
                if (m->type == MUTEX_ERRORCHECK) return -1;
                return -1;
            }
            if (m->type == MUTEX_RECURSIVE) {
                m->lock_count--;
                if (m->lock_count > 0) return 0;
            }
            m->locked = 0;
            m->owner_id = 0;
            m->lock_count = 0;
            if (m->wait_count > 0) m->wait_count--;
            return 0;
        }
    }
    return -1;
}

/* Try lock a mutex (non-blocking) */
int mutex_trylock(int mutex_id, int thread_id) {
    for (int i = 0; i < MAX_THREADS; i++) {
        if (mutexes[i].mutex_id == mutex_id) {
            mutex_t *m = &mutexes[i];
            if (m->locked) return -1; /* already locked */
            if (m->type == MUTEX_RECURSIVE && m->owner_id == thread_id) {
                m->lock_count++;
                return 0;
            }
            m->locked = 1;
            m->owner_id = thread_id;
            m->lock_count = 1;
            return 0;
        }
    }
    return -1;
}

/* Destroy a mutex */
int mutex_destroy(int mutex_id) {
    for (int i = 0; i < MAX_THREADS; i++) {
        if (mutexes[i].mutex_id == mutex_id) {
            mutexes[i].mutex_id = 0;
            mutex_count--;
            return 0;
        }
    }
    return -1;
}

/* ===== Condition Variables ===== */

/* Create a condition variable */
int cond_create(void) {
    if (cond_count >= MAX_THREADS) return -1;
    int idx = -1;
    for (int i = 0; i < MAX_THREADS; i++) {
        if (conditions[i].cond_id == 0) { idx = i; break; }
    }
    if (idx < 0) return -1;

    int cid = next_cond_id++;
    conditions[idx].cond_id = cid;
    conditions[idx].wait_count = 0;
    conditions[idx].signal_count = 0;
    conditions[idx].created_at = ++clock;
    cond_count++;
    return cid;
}

/* Wait on a condition variable (releases mutex, then re-acquires) */
int cond_wait(int cond_id, int mutex_id, int thread_id) {
    for (int i = 0; i < MAX_THREADS; i++) {
        if (conditions[i].cond_id == cond_id) {
            conditions[i].wait_count++;
            mutex_unlock(mutex_id, thread_id);
            thread_block(thread_id);
            return 0;
        }
    }
    return -1;
}

/* Signal a condition variable */
int cond_signal(int cond_id) {
    for (int i = 0; i < MAX_THREADS; i++) {
        if (conditions[i].cond_id == cond_id) {
            if (conditions[i].wait_count > 0) {
                conditions[i].wait_count--;
                conditions[i].signal_count++;
            }
            return 0;
        }
    }
    return -1;
}

/* Broadcast a condition variable */
int cond_broadcast(int cond_id) {
    for (int i = 0; i < MAX_THREADS; i++) {
        if (conditions[i].cond_id == cond_id) {
            int w = conditions[i].wait_count;
            conditions[i].wait_count = 0;
            conditions[i].signal_count += w;
            return w;
        }
    }
    return -1;
}

/* Destroy a condition variable */
int cond_destroy(int cond_id) {
    for (int i = 0; i < MAX_THREADS; i++) {
        if (conditions[i].cond_id == cond_id) {
            conditions[i].cond_id = 0;
            cond_count--;
            return 0;
        }
    }
    return -1;
}

/* ===== Thread-Local Storage ===== */

/* Create a TLS key */
int tls_create_key(void (*destructor)(void *)) {
    if (tls_key_count >= MAX_TLS_KEYS) return -1;
    int idx = -1;
    for (int i = 0; i < MAX_TLS_KEYS; i++) {
        if (!tls_keys[i].allocated) { idx = i; break; }
    }
    if (idx < 0) return -1;

    int kid = next_tls_key_id++;
    tls_keys[idx].key_id = kid;
    tls_keys[idx].allocated = 1;
    tls_keys[idx].destructor = destructor;
    tls_key_count++;
    return kid;
}

/* Set TLS value for a thread */
int tls_set(int thread_id, int key_id, unsigned int value) {
    thread_t *t = find_thread(thread_id);
    if (!t) return -1;
    for (int i = 0; i < MAX_TLS_KEYS; i++) {
        if (tls_keys[i].key_id == key_id && tls_keys[i].allocated) {
            /* Map key to TLS slot index */
            int slot = key_id - 1;
            if (slot >= 0 && slot < MAX_TLS_KEYS) {
                t->tls[slot] = (int)value;
                return 0;
            }
        }
    }
    return -1;
}

/* Get TLS value for a thread */
unsigned int tls_get(int thread_id, int key_id) {
    thread_t *t = find_thread(thread_id);
    if (!t) return 0;
    for (int i = 0; i < MAX_TLS_KEYS; i++) {
        if (tls_keys[i].key_id == key_id && tls_keys[i].allocated) {
            int slot = key_id - 1;
            if (slot >= 0 && slot < MAX_TLS_KEYS) {
                return (unsigned int)t->tls[slot];
            }
        }
    }
    return 0;
}

/* Delete a TLS key */
int tls_delete_key(int key_id) {
    for (int i = 0; i < MAX_TLS_KEYS; i++) {
        if (tls_keys[i].key_id == key_id) {
            tls_keys[i].allocated = 0;
            tls_keys[i].destructor = 0;
            tls_key_count--;
            return 0;
        }
    }
    return -1;
}

/* ===== Thread Pool ===== */

/* Initialize thread pool */
int pool_init(int pool_size, int queue_size) {
    pool.pool_size = pool_size < MAX_POOL_SIZE ? pool_size : MAX_POOL_SIZE;
    pool.queue_size = queue_size < MAX_QUEUE_SIZE ? queue_size : MAX_QUEUE_SIZE;
    pool.queue_head = 0;
    pool.queue_tail = 0;
    pool.active_count = 0;
    pool.completed = 0;
    pool.created_at = ++clock;
    for (int i = 0; i < pool.queue_size; i++) {
        pool.queue[i].func = 0;
        pool.queue[i].arg = 0;
        pool.queue[i].status = 0;
        pool.queue[i].result = 0;
    }
    return 0;
}

/* Submit work to pool */
int pool_submit(thread_func_id func, unsigned int arg) {
    int next = (pool.queue_tail + 1) % pool.queue_size;
    if (next == pool.queue_head) return -1; /* queue full */
    pool.queue[pool.queue_tail].func = func;
    pool.queue[pool.queue_tail].arg = arg;
    pool.queue[pool.queue_tail].status = 0; /* pending */
    pool.queue[pool.queue_tail].result = 0;
    pool.queue_tail = next;
    return 0;
}

/* Dequeue work item */
int pool_dequeue(int *func, unsigned int *arg) {
    if (pool.queue_head == pool.queue_tail) return -1; /* queue empty */
    pool.queue[pool.queue_head].status = 1; /* running */
    if (func) *func = (int)pool.queue[pool.queue_head].func;
    if (arg) *arg = pool.queue[pool.queue_head].arg;
    pool.queue_head = (pool.queue_head + 1) % pool.queue_size;
    pool.active_count++;
    return 0;
}

/* Complete work item */
int pool_complete(unsigned int result) {
    pool.active_count--;
    pool.completed++;
    return 0;
}

/* Get pool stats */
int pool_stats(int *active, int *completed, int *pending) {
    int pending_count = 0;
    int h = pool.queue_head;
    while (h != pool.queue_tail) {
        if (pool.queue[h].status == 0) pending_count++;
        h = (h + 1) % pool.queue_size;
    }
    if (active) *active = pool.active_count;
    if (completed) *completed = pool.completed;
    if (pending) *pending = pending_count;
    return 0;
}

/* ===== CLI entry ===== */
void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);
    int help = 0, test = 0;
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0) help = 1;
        else if (my_strcmp(arg, "-t") == 0 || my_strcmp(arg, "--test") == 0) test = 1;
        while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    }

    print_str("WasmThread v46.0 - Wasm3 Multi-Threading Support\n");
    if (help) {
        print_str("Usage: wasm_thread [options]\n");
        print_str("  -h, --help    Show help\n");
        print_str("  -t, --test    Run thread tests\n");
        return;
    }

    if (test) {
        print_str("=== WasmThread Test Suite ===\n\n");

        /* Test 1: Thread creation */
        print_str("[Test 1] Thread Creation\n");
        int t1 = thread_create("worker-1", 0x1000, 8192, PRIO_NORMAL, 1);
        int t2 = thread_create("worker-2", 0x2000, 8192, PRIO_HIGH, 1);
        int t3 = thread_create("worker-3", 0x3000, 4096, PRIO_LOW, 0);
        print_str("  Created threads: ");
        print_int(t1); print_str(", ");
        print_int(t2); print_str(", ");
        print_int(t3); print_str("\n");
        thread_start(t1);
        thread_start(t2);
        thread_start(t3);
        print_str("  All threads started\n\n");

        /* Test 2: Thread listing */
        print_str("[Test 2] Thread Listing\n");
        thread_list();
        print_str("\n");

        /* Test 3: Mutex operations */
        print_str("[Test 3] Mutex Operations\n");
        int m1 = mutex_create(MUTEX_NORMAL);
        int m2 = mutex_create(MUTEX_RECURSIVE);
        int m3 = mutex_create(MUTEX_ERRORCHECK);
        print_str("  Created mutexes: ");
        print_int(m1); print_str(", ");
        print_int(m2); print_str(", ");
        print_int(m3); print_str("\n");

        int lock_result = mutex_lock(m1, t1);
        print_str("  Thread "); print_int(t1);
        print_str(" locked mutex "); print_int(m1);
        print_str(" (result="); print_int(lock_result); print_str(")\n");

        lock_result = mutex_trylock(m1, t2);
        print_str("  Thread "); print_int(t2);
        print_str(" trylock mutex "); print_int(m1);
        print_str(" (result="); print_int(lock_result); print_str(" - expected -1)\n");

        mutex_unlock(m1, t1);
        print_str("  Thread "); print_int(t1);
        print_str(" unlocked mutex "); print_int(m1); print_str("\n");

        lock_result = mutex_trylock(m1, t2);
        print_str("  Thread "); print_int(t2);
        print_str(" trylock mutex "); print_int(m1);
        print_str(" (result="); print_int(lock_result); print_str(" - expected 0)\n");
        mutex_unlock(m1, t2);
        print_str("\n");

        /* Test 4: Condition variables */
        print_str("[Test 4] Condition Variables\n");
        int c1 = cond_create();
        print_str("  Created cond var: "); print_int(c1); print_str("\n");
        mutex_lock(m1, t1);
        print_str("  Thread "); print_int(t1);
        print_str(" waiting on cond "); print_int(c1); print_str(" (would block)\n");
        cond_signal(c1);
        mutex_unlock(m1, t1);
        print_str("\n");

        /* Test 5: Thread-local storage */
        print_str("[Test 5] Thread-Local Storage\n");
        int k1 = tls_create_key(0);
        int k2 = tls_create_key(0);
        print_str("  Created TLS keys: ");
        print_int(k1); print_str(", "); print_int(k2); print_str("\n");

        tls_set(t1, k1, 0x1234);
        tls_set(t1, k2, 0x5678);
        tls_set(t2, k1, 0xABCD);

        unsigned int v1 = tls_get(t1, k1);
        unsigned int v2 = tls_get(t1, k2);
        unsigned int v3 = tls_get(t2, k1);
        print_str("  TLS[T"); print_int(t1); print_str("][K"); print_int(k1);
        print_str("]="); print_int((int)v1);
        print_str(" TLS[T"); print_int(t1); print_str("][K"); print_int(k2);
        print_str("]="); print_int((int)v2);
        print_str(" TLS[T"); print_int(t2); print_str("][K"); print_int(k1);
        print_str("]="); print_int((int)v3); print_str("\n\n");

        /* Test 6: Thread pool */
        print_str("[Test 6] Thread Pool\n");
        pool_init(4, 16);
        print_str("  Pool initialized (size=4, queue=16)\n");
        pool_submit(0x100, 100);
        pool_submit(0x200, 200);
        pool_submit(0x300, 300);
        print_str("  Submitted 3 work items\n");

        int active = 0, completed = 0, pending = 0;
        pool_stats(&active, &completed, &pending);
        print_str("  Stats: active="); print_int(active);
        print_str(" completed="); print_int(completed);
        print_str(" pending="); print_int(pending); print_str("\n");

        int func = 0;
        unsigned int arg = 0;
        pool_dequeue(&func, &arg);
        print_str("  Dequeued: func="); print_int(func);
        print_str(" arg="); print_int((int)arg); print_str("\n");
        pool_complete(42);
        pool_stats(&active, &completed, &pending);
        print_str("  After complete: active="); print_int(active);
        print_str(" completed="); print_int(completed); print_str("\n\n");

        /* Test 7: Thread termination */
        print_str("[Test 7] Thread Termination\n");
        thread_terminate(t3, 100);
        print_str("  Thread "); print_int(t3);
        print_str(" terminated (non-joinable, cleaned up)\n");

        thread_terminate(t1, 200);
        unsigned int result = 0;
        thread_join(t1, &result);
        print_str("  Thread "); print_int(t1);
        print_str(" joined (result="); print_int((int)result); print_str(")\n");

        thread_list();
        print_str("\n");

        /* Cleanup */
        mutex_destroy(m1);
        mutex_destroy(m2);
        mutex_destroy(m3);
        cond_destroy(c1);
        tls_delete_key(k1);
        tls_delete_key(k2);

        print_str("=== WasmThread Test Complete ===\n");
        return;
    }

    print_str("Use -h for help, -t for test\n");
}
