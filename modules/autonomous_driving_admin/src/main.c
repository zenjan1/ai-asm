/* autonomous_driving_admin: Autonomous driving administration system (v1.0)
 * Perception system, decision system, execution system, testing verification, operation management
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

#define MAX_PERCEPTION   16
#define MAX_DECISION     14
#define MAX_EXECUTION    12
#define MAX_TESTING      10
#define MAX_OPERATION    10

typedef struct {
    int    perception_id;
    int    perception_type;
    int    perception_category;
    int    lidar;
    int    mmwave_radar;
    int    camera_fusion;
    int    year;
    int    active;
} perception_t;

typedef struct {
    int    decision_id;
    int    decision_type;
    int    decision_category;
    int    path_plan;
    int    behavior_predict;
    int    scene_understand;
    int    year;
    int    active;
} decision_t;

typedef struct {
    int    execution_id;
    int    execution_type;
    int    execution_category;
    int    steer_control;
    int    brake_control;
    int    drive_control;
    int    year;
    int    active;
} execution_t;

typedef struct {
    int    testing_id;
    int    testing_type;
    int    testing_category;
    int    simulation;
    int    closed_course;
    int    road_test;
    int    year;
    int    active;
} testing_t;

typedef struct {
    int    operation_id;
    int    operation_type;
    int    operation_category;
    int    fleet_manage;
    int    dispatch;
    int    safety_manage;
    int    year;
    int    active;
} operation_t;

typedef struct {
    int    n_perception;
    int    n_decision;
    int    n_execution;
    int    n_testing;
    int    n_operation;
    int    total_lidar;
    int    total_path;
    int    total_steer;
    int    total_simulation;
    int    total_fleet;
} ada_state_t;

static perception_t perceptions[MAX_PERCEPTION];
static decision_t decisions[MAX_DECISION];
static execution_t executions[MAX_EXECUTION];
static testing_t tests[MAX_TESTING];
static operation_t operations[MAX_OPERATION];
static ada_state_t ada;

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

int ada_init(void) {
    if (initialized) return -1;
    ada.n_perception = 0; ada.n_decision = 0; ada.n_execution = 0;
    ada.n_testing = 0; ada.n_operation = 0;
    ada.total_lidar = 0; ada.total_path = 0;
    ada.total_steer = 0; ada.total_simulation = 0;
    ada.total_fleet = 0;
    for (int i = 0; i < MAX_PERCEPTION; i++) perceptions[i].active = 0;
    for (int i = 0; i < MAX_DECISION; i++) decisions[i].active = 0;
    for (int i = 0; i < MAX_EXECUTION; i++) executions[i].active = 0;
    for (int i = 0; i < MAX_TESTING; i++) tests[i].active = 0;
    for (int i = 0; i < MAX_OPERATION; i++) operations[i].active = 0;
    initialized = 1;
    print_str("[ADA] Autonomous driving initialized\n");
    return 0;
}

int ada_perception(int pct_type, int cat, int lid, int mmw, int cam, int year) {
    if (ada.n_perception >= MAX_PERCEPTION) return -1;
    perception_t* p = &perceptions[ada.n_perception];
    p->perception_id = ada.n_perception;
    p->perception_type = pct_type;
    p->perception_category = cat;
    p->lidar = lid;
    p->mmwave_radar = mmw;
    p->camera_fusion = cam;
    p->year = year;
    p->active = 1;
    ada.total_lidar += lid;
    ada.n_perception++;
    print_str("[ADA] Perception "); print_int(ada.n_perception - 1);
    print_str(" type="); print_int(pct_type);
    print_str(" cat="); print_int(cat);
    print_str(" lid="); print_int(lid);
    print_str(" mmw="); print_int(mmw);
    print_str(" cam="); print_int(cam); print_str("\n");
    return ada.n_perception - 1;
}

int ada_decision(int dcs_type, int cat, int path, int behavior, int scene, int year) {
    if (ada.n_decision >= MAX_DECISION) return -1;
    decision_t* d = &decisions[ada.n_decision];
    d->decision_id = ada.n_decision;
    d->decision_type = dcs_type;
    d->decision_category = cat;
    d->path_plan = path;
    d->behavior_predict = behavior;
    d->scene_understand = scene;
    d->year = year;
    d->active = 1;
    ada.total_path += path;
    ada.n_decision++;
    print_str("[ADA] Decision "); print_int(ada.n_decision - 1);
    print_str(" type="); print_int(dcs_type);
    print_str(" cat="); print_int(cat);
    print_str(" pth="); print_int(path);
    print_str(" bhv="); print_int(behavior);
    print_str(" scn="); print_int(scene); print_str("\n");
    return ada.n_decision - 1;
}

int ada_execution(int exc_type, int cat, int steer, int brake, int drive, int year) {
    if (ada.n_execution >= MAX_EXECUTION) return -1;
    execution_t* e = &executions[ada.n_execution];
    e->execution_id = ada.n_execution;
    e->execution_type = exc_type;
    e->execution_category = cat;
    e->steer_control = steer;
    e->brake_control = brake;
    e->drive_control = drive;
    e->year = year;
    e->active = 1;
    ada.total_steer += steer;
    ada.n_execution++;
    print_str("[ADA] Execution "); print_int(ada.n_execution - 1);
    print_str(" type="); print_int(exc_type);
    print_str(" cat="); print_int(cat);
    print_str(" str="); print_int(steer);
    print_str(" brk="); print_int(brake);
    print_str(" drv="); print_int(drive); print_str("\n");
    return ada.n_execution - 1;
}

int ada_testing(int tst_type, int cat, int sim, int closed_c, int road, int year) {
    if (ada.n_testing >= MAX_TESTING) return -1;
    testing_t* t = &tests[ada.n_testing];
    t->testing_id = ada.n_testing;
    t->testing_type = tst_type;
    t->testing_category = cat;
    t->simulation = sim;
    t->closed_course = closed_c;
    t->road_test = road;
    t->year = year;
    t->active = 1;
    ada.total_simulation += sim;
    ada.n_testing++;
    print_str("[ADA] Testing "); print_int(ada.n_testing - 1);
    print_str(" type="); print_int(tst_type);
    print_str(" cat="); print_int(cat);
    print_str(" sim="); print_int(sim);
    print_str(" cls="); print_int(closed_c);
    print_str(" rds="); print_int(road); print_str("\n");
    return ada.n_testing - 1;
}

int ada_operation(int opt_type, int cat, int fleet, int disp, int safety, int year) {
    if (ada.n_operation >= MAX_OPERATION) return -1;
    operation_t* o = &operations[ada.n_operation];
    o->operation_id = ada.n_operation;
    o->operation_type = opt_type;
    o->operation_category = cat;
    o->fleet_manage = fleet;
    o->dispatch = disp;
    o->safety_manage = safety;
    o->year = year;
    o->active = 1;
    ada.total_fleet += fleet;
    ada.n_operation++;
    print_str("[ADA] Operation "); print_int(ada.n_operation - 1);
    print_str(" type="); print_int(opt_type);
    print_str(" cat="); print_int(cat);
    print_str(" flt="); print_int(fleet);
    print_str(" dsp="); print_int(disp);
    print_str(" sft="); print_int(safety); print_str("\n");
    return ada.n_operation - 1;
}

void ada_perception_report(void) {
    print_str("[ADA] Perception report:\n");
    print_str("  Perception system categories: "); print_int(ada.n_perception); print_str("\n");
    print_str("  Total LiDAR units: "); print_int(ada.total_lidar); print_str("\n");
}

void ada_decision_report(void) {
    print_str("[ADA] Decision report:\n");
    print_str("  Decision system categories: "); print_int(ada.n_decision); print_str("\n");
    print_str("  Total path plans: "); print_int(ada.total_path); print_str("\n");
}

void ada_operation_report(void) {
    print_str("[ADA] Operation report:\n");
    print_str("  Execution system categories: "); print_int(ada.n_execution); print_str("\n");
    print_str("  Total steer controls: "); print_int(ada.total_steer); print_str("\n");
    print_str("  Testing categories: "); print_int(ada.n_testing); print_str("\n");
    print_str("  Total simulations: "); print_int(ada.total_simulation); print_str("\n");
    print_str("  Operation categories: "); print_int(ada.n_operation); print_str("\n");
    print_str("  Total fleet manage: "); print_int(ada.total_fleet); print_str("\n");
}

void ada_print_state(void) {
    print_str("[ADA] Pc="); print_int(ada.n_perception);
    print_str(" Dc="); print_int(ada.n_decision);
    print_str(" Ex="); print_int(ada.n_execution);
    print_str(" Ts="); print_int(ada.n_testing);
    print_str(" Op="); print_int(ada.n_operation);
    print_str("\n");
}

int main(void) {
    print_str("=== Autonomous Driving Admin Demo ===\n\n");
    ada_init();

    print_str("Perception systems...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int lid = 50 + (i * 12);
        int mmw = 35 + (i * 8);
        int cam = 25 + (i * 6);
        int year = 2020 + (i % 5);
        ada_perception(type, cat, lid, mmw, cam, year);
    }

    print_str("\nDecision systems...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pth = 40 + (i * 10);
        int bhv = 30 + (i * 7);
        int scn = 20 + (i * 5);
        int year = 2021 + (i % 4);
        ada_decision(type, cat, pth, bhv, scn, year);
    }

    print_str("\nExecution systems...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int str = 35 + (i * 9);
        int brk = 25 + (i * 6);
        int drv = 18 + (i * 4);
        int year = 2022 + (i % 3);
        ada_execution(type, cat, str, brk, drv, year);
    }

    print_str("\nTesting verification...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int sim = 60 + (i * 15);
        int cls = 30 + (i * 7);
        int rds = 20 + (i * 5);
        int year = 2023 + (i % 2);
        ada_testing(type, cat, sim, cls, rds, year);
    }

    print_str("\nOperation management...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int flt = 25 + (i * 6);
        int dsp = 18 + (i * 4);
        int sft = 12 + (i * 3);
        int year = 2024;
        ada_operation(type, cat, flt, dsp, sft, year);
    }

    print_str("\nPerception report...\n");
    ada_perception_report();

    print_str("\nDecision report...\n");
    ada_decision_report();

    print_str("\nOperation report...\n");
    ada_operation_report();

    print_str("\nFinal state...\n");
    ada_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
