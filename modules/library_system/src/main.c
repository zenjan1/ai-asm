/* library_system: Library management system (v1.0)
 * Collections, readers, lending, search, analytics
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

#define MAX_BOOKS      16
#define MAX_READERS    12
#define MAX_LOANS      18
#define MAX_CATEGORIES 8
#define MAX_RESERVATIONS 10

typedef struct {
    int    book_id;
    int    category_id;
    int    isbn_hash;
    int    location;
    int    status;
    int    n_loans;
    int    n_reservations;
    int    active;
} book_t;

typedef struct {
    int    reader_id;
    int    tier;
    int    n_loans;
    int    overdue_count;
    double fines_paid;
    int    active;
} reader_t;

typedef struct {
    int    loan_id;
    int    book_id;
    int    reader_id;
    int    borrow_day;
    int    due_day;
    int    return_day;
    double fine;
    int    status;
    int    active;
} loan_t;

typedef struct {
    int    category_id;
    int    n_books;
    int    n_loans;
    int    active;
} category_t;

typedef struct {
    int    res_id;
    int    book_id;
    int    reader_id;
    int    request_day;
    int    status;
    int    active;
} reservation_t;

typedef struct {
    int    n_books;
    int    n_readers;
    int    n_loans;
    int    n_categories;
    int    n_reservations;
    double total_fines;
    int    active_loans;
    int    overdue_loans;
    int    total_loans_ever;
} lib_state_t;

static book_t books[MAX_BOOKS];
static reader_t readers[MAX_READERS];
static loan_t loans[MAX_LOANS];
static category_t categories[MAX_CATEGORIES];
static reservation_t reservations[MAX_RESERVATIONS];
static lib_state_t lib;

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

int lib_init(void) {
    if (initialized) return -1;
    lib.n_books = 0; lib.n_readers = 0; lib.n_loans = 0;
    lib.n_categories = 0; lib.n_reservations = 0;
    lib.total_fines = 0.0; lib.active_loans = 0;
    lib.overdue_loans = 0; lib.total_loans_ever = 0;
    for (int i = 0; i < MAX_BOOKS; i++) books[i].active = 0;
    for (int i = 0; i < MAX_READERS; i++) readers[i].active = 0;
    for (int i = 0; i < MAX_LOANS; i++) loans[i].active = 0;
    for (int i = 0; i < MAX_CATEGORIES; i++) categories[i].active = 0;
    for (int i = 0; i < MAX_RESERVATIONS; i++) reservations[i].active = 0;
    initialized = 1;
    print_str("[LIB] Library system initialized\n");
    return 0;
}

int lib_add_category(void) {
    if (lib.n_categories >= MAX_CATEGORIES) return -1;
    category_t* c = &categories[lib.n_categories];
    c->category_id = lib.n_categories;
    c->n_books = 0;
    c->n_loans = 0;
    c->active = 1;
    lib.n_categories++;
    print_str("[LIB] Category "); print_int(lib.n_categories - 1); print_str("\n");
    return lib.n_categories - 1;
}

int lib_add_book(int category_id, int isbn_hash, int location) {
    if (lib.n_books >= MAX_BOOKS || category_id >= lib.n_categories) return -1;
    book_t* b = &books[lib.n_books];
    b->book_id = lib.n_books;
    b->category_id = category_id;
    b->isbn_hash = isbn_hash;
    b->location = location;
    b->status = 1;
    b->n_loans = 0;
    b->n_reservations = 0;
    b->active = 1;
    categories[category_id].n_books++;
    lib.n_books++;
    print_str("[LIB] Book "); print_int(lib.n_books - 1);
    print_str(" cat="); print_int(category_id);
    print_str(" ISBN="); print_int(isbn_hash);
    print_str(" loc="); print_int(location); print_str("\n");
    return lib.n_books - 1;
}

int lib_register_reader(int tier) {
    if (lib.n_readers >= MAX_READERS) return -1;
    reader_t* r = &readers[lib.n_readers];
    r->reader_id = lib.n_readers;
    r->tier = tier;
    r->n_loans = 0;
    r->overdue_count = 0;
    r->fines_paid = 0.0;
    r->active = 1;
    lib.n_readers++;
    print_str("[LIB] Reader "); print_int(lib.n_readers - 1);
    print_str(" tier="); print_int(tier); print_str("\n");
    return lib.n_readers - 1;
}

int lib_borrow_book(int book_id, int reader_id, int borrow_day, int loan_days) {
    if (lib.n_loans >= MAX_LOANS) return -1;
    if (book_id >= lib.n_books || reader_id >= lib.n_readers) return -2;
    book_t* b = &books[book_id];
    if (b->status != 1) return -3;
    int max_loans[4] = {3, 5, 8, 12};
    if (readers[reader_id].n_loans >= max_loans[readers[reader_id].tier < 4 ? readers[reader_id].tier : 0]) return -4;
    loan_t* l = &loans[lib.n_loans];
    l->loan_id = lib.n_loans;
    l->book_id = book_id;
    l->reader_id = reader_id;
    l->borrow_day = borrow_day;
    l->due_day = borrow_day + loan_days;
    l->return_day = 0;
    l->fine = 0.0;
    l->status = 1;
    l->active = 1;
    b->status = 2;
    b->n_loans++;
    readers[reader_id].n_loans++;
    categories[b->category_id].n_loans++;
    lib.n_loans++;
    lib.active_loans++;
    lib.total_loans_ever++;
    print_str("[LIB] Loan "); print_int(lib.n_loans - 1);
    print_str(" B"); print_int(book_id);
    print_str(" R"); print_int(reader_id);
    print_str(" due day "); print_int(l->due_day); print_str("\n");
    return lib.n_loans - 1;
}

int lib_return_book(int loan_id, int return_day) {
    if (loan_id >= lib.n_loans) return -1;
    loan_t* l = &loans[loan_id];
    l->return_day = return_day;
    l->status = 2;
    book_t* b = &books[l->book_id];
    b->status = 1;
    readers[l->reader_id].n_loans--;
    lib.active_loans--;
    if (return_day > l->due_day) {
        int overdue_days = return_day - l->due_day;
        l->fine = overdue_days * 0.5;
        lib.total_fines += l->fine;
        readers[l->reader_id].fines_paid += l->fine;
        readers[l->reader_id].overdue_count++;
        lib.overdue_loans++;
        print_str("[LIB] Return L"); print_int(loan_id);
        print_str(" OVERDUE "); print_int(overdue_days); print_str("d");
        print_str(" fine="); print_int((int)(l->fine * 10)); print_str("\n");
    } else {
        print_str("[LIB] Return L"); print_int(loan_id);
        print_str(" on time day="); print_int(return_day); print_str("\n");
    }
    return 0;
}

int lib_reserve_book(int book_id, int reader_id, int request_day) {
    if (lib.n_reservations >= MAX_RESERVATIONS) return -1;
    if (book_id >= lib.n_books || reader_id >= lib.n_readers) return -2;
    reservation_t* r = &reservations[lib.n_reservations];
    r->res_id = lib.n_reservations;
    r->book_id = book_id;
    r->reader_id = reader_id;
    r->request_day = request_day;
    r->status = 1;
    r->active = 1;
    books[book_id].n_reservations++;
    lib.n_reservations++;
    print_str("[LIB] Reservation "); print_int(lib.n_reservations - 1);
    print_str(" B"); print_int(book_id);
    print_str(" R"); print_int(reader_id); print_str("\n");
    return lib.n_reservations - 1;
}

void lib_fulfill_reservation(int res_id) {
    if (res_id >= lib.n_reservations) return;
    reservations[res_id].status = 3;
    print_str("[LIB] Reservation "); print_int(res_id); print_str(" fulfilled\n");
}

void lib_category_stats(void) {
    print_str("[LIB] Category stats:\n");
    for (int i = 0; i < lib.n_categories; i++) {
        category_t* c = &categories[i];
        print_str("  Cat "); print_int(i);
        print_str(" books="); print_int(c->n_books);
        print_str(" loans="); print_int(c->n_loans); print_str("\n");
    }
}

void lib_popular_books(void) {
    print_str("[LIB] Popular books:\n");
    for (int i = 0; i < lib.n_books; i++) {
        if (books[i].n_loans > 0) {
            print_str("  B"); print_int(i);
            print_str(" cat="); print_int(books[i].category_id);
            print_str(" loans="); print_int(books[i].n_loans); print_str("\n");
        }
    }
}

void lib_reader_stats(void) {
    print_str("[LIB] Reader stats:\n");
    for (int i = 0; i < lib.n_readers; i++) {
        reader_t* r = &readers[i];
        print_str("  R"); print_int(i);
        print_str(" tier="); print_int(r->tier);
        print_str(" loans="); print_int(r->n_loans);
        print_str(" overdue="); print_int(r->overdue_count);
        print_str(" fines="); print_int((int)(r->fines_paid * 10)); print_str("\n");
    }
}

void lib_print_state(void) {
    print_str("[LIB] Books="); print_int(lib.n_books);
    print_str(" Readers="); print_int(lib.n_readers);
    print_str(" Loans="); print_int(lib.n_loans);
    print_str(" Categories="); print_int(lib.n_categories);
    print_str(" Res="); print_int(lib.n_reservations);
    print_str("\n");
    print_str("  Active loans: "); print_int(lib.active_loans);
    print_str(" Overdue: "); print_int(lib.overdue_loans);
    print_str(" Total fines: "); print_int((int)(lib.total_fines * 10)); print_str("\n");
}

int main(void) {
    print_str("=== Library System Demo ===\n\n");
    lib_init();

    print_str("Adding categories...\n");
    lib_add_category();
    lib_add_category();
    lib_add_category();
    lib_add_category();
    lib_add_category();

    print_str("\nAdding books...\n");
    lib_add_book(0, 1001, 101);
    lib_add_book(0, 1002, 101);
    lib_add_book(0, 1003, 102);
    lib_add_book(1, 2001, 201);
    lib_add_book(1, 2002, 201);
    lib_add_book(1, 2003, 202);
    lib_add_book(2, 3001, 301);
    lib_add_book(2, 3002, 301);
    lib_add_book(3, 4001, 401);
    lib_add_book(3, 4002, 401);
    lib_add_book(3, 4003, 402);
    lib_add_book(4, 5001, 501);
    lib_add_book(4, 5002, 501);
    lib_add_book(4, 5003, 502);
    lib_add_book(0, 1004, 103);
    lib_add_book(1, 2004, 203);

    print_str("\nRegistering readers...\n");
    lib_register_reader(0);
    lib_register_reader(1);
    lib_register_reader(2);
    lib_register_reader(3);
    lib_register_reader(0);
    lib_register_reader(1);
    lib_register_reader(2);
    lib_register_reader(0);

    print_str("\nBorrowing books...\n");
    lib_borrow_book(0, 0, 10, 14);
    lib_borrow_book(3, 1, 10, 21);
    lib_borrow_book(6, 2, 11, 28);
    lib_borrow_book(8, 3, 11, 14);
    lib_borrow_book(11, 4, 12, 14);
    lib_borrow_book(1, 5, 12, 21);
    lib_borrow_book(4, 6, 13, 28);
    lib_borrow_book(9, 7, 13, 14);
    lib_borrow_book(2, 0, 14, 14);
    lib_borrow_book(5, 1, 14, 21);
    lib_borrow_book(7, 2, 15, 28);
    lib_borrow_book(10, 3, 15, 14);
    lib_borrow_book(12, 4, 16, 14);
    lib_borrow_book(13, 5, 16, 21);
    lib_borrow_book(14, 6, 17, 28);
    lib_borrow_book(15, 7, 17, 14);

    print_str("\nMaking reservations...\n");
    lib_reserve_book(0, 1, 12);
    lib_reserve_book(3, 2, 13);
    lib_reserve_book(6, 3, 14);
    lib_reserve_book(8, 4, 15);
    lib_reserve_book(11, 5, 16);

    print_str("\nReturning books...\n");
    lib_return_book(0, 20);
    lib_return_book(1, 35);
    lib_return_book(2, 30);
    lib_return_book(3, 25);
    lib_return_book(4, 20);
    lib_return_book(5, 35);
    lib_return_book(6, 35);
    lib_return_book(7, 25);
    lib_return_book(8, 28);
    lib_return_book(9, 20);
    lib_return_book(10, 25);
    lib_return_book(11, 25);
    lib_return_book(12, 20);
    lib_return_book(13, 25);
    lib_return_book(14, 30);
    lib_return_book(15, 25);

    print_str("\nFulfilling reservations...\n");
    lib_fulfill_reservation(0);
    lib_fulfill_reservation(1);
    lib_fulfill_reservation(2);
    lib_fulfill_reservation(3);
    lib_fulfill_reservation(4);

    print_str("\nCategory stats...\n");
    lib_category_stats();

    print_str("\nPopular books...\n");
    lib_popular_books();

    print_str("\nReader stats...\n");
    lib_reader_stats();

    print_str("\nFinal state...\n");
    lib_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
