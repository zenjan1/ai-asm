/* brain_science_admin: Brain science administration system (v1.0)
 * Brain structure, brain imaging, neural networks, brain diseases, brain-computer interface
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

#define MAX_STRUCTURE    16
#define MAX_IMAGING      14
#define MAX_NEURAL_NET   12
#define MAX_DISEASE      10
#define MAX_BCI          10

typedef struct {
    int    struct_id;
    int    struct_type;
    int    struct_category;
    int    cortex;
    int    cerebellum;
    int    brainstem;
    int    year;
    int    active;
} structure_t;

typedef struct {
    int    imaging_id;
    int    imaging_type;
    int    imaging_category;
    int    fmri_scan;
    int    pet_scan;
    int    eeg_scan;
    int    year;
    int    active;
} imaging_t;

typedef struct {
    int    net_id;
    int    net_type;
    int    net_category;
    int    neuron_link;
    int    synapse_trans;
    int    plasticity;
    int    year;
    int    active;
} neural_net_t;

typedef struct {
    int    disease_id;
    int    disease_type;
    int    disease_category;
    int    alzheimer;
    int    parkinson;
    int    stroke;
    int    year;
    int    active;
} disease_t;

typedef struct {
    int    bci_id;
    int    bci_type;
    int    bci_category;
    int    invasive_bci;
    int    noninvasive_bci;
    int    hybrid_bci;
    int    year;
    int    active;
} bci_t;

typedef struct {
    int    n_structure;
    int    n_imaging;
    int    n_neural_net;
    int    n_disease;
    int    n_bci;
    int    total_cortex;
    int    total_fmri;
    int    total_neuron;
    int    total_alzheimer;
    int    total_invasive;
} bsa_state_t;

static structure_t structures[MAX_STRUCTURE];
static imaging_t imagings[MAX_IMAGING];
static neural_net_t neural_nets[MAX_NEURAL_NET];
static disease_t diseases[MAX_DISEASE];
static bci_t bci_list[MAX_BCI];
static bsa_state_t bsa;

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

int bsa_init(void) {
    if (initialized) return -1;
    bsa.n_structure = 0; bsa.n_imaging = 0; bsa.n_neural_net = 0;
    bsa.n_disease = 0; bsa.n_bci = 0;
    bsa.total_cortex = 0; bsa.total_fmri = 0;
    bsa.total_neuron = 0; bsa.total_alzheimer = 0;
    bsa.total_invasive = 0;
    for (int i = 0; i < MAX_STRUCTURE; i++) structures[i].active = 0;
    for (int i = 0; i < MAX_IMAGING; i++) imagings[i].active = 0;
    for (int i = 0; i < MAX_NEURAL_NET; i++) neural_nets[i].active = 0;
    for (int i = 0; i < MAX_DISEASE; i++) diseases[i].active = 0;
    for (int i = 0; i < MAX_BCI; i++) bci_list[i].active = 0;
    initialized = 1;
    print_str("[BSA] Brain science initialized\n");
    return 0;
}

int bsa_structure(int st_type, int cat, int crtx, int cereb, int bstm, int year) {
    if (bsa.n_structure >= MAX_STRUCTURE) return -1;
    structure_t* s = &structures[bsa.n_structure];
    s->struct_id = bsa.n_structure;
    s->struct_type = st_type;
    s->struct_category = cat;
    s->cortex = crtx;
    s->cerebellum = cereb;
    s->brainstem = bstm;
    s->year = year;
    s->active = 1;
    bsa.total_cortex += crtx;
    bsa.n_structure++;
    print_str("[BSA] Structure "); print_int(bsa.n_structure - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" crx="); print_int(crtx);
    print_str(" cer="); print_int(cereb);
    print_str(" bst="); print_int(bstm); print_str("\n");
    return bsa.n_structure - 1;
}

int bsa_imaging(int im_type, int cat, int fmri_v, int pet_v, int eeg_v, int year) {
    if (bsa.n_imaging >= MAX_IMAGING) return -1;
    imaging_t* im = &imagings[bsa.n_imaging];
    im->imaging_id = bsa.n_imaging;
    im->imaging_type = im_type;
    im->imaging_category = cat;
    im->fmri_scan = fmri_v;
    im->pet_scan = pet_v;
    im->eeg_scan = eeg_v;
    im->year = year;
    im->active = 1;
    bsa.total_fmri += fmri_v;
    bsa.n_imaging++;
    print_str("[BSA] Imaging "); print_int(bsa.n_imaging - 1);
    print_str(" type="); print_int(im_type);
    print_str(" cat="); print_int(cat);
    print_str(" fmr="); print_int(fmri_v);
    print_str(" pet="); print_int(pet_v);
    print_str(" eeg="); print_int(eeg_v); print_str("\n");
    return bsa.n_imaging - 1;
}

int bsa_neural_net(int nn_type, int cat, int neuron_l, int synapse_t, int plast, int year) {
    if (bsa.n_neural_net >= MAX_NEURAL_NET) return -1;
    neural_net_t* n = &neural_nets[bsa.n_neural_net];
    n->net_id = bsa.n_neural_net;
    n->net_type = nn_type;
    n->net_category = cat;
    n->neuron_link = neuron_l;
    n->synapse_trans = synapse_t;
    n->plasticity = plast;
    n->year = year;
    n->active = 1;
    bsa.total_neuron += neuron_l;
    bsa.n_neural_net++;
    print_str("[BSA] Neural net "); print_int(bsa.n_neural_net - 1);
    print_str(" type="); print_int(nn_type);
    print_str(" cat="); print_int(cat);
    print_str(" nrl="); print_int(neuron_l);
    print_str(" syn="); print_int(synapse_t);
    print_str(" pls="); print_int(plast); print_str("\n");
    return bsa.n_neural_net - 1;
}

int bsa_disease(int ds_type, int cat, int alz, int park, int strk, int year) {
    if (bsa.n_disease >= MAX_DISEASE) return -1;
    disease_t* d = &diseases[bsa.n_disease];
    d->disease_id = bsa.n_disease;
    d->disease_type = ds_type;
    d->disease_category = cat;
    d->alzheimer = alz;
    d->parkinson = park;
    d->stroke = strk;
    d->year = year;
    d->active = 1;
    bsa.total_alzheimer += alz;
    bsa.n_disease++;
    print_str("[BSA] Disease "); print_int(bsa.n_disease - 1);
    print_str(" type="); print_int(ds_type);
    print_str(" cat="); print_int(cat);
    print_str(" alz="); print_int(alz);
    print_str(" prk="); print_int(park);
    print_str(" stk="); print_int(strk); print_str("\n");
    return bsa.n_disease - 1;
}

int bsa_bci(int bc_type, int cat, int invas, int noninv, int hyb, int year) {
    if (bsa.n_bci >= MAX_BCI) return -1;
    bci_t* b = &bci_list[bsa.n_bci];
    b->bci_id = bsa.n_bci;
    b->bci_type = bc_type;
    b->bci_category = cat;
    b->invasive_bci = invas;
    b->noninvasive_bci = noninv;
    b->hybrid_bci = hyb;
    b->year = year;
    b->active = 1;
    bsa.total_invasive += invas;
    bsa.n_bci++;
    print_str("[BSA] BCI "); print_int(bsa.n_bci - 1);
    print_str(" type="); print_int(bc_type);
    print_str(" cat="); print_int(cat);
    print_str(" inv="); print_int(invas);
    print_str(" nni="); print_int(noninv);
    print_str(" hyb="); print_int(hyb); print_str("\n");
    return bsa.n_bci - 1;
}

void bsa_structure_report(void) {
    print_str("[BSA] Structure report:\n");
    print_str("  Brain structure categories: "); print_int(bsa.n_structure); print_str("\n");
    print_str("  Total cortex studies: "); print_int(bsa.total_cortex); print_str("\n");
}

void bsa_imaging_report(void) {
    print_str("[BSA] Imaging report:\n");
    print_str("  Brain imaging categories: "); print_int(bsa.n_imaging); print_str("\n");
    print_str("  Total fMRI scans: "); print_int(bsa.total_fmri); print_str("\n");
}

void bsa_bci_report(void) {
    print_str("[BSA] BCI report:\n");
    print_str("  Neural network categories: "); print_int(bsa.n_neural_net); print_str("\n");
    print_str("  Total neuron links: "); print_int(bsa.total_neuron); print_str("\n");
    print_str("  Brain disease categories: "); print_int(bsa.n_disease); print_str("\n");
    print_str("  Total Alzheimer research: "); print_int(bsa.total_alzheimer); print_str("\n");
    print_str("  BCI categories: "); print_int(bsa.n_bci); print_str("\n");
    print_str("  Total invasive BCI: "); print_int(bsa.total_invasive); print_str("\n");
}

void bsa_print_state(void) {
    print_str("[BSA] St="); print_int(bsa.n_structure);
    print_str(" Im="); print_int(bsa.n_imaging);
    print_str(" Nn="); print_int(bsa.n_neural_net);
    print_str(" Ds="); print_int(bsa.n_disease);
    print_str(" Bi="); print_int(bsa.n_bci);
    print_str("\n");
}

int main(void) {
    print_str("=== Brain Science Admin Demo ===\n\n");
    bsa_init();

    print_str("Brain structure...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int crx = 55 + (i * 13);
        int cer = 38 + (i * 9);
        int bst = 22 + (i * 5);
        int year = 2020 + (i % 5);
        bsa_structure(type, cat, crx, cer, bst, year);
    }

    print_str("\nBrain imaging...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int fmr = 48 + (i * 12);
        int pet = 35 + (i * 8);
        int eeg = 20 + (i * 5);
        int year = 2021 + (i % 4);
        bsa_imaging(type, cat, fmr, pet, eeg, year);
    }

    print_str("\nNeural networks...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int nrl = 42 + (i * 10);
        int syn = 28 + (i * 7);
        int pls = 18 + (i * 4);
        int year = 2022 + (i % 3);
        bsa_neural_net(type, cat, nrl, syn, pls, year);
    }

    print_str("\nBrain diseases...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int alz = 35 + (i * 8);
        int prk = 25 + (i * 6);
        int stk = 15 + (i * 3);
        int year = 2023 + (i % 2);
        bsa_disease(type, cat, alz, prk, stk, year);
    }

    print_str("\nBrain-computer interface...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int inv = 30 + (i * 7);
        int nni = 22 + (i * 5);
        int hyb = 12 + (i * 3);
        int year = 2024;
        bsa_bci(type, cat, inv, nni, hyb, year);
    }

    print_str("\nStructure report...\n");
    bsa_structure_report();

    print_str("\nImaging report...\n");
    bsa_imaging_report();

    print_str("\nBCI report...\n");
    bsa_bci_report();

    print_str("\nFinal state...\n");
    bsa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
