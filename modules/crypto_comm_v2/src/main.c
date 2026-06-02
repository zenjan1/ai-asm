/* crypto_comm_v2: Enhanced cryptographic communication (v34.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_CHANNELS   32
#define MAX_KEYS       48
#define MAX_MSG        128

/* Encryption algorithms */
#define ALGO_AES_GCM       0
#define ALGO_CHACHA20      1
#define ALGO_KYBER         2  /* Post-quantum KEM */
#define ALGO_DILITHIUM     3  /* Post-quantum signature */
#define ALGO_X25519        4  /* Key exchange */

/* Channel status */
#define CH_INACTIVE    0
#define CH_ACTIVE      1
#define CH_REKEYING    2
#define CH_CLOSED      3

/* Key status */
#define KEY_ACTIVE     0
#define KEY_EXPIRED    1
#define KEY_ROTATED    2
#define KEY_REVOKED    3

/* Secure channel */
typedef struct {
    int   channel_id;
    int   algorithm;
    int   key_id;
    int   status;
    int   latency_ms;
    int   bytes_encrypted;
    int   bytes_decrypted;
    int   error_count;
} secure_channel_t;

/* Key entry */
typedef struct {
    int   key_id;
    int   algorithm;
    int   created_at;
    int   expires_at;
    int   rotated;         /* rotation count */
    int   usage_count;
    int   status;
} key_entry_t;

/* Crypto stats */
typedef struct {
    int   enc_ops;
    int   dec_ops;
    int   avg_latency_ms;
    int   errors;
    int   key_rotations;
} crypto_stats_t;

static secure_channel_t channels[MAX_CHANNELS];
static int channel_count = 0;
static int next_channel_id = 1;

static key_entry_t keys[MAX_KEYS];
static int key_count = 0;
static int next_key_id = 1;

static crypto_stats_t global_stats = {0, 0, 0, 0, 0};

/* String utilities */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
}
static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0; while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; } dst[i] = '\0';
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

static const char *algo_name(int a) {
    if (a == ALGO_AES_GCM) return "AES-256-GCM";
    if (a == ALGO_CHACHA20) return "ChaCha20-Poly1305";
    if (a == ALGO_KYBER) return "Kyber-1024 (PQC)";
    if (a == ALGO_DILITHIUM) return "Dilithium-5 (PQC)";
    return "X25519";
}

static const char *channel_status_name(int s) {
    if (s == CH_INACTIVE) return "inactive";
    if (s == CH_ACTIVE) return "active";
    if (s == CH_REKEYING) return "rekeying";
    return "closed";
}

static const char *key_status_name(int s) {
    if (s == KEY_ACTIVE) return "active";
    if (s == KEY_EXPIRED) return "expired";
    if (s == KEY_ROTATED) return "rotated";
    return "revoked";
}

/* Generate key */
int cc2_create_key(int algorithm, int lifetime) {
    if (key_count >= MAX_KEYS) return -1;
    int idx = key_count;
    keys[idx].key_id = next_key_id++;
    keys[idx].algorithm = algorithm;
    keys[idx].created_at = key_count + 1;
    keys[idx].expires_at = keys[idx].created_at + lifetime;
    keys[idx].rotated = 0;
    keys[idx].usage_count = 0;
    keys[idx].status = KEY_ACTIVE;
    key_count++;
    return keys[idx].key_id;
}

/* Create secure channel */
int cc2_create_channel(int algorithm, int key_id) {
    if (channel_count >= MAX_CHANNELS) return -1;
    int idx = channel_count;
    channels[idx].channel_id = next_channel_id++;
    channels[idx].algorithm = algorithm;
    channels[idx].key_id = key_id;
    channels[idx].status = CH_ACTIVE;
    channels[idx].latency_ms = 2;
    channels[idx].bytes_encrypted = 0;
    channels[idx].bytes_decrypted = 0;
    channels[idx].error_count = 0;
    channel_count++;
    return channels[idx].channel_id;
}

/* Encrypt on channel */
int cc2_encrypt(int channel_id, int data_size) {
    for (int i = 0; i < channel_count; i++) {
        if (channels[i].channel_id == channel_id) {
            if (channels[i].status != CH_ACTIVE) return -1;
            channels[i].bytes_encrypted += data_size;
            channels[i].latency_ms = (channels[i].latency_ms + 3) / 2;
            global_stats.enc_ops++;
            global_stats.avg_latency_ms = (global_stats.avg_latency_ms + channels[i].latency_ms) / 2;
            for (int j = 0; j < key_count; j++) {
                if (keys[j].key_id == channels[i].key_id) keys[j].usage_count++;
            }
            return data_size;
        }
    }
    return -1;
}

/* Decrypt on channel */
int cc2_decrypt(int channel_id, int data_size) {
    for (int i = 0; i < channel_count; i++) {
        if (channels[i].channel_id == channel_id) {
            if (channels[i].status != CH_ACTIVE) return -1;
            channels[i].bytes_decrypted += data_size;
            global_stats.dec_ops++;
            return data_size;
        }
    }
    return -1;
}

/* Rotate key */
int cc2_rotate_key(int key_id) {
    for (int i = 0; i < key_count; i++) {
        if (keys[i].key_id == key_id) {
            if (keys[i].status != KEY_ACTIVE) return -1;
            keys[i].status = KEY_ROTATED;
            keys[i].rotated++;
            global_stats.key_rotations++;
            /* Create replacement key with same algorithm */
            int new_key = cc2_create_key(keys[i].algorithm, 100);
            print_str("    Key #");
            print_int(key_id);
            print_str(" rotated (");
            print_int(keys[i].rotated);
            print_str(" times), new key #");
            print_int(new_key);
            print_str("\n");
            return new_key;
        }
    }
    return -1;
}

/* Channel statistics */
int cc2_channel_stats(void) {
    print_str("  Channel Statistics\n");
    print_str("  ==========================================================================\n");
    print_str("  ID  Algorithm               Status    Latency  Enc(KB)  Dec(KB)  Errors\n");
    print_str("  --------------------------------------------------------------------------\n");
    int total_enc = 0, total_dec = 0;
    for (int i = 0; i < channel_count; i++) {
        print_int(channels[i].channel_id);
        print_str("  ");
        print_str(algo_name(channels[i].algorithm));
        int len = my_strlen(algo_name(channels[i].algorithm));
        for (int p = 0; p < 24 - len; p++) print_str(" ");
        print_str(channel_status_name(channels[i].status));
        if (channels[i].status == CH_ACTIVE) print_str("   ");
        else print_str("  ");
        print_int(channels[i].latency_ms);
        print_str("ms     ");
        print_int(channels[i].bytes_encrypted / 1024);
        print_str("      ");
        print_int(channels[i].bytes_decrypted / 1024);
        print_str("      ");
        print_int(channels[i].error_count);
        print_str("\n");
        total_enc += channels[i].bytes_encrypted;
        total_dec += channels[i].bytes_decrypted;
    }
    print_str("  Total encrypted: "); print_int(total_enc / 1024); print_str(" KB\n");
    print_str("  Total decrypted: "); print_int(total_dec / 1024); print_str(" KB\n");
    return channel_count;
}

/* Global crypto stats */
int cc2_global_stats(void) {
    print_str("  Global Crypto Statistics\n");
    print_str("  =================================================\n");
    print_str("  Encrypt operations: "); print_int(global_stats.enc_ops); print_str("\n");
    print_str("  Decrypt operations: "); print_int(global_stats.dec_ops); print_str("\n");
    print_str("  Average latency: "); print_int(global_stats.avg_latency_ms); print_str(" ms\n");
    print_str("  Key rotations: "); print_int(global_stats.key_rotations); print_str("\n");
    print_str("  Active keys: ");
    int active = 0;
    for (int i = 0; i < key_count; i++) if (keys[i].status == KEY_ACTIVE) active++;
    print_int(active);
    print_str("/");
    print_int(key_count);
    print_str("\n");
    return global_stats.enc_ops + global_stats.dec_ops;
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
    print_str("Crypto Comm v2 v34.0 - Enhanced Cryptographic Communication\n");
    if (help) {
        print_str("Usage: crypto_comm_v2 [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run crypto test\n");
        return;
    }
    if (test) {
        print_str("=== Crypto Comm v2 Test ===\n\n");
        /* Create keys with different algorithms */
        int k1 = cc2_create_key(ALGO_AES_GCM, 100);
        int k2 = cc2_create_key(ALGO_CHACHA20, 100);
        int k3 = cc2_create_key(ALGO_KYBER, 200);     /* Post-quantum */
        int k4 = cc2_create_key(ALGO_DILITHIUM, 200); /* Post-quantum sig */
        print_str("  Keys created: "); print_int(key_count); print_str("\n\n");
        /* Create secure channels */
        int c1 = cc2_create_channel(ALGO_AES_GCM, k1);
        int c2 = cc2_create_channel(ALGO_CHACHA20, k2);
        int c3 = cc2_create_channel(ALGO_KYBER, k3);
        print_str("  Channels established: "); print_int(channel_count); print_str("\n\n");
        /* Encrypt/decrypt operations */
        print_str("Crypto operations:\n");
        cc2_encrypt(c1, 4096);
        cc2_encrypt(c1, 2048);
        cc2_encrypt(c2, 8192);
        cc2_decrypt(c1, 4096);
        cc2_decrypt(c2, 8192);
        cc2_encrypt(c3, 1024);  /* Post-quantum channel */
        cc2_decrypt(c3, 1024);
        print_str("\n");
        /* Key rotation */
        print_str("Key rotation:\n");
        cc2_rotate_key(k1);
        cc2_rotate_key(k2);
        print_str("\n");
        /* Channel stats */
        cc2_channel_stats();
        print_str("\n");
        /* Global stats */
        cc2_global_stats();
        print_str("\n=== Crypto Comm v2 Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
