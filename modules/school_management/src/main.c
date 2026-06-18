/* school_management: School management system (v1.0)
 * Classes, teachers, students, courses, grades, attendance
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

#define MAX_CLASSES    8
#define MAX_TEACHERS   10
#define MAX_STUDENTS   16
#define MAX_COURSES    10
#define MAX_GRADES     20
#define MAX_ATTENDANCE 18

typedef struct {
    int    class_id;
    int    grade;
    int    section;
    int    n_students;
    int    teacher_id;
    int    active;
} class_t;

typedef struct {
    int    teacher_id;
    int    subject;
    int    n_classes;
    int    n_hours;
    double salary;
    int    rating;
    int    active;
} teacher_t;

typedef struct {
    int    student_id;
    int    class_id;
    int    age;
    double gpa;
    int    n_courses;
    int    attendance_pct;
    double tuition_paid;
    int    active;
} student_t;

typedef struct {
    int    course_id;
    int    teacher_id;
    int    class_id;
    int    subject;
    int    hours_per_week;
    int    n_students;
    double fee;
    int    active;
} course_t;

typedef struct {
    int    grade_id;
    int    student_id;
    int    course_id;
    int    exam_type;
    double score;
    int    day;
    int    active;
} grade_t;

typedef struct {
    int    record_id;
    int    student_id;
    int    day;
    int    present;
    int    active;
} attendance_t;

typedef struct {
    int    n_classes;
    int    n_teachers;
    int    n_students;
    int    n_courses;
    int    n_grades;
    int    n_attendance;
    double tuition_revenue;
    double course_fees;
    double total_revenue;
    double total_salaries;
    int    total_present;
    int    total_records;
} sm_state_t;

static class_t classes[MAX_CLASSES];
static teacher_t teachers[MAX_TEACHERS];
static student_t students[MAX_STUDENTS];
static course_t courses[MAX_COURSES];
static grade_t grades[MAX_GRADES];
static attendance_t attendance[MAX_ATTENDANCE];
static sm_state_t sm;

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

int sm_init(void) {
    if (initialized) return -1;
    sm.n_classes = 0; sm.n_teachers = 0; sm.n_students = 0;
    sm.n_courses = 0; sm.n_grades = 0; sm.n_attendance = 0;
    sm.tuition_revenue = 0.0; sm.course_fees = 0.0;
    sm.total_revenue = 0.0; sm.total_salaries = 0.0;
    sm.total_present = 0; sm.total_records = 0;
    for (int i = 0; i < MAX_CLASSES; i++) classes[i].active = 0;
    for (int i = 0; i < MAX_TEACHERS; i++) teachers[i].active = 0;
    for (int i = 0; i < MAX_STUDENTS; i++) students[i].active = 0;
    for (int i = 0; i < MAX_COURSES; i++) courses[i].active = 0;
    for (int i = 0; i < MAX_GRADES; i++) grades[i].active = 0;
    for (int i = 0; i < MAX_ATTENDANCE; i++) attendance[i].active = 0;
    initialized = 1;
    print_str("[SM] School initialized\n");
    return 0;
}

int sm_add_class(int grade, int section) {
    if (sm.n_classes >= MAX_CLASSES) return -1;
    class_t* c = &classes[sm.n_classes];
    c->class_id = sm.n_classes;
    c->grade = grade;
    c->section = section;
    c->n_students = 0;
    c->teacher_id = -1;
    c->active = 1;
    sm.n_classes++;
    print_str("[SM] Class "); print_int(sm.n_classes - 1);
    print_str(" G"); print_int(grade);
    print_str(" S"); print_int(section); print_str("\n");
    return sm.n_classes - 1;
}

int sm_add_teacher(int subject, double salary, int rating) {
    if (sm.n_teachers >= MAX_TEACHERS) return -1;
    teacher_t* t = &teachers[sm.n_teachers];
    t->teacher_id = sm.n_teachers;
    t->subject = subject;
    t->n_classes = 0;
    t->n_hours = 0;
    t->salary = salary;
    t->rating = rating;
    t->active = 1;
    sm.n_teachers++;
    print_str("[SM] Teacher "); print_int(sm.n_teachers - 1);
    print_str(" subj="); print_int(subject);
    print_str(" salary="); print_int((int)salary);
    print_str(" rating="); print_int(rating); print_str("\n");
    return sm.n_teachers - 1;
}

int sm_register_student(int class_id, int age) {
    if (sm.n_students >= MAX_STUDENTS || class_id >= sm.n_classes) return -1;
    student_t* s = &students[sm.n_students];
    s->student_id = sm.n_students;
    s->class_id = class_id;
    s->age = age;
    s->gpa = 0.0;
    s->n_courses = 0;
    s->attendance_pct = 100;
    s->tuition_paid = 0.0;
    s->active = 1;
    classes[class_id].n_students++;
    sm.n_students++;
    print_str("[SM] Student "); print_int(sm.n_students - 1);
    print_str(" C"); print_int(class_id);
    print_str(" age="); print_int(age); print_str("\n");
    return sm.n_students - 1;
}

int sm_add_course(int teacher_id, int class_id, int subject, int hours, double fee) {
    if (sm.n_courses >= MAX_COURSES) return -1;
    if (teacher_id >= sm.n_teachers || class_id >= sm.n_classes) return -2;
    course_t* c = &courses[sm.n_courses];
    c->course_id = sm.n_courses;
    c->teacher_id = teacher_id;
    c->class_id = class_id;
    c->subject = subject;
    c->hours_per_week = hours;
    c->n_students = classes[class_id].n_students;
    c->fee = fee;
    c->active = 1;
    teachers[teacher_id].n_classes++;
    teachers[teacher_id].n_hours += hours;
    sm.n_courses++;
    print_str("[SM] Course "); print_int(sm.n_courses - 1);
    print_str(" T"); print_int(teacher_id);
    print_str(" C"); print_int(class_id);
    print_str(" subj="); print_int(subject);
    print_str(" hrs="); print_int(hours);
    print_str(" fee="); print_int((int)fee); print_str("\n");
    return sm.n_courses - 1;
}

int sm_pay_tuition(int student_id, double amount) {
    if (student_id >= sm.n_students) return -1;
    students[student_id].tuition_paid += amount;
    sm.tuition_revenue += amount;
    sm.total_revenue += amount;
    print_str("[SM] Tuition St"); print_int(student_id);
    print_str(" amount="); print_int((int)amount); print_str("\n");
    return 0;
}

int sm_record_grade(int student_id, int course_id, int exam_type, double score, int day) {
    if (sm.n_grades >= MAX_GRADES) return -1;
    if (student_id >= sm.n_students || course_id >= sm.n_courses) return -2;
    grade_t* g = &grades[sm.n_grades];
    g->grade_id = sm.n_grades;
    g->student_id = student_id;
    g->course_id = course_id;
    g->exam_type = exam_type;
    g->score = score;
    g->day = day;
    g->active = 1;
    student_t* s = &students[student_id];
    s->n_courses++;
    double total_gpa = s->gpa * (s->n_courses - 1) + score;
    s->gpa = total_gpa / s->n_courses;
    sm.course_fees += courses[course_id].fee;
    sm.total_revenue += courses[course_id].fee;
    sm.n_grades++;
    print_str("[SM] Grade "); print_int(sm.n_grades - 1);
    print_str(" St"); print_int(student_id);
    print_str(" Co"); print_int(course_id);
    print_str(" exam="); print_int(exam_type);
    print_str(" score="); print_int((int)score);
    print_str(" day="); print_int(day); print_str("\n");
    return sm.n_grades - 1;
}

int sm_record_attendance(int student_id, int day, int present) {
    if (sm.n_attendance >= MAX_ATTENDANCE || student_id >= sm.n_students) return -1;
    attendance_t* a = &attendance[sm.n_attendance];
    a->record_id = sm.n_attendance;
    a->student_id = student_id;
    a->day = day;
    a->present = present;
    a->active = 1;
    sm.total_records++;
    if (present) sm.total_present++;
    int total_days = sm.n_attendance / sm.n_students + 1;
    students[student_id].attendance_pct = (sm.total_present * 100) / (sm.n_students * total_days);
    sm.n_attendance++;
    print_str("[SM] Attendance St"); print_int(student_id);
    print_str(" day="); print_int(day);
    if (present) print_str(" PRESENT");
    else print_str(" ABSENT");
    print_str("\n");
    return 0;
}

void sm_teacher_report(void) {
    print_str("[SM] Teacher report:\n");
    for (int i = 0; i < sm.n_teachers; i++) {
        teacher_t* t = &teachers[i];
        print_str("  T"); print_int(i);
        print_str(" subj="); print_int(t->subject);
        print_str(" classes="); print_int(t->n_classes);
        print_str(" hrs="); print_int(t->n_hours);
        print_str(" salary="); print_int((int)t->salary);
        print_str(" rating="); print_int(t->rating); print_str("\n");
    }
}

void sm_class_report(void) {
    print_str("[SM] Class report:\n");
    for (int i = 0; i < sm.n_classes; i++) {
        class_t* c = &classes[i];
        print_str("  C"); print_int(i);
        print_str(" G"); print_int(c->grade);
        print_str(" S"); print_int(c->section);
        print_str(" students="); print_int(c->n_students); print_str("\n");
    }
}

void sm_financial_report(void) {
    print_str("[SM] Financial report:\n");
    print_str("  Tuition revenue: "); print_int((int)sm.tuition_revenue); print_str("\n");
    print_str("  Course fees: "); print_int((int)sm.course_fees); print_str("\n");
    print_str("  Total revenue: "); print_int((int)sm.total_revenue); print_str("\n");
    print_str("  Total salaries: "); print_int((int)sm.total_salaries); print_str("\n");
    double net = sm.total_revenue - sm.total_salaries;
    print_str("  Net: "); print_int((int)net); print_str("\n");
    print_str("  Students: "); print_int(sm.n_students); print_str("\n");
    print_str("  Attendance rate: ");
    int rate = (sm.total_records > 0) ? (sm.total_present * 100 / sm.total_records) : 0;
    print_int(rate); print_str("%\n");
}

void sm_print_state(void) {
    print_str("[SM] Classes="); print_int(sm.n_classes);
    print_str(" Teachers="); print_int(sm.n_teachers);
    print_str(" Students="); print_int(sm.n_students);
    print_str(" Courses="); print_int(sm.n_courses);
    print_str(" Grades="); print_int(sm.n_grades);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)sm.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== School Management Demo ===\n\n");
    sm_init();

    print_str("Adding classes...\n");
    sm_add_class(1, 1);
    sm_add_class(1, 2);
    sm_add_class(2, 1);
    sm_add_class(2, 2);
    sm_add_class(3, 1);
    sm_add_class(3, 2);
    sm_add_class(4, 1);
    sm_add_class(4, 2);

    print_str("\nAdding teachers...\n");
    sm_add_teacher(1, 4500.0, 90);
    sm_add_teacher(2, 4800.0, 88);
    sm_add_teacher(3, 5000.0, 92);
    sm_add_teacher(4, 4200.0, 85);
    sm_add_teacher(5, 5200.0, 94);
    sm_add_teacher(1, 4600.0, 87);
    sm_add_teacher(2, 4900.0, 91);
    sm_add_teacher(3, 5100.0, 89);
    sm_add_teacher(4, 4300.0, 86);
    sm_add_teacher(5, 5300.0, 93);

    print_str("\nRegistering students...\n");
    for (int i = 0; i < 16; i++) {
        sm_register_student(i % 8, 6 + (i % 4));
    }

    print_str("\nAdding courses...\n");
    sm_add_course(0, 0, 1, 5, 200.0);
    sm_add_course(1, 0, 2, 4, 180.0);
    sm_add_course(2, 1, 3, 5, 220.0);
    sm_add_course(3, 1, 4, 3, 150.0);
    sm_add_course(4, 2, 5, 5, 250.0);
    sm_add_course(5, 2, 1, 4, 190.0);
    sm_add_course(6, 3, 2, 5, 210.0);
    sm_add_course(7, 3, 3, 3, 160.0);
    sm_add_course(8, 4, 4, 4, 170.0);
    sm_add_course(9, 4, 5, 5, 240.0);

    print_str("\nPaying tuition...\n");
    for (int i = 0; i < 16; i++) {
        sm_pay_tuition(i, 1500.0);
    }

    print_str("\nRecording grades...\n");
    sm_record_grade(0, 0, 1, 85.0, 10);
    sm_record_grade(1, 0, 1, 92.0, 10);
    sm_record_grade(2, 1, 1, 78.0, 10);
    sm_record_grade(3, 1, 1, 88.0, 10);
    sm_record_grade(4, 2, 1, 95.0, 10);
    sm_record_grade(5, 2, 1, 72.0, 10);
    sm_record_grade(6, 3, 1, 89.0, 11);
    sm_record_grade(7, 3, 1, 91.0, 11);
    sm_record_grade(8, 4, 2, 82.0, 15);
    sm_record_grade(9, 4, 2, 76.0, 15);
    sm_record_grade(10, 5, 2, 94.0, 15);
    sm_record_grade(11, 5, 2, 87.0, 15);
    sm_record_grade(12, 6, 2, 90.0, 16);
    sm_record_grade(13, 6, 2, 83.0, 16);
    sm_record_grade(14, 7, 2, 79.0, 16);
    sm_record_grade(15, 7, 2, 96.0, 16);
    sm_record_grade(0, 8, 1, 88.0, 20);
    sm_record_grade(1, 9, 1, 74.0, 20);
    sm_record_grade(2, 0, 2, 91.0, 20);
    sm_record_grade(3, 1, 2, 86.0, 20);

    print_str("\nRecording attendance...\n");
    for (int d = 10; d <= 20; d++) {
        for (int s = 0; s < 16; s++) {
            if (sm.n_attendance >= MAX_ATTENDANCE) goto done_att;
            sm_record_attendance(s, d, (s + d) % 5 != 0);
        }
    }
done_att:

    print_str("\nTeacher report...\n");
    sm_teacher_report();

    print_str("\nClass report...\n");
    sm_class_report();

    print_str("\nFinancial report...\n");
    sm_financial_report();

    print_str("\nFinal state...\n");
    sm_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
