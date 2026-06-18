/* food_safety: Food safety management (v1.0)
 * Inspections, licenses, traceability, standards, complaints
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

#define MAX_INSPECTS   18
#define MAX_LICENSES   14
#define MAX_BATCHES    16
#define MAX_STANDARDS  10
#define MAX_COMPLAINTS 12

typedef struct {
    int    insp_id;
    int    company_id;
    int    type;
    int    result;
    int    score;
    int    findings;
    int    year;
    int    month;
    int    active;
} inspection_t;

typedef struct {
    int    lic_id;
    int    company_id;
    int    type;
    int    issued_year;
    int    expiry_year;
    int    status;
    int    active;
} license_t;

typedef struct {
    int    batch_id;
    int    product_id;
    int    company_id;
    int    quantity;
    int    produced_year;
    int    produced_month;
    int    produced_day;
    int    distributed;
    int    recalled;
    int    active;
} batch_t;

typedef struct {
    int    std_id;
    int    category;
    int    limit_value;
    int    unit;
    int    year;
    int    status;
    int    active;
} standard_t;

typedef struct {
    int    comp_id;
    int    product_id;
    int    consumer_id;
    int    type;
    int    severity;
    int    year;
    int    investigated;
    int    resolved;
    int    active;
} complaint_t;

typedef struct {
    int    n_inspections;
    int    n_licenses;
    int    n_batches;
    int    n_standards;
    int    n_complaints;
    int    total_inspections;
    int    total_pass;
    int    total_fail;
    int    total_recalled;
    int    total_complaints_resolved;
    int    avg_score;
} fs_state_t;

static inspection_t inspections[MAX_INSPECTS];
static license_t licenses[MAX_LICENSES];
static batch_t batches[MAX_BATCHES];
static standard_t standards[MAX_STANDARDS];
static complaint_t complaints[MAX_COMPLAINTS];
static fs_state_t fs;

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

int fs_init(void) {
    if (initialized) return -1;
    fs.n_inspections = 0; fs.n_licenses = 0; fs.n_batches = 0;
    fs.n_standards = 0; fs.n_complaints = 0;
    fs.total_inspections = 0; fs.total_pass = 0; fs.total_fail = 0;
    fs.total_recalled = 0; fs.total_complaints_resolved = 0;
    fs.avg_score = 0;
    for (int i = 0; i < MAX_INSPECTS; i++) inspections[i].active = 0;
    for (int i = 0; i < MAX_LICENSES; i++) licenses[i].active = 0;
    for (int i = 0; i < MAX_BATCHES; i++) batches[i].active = 0;
    for (int i = 0; i < MAX_STANDARDS; i++) standards[i].active = 0;
    for (int i = 0; i < MAX_COMPLAINTS; i++) complaints[i].active = 0;
    initialized = 1;
    print_str("[FS] Food safety initialized\n");
    return 0;
}

int fs_inspect(int company, int type, int result, int score, int findings, int year, int month) {
    if (fs.n_inspections >= MAX_INSPECTS) return -1;
    inspection_t* ins = &inspections[fs.n_inspections];
    ins->insp_id = fs.n_inspections;
    ins->company_id = company;
    ins->type = type;
    ins->result = result;
    ins->score = score;
    ins->findings = findings;
    ins->year = year;
    ins->month = month;
    ins->active = 1;
    fs.total_inspections++;
    if (result == 1) fs.total_pass++;
    else fs.total_fail++;
    fs.avg_score += score;
    fs.n_inspections++;
    print_str("[FS] Insp "); print_int(fs.n_inspections - 1);
    print_str(" Co"); print_int(company);
    print_str(" type="); print_int(type);
    print_str(" result="); print_int(result);
    print_str(" score="); print_int(score);
    print_str(" findings="); print_int(findings); print_str("\n");
    return fs.n_inspections - 1;
}

int fs_issue_license(int company, int type, int issued, int expiry) {
    if (fs.n_licenses >= MAX_LICENSES) return -1;
    license_t* l = &licenses[fs.n_licenses];
    l->lic_id = fs.n_licenses;
    l->company_id = company;
    l->type = type;
    l->issued_year = issued;
    l->expiry_year = expiry;
    l->status = 1;
    l->active = 1;
    fs.n_licenses++;
    print_str("[FS] License "); print_int(fs.n_licenses - 1);
    print_str(" Co"); print_int(company);
    print_str(" type="); print_int(type);
    print_str(" "); print_int(issued); print_str("-"); print_int(expiry); print_str("\n");
    return fs.n_licenses - 1;
}

int fs_revoke_license(int lic_id) {
    if (lic_id >= fs.n_licenses) return -1;
    licenses[lic_id].status = 2;
    print_str("[FS] Revoke L"); print_int(lic_id); print_str("\n");
    return 0;
}

int fs_record_batch(int product, int company, int qty, int year, int month, int day) {
    if (fs.n_batches >= MAX_BATCHES) return -1;
    batch_t* b = &batches[fs.n_batches];
    b->batch_id = fs.n_batches;
    b->product_id = product;
    b->company_id = company;
    b->quantity = qty;
    b->produced_year = year;
    b->produced_month = month;
    b->produced_day = day;
    b->distributed = 0;
    b->recalled = 0;
    b->active = 1;
    fs.n_batches++;
    print_str("[FS] Batch "); print_int(fs.n_batches - 1);
    print_str(" prod="); print_int(product);
    print_str(" Co"); print_int(company);
    print_str(" qty="); print_int(qty);
    print_str(" "); print_int(year); print_str("-"); print_int(month); print_str("-"); print_int(day); print_str("\n");
    return fs.n_batches - 1;
}

int fs_distribute_batch(int batch_id) {
    if (batch_id >= fs.n_batches) return -1;
    batches[batch_id].distributed = 1;
    print_str("[FS] Distribute B"); print_int(batch_id); print_str("\n");
    return 0;
}

int fs_recall_batch(int batch_id) {
    if (batch_id >= fs.n_batches) return -1;
    batches[batch_id].recalled = 1;
    fs.total_recalled++;
    print_str("[FS] Recall B"); print_int(batch_id); print_str("\n");
    return 0;
}

int fs_add_standard(int category, int limit, int unit, int year) {
    if (fs.n_standards >= MAX_STANDARDS) return -1;
    standard_t* s = &standards[fs.n_standards];
    s->std_id = fs.n_standards;
    s->category = category;
    s->limit_value = limit;
    s->unit = unit;
    s->year = year;
    s->status = 1;
    s->active = 1;
    fs.n_standards++;
    print_str("[FS] Standard "); print_int(fs.n_standards - 1);
    print_str(" cat="); print_int(category);
    print_str(" limit="); print_int(limit);
    print_str(" unit="); print_int(unit);
    print_str(" "); print_int(year); print_str("\n");
    return fs.n_standards - 1;
}

int fs_file_complaint(int product, int consumer, int type, int severity, int year) {
    if (fs.n_complaints >= MAX_COMPLAINTS) return -1;
    complaint_t* c = &complaints[fs.n_complaints];
    c->comp_id = fs.n_complaints;
    c->product_id = product;
    c->consumer_id = consumer;
    c->type = type;
    c->severity = severity;
    c->year = year;
    c->investigated = 0;
    c->resolved = 0;
    c->active = 1;
    fs.n_complaints++;
    print_str("[FS] Complaint "); print_int(fs.n_complaints - 1);
    print_str(" prod="); print_int(product);
    print_str(" cons="); print_int(consumer);
    print_str(" type="); print_int(type);
    print_str(" sev="); print_int(severity); print_str("\n");
    return fs.n_complaints - 1;
}

int fs_resolve_complaint(int comp_id) {
    if (comp_id >= fs.n_complaints) return -1;
    complaints[comp_id].investigated = 1;
    complaints[comp_id].resolved = 1;
    fs.total_complaints_resolved++;
    print_str("[FS] Resolve C"); print_int(comp_id); print_str("\n");
    return 0;
}

void fs_inspection_report(void) {
    print_str("[FS] Inspection report:\n");
    print_str("  Total: "); print_int(fs.total_inspections);
    print_str(" pass="); print_int(fs.total_pass);
    print_str(" fail="); print_int(fs.total_fail); print_str("\n");
    if (fs.total_inspections > 0) {
        print_str("  Avg score: "); print_int(fs.avg_score / fs.total_inspections); print_str("\n");
    }
    for (int i = 0; i < fs.n_inspections; i++) {
        inspection_t* ins = &inspections[i];
        print_str("  I"); print_int(i);
        print_str(" Co"); print_int(ins->company_id);
        print_str(" score="); print_int(ins->score);
        print_str(" result="); print_int(ins->result); print_str("\n");
    }
}

void fs_license_report(void) {
    print_str("[FS] Licenses: "); print_int(fs.n_licenses); print_str("\n");
    for (int i = 0; i < fs.n_licenses; i++) {
        license_t* l = &licenses[i];
        print_str("  L"); print_int(i);
        print_str(" Co"); print_int(l->company_id);
        print_str(" type="); print_int(l->type);
        print_str(" "); print_int(l->issued_year); print_str("-"); print_int(l->expiry_year);
        print_str(" st="); print_int(l->status); print_str("\n");
    }
}

void fs_traceability_report(void) {
    print_str("[FS] Traceability:\n");
    print_str("  Batches: "); print_int(fs.n_batches); print_str("\n");
    print_str("  Recalled: "); print_int(fs.total_recalled); print_str("\n");
}

void fs_complaint_report(void) {
    print_str("[FS] Complaints: "); print_int(fs.n_complaints); print_str("\n");
    print_str("  Resolved: "); print_int(fs.total_complaints_resolved); print_str("\n");
}

void fs_print_state(void) {
    print_str("[FS] Insp="); print_int(fs.n_inspections);
    print_str(" Lic="); print_int(fs.n_licenses);
    print_str(" Batch="); print_int(fs.n_batches);
    print_str(" Std="); print_int(fs.n_standards);
    print_str(" Comp="); print_int(fs.n_complaints);
    print_str("\n");
}

int main(void) {
    print_str("=== Food Safety Demo ===\n\n");
    fs_init();

    print_str("Adding standards...\n");
    fs_add_standard(1, 100, 1, 2024);
    fs_add_standard(2, 50, 2, 2024);
    fs_add_standard(3, 200, 1, 2024);
    fs_add_standard(4, 30, 3, 2024);
    fs_add_standard(5, 10, 2, 2024);
    fs_add_standard(1, 150, 1, 2024);
    fs_add_standard(2, 80, 2, 2024);
    fs_add_standard(3, 250, 1, 2024);
    fs_add_standard(4, 40, 3, 2024);
    fs_add_standard(5, 15, 2, 2024);

    print_str("\nIssuing licenses...\n");
    for (int i = 0; i < 14; i++) {
        int company = 100 + i;
        int type = (i % 3) + 1;
        int issued = 2022 + (i % 3);
        int expiry = issued + 3;
        fs_issue_license(company, type, issued, expiry);
    }

    print_str("\nConducting inspections...\n");
    for (int i = 0; i < 18; i++) {
        int company = 100 + (i % 14);
        int type = (i % 4) + 1;
        int result = (i % 5 == 0) ? 2 : 1;
        int score = 60 + (i * 3) % 40;
        int findings = i % 4;
        int year = 2024;
        int month = 1 + (i % 12);
        fs_inspect(company, type, result, score, findings, year, month);
    }

    print_str("\nRecording batches...\n");
    for (int i = 0; i < 16; i++) {
        int product = 500 + (i % 10);
        int company = 100 + (i % 14);
        int qty = 100 + (i * 50);
        int year = 2024;
        int month = 1 + (i % 12);
        int day = 1 + (i % 28);
        fs_record_batch(product, company, qty, year, month, day);
    }

    print_str("\nDistributing batches...\n");
    for (int i = 0; i < 16; i++) {
        fs_distribute_batch(i);
    }

    print_str("\nRecalling batches...\n");
    fs_recall_batch(3);
    fs_recall_batch(7);
    fs_recall_batch(11);
    fs_recall_batch(15);

    print_str("\nFiling complaints...\n");
    for (int i = 0; i < 12; i++) {
        int product = 500 + (i % 10);
        int consumer = 2000 + i;
        int type = (i % 3) + 1;
        int severity = (i % 4) + 1;
        int year = 2024;
        fs_file_complaint(product, consumer, type, severity, year);
    }

    print_str("\nResolving complaints...\n");
    for (int i = 0; i < 12; i++) {
        fs_resolve_complaint(i);
    }

    print_str("\nRevoking some licenses...\n");
    fs_revoke_license(2);
    fs_revoke_license(5);
    fs_revoke_license(9);

    print_str("\nInspection report...\n");
    fs_inspection_report();

    print_str("\nLicense report...\n");
    fs_license_report();

    print_str("\nTraceability report...\n");
    fs_traceability_report();

    print_str("\nComplaint report...\n");
    fs_complaint_report();

    print_str("\nFinal state...\n");
    fs_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
