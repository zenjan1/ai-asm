/* education_management: Education management system (v1.0)
 * Student management, courses, grades, scheduling, campus resources
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

#define MAX_STUDENTS   20
#define MAX_COURSES    12
#define MAX_TEACHERS   10
#define MAX_ENROLLMENTS 30
#define MAX_GRADES     40
#define MAX_CLASSROOMS 10
#define MAX_RESOURCES  10

typedef struct {
    int    student_id;
    int    grade;
    int    class_num;
    int    enrollment_day;
    double gpa;
    int    credits_earned;
    int    active;
} student_t;

typedef struct {
    int    course_id;
    int    teacher_id;
    int    credits;
    int    max_students;
    int    enrolled;
    int    schedule_slot;
    double pass_rate;
    int    active;
} course_t;

typedef struct {
    int    teacher_id;
    int    department;
    int    title;
    int    courses_assigned;
    int    students_taught;
    double rating;
    int    active;
} teacher_t;

typedef struct {
    int    enrollment_id;
    int    student_id;
    int    course_id;
    int    semester;
    int    status;
    int    active;
} enrollment_t;

typedef struct {
    int    grade_id;
    int    enrollment_id;
    int    student_id;
    int    course_id;
    double score;
    int    semester;
    int    active;
} grade_t;

typedef struct {
    int    room_id;
    int    building;
    int    capacity;
    int    type;
    int    bookings;
    int    active;
} classroom_t;

typedef struct {
    int    resource_id;
    int    type;
    int    quantity;
    int    borrowed;
    double fine_rate;
    int    active;
} resource_t;

typedef struct {
    int    n_students;
    int    n_courses;
    int    n_teachers;
    int    n_enrollments;
    int    n_grades;
    int    n_classrooms;
    int    n_resources;
    double avg_gpa;
    int    total_credits;
    int    total_enrollments;
    int    total_bookings;
} edu_state_t;

static student_t students[MAX_STUDENTS];
static course_t courses[MAX_COURSES];
static teacher_t teachers[MAX_TEACHERS];
static enrollment_t enrollments[MAX_ENROLLMENTS];
static grade_t grades[MAX_GRADES];
static classroom_t classrooms[MAX_CLASSROOMS];
static resource_t resources[MAX_RESOURCES];
static edu_state_t edu;

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

int edu_init(void) {
    if (initialized) return -1;
    edu.n_students = 0; edu.n_courses = 0; edu.n_teachers = 0;
    edu.n_enrollments = 0; edu.n_grades = 0;
    edu.n_classrooms = 0; edu.n_resources = 0;
    edu.avg_gpa = 0.0; edu.total_credits = 0;
    edu.total_enrollments = 0; edu.total_bookings = 0;
    for (int i = 0; i < MAX_STUDENTS; i++) students[i].active = 0;
    for (int i = 0; i < MAX_COURSES; i++) courses[i].active = 0;
    for (int i = 0; i < MAX_TEACHERS; i++) teachers[i].active = 0;
    for (int i = 0; i < MAX_ENROLLMENTS; i++) enrollments[i].active = 0;
    for (int i = 0; i < MAX_GRADES; i++) grades[i].active = 0;
    for (int i = 0; i < MAX_CLASSROOMS; i++) classrooms[i].active = 0;
    for (int i = 0; i < MAX_RESOURCES; i++) resources[i].active = 0;
    initialized = 1;
    print_str("[EDU] Education system initialized\n");
    return 0;
}

int edu_add_student(int grade, int class_num, int day) {
    if (edu.n_students >= MAX_STUDENTS) return -1;
    student_t* s = &students[edu.n_students];
    s->student_id = edu.n_students;
    s->grade = grade;
    s->class_num = class_num;
    s->enrollment_day = day;
    s->gpa = 0.0;
    s->credits_earned = 0;
    s->active = 1;
    edu.n_students++;
    print_str("[EDU] Student "); print_int(edu.n_students - 1);
    print_str(" G"); print_int(grade); print_str("C"); print_int(class_num);
    print_str(" day="); print_int(day); print_str("\n");
    return edu.n_students - 1;
}

int edu_add_teacher(int department, int title) {
    if (edu.n_teachers >= MAX_TEACHERS) return -1;
    teacher_t* t = &teachers[edu.n_teachers];
    t->teacher_id = edu.n_teachers;
    t->department = department;
    t->title = title;
    t->courses_assigned = 0;
    t->students_taught = 0;
    t->rating = 80.0;
    t->active = 1;
    edu.n_teachers++;
    print_str("[EDU] Teacher "); print_int(edu.n_teachers - 1);
    print_str(" dept="); print_int(department);
    print_str(" title="); print_int(title); print_str("\n");
    return edu.n_teachers - 1;
}

int edu_add_course(int teacher_id, int credits, int max_students, int slot) {
    if (edu.n_courses >= MAX_COURSES) return -1;
    if (teacher_id >= edu.n_teachers) return -2;
    course_t* c = &courses[edu.n_courses];
    c->course_id = edu.n_courses;
    c->teacher_id = teacher_id;
    c->credits = credits;
    c->max_students = max_students;
    c->enrolled = 0;
    c->schedule_slot = slot;
    c->pass_rate = 0.0;
    c->active = 1;
    teachers[teacher_id].courses_assigned++;
    edu.n_courses++;
    print_str("[EDU] Course "); print_int(edu.n_courses - 1);
    print_str(" T"); print_int(teacher_id);
    print_str(" credits="); print_int(credits);
    print_str(" max="); print_int(max_students);
    print_str(" slot="); print_int(slot); print_str("\n");
    return edu.n_courses - 1;
}

int edu_enroll(int student_id, int course_id, int semester) {
    if (edu.n_enrollments >= MAX_ENROLLMENTS) return -1;
    if (student_id >= edu.n_students || course_id >= edu.n_courses) return -2;
    course_t* c = &courses[course_id];
    if (c->enrolled >= c->max_students) return -3;
    enrollment_t* e = &enrollments[edu.n_enrollments];
    e->enrollment_id = edu.n_enrollments;
    e->student_id = student_id;
    e->course_id = course_id;
    e->semester = semester;
    e->status = 1;
    e->active = 1;
    c->enrolled++;
    teachers[c->teacher_id].students_taught++;
    edu.n_enrollments++;
    edu.total_enrollments++;
    print_str("[EDU] Enroll E"); print_int(edu.n_enrollments - 1);
    print_str(" S"); print_int(student_id);
    print_str(" C"); print_int(course_id);
    print_str(" sem="); print_int(semester); print_str("\n");
    return edu.n_enrollments - 1;
}

int edu_record_grade(int enrollment_id, double score, int semester) {
    if (edu.n_grades >= MAX_GRADES || enrollment_id >= edu.n_enrollments) return -1;
    enrollment_t* e = &enrollments[enrollment_id];
    grade_t* g = &grades[edu.n_grades];
    g->grade_id = edu.n_grades;
    g->enrollment_id = enrollment_id;
    g->student_id = e->student_id;
    g->course_id = e->course_id;
    g->score = score;
    g->semester = semester;
    g->active = 1;
    edu.n_grades++;
    /* Update student GPA and credits */
    student_t* s = &students[e->student_id];
    course_t* c = &courses[e->course_id];
    double total_points = s->gpa * s->credits_earned;
    double grade_points = 0.0;
    if (score >= 90) grade_points = 4.0;
    else if (score >= 80) grade_points = 3.0;
    else if (score >= 70) grade_points = 2.0;
    else if (score >= 60) grade_points = 1.0;
    else grade_points = 0.0;
    int new_credits = s->credits_earned + (score >= 60 ? c->credits : 0);
    if (new_credits > 0) {
        s->gpa = (total_points + grade_points * c->credits) / new_credits;
    }
    s->credits_earned = new_credits;
    e->status = 2;
    print_str("[EDU] Grade G"); print_int(edu.n_grades - 1);
    print_str(" S"); print_int(e->student_id);
    print_str(" C"); print_int(e->course_id);
    print_str(" score="); print_int((int)score);
    print_str(" GPA="); print_int((int)(s->gpa * 10)); print_str("\n");
    return edu.n_grades - 1;
}

int edu_add_classroom(int building, int capacity, int type) {
    if (edu.n_classrooms >= MAX_CLASSROOMS) return -1;
    classroom_t* r = &classrooms[edu.n_classrooms];
    r->room_id = edu.n_classrooms;
    r->building = building;
    r->capacity = capacity;
    r->type = type;
    r->bookings = 0;
    r->active = 1;
    edu.n_classrooms++;
    print_str("[EDU] Classroom "); print_int(edu.n_classrooms - 1);
    print_str(" B"); print_int(building);
    print_str(" cap="); print_int(capacity);
    print_str(" type="); print_int(type); print_str("\n");
    return edu.n_classrooms - 1;
}

int edu_add_resource(int type, int quantity, double fine_rate) {
    if (edu.n_resources >= MAX_RESOURCES) return -1;
    resource_t* r = &resources[edu.n_resources];
    r->resource_id = edu.n_resources;
    r->type = type;
    r->quantity = quantity;
    r->borrowed = 0;
    r->fine_rate = fine_rate;
    r->active = 1;
    edu.n_resources++;
    print_str("[EDU] Resource "); print_int(edu.n_resources - 1);
    print_str(" type="); print_int(type);
    print_str(" qty="); print_int(quantity);
    print_str(" fine="); print_int((int)(fine_rate * 10)); print_str("\n");
    return edu.n_resources - 1;
}

int edu_book_classroom(int room_id) {
    if (room_id >= edu.n_classrooms) return -1;
    classrooms[room_id].bookings++;
    edu.total_bookings++;
    print_str("[EDU] Room "); print_int(room_id);
    print_str(" booked (total="); print_int(classrooms[room_id].bookings);
    print_str(")\n");
    return 0;
}

int edu_borrow_resource(int resource_id) {
    if (resource_id >= edu.n_resources) return -1;
    resource_t* r = &resources[resource_id];
    if (r->borrowed >= r->quantity) return -2;
    r->borrowed++;
    print_str("[EDU] Resource R"); print_int(resource_id);
    print_str(" borrowed ("); print_int(r->borrowed);
    print_str("/"); print_int(r->quantity); print_str(")\n");
    return 0;
}

void edu_compute_pass_rates(void) {
    for (int c = 0; c < edu.n_courses; c++) {
        int total = 0, passed = 0;
        for (int g = 0; g < edu.n_grades; g++) {
            if (grades[g].course_id == c) {
                total++;
                if (grades[g].score >= 60) passed++;
            }
        }
        courses[c].pass_rate = (total > 0) ? (double)passed / total * 100.0 : 0.0;
    }
}

void edu_update_teacher_ratings(void) {
    for (int t = 0; t < edu.n_teachers; t++) {
        double total_score = 0.0;
        int count = 0;
        for (int g = 0; g < edu.n_grades; g++) {
            int cid = grades[g].course_id;
            if (courses[cid].teacher_id == t) {
                total_score += grades[g].score;
                count++;
            }
        }
        if (count > 0) {
            teachers[t].rating = total_score / count;
        }
    }
}

void edu_academic_report(void) {
    print_str("[EDU] Academic report:\n");
    print_str("  Students: "); print_int(edu.n_students); print_str("\n");
    print_str("  Courses: "); print_int(edu.n_courses); print_str("\n");
    print_str("  Teachers: "); print_int(edu.n_teachers); print_str("\n");
    print_str("  Enrollments: "); print_int(edu.total_enrollments); print_str("\n");
    print_str("  Grades recorded: "); print_int(edu.n_grades); print_str("\n");
    /* Average GPA */
    double total_gpa = 0.0;
    for (int i = 0; i < edu.n_students; i++) {
        total_gpa += students[i].gpa;
    }
    double avg = (edu.n_students > 0) ? total_gpa / edu.n_students : 0.0;
    print_str("  Avg GPA: "); print_int((int)(avg * 10)); print_str("/40\n");
    print_str("  Classroom bookings: "); print_int(edu.total_bookings); print_str("\n");
}

void edu_print_state(void) {
    print_str("[EDU] Students="); print_int(edu.n_students);
    print_str(" Courses="); print_int(edu.n_courses);
    print_str(" Teachers="); print_int(edu.n_teachers);
    print_str(" Enrollments="); print_int(edu.n_enrollments);
    print_str(" Grades="); print_int(edu.n_grades);
    print_str("\n");
    print_str("  Classrooms: "); print_int(edu.n_classrooms);
    print_str(" Resources: "); print_int(edu.n_resources); print_str("\n");
    for (int i = 0; i < edu.n_students; i++) {
        print_str("  Student "); print_int(i);
        print_str(" GPA="); print_int((int)(students[i].gpa * 10));
        print_str(" credits="); print_int(students[i].credits_earned);
        print_str("\n");
    }
}

int main(void) {
    print_str("=== Education Management System Demo ===\n\n");
    edu_init();

    print_str("Adding teachers...\n");
    edu_add_teacher(1, 2);
    edu_add_teacher(1, 1);
    edu_add_teacher(2, 2);
    edu_add_teacher(2, 1);
    edu_add_teacher(3, 3);
    edu_add_teacher(3, 1);

    print_str("\nAdding courses...\n");
    edu_add_course(0, 4, 30, 1);
    edu_add_course(1, 3, 25, 2);
    edu_add_course(2, 4, 35, 3);
    edu_add_course(3, 3, 20, 4);
    edu_add_course(4, 2, 40, 1);
    edu_add_course(5, 3, 30, 2);
    edu_add_course(0, 2, 25, 3);

    print_str("\nAdding students...\n");
    edu_add_student(1, 1, 1);
    edu_add_student(1, 1, 1);
    edu_add_student(1, 2, 2);
    edu_add_student(2, 1, 1);
    edu_add_student(2, 1, 1);
    edu_add_student(2, 2, 2);
    edu_add_student(3, 1, 1);
    edu_add_student(3, 1, 1);
    edu_add_student(3, 2, 3);
    edu_add_student(4, 1, 1);
    edu_add_student(4, 2, 2);

    print_str("\nEnrolling students...\n");
    edu_enroll(0, 0, 1);
    edu_enroll(0, 1, 1);
    edu_enroll(1, 0, 1);
    edu_enroll(1, 2, 1);
    edu_enroll(2, 1, 1);
    edu_enroll(2, 3, 1);
    edu_enroll(3, 0, 1);
    edu_enroll(3, 4, 1);
    edu_enroll(4, 2, 1);
    edu_enroll(4, 5, 1);
    edu_enroll(5, 3, 1);
    edu_enroll(5, 6, 1);
    edu_enroll(6, 0, 1);
    edu_enroll(7, 1, 1);
    edu_enroll(8, 4, 1);
    edu_enroll(9, 5, 1);

    print_str("\nRecording grades...\n");
    edu_record_grade(0, 88.0, 1);
    edu_record_grade(1, 92.0, 1);
    edu_record_grade(2, 75.0, 1);
    edu_record_grade(3, 95.0, 1);
    edu_record_grade(4, 68.0, 1);
    edu_record_grade(5, 82.0, 1);
    edu_record_grade(6, 90.0, 1);
    edu_record_grade(7, 55.0, 1);
    edu_record_grade(8, 78.0, 1);
    edu_record_grade(9, 85.0, 1);
    edu_record_grade(10, 91.0, 1);
    edu_record_grade(11, 72.0, 1);
    edu_record_grade(12, 60.0, 1);
    edu_record_grade(13, 88.0, 1);
    edu_record_grade(14, 95.0, 1);
    edu_record_grade(15, 70.0, 1);

    print_str("\nAdding classrooms...\n");
    edu_add_classroom(1, 40, 1);
    edu_add_classroom(1, 30, 1);
    edu_add_classroom(2, 50, 2);
    edu_add_classroom(2, 25, 1);
    edu_add_classroom(3, 60, 3);

    print_str("\nAdding resources...\n");
    edu_add_resource(1, 100, 0.5);
    edu_add_resource(2, 50, 1.0);
    edu_add_resource(3, 20, 2.0);
    edu_add_resource(4, 30, 0.5);

    print_str("\nBooking classrooms...\n");
    edu_book_classroom(0);
    edu_book_classroom(1);
    edu_book_classroom(2);
    edu_book_classroom(0);
    edu_book_classroom(3);

    print_str("\nBorrowing resources...\n");
    edu_borrow_resource(0);
    edu_borrow_resource(0);
    edu_borrow_resource(1);
    edu_borrow_resource(2);
    edu_borrow_resource(3);

    print_str("\nComputing pass rates...\n");
    edu_compute_pass_rates();
    for (int i = 0; i < edu.n_courses; i++) {
        print_str("  Course "); print_int(i);
        print_str(" pass_rate="); print_int((int)courses[i].pass_rate);
        print_str("%\n");
    }

    print_str("\nUpdating teacher ratings...\n");
    edu_update_teacher_ratings();
    for (int i = 0; i < edu.n_teachers; i++) {
        print_str("  Teacher "); print_int(i);
        print_str(" rating="); print_int((int)teachers[i].rating);
        print_str("\n");
    }

    print_str("\nAcademic report...\n");
    edu_academic_report();

    print_str("\nFinal state...\n");
    edu_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
