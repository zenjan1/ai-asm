/* charity: Charity management system (v1.0)
 * Fundraising, projects, funds, volunteers, transparency
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

#define MAX_FUNDRAISE    16
#define MAX_PROJECT      14
#define MAX_FUND         12
#define MAX_VOLUNTEER    10
#define MAX_TRANSP       10

typedef struct {
    int    fund_id;
    int    campaign_id;
    int    type;
    int    donor_count;
    int    amount_raised;
    int    goal;
    int    year;
    int    status;
    int    active;
} fundraising_t;

typedef struct {
    int    project_id;
    int    name_id;
    int    category;
    int    beneficiaries;
    int    budget;
    int    spent;
    int    completion_pct;
    int    year;
    int    active;
} project_t;

typedef struct {
    int    fund_mgr_id;
    int    source_id;
    int    amount;
    int    investment_return;
    int    administrative;
    int    disbursed;
    int    year;
    int    active;
} fund_mgr_t;

typedef struct {
    int    vol_id;
    int    person_id;
    int    project_id;
    int    hours;
    int    category;
    int    recognition;
    int    year;
    int    active;
} volunteer_t;

typedef struct {
    int    transp_id;
    int    report_type;
    int    auditor_id;
    int    total_income;
    int    total_expense;
    int    rating;
    int    public_score;
    int    year;
    int    active;
} transparency_t;

typedef struct {
    int    n_fundraise;
    int    n_project;
    int    n_fund;
    int    n_volunteer;
    int    n_transp;
    int    total_raised;
    int    total_beneficiaries;
    int    total_disbursed;
    int    total_hours;
    int    total_expense;
} ch_state_t;

static fundraising_t fundraisings[MAX_FUNDRAISE];
static project_t projects[MAX_PROJECT];
static fund_mgr_t funds[MAX_FUND];
static volunteer_t volunteers[MAX_VOLUNTEER];
static transparency_t transparency[MAX_TRANSP];
static ch_state_t ch;

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

int ch_init(void) {
    if (initialized) return -1;
    ch.n_fundraise = 0; ch.n_project = 0; ch.n_fund = 0;
    ch.n_volunteer = 0; ch.n_transp = 0;
    ch.total_raised = 0; ch.total_beneficiaries = 0;
    ch.total_disbursed = 0; ch.total_hours = 0;
    ch.total_expense = 0;
    for (int i = 0; i < MAX_FUNDRAISE; i++) fundraisings[i].active = 0;
    for (int i = 0; i < MAX_PROJECT; i++) projects[i].active = 0;
    for (int i = 0; i < MAX_FUND; i++) funds[i].active = 0;
    for (int i = 0; i < MAX_VOLUNTEER; i++) volunteers[i].active = 0;
    for (int i = 0; i < MAX_TRANSP; i++) transparency[i].active = 0;
    initialized = 1;
    print_str("[CH] Charity initialized\n");
    return 0;
}

int ch_fundraise(int campaign, int type, int donors, int amount, int goal, int year) {
    if (ch.n_fundraise >= MAX_FUNDRAISE) return -1;
    fundraising_t* f = &fundraisings[ch.n_fundraise];
    f->fund_id = ch.n_fundraise;
    f->campaign_id = campaign;
    f->type = type;
    f->donor_count = donors;
    f->amount_raised = amount;
    f->goal = goal;
    f->year = year;
    f->status = 1;
    f->active = 1;
    ch.total_raised += amount;
    ch.n_fundraise++;
    print_str("[CH] Fundraise "); print_int(ch.n_fundraise - 1);
    print_str(" cmp="); print_int(campaign);
    print_str(" type="); print_int(type);
    print_str(" dnr="); print_int(donors);
    print_str(" amt=$"); print_int(amount);
    print_str(" gol=$"); print_int(goal); print_str("\n");
    return ch.n_fundraise - 1;
}

int ch_project(int name, int category, int beneficiaries, int budget, int spent, int completion, int year) {
    if (ch.n_project >= MAX_PROJECT) return -1;
    project_t* p = &projects[ch.n_project];
    p->project_id = ch.n_project;
    p->name_id = name;
    p->category = category;
    p->beneficiaries = beneficiaries;
    p->budget = budget;
    p->spent = spent;
    p->completion_pct = completion;
    p->year = year;
    p->active = 1;
    ch.total_beneficiaries += beneficiaries;
    ch.total_expense += spent;
    ch.n_project++;
    print_str("[CH] Project "); print_int(ch.n_project - 1);
    print_str(" nm="); print_int(name);
    print_str(" cat="); print_int(category);
    print_str(" ben="); print_int(beneficiaries);
    print_str(" bgt=$"); print_int(budget);
    print_str(" spn=$"); print_int(spent);
    print_str(" cmp="); print_int(completion); print_str("%\n");
    return ch.n_project - 1;
}

int ch_fund(int source, int amount, int investment, int admin, int disbursed, int year) {
    if (ch.n_fund >= MAX_FUND) return -1;
    fund_mgr_t* f = &funds[ch.n_fund];
    f->fund_mgr_id = ch.n_fund;
    f->source_id = source;
    f->amount = amount;
    f->investment_return = investment;
    f->administrative = admin;
    f->disbursed = disbursed;
    f->year = year;
    f->active = 1;
    ch.total_disbursed += disbursed;
    ch.n_fund++;
    print_str("[CH] Fund "); print_int(ch.n_fund - 1);
    print_str(" src="); print_int(source);
    print_str(" amt=$"); print_int(amount);
    print_str(" inv=$"); print_int(investment);
    print_str(" adm=$"); print_int(admin);
    print_str(" dis=$"); print_int(disbursed); print_str("\n");
    return ch.n_fund - 1;
}

int ch_volunteer(int person, int project, int hours, int category, int recognition, int year) {
    if (ch.n_volunteer >= MAX_VOLUNTEER) return -1;
    volunteer_t* v = &volunteers[ch.n_volunteer];
    v->vol_id = ch.n_volunteer;
    v->person_id = person;
    v->project_id = project;
    v->hours = hours;
    v->category = category;
    v->recognition = recognition;
    v->year = year;
    v->active = 1;
    ch.total_hours += hours;
    ch.n_volunteer++;
    print_str("[CH] Volunteer "); print_int(ch.n_volunteer - 1);
    print_str(" per="); print_int(person);
    print_str(" prj="); print_int(project);
    print_str(" hrs="); print_int(hours);
    print_str(" cat="); print_int(category);
    print_str(" rec="); print_int(recognition); print_str("\n");
    return ch.n_volunteer - 1;
}

int ch_transparency(int report, int auditor, int income, int expense, int rating, int score, int year) {
    if (ch.n_transp >= MAX_TRANSP) return -1;
    transparency_t* t = &transparency[ch.n_transp];
    t->transp_id = ch.n_transp;
    t->report_type = report;
    t->auditor_id = auditor;
    t->total_income = income;
    t->total_expense = expense;
    t->rating = rating;
    t->public_score = score;
    t->year = year;
    t->active = 1;
    ch.n_transp++;
    print_str("[CH] Transparency "); print_int(ch.n_transp - 1);
    print_str(" rpt="); print_int(report);
    print_str(" aud="); print_int(auditor);
    print_str(" inc=$"); print_int(income);
    print_str(" exp=$"); print_int(expense);
    print_str(" rtg="); print_int(rating);
    print_str(" scr="); print_int(score); print_str("\n");
    return ch.n_transp - 1;
}

void ch_fundraise_report(void) {
    print_str("[CH] Fundraising report:\n");
    print_str("  Campaigns: "); print_int(ch.n_fundraise); print_str("\n");
    print_str("  Total raised: $"); print_int(ch.total_raised); print_str("\n");
}

void ch_project_report(void) {
    print_str("[CH] Project report:\n");
    print_str("  Projects: "); print_int(ch.n_project); print_str("\n");
    print_str("  Total beneficiaries: "); print_int(ch.total_beneficiaries); print_str("\n");
    print_str("  Total spent: $"); print_int(ch.total_expense); print_str("\n");
}

void ch_transparency_report(void) {
    print_str("[CH] Transparency report:\n");
    print_str("  Fund records: "); print_int(ch.n_fund); print_str("\n");
    print_str("  Total disbursed: $"); print_int(ch.total_disbursed); print_str("\n");
    print_str("  Volunteers: "); print_int(ch.n_volunteer); print_str("\n");
    print_str("  Total hours: "); print_int(ch.total_hours); print_str("\n");
    print_str("  Transparency reports: "); print_int(ch.n_transp); print_str("\n");
}

void ch_print_state(void) {
    print_str("[CH] Fr="); print_int(ch.n_fundraise);
    print_str(" Pj="); print_int(ch.n_project);
    print_str(" Fn="); print_int(ch.n_fund);
    print_str(" Vl="); print_int(ch.n_volunteer);
    print_str(" Tr="); print_int(ch.n_transp);
    print_str("\n");
}

int main(void) {
    print_str("=== Charity Demo ===\n\n");
    ch_init();

    print_str("Fundraising campaigns...\n");
    for (int i = 0; i < 16; i++) {
        int cmp = 1000 + (i * 13);
        int type = (i % 4) + 1;
        int dnr = 50 + (i * 30);
        int amt = 10000 + (i * 5000);
        int gol = amt + 5000;
        int year = 2020 + (i % 5);
        ch_fundraise(cmp, type, dnr, amt, gol, year);
    }

    print_str("\nCharity projects...\n");
    for (int i = 0; i < 14; i++) {
        int nm = 2000 + (i * 7);
        int cat = (i % 5) + 1;
        int ben = 100 + (i * 50);
        int bgt = 50000 + (i * 20000);
        int spn = bgt - (i * 3000);
        int cmp = 60 + (i * 3);
        int year = 2021 + (i % 4);
        ch_project(nm, cat, ben, bgt, spn, cmp, year);
    }

    print_str("\nFund management...\n");
    for (int i = 0; i < 12; i++) {
        int src = 3000 + (i * 11);
        int amt = 20000 + (i * 10000);
        int inv = amt / 10;
        int adm = amt / 20;
        int dis = amt - inv - adm;
        int year = 2022 + (i % 3);
        ch_fund(src, amt, inv, adm, dis, year);
    }

    print_str("\nVolunteer services...\n");
    for (int i = 0; i < 10; i++) {
        int per = 4000 + (i * 17);
        int prj = 100 + (i * 5);
        int hrs = 20 + (i * 15);
        int cat = (i % 4) + 1;
        int rec = (i % 3 == 0) ? 1 : 0;
        int year = 2023 + (i % 2);
        ch_volunteer(per, prj, hrs, cat, rec, year);
    }

    print_str("\nTransparency reports...\n");
    for (int i = 0; i < 10; i++) {
        int rpt = (i % 3) + 1;
        int aud = 500 + (i % 6);
        int inc = 100000 + (i * 50000);
        int exp = 80000 + (i * 40000);
        int rtg = 4 + (i % 2);
        int scr = 80 + (i * 2);
        int year = 2024;
        ch_transparency(rpt, aud, inc, exp, rtg, scr, year);
    }

    print_str("\nFundraising report...\n");
    ch_fundraise_report();

    print_str("\nProject report...\n");
    ch_project_report();

    print_str("\nTransparency report...\n");
    ch_transparency_report();

    print_str("\nFinal state...\n");
    ch_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
