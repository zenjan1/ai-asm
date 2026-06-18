/* property_management: Property management system (v1.0)
 * Facilities, fees, owner services, security, environment management
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

#define MAX_UNITS       16
#define MAX_EQUIPMENT   10
#define MAX_WORK_ORDERS 14
#define MAX_FEES        12
#define MAX_COMPLAINTS  10
#define MAX_PATROLS     8
#define MAX_VISITORS    10

typedef struct {
    int    unit_id;
    int    owner_id;
    int    area_sqm;
    double monthly_fee;
    int    paid_months;
    int    outstanding;
    int    active;
} unit_t;

typedef struct {
    int    equip_id;
    int    type;
    int    install_year;
    int    last_service_month;
    int    service_interval;
    int    status;
    int    active;
} equipment_t;

typedef struct {
    int    wo_id;
    int    unit_id;
    int    type;
    int    priority;
    int    created_month;
    int    resolved_month;
    int    status;
    int    active;
} work_order_t;

typedef struct {
    int    fee_id;
    int    unit_id;
    int    type;
    double amount;
    int    month;
    int    paid;
    int    active;
} fee_t;

typedef struct {
    int    complaint_id;
    int    unit_id;
    int    category;
    int    priority;
    int    month;
    int    resolved;
    int    satisfaction;
    int    active;
} complaint_t;

typedef struct {
    int    patrol_id;
    int    guard_id;
    int    month;
    int    day;
    int    hour;
    int    zones_covered;
    int    incidents;
    int    active;
} patrol_t;

typedef struct {
    int    visitor_id;
    int    unit_id;
    int    entry_hour;
    int    exit_hour;
    int    approved;
    int    active;
} visitor_t;

typedef struct {
    int    n_units;
    int    n_equipment;
    int    n_work_orders;
    int    n_fees;
    int    n_complaints;
    int    n_patrols;
    int    n_visitors;
    double total_income;
    double outstanding_fees;
    int    resolved_complaints;
    int    resolved_work_orders;
} pm_state_t;

static unit_t units[MAX_UNITS];
static equipment_t equipment[MAX_EQUIPMENT];
static work_order_t work_orders[MAX_WORK_ORDERS];
static fee_t fees[MAX_FEES];
static complaint_t complaints[MAX_COMPLAINTS];
static patrol_t patrols[MAX_PATROLS];
static visitor_t visitors[MAX_VISITORS];
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
    pm.n_units = 0; pm.n_equipment = 0; pm.n_work_orders = 0;
    pm.n_fees = 0; pm.n_complaints = 0; pm.n_patrols = 0; pm.n_visitors = 0;
    pm.total_income = 0.0; pm.outstanding_fees = 0.0;
    pm.resolved_complaints = 0; pm.resolved_work_orders = 0;
    for (int i = 0; i < MAX_UNITS; i++) units[i].active = 0;
    for (int i = 0; i < MAX_EQUIPMENT; i++) equipment[i].active = 0;
    for (int i = 0; i < MAX_WORK_ORDERS; i++) work_orders[i].active = 0;
    for (int i = 0; i < MAX_FEES; i++) fees[i].active = 0;
    for (int i = 0; i < MAX_COMPLAINTS; i++) complaints[i].active = 0;
    for (int i = 0; i < MAX_PATROLS; i++) patrols[i].active = 0;
    for (int i = 0; i < MAX_VISITORS; i++) visitors[i].active = 0;
    initialized = 1;
    print_str("[PM] Property management initialized\n");
    return 0;
}

int pm_add_unit(int owner_id, int area, double fee) {
    if (pm.n_units >= MAX_UNITS) return -1;
    unit_t* u = &units[pm.n_units];
    u->unit_id = pm.n_units;
    u->owner_id = owner_id;
    u->area_sqm = area;
    u->monthly_fee = fee;
    u->paid_months = 0;
    u->outstanding = 0;
    u->active = 1;
    pm.n_units++;
    print_str("[PM] Unit "); print_int(pm.n_units - 1);
    print_str(" owner="); print_int(owner_id);
    print_str(" "); print_int(area); print_str("sqm");
    print_str(" fee="); print_int((int)fee); print_str("/mo\n");
    return pm.n_units - 1;
}

int pm_add_equipment(int type, int install_year, int interval) {
    if (pm.n_equipment >= MAX_EQUIPMENT) return -1;
    equipment_t* e = &equipment[pm.n_equipment];
    e->equip_id = pm.n_equipment;
    e->type = type;
    e->install_year = install_year;
    e->last_service_month = 1;
    e->service_interval = interval;
    e->status = 1;
    e->active = 1;
    pm.n_equipment++;
    print_str("[PM] Equipment "); print_int(pm.n_equipment - 1);
    print_str(" type="); print_int(type);
    print_str(" year="); print_int(install_year);
    print_str(" interval="); print_int(interval); print_str("mo\n");
    return pm.n_equipment - 1;
}

void pm_service_equipment(int equip_id, int month) {
    if (equip_id >= pm.n_equipment) return;
    equipment[equip_id].last_service_month = month;
    print_str("[PM] Equipment "); print_int(equip_id);
    print_str(" serviced at month "); print_int(month); print_str("\n");
}

int pm_create_work_order(int unit_id, int type, int priority, int month) {
    if (pm.n_work_orders >= MAX_WORK_ORDERS || unit_id >= pm.n_units) return -1;
    work_order_t* w = &work_orders[pm.n_work_orders];
    w->wo_id = pm.n_work_orders;
    w->unit_id = unit_id;
    w->type = type;
    w->priority = priority;
    w->created_month = month;
    w->resolved_month = 0;
    w->status = 1;
    w->active = 1;
    pm.n_work_orders++;
    print_str("[PM] Work order "); print_int(pm.n_work_orders - 1);
    print_str(" U"); print_int(unit_id);
    print_str(" type="); print_int(type);
    print_str(" P"); print_int(priority);
    print_str(" month="); print_int(month); print_str("\n");
    return pm.n_work_orders - 1;
}

void pm_resolve_work_order(int wo_id, int month) {
    if (wo_id >= pm.n_work_orders) return;
    work_orders[wo_id].resolved_month = month;
    work_orders[wo_id].status = 3;
    pm.resolved_work_orders++;
    print_str("[PM] Work order "); print_int(wo_id);
    print_str(" resolved month "); print_int(month); print_str("\n");
}

int pm_charge_fee(int unit_id, int type, double amount, int month) {
    if (pm.n_fees >= MAX_FEES || unit_id >= pm.n_units) return -1;
    fee_t* f = &fees[pm.n_fees];
    f->fee_id = pm.n_fees;
    f->unit_id = unit_id;
    f->type = type;
    f->amount = amount;
    f->month = month;
    f->paid = 0;
    f->active = 1;
    pm.n_fees++;
    pm.outstanding_fees += amount;
    print_str("[PM] Fee "); print_int(pm.n_fees - 1);
    print_str(" U"); print_int(unit_id);
    print_str(" type="); print_int(type);
    print_str(" amt="); print_int((int)amount);
    print_str(" month="); print_int(month); print_str("\n");
    return pm.n_fees - 1;
}

void pm_pay_fee(int fee_id) {
    if (fee_id >= pm.n_fees) return;
    if (fees[fee_id].paid == 0) {
        fees[fee_id].paid = 1;
        pm.total_income += fees[fee_id].amount;
        pm.outstanding_fees -= fees[fee_id].amount;
        print_str("[PM] Fee "); print_int(fee_id); print_str(" paid\n");
    }
}

int pm_file_complaint(int unit_id, int category, int priority, int month) {
    if (pm.n_complaints >= MAX_COMPLAINTS || unit_id >= pm.n_units) return -1;
    complaint_t* c = &complaints[pm.n_complaints];
    c->complaint_id = pm.n_complaints;
    c->unit_id = unit_id;
    c->category = category;
    c->priority = priority;
    c->month = month;
    c->resolved = 0;
    c->satisfaction = 0;
    c->active = 1;
    pm.n_complaints++;
    print_str("[PM] Complaint "); print_int(pm.n_complaints - 1);
    print_str(" U"); print_int(unit_id);
    print_str(" cat="); print_int(category);
    print_str(" P"); print_int(priority); print_str("\n");
    return pm.n_complaints - 1;
}

void pm_resolve_complaint(int complaint_id, int satisfaction) {
    if (complaint_id >= pm.n_complaints) return;
    complaints[complaint_id].resolved = 1;
    complaints[complaint_id].satisfaction = satisfaction;
    pm.resolved_complaints++;
    print_str("[PM] Complaint "); print_int(complaint_id);
    print_str(" resolved sat="); print_int(satisfaction); print_str("/10\n");
}

int pm_record_patrol(int guard_id, int month, int day, int hour, int zones) {
    if (pm.n_patrols >= MAX_PATROLS) return -1;
    patrol_t* p = &patrols[pm.n_patrols];
    p->patrol_id = pm.n_patrols;
    p->guard_id = guard_id;
    p->month = month;
    p->day = day;
    p->hour = hour;
    p->zones_covered = zones;
    p->incidents = 0;
    p->active = 1;
    pm.n_patrols++;
    print_str("[PM] Patrol "); print_int(pm.n_patrols - 1);
    print_str(" G"); print_int(guard_id);
    print_str(" "); print_int(month); print_str("/"); print_int(day);
    print_str(" "); print_int(hour); print_str(":00");
    print_str(" zones="); print_int(zones); print_str("\n");
    return pm.n_patrols - 1;
}

int pm_register_visitor(int unit_id, int entry_hour) {
    if (pm.n_visitors >= MAX_VISITORS || unit_id >= pm.n_units) return -1;
    visitor_t* v = &visitors[pm.n_visitors];
    v->visitor_id = pm.n_visitors;
    v->unit_id = unit_id;
    v->entry_hour = entry_hour;
    v->exit_hour = 0;
    v->approved = 1;
    v->active = 1;
    pm.n_visitors++;
    print_str("[PM] Visitor "); print_int(pm.n_visitors - 1);
    print_str(" U"); print_int(unit_id);
    print_str(" entry="); print_int(entry_hour); print_str(":00\n");
    return pm.n_visitors - 1;
}

void pm_visitor_exit(int visitor_id, int exit_hour) {
    if (visitor_id >= pm.n_visitors) return;
    visitors[visitor_id].exit_hour = exit_hour;
    print_str("[PM] Visitor "); print_int(visitor_id);
    print_str(" exit="); print_int(exit_hour); print_str(":00\n");
}

void pm_financial_summary(void) {
    print_str("[PM] Financial summary:\n");
    print_str("  Total income: "); print_int((int)pm.total_income); print_str("\n");
    print_str("  Outstanding: "); print_int((int)pm.outstanding_fees); print_str("\n");
    int paid = 0, unpaid = 0;
    for (int i = 0; i < pm.n_fees; i++) {
        if (fees[i].paid) paid++;
        else unpaid++;
    }
    print_str("  Fees paid: "); print_int(paid); print_str("\n");
    print_str("  Fees unpaid: "); print_int(unpaid); print_str("\n");
    double collection = (pm.n_fees > 0) ? (double)paid / pm.n_fees * 100.0 : 0.0;
    print_str("  Collection rate: "); print_int((int)collection); print_str("%\n");
}

void pm_print_state(void) {
    print_str("[PM] Units="); print_int(pm.n_units);
    print_str(" Equip="); print_int(pm.n_equipment);
    print_str(" WOs="); print_int(pm.n_work_orders);
    print_str(" Fees="); print_int(pm.n_fees);
    print_str(" Complaints="); print_int(pm.n_complaints);
    print_str(" Patrols="); print_int(pm.n_patrols);
    print_str(" Visitors="); print_int(pm.n_visitors);
    print_str("\n");
    print_str("  Resolved WOs: "); print_int(pm.resolved_work_orders); print_str("\n");
    print_str("  Resolved complaints: "); print_int(pm.resolved_complaints); print_str("\n");
}

int main(void) {
    print_str("=== Property Management System Demo ===\n\n");
    pm_init();

    print_str("Adding units...\n");
    for (int i = 0; i < 10; i++) {
        pm_add_unit(i + 1, 50 + i * 15, 800.0 + i * 100.0);
    }

    print_str("\nAdding equipment...\n");
    pm_add_equipment(1, 2018, 6);
    pm_add_equipment(2, 2019, 3);
    pm_add_equipment(3, 2020, 12);
    pm_add_equipment(4, 2017, 6);
    pm_add_equipment(1, 2021, 6);

    print_str("\nServicing equipment...\n");
    pm_service_equipment(0, 6);
    pm_service_equipment(1, 4);
    pm_service_equipment(3, 7);

    print_str("\nCreating work orders...\n");
    pm_create_work_order(0, 1, 2, 1);
    pm_create_work_order(2, 2, 1, 2);
    pm_create_work_order(4, 3, 3, 2);
    pm_create_work_order(5, 1, 2, 3);
    pm_create_work_order(7, 2, 1, 4);
    pm_create_work_order(1, 4, 2, 5);
    pm_create_work_order(3, 1, 3, 6);
    pm_create_work_order(6, 3, 2, 7);

    print_str("\nResolving work orders...\n");
    pm_resolve_work_order(0, 2);
    pm_resolve_work_order(1, 3);
    pm_resolve_work_order(2, 3);
    pm_resolve_work_order(3, 4);
    pm_resolve_work_order(4, 5);
    pm_resolve_work_order(5, 6);

    print_str("\nCharging fees...\n");
    for (int m = 1; m <= 6; m++) {
        for (int u = 0; u < 5; u++) {
            pm_charge_fee(u, 1, units[u].monthly_fee, m);
        }
    }

    print_str("\nPaying fees...\n");
    for (int i = 0; i < pm.n_fees; i += 2) {
        pm_pay_fee(i);
    }
    pm_pay_fee(1);
    pm_pay_fee(5);
    pm_pay_fee(9);
    pm_pay_fee(13);
    pm_pay_fee(17);

    print_str("\nFiling complaints...\n");
    pm_file_complaint(0, 1, 2, 2);
    pm_file_complaint(1, 2, 1, 3);
    pm_file_complaint(3, 1, 3, 4);
    pm_file_complaint(5, 3, 2, 5);
    pm_file_complaint(7, 2, 1, 6);
    pm_file_complaint(2, 1, 2, 7);

    print_str("\nResolving complaints...\n");
    pm_resolve_complaint(0, 8);
    pm_resolve_complaint(1, 9);
    pm_resolve_complaint(2, 7);
    pm_resolve_complaint(3, 8);

    print_str("\nRecording patrols...\n");
    pm_record_patrol(0, 1, 5, 8, 4);
    pm_record_patrol(1, 1, 5, 14, 3);
    pm_record_patrol(0, 1, 6, 8, 4);
    pm_record_patrol(1, 1, 6, 20, 3);
    pm_record_patrol(0, 2, 1, 8, 4);
    pm_record_patrol(1, 2, 1, 14, 3);

    print_str("\nRegistering visitors...\n");
    pm_register_visitor(0, 9);
    pm_register_visitor(2, 10);
    pm_register_visitor(4, 11);
    pm_register_visitor(1, 14);
    pm_register_visitor(3, 15);

    print_str("\nVisitor exits...\n");
    pm_visitor_exit(0, 12);
    pm_visitor_exit(1, 13);
    pm_visitor_exit(3, 17);
    pm_visitor_exit(2, 14);

    print_str("\nFinancial summary...\n");
    pm_financial_summary();

    print_str("\nFinal state...\n");
    pm_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
