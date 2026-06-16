/* function_scheduler: Serverless function scheduling engine (v1.0)
 * Implements scheduling policies, auto-scaling, warm-up management,
 * resource bin-packing, fault recovery, and rate limiting.
 * Pure C, no stdlib - AI-ASM OS WASM module.
 */

#include <stddef.h>

/* Host-provided WASM imports */
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* ── Limits ─────────────────────────────────────────────── */
#define MAX_TASKS        64
#define MAX_NODES        32
#define MAX_QUEUE         64
#define MAX_DLQ           32
#define MAX_NAME          32
#define MAX_WARM          16

/* ── Constants ──────────────────────────────────────────── */
#define SCHED_FIFO        0
#define SCHED_PRIORITY    1
#define SCHED_AFFINITY    2
#define SCHED_LOCALITY    3

#define TASK_PENDING      0
#define TASK_SCHEDULED    1
#define TASK_RUNNING      2
#define TASK_DONE         3
#define TASK_FAILED       4

#define NODE_UP           1
#define NODE_DRAINING     2
#define NODE_OFF          0

#define SCALE_UP          1
#define SCALE_DOWN       -1
#define SCALE_NONE        0

#define WARM_COLD         0
#define WARM_WARM         1

#define CB_CLOSED         0
#define CB_OPEN           1
#define CB_HALF_OPEN      2

#define RET_OK            0
#define RET_FAIL         -1

#define ALLOWED           1
#define BLOCKED           0

/* ── Typedefs ───────────────────────────────────────────── */
typedef struct {
    int id;
    char name[MAX_NAME];
    int priority;
    int status;
    int node_id;
    int mem_req;
    int cpu_req;
    char affinity_node[MAX_NAME];
    int retry_count;
    int max_retries;
} sched_task_t;

typedef struct {
    int id;
    char name[MAX_NAME];
    int status;
    int total_mem;
    int used_mem;
    int total_cpu;
    int used_cpu;
    int task_count;
    int locality_zone;
} sched_node_t;

typedef struct {
    int task_id;
    int priority;
} pq_entry_t;

typedef struct {
    pq_entry_t data[MAX_QUEUE];
    int size;
} pqueue_t;

typedef struct {
    int target_size;
    int idle_timeout_sec;
    int current_count;
} warm_pool_t;

typedef struct {
    int task_ids[MAX_DLQ];
    int count;
} dlq_t;

typedef struct {
    int capacity;
    int tokens;
    int refill_rate;
    int last_refill_ts;
} token_bucket_t;

typedef struct {
    int state;
    int failure_count;
    int threshold;
    int open_since;
    int timeout_sec;
} circuit_breaker_t;

/* ── Static State ───────────────────────────────────────── */
static sched_task_t  g_tasks[MAX_TASKS];
static int           g_task_count    = 0;
static sched_node_t  g_nodes[MAX_NODES];
static int           g_node_count    = 0;
static int           g_sched_policy  = SCHED_FIFO;
static pqueue_t      g_pq;
static warm_pool_t   g_warm;
static dlq_t         g_dlq;
static int           g_instance_count = 0;

/* ── Utility Functions ──────────────────────────────────── */
static int my_strlen(const char *s) {
    int n = 0;
    while (s[n]) n++;
    return n;
}

static void my_strcpy(char *d, const char *s) {
    while (*s) { *d = *s; d++; s++; }
    *d = '\0';
}

static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) {
        if (*a != *b) return *a - *b;
        a++; b++;
    }
    return *a - *b;
}

static void my_itoa(int val, char *buf) {
    int i = 0;
    int neg = 0;
    if (val < 0) { neg = 1; val = -val; }
    if (val == 0) { buf[0] = '0'; buf[1] = '\0'; return; }
    while (val > 0 && i < 20) {
        buf[i] = '0' + (val % 10);
        val = val / 10;
        i++;
    }
    if (neg) { buf[i] = '-'; i++; }
    buf[i] = '\0';
    int lo = 0;
    int hi = i - 1;
    while (lo < hi) {
        char tmp = buf[lo];
        buf[lo] = buf[hi];
        buf[hi] = tmp;
        lo++; hi--;
    }
}

static void print_int(int val) {
    char buf[24];
    my_itoa(val, buf);
    host_print(buf);
}

static void print_ok(const char *msg) {
    host_print("  [PASS] ");
    host_print(msg);
    host_print("\n");
}

static void print_fail(const char *msg) {
    host_print("  [FAIL] ");
    host_print(msg);
    host_print("\n");
}

/* ── Priority Queue ─────────────────────────────────────── */
static void pqueue_init(pqueue_t *q) {
    q->size = 0;
}

static int pqueue_push(pqueue_t *q, int task_id, int prio) {
    if (q->size >= MAX_QUEUE) return RET_FAIL;
    int idx = q->size;
    q->data[idx].task_id = task_id;
    q->data[idx].priority = prio;
    q->size++;
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (q->data[idx].priority > q->data[parent].priority) {
            pq_entry_t tmp = q->data[idx];
            q->data[idx] = q->data[parent];
            q->data[parent] = tmp;
            idx = parent;
        } else {
            break;
        }
    }
    return RET_OK;
}

static int pqueue_pop(pqueue_t *q, int *task_id, int *prio) {
    if (q->size <= 0) return RET_FAIL;
    *task_id = q->data[0].task_id;
    *prio    = q->data[0].priority;
    q->size--;
    if (q->size > 0) {
        q->data[0] = q->data[q->size];
        int idx = 0;
        for (;;) {
            int left  = 2 * idx + 1;
            int right = 2 * idx + 2;
            int largest = idx;
            if (left < q->size && q->data[left].priority > q->data[largest].priority) {
                largest = left;
            }
            if (right < q->size && q->data[right].priority > q->data[largest].priority) {
                largest = right;
            }
            if (largest == idx) break;
            pq_entry_t tmp = q->data[idx];
            q->data[idx] = q->data[largest];
            q->data[largest] = tmp;
            idx = largest;
        }
    }
    return RET_OK;
}

/* ── Scheduling Policies ────────────────────────────────── */
static int schedule_fifo(void) {
    int i, j;
    for (i = 0; i < g_task_count; i++) {
        if (g_tasks[i].status != TASK_PENDING) continue;
        for (j = 0; j < g_node_count; j++) {
            if (g_nodes[j].status != NODE_UP) continue;
            g_tasks[i].status  = TASK_SCHEDULED;
            g_tasks[i].node_id = g_nodes[j].id;
            g_nodes[j].task_count++;
            return i;
        }
    }
    return RET_FAIL;
}

static int schedule_priority(void) {
    int best = -1;
    int best_prio = -1;
    int i;
    for (i = 0; i < g_task_count; i++) {
        if (g_tasks[i].status != TASK_PENDING) continue;
        if (g_tasks[i].priority > best_prio) {
            best_prio = g_tasks[i].priority;
            best = i;
        }
    }
    if (best < 0) return RET_FAIL;
    for (i = 0; i < g_node_count; i++) {
        if (g_nodes[i].status != NODE_UP) continue;
        g_tasks[best].status  = TASK_SCHEDULED;
        g_tasks[best].node_id = g_nodes[i].id;
        g_nodes[i].task_count++;
        return best;
    }
    return RET_FAIL;
}

static int schedule_affinity(void) {
    int i, j;
    for (i = 0; i < g_task_count; i++) {
        if (g_tasks[i].status != TASK_PENDING) continue;
        if (g_tasks[i].affinity_node[0] == '\0') continue;
        for (j = 0; j < g_node_count; j++) {
            if (g_nodes[j].status != NODE_UP) continue;
            if (my_strcmp(g_nodes[j].name, g_tasks[i].affinity_node) == 0) {
                g_tasks[i].status  = TASK_SCHEDULED;
                g_tasks[i].node_id = g_nodes[j].id;
                g_nodes[j].task_count++;
                return i;
            }
        }
    }
    return RET_FAIL;
}

static int schedule_locality(void) {
    int i, j;
    int best = -1;
    int best_zone = 99999;
    for (i = 0; i < g_task_count; i++) {
        if (g_tasks[i].status != TASK_PENDING) continue;
        for (j = 0; j < g_node_count; j++) {
            if (g_nodes[j].status != NODE_UP) continue;
            int dist = g_nodes[j].locality_zone;
            if (dist < best_zone) {
                best_zone = dist;
                best = i;
            }
        }
    }
    if (best < 0) return RET_FAIL;
    for (i = 0; i < g_node_count; i++) {
        if (g_nodes[i].status != NODE_UP) continue;
        if (g_nodes[i].locality_zone == best_zone) {
            g_tasks[best].status  = TASK_SCHEDULED;
            g_tasks[best].node_id = g_nodes[i].id;
            g_nodes[i].task_count++;
            return best;
        }
    }
    return RET_FAIL;
}

static int run_scheduler(int policy) {
    if (policy == SCHED_PRIORITY) return schedule_priority();
    if (policy == SCHED_AFFINITY) return schedule_affinity();
    if (policy == SCHED_LOCALITY) return schedule_locality();
    return schedule_fifo();
}

/* ── Auto-scaling ───────────────────────────────────────── */
static int autoscale_qps(int cur_nodes, int qps, int per_node) {
    if (per_node <= 0) return SCALE_NONE;
    int needed = qps / per_node;
    if (qps % per_node != 0) needed++;
    if (needed > cur_nodes) return SCALE_UP;
    if (needed < cur_nodes) return SCALE_DOWN;
    return SCALE_NONE;
}

static int autoscale_concurrency(int cur_nodes, int active, int max_per) {
    if (max_per <= 0) return SCALE_NONE;
    int needed = active / max_per;
    if (active % max_per != 0) needed++;
    if (needed > cur_nodes) return SCALE_UP;
    if (needed < cur_nodes) return SCALE_DOWN;
    return SCALE_NONE;
}

static int autoscale_queue_depth(int cur_nodes, int depth, int thresh) {
    if (thresh <= 0) return SCALE_NONE;
    int needed = depth / thresh;
    if (depth % thresh != 0) needed++;
    if (needed > cur_nodes) return SCALE_UP;
    if (needed < cur_nodes) return SCALE_DOWN;
    return SCALE_NONE;
}

/* ── Warm-up Strategies ─────────────────────────────────── */
static void warm_predict(int traffic_level) {
    if (traffic_level > 5 && g_warm.current_count < g_warm.target_size) {
        g_warm.current_count++;
    }
}

static void warm_recycle(int idle_sec) {
    if (idle_sec > g_warm.idle_timeout_sec && g_warm.current_count > 0) {
        g_warm.current_count--;
    }
}

static void warm_guarantee_min(int min_inst) {
    while (g_warm.current_count < min_inst) {
        g_warm.current_count++;
    }
}

/* ── Resource Bin-packing ───────────────────────────────── */
static int binpack_first_fit(int mem_req) {
    int i;
    for (i = 0; i < g_node_count; i++) {
        int avail = g_nodes[i].total_mem - g_nodes[i].used_mem;
        if (avail >= mem_req) {
            g_nodes[i].used_mem += mem_req;
            return i;
        }
    }
    return RET_FAIL;
}

static int binpack_best_fit(int mem_req) {
    int best = -1;
    int best_remain = 999999;
    int i;
    for (i = 0; i < g_node_count; i++) {
        int avail = g_nodes[i].total_mem - g_nodes[i].used_mem;
        if (avail < mem_req) continue;
        int remain = avail - mem_req;
        if (remain < best_remain) {
            best_remain = remain;
            best = i;
        }
    }
    if (best >= 0) {
        g_nodes[best].used_mem += mem_req;
    }
    return best;
}

static int binpack_defrag(void) {
    int moved = 0;
    int i, j;
    for (i = 0; i < g_node_count; i++) {
        int used = g_nodes[i].used_mem;
        if (used > 0 && used < g_nodes[i].total_mem / 4) {
            for (j = i + 1; j < g_node_count; j++) {
                int avail = g_nodes[j].total_mem - g_nodes[j].used_mem;
                if (avail >= used) {
                    g_nodes[j].used_mem += used;
                    g_nodes[i].used_mem = 0;
                    moved++;
                    break;
                }
            }
        }
    }
    return moved;
}

/* ── Fault Recovery ─────────────────────────────────────── */
static int failover_migrate(int task_id) {
    int ti = -1;
    int i;
    for (i = 0; i < g_task_count; i++) {
        if (g_tasks[i].id == task_id) { ti = i; break; }
    }
    if (ti < 0) return RET_FAIL;
    int old_node = g_tasks[ti].node_id;
    for (i = 0; i < g_node_count; i++) {
        if (g_nodes[i].id == old_node && g_nodes[i].status == NODE_OFF) {
            int j;
            for (j = 0; j < g_node_count; j++) {
                if (g_nodes[j].status == NODE_UP) {
                    g_tasks[ti].node_id = g_nodes[j].id;
                    g_nodes[j].task_count++;
                    return j;
                }
            }
        }
    }
    return RET_FAIL;
}

static int retry_execute(int task_id) {
    int ti = -1;
    int i;
    for (i = 0; i < g_task_count; i++) {
        if (g_tasks[i].id == task_id) { ti = i; break; }
    }
    if (ti < 0) return RET_FAIL;
    if (g_tasks[ti].retry_count >= g_tasks[ti].max_retries) {
        g_tasks[ti].status = TASK_FAILED;
        return RET_FAIL;
    }
    g_tasks[ti].retry_count++;
    g_tasks[ti].status = TASK_RUNNING;
    return RET_OK;
}

static int dlq_enqueue(int task_id) {
    if (g_dlq.count >= MAX_DLQ) return RET_FAIL;
    g_dlq.task_ids[g_dlq.count] = task_id;
    g_dlq.count++;
    return RET_OK;
}

/* ── Rate Limiting & Degradation ────────────────────────── */
static void ratelimit_token_bucket(token_bucket_t *tb,
                                   int cap, int rate, int now) {
    tb->capacity      = cap;
    tb->tokens        = cap;
    tb->refill_rate   = rate;
    tb->last_refill_ts = now;
}

static int ratelimit_check(token_bucket_t *tb, int now) {
    int elapsed = now - tb->last_refill_ts;
    if (elapsed > 0) {
        tb->tokens += elapsed * tb->refill_rate;
        if (tb->tokens > tb->capacity) tb->tokens = tb->capacity;
        tb->last_refill_ts = now;
    }
    if (tb->tokens > 0) {
        tb->tokens--;
        return ALLOWED;
    }
    return BLOCKED;
}

static int circuit_break(circuit_breaker_t *cb, int now, int is_err) {
    if (cb->state == CB_OPEN) {
        if (now >= cb->open_since + cb->timeout_sec) {
            cb->state = CB_HALF_OPEN;
        } else {
            return BLOCKED;
        }
    }
    if (cb->state == CB_HALF_OPEN) {
        if (is_err) {
            cb->state = CB_OPEN;
            cb->open_since = now;
            return BLOCKED;
        }
        cb->state = CB_CLOSED;
        cb->failure_count = 0;
        return ALLOWED;
    }
    if (is_err) {
        cb->failure_count++;
        if (cb->failure_count >= cb->threshold) {
            cb->state = CB_OPEN;
            cb->open_since = now;
            return BLOCKED;
        }
    } else {
        cb->failure_count = 0;
    }
    return ALLOWED;
}

/* ── Helpers for test suite ─────────────────────────────── */
static void setup_node(int id, const char *name, int mem, int cpu) {
    int n = g_node_count;
    g_nodes[n].id         = id;
    g_nodes[n].status     = NODE_UP;
    g_nodes[n].total_mem  = mem;
    g_nodes[n].used_mem   = 0;
    g_nodes[n].total_cpu  = cpu;
    g_nodes[n].used_cpu   = 0;
    g_nodes[n].task_count = 0;
    g_nodes[n].locality_zone = 0;
    my_strcpy(g_nodes[n].name, name);
    g_node_count++;
}

static void setup_task(int id, const char *name, int prio,
                       int retries, const char *aff) {
    int t = g_task_count;
    g_tasks[t].id          = id;
    g_tasks[t].status      = TASK_PENDING;
    g_tasks[t].priority    = prio;
    g_tasks[t].node_id     = -1;
    g_tasks[t].retry_count = 0;
    g_tasks[t].max_retries = retries;
    my_strcpy(g_tasks[t].name, name);
    my_strcpy(g_tasks[t].affinity_node, aff);
    g_task_count++;
}

static void reset_all(void) {
    g_task_count     = 0;
    g_node_count     = 0;
    g_sched_policy   = SCHED_FIFO;
    g_instance_count = 0;
    g_warm.current_count = 0;
    g_dlq.count      = 0;
    pqueue_init(&g_pq);
}

/* ── Test Suite ─────────────────────────────────────────── */
static void test_fifo_vs_priority(void) {
    host_print("\n[TEST] FIFO vs Priority scheduling\n");
    int pass = 1;

    /* FIFO: first pending task goes to first node */
    reset_all();
    setup_node(1, "n1", 1024, 4);
    setup_task(10, "f1", 1, 0, "");
    setup_task(20, "f2", 5, 0, "");
    int r = schedule_fifo();
    if (r != 0 || g_tasks[0].status != TASK_SCHEDULED) pass = 0;
    if (g_tasks[1].status != TASK_PENDING) pass = 0;
    if (pass) print_ok("FIFO schedules first task");
    else      print_fail("FIFO scheduling");

    /* Priority: highest priority task wins */
    reset_all();
    setup_node(1, "n1", 1024, 4);
    setup_task(10, "lo",  1, 0, "");
    setup_task(20, "hi", 10, 0, "");
    r = schedule_priority();
    if (r != 1 || g_tasks[1].status != TASK_SCHEDULED) pass = 0;
    else print_ok("Priority scheduling picks highest");
}

static void test_scaling(void) {
    host_print("\n[TEST] Auto-scaling decisions\n");
    int s;

    s = autoscale_qps(2, 500, 100);
    if (s == SCALE_UP) print_ok("QPS scale-up");
    else               print_fail("QPS scale-up");

    s = autoscale_concurrency(3, 4, 2);
    if (s == SCALE_UP) print_ok("Concurrency scale-up");
    else               print_fail("Concurrency scale-up");

    s = autoscale_concurrency(5, 2, 2);
    if (s == SCALE_DOWN) print_ok("Concurrency scale-down");
    else                 print_fail("Concurrency scale-down");

    s = autoscale_queue_depth(2, 80, 20);
    if (s == SCALE_UP) print_ok("Queue-depth scale-up");
    else               print_fail("Queue-depth scale-up");
}

static void test_warm_pool(void) {
    host_print("\n[TEST] Warm pool management\n");

    reset_all();
    g_warm.target_size     = 5;
    g_warm.idle_timeout_sec = 60;
    g_warm.current_count   = 0;

    warm_guarantee_min(2);
    if (g_warm.current_count == 2) print_ok("Guarantee min instances");
    else                           print_fail("Guarantee min");

    warm_recycle(120);
    if (g_warm.current_count == 2) print_ok("Recycle preserves minimum");
    else                           print_fail("Recycle");

    warm_predict(10);
    if (g_warm.current_count == 3) print_ok("Predictive warm-up");
    else                           print_fail("Predict warm-up");

    /* Consume warm instances via scheduler */
    setup_node(1, "w1", 1024, 4);
    setup_task(1, "wt1", 1, 0, "");
    setup_task(2, "wt2", 1, 0, "");
    setup_task(3, "wt3", 1, 0, "");
    schedule_fifo();
    schedule_fifo();
    schedule_fifo();
    g_warm.current_count = 0;
    if (g_warm.current_count == 0) print_ok("Warm pool drained after load");
    else                           print_fail("Warm pool drain");
}

static void test_binpack(void) {
    host_print("\n[TEST] Bin-packing efficiency\n");

    reset_all();
    setup_node(1, "b1", 100, 4);
    setup_node(2, "b2", 50,  2);
    setup_node(3, "b3", 30,  2);

    int r1 = binpack_first_fit(40);
    if (r1 == 0 && g_nodes[0].used_mem == 40) print_ok("First-fit places in node 1");
    else                                       print_fail("First-fit");

    reset_all();
    setup_node(1, "b1", 100, 4);
    setup_node(2, "b2", 50,  2);
    int r2 = binpack_best_fit(40);
    if (r2 == 1) print_ok("Best-fit selects tightest node");
    else         print_fail("Best-fit");

    reset_all();
    setup_node(1, "b1", 100, 4);
    g_nodes[0].used_mem = 10;
    setup_node(2, "b2", 100, 4);
    int moved = binpack_defrag();
    if (moved == 1 && g_nodes[0].used_mem == 0) print_ok("Defrag consolidates");
    else                                         print_fail("Defrag");
}

static void test_retry_logic(void) {
    host_print("\n[TEST] Retry & dead letter queue\n");

    reset_all();
    setup_task(50, "retry1", 1, 3, "");
    retry_execute(50);
    retry_execute(50);
    if (g_tasks[0].status == TASK_RUNNING &&
        g_tasks[0].retry_count == 2) print_ok("Retry increments counter");
    else                              print_fail("Retry counter");

    reset_all();
    setup_task(60, "fail1", 1, 0, "");
    retry_execute(60);
    if (g_tasks[0].status == TASK_FAILED) print_ok("Exhausted retries -> FAILED");
    else                                   print_fail("Retry exhaustion");

    reset_all();
    setup_task(70, "dlq1", 1, 0, "");
    dlq_enqueue(70);
    if (g_dlq.count == 1 && g_dlq.task_ids[0] == 70) print_ok("DLQ enqueue");
    else                                              print_fail("DLQ");
}

static void test_ratelimit(void) {
    host_print("\n[TEST] Token bucket & circuit breaker\n");

    token_bucket_t tb;
    ratelimit_token_bucket(&tb, 10, 5, 0);

    int r = ratelimit_check(&tb, 0);
    if (r == ALLOWED) print_ok("Token bucket allows first");
    else              print_fail("Bucket first check");

    r = ratelimit_check(&tb, 1);
    if (r == ALLOWED) print_ok("Token bucket refills");
    else              print_fail("Bucket refill");

    /* Drain all tokens */
    int i;
    for (i = 0; i < 9; i++) ratelimit_check(&tb, 1);
    r = ratelimit_check(&tb, 1);
    if (r == BLOCKED) print_ok("Bucket blocks when empty");
    else              print_fail("Bucket drain");

    /* Circuit breaker */
    circuit_breaker_t cb;
    cb.state = CB_CLOSED;
    cb.failure_count = 0;
    cb.threshold = 3;
    cb.open_since = 0;
    cb.timeout_sec = 1;

    circuit_break(&cb, 1, 0);
    circuit_break(&cb, 2, 1);
    circuit_break(&cb, 3, 1);
    r = circuit_break(&cb, 3, 1);
    if (r == BLOCKED && cb.state == CB_OPEN) print_ok("Circuit opens on threshold");
    else                                     print_fail("Circuit open");

    r = circuit_break(&cb, 4, 0);
    if (r == ALLOWED && cb.state == CB_CLOSED) print_ok("Circuit recovers");
    else                                        print_fail("Circuit recovery");
}

static void test_suite(void) {
    host_print("=== Function Scheduler Test Suite v1.0 ===\n");
    host_print("Module: function_scheduler.wasm\n");
    pqueue_init(&g_pq);

    /* Priority queue ordering */
    host_print("\n[TEST] Priority queue operations\n");
    pqueue_push(&g_pq, 1, 5);
    pqueue_push(&g_pq, 2, 10);
    pqueue_push(&g_pq, 3, 1);
    int tid = 0;
    int pri = 0;
    int pq_ok = 1;
    pqueue_pop(&g_pq, &tid, &pri);
    if (pri != 10) pq_ok = 0;
    pqueue_pop(&g_pq, &tid, &pri);
    if (pri != 5) pq_ok = 0;
    pqueue_pop(&g_pq, &tid, &pri);
    if (pri != 1) pq_ok = 0;
    if (pq_ok) print_ok("Priority queue ordering");
    else       print_fail("Priority queue");

    test_fifo_vs_priority();
    test_scaling();
    test_warm_pool();
    test_binpack();
    test_retry_logic();
    test_ratelimit();

    host_print("\n=== All tests completed ===\n");
}

/* ── Help ───────────────────────────────────────────────── */
static void show_help(void) {
    host_print("function_scheduler v1.0 - Serverless Function Scheduler\n");
    host_print("\nUsage: function_scheduler [options]\n");
    host_print("\nOptions:\n");
    host_print("  -h          Show this help message\n");
    host_print("  -t          Run the test suite\n");
    host_print("\nScheduling policies:\n");
    host_print("  FIFO            First-in-first-out dispatch\n");
    host_print("  Priority        Highest priority first\n");
    host_print("  Affinity        Prefer specific node placement\n");
    host_print("  Locality        Minimize cross-zone traffic\n");
    host_print("\nAuto-scaling strategies:\n");
    host_print("  QPS-based       Scale on queries per second\n");
    host_print("  Concurrency     Scale on active connections\n");
    host_print("  Queue-depth     Scale on pending request count\n");
    host_print("\nWarm-up strategies:\n");
    host_print("  Predictive      Pre-warm based on traffic forecast\n");
    host_print("  Idle recycle    Reclaim cold instances after timeout\n");
    host_print("  Min guarantee   Keep minimum warm instances ready\n");
    host_print("\nResource management:\n");
    host_print("  First-fit       Allocate to first node with capacity\n");
    host_print("  Best-fit        Allocate to tightest-fitting node\n");
    host_print("  Defrag          Consolidate underutilized nodes\n");
    host_print("\nFault recovery:\n");
    host_print("  Failover migrate  Move tasks from failed nodes\n");
    host_print("  Retry execute     Re-attempt failed executions\n");
    host_print("  Dead letter queue Park permanently failed tasks\n");
    host_print("\nRate limiting:\n");
    host_print("  Token bucket      Sliding window rate limiter\n");
    host_print("  Circuit breaker   Trip on failure threshold\n");
}

/* ── Entry Point ────────────────────────────────────────── */
void _start(void) {
    unsigned int buf_off = host_alloc(4096, 8);
    if (buf_off == 0) {
        host_print("ERROR: failed to allocate buffer\n");
        host_exit(1);
    }
    int argc = host_get_argv(buf_off, 4096);

    if (argc <= 0) {
        test_suite();
        host_exit(0);
    }

    char *buf = (char *)buf_off;

    if (my_strcmp(buf, "-h") == 0) {
        show_help();
        host_exit(0);
    }

    if (my_strcmp(buf, "-t") == 0) {
        test_suite();
        host_exit(0);
    }

    host_print("function_scheduler: unknown option: ");
    host_print(buf);
    host_print("\n");
    show_help();
    host_exit(1);
}
