/* museum_system: Museum management system (v1.0)
 * Collections, exhibitions, ticketing, education, operations
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

#define MAX_ARTIFACTS  16
#define MAX_EXHIBITIONS 8
#define MAX_TICKETS    18
#define MAX_MEMBERS    10
#define MAX_EVENTS     10
#define MAX_VISITORS   12

typedef struct {
    int    artifact_id;
    int    category;
    int    era;
    int    condition;
    int    exhibition_id;
    int    n_views;
    double value;
    int    active;
} artifact_t;

typedef struct {
    int    exhibition_id;
    int    theme;
    int    start_day;
    int    end_day;
    int    n_artifacts;
    int    n_visitors;
    double revenue;
    int    active;
} exhibition_t;

typedef struct {
    int    ticket_id;
    int    exhibition_id;
    int    visitor_id;
    double price;
    int    day;
    int    guided;
    int    active;
} ticket_t;

typedef struct {
    int    member_id;
    int    tier;
    int    n_visits;
    double total_paid;
    int    active;
} member_t;

typedef struct {
    int    event_id;
    int    type;
    int    day;
    int    capacity;
    int    enrolled;
    double fee;
    int    active;
} event_t;

typedef struct {
    int    visitor_id;
    int    group_size;
    int    n_tickets;
    double total_spent;
    int    active;
} visitor_t;

typedef struct {
    int    n_artifacts;
    int    n_exhibitions;
    int    n_tickets;
    int    n_members;
    int    n_events;
    int    n_visitors;
    double ticket_revenue;
    double event_revenue;
    double total_revenue;
    int    total_visitors;
} mus_state_t;

static artifact_t artifacts[MAX_ARTIFACTS];
static exhibition_t exhibitions[MAX_EXHIBITIONS];
static ticket_t tickets[MAX_TICKETS];
static member_t members[MAX_MEMBERS];
static event_t events[MAX_EVENTS];
static visitor_t visitors[MAX_VISITORS];
static mus_state_t mus;

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

int mus_init(void) {
    if (initialized) return -1;
    mus.n_artifacts = 0; mus.n_exhibitions = 0; mus.n_tickets = 0;
    mus.n_members = 0; mus.n_events = 0; mus.n_visitors = 0;
    mus.ticket_revenue = 0.0; mus.event_revenue = 0.0;
    mus.total_revenue = 0.0; mus.total_visitors = 0;
    for (int i = 0; i < MAX_ARTIFACTS; i++) artifacts[i].active = 0;
    for (int i = 0; i < MAX_EXHIBITIONS; i++) exhibitions[i].active = 0;
    for (int i = 0; i < MAX_TICKETS; i++) tickets[i].active = 0;
    for (int i = 0; i < MAX_MEMBERS; i++) members[i].active = 0;
    for (int i = 0; i < MAX_EVENTS; i++) events[i].active = 0;
    for (int i = 0; i < MAX_VISITORS; i++) visitors[i].active = 0;
    initialized = 1;
    print_str("[MUS] Museum system initialized\n");
    return 0;
}

int mus_add_exhibition(int theme, int start_day, int end_day) {
    if (mus.n_exhibitions >= MAX_EXHIBITIONS) return -1;
    exhibition_t* e = &exhibitions[mus.n_exhibitions];
    e->exhibition_id = mus.n_exhibitions;
    e->theme = theme;
    e->start_day = start_day;
    e->end_day = end_day;
    e->n_artifacts = 0;
    e->n_visitors = 0;
    e->revenue = 0.0;
    e->active = 1;
    mus.n_exhibitions++;
    print_str("[MUS] Exhibition "); print_int(mus.n_exhibitions - 1);
    print_str(" theme="); print_int(theme);
    print_str(" day "); print_int(start_day); print_str("-"); print_int(end_day); print_str("\n");
    return mus.n_exhibitions - 1;
}

int mus_add_artifact(int category, int era, int condition, int exhibition_id, double value) {
    if (mus.n_artifacts >= MAX_ARTIFACTS) return -1;
    if (exhibition_id >= mus.n_exhibitions) return -2;
    artifact_t* a = &artifacts[mus.n_artifacts];
    a->artifact_id = mus.n_artifacts;
    a->category = category;
    a->era = era;
    a->condition = condition;
    a->exhibition_id = exhibition_id;
    a->n_views = 0;
    a->value = value;
    a->active = 1;
    exhibitions[exhibition_id].n_artifacts++;
    mus.n_artifacts++;
    print_str("[MUS] Artifact "); print_int(mus.n_artifacts - 1);
    print_str(" cat="); print_int(category);
    print_str(" era="); print_int(era);
    print_str(" exh="); print_int(exhibition_id);
    print_str(" val="); print_int((int)value); print_str("\n");
    return mus.n_artifacts - 1;
}

int mus_register_member(int tier) {
    if (mus.n_members >= MAX_MEMBERS) return -1;
    member_t* m = &members[mus.n_members];
    m->member_id = mus.n_members;
    m->tier = tier;
    m->n_visits = 0;
    m->total_paid = 0.0;
    m->active = 1;
    mus.n_members++;
    print_str("[MUS] Member "); print_int(mus.n_members - 1);
    print_str(" tier="); print_int(tier); print_str("\n");
    return mus.n_members - 1;
}

int mus_register_visitor(int group_size) {
    if (mus.n_visitors >= MAX_VISITORS) return -1;
    visitor_t* v = &visitors[mus.n_visitors];
    v->visitor_id = mus.n_visitors;
    v->group_size = group_size;
    v->n_tickets = 0;
    v->total_spent = 0.0;
    v->active = 1;
    mus.n_visitors++;
    print_str("[MUS] Visitor "); print_int(mus.n_visitors - 1);
    print_str(" group="); print_int(group_size); print_str("\n");
    return mus.n_visitors - 1;
}

int mus_add_event(int type, int day, int capacity, double fee) {
    if (mus.n_events >= MAX_EVENTS) return -1;
    event_t* e = &events[mus.n_events];
    e->event_id = mus.n_events;
    e->type = type;
    e->day = day;
    e->capacity = capacity;
    e->enrolled = 0;
    e->fee = fee;
    e->active = 1;
    mus.n_events++;
    print_str("[MUS] Event "); print_int(mus.n_events - 1);
    print_str(" type="); print_int(type);
    print_str(" day="); print_int(day);
    print_str(" cap="); print_int(capacity);
    print_str(" fee="); print_int((int)fee); print_str("\n");
    return mus.n_events - 1;
}

int mus_sell_ticket(int exhibition_id, int visitor_id, double price, int day, int guided) {
    if (mus.n_tickets >= MAX_TICKETS) return -1;
    if (exhibition_id >= mus.n_exhibitions || visitor_id >= mus.n_visitors) return -2;
    ticket_t* t = &tickets[mus.n_tickets];
    t->ticket_id = mus.n_tickets;
    t->exhibition_id = exhibition_id;
    t->visitor_id = visitor_id;
    t->price = price;
    t->day = day;
    t->guided = guided;
    t->active = 1;
    exhibitions[exhibition_id].n_visitors++;
    exhibitions[exhibition_id].revenue += price;
    visitors[visitor_id].n_tickets++;
    visitors[visitor_id].total_spent += price;
    mus.ticket_revenue += price;
    mus.total_revenue += price;
    mus.total_visitors++;
    mus.n_tickets++;
    print_str("[MUS] Ticket "); print_int(mus.n_tickets - 1);
    print_str(" E"); print_int(exhibition_id);
    print_str(" V"); print_int(visitor_id);
    print_str(" price="); print_int((int)price);
    if (guided) print_str(" GUIDED");
    print_str("\n");
    return mus.n_tickets - 1;
}

int mus_enroll_event(int event_id, int member_id) {
    if (event_id >= mus.n_events || member_id >= mus.n_members) return -1;
    event_t* e = &events[event_id];
    if (e->enrolled >= e->capacity) return -2;
    e->enrolled++;
    members[member_id].n_visits++;
    members[member_id].total_paid += e->fee;
    mus.event_revenue += e->fee;
    mus.total_revenue += e->fee;
    print_str("[MUS] Enroll E"); print_int(event_id);
    print_str(" M"); print_int(member_id);
    print_str(" fee="); print_int((int)e->fee); print_str("\n");
    return 0;
}

void mus_view_artifact(int artifact_id) {
    if (artifact_id >= mus.n_artifacts) return;
    artifacts[artifact_id].n_views++;
    print_str("[MUS] Artifact "); print_int(artifact_id);
    print_str(" viewed (total="); print_int(artifacts[artifact_id].n_views); print_str(")\n");
}

void mus_exhibition_report(void) {
    print_str("[MUS] Exhibition report:\n");
    for (int i = 0; i < mus.n_exhibitions; i++) {
        exhibition_t* e = &exhibitions[i];
        print_str("  E"); print_int(i);
        print_str(" theme="); print_int(e->theme);
        print_str(" artifacts="); print_int(e->n_artifacts);
        print_str(" visitors="); print_int(e->n_visitors);
        print_str(" rev="); print_int((int)e->revenue); print_str("\n");
    }
}

void mus_popular_artifacts(void) {
    print_str("[MUS] Popular artifacts:\n");
    for (int i = 0; i < mus.n_artifacts; i++) {
        if (artifacts[i].n_views > 0) {
            print_str("  A"); print_int(i);
            print_str(" cat="); print_int(artifacts[i].category);
            print_str(" views="); print_int(artifacts[i].n_views);
            print_str(" val="); print_int((int)artifacts[i].value); print_str("\n");
        }
    }
}

void mus_financial_report(void) {
    print_str("[MUS] Financial report:\n");
    print_str("  Ticket revenue: "); print_int((int)mus.ticket_revenue); print_str("\n");
    print_str("  Event revenue: "); print_int((int)mus.event_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)mus.total_revenue); print_str("\n");
    print_str("  Total visitors: "); print_int(mus.total_visitors); print_str("\n");
    double avg_spend = (mus.total_visitors > 0) ? mus.total_revenue / mus.total_visitors : 0.0;
    print_str("  Avg spend/visitor: "); print_int((int)avg_spend); print_str("\n");
}

void mus_print_state(void) {
    print_str("[MUS] Artifacts="); print_int(mus.n_artifacts);
    print_str(" Exhibitions="); print_int(mus.n_exhibitions);
    print_str(" Tickets="); print_int(mus.n_tickets);
    print_str(" Members="); print_int(mus.n_members);
    print_str(" Events="); print_int(mus.n_events);
    print_str(" Visitors="); print_int(mus.n_visitors);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)mus.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Museum System Demo ===\n\n");
    mus_init();

    print_str("Creating exhibitions...\n");
    mus_add_exhibition(1, 1, 30);
    mus_add_exhibition(2, 10, 40);
    mus_add_exhibition(3, 15, 45);
    mus_add_exhibition(4, 20, 50);
    mus_add_exhibition(1, 25, 55);

    print_str("\nAdding artifacts...\n");
    mus_add_artifact(1, 1, 95, 0, 50000.0);
    mus_add_artifact(1, 2, 88, 0, 35000.0);
    mus_add_artifact(2, 1, 92, 0, 80000.0);
    mus_add_artifact(2, 3, 90, 1, 45000.0);
    mus_add_artifact(3, 2, 85, 1, 60000.0);
    mus_add_artifact(3, 1, 93, 1, 70000.0);
    mus_add_artifact(4, 3, 87, 2, 55000.0);
    mus_add_artifact(4, 2, 91, 2, 40000.0);
    mus_add_artifact(1, 1, 89, 2, 75000.0);
    mus_add_artifact(2, 2, 94, 3, 90000.0);
    mus_add_artifact(3, 3, 86, 3, 30000.0);
    mus_add_artifact(4, 1, 92, 4, 65000.0);
    mus_add_artifact(1, 2, 88, 4, 42000.0);
    mus_add_artifact(2, 3, 90, 4, 58000.0);
    mus_add_artifact(3, 1, 95, 0, 85000.0);
    mus_add_artifact(4, 2, 87, 1, 48000.0);

    print_str("\nRegistering members...\n");
    mus_register_member(0);
    mus_register_member(1);
    mus_register_member(2);
    mus_register_member(3);
    mus_register_member(1);
    mus_register_member(2);

    print_str("\nRegistering visitors...\n");
    mus_register_visitor(1);
    mus_register_visitor(2);
    mus_register_visitor(4);
    mus_register_visitor(1);
    mus_register_visitor(3);
    mus_register_visitor(2);
    mus_register_visitor(1);
    mus_register_visitor(5);
    mus_register_visitor(1);
    mus_register_visitor(2);

    print_str("\nAdding events...\n");
    mus_add_event(1, 5, 30, 20.0);
    mus_add_event(2, 12, 20, 35.0);
    mus_add_event(3, 18, 25, 15.0);
    mus_add_event(1, 25, 30, 25.0);
    mus_add_event(2, 32, 20, 40.0);

    print_str("\nSelling tickets...\n");
    mus_sell_ticket(0, 0, 15.0, 2, 0);
    mus_sell_ticket(0, 1, 15.0, 2, 0);
    mus_sell_ticket(0, 2, 12.0, 3, 1);
    mus_sell_ticket(1, 3, 15.0, 11, 0);
    mus_sell_ticket(1, 4, 15.0, 11, 0);
    mus_sell_ticket(1, 5, 12.0, 12, 1);
    mus_sell_ticket(2, 6, 15.0, 16, 0);
    mus_sell_ticket(2, 7, 15.0, 16, 0);
    mus_sell_ticket(3, 8, 15.0, 21, 0);
    mus_sell_ticket(3, 9, 15.0, 21, 0);
    mus_sell_ticket(4, 0, 12.0, 26, 1);
    mus_sell_ticket(4, 1, 12.0, 26, 1);
    mus_sell_ticket(0, 2, 15.0, 27, 0);
    mus_sell_ticket(1, 3, 15.0, 28, 0);
    mus_sell_ticket(2, 4, 15.0, 29, 0);
    mus_sell_ticket(3, 5, 15.0, 30, 0);
    mus_sell_ticket(4, 6, 15.0, 31, 0);
    mus_sell_ticket(0, 7, 15.0, 32, 0);

    print_str("\nEnrolling in events...\n");
    mus_enroll_event(0, 0);
    mus_enroll_event(0, 1);
    mus_enroll_event(1, 2);
    mus_enroll_event(1, 3);
    mus_enroll_event(2, 4);
    mus_enroll_event(2, 5);
    mus_enroll_event(3, 0);
    mus_enroll_event(3, 1);
    mus_enroll_event(4, 2);
    mus_enroll_event(4, 3);

    print_str("\nViewing artifacts...\n");
    mus_view_artifact(0);
    mus_view_artifact(0);
    mus_view_artifact(2);
    mus_view_artifact(3);
    mus_view_artifact(4);
    mus_view_artifact(5);
    mus_view_artifact(6);
    mus_view_artifact(9);
    mus_view_artifact(9);
    mus_view_artifact(10);
    mus_view_artifact(14);
    mus_view_artifact(14);
    mus_view_artifact(14);

    print_str("\nExhibition report...\n");
    mus_exhibition_report();

    print_str("\nPopular artifacts...\n");
    mus_popular_artifacts();

    print_str("\nFinancial report...\n");
    mus_financial_report();

    print_str("\nFinal state...\n");
    mus_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
