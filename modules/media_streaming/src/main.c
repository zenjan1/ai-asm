/* media_streaming: Media streaming platform (v1.0)
 * Content management, subscriptions, playback, recommendations, analytics
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

#define MAX_CONTENT    16
#define MAX_USERS      12
#define MAX_PLAYLISTS  10
#define MAX_VIEWS      20
#define MAX_RECS       12

typedef struct {
    int    content_id;
    int    type;
    int    category;
    int    duration_min;
    double rating;
    int    n_views;
    int    premium;
    int    active;
} content_t;

typedef struct {
    int    user_id;
    int    tier;
    int    subscription_months;
    double total_paid;
    int    n_watched;
    int    n_downloads;
    int    active;
} user_t;

typedef struct {
    int    playlist_id;
    int    user_id;
    int    n_items;
    int    items[8];
    int    active;
} playlist_t;

typedef struct {
    int    view_id;
    int    user_id;
    int    content_id;
    int    watch_duration;
    int    completed;
    int    liked;
    int    active;
} view_t;

typedef struct {
    int    rec_id;
    int    user_id;
    int    content_id;
    double score;
    int    type;
    int    active;
} rec_t;

typedef struct {
    int    n_content;
    int    n_users;
    int    n_playlists;
    int    n_views;
    int    n_recs;
    double total_revenue;
    int    premium_users;
    int    total_watch_min;
    int    total_downloads;
} ms_state_t;

static content_t contents[MAX_CONTENT];
static user_t users[MAX_USERS];
static playlist_t playlists[MAX_PLAYLISTS];
static view_t views[MAX_VIEWS];
static rec_t recs[MAX_RECS];
static ms_state_t ms;

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

int ms_init(void) {
    if (initialized) return -1;
    ms.n_content = 0; ms.n_users = 0; ms.n_playlists = 0;
    ms.n_views = 0; ms.n_recs = 0;
    ms.total_revenue = 0.0; ms.premium_users = 0;
    ms.total_watch_min = 0; ms.total_downloads = 0;
    for (int i = 0; i < MAX_CONTENT; i++) contents[i].active = 0;
    for (int i = 0; i < MAX_USERS; i++) users[i].active = 0;
    for (int i = 0; i < MAX_PLAYLISTS; i++) playlists[i].active = 0;
    for (int i = 0; i < MAX_VIEWS; i++) views[i].active = 0;
    for (int i = 0; i < MAX_RECS; i++) recs[i].active = 0;
    initialized = 1;
    print_str("[MS] Media streaming initialized\n");
    return 0;
}

int ms_add_content(int type, int category, int duration, double rating, int premium) {
    if (ms.n_content >= MAX_CONTENT) return -1;
    content_t* c = &contents[ms.n_content];
    c->content_id = ms.n_content;
    c->type = type;
    c->category = category;
    c->duration_min = duration;
    c->rating = rating;
    c->n_views = 0;
    c->premium = premium;
    c->active = 1;
    ms.n_content++;
    print_str("[MS] Content "); print_int(ms.n_content - 1);
    print_str(" type="); print_int(type);
    print_str(" cat="); print_int(category);
    print_str(" dur="); print_int(duration); print_str("min");
    print_str(" rating="); print_int((int)(rating * 10));
    if (premium) print_str(" [PREMIUM]");
    print_str("\n");
    return ms.n_content - 1;
}

int ms_register_user(int tier) {
    if (ms.n_users >= MAX_USERS) return -1;
    user_t* u = &users[ms.n_users];
    u->user_id = ms.n_users;
    u->tier = tier;
    u->subscription_months = 0;
    u->total_paid = 0.0;
    u->n_watched = 0;
    u->n_downloads = 0;
    u->active = 1;
    ms.n_users++;
    if (tier >= 2) ms.premium_users++;
    print_str("[MS] User "); print_int(ms.n_users - 1);
    print_str(" tier="); print_int(tier); print_str("\n");
    return ms.n_users - 1;
}

int ms_subscribe(int user_id, int months) {
    if (user_id >= ms.n_users) return -1;
    user_t* u = &users[user_id];
    double monthly_fee = (u->tier == 0) ? 0.0 : (u->tier == 1) ? 9.99 : 14.99;
    double total = monthly_fee * months;
    u->subscription_months += months;
    u->total_paid += total;
    ms.total_revenue += total;
    print_str("[MS] User "); print_int(user_id);
    print_str(" subscribed "); print_int(months); print_str("mo");
    print_str(" fee="); print_int((int)total); print_str("\n");
    return 0;
}

int ms_create_playlist(int user_id) {
    if (ms.n_playlists >= MAX_PLAYLISTS || user_id >= ms.n_users) return -1;
    playlist_t* p = &playlists[ms.n_playlists];
    p->playlist_id = ms.n_playlists;
    p->user_id = user_id;
    p->n_items = 0;
    for (int i = 0; i < 8; i++) p->items[i] = -1;
    p->active = 1;
    ms.n_playlists++;
    print_str("[MS] Playlist "); print_int(ms.n_playlists - 1);
    print_str(" user="); print_int(user_id); print_str("\n");
    return ms.n_playlists - 1;
}

int ms_add_to_playlist(int playlist_id, int content_id) {
    if (playlist_id >= ms.n_playlists || content_id >= ms.n_content) return -1;
    playlist_t* p = &playlists[playlist_id];
    if (p->n_items >= 8) return -2;
    p->items[p->n_items] = content_id;
    p->n_items++;
    print_str("[MS] Added C"); print_int(content_id);
    print_str(" to PL"); print_int(playlist_id);
    print_str(" ("); print_int(p->n_items); print_str("/8)\n");
    return 0;
}

int ms_record_view(int user_id, int content_id, int watch_dur, int liked) {
    if (ms.n_views >= MAX_VIEWS) return -1;
    if (user_id >= ms.n_users || content_id >= ms.n_content) return -2;
    content_t* c = &contents[content_id];
    if (c->premium && users[user_id].tier < 2) return -3;
    view_t* v = &views[ms.n_views];
    v->view_id = ms.n_views;
    v->user_id = user_id;
    v->content_id = content_id;
    v->watch_duration = watch_dur;
    v->completed = (watch_dur >= c->duration_min * 90 / 100) ? 1 : 0;
    v->liked = liked;
    v->active = 1;
    c->n_views++;
    users[user_id].n_watched++;
    ms.total_watch_min += watch_dur;
    ms.n_views++;
    print_str("[MS] View "); print_int(ms.n_views - 1);
    print_str(" U"); print_int(user_id);
    print_str(" C"); print_int(content_id);
    print_str(" "); print_int(watch_dur); print_str("/"); print_int(c->duration_min); print_str("min");
    if (v->completed) print_str(" COMPLETE");
    if (liked) print_str(" LIKED");
    print_str("\n");
    return ms.n_views - 1;
}

int ms_download(int user_id, int content_id) {
    if (user_id >= ms.n_users || content_id >= ms.n_content) return -1;
    if (users[user_id].tier < 2) return -2;
    users[user_id].n_downloads++;
    ms.total_downloads++;
    print_str("[MS] U"); print_int(user_id);
    print_str(" downloaded C"); print_int(content_id); print_str("\n");
    return 0;
}

void ms_generate_recommendations(void) {
    for (int u = 0; u < ms.n_users; u++) {
        double best_score = 0.0;
        int best_content = -1;
        for (int c = 0; c < ms.n_content; c++) {
            int watched = 0;
            for (int v = 0; v < ms.n_views; v++) {
                if (views[v].user_id == u && views[v].content_id == c) {
                    watched = 1;
                    break;
                }
            }
            if (watched) continue;
            if (contents[c].premium && users[u].tier < 2) continue;
            double score = contents[c].rating * 10.0;
            for (int v = 0; v < ms.n_views; v++) {
                if (views[v].user_id == u && views[v].content_id != c) {
                    if (contents[views[v].content_id].category == contents[c].category) {
                        score += 20.0;
                    }
                    if (views[v].liked) score += 10.0;
                }
            }
            if (score > best_score) {
                best_score = score;
                best_content = c;
            }
        }
        if (best_content >= 0 && ms.n_recs < MAX_RECS) {
            rec_t* r = &recs[ms.n_recs];
            r->rec_id = ms.n_recs;
            r->user_id = u;
            r->content_id = best_content;
            r->score = best_score;
            r->type = 1;
            r->active = 1;
            ms.n_recs++;
            print_str("[MS] Rec: U"); print_int(u);
            print_str(" -> C"); print_int(best_content);
            print_str(" score="); print_int((int)best_score); print_str("\n");
        }
    }
}

void ms_analytics(void) {
    print_str("[MS] Analytics:\n");
    print_str("  Content: "); print_int(ms.n_content); print_str("\n");
    print_str("  Users: "); print_int(ms.n_users); print_str("\n");
    print_str("  Premium users: "); print_int(ms.premium_users); print_str("\n");
    print_str("  Total revenue: "); print_int((int)ms.total_revenue); print_str("\n");
    print_str("  Total watch time: "); print_int(ms.total_watch_min); print_str("min\n");
    print_str("  Downloads: "); print_int(ms.total_downloads); print_str("\n");
    print_str("  Top content:\n");
    for (int i = 0; i < ms.n_content; i++) {
        print_str("    C"); print_int(i);
        print_str(" views="); print_int(contents[i].n_views);
        print_str(" rating="); print_int((int)(contents[i].rating * 10)); print_str("\n");
    }
}

void ms_print_state(void) {
    print_str("[MS] Content="); print_int(ms.n_content);
    print_str(" Users="); print_int(ms.n_users);
    print_str(" Playlists="); print_int(ms.n_playlists);
    print_str(" Views="); print_int(ms.n_views);
    print_str(" Recs="); print_int(ms.n_recs);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)ms.total_revenue);
    print_str(" Watch: "); print_int(ms.total_watch_min); print_str("min\n");
}

int main(void) {
    print_str("=== Media Streaming Platform Demo ===\n\n");
    ms_init();

    print_str("Adding content...\n");
    ms_add_content(1, 1, 120, 8.5, 0);
    ms_add_content(2, 1, 45, 7.8, 0);
    ms_add_content(1, 2, 90, 9.0, 1);
    ms_add_content(3, 2, 30, 7.5, 0);
    ms_add_content(1, 3, 150, 8.2, 1);
    ms_add_content(2, 3, 60, 8.8, 0);
    ms_add_content(1, 1, 100, 7.9, 0);
    ms_add_content(3, 2, 25, 8.1, 1);
    ms_add_content(1, 3, 110, 8.6, 0);
    ms_add_content(2, 1, 50, 7.2, 0);

    print_str("\nRegistering users...\n");
    ms_register_user(0);
    ms_register_user(1);
    ms_register_user(2);
    ms_register_user(0);
    ms_register_user(1);
    ms_register_user(2);
    ms_register_user(0);
    ms_register_user(2);

    print_str("\nSubscriptions...\n");
    ms_subscribe(1, 3);
    ms_subscribe(2, 6);
    ms_subscribe(4, 2);
    ms_subscribe(5, 12);
    ms_subscribe(7, 6);

    print_str("\nCreating playlists...\n");
    int pl0 = ms_create_playlist(0);
    int pl1 = ms_create_playlist(2);
    int pl2 = ms_create_playlist(5);
    ms_add_to_playlist(pl0, 0);
    ms_add_to_playlist(pl0, 1);
    ms_add_to_playlist(pl0, 6);
    ms_add_to_playlist(pl1, 2);
    ms_add_to_playlist(pl1, 3);
    ms_add_to_playlist(pl1, 7);
    ms_add_to_playlist(pl2, 4);
    ms_add_to_playlist(pl2, 5);
    ms_add_to_playlist(pl2, 8);

    print_str("\nRecording views...\n");
    ms_record_view(0, 0, 100, 1);
    ms_record_view(0, 1, 40, 0);
    ms_record_view(1, 0, 120, 1);
    ms_record_view(1, 2, 85, 1);
    ms_record_view(2, 2, 90, 1);
    ms_record_view(2, 3, 28, 0);
    ms_record_view(2, 7, 25, 1);
    ms_record_view(3, 0, 110, 1);
    ms_record_view(3, 6, 95, 1);
    ms_record_view(4, 1, 45, 0);
    ms_record_view(5, 4, 140, 1);
    ms_record_view(5, 5, 55, 1);
    ms_record_view(5, 8, 100, 0);
    ms_record_view(6, 0, 80, 0);
    ms_record_view(7, 4, 150, 1);
    ms_record_view(7, 5, 60, 1);
    ms_record_view(7, 7, 25, 1);

    print_str("\nDownloads...\n");
    ms_download(2, 3);
    ms_download(5, 4);
    ms_download(7, 7);

    print_str("\nGenerating recommendations...\n");
    ms_generate_recommendations();

    print_str("\nAnalytics...\n");
    ms_analytics();

    print_str("\nFinal state...\n");
    ms_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
