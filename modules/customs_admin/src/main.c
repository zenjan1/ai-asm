/* customs_admin: Customs administration system (v1.0)
 * Customs clearance, supervision, anti-smuggling, statistics, international cooperation
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

#define MAX_CLEARANCE    16
#define MAX_SUPERVISION  14
#define MAX_SMUGGLING    12
#define MAX_STATISTICS   10
#define MAX_COOPERATION  10

typedef struct {
    int    clearance_id;
    int    port_id;
    int    declaration_type;
    int    declarations;
    int    approved;
    int    tax_collected;
    int    year;
    int    active;
} clearance_t;

typedef struct {
    int    supervision_id;
    int    checkpoint_id;
    int    cargo_type;
    int    cargo_inspections;
    int    luggage_checks;
    int    transport_checks;
    int    year;
    int    active;
} supervision_t;

typedef struct {
    int    smuggling_id;
    int    case_type;
    int    intelligence_source;
    int    cases_investigated;
    int    seizures;
    int    value_seized;
    int    year;
    int    active;
} smuggling_t;

typedef struct {
    int    statistics_id;
    int    trade_type;
    int    data_category;
    int    import_value;
    int    export_value;
    int    trade_balance;
    int    year;
    int    active;
} statistics_t;

typedef struct {
    int    cooperation_id;
    int    partner_country;
    int    cooperation_type;
    int    mutual_assistance;
    int    standards_recognized;
    int    cross_border_cases;
    int    year;
    int    active;
} cooperation_t;

typedef struct {
    int    n_clearance;
    int    n_supervision;
    int    n_smuggling;
    int    n_statistics;
    int    n_cooperation;
    int    total_declarations;
    int    total_tax;
    int    total_inspections;
    int    total_seizures;
    int    total_trade;
} ca_state_t;

static clearance_t clearances[MAX_CLEARANCE];
static supervision_t supervisions[MAX_SUPERVISION];
static smuggling_t smugglings[MAX_SMUGGLING];
static statistics_t statistics[MAX_STATISTICS];
static cooperation_t cooperations[MAX_COOPERATION];
static ca_state_t ca;

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

int ca_init(void) {
    if (initialized) return -1;
    ca.n_clearance = 0; ca.n_supervision = 0; ca.n_smuggling = 0;
    ca.n_statistics = 0; ca.n_cooperation = 0;
    ca.total_declarations = 0; ca.total_tax = 0;
    ca.total_inspections = 0; ca.total_seizures = 0;
    ca.total_trade = 0;
    for (int i = 0; i < MAX_CLEARANCE; i++) clearances[i].active = 0;
    for (int i = 0; i < MAX_SUPERVISION; i++) supervisions[i].active = 0;
    for (int i = 0; i < MAX_SMUGGLING; i++) smugglings[i].active = 0;
    for (int i = 0; i < MAX_STATISTICS; i++) statistics[i].active = 0;
    for (int i = 0; i < MAX_COOPERATION; i++) cooperations[i].active = 0;
    initialized = 1;
    print_str("[CA] Customs admin initialized\n");
    return 0;
}

int ca_clearance(int port, int decl_type, int declarations, int approved, int tax, int year) {
    if (ca.n_clearance >= MAX_CLEARANCE) return -1;
    clearance_t* c = &clearances[ca.n_clearance];
    c->clearance_id = ca.n_clearance;
    c->port_id = port;
    c->declaration_type = decl_type;
    c->declarations = declarations;
    c->approved = approved;
    c->tax_collected = tax;
    c->year = year;
    c->active = 1;
    ca.total_declarations += declarations;
    ca.total_tax += tax;
    ca.n_clearance++;
    print_str("[CA] Clearance "); print_int(ca.n_clearance - 1);
    print_str(" port="); print_int(port);
    print_str(" type="); print_int(decl_type);
    print_str(" dec="); print_int(declarations);
    print_str(" app="); print_int(approved);
    print_str(" tax=$"); print_int(tax); print_str("\n");
    return ca.n_clearance - 1;
}

int ca_supervision(int checkpoint, int cargo_type, int cargo, int luggage, int transport, int year) {
    if (ca.n_supervision >= MAX_SUPERVISION) return -1;
    supervision_t* s = &supervisions[ca.n_supervision];
    s->supervision_id = ca.n_supervision;
    s->checkpoint_id = checkpoint;
    s->cargo_type = cargo_type;
    s->cargo_inspections = cargo;
    s->luggage_checks = luggage;
    s->transport_checks = transport;
    s->year = year;
    s->active = 1;
    ca.total_inspections += cargo + luggage + transport;
    ca.n_supervision++;
    print_str("[CA] Supervision "); print_int(ca.n_supervision - 1);
    print_str(" chk="); print_int(checkpoint);
    print_str(" type="); print_int(cargo_type);
    print_str(" crg="); print_int(cargo);
    print_str(" lug="); print_int(luggage);
    print_str(" trn="); print_int(transport); print_str("\n");
    return ca.n_supervision - 1;
}

int ca_smuggling(int case_type, int intel_src, int investigated, int seizures, int value, int year) {
    if (ca.n_smuggling >= MAX_SMUGGLING) return -1;
    smuggling_t* sm = &smugglings[ca.n_smuggling];
    sm->smuggling_id = ca.n_smuggling;
    sm->case_type = case_type;
    sm->intelligence_source = intel_src;
    sm->cases_investigated = investigated;
    sm->seizures = seizures;
    sm->value_seized = value;
    sm->year = year;
    sm->active = 1;
    ca.total_seizures += seizures;
    ca.n_smuggling++;
    print_str("[CA] Smuggling "); print_int(ca.n_smuggling - 1);
    print_str(" type="); print_int(case_type);
    print_str(" int="); print_int(intel_src);
    print_str(" inv="); print_int(investigated);
    print_str(" sez="); print_int(seizures);
    print_str(" val=$"); print_int(value); print_str("\n");
    return ca.n_smuggling - 1;
}

int ca_statistics(int trade_type, int data_cat, int imports, int exports, int balance, int year) {
    if (ca.n_statistics >= MAX_STATISTICS) return -1;
    statistics_t* st = &statistics[ca.n_statistics];
    st->statistics_id = ca.n_statistics;
    st->trade_type = trade_type;
    st->data_category = data_cat;
    st->import_value = imports;
    st->export_value = exports;
    st->trade_balance = balance;
    st->year = year;
    st->active = 1;
    ca.total_trade += imports + exports;
    ca.n_statistics++;
    print_str("[CA] Statistics "); print_int(ca.n_statistics - 1);
    print_str(" type="); print_int(trade_type);
    print_str(" cat="); print_int(data_cat);
    print_str(" imp=$"); print_int(imports);
    print_str(" exp=$"); print_int(exports);
    print_str(" bal=$"); print_int(balance); print_str("\n");
    return ca.n_statistics - 1;
}

int ca_cooperation(int country, int coop_type, int assistance, int standards, int cross_border, int year) {
    if (ca.n_cooperation >= MAX_COOPERATION) return -1;
    cooperation_t* co = &cooperations[ca.n_cooperation];
    co->cooperation_id = ca.n_cooperation;
    co->partner_country = country;
    co->cooperation_type = coop_type;
    co->mutual_assistance = assistance;
    co->standards_recognized = standards;
    co->cross_border_cases = cross_border;
    co->year = year;
    co->active = 1;
    ca.n_cooperation++;
    print_str("[CA] Cooperation "); print_int(ca.n_cooperation - 1);
    print_str(" cty="); print_int(country);
    print_str(" type="); print_int(coop_type);
    print_str(" ast="); print_int(assistance);
    print_str(" std="); print_int(standards);
    print_str(" crs="); print_int(cross_border); print_str("\n");
    return ca.n_cooperation - 1;
}

void ca_clearance_report(void) {
    print_str("[CA] Clearance report:\n");
    print_str("  Ports: "); print_int(ca.n_clearance); print_str("\n");
    print_str("  Total declarations: "); print_int(ca.total_declarations); print_str("\n");
    print_str("  Total tax collected: $"); print_int(ca.total_tax); print_str("\n");
}

void ca_supervision_report(void) {
    print_str("[CA] Supervision report:\n");
    print_str("  Checkpoints: "); print_int(ca.n_supervision); print_str("\n");
    print_str("  Total inspections: "); print_int(ca.total_inspections); print_str("\n");
}

void ca_smuggling_report(void) {
    print_str("[CA] Anti-smuggling report:\n");
    print_str("  Smuggling cases: "); print_int(ca.n_smuggling); print_str("\n");
    print_str("  Total seizures: "); print_int(ca.total_seizures); print_str("\n");
    print_str("  Trade statistics: "); print_int(ca.n_statistics); print_str("\n");
    print_str("  Total trade volume: $"); print_int(ca.total_trade); print_str("\n");
    print_str("  International cooperation: "); print_int(ca.n_cooperation); print_str("\n");
}

void ca_print_state(void) {
    print_str("[CA] Cl="); print_int(ca.n_clearance);
    print_str(" Sp="); print_int(ca.n_supervision);
    print_str(" Sm="); print_int(ca.n_smuggling);
    print_str(" St="); print_int(ca.n_statistics);
    print_str(" Co="); print_int(ca.n_cooperation);
    print_str("\n");
}

int main(void) {
    print_str("=== Customs Admin Demo ===\n\n");
    ca_init();

    print_str("Customs clearance...\n");
    for (int i = 0; i < 16; i++) {
        int port = 1000 + (i * 7);
        int type = (i % 5) + 1;
        int dec = 5000 + (i * 1000);
        int app = dec - (i * 50);
        int tax = 1000000 + (i * 500000);
        int year = 2020 + (i % 5);
        ca_clearance(port, type, dec, app, tax, year);
    }

    print_str("\nSupervision and inspection...\n");
    for (int i = 0; i < 14; i++) {
        int chk = 2000 + (i * 11);
        int type = (i % 4) + 1;
        int crg = 1000 + (i * 200);
        int lug = 500 + (i * 100);
        int trn = 200 + (i * 50);
        int year = 2021 + (i % 4);
        ca_supervision(chk, type, crg, lug, trn, year);
    }

    print_str("\nAnti-smuggling...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 6) + 1;
        int intel = (i % 4) + 1;
        int inv = 10 + (i * 3);
        int sez = inv - (i % 3);
        int val = 500000 + (i * 200000);
        int year = 2022 + (i % 3);
        ca_smuggling(type, intel, inv, sez, val, year);
    }

    print_str("\nTrade statistics...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int imp = 10000000 + (i * 5000000);
        int exp = 12000000 + (i * 6000000);
        int bal = exp - imp;
        int year = 2023 + (i % 2);
        ca_statistics(type, cat, imp, exp, bal, year);
    }

    print_str("\nInternational cooperation...\n");
    for (int i = 0; i < 10; i++) {
        int cty = 3000 + (i * 13);
        int type = (i % 4) + 1;
        int ast = 20 + (i * 5);
        int std = 10 + (i * 3);
        int crs = 5 + (i * 2);
        int year = 2024;
        ca_cooperation(cty, type, ast, std, crs, year);
    }

    print_str("\nClearance report...\n");
    ca_clearance_report();

    print_str("\nSupervision report...\n");
    ca_supervision_report();

    print_str("\nSmuggling report...\n");
    ca_smuggling_report();

    print_str("\nFinal state...\n");
    ca_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
