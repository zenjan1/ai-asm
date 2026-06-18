/* religion_admin: Religious affairs administration system (v1.0)
 * Religious venues, clergy, activities, education, international exchange
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

#define MAX_VENUE        16
#define MAX_CLERGY       14
#define MAX_ACTIVITY     12
#define MAX_EDUCATION    10
#define MAX_FOREIGN      10

typedef struct {
    int    venue_id;
    int    religion_type;
    int    venue_type;
    int    registered_members;
    int    annual_visitors;
    int    staff_count;
    int    year;
    int    active;
} venue_t;

typedef struct {
    int    clergy_id;
    int    religion_type;
    int    clergy_rank;
    int    ordination_year;
    int    congregation_size;
    int    certification;
    int    year;
    int    active;
} clergy_t;

typedef struct {
    int    activity_id;
    int    venue_id;
    int    activity_type;
    int    participants;
    int    duration_days;
    int    approval_status;
    int    year;
    int    active;
} activity_t;

typedef struct {
    int    education_id;
    int    institution_type;
    int    religion_type;
    int    students_enrolled;
    int    graduates;
    int    faculty;
    int    year;
    int    active;
} education_t;

typedef struct {
    int    foreign_id;
    int    country_id;
    int    exchange_type;
    int    delegates;
    int    agreements_signed;
    int    pilgrims;
    int    year;
    int    active;
} foreign_t;

typedef struct {
    int    n_venue;
    int    n_clergy;
    int    n_activity;
    int    n_education;
    int    n_foreign;
    int    total_members;
    int    total_visitors;
    int    total_participants;
    int    total_students;
    int    total_pilgrims;
} ra_state_t;

static venue_t venues[MAX_VENUE];
static clergy_t clergy[MAX_CLERGY];
static activity_t activities[MAX_ACTIVITY];
static education_t educations[MAX_EDUCATION];
static foreign_t foreigns[MAX_FOREIGN];
static ra_state_t ra;

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

int ra_init(void) {
    if (initialized) return -1;
    ra.n_venue = 0; ra.n_clergy = 0; ra.n_activity = 0;
    ra.n_education = 0; ra.n_foreign = 0;
    ra.total_members = 0; ra.total_visitors = 0;
    ra.total_participants = 0; ra.total_students = 0;
    ra.total_pilgrims = 0;
    for (int i = 0; i < MAX_VENUE; i++) venues[i].active = 0;
    for (int i = 0; i < MAX_CLERGY; i++) clergy[i].active = 0;
    for (int i = 0; i < MAX_ACTIVITY; i++) activities[i].active = 0;
    for (int i = 0; i < MAX_EDUCATION; i++) educations[i].active = 0;
    for (int i = 0; i < MAX_FOREIGN; i++) foreigns[i].active = 0;
    initialized = 1;
    print_str("[RA] Religion admin initialized\n");
    return 0;
}

int ra_venue(int religion, int venue_type, int members, int visitors, int staff, int year) {
    if (ra.n_venue >= MAX_VENUE) return -1;
    venue_t* v = &venues[ra.n_venue];
    v->venue_id = ra.n_venue;
    v->religion_type = religion;
    v->venue_type = venue_type;
    v->registered_members = members;
    v->annual_visitors = visitors;
    v->staff_count = staff;
    v->year = year;
    v->active = 1;
    ra.total_members += members;
    ra.total_visitors += visitors;
    ra.n_venue++;
    print_str("[RA] Venue "); print_int(ra.n_venue - 1);
    print_str(" rel="); print_int(religion);
    print_str(" type="); print_int(venue_type);
    print_str(" mem="); print_int(members);
    print_str(" vis="); print_int(visitors);
    print_str(" stf="); print_int(staff); print_str("\n");
    return ra.n_venue - 1;
}

int ra_clergy(int religion, int rank, int ordination, int congregation, int cert, int year) {
    if (ra.n_clergy >= MAX_CLERGY) return -1;
    clergy_t* c = &clergy[ra.n_clergy];
    c->clergy_id = ra.n_clergy;
    c->religion_type = religion;
    c->clergy_rank = rank;
    c->ordination_year = ordination;
    c->congregation_size = congregation;
    c->certification = cert;
    c->year = year;
    c->active = 1;
    ra.n_clergy++;
    print_str("[RA] Clergy "); print_int(ra.n_clergy - 1);
    print_str(" rel="); print_int(religion);
    print_str(" rank="); print_int(rank);
    print_str(" ord="); print_int(ordination);
    print_str(" cong="); print_int(congregation);
    print_str(" cert="); print_int(cert); print_str("\n");
    return ra.n_clergy - 1;
}

int ra_activity(int venue, int act_type, int participants, int duration, int approval, int year) {
    if (ra.n_activity >= MAX_ACTIVITY) return -1;
    activity_t* a = &activities[ra.n_activity];
    a->activity_id = ra.n_activity;
    a->venue_id = venue;
    a->activity_type = act_type;
    a->participants = participants;
    a->duration_days = duration;
    a->approval_status = approval;
    a->year = year;
    a->active = 1;
    ra.total_participants += participants;
    ra.n_activity++;
    print_str("[RA] Activity "); print_int(ra.n_activity - 1);
    print_str(" vnu="); print_int(venue);
    print_str(" type="); print_int(act_type);
    print_str(" par="); print_int(participants);
    print_str(" dur="); print_int(duration); print_str("d");
    print_str(" app="); print_int(approval); print_str("\n");
    return ra.n_activity - 1;
}

int ra_education(int inst_type, int religion, int students, int graduates, int faculty, int year) {
    if (ra.n_education >= MAX_EDUCATION) return -1;
    education_t* e = &educations[ra.n_education];
    e->education_id = ra.n_education;
    e->institution_type = inst_type;
    e->religion_type = religion;
    e->students_enrolled = students;
    e->graduates = graduates;
    e->faculty = faculty;
    e->year = year;
    e->active = 1;
    ra.total_students += students;
    ra.n_education++;
    print_str("[RA] Education "); print_int(ra.n_education - 1);
    print_str(" inst="); print_int(inst_type);
    print_str(" rel="); print_int(religion);
    print_str(" stu="); print_int(students);
    print_str(" grd="); print_int(graduates);
    print_str(" fac="); print_int(faculty); print_str("\n");
    return ra.n_education - 1;
}

int ra_foreign(int country, int exchange_type, int delegates, int agreements, int pilgrims, int year) {
    if (ra.n_foreign >= MAX_FOREIGN) return -1;
    foreign_t* f = &foreigns[ra.n_foreign];
    f->foreign_id = ra.n_foreign;
    f->country_id = country;
    f->exchange_type = exchange_type;
    f->delegates = delegates;
    f->agreements_signed = agreements;
    f->pilgrims = pilgrims;
    f->year = year;
    f->active = 1;
    ra.total_pilgrims += pilgrims;
    ra.n_foreign++;
    print_str("[RA] Foreign "); print_int(ra.n_foreign - 1);
    print_str(" cty="); print_int(country);
    print_str(" type="); print_int(exchange_type);
    print_str(" del="); print_int(delegates);
    print_str(" agr="); print_int(agreements);
    print_str(" pil="); print_int(pilgrims); print_str("\n");
    return ra.n_foreign - 1;
}

void ra_venue_report(void) {
    print_str("[RA] Venue report:\n");
    print_str("  Religious venues: "); print_int(ra.n_venue); print_str("\n");
    print_str("  Total registered members: "); print_int(ra.total_members); print_str("\n");
    print_str("  Total annual visitors: "); print_int(ra.total_visitors); print_str("\n");
}

void ra_clergy_report(void) {
    print_str("[RA] Clergy report:\n");
    print_str("  Certified clergy: "); print_int(ra.n_clergy); print_str("\n");
    print_str("  Religious activities: "); print_int(ra.n_activity); print_str("\n");
    print_str("  Total participants: "); print_int(ra.total_participants); print_str("\n");
}

void ra_education_report(void) {
    print_str("[RA] Education report:\n");
    print_str("  Religious schools: "); print_int(ra.n_education); print_str("\n");
    print_str("  Total students: "); print_int(ra.total_students); print_str("\n");
    print_str("  International exchanges: "); print_int(ra.n_foreign); print_str("\n");
    print_str("  Total pilgrims: "); print_int(ra.total_pilgrims); print_str("\n");
}

void ra_print_state(void) {
    print_str("[RA] Vn="); print_int(ra.n_venue);
    print_str(" Cl="); print_int(ra.n_clergy);
    print_str(" Ac="); print_int(ra.n_activity);
    print_str(" Ed="); print_int(ra.n_education);
    print_str(" Fr="); print_int(ra.n_foreign);
    print_str("\n");
}

int main(void) {
    print_str("=== Religion Admin Demo ===\n\n");
    ra_init();

    print_str("Religious venues...\n");
    for (int i = 0; i < 16; i++) {
        int rel = (i % 5) + 1;
        int type = (i % 4) + 1;
        int mem = 500 + (i * 200);
        int vis = 10000 + (i * 5000);
        int stf = 10 + (i * 3);
        int year = 2020 + (i % 5);
        ra_venue(rel, type, mem, vis, stf, year);
    }

    print_str("\nClergy registration...\n");
    for (int i = 0; i < 14; i++) {
        int rel = (i % 5) + 1;
        int rank = (i % 4) + 1;
        int ord = 2000 + (i * 2);
        int cong = 200 + (i * 50);
        int cert = (i % 3 == 0) ? 0 : 1;
        int year = 2021 + (i % 4);
        ra_clergy(rel, rank, ord, cong, cert, year);
    }

    print_str("\nReligious activities...\n");
    for (int i = 0; i < 12; i++) {
        int vnu = i % 16;
        int type = (i % 6) + 1;
        int par = 100 + (i * 50);
        int dur = 1 + (i % 7);
        int app = (i % 4 == 0) ? 0 : 1;
        int year = 2022 + (i % 3);
        ra_activity(vnu, type, par, dur, app, year);
    }

    print_str("\nReligious education...\n");
    for (int i = 0; i < 10; i++) {
        int inst = (i % 3) + 1;
        int rel = (i % 5) + 1;
        int stu = 50 + (i * 20);
        int grd = 10 + (i * 5);
        int fac = 5 + (i * 2);
        int year = 2023 + (i % 2);
        ra_education(inst, rel, stu, grd, fac, year);
    }

    print_str("\nInternational exchange...\n");
    for (int i = 0; i < 10; i++) {
        int cty = 100 + (i * 11);
        int type = (i % 4) + 1;
        int del = 10 + (i * 5);
        int agr = 1 + (i % 3);
        int pil = 50 + (i * 30);
        int year = 2024;
        ra_foreign(cty, type, del, agr, pil, year);
    }

    print_str("\nVenue report...\n");
    ra_venue_report();

    print_str("\nClergy report...\n");
    ra_clergy_report();

    print_str("\nEducation report...\n");
    ra_education_report();

    print_str("\nFinal state...\n");
    ra_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
