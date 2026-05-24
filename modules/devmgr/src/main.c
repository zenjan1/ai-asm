/*
 * aiasm-aarch64/modules/devmgr/src/main.c
 * Device manager module for AI-ASM AArch64 kernel v6.0
 *
 * Terminal-based device list display, hot-plug detection,
 * and driver registration/unregistration.
 */

/* -------------------------------------------------------------------------- */
/* WASM imports                                                               */
/* -------------------------------------------------------------------------- */

__attribute__((import_module("host"), import_name("print")))
extern void wasm_host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("getc")))
extern int wasm_host_getc(void);

__attribute__((import_module("host"), import_name("yield")))
extern void wasm_host_yield(void);

__attribute__((import_module("host"), import_name("sleep")))
extern void wasm_host_sleep(unsigned int ms);

__attribute__((import_module("host"), import_name("exit")))
extern void wasm_host_exit(int code);

__attribute__((import_module("host"), import_name("device_list")))
extern int wasm_host_device_list(unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("device_status")))
extern int wasm_host_device_status(int device_id);

__attribute__((import_module("host"), import_name("device_attach")))
extern int wasm_host_device_attach(unsigned int type_off, unsigned int type_len);

__attribute__((import_module("host"), import_name("device_detach")))
extern int wasm_host_device_detach(int device_id);

/* -------------------------------------------------------------------------- */
/* Constants                                                                  */
/* -------------------------------------------------------------------------- */

#define MAX_DEVICES 16
#define MAX_NAME  32

/* Device types */
#define DEV_TYPE_BLK   0
#define DEV_TYPE_NET   1
#define DEV_TYPE_GPU   2
#define DEV_TYPE_INPUT 3

typedef struct {
    char name[MAX_NAME];
    int type;
    int id;
    int status;  /* 0=disconnected, 1=connected, 2=active */
} device_t;

/* -------------------------------------------------------------------------- */
/* Helpers                                                                    */
/* -------------------------------------------------------------------------- */

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

static void print_str(const char *s)
{
    unsigned int len = my_strlen(s);
    char *dst = (char *)65536;
    for (unsigned int i = 0; i < len; i++) dst[i] = s[i];
    wasm_host_print(65536, len);
}

static void print_int(int v)
{
    char buf[12];
    int i = 0;
    if (v == 0) { buf[i++] = '0'; }
    else {
        int n = v;
        if (n < 0) { print_str("-"); n = -n; }
        while (n > 0) { buf[i++] = (char)('0' + (n % 10)); n /= 10; }
    }
    char *d = (char *)65536;
    for (int j = 0; j < i; j++) d[j] = buf[i - 1 - j];
    wasm_host_print(65536, i);
}

static void print_nl(void)
{
    wasm_host_print(65536, 1);
    ((char *)65536)[0] = '\n';
}

static const char *dev_type_str(int type)
{
    switch (type) {
        case DEV_TYPE_BLK:   return "BLK  ";
        case DEV_TYPE_NET:   return "NET  ";
        case DEV_TYPE_GPU:   return "GPU  ";
        case DEV_TYPE_INPUT: return "INPUT";
        default:             return "?????";
    }
}

static const char *dev_status_str(int status)
{
    switch (status) {
        case 0: return "disconnected";
        case 1: return "connected";
        case 2: return "active";
        default: return "unknown";
    }
}

/* -------------------------------------------------------------------------- */
/* Device list display                                                        */
/* -------------------------------------------------------------------------- */

static void show_devices(void)
{
    unsigned int buf_off = 65536 + 1;  /* leave null terminator */
    unsigned int buf_len = 1500;       /* ~1.5KB buffer */

    int count = wasm_host_device_list(buf_off, buf_len);

    print_str("\n--- VirtIO Device List ---\n");

    if (count <= 0) {
        print_str("  (no devices found)\n");
        return;
    }

    /* Header */
    print_str("  ID  Type   Status\n");
    print_str("  --  ----   ------\n");

    /* Parse device entries: each entry is "id,type,status\n" */
    char *buf = (char *)buf_off;
    int i = 0;
    while (i < count) {
        print_str("  ");

        /* Print ID */
        while (i < count && buf[i] >= '0' && buf[i] <= '9') {
            wasm_host_print(65536, 1);
            ((char *)65536)[0] = buf[i];
            i++;
        }
        if (i >= count) break;
        i++; /* skip comma */

        print_str("  ");

        /* Type string */
        int type = buf[i] - '0';
        i += 2; /* skip digit and comma */
        const char *ts = dev_type_str(type);
        print_str(ts);
        print_str("  ");

        /* Status */
        int status = buf[i] - '0';
        i++;
        if (i < count && buf[i] == '\n') i++;
        const char *ss = dev_status_str(status);
        print_str(ss);
        print_nl();
    }

    print_str("--------------------------\n");
}

/* -------------------------------------------------------------------------- */
/* Menu                                                                       */
/* -------------------------------------------------------------------------- */

static void show_menu(void)
{
    print_str("\n=== Device Manager ===\n");
    print_str("  l - List devices\n");
    print_str("  a - Attach device (virtio-blk/net/gpu)\n");
    print_str("  d - Detach device (by ID)\n");
    print_str("  r - Refresh (re-scan)\n");
    print_str("  q - Quit\n");
    print_str("Enter choice: ");
}

/* -------------------------------------------------------------------------- */
/* Entry point                                                                */
/* -------------------------------------------------------------------------- */

__attribute__((export_name("_start")))
void _start(void)
{
    print_str("\n=== AI-ASM v6.0 Device Manager ===\n");
    print_str("Managing VirtIO device lifecycle\n");

    show_menu();

    for (;;) {
        int ch = wasm_host_getc();
        if (ch > 0) {
            char c = (char)ch;
            switch (c) {
                case 'l': case 'L':
                    show_devices();
                    break;
                case 'a': case 'A':
                    print_str("\nAttach device type: ");
                    /* Default: attach virtio-blk */
                    print_str("virtio-blk (type=0)\n");
                    {
                        const char *type_str = "virtio-blk";
                        unsigned int tlen = my_strlen(type_str);
                        char *d = (char *)65536;
                        for (unsigned int i = 0; i < tlen; i++) d[i] = type_str[i];
                        int id = wasm_host_device_attach(65536, tlen);
                        print_str("  Attach result: ");
                        print_int(id);
                        print_nl();
                    }
                    break;
                case 'd': case 'D':
                    print_str("\nDetach device (ID 0): ");
                    {
                        int id = wasm_host_device_detach(0);
                        print_str("  Detach result: ");
                        print_int(id);
                        print_nl();
                    }
                    break;
                case 'r': case 'R':
                    show_devices();
                    break;
                case 'q': case 'Q':
                    print_str("Exiting device manager.\n");
                    wasm_host_exit(0);
                    return;
                case '\n': case '\r':
                    break;
                default:
                    print_str("Unknown option. ");
                    break;
            }
            show_menu();
        }
        wasm_host_yield();
        wasm_host_sleep(50);
    }
}
