/* tax_authority: Tax management (v1.0)
 * Taxpayers, filings, audits, refunds, revenue analysis
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

#define MAX_TAXPAYERS 20
#define MAX_FILINGS   16
#define MAX_AUDITS    10
#define MAX_REFUNDS   8

typedef struct {
    int    tp_id;
    int    type;
    int    category;
    double annual_income;
    double tax_paid;
    int    n_filings;
    int    compliance_score;
    int    active;
} taxpayer_t;

typedef struct {
    int    filing_id;
    int    tp_id;
    int    tax_type;
    double gross;
    double taxable;
    double tax_due;
    double paid;
    int    quarter;
    int    filed;
    int    active;
} filing_t;

typedef struct {
    int    audit_id;
    int    tp_id;
    int    year;
    double discrepancy;
    double penalty;
    int    resolved;
    int    day;
    int    active;
} audit_t;

typedef struct {
    int    refund_id;
    int    tp_id;
    int    type;
    double amount;
    int    approved;
    int    processed;
    int    active;
} refund_t;

typedef struct {
    int    n_taxpayers;
    int    n_filings;
    int    n_audits;
    int    n_refunds;
    double income_tax;
    double vat_collected;
    double corporate_tax;
    double excise_tax;
    double total_collected;
    double refunds_issued;
    double penalties_collected;
    double net_revenue;
    int    total_compliant;
    int    total_audits_done;
} ta_state_t;

static taxpayer_t taxpayers[MAX_TAXPAYERS];
static filing_t filings[MAX_FILINGS];
static audit_t audits[MAX_AUDITS];
static refund_t refunds[MAX_REFUNDS];
static ta_state_t ta;

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

int ta_init(void) {
    if (initialized) return -1;
    ta.n_taxpayers = 0; ta.n_filings = 0;
    ta.n_audits = 0; ta.n_refunds = 0;
    ta.income_tax = 0.0; ta.vat_collected = 0.0;
    ta.corporate_tax = 0.0; ta.excise_tax = 0.0;
    ta.total_collected = 0.0; ta.refunds_issued = 0.0;
    ta.penalties_collected = 0.0; ta.net_revenue = 0.0;
    ta.total_compliant = 0; ta.total_audits_done = 0;
    for (int i = 0; i < MAX_TAXPAYERS; i++) taxpayers[i].active = 0;
    for (int i = 0; i < MAX_FILINGS; i++) filings[i].active = 0;
    for (int i = 0; i < MAX_AUDITS; i++) audits[i].active = 0;
    for (int i = 0; i < MAX_REFUNDS; i++) refunds[i].active = 0;
    initialized = 1;
    print_str("[TA] Tax authority initialized\n");
    return 0;
}

int ta_register_taxpayer(int type, int category, double income) {
    if (ta.n_taxpayers >= MAX_TAXPAYERS) return -1;
    taxpayer_t* tp = &taxpayers[ta.n_taxpayers];
    tp->tp_id = ta.n_taxpayers;
    tp->type = type;
    tp->category = category;
    tp->annual_income = income;
    tp->tax_paid = 0.0;
    tp->n_filings = 0;
    tp->compliance_score = 100;
    tp->active = 1;
    ta.n_taxpayers++;
    print_str("[TA] Taxpayer "); print_int(ta.n_taxpayers - 1);
    print_str(" type="); print_int(type);
    print_str(" cat="); print_int(category);
    print_str(" income=$"); print_int((int)income); print_str("\n");
    return ta.n_taxpayers - 1;
}

int ta_file_return(int tp_id, int tax_type, double gross, double taxable, int quarter) {
    if (ta.n_filings >= MAX_FILINGS || tp_id >= ta.n_taxpayers) return -1;
    filing_t* f = &filings[ta.n_filings];
    f->filing_id = ta.n_filings;
    f->tp_id = tp_id;
    f->tax_type = tax_type;
    f->gross = gross;
    f->taxable = taxable;
    f->tax_due = 0.0;
    f->paid = 0.0;
    f->quarter = quarter;
    f->filed = 1;
    f->active = 1;
    taxpayers[tp_id].n_filings++;
    ta.n_filings++;
    print_str("[TA] Filing "); print_int(ta.n_filings - 1);
    print_str(" Tp"); print_int(tp_id);
    print_str(" type="); print_int(tax_type);
    print_str(" gross=$"); print_int((int)gross);
    print_str(" taxable=$"); print_int((int)taxable); print_str("\n");
    return ta.n_filings - 1;
}

int ta_calculate_tax(int filing_id, double rate) {
    if (filing_id >= ta.n_filings) return -1;
    filing_t* f = &filings[filing_id];
    f->tax_due = f->taxable * rate;
    f->paid = f->tax_due;
    taxpayers[f->tp_id].tax_paid += f->tax_due;
    if (f->tax_type == 1) ta.income_tax += f->tax_due;
    else if (f->tax_type == 2) ta.vat_collected += f->tax_due;
    else if (f->tax_type == 3) ta.corporate_tax += f->tax_due;
    else ta.excise_tax += f->tax_due;
    ta.total_collected += f->tax_due;
    ta.net_revenue += f->tax_due;
    print_str("[TA] Tax Fl"); print_int(filing_id);
    print_str(" due=$"); print_int((int)f->tax_due); print_str("\n");
    return 0;
}

int ta_schedule_audit(int tp_id, int year) {
    if (ta.n_audits >= MAX_AUDITS || tp_id >= ta.n_taxpayers) return -1;
    audit_t* a = &audits[ta.n_audits];
    a->audit_id = ta.n_audits;
    a->tp_id = tp_id;
    a->year = year;
    a->discrepancy = 0.0;
    a->penalty = 0.0;
    a->resolved = 0;
    a->day = 30 + ta.n_audits;
    a->active = 1;
    ta.n_audits++;
    print_str("[TA] Audit "); print_int(ta.n_audits - 1);
    print_str(" Tp"); print_int(tp_id);
    print_str(" yr="); print_int(year); print_str("\n");
    return ta.n_audits - 1;
}

int ta_record_discrepancy(int audit_id, double discrepancy, double penalty) {
    if (audit_id >= ta.n_audits) return -1;
    audit_t* a = &audits[audit_id];
    a->discrepancy = discrepancy;
    a->penalty = penalty;
    taxpayers[a->tp_id].compliance_score -= (int)(penalty / 100);
    print_str("[TA] Discr A"); print_int(audit_id);
    print_str(" disc=$"); print_int((int)discrepancy);
    print_str(" pen=$"); print_int((int)penalty); print_str("\n");
    return 0;
}

int ta_resolve_audit(int audit_id) {
    if (audit_id >= ta.n_audits) return -1;
    audit_t* a = &audits[audit_id];
    a->resolved = 1;
    double collected = a->discrepancy + a->penalty;
    ta.penalties_collected += a->penalty;
    ta.total_collected += a->discrepancy;
    ta.net_revenue += collected;
    ta.total_audits_done++;
    print_str("[TA] Resolve A"); print_int(audit_id);
    print_str(" +$"); print_int((int)collected); print_str("\n");
    return 0;
}

int ta_request_refund(int tp_id, int type, double amount) {
    if (ta.n_refunds >= MAX_REFUNDS || tp_id >= ta.n_taxpayers) return -1;
    refund_t* r = &refunds[ta.n_refunds];
    r->refund_id = ta.n_refunds;
    r->tp_id = tp_id;
    r->type = type;
    r->amount = amount;
    r->approved = 0;
    r->processed = 0;
    r->active = 1;
    ta.n_refunds++;
    print_str("[TA] Refund "); print_int(ta.n_refunds - 1);
    print_str(" Tp"); print_int(tp_id);
    print_str(" type="); print_int(type);
    print_str(" $"); print_int((int)amount); print_str("\n");
    return ta.n_refunds - 1;
}

int ta_approve_refund(int refund_id) {
    if (refund_id >= ta.n_refunds) return -1;
    refund_t* r = &refunds[refund_id];
    r->approved = 1;
    r->processed = 1;
    ta.refunds_issued += r->amount;
    ta.net_revenue -= r->amount;
    print_str("[TA] Approve R"); print_int(refund_id);
    print_str(" $"); print_int((int)r->amount); print_str("\n");
    return 0;
}

void ta_taxpayer_report(void) {
    print_str("[TA] Taxpayer report:\n");
    for (int i = 0; i < ta.n_taxpayers; i++) {
        taxpayer_t* tp = &taxpayers[i];
        print_str("  Tp"); print_int(i);
        print_str(" type="); print_int(tp->type);
        print_str(" income=$"); print_int((int)tp->annual_income);
        print_str(" paid=$"); print_int((int)tp->tax_paid);
        print_str(" score="); print_int(tp->compliance_score); print_str("\n");
    }
}

void ta_revenue_report(void) {
    print_str("[TA] Revenue report:\n");
    print_str("  Income tax: "); print_int((int)ta.income_tax); print_str("\n");
    print_str("  VAT: "); print_int((int)ta.vat_collected); print_str("\n");
    print_str("  Corporate tax: "); print_int((int)ta.corporate_tax); print_str("\n");
    print_str("  Excise tax: "); print_int((int)ta.excise_tax); print_str("\n");
    print_str("  Total collected: "); print_int((int)ta.total_collected); print_str("\n");
    print_str("  Refunds: "); print_int((int)ta.refunds_issued); print_str("\n");
    print_str("  Penalties: "); print_int((int)ta.penalties_collected); print_str("\n");
    print_str("  Net revenue: "); print_int((int)ta.net_revenue); print_str("\n");
    print_str("  Taxpayers: "); print_int(ta.n_taxpayers);
    print_str(" Audits: "); print_int(ta.total_audits_done); print_str("\n");
}

void ta_print_state(void) {
    print_str("[TA] Taxpayers="); print_int(ta.n_taxpayers);
    print_str(" Filings="); print_int(ta.n_filings);
    print_str(" Audits="); print_int(ta.n_audits);
    print_str(" Refunds="); print_int(ta.n_refunds);
    print_str("\n");
    print_str("  Net revenue: "); print_int((int)ta.net_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Tax Authority Demo ===\n\n");
    ta_init();

    print_str("Registering taxpayers...\n");
    ta_register_taxpayer(1, 1, 50000.0);
    ta_register_taxpayer(1, 2, 75000.0);
    ta_register_taxpayer(2, 3, 500000.0);
    ta_register_taxpayer(2, 3, 800000.0);
    ta_register_taxpayer(1, 1, 45000.0);
    ta_register_taxpayer(1, 2, 90000.0);
    ta_register_taxpayer(2, 4, 1200000.0);
    ta_register_taxpayer(2, 3, 650000.0);
    ta_register_taxpayer(1, 1, 60000.0);
    ta_register_taxpayer(2, 5, 2000000.0);
    ta_register_taxpayer(1, 2, 85000.0);
    ta_register_taxpayer(2, 3, 700000.0);
    ta_register_taxpayer(1, 1, 55000.0);
    ta_register_taxpayer(1, 2, 70000.0);
    ta_register_taxpayer(2, 4, 950000.0);
    ta_register_taxpayer(2, 5, 1500000.0);
    ta_register_taxpayer(1, 1, 48000.0);
    ta_register_taxpayer(1, 2, 82000.0);
    ta_register_taxpayer(2, 3, 600000.0);
    ta_register_taxpayer(2, 4, 1100000.0);

    print_str("\nFiling returns...\n");
    for (int i = 0; i < 16; i++) {
        int tp = i % 20;
        int type = (i % 4) + 1;
        double gross = 20000.0 + (i * 15000);
        double taxable = gross * 0.7;
        int quarter = (i % 4) + 1;
        ta_file_return(tp, type, gross, taxable, quarter);
    }

    print_str("\nCalculating taxes...\n");
    for (int i = 0; i < 16; i++) {
        double rate = 0.15 + (i % 5) * 0.05;
        ta_calculate_tax(i, rate);
    }

    print_str("\nScheduling audits...\n");
    for (int i = 0; i < 10; i++) {
        ta_schedule_audit(i % 20, 2024 - (i % 3));
    }

    print_str("\nRecording discrepancies...\n");
    for (int i = 0; i < 10; i++) {
        double disc = 5000.0 + (i * 3000);
        double pen = disc * 0.2;
        ta_record_discrepancy(i, disc, pen);
    }

    print_str("\nResolving audits...\n");
    for (int i = 0; i < 10; i++) {
        ta_resolve_audit(i);
    }

    print_str("\nRefund requests...\n");
    ta_request_refund(0, 1, 2000.0);
    ta_request_refund(3, 2, 5000.0);
    ta_request_refund(6, 1, 8000.0);
    ta_request_refund(9, 3, 15000.0);
    ta_request_refund(12, 2, 3000.0);
    ta_request_refund(15, 1, 4000.0);
    ta_request_refund(18, 3, 10000.0);
    ta_request_refund(1, 2, 2500.0);

    print_str("\nApproving refunds...\n");
    for (int i = 0; i < 8; i++) {
        ta_approve_refund(i);
    }

    print_str("\nTaxpayer report...\n");
    ta_taxpayer_report();

    print_str("\nRevenue report...\n");
    ta_revenue_report();

    print_str("\nFinal state...\n");
    ta_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
