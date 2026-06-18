/* business_intelligence: Business intelligence system (v1.0)
 * OLAP analysis, report engine, predictive analytics, decision support, performance management
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

#define MAX_DIMENSIONS  8
#define MAX_MEASURES    16
#define MAX_REPORTS     8
#define MAX_KPIS        12
#define MAX_FORECASTS   8
#define MAX_PERIODS     12
#define MAX_SCENARIOS   8

typedef struct {
    int    dim_id;
    int    type;
    int    n_levels;
    int    n_members;
    int    active;
} dimension_t;

typedef struct {
    int    measure_id;
    int    agg_type;
    double values[MAX_PERIODS];
    double total;
    double average;
    double min_val;
    double max_val;
    int    n_periods;
    int    active;
} measure_t;

typedef struct {
    int    report_id;
    int    type;
    int    n_pages;
    int    n_charts;
    int    schedule;
    int    last_run;
    int    active;
} report_t;

typedef struct {
    int    kpi_id;
    double target;
    double actual;
    double threshold_warn;
    double threshold_crit;
    int    status;
    int    active;
} kpi_t;

typedef struct {
    int    forecast_id;
    int    method;
    double history[MAX_PERIODS];
    double forecast[MAX_PERIODS];
    double mape;
    double rmse;
    int    horizon;
    int    active;
} forecast_t;

typedef struct {
    int    scenario_id;
    double revenue_change;
    double cost_change;
    double margin_impact;
    double risk_score;
    int    active;
} scenario_t;

typedef struct {
    int    n_dimensions;
    int    n_measures;
    int    n_reports;
    int    n_kpis;
    int    n_forecasts;
    int    n_scenarios;
    double total_revenue;
    double total_cost;
    double total_profit;
    int    alerts;
} bi_state_t;

static dimension_t dimensions[MAX_DIMENSIONS];
static measure_t measures[MAX_MEASURES];
static report_t reports[MAX_REPORTS];
static kpi_t kpis[MAX_KPIS];
static forecast_t forecasts[MAX_FORECASTS];
static scenario_t scenarios[MAX_SCENARIOS];
static bi_state_t bi;

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

int bi_init(void) {
    if (initialized) return -1;
    bi.n_dimensions = 0; bi.n_measures = 0; bi.n_reports = 0;
    bi.n_kpis = 0; bi.n_forecasts = 0; bi.n_scenarios = 0;
    bi.total_revenue = 0.0; bi.total_cost = 0.0; bi.total_profit = 0.0;
    bi.alerts = 0;
    for (int i = 0; i < MAX_DIMENSIONS; i++) dimensions[i].active = 0;
    for (int i = 0; i < MAX_MEASURES; i++) measures[i].active = 0;
    for (int i = 0; i < MAX_REPORTS; i++) reports[i].active = 0;
    for (int i = 0; i < MAX_KPIS; i++) kpis[i].active = 0;
    for (int i = 0; i < MAX_FORECASTS; i++) forecasts[i].active = 0;
    for (int i = 0; i < MAX_SCENARIOS; i++) scenarios[i].active = 0;
    initialized = 1;
    print_str("[BI] Business intelligence initialized\n");
    return 0;
}

int bi_add_dimension(int type, int n_levels, int n_members) {
    if (bi.n_dimensions >= MAX_DIMENSIONS) return -1;
    dimension_t* d = &dimensions[bi.n_dimensions];
    d->dim_id = bi.n_dimensions;
    d->type = type;
    d->n_levels = n_levels;
    d->n_members = n_members;
    d->active = 1;
    bi.n_dimensions++;
    print_str("[BI] Dimension "); print_int(bi.n_dimensions - 1);
    print_str(" type="); print_int(type);
    print_str(" levels="); print_int(n_levels);
    print_str(" members="); print_int(n_members); print_str("\n");
    return bi.n_dimensions - 1;
}

int bi_add_measure(int agg_type, double base_value, double growth_rate) {
    if (bi.n_measures >= MAX_MEASURES) return -1;
    measure_t* m = &measures[bi.n_measures];
    m->measure_id = bi.n_measures;
    m->agg_type = agg_type;
    m->n_periods = MAX_PERIODS;
    double val = base_value;
    m->min_val = val;
    m->max_val = val;
    m->total = 0.0;
    for (int i = 0; i < MAX_PERIODS; i++) {
        m->values[i] = val;
        m->total += val;
        if (val < m->min_val) m->min_val = val;
        if (val > m->max_val) m->max_val = val;
        val *= (1.0 + growth_rate + (double)((i * 3 + bi.n_measures) % 7 - 3) * 0.01);
    }
    m->average = m->total / MAX_PERIODS;
    m->active = 1;
    bi.n_measures++;
    print_str("[BI] Measure "); print_int(bi.n_measures - 1);
    print_str(" agg="); print_int(agg_type);
    print_str(" base="); print_int((int)base_value);
    print_str(" growth="); print_int((int)(growth_rate * 100)); print_str("%");
    print_str(" avg="); print_int((int)m->average); print_str("\n");
    return bi.n_measures - 1;
}

void bi_olap_query(int measure_id, int dim_id) {
    if (measure_id >= bi.n_measures || dim_id >= bi.n_dimensions) return;
    measure_t* m = &measures[measure_id];
    dimension_t* d = &dimensions[dim_id];
    print_str("[BI] OLAP query measure="); print_int(measure_id);
    print_str(" dim="); print_int(dim_id); print_str("\n");
    print_str("  Slices: "); print_int(d->n_members); print_str("\n");
    print_str("  Total: "); print_int((int)m->total); print_str("\n");
    print_str("  Avg: "); print_int((int)m->average); print_str("\n");
    print_str("  Min: "); print_int((int)m->min_val); print_str("\n");
    print_str("  Max: "); print_int((int)m->max_val); print_str("\n");
}

int bi_create_report(int type, int n_charts, int schedule) {
    if (bi.n_reports >= MAX_REPORTS) return -1;
    report_t* r = &reports[bi.n_reports];
    r->report_id = bi.n_reports;
    r->type = type;
    r->n_pages = 2 + (type % 4);
    r->n_charts = n_charts;
    r->schedule = schedule;
    r->last_run = 0;
    r->active = 1;
    bi.n_reports++;
    print_str("[BI] Report "); print_int(bi.n_reports - 1);
    print_str(" type="); print_int(type);
    print_str(" pages="); print_int(r->n_pages);
    print_str(" charts="); print_int(n_charts);
    print_str(" sched="); print_int(schedule); print_str("\n");
    return bi.n_reports - 1;
}

void bi_generate_report(int report_id) {
    if (report_id >= bi.n_reports) return;
    report_t* r = &reports[report_id];
    r->last_run++;
    print_str("[BI] Generating report "); print_int(report_id);
    print_str(" type="); print_int(r->type);
    print_str(" pages="); print_int(r->n_pages);
    print_str(" charts="); print_int(r->n_charts);
    print_str(" run#"); print_int(r->last_run); print_str("\n");
    for (int i = 0; i < r->n_charts; i++) {
        print_str("  Chart "); print_int(i);
        print_str(" ");
        int bars = 5 + (i * 3 + report_id) % 8;
        for (int j = 0; j < bars; j++) print_str("#");
        print_str(" "); print_int(bars * 10); print_str("\n");
    }
}

int bi_add_kpi(double target, double actual, double warn_threshold, double crit_threshold) {
    if (bi.n_kpis >= MAX_KPIS) return -1;
    kpi_t* k = &kpis[bi.n_kpis];
    k->kpi_id = bi.n_kpis;
    k->target = target;
    k->actual = actual;
    k->threshold_warn = warn_threshold;
    k->threshold_crit = crit_threshold;
    double ratio = actual / (target + 1e-10) * 100.0;
    if (ratio < crit_threshold) k->status = 3;
    else if (ratio < warn_threshold) k->status = 2;
    else if (ratio < 100.0) k->status = 1;
    else k->status = 0;
    k->active = 1;
    bi.n_kpis++;
    if (k->status >= 2) bi.alerts++;
    print_str("[BI] KPI "); print_int(bi.n_kpis - 1);
    print_str(" target="); print_int((int)target);
    print_str(" actual="); print_int((int)actual);
    print_str(" ratio="); print_int((int)ratio); print_str("%");
    print_str(" status="); print_int(k->status); print_str("\n");
    return bi.n_kpis - 1;
}

int bi_forecast(int method, double base, double trend, int horizon) {
    if (bi.n_forecasts >= MAX_FORECASTS) return -1;
    forecast_t* f = &forecasts[bi.n_forecasts];
    f->forecast_id = bi.n_forecasts;
    f->method = method;
    f->horizon = horizon;
    double val = base;
    for (int i = 0; i < MAX_PERIODS; i++) {
        f->history[i] = val;
        val += trend + (double)((i * 5 + bi.n_forecasts * 3) % 11 - 5) * 0.5;
    }
    val = f->history[MAX_PERIODS - 1];
    double sum_err2 = 0.0;
    for (int i = 0; i < horizon; i++) {
        double noise = (double)((i * 7 + bi.n_forecasts) % 9 - 4) * 0.3;
        f->forecast[i] = val + trend * (double)(i + 1) + noise;
        double err = noise;
        sum_err2 += err * err;
    }
    f->rmse = dsqrt(sum_err2 / horizon);
    f->mape = f->rmse / base * 100.0;
    f->active = 1;
    bi.n_forecasts++;
    print_str("[BI] Forecast "); print_int(bi.n_forecasts - 1);
    print_str(" method="); print_int(method);
    print_str(" base="); print_int((int)base);
    print_str(" trend="); print_int((int)trend);
    print_str(" horizon="); print_int(horizon);
    print_str(" RMSE="); print_int((int)(f->rmse * 100)); print_str("c");
    print_str(" MAPE="); print_int((int)(f->mape * 100)); print_str("bp\n");
    return bi.n_forecasts - 1;
}

int bi_scenario(double revenue_change, double cost_change) {
    if (bi.n_scenarios >= MAX_SCENARIOS) return -1;
    scenario_t* s = &scenarios[bi.n_scenarios];
    s->scenario_id = bi.n_scenarios;
    s->revenue_change = revenue_change;
    s->cost_change = cost_change;
    s->margin_impact = revenue_change - cost_change;
    double risk = dabs(revenue_change) * 0.3 + dabs(cost_change) * 0.4;
    s->risk_score = risk > 100.0 ? 100.0 : risk;
    s->active = 1;
    bi.n_scenarios++;
    print_str("[BI] Scenario "); print_int(bi.n_scenarios - 1);
    print_str(" rev="); print_int((int)revenue_change); print_str("%");
    print_str(" cost="); print_int((int)cost_change); print_str("%");
    print_str(" margin="); print_int((int)s->margin_impact); print_str("%");
    print_str(" risk="); print_int((int)s->risk_score); print_str("\n");
    return bi.n_scenarios - 1;
}

void bi_sensitivity_analysis(int measure_id) {
    if (measure_id >= bi.n_measures) return;
    measure_t* m = &measures[measure_id];
    print_str("[BI] Sensitivity analysis measure="); print_int(measure_id); print_str("\n");
    double factors[4] = {-10.0, -5.0, 5.0, 10.0};
    for (int i = 0; i < 4; i++) {
        double impact = m->average * factors[i] * 0.01;
        print_str("  Factor "); print_int((int)factors[i]); print_str("%");
        print_str(" -> impact="); print_int((int)impact); print_str("\n");
    }
}

void bi_print_state(void) {
    print_str("[BI] Dims="); print_int(bi.n_dimensions);
    print_str(" Measures="); print_int(bi.n_measures);
    print_str(" Reports="); print_int(bi.n_reports);
    print_str(" KPIs="); print_int(bi.n_kpis);
    print_str(" Forecasts="); print_int(bi.n_forecasts);
    print_str(" Scenarios="); print_int(bi.n_scenarios);
    print_str("\n");
    print_str("  Alerts: "); print_int(bi.alerts); print_str("\n");
    int on_target = 0;
    for (int i = 0; i < bi.n_kpis; i++) {
        if (kpis[i].status == 0) on_target++;
    }
    print_str("  KPIs on target: "); print_int(on_target);
    print_str("/"); print_int(bi.n_kpis); print_str("\n");
}

int main(void) {
    print_str("=== Business Intelligence System Demo ===\n\n");
    bi_init();

    print_str("Adding dimensions...\n");
    bi_add_dimension(1, 3, 12);
    bi_add_dimension(2, 4, 50);
    bi_add_dimension(3, 2, 8);
    bi_add_dimension(4, 3, 25);

    print_str("\nAdding measures...\n");
    bi_add_measure(1, 1000.0, 0.02);
    bi_add_measure(2, 5000.0, 0.05);
    bi_add_measure(1, 2500.0, 0.03);
    bi_add_measure(3, 800.0, 0.01);
    bi_add_measure(2, 3500.0, 0.04);

    print_str("\nOLAP queries...\n");
    bi_olap_query(0, 0);
    bi_olap_query(1, 1);
    bi_olap_query(2, 2);

    print_str("\nCreating reports...\n");
    bi_create_report(1, 4, 1);
    bi_create_report(2, 6, 7);
    bi_create_report(3, 3, 30);

    print_str("\nGenerating reports...\n");
    bi_generate_report(0);
    bi_generate_report(1);
    bi_generate_report(2);

    print_str("\nAdding KPIs...\n");
    bi_add_kpi(100.0, 95.0, 90.0, 70.0);
    bi_add_kpi(50.0, 48.0, 80.0, 60.0);
    bi_add_kpi(200.0, 180.0, 85.0, 65.0);
    bi_add_kpi(75.0, 60.0, 80.0, 50.0);
    bi_add_kpi(150.0, 155.0, 90.0, 70.0);
    bi_add_kpi(80.0, 50.0, 80.0, 60.0);

    print_str("\nForecasting...\n");
    bi_forecast(1, 1000.0, 50.0, 6);
    bi_forecast(2, 5000.0, 200.0, 6);
    bi_forecast(3, 2500.0, 100.0, 6);

    print_str("\nScenario analysis...\n");
    bi_scenario(10.0, 5.0);
    bi_scenario(-15.0, -8.0);
    bi_scenario(20.0, 15.0);
    bi_scenario(-5.0, 3.0);

    print_str("\nSensitivity analysis...\n");
    bi_sensitivity_analysis(0);
    bi_sensitivity_analysis(1);

    print_str("\nFinal state...\n");
    bi_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
