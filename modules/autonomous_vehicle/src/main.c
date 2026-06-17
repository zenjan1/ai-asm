/* autonomous_vehicle: Autonomous driving system (v1.0)
 * Perception, localization, prediction, planning, control, decision
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

#define MAX_OBJECTS  32
#define MAX_WAYPOINTS 64
#define MAX_OBSTACLES 32
#define MAX_STATES   16

#define OBJ_CAR     0x01
#define OBJ_TRUCK   0x02
#define OBJ_PED     0x03
#define OBJ_CYCLIST 0x04
#define OBJ_SIGN    0x05

#define LOC_GPS     0x01
#define LOC_VISUAL  0x02
#define LOC_LIDAR   0x03
#define LOC_FUSED   0x04

#define PLAN_ASTAR  0x01
#define PLAN_RRT    0x02
#define PLAN_DWA    0x03
#define PLAN_APF    0x04

#define CTRL_PID    0x01
#define CTRL_LQR    0x02
#define CTRL_MPC    0x03
#define CTRL_PURE   0x04
#define CTRL_STAN   0x05

typedef struct {
    double x, y;
    double vx, vy;
    double heading;
    double length, width;
    int    type;
    int    id;
    double confidence;
    int    active;
} object_t;

typedef struct {
    double x[MAX_WAYPOINTS], y[MAX_WAYPOINTS];
    double heading[MAX_WAYPOINTS];
    double speed[MAX_WAYPOINTS];
    int    count;
    int    current_idx;
} trajectory_t;

typedef struct {
    double x, y;
    double vx, vy;
    double heading;
    double yaw_rate;
    double ax, ay;
    double steering;
    double throttle;
    double brake;
    double speed;
} vehicle_state_t;

typedef struct {
    double x, y;
    double radius;
    int    active;
} obstacle_t;

typedef struct {
    int    loc_method;
    double x, y, heading;
    double cov[3][3];
    double confidence;
} localization_t;

typedef struct {
    int    plan_method;
    int    ctrl_method;
    double target_speed;
    double path_length;
    double eta;
    int    goal_reached;
    int    collision_risk;
} planner_t;

typedef struct {
    int    object_count;
    int    obstacle_count;
    int    waypoint_count;
    double ego_speed;
    double ego_heading;
    double ego_x, ego_y;
    int    loc_method;
    int    plan_method;
    int    ctrl_method;
    int    decision;
    int    safety_level;
} av_state_t;

static object_t objects[MAX_OBJECTS];
static trajectory_t planned_path;
static trajectory_t reference_path;
static vehicle_state_t ego;
static obstacle_t obstacles[MAX_OBSTACLES];
static localization_t loc;
static planner_t planner;
static av_state_t av;

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

int av_init(void) {
    if (initialized) return -1;
    av.object_count = 0; av.obstacle_count = 0; av.waypoint_count = 0;
    av.ego_speed = 0.0; av.ego_heading = 0.0; av.ego_x = 0.0; av.ego_y = 0.0;
    av.loc_method = LOC_FUSED; av.plan_method = PLAN_RRT; av.ctrl_method = CTRL_MPC;
    av.decision = 0; av.safety_level = 3;
    ego.x = 0.0; ego.y = 0.0; ego.heading = 0.0; ego.speed = 0.0;
    ego.steering = 0.0; ego.throttle = 0.0; ego.brake = 0.0;
    loc.x = 0.0; loc.y = 0.0; loc.heading = 0.0; loc.confidence = 1.0;
    planned_path.count = 0; reference_path.count = 0;
    for (int i = 0; i < MAX_OBJECTS; i++) objects[i].active = 0;
    for (int i = 0; i < MAX_OBSTACLES; i++) obstacles[i].active = 0;
    initialized = 1;
    print_str("[AV] Autonomous vehicle system initialized\n");
    return 0;
}

int av_add_object(double x, double y, double vx, double vy, int type) {
    if (av.object_count >= MAX_OBJECTS) return -1;
    object_t* o = &objects[av.object_count];
    o->x = x; o->y = y; o->vx = vx; o->vy = vy;
    o->type = type; o->id = av.object_count;
    o->heading = 0.0;
    if (dabs(vx) > 0.01 || dabs(vy) > 0.01) {
        o->heading = 0.0;
    }
    o->length = 4.5; o->width = 1.8;
    if (type == OBJ_PED) { o->length = 0.5; o->width = 0.5; }
    else if (type == OBJ_CYCLIST) { o->length = 1.8; o->width = 0.6; }
    else if (type == OBJ_TRUCK) { o->length = 10.0; o->width = 2.5; }
    o->confidence = 0.95;
    o->active = 1;
    return av.object_count++;
}

int av_add_obstacle(double x, double y, double r) {
    if (av.obstacle_count >= MAX_OBSTACLES) return -1;
    obstacles[av.obstacle_count].x = x;
    obstacles[av.obstacle_count].y = y;
    obstacles[av.obstacle_count].radius = r;
    obstacles[av.obstacle_count].active = 1;
    return av.obstacle_count++;
}

void av_set_reference_path(double* wx, double* wy, int count) {
    reference_path.count = count < MAX_WAYPOINTS ? count : MAX_WAYPOINTS;
    for (int i = 0; i < reference_path.count; i++) {
        reference_path.x[i] = wx[i];
        reference_path.y[i] = wy[i];
        reference_path.speed[i] = 10.0;
        if (i < reference_path.count - 1) {
            double dx = wx[i+1] - wx[i];
            double dy = wy[i+1] - wy[i];
            reference_path.heading[i] = 0.0;
            if (dabs(dx) > 0.01 || dabs(dy) > 0.01) reference_path.heading[i] = 0.0;
        }
    }
    av.waypoint_count = reference_path.count;
    print_str("[AV] Reference path set with "); print_int(reference_path.count);
    print_str(" waypoints\n");
}

void av_perception(void) {
    print_str("[AV] Perception: "); print_int(av.object_count); print_str(" objects detected\n");
    for (int i = 0; i < av.object_count && i < 6; i++) {
        object_t* o = &objects[i];
        double rel_x = o->x - ego.x;
        double rel_y = o->y - ego.y;
        double dist = dsqrt(rel_x*rel_x + rel_y*rel_y);
        double speed = dsqrt(o->vx*o->vx + o->vy*o->vy);
        print_str("  ID "); print_int(o->id);
        print_str(" type="); print_int(o->type);
        print_str(" d="); print_int((int)(dist * 10));
        print_str("m v="); print_int((int)(speed * 10));
        print_str("m/s conf="); print_int((int)(o->confidence * 100)); print_str("%\n");
    }
}

void av_localization(int method) {
    av.loc_method = method;
    loc.loc_method = method;
    if (method == LOC_GPS) {
        loc.x = ego.x + 0.5;
        loc.y = ego.y + 0.3;
        loc.heading = ego.heading;
        loc.confidence = 0.7;
    } else if (method == LOC_VISUAL) {
        loc.x = ego.x - 0.2;
        loc.y = ego.y + 0.1;
        loc.heading = ego.heading + 0.01;
        loc.confidence = 0.8;
    } else if (method == LOC_LIDAR) {
        loc.x = ego.x + 0.1;
        loc.y = ego.y - 0.1;
        loc.heading = ego.heading - 0.005;
        loc.confidence = 0.9;
    } else {
        loc.x = ego.x;
        loc.y = ego.y;
        loc.heading = ego.heading;
        loc.confidence = 0.95;
    }
    loc.cov[0][0] = (1.0 - loc.confidence) * 10.0;
    loc.cov[1][1] = (1.0 - loc.confidence) * 10.0;
    loc.cov[2][2] = (1.0 - loc.confidence) * 0.1;
    print_str("[AV] Localization method="); print_int(method);
    print_str(" x="); print_int((int)(loc.x * 100));
    print_str(" y="); print_int((int)(loc.y * 100));
    print_str(" conf="); print_int((int)(loc.confidence * 100)); print_str("%\n");
}

void av_prediction(void) {
    print_str("[AV] Object prediction (2s horizon):\n");
    for (int i = 0; i < av.object_count && i < 5; i++) {
        object_t* o = &objects[i];
        double pred_x = o->x + o->vx * 2.0;
        double pred_y = o->y + o->vy * 2.0;
        double dist_to_ego = dsqrt((pred_x - ego.x) * (pred_x - ego.x) +
                                   (pred_y - ego.y) * (pred_y - ego.y));
        print_str("  ID "); print_int(o->id);
        print_str(" pred=("); print_int((int)(pred_x * 10));
        print_str(","); print_int((int)(pred_y * 10));
        print_str(") d_ego="); print_int((int)(dist_to_ego * 10));
        print_str("m\n");
    }
}

void av_path_planning(int method) {
    av.plan_method = method;
    planner.plan_method = method;
    planned_path.count = 0;
    if (method == PLAN_ASTAR) {
        double gx = reference_path.x[reference_path.count - 1];
        double gy = reference_path.y[reference_path.count - 1];
        double dx = gx - ego.x, dy = gy - ego.y;
        double dist = dsqrt(dx*dx + dy*dy);
        int steps = (int)(dist / 2.0);
        if (steps > MAX_WAYPOINTS) steps = MAX_WAYPOINTS;
        for (int i = 0; i < steps; i++) {
            double t = (double)i / (steps - 1);
            planned_path.x[i] = ego.x + t * dx;
            planned_path.y[i] = ego.y + t * dy;
            planned_path.count++;
        }
    } else if (method == PLAN_RRT) {
        int max_iter = 20;
        planned_path.x[0] = ego.x; planned_path.y[0] = ego.y;
        planned_path.count = 1;
        double gx = reference_path.x[reference_path.count - 1];
        double gy = reference_path.y[reference_path.count - 1];
        for (int iter = 0; iter < max_iter && planned_path.count < MAX_WAYPOINTS; iter++) {
            double rx = ego.x + ((iter * 7 + 3) % 50) - 25.0;
            double ry = ego.y + ((iter * 13 + 5) % 50) - 25.0;
            if (iter > max_iter - 5) { rx = gx; ry = gy; }
            double near_dist = 1e10;
            int near_idx = 0;
            for (int i = 0; i < planned_path.count; i++) {
                double d = dsqrt((rx - planned_path.x[i]) * (rx - planned_path.x[i]) +
                                 (ry - planned_path.y[i]) * (ry - planned_path.y[i]));
                if (d < near_dist) { near_dist = d; near_idx = i; }
            }
            double step = 3.0;
            double ang = 0.0;
            if (near_dist > 0.01) {
                double dx = rx - planned_path.x[near_idx];
                double dy = ry - planned_path.y[near_idx];
                ang = 0.0;
                if (dabs(dx) > 0.01 || dabs(dy) > 0.01) ang = 0.0;
                double len = dsqrt(dx*dx + dy*dy);
                if (len > step) { dx = dx / len * step; dy = dy / len * step; }
                double nx = planned_path.x[near_idx] + dx;
                double ny = planned_path.y[near_idx] + dy;
                int collision = 0;
                for (int o = 0; o < av.obstacle_count; o++) {
                    if (!obstacles[o].active) continue;
                    double d_obs = dsqrt((nx - obstacles[o].x) * (nx - obstacles[o].x) +
                                         (ny - obstacles[o].y) * (ny - obstacles[o].y));
                    if (d_obs < obstacles[o].radius + 1.0) { collision = 1; break; }
                }
                if (!collision) {
                    planned_path.x[planned_path.count] = nx;
                    planned_path.y[planned_path.count] = ny;
                    planned_path.count++;
                }
            }
            double to_goal = dsqrt((planned_path.x[planned_path.count-1] - gx) *
                                   (planned_path.x[planned_path.count-1] - gx) +
                                   (planned_path.y[planned_path.count-1] - gy) *
                                   (planned_path.y[planned_path.count-1] - gy));
            if (to_goal < 3.0) break;
        }
    } else if (method == PLAN_APF) {
        planned_path.x[0] = ego.x; planned_path.y[0] = ego.y;
        planned_path.count = 1;
        double gx = reference_path.x[reference_path.count - 1];
        double gy = reference_path.y[reference_path.count - 1];
        for (int step = 0; step < 30 && planned_path.count < MAX_WAYPOINTS; step++) {
            double cx = planned_path.x[planned_path.count - 1];
            double cy = planned_path.y[planned_path.count - 1];
            double dx_g = gx - cx, dy_g = gy - cy;
            double d_g = dsqrt(dx_g*dx_g + dy_g*dy_g);
            double fx = dx_g / (d_g + 1e-10) * 0.5;
            double fy = dy_g / (d_g + 1e-10) * 0.5;
            for (int o = 0; o < av.obstacle_count; o++) {
                if (!obstacles[o].active) continue;
                double dx_o = cx - obstacles[o].x;
                double dy_o = cy - obstacles[o].y;
                double d_o = dsqrt(dx_o*dx_o + dy_o*dy_o);
                if (d_o < obstacles[o].radius * 3.0 && d_o > 0.01) {
                    double rep = 1.0 / (d_o * d_o);
                    fx += dx_o / d_o * rep;
                    fy += dy_o / d_o * rep;
                }
            }
            double f_mag = dsqrt(fx*fx + fy*fy);
            if (f_mag > 0.01) {
                cx += fx / f_mag * 2.0;
                cy += fy / f_mag * 2.0;
                planned_path.x[planned_path.count] = cx;
                planned_path.y[planned_path.count] = cy;
                planned_path.count++;
            }
            if (d_g < 3.0) break;
        }
    }
    planner.path_length = 0.0;
    for (int i = 1; i < planned_path.count; i++) {
        double dx = planned_path.x[i] - planned_path.x[i-1];
        double dy = planned_path.y[i] - planned_path.y[i-1];
        planner.path_length += dsqrt(dx*dx + dy*dy);
    }
    planner.eta = planner.path_length / 10.0;
    print_str("[AV] Path planning method="); print_int(method);
    print_str(" waypoints="); print_int(planned_path.count);
    print_str(" length="); print_int((int)planner.path_length);
    print_str("m ETA="); print_int((int)planner.eta); print_str("s\n");
}

void av_control(int method) {
    av.ctrl_method = method;
    planner.ctrl_method = method;
    if (planned_path.count < 2) return;
    double target_x = planned_path.x[1];
    double target_y = planned_path.y[1];
    double dx = target_x - ego.x;
    double dy = target_y - ego.y;
    double target_heading = 0.0;
    if (dabs(dx) > 0.01 || dabs(dy) > 0.01) target_heading = 0.0;
    double heading_err = target_heading - ego.heading;
    if (method == CTRL_PID) {
        ego.steering = heading_err * 0.5;
        ego.throttle = (10.0 - ego.speed) * 0.3;
        ego.brake = 0.0;
    } else if (method == CTRL_PURE) {
        double ld = 5.0;
        double alpha = dsin(heading_err);
        ego.steering = 2.0 * alpha * ld / (10.0 + 1e-10);
        ego.throttle = (10.0 - ego.speed) * 0.2;
    } else if (method == CTRL_STAN) {
        double k = 0.5;
        double e_y = -dabs(dx) * dsin(heading_err);
        ego.steering = -k * e_y;
        ego.throttle = (10.0 - ego.speed) * 0.2;
    } else if (method == CTRL_LQR || method == CTRL_MPC) {
        double K1 = 0.8, K2 = 0.3;
        ego.steering = -(K1 * heading_err + K2 * ego.yaw_rate);
        ego.throttle = (10.0 - ego.speed) * 0.2;
    }
    if (ego.steering > 0.5) ego.steering = 0.5;
    if (ego.steering < -0.5) ego.steering = -0.5;
    if (ego.throttle < 0.0) ego.throttle = 0.0;
    if (ego.throttle > 1.0) ego.throttle = 1.0;
    print_str("[AV] Control method="); print_int(method);
    print_str(" steer="); print_int((int)(ego.steering * 1000));
    print_str(" thr="); print_int((int)(ego.throttle * 100));
    print_str(" speed="); print_int((int)(ego.speed * 10)); print_str("\n");
}

void av_collision_check(void) {
    planner.collision_risk = 0;
    for (int i = 0; i < av.object_count; i++) {
        if (!objects[i].active) continue;
        double dx = objects[i].x - ego.x;
        double dy = objects[i].y - ego.y;
        double dist = dsqrt(dx*dx + dy*dy);
        double pred_x = objects[i].x + objects[i].vx * 2.0;
        double pred_y = objects[i].y + objects[i].vy * 2.0;
        double pred_dist = dsqrt((pred_x - ego.x) * (pred_x - ego.x) +
                                 (pred_y - ego.y) * (pred_y - ego.y));
        if (dist < 5.0 || pred_dist < 5.0) planner.collision_risk = 1;
    }
    for (int o = 0; o < av.obstacle_count; o++) {
        if (!obstacles[o].active) continue;
        double dx = obstacles[o].x - ego.x;
        double dy = obstacles[o].y - ego.y;
        double dist = dsqrt(dx*dx + dy*dy);
        if (dist < obstacles[o].radius + 2.0) planner.collision_risk = 1;
    }
    print_str("[AV] Collision risk: "); print_int(planner.collision_risk); print_str("\n");
}

void av_decision_making(void) {
    if (planner.collision_risk) av.decision = 1;
    else if (ego.speed > 12.0) av.decision = 2;
    else av.decision = 0;
    print_str("[AV] Decision: ");
    if (av.decision == 0) print_str("cruise");
    else if (av.decision == 1) print_str("emergency brake");
    else print_str("slow down");
    print_str("\n");
    av.safety_level = planner.collision_risk ? 1 : 3;
}

void av_update_ego(double dt) {
    ego.speed += (ego.throttle - ego.brake) * dt * 3.0;
    if (ego.speed < 0.0) ego.speed = 0.0;
    if (ego.speed > 20.0) ego.speed = 20.0;
    ego.yaw_rate = ego.speed * ego.steering / 3.0;
    ego.heading += ego.yaw_rate * dt;
    ego.x += ego.speed * dcos(ego.heading) * dt;
    ego.y += ego.speed * dsin(ego.heading) * dt;
    av.ego_speed = ego.speed;
    av.ego_heading = ego.heading;
    av.ego_x = ego.x;
    av.ego_y = ego.y;
}

void av_print_state(void) {
    print_str("[AV] Obj="); print_int(av.object_count);
    print_str(" Obs="); print_int(av.obstacle_count);
    print_str(" WP="); print_int(av.waypoint_count);
    print_str(" Spd="); print_int((int)(av.ego_speed * 10));
    print_str(" Head="); print_int((int)(av.ego_heading * 573));
    print_str(" Safety="); print_int(av.safety_level);
    print_str("\n");
}

int main(void) {
    print_str("=== Autonomous Vehicle Demo ===\n\n");
    av_init();

    print_str("Setting reference path...\n");
    double wx[10] = {0.0, 10.0, 20.0, 30.0, 40.0, 50.0, 60.0, 70.0, 80.0, 90.0};
    double wy[10] = {0.0, 5.0, 10.0, 12.0, 10.0, 5.0, 0.0, -5.0, -8.0, -10.0};
    av_set_reference_path(wx, wy, 10);

    print_str("\nAdding detected objects...\n");
    av_add_object(20.0, 5.0, 2.0, 0.0, OBJ_CAR);
    av_add_object(30.0, 8.0, -1.0, 0.5, OBJ_PED);
    av_add_object(50.0, 3.0, 0.0, -1.0, OBJ_CYCLIST);
    av_add_object(15.0, -5.0, 3.0, 0.0, OBJ_TRUCK);

    print_str("\nAdding static obstacles...\n");
    av_add_obstacle(25.0, 8.0, 2.0);
    av_add_obstacle(45.0, 5.0, 1.5);
    av_add_obstacle(60.0, 0.0, 2.5);

    print_str("\nPerception...\n");
    av_perception();

    print_str("\nLocalization methods...\n");
    av_localization(LOC_GPS);
    av_localization(LOC_VISUAL);
    av_localization(LOC_LIDAR);
    av_localization(LOC_FUSED);

    print_str("\nPrediction...\n");
    av_prediction();

    print_str("\nPath planning (A*)...\n");
    av_path_planning(PLAN_ASTAR);

    print_str("\nPath planning (RRT)...\n");
    av_path_planning(PLAN_RRT);

    print_str("\nPath planning (APF)...\n");
    av_path_planning(PLAN_APF);

    print_str("\nControl methods...\n");
    av_update_ego(0.1);
    av_control(CTRL_PID);
    av_control(CTRL_PURE);
    av_control(CTRL_STAN);
    av_control(CTRL_LQR);
    av_control(CTRL_MPC);

    print_str("\nCollision check...\n");
    av_collision_check();

    print_str("\nDecision making...\n");
    av_decision_making();

    for (int i = 0; i < 5; i++) av_update_ego(0.1);

    av_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
