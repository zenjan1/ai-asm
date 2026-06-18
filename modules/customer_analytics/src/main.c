/* customer_analytics: Customer analytics system (v1.0)
 * Customer profiles, acquisition, retention, recommendations, customer value
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

#define MAX_CUSTOMERS    16
#define MAX_SEGMENTS     8
#define MAX_CAMPAIGNS    8
#define MAX_CHANNELS     6
#define MAX_PRODUCTS     12
#define MAX_INTERACTIONS 24

typedef struct {
    int    customer_id;
    int    segment_id;
    int    age;
    int    gender;
    double income;
    int    tenure_months;
    double total_spend;
    double avg_order_value;
    int    n_purchases;
    int    loyalty_tier;
    int    nps_score;
    int    churn_risk;
    int    active;
} customer_t;

typedef struct {
    int    segment_id;
    int    type;
    int    n_customers;
    double avg_revenue;
    double avg_retention;
    int    active;
} segment_t;

typedef struct {
    int    campaign_id;
    int    channel;
    double budget;
    double spend;
    int    impressions;
    int    conversions;
    double revenue;
    int    active;
} campaign_t;

typedef struct {
    int    channel_id;
    int    type;
    int    impressions;
    int    clicks;
    int    conversions;
    double cost;
    double revenue;
    int    active;
} channel_t;

typedef struct {
    int    product_id;
    int    category;
    double price;
    int    units_sold;
    double revenue;
    int    active;
} product_t;

typedef struct {
    int    interaction_id;
    int    customer_id;
    int    product_id;
    int    type;
    double value;
    int    active;
} interaction_t;

typedef struct {
    int    n_customers;
    int    n_segments;
    int    n_campaigns;
    int    n_channels;
    int    n_products;
    int    n_interactions;
    double total_revenue;
    double total_customers_acquired;
    double avg_clv;
    double churn_rate;
    double avg_nps;
    int    vip_count;
} ca_state_t;

static customer_t customers[MAX_CUSTOMERS];
static segment_t segments[MAX_SEGMENTS];
static campaign_t campaigns[MAX_CAMPAIGNS];
static channel_t channels[MAX_CHANNELS];
static product_t products[MAX_PRODUCTS];
static interaction_t interactions[MAX_INTERACTIONS];
static ca_state_t ca;

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
static double dabs(double x) { return x < 0 ? -x : x; }
static double dsqrt(double x) {
    if (x <= 0.0) return 0.0;
    double g = x, p = x / g;
    for (int i = 0; i < 30; i++) { g = (g + p) * 0.5; p = x / g; }
    return g;
}

int ca_init(void) {
    if (initialized) return -1;
    ca.n_customers = 0; ca.n_segments = 0; ca.n_campaigns = 0;
    ca.n_channels = 0; ca.n_products = 0; ca.n_interactions = 0;
    ca.total_revenue = 0.0; ca.total_customers_acquired = 0.0;
    ca.avg_clv = 0.0; ca.churn_rate = 0.0; ca.avg_nps = 0.0;
    ca.vip_count = 0;
    for (int i = 0; i < MAX_CUSTOMERS; i++) customers[i].active = 0;
    for (int i = 0; i < MAX_SEGMENTS; i++) segments[i].active = 0;
    for (int i = 0; i < MAX_CAMPAIGNS; i++) campaigns[i].active = 0;
    for (int i = 0; i < MAX_CHANNELS; i++) channels[i].active = 0;
    for (int i = 0; i < MAX_PRODUCTS; i++) products[i].active = 0;
    for (int i = 0; i < MAX_INTERACTIONS; i++) interactions[i].active = 0;
    initialized = 1;
    print_str("[CA] Customer analytics initialized\n");
    return 0;
}

int ca_add_customer(int age, int gender, double income, int tenure) {
    if (ca.n_customers >= MAX_CUSTOMERS) return -1;
    customer_t* c = &customers[ca.n_customers];
    c->customer_id = ca.n_customers;
    c->age = age;
    c->gender = gender;
    c->income = income;
    c->tenure_months = tenure;
    c->n_purchases = 0;
    c->total_spend = 0.0;
    c->avg_order_value = 0.0;
    c->loyalty_tier = (tenure > 24) ? 3 : (tenure > 12) ? 2 : (tenure > 6) ? 1 : 0;
    c->nps_score = 5 + (tenure % 5);
    c->churn_risk = (tenure < 3) ? 40 : (tenure < 6) ? 25 : (tenure < 12) ? 15 : 8;
    c->segment_id = 0;
    c->active = 1;
    ca.n_customers++;
    ca.total_customers_acquired += 1.0;
    print_str("[CA] Customer "); print_int(ca.n_customers - 1);
    print_str(" age="); print_int(age);
    print_str(" gender="); print_int(gender);
    print_str(" income="); print_int((int)income);
    print_str(" tenure="); print_int(tenure); print_str("m");
    print_str(" tier="); print_int(c->loyalty_tier);
    print_str(" NPS="); print_int(c->nps_score); print_str("\n");
    return ca.n_customers - 1;
}

void ca_customer_purchase(int customer_id, int product_id, double amount) {
    if (customer_id >= ca.n_customers || product_id >= ca.n_products) return;
    customer_t* c = &customers[customer_id];
    c->n_purchases++;
    c->total_spend += amount;
    c->avg_order_value = c->total_spend / (double)c->n_purchases;
    ca.total_revenue += amount;
    products[product_id].units_sold++;
    products[product_id].revenue += amount;
    if (ca.n_interactions < MAX_INTERACTIONS) {
        interaction_t* it = &interactions[ca.n_interactions];
        it->interaction_id = ca.n_interactions;
        it->customer_id = customer_id;
        it->product_id = product_id;
        it->type = 1;
        it->value = amount;
        it->active = 1;
        ca.n_interactions++;
    }
    print_str("[CA] Purchase C"); print_int(customer_id);
    print_str(" P"); print_int(product_id);
    print_str(" amt="); print_int((int)amount); print_str("\n");
}

int ca_create_segment(int type) {
    if (ca.n_segments >= MAX_SEGMENTS) return -1;
    segment_t* s = &segments[ca.n_segments];
    s->segment_id = ca.n_segments;
    s->type = type;
    s->n_customers = 0;
    s->avg_revenue = 0.0;
    s->avg_retention = 0.0;
    s->active = 1;
    ca.n_segments++;
    print_str("[CA] Segment "); print_int(ca.n_segments - 1);
    print_str(" type="); print_int(type); print_str(" created\n");
    return ca.n_segments - 1;
}

void ca_assign_segments(void) {
    print_str("[CA] Segment assignment:\n");
    for (int i = 0; i < ca.n_customers; i++) {
        customer_t* c = &customers[i];
        int seg = 0;
        if (c->loyalty_tier >= 3 && c->total_spend > 5000.0) seg = 0;
        else if (c->total_spend > 2000.0) seg = 1;
        else if (c->n_purchases > 3) seg = 2;
        else if (c->churn_risk > 20) seg = 3;
        else seg = 4;
        c->segment_id = seg;
        if (seg < ca.n_segments) segments[seg].n_customers++;
        print_str("  C"); print_int(i);
        print_str(" -> seg "); print_int(seg);
        print_str(" (tier="); print_int(c->loyalty_tier);
        print_str(" spend="); print_int((int)c->total_spend);
        print_str(" risk="); print_int(c->churn_risk); print_str("%)\n");
    }
    double total_rev = 0.0;
    for (int i = 0; i < ca.n_segments; i++) {
        double seg_rev = 0.0;
        int seg_cnt = 0;
        for (int j = 0; j < ca.n_customers; j++) {
            if (customers[j].segment_id == i) {
                seg_rev += customers[j].total_spend;
                seg_cnt++;
            }
        }
        segments[i].avg_revenue = (seg_cnt > 0) ? seg_rev / seg_cnt : 0.0;
        total_rev += seg_rev;
        print_str("  Segment "); print_int(i);
        print_str(" n="); print_int(segments[i].n_customers);
        print_str(" avg_rev="); print_int((int)segments[i].avg_revenue); print_str("\n");
    }
}

int ca_create_campaign(int channel, double budget) {
    if (ca.n_campaigns >= MAX_CAMPAIGNS) return -1;
    campaign_t* c = &campaigns[ca.n_campaigns];
    c->campaign_id = ca.n_campaigns;
    c->channel = channel;
    c->budget = budget;
    c->spend = 0.0;
    c->impressions = 0;
    c->conversions = 0;
    c->revenue = 0.0;
    c->active = 1;
    ca.n_campaigns++;
    print_str("[CA] Campaign "); print_int(ca.n_campaigns - 1);
    print_str(" channel="); print_int(channel);
    print_str(" budget="); print_int((int)budget); print_str("\n");
    return ca.n_campaigns - 1;
}

void ca_run_campaign(int campaign_id, int impressions, int conversions, double revenue) {
    if (campaign_id >= ca.n_campaigns) return;
    campaign_t* c = &campaigns[campaign_id];
    c->impressions = impressions;
    c->conversions = conversions;
    c->revenue = revenue;
    c->spend = c->budget * 0.8;
    double roi = (revenue - c->spend) / (c->spend + 1e-10) * 100.0;
    double conv_rate = (impressions > 0) ? (double)conversions / impressions * 100.0 : 0.0;
    print_str("[CA] Campaign "); print_int(campaign_id);
    print_str(" impr="); print_int(impressions);
    print_str(" conv="); print_int(conversions);
    print_str(" rate="); print_int((int)conv_rate); print_str("%");
    print_str(" rev="); print_int((int)revenue);
    print_str(" ROI="); print_int((int)roi); print_str("%\n");
}

int ca_add_channel(int type, double cost) {
    if (ca.n_channels >= MAX_CHANNELS) return -1;
    channel_t* ch = &channels[ca.n_channels];
    ch->channel_id = ca.n_channels;
    ch->type = type;
    ch->impressions = 0;
    ch->clicks = 0;
    ch->conversions = 0;
    ch->cost = cost;
    ch->revenue = 0.0;
    ch->active = 1;
    ca.n_channels++;
    print_str("[CA] Channel "); print_int(ca.n_channels - 1);
    print_str(" type="); print_int(type);
    print_str(" cost="); print_int((int)cost); print_str("\n");
    return ca.n_channels - 1;
}

void ca_ab_test(int campaign_a, int campaign_b) {
    if (campaign_a >= ca.n_campaigns || campaign_b >= ca.n_campaigns) return;
    campaign_t* a = &campaigns[campaign_a];
    campaign_t* b = &campaigns[campaign_b];
    double rate_a = (a->impressions > 0) ? (double)a->conversions / a->impressions * 100.0 : 0.0;
    double rate_b = (b->impressions > 0) ? (double)b->conversions / b->impressions * 100.0 : 0.0;
    int winner = (rate_a > rate_b) ? campaign_a : campaign_b;
    double lift = dabs(rate_a - rate_b);
    print_str("[CA] A/B test A="); print_int(campaign_a);
    print_str(" B="); print_int(campaign_b); print_str("\n");
    print_str("  A conv rate: "); print_int((int)(rate_a * 100)); print_str("bp\n");
    print_str("  B conv rate: "); print_int((int)(rate_b * 100)); print_str("bp\n");
    print_str("  Winner: "); print_int(winner);
    print_str(" lift="); print_int((int)(lift * 100)); print_str("bp\n");
}

int ca_add_product(int category, double price) {
    if (ca.n_products >= MAX_PRODUCTS) return -1;
    product_t* p = &products[ca.n_products];
    p->product_id = ca.n_products;
    p->category = category;
    p->price = price;
    p->units_sold = 0;
    p->revenue = 0.0;
    p->active = 1;
    ca.n_products++;
    print_str("[CA] Product "); print_int(ca.n_products - 1);
    print_str(" cat="); print_int(category);
    print_str(" price="); print_int((int)price); print_str("\n");
    return ca.n_products - 1;
}

void ca_recommendation(int customer_id) {
    if (customer_id >= ca.n_customers) return;
    customer_t* c = &customers[customer_id];
    print_str("[CA] Recommendations for C"); print_int(customer_id);
    print_str(" (tier="); print_int(c->loyalty_tier);
    print_str(" purchases="); print_int(c->n_purchases); print_str("):\n");
    int best_cat = c->customer_id % 4;
    for (int i = 0; i < ca.n_products && i < 4; i++) {
        int score = (products[i].category == best_cat) ? 90 : 50 + (i * 3) % 30;
        print_str("  P"); print_int(products[i].product_id);
        print_str(" score="); print_int(score);
        print_str(" price="); print_int((int)products[i].price); print_str("\n");
    }
}

void ca_clv_calculation(void) {
    print_str("[CA] Customer Lifetime Value:\n");
    double sum_clv = 0.0;
    int vip_count = 0;
    for (int i = 0; i < ca.n_customers; i++) {
        customer_t* c = &customers[i];
        double avg_monthly = c->total_spend / (double)(c->tenure_months + 1);
        double retention_rate = 0.8 + (double)c->loyalty_tier * 0.05;
        double clv = avg_monthly * retention_rate * 12.0 * (double)(c->loyalty_tier + 1);
        sum_clv += clv;
        if (clv > 3000.0) vip_count++;
        print_str("  C"); print_int(i);
        print_str(" CLV="); print_int((int)clv);
        print_str(" avg_m="); print_int((int)avg_monthly);
        print_str(" ret="); print_int((int)(retention_rate * 100)); print_str("%\n");
    }
    ca.avg_clv = (ca.n_customers > 0) ? sum_clv / ca.n_customers : 0.0;
    ca.vip_count = vip_count;
    print_str("  Avg CLV: "); print_int((int)ca.avg_clv); print_str("\n");
    print_str("  VIP count: "); print_int(vip_count); print_str("\n");
}

void ca_churn_analysis(void) {
    print_str("[CA] Churn analysis:\n");
    double total_risk = 0.0;
    int at_risk = 0;
    for (int i = 0; i < ca.n_customers; i++) {
        customer_t* c = &customers[i];
        total_risk += c->churn_risk;
        if (c->churn_risk > 25) at_risk++;
    }
    ca.churn_rate = (ca.n_customers > 0) ? total_risk / ca.n_customers : 0.0;
    print_str("  Avg churn risk: "); print_int((int)ca.churn_rate); print_str("%\n");
    print_str("  At-risk customers: "); print_int(at_risk); print_str("\n");
    print_str("  Retention strategy: ");
    if (ca.churn_rate > 30) print_str("AGGRESSIVE\n");
    else if (ca.churn_rate > 20) print_str("ACTIVE\n");
    else print_str("MONITOR\n");
}

void ca_nps_summary(void) {
    print_str("[CA] NPS Summary:\n");
    int promoters = 0, passives = 0, detractors = 0;
    double sum_nps = 0.0;
    for (int i = 0; i < ca.n_customers; i++) {
        int score = customers[i].nps_score;
        sum_nps += score;
        if (score >= 9) promoters++;
        else if (score >= 7) passives++;
        else detractors++;
    }
    ca.avg_nps = (ca.n_customers > 0) ? sum_nps / ca.n_customers : 0.0;
    int nps = (int)((double)(promoters - detractors) / (ca.n_customers + 1e-10) * 100.0);
    print_str("  Promoters: "); print_int(promoters); print_str("\n");
    print_str("  Passives: "); print_int(passives); print_str("\n");
    print_str("  Detractors: "); print_int(detractors); print_str("\n");
    print_str("  NPS score: "); print_int(nps); print_str("\n");
    print_str("  Avg score: "); print_int((int)ca.avg_nps); print_str("\n");
}

void ca_print_state(void) {
    print_str("[CA] Customers="); print_int(ca.n_customers);
    print_str(" Segments="); print_int(ca.n_segments);
    print_str(" Campaigns="); print_int(ca.n_campaigns);
    print_str(" Channels="); print_int(ca.n_channels);
    print_str(" Products="); print_int(ca.n_products);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)ca.total_revenue); print_str("\n");
    print_str("  Avg CLV: "); print_int((int)ca.avg_clv); print_str("\n");
    print_str("  VIP: "); print_int(ca.vip_count); print_str("\n");
    print_str("  Churn rate: "); print_int((int)ca.churn_rate); print_str("%\n");
    print_str("  Avg NPS: "); print_int((int)ca.avg_nps); print_str("\n");
}

int main(void) {
    print_str("=== Customer Analytics System Demo ===\n\n");
    ca_init();

    print_str("Adding customers...\n");
    ca_add_customer(35, 1, 80000.0, 36);
    ca_add_customer(28, 2, 60000.0, 24);
    ca_add_customer(42, 1, 120000.0, 48);
    ca_add_customer(31, 2, 55000.0, 12);
    ca_add_customer(25, 1, 45000.0, 6);
    ca_add_customer(38, 2, 90000.0, 18);
    ca_add_customer(50, 1, 150000.0, 60);
    ca_add_customer(29, 2, 52000.0, 3);

    print_str("\nAdding products...\n");
    ca_add_product(1, 50.0);
    ca_add_product(1, 75.0);
    ca_add_product(2, 120.0);
    ca_add_product(2, 200.0);
    ca_add_product(3, 30.0);
    ca_add_product(3, 45.0);
    ca_add_product(4, 500.0);
    ca_add_product(4, 800.0);

    print_str("\nCustomer purchases...\n");
    ca_customer_purchase(0, 0, 50.0);
    ca_customer_purchase(0, 2, 120.0);
    ca_customer_purchase(0, 6, 500.0);
    ca_customer_purchase(1, 1, 75.0);
    ca_customer_purchase(1, 3, 200.0);
    ca_customer_purchase(2, 6, 500.0);
    ca_customer_purchase(2, 7, 800.0);
    ca_customer_purchase(2, 2, 120.0);
    ca_customer_purchase(3, 4, 30.0);
    ca_customer_purchase(4, 5, 45.0);
    ca_customer_purchase(5, 1, 75.0);
    ca_customer_purchase(6, 7, 800.0);
    ca_customer_purchase(6, 6, 500.0);
    ca_customer_purchase(6, 3, 200.0);
    ca_customer_purchase(7, 0, 50.0);

    print_str("\nCreating segments...\n");
    ca_create_segment(1);
    ca_create_segment(2);
    ca_create_segment(3);
    ca_create_segment(4);
    ca_create_segment(5);

    print_str("\nAssigning segments...\n");
    ca_assign_segments();

    print_str("\nAdding channels...\n");
    ca_add_channel(1, 5000.0);
    ca_add_channel(2, 3000.0);
    ca_add_channel(3, 8000.0);
    ca_add_channel(4, 2000.0);

    print_str("\nRunning campaigns...\n");
    ca_create_campaign(0, 10000.0);
    ca_create_campaign(1, 8000.0);
    ca_create_campaign(2, 12000.0);
    ca_create_campaign(3, 5000.0);
    ca_run_campaign(0, 50000, 250, 15000.0);
    ca_run_campaign(1, 30000, 180, 12000.0);
    ca_run_campaign(2, 60000, 420, 25000.0);
    ca_run_campaign(3, 20000, 80, 4000.0);

    print_str("\nA/B testing...\n");
    ca_ab_test(0, 1);
    ca_ab_test(2, 3);

    print_str("\nCustomer recommendations...\n");
    ca_recommendation(0);
    ca_recommendation(2);
    ca_recommendation(6);

    print_str("\nCLV calculation...\n");
    ca_clv_calculation();

    print_str("\nChurn analysis...\n");
    ca_churn_analysis();

    print_str("\nNPS summary...\n");
    ca_nps_summary();

    print_str("\nFinal state...\n");
    ca_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
