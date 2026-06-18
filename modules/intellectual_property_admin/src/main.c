/* intellectual_property_admin: IP administration system (v1.0)
 * Patents, trademarks, copyrights, geographical indications, IP services
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

#define MAX_PATENT       16
#define MAX_TRADEMARK    14
#define MAX_COPYRIGHT    12
#define MAX_GEOGRAPHIC   10
#define MAX_IPSERVICE    10

typedef struct {
    int    patent_id;
    int    applicant_id;
    int    patent_type;
    int    field;
    int    filed;
    int    granted;
    int    year;
    int    active;
} patent_t;

typedef struct {
    int    trademark_id;
    int    owner_id;
    int    mark_type;
    int    class_id;
    int    filed;
    int    registered;
    int    year;
    int    active;
} trademark_t;

typedef struct {
    int    copyright_id;
    int    author_id;
    int    work_type;
    int    registration;
    int    transfers;
    int    licensing;
    int    year;
    int    active;
} copyright_t;

typedef struct {
    int    geographic_id;
    int    product_id;
    int    region_id;
    int    product_type;
    int    producers;
    int    certification;
    int    year;
    int    active;
} geographic_t;

typedef struct {
    int    ipservice_id;
    int    provider_id;
    int    service_type;
    int    cases_handled;
    int    valuation;
    int    financing;
    int    year;
    int    active;
} ipservice_t;

typedef struct {
    int    n_patent;
    int    n_trademark;
    int    n_copyright;
    int    n_geographic;
    int    n_ipservice;
    int    total_filed;
    int    total_granted;
    int    total_registered;
    int    total_transfers;
    int    total_valuation;
} ipa_state_t;

static patent_t patents[MAX_PATENT];
static trademark_t trademarks[MAX_TRADEMARK];
static copyright_t copyrights[MAX_COPYRIGHT];
static geographic_t geographics[MAX_GEOGRAPHIC];
static ipservice_t ipservices[MAX_IPSERVICE];
static ipa_state_t ipa;

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

int ipa_init(void) {
    if (initialized) return -1;
    ipa.n_patent = 0; ipa.n_trademark = 0; ipa.n_copyright = 0;
    ipa.n_geographic = 0; ipa.n_ipservice = 0;
    ipa.total_filed = 0; ipa.total_granted = 0;
    ipa.total_registered = 0; ipa.total_transfers = 0;
    ipa.total_valuation = 0;
    for (int i = 0; i < MAX_PATENT; i++) patents[i].active = 0;
    for (int i = 0; i < MAX_TRADEMARK; i++) trademarks[i].active = 0;
    for (int i = 0; i < MAX_COPYRIGHT; i++) copyrights[i].active = 0;
    for (int i = 0; i < MAX_GEOGRAPHIC; i++) geographics[i].active = 0;
    for (int i = 0; i < MAX_IPSERVICE; i++) ipservices[i].active = 0;
    initialized = 1;
    print_str("[IPA] IP admin initialized\n");
    return 0;
}

int ipa_patent(int applicant, int patent_type, int field, int filed, int granted, int year) {
    if (ipa.n_patent >= MAX_PATENT) return -1;
    patent_t* p = &patents[ipa.n_patent];
    p->patent_id = ipa.n_patent;
    p->applicant_id = applicant;
    p->patent_type = patent_type;
    p->field = field;
    p->filed = filed;
    p->granted = granted;
    p->year = year;
    p->active = 1;
    ipa.total_filed += filed;
    ipa.total_granted += granted;
    ipa.n_patent++;
    print_str("[IPA] Patent "); print_int(ipa.n_patent - 1);
    print_str(" app="); print_int(applicant);
    print_str(" type="); print_int(patent_type);
    print_str(" fld="); print_int(field);
    print_str(" fldN="); print_int(filed);
    print_str(" gnt="); print_int(granted); print_str("\n");
    return ipa.n_patent - 1;
}

int ipa_trademark(int owner, int mark_type, int class_id, int filed, int registered, int year) {
    if (ipa.n_trademark >= MAX_TRADEMARK) return -1;
    trademark_t* t = &trademarks[ipa.n_trademark];
    t->trademark_id = ipa.n_trademark;
    t->owner_id = owner;
    t->mark_type = mark_type;
    t->class_id = class_id;
    t->filed = filed;
    t->registered = registered;
    t->year = year;
    t->active = 1;
    ipa.total_registered += registered;
    ipa.n_trademark++;
    print_str("[IPA] Trademark "); print_int(ipa.n_trademark - 1);
    print_str(" own="); print_int(owner);
    print_str(" type="); print_int(mark_type);
    print_str(" cls="); print_int(class_id);
    print_str(" fld="); print_int(filed);
    print_str(" reg="); print_int(registered); print_str("\n");
    return ipa.n_trademark - 1;
}

int ipa_copyright(int author, int work_type, int registration, int transfers, int licensing, int year) {
    if (ipa.n_copyright >= MAX_COPYRIGHT) return -1;
    copyright_t* c = &copyrights[ipa.n_copyright];
    c->copyright_id = ipa.n_copyright;
    c->author_id = author;
    c->work_type = work_type;
    c->registration = registration;
    c->transfers = transfers;
    c->licensing = licensing;
    c->year = year;
    c->active = 1;
    ipa.total_transfers += transfers;
    ipa.n_copyright++;
    print_str("[IPA] Copyright "); print_int(ipa.n_copyright - 1);
    print_str(" aut="); print_int(author);
    print_str(" type="); print_int(work_type);
    print_str(" reg="); print_int(registration);
    print_str(" trf="); print_int(transfers);
    print_str(" lic="); print_int(licensing); print_str("\n");
    return ipa.n_copyright - 1;
}

int ipa_geographic(int product, int region, int product_type, int producers, int certification, int year) {
    if (ipa.n_geographic >= MAX_GEOGRAPHIC) return -1;
    geographic_t* g = &geographics[ipa.n_geographic];
    g->geographic_id = ipa.n_geographic;
    g->product_id = product;
    g->region_id = region;
    g->product_type = product_type;
    g->producers = producers;
    g->certification = certification;
    g->year = year;
    g->active = 1;
    ipa.n_geographic++;
    print_str("[IPA] Geographic "); print_int(ipa.n_geographic - 1);
    print_str(" prd="); print_int(product);
    print_str(" rgn="); print_int(region);
    print_str(" type="); print_int(product_type);
    print_str(" prdN="); print_int(producers);
    print_str(" crt="); print_int(certification); print_str("\n");
    return ipa.n_geographic - 1;
}

int ipa_service(int provider, int service_type, int cases, int valuation, int financing, int year) {
    if (ipa.n_ipservice >= MAX_IPSERVICE) return -1;
    ipservice_t* s = &ipservices[ipa.n_ipservice];
    s->ipservice_id = ipa.n_ipservice;
    s->provider_id = provider;
    s->service_type = service_type;
    s->cases_handled = cases;
    s->valuation = valuation;
    s->financing = financing;
    s->year = year;
    s->active = 1;
    ipa.total_valuation += valuation;
    ipa.n_ipservice++;
    print_str("[IPA] Service "); print_int(ipa.n_ipservice - 1);
    print_str(" prv="); print_int(provider);
    print_str(" type="); print_int(service_type);
    print_str(" cas="); print_int(cases);
    print_str(" val=$"); print_int(valuation);
    print_str(" fin=$"); print_int(financing); print_str("\n");
    return ipa.n_ipservice - 1;
}

void ipa_patent_report(void) {
    print_str("[IPA] Patent report:\n");
    print_str("  Patent categories: "); print_int(ipa.n_patent); print_str("\n");
    print_str("  Total filed: "); print_int(ipa.total_filed); print_str("\n");
    print_str("  Total granted: "); print_int(ipa.total_granted); print_str("\n");
}

void ipa_trademark_report(void) {
    print_str("[IPA] Trademark report:\n");
    print_str("  Trademark classes: "); print_int(ipa.n_trademark); print_str("\n");
    print_str("  Total registered: "); print_int(ipa.total_registered); print_str("\n");
}

void ipa_service_report(void) {
    print_str("[IPA] Service report:\n");
    print_str("  Copyright registrations: "); print_int(ipa.n_copyright); print_str("\n");
    print_str("  Total transfers: "); print_int(ipa.total_transfers); print_str("\n");
    print_str("  Geographic indications: "); print_int(ipa.n_geographic); print_str("\n");
    print_str("  IP service providers: "); print_int(ipa.n_ipservice); print_str("\n");
    print_str("  Total valuation: $"); print_int(ipa.total_valuation); print_str("\n");
}

void ipa_print_state(void) {
    print_str("[IPA] Pt="); print_int(ipa.n_patent);
    print_str(" Tm="); print_int(ipa.n_trademark);
    print_str(" Cr="); print_int(ipa.n_copyright);
    print_str(" Ge="); print_int(ipa.n_geographic);
    print_str(" Sv="); print_int(ipa.n_ipservice);
    print_str("\n");
}

int main(void) {
    print_str("=== IP Admin Demo ===\n\n");
    ipa_init();

    print_str("Patents...\n");
    for (int i = 0; i < 16; i++) {
        int app = 1000 + (i * 11);
        int type = (i % 3) + 1;
        int fld = (i % 8) + 1;
        int fldN = 20 + (i * 5);
        int gnt = fldN - (i % 5);
        int year = 2020 + (i % 5);
        ipa_patent(app, type, fld, fldN, gnt, year);
    }

    print_str("\nTrademarks...\n");
    for (int i = 0; i < 14; i++) {
        int own = 2000 + (i * 7);
        int type = (i % 4) + 1;
        int cls = (i % 45) + 1;
        int fld = 30 + (i * 5);
        int reg = fld - (i % 4);
        int year = 2021 + (i % 4);
        ipa_trademark(own, type, cls, fld, reg, year);
    }

    print_str("\nCopyrights...\n");
    for (int i = 0; i < 12; i++) {
        int aut = 3000 + (i * 13);
        int type = (i % 5) + 1;
        int reg = 50 + (i * 10);
        int trf = i % 6;
        int lic = 5 + (i * 2);
        int year = 2022 + (i % 3);
        ipa_copyright(aut, type, reg, trf, lic, year);
    }

    print_str("\nGeographic indications...\n");
    for (int i = 0; i < 10; i++) {
        int prd = 4000 + (i * 17);
        int rgn = (i % 8) + 1;
        int type = (i % 4) + 1;
        int prdN = 20 + (i * 5);
        int crt = prdN - (i % 3);
        int year = 2023 + (i % 2);
        ipa_geographic(prd, rgn, type, prdN, crt, year);
    }

    print_str("\nIP services...\n");
    for (int i = 0; i < 10; i++) {
        int prv = 5000 + (i * 11);
        int type = (i % 4) + 1;
        int cas = 100 + (i * 20);
        int val = 500000 + (i * 200000);
        int fin = 300000 + (i * 100000);
        int year = 2024;
        ipa_service(prv, type, cas, val, fin, year);
    }

    print_str("\nPatent report...\n");
    ipa_patent_report();

    print_str("\nTrademark report...\n");
    ipa_trademark_report();

    print_str("\nService report...\n");
    ipa_service_report();

    print_str("\nFinal state...\n");
    ipa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
