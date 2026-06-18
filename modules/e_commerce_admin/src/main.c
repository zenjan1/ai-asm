/* e_commerce_admin: E-commerce administration system (v1.0)
 * Platform management, merchant management, transaction management, live commerce, rural e-commerce
 */
#include <stddef.h>

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_PLATFORM     16
#define MAX_MERCHANT     14
#define MAX_TRANSACTION  12
#define MAX_LIVE         10
#define MAX_RURAL        10

typedef struct {
    int    platform_id;
    int    platform_type;
    int    platform_category;
    int    platforms_registered;
    int    platform_operations;
    int    platform_credit;
    int    year;
    int    active;
} platform_t;

typedef struct {
    int    merchant_id;
    int    merchant_type;
    int    merchant_category;
    int    merchants_admitted;
    int    merchant_training;
    int    merchant_rating;
    int    year;
    int    active;
} merchant_t;

typedef struct {
    int    transaction_id;
    int    transaction_type;
    int    transaction_category;
    int    transaction_monitoring;
    int    transaction_disputes;
    int    transaction_statistics;
    int    year;
    int    active;
} transaction_t;

typedef struct {
    int    live_id;
    int    live_type;
    int    live_category;
    int    live_streamers;
    int    live_merchants;
    int    content_reviews;
    int    year;
    int    active;
} live_t;

typedef struct {
    int    rural_id;
    int    rural_type;
    int    rural_category;
    int    rural_platforms;
    int    agricultural_ecommerce;
    int    poverty_alleviation;
    int    year;
    int    active;
} rural_t;

typedef struct {
    int    n_platform;
    int    n_merchant;
    int    n_transaction;
    int    n_live;
    int    n_rural;
    int    total_registered;
    int    total_admitted;
    int    total_monitoring;
    int    total_streamers;
    int    total_rural;
} eca_state_t;

static platform_t platforms[MAX_PLATFORM];
static merchant_t merchants[MAX_MERCHANT];
static transaction_t transactions[MAX_TRANSACTION];
static live_t lives[MAX_LIVE];
static rural_t rurals[MAX_RURAL];
static eca_state_t eca;

static int initialized = 0;

static void print_str(const char* s) { host_print(s); }
static void print_int(int val) {
    char buf[32]; int i = 0;
    if (val < 0) { buf[i++] = '-'; val = -val; }
    if (val == 0) { buf[i++] = '0'; }
    else { int s = i; while (val > 0) { buf[i++] = '0' + (val % 10); val /= 10; }
           int e = i - 1; while (s < e) { char t = buf[s]; buf[s] = buf[e]; buf[e] = t; s++; e--; } }
    buf[i] = '\0'; host_print(buf);
}

int eca_init(void) {
    if (initialized) return -1;
    eca.n_platform = 0; eca.n_merchant = 0; eca.n_transaction = 0;
    eca.n_live = 0; eca.n_rural = 0;
    eca.total_registered = 0; eca.total_admitted = 0;
    eca.total_monitoring = 0; eca.total_streamers = 0;
    eca.total_rural = 0;
    for (int i = 0; i < MAX_PLATFORM; i++) platforms[i].active = 0;
    for (int i = 0; i < MAX_MERCHANT; i++) merchants[i].active = 0;
    for (int i = 0; i < MAX_TRANSACTION; i++) transactions[i].active = 0;
    for (int i = 0; i < MAX_LIVE; i++) lives[i].active = 0;
    for (int i = 0; i < MAX_RURAL; i++) rurals[i].active = 0;
    initialized = 1;
    print_str("[ECA] E-commerce initialized\n");
    return 0;
}

int eca_platform(int plt_type, int cat, int registered, int operations, int credit, int year) {
    if (eca.n_platform >= MAX_PLATFORM) return -1;
    platform_t* p = &platforms[eca.n_platform];
    p->platform_id = eca.n_platform;
    p->platform_type = plt_type;
    p->platform_category = cat;
    p->platforms_registered = registered;
    p->platform_operations = operations;
    p->platform_credit = credit;
    p->year = year;
    p->active = 1;
    eca.total_registered += registered;
    eca.n_platform++;
    print_str("[ECA] Platform "); print_int(eca.n_platform - 1);
    print_str(" type="); print_int(plt_type);
    print_str(" cat="); print_int(cat);
    print_str(" rgs="); print_int(registered);
    print_str(" ops="); print_int(operations);
    print_str(" crd="); print_int(credit); print_str("\n");
    return eca.n_platform - 1;
}

int eca_merchant(int mch_type, int cat, int admitted, int training, int rating, int year) {
    if (eca.n_merchant >= MAX_MERCHANT) return -1;
    merchant_t* m = &merchants[eca.n_merchant];
    m->merchant_id = eca.n_merchant;
    m->merchant_type = mch_type;
    m->merchant_category = cat;
    m->merchants_admitted = admitted;
    m->merchant_training = training;
    m->merchant_rating = rating;
    m->year = year;
    m->active = 1;
    eca.total_admitted += admitted;
    eca.n_merchant++;
    print_str("[ECA] Merchant "); print_int(eca.n_merchant - 1);
    print_str(" type="); print_int(mch_type);
    print_str(" cat="); print_int(cat);
    print_str(" adm="); print_int(admitted);
    print_str(" trn="); print_int(training);
    print_str(" rtg="); print_int(rating); print_str("\n");
    return eca.n_merchant - 1;
}

int eca_transaction(int trn_type, int cat, int monitoring, int disputes, int statistics, int year) {
    if (eca.n_transaction >= MAX_TRANSACTION) return -1;
    transaction_t* t = &transactions[eca.n_transaction];
    t->transaction_id = eca.n_transaction;
    t->transaction_type = trn_type;
    t->transaction_category = cat;
    t->transaction_monitoring = monitoring;
    t->transaction_disputes = disputes;
    t->transaction_statistics = statistics;
    t->year = year;
    t->active = 1;
    eca.total_monitoring += monitoring;
    eca.n_transaction++;
    print_str("[ECA] Transaction "); print_int(eca.n_transaction - 1);
    print_str(" type="); print_int(trn_type);
    print_str(" cat="); print_int(cat);
    print_str(" mnt="); print_int(monitoring);
    print_str(" dsp="); print_int(disputes);
    print_str(" sts="); print_int(statistics); print_str("\n");
    return eca.n_transaction - 1;
}

int eca_live(int lv_type, int cat, int streamers, int lv_merchants, int reviews, int year) {
    if (eca.n_live >= MAX_LIVE) return -1;
    live_t* l = &lives[eca.n_live];
    l->live_id = eca.n_live;
    l->live_type = lv_type;
    l->live_category = cat;
    l->live_streamers = streamers;
    l->live_merchants = lv_merchants;
    l->content_reviews = reviews;
    l->year = year;
    l->active = 1;
    eca.total_streamers += streamers;
    eca.n_live++;
    print_str("[ECA] Live "); print_int(eca.n_live - 1);
    print_str(" type="); print_int(lv_type);
    print_str(" cat="); print_int(cat);
    print_str(" stm="); print_int(streamers);
    print_str(" mch="); print_int(lv_merchants);
    print_str(" rvw="); print_int(reviews); print_str("\n");
    return eca.n_live - 1;
}

int eca_rural(int rur_type, int cat, int plt_rural, int agricultural, int poverty, int year) {
    if (eca.n_rural >= MAX_RURAL) return -1;
    rural_t* r = &rurals[eca.n_rural];
    r->rural_id = eca.n_rural;
    r->rural_type = rur_type;
    r->rural_category = cat;
    r->rural_platforms = plt_rural;
    r->agricultural_ecommerce = agricultural;
    r->poverty_alleviation = poverty;
    r->year = year;
    r->active = 1;
    eca.total_rural += plt_rural;
    eca.n_rural++;
    print_str("[ECA] Rural "); print_int(eca.n_rural - 1);
    print_str(" type="); print_int(rur_type);
    print_str(" cat="); print_int(cat);
    print_str(" plt="); print_int(plt_rural);
    print_str(" agr="); print_int(agricultural);
    print_str(" pvr="); print_int(poverty); print_str("\n");
    return eca.n_rural - 1;
}

void eca_platform_report(void) {
    print_str("[ECA] Platform report:\n");
    print_str("  Platform categories: "); print_int(eca.n_platform); print_str("\n");
    print_str("  Total platforms registered: "); print_int(eca.total_registered); print_str("\n");
}

void eca_merchant_report(void) {
    print_str("[ECA] Merchant report:\n");
    print_str("  Merchant categories: "); print_int(eca.n_merchant); print_str("\n");
    print_str("  Total merchants admitted: "); print_int(eca.total_admitted); print_str("\n");
}

void eca_rural_report(void) {
    print_str("[ECA] Rural report:\n");
    print_str("  Transaction categories: "); print_int(eca.n_transaction); print_str("\n");
    print_str("  Total transaction monitoring: "); print_int(eca.total_monitoring); print_str("\n");
    print_str("  Live commerce categories: "); print_int(eca.n_live); print_str("\n");
    print_str("  Total live streamers: "); print_int(eca.total_streamers); print_str("\n");
    print_str("  Rural e-commerce categories: "); print_int(eca.n_rural); print_str("\n");
    print_str("  Total rural platforms: "); print_int(eca.total_rural); print_str("\n");
}

void eca_print_state(void) {
    print_str("[ECA] Pl="); print_int(eca.n_platform);
    print_str(" Mc="); print_int(eca.n_merchant);
    print_str(" Tr="); print_int(eca.n_transaction);
    print_str(" Lv="); print_int(eca.n_live);
    print_str(" Rr="); print_int(eca.n_rural);
    print_str("\n");
}

int main(void) {
    print_str("=== E-Commerce Admin Demo ===\n\n");
    eca_init();

    print_str("Platform management...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int rgs = 20 + (i * 5);
        int ops = 50 + (i * 12);
        int crd = 30 + (i * 8);
        int year = 2020 + (i % 5);
        eca_platform(type, cat, rgs, ops, crd, year);
    }

    print_str("\nMerchant management...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int adm = 100 + (i * 25);
        int trn = 60 + (i * 15);
        int rtg = 40 + (i * 10);
        int year = 2021 + (i % 4);
        eca_merchant(type, cat, adm, trn, rtg, year);
    }

    print_str("\nTransaction management...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int mnt = 200 + (i * 50);
        int dsp = 30 + (i * 8);
        int sts = 100 + (i * 25);
        int year = 2022 + (i % 3);
        eca_transaction(type, cat, mnt, dsp, sts, year);
    }

    print_str("\nLive commerce...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int stm = 50 + (i * 12);
        int mch = 80 + (i * 20);
        int rvw = 40 + (i * 10);
        int year = 2023 + (i % 2);
        eca_live(type, cat, stm, mch, rvw, year);
    }

    print_str("\nRural e-commerce...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int plt_r = 15 + (i * 4);
        int agr = 25 + (i * 6);
        int pvr = 10 + (i * 3);
        int year = 2024;
        eca_rural(type, cat, plt_r, agr, pvr, year);
    }

    print_str("\nPlatform report...\n");
    eca_platform_report();

    print_str("\nMerchant report...\n");
    eca_merchant_report();

    print_str("\nRural report...\n");
    eca_rural_report();

    print_str("\nFinal state...\n");
    eca_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
