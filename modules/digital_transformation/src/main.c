/* digital_transformation: Digital transformation administration system (v1.0)
 * Enterprise digital, industry digital, digital infrastructure, digital governance, digital talent
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

#define MAX_ENTERPRISE   16
#define MAX_INDUSTRY     14
#define MAX_INFRA        12
#define MAX_GOVERNANCE   10
#define MAX_TALENT       10

typedef struct {
    int    enterprise_id;
    int    enterprise_type;
    int    enterprise_size;
    int    digital_assessment;
    int    digital_planning;
    int    digital_implementation;
    int    year;
    int    active;
} enterprise_t;

typedef struct {
    int    industry_id;
    int    industry_type;
    int    industry_category;
    int    agriculture_digital;
    int    industry_digital;
    int    service_digital;
    int    year;
    int    active;
} industry_t;

typedef struct {
    int    infra_id;
    int    infra_type;
    int    infra_category;
    int    five_g_network;
    int    data_center;
    int    computing_center;
    int    year;
    int    active;
} infra_t;

typedef struct {
    int    governance_id;
    int    governance_type;
    int    governance_category;
    int    digital_government;
    int    digital_society;
    int    digital_ecology;
    int    year;
    int    active;
} governance_t;

typedef struct {
    int    talent_id;
    int    talent_type;
    int    talent_category;
    int    skill_training;
    int    talent_introduction;
    int    talent_cultivation;
    int    year;
    int    active;
} talent_t;

typedef struct {
    int    n_enterprise;
    int    n_industry;
    int    n_infra;
    int    n_governance;
    int    n_talent;
    int    total_assessment;
    int    total_agriculture;
    int    total_five_g;
    int    total_government;
    int    total_training;
} dtf_state_t;

static enterprise_t enterprises[MAX_ENTERPRISE];
static industry_t industries[MAX_INDUSTRY];
static infra_t infras[MAX_INFRA];
static governance_t governances[MAX_GOVERNANCE];
static talent_t talents[MAX_TALENT];
static dtf_state_t dtf;

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

int dtf_init(void) {
    if (initialized) return -1;
    dtf.n_enterprise = 0; dtf.n_industry = 0; dtf.n_infra = 0;
    dtf.n_governance = 0; dtf.n_talent = 0;
    dtf.total_assessment = 0; dtf.total_agriculture = 0;
    dtf.total_five_g = 0; dtf.total_government = 0;
    dtf.total_training = 0;
    for (int i = 0; i < MAX_ENTERPRISE; i++) enterprises[i].active = 0;
    for (int i = 0; i < MAX_INDUSTRY; i++) industries[i].active = 0;
    for (int i = 0; i < MAX_INFRA; i++) infras[i].active = 0;
    for (int i = 0; i < MAX_GOVERNANCE; i++) governances[i].active = 0;
    for (int i = 0; i < MAX_TALENT; i++) talents[i].active = 0;
    initialized = 1;
    print_str("[DTF] Digital transformation initialized\n");
    return 0;
}

int dtf_enterprise(int ent_type, int size, int assessment, int planning, int implementation, int year) {
    if (dtf.n_enterprise >= MAX_ENTERPRISE) return -1;
    enterprise_t* e = &enterprises[dtf.n_enterprise];
    e->enterprise_id = dtf.n_enterprise;
    e->enterprise_type = ent_type;
    e->enterprise_size = size;
    e->digital_assessment = assessment;
    e->digital_planning = planning;
    e->digital_implementation = implementation;
    e->year = year;
    e->active = 1;
    dtf.total_assessment += assessment;
    dtf.n_enterprise++;
    print_str("[DTF] Enterprise "); print_int(dtf.n_enterprise - 1);
    print_str(" type="); print_int(ent_type);
    print_str(" sz="); print_int(size);
    print_str(" ast="); print_int(assessment);
    print_str(" pln="); print_int(planning);
    print_str(" imp="); print_int(implementation); print_str("\n");
    return dtf.n_enterprise - 1;
}

int dtf_industry(int ind_type, int cat, int agriculture, int industry, int service, int year) {
    if (dtf.n_industry >= MAX_INDUSTRY) return -1;
    industry_t* ind = &industries[dtf.n_industry];
    ind->industry_id = dtf.n_industry;
    ind->industry_type = ind_type;
    ind->industry_category = cat;
    ind->agriculture_digital = agriculture;
    ind->industry_digital = industry;
    ind->service_digital = service;
    ind->year = year;
    ind->active = 1;
    dtf.total_agriculture += agriculture;
    dtf.n_industry++;
    print_str("[DTF] Industry "); print_int(dtf.n_industry - 1);
    print_str(" type="); print_int(ind_type);
    print_str(" cat="); print_int(cat);
    print_str(" agr="); print_int(agriculture);
    print_str(" ind="); print_int(industry);
    print_str(" svc="); print_int(service); print_str("\n");
    return dtf.n_industry - 1;
}

int dtf_infra(int inf_type, int cat, int five_g, int data_ctr, int computing, int year) {
    if (dtf.n_infra >= MAX_INFRA) return -1;
    infra_t* inf = &infras[dtf.n_infra];
    inf->infra_id = dtf.n_infra;
    inf->infra_type = inf_type;
    inf->infra_category = cat;
    inf->five_g_network = five_g;
    inf->data_center = data_ctr;
    inf->computing_center = computing;
    inf->year = year;
    inf->active = 1;
    dtf.total_five_g += five_g;
    dtf.n_infra++;
    print_str("[DTF] Infra "); print_int(dtf.n_infra - 1);
    print_str(" type="); print_int(inf_type);
    print_str(" cat="); print_int(cat);
    print_str(" 5g="); print_int(five_g);
    print_str(" dct="); print_int(data_ctr);
    print_str(" cmp="); print_int(computing); print_str("\n");
    return dtf.n_infra - 1;
}

int dtf_governance(int gov_type, int cat, int government, int society, int ecology, int year) {
    if (dtf.n_governance >= MAX_GOVERNANCE) return -1;
    governance_t* g = &governances[dtf.n_governance];
    g->governance_id = dtf.n_governance;
    g->governance_type = gov_type;
    g->governance_category = cat;
    g->digital_government = government;
    g->digital_society = society;
    g->digital_ecology = ecology;
    g->year = year;
    g->active = 1;
    dtf.total_government += government;
    dtf.n_governance++;
    print_str("[DTF] Governance "); print_int(dtf.n_governance - 1);
    print_str(" type="); print_int(gov_type);
    print_str(" cat="); print_int(cat);
    print_str(" gov="); print_int(government);
    print_str(" soc="); print_int(society);
    print_str(" eco="); print_int(ecology); print_str("\n");
    return dtf.n_governance - 1;
}

int dtf_talent(int tlt_type, int cat, int training, int introduction, int cultivation, int year) {
    if (dtf.n_talent >= MAX_TALENT) return -1;
    talent_t* t = &talents[dtf.n_talent];
    t->talent_id = dtf.n_talent;
    t->talent_type = tlt_type;
    t->talent_category = cat;
    t->skill_training = training;
    t->talent_introduction = introduction;
    t->talent_cultivation = cultivation;
    t->year = year;
    t->active = 1;
    dtf.total_training += training;
    dtf.n_talent++;
    print_str("[DTF] Talent "); print_int(dtf.n_talent - 1);
    print_str(" type="); print_int(tlt_type);
    print_str(" cat="); print_int(cat);
    print_str(" trn="); print_int(training);
    print_str(" itn="); print_int(introduction);
    print_str(" clt="); print_int(cultivation); print_str("\n");
    return dtf.n_talent - 1;
}

void dtf_enterprise_report(void) {
    print_str("[DTF] Enterprise report:\n");
    print_str("  Enterprise categories: "); print_int(dtf.n_enterprise); print_str("\n");
    print_str("  Total digital assessment: "); print_int(dtf.total_assessment); print_str("\n");
}

void dtf_industry_report(void) {
    print_str("[DTF] Industry report:\n");
    print_str("  Industry categories: "); print_int(dtf.n_industry); print_str("\n");
    print_str("  Total agriculture digital: "); print_int(dtf.total_agriculture); print_str("\n");
}

void dtf_talent_report(void) {
    print_str("[DTF] Talent report:\n");
    print_str("  Infrastructure categories: "); print_int(dtf.n_infra); print_str("\n");
    print_str("  Total 5G network: "); print_int(dtf.total_five_g); print_str("\n");
    print_str("  Governance categories: "); print_int(dtf.n_governance); print_str("\n");
    print_str("  Total digital government: "); print_int(dtf.total_government); print_str("\n");
    print_str("  Talent categories: "); print_int(dtf.n_talent); print_str("\n");
    print_str("  Total skill training: "); print_int(dtf.total_training); print_str("\n");
}

void dtf_print_state(void) {
    print_str("[DTF] En="); print_int(dtf.n_enterprise);
    print_str(" In="); print_int(dtf.n_industry);
    print_str(" If="); print_int(dtf.n_infra);
    print_str(" Gv="); print_int(dtf.n_governance);
    print_str(" Tl="); print_int(dtf.n_talent);
    print_str("\n");
}

int main(void) {
    print_str("=== Digital Transformation Demo ===\n\n");
    dtf_init();

    print_str("Enterprise digital...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int size = (i % 4) + 1;
        int ast = 20 + (i * 5);
        int pln = 15 + (i * 4);
        int imp = 10 + (i * 3);
        int year = 2020 + (i % 5);
        dtf_enterprise(type, size, ast, pln, imp, year);
    }

    print_str("\nIndustry digital...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int agr = 30 + (i * 8);
        int ind = 50 + (i * 12);
        int svc = 40 + (i * 10);
        int year = 2021 + (i % 4);
        dtf_industry(type, cat, agr, ind, svc, year);
    }

    print_str("\nDigital infrastructure...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int f5g = 15 + (i * 4);
        int dct = 10 + (i * 3);
        int cmp = 8 + (i * 2);
        int year = 2022 + (i % 3);
        dtf_infra(type, cat, f5g, dct, cmp, year);
    }

    print_str("\nDigital governance...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int gov = 25 + (i * 6);
        int soc = 20 + (i * 5);
        int eco = 15 + (i * 4);
        int year = 2023 + (i % 2);
        dtf_governance(type, cat, gov, soc, eco, year);
    }

    print_str("\nDigital talent...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int trn = 40 + (i * 10);
        int itn = 20 + (i * 5);
        int clt = 15 + (i * 4);
        int year = 2024;
        dtf_talent(type, cat, trn, itn, clt, year);
    }

    print_str("\nEnterprise report...\n");
    dtf_enterprise_report();

    print_str("\nIndustry report...\n");
    dtf_industry_report();

    print_str("\nTalent report...\n");
    dtf_talent_report();

    print_str("\nFinal state...\n");
    dtf_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
