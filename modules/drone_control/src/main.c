/* drone_control: UAV flight control system (v1.0)
 * Attitude control, navigation, missions, vision, communication, power
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

#define MAX_WAYPOINTS 32
#define MAX_FENCES    8
#define MAX_TARGETS   8
#define MAX_LOGS      64

#define MODE_MANUAL   0x01
#define MODE_STABILIZE 0x02
#define MODE_ALT_HOLD 0x03
#define MODE_LOITER   0x04
#define MODE_RTH      0x05
#define MODE_AUTO     0x06
#define MODE_LAND     0x07

#define NAV_WPT     0x01
#define NAV_TRACK   0x02
#define NAV_ORBIT   0x03
#define NAV_FOLLOW  0x04

#define MISSION_SURVEY   0x01
#define MISSION_INSPECT  0x02
#define MISSION_SWARM    0x03
#define MISSION_PAYLOAD  0x04

typedef struct {
    double x, y, z;
    double roll, pitch, yaw;
    double vx, vy, vz;
    double roll_rate, pitch_rate, yaw_rate;
    double ax, ay, az;
    double throttle;
} drone_state_t;

typedef struct {
    double x, y, z;
    double speed;
    double loiter_time;
    int    action;
    int    active;
} waypoint_t;

typedef struct {
    double x, y;
    double radius;
    int    type;
    int    active;
} fence_t;

typedef struct {
    double x, y, z;
    double vx, vy, vz;
    int    type;
    int    id;
    double confidence;
    int    active;
} target_t;

typedef struct {
    int    type;
    int    waypoint_count;
    double coverage_area;
    double flight_time;
    double total_distance;
    int    completed;
    int    current_wp;
} mission_t;

typedef struct {
    double voltage;
    double current;
    double capacity;
    double soc;
    double temperature;
    double remaining_time;
    int    cell_count;
    int    low_battery;
    int    critical_battery;
} battery_t;

typedef struct {
    int    mode;
    int    nav_mode;
    int    armed;
    int    gps_fix;
    int    gps_sats;
    double home_x, home_y, home_z;
    int    in_fence;
    int    rth_triggered;
    int    land_triggered;
    int    logs_count;
} drone_control_t;

static drone_state_t drone;
static waypoint_t waypoints[MAX_WAYPOINTS];
static fence_t fences[MAX_FENCES];
static target_t targets[MAX_TARGETS];
static mission_t mission;
static battery_t battery;
static drone_control_t ctrl;

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
static double dsqrt(double x) {
    if (x <= 0.0) return 0.0;
    double g = x, p = x / g;
    for (int i = 0; i < 30; i++) { g = (g + p) * 0.5; p = x / g; }
    return g;
}
static double dcos(double x) {
    double sum = 1.0, term = 1.0, x2 = x * x;
    for (int i = 1; i < 15; i++) { term *= -x2 / ((2*i-1)*(2*i)); sum += term; }
    return sum;
}
static double dsin(double x) {
    double sum = x, term = x, x2 = x * x;
    for (int i = 1; i < 15; i++) { term *= -x2 / ((2*i)*(2*i+1)); sum += term; }
    return sum;
}

int drone_init(void) {
    if (initialized) return -1;
    drone.x = 0.0; drone.y = 0.0; drone.z = 0.0;
    drone.roll = 0.0; drone.pitch = 0.0; drone.yaw = 0.0;
    drone.vx = 0.0; drone.vy = 0.0; drone.vz = 0.0;
    drone.roll_rate = 0.0; drone.pitch_rate = 0.0; drone.yaw_rate = 0.0;
    drone.throttle = 0.0;
    ctrl.mode = MODE_STABILIZE; ctrl.nav_mode = NAV_WPT;
    ctrl.armed = 0; ctrl.gps_fix = 1; ctrl.gps_sats = 12;
    ctrl.home_x = 0.0; ctrl.home_y = 0.0; ctrl.home_z = 0.0;
    ctrl.in_fence = 1; ctrl.rth_triggered = 0; ctrl.land_triggered = 0;
    ctrl.logs_count = 0;
    battery.voltage = 22.2; battery.current = 0.0;
    battery.capacity = 5000.0; battery.soc = 100.0;
    battery.temperature = 25.0; battery.remaining_time = 25.0 * 60.0;
    battery.cell_count = 6; battery.low_battery = 0; battery.critical_battery = 0;
    for (int i = 0; i < MAX_WAYPOINTS; i++) waypoints[i].active = 0;
    for (int i = 0; i < MAX_FENCES; i++) fences[i].active = 0;
    for (int i = 0; i < MAX_TARGETS; i++) targets[i].active = 0;
    mission.type = 0; mission.completed = 0; mission.current_wp = 0;
    initialized = 1;
    print_str("[DRONE] Flight control initialized\n");
    return 0;
}

int drone_add_waypoint(double x, double y, double z, double speed) {
    int i = mission.waypoint_count;
    if (i >= MAX_WAYPOINTS) return -1;
    waypoints[i].x = x; waypoints[i].y = y; waypoints[i].z = z;
    waypoints[i].speed = speed; waypoints[i].loiter_time = 0.0;
    waypoints[i].action = 0; waypoints[i].active = 1;
    mission.waypoint_count++;
    return i;
}

int drone_add_fence(double x, double y, double radius, int type) {
    for (int i = 0; i < MAX_FENCES; i++) {
        if (!fences[i].active) {
            fences[i].x = x; fences[i].y = y; fences[i].radius = radius;
            fences[i].type = type; fences[i].active = 1;
            return i;
        }
    }
    return -1;
}

void drone_attitude_control(double dt) {
    double Kp_roll = 5.0, Kd_roll = 0.3;
    double Kp_pitch = 5.0, Kd_pitch = 0.3;
    double Kp_yaw = 3.0, Kd_yaw = 0.2;
    double target_roll = drone.pitch * 0.3;
    double target_pitch = -drone.pitch * 0.3;
    double target_yaw = 0.0;
    double err_roll = target_roll - drone.roll;
    double err_pitch = target_pitch - drone.pitch;
    double err_yaw = target_yaw - drone.yaw;
    double cmd_roll = Kp_roll * err_roll - Kd_roll * drone.roll_rate;
    double cmd_pitch = Kp_pitch * err_pitch - Kd_pitch * drone.pitch_rate;
    double cmd_yaw = Kp_yaw * err_yaw - Kd_yaw * drone.yaw_rate;
    drone.roll_rate += cmd_roll * dt;
    drone.pitch_rate += cmd_pitch * dt;
    drone.yaw_rate += cmd_yaw * dt;
    drone.roll += drone.roll_rate * dt;
    drone.pitch += drone.pitch_rate * dt;
    drone.yaw += drone.yaw_rate * dt;
}

void drone_position_control(double dt) {
    if (mission.waypoint_count == 0) return;
    int idx = mission.current_wp;
    if (idx >= mission.waypoint_count) return;
    double dx = waypoints[idx].x - drone.x;
    double dy = waypoints[idx].y - drone.y;
    double dz = waypoints[idx].z - drone.z;
    double dist = dsqrt(dx*dx + dy*dy + dz*dz);
    double target_speed = waypoints[idx].speed;
    if (dist < 2.0) target_speed = 0.0;
    double Kp_xy = 0.5, Kp_z = 0.3;
    drone.vx = dx * Kp_xy; drone.vy = dy * Kp_xy; drone.vz = dz * Kp_z;
    double v_mag = dsqrt(drone.vx*drone.vx + drone.vy*drone.vy);
    if (v_mag > target_speed && target_speed > 0.0) {
        drone.vx *= target_speed / v_mag;
        drone.vy *= target_speed / v_mag;
    }
    drone.x += drone.vx * dt;
    drone.y += drone.vy * dt;
    drone.z += drone.vz * dt;
    if (dist < 1.0) {
        mission.current_wp++;
        print_str("[DRONE] Reached waypoint "); print_int(idx); print_str("\n");
    }
}

void drone_check_fence(void) {
    ctrl.in_fence = 1;
    for (int i = 0; i < MAX_FENCES; i++) {
        if (!fences[i].active) continue;
        double dx = drone.x - fences[i].x;
        double dy = drone.y - fences[i].y;
        double dist = dsqrt(dx*dx + dy*dy);
        if (fences[i].type == 1 && dist > fences[i].radius) {
            ctrl.in_fence = 0;
            ctrl.rth_triggered = 1;
        } else if (fences[i].type == 2 && dist < fences[i].radius) {
            ctrl.in_fence = 0;
            ctrl.rth_triggered = 1;
        }
    }
}

void drone_battery_monitor(double dt) {
    battery.current = 10.0 + dabs(drone.vx) + dabs(drone.vy) + dabs(drone.vz) * 0.5;
    double used_mah = battery.current * dt * 0.001;
    battery.soc -= used_mah / battery.capacity * 100.0;
    if (battery.soc < 0.0) battery.soc = 0.0;
    battery.voltage = 3.7 * battery.cell_count * (0.8 + 0.2 * battery.soc / 100.0);
    battery.temperature = 25.0 + battery.current * 0.5;
    battery.remaining_time = battery.soc / 100.0 * battery.capacity / (battery.current + 1e-10) * 60.0;
    if (battery.soc < 30.0 && !battery.low_battery) {
        battery.low_battery = 1;
        print_str("[DRONE] LOW BATTERY WARNING - ");
        print_int((int)battery.soc); print_str("%\n");
    }
    if (battery.soc < 15.0 && !battery.critical_battery) {
        battery.critical_battery = 1;
        ctrl.land_triggered = 1;
        print_str("[DRONE] CRITICAL BATTERY - Auto landing\n");
    }
}

void drone_mission_plan(int type) {
    mission.type = type;
    mission.completed = 0;
    mission.current_wp = 0;
    mission.waypoint_count = 0;
    if (type == MISSION_SURVEY) {
        double area = 100.0;
        double spacing = 20.0;
        int n_lines = (int)(area / spacing);
        for (int i = 0; i < n_lines && mission.waypoint_count < MAX_WAYPOINTS; i++) {
            double y = i * spacing;
            drone_add_waypoint(0.0, y, 50.0, 10.0);
            drone_add_waypoint(area, y, 50.0, 10.0);
        }
        mission.coverage_area = area * area;
    } else if (type == MISSION_INSPECT) {
        double cx = 50.0, cy = 50.0, r = 30.0;
        int n_points = 8;
        for (int i = 0; i < n_points; i++) {
            double ang = 2.0 * 3.14159 * i / n_points;
            drone_add_waypoint(cx + r * dcos(ang), cy + r * dsin(ang), 20.0, 5.0);
        }
        mission.coverage_area = 3.14159 * r * r;
    } else if (type == MISSION_SWARM) {
        for (int i = 0; i < 5; i++) {
            drone_add_waypoint(20.0 * i, 0.0, 30.0, 8.0);
            drone_add_waypoint(20.0 * i, 40.0, 30.0, 8.0);
        }
        mission.coverage_area = 80.0 * 40.0;
    }
    mission.total_distance = 0.0;
    for (int i = 1; i < mission.waypoint_count; i++) {
        double dx = waypoints[i].x - waypoints[i-1].x;
        double dy = waypoints[i].y - waypoints[i-1].y;
        double dz = waypoints[i].z - waypoints[i-1].z;
        mission.total_distance += dsqrt(dx*dx + dy*dy + dz*dz);
    }
    mission.flight_time = mission.total_distance / 8.0;
    print_str("[DRONE] Mission type="); print_int(type);
    print_str(" waypoints="); print_int(mission.waypoint_count);
    print_str(" distance="); print_int((int)mission.total_distance);
    print_str("m time="); print_int((int)mission.flight_time); print_str("s\n");
}

void drone_arm(void) {
    if (ctrl.gps_fix && ctrl.gps_sats >= 8 && battery.soc > 20.0) {
        ctrl.armed = 1;
        print_str("[DRONE] Motors armed\n");
    } else {
        print_str("[DRONE] Arm failed: GPS="); print_int(ctrl.gps_fix);
        print_str(" sats="); print_int(ctrl.gps_sats);
        print_str(" SOC="); print_int((int)battery.soc); print_str("%\n");
    }
}

void drone_takeoff(double target_alt) {
    if (!ctrl.armed) return;
    ctrl.mode = MODE_AUTO;
    drone.z = 0.0;
    double alt = 0.0;
    while (alt < target_alt) {
        alt += 1.0;
        drone.z = alt;
    }
    print_str("[DRONE] Takeoff to "); print_int((int)target_alt); print_str("m\n");
}

void drone_return_to_home(void) {
    ctrl.rth_triggered = 1;
    ctrl.mode = MODE_RTH;
    double dx = ctrl.home_x - drone.x;
    double dy = ctrl.home_y - drone.y;
    double dist = dsqrt(dx*dx + dy*dy);
    print_str("[DRONE] Return to home: dist="); print_int((int)dist); print_str("m\n");
    drone.vx = dx * 0.1; drone.vy = dy * 0.1;
    drone.x += drone.vx; drone.y += drone.vy;
}

void drone_land(void) {
    ctrl.land_triggered = 1;
    ctrl.mode = MODE_LAND;
    print_str("[DRONE] Landing...\n");
    while (drone.z > 0.0) {
        drone.z -= 0.5;
        if (drone.z < 0.0) drone.z = 0.0;
    }
    ctrl.armed = 0;
    print_str("[DRONE] Landed and disarmed\n");
}

void drone_track_target(int target_id) {
    if (target_id >= MAX_TARGETS || !targets[target_id].active) return;
    ctrl.nav_mode = NAV_FOLLOW;
    target_t* t = &targets[target_id];
    double dx = t->x - drone.x;
    double dy = t->y - drone.y;
    double dist = dsqrt(dx*dx + dy*dy);
    print_str("[DRONE] Tracking target "); print_int(target_id);
    print_str(" dist="); print_int((int)dist); print_str("m\n");
}

void drone_detect_targets(void) {
    int detected = 0;
    for (int i = 0; i < MAX_TARGETS; i++) {
        if (targets[i].active) detected++;
    }
    print_str("[DRONE] Vision: "); print_int(detected); print_str(" targets\n");
}

void drone_log_state(void) {
    if (ctrl.logs_count >= MAX_LOGS) return;
    ctrl.logs_count++;
}

void drone_print_state(void) {
    print_str("[DRONE] x="); print_int((int)(drone.x * 10));
    print_str(" y="); print_int((int)(drone.y * 10));
    print_str(" z="); print_int((int)(drone.z * 10));
    print_str(" roll="); print_int((int)(drone.roll * 573));
    print_str(" pitch="); print_int((int)(drone.pitch * 573));
    print_str(" yaw="); print_int((int)(drone.yaw * 573));
    print_str("\n  mode="); print_int(ctrl.mode);
    print_str(" armed="); print_int(ctrl.armed);
    print_str(" SOC="); print_int((int)battery.soc);
    print_str("% V="); print_int((int)(battery.voltage * 100));
    print_str(" T="); print_int((int)battery.remaining_time); print_str("s\n");
}

int main(void) {
    print_str("=== Drone Control System Demo ===\n\n");
    drone_init();

    print_str("Setting home position...\n");
    ctrl.home_x = 0.0; ctrl.home_y = 0.0;

    print_str("\nAdding geofences...\n");
    drone_add_fence(0.0, 0.0, 200.0, 1);
    drone_add_fence(100.0, 100.0, 30.0, 2);

    print_str("\nAdding targets...\n");
    for (int i = 0; i < 3; i++) {
        targets[i].x = 30.0 + i * 20.0;
        targets[i].y = 40.0;
        targets[i].z = 10.0;
        targets[i].vx = 0.5; targets[i].vy = 0.0;
        targets[i].type = 1; targets[i].id = i;
        targets[i].confidence = 0.9;
        targets[i].active = 1;
    }

    print_str("\nArming motors...\n");
    drone_arm();

    print_str("\nTakeoff to 30m...\n");
    drone_takeoff(30.0);

    print_str("\nMission: area survey...\n");
    drone_mission_plan(MISSION_SURVEY);

    print_str("\nFlying mission...\n");
    for (int i = 0; i < 5; i++) {
        drone_attitude_control(0.1);
        drone_position_control(0.1);
        drone_check_fence();
        drone_battery_monitor(0.1);
        drone_log_state();
    }
    drone_print_state();

    print_str("\nVision detection...\n");
    drone_detect_targets();

    print_str("\nTracking target 0...\n");
    drone_track_target(0);

    print_str("\nBattery status...\n");
    for (int i = 0; i < 100; i++) drone_battery_monitor(1.0);
    print_str("  V="); print_int((int)(battery.voltage * 100));
    print_str(" I="); print_int((int)(battery.current * 10));
    print_str(" SOC="); print_int((int)battery.soc);
    print_str("% T="); print_int((int)battery.remaining_time); print_str("s\n");

    print_str("\nReturn to home...\n");
    drone_return_to_home();

    print_str("\nLanding...\n");
    drone_land();

    drone_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
