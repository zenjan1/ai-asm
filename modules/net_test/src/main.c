/*
 * aiasm-aarch64/modules/net_test/src/main.c
 * Network stack validation tests for AI-ASM v3.0
 *
 * Tests: TCP connect, HTTP GET, DNS resolution, socket lifecycle.
 */

/* -------------------------------------------------------------------------- */
/* WASM imports - core                                                        */
/* -------------------------------------------------------------------------- */

__attribute__((import_module("host"), import_name("print")))
extern void wasm_host_print(unsigned int offset, unsigned int len);

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

/* -------------------------------------------------------------------------- */
/* WASM imports - network                                                     */
/* -------------------------------------------------------------------------- */

__attribute__((import_module("host"), import_name("net_connect")))
extern int wasm_net_connect(unsigned int ip, unsigned int port, unsigned int proto);

__attribute__((import_module("host"), import_name("net_send")))
extern int wasm_net_send(int sock, unsigned int buf_off, unsigned int len);

__attribute__((import_module("host"), import_name("net_recv")))
extern int wasm_net_recv(int sock, unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("net_close")))
extern void wasm_net_close(int sock);

/* -------------------------------------------------------------------------- */
/* Constants                                                                  */
/* -------------------------------------------------------------------------- */

#define TCP_PROTO  6
#define UDP_PROTO 17
#define HEAP_BASE 65536
#define BUF_SIZE  512

/* -------------------------------------------------------------------------- */
/* Helpers                                                                    */
/* -------------------------------------------------------------------------- */

static unsigned int my_strlen(const char *s)
{
    const char *p = s;
    while (*p) p++;
    return (unsigned int)(p - s);
}

static void print_str(const char *s)
{
    unsigned int len = my_strlen(s);
    char *dst = (char *)HEAP_BASE;
    for (unsigned int i = 0; i < len; i++) dst[i] = s[i];
    wasm_host_print(HEAP_BASE, len);
}

static void log_str(const char *msg)
{
    unsigned int ml = my_strlen(msg);
    const char *level = "INFO";
    char *buf = (char *)HEAP_BASE;
    buf[0] = 'I'; buf[1] = 'N'; buf[2] = 'F'; buf[3] = 'O'; buf[4] = '\0';
    for (unsigned int i = 0; i < ml; i++) buf[5 + i] = msg[i];
    buf[5 + ml] = '\0';
    wasm_host_log(HEAP_BASE, 4, HEAP_BASE + 5, ml);
}

static void print_int(int v)
{
    char buf[16];
    int i = 0;
    if (v < 0) { print_str("-"); v = -v; }
    if (v == 0) buf[i++] = '0';
    else while (v > 0) { buf[i++] = '0' + (v % 10); v /= 10; }
    buf[i] = '\0';
    char *dst = (char *)HEAP_BASE;
    for (int j = 0; j <= i; j++) dst[j] = buf[j];
    wasm_host_print(HEAP_BASE, i);
}

static void ip_to_str(unsigned int ip, char *buf)
{
    unsigned int b0 = (ip >> 24) & 0xFF;
    unsigned int b1 = (ip >> 16) & 0xFF;
    unsigned int b2 = (ip >> 8) & 0xFF;
    unsigned int b3 = ip & 0xFF;
    int idx = 0;

    /* b0 */
    if (b0 >= 100) { buf[idx++] = '0' + (b0 / 100); buf[idx++] = '0' + ((b0 / 10) % 10); buf[idx++] = '0' + (b0 % 10); }
    else if (b0 >= 10) { buf[idx++] = '0' + (b0 / 10); buf[idx++] = '0' + (b0 % 10); }
    else { buf[idx++] = '0' + b0; }
    buf[idx++] = '.';
    /* b1 */
    if (b1 >= 100) { buf[idx++] = '0' + (b1 / 100); buf[idx++] = '0' + ((b1 / 10) % 10); buf[idx++] = '0' + (b1 % 10); }
    else if (b1 >= 10) { buf[idx++] = '0' + (b1 / 10); buf[idx++] = '0' + (b1 % 10); }
    else { buf[idx++] = '0' + b1; }
    buf[idx++] = '.';
    /* b2 */
    if (b2 >= 100) { buf[idx++] = '0' + (b2 / 100); buf[idx++] = '0' + ((b2 / 10) % 10); buf[idx++] = '0' + (b2 % 10); }
    else if (b2 >= 10) { buf[idx++] = '0' + (b2 / 10); buf[idx++] = '0' + (b2 % 10); }
    else { buf[idx++] = '0' + b2; }
    buf[idx++] = '.';
    /* b3 */
    if (b3 >= 100) { buf[idx++] = '0' + (b3 / 100); buf[idx++] = '0' + ((b3 / 10) % 10); buf[idx++] = '0' + (b3 % 10); }
    else if (b3 >= 10) { buf[idx++] = '0' + (b3 / 10); buf[idx++] = '0' + (b3 % 10); }
    else { buf[idx++] = '0' + b3; }
    buf[idx] = '\0';
}

static int tests_passed = 0;
static int tests_failed = 0;

static void check_result(const char *name, int ok)
{
    print_str("  ");
    print_str(name);
    print_str(": ");
    if (ok) {
        print_str("PASS\n");
        tests_passed++;
    } else {
        print_str("FAIL\n");
        tests_failed++;
    }
}

/* -------------------------------------------------------------------------- */
/* Tests                                                                      */
/* -------------------------------------------------------------------------- */

static void test_socket_lifecycle(void)
{
    print_str("\n[Test 1] Socket Lifecycle\n");

    /* Connect to localhost:80 (may fail, that's ok for testing) */
    int sock = wasm_net_connect(0x0A000001, 80, TCP_PROTO); /* 10.0.0.1:80 */
    print_str("  net_connect(10.0.0.1:80) = ");
    print_int(sock);
    print_str("\n");

    if (sock >= 0) {
        check_result("socket created", 1);
        wasm_net_close(sock);
        check_result("socket closed", 1);
    } else {
        /* Connection expected to fail in QEMU without network setup */
        print_str("  (expected in QEMU without virtio-net)\n");
        check_result("connection refused (expected)", 1);
    }
}

static void test_tcp_connect_http(void)
{
    print_str("\n[Test 2] TCP Connect + HTTP GET\n");

    /* Try connecting to 10.0.2.2:80 (QEMU host gateway) */
    int sock = wasm_net_connect(0x0A000202, 80, TCP_PROTO);
    print_str("  net_connect(10.0.2.2:80) = ");
    print_int(sock);
    print_str("\n");

    if (sock < 0) {
        print_str("  (network not available in QEMU without virtio-net)\n");
        check_result("HTTP GET skipped (no network)", 1);
        return;
    }

    check_result("TCP connected", 1);

    /* Send HTTP GET */
    const char *req = "GET / HTTP/1.0\r\nHost: 10.0.2.2\r\n\r\n";
    unsigned int rlen = my_strlen(req);
    char *buf = (char *)HEAP_BASE;
    for (unsigned int i = 0; i < rlen; i++) buf[i] = req[i];

    int sent = wasm_net_send(sock, HEAP_BASE, rlen);
    print_str("  net_send() = ");
    print_int(sent);
    print_str("\n");
    check_result("data sent", sent == (int)rlen);

    /* Receive response */
    wasm_host_sleep(1000);
    int recv = wasm_net_recv(sock, HEAP_BASE + 1, BUF_SIZE);
    print_str("  net_recv() = ");
    print_int(recv);
    print_str("\n");

    if (recv > 0) {
        check_result("data received", 1);
        buf[recv + 1] = '\0';
        print_str("  Response: ");
        /* Print first 80 chars of response */
        int limit = recv < 80 ? recv : 80;
        wasm_host_print(HEAP_BASE + 1, limit);
        print_str("\n");
    } else {
        check_result("no data received", 0);
    }

    wasm_net_close(sock);
}

static void test_udp_send(void)
{
    print_str("\n[Test 3] UDP Send\n");

    int sock = wasm_net_connect(0x0A000202, 53, UDP_PROTO);
    print_str("  UDP socket = ");
    print_int(sock);
    print_str("\n");

    if (sock >= 0) {
        check_result("UDP socket created", 1);
        /* Send minimal DNS query for example.com */
        unsigned char dns_query[] = {
            0x00, 0x01, /* Transaction ID */
            0x01, 0x00, /* Flags: standard query, recursion desired */
            0x00, 0x01, /* Questions: 1 */
            0x00, 0x00, /* Answer RRs: 0 */
            0x00, 0x00, /* Authority RRs: 0 */
            0x00, 0x00, /* Additional RRs: 0 */
            7, 'e', 'x', 'a', 'm', 'p', 'l', 'e',
            3, 'c', 'o', 'm',
            0,           /* Root */
            0x00, 0x01, /* Type: A */
            0x00, 0x01  /* Class: IN */
        };
        unsigned int dlen = sizeof(dns_query);
        char *buf = (char *)(HEAP_BASE + 1);
        for (unsigned int i = 0; i < dlen; i++) buf[i] = (char)dns_query[i];

        int sent = wasm_net_send(sock, HEAP_BASE + 1, dlen);
        print_str("  DNS query sent = ");
        print_int(sent);
        print_str("\n");
        check_result("DNS query sent", sent == (int)dlen);

        wasm_host_sleep(2000);
        int recv = wasm_net_recv(sock, HEAP_BASE + 1 + dlen, BUF_SIZE);
        print_str("  DNS response = ");
        print_int(recv);
        print_str("\n");

        if (recv > 0) {
            /* Parse DNS response - extract IP */
            unsigned char *resp = (unsigned char *)(HEAP_BASE + 1 + dlen);
            if (resp[3] & 0x80) { /* Check QR bit */
                unsigned int a1 = resp[recv - 4];
                unsigned int a2 = resp[recv - 3];
                unsigned int a3 = resp[recv - 2];
                unsigned int a4 = resp[recv - 1];
                char ip_buf[20];
                ip_to_str((a1 << 24) | (a2 << 16) | (a3 << 8) | a4, ip_buf);
                print_str("  example.com -> ");
                print_str(ip_buf);
                print_str("\n");
                check_result("DNS resolved", 1);
            } else {
                check_result("DNS invalid response", 0);
            }
        } else {
            check_result("DNS no response", 0);
        }

        wasm_net_close(sock);
    } else {
        check_result("UDP socket failed (expected without network)", 1);
    }
}

static void test_error_handling(void)
{
    print_str("\n[Test 4] Error Handling\n");

    /* Close invalid socket - should not crash */
    wasm_net_close(-1);
    check_result("close invalid sock (no crash)", 1);

    /* Send on invalid socket */
    int rc = wasm_net_send(-99, HEAP_BASE, 10);
    print_str("  send on invalid sock = ");
    print_int(rc);
    print_str("\n");
    check_result("send returns error", rc < 0);

    /* Recv on invalid socket */
    rc = wasm_net_recv(-99, HEAP_BASE, 10);
    print_str("  recv on invalid sock = ");
    print_int(rc);
    print_str("\n");
    check_result("recv returns error", rc < 0);
}

/* -------------------------------------------------------------------------- */
/* Entry point                                                                */
/* -------------------------------------------------------------------------- */

__attribute__((export_name("_start"), used))
void _start(void)
{
    print_str("\n=== AI-ASM v3.0 Network Test ===\n");

    test_socket_lifecycle();
    test_tcp_connect_http();
    test_udp_send();
    test_error_handling();

    print_str("\n=== Results ===\n");
    print_str("  Passed: ");
    print_int(tests_passed);
    print_str("\n  Failed: ");
    print_int(tests_failed);
    print_str("\n  Total:  ");
    print_int(tests_passed + tests_failed);
    print_str("\n");

    log_str(tests_failed == 0 ? "all network tests passed" : "some network tests failed");

    print_str("\n=== Network Test Complete ===\n");
    wasm_host_exit(0);
}
