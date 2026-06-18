/* employment_service: Employment service management (v1.0)
 * Job seekers, recruitment, training, employment aid, statistics
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

#define MAX_SEEKERS      18
#define MAX_RECRUIT      14
#define MAX_TRAINING     12
#define MAX_AID          10
#define MAX_STATS        10

typedef struct {
    int    seeker_id;
    int    age;
    int    education;
    int    skills;
    int    experience_years;
    int    expected_salary;
    int    registered_year;
    int    status;
    int    active;
} seeker_t;

typedef struct {
    int    recruit_id;
    int    employer_id;
    int    position;
    int    vacancies;
    int    salary;
    int    applicants;
    int    hired;
    int    year;
    int    active;
} recruitment_t;

typedef struct {
    int    train_id;
    int    type;
    int    participants;
    int    duration_hours;
    int    completed;
    int    certified;
    int    year;
    int    status;
    int    active;
} training_t;

typedef struct {
    int    aid_id;
    int    recipient_id;
    int    type;
    int    duration_months;
    int    amount;
    int    outcome;
    int    year;
    int    status;
    int    active;
} aid_t;

typedef struct {
    int    stat_id;
    int    region_id;
    int    jobseekers;
    int    employed;
    int    unemployed;
    int    training_enrolled;
    int    aid_recipients;
    int    year;
    int    active;
} stat_t;

typedef struct {
    int    n_seekers;
    int    n_recruit;
    int    n_training;
    int    n_aid;
    int    n_stats;
    int    total_hired;
    int    total_completed;
    int    total_certified;
    int    total_aid_amount;
    int    total_employed;
} es_state_t;

static seeker_t seekers[MAX_SEEKERS];
static recruitment_t recruitments[MAX_RECRUIT];
static training_t trainings[MAX_TRAINING];
static aid_t aids[MAX_AID];
static stat_t stats[MAX_STATS];
static es_state_t es;

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

int es_init(void) {
    if (initialized) return -1;
    es.n_seekers = 0; es.n_recruit = 0; es.n_training = 0;
    es.n_aid = 0; es.n_stats = 0;
    es.total_hired = 0; es.total_completed = 0;
    es.total_certified = 0; es.total_aid_amount = 0;
    es.total_employed = 0;
    for (int i = 0; i < MAX_SEEKERS; i++) seekers[i].active = 0;
    for (int i = 0; i < MAX_RECRUIT; i++) recruitments[i].active = 0;
    for (int i = 0; i < MAX_TRAINING; i++) trainings[i].active = 0;
    for (int i = 0; i < MAX_AID; i++) aids[i].active = 0;
    for (int i = 0; i < MAX_STATS; i++) stats[i].active = 0;
    initialized = 1;
    print_str("[ES] Employment service initialized\n");
    return 0;
}

int es_register_seeker(int age, int education, int skills, int experience, int salary, int year) {
    if (es.n_seekers >= MAX_SEEKERS) return -1;
    seeker_t* s = &seekers[es.n_seekers];
    s->seeker_id = es.n_seekers;
    s->age = age;
    s->education = education;
    s->skills = skills;
    s->experience_years = experience;
    s->expected_salary = salary;
    s->registered_year = year;
    s->status = 1;
    s->active = 1;
    es.n_seekers++;
    print_str("[ES] Seeker "); print_int(es.n_seekers - 1);
    print_str(" age="); print_int(age);
    print_str(" edu="); print_int(education);
    print_str(" skl="); print_int(skills);
    print_str(" exp="); print_int(experience); print_str("yr");
    print_str(" sal=$"); print_int(salary); print_str("\n");
    return es.n_seekers - 1;
}

int es_post_job(int employer, int position, int vacancies, int salary, int applicants, int hired, int year) {
    if (es.n_recruit >= MAX_RECRUIT) return -1;
    recruitment_t* r = &recruitments[es.n_recruit];
    r->recruit_id = es.n_recruit;
    r->employer_id = employer;
    r->position = position;
    r->vacancies = vacancies;
    r->salary = salary;
    r->applicants = applicants;
    r->hired = hired;
    r->year = year;
    r->active = 1;
    es.total_hired += hired;
    es.n_recruit++;
    print_str("[ES] Job "); print_int(es.n_recruit - 1);
    print_str(" er="); print_int(employer);
    print_str(" pos="); print_int(position);
    print_str(" vac="); print_int(vacancies);
    print_str(" sal=$"); print_int(salary);
    print_str(" app="); print_int(applicants);
    print_str(" hrd="); print_int(hired); print_str("\n");
    return es.n_recruit - 1;
}

int es_add_training(int type, int participants, int duration, int completed, int certified, int year) {
    if (es.n_training >= MAX_TRAINING) return -1;
    training_t* t = &trainings[es.n_training];
    t->train_id = es.n_training;
    t->type = type;
    t->participants = participants;
    t->duration_hours = duration;
    t->completed = completed;
    t->certified = certified;
    t->year = year;
    t->status = 1;
    t->active = 1;
    es.total_completed += completed;
    es.total_certified += certified;
    es.n_training++;
    print_str("[ES] Training "); print_int(es.n_training - 1);
    print_str(" type="); print_int(type);
    print_str(" ptc="); print_int(participants);
    print_str(" dur="); print_int(duration); print_str("h");
    print_str(" cmp="); print_int(completed);
    print_str(" cert="); print_int(certified); print_str("\n");
    return es.n_training - 1;
}

int es_grant_aid(int recipient, int type, int duration, int amount, int outcome, int year) {
    if (es.n_aid >= MAX_AID) return -1;
    aid_t* a = &aids[es.n_aid];
    a->aid_id = es.n_aid;
    a->recipient_id = recipient;
    a->type = type;
    a->duration_months = duration;
    a->amount = amount;
    a->outcome = outcome;
    a->year = year;
    a->status = 1;
    a->active = 1;
    es.total_aid_amount += amount;
    es.n_aid++;
    print_str("[ES] Aid "); print_int(es.n_aid - 1);
    print_str(" rec="); print_int(recipient);
    print_str(" type="); print_int(type);
    print_str(" dur="); print_int(duration); print_str("mo");
    print_str(" amt=$"); print_int(amount);
    print_str(" out="); print_int(outcome); print_str("\n");
    return es.n_aid - 1;
}

int es_record_stat(int region, int jobseekers, int employed, int unemployed, int training, int aid, int year) {
    if (es.n_stats >= MAX_STATS) return -1;
    stat_t* s = &stats[es.n_stats];
    s->stat_id = es.n_stats;
    s->region_id = region;
    s->jobseekers = jobseekers;
    s->employed = employed;
    s->unemployed = unemployed;
    s->training_enrolled = training;
    s->aid_recipients = aid;
    s->year = year;
    s->active = 1;
    es.total_employed += employed;
    es.n_stats++;
    print_str("[ES] Stat "); print_int(es.n_stats - 1);
    print_str(" reg="); print_int(region);
    print_str(" jbs="); print_int(jobseekers);
    print_str(" emp="); print_int(employed);
    print_str(" unemp="); print_int(unemployed);
    print_str(" yr="); print_int(year); print_str("\n");
    return es.n_stats - 1;
}

void es_recruitment_report(void) {
    print_str("[ES] Recruitment report:\n");
    print_str("  Job seekers: "); print_int(es.n_seekers); print_str("\n");
    print_str("  Recruitments: "); print_int(es.n_recruit); print_str("\n");
    print_str("  Total hired: "); print_int(es.total_hired); print_str("\n");
}

void es_training_report(void) {
    print_str("[ES] Training report:\n");
    print_str("  Programs: "); print_int(es.n_training); print_str("\n");
    print_str("  Completed: "); print_int(es.total_completed); print_str("\n");
    print_str("  Certified: "); print_int(es.total_certified); print_str("\n");
}

void es_aid_report(void) {
    print_str("[ES] Aid report:\n");
    print_str("  Aid cases: "); print_int(es.n_aid); print_str("\n");
    print_str("  Total amount: $"); print_int(es.total_aid_amount); print_str("\n");
    print_str("  Total employed: "); print_int(es.total_employed); print_str("\n");
}

void es_print_state(void) {
    print_str("[ES] Sk="); print_int(es.n_seekers);
    print_str(" Rc="); print_int(es.n_recruit);
    print_str(" Tr="); print_int(es.n_training);
    print_str(" Ad="); print_int(es.n_aid);
    print_str(" St="); print_int(es.n_stats);
    print_str("\n");
}

int main(void) {
    print_str("=== Employment Service Demo ===\n\n");
    es_init();

    print_str("Registering job seekers...\n");
    for (int i = 0; i < 18; i++) {
        int age = 20 + (i % 40);
        int edu = (i % 5) + 1;
        int skills = (i % 8) + 1;
        int exp = i % 15;
        int sal = 3000 + (i * 300);
        int year = 2020 + (i % 5);
        es_register_seeker(age, edu, skills, exp, sal, year);
    }

    print_str("\nPosting jobs...\n");
    for (int i = 0; i < 14; i++) {
        int er = 6000 + (i * 10);
        int pos = (i % 10) + 1;
        int vac = 1 + (i % 5);
        int sal = 4000 + (i * 400);
        int app = 5 + (i * 3);
        int hrd = 1 + (i % 3);
        int year = 2021 + (i % 4);
        es_post_job(er, pos, vac, sal, app, hrd, year);
    }

    print_str("\nRunning training programs...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int ptc = 10 + (i * 5);
        int dur = 20 + (i * 10);
        int cmp = 8 + (i * 4);
        int cert = 5 + (i * 3);
        int year = 2022 + (i % 3);
        es_add_training(type, ptc, dur, cmp, cert, year);
    }

    print_str("\nGranting employment aid...\n");
    for (int i = 0; i < 10; i++) {
        int rec = 7000 + (i * 7);
        int type = (i % 3) + 1;
        int dur = 3 + (i % 6);
        int amt = 2000 + (i * 500);
        int out = (i % 3) + 1;
        int year = 2023 + (i % 2);
        es_grant_aid(rec, type, dur, amt, out, year);
    }

    print_str("\nRecording statistics...\n");
    for (int i = 0; i < 10; i++) {
        int reg = (i % 5) + 1;
        int jbs = 500 + (i * 100);
        int emp = 400 + (i * 80);
        int unemp = 100 + (i * 20);
        int trn = 50 + (i * 15);
        int aid_r = 30 + (i * 10);
        int year = 2020 + (i % 4);
        es_record_stat(reg, jbs, emp, unemp, trn, aid_r, year);
    }

    print_str("\nRecruitment report...\n");
    es_recruitment_report();

    print_str("\nTraining report...\n");
    es_training_report();

    print_str("\nAid report...\n");
    es_aid_report();

    print_str("\nFinal state...\n");
    es_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
