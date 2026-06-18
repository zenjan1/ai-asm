/* archives: Archives management system (v1.0)
 * Collection, preservation, utilization, electronic archives, compilation
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

#define MAX_COLLECTION   16
#define MAX_PRESERVE     14
#define MAX_UTILIZE      12
#define MAX_ELECTRONIC   10
#define MAX_COMPILE      10

typedef struct {
    int    collection_id;
    int    source_type;
    int    donor_id;
    int    volume_count;
    int    category;
    int    year_range;
    int    year;
    int    active;
} collection_t;

typedef struct {
    int    preserve_id;
    int    archive_id;
    int    storage_id;
    int    condition;
    int    restoration;
    int    digitized;
    int    year;
    int    status;
    int    active;
} preserve_t;

typedef struct {
    int    utilize_id;
    int    user_id;
    int    archive_id;
    int    purpose;
    int    copies_made;
    int    duration_days;
    int    year;
    int    active;
} utilize_t;

typedef struct {
    int    electronic_id;
    int    file_type;
    int    department_id;
    int    file_count;
    int    storage_mb;
    int    backup_count;
    int    year;
    int    active;
} electronic_t;

typedef struct {
    int    compile_id;
    int    project_type;
    int    archive_source;
    int    volumes_published;
    int    researchers;
    int    distribution;
    int    year;
    int    active;
} compile_t;

typedef struct {
    int    n_collection;
    int    n_preserve;
    int    n_utilize;
    int    n_electronic;
    int    n_compile;
    int    total_volumes;
    int    total_restored;
    int    total_digitized;
    int    total_copies;
    int    total_storage;
} ar_state_t;

static collection_t collections[MAX_COLLECTION];
static preserve_t preserves[MAX_PRESERVE];
static utilize_t utilizes[MAX_UTILIZE];
static electronic_t electronics[MAX_ELECTRONIC];
static compile_t compilations[MAX_COMPILE];
static ar_state_t ar;

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

int ar_init(void) {
    if (initialized) return -1;
    ar.n_collection = 0; ar.n_preserve = 0; ar.n_utilize = 0;
    ar.n_electronic = 0; ar.n_compile = 0;
    ar.total_volumes = 0; ar.total_restored = 0;
    ar.total_digitized = 0; ar.total_copies = 0;
    ar.total_storage = 0;
    for (int i = 0; i < MAX_COLLECTION; i++) collections[i].active = 0;
    for (int i = 0; i < MAX_PRESERVE; i++) preserves[i].active = 0;
    for (int i = 0; i < MAX_UTILIZE; i++) utilizes[i].active = 0;
    for (int i = 0; i < MAX_ELECTRONIC; i++) electronics[i].active = 0;
    for (int i = 0; i < MAX_COMPILE; i++) compilations[i].active = 0;
    initialized = 1;
    print_str("[AR] Archives initialized\n");
    return 0;
}

int ar_collect(int source, int donor, int volumes, int category, int year_range, int year) {
    if (ar.n_collection >= MAX_COLLECTION) return -1;
    collection_t* c = &collections[ar.n_collection];
    c->collection_id = ar.n_collection;
    c->source_type = source;
    c->donor_id = donor;
    c->volume_count = volumes;
    c->category = category;
    c->year_range = year_range;
    c->year = year;
    c->active = 1;
    ar.total_volumes += volumes;
    ar.n_collection++;
    print_str("[AR] Collection "); print_int(ar.n_collection - 1);
    print_str(" src="); print_int(source);
    print_str(" dnr="); print_int(donor);
    print_str(" vol="); print_int(volumes);
    print_str(" cat="); print_int(category);
    print_str(" yr="); print_int(year_range); print_str("\n");
    return ar.n_collection - 1;
}

int ar_preserve(int archive, int storage, int condition, int restoration, int digitized, int year) {
    if (ar.n_preserve >= MAX_PRESERVE) return -1;
    preserve_t* p = &preserves[ar.n_preserve];
    p->preserve_id = ar.n_preserve;
    p->archive_id = archive;
    p->storage_id = storage;
    p->condition = condition;
    p->restoration = restoration;
    p->digitized = digitized;
    p->year = year;
    p->status = 1;
    p->active = 1;
    if (restoration) ar.total_restored++;
    if (digitized) ar.total_digitized++;
    ar.n_preserve++;
    print_str("[AR] Preserve "); print_int(ar.n_preserve - 1);
    print_str(" arc="); print_int(archive);
    print_str(" stg="); print_int(storage);
    print_str(" cnd="); print_int(condition);
    print_str(" rst="); print_int(restoration);
    print_str(" dgt="); print_int(digitized); print_str("\n");
    return ar.n_preserve - 1;
}

int ar_utilize(int user, int archive, int purpose, int copies, int duration, int year) {
    if (ar.n_utilize >= MAX_UTILIZE) return -1;
    utilize_t* u = &utilizes[ar.n_utilize];
    u->utilize_id = ar.n_utilize;
    u->user_id = user;
    u->archive_id = archive;
    u->purpose = purpose;
    u->copies_made = copies;
    u->duration_days = duration;
    u->year = year;
    u->active = 1;
    ar.total_copies += copies;
    ar.n_utilize++;
    print_str("[AR] Utilize "); print_int(ar.n_utilize - 1);
    print_str(" usr="); print_int(user);
    print_str(" arc="); print_int(archive);
    print_str(" pur="); print_int(purpose);
    print_str(" cps="); print_int(copies);
    print_str(" dur="); print_int(duration); print_str("d\n");
    return ar.n_utilize - 1;
}

int ar_electronic(int file_type, int dept, int files, int storage, int backups, int year) {
    if (ar.n_electronic >= MAX_ELECTRONIC) return -1;
    electronic_t* e = &electronics[ar.n_electronic];
    e->electronic_id = ar.n_electronic;
    e->file_type = file_type;
    e->department_id = dept;
    e->file_count = files;
    e->storage_mb = storage;
    e->backup_count = backups;
    e->year = year;
    e->active = 1;
    ar.total_storage += storage;
    ar.n_electronic++;
    print_str("[AR] Electronic "); print_int(ar.n_electronic - 1);
    print_str(" typ="); print_int(file_type);
    print_str(" dpt="); print_int(dept);
    print_str(" fls="); print_int(files);
    print_str(" stg="); print_int(storage); print_str("MB");
    print_str(" bkp="); print_int(backups); print_str("\n");
    return ar.n_electronic - 1;
}

int ar_compile(int project, int source, int volumes, int researchers, int distribution, int year) {
    if (ar.n_compile >= MAX_COMPILE) return -1;
    compile_t* c = &compilations[ar.n_compile];
    c->compile_id = ar.n_compile;
    c->project_type = project;
    c->archive_source = source;
    c->volumes_published = volumes;
    c->researchers = researchers;
    c->distribution = distribution;
    c->year = year;
    c->active = 1;
    ar.n_compile++;
    print_str("[AR] Compile "); print_int(ar.n_compile - 1);
    print_str(" prj="); print_int(project);
    print_str(" src="); print_int(source);
    print_str(" vol="); print_int(volumes);
    print_str(" rch="); print_int(researchers);
    print_str(" dst="); print_int(distribution); print_str("\n");
    return ar.n_compile - 1;
}

void ar_collection_report(void) {
    print_str("[AR] Collection report:\n");
    print_str("  Collections: "); print_int(ar.n_collection); print_str("\n");
    print_str("  Total volumes: "); print_int(ar.total_volumes); print_str("\n");
}

void ar_preserve_report(void) {
    print_str("[AR] Preservation report:\n");
    print_str("  Records: "); print_int(ar.n_preserve); print_str("\n");
    print_str("  Restored: "); print_int(ar.total_restored); print_str("\n");
    print_str("  Digitized: "); print_int(ar.total_digitized); print_str("\n");
}

void ar_utilize_report(void) {
    print_str("[AR] Utilization report:\n");
    print_str("  Access requests: "); print_int(ar.n_utilize); print_str("\n");
    print_str("  Total copies: "); print_int(ar.total_copies); print_str("\n");
    print_str("  Electronic archives: "); print_int(ar.n_electronic); print_str("\n");
    print_str("  Total storage: "); print_int(ar.total_storage); print_str(" MB\n");
    print_str("  Compilation projects: "); print_int(ar.n_compile); print_str("\n");
}

void ar_print_state(void) {
    print_str("[AR] Cl="); print_int(ar.n_collection);
    print_str(" Ps="); print_int(ar.n_preserve);
    print_str(" Ut="); print_int(ar.n_utilize);
    print_str(" El="); print_int(ar.n_electronic);
    print_str(" Cp="); print_int(ar.n_compile);
    print_str("\n");
}

int main(void) {
    print_str("=== Archives Demo ===\n\n");
    ar_init();

    print_str("Archive collection...\n");
    for (int i = 0; i < 16; i++) {
        int src = (i % 4) + 1;
        int dnr = 1000 + (i * 13);
        int vol = 10 + (i * 5);
        int cat = (i % 6) + 1;
        int yr = 1950 + (i * 5);
        int year = 2020 + (i % 5);
        ar_collect(src, dnr, vol, cat, yr, year);
    }

    print_str("\nArchive preservation...\n");
    for (int i = 0; i < 14; i++) {
        int arc = 200 + (i * 10);
        int stg = 300 + (i % 8);
        int cnd = 50 + (i * 4);
        int rst = (i % 4 == 0) ? 1 : 0;
        int dgt = (i % 3 == 0) ? 1 : 0;
        int year = 2022 + (i % 3);
        ar_preserve(arc, stg, cnd, rst, dgt, year);
    }

    print_str("\nArchive utilization...\n");
    for (int i = 0; i < 12; i++) {
        int usr = 3000 + (i * 11);
        int arc = 400 + (i * 7);
        int pur = (i % 4) + 1;
        int cps = 1 + (i % 5);
        int dur = 1 + (i % 7);
        int year = 2023 + (i % 2);
        ar_utilize(usr, arc, pur, cps, dur, year);
    }

    print_str("\nElectronic archives...\n");
    for (int i = 0; i < 10; i++) {
        int typ = (i % 4) + 1;
        int dpt = 500 + (i * 17);
        int fls = 500 + (i * 200);
        int stg = 100 + (i * 50);
        int bkp = 2 + (i % 3);
        int year = 2024;
        ar_electronic(typ, dpt, fls, stg, bkp, year);
    }

    print_str("\nArchive compilation...\n");
    for (int i = 0; i < 10; i++) {
        int prj = (i % 3) + 1;
        int src = 600 + (i * 7);
        int vol = 3 + (i % 5);
        int rch = 2 + (i % 4);
        int dst = 50 + (i * 20);
        int year = 2024;
        ar_compile(prj, src, vol, rch, dst, year);
    }

    print_str("\nCollection report...\n");
    ar_collection_report();

    print_str("\nPreservation report...\n");
    ar_preserve_report();

    print_str("\nUtilization report...\n");
    ar_utilize_report();

    print_str("\nFinal state...\n");
    ar_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
