/* brand_development: Brand development administration system (v1.0)
 * Brand management, quality brand, regional brand, enterprise brand, international brand
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

#define MAX_BRAND        16
#define MAX_QUALITY_BR   14
#define MAX_REGIONAL     12
#define MAX_ENTERPRISE   10
#define MAX_INTERNATIONAL 10

typedef struct {
    int    brand_id;
    int    brand_category;
    int    brand_level;
    int    brands_cultivated;
    int    brands_evaluated;
    int    brands_protected;
    int    year;
    int    active;
} brand_t;

typedef struct {
    int    quality_br_id;
    int    quality_type;
    int    improvement_area;
    int    quality_improvements;
    int    brands_created;
    int    brands_promoted;
    int    year;
    int    active;
} quality_br_t;

typedef struct {
    int    regional_id;
    int    regional_type;
    int    geographic_indicator;
    int    regional_brands;
    int    geo_indicators_registered;
    int    specialty_brands;
    int    year;
    int    active;
} regional_t;

typedef struct {
    int    enterprise_id;
    int    enterprise_type;
    int    product_category;
    int    enterprise_brands;
    int    product_brands;
    int    service_brands;
    int    year;
    int    active;
} enterprise_br_t;

typedef struct {
    int    intl_id;
    int    intl_type;
    int    market_region;
    int    intl_brands;
    int    export_brands;
    int    cooperation_brands;
    int    year;
    int    active;
} international_t;

typedef struct {
    int    n_brand;
    int    n_quality_br;
    int    n_regional;
    int    n_enterprise_br;
    int    n_international;
    int    total_cultivated;
    int    total_improvements;
    int    total_regional;
    int    total_enterprise;
    int    total_intl;
} bd_state_t;

static brand_t brands[MAX_BRAND];
static quality_br_t quality_brs[MAX_QUALITY_BR];
static regional_t regionals[MAX_REGIONAL];
static enterprise_br_t enterprise_brs[MAX_ENTERPRISE];
static international_t internationals[MAX_INTERNATIONAL];
static bd_state_t bd;

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

int bd_init(void) {
    if (initialized) return -1;
    bd.n_brand = 0; bd.n_quality_br = 0; bd.n_regional = 0;
    bd.n_enterprise_br = 0; bd.n_international = 0;
    bd.total_cultivated = 0; bd.total_improvements = 0;
    bd.total_regional = 0; bd.total_enterprise = 0;
    bd.total_intl = 0;
    for (int i = 0; i < MAX_BRAND; i++) brands[i].active = 0;
    for (int i = 0; i < MAX_QUALITY_BR; i++) quality_brs[i].active = 0;
    for (int i = 0; i < MAX_REGIONAL; i++) regionals[i].active = 0;
    for (int i = 0; i < MAX_ENTERPRISE; i++) enterprise_brs[i].active = 0;
    for (int i = 0; i < MAX_INTERNATIONAL; i++) internationals[i].active = 0;
    initialized = 1;
    print_str("[BD] Brand development initialized\n");
    return 0;
}

int bd_brand(int cat, int level, int cultivated, int evaluated, int protected_cnt, int year) {
    if (bd.n_brand >= MAX_BRAND) return -1;
    brand_t* b = &brands[bd.n_brand];
    b->brand_id = bd.n_brand;
    b->brand_category = cat;
    b->brand_level = level;
    b->brands_cultivated = cultivated;
    b->brands_evaluated = evaluated;
    b->brands_protected = protected_cnt;
    b->year = year;
    b->active = 1;
    bd.total_cultivated += cultivated;
    bd.n_brand++;
    print_str("[BD] Brand "); print_int(bd.n_brand - 1);
    print_str(" cat="); print_int(cat);
    print_str(" lvl="); print_int(level);
    print_str(" clt="); print_int(cultivated);
    print_str(" evl="); print_int(evaluated);
    print_str(" prt="); print_int(protected_cnt); print_str("\n");
    return bd.n_brand - 1;
}

int bd_quality(int quality_type, int area, int improvements, int created, int promoted, int year) {
    if (bd.n_quality_br >= MAX_QUALITY_BR) return -1;
    quality_br_t* q = &quality_brs[bd.n_quality_br];
    q->quality_br_id = bd.n_quality_br;
    q->quality_type = quality_type;
    q->improvement_area = area;
    q->quality_improvements = improvements;
    q->brands_created = created;
    q->brands_promoted = promoted;
    q->year = year;
    q->active = 1;
    bd.total_improvements += improvements;
    bd.n_quality_br++;
    print_str("[BD] Quality "); print_int(bd.n_quality_br - 1);
    print_str(" type="); print_int(quality_type);
    print_str(" area="); print_int(area);
    print_str(" imp="); print_int(improvements);
    print_str(" crt="); print_int(created);
    print_str(" prm="); print_int(promoted); print_str("\n");
    return bd.n_quality_br - 1;
}

int bd_regional(int reg_type, int geo_ind, int regional_cnt, int geo_registered, int specialty, int year) {
    if (bd.n_regional >= MAX_REGIONAL) return -1;
    regional_t* r = &regionals[bd.n_regional];
    r->regional_id = bd.n_regional;
    r->regional_type = reg_type;
    r->geographic_indicator = geo_ind;
    r->regional_brands = regional_cnt;
    r->geo_indicators_registered = geo_registered;
    r->specialty_brands = specialty;
    r->year = year;
    r->active = 1;
    bd.total_regional += regional_cnt;
    bd.n_regional++;
    print_str("[BD] Regional "); print_int(bd.n_regional - 1);
    print_str(" type="); print_int(reg_type);
    print_str(" geo="); print_int(geo_ind);
    print_str(" rgn="); print_int(regional_cnt);
    print_str(" grg="); print_int(geo_registered);
    print_str(" spc="); print_int(specialty); print_str("\n");
    return bd.n_regional - 1;
}

int bd_enterprise(int ent_type, int product, int ent_brands, int prod_brands, int svc_brands, int year) {
    if (bd.n_enterprise_br >= MAX_ENTERPRISE) return -1;
    enterprise_br_t* e = &enterprise_brs[bd.n_enterprise_br];
    e->enterprise_id = bd.n_enterprise_br;
    e->enterprise_type = ent_type;
    e->product_category = product;
    e->enterprise_brands = ent_brands;
    e->product_brands = prod_brands;
    e->service_brands = svc_brands;
    e->year = year;
    e->active = 1;
    bd.total_enterprise += ent_brands;
    bd.n_enterprise_br++;
    print_str("[BD] Enterprise "); print_int(bd.n_enterprise_br - 1);
    print_str(" type="); print_int(ent_type);
    print_str(" prd="); print_int(product);
    print_str(" ent="); print_int(ent_brands);
    print_str(" prb="); print_int(prod_brands);
    print_str(" svb="); print_int(svc_brands); print_str("\n");
    return bd.n_enterprise_br - 1;
}

int bd_international(int intl_type, int market, int intl_brands_cnt, int export, int cooperation, int year) {
    if (bd.n_international >= MAX_INTERNATIONAL) return -1;
    international_t* intl = &internationals[bd.n_international];
    intl->intl_id = bd.n_international;
    intl->intl_type = intl_type;
    intl->market_region = market;
    intl->intl_brands = intl_brands_cnt;
    intl->export_brands = export;
    intl->cooperation_brands = cooperation;
    intl->year = year;
    intl->active = 1;
    bd.total_intl += intl_brands_cnt;
    bd.n_international++;
    print_str("[BD] International "); print_int(bd.n_international - 1);
    print_str(" type="); print_int(intl_type);
    print_str(" mkt="); print_int(market);
    print_str(" int="); print_int(intl_brands_cnt);
    print_str(" exp="); print_int(export);
    print_str(" cop="); print_int(cooperation); print_str("\n");
    return bd.n_international - 1;
}

void bd_brand_report(void) {
    print_str("[BD] Brand report:\n");
    print_str("  Brand categories: "); print_int(bd.n_brand); print_str("\n");
    print_str("  Total brands cultivated: "); print_int(bd.total_cultivated); print_str("\n");
}

void bd_quality_report(void) {
    print_str("[BD] Quality report:\n");
    print_str("  Quality categories: "); print_int(bd.n_quality_br); print_str("\n");
    print_str("  Total quality improvements: "); print_int(bd.total_improvements); print_str("\n");
}

void bd_international_report(void) {
    print_str("[BD] International report:\n");
    print_str("  Regional brand categories: "); print_int(bd.n_regional); print_str("\n");
    print_str("  Total regional brands: "); print_int(bd.total_regional); print_str("\n");
    print_str("  Enterprise brand categories: "); print_int(bd.n_enterprise_br); print_str("\n");
    print_str("  Total enterprise brands: "); print_int(bd.total_enterprise); print_str("\n");
    print_str("  International categories: "); print_int(bd.n_international); print_str("\n");
    print_str("  Total international brands: "); print_int(bd.total_intl); print_str("\n");
}

void bd_print_state(void) {
    print_str("[BD] Br="); print_int(bd.n_brand);
    print_str(" Qu="); print_int(bd.n_quality_br);
    print_str(" Rg="); print_int(bd.n_regional);
    print_str(" En="); print_int(bd.n_enterprise_br);
    print_str(" In="); print_int(bd.n_international);
    print_str("\n");
}

int main(void) {
    print_str("=== Brand Development Demo ===\n\n");
    bd_init();

    print_str("Brand management...\n");
    for (int i = 0; i < 16; i++) {
        int cat = (i % 5) + 1;
        int lvl = (i % 4) + 1;
        int clt = 50 + (i * 12);
        int evl = 30 + (i * 8);
        int prt = 20 + (i * 5);
        int year = 2020 + (i % 5);
        bd_brand(cat, lvl, clt, evl, prt, year);
    }

    print_str("\nQuality brand...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int area = (i % 6) + 1;
        int imp = 40 + (i * 10);
        int crt = 25 + (i * 6);
        int prm = 15 + (i * 4);
        int year = 2021 + (i % 4);
        bd_quality(type, area, imp, crt, prm, year);
    }

    print_str("\nRegional brand...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int geo = (i % 5) + 1;
        int rgn = 20 + (i * 5);
        int grg = 10 + (i * 3);
        int spc = 15 + (i * 4);
        int year = 2022 + (i % 3);
        bd_regional(type, geo, rgn, grg, spc, year);
    }

    print_str("\nEnterprise brand...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int prd = (i % 5) + 1;
        int ent = 30 + (i * 8);
        int prb = 50 + (i * 12);
        int svb = 20 + (i * 5);
        int year = 2023 + (i % 2);
        bd_enterprise(type, prd, ent, prb, svb, year);
    }

    print_str("\nInternational brand...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int mkt = (i % 6) + 1;
        int intl_b = 15 + (i * 4);
        int exp = 10 + (i * 3);
        int cop = 5 + (i * 2);
        int year = 2024;
        bd_international(type, mkt, intl_b, exp, cop, year);
    }

    print_str("\nBrand report...\n");
    bd_brand_report();

    print_str("\nQuality report...\n");
    bd_quality_report();

    print_str("\nInternational report...\n");
    bd_international_report();

    print_str("\nFinal state...\n");
    bd_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
