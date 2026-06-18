/* flight_school: Flight school management (v1.0)
 * Students, instructors, aircraft, courses, flight logs, revenue
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

#define MAX_STUDENTS    14
#define MAX_INSTRUCTORS 8
#define MAX_AIRCRAFT    8
#define MAX_COURSES     10
#define MAX_FLIGHTS     18

typedef struct {
    int    student_id;
    int    license_level;
    int    instructor_id;
    int    total_hours;
    int    solo_hours;
    double total_paid;
    int    medical_valid;
    int    active;
} student_t;

typedef struct {
    int    instructor_id;
    int    rating;
    int    total_hours;
    int    n_students;
    double hourly_rate;
    double total_earned;
    int    status;
    int    active;
} instructor_t;

typedef struct {
    int    aircraft_id;
    int    type;
    int    total_hours;
    int    fuel_gallons;
    double fuel_cost;
    int    maint_status;
    double maint_cost;
    int    active;
} aircraft_t;

typedef struct {
    int    course_id;
    int    type;
    int    min_hours;
    double price;
    int    n_enrolled;
    double revenue;
    int    active;
} course_t;

typedef struct {
    int    flight_id;
    int    student_id;
    int    instructor_id;
    int    aircraft_id;
    int    duration_min;
    int    landings;
    int    solo;
    double cost;
    int    day;
    int    active;
} flight_t;

typedef struct {
    int    n_students;
    int    n_instructors;
    int    n_aircraft;
    int    n_courses;
    int    n_flights;
    double course_revenue;
    double flight_revenue;
    double exam_revenue;
    double total_revenue;
    double fuel_costs;
    double maint_costs;
    double instructor_costs;
    int    total_flight_hours;
} fs_state_t;

static student_t students[MAX_STUDENTS];
static instructor_t instructors[MAX_INSTRUCTORS];
static aircraft_t aircraft[MAX_AIRCRAFT];
static course_t courses[MAX_COURSES];
static flight_t flights[MAX_FLIGHTS];
static fs_state_t fss;

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

int fs_init(void) {
    if (initialized) return -1;
    fss.n_students = 0; fss.n_instructors = 0; fss.n_aircraft = 0;
    fss.n_courses = 0; fss.n_flights = 0;
    fss.course_revenue = 0.0; fss.flight_revenue = 0.0;
    fss.exam_revenue = 0.0; fss.total_revenue = 0.0;
    fss.fuel_costs = 0.0; fss.maint_costs = 0.0;
    fss.instructor_costs = 0.0; fss.total_flight_hours = 0;
    for (int i = 0; i < MAX_STUDENTS; i++) students[i].active = 0;
    for (int i = 0; i < MAX_INSTRUCTORS; i++) instructors[i].active = 0;
    for (int i = 0; i < MAX_AIRCRAFT; i++) aircraft[i].active = 0;
    for (int i = 0; i < MAX_COURSES; i++) courses[i].active = 0;
    for (int i = 0; i < MAX_FLIGHTS; i++) flights[i].active = 0;
    initialized = 1;
    print_str("[FS] Flight school initialized\n");
    return 0;
}

int fs_add_instructor(int rating, double rate, int status) {
    if (fss.n_instructors >= MAX_INSTRUCTORS) return -1;
    instructor_t* inst = &instructors[fss.n_instructors];
    inst->instructor_id = fss.n_instructors;
    inst->rating = rating;
    inst->total_hours = 0;
    inst->n_students = 0;
    inst->hourly_rate = rate;
    inst->total_earned = 0.0;
    inst->status = status;
    inst->active = 1;
    fss.n_instructors++;
    print_str("[FS] Instructor "); print_int(fss.n_instructors - 1);
    print_str(" rating="); print_int(rating);
    print_str(" rate="); print_int((int)rate); print_str("/hr\n");
    return fss.n_instructors - 1;
}

int fs_add_aircraft(int type, double maint_cost) {
    if (fss.n_aircraft >= MAX_AIRCRAFT) return -1;
    aircraft_t* a = &aircraft[fss.n_aircraft];
    a->aircraft_id = fss.n_aircraft;
    a->type = type;
    a->total_hours = 0;
    a->fuel_gallons = 0;
    a->fuel_cost = 0.0;
    a->maint_status = 1;
    a->maint_cost = maint_cost;
    a->active = 1;
    fss.n_aircraft++;
    print_str("[FS] Aircraft "); print_int(fss.n_aircraft - 1);
    print_str(" type="); print_int(type); print_str("\n");
    return fss.n_aircraft - 1;
}

int fs_add_course(int type, int min_hours, double price) {
    if (fss.n_courses >= MAX_COURSES) return -1;
    course_t* c = &courses[fss.n_courses];
    c->course_id = fss.n_courses;
    c->type = type;
    c->min_hours = min_hours;
    c->price = price;
    c->n_enrolled = 0;
    c->revenue = 0.0;
    c->active = 1;
    fss.n_courses++;
    print_str("[FS] Course "); print_int(fss.n_courses - 1);
    print_str(" type="); print_int(type);
    print_str(" hrs="); print_int(min_hours);
    print_str(" price="); print_int((int)price); print_str("\n");
    return fss.n_courses - 1;
}

int fs_enroll_student(int license_level, int instructor_id, int course_id) {
    if (fss.n_students >= MAX_STUDENTS) return -1;
    if (instructor_id >= fss.n_instructors || course_id >= fss.n_courses) return -2;
    student_t* s = &students[fss.n_students];
    s->student_id = fss.n_students;
    s->license_level = license_level;
    s->instructor_id = instructor_id;
    s->total_hours = 0;
    s->solo_hours = 0;
    s->total_paid = courses[course_id].price;
    s->medical_valid = 1;
    s->active = 1;
    instructors[instructor_id].n_students++;
    courses[course_id].n_enrolled++;
    courses[course_id].revenue += courses[course_id].price;
    fss.course_revenue += courses[course_id].price;
    fss.total_revenue += courses[course_id].price;
    fss.n_students++;
    print_str("[FS] Student "); print_int(fss.n_students - 1);
    print_str(" lvl="); print_int(license_level);
    print_str(" Inst"); print_int(instructor_id);
    print_str(" Co"); print_int(course_id); print_str("\n");
    return fss.n_students - 1;
}

int fs_log_flight(int student_id, int instructor_id, int aircraft_id, int duration, int landings, int solo, int day) {
    if (fss.n_flights >= MAX_FLIGHTS) return -1;
    if (student_id >= fss.n_students || aircraft_id >= fss.n_aircraft) return -2;
    flight_t* f = &flights[fss.n_flights];
    f->flight_id = fss.n_flights;
    f->student_id = student_id;
    f->instructor_id = instructor_id;
    f->aircraft_id = aircraft_id;
    f->duration_min = duration;
    f->landings = landings;
    f->solo = solo;
    double hours = duration / 60.0;
    double rate = solo ? 80.0 : 120.0;
    f->cost = rate * hours;
    f->day = day;
    f->active = 1;
    students[student_id].total_hours += duration;
    if (solo) students[student_id].solo_hours += duration;
    aircraft_t* a = &aircraft[aircraft_id];
    a->total_hours += duration;
    int fuel = duration / 5;
    a->fuel_gallons += fuel;
    double fuel_cost = fuel * 5.0;
    a->fuel_cost += fuel_cost;
    fss.fuel_costs += fuel_cost;
    if (instructor_id >= 0 && instructor_id < fss.n_instructors) {
        double inst_pay = instructors[instructor_id].hourly_rate * hours;
        instructors[instructor_id].total_hours += duration;
        instructors[instructor_id].total_earned += inst_pay;
        fss.instructor_costs += inst_pay;
    }
    fss.flight_revenue += f->cost;
    fss.total_revenue += f->cost;
    fss.total_flight_hours += duration / 60;
    fss.n_flights++;
    print_str("[FS] Flight "); print_int(fss.n_flights - 1);
    print_str(" St"); print_int(student_id);
    if (!solo) { print_str(" Inst"); print_int(instructor_id); }
    print_str(" Ac"); print_int(aircraft_id);
    print_str(" dur="); print_int(duration); print_str("min");
    print_str(" land="); print_int(landings);
    if (solo) print_str(" [SOLO]");
    print_str(" cost="); print_int((int)f->cost); print_str("\n");
    return fss.n_flights - 1;
}

int fs_charge_exam(int student_id, double fee) {
    if (student_id >= fss.n_students) return -1;
    students[student_id].total_paid += fee;
    fss.exam_revenue += fee;
    fss.total_revenue += fee;
    print_str("[FS] Exam St"); print_int(student_id);
    print_str(" fee="); print_int((int)fee); print_str("\n");
    return 0;
}

int fs_schedule_maint(int aircraft_id) {
    if (aircraft_id >= fss.n_aircraft) return -1;
    aircraft_t* a = &aircraft[aircraft_id];
    a->maint_status = 2;
    fss.maint_costs += a->maint_cost;
    print_str("[FS] Maint Ac"); print_int(aircraft_id);
    print_str(" cost="); print_int((int)a->maint_cost); print_str("\n");
    return 0;
}

void fs_complete_maint(int aircraft_id) {
    if (aircraft_id >= fss.n_aircraft) return;
    aircraft[aircraft_id].maint_status = 1;
    print_str("[FS] Ac"); print_int(aircraft_id); print_str(" maint done\n");
}

void fs_aircraft_report(void) {
    print_str("[FS] Aircraft report:\n");
    for (int i = 0; i < fss.n_aircraft; i++) {
        aircraft_t* a = &aircraft[i];
        print_str("  Ac"); print_int(i);
        print_str(" type="); print_int(a->type);
        print_str(" hrs="); print_int(a->total_hours);
        print_str(" fuel="); print_int(a->fuel_gallons); print_str("gal");
        print_str(" fuel$="); print_int((int)a->fuel_cost);
        print_str(" status="); print_int(a->maint_status); print_str("\n");
    }
}

void fs_financial_report(void) {
    print_str("[FS] Financial report:\n");
    print_str("  Course revenue: "); print_int((int)fss.course_revenue); print_str("\n");
    print_str("  Flight revenue: "); print_int((int)fss.flight_revenue); print_str("\n");
    print_str("  Exam revenue: "); print_int((int)fss.exam_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)fss.total_revenue); print_str("\n");
    print_str("  Fuel costs: "); print_int((int)fss.fuel_costs); print_str("\n");
    print_str("  Maint costs: "); print_int((int)fss.maint_costs); print_str("\n");
    print_str("  Instructor costs: "); print_int((int)fss.instructor_costs); print_str("\n");
    double total_costs = fss.fuel_costs + fss.maint_costs + fss.instructor_costs;
    double profit = fss.total_revenue - total_costs;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    print_str("  Total flight hours: "); print_int(fss.total_flight_hours); print_str("\n");
}

void fs_print_state(void) {
    print_str("[FS] Students="); print_int(fss.n_students);
    print_str(" Inst="); print_int(fss.n_instructors);
    print_str(" Ac="); print_int(fss.n_aircraft);
    print_str(" Co="); print_int(fss.n_courses);
    print_str(" Flights="); print_int(fss.n_flights);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)fss.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Flight School Demo ===\n\n");
    fs_init();

    print_str("Adding instructors...\n");
    fs_add_instructor(3, 80.0, 1);
    fs_add_instructor(2, 65.0, 1);
    fs_add_instructor(3, 85.0, 1);
    fs_add_instructor(1, 55.0, 1);
    fs_add_instructor(2, 70.0, 1);
    fs_add_instructor(3, 90.0, 1);
    fs_add_instructor(1, 50.0, 1);
    fs_add_instructor(2, 68.0, 1);

    print_str("\nAdding aircraft...\n");
    fs_add_aircraft(1, 1500.0);
    fs_add_aircraft(1, 1200.0);
    fs_add_aircraft(2, 2500.0);
    fs_add_aircraft(2, 2200.0);
    fs_add_aircraft(3, 3500.0);
    fs_add_aircraft(3, 3000.0);
    fs_add_aircraft(1, 1800.0);
    fs_add_aircraft(2, 2800.0);

    print_str("\nAdding courses...\n");
    fs_add_course(1, 40, 5000.0);
    fs_add_course(1, 35, 4500.0);
    fs_add_course(2, 50, 8000.0);
    fs_add_course(2, 45, 7500.0);
    fs_add_course(3, 100, 15000.0);
    fs_add_course(3, 120, 18000.0);
    fs_add_course(4, 200, 25000.0);
    fs_add_course(1, 30, 4000.0);
    fs_add_course(2, 55, 8500.0);
    fs_add_course(3, 110, 16000.0);

    print_str("\nEnrolling students...\n");
    fs_enroll_student(1, 0, 0);
    fs_enroll_student(1, 1, 1);
    fs_enroll_student(2, 2, 2);
    fs_enroll_student(2, 3, 3);
    fs_enroll_student(3, 4, 4);
    fs_enroll_student(3, 5, 5);
    fs_enroll_student(1, 6, 7);
    fs_enroll_student(2, 7, 8);
    fs_enroll_student(1, 0, 0);
    fs_enroll_student(2, 2, 2);
    fs_enroll_student(3, 4, 4);
    fs_enroll_student(1, 1, 1);
    fs_enroll_student(2, 3, 3);
    fs_enroll_student(3, 5, 5);

    print_str("\nLogging flights...\n");
    fs_log_flight(0, 0, 0, 60, 3, 0, 10);
    fs_log_flight(1, 1, 1, 45, 2, 0, 10);
    fs_log_flight(2, 2, 2, 90, 4, 0, 10);
    fs_log_flight(3, 3, 3, 75, 3, 0, 11);
    fs_log_flight(4, 4, 4, 120, 5, 0, 11);
    fs_log_flight(5, 5, 5, 100, 4, 0, 11);
    fs_log_flight(6, 6, 6, 50, 2, 0, 12);
    fs_log_flight(7, 7, 7, 80, 3, 0, 12);
    fs_log_flight(0, -1, 0, 40, 2, 1, 13);
    fs_log_flight(2, -1, 2, 60, 3, 1, 13);
    fs_log_flight(4, -1, 4, 90, 4, 1, 13);
    fs_log_flight(1, 1, 1, 55, 2, 0, 14);
    fs_log_flight(3, 3, 3, 65, 3, 0, 14);
    fs_log_flight(5, 5, 5, 85, 4, 0, 14);
    fs_log_flight(8, 0, 0, 50, 2, 0, 15);
    fs_log_flight(9, 2, 2, 70, 3, 0, 15);
    fs_log_flight(10, 4, 4, 100, 5, 0, 15);
    fs_log_flight(11, 1, 6, 45, 2, 0, 16);

    print_str("\nCharging exams...\n");
    fs_charge_exam(0, 500.0);
    fs_charge_exam(2, 800.0);
    fs_charge_exam(4, 1200.0);
    fs_charge_exam(6, 500.0);
    fs_charge_exam(8, 500.0);

    print_str("\nAircraft maintenance...\n");
    fs_schedule_maint(0);
    fs_schedule_maint(2);
    fs_schedule_maint(4);
    fs_complete_maint(0);
    fs_complete_maint(2);
    fs_complete_maint(4);

    print_str("\nAircraft report...\n");
    fs_aircraft_report();

    print_str("\nFinancial report...\n");
    fs_financial_report();

    print_str("\nFinal state...\n");
    fs_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
