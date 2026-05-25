/*
 * aiasm-aarch64/modules/sem_test/src/main.c
 * Semaphore IPC test module.
 * Tests: create, post, wait, try_wait, get_value, destroy.
 */

__attribute__((import_module("host"), import_name("print")))
extern void wasm_host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void wasm_host_exit(int code);

__attribute__((import_module("host"), import_name("sem_create")))
extern int wasm_sem_create(int initial_value);

__attribute__((import_module("host"), import_name("sem_wait")))
extern int wasm_sem_wait(int sem_id);

__attribute__((import_module("host"), import_name("sem_try_wait")))
extern int wasm_sem_try_wait(int sem_id);

__attribute__((import_module("host"), import_name("sem_post")))
extern int wasm_sem_post(int sem_id);

__attribute__((import_module("host"), import_name("sem_destroy")))
extern void wasm_sem_destroy(int sem_id);

__attribute__((import_module("host"), import_name("sem_get_value")))
extern int wasm_sem_get_value(int sem_id);

#define BUF_SCRATCH 0x1000

static void print_str(const char *s)
{
    unsigned int len = 0;
    while (s[len]) len++;
    char *dst = (char *)BUF_SCRATCH;
    for (unsigned int i = 0; i < len; i++)
        dst[i] = s[i];
    wasm_host_print(BUF_SCRATCH, len);
}

static void print_int(int v)
{
    char buf[16]; int pos = 0;
    if (v < 0) { buf[pos++] = '-'; v = -v; }
    if (v == 0) buf[pos++] = '0';
    else {
        char tmp[12]; int ti = 0;
        while (v > 0) { tmp[ti++] = (char)('0' + (v % 10)); v /= 10; }
        while (ti > 0) buf[pos++] = tmp[--ti];
    }
    buf[pos++] = '\n';
    wasm_host_print(BUF_SCRATCH, pos);
}

__attribute__((export_name("_start")))
void _start(void)
{
    int sid, rc, val;

    print_str("\n=== Semaphore IPC Test ===\n\n");

    /* Test 1: Create semaphore with initial value 0 */
    print_str("[1] Creating semaphore (initial=0)...\n");
    sid = wasm_sem_create(0);
    print_str("  sem_id = ");
    print_int(sid);
    if (sid <= 0) {
        print_str("  FAILED\n");
        wasm_host_exit(1);
    }

    /* Test 2: Get value (should be 0) */
    print_str("[2] Getting value (expect 0)...\n");
    val = wasm_sem_get_value(sid);
    print_str("  value = ");
    print_int(val);

    /* Test 3: Try wait (should fail, would block) */
    print_str("[3] Try wait on value=0 (expect -2)...\n");
    rc = wasm_sem_try_wait(sid);
    print_str("  try_wait = ");
    print_int(rc);

    /* Test 4: Post (increment to 1) */
    print_str("[4] Posting (value -> 1)...\n");
    rc = wasm_sem_post(sid);
    print_str("  post returned: ");
    print_int(rc);
    val = wasm_sem_get_value(sid);
    print_str("  value after post = ");
    print_int(val);

    /* Test 5: Wait (should succeed, decrement to 0) */
    print_str("[5] Waiting (should succeed, value -> 0)...\n");
    rc = wasm_sem_wait(sid);
    print_str("  wait returned: ");
    print_int(rc);
    val = wasm_sem_get_value(sid);
    print_str("  value after wait = ");
    print_int(val);

    /* Test 6: Create semaphore with initial value 3 */
    print_str("[6] Creating semaphore (initial=3)...\n");
    int sid2 = wasm_sem_create(3);
    print_str("  sem_id = ");
    print_int(sid2);
    val = wasm_sem_get_value(sid2);
    print_str("  value = ");
    print_int(val);

    /* Test 7: Try wait (should succeed) */
    print_str("[7] Try wait on value=3 (expect 0)...\n");
    rc = wasm_sem_try_wait(sid2);
    print_str("  try_wait = ");
    print_int(rc);
    val = wasm_sem_get_value(sid2);
    print_str("  value after try_wait = ");
    print_int(val);

    /* Test 8: Destroy first semaphore */
    print_str("[8] Destroying first semaphore...\n");
    wasm_sem_destroy(sid);
    rc = wasm_sem_get_value(sid);
    print_str("  value after destroy = ");
    print_int(rc);

    /* Test 9: Create many semaphores (fill table) */
    print_str("[9] Creating 15 more semaphores...\n");
    int ids[15];
    for (int i = 0; i < 15; i++) {
        ids[i] = wasm_sem_create(i + 1);
    }
    print_str("  last sem_id = ");
    print_int(ids[14]);

    print_str("\n=== Semaphore Test Complete ===\n");

    wasm_host_exit(0);
}
