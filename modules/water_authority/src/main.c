/* water_authority: Water management (v1.0)
 * Water sources, supply, drainage, resources, flood control
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

#define MAX_SOURCES    12
#define MAX_PLANTS     10
#define MAX_PIPES      16
#define MAX_PERMITS    14
#define MAX_ALERTS     12
#define MAX_TREATMENT  8

typedef struct {
    int    source_id;
    int    type;
    int    zone;
    int    capacity;
    int    current_level;
    int    quality;
    int    protected;
    int    active;
} source_t;

typedef struct {
    int    plant_id;
    int    type;
    int    zone;
    int    capacity;
    int    output;
    int    quality;
    int    staff;
    int    active;
} plant_t;

typedef struct {
    int    pipe_id;
    int    from_id;
    int    to_id;
    int    type;
    int    diameter;
    int    length_km;
    int    flow;
    int    status;
    int    active;
} pipe_t;

typedef struct {
    int    permit_id;
    int    user_id;
    int    source_id;
    int    amount;
    int    purpose;
    int    issued_year;
    int    expiry_year;
    int    used;
    int    active;
} permit_t;

typedef struct {
    int    alert_id;
    int    source_id;
    int    type;
    int    severity;
    int    level;
    int    threshold;
    int    year;
    int    responded;
    int    active;
} alert_t;

typedef struct {
    int    treat_id;
    int    plant_id;
    int    inflow;
    int    outflow;
    int    quality_in;
    int    quality_out;
    int    cost;
    int    year;
    int    active;
} treatment_t;

typedef struct {
    int    n_sources;
    int    n_plants;
    int    n_pipes;
    int    n_permits;
    int    n_alerts;
    int    n_treatments;
    int    total_supply;
    int    total_consumption;
    int    total_treatment;
    int    total_alerts;
    int    total_responded;
} wa_state_t;

static source_t sources[MAX_SOURCES];
static plant_t plants[MAX_PLANTS];
static pipe_t pipes[MAX_PIPES];
static permit_t permits[MAX_PERMITS];
static alert_t alerts[MAX_ALERTS];
static treatment_t treatments[MAX_TREATMENT];
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
    wa.n_sources = 0; wa.n_plants = 0; wa.n_pipes = 0;
    wa.n_permits = 0; wa.n_alerts = 0; wa.n_treatments = 0;
    wa.total_supply = 0; wa.total_consumption = 0;
    wa.total_treatment = 0; wa.total_alerts = 0; wa.total_responded = 0;
    for (int i = 0; i < MAX_SOURCES; i++) sources[i].active = 0;
    for (int i = 0; i < MAX_PLANTS; i++) plants[i].active = 0;
    for (int i = 0; i < MAX_PIPES; i++) pipes[i].active = 0;
    for (int i = 0; i < MAX_PERMITS; i++) permits[i].active = 0;
    for (int i = 0; i < MAX_ALERTS; i++) alerts[i].active = 0;
    for (int i = 0; i < MAX_TREATMENT; i++) treatments[i].active = 0;
    initialized = 1;
    print_str("[WA] Water authority initialized\n");
    return 0;
}

int wa_add_source(int type, int zone, int capacity, int quality) {
    if (wa.n_sources >= MAX_SOURCES) return -1;
    source_t* s = &sources[wa.n_sources];
    s->source_id = wa.n_sources;
    s->type = type;
    s->zone = zone;
    s->capacity = capacity;
    s->current_level = capacity * 3 / 4;
    s->quality = quality;
    s->protected = 0;
    s->active = 1;
    wa.n_sources++;
    print_str("[WA] Source "); print_int(wa.n_sources - 1);
    print_str(" type="); print_int(type);
    print_str(" zone="); print_int(zone);
    print_str(" cap="); print_int(capacity);
    print_str(" qual="); print_int(quality); print_str("\n");
    return wa.n_sources - 1;
}

int wa_protect_source(int source_id) {
    if (source_id >= wa.n_sources) return -1;
    sources[source_id].protected = 1;
    print_str("[WA] Protect S"); print_int(source_id); print_str("\n");
    return 0;
}

int wa_add_plant(int type, int zone, int capacity, int staff) {
    if (wa.n_plants >= MAX_PLANTS) return -1;
    plant_t* p = &plants[wa.n_plants];
    p->plant_id = wa.n_plants;
    p->type = type;
    p->zone = zone;
    p->capacity = capacity;
    p->output = 0;
    p->quality = 0;
    p->staff = staff;
    p->active = 1;
    wa.n_plants++;
    print_str("[WA] Plant "); print_int(wa.n_plants - 1);
    print_str(" type="); print_int(type);
    print_str(" zone="); print_int(zone);
    print_str(" cap="); print_int(capacity);
    print_str(" staff="); print_int(staff); print_str("\n");
    return wa.n_plants - 1;
}

int wa_add_pipe(int from, int to, int type, int diameter, int length) {
    if (wa.n_pipes >= MAX_PIPES || from >= wa.n_plants || to >= wa.n_plants) return -1;
    pipe_t* p = &pipes[wa.n_pipes];
    p->pipe_id = wa.n_pipes;
    p->from_id = from;
    p->to_id = to;
    p->type = type;
    p->diameter = diameter;
    p->length_km = length;
    p->flow = 0;
    p->status = 1;
    p->active = 1;
    wa.n_pipes++;
    print_str("[WA] Pipe "); print_int(wa.n_pipes - 1);
    print_str(" P"); print_int(from); print_str("->P"); print_int(to);
    print_str(" type="); print_int(type);
    print_str(" dia="); print_int(diameter);
    print_str(" km="); print_int(length); print_str("\n");
    return wa.n_pipes - 1;
}

int wa_supply_water(int plant_id, int amount) {
    if (plant_id >= wa.n_plants) return -1;
    plants[plant_id].output += amount;
    wa.total_supply += amount;
    print_str("[WA] Supply P"); print_int(plant_id);
    print_str(" amt="); print_int(amount); print_str("\n");
    return 0;
}

int wa_issue_permit(int user, int source, int amount, int purpose, int issued, int expiry) {
    if (wa.n_permits >= MAX_PERMITS || source >= wa.n_sources) return -1;
    permit_t* p = &permits[wa.n_permits];
    p->permit_id = wa.n_permits;
    p->user_id = user;
    p->source_id = source;
    p->amount = amount;
    p->purpose = purpose;
    p->issued_year = issued;
    p->expiry_year = expiry;
    p->used = 0;
    p->active = 1;
    wa.total_consumption += amount;
    wa.n_permits++;
    print_str("[WA] Permit "); print_int(wa.n_permits - 1);
    print_str(" U"); print_int(user);
    print_str(" S"); print_int(source);
    print_str(" amt="); print_int(amount);
    print_str(" purpose="); print_int(purpose);
    print_str(" "); print_int(issued); print_str("-"); print_int(expiry); print_str("\n");
    return wa.n_permits - 1;
}

int wa_use_permit(int permit_id, int used) {
    if (permit_id >= wa.n_permits) return -1;
    permits[permit_id].used += used;
    print_str("[WA] Use Pt"); print_int(permit_id);
    print_str(" used="); print_int(used); print_str("\n");
    return 0;
}

int wa_record_alert(int source, int type, int severity, int level, int threshold, int year) {
    if (wa.n_alerts >= MAX_ALERTS || source >= wa.n_sources) return -1;
    alert_t* a = &alerts[wa.n_alerts];
    a->alert_id = wa.n_alerts;
    a->source_id = source;
    a->type = type;
    a->severity = severity;
    a->level = level;
    a->threshold = threshold;
    a->year = year;
    a->responded = 0;
    a->active = 1;
    wa.total_alerts++;
    wa.n_alerts++;
    print_str("[WA] Alert "); print_int(wa.n_alerts - 1);
    print_str(" S"); print_int(source);
    print_str(" type="); print_int(type);
    print_str(" sev="); print_int(severity);
    print_str(" level="); print_int(level);
    print_str(" thresh="); print_int(threshold); print_str("\n");
    return wa.n_alerts - 1;
}

int wa_respond_alert(int alert_id) {
    if (alert_id >= wa.n_alerts) return -1;
    alerts[alert_id].responded = 1;
    wa.total_responded++;
    print_str("[WA] Respond Al"); print_int(alert_id); print_str("\n");
    return 0;
}

int wa_record_treatment(int plant, int inflow, int quality_in, int quality_out, int cost, int year) {
    if (wa.n_treatments >= MAX_TREATMENT || plant >= wa.n_plants) return -1;
    treatment_t* t = &treatments[wa.n_treatments];
    t->treat_id = wa.n_treatments;
    t->plant_id = plant;
    t->inflow = inflow;
    t->outflow = inflow * 9 / 10;
    t->quality_in = quality_in;
    t->quality_out = quality_out;
    t->cost = cost;
    t->year = year;
    t->active = 1;
    wa.total_treatment += cost;
    wa.n_treatments++;
    print_str("[WA] Treat P"); print_int(plant);
    print_str(" in="); print_int(inflow);
    print_str(" q="); print_int(quality_in); print_str("->"); print_int(quality_out);
    print_str(" $"); print_int(cost); print_str("\n");
    return wa.n_treatments - 1;
}

void wa_source_report(void) {
    print_str("[WA] Source report:\n");
    for (int i = 0; i < wa.n_sources; i++) {
        source_t* s = &sources[i];
        print_str("  S"); print_int(i);
        print_str(" type="); print_int(s->type);
        print_str(" zone="); print_int(s->zone);
        print_str(" lvl="); print_int(s->current_level);
        print_str("/"); print_int(s->capacity);
        print_str(" qual="); print_int(s->quality);
        print_str(" prot="); print_int(s->protected); print_str("\n");
    }
}

void wa_plant_report(void) {
    print_str("[WA] Plant report:\n");
    for (int i = 0; i < wa.n_plants; i++) {
        plant_t* p = &plants[i];
        print_str("  P"); print_int(i);
        print_str(" type="); print_int(p->type);
        print_str(" zone="); print_int(p->zone);
        print_str(" out="); print_int(p->output);
        print_str("/"); print_int(p->capacity);
        print_str(" qual="); print_int(p->quality);
        print_str(" staff="); print_int(p->staff); print_str("\n");
    }
    print_str("  Total supply: "); print_int(wa.total_supply); print_str("\n");
}

void wa_alert_report(void) {
    print_str("[WA] Alert report:\n");
    print_str("  Total: "); print_int(wa.total_alerts);
    print_str(" responded="); print_int(wa.total_responded); print_str("\n");
}

void wa_print_state(void) {
    print_str("[WA] Src="); print_int(wa.n_sources);
    print_str(" Plant="); print_int(wa.n_plants);
    print_str(" Pipe="); print_int(wa.n_pipes);
    print_str(" Perm="); print_int(wa.n_permits);
    print_str(" Alert="); print_int(wa.n_alerts);
    print_str(" Treat="); print_int(wa.n_treatments);
    print_str("\n");
    print_str("  Supply: "); print_int(wa.total_supply);
    print_str(" Consump: "); print_int(wa.total_consumption); print_str("\n");
}

int main(void) {
    print_str("=== Water Authority Demo ===\n\n");
    wa_init();

    print_str("Adding water sources...\n");
    wa_add_source(1, 1, 50000, 90);
    wa_add_source(1, 2, 40000, 85);
    wa_add_source(2, 1, 30000, 80);
    wa_add_source(2, 3, 35000, 88);
    wa_add_source(3, 2, 25000, 92);
    wa_add_source(3, 4, 20000, 87);
    wa_add_source(1, 3, 45000, 91);
    wa_add_source(2, 4, 28000, 83);
    wa_add_source(3, 1, 32000, 89);
    wa_add_source(1, 4, 38000, 86);
    wa_add_source(2, 2, 42000, 90);
    wa_add_source(3, 3, 22000, 84);

    print_str("\nProtecting sources...\n");
    for (int i = 0; i < 8; i++) {
        wa_protect_source(i);
    }

    print_str("\nAdding water plants...\n");
    wa_add_plant(1, 1, 30000, 15);
    wa_add_plant(1, 2, 25000, 12);
    wa_add_plant(2, 1, 20000, 10);
    wa_add_plant(2, 3, 22000, 11);
    wa_add_plant(1, 4, 28000, 14);
    wa_add_plant(2, 2, 18000, 9);
    wa_add_plant(1, 3, 26000, 13);
    wa_add_plant(2, 4, 19000, 10);
    wa_add_plant(1, 1, 32000, 16);
    wa_add_plant(2, 3, 21000, 11);

    print_str("\nAdding pipes...\n");
    for (int i = 0; i < 16; i++) {
        int from = i % 10;
        int to = (i + 3) % 10;
        int type = (i % 2) + 1;
        int diameter = 200 + (i * 50);
        int length = 5 + (i * 2);
        wa_add_pipe(from, to, type, diameter, length);
    }

    print_str("\nSupplying water...\n");
    for (int i = 0; i < 10; i++) {
        int amount = 5000 + (i * 2000);
        wa_supply_water(i, amount);
    }

    print_str("\nIssuing permits...\n");
    for (int i = 0; i < 14; i++) {
        int user = 1000 + i;
        int source = i % 12;
        int amount = 1000 + (i * 500);
        int purpose = (i % 4) + 1;
        int issued = 2023 + (i % 2);
        int expiry = issued + 2;
        wa_issue_permit(user, source, amount, purpose, issued, expiry);
    }

    print_str("\nUsing permits...\n");
    for (int i = 0; i < 14; i++) {
        int used = 500 + (i * 200);
        wa_use_permit(i, used);
    }

    print_str("\nRecording alerts...\n");
    for (int i = 0; i < 12; i++) {
        int source = i % 12;
        int type = (i % 3) + 1;
        int severity = (i % 4) + 1;
        int level = 60 + (i * 5);
        int threshold = 80;
        int year = 2024;
        wa_record_alert(source, type, severity, level, threshold, year);
    }

    print_str("\nResponding to alerts...\n");
    for (int i = 0; i < 12; i++) {
        wa_respond_alert(i);
    }

    print_str("\nRecording treatments...\n");
    for (int i = 0; i < 8; i++) {
        int plant = i % 10;
        int inflow = 3000 + (i * 1000);
        int quality_in = 40 + (i * 5);
        int quality_out = 90 + (i % 5);
        int cost = 5000 + (i * 2000);
        int year = 2024;
        wa_record_treatment(plant, inflow, quality_in, quality_out, cost, year);
    }

    print_str("\nSource report...\n");
    wa_source_report();

    print_str("\nPlant report...\n");
    wa_plant_report();

    print_str("\nAlert report...\n");
    wa_alert_report();

    print_str("\nFinal state...\n");
    wa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
