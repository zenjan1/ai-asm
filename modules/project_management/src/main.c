/* project_management: Project management system (v1.0)
 * Project planning, task management, team collaboration, progress monitoring, resource management
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

#define MAX_PROJECTS   6
#define MAX_TASKS      20
#define MAX_MEMBERS    12
#define MAX_MILESTONES 10
#define MAX_RISKS      8
#define MAX_MEETINGS   8

typedef struct {
    int    project_id;
    int    status;
    int    start_day;
    int    end_day;
    int    progress;
    double budget;
    double spent;
    int    n_tasks;
    int    n_members;
    int    active;
} project_t;

typedef struct {
    int    task_id;
    int    project_id;
    int    assignee;
    int    priority;
    int    estimated_hours;
    int    actual_hours;
    int    progress;
    int    status;
    int    depends_on;
    int    active;
} task_t;

typedef struct {
    int    member_id;
    int    project_id;
    int    role;
    int    availability;
    int    n_tasks;
    int    active;
} member_t;

typedef struct {
    int    milestone_id;
    int    project_id;
    int    target_day;
    int    achieved_day;
    int    status;
    int    active;
} milestone_t;

typedef struct {
    int    risk_id;
    int    project_id;
    int    probability;
    int    impact;
    int    risk_score;
    int    mitigated;
    int    active;
} risk_t;

typedef struct {
    int    meeting_id;
    int    project_id;
    int    day;
    int    duration;
    int    attendees;
    int    action_items;
    int    active;
} meeting_t;

typedef struct {
    int    n_projects;
    int    n_tasks;
    int    n_members;
    int    n_milestones;
    int    n_risks;
    int    n_meetings;
    double total_budget;
    double total_spent;
    int    completed_tasks;
    int    overdue_tasks;
    int    blocked_tasks;
} pm_state_t;

static project_t projects[MAX_PROJECTS];
static task_t tasks[MAX_TASKS];
static member_t members[MAX_MEMBERS];
static milestone_t milestones[MAX_MILESTONES];
static risk_t risks[MAX_RISKS];
static meeting_t meetings[MAX_MEETINGS];
static pm_state_t pm;

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

int pm_init(void) {
    if (initialized) return -1;
    pm.n_projects = 0; pm.n_tasks = 0; pm.n_members = 0;
    pm.n_milestones = 0; pm.n_risks = 0; pm.n_meetings = 0;
    pm.total_budget = 0.0; pm.total_spent = 0.0;
    pm.completed_tasks = 0; pm.overdue_tasks = 0; pm.blocked_tasks = 0;
    for (int i = 0; i < MAX_PROJECTS; i++) projects[i].active = 0;
    for (int i = 0; i < MAX_TASKS; i++) tasks[i].active = 0;
    for (int i = 0; i < MAX_MEMBERS; i++) members[i].active = 0;
    for (int i = 0; i < MAX_MILESTONES; i++) milestones[i].active = 0;
    for (int i = 0; i < MAX_RISKS; i++) risks[i].active = 0;
    for (int i = 0; i < MAX_MEETINGS; i++) meetings[i].active = 0;
    initialized = 1;
    print_str("[PM] Project management initialized\n");
    return 0;
}

int pm_create_project(int duration_days, double budget) {
    if (pm.n_projects >= MAX_PROJECTS) return -1;
    project_t* p = &projects[pm.n_projects];
    p->project_id = pm.n_projects;
    p->status = 1;
    p->start_day = 1;
    p->end_day = duration_days;
    p->progress = 0;
    p->budget = budget;
    p->spent = 0.0;
    p->n_tasks = 0;
    p->n_members = 0;
    p->active = 1;
    pm.n_projects++;
    pm.total_budget += budget;
    print_str("[PM] Project "); print_int(pm.n_projects - 1);
    print_str(" duration="); print_int(duration_days); print_str("d");
    print_str(" budget="); print_int((int)budget); print_str("\n");
    return pm.n_projects - 1;
}

int pm_add_member(int project_id, int role) {
    if (pm.n_members >= MAX_MEMBERS || project_id >= pm.n_projects) return -1;
    member_t* m = &members[pm.n_members];
    m->member_id = pm.n_members;
    m->project_id = project_id;
    m->role = role;
    m->availability = 100;
    m->n_tasks = 0;
    m->active = 1;
    pm.n_members++;
    projects[project_id].n_members++;
    print_str("[PM] Member "); print_int(pm.n_members - 1);
    print_str(" project="); print_int(project_id);
    print_str(" role="); print_int(role); print_str("\n");
    return pm.n_members - 1;
}

int pm_add_task(int project_id, int assignee, int priority, int hours, int depends) {
    if (pm.n_tasks >= MAX_TASKS) return -1;
    if (project_id >= pm.n_projects) return -1;
    task_t* t = &tasks[pm.n_tasks];
    t->task_id = pm.n_tasks;
    t->project_id = project_id;
    t->assignee = assignee;
    t->priority = priority;
    t->estimated_hours = hours;
    t->actual_hours = 0;
    t->progress = 0;
    t->status = 1;
    t->depends_on = depends;
    t->active = 1;
    pm.n_tasks++;
    projects[project_id].n_tasks++;
    if (assignee >= 0 && assignee < pm.n_members) members[assignee].n_tasks++;
    print_str("[PM] Task "); print_int(pm.n_tasks - 1);
    print_str(" proj="); print_int(project_id);
    print_str(" M"); print_int(assignee);
    print_str(" P"); print_int(priority);
    print_str(" est="); print_int(hours); print_str("h");
    if (depends >= 0) { print_str(" dep="); print_int(depends); }
    print_str("\n");
    return pm.n_tasks - 1;
}

void pm_update_task(int task_id, int actual_hours, int progress) {
    if (task_id >= pm.n_tasks) return;
    task_t* t = &tasks[task_id];
    t->actual_hours = actual_hours;
    t->progress = progress;
    if (progress >= 100) {
        t->status = 3;
        pm.completed_tasks++;
        print_str("[PM] Task "); print_int(task_id); print_str(" COMPLETED\n");
    } else {
        print_str("[PM] Task "); print_int(task_id);
        print_str(" progress="); print_int(progress); print_str("%");
        print_str(" hours="); print_int(actual_hours);
        print_str("/"); print_int(t->estimated_hours); print_str("\n");
    }
}

int pm_add_milestone(int project_id, int target_day) {
    if (pm.n_milestones >= MAX_MILESTONES || project_id >= pm.n_projects) return -1;
    milestone_t* m = &milestones[pm.n_milestones];
    m->milestone_id = pm.n_milestones;
    m->project_id = project_id;
    m->target_day = target_day;
    m->achieved_day = 0;
    m->status = 0;
    m->active = 1;
    pm.n_milestones++;
    print_str("[PM] Milestone "); print_int(pm.n_milestones - 1);
    print_str(" proj="); print_int(project_id);
    print_str(" target=day "); print_int(target_day); print_str("\n");
    return pm.n_milestones - 1;
}

void pm_achieve_milestone(int milestone_id, int day) {
    if (milestone_id >= pm.n_milestones) return;
    milestone_t* m = &milestones[milestone_id];
    m->achieved_day = day;
    m->status = 1;
    int on_time = (day <= m->target_day) ? 1 : 0;
    print_str("[PM] Milestone "); print_int(milestone_id);
    print_str(" achieved day "); print_int(day);
    if (on_time) print_str(" (ON TIME)\n");
    else print_str(" (LATE by "); print_int(day - m->target_day); print_str("d)\n");
}

int pm_add_risk(int project_id, int probability, int impact) {
    if (pm.n_risks >= MAX_RISKS || project_id >= pm.n_projects) return -1;
    risk_t* r = &risks[pm.n_risks];
    r->risk_id = pm.n_risks;
    r->project_id = project_id;
    r->probability = probability;
    r->impact = impact;
    r->risk_score = probability * impact / 10;
    r->mitigated = 0;
    r->active = 1;
    pm.n_risks++;
    print_str("[PM] Risk "); print_int(pm.n_risks - 1);
    print_str(" proj="); print_int(project_id);
    print_str(" P="); print_int(probability);
    print_str(" I="); print_int(impact);
    print_str(" score="); print_int(r->risk_score); print_str("\n");
    return pm.n_risks - 1;
}

void pm_mitigate_risk(int risk_id) {
    if (risk_id >= pm.n_risks) return;
    risks[risk_id].mitigated = 1;
    print_str("[PM] Risk "); print_int(risk_id); print_str(" mitigated\n");
}

int pm_schedule_meeting(int project_id, int day, int duration, int attendees) {
    if (pm.n_meetings >= MAX_MEETINGS || project_id >= pm.n_projects) return -1;
    meeting_t* m = &meetings[pm.n_meetings];
    m->meeting_id = pm.n_meetings;
    m->project_id = project_id;
    m->day = day;
    m->duration = duration;
    m->attendees = attendees;
    m->action_items = 0;
    m->active = 1;
    pm.n_meetings++;
    print_str("[PM] Meeting "); print_int(pm.n_meetings - 1);
    print_str(" proj="); print_int(project_id);
    print_str(" day="); print_int(day);
    print_str(" dur="); print_int(duration); print_str("h");
    print_str(" att="); print_int(attendees); print_str("\n");
    return pm.n_meetings - 1;
}

void pm_add_action_items(int meeting_id, int items) {
    if (meeting_id >= pm.n_meetings) return;
    meetings[meeting_id].action_items += items;
    print_str("[PM] Meeting "); print_int(meeting_id);
    print_str(" +"); print_int(items); print_str(" action items (total: ");
    print_int(meetings[meeting_id].action_items); print_str(")\n");
}

void pm_project_status(int project_id) {
    if (project_id >= pm.n_projects) return;
    project_t* p = &projects[project_id];
    int total_hours = 0, done_hours = 0;
    for (int i = 0; i < pm.n_tasks; i++) {
        if (tasks[i].project_id == project_id) {
            total_hours += tasks[i].estimated_hours;
            done_hours += tasks[i].estimated_hours * tasks[i].progress / 100;
        }
    }
    p->progress = (total_hours > 0) ? done_hours * 100 / total_hours : 0;
    print_str("[PM] Project "); print_int(project_id); print_str(" status:\n");
    print_str("  Progress: "); print_int(p->progress); print_str("%\n");
    print_str("  Tasks: "); print_int(p->n_tasks); print_str("\n");
    print_str("  Members: "); print_int(p->n_members); print_str("\n");
    print_str("  Budget: "); print_int((int)p->spent);
    print_str("/"); print_int((int)p->budget); print_str("\n");
    double burn = (p->budget > 0) ? p->spent / p->budget * 100.0 : 0.0;
    print_str("  Burn: "); print_int((int)burn); print_str("%\n");
}

void pm_risk_summary(void) {
    print_str("[PM] Risk summary:\n");
    int high = 0, med = 0, low = 0;
    for (int i = 0; i < pm.n_risks; i++) {
        int score = risks[i].risk_score;
        if (score >= 70) high++;
        else if (score >= 40) med++;
        else low++;
    }
    print_str("  High: "); print_int(high);
    print_str(" Medium: "); print_int(med);
    print_str(" Low: "); print_int(low); print_str("\n");
    int mitigated = 0;
    for (int i = 0; i < pm.n_risks; i++) {
        if (risks[i].mitigated) mitigated++;
    }
    print_str("  Mitigated: "); print_int(mitigated);
    print_str("/"); print_int(pm.n_risks); print_str("\n");
}

void pm_print_state(void) {
    print_str("[PM] Projects="); print_int(pm.n_projects);
    print_str(" Tasks="); print_int(pm.n_tasks);
    print_str(" Members="); print_int(pm.n_members);
    print_str(" Milestones="); print_int(pm.n_milestones);
    print_str(" Risks="); print_int(pm.n_risks);
    print_str(" Meetings="); print_int(pm.n_meetings);
    print_str("\n");
    print_str("  Total budget: "); print_int((int)pm.total_budget); print_str("\n");
    print_str("  Total spent: "); print_int((int)pm.total_spent); print_str("\n");
    print_str("  Completed tasks: "); print_int(pm.completed_tasks); print_str("\n");
    print_str("  Overdue: "); print_int(pm.overdue_tasks); print_str("\n");
    print_str("  Blocked: "); print_int(pm.blocked_tasks); print_str("\n");
}

int main(void) {
    print_str("=== Project Management System Demo ===\n\n");
    pm_init();

    print_str("Creating projects...\n");
    pm_create_project(90, 200000.0);
    pm_create_project(60, 120000.0);
    pm_create_project(120, 350000.0);

    print_str("\nAdding team members...\n");
    pm_add_member(0, 1);
    pm_add_member(0, 2);
    pm_add_member(0, 2);
    pm_add_member(1, 1);
    pm_add_member(1, 2);
    pm_add_member(2, 1);
    pm_add_member(2, 2);
    pm_add_member(2, 2);
    pm_add_member(2, 3);

    print_str("\nAdding tasks...\n");
    pm_add_task(0, 0, 1, 40, -1);
    pm_add_task(0, 1, 2, 30, 0);
    pm_add_task(0, 2, 3, 20, 0);
    pm_add_task(0, 1, 2, 25, 1);
    pm_add_task(0, 2, 1, 15, 2);
    pm_add_task(1, 3, 1, 35, -1);
    pm_add_task(1, 4, 2, 20, 5);
    pm_add_task(1, 3, 3, 25, 5);
    pm_add_task(2, 5, 1, 50, -1);
    pm_add_task(2, 6, 2, 40, 9);
    pm_add_task(2, 7, 2, 30, 9);
    pm_add_task(2, 8, 3, 35, 10);
    pm_add_task(2, 6, 1, 25, 11);

    print_str("\nUpdating task progress...\n");
    pm_update_task(0, 40, 100);
    pm_update_task(1, 20, 70);
    pm_update_task(2, 20, 100);
    pm_update_task(3, 10, 40);
    pm_update_task(5, 35, 100);
    pm_update_task(6, 15, 75);
    pm_update_task(9, 30, 75);
    pm_update_task(10, 20, 65);

    print_str("\nSetting milestones...\n");
    pm_add_milestone(0, 30);
    pm_add_milestone(0, 60);
    pm_add_milestone(0, 90);
    pm_add_milestone(1, 30);
    pm_add_milestone(1, 60);
    pm_add_milestone(2, 40);
    pm_add_milestone(2, 80);
    pm_add_milestone(2, 120);

    print_str("\nAchieving milestones...\n");
    pm_achieve_milestone(0, 28);
    pm_achieve_milestone(1, 62);
    pm_achieve_milestone(3, 30);
    pm_achieve_milestone(5, 38);

    print_str("\nIdentifying risks...\n");
    pm_add_risk(0, 70, 80);
    pm_add_risk(0, 40, 60);
    pm_add_risk(1, 60, 50);
    pm_add_risk(1, 30, 40);
    pm_add_risk(2, 80, 90);
    pm_add_risk(2, 50, 70);
    pm_add_risk(2, 25, 30);

    print_str("\nMitigating risks...\n");
    pm_mitigate_risk(0);
    pm_mitigate_risk(2);
    pm_mitigate_risk(4);

    print_str("\nScheduling meetings...\n");
    pm_schedule_meeting(0, 7, 1, 3);
    pm_schedule_meeting(0, 30, 2, 5);
    pm_schedule_meeting(1, 14, 1, 2);
    pm_schedule_meeting(2, 10, 2, 4);
    pm_schedule_meeting(2, 45, 2, 6);

    print_str("\nAdding action items...\n");
    pm_add_action_items(0, 3);
    pm_add_action_items(1, 5);
    pm_add_action_items(2, 2);
    pm_add_action_items(3, 4);
    pm_add_action_items(4, 6);

    print_str("\nProject status reports...\n");
    pm_project_status(0);
    pm_project_status(1);
    pm_project_status(2);

    print_str("\nRisk summary...\n");
    pm_risk_summary();

    print_str("\nFinal state...\n");
    pm_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
