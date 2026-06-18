/* civil_admin: Civil affairs administration system (v1.0)
 * Social assistance, marriage, funeral, charity, community governance
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

#define MAX_ASSISTANCE   16
#define MAX_MARRIAGE     14
#define MAX_FUNERAL      12
#define MAX_CHARITY      10
#define MAX_COMMUNITY    10

typedef struct {
    int    assistance_id;
    int    beneficiary_id;
    int    assistance_type;
    int    amount;
    int    duration_months;
    int    family_size;
    int    year;
    int    active;
} assistance_t;

typedef struct {
    int    marriage_id;
    int    couple_id;
    int    marriage_type;
    int    region_id;
    int    age_groom;
    int    age_bride;
    int    year;
    int    active;
} marriage_t;

typedef struct {
    int    funeral_id;
    int    deceased_id;
    int    service_type;
    int    burial_type;
    int    cost;
    int    ceremony_size;
    int    year;
    int    active;
} funeral_t;

typedef struct {
    int    charity_id;
    int    organization_id;
    int    charity_type;
    int    donations;
    int    volunteers;
    int    beneficiaries;
    int    year;
    int    active;
} charity_t;

typedef struct {
    int    community_id;
    int    region_id;
    int    governance_type;
    int    residents;
    int    services;
    int    satisfaction;
    int    year;
    int    active;
} community_t;

typedef struct {
    int    n_assistance;
    int    n_marriage;
    int    n_funeral;
    int    n_charity;
    int    n_community;
    int    total_assistance_amount;
    int    total_beneficiaries;
    int    total_donations;
    int    total_volunteers;
    int    total_residents;
} cvl_state_t;

static assistance_t assistances[MAX_ASSISTANCE];
static marriage_t marriages[MAX_MARRIAGE];
static funeral_t funerals[MAX_FUNERAL];
static charity_t charities[MAX_CHARITY];
static community_t communities[MAX_COMMUNITY];
static cvl_state_t cvl;

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

int cvl_init(void) {
    if (initialized) return -1;
    cvl.n_assistance = 0; cvl.n_marriage = 0; cvl.n_funeral = 0;
    cvl.n_charity = 0; cvl.n_community = 0;
    cvl.total_assistance_amount = 0; cvl.total_beneficiaries = 0;
    cvl.total_donations = 0; cvl.total_volunteers = 0;
    cvl.total_residents = 0;
    for (int i = 0; i < MAX_ASSISTANCE; i++) assistances[i].active = 0;
    for (int i = 0; i < MAX_MARRIAGE; i++) marriages[i].active = 0;
    for (int i = 0; i < MAX_FUNERAL; i++) funerals[i].active = 0;
    for (int i = 0; i < MAX_CHARITY; i++) charities[i].active = 0;
    for (int i = 0; i < MAX_COMMUNITY; i++) communities[i].active = 0;
    initialized = 1;
    print_str("[CVL] Civil admin initialized\n");
    return 0;
}

int cvl_assistance(int beneficiary, int assistance_type, int amount, int duration, int family_size, int year) {
    if (cvl.n_assistance >= MAX_ASSISTANCE) return -1;
    assistance_t* a = &assistances[cvl.n_assistance];
    a->assistance_id = cvl.n_assistance;
    a->beneficiary_id = beneficiary;
    a->assistance_type = assistance_type;
    a->amount = amount;
    a->duration_months = duration;
    a->family_size = family_size;
    a->year = year;
    a->active = 1;
    cvl.total_assistance_amount += amount * duration;
    cvl.total_beneficiaries += family_size;
    cvl.n_assistance++;
    print_str("[CVL] Assistance "); print_int(cvl.n_assistance - 1);
    print_str(" ben="); print_int(beneficiary);
    print_str(" type="); print_int(assistance_type);
    print_str(" amt=$"); print_int(amount);
    print_str(" dur="); print_int(duration); print_str("mo");
    print_str(" fm="); print_int(family_size); print_str("\n");
    return cvl.n_assistance - 1;
}

int cvl_marriage(int couple, int marriage_type, int region, int age_groom, int age_bride, int year) {
    if (cvl.n_marriage >= MAX_MARRIAGE) return -1;
    marriage_t* m = &marriages[cvl.n_marriage];
    m->marriage_id = cvl.n_marriage;
    m->couple_id = couple;
    m->marriage_type = marriage_type;
    m->region_id = region;
    m->age_groom = age_groom;
    m->age_bride = age_bride;
    m->year = year;
    m->active = 1;
    cvl.n_marriage++;
    print_str("[CVL] Marriage "); print_int(cvl.n_marriage - 1);
    print_str(" cup="); print_int(couple);
    print_str(" type="); print_int(marriage_type);
    print_str(" rgn="); print_int(region);
    print_str(" gAge="); print_int(age_groom);
    print_str(" bAge="); print_int(age_bride); print_str("\n");
    return cvl.n_marriage - 1;
}

int cvl_funeral(int deceased, int service_type, int burial_type, int cost, int ceremony_size, int year) {
    if (cvl.n_funeral >= MAX_FUNERAL) return -1;
    funeral_t* f = &funerals[cvl.n_funeral];
    f->funeral_id = cvl.n_funeral;
    f->deceased_id = deceased;
    f->service_type = service_type;
    f->burial_type = burial_type;
    f->cost = cost;
    f->ceremony_size = ceremony_size;
    f->year = year;
    f->active = 1;
    cvl.n_funeral++;
    print_str("[CVL] Funeral "); print_int(cvl.n_funeral - 1);
    print_str(" dec="); print_int(deceased);
    print_str(" svc="); print_int(service_type);
    print_str(" brl="); print_int(burial_type);
    print_str(" cst=$"); print_int(cost);
    print_str(" crm="); print_int(ceremony_size); print_str("\n");
    return cvl.n_funeral - 1;
}

int cvl_charity(int organization, int charity_type, int donations, int volunteers, int beneficiaries, int year) {
    if (cvl.n_charity >= MAX_CHARITY) return -1;
    charity_t* c = &charities[cvl.n_charity];
    c->charity_id = cvl.n_charity;
    c->organization_id = organization;
    c->charity_type = charity_type;
    c->donations = donations;
    c->volunteers = volunteers;
    c->beneficiaries = beneficiaries;
    c->year = year;
    c->active = 1;
    cvl.total_donations += donations;
    cvl.total_volunteers += volunteers;
    cvl.n_charity++;
    print_str("[CVL] Charity "); print_int(cvl.n_charity - 1);
    print_str(" org="); print_int(organization);
    print_str(" type="); print_int(charity_type);
    print_str(" dnt=$"); print_int(donations);
    print_str(" vlt="); print_int(volunteers);
    print_str(" ben="); print_int(beneficiaries); print_str("\n");
    return cvl.n_charity - 1;
}

int cvl_community(int region, int governance_type, int residents, int services, int satisfaction, int year) {
    if (cvl.n_community >= MAX_COMMUNITY) return -1;
    community_t* cm = &communities[cvl.n_community];
    cm->community_id = cvl.n_community;
    cm->region_id = region;
    cm->governance_type = governance_type;
    cm->residents = residents;
    cm->services = services;
    cm->satisfaction = satisfaction;
    cm->year = year;
    cm->active = 1;
    cvl.total_residents += residents;
    cvl.n_community++;
    print_str("[CVL] Community "); print_int(cvl.n_community - 1);
    print_str(" rgn="); print_int(region);
    print_str(" type="); print_int(governance_type);
    print_str(" res="); print_int(residents);
    print_str(" svc="); print_int(services);
    print_str(" sat="); print_int(satisfaction); print_str("\n");
    return cvl.n_community - 1;
}

void cvl_assistance_report(void) {
    print_str("[CVL] Assistance report:\n");
    print_str("  Assistance cases: "); print_int(cvl.n_assistance); print_str("\n");
    print_str("  Total amount: $"); print_int(cvl.total_assistance_amount); print_str("\n");
    print_str("  Total beneficiaries: "); print_int(cvl.total_beneficiaries); print_str("\n");
}

void cvl_marriage_report(void) {
    print_str("[CVL] Marriage report:\n");
    print_str("  Marriages registered: "); print_int(cvl.n_marriage); print_str("\n");
    print_str("  Funeral services: "); print_int(cvl.n_funeral); print_str("\n");
}

void cvl_charity_report(void) {
    print_str("[CVL] Charity report:\n");
    print_str("  Charity organizations: "); print_int(cvl.n_charity); print_str("\n");
    print_str("  Total donations: $"); print_int(cvl.total_donations); print_str("\n");
    print_str("  Total volunteers: "); print_int(cvl.total_volunteers); print_str("\n");
    print_str("  Communities: "); print_int(cvl.n_community); print_str("\n");
    print_str("  Total residents: "); print_int(cvl.total_residents); print_str("\n");
}

void cvl_print_state(void) {
    print_str("[CVL] As="); print_int(cvl.n_assistance);
    print_str(" Mr="); print_int(cvl.n_marriage);
    print_str(" Fn="); print_int(cvl.n_funeral);
    print_str(" Ch="); print_int(cvl.n_charity);
    print_str(" Cm="); print_int(cvl.n_community);
    print_str("\n");
}

int main(void) {
    print_str("=== Civil Admin Demo ===\n\n");
    cvl_init();

    print_str("Social assistance...\n");
    for (int i = 0; i < 16; i++) {
        int ben = 1000 + (i * 11);
        int type = (i % 4) + 1;
        int amt = 500 + (i * 100);
        int dur = 6 + (i % 7);
        int fm = 2 + (i % 5);
        int year = 2020 + (i % 5);
        cvl_assistance(ben, type, amt, dur, fm, year);
    }

    print_str("\nMarriage registration...\n");
    for (int i = 0; i < 14; i++) {
        int cup = 2000 + (i * 7);
        int type = (i % 3) + 1;
        int rgn = (i % 8) + 1;
        int ag = 25 + (i % 10);
        int ab = 23 + (i % 10);
        int year = 2021 + (i % 4);
        cvl_marriage(cup, type, rgn, ag, ab, year);
    }

    print_str("\nFuneral services...\n");
    for (int i = 0; i < 12; i++) {
        int dec = 3000 + (i * 13);
        int svc = (i % 3) + 1;
        int brl = (i % 4) + 1;
        int cst = 5000 + (i * 2000);
        int crm = 30 + (i * 10);
        int year = 2022 + (i % 3);
        cvl_funeral(dec, svc, brl, cst, crm, year);
    }

    print_str("\nCharity organizations...\n");
    for (int i = 0; i < 10; i++) {
        int org = 4000 + (i * 17);
        int type = (i % 4) + 1;
        int dnt = 100000 + (i * 50000);
        int vlt = 20 + (i * 10);
        int ben = 500 + (i * 200);
        int year = 2023 + (i % 2);
        cvl_charity(org, type, dnt, vlt, ben, year);
    }

    print_str("\nCommunity governance...\n");
    for (int i = 0; i < 10; i++) {
        int rgn = (i % 8) + 1;
        int type = (i % 3) + 1;
        int res = 2000 + (i * 500);
        int svc = 10 + (i * 3);
        int sat = 70 + (i * 3);
        int year = 2024;
        cvl_community(rgn, type, res, svc, sat, year);
    }

    print_str("\nAssistance report...\n");
    cvl_assistance_report();

    print_str("\nMarriage report...\n");
    cvl_marriage_report();

    print_str("\nCharity report...\n");
    cvl_charity_report();

    print_str("\nFinal state...\n");
    cvl_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
