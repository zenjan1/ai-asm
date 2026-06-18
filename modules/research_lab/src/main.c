/* research_lab: Research laboratory management (v1.0)
 * Experiments, papers, equipment, funding, patents, costs
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

#define MAX_EXPERIMENTS 16
#define MAX_PAPERS     14
#define MAX_EQUIPMENT  12
#define MAX_GRANTS     10
#define MAX_PATENTS    8

typedef struct {
    int    exp_id;
    int    type;
    int    samples;
    int    duration_days;
    int    equipment_id;
    double material_cost;
    double labor_cost;
    int    successful;
    int    active;
} experiment_t;

typedef struct {
    int    paper_id;
    int    exp_id;
    int    journal_tier;
    int    citations;
    double impact_factor;
    int    n_authors;
    int    year;
    int    active;
} paper_t;

typedef struct {
    int    equip_id;
    int    type;
    double purchase_cost;
    double maintenance_cost;
    int    hours_used;
    double hourly_rate;
    double revenue;
    int    age_years;
    int    active;
} equipment_t;

typedef struct {
    int    grant_id;
    int    agency;
    double amount;
    int    duration_months;
    int    months_remaining;
    double spent;
    int    active;
} grant_t;

typedef struct {
    int    patent_id;
    int    exp_id;
    int    field;
    double filing_cost;
    int    licensed;
    double license_revenue;
    int    active;
} patent_t;

typedef struct {
    int    n_experiments;
    int    n_papers;
    int    n_equipment;
    int    n_grants;
    int    n_patents;
    double grant_funding;
    double industry_funding;
    double license_revenue;
    double equipment_revenue;
    double total_revenue;
    double equipment_costs;
    double material_costs;
    double labor_costs;
    double admin_costs;
    int    total_experiments;
    int    total_successful;
} rl_state_t;

static experiment_t experiments[MAX_EXPERIMENTS];
static paper_t papers[MAX_PAPERS];
static equipment_t equipment[MAX_EQUIPMENT];
static grant_t grants[MAX_GRANTS];
static patent_t patents[MAX_PATENTS];
static rl_state_t rl;

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

int rl_init(void) {
    if (initialized) return -1;
    rl.n_experiments = 0; rl.n_papers = 0; rl.n_equipment = 0;
    rl.n_grants = 0; rl.n_patents = 0;
    rl.grant_funding = 0.0; rl.industry_funding = 0.0;
    rl.license_revenue = 0.0; rl.equipment_revenue = 0.0;
    rl.total_revenue = 0.0; rl.equipment_costs = 0.0;
    rl.material_costs = 0.0; rl.labor_costs = 0.0;
    rl.admin_costs = 0.0;
    rl.total_experiments = 0; rl.total_successful = 0;
    for (int i = 0; i < MAX_EXPERIMENTS; i++) experiments[i].active = 0;
    for (int i = 0; i < MAX_PAPERS; i++) papers[i].active = 0;
    for (int i = 0; i < MAX_EQUIPMENT; i++) equipment[i].active = 0;
    for (int i = 0; i < MAX_GRANTS; i++) grants[i].active = 0;
    for (int i = 0; i < MAX_PATENTS; i++) patents[i].active = 0;
    initialized = 1;
    print_str("[RL] Research lab initialized\n");
    return 0;
}

int rl_add_equipment(int type, double cost, double hourly_rate) {
    if (rl.n_equipment >= MAX_EQUIPMENT) return -1;
    equipment_t* e = &equipment[rl.n_equipment];
    e->equip_id = rl.n_equipment;
    e->type = type;
    e->purchase_cost = cost;
    e->maintenance_cost = 0.0;
    e->hours_used = 0;
    e->hourly_rate = hourly_rate;
    e->revenue = 0.0;
    e->age_years = 0;
    e->active = 1;
    rl.equipment_costs += cost;
    rl.n_equipment++;
    print_str("[RL] Equip "); print_int(rl.n_equipment - 1);
    print_str(" type="); print_int(type);
    print_str(" cost=$"); print_int((int)cost);
    print_str(" $"); print_int((int)hourly_rate); print_str("/hr\n");
    return rl.n_equipment - 1;
}

int rl_get_grant(int agency, double amount, int months) {
    if (rl.n_grants >= MAX_GRANTS) return -1;
    grant_t* g = &grants[rl.n_grants];
    g->grant_id = rl.n_grants;
    g->agency = agency;
    g->amount = amount;
    g->duration_months = months;
    g->months_remaining = months;
    g->spent = 0.0;
    g->active = 1;
    rl.grant_funding += amount;
    rl.total_revenue += amount;
    rl.n_grants++;
    print_str("[RL] Grant "); print_int(rl.n_grants - 1);
    print_str(" agency="); print_int(agency);
    print_str(" $"); print_int((int)amount);
    print_str(" mo="); print_int(months); print_str("\n");
    return rl.n_grants - 1;
}

int rl_run_experiment(int type, int samples, int days, int equip_id, double mat_cost, double labor_cost) {
    if (rl.n_experiments >= MAX_EXPERIMENTS || equip_id >= rl.n_equipment) return -1;
    experiment_t* ex = &experiments[rl.n_experiments];
    ex->exp_id = rl.n_experiments;
    ex->type = type;
    ex->samples = samples;
    ex->duration_days = days;
    ex->equipment_id = equip_id;
    ex->material_cost = mat_cost;
    ex->labor_cost = labor_cost;
    ex->successful = (type + samples + days) % 3 != 0;
    ex->active = 1;
    rl.material_costs += mat_cost;
    rl.labor_costs += labor_cost;
    equipment[equip_id].hours_used += days * 8;
    rl.total_experiments++;
    if (ex->successful) rl.total_successful++;
    rl.n_experiments++;
    print_str("[RL] Experiment "); print_int(rl.n_experiments - 1);
    print_str(" type="); print_int(type);
    print_str(" samples="); print_int(samples);
    print_str(" days="); print_int(days);
    print_str(" eq="); print_int(equip_id);
    print_str(" ok="); print_int(ex->successful); print_str("\n");
    return rl.n_experiments - 1;
}

int rl_publish_paper(int exp_id, int journal_tier, int citations, double impact, int n_authors, int year) {
    if (rl.n_papers >= MAX_PAPERS || exp_id >= rl.n_experiments) return -1;
    paper_t* p = &papers[rl.n_papers];
    p->paper_id = rl.n_papers;
    p->exp_id = exp_id;
    p->journal_tier = journal_tier;
    p->citations = citations;
    p->impact_factor = impact;
    p->n_authors = n_authors;
    p->year = year;
    p->active = 1;
    rl.n_papers++;
    print_str("[RL] Paper "); print_int(rl.n_papers - 1);
    print_str(" E"); print_int(exp_id);
    print_str(" tier="); print_int(journal_tier);
    print_str(" cites="); print_int(citations);
    print_str(" IF="); print_int((int)(impact * 10)); print_str("\n");
    return rl.n_papers - 1;
}

int rl_use_equipment(int equip_id, int hours) {
    if (equip_id >= rl.n_equipment) return -1;
    equipment_t* e = &equipment[equip_id];
    e->hours_used += hours;
    double rev = hours * e->hourly_rate;
    e->revenue += rev;
    rl.equipment_revenue += rev;
    rl.total_revenue += rev;
    e->maintenance_cost += hours * 5.0;
    rl.equipment_costs += hours * 5.0;
    print_str("[RL] Use Eq"); print_int(equip_id);
    print_str(" hrs="); print_int(hours);
    print_str(" $"); print_int((int)rev); print_str("\n");
    return 0;
}

int rl_spend_grant(int grant_id, double amount) {
    if (grant_id >= rl.n_grants) return -1;
    grant_t* g = &grants[grant_id];
    if (g->spent + amount > g->amount) return -2;
    g->spent += amount;
    print_str("[RL] Spend G"); print_int(grant_id);
    print_str(" $"); print_int((int)amount); print_str("\n");
    return 0;
}

int rl_file_patent(int exp_id, int field, double filing_cost) {
    if (rl.n_patents >= MAX_PATENTS || exp_id >= rl.n_experiments) return -1;
    patent_t* pt = &patents[rl.n_patents];
    pt->patent_id = rl.n_patents;
    pt->exp_id = exp_id;
    pt->field = field;
    pt->filing_cost = filing_cost;
    pt->licensed = 0;
    pt->license_revenue = 0.0;
    pt->active = 1;
    rl.equipment_costs += filing_cost;
    rl.n_patents++;
    print_str("[RL] Patent "); print_int(rl.n_patents - 1);
    print_str(" E"); print_int(exp_id);
    print_str(" field="); print_int(field);
    print_str(" $"); print_int((int)filing_cost); print_str("\n");
    return rl.n_patents - 1;
}

int rl_license_patent(int patent_id, double revenue) {
    if (patent_id >= rl.n_patents) return -1;
    patents[patent_id].licensed = 1;
    patents[patent_id].license_revenue += revenue;
    rl.license_revenue += revenue;
    rl.total_revenue += revenue;
    print_str("[RL] License Pt"); print_int(patent_id);
    print_str(" $"); print_int((int)revenue); print_str("\n");
    return 0;
}

int rl_add_industry_funding(double amount) {
    rl.industry_funding += amount;
    rl.total_revenue += amount;
    print_str("[RL] Industry +$"); print_int((int)amount); print_str("\n");
    return 0;
}

int rl_add_admin_cost(double amount) {
    rl.admin_costs += amount;
    print_str("[RL] Admin +$"); print_int((int)amount); print_str("\n");
    return 0;
}

void rl_experiment_report(void) {
    print_str("[RL] Experiment report:\n");
    for (int i = 0; i < rl.n_experiments; i++) {
        experiment_t* ex = &experiments[i];
        print_str("  E"); print_int(i);
        print_str(" type="); print_int(ex->type);
        print_str(" samples="); print_int(ex->samples);
        print_str(" days="); print_int(ex->duration_days);
        print_str(" ok="); print_int(ex->successful); print_str("\n");
    }
    for (int i = 0; i < rl.n_papers; i++) {
        paper_t* p = &papers[i];
        print_str("  P"); print_int(i);
        print_str(" E"); print_int(p->exp_id);
        print_str(" tier="); print_int(p->journal_tier);
        print_str(" cites="); print_int(p->citations);
        print_str(" IF="); print_int((int)(p->impact_factor * 10)); print_str("\n");
    }
}

void rl_financial_report(void) {
    print_str("[RL] Financial report:\n");
    print_str("  Grant funding: "); print_int((int)rl.grant_funding); print_str("\n");
    print_str("  Industry funding: "); print_int((int)rl.industry_funding); print_str("\n");
    print_str("  License revenue: "); print_int((int)rl.license_revenue); print_str("\n");
    print_str("  Equipment revenue: "); print_int((int)rl.equipment_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)rl.total_revenue); print_str("\n");
    print_str("  Equipment costs: "); print_int((int)rl.equipment_costs); print_str("\n");
    print_str("  Material costs: "); print_int((int)rl.material_costs); print_str("\n");
    print_str("  Labor costs: "); print_int((int)rl.labor_costs); print_str("\n");
    print_str("  Admin costs: "); print_int((int)rl.admin_costs); print_str("\n");
    double total_costs = rl.equipment_costs + rl.material_costs + rl.labor_costs + rl.admin_costs;
    double net = rl.total_revenue - total_costs;
    print_str("  Net: "); print_int((int)net); print_str("\n");
    print_str("  Experiments: "); print_int(rl.total_experiments);
    print_str(" Success: "); print_int(rl.total_successful);
    print_str(" Papers: "); print_int(rl.n_papers);
    print_str(" Patents: "); print_int(rl.n_patents); print_str("\n");
}

void rl_print_state(void) {
    print_str("[RL] Exp="); print_int(rl.n_experiments);
    print_str(" Papers="); print_int(rl.n_papers);
    print_str(" Equip="); print_int(rl.n_equipment);
    print_str(" Grants="); print_int(rl.n_grants);
    print_str(" Patents="); print_int(rl.n_patents);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)rl.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Research Lab Demo ===\n\n");
    rl_init();

    print_str("Adding equipment...\n");
    rl_add_equipment(1, 500000.0, 150.0);
    rl_add_equipment(2, 350000.0, 120.0);
    rl_add_equipment(3, 800000.0, 200.0);
    rl_add_equipment(1, 450000.0, 140.0);
    rl_add_equipment(4, 250000.0, 100.0);
    rl_add_equipment(2, 380000.0, 130.0);
    rl_add_equipment(3, 700000.0, 180.0);
    rl_add_equipment(5, 150000.0, 80.0);
    rl_add_equipment(1, 420000.0, 135.0);
    rl_add_equipment(4, 280000.0, 110.0);
    rl_add_equipment(2, 360000.0, 125.0);
    rl_add_equipment(5, 180000.0, 85.0);

    print_str("\nGetting grants...\n");
    rl_get_grant(1, 1000000.0, 36);
    rl_get_grant(2, 750000.0, 24);
    rl_get_grant(1, 1200000.0, 48);
    rl_get_grant(3, 500000.0, 18);
    rl_get_grant(2, 800000.0, 30);
    rl_get_grant(4, 400000.0, 12);
    rl_get_grant(1, 900000.0, 36);
    rl_get_grant(3, 600000.0, 24);
    rl_get_grant(2, 850000.0, 30);
    rl_get_grant(4, 350000.0, 12);

    print_str("\nRunning experiments...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 4) + 1;
        int samples = 10 + (i * 5);
        int days = 5 + (i % 10);
        int equip = i % 12;
        double mat = 5000.0 + (i * 1000);
        double lab = 3000.0 + (i * 800);
        rl_run_experiment(type, samples, days, equip, mat, lab);
    }

    print_str("\nPublishing papers...\n");
    for (int i = 0; i < 14; i++) {
        if (i < 16 && experiments[i].successful) {
            int tier = (i % 3) + 1;
            int cites = 20 + (i * 10);
            double impact = 2.5 + (i % 5) * 1.5;
            int authors = 2 + (i % 4);
            int year = 2024;
            rl_publish_paper(i, tier, cites, impact, authors, year);
        }
    }

    print_str("\nEquipment usage...\n");
    for (int i = 0; i < 12; i++) {
        rl_use_equipment(i, 100 + (i * 20));
    }

    print_str("\nSpending grants...\n");
    for (int i = 0; i < 10; i++) {
        rl_spend_grant(i, 50000.0 + (i * 20000));
    }

    print_str("\nFiling patents...\n");
    for (int i = 0; i < 8; i++) {
        if (i < 16 && experiments[i].successful) {
            int field = (i % 4) + 1;
            double cost = 15000.0 + (i * 2000);
            rl_file_patent(i, field, cost);
        }
    }

    print_str("\nLicensing patents...\n");
    rl_license_patent(0, 200000.0);
    rl_license_patent(1, 150000.0);
    rl_license_patent(2, 300000.0);
    rl_license_patent(3, 180000.0);
    rl_license_patent(4, 250000.0);
    rl_license_patent(5, 120000.0);

    print_str("\nIndustry funding...\n");
    rl_add_industry_funding(300000.0);
    rl_add_industry_funding(450000.0);
    rl_add_industry_funding(200000.0);
    rl_add_industry_funding(380000.0);
    rl_add_industry_funding(250000.0);

    print_str("\nAdmin costs...\n");
    rl_add_admin_cost(100000.0);
    rl_add_admin_cost(80000.0);
    rl_add_admin_cost(120000.0);

    print_str("\nExperiment report...\n");
    rl_experiment_report();

    print_str("\nFinancial report...\n");
    rl_financial_report();

    print_str("\nFinal state...\n");
    rl_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
