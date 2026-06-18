/* land_registry: Land and property registration (v1.0)
 * Land parcels, titles, mortgages, transfers, transactions
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

#define MAX_PARCELS    16
#define MAX_TITLES     20
#define MAX_MORTGAGES  12
#define MAX_TRANSFERS  14
#define MAX_LEASES     10

typedef struct {
    int    parcel_id;
    int    zone;
    int    area_sqm;
    int    land_type;
    int    value;
    int    north;
    int    south;
    int    east;
    int    west;
    int    active;
} parcel_t;

typedef struct {
    int    title_id;
    int    parcel_id;
    int    owner_id;
    int    type;
    int    issue_year;
    int    value;
    int    status;
    int    active;
} title_t;

typedef struct {
    int    mort_id;
    int    title_id;
    int    lender_id;
    int    amount;
    int    rate_pct;
    int    term_years;
    int    start_year;
    int    paid;
    int    active;
} mortgage_t;

typedef struct {
    int    trans_id;
    int    title_id;
    int    from_id;
    int    to_id;
    int    type;
    int    price;
    int    year;
    int    registered;
    int    active;
} transfer_t;

typedef struct {
    int    lease_id;
    int    parcel_id;
    int    lessee_id;
    int    type;
    int    area_sqm;
    int    rent_annual;
    int    start_year;
    int    term_years;
    int    active;
} lease_t;

typedef struct {
    int    n_parcels;
    int    n_titles;
    int    n_mortgages;
    int    n_transfers;
    int    n_leases;
    int    total_area;
    int    total_value;
    int    total_mortgage_amt;
    int    total_transfer_value;
    int    total_lease_revenue;
    int    total_registered;
} lr_state_t;

static parcel_t parcels[MAX_PARCELS];
static title_t titles[MAX_TITLES];
static mortgage_t mortgages[MAX_MORTGAGES];
static transfer_t transfers[MAX_TRANSFERS];
static lease_t leases[MAX_LEASES];
static lr_state_t lr;

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

int lr_init(void) {
    if (initialized) return -1;
    lr.n_parcels = 0; lr.n_titles = 0; lr.n_mortgages = 0;
    lr.n_transfers = 0; lr.n_leases = 0;
    lr.total_area = 0; lr.total_value = 0;
    lr.total_mortgage_amt = 0; lr.total_transfer_value = 0;
    lr.total_lease_revenue = 0; lr.total_registered = 0;
    for (int i = 0; i < MAX_PARCELS; i++) parcels[i].active = 0;
    for (int i = 0; i < MAX_TITLES; i++) titles[i].active = 0;
    for (int i = 0; i < MAX_MORTGAGES; i++) mortgages[i].active = 0;
    for (int i = 0; i < MAX_TRANSFERS; i++) transfers[i].active = 0;
    for (int i = 0; i < MAX_LEASES; i++) leases[i].active = 0;
    initialized = 1;
    print_str("[LR] Land registry initialized\n");
    return 0;
}

int lr_register_parcel(int zone, int area, int type, int value, int n, int s, int e, int w) {
    if (lr.n_parcels >= MAX_PARCELS) return -1;
    parcel_t* p = &parcels[lr.n_parcels];
    p->parcel_id = lr.n_parcels;
    p->zone = zone;
    p->area_sqm = area;
    p->land_type = type;
    p->value = value;
    p->north = n;
    p->south = s;
    p->east = e;
    p->west = w;
    p->active = 1;
    lr.total_area += area;
    lr.total_value += value;
    lr.n_parcels++;
    print_str("[LR] Parcel "); print_int(lr.n_parcels - 1);
    print_str(" zone="); print_int(zone);
    print_str(" area="); print_int(area);
    print_str(" type="); print_int(type);
    print_str(" $"); print_int(value); print_str("\n");
    return lr.n_parcels - 1;
}

int lr_issue_title(int parcel, int owner, int type, int year, int value) {
    if (lr.n_titles >= MAX_TITLES || parcel >= lr.n_parcels) return -1;
    title_t* t = &titles[lr.n_titles];
    t->title_id = lr.n_titles;
    t->parcel_id = parcel;
    t->owner_id = owner;
    t->type = type;
    t->issue_year = year;
    t->value = value;
    t->status = 1;
    t->active = 1;
    lr.total_registered++;
    lr.n_titles++;
    print_str("[LR] Title "); print_int(lr.n_titles - 1);
    print_str(" P"); print_int(parcel);
    print_str(" O"); print_int(owner);
    print_str(" type="); print_int(type);
    print_str(" "); print_int(year);
    print_str(" $"); print_int(value); print_str("\n");
    return lr.n_titles - 1;
}

int lr_register_mortgage(int title, int lender, int amount, int rate, int term, int year) {
    if (lr.n_mortgages >= MAX_MORTGAGES || title >= lr.n_titles) return -1;
    mortgage_t* m = &mortgages[lr.n_mortgages];
    m->mort_id = lr.n_mortgages;
    m->title_id = title;
    m->lender_id = lender;
    m->amount = amount;
    m->rate_pct = rate;
    m->term_years = term;
    m->start_year = year;
    m->paid = 0;
    m->active = 1;
    titles[title].status = 2;
    lr.total_mortgage_amt += amount;
    lr.n_mortgages++;
    print_str("[LR] Mortgage "); print_int(lr.n_mortgages - 1);
    print_str(" T"); print_int(title);
    print_str(" L"); print_int(lender);
    print_str(" $"); print_int(amount);
    print_str(" rate="); print_int(rate);
    print_str("% term="); print_int(term); print_str("yr\n");
    return lr.n_mortgages - 1;
}

int lr_release_mortgage(int mort_id) {
    if (mort_id >= lr.n_mortgages) return -1;
    mortgages[mort_id].paid = 1;
    titles[mortgages[mort_id].title_id].status = 1;
    print_str("[LR] Release M"); print_int(mort_id); print_str("\n");
    return 0;
}

int lr_transfer_title(int title, int from, int to, int type, int price, int year) {
    if (lr.n_transfers >= MAX_TRANSFERS || title >= lr.n_titles) return -1;
    transfer_t* tr = &transfers[lr.n_transfers];
    tr->trans_id = lr.n_transfers;
    tr->title_id = title;
    tr->from_id = from;
    tr->to_id = to;
    tr->type = type;
    tr->price = price;
    tr->year = year;
    tr->registered = 0;
    tr->active = 1;
    titles[title].owner_id = to;
    lr.total_transfer_value += price;
    lr.n_transfers++;
    print_str("[LR] Transfer "); print_int(lr.n_transfers - 1);
    print_str(" T"); print_int(title);
    print_str(" O"); print_int(from); print_str("->O"); print_int(to);
    print_str(" type="); print_int(type);
    print_str(" $"); print_int(price);
    print_str(" "); print_int(year); print_str("\n");
    return lr.n_transfers - 1;
}

int lr_register_transfer(int trans_id) {
    if (trans_id >= lr.n_transfers) return -1;
    transfers[trans_id].registered = 1;
    print_str("[LR] Register Tr"); print_int(trans_id); print_str("\n");
    return 0;
}

int lr_grant_lease(int parcel, int lessee, int type, int area, int rent, int start, int term) {
    if (lr.n_leases >= MAX_LEASES || parcel >= lr.n_parcels) return -1;
    lease_t* l = &leases[lr.n_leases];
    l->lease_id = lr.n_leases;
    l->parcel_id = parcel;
    l->lessee_id = lessee;
    l->type = type;
    l->area_sqm = area;
    l->rent_annual = rent;
    l->start_year = start;
    l->term_years = term;
    l->active = 1;
    lr.total_lease_revenue += rent * term;
    lr.n_leases++;
    print_str("[LR] Lease "); print_int(lr.n_leases - 1);
    print_str(" P"); print_int(parcel);
    print_str(" L"); print_int(lessee);
    print_str(" type="); print_int(type);
    print_str(" area="); print_int(area);
    print_str(" rent=$"); print_int(rent);
    print_str(" term="); print_int(term); print_str("yr\n");
    return lr.n_leases - 1;
}

int lr_renew_lease(int lease_id, int new_term) {
    if (lease_id >= lr.n_leases) return -1;
    leases[lease_id].term_years += new_term;
    lr.total_lease_revenue += leases[lease_id].rent_annual * new_term;
    print_str("[LR] Renew Lease"); print_int(lease_id);
    print_str(" +"); print_int(new_term); print_str("yr\n");
    return 0;
}

void lr_parcel_report(void) {
    print_str("[LR] Parcel report:\n");
    print_str("  Total area: "); print_int(lr.total_area); print_str(" sqm\n");
    print_str("  Total value: "); print_int(lr.total_value); print_str("\n");
    for (int i = 0; i < lr.n_parcels; i++) {
        parcel_t* p = &parcels[i];
        print_str("  P"); print_int(i);
        print_str(" zone="); print_int(p->zone);
        print_str(" area="); print_int(p->area_sqm);
        print_str(" type="); print_int(p->land_type);
        print_str(" $"); print_int(p->value); print_str("\n");
    }
}

void lr_title_report(void) {
    print_str("[LR] Title report:\n");
    print_str("  Registered: "); print_int(lr.total_registered); print_str("\n");
    for (int i = 0; i < lr.n_titles; i++) {
        title_t* t = &titles[i];
        print_str("  T"); print_int(i);
        print_str(" P"); print_int(t->parcel_id);
        print_str(" O"); print_int(t->owner_id);
        print_str(" type="); print_int(t->type);
        print_str(" "); print_int(t->issue_year);
        print_str(" $"); print_int(t->value);
        print_str(" st="); print_int(t->status); print_str("\n");
    }
}

void lr_financial_report(void) {
    print_str("[LR] Financial report:\n");
    print_str("  Total value: "); print_int(lr.total_value); print_str("\n");
    print_str("  Mortgage amt: "); print_int(lr.total_mortgage_amt); print_str("\n");
    print_str("  Transfer value: "); print_int(lr.total_transfer_value); print_str("\n");
    print_str("  Lease revenue: "); print_int(lr.total_lease_revenue); print_str("\n");
}

void lr_print_state(void) {
    print_str("[LR] Parcels="); print_int(lr.n_parcels);
    print_str(" Titles="); print_int(lr.n_titles);
    print_str(" Mort="); print_int(lr.n_mortgages);
    print_str(" Transfers="); print_int(lr.n_transfers);
    print_str(" Leases="); print_int(lr.n_leases);
    print_str("\n");
    print_str("  Value: "); print_int(lr.total_value); print_str("\n");
}

int main(void) {
    print_str("=== Land Registry Demo ===\n\n");
    lr_init();

    print_str("Registering parcels...\n");
    for (int i = 0; i < 16; i++) {
        int zone = (i % 4) + 1;
        int area = 200 + (i * 100);
        int type = (i % 3) + 1;
        int value = 100000 + (i * 50000);
        int n = 100 + i; int s = 100 + i + 1;
        int e = 200 + i; int w = 200 + i + 1;
        lr_register_parcel(zone, area, type, value, n, s, e, w);
    }

    print_str("\nIssuing titles...\n");
    for (int i = 0; i < 20; i++) {
        int parcel = i % 16;
        int owner = 1000 + i;
        int type = (i % 3) + 1;
        int year = 2018 + (i % 7);
        int value = 150000 + (i * 30000);
        lr_issue_title(parcel, owner, type, year, value);
    }

    print_str("\nRegistering mortgages...\n");
    for (int i = 0; i < 12; i++) {
        int title = i;
        int lender = 2000 + i;
        int amount = 200000 + (i * 50000);
        int rate = 4 + (i % 4);
        int term = 15 + (i % 15);
        int year = 2020 + (i % 5);
        lr_register_mortgage(title, lender, amount, rate, term, year);
    }

    print_str("\nReleasing mortgages...\n");
    for (int i = 0; i < 6; i++) {
        lr_release_mortgage(i);
    }

    print_str("\nTransferring titles...\n");
    for (int i = 0; i < 14; i++) {
        int title = (i % 10) + 5;
        int from = 1000 + title;
        int to = 3000 + i;
        int type = (i % 3) + 1;
        int price = 300000 + (i * 40000);
        int year = 2023 + (i % 3);
        lr_transfer_title(title, from, to, type, price, year);
    }

    print_str("\nRegistering transfers...\n");
    for (int i = 0; i < 14; i++) {
        lr_register_transfer(i);
    }

    print_str("\nGranting leases...\n");
    for (int i = 0; i < 10; i++) {
        int parcel = i % 16;
        int lessee = 4000 + i;
        int type = (i % 3) + 1;
        int area = 100 + (i * 50);
        int rent = 10000 + (i * 5000);
        int start = 2024;
        int term = 5 + (i % 10);
        lr_grant_lease(parcel, lessee, type, area, rent, start, term);
    }

    print_str("\nRenewing leases...\n");
    for (int i = 0; i < 5; i++) {
        lr_renew_lease(i, 5);
    }

    print_str("\nParcel report...\n");
    lr_parcel_report();

    print_str("\nTitle report...\n");
    lr_title_report();

    print_str("\nFinancial report...\n");
    lr_financial_report();

    print_str("\nFinal state...\n");
    lr_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
