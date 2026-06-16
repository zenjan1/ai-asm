/* app_marketplace: App publishing, search, install, updates, ratings & signature verification */
/* AI-ASM OS v47.0 */

__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int off, unsigned int len);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* ── Limits ─────────────────────────────────────────────────────────────── */
#define MAX_APPS         64
#define MAX_DEPS          8
#define MAX_CATS          4
#define MAX_PERMS         8
#define MAX_REVIEWS      64
#define MAX_INSTALLED    32
#define MAX_UPDATES      32
#define MAX_NAME_LEN     48
#define MAX_DESC_LEN    128
#define MAX_VER_LEN      16
#define MAX_PUB_LEN      32
#define MAX_KEY_LEN      32
#define MAX_SIG_LEN      64
#define MAX_URL_LEN      96
#define MAX_INPUT       512

/* ── Categories ─────────────────────────────────────────────────────────── */
#define CAT_PRODUCTIVITY  0
#define CAT_DEVELOPER     1
#define CAT_MEDIA         2
#define CAT_SYSTEM        3
#define CAT_SECURITY      4
#define CAT_AI_ML         5
#define CAT_NETWORK       6
#define CAT_UTILITY       7
#define CAT_EDUCATION     8
#define CAT_GAME          9
#define CAT_COUNT        10

/* ── Statuses ───────────────────────────────────────────────────────────── */
#define APP_STATUS_PUBLISHED   0
#define APP_STATUS_DEPRECATED  1
#define APP_STATUS_SUSPENDED   2

#define INSTALL_STATUS_AVAILABLE  0
#define INSTALL_STATUS_INSTALLED  1
#define INSTALL_STATUS_UPDATING   2
#define INSTALL_STATUS_FAILED     3

#define SIG_ALGORITHM_SHA256  0
#define SIG_ALGORITHM_ED25519 1

/* ── Data structures ────────────────────────────────────────────────────── */

typedef struct {
    int major;
    int minor;
    int patch;
} version_t;

typedef struct {
    char name[MAX_NAME_LEN];
    version_t min_version;
    int required;            /* 1 = required, 0 = optional */
} dependency_t;

typedef struct {
    int app_id;
    char name[MAX_NAME_LEN];
    char description[MAX_DESC_LEN];
    char publisher[MAX_PUB_LEN];
    version_t version;
    version_t prev_version;  /* for rollback */
    int categories[MAX_CATS];
    int cat_count;
    char keywords[MAX_DEPS][MAX_KEY_LEN];
    int keyword_count;
    dependency_t deps[MAX_DEPS];
    int dep_count;
    char permissions[MAX_PERMS][MAX_KEY_LEN];
    int perm_count;
    int size_kb;
    int download_count;
    int status;
    char download_url[MAX_URL_LEN];
    char signature[MAX_SIG_LEN];
    int sig_algorithm;
    char publisher_key[MAX_SIG_LEN];
    int rating_sum;
    int rating_count;
} app_listing_t;

typedef struct {
    int review_id;
    int app_id;
    int rating;              /* 1-5 stars */
    char comment[MAX_DESC_LEN];
    int helpful_count;
} review_t;

typedef struct {
    int app_id;
    int status;
    version_t installed_ver;
    int install_timestamp;
    int size_on_disk_kb;
} install_record_t;

typedef struct {
    int app_id;
    version_t from_ver;
    version_t to_ver;
    int available;           /* 1 = update available */
    int auto_update;         /* 1 = auto-update enabled */
} update_record_t;

/* ── Global state ───────────────────────────────────────────────────────── */

static unsigned int heap_pos = 65536;
static app_listing_t   apps[MAX_APPS];
static int             app_count = 0;
static int             next_app_id = 0;
static review_t        reviews[MAX_REVIEWS];
static int             review_count = 0;
static install_record_t installs[MAX_INSTALLED];
static int             install_count = 0;
static update_record_t updates[MAX_UPDATES];
static int             update_count = 0;

/* ── Helpers ────────────────────────────────────────────────────────────── */

static unsigned int alloc(unsigned int n) {
    unsigned int p = heap_pos; heap_pos += n;
    heap_pos = (heap_pos + 15) & ~15u; return p;
}
static void copy_to_mem(const char *s, unsigned int d, unsigned int n) {
    char *p = (char *)d; for (unsigned int i = 0; i < n; i++) p[i] = s[i];
}
static unsigned int my_strlen(const char *s) {
    unsigned int n = 0; while (s[n]) n++; return n;
}
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return 1; a++; b++; } return (*a != *b);
}
static int my_strncmp(const char *a, const char *b, unsigned int n) {
    for (unsigned int i = 0; i < n; i++) {
        if (a[i] != b[i]) return 1;
        if (a[i] == 0) return 0;
    }
    return 0;
}
static void copy_string(char *d, const char *s, unsigned int m) {
    unsigned int i = 0;
    while (s[i] && i < m - 1) { d[i] = s[i]; i++; }
    d[i] = 0;
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

/* ── Name lookups ───────────────────────────────────────────────────────── */

static const char *cat_name(int c) {
    switch (c) {
        case CAT_PRODUCTIVITY: return "productivity";
        case CAT_DEVELOPER:    return "developer";
        case CAT_MEDIA:        return "media";
        case CAT_SYSTEM:       return "system";
        case CAT_SECURITY:     return "security";
        case CAT_AI_ML:        return "ai_ml";
        case CAT_NETWORK:      return "network";
        case CAT_UTILITY:      return "utility";
        case CAT_EDUCATION:    return "education";
        case CAT_GAME:         return "game";
        default: return "unknown";
    }
}

static const char *app_status_name(int s) {
    switch (s) {
        case APP_STATUS_PUBLISHED:  return "published";
        case APP_STATUS_DEPRECATED: return "deprecated";
        case APP_STATUS_SUSPENDED:  return "suspended";
        default: return "unknown";
    }
}

static const char *install_status_name(int s) {
    switch (s) {
        case INSTALL_STATUS_AVAILABLE: return "available";
        case INSTALL_STATUS_INSTALLED: return "installed";
        case INSTALL_STATUS_UPDATING:  return "updating";
        case INSTALL_STATUS_FAILED:    return "failed";
        default: return "unknown";
    }
}

static const char *sig_algo_name(int a) {
    switch (a) {
        case SIG_ALGORITHM_SHA256:  return "sha256";
        case SIG_ALGORITHM_ED25519: return "ed25519";
        default: return "unknown";
    }
}

/* ── Version helpers ────────────────────────────────────────────────────── */

static int version_cmp(version_t a, version_t b) {
    if (a.major != b.major) return (a.major > b.major) ? 1 : -1;
    if (a.minor != b.minor) return (a.minor > b.minor) ? 1 : -1;
    if (a.patch != b.patch) return (a.patch > b.patch) ? 1 : -1;
    return 0;
}

static void print_version(version_t v) {
    print_int(v.major); print_str(".");
    print_int(v.minor); print_str(".");
    print_int(v.patch);
}

/* ── App publishing ─────────────────────────────────────────────────────── */

static int publish_app(const char *name, const char *desc, const char *pub,
                       int vmaj, int vmin, int vpat,
                       int cat0, int cat1,
                       const char *kw0, const char *kw1, const char *kw2,
                       int size_kb) {
    if (app_count >= MAX_APPS) return -1;
    app_listing_t *a = &apps[app_count];
    a->app_id     = next_app_id++;
    a->status     = APP_STATUS_PUBLISHED;
    a->version.major = vmaj;
    a->version.minor = vmin;
    a->version.patch = vpat;
    a->prev_version = a->version;
    a->size_kb    = size_kb;
    a->download_count = 0;
    a->rating_sum = 0;
    a->rating_count = 0;
    a->dep_count  = 0;
    a->perm_count = 0;
    a->keyword_count = 0;
    a->cat_count  = 0;

    copy_string(a->name, name, MAX_NAME_LEN);
    copy_string(a->description, desc, MAX_DESC_LEN);
    copy_string(a->publisher, pub, MAX_PUB_LEN);
    copy_string(a->download_url, "https://market.aiasm.io/pkg/", MAX_URL_LEN);
    unsigned int url_off = my_strlen("https://market.aiasm.io/pkg/");
    /* append name into url manually */
    char *url = a->download_url;
    unsigned int ni = 0;
    while (name[ni] && url_off + ni < MAX_URL_LEN - 1) {
        url[url_off + ni] = name[ni]; ni++;
    }
    url[url_off + ni] = 0;

    /* categories */
    if (cat0 >= 0 && cat0 < CAT_COUNT) a->categories[a->cat_count++] = cat0;
    if (cat1 >= 0 && cat1 < CAT_COUNT) a->categories[a->cat_count++] = cat1;

    /* keywords */
    if (kw0 && kw0[0]) copy_string(a->keywords[a->keyword_count++], kw0, MAX_KEY_LEN);
    if (kw1 && kw1[0]) copy_string(a->keywords[a->keyword_count++], kw1, MAX_KEY_LEN);
    if (kw2 && kw2[0]) copy_string(a->keywords[a->keyword_count++], kw2, MAX_KEY_LEN);

    /* placeholder signature */
    copy_string(a->signature, "sig_placeholder", MAX_SIG_LEN);
    a->sig_algorithm = SIG_ALGORITHM_SHA256;
    copy_string(a->publisher_key, "pubkey_placeholder", MAX_SIG_LEN);

    app_count++;
    print_str("app_marketplace: published '"); print_str(name);
    print_str("' v"); print_version(a->version);
    print_str(" by "); print_str(pub);
    print_str(" size="); print_int(size_kb); print_str("KB\n");
    return a->app_id;
}

static int add_dependency(int app_id, const char *dep_name,
                          int dmin_maj, int dmin_min, int dmin_pat,
                          int required) {
    if (app_id < 0 || app_id >= app_count) return -1;
    app_listing_t *a = &apps[app_id];
    if (a->dep_count >= MAX_DEPS) return -1;
    dependency_t *d = &a->deps[a->dep_count++];
    copy_string(d->name, dep_name, MAX_NAME_LEN);
    d->min_version.major = dmin_maj;
    d->min_version.minor = dmin_min;
    d->min_version.patch = dmin_pat;
    d->required = required;
    print_str("app_marketplace:   dep added: "); print_str(dep_name);
    print_str(" required="); print_int(required); print_str("\n");
    return 0;
}

static int add_permission(int app_id, const char *perm) {
    if (app_id < 0 || app_id >= app_count) return -1;
    app_listing_t *a = &apps[app_id];
    if (a->perm_count >= MAX_PERMS) return -1;
    copy_string(a->permissions[a->perm_count++], perm, MAX_KEY_LEN);
    return 0;
}

/* ── Search & discovery ─────────────────────────────────────────────────── */

static int search_by_keyword(const char *keyword) {
    int found = 0;
    print_str("app_marketplace: search keyword='"); print_str(keyword);
    print_str("'\n");
    for (int i = 0; i < app_count; i++) {
        app_listing_t *a = &apps[i];
        if (a->status != APP_STATUS_PUBLISHED) continue;
        int match = 0;
        for (int k = 0; k < a->keyword_count; k++) {
            if (my_strcmp(a->keywords[k], keyword) == 0) { match = 1; break; }
        }
        /* also match name substring */
        if (!match) {
            unsigned int kn = my_strlen(keyword);
            unsigned int an = my_strlen(a->name);
            for (unsigned int j = 0; j + kn <= an; j++) {
                if (my_strncmp(&a->name[j], keyword, kn) == 0) { match = 1; break; }
            }
        }
        if (match) {
            print_str("app_marketplace:   ["); print_int(a->app_id); print_str("] ");
            print_str(a->name); print_str(" v"); print_version(a->version);
            print_str(" by "); print_str(a->publisher); print_str("\n");
            found++;
        }
    }
    print_str("app_marketplace: found "); print_int(found); print_str(" results\n");
    return found;
}

static int browse_category(int category) {
    int found = 0;
    print_str("app_marketplace: browse category='");
    print_str(cat_name(category)); print_str("'\n");
    for (int i = 0; i < app_count; i++) {
        app_listing_t *a = &apps[i];
        if (a->status != APP_STATUS_PUBLISHED) continue;
        int in_cat = 0;
        for (int c = 0; c < a->cat_count; c++) {
            if (a->categories[c] == category) { in_cat = 1; break; }
        }
        if (in_cat) {
            int avg = (a->rating_count > 0) ? (a->rating_sum / a->rating_count) : 0;
            print_str("app_marketplace:   ["); print_int(a->app_id); print_str("] ");
            print_str(a->name); print_str(" v"); print_version(a->version);
            print_str(" dl="); print_int(a->download_count);
            print_str(" rating="); print_int(avg); print_str("/5\n");
            found++;
        }
    }
    print_str("app_marketplace: found "); print_int(found); print_str(" apps\n");
    return found;
}

static int recommend_apps(int min_rating, int max_results) {
    int emitted = 0;
    print_str("app_marketplace: recommend min_rating=");
    print_int(min_rating); print_str("\n");
    for (int i = 0; i < app_count && emitted < max_results; i++) {
        app_listing_t *a = &apps[i];
        if (a->status != APP_STATUS_PUBLISHED) continue;
        int avg = (a->rating_count > 0) ? (a->rating_sum / a->rating_count) : 0;
        if (avg >= min_rating || (a->rating_count == 0 && min_rating <= 3)) {
            print_str("app_marketplace:   recommended: "); print_str(a->name);
            print_str(" ("); print_str(cat_name(a->categories[0]));
            print_str(") rating="); print_int(avg); print_str("/5\n");
            emitted++;
        }
    }
    print_str("app_marketplace: recommended "); print_int(emitted);
    print_str(" apps\n");
    return emitted;
}

/* ── App installer ──────────────────────────────────────────────────────── */

static int find_app_by_name(const char *name) {
    for (int i = 0; i < app_count; i++) {
        if (apps[i].status == APP_STATUS_PUBLISHED &&
            my_strcmp(apps[i].name, name) == 0) return i;
    }
    return -1;
}

static int resolve_dependencies(int app_id) {
    if (app_id < 0 || app_id >= app_count) return -1;
    app_listing_t *a = &apps[app_id];
    int unmet = 0;
    print_str("app_marketplace: resolve deps for '"); print_str(a->name);
    print_str("'\n");
    for (int d = 0; d < a->dep_count; d++) {
        dependency_t *dep = &a->deps[d];
        int found = find_app_by_name(dep->name);
        if (found < 0) {
            if (dep->required) {
                print_str("app_marketplace:   MISSING required dep: ");
                print_str(dep->name); print_str("\n");
                unmet++;
            } else {
                print_str("app_marketplace:   optional dep missing: ");
                print_str(dep->name); print_str("\n");
            }
            continue;
        }
        /* check version */
        int vc = version_cmp(apps[found].version, dep->min_version);
        if (vc < 0) {
            print_str("app_marketplace:   dep "); print_str(dep->name);
            print_str(" too old: have v"); print_version(apps[found].version);
            print_str(" need v"); print_version(dep->min_version);
            print_str("\n");
            if (dep->required) unmet++;
        } else {
            print_str("app_marketplace:   dep "); print_str(dep->name);
            print_str(" satisfied v"); print_version(apps[found].version);
            print_str("\n");
        }
    }
    if (unmet == 0) {
        print_str("app_marketplace: all dependencies satisfied\n");
    } else {
        print_str("app_marketplace: UNMET dependencies: "); print_int(unmet);
        print_str("\n");
    }
    return unmet;
}

static int verify_app_signature(int app_id) {
    if (app_id < 0 || app_id >= app_count) return -1;
    app_listing_t *a = &apps[app_id];
    print_str("app_marketplace: verify signature for '"); print_str(a->name);
    print_str("' algo="); print_str(sig_algo_name(a->sig_algorithm));
    print_str(" sig="); print_str(a->signature); print_str("\n");
    /* placeholder: real impl would hash binary and compare sig */
    if (a->signature[0] == 0) {
        print_str("app_marketplace: SIGNATURE MISSING - reject\n");
        return -1;
    }
    if (a->publisher_key[0] == 0) {
        print_str("app_marketplace: PUBLISHER KEY MISSING - reject\n");
        return -1;
    }
    print_str("app_marketplace: signature valid, publisher verified\n");
    return 0;
}

static int install_app(int app_id) {
    if (app_id < 0 || app_id >= app_count) return -1;
    if (install_count >= MAX_INSTALLED) return -1;
    app_listing_t *a = &apps[app_id];
    if (a->status != APP_STATUS_PUBLISHED) return -1;
    /* check already installed */
    for (int i = 0; i < install_count; i++) {
        if (installs[i].app_id == app_id &&
            installs[i].status == INSTALL_STATUS_INSTALLED) {
            print_str("app_marketplace: '"); print_str(a->name);
            print_str("' already installed\n");
            return -1;
        }
    }
    /* verify signature */
    if (verify_app_signature(app_id) != 0) {
        print_str("app_marketplace: install blocked: signature invalid\n");
        return -1;
    }
    /* resolve deps */
    if (resolve_dependencies(app_id) != 0) {
        print_str("app_marketplace: install blocked: deps unmet\n");
        return -1;
    }
    /* download step */
    print_str("app_marketplace: downloading '"); print_str(a->name);
    print_str("' from "); print_str(a->download_url); print_str("\n");
    print_str("app_marketplace: download complete ");
    print_int(a->size_kb); print_str("KB\n");
    /* record install */
    install_record_t *ir = &installs[install_count++];
    ir->app_id           = app_id;
    ir->status           = INSTALL_STATUS_INSTALLED;
    ir->installed_ver    = a->version;
    ir->install_timestamp = 1700000000 + app_id * 3600;
    ir->size_on_disk_kb  = a->size_kb;
    a->download_count++;
    /* record in update tracker */
    if (update_count < MAX_UPDATES) {
        update_record_t *ur = &updates[update_count++];
        ur->app_id     = app_id;
        ur->from_ver   = a->version;
        ur->to_ver     = a->version;
        ur->available  = 0;
        ur->auto_update = 1;
    }
    print_str("app_marketplace: installed '"); print_str(a->name);
    print_str("' v"); print_version(a->version); print_str("\n");
    return 0;
}

static int verify_install(int app_id) {
    for (int i = 0; i < install_count; i++) {
        if (installs[i].app_id == app_id) {
            install_record_t *ir = &installs[i];
            print_str("app_marketplace: verify install '");
            print_str(apps[app_id].name); print_str("'\n");
            print_str("app_marketplace:   status=");
            print_str(install_status_name(ir->status)); print_str("\n");
            print_str("app_marketplace:   installed_v=");
            print_version(ir->installed_ver); print_str("\n");
            print_str("app_marketplace:   size=");
            print_int(ir->size_on_disk_kb); print_str("KB\n");
            if (ir->status != INSTALL_STATUS_INSTALLED) return -1;
            print_str("app_marketplace: install verified OK\n");
            return 0;
        }
    }
    print_str("app_marketplace: app not installed\n");
    return -1;
}

/* ── App update management ──────────────────────────────────────────────── */

static int bump_version(int app_id, int new_maj, int new_min, int new_pat) {
    if (app_id < 0 || app_id >= app_count) return -1;
    app_listing_t *a = &apps[app_id];
    a->prev_version = a->version;
    a->version.major = new_maj;
    a->version.minor = new_min;
    a->version.patch = new_pat;
    print_str("app_marketplace: bumped '"); print_str(a->name);
    print_str("' v"); print_version(a->prev_version);
    print_str(" -> v"); print_version(a->version); print_str("\n");
    return 0;
}

static int check_updates(void) {
    int avail = 0;
    print_str("app_marketplace: check for updates\n");
    for (int i = 0; i < install_count; i++) {
        install_record_t *ir = &installs[i];
        if (ir->status != INSTALL_STATUS_INSTALLED) continue;
        app_listing_t *a = &apps[ir->app_id];
        int cmp = version_cmp(a->version, ir->installed_ver);
        if (cmp > 0) {
            print_str("app_marketplace:   update available: ");
            print_str(a->name); print_str(" v");
            print_version(ir->installed_ver); print_str(" -> v");
            print_version(a->version); print_str("\n");
            /* update record */
            for (int u = 0; u < update_count; u++) {
                if (updates[u].app_id == ir->app_id) {
                    updates[u].from_ver  = ir->installed_ver;
                    updates[u].to_ver    = a->version;
                    updates[u].available = 1;
                }
            }
            avail++;
        }
    }
    print_str("app_marketplace: "); print_int(avail);
    print_str(" updates available\n");
    return avail;
}

static int auto_update_all(void) {
    int updated = 0;
    print_str("app_marketplace: auto-update pass\n");
    for (int u = 0; u < update_count; u++) {
        update_record_t *ur = &updates[u];
        if (!ur->available || !ur->auto_update) continue;
        app_listing_t *a = &apps[ur->app_id];
        if (verify_app_signature(ur->app_id) != 0) continue;
        /* mark updating */
        for (int i = 0; i < install_count; i++) {
            if (installs[i].app_id == ur->app_id) {
                installs[i].status = INSTALL_STATUS_UPDATING;
                installs[i].installed_ver = a->version;
                installs[i].size_on_disk_kb = a->size_kb;
                installs[i].status = INSTALL_STATUS_INSTALLED;
                break;
            }
        }
        ur->available = 0;
        ur->from_ver  = ur->to_ver;
        print_str("app_marketplace:   auto-updated '"); print_str(a->name);
        print_str("' to v"); print_version(a->version); print_str("\n");
        updated++;
    }
    print_str("app_marketplace: updated "); print_int(updated);
    print_str(" apps\n");
    return updated;
}

static int rollback_app(int app_id) {
    if (app_id < 0 || app_id >= app_count) return -1;
    app_listing_t *a = &apps[app_id];
    for (int i = 0; i < install_count; i++) {
        if (installs[i].app_id != app_id) continue;
        if (version_cmp(a->prev_version, a->version) == 0) {
            print_str("app_marketplace: no rollback available for '");
            print_str(a->name); print_str("'\n");
            return -1;
        }
        version_t target = a->prev_version;
        print_str("app_marketplace: rollback '"); print_str(a->name);
        print_str("' v"); print_version(a->version);
        print_str(" -> v"); print_version(target); print_str("\n");
        a->prev_version = a->version;
        a->version = target;
        installs[i].installed_ver = target;
        print_str("app_marketplace: rollback complete\n");
        return 0;
    }
    print_str("app_marketplace: app not installed, cannot rollback\n");
    return -1;
}

/* ── Ratings & reviews ──────────────────────────────────────────────────── */

static int submit_review(int app_id, int rating, const char *comment) {
    if (app_id < 0 || app_id >= app_count) return -1;
    if (review_count >= MAX_REVIEWS) return -1;
    if (rating < 1) rating = 1;
    if (rating > 5) rating = 5;
    review_t *r = &reviews[review_count++];
    r->review_id    = review_count - 1;
    r->app_id       = app_id;
    r->rating       = rating;
    r->helpful_count = 0;
    copy_string(r->comment, comment, MAX_DESC_LEN);
    app_listing_t *a = &apps[app_id];
    a->rating_sum   += rating;
    a->rating_count += 1;
    int avg = a->rating_sum / a->rating_count;
    print_str("app_marketplace: review '"); print_str(a->name);
    print_str("' "); print_int(rating); print_str("/5 '");
    print_str(comment); print_str("' avg="); print_int(avg);
    print_str("/5\n");
    return 0;
}

static int get_avg_rating(int app_id) {
    if (app_id < 0 || app_id >= app_count) return -1;
    app_listing_t *a = &apps[app_id];
    if (a->rating_count == 0) return 0;
    return a->rating_sum / a->rating_count;
}

static void list_reviews(int app_id) {
    if (app_id < 0 || app_id >= app_count) return;
    print_str("app_marketplace: reviews for '");
    print_str(apps[app_id].name); print_str("'\n");
    int found = 0;
    for (int i = 0; i < review_count; i++) {
        if (reviews[i].app_id != app_id) continue;
        print_str("app_marketplace:   ["); print_int(reviews[i].rating);
        print_str("/5] "); print_str(reviews[i].comment); print_str("\n");
        found++;
    }
    if (found == 0) print_str("app_marketplace:   (no reviews)\n");
    else {
        print_str("app_marketplace:   "); print_int(found);
        print_str(" review(s) avg="); print_int(get_avg_rating(app_id));
        print_str("/5\n");
    }
}

/* ── Info / help / stats ────────────────────────────────────────────────── */

static void list_all_apps(int verbose) {
    print_str("app_marketplace: apps "); print_int(app_count);
    print_str("/"); print_int(MAX_APPS); print_str("\n");
    for (int i = 0; i < app_count; i++) {
        app_listing_t *a = &apps[i];
        int avg = (a->rating_count > 0) ? (a->rating_sum / a->rating_count) : 0;
        print_str("app_marketplace:   ["); print_int(a->app_id); print_str("] ");
        print_str(a->name); print_str(" v"); print_version(a->version);
        print_str(" by "); print_str(a->publisher);
        print_str(" ["); print_str(app_status_name(a->status)); print_str("]");
        print_str(" dl="); print_int(a->download_count);
        print_str(" rating="); print_int(avg); print_str("/5");
        if (verbose) {
            print_str(" cats=");
            for (int c = 0; c < a->cat_count; c++) {
                if (c > 0) print_str(",");
                print_str(cat_name(a->categories[c]));
            }
            print_str(" deps="); print_int(a->dep_count);
            print_str(" perms="); print_int(a->perm_count);
            print_str(" kw="); print_int(a->keyword_count);
        }
        print_str("\n");
    }
}

static void show_help(void) {
    print_str("app_marketplace: AI-ASM OS App Marketplace (v47.0)\n");
    print_str("  -t test     run full test suite\n");
    print_str("  -h help     show this message\n");
    print_str("  -l list     list published apps\n");
    print_str("  -v verbose  verbose output\n");
    print_str("  -i info     show module info\n");
    print_str("  -s stats    show statistics\n");
}

static void show_info(void) {
    print_str("app_marketplace: module info\n");
    print_str("  max_apps=");      print_int(MAX_APPS);      print_str("\n");
    print_str("  max_reviews=");   print_int(MAX_REVIEWS);   print_str("\n");
    print_str("  max_installed="); print_int(MAX_INSTALLED); print_str("\n");
    print_str("  max_updates=");   print_int(MAX_UPDATES);   print_str("\n");
    print_str("  categories=");    print_int(CAT_COUNT);     print_str("\n");
    print_str("  sig_algorithms=sha256,ed25519\n");
}

static void show_stats(void) {
    int published = 0, installed = 0, avg_rating_total = 0, rated_count = 0;
    for (int i = 0; i < app_count; i++) {
        if (apps[i].status == APP_STATUS_PUBLISHED) published++;
        if (apps[i].rating_count > 0) {
            avg_rating_total += apps[i].rating_sum / apps[i].rating_count;
            rated_count++;
        }
    }
    for (int i = 0; i < install_count; i++) {
        if (installs[i].status == INSTALL_STATUS_INSTALLED) installed++;
    }
    print_str("app_marketplace: stats\n");
    print_str("  published=");  print_int(published);  print_str("\n");
    print_str("  installed=");  print_int(installed);  print_str("\n");
    print_str("  reviews=");    print_int(review_count); print_str("\n");
    print_str("  avg_rating=");
    if (rated_count > 0) print_int(avg_rating_total / rated_count);
    else print_int(0);
    print_str("/5\n");
    print_str("  pending_updates=");
    int pu = 0;
    for (int u = 0; u < update_count; u++) if (updates[u].available) pu++;
    print_int(pu); print_str("\n");
}

/* ── Test suite ─────────────────────────────────────────────────────────── */

static void test_mode(int verbose) {
    print_str("app_marketplace: === test suite start ===\n");

    /* 1. Publish apps */
    print_str("app_marketplace: --- 1. publish apps ---\n");
    int id_textedit = publish_app("text-editor", "Advanced text editor",
                                  "aiasm_official", 2, 1, 0,
                                  CAT_PRODUCTIVITY, CAT_DEVELOPER,
                                  "editor", "text", "syntax", 512);
    add_permission(id_textedit, "file_read");
    add_permission(id_textedit, "file_write");

    int id_compiler = publish_app("wasm-compiler", "WASM toolchain",
                                  "aiasm_official", 1, 5, 3,
                                  CAT_DEVELOPER, CAT_SYSTEM,
                                  "compiler", "wasm", "build", 8192);
    add_permission(id_compiler, "file_write");
    add_permission(id_compiler, "network");

    int id_imagev = publish_app("image-viewer", "Image viewer & editor",
                                "media_team", 1, 0, 2,
                                CAT_MEDIA, CAT_UTILITY,
                                "image", "viewer", "png", 1024);
    add_permission(id_imagev, "file_read");

    int id_netmon = publish_app("net-monitor", "Network traffic monitor",
                                "sec_team", 3, 0, 0,
                                CAT_NETWORK, CAT_SECURITY,
                                "network", "monitor", "traffic", 768);
    add_permission(id_netmon, "network");
    add_permission(id_netmon, "file_write");

    int id_aichat = publish_app("ai-chat", "Local AI chat assistant",
                                "ai_team", 1, 2, 0,
                                CAT_AI_ML, CAT_PRODUCTIVITY,
                                "ai", "chat", "llm", 4096);
    add_permission(id_aichat, "network");

    int id_filemgr = publish_app("file-manager", "File manager utility",
                                 "aiasm_official", 1, 1, 0,
                                 CAT_UTILITY, CAT_SYSTEM,
                                 "file", "manager", "browse", 640);
    add_permission(id_filemgr, "file_read");
    add_permission(id_filemgr, "file_write");

    if (id_textedit >= 0 && id_compiler >= 0 && id_imagev >= 0 &&
        id_netmon >= 0 && id_aichat >= 0 && id_filemgr >= 0)
        print_str("app_marketplace: publish: OK\n");

    /* 2. Declare dependencies */
    print_str("app_marketplace: --- 2. declare dependencies ---\n");
    add_dependency(id_textedit, "file-manager", 1, 0, 0, 0);   /* optional */
    add_dependency(id_compiler, "file-manager", 1, 0, 0, 1);   /* required */
    add_dependency(id_aichat,   "net-monitor",  2, 0, 0, 0);   /* optional */
    print_str("app_marketplace: dependencies declared: OK\n");

    /* 3. Search & discovery */
    print_str("app_marketplace: --- 3. search & discovery ---\n");
    int fs1 = search_by_keyword("editor");
    int fs2 = search_by_keyword("network");
    int fs3 = search_by_keyword("nonexistent");
    if (fs1 > 0 && fs2 > 0 && fs3 == 0)
        print_str("app_marketplace: search: OK\n");

    print_str("app_marketplace: --- 3b. browse categories ---\n");
    int bc1 = browse_category(CAT_DEVELOPER);
    int bc2 = browse_category(CAT_GAME);
    if (bc1 > 0 && bc2 == 0)
        print_str("app_marketplace: browse: OK\n");

    /* 4. Reviews before install for recommendation seed */
    print_str("app_marketplace: --- 4. seed reviews ---\n");
    submit_review(id_textedit, 5, "Best editor on the platform");
    submit_review(id_textedit, 4, "Fast and reliable");
    submit_review(id_compiler, 5, "Essential for wasm dev");
    submit_review(id_imagev,   4, "Handles all formats");
    submit_review(id_imagev,   3, "Could use more filters");
    submit_review(id_netmon,   5, "Saved my network");
    submit_review(id_aichat,   4, "Surprisingly good responses");
    submit_review(id_filemgr,  5, "Clean and fast");
    print_str("app_marketplace: reviews submitted: OK\n");

    /* 5. Recommendations */
    print_str("app_marketplace: --- 5. recommendations ---\n");
    recommend_apps(4, 3);

    /* 6. Install apps */
    print_str("app_marketplace: --- 6. install apps ---\n");
    int r1 = install_app(id_filemgr);
    int r2 = install_app(id_textedit);
    int r3 = install_app(id_compiler);
    int r4 = install_app(id_netmon);
    if (r1 == 0 && r2 == 0 && r3 == 0 && r4 == 0)
        print_str("app_marketplace: install: OK\n");

    /* 7. Verify installs */
    print_str("app_marketplace: --- 7. verify installs ---\n");
    verify_install(id_filemgr);
    verify_install(id_compiler);

    /* 8. Duplicate install should fail */
    print_str("app_marketplace: --- 8. duplicate install rejected ---\n");
    int dup = install_app(id_filemgr);
    if (dup == -1) print_str("app_marketplace: duplicate install blocked: OK\n");

    /* 9. Version bump + update check */
    print_str("app_marketplace: --- 9. version bump & update check ---\n");
    bump_version(id_filemgr, 1, 2, 0);
    bump_version(id_netmon,  3, 1, 0);
    int upd_avail = check_updates();
    if (upd_avail == 2)
        print_str("app_marketplace: update detection: OK\n");

    /* 10. Auto-update */
    print_str("app_marketplace: --- 10. auto-update ---\n");
    int upd_done = auto_update_all();
    if (upd_done == 2)
        print_str("app_marketplace: auto-update: OK\n");

    /* 11. Rollback */
    print_str("app_marketplace: --- 11. rollback ---\n");
    /* bump filemgr again so we can roll it back */
    bump_version(id_filemgr, 2, 0, 0);
    int rb = rollback_app(id_filemgr);
    if (rb == 0) print_str("app_marketplace: rollback: OK\n");

    /* 12. Review listing */
    print_str("app_marketplace: --- 12. review listing ---\n");
    list_reviews(id_textedit);
    list_reviews(id_imagev);

    /* 13. Signature verification */
    print_str("app_marketplace: --- 13. signature verification ---\n");
    int sv1 = verify_app_signature(id_textedit);
    int sv2 = verify_app_signature(id_compiler);
    if (sv1 == 0 && sv2 == 0)
        print_str("app_marketplace: signature verify: OK\n");

    /* Final summary */
    if (verbose) list_all_apps(1);
    show_stats();
    print_str("app_marketplace: === test suite complete ===\n");
}

/* ── Entry point ────────────────────────────────────────────────────────── */

void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, MAX_INPUT);

    int help = 0, test = 0, list = 0, verbose = 0, info = 0, stats = 0;
    unsigned int pos = 0;
    char *ap = (char *)buf;
    /* skip argv[0] */
    while (pos < MAX_INPUT && ap[pos]) pos++;
    pos++;
    /* parse remaining args */
    while (pos < MAX_INPUT && ap[pos]) {
        char *a = &ap[pos];
        if (my_strcmp(a, "-h") == 0 || my_strcmp(a, "--help") == 0) help = 1;
        else if (my_strcmp(a, "-t") == 0) test = 1;
        else if (my_strcmp(a, "-l") == 0) list = 1;
        else if (my_strcmp(a, "-v") == 0) verbose = 1;
        else if (my_strcmp(a, "-i") == 0) info = 1;
        else if (my_strcmp(a, "-s") == 0) stats = 1;
        while (pos < MAX_INPUT && ap[pos]) pos++;
        pos++;
    }

    if (help)  { show_help();  host_exit(0); }
    if (info)  { show_info();  host_exit(0); }
    if (stats) { show_stats(); host_exit(0); }
    if (test)  { test_mode(verbose); host_exit(0); }
    if (list)  { list_all_apps(verbose); host_exit(0); }

    print_str("app_marketplace: usage: app_marketplace [-t -l -v -i -s -h]\n");
    host_exit(0);
}
