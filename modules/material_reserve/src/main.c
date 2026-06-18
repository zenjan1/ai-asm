/* material_reserve: Material reserve administration system (v1.0)
 * Strategic reserve, grain reserve, energy reserve, medical reserve, reserve management
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

#define MAX_STRATEGIC    16
#define MAX_GRAIN        14
#define MAX_ENERGY       12
#define MAX_MEDICAL      10
#define MAX_MGMT         10

typedef struct {
    int    strategic_id;
    int    strategic_type;
    int    reserve_level;
    int    national_reserves;
    int    local_reserves;
    int    enterprise_reserves;
    int    year;
    int    active;
} strategic_t;

typedef struct {
    int    grain_id;
    int    grain_type;
    int    grain_category;
    int    central_grain;
    int    local_grain;
    int    turnover_grain;
    int    year;
    int    active;
} grain_t;

typedef struct {
    int    energy_id;
    int    energy_type;
    int    energy_category;
    int    oil_reserve;
    int    gas_reserve;
    int    coal_reserve;
    int    year;
    int    active;
} energy_t;

typedef struct {
    int    medical_id;
    int    medical_type;
    int    medical_category;
    int    medicine_reserve;
    int    equipment_reserve;
    int    epidemic_reserve;
    int    year;
    int    active;
} medical_t;

typedef struct {
    int    mgmt_id;
    int    mgmt_type;
    int    management_area;
    int    reserve_plans;
    int    rotation_updates;
    int    quality_inspections;
    int    year;
    int    active;
} mgmt_t;

typedef struct {
    int    n_strategic;
    int    n_grain;
    int    n_energy;
    int    n_medical;
    int    n_mgmt;
    int    total_national;
    int    total_central;
    int    total_oil;
    int    total_medicine;
    int    total_plans;
} mr_state_t;

static strategic_t strategics[MAX_STRATEGIC];
static grain_t grains[MAX_GRAIN];
static energy_t energies[MAX_ENERGY];
static medical_t medicals[MAX_MEDICAL];
static mgmt_t mgmts[MAX_MGMT];
static mr_state_t mr;

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

int mr_init(void) {
    if (initialized) return -1;
    mr.n_strategic = 0; mr.n_grain = 0; mr.n_energy = 0;
    mr.n_medical = 0; mr.n_mgmt = 0;
    mr.total_national = 0; mr.total_central = 0;
    mr.total_oil = 0; mr.total_medicine = 0;
    mr.total_plans = 0;
    for (int i = 0; i < MAX_STRATEGIC; i++) strategics[i].active = 0;
    for (int i = 0; i < MAX_GRAIN; i++) grains[i].active = 0;
    for (int i = 0; i < MAX_ENERGY; i++) energies[i].active = 0;
    for (int i = 0; i < MAX_MEDICAL; i++) medicals[i].active = 0;
    for (int i = 0; i < MAX_MGMT; i++) mgmts[i].active = 0;
    initialized = 1;
    print_str("[MR] Material reserve initialized\n");
    return 0;
}

int mr_strategic(int st_type, int level, int national, int local, int enterprise, int year) {
    if (mr.n_strategic >= MAX_STRATEGIC) return -1;
    strategic_t* s = &strategics[mr.n_strategic];
    s->strategic_id = mr.n_strategic;
    s->strategic_type = st_type;
    s->reserve_level = level;
    s->national_reserves = national;
    s->local_reserves = local;
    s->enterprise_reserves = enterprise;
    s->year = year;
    s->active = 1;
    mr.total_national += national;
    mr.n_strategic++;
    print_str("[MR] Strategic "); print_int(mr.n_strategic - 1);
    print_str(" type="); print_int(st_type);
    print_str(" lvl="); print_int(level);
    print_str(" ntl="); print_int(national);
    print_str(" lcl="); print_int(local);
    print_str(" ent="); print_int(enterprise); print_str("\n");
    return mr.n_strategic - 1;
}

int mr_grain(int gr_type, int cat, int central, int local, int turnover, int year) {
    if (mr.n_grain >= MAX_GRAIN) return -1;
    grain_t* g = &grains[mr.n_grain];
    g->grain_id = mr.n_grain;
    g->grain_type = gr_type;
    g->grain_category = cat;
    g->central_grain = central;
    g->local_grain = local;
    g->turnover_grain = turnover;
    g->year = year;
    g->active = 1;
    mr.total_central += central;
    mr.n_grain++;
    print_str("[MR] Grain "); print_int(mr.n_grain - 1);
    print_str(" type="); print_int(gr_type);
    print_str(" cat="); print_int(cat);
    print_str(" cnl="); print_int(central);
    print_str(" lcl="); print_int(local);
    print_str(" tno="); print_int(turnover); print_str("\n");
    return mr.n_grain - 1;
}

int mr_energy(int en_type, int cat, int oil, int gas, int coal, int year) {
    if (mr.n_energy >= MAX_ENERGY) return -1;
    energy_t* e = &energies[mr.n_energy];
    e->energy_id = mr.n_energy;
    e->energy_type = en_type;
    e->energy_category = cat;
    e->oil_reserve = oil;
    e->gas_reserve = gas;
    e->coal_reserve = coal;
    e->year = year;
    e->active = 1;
    mr.total_oil += oil;
    mr.n_energy++;
    print_str("[MR] Energy "); print_int(mr.n_energy - 1);
    print_str(" type="); print_int(en_type);
    print_str(" cat="); print_int(cat);
    print_str(" oil="); print_int(oil);
    print_str(" gas="); print_int(gas);
    print_str(" col="); print_int(coal); print_str("\n");
    return mr.n_energy - 1;
}

int mr_medical(int md_type, int cat, int medicine, int equipment, int epidemic, int year) {
    if (mr.n_medical >= MAX_MEDICAL) return -1;
    medical_t* m = &medicals[mr.n_medical];
    m->medical_id = mr.n_medical;
    m->medical_type = md_type;
    m->medical_category = cat;
    m->medicine_reserve = medicine;
    m->equipment_reserve = equipment;
    m->epidemic_reserve = epidemic;
    m->year = year;
    m->active = 1;
    mr.total_medicine += medicine;
    mr.n_medical++;
    print_str("[MR] Medical "); print_int(mr.n_medical - 1);
    print_str(" type="); print_int(md_type);
    print_str(" cat="); print_int(cat);
    print_str(" mdc="); print_int(medicine);
    print_str(" eqp="); print_int(equipment);
    print_str(" epid="); print_int(epidemic); print_str("\n");
    return mr.n_medical - 1;
}

int mr_mgmt(int mg_type, int area, int plans, int rotations, int inspections, int year) {
    if (mr.n_mgmt >= MAX_MGMT) return -1;
    mgmt_t* mg = &mgmts[mr.n_mgmt];
    mg->mgmt_id = mr.n_mgmt;
    mg->mgmt_type = mg_type;
    mg->management_area = area;
    mg->reserve_plans = plans;
    mg->rotation_updates = rotations;
    mg->quality_inspections = inspections;
    mg->year = year;
    mg->active = 1;
    mr.total_plans += plans;
    mr.n_mgmt++;
    print_str("[MR] Mgmt "); print_int(mr.n_mgmt - 1);
    print_str(" type="); print_int(mg_type);
    print_str(" area="); print_int(area);
    print_str(" pln="); print_int(plans);
    print_str(" rtt="); print_int(rotations);
    print_str(" insp="); print_int(inspections); print_str("\n");
    return mr.n_mgmt - 1;
}

void mr_strategic_report(void) {
    print_str("[MR] Strategic report:\n");
    print_str("  Strategic reserve categories: "); print_int(mr.n_strategic); print_str("\n");
    print_str("  Total national reserves: "); print_int(mr.total_national); print_str("\n");
}

void mr_grain_report(void) {
    print_str("[MR] Grain report:\n");
    print_str("  Grain reserve categories: "); print_int(mr.n_grain); print_str("\n");
    print_str("  Total central grain: "); print_int(mr.total_central); print_str("\n");
}

void mr_mgmt_report(void) {
    print_str("[MR] Management report:\n");
    print_str("  Energy reserve categories: "); print_int(mr.n_energy); print_str("\n");
    print_str("  Total oil reserve: "); print_int(mr.total_oil); print_str("\n");
    print_str("  Medical reserve categories: "); print_int(mr.n_medical); print_str("\n");
    print_str("  Total medicine reserve: "); print_int(mr.total_medicine); print_str("\n");
    print_str("  Management categories: "); print_int(mr.n_mgmt); print_str("\n");
    print_str("  Total reserve plans: "); print_int(mr.total_plans); print_str("\n");
}

void mr_print_state(void) {
    print_str("[MR] St="); print_int(mr.n_strategic);
    print_str(" Gr="); print_int(mr.n_grain);
    print_str(" En="); print_int(mr.n_energy);
    print_str(" Md="); print_int(mr.n_medical);
    print_str(" Mg="); print_int(mr.n_mgmt);
    print_str("\n");
}

int main(void) {
    print_str("=== Material Reserve Demo ===\n\n");
    mr_init();

    print_str("Strategic reserves...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int lvl = (i % 4) + 1;
        int ntl = 100 + (i * 25);
        int lcl = 200 + (i * 50);
        int ent = 150 + (i * 40);
        int year = 2020 + (i % 5);
        mr_strategic(type, lvl, ntl, lcl, ent, year);
    }

    print_str("\nGrain reserves...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cnl = 500 + (i * 120);
        int lcl = 300 + (i * 75);
        int tno = 200 + (i * 50);
        int year = 2021 + (i % 4);
        mr_grain(type, cat, cnl, lcl, tno, year);
    }

    print_str("\nEnergy reserves...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 3) + 1;
        int oil = 50 + (i * 12);
        int gas = 80 + (i * 20);
        int col = 100 + (i * 25);
        int year = 2022 + (i % 3);
        mr_energy(type, cat, oil, gas, col, year);
    }

    print_str("\nMedical reserves...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int mdc = 200 + (i * 50);
        int eqp = 80 + (i * 20);
        int epid = 150 + (i * 40);
        int year = 2023 + (i % 2);
        mr_medical(type, cat, mdc, eqp, epid, year);
    }

    print_str("\nReserve management...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int area = (i % 5) + 1;
        int pln = 30 + (i * 8);
        int rtt = 20 + (i * 5);
        int insp = 15 + (i * 4);
        int year = 2024;
        mr_mgmt(type, area, pln, rtt, insp, year);
    }

    print_str("\nStrategic report...\n");
    mr_strategic_report();

    print_str("\nGrain report...\n");
    mr_grain_report();

    print_str("\nManagement report...\n");
    mr_mgmt_report();

    print_str("\nFinal state...\n");
    mr_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
