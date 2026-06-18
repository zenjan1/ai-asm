/* digital_economy_admin: Digital economy administration system (v1.0)
 * Digital industrialization, industrial digitalization, digital governance, digital society, international cooperation
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

#define MAX_INDUSTRIAL   16
#define MAX_DIGITIZE     14
#define MAX_GOVERNANCE   12
#define MAX_SOCIETY      10
#define MAX_COOPERATE    10

typedef struct {
    int    industrial_id;
    int    industrial_type;
    int    industrial_category;
    int    rd_research;
    int    product_manufacture;
    int    service_provide;
    int    year;
    int    active;
} industrial_t;

typedef struct {
    int    digitize_id;
    int    digitize_type;
    int    digitize_category;
    int    industry_digital;
    int    agriculture_digital;
    int    service_digital;
    int    year;
    int    active;
} digitize_t;

typedef struct {
    int    governance_id;
    int    governance_type;
    int    governance_category;
    int    gov_digital;
    int    regulate_digital;
    int    govern_digital;
    int    year;
    int    active;
} governance_t;

typedef struct {
    int    society_id;
    int    society_type;
    int    society_category;
    int    digital_education;
    int    digital_medical;
    int    digital_culture;
    int    year;
    int    active;
} society_t;

typedef struct {
    int    cooperate_id;
    int    cooperate_type;
    int    cooperate_category;
    int    digital_trade;
    int    digital_diplomacy;
    int    digital_standard;
    int    year;
    int    active;
} cooperate_t;

typedef struct {
    int    n_industrial;
    int    n_digitize;
    int    n_governance;
    int    n_society;
    int    n_cooperate;
    int    total_rd;
    int    total_industry;
    int    total_gov;
    int    total_education;
    int    total_trade;
} dea_state_t;

static industrial_t industrials[MAX_INDUSTRIAL];
static digitize_t digitizes[MAX_DIGITIZE];
static governance_t governances[MAX_GOVERNANCE];
static society_t societies[MAX_SOCIETY];
static cooperate_t cooperates[MAX_COOPERATE];
static dea_state_t dea;

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

int dea_init(void) {
    if (initialized) return -1;
    dea.n_industrial = 0; dea.n_digitize = 0; dea.n_governance = 0;
    dea.n_society = 0; dea.n_cooperate = 0;
    dea.total_rd = 0; dea.total_industry = 0;
    dea.total_gov = 0; dea.total_education = 0;
    dea.total_trade = 0;
    for (int i = 0; i < MAX_INDUSTRIAL; i++) industrials[i].active = 0;
    for (int i = 0; i < MAX_DIGITIZE; i++) digitizes[i].active = 0;
    for (int i = 0; i < MAX_GOVERNANCE; i++) governances[i].active = 0;
    for (int i = 0; i < MAX_SOCIETY; i++) societies[i].active = 0;
    for (int i = 0; i < MAX_COOPERATE; i++) cooperates[i].active = 0;
    initialized = 1;
    print_str("[DEA] Digital economy initialized\n");
    return 0;
}

int dea_industrial(int ind_type, int cat, int rd, int product, int service, int year) {
    if (dea.n_industrial >= MAX_INDUSTRIAL) return -1;
    industrial_t* ind = &industrials[dea.n_industrial];
    ind->industrial_id = dea.n_industrial;
    ind->industrial_type = ind_type;
    ind->industrial_category = cat;
    ind->rd_research = rd;
    ind->product_manufacture = product;
    ind->service_provide = service;
    ind->year = year;
    ind->active = 1;
    dea.total_rd += rd;
    dea.n_industrial++;
    print_str("[DEA] Industrial "); print_int(dea.n_industrial - 1);
    print_str(" type="); print_int(ind_type);
    print_str(" cat="); print_int(cat);
    print_str(" rd="); print_int(rd);
    print_str(" prd="); print_int(product);
    print_str(" svc="); print_int(service); print_str("\n");
    return dea.n_industrial - 1;
}

int dea_digitize(int dig_type, int cat, int industry, int agriculture, int service, int year) {
    if (dea.n_digitize >= MAX_DIGITIZE) return -1;
    digitize_t* d = &digitizes[dea.n_digitize];
    d->digitize_id = dea.n_digitize;
    d->digitize_type = dig_type;
    d->digitize_category = cat;
    d->industry_digital = industry;
    d->agriculture_digital = agriculture;
    d->service_digital = service;
    d->year = year;
    d->active = 1;
    dea.total_industry += industry;
    dea.n_digitize++;
    print_str("[DEA] Digitize "); print_int(dea.n_digitize - 1);
    print_str(" type="); print_int(dig_type);
    print_str(" cat="); print_int(cat);
    print_str(" ind="); print_int(industry);
    print_str(" agr="); print_int(agriculture);
    print_str(" svc="); print_int(service); print_str("\n");
    return dea.n_digitize - 1;
}

int dea_governance(int gov_type, int cat, int gov_dig, int regulate, int govern, int year) {
    if (dea.n_governance >= MAX_GOVERNANCE) return -1;
    governance_t* g = &governances[dea.n_governance];
    g->governance_id = dea.n_governance;
    g->governance_type = gov_type;
    g->governance_category = cat;
    g->gov_digital = gov_dig;
    g->regulate_digital = regulate;
    g->govern_digital = govern;
    g->year = year;
    g->active = 1;
    dea.total_gov += gov_dig;
    dea.n_governance++;
    print_str("[DEA] Governance "); print_int(dea.n_governance - 1);
    print_str(" type="); print_int(gov_type);
    print_str(" cat="); print_int(cat);
    print_str(" gov="); print_int(gov_dig);
    print_str(" reg="); print_int(regulate);
    print_str(" gvr="); print_int(govern); print_str("\n");
    return dea.n_governance - 1;
}

int dea_society(int soc_type, int cat, int education, int medical, int culture, int year) {
    if (dea.n_society >= MAX_SOCIETY) return -1;
    society_t* s = &societies[dea.n_society];
    s->society_id = dea.n_society;
    s->society_type = soc_type;
    s->society_category = cat;
    s->digital_education = education;
    s->digital_medical = medical;
    s->digital_culture = culture;
    s->year = year;
    s->active = 1;
    dea.total_education += education;
    dea.n_society++;
    print_str("[DEA] Society "); print_int(dea.n_society - 1);
    print_str(" type="); print_int(soc_type);
    print_str(" cat="); print_int(cat);
    print_str(" edu="); print_int(education);
    print_str(" med="); print_int(medical);
    print_str(" cul="); print_int(culture); print_str("\n");
    return dea.n_society - 1;
}

int dea_cooperate(int cop_type, int cat, int trade, int diplomacy, int standard, int year) {
    if (dea.n_cooperate >= MAX_COOPERATE) return -1;
    cooperate_t* c = &cooperates[dea.n_cooperate];
    c->cooperate_id = dea.n_cooperate;
    c->cooperate_type = cop_type;
    c->cooperate_category = cat;
    c->digital_trade = trade;
    c->digital_diplomacy = diplomacy;
    c->digital_standard = standard;
    c->year = year;
    c->active = 1;
    dea.total_trade += trade;
    dea.n_cooperate++;
    print_str("[DEA] Cooperate "); print_int(dea.n_cooperate - 1);
    print_str(" type="); print_int(cop_type);
    print_str(" cat="); print_int(cat);
    print_str(" trd="); print_int(trade);
    print_str(" dpl="); print_int(diplomacy);
    print_str(" std="); print_int(standard); print_str("\n");
    return dea.n_cooperate - 1;
}

void dea_industrial_report(void) {
    print_str("[DEA] Industrial report:\n");
    print_str("  Digital industrialization categories: "); print_int(dea.n_industrial); print_str("\n");
    print_str("  Total R&D research: "); print_int(dea.total_rd); print_str("\n");
}

void dea_digitize_report(void) {
    print_str("[DEA] Digitize report:\n");
    print_str("  Industrial digitalization categories: "); print_int(dea.n_digitize); print_str("\n");
    print_str("  Total industry digital: "); print_int(dea.total_industry); print_str("\n");
}

void dea_cooperate_report(void) {
    print_str("[DEA] Cooperate report:\n");
    print_str("  Governance categories: "); print_int(dea.n_governance); print_str("\n");
    print_str("  Total gov digital: "); print_int(dea.total_gov); print_str("\n");
    print_str("  Society categories: "); print_int(dea.n_society); print_str("\n");
    print_str("  Total digital education: "); print_int(dea.total_education); print_str("\n");
    print_str("  Cooperate categories: "); print_int(dea.n_cooperate); print_str("\n");
    print_str("  Total digital trade: "); print_int(dea.total_trade); print_str("\n");
}

void dea_print_state(void) {
    print_str("[DEA] In="); print_int(dea.n_industrial);
    print_str(" Dg="); print_int(dea.n_digitize);
    print_str(" Gv="); print_int(dea.n_governance);
    print_str(" Sc="); print_int(dea.n_society);
    print_str(" Cp="); print_int(dea.n_cooperate);
    print_str("\n");
}

int main(void) {
    print_str("=== Digital Economy Admin Demo ===\n\n");
    dea_init();

    print_str("Digital industrialization...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int rd = 55 + (i * 14);
        int prd = 40 + (i * 10);
        int svc = 25 + (i * 6);
        int year = 2020 + (i % 5);
        dea_industrial(type, cat, rd, prd, svc, year);
    }

    print_str("\nIndustrial digitalization...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ind = 70 + (i * 17);
        int agr = 45 + (i * 11);
        int svc = 30 + (i * 7);
        int year = 2021 + (i % 4);
        dea_digitize(type, cat, ind, agr, svc, year);
    }

    print_str("\nDigital governance...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int gov_dig = 35 + (i * 9);
        int reg = 25 + (i * 6);
        int gvr = 18 + (i * 4);
        int year = 2022 + (i % 3);
        dea_governance(type, cat, gov_dig, reg, gvr, year);
    }

    print_str("\nDigital society...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int edu = 30 + (i * 8);
        int med = 22 + (i * 5);
        int cul = 15 + (i * 4);
        int year = 2023 + (i % 2);
        dea_society(type, cat, edu, med, cul, year);
    }

    print_str("\nInternational cooperation...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int trd = 40 + (i * 10);
        int dpl = 28 + (i * 7);
        int std = 20 + (i * 5);
        int year = 2024;
        dea_cooperate(type, cat, trd, dpl, std, year);
    }

    print_str("\nIndustrial report...\n");
    dea_industrial_report();

    print_str("\nDigitize report...\n");
    dea_digitize_report();

    print_str("\nCooperate report...\n");
    dea_cooperate_report();

    print_str("\nFinal state...\n");
    dea_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
