/* nursing_admin: Nursing administration system (v1.0)
 * Basic nursing, clinical nursing, community nursing, management, specialized
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

#define MAX_BASIC_N     16
#define MAX_CLINIC_N    14
#define MAX_COMMUN_N    12
#define MAX_MGMT_N      10
#define MAX_SPEC_N      10

typedef struct {
    int    bn_id;
    int    bn_type;
    int    bn_category;
    int    nur_base;
    int    nur_skill;
    int    nur_assess;
    int    nur_plan;
    int    year;
    int    active;
} basic_n_t;

typedef struct {
    int    cn_id;
    int    cn_type;
    int    cn_category;
    int    int_nurs;
    int    srg_nurs;
    int    obs_nurs;
    int    ped_nurs;
    int    year;
    int    active;
} clinic_n_t;

typedef struct {
    int    cm_id;
    int    cm_type;
    int    cm_category;
    int    com_hlth;
    int    fam_nurs;
    int    old_nurs;
    int    reh_nurs;
    int    year;
    int    active;
} commun_n_t;

typedef struct {
    int    mg_id;
    int    mg_type;
    int    mg_category;
    int    nur_qual;
    int    nur_edu;
    int    nur_res;
    int    nur_lead;
    int    year;
    int    active;
} mgmt_n_t;

typedef struct {
    int    sn_id;
    int    sn_type;
    int    sn_category;
    int    emg_nurs;
    int    icu_nurs;
    int    opr_nurs;
    int    onc_nurs;
    int    year;
    int    active;
} spec_n_t;

typedef struct {
    int    n_basic_n;
    int    n_clinic_n;
    int    n_commun_n;
    int    n_mgmt_n;
    int    n_spec_n;
    int    total_nur_base;
    int    total_int_nurs;
    int    total_com_hlth;
    int    total_nur_qual;
    int    total_emg_nurs;
} nsa_state_t;

static basic_n_t basic_ns[MAX_BASIC_N];
static clinic_n_t clinic_ns[MAX_CLINIC_N];
static commun_n_t commun_ns[MAX_COMMUN_N];
static mgmt_n_t mgmt_ns[MAX_MGMT_N];
static spec_n_t spec_ns[MAX_SPEC_N];
static nsa_state_t nsa;

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

int nsa_init(void) {
    if (initialized) return -1;
    nsa.n_basic_n = 0; nsa.n_clinic_n = 0; nsa.n_commun_n = 0;
    nsa.n_mgmt_n = 0; nsa.n_spec_n = 0;
    nsa.total_nur_base = 0; nsa.total_int_nurs = 0;
    nsa.total_com_hlth = 0; nsa.total_nur_qual = 0;
    nsa.total_emg_nurs = 0;
    for (int i = 0; i < MAX_BASIC_N; i++) basic_ns[i].active = 0;
    for (int i = 0; i < MAX_CLINIC_N; i++) clinic_ns[i].active = 0;
    for (int i = 0; i < MAX_COMMUN_N; i++) commun_ns[i].active = 0;
    for (int i = 0; i < MAX_MGMT_N; i++) mgmt_ns[i].active = 0;
    for (int i = 0; i < MAX_SPEC_N; i++) spec_ns[i].active = 0;
    initialized = 1;
    print_str("[NSA] Nursing initialized\n");
    return 0;
}

int nsa_basic_n(int bt_type, int cat, int nrb, int nsk, int nas, int npl, int year) {
    if (nsa.n_basic_n >= MAX_BASIC_N) return -1;
    basic_n_t* b = &basic_ns[nsa.n_basic_n];
    b->bn_id = nsa.n_basic_n;
    b->bn_type = bt_type;
    b->bn_category = cat;
    b->nur_base = nrb;
    b->nur_skill = nsk;
    b->nur_assess = nas;
    b->nur_plan = npl;
    b->year = year;
    b->active = 1;
    nsa.total_nur_base += nrb;
    nsa.n_basic_n++;
    print_str("[NSA] Basic n "); print_int(nsa.n_basic_n - 1);
    print_str(" type="); print_int(bt_type);
    print_str(" cat="); print_int(cat);
    print_str(" nrb="); print_int(nrb);
    print_str(" nsk="); print_int(nsk);
    print_str(" nas="); print_int(nas);
    print_str(" npl="); print_int(npl); print_str("\n");
    return nsa.n_basic_n - 1;
}

int nsa_clinic_n(int ct_type, int cat, int inn, int srn, int obn, int pdn, int year) {
    if (nsa.n_clinic_n >= MAX_CLINIC_N) return -1;
    clinic_n_t* c = &clinic_ns[nsa.n_clinic_n];
    c->cn_id = nsa.n_clinic_n;
    c->cn_type = ct_type;
    c->cn_category = cat;
    c->int_nurs = inn;
    c->srg_nurs = srn;
    c->obs_nurs = obn;
    c->ped_nurs = pdn;
    c->year = year;
    c->active = 1;
    nsa.total_int_nurs += inn;
    nsa.n_clinic_n++;
    print_str("[NSA] Clinic n "); print_int(nsa.n_clinic_n - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" inn="); print_int(inn);
    print_str(" srn="); print_int(srn);
    print_str(" obn="); print_int(obn);
    print_str(" pdn="); print_int(pdn); print_str("\n");
    return nsa.n_clinic_n - 1;
}

int nsa_commun_n(int ct_type, int cat, int cmh, int fmn, int oln, int rhn, int year) {
    if (nsa.n_commun_n >= MAX_COMMUN_N) return -1;
    commun_n_t* c = &commun_ns[nsa.n_commun_n];
    c->cm_id = nsa.n_commun_n;
    c->cm_type = ct_type;
    c->cm_category = cat;
    c->com_hlth = cmh;
    c->fam_nurs = fmn;
    c->old_nurs = oln;
    c->reh_nurs = rhn;
    c->year = year;
    c->active = 1;
    nsa.total_com_hlth += cmh;
    nsa.n_commun_n++;
    print_str("[NSA] Commun n "); print_int(nsa.n_commun_n - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" cmh="); print_int(cmh);
    print_str(" fmn="); print_int(fmn);
    print_str(" oln="); print_int(oln);
    print_str(" rhn="); print_int(rhn); print_str("\n");
    return nsa.n_commun_n - 1;
}

int nsa_mgmt_n(int mt_type, int cat, int nrq, int nre, int nrr, int nrl, int year) {
    if (nsa.n_mgmt_n >= MAX_MGMT_N) return -1;
    mgmt_n_t* m = &mgmt_ns[nsa.n_mgmt_n];
    m->mg_id = nsa.n_mgmt_n;
    m->mg_type = mt_type;
    m->mg_category = cat;
    m->nur_qual = nrq;
    m->nur_edu = nre;
    m->nur_res = nrr;
    m->nur_lead = nrl;
    m->year = year;
    m->active = 1;
    nsa.total_nur_qual += nrq;
    nsa.n_mgmt_n++;
    print_str("[NSA] Mgmt n "); print_int(nsa.n_mgmt_n - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" nrq="); print_int(nrq);
    print_str(" nre="); print_int(nre);
    print_str(" nrr="); print_int(nrr);
    print_str(" nrl="); print_int(nrl); print_str("\n");
    return nsa.n_mgmt_n - 1;
}

int nsa_spec_n(int st_type, int cat, int emn, int icn, int orn, int onn, int year) {
    if (nsa.n_spec_n >= MAX_SPEC_N) return -1;
    spec_n_t* s = &spec_ns[nsa.n_spec_n];
    s->sn_id = nsa.n_spec_n;
    s->sn_type = st_type;
    s->sn_category = cat;
    s->emg_nurs = emn;
    s->icu_nurs = icn;
    s->opr_nurs = orn;
    s->onc_nurs = onn;
    s->year = year;
    s->active = 1;
    nsa.total_emg_nurs += emn;
    nsa.n_spec_n++;
    print_str("[NSA] Spec n "); print_int(nsa.n_spec_n - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" emn="); print_int(emn);
    print_str(" icn="); print_int(icn);
    print_str(" orn="); print_int(orn);
    print_str(" onn="); print_int(onn); print_str("\n");
    return nsa.n_spec_n - 1;
}

void nsa_basic_report(void) {
    print_str("[NSA] Basic nursing report:\n");
    print_str("  Basic nursing categories: "); print_int(nsa.n_basic_n); print_str("\n");
    print_str("  Total nursing base: "); print_int(nsa.total_nur_base); print_str("\n");
}

void nsa_clinic_report(void) {
    print_str("[NSA] Clinical nursing report:\n");
    print_str("  Clinical nursing categories: "); print_int(nsa.n_clinic_n); print_str("\n");
    print_str("  Total internal nursing: "); print_int(nsa.total_int_nurs); print_str("\n");
}

void nsa_full_report(void) {
    print_str("[NSA] Full report:\n");
    print_str("  Community nursing categories: "); print_int(nsa.n_commun_n); print_str("\n");
    print_str("  Total community health: "); print_int(nsa.total_com_hlth); print_str("\n");
    print_str("  Management categories: "); print_int(nsa.n_mgmt_n); print_str("\n");
    print_str("  Total nursing quality: "); print_int(nsa.total_nur_qual); print_str("\n");
    print_str("  Specialized categories: "); print_int(nsa.n_spec_n); print_str("\n");
    print_str("  Total emergency nursing: "); print_int(nsa.total_emg_nurs); print_str("\n");
}

void nsa_print_state(void) {
    print_str("[NSA] Bn="); print_int(nsa.n_basic_n);
    print_str(" Cn="); print_int(nsa.n_clinic_n);
    print_str(" Cm="); print_int(nsa.n_commun_n);
    print_str(" Mg="); print_int(nsa.n_mgmt_n);
    print_str(" Sn="); print_int(nsa.n_spec_n);
    print_str("\n");
}

int main(void) {
    print_str("=== Nursing Admin Demo ===\n\n");
    nsa_init();

    print_str("Basic nursing...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int nrb = 55 + (i * 13);
        int nsk = 40 + (i * 10);
        int nas = 22 + (i * 5);
        int npl = 15 + (i * 3);
        int year = 2020 + (i % 5);
        nsa_basic_n(type, cat, nrb, nsk, nas, npl, year);
    }

    print_str("\nClinical nursing...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int inn = 48 + (i * 11);
        int srn = 35 + (i * 8);
        int obn = 20 + (i * 4);
        int pdn = 12 + (i * 3);
        int year = 2021 + (i % 4);
        nsa_clinic_n(type, cat, inn, srn, obn, pdn, year);
    }

    print_str("\nCommunity nursing...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cmh = 42 + (i * 10);
        int fmn = 28 + (i * 7);
        int oln = 18 + (i * 4);
        int rhn = 10 + (i * 2);
        int year = 2022 + (i % 3);
        nsa_commun_n(type, cat, cmh, fmn, oln, rhn, year);
    }

    print_str("\nManagement...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int nrq = 35 + (i * 8);
        int nre = 25 + (i * 6);
        int nrr = 15 + (i * 3);
        int nrl = 10 + (i * 2);
        int year = 2023 + (i % 2);
        nsa_mgmt_n(type, cat, nrq, nre, nrr, nrl, year);
    }

    print_str("\nSpecialized nursing...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int emn = 30 + (i * 7);
        int icn = 22 + (i * 5);
        int orn = 12 + (i * 3);
        int onn = 8 + (i * 2);
        int year = 2024;
        nsa_spec_n(type, cat, emn, icn, orn, onn, year);
    }

    print_str("\nBasic report...\n");
    nsa_basic_report();

    print_str("\nClinical report...\n");
    nsa_clinic_report();

    print_str("\nFull report...\n");
    nsa_full_report();

    print_str("\nFinal state...\n");
    nsa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
