/* telemedicine: Telemedicine platform (v1.0)
 * Video consultation, remote monitoring, e-prescription, data sharing, quality control
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

#define MAX_DOCTORS    8
#define MAX_PATIENTS   8
#define MAX_APPTS      16
#define MAX_SESSIONS   8
#define MAX_PRESCRIP   16
#define MAX_MONITOR    8

#define APPT_SCHEDULED 0x01
#define APPT_ONGOING   0x02
#define APPT_COMPLETED 0x03
#define APPT_CANCELLED 0x04

typedef struct {
    int    id;
    int    specialty;
    int    availability;
    int    consult_count;
    double rating;
    int    active;
} doctor_t;

typedef struct {
    int    id;
    int    age;
    int    priority;
    int    visit_count;
    double satisfaction;
    int    active;
} patient_t;

typedef struct {
    int    doctor_id;
    int    patient_id;
    int    status;
    int    duration_min;
    int    timestamp;
    double quality;
    int    id;
    int    active;
} appointment_t;

typedef struct {
    int    doctor_id;
    int    patient_id;
    int    appt_id;
    double duration_min;
    double video_quality;
    double audio_quality;
    int    id;
    int    active;
} session_t;

typedef struct {
    int    doctor_id;
    int    patient_id;
    int    drug_count;
    double total_cost;
    int    filled;
    int    id;
    int    active;
} prescription_t;

typedef struct {
    int    patient_id;
    double heart_rate;
    double spo2;
    double blood_pressure;
    double temperature;
    int    alert;
    int    timestamp;
    int    id;
    int    active;
} monitor_t;

typedef struct {
    int    n_doctors;
    int    n_patients;
    int    n_appts;
    int    n_sessions;
    int    n_prescriptions;
    int    n_monitors;
    int    completed_count;
    int    alert_count;
    double avg_quality;
    double avg_satisfaction;
} tm_state_t;

static doctor_t doctors[MAX_DOCTORS];
static patient_t patients[MAX_PATIENTS];
static appointment_t appointments[MAX_APPTS];
static session_t sessions[MAX_SESSIONS];
static prescription_t prescriptions[MAX_PRESCRIP];
static monitor_t monitors[MAX_MONITOR];
static tm_state_t tm;

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

int tm_init(void) {
    if (initialized) return -1;
    tm.n_doctors = 0; tm.n_patients = 0; tm.n_appts = 0;
    tm.n_sessions = 0; tm.n_prescriptions = 0; tm.n_monitors = 0;
    tm.completed_count = 0; tm.alert_count = 0;
    tm.avg_quality = 0.0; tm.avg_satisfaction = 0.0;
    for (int i = 0; i < MAX_DOCTORS; i++) doctors[i].active = 0;
    for (int i = 0; i < MAX_PATIENTS; i++) patients[i].active = 0;
    for (int i = 0; i < MAX_APPTS; i++) appointments[i].active = 0;
    for (int i = 0; i < MAX_SESSIONS; i++) sessions[i].active = 0;
    for (int i = 0; i < MAX_PRESCRIP; i++) prescriptions[i].active = 0;
    for (int i = 0; i < MAX_MONITOR; i++) monitors[i].active = 0;
    initialized = 1;
    print_str("[TM] Telemedicine initialized\n");
    return 0;
}

int tm_add_doctor(int specialty) {
    if (tm.n_doctors >= MAX_DOCTORS) return -1;
    doctor_t* d = &doctors[tm.n_doctors];
    d->specialty = specialty;
    d->availability = 1;
    d->consult_count = 0;
    d->rating = 4.5;
    d->id = tm.n_doctors; d->active = 1;
    tm.n_doctors++;
    print_str("[TM] Doctor "); print_int(tm.n_doctors - 1);
    print_str(" specialty="); print_int(specialty);
    print_str(" rating="); print_int((int)(d->rating * 10)); print_str("/10\n");
    return tm.n_doctors - 1;
}

int tm_add_patient(int age, int priority) {
    if (tm.n_patients >= MAX_PATIENTS) return -1;
    patient_t* p = &patients[tm.n_patients];
    p->age = age; p->priority = priority;
    p->visit_count = 0; p->satisfaction = 4.0;
    p->id = tm.n_patients; p->active = 1;
    tm.n_patients++;
    print_str("[TM] Patient "); print_int(tm.n_patients - 1);
    print_str(" age="); print_int(age);
    print_str(" priority="); print_int(priority); print_str("\n");
    return tm.n_patients - 1;
}

int tm_schedule_appointment(int doctor_id, int patient_id) {
    if (doctor_id >= tm.n_doctors || patient_id >= tm.n_patients) return -1;
    if (tm.n_appts >= MAX_APPTS) return -1;
    appointment_t* a = &appointments[tm.n_appts];
    a->doctor_id = doctor_id; a->patient_id = patient_id;
    a->status = APPT_SCHEDULED; a->duration_min = 0;
    a->timestamp = tm.n_appts * 3600;
    a->quality = 0.0;
    a->id = tm.n_appts; a->active = 1;
    tm.n_appts++;
    patients[patient_id].visit_count++;
    print_str("[TM] Appointment "); print_int(tm.n_appts - 1);
    print_str(" Dr"); print_int(doctor_id);
    print_str(" <- Pt"); print_int(patient_id);
    print_str(" SCHEDULED\n");
    return tm.n_appts - 1;
}

void tm_start_consultation(int appt_id) {
    if (appt_id >= tm.n_appts) return;
    appointment_t* a = &appointments[appt_id];
    a->status = APPT_ONGOING;
    doctors[a->doctor_id].availability = 0;
    if (tm.n_sessions < MAX_SESSIONS) {
        session_t* s = &sessions[tm.n_sessions];
        s->doctor_id = a->doctor_id;
        s->patient_id = a->patient_id;
        s->appt_id = appt_id;
        s->duration_min = 20.0 + (double)(appt_id % 3) * 5.0;
        s->video_quality = 85.0 + (double)(appt_id % 4) * 3.0;
        s->audio_quality = 90.0 + (double)(appt_id % 3) * 2.0;
        s->id = tm.n_sessions; s->active = 1;
        tm.n_sessions++;
    }
    print_str("[TM] Consultation "); print_int(appt_id);
    print_str(" STARTED\n");
}

void tm_complete_consultation(int appt_id) {
    if (appt_id >= tm.n_appts) return;
    appointment_t* a = &appointments[appt_id];
    a->status = APPT_COMPLETED;
    a->duration_min = 20 + appt_id % 15;
    a->quality = 85.0 + (double)(appt_id % 4) * 3.0;
    doctors[a->doctor_id].availability = 1;
    doctors[a->doctor_id].consult_count++;
    tm.completed_count++;
    patients[a->patient_id].satisfaction = a->quality / 20.0;
    print_str("[TM] Consultation "); print_int(appt_id);
    print_str(" COMPLETED dur="); print_int(a->duration_min); print_str("min");
    print_str(" quality="); print_int((int)a->quality); print_str("%\n");
}

int tm_create_prescription(int doctor_id, int patient_id, int drug_count) {
    if (tm.n_prescriptions >= MAX_PRESCRIP) return -1;
    prescription_t* rx = &prescriptions[tm.n_prescriptions];
    rx->doctor_id = doctor_id; rx->patient_id = patient_id;
    rx->drug_count = drug_count;
    rx->total_cost = drug_count * 25.0;
    rx->filled = 1;
    rx->id = tm.n_prescriptions; rx->active = 1;
    tm.n_prescriptions++;
    print_str("[TM] Prescription "); print_int(tm.n_prescriptions - 1);
    print_str(" Dr"); print_int(doctor_id);
    print_str(" -> Pt"); print_int(patient_id);
    print_str(" drugs="); print_int(drug_count);
    print_str(" cost="); print_int((int)rx->total_cost); print_str("USD\n");
    return tm.n_prescriptions - 1;
}

int tm_add_monitoring(int patient_id, double hr, double spo2, double bp, double temp) {
    if (tm.n_monitors >= MAX_MONITOR) return -1;
    monitor_t* m = &monitors[tm.n_monitors];
    m->patient_id = patient_id;
    m->heart_rate = hr; m->spo2 = spo2;
    m->blood_pressure = bp; m->temperature = temp;
    m->alert = 0;
    if (hr > 100 || hr < 60) m->alert = 1;
    if (spo2 < 95) m->alert = 1;
    if (bp > 140) m->alert = 1;
    if (temp > 37.5) m->alert = 1;
    if (m->alert) tm.alert_count++;
    m->timestamp = tm.n_monitors * 300;
    m->id = tm.n_monitors; m->active = 1;
    tm.n_monitors++;
    print_str("[TM] Monitor Pt"); print_int(patient_id);
    print_str(" HR="); print_int((int)hr);
    print_str(" SpO2="); print_int((int)spo2);
    print_str(" BP="); print_int((int)bp);
    print_str(" alert="); print_int(m->alert); print_str("\n");
    return tm.n_monitors - 1;
}

void tm_data_sharing(int from_patient, int to_doctor) {
    print_str("[TM] Data sharing: Pt"); print_int(from_patient);
    print_str(" -> Dr"); print_int(to_doctor); print_str("\n");
    int records = 0;
    for (int i = 0; i < tm.n_monitors; i++) {
        if (monitors[i].patient_id == from_patient) records++;
    }
    for (int i = 0; i < tm.n_prescriptions; i++) {
        if (prescriptions[i].patient_id == from_patient) records++;
    }
    print_str("  Shared records: "); print_int(records); print_str("\n");
}

void tm_quality_control(void) {
    double total_quality = 0.0;
    int completed = 0;
    for (int i = 0; i < tm.n_appts; i++) {
        if (appointments[i].status == APPT_COMPLETED) {
            total_quality += appointments[i].quality;
            completed++;
        }
    }
    tm.avg_quality = total_quality / (completed + 1e-10);
    double total_sat = 0.0;
    for (int i = 0; i < tm.n_patients; i++) {
        total_sat += patients[i].satisfaction;
    }
    tm.avg_satisfaction = total_sat / (tm.n_patients + 1e-10);
    print_str("[TM] Quality control:\n");
    print_str("  Completed: "); print_int(completed); print_str("\n");
    print_str("  Avg quality: "); print_int((int)tm.avg_quality); print_str("%\n");
    print_str("  Avg satisfaction: "); print_int((int)(tm.avg_satisfaction * 10)); print_str("/10\n");
    print_str("  Alerts: "); print_int(tm.alert_count); print_str("\n");
}

void tm_follow_up_management(void) {
    print_str("[TM] Follow-up management:\n");
    for (int i = 0; i < tm.n_patients; i++) {
        if (patients[i].visit_count > 0) {
            print_str("  Pt"); print_int(i);
            print_str(" visits="); print_int(patients[i].visit_count);
            print_str(" next="); print_int(30 - patients[i].visit_count * 5);
            print_str("d\n");
        }
    }
}

void tm_print_state(void) {
    print_str("[TM] Dr="); print_int(tm.n_doctors);
    print_str(" Pt="); print_int(tm.n_patients);
    print_str(" Appt="); print_int(tm.n_appts);
    print_str(" Sess="); print_int(tm.n_sessions);
    print_str(" Rx="); print_int(tm.n_prescriptions);
    print_str(" Mon="); print_int(tm.n_monitors);
    print_str(" Done="); print_int(tm.completed_count); print_str("\n");
}

int main(void) {
    print_str("=== Telemedicine Platform Demo ===\n\n");
    tm_init();

    print_str("Adding doctors...\n");
    tm_add_doctor(1);
    tm_add_doctor(2);
    tm_add_doctor(3);
    tm_add_doctor(4);

    print_str("\nAdding patients...\n");
    tm_add_patient(45, 2);
    tm_add_patient(72, 3);
    tm_add_patient(28, 1);
    tm_add_patient(55, 2);
    tm_add_patient(38, 1);

    print_str("\nScheduling appointments...\n");
    tm_schedule_appointment(0, 0);
    tm_schedule_appointment(1, 1);
    tm_schedule_appointment(2, 2);
    tm_schedule_appointment(0, 3);
    tm_schedule_appointment(3, 4);
    tm_schedule_appointment(1, 0);

    print_str("\nStarting consultations...\n");
    tm_start_consultation(0);
    tm_start_consultation(1);
    tm_start_consultation(2);

    print_str("\nCompleting consultations...\n");
    tm_complete_consultation(0);
    tm_complete_consultation(1);
    tm_complete_consultation(2);
    tm_complete_consultation(3);

    print_str("\nE-prescriptions...\n");
    tm_create_prescription(0, 0, 2);
    tm_create_prescription(1, 1, 3);
    tm_create_prescription(2, 2, 1);
    tm_create_prescription(0, 3, 2);

    print_str("\nRemote monitoring...\n");
    tm_add_monitoring(0, 72.0, 98.0, 120.0, 36.8);
    tm_add_monitoring(1, 88.0, 94.0, 148.0, 37.2);
    tm_add_monitoring(2, 65.0, 99.0, 110.0, 36.5);
    tm_add_monitoring(3, 108.0, 93.0, 155.0, 37.8);
    tm_add_monitoring(0, 75.0, 97.0, 118.0, 36.7);

    print_str("\nData sharing...\n");
    tm_data_sharing(0, 0);
    tm_data_sharing(1, 1);
    tm_data_sharing(3, 0);

    print_str("\nFollow-up management...\n");
    tm_follow_up_management();

    print_str("\nQuality control...\n");
    tm_quality_control();

    tm_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
