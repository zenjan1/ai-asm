/* superconductor_admin: Superconductor administration system (v1.0)
 * Low-temp SC, high-temp SC, iron-based SC, SC applications, SC devices
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

#define MAX_LOWTEMP    16
#define MAX_HIGHTEMP   14
#define MAX_IRONBASE   12
#define MAX_APPSC      10
#define MAX_DEVICE     10

typedef struct {
    int    lt_id;
    int    lt_type;
    int    lt_category;
    int    nbt_sc;
    int    nb3sn_sc;
    int    v3ga_sc;
    int    year;
    int    active;
} lowtemp_t;

typedef struct {
    int    ht_id;
    int    ht_type;
    int    ht_category;
    int    ybco_sc;
    int    bscco_sc;
    int    hbcco_sc;
    int    year;
    int    active;
} hightemp_t;

typedef struct {
    int    fe_id;
    int    fe_type;
    int    fe_category;
    int    lafeo_sc;
    int    smfeo_sc;
    int    fese_sc;
    int    year;
    int    active;
} ironbase_t;

typedef struct {
    int    app_id;
    int    app_type;
    int    app_category;
    int    sc_magnet;
    int    sc_cable;
    int    sc_storage;
    int    year;
    int    active;
} appsc_t;

typedef struct {
    int    dev_id;
    int    dev_type;
    int    dev_category;
    int    josephson;
    int    squid_dev;
    int    sc_qubit;
    int    year;
    int    active;
} device_t;

typedef struct {
    int    n_lowtemp;
    int    n_hightemp;
    int    n_ironbase;
    int    n_appsc;
    int    n_device;
    int    total_nbt;
    int    total_ybco;
    int    total_lafeo;
    int    total_magnet;
    int    total_josephson;
} scs_state_t;

static lowtemp_t lowtemps[MAX_LOWTEMP];
static hightemp_t hightemps[MAX_HIGHTEMP];
static ironbase_t ironbases[MAX_IRONBASE];
static appsc_t appscs[MAX_APPSC];
static device_t devices[MAX_DEVICE];
static scs_state_t scs;

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

int scs_init(void) {
    if (initialized) return -1;
    scs.n_lowtemp = 0; scs.n_hightemp = 0; scs.n_ironbase = 0;
    scs.n_appsc = 0; scs.n_device = 0;
    scs.total_nbt = 0; scs.total_ybco = 0;
    scs.total_lafeo = 0; scs.total_magnet = 0;
    scs.total_josephson = 0;
    for (int i = 0; i < MAX_LOWTEMP; i++) lowtemps[i].active = 0;
    for (int i = 0; i < MAX_HIGHTEMP; i++) hightemps[i].active = 0;
    for (int i = 0; i < MAX_IRONBASE; i++) ironbases[i].active = 0;
    for (int i = 0; i < MAX_APPSC; i++) appscs[i].active = 0;
    for (int i = 0; i < MAX_DEVICE; i++) devices[i].active = 0;
    initialized = 1;
    print_str("[SCS] Superconductor initialized\n");
    return 0;
}

int scs_lowtemp(int lt_type, int cat, int nbt, int nb3, int v3g, int year) {
    if (scs.n_lowtemp >= MAX_LOWTEMP) return -1;
    lowtemp_t* l = &lowtemps[scs.n_lowtemp];
    l->lt_id = scs.n_lowtemp;
    l->lt_type = lt_type;
    l->lt_category = cat;
    l->nbt_sc = nbt;
    l->nb3sn_sc = nb3;
    l->v3ga_sc = v3g;
    l->year = year;
    l->active = 1;
    scs.total_nbt += nbt;
    scs.n_lowtemp++;
    print_str("[SCS] Low-temp "); print_int(scs.n_lowtemp - 1);
    print_str(" type="); print_int(lt_type);
    print_str(" cat="); print_int(cat);
    print_str(" nbt="); print_int(nbt);
    print_str(" nb3="); print_int(nb3);
    print_str(" v3g="); print_int(v3g); print_str("\n");
    return scs.n_lowtemp - 1;
}

int scs_hightemp(int ht_type, int cat, int yb, int bsc, int hbc, int year) {
    if (scs.n_hightemp >= MAX_HIGHTEMP) return -1;
    hightemp_t* h = &hightemps[scs.n_hightemp];
    h->ht_id = scs.n_hightemp;
    h->ht_type = ht_type;
    h->ht_category = cat;
    h->ybco_sc = yb;
    h->bscco_sc = bsc;
    h->hbcco_sc = hbc;
    h->year = year;
    h->active = 1;
    scs.total_ybco += yb;
    scs.n_hightemp++;
    print_str("[SCS] High-temp "); print_int(scs.n_hightemp - 1);
    print_str(" type="); print_int(ht_type);
    print_str(" cat="); print_int(cat);
    print_str(" ybc="); print_int(yb);
    print_str(" bsc="); print_int(bsc);
    print_str(" hbc="); print_int(hbc); print_str("\n");
    return scs.n_hightemp - 1;
}

int scs_ironbase(int fe_type, int cat, int lafe, int smfe, int fese_v, int year) {
    if (scs.n_ironbase >= MAX_IRONBASE) return -1;
    ironbase_t* ir = &ironbases[scs.n_ironbase];
    ir->fe_id = scs.n_ironbase;
    ir->fe_type = fe_type;
    ir->fe_category = cat;
    ir->lafeo_sc = lafe;
    ir->smfeo_sc = smfe;
    ir->fese_sc = fese_v;
    ir->year = year;
    ir->active = 1;
    scs.total_lafeo += lafe;
    scs.n_ironbase++;
    print_str("[SCS] Iron-base "); print_int(scs.n_ironbase - 1);
    print_str(" type="); print_int(fe_type);
    print_str(" cat="); print_int(cat);
    print_str(" lfe="); print_int(lafe);
    print_str(" sfe="); print_int(smfe);
    print_str(" fse="); print_int(fese_v); print_str("\n");
    return scs.n_ironbase - 1;
}

int scs_appsc(int ap_type, int cat, int magnet_v, int cable_v, int store_v, int year) {
    if (scs.n_appsc >= MAX_APPSC) return -1;
    appsc_t* a = &appscs[scs.n_appsc];
    a->app_id = scs.n_appsc;
    a->app_type = ap_type;
    a->app_category = cat;
    a->sc_magnet = magnet_v;
    a->sc_cable = cable_v;
    a->sc_storage = store_v;
    a->year = year;
    a->active = 1;
    scs.total_magnet += magnet_v;
    scs.n_appsc++;
    print_str("[SCS] App "); print_int(scs.n_appsc - 1);
    print_str(" type="); print_int(ap_type);
    print_str(" cat="); print_int(cat);
    print_str(" mgn="); print_int(magnet_v);
    print_str(" cbl="); print_int(cable_v);
    print_str(" str="); print_int(store_v); print_str("\n");
    return scs.n_appsc - 1;
}

int scs_device(int dv_type, int cat, int josep, int squid_v, int qubit_v, int year) {
    if (scs.n_device >= MAX_DEVICE) return -1;
    device_t* d = &devices[scs.n_device];
    d->dev_id = scs.n_device;
    d->dev_type = dv_type;
    d->dev_category = cat;
    d->josephson = josep;
    d->squid_dev = squid_v;
    d->sc_qubit = qubit_v;
    d->year = year;
    d->active = 1;
    scs.total_josephson += josep;
    scs.n_device++;
    print_str("[SCS] Device "); print_int(scs.n_device - 1);
    print_str(" type="); print_int(dv_type);
    print_str(" cat="); print_int(cat);
    print_str(" jsp="); print_int(josep);
    print_str(" sqd="); print_int(squid_v);
    print_str(" qbt="); print_int(qubit_v); print_str("\n");
    return scs.n_device - 1;
}

void scs_lowtemp_report(void) {
    print_str("[SCS] Low-temp report:\n");
    print_str("  Low-temp SC categories: "); print_int(scs.n_lowtemp); print_str("\n");
    print_str("  Total NbTi SC: "); print_int(scs.total_nbt); print_str("\n");
}

void scs_hightemp_report(void) {
    print_str("[SCS] High-temp report:\n");
    print_str("  High-temp SC categories: "); print_int(scs.n_hightemp); print_str("\n");
    print_str("  Total YBCO SC: "); print_int(scs.total_ybco); print_str("\n");
}

void scs_device_report(void) {
    print_str("[SCS] Device report:\n");
    print_str("  Iron-based SC categories: "); print_int(scs.n_ironbase); print_str("\n");
    print_str("  Total LaFeAsO SC: "); print_int(scs.total_lafeo); print_str("\n");
    print_str("  SC application categories: "); print_int(scs.n_appsc); print_str("\n");
    print_str("  Total SC magnets: "); print_int(scs.total_magnet); print_str("\n");
    print_str("  SC device categories: "); print_int(scs.n_device); print_str("\n");
    print_str("  Total Josephson junctions: "); print_int(scs.total_josephson); print_str("\n");
}

void scs_print_state(void) {
    print_str("[SCS] Lt="); print_int(scs.n_lowtemp);
    print_str(" Ht="); print_int(scs.n_hightemp);
    print_str(" Fe="); print_int(scs.n_ironbase);
    print_str(" Ap="); print_int(scs.n_appsc);
    print_str(" Dv="); print_int(scs.n_device);
    print_str("\n");
}

int main(void) {
    print_str("=== Superconductor Admin Demo ===\n\n");
    scs_init();

    print_str("Low-temp SC...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int nbt = 55 + (i * 13);
        int nb3 = 40 + (i * 10);
        int v3g = 22 + (i * 5);
        int year = 2020 + (i % 5);
        scs_lowtemp(type, cat, nbt, nb3, v3g, year);
    }

    print_str("\nHigh-temp SC...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ybc = 48 + (i * 12);
        int bsc = 35 + (i * 8);
        int hbc = 20 + (i * 5);
        int year = 2021 + (i % 4);
        scs_hightemp(type, cat, ybc, bsc, hbc, year);
    }

    print_str("\nIron-based SC...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int lfe = 42 + (i * 10);
        int sfe = 28 + (i * 7);
        int fse = 18 + (i * 4);
        int year = 2022 + (i % 3);
        scs_ironbase(type, cat, lfe, sfe, fse, year);
    }

    print_str("\nSC applications...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int mgn = 35 + (i * 8);
        int cbl = 25 + (i * 6);
        int str = 15 + (i * 3);
        int year = 2023 + (i % 2);
        scs_appsc(type, cat, mgn, cbl, str, year);
    }

    print_str("\nSC devices...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int jsp = 30 + (i * 7);
        int sqd = 22 + (i * 5);
        int qbt = 12 + (i * 3);
        int year = 2024;
        scs_device(type, cat, jsp, sqd, qbt, year);
    }

    print_str("\nLow-temp report...\n");
    scs_lowtemp_report();

    print_str("\nHigh-temp report...\n");
    scs_hightemp_report();

    print_str("\nDevice report...\n");
    scs_device_report();

    print_str("\nFinal state...\n");
    scs_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
