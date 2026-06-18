/* retail_store: Retail store management (v1.0)
 * Products, POS, inventory, loyalty, sales analytics
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

#define MAX_PRODUCTS   20
#define MAX_CUSTOMERS  14
#define MAX_SALES      18
#define MAX_SUPPLIERS  8
#define MAX_CATEGORIES 6

typedef struct {
    int    product_id;
    int    category;
    double price;
    double cost;
    int    stock;
    int    reorder_level;
    int    n_sold;
    double revenue;
    int    active;
} product_t;

typedef struct {
    int    customer_id;
    int    tier;
    int    points;
    double total_spent;
    int    n_visits;
    int    active;
} customer_t;

typedef struct {
    int    sale_id;
    int    customer_id;
    int    product_id;
    int    qty;
    double unit_price;
    double discount;
    double total;
    int    day;
    int    active;
} sale_t;

typedef struct {
    int    supplier_id;
    int    n_products;
    double total_orders;
    double outstanding;
    int    rating;
    int    active;
} supplier_t;

typedef struct {
    int    n_products;
    int    n_customers;
    int    n_sales;
    int    n_suppliers;
    int    n_categories;
    double total_revenue;
    double total_cost;
    double total_discount;
    double loyalty_discount;
    int    total_items_sold;
} rs_state_t;

static product_t products[MAX_PRODUCTS];
static customer_t customers[MAX_CUSTOMERS];
static sale_t sales[MAX_SALES];
static supplier_t suppliers[MAX_SUPPLIERS];
static rs_state_t rs;

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

int rs_init(void) {
    if (initialized) return -1;
    rs.n_products = 0; rs.n_customers = 0; rs.n_sales = 0;
    rs.n_suppliers = 0; rs.n_categories = 0;
    rs.total_revenue = 0.0; rs.total_cost = 0.0;
    rs.total_discount = 0.0; rs.loyalty_discount = 0.0;
    rs.total_items_sold = 0;
    for (int i = 0; i < MAX_PRODUCTS; i++) products[i].active = 0;
    for (int i = 0; i < MAX_CUSTOMERS; i++) customers[i].active = 0;
    for (int i = 0; i < MAX_SALES; i++) sales[i].active = 0;
    for (int i = 0; i < MAX_SUPPLIERS; i++) suppliers[i].active = 0;
    initialized = 1;
    print_str("[RS] Retail store initialized\n");
    return 0;
}

int rs_add_category(void) {
    if (rs.n_categories >= MAX_CATEGORIES) return -1;
    rs.n_categories++;
    print_str("[RS] Category "); print_int(rs.n_categories - 1); print_str(" added\n");
    return rs.n_categories - 1;
}

int rs_add_product(int category, double price, double cost, int stock, int reorder) {
    if (rs.n_products >= MAX_PRODUCTS) return -1;
    product_t* p = &products[rs.n_products];
    p->product_id = rs.n_products;
    p->category = category;
    p->price = price;
    p->cost = cost;
    p->stock = stock;
    p->reorder_level = reorder;
    p->n_sold = 0;
    p->revenue = 0.0;
    p->active = 1;
    rs.n_products++;
    print_str("[RS] Product "); print_int(rs.n_products - 1);
    print_str(" cat="); print_int(category);
    print_str(" price="); print_int((int)price);
    print_str(" stock="); print_int(stock); print_str("\n");
    return rs.n_products - 1;
}

int rs_register_customer(int tier) {
    if (rs.n_customers >= MAX_CUSTOMERS) return -1;
    customer_t* c = &customers[rs.n_customers];
    c->customer_id = rs.n_customers;
    c->tier = tier;
    c->points = 0;
    c->total_spent = 0.0;
    c->n_visits = 0;
    c->active = 1;
    rs.n_customers++;
    print_str("[RS] Customer "); print_int(rs.n_customers - 1);
    print_str(" tier="); print_int(tier); print_str("\n");
    return rs.n_customers - 1;
}

int rs_add_supplier(int rating) {
    if (rs.n_suppliers >= MAX_SUPPLIERS) return -1;
    supplier_t* s = &suppliers[rs.n_suppliers];
    s->supplier_id = rs.n_suppliers;
    s->n_products = 0;
    s->total_orders = 0.0;
    s->outstanding = 0.0;
    s->rating = rating;
    s->active = 1;
    rs.n_suppliers++;
    print_str("[RS] Supplier "); print_int(rs.n_suppliers - 1);
    print_str(" rating="); print_int(rating); print_str("\n");
    return rs.n_suppliers - 1;
}

int rs_make_sale(int customer_id, int product_id, int qty, int day) {
    if (rs.n_sales >= MAX_SALES) return -1;
    if (customer_id >= rs.n_customers || product_id >= rs.n_products) return -2;
    product_t* p = &products[product_id];
    if (p->stock < qty) return -3;
    customer_t* c = &customers[customer_id];
    sale_t* s = &sales[rs.n_sales];
    s->sale_id = rs.n_sales;
    s->customer_id = customer_id;
    s->product_id = product_id;
    s->qty = qty;
    s->unit_price = p->price;
    double tier_discount[4] = {0.0, 0.05, 0.10, 0.15};
    s->discount = p->price * qty * tier_discount[c->tier < 4 ? c->tier : 0];
    s->total = p->price * qty - s->discount;
    s->day = day;
    s->active = 1;
    p->stock -= qty;
    p->n_sold += qty;
    p->revenue += s->total;
    c->total_spent += s->total;
    c->points += (int)(s->total * 1.0);
    c->n_visits++;
    rs.total_revenue += s->total;
    rs.total_cost += p->cost * qty;
    rs.total_discount += s->discount;
    rs.loyalty_discount += s->discount;
    rs.total_items_sold += qty;
    rs.n_sales++;
    print_str("[RS] Sale "); print_int(rs.n_sales - 1);
    print_str(" C"); print_int(customer_id);
    print_str(" P"); print_int(product_id);
    print_str(" qty="); print_int(qty);
    print_str(" total="); print_int((int)s->total);
    print_str(" disc="); print_int((int)s->discount); print_str("\n");
    return rs.n_sales - 1;
}

void rs_restock(int product_id, int qty) {
    if (product_id >= rs.n_products) return;
    products[product_id].stock += qty;
    print_str("[RS] Restock P"); print_int(product_id);
    print_str(" +"); print_int(qty);
    print_str(" now="); print_int(products[product_id].stock); print_str("\n");
}

void rs_low_stock_alert(void) {
    print_str("[RS] Low stock alert:\n");
    for (int i = 0; i < rs.n_products; i++) {
        product_t* p = &products[i];
        if (p->stock <= p->reorder_level) {
            print_str("  P"); print_int(i);
            print_str(" cat="); print_int(p->category);
            print_str(" stock="); print_int(p->stock);
            print_str(" reorder="); print_int(p->reorder_level); print_str("\n");
        }
    }
}

void rs_sales_report(void) {
    print_str("[RS] Sales report:\n");
    for (int i = 0; i < rs.n_products; i++) {
        product_t* p = &products[i];
        if (p->n_sold > 0) {
            print_str("  P"); print_int(i);
            print_str(" cat="); print_int(p->category);
            print_str(" sold="); print_int(p->n_sold);
            print_str(" rev="); print_int((int)p->revenue); print_str("\n");
        }
    }
}

void rs_financial_report(void) {
    print_str("[RS] Financial report:\n");
    double profit = rs.total_revenue - rs.total_cost;
    print_str("  Total revenue: "); print_int((int)rs.total_revenue); print_str("\n");
    print_str("  Total cost: "); print_int((int)rs.total_cost); print_str("\n");
    print_str("  Total discount: "); print_int((int)rs.total_discount); print_str("\n");
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    print_str("  Items sold: "); print_int(rs.total_items_sold); print_str("\n");
    print_str("  Customers: "); print_int(rs.n_customers); print_str("\n");
}

void rs_print_state(void) {
    print_str("[RS] Products="); print_int(rs.n_products);
    print_str(" Customers="); print_int(rs.n_customers);
    print_str(" Sales="); print_int(rs.n_sales);
    print_str(" Suppliers="); print_int(rs.n_suppliers);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)rs.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Retail Store Demo ===\n\n");
    rs_init();

    print_str("Adding categories...\n");
    for (int i = 0; i < 5; i++) rs_add_category();

    print_str("\nAdding products...\n");
    rs_add_product(0, 12.99, 5.00, 100, 20);
    rs_add_product(0, 8.99, 3.50, 150, 30);
    rs_add_product(0, 24.99, 10.0, 60, 15);
    rs_add_product(1, 45.00, 20.0, 40, 10);
    rs_add_product(1, 32.50, 14.0, 55, 12);
    rs_add_product(2, 19.99, 8.0, 80, 20);
    rs_add_product(2, 15.50, 6.0, 90, 25);
    rs_add_product(2, 28.00, 12.0, 45, 10);
    rs_add_product(3, 55.00, 25.0, 30, 8);
    rs_add_product(3, 42.00, 18.0, 35, 10);
    rs_add_product(3, 68.00, 30.0, 25, 6);
    rs_add_product(4, 9.99, 3.0, 200, 40);
    rs_add_product(4, 14.99, 5.5, 120, 25);
    rs_add_product(4, 7.50, 2.5, 180, 35);
    rs_add_product(0, 35.00, 15.0, 50, 12);
    rs_add_product(1, 22.00, 9.0, 65, 15);
    rs_add_product(2, 18.50, 7.0, 70, 18);
    rs_add_product(3, 75.00, 35.0, 20, 5);
    rs_add_product(4, 11.99, 4.0, 160, 30);
    rs_add_product(0, 29.99, 12.0, 55, 14);

    print_str("\nRegistering customers...\n");
    for (int i = 0; i < 12; i++) {
        rs_register_customer(i % 4);
    }

    print_str("\nAdding suppliers...\n");
    rs_add_supplier(92);
    rs_add_supplier(88);
    rs_add_supplier(95);
    rs_add_supplier(85);
    rs_add_supplier(90);

    print_str("\nProcessing sales...\n");
    rs_make_sale(0, 0, 3, 10);
    rs_make_sale(1, 1, 2, 10);
    rs_make_sale(2, 3, 1, 11);
    rs_make_sale(3, 5, 4, 11);
    rs_make_sale(4, 8, 1, 12);
    rs_make_sale(5, 11, 5, 12);
    rs_make_sale(6, 2, 2, 13);
    rs_make_sale(7, 6, 3, 13);
    rs_make_sale(8, 10, 1, 14);
    rs_make_sale(9, 13, 4, 14);
    rs_make_sale(10, 4, 2, 15);
    rs_make_sale(11, 7, 3, 15);
    rs_make_sale(0, 9, 1, 16);
    rs_make_sale(1, 12, 2, 16);
    rs_make_sale(2, 15, 3, 17);
    rs_make_sale(3, 16, 2, 17);
    rs_make_sale(4, 17, 1, 18);
    rs_make_sale(5, 18, 5, 18);

    print_str("\nRestocking...\n");
    rs_restock(1, 50);
    rs_restock(6, 30);
    rs_restock(11, 80);
    rs_restock(13, 60);

    print_str("\nLow stock alerts...\n");
    rs_low_stock_alert();

    print_str("\nSales report...\n");
    rs_sales_report();

    print_str("\nFinancial report...\n");
    rs_financial_report();

    print_str("\nFinal state...\n");
    rs_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
