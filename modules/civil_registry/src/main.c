/* civil_registry: Civil registration system (v1.0)
 * Births, deaths, marriages, identity cards, vital statistics
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

#define MAX_BIRTHS     20
#define MAX_DEATHS     12
#define MAX_MARRIAGES  16
#define MAX_DIVORCES   8
#define MAX_ID_CARDS   20
#define MAX_CERTS      16

typedef struct {
    int    birth_id;
    int    family_id;
    int    gender;
    int    birth_weight;
    int    birth_year;
    int    birth_month;
    int    birth_day;
    int    father_id;
    int    mother_id;
    int    hospital_id;
    int    registered;
    int    active;
} birth_t;

typedef struct {
    int    death_id;
    int    person_id;
    int    age;
    int    cause;
    int    death_year;
    int    death_month;
    int    death_day;
    int    hospital_id;
    int    certified;
    int    active;
} death_t;

typedef struct {
    int    marriage_id;
    int    spouse1_id;
    int    spouse2_id;
    int    type;
    int    year;
    int    month;
    int    day;
    int    registrar_id;
    int    active;
} marriage_t;

typedef struct {
    int    divorce_id;
    int    marriage_id;
    int    spouse1_id;
    int    spouse2_id;
    int    year;
    int    month;
    int    day;
    int    reason;
    int    settled;
    int    active;
} divorce_t;

typedef struct {
    int    card_id;
    int    person_id;
    int    type;
    int    issued_year;
    int    expiry_year;
    int    status;
    int    active;
} id_card_t;

typedef struct {
    int    cert_id;
    int    type;
    int    ref_id;
    int    person_id;
    int    issued_year;
    int    status;
    int    active;
} cert_t;

typedef struct {
    int    n_births;
    int    n_deaths;
    int    n_marriages;
    int    n_divorces;
    int    n_id_cards;
    int    n_certs;
    int    total_births;
    int    total_deaths;
    int    total_marriages;
    int    total_divorces;
    int    birth_rate;
    int    death_rate;
    int    marriage_rate;
} cr_state_t;

static birth_t births[MAX_BIRTHS];
static death_t deaths[MAX_DEATHS];
static marriage_t marriages[MAX_MARRIAGES];
static divorce_t divorces[MAX_DIVORCES];
static id_card_t id_cards[MAX_ID_CARDS];
static cert_t certs[MAX_CERTS];
static cr_state_t cr;

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

int cr_init(void) {
    if (initialized) return -1;
    cr.n_births = 0; cr.n_deaths = 0; cr.n_marriages = 0;
    cr.n_divorces = 0; cr.n_id_cards = 0; cr.n_certs = 0;
    cr.total_births = 0; cr.total_deaths = 0;
    cr.total_marriages = 0; cr.total_divorces = 0;
    cr.birth_rate = 0; cr.death_rate = 0; cr.marriage_rate = 0;
    for (int i = 0; i < MAX_BIRTHS; i++) births[i].active = 0;
    for (int i = 0; i < MAX_DEATHS; i++) deaths[i].active = 0;
    for (int i = 0; i < MAX_MARRIAGES; i++) marriages[i].active = 0;
    for (int i = 0; i < MAX_DIVORCES; i++) divorces[i].active = 0;
    for (int i = 0; i < MAX_ID_CARDS; i++) id_cards[i].active = 0;
    for (int i = 0; i < MAX_CERTS; i++) certs[i].active = 0;
    initialized = 1;
    print_str("[CR] Civil registry initialized\n");
    return 0;
}

int cr_register_birth(int family, int gender, int weight, int year, int month, int day, int father, int mother, int hospital) {
    if (cr.n_births >= MAX_BIRTHS) return -1;
    birth_t* b = &births[cr.n_births];
    b->birth_id = cr.n_births;
    b->family_id = family;
    b->gender = gender;
    b->birth_weight = weight;
    b->birth_year = year;
    b->birth_month = month;
    b->birth_day = day;
    b->father_id = father;
    b->mother_id = mother;
    b->hospital_id = hospital;
    b->registered = 1;
    b->active = 1;
    cr.total_births++;
    cr.birth_rate += 10;
    cr.n_births++;
    print_str("[CR] Birth "); print_int(cr.n_births - 1);
    print_str(" fam="); print_int(family);
    print_str(" g="); print_int(gender);
    print_str(" wt="); print_int(weight);
    print_str(" "); print_int(year); print_str("-"); print_int(month); print_str("-"); print_int(day);
    print_str(" H"); print_int(hospital); print_str("\n");
    return cr.n_births - 1;
}

int cr_register_death(int person, int age, int cause, int year, int month, int day, int hospital) {
    if (cr.n_deaths >= MAX_DEATHS) return -1;
    death_t* d = &deaths[cr.n_deaths];
    d->death_id = cr.n_deaths;
    d->person_id = person;
    d->age = age;
    d->cause = cause;
    d->death_year = year;
    d->death_month = month;
    d->death_day = day;
    d->hospital_id = hospital;
    d->certified = 0;
    d->active = 1;
    cr.total_deaths++;
    cr.death_rate += 8;
    cr.n_deaths++;
    print_str("[CR] Death "); print_int(cr.n_deaths - 1);
    print_str(" P"); print_int(person);
    print_str(" age="); print_int(age);
    print_str(" cause="); print_int(cause);
    print_str(" "); print_int(year); print_str("-"); print_int(month); print_str("-"); print_int(day);
    print_str(" H"); print_int(hospital); print_str("\n");
    return cr.n_deaths - 1;
}

int cr_certify_death(int death_id) {
    if (death_id >= cr.n_deaths) return -1;
    deaths[death_id].certified = 1;
    print_str("[CR] Certify D"); print_int(death_id); print_str("\n");
    return 0;
}

int cr_register_marriage(int s1, int s2, int type, int year, int month, int day, int registrar) {
    if (cr.n_marriages >= MAX_MARRIAGES) return -1;
    marriage_t* m = &marriages[cr.n_marriages];
    m->marriage_id = cr.n_marriages;
    m->spouse1_id = s1;
    m->spouse2_id = s2;
    m->type = type;
    m->year = year;
    m->month = month;
    m->day = day;
    m->registrar_id = registrar;
    m->active = 1;
    cr.total_marriages++;
    cr.marriage_rate += 5;
    cr.n_marriages++;
    print_str("[CR] Marriage "); print_int(cr.n_marriages - 1);
    print_str(" S"); print_int(s1); print_str("+S"); print_int(s2);
    print_str(" type="); print_int(type);
    print_str(" "); print_int(year); print_str("-"); print_int(month); print_str("-"); print_int(day);
    print_str(" R"); print_int(registrar); print_str("\n");
    return cr.n_marriages - 1;
}

int cr_register_divorce(int marriage_id, int s1, int s2, int year, int month, int day, int reason) {
    if (cr.n_divorces >= MAX_DIVORCES || marriage_id >= cr.n_marriages) return -1;
    divorce_t* d = &divorces[cr.n_divorces];
    d->divorce_id = cr.n_divorces;
    d->marriage_id = marriage_id;
    d->spouse1_id = s1;
    d->spouse2_id = s2;
    d->year = year;
    d->month = month;
    d->day = day;
    d->reason = reason;
    d->settled = 0;
    d->active = 1;
    cr.total_divorces++;
    cr.n_divorces++;
    print_str("[CR] Divorce "); print_int(cr.n_divorces - 1);
    print_str(" M"); print_int(marriage_id);
    print_str(" S"); print_int(s1); print_str("+S"); print_int(s2);
    print_str(" reason="); print_int(reason); print_str("\n");
    return cr.n_divorces - 1;
}

int cr_settle_divorce(int divorce_id) {
    if (divorce_id >= cr.n_divorces) return -1;
    divorces[divorce_id].settled = 1;
    print_str("[CR] Settle Div"); print_int(divorce_id); print_str("\n");
    return 0;
}

int cr_issue_id_card(int person, int type, int issued_year, int expiry_year) {
    if (cr.n_id_cards >= MAX_ID_CARDS) return -1;
    id_card_t* c = &id_cards[cr.n_id_cards];
    c->card_id = cr.n_id_cards;
    c->person_id = person;
    c->type = type;
    c->issued_year = issued_year;
    c->expiry_year = expiry_year;
    c->status = 1;
    c->active = 1;
    cr.n_id_cards++;
    print_str("[CR] ID "); print_int(cr.n_id_cards - 1);
    print_str(" P"); print_int(person);
    print_str(" type="); print_int(type);
    print_str(" "); print_int(issued_year); print_str("-"); print_int(expiry_year); print_str("\n");
    return cr.n_id_cards - 1;
}

int cr_issue_cert(int type, int ref, int person, int year) {
    if (cr.n_certs >= MAX_CERTS) return -1;
    cert_t* c = &certs[cr.n_certs];
    c->cert_id = cr.n_certs;
    c->type = type;
    c->ref_id = ref;
    c->person_id = person;
    c->issued_year = year;
    c->status = 1;
    c->active = 1;
    cr.n_certs++;
    print_str("[CR] Cert "); print_int(cr.n_certs - 1);
    print_str(" type="); print_int(type);
    print_str(" ref="); print_int(ref);
    print_str(" P"); print_int(person);
    print_str(" "); print_int(year); print_str("\n");
    return cr.n_certs - 1;
}

int cr_renew_id(int card_id, int new_expiry) {
    if (card_id >= cr.n_id_cards) return -1;
    id_cards[card_id].expiry_year = new_expiry;
    id_cards[card_id].status = 1;
    print_str("[CR] Renew ID"); print_int(card_id);
    print_str(" exp="); print_int(new_expiry); print_str("\n");
    return 0;
}

void cr_vital_stats(void) {
    print_str("[CR] Vital statistics:\n");
    print_str("  Births: "); print_int(cr.total_births);
    print_str(" rate="); print_int(cr.birth_rate); print_str("/1000\n");
    print_str("  Deaths: "); print_int(cr.total_deaths);
    print_str(" rate="); print_int(cr.death_rate); print_str("/1000\n");
    print_str("  Marriages: "); print_int(cr.total_marriages);
    print_str(" rate="); print_int(cr.marriage_rate); print_str("/1000\n");
    print_str("  Divorces: "); print_int(cr.total_divorces); print_str("\n");
}

void cr_id_report(void) {
    print_str("[CR] ID cards: "); print_int(cr.n_id_cards); print_str("\n");
    for (int i = 0; i < cr.n_id_cards; i++) {
        id_card_t* c = &id_cards[i];
        print_str("  ID"); print_int(i);
        print_str(" P"); print_int(c->person_id);
        print_str(" type="); print_int(c->type);
        print_str(" "); print_int(c->issued_year); print_str("-"); print_int(c->expiry_year);
        print_str(" st="); print_int(c->status); print_str("\n");
    }
}

void cr_cert_report(void) {
    print_str("[CR] Certificates: "); print_int(cr.n_certs); print_str("\n");
    for (int i = 0; i < cr.n_certs; i++) {
        cert_t* c = &certs[i];
        print_str("  Cert"); print_int(i);
        print_str(" type="); print_int(c->type);
        print_str(" ref="); print_int(c->ref_id);
        print_str(" P"); print_int(c->person_id);
        print_str(" "); print_int(c->issued_year); print_str("\n");
    }
}

void cr_print_state(void) {
    print_str("[CR] Births="); print_int(cr.n_births);
    print_str(" Deaths="); print_int(cr.n_deaths);
    print_str(" Marriages="); print_int(cr.n_marriages);
    print_str(" Divorces="); print_int(cr.n_divorces);
    print_str(" IDs="); print_int(cr.n_id_cards);
    print_str(" Certs="); print_int(cr.n_certs);
    print_str("\n");
}

int main(void) {
    print_str("=== Civil Registry Demo ===\n\n");
    cr_init();

    print_str("Registering births...\n");
    for (int i = 0; i < 20; i++) {
        int fam = (i % 10) + 1;
        int gender = (i % 2) + 1;
        int weight = 2500 + (i * 150);
        int year = 2020 + (i % 6);
        int month = 1 + (i % 12);
        int day = 1 + (i % 28);
        int father = (i * 2) % 20;
        int mother = (i * 2 + 1) % 20;
        int hospital = (i % 5) + 1;
        cr_register_birth(fam, gender, weight, year, month, day, father, mother, hospital);
    }

    print_str("\nRegistering deaths...\n");
    for (int i = 0; i < 12; i++) {
        int person = 30 + i;
        int age = 50 + (i * 5);
        int cause = (i % 4) + 1;
        int year = 2022 + (i % 4);
        int month = 1 + (i % 12);
        int day = 1 + (i % 28);
        int hospital = (i % 5) + 1;
        cr_register_death(person, age, cause, year, month, day, hospital);
    }

    print_str("\nCertifying deaths...\n");
    for (int i = 0; i < 12; i++) {
        cr_certify_death(i);
    }

    print_str("\nRegistering marriages...\n");
    for (int i = 0; i < 16; i++) {
        int s1 = i * 2;
        int s2 = i * 2 + 1;
        int type = (i % 3) + 1;
        int year = 2018 + (i % 8);
        int month = 1 + (i % 12);
        int day = 1 + (i % 28);
        int reg = (i % 4) + 1;
        cr_register_marriage(s1, s2, type, year, month, day, reg);
    }

    print_str("\nRegistering divorces...\n");
    for (int i = 0; i < 8; i++) {
        int mid = i * 2;
        int s1 = i * 4;
        int s2 = i * 4 + 1;
        int year = 2023 + (i % 3);
        int month = 1 + (i % 12);
        int day = 1 + (i % 28);
        int reason = (i % 3) + 1;
        cr_register_divorce(mid, s1, s2, year, month, day, reason);
    }

    print_str("\nSettling divorces...\n");
    for (int i = 0; i < 8; i++) {
        cr_settle_divorce(i);
    }

    print_str("\nIssuing ID cards...\n");
    for (int i = 0; i < 20; i++) {
        int person = i;
        int type = (i % 3) + 1;
        int issued = 2020 + (i % 6);
        int expiry = issued + 10;
        cr_issue_id_card(person, type, issued, expiry);
    }

    print_str("\nRenewing ID cards...\n");
    for (int i = 0; i < 10; i++) {
        cr_renew_id(i, 2035 + i);
    }

    print_str("\nIssuing certificates...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 4) + 1;
        int ref = i % 20;
        int person = i;
        int year = 2022 + (i % 4);
        cr_issue_cert(type, ref, person, year);
    }

    print_str("\nVital statistics...\n");
    cr_vital_stats();

    print_str("\nID report...\n");
    cr_id_report();

    print_str("\nCertificate report...\n");
    cr_cert_report();

    print_str("\nFinal state...\n");
    cr_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
