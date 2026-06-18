/* standards_body: Standards organization management (v1.0)
 * Standard creation, review, library, certification, international coop
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

#define MAX_STDS        18
#define MAX_REVIEWS     14
#define MAX_LIBRARY     16
#define MAX_CERTS       12
#define MAX_COOP        10

typedef struct {
    int    std_id;
    int    level;
    int    category;
    int    committee;
    int    pages;
    int    status;
    int    year_proposed;
    int    year_published;
    int    active;
} standard_t;

typedef struct {
    int    review_id;
    int    std_id;
    int    reviewer_id;
    int    review_type;
    int    comments;
    int    approvals;
    int    rejections;
    int    year;
    int    active;
} review_t;

typedef struct {
    int    lib_id;
    int    std_id;
    int    category;
    int    version;
    int    superseded_by;
    int    downloads;
    int    year_added;
    int    status;
    int    active;
} library_entry_t;

typedef struct {
    int    cert_id;
    int    type;
    int    company_id;
    int    product_id;
    int    score;
    int    valid_years;
    int    year_issued;
    int    status;
    int    active;
} certification_t;

typedef struct {
    int    coop_id;
    int    org_type;
    int    partner_id;
    int    standards_aligned;
    int    meetings;
    int    joint_projects;
    int    year_started;
    int    active;
} cooperation_t;

typedef struct {
    int    n_stds;
    int    n_reviews;
    int    n_library;
    int    n_certs;
    int    n_coop;
    int    total_published;
    int    total_certified;
    int    total_downloads;
    int    total_partners;
    int    total_aligned;
} sb_state_t;

static standard_t standards[MAX_STDS];
static review_t reviews[MAX_REVIEWS];
static library_entry_t library[MAX_LIBRARY];
static certification_t certifications[MAX_CERTS];
static cooperation_t cooperations[MAX_COOP];
static sb_state_t sb;

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

int sb_init(void) {
    if (initialized) return -1;
    sb.n_stds = 0; sb.n_reviews = 0; sb.n_library = 0;
    sb.n_certs = 0; sb.n_coop = 0;
    sb.total_published = 0; sb.total_certified = 0;
    sb.total_downloads = 0; sb.total_partners = 0;
    sb.total_aligned = 0;
    for (int i = 0; i < MAX_STDS; i++) standards[i].active = 0;
    for (int i = 0; i < MAX_REVIEWS; i++) reviews[i].active = 0;
    for (int i = 0; i < MAX_LIBRARY; i++) library[i].active = 0;
    for (int i = 0; i < MAX_CERTS; i++) certifications[i].active = 0;
    for (int i = 0; i < MAX_COOP; i++) cooperations[i].active = 0;
    initialized = 1;
    print_str("[SB] Standards body initialized\n");
    return 0;
}

int sb_propose_standard(int level, int category, int committee, int pages, int year) {
    if (sb.n_stds >= MAX_STDS) return -1;
    standard_t* s = &standards[sb.n_stds];
    s->std_id = sb.n_stds;
    s->level = level;
    s->category = category;
    s->committee = committee;
    s->pages = pages;
    s->status = 1;
    s->year_proposed = year;
    s->year_published = 0;
    s->active = 1;
    sb.n_stds++;
    print_str("[SB] Standard "); print_int(sb.n_stds - 1);
    print_str(" lvl="); print_int(level);
    print_str(" cat="); print_int(category);
    print_str(" com="); print_int(committee);
    print_str(" pg="); print_int(pages);
    print_str(" yr="); print_int(year); print_str("\n");
    return sb.n_stds - 1;
}

int sb_publish_standard(int std_id, int year_published) {
    if (std_id >= sb.n_stds) return -1;
    standards[std_id].status = 2;
    standards[std_id].year_published = year_published;
    sb.total_published++;
    print_str("[SB] Published Std "); print_int(std_id);
    print_str(" yr="); print_int(year_published); print_str("\n");
    return 0;
}

int sb_review(int std_id, int reviewer, int review_type, int comments, int approvals, int rejections, int year) {
    if (sb.n_reviews >= MAX_REVIEWS) return -1;
    review_t* r = &reviews[sb.n_reviews];
    r->review_id = sb.n_reviews;
    r->std_id = std_id;
    r->reviewer_id = reviewer;
    r->review_type = review_type;
    r->comments = comments;
    r->approvals = approvals;
    r->rejections = rejections;
    r->year = year;
    r->active = 1;
    sb.n_reviews++;
    print_str("[SB] Review "); print_int(sb.n_reviews - 1);
    print_str(" std="); print_int(std_id);
    print_str(" rev="); print_int(reviewer);
    print_str(" type="); print_int(review_type);
    print_str(" ap="); print_int(approvals);
    print_str(" rj="); print_int(rejections); print_str("\n");
    return sb.n_reviews - 1;
}

int sb_add_library(int std_id, int category, int version, int superseded, int downloads, int year) {
    if (sb.n_library >= MAX_LIBRARY) return -1;
    library_entry_t* l = &library[sb.n_library];
    l->lib_id = sb.n_library;
    l->std_id = std_id;
    l->category = category;
    l->version = version;
    l->superseded_by = superseded;
    l->downloads = downloads;
    l->year_added = year;
    l->status = 1;
    l->active = 1;
    sb.total_downloads += downloads;
    sb.n_library++;
    print_str("[SB] Library "); print_int(sb.n_library - 1);
    print_str(" std="); print_int(std_id);
    print_str(" cat="); print_int(category);
    print_str(" ver="); print_int(version);
    print_str(" dl="); print_int(downloads); print_str("\n");
    return sb.n_library - 1;
}

int sb_issue_cert(int type, int company, int product, int score, int valid_years, int year) {
    if (sb.n_certs >= MAX_CERTS) return -1;
    certification_t* c = &certifications[sb.n_certs];
    c->cert_id = sb.n_certs;
    c->type = type;
    c->company_id = company;
    c->product_id = product;
    c->score = score;
    c->valid_years = valid_years;
    c->year_issued = year;
    c->status = 1;
    c->active = 1;
    sb.total_certified++;
    sb.n_certs++;
    print_str("[SB] Cert "); print_int(sb.n_certs - 1);
    print_str(" type="); print_int(type);
    print_str(" co="); print_int(company);
    print_str(" prod="); print_int(product);
    print_str(" score="); print_int(score);
    print_str(" valid="); print_int(valid_years); print_str("yr\n");
    return sb.n_certs - 1;
}

int sb_add_cooperation(int org_type, int partner, int aligned, int meetings, int projects, int year) {
    if (sb.n_coop >= MAX_COOP) return -1;
    cooperation_t* co = &cooperations[sb.n_coop];
    co->coop_id = sb.n_coop;
    co->org_type = org_type;
    co->partner_id = partner;
    co->standards_aligned = aligned;
    co->meetings = meetings;
    co->joint_projects = projects;
    co->year_started = year;
    co->active = 1;
    sb.total_partners++;
    sb.total_aligned += aligned;
    sb.n_coop++;
    print_str("[SB] Coop "); print_int(sb.n_coop - 1);
    print_str(" org="); print_int(org_type);
    print_str(" ptr="); print_int(partner);
    print_str(" align="); print_int(aligned);
    print_str(" mtg="); print_int(meetings);
    print_str(" proj="); print_int(projects); print_str("\n");
    return sb.n_coop - 1;
}

void sb_standards_report(void) {
    print_str("[SB] Standards report:\n");
    print_str("  Standards: "); print_int(sb.n_stds); print_str("\n");
    print_str("  Published: "); print_int(sb.total_published); print_str("\n");
    print_str("  Reviews: "); print_int(sb.n_reviews); print_str("\n");
}

void sb_library_report(void) {
    print_str("[SB] Library report:\n");
    print_str("  Entries: "); print_int(sb.n_library); print_str("\n");
    print_str("  Total downloads: "); print_int(sb.total_downloads); print_str("\n");
}

void sb_certification_report(void) {
    print_str("[SB] Certification report:\n");
    print_str("  Certificates: "); print_int(sb.n_certs); print_str("\n");
    print_str("  Partners: "); print_int(sb.total_partners); print_str("\n");
    print_str("  Standards aligned: "); print_int(sb.total_aligned); print_str("\n");
}

void sb_print_state(void) {
    print_str("[SB] St="); print_int(sb.n_stds);
    print_str(" Rv="); print_int(sb.n_reviews);
    print_str(" Lb="); print_int(sb.n_library);
    print_str(" Ct="); print_int(sb.n_certs);
    print_str(" Cp="); print_int(sb.n_coop);
    print_str("\n");
}

int main(void) {
    print_str("=== Standards Body Demo ===\n\n");
    sb_init();

    print_str("Proposing standards...\n");
    for (int i = 0; i < 18; i++) {
        int level = (i % 3) + 1;
        int cat = (i % 10) + 1;
        int com = 100 + (i * 3);
        int pages = 20 + (i * 5);
        int year = 2018 + (i % 6);
        sb_propose_standard(level, cat, com, pages, year);
    }

    print_str("\nPublishing standards...\n");
    for (int i = 0; i < 14; i++) {
        sb_publish_standard(i, 2020 + (i % 5));
    }

    print_str("\nConducting reviews...\n");
    for (int i = 0; i < 14; i++) {
        int std = i % 18;
        int reviewer = 300 + (i % 12);
        int type = (i % 3) + 1;
        int comments = 3 + (i % 10);
        int ap = 5 + (i % 8);
        int rj = i % 3;
        int year = 2021 + (i % 4);
        sb_review(std, reviewer, type, comments, ap, rj, year);
    }

    print_str("\nBuilding library...\n");
    for (int i = 0; i < 16; i++) {
        int std = i % 18;
        int cat = (i % 10) + 1;
        int ver = 1 + (i % 3);
        int sup = 0;
        int dl = 100 + (i * 50);
        int year = 2020 + (i % 5);
        sb_add_library(std, cat, ver, sup, dl, year);
    }

    print_str("\nIssuing certifications...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int co = 500 + (i * 10);
        int prod = 700 + (i * 5);
        int score = 70 + (i * 2);
        int valid = 2 + (i % 3);
        int year = 2022 + (i % 3);
        sb_issue_cert(type, co, prod, score, valid, year);
    }

    print_str("\nInternational cooperation...\n");
    for (int i = 0; i < 10; i++) {
        int org = (i % 3) + 1;
        int partner = 900 + (i * 10);
        int aligned = 3 + (i % 8);
        int mtg = 2 + (i % 6);
        int proj = 1 + (i % 4);
        int year = 2019 + (i % 5);
        sb_add_cooperation(org, partner, aligned, mtg, proj, year);
    }

    print_str("\nStandards report...\n");
    sb_standards_report();

    print_str("\nLibrary report...\n");
    sb_library_report();

    print_str("\nCertification report...\n");
    sb_certification_report();

    print_str("\nFinal state...\n");
    sb_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
