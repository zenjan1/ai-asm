/* broadcast_network: Broadcast network management (v1.0)
 * Channels, programs, advertising, ratings, distribution, revenue
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

#define MAX_CHANNELS   10
#define MAX_PROGRAMS   16
#define MAX_AD_SLOTS   20
#define MAX_CONTRACTS  12
#define MAX_PLATFORMS  8

typedef struct {
    int    channel_id;
    int    type;
    int    bandwidth_mhz;
    int    coverage_pct;
    int    subscribers;
    double subscription_fee;
    double revenue;
    int    active;
} channel_t;

typedef struct {
    int    prog_id;
    int    channel_id;
    int    genre;
    int    duration_min;
    int    viewers_avg;
    double production_cost;
    int    episodes;
    int    timeslot;
    double ad_revenue;
    int    active;
} program_t;

typedef struct {
    int    slot_id;
    int    program_id;
    int    duration_sec;
    double rate_per_sec;
    int    position;
    double revenue;
    int    sold;
    int    active;
} ad_slot_t;

typedef struct {
    int    contract_id;
    int    advertiser_id;
    int    program_id;
    int    n_slots;
    double total_value;
    int    start_month;
    int    end_month;
    int    active;
} contract_t;

typedef struct {
    int    platform_id;
    int    type;
    int    n_subscribers;
    double fee_per_sub;
    double revenue;
    double distribution_cost;
    int    active;
} platform_t;

typedef struct {
    int    n_channels;
    int    n_programs;
    int    n_ad_slots;
    int    n_contracts;
    int    n_platforms;
    double subscription_revenue;
    double advertising_revenue;
    double distribution_revenue;
    double licensing_revenue;
    double total_revenue;
    double production_costs;
    double distribution_costs;
    double ops_costs;
    int    total_viewers;
    int    total_subscribers;
} bn_state_t;

static channel_t channels[MAX_CHANNELS];
static program_t programs[MAX_PROGRAMS];
static ad_slot_t ad_slots[MAX_AD_SLOTS];
static contract_t contracts[MAX_CONTRACTS];
static platform_t platforms[MAX_PLATFORMS];
static bn_state_t bn;

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

int bn_init(void) {
    if (initialized) return -1;
    bn.n_channels = 0; bn.n_programs = 0; bn.n_ad_slots = 0;
    bn.n_contracts = 0; bn.n_platforms = 0;
    bn.subscription_revenue = 0.0; bn.advertising_revenue = 0.0;
    bn.distribution_revenue = 0.0; bn.licensing_revenue = 0.0;
    bn.total_revenue = 0.0; bn.production_costs = 0.0;
    bn.distribution_costs = 0.0; bn.ops_costs = 0.0;
    bn.total_viewers = 0; bn.total_subscribers = 0;
    for (int i = 0; i < MAX_CHANNELS; i++) channels[i].active = 0;
    for (int i = 0; i < MAX_PROGRAMS; i++) programs[i].active = 0;
    for (int i = 0; i < MAX_AD_SLOTS; i++) ad_slots[i].active = 0;
    for (int i = 0; i < MAX_CONTRACTS; i++) contracts[i].active = 0;
    for (int i = 0; i < MAX_PLATFORMS; i++) platforms[i].active = 0;
    initialized = 1;
    print_str("[BN] Broadcast network initialized\n");
    return 0;
}

int bn_add_channel(int type, int bandwidth, int coverage, int subs, double fee) {
    if (bn.n_channels >= MAX_CHANNELS) return -1;
    channel_t* ch = &channels[bn.n_channels];
    ch->channel_id = bn.n_channels;
    ch->type = type;
    ch->bandwidth_mhz = bandwidth;
    ch->coverage_pct = coverage;
    ch->subscribers = subs;
    ch->subscription_fee = fee;
    ch->revenue = 0.0;
    ch->active = 1;
    bn.total_subscribers += subs;
    bn.ops_costs += 500.0;
    bn.n_channels++;
    print_str("[BN] Channel "); print_int(bn.n_channels - 1);
    print_str(" type="); print_int(type);
    print_str(" MHz="); print_int(bandwidth);
    print_str(" cov="); print_int(coverage);
    print_str("% subs="); print_int(subs); print_str("\n");
    return bn.n_channels - 1;
}

int bn_add_program(int channel_id, int genre, int duration, int viewers, double cost, int episodes, int timeslot) {
    if (bn.n_programs >= MAX_PROGRAMS || channel_id >= bn.n_channels) return -1;
    program_t* p = &programs[bn.n_programs];
    p->prog_id = bn.n_programs;
    p->channel_id = channel_id;
    p->genre = genre;
    p->duration_min = duration;
    p->viewers_avg = viewers;
    p->production_cost = cost;
    p->episodes = episodes;
    p->timeslot = timeslot;
    p->ad_revenue = 0.0;
    p->active = 1;
    bn.production_costs += cost * episodes;
    bn.total_viewers += viewers * episodes;
    bn.n_programs++;
    print_str("[BN] Program "); print_int(bn.n_programs - 1);
    print_str(" Ch"); print_int(channel_id);
    print_str(" genre="); print_int(genre);
    print_str(" min="); print_int(duration);
    print_str(" viewers="); print_int(viewers); print_str("\n");
    return bn.n_programs - 1;
}

int bn_create_ad_slot(int program_id, int duration_sec, double rate, int position) {
    if (bn.n_ad_slots >= MAX_AD_SLOTS || program_id >= bn.n_programs) return -1;
    ad_slot_t* a = &ad_slots[bn.n_ad_slots];
    a->slot_id = bn.n_ad_slots;
    a->program_id = program_id;
    a->duration_sec = duration_sec;
    a->rate_per_sec = rate;
    a->position = position;
    a->revenue = 0.0;
    a->sold = 0;
    a->active = 1;
    bn.n_ad_slots++;
    print_str("[BN] AdSlot "); print_int(bn.n_ad_slots - 1);
    print_str(" Pr"); print_int(program_id);
    print_str(" sec="); print_int(duration_sec);
    print_str(" rate=$"); print_int((int)rate);
    print_str("/s pos="); print_int(position); print_str("\n");
    return bn.n_ad_slots - 1;
}

int bn_sell_ad(int slot_id, int advertiser_id, int months) {
    if (slot_id >= bn.n_ad_slots) return -1;
    ad_slot_t* a = &ad_slots[slot_id];
    double rev = a->duration_sec * a->rate_per_sec * months;
    a->revenue += rev;
    a->sold = 1;
    programs[a->program_id].ad_revenue += rev;
    bn.advertising_revenue += rev;
    bn.total_revenue += rev;
    print_str("[BN] SellAd Sl"); print_int(slot_id);
    print_str(" Adv"); print_int(advertiser_id);
    print_str(" mo="); print_int(months);
    print_str(" $"); print_int((int)rev); print_str("\n");
    return 0;
}

int bn_add_contract(int advertiser_id, int program_id, int n_slots, double value, int start, int end) {
    if (bn.n_contracts >= MAX_CONTRACTS || program_id >= bn.n_programs) return -1;
    contract_t* c = &contracts[bn.n_contracts];
    c->contract_id = bn.n_contracts;
    c->advertiser_id = advertiser_id;
    c->program_id = program_id;
    c->n_slots = n_slots;
    c->total_value = value;
    c->start_month = start;
    c->end_month = end;
    c->active = 1;
    bn.advertising_revenue += value;
    bn.total_revenue += value;
    bn.n_contracts++;
    print_str("[BN] Contract "); print_int(bn.n_contracts - 1);
    print_str(" Adv"); print_int(advertiser_id);
    print_str(" Pr"); print_int(program_id);
    print_str(" slots="); print_int(n_slots);
    print_str(" $"); print_int((int)value); print_str("\n");
    return bn.n_contracts - 1;
}

int bn_add_platform(int type, int subs, double fee, double dist_cost) {
    if (bn.n_platforms >= MAX_PLATFORMS) return -1;
    platform_t* pl = &platforms[bn.n_platforms];
    pl->platform_id = bn.n_platforms;
    pl->type = type;
    pl->n_subscribers = subs;
    pl->fee_per_sub = fee;
    pl->revenue = subs * fee;
    pl->distribution_cost = dist_cost;
    pl->active = 1;
    bn.distribution_revenue += pl->revenue;
    bn.total_revenue += pl->revenue;
    bn.distribution_costs += dist_cost;
    bn.total_subscribers += subs;
    bn.n_platforms++;
    print_str("[BN] Platform "); print_int(bn.n_platforms - 1);
    print_str(" type="); print_int(type);
    print_str(" subs="); print_int(subs);
    print_str(" $"); print_int((int)pl->revenue); print_str("\n");
    return bn.n_platforms - 1;
}

int bn_collect_subscription(int channel_id) {
    if (channel_id >= bn.n_channels) return -1;
    channel_t* ch = &channels[channel_id];
    double rev = ch->subscribers * ch->subscription_fee;
    ch->revenue += rev;
    bn.subscription_revenue += rev;
    bn.total_revenue += rev;
    print_str("[BN] Sub Ch"); print_int(channel_id);
    print_str(" $"); print_int((int)rev); print_str("\n");
    return 0;
}

int bn_add_licensing(double amount) {
    bn.licensing_revenue += amount;
    bn.total_revenue += amount;
    print_str("[BN] License +$"); print_int((int)amount); print_str("\n");
    return 0;
}

void bn_channel_report(void) {
    print_str("[BN] Channel report:\n");
    for (int i = 0; i < bn.n_channels; i++) {
        channel_t* ch = &channels[i];
        print_str("  Ch"); print_int(i);
        print_str(" type="); print_int(ch->type);
        print_str(" subs="); print_int(ch->subscribers);
        print_str(" cov="); print_int(ch->coverage_pct);
        print_str("% rev="); print_int((int)ch->revenue); print_str("\n");
    }
    for (int i = 0; i < bn.n_programs; i++) {
        program_t* p = &programs[i];
        print_str("  Pr"); print_int(i);
        print_str(" Ch"); print_int(p->channel_id);
        print_str(" genre="); print_int(p->genre);
        print_str(" viewers="); print_int(p->viewers_avg);
        print_str(" ad=$"); print_int((int)p->ad_revenue); print_str("\n");
    }
}

void bn_financial_report(void) {
    print_str("[BN] Financial report:\n");
    print_str("  Subscription rev: "); print_int((int)bn.subscription_revenue); print_str("\n");
    print_str("  Advertising rev: "); print_int((int)bn.advertising_revenue); print_str("\n");
    print_str("  Distribution rev: "); print_int((int)bn.distribution_revenue); print_str("\n");
    print_str("  Licensing rev: "); print_int((int)bn.licensing_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)bn.total_revenue); print_str("\n");
    print_str("  Production costs: "); print_int((int)bn.production_costs); print_str("\n");
    print_str("  Distribution costs: "); print_int((int)bn.distribution_costs); print_str("\n");
    print_str("  Ops costs: "); print_int((int)bn.ops_costs); print_str("\n");
    double total_costs = bn.production_costs + bn.distribution_costs + bn.ops_costs;
    double profit = bn.total_revenue - total_costs;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    print_str("  Viewers: "); print_int(bn.total_viewers);
    print_str(" Subscribers: "); print_int(bn.total_subscribers); print_str("\n");
}

void bn_print_state(void) {
    print_str("[BN] Channels="); print_int(bn.n_channels);
    print_str(" Programs="); print_int(bn.n_programs);
    print_str(" Ads="); print_int(bn.n_ad_slots);
    print_str(" Contracts="); print_int(bn.n_contracts);
    print_str(" Platforms="); print_int(bn.n_platforms);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)bn.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Broadcast Network Demo ===\n\n");
    bn_init();

    print_str("Adding channels...\n");
    bn_add_channel(1, 6, 95, 2000000, 15.0);
    bn_add_channel(1, 6, 92, 1800000, 14.0);
    bn_add_channel(2, 8, 88, 1200000, 12.0);
    bn_add_channel(2, 8, 85, 1000000, 10.0);
    bn_add_channel(3, 4, 78, 800000, 8.0);
    bn_add_channel(3, 4, 75, 600000, 7.0);
    bn_add_channel(4, 6, 90, 1500000, 13.0);
    bn_add_channel(4, 6, 87, 1300000, 11.0);
    bn_add_channel(1, 8, 82, 900000, 9.0);
    bn_add_channel(2, 4, 70, 500000, 6.0);

    print_str("\nAdding programs...\n");
    bn_add_program(0, 1, 60, 1500000, 200000.0, 24, 1);
    bn_add_program(0, 2, 30, 800000, 100000.0, 52, 2);
    bn_add_program(1, 3, 45, 1200000, 180000.0, 13, 3);
    bn_add_program(1, 1, 60, 900000, 150000.0, 24, 4);
    bn_add_program(2, 4, 90, 600000, 250000.0, 10, 1);
    bn_add_program(2, 2, 30, 450000, 80000.0, 52, 2);
    bn_add_program(3, 5, 120, 350000, 300000.0, 8, 3);
    bn_add_program(3, 1, 60, 700000, 160000.0, 24, 4);
    bn_add_program(4, 3, 45, 500000, 120000.0, 13, 1);
    bn_add_program(4, 2, 30, 380000, 90000.0, 26, 2);
    bn_add_program(5, 4, 60, 280000, 200000.0, 10, 3);
    bn_add_program(6, 1, 60, 1100000, 190000.0, 24, 4);
    bn_add_program(7, 5, 90, 400000, 220000.0, 8, 1);
    bn_add_program(8, 2, 30, 600000, 110000.0, 52, 2);
    bn_add_program(9, 3, 45, 350000, 130000.0, 13, 3);
    bn_add_program(0, 1, 60, 1300000, 210000.0, 24, 4);

    print_str("\nCreating ad slots...\n");
    for (int i = 0; i < 20; i++) {
        int prog = i % 16;
        int dur = 15 + (i % 4) * 15;
        double rate = 2000.0 + (i % 5) * 500.0;
        int pos = 1 + (i % 3);
        bn_create_ad_slot(prog, dur, rate, pos);
    }

    print_str("\nSelling ads...\n");
    for (int i = 0; i < 20; i++) {
        bn_sell_ad(i, i % 8, 3 + (i % 6));
    }

    print_str("\nAdding contracts...\n");
    bn_add_contract(0, 0, 4, 500000.0, 1, 12);
    bn_add_contract(1, 2, 3, 350000.0, 1, 12);
    bn_add_contract(2, 4, 2, 250000.0, 3, 9);
    bn_add_contract(3, 6, 3, 400000.0, 1, 12);
    bn_add_contract(4, 8, 2, 200000.0, 2, 8);
    bn_add_contract(5, 10, 2, 180000.0, 4, 10);
    bn_add_contract(6, 1, 3, 380000.0, 1, 12);
    bn_add_contract(7, 3, 2, 280000.0, 2, 10);
    bn_add_contract(0, 5, 2, 220000.0, 3, 9);
    bn_add_contract(1, 7, 3, 320000.0, 1, 12);
    bn_add_contract(2, 9, 2, 190000.0, 4, 10);
    bn_add_contract(3, 11, 2, 350000.0, 1, 12);

    print_str("\nAdding platforms...\n");
    bn_add_platform(1, 3000000, 12.0, 1500000.0);
    bn_add_platform(2, 2500000, 10.0, 1200000.0);
    bn_add_platform(3, 1800000, 8.0, 800000.0);
    bn_add_platform(4, 4000000, 15.0, 2000000.0);
    bn_add_platform(1, 2000000, 11.0, 1000000.0);
    bn_add_platform(2, 1500000, 9.0, 700000.0);
    bn_add_platform(3, 1200000, 7.0, 500000.0);
    bn_add_platform(4, 3500000, 14.0, 1800000.0);

    print_str("\nCollecting subscriptions...\n");
    for (int i = 0; i < 10; i++) {
        bn_collect_subscription(i);
    }

    print_str("\nLicensing...\n");
    bn_add_licensing(400000.0);
    bn_add_licensing(350000.0);
    bn_add_licensing(500000.0);
    bn_add_licensing(280000.0);
    bn_add_licensing(450000.0);
    bn_add_licensing(320000.0);

    print_str("\nChannel report...\n");
    bn_channel_report();

    print_str("\nFinancial report...\n");
    bn_financial_report();

    print_str("\nFinal state...\n");
    bn_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
