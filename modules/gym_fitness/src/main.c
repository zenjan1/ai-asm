/* gym_fitness: Gym and fitness center management (v1.0)
 * Members, classes, trainers, equipment, check-ins, revenue
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

#define MAX_MEMBERS    16
#define MAX_CLASSES    10
#define MAX_TRAINERS   8
#define MAX_EQUIPMENT  12
#define MAX_CHECKINS   18

typedef struct {
    int    member_id;
    int    tier;
    int    start_month;
    int    duration_months;
    double monthly_fee;
    int    n_checkins;
    int    n_classes;
    double total_paid;
    int    active;
} member_t;

typedef struct {
    int    class_id;
    int    type;
    int    trainer_id;
    int    capacity;
    int    n_enrolled;
    double fee;
    int    day;
    int    hour;
    int    active;
} fitness_class_t;

typedef struct {
    int    trainer_id;
    int    specialty;
    int    n_classes;
    int    n_private;
    double hourly_rate;
    double total_earned;
    int    rating;
    int    active;
} trainer_t;

typedef struct {
    int    equip_id;
    int    zone;
    int    type;
    int    status;
    int    n_uses;
    int    maint_day;
    double maint_cost;
    int    active;
} equipment_t;

typedef struct {
    int    checkin_id;
    int    member_id;
    int    day;
    int    hour;
    int    duration_min;
    int    active;
} checkin_t;

typedef struct {
    int    n_members;
    int    n_classes;
    int    n_trainers;
    int    n_equipment;
    int    n_checkins;
    double membership_revenue;
    double class_revenue;
    double private_revenue;
    double total_revenue;
    double total_costs;
    int    total_checkins;
} gf_state_t;

static member_t members[MAX_MEMBERS];
static fitness_class_t classes[MAX_CLASSES];
static trainer_t trainers[MAX_TRAINERS];
static equipment_t equipment[MAX_EQUIPMENT];
static checkin_t checkins[MAX_CHECKINS];
static gf_state_t gf;

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

int gf_init(void) {
    if (initialized) return -1;
    gf.n_members = 0; gf.n_classes = 0; gf.n_trainers = 0;
    gf.n_equipment = 0; gf.n_checkins = 0;
    gf.membership_revenue = 0.0; gf.class_revenue = 0.0;
    gf.private_revenue = 0.0; gf.total_revenue = 0.0;
    gf.total_costs = 0.0; gf.total_checkins = 0;
    for (int i = 0; i < MAX_MEMBERS; i++) members[i].active = 0;
    for (int i = 0; i < MAX_CLASSES; i++) classes[i].active = 0;
    for (int i = 0; i < MAX_TRAINERS; i++) trainers[i].active = 0;
    for (int i = 0; i < MAX_EQUIPMENT; i++) equipment[i].active = 0;
    for (int i = 0; i < MAX_CHECKINS; i++) checkins[i].active = 0;
    initialized = 1;
    print_str("[GF] Gym initialized\n");
    return 0;
}

int gf_add_trainer(int specialty, double rate, int rating) {
    if (gf.n_trainers >= MAX_TRAINERS) return -1;
    trainer_t* t = &trainers[gf.n_trainers];
    t->trainer_id = gf.n_trainers;
    t->specialty = specialty;
    t->n_classes = 0;
    t->n_private = 0;
    t->hourly_rate = rate;
    t->total_earned = 0.0;
    t->rating = rating;
    t->active = 1;
    gf.n_trainers++;
    print_str("[GF] Trainer "); print_int(gf.n_trainers - 1);
    print_str(" spec="); print_int(specialty);
    print_str(" rate="); print_int((int)rate);
    print_str(" rating="); print_int(rating); print_str("\n");
    return gf.n_trainers - 1;
}

int gf_add_equipment(int zone, int type, double maint_cost) {
    if (gf.n_equipment >= MAX_EQUIPMENT) return -1;
    equipment_t* e = &equipment[gf.n_equipment];
    e->equip_id = gf.n_equipment;
    e->zone = zone;
    e->type = type;
    e->status = 1;
    e->n_uses = 0;
    e->maint_day = 0;
    e->maint_cost = maint_cost;
    e->active = 1;
    gf.n_equipment++;
    print_str("[GF] Equipment "); print_int(gf.n_equipment - 1);
    print_str(" zone="); print_int(zone);
    print_str(" type="); print_int(type); print_str("\n");
    return gf.n_equipment - 1;
}

int gf_register_member(int tier, int duration) {
    if (gf.n_members >= MAX_MEMBERS) return -1;
    member_t* m = &members[gf.n_members];
    m->member_id = gf.n_members;
    m->tier = tier;
    m->start_month = 1;
    m->duration_months = duration;
    double fees[4] = {40.0, 55.0, 75.0, 100.0};
    m->monthly_fee = fees[tier < 4 ? tier : 0];
    m->n_checkins = 0;
    m->n_classes = 0;
    m->total_paid = m->monthly_fee * duration;
    m->active = 1;
    gf.membership_revenue += m->total_paid;
    gf.total_revenue += m->total_paid;
    gf.n_members++;
    print_str("[GF] Member "); print_int(gf.n_members - 1);
    print_str(" tier="); print_int(tier);
    print_str(" dur="); print_int(duration); print_str("mo");
    print_str(" fee="); print_int((int)m->monthly_fee); print_str("/mo\n");
    return gf.n_members - 1;
}

int gf_add_class(int type, int trainer_id, int capacity, double fee, int day, int hour) {
    if (gf.n_classes >= MAX_CLASSES || trainer_id >= gf.n_trainers) return -1;
    fitness_class_t* c = &classes[gf.n_classes];
    c->class_id = gf.n_classes;
    c->type = type;
    c->trainer_id = trainer_id;
    c->capacity = capacity;
    c->n_enrolled = 0;
    c->fee = fee;
    c->day = day;
    c->hour = hour;
    c->active = 1;
    trainers[trainer_id].n_classes++;
    gf.n_classes++;
    print_str("[GF] Class "); print_int(gf.n_classes - 1);
    print_str(" type="); print_int(type);
    print_str(" T"); print_int(trainer_id);
    print_str(" cap="); print_int(capacity);
    print_str(" fee="); print_int((int)fee);
    print_str(" day="); print_int(day); print_str(" hr="); print_int(hour); print_str("\n");
    return gf.n_classes - 1;
}

int gf_enroll_member(int member_id, int class_id) {
    if (member_id >= gf.n_members || class_id >= gf.n_classes) return -1;
    fitness_class_t* c = &classes[class_id];
    if (c->n_enrolled >= c->capacity) return -2;
    c->n_enrolled++;
    members[member_id].n_classes++;
    gf.class_revenue += c->fee;
    gf.total_revenue += c->fee;
    print_str("[GF] M"); print_int(member_id);
    print_str(" enrolled C"); print_int(class_id);
    print_str(" fee="); print_int((int)c->fee); print_str("\n");
    return 0;
}

int gf_checkin(int member_id, int day, int hour, int duration) {
    if (gf.n_checkins >= MAX_CHECKINS || member_id >= gf.n_members) return -1;
    checkin_t* ci = &checkins[gf.n_checkins];
    ci->checkin_id = gf.n_checkins;
    ci->member_id = member_id;
    ci->day = day;
    ci->hour = hour;
    ci->duration_min = duration;
    ci->active = 1;
    members[member_id].n_checkins++;
    gf.total_checkins++;
    gf.n_checkins++;
    print_str("[GF] Checkin M"); print_int(member_id);
    print_str(" day="); print_int(day);
    print_str(" hr="); print_int(hour);
    print_str(" dur="); print_int(duration); print_str("min\n");
    return 0;
}

int gf_use_equipment(int equip_id, int member_id) {
    if (equip_id >= gf.n_equipment || member_id >= gf.n_members) return -1;
    equipment_t* e = &equipment[equip_id];
    if (e->status != 1) return -2;
    e->n_uses++;
    print_str("[GF] M"); print_int(member_id);
    print_str(" used E"); print_int(equip_id); print_str("\n");
    return 0;
}

int gf_private_session(int member_id, int trainer_id, int hours) {
    if (member_id >= gf.n_members || trainer_id >= gf.n_trainers) return -1;
    double cost = trainers[trainer_id].hourly_rate * hours;
    trainers[trainer_id].n_private++;
    trainers[trainer_id].total_earned += cost * 0.6;
    gf.private_revenue += cost;
    gf.total_revenue += cost;
    print_str("[GF] Private M"); print_int(member_id);
    print_str(" T"); print_int(trainer_id);
    print_str(" hrs="); print_int(hours);
    print_str(" cost="); print_int((int)cost); print_str("\n");
    return 0;
}

int gf_schedule_maintenance(int equip_id, int day) {
    if (equip_id >= gf.n_equipment) return -1;
    equipment_t* e = &equipment[equip_id];
    e->status = 2;
    e->maint_day = day;
    gf.total_costs += e->maint_cost;
    print_str("[GF] Maintenance E"); print_int(equip_id);
    print_str(" day="); print_int(day);
    print_str(" cost="); print_int((int)e->maint_cost); print_str("\n");
    return 0;
}

void gf_complete_maintenance(int equip_id) {
    if (equip_id >= gf.n_equipment) return;
    equipment[equip_id].status = 1;
    print_str("[GF] E"); print_int(equip_id); print_str(" maintenance complete\n");
}

void gf_trainer_report(void) {
    print_str("[GF] Trainer report:\n");
    for (int i = 0; i < gf.n_trainers; i++) {
        trainer_t* t = &trainers[i];
        print_str("  T"); print_int(i);
        print_str(" spec="); print_int(t->specialty);
        print_str(" classes="); print_int(t->n_classes);
        print_str(" private="); print_int(t->n_private);
        print_str(" earned="); print_int((int)t->total_earned);
        print_str(" rating="); print_int(t->rating); print_str("\n");
    }
}

void gf_financial_report(void) {
    print_str("[GF] Financial report:\n");
    print_str("  Membership: "); print_int((int)gf.membership_revenue); print_str("\n");
    print_str("  Classes: "); print_int((int)gf.class_revenue); print_str("\n");
    print_str("  Private: "); print_int((int)gf.private_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)gf.total_revenue); print_str("\n");
    print_str("  Costs: "); print_int((int)gf.total_costs); print_str("\n");
    double profit = gf.total_revenue - gf.total_costs;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    print_str("  Total checkins: "); print_int(gf.total_checkins); print_str("\n");
}

void gf_print_state(void) {
    print_str("[GF] Members="); print_int(gf.n_members);
    print_str(" Classes="); print_int(gf.n_classes);
    print_str(" Trainers="); print_int(gf.n_trainers);
    print_str(" Equip="); print_int(gf.n_equipment);
    print_str(" Checkins="); print_int(gf.n_checkins);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)gf.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Gym Fitness Demo ===\n\n");
    gf_init();

    print_str("Adding trainers...\n");
    gf_add_trainer(1, 50.0, 92);
    gf_add_trainer(2, 55.0, 90);
    gf_add_trainer(3, 45.0, 88);
    gf_add_trainer(1, 60.0, 95);
    gf_add_trainer(4, 48.0, 87);
    gf_add_trainer(2, 52.0, 91);
    gf_add_trainer(3, 42.0, 85);
    gf_add_trainer(4, 58.0, 93);

    print_str("\nAdding equipment...\n");
    gf_add_equipment(1, 1, 200.0);
    gf_add_equipment(1, 1, 180.0);
    gf_add_equipment(1, 2, 250.0);
    gf_add_equipment(2, 3, 300.0);
    gf_add_equipment(2, 3, 280.0);
    gf_add_equipment(2, 4, 350.0);
    gf_add_equipment(3, 5, 150.0);
    gf_add_equipment(3, 5, 160.0);
    gf_add_equipment(3, 6, 200.0);
    gf_add_equipment(4, 7, 120.0);
    gf_add_equipment(4, 7, 130.0);
    gf_add_equipment(4, 8, 180.0);

    print_str("\nRegistering members...\n");
    gf_register_member(0, 12);
    gf_register_member(1, 6);
    gf_register_member(2, 12);
    gf_register_member(3, 6);
    gf_register_member(0, 3);
    gf_register_member(1, 12);
    gf_register_member(2, 6);
    gf_register_member(3, 3);
    gf_register_member(0, 6);
    gf_register_member(1, 3);
    gf_register_member(2, 12);
    gf_register_member(3, 6);
    gf_register_member(0, 1);
    gf_register_member(1, 1);
    gf_register_member(2, 1);
    gf_register_member(3, 1);

    print_str("\nAdding classes...\n");
    gf_add_class(1, 0, 15, 20.0, 10, 7);
    gf_add_class(2, 1, 12, 25.0, 10, 9);
    gf_add_class(3, 2, 20, 15.0, 10, 18);
    gf_add_class(4, 3, 10, 30.0, 11, 7);
    gf_add_class(1, 4, 15, 20.0, 11, 9);
    gf_add_class(2, 5, 12, 25.0, 11, 18);
    gf_add_class(3, 6, 20, 15.0, 12, 7);
    gf_add_class(4, 7, 10, 30.0, 12, 9);
    gf_add_class(1, 0, 15, 20.0, 12, 18);
    gf_add_class(2, 1, 12, 25.0, 13, 7);

    print_str("\nEnrolling members...\n");
    gf_enroll_member(0, 0);
    gf_enroll_member(1, 1);
    gf_enroll_member(2, 2);
    gf_enroll_member(3, 3);
    gf_enroll_member(4, 4);
    gf_enroll_member(5, 5);
    gf_enroll_member(6, 6);
    gf_enroll_member(7, 7);
    gf_enroll_member(8, 8);
    gf_enroll_member(9, 9);
    gf_enroll_member(10, 0);
    gf_enroll_member(11, 1);
    gf_enroll_member(0, 3);
    gf_enroll_member(2, 5);
    gf_enroll_member(4, 7);

    print_str("\nCheck-ins...\n");
    for (int d = 10; d <= 15; d++) {
        for (int m = 0; m < 12; m++) {
            if (gf.n_checkins >= MAX_CHECKINS) goto done_ci;
            gf_checkin(m, d, 7 + (m % 12), 60);
        }
    }
done_ci:

    print_str("\nEquipment usage...\n");
    for (int e = 0; e < 12; e++) {
        gf_use_equipment(e, e % 12);
    }

    print_str("\nPrivate sessions...\n");
    gf_private_session(0, 0, 1);
    gf_private_session(2, 1, 2);
    gf_private_session(4, 3, 1);
    gf_private_session(6, 5, 1);
    gf_private_session(8, 7, 2);
    gf_private_session(10, 2, 1);

    print_str("\nMaintenance...\n");
    gf_schedule_maintenance(0, 20);
    gf_schedule_maintenance(3, 22);
    gf_schedule_maintenance(6, 25);
    gf_complete_maintenance(0);
    gf_complete_maintenance(3);
    gf_complete_maintenance(6);

    print_str("\nTrainer report...\n");
    gf_trainer_report();

    print_str("\nFinancial report...\n");
    gf_financial_report();

    print_str("\nFinal state...\n");
    gf_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
