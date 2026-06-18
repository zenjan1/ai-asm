/* science_admin: Science administration system (v1.0)
 * Research management, tech innovation, IP, science parks, talent
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

#define MAX_RESEARCH     16
#define MAX_INNOVATION   14
#define MAX_IP           12
#define MAX_PARK         10
#define MAX_TALENT       10

typedef struct {
    int    research_id;
    int    institution_id;
    int    field;
    int    funding;
    int    papers;
    int    citations;
    int    year;
    int    active;
} research_t;

typedef struct {
    int    innovation_id;
    int    company_id;
    int    tech_field;
    int    rd_expense;
    int    patents_filed;
    int    revenue_from_innovation;
    int    year;
    int    active;
} innovation_t;

typedef struct {
    int    ip_id;
    int    applicant_id;
    int    ip_type;
    int    applications;
    int    granted;
    int    international;
    int    year;
    int    active;
} ip_t;

typedef struct {
    int    park_id;
    int    region_id;
    int    park_type;
    int    companies;
    int    incubatees;
    int    total_revenue;
    int    year;
    int    active;
} park_t;

typedef struct {
    int    talent_id;
    int    person_id;
    int    talent_level;
    int    field;
    int    achievements;
    int    funding_secured;
    int    year;
    int    active;
} talent_t;

typedef struct {
    int    n_research;
    int    n_innovation;
    int    n_ip;
    int    n_park;
    int    n_talent;
    int    total_funding;
    int    total_papers;
    int    total_patents;
    int    total_companies;
    int    total_revenue;
} sci_state_t;

static research_t researches[MAX_RESEARCH];
static innovation_t innovations[MAX_INNOVATION];
static ip_t ips[MAX_IP];
static park_t parks[MAX_PARK];
static talent_t talents[MAX_TALENT];
static sci_state_t sci;

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

int sci_init(void) {
    if (initialized) return -1;
    sci.n_research = 0; sci.n_innovation = 0; sci.n_ip = 0;
    sci.n_park = 0; sci.n_talent = 0;
    sci.total_funding = 0; sci.total_papers = 0;
    sci.total_patents = 0; sci.total_companies = 0;
    sci.total_revenue = 0;
    for (int i = 0; i < MAX_RESEARCH; i++) researches[i].active = 0;
    for (int i = 0; i < MAX_INNOVATION; i++) innovations[i].active = 0;
    for (int i = 0; i < MAX_IP; i++) ips[i].active = 0;
    for (int i = 0; i < MAX_PARK; i++) parks[i].active = 0;
    for (int i = 0; i < MAX_TALENT; i++) talents[i].active = 0;
    initialized = 1;
    print_str("[SCI] Science admin initialized\n");
    return 0;
}

int sci_research(int institution, int field, int funding, int papers, int citations, int year) {
    if (sci.n_research >= MAX_RESEARCH) return -1;
    research_t* r = &researches[sci.n_research];
    r->research_id = sci.n_research;
    r->institution_id = institution;
    r->field = field;
    r->funding = funding;
    r->papers = papers;
    r->citations = citations;
    r->year = year;
    r->active = 1;
    sci.total_funding += funding;
    sci.total_papers += papers;
    sci.n_research++;
    print_str("[SCI] Research "); print_int(sci.n_research - 1);
    print_str(" inst="); print_int(institution);
    print_str(" fld="); print_int(field);
    print_str(" fnd=$"); print_int(funding);
    print_str(" pap="); print_int(papers);
    print_str(" cit="); print_int(citations); print_str("\n");
    return sci.n_research - 1;
}

int sci_innovation(int company, int tech_field, int rd_expense, int patents, int revenue, int year) {
    if (sci.n_innovation >= MAX_INNOVATION) return -1;
    innovation_t* inv = &innovations[sci.n_innovation];
    inv->innovation_id = sci.n_innovation;
    inv->company_id = company;
    inv->tech_field = tech_field;
    inv->rd_expense = rd_expense;
    inv->patents_filed = patents;
    inv->revenue_from_innovation = revenue;
    inv->year = year;
    inv->active = 1;
    sci.total_patents += patents;
    sci.total_revenue += revenue;
    sci.n_innovation++;
    print_str("[SCI] Innovation "); print_int(sci.n_innovation - 1);
    print_str(" cmp="); print_int(company);
    print_str(" fld="); print_int(tech_field);
    print_str(" rd=$"); print_int(rd_expense);
    print_str(" pat="); print_int(patents);
    print_str(" rev=$"); print_int(revenue); print_str("\n");
    return sci.n_innovation - 1;
}

int sci_ip(int applicant, int ip_type, int applications, int granted, int intl, int year) {
    if (sci.n_ip >= MAX_IP) return -1;
    ip_t* ip = &ips[sci.n_ip];
    ip->ip_id = sci.n_ip;
    ip->applicant_id = applicant;
    ip->ip_type = ip_type;
    ip->applications = applications;
    ip->granted = granted;
    ip->international = intl;
    ip->year = year;
    ip->active = 1;
    sci.total_patents += granted;
    sci.n_ip++;
    print_str("[SCI] IP "); print_int(sci.n_ip - 1);
    print_str(" app="); print_int(applicant);
    print_str(" type="); print_int(ip_type);
    print_str(" apl="); print_int(applications);
    print_str(" gnt="); print_int(granted);
    print_str(" int="); print_int(intl); print_str("\n");
    return sci.n_ip - 1;
}

int sci_park(int region, int park_type, int companies, int incubatees, int revenue, int year) {
    if (sci.n_park >= MAX_PARK) return -1;
    park_t* p = &parks[sci.n_park];
    p->park_id = sci.n_park;
    p->region_id = region;
    p->park_type = park_type;
    p->companies = companies;
    p->incubatees = incubatees;
    p->total_revenue = revenue;
    p->year = year;
    p->active = 1;
    sci.total_companies += companies;
    sci.total_revenue += revenue;
    sci.n_park++;
    print_str("[SCI] Park "); print_int(sci.n_park - 1);
    print_str(" rgn="); print_int(region);
    print_str(" type="); print_int(park_type);
    print_str(" cmp="); print_int(companies);
    print_str(" inc="); print_int(incubatees);
    print_str(" rev=$"); print_int(revenue); print_str("\n");
    return sci.n_park - 1;
}

int sci_talent(int person, int talent_level, int field, int achievements, int funding, int year) {
    if (sci.n_talent >= MAX_TALENT) return -1;
    talent_t* t = &talents[sci.n_talent];
    t->talent_id = sci.n_talent;
    t->person_id = person;
    t->talent_level = talent_level;
    t->field = field;
    t->achievements = achievements;
    t->funding_secured = funding;
    t->year = year;
    t->active = 1;
    sci.total_funding += funding;
    sci.n_talent++;
    print_str("[SCI] Talent "); print_int(sci.n_talent - 1);
    print_str(" prs="); print_int(person);
    print_str(" lvl="); print_int(talent_level);
    print_str(" fld="); print_int(field);
    print_str(" ach="); print_int(achievements);
    print_str(" fnd=$"); print_int(funding); print_str("\n");
    return sci.n_talent - 1;
}

void sci_research_report(void) {
    print_str("[SCI] Research report:\n");
    print_str("  Research projects: "); print_int(sci.n_research); print_str("\n");
    print_str("  Total funding: $"); print_int(sci.total_funding); print_str("\n");
    print_str("  Total papers: "); print_int(sci.total_papers); print_str("\n");
}

void sci_innovation_report(void) {
    print_str("[SCI] Innovation report:\n");
    print_str("  Innovation companies: "); print_int(sci.n_innovation); print_str("\n");
    print_str("  Total patents: "); print_int(sci.total_patents); print_str("\n");
    print_str("  Total revenue: $"); print_int(sci.total_revenue); print_str("\n");
}

void sci_talent_report(void) {
    print_str("[SCI] Talent report:\n");
    print_str("  IP registrations: "); print_int(sci.n_ip); print_str("\n");
    print_str("  Science parks: "); print_int(sci.n_park); print_str("\n");
    print_str("  Total companies: "); print_int(sci.total_companies); print_str("\n");
    print_str("  Talents managed: "); print_int(sci.n_talent); print_str("\n");
}

void sci_print_state(void) {
    print_str("[SCI] Rs="); print_int(sci.n_research);
    print_str(" In="); print_int(sci.n_innovation);
    print_str(" Ip="); print_int(sci.n_ip);
    print_str(" Pk="); print_int(sci.n_park);
    print_str(" Tl="); print_int(sci.n_talent);
    print_str("\n");
}

int main(void) {
    print_str("=== Science Admin Demo ===\n\n");
    sci_init();

    print_str("Research management...\n");
    for (int i = 0; i < 16; i++) {
        int inst = 100 + (i * 10);
        int fld = (i % 6) + 1;
        int fnd = 500000 + (i * 200000);
        int pap = 5 + (i * 2);
        int cit = pap * 20;
        int year = 2020 + (i % 5);
        sci_research(inst, fld, fnd, pap, cit, year);
    }

    print_str("\nTech innovation...\n");
    for (int i = 0; i < 14; i++) {
        int cmp = 200 + (i * 12);
        int fld = (i % 5) + 1;
        int rd = 1000000 + (i * 500000);
        int pat = 3 + (i * 2);
        int rev = rd * 3;
        int year = 2021 + (i % 4);
        sci_innovation(cmp, fld, rd, pat, rev, year);
    }

    print_str("\nIntellectual property...\n");
    for (int i = 0; i < 12; i++) {
        int app = 300 + (i * 7);
        int type = (i % 3) + 1;
        int apl = 10 + (i * 5);
        int gnt = apl - (i % 4);
        int intl = i % 3;
        int year = 2022 + (i % 3);
        sci_ip(app, type, apl, gnt, intl, year);
    }

    print_str("\nScience parks...\n");
    for (int i = 0; i < 10; i++) {
        int rgn = (i % 8) + 1;
        int type = (i % 3) + 1;
        int cmp = 50 + (i * 20);
        int inc = 10 + (i * 5);
        int rev = 5000000 + (i * 2000000);
        int year = 2023 + (i % 2);
        sci_park(rgn, type, cmp, inc, rev, year);
    }

    print_str("\nTalent programs...\n");
    for (int i = 0; i < 10; i++) {
        int prs = 400 + (i * 13);
        int lvl = (i % 4) + 1;
        int fld = (i % 6) + 1;
        int ach = 5 + (i * 2);
        int fnd = 200000 + (i * 100000);
        int year = 2024;
        sci_talent(prs, lvl, fld, ach, fnd, year);
    }

    print_str("\nResearch report...\n");
    sci_research_report();

    print_str("\nInnovation report...\n");
    sci_innovation_report();

    print_str("\nTalent report...\n");
    sci_talent_report();

    print_str("\nFinal state...\n");
    sci_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
