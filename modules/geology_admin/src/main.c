/* geology_admin: Geology administration system (v1.0)
 * Mineralogy, petrology, structural geology, paleontology, deposit geology
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

#define MAX_MINERAL     16
#define MAX_PETRO       14
#define MAX_STRUCT_G    12
#define MAX_PALEO       10
#define MAX_DEPOSIT     10

typedef struct {
    int    mi_id;
    int    mi_type;
    int    mi_category;
    int    min_iden;
    int    min_phy;
    int    min_chem;
    int    min_gen;
    int    year;
    int    active;
} mineral_t;

typedef struct {
    int    pt_id;
    int    pt_type;
    int    pt_category;
    int    ign_rock;
    int    sed_rock;
    int    met_rock;
    int    rock_gen;
    int    year;
    int    active;
} petro_t;

typedef struct {
    int    sg_id;
    int    sg_type;
    int    sg_category;
    int    geo_str;
    int    plat_tec;
    int    seis_geo;
    int    act_str;
    int    year;
    int    active;
} struct_g_t;

typedef struct {
    int    pa_id;
    int    pa_type;
    int    pa_category;
    int    pal_iden;
    int    str_pal;
    int    mic_pal;
    int    trace_fos;
    int    year;
    int    active;
} paleo_t;

typedef struct {
    int    dp_id;
    int    dp_type;
    int    dp_category;
    int    dep_gen;
    int    min_exp;
    int    dep_eva;
    int    min_geo;
    int    year;
    int    active;
} deposit_t;

typedef struct {
    int    n_mineral;
    int    n_petro;
    int    n_struct_g;
    int    n_paleo;
    int    n_deposit;
    int    total_min_iden;
    int    total_ign_rock;
    int    total_geo_str;
    int    total_pal_iden;
    int    total_dep_gen;
} gla_state_t;

static mineral_t mineral_s[MAX_MINERAL];
static petro_t petros[MAX_PETRO];
static struct_g_t struct_gs[MAX_STRUCT_G];
static paleo_t paleos[MAX_PALEO];
static deposit_t deposits[MAX_DEPOSIT];
static gla_state_t gla;

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

int gla_init(void) {
    if (initialized) return -1;
    gla.n_mineral = 0; gla.n_petro = 0; gla.n_struct_g = 0;
    gla.n_paleo = 0; gla.n_deposit = 0;
    gla.total_min_iden = 0; gla.total_ign_rock = 0;
    gla.total_geo_str = 0; gla.total_pal_iden = 0;
    gla.total_dep_gen = 0;
    for (int i = 0; i < MAX_MINERAL; i++) mineral_s[i].active = 0;
    for (int i = 0; i < MAX_PETRO; i++) petros[i].active = 0;
    for (int i = 0; i < MAX_STRUCT_G; i++) struct_gs[i].active = 0;
    for (int i = 0; i < MAX_PALEO; i++) paleos[i].active = 0;
    for (int i = 0; i < MAX_DEPOSIT; i++) deposits[i].active = 0;
    initialized = 1;
    print_str("[GLA] Geology initialized\n");
    return 0;
}

int gla_mineral(int mt_type, int cat, int mid, int mphy, int mch, int mgn, int year) {
    if (gla.n_mineral >= MAX_MINERAL) return -1;
    mineral_t* m = &mineral_s[gla.n_mineral];
    m->mi_id = gla.n_mineral;
    m->mi_type = mt_type;
    m->mi_category = cat;
    m->min_iden = mid;
    m->min_phy = mphy;
    m->min_chem = mch;
    m->min_gen = mgn;
    m->year = year;
    m->active = 1;
    gla.total_min_iden += mid;
    gla.n_mineral++;
    print_str("[GLA] Mineral "); print_int(gla.n_mineral - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" mid="); print_int(mid);
    print_str(" mphy="); print_int(mphy);
    print_str(" mch="); print_int(mch);
    print_str(" mgn="); print_int(mgn); print_str("\n");
    return gla.n_mineral - 1;
}

int gla_petro(int pt_type, int cat, int ign, int sed, int met, int rgn, int year) {
    if (gla.n_petro >= MAX_PETRO) return -1;
    petro_t* p = &petros[gla.n_petro];
    p->pt_id = gla.n_petro;
    p->pt_type = pt_type;
    p->pt_category = cat;
    p->ign_rock = ign;
    p->sed_rock = sed;
    p->met_rock = met;
    p->rock_gen = rgn;
    p->year = year;
    p->active = 1;
    gla.total_ign_rock += ign;
    gla.n_petro++;
    print_str("[GLA] Petro "); print_int(gla.n_petro - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" ign="); print_int(ign);
    print_str(" sed="); print_int(sed);
    print_str(" met="); print_int(met);
    print_str(" rgn="); print_int(rgn); print_str("\n");
    return gla.n_petro - 1;
}

int gla_struct_g(int st_type, int cat, int gstr, int plt, int sze, int astr, int year) {
    if (gla.n_struct_g >= MAX_STRUCT_G) return -1;
    struct_g_t* s = &struct_gs[gla.n_struct_g];
    s->sg_id = gla.n_struct_g;
    s->sg_type = st_type;
    s->sg_category = cat;
    s->geo_str = gstr;
    s->plat_tec = plt;
    s->seis_geo = sze;
    s->act_str = astr;
    s->year = year;
    s->active = 1;
    gla.total_geo_str += gstr;
    gla.n_struct_g++;
    print_str("[GLA] Struct g "); print_int(gla.n_struct_g - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" gstr="); print_int(gstr);
    print_str(" plt="); print_int(plt);
    print_str(" sze="); print_int(sze);
    print_str(" astr="); print_int(astr); print_str("\n");
    return gla.n_struct_g - 1;
}

int gla_paleo(int pt_type, int cat, int plid, int stp, int mcp, int trf, int year) {
    if (gla.n_paleo >= MAX_PALEO) return -1;
    paleo_t* p = &paleos[gla.n_paleo];
    p->pa_id = gla.n_paleo;
    p->pa_type = pt_type;
    p->pa_category = cat;
    p->pal_iden = plid;
    p->str_pal = stp;
    p->mic_pal = mcp;
    p->trace_fos = trf;
    p->year = year;
    p->active = 1;
    gla.total_pal_iden += plid;
    gla.n_paleo++;
    print_str("[GLA] Paleo "); print_int(gla.n_paleo - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" plid="); print_int(plid);
    print_str(" stp="); print_int(stp);
    print_str(" mcp="); print_int(mcp);
    print_str(" trf="); print_int(trf); print_str("\n");
    return gla.n_paleo - 1;
}

int gla_deposit(int dt_type, int cat, int dgn, int mex, int dev, int mge, int year) {
    if (gla.n_deposit >= MAX_DEPOSIT) return -1;
    deposit_t* d = &deposits[gla.n_deposit];
    d->dp_id = gla.n_deposit;
    d->dp_type = dt_type;
    d->dp_category = cat;
    d->dep_gen = dgn;
    d->min_exp = mex;
    d->dep_eva = dev;
    d->min_geo = mge;
    d->year = year;
    d->active = 1;
    gla.total_dep_gen += dgn;
    gla.n_deposit++;
    print_str("[GLA] Deposit "); print_int(gla.n_deposit - 1);
    print_str(" type="); print_int(dt_type);
    print_str(" cat="); print_int(cat);
    print_str(" dgn="); print_int(dgn);
    print_str(" mex="); print_int(mex);
    print_str(" dev="); print_int(dev);
    print_str(" mge="); print_int(mge); print_str("\n");
    return gla.n_deposit - 1;
}

void gla_mineral_report(void) {
    print_str("[GLA] Mineralogy report:\n");
    print_str("  Mineral categories: "); print_int(gla.n_mineral); print_str("\n");
    print_str("  Total mineral identification: "); print_int(gla.total_min_iden); print_str("\n");
}

void gla_petro_report(void) {
    print_str("[GLA] Petrology report:\n");
    print_str("  Petrology categories: "); print_int(gla.n_petro); print_str("\n");
    print_str("  Total igneous rock: "); print_int(gla.total_ign_rock); print_str("\n");
}

void gla_full_report(void) {
    print_str("[GLA] Full report:\n");
    print_str("  Structural geology categories: "); print_int(gla.n_struct_g); print_str("\n");
    print_str("  Total geo structure: "); print_int(gla.total_geo_str); print_str("\n");
    print_str("  Paleontology categories: "); print_int(gla.n_paleo); print_str("\n");
    print_str("  Total paleo identification: "); print_int(gla.total_pal_iden); print_str("\n");
    print_str("  Deposit geology categories: "); print_int(gla.n_deposit); print_str("\n");
    print_str("  Total deposit genesis: "); print_int(gla.total_dep_gen); print_str("\n");
}

void gla_print_state(void) {
    print_str("[GLA] Mi="); print_int(gla.n_mineral);
    print_str(" Pt="); print_int(gla.n_petro);
    print_str(" Sg="); print_int(gla.n_struct_g);
    print_str(" Pa="); print_int(gla.n_paleo);
    print_str(" Dp="); print_int(gla.n_deposit);
    print_str("\n");
}

int main(void) {
    print_str("=== Geology Admin Demo ===\n\n");
    gla_init();

    print_str("Mineralogy...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int mid = 55 + (i * 13);
        int mphy = 40 + (i * 10);
        int mch = 22 + (i * 5);
        int mgn = 15 + (i * 3);
        int year = 2020 + (i % 5);
        gla_mineral(type, cat, mid, mphy, mch, mgn, year);
    }

    print_str("\nPetrology...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ign = 48 + (i * 11);
        int sed = 35 + (i * 8);
        int met = 20 + (i * 4);
        int rgn = 12 + (i * 3);
        int year = 2021 + (i % 4);
        gla_petro(type, cat, ign, sed, met, rgn, year);
    }

    print_str("\nStructural geology...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int gstr = 42 + (i * 10);
        int plt = 28 + (i * 7);
        int sze = 18 + (i * 4);
        int astr = 10 + (i * 2);
        int year = 2022 + (i % 3);
        gla_struct_g(type, cat, gstr, plt, sze, astr, year);
    }

    print_str("\nPaleontology...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int plid = 35 + (i * 8);
        int stp = 25 + (i * 6);
        int mcp = 15 + (i * 3);
        int trf = 10 + (i * 2);
        int year = 2023 + (i % 2);
        gla_paleo(type, cat, plid, stp, mcp, trf, year);
    }

    print_str("\nDeposit geology...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int dgn = 30 + (i * 7);
        int mex = 22 + (i * 5);
        int dev = 12 + (i * 3);
        int mge = 8 + (i * 2);
        int year = 2024;
        gla_deposit(type, cat, dgn, mex, dev, mge, year);
    }

    print_str("\nMineral report...\n");
    gla_mineral_report();

    print_str("\nPetro report...\n");
    gla_petro_report();

    print_str("\nFull report...\n");
    gla_full_report();

    print_str("\nFinal state...\n");
    gla_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
