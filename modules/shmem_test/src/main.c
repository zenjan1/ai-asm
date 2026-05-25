/*
 * aiasm-aarch64/modules/shmem_test/src/main.c
 * Shared memory IPC test module.
 * Tests: allocate, write, read, attach/detach, flag sync.
 */

__attribute__((import_module("host"), import_name("print")))
extern void wasm_host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void wasm_host_exit(int code);

__attribute__((import_module("host"), import_name("shmem_alloc")))
extern int wasm_shmem_alloc(int pid, int size);

__attribute__((import_module("host"), import_name("shmem_free")))
extern void wasm_shmem_free(int region_id);

__attribute__((import_module("host"), import_name("shmem_attach")))
extern int wasm_shmem_attach(int region_id, int pid);

__attribute__((import_module("host"), import_name("shmem_detach")))
extern int wasm_shmem_detach(int region_id);

__attribute__((import_module("host"), import_name("shmem_get_ptr")))
extern int wasm_shmem_get_ptr(int region_id);

__attribute__((import_module("host"), import_name("shmem_set_flag")))
extern void wasm_shmem_set_flag(int region_id, int flag);

__attribute__((import_module("host"), import_name("shmem_get_flag")))
extern int wasm_shmem_get_flag(int region_id);

__attribute__((import_module("host"), import_name("shmem_clear_flag")))
extern void wasm_shmem_clear_flag(int region_id, int flag);

__attribute__((import_module("host"), import_name("shmem_write")))
extern int wasm_shmem_write(int region_id, unsigned int buf_off, int len);

__attribute__((import_module("host"), import_name("shmem_read")))
extern int wasm_shmem_read(int region_id, unsigned int buf_off, int len);

__attribute__((import_module("host"), import_name("sleep")))
extern void wasm_host_sleep(unsigned int ms);

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

static void print_hex(const char *label, unsigned int buf, int len)
{
    print_str(label);
    char *data = (char *)buf;
    char *out = (char *)(BUF_SCRATCH);
    int pos = 0;
    for (int i = 0; i < len; i++) {
        unsigned char b = (unsigned char)data[i];
        unsigned char hi = (b >> 4) & 0xF;
        unsigned char lo = b & 0xF;
        out[pos++] = (char)(hi < 10 ? '0' + hi : 'a' + hi - 10);
        out[pos++] = (char)(lo < 10 ? '0' + lo : 'a' + lo - 10);
        if (pos >= 60) {
            out[pos++] = '\n';
            wasm_host_print(BUF_SCRATCH, pos);
            pos = 0;
        } else {
            out[pos++] = ' ';
        }
    }
    if (pos > 0) {
        out[pos++] = '\n';
        wasm_host_print(BUF_SCRATCH, pos);
    }
}

/* Test message */
static const char test_msg[] = "Hello from PID 1 via shared memory!";

__attribute__((export_name("_start")))
void _start(void)
{
    int pid = 1;  /* We are PID 1 (init) */
    int rid, rc, flags;

    print_str("\n=== Shared Memory IPC Test ===\n\n");

    /* Test 1: Allocate a shared memory region */
    print_str("[1] Allocating shared memory (pid=1, size=64)...\n");
    rid = wasm_shmem_alloc(pid, 64);
    print_str("  region_id = ");
    print_int(rid);
    if (rid <= 0) {
        print_str("  FAILED: could not allocate\n");
        wasm_host_exit(1);
    }

    /* Test 2: Write data to shared memory */
    print_str("[2] Writing data to shared memory...\n");
    /* Copy test message to scratch buffer */
    unsigned int msg_len = 0;
    while (test_msg[msg_len]) msg_len++;
    char *scratch = (char *)BUF_SCRATCH;
    for (unsigned int i = 0; i < msg_len; i++)
        scratch[i] = test_msg[i];
    scratch[msg_len] = '\0';

    rc = wasm_shmem_write(rid, BUF_SCRATCH, (int)msg_len);
    print_str("  write returned: ");
    print_int(rc);

    /* Test 3: Read back the data */
    print_str("[3] Reading data from shared memory...\n");
    /* Clear scratch first */
    for (unsigned int i = 0; i < msg_len + 4; i++)
        scratch[i] = '?';

    rc = wasm_shmem_read(rid, BUF_SCRATCH, (int)msg_len);
    scratch[msg_len] = '\0';
    print_str("  read returned: ");
    print_int(rc);
    print_str("  data = ");
    print_str(scratch);
    print_str("\n");

    /* Test 4: Set writer flag */
    print_str("[4] Setting WRITER flag (0x10)...\n");
    wasm_shmem_set_flag(rid, 0x10);
    flags = wasm_shmem_get_flag(rid);
    print_str("  flags = ");
    print_int(flags);

    /* Test 5: Clear reader flag (simulating reader consumed) */
    print_str("[5] Clearing flags, simulating reader...\n");
    wasm_shmem_clear_flag(rid, 0x10);
    flags = wasm_shmem_get_flag(rid);
    print_str("  flags after clear = ");
    print_int(flags);

    /* Test 6: Attach another process (simulated) */
    print_str("[6] Attaching pid=2 to shared memory...\n");
    rc = wasm_shmem_attach(rid, 2);
    print_str("  attach returned: ");
    print_int(rc);
    flags = wasm_shmem_get_flag(rid);
    print_str("  flags after attach = ");
    print_int(flags);

    /* Test 7: Detach */
    print_str("[7] Detaching pid=2 from shared memory...\n");
    rc = wasm_shmem_detach(rid);
    print_str("  detach returned: ");
    print_int(rc);
    flags = wasm_shmem_get_flag(rid);
    print_str("  flags after detach = ");
    print_int(flags);

    /* Test 8: Free the region */
    print_str("[8] Freeing shared memory region...\n");
    wasm_shmem_free(rid);

    /* Test 9: Verify freed (should fail) */
    print_str("[9] Verifying region is freed (should return -1)...\n");
    rc = wasm_shmem_read(rid, BUF_SCRATCH, 10);
    print_str("  read on freed region = ");
    print_int(rc);

    print_str("\n=== Shared Memory Test Complete ===\n");

    wasm_host_exit(0);
}
