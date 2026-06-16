/* agent_marketplace: Agent publishing, discovery, transactions & reputation */

__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int off, unsigned int len);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_AGENTS 32
#define MAX_CAPS 8
#define MAX_CAP_LEN 32
#define MAX_NAME_LEN 48
#define MAX_DESC_LEN 128
#define MAX_TXNS 64
#define MAX_FEEDBACK 64
#define MAX_INPUT 512

#define STATUS_ACTIVE 0
#define STATUS_INACTIVE 1
#define TXN_PENDING 0
#define TXN_IN_PROGRESS 1
#define TXN_COMPLETED 2
#define TXN_REFUNDED 3

#define ONTOLOGY_NLP 0
#define ONTOLOGY_VISION 1
#define ONTOLOGY_REASON 2
#define ONTOLOGY_CODE 3
#define ONTOLOGY_DATA 4
#define ONTOLOGY_SECURITY 5
#define ONTOLOGY_OPTIMIZE 6
#define ONTOLOGY_GENERAL 7

typedef struct {
    int agent_id; char name[MAX_NAME_LEN]; char description[MAX_DESC_LEN];
    int ontology; int caps_count; char caps[MAX_CAPS][MAX_CAP_LEN];
    int price; int status; int reputation; int total_txns; int total_feedback;
} agent_listing_t;

typedef struct {
    int txn_id; int buyer_id; int seller_id; int amount;
    int status; char detail[MAX_DESC_LEN];
} transaction_t;

typedef struct {
    int feedback_id; int agent_id; int rating; char comment[MAX_DESC_LEN];
} feedback_t;

static unsigned int heap_pos = 65536;
static agent_listing_t listings[MAX_AGENTS];
static int listing_count = 0;
static transaction_t transactions[MAX_TXNS];
static int txn_count = 0, next_txn_id = 0;
static feedback_t feedbacks[MAX_FEEDBACK];
static int feedback_count = 0;

static unsigned int alloc(unsigned int n) {
    unsigned int p = heap_pos; heap_pos += n;
    heap_pos = (heap_pos + 15) & ~15u; return p;
}
static void copy_to_mem(const char *s, unsigned int d, unsigned int n) {
    char *p = (char *)d; for (unsigned int i = 0; i < n; i++) p[i] = s[i];
}
static unsigned int my_strlen(const char *s) { unsigned int n = 0; while (s[n]) n++; return n; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return 1; a++; b++; } return (*a != *b);
}
static void copy_string(char *d, const char *s, unsigned int m) {
    unsigned int i = 0; while (s[i] && i < m - 1) { d[i] = s[i]; i++; } d[i] = 0;
}
static void print_str(const char *s) {
    unsigned int l = my_strlen(s), b = alloc(l + 1);
    copy_to_mem(s, b, l + 1); host_print(b, l);
}
static void print_int(int n) {
    char b[16]; int i = 14, neg = 0; b[15] = 0;
    if (n < 0) { neg = 1; n = -n; }
    if (n == 0) b[i--] = '0';
    while (n > 0) { b[i--] = '0' + (n % 10); n /= 10; }
    if (neg) b[i--] = '-'; i++;
    unsigned int l = my_strlen(&b[i]), d = alloc(l + 1);
    copy_to_mem(&b[i], d, l + 1); host_print(d, l);
}

static const char *ontology_name(int o) {
    switch (o) {
        case ONTOLOGY_NLP: return "NLP"; case ONTOLOGY_VISION: return "Vision";
        case ONTOLOGY_REASON: return "Reasoning"; case ONTOLOGY_CODE: return "CodeGen";
        case ONTOLOGY_DATA: return "DataProc"; case ONTOLOGY_SECURITY: return "Security";
        case ONTOLOGY_OPTIMIZE: return "Optimizer"; case ONTOLOGY_GENERAL: return "General";
        default: return "Unknown";
    }
}
static const char *txn_status_name(int s) {
    switch (s) {
        case TXN_PENDING: return "pending"; case TXN_IN_PROGRESS: return "in_progress";
        case TXN_COMPLETED: return "completed"; case TXN_REFUNDED: return "refunded";
        default: return "unknown";
    }
}

static int publish_agent(const char *name, const char *desc, int ont,
                         int price, const char *c1, const char *c2) {
    if (listing_count >= MAX_AGENTS) return -1;
    agent_listing_t *a = &listings[listing_count];
    a->agent_id = listing_count; a->status = STATUS_ACTIVE; a->reputation = 50;
    a->total_txns = 0; a->total_feedback = 0; a->ontology = ont;
    a->price = price; a->caps_count = 0;
    copy_string(a->name, name, MAX_NAME_LEN);
    copy_string(a->description, desc, MAX_DESC_LEN);
    if (c1 && c1[0]) copy_string(a->caps[a->caps_count++], c1, MAX_CAP_LEN);
    if (c2 && c2[0]) copy_string(a->caps[a->caps_count++], c2, MAX_CAP_LEN);
    listing_count++;
    print_str("marketplace: published "); print_str(name);
    print_str(" ont="); print_str(ontology_name(ont));
    print_str(" price="); print_int(price); print_str("\n");
    return a->agent_id;
}

static int discover_agents(int ont, int max_price) {
    int found = 0;
    print_str("marketplace: discovery ont="); print_str(ontology_name(ont));
    print_str(" max_price="); print_int(max_price); print_str("\n");
    for (int i = 0; i < listing_count; i++) {
        agent_listing_t *a = &listings[i];
        if (a->status != STATUS_ACTIVE || a->ontology != ont || a->price > max_price) continue;
        print_str("marketplace:   ["); print_int(a->agent_id); print_str("] ");
        print_str(a->name); print_str(" price="); print_int(a->price);
        print_str(" rep="); print_int(a->reputation); print_str(" caps=");
        for (int c = 0; c < a->caps_count; c++) {
            if (c > 0) print_str(","); print_str(a->caps[c]);
        }
        print_str("\n"); found++;
    }
    print_str("marketplace: found "); print_int(found); print_str(" agents\n");
    return found;
}

static int create_transaction(int buyer_id, int seller_id) {
    if (txn_count >= MAX_TXNS || seller_id >= listing_count) return -1;
    agent_listing_t *s = &listings[seller_id];
    if (s->status != STATUS_ACTIVE) return -1;
    transaction_t *t = &transactions[txn_count];
    t->txn_id = next_txn_id++; t->buyer_id = buyer_id;
    t->seller_id = seller_id; t->amount = s->price; t->status = TXN_PENDING;
    copy_string(t->detail, "invocation of agent", MAX_DESC_LEN); txn_count++;
    print_str("marketplace: txn "); print_int(t->txn_id);
    print_str(" buyer="); print_int(buyer_id);
    print_str(" seller="); print_int(seller_id);
    print_str(" amt="); print_int(t->amount); print_str("\n");
    return t->txn_id;
}

static int execute_transaction(int txn_id) {
    for (int i = 0; i < txn_count; i++) {
        transaction_t *t = &transactions[i];
        if (t->txn_id != txn_id || t->status != TXN_PENDING) continue;
        print_str("marketplace: executing txn "); print_int(txn_id); print_str("\n");
        t->status = TXN_COMPLETED; listings[t->seller_id].total_txns++;
        print_str("marketplace: txn "); print_int(txn_id); print_str(" completed\n");
        return 0;
    }
    return -1;
}

static int refund_transaction(int txn_id) {
    for (int i = 0; i < txn_count; i++) {
        transaction_t *t = &transactions[i];
        if (t->txn_id != txn_id || t->status != TXN_COMPLETED) continue;
        t->status = TXN_REFUNDED;
        agent_listing_t *s = &listings[t->seller_id];
        if (s->total_txns > 0) s->total_txns--;
        print_str("marketplace: txn "); print_int(txn_id); print_str(" refunded\n");
        return 0;
    }
    return -1;
}

static int submit_feedback(int agent_id, int rating, const char *comment) {
    if (feedback_count >= MAX_FEEDBACK || agent_id >= listing_count) return -1;
    if (rating < 1) rating = 1; if (rating > 5) rating = 5;
    feedback_t *f = &feedbacks[feedback_count++];
    f->feedback_id = feedback_count - 1; f->agent_id = agent_id; f->rating = rating;
    copy_string(f->comment, comment, MAX_DESC_LEN);
    agent_listing_t *a = &listings[agent_id];
    a->reputation = (a->reputation * 3 + rating * 20) / 4; a->total_feedback++;
    print_str("marketplace: feedback agent="); print_int(agent_id);
    print_str(" rating="); print_int(rating);
    print_str(" rep="); print_int(a->reputation); print_str("\n");
    return 0;
}

static void show_reputation(void) {
    print_str("marketplace: reputation table\n");
    for (int i = 0; i < listing_count; i++) {
        agent_listing_t *a = &listings[i];
        print_str("marketplace:   ["); print_int(a->agent_id); print_str("] ");
        print_str(a->name); print_str(" rep="); print_int(a->reputation);
        print_str(" txns="); print_int(a->total_txns);
        print_str(" fb="); print_int(a->total_feedback); print_str("\n");
    }
}

static void list_all(int verbose) {
    print_str("marketplace: listings "); print_int(listing_count);
    print_str("/"); print_int(MAX_AGENTS); print_str("\n");
    for (int i = 0; i < listing_count; i++) {
        agent_listing_t *a = &listings[i];
        print_str("marketplace:   "); print_int(a->agent_id); print_str(" ");
        print_str(a->name); print_str(" ["); print_str(ontology_name(a->ontology));
        print_str("] price="); print_int(a->price);
        print_str(" rep="); print_int(a->reputation);
        if (verbose) { print_str(" caps=");
            for (int c = 0; c < a->caps_count; c++) {
                if (c > 0) print_str(","); print_str(a->caps[c]);
            }
        }
        print_str("\n");
    }
    print_str("marketplace: txns "); print_int(txn_count);
    print_str("/"); print_int(MAX_TXNS); print_str("\n");
    for (int i = 0; i < txn_count; i++) {
        transaction_t *t = &transactions[i];
        print_str("marketplace:   txn "); print_int(t->txn_id);
        print_str(" buyer="); print_int(t->buyer_id);
        print_str(" seller="); print_int(t->seller_id);
        print_str(" amt="); print_int(t->amount);
        print_str(" "); print_str(txn_status_name(t->status)); print_str("\n");
    }
}

static void show_help(void) {
    print_str("agent_marketplace: Agent marketplace (v1.0)\n");
    print_str("  -t test  -l list  -v verbose  -i info  -s stats  -r reputation\n");
}
static void show_info(void) {
    print_str("marketplace: agents=32 txns=64 feedbacks=64 ontologies=8\n");
}
static void show_stats(void) {
    int active = 0, completed = 0, pending = 0;
    for (int i = 0; i < listing_count; i++) if (listings[i].status == STATUS_ACTIVE) active++;
    for (int i = 0; i < txn_count; i++) {
        if (transactions[i].status == TXN_COMPLETED) completed++;
        if (transactions[i].status == TXN_PENDING) pending++;
    }
    print_str("marketplace: listings="); print_int(listing_count);
    print_str(" active="); print_int(active);
    print_str(" txns="); print_int(txn_count);
    print_str(" done="); print_int(completed);
    print_str(" pending="); print_int(pending);
    print_str(" fb="); print_int(feedback_count); print_str("\n");
}

static void test_mode(int verbose) {
    print_str("marketplace: testing\n");
    print_str("marketplace: --- publish ---\n");
    int a_nlp  = publish_agent("nlp-summarizer", "Text summarization", ONTOLOGY_NLP, 10, "summarize", "translate");
    int a_vis  = publish_agent("image-detector", "Object detection", ONTOLOGY_VISION, 25, "detect", "classify");
    int a_code = publish_agent("code-reviewer", "Code review", ONTOLOGY_CODE, 15, "review", "lint");
    int a_data = publish_agent("data-cleaner", "ETL cleaning", ONTOLOGY_DATA, 8, "clean", "transform");
    int a_sec  = publish_agent("audit-scanner", "Security scan", ONTOLOGY_SECURITY, 30, "scan", "audit");
    if (a_nlp >= 0 && a_vis >= 0 && a_code >= 0 && a_data >= 0 && a_sec >= 0)
        print_str("marketplace: publish: OK\n");

    print_str("marketplace: --- discover ---\n");
    int fn = discover_agents(ONTOLOGY_NLP, 100);
    int fv = discover_agents(ONTOLOGY_VISION, 20);
    if (fn > 0) print_str("marketplace: discover NLP: OK\n");
    if (fv == 0) print_str("marketplace: discover Vision filtered: OK\n");

    print_str("marketplace: --- transactions ---\n");
    int t1 = create_transaction(99, a_nlp);
    int t2 = create_transaction(99, a_code);
    int t3 = create_transaction(98, a_data);
    if (t1 >= 0 && t2 >= 0 && t3 >= 0) print_str("marketplace: create txns: OK\n");
    if (execute_transaction(t1) == 0 && execute_transaction(t2) == 0 && execute_transaction(t3) == 0)
        print_str("marketplace: execute txns: OK\n");
    if (refund_transaction(t2) == 0) print_str("marketplace: refund: OK\n");

    print_str("marketplace: --- reputation ---\n");
    submit_feedback(a_nlp,  5, "Excellent summarization");
    submit_feedback(a_nlp,  4, "Fast but minor gaps");
    submit_feedback(a_code, 5, "Caught all bugs");
    submit_feedback(a_data, 3, "Adequate but slow");
    submit_feedback(a_sec,  5, "Found critical vuln");
    submit_feedback(a_nlp,  4, "Consistent results");
    print_str("marketplace: feedback: OK\n");

    if (verbose) list_all(1);
    show_reputation();
    show_stats();
    print_str("marketplace: test complete\n");
}

void _start(void) {
    unsigned int buf = alloc(MAX_INPUT);
    host_get_argv(buf, MAX_INPUT);
    int help = 0, info = 0, verbose = 0, test = 0, list = 0, stats = 0, rep = 0;
    unsigned int pos = 0; char *ap = (char *)buf;
    while (pos < MAX_INPUT && ap[pos]) pos++; pos++;
    while (pos < MAX_INPUT && ap[pos]) {
        char *a = &ap[pos];
        if (my_strcmp(a, "-h") == 0 || my_strcmp(a, "--help") == 0) help = 1;
        else if (my_strcmp(a, "-i") == 0) info = 1;
        else if (my_strcmp(a, "-v") == 0) verbose = 1;
        else if (my_strcmp(a, "-t") == 0) test = 1;
        else if (my_strcmp(a, "-l") == 0) list = 1;
        else if (my_strcmp(a, "-s") == 0) stats = 1;
        else if (my_strcmp(a, "-r") == 0) rep = 1;
        while (pos < MAX_INPUT && ap[pos]) pos++; pos++;
    }
    if (help)  { show_help(); host_exit(0); }
    if (info)  { show_info(); host_exit(0); }
    if (stats) { show_stats(); host_exit(0); }
    if (rep)   { show_reputation(); host_exit(0); }
    if (test)  { test_mode(verbose); host_exit(0); }
    if (list)  { list_all(verbose); host_exit(0); }
    print_str("marketplace: usage: agent_marketplace [-t -l -v -i -s -r]\n");
    host_exit(0);
}
