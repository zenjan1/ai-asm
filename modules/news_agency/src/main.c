/* news_agency: News gathering and distribution (v1.0)
 * Reporters, articles, photos, subscriptions, international
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

#define MAX_REPORTERS  16
#define MAX_ARTICLES   20
#define MAX_PHOTOS     14
#define MAX_SUBS       12
#define MAX_INTERNATL  10

typedef struct {
    int    reporter_id;
    int    zone;
    int    specialty;
    int    years_exp;
    int    stories_filed;
    int    bureau_id;
    int    active;
} reporter_t;

typedef struct {
    int    article_id;
    int    reporter_id;
    int    type;
    int    zone;
    int    word_count;
    int    priority;
    int    status;
    int    year;
    int    month;
    int    day;
    int    active;
} article_t;

typedef struct {
    int    photo_id;
    int    reporter_id;
    int    article_id;
    int    type;
    int    resolution;
    int    size_kb;
    int    year;
    int    active;
} photo_t;

typedef struct {
    int    sub_id;
    int    type;
    int    client;
    int    zone;
    int    articles_per_day;
    int    fee_monthly;
    int    year_started;
    int    active;
} subscription_t;

typedef struct {
    int    intl_id;
    int    zone;
    int    reporters;
    int    bureaus;
    int    stories_filed;
    int    year;
    int    active;
} international_t;

typedef struct {
    int    n_reporters;
    int    n_articles;
    int    n_photos;
    int    n_subs;
    int    n_intl;
    int    total_stories;
    int    total_words;
    int    total_photos;
    int    total_revenue;
    int    total_bureaus;
} na_state_t;

static reporter_t reporters[MAX_REPORTERS];
static article_t articles[MAX_ARTICLES];
static photo_t photos[MAX_PHOTOS];
static subscription_t subscriptions[MAX_SUBS];
static international_t international[MAX_INTERNATL];
static na_state_t na;

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

int na_init(void) {
    if (initialized) return -1;
    na.n_reporters = 0; na.n_articles = 0; na.n_photos = 0;
    na.n_subs = 0; na.n_intl = 0;
    na.total_stories = 0; na.total_words = 0;
    na.total_photos = 0; na.total_revenue = 0;
    na.total_bureaus = 0;
    for (int i = 0; i < MAX_REPORTERS; i++) reporters[i].active = 0;
    for (int i = 0; i < MAX_ARTICLES; i++) articles[i].active = 0;
    for (int i = 0; i < MAX_PHOTOS; i++) photos[i].active = 0;
    for (int i = 0; i < MAX_SUBS; i++) subscriptions[i].active = 0;
    for (int i = 0; i < MAX_INTERNATL; i++) international[i].active = 0;
    initialized = 1;
    print_str("[NA] News agency initialized\n");
    return 0;
}

int na_add_reporter(int zone, int specialty, int years, int bureau) {
    if (na.n_reporters >= MAX_REPORTERS) return -1;
    reporter_t* r = &reporters[na.n_reporters];
    r->reporter_id = na.n_reporters;
    r->zone = zone;
    r->specialty = specialty;
    r->years_exp = years;
    r->stories_filed = 0;
    r->bureau_id = bureau;
    r->active = 1;
    na.n_reporters++;
    print_str("[NA] Reporter "); print_int(na.n_reporters - 1);
    print_str(" zone="); print_int(zone);
    print_str(" spec="); print_int(specialty);
    print_str(" yrs="); print_int(years);
    print_str(" bureau="); print_int(bureau); print_str("\n");
    return na.n_reporters - 1;
}

int na_file_article(int reporter_id, int type, int zone, int words, int priority, int year, int month, int day) {
    if (na.n_articles >= MAX_ARTICLES) return -1;
    if (reporter_id >= na.n_reporters) return -1;
    article_t* a = &articles[na.n_articles];
    a->article_id = na.n_articles;
    a->reporter_id = reporter_id;
    a->type = type;
    a->zone = zone;
    a->word_count = words;
    a->priority = priority;
    a->status = 1;
    a->year = year;
    a->month = month;
    a->day = day;
    a->active = 1;
    reporters[reporter_id].stories_filed++;
    na.total_stories++;
    na.total_words += words;
    na.n_articles++;
    print_str("[NA] Article "); print_int(na.n_articles - 1);
    print_str(" R"); print_int(reporter_id);
    print_str(" type="); print_int(type);
    print_str(" zone="); print_int(zone);
    print_str(" words="); print_int(words);
    print_str(" pri="); print_int(priority); print_str("\n");
    return na.n_articles - 1;
}

int na_publish_article(int article_id) {
    if (article_id >= na.n_articles) return -1;
    articles[article_id].status = 2;
    print_str("[NA] Publish A"); print_int(article_id); print_str("\n");
    return 0;
}

int na_add_photo(int reporter_id, int article_id, int type, int resolution, int size, int year) {
    if (na.n_photos >= MAX_PHOTOS) return -1;
    photo_t* p = &photos[na.n_photos];
    p->photo_id = na.n_photos;
    p->reporter_id = reporter_id;
    p->article_id = article_id;
    p->type = type;
    p->resolution = resolution;
    p->size_kb = size;
    p->year = year;
    p->active = 1;
    na.total_photos++;
    na.n_photos++;
    print_str("[NA] Photo "); print_int(na.n_photos - 1);
    print_str(" R"); print_int(reporter_id);
    print_str(" A"); print_int(article_id);
    print_str(" res="); print_int(resolution);
    print_str(" size="); print_int(size); print_str("KB\n");
    return na.n_photos - 1;
}

int na_add_subscription(int type, int client, int zone, int articles_per_day, int fee, int year) {
    if (na.n_subs >= MAX_SUBS) return -1;
    subscription_t* s = &subscriptions[na.n_subs];
    s->sub_id = na.n_subs;
    s->type = type;
    s->client = client;
    s->zone = zone;
    s->articles_per_day = articles_per_day;
    s->fee_monthly = fee;
    s->year_started = year;
    s->active = 1;
    na.total_revenue += fee;
    na.n_subs++;
    print_str("[NA] Sub "); print_int(na.n_subs - 1);
    print_str(" type="); print_int(type);
    print_str(" client="); print_int(client);
    print_str(" zone="); print_int(zone);
    print_str(" art/day="); print_int(articles_per_day);
    print_str(" fee=$"); print_int(fee); print_str("\n");
    return na.n_subs - 1;
}

int na_renew_subscription(int sub_id, int fee) {
    if (sub_id >= na.n_subs) return -1;
    subscriptions[sub_id].fee_monthly += fee;
    na.total_revenue += fee;
    print_str("[NA] Renew S"); print_int(sub_id);
    print_str(" fee=$"); print_int(fee); print_str("\n");
    return 0;
}

int na_add_international(int zone, int reporters, int bureaus, int stories, int year) {
    if (na.n_intl >= MAX_INTERNATL) return -1;
    international_t* in = &international[na.n_intl];
    in->intl_id = na.n_intl;
    in->zone = zone;
    in->reporters = reporters;
    in->bureaus = bureaus;
    in->stories_filed = stories;
    in->year = year;
    in->active = 1;
    na.total_stories += stories;
    na.total_bureaus += bureaus;
    na.n_intl++;
    print_str("[NA] Intl "); print_int(na.n_intl - 1);
    print_str(" zone="); print_int(zone);
    print_str(" rep="); print_int(reporters);
    print_str(" bureaus="); print_int(bureaus);
    print_str(" stories="); print_int(stories); print_str("\n");
    return na.n_intl - 1;
}

void na_reporter_report(void) {
    print_str("[NA] Reporter report:\n");
    print_str("  Reporters: "); print_int(na.n_reporters); print_str("\n");
    print_str("  Total stories: "); print_int(na.total_stories); print_str("\n");
    print_str("  Total words: "); print_int(na.total_words); print_str("\n");
}

void na_article_report(void) {
    print_str("[NA] Article report:\n");
    print_str("  Articles: "); print_int(na.n_articles); print_str("\n");
    print_str("  Photos: "); print_int(na.total_photos); print_str("\n");
}

void na_revenue_report(void) {
    print_str("[NA] Revenue report:\n");
    print_str("  Subscriptions: "); print_int(na.n_subs); print_str("\n");
    print_str("  Total revenue: $"); print_int(na.total_revenue); print_str("\n");
}

void na_print_state(void) {
    print_str("[NA] Rep="); print_int(na.n_reporters);
    print_str(" Art="); print_int(na.n_articles);
    print_str(" Pho="); print_int(na.n_photos);
    print_str(" Sub="); print_int(na.n_subs);
    print_str(" Intl="); print_int(na.n_intl);
    print_str("\n");
}

int main(void) {
    print_str("=== News Agency Demo ===\n\n");
    na_init();

    print_str("Adding reporters...\n");
    for (int i = 0; i < 16; i++) {
        int zone = (i % 4) + 1;
        int spec = (i % 5) + 1;
        int yrs = 1 + (i % 20);
        int bureau = (i % 3) + 1;
        na_add_reporter(zone, spec, yrs, bureau);
    }

    print_str("\nFiling articles...\n");
    for (int i = 0; i < 20; i++) {
        int rep = i % 16;
        int type = (i % 5) + 1;
        int zone = (i % 4) + 1;
        int words = 300 + (i * 100);
        int pri = (i % 4) + 1;
        int year = 2024;
        int month = 1 + (i % 12);
        int day = 1 + (i % 28);
        na_file_article(rep, type, zone, words, pri, year, month, day);
    }

    print_str("\nPublishing articles...\n");
    for (int i = 0; i < 20; i++) {
        na_publish_article(i);
    }

    print_str("\nAdding photos...\n");
    for (int i = 0; i < 14; i++) {
        int rep = i % 16;
        int art = i % 20;
        int type = (i % 3) + 1;
        int res = 2000 + (i * 500);
        int size = 500 + (i * 200);
        int year = 2024;
        na_add_photo(rep, art, type, res, size, year);
    }

    print_str("\nAdding subscriptions...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 3) + 1;
        int client = 200 + i;
        int zone = (i % 4) + 1;
        int apd = 10 + (i * 5);
        int fee = 2000 + (i * 500);
        int year = 2020 + (i % 5);
        na_add_subscription(type, client, zone, apd, fee, year);
    }

    print_str("\nRenewing subscriptions...\n");
    for (int i = 0; i < 6; i++) {
        na_renew_subscription(i, 1000);
    }

    print_str("\nAdding international bureaus...\n");
    for (int i = 0; i < 10; i++) {
        int zone = (i % 6) + 1;
        int reps = 3 + (i % 5);
        int bureaus = 1 + (i % 3);
        int stories = 100 + (i * 50);
        int year = 2022 + (i % 3);
        na_add_international(zone, reps, bureaus, stories, year);
    }

    print_str("\nReporter report...\n");
    na_reporter_report();

    print_str("\nArticle report...\n");
    na_article_report();

    print_str("\nRevenue report...\n");
    na_revenue_report();

    print_str("\nFinal state...\n");
    na_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
