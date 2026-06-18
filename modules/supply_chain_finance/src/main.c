/* supply_chain_finance: Supply chain finance system (v1.0)
 * AR financing, advance payment financing, inventory financing, core credit, risk models
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

#define MAX_SUPPLIERS    8
#define MAX_INVOICES     16
#define MAX_ORDERS       12
#define MAX_INVENTORY    12
#define MAX_CREDITS      8
#define MAX_TRANSACTIONS 16

typedef struct {
    int    supplier_id;
    int    tier;
    double credit_limit;
    double credit_used;
    int    credit_rating;
    double default_prob;
    int    active;
} supplier_t;

typedef struct {
    int    invoice_id;
    int    supplier_id;
    double amount;
    int    due_days;
    double discount_rate;
    double financed_amount;
    int    status;
    int    active;
} invoice_t;

typedef struct {
    int    order_id;
    int    supplier_id;
    double amount;
    double advance_pct;
    double advance_amount;
    double lc_amount;
    int    status;
    int    active;
} order_t;

typedef struct {
    int    inventory_id;
    int    supplier_id;
    double quantity;
    double unit_value;
    double total_value;
    double pledge_ratio;
    double loan_amount;
    int    active;
} inventory_t;

typedef struct {
    int    credit_id;
    int    origin_supplier;
    double total_credit;
    double used_credit;
    int    n_transfers;
    int    active;
} credit_t;

typedef struct {
    int    tx_id;
    int    from_supplier;
    int    to_supplier;
    double amount;
    int    risk_score;
    int    active;
} transaction_t;

typedef struct {
    int    n_suppliers;
    int    n_invoices;
    int    n_orders;
    int    n_inventory;
    int    n_credits;
    int    n_transactions;
    double total_ar_financed;
    double total_advance;
    double total_inventory_loan;
    double total_credit_transferred;
    int    risk_alerts;
    int    fraud_detections;
} scf_state_t;

static supplier_t suppliers[MAX_SUPPLIERS];
static invoice_t invoices[MAX_INVOICES];
static order_t orders[MAX_ORDERS];
static inventory_t inventory[MAX_INVENTORY];
static credit_t credits[MAX_CREDITS];
static transaction_t transactions[MAX_TRANSACTIONS];
static scf_state_t scf;

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

int scf_init(void) {
    if (initialized) return -1;
    scf.n_suppliers = 0; scf.n_invoices = 0; scf.n_orders = 0;
    scf.n_inventory = 0; scf.n_credits = 0; scf.n_transactions = 0;
    scf.total_ar_financed = 0.0; scf.total_advance = 0.0;
    scf.total_inventory_loan = 0.0; scf.total_credit_transferred = 0.0;
    scf.risk_alerts = 0; scf.fraud_detections = 0;
    for (int i = 0; i < MAX_SUPPLIERS; i++) suppliers[i].active = 0;
    for (int i = 0; i < MAX_INVOICES; i++) invoices[i].active = 0;
    for (int i = 0; i < MAX_ORDERS; i++) orders[i].active = 0;
    for (int i = 0; i < MAX_INVENTORY; i++) inventory[i].active = 0;
    for (int i = 0; i < MAX_CREDITS; i++) credits[i].active = 0;
    for (int i = 0; i < MAX_TRANSACTIONS; i++) transactions[i].active = 0;
    initialized = 1;
    print_str("[SCF] Supply chain finance initialized\n");
    return 0;
}

int scf_add_supplier(int tier, double credit_limit, int rating) {
    if (scf.n_suppliers >= MAX_SUPPLIERS) return -1;
    supplier_t* s = &suppliers[scf.n_suppliers];
    s->supplier_id = scf.n_suppliers;
    s->tier = tier;
    s->credit_limit = credit_limit;
    s->credit_used = 0.0;
    s->credit_rating = rating;
    s->default_prob = 0.005 + (double)(6 - rating) * 0.01;
    s->active = 1;
    scf.n_suppliers++;
    print_str("[SCF] Supplier "); print_int(scf.n_suppliers - 1);
    print_str(" tier="); print_int(tier);
    print_str(" limit="); print_int((int)credit_limit);
    print_str(" rating="); print_int(rating);
    print_str(" PD="); print_int((int)(s->default_prob * 10000)); print_str("bp\n");
    return scf.n_suppliers - 1;
}

int scf_invoice_discounting(int supplier_id, double amount, int due_days) {
    if (scf.n_invoices >= MAX_INVOICES || supplier_id >= scf.n_suppliers) return -1;
    invoice_t* inv = &invoices[scf.n_invoices];
    inv->invoice_id = scf.n_invoices;
    inv->supplier_id = supplier_id;
    inv->amount = amount;
    inv->due_days = due_days;
    inv->discount_rate = 0.06 + (double)(due_days / 30) * 0.005;
    inv->financed_amount = amount * (1.0 - inv->discount_rate * (double)due_days / 360.0);
    inv->status = 1;
    inv->active = 1;
    scf.n_invoices++;
    scf.total_ar_financed += inv->financed_amount;
    suppliers[supplier_id].credit_used += inv->financed_amount;
    print_str("[SCF] Invoice discount "); print_int(scf.n_invoices - 1);
    print_str(" sup="); print_int(supplier_id);
    print_str(" amt="); print_int((int)amount);
    print_str(" days="); print_int(due_days);
    print_str(" rate="); print_int((int)(inv->discount_rate * 10000)); print_str("bp");
    print_str(" financed="); print_int((int)inv->financed_amount); print_str("\n");
    return scf.n_invoices - 1;
}

int scf_factoring(int supplier_id, double amount) {
    if (scf.n_invoices >= MAX_INVOICES || supplier_id >= scf.n_suppliers) return -1;
    invoice_t* inv = &invoices[scf.n_invoices];
    inv->invoice_id = scf.n_invoices;
    inv->supplier_id = supplier_id;
    inv->amount = amount;
    inv->due_days = 90;
    inv->discount_rate = 0.03;
    inv->financed_amount = amount * 0.97;
    inv->status = 2;
    inv->active = 1;
    scf.n_invoices++;
    scf.total_ar_financed += inv->financed_amount;
    suppliers[supplier_id].credit_used += inv->financed_amount;
    print_str("[SCF] Factoring "); print_int(scf.n_invoices - 1);
    print_str(" sup="); print_int(supplier_id);
    print_str(" amt="); print_int((int)amount);
    print_str(" fee="); print_int((int)(amount * 0.03));
    print_str(" funded="); print_int((int)inv->financed_amount); print_str("\n");
    return scf.n_invoices - 1;
}

int scf_order_financing(int supplier_id, double amount, double advance_pct) {
    if (scf.n_orders >= MAX_ORDERS || supplier_id >= scf.n_suppliers) return -1;
    order_t* o = &orders[scf.n_orders];
    o->order_id = scf.n_orders;
    o->supplier_id = supplier_id;
    o->amount = amount;
    o->advance_pct = advance_pct;
    o->advance_amount = amount * advance_pct;
    o->lc_amount = amount * (1.0 - advance_pct);
    o->status = 1;
    o->active = 1;
    scf.n_orders++;
    scf.total_advance += o->advance_amount;
    suppliers[supplier_id].credit_used += o->advance_amount;
    print_str("[SCF] Order finance "); print_int(scf.n_orders - 1);
    print_str(" sup="); print_int(supplier_id);
    print_str(" amt="); print_int((int)amount);
    print_str(" advance="); print_int((int)advance_pct); print_str("%");
    print_str(" funded="); print_int((int)o->advance_amount);
    print_str(" LC="); print_int((int)o->lc_amount); print_str("\n");
    return scf.n_orders - 1;
}

int scf_letter_of_credit(int order_id, double amount) {
    if (order_id >= scf.n_orders) return -1;
    order_t* o = &orders[order_id];
    o->lc_amount += amount;
    print_str("[SCF] LC issued order="); print_int(order_id);
    print_str(" amt="); print_int((int)amount);
    print_str(" total_LC="); print_int((int)o->lc_amount); print_str("\n");
    return 0;
}

int scf_inventory_pledge(int supplier_id, double qty, double unit_value, double pledge_ratio) {
    if (scf.n_inventory >= MAX_INVENTORY || supplier_id >= scf.n_suppliers) return -1;
    inventory_t* inv = &inventory[scf.n_inventory];
    inv->inventory_id = scf.n_inventory;
    inv->supplier_id = supplier_id;
    inv->quantity = qty;
    inv->unit_value = unit_value;
    inv->total_value = qty * unit_value;
    inv->pledge_ratio = pledge_ratio;
    inv->loan_amount = inv->total_value * pledge_ratio;
    inv->active = 1;
    scf.n_inventory++;
    scf.total_inventory_loan += inv->loan_amount;
    suppliers[supplier_id].credit_used += inv->loan_amount;
    print_str("[SCF] Inventory pledge "); print_int(scf.n_inventory - 1);
    print_str(" sup="); print_int(supplier_id);
    print_str(" qty="); print_int((int)qty);
    print_str(" val="); print_int((int)inv->total_value);
    print_str(" pledge="); print_int((int)(pledge_ratio * 100)); print_str("%");
    print_str(" loan="); print_int((int)inv->loan_amount); print_str("\n");
    return scf.n_inventory - 1;
}

int scf_create_credit(int origin_supplier, double credit_amount) {
    if (scf.n_credits >= MAX_CREDITS || origin_supplier >= scf.n_suppliers) return -1;
    credit_t* c = &credits[scf.n_credits];
    c->credit_id = scf.n_credits;
    c->origin_supplier = origin_supplier;
    c->total_credit = credit_amount;
    c->used_credit = 0.0;
    c->n_transfers = 0;
    c->active = 1;
    scf.n_credits++;
    print_str("[SCF] Credit issued "); print_int(scf.n_credits - 1);
    print_str(" origin="); print_int(origin_supplier);
    print_str(" amount="); print_int((int)credit_amount); print_str("\n");
    return scf.n_credits - 1;
}

int scf_transfer_credit(int credit_id, int to_supplier, double amount) {
    if (credit_id >= scf.n_credits || to_supplier >= scf.n_suppliers) return -1;
    credit_t* c = &credits[credit_id];
    if (c->used_credit + amount > c->total_credit) return -2;
    c->used_credit += amount;
    c->n_transfers++;
    scf.total_credit_transferred += amount;
    suppliers[to_supplier].credit_used += amount;
    if (scf.n_transactions < MAX_TRANSACTIONS) {
        transaction_t* t = &transactions[scf.n_transactions];
        t->tx_id = scf.n_transactions;
        t->from_supplier = c->origin_supplier;
        t->to_supplier = to_supplier;
        t->amount = amount;
        t->risk_score = 0;
        t->active = 1;
        scf.n_transactions++;
    }
    print_str("[SCF] Credit transfer credit="); print_int(credit_id);
    print_str(" to="); print_int(to_supplier);
    print_str(" amt="); print_int((int)amount);
    print_str(" used="); print_int((int)c->used_credit);
    print_str("/"); print_int((int)c->total_credit); print_str("\n");
    return 0;
}

int scf_trade_verification(int from_supplier, int to_supplier, double amount) {
    int risk = 0;
    if (dabs((double)from_supplier - (double)to_supplier) < 1.0) {
        print_str("[SCF] ALERT: Self-trade detected S"); print_int(from_supplier);
        print_str("->S"); print_int(to_supplier); print_str("\n");
        risk += 50;
        scf.fraud_detections++;
    }
    if (amount > 100000.0) {
        print_str("[SCF] ALERT: Large trade S"); print_int(from_supplier);
        print_str("->S"); print_int(to_supplier);
        print_str(" amt="); print_int((int)amount); print_str("\n");
        risk += 20;
    }
    if (risk > 30) scf.risk_alerts++;
    print_str("[SCF] Verify trade S"); print_int(from_supplier);
    print_str("->S"); print_int(to_supplier);
    print_str(" amt="); print_int((int)amount);
    print_str(" risk="); print_int(risk); print_str("\n");
    return risk;
}

void scf_risk_assessment(void) {
    print_str("[SCF] Risk assessment:\n");
    for (int i = 0; i < scf.n_suppliers; i++) {
        supplier_t* s = &suppliers[i];
        double utilization = s->credit_used / (s->credit_limit + 1e-10) * 100.0;
        int risk_level = 0;
        if (utilization > 80.0) risk_level = 3;
        else if (utilization > 60.0) risk_level = 2;
        else if (utilization > 40.0) risk_level = 1;
        print_str("  Supplier "); print_int(i);
        print_str(" tier="); print_int(s->tier);
        print_str(" used="); print_int((int)s->credit_used);
        print_str("/"); print_int((int)s->credit_limit);
        print_str(" util="); print_int((int)utilization); print_str("%");
        print_str(" risk="); print_int(risk_level); print_str("\n");
        if (risk_level >= 3) scf.risk_alerts++;
    }
    print_str("  Total alerts: "); print_int(scf.risk_alerts); print_str("\n");
    print_str("  Fraud detections: "); print_int(scf.fraud_detections); print_str("\n");
}

void scf_print_state(void) {
    print_str("[SCF] Suppliers="); print_int(scf.n_suppliers);
    print_str(" Invoices="); print_int(scf.n_invoices);
    print_str(" Orders="); print_int(scf.n_orders);
    print_str(" Inventory="); print_int(scf.n_inventory);
    print_str(" Credits="); print_int(scf.n_credits);
    print_str("\n");
    print_str("  AR financed: "); print_int((int)scf.total_ar_financed); print_str("\n");
    print_str("  Advances: "); print_int((int)scf.total_advance); print_str("\n");
    print_str("  Inv loans: "); print_int((int)scf.total_inventory_loan); print_str("\n");
    print_str("  Credit xfer: "); print_int((int)scf.total_credit_transferred); print_str("\n");
    print_str("  Alerts: "); print_int(scf.risk_alerts);
    print_str(" Frauds: "); print_int(scf.fraud_detections); print_str("\n");
}

int main(void) {
    print_str("=== Supply Chain Finance System Demo ===\n\n");
    scf_init();

    print_str("Adding suppliers...\n");
    scf_add_supplier(1, 100000.0, 2);
    scf_add_supplier(1, 80000.0, 3);
    scf_add_supplier(2, 50000.0, 4);
    scf_add_supplier(2, 60000.0, 3);
    scf_add_supplier(3, 30000.0, 5);

    print_str("\nInvoice discounting...\n");
    scf_invoice_discounting(0, 50000.0, 60);
    scf_invoice_discounting(1, 30000.0, 90);
    scf_invoice_discounting(2, 20000.0, 45);

    print_str("\nFactoring...\n");
    scf_factoring(0, 40000.0);
    scf_factoring(3, 25000.0);

    print_str("\nOrder financing...\n");
    scf_order_financing(0, 100000.0, 30.0);
    scf_order_financing(1, 75000.0, 25.0);
    scf_order_financing(4, 40000.0, 40.0);

    print_str("\nIssuing letters of credit...\n");
    scf_letter_of_credit(0, 50000.0);
    scf_letter_of_credit(1, 30000.0);

    print_str("\nInventory pledges...\n");
    scf_inventory_pledge(0, 1000.0, 50.0, 0.70);
    scf_inventory_pledge(1, 500.0, 100.0, 0.65);
    scf_inventory_pledge(2, 200.0, 200.0, 0.60);

    print_str("\nCreating core enterprise credits...\n");
    scf_create_credit(0, 200000.0);
    scf_create_credit(1, 150000.0);

    print_str("\nTransferring credits...\n");
    scf_transfer_credit(0, 2, 50000.0);
    scf_transfer_credit(0, 3, 30000.0);
    scf_transfer_credit(0, 4, 20000.0);
    scf_transfer_credit(1, 2, 40000.0);
    scf_transfer_credit(1, 4, 15000.0);

    print_str("\nTrade verification...\n");
    scf_trade_verification(0, 2, 50000.0);
    scf_trade_verification(1, 3, 30000.0);
    scf_trade_verification(2, 4, 20000.0);
    scf_trade_verification(0, 0, 150000.0);
    scf_trade_verification(3, 3, 120000.0);

    print_str("\nRisk assessment...\n");
    scf_risk_assessment();

    print_str("\nFinal state...\n");
    scf_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
