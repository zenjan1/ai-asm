/* nft_marketplace: NFT trading, auctions, royalties and metadata platform (v57.0)
 * AI-ASM OS module. Pure C, no stdlib. ERC721/ERC1155, English/Dutch/buy-now auctions,
 * royalty splits, IPFS metadata and market stats. */
#include <stddef.h>

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* ── Limits ────────────────────────────────────────────────────────────── */
#define MAX_TOKENS 256
#define MAX_COLLECTIONS 32
#define MAX_AUCTIONS 64
#define MAX_EVENTS 512
#define MAX_ATTRS 16
#define MAX_HOLDERS 64
#define MAX_TOKENS_COL 128
#define MAX_NAME_LEN 64
#define MAX_SYMBOL_LEN 16
#define MAX_URI_LEN 128
#define MAX_DESC_LEN 512
#define MAX_IMAGE_LEN 256
/* ── Enumerations ──────────────────────────────────────────────────────── */
#define STD_ERC721 0
#define STD_ERC1155 1
#define AUCTION_ENGLISH 0
#define AUCTION_DUTCH 1
#define AUCTION_BUY_NOW 2
#define AUCTION_ACTIVE 0
#define AUCTION_ENDED 1
#define AUCTION_CANCELLED 2
#define MEDIA_IMAGE 0
#define MEDIA_VIDEO 1
#define MEDIA_AUDIO 2
#define MEDIA_3D 3
#define MEDIA_UNKNOWN 4
#define EVT_MINT 0
#define EVT_SALE 1
#define EVT_TRANSFER 2
#define EVT_LIST 3
#define EVT_CANCEL 4
#define OK 0
#define ERR_NOT_FOUND -1
#define ERR_OWNER -2
#define ERR_APPROVAL -3
#define ERR_CAPACITY -4
#define ERR_BID_LOW -5
#define ERR_STATE -6
#define ERR_INVALID -7
#define ERR_MAX_SUPPLY -8

/* ── Data structures ───────────────────────────────────────────────────── */
typedef struct {
    unsigned int token_id; char owner[20]; char creator[20];
    char uri[MAX_URI_LEN]; int standard;
    unsigned int balance; int burned;
} nft_token_t;
typedef struct {
    char name[MAX_NAME_LEN]; char symbol[MAX_SYMBOL_LEN];
    unsigned int token_ids[MAX_TOKENS_COL];
    unsigned int token_count; unsigned int max_supply;
    unsigned int royalty_pct; int active;
} nft_collection_t;
typedef struct {
    unsigned int token_id; char seller[20]; int auction_type;
    unsigned int start_price; unsigned int reserve_price;
    unsigned int end_price; unsigned int current_bid;
    char highest_bidder[20]; unsigned int end_time;
    unsigned int duration_ms; unsigned int start_time; int status;
} auction_t;
typedef struct { char creator[20]; unsigned int royalty_pct; } royalty_config_t;
typedef struct {
    unsigned int token_id; unsigned int original_sale_price;
    unsigned int secondary_sale_price; unsigned int royalty_amount;
    char creator[20];
} royalty_info_t;
typedef struct { char key[32]; char value[64]; } attribute_t;
typedef struct {
    char name[MAX_NAME_LEN]; char description[MAX_DESC_LEN];
    char image_uri[MAX_IMAGE_LEN]; attribute_t attributes[MAX_ATTRS];
    int attribute_count; int media_type;
} metadata_t;
typedef struct {
    unsigned int floor_price; unsigned int total_volume;
    unsigned int holder_count; unsigned int token_count;
    unsigned int avg_price;
} collection_stats_t;
typedef struct {
    int type; unsigned int token_id; unsigned int price;
    unsigned int timestamp; char from[20]; char to[20];
} market_event_t;
typedef struct {
    market_event_t events[MAX_EVENTS];
    unsigned int count; unsigned int max_events;
} market_log_t;

/* ── Global state ──────────────────────────────────────────────────────── */
static nft_token_t g_tokens[MAX_TOKENS];
static unsigned int g_token_count = 0;
static nft_collection_t g_collections[MAX_COLLECTIONS];
static unsigned int g_collection_count = 0;
static auction_t g_auctions[MAX_AUCTIONS];
static unsigned int g_auction_count = 0;
static royalty_config_t g_royalties[MAX_COLLECTIONS];
static market_log_t g_market_log;
static unsigned int g_approval_tid[MAX_TOKENS];
static char g_approval_addr[MAX_TOKENS][20];
static unsigned int g_approval_count = 0;

/* ── Forward declarations ──────────────────────────────────────────────── */
static int market_record_event(market_log_t *log, const market_event_t *ev);
static int nft_is_approved(unsigned int token_id, const char *caller);
static int auction_settle(int auc_idx);

/* ── Helpers ───────────────────────────────────────────────────────────── */
static int my_strlen(const char *s) {
    int n = 0; while (s[n]) n++; return n;
}
static void my_strncpy(char *d, const char *s, int n) {
    int i = 0; while (i < n - 1 && s[i]) { d[i] = s[i]; i++; } d[i] = '\0';
}
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
}
static int my_strncmp(const char *a, const char *b, int n) {
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i]) return (unsigned char)a[i] - (unsigned char)b[i];
        if (!a[i]) return 0;
    }
    return 0;
}
static int my_abs(int x) { return x < 0 ? -x : x; }
static void print_str(const char *s) { host_print(s); }
static void print_int(int v) {
    char b[32]; int p = 0;
    if (v < 0) { b[p++] = '-'; v = -v; }
    if (v == 0) b[p++] = '0';
    else {
        int d = 0, t = v;
        while (t > 0) { d++; t /= 10; }
        p += d; b[p] = '\0'; p--;
        while (v > 0) { b[p--] = '0' + (v % 10); v /= 10; }
    }
    host_print(b);
}
static void print_double(int whole, int frac) {
    print_int(whole); print_str(".");
    if (frac < 0) frac = -frac;
    if (frac < 10) print_str("0");
    print_int(frac);
}
static int addr_eq(const char *a, const char *b) {
    for (int i = 0; i < 20; i++) if (a[i] != b[i]) return 0;
    return 1;
}
static void addr_copy(char *d, const char *s) {
    for (int i = 0; i < 20; i++) d[i] = s[i];
}

/* ===== Token operations ================================================ */
static int nft_mint(int col_idx, const char *creator, const char *uri,
                    unsigned int token_id, int standard, unsigned int balance) {
    if (g_token_count >= MAX_TOKENS) return ERR_CAPACITY;
    if (col_idx < 0 || col_idx >= (int)g_collection_count) return ERR_NOT_FOUND;
    nft_collection_t *c = &g_collections[col_idx];
    if (c->token_count >= c->max_supply) return ERR_MAX_SUPPLY;
    if (c->token_count >= MAX_TOKENS_COL) return ERR_CAPACITY;
    nft_token_t *t = &g_tokens[g_token_count];
    t->token_id = token_id;
    addr_copy(t->owner, creator); addr_copy(t->creator, creator);
    my_strncpy(t->uri, uri, MAX_URI_LEN);
    t->standard = standard;
    t->balance = standard == STD_ERC1155 ? (balance ? balance : 1) : 1;
    t->burned = 0;
    c->token_ids[c->token_count++] = token_id;
    g_token_count++;
    market_event_t ev = { EVT_MINT, token_id, 0, 0, {0}, {0} };
    addr_copy(ev.to, creator);
    market_record_event(&g_market_log, &ev);
    return OK;
}
static nft_token_t *nft_find(unsigned int token_id) {
    for (unsigned int i = 0; i < g_token_count; i++)
        if (g_tokens[i].token_id == token_id && !g_tokens[i].burned) return &g_tokens[i];
    return NULL;
}
static int nft_transfer(unsigned int token_id, const char *from, const char *to) {
    nft_token_t *t = nft_find(token_id);
    if (!t) return ERR_NOT_FOUND;
    if (!addr_eq(t->owner, from) && !nft_is_approved(token_id, from)) return ERR_OWNER;
    addr_copy(t->owner, to);
    for (unsigned int i = 0; i < g_approval_count; i++)
        if (g_approval_tid[i] == token_id) { g_approval_addr[i][0] = '\0'; break; }
    return OK;
}
static int nft_burn(unsigned int token_id, const char *owner) {
    nft_token_t *t = nft_find(token_id);
    if (!t) return ERR_NOT_FOUND;
    if (!addr_eq(t->owner, owner)) return ERR_OWNER;
    t->burned = 1; t->balance = 0;
    return OK;
}
static int nft_approve(unsigned int token_id, const char *owner, const char *approved) {
    nft_token_t *t = nft_find(token_id);
    if (!t) return ERR_NOT_FOUND;
    if (!addr_eq(t->owner, owner)) return ERR_OWNER;
    for (unsigned int i = 0; i < g_approval_count; i++)
        if (g_approval_tid[i] == token_id) {
            addr_copy(g_approval_addr[i], approved); return OK;
        }
    if (g_approval_count >= MAX_TOKENS) return ERR_CAPACITY;
    g_approval_tid[g_approval_count] = token_id;
    addr_copy(g_approval_addr[g_approval_count], approved);
    g_approval_count++;
    return OK;
}
static int nft_is_approved(unsigned int token_id, const char *caller) {
    for (unsigned int i = 0; i < g_approval_count; i++)
        if (g_approval_tid[i] == token_id && g_approval_addr[i][0] &&
            addr_eq(g_approval_addr[i], caller)) return 1;
    return 0;
}
static unsigned int nft_balance_of(int col_idx, const char *owner) {
    if (col_idx < 0 || col_idx >= (int)g_collection_count) return 0;
    nft_collection_t *c = &g_collections[col_idx];
    unsigned int count = 0;
    for (unsigned int i = 0; i < c->token_count; i++) {
        nft_token_t *t = nft_find(c->token_ids[i]);
        if (t && addr_eq(t->owner, owner)) count += t->balance;
    }
    return count;
}

/* ===== Auction engine ================================================== */
/* ── Auction helpers ──────────────────────────────────────────────────── */
static int auction_alloc(unsigned int token_id, const char *seller, int type,
                         unsigned int sp, unsigned int rp, unsigned int ep,
                         unsigned int dur) {
    if (g_auction_count >= MAX_AUCTIONS) return ERR_CAPACITY;
    if (!nft_find(token_id)) return ERR_NOT_FOUND;
    auction_t *a = &g_auctions[g_auction_count++];
    a->token_id = token_id; addr_copy(a->seller, seller);
    a->auction_type = type;
    a->start_price = sp; a->reserve_price = rp;
    a->end_price = ep; a->current_bid = 0;
    a->highest_bidder[0] = '\0';
    a->duration_ms = dur; a->start_time = 0;
    a->end_time = dur; a->status = AUCTION_ACTIVE;
    return (int)(g_auction_count - 1);
}
static int auction_create_english(unsigned int tid, const char *seller,
                                  unsigned int sp, unsigned int res, unsigned int dur) {
    return auction_alloc(tid, seller, AUCTION_ENGLISH, sp, res, 0, dur);
}
static int auction_create_dutch(unsigned int tid, const char *seller,
                                unsigned int sp, unsigned int ep, unsigned int dur) {
    return auction_alloc(tid, seller, AUCTION_DUTCH, sp, 0, ep, dur);
}
static int auction_create_buy_now(unsigned int tid, const char *seller, unsigned int price) {
    return auction_alloc(tid, seller, AUCTION_BUY_NOW, price, price, price, 0);
}
static int auction_place_bid(int auc_idx, const char *bidder, unsigned int amount) {
    if (auc_idx < 0 || auc_idx >= (int)g_auction_count) return ERR_NOT_FOUND;
    auction_t *a = &g_auctions[auc_idx];
    if (a->status != AUCTION_ACTIVE || a->auction_type != AUCTION_ENGLISH) return ERR_STATE;
    unsigned int min_bid = a->current_bid == 0 ? a->start_price : a->current_bid + 1;
    if (amount < min_bid) return ERR_BID_LOW;
    a->current_bid = amount; addr_copy(a->highest_bidder, bidder);
    return OK;
}
static unsigned int auction_tick_dutch(int auc_idx, unsigned int elapsed_ms) {
    if (auc_idx < 0 || auc_idx >= (int)g_auction_count) return 0;
    auction_t *a = &g_auctions[auc_idx];
    if (a->auction_type != AUCTION_DUTCH) return 0;
    if (elapsed_ms >= a->duration_ms) return a->end_price;
    unsigned int drop = a->start_price - a->end_price;
    return a->start_price - (drop * elapsed_ms / a->duration_ms);
}
static int auction_buy_now(int auc_idx, const char *buyer) {
    if (auc_idx < 0 || auc_idx >= (int)g_auction_count) return ERR_NOT_FOUND;
    auction_t *a = &g_auctions[auc_idx];
    if (a->status != AUCTION_ACTIVE || a->auction_type != AUCTION_BUY_NOW) return ERR_STATE;
    addr_copy(a->highest_bidder, buyer);
    a->current_bid = a->start_price; a->status = AUCTION_ENDED;
    return auction_settle(auc_idx);
}
static int auction_settle(int auc_idx) {
    if (auc_idx < 0 || auc_idx >= (int)g_auction_count) return ERR_NOT_FOUND;
    auction_t *a = &g_auctions[auc_idx];
    if (a->status != AUCTION_ENDED && a->status != AUCTION_ACTIVE) return ERR_STATE;
    if (a->current_bid == 0 || a->highest_bidder[0] == '\0') return ERR_INVALID;
    if (a->auction_type == AUCTION_ENGLISH && a->current_bid < a->reserve_price)
        return ERR_BID_LOW;
    nft_token_t *t = nft_find(a->token_id);
    if (!t) return ERR_NOT_FOUND;
    int rc = nft_transfer(a->token_id, a->seller, a->highest_bidder);
    if (rc != OK) return rc;
    a->status = AUCTION_ENDED;
    market_event_t ev = { EVT_SALE, a->token_id, a->current_bid, 0, {0}, {0} };
    addr_copy(ev.from, a->seller); addr_copy(ev.to, a->highest_bidder);
    market_record_event(&g_market_log, &ev);
    return OK;
}
static int auction_cancel(int auc_idx, const char *seller) {
    if (auc_idx < 0 || auc_idx >= (int)g_auction_count) return ERR_NOT_FOUND;
    auction_t *a = &g_auctions[auc_idx];
    if (a->status != AUCTION_ACTIVE) return ERR_STATE;
    if (!addr_eq(a->seller, seller)) return ERR_OWNER;
    if (a->current_bid > 0) return ERR_STATE;
    a->status = AUCTION_CANCELLED;
    return OK;
}
static int auction_end_english(int auc_idx) {
    if (auc_idx < 0 || auc_idx >= (int)g_auction_count) return ERR_NOT_FOUND;
    auction_t *a = &g_auctions[auc_idx];
    if (a->status != AUCTION_ACTIVE) return ERR_STATE;
    a->status = AUCTION_ENDED;
    if (a->current_bid > 0) return auction_settle(auc_idx);
    return OK;
}

/* ===== Royalty system ================================================== */
static int royalty_set(int col_idx, unsigned int royalty_pct) {
    if (col_idx < 0 || col_idx >= (int)g_collection_count) return ERR_NOT_FOUND;
    if (royalty_pct > 10000) return ERR_INVALID;
    g_collections[col_idx].royalty_pct = royalty_pct;
    addr_copy(g_royalties[col_idx].creator, g_collections[col_idx].name);
    g_royalties[col_idx].royalty_pct = royalty_pct;
    return OK;
}
static unsigned int royalty_compute(unsigned int original_price,
                                    unsigned int resale_price,
                                    unsigned int royalty_pct) {
    unsigned int sale_price = resale_price > 0 ? resale_price : original_price;
    return (sale_price * royalty_pct) / 10000;
}
static int royalty_distribute(const royalty_info_t *info) {
    if (!info) return ERR_INVALID;
    print_str("  Royalty of "); print_int((int)info->royalty_amount);
    print_str(" paid to creator for token ");
    print_int((int)info->token_id); print_str("\n");
    return OK;
}
static royalty_info_t royalty_query(unsigned int token_id, unsigned int resale_price) {
    royalty_info_t info = {0};
    info.token_id = token_id;
    nft_token_t *t = nft_find(token_id);
    if (!t) return info;
    for (unsigned int i = 0; i < g_collection_count; i++) {
        nft_collection_t *c = &g_collections[i];
        for (unsigned int j = 0; j < c->token_count; j++) {
            if (c->token_ids[j] == token_id) {
                addr_copy(info.creator, t->creator);
                info.secondary_sale_price = resale_price;
                info.royalty_amount = royalty_compute(0, resale_price, c->royalty_pct);
                return info;
            }
        }
    }
    return info;
}

/* ===== Metadata management ============================================= */
static int metadata_detect_media(const char *uri) {
    int len = my_strlen(uri);
    if (len < 4) return MEDIA_UNKNOWN;
    const char *ext = uri + len - 4;
    if (my_strncmp(ext, ".jpg", 4) == 0 || my_strncmp(ext, ".png", 4) == 0 ||
        my_strncmp(ext, "jpeg", 4) == 0 || my_strncmp(ext, ".gif", 4) == 0)
        return MEDIA_IMAGE;
    if (my_strncmp(ext, ".mp4", 4) == 0 || my_strncmp(ext, "webm", 4) == 0)
        return MEDIA_VIDEO;
    if (my_strncmp(ext, ".mp3", 4) == 0 || my_strncmp(ext, "ogg", 4) == 0)
        return MEDIA_AUDIO;
    if (my_strncmp(ext, ".glb", 4) == 0 || my_strncmp(ext, "gltf", 4) == 0)
        return MEDIA_3D;
    return MEDIA_UNKNOWN;
}
static int metadata_validate(const metadata_t *m) {
    if (!m) return ERR_INVALID;
    if (m->name[0] == '\0' || m->image_uri[0] == '\0') return ERR_INVALID;
    return OK;
}
static metadata_t metadata_parse(const char *json, int len) {
    metadata_t m = {0};
    if (!json || len <= 0) return m;
    for (int i = 0; i < len - 10; i++) {
        if (json[i] == '"' && my_strncmp(json + i + 1, "name\"", 5) == 0) {
            int s = i + 6;
            while (s < len && json[s] != ':') s++;
            s++; while (s < len && json[s] != '"') s++; s++;
            int e = s;
            while (e < len && json[e] != '"') e++;
            int n = e - s;
            if (n > MAX_NAME_LEN - 1) n = MAX_NAME_LEN - 1;
            for (int k = 0; k < n; k++) m.name[k] = json[s + k];
            m.name[n] = '\0';
        }
        if (json[i] == '"' && my_strncmp(json + i + 1, "image\"", 6) == 0) {
            int s = i + 7;
            while (s < len && json[s] != ':') s++;
            s++; while (s < len && json[s] != '"') s++; s++;
            int e = s;
            while (e < len && json[e] != '"') e++;
            int n = e - s;
            if (n > MAX_IMAGE_LEN - 1) n = MAX_IMAGE_LEN - 1;
            for (int k = 0; k < n; k++) m.image_uri[k] = json[s + k];
            m.image_uri[n] = '\0';
            m.media_type = metadata_detect_media(m.image_uri);
        }
    }
    return m;
}
static void metadata_ipfs_link(const char *cid, char *out, int max_len) {
    const char *prefix = "ipfs://";
    int plen = 7;
    int clen = my_strlen(cid);
    int n = plen + clen;
    if (n >= max_len) n = max_len - 1;
    my_strncpy(out, prefix, max_len);
    int off = plen;
    for (int i = 0; i < clen && off < max_len - 1; i++, off++)
        out[off] = cid[i];
    out[off] = '\0';
}
static int metadata_serialize(const metadata_t *m, char *buf, int max_len) {
    if (!m || !buf || max_len < 16) return 0;
    int pos = 0;
    const char *h = "{\"name\":\"";
    int hl = my_strlen(h);
    for (int i = 0; i < hl && pos < max_len - 1; i++) buf[pos++] = h[i];
    int nl = my_strlen(m->name);
    for (int i = 0; i < nl && pos < max_len - 1; i++) buf[pos++] = m->name[i];
    const char *m1 = "\",\"image\":\"";
    hl = my_strlen(m1);
    for (int i = 0; i < hl && pos < max_len - 1; i++) buf[pos++] = m1[i];
    int il = my_strlen(m->image_uri);
    for (int i = 0; i < il && pos < max_len - 1; i++) buf[pos++] = m->image_uri[i];
    const char *t = "\",\"attributes\":[]}";
    hl = my_strlen(t);
    for (int i = 0; i < hl && pos < max_len - 1; i++) buf[pos++] = t[i];
    buf[pos] = '\0';
    return pos;
}

/* ===== Collection management =========================================== */
static int collection_create(const char *name, const char *symbol,
                             unsigned int max_supply) {
    if (g_collection_count >= MAX_COLLECTIONS) return ERR_CAPACITY;
    nft_collection_t *c = &g_collections[g_collection_count++];
    my_strncpy(c->name, name, MAX_NAME_LEN);
    my_strncpy(c->symbol, symbol, MAX_SYMBOL_LEN);
    c->token_count = 0; c->max_supply = max_supply;
    c->royalty_pct = 0; c->active = 1;
    return (int)(g_collection_count - 1);
}
static collection_stats_t collection_compute_stats(int col_idx) {
    collection_stats_t s = {0};
    if (col_idx < 0 || col_idx >= (int)g_collection_count) return s;
    nft_collection_t *c = &g_collections[col_idx];
    s.token_count = c->token_count;
    char seen[MAX_HOLDERS][20];
    unsigned int seen_count = 0;
    for (unsigned int i = 0; i < c->token_count; i++) {
        nft_token_t *t = nft_find(c->token_ids[i]);
        if (!t) continue;
        int found = 0;
        for (unsigned int j = 0; j < seen_count; j++)
            if (addr_eq(seen[j], t->owner)) { found = 1; break; }
        if (!found && seen_count < MAX_HOLDERS)
            addr_copy(seen[seen_count++], t->owner);
    }
    s.holder_count = seen_count;
    unsigned int floor = 0xFFFFFFFF;
    unsigned int total = 0;
    unsigned int sales = 0;
    for (unsigned int i = 0; i < g_market_log.count; i++) {
        market_event_t *e = &g_market_log.events[i];
        if (e->type != EVT_SALE) continue;
        int in_col = 0;
        for (unsigned int j = 0; j < c->token_count; j++)
            if (c->token_ids[j] == e->token_id) { in_col = 1; break; }
        if (!in_col) continue;
        total += e->price; sales++;
        if (e->price < floor) floor = e->price;
    }
    s.total_volume = total;
    s.avg_price = sales > 0 ? total / sales : 0;
    s.floor_price = floor == 0xFFFFFFFF ? 0 : floor;
    return s;
}
static unsigned int collection_compute_rarity(unsigned int token_id, int col_idx) {
    if (col_idx < 0 || col_idx >= (int)g_collection_count) return 0;
    nft_collection_t *c = &g_collections[col_idx];
    if (c->token_count == 0) return 0;
    for (unsigned int i = 0; i < c->token_count; i++)
        if (c->token_ids[i] == token_id)
            return ((c->token_count - i) * 1000) / c->token_count;
    return 0;
}
static unsigned int collection_get_holders(int col_idx, char out[][20],
                                           unsigned int max_out) {
    if (col_idx < 0 || col_idx >= (int)g_collection_count) return 0;
    nft_collection_t *c = &g_collections[col_idx];
    unsigned int count = 0;
    for (unsigned int i = 0; i < c->token_count && count < max_out; i++) {
        nft_token_t *t = nft_find(c->token_ids[i]);
        if (!t) continue;
        int found = 0;
        for (unsigned int j = 0; j < count; j++)
            if (addr_eq(out[j], t->owner)) { found = 1; break; }
        if (!found) addr_copy(out[count++], t->owner);
    }
    return count;
}

/* ===== Market statistics =============================================== */
static void market_log_init(market_log_t *log) {
    log->count = 0; log->max_events = MAX_EVENTS;
}
static int market_record_event(market_log_t *log, const market_event_t *ev) {
    if (!log || !ev) return ERR_INVALID;
    if (log->count >= log->max_events) return ERR_CAPACITY;
    log->events[log->count++] = *ev;
    return OK;
}
static unsigned int market_floor_price(int col_idx) {
    return collection_compute_stats(col_idx).floor_price;
}
static unsigned int market_volume(int col_idx) {
    return collection_compute_stats(col_idx).total_volume;
}
static unsigned int market_price_history(unsigned int token_id,
                                         unsigned int *out, unsigned int max_out) {
    unsigned int count = 0;
    for (unsigned int i = 0; i < g_market_log.count && count < max_out; i++)
        if (g_market_log.events[i].token_id == token_id &&
            g_market_log.events[i].type == EVT_SALE)
            out[count++] = g_market_log.events[i].price;
    return count;
}
static int market_trending(int *out_col, unsigned int *out_vol, unsigned int max_out) {
    unsigned int vol[MAX_COLLECTIONS] = {0};
    for (unsigned int i = 0; i < g_market_log.count; i++) {
        market_event_t *e = &g_market_log.events[i];
        if (e->type != EVT_SALE) continue;
        for (unsigned int c = 0; c < g_collection_count; c++)
            for (unsigned int j = 0; j < g_collections[c].token_count; j++)
                if (g_collections[c].token_ids[j] == e->token_id) {
                    vol[c] += e->price; break;
                }
    }
    int count = 0;
    for (unsigned int k = 0; k < g_collection_count && count < (int)max_out; k++) {
        int best = -1;
        unsigned int best_v = 0;
        for (unsigned int c = 0; c < g_collection_count; c++) {
            int used = 0;
            for (int p = 0; p < count; p++)
                if (out_col[p] == (int)c) { used = 1; break; }
            if (!used && vol[c] > best_v) {
                best_v = vol[c]; best = (int)c;
            }
        }
        if (best >= 0) {
            out_col[count] = best;
            out_vol[count] = best_v; count++;
        }
    }
    return count;
}

/* ===== _start entry point ============================================== */
static void run_tests(void) {
    print_str("=== NFT Marketplace v57.0 - Test Suite ===\n");
    int art = collection_create("Digital Art", "DART", 1000);
    int music = collection_create("Music NFTs", "MUSIC", 500);
    print_str("[1] Collections: art="); print_int(art);
    print_str(" music="); print_int(music); print_str("\n");
    char alice[20] = "alice"; char bob[20] = "bob__"; char carol[20] = "carol";
    nft_mint(art, alice, "ipfs://QmArt1.png", 1, STD_ERC721, 0);
    nft_mint(art, alice, "ipfs://QmArt2.jpg", 2, STD_ERC721, 0);
    nft_mint(art, bob, "ipfs://QmArt3.png", 3, STD_ERC721, 0);
    nft_mint(music, carol, "ipfs://QmSong.mp3", 10, STD_ERC1155, 100);
    print_str("[2] Minted 3 art + 1 music(erc1155 x100)\n");
    print_str("  Alice bal="); print_int((int)nft_balance_of(art, alice));
    print_str(" Bob bal="); print_int((int)nft_balance_of(art, bob)); print_str("\n");
    nft_approve(1, alice, bob);
    print_str("[3] Approve token 1: "); print_int(nft_is_approved(1, bob));
    nft_transfer(1, alice, carol);
    print_str(" Transferred. Alice bal=");
    print_int((int)nft_balance_of(art, alice)); print_str("\n");
    royalty_set(art, 250);
    print_str("[4] Royalty 2.5% on 50000: ");
    print_int((int)royalty_compute(10000, 50000, 250)); print_str("\n");
    royalty_info_t ri = royalty_query(1, 50000);
    print_str("  Query token 1 @ 50000: "); print_int((int)ri.royalty_amount);
    print_str("\n"); royalty_distribute(&ri);
    const char *json = "{\"name\":\"Cosmic Art\",\"image\":\"ipfs://QmCosmic.png\"}";
    metadata_t md = metadata_parse(json, my_strlen(json));
    print_str("[5] Metadata: "); print_str(md.name);
    print_str(" media="); print_int(md.media_type);
    print_str(" valid="); print_int(metadata_validate(&md)); print_str("\n");
    char ipfs[64]; metadata_ipfs_link("QmAbc123", ipfs, 64);
    print_str("  IPFS: "); print_str(ipfs); print_str("\n");
    char ser[256]; int sl = metadata_serialize(&md, ser, 256);
    print_str("  Ser("); print_int(sl); print_str("B): "); print_str(ser);
    print_str("\n");
    int ea = auction_create_english(2, alice, 1000, 5000, 60000);
    print_str("[6] English idx="); print_int(ea); print_str("\n");
    auction_place_bid(ea, bob, 2000);
    auction_place_bid(ea, carol, 3000);
    print_str("  Low bid rejected: ");
    print_int(auction_place_bid(ea, bob, 2500)); print_str("\n");
    auction_end_english(ea);
    print_str("  Settled. Carol bal=");
    print_int((int)nft_balance_of(art, carol)); print_str("\n");
    int da = auction_create_dutch(3, bob, 10000, 1000, 10000);
    print_str("[7] Dutch idx="); print_int(da);
    print_str(" @0="); print_int((int)auction_tick_dutch(da, 0));
    print_str(" @5000="); print_int((int)auction_tick_dutch(da, 5000));
    print_str(" @10000="); print_int((int)auction_tick_dutch(da, 10000));
    print_str("\n");
    int bn = auction_create_buy_now(10, carol, 7500);
    print_str("[8] Buy-now idx="); print_int(bn); print_str("\n");
    auction_buy_now(bn, alice);
    print_str("  Alice bought token 10\n");
    nft_mint(art, alice, "ipfs://QmX.png", 4, STD_ERC721, 0);
    int cx = auction_create_english(4, alice, 500, 0, 30000);
    print_str("[9] Cancel token 4: ");
    print_int(auction_cancel(cx, alice)); print_str("\n");
    nft_burn(4, alice);
    print_str("[10] Burn. Bob bal=");
    print_int((int)nft_balance_of(art, bob)); print_str("\n");
    collection_stats_t cs = collection_compute_stats(art);
    print_str("[11] Stats: tokens="); print_int((int)cs.token_count);
    print_str(" holders="); print_int((int)cs.holder_count);
    print_str(" vol="); print_int((int)cs.total_volume);
    print_str(" floor="); print_int((int)cs.floor_price); print_str("\n");
    print_str("  Rarity token 1: ");
    print_int((int)collection_compute_rarity(1, art)); print_str("\n");
    char holders[4][20];
    print_str("  Holders: "); print_int((int)collection_get_holders(art, holders, 4));
    print_str("\n");
    print_str("[12] Market: floor=");
    print_int((int)market_floor_price(art));
    print_str(" vol="); print_int((int)market_volume(art)); print_str("\n");
    unsigned int hist[8];
    print_str("  Token 2 history: ");
    print_int((int)market_price_history(2, hist, 8)); print_str(" sales\n");
    int tcol[4]; unsigned int tvol[4];
    int tc = market_trending(tcol, tvol, 4);
    print_str("  Trending: "); print_int(tc);
    for (int i = 0; i < tc; i++) {
        print_str(" #"); print_int(tcol[i]);
        print_str("="); print_int((int)tvol[i]);
    }
    print_str("\n=== NFT Marketplace v57.0 - Test Complete ===\n");
}
static void print_help(void) {
    print_str("NFT Marketplace v57.0 - Digital Asset Trading Platform\n\n");
    print_str("Usage: nft_marketplace [options]\n\n");
    print_str("Options:\n");
    print_str("  -h    Show this help message\n");
    print_str("  -t    Run comprehensive test suite\n\n");
    print_str("Features:\n");
    print_str("  - ERC721/ERC1155 token minting, transfer, approval, burn\n");
    print_str("  - English, Dutch, and buy-now auctions\n");
    print_str("  - Royalty distribution (basis-point configurable)\n");
    print_str("  - JSON metadata parsing and IPFS link generation\n");
    print_str("  - Collection stats, rarity scoring, holder enumeration\n");
    print_str("  - Market event logging, floor price, volume, trending\n");
}
void _start(void) {
    char buf[64];
    int n = host_get_argv(0, 64);
    if (n > 0) {
        host_get_argv((unsigned int)(long)buf, 64);
        if (my_strcmp(buf, "-h") == 0) { print_help(); return; }
        if (my_strcmp(buf, "-t") == 0) { run_tests(); return; }
    }
    print_str("NFT Marketplace v57.0 - Digital Asset Trading Platform\n");
    print_str("Use -h for help, -t for test\n");
}
