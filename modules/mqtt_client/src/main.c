/* mqtt_client: MQTT protocol client for IoT (v13.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);
extern void host_http_request(const char *method, const char *url, const char *body, char *response, int max_len);

/* MQTT constants */
#define MQTT_PORT_DEFAULT  1883
#define MQTT_KEEPALIVE_DEFAULT 60

/* MQTT packet types */
#define MQTT_CONNECT     1
#define MQTT_CONNACK     2
#define MQTT_PUBLISH     3
#define MQTT_PUBACK      4
#define MQTT_SUBSCRIBE   8
#define MQTT_SUBACK      9
#define MQTT_UNSUBSCRIBE 10
#define MQTT_UNSUBACK    11
#define MQTT_PINGREQ     12
#define MQTT_PINGRESP    13
#define MQTT_DISCONNECT  14

/* QoS levels */
#define MQTT_QOS_0  0   /* At most once */
#define MQTT_QOS_1  1   /* At least once */
#define MQTT_QOS_2  2   /* Exactly once */

/* Connection states */
#define MQTT_DISCONNECTED 0
#define MQTT_CONNECTING   1
#define MQTT_CONNECTED    2

/* Limits */
#define MAX_SUBSCRIPTIONS 16
#define MAX_PENDING_MSGS  32
#define TOPIC_LEN        128
#define PAYLOAD_LEN      1024
#define CLIENT_ID_LEN    32

/* MQTT message */
typedef struct {
    char  topic[TOPIC_LEN];
    char  payload[PAYLOAD_LEN];
    int   payload_len;
    int   qos;
    int   retained;
    int   timestamp;
    int   message_id;
} mqtt_message_t;

/* Subscription entry */
typedef struct {
    char  topic[TOPIC_LEN];
    int   qos;
    int   active;
} mqtt_subscription_t;

/* MQTT config */
typedef struct {
    char  broker_host[64];
    int   broker_port;
    char  client_id[CLIENT_ID_LEN];
    char  username[64];
    char  password[64];
    int   keepalive;
    int   clean_session;
} mqtt_config_t;

/* Client state */
typedef struct {
    int   state;
    char  broker_host[64];
    int   broker_port;
    char  client_id[CLIENT_ID_LEN];
    int   keepalive;
    int   next_message_id;
    int   published_count;
    int   received_count;
    int   reconnect_count;
    mqtt_subscription_t subscriptions[MAX_SUBSCRIPTIONS];
    mqtt_message_t pending_msgs[MAX_PENDING_MSGS];
    int   pending_count;
} mqtt_client_state_t;

static mqtt_client_state_t client;

/* String utilities */
static int my_strlen(const char *s) {
    int len = 0;
    while (s[len]) len++;
    return len;
}

static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) {
        if (*a != *b) return *a - *b;
        a++; b++;
    }
    return *a - *b;
}

static void my_strcpy(char *dst, const char *src) {
    while (*src) *dst++ = *src++;
    *dst = '\0';
}

static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0;
    while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; }
    dst[i] = '\0';
}

static int my_strncmp(const char *a, const char *b, int n) {
    for (int i = 0; i < n; i++) {
        if (!a[i] || !b[i]) return (a[i] ? 1 : 0) - (b[i] ? 1 : 0);
        if (a[i] != b[i]) return a[i] - b[i];
    }
    return 0;
}

/* Print utilities */
static void print_str(const char *str) { host_print(str); }
static void print_int(int val) {
    char buf[32];
    int pos = 0;
    if (val < 0) { buf[pos++] = '-'; val = -val; }
    if (val == 0) buf[pos++] = '0';
    else {
        int d = 0, t = val;
        while (t > 0) { d++; t /= 10; }
        pos += d;
        buf[pos] = '\0'; pos--;
        while (val > 0) { buf[pos--] = '0' + (val % 10); val /= 10; }
    }
    host_print(buf);
}

/* Check if topic matches a subscription pattern (supports + and # wildcards) */
static int topic_match(const char *pattern, const char *topic) {
    int pi = 0, ti = 0;
    while (pattern[pi] && topic[ti]) {
        if (pattern[pi] == '#') return 1; /* match rest */
        if (pattern[pi] == '+') {
            /* Skip to next / */
            while (topic[ti] && topic[ti] != '/') ti++;
            while (pattern[pi] && pattern[pi] != '/') pi++;
            continue;
        }
        if (pattern[pi] != topic[ti]) return 0;
        pi++; ti++;
    }
    return (pattern[pi] == '\0' && topic[ti] == '\0') ? 1 : 0;
}

/* Connect to MQTT broker
   config: connection configuration
   Returns: 0=success, -1=failure */
int mqtt_connect(mqtt_config_t *config) {
    if (!config) return -1;

    my_strncpy(client.broker_host, config->broker_host, 63);
    client.broker_port = config->broker_port ? config->broker_port : MQTT_PORT_DEFAULT;
    my_strncpy(client.client_id, config->client_id, CLIENT_ID_LEN - 1);
    client.keepalive = config->keepalive ? config->keepalive : MQTT_KEEPALIVE_DEFAULT;
    client.state = MQTT_CONNECTING;
    client.next_message_id = 1;
    client.published_count = 0;
    client.received_count = 0;
    client.pending_count = 0;

    /* Clear subscriptions */
    for (int i = 0; i < MAX_SUBSCRIPTIONS; i++) {
        client.subscriptions[i].active = 0;
    }

    /* In real impl, send MQTT CONNECT packet and wait for CONNACK */
    /* Build CONNECT packet (simplified) */
    char conn_buf[256];
    int pos = 0;
    conn_buf[pos++] = (MQTT_CONNECT << 4); /* Fixed header */
    conn_buf[pos++] = 0;                   /* Remaining length (placeholder) */
    conn_buf[pos++] = 0;                   /* Protocol name length MSB */
    conn_buf[pos++] = 4;                   /* Protocol name length LSB = 4 ("MQTT") */
    conn_buf[pos++] = 'M';
    conn_buf[pos++] = 'Q';
    conn_buf[pos++] = 'T';
    conn_buf[pos++] = 'T';
    conn_buf[pos++] = 4;                   /* Protocol level (4 = 3.1.1) */
    conn_buf[pos++] = 2;                   /* Connect flags (clean session) */
    conn_buf[pos++] = client.keepalive >> 8;  /* Keepalive MSB */
    conn_buf[pos++] = client.keepalive & 0xFF; /* Keepalive LSB */

    /* Client ID */
    int cid_len = my_strlen(client.client_id);
    conn_buf[pos++] = cid_len >> 8;
    conn_buf[pos++] = cid_len & 0xFF;
    for (int i = 0; i < cid_len; i++) conn_buf[pos++] = client.client_id[i];
    conn_buf[pos] = '\0';

    /* Send to broker via HTTP (WebSocket bridge) */
    char url[128];
    int up = 0;
    url[up++] = 'h'; url[up++] = 't'; url[up++] = 't'; url[up++] = 'p';
    url[up++] = ':'; url[up++] = '/'; url[up++] = '/';
    for (int i = 0; client.broker_host[i]; i++) url[up++] = client.broker_host[i];
    url[up++] = ':';
    int p = client.broker_port;
    if (p == 0) url[up++] = '0';
    else {
        int d = 0, t = p; char tmp[8];
        while (t > 0) { tmp[d++] = '0' + (t % 10); t /= 10; }
        while (d > 0) url[up++] = tmp[--d];
    }
    url[up++] = '/'; url[up++] = 'm'; url[up++] = 'q'; url[up++] = 't';
    url[up++] = 't'; url[up++] = '/'; url[up++] = 'c'; url[up++] = 'o';
    url[up++] = 'n'; url[up++] = 'n'; url[up++] = 'e'; url[up++] = 'c';
    url[up++] = 't'; url[up++] = '\0';

    char resp[128];
    host_http_request("POST", url, conn_buf, resp, 128);

    client.state = MQTT_CONNECTED;
    client.reconnect_count++;

    print_str("[mqtt] connected to ");
    print_str(client.broker_host);
    print_str(":");
    print_int(client.broker_port);
    print_str(" (client=");
    print_str(client.client_id);
    print_str(")\n");

    return 0;
}

/* Publish a message
   topic: topic, payload: message data, qos: QoS level
   Returns: 0=success, -1=failure */
int mqtt_publish(const char *topic, const char *payload, int qos) {
    if (client.state != MQTT_CONNECTED) {
        print_str("[mqtt] not connected\n");
        return -1;
    }

    int msg_id = client.next_message_id++;
    int plen = payload ? my_strlen(payload) : 0;

    print_str("[mqtt] publish: topic=");
    print_str(topic);
    print_str(" qos=");
    print_int(qos);
    print_str(" len=");
    print_int(plen);
    print_str(" id=");
    print_int(msg_id);
    print_str("\n");

    /* Store in pending for QoS 1/2 */
    if (qos > 0 && client.pending_count < MAX_PENDING_MSGS) {
        int idx = client.pending_count;
        my_strncpy(client.pending_msgs[idx].topic, topic, TOPIC_LEN - 1);
        if (payload) my_strncpy(client.pending_msgs[idx].payload, payload, PAYLOAD_LEN - 1);
        client.pending_msgs[idx].payload_len = plen;
        client.pending_msgs[idx].qos = qos;
        client.pending_msgs[idx].message_id = msg_id;
        client.pending_count++;
    }

    client.published_count++;
    return 0;
}

/* Subscribe to a topic
   topic: subscription topic, qos: QoS level, callback: message handler (placeholder)
   Returns: 0=success, -1=failure */
int mqtt_subscribe(const char *topic, int qos, void *callback) {
    if (client.state != MQTT_CONNECTED) return -1;

    /* Find free subscription slot */
    for (int i = 0; i < MAX_SUBSCRIPTIONS; i++) {
        if (!client.subscriptions[i].active) {
            my_strncpy(client.subscriptions[i].topic, topic, TOPIC_LEN - 1);
            client.subscriptions[i].qos = qos;
            client.subscriptions[i].active = 1;
            print_str("[mqtt] subscribed: ");
            print_str(topic);
            print_str(" qos=");
            print_int(qos);
            print_str("\n");
            return 0;
        }
    }
    print_str("[mqtt] max subscriptions reached\n");
    return -1;
}

/* Unsubscribe from a topic
   topic: topic to unsubscribe from
   Returns: 0=success, -1=failure */
int mqtt_unsubscribe(const char *topic) {
    for (int i = 0; i < MAX_SUBSCRIPTIONS; i++) {
        if (client.subscriptions[i].active &&
            my_strcmp(client.subscriptions[i].topic, topic) == 0) {
            client.subscriptions[i].active = 0;
            print_str("[mqtt] unsubscribed: ");
            print_str(topic);
            print_str("\n");
            return 0;
        }
    }
    return -1;
}

/* Disconnect from broker
   Returns: 0=success */
int mqtt_disconnect(void) {
    client.state = MQTT_DISCONNECTED;
    print_str("[mqtt] disconnected\n");
    return 0;
}

/* Process incoming messages (event loop)
   Returns: number of messages processed */
int mqtt_loop(void) {
    if (client.state != MQTT_CONNECTED) return 0;

    /* In real impl, read from TCP socket and parse MQTT packets */
    /* For simulation, check if any pending messages got ACKs */
    int processed = 0;
    for (int i = 0; i < client.pending_count; i++) {
        /* Simulate PUBACK received */
        if (client.pending_msgs[i].qos >= 1) {
            client.pending_msgs[i].qos = 0; /* mark as acknowledged */
            processed++;
        }
    }

    client.received_count += processed;

    if (processed > 0) {
        print_str("[mqtt] loop: processed ");
        print_int(processed);
        print_str(" messages\n");
    }

    return processed;
}

/* Print MQTT client status */
void mqtt_status(void) {
    print_str("=== MQTT Client ===\n");
    print_str("State: ");
    print_str(client.state == MQTT_CONNECTED ? "CONNECTED" :
              client.state == MQTT_CONNECTING ? "CONNECTING" : "DISCONNECTED");
    print_str("\nBroker: ");
    print_str(client.broker_host);
    print_str(":");
    print_int(client.broker_port);
    print_str("\nClient: ");
    print_str(client.client_id);
    print_str("\nPublished: ");
    print_int(client.published_count);
    print_str(" Received: ");
    print_int(client.received_count);
    print_str("\nSubscriptions:\n");
    for (int i = 0; i < MAX_SUBSCRIPTIONS; i++) {
        if (client.subscriptions[i].active) {
            print_str("  ");
            print_str(client.subscriptions[i].topic);
            print_str(" (qos=");
            print_int(client.subscriptions[i].qos);
            print_str(")\n");
        }
    }
}

/* ===== CLI entry ===== */
void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);

    int help = 0, test = 0, status = 0;
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;
    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0) help = 1;
        else if (my_strcmp(arg, "-t") == 0 || my_strcmp(arg, "--test") == 0) test = 1;
        else if (my_strcmp(arg, "-s") == 0 || my_strcmp(arg, "--status") == 0) status = 1;
        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    print_str("MQTT Client v13.0 - IoT Message Protocol\n");

    if (help) {
        print_str("Usage: mqtt_client [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run MQTT test\n");
        print_str("  -s, --status  Show client status\n");
        return;
    }

    if (test) {
        print_str("=== MQTT Client Test ===\n");

        print_str("\n--- Connect ---\n");
        mqtt_config_t cfg;
        my_strcpy(cfg.broker_host, "broker.hivemq.com");
        cfg.broker_port = 1883;
        my_strcpy(cfg.client_id, "aiasm-edge-001");
        my_strcpy(cfg.username, "");
        my_strcpy(cfg.password, "");
        cfg.keepalive = 60;
        cfg.clean_session = 1;
        mqtt_connect(&cfg);

        print_str("\n--- Subscribe ---\n");
        mqtt_subscribe("sensors/temperature", 1, 0);
        mqtt_subscribe("sensors/humidity", 1, 0);
        mqtt_subscribe("sensors/+/status", 0, 0);
        mqtt_subscribe("commands/#", 2, 0);

        print_str("\n--- Publish ---\n");
        mqtt_publish("sensors/temperature", "{\"value\":22.5,\"unit\":\"C\"}", 1);
        mqtt_publish("sensors/humidity", "{\"value\":65,\"unit\":\"%\"}", 1);
        mqtt_publish("alerts/overtemp", "{\"value\":85.2}", 2);
        mqtt_publish("status/online", "{\"uptime\":3600}", 0);

        print_str("\n--- Process Loop ---\n");
        mqtt_loop();

        print_str("\n--- Unsubscribe ---\n");
        mqtt_unsubscribe("sensors/humidity");

        print_str("\n--- Status ---\n");
        mqtt_status();

        print_str("\n--- Disconnect ---\n");
        mqtt_disconnect();

        print_str("\n=== Test Complete ===\n");
        return;
    }

    if (status) {
        mqtt_status();
        return;
    }

    print_str("Use -h for help, -t for test, -s for status\n");
}
