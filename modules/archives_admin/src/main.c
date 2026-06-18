/* archives_admin: Archives administration system (v1.0)
 * Collection, preservation, utilization, digitization, electronic archives
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
#define MAX_DIGITIZE     10
#define MAX_ELECTRONIC   10

typedef struct {
    int    collection_id;
    int    archive_id;
    int    source_type;
    int    items_received;
    int    items_donated;
    int    items_collected;
    int    year;
    int    active;
} collection_t;

typedef struct {
    int    preserve_id;
    int    storage_id;
    int    preservation_type;
    int    items_stored;
    int    restoration;
    int    security_score;
    int    year;
    int    active;
} preserve_t;

typedef struct {
    int    utilize_id;
    int    service_id;
    int    service_type;
    int    inquiries;
    int    copies_provided;
    int    exhibitions;
    int    year;
    int    active;
} utilize_t;

typedef struct {
    int    digitize_id;
    int    project_id;
    int    scan_type;
    int    pages_scanned;
    int    database_records;
    int    online_access;
    int    year;
    int    active;
} digitize_t;

typedef struct {
    int    electronic_id;
    int    system_id;
    int    file_type;
    int    files_archived;
    int    storage_mb;
    int    migration_count;
    int    year;
    int    active;
} electronic_t;

typedef struct {
    int    n_collection;
    int    n_preserve;
    int    n_utilize;
    int    n_digitize;
    int    n_electronic;
    int    total_items;
    int    total_stored;
    int    total_inquiries;
    int    total_scanned;
    int    total_files;
} arc_state_t;

static collection_t collections[MAX_COLLECTION];
static preserve_t preserves[MAX_PRESERVE];
static utilize_t utilizes[MAX_UTILIZE];
static digitize_t digitizes[MAX_DIGITIZE];
static electronic_t electronics[MAX_ELECTRONIC];
static arc_state_t arc;

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

int arc_init(void) {
    if (initialized) return -1;
    arc.n_collection = 0; arc.n_preserve = 0; arc.n_utilize = 0;
    arc.n_digitize = 0; arc.n_electronic = 0;
    arc.total_items = 0; arc.total_stored = 0;
    arc.total_inquiries = 0; arc.total_scanned = 0;
    arc.total_files = 0;
    for (int i = 0; i < MAX_COLLECTION; i++) collections[i].active = 0;
    for (int i = 0; i < MAX_PRESERVE; i++) preserves[i].active = 0;
    for (int i = 0; i < MAX_UTILIZE; i++) utilizes[i].active = 0;
    for (int i = 0; i < MAX_DIGITIZE; i++) digitizes[i].active = 0;
    for (int i = 0; i < MAX_ELECTRONIC; i++) electronics[i].active = 0;
    initialized = 1;
    print_str("[ARC] Archives admin initialized\n");
    return 0;
}

int arc_collection(int archive_id, int source_type, int received, int donated, int collected, int year) {
    if (arc.n_collection >= MAX_COLLECTION) return -1;
    collection_t* c = &collections[arc.n_collection];
    c->collection_id = arc.n_collection;
    c->archive_id = archive_id;
    c->source_type = source_type;
    c->items_received = received;
    c->items_donated = donated;
    c->items_collected = collected;
    c->year = year;
    c->active = 1;
    arc.total_items += received + donated + collected;
    arc.n_collection++;
    print_str("[ARC] Collection "); print_int(arc.n_collection - 1);
    print_str(" arc="); print_int(archive_id);
    print_str(" src="); print_int(source_type);
    print_str(" rcv="); print_int(received);
    print_str(" dnt="); print_int(donated);
    print_str(" clt="); print_int(collected); print_str("\n");
    return arc.n_collection - 1;
}

int arc_preserve(int storage, int preservation_type, int stored, int restoration, int security, int year) {
    if (arc.n_preserve >= MAX_PRESERVE) return -1;
    preserve_t* p = &preserves[arc.n_preserve];
    p->preserve_id = arc.n_preserve;
    p->storage_id = storage;
    p->preservation_type = preservation_type;
    p->items_stored = stored;
    p->restoration = restoration;
    p->security_score = security;
    p->year = year;
    p->active = 1;
    arc.total_stored += stored;
    arc.n_preserve++;
    print_str("[ARC] Preserve "); print_int(arc.n_preserve - 1);
    print_str(" str="); print_int(storage);
    print_str(" type="); print_int(preservation_type);
    print_str(" stN="); print_int(stored);
    print_str(" rst="); print_int(restoration);
    print_str(" sec="); print_int(security); print_str("\n");
    return arc.n_preserve - 1;
}

int arc_utilize(int service, int service_type, int inquiries, int copies, int exhibitions, int year) {
    if (arc.n_utilize >= MAX_UTILIZE) return -1;
    utilize_t* u = &utilizes[arc.n_utilize];
    u->utilize_id = arc.n_utilize;
    u->service_id = service;
    u->service_type = service_type;
    u->inquiries = inquiries;
    u->copies_provided = copies;
    u->exhibitions = exhibitions;
    u->year = year;
    u->active = 1;
    arc.total_inquiries += inquiries;
    arc.n_utilize++;
    print_str("[ARC] Utilize "); print_int(arc.n_utilize - 1);
    print_str(" svc="); print_int(service);
    print_str(" type="); print_int(service_type);
    print_str(" inq="); print_int(inquiries);
    print_str(" cps="); print_int(copies);
    print_str(" exh="); print_int(exhibitions); print_str("\n");
    return arc.n_utilize - 1;
}

int arc_digitize(int project, int scan_type, int pages, int records, int online, int year) {
    if (arc.n_digitize >= MAX_DIGITIZE) return -1;
    digitize_t* d = &digitizes[arc.n_digitize];
    d->digitize_id = arc.n_digitize;
    d->project_id = project;
    d->scan_type = scan_type;
    d->pages_scanned = pages;
    d->database_records = records;
    d->online_access = online;
    d->year = year;
    d->active = 1;
    arc.total_scanned += pages;
    arc.n_digitize++;
    print_str("[ARC] Digitize "); print_int(arc.n_digitize - 1);
    print_str(" prj="); print_int(project);
    print_str(" type="); print_int(scan_type);
    print_str(" pg="); print_int(pages);
    print_str(" rec="); print_int(records);
    print_str(" onl="); print_int(online); print_str("\n");
    return arc.n_digitize - 1;
}

int arc_electronic(int system, int file_type, int files, int storage_mb, int migration, int year) {
    if (arc.n_electronic >= MAX_ELECTRONIC) return -1;
    electronic_t* e = &electronics[arc.n_electronic];
    e->electronic_id = arc.n_electronic;
    e->system_id = system;
    e->file_type = file_type;
    e->files_archived = files;
    e->storage_mb = storage_mb;
    e->migration_count = migration;
    e->year = year;
    e->active = 1;
    arc.total_files += files;
    arc.n_electronic++;
    print_str("[ARC] Electronic "); print_int(arc.n_electronic - 1);
    print_str(" sys="); print_int(system);
    print_str(" type="); print_int(file_type);
    print_str(" fls="); print_int(files);
    print_str(" stg="); print_int(storage_mb); print_str("MB");
    print_str(" mgr="); print_int(migration); print_str("\n");
    return arc.n_electronic - 1;
}

void arc_collection_report(void) {
    print_str("[ARC] Collection report:\n");
    print_str("  Collections: "); print_int(arc.n_collection); print_str("\n");
    print_str("  Total items: "); print_int(arc.total_items); print_str("\n");
}

void arc_preserve_report(void) {
    print_str("[ARC] Preservation report:\n");
    print_str("  Storage facilities: "); print_int(arc.n_preserve); print_str("\n");
    print_str("  Total stored: "); print_int(arc.total_stored); print_str("\n");
}

void arc_utilize_report(void) {
    print_str("[ARC] Utilization report:\n");
    print_str("  Service points: "); print_int(arc.n_utilize); print_str("\n");
    print_str("  Total inquiries: "); print_int(arc.total_inquiries); print_str("\n");
    print_str("  Digitization projects: "); print_int(arc.n_digitize); print_str("\n");
    print_str("  Total pages scanned: "); print_int(arc.total_scanned); print_str("\n");
    print_str("  Electronic archives: "); print_int(arc.n_electronic); print_str("\n");
    print_str("  Total files: "); print_int(arc.total_files); print_str("\n");
}

void arc_print_state(void) {
    print_str("[ARC] Cl="); print_int(arc.n_collection);
    print_str(" Ps="); print_int(arc.n_preserve);
    print_str(" Ut="); print_int(arc.n_utilize);
    print_str(" Dg="); print_int(arc.n_digitize);
    print_str(" El="); print_int(arc.n_electronic);
    print_str("\n");
}

int main(void) {
    print_str("=== Archives Admin Demo ===\n\n");
    arc_init();

    print_str("Archive collection...\n");
    for (int i = 0; i < 16; i++) {
        int arc_id = 100 + (i * 10);
        int src = (i % 5) + 1;
        int rcv = 1000 + (i * 500);
        int dnt = 200 + (i * 100);
        int clt = 300 + (i * 80);
        int year = 2020 + (i % 5);
        arc_collection(arc_id, src, rcv, dnt, clt, year);
    }

    print_str("\nPreservation...\n");
    for (int i = 0; i < 14; i++) {
        int str = 200 + (i * 7);
        int type = (i % 4) + 1;
        int stN = 5000 + (i * 2000);
        int rst = 50 + (i * 10);
        int sec = 80 + (i % 15);
        int year = 2021 + (i % 4);
        arc_preserve(str, type, stN, rst, sec, year);
    }

    print_str("\nUtilization...\n");
    for (int i = 0; i < 12; i++) {
        int svc = 300 + (i * 11);
        int type = (i % 4) + 1;
        int inq = 500 + (i * 100);
        int cps = inq / 2;
        int exh = 2 + (i % 5);
        int year = 2022 + (i % 3);
        arc_utilize(svc, type, inq, cps, exh, year);
    }

    print_str("\nDigitization...\n");
    for (int i = 0; i < 10; i++) {
        int prj = 400 + (i * 13);
        int type = (i % 3) + 1;
        int pg = 10000 + (i * 5000);
        int rec = pg / 10;
        int onl = rec - (i * 100);
        int year = 2023 + (i % 2);
        arc_digitize(prj, type, pg, rec, onl, year);
    }

    print_str("\nElectronic archives...\n");
    for (int i = 0; i < 10; i++) {
        int sys = 500 + (i * 17);
        int type = (i % 4) + 1;
        int fls = 20000 + (i * 10000);
        int stg = 500 + (i * 200);
        int mgr = 1 + (i % 3);
        int year = 2024;
        arc_electronic(sys, type, fls, stg, mgr, year);
    }

    print_str("\nCollection report...\n");
    arc_collection_report();

    print_str("\nPreservation report...\n");
    arc_preserve_report();

    print_str("\nUtilization report...\n");
    arc_utilize_report();

    print_str("\nFinal state...\n");
    arc_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
