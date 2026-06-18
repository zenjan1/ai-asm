/* science_technology: Science and technology management (v1.0)
 * Research management, tech transfer, innovation platforms, talent, popularization
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
#define MAX_TRANSFER     14
#define MAX_PLATFORM     12
#define MAX_TALENT       10
#define MAX_POPULAR      10

typedef struct {
    int    research_id;
    int    project_type;
    int    lead_id;
    int    team_size;
    int    funding;
    int    papers;
    int    patents;
    int    year;
    int    status;
    int    active;
} research_t;

typedef struct {
    int    transfer_id;
    int    tech_id;
    int    buyer_id;
    int    seller_id;
    int    value;
    int    cooperation_type;
    int    year;
    int    status;
    int    active;
} transfer_t;

typedef struct {
    int    platform_id;
    int    type;
    int    region_id;
    int    researchers;
    int    projects;
    int    funding;
    int    achievements;
    int    year;
    int    active;
} platform_t;

typedef struct {
    int    talent_id;
    int    person_id;
    int    category;
    int    field;
    int    achievements;
    int    title;
    int    year;
    int    active;
} talent_t;

typedef struct {
    int    popular_id;
    int    base_id;
    int    activity_type;
    int    visitors;
    int    events;
    int    budget;
    int    year;
    int    active;
} popular_t;

typedef struct {
    int    n_research;
    int    n_transfer;
    int    n_platform;
    int    n_talent;
    int    n_popular;
    int    total_papers;
    int    total_patents;
    int    total_transfer_value;
    int    total_researchers;
    int    total_visitors;
} st_state_t;

static research_t research[MAX_RESEARCH];
static transfer_t transfers[MAX_TRANSFER];
static platform_t platforms[MAX_PLATFORM];
static talent_t talents[MAX_TALENT];
static popular_t popularization[MAX_POPULAR];
static st_state_t st;

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

int st_init(void) {
    if (initialized) return -1;
    st.n_research = 0; st.n_transfer = 0; st.n_platform = 0;
    st.n_talent = 0; st.n_popular = 0;
    st.total_papers = 0; st.total_patents = 0;
    st.total_transfer_value = 0; st.total_researchers = 0;
    st.total_visitors = 0;
    for (int i = 0; i < MAX_RESEARCH; i++) research[i].active = 0;
    for (int i = 0; i < MAX_TRANSFER; i++) transfers[i].active = 0;
    for (int i = 0; i < MAX_PLATFORM; i++) platforms[i].active = 0;
    for (int i = 0; i < MAX_TALENT; i++) talents[i].active = 0;
    for (int i = 0; i < MAX_POPULAR; i++) popularization[i].active = 0;
    initialized = 1;
    print_str("[ST] Science technology initialized\n");
    return 0;
}

int st_research(int type, int lead, int team, int funding, int papers, int patents, int year) {
    if (st.n_research >= MAX_RESEARCH) return -1;
    research_t* r = &research[st.n_research];
    r->research_id = st.n_research;
    r->project_type = type;
    r->lead_id = lead;
    r->team_size = team;
    r->funding = funding;
    r->papers = papers;
    r->patents = patents;
    r->year = year;
    r->status = 1;
    r->active = 1;
    st.total_papers += papers;
    st.total_patents += patents;
    st.n_research++;
    print_str("[ST] Research "); print_int(st.n_research - 1);
    print_str(" type="); print_int(type);
    print_str(" lead="); print_int(lead);
    print_str(" team="); print_int(team);
    print_str(" fnd=$"); print_int(funding);
    print_str(" pap="); print_int(papers);
    print_str(" pat="); print_int(patents); print_str("\n");
    return st.n_research - 1;
}

int st_transfer(int tech, int buyer, int seller, int value, int coop_type, int year) {
    if (st.n_transfer >= MAX_TRANSFER) return -1;
    transfer_t* t = &transfers[st.n_transfer];
    t->transfer_id = st.n_transfer;
    t->tech_id = tech;
    t->buyer_id = buyer;
    t->seller_id = seller;
    t->value = value;
    t->cooperation_type = coop_type;
    t->year = year;
    t->status = 1;
    t->active = 1;
    st.total_transfer_value += value;
    st.n_transfer++;
    print_str("[ST] Transfer "); print_int(st.n_transfer - 1);
    print_str(" tech="); print_int(tech);
    print_str(" buy="); print_int(buyer);
    print_str(" sel="); print_int(seller);
    print_str(" val=$"); print_int(value);
    print_str(" cop="); print_int(coop_type); print_str("\n");
    return st.n_transfer - 1;
}

int st_platform(int type, int region, int researchers, int projects, int funding, int achievements, int year) {
    if (st.n_platform >= MAX_PLATFORM) return -1;
    platform_t* p = &platforms[st.n_platform];
    p->platform_id = st.n_platform;
    p->type = type;
    p->region_id = region;
    p->researchers = researchers;
    p->projects = projects;
    p->funding = funding;
    p->achievements = achievements;
    p->year = year;
    p->active = 1;
    st.total_researchers += researchers;
    st.n_platform++;
    print_str("[ST] Platform "); print_int(st.n_platform - 1);
    print_str(" type="); print_int(type);
    print_str(" reg="); print_int(region);
    print_str(" rch="); print_int(researchers);
    print_str(" prj="); print_int(projects);
    print_str(" fnd=$"); print_int(funding);
    print_str(" ach="); print_int(achievements); print_str("\n");
    return st.n_platform - 1;
}

int st_talent(int person, int category, int field, int achievements, int title, int year) {
    if (st.n_talent >= MAX_TALENT) return -1;
    talent_t* t = &talents[st.n_talent];
    t->talent_id = st.n_talent;
    t->person_id = person;
    t->category = category;
    t->field = field;
    t->achievements = achievements;
    t->title = title;
    t->year = year;
    t->active = 1;
    st.n_talent++;
    print_str("[ST] Talent "); print_int(st.n_talent - 1);
    print_str(" per="); print_int(person);
    print_str(" cat="); print_int(category);
    print_str(" fld="); print_int(field);
    print_str(" ach="); print_int(achievements);
    print_str(" ttl="); print_int(title); print_str("\n");
    return st.n_talent - 1;
}

int st_popular(int base, int activity, int visitors, int events, int budget, int year) {
    if (st.n_popular >= MAX_POPULAR) return -1;
    popular_t* p = &popularization[st.n_popular];
    p->popular_id = st.n_popular;
    p->base_id = base;
    p->activity_type = activity;
    p->visitors = visitors;
    p->events = events;
    p->budget = budget;
    p->year = year;
    p->active = 1;
    st.total_visitors += visitors;
    st.n_popular++;
    print_str("[ST] Popular "); print_int(st.n_popular - 1);
    print_str(" bas="); print_int(base);
    print_str(" act="); print_int(activity);
    print_str(" vst="); print_int(visitors);
    print_str(" evt="); print_int(events);
    print_str(" bgt=$"); print_int(budget); print_str("\n");
    return st.n_popular - 1;
}

void st_research_report(void) {
    print_str("[ST] Research report:\n");
    print_str("  Projects: "); print_int(st.n_research); print_str("\n");
    print_str("  Total papers: "); print_int(st.total_papers); print_str("\n");
    print_str("  Total patents: "); print_int(st.total_patents); print_str("\n");
}

void st_transfer_report(void) {
    print_str("[ST] Transfer report:\n");
    print_str("  Transfers: "); print_int(st.n_transfer); print_str("\n");
    print_str("  Total value: $"); print_int(st.total_transfer_value); print_str("\n");
}

void st_platform_report(void) {
    print_str("[ST] Platform report:\n");
    print_str("  Platforms: "); print_int(st.n_platform); print_str("\n");
    print_str("  Total researchers: "); print_int(st.total_researchers); print_str("\n");
    print_str("  Talents: "); print_int(st.n_talent); print_str("\n");
    print_str("  Science popularization: "); print_int(st.n_popular); print_str("\n");
    print_str("  Total visitors: "); print_int(st.total_visitors); print_str("\n");
}

void st_print_state(void) {
    print_str("[ST] Rs="); print_int(st.n_research);
    print_str(" Tr="); print_int(st.n_transfer);
    print_str(" Pl="); print_int(st.n_platform);
    print_str(" Tl="); print_int(st.n_talent);
    print_str(" Pp="); print_int(st.n_popular);
    print_str("\n");
}

int main(void) {
    print_str("=== Science Technology Demo ===\n\n");
    st_init();

    print_str("Research projects...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int lead = 1000 + (i * 13);
        int team = 3 + (i % 8);
        int fnd = 100000 + (i * 50000);
        int pap = 1 + (i % 6);
        int pat = i % 4;
        int year = 2020 + (i % 5);
        st_research(type, lead, team, fnd, pap, pat, year);
    }

    print_str("\nTechnology transfer...\n");
    for (int i = 0; i < 14; i++) {
        int tech = 2000 + (i * 7);
        int buy = 3000 + (i * 11);
        int sel = 4000 + (i * 13);
        int val = 50000 + (i * 30000);
        int cop = (i % 3) + 1;
        int year = 2021 + (i % 4);
        st_transfer(tech, buy, sel, val, cop, year);
    }

    print_str("\nInnovation platforms...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int reg = (i % 6) + 1;
        int rch = 10 + (i * 5);
        int prj = 3 + (i % 8);
        int fnd = 200000 + (i * 100000);
        int ach = 2 + (i % 5);
        int year = 2022 + (i % 3);
        st_platform(type, reg, rch, prj, fnd, ach, year);
    }

    print_str("\nScience talents...\n");
    for (int i = 0; i < 10; i++) {
        int per = 5000 + (i * 17);
        int cat = (i % 4) + 1;
        int fld = (i % 6) + 1;
        int ach = 5 + (i * 3);
        int ttl = (i % 5) + 1;
        int year = 2023 + (i % 2);
        st_talent(per, cat, fld, ach, ttl, year);
    }

    print_str("\nScience popularization...\n");
    for (int i = 0; i < 10; i++) {
        int bas = 600 + (i * 20);
        int act = (i % 4) + 1;
        int vst = 100 + (i * 50);
        int evt = 2 + (i % 5);
        int bgt = 20000 + (i * 10000);
        int year = 2024;
        st_popular(bas, act, vst, evt, bgt, year);
    }

    print_str("\nResearch report...\n");
    st_research_report();

    print_str("\nTransfer report...\n");
    st_transfer_report();

    print_str("\nPlatform report...\n");
    st_platform_report();

    print_str("\nFinal state...\n");
    st_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
