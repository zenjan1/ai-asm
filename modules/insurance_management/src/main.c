/* insurance_management: Insurance policy management (v1.0)
 * Products, policies, claims, agents, renewals, analytics
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

#define MAX_PRODUCTS  8
#define MAX_POLICIES  16
#define MAX_CLIENTS   14
#define MAX_CLAIMS    12
#define MAX_AGENTS    8

typedef struct {
    int    product_id;
    int    type;
    double premium;
    double coverage;
    int    term_months;
    int    n_sold;
    double revenue;
    int    active;
} product_t;

typedef struct {
    int    client_id;
    int    risk_level;
    int    n_policies;
    int    total_claims;
    double total_paid;
    int    active;
} client_t;

typedef struct {
    int    policy_id;
    int    client_id;
    int    product_id;
    int    agent_id;
    int    start_day;
    int    term_months;
    double premium;
    double coverage;
    int    status;
    int    active;
} policy_t;

typedef struct {
    int    claim_id;
    int    policy_id;
    int    client_id;
    int    type;
    double amount;
    int    day;
    int    status;
    int    active;
} claim_t;

typedef struct {
    int    agent_id;
    int    n_policies;
    int    n_claims;
    double premium_volume;
    double commission;
    int    rating;
    int    active;
} agent_t;

typedef struct {
    int    n_products;
    int    n_policies;
    int    n_clients;
    int    n_claims;
    int    n_agents;
    double premium_income;
    double claim_payout;
    double commission_total;
    double total_revenue;
    int    active_policies;
    int    pending_claims;
} im_state_t;

static product_t products[MAX_PRODUCTS];
static policy_t policies[MAX_POLICIES];
static client_t clients[MAX_CLIENTS];
static claim_t claims[MAX_CLAIMS];
static agent_t agents[MAX_AGENTS];
static im_state_t im;

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

int im_init(void) {
    if (initialized) return -1;
    im.n_products = 0; im.n_policies = 0; im.n_clients = 0;
    im.n_claims = 0; im.n_agents = 0;
    im.premium_income = 0.0; im.claim_payout = 0.0;
    im.commission_total = 0.0; im.total_revenue = 0.0;
    im.active_policies = 0; im.pending_claims = 0;
    for (int i = 0; i < MAX_PRODUCTS; i++) products[i].active = 0;
    for (int i = 0; i < MAX_POLICIES; i++) policies[i].active = 0;
    for (int i = 0; i < MAX_CLIENTS; i++) clients[i].active = 0;
    for (int i = 0; i < MAX_CLAIMS; i++) claims[i].active = 0;
    for (int i = 0; i < MAX_AGENTS; i++) agents[i].active = 0;
    initialized = 1;
    print_str("[IM] Insurance management initialized\n");
    return 0;
}

int im_add_product(int type, double premium, double coverage, int term) {
    if (im.n_products >= MAX_PRODUCTS) return -1;
    product_t* p = &products[im.n_products];
    p->product_id = im.n_products;
    p->type = type;
    p->premium = premium;
    p->coverage = coverage;
    p->term_months = term;
    p->n_sold = 0;
    p->revenue = 0.0;
    p->active = 1;
    im.n_products++;
    print_str("[IM] Product "); print_int(im.n_products - 1);
    print_str(" type="); print_int(type);
    print_str(" premium="); print_int((int)premium);
    print_str(" coverage="); print_int((int)coverage);
    print_str(" term="); print_int(term); print_str("mo\n");
    return im.n_products - 1;
}

int im_register_client(int risk_level) {
    if (im.n_clients >= MAX_CLIENTS) return -1;
    client_t* c = &clients[im.n_clients];
    c->client_id = im.n_clients;
    c->risk_level = risk_level;
    c->n_policies = 0;
    c->total_claims = 0;
    c->total_paid = 0.0;
    c->active = 1;
    im.n_clients++;
    print_str("[IM] Client "); print_int(im.n_clients - 1);
    print_str(" risk="); print_int(risk_level); print_str("\n");
    return im.n_clients - 1;
}

int im_add_agent(int rating) {
    if (im.n_agents >= MAX_AGENTS) return -1;
    agent_t* a = &agents[im.n_agents];
    a->agent_id = im.n_agents;
    a->n_policies = 0;
    a->n_claims = 0;
    a->premium_volume = 0.0;
    a->commission = 0.0;
    a->rating = rating;
    a->active = 1;
    im.n_agents++;
    print_str("[IM] Agent "); print_int(im.n_agents - 1);
    print_str(" rating="); print_int(rating); print_str("\n");
    return im.n_agents - 1;
}

int im_create_policy(int client_id, int product_id, int agent_id, int start_day) {
    if (im.n_policies >= MAX_POLICIES) return -1;
    if (client_id >= im.n_clients || product_id >= im.n_products) return -2;
    if (agent_id >= im.n_agents) return -3;
    product_t* p = &products[product_id];
    policy_t* pol = &policies[im.n_policies];
    pol->policy_id = im.n_policies;
    pol->client_id = client_id;
    pol->product_id = product_id;
    pol->agent_id = agent_id;
    pol->start_day = start_day;
    pol->term_months = p->term_months;
    double risk_mult[3] = {1.0, 1.3, 1.8};
    pol->premium = p->premium * risk_mult[clients[client_id].risk_level < 3 ? clients[client_id].risk_level : 0];
    pol->coverage = p->coverage;
    pol->status = 1;
    pol->active = 1;
    p->n_sold++;
    p->revenue += pol->premium;
    clients[client_id].n_policies++;
    clients[client_id].total_paid += pol->premium;
    agents[agent_id].n_policies++;
    agents[agent_id].premium_volume += pol->premium;
    double commission_rate = 0.10;
    double commission = pol->premium * commission_rate;
    agents[agent_id].commission += commission;
    im.premium_income += pol->premium;
    im.commission_total += commission;
    im.total_revenue += pol->premium - commission;
    im.active_policies++;
    im.n_policies++;
    print_str("[IM] Policy "); print_int(im.n_policies - 1);
    print_str(" C"); print_int(client_id);
    print_str(" P"); print_int(product_id);
    print_str(" A"); print_int(agent_id);
    print_str(" premium="); print_int((int)pol->premium);
    print_str(" coverage="); print_int((int)pol->coverage);
    print_str(" term="); print_int(pol->term_months); print_str("mo\n");
    return im.n_policies - 1;
}

int im_file_claim(int policy_id, int type, double amount, int day) {
    if (im.n_claims >= MAX_CLAIMS || policy_id >= im.n_policies) return -1;
    policy_t* pol = &policies[policy_id];
    claim_t* cl = &claims[im.n_claims];
    cl->claim_id = im.n_claims;
    cl->policy_id = policy_id;
    cl->client_id = pol->client_id;
    cl->type = type;
    cl->amount = amount;
    cl->day = day;
    cl->status = 1;
    cl->active = 1;
    clients[pol->client_id].total_claims++;
    agents[pol->agent_id].n_claims++;
    im.n_claims++;
    im.pending_claims++;
    print_str("[IM] Claim "); print_int(im.n_claims - 1);
    print_str(" Pol"); print_int(policy_id);
    print_str(" C"); print_int(pol->client_id);
    print_str(" type="); print_int(type);
    print_str(" amount="); print_int((int)amount);
    print_str(" day="); print_int(day); print_str("\n");
    return im.n_claims - 1;
}

int im_approve_claim(int claim_id) {
    if (claim_id >= im.n_claims) return -1;
    claim_t* cl = &claims[claim_id];
    if (cl->status != 1) return -2;
    policy_t* pol = &policies[cl->policy_id];
    double payout = cl->amount;
    if (payout > pol->coverage) payout = pol->coverage;
    cl->status = 3;
    im.claim_payout += payout;
    im.total_revenue -= payout;
    im.pending_claims--;
    print_str("[IM] Claim "); print_int(claim_id);
    print_str(" approved payout="); print_int((int)payout); print_str("\n");
    return 0;
}

int im_reject_claim(int claim_id) {
    if (claim_id >= im.n_claims) return -1;
    claims[claim_id].status = 4;
    im.pending_claims--;
    print_str("[IM] Claim "); print_int(claim_id); print_str(" rejected\n");
    return 0;
}

void im_renew_policy(int policy_id) {
    if (policy_id >= im.n_policies) return;
    policy_t* pol = &policies[policy_id];
    pol->start_day += pol->term_months;
    pol->status = 1;
    im.premium_income += pol->premium;
    im.total_revenue += pol->premium * 0.9;
    agents[pol->agent_id].premium_volume += pol->premium;
    clients[pol->client_id].total_paid += pol->premium;
    products[pol->product_id].n_sold++;
    products[pol->product_id].revenue += pol->premium;
    print_str("[IM] Policy "); print_int(policy_id);
    print_str(" renewed premium="); print_int((int)pol->premium); print_str("\n");
}

void im_product_report(void) {
    print_str("[IM] Product report:\n");
    for (int i = 0; i < im.n_products; i++) {
        product_t* p = &products[i];
        print_str("  P"); print_int(i);
        print_str(" type="); print_int(p->type);
        print_str(" premium="); print_int((int)p->premium);
        print_str(" coverage="); print_int((int)p->coverage);
        print_str(" sold="); print_int(p->n_sold);
        print_str(" rev="); print_int((int)p->revenue); print_str("\n");
    }
}

void im_agent_performance(void) {
    print_str("[IM] Agent performance:\n");
    for (int i = 0; i < im.n_agents; i++) {
        agent_t* a = &agents[i];
        print_str("  A"); print_int(i);
        print_str(" rating="); print_int(a->rating);
        print_str(" policies="); print_int(a->n_policies);
        print_str(" claims="); print_int(a->n_claims);
        print_str(" volume="); print_int((int)a->premium_volume);
        print_str(" commission="); print_int((int)a->commission); print_str("\n");
    }
}

void im_financial_report(void) {
    print_str("[IM] Financial report:\n");
    print_str("  Premium income: "); print_int((int)im.premium_income); print_str("\n");
    print_str("  Claim payouts: "); print_int((int)im.claim_payout); print_str("\n");
    print_str("  Commissions: "); print_int((int)im.commission_total); print_str("\n");
    print_str("  Net revenue: "); print_int((int)im.total_revenue); print_str("\n");
    print_str("  Active policies: "); print_int(im.active_policies); print_str("\n");
    print_str("  Pending claims: "); print_int(im.pending_claims); print_str("\n");
}

void im_print_state(void) {
    print_str("[IM] Products="); print_int(im.n_products);
    print_str(" Policies="); print_int(im.n_policies);
    print_str(" Clients="); print_int(im.n_clients);
    print_str(" Claims="); print_int(im.n_claims);
    print_str(" Agents="); print_int(im.n_agents);
    print_str("\n");
    print_str("  Net revenue: "); print_int((int)im.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Insurance Management Demo ===\n\n");
    im_init();

    print_str("Adding products...\n");
    im_add_product(1, 120.0, 50000.0, 12);
    im_add_product(2, 200.0, 100000.0, 12);
    im_add_product(3, 80.0, 30000.0, 6);
    im_add_product(4, 350.0, 250000.0, 24);
    im_add_product(1, 90.0, 40000.0, 12);
    im_add_product(2, 150.0, 75000.0, 12);
    im_add_product(3, 60.0, 20000.0, 6);
    im_add_product(4, 500.0, 500000.0, 24);

    print_str("\nRegistering clients...\n");
    for (int i = 0; i < 12; i++) {
        im_register_client(i % 3);
    }

    print_str("\nAdding agents...\n");
    im_add_agent(92);
    im_add_agent(88);
    im_add_agent(95);
    im_add_agent(85);
    im_add_agent(90);
    im_add_agent(87);

    print_str("\nCreating policies...\n");
    im_create_policy(0, 0, 0, 10);
    im_create_policy(1, 1, 1, 10);
    im_create_policy(2, 2, 2, 11);
    im_create_policy(3, 3, 0, 11);
    im_create_policy(4, 4, 3, 12);
    im_create_policy(5, 5, 4, 12);
    im_create_policy(6, 6, 5, 13);
    im_create_policy(7, 7, 1, 13);
    im_create_policy(8, 0, 2, 14);
    im_create_policy(9, 1, 3, 14);
    im_create_policy(10, 2, 4, 15);
    im_create_policy(11, 3, 5, 15);
    im_create_policy(0, 4, 0, 16);
    im_create_policy(1, 5, 1, 16);
    im_create_policy(2, 6, 2, 17);
    im_create_policy(3, 7, 3, 17);

    print_str("\nFiling claims...\n");
    im_file_claim(0, 1, 5000.0, 20);
    im_file_claim(1, 2, 15000.0, 22);
    im_file_claim(3, 1, 8000.0, 25);
    im_file_claim(5, 3, 25000.0, 28);
    im_file_claim(7, 1, 3000.0, 30);
    im_file_claim(2, 2, 12000.0, 32);
    im_file_claim(4, 1, 6000.0, 35);
    im_file_claim(6, 3, 30000.0, 38);
    im_file_claim(9, 1, 4500.0, 40);
    im_file_claim(11, 2, 18000.0, 42);

    print_str("\nApproving/rejecting claims...\n");
    im_approve_claim(0);
    im_approve_claim(1);
    im_reject_claim(2);
    im_approve_claim(3);
    im_approve_claim(4);
    im_approve_claim(5);
    im_reject_claim(6);
    im_approve_claim(7);

    print_str("\nRenewing policies...\n");
    im_renew_policy(0);
    im_renew_policy(2);
    im_renew_policy(4);
    im_renew_policy(6);
    im_renew_policy(8);

    print_str("\nProduct report...\n");
    im_product_report();

    print_str("\nAgent performance...\n");
    im_agent_performance();

    print_str("\nFinancial report...\n");
    im_financial_report();

    print_str("\nFinal state...\n");
    im_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
