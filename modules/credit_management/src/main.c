/* credit_management: Credit management administration system (v1.0)
 * Credit records, credit rating, dishonesty sanctions, credit supervision, credit service
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

#define MAX_CREDIT_RECORD  16
#define MAX_CREDIT_RATING  14
#define MAX_DISHONESTY     12
#define MAX_CREDIT_SUPER   10
#define MAX_CREDIT_SVC     10

typedef struct {
    int    record_id;
    int    record_type;
    int    entity_category;
    int    info_collected;
    int    archives_created;
    int    scores_assigned;
    int    year;
    int    active;
} credit_record_t;

typedef struct {
    int    rating_id;
    int    rating_type;
    int    evaluation_target;
    int    enterprise_ratings;
    int    personal_assessments;
    int    levels_published;
    int    year;
    int    active;
} credit_rating_t;

typedef struct {
    int    dishonesty_id;
    int    sanction_type;
    int    violation_category;
    int    blacklist_entries;
    int    joint_sanctions;
    int    credit_repairs;
    int    year;
    int    active;
} dishonesty_t;

typedef struct {
    int    supervision_id;
    int    supervision_type;
    int    monitoring_area;
    int    credit_monitors;
    int    risk_warnings;
    int    credit_disclosures;
    int    year;
    int    active;
} credit_supervision_t;

typedef struct {
    int    service_id;
    int    service_type;
    int    service_category;
    int    credit_queries;
    int    credit_consultations;
    int    credit_trainings;
    int    year;
    int    active;
} credit_svc_t;

typedef struct {
    int    n_record;
    int    n_rating;
    int    n_dishonesty;
    int    n_supervision;
    int    n_service;
    int    total_records;
    int    total_ratings;
    int    total_blacklist;
    int    total_monitors;
    int    total_queries;
} cm_state_t;

static credit_record_t records[MAX_CREDIT_RECORD];
static credit_rating_t ratings[MAX_CREDIT_RATING];
static dishonesty_t dishonesties[MAX_DISHONESTY];
static credit_supervision_t supervisions[MAX_CREDIT_SUPER];
static credit_svc_t services[MAX_CREDIT_SVC];
static cm_state_t cm;

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

int cm_init(void) {
    if (initialized) return -1;
    cm.n_record = 0; cm.n_rating = 0; cm.n_dishonesty = 0;
    cm.n_supervision = 0; cm.n_service = 0;
    cm.total_records = 0; cm.total_ratings = 0;
    cm.total_blacklist = 0; cm.total_monitors = 0;
    cm.total_queries = 0;
    for (int i = 0; i < MAX_CREDIT_RECORD; i++) records[i].active = 0;
    for (int i = 0; i < MAX_CREDIT_RATING; i++) ratings[i].active = 0;
    for (int i = 0; i < MAX_DISHONESTY; i++) dishonesties[i].active = 0;
    for (int i = 0; i < MAX_CREDIT_SUPER; i++) supervisions[i].active = 0;
    for (int i = 0; i < MAX_CREDIT_SVC; i++) services[i].active = 0;
    initialized = 1;
    print_str("[CM] Credit management initialized\n");
    return 0;
}

int cm_record(int rec_type, int entity, int collected, int archives, int scores, int year) {
    if (cm.n_record >= MAX_CREDIT_RECORD) return -1;
    credit_record_t* r = &records[cm.n_record];
    r->record_id = cm.n_record;
    r->record_type = rec_type;
    r->entity_category = entity;
    r->info_collected = collected;
    r->archives_created = archives;
    r->scores_assigned = scores;
    r->year = year;
    r->active = 1;
    cm.total_records += collected;
    cm.n_record++;
    print_str("[CM] Record "); print_int(cm.n_record - 1);
    print_str(" type="); print_int(rec_type);
    print_str(" ent="); print_int(entity);
    print_str(" col="); print_int(collected);
    print_str(" arc="); print_int(archives);
    print_str(" scr="); print_int(scores); print_str("\n");
    return cm.n_record - 1;
}

int cm_rating(int rat_type, int target, int enterprise, int personal, int levels, int year) {
    if (cm.n_rating >= MAX_CREDIT_RATING) return -1;
    credit_rating_t* r = &ratings[cm.n_rating];
    r->rating_id = cm.n_rating;
    r->rating_type = rat_type;
    r->evaluation_target = target;
    r->enterprise_ratings = enterprise;
    r->personal_assessments = personal;
    r->levels_published = levels;
    r->year = year;
    r->active = 1;
    cm.total_ratings += enterprise;
    cm.n_rating++;
    print_str("[CM] Rating "); print_int(cm.n_rating - 1);
    print_str(" type="); print_int(rat_type);
    print_str(" tgt="); print_int(target);
    print_str(" ent="); print_int(enterprise);
    print_str(" per="); print_int(personal);
    print_str(" lvl="); print_int(levels); print_str("\n");
    return cm.n_rating - 1;
}

int cm_dishonesty(int sanc_type, int violation, int blacklist, int joint, int repairs, int year) {
    if (cm.n_dishonesty >= MAX_DISHONESTY) return -1;
    dishonesty_t* d = &dishonesties[cm.n_dishonesty];
    d->dishonesty_id = cm.n_dishonesty;
    d->sanction_type = sanc_type;
    d->violation_category = violation;
    d->blacklist_entries = blacklist;
    d->joint_sanctions = joint;
    d->credit_repairs = repairs;
    d->year = year;
    d->active = 1;
    cm.total_blacklist += blacklist;
    cm.n_dishonesty++;
    print_str("[CM] Dishonesty "); print_int(cm.n_dishonesty - 1);
    print_str(" type="); print_int(sanc_type);
    print_str(" vio="); print_int(violation);
    print_str(" blk="); print_int(blacklist);
    print_str(" jnt="); print_int(joint);
    print_str(" rpr="); print_int(repairs); print_str("\n");
    return cm.n_dishonesty - 1;
}

int cm_supervision(int sup_type, int area, int monitors, int warnings, int disclosures, int year) {
    if (cm.n_supervision >= MAX_CREDIT_SUPER) return -1;
    credit_supervision_t* s = &supervisions[cm.n_supervision];
    s->supervision_id = cm.n_supervision;
    s->supervision_type = sup_type;
    s->monitoring_area = area;
    s->credit_monitors = monitors;
    s->risk_warnings = warnings;
    s->credit_disclosures = disclosures;
    s->year = year;
    s->active = 1;
    cm.total_monitors += monitors;
    cm.n_supervision++;
    print_str("[CM] Supervision "); print_int(cm.n_supervision - 1);
    print_str(" type="); print_int(sup_type);
    print_str(" area="); print_int(area);
    print_str(" mon="); print_int(monitors);
    print_str(" wrn="); print_int(warnings);
    print_str(" dsc="); print_int(disclosures); print_str("\n");
    return cm.n_supervision - 1;
}

int cm_service(int svc_type, int svc_cat, int queries, int consultations, int trainings, int year) {
    if (cm.n_service >= MAX_CREDIT_SVC) return -1;
    credit_svc_t* sv = &services[cm.n_service];
    sv->service_id = cm.n_service;
    sv->service_type = svc_type;
    sv->service_category = svc_cat;
    sv->credit_queries = queries;
    sv->credit_consultations = consultations;
    sv->credit_trainings = trainings;
    sv->year = year;
    sv->active = 1;
    cm.total_queries += queries;
    cm.n_service++;
    print_str("[CM] Service "); print_int(cm.n_service - 1);
    print_str(" type="); print_int(svc_type);
    print_str(" cat="); print_int(svc_cat);
    print_str(" qry="); print_int(queries);
    print_str(" csl="); print_int(consultations);
    print_str(" trn="); print_int(trainings); print_str("\n");
    return cm.n_service - 1;
}

void cm_record_report(void) {
    print_str("[CM] Record report:\n");
    print_str("  Record categories: "); print_int(cm.n_record); print_str("\n");
    print_str("  Total records collected: "); print_int(cm.total_records); print_str("\n");
}

void cm_rating_report(void) {
    print_str("[CM] Rating report:\n");
    print_str("  Rating categories: "); print_int(cm.n_rating); print_str("\n");
    print_str("  Total ratings assigned: "); print_int(cm.total_ratings); print_str("\n");
}

void cm_service_report(void) {
    print_str("[CM] Service report:\n");
    print_str("  Dishonesty sanction categories: "); print_int(cm.n_dishonesty); print_str("\n");
    print_str("  Total blacklist entries: "); print_int(cm.total_blacklist); print_str("\n");
    print_str("  Credit supervision categories: "); print_int(cm.n_supervision); print_str("\n");
    print_str("  Total credit monitors: "); print_int(cm.total_monitors); print_str("\n");
    print_str("  Credit service categories: "); print_int(cm.n_service); print_str("\n");
    print_str("  Total credit queries: "); print_int(cm.total_queries); print_str("\n");
}

void cm_print_state(void) {
    print_str("[CM] Rc="); print_int(cm.n_record);
    print_str(" Rt="); print_int(cm.n_rating);
    print_str(" Dh="); print_int(cm.n_dishonesty);
    print_str(" Sp="); print_int(cm.n_supervision);
    print_str(" Sv="); print_int(cm.n_service);
    print_str("\n");
}

int main(void) {
    print_str("=== Credit Management Demo ===\n\n");
    cm_init();

    print_str("Credit records...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int ent = (i % 4) + 1;
        int col = 100 + (i * 25);
        int arc = 80 + (i * 20);
        int scr = 70 + (i * 18);
        int year = 2020 + (i % 5);
        cm_record(type, ent, col, arc, scr, year);
    }

    print_str("\nCredit ratings...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int tgt = (i % 5) + 1;
        int ent = 50 + (i * 12);
        int per = 200 + (i * 50);
        int lvl = 10 + (i * 3);
        int year = 2021 + (i % 4);
        cm_rating(type, tgt, ent, per, lvl, year);
    }

    print_str("\nDishonesty sanctions...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int vio = (i % 5) + 1;
        int blk = 20 + (i * 5);
        int jnt = 15 + (i * 4);
        int rpr = 5 + (i * 2);
        int year = 2022 + (i % 3);
        cm_dishonesty(type, vio, blk, jnt, rpr, year);
    }

    print_str("\nCredit supervision...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int area = (i % 5) + 1;
        int mon = 60 + (i * 15);
        int wrn = 30 + (i * 8);
        int dsc = 40 + (i * 10);
        int year = 2023 + (i % 2);
        cm_supervision(type, area, mon, wrn, dsc, year);
    }

    print_str("\nCredit service...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int qry = 300 + (i * 75);
        int csl = 100 + (i * 25);
        int trn = 20 + (i * 5);
        int year = 2024;
        cm_service(type, cat, qry, csl, trn, year);
    }

    print_str("\nRecord report...\n");
    cm_record_report();

    print_str("\nRating report...\n");
    cm_rating_report();

    print_str("\nService report...\n");
    cm_service_report();

    print_str("\nFinal state...\n");
    cm_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
