/* smart_building: Smart building system (v1.0)
 * Building automation, energy management, security, parking, environment monitoring
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

#define MAX_FLOORS      8
#define MAX_ZONES       16
#define MAX_DEVICES     20
#define MAX_SENSORS     16
#define MAX_ALARMS      8
#define MAX_PARKING     12
#define MAX_ENERGY      12

typedef struct {
    int    floor_id;
    int    n_zones;
    int    n_devices;
    int    occupancy;
    int    active;
} floor_t;

typedef struct {
    int    zone_id;
    int    floor_id;
    int    type;
    double target_temp;
    double current_temp;
    int    occupancy;
    int    active;
} zone_t;

typedef struct {
    int    device_id;
    int    zone_id;
    int    type;
    int    state;
    double power_watts;
    int    active;
} device_t;

typedef struct {
    int    sensor_id;
    int    zone_id;
    int    type;
    double value;
    double threshold;
    int    status;
    int    active;
} sensor_t;

typedef struct {
    int    alarm_id;
    int    type;
    int    zone_id;
    int    severity;
    int    hour;
    int    acknowledged;
    int    active;
} alarm_t;

typedef struct {
    int    spot_id;
    int    floor_id;
    int    type;
    int    occupied;
    int    vehicle_id;
    int    active;
} parking_t;

typedef struct {
    int    reading_id;
    int    hour;
    double consumption_kwh;
    double cost;
    int    peak;
    int    active;
} energy_reading_t;

typedef struct {
    int    n_floors;
    int    n_zones;
    int    n_devices;
    int    n_sensors;
    int    n_alarms;
    int    n_parking;
    int    n_energy;
    double total_power_watts;
    double total_energy_kwh;
    double total_cost;
    int    occupied_spots;
    int    total_spots;
    int    alarms_active;
} sb_state_t;

static floor_t floors[MAX_FLOORS];
static zone_t zones[MAX_ZONES];
static device_t devices[MAX_DEVICES];
static sensor_t sensors[MAX_SENSORS];
static alarm_t alarms[MAX_ALARMS];
static parking_t parking[MAX_PARKING];
static energy_reading_t energy[MAX_ENERGY];
static sb_state_t sb;

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

int sb_init(void) {
    if (initialized) return -1;
    sb.n_floors = 0; sb.n_zones = 0; sb.n_devices = 0;
    sb.n_sensors = 0; sb.n_alarms = 0; sb.n_parking = 0; sb.n_energy = 0;
    sb.total_power_watts = 0.0; sb.total_energy_kwh = 0.0; sb.total_cost = 0.0;
    sb.occupied_spots = 0; sb.total_spots = 0; sb.alarms_active = 0;
    for (int i = 0; i < MAX_FLOORS; i++) floors[i].active = 0;
    for (int i = 0; i < MAX_ZONES; i++) zones[i].active = 0;
    for (int i = 0; i < MAX_DEVICES; i++) devices[i].active = 0;
    for (int i = 0; i < MAX_SENSORS; i++) sensors[i].active = 0;
    for (int i = 0; i < MAX_ALARMS; i++) alarms[i].active = 0;
    for (int i = 0; i < MAX_PARKING; i++) parking[i].active = 0;
    for (int i = 0; i < MAX_ENERGY; i++) energy[i].active = 0;
    initialized = 1;
    print_str("[SB] Smart building initialized\n");
    return 0;
}

int sb_add_floor(int n_zones) {
    if (sb.n_floors >= MAX_FLOORS) return -1;
    floor_t* f = &floors[sb.n_floors];
    f->floor_id = sb.n_floors;
    f->n_zones = n_zones;
    f->n_devices = 0;
    f->occupancy = 0;
    f->active = 1;
    sb.n_floors++;
    print_str("[SB] Floor "); print_int(sb.n_floors - 1);
    print_str(" zones="); print_int(n_zones); print_str("\n");
    return sb.n_floors - 1;
}

int sb_add_zone(int floor_id, int type, double target_temp) {
    if (sb.n_zones >= MAX_ZONES || floor_id >= sb.n_floors) return -1;
    zone_t* z = &zones[sb.n_zones];
    z->zone_id = sb.n_zones;
    z->floor_id = floor_id;
    z->type = type;
    z->target_temp = target_temp;
    z->current_temp = target_temp - 1.0;
    z->occupancy = 0;
    z->active = 1;
    sb.n_zones++;
    floors[floor_id].n_zones++;
    print_str("[SB] Zone "); print_int(sb.n_zones - 1);
    print_str(" F"); print_int(floor_id);
    print_str(" type="); print_int(type);
    print_str(" target="); print_int((int)(target_temp * 10)); print_str("C\n");
    return sb.n_zones - 1;
}

int sb_add_device(int zone_id, int type, double power) {
    if (sb.n_devices >= MAX_DEVICES || zone_id >= sb.n_zones) return -1;
    device_t* d = &devices[sb.n_devices];
    d->device_id = sb.n_devices;
    d->zone_id = zone_id;
    d->type = type;
    d->state = 0;
    d->power_watts = power;
    d->active = 1;
    sb.n_devices++;
    floors[zones[zone_id].floor_id].n_devices++;
    print_str("[SB] Device "); print_int(sb.n_devices - 1);
    print_str(" Z"); print_int(zone_id);
    print_str(" type="); print_int(type);
    print_str(" power="); print_int((int)power); print_str("W\n");
    return sb.n_devices - 1;
}

void sb_turn_on_device(int device_id) {
    if (device_id >= sb.n_devices) return;
    devices[device_id].state = 1;
    sb.total_power_watts += devices[device_id].power_watts;
    print_str("[SB] Device "); print_int(device_id);
    print_str(" ON ("); print_int((int)devices[device_id].power_watts); print_str("W)\n");
}

void sb_turn_off_device(int device_id) {
    if (device_id >= sb.n_devices) return;
    if (devices[device_id].state == 1) {
        devices[device_id].state = 0;
        sb.total_power_watts -= devices[device_id].power_watts;
    }
    print_str("[SB] Device "); print_int(device_id); print_str(" OFF\n");
}

int sb_add_sensor(int zone_id, int type, double threshold) {
    if (sb.n_sensors >= MAX_SENSORS || zone_id >= sb.n_zones) return -1;
    sensor_t* s = &sensors[sb.n_sensors];
    s->sensor_id = sb.n_sensors;
    s->zone_id = zone_id;
    s->type = type;
    s->value = threshold * 0.7;
    s->threshold = threshold;
    s->status = 1;
    s->active = 1;
    sb.n_sensors++;
    print_str("[SB] Sensor "); print_int(sb.n_sensors - 1);
    print_str(" Z"); print_int(zone_id);
    print_str(" type="); print_int(type);
    print_str(" thresh="); print_int((int)threshold); print_str("\n");
    return sb.n_sensors - 1;
}

void sb_read_sensor(int sensor_id) {
    if (sensor_id >= sb.n_sensors) return;
    sensor_t* s = &sensors[sensor_id];
    s->value = s->threshold * (0.6 + (double)((sensor_id * 7 + 3) % 10) * 0.05);
    int status = (s->value > s->threshold) ? 2 : 1;
    s->status = status;
    print_str("[SB] Sensor "); print_int(sensor_id);
    print_str(" val="); print_int((int)s->value);
    print_str(" thresh="); print_int((int)s->threshold);
    if (status == 2) print_str(" ALERT");
    print_str("\n");
}

int sb_trigger_alarm(int type, int zone_id, int severity, int hour) {
    if (sb.n_alarms >= MAX_ALARMS || zone_id >= sb.n_zones) return -1;
    alarm_t* a = &alarms[sb.n_alarms];
    a->alarm_id = sb.n_alarms;
    a->type = type;
    a->zone_id = zone_id;
    a->severity = severity;
    a->hour = hour;
    a->acknowledged = 0;
    a->active = 1;
    sb.n_alarms++;
    sb.alarms_active++;
    print_str("[SB] ALARM "); print_int(sb.n_alarms - 1);
    print_str(" type="); print_int(type);
    print_str(" Z"); print_int(zone_id);
    print_str(" sev="); print_int(severity);
    print_str(" hour="); print_int(hour); print_str("\n");
    return sb.n_alarms - 1;
}

void sb_acknowledge_alarm(int alarm_id) {
    if (alarm_id >= sb.n_alarms) return;
    alarms[alarm_id].acknowledged = 1;
    sb.alarms_active--;
    print_str("[SB] Alarm "); print_int(alarm_id); print_str(" acknowledged\n");
}

int sb_add_parking(int floor_id, int type) {
    if (sb.n_parking >= MAX_PARKING || floor_id >= sb.n_floors) return -1;
    parking_t* p = &parking[sb.n_parking];
    p->spot_id = sb.n_parking;
    p->floor_id = floor_id;
    p->type = type;
    p->occupied = 0;
    p->vehicle_id = -1;
    p->active = 1;
    sb.n_parking++;
    sb.total_spots++;
    print_str("[SB] Parking "); print_int(sb.n_parking - 1);
    print_str(" F"); print_int(floor_id);
    print_str(" type="); print_int(type); print_str("\n");
    return sb.n_parking - 1;
}

void sb_park_vehicle(int spot_id, int vehicle_id) {
    if (spot_id >= sb.n_parking) return;
    parking[spot_id].occupied = 1;
    parking[spot_id].vehicle_id = vehicle_id;
    sb.occupied_spots++;
    print_str("[SB] Vehicle "); print_int(vehicle_id);
    print_str(" parked at spot "); print_int(spot_id); print_str("\n");
}

void sb_unpark_vehicle(int spot_id) {
    if (spot_id >= sb.n_parking) return;
    if (parking[spot_id].occupied) {
        parking[spot_id].occupied = 0;
        parking[spot_id].vehicle_id = -1;
        sb.occupied_spots--;
    }
    print_str("[SB] Vehicle left spot "); print_int(spot_id); print_str("\n");
}

void sb_record_energy(int hour, double kwh, int peak) {
    if (sb.n_energy >= MAX_ENERGY) return;
    energy_reading_t* e = &energy[sb.n_energy];
    e->reading_id = sb.n_energy;
    e->hour = hour;
    e->consumption_kwh = kwh;
    e->cost = peak ? kwh * 0.25 : kwh * 0.12;
    e->peak = peak;
    e->active = 1;
    sb.n_energy++;
    sb.total_energy_kwh += kwh;
    sb.total_cost += e->cost;
    print_str("[SB] Energy "); print_int(hour); print_str(":00");
    print_str(" "); print_int((int)kwh); print_str("kWh");
    if (peak) print_str(" PEAK");
    print_str(" cost="); print_int((int)(e->cost * 100)); print_str("c\n");
}

void sb_energy_summary(void) {
    print_str("[SB] Energy summary:\n");
    print_str("  Total consumption: "); print_int((int)sb.total_energy_kwh); print_str("kWh\n");
    print_str("  Total cost: "); print_int((int)(sb.total_cost * 100)); print_str("c\n");
    int peak_hours = 0, off_peak = 0;
    double peak_total = 0.0, off_peak_total = 0.0;
    for (int i = 0; i < sb.n_energy; i++) {
        if (energy[i].peak) { peak_hours++; peak_total += energy[i].consumption_kwh; }
        else { off_peak++; off_peak_total += energy[i].consumption_kwh; }
    }
    print_str("  Peak hours: "); print_int(peak_hours); print_str("\n");
    print_str("  Peak consumption: "); print_int((int)peak_total); print_str("kWh\n");
    print_str("  Off-peak hours: "); print_int(off_peak); print_str("\n");
    print_str("  Off-peak consumption: "); print_int((int)off_peak_total); print_str("kWh\n");
}

void sb_print_state(void) {
    print_str("[SB] Floors="); print_int(sb.n_floors);
    print_str(" Zones="); print_int(sb.n_zones);
    print_str(" Devices="); print_int(sb.n_devices);
    print_str(" Sensors="); print_int(sb.n_sensors);
    print_str(" Alarms="); print_int(sb.n_alarms);
    print_str("\n");
    print_str("  Power: "); print_int((int)sb.total_power_watts); print_str("W\n");
    print_str("  Parking: "); print_int(sb.occupied_spots);
    print_str("/"); print_int(sb.total_spots); print_str(" occupied\n");
    print_str("  Active alarms: "); print_int(sb.alarms_active); print_str("\n");
    print_str("  Energy: "); print_int((int)sb.total_energy_kwh); print_str("kWh\n");
}

int main(void) {
    print_str("=== Smart Building System Demo ===\n\n");
    sb_init();

    print_str("Adding floors...\n");
    sb_add_floor(3);
    sb_add_floor(4);
    sb_add_floor(3);
    sb_add_floor(2);

    print_str("\nAdding zones...\n");
    sb_add_zone(0, 1, 22.0);
    sb_add_zone(0, 2, 20.0);
    sb_add_zone(0, 1, 23.0);
    sb_add_zone(1, 1, 22.5);
    sb_add_zone(1, 2, 21.0);
    sb_add_zone(1, 1, 22.0);
    sb_add_zone(1, 3, 19.0);
    sb_add_zone(2, 1, 22.0);
    sb_add_zone(2, 2, 20.5);
    sb_add_zone(2, 1, 23.0);
    sb_add_zone(3, 1, 22.5);
    sb_add_zone(3, 3, 18.0);

    print_str("\nAdding devices...\n");
    sb_add_device(0, 1, 1500.0);
    sb_add_device(0, 2, 500.0);
    sb_add_device(1, 1, 2000.0);
    sb_add_device(2, 2, 300.0);
    sb_add_device(3, 1, 1800.0);
    sb_add_device(4, 2, 400.0);
    sb_add_device(5, 1, 1200.0);
    sb_add_device(6, 3, 3000.0);
    sb_add_device(7, 1, 1600.0);
    sb_add_device(8, 2, 600.0);
    sb_add_device(9, 1, 1400.0);
    sb_add_device(10, 1, 1700.0);

    print_str("\nTurning on devices...\n");
    sb_turn_on_device(0);
    sb_turn_on_device(1);
    sb_turn_on_device(2);
    sb_turn_on_device(3);
    sb_turn_on_device(4);
    sb_turn_on_device(5);
    sb_turn_on_device(7);
    sb_turn_on_device(8);
    sb_turn_on_device(9);
    sb_turn_on_device(10);

    print_str("\nAdding sensors...\n");
    sb_add_sensor(0, 1, 25.0);
    sb_add_sensor(1, 2, 60.0);
    sb_add_sensor(2, 1, 25.0);
    sb_add_sensor(3, 3, 800.0);
    sb_add_sensor(4, 2, 65.0);
    sb_add_sensor(5, 1, 25.0);
    sb_add_sensor(6, 2, 70.0);
    sb_add_sensor(7, 1, 25.0);
    sb_add_sensor(8, 3, 1000.0);

    print_str("\nReading sensors...\n");
    for (int i = 0; i < sb.n_sensors; i++) sb_read_sensor(i);

    print_str("\nTriggering alarms...\n");
    sb_trigger_alarm(1, 3, 2, 14);
    sb_trigger_alarm(2, 6, 3, 15);
    sb_trigger_alarm(1, 8, 1, 16);

    print_str("\nAcknowledging alarms...\n");
    sb_acknowledge_alarm(0);
    sb_acknowledge_alarm(1);

    print_str("\nAdding parking spots...\n");
    for (int i = 0; i < 10; i++) {
        sb_add_parking(i % sb.n_floors, i % 3);
    }

    print_str("\nParking vehicles...\n");
    sb_park_vehicle(0, 100);
    sb_park_vehicle(1, 101);
    sb_park_vehicle(3, 102);
    sb_park_vehicle(5, 103);
    sb_park_vehicle(7, 104);
    sb_park_vehicle(8, 105);

    print_str("\nVehicles leaving...\n");
    sb_unpark_vehicle(1);
    sb_unpark_vehicle(5);

    print_str("\nRecording energy...\n");
    sb_record_energy(0, 45.0, 0);
    sb_record_energy(3, 30.0, 0);
    sb_record_energy(6, 55.0, 0);
    sb_record_energy(9, 120.0, 1);
    sb_record_energy(12, 150.0, 1);
    sb_record_energy(15, 140.0, 1);
    sb_record_energy(18, 110.0, 1);
    sb_record_energy(21, 70.0, 0);

    print_str("\nEnergy summary...\n");
    sb_energy_summary();

    print_str("\nTurning off some devices...\n");
    sb_turn_off_device(3);
    sb_turn_off_device(7);

    print_str("\nFinal state...\n");
    sb_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
