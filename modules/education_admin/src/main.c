/* education_admin: Education administration system (v1.0)
 * Basic education, higher education, vocational, teachers, exams
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
#define MAX_HIGHER       14
#define MAX_VOCATIONAL   12
#define MAX_TEACHER      10
#define MAX_EXAM         10

typedef struct {
    int    basic_id;
    int    school_id;
    int    level;
    int    region_id;
    int    students;
    int    teachers;
    int    pass_rate;
    int    year;
    int    active;
} basic_t;

typedef struct {
    int    higher_id;
    int    university_id;
    int    discipline;
    int    undergrads;
    int    postgrads;
    int    research_score;
    int    year;
    int    active;
} higher_t;

typedef struct {
    int    vocational_id;
    int    institute_id;
    int    trade_type;
    int    enrollees;
    int    certified;
    int    employment_rate;
    int    year;
    int    active;
} vocational_t;

typedef struct {
    int    teacher_id;
    int    region_id;
    int    qualification;
    int    experience_years;
    int    training_hours;
    int    evaluation_score;
    int    year;
    int    active;
} teacher_t;

typedef struct {
    int    exam_id;
    int    exam_type;
    int    region_id;
    int    candidates;
    int    pass_count;
    int    avg_score;
    int    year;
    int    active;
} exam_t;

typedef struct {
    int    n_basic;
    int    n_higher;
    int    n_vocational;
    int    n_teacher;
    int    n_exam;
    int    total_students;
    int    total_teachers;
    int    total_enrollees;
    int    total_candidates;
    int    total_pass;
} ea_state_t;

static basic_t basics[MAX_BASIC];
static higher_t highers[MAX_HIGHER];
static vocational_t vocationals[MAX_VOCATIONAL];
static teacher_t teachers_list[MAX_TEACHER];
static exam_t exams[MAX_EXAM];
static ea_state_t ea;

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

int ea_init(void) {
    if (initialized) return -1;
    ea.n_basic = 0; ea.n_higher = 0; ea.n_vocational = 0;
    ea.n_teacher = 0; ea.n_exam = 0;
    ea.total_students = 0; ea.total_teachers = 0;
    ea.total_enrollees = 0; ea.total_candidates = 0;
    ea.total_pass = 0;
    for (int i = 0; i < MAX_BASIC; i++) basics[i].active = 0;
    for (int i = 0; i < MAX_HIGHER; i++) highers[i].active = 0;
    for (int i = 0; i < MAX_VOCATIONAL; i++) vocationals[i].active = 0;
    for (int i = 0; i < MAX_TEACHER; i++) teachers_list[i].active = 0;
    for (int i = 0; i < MAX_EXAM; i++) exams[i].active = 0;
    initialized = 1;
    print_str("[EA] Education admin initialized\n");
    return 0;
}

int ea_basic(int school, int level, int region, int students, int teachers, int pass_rate, int year) {
    if (ea.n_basic >= MAX_BASIC) return -1;
    basic_t* b = &basics[ea.n_basic];
    b->basic_id = ea.n_basic;
    b->school_id = school;
    b->level = level;
    b->region_id = region;
    b->students = students;
    b->teachers = teachers;
    b->pass_rate = pass_rate;
    b->year = year;
    b->active = 1;
    ea.total_students += students;
    ea.total_teachers += teachers;
    ea.n_basic++;
    print_str("[EA] Basic "); print_int(ea.n_basic - 1);
    print_str(" sch="); print_int(school);
    print_str(" lvl="); print_int(level);
    print_str(" rgn="); print_int(region);
    print_str(" stu="); print_int(students);
    print_str(" tch="); print_int(teachers);
    print_str(" pass="); print_int(pass_rate); print_str("%\n");
    return ea.n_basic - 1;
}

int ea_higher(int university, int discipline, int undergrads, int postgrads, int research_score, int year) {
    if (ea.n_higher >= MAX_HIGHER) return -1;
    higher_t* h = &highers[ea.n_higher];
    h->higher_id = ea.n_higher;
    h->university_id = university;
    h->discipline = discipline;
    h->undergrads = undergrads;
    h->postgrads = postgrads;
    h->research_score = research_score;
    h->year = year;
    h->active = 1;
    ea.total_students += undergrads + postgrads;
    ea.n_higher++;
    print_str("[EA] Higher "); print_int(ea.n_higher - 1);
    print_str(" unv="); print_int(university);
    print_str(" dsp="); print_int(discipline);
    print_str(" ugr="); print_int(undergrads);
    print_str(" pgr="); print_int(postgrads);
    print_str(" rsc="); print_int(research_score); print_str("\n");
    return ea.n_higher - 1;
}

int ea_vocational(int institute, int trade_type, int enrollees, int certified, int employment_rate, int year) {
    if (ea.n_vocational >= MAX_VOCATIONAL) return -1;
    vocational_t* v = &vocationals[ea.n_vocational];
    v->vocational_id = ea.n_vocational;
    v->institute_id = institute;
    v->trade_type = trade_type;
    v->enrollees = enrollees;
    v->certified = certified;
    v->employment_rate = employment_rate;
    v->year = year;
    v->active = 1;
    ea.total_enrollees += enrollees;
    ea.n_vocational++;
    print_str("[EA] Vocational "); print_int(ea.n_vocational - 1);
    print_str(" inst="); print_int(institute);
    print_str(" trade="); print_int(trade_type);
    print_str(" enr="); print_int(enrollees);
    print_str(" cert="); print_int(certified);
    print_str(" emp="); print_int(employment_rate); print_str("%\n");
    return ea.n_vocational - 1;
}

int ea_teacher(int region, int qualification, int exp_years, int training_hours, int eval_score, int year) {
    if (ea.n_teacher >= MAX_TEACHER) return -1;
    teacher_t* t = &teachers_list[ea.n_teacher];
    t->teacher_id = ea.n_teacher;
    t->region_id = region;
    t->qualification = qualification;
    t->experience_years = exp_years;
    t->training_hours = training_hours;
    t->evaluation_score = eval_score;
    t->year = year;
    t->active = 1;
    ea.n_teacher++;
    print_str("[EA] Teacher "); print_int(ea.n_teacher - 1);
    print_str(" rgn="); print_int(region);
    print_str(" qual="); print_int(qualification);
    print_str(" exp="); print_int(exp_years); print_str("y");
    print_str(" trn="); print_int(training_hours); print_str("h");
    print_str(" evl="); print_int(eval_score); print_str("\n");
    return ea.n_teacher - 1;
}

int ea_exam(int exam_type, int region, int candidates, int pass_count, int avg_score, int year) {
    if (ea.n_exam >= MAX_EXAM) return -1;
    exam_t* e = &exams[ea.n_exam];
    e->exam_id = ea.n_exam;
    e->exam_type = exam_type;
    e->region_id = region;
    e->candidates = candidates;
    e->pass_count = pass_count;
    e->avg_score = avg_score;
    e->year = year;
    e->active = 1;
    ea.total_candidates += candidates;
    ea.total_pass += pass_count;
    ea.n_exam++;
    print_str("[EA] Exam "); print_int(ea.n_exam - 1);
    print_str(" type="); print_int(exam_type);
    print_str(" rgn="); print_int(region);
    print_str(" cnd="); print_int(candidates);
    print_str(" pass="); print_int(pass_count);
    print_str(" avg="); print_int(avg_score); print_str("\n");
    return ea.n_exam - 1;
}

void ea_basic_report(void) {
    print_str("[EA] Basic education report:\n");
    print_str("  Schools: "); print_int(ea.n_basic); print_str("\n");
    print_str("  Total students: "); print_int(ea.total_students); print_str("\n");
    print_str("  Total teachers: "); print_int(ea.total_teachers); print_str("\n");
}

void ea_higher_report(void) {
    print_str("[EA] Higher education report:\n");
    print_str("  Universities: "); print_int(ea.n_higher); print_str("\n");
    print_str("  Vocational institutes: "); print_int(ea.n_vocational); print_str("\n");
    print_str("  Total enrollees: "); print_int(ea.total_enrollees); print_str("\n");
}

void ea_exam_report(void) {
    print_str("[EA] Exam report:\n");
    print_str("  Exams administered: "); print_int(ea.n_exam); print_str("\n");
    print_str("  Total candidates: "); print_int(ea.total_candidates); print_str("\n");
    print_str("  Total passed: "); print_int(ea.total_pass); print_str("\n");
    print_str("  Teachers managed: "); print_int(ea.n_teacher); print_str("\n");
}

void ea_print_state(void) {
    print_str("[EA] Bs="); print_int(ea.n_basic);
    print_str(" Hg="); print_int(ea.n_higher);
    print_str(" Vc="); print_int(ea.n_vocational);
    print_str(" Tc="); print_int(ea.n_teacher);
    print_str(" Ex="); print_int(ea.n_exam);
    print_str("\n");
}

int main(void) {
    print_str("=== Education Admin Demo ===\n\n");
    ea_init();

    print_str("Basic education...\n");
    for (int i = 0; i < 16; i++) {
        int sch = 100 + (i * 10);
        int lvl = (i % 3) + 1;
        int rgn = (i % 8) + 1;
        int stu = 200 + (i * 50);
        int tch = 15 + (i * 3);
        int pass = 75 + (i % 20);
        int year = 2020 + (i % 5);
        ea_basic(sch, lvl, rgn, stu, tch, pass, year);
    }

    print_str("\nHigher education...\n");
    for (int i = 0; i < 14; i++) {
        int unv = 200 + (i * 15);
        int dsp = (i % 8) + 1;
        int ugr = 5000 + (i * 1000);
        int pgr = 1000 + (i * 200);
        int rsc = 60 + (i * 3);
        int year = 2021 + (i % 4);
        ea_higher(unv, dsp, ugr, pgr, rsc, year);
    }

    print_str("\nVocational education...\n");
    for (int i = 0; i < 12; i++) {
        int inst = 300 + (i * 12);
        int trade = (i % 6) + 1;
        int enr = 500 + (i * 100);
        int cert = enr - (i * 20);
        int emp = 70 + (i * 2);
        int year = 2022 + (i % 3);
        ea_vocational(inst, trade, enr, cert, emp, year);
    }

    print_str("\nTeacher management...\n");
    for (int i = 0; i < 10; i++) {
        int rgn = (i % 8) + 1;
        int qual = (i % 4) + 1;
        int exp = 3 + (i * 2);
        int trn = 40 + (i * 10);
        int evl = 70 + (i * 3);
        int year = 2023 + (i % 2);
        ea_teacher(rgn, qual, exp, trn, evl, year);
    }

    print_str("\nExaminations...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int rgn = (i % 8) + 1;
        int cnd = 5000 + (i * 2000);
        int pass = cnd - (i * 500);
        int avg = 400 + (i * 20);
        int year = 2024;
        ea_exam(type, rgn, cnd, pass, avg, year);
    }

    print_str("\nBasic education report...\n");
    ea_basic_report();

    print_str("\nHigher education report...\n");
    ea_higher_report();

    print_str("\nExam report...\n");
    ea_exam_report();

    print_str("\nFinal state...\n");
    ea_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
