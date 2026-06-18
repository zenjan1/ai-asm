/* railway_management: Railway management system (v1.0)
 * Train scheduling, track monitoring, passenger management, freight, safety
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

#define MAX_TRAINS     8
#define MAX_STATIONS   8
#define MAX_TRACKS     8
#define MAX_PASSENGERS 16
#define MAX_FREIGHT    8

typedef struct {
    int    train_id;
    int    current_station;
    double speed;
    double delay;
    int    direction;
    int    passengers;
    int    freight_cars;
    int    status;
    int    active;
} train_t;

typedef struct {
    int    station_id;
    int    platform_count;
    int    passengers_waiting;
    double dwell_time;
    int    active;
} station_t;

typedef struct {
    int    track_id;
    int    from_station;
    int    to_station;
    double length;
    double speed_limit;
    double condition;
    int    occupied;
    int    active;
} track_t;

typedef struct {
    int    passenger_id;
    int    from_station;
    int    to_station;
    int    train_id;
    int    ticket_valid;
    int    active;
} passenger_t;

typedef struct {
    int    freight_id;
    int    from_station;
    int    to_station;
    int    train_id;
    double weight;
    int    type;
    int    active;
} freight_t;

typedef struct {
    int    n_trains;
    int    n_stations;
    int    n_tracks;
    int    n_passengers;
    int    n_freight;
    double total_delay;
    int    safety_alerts;
    int    on_time_count;
} rm_state_t;

static train_t trains[MAX_TRAINS];
static station_t stations[MAX_STATIONS];
static track_t tracks[MAX_TRACKS];
static passenger_t passengers[MAX_PASSENGERS];
static freight_t freights[MAX_FREIGHT];
static rm_state_t rm;

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

int rm_init(void) {
    if (initialized) return -1;
    rm.n_trains = 0; rm.n_stations = 0; rm.n_tracks = 0;
    rm.n_passengers = 0; rm.n_freight = 0;
    rm.total_delay = 0.0; rm.safety_alerts = 0;
    rm.on_time_count = 0;
    for (int i = 0; i < MAX_TRAINS; i++) trains[i].active = 0;
    for (int i = 0; i < MAX_STATIONS; i++) stations[i].active = 0;
    for (int i = 0; i < MAX_TRACKS; i++) tracks[i].active = 0;
    for (int i = 0; i < MAX_PASSENGERS; i++) passengers[i].active = 0;
    for (int i = 0; i < MAX_FREIGHT; i++) freights[i].active = 0;
    initialized = 1;
    print_str("[RM] Railway management initialized\n");
    return 0;
}

int rm_add_train(int start_station, int direction) {
    if (rm.n_trains >= MAX_TRAINS) return -1;
    train_t* t = &trains[rm.n_trains];
    t->train_id = rm.n_trains;
    t->current_station = start_station;
    t->speed = 0.0; t->delay = 0.0;
    t->direction = direction;
    t->passengers = 50 + rm.n_trains * 20;
    t->freight_cars = rm.n_trains % 3;
    t->status = 1; t->active = 1;
    rm.n_trains++;
    print_str("[RM] Train "); print_int(rm.n_trains - 1);
    print_str(" station="); print_int(start_station);
    print_str(" dir="); print_int(direction);
    print_str(" pax="); print_int(t->passengers);
    print_str(" freight="); print_int(t->freight_cars); print_str("\n");
    return rm.n_trains - 1;
}

int rm_add_station(int platform_count) {
    if (rm.n_stations >= MAX_STATIONS) return -1;
    station_t* s = &stations[rm.n_stations];
    s->station_id = rm.n_stations;
    s->platform_count = platform_count;
    s->passengers_waiting = 30 + rm.n_stations * 10;
    s->dwell_time = 2.0 + (double)(rm.n_stations % 3);
    s->active = 1;
    rm.n_stations++;
    print_str("[RM] Station "); print_int(rm.n_stations - 1);
    print_str(" platforms="); print_int(platform_count);
    print_str(" waiting="); print_int(s->passengers_waiting);
    print_str(" dwell="); print_int((int)s->dwell_time); print_str("min\n");
    return rm.n_stations - 1;
}

int rm_add_track(int from, int to, double length, double speed_limit) {
    if (rm.n_tracks >= MAX_TRACKS) return -1;
    track_t* tr = &tracks[rm.n_tracks];
    tr->track_id = rm.n_tracks;
    tr->from_station = from;
    tr->to_station = to;
    tr->length = length; tr->speed_limit = speed_limit;
    tr->condition = 90.0 + (double)(rm.n_tracks % 4) * 2.0;
    tr->occupied = 0; tr->active = 1;
    rm.n_tracks++;
    print_str("[RM] Track "); print_int(rm.n_tracks - 1);
    print_str(" "); print_int(from); print_str("->"); print_int(to);
    print_str(" len="); print_int((int)length); print_str("km");
    print_str(" limit="); print_int((int)speed_limit); print_str("km/h");
    print_str(" cond="); print_int((int)tr->condition); print_str("%\n");
    return rm.n_tracks - 1;
}

void rm_train_schedule(int train_id) {
    if (train_id >= rm.n_trains) return;
    train_t* t = &trains[train_id];
    print_str("[RM] Schedule train "); print_int(train_id); print_str(":\n");
    int current = t->current_station;
    int dir = t->direction;
    for (int i = 0; i < 5 && current < rm.n_stations; i++) {
        print_str("  Station "); print_int(current);
        print_str(" ETA "); print_int(i * 15); print_str("min\n");
        current += dir;
        if (current >= rm.n_stations) dir = -1;
        if (current < 0) dir = 1;
    }
}

void rm_move_train(int train_id, int next_station) {
    if (train_id >= rm.n_trains) return;
    if (next_station >= rm.n_stations) return;
    train_t* t = &trains[train_id];
    int track_found = -1;
    for (int i = 0; i < rm.n_tracks; i++) {
        if (tracks[i].from_station == t->current_station &&
            tracks[i].to_station == next_station) {
            track_found = i;
            break;
        }
    }
    if (track_found < 0) {
        print_str("[RM] Train "); print_int(train_id);
        print_str(" no track to station "); print_int(next_station); print_str("\n");
        return;
    }
    tracks[track_found].occupied = 1;
    double travel_time = tracks[track_found].length / tracks[track_found].speed_limit * 60.0;
    t->speed = tracks[track_found].speed_limit;
    t->current_station = next_station;
    t->delay += (tracks[track_found].condition < 92.0) ? 2.0 : 0.0;
    rm.total_delay += t->delay;
    print_str("[RM] Train "); print_int(train_id);
    print_str(" -> station "); print_int(next_station);
    print_str(" speed="); print_int((int)t->speed); print_str("km/h");
    print_str(" time="); print_int((int)travel_time); print_str("min");
    print_str(" delay="); print_int((int)t->delay); print_str("min\n");
    tracks[track_found].occupied = 0;
    if (t->delay == 0.0) rm.on_time_count++;
}

int rm_add_passenger(int from, int to, int train_id) {
    if (rm.n_passengers >= MAX_PASSENGERS) return -1;
    passenger_t* p = &passengers[rm.n_passengers];
    p->passenger_id = rm.n_passengers;
    p->from_station = from; p->to_station = to;
    p->train_id = train_id;
    p->ticket_valid = 1;
    p->active = 1;
    rm.n_passengers++;
    if (train_id < rm.n_trains) trains[train_id].passengers++;
    print_str("[RM] Passenger "); print_int(rm.n_passengers - 1);
    print_str(" "); print_int(from); print_str("->"); print_int(to);
    print_str(" train="); print_int(train_id); print_str("\n");
    return rm.n_passengers - 1;
}

int rm_add_freight(int from, int to, int train_id, double weight) {
    if (rm.n_freight >= MAX_FREIGHT) return -1;
    freight_t* f = &freights[rm.n_freight];
    f->freight_id = rm.n_freight;
    f->from_station = from; f->to_station = to;
    f->train_id = train_id; f->weight = weight;
    f->type = rm.n_freight % 3 + 1;
    f->active = 1;
    rm.n_freight++;
    if (train_id < rm.n_trains) trains[train_id].freight_cars++;
    print_str("[RM] Freight "); print_int(rm.n_freight - 1);
    print_str(" "); print_int(from); print_str("->"); print_int(to);
    print_str(" train="); print_int(train_id);
    print_str(" weight="); print_int((int)weight); print_str("t\n");
    return rm.n_freight - 1;
}

void rm_track_monitoring(void) {
    print_str("[RM] Track monitoring:\n");
    int alerts = 0;
    for (int i = 0; i < rm.n_tracks; i++) {
        if (tracks[i].condition < 93.0) {
            print_str("  ALERT: Track "); print_int(i);
            print_str(" condition="); print_int((int)tracks[i].condition); print_str("%\n");
            alerts++;
        }
    }
    rm.safety_alerts += alerts;
    if (alerts == 0) print_str("  All tracks in good condition\n");
}

void rm_safety_check(void) {
    print_str("[RM] Safety check:\n");
    int occupied_count = 0;
    for (int i = 0; i < rm.n_tracks; i++) {
        if (tracks[i].occupied) occupied_count++;
    }
    print_str("  Occupied tracks: "); print_int(occupied_count); print_str("/"); print_int(rm.n_tracks); print_str("\n");
    print_str("  Safety alerts: "); print_int(rm.safety_alerts); print_str("\n");
    print_str("  On-time trains: "); print_int(rm.on_time_count); print_str("/"); print_int(rm.n_trains); print_str("\n");
}

void rm_print_state(void) {
    print_str("[RM] Trains="); print_int(rm.n_trains);
    print_str(" Stations="); print_int(rm.n_stations);
    print_str(" Tracks="); print_int(rm.n_tracks);
    print_str(" Pax="); print_int(rm.n_passengers);
    print_str(" Freight="); print_int(rm.n_freight);
    print_str(" Delay="); print_int((int)rm.total_delay); print_str("min\n");
}

int main(void) {
    print_str("=== Railway Management System Demo ===\n\n");
    rm_init();

    print_str("Adding stations...\n");
    rm_add_station(3);
    rm_add_station(2);
    rm_add_station(4);
    rm_add_station(2);
    rm_add_station(3);

    print_str("\nAdding tracks...\n");
    rm_add_track(0, 1, 50.0, 120.0);
    rm_add_track(1, 2, 80.0, 160.0);
    rm_add_track(2, 3, 60.0, 140.0);
    rm_add_track(3, 4, 70.0, 150.0);
    rm_add_track(1, 0, 50.0, 120.0);
    rm_add_track(2, 1, 80.0, 160.0);

    print_str("\nAdding trains...\n");
    rm_add_train(0, 1);
    rm_add_train(1, 1);
    rm_add_train(4, -1);
    rm_add_train(2, 1);

    print_str("\nTrain schedules...\n");
    rm_train_schedule(0);
    rm_train_schedule(1);
    rm_train_schedule(2);

    print_str("\nMoving trains...\n");
    rm_move_train(0, 1);
    rm_move_train(1, 2);
    rm_move_train(2, 1);
    rm_move_train(3, 3);

    print_str("\nAdding passengers...\n");
    rm_add_passenger(0, 2, 0);
    rm_add_passenger(0, 3, 0);
    rm_add_passenger(1, 4, 1);
    rm_add_passenger(2, 0, 2);
    rm_add_passenger(3, 1, 3);

    print_str("\nAdding freight...\n");
    rm_add_freight(0, 4, 0, 500.0);
    rm_add_freight(1, 3, 1, 300.0);
    rm_add_freight(2, 0, 2, 400.0);

    print_str("\nTrack monitoring...\n");
    rm_track_monitoring();

    print_str("\nSafety check...\n");
    rm_safety_check();

    rm_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
