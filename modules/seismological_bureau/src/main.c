/* seismological_bureau: Seismological administration system (v1.0)
 * Seismic monitoring, disaster prevention, emergency rescue, research, public service
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

#define MAX_MONITORING   16
#define MAX_PREVENTION   14
#define MAX_EMERGENCY    12
#define MAX_RESEARCH     10
#define MAX_SERVICE      10

typedef struct {
    int    monitoring_id;
    int    station_type;
    int    region_id;
    int    stations_online;
    int    events_recorded;
    int    data_quality;
    int    year;
    int    active;
} monitoring_t;

typedef struct {
    int    prevention_id;
    int    zone_type;
    int    risk_level;
    int    seismic_resistance;
    int    risk_assessments;
    int    emergency_drills;
    int    year;
    int    active;
} prevention_t;

typedef struct {
    int    emergency_id;
    int    event_type;
    int    magnitude;
    int    early_warnings;
    int    damage_assessments;
    int    rescue_operations;
    int    year;
    int    active;
} emergency_t;

typedef struct {
    int    research_id;
    int    research_area;
    int    project_type;
    int    projects_funded;
    int    papers_published;
    int    patents_granted;
    int    year;
    int    active;
} research_t;

typedef struct {
    int    service_id;
    int    service_type;
    int    audience_type;
    int    events_held;
    int    participants;
    int    consultations;
    int    year;
    int    active;
} service_t;

typedef struct {
    int    n_monitoring;
    int    n_prevention;
    int    n_emergency;
    int    n_research;
    int    n_service;
    int    total_stations;
    int    total_events;
    int    total_assessments;
    int    total_projects;
    int    total_participants;
} sb_state_t;

static monitoring_t monitorings[MAX_MONITORING];
static prevention_t preventions[MAX_PREVENTION];
static emergency_t emergencies[MAX_EMERGENCY];
static research_t researches[MAX_RESEARCH];
static service_t services[MAX_SERVICE];
static sb_state_t sb;

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

int sb_init(void) {
    if (initialized) return -1;
    sb.n_monitoring = 0; sb.n_prevention = 0; sb.n_emergency = 0;
    sb.n_research = 0; sb.n_service = 0;
    sb.total_stations = 0; sb.total_events = 0;
    sb.total_assessments = 0; sb.total_projects = 0;
    sb.total_participants = 0;
    for (int i = 0; i < MAX_MONITORING; i++) monitorings[i].active = 0;
    for (int i = 0; i < MAX_PREVENTION; i++) preventions[i].active = 0;
    for (int i = 0; i < MAX_EMERGENCY; i++) emergencies[i].active = 0;
    for (int i = 0; i < MAX_RESEARCH; i++) researches[i].active = 0;
    for (int i = 0; i < MAX_SERVICE; i++) services[i].active = 0;
    initialized = 1;
    print_str("[SB] Seismological bureau initialized\n");
    return 0;
}

int sb_monitoring(int stn_type, int region, int stations, int events, int quality, int year) {
    if (sb.n_monitoring >= MAX_MONITORING) return -1;
    monitoring_t* m = &monitorings[sb.n_monitoring];
    m->monitoring_id = sb.n_monitoring;
    m->station_type = stn_type;
    m->region_id = region;
    m->stations_online = stations;
    m->events_recorded = events;
    m->data_quality = quality;
    m->year = year;
    m->active = 1;
    sb.total_stations += stations;
    sb.total_events += events;
    sb.n_monitoring++;
    print_str("[SB] Monitoring "); print_int(sb.n_monitoring - 1);
    print_str(" type="); print_int(stn_type);
    print_str(" rgn="); print_int(region);
    print_str(" stn="); print_int(stations);
    print_str(" evt="); print_int(events);
    print_str(" qual="); print_int(quality); print_str("\n");
    return sb.n_monitoring - 1;
}

int sb_prevention(int zone, int risk, int resistance, int assessments, int drills, int year) {
    if (sb.n_prevention >= MAX_PREVENTION) return -1;
    prevention_t* p = &preventions[sb.n_prevention];
    p->prevention_id = sb.n_prevention;
    p->zone_type = zone;
    p->risk_level = risk;
    p->seismic_resistance = resistance;
    p->risk_assessments = assessments;
    p->emergency_drills = drills;
    p->year = year;
    p->active = 1;
    sb.total_assessments += assessments;
    sb.n_prevention++;
    print_str("[SB] Prevention "); print_int(sb.n_prevention - 1);
    print_str(" zone="); print_int(zone);
    print_str(" risk="); print_int(risk);
    print_str(" rst="); print_int(resistance);
    print_str(" ass="); print_int(assessments);
    print_str(" drl="); print_int(drills); print_str("\n");
    return sb.n_prevention - 1;
}

int sb_emergency(int evt_type, int magnitude, int warnings, int damage, int rescue, int year) {
    if (sb.n_emergency >= MAX_EMERGENCY) return -1;
    emergency_t* e = &emergencies[sb.n_emergency];
    e->emergency_id = sb.n_emergency;
    e->event_type = evt_type;
    e->magnitude = magnitude;
    e->early_warnings = warnings;
    e->damage_assessments = damage;
    e->rescue_operations = rescue;
    e->year = year;
    e->active = 1;
    sb.n_emergency++;
    print_str("[SB] Emergency "); print_int(sb.n_emergency - 1);
    print_str(" type="); print_int(evt_type);
    print_str(" M"); print_int(magnitude / 10); print_str("."); print_int(magnitude % 10);
    print_str(" wrn="); print_int(warnings);
    print_str(" dmg="); print_int(damage);
    print_str(" rsc="); print_int(rescue); print_str("\n");
    return sb.n_emergency - 1;
}

int sb_research(int area, int proj_type, int projects, int papers, int patents, int year) {
    if (sb.n_research >= MAX_RESEARCH) return -1;
    research_t* r = &researches[sb.n_research];
    r->research_id = sb.n_research;
    r->research_area = area;
    r->project_type = proj_type;
    r->projects_funded = projects;
    r->papers_published = papers;
    r->patents_granted = patents;
    r->year = year;
    r->active = 1;
    sb.total_projects += projects;
    sb.n_research++;
    print_str("[SB] Research "); print_int(sb.n_research - 1);
    print_str(" area="); print_int(area);
    print_str(" type="); print_int(proj_type);
    print_str(" prj="); print_int(projects);
    print_str(" pap="); print_int(papers);
    print_str(" pat="); print_int(patents); print_str("\n");
    return sb.n_research - 1;
}

int sb_service(int svc_type, int audience, int events, int participants, int consultations, int year) {
    if (sb.n_service >= MAX_SERVICE) return -1;
    service_t* s = &services[sb.n_service];
    s->service_id = sb.n_service;
    s->service_type = svc_type;
    s->audience_type = audience;
    s->events_held = events;
    s->participants = participants;
    s->consultations = consultations;
    s->year = year;
    s->active = 1;
    sb.total_participants += participants;
    sb.n_service++;
    print_str("[SB] Service "); print_int(sb.n_service - 1);
    print_str(" svc="); print_int(svc_type);
    print_str(" aud="); print_int(audience);
    print_str(" evt="); print_int(events);
    print_str(" par="); print_int(participants);
    print_str(" csl="); print_int(consultations); print_str("\n");
    return sb.n_service - 1;
}

void sb_monitoring_report(void) {
    print_str("[SB] Monitoring report:\n");
    print_str("  Station networks: "); print_int(sb.n_monitoring); print_str("\n");
    print_str("  Total stations: "); print_int(sb.total_stations); print_str("\n");
    print_str("  Total events recorded: "); print_int(sb.total_events); print_str("\n");
}

void sb_prevention_report(void) {
    print_str("[SB] Prevention report:\n");
    print_str("  Prevention zones: "); print_int(sb.n_prevention); print_str("\n");
    print_str("  Total risk assessments: "); print_int(sb.total_assessments); print_str("\n");
}

void sb_emergency_report(void) {
    print_str("[SB] Emergency report:\n");
    print_str("  Emergency responses: "); print_int(sb.n_emergency); print_str("\n");
    print_str("  Research projects: "); print_int(sb.n_research); print_str("\n");
    print_str("  Total projects funded: "); print_int(sb.total_projects); print_str("\n");
    print_str("  Public services: "); print_int(sb.n_service); print_str("\n");
    print_str("  Total participants: "); print_int(sb.total_participants); print_str("\n");
}

void sb_print_state(void) {
    print_str("[SB] Mn="); print_int(sb.n_monitoring);
    print_str(" Pv="); print_int(sb.n_prevention);
    print_str(" Em="); print_int(sb.n_emergency);
    print_str(" Rs="); print_int(sb.n_research);
    print_str(" Sv="); print_int(sb.n_service);
    print_str("\n");
}

int main(void) {
    print_str("=== Seismological Bureau Demo ===\n\n");
    sb_init();

    print_str("Seismic monitoring...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int rgn = (i % 8) + 1;
        int stn = 30 + (i * 10);
        int evt = 100 + (i * 50);
        int qual = 85 + (i % 15);
        int year = 2020 + (i % 5);
        sb_monitoring(type, rgn, stn, evt, qual, year);
    }

    print_str("\nDisaster prevention...\n");
    for (int i = 0; i < 14; i++) {
        int zone = (i % 6) + 1;
        int risk = (i % 5) + 1;
        int rst = 70 + (i * 2);
        int ass = 15 + (i * 5);
        int drl = 5 + (i * 2);
        int year = 2021 + (i % 4);
        sb_prevention(zone, risk, rst, ass, drl, year);
    }

    print_str("\nEmergency rescue...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int mag = 30 + (i * 8);
        int wrn = 5 + (i * 2);
        int dmg = 3 + (i % 4);
        int rsc = 2 + (i % 3);
        int year = 2022 + (i % 3);
        sb_emergency(type, mag, wrn, dmg, rsc, year);
    }

    print_str("\nResearch development...\n");
    for (int i = 0; i < 10; i++) {
        int area = (i % 6) + 1;
        int type = (i % 4) + 1;
        int prj = 10 + (i * 3);
        int pap = 30 + (i * 10);
        int pat = 3 + (i % 5);
        int year = 2023 + (i % 2);
        sb_research(area, type, prj, pap, pat, year);
    }

    print_str("\nPublic services...\n");
    for (int i = 0; i < 10; i++) {
        int svc = (i % 5) + 1;
        int aud = (i % 4) + 1;
        int evt = 20 + (i * 5);
        int par = 500 + (i * 100);
        int csl = 100 + (i * 30);
        int year = 2024;
        sb_service(svc, aud, evt, par, csl, year);
    }

    print_str("\nMonitoring report...\n");
    sb_monitoring_report();

    print_str("\nPrevention report...\n");
    sb_prevention_report();

    print_str("\nEmergency report...\n");
    sb_emergency_report();

    print_str("\nFinal state...\n");
    sb_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
