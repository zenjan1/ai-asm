/* food_supply_chain: Food supply chain traceability system (v1.0)
 * Origin tracking, quality control, cold chain, inventory, consumer queries
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

#define MAX_PRODUCTS   16
#define MAX_BATCHES    16
#define MAX_TRANSPORT  8
#define MAX_STORAGE    8
#define MAX_QUERIES    8

#define FOOD_GRAIN     0x01
#define FOOD_VEGETABLE 0x02
#define FOOD_FRUIT     0x03
#define FOOD_MEAT      0x04
#define FOOD_DAIRY     0x05
#define FOOD_SEAFOOD   0x06

#define QUALITY_A      0x01
#define QUALITY_B      0x02
#define QUALITY_C      0x03

typedef struct {
    int    food_type;
    double shelf_life_days;
    double optimal_temp;
    double optimal_humidity;
    double price_per_kg;
    int    farm_id;
    int    id;
    int    active;
} product_t;

typedef struct {
    int    product_id;
    int    batch_number;
    double quantity_kg;
    int    harvest_date;
    int    expiry_date;
    int    quality_grade;
    double safety_score;
    int    certified;
    int    id;
    int    active;
} batch_t;

typedef struct {
    int    batch_id;
    double temp_current;
    double temp_optimal;
    double humidity;
    double duration_hours;
    int    temp_violations;
    int    in_transit;
    int    id;
    int    active;
} transport_t;

typedef struct {
    int    batch_id;
    double quantity_kg;
    double temp_current;
    int    days_remaining;
    int    fifo_position;
    int    alert;
    int    id;
    int    active;
} storage_t;

typedef struct {
    int    batch_id;
    int    consumer_id;
    int    query_time;
    int    verified;
    int    id;
    int    active;
} query_t;

typedef struct {
    int    n_products;
    int    n_batches;
    int    n_transports;
    int    n_storage;
    int    n_queries;
    double total_quantity;
    int    total_violations;
    int    total_queries;
    double avg_quality;
} fsc_state_t;

static product_t products[MAX_PRODUCTS];
static batch_t batches[MAX_BATCHES];
static transport_t transports[MAX_TRANSPORT];
static storage_t storages[MAX_STORAGE];
static query_t queries[MAX_QUERIES];
static fsc_state_t fsc;

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

int fsc_init(void) {
    if (initialized) return -1;
    fsc.n_products = 0; fsc.n_batches = 0;
    fsc.n_transports = 0; fsc.n_storage = 0; fsc.n_queries = 0;
    fsc.total_quantity = 0.0; fsc.total_violations = 0;
    fsc.total_queries = 0; fsc.avg_quality = 0.0;
    for (int i = 0; i < MAX_PRODUCTS; i++) products[i].active = 0;
    for (int i = 0; i < MAX_BATCHES; i++) batches[i].active = 0;
    for (int i = 0; i < MAX_TRANSPORT; i++) transports[i].active = 0;
    for (int i = 0; i < MAX_STORAGE; i++) storages[i].active = 0;
    for (int i = 0; i < MAX_QUERIES; i++) queries[i].active = 0;
    initialized = 1;
    print_str("[FSC] Food supply chain initialized\n");
    return 0;
}

int fsc_add_product(int food_type, double shelf_life, double optimal_temp) {
    if (fsc.n_products >= MAX_PRODUCTS) return -1;
    product_t* p = &products[fsc.n_products];
    p->food_type = food_type;
    p->shelf_life_days = shelf_life;
    p->optimal_temp = optimal_temp;
    p->optimal_humidity = 85.0 + (double)(food_type % 3) * 3.0;
    p->price_per_kg = 2.0 + (double)(food_type % 4) * 1.5;
    p->farm_id = fsc.n_products % 4;
    p->id = fsc.n_products; p->active = 1;
    fsc.n_products++;
    print_str("[FSC] Product "); print_int(fsc.n_products - 1);
    print_str(" type="); print_int(food_type);
    print_str(" shelf="); print_int((int)shelf_life); print_str("d");
    print_str(" temp="); print_int((int)optimal_temp); print_str("C");
    print_str(" price="); print_int((int)(p->price_per_kg * 100)); print_str("c/kg\n");
    return fsc.n_products - 1;
}

int fsc_add_batch(int product_id, double quantity, int harvest_date) {
    if (product_id >= fsc.n_products) return -1;
    if (fsc.n_batches >= MAX_BATCHES) return -1;
    batch_t* b = &batches[fsc.n_batches];
    b->product_id = product_id;
    b->batch_number = 2026000 + fsc.n_batches;
    b->quantity_kg = quantity;
    b->harvest_date = harvest_date;
    b->expiry_date = harvest_date + (int)products[product_id].shelf_life_days;
    b->quality_grade = (quantity > 500) ? QUALITY_A : (quantity > 200 ? QUALITY_B : QUALITY_C);
    b->safety_score = 90.0 + (double)(fsc.n_batches % 4) * 2.0;
    b->certified = (b->safety_score > 92) ? 1 : 0;
    b->id = fsc.n_batches; b->active = 1;
    fsc.n_batches++;
    fsc.total_quantity += quantity;
    print_str("[FSC] Batch "); print_int(b->batch_number);
    print_str(" product="); print_int(product_id);
    print_str(" qty="); print_int((int)quantity); print_str("kg");
    print_str(" grade="); print_int(b->quality_grade);
    print_str(" safety="); print_int((int)b->safety_score);
    print_str(" cert="); print_int(b->certified); print_str("\n");
    return fsc.n_batches - 1;
}

int fsc_add_transport(int batch_id, double temp_current) {
    if (batch_id >= fsc.n_batches) return -1;
    if (fsc.n_transports >= MAX_TRANSPORT) return -1;
    transport_t* t = &transports[fsc.n_transports];
    t->batch_id = batch_id;
    t->temp_current = temp_current;
    t->temp_optimal = products[batches[batch_id].product_id].optimal_temp;
    t->humidity = products[batches[batch_id].product_id].optimal_humidity;
    t->duration_hours = 24.0 + (double)(fsc.n_transports % 3) * 12.0;
    t->temp_violations = 0;
    if (temp_current > t->temp_optimal + 2.0) t->temp_violations = 1;
    if (temp_current < t->temp_optimal - 3.0) t->temp_violations = 1;
    t->in_transit = 1;
    t->id = fsc.n_transports; t->active = 1;
    fsc.n_transports++;
    fsc.total_violations += t->temp_violations;
    print_str("[FSC] Transport "); print_int(fsc.n_transports - 1);
    print_str(" batch="); print_int(batches[batch_id].batch_number);
    print_str(" T="); print_int((int)temp_current); print_str("C");
    print_str(" optimal="); print_int((int)t->temp_optimal); print_str("C");
    print_str(" dur="); print_int((int)t->duration_hours); print_str("h");
    print_str(" violations="); print_int(t->temp_violations); print_str("\n");
    return fsc.n_transports - 1;
}

int fsc_add_storage(int batch_id, double quantity) {
    if (batch_id >= fsc.n_batches) return -1;
    if (fsc.n_storage >= MAX_STORAGE) return -1;
    storage_t* s = &storages[fsc.n_storage];
    s->batch_id = batch_id;
    s->quantity_kg = quantity;
    s->temp_current = products[batches[batch_id].product_id].optimal_temp;
    s->days_remaining = batches[batch_id].expiry_date - batches[batch_id].harvest_date;
    s->fifo_position = fsc.n_storage;
    s->alert = (s->days_remaining < 3) ? 1 : 0;
    s->id = fsc.n_storage; s->active = 1;
    fsc.n_storage++;
    print_str("[FSC] Storage "); print_int(fsc.n_storage - 1);
    print_str(" batch="); print_int(batches[batch_id].batch_number);
    print_str(" qty="); print_int((int)quantity); print_str("kg");
    print_str(" remaining="); print_int(s->days_remaining); print_str("d");
    print_str(" alert="); print_int(s->alert); print_str("\n");
    return fsc.n_storage - 1;
}

void fsc_quality_control(void) {
    double total_quality = 0.0;
    int graded = 0;
    for (int i = 0; i < fsc.n_batches; i++) {
        if (!batches[i].active) continue;
        total_quality += batches[i].safety_score;
        graded++;
        print_str("[FSC] Batch "); print_int(batches[i].batch_number);
        print_str(" grade="); print_int(batches[i].quality_grade);
        print_str(" safety="); print_int((int)batches[i].safety_score);
        print_str(" cert="); print_int(batches[i].certified); print_str("\n");
    }
    fsc.avg_quality = total_quality / (graded + 1e-10);
    print_str("[FSC] Avg quality: "); print_int((int)fsc.avg_quality); print_str("/100\n");
}

void fsc_cold_chain_monitoring(void) {
    print_str("[FSC] Cold chain monitoring:\n");
    int violations = 0;
    for (int i = 0; i < fsc.n_transports; i++) {
        if (transports[i].temp_violations > 0) {
            print_str("  ALERT: Transport "); print_int(i);
            print_str(" batch="); print_int(batches[transports[i].batch_id].batch_number);
            print_str(" T="); print_int((int)transports[i].temp_current); print_str("C");
            print_str(" (optimal "); print_int((int)transports[i].temp_optimal); print_str("C)\n");
            violations += transports[i].temp_violations;
        }
    }
    if (violations == 0) print_str("  All transports within optimal range\n");
    else { print_str("  Total violations: "); print_int(violations); print_str("\n"); }
}

void fsc_inventory_management(void) {
    print_str("[FSC] Inventory management (FIFO):\n");
    for (int i = 0; i < fsc.n_storage; i++) {
        storage_t* s = &storages[i];
        print_str("  Pos "); print_int(s->fifo_position);
        print_str(" batch="); print_int(batches[s->batch_id].batch_number);
        print_str(" qty="); print_int((int)s->quantity_kg); print_str("kg");
        print_str(" remaining="); print_int(s->days_remaining); print_str("d");
        if (s->alert) print_str(" EXPIRY WARNING");
        print_str("\n");
    }
}

void fsc_consumer_query(int batch_id) {
    if (batch_id >= fsc.n_batches) return;
    if (fsc.n_queries >= MAX_QUERIES) return;
    batch_t* b = &batches[batch_id];
    product_t* p = &products[b->product_id];
    query_t* q = &queries[fsc.n_queries];
    q->batch_id = batch_id;
    q->consumer_id = fsc.n_queries;
    q->query_time = fsc.n_queries * 3600;
    q->verified = b->certified;
    q->id = fsc.n_queries; q->active = 1;
    fsc.n_queries++;
    fsc.total_queries++;
    print_str("[FSC] Consumer query batch "); print_int(b->batch_number);
    print_str(":\n  Food type: "); print_int(p->food_type);
    print_str("\n  Farm: "); print_int(p->farm_id);
    print_str("\n  Harvest: day "); print_int(b->harvest_date);
    print_str("\n  Expiry: day "); print_int(b->expiry_date);
    print_str("\n  Grade: "); print_int(b->quality_grade);
    print_str("\n  Safety: "); print_int((int)b->safety_score);
    print_str("\n  Certified: "); print_int(b->certified);
    print_str("\n  Verified: "); print_int(q->verified); print_str("\n");
}

void fsc_print_state(void) {
    print_str("[FSC] Products="); print_int(fsc.n_products);
    print_str(" Batches="); print_int(fsc.n_batches);
    print_str(" Transports="); print_int(fsc.n_transports);
    print_str(" Storage="); print_int(fsc.n_storage);
    print_str(" Qty="); print_int((int)fsc.total_quantity); print_str("kg");
    print_str(" Violations="); print_int(fsc.total_violations);
    print_str(" Queries="); print_int(fsc.total_queries); print_str("\n");
}

int main(void) {
    print_str("=== Food Supply Chain Traceability Demo ===\n\n");
    fsc_init();

    print_str("Adding products...\n");
    fsc_add_product(FOOD_GRAIN, 365.0, 15.0);
    fsc_add_product(FOOD_VEGETABLE, 7.0, 4.0);
    fsc_add_product(FOOD_FRUIT, 14.0, 5.0);
    fsc_add_product(FOOD_MEAT, 3.0, -2.0);
    fsc_add_product(FOOD_DAIRY, 21.0, 4.0);
    fsc_add_product(FOOD_SEAFOOD, 2.0, 0.0);

    print_str("\nAdding batches...\n");
    fsc_add_batch(0, 1000.0, 100);
    fsc_add_batch(1, 500.0, 160);
    fsc_add_batch(2, 300.0, 155);
    fsc_add_batch(3, 200.0, 162);
    fsc_add_batch(4, 400.0, 158);
    fsc_add_batch(5, 150.0, 163);
    fsc_add_batch(0, 800.0, 110);
    fsc_add_batch(1, 600.0, 161);

    print_str("\nTransport monitoring...\n");
    fsc_add_transport(0, 14.0);
    fsc_add_transport(1, 5.0);
    fsc_add_transport(2, 6.0);
    fsc_add_transport(3, -1.0);
    fsc_add_transport(4, 3.0);
    fsc_add_transport(5, 2.0);

    print_str("\nQuality control...\n");
    fsc_quality_control();

    print_str("\nCold chain monitoring...\n");
    fsc_cold_chain_monitoring();

    print_str("\nStorage management...\n");
    fsc_add_storage(0, 800.0);
    fsc_add_storage(1, 400.0);
    fsc_add_storage(2, 250.0);
    fsc_add_storage(3, 180.0);
    fsc_add_storage(4, 350.0);
    fsc_add_storage(5, 130.0);
    fsc_inventory_management();

    print_str("\nConsumer queries...\n");
    fsc_consumer_query(0);
    fsc_consumer_query(3);
    fsc_consumer_query(5);

    fsc_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
