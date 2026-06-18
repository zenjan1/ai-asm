/* media_authority: Media regulation and management (v1.0)
 * Licensing, content review, advertising, statistics, digital media
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

#define MAX_LICENSES   14
#define MAX_ARTICLES   18
#define MAX_ADS        16
#define MAX_STATS      12
#define MAX_DIGITAL    10

typedef struct {
    int    license_id;
    int    type;
    int    zone;
    int    holder;
    int    valid_from;
    int    valid_until;
    int    fee_paid;
    int    status;
    int    active;
} license_t;

typedef struct {
    int    article_id;
    int    type;
    int    source;
    int    status;
    int    reviewer;
    int    year;
    int    month;
    int    flagged;
    int    active;
} article_t;

typedef struct {
    int    ad_id;
    int    type;
    int    medium;
    int    duration_sec;
    int    fee;
    int    status;
    int    year;
    int    violations;
    int    active;
} ad_t;

typedef struct {
    int    stat_id;
    int    type;
    int    zone;
    int    reach;
    int    rating;
    int    circulation;
    int    year;
    int    quarter;
    int    active;
} statistic_t;

typedef struct {
    int    digital_id;
    int    type;
    int    platform;
    int    users;
    int    posts;
    int    flagged_content;
    int    year;
    int    active;
} digital_t;

typedef struct {
    int    n_licenses;
    int    n_articles;
    int    n_ads;
    int    n_stats;
    int    n_digital;
    int    total_fees;
    int    total_flagged;
    int    total_violations;
    int    total_reach;
    int    total_users;
} ma_state_t;

static license_t licenses[MAX_LICENSES];
static article_t articles[MAX_ARTICLES];
static ad_t ads[MAX_ADS];
static statistic_t statistics[MAX_STATS];
static digital_t digitals[MAX_DIGITAL];
static ma_state_t ma;

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

int ma_init(void) {
    if (initialized) return -1;
    ma.n_licenses = 0; ma.n_articles = 0; ma.n_ads = 0;
    ma.n_stats = 0; ma.n_digital = 0;
    ma.total_fees = 0; ma.total_flagged = 0;
    ma.total_violations = 0; ma.total_reach = 0;
    ma.total_users = 0;
    for (int i = 0; i < MAX_LICENSES; i++) licenses[i].active = 0;
    for (int i = 0; i < MAX_ARTICLES; i++) articles[i].active = 0;
    for (int i = 0; i < MAX_ADS; i++) ads[i].active = 0;
    for (int i = 0; i < MAX_STATS; i++) statistics[i].active = 0;
    for (int i = 0; i < MAX_DIGITAL; i++) digitals[i].active = 0;
    initialized = 1;
    print_str("[MA] Media authority initialized\n");
    return 0;
}

int ma_issue_license(int type, int zone, int holder, int valid_from, int valid_until, int fee) {
    if (ma.n_licenses >= MAX_LICENSES) return -1;
    license_t* l = &licenses[ma.n_licenses];
    l->license_id = ma.n_licenses;
    l->type = type;
    l->zone = zone;
    l->holder = holder;
    l->valid_from = valid_from;
    l->valid_until = valid_until;
    l->fee_paid = fee;
    l->status = 1;
    l->active = 1;
    ma.total_fees += fee;
    ma.n_licenses++;
    print_str("[MA] License "); print_int(ma.n_licenses - 1);
    print_str(" type="); print_int(type);
    print_str(" zone="); print_int(zone);
    print_str(" fee=$"); print_int(fee);
    print_str(" "); print_int(valid_from); print_str("-"); print_int(valid_until); print_str("\n");
    return ma.n_licenses - 1;
}

int ma_renew_license(int license_id, int new_until, int fee) {
    if (license_id >= ma.n_licenses) return -1;
    licenses[license_id].valid_until = new_until;
    licenses[license_id].fee_paid += fee;
    ma.total_fees += fee;
    print_str("[MA] Renew L"); print_int(license_id);
    print_str(" until="); print_int(new_until);
    print_str(" fee=$"); print_int(fee); print_str("\n");
    return 0;
}

int ma_revoke_license(int license_id) {
    if (license_id >= ma.n_licenses) return -1;
    licenses[license_id].status = 0;
    print_str("[MA] Revoke L"); print_int(license_id); print_str("\n");
    return 0;
}

int ma_review_article(int type, int source, int reviewer, int year, int month) {
    if (ma.n_articles >= MAX_ARTICLES) return -1;
    article_t* a = &articles[ma.n_articles];
    a->article_id = ma.n_articles;
    a->type = type;
    a->source = source;
    a->status = 1;
    a->reviewer = reviewer;
    a->year = year;
    a->month = month;
    a->flagged = 0;
    a->active = 1;
    ma.n_articles++;
    print_str("[MA] Article "); print_int(ma.n_articles - 1);
    print_str(" type="); print_int(type);
    print_str(" src="); print_int(source);
    print_str(" rev="); print_int(reviewer);
    print_str(" "); print_int(year); print_str("/"); print_int(month); print_str("\n");
    return ma.n_articles - 1;
}

int ma_flag_article(int article_id) {
    if (article_id >= ma.n_articles) return -1;
    articles[article_id].flagged = 1;
    ma.total_flagged++;
    print_str("[MA] Flag A"); print_int(article_id); print_str("\n");
    return 0;
}

int ma_approve_ad(int type, int medium, int duration, int fee, int year) {
    if (ma.n_ads >= MAX_ADS) return -1;
    ad_t* a = &ads[ma.n_ads];
    a->ad_id = ma.n_ads;
    a->type = type;
    a->medium = medium;
    a->duration_sec = duration;
    a->fee = fee;
    a->status = 1;
    a->year = year;
    a->violations = 0;
    a->active = 1;
    ma.total_fees += fee;
    ma.n_ads++;
    print_str("[MA] Ad "); print_int(ma.n_ads - 1);
    print_str(" type="); print_int(type);
    print_str(" med="); print_int(medium);
    print_str(" dur="); print_int(duration); print_str("s");
    print_str(" fee=$"); print_int(fee); print_str("\n");
    return ma.n_ads - 1;
}

int ma_violate_ad(int ad_id) {
    if (ad_id >= ma.n_ads) return -1;
    ads[ad_id].violations++;
    ma.total_violations++;
    print_str("[MA] Violate Ad"); print_int(ad_id); print_str("\n");
    return 0;
}

int ma_record_stat(int type, int zone, int reach, int rating, int circulation, int year, int quarter) {
    if (ma.n_stats >= MAX_STATS) return -1;
    statistic_t* s = &statistics[ma.n_stats];
    s->stat_id = ma.n_stats;
    s->type = type;
    s->zone = zone;
    s->reach = reach;
    s->rating = rating;
    s->circulation = circulation;
    s->year = year;
    s->quarter = quarter;
    s->active = 1;
    ma.total_reach += reach;
    ma.n_stats++;
    print_str("[MA] Stat "); print_int(ma.n_stats - 1);
    print_str(" type="); print_int(type);
    print_str(" zone="); print_int(zone);
    print_str(" reach="); print_int(reach);
    print_str(" rating="); print_int(rating); print_str("\n");
    return ma.n_stats - 1;
}

int ma_register_digital(int type, int platform, int users, int posts, int year) {
    if (ma.n_digital >= MAX_DIGITAL) return -1;
    digital_t* d = &digitals[ma.n_digital];
    d->digital_id = ma.n_digital;
    d->type = type;
    d->platform = platform;
    d->users = users;
    d->posts = posts;
    d->flagged_content = 0;
    d->year = year;
    d->active = 1;
    ma.total_users += users;
    ma.n_digital++;
    print_str("[MA] Digital "); print_int(ma.n_digital - 1);
    print_str(" type="); print_int(type);
    print_str(" plat="); print_int(platform);
    print_str(" users="); print_int(users);
    print_str(" posts="); print_int(posts); print_str("\n");
    return ma.n_digital - 1;
}

int ma_flag_digital(int digital_id, int flagged) {
    if (digital_id >= ma.n_digital) return -1;
    digitals[digital_id].flagged_content += flagged;
    ma.total_flagged += flagged;
    print_str("[MA] Flag D"); print_int(digital_id);
    print_str(" cnt="); print_int(flagged); print_str("\n");
    return 0;
}

void ma_license_report(void) {
    print_str("[MA] License report:\n");
    print_str("  Licenses: "); print_int(ma.n_licenses); print_str("\n");
    print_str("  Total fees: $"); print_int(ma.total_fees); print_str("\n");
}

void ma_content_report(void) {
    print_str("[MA] Content report:\n");
    print_str("  Articles: "); print_int(ma.n_articles); print_str("\n");
    print_str("  Flagged: "); print_int(ma.total_flagged); print_str("\n");
    print_str("  Ad violations: "); print_int(ma.total_violations); print_str("\n");
}

void ma_reach_report(void) {
    print_str("[MA] Reach report:\n");
    print_str("  Total reach: "); print_int(ma.total_reach); print_str("\n");
    print_str("  Digital users: "); print_int(ma.total_users); print_str("\n");
}

void ma_print_state(void) {
    print_str("[MA] Lic="); print_int(ma.n_licenses);
    print_str(" Art="); print_int(ma.n_articles);
    print_str(" Ads="); print_int(ma.n_ads);
    print_str(" Stat="); print_int(ma.n_stats);
    print_str(" Dig="); print_int(ma.n_digital);
    print_str("\n");
}

int main(void) {
    print_str("=== Media Authority Demo ===\n\n");
    ma_init();

    print_str("Issuing licenses...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int zone = (i % 4) + 1;
        int holder = 100 + i;
        int from = 2024;
        int until = 2026 + (i % 3);
        int fee = 5000 + (i * 1000);
        ma_issue_license(type, zone, holder, from, until, fee);
    }

    print_str("\nRenewing licenses...\n");
    for (int i = 0; i < 5; i++) {
        ma_renew_license(i, 2028, 3000);
    }

    print_str("\nReviewing articles...\n");
    for (int i = 0; i < 18; i++) {
        int type = (i % 3) + 1;
        int source = (i % 6) + 1;
        int reviewer = (i % 4) + 1;
        int year = 2024;
        int month = 1 + (i % 12);
        ma_review_article(type, source, reviewer, year, month);
    }

    print_str("\nFlagging articles...\n");
    for (int i = 0; i < 4; i++) {
        ma_flag_article(i * 4);
    }

    print_str("\nApproving ads...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 4) + 1;
        int medium = (i % 3) + 1;
        int dur = 15 + (i * 5);
        int fee = 1000 + (i * 500);
        int year = 2024;
        ma_approve_ad(type, medium, dur, fee, year);
    }

    print_str("\nAd violations...\n");
    for (int i = 0; i < 6; i++) {
        ma_violate_ad(i * 2);
    }

    print_str("\nRecording statistics...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 3) + 1;
        int zone = (i % 4) + 1;
        int reach = 100000 + (i * 50000);
        int rating = 3 + (i % 5);
        int circ = 50000 + (i * 20000);
        int year = 2024;
        int quarter = (i % 4) + 1;
        ma_record_stat(type, zone, reach, rating, circ, year, quarter);
    }

    print_str("\nRegistering digital platforms...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 3) + 1;
        int platform = (i % 5) + 1;
        int users = 500000 + (i * 100000);
        int posts = 1000000 + (i * 500000);
        int year = 2024;
        ma_register_digital(type, platform, users, posts, year);
    }

    print_str("\nFlagging digital content...\n");
    for (int i = 0; i < 8; i++) {
        int flagged = 50 + (i * 20);
        ma_flag_digital(i % 10, flagged);
    }

    print_str("\nRevoking a license...\n");
    ma_revoke_license(3);

    print_str("\nLicense report...\n");
    ma_license_report();

    print_str("\nContent report...\n");
    ma_content_report();

    print_str("\nReach report...\n");
    ma_reach_report();

    print_str("\nFinal state...\n");
    ma_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
