/* surveying_mapping: Surveying and mapping administration system (v1.0)
 * Surveying management, geographic information, survey markers, boundary survey, technology
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

#define MAX_SURVEYING    16
#define MAX_GEOINFO      14
#define MAX_MARKERS      12
#define MAX_BOUNDARY     10
#define MAX_TECHNOLOGY   10

typedef struct {
    int    surveying_id;
    int    qualification_type;
    int    project_class;
    int    projects_approved;
    int    qualifications_issued;
    int    quality_inspections;
    int    year;
    int    active;
} surveying_t;

typedef struct {
    int    geoinfo_id;
    int    data_type;
    int    map_scale;
    int    datasets_produced;
    int    maps_published;
    int    online_services;
    int    year;
    int    active;
} geoinfo_t;

typedef struct {
    int    marker_id;
    int    marker_type;
    int    region_id;
    int    markers_protected;
    int    reference_stations;
    int    coordinate_systems;
    int    year;
    int    active;
} marker_t;

typedef struct {
    int    boundary_id;
    int    boundary_type;
    int    region_pair;
    int    boundaries_surveyed;
    int    disputes_resolved;
    int    maintenance_operations;
    int    year;
    int    active;
} boundary_t;

typedef struct {
    int    technology_id;
    int    research_area;
    int    standard_type;
    int    research_projects;
    int    standards_issued;
    int    intl_cooperation;
    int    year;
    int    active;
} technology_t;

typedef struct {
    int    n_surveying;
    int    n_geoinfo;
    int    n_markers;
    int    n_boundary;
    int    n_technology;
    int    total_projects;
    int    total_datasets;
    int    total_markers;
    int    total_boundaries;
    int    total_standards;
} sm_state_t;

static surveying_t surveyings[MAX_SURVEYING];
static geoinfo_t geoinfos[MAX_GEOINFO];
static marker_t markers[MAX_MARKERS];
static boundary_t boundaries[MAX_BOUNDARY];
static technology_t technologies[MAX_TECHNOLOGY];
static sm_state_t sm;

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

int sm_init(void) {
    if (initialized) return -1;
    sm.n_surveying = 0; sm.n_geoinfo = 0; sm.n_markers = 0;
    sm.n_boundary = 0; sm.n_technology = 0;
    sm.total_projects = 0; sm.total_datasets = 0;
    sm.total_markers = 0; sm.total_boundaries = 0;
    sm.total_standards = 0;
    for (int i = 0; i < MAX_SURVEYING; i++) surveyings[i].active = 0;
    for (int i = 0; i < MAX_GEOINFO; i++) geoinfos[i].active = 0;
    for (int i = 0; i < MAX_MARKERS; i++) markers[i].active = 0;
    for (int i = 0; i < MAX_BOUNDARY; i++) boundaries[i].active = 0;
    for (int i = 0; i < MAX_TECHNOLOGY; i++) technologies[i].active = 0;
    initialized = 1;
    print_str("[SM] Surveying & mapping initialized\n");
    return 0;
}

int sm_surveying(int qual_type, int proj_class, int approved, int issued, int inspections, int year) {
    if (sm.n_surveying >= MAX_SURVEYING) return -1;
    surveying_t* s = &surveyings[sm.n_surveying];
    s->surveying_id = sm.n_surveying;
    s->qualification_type = qual_type;
    s->project_class = proj_class;
    s->projects_approved = approved;
    s->qualifications_issued = issued;
    s->quality_inspections = inspections;
    s->year = year;
    s->active = 1;
    sm.total_projects += approved;
    sm.n_surveying++;
    print_str("[SM] Surveying "); print_int(sm.n_surveying - 1);
    print_str(" qual="); print_int(qual_type);
    print_str(" cls="); print_int(proj_class);
    print_str(" apr="); print_int(approved);
    print_str(" iss="); print_int(issued);
    print_str(" insp="); print_int(inspections); print_str("\n");
    return sm.n_surveying - 1;
}

int sm_geoinfo(int data_type, int scale, int datasets, int maps, int online, int year) {
    if (sm.n_geoinfo >= MAX_GEOINFO) return -1;
    geoinfo_t* g = &geoinfos[sm.n_geoinfo];
    g->geoinfo_id = sm.n_geoinfo;
    g->data_type = data_type;
    g->map_scale = scale;
    g->datasets_produced = datasets;
    g->maps_published = maps;
    g->online_services = online;
    g->year = year;
    g->active = 1;
    sm.total_datasets += datasets;
    sm.n_geoinfo++;
    print_str("[SM] Geoinfo "); print_int(sm.n_geoinfo - 1);
    print_str(" type="); print_int(data_type);
    print_str(" scl=1:"); print_int(scale);
    print_str(" dts="); print_int(datasets);
    print_str(" map="); print_int(maps);
    print_str(" onl="); print_int(online); print_str("\n");
    return sm.n_geoinfo - 1;
}

int sm_marker(int marker_type, int region, int protected_cnt, int ref_stations, int coord_sys, int year) {
    if (sm.n_markers >= MAX_MARKERS) return -1;
    marker_t* m = &markers[sm.n_markers];
    m->marker_id = sm.n_markers;
    m->marker_type = marker_type;
    m->region_id = region;
    m->markers_protected = protected_cnt;
    m->reference_stations = ref_stations;
    m->coordinate_systems = coord_sys;
    m->year = year;
    m->active = 1;
    sm.total_markers += protected_cnt;
    sm.n_markers++;
    print_str("[SM] Marker "); print_int(sm.n_markers - 1);
    print_str(" type="); print_int(marker_type);
    print_str(" rgn="); print_int(region);
    print_str(" prt="); print_int(protected_cnt);
    print_str(" ref="); print_int(ref_stations);
    print_str(" crd="); print_int(coord_sys); print_str("\n");
    return sm.n_markers - 1;
}

int sm_boundary(int bnd_type, int region_pair, int surveyed, int disputes, int maintenance, int year) {
    if (sm.n_boundary >= MAX_BOUNDARY) return -1;
    boundary_t* b = &boundaries[sm.n_boundary];
    b->boundary_id = sm.n_boundary;
    b->boundary_type = bnd_type;
    b->region_pair = region_pair;
    b->boundaries_surveyed = surveyed;
    b->disputes_resolved = disputes;
    b->maintenance_operations = maintenance;
    b->year = year;
    b->active = 1;
    sm.total_boundaries += surveyed;
    sm.n_boundary++;
    print_str("[SM] Boundary "); print_int(sm.n_boundary - 1);
    print_str(" type="); print_int(bnd_type);
    print_str(" pr="); print_int(region_pair);
    print_str(" srv="); print_int(surveyed);
    print_str(" dsp="); print_int(disputes);
    print_str(" mnt="); print_int(maintenance); print_str("\n");
    return sm.n_boundary - 1;
}

int sm_technology(int res_area, int std_type, int projects, int standards, int intl, int year) {
    if (sm.n_technology >= MAX_TECHNOLOGY) return -1;
    technology_t* t = &technologies[sm.n_technology];
    t->technology_id = sm.n_technology;
    t->research_area = res_area;
    t->standard_type = std_type;
    t->research_projects = projects;
    t->standards_issued = standards;
    t->intl_cooperation = intl;
    t->year = year;
    t->active = 1;
    sm.total_standards += standards;
    sm.n_technology++;
    print_str("[SM] Technology "); print_int(sm.n_technology - 1);
    print_str(" area="); print_int(res_area);
    print_str(" type="); print_int(std_type);
    print_str(" prj="); print_int(projects);
    print_str(" std="); print_int(standards);
    print_str(" int="); print_int(intl); print_str("\n");
    return sm.n_technology - 1;
}

void sm_surveying_report(void) {
    print_str("[SM] Surveying report:\n");
    print_str("  Qualification categories: "); print_int(sm.n_surveying); print_str("\n");
    print_str("  Total projects approved: "); print_int(sm.total_projects); print_str("\n");
}

void sm_geoinfo_report(void) {
    print_str("[SM] Geoinfo report:\n");
    print_str("  Data types: "); print_int(sm.n_geoinfo); print_str("\n");
    print_str("  Total datasets: "); print_int(sm.total_datasets); print_str("\n");
}

void sm_boundary_report(void) {
    print_str("[SM] Boundary report:\n");
    print_str("  Survey markers: "); print_int(sm.n_markers); print_str("\n");
    print_str("  Total markers protected: "); print_int(sm.total_markers); print_str("\n");
    print_str("  Boundary surveys: "); print_int(sm.n_boundary); print_str("\n");
    print_str("  Total boundaries: "); print_int(sm.total_boundaries); print_str("\n");
    print_str("  Technology projects: "); print_int(sm.n_technology); print_str("\n");
    print_str("  Total standards issued: "); print_int(sm.total_standards); print_str("\n");
}

void sm_print_state(void) {
    print_str("[SM] Sv="); print_int(sm.n_surveying);
    print_str(" Gi="); print_int(sm.n_geoinfo);
    print_str(" Mk="); print_int(sm.n_markers);
    print_str(" Bn="); print_int(sm.n_boundary);
    print_str(" Tc="); print_int(sm.n_technology);
    print_str("\n");
}

int main(void) {
    print_str("=== Surveying & Mapping Demo ===\n\n");
    sm_init();

    print_str("Surveying management...\n");
    for (int i = 0; i < 16; i++) {
        int qual = (i % 5) + 1;
        int cls = (i % 4) + 1;
        int apr = 50 + (i * 20);
        int iss = 30 + (i * 10);
        int insp = 20 + (i * 8);
        int year = 2020 + (i % 5);
        sm_surveying(qual, cls, apr, iss, insp, year);
    }

    print_str("\nGeographic information...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 6) + 1;
        int scl = 10000 + (i * 5000);
        int dts = 100 + (i * 30);
        int map = 50 + (i * 15);
        int onl = 20 + (i * 8);
        int year = 2021 + (i % 4);
        sm_geoinfo(type, scl, dts, map, onl, year);
    }

    print_str("\nSurvey markers...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int rgn = (i % 8) + 1;
        int prt = 100 + (i * 30);
        int ref = 10 + (i * 3);
        int crd = 2 + (i % 3);
        int year = 2022 + (i % 3);
        sm_marker(type, rgn, prt, ref, crd, year);
    }

    print_str("\nBoundary surveying...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int pr = 100 + (i * 11);
        int srv = 20 + (i * 5);
        int dsp = 3 + (i % 4);
        int mnt = 10 + (i * 3);
        int year = 2023 + (i % 2);
        sm_boundary(type, pr, srv, dsp, mnt, year);
    }

    print_str("\nSurveying technology...\n");
    for (int i = 0; i < 10; i++) {
        int area = (i % 5) + 1;
        int type = (i % 4) + 1;
        int prj = 8 + (i * 3);
        int std = 5 + (i * 2);
        int intl = 2 + (i % 3);
        int year = 2024;
        sm_technology(area, type, prj, std, intl, year);
    }

    print_str("\nSurveying report...\n");
    sm_surveying_report();

    print_str("\nGeoinfo report...\n");
    sm_geoinfo_report();

    print_str("\nBoundary report...\n");
    sm_boundary_report();

    print_str("\nFinal state...\n");
    sm_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
