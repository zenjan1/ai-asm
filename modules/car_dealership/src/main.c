/* car_dealership: Car dealership management (v1.0)
 * Inventory, sales, test drives, financing, service, revenue
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

#define MAX_VEHICLES    16
#define MAX_SALESPEOPLE 8
#define MAX_CUSTOMERS   12
#define MAX_TESTDRIVES  10
#define MAX_FINANCES    10
#define MAX_SERVICE     12

typedef struct {
    int    vehicle_id;
    int    type;
    int    condition;
    int    year;
    double price;
    double cost;
    int    color;
    int    sold;
    int    salesperson_id;
    int    days_on_lot;
    int    active;
} vehicle_t;

typedef struct {
    int    sp_id;
    int    n_sales;
    double total_sales;
    double commission_rate;
    double total_commission;
    int    active;
} salesperson_t;

typedef struct {
    int    cust_id;
    int    type;
    int    n_visits;
    double total_spent;
    int    active;
} customer_t;

typedef struct {
    int    td_id;
    int    vehicle_id;
    int    cust_id;
    int    duration_min;
    int    day;
    int    active;
} testdrive_t;

typedef struct {
    int    fin_id;
    int    cust_id;
    int    vehicle_id;
    double amount;
    double rate_pct;
    int    term_months;
    int    approved;
    int    active;
} finance_t;

typedef struct {
    int    svc_id;
    int    vehicle_id;
    int    cust_id;
    int    type;
    double fee;
    double cost;
    int    day;
    int    active;
} service_t;

typedef struct {
    int    n_vehicles;
    int    n_salespeople;
    int    n_customers;
    int    n_testdrives;
    int    n_finances;
    int    n_service;
    double vehicle_revenue;
    double finance_revenue;
    double service_revenue;
    double commission_paid;
    double total_revenue;
    double total_costs;
    int    vehicles_sold;
} cd_state_t;

static vehicle_t vehicles[MAX_VEHICLES];
static salesperson_t salespeople[MAX_SALESPEOPLE];
static customer_t customers[MAX_CUSTOMERS];
static testdrive_t testdrives[MAX_TESTDRIVES];
static finance_t finances[MAX_FINANCES];
static service_t services[MAX_SERVICE];
static cd_state_t cd;

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

int cd_init(void) {
    if (initialized) return -1;
    cd.n_vehicles = 0; cd.n_salespeople = 0; cd.n_customers = 0;
    cd.n_testdrives = 0; cd.n_finances = 0; cd.n_service = 0;
    cd.vehicle_revenue = 0.0; cd.finance_revenue = 0.0;
    cd.service_revenue = 0.0; cd.commission_paid = 0.0;
    cd.total_revenue = 0.0; cd.total_costs = 0.0;
    cd.vehicles_sold = 0;
    for (int i = 0; i < MAX_VEHICLES; i++) vehicles[i].active = 0;
    for (int i = 0; i < MAX_SALESPEOPLE; i++) salespeople[i].active = 0;
    for (int i = 0; i < MAX_CUSTOMERS; i++) customers[i].active = 0;
    for (int i = 0; i < MAX_TESTDRIVES; i++) testdrives[i].active = 0;
    for (int i = 0; i < MAX_FINANCES; i++) finances[i].active = 0;
    for (int i = 0; i < MAX_SERVICE; i++) services[i].active = 0;
    initialized = 1;
    print_str("[CD] Dealership initialized\n");
    return 0;
}

static const char* cond_str(int c) {
    if (c == 1) return " NEW";
    return " USED";
}

int cd_add_vehicle(int type, int condition, int year, double price, double cost, int color) {
    if (cd.n_vehicles >= MAX_VEHICLES) return -1;
    vehicle_t* v = &vehicles[cd.n_vehicles];
    v->vehicle_id = cd.n_vehicles;
    v->type = type;
    v->condition = condition;
    v->year = year;
    v->price = price;
    v->cost = cost;
    v->color = color;
    v->sold = 0;
    v->salesperson_id = -1;
    v->days_on_lot = 0;
    v->active = 1;
    cd.n_vehicles++;
    print_str("[CD] Vehicle "); print_int(cd.n_vehicles - 1);
    print_str(" type="); print_int(type);
    print_str(cond_str(condition));
    print_str(" yr="); print_int(year);
    print_str(" price="); print_int((int)price); print_str("\n");
    return cd.n_vehicles - 1;
}

int cd_add_salesperson(double commission_rate) {
    if (cd.n_salespeople >= MAX_SALESPEOPLE) return -1;
    salesperson_t* sp = &salespeople[cd.n_salespeople];
    sp->sp_id = cd.n_salespeople;
    sp->n_sales = 0;
    sp->total_sales = 0.0;
    sp->commission_rate = commission_rate;
    sp->total_commission = 0.0;
    sp->active = 1;
    cd.n_salespeople++;
    print_str("[CD] Salesperson "); print_int(cd.n_salespeople - 1);
    print_str(" comm="); print_int((int)(commission_rate * 100)); print_str("%\n");
    return cd.n_salespeople - 1;
}

int cd_register_customer(int type) {
    if (cd.n_customers >= MAX_CUSTOMERS) return -1;
    customer_t* c = &customers[cd.n_customers];
    c->cust_id = cd.n_customers;
    c->type = type;
    c->n_visits = 0;
    c->total_spent = 0.0;
    c->active = 1;
    cd.n_customers++;
    print_str("[CD] Customer "); print_int(cd.n_customers - 1);
    print_str(" type="); print_int(type); print_str("\n");
    return cd.n_customers - 1;
}

int cd_schedule_testdrive(int vehicle_id, int cust_id, int duration, int day) {
    if (cd.n_testdrives >= MAX_TESTDRIVES) return -1;
    if (vehicle_id >= cd.n_vehicles || cust_id >= cd.n_customers) return -2;
    testdrive_t* td = &testdrives[cd.n_testdrives];
    td->td_id = cd.n_testdrives;
    td->vehicle_id = vehicle_id;
    td->cust_id = cust_id;
    td->duration_min = duration;
    td->day = day;
    td->active = 1;
    customers[cust_id].n_visits++;
    cd.n_testdrives++;
    print_str("[CD] TestDrive "); print_int(cd.n_testdrives - 1);
    print_str(" V"); print_int(vehicle_id);
    print_str(" Cu"); print_int(cust_id);
    print_str(" dur="); print_int(duration); print_str("min\n");
    return cd.n_testdrives - 1;
}

int cd_sell_vehicle(int vehicle_id, int cust_id, int sp_id) {
    if (vehicle_id >= cd.n_vehicles || cust_id >= cd.n_customers) return -1;
    if (sp_id >= cd.n_salespeople) return -2;
    vehicle_t* v = &vehicles[vehicle_id];
    if (v->sold) return -3;
    v->sold = 1;
    v->salesperson_id = sp_id;
    customer_t* c = &customers[cust_id];
    c->total_spent += v->price;
    salesperson_t* sp = &salespeople[sp_id];
    double commission = v->price * sp->commission_rate;
    sp->n_sales++;
    sp->total_sales += v->price;
    sp->total_commission += commission;
    cd.vehicle_revenue += v->price;
    cd.total_revenue += v->price;
    cd.total_costs += v->cost + commission;
    cd.commission_paid += commission;
    cd.vehicles_sold++;
    print_str("[CD] Sold V"); print_int(vehicle_id);
    print_str(" Cu"); print_int(cust_id);
    print_str(" SP"); print_int(sp_id);
    print_str(" price="); print_int((int)v->price);
    print_str(" comm="); print_int((int)commission); print_str("\n");
    return 0;
}

int cd_approve_finance(int cust_id, int vehicle_id, double amount, double rate, int term) {
    if (cd.n_finances >= MAX_FINANCES) return -1;
    if (cust_id >= cd.n_customers || vehicle_id >= cd.n_vehicles) return -2;
    finance_t* f = &finances[cd.n_finances];
    f->fin_id = cd.n_finances;
    f->cust_id = cust_id;
    f->vehicle_id = vehicle_id;
    f->amount = amount;
    f->rate_pct = rate;
    f->term_months = term;
    f->approved = 1;
    f->active = 1;
    double interest = amount * rate / 100.0 * term / 12.0;
    cd.finance_revenue += interest;
    cd.total_revenue += interest;
    cd.n_finances++;
    print_str("[CD] Finance "); print_int(cd.n_finances - 1);
    print_str(" Cu"); print_int(cust_id);
    print_str(" V"); print_int(vehicle_id);
    print_str(" amt="); print_int((int)amount);
    print_str(" rate="); print_int((int)rate); print_str("%");
    print_str(" term="); print_int(term); print_str("mo\n");
    return cd.n_finances - 1;
}

int cd_schedule_service(int vehicle_id, int cust_id, int type, double fee, double cost, int day) {
    if (cd.n_service >= MAX_SERVICE) return -1;
    if (cust_id >= cd.n_customers) return -2;
    service_t* s = &services[cd.n_service];
    s->svc_id = cd.n_service;
    s->vehicle_id = vehicle_id;
    s->cust_id = cust_id;
    s->type = type;
    s->fee = fee;
    s->cost = cost;
    s->day = day;
    s->active = 1;
    customers[cust_id].total_spent += fee;
    cd.service_revenue += fee;
    cd.total_revenue += fee;
    cd.total_costs += cost;
    cd.n_service++;
    print_str("[CD] Service "); print_int(cd.n_service - 1);
    print_str(" V"); print_int(vehicle_id);
    print_str(" Cu"); print_int(cust_id);
    print_str(" type="); print_int(type);
    print_str(" fee="); print_int((int)fee); print_str("\n");
    return cd.n_service - 1;
}

void cd_inventory_report(void) {
    print_str("[CD] Inventory report:\n");
    int new_count = 0, used_count = 0, sold_count = 0;
    for (int i = 0; i < cd.n_vehicles; i++) {
        vehicle_t* v = &vehicles[i];
        if (v->sold) { sold_count++; continue; }
        if (v->condition == 1) new_count++;
        else used_count++;
        print_str("  V"); print_int(i);
        print_str(" type="); print_int(v->type);
        print_str(v->condition == 1 ? " NEW" : " USED");
        print_str(" yr="); print_int(v->year);
        print_str(" price="); print_int((int)v->price); print_str("\n");
    }
    print_str("  New: "); print_int(new_count);
    print_str(" Used: "); print_int(used_count);
    print_str(" Sold: "); print_int(sold_count); print_str("\n");
}

void cd_financial_report(void) {
    print_str("[CD] Financial report:\n");
    print_str("  Vehicle revenue: "); print_int((int)cd.vehicle_revenue); print_str("\n");
    print_str("  Finance revenue: "); print_int((int)cd.finance_revenue); print_str("\n");
    print_str("  Service revenue: "); print_int((int)cd.service_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)cd.total_revenue); print_str("\n");
    print_str("  Costs: "); print_int((int)cd.total_costs); print_str("\n");
    print_str("  Commission paid: "); print_int((int)cd.commission_paid); print_str("\n");
    double profit = cd.total_revenue - cd.total_costs;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    print_str("  Vehicles sold: "); print_int(cd.vehicles_sold); print_str("\n");
}

void cd_print_state(void) {
    print_str("[CD] Vehicles="); print_int(cd.n_vehicles);
    print_str(" SP="); print_int(cd.n_salespeople);
    print_str(" Cust="); print_int(cd.n_customers);
    print_str(" TD="); print_int(cd.n_testdrives);
    print_str(" Fin="); print_int(cd.n_finances);
    print_str(" Svc="); print_int(cd.n_service);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)cd.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Car Dealership Demo ===\n\n");
    cd_init();

    print_str("Adding salespeople...\n");
    cd_add_salesperson(0.02);
    cd_add_salesperson(0.025);
    cd_add_salesperson(0.02);
    cd_add_salesperson(0.03);
    cd_add_salesperson(0.025);
    cd_add_salesperson(0.02);
    cd_add_salesperson(0.03);
    cd_add_salesperson(0.025);

    print_str("\nRegistering customers...\n");
    cd_register_customer(1);
    cd_register_customer(2);
    cd_register_customer(1);
    cd_register_customer(3);
    cd_register_customer(2);
    cd_register_customer(1);
    cd_register_customer(3);
    cd_register_customer(2);
    cd_register_customer(1);
    cd_register_customer(3);
    cd_register_customer(2);
    cd_register_customer(1);

    print_str("\nAdding vehicles...\n");
    cd_add_vehicle(1, 1, 2025, 35000.0, 28000.0, 1);
    cd_add_vehicle(1, 1, 2025, 42000.0, 33000.0, 2);
    cd_add_vehicle(2, 1, 2025, 55000.0, 44000.0, 3);
    cd_add_vehicle(2, 1, 2025, 68000.0, 54000.0, 1);
    cd_add_vehicle(3, 1, 2025, 85000.0, 68000.0, 4);
    cd_add_vehicle(1, 2, 2022, 22000.0, 18000.0, 2);
    cd_add_vehicle(2, 2, 2021, 38000.0, 30000.0, 5);
    cd_add_vehicle(1, 2, 2023, 28000.0, 22000.0, 1);
    cd_add_vehicle(3, 2, 2020, 55000.0, 45000.0, 3);
    cd_add_vehicle(1, 1, 2025, 32000.0, 25000.0, 4);
    cd_add_vehicle(2, 2, 2022, 45000.0, 36000.0, 2);
    cd_add_vehicle(1, 1, 2025, 38000.0, 30000.0, 5);
    cd_add_vehicle(3, 1, 2025, 95000.0, 76000.0, 1);
    cd_add_vehicle(2, 2, 2023, 52000.0, 42000.0, 3);
    cd_add_vehicle(1, 1, 2025, 30000.0, 24000.0, 2);
    cd_add_vehicle(1, 2, 2021, 18000.0, 14000.0, 4);

    print_str("\nScheduling test drives...\n");
    cd_schedule_testdrive(0, 0, 30, 10);
    cd_schedule_testdrive(2, 1, 45, 10);
    cd_schedule_testdrive(4, 2, 30, 11);
    cd_schedule_testdrive(1, 3, 45, 11);
    cd_schedule_testdrive(5, 4, 30, 12);
    cd_schedule_testdrive(3, 5, 45, 12);
    cd_schedule_testdrive(8, 6, 30, 13);
    cd_schedule_testdrive(6, 7, 30, 13);
    cd_schedule_testdrive(9, 8, 45, 14);
    cd_schedule_testdrive(12, 9, 30, 14);

    print_str("\nSelling vehicles...\n");
    cd_sell_vehicle(0, 0, 0);
    cd_sell_vehicle(2, 1, 1);
    cd_sell_vehicle(4, 2, 2);
    cd_sell_vehicle(1, 3, 3);
    cd_sell_vehicle(5, 4, 4);
    cd_sell_vehicle(3, 5, 5);
    cd_sell_vehicle(8, 6, 6);
    cd_sell_vehicle(6, 7, 7);
    cd_sell_vehicle(9, 8, 0);
    cd_sell_vehicle(12, 9, 1);

    print_str("\nApproving financing...\n");
    cd_approve_finance(0, 0, 30000.0, 5.5, 60);
    cd_approve_finance(1, 2, 48000.0, 4.9, 72);
    cd_approve_finance(2, 4, 75000.0, 6.0, 60);
    cd_approve_finance(3, 1, 36000.0, 5.2, 48);
    cd_approve_finance(4, 5, 20000.0, 7.0, 36);
    cd_approve_finance(5, 3, 60000.0, 5.5, 60);
    cd_approve_finance(6, 6, 48000.0, 5.8, 72);
    cd_approve_finance(7, 7, 40000.0, 6.2, 48);
    cd_approve_finance(8, 9, 85000.0, 5.0, 60);
    cd_approve_finance(9, 8, 26000.0, 6.5, 48);

    print_str("\nService appointments...\n");
    cd_schedule_service(0, 0, 1, 150.0, 50.0, 15);
    cd_schedule_service(2, 1, 2, 300.0, 100.0, 15);
    cd_schedule_service(4, 2, 3, 500.0, 180.0, 16);
    cd_schedule_service(1, 3, 1, 180.0, 60.0, 16);
    cd_schedule_service(5, 4, 2, 250.0, 85.0, 17);
    cd_schedule_service(3, 5, 1, 160.0, 55.0, 17);
    cd_schedule_service(8, 6, 3, 450.0, 160.0, 18);
    cd_schedule_service(6, 7, 2, 280.0, 95.0, 18);
    cd_schedule_service(9, 8, 1, 170.0, 58.0, 19);
    cd_schedule_service(12, 9, 2, 320.0, 110.0, 19);
    cd_schedule_service(0, 0, 3, 400.0, 140.0, 20);
    cd_schedule_service(4, 2, 1, 200.0, 68.0, 20);

    print_str("\nInventory report...\n");
    cd_inventory_report();

    print_str("\nFinancial report...\n");
    cd_financial_report();

    print_str("\nFinal state...\n");
    cd_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
