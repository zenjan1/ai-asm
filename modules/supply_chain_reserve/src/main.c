/* supply_chain_reserve: Strategic material reserve system (v1.0)
 * Strategic reserve, warehouse management, emergency dispatch, quality, rotation
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

#define MAX_STRATEGIC    16
#define MAX_WAREHOUSE    14
#define MAX_DISPATCH     12
#define MAX_QUALITY      10
#define MAX_ROTATION     10

typedef struct {
    int    strategic_id;
    int    material_type;
    int    category;
    int    quantity;
    int    value;
    int    storage_location;
    int    year;
    int    active;
} strategic_t;

typedef struct {
    int    warehouse_id;
    int    location_id;
    int    capacity;
    int    current_stock;
    int    temperature;
    int    humidity;
    int    staff_count;
    int    year;
    int    active;
} warehouse_t;

typedef struct {
    int    dispatch_id;
    int    emergency_id;
    int    material_type;
    int    quantity;
    int    destination_id;
    int    transport_mode;
    int    delivery_hours;
    int    year;
    int    active;
} dispatch_t;

typedef struct {
    int    quality_id;
    int    material_id;
    int    inspector_id;
    int    test_type;
    int    pass;
    int    shelf_life_days;
    int    year;
    int    active;
} quality_t;

typedef struct {
    int    rotation_id;
    int    material_id;
    int    old_qty;
    int    new_qty;
    int    disposal_value;
    int    completion_pct;
    int    year;
    int    active;
} rotation_t;

typedef struct {
    int    n_strategic;
    int    n_warehouse;
    int    n_dispatch;
    int    n_quality;
    int    n_rotation;
    int    total_quantity;
    int    total_value;
    int    total_stock;
    int    total_dispatched;
    int    total_disposal;
} sr_state_t;

static strategic_t strategics[MAX_STRATEGIC];
static warehouse_t warehouses[MAX_WAREHOUSE];
static dispatch_t dispatches[MAX_DISPATCH];
static quality_t qualities[MAX_QUALITY];
static rotation_t rotations[MAX_ROTATION];
static sr_state_t sr;

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

int sr_init(void) {
    if (initialized) return -1;
    sr.n_strategic = 0; sr.n_warehouse = 0; sr.n_dispatch = 0;
    sr.n_quality = 0; sr.n_rotation = 0;
    sr.total_quantity = 0; sr.total_value = 0;
    sr.total_stock = 0; sr.total_dispatched = 0;
    sr.total_disposal = 0;
    for (int i = 0; i < MAX_STRATEGIC; i++) strategics[i].active = 0;
    for (int i = 0; i < MAX_WAREHOUSE; i++) warehouses[i].active = 0;
    for (int i = 0; i < MAX_DISPATCH; i++) dispatches[i].active = 0;
    for (int i = 0; i < MAX_QUALITY; i++) qualities[i].active = 0;
    for (int i = 0; i < MAX_ROTATION; i++) rotations[i].active = 0;
    initialized = 1;
    print_str("[SR] Supply chain reserve initialized\n");
    return 0;
}

int sr_strategic(int material, int category, int quantity, int value, int location, int year) {
    if (sr.n_strategic >= MAX_STRATEGIC) return -1;
    strategic_t* s = &strategics[sr.n_strategic];
    s->strategic_id = sr.n_strategic;
    s->material_type = material;
    s->category = category;
    s->quantity = quantity;
    s->value = value;
    s->storage_location = location;
    s->year = year;
    s->active = 1;
    sr.total_quantity += quantity;
    sr.total_value += value;
    sr.n_strategic++;
    print_str("[SR] Strategic "); print_int(sr.n_strategic - 1);
    print_str(" mat="); print_int(material);
    print_str(" cat="); print_int(category);
    print_str(" qty="); print_int(quantity);
    print_str(" val=$"); print_int(value);
    print_str(" loc="); print_int(location); print_str("\n");
    return sr.n_strategic - 1;
}

int sr_warehouse(int location, int capacity, int stock, int temp, int humidity, int staff, int year) {
    if (sr.n_warehouse >= MAX_WAREHOUSE) return -1;
    warehouse_t* w = &warehouses[sr.n_warehouse];
    w->warehouse_id = sr.n_warehouse;
    w->location_id = location;
    w->capacity = capacity;
    w->current_stock = stock;
    w->temperature = temp;
    w->humidity = humidity;
    w->staff_count = staff;
    w->year = year;
    w->active = 1;
    sr.total_stock += stock;
    sr.n_warehouse++;
    print_str("[SR] Warehouse "); print_int(sr.n_warehouse - 1);
    print_str(" loc="); print_int(location);
    print_str(" cap="); print_int(capacity);
    print_str(" stk="); print_int(stock);
    print_str(" tmp="); print_int(temp);
    print_str(" hum="); print_int(humidity); print_str("%\n");
    return sr.n_warehouse - 1;
}

int sr_dispatch(int emergency, int material, int quantity, int destination, int transport, int delivery, int year) {
    if (sr.n_dispatch >= MAX_DISPATCH) return -1;
    dispatch_t* d = &dispatches[sr.n_dispatch];
    d->dispatch_id = sr.n_dispatch;
    d->emergency_id = emergency;
    d->material_type = material;
    d->quantity = quantity;
    d->destination_id = destination;
    d->transport_mode = transport;
    d->delivery_hours = delivery;
    d->year = year;
    d->active = 1;
    sr.total_dispatched += quantity;
    sr.n_dispatch++;
    print_str("[SR] Dispatch "); print_int(sr.n_dispatch - 1);
    print_str(" emg="); print_int(emergency);
    print_str(" mat="); print_int(material);
    print_str(" qty="); print_int(quantity);
    print_str(" dst="); print_int(destination);
    print_str(" dlvr="); print_int(delivery); print_str("h\n");
    return sr.n_dispatch - 1;
}

int sr_quality(int material, int inspector, int test, int pass, int shelf_life, int year) {
    if (sr.n_quality >= MAX_QUALITY) return -1;
    quality_t* q = &qualities[sr.n_quality];
    q->quality_id = sr.n_quality;
    q->material_id = material;
    q->inspector_id = inspector;
    q->test_type = test;
    q->pass = pass;
    q->shelf_life_days = shelf_life;
    q->year = year;
    q->active = 1;
    sr.n_quality++;
    print_str("[SR] Quality "); print_int(sr.n_quality - 1);
    print_str(" mat="); print_int(material);
    print_str(" ins="); print_int(inspector);
    print_str(" tst="); print_int(test);
    print_str(" pass="); print_int(pass);
    print_str(" slf="); print_int(shelf_life); print_str("d\n");
    return sr.n_quality - 1;
}

int sr_rotation(int material, int old_qty, int new_qty, int disposal, int completion, int year) {
    if (sr.n_rotation >= MAX_ROTATION) return -1;
    rotation_t* r = &rotations[sr.n_rotation];
    r->rotation_id = sr.n_rotation;
    r->material_id = material;
    r->old_qty = old_qty;
    r->new_qty = new_qty;
    r->disposal_value = disposal;
    r->completion_pct = completion;
    r->year = year;
    r->active = 1;
    sr.total_disposal += disposal;
    sr.n_rotation++;
    print_str("[SR] Rotation "); print_int(sr.n_rotation - 1);
    print_str(" mat="); print_int(material);
    print_str(" old="); print_int(old_qty);
    print_str(" new="); print_int(new_qty);
    print_str(" dsp=$"); print_int(disposal);
    print_str(" cmp="); print_int(completion); print_str("%\n");
    return sr.n_rotation - 1;
}

void sr_strategic_report(void) {
    print_str("[SR] Strategic report:\n");
    print_str("  Reserve items: "); print_int(sr.n_strategic); print_str("\n");
    print_str("  Total quantity: "); print_int(sr.total_quantity); print_str("\n");
    print_str("  Total value: $"); print_int(sr.total_value); print_str("\n");
}

void sr_warehouse_report(void) {
    print_str("[SR] Warehouse report:\n");
    print_str("  Warehouses: "); print_int(sr.n_warehouse); print_str("\n");
    print_str("  Total stock: "); print_int(sr.total_stock); print_str("\n");
}

void sr_dispatch_report(void) {
    print_str("[SR] Dispatch report:\n");
    print_str("  Dispatches: "); print_int(sr.n_dispatch); print_str("\n");
    print_str("  Total dispatched: "); print_int(sr.total_dispatched); print_str("\n");
    print_str("  Quality checks: "); print_int(sr.n_quality); print_str("\n");
    print_str("  Rotation plans: "); print_int(sr.n_rotation); print_str("\n");
    print_str("  Total disposal: $"); print_int(sr.total_disposal); print_str("\n");
}

void sr_print_state(void) {
    print_str("[SR] St="); print_int(sr.n_strategic);
    print_str(" Wh="); print_int(sr.n_warehouse);
    print_str(" Ds="); print_int(sr.n_dispatch);
    print_str(" Qu="); print_int(sr.n_quality);
    print_str(" Rt="); print_int(sr.n_rotation);
    print_str("\n");
}

int main(void) {
    print_str("=== Supply Chain Reserve Demo ===\n\n");
    sr_init();

    print_str("Strategic reserves...\n");
    for (int i = 0; i < 16; i++) {
        int mat = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int qty = 1000 + (i * 500);
        int val = 50000 + (i * 30000);
        int loc = (i % 8) + 1;
        int year = 2020 + (i % 5);
        sr_strategic(mat, cat, qty, val, loc, year);
    }

    print_str("\nWarehouse management...\n");
    for (int i = 0; i < 14; i++) {
        int loc = (i % 8) + 1;
        int cap = 5000 + (i * 2000);
        int stk = 3000 + (i * 1500);
        int tmp = 15 + (i % 10);
        int hum = 50 + (i * 3);
        int stf = 5 + (i * 2);
        int year = 2021 + (i % 4);
        sr_warehouse(loc, cap, stk, tmp, hum, stf, year);
    }

    print_str("\nEmergency dispatch...\n");
    for (int i = 0; i < 12; i++) {
        int emg = 100 + (i * 7);
        int mat = (i % 5) + 1;
        int qty = 200 + (i * 100);
        int dst = (i % 8) + 1;
        int trn = (i % 3) + 1;
        int dlv = 4 + (i * 3);
        int year = 2022 + (i % 3);
        sr_dispatch(emg, mat, qty, dst, trn, dlv, year);
    }

    print_str("\nQuality inspection...\n");
    for (int i = 0; i < 10; i++) {
        int mat = 200 + (i * 13);
        int ins = 300 + (i % 6);
        int tst = (i % 4) + 1;
        int pass = (i % 3 == 0) ? 0 : 1;
        int slf = 180 + (i * 30);
        int year = 2023 + (i % 2);
        sr_quality(mat, ins, tst, pass, slf, year);
    }

    print_str("\nRotation management...\n");
    for (int i = 0; i < 10; i++) {
        int mat = 400 + (i * 7);
        int old = 500 + (i * 200);
        int newq = old;
        int dsp = 5000 + (i * 3000);
        int cmp = 60 + (i * 4);
        int year = 2024;
        sr_rotation(mat, old, newq, dsp, cmp, year);
    }

    print_str("\nStrategic report...\n");
    sr_strategic_report();

    print_str("\nWarehouse report...\n");
    sr_warehouse_report();

    print_str("\nDispatch report...\n");
    sr_dispatch_report();

    print_str("\nFinal state...\n");
    sr_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
