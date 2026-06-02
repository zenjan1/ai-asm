/* general_solver: Universal problem solving engine (v35.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_PROBLEMS    64
#define MAX_SUBPROBLEMS 128
#define MAX_SOLUTIONS   128
#define MAX_STRATEGIES  16
#define MAX_NAME        64
#define MAX_DESC        128

/* Problem types */
#define PROB_MATH       0
#define PROB_LOGIC      1
#define PROB_PLANNING   2
#define PROB_DIAGNOSIS  3
#define PROB_DESIGN     4
#define PROB_OPTIMIZATION 5

/* Problem status */
#define PROB_PENDING    0
#define PROB_SOLVING    1
#define PROB_SOLVED     2
#define PROB_UNSOLVABLE 3

/* Strategy types */
#define STRAT_DECOMPOSE    0
#define STRAT_ANALOGY      1
#define STRAT_DEDUCTION    2
#define STRAT_INDUTION     3
#define STRAT_HEURISTIC    4

/* Problem */
typedef struct {
    int   prob_id;
    char  type[MAX_NAME];
    int   complexity;       /* 1-10 */
    char  description[MAX_DESC];
    int   status;
    int   strategy_used;
    int   subproblem_count;
} problem_t;

/* Subproblem */
typedef struct {
    int   sub_id;
    int   parent_id;
    int   status;
    int   solution_id;      /* -1 if not solved */
} subproblem_t;

/* Solution */
typedef struct {
    int   sol_id;
    int   prob_id;
    int   steps_taken;
    int   confidence;       /* 0-100 */
    int   verified;         /* 0/1 */
    int   duration_ms;
} solution_t;

/* Strategy */
typedef struct {
    int   strat_id;
    char  name[MAX_NAME];
    int   success_rate;     /* 0-100 */
    int   avg_time_ms;
    char  applicable_types[MAX_DESC];
} strategy_t;

static problem_t problems[MAX_PROBLEMS];
static int problem_count = 0;
static int next_prob_id = 1;

static subproblem_t subproblems[MAX_SUBPROBLEMS];
static int subproblem_count = 0;
static int next_sub_id = 1;

static solution_t solutions[MAX_SOLUTIONS];
static int solution_count = 0;
static int next_sol_id = 1;

static strategy_t strategies[MAX_STRATEGIES];
static int strategy_count = 0;
static int next_strat_id = 1;

/* String utilities */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
}
static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0; while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; } dst[i] = '\0';
}

/* Print utilities */
static void print_str(const char *str) { host_print(str); }
static void print_int(int val) {
    char buf[32]; int pos = 0;
    if (val < 0) { buf[pos++] = '-'; val = -val; }
    if (val == 0) buf[pos++] = '0';
    else { int d = 0, t = val; while (t > 0) { d++; t /= 10; } pos += d; buf[pos] = '\0'; pos--;
        while (val > 0) { buf[pos--] = '0' + (val % 10); val /= 10; } }
    host_print(buf);
}

static const char *prob_type_name(int t) {
    if (t == PROB_MATH) return "Math";
    if (t == PROB_LOGIC) return "Logic";
    if (t == PROB_PLANNING) return "Planning";
    if (t == PROB_DIAGNOSIS) return "Diagnosis";
    if (t == PROB_DESIGN) return "Design";
    return "Optimization";
}

static const char *prob_status_name(int s) {
    if (s == PROB_PENDING) return "Pending";
    if (s == PROB_SOLVING) return "Solving";
    if (s == PROB_SOLVED) return "Solved";
    return "Unsolvable";
}

static const char *strategy_name(int s) {
    if (s == STRAT_DECOMPOSE) return "Decomposition";
    if (s == STRAT_ANALOGY) return "Analogy";
    if (s == STRAT_DEDUCTION) return "Deduction";
    if (s == STRAT_INDUTION) return "Induction";
    return "Heuristic";
}

/* Register strategy */
int gs_register_strategy(const char *name, int success_rate, int avg_time, const char *types) {
    if (strategy_count >= MAX_STRATEGIES) return -1;
    int idx = strategy_count;
    strategies[idx].strat_id = next_strat_id++;
    my_strncpy(strategies[idx].name, name, MAX_NAME - 1);
    strategies[idx].success_rate = success_rate;
    strategies[idx].avg_time_ms = avg_time;
    my_strncpy(strategies[idx].applicable_types, types, MAX_DESC - 1);
    strategy_count++;
    return strategies[idx].strat_id;
}

/* Submit problem */
int gs_submit_problem(int type, int complexity, const char *description) {
    if (problem_count >= MAX_PROBLEMS) return -1;
    int idx = problem_count;
    problems[idx].prob_id = next_prob_id++;
    my_strncpy(problems[idx].type, prob_type_name(type), MAX_NAME - 1);
    problems[idx].complexity = complexity;
    my_strncpy(problems[idx].description, description, MAX_DESC - 1);
    problems[idx].status = PROB_PENDING;
    problems[idx].strategy_used = -1;
    problems[idx].subproblem_count = 0;
    problem_count++;
    return problems[idx].prob_id;
}

/* Decompose problem into subproblems */
int gs_decompose(int prob_id, int num_sub) {
    for (int i = 0; i < problem_count; i++) {
        if (problems[i].prob_id == prob_id) {
            problems[i].status = PROB_SOLVING;
            problems[i].subproblem_count = num_sub;
            print_str("  Decomposing problem #");
            print_int(prob_id);
            print_str(" into ");
            print_int(num_sub);
            print_str(" subproblems\n");
            for (int j = 0; j < num_sub && subproblem_count < MAX_SUBPROBLEMS; j++) {
                int sidx = subproblem_count;
                subproblems[sidx].sub_id = next_sub_id++;
                subproblems[sidx].parent_id = prob_id;
                subproblems[sidx].status = PROB_PENDING;
                subproblems[sidx].solution_id = -1;
                subproblem_count++;
            }
            return num_sub;
        }
    }
    return -1;
}

/* Select best strategy for problem type */
int gs_select_strategy(int prob_type) {
    int best = -1, best_rate = 0;
    for (int i = 0; i < strategy_count; i++) {
        /* Simple matching: check if type name appears in applicable_types */
        const char *tn = prob_type_name(prob_type);
        int found = 0;
        for (int j = 0; strategies[i].applicable_types[j]; j++) {
            if (strategies[i].applicable_types[j] == tn[0] &&
                (j == 0 || strategies[i].applicable_types[j-1] == ',')) {
                int k = 0;
                while (tn[k] && strategies[i].applicable_types[j+k] == tn[k]) k++;
                if (!tn[k] || strategies[i].applicable_types[j+k] == ',') found = 1;
            }
        }
        if (found && strategies[i].success_rate > best_rate) {
            best_rate = strategies[i].success_rate;
            best = i;
        }
    }
    /* Fallback to heuristic */
    if (best < 0) {
        for (int i = 0; i < strategy_count; i++) {
            if (my_strcmp(strategies[i].name, "Heuristic") == 0) { best = i; break; }
        }
    }
    return best >= 0 ? best + 1 : -1;
}

/* Solve problem */
int gs_solve(int prob_id) {
    for (int i = 0; i < problem_count; i++) {
        if (problems[i].prob_id == prob_id) {
            /* Select strategy */
            int prob_type = -1;
            for (int t = 0; t < 6; t++) {
                if (my_strcmp(problems[i].type, prob_type_name(t)) == 0) { prob_type = t; break; }
            }
            if (prob_type < 0) return -1;
            int strat_idx = gs_select_strategy(prob_type);
            if (strat_idx < 0) return -1;
            problems[i].strategy_used = strat_idx - 1;
            problems[i].status = PROB_SOLVED;
            /* Generate solution */
            if (solution_count < MAX_SOLUTIONS) {
                int sidx = solution_count;
                solutions[sidx].sol_id = next_sol_id++;
                solutions[sidx].prob_id = prob_id;
                solutions[sidx].steps_taken = problems[i].complexity * 3 + 2;
                /* Confidence based on strategy success rate and problem complexity */
                solutions[sidx].confidence = strategies[strat_idx - 1].success_rate - problems[i].complexity * 3;
                if (solutions[sidx].confidence > 100) solutions[sidx].confidence = 100;
                if (solutions[sidx].confidence < 0) solutions[sidx].confidence = 0;
                solutions[sidx].verified = solutions[sidx].confidence >= 70 ? 1 : 0;
                solutions[sidx].duration_ms = strategies[strat_idx - 1].avg_time_ms * problems[i].complexity / 5;
                solution_count++;
                /* Mark subproblems as solved */
                for (int j = 0; j < subproblem_count; j++) {
                    if (subproblems[j].parent_id == prob_id) {
                        subproblems[j].status = PROB_SOLVED;
                        subproblems[j].solution_id = solutions[sidx].sol_id;
                    }
                }
                print_str("  Problem #");
                print_int(prob_id);
                print_str(" solved via ");
                print_str(strategy_name(problems[i].strategy_used));
                print_str(" (confidence: ");
                print_int(solutions[sidx].confidence);
                print_str("%)\n");
            }
            return solutions[solution_count - 1].sol_id;
        }
    }
    return -1;
}

/* Problem report */
int gs_report(void) {
    print_str("  Problem Solving Report\n");
    print_str("  ========================================================================\n");
    print_str("  ID  Type         Complexity  Strategy       Status      Confidence\n");
    print_str("  ------------------------------------------------------------------------\n");
    for (int i = 0; i < problem_count; i++) {
        print_int(problems[i].prob_id);
        print_str("  ");
        print_str(problems[i].type);
        int len = my_strlen(problems[i].type);
        for (int p = 0; p < 13 - len; p++) print_str(" ");
        print_int(problems[i].complexity);
        print_str("         ");
        if (problems[i].strategy_used >= 0) {
            print_str(strategy_name(problems[i].strategy_used));
            len = my_strlen(strategy_name(problems[i].strategy_used));
        } else {
            print_str("None");
            len = 4;
        }
        for (int p = 0; p < 15 - len; p++) print_str(" ");
        print_str(prob_status_name(problems[i].status));
        if (problems[i].status == PROB_SOLVED) print_str("  ");
        else print_str("    ");
        /* Find solution for confidence */
        for (int j = 0; j < solution_count; j++) {
            if (solutions[j].prob_id == problems[i].prob_id) {
                print_int(solutions[j].confidence);
                print_str("%");
                if (solutions[j].verified) print_str(" [verified]");
                break;
            }
        }
        if (problems[i].status != PROB_SOLVED) print_str("-");
        print_str("\n");
    }
    print_str("  Total: ");
    print_int(problem_count);
    print_str(" problems, ");
    print_int(solution_count);
    print_str(" solved\n");
    return problem_count;
}

/* Strategy listing */
int gs_list_strategies(void) {
    print_str("  Strategy Library\n");
    print_str("  ========================================================================\n");
    print_str("  ID  Name            Success Rate  Avg Time  Applicable Types\n");
    print_str("  ------------------------------------------------------------------------\n");
    for (int i = 0; i < strategy_count; i++) {
        print_int(strategies[i].strat_id);
        print_str("  ");
        print_str(strategies[i].name);
        int len = my_strlen(strategies[i].name);
        for (int p = 0; p < 16 - len; p++) print_str(" ");
        print_int(strategies[i].success_rate);
        print_str("%         ");
        print_int(strategies[i].avg_time_ms);
        print_str("ms    ");
        print_str(strategies[i].applicable_types);
        print_str("\n");
    }
    return strategy_count;
}

/* ===== CLI entry ===== */
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
    print_str("General Solver v35.0 - Universal Problem Solving Engine\n");
    if (help) {
        print_str("Usage: general_solver [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run problem solver test\n");
        return;
    }
    if (test) {
        print_str("=== General Solver Test ===\n\n");
        /* Register strategies */
        gs_register_strategy("Decomposition", 85, 500, "Math,Planning,Design,Optimization");
        gs_register_strategy("Analogy", 70, 300, "Design,Planning");
        gs_register_strategy("Deduction", 90, 400, "Math,Logic,Diagnosis");
        gs_register_strategy("Induction", 75, 350, "Logic,Diagnosis");
        gs_register_strategy("Heuristic", 60, 200, "Math,Logic,Planning,Diagnosis,Design,Optimization");
        print_str("  Strategies: "); print_int(strategy_count); print_str("\n\n");
        gs_list_strategies();
        print_str("\n");
        /* Submit problems */
        print_str("Submitting problems:\n");
        int p1 = gs_submit_problem(PROB_MATH, 7, "Solve differential equation: dy/dx = 3x^2 + 2x");
        int p2 = gs_submit_problem(PROB_LOGIC, 5, "Prove: if A->B and B->C, then A->C");
        int p3 = gs_submit_problem(PROB_PLANNING, 8, "Plan optimal route for 10 delivery points");
        int p4 = gs_submit_problem(PROB_DIAGNOSIS, 6, "System crash after module X load");
        int p5 = gs_submit_problem(PROB_OPTIMIZATION, 9, "Minimize memory usage for 423 modules");
        print_str("\n");
        /* Solve all */
        print_str("Solving problems:\n");
        gs_solve(p1);
        gs_solve(p2);
        gs_solve(p3);
        gs_solve(p4);
        gs_solve(p5);
        print_str("\n");
        /* Decompose a complex problem */
        print_str("Decomposition demo:\n");
        int p6 = gs_submit_problem(PROB_PLANNING, 10, "Design distributed backup system");
        gs_decompose(p6, 4);
        gs_solve(p6);
        print_str("\n");
        /* Final report */
        gs_report();
        print_str("\n=== General Solver Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
