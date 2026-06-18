/* supply_chain: Supply chain management system (v1.0)
 * Inventory, logistics, demand forecasting, optimization, risk management
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

#define MAX_PRODUCTS  16
#define MAX_WAREHOUSE 8
#define MAX_SUPPLIERS 8
#define MAX_ROUTES    8
#define MAX_PERIODS   12

#define STRAT_LIFO   0x01
#define STRAT_FIFO   0x02
#define STRAT_FEFO   0x03
#define STRAT_JIT    0x04

typedef struct {
    int    id;
    double unit_cost;
    double unit_price;
    double lead_time;
    double safety_stock;
    double reorder_point;
    double eoq;
    double current_stock;
    double demand_rate;
    int    active;
} product_t;

typedef struct {
    double capacity;
    double current_load;
    double throughput;
    double cost_per_unit;
    double x, y;
    int    id;
    int    active;
} warehouse_t;

typedef struct {
    double capacity;
    double lead_time;
    double cost_per_unit;
    double reliability;
    int    id;
    int    active;
} supplier_t;

typedef struct {
    double distance;
    double cost;
    double time;
    double capacity;
    int    from_wh;
    int    to_wh;
    int    id;
    int    active;
} route_t;

typedef struct {
    double demand[MAX_PERIODS];
    double forecast[MAX_PERIODS];
    double error;
    double mape;
    int    product_id;
    int    n_periods;
    int    active;
} forecast_t;

typedef struct {
    int    n_products;
    int    n_warehouses;
    int    n_suppliers;
    int    n_routes;
    double total_cost;
    double total_revenue;
    double service_level;
    double fill_rate;
    int    stockout_count;
    int    optimization_count;
} sc_state_t;

static product_t products[MAX_PRODUCTS];
static warehouse_t warehouses[MAX_WAREHOUSE];
static supplier_t suppliers[MAX_SUPPLIERS];
static route_t routes[MAX_ROUTES];
static forecast_t forecasts[MAX_PRODUCTS];
static sc_state_t sc;

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
static double dsin(double x) {
    double sum = x, term = x, x2 = x * x;
    for (int i = 1; i < 15; i++) { term *= -x2 / ((2*i)*(2*i+1)); sum += term; }
    return sum;
}

int sc_init(void) {
    if (initialized) return -1;
    sc.n_products = 0; sc.n_warehouses = 0;
    sc.n_suppliers = 0; sc.n_routes = 0;
    sc.total_cost = 0.0; sc.total_revenue = 0.0;
    sc.service_level = 0.0; sc.fill_rate = 0.0;
    sc.stockout_count = 0; sc.optimization_count = 0;
    for (int i = 0; i < MAX_PRODUCTS; i++) { products[i].active = 0; forecasts[i].active = 0; }
    for (int i = 0; i < MAX_WAREHOUSE; i++) warehouses[i].active = 0;
    for (int i = 0; i < MAX_SUPPLIERS; i++) suppliers[i].active = 0;
    for (int i = 0; i < MAX_ROUTES; i++) routes[i].active = 0;
    initialized = 1;
    print_str("[SC] Supply chain initialized\n");
    return 0;
}

int sc_add_product(double cost, double price, double lead_time, double demand) {
    if (sc.n_products >= MAX_PRODUCTS) return -1;
    product_t* p = &products[sc.n_products];
    p->id = sc.n_products; p->unit_cost = cost; p->unit_price = price;
    p->lead_time = lead_time; p->demand_rate = demand;
    p->safety_stock = demand * lead_time * 0.5;
    p->reorder_point = demand * lead_time + p->safety_stock;
    double setup_cost = 100.0; double holding_cost = cost * 0.2;
    p->eoq = dsqrt(2.0 * demand * setup_cost / (holding_cost + 1e-10));
    p->current_stock = p->reorder_point * 2.0;
    p->active = 1;
    sc.n_products++;
    print_str("[SC] Product "); print_int(sc.n_products - 1);
    print_str(" cost="); print_int((int)cost); print_str(" price="); print_int((int)price);
    print_str(" demand="); print_int((int)demand);
    print_str(" EOQ="); print_int((int)p->eoq);
    print_str(" ROP="); print_int((int)p->reorder_point); print_str("\n");
    return sc.n_products - 1;
}

int sc_add_warehouse(double capacity, double x, double y) {
    if (sc.n_warehouses >= MAX_WAREHOUSE) return -1;
    warehouse_t* w = &warehouses[sc.n_warehouses];
    w->capacity = capacity; w->current_load = 0.0;
    w->throughput = capacity * 0.1;
    w->cost_per_unit = 0.5; w->x = x; w->y = y;
    w->id = sc.n_warehouses; w->active = 1;
    sc.n_warehouses++;
    print_str("[SC] Warehouse "); print_int(sc.n_warehouses - 1);
    print_str(" cap="); print_int((int)capacity);
    print_str(" pos=("); print_int((int)x); print_str(","); print_int((int)y); print_str(")\n");
    return sc.n_warehouses - 1;
}

int sc_add_supplier(double capacity, double lead_time, double reliability) {
    if (sc.n_suppliers >= MAX_SUPPLIERS) return -1;
    supplier_t* s = &suppliers[sc.n_suppliers];
    s->capacity = capacity; s->lead_time = lead_time;
    s->reliability = reliability;
    s->cost_per_unit = 5.0; s->id = sc.n_suppliers;
    s->active = 1;
    sc.n_suppliers++;
    print_str("[SC] Supplier "); print_int(sc.n_suppliers - 1);
    print_str(" cap="); print_int((int)capacity);
    print_str(" LT="); print_int((int)lead_time); print_str("d");
    print_str(" rel="); print_int((int)(reliability * 100)); print_str("%\n");
    return sc.n_suppliers - 1;
}

int sc_add_route(int from, int to, double distance, double cost) {
    if (sc.n_routes >= MAX_ROUTES) return -1;
    route_t* r = &routes[sc.n_routes];
    r->from_wh = from; r->to_wh = to;
    r->distance = distance; r->cost = cost;
    r->time = distance / 60.0; r->capacity = 1000.0;
    r->id = sc.n_routes; r->active = 1;
    sc.n_routes++;
    print_str("[SC] Route "); print_int(from); print_str("->"); print_int(to);
    print_str(" dist="); print_int((int)distance); print_str("km cost=");
    print_int((int)cost); print_str("\n");
    return sc.n_routes - 1;
}

void sc_demand_forecast(int product_id, int method) {
    if (product_id >= sc.n_products) return;
    forecast_t* f = &forecasts[product_id];
    f->product_id = product_id;
    f->n_periods = MAX_PERIODS;
    double base_demand = products[product_id].demand_rate;
    for (int i = 0; i < MAX_PERIODS; i++) {
        f->demand[i] = base_demand * (1.0 + 0.1 * dsin(2.0 * 3.14159 * i / 12.0));
        if (method == 1) {
            double sum = 0.0; int cnt = 0;
            for (int j = 0; j <= i && j < 3; j++) { sum += f->demand[i - j]; cnt++; }
            f->forecast[i] = sum / cnt;
        } else if (method == 2) {
            double alpha = 0.3;
            if (i == 0) f->forecast[i] = f->demand[i];
            else f->forecast[i] = alpha * f->demand[i] + (1.0 - alpha) * f->forecast[i - 1];
        } else {
            f->forecast[i] = f->demand[i] * 1.05;
        }
    }
    double sum_err = 0.0;
    for (int i = 0; i < MAX_PERIODS; i++)
        sum_err += dabs(f->forecast[i] - f->demand[i]) / (f->demand[i] + 1e-10);
    f->mape = sum_err / MAX_PERIODS * 100.0;
    f->active = 1;
    print_str("[SC] Forecast product "); print_int(product_id);
    print_str(" method="); print_int(method);
    print_str(" MAPE="); print_int((int)f->mape); print_str("%\n");
    for (int i = 0; i < 6; i++) {
        print_str("  P"); print_int(i + 1);
        print_str(" D="); print_int((int)f->demand[i]);
        print_str(" F="); print_int((int)f->forecast[i]); print_str("\n");
    }
}

void sc_inventory_optimization(void) {
    double total_cost = 0.0;
    for (int i = 0; i < sc.n_products; i++) {
        product_t* p = &products[i];
        double holding_cost = p->current_stock * p->unit_cost * 0.2;
        double order_cost = 100.0;
        int stockout = 0;
        if (p->current_stock < p->reorder_point) {
            p->current_stock += p->eoq;
            total_cost += order_cost;
            stockout = 1;
            sc.stockout_count++;
        }
        total_cost += holding_cost;
    }
    sc.total_cost = total_cost;
    sc.optimization_count++;
    print_str("[SC] Inventory optimization: cost="); print_int((int)total_cost);
    print_str(" stockouts="); print_int(sc.stockout_count); print_str("\n");
}

void sc_transportation_optimization(void) {
    double best_cost = 1e10;
    int best_route = -1;
    for (int i = 0; i < sc.n_routes; i++) {
        if (routes[i].cost < best_cost) { best_cost = routes[i].cost; best_route = i; }
    }
    sc.total_cost += best_cost * 10;
    print_str("[SC] Transport opt: best route="); print_int(best_route);
    print_str(" cost="); print_int((int)best_cost); print_str("\n");
}

void sc_service_level_calc(void) {
    double total_demand = 0.0, total_filled = 0.0;
    for (int i = 0; i < sc.n_products; i++) {
        total_demand += products[i].demand_rate;
        double filled = products[i].current_stock < products[i].demand_rate ?
                       products[i].current_stock : products[i].demand_rate;
        total_filled += filled;
    }
    sc.fill_rate = total_filled / (total_demand + 1e-10) * 100.0;
    sc.service_level = 95.0 - sc.stockout_count * 2.0;
    if (sc.service_level < 0) sc.service_level = 0;
    print_str("[SC] Service level: "); print_int((int)sc.service_level);
    print_str("% fill rate="); print_int((int)sc.fill_rate); print_str("%\n");
}

void sc_risk_assessment(void) {
    double risk_score = 0.0;
    for (int i = 0; i < sc.n_suppliers; i++) {
        risk_score += (1.0 - suppliers[i].reliability) * 10.0;
    }
    for (int i = 0; i < sc.n_products; i++) {
        if (products[i].current_stock < products[i].safety_stock) risk_score += 5.0;
    }
    print_str("[SC] Risk assessment: score="); print_int((int)(risk_score * 10)); print_str("/100\n");
}

void sc_print_state(void) {
    print_str("[SC] Products="); print_int(sc.n_products);
    print_str(" WH="); print_int(sc.n_warehouses);
    print_str(" Suppliers="); print_int(sc.n_suppliers);
    print_str(" Routes="); print_int(sc.n_routes);
    print_str(" Cost="); print_int((int)sc.total_cost);
    print_str(" SL="); print_int((int)sc.service_level); print_str("%\n");
}

int main(void) {
    print_str("=== Supply Chain Management Demo ===\n\n");
    sc_init();

    print_str("Adding products...\n");
    sc_add_product(10.0, 25.0, 7.0, 100.0);
    sc_add_product(50.0, 120.0, 14.0, 30.0);
    sc_add_product(5.0, 15.0, 3.0, 500.0);
    sc_add_product(200.0, 450.0, 21.0, 10.0);

    print_str("\nAdding warehouses...\n");
    sc_add_warehouse(5000.0, 0.0, 0.0);
    sc_add_warehouse(3000.0, 100.0, 50.0);
    sc_add_warehouse(4000.0, 200.0, 0.0);

    print_str("\nAdding suppliers...\n");
    sc_add_supplier(10000.0, 7.0, 0.95);
    sc_add_supplier(8000.0, 14.0, 0.90);
    sc_add_supplier(15000.0, 5.0, 0.98);

    print_str("\nAdding routes...\n");
    sc_add_route(0, 1, 120.0, 500.0);
    sc_add_route(1, 2, 100.0, 400.0);
    sc_add_route(0, 2, 200.0, 800.0);
    sc_add_route(2, 0, 200.0, 750.0);

    print_str("\nDemand forecasting (moving avg)...\n");
    sc_demand_forecast(0, 1);

    print_str("\nDemand forecasting (exponential smoothing)...\n");
    sc_demand_forecast(1, 2);

    print_str("\nDemand forecasting (trend)...\n");
    sc_demand_forecast(2, 3);

    print_str("\nInventory optimization...\n");
    products[0].current_stock = 50.0;
    products[1].current_stock = 200.0;
    sc_inventory_optimization();
    sc_inventory_optimization();

    print_str("\nTransportation optimization...\n");
    sc_transportation_optimization();

    print_str("\nService level calculation...\n");
    sc_service_level_calc();

    print_str("\nRisk assessment...\n");
    sc_risk_assessment();

    sc_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
