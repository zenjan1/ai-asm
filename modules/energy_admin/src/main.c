/* energy_admin: Energy administration system (v1.0)
 * Energy planning, electricity, oil and gas, new energy, energy security
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

#define MAX_PLAN         16
#define MAX_ELECTRIC     14
#define MAX_OILGAS       12
#define MAX_NEWENERGY    10
#define MAX_SECURITY     10

typedef struct {
    int    plan_id;
    int    plan_type;
    int    region_id;
    int    target_output;
    int    investment;
    int    completion_pct;
    int    year;
    int    active;
} plan_t;

typedef struct {
    int    electric_id;
    int    power_plant_id;
    int    type;
    int    capacity_mw;
    int    generation_mwh;
    int    grid_dispatch;
    int    efficiency;
    int    year;
    int    active;
} electric_t;

typedef struct {
    int    oilgas_id;
    int    field_id;
    int    type;
    int    production;
    int    reserves;
    int    pipeline_km;
    int    year;
    int    active;
} oilgas_t;

typedef struct {
    int    newenergy_id;
    int    source_type;
    int    location_id;
    int    capacity_mw;
    int    generation_mwh;
    int    subsidy;
    int    co2_reduced;
    int    year;
    int    active;
} newenergy_t;

typedef struct {
    int    security_id;
    int    reserve_type;
    int    facility_id;
    int    capacity;
    int    current_stock;
    int    days_of_supply;
    int    year;
    int    active;
} security_t;

typedef struct {
    int    n_plan;
    int    n_electric;
    int    n_oilgas;
    int    n_newenergy;
    int    n_security;
    int    total_capacity;
    int    total_generation;
    int    total_production;
    int    total_reserves;
    int    total_co2;
} ea_state_t;

static plan_t plans[MAX_PLAN];
static electric_t electrics[MAX_ELECTRIC];
static oilgas_t oilgases[MAX_OILGAS];
static newenergy_t newenergies[MAX_NEWENERGY];
static security_t securities[MAX_SECURITY];
static ea_state_t ea;

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

int ea_init(void) {
    if (initialized) return -1;
    ea.n_plan = 0; ea.n_electric = 0; ea.n_oilgas = 0;
    ea.n_newenergy = 0; ea.n_security = 0;
    ea.total_capacity = 0; ea.total_generation = 0;
    ea.total_production = 0; ea.total_reserves = 0;
    ea.total_co2 = 0;
    for (int i = 0; i < MAX_PLAN; i++) plans[i].active = 0;
    for (int i = 0; i < MAX_ELECTRIC; i++) electrics[i].active = 0;
    for (int i = 0; i < MAX_OILGAS; i++) oilgases[i].active = 0;
    for (int i = 0; i < MAX_NEWENERGY; i++) newenergies[i].active = 0;
    for (int i = 0; i < MAX_SECURITY; i++) securities[i].active = 0;
    initialized = 1;
    print_str("[EA] Energy admin initialized\n");
    return 0;
}

int ea_plan(int plan_type, int region, int target, int investment, int completion, int year) {
    if (ea.n_plan >= MAX_PLAN) return -1;
    plan_t* p = &plans[ea.n_plan];
    p->plan_id = ea.n_plan;
    p->plan_type = plan_type;
    p->region_id = region;
    p->target_output = target;
    p->investment = investment;
    p->completion_pct = completion;
    p->year = year;
    p->active = 1;
    ea.n_plan++;
    print_str("[EA] Plan "); print_int(ea.n_plan - 1);
    print_str(" type="); print_int(plan_type);
    print_str(" reg="); print_int(region);
    print_str(" tgt="); print_int(target);
    print_str(" inv=$"); print_int(investment);
    print_str(" cmp="); print_int(completion); print_str("%\n");
    return ea.n_plan - 1;
}

int ea_electric(int plant, int type, int capacity, int generation, int dispatch, int efficiency, int year) {
    if (ea.n_electric >= MAX_ELECTRIC) return -1;
    electric_t* e = &electrics[ea.n_electric];
    e->electric_id = ea.n_electric;
    e->power_plant_id = plant;
    e->type = type;
    e->capacity_mw = capacity;
    e->generation_mwh = generation;
    e->grid_dispatch = dispatch;
    e->efficiency = efficiency;
    e->year = year;
    e->active = 1;
    ea.total_capacity += capacity;
    ea.total_generation += generation;
    ea.n_electric++;
    print_str("[EA] Electric "); print_int(ea.n_electric - 1);
    print_str(" plt="); print_int(plant);
    print_str(" type="); print_int(type);
    print_str(" cap="); print_int(capacity); print_str("MW");
    print_str(" gen="); print_int(generation); print_str("MWh");
    print_str(" eff="); print_int(efficiency); print_str("%\n");
    return ea.n_electric - 1;
}

int ea_oilgas(int field, int type, int production, int reserves, int pipeline, int year) {
    if (ea.n_oilgas >= MAX_OILGAS) return -1;
    oilgas_t* o = &oilgases[ea.n_oilgas];
    o->oilgas_id = ea.n_oilgas;
    o->field_id = field;
    o->type = type;
    o->production = production;
    o->reserves = reserves;
    o->pipeline_km = pipeline;
    o->year = year;
    o->active = 1;
    ea.total_production += production;
    ea.total_reserves += reserves;
    ea.n_oilgas++;
    print_str("[EA] OilGas "); print_int(ea.n_oilgas - 1);
    print_str(" fld="); print_int(field);
    print_str(" type="); print_int(type);
    print_str(" prd="); print_int(production); print_str("t");
    print_str(" rsv="); print_int(reserves); print_str("t");
    print_str(" pip="); print_int(pipeline); print_str("km\n");
    return ea.n_oilgas - 1;
}

int ea_newenergy(int source, int location, int capacity, int generation, int subsidy, int co2, int year) {
    if (ea.n_newenergy >= MAX_NEWENERGY) return -1;
    newenergy_t* n = &newenergies[ea.n_newenergy];
    n->newenergy_id = ea.n_newenergy;
    n->source_type = source;
    n->location_id = location;
    n->capacity_mw = capacity;
    n->generation_mwh = generation;
    n->subsidy = subsidy;
    n->co2_reduced = co2;
    n->year = year;
    n->active = 1;
    ea.total_co2 += co2;
    ea.n_newenergy++;
    print_str("[EA] NewEnergy "); print_int(ea.n_newenergy - 1);
    print_str(" src="); print_int(source);
    print_str(" loc="); print_int(location);
    print_str(" cap="); print_int(capacity); print_str("MW");
    print_str(" gen="); print_int(generation); print_str("MWh");
    print_str(" co2="); print_int(co2); print_str("t\n");
    return ea.n_newenergy - 1;
}

int ea_security(int reserve_type, int facility, int capacity, int stock, int days_supply, int year) {
    if (ea.n_security >= MAX_SECURITY) return -1;
    security_t* s = &securities[ea.n_security];
    s->security_id = ea.n_security;
    s->reserve_type = reserve_type;
    s->facility_id = facility;
    s->capacity = capacity;
    s->current_stock = stock;
    s->days_of_supply = days_supply;
    s->year = year;
    s->active = 1;
    ea.n_security++;
    print_str("[EA] Security "); print_int(ea.n_security - 1);
    print_str(" type="); print_int(reserve_type);
    print_str(" fac="); print_int(facility);
    print_str(" cap="); print_int(capacity);
    print_str(" stk="); print_int(stock);
    print_str(" day="); print_int(days_supply); print_str("d\n");
    return ea.n_security - 1;
}

void ea_plan_report(void) {
    print_str("[EA] Plan report:\n");
    print_str("  Plans: "); print_int(ea.n_plan); print_str("\n");
}

void ea_electric_report(void) {
    print_str("[EA] Electricity report:\n");
    print_str("  Power plants: "); print_int(ea.n_electric); print_str("\n");
    print_str("  Total capacity: "); print_int(ea.total_capacity); print_str(" MW\n");
    print_str("  Total generation: "); print_int(ea.total_generation); print_str(" MWh\n");
}

void ea_energy_report(void) {
    print_str("[EA] Energy report:\n");
    print_str("  Oil and gas fields: "); print_int(ea.n_oilgas); print_str("\n");
    print_str("  Total production: "); print_int(ea.total_production); print_str(" tons\n");
    print_str("  New energy: "); print_int(ea.n_newenergy); print_str("\n");
    print_str("  CO2 reduced: "); print_int(ea.total_co2); print_str(" tons\n");
    print_str("  Energy security: "); print_int(ea.n_security); print_str("\n");
}

void ea_print_state(void) {
    print_str("[EA] Pl="); print_int(ea.n_plan);
    print_str(" El="); print_int(ea.n_electric);
    print_str(" Og="); print_int(ea.n_oilgas);
    print_str(" Ne="); print_int(ea.n_newenergy);
    print_str(" Sc="); print_int(ea.n_security);
    print_str("\n");
}

int main(void) {
    print_str("=== Energy Admin Demo ===\n\n");
    ea_init();

    print_str("Energy planning...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 4) + 1;
        int reg = (i % 6) + 1;
        int tgt = 1000 + (i * 500);
        int inv = 1000000 + (i * 500000);
        int cmp = 50 + (i * 3);
        int year = 2020 + (i % 5);
        ea_plan(type, reg, tgt, inv, cmp, year);
    }

    print_str("\nElectricity management...\n");
    for (int i = 0; i < 14; i++) {
        int plt = 100 + (i * 10);
        int type = (i % 4) + 1;
        int cap = 100 + (i * 50);
        int gen = cap * 24 * 30;
        int dsp = gen - (i * 1000);
        int eff = 35 + (i * 2);
        int year = 2021 + (i % 4);
        ea_electric(plt, type, cap, gen, dsp, eff, year);
    }

    print_str("\nOil and gas...\n");
    for (int i = 0; i < 12; i++) {
        int fld = 200 + (i * 15);
        int type = (i % 3) + 1;
        int prd = 50000 + (i * 20000);
        int rsv = 500000 + (i * 200000);
        int pip = 100 + (i * 50);
        int year = 2022 + (i % 3);
        ea_oilgas(fld, type, prd, rsv, pip, year);
    }

    print_str("\nNew energy...\n");
    for (int i = 0; i < 10; i++) {
        int src = (i % 4) + 1;
        int loc = (i % 6) + 1;
        int cap = 50 + (i * 20);
        int gen = cap * 20 * 30;
        int sub = 10000 + (i * 5000);
        int co2 = 1000 + (i * 500);
        int year = 2023 + (i % 2);
        ea_newenergy(src, loc, cap, gen, sub, co2, year);
    }

    print_str("\nEnergy security...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 3) + 1;
        int fac = 300 + (i * 20);
        int cap = 100000 + (i * 50000);
        int stk = 70000 + (i * 30000);
        int day = 30 + (i * 5);
        int year = 2024;
        ea_security(type, fac, cap, stk, day, year);
    }

    print_str("\nPlan report...\n");
    ea_plan_report();

    print_str("\nElectricity report...\n");
    ea_electric_report();

    print_str("\nEnergy report...\n");
    ea_energy_report();

    print_str("\nFinal state...\n");
    ea_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
