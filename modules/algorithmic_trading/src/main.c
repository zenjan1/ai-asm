/* algorithmic_trading: Algorithmic trading system (v1.0)
 * High-frequency trading, strategy backtesting, execution algorithms, market data, compliance
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

#define MAX_STRATEGIES 8
#define MAX_ORDERS     16
#define MAX_INSTRUMENTS 8
#define MAX_PERIODS    16

typedef struct {
    int    strategy_id;
    int    type;
    double pnl;
    double sharpe;
    double max_drawdown;
    int    trades;
    double win_rate;
    int    active;
} strategy_t;

typedef struct {
    int    order_id;
    int    instrument_id;
    int    side;
    double price;
    double quantity;
    double filled_qty;
    int    status;
    int    strategy_id;
    int    active;
} order_t;

typedef struct {
    int    instrument_id;
    double bid;
    double ask;
    double last;
    double volume;
    double volatility;
    int    active;
} instrument_t;

typedef struct {
    double prices[MAX_PERIODS];
    double returns[MAX_PERIODS];
    double cumulative_return;
    double volatility;
    int    n_periods;
    int    active;
} backtest_t;

typedef struct {
    int    n_strategies;
    int    n_orders;
    int    n_instruments;
    double total_pnl;
    double total_volume;
    int    compliance_alerts;
    int    filled_orders;
} at_state_t;

static strategy_t strategies[MAX_STRATEGIES];
static order_t orders[MAX_ORDERS];
static instrument_t instruments[MAX_INSTRUMENTS];
static backtest_t backtests[MAX_STRATEGIES];
static at_state_t at;

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

int at_init(void) {
    if (initialized) return -1;
    at.n_strategies = 0; at.n_orders = 0; at.n_instruments = 0;
    at.total_pnl = 0.0; at.total_volume = 0.0;
    at.compliance_alerts = 0; at.filled_orders = 0;
    for (int i = 0; i < MAX_STRATEGIES; i++) strategies[i].active = 0;
    for (int i = 0; i < MAX_ORDERS; i++) orders[i].active = 0;
    for (int i = 0; i < MAX_INSTRUMENTS; i++) instruments[i].active = 0;
    for (int i = 0; i < MAX_STRATEGIES; i++) backtests[i].active = 0;
    initialized = 1;
    print_str("[AT] Algorithmic trading initialized\n");
    return 0;
}

int at_add_instrument(double bid, double ask, double volume) {
    if (at.n_instruments >= MAX_INSTRUMENTS) return -1;
    instrument_t* inst = &instruments[at.n_instruments];
    inst->instrument_id = at.n_instruments;
    inst->bid = bid; inst->ask = ask;
    inst->last = (bid + ask) / 2.0;
    inst->volume = volume;
    inst->volatility = 0.02 + (double)(at.n_instruments % 4) * 0.005;
    inst->active = 1;
    at.n_instruments++;
    print_str("[AT] Instrument "); print_int(at.n_instruments - 1);
    print_str(" bid="); print_int((int)(bid * 100));
    print_str(" ask="); print_int((int)(ask * 100));
    print_str(" vol="); print_int((int)volume);
    print_str(" sigma="); print_int((int)(inst->volatility * 1000)); print_str("bp\n");
    return at.n_instruments - 1;
}

int at_add_strategy(int type) {
    if (at.n_strategies >= MAX_STRATEGIES) return -1;
    strategy_t* s = &strategies[at.n_strategies];
    s->strategy_id = at.n_strategies;
    s->type = type;
    s->pnl = 0.0; s->sharpe = 0.0;
    s->max_drawdown = 0.0; s->trades = 0;
    s->win_rate = 0.0; s->active = 1;
    at.n_strategies++;
    print_str("[AT] Strategy "); print_int(at.n_strategies - 1);
    print_str(" type="); print_int(type); print_str("\n");
    return at.n_strategies - 1;
}

int at_place_order(int instrument_id, int side, double price, double qty, int strategy_id) {
    if (at.n_orders >= MAX_ORDERS) return -1;
    if (instrument_id >= at.n_instruments) return -1;
    order_t* o = &orders[at.n_orders];
    o->order_id = at.n_orders;
    o->instrument_id = instrument_id;
    o->side = side; o->price = price;
    o->quantity = qty; o->filled_qty = 0.0;
    o->status = 1; o->strategy_id = strategy_id;
    o->active = 1;
    at.n_orders++;
    print_str("[AT] Order "); print_int(at.n_orders - 1);
    print_str(" inst="); print_int(instrument_id);
    print_str(" side="); print_int(side);
    print_str(" px="); print_int((int)(price * 100));
    print_str(" qty="); print_int((int)qty);
    print_str(" strat="); print_int(strategy_id); print_str("\n");
    return at.n_orders - 1;
}

void at_fill_order(int order_id, double fill_price, double fill_qty) {
    if (order_id >= at.n_orders) return;
    order_t* o = &orders[order_id];
    o->filled_qty = fill_qty;
    o->status = 2;
    at.filled_orders++;
    double pnl = (o->side == 1) ? (fill_price - o->price) * fill_qty :
                                   (o->price - fill_price) * fill_qty;
    if (o->strategy_id < at.n_strategies) {
        strategies[o->strategy_id].pnl += pnl;
        strategies[o->strategy_id].trades++;
    }
    at.total_pnl += pnl;
    at.total_volume += fill_qty * fill_price;
    print_str("[AT] Order "); print_int(order_id);
    print_str(" filled px="); print_int((int)(fill_price * 100));
    print_str(" qty="); print_int((int)fill_qty);
    print_str(" pnl="); print_int((int)(pnl * 100)); print_str("c\n");
}

void at_backtest_strategy(int strategy_id) {
    if (strategy_id >= at.n_strategies) return;
    backtest_t* bt = &backtests[strategy_id];
    bt->n_periods = MAX_PERIODS;
    double base_price = 100.0;
    double cum = 0.0;
    for (int i = 0; i < MAX_PERIODS; i++) {
        double ret = 0.01 * (double)((i * 7 + strategy_id * 3) % 11 - 5);
        bt->returns[i] = ret;
        base_price *= (1.0 + ret);
        bt->prices[i] = base_price;
        cum += ret;
    }
    bt->cumulative_return = cum;
    double sum2 = 0.0;
    for (int i = 0; i < MAX_PERIODS; i++) sum2 += bt->returns[i] * bt->returns[i];
    bt->volatility = dsqrt(sum2 / MAX_PERIODS);
    bt->active = 1;
    strategies[strategy_id].sharpe = cum / (bt->volatility + 1e-10);
    strategies[strategy_id].win_rate = 50.0 + (double)(strategy_id % 4) * 5.0;
    strategies[strategy_id].max_drawdown = 5.0 + (double)(strategy_id % 3) * 2.0;
    print_str("[AT] Backtest strategy "); print_int(strategy_id);
    print_str(" ret="); print_int((int)(cum * 1000)); print_str("bp");
    print_str(" vol="); print_int((int)(bt->volatility * 10000)); print_str("bp");
    print_str(" sharpe="); print_int((int)(strategies[strategy_id].sharpe * 100));
    print_str(" win="); print_int((int)strategies[strategy_id].win_rate); print_str("%\n");
}

void at_execution_algorithm(int instrument_id, double target_qty, int algo_type) {
    if (instrument_id >= at.n_instruments) return;
    print_str("[AT] Execution algo inst="); print_int(instrument_id);
    print_str(" target="); print_int((int)target_qty);
    print_str(" algo="); print_int(algo_type); print_str("\n");
    double remaining = target_qty;
    int slices = 4;
    double slice_qty = target_qty / slices;
    for (int i = 0; i < slices && remaining > 0; i++) {
        double exec_qty = (remaining < slice_qty) ? remaining : slice_qty;
        remaining -= exec_qty;
        double exec_price = instruments[instrument_id].last +
                           (double)((i * 3 + algo_type) % 5 - 2) * 0.01;
        print_str("  Slice "); print_int(i);
        print_str(" px="); print_int((int)(exec_price * 100));
        print_str(" qty="); print_int((int)exec_qty);
        print_str(" remaining="); print_int((int)remaining); print_str("\n");
        at.total_volume += exec_qty * exec_price;
    }
}

void at_compliance_check(void) {
    print_str("[AT] Compliance check:\n");
    int alerts = 0;
    for (int i = 0; i < at.n_orders; i++) {
        if (orders[i].quantity > 10000) {
            print_str("  ALERT: Order "); print_int(i);
            print_str(" qty="); print_int((int)orders[i].quantity);
            print_str(" exceeds limit\n");
            alerts++;
        }
    }
    at.compliance_alerts += alerts;
    if (alerts == 0) print_str("  All orders compliant\n");
    print_str("  Total alerts: "); print_int(at.compliance_alerts); print_str("\n");
}

void at_print_state(void) {
    print_str("[AT] Strategies="); print_int(at.n_strategies);
    print_str(" Orders="); print_int(at.n_orders);
    print_str(" Instruments="); print_int(at.n_instruments);
    print_str(" PnL="); print_int((int)(at.total_pnl * 100)); print_str("c");
    print_str(" Vol="); print_int((int)at.total_volume);
    print_str(" Filled="); print_int(at.filled_orders); print_str("\n");
    for (int i = 0; i < at.n_strategies; i++) {
        print_str("  Strategy "); print_int(i);
        print_str(" pnl="); print_int((int)(strategies[i].pnl * 100)); print_str("c");
        print_str(" trades="); print_int(strategies[i].trades); print_str("\n");
    }
}

int main(void) {
    print_str("=== Algorithmic Trading System Demo ===\n\n");
    at_init();

    print_str("Adding instruments...\n");
    at_add_instrument(100.0, 100.05, 50000);
    at_add_instrument(50.0, 50.02, 80000);
    at_add_instrument(200.0, 200.10, 30000);
    at_add_instrument(75.0, 75.03, 60000);

    print_str("\nAdding strategies...\n");
    at_add_strategy(1);
    at_add_strategy(2);
    at_add_strategy(3);
    at_add_strategy(4);

    print_str("\nBacktesting strategies...\n");
    for (int i = 0; i < at.n_strategies; i++) at_backtest_strategy(i);

    print_str("\nPlacing orders...\n");
    at_place_order(0, 1, 100.0, 100.0, 0);
    at_place_order(1, 1, 50.0, 200.0, 1);
    at_place_order(2, 2, 200.0, 50.0, 2);
    at_place_order(3, 1, 75.0, 150.0, 3);
    at_place_order(0, 2, 100.5, 80.0, 0);

    print_str("\nFilling orders...\n");
    at_fill_order(0, 100.10, 100.0);
    at_fill_order(1, 50.05, 200.0);
    at_fill_order(2, 199.80, 50.0);
    at_fill_order(3, 75.08, 150.0);
    at_fill_order(4, 100.40, 80.0);

    print_str("\nExecution algorithms...\n");
    at_execution_algorithm(0, 500.0, 1);
    at_execution_algorithm(1, 800.0, 2);
    at_execution_algorithm(2, 300.0, 3);

    print_str("\nCompliance check...\n");
    at_compliance_check();

    at_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
