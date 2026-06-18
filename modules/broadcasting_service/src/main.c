/* broadcasting_service: Broadcasting and TV/radio service (v1.0)
 * Channels, programs, transmission, ratings, emergency
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

#define MAX_CHANNELS   14
#define MAX_PROGRAMS   18
#define MAX_TOWERS     10
#define MAX_RATINGS    12
#define MAX_EMERGENCY  8

typedef struct {
    int    channel_id;
    int    type;
    int    frequency;
    int    zone;
    int    bandwidth;
    int    language;
    int    year_started;
    int    active;
} channel_t;

typedef struct {
    int    program_id;
    int    channel_id;
    int    type;
    int    duration_min;
    int    rating;
    int    viewers;
    int    air_hour;
    int    year;
    int    active;
} program_t;

typedef struct {
    int    tower_id;
    int    zone;
    int    height_m;
    int    power_kw;
    int    coverage_km;
    int    signal_quality;
    int    year_built;
    int    active;
} tower_t;

typedef struct {
    int    rating_id;
    int    channel_id;
    int    program_type;
    int    time_slot;
    int    viewers;
    int    share;
    int    year;
    int    month;
    int    active;
} rating_t;

typedef struct {
    int    emergency_id;
    int    type;
    int    zone;
    int    severity;
    int    channels_used;
    int    duration_min;
    int    reached;
    int    year;
    int    active;
} emergency_t;

typedef struct {
    int    n_channels;
    int    n_programs;
    int    n_towers;
    int    n_ratings;
    int    n_emergency;
    int    total_viewers;
    int    total_coverage;
    int    total_emergency_reached;
    int    avg_signal_quality;
    int    total_programs_aired;
} bs_state_t;

static channel_t channels[MAX_CHANNELS];
static program_t programs[MAX_PROGRAMS];
static tower_t towers[MAX_TOWERS];
static rating_t ratings[MAX_RATINGS];
static emergency_t emergencies[MAX_EMERGENCY];
static bs_state_t bs;

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

int bs_init(void) {
    if (initialized) return -1;
    bs.n_channels = 0; bs.n_programs = 0; bs.n_towers = 0;
    bs.n_ratings = 0; bs.n_emergency = 0;
    bs.total_viewers = 0; bs.total_coverage = 0;
    bs.total_emergency_reached = 0; bs.avg_signal_quality = 0;
    bs.total_programs_aired = 0;
    for (int i = 0; i < MAX_CHANNELS; i++) channels[i].active = 0;
    for (int i = 0; i < MAX_PROGRAMS; i++) programs[i].active = 0;
    for (int i = 0; i < MAX_TOWERS; i++) towers[i].active = 0;
    for (int i = 0; i < MAX_RATINGS; i++) ratings[i].active = 0;
    for (int i = 0; i < MAX_EMERGENCY; i++) emergencies[i].active = 0;
    initialized = 1;
    print_str("[BS] Broadcasting service initialized\n");
    return 0;
}

int bs_add_channel(int type, int frequency, int zone, int bandwidth, int language, int year) {
    if (bs.n_channels >= MAX_CHANNELS) return -1;
    channel_t* c = &channels[bs.n_channels];
    c->channel_id = bs.n_channels;
    c->type = type;
    c->frequency = frequency;
    c->zone = zone;
    c->bandwidth = bandwidth;
    c->language = language;
    c->year_started = year;
    c->active = 1;
    bs.n_channels++;
    print_str("[BS] Channel "); print_int(bs.n_channels - 1);
    print_str(" type="); print_int(type);
    print_str(" freq="); print_int(frequency);
    print_str(" zone="); print_int(zone);
    print_str(" bw="); print_int(bandwidth); print_str("MHz\n");
    return bs.n_channels - 1;
}

int bs_add_program(int channel_id, int type, int duration, int rating, int viewers, int hour, int year) {
    if (bs.n_programs >= MAX_PROGRAMS) return -1;
    program_t* p = &programs[bs.n_programs];
    p->program_id = bs.n_programs;
    p->channel_id = channel_id;
    p->type = type;
    p->duration_min = duration;
    p->rating = rating;
    p->viewers = viewers;
    p->air_hour = hour;
    p->year = year;
    p->active = 1;
    bs.total_viewers += viewers;
    bs.total_programs_aired++;
    bs.n_programs++;
    print_str("[BS] Prog "); print_int(bs.n_programs - 1);
    print_str(" ch="); print_int(channel_id);
    print_str(" type="); print_int(type);
    print_str(" dur="); print_int(duration); print_str("min");
    print_str(" view="); print_int(viewers); print_str("\n");
    return bs.n_programs - 1;
}

int bs_add_tower(int zone, int height, int power, int coverage, int signal, int year) {
    if (bs.n_towers >= MAX_TOWERS) return -1;
    tower_t* t = &towers[bs.n_towers];
    t->tower_id = bs.n_towers;
    t->zone = zone;
    t->height_m = height;
    t->power_kw = power;
    t->coverage_km = coverage;
    t->signal_quality = signal;
    t->year_built = year;
    t->active = 1;
    bs.total_coverage += coverage;
    if (bs.n_towers == 0) bs.avg_signal_quality = signal;
    else bs.avg_signal_quality = (bs.avg_signal_quality * bs.n_towers + signal) / (bs.n_towers + 1);
    bs.n_towers++;
    print_str("[BS] Tower "); print_int(bs.n_towers - 1);
    print_str(" zone="); print_int(zone);
    print_str(" h="); print_int(height); print_str("m");
    print_str(" pwr="); print_int(power); print_str("kW");
    print_str(" cov="); print_int(coverage); print_str("km\n");
    return bs.n_towers - 1;
}

int bs_record_rating(int channel_id, int program_type, int time_slot, int viewers, int share, int year, int month) {
    if (bs.n_ratings >= MAX_RATINGS) return -1;
    rating_t* r = &ratings[bs.n_ratings];
    r->rating_id = bs.n_ratings;
    r->channel_id = channel_id;
    r->program_type = program_type;
    r->time_slot = time_slot;
    r->viewers = viewers;
    r->share = share;
    r->year = year;
    r->month = month;
    r->active = 1;
    bs.total_viewers += viewers;
    bs.n_ratings++;
    print_str("[BS] Rating "); print_int(bs.n_ratings - 1);
    print_str(" ch="); print_int(channel_id);
    print_str(" slot="); print_int(time_slot);
    print_str(" view="); print_int(viewers);
    print_str(" share="); print_int(share); print_str("%\n");
    return bs.n_ratings - 1;
}

int bs_trigger_emergency(int type, int zone, int severity, int channels_used, int duration, int year) {
    if (bs.n_emergency >= MAX_EMERGENCY) return -1;
    emergency_t* e = &emergencies[bs.n_emergency];
    e->emergency_id = bs.n_emergency;
    e->type = type;
    e->zone = zone;
    e->severity = severity;
    e->channels_used = channels_used;
    e->duration_min = duration;
    e->reached = 0;
    e->year = year;
    e->active = 1;
    bs.n_emergency++;
    print_str("[BS] Emergency "); print_int(bs.n_emergency - 1);
    print_str(" type="); print_int(type);
    print_str(" zone="); print_int(zone);
    print_str(" sev="); print_int(severity);
    print_str(" ch="); print_int(channels_used); print_str("\n");
    return bs.n_emergency - 1;
}

int bs_emergency_reach(int emergency_id, int reached) {
    if (emergency_id >= bs.n_emergency) return -1;
    emergencies[emergency_id].reached = reached;
    bs.total_emergency_reached += reached;
    print_str("[BS] Reach E"); print_int(emergency_id);
    print_str(" ="); print_int(reached); print_str("\n");
    return 0;
}

void bs_channel_report(void) {
    print_str("[BS] Channel report:\n");
    print_str("  Channels: "); print_int(bs.n_channels); print_str("\n");
    print_str("  Towers: "); print_int(bs.n_towers); print_str("\n");
    print_str("  Coverage: "); print_int(bs.total_coverage); print_str(" km\n");
    print_str("  Avg signal: "); print_int(bs.avg_signal_quality); print_str("%\n");
}

void bs_program_report(void) {
    print_str("[BS] Program report:\n");
    print_str("  Programs aired: "); print_int(bs.total_programs_aired); print_str("\n");
    print_str("  Total viewers: "); print_int(bs.total_viewers); print_str("\n");
}

void bs_emergency_report(void) {
    print_str("[BS] Emergency report:\n");
    print_str("  Emergencies: "); print_int(bs.n_emergency); print_str("\n");
    print_str("  People reached: "); print_int(bs.total_emergency_reached); print_str("\n");
}

void bs_print_state(void) {
    print_str("[BS] Ch="); print_int(bs.n_channels);
    print_str(" Pr="); print_int(bs.n_programs);
    print_str(" Tw="); print_int(bs.n_towers);
    print_str(" Rt="); print_int(bs.n_ratings);
    print_str(" Em="); print_int(bs.n_emergency);
    print_str("\n");
}

int main(void) {
    print_str("=== Broadcasting Service Demo ===\n\n");
    bs_init();

    print_str("Adding channels...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 3) + 1;
        int freq = 88 + (i * 8);
        int zone = (i % 4) + 1;
        int bw = 6 + (i % 4) * 2;
        int lang = (i % 3) + 1;
        int year = 2000 + (i * 2);
        bs_add_channel(type, freq, zone, bw, lang, year);
    }

    print_str("\nAdding programs...\n");
    for (int i = 0; i < 18; i++) {
        int ch = i % 14;
        int type = (i % 5) + 1;
        int dur = 30 + (i * 10);
        int rating = 3 + (i % 5);
        int viewers = 50000 + (i * 20000);
        int hour = (i * 2) % 24;
        int year = 2024;
        bs_add_program(ch, type, dur, rating, viewers, hour, year);
    }

    print_str("\nAdding towers...\n");
    for (int i = 0; i < 10; i++) {
        int zone = (i % 4) + 1;
        int height = 50 + (i * 20);
        int power = 10 + (i * 5);
        int coverage = 30 + (i * 15);
        int signal = 80 + (i % 20);
        int year = 2005 + (i * 2);
        bs_add_tower(zone, height, power, coverage, signal, year);
    }

    print_str("\nRecording ratings...\n");
    for (int i = 0; i < 12; i++) {
        int ch = i % 14;
        int ptype = (i % 4) + 1;
        int slot = (i % 6) + 1;
        int viewers = 100000 + (i * 30000);
        int share = 5 + (i % 15);
        int year = 2024;
        int month = 1 + (i % 12);
        bs_record_rating(ch, ptype, slot, viewers, share, year, month);
    }

    print_str("\nTriggering emergencies...\n");
    for (int i = 0; i < 8; i++) {
        int type = (i % 3) + 1;
        int zone = (i % 4) + 1;
        int sev = (i % 4) + 1;
        int ch = 2 + (i % 5);
        int dur = 5 + (i * 3);
        int year = 2023 + (i % 2);
        bs_trigger_emergency(type, zone, sev, ch, dur, year);
    }

    print_str("\nEmergency reach...\n");
    for (int i = 0; i < 8; i++) {
        int reached = 50000 + (i * 20000);
        bs_emergency_reach(i, reached);
    }

    print_str("\nChannel report...\n");
    bs_channel_report();

    print_str("\nProgram report...\n");
    bs_program_report();

    print_str("\nEmergency report...\n");
    bs_emergency_report();

    print_str("\nFinal state...\n");
    bs_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
