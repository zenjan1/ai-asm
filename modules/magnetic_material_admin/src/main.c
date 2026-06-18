/* magnetic_material_admin: Magnetic materials administration system (v1.0)
 * Soft magnetic, hard magnetic, magnetic recording, magnetostrictive, magnetic fluids
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

#define MAX_SOFT       16
#define MAX_HARD       14
#define MAX_RECORD     12
#define MAX_STRICT     10
#define MAX_FLUID      10

typedef struct {
    int    soft_id;
    int    soft_type;
    int    soft_category;
    int    silicon_steel;
    int    fe_ni_alloy;
    int    amorphous;
    int    year;
    int    active;
} soft_t;

typedef struct {
    int    hard_id;
    int    hard_type;
    int    hard_category;
    int    alnico;
    int    smco;
    int    fecrco;
    int    year;
    int    active;
} hard_t;

typedef struct {
    int    rec_id;
    int    rec_type;
    int    rec_category;
    int    mag_tape;
    int    mag_disk;
    int    mag_card;
    int    year;
    int    active;
} record_t;

typedef struct {
    int    str_id;
    int    str_type;
    int    str_category;
    int    tbdfe;
    int    nizin_fer;
    int    mnzn_fer;
    int    year;
    int    active;
} strict_t;

typedef struct {
    int    fl_id;
    int    fl_type;
    int    fl_category;
    int    ferro_fluid;
    int    mag_seal;
    int    mag_lubric;
    int    year;
    int    active;
} fluid_t;

typedef struct {
    int    n_soft;
    int    n_hard;
    int    n_record;
    int    n_strict;
    int    n_fluid;
    int    total_silicon;
    int    total_alnico;
    int    total_tape;
    int    total_tbdfe;
    int    total_ferro;
} mma_state_t;

static soft_t softs[MAX_SOFT];
static hard_t hards[MAX_HARD];
static record_t records[MAX_RECORD];
static strict_t stricts[MAX_STRICT];
static fluid_t fluids[MAX_FLUID];
static mma_state_t mma;

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

int mma_init(void) {
    if (initialized) return -1;
    mma.n_soft = 0; mma.n_hard = 0; mma.n_record = 0;
    mma.n_strict = 0; mma.n_fluid = 0;
    mma.total_silicon = 0; mma.total_alnico = 0;
    mma.total_tape = 0; mma.total_tbdfe = 0;
    mma.total_ferro = 0;
    for (int i = 0; i < MAX_SOFT; i++) softs[i].active = 0;
    for (int i = 0; i < MAX_HARD; i++) hards[i].active = 0;
    for (int i = 0; i < MAX_RECORD; i++) records[i].active = 0;
    for (int i = 0; i < MAX_STRICT; i++) stricts[i].active = 0;
    for (int i = 0; i < MAX_FLUID; i++) fluids[i].active = 0;
    initialized = 1;
    print_str("[MMA] Magnetic materials initialized\n");
    return 0;
}

int mma_soft(int sf_type, int cat, int sil_steel, int feni, int amor, int year) {
    if (mma.n_soft >= MAX_SOFT) return -1;
    soft_t* s = &softs[mma.n_soft];
    s->soft_id = mma.n_soft;
    s->soft_type = sf_type;
    s->soft_category = cat;
    s->silicon_steel = sil_steel;
    s->fe_ni_alloy = feni;
    s->amorphous = amor;
    s->year = year;
    s->active = 1;
    mma.total_silicon += sil_steel;
    mma.n_soft++;
    print_str("[MMA] Soft "); print_int(mma.n_soft - 1);
    print_str(" type="); print_int(sf_type);
    print_str(" cat="); print_int(cat);
    print_str(" sst="); print_int(sil_steel);
    print_str(" fni="); print_int(feni);
    print_str(" amr="); print_int(amor); print_str("\n");
    return mma.n_soft - 1;
}

int mma_hard(int hd_type, int cat, int aln, int smc, int fecr, int year) {
    if (mma.n_hard >= MAX_HARD) return -1;
    hard_t* h = &hards[mma.n_hard];
    h->hard_id = mma.n_hard;
    h->hard_type = hd_type;
    h->hard_category = cat;
    h->alnico = aln;
    h->smco = smc;
    h->fecrco = fecr;
    h->year = year;
    h->active = 1;
    mma.total_alnico += aln;
    mma.n_hard++;
    print_str("[MMA] Hard "); print_int(mma.n_hard - 1);
    print_str(" type="); print_int(hd_type);
    print_str(" cat="); print_int(cat);
    print_str(" alc="); print_int(aln);
    print_str(" smc="); print_int(smc);
    print_str(" fcr="); print_int(fecr); print_str("\n");
    return mma.n_hard - 1;
}

int mma_record(int rc_type, int cat, int tape, int disk, int card, int year) {
    if (mma.n_record >= MAX_RECORD) return -1;
    record_t* r = &records[mma.n_record];
    r->rec_id = mma.n_record;
    r->rec_type = rc_type;
    r->rec_category = cat;
    r->mag_tape = tape;
    r->mag_disk = disk;
    r->mag_card = card;
    r->year = year;
    r->active = 1;
    mma.total_tape += tape;
    mma.n_record++;
    print_str("[MMA] Record "); print_int(mma.n_record - 1);
    print_str(" type="); print_int(rc_type);
    print_str(" cat="); print_int(cat);
    print_str(" tpe="); print_int(tape);
    print_str(" dsk="); print_int(disk);
    print_str(" crd="); print_int(card); print_str("\n");
    return mma.n_record - 1;
}

int mma_strict(int st_type, int cat, int tbdfe_v, int nizin, int mnzn, int year) {
    if (mma.n_strict >= MAX_STRICT) return -1;
    strict_t* s = &stricts[mma.n_strict];
    s->str_id = mma.n_strict;
    s->str_type = st_type;
    s->str_category = cat;
    s->tbdfe = tbdfe_v;
    s->nizin_fer = nizin;
    s->mnzn_fer = mnzn;
    s->year = year;
    s->active = 1;
    mma.total_tbdfe += tbdfe_v;
    mma.n_strict++;
    print_str("[MMA] Strict "); print_int(mma.n_strict - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" tbd="); print_int(tbdfe_v);
    print_str(" niz="); print_int(nizin);
    print_str(" mnz="); print_int(mnzn); print_str("\n");
    return mma.n_strict - 1;
}

int mma_fluid(int fl_type, int cat, int ff, int m_seal, int mlub, int year) {
    if (mma.n_fluid >= MAX_FLUID) return -1;
    fluid_t* f = &fluids[mma.n_fluid];
    f->fl_id = mma.n_fluid;
    f->fl_type = fl_type;
    f->fl_category = cat;
    f->ferro_fluid = ff;
    f->mag_seal = m_seal;
    f->mag_lubric = mlub;
    f->year = year;
    f->active = 1;
    mma.total_ferro += ff;
    mma.n_fluid++;
    print_str("[MMA] Fluid "); print_int(mma.n_fluid - 1);
    print_str(" type="); print_int(fl_type);
    print_str(" cat="); print_int(cat);
    print_str(" ffl="); print_int(ff);
    print_str(" msl="); print_int(m_seal);
    print_str(" mlu="); print_int(mlub); print_str("\n");
    return mma.n_fluid - 1;
}

void mma_soft_report(void) {
    print_str("[MMA] Soft report:\n");
    print_str("  Soft magnetic categories: "); print_int(mma.n_soft); print_str("\n");
    print_str("  Total silicon steel: "); print_int(mma.total_silicon); print_str("\n");
}

void mma_hard_report(void) {
    print_str("[MMA] Hard report:\n");
    print_str("  Hard magnetic categories: "); print_int(mma.n_hard); print_str("\n");
    print_str("  Total AlNiCo: "); print_int(mma.total_alnico); print_str("\n");
}

void mma_fluid_report(void) {
    print_str("[MMA] Fluid report:\n");
    print_str("  Magnetic recording categories: "); print_int(mma.n_record); print_str("\n");
    print_str("  Total magnetic tape: "); print_int(mma.total_tape); print_str("\n");
    print_str("  Magnetostrictive categories: "); print_int(mma.n_strict); print_str("\n");
    print_str("  Total TbDyFe: "); print_int(mma.total_tbdfe); print_str("\n");
    print_str("  Magnetic fluid categories: "); print_int(mma.n_fluid); print_str("\n");
    print_str("  Total ferrofluid: "); print_int(mma.total_ferro); print_str("\n");
}

void mma_print_state(void) {
    print_str("[MMA] Sf="); print_int(mma.n_soft);
    print_str(" Hd="); print_int(mma.n_hard);
    print_str(" Rc="); print_int(mma.n_record);
    print_str(" St="); print_int(mma.n_strict);
    print_str(" Fl="); print_int(mma.n_fluid);
    print_str("\n");
}

int main(void) {
    print_str("=== Magnetic Materials Admin Demo ===\n\n");
    mma_init();

    print_str("Soft magnetic...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int sst = 55 + (i * 13);
        int fni = 40 + (i * 10);
        int amr = 22 + (i * 5);
        int year = 2020 + (i % 5);
        mma_soft(type, cat, sst, fni, amr, year);
    }

    print_str("\nHard magnetic...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int alc = 48 + (i * 12);
        int smc = 35 + (i * 8);
        int fcr = 20 + (i * 5);
        int year = 2021 + (i % 4);
        mma_hard(type, cat, alc, smc, fcr, year);
    }

    print_str("\nMagnetic recording...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int tpe = 42 + (i * 10);
        int dsk = 28 + (i * 7);
        int crd = 18 + (i * 4);
        int year = 2022 + (i % 3);
        mma_record(type, cat, tpe, dsk, crd, year);
    }

    print_str("\nMagnetostrictive...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int tbd = 35 + (i * 8);
        int niz = 25 + (i * 6);
        int mnz = 15 + (i * 3);
        int year = 2023 + (i % 2);
        mma_strict(type, cat, tbd, niz, mnz, year);
    }

    print_str("\nMagnetic fluids...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ffl = 30 + (i * 7);
        int msl = 22 + (i * 5);
        int mlu = 12 + (i * 3);
        int year = 2024;
        mma_fluid(type, cat, ffl, msl, mlu, year);
    }

    print_str("\nSoft report...\n");
    mma_soft_report();

    print_str("\nHard report...\n");
    mma_hard_report();

    print_str("\nFluid report...\n");
    mma_fluid_report();

    print_str("\nFinal state...\n");
    mma_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
