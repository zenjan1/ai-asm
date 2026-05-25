/*
 * aiasm-aarch64/modules/dns_resolver/src/main.c
 * Minimal DNS resolver WASM module.
 * Sends A-record queries via UDP to DNS server (10.0.2.3:53).
 * Parses response, returns resolved IP address.
 */

__attribute__((import_module("host"), import_name("net_connect")))
extern int wasm_host_net_connect(unsigned int ip, unsigned int port, unsigned int proto);

__attribute__((import_module("host"), import_name("net_send")))
extern int wasm_host_net_send(int sock, unsigned int buf_off, unsigned int len);

__attribute__((import_module("host"), import_name("net_recv")))
extern int wasm_host_net_recv(int sock, unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("net_close")))
extern void wasm_host_net_close(int sock);

__attribute__((import_module("host"), import_name("print")))
extern void wasm_host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("sleep")))
extern void wasm_host_sleep(unsigned int ms);

__attribute__((import_module("host"), import_name("log")))
extern void wasm_host_log(unsigned int level_off, unsigned int level_len,
                          unsigned int msg_off, unsigned int msg_len);

/* Fixed buffer layout (avoid overlap):
 * 0x00000 - 0x001FF: 512 bytes DNS query buffer
 * 0x00200 - 0x003FF: 512 bytes DNS response buffer
 * 0x00400 - 0x004FF: 256 bytes string/scratch buffer
 */
#define BUF_QUERY   0
#define BUF_RESP    512
#define BUF_SCRATCH 1024

static unsigned int my_strlen(const char *s)
{
    unsigned int len = 0;
    while (s[len]) len++;
    return len;
}

static void print_str(const char *s)
{
    unsigned int len = my_strlen(s);
    char *dst = (char *)BUF_SCRATCH;
    for (unsigned int i = 0; i < len; i++)
        dst[i] = s[i];
    wasm_host_print(BUF_SCRATCH, len);
}

static void print_ip(unsigned int ip)
{
    unsigned int b1 = (ip >> 24) & 0xFF;
    unsigned int b2 = (ip >> 16) & 0xFF;
    unsigned int b3 = (ip >> 8) & 0xFF;
    unsigned int b4 = ip & 0xFF;
    char *buf = (char *)BUF_SCRATCH;
    int pos = 0;

    /* Format each octet */
    unsigned int octets[4] = {b1, b2, b3, b4};
    for (int o = 0; o < 4; o++) {
        unsigned int v = octets[o];
        if (v >= 100) { buf[pos++] = (char)('0' + (v / 100)); v %= 100; }
        if (v >= 10)  { buf[pos++] = (char)('0' + (v / 10)); v %= 10; }
        buf[pos++] = (char)('0' + v);
        if (o < 3) buf[pos++] = '.';
    }
    buf[pos++] = '\n';
    wasm_host_print(BUF_SCRATCH, pos);
}

/* Copy string to buffer at dst, return length */
static unsigned int buf_str(unsigned int dst, const char *s)
{
    unsigned int i = 0;
    while (s[i]) { ((char *)dst)[i] = s[i]; i++; }
    ((char *)dst)[i] = '\0';
    return i;
}

/* Write u16 big-endian */
static void write_u16_be(unsigned int dst, unsigned int v)
{
    ((char *)dst)[0] = (char)((v >> 8) & 0xFF);
    ((char *)dst)[1] = (char)(v & 0xFF);
}

/* Read u16 big-endian */
static unsigned int read_u16_be(unsigned int src)
{
    return (((unsigned char *)((char *)src))[0] << 8) |
            ((unsigned char *)((char *)src))[1];
}

/* Read u32 big-endian */
static unsigned int read_u32_be(unsigned int src)
{
    return (((unsigned char *)((char *)src))[0] << 24) |
           (((unsigned char *)((char *)src))[1] << 16) |
           (((unsigned char *)((char *)src))[2] << 8) |
            ((unsigned char *)((char *)src))[3];
}

/* Encode DNS question name: "www.example.com" -> 3www7example3com0 */
static unsigned int encode_dns_name(unsigned int dst, const char *name)
{
    unsigned int pos = 0;
    const char *p = name;
    const char *start = name;

    while (*p) {
        if (*p == '.') {
            unsigned int label_len = (unsigned int)(p - start);
            ((char *)dst)[pos++] = (char)label_len;
            for (unsigned int i = 0; i < label_len; i++)
                ((char *)dst)[pos++] = start[i];
            start = p + 1;
        }
        p++;
    }
    /* Final label */
    unsigned int label_len = (unsigned int)(p - start);
    ((char *)dst)[pos++] = (char)label_len;
    for (unsigned int i = 0; i < label_len; i++)
        ((char *)dst)[pos++] = start[i];
    ((char *)dst)[pos++] = '\0'; /* terminator */

    return pos;
}

/* Build DNS query packet
 * Returns total packet length
 */
static unsigned int build_dns_query(unsigned int buf, const char *hostname, unsigned int txid)
{
    unsigned int pos = 0;

    /* DNS Header (12 bytes) */
    write_u16_be(buf + pos, txid);       /* Transaction ID */
    pos += 2;
    write_u16_be(buf + pos, 0x0100);     /* Flags: standard query, recursion desired */
    pos += 2;
    write_u16_be(buf + pos, 1);          /* QDCOUNT = 1 */
    pos += 2;
    write_u16_be(buf + pos, 0);          /* ANCOUNT = 0 */
    pos += 2;
    write_u16_be(buf + pos, 0);          /* NSCOUNT = 0 */
    pos += 2;
    write_u16_be(buf + pos, 0);          /* ARCOUNT = 0 */
    pos += 2;

    /* Question section */
    pos += encode_dns_name(buf + pos, hostname);

    /* QTYPE = A (1), QCLASS = IN (1) */
    write_u16_be(buf + pos, 1);          /* Type A */
    pos += 2;
    write_u16_be(buf + pos, 1);          /* Class IN */
    pos += 2;

    return pos;
}

/* Parse DNS response, extract first A record IP
 * Returns IP address in host byte order, or 0 on failure
 */
static unsigned int parse_dns_response(unsigned int buf, int resp_len)
{
    /* Skip header (12 bytes) */
    unsigned int pos = 12;
    unsigned int qdcount = read_u16_be(buf + 4);
    unsigned int ancount = read_u16_be(buf + 6);

    if (ancount == 0) return 0;

    /* Skip question section */
    for (unsigned int i = 0; i < qdcount; i++) {
        while (pos < (unsigned int)resp_len) {
            unsigned char label_len = (unsigned char)((char *)buf)[pos];
            pos++;
            if (label_len == 0) break;
            if (label_len & 0xC0) { pos++; break; } /* pointer */
            pos += label_len;
        }
        pos += 4; /* QTYPE + QCLASS */
    }

    /* Parse answer section - find first A record */
    for (unsigned int i = 0; i < ancount; i++) {
        if (pos + 10 >= (unsigned int)resp_len) break;

        /* Skip name (may be pointer) */
        unsigned char label = (unsigned char)((char *)buf)[pos];
        if (label & 0xC0) {
            pos += 2; /* pointer */
        } else {
            while (pos < (unsigned int)resp_len) {
                unsigned char ll = (unsigned char)((char *)buf)[pos];
                pos++;
                if (ll == 0) break;
                if (ll & 0xC0) { pos++; break; }
                pos += ll;
            }
        }

        unsigned int rtype = read_u16_be(buf + pos);
        pos += 2;
        pos += 2; /* RDLENGTH offset - skip class */
        pos += 4; /* TTL */
        unsigned int rdlength = read_u16_be(buf + pos);
        pos += 2;

        if (rtype == 1 && rdlength == 4) {
            /* A record */
            unsigned int ip = read_u32_be(buf + pos);
            return ip;
        }

        pos += rdlength;
    }

    return 0;
}

/* Print hex bytes of a buffer region */
static void print_hex_dump(unsigned int buf, int len)
{
    char *buf_c = (char *)buf;
    char *out = (char *)BUF_SCRATCH;
    int pos = 0;
    for (int i = 0; i < len; i++) {
        unsigned char b = (unsigned char)buf_c[i];
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

/* Resolve hostname to IP address
 * Returns IP in host byte order, or 0 on failure
 */
static unsigned int dns_resolve(const char *hostname)
{
    static unsigned int txid = 0x1234;
    txid++;

    print_str("  Query: ");
    print_str(hostname);
    print_str("\n");

    /* Connect to DNS server via UDP */
    int sock = wasm_host_net_connect(0x0A000203, 53, 17);
    if (sock < 0) {
        print_str("  DNS: connect failed\n");
        return 0;
    }
    print_str("  Socket: ");
    /* print sock fd */
    char fd_buf[8]; int fi = 0; int fv = sock;
    if (fv < 0) { fd_buf[fi++] = '-'; fv = -fv; }
    if (fv == 0) fd_buf[fi++] = '0';
    else { while (fv > 0) { fd_buf[fi++] = (char)('0' + (fv % 10)); fv /= 10; } }
    fd_buf[fi] = '\n';
    wasm_host_print((unsigned int)fd_buf, fi + 1);

    /* Build query */
    unsigned int qlen = build_dns_query(BUF_QUERY, hostname, txid);
    print_str("  Query length: ");
    /* print qlen */
    fi = 0; fv = qlen;
    if (fv == 0) fd_buf[fi++] = '0';
    else { while (fv > 0) { fd_buf[fi++] = (char)('0' + (fv % 10)); fv /= 10; } }
    fd_buf[fi] = '\n';
    wasm_host_print((unsigned int)fd_buf, fi + 1);

    print_str("  Query hex:\n");
    print_hex_dump(BUF_QUERY, qlen);

    /* Send query */
    int sent = wasm_host_net_send(sock, BUF_QUERY, qlen);
    if (sent <= 0) {
        print_str("  DNS: send failed\n");
        wasm_host_net_close(sock);
        return 0;
    }
    print_str("  Sent bytes: ");
    fi = 0; fv = sent;
    if (fv == 0) fd_buf[fi++] = '0';
    else { while (fv > 0) { fd_buf[fi++] = (char)('0' + (fv % 10)); fv /= 10; } }
    fd_buf[fi] = '\n';
    wasm_host_print((unsigned int)fd_buf, fi + 1);

    /* Wait for response */
    wasm_host_sleep(3000);

    /* Receive response */
    int rlen = wasm_host_net_recv(sock, BUF_RESP, 512);
    wasm_host_net_close(sock);

    if (rlen <= 0) {
        print_str("  DNS: recv failed/timeout\n");
        return 0;
    }

    print_str("  Response length: ");
    fi = 0; fv = rlen;
    if (fv == 0) fd_buf[fi++] = '0';
    else { while (fv > 0) { fd_buf[fi++] = (char)('0' + (fv % 10)); fv /= 10; } }
    fd_buf[fi] = '\n';
    wasm_host_print((unsigned int)fd_buf, fi + 1);

    print_str("  Response hex:\n");
    print_hex_dump(BUF_RESP, rlen);

    /* Parse response */
    unsigned int ip = parse_dns_response(BUF_RESP, rlen);

    /* Print DNS response header info */
    unsigned int flags = read_u16_be(BUF_RESP + 2);
    unsigned int ancount = read_u16_be(BUF_RESP + 6);
    print_str("  Flags: ");
    fi = 0; fv = flags;
    if (fv == 0) fd_buf[fi++] = '0';
    else { while (fv > 0) { fd_buf[fi++] = (char)('0' + (fv % 10)); fv /= 10; } }
    fd_buf[fi] = '\n';
    wasm_host_print((unsigned int)fd_buf, fi + 1);

    print_str("  Answers: ");
    fi = 0; fv = ancount;
    if (fv == 0) fd_buf[fi++] = '0';
    else { while (fv > 0) { fd_buf[fi++] = (char)('0' + (fv % 10)); fv /= 10; } }
    fd_buf[fi] = '\n';
    wasm_host_print((unsigned int)fd_buf, fi + 1);

    return ip;
}

__attribute__((export_name("_start")))
void _start(void)
{
    print_str("\n== AI-ASM DNS Resolver ==\n");

    /* Test: resolve multiple hostnames */
    unsigned int ip;

    ip = dns_resolve("example.com");
    if (ip) {
        print_str("  => ");
        print_ip(ip);
    } else {
        print_str("  => FAILED\n");
    }
    print_str("\n");

    ip = dns_resolve("google.com");
    if (ip) {
        print_str("  => ");
        print_ip(ip);
    } else {
        print_str("  => FAILED\n");
    }
    print_str("\n");

    /* Log result */
    wasm_host_log(BUF_SCRATCH, 4, BUF_SCRATCH + 5, 10);
}
