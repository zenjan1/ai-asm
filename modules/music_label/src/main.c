/* music_label: Music label management (v1.0)
 * Artists, albums, releases, royalties, touring, revenue
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

#define MAX_ARTISTS   12
#define MAX_ALBUMS    14
#define MAX_SINGLES   16
#define MAX_CONCERTS  12
#define MAX_LICENSES  10

typedef struct {
    int    artist_id;
    int    genre;
    int    contract_years;
    double advance;
    double royalty_rate;
    int    albums_released;
    int    singles_released;
    int    concerts_done;
    double total_earnings;
    int    active;
} artist_t;

typedef struct {
    int    album_id;
    int    artist_id;
    int    tracks;
    int    year;
    int    units_sold;
    double unit_price;
    double production_cost;
    double revenue;
    int    active;
} album_t;

typedef struct {
    int    single_id;
    int    artist_id;
    int    album_id;
    int    streams;
    double stream_rate;
    double revenue;
    int    active;
} single_t;

typedef struct {
    int    concert_id;
    int    artist_id;
    int    venue_type;
    int    capacity;
    int    tickets_sold;
    double ticket_price;
    double revenue;
    double production_cost;
    int    day;
    int    active;
} concert_t;

typedef struct {
    int    license_id;
    int    artist_id;
    int    type;
    int    album_id;
    double fee;
    int    duration_months;
    int    active;
} license_t;

typedef struct {
    int    n_artists;
    int    n_albums;
    int    n_singles;
    int    n_concerts;
    int    n_licenses;
    double album_revenue;
    double streaming_revenue;
    double concert_revenue;
    double licensing_revenue;
    double merch_revenue;
    double total_revenue;
    double advances_paid;
    double production_costs;
    double royalties_paid;
    int    total_streams;
    int    total_albums_sold;
} ml_state_t;

static artist_t artists[MAX_ARTISTS];
static album_t albums[MAX_ALBUMS];
static single_t singles[MAX_SINGLES];
static concert_t concerts[MAX_CONCERTS];
static license_t licenses[MAX_LICENSES];
static ml_state_t ml;

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

int ml_init(void) {
    if (initialized) return -1;
    ml.n_artists = 0; ml.n_albums = 0; ml.n_singles = 0;
    ml.n_concerts = 0; ml.n_licenses = 0;
    ml.album_revenue = 0.0; ml.streaming_revenue = 0.0;
    ml.concert_revenue = 0.0; ml.licensing_revenue = 0.0;
    ml.merch_revenue = 0.0; ml.total_revenue = 0.0;
    ml.advances_paid = 0.0; ml.production_costs = 0.0;
    ml.royalties_paid = 0.0;
    ml.total_streams = 0; ml.total_albums_sold = 0;
    for (int i = 0; i < MAX_ARTISTS; i++) artists[i].active = 0;
    for (int i = 0; i < MAX_ALBUMS; i++) albums[i].active = 0;
    for (int i = 0; i < MAX_SINGLES; i++) singles[i].active = 0;
    for (int i = 0; i < MAX_CONCERTS; i++) concerts[i].active = 0;
    for (int i = 0; i < MAX_LICENSES; i++) licenses[i].active = 0;
    initialized = 1;
    print_str("[ML] Music label initialized\n");
    return 0;
}

int ml_sign_artist(int genre, int years, double advance, double royalty) {
    if (ml.n_artists >= MAX_ARTISTS) return -1;
    artist_t* a = &artists[ml.n_artists];
    a->artist_id = ml.n_artists;
    a->genre = genre;
    a->contract_years = years;
    a->advance = advance;
    a->royalty_rate = royalty;
    a->albums_released = 0;
    a->singles_released = 0;
    a->concerts_done = 0;
    a->total_earnings = advance;
    a->active = 1;
    ml.advances_paid += advance;
    ml.n_artists++;
    print_str("[ML] Artist "); print_int(ml.n_artists - 1);
    print_str(" genre="); print_int(genre);
    print_str(" yrs="); print_int(years);
    print_str(" adv=$"); print_int((int)advance);
    print_str(" roy="); print_int((int)(royalty * 100)); print_str("%\n");
    return ml.n_artists - 1;
}

int ml_record_album(int artist_id, int tracks, int year, double prod_cost, double price) {
    if (ml.n_albums >= MAX_ALBUMS || artist_id >= ml.n_artists) return -1;
    album_t* al = &albums[ml.n_albums];
    al->album_id = ml.n_albums;
    al->artist_id = artist_id;
    al->tracks = tracks;
    al->year = year;
    al->units_sold = 0;
    al->unit_price = price;
    al->production_cost = prod_cost;
    al->revenue = 0.0;
    al->active = 1;
    ml.production_costs += prod_cost;
    artists[artist_id].albums_released++;
    ml.n_albums++;
    print_str("[ML] Album "); print_int(ml.n_albums - 1);
    print_str(" A"); print_int(artist_id);
    print_str(" tracks="); print_int(tracks);
    print_str(" $"); print_int((int)price); print_str("\n");
    return ml.n_albums - 1;
}

int ml_release_single(int artist_id, int album_id, int streams, double rate) {
    if (ml.n_singles >= MAX_SINGLES || artist_id >= ml.n_artists) return -1;
    single_t* s = &singles[ml.n_singles];
    s->single_id = ml.n_singles;
    s->artist_id = artist_id;
    s->album_id = album_id;
    s->streams = streams;
    s->stream_rate = rate;
    s->revenue = streams * rate;
    s->active = 1;
    ml.streaming_revenue += s->revenue;
    ml.total_revenue += s->revenue;
    ml.total_streams += streams;
    double royalty = s->revenue * artists[artist_id].royalty_rate;
    ml.royalties_paid += royalty;
    artists[artist_id].total_earnings += royalty;
    artists[artist_id].singles_released++;
    ml.n_singles++;
    print_str("[ML] Single "); print_int(ml.n_singles - 1);
    print_str(" A"); print_int(artist_id);
    print_str(" streams="); print_int(streams);
    print_str(" $"); print_int((int)s->revenue); print_str("\n");
    return ml.n_singles - 1;
}

int ml_sell_album(int album_id, int units) {
    if (album_id >= ml.n_albums) return -1;
    album_t* al = &albums[album_id];
    al->units_sold += units;
    double rev = units * al->unit_price;
    al->revenue += rev;
    ml.album_revenue += rev;
    ml.total_revenue += rev;
    ml.total_albums_sold += units;
    double royalty = rev * artists[al->artist_id].royalty_rate;
    ml.royalties_paid += royalty;
    artists[al->artist_id].total_earnings += royalty;
    print_str("[ML] Sell Al"); print_int(album_id);
    print_str(" units="); print_int(units);
    print_str(" $"); print_int((int)rev); print_str("\n");
    return 0;
}

int ml_schedule_concert(int artist_id, int venue_type, int capacity, int sold, double price, double prod_cost, int day) {
    if (ml.n_concerts >= MAX_CONCERTS || artist_id >= ml.n_artists) return -1;
    concert_t* c = &concerts[ml.n_concerts];
    c->concert_id = ml.n_concerts;
    c->artist_id = artist_id;
    c->venue_type = venue_type;
    c->capacity = capacity;
    c->tickets_sold = sold;
    c->ticket_price = price;
    c->revenue = sold * price;
    c->production_cost = prod_cost;
    c->day = day;
    c->active = 1;
    ml.concert_revenue += c->revenue;
    ml.total_revenue += c->revenue;
    ml.production_costs += prod_cost;
    artists[artist_id].concerts_done++;
    double royalty = c->revenue * artists[artist_id].royalty_rate;
    ml.royalties_paid += royalty;
    artists[artist_id].total_earnings += royalty;
    ml.n_concerts++;
    print_str("[ML] Concert "); print_int(ml.n_concerts - 1);
    print_str(" A"); print_int(artist_id);
    print_str(" sold="); print_int(sold);
    print_str(" $"); print_int((int)c->revenue); print_str("\n");
    return ml.n_concerts - 1;
}

int ml_add_license(int artist_id, int type, int album_id, double fee, int months) {
    if (ml.n_licenses >= MAX_LICENSES || artist_id >= ml.n_artists) return -1;
    license_t* l = &licenses[ml.n_licenses];
    l->license_id = ml.n_licenses;
    l->artist_id = artist_id;
    l->type = type;
    l->album_id = album_id;
    l->fee = fee;
    l->duration_months = months;
    l->active = 1;
    ml.licensing_revenue += fee;
    ml.total_revenue += fee;
    double royalty = fee * artists[artist_id].royalty_rate;
    ml.royalties_paid += royalty;
    artists[artist_id].total_earnings += royalty;
    ml.n_licenses++;
    print_str("[ML] License "); print_int(ml.n_licenses - 1);
    print_str(" A"); print_int(artist_id);
    print_str(" type="); print_int(type);
    print_str(" $"); print_int((int)fee); print_str("\n");
    return ml.n_licenses - 1;
}

int ml_add_merch(double amount) {
    ml.merch_revenue += amount;
    ml.total_revenue += amount;
    print_str("[ML] Merch +$"); print_int((int)amount); print_str("\n");
    return 0;
}

void ml_artist_report(void) {
    print_str("[ML] Artist report:\n");
    for (int i = 0; i < ml.n_artists; i++) {
        artist_t* a = &artists[i];
        print_str("  A"); print_int(i);
        print_str(" genre="); print_int(a->genre);
        print_str(" albums="); print_int(a->albums_released);
        print_str(" singles="); print_int(a->singles_released);
        print_str(" concerts="); print_int(a->concerts_done);
        print_str(" earn=$"); print_int((int)a->total_earnings); print_str("\n");
    }
}

void ml_financial_report(void) {
    print_str("[ML] Financial report:\n");
    print_str("  Album revenue: "); print_int((int)ml.album_revenue); print_str("\n");
    print_str("  Streaming revenue: "); print_int((int)ml.streaming_revenue); print_str("\n");
    print_str("  Concert revenue: "); print_int((int)ml.concert_revenue); print_str("\n");
    print_str("  Licensing revenue: "); print_int((int)ml.licensing_revenue); print_str("\n");
    print_str("  Merch revenue: "); print_int((int)ml.merch_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)ml.total_revenue); print_str("\n");
    print_str("  Advances: "); print_int((int)ml.advances_paid); print_str("\n");
    print_str("  Production costs: "); print_int((int)ml.production_costs); print_str("\n");
    print_str("  Royalties: "); print_int((int)ml.royalties_paid); print_str("\n");
    double total_costs = ml.advances_paid + ml.production_costs + ml.royalties_paid;
    double profit = ml.total_revenue - total_costs;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    print_str("  Streams: "); print_int(ml.total_streams);
    print_str(" Albums sold: "); print_int(ml.total_albums_sold); print_str("\n");
}

void ml_print_state(void) {
    print_str("[ML] Artists="); print_int(ml.n_artists);
    print_str(" Albums="); print_int(ml.n_albums);
    print_str(" Singles="); print_int(ml.n_singles);
    print_str(" Concerts="); print_int(ml.n_concerts);
    print_str(" Licenses="); print_int(ml.n_licenses);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)ml.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Music Label Demo ===\n\n");
    ml_init();

    print_str("Signing artists...\n");
    ml_sign_artist(1, 5, 500000.0, 0.15);
    ml_sign_artist(2, 4, 350000.0, 0.12);
    ml_sign_artist(3, 3, 250000.0, 0.10);
    ml_sign_artist(1, 5, 600000.0, 0.18);
    ml_sign_artist(4, 4, 400000.0, 0.14);
    ml_sign_artist(2, 3, 300000.0, 0.11);
    ml_sign_artist(5, 5, 450000.0, 0.16);
    ml_sign_artist(3, 4, 280000.0, 0.13);
    ml_sign_artist(1, 3, 200000.0, 0.09);
    ml_sign_artist(4, 5, 550000.0, 0.17);
    ml_sign_artist(2, 4, 320000.0, 0.12);
    ml_sign_artist(5, 3, 220000.0, 0.10);

    print_str("\nRecording albums...\n");
    ml_record_album(0, 12, 2024, 300000.0, 15.0);
    ml_record_album(1, 10, 2024, 250000.0, 14.0);
    ml_record_album(2, 14, 2024, 200000.0, 12.0);
    ml_record_album(3, 11, 2024, 350000.0, 16.0);
    ml_record_album(4, 13, 2024, 280000.0, 15.0);
    ml_record_album(5, 10, 2024, 220000.0, 13.0);
    ml_record_album(6, 12, 2024, 320000.0, 14.0);
    ml_record_album(7, 11, 2024, 240000.0, 13.0);
    ml_record_album(8, 9, 2024, 180000.0, 11.0);
    ml_record_album(9, 14, 2024, 380000.0, 17.0);
    ml_record_album(10, 10, 2024, 260000.0, 14.0);
    ml_record_album(11, 12, 2024, 200000.0, 12.0);
    ml_record_album(0, 10, 2025, 310000.0, 15.0);
    ml_record_album(3, 13, 2025, 360000.0, 16.0);

    print_str("\nReleasing singles...\n");
    for (int i = 0; i < 16; i++) {
        int artist = i % 12;
        int album = i % 14;
        int streams = 5000000 + (i * 2000000);
        double rate = 0.003 + (i % 5) * 0.001;
        ml_release_single(artist, album, streams, rate);
    }

    print_str("\nSelling albums...\n");
    for (int i = 0; i < 14; i++) {
        ml_sell_album(i, 50000 + (i * 20000));
    }

    print_str("\nScheduling concerts...\n");
    ml_schedule_concert(0, 1, 15000, 14000, 85.0, 500000.0, 60);
    ml_schedule_concert(1, 2, 5000, 4800, 65.0, 200000.0, 65);
    ml_schedule_concert(2, 1, 12000, 11000, 75.0, 400000.0, 70);
    ml_schedule_concert(3, 3, 25000, 24000, 120.0, 800000.0, 75);
    ml_schedule_concert(4, 2, 8000, 7500, 70.0, 300000.0, 80);
    ml_schedule_concert(5, 1, 10000, 9500, 60.0, 350000.0, 85);
    ml_schedule_concert(6, 3, 30000, 28000, 110.0, 900000.0, 90);
    ml_schedule_concert(7, 2, 6000, 5800, 55.0, 250000.0, 95);
    ml_schedule_concert(8, 1, 8000, 7200, 50.0, 280000.0, 100);
    ml_schedule_concert(9, 3, 35000, 33000, 130.0, 1000000.0, 105);
    ml_schedule_concert(10, 2, 7000, 6500, 60.0, 270000.0, 110);
    ml_schedule_concert(11, 1, 9000, 8500, 55.0, 320000.0, 115);

    print_str("\nLicensing...\n");
    ml_add_license(0, 1, 0, 150000.0, 12);
    ml_add_license(3, 2, 3, 200000.0, 24);
    ml_add_license(1, 1, 1, 120000.0, 12);
    ml_add_license(6, 3, 6, 250000.0, 36);
    ml_add_license(4, 2, 4, 180000.0, 18);
    ml_add_license(9, 1, 9, 300000.0, 24);
    ml_add_license(2, 3, 2, 100000.0, 12);
    ml_add_license(7, 2, 7, 140000.0, 18);
    ml_add_license(5, 1, 5, 110000.0, 12);
    ml_add_license(10, 3, 10, 160000.0, 24);

    print_str("\nMerchandise...\n");
    ml_add_merch(200000.0);
    ml_add_merch(350000.0);
    ml_add_merch(180000.0);
    ml_add_merch(420000.0);
    ml_add_merch(280000.0);
    ml_add_merch(150000.0);
    ml_add_merch(380000.0);
    ml_add_merch(220000.0);

    print_str("\nArtist report...\n");
    ml_artist_report();

    print_str("\nFinancial report...\n");
    ml_financial_report();

    print_str("\nFinal state...\n");
    ml_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
