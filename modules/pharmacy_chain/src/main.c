/* pharmacy_chain: Pharmacy management (v1.0)
 * Drugs, prescriptions, customers, suppliers, pharmacists, analytics
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

#define MAX_DRUGS       20
#define MAX_PRESCRIPTIONS 14
#define MAX_CUSTOMERS   12
#define MAX_SUPPLIERS   8
#define MAX_PHARMACISTS 6

typedef struct {
    int    drug_id;
    int    category;
    double price;
    double cost;
    int    stock;
    int    expiry_day;
    int    n_sold;
    double revenue;
    int    active;
} drug_t;

typedef struct {
    int    rx_id;
    int    customer_id;
    int    pharmacist_id;
    int    drug_id;
    int    qty;
    double total;
    int    status;
    int    day;
    int    active;
} prescription_t;

typedef struct {
    int    customer_id;
    int    tier;
    int    points;
    double total_spent;
    int    n_visits;
    int    active;
} customer_t;

typedef struct {
    int    supplier_id;
    int    n_drugs;
    double total_orders;
    int    rating;
    int    active;
} supplier_t;

typedef struct {
    int    pharmacist_id;
    int    n_rx;
    int    n_verified;
    int    shift;
    int    active;
} pharmacist_t;

typedef struct {
    int    n_drugs;
    int    n_rx;
    int    n_customers;
    int    n_suppliers;
    int    n_pharmacists;
    double rx_revenue;
    double otc_revenue;
    double total_revenue;
    double total_cost;
    int    total_items_sold;
} pc_state_t;

static drug_t drugs[MAX_DRUGS];
static prescription_t prescriptions[MAX_PRESCRIPTIONS];
static customer_t customers[MAX_CUSTOMERS];
static supplier_t suppliers[MAX_SUPPLIERS];
static pharmacist_t pharmacists[MAX_PHARMACISTS];
static pc_state_t pc;

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

int pc_init(void) {
    if (initialized) return -1;
    pc.n_drugs = 0; pc.n_rx = 0; pc.n_customers = 0;
    pc.n_suppliers = 0; pc.n_pharmacists = 0;
    pc.rx_revenue = 0.0; pc.otc_revenue = 0.0;
    pc.total_revenue = 0.0; pc.total_cost = 0.0;
    pc.total_items_sold = 0;
    for (int i = 0; i < MAX_DRUGS; i++) drugs[i].active = 0;
    for (int i = 0; i < MAX_PRESCRIPTIONS; i++) prescriptions[i].active = 0;
    for (int i = 0; i < MAX_CUSTOMERS; i++) customers[i].active = 0;
    for (int i = 0; i < MAX_SUPPLIERS; i++) suppliers[i].active = 0;
    for (int i = 0; i < MAX_PHARMACISTS; i++) pharmacists[i].active = 0;
    initialized = 1;
    print_str("[PC] Pharmacy chain initialized\n");
    return 0;
}

int pc_add_drug(int category, double price, double cost, int stock, int expiry) {
    if (pc.n_drugs >= MAX_DRUGS) return -1;
    drug_t* d = &drugs[pc.n_drugs];
    d->drug_id = pc.n_drugs;
    d->category = category;
    d->price = price;
    d->cost = cost;
    d->stock = stock;
    d->expiry_day = expiry;
    d->n_sold = 0;
    d->revenue = 0.0;
    d->active = 1;
    pc.n_drugs++;
    print_str("[PC] Drug "); print_int(pc.n_drugs - 1);
    print_str(" cat="); print_int(category);
    print_str(" price="); print_int((int)price);
    print_str(" stock="); print_int(stock);
    print_str(" exp="); print_int(expiry); print_str("\n");
    return pc.n_drugs - 1;
}

int pc_register_customer(int tier) {
    if (pc.n_customers >= MAX_CUSTOMERS) return -1;
    customer_t* c = &customers[pc.n_customers];
    c->customer_id = pc.n_customers;
    c->tier = tier;
    c->points = 0;
    c->total_spent = 0.0;
    c->n_visits = 0;
    c->active = 1;
    pc.n_customers++;
    print_str("[PC] Customer "); print_int(pc.n_customers - 1);
    print_str(" tier="); print_int(tier); print_str("\n");
    return pc.n_customers - 1;
}

int pc_add_supplier(int rating) {
    if (pc.n_suppliers >= MAX_SUPPLIERS) return -1;
    supplier_t* s = &suppliers[pc.n_suppliers];
    s->supplier_id = pc.n_suppliers;
    s->n_drugs = 0;
    s->total_orders = 0.0;
    s->rating = rating;
    s->active = 1;
    pc.n_suppliers++;
    print_str("[PC] Supplier "); print_int(pc.n_suppliers - 1);
    print_str(" rating="); print_int(rating); print_str("\n");
    return pc.n_suppliers - 1;
}

int pc_add_pharmacist(int shift) {
    if (pc.n_pharmacists >= MAX_PHARMACISTS) return -1;
    pharmacist_t* p = &pharmacists[pc.n_pharmacists];
    p->pharmacist_id = pc.n_pharmacists;
    p->n_rx = 0;
    p->n_verified = 0;
    p->shift = shift;
    p->active = 1;
    pc.n_pharmacists++;
    print_str("[PC] Pharmacist "); print_int(pc.n_pharmacists - 1);
    print_str(" shift="); print_int(shift); print_str("\n");
    return pc.n_pharmacists - 1;
}

int pc_fill_prescription(int customer_id, int pharmacist_id, int drug_id, int qty, int day) {
    if (pc.n_rx >= MAX_PRESCRIPTIONS) return -1;
    if (customer_id >= pc.n_customers || pharmacist_id >= pc.n_pharmacists) return -2;
    if (drug_id >= pc.n_drugs) return -3;
    drug_t* d = &drugs[drug_id];
    if (d->stock < qty) return -4;
    prescription_t* rx = &prescriptions[pc.n_rx];
    rx->rx_id = pc.n_rx;
    rx->customer_id = customer_id;
    rx->pharmacist_id = pharmacist_id;
    rx->drug_id = drug_id;
    rx->qty = qty;
    rx->total = d->price * qty;
    rx->status = 3;
    rx->day = day;
    rx->active = 1;
    d->stock -= qty;
    d->n_sold += qty;
    d->revenue += rx->total;
    customers[customer_id].total_spent += rx->total;
    customers[customer_id].points += (int)rx->total;
    customers[customer_id].n_visits++;
    pharmacists[pharmacist_id].n_rx++;
    pharmacists[pharmacist_id].n_verified++;
    pc.rx_revenue += rx->total;
    pc.total_revenue += rx->total;
    pc.total_cost += d->cost * qty;
    pc.total_items_sold += qty;
    pc.n_rx++;
    print_str("[PC] Rx "); print_int(pc.n_rx - 1);
    print_str(" C"); print_int(customer_id);
    print_str(" Ph"); print_int(pharmacist_id);
    print_str(" D"); print_int(drug_id);
    print_str(" qty="); print_int(qty);
    print_str(" total="); print_int((int)rx->total); print_str("\n");
    return pc.n_rx - 1;
}

int pc_sell_otc(int customer_id, int drug_id, int qty) {
    if (customer_id >= pc.n_customers || drug_id >= pc.n_drugs) return -1;
    drug_t* d = &drugs[drug_id];
    if (d->stock < qty) return -2;
    d->stock -= qty;
    d->n_sold += qty;
    double total = d->price * qty;
    d->revenue += total;
    customers[customer_id].total_spent += total;
    customers[customer_id].points += (int)total;
    customers[customer_id].n_visits++;
    pc.otc_revenue += total;
    pc.total_revenue += total;
    pc.total_cost += d->cost * qty;
    pc.total_items_sold += qty;
    print_str("[PC] OTC sale C"); print_int(customer_id);
    print_str(" D"); print_int(drug_id);
    print_str(" qty="); print_int(qty);
    print_str(" total="); print_int((int)total); print_str("\n");
    return 0;
}

void pc_drug_inventory(void) {
    print_str("[PC] Drug inventory:\n");
    for (int i = 0; i < pc.n_drugs; i++) {
        drug_t* d = &drugs[i];
        print_str("  D"); print_int(i);
        print_str(" cat="); print_int(d->category);
        print_str(" stock="); print_int(d->stock);
        print_str(" sold="); print_int(d->n_sold);
        print_str(" rev="); print_int((int)d->revenue);
        print_str(" exp="); print_int(d->expiry_day); print_str("\n");
    }
}

void pc_pharmacist_report(void) {
    print_str("[PC] Pharmacist report:\n");
    for (int i = 0; i < pc.n_pharmacists; i++) {
        pharmacist_t* p = &pharmacists[i];
        print_str("  Ph"); print_int(i);
        print_str(" shift="); print_int(p->shift);
        print_str(" rx="); print_int(p->n_rx);
        print_str(" verified="); print_int(p->n_verified); print_str("\n");
    }
}

void pc_financial_report(void) {
    print_str("[PC] Financial report:\n");
    double profit = pc.total_revenue - pc.total_cost;
    print_str("  Rx revenue: "); print_int((int)pc.rx_revenue); print_str("\n");
    print_str("  OTC revenue: "); print_int((int)pc.otc_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)pc.total_revenue); print_str("\n");
    print_str("  Total cost: "); print_int((int)pc.total_cost); print_str("\n");
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    print_str("  Items sold: "); print_int(pc.total_items_sold); print_str("\n");
}

void pc_print_state(void) {
    print_str("[PC] Drugs="); print_int(pc.n_drugs);
    print_str(" Rx="); print_int(pc.n_rx);
    print_str(" Customers="); print_int(pc.n_customers);
    print_str(" Suppliers="); print_int(pc.n_suppliers);
    print_str(" Pharmacists="); print_int(pc.n_pharmacists);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)pc.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Pharmacy Chain Demo ===\n\n");
    pc_init();

    print_str("Adding drugs...\n");
    pc_add_drug(1, 25.0, 10.0, 100, 365);
    pc_add_drug(1, 45.0, 18.0, 80, 270);
    pc_add_drug(1, 18.0, 6.0, 150, 180);
    pc_add_drug(2, 12.0, 4.0, 200, 365);
    pc_add_drug(2, 8.5, 2.5, 250, 270);
    pc_add_drug(2, 15.0, 5.0, 180, 180);
    pc_add_drug(3, 35.0, 15.0, 60, 365);
    pc_add_drug(3, 28.0, 12.0, 90, 270);
    pc_add_drug(1, 55.0, 22.0, 70, 365);
    pc_add_drug(2, 10.0, 3.0, 300, 180);
    pc_add_drug(1, 32.0, 14.0, 110, 270);
    pc_add_drug(3, 42.0, 18.0, 75, 365);
    pc_add_drug(2, 9.0, 3.0, 280, 180);
    pc_add_drug(1, 38.0, 16.0, 95, 365);
    pc_add_drug(2, 14.0, 5.0, 220, 270);
    pc_add_drug(3, 50.0, 20.0, 65, 365);
    pc_add_drug(1, 22.0, 9.0, 130, 270);
    pc_add_drug(2, 11.0, 4.0, 260, 180);
    pc_add_drug(3, 30.0, 13.0, 85, 365);
    pc_add_drug(1, 48.0, 20.0, 78, 270);

    print_str("\nRegistering customers...\n");
    for (int i = 0; i < 10; i++) {
        pc_register_customer(i % 3);
    }

    print_str("\nAdding suppliers...\n");
    pc_add_supplier(92);
    pc_add_supplier(88);
    pc_add_supplier(95);
    pc_add_supplier(85);
    pc_add_supplier(90);

    print_str("\nAdding pharmacists...\n");
    pc_add_pharmacist(1);
    pc_add_pharmacist(2);
    pc_add_pharmacist(1);
    pc_add_pharmacist(2);
    pc_add_pharmacist(1);
    pc_add_pharmacist(2);

    print_str("\nFilling prescriptions...\n");
    pc_fill_prescription(0, 0, 0, 2, 10);
    pc_fill_prescription(1, 1, 1, 1, 10);
    pc_fill_prescription(2, 2, 2, 3, 11);
    pc_fill_prescription(3, 3, 8, 1, 11);
    pc_fill_prescription(4, 4, 3, 2, 12);
    pc_fill_prescription(5, 5, 6, 1, 12);
    pc_fill_prescription(6, 0, 10, 2, 13);
    pc_fill_prescription(7, 1, 11, 1, 13);
    pc_fill_prescription(8, 2, 13, 3, 14);
    pc_fill_prescription(9, 3, 4, 2, 14);
    pc_fill_prescription(0, 4, 14, 1, 15);
    pc_fill_prescription(1, 5, 16, 2, 15);
    pc_fill_prescription(2, 0, 17, 3, 16);
    pc_fill_prescription(3, 1, 19, 1, 16);

    print_str("\nOTC sales...\n");
    pc_sell_otc(4, 5, 2);
    pc_sell_otc(5, 7, 1);
    pc_sell_otc(6, 9, 3);
    pc_sell_otc(7, 12, 2);
    pc_sell_otc(8, 15, 1);
    pc_sell_otc(9, 18, 2);
    pc_sell_otc(0, 5, 1);
    pc_sell_otc(1, 7, 2);

    print_str("\nDrug inventory...\n");
    pc_drug_inventory();

    print_str("\nPharmacist report...\n");
    pc_pharmacist_report();

    print_str("\nFinancial report...\n");
    pc_financial_report();

    print_str("\nFinal state...\n");
    pc_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
