/* taxi_service: Taxi service management (v1.0)
 * Fleet, drivers, rides, dispatch, fares
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

#define MAX_TAXIS      16
#define MAX_DRIVERS    18
#define MAX_RIDES      20
#define MAX_FARES      10
#define MAX_RATINGS    12

typedef struct {
    int    taxi_id;
    int    zone;
    int    plate;
    int    type;
    int    status;
    int    mileage;
    int    year_built;
    int    insurance;
    int    active;
} taxi_t;

typedef struct {
    int    driver_id;
    int    taxi_id;
    int    license;
    int    years_exp;
    int    rating;
    int    rides_completed;
    int    complaints;
    int    active;
} driver_t;

typedef struct {
    int    ride_id;
    int    driver_id;
    int    zone;
    int    distance_km;
    int    duration_min;
    int    fare;
    int    tip;
    int    status;
    int    year;
    int    active;
} ride_t;

typedef struct {
    int    fare_id;
    int    type;
    int    zone;
    int    base_fare;
    int    per_km;
    int    per_min;
    int    minimum;
    int    active;
} fare_t;

typedef struct {
    int    rating_id;
    int    driver_id;
    int    ride_id;
    int    score;
    int    feedback;
    int    year;
    int    active;
} rating_t;

typedef struct {
    int    n_taxis;
    int    n_drivers;
    int    n_rides;
    int    n_fares;
    int    n_ratings;
    int    total_rides;
    int    total_distance;
    int    total_revenue;
    int    total_tips;
    int    total_complaints;
    int    avg_rating;
} ts_state_t;

static taxi_t taxis[MAX_TAXIS];
static driver_t drivers[MAX_DRIVERS];
static ride_t rides[MAX_RIDES];
static fare_t fares[MAX_FARES];
static rating_t ratings[MAX_RATINGS];
static ts_state_t ts;

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

int ts_init(void) {
    if (initialized) return -1;
    ts.n_taxis = 0; ts.n_drivers = 0; ts.n_rides = 0;
    ts.n_fares = 0; ts.n_ratings = 0;
    ts.total_rides = 0; ts.total_distance = 0;
    ts.total_revenue = 0; ts.total_tips = 0;
    ts.total_complaints = 0; ts.avg_rating = 0;
    for (int i = 0; i < MAX_TAXIS; i++) taxis[i].active = 0;
    for (int i = 0; i < MAX_DRIVERS; i++) drivers[i].active = 0;
    for (int i = 0; i < MAX_RIDES; i++) rides[i].active = 0;
    for (int i = 0; i < MAX_FARES; i++) fares[i].active = 0;
    for (int i = 0; i < MAX_RATINGS; i++) ratings[i].active = 0;
    initialized = 1;
    print_str("[TS] Taxi service initialized\n");
    return 0;
}

int ts_add_taxi(int zone, int plate, int type, int year, int insurance) {
    if (ts.n_taxis >= MAX_TAXIS) return -1;
    taxi_t* t = &taxis[ts.n_taxis];
    t->taxi_id = ts.n_taxis;
    t->zone = zone;
    t->plate = plate;
    t->type = type;
    t->status = 1;
    t->mileage = 0;
    t->year_built = year;
    t->insurance = insurance;
    t->active = 1;
    ts.n_taxis++;
    print_str("[TS] Taxi "); print_int(ts.n_taxis - 1);
    print_str(" zone="); print_int(zone);
    print_str(" plate="); print_int(plate);
    print_str(" type="); print_int(type);
    print_str(" "); print_int(year); print_str("\n");
    return ts.n_taxis - 1;
}

int ts_add_driver(int taxi_id, int license, int years, int rating) {
    if (ts.n_drivers >= MAX_DRIVERS) return -1;
    driver_t* d = &drivers[ts.n_drivers];
    d->driver_id = ts.n_drivers;
    d->taxi_id = taxi_id;
    d->license = license;
    d->years_exp = years;
    d->rating = rating;
    d->rides_completed = 0;
    d->complaints = 0;
    d->active = 1;
    ts.n_drivers++;
    print_str("[TS] Driver "); print_int(ts.n_drivers - 1);
    print_str(" taxi="); print_int(taxi_id);
    print_str(" lic="); print_int(license);
    print_str(" yrs="); print_int(years);
    print_str(" rating="); print_int(rating); print_str("\n");
    return ts.n_drivers - 1;
}

int ts_dispatch_ride(int driver_id, int zone, int distance, int duration, int fare, int tip, int year) {
    if (ts.n_rides >= MAX_RIDES) return -1;
    if (driver_id >= ts.n_drivers) return -1;
    ride_t* r = &rides[ts.n_rides];
    r->ride_id = ts.n_rides;
    r->driver_id = driver_id;
    r->zone = zone;
    r->distance_km = distance;
    r->duration_min = duration;
    r->fare = fare;
    r->tip = tip;
    r->status = 2;
    r->year = year;
    r->active = 1;
    drivers[driver_id].rides_completed++;
    ts.total_rides++;
    ts.total_distance += distance;
    ts.total_revenue += fare;
    ts.total_tips += tip;
    ts.n_rides++;
    print_str("[TS] Ride "); print_int(ts.n_rides - 1);
    print_str(" D"); print_int(driver_id);
    print_str(" zone="); print_int(zone);
    print_str(" dist="); print_int(distance); print_str("km");
    print_str(" fare=$"); print_int(fare);
    print_str(" tip=$"); print_int(tip); print_str("\n");
    return ts.n_rides - 1;
}

int ts_complain_driver(int driver_id) {
    if (driver_id >= ts.n_drivers) return -1;
    drivers[driver_id].complaints++;
    ts.total_complaints++;
    print_str("[TS] Complain D"); print_int(driver_id); print_str("\n");
    return 0;
}

int ts_add_fare(int type, int zone, int base, int per_km, int per_min, int minimum) {
    if (ts.n_fares >= MAX_FARES) return -1;
    fare_t* f = &fares[ts.n_fares];
    f->fare_id = ts.n_fares;
    f->type = type;
    f->zone = zone;
    f->base_fare = base;
    f->per_km = per_km;
    f->per_min = per_min;
    f->minimum = minimum;
    f->active = 1;
    ts.n_fares++;
    print_str("[TS] Fare "); print_int(ts.n_fares - 1);
    print_str(" type="); print_int(type);
    print_str(" zone="); print_int(zone);
    print_str(" base=$"); print_int(base);
    print_str(" /km=$"); print_int(per_km);
    print_str(" /min=$"); print_int(per_min); print_str("\n");
    return ts.n_fares - 1;
}

int ts_add_rating(int driver_id, int ride_id, int score, int feedback, int year) {
    if (ts.n_ratings >= MAX_RATINGS) return -1;
    rating_t* r = &ratings[ts.n_ratings];
    r->rating_id = ts.n_ratings;
    r->driver_id = driver_id;
    r->ride_id = ride_id;
    r->score = score;
    r->feedback = feedback;
    r->year = year;
    r->active = 1;
    if (ts.n_ratings == 0) ts.avg_rating = score;
    else ts.avg_rating = (ts.avg_rating * ts.n_ratings + score) / (ts.n_ratings + 1);
    ts.n_ratings++;
    print_str("[TS] Rating "); print_int(ts.n_ratings - 1);
    print_str(" D"); print_int(driver_id);
    print_str(" R"); print_int(ride_id);
    print_str(" score="); print_int(score); print_str("/5\n");
    return ts.n_ratings - 1;
}

void ts_fleet_report(void) {
    print_str("[TS] Fleet report:\n");
    print_str("  Taxis: "); print_int(ts.n_taxis); print_str("\n");
    print_str("  Drivers: "); print_int(ts.n_drivers); print_str("\n");
    print_str("  Complaints: "); print_int(ts.total_complaints); print_str("\n");
}

void ts_ride_report(void) {
    print_str("[TS] Ride report:\n");
    print_str("  Total rides: "); print_int(ts.total_rides); print_str("\n");
    print_str("  Total distance: "); print_int(ts.total_distance); print_str(" km\n");
    print_str("  Total revenue: $"); print_int(ts.total_revenue); print_str("\n");
    print_str("  Total tips: $"); print_int(ts.total_tips); print_str("\n");
}

void ts_rating_report(void) {
    print_str("[TS] Rating report:\n");
    print_str("  Ratings: "); print_int(ts.n_ratings); print_str("\n");
    print_str("  Avg rating: "); print_int(ts.avg_rating); print_str("/5\n");
}

void ts_print_state(void) {
    print_str("[TS] Tx="); print_int(ts.n_taxis);
    print_str(" Dr="); print_int(ts.n_drivers);
    print_str(" Rd="); print_int(ts.n_rides);
    print_str(" Fr="); print_int(ts.n_fares);
    print_str(" Rt="); print_int(ts.n_ratings);
    print_str("\n");
}

int main(void) {
    print_str("=== Taxi Service Demo ===\n\n");
    ts_init();

    print_str("Adding taxis...\n");
    for (int i = 0; i < 16; i++) {
        int zone = (i % 4) + 1;
        int plate = 1000 + i;
        int type = (i % 3) + 1;
        int year = 2018 + (i % 6);
        int ins = 2024 + (i % 3);
        ts_add_taxi(zone, plate, type, year, ins);
    }

    print_str("\nAdding drivers...\n");
    for (int i = 0; i < 18; i++) {
        int taxi = i % 16;
        int lic = (i % 3) + 1;
        int yrs = 1 + (i % 15);
        int rating = 3 + (i % 3);
        ts_add_driver(taxi, lic, yrs, rating);
    }

    print_str("\nDispatching rides...\n");
    for (int i = 0; i < 20; i++) {
        int driver = i % 18;
        int zone = (i % 4) + 1;
        int dist = 5 + (i % 20);
        int dur = 10 + (i * 3);
        int fare = 10 + (i * 3);
        int tip = i % 5;
        int year = 2024;
        ts_dispatch_ride(driver, zone, dist, dur, fare, tip, year);
    }

    print_str("\nComplaints...\n");
    for (int i = 0; i < 4; i++) {
        ts_complain_driver(i * 4);
    }

    print_str("\nAdding fare rules...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 3) + 1;
        int zone = (i % 4) + 1;
        int base = 5 + (i % 5);
        int per_km = 2 + (i % 3);
        int per_min = 1 + (i % 2);
        int min = 8 + (i % 5);
        ts_add_fare(type, zone, base, per_km, per_min, min);
    }

    print_str("\nAdding ratings...\n");
    for (int i = 0; i < 12; i++) {
        int driver = i % 18;
        int ride = i % 20;
        int score = 3 + (i % 3);
        int fb = (i % 4) + 1;
        int year = 2024;
        ts_add_rating(driver, ride, score, fb, year);
    }

    print_str("\nFleet report...\n");
    ts_fleet_report();

    print_str("\nRide report...\n");
    ts_ride_report();

    print_str("\nRating report...\n");
    ts_rating_report();

    print_str("\nFinal state...\n");
    ts_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
