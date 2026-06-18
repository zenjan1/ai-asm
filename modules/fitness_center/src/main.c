/* fitness_center: Fitness center management (v1.0)
 * Memberships, classes, equipment, check-in, analytics
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
#define MAX_TRAINERS   8
#define MAX_CLASSES    12
#define MAX_EQUIPMENT  10
#define MAX_BOOKINGS   18
#define MAX_CHECKINS   20

typedef struct {
    int    member_id;
    int    tier;
    int    join_day;
    int    expiry_day;
    double monthly_fee;
    int    n_visits;
    int    n_classes;
    double total_paid;
    int    active;
} member_t;

typedef struct {
    int    trainer_id;
    int    specialty;
    int    rating;
    int    n_sessions;
    double revenue;
    int    active;
} trainer_t;

typedef struct {
    int    class_id;
    int    trainer_id;
    int    type;
    int    capacity;
    int    enrolled;
    int    day;
    int    duration_min;
    double price;
    int    active;
} class_t;

typedef struct {
    int    equip_id;
    int    type;
    int    usage_count;
    int    last_service_day;
    int    status;
    int    active;
} equipment_t;

typedef struct {
    int    booking_id;
    int    member_id;
    int    class_id;
    int    day;
    int    status;
    int    active;
} booking_t;

typedef struct {
    int    checkin_id;
    int    member_id;
    int    day;
    int    duration_min;
    int    type;
    int    active;
} checkin_t;

typedef struct {
    int    n_members;
    int    n_trainers;
    int    n_classes;
    int    n_equipment;
    int    n_bookings;
    int    n_checkins;
    double total_revenue;
    double membership_revenue;
    double class_revenue;
    int    total_visits;
    int    active_members;
} fc_state_t;

static member_t members[MAX_MEMBERS];
static trainer_t trainers[MAX_TRAINERS];
static class_t classes[MAX_CLASSES];
static equipment_t equipment[MAX_EQUIPMENT];
static booking_t bookings[MAX_BOOKINGS];
static checkin_t checkins[MAX_CHECKINS];
static fc_state_t fc;

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

int fc_init(void) {
    if (initialized) return -1;
    fc.n_members = 0; fc.n_trainers = 0; fc.n_classes = 0;
    fc.n_equipment = 0; fc.n_bookings = 0; fc.n_checkins = 0;
    fc.total_revenue = 0.0; fc.membership_revenue = 0.0;
    fc.class_revenue = 0.0; fc.total_visits = 0; fc.active_members = 0;
    for (int i = 0; i < MAX_MEMBERS; i++) members[i].active = 0;
    for (int i = 0; i < MAX_TRAINERS; i++) trainers[i].active = 0;
    for (int i = 0; i < MAX_CLASSES; i++) classes[i].active = 0;
    for (int i = 0; i < MAX_EQUIPMENT; i++) equipment[i].active = 0;
    for (int i = 0; i < MAX_BOOKINGS; i++) bookings[i].active = 0;
    for (int i = 0; i < MAX_CHECKINS; i++) checkins[i].active = 0;
    initialized = 1;
    print_str("[FC] Fitness center initialized\n");
    return 0;
}

int fc_add_member(int tier, int join_day) {
    if (fc.n_members >= MAX_MEMBERS) return -1;
    member_t* m = &members[fc.n_members];
    m->member_id = fc.n_members;
    m->tier = tier;
    m->join_day = join_day;
    m->expiry_day = join_day + 30;
    double fees[4] = {29.99, 49.99, 79.99, 129.99};
    m->monthly_fee = fees[tier < 4 ? tier : 0];
    m->n_visits = 0;
    m->n_classes = 0;
    m->total_paid = 0.0;
    m->active = 1;
    fc.n_members++;
    fc.active_members++;
    print_str("[FC] Member "); print_int(fc.n_members - 1);
    print_str(" tier="); print_int(tier);
    print_str(" fee="); print_int((int)m->monthly_fee); print_str("/mo\n");
    return fc.n_members - 1;
}

int fc_add_trainer(int specialty, int rating) {
    if (fc.n_trainers >= MAX_TRAINERS) return -1;
    trainer_t* t = &trainers[fc.n_trainers];
    t->trainer_id = fc.n_trainers;
    t->specialty = specialty;
    t->rating = rating;
    t->n_sessions = 0;
    t->revenue = 0.0;
    t->active = 1;
    fc.n_trainers++;
    print_str("[FC] Trainer "); print_int(fc.n_trainers - 1);
    print_str(" specialty="); print_int(specialty);
    print_str(" rating="); print_int(rating); print_str("\n");
    return fc.n_trainers - 1;
}

int fc_add_class(int trainer_id, int type, int capacity, int day, int duration, double price) {
    if (fc.n_classes >= MAX_CLASSES || trainer_id >= fc.n_trainers) return -1;
    class_t* c = &classes[fc.n_classes];
    c->class_id = fc.n_classes;
    c->trainer_id = trainer_id;
    c->type = type;
    c->capacity = capacity;
    c->enrolled = 0;
    c->day = day;
    c->duration_min = duration;
    c->price = price;
    c->active = 1;
    fc.n_classes++;
    print_str("[FC] Class "); print_int(fc.n_classes - 1);
    print_str(" T"); print_int(trainer_id);
    print_str(" type="); print_int(type);
    print_str(" cap="); print_int(capacity);
    print_str(" day="); print_int(day);
    print_str(" dur="); print_int(duration); print_str("min");
    print_str(" price="); print_int((int)price); print_str("\n");
    return fc.n_classes - 1;
}

int fc_add_equipment(int type) {
    if (fc.n_equipment >= MAX_EQUIPMENT) return -1;
    equipment_t* e = &equipment[fc.n_equipment];
    e->equip_id = fc.n_equipment;
    e->type = type;
    e->usage_count = 0;
    e->last_service_day = 0;
    e->status = 1;
    e->active = 1;
    fc.n_equipment++;
    print_str("[FC] Equipment "); print_int(fc.n_equipment - 1);
    print_str(" type="); print_int(type); print_str("\n");
    return fc.n_equipment - 1;
}

int fc_book_class(int member_id, int class_id, int day) {
    if (fc.n_bookings >= MAX_BOOKINGS) return -1;
    if (member_id >= fc.n_members || class_id >= fc.n_classes) return -2;
    class_t* c = &classes[class_id];
    if (c->enrolled >= c->capacity) return -3;
    booking_t* b = &bookings[fc.n_bookings];
    b->booking_id = fc.n_bookings;
    b->member_id = member_id;
    b->class_id = class_id;
    b->day = day;
    b->status = 1;
    b->active = 1;
    c->enrolled++;
    fc.n_bookings++;
    print_str("[FC] Booking "); print_int(fc.n_bookings - 1);
    print_str(" M"); print_int(member_id);
    print_str(" C"); print_int(class_id);
    print_str(" day="); print_int(day); print_str("\n");
    return fc.n_bookings - 1;
}

int fc_checkin(int member_id, int day, int duration, int type) {
    if (fc.n_checkins >= MAX_CHECKINS || member_id >= fc.n_members) return -1;
    checkin_t* c = &checkins[fc.n_checkins];
    c->checkin_id = fc.n_checkins;
    c->member_id = member_id;
    c->day = day;
    c->duration_min = duration;
    c->type = type;
    c->active = 1;
    members[member_id].n_visits++;
    fc.total_visits++;
    fc.n_checkins++;
    print_str("[FC] Check-in "); print_int(fc.n_checkins - 1);
    print_str(" M"); print_int(member_id);
    print_str(" day="); print_int(day);
    print_str(" dur="); print_int(duration); print_str("min");
    if (type == 1) print_str(" gym");
    else if (type == 2) print_str(" class");
    else print_str(" pool");
    print_str("\n");
    return fc.n_checkins - 1;
}

void fc_renew_membership(int member_id, int months) {
    if (member_id >= fc.n_members) return;
    member_t* m = &members[member_id];
    double fee = m->monthly_fee * months;
    m->expiry_day += months * 30;
    m->total_paid += fee;
    fc.membership_revenue += fee;
    fc.total_revenue += fee;
    print_str("[FC] Member "); print_int(member_id);
    print_str(" renewed "); print_int(months); print_str("mo");
    print_str(" fee="); print_int((int)fee); print_str("\n");
}

void fc_attend_class(int booking_id) {
    if (booking_id >= fc.n_bookings) return;
    booking_t* b = &bookings[booking_id];
    b->status = 3;
    members[b->member_id].n_classes++;
    class_t* c = &classes[b->class_id];
    double fee = c->price;
    members[b->member_id].total_paid += fee;
    fc.class_revenue += fee;
    fc.total_revenue += fee;
    trainers[c->trainer_id].n_sessions++;
    trainers[c->trainer_id].revenue += fee;
    print_str("[FC] Booking "); print_int(booking_id);
    print_str(" attended M"); print_int(b->member_id);
    print_str(" C"); print_int(b->class_id);
    print_str(" fee="); print_int((int)fee); print_str("\n");
}

void fc_service_equipment(int equip_id, int day) {
    if (equip_id >= fc.n_equipment) return;
    equipment[equip_id].last_service_day = day;
    equipment[equip_id].status = 1;
    print_str("[FC] Equipment "); print_int(equip_id);
    print_str(" serviced day="); print_int(day); print_str("\n");
}

void fc_use_equipment(int equip_id) {
    if (equip_id >= fc.n_equipment) return;
    equipment[equip_id].usage_count++;
    print_str("[FC] Equipment "); print_int(equip_id);
    print_str(" used (total="); print_int(equipment[equip_id].usage_count); print_str(")\n");
}

void fc_member_analytics(void) {
    print_str("[FC] Member analytics:\n");
    for (int i = 0; i < fc.n_members; i++) {
        member_t* m = &members[i];
        print_str("  M"); print_int(i);
        print_str(" tier="); print_int(m->tier);
        print_str(" visits="); print_int(m->n_visits);
        print_str(" classes="); print_int(m->n_classes);
        print_str(" paid="); print_int((int)m->total_paid); print_str("\n");
    }
}

void fc_revenue_report(void) {
    print_str("[FC] Revenue report:\n");
    print_str("  Membership: "); print_int((int)fc.membership_revenue); print_str("\n");
    print_str("  Classes: "); print_int((int)fc.class_revenue); print_str("\n");
    print_str("  Total: "); print_int((int)fc.total_revenue); print_str("\n");
    print_str("  Active members: "); print_int(fc.active_members); print_str("\n");
    print_str("  Total visits: "); print_int(fc.total_visits); print_str("\n");
    double avg_visits = (fc.active_members > 0) ? (double)fc.total_visits / fc.active_members : 0.0;
    print_str("  Avg visits/member: "); print_int((int)avg_visits); print_str("\n");
}

void fc_print_state(void) {
    print_str("[FC] Members="); print_int(fc.n_members);
    print_str(" Trainers="); print_int(fc.n_trainers);
    print_str(" Classes="); print_int(fc.n_classes);
    print_str(" Equipment="); print_int(fc.n_equipment);
    print_str(" Bookings="); print_int(fc.n_bookings);
    print_str(" Checkins="); print_int(fc.n_checkins);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)fc.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Fitness Center Demo ===\n\n");
    fc_init();

    print_str("Adding trainers...\n");
    fc_add_trainer(1, 90);
    fc_add_trainer(2, 85);
    fc_add_trainer(3, 92);
    fc_add_trainer(1, 88);
    fc_add_trainer(4, 80);

    print_str("\nAdding members...\n");
    fc_add_member(0, 1);
    fc_add_member(1, 1);
    fc_add_member(2, 2);
    fc_add_member(3, 1);
    fc_add_member(1, 3);
    fc_add_member(0, 2);
    fc_add_member(2, 1);
    fc_add_member(1, 4);
    fc_add_member(0, 2);
    fc_add_member(3, 1);

    print_str("\nAdding classes...\n");
    fc_add_class(0, 1, 15, 10, 60, 15.0);
    fc_add_class(1, 2, 10, 11, 45, 20.0);
    fc_add_class(2, 3, 20, 12, 60, 12.0);
    fc_add_class(3, 1, 12, 13, 60, 18.0);
    fc_add_class(4, 4, 8, 14, 90, 25.0);
    fc_add_class(0, 2, 15, 15, 45, 15.0);
    fc_add_class(1, 1, 10, 16, 60, 20.0);

    print_str("\nAdding equipment...\n");
    fc_add_equipment(1);
    fc_add_equipment(1);
    fc_add_equipment(2);
    fc_add_equipment(2);
    fc_add_equipment(3);
    fc_add_equipment(3);
    fc_add_equipment(4);
    fc_add_equipment(4);

    print_str("\nBooking classes...\n");
    fc_book_class(0, 0, 10);
    fc_book_class(0, 2, 12);
    fc_book_class(1, 1, 11);
    fc_book_class(1, 4, 14);
    fc_book_class(2, 0, 10);
    fc_book_class(2, 3, 13);
    fc_book_class(3, 5, 15);
    fc_book_class(3, 6, 16);
    fc_book_class(4, 2, 12);
    fc_book_class(4, 1, 11);
    fc_book_class(5, 0, 10);
    fc_book_class(5, 3, 13);
    fc_book_class(6, 5, 15);
    fc_book_class(7, 6, 16);
    fc_book_class(7, 4, 14);
    fc_book_class(8, 0, 10);
    fc_book_class(9, 2, 12);
    fc_book_class(9, 5, 15);

    print_str("\nCheck-ins...\n");
    fc_checkin(0, 10, 90, 1);
    fc_checkin(0, 12, 60, 2);
    fc_checkin(1, 11, 75, 1);
    fc_checkin(1, 14, 90, 2);
    fc_checkin(2, 10, 60, 1);
    fc_checkin(2, 13, 60, 2);
    fc_checkin(3, 15, 120, 1);
    fc_checkin(3, 16, 60, 2);
    fc_checkin(4, 12, 60, 2);
    fc_checkin(4, 11, 45, 1);
    fc_checkin(5, 10, 90, 1);
    fc_checkin(5, 13, 60, 2);
    fc_checkin(6, 15, 60, 2);
    fc_checkin(7, 16, 75, 2);
    fc_checkin(7, 14, 90, 2);
    fc_checkin(8, 10, 60, 1);
    fc_checkin(9, 12, 60, 2);
    fc_checkin(9, 15, 45, 2);

    print_str("\nAttending classes...\n");
    fc_attend_class(0);
    fc_attend_class(1);
    fc_attend_class(2);
    fc_attend_class(3);
    fc_attend_class(4);
    fc_attend_class(5);
    fc_attend_class(6);
    fc_attend_class(7);
    fc_attend_class(8);
    fc_attend_class(9);
    fc_attend_class(10);
    fc_attend_class(11);
    fc_attend_class(12);
    fc_attend_class(13);
    fc_attend_class(14);
    fc_attend_class(15);
    fc_attend_class(16);
    fc_attend_class(17);

    print_str("\nRenewing memberships...\n");
    fc_renew_membership(0, 3);
    fc_renew_membership(1, 6);
    fc_renew_membership(2, 3);
    fc_renew_membership(3, 12);
    fc_renew_membership(4, 6);
    fc_renew_membership(5, 3);
    fc_renew_membership(6, 3);
    fc_renew_membership(7, 6);
    fc_renew_membership(8, 3);
    fc_renew_membership(9, 12);

    print_str("\nUsing equipment...\n");
    fc_use_equipment(0);
    fc_use_equipment(1);
    fc_use_equipment(2);
    fc_use_equipment(3);
    fc_use_equipment(4);
    fc_use_equipment(5);
    fc_use_equipment(6);
    fc_use_equipment(7);
    fc_use_equipment(0);
    fc_use_equipment(2);
    fc_use_equipment(4);
    fc_use_equipment(6);

    print_str("\nServicing equipment...\n");
    fc_service_equipment(0, 20);
    fc_service_equipment(2, 20);
    fc_service_equipment(4, 20);
    fc_service_equipment(6, 20);

    print_str("\nMember analytics...\n");
    fc_member_analytics();

    print_str("\nRevenue report...\n");
    fc_revenue_report();

    print_str("\nFinal state...\n");
    fc_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
