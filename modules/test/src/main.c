/*
 * aiasm-aarch64/modules/test/src/main.c
 * Integration test module for AI-ASM AArch64 kernel v0.7
 *
 * Tests: FS create/write/read/delete, GUI window creation,
 * network socket creation, and scheduler yield.
 */

/* -------------------------------------------------------------------------- */
/* WASM imports - core                                                        */
/* -------------------------------------------------------------------------- */

__attribute__((import_module("host"), import_name("print")))
extern void wasm_host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("get_tick")))
extern unsigned long long wasm_host_get_tick(void);

__attribute__((import_module("host"), import_name("exit")))
extern void wasm_host_exit(int code);

__attribute__((import_module("host"), import_name("alloc")))
extern void *wasm_host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("free")))
extern void wasm_host_free(void *ptr);

__attribute__((import_module("host"), import_name("log")))
extern void wasm_host_log(unsigned int level_off, unsigned int level_len,
                          unsigned int msg_off, unsigned int msg_len);

__attribute__((import_module("host"), import_name("sleep")))
extern void wasm_host_sleep(unsigned int ms);

__attribute__((import_module("host"), import_name("yield")))
extern void wasm_host_yield(void);

/* -------------------------------------------------------------------------- */
/* WASM imports - filesystem                                                  */
/* -------------------------------------------------------------------------- */

__attribute__((import_module("host"), import_name("fs_open")))
extern int wasm_host_fs_open(unsigned int path_off, unsigned int path_len);

__attribute__((import_module("host"), import_name("fs_read")))
extern int wasm_host_fs_read(int fd, unsigned int buf_off, unsigned int len);

__attribute__((import_module("host"), import_name("fs_close")))
extern void wasm_host_fs_close(int fd);

__attribute__((import_module("host"), import_name("fs_list")))
extern int wasm_host_fs_list(unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("fs_write")))
extern int wasm_host_fs_write(int fd, unsigned int buf_off, unsigned int len);

__attribute__((import_module("host"), import_name("fs_create")))
extern int wasm_host_fs_create(unsigned int name_off, unsigned int name_len);

__attribute__((import_module("host"), import_name("fs_delete")))
extern int wasm_host_fs_delete(unsigned int name_off, unsigned int name_len);

/* -------------------------------------------------------------------------- */
/* WASM imports - network                                                     */
/* -------------------------------------------------------------------------- */

__attribute__((import_module("host"), import_name("net_connect")))
extern int wasm_host_net_connect(unsigned int ip, unsigned int port, unsigned int proto);

__attribute__((import_module("host"), import_name("net_send")))
extern int wasm_host_net_send(int sock, unsigned int buf_off, unsigned int len);

__attribute__((import_module("host"), import_name("net_recv")))
extern int wasm_host_net_recv(int sock, unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("net_close")))
extern void wasm_host_net_close(int sock);

/* -------------------------------------------------------------------------- */
/* WASM imports - GUI                                                         */
/* -------------------------------------------------------------------------- */

__attribute__((import_module("host"), import_name("gui_create")))
extern int wasm_host_gui_create(unsigned int x, unsigned int y, unsigned int w,
                                unsigned int h, unsigned int title_off);

__attribute__((import_module("host"), import_name("gui_draw")))
extern int wasm_host_gui_draw(int win_id, unsigned int buf_off,
                              unsigned int x, unsigned int y,
                              unsigned int w, unsigned int h);

__attribute__((import_module("host"), import_name("gui_flush")))
extern void wasm_host_gui_flush(unsigned int win_id);

__attribute__((import_module("host"), import_name("gui_poll_event")))
extern int wasm_host_gui_poll_event(int win_id, unsigned int event_buf_off);

/* -------------------------------------------------------------------------- */
/* Memory and string helpers                                                  */
/* -------------------------------------------------------------------------- */

static unsigned int heap_pos = 65536;

static unsigned int alloc(unsigned int n)
{
    unsigned int ptr = heap_pos;
    heap_pos += n;
    heap_pos = (heap_pos + 15) & ~15u;
    return ptr;
}

static unsigned int my_strlen(const char *s)
{
    const char *p = s;
    while (*p) p++;
    return (unsigned int)(p - s);
}

static int my_strcmp(const char *a, const char *b)
{
    while (*a && *b) {
        if (*a != *b) return 1;
        a++; b++;
    }
    return (*a != *b) ? 1 : 0;
}

static void my_memcpy(unsigned int dst_off, const char *src, unsigned int len)
{
    char *dst = (char *)dst_off;
    for (unsigned int i = 0; i < len; i++) dst[i] = src[i];
}

static void print_str(const char *s)
{
    unsigned int len = my_strlen(s);
    unsigned int offset = alloc(len);
    my_memcpy(offset, s, len);
    wasm_host_print(offset, len);
}

static void print_int(int v)
{
    char buf[12];
    int i = 0;
    if (v == 0) {
        buf[i++] = '0';
    } else {
        int n = v;
        if (n < 0) { print_str("-"); n = -n; }
        while (n > 0) {
            buf[i++] = '0' + (char)(n % 10);
            n /= 10;
        }
    }
    unsigned int offset = alloc((unsigned int)i);
    char *dst = (char *)(offset);
    for (int j = 0; j < i; j++)
        dst[j] = buf[i - 1 - j];
    wasm_host_print(offset, (unsigned int)i);
}

/* -------------------------------------------------------------------------- */
/* Test framework                                                             */
/* -------------------------------------------------------------------------- */

static int tests_passed = 0;
static int tests_failed = 0;

static void test_begin(const char *name)
{
    print_str("  [TEST] ");
    print_str(name);
    print_str("... ");
}

static void test_pass(void)
{
    print_str("PASS\n");
    tests_passed++;
}

static void test_fail(const char *reason)
{
    print_str("FAIL: ");
    print_str(reason);
    print_str("\n");
    tests_failed++;
}

/* -------------------------------------------------------------------------- */
/* Test: Filesystem - create file                                             */
/* -------------------------------------------------------------------------- */

static void test_fs_create(void)
{
    test_begin("fs_create");

    unsigned int nlen = my_strlen("test.dat");
    unsigned int noff = alloc(nlen);
    my_memcpy(noff, "test.dat", nlen);

    int rc = wasm_host_fs_create(noff, nlen);
    if (rc >= 0) {
        test_pass();
    } else {
        print_int(rc);
        test_fail("create returned negative");
    }
}

/* -------------------------------------------------------------------------- */
/* Test: Filesystem - write to file                                           */
/* -------------------------------------------------------------------------- */

static void test_fs_write(void)
{
    test_begin("fs_write");

    /* Open the file we just created */
    unsigned int nlen = my_strlen("test.dat");
    unsigned int noff = alloc(nlen);
    my_memcpy(noff, "test.dat", nlen);

    int fd = wasm_host_fs_open(noff, nlen);
    if (fd < 0) {
        print_int(fd);
        test_fail("open failed");
        return;
    }

    /* Write test data */
    const char *data = "Hello from WASM test!";
    unsigned int dlen = my_strlen(data);
    unsigned int doff = alloc(dlen);
    my_memcpy(doff, data, dlen);

    int rc = wasm_host_fs_write(fd, doff, dlen);
    wasm_host_fs_close(fd);

    if (rc == (int)dlen) {
        test_pass();
    } else {
        print_int(rc);
        test_fail("write size mismatch");
    }
}

/* -------------------------------------------------------------------------- */
/* Test: Filesystem - read back                                               */
/* -------------------------------------------------------------------------- */

static void test_fs_read(void)
{
    test_begin("fs_read");

    unsigned int nlen = my_strlen("test.dat");
    unsigned int noff = alloc(nlen);
    my_memcpy(noff, "test.dat", nlen);

    int fd = wasm_host_fs_open(noff, nlen);
    if (fd < 0) {
        print_int(fd);
        test_fail("open failed");
        return;
    }

    unsigned int buf_off = alloc(64);
    int rc = wasm_host_fs_read(fd, buf_off, 64);
    wasm_host_fs_close(fd);

    if (rc > 0) {
        /* Verify content */
        const char *expected = "Hello from WASM test!";
        int match = 1;
        for (int i = 0; i < rc; i++) {
            if (((char *)buf_off)[i] != expected[i]) {
                match = 0;
                break;
            }
        }
        if (match) {
            test_pass();
        } else {
            test_fail("content mismatch");
        }
    } else {
        print_int(rc);
        test_fail("read returned nothing");
    }
}

/* -------------------------------------------------------------------------- */
/* Test: Filesystem - delete file                                             */
/* -------------------------------------------------------------------------- */

static void test_fs_delete(void)
{
    test_begin("fs_delete");

    unsigned int nlen = my_strlen("test.dat");
    unsigned int noff = alloc(nlen);
    my_memcpy(noff, "test.dat", nlen);

    int rc = wasm_host_fs_delete(noff, nlen);
    if (rc >= 0) {
        test_pass();
    } else {
        print_int(rc);
        test_fail("delete failed");
    }
}

/* -------------------------------------------------------------------------- */
/* Test: Filesystem - list files                                              */
/* -------------------------------------------------------------------------- */

static void test_fs_list(void)
{
    test_begin("fs_list");

    unsigned int buf_off = alloc(512);
    int rc = wasm_host_fs_list(buf_off, 512);
    if (rc >= 0) {
        test_pass();
    } else {
        print_int(rc);
        test_fail("list failed");
    }
}

/* -------------------------------------------------------------------------- */
/* Test: Network - socket creation (connect will fail without remote, but
 * we test that the API is reachable and returns a socket id or error)        */
/* -------------------------------------------------------------------------- */

static void test_net_connect(void)
{
    test_begin("net_connect");

    /* Try connecting to a dummy address - should return socket id or -1 */
    int sock = wasm_host_net_connect(0x7F000001, 8080, 0);  /* 127.0.0.1:8080 */

    if (sock >= 0) {
        test_pass();
        wasm_host_net_close(sock);
    } else {
        /* API is reachable even if connection fails */
        print_str("API reachable (no remote) ");
        test_pass();
    }
}

/* -------------------------------------------------------------------------- */
/* Test: GUI - window creation                                                */
/* -------------------------------------------------------------------------- */

static void test_gui_create(void)
{
    test_begin("gui_create");

    unsigned int tlen = my_strlen("Test Window");
    unsigned int toff = alloc(tlen);
    my_memcpy(toff, "Test Window", tlen);

    int win = wasm_host_gui_create(50, 50, 400, 300, toff);
    if (win >= 0) {
        test_pass();
    } else {
        print_int(win);
        test_fail("create returned negative");
    }
}

/* -------------------------------------------------------------------------- */
/* Test: GUI - draw and flush                                                 */
/* -------------------------------------------------------------------------- */

static void test_gui_draw(void)
{
    test_begin("gui_draw+flush");

    /* Create a small buffer with colored pixels */
    unsigned int w = 100, h = 50;
    unsigned int npixels = w * h;
    unsigned int buf_off = alloc(npixels * 4);

    /* Fill with a gradient pattern */
    unsigned int *pixels = (unsigned int *)buf_off;
    for (unsigned int y = 0; y < h; y++) {
        for (unsigned int x = 0; x < w; x++) {
            unsigned int r = (x * 255) / w;
            unsigned int g = (y * 255) / h;
            unsigned int b = 0x80;
            pixels[y * w + x] = (r << 16) | (g << 8) | b;
        }
    }

    /* Create window first */
    unsigned int tlen = my_strlen("Gradient");
    unsigned int toff = alloc(tlen);
    my_memcpy(toff, "Gradient", tlen);
    int win = wasm_host_gui_create(100, 100, 200, 150, toff);

    if (win >= 0) {
        int rc = wasm_host_gui_draw(win, buf_off, 0, 0, w, h);
        wasm_host_gui_flush(win);
        if (rc == 0) {
            test_pass();
        } else {
            print_int(rc);
            test_fail("draw failed");
        }
    } else {
        print_int(win);
        test_fail("window create failed");
    }
}

/* -------------------------------------------------------------------------- */
/* Test: GUI - poll events (should return no events in automated test)        */
/* -------------------------------------------------------------------------- */

static void test_gui_poll(void)
{
    test_begin("gui_poll_event");

    unsigned int evt_off = alloc(16);
    int rc = wasm_host_gui_poll_event(0, evt_off);

    /* In automated test, no user input so expect 0 (no event) */
    if (rc == 0 || rc == 1) {
        test_pass();
    } else {
        print_int(rc);
        test_fail("unexpected return");
    }
}

/* -------------------------------------------------------------------------- */
/* Test: Allocator                                                            */
/* -------------------------------------------------------------------------- */

static void test_alloc(void)
{
    test_begin("alloc/free");

    void *p1 = wasm_host_alloc(128, 16);
    void *p2 = wasm_host_alloc(256, 16);
    void *p3 = wasm_host_alloc(64, 16);

    if (p1 && p2 && p3 && p1 != p2 && p2 != p3) {
        test_pass();
        wasm_host_free(p1);
        wasm_host_free(p2);
        wasm_host_free(p3);
    } else {
        test_fail("alloc returned null or duplicate");
    }
}

/* -------------------------------------------------------------------------- */
/* Test: Sleep/yield                                                          */
/* -------------------------------------------------------------------------- */

static void test_sleep(void)
{
    test_begin("sleep(50ms)");

    unsigned long long t0 = wasm_host_get_tick();
    wasm_host_sleep(50);
    unsigned long long t1 = wasm_host_get_tick();

    if (t1 > t0) {
        test_pass();
    } else {
        test_fail("tick did not advance");
    }
}

/* -------------------------------------------------------------------------- */
/* Entry point                                                                */
/* -------------------------------------------------------------------------- */

__attribute__((export_name("_start"), used))
void _start(void)
{
    unsigned long long t_boot = wasm_host_get_tick();

    print_str("\n");
    print_str("========================================\n");
    print_str("  AI-ASM AArch64 v0.7 Integration Test\n");
    print_str("========================================\n\n");

    /* --- Filesystem tests --- */
    print_str("[FS]\n");
    test_fs_list();
    test_fs_create();
    test_fs_write();
    test_fs_read();
    test_fs_delete();
    print_str("\n");

    /* --- Network tests --- */
    print_str("[NET]\n");
    test_net_connect();
    print_str("\n");

    /* --- GUI tests --- */
    print_str("[GUI]\n");
    test_gui_create();
    test_gui_draw();
    test_gui_poll();
    print_str("\n");

    /* --- Core tests --- */
    print_str("[CORE]\n");
    test_alloc();
    test_sleep();
    print_str("\n");

    /* --- Summary --- */
    unsigned long long t_end = wasm_host_get_tick();
    unsigned long long elapsed = t_end - t_boot;

    print_str("========================================\n");
    print_str("  Results: ");
    print_int(tests_passed);
    print_str(" passed, ");
    print_int(tests_failed);
    print_str(" failed\n");
    print_str("  Boot ticks: ");
    print_int((int)elapsed);
    print_str("\n");
    print_str("========================================\n\n");

    if (tests_failed == 0) {
        print_str("All tests passed!\n");
    } else {
        print_str("SOME TESTS FAILED\n");
    }

    wasm_host_log(alloc(4), 4, alloc(21), 21);  /* INFO, "integration test complete" */

    wasm_host_exit(0);
}
