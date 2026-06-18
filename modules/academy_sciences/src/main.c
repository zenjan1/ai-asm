/* academy_sciences: Academy of Sciences administration system (v1.0)
 * Basic research, applied research, talent cultivation, research platforms, achievement transformation
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

#define MAX_BASIC        16
#define MAX_APPLIED      14
#define MAX_TALENT       12
#define MAX_PLATFORM     10
#define MAX_TRANSFORMATION 10

typedef struct {
    int    basic_id;
    int    discipline;
    int    research_type;
    int    projects_funded;
    int    papers_published;
    int    citations;
    int    year;
    int    active;
} basic_t;

typedef struct {
    int    applied_id;
    int    field;
    int    application_area;
    int    projects_funded;
    int    patents_granted;
    int    prototypes_developed;
    int    year;
    int    active;
} applied_t;

typedef struct {
    int    talent_id;
    int    program_type;
    int    academic_level;
    int    students_enrolled;
    int    graduates;
    int    postdocs;
    int    year;
    int    active;
} talent_t;

typedef struct {
    int    platform_id;
    int    platform_type;
    int    facility_class;
    int    laboratories;
    int    major_facilities;
    int    field_stations;
    int    year;
    int    active;
} platform_t;

typedef struct {
    int    transformation_id;
    int    transfer_type;
    int    industry_sector;
    int    technologies_transferred;
    int    spinoff_companies;
    int    revenue_generated;
    int    year;
    int    active;
} transformation_t;

typedef struct {
    int    n_basic;
    int    n_applied;
    int    n_talent;
    int    n_platform;
    int    n_transformation;
    int    total_projects;
    int    total_papers;
    int    total_patents;
    int    total_students;
    int    total_revenue;
} as_state_t;

static basic_t basics[MAX_BASIC];
static applied_t applieds[MAX_APPLIED];
static talent_t talents[MAX_TALENT];
static platform_t platforms[MAX_PLATFORM];
static transformation_t transformations[MAX_TRANSFORMATION];
static as_state_t as;

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

int as_init(void) {
    if (initialized) return -1;
    as.n_basic = 0; as.n_applied = 0; as.n_talent = 0;
    as.n_platform = 0; as.n_transformation = 0;
    as.total_projects = 0; as.total_papers = 0;
    as.total_patents = 0; as.total_students = 0;
    as.total_revenue = 0;
    for (int i = 0; i < MAX_BASIC; i++) basics[i].active = 0;
    for (int i = 0; i < MAX_APPLIED; i++) applieds[i].active = 0;
    for (int i = 0; i < MAX_TALENT; i++) talents[i].active = 0;
    for (int i = 0; i < MAX_PLATFORM; i++) platforms[i].active = 0;
    for (int i = 0; i < MAX_TRANSFORMATION; i++) transformations[i].active = 0;
    initialized = 1;
    print_str("[AS] Academy of Sciences initialized\n");
    return 0;
}

int as_basic(int discipline, int res_type, int projects, int papers, int citations, int year) {
    if (as.n_basic >= MAX_BASIC) return -1;
    basic_t* b = &basics[as.n_basic];
    b->basic_id = as.n_basic;
    b->discipline = discipline;
    b->research_type = res_type;
    b->projects_funded = projects;
    b->papers_published = papers;
    b->citations = citations;
    b->year = year;
    b->active = 1;
    as.total_projects += projects;
    as.total_papers += papers;
    as.n_basic++;
    print_str("[AS] Basic "); print_int(as.n_basic - 1);
    print_str(" disc="); print_int(discipline);
    print_str(" type="); print_int(res_type);
    print_str(" prj="); print_int(projects);
    print_str(" pap="); print_int(papers);
    print_str(" cit="); print_int(citations); print_str("\n");
    return as.n_basic - 1;
}

int as_applied(int field, int app_area, int projects, int patents, int prototypes, int year) {
    if (as.n_applied >= MAX_APPLIED) return -1;
    applied_t* a = &applieds[as.n_applied];
    a->applied_id = as.n_applied;
    a->field = field;
    a->application_area = app_area;
    a->projects_funded = projects;
    a->patents_granted = patents;
    a->prototypes_developed = prototypes;
    a->year = year;
    a->active = 1;
    as.total_projects += projects;
    as.total_patents += patents;
    as.n_applied++;
    print_str("[AS] Applied "); print_int(as.n_applied - 1);
    print_str(" fld="); print_int(field);
    print_str(" area="); print_int(app_area);
    print_str(" prj="); print_int(projects);
    print_str(" pat="); print_int(patents);
    print_str(" prt="); print_int(prototypes); print_str("\n");
    return as.n_applied - 1;
}

int as_talent(int prog_type, int level, int students, int graduates, int postdocs, int year) {
    if (as.n_talent >= MAX_TALENT) return -1;
    talent_t* t = &talents[as.n_talent];
    t->talent_id = as.n_talent;
    t->program_type = prog_type;
    t->academic_level = level;
    t->students_enrolled = students;
    t->graduates = graduates;
    t->postdocs = postdocs;
    t->year = year;
    t->active = 1;
    as.total_students += students;
    as.n_talent++;
    print_str("[AS] Talent "); print_int(as.n_talent - 1);
    print_str(" prog="); print_int(prog_type);
    print_str(" lvl="); print_int(level);
    print_str(" stu="); print_int(students);
    print_str(" grd="); print_int(graduates);
    print_str(" post="); print_int(postdocs); print_str("\n");
    return as.n_talent - 1;
}

int as_platform(int plat_type, int facility_class, int labs, int major_fac, int field_stn, int year) {
    if (as.n_platform >= MAX_PLATFORM) return -1;
    platform_t* p = &platforms[as.n_platform];
    p->platform_id = as.n_platform;
    p->platform_type = plat_type;
    p->facility_class = facility_class;
    p->laboratories = labs;
    p->major_facilities = major_fac;
    p->field_stations = field_stn;
    p->year = year;
    p->active = 1;
    as.n_platform++;
    print_str("[AS] Platform "); print_int(as.n_platform - 1);
    print_str(" type="); print_int(plat_type);
    print_str(" cls="); print_int(facility_class);
    print_str(" lab="); print_int(labs);
    print_str(" maj="); print_int(major_fac);
    print_str(" fld="); print_int(field_stn); print_str("\n");
    return as.n_platform - 1;
}

int as_transformation(int trans_type, int sector, int techs, int spinoffs, int revenue, int year) {
    if (as.n_transformation >= MAX_TRANSFORMATION) return -1;
    transformation_t* t = &transformations[as.n_transformation];
    t->transformation_id = as.n_transformation;
    t->transfer_type = trans_type;
    t->industry_sector = sector;
    t->technologies_transferred = techs;
    t->spinoff_companies = spinoffs;
    t->revenue_generated = revenue;
    t->year = year;
    t->active = 1;
    as.total_revenue += revenue;
    as.n_transformation++;
    print_str("[AS] Transformation "); print_int(as.n_transformation - 1);
    print_str(" type="); print_int(trans_type);
    print_str(" sec="); print_int(sector);
    print_str(" tch="); print_int(techs);
    print_str(" spin="); print_int(spinoffs);
    print_str(" rev=$"); print_int(revenue); print_str("\n");
    return as.n_transformation - 1;
}

void as_basic_report(void) {
    print_str("[AS] Basic research report:\n");
    print_str("  Disciplines: "); print_int(as.n_basic); print_str("\n");
    print_str("  Total projects: "); print_int(as.total_projects); print_str("\n");
    print_str("  Total papers: "); print_int(as.total_papers); print_str("\n");
}

void as_applied_report(void) {
    print_str("[AS] Applied research report:\n");
    print_str("  Research fields: "); print_int(as.n_applied); print_str("\n");
    print_str("  Total patents: "); print_int(as.total_patents); print_str("\n");
}

void as_talent_report(void) {
    print_str("[AS] Talent cultivation report:\n");
    print_str("  Training programs: "); print_int(as.n_talent); print_str("\n");
    print_str("  Total students: "); print_int(as.total_students); print_str("\n");
    print_str("  Research platforms: "); print_int(as.n_platform); print_str("\n");
    print_str("  Technology transfers: "); print_int(as.n_transformation); print_str("\n");
    print_str("  Total revenue: $"); print_int(as.total_revenue); print_str("\n");
}

void as_print_state(void) {
    print_str("[AS] Bs="); print_int(as.n_basic);
    print_str(" Ap="); print_int(as.n_applied);
    print_str(" Tl="); print_int(as.n_talent);
    print_str(" Pl="); print_int(as.n_platform);
    print_str(" Tr="); print_int(as.n_transformation);
    print_str("\n");
}

int main(void) {
    print_str("=== Academy of Sciences Demo ===\n\n");
    as_init();

    print_str("Basic research...\n");
    for (int i = 0; i < 16; i++) {
        int disc = (i % 8) + 1;
        int type = (i % 5) + 1;
        int prj = 20 + (i * 5);
        int pap = 100 + (i * 30);
        int cit = 500 + (i * 200);
        int year = 2020 + (i % 5);
        as_basic(disc, type, prj, pap, cit, year);
    }

    print_str("\nApplied research...\n");
    for (int i = 0; i < 14; i++) {
        int fld = (i % 7) + 1;
        int area = (i % 6) + 1;
        int prj = 15 + (i * 4);
        int pat = 10 + (i * 3);
        int prt = 5 + (i * 2);
        int year = 2021 + (i % 4);
        as_applied(fld, area, prj, pat, prt, year);
    }

    print_str("\nTalent cultivation...\n");
    for (int i = 0; i < 12; i++) {
        int prog = (i % 4) + 1;
        int lvl = (i % 3) + 1;
        int stu = 50 + (i * 15);
        int grd = stu - (i * 2);
        int post = 10 + (i * 3);
        int year = 2022 + (i % 3);
        as_talent(prog, lvl, stu, grd, post, year);
    }

    print_str("\nResearch platforms...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cls = (i % 5) + 1;
        int lab = 20 + (i * 5);
        int maj = 3 + (i % 4);
        int fld = 5 + (i * 2);
        int year = 2023 + (i % 2);
        as_platform(type, cls, lab, maj, fld, year);
    }

    print_str("\nAchievement transformation...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 5) + 1;
        int sec = (i % 6) + 1;
        int tch = 15 + (i * 4);
        int spin = 3 + (i % 5);
        int rev = 1000000 + (i * 500000);
        int year = 2024;
        as_transformation(type, sec, tch, spin, rev, year);
    }

    print_str("\nBasic research report...\n");
    as_basic_report();

    print_str("\nApplied research report...\n");
    as_applied_report();

    print_str("\nTalent report...\n");
    as_talent_report();

    print_str("\nFinal state...\n");
    as_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
