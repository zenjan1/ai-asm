/* urban_planning: Urban-rural planning management (v1.0)
 * Master plans, detailed plans, specialized plans, approval, archives
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

#define MAX_MASTER       14
#define MAX_DETAIL       16
#define MAX_SPECIAL      12
#define MAX_APPROVE      10
#define MAX_ARCHIVE      10

typedef struct {
    int    plan_id;
    int    level;
    int    region_id;
    int    area_km2;
    int    population_target;
    int    year_start;
    int    year_end;
    int    status;
    int    active;
} master_plan_t;

typedef struct {
    int    plan_id;
    int    master_id;
    int    type;
    int    block_id;
    int    land_use;
    int    density;
    int    height_limit;
    int    year;
    int    active;
} detail_plan_t;

typedef struct {
    int    plan_id;
    int    category;
    int    region_id;
    int    length_km;
    int    budget;
    int    completion_year;
    int    year;
    int    status;
    int    active;
} special_plan_t;

typedef struct {
    int    approve_id;
    int    plan_id;
    int    reviewer_id;
    int    stage;
    int    comments;
    int    approved;
    int    year;
    int    active;
} approval_t;

typedef struct {
    int    archive_id;
    int    plan_id;
    int    doc_type;
    int    pages;
    int    drawings;
    int    storage_location;
    int    year_archived;
    int    active;
} archive_t;

typedef struct {
    int    n_master;
    int    n_detail;
    int    n_special;
    int    n_approve;
    int    n_archive;
    int    total_area;
    int    total_population;
    int    total_budget;
    int    total_approved;
    int    total_archived;
} up_state_t;

static master_plan_t master_plans[MAX_MASTER];
static detail_plan_t detail_plans[MAX_DETAIL];
static special_plan_t special_plans[MAX_SPECIAL];
static approval_t approvals[MAX_APPROVE];
static archive_t archives[MAX_ARCHIVE];
static up_state_t up;

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

int up_init(void) {
    if (initialized) return -1;
    up.n_master = 0; up.n_detail = 0; up.n_special = 0;
    up.n_approve = 0; up.n_archive = 0;
    up.total_area = 0; up.total_population = 0;
    up.total_budget = 0; up.total_approved = 0;
    up.total_archived = 0;
    for (int i = 0; i < MAX_MASTER; i++) master_plans[i].active = 0;
    for (int i = 0; i < MAX_DETAIL; i++) detail_plans[i].active = 0;
    for (int i = 0; i < MAX_SPECIAL; i++) special_plans[i].active = 0;
    for (int i = 0; i < MAX_APPROVE; i++) approvals[i].active = 0;
    for (int i = 0; i < MAX_ARCHIVE; i++) archives[i].active = 0;
    initialized = 1;
    print_str("[UP] Urban planning initialized\n");
    return 0;
}

int up_master_plan(int level, int region, int area, int population, int year_start, int year_end) {
    if (up.n_master >= MAX_MASTER) return -1;
    master_plan_t* m = &master_plans[up.n_master];
    m->plan_id = up.n_master;
    m->level = level;
    m->region_id = region;
    m->area_km2 = area;
    m->population_target = population;
    m->year_start = year_start;
    m->year_end = year_end;
    m->status = 1;
    m->active = 1;
    up.total_area += area;
    up.total_population += population;
    up.n_master++;
    print_str("[UP] Master "); print_int(up.n_master - 1);
    print_str(" lvl="); print_int(level);
    print_str(" reg="); print_int(region);
    print_str(" area="); print_int(area); print_str("km2");
    print_str(" pop="); print_int(population);
    print_str(" "); print_int(year_start); print_str("-"); print_int(year_end); print_str("\n");
    return up.n_master - 1;
}

int up_detail_plan(int master, int type, int block, int land_use, int density, int height, int year) {
    if (up.n_detail >= MAX_DETAIL) return -1;
    detail_plan_t* d = &detail_plans[up.n_detail];
    d->plan_id = up.n_detail;
    d->master_id = master;
    d->type = type;
    d->block_id = block;
    d->land_use = land_use;
    d->density = density;
    d->height_limit = height;
    d->year = year;
    d->active = 1;
    up.n_detail++;
    print_str("[UP] Detail "); print_int(up.n_detail - 1);
    print_str(" master="); print_int(master);
    print_str(" type="); print_int(type);
    print_str(" blk="); print_int(block);
    print_str(" use="); print_int(land_use);
    print_str(" dens="); print_int(density);
    print_str(" hgt="); print_int(height); print_str("m\n");
    return up.n_detail - 1;
}

int up_special_plan(int category, int region, int length, int budget, int completion, int year) {
    if (up.n_special >= MAX_SPECIAL) return -1;
    special_plan_t* s = &special_plans[up.n_special];
    s->plan_id = up.n_special;
    s->category = category;
    s->region_id = region;
    s->length_km = length;
    s->budget = budget;
    s->completion_year = completion;
    s->year = year;
    s->status = 1;
    s->active = 1;
    up.total_budget += budget;
    up.n_special++;
    print_str("[UP] Special "); print_int(up.n_special - 1);
    print_str(" cat="); print_int(category);
    print_str(" reg="); print_int(region);
    print_str(" len="); print_int(length); print_str("km");
    print_str(" bud=$"); print_int(budget); print_str("\n");
    return up.n_special - 1;
}

int up_approve(int plan, int reviewer, int stage, int comments, int approved, int year) {
    if (up.n_approve >= MAX_APPROVE) return -1;
    approval_t* a = &approvals[up.n_approve];
    a->approve_id = up.n_approve;
    a->plan_id = plan;
    a->reviewer_id = reviewer;
    a->stage = stage;
    a->comments = comments;
    a->approved = approved;
    a->year = year;
    a->active = 1;
    if (approved > 0) up.total_approved++;
    up.n_approve++;
    print_str("[UP] Approve "); print_int(up.n_approve - 1);
    print_str(" plan="); print_int(plan);
    print_str(" rev="); print_int(reviewer);
    print_str(" stg="); print_int(stage);
    print_str(" cmt="); print_int(comments);
    print_str(" ap="); print_int(approved); print_str("\n");
    return up.n_approve - 1;
}

int up_archive(int plan, int doc_type, int pages, int drawings, int location, int year) {
    if (up.n_archive >= MAX_ARCHIVE) return -1;
    archive_t* a = &archives[up.n_archive];
    a->archive_id = up.n_archive;
    a->plan_id = plan;
    a->doc_type = doc_type;
    a->pages = pages;
    a->drawings = drawings;
    a->storage_location = location;
    a->year_archived = year;
    a->active = 1;
    up.total_archived++;
    up.n_archive++;
    print_str("[UP] Archive "); print_int(up.n_archive - 1);
    print_str(" plan="); print_int(plan);
    print_str(" type="); print_int(doc_type);
    print_str(" pg="); print_int(pages);
    print_str(" dwg="); print_int(drawings);
    print_str(" loc="); print_int(location); print_str("\n");
    return up.n_archive - 1;
}

void up_master_report(void) {
    print_str("[UP] Master plan report:\n");
    print_str("  Plans: "); print_int(up.n_master); print_str("\n");
    print_str("  Total area: "); print_int(up.total_area); print_str(" km2\n");
    print_str("  Total population: "); print_int(up.total_population); print_str("\n");
}

void up_detail_report(void) {
    print_str("[UP] Detail plan report:\n");
    print_str("  Detail plans: "); print_int(up.n_detail); print_str("\n");
    print_str("  Special plans: "); print_int(up.n_special); print_str("\n");
    print_str("  Total budget: $"); print_int(up.total_budget); print_str("\n");
}

void up_approval_report(void) {
    print_str("[UP] Approval report:\n");
    print_str("  Reviews: "); print_int(up.n_approve); print_str("\n");
    print_str("  Approved: "); print_int(up.total_approved); print_str("\n");
    print_str("  Archived: "); print_int(up.total_archived); print_str("\n");
}

void up_print_state(void) {
    print_str("[UP] Ms="); print_int(up.n_master);
    print_str(" Dt="); print_int(up.n_detail);
    print_str(" Sp="); print_int(up.n_special);
    print_str(" Ap="); print_int(up.n_approve);
    print_str(" Ar="); print_int(up.n_archive);
    print_str("\n");
}

int main(void) {
    print_str("=== Urban Planning Demo ===\n\n");
    up_init();

    print_str("Creating master plans...\n");
    for (int i = 0; i < 14; i++) {
        int level = (i % 3) + 1;
        int region = (i % 8) + 1;
        int area = 50 + (i * 30);
        int pop = 100000 + (i * 50000);
        int start = 2020 + (i % 5);
        int end = start + 10 + (i % 5);
        up_master_plan(level, region, area, pop, start, end);
    }

    print_str("\nCreating detail plans...\n");
    for (int i = 0; i < 16; i++) {
        int master = i % 14;
        int type = (i % 2) + 1;
        int block = 100 + (i * 5);
        int use = (i % 6) + 1;
        int density = 100 + (i * 50);
        int height = 20 + (i * 5);
        int year = 2021 + (i % 4);
        up_detail_plan(master, type, block, use, density, height, year);
    }

    print_str("\nCreating special plans...\n");
    for (int i = 0; i < 12; i++) {
        int cat = (i % 4) + 1;
        int region = (i % 8) + 1;
        int length = 5 + (i * 3);
        int budget = 1000000 + (i * 500000);
        int comp = 2025 + (i % 5);
        int year = 2022 + (i % 3);
        up_special_plan(cat, region, length, budget, comp, year);
    }

    print_str("\nApproval process...\n");
    for (int i = 0; i < 10; i++) {
        int plan = i % 14;
        int reviewer = 500 + (i * 7);
        int stage = (i % 3) + 1;
        int comments = 2 + (i % 8);
        int approved = (i % 3 == 0) ? 0 : 1;
        int year = 2023 + (i % 2);
        up_approve(plan, reviewer, stage, comments, approved, year);
    }

    print_str("\nArchiving plans...\n");
    for (int i = 0; i < 10; i++) {
        int plan = i % 14;
        int type = (i % 3) + 1;
        int pages = 50 + (i * 20);
        int dwg = 10 + (i * 5);
        int loc = (i % 5) + 1;
        int year = 2024;
        up_archive(plan, type, pages, dwg, loc, year);
    }

    print_str("\nMaster plan report...\n");
    up_master_report();

    print_str("\nDetail plan report...\n");
    up_detail_report();

    print_str("\nApproval report...\n");
    up_approval_report();

    print_str("\nFinal state...\n");
    up_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
