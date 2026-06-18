/* culture_admin: Culture administration system (v1.0)
 * Cultural heritage, arts, media regulation, public culture, cultural industry
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

#define MAX_HERITAGE     16
#define MAX_ARTS         14
#define MAX_MEDIA        12
#define MAX_PUBLIC       10
#define MAX_INDUSTRY     10

typedef struct {
    int    heritage_id;
    int    site_id;
    int    heritage_type;
    int    protection_level;
    int    visitors;
    int    conservation_score;
    int    year;
    int    active;
} heritage_t;

typedef struct {
    int    arts_id;
    int    venue_id;
    int    art_type;
    int    performances;
    int    attendance;
    int    rating;
    int    year;
    int    active;
} arts_t;

typedef struct {
    int    media_id;
    int    publisher_id;
    int    media_type;
    int    titles_published;
    int    circulation;
    int    compliance;
    int    year;
    int    active;
} media_t;

typedef struct {
    int    public_id;
    int    facility_id;
    int    facility_type;
    int    collection_size;
    int    visitors;
    int    programs;
    int    year;
    int    active;
} public_t;

typedef struct {
    int    industry_id;
    int    company_id;
    int    sector;
    int    revenue;
    int    exports;
    int    employment;
    int    year;
    int    active;
} industry_t;

typedef struct {
    int    n_heritage;
    int    n_arts;
    int    n_media;
    int    n_public;
    int    n_industry;
    int    total_visitors;
    int    total_performances;
    int    total_titles;
    int    total_revenue;
    int    total_employment;
} ca_state_t;

static heritage_t heritages[MAX_HERITAGE];
static arts_t arts_list[MAX_ARTS];
static media_t media_list[MAX_MEDIA];
static public_t publics[MAX_PUBLIC];
static industry_t industries[MAX_INDUSTRY];
static ca_state_t ca;

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

int ca_init(void) {
    if (initialized) return -1;
    ca.n_heritage = 0; ca.n_arts = 0; ca.n_media = 0;
    ca.n_public = 0; ca.n_industry = 0;
    ca.total_visitors = 0; ca.total_performances = 0;
    ca.total_titles = 0; ca.total_revenue = 0;
    ca.total_employment = 0;
    for (int i = 0; i < MAX_HERITAGE; i++) heritages[i].active = 0;
    for (int i = 0; i < MAX_ARTS; i++) arts_list[i].active = 0;
    for (int i = 0; i < MAX_MEDIA; i++) media_list[i].active = 0;
    for (int i = 0; i < MAX_PUBLIC; i++) publics[i].active = 0;
    for (int i = 0; i < MAX_INDUSTRY; i++) industries[i].active = 0;
    initialized = 1;
    print_str("[CA] Culture admin initialized\n");
    return 0;
}

int ca_heritage(int site, int heritage_type, int protection_level, int visitors, int conservation, int year) {
    if (ca.n_heritage >= MAX_HERITAGE) return -1;
    heritage_t* h = &heritages[ca.n_heritage];
    h->heritage_id = ca.n_heritage;
    h->site_id = site;
    h->heritage_type = heritage_type;
    h->protection_level = protection_level;
    h->visitors = visitors;
    h->conservation_score = conservation;
    h->year = year;
    h->active = 1;
    ca.total_visitors += visitors;
    ca.n_heritage++;
    print_str("[CA] Heritage "); print_int(ca.n_heritage - 1);
    print_str(" site="); print_int(site);
    print_str(" type="); print_int(heritage_type);
    print_str(" lvl="); print_int(protection_level);
    print_str(" vst="); print_int(visitors);
    print_str(" cns="); print_int(conservation); print_str("\n");
    return ca.n_heritage - 1;
}

int ca_arts(int venue, int art_type, int performances, int attendance, int rating, int year) {
    if (ca.n_arts >= MAX_ARTS) return -1;
    arts_t* a = &arts_list[ca.n_arts];
    a->arts_id = ca.n_arts;
    a->venue_id = venue;
    a->art_type = art_type;
    a->performances = performances;
    a->attendance = attendance;
    a->rating = rating;
    a->year = year;
    a->active = 1;
    ca.total_performances += performances;
    ca.total_visitors += attendance;
    ca.n_arts++;
    print_str("[CA] Arts "); print_int(ca.n_arts - 1);
    print_str(" vnu="); print_int(venue);
    print_str(" type="); print_int(art_type);
    print_str(" prf="); print_int(performances);
    print_str(" att="); print_int(attendance);
    print_str(" rat="); print_int(rating); print_str("\n");
    return ca.n_arts - 1;
}

int ca_media(int publisher, int media_type, int titles, int circulation, int compliance, int year) {
    if (ca.n_media >= MAX_MEDIA) return -1;
    media_t* m = &media_list[ca.n_media];
    m->media_id = ca.n_media;
    m->publisher_id = publisher;
    m->media_type = media_type;
    m->titles_published = titles;
    m->circulation = circulation;
    m->compliance = compliance;
    m->year = year;
    m->active = 1;
    ca.total_titles += titles;
    ca.n_media++;
    print_str("[CA] Media "); print_int(ca.n_media - 1);
    print_str(" pub="); print_int(publisher);
    print_str(" type="); print_int(media_type);
    print_str(" ttl="); print_int(titles);
    print_str(" crc="); print_int(circulation);
    print_str(" cpl="); print_int(compliance); print_str("\n");
    return ca.n_media - 1;
}

int ca_public(int facility, int facility_type, int collection, int visitors, int programs, int year) {
    if (ca.n_public >= MAX_PUBLIC) return -1;
    public_t* p = &publics[ca.n_public];
    p->public_id = ca.n_public;
    p->facility_id = facility;
    p->facility_type = facility_type;
    p->collection_size = collection;
    p->visitors = visitors;
    p->programs = programs;
    p->year = year;
    p->active = 1;
    ca.total_visitors += visitors;
    ca.n_public++;
    print_str("[CA] Public "); print_int(ca.n_public - 1);
    print_str(" fac="); print_int(facility);
    print_str(" type="); print_int(facility_type);
    print_str(" col="); print_int(collection);
    print_str(" vst="); print_int(visitors);
    print_str(" prg="); print_int(programs); print_str("\n");
    return ca.n_public - 1;
}

int ca_industry(int company, int sector, int revenue, int exports, int employment, int year) {
    if (ca.n_industry >= MAX_INDUSTRY) return -1;
    industry_t* ind = &industries[ca.n_industry];
    ind->industry_id = ca.n_industry;
    ind->company_id = company;
    ind->sector = sector;
    ind->revenue = revenue;
    ind->exports = exports;
    ind->employment = employment;
    ind->year = year;
    ind->active = 1;
    ca.total_revenue += revenue;
    ca.total_employment += employment;
    ca.n_industry++;
    print_str("[CA] Industry "); print_int(ca.n_industry - 1);
    print_str(" cmp="); print_int(company);
    print_str(" sec="); print_int(sector);
    print_str(" rev=$"); print_int(revenue);
    print_str(" exp=$"); print_int(exports);
    print_str(" emp="); print_int(employment); print_str("\n");
    return ca.n_industry - 1;
}

void ca_heritage_report(void) {
    print_str("[CA] Heritage report:\n");
    print_str("  Heritage sites: "); print_int(ca.n_heritage); print_str("\n");
    print_str("  Total visitors: "); print_int(ca.total_visitors); print_str("\n");
}

void ca_arts_report(void) {
    print_str("[CA] Arts report:\n");
    print_str("  Arts venues: "); print_int(ca.n_arts); print_str("\n");
    print_str("  Total performances: "); print_int(ca.total_performances); print_str("\n");
}

void ca_industry_report(void) {
    print_str("[CA] Industry report:\n");
    print_str("  Media publishers: "); print_int(ca.n_media); print_str("\n");
    print_str("  Total titles: "); print_int(ca.total_titles); print_str("\n");
    print_str("  Public facilities: "); print_int(ca.n_public); print_str("\n");
    print_str("  Cultural companies: "); print_int(ca.n_industry); print_str("\n");
    print_str("  Total revenue: $"); print_int(ca.total_revenue); print_str("\n");
    print_str("  Total employment: "); print_int(ca.total_employment); print_str("\n");
}

void ca_print_state(void) {
    print_str("[CA] Hr="); print_int(ca.n_heritage);
    print_str(" Ar="); print_int(ca.n_arts);
    print_str(" Md="); print_int(ca.n_media);
    print_str(" Pb="); print_int(ca.n_public);
    print_str(" In="); print_int(ca.n_industry);
    print_str("\n");
}

int main(void) {
    print_str("=== Culture Admin Demo ===\n\n");
    ca_init();

    print_str("Cultural heritage...\n");
    for (int i = 0; i < 16; i++) {
        int site = 1000 + (i * 13);
        int type = (i % 5) + 1;
        int lvl = (i % 4) + 1;
        int vst = 5000 + (i * 2000);
        int cns = 70 + (i * 2);
        int year = 2020 + (i % 5);
        ca_heritage(site, type, lvl, vst, cns, year);
    }

    print_str("\nArts management...\n");
    for (int i = 0; i < 14; i++) {
        int vnu = 2000 + (i * 11);
        int type = (i % 4) + 1;
        int prf = 50 + (i * 10);
        int att = prf * 200;
        int rat = 75 + (i % 20);
        int year = 2021 + (i % 4);
        ca_arts(vnu, type, prf, att, rat, year);
    }

    print_str("\nMedia regulation...\n");
    for (int i = 0; i < 12; i++) {
        int pub = 3000 + (i * 7);
        int type = (i % 3) + 1;
        int ttl = 100 + (i * 30);
        int crc = ttl * 1000;
        int cpl = (i % 4 == 0) ? 0 : 1;
        int year = 2022 + (i % 3);
        ca_media(pub, type, ttl, crc, cpl, year);
    }

    print_str("\nPublic culture...\n");
    for (int i = 0; i < 10; i++) {
        int fac = 4000 + (i * 13);
        int type = (i % 3) + 1;
        int col = 10000 + (i * 5000);
        int vst = 20000 + (i * 8000);
        int prg = 50 + (i * 10);
        int year = 2023 + (i % 2);
        ca_public(fac, type, col, vst, prg, year);
    }

    print_str("\nCultural industry...\n");
    for (int i = 0; i < 10; i++) {
        int cmp = 5000 + (i * 17);
        int sec = (i % 4) + 1;
        int rev = 500000 + (i * 200000);
        int exp = rev / 5;
        int emp = 50 + (i * 20);
        int year = 2024;
        ca_industry(cmp, sec, rev, exp, emp, year);
    }

    print_str("\nHeritage report...\n");
    ca_heritage_report();

    print_str("\nArts report...\n");
    ca_arts_report();

    print_str("\nIndustry report...\n");
    ca_industry_report();

    print_str("\nFinal state...\n");
    ca_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
