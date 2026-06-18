/* optical_engineering_admin: Optical engineering administration system (v1.0)
 * Geometric optics, physical optics, optical design, optical manufacturing, optoelectronic systems
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

#define MAX_GEOMETRIC    16
#define MAX_PHYSICAL     14
#define MAX_DESIGN       12
#define MAX_MANUFACT     10
#define MAX_OPTOELEC     10

typedef struct {
    int    geo_id;
    int    geo_type;
    int    geo_category;
    int    lens_system;
    int    mirror_system;
    int    prism_system;
    int    year;
    int    active;
} geometric_t;

typedef struct {
    int    phys_id;
    int    phys_type;
    int    phys_category;
    int    interferometer;
    int    diffraction;
    int    polarization;
    int    year;
    int    active;
} physical_t;

typedef struct {
    int    dsn_id;
    int    dsn_type;
    int    dsn_category;
    int    lens_design;
    int    system_optim;
    int    aberration;
    int    year;
    int    active;
} design_o_t;

typedef struct {
    int    mfg_id;
    int    mfg_type;
    int    mfg_category;
    int    grinding;
    int    coating;
    int    inspection;
    int    year;
    int    active;
} manufact_t;

typedef struct {
    int    oe_id;
    int    oe_type;
    int    oe_category;
    int    ccd_camera;
    int    cmos_sensor;
    int    photodetector;
    int    year;
    int    active;
} optoelec_t;

typedef struct {
    int    n_geometric;
    int    n_physical;
    int    n_design;
    int    n_manufact;
    int    n_optoelec;
    int    total_lens;
    int    total_interfero;
    int    total_lens_dsn;
    int    total_grinding;
    int    total_ccd;
} oea_state_t;

static geometric_t geometrics[MAX_GEOMETRIC];
static physical_t physicals[MAX_PHYSICAL];
static design_o_t designs[MAX_DESIGN];
static manufact_t manufs[MAX_MANUFACT];
static optoelec_t optoelecs[MAX_OPTOELEC];
static oea_state_t oea;

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

int oea_init(void) {
    if (initialized) return -1;
    oea.n_geometric = 0; oea.n_physical = 0; oea.n_design = 0;
    oea.n_manufact = 0; oea.n_optoelec = 0;
    oea.total_lens = 0; oea.total_interfero = 0;
    oea.total_lens_dsn = 0; oea.total_grinding = 0;
    oea.total_ccd = 0;
    for (int i = 0; i < MAX_GEOMETRIC; i++) geometrics[i].active = 0;
    for (int i = 0; i < MAX_PHYSICAL; i++) physicals[i].active = 0;
    for (int i = 0; i < MAX_DESIGN; i++) designs[i].active = 0;
    for (int i = 0; i < MAX_MANUFACT; i++) manufs[i].active = 0;
    for (int i = 0; i < MAX_OPTOELEC; i++) optoelecs[i].active = 0;
    initialized = 1;
    print_str("[OEA] Optical engineering initialized\n");
    return 0;
}

int oea_geometric(int g_type, int cat, int lens_s, int mirror_s, int prism_s, int year) {
    if (oea.n_geometric >= MAX_GEOMETRIC) return -1;
    geometric_t* g = &geometrics[oea.n_geometric];
    g->geo_id = oea.n_geometric;
    g->geo_type = g_type;
    g->geo_category = cat;
    g->lens_system = lens_s;
    g->mirror_system = mirror_s;
    g->prism_system = prism_s;
    g->year = year;
    g->active = 1;
    oea.total_lens += lens_s;
    oea.n_geometric++;
    print_str("[OEA] Geometric "); print_int(oea.n_geometric - 1);
    print_str(" type="); print_int(g_type);
    print_str(" cat="); print_int(cat);
    print_str(" lns="); print_int(lens_s);
    print_str(" mir="); print_int(mirror_s);
    print_str(" prm="); print_int(prism_s); print_str("\n");
    return oea.n_geometric - 1;
}

int oea_physical(int p_type, int cat, int inter, int diffr, int polar, int year) {
    if (oea.n_physical >= MAX_PHYSICAL) return -1;
    physical_t* p = &physicals[oea.n_physical];
    p->phys_id = oea.n_physical;
    p->phys_type = p_type;
    p->phys_category = cat;
    p->interferometer = inter;
    p->diffraction = diffr;
    p->polarization = polar;
    p->year = year;
    p->active = 1;
    oea.total_interfero += inter;
    oea.n_physical++;
    print_str("[OEA] Physical "); print_int(oea.n_physical - 1);
    print_str(" type="); print_int(p_type);
    print_str(" cat="); print_int(cat);
    print_str(" int="); print_int(inter);
    print_str(" dif="); print_int(diffr);
    print_str(" pol="); print_int(polar); print_str("\n");
    return oea.n_physical - 1;
}

int oea_design(int d_type, int cat, int lens_d, int optim_v, int aber, int year) {
    if (oea.n_design >= MAX_DESIGN) return -1;
    design_o_t* d = &designs[oea.n_design];
    d->dsn_id = oea.n_design;
    d->dsn_type = d_type;
    d->dsn_category = cat;
    d->lens_design = lens_d;
    d->system_optim = optim_v;
    d->aberration = aber;
    d->year = year;
    d->active = 1;
    oea.total_lens_dsn += lens_d;
    oea.n_design++;
    print_str("[OEA] Design "); print_int(oea.n_design - 1);
    print_str(" type="); print_int(d_type);
    print_str(" cat="); print_int(cat);
    print_str(" lnd="); print_int(lens_d);
    print_str(" opt="); print_int(optim_v);
    print_str(" abr="); print_int(aber); print_str("\n");
    return oea.n_design - 1;
}

int oea_manufact(int m_type, int cat, int grind_v, int coat_v, int inspect_v, int year) {
    if (oea.n_manufact >= MAX_MANUFACT) return -1;
    manufact_t* m = &manufs[oea.n_manufact];
    m->mfg_id = oea.n_manufact;
    m->mfg_type = m_type;
    m->mfg_category = cat;
    m->grinding = grind_v;
    m->coating = coat_v;
    m->inspection = inspect_v;
    m->year = year;
    m->active = 1;
    oea.total_grinding += grind_v;
    oea.n_manufact++;
    print_str("[OEA] Manufact "); print_int(oea.n_manufact - 1);
    print_str(" type="); print_int(m_type);
    print_str(" cat="); print_int(cat);
    print_str(" grd="); print_int(grind_v);
    print_str(" cot="); print_int(coat_v);
    print_str(" isp="); print_int(inspect_v); print_str("\n");
    return oea.n_manufact - 1;
}

int oea_optoelec(int o_type, int cat, int ccd_v, int cmos_v, int photo_v, int year) {
    if (oea.n_optoelec >= MAX_OPTOELEC) return -1;
    optoelec_t* o = &optoelecs[oea.n_optoelec];
    o->oe_id = oea.n_optoelec;
    o->oe_type = o_type;
    o->oe_category = cat;
    o->ccd_camera = ccd_v;
    o->cmos_sensor = cmos_v;
    o->photodetector = photo_v;
    o->year = year;
    o->active = 1;
    oea.total_ccd += ccd_v;
    oea.n_optoelec++;
    print_str("[OEA] Optoelec "); print_int(oea.n_optoelec - 1);
    print_str(" type="); print_int(o_type);
    print_str(" cat="); print_int(cat);
    print_str(" ccd="); print_int(ccd_v);
    print_str(" cmo="); print_int(cmos_v);
    print_str(" pht="); print_int(photo_v); print_str("\n");
    return oea.n_optoelec - 1;
}

void oea_geometric_report(void) {
    print_str("[OEA] Geometric report:\n");
    print_str("  Geometric optics categories: "); print_int(oea.n_geometric); print_str("\n");
    print_str("  Total lens systems: "); print_int(oea.total_lens); print_str("\n");
}

void oea_physical_report(void) {
    print_str("[OEA] Physical report:\n");
    print_str("  Physical optics categories: "); print_int(oea.n_physical); print_str("\n");
    print_str("  Total interferometers: "); print_int(oea.total_interfero); print_str("\n");
}

void oea_optoelec_report(void) {
    print_str("[OEA] Optoelec report:\n");
    print_str("  Optical design categories: "); print_int(oea.n_design); print_str("\n");
    print_str("  Total lens design: "); print_int(oea.total_lens_dsn); print_str("\n");
    print_str("  Optical manufacturing categories: "); print_int(oea.n_manufact); print_str("\n");
    print_str("  Total optical grinding: "); print_int(oea.total_grinding); print_str("\n");
    print_str("  Optoelectronic system categories: "); print_int(oea.n_optoelec); print_str("\n");
    print_str("  Total CCD cameras: "); print_int(oea.total_ccd); print_str("\n");
}

void oea_print_state(void) {
    print_str("[OEA] Ge="); print_int(oea.n_geometric);
    print_str(" Ph="); print_int(oea.n_physical);
    print_str(" Ds="); print_int(oea.n_design);
    print_str(" Mf="); print_int(oea.n_manufact);
    print_str(" Oe="); print_int(oea.n_optoelec);
    print_str("\n");
}

int main(void) {
    print_str("=== Optical Engineering Admin Demo ===\n\n");
    oea_init();

    print_str("Geometric optics...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int lns = 58 + (i * 14);
        int mir = 40 + (i * 10);
        int prm = 25 + (i * 6);
        int year = 2020 + (i % 5);
        oea_geometric(type, cat, lns, mir, prm, year);
    }

    print_str("\nPhysical optics...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int inter = 50 + (i * 12);
        int dif = 35 + (i * 8);
        int pol = 22 + (i * 5);
        int year = 2021 + (i % 4);
        oea_physical(type, cat, inter, dif, pol, year);
    }

    print_str("\nOptical design...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int lnd = 45 + (i * 11);
        int opt = 30 + (i * 7);
        int abr = 18 + (i * 4);
        int year = 2022 + (i % 3);
        oea_design(type, cat, lnd, opt, abr, year);
    }

    print_str("\nOptical manufacturing...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int grd = 38 + (i * 9);
        int cot = 25 + (i * 6);
        int isp = 15 + (i * 3);
        int year = 2023 + (i % 2);
        oea_manufact(type, cat, grd, cot, isp, year);
    }

    print_str("\nOptoelectronic systems...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ccd = 32 + (i * 8);
        int cmo = 22 + (i * 5);
        int pht = 12 + (i * 3);
        int year = 2024;
        oea_optoelec(type, cat, ccd, cmo, pht, year);
    }

    print_str("\nGeometric report...\n");
    oea_geometric_report();

    print_str("\nPhysical report...\n");
    oea_physical_report();

    print_str("\nOptoelec report...\n");
    oea_optoelec_report();

    print_str("\nFinal state...\n");
    oea_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
