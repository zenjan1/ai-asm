/* school_district: School district management (v1.0)
 * Schools, teachers, students, curriculum, budget, performance
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

#define MAX_SCHOOLS    10
#define MAX_TEACHERS   20
#define MAX_STUDENTS   24
#define MAX_COURSES    16
#define MAX_BUDGET     8

typedef struct {
    int    school_id;
    int    level;
    int    n_teachers;
    int    n_students;
    int    n_classrooms;
    double budget;
    double avg_score;
    double graduation_rate;
    int    active;
} school_t;

typedef struct {
    int    teacher_id;
    int    school_id;
    int    subject;
    int    grade_level;
    int    n_classes;
    int    n_students;
    double salary;
    int    years_exp;
    int    rating;
    int    active;
} teacher_t;

typedef struct {
    int    student_id;
    int    school_id;
    int    grade;
    int    section;
    double avg_score;
    int    attendance_pct;
    int    n_absences;
    double gpa;
    int    active;
} student_t;

typedef struct {
    int    course_id;
    int    level;
    int    subject;
    int    credits;
    int    n_teachers;
    int    n_students;
    int    pass_rate;
    int    active;
} course_t;

typedef struct {
    int    budget_id;
    int    category;
    double allocated;
    double spent;
    double remaining;
    int    active;
} budget_t;

typedef struct {
    int    n_schools;
    int    n_teachers;
    int    n_students;
    int    n_courses;
    int    n_budgets;
    double total_budget;
    double personnel_costs;
    double facility_costs;
    double material_costs;
    double transport_costs;
    double total_spent;
    double state_funding;
    double local_funding;
    double federal_funding;
    double total_revenue;
    int    total_graduates;
    double district_avg_score;
} sd_state_t;

static school_t schools[MAX_SCHOOLS];
static teacher_t teachers[MAX_TEACHERS];
static student_t students[MAX_STUDENTS];
static course_t courses[MAX_COURSES];
static budget_t budgets[MAX_BUDGET];
static sd_state_t sd;

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

int sd_init(void) {
    if (initialized) return -1;
    sd.n_schools = 0; sd.n_teachers = 0; sd.n_students = 0;
    sd.n_courses = 0; sd.n_budgets = 0;
    sd.total_budget = 0.0; sd.personnel_costs = 0.0;
    sd.facility_costs = 0.0; sd.material_costs = 0.0;
    sd.transport_costs = 0.0; sd.total_spent = 0.0;
    sd.state_funding = 0.0; sd.local_funding = 0.0;
    sd.federal_funding = 0.0; sd.total_revenue = 0.0;
    sd.total_graduates = 0; sd.district_avg_score = 0.0;
    for (int i = 0; i < MAX_SCHOOLS; i++) schools[i].active = 0;
    for (int i = 0; i < MAX_TEACHERS; i++) teachers[i].active = 0;
    for (int i = 0; i < MAX_STUDENTS; i++) students[i].active = 0;
    for (int i = 0; i < MAX_COURSES; i++) courses[i].active = 0;
    for (int i = 0; i < MAX_BUDGET; i++) budgets[i].active = 0;
    initialized = 1;
    print_str("[SD] School district initialized\n");
    return 0;
}

int sd_add_school(int level, int n_teachers, int n_students, int n_rooms, double budget) {
    if (sd.n_schools >= MAX_SCHOOLS) return -1;
    school_t* s = &schools[sd.n_schools];
    s->school_id = sd.n_schools;
    s->level = level;
    s->n_teachers = n_teachers;
    s->n_students = n_students;
    s->n_classrooms = n_rooms;
    s->budget = budget;
    s->avg_score = 0.0;
    s->graduation_rate = 0.0;
    s->active = 1;
    sd.total_budget += budget;
    sd.n_schools++;
    print_str("[SD] School "); print_int(sd.n_schools - 1);
    print_str(" lvl="); print_int(level);
    print_str(" tch="); print_int(n_teachers);
    print_str(" stu="); print_int(n_students);
    print_str(" rooms="); print_int(n_rooms);
    print_str(" $"); print_int((int)budget); print_str("\n");
    return sd.n_schools - 1;
}

int sd_hire_teacher(int school_id, int subject, int grade, int n_classes, double salary, int years) {
    if (sd.n_teachers >= MAX_TEACHERS || school_id >= sd.n_schools) return -1;
    teacher_t* t = &teachers[sd.n_teachers];
    t->teacher_id = sd.n_teachers;
    t->school_id = school_id;
    t->subject = subject;
    t->grade_level = grade;
    t->n_classes = n_classes;
    t->n_students = 0;
    t->salary = salary;
    t->years_exp = years;
    t->rating = 0;
    t->active = 1;
    schools[school_id].n_teachers++;
    sd.personnel_costs += salary;
    sd.total_spent += salary;
    sd.n_teachers++;
    print_str("[SD] Teacher "); print_int(sd.n_teachers - 1);
    print_str(" Sch"); print_int(school_id);
    print_str(" subj="); print_int(subject);
    print_str(" cls="); print_int(n_classes);
    print_str(" $"); print_int((int)salary); print_str("\n");
    return sd.n_teachers - 1;
}

int sd_enroll_student(int school_id, int grade, int section, double score, int attendance) {
    if (sd.n_students >= MAX_STUDENTS || school_id >= sd.n_schools) return -1;
    student_t* s = &students[sd.n_students];
    s->student_id = sd.n_students;
    s->school_id = school_id;
    s->grade = grade;
    s->section = section;
    s->avg_score = score;
    s->attendance_pct = attendance;
    s->n_absences = (100 - attendance) * 180 / 100;
    s->gpa = 0.0;
    s->active = 1;
    schools[school_id].n_students++;
    sd.n_students++;
    print_str("[SD] Student "); print_int(sd.n_students - 1);
    print_str(" Sch"); print_int(school_id);
    print_str(" grd="); print_int(grade);
    print_str(" sec="); print_int(section);
    print_str(" score="); print_int((int)score); print_str("\n");
    return sd.n_students - 1;
}

int sd_add_course(int level, int subject, int credits) {
    if (sd.n_courses >= MAX_COURSES) return -1;
    course_t* c = &courses[sd.n_courses];
    c->course_id = sd.n_courses;
    c->level = level;
    c->subject = subject;
    c->credits = credits;
    c->n_teachers = 0;
    c->n_students = 0;
    c->pass_rate = 0;
    c->active = 1;
    sd.n_courses++;
    print_str("[SD] Course "); print_int(sd.n_courses - 1);
    print_str(" lvl="); print_int(level);
    print_str(" subj="); print_int(subject);
    print_str(" cr="); print_int(credits); print_str("\n");
    return sd.n_courses - 1;
}

int sd_set_budget(int category, double allocated) {
    if (sd.n_budgets >= MAX_BUDGET) return -1;
    budget_t* b = &budgets[sd.n_budgets];
    b->budget_id = sd.n_budgets;
    b->category = category;
    b->allocated = allocated;
    b->spent = 0.0;
    b->remaining = allocated;
    b->active = 1;
    sd.n_budgets++;
    print_str("[SD] Budget "); print_int(sd.n_budgets - 1);
    print_str(" cat="); print_int(category);
    print_str(" $"); print_int((int)allocated); print_str("\n");
    return sd.n_budgets - 1;
}

int sd_spend_budget(int budget_id, double amount) {
    if (budget_id >= sd.n_budgets) return -1;
    budget_t* b = &budgets[budget_id];
    if (b->remaining < amount) return -2;
    b->spent += amount;
    b->remaining -= amount;
    print_str("[SD] Spend B"); print_int(budget_id);
    print_str(" $"); print_int((int)amount); print_str("\n");
    return 0;
}

int sd_assign_teacher(int teacher_id, int course_id) {
    if (teacher_id >= sd.n_teachers || course_id >= sd.n_courses) return -1;
    courses[course_id].n_teachers++;
    teachers[teacher_id].n_students += 25;
    print_str("[SD] Assign T"); print_int(teacher_id);
    print_str(" to C"); print_int(course_id); print_str("\n");
    return 0;
}

int sd_record_score(int student_id, double score) {
    if (student_id >= sd.n_students) return -1;
    students[student_id].avg_score = score;
    students[student_id].gpa = score / 25.0;
    schools[students[student_id].school_id].avg_score += score;
    print_str("[SD] Score S"); print_int(student_id);
    print_str(" ="); print_int((int)score); print_str("\n");
    return 0;
}

int sd_record_attendance(int student_id, int days_present) {
    if (student_id >= sd.n_students) return -1;
    students[student_id].attendance_pct = days_present * 100 / 180;
    students[student_id].n_absences = 180 - days_present;
    print_str("[SD] Attend S"); print_int(student_id);
    print_str(" pct="); print_int(students[student_id].attendance_pct); print_str("%\n");
    return 0;
}

int sd_graduate_student(int student_id) {
    if (student_id >= sd.n_students) return -1;
    students[student_id].active = 0;
    sd.total_graduates++;
    print_str("[SD] Graduate S"); print_int(student_id); print_str("\n");
    return 0;
}

int sd_add_state_funding(double amount) {
    sd.state_funding += amount;
    sd.total_revenue += amount;
    print_str("[SD] State +$"); print_int((int)amount); print_str("\n");
    return 0;
}

int sd_add_local_funding(double amount) {
    sd.local_funding += amount;
    sd.total_revenue += amount;
    print_str("[SD] Local +$"); print_int((int)amount); print_str("\n");
    return 0;
}

int sd_add_federal_funding(double amount) {
    sd.federal_funding += amount;
    sd.total_revenue += amount;
    print_str("[SD] Federal +$"); print_int((int)amount); print_str("\n");
    return 0;
}

int sd_set_facility_cost(double amount) {
    sd.facility_costs += amount;
    sd.total_spent += amount;
    print_str("[SD] Facility +$"); print_int((int)amount); print_str("\n");
    return 0;
}

int sd_set_material_cost(double amount) {
    sd.material_costs += amount;
    sd.total_spent += amount;
    print_str("[SD] Material +$"); print_int((int)amount); print_str("\n");
    return 0;
}

int sd_set_transport_cost(double amount) {
    sd.transport_costs += amount;
    sd.total_spent += amount;
    print_str("[SD] Transport +$"); print_int((int)amount); print_str("\n");
    return 0;
}

void sd_school_report(void) {
    print_str("[SD] School report:\n");
    for (int i = 0; i < sd.n_schools; i++) {
        school_t* s = &schools[i];
        print_str("  Sch"); print_int(i);
        print_str(" lvl="); print_int(s->level);
        print_str(" tch="); print_int(s->n_teachers);
        print_str(" stu="); print_int(s->n_students);
        print_str(" avg="); print_int((int)s->avg_score); print_str("\n");
    }
}

void sd_financial_report(void) {
    print_str("[SD] Financial report:\n");
    print_str("  State funding: "); print_int((int)sd.state_funding); print_str("\n");
    print_str("  Local funding: "); print_int((int)sd.local_funding); print_str("\n");
    print_str("  Federal funding: "); print_int((int)sd.federal_funding); print_str("\n");
    print_str("  Total revenue: "); print_int((int)sd.total_revenue); print_str("\n");
    print_str("  Personnel: "); print_int((int)sd.personnel_costs); print_str("\n");
    print_str("  Facilities: "); print_int((int)sd.facility_costs); print_str("\n");
    print_str("  Materials: "); print_int((int)sd.material_costs); print_str("\n");
    print_str("  Transport: "); print_int((int)sd.transport_costs); print_str("\n");
    print_str("  Total spent: "); print_int((int)sd.total_spent); print_str("\n");
    double balance = sd.total_revenue - sd.total_spent;
    print_str("  Balance: "); print_int((int)balance); print_str("\n");
    print_str("  Graduates: "); print_int(sd.total_graduates); print_str("\n");
}

void sd_print_state(void) {
    print_str("[SD] Schools="); print_int(sd.n_schools);
    print_str(" Teachers="); print_int(sd.n_teachers);
    print_str(" Students="); print_int(sd.n_students);
    print_str(" Courses="); print_int(sd.n_courses);
    print_str(" Budgets="); print_int(sd.n_budgets);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)sd.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== School District Demo ===\n\n");
    sd_init();

    print_str("Adding schools...\n");
    sd_add_school(1, 3, 120, 8, 800000.0);
    sd_add_school(1, 3, 130, 9, 850000.0);
    sd_add_school(1, 3, 110, 8, 780000.0);
    sd_add_school(2, 4, 150, 10, 950000.0);
    sd_add_school(2, 4, 140, 10, 920000.0);
    sd_add_school(2, 4, 160, 11, 980000.0);
    sd_add_school(3, 5, 200, 15, 1200000.0);
    sd_add_school(3, 5, 180, 14, 1150000.0);
    sd_add_school(3, 5, 190, 15, 1180000.0);
    sd_add_school(4, 3, 100, 8, 700000.0);

    print_str("\nHiring teachers...\n");
    for (int i = 0; i < 20; i++) {
        int school = i % 10;
        int subject = (i % 6) + 1;
        int grade = (i % 4) + 1;
        int classes = 4 + (i % 3);
        double salary = 45000.0 + (i * 2000);
        int years = 1 + (i % 20);
        sd_hire_teacher(school, subject, grade, classes, salary, years);
    }

    print_str("\nEnrolling students...\n");
    for (int i = 0; i < 24; i++) {
        int school = i % 10;
        int grade = (i % 4) + 1;
        int section = (i % 3) + 1;
        double score = 60 + (i * 1.5);
        int attendance = 85 + (i % 15);
        sd_enroll_student(school, grade, section, score, attendance);
    }

    print_str("\nAdding courses...\n");
    sd_add_course(1, 1, 5);
    sd_add_course(1, 2, 5);
    sd_add_course(1, 3, 4);
    sd_add_course(2, 1, 5);
    sd_add_course(2, 2, 5);
    sd_add_course(2, 4, 4);
    sd_add_course(3, 1, 5);
    sd_add_course(3, 2, 5);
    sd_add_course(3, 5, 4);
    sd_add_course(3, 6, 3);
    sd_add_course(4, 1, 5);
    sd_add_course(4, 2, 5);
    sd_add_course(4, 7, 4);
    sd_add_course(4, 8, 4);
    sd_add_course(3, 3, 4);
    sd_add_course(3, 4, 4);

    print_str("\nSetting budgets...\n");
    sd_set_budget(1, 5000000.0);
    sd_set_budget(2, 2000000.0);
    sd_set_budget(3, 800000.0);
    sd_set_budget(4, 600000.0);
    sd_set_budget(5, 400000.0);
    sd_set_budget(6, 300000.0);
    sd_set_budget(7, 250000.0);
    sd_set_budget(8, 150000.0);

    print_str("\nAssigning teachers...\n");
    for (int i = 0; i < 16; i++) {
        sd_assign_teacher(i % 20, i);
    }

    print_str("\nRecording scores...\n");
    for (int i = 0; i < 24; i++) {
        sd_record_score(i, 65 + (i * 1.2));
    }

    print_str("\nRecording attendance...\n");
    for (int i = 0; i < 24; i++) {
        sd_record_attendance(i, 160 + (i % 20));
    }

    print_str("\nSpending budgets...\n");
    for (int i = 0; i < 8; i++) {
        sd_spend_budget(i, 100000.0 + (i * 50000));
    }

    print_str("\nGraduating students...\n");
    for (int i = 0; i < 8; i++) {
        sd_graduate_student(i);
    }

    print_str("\nFunding...\n");
    sd_add_state_funding(8000000.0);
    sd_add_local_funding(3000000.0);
    sd_add_federal_funding(1500000.0);
    sd_add_state_funding(2000000.0);
    sd_add_local_funding(800000.0);

    print_str("\nAdditional costs...\n");
    sd_set_facility_cost(1500000.0);
    sd_set_material_cost(500000.0);
    sd_set_transport_cost(400000.0);

    print_str("\nSchool report...\n");
    sd_school_report();

    print_str("\nFinancial report...\n");
    sd_financial_report();

    print_str("\nFinal state...\n");
    sd_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
