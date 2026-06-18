/* logistics_admin: Logistics administration system (v1.0)
 * Transportation management, freight management, logistics parks, cold chain, green logistics
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

#define MAX_TRANSPORT    16
#define MAX_FREIGHT      14
#define MAX_PARK         12
#define MAX_COLD_CHAIN   10
#define MAX_GREEN        10

typedef struct {
    int    transport_id;
    int    transport_type;
    int    transport_mode;
    int    road_transport;
    int    rail_transport;
    int    air_transport;
    int    year;
    int    active;
} transport_t;

typedef struct {
    int    freight_id;
    int    freight_type;
    int    freight_category;
    int    loading_ops;
    int    transit_ops;
    int    delivery_ops;
    int    year;
    int    active;
} freight_t;

typedef struct {
    int    park_id;
    int    park_type;
    int    park_category;
    int    park_planning;
    int    park_operations;
    int    park_services;
    int    year;
    int    active;
} park_t;

typedef struct {
    int    cold_chain_id;
    int    cold_chain_type;
    int    cold_category;
    int    cold_transport;
    int    cold_storage;
    int    temp_monitoring;
    int    year;
    int    active;
} cold_chain_t;

typedef struct {
    int    green_id;
    int    green_type;
    int    green_category;
    int    energy_saving;
    int    packaging_recycle;
    int    green_transport;
    int    year;
    int    active;
} green_t;

typedef struct {
    int    n_transport;
    int    n_freight;
    int    n_park;
    int    n_cold_chain;
    int    n_green;
    int    total_road;
    int    total_loading;
    int    total_planning;
    int    total_cold;
    int    total_saving;
} la_state_t;

static transport_t transports[MAX_TRANSPORT];
static freight_t freights[MAX_FREIGHT];
static park_t parks[MAX_PARK];
static cold_chain_t cold_chains[MAX_COLD_CHAIN];
static green_t greens[MAX_GREEN];
static la_state_t la;

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

int la_init(void) {
    if (initialized) return -1;
    la.n_transport = 0; la.n_freight = 0; la.n_park = 0;
    la.n_cold_chain = 0; la.n_green = 0;
    la.total_road = 0; la.total_loading = 0;
    la.total_planning = 0; la.total_cold = 0;
    la.total_saving = 0;
    for (int i = 0; i < MAX_TRANSPORT; i++) transports[i].active = 0;
    for (int i = 0; i < MAX_FREIGHT; i++) freights[i].active = 0;
    for (int i = 0; i < MAX_PARK; i++) parks[i].active = 0;
    for (int i = 0; i < MAX_COLD_CHAIN; i++) cold_chains[i].active = 0;
    for (int i = 0; i < MAX_GREEN; i++) greens[i].active = 0;
    initialized = 1;
    print_str("[LA] Logistics initialized\n");
    return 0;
}

int la_transport(int trp_type, int mode, int road, int rail, int air, int year) {
    if (la.n_transport >= MAX_TRANSPORT) return -1;
    transport_t* t = &transports[la.n_transport];
    t->transport_id = la.n_transport;
    t->transport_type = trp_type;
    t->transport_mode = mode;
    t->road_transport = road;
    t->rail_transport = rail;
    t->air_transport = air;
    t->year = year;
    t->active = 1;
    la.total_road += road;
    la.n_transport++;
    print_str("[LA] Transport "); print_int(la.n_transport - 1);
    print_str(" type="); print_int(trp_type);
    print_str(" mode="); print_int(mode);
    print_str(" rd="); print_int(road);
    print_str(" rl="); print_int(rail);
    print_str(" ar="); print_int(air); print_str("\n");
    return la.n_transport - 1;
}

int la_freight(int frt_type, int cat, int loading, int transit, int delivery, int year) {
    if (la.n_freight >= MAX_FREIGHT) return -1;
    freight_t* f = &freights[la.n_freight];
    f->freight_id = la.n_freight;
    f->freight_type = frt_type;
    f->freight_category = cat;
    f->loading_ops = loading;
    f->transit_ops = transit;
    f->delivery_ops = delivery;
    f->year = year;
    f->active = 1;
    la.total_loading += loading;
    la.n_freight++;
    print_str("[LA] Freight "); print_int(la.n_freight - 1);
    print_str(" type="); print_int(frt_type);
    print_str(" cat="); print_int(cat);
    print_str(" ldd="); print_int(loading);
    print_str(" trs="); print_int(transit);
    print_str(" dlv="); print_int(delivery); print_str("\n");
    return la.n_freight - 1;
}

int la_park(int prk_type, int cat, int planning, int operations, int services, int year) {
    if (la.n_park >= MAX_PARK) return -1;
    park_t* p = &parks[la.n_park];
    p->park_id = la.n_park;
    p->park_type = prk_type;
    p->park_category = cat;
    p->park_planning = planning;
    p->park_operations = operations;
    p->park_services = services;
    p->year = year;
    p->active = 1;
    la.total_planning += planning;
    la.n_park++;
    print_str("[LA] Park "); print_int(la.n_park - 1);
    print_str(" type="); print_int(prk_type);
    print_str(" cat="); print_int(cat);
    print_str(" pln="); print_int(planning);
    print_str(" ops="); print_int(operations);
    print_str(" svc="); print_int(services); print_str("\n");
    return la.n_park - 1;
}

int la_cold_chain(int cc_type, int cat, int transport, int storage, int monitoring, int year) {
    if (la.n_cold_chain >= MAX_COLD_CHAIN) return -1;
    cold_chain_t* c = &cold_chains[la.n_cold_chain];
    c->cold_chain_id = la.n_cold_chain;
    c->cold_chain_type = cc_type;
    c->cold_category = cat;
    c->cold_transport = transport;
    c->cold_storage = storage;
    c->temp_monitoring = monitoring;
    c->year = year;
    c->active = 1;
    la.total_cold += transport;
    la.n_cold_chain++;
    print_str("[LA] Cold Chain "); print_int(la.n_cold_chain - 1);
    print_str(" type="); print_int(cc_type);
    print_str(" cat="); print_int(cat);
    print_str(" trp="); print_int(transport);
    print_str(" stg="); print_int(storage);
    print_str(" mon="); print_int(monitoring); print_str("\n");
    return la.n_cold_chain - 1;
}

int la_green(int grn_type, int cat, int saving, int recycling, int green_trp, int year) {
    if (la.n_green >= MAX_GREEN) return -1;
    green_t* g = &greens[la.n_green];
    g->green_id = la.n_green;
    g->green_type = grn_type;
    g->green_category = cat;
    g->energy_saving = saving;
    g->packaging_recycle = recycling;
    g->green_transport = green_trp;
    g->year = year;
    g->active = 1;
    la.total_saving += saving;
    la.n_green++;
    print_str("[LA] Green "); print_int(la.n_green - 1);
    print_str(" type="); print_int(grn_type);
    print_str(" cat="); print_int(cat);
    print_str(" sv="); print_int(saving);
    print_str(" rcy="); print_int(recycling);
    print_str(" gtr="); print_int(green_trp); print_str("\n");
    return la.n_green - 1;
}

void la_transport_report(void) {
    print_str("[LA] Transport report:\n");
    print_str("  Transport categories: "); print_int(la.n_transport); print_str("\n");
    print_str("  Total road transport: "); print_int(la.total_road); print_str("\n");
}

void la_freight_report(void) {
    print_str("[LA] Freight report:\n");
    print_str("  Freight categories: "); print_int(la.n_freight); print_str("\n");
    print_str("  Total loading operations: "); print_int(la.total_loading); print_str("\n");
}

void la_green_report(void) {
    print_str("[LA] Green report:\n");
    print_str("  Park categories: "); print_int(la.n_park); print_str("\n");
    print_str("  Total park planning: "); print_int(la.total_planning); print_str("\n");
    print_str("  Cold chain categories: "); print_int(la.n_cold_chain); print_str("\n");
    print_str("  Total cold transport: "); print_int(la.total_cold); print_str("\n");
    print_str("  Green logistics categories: "); print_int(la.n_green); print_str("\n");
    print_str("  Total energy saving: "); print_int(la.total_saving); print_str("\n");
}

void la_print_state(void) {
    print_str("[LA] Tr="); print_int(la.n_transport);
    print_str(" Fr="); print_int(la.n_freight);
    print_str(" Pk="); print_int(la.n_park);
    print_str(" Cc="); print_int(la.n_cold_chain);
    print_str(" Gn="); print_int(la.n_green);
    print_str("\n");
}

int main(void) {
    print_str("=== Logistics Admin Demo ===\n\n");
    la_init();

    print_str("Transportation management...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int mode = (i % 3) + 1;
        int rd = 100 + (i * 25);
        int rl = 60 + (i * 15);
        int ar = 30 + (i * 8);
        int year = 2020 + (i % 5);
        la_transport(type, mode, rd, rl, ar, year);
    }

    print_str("\nFreight management...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ldd = 80 + (i * 20);
        int trs = 50 + (i * 12);
        int dlv = 70 + (i * 18);
        int year = 2021 + (i % 4);
        la_freight(type, cat, ldd, trs, dlv, year);
    }

    print_str("\nLogistics parks...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pln = 15 + (i * 4);
        int ops = 40 + (i * 10);
        int svc = 25 + (i * 6);
        int year = 2022 + (i % 3);
        la_park(type, cat, pln, ops, svc, year);
    }

    print_str("\nCold chain logistics...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int trp = 30 + (i * 8);
        int stg = 20 + (i * 5);
        int mon = 40 + (i * 10);
        int year = 2023 + (i % 2);
        la_cold_chain(type, cat, trp, stg, mon, year);
    }

    print_str("\nGreen logistics...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int sv = 25 + (i * 6);
        int rcy = 15 + (i * 4);
        int gtr = 20 + (i * 5);
        int year = 2024;
        la_green(type, cat, sv, rcy, gtr, year);
    }

    print_str("\nTransport report...\n");
    la_transport_report();

    print_str("\nFreight report...\n");
    la_freight_report();

    print_str("\nGreen report...\n");
    la_green_report();

    print_str("\nFinal state...\n");
    la_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
