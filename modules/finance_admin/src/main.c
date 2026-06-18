/* finance_admin: Finance administration system (v1.0)
 * Banking, securities, insurance, fintech, forex management
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

#define MAX_BANK         16
#define MAX_SECURITIES   14
#define MAX_INSURANCE    12
#define MAX_FINTECH      10
#define MAX_FOREX        10

typedef struct {
    int    bank_id;
    int    bank_type;
    int    region_id;
    int    assets;
    int    deposits;
    int    reserve_ratio;
    int    year;
    int    active;
} bank_t;

typedef struct {
    int    securities_id;
    int    exchange_id;
    int    issuer_id;
    int    stock_code;
    int    market_cap;
    int    volume;
    int    pe_ratio;
    int    year;
    int    active;
} securities_t;

typedef struct {
    int    insurance_id;
    int    company_id;
    int    product_type;
    int    premium;
    int    claims;
    int    risk_score;
    int    year;
    int    active;
} insurance_t;

typedef struct {
    int    fintech_id;
    int    provider_id;
    int    service_type;
    int    transactions;
    int    value;
    int    compliance;
    int    year;
    int    active;
} fintech_t;

typedef struct {
    int    forex_id;
    int    currency_pair;
    int    operation_type;
    int    amount;
    int    rate;
    int    reserve_impact;
    int    year;
    int    active;
} forex_t;

typedef struct {
    int    n_bank;
    int    n_securities;
    int    n_insurance;
    int    n_fintech;
    int    n_forex;
    int    total_assets;
    int    total_deposits;
    int    total_market_cap;
    int    total_premium;
    int    total_reserves;
} fa_state_t;

static bank_t banks[MAX_BANK];
static securities_t securities_list[MAX_SECURITIES];
static insurance_t insurances[MAX_INSURANCE];
static fintech_t fintechs[MAX_FINTECH];
static forex_t forexes[MAX_FOREX];
static fa_state_t fa;

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

int fa_init(void) {
    if (initialized) return -1;
    fa.n_bank = 0; fa.n_securities = 0; fa.n_insurance = 0;
    fa.n_fintech = 0; fa.n_forex = 0;
    fa.total_assets = 0; fa.total_deposits = 0;
    fa.total_market_cap = 0; fa.total_premium = 0;
    fa.total_reserves = 0;
    for (int i = 0; i < MAX_BANK; i++) banks[i].active = 0;
    for (int i = 0; i < MAX_SECURITIES; i++) securities_list[i].active = 0;
    for (int i = 0; i < MAX_INSURANCE; i++) insurances[i].active = 0;
    for (int i = 0; i < MAX_FINTECH; i++) fintechs[i].active = 0;
    for (int i = 0; i < MAX_FOREX; i++) forexes[i].active = 0;
    initialized = 1;
    print_str("[FA] Finance admin initialized\n");
    return 0;
}

int fa_bank(int bank_type, int region, int assets, int deposits, int reserve_ratio, int year) {
    if (fa.n_bank >= MAX_BANK) return -1;
    bank_t* b = &banks[fa.n_bank];
    b->bank_id = fa.n_bank;
    b->bank_type = bank_type;
    b->region_id = region;
    b->assets = assets;
    b->deposits = deposits;
    b->reserve_ratio = reserve_ratio;
    b->year = year;
    b->active = 1;
    fa.total_assets += assets;
    fa.total_deposits += deposits;
    fa.n_bank++;
    print_str("[FA] Bank "); print_int(fa.n_bank - 1);
    print_str(" type="); print_int(bank_type);
    print_str(" rgn="); print_int(region);
    print_str(" ast=$"); print_int(assets);
    print_str(" dep=$"); print_int(deposits);
    print_str(" rsv="); print_int(reserve_ratio); print_str("%\n");
    return fa.n_bank - 1;
}

int fa_securities(int exchange, int issuer, int stock_code, int market_cap, int volume, int pe_ratio, int year) {
    if (fa.n_securities >= MAX_SECURITIES) return -1;
    securities_t* s = &securities_list[fa.n_securities];
    s->securities_id = fa.n_securities;
    s->exchange_id = exchange;
    s->issuer_id = issuer;
    s->stock_code = stock_code;
    s->market_cap = market_cap;
    s->volume = volume;
    s->pe_ratio = pe_ratio;
    s->year = year;
    s->active = 1;
    fa.total_market_cap += market_cap;
    fa.n_securities++;
    print_str("[FA] Securities "); print_int(fa.n_securities - 1);
    print_str(" exc="); print_int(exchange);
    print_str(" isr="); print_int(issuer);
    print_str(" cod="); print_int(stock_code);
    print_str(" cap=$"); print_int(market_cap);
    print_str(" vol="); print_int(volume);
    print_str(" pe="); print_int(pe_ratio); print_str("\n");
    return fa.n_securities - 1;
}

int fa_insurance(int company, int product_type, int premium, int claims, int risk_score, int year) {
    if (fa.n_insurance >= MAX_INSURANCE) return -1;
    insurance_t* ins = &insurances[fa.n_insurance];
    ins->insurance_id = fa.n_insurance;
    ins->company_id = company;
    ins->product_type = product_type;
    ins->premium = premium;
    ins->claims = claims;
    ins->risk_score = risk_score;
    ins->year = year;
    ins->active = 1;
    fa.total_premium += premium;
    fa.n_insurance++;
    print_str("[FA] Insurance "); print_int(fa.n_insurance - 1);
    print_str(" cmp="); print_int(company);
    print_str(" type="); print_int(product_type);
    print_str(" prm=$"); print_int(premium);
    print_str(" clm=$"); print_int(claims);
    print_str(" rsk="); print_int(risk_score); print_str("\n");
    return fa.n_insurance - 1;
}

int fa_fintech(int provider, int service_type, int transactions, int value, int compliance, int year) {
    if (fa.n_fintech >= MAX_FINTECH) return -1;
    fintech_t* ft = &fintechs[fa.n_fintech];
    ft->fintech_id = fa.n_fintech;
    ft->provider_id = provider;
    ft->service_type = service_type;
    ft->transactions = transactions;
    ft->value = value;
    ft->compliance = compliance;
    ft->year = year;
    ft->active = 1;
    fa.n_fintech++;
    print_str("[FA] Fintech "); print_int(fa.n_fintech - 1);
    print_str(" prv="); print_int(provider);
    print_str(" type="); print_int(service_type);
    print_str(" txn="); print_int(transactions);
    print_str(" val=$"); print_int(value);
    print_str(" cpl="); print_int(compliance); print_str("\n");
    return fa.n_fintech - 1;
}

int fa_forex(int currency_pair, int op_type, int amount, int rate, int reserve_impact, int year) {
    if (fa.n_forex >= MAX_FOREX) return -1;
    forex_t* fx = &forexes[fa.n_forex];
    fx->forex_id = fa.n_forex;
    fx->currency_pair = currency_pair;
    fx->operation_type = op_type;
    fx->amount = amount;
    fx->rate = rate;
    fx->reserve_impact = reserve_impact;
    fx->year = year;
    fx->active = 1;
    fa.total_reserves += reserve_impact;
    fa.n_forex++;
    print_str("[FA] Forex "); print_int(fa.n_forex - 1);
    print_str(" pair="); print_int(currency_pair);
    print_str(" op="); print_int(op_type);
    print_str(" amt=$"); print_int(amount);
    print_str(" rate="); print_int(rate);
    print_str(" rsv="); print_int(reserve_impact); print_str("\n");
    return fa.n_forex - 1;
}

void fa_banking_report(void) {
    print_str("[FA] Banking report:\n");
    print_str("  Banks: "); print_int(fa.n_bank); print_str("\n");
    print_str("  Total assets: $"); print_int(fa.total_assets); print_str("\n");
    print_str("  Total deposits: $"); print_int(fa.total_deposits); print_str("\n");
}

void fa_securities_report(void) {
    print_str("[FA] Securities report:\n");
    print_str("  Listed companies: "); print_int(fa.n_securities); print_str("\n");
    print_str("  Total market cap: $"); print_int(fa.total_market_cap); print_str("\n");
}

void fa_insurance_report(void) {
    print_str("[FA] Insurance report:\n");
    print_str("  Insurance companies: "); print_int(fa.n_insurance); print_str("\n");
    print_str("  Total premium: $"); print_int(fa.total_premium); print_str("\n");
    print_str("  Fintech providers: "); print_int(fa.n_fintech); print_str("\n");
    print_str("  Forex operations: "); print_int(fa.n_forex); print_str("\n");
    print_str("  Total reserves: $"); print_int(fa.total_reserves); print_str("\n");
}

void fa_print_state(void) {
    print_str("[FA] Bk="); print_int(fa.n_bank);
    print_str(" Sc="); print_int(fa.n_securities);
    print_str(" In="); print_int(fa.n_insurance);
    print_str(" Ft="); print_int(fa.n_fintech);
    print_str(" Fx="); print_int(fa.n_forex);
    print_str("\n");
}

int main(void) {
    print_str("=== Finance Admin Demo ===\n\n");
    fa_init();

    print_str("Banking supervision...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 4) + 1;
        int rgn = (i % 6) + 1;
        int ast = 1000000 + (i * 500000);
        int dep = ast - (i * 100000);
        int rsv = 10 + (i % 5);
        int year = 2020 + (i % 5);
        fa_bank(type, rgn, ast, dep, rsv, year);
    }

    print_str("\nSecurities regulation...\n");
    for (int i = 0; i < 14; i++) {
        int exc = (i % 3) + 1;
        int isr = 1000 + (i * 11);
        int cod = 600000 + (i * 1000);
        int cap = 500000 + (i * 200000);
        int vol = 100000 + (i * 50000);
        int pe = 10 + (i * 2);
        int year = 2021 + (i % 4);
        fa_securities(exc, isr, cod, cap, vol, pe, year);
    }

    print_str("\nInsurance regulation...\n");
    for (int i = 0; i < 12; i++) {
        int cmp = 2000 + (i * 7);
        int type = (i % 4) + 1;
        int prm = 50000 + (i * 20000);
        int clm = prm - (i * 5000);
        int rsk = 30 + (i * 5);
        int year = 2022 + (i % 3);
        fa_insurance(cmp, type, prm, clm, rsk, year);
    }

    print_str("\nFintech supervision...\n");
    for (int i = 0; i < 10; i++) {
        int prv = 3000 + (i * 13);
        int type = (i % 4) + 1;
        int txn = 10000 + (i * 5000);
        int val = 1000000 + (i * 500000);
        int cpl = (i % 3 == 0) ? 0 : 1;
        int year = 2023 + (i % 2);
        fa_fintech(prv, type, txn, val, cpl, year);
    }

    print_str("\nForex management...\n");
    for (int i = 0; i < 10; i++) {
        int pair = (i % 5) + 1;
        int op = (i % 3) + 1;
        int amt = 500000 + (i * 200000);
        int rate = 600 + (i * 10);
        int rsv = (i % 2 == 0) ? 100000 : -50000;
        int year = 2024;
        fa_forex(pair, op, amt, rate, rsv, year);
    }

    print_str("\nBanking report...\n");
    fa_banking_report();

    print_str("\nSecurities report...\n");
    fa_securities_report();

    print_str("\nInsurance report...\n");
    fa_insurance_report();

    print_str("\nFinal state...\n");
    fa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
