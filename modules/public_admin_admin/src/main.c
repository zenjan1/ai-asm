/* public_admin_admin: Public administration management system (v1.0)
 * Public management, urban management, rural management, information, emergency
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

#define MAX_PUB_MG      16
#define MAX_URBAN_MG    14
#define MAX_RURAL_MG    12
#define MAX_INFO_MG     10
#define MAX_EMERG_MG    10

typedef struct {
    int    pm_id;
    int    pm_type;
    int    pm_category;
    int    pub_pri;
    int    pub_dept;
    int    nonprof;
    int    pub_ethic;
    int    year;
    int    active;
} pub_mg_t;

typedef struct {
    int    um_id;
    int    um_type;
    int    um_category;
    int    urb_plan;
    int    munic;
    int    urb_tra;
    int    urb_env;
    int    year;
    int    active;
} urban_mg_t;

typedef struct {
    int    rm_id;
    int    rm_type;
    int    rm_category;
    int    rur_dev;
    int    agri_mg;
    int    farmer;
    int    rur_gov;
    int    year;
    int    active;
} rural_mg_t;

typedef struct {
    int    im_id;
    int    im_type;
    int    im_category;
    int    info_sys;
    int    info_res;
    int    info_sec;
    int    info_svc;
    int    year;
    int    active;
} info_mg_t;

typedef struct {
    int    em_id;
    int    em_type;
    int    em_category;
    int    risk_as;
    int    emerg_prep;
    int    emerg_resp;
    int    recovery;
    int    year;
    int    active;
} emerg_mg_t;

typedef struct {
    int    n_pub_mg;
    int    n_urban_mg;
    int    n_rural_mg;
    int    n_info_mg;
    int    n_emerg_mg;
    int    total_pub_pri;
    int    total_urb_plan;
    int    total_rur_dev;
    int    total_info_sys;
    int    total_risk_as;
} paa_state_t;

static pub_mg_t pub_mgs[MAX_PUB_MG];
static urban_mg_t urban_mgs[MAX_URBAN_MG];
static rural_mg_t rural_mgs[MAX_RURAL_MG];
static info_mg_t info_mgs[MAX_INFO_MG];
static emerg_mg_t emerg_mgs[MAX_EMERG_MG];
static paa_state_t paa;

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

int paa_init(void) {
    if (initialized) return -1;
    paa.n_pub_mg = 0; paa.n_urban_mg = 0; paa.n_rural_mg = 0;
    paa.n_info_mg = 0; paa.n_emerg_mg = 0;
    paa.total_pub_pri = 0; paa.total_urb_plan = 0;
    paa.total_rur_dev = 0; paa.total_info_sys = 0;
    paa.total_risk_as = 0;
    for (int i = 0; i < MAX_PUB_MG; i++) pub_mgs[i].active = 0;
    for (int i = 0; i < MAX_URBAN_MG; i++) urban_mgs[i].active = 0;
    for (int i = 0; i < MAX_RURAL_MG; i++) rural_mgs[i].active = 0;
    for (int i = 0; i < MAX_INFO_MG; i++) info_mgs[i].active = 0;
    for (int i = 0; i < MAX_EMERG_MG; i++) emerg_mgs[i].active = 0;
    initialized = 1;
    print_str("[PAA] Public admin initialized\n");
    return 0;
}

int paa_pub_mg(int pt_type, int cat, int ppr, int pdp, int npr, int pet, int year) {
    if (paa.n_pub_mg >= MAX_PUB_MG) return -1;
    pub_mg_t* p = &pub_mgs[paa.n_pub_mg];
    p->pm_id = paa.n_pub_mg;
    p->pm_type = pt_type;
    p->pm_category = cat;
    p->pub_pri = ppr;
    p->pub_dept = pdp;
    p->nonprof = npr;
    p->pub_ethic = pet;
    p->year = year;
    p->active = 1;
    paa.total_pub_pri += ppr;
    paa.n_pub_mg++;
    print_str("[PAA] Pub mg "); print_int(paa.n_pub_mg - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" ppr="); print_int(ppr);
    print_str(" pdp="); print_int(pdp);
    print_str(" npr="); print_int(npr);
    print_str(" pet="); print_int(pet); print_str("\n");
    return paa.n_pub_mg - 1;
}

int paa_urban_mg(int ut_type, int cat, int upl, int mnc, int utr, int uen, int year) {
    if (paa.n_urban_mg >= MAX_URBAN_MG) return -1;
    urban_mg_t* u = &urban_mgs[paa.n_urban_mg];
    u->um_id = paa.n_urban_mg;
    u->um_type = ut_type;
    u->um_category = cat;
    u->urb_plan = upl;
    u->munic = mnc;
    u->urb_tra = utr;
    u->urb_env = uen;
    u->year = year;
    u->active = 1;
    paa.total_urb_plan += upl;
    paa.n_urban_mg++;
    print_str("[PAA] Urban mg "); print_int(paa.n_urban_mg - 1);
    print_str(" type="); print_int(ut_type);
    print_str(" cat="); print_int(cat);
    print_str(" upl="); print_int(upl);
    print_str(" mnc="); print_int(mnc);
    print_str(" utr="); print_int(utr);
    print_str(" uen="); print_int(uen); print_str("\n");
    return paa.n_urban_mg - 1;
}

int paa_rural_mg(int rt_type, int cat, int rdv, int amg, int frm, int rgo, int year) {
    if (paa.n_rural_mg >= MAX_RURAL_MG) return -1;
    rural_mg_t* r = &rural_mgs[paa.n_rural_mg];
    r->rm_id = paa.n_rural_mg;
    r->rm_type = rt_type;
    r->rm_category = cat;
    r->rur_dev = rdv;
    r->agri_mg = amg;
    r->farmer = frm;
    r->rur_gov = rgo;
    r->year = year;
    r->active = 1;
    paa.total_rur_dev += rdv;
    paa.n_rural_mg++;
    print_str("[PAA] Rural mg "); print_int(paa.n_rural_mg - 1);
    print_str(" type="); print_int(rt_type);
    print_str(" cat="); print_int(cat);
    print_str(" rdv="); print_int(rdv);
    print_str(" amg="); print_int(amg);
    print_str(" frm="); print_int(frm);
    print_str(" rgo="); print_int(rgo); print_str("\n");
    return paa.n_rural_mg - 1;
}

int paa_info_mg(int it_type, int cat, int isy, int irs, int isc, int isv, int year) {
    if (paa.n_info_mg >= MAX_INFO_MG) return -1;
    info_mg_t* n = &info_mgs[paa.n_info_mg];
    n->im_id = paa.n_info_mg;
    n->im_type = it_type;
    n->im_category = cat;
    n->info_sys = isy;
    n->info_res = irs;
    n->info_sec = isc;
    n->info_svc = isv;
    n->year = year;
    n->active = 1;
    paa.total_info_sys += isy;
    paa.n_info_mg++;
    print_str("[PAA] Info mg "); print_int(paa.n_info_mg - 1);
    print_str(" type="); print_int(it_type);
    print_str(" cat="); print_int(cat);
    print_str(" isy="); print_int(isy);
    print_str(" irs="); print_int(irs);
    print_str(" isc="); print_int(isc);
    print_str(" isv="); print_int(isv); print_str("\n");
    return paa.n_info_mg - 1;
}

int paa_emerg_mg(int et_type, int cat, int rks, int epr, int ers, int rcv, int year) {
    if (paa.n_emerg_mg >= MAX_EMERG_MG) return -1;
    emerg_mg_t* e = &emerg_mgs[paa.n_emerg_mg];
    e->em_id = paa.n_emerg_mg;
    e->em_type = et_type;
    e->em_category = cat;
    e->risk_as = rks;
    e->emerg_prep = epr;
    e->emerg_resp = ers;
    e->recovery = rcv;
    e->year = year;
    e->active = 1;
    paa.total_risk_as += rks;
    paa.n_emerg_mg++;
    print_str("[PAA] Emerg mg "); print_int(paa.n_emerg_mg - 1);
    print_str(" type="); print_int(et_type);
    print_str(" cat="); print_int(cat);
    print_str(" rks="); print_int(rks);
    print_str(" epr="); print_int(epr);
    print_str(" ers="); print_int(ers);
    print_str(" rcv="); print_int(rcv); print_str("\n");
    return paa.n_emerg_mg - 1;
}

void paa_pub_report(void) {
    print_str("[PAA] Public management report:\n");
    print_str("  Public mgmt categories: "); print_int(paa.n_pub_mg); print_str("\n");
    print_str("  Total public principles: "); print_int(paa.total_pub_pri); print_str("\n");
}

void paa_urban_report(void) {
    print_str("[PAA] Urban management report:\n");
    print_str("  Urban mgmt categories: "); print_int(paa.n_urban_mg); print_str("\n");
    print_str("  Total urban planning: "); print_int(paa.total_urb_plan); print_str("\n");
}

void paa_full_report(void) {
    print_str("[PAA] Full report:\n");
    print_str("  Rural mgmt categories: "); print_int(paa.n_rural_mg); print_str("\n");
    print_str("  Total rural development: "); print_int(paa.total_rur_dev); print_str("\n");
    print_str("  Information mgmt categories: "); print_int(paa.n_info_mg); print_str("\n");
    print_str("  Total information system: "); print_int(paa.total_info_sys); print_str("\n");
    print_str("  Emergency mgmt categories: "); print_int(paa.n_emerg_mg); print_str("\n");
    print_str("  Total risk assessment: "); print_int(paa.total_risk_as); print_str("\n");
}

void paa_print_state(void) {
    print_str("[PAA] Pm="); print_int(paa.n_pub_mg);
    print_str(" Um="); print_int(paa.n_urban_mg);
    print_str(" Rm="); print_int(paa.n_rural_mg);
    print_str(" Im="); print_int(paa.n_info_mg);
    print_str(" Em="); print_int(paa.n_emerg_mg);
    print_str("\n");
}

int main(void) {
    print_str("=== Public Admin Demo ===\n\n");
    paa_init();

    print_str("Public management...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int ppr = 55 + (i * 13);
        int pdp = 40 + (i * 10);
        int npr = 22 + (i * 5);
        int pet = 15 + (i * 3);
        int year = 2020 + (i % 5);
        paa_pub_mg(type, cat, ppr, pdp, npr, pet, year);
    }

    print_str("\nUrban management...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int upl = 48 + (i * 11);
        int mnc = 35 + (i * 8);
        int utr = 20 + (i * 4);
        int uen = 12 + (i * 3);
        int year = 2021 + (i % 4);
        paa_urban_mg(type, cat, upl, mnc, utr, uen, year);
    }

    print_str("\nRural management...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int rdv = 42 + (i * 10);
        int amg = 28 + (i * 7);
        int frm = 18 + (i * 4);
        int rgo = 10 + (i * 2);
        int year = 2022 + (i % 3);
        paa_rural_mg(type, cat, rdv, amg, frm, rgo, year);
    }

    print_str("\nInformation management...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int isy = 35 + (i * 8);
        int irs = 25 + (i * 6);
        int isc = 15 + (i * 3);
        int isv = 10 + (i * 2);
        int year = 2023 + (i % 2);
        paa_info_mg(type, cat, isy, irs, isc, isv, year);
    }

    print_str("\nEmergency management...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int rks = 30 + (i * 7);
        int epr = 22 + (i * 5);
        int ers = 12 + (i * 3);
        int rcv = 8 + (i * 2);
        int year = 2024;
        paa_emerg_mg(type, cat, rks, epr, ers, rcv, year);
    }

    print_str("\nPublic report...\n");
    paa_pub_report();

    print_str("\nUrban report...\n");
    paa_urban_report();

    print_str("\nFull report...\n");
    paa_full_report();

    print_str("\nFinal state...\n");
    paa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
