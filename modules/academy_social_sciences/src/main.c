/* academy_social_sciences: Academy of Social Sciences administration system (v1.0)
 * Philosophy, economics, law, history, literature research
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

#define MAX_PHILOSOPHY   16
#define MAX_ECONOMICS    14
#define MAX_LAW          12
#define MAX_HISTORY      10
#define MAX_LITERATURE   10

typedef struct {
    int    philosophy_id;
    int    sub_discipline;
    int    research_focus;
    int    projects_funded;
    int    papers_published;
    int    books_published;
    int    year;
    int    active;
} philosophy_t;

typedef struct {
    int    economics_id;
    int    economic_field;
    int    research_type;
    int    projects_funded;
    int    policy_papers;
    int    international_collabs;
    int    year;
    int    active;
} economics_t;

typedef struct {
    int    law_id;
    int    legal_field;
    int    research_area;
    int    projects_funded;
    int    legal_papers;
    int    case_studies;
    int    year;
    int    active;
} law_t;

typedef struct {
    int    history_id;
    int    historical_period;
    int    research_type;
    int    projects_funded;
    int    archives_digitized;
    int    books_published;
    int    year;
    int    active;
} history_t;

typedef struct {
    int    literature_id;
    int    literary_field;
    int    research_focus;
    int    projects_funded;
    int    critical_papers;
    int    translations;
    int    year;
    int    active;
} literature_t;

typedef struct {
    int    n_philosophy;
    int    n_economics;
    int    n_law;
    int    n_history;
    int    n_literature;
    int    total_projects;
    int    total_papers;
    int    total_books;
    int    total_archives;
    int    total_collabs;
} ass_state_t;

static philosophy_t philosophies[MAX_PHILOSOPHY];
static economics_t economics_list[MAX_ECONOMICS];
static law_t laws[MAX_LAW];
static history_t histories[MAX_HISTORY];
static literature_t literatures[MAX_LITERATURE];
static ass_state_t ass;

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

int ass_init(void) {
    if (initialized) return -1;
    ass.n_philosophy = 0; ass.n_economics = 0; ass.n_law = 0;
    ass.n_history = 0; ass.n_literature = 0;
    ass.total_projects = 0; ass.total_papers = 0;
    ass.total_books = 0; ass.total_archives = 0;
    ass.total_collabs = 0;
    for (int i = 0; i < MAX_PHILOSOPHY; i++) philosophies[i].active = 0;
    for (int i = 0; i < MAX_ECONOMICS; i++) economics_list[i].active = 0;
    for (int i = 0; i < MAX_LAW; i++) laws[i].active = 0;
    for (int i = 0; i < MAX_HISTORY; i++) histories[i].active = 0;
    for (int i = 0; i < MAX_LITERATURE; i++) literatures[i].active = 0;
    initialized = 1;
    print_str("[ASS] Academy of Social Sciences initialized\n");
    return 0;
}

int ass_philosophy(int sub_disc, int focus, int projects, int papers, int books, int year) {
    if (ass.n_philosophy >= MAX_PHILOSOPHY) return -1;
    philosophy_t* p = &philosophies[ass.n_philosophy];
    p->philosophy_id = ass.n_philosophy;
    p->sub_discipline = sub_disc;
    p->research_focus = focus;
    p->projects_funded = projects;
    p->papers_published = papers;
    p->books_published = books;
    p->year = year;
    p->active = 1;
    ass.total_projects += projects;
    ass.total_papers += papers;
    ass.total_books += books;
    ass.n_philosophy++;
    print_str("[ASS] Philosophy "); print_int(ass.n_philosophy - 1);
    print_str(" sub="); print_int(sub_disc);
    print_str(" foc="); print_int(focus);
    print_str(" prj="); print_int(projects);
    print_str(" pap="); print_int(papers);
    print_str(" bks="); print_int(books); print_str("\n");
    return ass.n_philosophy - 1;
}

int ass_economics(int field, int res_type, int projects, int policy, int intl, int year) {
    if (ass.n_economics >= MAX_ECONOMICS) return -1;
    economics_t* e = &economics_list[ass.n_economics];
    e->economics_id = ass.n_economics;
    e->economic_field = field;
    e->research_type = res_type;
    e->projects_funded = projects;
    e->policy_papers = policy;
    e->international_collabs = intl;
    e->year = year;
    e->active = 1;
    ass.total_projects += projects;
    ass.total_papers += policy;
    ass.total_collabs += intl;
    ass.n_economics++;
    print_str("[ASS] Economics "); print_int(ass.n_economics - 1);
    print_str(" fld="); print_int(field);
    print_str(" type="); print_int(res_type);
    print_str(" prj="); print_int(projects);
    print_str(" pol="); print_int(policy);
    print_str(" int="); print_int(intl); print_str("\n");
    return ass.n_economics - 1;
}

int ass_law(int field, int area, int projects, int papers, int cases, int year) {
    if (ass.n_law >= MAX_LAW) return -1;
    law_t* l = &laws[ass.n_law];
    l->law_id = ass.n_law;
    l->legal_field = field;
    l->research_area = area;
    l->projects_funded = projects;
    l->legal_papers = papers;
    l->case_studies = cases;
    l->year = year;
    l->active = 1;
    ass.total_projects += projects;
    ass.total_papers += papers;
    ass.n_law++;
    print_str("[ASS] Law "); print_int(ass.n_law - 1);
    print_str(" fld="); print_int(field);
    print_str(" area="); print_int(area);
    print_str(" prj="); print_int(projects);
    print_str(" pap="); print_int(papers);
    print_str(" cas="); print_int(cases); print_str("\n");
    return ass.n_law - 1;
}

int ass_history(int period, int res_type, int projects, int archives, int books, int year) {
    if (ass.n_history >= MAX_HISTORY) return -1;
    history_t* h = &histories[ass.n_history];
    h->history_id = ass.n_history;
    h->historical_period = period;
    h->research_type = res_type;
    h->projects_funded = projects;
    h->archives_digitized = archives;
    h->books_published = books;
    h->year = year;
    h->active = 1;
    ass.total_projects += projects;
    ass.total_archives += archives;
    ass.total_books += books;
    ass.n_history++;
    print_str("[ASS] History "); print_int(ass.n_history - 1);
    print_str(" per="); print_int(period);
    print_str(" type="); print_int(res_type);
    print_str(" prj="); print_int(projects);
    print_str(" arc="); print_int(archives);
    print_str(" bks="); print_int(books); print_str("\n");
    return ass.n_history - 1;
}

int ass_literature(int field, int focus, int projects, int papers, int translations, int year) {
    if (ass.n_literature >= MAX_LITERATURE) return -1;
    literature_t* l = &literatures[ass.n_literature];
    l->literature_id = ass.n_literature;
    l->literary_field = field;
    l->research_focus = focus;
    l->projects_funded = projects;
    l->critical_papers = papers;
    l->translations = translations;
    l->year = year;
    l->active = 1;
    ass.total_projects += projects;
    ass.total_papers += papers;
    ass.n_literature++;
    print_str("[ASS] Literature "); print_int(ass.n_literature - 1);
    print_str(" fld="); print_int(field);
    print_str(" foc="); print_int(focus);
    print_str(" prj="); print_int(projects);
    print_str(" pap="); print_int(papers);
    print_str(" trn="); print_int(translations); print_str("\n");
    return ass.n_literature - 1;
}

void ass_philosophy_report(void) {
    print_str("[ASS] Philosophy report:\n");
    print_str("  Sub-disciplines: "); print_int(ass.n_philosophy); print_str("\n");
    print_str("  Total projects: "); print_int(ass.total_projects); print_str("\n");
    print_str("  Total papers: "); print_int(ass.total_papers); print_str("\n");
}

void ass_economics_report(void) {
    print_str("[ASS] Economics report:\n");
    print_str("  Research fields: "); print_int(ass.n_economics); print_str("\n");
    print_str("  International collabs: "); print_int(ass.total_collabs); print_str("\n");
}

void ass_law_report(void) {
    print_str("[ASS] Law report:\n");
    print_str("  Legal fields: "); print_int(ass.n_law); print_str("\n");
    print_str("  History periods: "); print_int(ass.n_history); print_str("\n");
    print_str("  Total archives digitized: "); print_int(ass.total_archives); print_str("\n");
    print_str("  Literature fields: "); print_int(ass.n_literature); print_str("\n");
    print_str("  Total books published: "); print_int(ass.total_books); print_str("\n");
}

void ass_print_state(void) {
    print_str("[ASS] Ph="); print_int(ass.n_philosophy);
    print_str(" Ec="); print_int(ass.n_economics);
    print_str(" Lw="); print_int(ass.n_law);
    print_str(" Hi="); print_int(ass.n_history);
    print_str(" Lt="); print_int(ass.n_literature);
    print_str("\n");
}

int main(void) {
    print_str("=== Academy of Social Sciences Demo ===\n\n");
    ass_init();

    print_str("Philosophy research...\n");
    for (int i = 0; i < 16; i++) {
        int sub = (i % 6) + 1;
        int foc = (i % 5) + 1;
        int prj = 8 + (i * 2);
        int pap = 20 + (i * 5);
        int bks = 2 + (i % 4);
        int year = 2020 + (i % 5);
        ass_philosophy(sub, foc, prj, pap, bks, year);
    }

    print_str("\nEconomics research...\n");
    for (int i = 0; i < 14; i++) {
        int fld = (i % 6) + 1;
        int type = (i % 4) + 1;
        int prj = 10 + (i * 3);
        int pol = 15 + (i * 4);
        int intl = 3 + (i % 5);
        int year = 2021 + (i % 4);
        ass_economics(fld, type, prj, pol, intl, year);
    }

    print_str("\nLaw research...\n");
    for (int i = 0; i < 12; i++) {
        int fld = (i % 5) + 1;
        int area = (i % 6) + 1;
        int prj = 6 + (i * 2);
        int pap = 12 + (i * 3);
        int cas = 5 + (i % 8);
        int year = 2022 + (i % 3);
        ass_law(fld, area, prj, pap, cas, year);
    }

    print_str("\nHistory research...\n");
    for (int i = 0; i < 10; i++) {
        int per = (i % 6) + 1;
        int type = (i % 4) + 1;
        int prj = 5 + (i * 2);
        int arc = 100 + (i * 50);
        int bks = 3 + (i % 4);
        int year = 2023 + (i % 2);
        ass_history(per, type, prj, arc, bks, year);
    }

    print_str("\nLiterature research...\n");
    for (int i = 0; i < 10; i++) {
        int fld = (i % 5) + 1;
        int foc = (i % 4) + 1;
        int prj = 6 + (i * 2);
        int pap = 15 + (i * 3);
        int trn = 2 + (i % 3);
        int year = 2024;
        ass_literature(fld, foc, prj, pap, trn, year);
    }

    print_str("\nPhilosophy report...\n");
    ass_philosophy_report();

    print_str("\nEconomics report...\n");
    ass_economics_report();

    print_str("\nLaw report...\n");
    ass_law_report();

    print_str("\nFinal state...\n");
    ass_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
