/* healthcare_system: Healthcare management system (v1.0)
 * Patient management, appointments, diagnosis, pharmacy, billing
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

#define MAX_PATIENTS   16
#define MAX_DOCTORS    10
#define MAX_DEPARTMENTS 8
#define MAX_APPOINTMENTS 20
#define MAX_PRESCRIPTIONS 16
#define MAX_MEDICINES  12
#define MAX_BILLS      16

typedef struct {
    int    patient_id;
    int    age;
    int    gender;
    int    blood_type;
    int    visit_count;
    double total_paid;
    int    active;
} patient_t;

typedef struct {
    int    doctor_id;
    int    department_id;
    int    title;
    int    patients_seen;
    double total_billing;
    int    schedule_slot;
    int    active;
} doctor_t;

typedef struct {
    int    dept_id;
    int    type;
    int    n_doctors;
    int    n_patients;
    double revenue;
    int    active;
} department_t;

typedef struct {
    int    appt_id;
    int    patient_id;
    int    doctor_id;
    int    dept_id;
    int    day;
    int    slot;
    int    status;
    int    active;
} appointment_t;

typedef struct {
    int    rx_id;
    int    patient_id;
    int    doctor_id;
    int    diagnosis;
    int    n_medicines;
    double total_cost;
    int    filled;
    int    active;
} prescription_t;

typedef struct {
    int    medicine_id;
    int    type;
    double price;
    int    stock;
    int    dispensed;
    int    active;
} medicine_t;

typedef struct {
    int    bill_id;
    int    patient_id;
    int    appt_id;
    double consult_fee;
    double medicine_cost;
    double insurance;
    double total;
    int    paid;
    int    active;
} bill_t;

typedef struct {
    int    n_patients;
    int    n_doctors;
    int    n_depts;
    int    n_appts;
    int    n_prescriptions;
    int    n_medicines;
    int    n_bills;
    double total_revenue;
    double total_insurance;
    int    total_visits;
    int    completed_appts;
} hc_state_t;

static patient_t patients[MAX_PATIENTS];
static doctor_t doctors[MAX_DOCTORS];
static department_t departments[MAX_DEPARTMENTS];
static appointment_t appointments[MAX_APPOINTMENTS];
static prescription_t prescriptions[MAX_PRESCRIPTIONS];
static medicine_t medicines[MAX_MEDICINES];
static bill_t bills[MAX_BILLS];
static hc_state_t hc;

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

int hc_init(void) {
    if (initialized) return -1;
    hc.n_patients = 0; hc.n_doctors = 0; hc.n_depts = 0;
    hc.n_appts = 0; hc.n_prescriptions = 0;
    hc.n_medicines = 0; hc.n_bills = 0;
    hc.total_revenue = 0.0; hc.total_insurance = 0.0;
    hc.total_visits = 0; hc.completed_appts = 0;
    for (int i = 0; i < MAX_PATIENTS; i++) patients[i].active = 0;
    for (int i = 0; i < MAX_DOCTORS; i++) doctors[i].active = 0;
    for (int i = 0; i < MAX_DEPARTMENTS; i++) departments[i].active = 0;
    for (int i = 0; i < MAX_APPOINTMENTS; i++) appointments[i].active = 0;
    for (int i = 0; i < MAX_PRESCRIPTIONS; i++) prescriptions[i].active = 0;
    for (int i = 0; i < MAX_MEDICINES; i++) medicines[i].active = 0;
    for (int i = 0; i < MAX_BILLS; i++) bills[i].active = 0;
    initialized = 1;
    print_str("[HC] Healthcare system initialized\n");
    return 0;
}

int hc_add_department(int type) {
    if (hc.n_depts >= MAX_DEPARTMENTS) return -1;
    department_t* d = &departments[hc.n_depts];
    d->dept_id = hc.n_depts;
    d->type = type;
    d->n_doctors = 0;
    d->n_patients = 0;
    d->revenue = 0.0;
    d->active = 1;
    hc.n_depts++;
    print_str("[HC] Department "); print_int(hc.n_depts - 1);
    print_str(" type="); print_int(type); print_str("\n");
    return hc.n_depts - 1;
}

int hc_add_doctor(int dept_id, int title, int slot) {
    if (hc.n_doctors >= MAX_DOCTORS || dept_id >= hc.n_depts) return -1;
    doctor_t* d = &doctors[hc.n_doctors];
    d->doctor_id = hc.n_doctors;
    d->department_id = dept_id;
    d->title = title;
    d->patients_seen = 0;
    d->total_billing = 0.0;
    d->schedule_slot = slot;
    d->active = 1;
    departments[dept_id].n_doctors++;
    hc.n_doctors++;
    print_str("[HC] Doctor "); print_int(hc.n_doctors - 1);
    print_str(" dept="); print_int(dept_id);
    print_str(" title="); print_int(title);
    print_str(" slot="); print_int(slot); print_str("\n");
    return hc.n_doctors - 1;
}

int hc_register_patient(int age, int gender, int blood_type) {
    if (hc.n_patients >= MAX_PATIENTS) return -1;
    patient_t* p = &patients[hc.n_patients];
    p->patient_id = hc.n_patients;
    p->age = age;
    p->gender = gender;
    p->blood_type = blood_type;
    p->visit_count = 0;
    p->total_paid = 0.0;
    p->active = 1;
    hc.n_patients++;
    print_str("[HC] Patient "); print_int(hc.n_patients - 1);
    print_str(" age="); print_int(age);
    print_str(" blood="); print_int(blood_type); print_str("\n");
    return hc.n_patients - 1;
}

int hc_make_appointment(int patient_id, int doctor_id, int day, int slot) {
    if (hc.n_appts >= MAX_APPOINTMENTS) return -1;
    if (patient_id >= hc.n_patients || doctor_id >= hc.n_doctors) return -2;
    appointment_t* a = &appointments[hc.n_appts];
    a->appt_id = hc.n_appts;
    a->patient_id = patient_id;
    a->doctor_id = doctor_id;
    a->dept_id = doctors[doctor_id].department_id;
    a->day = day;
    a->slot = slot;
    a->status = 1;
    a->active = 1;
    hc.n_appts++;
    print_str("[HC] Appointment A"); print_int(hc.n_appts - 1);
    print_str(" P"); print_int(patient_id);
    print_str(" D"); print_int(doctor_id);
    print_str(" day="); print_int(day);
    print_str(" slot="); print_int(slot); print_str("\n");
    return hc.n_appts - 1;
}

int hc_complete_appointment(int appt_id) {
    if (appt_id >= hc.n_appts) return -1;
    appointment_t* a = &appointments[appt_id];
    a->status = 3;
    patients[a->patient_id].visit_count++;
    doctors[a->doctor_id].patients_seen++;
    departments[a->dept_id].n_patients++;
    hc.total_visits++;
    hc.completed_appts++;
    print_str("[HC] Appointment A"); print_int(appt_id);
    print_str(" completed P"); print_int(a->patient_id);
    print_str(" D"); print_int(a->doctor_id); print_str("\n");
    return 0;
}

int hc_add_medicine(int type, double price, int stock) {
    if (hc.n_medicines >= MAX_MEDICINES) return -1;
    medicine_t* m = &medicines[hc.n_medicines];
    m->medicine_id = hc.n_medicines;
    m->type = type;
    m->price = price;
    m->stock = stock;
    m->dispensed = 0;
    m->active = 1;
    hc.n_medicines++;
    print_str("[HC] Medicine M"); print_int(hc.n_medicines - 1);
    print_str(" type="); print_int(type);
    print_str(" price="); print_int((int)price);
    print_str(" stock="); print_int(stock); print_str("\n");
    return hc.n_medicines - 1;
}

int hc_write_prescription(int patient_id, int doctor_id, int diagnosis, int medicine_id, int qty) {
    if (hc.n_prescriptions >= MAX_PRESCRIPTIONS) return -1;
    if (patient_id >= hc.n_patients || doctor_id >= hc.n_doctors) return -2;
    if (medicine_id >= hc.n_medicines) return -3;
    medicine_t* med = &medicines[medicine_id];
    if (med->stock < qty) return -4;
    prescription_t* rx = &prescriptions[hc.n_prescriptions];
    rx->rx_id = hc.n_prescriptions;
    rx->patient_id = patient_id;
    rx->doctor_id = doctor_id;
    rx->diagnosis = diagnosis;
    rx->n_medicines = qty;
    rx->total_cost = med->price * qty;
    rx->filled = 0;
    rx->active = 1;
    hc.n_prescriptions++;
    print_str("[HC] Prescription RX"); print_int(hc.n_prescriptions - 1);
    print_str(" P"); print_int(patient_id);
    print_str(" D"); print_int(doctor_id);
    print_str(" diag="); print_int(diagnosis);
    print_str(" M"); print_int(medicine_id);
    print_str(" qty="); print_int(qty);
    print_str(" cost="); print_int((int)rx->total_cost); print_str("\n");
    return hc.n_prescriptions - 1;
}

int hc_dispense_prescription(int rx_id) {
    if (rx_id >= hc.n_prescriptions) return -1;
    prescription_t* rx = &prescriptions[rx_id];
    rx->filled = 1;
    print_str("[HC] RX"); print_int(rx_id);
    print_str(" dispensed P"); print_int(rx->patient_id);
    print_str(" cost="); print_int((int)rx->total_cost); print_str("\n");
    return 0;
}

int hc_generate_bill(int appt_id, double consult_fee, double insurance_pct) {
    if (hc.n_bills >= MAX_BILLS || appt_id >= hc.n_appts) return -1;
    appointment_t* a = &appointments[appt_id];
    double med_cost = 0.0;
    for (int i = 0; i < hc.n_prescriptions; i++) {
        if (prescriptions[i].patient_id == a->patient_id &&
            prescriptions[i].doctor_id == a->doctor_id &&
            prescriptions[i].filled) {
            med_cost += prescriptions[i].total_cost;
        }
    }
    bill_t* b = &bills[hc.n_bills];
    b->bill_id = hc.n_bills;
    b->patient_id = a->patient_id;
    b->appt_id = appt_id;
    b->consult_fee = consult_fee;
    b->medicine_cost = med_cost;
    b->insurance = (consult_fee + med_cost) * insurance_pct / 100.0;
    b->total = consult_fee + med_cost;
    b->paid = 0;
    b->active = 1;
    hc.n_bills++;
    print_str("[HC] Bill B"); print_int(hc.n_bills - 1);
    print_str(" P"); print_int(a->patient_id);
    print_str(" consult="); print_int((int)consult_fee);
    print_str(" med="); print_int((int)med_cost);
    print_str(" ins="); print_int((int)b->insurance);
    print_str(" total="); print_int((int)b->total); print_str("\n");
    return hc.n_bills - 1;
}

void hc_pay_bill(int bill_id) {
    if (bill_id >= hc.n_bills) return;
    bill_t* b = &bills[bill_id];
    b->paid = 1;
    double patient_pay = b->total - b->insurance;
    hc.total_revenue += b->total;
    hc.total_insurance += b->insurance;
    patients[b->patient_id].total_paid += patient_pay;
    doctors[appointments[b->appt_id].doctor_id].total_billing += b->total;
    departments[appointments[b->appt_id].dept_id].revenue += b->total;
    print_str("[HC] Bill B"); print_int(bill_id);
    print_str(" paid patient="); print_int((int)patient_pay);
    print_str(" insurance="); print_int((int)b->insurance); print_str("\n");
}

void hc_financial_report(void) {
    print_str("[HC] Financial report:\n");
    print_str("  Total patients: "); print_int(hc.n_patients); print_str("\n");
    print_str("  Total visits: "); print_int(hc.total_visits); print_str("\n");
    print_str("  Total revenue: "); print_int((int)hc.total_revenue); print_str("\n");
    print_str("  Insurance covered: "); print_int((int)hc.total_insurance); print_str("\n");
    print_str("  Patient out-of-pocket: ");
    print_int((int)(hc.total_revenue - hc.total_insurance)); print_str("\n");
    print_str("  Department revenue:\n");
    for (int i = 0; i < hc.n_depts; i++) {
        print_str("    Dept "); print_int(i);
        print_str(" doctors="); print_int(departments[i].n_doctors);
        print_str(" patients="); print_int(departments[i].n_patients);
        print_str(" rev="); print_int((int)departments[i].revenue); print_str("\n");
    }
}

void hc_print_state(void) {
    print_str("[HC] Patients="); print_int(hc.n_patients);
    print_str(" Doctors="); print_int(hc.n_doctors);
    print_str(" Depts="); print_int(hc.n_depts);
    print_str(" Appts="); print_int(hc.n_appts);
    print_str(" Rx="); print_int(hc.n_prescriptions);
    print_str(" Meds="); print_int(hc.n_medicines);
    print_str(" Bills="); print_int(hc.n_bills);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)hc.total_revenue);
    print_str(" Visits: "); print_int(hc.total_visits); print_str("\n");
}

int main(void) {
    print_str("=== Healthcare System Demo ===\n\n");
    hc_init();

    print_str("Adding departments...\n");
    hc_add_department(1);
    hc_add_department(2);
    hc_add_department(3);
    hc_add_department(4);
    hc_add_department(5);

    print_str("\nAdding doctors...\n");
    hc_add_doctor(0, 3, 1);
    hc_add_doctor(0, 2, 2);
    hc_add_doctor(1, 3, 1);
    hc_add_doctor(1, 1, 2);
    hc_add_doctor(2, 2, 1);
    hc_add_doctor(2, 3, 2);
    hc_add_doctor(3, 2, 1);
    hc_add_doctor(4, 1, 1);

    print_str("\nRegistering patients...\n");
    hc_register_patient(35, 1, 1);
    hc_register_patient(28, 2, 2);
    hc_register_patient(45, 1, 3);
    hc_register_patient(52, 2, 1);
    hc_register_patient(19, 1, 4);
    hc_register_patient(67, 2, 2);
    hc_register_patient(41, 1, 3);
    hc_register_patient(33, 2, 1);
    hc_register_patient(55, 1, 4);
    hc_register_patient(24, 2, 2);

    print_str("\nAdding medicines...\n");
    hc_add_medicine(1, 15.0, 100);
    hc_add_medicine(2, 25.0, 80);
    hc_add_medicine(3, 45.0, 50);
    hc_add_medicine(1, 12.0, 120);
    hc_add_medicine(4, 80.0, 30);
    hc_add_medicine(2, 30.0, 60);
    hc_add_medicine(5, 120.0, 20);

    print_str("\nMaking appointments...\n");
    hc_make_appointment(0, 0, 10, 1);
    hc_make_appointment(1, 2, 10, 2);
    hc_make_appointment(2, 4, 10, 1);
    hc_make_appointment(3, 1, 11, 1);
    hc_make_appointment(4, 3, 11, 2);
    hc_make_appointment(5, 5, 11, 1);
    hc_make_appointment(6, 6, 12, 1);
    hc_make_appointment(7, 7, 12, 2);
    hc_make_appointment(8, 0, 12, 1);
    hc_make_appointment(9, 2, 13, 1);

    print_str("\nCompleting appointments...\n");
    hc_complete_appointment(0);
    hc_complete_appointment(1);
    hc_complete_appointment(2);
    hc_complete_appointment(3);
    hc_complete_appointment(4);
    hc_complete_appointment(5);
    hc_complete_appointment(6);
    hc_complete_appointment(7);

    print_str("\nWriting prescriptions...\n");
    hc_write_prescription(0, 0, 1, 0, 3);
    hc_write_prescription(1, 2, 2, 1, 2);
    hc_write_prescription(2, 4, 1, 3, 5);
    hc_write_prescription(3, 1, 3, 2, 2);
    hc_write_prescription(4, 3, 2, 4, 1);
    hc_write_prescription(5, 5, 1, 5, 3);
    hc_write_prescription(6, 6, 4, 6, 2);

    print_str("\nDispensing prescriptions...\n");
    hc_dispense_prescription(0);
    hc_dispense_prescription(1);
    hc_dispense_prescription(2);
    hc_dispense_prescription(3);
    hc_dispense_prescription(4);
    hc_dispense_prescription(5);
    hc_dispense_prescription(6);

    print_str("\nGenerating bills...\n");
    hc_generate_bill(0, 50.0, 70);
    hc_generate_bill(1, 50.0, 80);
    hc_generate_bill(2, 60.0, 70);
    hc_generate_bill(3, 80.0, 60);
    hc_generate_bill(4, 50.0, 80);
    hc_generate_bill(5, 60.0, 70);
    hc_generate_bill(6, 70.0, 75);
    hc_generate_bill(7, 50.0, 80);

    print_str("\nProcessing payments...\n");
    hc_pay_bill(0);
    hc_pay_bill(1);
    hc_pay_bill(2);
    hc_pay_bill(3);
    hc_pay_bill(4);
    hc_pay_bill(5);
    hc_pay_bill(6);
    hc_pay_bill(7);

    print_str("\nFinancial report...\n");
    hc_financial_report();

    print_str("\nFinal state...\n");
    hc_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
