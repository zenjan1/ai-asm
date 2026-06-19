/* environmental_economics_admin: Environmental economics administration system (v1.0)
 * Environmental economics, green economy, carbon trading, resource environment, sustainability
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

#define MAX_ENV_ECON   16
#define MAX_GRN_ECON   14
#define MAX_CARB_TRD   12
#define MAX_RES_ENV    10
#define MAX_SUS_DEV    10

typedef struct {
    int    ee_id;
    int    ee_type;
    int    ee_category;
    int    env_val;
    int    env_cos;
    int    ext_the;
    int    env_ben;
    int    year;
    int    active;
} env_econ_t;

typedef struct {
    int    ge_id;
    int    ge_type;
    int    ge_category;
    int    grn_the;
    int    cyc_eco;
    int    low_car;
    int    eco_com;
    int    year;
    int    active;
} grn_econ_t;

typedef struct {
    int    ct_id;
    int    ct_type;
    int    ct_category;
    int    carb_ri;
    int    carb_mk;
    int    carb_fn;
    int    carb_ac;
    int    year;
    int    active;
} carb_trd_t;

typedef struct {
    int    re_id;
    int    re_type;
    int    re_category;
    int    res_pri;
    int    res_tax;
    int    env_tax;
    int    eco_tax;
    int    year;
    int    active;
} res_env_t;

typedef struct {
    int    sd_id;
    int    sd_type;
    int    sd_category;
    int    sus_the;
    int    grn_fn;
    int    esg_inv;
    int    env_res;
    int    year;
    int    active;
} sus_dev_t;

typedef struct {
    int    n_env_econ;
    int    n_grn_econ;
    int    n_carb_trd;
    int    n_res_env;
    int    n_sus_dev;
    int    total_env_val;
    int    total_grn_the;
    int    total_carb_ri;
    int    total_res_pri;
    int    total_sus_the;
} env_state_t;

static env_econ_t env_econs[MAX_ENV_ECON];
static grn_econ_t grn_econs[MAX_GRN_ECON];
static carb_trd_t carb_trds[MAX_CARB_TRD];
static res_env_t  res_envs[MAX_RES_ENV];
static sus_dev_t  sus_devs[MAX_SUS_DEV];
static env_state_t env;

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

int env_init(void) {
    if (initialized) return -1;
    env.n_env_econ = 0; env.n_grn_econ = 0; env.n_carb_trd = 0;
    env.n_res_env = 0; env.n_sus_dev = 0;
    env.total_env_val = 0; env.total_grn_the = 0;
    env.total_carb_ri = 0; env.total_res_pri = 0;
    env.total_sus_the = 0;
    for (int i = 0; i < MAX_ENV_ECON; i++) env_econs[i].active = 0;
    for (int i = 0; i < MAX_GRN_ECON; i++) grn_econs[i].active = 0;
    for (int i = 0; i < MAX_CARB_TRD; i++) carb_trds[i].active = 0;
    for (int i = 0; i < MAX_RES_ENV; i++) res_envs[i].active = 0;
    for (int i = 0; i < MAX_SUS_DEV; i++) sus_devs[i].active = 0;
    initialized = 1;
    print_str("[ENV] Environmental economics initialized\n");
    return 0;
}

int env_env_econ(int ee_type, int cat, int val, int cos, int the, int ben, int year) {
    if (env.n_env_econ >= MAX_ENV_ECON) return -1;
    env_econ_t* e = &env_econs[env.n_env_econ];
    e->ee_id = env.n_env_econ;
    e->ee_type = ee_type;
    e->ee_category = cat;
    e->env_val = val;
    e->env_cos = cos;
    e->ext_the = the;
    e->env_ben = ben;
    e->year = year;
    e->active = 1;
    env.total_env_val += val;
    env.n_env_econ++;
    print_str("[ENV] Env econ "); print_int(env.n_env_econ - 1);
    print_str(" type="); print_int(ee_type);
    print_str(" cat="); print_int(cat);
    print_str(" val="); print_int(val);
    print_str(" cos="); print_int(cos);
    print_str(" the="); print_int(the);
    print_str(" ben="); print_int(ben); print_str("\n");
    return env.n_env_econ - 1;
}

int env_grn_econ(int ge_type, int cat, int gthe, int ceco, int lcar, int ecom, int year) {
    if (env.n_grn_econ >= MAX_GRN_ECON) return -1;
    grn_econ_t* g = &grn_econs[env.n_grn_econ];
    g->ge_id = env.n_grn_econ;
    g->ge_type = ge_type;
    g->ge_category = cat;
    g->grn_the = gthe;
    g->cyc_eco = ceco;
    g->low_car = lcar;
    g->eco_com = ecom;
    g->year = year;
    g->active = 1;
    env.total_grn_the += gthe;
    env.n_grn_econ++;
    print_str("[ENV] Grn econ "); print_int(env.n_grn_econ - 1);
    print_str(" type="); print_int(ge_type);
    print_str(" cat="); print_int(cat);
    print_str(" gthe="); print_int(gthe);
    print_str(" ceco="); print_int(ceco);
    print_str(" lcar="); print_int(lcar);
    print_str(" ecom="); print_int(ecom); print_str("\n");
    return env.n_grn_econ - 1;
}

int env_carb_trd(int ct_type, int cat, int cri, int cmk, int cfn, int cac, int year) {
    if (env.n_carb_trd >= MAX_CARB_TRD) return -1;
    carb_trd_t* c = &carb_trds[env.n_carb_trd];
    c->ct_id = env.n_carb_trd;
    c->ct_type = ct_type;
    c->ct_category = cat;
    c->carb_ri = cri;
    c->carb_mk = cmk;
    c->carb_fn = cfn;
    c->carb_ac = cac;
    c->year = year;
    c->active = 1;
    env.total_carb_ri += cri;
    env.n_carb_trd++;
    print_str("[ENV] Carb trd "); print_int(env.n_carb_trd - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" cri="); print_int(cri);
    print_str(" cmk="); print_int(cmk);
    print_str(" cfn="); print_int(cfn);
    print_str(" cac="); print_int(cac); print_str("\n");
    return env.n_carb_trd - 1;
}

int env_res_env(int re_type, int cat, int rpri, int rtax, int etax, int etax2, int year) {
    if (env.n_res_env >= MAX_RES_ENV) return -1;
    res_env_t* r = &res_envs[env.n_res_env];
    r->re_id = env.n_res_env;
    r->re_type = re_type;
    r->re_category = cat;
    r->res_pri = rpri;
    r->res_tax = rtax;
    r->env_tax = etax;
    r->eco_tax = etax2;
    r->year = year;
    r->active = 1;
    env.total_res_pri += rpri;
    env.n_res_env++;
    print_str("[ENV] Res env "); print_int(env.n_res_env - 1);
    print_str(" type="); print_int(re_type);
    print_str(" cat="); print_int(cat);
    print_str(" rpri="); print_int(rpri);
    print_str(" rtax="); print_int(rtax);
    print_str(" etax="); print_int(etax);
    print_str(" etax2="); print_int(etax2); print_str("\n");
    return env.n_res_env - 1;
}

int env_sus_dev(int sd_type, int cat, int sthe, int gfn, int esg, int eres, int year) {
    if (env.n_sus_dev >= MAX_SUS_DEV) return -1;
    sus_dev_t* s = &sus_devs[env.n_sus_dev];
    s->sd_id = env.n_sus_dev;
    s->sd_type = sd_type;
    s->sd_category = cat;
    s->sus_the = sthe;
    s->grn_fn = gfn;
    s->esg_inv = esg;
    s->env_res = eres;
    s->year = year;
    s->active = 1;
    env.total_sus_the += sthe;
    env.n_sus_dev++;
    print_str("[ENV] Sus dev "); print_int(env.n_sus_dev - 1);
    print_str(" type="); print_int(sd_type);
    print_str(" cat="); print_int(cat);
    print_str(" sthe="); print_int(sthe);
    print_str(" gfn="); print_int(gfn);
    print_str(" esg="); print_int(esg);
    print_str(" eres="); print_int(eres); print_str("\n");
    return env.n_sus_dev - 1;
}

void env_econ_report(void) {
    print_str("[ENV] Environmental economics report:\n");
    print_str("  Economics categories: "); print_int(env.n_env_econ); print_str("\n");
    print_str("  Total environmental value: "); print_int(env.total_env_val); print_str("\n");
}

void env_green_report(void) {
    print_str("[ENV] Green economy report:\n");
    print_str("  Green categories: "); print_int(env.n_grn_econ); print_str("\n");
    print_str("  Total green theory: "); print_int(env.total_grn_the); print_str("\n");
}

void env_full_report(void) {
    print_str("[ENV] Full report:\n");
    print_str("  Carbon trading categories: "); print_int(env.n_carb_trd); print_str("\n");
    print_str("  Total carbon rights: "); print_int(env.total_carb_ri); print_str("\n");
    print_str("  Resource categories: "); print_int(env.n_res_env); print_str("\n");
    print_str("  Total resource pricing: "); print_int(env.total_res_pri); print_str("\n");
    print_str("  Sustainability categories: "); print_int(env.n_sus_dev); print_str("\n");
    print_str("  Total sustainability theory: "); print_int(env.total_sus_the); print_str("\n");
}

void env_print_state(void) {
    print_str("[ENV] Ee="); print_int(env.n_env_econ);
    print_str(" Ge="); print_int(env.n_grn_econ);
    print_str(" Ct="); print_int(env.n_carb_trd);
    print_str(" Re="); print_int(env.n_res_env);
    print_str(" Sd="); print_int(env.n_sus_dev);
    print_str("\n");
}

int main(void) {
    print_str("=== Environmental Economics Admin Demo ===\n\n");
    env_init();

    print_str("Environmental economics...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int val = 62 + (i * 15);
        int cos = 48 + (i * 12);
        int the = 28 + (i * 7);
        int ben = 19 + (i * 5);
        int year = 2020 + (i % 5);
        env_env_econ(type, cat, val, cos, the, ben, year);
    }

    print_str("\nGreen economy...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int gthe = 52 + (i * 13);
        int ceco = 40 + (i * 10);
        int lcar = 24 + (i * 6);
        int ecom = 15 + (i * 4);
        int year = 2021 + (i % 4);
        env_grn_econ(type, cat, gthe, ceco, lcar, ecom, year);
    }

    print_str("\nCarbon trading...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cri = 44 + (i * 11);
        int cmk = 32 + (i * 8);
        int cfn = 18 + (i * 5);
        int cac = 12 + (i * 3);
        int year = 2022 + (i % 3);
        env_carb_trd(type, cat, cri, cmk, cfn, cac, year);
    }

    print_str("\nResource environment...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int rpri = 38 + (i * 9);
        int rtax = 27 + (i * 7);
        int etax = 16 + (i * 4);
        int etax2 = 11 + (i * 3);
        int year = 2023 + (i % 2);
        env_res_env(type, cat, rpri, rtax, etax, etax2, year);
    }

    print_str("\nSustainability...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int sthe = 35 + (i * 8);
        int gfn = 26 + (i * 6);
        int esg = 15 + (i * 4);
        int eres = 10 + (i * 2);
        int year = 2024;
        env_sus_dev(type, cat, sthe, gfn, esg, eres, year);
    }

    print_str("\nEconomics report...\n");
    env_econ_report();

    print_str("\nGreen report...\n");
    env_green_report();

    print_str("\nFull report...\n");
    env_full_report();

    print_str("\nFinal state...\n");
    env_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
