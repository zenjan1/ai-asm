/* disaster_relief: Disaster relief administration system (v1.0)
 * Disaster reporting, rescue operations, victim resettlement, post-disaster reconstruction, relief donations
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

#define MAX_REPORT       16
#define MAX_RESCUE       14
#define MAX_RESETTLEMENT 12
#define MAX_RECONSTRUCT  10
#define MAX_DONATION     10

typedef struct {
    int    report_id;
    int    disaster_type;
    int    severity_level;
    int    incidents_collected;
    int    assessments_done;
    int    reports_submitted;
    int    year;
    int    active;
} disaster_report_t;

typedef struct {
    int    rescue_id;
    int    rescue_type;
    int    operation_area;
    int    rescue_teams;
    int    supply_drops;
    int    coordination_ops;
    int    year;
    int    active;
} rescue_op_t;

typedef struct {
    int    resettlement_id;
    int    resettlement_type;
    int    victim_category;
    int    victims_transferred;
    int    temporary_housing;
    int    living_assistance;
    int    year;
    int    active;
} resettlement_t;

typedef struct {
    int    reconstruct_id;
    int    reconstruct_type;
    int    reconstruction_area;
    int    infrastructure_repaired;
    int    housing_rebuilt;
    int    production_restored;
    int    year;
    int    active;
} reconstruction_t;

typedef struct {
    int    donation_id;
    int    donation_type;
    int    donor_category;
    int    donations_received;
    int    supplies_distributed;
    int    donation_disclosures;
    int    year;
    int    active;
} donation_t;

typedef struct {
    int    n_report;
    int    n_rescue;
    int    n_resettlement;
    int    n_reconstruct;
    int    n_donation;
    int    total_incidents;
    int    total_teams;
    int    total_transferred;
    int    total_infrastructure;
    int    total_donations;
} dr_state_t;

static disaster_report_t reports[MAX_REPORT];
static rescue_op_t rescues[MAX_RESCUE];
static resettlement_t resettlements[MAX_RESETTLEMENT];
static reconstruction_t reconstructions[MAX_RECONSTRUCT];
static donation_t donations[MAX_DONATION];
static dr_state_t dr;

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

int dr_init(void) {
    if (initialized) return -1;
    dr.n_report = 0; dr.n_rescue = 0; dr.n_resettlement = 0;
    dr.n_reconstruct = 0; dr.n_donation = 0;
    dr.total_incidents = 0; dr.total_teams = 0;
    dr.total_transferred = 0; dr.total_infrastructure = 0;
    dr.total_donations = 0;
    for (int i = 0; i < MAX_REPORT; i++) reports[i].active = 0;
    for (int i = 0; i < MAX_RESCUE; i++) rescues[i].active = 0;
    for (int i = 0; i < MAX_RESETTLEMENT; i++) resettlements[i].active = 0;
    for (int i = 0; i < MAX_RECONSTRUCT; i++) reconstructions[i].active = 0;
    for (int i = 0; i < MAX_DONATION; i++) donations[i].active = 0;
    initialized = 1;
    print_str("[DR] Disaster relief initialized\n");
    return 0;
}

int dr_report(int d_type, int severity, int collected, int assessments, int submitted, int year) {
    if (dr.n_report >= MAX_REPORT) return -1;
    disaster_report_t* r = &reports[dr.n_report];
    r->report_id = dr.n_report;
    r->disaster_type = d_type;
    r->severity_level = severity;
    r->incidents_collected = collected;
    r->assessments_done = assessments;
    r->reports_submitted = submitted;
    r->year = year;
    r->active = 1;
    dr.total_incidents += collected;
    dr.n_report++;
    print_str("[DR] Report "); print_int(dr.n_report - 1);
    print_str(" type="); print_int(d_type);
    print_str(" sev="); print_int(severity);
    print_str(" col="); print_int(collected);
    print_str(" ast="); print_int(assessments);
    print_str(" sub="); print_int(submitted); print_str("\n");
    return dr.n_report - 1;
}

int dr_rescue(int r_type, int area, int teams, int drops, int coordination, int year) {
    if (dr.n_rescue >= MAX_RESCUE) return -1;
    rescue_op_t* r = &rescues[dr.n_rescue];
    r->rescue_id = dr.n_rescue;
    r->rescue_type = r_type;
    r->operation_area = area;
    r->rescue_teams = teams;
    r->supply_drops = drops;
    r->coordination_ops = coordination;
    r->year = year;
    r->active = 1;
    dr.total_teams += teams;
    dr.n_rescue++;
    print_str("[DR] Rescue "); print_int(dr.n_rescue - 1);
    print_str(" type="); print_int(r_type);
    print_str(" area="); print_int(area);
    print_str(" tm="); print_int(teams);
    print_str(" drp="); print_int(drops);
    print_str(" crd="); print_int(coordination); print_str("\n");
    return dr.n_rescue - 1;
}

int dr_resettlement(int rs_type, int v_cat, int transferred, int housing, int assistance, int year) {
    if (dr.n_resettlement >= MAX_RESETTLEMENT) return -1;
    resettlement_t* r = &resettlements[dr.n_resettlement];
    r->resettlement_id = dr.n_resettlement;
    r->resettlement_type = rs_type;
    r->victim_category = v_cat;
    r->victims_transferred = transferred;
    r->temporary_housing = housing;
    r->living_assistance = assistance;
    r->year = year;
    r->active = 1;
    dr.total_transferred += transferred;
    dr.n_resettlement++;
    print_str("[DR] Resettlement "); print_int(dr.n_resettlement - 1);
    print_str(" type="); print_int(rs_type);
    print_str(" cat="); print_int(v_cat);
    print_str(" trf="); print_int(transferred);
    print_str(" hsg="); print_int(housing);
    print_str(" ast="); print_int(assistance); print_str("\n");
    return dr.n_resettlement - 1;
}

int dr_reconstruct(int rc_type, int area, int infra, int housing, int production, int year) {
    if (dr.n_reconstruct >= MAX_RECONSTRUCT) return -1;
    reconstruction_t* r = &reconstructions[dr.n_reconstruct];
    r->reconstruct_id = dr.n_reconstruct;
    r->reconstruct_type = rc_type;
    r->reconstruction_area = area;
    r->infrastructure_repaired = infra;
    r->housing_rebuilt = housing;
    r->production_restored = production;
    r->year = year;
    r->active = 1;
    dr.total_infrastructure += infra;
    dr.n_reconstruct++;
    print_str("[DR] Reconstruct "); print_int(dr.n_reconstruct - 1);
    print_str(" type="); print_int(rc_type);
    print_str(" area="); print_int(area);
    print_str(" inf="); print_int(infra);
    print_str(" hsg="); print_int(housing);
    print_str(" prd="); print_int(production); print_str("\n");
    return dr.n_reconstruct - 1;
}

int dr_donation(int dn_type, int donor, int received, int distributed, int disclosures, int year) {
    if (dr.n_donation >= MAX_DONATION) return -1;
    donation_t* d = &donations[dr.n_donation];
    d->donation_id = dr.n_donation;
    d->donation_type = dn_type;
    d->donor_category = donor;
    d->donations_received = received;
    d->supplies_distributed = distributed;
    d->donation_disclosures = disclosures;
    d->year = year;
    d->active = 1;
    dr.total_donations += received;
    dr.n_donation++;
    print_str("[DR] Donation "); print_int(dr.n_donation - 1);
    print_str(" type="); print_int(dn_type);
    print_str(" dnr="); print_int(donor);
    print_str(" rcv="); print_int(received);
    print_str(" dst="); print_int(distributed);
    print_str(" dsc="); print_int(disclosures); print_str("\n");
    return dr.n_donation - 1;
}

void dr_report_summary(void) {
    print_str("[DR] Report summary:\n");
    print_str("  Disaster report categories: "); print_int(dr.n_report); print_str("\n");
    print_str("  Total incidents collected: "); print_int(dr.total_incidents); print_str("\n");
}

void dr_rescue_summary(void) {
    print_str("[DR] Rescue summary:\n");
    print_str("  Rescue operation categories: "); print_int(dr.n_rescue); print_str("\n");
    print_str("  Total rescue teams: "); print_int(dr.total_teams); print_str("\n");
}

void dr_donation_summary(void) {
    print_str("[DR] Donation summary:\n");
    print_str("  Resettlement categories: "); print_int(dr.n_resettlement); print_str("\n");
    print_str("  Total victims transferred: "); print_int(dr.total_transferred); print_str("\n");
    print_str("  Reconstruction categories: "); print_int(dr.n_reconstruct); print_str("\n");
    print_str("  Total infrastructure repaired: "); print_int(dr.total_infrastructure); print_str("\n");
    print_str("  Donation categories: "); print_int(dr.n_donation); print_str("\n");
    print_str("  Total donations received: "); print_int(dr.total_donations); print_str("\n");
}

void dr_print_state(void) {
    print_str("[DR] Rp="); print_int(dr.n_report);
    print_str(" Rs="); print_int(dr.n_rescue);
    print_str(" St="); print_int(dr.n_resettlement);
    print_str(" Rc="); print_int(dr.n_reconstruct);
    print_str(" Dn="); print_int(dr.n_donation);
    print_str("\n");
}

int main(void) {
    print_str("=== Disaster Relief Demo ===\n\n");
    dr_init();

    print_str("Disaster reporting...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 6) + 1;
        int sev = (i % 5) + 1;
        int col = 30 + (i * 8);
        int ast = 20 + (i * 5);
        int sub = 15 + (i * 4);
        int year = 2020 + (i % 5);
        dr_report(type, sev, col, ast, sub, year);
    }

    print_str("\nRescue operations...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int area = (i % 5) + 1;
        int tm = 10 + (i * 3);
        int drp = 50 + (i * 12);
        int crd = 20 + (i * 5);
        int year = 2021 + (i % 4);
        dr_rescue(type, area, tm, drp, crd, year);
    }

    print_str("\nVictim resettlement...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int trf = 100 + (i * 25);
        int hsg = 80 + (i * 20);
        int ast = 60 + (i * 15);
        int year = 2022 + (i % 3);
        dr_resettlement(type, cat, trf, hsg, ast, year);
    }

    print_str("\nPost-disaster reconstruction...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int area = (i % 5) + 1;
        int inf = 40 + (i * 10);
        int hsg = 30 + (i * 8);
        int prd = 25 + (i * 6);
        int year = 2023 + (i % 2);
        dr_reconstruct(type, area, inf, hsg, prd, year);
    }

    print_str("\nRelief donations...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int dnr = (i % 5) + 1;
        int rcv = 200 + (i * 50);
        int dst = 180 + (i * 45);
        int dsc = 50 + (i * 12);
        int year = 2024;
        dr_donation(type, dnr, rcv, dst, dsc, year);
    }

    print_str("\nReport summary...\n");
    dr_report_summary();

    print_str("\nRescue summary...\n");
    dr_rescue_summary();

    print_str("\nDonation summary...\n");
    dr_donation_summary();

    print_str("\nFinal state...\n");
    dr_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
