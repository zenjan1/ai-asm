/* health_admin: Health administration system (v1.0)
 * Public health, medical services, health insurance, personnel, supervision
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

#define MAX_PUBLIC       16
#define MAX_MEDICAL      14
#define MAX_INSURANCE    12
#define MAX_PERSONNEL    10
#define MAX_SUPERVISION  10

typedef struct {
    int    public_id;
    int    region_id;
    int    disease_type;
    int    cases;
    int    vaccinations;
    int    control_score;
    int    year;
    int    active;
} public_health_t;

typedef struct {
    int    medical_id;
    int    hospital_id;
    int    level;
    int    beds;
    int    patients;
    int    bed_occupancy;
    int    year;
    int    active;
} medical_t;

typedef struct {
    int    insurance_id;
    int    region_id;
    int    scheme_type;
    int    enrollees;
    int    claims_paid;
    int    fund_balance;
    int    year;
    int    active;
} insurance_t;

typedef struct {
    int    personnel_id;
    int    worker_id;
    int    profession;
    int    qualification;
    int    experience_years;
    int    training_hours;
    int    year;
    int    active;
} personnel_t;

typedef struct {
    int    supervision_id;
    int    facility_id;
    int    inspection_type;
    int    score;
    int    violations;
    int    rectified;
    int    year;
    int    active;
} supervision_t;

typedef struct {
    int    n_public;
    int    n_medical;
    int    n_insurance;
    int    n_personnel;
    int    n_supervision;
    int    total_cases;
    int    total_vaccinations;
    int    total_beds;
    int    total_patients;
    int    total_enrollees;
} ha_state_t;

static public_health_t public_list[MAX_PUBLIC];
static medical_t medicals[MAX_MEDICAL];
static insurance_t insurances[MAX_INSURANCE];
static personnel_t personnel_list[MAX_PERSONNEL];
static supervision_t supervisions[MAX_SUPERVISION];
static ha_state_t ha;

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

int ha_init(void) {
    if (initialized) return -1;
    ha.n_public = 0; ha.n_medical = 0; ha.n_insurance = 0;
    ha.n_personnel = 0; ha.n_supervision = 0;
    ha.total_cases = 0; ha.total_vaccinations = 0;
    ha.total_beds = 0; ha.total_patients = 0;
    ha.total_enrollees = 0;
    for (int i = 0; i < MAX_PUBLIC; i++) public_list[i].active = 0;
    for (int i = 0; i < MAX_MEDICAL; i++) medicals[i].active = 0;
    for (int i = 0; i < MAX_INSURANCE; i++) insurances[i].active = 0;
    for (int i = 0; i < MAX_PERSONNEL; i++) personnel_list[i].active = 0;
    for (int i = 0; i < MAX_SUPERVISION; i++) supervisions[i].active = 0;
    initialized = 1;
    print_str("[HA] Health admin initialized\n");
    return 0;
}

int ha_public(int region, int disease_type, int cases, int vaccinations, int control_score, int year) {
    if (ha.n_public >= MAX_PUBLIC) return -1;
    public_health_t* p = &public_list[ha.n_public];
    p->public_id = ha.n_public;
    p->region_id = region;
    p->disease_type = disease_type;
    p->cases = cases;
    p->vaccinations = vaccinations;
    p->control_score = control_score;
    p->year = year;
    p->active = 1;
    ha.total_cases += cases;
    ha.total_vaccinations += vaccinations;
    ha.n_public++;
    print_str("[HA] Public "); print_int(ha.n_public - 1);
    print_str(" rgn="); print_int(region);
    print_str(" dis="); print_int(disease_type);
    print_str(" cas="); print_int(cases);
    print_str(" vax="); print_int(vaccinations);
    print_str(" ctl="); print_int(control_score); print_str("\n");
    return ha.n_public - 1;
}

int ha_medical(int hospital, int level, int beds, int patients, int occupancy, int year) {
    if (ha.n_medical >= MAX_MEDICAL) return -1;
    medical_t* m = &medicals[ha.n_medical];
    m->medical_id = ha.n_medical;
    m->hospital_id = hospital;
    m->level = level;
    m->beds = beds;
    m->patients = patients;
    m->bed_occupancy = occupancy;
    m->year = year;
    m->active = 1;
    ha.total_beds += beds;
    ha.total_patients += patients;
    ha.n_medical++;
    print_str("[HA] Medical "); print_int(ha.n_medical - 1);
    print_str(" hsp="); print_int(hospital);
    print_str(" lvl="); print_int(level);
    print_str(" bed="); print_int(beds);
    print_str(" pt="); print_int(patients);
    print_str(" occ="); print_int(occupancy); print_str("%\n");
    return ha.n_medical - 1;
}

int ha_insurance(int region, int scheme_type, int enrollees, int claims, int fund_balance, int year) {
    if (ha.n_insurance >= MAX_INSURANCE) return -1;
    insurance_t* ins = &insurances[ha.n_insurance];
    ins->insurance_id = ha.n_insurance;
    ins->region_id = region;
    ins->scheme_type = scheme_type;
    ins->enrollees = enrollees;
    ins->claims_paid = claims;
    ins->fund_balance = fund_balance;
    ins->year = year;
    ins->active = 1;
    ha.total_enrollees += enrollees;
    ha.n_insurance++;
    print_str("[HA] Insurance "); print_int(ha.n_insurance - 1);
    print_str(" rgn="); print_int(region);
    print_str(" sch="); print_int(scheme_type);
    print_str(" enrl="); print_int(enrollees);
    print_str(" clm=$"); print_int(claims);
    print_str(" bal=$"); print_int(fund_balance); print_str("\n");
    return ha.n_insurance - 1;
}

int ha_personnel(int worker, int profession, int qualification, int exp_years, int training_hours, int year) {
    if (ha.n_personnel >= MAX_PERSONNEL) return -1;
    personnel_t* p = &personnel_list[ha.n_personnel];
    p->personnel_id = ha.n_personnel;
    p->worker_id = worker;
    p->profession = profession;
    p->qualification = qualification;
    p->experience_years = exp_years;
    p->training_hours = training_hours;
    p->year = year;
    p->active = 1;
    ha.n_personnel++;
    print_str("[HA] Personnel "); print_int(ha.n_personnel - 1);
    print_str(" wrk="); print_int(worker);
    print_str(" prf="); print_int(profession);
    print_str(" qual="); print_int(qualification);
    print_str(" exp="); print_int(exp_years); print_str("y");
    print_str(" trn="); print_int(training_hours); print_str("h\n");
    return ha.n_personnel - 1;
}

int ha_supervision(int facility, int inspection_type, int score, int violations, int rectified, int year) {
    if (ha.n_supervision >= MAX_SUPERVISION) return -1;
    supervision_t* s = &supervisions[ha.n_supervision];
    s->supervision_id = ha.n_supervision;
    s->facility_id = facility;
    s->inspection_type = inspection_type;
    s->score = score;
    s->violations = violations;
    s->rectified = rectified;
    s->year = year;
    s->active = 1;
    ha.n_supervision++;
    print_str("[HA] Supervision "); print_int(ha.n_supervision - 1);
    print_str(" fac="); print_int(facility);
    print_str(" type="); print_int(inspection_type);
    print_str(" scr="); print_int(score);
    print_str(" vio="); print_int(violations);
    print_str(" rct="); print_int(rectified); print_str("\n");
    return ha.n_supervision - 1;
}

void ha_public_report(void) {
    print_str("[HA] Public health report:\n");
    print_str("  Regions monitored: "); print_int(ha.n_public); print_str("\n");
    print_str("  Total cases: "); print_int(ha.total_cases); print_str("\n");
    print_str("  Total vaccinations: "); print_int(ha.total_vaccinations); print_str("\n");
}

void ha_medical_report(void) {
    print_str("[HA] Medical report:\n");
    print_str("  Hospitals: "); print_int(ha.n_medical); print_str("\n");
    print_str("  Total beds: "); print_int(ha.total_beds); print_str("\n");
    print_str("  Total patients: "); print_int(ha.total_patients); print_str("\n");
}

void ha_supervision_report(void) {
    print_str("[HA] Supervision report:\n");
    print_str("  Insurance schemes: "); print_int(ha.n_insurance); print_str("\n");
    print_str("  Total enrollees: "); print_int(ha.total_enrollees); print_str("\n");
    print_str("  Health personnel: "); print_int(ha.n_personnel); print_str("\n");
    print_str("  Inspections: "); print_int(ha.n_supervision); print_str("\n");
}

void ha_print_state(void) {
    print_str("[HA] Pb="); print_int(ha.n_public);
    print_str(" Md="); print_int(ha.n_medical);
    print_str(" In="); print_int(ha.n_insurance);
    print_str(" Ps="); print_int(ha.n_personnel);
    print_str(" Sp="); print_int(ha.n_supervision);
    print_str("\n");
}

int main(void) {
    print_str("=== Health Admin Demo ===\n\n");
    ha_init();

    print_str("Public health...\n");
    for (int i = 0; i < 16; i++) {
        int rgn = (i % 8) + 1;
        int dis = (i % 5) + 1;
        int cas = 100 + (i * 30);
        int vax = cas * 10;
        int ctl = 70 + (i % 25);
        int year = 2020 + (i % 5);
        ha_public(rgn, dis, cas, vax, ctl, year);
    }

    print_str("\nMedical services...\n");
    for (int i = 0; i < 14; i++) {
        int hsp = 100 + (i * 10);
        int lvl = (i % 4) + 1;
        int bed = 200 + (i * 50);
        int pt = bed - (i * 10);
        int occ = 70 + (i % 25);
        int year = 2021 + (i % 4);
        ha_medical(hsp, lvl, bed, pt, occ, year);
    }

    print_str("\nHealth insurance...\n");
    for (int i = 0; i < 12; i++) {
        int rgn = (i % 8) + 1;
        int sch = (i % 3) + 1;
        int enrl = 50000 + (i * 10000);
        int clm = enrl * 100;
        int bal = clm / 5;
        int year = 2022 + (i % 3);
        ha_insurance(rgn, sch, enrl, clm, bal, year);
    }

    print_str("\nHealth personnel...\n");
    for (int i = 0; i < 10; i++) {
        int wrk = 200 + (i * 15);
        int prf = (i % 4) + 1;
        int qual = (i % 3) + 1;
        int exp = 3 + (i * 2);
        int trn = 40 + (i * 10);
        int year = 2023 + (i % 2);
        ha_personnel(wrk, prf, qual, exp, trn, year);
    }

    print_str("\nHealth supervision...\n");
    for (int i = 0; i < 10; i++) {
        int fac = 300 + (i * 12);
        int type = (i % 3) + 1;
        int scr = 70 + (i * 3);
        int vio = 10 - (i % 8);
        int rct = vio - (i % 3);
        int year = 2024;
        ha_supervision(fac, type, scr, vio, rct, year);
    }

    print_str("\nPublic health report...\n");
    ha_public_report();

    print_str("\nMedical report...\n");
    ha_medical_report();

    print_str("\nSupervision report...\n");
    ha_supervision_report();

    print_str("\nFinal state...\n");
    ha_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
