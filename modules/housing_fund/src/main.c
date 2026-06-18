/* housing_fund: Housing provident fund administration system (v1.0)
 * Fund deposit, withdrawal, loans, unit management, risk monitoring
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

#define MAX_DEPOSIT      16
#define MAX_WITHDRAWAL   14
#define MAX_LOAN         12
#define MAX_UNIT         10
#define MAX_RISK         10

typedef struct {
    int    deposit_id;
    int    deposit_type;
    int    participant_category;
    int    accounts_opened;
    int    deposits_collected;
    int    interest_accrued;
    int    year;
    int    active;
} deposit_t;

typedef struct {
    int    withdrawal_id;
    int    withdrawal_type;
    int    reason_category;
    int    applications_received;
    int    applications_approved;
    int    amount_withdrawn;
    int    year;
    int    active;
} withdrawal_t;

typedef struct {
    int    loan_id;
    int    loan_type;
    int    loan_purpose;
    int    loans_approved;
    int    amount_lent;
    int    loans_overdue;
    int    year;
    int    active;
} loan_t;

typedef struct {
    int    unit_id;
    int    unit_type;
    int    industry_sector;
    int    units_registered;
    int    employees_covered;
    int    contributions_made;
    int    year;
    int    active;
} unit_t;

typedef struct {
    int    risk_id;
    int    risk_type;
    int    risk_level;
    int    risks_identified;
    int    warnings_issued;
    int    cases_resolved;
    int    year;
    int    active;
} risk_t;

typedef struct {
    int    n_deposit;
    int    n_withdrawal;
    int    n_loan;
    int    n_unit;
    int    n_risk;
    int    total_accounts;
    int    total_deposits;
    int    total_loans;
    int    total_employees;
    int    total_risks;
} hf_state_t;

static deposit_t deposits[MAX_DEPOSIT];
static withdrawal_t withdrawals[MAX_WITHDRAWAL];
static loan_t loans[MAX_LOAN];
static unit_t units[MAX_UNIT];
static risk_t risks[MAX_RISK];
static hf_state_t hf;

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

int hf_init(void) {
    if (initialized) return -1;
    hf.n_deposit = 0; hf.n_withdrawal = 0; hf.n_loan = 0;
    hf.n_unit = 0; hf.n_risk = 0;
    hf.total_accounts = 0; hf.total_deposits = 0;
    hf.total_loans = 0; hf.total_employees = 0;
    hf.total_risks = 0;
    for (int i = 0; i < MAX_DEPOSIT; i++) deposits[i].active = 0;
    for (int i = 0; i < MAX_WITHDRAWAL; i++) withdrawals[i].active = 0;
    for (int i = 0; i < MAX_LOAN; i++) loans[i].active = 0;
    for (int i = 0; i < MAX_UNIT; i++) units[i].active = 0;
    for (int i = 0; i < MAX_RISK; i++) risks[i].active = 0;
    initialized = 1;
    print_str("[HF] Housing fund initialized\n");
    return 0;
}

int hf_deposit(int dep_type, int part_cat, int opened, int collected, int interest, int year) {
    if (hf.n_deposit >= MAX_DEPOSIT) return -1;
    deposit_t* d = &deposits[hf.n_deposit];
    d->deposit_id = hf.n_deposit;
    d->deposit_type = dep_type;
    d->participant_category = part_cat;
    d->accounts_opened = opened;
    d->deposits_collected = collected;
    d->interest_accrued = interest;
    d->year = year;
    d->active = 1;
    hf.total_accounts += opened;
    hf.total_deposits += collected;
    hf.n_deposit++;
    print_str("[HF] Deposit "); print_int(hf.n_deposit - 1);
    print_str(" type="); print_int(dep_type);
    print_str(" cat="); print_int(part_cat);
    print_str(" opn="); print_int(opened);
    print_str(" col="); print_int(collected);
    print_str(" int="); print_int(interest); print_str("\n");
    return hf.n_deposit - 1;
}

int hf_withdrawal(int wd_type, int reason, int received, int approved, int amount, int year) {
    if (hf.n_withdrawal >= MAX_WITHDRAWAL) return -1;
    withdrawal_t* w = &withdrawals[hf.n_withdrawal];
    w->withdrawal_id = hf.n_withdrawal;
    w->withdrawal_type = wd_type;
    w->reason_category = reason;
    w->applications_received = received;
    w->applications_approved = approved;
    w->amount_withdrawn = amount;
    w->year = year;
    w->active = 1;
    hf.n_withdrawal++;
    print_str("[HF] Withdrawal "); print_int(hf.n_withdrawal - 1);
    print_str(" type="); print_int(wd_type);
    print_str(" rsn="); print_int(reason);
    print_str(" rcv="); print_int(received);
    print_str(" apr="); print_int(approved);
    print_str(" amt="); print_int(amount); print_str("\n");
    return hf.n_withdrawal - 1;
}

int hf_loan(int loan_type, int purpose, int approved_cnt, int amount, int overdue, int year) {
    if (hf.n_loan >= MAX_LOAN) return -1;
    loan_t* l = &loans[hf.n_loan];
    l->loan_id = hf.n_loan;
    l->loan_type = loan_type;
    l->loan_purpose = purpose;
    l->loans_approved = approved_cnt;
    l->amount_lent = amount;
    l->loans_overdue = overdue;
    l->year = year;
    l->active = 1;
    hf.total_loans += approved_cnt;
    hf.n_loan++;
    print_str("[HF] Loan "); print_int(hf.n_loan - 1);
    print_str(" type="); print_int(loan_type);
    print_str(" pur="); print_int(purpose);
    print_str(" apr="); print_int(approved_cnt);
    print_str(" amt="); print_int(amount);
    print_str(" ovd="); print_int(overdue); print_str("\n");
    return hf.n_loan - 1;
}

int hf_unit(int unit_type, int sector, int registered, int employees_cnt, int contributions, int year) {
    if (hf.n_unit >= MAX_UNIT) return -1;
    unit_t* u = &units[hf.n_unit];
    u->unit_id = hf.n_unit;
    u->unit_type = unit_type;
    u->industry_sector = sector;
    u->units_registered = registered;
    u->employees_covered = employees_cnt;
    u->contributions_made = contributions;
    u->year = year;
    u->active = 1;
    hf.total_employees += employees_cnt;
    hf.n_unit++;
    print_str("[HF] Unit "); print_int(hf.n_unit - 1);
    print_str(" type="); print_int(unit_type);
    print_str(" sec="); print_int(sector);
    print_str(" reg="); print_int(registered);
    print_str(" emp="); print_int(employees_cnt);
    print_str(" cnt="); print_int(contributions); print_str("\n");
    return hf.n_unit - 1;
}

int hf_risk(int risk_type, int level, int identified, int warnings, int resolved, int year) {
    if (hf.n_risk >= MAX_RISK) return -1;
    risk_t* r = &risks[hf.n_risk];
    r->risk_id = hf.n_risk;
    r->risk_type = risk_type;
    r->risk_level = level;
    r->risks_identified = identified;
    r->warnings_issued = warnings;
    r->cases_resolved = resolved;
    r->year = year;
    r->active = 1;
    hf.total_risks += identified;
    hf.n_risk++;
    print_str("[HF] Risk "); print_int(hf.n_risk - 1);
    print_str(" type="); print_int(risk_type);
    print_str(" lvl="); print_int(level);
    print_str(" idf="); print_int(identified);
    print_str(" wrn="); print_int(warnings);
    print_str(" rsl="); print_int(resolved); print_str("\n");
    return hf.n_risk - 1;
}

void hf_deposit_report(void) {
    print_str("[HF] Deposit report:\n");
    print_str("  Deposit categories: "); print_int(hf.n_deposit); print_str("\n");
    print_str("  Total accounts: "); print_int(hf.total_accounts); print_str("\n");
    print_str("  Total deposits: "); print_int(hf.total_deposits); print_str("\n");
}

void hf_loan_report(void) {
    print_str("[HF] Loan report:\n");
    print_str("  Withdrawal categories: "); print_int(hf.n_withdrawal); print_str("\n");
    print_str("  Loan categories: "); print_int(hf.n_loan); print_str("\n");
    print_str("  Total loans approved: "); print_int(hf.total_loans); print_str("\n");
}

void hf_risk_report(void) {
    print_str("[HF] Risk report:\n");
    print_str("  Registered units: "); print_int(hf.n_unit); print_str("\n");
    print_str("  Total employees covered: "); print_int(hf.total_employees); print_str("\n");
    print_str("  Risk categories: "); print_int(hf.n_risk); print_str("\n");
    print_str("  Total risks identified: "); print_int(hf.total_risks); print_str("\n");
}

void hf_print_state(void) {
    print_str("[HF] Dp="); print_int(hf.n_deposit);
    print_str(" Wd="); print_int(hf.n_withdrawal);
    print_str(" Ln="); print_int(hf.n_loan);
    print_str(" Un="); print_int(hf.n_unit);
    print_str(" Rk="); print_int(hf.n_risk);
    print_str("\n");
}

int main(void) {
    print_str("=== Housing Fund Demo ===\n\n");
    hf_init();

    print_str("Deposit management...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int opn = 5000 + (i * 1000);
        int col = 10000 + (i * 2000);
        int intr = 200 + (i * 50);
        int year = 2020 + (i % 5);
        hf_deposit(type, cat, opn, col, intr, year);
    }

    print_str("\nWithdrawal processing...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 5) + 1;
        int rsn = (i % 4) + 1;
        int rcv = 3000 + (i * 600);
        int apr = 2500 + (i * 500);
        int amt = 5000 + (i * 1000);
        int year = 2021 + (i % 4);
        hf_withdrawal(type, rsn, rcv, apr, amt, year);
    }

    print_str("\nLoan management...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int pur = (i % 3) + 1;
        int apr = 1000 + (i * 200);
        int amt = 2000 + (i * 500);
        int ovd = 20 + (i * 5);
        int year = 2022 + (i % 3);
        hf_loan(type, pur, apr, amt, ovd, year);
    }

    print_str("\nUnit management...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int sec = (i % 5) + 1;
        int reg = 500 + (i * 100);
        int emp = 10000 + (i * 2000);
        int cnt = 800 + (i * 150);
        int year = 2023 + (i % 2);
        hf_unit(type, sec, reg, emp, cnt, year);
    }

    print_str("\nRisk monitoring...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int lvl = (i % 5) + 1;
        int idf = 30 + (i * 8);
        int wrn = 15 + (i * 4);
        int rsl = 10 + (i * 3);
        int year = 2024;
        hf_risk(type, lvl, idf, wrn, rsl, year);
    }

    print_str("\nDeposit report...\n");
    hf_deposit_report();

    print_str("\nLoan report...\n");
    hf_loan_report();

    print_str("\nRisk report...\n");
    hf_risk_report();

    print_str("\nFinal state...\n");
    hf_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
