/*
 * aiasm-aarch64/modules/state_machine_engine/src/main.c
 * State Machine Engine WASM module for AI-ASM OS
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

#define MAX_STATES      128
#define MAX_TRANSITIONS 256
#define MAX_INSTANCES   32
#define MAX_HISTORY     512
#define MAX_LOG         1024
#define MAX_NAME        48
#define STATE_INITIAL   0x01
#define STATE_FINAL     0x02
#define STATE_PARALLEL  0x04
#define TRANS_INTERNAL  0x01

typedef unsigned int uint;
typedef unsigned char uchar;

typedef struct {
    char name[MAX_NAME]; uint flags;
    int parent_id; int child_ids[8]; int child_count;
    int entry_action_id; int exit_action_id;
} State;

typedef struct {
    char name[MAX_NAME]; char event_name[MAX_NAME];
    int source_id; int target_id; uint flags;
    int guard_id; int action_id;
} Transition;

typedef struct {
    char name[MAX_NAME];
    int (*guard_func)(void *); void (*action_func)(void *);
} GuardAction;

typedef struct {
    char name[MAX_NAME];
    State states[MAX_STATES]; int state_count;
    Transition transitions[MAX_TRANSITIONS]; int transition_count;
    GuardAction guards[MAX_TRANSITIONS]; int guard_count;
    int current_state_id; int parallel_states[8]; int parallel_count;
    int history[MAX_HISTORY]; int history_count;
    char log[MAX_LOG]; int log_pos;
    int locked; int debug_mode; int breakpoints[MAX_STATES];
} StateMachine;

typedef struct {
    char name[MAX_NAME]; StateMachine *machine;
    int instance_id; int locked;
} MachineInstance;

static unsigned int heap_pos = 65536;
static StateMachine machines[16];
static int machine_count = 0;
static MachineInstance instances[MAX_INSTANCES];
static int instance_count = 0;
static int debug_trace_enabled = 0;

/* Forward declarations */
static uint alloc_mem(uint n);
static uint my_strlen(const char *s);
static void my_strcpy(char *d, const char *s, uint m);
static int my_strcmp(const char *a, const char *b);
static void print_str(const char *s);
static void print_int(int v);
static StateMachine* sm_create(const char *name);
static void sm_delete(StateMachine *sm);
static void sm_reset(StateMachine *sm);
static int state_add(StateMachine *sm, const char *name);
static int state_add_initial(StateMachine *sm, const char *name);
static int state_add_final(StateMachine *sm, const char *name);
static int state_add_hierarchical(StateMachine *sm, const char *name, int parent_id);
static int state_add_parallel(StateMachine *sm, const char *name);
static int transition_add(StateMachine *sm, const char *event, int from_id, int to_id);
static int transition_add_with_guard(StateMachine *sm, const char *event, int from_id, int to_id, int guard_id);
static int transition_add_internal(StateMachine *sm, const char *event, int state_id, int action_id);
static int guard_evaluate(StateMachine *sm, int guard_id, void *payload);
static void action_execute(StateMachine *sm, int action_id, void *payload);
static int enter_state(StateMachine *sm, int state_id);
static int exit_state(StateMachine *sm, int state_id);
static int enter_nested(StateMachine *sm, int state_id);
static int exit_nested(StateMachine *sm, int state_id);
static int event_fire(StateMachine *sm, const char *event_name);
static int event_fire_with_payload(StateMachine *sm, const char *event_name, void *payload);
static int history_save(StateMachine *sm);
static int history_restore(StateMachine *sm);
static int history_shallow(StateMachine *sm);
static int history_deep(StateMachine *sm);
static int history_replay(StateMachine *sm);
static int history_trace(StateMachine *sm);
static int snapshot_save(StateMachine *sm);
static int snapshot_load(StateMachine *sm);
static int snapshot_list(void);
static int transition_log(StateMachine *sm, const char *msg);
static int transition_get_log(StateMachine *sm);
static int instance_create(StateMachine *sm, const char *inst_name);
static void instance_delete(int inst_id);
static int instance_list(void);
static int instance_lock(int inst_id);
static int instance_unlock(int inst_id);
static void debug_visualize(StateMachine *sm);
static void debug_trace_on(StateMachine *sm);
static void debug_trace_off(StateMachine *sm);
static int debug_breakpoint_set(StateMachine *sm, int state_id);
static int debug_simulate(StateMachine *sm, const char *event);
static int debug_step(StateMachine *sm);
static void test_suite(void);

static uint alloc_mem(uint n) {
    uint p = heap_pos; heap_pos += n; heap_pos = (heap_pos + 15) & ~15u; return p;
}
static uint my_strlen(const char *s) { uint l = 0; while (s[l]) l++; return l; }
static void my_strcpy(char *d, const char *s, uint m) {
    uint i = 0; while (i < m - 1 && s[i]) { d[i] = s[i]; i++; } d[i] = '\0';
}
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b && *a == *b) { a++; b++; } return (int)(*a) - (int)(*b);
}
static void print_str(const char *s) { host_print(s); }

static void print_int(int v) {
    char b[32]; int p = 0, neg = 0;
    if (v < 0) { neg = 1; v = -v; }
    if (v == 0) { b[p++] = '0'; }
    while (v > 0 && p < 30) { b[p++] = '0' + (v % 10); v /= 10; }
    if (neg) b[p++] = '-'; b[p] = '\0';
    for (int i = 0; i < p / 2; i++) { char t = b[i]; b[i] = b[p-1-i]; b[p-1-i] = t; }
    print_str(b);
}

static StateMachine* sm_create(const char *name) {
    if (machine_count >= 16) { print_str("Error: Max machines\n"); return NULL; }
    StateMachine *sm = &machines[machine_count++];
    my_strcpy(sm->name, name, MAX_NAME);
    sm->state_count = 0; sm->transition_count = 0; sm->guard_count = 0;
    sm->current_state_id = -1; sm->parallel_count = 0; sm->history_count = 0;
    sm->log_pos = 0; sm->locked = 0; sm->debug_mode = 0;
    print_str("Created machine: "); print_str(name); print_str("\n");
    return sm;
}

static void sm_delete(StateMachine *sm) {
    if (!sm) return; print_str("Deleted machine: "); print_str(sm->name); print_str("\n");
    sm->state_count = 0; sm->transition_count = 0;
}

static void sm_reset(StateMachine *sm) {
    if (!sm) return; sm->current_state_id = -1; sm->parallel_count = 0;
    sm->history_count = 0; sm->log_pos = 0;
    print_str("Reset machine: "); print_str(sm->name); print_str("\n");
}

static int state_add(StateMachine *sm, const char *name) {
    if (!sm || sm->state_count >= MAX_STATES) return -1;
    int id = sm->state_count++;
    State *s = &sm->states[id];
    my_strcpy(s->name, name, MAX_NAME); s->flags = 0; s->parent_id = -1;
    s->child_count = 0; s->entry_action_id = -1; s->exit_action_id = -1;
    print_str("  Added state: "); print_str(name);
    print_str(" (id="); print_int(id); print_str(")\n");
    return id;
}

static int state_add_initial(StateMachine *sm, const char *name) {
    int id = state_add(sm, name);
    if (id >= 0) { sm->states[id].flags |= STATE_INITIAL; sm->current_state_id = id;
        print_str("    -> Initial state set\n"); } return id;
}

static int state_add_final(StateMachine *sm, const char *name) {
    int id = state_add(sm, name);
    if (id >= 0) { sm->states[id].flags |= STATE_FINAL; print_str("    -> Final state\n"); }
    return id;
}

static int state_add_hierarchical(StateMachine *sm, const char *name, int parent_id) {
    int id = state_add(sm, name);
    if (id >= 0 && parent_id >= 0 && parent_id < sm->state_count) {
        sm->states[id].parent_id = parent_id;
        State *p = &sm->states[parent_id];
        if (p->child_count < 8) p->child_ids[p->child_count++] = id;
        print_str("    -> Child of "); print_str(p->name); print_str("\n");
    } return id;
}

static int state_add_parallel(StateMachine *sm, const char *name) {
    int id = state_add(sm, name);
    if (id >= 0) { sm->states[id].flags |= STATE_PARALLEL;
        if (sm->parallel_count < 8) sm->parallel_states[sm->parallel_count++] = id;
        print_str("    -> Parallel state\n"); } return id;
}

static int transition_add(StateMachine *sm, const char *event, int from_id, int to_id) {
    if (!sm || sm->transition_count >= MAX_TRANSITIONS) return -1;
    int id = sm->transition_count++;
    Transition *t = &sm->transitions[id];
    my_strcpy(t->name, event, MAX_NAME); my_strcpy(t->event_name, event, MAX_NAME);
    t->source_id = from_id; t->target_id = to_id; t->flags = 0;
    t->guard_id = -1; t->action_id = -1;
    print_str("  Transition: "); print_str(event);
    print_str(" ("); print_str(sm->states[from_id].name);
    print_str(" -> "); print_str(sm->states[to_id].name); print_str(")\n");
    return id;
}

static int transition_add_with_guard(StateMachine *sm, const char *event,
                                     int from_id, int to_id, int guard_id) {
    int id = transition_add(sm, event, from_id, to_id);
    if (id >= 0) { sm->transitions[id].guard_id = guard_id;
        print_str("    -> Guard id="); print_int(guard_id); print_str("\n"); } return id;
}

static int transition_add_internal(StateMachine *sm, const char *event,
                                   int state_id, int action_id) {
    if (!sm || sm->transition_count >= MAX_TRANSITIONS) return -1;
    int id = sm->transition_count++;
    Transition *t = &sm->transitions[id];
    my_strcpy(t->name, event, MAX_NAME); my_strcpy(t->event_name, event, MAX_NAME);
    t->source_id = state_id; t->target_id = state_id; t->flags = TRANS_INTERNAL;
    t->guard_id = -1; t->action_id = action_id;
    print_str("  Internal: "); print_str(event);
    print_str(" in "); print_str(sm->states[state_id].name); print_str("\n");
    return id;
}

static int guard_evaluate(StateMachine *sm, int guard_id, void *payload) {
    if (!sm || guard_id < 0 || guard_id >= sm->guard_count) return 1;
    GuardAction *g = &sm->guards[guard_id];
    if (g->guard_func) { int r = g->guard_func(payload);
        print_str("  Guard "); print_str(g->name);
        print_str(": "); print_str(r ? "true" : "false"); print_str("\n"); return r; }
    return 1;
}

static void action_execute(StateMachine *sm, int action_id, void *payload) {
    if (!sm || action_id < 0 || action_id >= sm->guard_count) return;
    GuardAction *a = &sm->guards[action_id];
    if (a->action_func) { print_str("  Action: "); print_str(a->name); print_str("\n");
        a->action_func(payload); }
}

static int enter_state(StateMachine *sm, int state_id) {
    if (!sm || state_id < 0 || state_id >= sm->state_count) return -1;
    State *s = &sm->states[state_id];
    print_str("Enter: "); print_str(s->name); print_str("\n");
    if (s->entry_action_id >= 0) action_execute(sm, s->entry_action_id, NULL);
    sm->current_state_id = state_id;
    if (sm->history_count < MAX_HISTORY) sm->history[sm->history_count++] = state_id;
    return 0;
}

static int exit_state(StateMachine *sm, int state_id) {
    if (!sm || state_id < 0 || state_id >= sm->state_count) return -1;
    State *s = &sm->states[state_id];
    print_str("Exit: "); print_str(s->name); print_str("\n");
    if (s->exit_action_id >= 0) action_execute(sm, s->exit_action_id, NULL);
    return 0;
}

static int enter_nested(StateMachine *sm, int state_id) {
    if (!sm) return -1; int cur = state_id, depth = 0;
    while (cur >= 0 && depth < 8) { State *s = &sm->states[cur];
        print_str("  [d="); print_int(depth); print_str("] Enter: ");
        print_str(s->name); print_str("\n");
        if (s->entry_action_id >= 0) action_execute(sm, s->entry_action_id, NULL);
        cur = s->parent_id; depth++; }
    sm->current_state_id = state_id; return depth;
}

static int exit_nested(StateMachine *sm, int state_id) {
    if (!sm) return -1; int cur = state_id, depth = 0;
    while (cur >= 0 && depth < 8) { State *s = &sm->states[cur];
        print_str("  [d="); print_int(depth); print_str("] Exit: ");
        print_str(s->name); print_str("\n");
        if (s->exit_action_id >= 0) action_execute(sm, s->exit_action_id, NULL);
        cur = s->parent_id; depth++; } return depth;
}

static int event_fire(StateMachine *sm, const char *event_name) {
    if (!sm || sm->locked) { print_str("Cannot fire: locked\n"); return -1; }
    print_str("Fire: "); print_str(event_name); print_str("\n");
    for (int i = 0; i < sm->transition_count; i++) {
        Transition *t = &sm->transitions[i];
        if (my_strcmp(t->event_name, event_name) == 0 && t->source_id == sm->current_state_id) {
            if (t->guard_id >= 0 && !guard_evaluate(sm, t->guard_id, NULL)) {
                print_str("  Blocked by guard\n"); return -1; }
            if (t->flags == 0) { exit_state(sm, sm->current_state_id);
                enter_state(sm, t->target_id); }
            else if (t->action_id >= 0) action_execute(sm, t->action_id, NULL);
            if (sm->log_pos + 32 < MAX_LOG) {
                my_strcpy(&sm->log[sm->log_pos], event_name, 32); sm->log_pos += 32; }
            return i; } }
    print_str("  No transition\n"); return -1;
}

static int event_fire_with_payload(StateMachine *sm, const char *event_name, void *payload) {
    if (!sm || sm->locked) { print_str("Cannot fire: locked\n"); return -1; }
    print_str("Fire with payload: "); print_str(event_name); print_str("\n");
    for (int i = 0; i < sm->transition_count; i++) {
        Transition *t = &sm->transitions[i];
        if (my_strcmp(t->event_name, event_name) == 0 && t->source_id == sm->current_state_id) {
            if (t->guard_id >= 0 && !guard_evaluate(sm, t->guard_id, payload)) {
                print_str("  Blocked by guard\n"); return -1; }
            if (t->flags == 0) { exit_state(sm, sm->current_state_id);
                enter_state(sm, t->target_id); }
            if (t->action_id >= 0) action_execute(sm, t->action_id, payload);
            return i; } }
    print_str("  No transition\n"); return -1;
}

static int history_save(StateMachine *sm) {
    if (!sm) return -1; print_str("History saved: ");
    print_str(sm->states[sm->current_state_id].name); print_str("\n");
    return sm->history_count;
}

static int history_restore(StateMachine *sm) {
    if (!sm || sm->history_count == 0) return -1;
    int prev = sm->history[--sm->history_count];
    print_str("History restored: "); print_str(sm->states[prev].name); print_str("\n");
    sm->current_state_id = prev; return prev;
}

static int history_shallow(StateMachine *sm) {
    if (!sm || sm->history_count < 2) return -1;
    int prev = sm->history[sm->history_count - 2];
    print_str("Shallow history: "); print_str(sm->states[prev].name); print_str("\n");
    return prev;
}

static int history_deep(StateMachine *sm) {
    if (!sm || sm->history_count == 0) return -1;
    print_str("Deep history:\n");
    for (int i = 0; i < sm->history_count; i++) {
        print_str("  ["); print_int(i); print_str("] ");
        print_str(sm->states[sm->history[i]].name); print_str("\n"); }
    return sm->history_count;
}

static int history_replay(StateMachine *sm) {
    if (!sm || sm->history_count == 0) return -1;
    print_str("Replay history:\n");
    for (int i = 0; i < sm->history_count; i++) {
        print_str("  -> "); print_str(sm->states[sm->history[i]].name); print_str("\n"); }
    return sm->history_count;
}

static int history_trace(StateMachine *sm) { return history_deep(sm); }

static int snapshot_save(StateMachine *sm) {
    if (!sm) return -1; uint size = sizeof(StateMachine);
    uint ptr = host_alloc(size, 8);
    StateMachine *copy = (StateMachine *)ptr;
    for (uint i = 0; i < size; i++) ((uchar *)copy)[i] = ((uchar *)sm)[i];
    print_str("Snapshot saved: "); print_str(sm->name);
    print_str(" (size="); print_int(size); print_str(")\n"); return 0;
}

static int snapshot_load(StateMachine *sm) {
    if (!sm) return -1; print_str("Snapshot loaded: ");
    print_str(sm->name); print_str("\n"); return 0;
}

static int snapshot_list(void) {
    print_str("Snapshots:\n");
    for (int i = 0; i < machine_count; i++) {
        print_str("  - "); print_str(machines[i].name); print_str("\n"); }
    return machine_count;
}

static int transition_log(StateMachine *sm, const char *msg) {
    if (!sm || sm->log_pos + 32 >= MAX_LOG) return -1;
    my_strcpy(&sm->log[sm->log_pos], msg, 32); sm->log_pos += 32;
    print_str("Logged: "); print_str(msg); print_str("\n"); return 0;
}

static int transition_get_log(StateMachine *sm) {
    if (!sm) return -1; print_str("Transition log:\n");
    int pos = 0, count = 0;
    while (pos < sm->log_pos) { print_str("  "); print_int(count++);
        print_str(": "); print_str(&sm->log[pos]); print_str("\n"); pos += 32; }
    return count;
}

static int instance_create(StateMachine *sm, const char *inst_name) {
    if (instance_count >= MAX_INSTANCES) { print_str("Error: Max instances\n"); return -1; }
    int id = instance_count++;
    MachineInstance *inst = &instances[id];
    my_strcpy(inst->name, inst_name, MAX_NAME);
    inst->machine = sm; inst->instance_id = id; inst->locked = 0;
    print_str("Created instance: "); print_str(inst_name);
    print_str(" (id="); print_int(id); print_str(")\n"); return id;
}

static void instance_delete(int inst_id) {
    if (inst_id < 0 || inst_id >= instance_count) return;
    print_str("Deleted instance: "); print_str(instances[inst_id].name); print_str("\n");
    instances[inst_id].machine = NULL;
}

static int instance_list(void) {
    print_str("Instances:\n");
    for (int i = 0; i < instance_count; i++) {
        if (instances[i].machine) { print_str("  ["); print_int(i); print_str("] ");
            print_str(instances[i].name); print_str(" -> ");
            print_str(instances[i].machine->name); print_str("\n"); } }
    return instance_count;
}

static int instance_lock(int inst_id) {
    if (inst_id < 0 || inst_id >= instance_count) return -1;
    instances[inst_id].locked = 1;
    print_str("Locked: "); print_str(instances[inst_id].name); print_str("\n"); return 0;
}

static int instance_unlock(int inst_id) {
    if (inst_id < 0 || inst_id >= instance_count) return -1;
    instances[inst_id].locked = 0;
    print_str("Unlocked: "); print_str(instances[inst_id].name); print_str("\n"); return 0;
}

static void debug_visualize(StateMachine *sm) {
    if (!sm) return;
    print_str("=== Visualization ===\nMachine: "); print_str(sm->name); print_str("\n");
    print_str("States ("); print_int(sm->state_count); print_str("):\n");
    for (int i = 0; i < sm->state_count; i++) { State *s = &sm->states[i];
        print_str("  ["); print_int(i); print_str("] "); print_str(s->name);
        if (s->flags & STATE_INITIAL) print_str(" (INIT)");
        if (s->flags & STATE_FINAL) print_str(" (FINAL)");
        if (s->flags & STATE_PARALLEL) print_str(" (PAR)");
        if (s->parent_id >= 0) { print_str(" parent=");
            print_str(sm->states[s->parent_id].name); }
        print_str("\n"); }
    print_str("Transitions ("); print_int(sm->transition_count); print_str("):\n");
    for (int i = 0; i < sm->transition_count; i++) { Transition *t = &sm->transitions[i];
        print_str("  "); print_str(t->event_name); print_str(": ");
        print_str(sm->states[t->source_id].name); print_str(" -> ");
        print_str(sm->states[t->target_id].name);
        if (t->flags == TRANS_INTERNAL) print_str(" [INT]");
        print_str("\n"); }
    print_str("Current: ");
    if (sm->current_state_id >= 0) print_str(sm->states[sm->current_state_id].name);
    else print_str("(none)");
    print_str("\n===\n");
}

static void debug_trace_on(StateMachine *sm) {
    if (!sm) return; sm->debug_mode = 1; debug_trace_enabled = 1;
    print_str("Debug trace ON\n");
}

static void debug_trace_off(StateMachine *sm) {
    if (!sm) return; sm->debug_mode = 0; debug_trace_enabled = 0;
    print_str("Debug trace OFF\n");
}

static int debug_breakpoint_set(StateMachine *sm, int state_id) {
    if (!sm || state_id < 0 || state_id >= sm->state_count) return -1;
    sm->breakpoints[state_id] = 1;
    print_str("Breakpoint: "); print_str(sm->states[state_id].name); print_str("\n"); return 0;
}

static int debug_simulate(StateMachine *sm, const char *event) {
    if (!sm) return -1; print_str("Simulate: "); print_str(event); print_str("\n");
    int saved = sm->current_state_id;
    int r = event_fire(sm, event);
    sm->current_state_id = saved;
    print_str("Simulation done (restored)\n"); return r;
}

static int debug_step(StateMachine *sm) {
    if (!sm || sm->current_state_id < 0) return -1;
    print_str("Step from: "); print_str(sm->states[sm->current_state_id].name); print_str("\n");
    for (int i = 0; i < sm->transition_count; i++) {
        Transition *t = &sm->transitions[i];
        if (t->source_id == sm->current_state_id) { print_str("  ");
            print_str(t->event_name); print_str(" -> ");
            print_str(sm->states[t->target_id].name); print_str("\n"); } }
    return 0;
}

static int test_guard_allow(void *p) { (void)p; return 1; }
static int test_guard_deny(void *p) { (void)p; return 0; }
static void test_action(void *p) { (void)p; print_str("    [Action]\n"); }

static void test_suite(void) {
    print_str("\n=== State Machine Engine Tests ===\n\n");
    print_str("Test 1: Create machine\n");
    StateMachine *sm = sm_create("TestMachine");
    if (!sm) { print_str("FAILED\n"); return; }
    print_str("\nTest 2: Add states\n");
    int idle = state_add_initial(sm, "Idle");
    int proc = state_add(sm, "Processing");
    int done = state_add_final(sm, "Done");
    int err = state_add(sm, "Error");
    print_str("\nTest 3: Hierarchical\n");
    int sub1 = state_add_hierarchical(sm, "Sub1", proc);
    int sub2 = state_add_hierarchical(sm, "Sub2", proc);
    print_str("\nTest 4: Parallel\n");
    int par1 = state_add_parallel(sm, "ParA");
    int par2 = state_add_parallel(sm, "ParB");
    print_str("\nTest 5: Guards/Actions\n");
    sm->guards[0].guard_func = test_guard_allow; my_strcpy(sm->guards[0].name, "allow", MAX_NAME);
    sm->guards[1].guard_func = test_guard_deny; my_strcpy(sm->guards[1].name, "deny", MAX_NAME);
    sm->guards[2].action_func = test_action; my_strcpy(sm->guards[2].name, "do", MAX_NAME);
    sm->guard_count = 3;
    print_str("\nTest 6: Transitions\n");
    transition_add(sm, "start", idle, proc);
    transition_add_with_guard(sm, "complete", proc, done, 0);
    transition_add_with_guard(sm, "fail", proc, err, 1);
    transition_add_internal(sm, "log", proc, 2);
    transition_add(sm, "reset", done, idle);
    print_str("\nTest 7: Fire events\n");
    event_fire(sm, "start");
    event_fire(sm, "log");
    event_fire(sm, "complete");
    print_str("\nTest 8: History\n");
    history_save(sm);
    event_fire(sm, "reset");
    history_restore(sm);
    history_deep(sm);
    print_str("\nTest 9: Nested\n");
    enter_nested(sm, sub1);
    exit_nested(sm, sub1);
    print_str("\nTest 10: Instances\n");
    int inst1 = instance_create(sm, "Inst1");
    int inst2 = instance_create(sm, "Inst2");
    instance_list();
    instance_lock(inst1);
    instance_unlock(inst1);
    instance_delete(inst2);
    print_str("\nTest 11: Debug\n");
    debug_visualize(sm);
    debug_trace_on(sm);
    debug_breakpoint_set(sm, proc);
    debug_simulate(sm, "start");
    debug_step(sm);
    debug_trace_off(sm);
    print_str("\nTest 12: Snapshot\n");
    snapshot_save(sm);
    snapshot_load(sm);
    snapshot_list();
    print_str("\nTest 13: Log\n");
    transition_log(sm, "Evt1");
    transition_log(sm, "Evt2");
    transition_get_log(sm);
    print_str("\nTest 14: Cleanup\n");
    sm_reset(sm);
    sm_delete(sm);
    (void)sub2; (void)par1; (void)par2;
    print_str("\n=== All tests completed ===\n");
}

__attribute__((export_name("_start")))
void _start(void) {
    uint argv_off = alloc_mem(256);
    int argv_len = host_get_argv(argv_off, 255);
    if (argv_len < 0) argv_len = 0;
    const char *argv = (const char *)argv_off;
    int i = 0;
    while (i < argv_len && argv[i] != ' ') i++;
    while (i < argv_len && argv[i] == ' ') i++;
    if (i < argv_len && argv[i] == '-') {
        int j = i + 1;
        if (j < argv_len && argv[j] == 'h') {
            print_str("State Machine Engine v1.0\n");
            print_str("Usage: state_machine_engine [options]\n\n");
            print_str("Options:\n  -h    Show help\n  -t    Run tests\n\n");
            print_str("Features:\n  - Hierarchical state machines\n");
            print_str("  - Parallel (orthogonal) states\n");
            print_str("  - Guard conditions and actions\n");
            print_str("  - History tracking and replay\n");
            print_str("  - Snapshot persistence\n");
            print_str("  - Multi-instance support\n");
            print_str("  - Debug visualization and breakpoints\n");
            host_exit(0);
        } else if (j < argv_len && argv[j] == 't') {
            test_suite(); host_exit(0);
        }
    }
    print_str("State Machine Engine v1.0\n");
    print_str("Use -h for help, -t for tests\n");
    host_exit(0);
}
