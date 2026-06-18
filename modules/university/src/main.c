/* university: University management (v1.0)
 * Departments, faculty, courses, students, research, revenue
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

#define MAX_DEPTS     10
#define MAX_FACULTY   16
#define MAX_COURSES   20
#define MAX_STUDENTS  24
#define MAX_PROJECTS  12

typedef struct {
    int    dept_id;
    int    field;
    int    n_faculty;
    int    n_students;
    int    n_courses;
    double budget;
    double research_funding;
    int    active;
} dept_t;

typedef struct {
    int    fac_id;
    int    dept_id;
    int    rank;
    int    research_area;
    int    n_papers;
    int    n_students_supervised;
    double salary;
    int    n_courses;
    int    active;
} faculty_t;

typedef struct {
    int    course_id;
    int    dept_id;
    int    level;
    int    credits;
    int    n_students;
    int    fac_id;
    double tuition_per_credit;
    double revenue;
    int    active;
} course_t;

typedef struct {
    int    student_id;
    int    dept_id;
    int    level;
    int    year;
    double gpa;
    int    credits_completed;
    double tuition_paid;
    int    active;
} student_t;

typedef struct {
    int    proj_id;
    int    fac_id;
    int    field;
    double funding;
    int    duration_months;
    int    n_papers;
    int    n_patents;
    double revenue;
    int    active;
} project_t;

typedef struct {
    int    n_depts;
    int    n_faculty;
    int    n_courses;
    int    n_students;
    int    n_projects;
    double tuition_revenue;
    double research_revenue;
    double donation_revenue;
    double licensing_revenue;
    double total_revenue;
    double salary_costs;
    double ops_costs;
    double scholarship_costs;
    int    total_graduates;
} uni_state_t;

static dept_t depts[MAX_DEPTS];
static faculty_t faculty[MAX_FACULTY];
static course_t courses[MAX_COURSES];
static student_t students[MAX_STUDENTS];
static project_t projects[MAX_PROJECTS];
static uni_state_t uni;

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

int uni_init(void) {
    if (initialized) return -1;
    uni.n_depts = 0; uni.n_faculty = 0; uni.n_courses = 0;
    uni.n_students = 0; uni.n_projects = 0;
    uni.tuition_revenue = 0.0; uni.research_revenue = 0.0;
    uni.donation_revenue = 0.0; uni.licensing_revenue = 0.0;
    uni.total_revenue = 0.0; uni.salary_costs = 0.0;
    uni.ops_costs = 0.0; uni.scholarship_costs = 0.0;
    uni.total_graduates = 0;
    for (int i = 0; i < MAX_DEPTS; i++) depts[i].active = 0;
    for (int i = 0; i < MAX_FACULTY; i++) faculty[i].active = 0;
    for (int i = 0; i < MAX_COURSES; i++) courses[i].active = 0;
    for (int i = 0; i < MAX_STUDENTS; i++) students[i].active = 0;
    for (int i = 0; i < MAX_PROJECTS; i++) projects[i].active = 0;
    initialized = 1;
    print_str("[UNI] University initialized\n");
    return 0;
}

int uni_add_dept(int field, int n_fac, int n_stu, int n_crs, double budget, double funding) {
    if (uni.n_depts >= MAX_DEPTS) return -1;
    dept_t* d = &depts[uni.n_depts];
    d->dept_id = uni.n_depts;
    d->field = field;
    d->n_faculty = n_fac;
    d->n_students = n_stu;
    d->n_courses = n_crs;
    d->budget = budget;
    d->research_funding = funding;
    d->active = 1;
    uni.ops_costs += budget * 0.1;
    uni.n_depts++;
    print_str("[UNI] Dept "); print_int(uni.n_depts - 1);
    print_str(" field="); print_int(field);
    print_str(" fac="); print_int(n_fac);
    print_str(" stu="); print_int(n_stu);
    print_str(" crs="); print_int(n_crs); print_str("\n");
    return uni.n_depts - 1;
}

int uni_hire_faculty(int dept_id, int rank, int area, double salary) {
    if (uni.n_faculty >= MAX_FACULTY || dept_id >= uni.n_depts) return -1;
    faculty_t* f = &faculty[uni.n_faculty];
    f->fac_id = uni.n_faculty;
    f->dept_id = dept_id;
    f->rank = rank;
    f->research_area = area;
    f->n_papers = 0;
    f->n_students_supervised = 0;
    f->salary = salary;
    f->n_courses = 0;
    f->active = 1;
    depts[dept_id].n_faculty++;
    uni.salary_costs += salary;
    uni.n_faculty++;
    print_str("[UNI] Faculty "); print_int(uni.n_faculty - 1);
    print_str(" D"); print_int(dept_id);
    print_str(" rank="); print_int(rank);
    print_str(" area="); print_int(area);
    print_str(" $"); print_int((int)salary); print_str("\n");
    return uni.n_faculty - 1;
}

int uni_add_course(int dept_id, int level, int credits, int fac_id, double tuition) {
    if (uni.n_courses >= MAX_COURSES) return -1;
    if (dept_id >= uni.n_depts || fac_id >= uni.n_faculty) return -2;
    course_t* c = &courses[uni.n_courses];
    c->course_id = uni.n_courses;
    c->dept_id = dept_id;
    c->level = level;
    c->credits = credits;
    c->n_students = 0;
    c->fac_id = fac_id;
    c->tuition_per_credit = tuition;
    c->revenue = 0.0;
    c->active = 1;
    depts[dept_id].n_courses++;
    faculty[fac_id].n_courses++;
    uni.n_courses++;
    print_str("[UNI] Course "); print_int(uni.n_courses - 1);
    print_str(" D"); print_int(dept_id);
    print_str(" F"); print_int(fac_id);
    print_str(" lvl="); print_int(level);
    print_str(" cr="); print_int(credits);
    print_str(" $"); print_int((int)tuition); print_str("/cr\n");
    return uni.n_courses - 1;
}

int uni_enroll_student(int dept_id, int level, int year, double gpa, int credits) {
    if (uni.n_students >= MAX_STUDENTS || dept_id >= uni.n_depts) return -1;
    student_t* s = &students[uni.n_students];
    s->student_id = uni.n_students;
    s->dept_id = dept_id;
    s->level = level;
    s->year = year;
    s->gpa = gpa;
    s->credits_completed = credits;
    s->tuition_paid = 0.0;
    s->active = 1;
    depts[dept_id].n_students++;
    uni.n_students++;
    print_str("[UNI] Student "); print_int(uni.n_students - 1);
    print_str(" D"); print_int(dept_id);
    print_str(" lvl="); print_int(level);
    print_str(" yr="); print_int(year);
    print_str(" gpa="); print_int((int)(gpa * 100)); print_str("\n");
    return uni.n_students - 1;
}

int uni_pay_tuition(int student_id, int course_id, int credits) {
    if (student_id >= uni.n_students || course_id >= uni.n_courses) return -1;
    course_t* c = &courses[course_id];
    student_t* s = &students[student_id];
    double payment = credits * c->tuition_per_credit;
    s->tuition_paid += payment;
    s->credits_completed += credits;
    c->n_students++;
    c->revenue += payment;
    uni.tuition_revenue += payment;
    uni.total_revenue += payment;
    print_str("[UNI] Tuition S"); print_int(student_id);
    print_str(" C"); print_int(course_id);
    print_str(" cr="); print_int(credits);
    print_str(" $"); print_int((int)payment); print_str("\n");
    return 0;
}

int uni_start_project(int fac_id, int field, double funding, int months) {
    if (uni.n_projects >= MAX_PROJECTS || fac_id >= uni.n_faculty) return -1;
    project_t* p = &projects[uni.n_projects];
    p->proj_id = uni.n_projects;
    p->fac_id = fac_id;
    p->field = field;
    p->funding = funding;
    p->duration_months = months;
    p->n_papers = 0;
    p->n_patents = 0;
    p->revenue = 0.0;
    p->active = 1;
    uni.research_revenue += funding;
    uni.total_revenue += funding;
    uni.n_projects++;
    print_str("[UNI] Project "); print_int(uni.n_projects - 1);
    print_str(" F"); print_int(fac_id);
    print_str(" field="); print_int(field);
    print_str(" mo="); print_int(months);
    print_str(" $"); print_int((int)funding); print_str("\n");
    return uni.n_projects - 1;
}

int uni_publish_paper(int proj_id, int n_citations) {
    if (proj_id >= uni.n_projects) return -1;
    project_t* p = &projects[proj_id];
    p->n_papers++;
    faculty[p->fac_id].n_papers++;
    print_str("[UNI] Paper Pr"); print_int(proj_id);
    print_str(" cites="); print_int(n_citations); print_str("\n");
    return 0;
}

int uni_file_patent(int proj_id) {
    if (proj_id >= uni.n_projects) return -1;
    projects[proj_id].n_patents++;
    print_str("[UNI] Patent Pr"); print_int(proj_id); print_str("\n");
    return 0;
}

int uni_license_tech(int proj_id, double amount) {
    if (proj_id >= uni.n_projects) return -1;
    projects[proj_id].revenue += amount;
    uni.licensing_revenue += amount;
    uni.total_revenue += amount;
    print_str("[UNI] License Pr"); print_int(proj_id);
    print_str(" $"); print_int((int)amount); print_str("\n");
    return 0;
}

int uni_add_donation(double amount) {
    uni.donation_revenue += amount;
    uni.total_revenue += amount;
    print_str("[UNI] Donation +$"); print_int((int)amount); print_str("\n");
    return 0;
}

int uni_award_scholarship(int student_id, double amount) {
    if (student_id >= uni.n_students) return -1;
    uni.scholarship_costs += amount;
    print_str("[UNI] Scholarship S"); print_int(student_id);
    print_str(" $"); print_int((int)amount); print_str("\n");
    return 0;
}

int uni_graduate(int student_id) {
    if (student_id >= uni.n_students) return -1;
    students[student_id].active = 0;
    uni.total_graduates++;
    print_str("[UNI] Graduate S"); print_int(student_id); print_str("\n");
    return 0;
}

void uni_dept_report(void) {
    print_str("[UNI] Department report:\n");
    for (int i = 0; i < uni.n_depts; i++) {
        dept_t* d = &depts[i];
        print_str("  D"); print_int(i);
        print_str(" field="); print_int(d->field);
        print_str(" fac="); print_int(d->n_faculty);
        print_str(" stu="); print_int(d->n_students);
        print_str(" crs="); print_int(d->n_courses);
        print_str(" fund=$"); print_int((int)d->research_funding); print_str("\n");
    }
}

void ui_financial_report(void) {
    print_str("[UNI] Financial report:\n");
    print_str("  Tuition revenue: "); print_int((int)uni.tuition_revenue); print_str("\n");
    print_str("  Research revenue: "); print_int((int)uni.research_revenue); print_str("\n");
    print_str("  Donations: "); print_int((int)uni.donation_revenue); print_str("\n");
    print_str("  Licensing: "); print_int((int)uni.licensing_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)uni.total_revenue); print_str("\n");
    print_str("  Salaries: "); print_int((int)uni.salary_costs); print_str("\n");
    print_str("  Ops costs: "); print_int((int)uni.ops_costs); print_str("\n");
    print_str("  Scholarships: "); print_int((int)uni.scholarship_costs); print_str("\n");
    double total_costs = uni.salary_costs + uni.ops_costs + uni.scholarship_costs;
    double profit = uni.total_revenue - total_costs;
    print_str("  Net: "); print_int((int)profit); print_str("\n");
    print_str("  Graduates: "); print_int(uni.total_graduates); print_str("\n");
}

void uni_print_state(void) {
    print_str("[UNI] Depts="); print_int(uni.n_depts);
    print_str(" Faculty="); print_int(uni.n_faculty);
    print_str(" Courses="); print_int(uni.n_courses);
    print_str(" Students="); print_int(uni.n_students);
    print_str(" Projects="); print_int(uni.n_projects);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)uni.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== University Demo ===\n\n");
    uni_init();

    print_str("Adding departments...\n");
    uni_add_dept(1, 2, 200, 3, 2000000.0, 500000.0);
    uni_add_dept(2, 2, 180, 3, 2500000.0, 800000.0);
    uni_add_dept(3, 2, 250, 3, 3000000.0, 1200000.0);
    uni_add_dept(4, 2, 220, 3, 2200000.0, 600000.0);
    uni_add_dept(5, 1, 150, 2, 2800000.0, 900000.0);
    uni_add_dept(6, 1, 120, 2, 1800000.0, 400000.0);
    uni_add_dept(7, 1, 100, 2, 1500000.0, 300000.0);
    uni_add_dept(8, 2, 160, 2, 2000000.0, 700000.0);
    uni_add_dept(9, 1, 130, 2, 1700000.0, 450000.0);
    uni_add_dept(10, 1, 90, 2, 1400000.0, 250000.0);

    print_str("\nHiring faculty...\n");
    uni_hire_faculty(0, 3, 1, 120000.0);
    uni_hire_faculty(0, 2, 1, 90000.0);
    uni_hire_faculty(1, 3, 2, 130000.0);
    uni_hire_faculty(1, 2, 2, 95000.0);
    uni_hire_faculty(2, 3, 3, 140000.0);
    uni_hire_faculty(2, 2, 3, 100000.0);
    uni_hire_faculty(3, 3, 4, 125000.0);
    uni_hire_faculty(3, 2, 4, 92000.0);
    uni_hire_faculty(4, 3, 5, 150000.0);
    uni_hire_faculty(4, 1, 5, 80000.0);
    uni_hire_faculty(5, 2, 6, 88000.0);
    uni_hire_faculty(6, 2, 7, 85000.0);
    uni_hire_faculty(7, 3, 8, 135000.0);
    uni_hire_faculty(7, 2, 8, 98000.0);
    uni_hire_faculty(8, 2, 9, 90000.0);
    uni_hire_faculty(9, 1, 10, 75000.0);

    print_str("\nAdding courses...\n");
    for (int i = 0; i < 20; i++) {
        int dept = i % 10;
        int lvl = (i % 3) + 1;
        int cr = 3 + (i % 3);
        int fac = i % 16;
        double tu = 400.0 + (lvl * 100);
        uni_add_course(dept, lvl, cr, fac, tu);
    }

    print_str("\nEnrolling students...\n");
    for (int i = 0; i < 24; i++) {
        int dept = i % 10;
        int lvl = (i % 3) + 1;
        int yr = (i % 4) + 1;
        double gpa = 2.5 + (i % 15) * 0.1;
        int cr = yr * 30;
        uni_enroll_student(dept, lvl, yr, gpa, cr);
    }

    print_str("\nCollecting tuition...\n");
    for (int i = 0; i < 24; i++) {
        uni_pay_tuition(i, i % 20, 3 + (i % 3));
    }
    for (int i = 0; i < 16; i++) {
        uni_pay_tuition(i % 24, (i + 5) % 20, 3);
    }

    print_str("\nStarting research projects...\n");
    for (int i = 0; i < 12; i++) {
        int fac = i % 16;
        int field = (i % 5) + 1;
        double funding = 100000.0 + (i * 50000);
        int months = 12 + (i % 12);
        uni_start_project(fac, field, funding, months);
    }

    print_str("\nPublishing papers...\n");
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 3; j++) {
            uni_publish_paper(i, 10 + (i * 5) + j);
        }
    }

    print_str("\nFiling patents...\n");
    for (int i = 0; i < 8; i++) {
        uni_file_patent(i);
    }

    print_str("\nLicensing technology...\n");
    uni_license_tech(0, 250000.0);
    uni_license_tech(2, 180000.0);
    uni_license_tech(4, 320000.0);
    uni_license_tech(6, 150000.0);
    uni_license_tech(8, 280000.0);
    uni_license_tech(10, 200000.0);

    print_str("\nDonations...\n");
    uni_add_donation(500000.0);
    uni_add_donation(1000000.0);
    uni_add_donation(750000.0);
    uni_add_donation(300000.0);
    uni_add_donation(600000.0);

    print_str("\nScholarships...\n");
    for (int i = 0; i < 10; i++) {
        uni_award_scholarship(i, 5000.0 + (i * 1000));
    }

    print_str("\nGraduating...\n");
    for (int i = 0; i < 8; i++) {
        uni_graduate(i);
    }

    print_str("\nDepartment report...\n");
    uni_dept_report();

    print_str("\nFinancial report...\n");
    ui_financial_report();

    print_str("\nFinal state...\n");
    uni_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
