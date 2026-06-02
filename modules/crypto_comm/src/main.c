/* crypto_comm: Encrypted communication module (v15.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Cipher types */
#define CIPHER_AES128  0
#define CIPHER_AES256  1
#define CIPHER_RSA     2

/* Cipher modes */
#define MODE_ECB  0
#define MODE_CBC  1
#define MODE_GCM  2

/* Protocol versions */
#define PROTO_TLS12  0
#define PROTO_TLS13  1

/* Limits */
#define MAX_CHANNELS   16
#define MAX_KEYS       32
#define KEY_ID_LEN     64
#define ENDPOINT_LEN   128
#define DATA_LEN       512
#define CRYPTO_DATA_LEN 1024

/* Crypto configuration */
typedef struct {
    int   cipher_type;
    int   key_length;
    int   mode;
    char  key_id[KEY_ID_LEN];
    int   rotation_interval;
} crypto_config_t;

/* Secure channel */
typedef struct {
    int   channel_id;
    char  endpoint[ENDPOINT_LEN];
    int   protocol;
    int   cert_verified;
    int   encrypted;
    int   handshake_complete;
    int   active;
} secure_channel_t;

static secure_channel_t channels[MAX_CHANNELS];
static int channel_count = 0;
static int next_channel_id = 1;

/* Key tracking */
static int key_count = 0;
static int key_rotations = 0;

/* String utilities */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
}
static void my_strcpy(char *dst, const char *src) { while (*src) *dst++ = *src++; *dst = '\0'; }
static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0; while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; } dst[i] = '\0';
}
static int my_strncmp(const char *a, const char *b, int n) {
    for (int i = 0; i < n; i++) {
        if (!a[i] || !b[i]) return (a[i] ? 1 : 0) - (b[i] ? 1 : 0);
        if (a[i] != b[i]) return a[i] - b[i];
    } return 0;
}

/* Print utilities */
static void print_str(const char *str) { host_print(str); }
static void print_int(int val) {
    char buf[32]; int pos = 0;
    if (val < 0) { buf[pos++] = '-'; val = -val; }
    if (val == 0) buf[pos++] = '0';
    else { int d = 0, t = val; while (t > 0) { d++; t /= 10; } pos += d; buf[pos] = '\0'; pos--;
        while (val > 0) { buf[pos--] = '0' + (val % 10); val /= 10; } }
    host_print(buf);
}

/* Get cipher name */
static const char *cipher_name(int type) {
    if (type == CIPHER_AES128) return "AES-128";
    if (type == CIPHER_AES256) return "AES-256";
    if (type == CIPHER_RSA) return "RSA";
    return "unknown";
}

/* Get mode name */
static const char *mode_name(int mode) {
    if (mode == MODE_ECB) return "ECB";
    if (mode == MODE_CBC) return "CBC";
    if (mode == MODE_GCM) return "GCM";
    return "unknown";
}

/* Encrypt data (simulated) */
int crypto_encrypt(const char *data, char *encrypted, crypto_config_t *config) {
    if (!data || !encrypted || !config) return -1;
    print_str("[crypto] encrypt: cipher=");
    print_str(cipher_name(config->cipher_type));
    print_str(" mode=");
    print_str(mode_name(config->mode));
    print_str(" key_len=");
    print_int(config->key_length);
    print_str(" bits\n");

    /* Simulate encryption by prefixing with cipher info */
    my_strcpy(encrypted, "[ENC:");
    if (config->cipher_type == CIPHER_AES128) my_strcpy(encrypted + 5, "A128]");
    else if (config->cipher_type == CIPHER_AES256) my_strcpy(encrypted + 5, "A256]");
    else my_strcpy(encrypted + 5, "RSA]");

    /* Copy encrypted data (simulated as transformed input) */
    int el = my_strlen(encrypted);
    int dl = my_strlen(data);
    int max_copy = CRYPTO_DATA_LEN - el - 1;
    if (dl > max_copy) dl = max_copy;
    for (int i = 0; i < dl; i++) {
        encrypted[el + i] = data[i] ^ 0x55; /* Simple XOR simulation */
    }
    encrypted[el + dl] = '\0';

    key_count++;
    print_str("[crypto] encrypted ");
    print_int(my_strlen(data));
    print_str(" bytes -> ");
    print_int(my_strlen(encrypted));
    print_str(" bytes\n");
    return 0;
}

/* Decrypt data (simulated) */
int crypto_decrypt(const char *encrypted, char *data, crypto_config_t *config) {
    if (!encrypted || !data || !config) return -1;
    print_str("[crypto] decrypt: cipher=");
    print_str(cipher_name(config->cipher_type));
    print_str("\n");

    /* Simulate decryption */
    const char *src = encrypted;
    if (my_strncmp(encrypted, "[ENC:", 5) == 0) {
        src = encrypted + 10; /* skip header */
    }

    int sl = my_strlen(src);
    int max_copy = DATA_LEN - 1;
    if (sl > max_copy) sl = max_copy;
    for (int i = 0; i < sl; i++) {
        data[i] = src[i] ^ 0x55;
    }
    data[sl] = '\0';

    print_str("[crypto] decrypted ");
    print_int(my_strlen(encrypted));
    print_str(" bytes -> ");
    print_int(my_strlen(data));
    print_str(" bytes\n");
    return 0;
}

/* Create a secure channel */
int crypto_create_secure_channel(const char *endpoint) {
    if (!endpoint || channel_count >= MAX_CHANNELS) return -1;
    int idx = channel_count;
    channels[idx].channel_id = next_channel_id;
    my_strncpy(channels[idx].endpoint, endpoint, ENDPOINT_LEN - 1);
    channels[idx].protocol = PROTO_TLS13;
    channels[idx].cert_verified = 1;
    channels[idx].encrypted = 1;
    channels[idx].handshake_complete = 1;
    channels[idx].active = 1;

    int id = next_channel_id++;
    channel_count++;

    print_str("[crypto] secure channel created: id=");
    print_int(id);
    print_str(" endpoint=");
    print_str(endpoint);
    print_str(" protocol=TLS1.3\n");
    return id;
}

/* Rotate a key */
int crypto_rotate_key(int key_id) {
    print_str("[crypto] rotating key id=");
    print_int(key_id);
    print_str("\n");
    key_rotations++;
    print_str("[crypto] key rotated successfully\n");
    return 0;
}

/* Verify certificate (simulated) */
int crypto_verify_certificate(const char *cert_path) {
    if (!cert_path) return -1;
    print_str("[crypto] verifying certificate: ");
    print_str(cert_path);
    print_str("\n");

    /* Simulate verification */
    int valid = 1;
    if (my_strncmp(cert_path, "/certs/trusted", 14) == 0) {
        print_str("[crypto] certificate VERIFIED\n");
    } else {
        print_str("[crypto] certificate verification: UNTRUSTED CA\n");
        valid = 0;
    }
    return valid ? 0 : -1;
}

/* List active channels */
void crypto_list_channels(void) {
    print_str("=== Secure Channels ===\n");
    print_str("ID\tENDPOINT\tPROTOCOL\tSTATUS\n");
    for (int i = 0; i < channel_count; i++) {
        print_str("  ");
        print_int(channels[i].channel_id);
        print_str("\t");
        print_str(channels[i].endpoint);
        print_str("\t");
        print_str(channels[i].protocol == PROTO_TLS12 ? "TLS1.2" : "TLS1.3");
        print_str("\t");
        print_str(channels[i].active ? "active" : "inactive");
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
    while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0) help = 1;
        else if (my_strcmp(arg, "-t") == 0 || my_strcmp(arg, "--test") == 0) test = 1;
        else if (my_strcmp(arg, "-l") == 0 || my_strcmp(arg, "--list") == 0) list = 1;
        while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    }
    print_str("Crypto Comm v15.0 - Encrypted Communication\n");
    if (help) {
        print_str("Usage: crypto_comm [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run crypto test\n");
        print_str("  -l, --list    List channels\n");
        return;
    }
    if (test) {
        print_str("=== Crypto Comm Test ===\n");
        print_str("\n--- Create Secure Channels ---\n");
        crypto_create_secure_channel("https://api.example.com");
        crypto_create_secure_channel("https://mqtt.internal.net");
        crypto_create_secure_channel("wss://agent.remote.io");

        print_str("\n--- Verify Certificates ---\n");
        crypto_verify_certificate("/certs/trusted/api.pem");
        crypto_verify_certificate("/certs/unknown/ca.pem");

        print_str("\n--- Encrypt/Decrypt Test ---\n");
        crypto_config_t cfg;
        cfg.cipher_type = CIPHER_AES256;
        cfg.key_length = 256;
        cfg.mode = MODE_GCM;
        my_strcpy(cfg.key_id, "key-001");
        cfg.rotation_interval = 30;

        char encrypted[CRYPTO_DATA_LEN];
        char decrypted[DATA_LEN];
        const char *msg = "Hello, secure world!";

        print_str("  Original: ");
        print_str(msg);
        print_str("\n");

        crypto_encrypt(msg, encrypted, &cfg);

        cfg.cipher_type = CIPHER_AES256;
        cfg.key_length = 256;
        cfg.mode = MODE_GCM;
        my_strcpy(cfg.key_id, "key-001");
        crypto_decrypt(encrypted, decrypted, &cfg);
        print_str("  Decrypted: ");
        print_str(decrypted);
        print_str("\n");

        print_str("\n--- Key Rotation ---\n");
        crypto_rotate_key(1);
        crypto_rotate_key(2);

        print_str("\n--- AES-128 Test ---\n");
        crypto_config_t cfg2;
        cfg2.cipher_type = CIPHER_AES128;
        cfg2.key_length = 128;
        cfg2.mode = MODE_CBC;
        my_strcpy(cfg2.key_id, "key-002");
        cfg2.rotation_interval = 7;

        crypto_encrypt("secret data", encrypted, &cfg2);

        print_str("\n--- Channel List ---\n");
        crypto_list_channels();

        print_str("\n=== Test Complete ===\n");
        return;
    }
    if (list) { crypto_list_channels(); return; }
    print_str("Use -h for help, -t for test, -l for list\n");
}
