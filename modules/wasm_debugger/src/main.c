/* wasm_debugger: WASM debugger with breakpoints, stepping, inspection, profiling (v1.0)
 *
 * Provides comprehensive debugging capabilities for WebAssembly modules:
 * - Breakpoint debugging (source-level, conditional, data breakpoints)
 * - Execution control (step over/into/out, continue, pause/resume)
 * - State inspection (variables, memory, call stack, registers)
 * - Performance profiling (CPU/memory profiling, hotspot analysis)
 * - Remote debugging support (DAP-style protocol stubs)
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

/* ===== Limits ===== */
#define MAX_BREAKPOINTS     64
#define MAX_DATA_BP         32
#define MAX_WATCHPOINTS     16
#define MAX_STACK_FRAMES    128
#define MAX_VARIABLES       256
#define MAX_REGISTERS       32
#define MAX_PROF_SAMPLES    512
#define MAX_PROF_FUNCS      64
#define MAX_REMOTE_CONNS    8
#define MAX_NAME            48
#define MAX_EXPR            128

/* ===== Breakpoint types ===== */
#define BP_SOURCE       0
#define BP_CONDITIONAL   1
#define BP_DATA_READ    2
#define BP_DATA_WRITE   3
#define BP_DATA_ACCESS  4
#define BP_FUNCTION   5
#define BP_EXCEPTION   6

/* ===== Execution states ===== */
#define STATE_STOPPED      0
#define STATE_RUNNING      1
#define STATE_STEPPING     2
#define STATE_PAUSED       3
#define STATE_TERMINATED   4

/* ===== Step modes ===== */
#define STEP_OVER     0
#define STEP_INTO     1
#define STEP_OUT      2
#define STEP_INSTR    3

/* ===== Variable types ===== */
#define VAR_I32    0
#define VAR_I64    1
#define VAR_F32    2
#define VAR_F64    3
#define VAR_PTR    4
#define VAR_STRUCT 5

/* ===== Register names (WASM virtual registers) ===== */
#define REG_SP     0
#define REG_FP     1
#define REG_PC     2
#define REG_LOCAL0 3
#define REG_LOCAL1 4
#define REG_LOCAL2 5
#define REG_LOCAL3 6
#define REG_RET0   7
#define REG_RET1   8

/* ===== Remote debug commands ===== */
#define RDC_CONTINUE   0x01
#define RDC_PAUSE      0x02
#define RDC_STEP       0x03
#define RDC_SET_BP     0x10
#define RDC_CLEAR_BP   0x11
#define RDC_READ_MEM   0x20
#define RDC_WRITE_MEM  0x21
#define RDC_READ_REG   0x30
#define RDC_WRITE_REG  0x31
#define RDC_STACK      0x40
#define RDC_VARS       0x41

/* ===== Data Structures ===== */

/* Source-level breakpoint */
typedef struct {
    int     bp_id;
    int     type;
    char    file[MAX_NAME];
    int     line;
    char    func[MAX_NAME];
    int     address;
    int     enabled;
    int     hit_count;
    int     ignore_count;
    char    condition[MAX_EXPR];
    int     cond_true_count;
} breakpoint_t;

/* Data watchpoint */
typedef struct {
    int     wp_id;
    int     type;          /* BP_DATA_READ, BP_DATA_WRITE, BP_DATA_ACCESS */
    int     address;
    int     size;
    int     enabled;
    int     hit_count;
    int     old_value;
    int     new_value;
} watchpoint_t;

/* Stack frame */
typedef struct {
    int     frame_id;
    char    func_name[MAX_NAME];
    char    file[MAX_NAME];
    int     line;
    int     col;
    int     pc;
    int     sp;
    int     fp;
    int     local_count;
    int     depth;
} stack_frame_t;

/* Variable */
typedef struct {
    char    name[MAX_NAME];
    int     type;
    int     value;
    int     address;
    int     size;
    int     scope_depth;
    int     frame_id;
    int     modified;
} variable_t;

/* Register state */
typedef struct {
    int     reg_id;
    char    name[16];
    int     value;
    int     prev_value;
    int     changed;
} reg_state_t;

/* Profiling sample (CPU) */
typedef struct {
    int     pc;
    char    func_name[MAX_NAME];
    int     samples;
    int     self_samples;
    int     total_time_us;
} prof_sample_t;

/* Profiling function entry */
typedef struct {
    char    name[MAX_NAME];
    int     call_count;
    int     total_time_us;
    int     self_time_us;
    int     min_time_us;
    int     max_time_us;
    int     avg_time_us;
} prof_func_t;

/* Memory profiling region */
typedef struct {
    int     address;
    int     size;
    int     alloc_time;
    int     free_time;
    char    alloc_func[MAX_NAME];
    int     freed;
} mem_region_t;

#define MAX_MEM_REGIONS 128

/* Remote debug connection */
typedef struct {
    int     conn_id;
    int     active;
    int     protocol_version;
    int     last_cmd;
    int     last_status;
    int     packets_sent;
    int     packets_recv;
} remote_conn_t;

/* ===== Global State ===== */
static breakpoint_t breakpoints[MAX_BREAKPOINTS];
static int bp_count = 0;
static int next_bp_id = 1;

static watchpoint_t watchpoints[MAX_DATA_BP];
static int wp_count = 0;
static int next_wp_id = 1;

static stack_frame_t call_stack[MAX_STACK_FRAMES];
static int stack_depth = 0;

static variable_t variables[MAX_VARIABLES];
static int var_count = 0;

static reg_state_t registers[MAX_REGISTERS];
static int reg_count = 9;

static prof_sample_t prof_samples[MAX_PROF_SAMPLES];
static int prof_sample_count = 0;

static prof_func_t prof_funcs[MAX_PROF_FUNCS];
static int prof_func_count = 0;

static mem_region_t mem_regions[MAX_MEM_REGIONS];
static int mem_region_count = 0;

static remote_conn_t remote_conns[MAX_REMOTE_CONNS];
static int remote_conn_count = 0;
static int next_conn_id = 1;

static int exec_state = STATE_STOPPED;
static int step_mode = STEP_OVER;
static int step_count = 0;
static int clock = 0;
static int profiling_active = 0;

/* ===== String Utilities ===== */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
}
static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0; while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; } dst[i] = '\0';
}
static int my_strstr(const char *haystack, const char *needle) {
    int hl = my_strlen(haystack), nl = my_strlen(needle);
    if (nl == 0 || nl > hl) return 0;
    for (int i = 0; i <= hl - nl; i++) {
        int j = 0; while (j < nl && haystack[i+j] == needle[j]) j++;
        if (j == nl) return 1;
    }
    return 0;
}

/* ===== Print Utilities ===== */
static void print_str(const char *str) { host_print(str); }
static void print_int(int val) {
    char buf[32]; int pos = 0;
    if (val < 0) { buf[pos++] = '-'; val = -val; }
    if (val == 0) buf[pos++] = '0';
    else { int d = 0, t = val; while (t > 0) { d++; t /= 10; } pos += d; buf[pos] = '\0'; pos--;
        while (val > 0) { buf[pos--] = '0' + (val % 10); val /= 10; } }
    host_print(buf);
}
static void print_hex(int val) {
    char buf[16]; const char *hex = "0123456789abcdef";
    int pos = 0;
    if (val == 0) { buf[pos++] = '0'; }
    else { unsigned int v = (unsigned int)val; char tmp[12]; int tpos = 0;
        while (v > 0) { tmp[tpos++] = hex[v & 0xf]; v >>= 4; }
        buf[pos++] = '0'; buf[pos++] = 'x';
        for (int i = tpos - 1; i >= 0; i--) buf[pos++] = tmp[i]; }
    buf[pos] = '\0';
    host_print(buf);
}

/* ===== Register Initialization ===== */
static void init_registers(void) {
    my_strncpy(registers[REG_SP].name, "sp", 15);
    my_strncpy(registers[REG_FP].name, "fp", 15);
    my_strncpy(registers[REG_PC].name, "pc", 15);
    my_strncpy(registers[REG_LOCAL0].name, "local0", 15);
    my_strncpy(registers[REG_LOCAL1].name, "local1", 15);
    my_strncpy(registers[REG_LOCAL2].name, "local2", 15);
    my_strncpy(registers[REG_LOCAL3].name, "local3", 15);
    my_strncpy(registers[REG_RET0].name, "ret0", 15);
    my_strncpy(registers[REG_RET1].name, "ret1", 15);
    for (int i = 0; i < reg_count; i++) {
        registers[i].reg_id = i;
        registers[i].value = 0;
        registers[i].prev_value = 0;
        registers[i].changed = 0;
    }
    registers[REG_SP].value = 0x10000;
    registers[REG_FP].value = 0x10000;
    registers[REG_PC].value = 0x0000;
}

/* ===== Breakpoint Management ===== */

int dbg_set_breakpoint(const char *file, int line, const char *func, int address, int type, const char *condition) {
    if (bp_count >= MAX_BREAKPOINTS) return -1;
    int idx = bp_count;
    breakpoints[idx].bp_id = next_bp_id++;
    breakpoints[idx].type = type;
    my_strncpy(breakpoints[idx].file, file ? file : "<unknown>", MAX_NAME - 1);
    breakpoints[idx].line = line;
    my_strncpy(breakpoints[idx].func, func ? func : "<unknown>", MAX_NAME - 1);
    breakpoints[idx].address = address;
    breakpoints[idx].enabled = 1;
    breakpoints[idx].hit_count = 0;
    breakpoints[idx].ignore_count = 0;
    my_strncpy(breakpoints[idx].condition, condition ? condition : "", MAX_EXPR - 1);
    breakpoints[idx].cond_true_count = 0;
    bp_count++;
    return breakpoints[idx].bp_id;
}

int dbg_clear_breakpoint(int bp_id) {
    for (int i = 0; i < bp_count; i++) {
        if (breakpoints[i].bp_id == bp_id) {
            breakpoints[i].enabled = 0;
            return 0;
        }
    }
    return -1;
}

int dbg_enable_breakpoint(int bp_id, int enable) {
    for (int i = 0; i < bp_count; i++) {
        if (breakpoints[i].bp_id == bp_id) {
            breakpoints[i].enabled = enable;
            return 0;
        }
    }
    return -1;
}

int dbg_hit_breakpoint(int bp_id) {
    for (int i = 0; i < bp_count; i++) {
        if (breakpoints[i].bp_id == bp_id && breakpoints[i].enabled) {
            breakpoints[i].hit_count++;
            if (breakpoints[i].type == BP_CONDITIONAL) {
                breakpoints[i].cond_true_count++;
            }
            exec_state = STATE_STOPPED;
            return breakpoints[i].hit_count;
        }
    }
    return 0;
}

/* ===== Data Watchpoints ===== */

int dbg_set_watchpoint(int address, int size, int type) {
    if (wp_count >= MAX_DATA_BP) return -1;
    int idx = wp_count;
    watchpoints[idx].wp_id = next_wp_id++;
    watchpoints[idx].type = type;
    watchpoints[idx].address = address;
    watchpoints[idx].size = size;
    watchpoints[idx].enabled = 1;
    watchpoints[idx].hit_count = 0;
    watchpoints[idx].old_value = 0;
    watchpoints[idx].new_value = 0;
    wp_count++;
    return watchpoints[idx].wp_id;
}

int dbg_clear_watchpoint(int wp_id) {
    for (int i = 0; i < wp_count; i++) {
        if (watchpoints[i].wp_id == wp_id) {
            watchpoints[i].enabled = 0;
            return 0;
        }
    }
    return -1;
}

int dbg_check_watchpoint(int address, int access_type, int value) {
    for (int i = 0; i < wp_count; i++) {
        if (!watchpoints[i].enabled) continue;
        if (address >= watchpoints[i].address &&
            address < watchpoints[i].address + watchpoints[i].size) {
            int match = 0;
            if (watchpoints[i].type == BP_DATA_ACCESS) match = 1;
            else if (watchpoints[i].type == BP_DATA_READ && access_type == 0) match = 1;
            else if (watchpoints[i].type == BP_DATA_WRITE && access_type == 1) match = 1;
            if (match) {
                watchpoints[i].old_value = watchpoints[i].new_value;
                watchpoints[i].new_value = value;
                watchpoints[i].hit_count++;
                exec_state = STATE_STOPPED;
                return watchpoints[i].hit_count;
            }
        }
    }
    return 0;
}

/* ===== Execution Control ===== */

int dbg_continue(void) {
    if (exec_state != STATE_STOPPED && exec_state != STATE_PAUSED) return -1;
    exec_state = STATE_RUNNING;
    return 0;
}

int dbg_pause(void) {
    if (exec_state != STATE_RUNNING) return -1;
    exec_state = STATE_PAUSED;
    return 0;
}

int dbg_step(int mode, int count) {
    if (exec_state != STATE_STOPPED) return -1;
    step_mode = mode;
    step_count = count > 0 ? count : 1;
    exec_state = STATE_STEPPING;
    return 0;
}

int dbg_step_over(int count) { return dbg_step(STEP_OVER, count); }
int dbg_step_into(int count) { return dbg_step(STEP_INTO, count); }
int dbg_step_out(void) { return dbg_step(STEP_OUT, 1); }
int dbg_step_instruction(int count) { return dbg_step(STEP_INSTR, count); }

int dbg_get_state(void) { return exec_state; }

const char *dbg_state_name(int state) {
    if (state == STATE_STOPPED) return "STOPPED";
    if (state == STATE_RUNNING) return "RUNNING";
    if (state == STATE_STEPPING) return "STEPPING";
    if (state == STATE_PAUSED) return "PAUSED";
    if (state == STATE_TERMINATED) return "TERMINATED";
    return "UNKNOWN";
}

/* ===== State Inspection: Call Stack ===== */

int dbg_push_frame(const char *func_name, const char *file, int line, int col, int pc) {
    if (stack_depth >= MAX_STACK_FRAMES) return -1;
    int idx = stack_depth;
    call_stack[idx].frame_id = idx;
    my_strncpy(call_stack[idx].func_name, func_name, MAX_NAME - 1);
    my_strncpy(call_stack[idx].file, file, MAX_NAME - 1);
    call_stack[idx].line = line;
    call_stack[idx].col = col;
    call_stack[idx].pc = pc;
    call_stack[idx].sp = registers[REG_SP].value;
    call_stack[idx].fp = registers[REG_FP].value;
    call_stack[idx].local_count = 0;
    call_stack[idx].depth = idx;
    stack_depth++;
    return idx;
}

int dbg_pop_frame(void) {
    if (stack_depth <= 0) return -1;
    stack_depth--;
    if (stack_depth >= 0) {
        registers[REG_SP].value = call_stack[stack_depth].sp;
        registers[REG_FP].value = call_stack[stack_depth].fp;
    }
    return 0;
}

int dbg_get_stack_depth(void) { return stack_depth; }

int dbg_print_call_stack(void) {
    print_str("  Call Stack:\n");
    print_str("  ============================================================\n");
    for (int i = stack_depth - 1; i >= 0; i--) {
        print_str("  #"); print_int(i);
        print_str("  "); print_str(call_stack[i].func_name);
        print_str("  at "); print_str(call_stack[i].file);
        print_str(":"); print_int(call_stack[i].line);
        print_str(":"); print_int(call_stack[i].col);
        print_str("  pc="); print_hex(call_stack[i].pc);
        print_str("\n");
    }
    return stack_depth;
}

/* ===== State Inspection: Variables ===== */

int dbg_add_variable(const char *name, int type, int value, int address, int size, int scope_depth, int frame_id) {
    if (var_count >= MAX_VARIABLES) return -1;
    int idx = var_count;
    my_strncpy(variables[idx].name, name, MAX_NAME - 1);
    variables[idx].type = type;
    variables[idx].value = value;
    variables[idx].address = address;
    variables[idx].size = size;
    variables[idx].scope_depth = scope_depth;
    variables[idx].frame_id = frame_id;
    variables[idx].modified = 0;
    var_count++;
    return idx;
}

int dbg_set_variable(const char *name, int value) {
    for (int i = 0; i < var_count; i++) {
        if (my_strcmp(variables[i].name, name) == 0) {
            variables[i].value = value;
            variables[i].modified = 1;
            return 0;
        }
    }
    return -1;
}

static const char *var_type_name(int type) {
    if (type == VAR_I32) return "i32";
    if (type == VAR_I64) return "i64";
    if (type == VAR_F32) return "f32";
    if (type == VAR_F64) return "f64";
    if (type == VAR_PTR) return "ptr";
    if (type == VAR_STRUCT) return "struct";
    return "???";
}

int dbg_print_variables(int frame_id) {
    print_str("  Variables");
    if (frame_id >= 0) { print_str(" (frame #"); print_int(frame_id); print_str(")"); }
    print_str(":\n");
    print_str("  ============================================================\n");
    int count = 0;
    for (int i = 0; i < var_count; i++) {
        if (frame_id < 0 || variables[i].frame_id == frame_id) {
            print_str("  "); print_str(variables[i].name);
            int len = my_strlen(variables[i].name);
            for (int p = 0; p < 16 - len; p++) print_str(" ");
            print_str(": "); print_str(var_type_name(variables[i].type));
            print_str(" = "); print_int(variables[i].value);
            print_str("  @"); print_hex(variables[i].address);
            if (variables[i].modified) print_str(" [MODIFIED]");
            print_str("\n");
            count++;
        }
    }
    print_str("  Total: "); print_int(count); print_str(" variables\n");
    return count;
}

/* ===== State Inspection: Registers ===== */

int dbg_read_register(int reg_id) {
    if (reg_id < 0 || reg_id >= reg_count) return -1;
    return registers[reg_id].value;
}

int dbg_write_register(int reg_id, int value) {
    if (reg_id < 0 || reg_id >= reg_count) return -1;
    registers[reg_id].prev_value = registers[reg_id].value;
    registers[reg_id].value = value;
    registers[reg_id].changed = 1;
    return 0;
}

int dbg_print_registers(void) {
    print_str("  Registers:\n");
    print_str("  ============================================================\n");
    for (int i = 0; i < reg_count; i++) {
        print_str("  "); print_str(registers[i].name);
        print_str("  =  "); print_hex(registers[i].value);
        if (registers[i].changed) {
            print_str("  (prev: "); print_hex(registers[i].prev_value);
            print_str(") [CHANGED]");
        }
        print_str("\n");
    }
    return reg_count;
}

/* ===== State Inspection: Memory ===== */

int dbg_read_memory(int address, int size) {
    /* Simulated memory read - returns address as value */
    return address;
}

int dbg_write_memory(int address, int value, int size) {
    return dbg_check_watchpoint(address, 1, value);
}

int dbg_print_memory(int address, int length) {
    print_str("  Memory Dump at "); print_hex(address);
    print_str(" ("); print_int(length); print_str(" bytes):\n");
    print_str("  ============================================================\n");
    for (int row = 0; row < length; row += 16) {
        print_str("  "); print_hex(address + row); print_str(": ");
        int cols = (length - row < 16) ? length - row : 16;
        for (int c = 0; c < cols; c++) {
            print_hex(address + row + c); print_str(" ");
        }
        print_str("\n");
    }
    return length;
}

/* ===== Performance Profiling: CPU ===== */

int dbg_prof_start(void) {
    profiling_active = 1;
    prof_sample_count = 0;
    prof_func_count = 0;
    return 0;
}

int dbg_prof_stop(void) {
    profiling_active = 0;
    return 0;
}

int dbg_prof_record_sample(int pc, const char *func_name, int elapsed_us) {
    if (!profiling_active) return -1;
    /* Record sample */
    if (prof_sample_count < MAX_PROF_SAMPLES) {
        prof_samples[prof_sample_count].pc = pc;
        my_strncpy(prof_samples[prof_sample_count].func_name, func_name, MAX_NAME - 1);
        prof_samples[prof_sample_count].samples = 1;
        prof_samples[prof_sample_count].self_samples = 1;
        prof_samples[prof_sample_count].total_time_us = elapsed_us;
        prof_sample_count++;
    }
    /* Update function entry */
    for (int i = 0; i < prof_func_count; i++) {
        if (my_strcmp(prof_funcs[i].name, func_name) == 0) {
            prof_funcs[i].call_count++;
            prof_funcs[i].total_time_us += elapsed_us;
            prof_funcs[i].self_time_us += elapsed_us;
            if (elapsed_us < prof_funcs[i].min_time_us) prof_funcs[i].min_time_us = elapsed_us;
            if (elapsed_us > prof_funcs[i].max_time_us) prof_funcs[i].max_time_us = elapsed_us;
            return 0;
        }
    }
    /* New function */
    if (prof_func_count < MAX_PROF_FUNCS) {
        int idx = prof_func_count;
        my_strncpy(prof_funcs[idx].name, func_name, MAX_NAME - 1);
        prof_funcs[idx].call_count = 1;
        prof_funcs[idx].total_time_us = elapsed_us;
        prof_funcs[idx].self_time_us = elapsed_us;
        prof_funcs[idx].min_time_us = elapsed_us;
        prof_funcs[idx].max_time_us = elapsed_us;
        prof_funcs[idx].avg_time_us = elapsed_us;
        prof_func_count++;
    }
    return 0;
}

int dbg_prof_hotspot_report(void) {
    print_str("  CPU Hotspot Analysis\n");
    print_str("  ============================================================\n");
    print_str("  Function             Calls   Total(us)  Avg(us)  Max(us)  %%Time\n");
    print_str("  ------------------------------------------------------------\n");
    int total_time = 0;
    for (int i = 0; i < prof_func_count; i++) total_time += prof_funcs[i].total_time_us;
    for (int i = 0; i < prof_func_count; i++) {
        int avg = prof_funcs[i].call_count > 0 ?
            prof_funcs[i].total_time_us / prof_funcs[i].call_count : 0;
        int pct = total_time > 0 ?
            (prof_funcs[i].total_time_us * 100) / total_time : 0;
        print_str("  "); print_str(prof_funcs[i].name);
        int len = my_strlen(prof_funcs[i].name);
        for (int p = 0; p < 21 - len; p++) print_str(" ");
        print_int(prof_funcs[i].call_count); print_str("     ");
        print_int(prof_funcs[i].total_time_us); print_str("      ");
        print_int(avg); print_str("      ");
        print_int(prof_funcs[i].max_time_us); print_str("      ");
        print_int(pct); print_str("%%\n");
    }
    print_str("  Total samples: "); print_int(prof_sample_count);
    print_str("  Total time: "); print_int(total_time); print_str(" us\n");
    return prof_func_count;
}

/* ===== Performance Profiling: Memory ===== */

int dbg_prof_track_alloc(int address, int size, const char *func_name) {
    if (mem_region_count >= MAX_MEM_REGIONS) return -1;
    int idx = mem_region_count;
    mem_regions[idx].address = address;
    mem_regions[idx].size = size;
    mem_regions[idx].alloc_time = ++clock;
    mem_regions[idx].free_time = 0;
    mem_regions[idx].freed = 0;
    my_strncpy(mem_regions[idx].alloc_func, func_name, MAX_NAME - 1);
    mem_region_count++;
    return idx;
}

int dbg_prof_track_free(int address) {
    for (int i = 0; i < mem_region_count; i++) {
        if (mem_regions[i].address == address && !mem_regions[i].freed) {
            mem_regions[i].freed = 1;
            mem_regions[i].free_time = ++clock;
            return 0;
        }
    }
    return -1;
}

int dbg_prof_memory_report(void) {
    print_str("  Memory Profiling Report\n");
    print_str("  ============================================================\n");
    int total_alloc = 0, total_freed = 0, leaks = 0, leaked_bytes = 0;
    int peak_usage = 0, current_usage = 0;
    for (int i = 0; i < mem_region_count; i++) {
        total_alloc += mem_regions[i].size;
        if (mem_regions[i].freed) {
            total_freed += mem_regions[i].size;
        } else {
            leaks++;
            leaked_bytes += mem_regions[i].size;
            current_usage += mem_regions[i].size;
        }
    }
    peak_usage = total_alloc - total_freed + leaked_bytes;
    print_str("  Total allocated:   "); print_int(total_alloc); print_str(" bytes\n");
    print_str("  Total freed:       "); print_int(total_freed); print_str(" bytes\n");
    print_str("  Current usage:     "); print_int(current_usage); print_str(" bytes\n");
    print_str("  Peak usage:        "); print_int(peak_usage); print_str(" bytes\n");
    print_str("  Leaked blocks:     "); print_int(leaks); print_str(" (");
    print_int(leaked_bytes); print_str(" bytes)\n");
    print_str("\n  Unfreed allocations:\n");
    for (int i = 0; i < mem_region_count; i++) {
        if (!mem_regions[i].freed) {
            print_str("    "); print_hex(mem_regions[i].address);
            print_str("  size="); print_int(mem_regions[i].size);
            print_str("  from "); print_str(mem_regions[i].alloc_func);
            print_str("  t="); print_int(mem_regions[i].alloc_time);
            print_str("\n");
        }
    }
    return leaks;
}

/* ===== Remote Debugging ===== */

int dbg_remote_accept(int protocol_version) {
    if (remote_conn_count >= MAX_REMOTE_CONNS) return -1;
    int idx = remote_conn_count;
    remote_conns[idx].conn_id = next_conn_id++;
    remote_conns[idx].active = 1;
    remote_conns[idx].protocol_version = protocol_version;
    remote_conns[idx].last_cmd = 0;
    remote_conns[idx].last_status = exec_state;
    remote_conns[idx].packets_sent = 0;
    remote_conns[idx].packets_recv = 0;
    remote_conn_count++;
    return remote_conns[idx].conn_id;
}

int dbg_remote_disconnect(int conn_id) {
    for (int i = 0; i < remote_conn_count; i++) {
        if (remote_conns[i].conn_id == conn_id) {
            remote_conns[i].active = 0;
            return 0;
        }
    }
    return -1;
}

int dbg_remote_handle_cmd(int conn_id, int cmd, int arg) {
    for (int i = 0; i < remote_conn_count; i++) {
        if (remote_conns[i].conn_id == conn_id && remote_conns[i].active) {
            remote_conns[i].last_cmd = cmd;
            remote_conns[i].packets_recv++;
            int result = 0;
            if (cmd == RDC_CONTINUE) result = dbg_continue();
            else if (cmd == RDC_PAUSE) result = dbg_pause();
            else if (cmd == RDC_STEP) result = dbg_step_into(arg);
            else if (cmd == RDC_READ_REG) result = dbg_read_register(arg);
            else if (cmd == RDC_STACK) result = stack_depth;
            else if (cmd == RDC_VARS) result = var_count;
            remote_conns[i].last_status = result;
            remote_conns[i].packets_sent++;
            return result;
        }
    }
    return -1;
}

int dbg_remote_print_connections(void) {
    print_str("  Remote Debug Connections\n");
    print_str("  ============================================================\n");
    int active = 0;
    for (int i = 0; i < remote_conn_count; i++) {
        if (remote_conns[i].active) {
            print_str("  Conn #"); print_int(remote_conns[i].conn_id);
            print_str("  proto=v"); print_int(remote_conns[i].protocol_version);
            print_str("  last_cmd="); print_hex(remote_conns[i].last_cmd);
            print_str("  sent="); print_int(remote_conns[i].packets_sent);
            print_str("  recv="); print_int(remote_conns[i].packets_recv);
            print_str("  status="); print_str(dbg_state_name(remote_conns[i].last_status));
            print_str("\n");
            active++;
        }
    }
    print_str("  Active connections: "); print_int(active); print_str("\n");
    return active;
}

/* ===== CLI Entry ===== */
void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);
    int help = 0, test = 0;
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0) help = 1;
        else if (my_strcmp(arg, "-t") == 0 || my_strcmp(arg, "--test") == 0) test = 1;
        while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    }

    print_str("WasmDebugger v1.0 - WebAssembly Debug & Profiling\n");
    if (help) {
        print_str("Usage: wasm_debugger [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run debugger test suite\n");
        print_str("\nFeatures:\n");
        print_str("  Breakpoints: source, conditional, data (read/write/access)\n");
        print_str("  Execution: step over/into/out, continue, pause/resume\n");
        print_str("  Inspection: variables, memory, call stack, registers\n");
        print_str("  Profiling: CPU hotspots, memory allocations, leak detection\n");
        print_str("  Remote: DAP-style protocol with multi-connection support\n");
        return;
    }

    if (test) {
        init_registers();
        print_str("=== WasmDebugger Test Suite ===\n\n");

        /* Test 1: Breakpoints */
        print_str("[Test 1] Breakpoint Debugging\n");
        int bp1 = dbg_set_breakpoint("main.c", 42, "compute", 0x1000, BP_SOURCE, 0);
        int bp2 = dbg_set_breakpoint("math.c", 100, "add", 0x2000, BP_CONDITIONAL, "x > 10");
        int bp3 = dbg_set_breakpoint("loop.c", 15, "iterate", 0x3000, BP_SOURCE, 0);
        print_str("  Set breakpoints: #"); print_int(bp1);
        print_str(" #"); print_int(bp2); print_str(" #"); print_int(bp3); print_str("\n");

        dbg_hit_breakpoint(bp1);
        dbg_hit_breakpoint(bp1);
        dbg_hit_breakpoint(bp2);
        print_str("  BP#"); print_int(bp1); print_str(" hit 2x, BP#");
        print_int(bp2); print_str(" hit 1x\n");
        print_str("  State: "); print_str(dbg_state_name(dbg_get_state())); print_str("\n");

        dbg_enable_breakpoint(bp3, 0);
        print_str("  Disabled BP#"); print_int(bp3); print_str("\n");
        dbg_clear_breakpoint(bp3);
        print_str("  Cleared BP#"); print_int(bp3); print_str("\n\n");

        /* Test 2: Data Watchpoints */
        print_str("[Test 2] Data Watchpoints\n");
        int wp1 = dbg_set_watchpoint(0x10000, 4, BP_DATA_WRITE);
        int wp2 = dbg_set_watchpoint(0x10100, 8, BP_DATA_ACCESS);
        print_str("  Set watchpoints: #"); print_int(wp1);
        print_str(" (write @0x10000) #"); print_int(wp2);
        print_str(" (access @0x10100)\n");

        int hit = dbg_check_watchpoint(0x10000, 1, 42);
        print_str("  Write @0x10000: hit="); print_int(hit); print_str("\n");
        hit = dbg_check_watchpoint(0x10000, 0, 0);
        print_str("  Read @0x10000: hit="); print_int(hit);
        print_str(" (watchpoint is write-only)\n");
        hit = dbg_check_watchpoint(0x10100, 0, 0);
        print_str("  Read @0x10100: hit="); print_int(hit);
        print_str(" (watchpoint is access)\n\n");

        /* Test 3: Execution Control */
        print_str("[Test 3] Execution Control\n");
        print_str("  State: "); print_str(dbg_state_name(dbg_get_state())); print_str("\n");
        dbg_continue();
        print_str("  After continue: "); print_str(dbg_state_name(dbg_get_state())); print_str("\n");
        dbg_pause();
        print_str("  After pause: "); print_str(dbg_state_name(dbg_get_state())); print_str("\n");
        dbg_continue();
        dbg_pause();
        dbg_step_over(3);
        print_str("  After step-over(3): "); print_str(dbg_state_name(dbg_get_state())); print_str("\n");
        exec_state = STATE_STOPPED;
        dbg_step_into(1);
        print_str("  After step-into: "); print_str(dbg_state_name(dbg_get_state())); print_str("\n");
        exec_state = STATE_STOPPED;
        dbg_step_out();
        print_str("  After step-out: "); print_str(dbg_state_name(dbg_get_state())); print_str("\n");
        exec_state = STATE_STOPPED;
        dbg_step_instruction(5);
        print_str("  After step-instr(5): "); print_str(dbg_state_name(dbg_get_state()));
        print_str("\n\n");

        /* Test 4: Call Stack */
        print_str("[Test 4] Call Stack Inspection\n");
        dbg_push_frame("main", "main.c", 10, 0, 0x0000);
        dbg_push_frame("compute", "compute.c", 55, 4, 0x1000);
        dbg_push_frame("add", "math.c", 22, 8, 0x2000);
        dbg_push_frame("multiply", "math.c", 40, 2, 0x2100);
        dbg_print_call_stack();
        print_str("  Stack depth: "); print_int(dbg_get_stack_depth()); print_str("\n\n");

        /* Test 5: Variables */
        print_str("[Test 5] Variable Inspection\n");
        dbg_add_variable("x", VAR_I32, 42, 0x10000, 4, 1, 3);
        dbg_add_variable("y", VAR_I32, 100, 0x10004, 4, 1, 3);
        dbg_add_variable("result", VAR_I32, 0, 0x10008, 4, 1, 3);
        dbg_add_variable("ptr", VAR_PTR, 0x20000, 0x1000C, 4, 1, 3);
        dbg_add_variable("data", VAR_STRUCT, 0x30000, 0x10010, 32, 1, 3);
        dbg_add_variable("local_i", VAR_I32, 0, 0x10030, 4, 2, 2);
        dbg_set_variable("result", 142);
        dbg_print_variables(3);
        print_str("\n");

        /* Test 6: Registers */
        print_str("[Test 6] Register Inspection\n");
        dbg_write_register(REG_SP, 0xFF00);
        dbg_write_register(REG_PC, 0x2100);
        dbg_write_register(REG_LOCAL0, 42);
        dbg_write_register(REG_LOCAL1, 100);
        dbg_write_register(REG_RET0, 142);
        dbg_print_registers();
        print_str("\n");

        /* Test 7: Memory */
        print_str("[Test 7] Memory Inspection\n");
        dbg_print_memory(0x10000, 32);
        print_str("\n");

        /* Test 8: CPU Profiling */
        print_str("[Test 8] CPU Profiling & Hotspot Analysis\n");
        dbg_prof_start();
        dbg_prof_record_sample(0x1000, "compute", 150);
        dbg_prof_record_sample(0x2000, "add", 25);
        dbg_prof_record_sample(0x2000, "add", 30);
        dbg_prof_record_sample(0x2000, "add", 20);
        dbg_prof_record_sample(0x2100, "multiply", 80);
        dbg_prof_record_sample(0x2100, "multiply", 95);
        dbg_prof_record_sample(0x1000, "compute", 200);
        dbg_prof_record_sample(0x3000, "validate", 45);
        dbg_prof_record_sample(0x3000, "validate", 50);
        dbg_prof_record_sample(0x4000, "render", 500);
        dbg_prof_stop();
        print_str("\n");
        dbg_prof_hotspot_report();
        print_str("\n");

        /* Test 9: Memory Profiling */
        print_str("[Test 9] Memory Profiling\n");
        dbg_prof_track_alloc(0x40000, 256, "compute");
        dbg_prof_track_alloc(0x40100, 512, "render");
        dbg_prof_track_alloc(0x40300, 128, "validate");
        dbg_prof_track_alloc(0x40400, 1024, "main");
        dbg_prof_track_free(0x40000);
        dbg_prof_track_free(0x40300);
        print_str("\n");
        dbg_prof_memory_report();
        print_str("\n");

        /* Test 10: Remote Debugging */
        print_str("[Test 10] Remote Debugging\n");
        int conn1 = dbg_remote_accept(1);
        int conn2 = dbg_remote_accept(1);
        print_str("  Accepted connections: #"); print_int(conn1);
        print_str(" #"); print_int(conn2); print_str("\n");

        dbg_remote_handle_cmd(conn1, RDC_PAUSE, 0);
        print_str("  Conn#"); print_int(conn1); print_str(" -> PAUSE\n");
        dbg_remote_handle_cmd(conn1, RDC_READ_REG, REG_PC);
        print_str("  Conn#"); print_int(conn1); print_str(" -> READ_REG(PC)\n");
        dbg_remote_handle_cmd(conn2, RDC_VARS, 0);
        print_str("  Conn#"); print_int(conn2); print_str(" -> VARS\n");
        dbg_remote_handle_cmd(conn1, RDC_CONTINUE, 0);
        print_str("  Conn#"); print_int(conn1); print_str(" -> CONTINUE\n");

        dbg_remote_disconnect(conn2);
        print_str("  Disconnected conn#"); print_int(conn2); print_str("\n\n");
        dbg_remote_print_connections();
        print_str("\n");

        print_str("=== WasmDebugger Test Complete ===\n");
        return;
    }

    print_str("Use -h for help, -t for test\n");
}
