/* photonics_admin: Photonics administration system (v1.0)
 * Optical communication, optical computing, optical sensing, quantum photonics, biophotonics
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

#define MAX_COMM       16
#define MAX_COMPUTING  14
#define MAX_SENSING    12
#define MAX_QUANTUM    10
#define MAX_BIO        10

typedef struct {
    int    comm_id;
    int    comm_type;
    int    comm_category;
    int    fiber_comm;
    int    free_space;
    int    optical_inter;
    int    year;
    int    active;
} comm_t;

typedef struct {
    int    compute_id;
    int    compute_type;
    int    compute_category;
    int    photonic_chip;
    int    optical_gate;
    int    optical_nn;
    int    year;
    int    active;
} computing_t;

typedef struct {
    int    sense_id;
    int    sense_type;
    int    sense_category;
    int    fiber_sense;
    int    spectral_sense;
    int    optical_img;
    int    year;
    int    active;
} sensing_t;

typedef struct {
    int    quantum_id;
    int    quantum_type;
    int    quantum_category;
    int    single_photon;
    int    quantum_dot;
    int    photon_entgl;
    int    year;
    int    active;
} quantum_p_t;

typedef struct {
    int    bio_id;
    int    bio_type;
    int    bio_category;
    int    fluorescence;
    int    optogenetics;
    int    photodynamic;
    int    year;
    int    active;
} bio_p_t;

typedef struct {
    int    n_comm;
    int    n_computing;
    int    n_sensing;
    int    n_quantum;
    int    n_bio;
    int    total_fiber;
    int    total_chip;
    int    total_fiber_sense;
    int    total_single;
    int    total_fluor;
} pa_state_t;

static comm_t comms[MAX_COMM];
static computing_t computers[MAX_COMPUTING];
static sensing_t sensings[MAX_SENSING];
static quantum_p_t quantums[MAX_QUANTUM];
static bio_p_t bios[MAX_BIO];
static pa_state_t pa;

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

int pa_init(void) {
    if (initialized) return -1;
    pa.n_comm = 0; pa.n_computing = 0; pa.n_sensing = 0;
    pa.n_quantum = 0; pa.n_bio = 0;
    pa.total_fiber = 0; pa.total_chip = 0;
    pa.total_fiber_sense = 0; pa.total_single = 0;
    pa.total_fluor = 0;
    for (int i = 0; i < MAX_COMM; i++) comms[i].active = 0;
    for (int i = 0; i < MAX_COMPUTING; i++) computers[i].active = 0;
    for (int i = 0; i < MAX_SENSING; i++) sensings[i].active = 0;
    for (int i = 0; i < MAX_QUANTUM; i++) quantums[i].active = 0;
    for (int i = 0; i < MAX_BIO; i++) bios[i].active = 0;
    initialized = 1;
    print_str("[PA] Photonics initialized\n");
    return 0;
}

int pa_comm(int cm_type, int cat, int fiber_c, int free_s, int opt_int, int year) {
    if (pa.n_comm >= MAX_COMM) return -1;
    comm_t* c = &comms[pa.n_comm];
    c->comm_id = pa.n_comm;
    c->comm_type = cm_type;
    c->comm_category = cat;
    c->fiber_comm = fiber_c;
    c->free_space = free_s;
    c->optical_inter = opt_int;
    c->year = year;
    c->active = 1;
    pa.total_fiber += fiber_c;
    pa.n_comm++;
    print_str("[PA] Comm "); print_int(pa.n_comm - 1);
    print_str(" type="); print_int(cm_type);
    print_str(" cat="); print_int(cat);
    print_str(" fbr="); print_int(fiber_c);
    print_str(" frs="); print_int(free_s);
    print_str(" opi="); print_int(opt_int); print_str("\n");
    return pa.n_comm - 1;
}

int pa_computing(int cp_type, int cat, int chip_v, int gate_v, int nn_v, int year) {
    if (pa.n_computing >= MAX_COMPUTING) return -1;
    computing_t* c = &computers[pa.n_computing];
    c->compute_id = pa.n_computing;
    c->compute_type = cp_type;
    c->compute_category = cat;
    c->photonic_chip = chip_v;
    c->optical_gate = gate_v;
    c->optical_nn = nn_v;
    c->year = year;
    c->active = 1;
    pa.total_chip += chip_v;
    pa.n_computing++;
    print_str("[PA] Computing "); print_int(pa.n_computing - 1);
    print_str(" type="); print_int(cp_type);
    print_str(" cat="); print_int(cat);
    print_str(" chp="); print_int(chip_v);
    print_str(" gte="); print_int(gate_v);
    print_str(" nnc="); print_int(nn_v); print_str("\n");
    return pa.n_computing - 1;
}

int pa_sensing(int sn_type, int cat, int fiber_s, int spectral, int opt_img, int year) {
    if (pa.n_sensing >= MAX_SENSING) return -1;
    sensing_t* s = &sensings[pa.n_sensing];
    s->sense_id = pa.n_sensing;
    s->sense_type = sn_type;
    s->sense_category = cat;
    s->fiber_sense = fiber_s;
    s->spectral_sense = spectral;
    s->optical_img = opt_img;
    s->year = year;
    s->active = 1;
    pa.total_fiber_sense += fiber_s;
    pa.n_sensing++;
    print_str("[PA] Sensing "); print_int(pa.n_sensing - 1);
    print_str(" type="); print_int(sn_type);
    print_str(" cat="); print_int(cat);
    print_str(" fbs="); print_int(fiber_s);
    print_str(" spc="); print_int(spectral);
    print_str(" opi="); print_int(opt_img); print_str("\n");
    return pa.n_sensing - 1;
}

int pa_quantum(int q_type, int cat, int single_p, int qdot, int entgl, int year) {
    if (pa.n_quantum >= MAX_QUANTUM) return -1;
    quantum_p_t* q = &quantums[pa.n_quantum];
    q->quantum_id = pa.n_quantum;
    q->quantum_type = q_type;
    q->quantum_category = cat;
    q->single_photon = single_p;
    q->quantum_dot = qdot;
    q->photon_entgl = entgl;
    q->year = year;
    q->active = 1;
    pa.total_single += single_p;
    pa.n_quantum++;
    print_str("[PA] Quantum "); print_int(pa.n_quantum - 1);
    print_str(" type="); print_int(q_type);
    print_str(" cat="); print_int(cat);
    print_str(" sgp="); print_int(single_p);
    print_str(" qdt="); print_int(qdot);
    print_str(" ent="); print_int(entgl); print_str("\n");
    return pa.n_quantum - 1;
}

int pa_bio(int b_type, int cat, int fluor_v, int optogen, int photodyn, int year) {
    if (pa.n_bio >= MAX_BIO) return -1;
    bio_p_t* b = &bios[pa.n_bio];
    b->bio_id = pa.n_bio;
    b->bio_type = b_type;
    b->bio_category = cat;
    b->fluorescence = fluor_v;
    b->optogenetics = optogen;
    b->photodynamic = photodyn;
    b->year = year;
    b->active = 1;
    pa.total_fluor += fluor_v;
    pa.n_bio++;
    print_str("[PA] Bio "); print_int(pa.n_bio - 1);
    print_str(" type="); print_int(b_type);
    print_str(" cat="); print_int(cat);
    print_str(" flr="); print_int(fluor_v);
    print_str(" opt="); print_int(optogen);
    print_str(" pht="); print_int(photodyn); print_str("\n");
    return pa.n_bio - 1;
}

void pa_comm_report(void) {
    print_str("[PA] Comm report:\n");
    print_str("  Optical communication categories: "); print_int(pa.n_comm); print_str("\n");
    print_str("  Total fiber communication: "); print_int(pa.total_fiber); print_str("\n");
}

void pa_computing_report(void) {
    print_str("[PA] Computing report:\n");
    print_str("  Optical computing categories: "); print_int(pa.n_computing); print_str("\n");
    print_str("  Total photonic chips: "); print_int(pa.total_chip); print_str("\n");
}

void pa_bio_report(void) {
    print_str("[PA] Bio report:\n");
    print_str("  Optical sensing categories: "); print_int(pa.n_sensing); print_str("\n");
    print_str("  Total fiber sensing: "); print_int(pa.total_fiber_sense); print_str("\n");
    print_str("  Quantum photonics categories: "); print_int(pa.n_quantum); print_str("\n");
    print_str("  Total single photon sources: "); print_int(pa.total_single); print_str("\n");
    print_str("  Biophotonics categories: "); print_int(pa.n_bio); print_str("\n");
    print_str("  Total fluorescence imaging: "); print_int(pa.total_fluor); print_str("\n");
}

void pa_print_state(void) {
    print_str("[PA] Cm="); print_int(pa.n_comm);
    print_str(" Cp="); print_int(pa.n_computing);
    print_str(" Sn="); print_int(pa.n_sensing);
    print_str(" Qt="); print_int(pa.n_quantum);
    print_str(" Bi="); print_int(pa.n_bio);
    print_str("\n");
}

int main(void) {
    print_str("=== Photonics Admin Demo ===\n\n");
    pa_init();

    print_str("Optical communication...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int fbr = 60 + (i * 14);
        int frs = 42 + (i * 10);
        int opi = 25 + (i * 6);
        int year = 2020 + (i % 5);
        pa_comm(type, cat, fbr, frs, opi, year);
    }

    print_str("\nOptical computing...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int chp = 50 + (i * 12);
        int gte = 35 + (i * 8);
        int nnc = 22 + (i * 5);
        int year = 2021 + (i % 4);
        pa_computing(type, cat, chp, gte, nnc, year);
    }

    print_str("\nOptical sensing...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int fbs = 45 + (i * 11);
        int spc = 30 + (i * 7);
        int opi = 18 + (i * 4);
        int year = 2022 + (i % 3);
        pa_sensing(type, cat, fbs, spc, opi, year);
    }

    print_str("\nQuantum photonics...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int sgp = 38 + (i * 9);
        int qdt = 25 + (i * 6);
        int ent = 15 + (i * 3);
        int year = 2023 + (i % 2);
        pa_quantum(type, cat, sgp, qdt, ent, year);
    }

    print_str("\nBiophotonics...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int flr = 32 + (i * 8);
        int opt = 22 + (i * 5);
        int pht = 12 + (i * 3);
        int year = 2024;
        pa_bio(type, cat, flr, opt, pht, year);
    }

    print_str("\nComm report...\n");
    pa_comm_report();

    print_str("\nComputing report...\n");
    pa_computing_report();

    print_str("\nBio report...\n");
    pa_bio_report();

    print_str("\nFinal state...\n");
    pa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
