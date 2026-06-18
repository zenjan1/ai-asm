/* cultural_heritage: Cultural heritage administration system (v1.0)
 * Heritage preservation, museum management, archaeology, intangible heritage, restoration
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
#define MAX_MUSEUM       14
#define MAX_ARCHAEOLOGY  12
#define MAX_INTANGIBLE   10
#define MAX_RESTORATION  10

typedef struct {
    int    heritage_id;
    int    heritage_type;
    int    protection_level;
    int    sites_protected;
    int    funding_allocated;
    int    visitors_hosted;
    int    year;
    int    active;
} heritage_t;

typedef struct {
    int    museum_id;
    int    museum_type;
    int    collection_category;
    int    artifacts_held;
    int    exhibitions_held;
    int    education_events;
    int    year;
    int    active;
} museum_t;

typedef struct {
    int    archaeology_id;
    int    site_type;
    int    historical_period;
    int    excavations_conducted;
    int    artifacts_recovered;
    int    publications_made;
    int    year;
    int    active;
} archaeology_t;

typedef struct {
    int    intangible_id;
    int    heritage_category;
    int    tradition_type;
    int    inheritors_registered;
    int    skills_preserved;
    int    events_promoted;
    int    year;
    int    active;
} intangible_t;

typedef struct {
    int    restoration_id;
    int    restoration_type;
    int    material_type;
    int    artifacts_restored;
    int    techniques_applied;
    int    intl_cooperation;
    int    year;
    int    active;
} restoration_t;

typedef struct {
    int    n_heritage;
    int    n_museum;
    int    n_archaeology;
    int    n_intangible;
    int    n_restoration;
    int    total_sites;
    int    total_artifacts;
    int    total_exhibitions;
    int    total_inheritors;
    int    total_restorations;
} ch_state_t;

static heritage_t heritages[MAX_HERITAGE];
static museum_t museums[MAX_MUSEUM];
static archaeology_t archaeologies[MAX_ARCHAEOLOGY];
static intangible_t intangibles[MAX_INTANGIBLE];
static restoration_t restorations[MAX_RESTORATION];
static ch_state_t ch;

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

int ch_init(void) {
    if (initialized) return -1;
    ch.n_heritage = 0; ch.n_museum = 0; ch.n_archaeology = 0;
    ch.n_intangible = 0; ch.n_restoration = 0;
    ch.total_sites = 0; ch.total_artifacts = 0;
    ch.total_exhibitions = 0; ch.total_inheritors = 0;
    ch.total_restorations = 0;
    for (int i = 0; i < MAX_HERITAGE; i++) heritages[i].active = 0;
    for (int i = 0; i < MAX_MUSEUM; i++) museums[i].active = 0;
    for (int i = 0; i < MAX_ARCHAEOLOGY; i++) archaeologies[i].active = 0;
    for (int i = 0; i < MAX_INTANGIBLE; i++) intangibles[i].active = 0;
    for (int i = 0; i < MAX_RESTORATION; i++) restorations[i].active = 0;
    initialized = 1;
    print_str("[CH] Cultural heritage initialized\n");
    return 0;
}

int ch_heritage(int htype, int protection, int sites, int funding, int visitors, int year) {
    if (ch.n_heritage >= MAX_HERITAGE) return -1;
    heritage_t* h = &heritages[ch.n_heritage];
    h->heritage_id = ch.n_heritage;
    h->heritage_type = htype;
    h->protection_level = protection;
    h->sites_protected = sites;
    h->funding_allocated = funding;
    h->visitors_hosted = visitors;
    h->year = year;
    h->active = 1;
    ch.total_sites += sites;
    ch.n_heritage++;
    print_str("[CH] Heritage "); print_int(ch.n_heritage - 1);
    print_str(" type="); print_int(htype);
    print_str(" lvl="); print_int(protection);
    print_str(" st="); print_int(sites);
    print_str(" fnd="); print_int(funding);
    print_str(" vst="); print_int(visitors); print_str("\n");
    return ch.n_heritage - 1;
}

int ch_museum(int mtype, int collection, int artifacts, int exhibitions, int education, int year) {
    if (ch.n_museum >= MAX_MUSEUM) return -1;
    museum_t* m = &museums[ch.n_museum];
    m->museum_id = ch.n_museum;
    m->museum_type = mtype;
    m->collection_category = collection;
    m->artifacts_held = artifacts;
    m->exhibitions_held = exhibitions;
    m->education_events = education;
    m->year = year;
    m->active = 1;
    ch.total_artifacts += artifacts;
    ch.total_exhibitions += exhibitions;
    ch.n_museum++;
    print_str("[CH] Museum "); print_int(ch.n_museum - 1);
    print_str(" type="); print_int(mtype);
    print_str(" col="); print_int(collection);
    print_str(" art="); print_int(artifacts);
    print_str(" exh="); print_int(exhibitions);
    print_str(" edu="); print_int(education); print_str("\n");
    return ch.n_museum - 1;
}

int ch_archaeology(int site_type, int period, int excavations, int artifacts, int publications, int year) {
    if (ch.n_archaeology >= MAX_ARCHAEOLOGY) return -1;
    archaeology_t* a = &archaeologies[ch.n_archaeology];
    a->archaeology_id = ch.n_archaeology;
    a->site_type = site_type;
    a->historical_period = period;
    a->excavations_conducted = excavations;
    a->artifacts_recovered = artifacts;
    a->publications_made = publications;
    a->year = year;
    a->active = 1;
    ch.total_artifacts += artifacts;
    ch.n_archaeology++;
    print_str("[CH] Archaeology "); print_int(ch.n_archaeology - 1);
    print_str(" site="); print_int(site_type);
    print_str(" per="); print_int(period);
    print_str(" exc="); print_int(excavations);
    print_str(" art="); print_int(artifacts);
    print_str(" pub="); print_int(publications); print_str("\n");
    return ch.n_archaeology - 1;
}

int ch_intangible(int category, int tradition, int inheritors, int skills, int events, int year) {
    if (ch.n_intangible >= MAX_INTANGIBLE) return -1;
    intangible_t* ih = &intangibles[ch.n_intangible];
    ih->intangible_id = ch.n_intangible;
    ih->heritage_category = category;
    ih->tradition_type = tradition;
    ih->inheritors_registered = inheritors;
    ih->skills_preserved = skills;
    ih->events_promoted = events;
    ih->year = year;
    ih->active = 1;
    ch.total_inheritors += inheritors;
    ch.n_intangible++;
    print_str("[CH] Intangible "); print_int(ch.n_intangible - 1);
    print_str(" cat="); print_int(category);
    print_str(" trd="); print_int(tradition);
    print_str(" inh="); print_int(inheritors);
    print_str(" skl="); print_int(skills);
    print_str(" evt="); print_int(events); print_str("\n");
    return ch.n_intangible - 1;
}

int ch_restoration(int rtype, int material, int restored, int techniques, int intl, int year) {
    if (ch.n_restoration >= MAX_RESTORATION) return -1;
    restoration_t* r = &restorations[ch.n_restoration];
    r->restoration_id = ch.n_restoration;
    r->restoration_type = rtype;
    r->material_type = material;
    r->artifacts_restored = restored;
    r->techniques_applied = techniques;
    r->intl_cooperation = intl;
    r->year = year;
    r->active = 1;
    ch.total_restorations += restored;
    ch.n_restoration++;
    print_str("[CH] Restoration "); print_int(ch.n_restoration - 1);
    print_str(" type="); print_int(rtype);
    print_str(" mat="); print_int(material);
    print_str(" rst="); print_int(restored);
    print_str(" tch="); print_int(techniques);
    print_str(" int="); print_int(intl); print_str("\n");
    return ch.n_restoration - 1;
}

void ch_heritage_report(void) {
    print_str("[CH] Heritage report:\n");
    print_str("  Heritage categories: "); print_int(ch.n_heritage); print_str("\n");
    print_str("  Total sites protected: "); print_int(ch.total_sites); print_str("\n");
}

void ch_museum_report(void) {
    print_str("[CH] Museum report:\n");
    print_str("  Museums registered: "); print_int(ch.n_museum); print_str("\n");
    print_str("  Total artifacts: "); print_int(ch.total_artifacts); print_str("\n");
    print_str("  Total exhibitions: "); print_int(ch.total_exhibitions); print_str("\n");
}

void ch_restoration_report(void) {
    print_str("[CH] Restoration report:\n");
    print_str("  Archaeology sites: "); print_int(ch.n_archaeology); print_str("\n");
    print_str("  Intangible heritage: "); print_int(ch.n_intangible); print_str("\n");
    print_str("  Total inheritors: "); print_int(ch.total_inheritors); print_str("\n");
    print_str("  Restoration projects: "); print_int(ch.n_restoration); print_str("\n");
    print_str("  Total restorations: "); print_int(ch.total_restorations); print_str("\n");
}

void ch_print_state(void) {
    print_str("[CH] Hr="); print_int(ch.n_heritage);
    print_str(" Mu="); print_int(ch.n_museum);
    print_str(" Ar="); print_int(ch.n_archaeology);
    print_str(" In="); print_int(ch.n_intangible);
    print_str(" Rs="); print_int(ch.n_restoration);
    print_str("\n");
}

int main(void) {
    print_str("=== Cultural Heritage Demo ===\n\n");
    ch_init();

    print_str("Heritage preservation...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int lvl = (i % 4) + 1;
        int sites = 20 + (i * 8);
        int fnd = 500 + (i * 150);
        int vst = 10000 + (i * 3000);
        int year = 2020 + (i % 5);
        ch_heritage(type, lvl, sites, fnd, vst, year);
    }

    print_str("\nMuseum management...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int col = (i % 6) + 1;
        int art = 500 + (i * 200);
        int exh = 10 + (i * 4);
        int edu = 20 + (i * 5);
        int year = 2021 + (i % 4);
        ch_museum(type, col, art, exh, edu, year);
    }

    print_str("\nArchaeological excavations...\n");
    for (int i = 0; i < 12; i++) {
        int site = (i % 5) + 1;
        int per = (i % 6) + 1;
        int exc = 5 + (i * 2);
        int art = 50 + (i * 20);
        int pub = 3 + (i % 5);
        int year = 2022 + (i % 3);
        ch_archaeology(site, per, exc, art, pub, year);
    }

    print_str("\nIntangible heritage...\n");
    for (int i = 0; i < 10; i++) {
        int cat = (i % 4) + 1;
        int trd = (i % 5) + 1;
        int inh = 30 + (i * 10);
        int skl = 20 + (i * 5);
        int evt = 10 + (i * 3);
        int year = 2023 + (i % 2);
        ch_intangible(cat, trd, inh, skl, evt, year);
    }

    print_str("\nRestoration projects...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int mat = (i % 5) + 1;
        int rst = 15 + (i * 4);
        int tch = 5 + (i * 2);
        int intl = 2 + (i % 3);
        int year = 2024;
        ch_restoration(type, mat, rst, tch, intl, year);
    }

    print_str("\nHeritage report...\n");
    ch_heritage_report();

    print_str("\nMuseum report...\n");
    ch_museum_report();

    print_str("\nRestoration report...\n");
    ch_restoration_report();

    print_str("\nFinal state...\n");
    ch_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
