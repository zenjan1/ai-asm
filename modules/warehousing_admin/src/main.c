/* warehousing_admin: Warehousing administration system (v1.0)
 * Warehouse management, inbound, outbound, inventory control, special warehousing
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

#define MAX_WAREHOUSE    16
#define MAX_INBOUND      14
#define MAX_OUTBOUND     12
#define MAX_CONTROL      10
#define MAX_SPECIAL      10

typedef struct {
    int    warehouse_id;
    int    warehouse_type;
    int    warehouse_category;
    int    warehouse_planning;
    int    warehouse_construction;
    int    warehouse_operations;
    int    year;
    int    active;
} warehouse_t;

typedef struct {
    int    inbound_id;
    int    inbound_type;
    int    inbound_category;
    int    inbound_acceptance;
    int    inbound_shelving;
    int    inbound_registration;
    int    year;
    int    active;
} inbound_t;

typedef struct {
    int    outbound_id;
    int    outbound_type;
    int    outbound_category;
    int    outbound_audit;
    int    outbound_picking;
    int    outbound_shipping;
    int    year;
    int    active;
} outbound_t;

typedef struct {
    int    control_id;
    int    control_type;
    int    control_category;
    int    stocktaking;
    int    transfer_ops;
    int    optimization_ops;
    int    year;
    int    active;
} control_t;

typedef struct {
    int    special_id;
    int    special_type;
    int    special_category;
    int    hazardous_storage;
    int    constant_temp;
    int    automated_storage;
    int    year;
    int    active;
} special_t;

typedef struct {
    int    n_warehouse;
    int    n_inbound;
    int    n_outbound;
    int    n_control;
    int    n_special;
    int    total_planning;
    int    total_acceptance;
    int    total_audit;
    int    total_stocktaking;
    int    total_hazardous;
} wa_state_t;

static warehouse_t warehouses[MAX_WAREHOUSE];
static inbound_t inbounds[MAX_INBOUND];
static outbound_t outbounds[MAX_OUTBOUND];
static control_t controls[MAX_CONTROL];
static special_t specials[MAX_SPECIAL];
static wa_state_t wa;

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

int wa_init(void) {
    if (initialized) return -1;
    wa.n_warehouse = 0; wa.n_inbound = 0; wa.n_outbound = 0;
    wa.n_control = 0; wa.n_special = 0;
    wa.total_planning = 0; wa.total_acceptance = 0;
    wa.total_audit = 0; wa.total_stocktaking = 0;
    wa.total_hazardous = 0;
    for (int i = 0; i < MAX_WAREHOUSE; i++) warehouses[i].active = 0;
    for (int i = 0; i < MAX_INBOUND; i++) inbounds[i].active = 0;
    for (int i = 0; i < MAX_OUTBOUND; i++) outbounds[i].active = 0;
    for (int i = 0; i < MAX_CONTROL; i++) controls[i].active = 0;
    for (int i = 0; i < MAX_SPECIAL; i++) specials[i].active = 0;
    initialized = 1;
    print_str("[WA] Warehousing initialized\n");
    return 0;
}

int wa_warehouse(int wh_type, int cat, int planning, int construction, int operations, int year) {
    if (wa.n_warehouse >= MAX_WAREHOUSE) return -1;
    warehouse_t* w = &warehouses[wa.n_warehouse];
    w->warehouse_id = wa.n_warehouse;
    w->warehouse_type = wh_type;
    w->warehouse_category = cat;
    w->warehouse_planning = planning;
    w->warehouse_construction = construction;
    w->warehouse_operations = operations;
    w->year = year;
    w->active = 1;
    wa.total_planning += planning;
    wa.n_warehouse++;
    print_str("[WA] Warehouse "); print_int(wa.n_warehouse - 1);
    print_str(" type="); print_int(wh_type);
    print_str(" cat="); print_int(cat);
    print_str(" pln="); print_int(planning);
    print_str(" cst="); print_int(construction);
    print_str(" ops="); print_int(operations); print_str("\n");
    return wa.n_warehouse - 1;
}

int wa_inbound(int ib_type, int cat, int acceptance, int shelving, int registration, int year) {
    if (wa.n_inbound >= MAX_INBOUND) return -1;
    inbound_t* ib = &inbounds[wa.n_inbound];
    ib->inbound_id = wa.n_inbound;
    ib->inbound_type = ib_type;
    ib->inbound_category = cat;
    ib->inbound_acceptance = acceptance;
    ib->inbound_shelving = shelving;
    ib->inbound_registration = registration;
    ib->year = year;
    ib->active = 1;
    wa.total_acceptance += acceptance;
    wa.n_inbound++;
    print_str("[WA] Inbound "); print_int(wa.n_inbound - 1);
    print_str(" type="); print_int(ib_type);
    print_str(" cat="); print_int(cat);
    print_str(" acc="); print_int(acceptance);
    print_str(" shv="); print_int(shelving);
    print_str(" rgs="); print_int(registration); print_str("\n");
    return wa.n_inbound - 1;
}

int wa_outbound(int ob_type, int cat, int audit, int picking, int shipping, int year) {
    if (wa.n_outbound >= MAX_OUTBOUND) return -1;
    outbound_t* ob = &outbounds[wa.n_outbound];
    ob->outbound_id = wa.n_outbound;
    ob->outbound_type = ob_type;
    ob->outbound_category = cat;
    ob->outbound_audit = audit;
    ob->outbound_picking = picking;
    ob->outbound_shipping = shipping;
    ob->year = year;
    ob->active = 1;
    wa.total_audit += audit;
    wa.n_outbound++;
    print_str("[WA] Outbound "); print_int(wa.n_outbound - 1);
    print_str(" type="); print_int(ob_type);
    print_str(" cat="); print_int(cat);
    print_str(" aud="); print_int(audit);
    print_str(" pck="); print_int(picking);
    print_str(" shp="); print_int(shipping); print_str("\n");
    return wa.n_outbound - 1;
}

int wa_control(int ctl_type, int cat, int stocktaking, int transfer, int optimization, int year) {
    if (wa.n_control >= MAX_CONTROL) return -1;
    control_t* c = &controls[wa.n_control];
    c->control_id = wa.n_control;
    c->control_type = ctl_type;
    c->control_category = cat;
    c->stocktaking = stocktaking;
    c->transfer_ops = transfer;
    c->optimization_ops = optimization;
    c->year = year;
    c->active = 1;
    wa.total_stocktaking += stocktaking;
    wa.n_control++;
    print_str("[WA] Control "); print_int(wa.n_control - 1);
    print_str(" type="); print_int(ctl_type);
    print_str(" cat="); print_int(cat);
    print_str(" stc="); print_int(stocktaking);
    print_str(" trf="); print_int(transfer);
    print_str(" opt="); print_int(optimization); print_str("\n");
    return wa.n_control - 1;
}

int wa_special(int sp_type, int cat, int hazardous, int constant, int automated, int year) {
    if (wa.n_special >= MAX_SPECIAL) return -1;
    special_t* s = &specials[wa.n_special];
    s->special_id = wa.n_special;
    s->special_type = sp_type;
    s->special_category = cat;
    s->hazardous_storage = hazardous;
    s->constant_temp = constant;
    s->automated_storage = automated;
    s->year = year;
    s->active = 1;
    wa.total_hazardous += hazardous;
    wa.n_special++;
    print_str("[WA] Special "); print_int(wa.n_special - 1);
    print_str(" type="); print_int(sp_type);
    print_str(" cat="); print_int(cat);
    print_str(" hzd="); print_int(hazardous);
    print_str(" cns="); print_int(constant);
    print_str(" atm="); print_int(automated); print_str("\n");
    return wa.n_special - 1;
}

void wa_warehouse_report(void) {
    print_str("[WA] Warehouse report:\n");
    print_str("  Warehouse categories: "); print_int(wa.n_warehouse); print_str("\n");
    print_str("  Total warehouse planning: "); print_int(wa.total_planning); print_str("\n");
}

void wa_inbound_report(void) {
    print_str("[WA] Inbound report:\n");
    print_str("  Inbound categories: "); print_int(wa.n_inbound); print_str("\n");
    print_str("  Total acceptance ops: "); print_int(wa.total_acceptance); print_str("\n");
}

void wa_special_report(void) {
    print_str("[WA] Special report:\n");
    print_str("  Outbound categories: "); print_int(wa.n_outbound); print_str("\n");
    print_str("  Total audit ops: "); print_int(wa.total_audit); print_str("\n");
    print_str("  Control categories: "); print_int(wa.n_control); print_str("\n");
    print_str("  Total stocktaking: "); print_int(wa.total_stocktaking); print_str("\n");
    print_str("  Special warehousing categories: "); print_int(wa.n_special); print_str("\n");
    print_str("  Total hazardous storage: "); print_int(wa.total_hazardous); print_str("\n");
}

void wa_print_state(void) {
    print_str("[WA] Wh="); print_int(wa.n_warehouse);
    print_str(" Ib="); print_int(wa.n_inbound);
    print_str(" Ob="); print_int(wa.n_outbound);
    print_str(" Ct="); print_int(wa.n_control);
    print_str(" Sp="); print_int(wa.n_special);
    print_str("\n");
}

int main(void) {
    print_str("=== Warehousing Admin Demo ===\n\n");
    wa_init();

    print_str("Warehouse management...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int pln = 10 + (i * 3);
        int cst = 8 + (i * 2);
        int ops = 20 + (i * 5);
        int year = 2020 + (i % 5);
        wa_warehouse(type, cat, pln, cst, ops, year);
    }

    print_str("\nInbound management...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int acc = 60 + (i * 15);
        int shv = 50 + (i * 12);
        int rgs = 55 + (i * 14);
        int year = 2021 + (i % 4);
        wa_inbound(type, cat, acc, shv, rgs, year);
    }

    print_str("\nOutbound management...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int aud = 40 + (i * 10);
        int pck = 70 + (i * 18);
        int shp = 60 + (i * 15);
        int year = 2022 + (i % 3);
        wa_outbound(type, cat, aud, pck, shp, year);
    }

    print_str("\nInventory control...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int stc = 30 + (i * 8);
        int trf = 15 + (i * 4);
        int opt = 20 + (i * 5);
        int year = 2023 + (i % 2);
        wa_control(type, cat, stc, trf, opt, year);
    }

    print_str("\nSpecial warehousing...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int hzd = 10 + (i * 3);
        int cns = 15 + (i * 4);
        int atm = 8 + (i * 2);
        int year = 2024;
        wa_special(type, cat, hzd, cns, atm, year);
    }

    print_str("\nWarehouse report...\n");
    wa_warehouse_report();

    print_str("\nInbound report...\n");
    wa_inbound_report();

    print_str("\nSpecial report...\n");
    wa_special_report();

    print_str("\nFinal state...\n");
    wa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
