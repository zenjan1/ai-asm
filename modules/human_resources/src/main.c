/* human_resources: Human resources management system (v1.0)
 * Employee management, attendance, payroll, recruitment, training
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

#define MAX_EMPLOYEES   16
#define MAX_DEPARTMENTS 8
#define MAX_POSITIONS   12
#define MAX_ATTENDANCE  24
#define MAX_PAYROLL     16
#define MAX_CANDIDATES  8
#define MAX_TRAINING    8

typedef struct {
    int    employee_id;
    int    department_id;
    int    position_id;
    int    age;
    int    tenure_months;
    double base_salary;
    double bonus;
    int    performance;
    int    status;
    int    active;
} employee_t;

typedef struct {
    int    dept_id;
    int    n_employees;
    int    manager_id;
    double budget;
    int    active;
} department_t;

typedef struct {
    int    position_id;
    int    level;
    int    n_filled;
    int    n_open;
    double salary_range_min;
    double salary_range_max;
    int    active;
} position_t;

typedef struct {
    int    record_id;
    int    employee_id;
    int    day;
    int    check_in;
    int    check_out;
    int    hours_worked;
    int    overtime;
    int    status;
    int    active;
} attendance_t;

typedef struct {
    int    payroll_id;
    int    employee_id;
    double base;
    double overtime_pay;
    double bonus;
    double deductions;
    double net_pay;
    int    active;
} payroll_t;

typedef struct {
    int    candidate_id;
    int    position_id;
    int    stage;
    int    score;
    int    hired;
    int    active;
} candidate_t;

typedef struct {
    int    training_id;
    int    type;
    int    n_participants;
    int    hours;
    double cost;
    double effectiveness;
    int    active;
} training_t;

typedef struct {
    int    n_employees;
    int    n_departments;
    int    n_positions;
    int    n_attendance;
    int    n_payroll;
    int    n_candidates;
    int    n_training;
    double total_payroll;
    double total_recruitment_cost;
    double total_training_cost;
    int    headcount;
    int    open_positions;
} hr_state_t;

static employee_t employees[MAX_EMPLOYEES];
static department_t departments[MAX_DEPARTMENTS];
static position_t positions[MAX_POSITIONS];
static attendance_t attendance[MAX_ATTENDANCE];
static payroll_t payroll[MAX_PAYROLL];
static candidate_t candidates[MAX_CANDIDATES];
static training_t training_sessions[MAX_TRAINING];
static hr_state_t hr;

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
static double dabs(double x) { return x < 0 ? -x : x; }

int hr_init(void) {
    if (initialized) return -1;
    hr.n_employees = 0; hr.n_departments = 0; hr.n_positions = 0;
    hr.n_attendance = 0; hr.n_payroll = 0; hr.n_candidates = 0;
    hr.n_training = 0;
    hr.total_payroll = 0.0; hr.total_recruitment_cost = 0.0;
    hr.total_training_cost = 0.0; hr.headcount = 0; hr.open_positions = 0;
    for (int i = 0; i < MAX_EMPLOYEES; i++) employees[i].active = 0;
    for (int i = 0; i < MAX_DEPARTMENTS; i++) departments[i].active = 0;
    for (int i = 0; i < MAX_POSITIONS; i++) positions[i].active = 0;
    for (int i = 0; i < MAX_ATTENDANCE; i++) attendance[i].active = 0;
    for (int i = 0; i < MAX_PAYROLL; i++) payroll[i].active = 0;
    for (int i = 0; i < MAX_CANDIDATES; i++) candidates[i].active = 0;
    for (int i = 0; i < MAX_TRAINING; i++) training_sessions[i].active = 0;
    initialized = 1;
    print_str("[HR] Human resources initialized\n");
    return 0;
}

int hr_add_department(double budget) {
    if (hr.n_departments >= MAX_DEPARTMENTS) return -1;
    department_t* d = &departments[hr.n_departments];
    d->dept_id = hr.n_departments;
    d->n_employees = 0;
    d->manager_id = -1;
    d->budget = budget;
    d->active = 1;
    hr.n_departments++;
    print_str("[HR] Department "); print_int(hr.n_departments - 1);
    print_str(" budget="); print_int((int)budget); print_str("\n");
    return hr.n_departments - 1;
}

int hr_add_position(int level, double salary_min, double salary_max) {
    if (hr.n_positions >= MAX_POSITIONS) return -1;
    position_t* p = &positions[hr.n_positions];
    p->position_id = hr.n_positions;
    p->level = level;
    p->n_filled = 0;
    p->n_open = 1;
    p->salary_range_min = salary_min;
    p->salary_range_max = salary_max;
    p->active = 1;
    hr.n_positions++;
    hr.open_positions++;
    print_str("[HR] Position "); print_int(hr.n_positions - 1);
    print_str(" L"); print_int(level);
    print_str(" range="); print_int((int)salary_min); print_str("-"); print_int((int)salary_max); print_str("\n");
    return hr.n_positions - 1;
}

int hr_hire_employee(int dept_id, int pos_id, int age, int tenure, double salary) {
    if (hr.n_employees >= MAX_EMPLOYEES) return -1;
    if (dept_id >= hr.n_departments || pos_id >= hr.n_positions) return -1;
    employee_t* e = &employees[hr.n_employees];
    e->employee_id = hr.n_employees;
    e->department_id = dept_id;
    e->position_id = pos_id;
    e->age = age;
    e->tenure_months = tenure;
    e->base_salary = salary;
    e->bonus = 0.0;
    e->performance = 70 + (hr.n_employees * 3) % 25;
    e->status = 1;
    e->active = 1;
    hr.n_employees++;
    hr.headcount++;
    departments[dept_id].n_employees++;
    positions[pos_id].n_filled++;
    positions[pos_id].n_open--;
    if (positions[pos_id].n_open <= 0) hr.open_positions--;
    print_str("[HR] Hired E"); print_int(hr.n_employees - 1);
    print_str(" D"); print_int(dept_id); print_str(" P"); print_int(pos_id);
    print_str(" age="); print_int(age);
    print_str(" tenure="); print_int(tenure); print_str("m");
    print_str(" salary="); print_int((int)salary); print_str("\n");
    return hr.n_employees - 1;
}

int hr_record_attendance(int emp_id, int day, int check_in, int check_out) {
    if (hr.n_attendance >= MAX_ATTENDANCE || emp_id >= hr.n_employees) return -1;
    attendance_t* a = &attendance[hr.n_attendance];
    a->record_id = hr.n_attendance;
    a->employee_id = emp_id;
    a->day = day;
    a->check_in = check_in;
    a->check_out = check_out;
    a->hours_worked = check_out - check_in;
    a->overtime = (a->hours_worked > 8) ? a->hours_worked - 8 : 0;
    a->status = (a->hours_worked >= 8) ? 1 : 0;
    a->active = 1;
    hr.n_attendance++;
    print_str("[HR] Attendance E"); print_int(emp_id);
    print_str(" day="); print_int(day);
    print_str(" "); print_int(check_in); print_str("-"); print_int(check_out);
    print_str(" hrs="); print_int(a->hours_worked);
    print_str(" OT="); print_int(a->overtime); print_str("\n");
    return hr.n_attendance - 1;
}

int hr_process_payroll(int emp_id) {
    if (hr.n_payroll >= MAX_PAYROLL || emp_id >= hr.n_employees) return -1;
    employee_t* e = &employees[emp_id];
    payroll_t* p = &payroll[hr.n_payroll];
    p->payroll_id = hr.n_payroll;
    p->employee_id = emp_id;
    p->base = e->base_salary;
    double ot_hours = 0.0;
    for (int i = 0; i < hr.n_attendance; i++) {
        if (attendance[i].employee_id == emp_id) ot_hours += attendance[i].overtime;
    }
    p->overtime_pay = ot_hours * (e->base_salary / 160.0) * 1.5;
    p->bonus = e->bonus + (double)e->performance * 10.0;
    double gross = p->base + p->overtime_pay + p->bonus;
    p->deductions = gross * 0.22;
    p->net_pay = gross - p->deductions;
    p->active = 1;
    hr.n_payroll++;
    hr.total_payroll += p->net_pay;
    print_str("[HR] Payroll E"); print_int(emp_id);
    print_str(" base="); print_int((int)p->base);
    print_str(" OT="); print_int((int)p->overtime_pay);
    print_str(" bonus="); print_int((int)p->bonus);
    print_str(" ded="); print_int((int)p->deductions);
    print_str(" net="); print_int((int)p->net_pay); print_str("\n");
    return hr.n_payroll - 1;
}

int hr_add_candidate(int pos_id, int score) {
    if (hr.n_candidates >= MAX_CANDIDATES || pos_id >= hr.n_positions) return -1;
    candidate_t* c = &candidates[hr.n_candidates];
    c->candidate_id = hr.n_candidates;
    c->position_id = pos_id;
    c->stage = 1;
    c->score = score;
    c->hired = 0;
    c->active = 1;
    hr.n_candidates++;
    hr.total_recruitment_cost += 500.0;
    print_str("[HR] Candidate "); print_int(hr.n_candidates - 1);
    print_str(" pos="); print_int(pos_id);
    print_str(" score="); print_int(score); print_str("\n");
    return hr.n_candidates - 1;
}

void hr_advance_candidate(int cand_id, int new_stage) {
    if (cand_id >= hr.n_candidates) return;
    candidate_t* c = &candidates[cand_id];
    c->stage = new_stage;
    if (new_stage >= 4 && c->score > 75) {
        c->hired = 1;
        print_str("[HR] Candidate "); print_int(cand_id); print_str(" HIRED!\n");
    } else {
        print_str("[HR] Candidate "); print_int(cand_id);
        print_str(" stage="); print_int(new_stage); print_str("\n");
    }
}

int hr_add_training(int type, int hours, double cost, int participants) {
    if (hr.n_training >= MAX_TRAINING) return -1;
    training_t* t = &training_sessions[hr.n_training];
    t->training_id = hr.n_training;
    t->type = type;
    t->n_participants = participants;
    t->hours = hours;
    t->cost = cost;
    t->effectiveness = 70.0 + (double)((hr.n_training * 5) % 20);
    t->active = 1;
    hr.n_training++;
    hr.total_training_cost += cost;
    print_str("[HR] Training "); print_int(hr.n_training - 1);
    print_str(" type="); print_int(type);
    print_str(" hrs="); print_int(hours);
    print_str(" cost="); print_int((int)cost);
    print_str(" ppl="); print_int(participants);
    print_str(" eff="); print_int((int)t->effectiveness); print_str("%\n");
    return hr.n_training - 1;
}

void hr_performance_review(void) {
    print_str("[HR] Performance review:\n");
    int top = 0, good = 0, needs = 0;
    for (int i = 0; i < hr.n_employees; i++) {
        int perf = employees[i].performance;
        if (perf >= 85) { top++; employees[i].bonus = 2000.0; }
        else if (perf >= 70) { good++; employees[i].bonus = 1000.0; }
        else { needs++; employees[i].bonus = 0.0; }
        print_str("  E"); print_int(i);
        print_str(" perf="); print_int(perf);
        print_str(" bonus="); print_int((int)employees[i].bonus); print_str("\n");
    }
    print_str("  Top: "); print_int(top);
    print_str(" Good: "); print_int(good);
    print_str(" Needs improvement: "); print_int(needs); print_str("\n");
}

void hr_print_state(void) {
    print_str("[HR] Employees="); print_int(hr.n_employees);
    print_str(" Depts="); print_int(hr.n_departments);
    print_str(" Positions="); print_int(hr.n_positions);
    print_str(" Open="); print_int(hr.open_positions);
    print_str("\n");
    print_str("  Attendance records: "); print_int(hr.n_attendance); print_str("\n");
    print_str("  Payroll processed: "); print_int(hr.n_payroll); print_str("\n");
    print_str("  Total payroll: "); print_int((int)hr.total_payroll); print_str("\n");
    print_str("  Candidates: "); print_int(hr.n_candidates); print_str("\n");
    print_str("  Training sessions: "); print_int(hr.n_training); print_str("\n");
    print_str("  Recruitment cost: "); print_int((int)hr.total_recruitment_cost); print_str("\n");
    print_str("  Training cost: "); print_int((int)hr.total_training_cost); print_str("\n");
}

int main(void) {
    print_str("=== Human Resources Management Demo ===\n\n");
    hr_init();

    print_str("Creating departments...\n");
    hr_add_department(500000.0);
    hr_add_department(400000.0);
    hr_add_department(300000.0);
    hr_add_department(250000.0);

    print_str("\nDefining positions...\n");
    hr_add_position(1, 3000.0, 5000.0);
    hr_add_position(2, 5000.0, 8000.0);
    hr_add_position(3, 8000.0, 12000.0);
    hr_add_position(4, 12000.0, 18000.0);
    hr_add_position(5, 18000.0, 30000.0);

    print_str("\nHiring employees...\n");
    hr_hire_employee(0, 4, 45, 120, 25000.0);
    hr_hire_employee(0, 2, 35, 60, 9000.0);
    hr_hire_employee(0, 1, 28, 24, 5500.0);
    hr_hire_employee(1, 3, 40, 96, 15000.0);
    hr_hire_employee(1, 2, 32, 36, 7500.0);
    hr_hire_employee(2, 1, 26, 12, 4500.0);
    hr_hire_employee(2, 1, 29, 18, 4800.0);
    hr_hire_employee(3, 2, 38, 72, 8500.0);
    hr_hire_employee(3, 1, 25, 6, 4000.0);
    hr_hire_employee(0, 1, 31, 30, 6000.0);

    print_str("\nRecording attendance...\n");
    for (int d = 1; d <= 5; d++) {
        for (int e = 0; e < 5; e++) {
            int in_h = 8 + (d + e) % 2;
            int out_h = in_h + 8 + (d * e) % 3;
            hr_record_attendance(e, d, in_h, out_h);
        }
    }

    print_str("\nProcessing payroll...\n");
    for (int e = 0; e < hr.n_employees; e++) {
        hr_process_payroll(e);
    }

    print_str("\nPerformance review...\n");
    hr_performance_review();

    print_str("\nRecruitment...\n");
    hr_add_candidate(0, 85);
    hr_add_candidate(0, 72);
    hr_add_candidate(1, 90);
    hr_add_candidate(2, 65);
    hr_add_candidate(2, 78);
    hr_advance_candidate(0, 2);
    hr_advance_candidate(0, 3);
    hr_advance_candidate(0, 4);
    hr_advance_candidate(2, 2);
    hr_advance_candidate(2, 3);
    hr_advance_candidate(2, 4);

    print_str("\nTraining programs...\n");
    hr_add_training(1, 16, 5000.0, 10);
    hr_add_training(2, 8, 3000.0, 5);
    hr_add_training(3, 24, 8000.0, 15);
    hr_add_training(1, 12, 4000.0, 8);

    print_str("\nFinal state...\n");
    hr_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
