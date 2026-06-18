/* risk_management: Risk management system (v1.0)
 * Credit risk, market risk, operational risk, liquidity risk, risk reporting
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

#define MAX_COUNTERPARTIES 8
#define MAX_POSITIONS      16
#define MAX_EVENTS         8
#define MAX_SCENARIOS      8

typedef struct {
    int    counterparty_id;
    double exposure;
    double pd;
    double lgd;
    double ead;
    double el;
    int    credit_rating;
    int    active;
} counterparty_t;

typedef struct {
    int    position_id;
    int    instrument_type;
    double notional;
    double market_value;
    double delta;
    double gamma;
    double vega;
    double theta;
    int    active;
} position_t;

typedef struct {
    int    event_id;
    int    event_type;
    double loss_amount;
    double frequency;
    double severity;
    double expected_loss;
    int    active;
} op_event_t;

typedef struct {
    int    scenario_id;
    double market_move;
    double credit_spread;
    double liquidity_stress;
    double total_loss;
    int    active;
} scenario_t;

typedef struct {
    int    n_counterparties;
    int    n_positions;
    int    n_events;
    int    n_scenarios;
    double total_var;
    double total_cvar;
    double total_credit_el;
    double total_op_el;
    double liquidity_ratio;
    int    limit_breaches;
} rm_state_t;

static counterparty_t counterparties[MAX_COUNTERPARTIES];
static position_t positions[MAX_POSITIONS];
static op_event_t op_events[MAX_EVENTS];
static scenario_t scenarios[MAX_SCENARIOS];
static rm_state_t rm;

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
static double dsqrt(double x) {
    if (x <= 0.0) return 0.0;
    double g = x, p = x / g;
    for (int i = 0; i < 30; i++) { g = (g + p) * 0.5; p = x / g; }
    return g;
}

int rm_init(void) {
    if (initialized) return -1;
    rm.n_counterparties = 0; rm.n_positions = 0;
    rm.n_events = 0; rm.n_scenarios = 0;
    rm.total_var = 0.0; rm.total_cvar = 0.0;
    rm.total_credit_el = 0.0; rm.total_op_el = 0.0;
    rm.liquidity_ratio = 0.0; rm.limit_breaches = 0;
    for (int i = 0; i < MAX_COUNTERPARTIES; i++) counterparties[i].active = 0;
    for (int i = 0; i < MAX_POSITIONS; i++) positions[i].active = 0;
    for (int i = 0; i < MAX_EVENTS; i++) op_events[i].active = 0;
    for (int i = 0; i < MAX_SCENARIOS; i++) scenarios[i].active = 0;
    initialized = 1;
    print_str("[RM] Risk management initialized\n");
    return 0;
}

int rm_add_counterparty(double exposure, double pd, double lgd, int rating) {
    if (rm.n_counterparties >= MAX_COUNTERPARTIES) return -1;
    counterparty_t* cp = &counterparties[rm.n_counterparties];
    cp->counterparty_id = rm.n_counterparties;
    cp->exposure = exposure; cp->pd = pd; cp->lgd = lgd;
    cp->ead = exposure;
    cp->el = exposure * pd * lgd;
    cp->credit_rating = rating;
    cp->active = 1;
    rm.n_counterparties++;
    rm.total_credit_el += cp->el;
    print_str("[RM] Counterparty "); print_int(rm.n_counterparties - 1);
    print_str(" exp="); print_int((int)exposure);
    print_str(" PD="); print_int((int)(pd * 10000)); print_str("bp");
    print_str(" LGD="); print_int((int)(lgd * 100)); print_str("%");
    print_str(" EL="); print_int((int)cp->el);
    print_str(" rating="); print_int(rating); print_str("\n");
    return rm.n_counterparties - 1;
}

int rm_add_position(int type, double notional, double market_value) {
    if (rm.n_positions >= MAX_POSITIONS) return -1;
    position_t* p = &positions[rm.n_positions];
    p->position_id = rm.n_positions;
    p->instrument_type = type;
    p->notional = notional; p->market_value = market_value;
    p->delta = 0.5 + (double)(rm.n_positions % 4) * 0.1;
    p->gamma = 0.01 + (double)(rm.n_positions % 3) * 0.005;
    p->vega = 10.0 + (double)(rm.n_positions % 5) * 2.0;
    p->theta = -5.0 - (double)(rm.n_positions % 4);
    p->active = 1;
    rm.n_positions++;
    print_str("[RM] Position "); print_int(rm.n_positions - 1);
    print_str(" type="); print_int(type);
    print_str(" notion="); print_int((int)notional);
    print_str(" MV="); print_int((int)market_value);
    print_str(" delta="); print_int((int)(p->delta * 100)); print_str("%\n");
    return rm.n_positions - 1;
}

void rm_var_calculation(void) {
    double total_delta = 0.0, total_vega = 0.0;
    for (int i = 0; i < rm.n_positions; i++) {
        total_delta += dabs(positions[i].delta * positions[i].market_value);
        total_vega += positions[i].vega;
    }
    rm.total_var = total_delta * 0.02 + total_vega * 0.5;
    rm.total_cvar = rm.total_var * 1.3;
    print_str("[RM] VaR calculation:\n");
    print_str("  VaR (95%): "); print_int((int)rm.total_var); print_str("\n");
    print_str("  CVaR (95%): "); print_int((int)rm.total_cvar); print_str("\n");
    print_str("  Total delta: "); print_int((int)total_delta); print_str("\n");
    print_str("  Total vega: "); print_int((int)total_vega); print_str("\n");
}

int rm_add_op_event(int type, double loss, double frequency) {
    if (rm.n_events >= MAX_EVENTS) return -1;
    op_event_t* e = &op_events[rm.n_events];
    e->event_id = rm.n_events;
    e->event_type = type;
    e->loss_amount = loss;
    e->frequency = frequency;
    e->severity = loss;
    e->expected_loss = loss * frequency;
    e->active = 1;
    rm.n_events++;
    rm.total_op_el += e->expected_loss;
    print_str("[RM] Op event "); print_int(rm.n_events - 1);
    print_str(" type="); print_int(type);
    print_str(" loss="); print_int((int)loss);
    print_str(" freq="); print_int((int)(frequency * 100)); print_str("%");
    print_str(" EL="); print_int((int)e->expected_loss); print_str("\n");
    return rm.n_events - 1;
}

int rm_add_scenario(double market_move, double credit_spread, double liquidity) {
    if (rm.n_scenarios >= MAX_SCENARIOS) return -1;
    scenario_t* s = &scenarios[rm.n_scenarios];
    s->scenario_id = rm.n_scenarios;
    s->market_move = market_move;
    s->credit_spread = credit_spread;
    s->liquidity_stress = liquidity;
    double total_mv = 0.0;
    for (int i = 0; i < rm.n_positions; i++) total_mv += positions[i].market_value;
    s->total_loss = total_mv * dabs(market_move) * 0.01 +
                    rm.total_credit_el * credit_spread * 0.1;
    s->active = 1;
    rm.n_scenarios++;
    print_str("[RM] Scenario "); print_int(rm.n_scenarios - 1);
    print_str(" mkt="); print_int((int)(market_move * 100)); print_str("%");
    print_str(" cred="); print_int((int)(credit_spread * 100)); print_str("%");
    print_str(" liq="); print_int((int)(liquidity * 100)); print_str("%");
    print_str(" loss="); print_int((int)s->total_loss); print_str("\n");
    return rm.n_scenarios - 1;
}

void rm_liquidity_assessment(void) {
    double cash = 5000.0;
    double obligations = 3000.0 + rm.n_positions * 100.0;
    rm.liquidity_ratio = cash / (obligations + 1e-10) * 100.0;
    print_str("[RM] Liquidity assessment:\n");
    print_str("  Cash: "); print_int((int)cash); print_str("\n");
    print_str("  Obligations: "); print_int((int)obligations); print_str("\n");
    print_str("  LCR: "); print_int((int)rm.liquidity_ratio); print_str("%\n");
    if (rm.liquidity_ratio < 100) {
        print_str("  ALERT: Below regulatory minimum\n");
        rm.limit_breaches++;
    }
}

void rm_limit_monitoring(void) {
    print_str("[RM] Limit monitoring:\n");
    int breaches = 0;
    for (int i = 0; i < rm.n_positions; i++) {
        double exposure = positions[i].market_value;
        double limit = 5000.0;
        if (exposure > limit) {
            print_str("  BREACH: Position "); print_int(i);
            print_str(" exp="); print_int((int)exposure);
            print_str(" limit="); print_int((int)limit); print_str("\n");
            breaches++;
        }
    }
    rm.limit_breaches += breaches;
    if (breaches == 0) print_str("  All positions within limits\n");
    print_str("  Total breaches: "); print_int(rm.limit_breaches); print_str("\n");
}

void rm_risk_report(void) {
    print_str("[RM] Risk report:\n");
    print_str("  Credit EL: "); print_int((int)rm.total_credit_el); print_str("\n");
    print_str("  Market VaR: "); print_int((int)rm.total_var); print_str("\n");
    print_str("  Market CVaR: "); print_int((int)rm.total_cvar); print_str("\n");
    print_str("  Operational EL: "); print_int((int)rm.total_op_el); print_str("\n");
    print_str("  Liquidity: "); print_int((int)rm.liquidity_ratio); print_str("%\n");
    print_str("  Limit breaches: "); print_int(rm.limit_breaches); print_str("\n");
}

void rm_print_state(void) {
    print_str("[RM] CPs="); print_int(rm.n_counterparties);
    print_str(" Pos="); print_int(rm.n_positions);
    print_str(" Events="); print_int(rm.n_events);
    print_str(" Scenarios="); print_int(rm.n_scenarios);
    print_str(" VaR="); print_int((int)rm.total_var);
    print_str(" CrEL="); print_int((int)rm.total_credit_el); print_str("\n");
}

int main(void) {
    print_str("=== Risk Management System Demo ===\n\n");
    rm_init();

    print_str("Adding counterparties...\n");
    rm_add_counterparty(10000.0, 0.02, 0.45, 3);
    rm_add_counterparty(8000.0, 0.05, 0.40, 4);
    rm_add_counterparty(15000.0, 0.01, 0.50, 2);
    rm_add_counterparty(5000.0, 0.08, 0.35, 5);

    print_str("\nAdding positions...\n");
    rm_add_position(1, 5000.0, 4800.0);
    rm_add_position(2, 3000.0, 3200.0);
    rm_add_position(3, 8000.0, 7500.0);
    rm_add_position(1, 2000.0, 2100.0);
    rm_add_position(2, 6000.0, 5800.0);

    print_str("\nVaR calculation...\n");
    rm_var_calculation();

    print_str("\nOperational risk events...\n");
    rm_add_op_event(1, 500.0, 0.1);
    rm_add_op_event(2, 1000.0, 0.05);
    rm_add_op_event(3, 200.0, 0.2);
    rm_add_op_event(4, 2000.0, 0.02);

    print_str("\nStress scenarios...\n");
    rm_add_scenario(-10.0, 50.0, 30.0);
    rm_add_scenario(-20.0, 100.0, 50.0);
    rm_add_scenario(5.0, -20.0, 10.0);

    print_str("\nLiquidity assessment...\n");
    rm_liquidity_assessment();

    print_str("\nLimit monitoring...\n");
    rm_limit_monitoring();

    print_str("\nRisk report...\n");
    rm_risk_report();

    rm_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
