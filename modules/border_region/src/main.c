/* border_region: Border region development administration system (v1.0)
 * Border construction, border defense, opening up, ethnic unity, prosperity program
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

#define MAX_BORDER_CON   16
#define MAX_DEFENSE      14
#define MAX_OPENING      12
#define MAX_UNITY        10
#define MAX_PROSPERITY   10

typedef struct {
    int    border_con_id;
    int    construction_type;
    int    border_area;
    int    infrastructure;
    int    urban_construction;
    int    livelihood_improvement;
    int    year;
    int    active;
} border_con_t;

typedef struct {
    int    defense_id;
    int    defense_type;
    int    border_section;
    int    border_management;
    int    defense_facilities;
    int    border_patrols;
    int    year;
    int    active;
} defense_t;

typedef struct {
    int    opening_id;
    int    opening_type;
    int    opening_area;
    int    port_construction;
    int    economic_zones;
    int    cross_border_cooperation;
    int    year;
    int    active;
} opening_t;

typedef struct {
    int    unity_id;
    int    unity_type;
    int    ethnic_area;
    int    unity_progress;
    int    religious_harmony;
    int    border_stability;
    int    year;
    int    active;
} unity_t;

typedef struct {
    int    prosperity_id;
    int    prosperity_type;
    int    border_region;
    int    prosperity_action;
    int    border_subsidies;
    int    border_support;
    int    year;
    int    active;
} prosperity_t;

typedef struct {
    int    n_border_con;
    int    n_defense;
    int    n_opening;
    int    n_unity;
    int    n_prosperity;
    int    total_infrastructure;
    int    total_management;
    int    total_ports;
    int    total_unity;
    int    total_prosperity;
} br_state_t;

static border_con_t border_cons[MAX_BORDER_CON];
static defense_t defenses[MAX_DEFENSE];
static opening_t openings[MAX_OPENING];
static unity_t unities[MAX_UNITY];
static prosperity_t prosperities[MAX_PROSPERITY];
static br_state_t br;

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

int br_init(void) {
    if (initialized) return -1;
    br.n_border_con = 0; br.n_defense = 0; br.n_opening = 0;
    br.n_unity = 0; br.n_prosperity = 0;
    br.total_infrastructure = 0; br.total_management = 0;
    br.total_ports = 0; br.total_unity = 0;
    br.total_prosperity = 0;
    for (int i = 0; i < MAX_BORDER_CON; i++) border_cons[i].active = 0;
    for (int i = 0; i < MAX_DEFENSE; i++) defenses[i].active = 0;
    for (int i = 0; i < MAX_OPENING; i++) openings[i].active = 0;
    for (int i = 0; i < MAX_UNITY; i++) unities[i].active = 0;
    for (int i = 0; i < MAX_PROSPERITY; i++) prosperities[i].active = 0;
    initialized = 1;
    print_str("[BR] Border region initialized\n");
    return 0;
}

int br_construction(int con_type, int area, int infra, int urban, int livelihood, int year) {
    if (br.n_border_con >= MAX_BORDER_CON) return -1;
    border_con_t* b = &border_cons[br.n_border_con];
    b->border_con_id = br.n_border_con;
    b->construction_type = con_type;
    b->border_area = area;
    b->infrastructure = infra;
    b->urban_construction = urban;
    b->livelihood_improvement = livelihood;
    b->year = year;
    b->active = 1;
    br.total_infrastructure += infra;
    br.n_border_con++;
    print_str("[BR] Construction "); print_int(br.n_border_con - 1);
    print_str(" type="); print_int(con_type);
    print_str(" area="); print_int(area);
    print_str(" inf="); print_int(infra);
    print_str(" urb="); print_int(urban);
    print_str(" lvd="); print_int(livelihood); print_str("\n");
    return br.n_border_con - 1;
}

int br_defense(int def_type, int section, int management, int facilities, int patrols, int year) {
    if (br.n_defense >= MAX_DEFENSE) return -1;
    defense_t* d = &defenses[br.n_defense];
    d->defense_id = br.n_defense;
    d->defense_type = def_type;
    d->border_section = section;
    d->border_management = management;
    d->defense_facilities = facilities;
    d->border_patrols = patrols;
    d->year = year;
    d->active = 1;
    br.total_management += management;
    br.n_defense++;
    print_str("[BR] Defense "); print_int(br.n_defense - 1);
    print_str(" type="); print_int(def_type);
    print_str(" sec="); print_int(section);
    print_str(" mgt="); print_int(management);
    print_str(" fcl="); print_int(facilities);
    print_str(" ptr="); print_int(patrols); print_str("\n");
    return br.n_defense - 1;
}

int br_opening(int opn_type, int area, int ports, int zones, int cross_border, int year) {
    if (br.n_opening >= MAX_OPENING) return -1;
    opening_t* o = &openings[br.n_opening];
    o->opening_id = br.n_opening;
    o->opening_type = opn_type;
    o->opening_area = area;
    o->port_construction = ports;
    o->economic_zones = zones;
    o->cross_border_cooperation = cross_border;
    o->year = year;
    o->active = 1;
    br.total_ports += ports;
    br.n_opening++;
    print_str("[BR] Opening "); print_int(br.n_opening - 1);
    print_str(" type="); print_int(opn_type);
    print_str(" area="); print_int(area);
    print_str(" prt="); print_int(ports);
    print_str(" zn="); print_int(zones);
    print_str(" crs="); print_int(cross_border); print_str("\n");
    return br.n_opening - 1;
}

int br_unity(int unt_type, int area, int progress, int harmony, int stability, int year) {
    if (br.n_unity >= MAX_UNITY) return -1;
    unity_t* u = &unities[br.n_unity];
    u->unity_id = br.n_unity;
    u->unity_type = unt_type;
    u->ethnic_area = area;
    u->unity_progress = progress;
    u->religious_harmony = harmony;
    u->border_stability = stability;
    u->year = year;
    u->active = 1;
    br.total_unity += progress;
    br.n_unity++;
    print_str("[BR] Unity "); print_int(br.n_unity - 1);
    print_str(" type="); print_int(unt_type);
    print_str(" area="); print_int(area);
    print_str(" prg="); print_int(progress);
    print_str(" hrm="); print_int(harmony);
    print_str(" stb="); print_int(stability); print_str("\n");
    return br.n_unity - 1;
}

int br_prosperity(int pro_type, int region, int action, int subsidies, int support, int year) {
    if (br.n_prosperity >= MAX_PROSPERITY) return -1;
    prosperity_t* p = &prosperities[br.n_prosperity];
    p->prosperity_id = br.n_prosperity;
    p->prosperity_type = pro_type;
    p->border_region = region;
    p->prosperity_action = action;
    p->border_subsidies = subsidies;
    p->border_support = support;
    p->year = year;
    p->active = 1;
    br.total_prosperity += action;
    br.n_prosperity++;
    print_str("[BR] Prosperity "); print_int(br.n_prosperity - 1);
    print_str(" type="); print_int(pro_type);
    print_str(" rgn="); print_int(region);
    print_str(" act="); print_int(action);
    print_str(" sbs="); print_int(subsidies);
    print_str(" sup="); print_int(support); print_str("\n");
    return br.n_prosperity - 1;
}

void br_construction_report(void) {
    print_str("[BR] Construction report:\n");
    print_str("  Border construction categories: "); print_int(br.n_border_con); print_str("\n");
    print_str("  Total infrastructure projects: "); print_int(br.total_infrastructure); print_str("\n");
}

void br_defense_report(void) {
    print_str("[BR] Defense report:\n");
    print_str("  Border defense categories: "); print_int(br.n_defense); print_str("\n");
    print_str("  Total border management ops: "); print_int(br.total_management); print_str("\n");
}

void br_prosperity_report(void) {
    print_str("[BR] Prosperity report:\n");
    print_str("  Opening up categories: "); print_int(br.n_opening); print_str("\n");
    print_str("  Total port constructions: "); print_int(br.total_ports); print_str("\n");
    print_str("  Ethnic unity categories: "); print_int(br.n_unity); print_str("\n");
    print_str("  Total unity progress: "); print_int(br.total_unity); print_str("\n");
    print_str("  Prosperity categories: "); print_int(br.n_prosperity); print_str("\n");
    print_str("  Total prosperity actions: "); print_int(br.total_prosperity); print_str("\n");
}

void br_print_state(void) {
    print_str("[BR] Cn="); print_int(br.n_border_con);
    print_str(" Df="); print_int(br.n_defense);
    print_str(" Op="); print_int(br.n_opening);
    print_str(" Un="); print_int(br.n_unity);
    print_str(" Pr="); print_int(br.n_prosperity);
    print_str("\n");
}

int main(void) {
    print_str("=== Border Region Demo ===\n\n");
    br_init();

    print_str("Border construction...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int area = (i % 6) + 1;
        int inf = 25 + (i * 6);
        int urb = 15 + (i * 4);
        int lvd = 20 + (i * 5);
        int year = 2020 + (i % 5);
        br_construction(type, area, inf, urb, lvd, year);
    }

    print_str("\nBorder defense...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int sec = (i % 5) + 1;
        int mgt = 40 + (i * 10);
        int fcl = 20 + (i * 5);
        int ptr = 30 + (i * 8);
        int year = 2021 + (i % 4);
        br_defense(type, sec, mgt, fcl, ptr, year);
    }

    print_str("\nOpening up...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int area = (i % 5) + 1;
        int prt = 10 + (i * 3);
        int zn = 5 + (i * 2);
        int crs = 8 + (i * 2);
        int year = 2022 + (i % 3);
        br_opening(type, area, prt, zn, crs, year);
    }

    print_str("\nEthnic unity...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int area = (i % 5) + 1;
        int prg = 20 + (i * 5);
        int hrm = 15 + (i * 4);
        int stb = 10 + (i * 3);
        int year = 2023 + (i % 2);
        br_unity(type, area, prg, hrm, stb, year);
    }

    print_str("\nProsperity program...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int rgn = (i % 5) + 1;
        int act = 15 + (i * 4);
        int sbs = 50 + (i * 12);
        int sup = 30 + (i * 8);
        int year = 2024;
        br_prosperity(type, rgn, act, sbs, sup, year);
    }

    print_str("\nConstruction report...\n");
    br_construction_report();

    print_str("\nDefense report...\n");
    br_defense_report();

    print_str("\nProsperity report...\n");
    br_prosperity_report();

    print_str("\nFinal state...\n");
    br_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
