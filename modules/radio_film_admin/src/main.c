/* radio_film_admin: Radio, film & TV administration system (v1.0)
 * Radio, TV, film, online video, media industry
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

#define MAX_RADIO        16
#define MAX_TV           14
#define MAX_FILM         12
#define MAX_ONLINE       10
#define MAX_MEDIA        10

typedef struct {
    int    radio_id;
    int    station_id;
    int    frequency;
    int    power_kw;
    int    listeners;
    int    program_hours;
    int    year;
    int    active;
} radio_t;

typedef struct {
    int    tv_id;
    int    channel_id;
    int    channel_type;
    int    viewers;
    int    program_hours;
    int    rating;
    int    year;
    int    active;
} tv_t;

typedef struct {
    int    film_id;
    int    producer_id;
    int    genre;
    int    production_cost;
    int    box_office;
    int    audience;
    int    year;
    int    active;
} film_t;

typedef struct {
    int    online_id;
    int    platform_id;
    int    content_type;
    int    content_hours;
    int    users;
    int    revenue;
    int    year;
    int    active;
} online_t;

typedef struct {
    int    media_id;
    int    company_id;
    int    sector;
    int    ad_revenue;
    int    employees;
    int    international;
    int    year;
    int    active;
} media_t;

typedef struct {
    int    n_radio;
    int    n_tv;
    int    n_film;
    int    n_online;
    int    n_media;
    int    total_listeners;
    int    total_viewers;
    int    total_box_office;
    int    total_users;
    int    total_revenue;
} rfa_state_t;

static radio_t radios[MAX_RADIO];
static tv_t tvs[MAX_TV];
static film_t films[MAX_FILM];
static online_t onlines[MAX_ONLINE];
static media_t medias[MAX_MEDIA];
static rfa_state_t rfa;

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

int rfa_init(void) {
    if (initialized) return -1;
    rfa.n_radio = 0; rfa.n_tv = 0; rfa.n_film = 0;
    rfa.n_online = 0; rfa.n_media = 0;
    rfa.total_listeners = 0; rfa.total_viewers = 0;
    rfa.total_box_office = 0; rfa.total_users = 0;
    rfa.total_revenue = 0;
    for (int i = 0; i < MAX_RADIO; i++) radios[i].active = 0;
    for (int i = 0; i < MAX_TV; i++) tvs[i].active = 0;
    for (int i = 0; i < MAX_FILM; i++) films[i].active = 0;
    for (int i = 0; i < MAX_ONLINE; i++) onlines[i].active = 0;
    for (int i = 0; i < MAX_MEDIA; i++) medias[i].active = 0;
    initialized = 1;
    print_str("[RFA] Radio/Film admin initialized\n");
    return 0;
}

int rfa_radio(int station, int frequency, int power, int listeners, int hours, int year) {
    if (rfa.n_radio >= MAX_RADIO) return -1;
    radio_t* r = &radios[rfa.n_radio];
    r->radio_id = rfa.n_radio;
    r->station_id = station;
    r->frequency = frequency;
    r->power_kw = power;
    r->listeners = listeners;
    r->program_hours = hours;
    r->year = year;
    r->active = 1;
    rfa.total_listeners += listeners;
    rfa.n_radio++;
    print_str("[RFA] Radio "); print_int(rfa.n_radio - 1);
    print_str(" stn="); print_int(station);
    print_str(" freq="); print_int(frequency);
    print_str(" pwr="); print_int(power); print_str("kW");
    print_str(" lst="); print_int(listeners);
    print_str(" hrs="); print_int(hours); print_str("\n");
    return rfa.n_radio - 1;
}

int rfa_tv(int channel, int channel_type, int viewers, int hours, int rating, int year) {
    if (rfa.n_tv >= MAX_TV) return -1;
    tv_t* t = &tvs[rfa.n_tv];
    t->tv_id = rfa.n_tv;
    t->channel_id = channel;
    t->channel_type = channel_type;
    t->viewers = viewers;
    t->program_hours = hours;
    t->rating = rating;
    t->year = year;
    t->active = 1;
    rfa.total_viewers += viewers;
    rfa.n_tv++;
    print_str("[RFA] TV "); print_int(rfa.n_tv - 1);
    print_str(" chn="); print_int(channel);
    print_str(" type="); print_int(channel_type);
    print_str(" vwr="); print_int(viewers);
    print_str(" hrs="); print_int(hours);
    print_str(" rat="); print_int(rating); print_str("\n");
    return rfa.n_tv - 1;
}

int rfa_film(int producer, int genre, int cost, int box_office, int audience, int year) {
    if (rfa.n_film >= MAX_FILM) return -1;
    film_t* f = &films[rfa.n_film];
    f->film_id = rfa.n_film;
    f->producer_id = producer;
    f->genre = genre;
    f->production_cost = cost;
    f->box_office = box_office;
    f->audience = audience;
    f->year = year;
    f->active = 1;
    rfa.total_box_office += box_office;
    rfa.n_film++;
    print_str("[RFA] Film "); print_int(rfa.n_film - 1);
    print_str(" prd="); print_int(producer);
    print_str(" gnre="); print_int(genre);
    print_str(" cst=$"); print_int(cost);
    print_str(" box=$"); print_int(box_office);
    print_str(" aud="); print_int(audience); print_str("\n");
    return rfa.n_film - 1;
}

int rfa_online(int platform, int content_type, int hours, int users, int revenue, int year) {
    if (rfa.n_online >= MAX_ONLINE) return -1;
    online_t* o = &onlines[rfa.n_online];
    o->online_id = rfa.n_online;
    o->platform_id = platform;
    o->content_type = content_type;
    o->content_hours = hours;
    o->users = users;
    o->revenue = revenue;
    o->year = year;
    o->active = 1;
    rfa.total_users += users;
    rfa.total_revenue += revenue;
    rfa.n_online++;
    print_str("[RFA] Online "); print_int(rfa.n_online - 1);
    print_str(" plt="); print_int(platform);
    print_str(" type="); print_int(content_type);
    print_str(" hrs="); print_int(hours);
    print_str(" usr="); print_int(users);
    print_str(" rev=$"); print_int(revenue); print_str("\n");
    return rfa.n_online - 1;
}

int rfa_media(int company, int sector, int ad_revenue, int employees, int intl, int year) {
    if (rfa.n_media >= MAX_MEDIA) return -1;
    media_t* m = &medias[rfa.n_media];
    m->media_id = rfa.n_media;
    m->company_id = company;
    m->sector = sector;
    m->ad_revenue = ad_revenue;
    m->employees = employees;
    m->international = intl;
    m->year = year;
    m->active = 1;
    rfa.total_revenue += ad_revenue;
    rfa.n_media++;
    print_str("[RFA] Media "); print_int(rfa.n_media - 1);
    print_str(" cmp="); print_int(company);
    print_str(" sec="); print_int(sector);
    print_str(" ad=$"); print_int(ad_revenue);
    print_str(" emp="); print_int(employees);
    print_str(" int="); print_int(intl); print_str("\n");
    return rfa.n_media - 1;
}

void rfa_radio_report(void) {
    print_str("[RFA] Radio report:\n");
    print_str("  Radio stations: "); print_int(rfa.n_radio); print_str("\n");
    print_str("  Total listeners: "); print_int(rfa.total_listeners); print_str("\n");
}

void rfa_tv_report(void) {
    print_str("[RFA] TV report:\n");
    print_str("  TV channels: "); print_int(rfa.n_tv); print_str("\n");
    print_str("  Total viewers: "); print_int(rfa.total_viewers); print_str("\n");
}

void rfa_film_report(void) {
    print_str("[RFA] Film report:\n");
    print_str("  Films produced: "); print_int(rfa.n_film); print_str("\n");
    print_str("  Total box office: $"); print_int(rfa.total_box_office); print_str("\n");
    print_str("  Online platforms: "); print_int(rfa.n_online); print_str("\n");
    print_str("  Total users: "); print_int(rfa.total_users); print_str("\n");
    print_str("  Media companies: "); print_int(rfa.n_media); print_str("\n");
    print_str("  Total revenue: $"); print_int(rfa.total_revenue); print_str("\n");
}

void rfa_print_state(void) {
    print_str("[RFA] Rd="); print_int(rfa.n_radio);
    print_str(" Tv="); print_int(rfa.n_tv);
    print_str(" Fl="); print_int(rfa.n_film);
    print_str(" On="); print_int(rfa.n_online);
    print_str(" Md="); print_int(rfa.n_media);
    print_str("\n");
}

int main(void) {
    print_str("=== Radio/Film Admin Demo ===\n\n");
    rfa_init();

    print_str("Radio broadcasting...\n");
    for (int i = 0; i < 16; i++) {
        int stn = 100 + (i * 5);
        int freq = 88 + (i * 2);
        int pwr = 10 + (i * 5);
        int lst = 100000 + (i * 50000);
        int hrs = 12 + (i % 12);
        int year = 2020 + (i % 5);
        rfa_radio(stn, freq, pwr, lst, hrs, year);
    }

    print_str("\nTelevision...\n");
    for (int i = 0; i < 14; i++) {
        int chn = 1 + i;
        int type = (i % 5) + 1;
        int vwr = 500000 + (i * 200000);
        int hrs = 16 + (i % 8);
        int rat = 1 + (i % 10);
        int year = 2021 + (i % 4);
        rfa_tv(chn, type, vwr, hrs, rat, year);
    }

    print_str("\nFilm production...\n");
    for (int i = 0; i < 12; i++) {
        int prd = 200 + (i * 11);
        int gnre = (i % 6) + 1;
        int cst = 5000000 + (i * 2000000);
        int box = cst * 3;
        int aud = 1000000 + (i * 500000);
        int year = 2022 + (i % 3);
        rfa_film(prd, gnre, cst, box, aud, year);
    }

    print_str("\nOnline video...\n");
    for (int i = 0; i < 10; i++) {
        int plt = 300 + (i * 13);
        int type = (i % 4) + 1;
        int hrs = 10000 + (i * 5000);
        int usr = 1000000 + (i * 500000);
        int rev = 2000000 + (i * 1000000);
        int year = 2023 + (i % 2);
        rfa_online(plt, type, hrs, usr, rev, year);
    }

    print_str("\nMedia industry...\n");
    for (int i = 0; i < 10; i++) {
        int cmp = 400 + (i * 17);
        int sec = (i % 4) + 1;
        int ad = 1000000 + (i * 500000);
        int emp = 100 + (i * 30);
        int intl = (i % 2 == 0) ? 1 : 0;
        int year = 2024;
        rfa_media(cmp, sec, ad, emp, intl, year);
    }

    print_str("\nRadio report...\n");
    rfa_radio_report();

    print_str("\nTV report...\n");
    rfa_tv_report();

    print_str("\nFilm report...\n");
    rfa_film_report();

    print_str("\nFinal state...\n");
    rfa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
