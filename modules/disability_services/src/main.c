/* disability_services: Disability services management (v1.0)
 * Rehabilitation, education/employment, social security, accessibility, activities
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

#define MAX_REHAB        16
#define MAX_EDUCATION    14
#define MAX_SECURITY     12
#define MAX_ACCESS       10
#define MAX_ACTIVITY     10

typedef struct {
    int    rehab_id;
    int    person_id;
    int    disability_type;
    int    assessment_score;
    int    training_hours;
    int    assistive_device;
    int    improvement;
    int    year;
    int    active;
} rehab_t;

typedef struct {
    int    edu_id;
    int    person_id;
    int    type;
    int    program;
    int    duration;
    int    placement;
    int    outcome;
    int    year;
    int    active;
} education_t;

typedef struct {
    int    security_id;
    int    person_id;
    int    category;
    int    living_allowance;
    int    nursing_subsidy;
    int    insurance;
    int    total_received;
    int    year;
    int    active;
} security_t;

typedef struct {
    int    access_id;
    int    facility_id;
    int    type;
    int    renovation_cost;
    int    compliance;
    int    users_served;
    int    year;
    int    status;
    int    active;
} access_t;

typedef struct {
    int    activity_id;
    int    type;
    int    participants;
    int    volunteers;
    int    budget;
    int    satisfaction;
    int    year;
    int    active;
} activity_t;

typedef struct {
    int    n_rehab;
    int    n_education;
    int    n_security;
    int    n_access;
    int    n_activity;
    int    total_training;
    int    total_devices;
    int    total_placed;
    int    total_allowance;
    int    total_users;
} ds_state_t;

static rehab_t rehabs[MAX_REHAB];
static education_t educations[MAX_EDUCATION];
static security_t securities[MAX_SECURITY];
static access_t accessibility[MAX_ACCESS];
static activity_t activities[MAX_ACTIVITY];
static ds_state_t ds;

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

int ds_init(void) {
    if (initialized) return -1;
    ds.n_rehab = 0; ds.n_education = 0; ds.n_security = 0;
    ds.n_access = 0; ds.n_activity = 0;
    ds.total_training = 0; ds.total_devices = 0;
    ds.total_placed = 0; ds.total_allowance = 0;
    ds.total_users = 0;
    for (int i = 0; i < MAX_REHAB; i++) rehabs[i].active = 0;
    for (int i = 0; i < MAX_EDUCATION; i++) educations[i].active = 0;
    for (int i = 0; i < MAX_SECURITY; i++) securities[i].active = 0;
    for (int i = 0; i < MAX_ACCESS; i++) accessibility[i].active = 0;
    for (int i = 0; i < MAX_ACTIVITY; i++) activities[i].active = 0;
    initialized = 1;
    print_str("[DS] Disability services initialized\n");
    return 0;
}

int ds_rehab(int person, int dis_type, int assessment, int training, int device, int improvement, int year) {
    if (ds.n_rehab >= MAX_REHAB) return -1;
    rehab_t* r = &rehabs[ds.n_rehab];
    r->rehab_id = ds.n_rehab;
    r->person_id = person;
    r->disability_type = dis_type;
    r->assessment_score = assessment;
    r->training_hours = training;
    r->assistive_device = device;
    r->improvement = improvement;
    r->year = year;
    r->active = 1;
    ds.total_training += training;
    if (device) ds.total_devices++;
    ds.n_rehab++;
    print_str("[DS] Rehab "); print_int(ds.n_rehab - 1);
    print_str(" per="); print_int(person);
    print_str(" dis="); print_int(dis_type);
    print_str(" asm="); print_int(assessment);
    print_str(" trn="); print_int(training); print_str("h");
    print_str(" dev="); print_int(device);
    print_str(" imp="); print_int(improvement); print_str("\n");
    return ds.n_rehab - 1;
}

int ds_education(int person, int type, int program, int duration, int placement, int outcome, int year) {
    if (ds.n_education >= MAX_EDUCATION) return -1;
    education_t* e = &educations[ds.n_education];
    e->edu_id = ds.n_education;
    e->person_id = person;
    e->type = type;
    e->program = program;
    e->duration = duration;
    e->placement = placement;
    e->outcome = outcome;
    e->year = year;
    e->active = 1;
    if (placement) ds.total_placed++;
    ds.n_education++;
    print_str("[DS] Education "); print_int(ds.n_education - 1);
    print_str(" per="); print_int(person);
    print_str(" type="); print_int(type);
    print_str(" prg="); print_int(program);
    print_str(" dur="); print_int(duration);
    print_str(" plc="); print_int(placement);
    print_str(" out="); print_int(outcome); print_str("\n");
    return ds.n_education - 1;
}

int ds_security(int person, int category, int living, int nursing, int insurance, int total, int year) {
    if (ds.n_security >= MAX_SECURITY) return -1;
    security_t* s = &securities[ds.n_security];
    s->security_id = ds.n_security;
    s->person_id = person;
    s->category = category;
    s->living_allowance = living;
    s->nursing_subsidy = nursing;
    s->insurance = insurance;
    s->total_received = total;
    s->year = year;
    s->active = 1;
    ds.total_allowance += living + nursing;
    ds.n_security++;
    print_str("[DS] Security "); print_int(ds.n_security - 1);
    print_str(" per="); print_int(person);
    print_str(" cat="); print_int(category);
    print_str(" liv=$"); print_int(living);
    print_str(" nur=$"); print_int(nursing);
    print_str(" ins=$"); print_int(insurance); print_str("\n");
    return ds.n_security - 1;
}

int ds_access(int facility, int type, int cost, int compliance, int users, int year) {
    if (ds.n_access >= MAX_ACCESS) return -1;
    access_t* a = &accessibility[ds.n_access];
    a->access_id = ds.n_access;
    a->facility_id = facility;
    a->type = type;
    a->renovation_cost = cost;
    a->compliance = compliance;
    a->users_served = users;
    a->year = year;
    a->status = 1;
    a->active = 1;
    ds.total_users += users;
    ds.n_access++;
    print_str("[DS] Access "); print_int(ds.n_access - 1);
    print_str(" fac="); print_int(facility);
    print_str(" type="); print_int(type);
    print_str(" cst=$"); print_int(cost);
    print_str(" cmp="); print_int(compliance);
    print_str(" usr="); print_int(users); print_str("\n");
    return ds.n_access - 1;
}

int ds_activity(int type, int participants, int volunteers, int budget, int satisfaction, int year) {
    if (ds.n_activity >= MAX_ACTIVITY) return -1;
    activity_t* a = &activities[ds.n_activity];
    a->activity_id = ds.n_activity;
    a->type = type;
    a->participants = participants;
    a->volunteers = volunteers;
    a->budget = budget;
    a->satisfaction = satisfaction;
    a->year = year;
    a->active = 1;
    ds.n_activity++;
    print_str("[DS] Activity "); print_int(ds.n_activity - 1);
    print_str(" type="); print_int(type);
    print_str(" ptc="); print_int(participants);
    print_str(" vol="); print_int(volunteers);
    print_str(" bgt=$"); print_int(budget);
    print_str(" sat="); print_int(satisfaction); print_str("\n");
    return ds.n_activity - 1;
}

void ds_rehab_report(void) {
    print_str("[DS] Rehab report:\n");
    print_str("  Rehab cases: "); print_int(ds.n_rehab); print_str("\n");
    print_str("  Total training hours: "); print_int(ds.total_training); print_str("\n");
    print_str("  Devices provided: "); print_int(ds.total_devices); print_str("\n");
}

void ds_education_report(void) {
    print_str("[DS] Education report:\n");
    print_str("  Education cases: "); print_int(ds.n_education); print_str("\n");
    print_str("  Total placed: "); print_int(ds.total_placed); print_str("\n");
}

void ds_security_report(void) {
    print_str("[DS] Security report:\n");
    print_str("  Beneficiaries: "); print_int(ds.n_security); print_str("\n");
    print_str("  Total allowance: $"); print_int(ds.total_allowance); print_str("\n");
    print_str("  Access facilities: "); print_int(ds.n_access); print_str("\n");
    print_str("  Total users served: "); print_int(ds.total_users); print_str("\n");
    print_str("  Activities: "); print_int(ds.n_activity); print_str("\n");
}

void ds_print_state(void) {
    print_str("[DS] Rh="); print_int(ds.n_rehab);
    print_str(" Ed="); print_int(ds.n_education);
    print_str(" Sc="); print_int(ds.n_security);
    print_str(" Ac="); print_int(ds.n_access);
    print_str(" At="); print_int(ds.n_activity);
    print_str("\n");
}

int main(void) {
    print_str("=== Disability Services Demo ===\n\n");
    ds_init();

    print_str("Rehabilitation services...\n");
    for (int i = 0; i < 16; i++) {
        int per = 1000 + (i * 17);
        int dis = (i % 5) + 1;
        int asmt = 40 + (i * 4);
        int trn = 20 + (i * 15);
        int dev = (i % 3 == 0) ? 1 : 0;
        int imp = 10 + (i * 5);
        int year = 2021 + (i % 4);
        ds_rehab(per, dis, asmt, trn, dev, imp, year);
    }

    print_str("\nEducation and employment...\n");
    for (int i = 0; i < 14; i++) {
        int per = 2000 + (i * 13);
        int type = (i % 3) + 1;
        int prg = (i % 6) + 1;
        int dur = 3 + (i % 12);
        int plc = (i % 3 == 0) ? 1 : 0;
        int out = 60 + (i * 3);
        int year = 2022 + (i % 3);
        ds_education(per, type, prg, dur, plc, out, year);
    }

    print_str("\nSocial security...\n");
    for (int i = 0; i < 12; i++) {
        int per = 3000 + (i * 11);
        int cat = (i % 4) + 1;
        int liv = 500 + (i * 100);
        int nur = 200 + (i * 50);
        int ins = 300 + (i * 80);
        int tot = liv + nur + ins;
        int year = 2023 + (i % 2);
        ds_security(per, cat, liv, nur, ins, tot, year);
    }

    print_str("\nAccessibility construction...\n");
    for (int i = 0; i < 10; i++) {
        int fac = 4000 + (i * 50);
        int type = (i % 4) + 1;
        int cst = 10000 + (i * 5000);
        int cmp = 70 + (i * 3);
        int usr = 50 + (i * 20);
        int year = 2024;
        ds_access(fac, type, cst, cmp, usr, year);
    }

    print_str("\nCultural and sports activities...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 3) + 1;
        int ptc = 30 + (i * 15);
        int vol = 5 + (i * 3);
        int bgt = 5000 + (i * 2000);
        int sat = 75 + (i * 2);
        int year = 2024;
        ds_activity(type, ptc, vol, bgt, sat, year);
    }

    print_str("\nRehab report...\n");
    ds_rehab_report();

    print_str("\nEducation report...\n");
    ds_education_report();

    print_str("\nSecurity report...\n");
    ds_security_report();

    print_str("\nFinal state...\n");
    ds_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
