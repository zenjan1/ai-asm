/* digital_health: Digital health management system (v1.0)
 * Health records, vital signs, drug management, risk assessment, health reports
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

#define MAX_PATIENTS   8
#define MAX_VITALS     8
#define MAX_MEDS       16
#define MAX_ALLERGIES  8
#define MAX_RECORDS    8

typedef struct {
    double heart_rate;
    double blood_pressure_sys;
    double blood_pressure_dia;
    double spo2;
    double temperature;
    double glucose;
    double bmi;
    int    timestamp;
    int    patient_id;
    int    active;
} vitals_t;

typedef struct {
    char   name[32];
    double dosage;
    double frequency;
    double interactions;
    int    drug_id;
    int    patient_id;
    int    active;
} medication_t;

typedef struct {
    int    patient_id;
    int    allergy_type;
    int    severity;
    int    active;
} allergy_t;

typedef struct {
    int    id;
    int    age;
    int    gender;
    double height;
    double weight;
    double bmi;
    double risk_score;
    int    visit_count;
    int    active;
} patient_t;

typedef struct {
    int    n_patients;
    int    n_vitals;
    int    n_meds;
    int    n_allergies;
    int    n_records;
    double avg_risk;
    int    alert_count;
    int    total_visits;
} dh_state_t;

static patient_t patients[MAX_PATIENTS];
static vitals_t vitals[MAX_VITALS];
static medication_t meds[MAX_MEDS];
static allergy_t allergies[MAX_ALLERGIES];
static dh_state_t dh;

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
static double dabs(double x) { return x < 0 ? -x : x; }

int dh_init(void) {
    if (initialized) return -1;
    dh.n_patients = 0; dh.n_vitals = 0;
    dh.n_meds = 0; dh.n_allergies = 0; dh.n_records = 0;
    dh.avg_risk = 0.0; dh.alert_count = 0; dh.total_visits = 0;
    for (int i = 0; i < MAX_PATIENTS; i++) patients[i].active = 0;
    for (int i = 0; i < MAX_VITALS; i++) vitals[i].active = 0;
    for (int i = 0; i < MAX_MEDS; i++) meds[i].active = 0;
    for (int i = 0; i < MAX_ALLERGIES; i++) allergies[i].active = 0;
    initialized = 1;
    print_str("[DH] Digital health initialized\n");
    return 0;
}

int dh_add_patient(int age, int gender, double height, double weight) {
    if (dh.n_patients >= MAX_PATIENTS) return -1;
    patient_t* p = &patients[dh.n_patients];
    p->age = age; p->gender = gender;
    p->height = height; p->weight = weight;
    p->bmi = weight / ((height / 100.0) * (height / 100.0));
    p->risk_score = 10.0 + (age > 60 ? 20.0 : 0.0) + (p->bmi > 30.0 ? 15.0 : 0.0);
    p->visit_count = 0; p->id = dh.n_patients; p->active = 1;
    dh.n_patients++;
    print_str("[DH] Patient "); print_int(dh.n_patients - 1);
    print_str(" age="); print_int(age);
    print_str(" BMI="); print_int((int)(p->bmi * 10));
    print_str(" risk="); print_int((int)p->risk_score); print_str("\n");
    return dh.n_patients - 1;
}

int dh_add_vitals(int patient_id, double hr, double bp_sys, double bp_dia,
                  double spo2, double temp, double glucose) {
    if (patient_id >= dh.n_patients) return -1;
    if (dh.n_vitals >= MAX_VITALS) return -1;
    vitals_t* v = &vitals[dh.n_vitals];
    v->heart_rate = hr; v->blood_pressure_sys = bp_sys;
    v->blood_pressure_dia = bp_dia; v->spo2 = spo2;
    v->temperature = temp; v->glucose = glucose;
    v->patient_id = patient_id;
    v->timestamp = dh.n_vitals * 3600;
    v->active = 1;
    int alerts = 0;
    if (hr < 60 || hr > 100) { alerts++; dh.alert_count++; }
    if (bp_sys > 140 || bp_dia > 90) { alerts++; dh.alert_count++; }
    if (spo2 < 95) { alerts++; dh.alert_count++; }
    if (temp > 37.5 || temp < 36.0) { alerts++; dh.alert_count++; }
    if (glucose > 7.0 || glucose < 3.9) { alerts++; dh.alert_count++; }
    dh.n_vitals++;
    patients[patient_id].visit_count++;
    dh.total_visits++;
    print_str("[DH] Vitals patient "); print_int(patient_id);
    print_str(" HR="); print_int((int)hr);
    print_str(" BP="); print_int((int)bp_sys); print_str("/"); print_int((int)bp_dia);
    print_str(" SpO2="); print_int((int)spo2);
    print_str(" T="); print_int((int)(temp * 10));
    print_str(" alerts="); print_int(alerts); print_str("\n");
    return dh.n_vitals - 1;
}

int dh_add_medication(int patient_id, double dosage, double frequency) {
    if (patient_id >= dh.n_patients) return -1;
    if (dh.n_meds >= MAX_MEDS) return -1;
    medication_t* m = &meds[dh.n_meds];
    m->drug_id = dh.n_meds; m->patient_id = patient_id;
    m->dosage = dosage; m->frequency = frequency;
    m->interactions = 0.0;
    for (int i = 0; i < dh.n_meds; i++) {
        if (meds[i].patient_id == patient_id) {
            m->interactions += 0.1;
        }
    }
    m->active = 1;
    dh.n_meds++;
    print_str("[DH] Medication patient "); print_int(patient_id);
    print_str(" dose="); print_int((int)dosage); print_str("mg");
    print_str(" freq="); print_int((int)frequency); print_str("/day");
    print_str(" interactions="); print_int((int)(m->interactions * 100)); print_str("%\n");
    return dh.n_meds - 1;
}

int dh_add_allergy(int patient_id, int allergy_type, int severity) {
    if (patient_id >= dh.n_patients) return -1;
    if (dh.n_allergies >= MAX_ALLERGIES) return -1;
    allergy_t* a = &allergies[dh.n_allergies];
    a->patient_id = patient_id;
    a->allergy_type = allergy_type;
    a->severity = severity;
    a->active = 1;
    dh.n_allergies++;
    patients[patient_id].risk_score += severity * 5.0;
    print_str("[DH] Allergy patient "); print_int(patient_id);
    print_str(" type="); print_int(allergy_type);
    print_str(" severity="); print_int(severity); print_str("/5\n");
    return dh.n_allergies - 1;
}

void dh_risk_assessment(int patient_id) {
    if (patient_id >= dh.n_patients) return;
    patient_t* p = &patients[patient_id];
    double risk = p->risk_score;
    risk += (p->age > 70 ? 10.0 : 0.0);
    risk += (p->bmi > 35.0 ? 10.0 : (p->bmi > 30.0 ? 5.0 : 0.0));
    for (int i = 0; i < dh.n_vitals; i++) {
        if (vitals[i].patient_id == patient_id) {
            if (vitals[i].heart_rate > 100) risk += 5.0;
            if (vitals[i].blood_pressure_sys > 140) risk += 8.0;
            if (vitals[i].spo2 < 95) risk += 10.0;
            if (vitals[i].glucose > 7.0) risk += 7.0;
        }
    }
    p->risk_score = risk;
    print_str("[DH] Risk assessment patient "); print_int(patient_id);
    print_str(": score="); print_int((int)risk);
    if (risk > 50) print_str(" HIGH\n");
    else if (risk > 30) print_str(" MEDIUM\n");
    else print_str(" LOW\n");
}

void dh_health_report(int patient_id) {
    if (patient_id >= dh.n_patients) return;
    patient_t* p = &patients[patient_id];
    print_str("[DH] Health report patient "); print_int(patient_id); print_str(":\n");
    print_str("  Age="); print_int(p->age);
    print_str(" BMI="); print_int((int)(p->bmi * 10));
    print_str(" Visits="); print_int(p->visit_count); print_str("\n");
    print_str("  Risk="); print_int((int)p->risk_score); print_str("/100\n");
    int med_count = 0;
    for (int i = 0; i < dh.n_meds; i++) {
        if (meds[i].patient_id == patient_id) med_count++;
    }
    print_str("  Medications: "); print_int(med_count); print_str("\n");
    int allergy_count = 0;
    for (int i = 0; i < dh.n_allergies; i++) {
        if (allergies[i].patient_id == patient_id) allergy_count++;
    }
    print_str("  Allergies: "); print_int(allergy_count); print_str("\n");
}

void dh_drug_interaction_check(void) {
    print_str("[DH] Drug interaction check:\n");
    int interactions = 0;
    for (int i = 0; i < dh.n_meds; i++) {
        if (meds[i].interactions > 0.0) {
            print_str("  Med "); print_int(meds[i].drug_id);
            print_str(" patient "); print_int(meds[i].patient_id);
            print_str(" interactions="); print_int((int)(meds[i].interactions * 100)); print_str("%\n");
            interactions++;
        }
    }
    if (interactions == 0) print_str("  No interactions found\n");
    else print_str("  Total: "); print_int(interactions); print_str(" interactions\n");
}

void dh_anomaly_detection(void) {
    print_str("[DH] Anomaly detection:\n");
    int anomalies = 0;
    for (int i = 0; i < dh.n_vitals; i++) {
        int is_anomaly = 0;
        if (vitals[i].heart_rate < 50 || vitals[i].heart_rate > 120) is_anomaly = 1;
        if (vitals[i].blood_pressure_sys > 160) is_anomaly = 1;
        if (vitals[i].spo2 < 90) is_anomaly = 1;
        if (vitals[i].glucose > 11.0 || vitals[i].glucose < 3.0) is_anomaly = 1;
        if (is_anomaly) {
            print_str("  Vitals "); print_int(i);
            print_str(" patient "); print_int(vitals[i].patient_id);
            print_str(" ANOMALY\n");
            anomalies++;
        }
    }
    print_str("  Total anomalies: "); print_int(anomalies); print_str("\n");
}

void dh_print_state(void) {
    double sum_risk = 0.0;
    for (int i = 0; i < dh.n_patients; i++) sum_risk += patients[i].risk_score;
    dh.avg_risk = sum_risk / (dh.n_patients + 1e-10);
    print_str("[DH] Patients="); print_int(dh.n_patients);
    print_str(" Vitals="); print_int(dh.n_vitals);
    print_str(" Meds="); print_int(dh.n_meds);
    print_str(" Allergies="); print_int(dh.n_allergies);
    print_str(" Risk="); print_int((int)dh.avg_risk);
    print_str(" Alerts="); print_int(dh.alert_count); print_str("\n");
}

int main(void) {
    print_str("=== Digital Health System Demo ===\n\n");
    dh_init();

    print_str("Adding patients...\n");
    dh_add_patient(45, 1, 175.0, 80.0);
    dh_add_patient(72, 1, 168.0, 75.0);
    dh_add_patient(28, 2, 165.0, 60.0);
    dh_add_patient(55, 1, 180.0, 95.0);

    print_str("\nAdding vitals...\n");
    dh_add_vitals(0, 72.0, 120.0, 80.0, 98.0, 36.8, 5.5);
    dh_add_vitals(1, 88.0, 145.0, 92.0, 96.0, 37.2, 7.5);
    dh_add_vitals(2, 65.0, 110.0, 70.0, 99.0, 36.5, 4.8);
    dh_add_vitals(3, 105.0, 155.0, 95.0, 94.0, 37.8, 8.2);
    dh_add_vitals(0, 75.0, 118.0, 78.0, 97.0, 36.7, 5.2);
    dh_add_vitals(1, 92.0, 148.0, 94.0, 95.0, 37.0, 7.8);

    print_str("\nAdding medications...\n");
    dh_add_medication(0, 500.0, 2.0);
    dh_add_medication(1, 250.0, 3.0);
    dh_add_medication(1, 100.0, 1.0);
    dh_add_medication(2, 200.0, 1.0);
    dh_add_medication(3, 750.0, 2.0);
    dh_add_medication(3, 150.0, 2.0);

    print_str("\nAdding allergies...\n");
    dh_add_allergy(0, 1, 3);
    dh_add_allergy(1, 2, 4);
    dh_add_allergy(1, 3, 2);
    dh_add_allergy(3, 1, 5);

    print_str("\nRisk assessments...\n");
    for (int i = 0; i < dh.n_patients; i++) dh_risk_assessment(i);

    print_str("\nDrug interaction check...\n");
    dh_drug_interaction_check();

    print_str("\nAnomaly detection...\n");
    dh_anomaly_detection();

    print_str("\nHealth reports...\n");
    for (int i = 0; i < dh.n_patients; i++) dh_health_report(i);

    dh_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
