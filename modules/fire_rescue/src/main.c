/* fire_rescue: Fire rescue system (v1.0)
 * Fire supervision, firefighting rescue, investigation, education, equipment
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

#define MAX_SUPERVISION  16
#define MAX_RESCUE       14
#define MAX_INVEST       12
#define MAX_EDUCATION    10
#define MAX_EQUIP        10

typedef struct {
    int    supervision_id;
    int    building_id;
    int    type;
    int    inspector_id;
    int    violations;
    int    hazard_level;
    int    rectified;
    int    year;
    int    active;
} supervision_t;

typedef struct {
    int    rescue_id;
    int    incident_id;
    int    alarm_id;
    int    companies_dispatched;
    int    vehicles;
    int    personnel;
    int    rescued;
    int    duration_min;
    int    year;
    int    active;
} rescue_t;

typedef struct {
    int    invest_id;
    int    fire_id;
    int    cause;
    int    investigator_id;
    int    damage;
    int    injuries;
    int    deaths;
    int    year;
    int    status;
    int    active;
} investigation_t;

typedef struct {
    int    edu_id;
    int    type;
    int    audience;
    int    participants;
    int    duration_hours;
    int    drills;
    int    year;
    int    active;
} education_t;

typedef struct {
    int    equip_id;
    int    type;
    int    station_id;
    int    quantity;
    int    condition;
    int    last_maintenance;
    int    replacement_due;
    int    year;
    int    active;
} equipment_t;

typedef struct {
    int    n_supervision;
    int    n_rescue;
    int    n_invest;
    int    n_education;
    int    n_equip;
    int    total_violations;
    int    total_rescued;
    int    total_damage;
    int    total_participants;
    int    total_vehicles;
} fr_state_t;

static supervision_t supervisions[MAX_SUPERVISION];
static rescue_t rescues[MAX_RESCUE];
static investigation_t investigations[MAX_INVEST];
static education_t educations[MAX_EDUCATION];
static equipment_t equipment[MAX_EQUIP];
static fr_state_t fr;

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

int fr_init(void) {
    if (initialized) return -1;
    fr.n_supervision = 0; fr.n_rescue = 0; fr.n_invest = 0;
    fr.n_education = 0; fr.n_equip = 0;
    fr.total_violations = 0; fr.total_rescued = 0;
    fr.total_damage = 0; fr.total_participants = 0;
    fr.total_vehicles = 0;
    for (int i = 0; i < MAX_SUPERVISION; i++) supervisions[i].active = 0;
    for (int i = 0; i < MAX_RESCUE; i++) rescues[i].active = 0;
    for (int i = 0; i < MAX_INVEST; i++) investigations[i].active = 0;
    for (int i = 0; i < MAX_EDUCATION; i++) educations[i].active = 0;
    for (int i = 0; i < MAX_EQUIP; i++) equipment[i].active = 0;
    initialized = 1;
    print_str("[FR] Fire rescue initialized\n");
    return 0;
}

int fr_supervise(int building, int type, int inspector, int violations, int hazard, int rectified, int year) {
    if (fr.n_supervision >= MAX_SUPERVISION) return -1;
    supervision_t* s = &supervisions[fr.n_supervision];
    s->supervision_id = fr.n_supervision;
    s->building_id = building;
    s->type = type;
    s->inspector_id = inspector;
    s->violations = violations;
    s->hazard_level = hazard;
    s->rectified = rectified;
    s->year = year;
    s->active = 1;
    fr.total_violations += violations;
    fr.n_supervision++;
    print_str("[FR] Supervision "); print_int(fr.n_supervision - 1);
    print_str(" bld="); print_int(building);
    print_str(" type="); print_int(type);
    print_str(" insp="); print_int(inspector);
    print_str(" viol="); print_int(violations);
    print_str(" haz="); print_int(hazard); print_str("\n");
    return fr.n_supervision - 1;
}

int fr_dispatch(int incident, int alarm, int companies, int vehicles, int personnel, int rescued, int duration, int year) {
    if (fr.n_rescue >= MAX_RESCUE) return -1;
    rescue_t* r = &rescues[fr.n_rescue];
    r->rescue_id = fr.n_rescue;
    r->incident_id = incident;
    r->alarm_id = alarm;
    r->companies_dispatched = companies;
    r->vehicles = vehicles;
    r->personnel = personnel;
    r->rescued = rescued;
    r->duration_min = duration;
    r->year = year;
    r->active = 1;
    fr.total_rescued += rescued;
    fr.total_vehicles += vehicles;
    fr.n_rescue++;
    print_str("[FR] Rescue "); print_int(fr.n_rescue - 1);
    print_str(" inc="); print_int(incident);
    print_str(" co="); print_int(companies);
    print_str(" veh="); print_int(vehicles);
    print_str(" pers="); print_int(personnel);
    print_str(" resc="); print_int(rescued);
    print_str(" dur="); print_int(duration); print_str("min\n");
    return fr.n_rescue - 1;
}

int fr_investigate(int fire, int cause, int investigator, int damage, int injuries, int deaths, int year) {
    if (fr.n_invest >= MAX_INVEST) return -1;
    investigation_t* inv = &investigations[fr.n_invest];
    inv->invest_id = fr.n_invest;
    inv->fire_id = fire;
    inv->cause = cause;
    inv->investigator_id = investigator;
    inv->damage = damage;
    inv->injuries = injuries;
    inv->deaths = deaths;
    inv->year = year;
    inv->status = 1;
    inv->active = 1;
    fr.total_damage += damage;
    fr.n_invest++;
    print_str("[FR] Investigation "); print_int(fr.n_invest - 1);
    print_str(" fire="); print_int(fire);
    print_str(" cause="); print_int(cause);
    print_str(" inv="); print_int(investigator);
    print_str(" dmg=$"); print_int(damage);
    print_str(" inj="); print_int(injuries);
    print_str(" dth="); print_int(deaths); print_str("\n");
    return fr.n_invest - 1;
}

int fr_educate(int type, int audience, int participants, int duration, int drills, int year) {
    if (fr.n_education >= MAX_EDUCATION) return -1;
    education_t* e = &educations[fr.n_education];
    e->edu_id = fr.n_education;
    e->type = type;
    e->audience = audience;
    e->participants = participants;
    e->duration_hours = duration;
    e->drills = drills;
    e->year = year;
    e->active = 1;
    fr.total_participants += participants;
    fr.n_education++;
    print_str("[FR] Education "); print_int(fr.n_education - 1);
    print_str(" type="); print_int(type);
    print_str(" aud="); print_int(audience);
    print_str(" ptc="); print_int(participants);
    print_str(" dur="); print_int(duration); print_str("h");
    print_str(" drl="); print_int(drills); print_str("\n");
    return fr.n_education - 1;
}

int fr_add_equipment(int type, int station, int quantity, int condition, int maint, int replacement, int year) {
    if (fr.n_equip >= MAX_EQUIP) return -1;
    equipment_t* eq = &equipment[fr.n_equip];
    eq->equip_id = fr.n_equip;
    eq->type = type;
    eq->station_id = station;
    eq->quantity = quantity;
    eq->condition = condition;
    eq->last_maintenance = maint;
    eq->replacement_due = replacement;
    eq->year = year;
    eq->active = 1;
    fr.n_equip++;
    print_str("[FR] Equipment "); print_int(fr.n_equip - 1);
    print_str(" type="); print_int(type);
    print_str(" stn="); print_int(station);
    print_str(" qty="); print_int(quantity);
    print_str(" cond="); print_int(condition);
    print_str(" maint="); print_int(maint); print_str("\n");
    return fr.n_equip - 1;
}

void fr_supervision_report(void) {
    print_str("[FR] Supervision report:\n");
    print_str("  Inspections: "); print_int(fr.n_supervision); print_str("\n");
    print_str("  Total violations: "); print_int(fr.total_violations); print_str("\n");
}

void fr_rescue_report(void) {
    print_str("[FR] Rescue report:\n");
    print_str("  Rescues: "); print_int(fr.n_rescue); print_str("\n");
    print_str("  Total rescued: "); print_int(fr.total_rescued); print_str("\n");
    print_str("  Total vehicles: "); print_int(fr.total_vehicles); print_str("\n");
}

void fr_equipment_report(void) {
    print_str("[FR] Equipment report:\n");
    print_str("  Equipment types: "); print_int(fr.n_equip); print_str("\n");
    print_str("  Investigations: "); print_int(fr.n_invest); print_str("\n");
    print_str("  Total damage: $"); print_int(fr.total_damage); print_str("\n");
    print_str("  Total education participants: "); print_int(fr.total_participants); print_str("\n");
}

void fr_print_state(void) {
    print_str("[FR] Sp="); print_int(fr.n_supervision);
    print_str(" Rs="); print_int(fr.n_rescue);
    print_str(" Iv="); print_int(fr.n_invest);
    print_str(" Ed="); print_int(fr.n_education);
    print_str(" Eq="); print_int(fr.n_equip);
    print_str("\n");
}

int main(void) {
    print_str("=== Fire Rescue Demo ===\n\n");
    fr_init();

    print_str("Fire supervision...\n");
    for (int i = 0; i < 16; i++) {
        int bld = 1000 + (i * 50);
        int type = (i % 4) + 1;
        int insp = 100 + (i % 10);
        int viol = (i % 6);
        int haz = (i % 4) + 1;
        int rect = (i % 3 == 0) ? 0 : 1;
        int year = 2022 + (i % 3);
        fr_supervise(bld, type, insp, viol, haz, rect, year);
    }

    print_str("\nFire rescue dispatches...\n");
    for (int i = 0; i < 14; i++) {
        int inc = 5000 + (i * 11);
        int alm = 9000 + (i * 3);
        int co = 1 + (i % 4);
        int veh = 2 + (i % 5);
        int pers = 10 + (i * 5);
        int resc = (i % 3 == 0) ? 1 + (i % 4) : 0;
        int dur = 30 + (i * 15);
        int year = 2023 + (i % 2);
        fr_dispatch(inc, alm, co, veh, pers, resc, dur, year);
    }

    print_str("\nFire investigations...\n");
    for (int i = 0; i < 12; i++) {
        int fire = 6000 + (i * 7);
        int cause = (i % 5) + 1;
        int inv = 200 + (i % 8);
        int dmg = 10000 + (i * 5000);
        int inj = i % 4;
        int dth = (i % 6 == 0) ? 1 : 0;
        int year = 2023 + (i % 2);
        fr_investigate(fire, cause, inv, dmg, inj, dth, year);
    }

    print_str("\nFire education...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 3) + 1;
        int aud = (i % 4) + 1;
        int ptc = 50 + (i * 30);
        int dur = 2 + (i * 2);
        int drl = 1 + (i % 3);
        int year = 2024;
        fr_educate(type, aud, ptc, dur, drl, year);
    }

    print_str("\nEquipment management...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 5) + 1;
        int stn = 300 + (i * 10);
        int qty = 5 + (i * 3);
        int cond = 70 + (i * 3);
        int maint = 20240101 + (i * 30);
        int repl = 20250101 + (i * 60);
        int year = 2024;
        fr_add_equipment(type, stn, qty, cond, maint, repl, year);
    }

    print_str("\nSupervision report...\n");
    fr_supervision_report();

    print_str("\nRescue report...\n");
    fr_rescue_report();

    print_str("\nEquipment report...\n");
    fr_equipment_report();

    print_str("\nFinal state...\n");
    fr_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
