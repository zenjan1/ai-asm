/* metaverse_admin: Metaverse administration system (v1.0)
 * Virtual worlds, interaction technology, economic system, content ecosystem, security governance
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

#define MAX_VIRTUAL      16
#define MAX_INTERACT     14
#define MAX_ECONOMY      12
#define MAX_CONTENT      10
#define MAX_SECURITY     10

typedef struct {
    int    virtual_id;
    int    virtual_type;
    int    virtual_category;
    int    virtual_space;
    int    virtual_asset;
    int    virtual_identity;
    int    year;
    int    active;
} virtual_t;

typedef struct {
    int    interact_id;
    int    interact_type;
    int    interact_category;
    int    vr_device;
    int    ar_device;
    int    mr_device;
    int    year;
    int    active;
} interact_t;

typedef struct {
    int    economy_id;
    int    economy_type;
    int    economy_category;
    int    virtual_currency;
    int    virtual_trade;
    int    virtual_asset_mgmt;
    int    year;
    int    active;
} economy_t;

typedef struct {
    int    content_id;
    int    content_type;
    int    content_category;
    int    content_create;
    int    social_manage;
    int    entertainment;
    int    year;
    int    active;
} content_t;

typedef struct {
    int    security_id;
    int    security_type;
    int    security_category;
    int    virtual_security;
    int    virtual_ethics;
    int    virtual_legal;
    int    year;
    int    active;
} security_t;

typedef struct {
    int    n_virtual;
    int    n_interact;
    int    n_economy;
    int    n_content;
    int    n_security;
    int    total_space;
    int    total_vr;
    int    total_currency;
    int    total_create;
    int    total_security;
} mva_state_t;

static virtual_t virtuals[MAX_VIRTUAL];
static interact_t interacts[MAX_INTERACT];
static economy_t economies[MAX_ECONOMY];
static content_t contents[MAX_CONTENT];
static security_t securities[MAX_SECURITY];
static mva_state_t mva;

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

int mva_init(void) {
    if (initialized) return -1;
    mva.n_virtual = 0; mva.n_interact = 0; mva.n_economy = 0;
    mva.n_content = 0; mva.n_security = 0;
    mva.total_space = 0; mva.total_vr = 0;
    mva.total_currency = 0; mva.total_create = 0;
    mva.total_security = 0;
    for (int i = 0; i < MAX_VIRTUAL; i++) virtuals[i].active = 0;
    for (int i = 0; i < MAX_INTERACT; i++) interacts[i].active = 0;
    for (int i = 0; i < MAX_ECONOMY; i++) economies[i].active = 0;
    for (int i = 0; i < MAX_CONTENT; i++) contents[i].active = 0;
    for (int i = 0; i < MAX_SECURITY; i++) securities[i].active = 0;
    initialized = 1;
    print_str("[MVA] Metaverse initialized\n");
    return 0;
}

int mva_virtual(int vrt_type, int cat, int space, int asset, int identity, int year) {
    if (mva.n_virtual >= MAX_VIRTUAL) return -1;
    virtual_t* v = &virtuals[mva.n_virtual];
    v->virtual_id = mva.n_virtual;
    v->virtual_type = vrt_type;
    v->virtual_category = cat;
    v->virtual_space = space;
    v->virtual_asset = asset;
    v->virtual_identity = identity;
    v->year = year;
    v->active = 1;
    mva.total_space += space;
    mva.n_virtual++;
    print_str("[MVA] Virtual "); print_int(mva.n_virtual - 1);
    print_str(" type="); print_int(vrt_type);
    print_str(" cat="); print_int(cat);
    print_str(" spc="); print_int(space);
    print_str(" ast="); print_int(asset);
    print_str(" idn="); print_int(identity); print_str("\n");
    return mva.n_virtual - 1;
}

int mva_interact(int itr_type, int cat, int vr, int ar, int mr, int year) {
    if (mva.n_interact >= MAX_INTERACT) return -1;
    interact_t* it = &interacts[mva.n_interact];
    it->interact_id = mva.n_interact;
    it->interact_type = itr_type;
    it->interact_category = cat;
    it->vr_device = vr;
    it->ar_device = ar;
    it->mr_device = mr;
    it->year = year;
    it->active = 1;
    mva.total_vr += vr;
    mva.n_interact++;
    print_str("[MVA] Interact "); print_int(mva.n_interact - 1);
    print_str(" type="); print_int(itr_type);
    print_str(" cat="); print_int(cat);
    print_str(" vr="); print_int(vr);
    print_str(" ar="); print_int(ar);
    print_str(" mr="); print_int(mr); print_str("\n");
    return mva.n_interact - 1;
}

int mva_economy(int eco_type, int cat, int currency, int trade, int asset_mgmt, int year) {
    if (mva.n_economy >= MAX_ECONOMY) return -1;
    economy_t* e = &economies[mva.n_economy];
    e->economy_id = mva.n_economy;
    e->economy_type = eco_type;
    e->economy_category = cat;
    e->virtual_currency = currency;
    e->virtual_trade = trade;
    e->virtual_asset_mgmt = asset_mgmt;
    e->year = year;
    e->active = 1;
    mva.total_currency += currency;
    mva.n_economy++;
    print_str("[MVA] Economy "); print_int(mva.n_economy - 1);
    print_str(" type="); print_int(eco_type);
    print_str(" cat="); print_int(cat);
    print_str(" cur="); print_int(currency);
    print_str(" trd="); print_int(trade);
    print_str(" ast="); print_int(asset_mgmt); print_str("\n");
    return mva.n_economy - 1;
}

int mva_content(int cnt_type, int cat, int create, int social, int entertain, int year) {
    if (mva.n_content >= MAX_CONTENT) return -1;
    content_t* c = &contents[mva.n_content];
    c->content_id = mva.n_content;
    c->content_type = cnt_type;
    c->content_category = cat;
    c->content_create = create;
    c->social_manage = social;
    c->entertainment = entertain;
    c->year = year;
    c->active = 1;
    mva.total_create += create;
    mva.n_content++;
    print_str("[MVA] Content "); print_int(mva.n_content - 1);
    print_str(" type="); print_int(cnt_type);
    print_str(" cat="); print_int(cat);
    print_str(" crt="); print_int(create);
    print_str(" scl="); print_int(social);
    print_str(" ent="); print_int(entertain); print_str("\n");
    return mva.n_content - 1;
}

int mva_security(int sec_type, int cat, int security, int ethics, int legal, int year) {
    if (mva.n_security >= MAX_SECURITY) return -1;
    security_t* s = &securities[mva.n_security];
    s->security_id = mva.n_security;
    s->security_type = sec_type;
    s->security_category = cat;
    s->virtual_security = security;
    s->virtual_ethics = ethics;
    s->virtual_legal = legal;
    s->year = year;
    s->active = 1;
    mva.total_security += security;
    mva.n_security++;
    print_str("[MVA] Security "); print_int(mva.n_security - 1);
    print_str(" type="); print_int(sec_type);
    print_str(" cat="); print_int(cat);
    print_str(" sec="); print_int(security);
    print_str(" eth="); print_int(ethics);
    print_str(" lgl="); print_int(legal); print_str("\n");
    return mva.n_security - 1;
}

void mva_virtual_report(void) {
    print_str("[MVA] Virtual report:\n");
    print_str("  Virtual world categories: "); print_int(mva.n_virtual); print_str("\n");
    print_str("  Total virtual spaces: "); print_int(mva.total_space); print_str("\n");
}

void mva_interact_report(void) {
    print_str("[MVA] Interact report:\n");
    print_str("  Interaction technology categories: "); print_int(mva.n_interact); print_str("\n");
    print_str("  Total VR devices: "); print_int(mva.total_vr); print_str("\n");
}

void mva_content_report(void) {
    print_str("[MVA] Content report:\n");
    print_str("  Economy categories: "); print_int(mva.n_economy); print_str("\n");
    print_str("  Total virtual currency: "); print_int(mva.total_currency); print_str("\n");
    print_str("  Content categories: "); print_int(mva.n_content); print_str("\n");
    print_str("  Total content creation: "); print_int(mva.total_create); print_str("\n");
    print_str("  Security categories: "); print_int(mva.n_security); print_str("\n");
    print_str("  Total virtual security: "); print_int(mva.total_security); print_str("\n");
}

void mva_print_state(void) {
    print_str("[MVA] Vr="); print_int(mva.n_virtual);
    print_str(" It="); print_int(mva.n_interact);
    print_str(" Ec="); print_int(mva.n_economy);
    print_str(" Cn="); print_int(mva.n_content);
    print_str(" Sc="); print_int(mva.n_security);
    print_str("\n");
}

int main(void) {
    print_str("=== Metaverse Admin Demo ===\n\n");
    mva_init();

    print_str("Virtual worlds...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int spc = 50 + (i * 12);
        int ast = 35 + (i * 8);
        int idn = 22 + (i * 5);
        int year = 2020 + (i % 5);
        mva_virtual(type, cat, spc, ast, idn, year);
    }

    print_str("\nInteraction technology...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int vr = 40 + (i * 10);
        int ar = 30 + (i * 7);
        int mr = 20 + (i * 5);
        int year = 2021 + (i % 4);
        mva_interact(type, cat, vr, ar, mr, year);
    }

    print_str("\nEconomic system...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cur = 45 + (i * 11);
        int trd = 32 + (i * 8);
        int ast = 22 + (i * 5);
        int year = 2022 + (i % 3);
        mva_economy(type, cat, cur, trd, ast, year);
    }

    print_str("\nContent ecosystem...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int crt = 28 + (i * 7);
        int scl = 20 + (i * 5);
        int ent = 15 + (i * 3);
        int year = 2023 + (i % 2);
        mva_content(type, cat, crt, scl, ent, year);
    }

    print_str("\nSecurity governance...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int sec = 25 + (i * 6);
        int eth = 18 + (i * 4);
        int lgl = 12 + (i * 3);
        int year = 2024;
        mva_security(type, cat, sec, eth, lgl, year);
    }

    print_str("\nVirtual report...\n");
    mva_virtual_report();

    print_str("\nInteract report...\n");
    mva_interact_report();

    print_str("\nContent report...\n");
    mva_content_report();

    print_str("\nFinal state...\n");
    mva_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
