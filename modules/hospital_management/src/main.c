/* hospital_management: Hospital management system (v1.0)
 * Departments, doctors, patients, beds, medications, revenue
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

#define MAX_DEPARTMENTS  8
#define MAX_DOCTORS      12
#define MAX_PATIENTS     16
#define MAX_BEDS         20
#define MAX_MEDICATIONS  10
#define MAX_VISITS       18

typedef struct {
    int    dept_id;
    int    type;
    int    n_doctors;
    int    n_patients;
    int    n_beds;
    double revenue;
    int    active;
} department_t;

typedef struct {
    int    doctor_id;
    int    dept_id;
    int    rank;
    int    n_patients;
    int    n_visits;
    double fee_per_visit;
    double total_earned;
    int    active;
} doctor_t;

typedef struct {
    int    patient_id;
    int    age_group;
    int    dept_id;
    int    doctor_id;
    int    bed_id;
    int    status;
    double total_paid;
    int    n_visits;
    int    active;
} patient_t;

typedef struct {
    int    bed_id;
    int    dept_id;
    int    type;
    double daily_rate;
    int    occupied;
    int    patient_id;
    int    days_used;
    double revenue;
    int    active;
} bed_t;

typedef struct {
    int    med_id;
    int    type;
    double price;
    double cost;
    int    stock;
    int    n_dispensed;
    double revenue;
    int    active;
} medication_t;

typedef struct {
    int    visit_id;
    int    patient_id;
    int    doctor_id;
    int    dept_id;
    int    type;
    double fee;
    double med_cost;
    int    day;
    int    active;
} visit_t;

typedef struct {
    int    n_depts;
    int    n_doctors;
    int    n_patients;
    int    n_beds;
    int    n_meds;
    int    n_visits;
    double visit_revenue;
    double bed_revenue;
    double med_revenue;
    double total_revenue;
    double total_costs;
    int    occupied_beds;
} hm_state_t;

static department_t departments[MAX_DEPARTMENTS];
static doctor_t doctors[MAX_DOCTORS];
static patient_t patients[MAX_PATIENTS];
static bed_t beds[MAX_BEDS];
static medication_t medications[MAX_MEDICATIONS];
static visit_t visits[MAX_VISITS];
static hm_state_t hm;

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

int hm_init(void) {
    if (initialized) return -1;
    hm.n_depts = 0; hm.n_doctors = 0; hm.n_patients = 0;
    hm.n_beds = 0; hm.n_meds = 0; hm.n_visits = 0;
    hm.visit_revenue = 0.0; hm.bed_revenue = 0.0;
    hm.med_revenue = 0.0; hm.total_revenue = 0.0;
    hm.total_costs = 0.0; hm.occupied_beds = 0;
    for (int i = 0; i < MAX_DEPARTMENTS; i++) departments[i].active = 0;
    for (int i = 0; i < MAX_DOCTORS; i++) doctors[i].active = 0;
    for (int i = 0; i < MAX_PATIENTS; i++) patients[i].active = 0;
    for (int i = 0; i < MAX_BEDS; i++) beds[i].active = 0;
    for (int i = 0; i < MAX_MEDICATIONS; i++) medications[i].active = 0;
    for (int i = 0; i < MAX_VISITS; i++) visits[i].active = 0;
    initialized = 1;
    print_str("[HM] Hospital initialized\n");
    return 0;
}

int hm_add_department(int type) {
    if (hm.n_depts >= MAX_DEPARTMENTS) return -1;
    department_t* d = &departments[hm.n_depts];
    d->dept_id = hm.n_depts;
    d->type = type;
    d->n_doctors = 0;
    d->n_patients = 0;
    d->n_beds = 0;
    d->revenue = 0.0;
    d->active = 1;
    hm.n_depts++;
    print_str("[HM] Dept "); print_int(hm.n_depts - 1);
    print_str(" type="); print_int(type); print_str("\n");
    return hm.n_depts - 1;
}

int hm_add_doctor(int dept_id, int rank, double fee) {
    if (hm.n_doctors >= MAX_DOCTORS || dept_id >= hm.n_depts) return -1;
    doctor_t* d = &doctors[hm.n_doctors];
    d->doctor_id = hm.n_doctors;
    d->dept_id = dept_id;
    d->rank = rank;
    d->n_patients = 0;
    d->n_visits = 0;
    d->fee_per_visit = fee;
    d->total_earned = 0.0;
    d->active = 1;
    departments[dept_id].n_doctors++;
    hm.n_doctors++;
    print_str("[HM] Doctor "); print_int(hm.n_doctors - 1);
    print_str(" D"); print_int(dept_id);
    print_str(" rank="); print_int(rank);
    print_str(" fee="); print_int((int)fee); print_str("\n");
    return hm.n_doctors - 1;
}

int hm_add_bed(int dept_id, int type, double rate) {
    if (hm.n_beds >= MAX_BEDS || dept_id >= hm.n_depts) return -1;
    bed_t* b = &beds[hm.n_beds];
    b->bed_id = hm.n_beds;
    b->dept_id = dept_id;
    b->type = type;
    b->daily_rate = rate;
    b->occupied = 0;
    b->patient_id = -1;
    b->days_used = 0;
    b->revenue = 0.0;
    b->active = 1;
    departments[dept_id].n_beds++;
    hm.n_beds++;
    print_str("[HM] Bed "); print_int(hm.n_beds - 1);
    print_str(" D"); print_int(dept_id);
    print_str(" type="); print_int(type);
    print_str(" rate="); print_int((int)rate); print_str("/day\n");
    return hm.n_beds - 1;
}

int hm_add_medication(int type, double price, double cost, int stock) {
    if (hm.n_meds >= MAX_MEDICATIONS) return -1;
    medication_t* m = &medications[hm.n_meds];
    m->med_id = hm.n_meds;
    m->type = type;
    m->price = price;
    m->cost = cost;
    m->stock = stock;
    m->n_dispensed = 0;
    m->revenue = 0.0;
    m->active = 1;
    hm.n_meds++;
    print_str("[HM] Med "); print_int(hm.n_meds - 1);
    print_str(" type="); print_int(type);
    print_str(" price="); print_int((int)price);
    print_str(" stock="); print_int(stock); print_str("\n");
    return hm.n_meds - 1;
}

int hm_register_patient(int age_group, int dept_id) {
    if (hm.n_patients >= MAX_PATIENTS || dept_id >= hm.n_depts) return -1;
    patient_t* p = &patients[hm.n_patients];
    p->patient_id = hm.n_patients;
    p->age_group = age_group;
    p->dept_id = dept_id;
    p->doctor_id = -1;
    p->bed_id = -1;
    p->status = 1;
    p->total_paid = 0.0;
    p->n_visits = 0;
    p->active = 1;
    departments[dept_id].n_patients++;
    hm.n_patients++;
    print_str("[HM] Patient "); print_int(hm.n_patients - 1);
    print_str(" age="); print_int(age_group);
    print_str(" D"); print_int(dept_id); print_str("\n");
    return hm.n_patients - 1;
}

int hm_create_visit(int patient_id, int doctor_id, int type, int day) {
    if (hm.n_visits >= MAX_VISITS) return -1;
    if (patient_id >= hm.n_patients || doctor_id >= hm.n_doctors) return -2;
    doctor_t* doc = &doctors[doctor_id];
    patient_t* pat = &patients[patient_id];
    visit_t* v = &visits[hm.n_visits];
    v->visit_id = hm.n_visits;
    v->patient_id = patient_id;
    v->doctor_id = doctor_id;
    v->dept_id = doc->dept_id;
    v->type = type;
    double type_mult[3] = {1.0, 1.5, 2.0};
    v->fee = doc->fee_per_visit * type_mult[type < 3 ? type : 0];
    v->med_cost = 0.0;
    v->day = day;
    v->active = 1;
    doc->n_visits++;
    doc->n_patients++;
    doc->total_earned += v->fee * 0.6;
    pat->n_visits++;
    pat->total_paid += v->fee;
    pat->doctor_id = doctor_id;
    departments[doc->dept_id].revenue += v->fee;
    hm.visit_revenue += v->fee;
    hm.total_revenue += v->fee;
    hm.n_visits++;
    print_str("[HM] Visit "); print_int(hm.n_visits - 1);
    print_str(" P"); print_int(patient_id);
    print_str(" Dr"); print_int(doctor_id);
    print_str(" type="); print_int(type);
    print_str(" fee="); print_int((int)v->fee);
    print_str(" day="); print_int(day); print_str("\n");
    return hm.n_visits - 1;
}

int hm_prescribe_med(int visit_id, int med_id, int qty) {
    if (visit_id >= hm.n_visits || med_id >= hm.n_meds) return -1;
    medication_t* m = &medications[med_id];
    if (m->stock < qty) return -2;
    m->stock -= qty;
    m->n_dispensed += qty;
    double cost = m->price * qty;
    m->revenue += cost;
    visit_t* v = &visits[visit_id];
    v->med_cost += cost;
    patients[v->patient_id].total_paid += cost;
    departments[v->dept_id].revenue += cost;
    hm.med_revenue += cost;
    hm.total_revenue += cost;
    hm.total_costs += m->cost * qty;
    print_str("[HM] Prescribed M"); print_int(med_id);
    print_str(" qty="); print_int(qty);
    print_str(" cost="); print_int((int)cost);
    print_str(" V"); print_int(visit_id); print_str("\n");
    return 0;
}

int hm_admit_patient(int patient_id, int bed_id) {
    if (patient_id >= hm.n_patients || bed_id >= hm.n_beds) return -1;
    bed_t* b = &beds[bed_id];
    if (b->occupied) return -2;
    b->occupied = 1;
    b->patient_id = patient_id;
    patients[patient_id].bed_id = bed_id;
    patients[patient_id].status = 2;
    hm.occupied_beds++;
    print_str("[HM] Admitted P"); print_int(patient_id);
    print_str(" to Bed "); print_int(bed_id); print_str("\n");
    return 0;
}

int hm_discharge_patient(int patient_id, int days) {
    if (patient_id >= hm.n_patients) return -1;
    patient_t* p = &patients[patient_id];
    if (p->bed_id < 0) return -2;
    bed_t* b = &beds[p->bed_id];
    double bed_fee = b->daily_rate * days;
    b->revenue += bed_fee;
    b->days_used += days;
    b->occupied = 0;
    b->patient_id = -1;
    p->bed_id = -1;
    p->status = 3;
    p->total_paid += bed_fee;
    departments[b->dept_id].revenue += bed_fee;
    hm.bed_revenue += bed_fee;
    hm.total_revenue += bed_fee;
    hm.occupied_beds--;
    print_str("[HM] Discharged P"); print_int(patient_id);
    print_str(" days="); print_int(days);
    print_str(" bed_fee="); print_int((int)bed_fee); print_str("\n");
    return 0;
}

void hm_doctor_report(void) {
    print_str("[HM] Doctor report:\n");
    for (int i = 0; i < hm.n_doctors; i++) {
        doctor_t* d = &doctors[i];
        print_str("  Dr"); print_int(i);
        print_str(" D"); print_int(d->dept_id);
        print_str(" rank="); print_int(d->rank);
        print_str(" visits="); print_int(d->n_visits);
        print_str(" earned="); print_int((int)d->total_earned); print_str("\n");
    }
}

void hm_bed_report(void) {
    print_str("[HM] Bed report:\n");
    print_str("  Total beds: "); print_int(hm.n_beds); print_str("\n");
    print_str("  Occupied: "); print_int(hm.occupied_beds); print_str("\n");
    print_str("  Available: "); print_int(hm.n_beds - hm.occupied_beds); print_str("\n");
    for (int i = 0; i < hm.n_beds; i++) {
        bed_t* b = &beds[i];
        print_str("  B"); print_int(i);
        print_str(" D"); print_int(b->dept_id);
        print_str(" type="); print_int(b->type);
        print_str(" days="); print_int(b->days_used);
        print_str(" rev="); print_int((int)b->revenue);
        if (b->occupied) print_str(" [OCCUPIED]");
        print_str("\n");
    }
}

void hm_financial_report(void) {
    print_str("[HM] Financial report:\n");
    print_str("  Visit revenue: "); print_int((int)hm.visit_revenue); print_str("\n");
    print_str("  Bed revenue: "); print_int((int)hm.bed_revenue); print_str("\n");
    print_str("  Med revenue: "); print_int((int)hm.med_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)hm.total_revenue); print_str("\n");
    print_str("  Total costs: "); print_int((int)hm.total_costs); print_str("\n");
    double profit = hm.total_revenue - hm.total_costs;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    print_str("  Patients: "); print_int(hm.n_patients); print_str("\n");
    print_str("  Visits: "); print_int(hm.n_visits); print_str("\n");
}

void hm_print_state(void) {
    print_str("[HM] Depts="); print_int(hm.n_depts);
    print_str(" Docs="); print_int(hm.n_doctors);
    print_str(" Pats="); print_int(hm.n_patients);
    print_str(" Beds="); print_int(hm.n_beds);
    print_str(" Meds="); print_int(hm.n_meds);
    print_str(" Visits="); print_int(hm.n_visits);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)hm.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Hospital Management Demo ===\n\n");
    hm_init();

    print_str("Adding departments...\n");
    hm_add_department(1);
    hm_add_department(2);
    hm_add_department(3);
    hm_add_department(4);
    hm_add_department(5);
    hm_add_department(6);
    hm_add_department(1);
    hm_add_department(2);

    print_str("\nAdding doctors...\n");
    hm_add_doctor(0, 3, 120.0);
    hm_add_doctor(0, 2, 90.0);
    hm_add_doctor(1, 3, 130.0);
    hm_add_doctor(1, 1, 75.0);
    hm_add_doctor(2, 2, 100.0);
    hm_add_doctor(2, 3, 140.0);
    hm_add_doctor(3, 1, 80.0);
    hm_add_doctor(3, 2, 95.0);
    hm_add_doctor(4, 2, 110.0);
    hm_add_doctor(4, 1, 70.0);
    hm_add_doctor(5, 3, 150.0);
    hm_add_doctor(5, 2, 100.0);

    print_str("\nAdding beds...\n");
    hm_add_bed(0, 1, 80.0);
    hm_add_bed(0, 1, 80.0);
    hm_add_bed(0, 2, 150.0);
    hm_add_bed(1, 1, 90.0);
    hm_add_bed(1, 2, 160.0);
    hm_add_bed(2, 1, 85.0);
    hm_add_bed(2, 1, 85.0);
    hm_add_bed(2, 2, 170.0);
    hm_add_bed(3, 1, 75.0);
    hm_add_bed(3, 2, 140.0);
    hm_add_bed(4, 1, 95.0);
    hm_add_bed(4, 1, 95.0);
    hm_add_bed(5, 2, 200.0);
    hm_add_bed(5, 1, 100.0);
    hm_add_bed(6, 1, 70.0);
    hm_add_bed(6, 1, 70.0);
    hm_add_bed(7, 1, 80.0);
    hm_add_bed(7, 2, 155.0);
    hm_add_bed(0, 2, 160.0);
    hm_add_bed(1, 1, 85.0);

    print_str("\nAdding medications...\n");
    hm_add_medication(1, 15.0, 4.0, 200);
    hm_add_medication(1, 25.0, 8.0, 150);
    hm_add_medication(2, 35.0, 12.0, 100);
    hm_add_medication(2, 50.0, 18.0, 80);
    hm_add_medication(3, 12.0, 3.0, 300);
    hm_add_medication(3, 20.0, 6.0, 250);
    hm_add_medication(4, 45.0, 15.0, 60);
    hm_add_medication(4, 60.0, 22.0, 40);
    hm_add_medication(1, 18.0, 5.0, 180);
    hm_add_medication(2, 40.0, 14.0, 90);

    print_str("\nRegistering patients...\n");
    for (int i = 0; i < 16; i++) {
        hm_register_patient(i % 4, i % 8);
    }

    print_str("\nCreating visits...\n");
    hm_create_visit(0, 0, 1, 10);
    hm_create_visit(1, 1, 0, 10);
    hm_create_visit(2, 2, 2, 11);
    hm_create_visit(3, 3, 1, 11);
    hm_create_visit(4, 4, 0, 12);
    hm_create_visit(5, 5, 2, 12);
    hm_create_visit(6, 6, 1, 13);
    hm_create_visit(7, 7, 0, 13);
    hm_create_visit(8, 8, 2, 14);
    hm_create_visit(9, 9, 1, 14);
    hm_create_visit(10, 10, 0, 15);
    hm_create_visit(11, 11, 2, 15);
    hm_create_visit(12, 0, 1, 16);
    hm_create_visit(13, 2, 0, 16);
    hm_create_visit(14, 4, 2, 17);
    hm_create_visit(15, 6, 1, 17);
    hm_create_visit(0, 8, 0, 18);
    hm_create_visit(1, 10, 2, 18);

    print_str("\nPrescribing medications...\n");
    hm_prescribe_med(0, 0, 2);
    hm_prescribe_med(1, 1, 1);
    hm_prescribe_med(2, 2, 3);
    hm_prescribe_med(3, 4, 2);
    hm_prescribe_med(4, 5, 1);
    hm_prescribe_med(5, 6, 2);
    hm_prescribe_med(6, 3, 1);
    hm_prescribe_med(7, 7, 1);
    hm_prescribe_med(8, 8, 2);
    hm_prescribe_med(9, 9, 1);
    hm_prescribe_med(10, 0, 3);
    hm_prescribe_med(11, 1, 2);
    hm_prescribe_med(12, 4, 1);
    hm_prescribe_med(13, 2, 2);
    hm_prescribe_med(14, 5, 1);
    hm_prescribe_med(15, 3, 2);

    print_str("\nAdmitting patients...\n");
    hm_admit_patient(2, 2);
    hm_admit_patient(5, 4);
    hm_admit_patient(8, 7);
    hm_admit_patient(11, 12);
    hm_admit_patient(14, 0);

    print_str("\nDischarging patients...\n");
    hm_discharge_patient(2, 3);
    hm_discharge_patient(5, 5);
    hm_discharge_patient(8, 2);
    hm_discharge_patient(11, 7);
    hm_discharge_patient(14, 4);

    print_str("\nDoctor report...\n");
    hm_doctor_report();

    print_str("\nBed report...\n");
    hm_bed_report();

    print_str("\nFinancial report...\n");
    hm_financial_report();

    print_str("\nFinal state...\n");
    hm_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
