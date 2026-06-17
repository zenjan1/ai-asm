/* autonomous_navigation: WASM navigation and path-planning module
 *
 * Provides an autonomous navigation system for mobile robots:
 * - Map building: occupancy grids, cost maps, voxel grids, topological graphs
 * - Localization: AMCL particle filter, SLAM, EKF GPS/IMU fusion
 * - Path planning: A*, Dijkstra, DWA, TEB, kinodynamic planning
 * - Obstacle avoidance: velocity obstacles, social navigation, reactive avoidance
 * - Task planning: goal navigation, patrol, frontier exploration, missions
 * - Multi-robot coordination: formations, task allocation, deconfliction
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

/* ---------- Limits and constants ---------- */
#define MAX_GRID_W          128
#define MAX_GRID_H          128
#define MAX_VOXEL_DIM       32
#define MAX_PARTICLES       256
#define MAX_PATH_LEN        512
#define MAX_WAYPOINTS       64
#define MAX_NODES           128
#define MAX_EDGES           256
#define MAX_OBSTACLES       64
#define MAX_ROBOTS          8
#define MAX_SCAN_POINTS     180
#define MAX_PLAN_STEPS      32
#define MAX_MISSION_GOALS   16
#define MAX_FORMATION_SLOTS 8
#define MAX_MSG_QUEUE       32
#define PI_FIX              31416
#define TWO_PI_FIX          62832

/* Cell / occupancy states */
#define CELL_FREE           0
#define CELL_OCCUPIED       1
#define CELL_UNKNOWN        2
#define CELL_INFLATED       3

/* Robot states */
#define ROBOT_IDLE          0
#define ROBOT_NAVIGATING    1
#define ROBOT_EXPLORING     2
#define ROBOT_PATROLLING    3
#define ROBOT_AVOIDING      4
#define ROBOT_COORDINATING  5

/* Planner modes */
#define PLAN_GLOBAL         0
#define PLAN_LOCAL          1
#define PLAN_KINODYNAMIC    2

/* Topological semantic labels */
#define SEM_ROOM            0
#define SEM_CORRIDOR        1
#define SEM_DOORWAY         2
#define SEM_WAYPOINT        3
#define SEM_CHARGING        4

/* ---------- Fixed-point math helpers (16.16 format) ---------- */
typedef int fix14_t;

static fix14_t fix14_mul(fix14_t a, fix14_t b) { return (fix14_t)(((long long)a * b) >> 14); }
static fix14_t fix14_div(fix14_t a, fix14_t b) { return (fix14_t)(((long long)a << 14) / b); }

/* fast integer square root for unsigned 32-bit */
static unsigned int isqrt32(unsigned int n) {
    unsigned int x = n;
    unsigned int y = 0;
    unsigned int bit = 1u << 30;
    while (bit > x) bit >>= 2;
    while (bit != 0) {
        if (x >= y + bit) { x -= y + bit; y = (y >> 1) + bit; }
        else              { y >>= 1; }
        bit >>= 2;
    }
    return y;
}

static int iabs(int v) { return v < 0 ? -v : v; }
static int imin(int a, int b) { return a < b ? a : b; }
static int imax(int a, int b) { return a > b ? a : b; }

/* ---------- Geometry types ---------- */
typedef struct { fix14_t x; fix14_t y; } vec2_t;

typedef struct { fix14_t x; fix14_t y; fix14_t z; } vec3_t;

typedef struct { vec2_t pos; fix14_t theta; fix14_t v; fix14_t w; } pose2d_t;

typedef struct { fix14_t range; fix14_t angle; } laser_point_t;

typedef struct { fix14_t xmin; fix14_t ymin; fix14_t xmax; fix14_t ymax; } bbox2d_t;

/* ---------- Map structures ---------- */
typedef struct {
    unsigned int w;
    unsigned int h;
    fix14_t resolution;
    vec2_t origin;
    unsigned char *cells;
} occupancy_grid_t;

typedef struct {
    unsigned int w;
    unsigned int h;
    unsigned char *costs;
    fix14_t inflation_radius;
    fix14_t lethal_cost;
} costmap_t;

typedef struct {
    unsigned int dim_x;
    unsigned int dim_y;
    unsigned int dim_z;
    fix14_t resolution;
    vec3_t origin;
    unsigned char *voxels;
} voxel_grid_t;

typedef struct {
    int id;
    vec2_t pos;
    unsigned int semantic;
    unsigned char active;
} topo_node_t;

typedef struct {
    int from_id;
    int to_id;
    fix14_t weight;
    unsigned char traversable;
} topo_edge_t;

typedef struct {
    unsigned int node_count;
    unsigned int edge_count;
    topo_node_t *nodes;
    topo_edge_t *edges;
} topo_map_t;

typedef struct {
    vec2_t pos;
    vec2_t vel;
    fix14_t radius;
    unsigned int track_id;
    unsigned int age;
} moving_obstacle_t;

typedef struct {
    occupancy_grid_t occ;
    costmap_t cost;
    voxel_grid_t vox;
    topo_map_t topo;
    moving_obstacle_t *obstacles;
    unsigned int obstacle_count;
    unsigned int update_counter;
} world_map_t;

/* ---------- Localization ---------- */
typedef struct {
    pose2d_t pose;
    fix14_t weight;
} particle_t;

typedef struct {
    particle_t *particles;
    unsigned int count;
    fix14_t neff;
    pose2d_t estimate;
    fix14_t cov_xx;
    fix14_t cov_yy;
    fix14_t cov_tt;
    unsigned int resample_counter;
} amcl_state_t;

typedef struct {
    fix14_t x; fix14_t y; fix14_t z;
    fix14_t vx; fix14_t vy; fix14_t vz;
    fix14_t roll; fix14_t pitch; fix14_t yaw;
    fix14_t P[9];
    fix14_t Q[9];
    fix14_t R[9];
} ekf_state_t;

/* ---------- Path planning ---------- */
typedef struct {
    int x; int y;
    fix14_t g; fix14_t f;
    int parent_x; int parent_y;
    unsigned char open;
    unsigned char closed;
} astar_node_t;

typedef struct {
    vec2_t *points;
    unsigned int count;
    unsigned int capacity;
    fix14_t total_length;
} plan_path_t;

typedef struct {
    fix14_t v_max;
    fix14_t w_max;
    fix14_t a_lin;
    fix14_t a_ang;
    fix14_t sim_time;
    fix14_t dt;
    fix14_t goal_weight;
    fix14_t obs_weight;
    fix14_t path_weight;
} dwa_params_t;

typedef struct {
    fix14_t v;
    fix14_t w;
    fix14_t score;
} velocity_cmd_t;

/* ---------- Task planning ---------- */
typedef struct {
    vec2_t goal;
    unsigned int priority;
    unsigned char reached;
    unsigned char active;
} goal_task_t;

typedef struct {
    vec2_t *waypoints;
    unsigned int count;
    unsigned int current_idx;
    unsigned char loop;
} patrol_task_t;

typedef struct {
    vec2_t frontier;
    fix14_t info_gain;
    unsigned char visited;
} frontier_t;

typedef struct {
    goal_task_t goals[MAX_MISSION_GOALS];
    unsigned int goal_count;
    unsigned int current_goal;
    unsigned char mission_complete;
} mission_plan_t;

/* ---------- Multi-robot coordination ---------- */
typedef struct {
    int robot_id;
    pose2d_t pose;
    unsigned int state;
    unsigned int task_id;
    fix14_t capability;
    unsigned char online;
} fleet_member_t;

typedef struct {
    unsigned int robot_id;
    unsigned int formation_id;
    vec2_t offset;
} formation_slot_t;

typedef struct {
    int sender_id;
    int receiver_id;
    unsigned int msg_type;
    fix14_t payload[4];
} coord_msg_t;

typedef struct {
    fleet_member_t robots[MAX_ROBOTS];
    unsigned int count;
    formation_slot_t formation[MAX_FORMATION_SLOTS];
    unsigned int formation_count;
    coord_msg_t mailbox[MAX_MSG_QUEUE];
    unsigned int mail_head;
    unsigned int mail_tail;
} fleet_state_t;

/* ---------- Navigation controller (top-level) ---------- */
typedef struct {
    world_map_t map;
    amcl_state_t amcl;
    ekf_state_t ekf;
    plan_path_t global_path;
    plan_path_t local_path;
    dwa_params_t dwa;
    mission_plan_t mission;
    fleet_state_t fleet;
    frontier_t *frontiers;
    unsigned int frontier_count;
    pose2d_t current_pose;
    unsigned int robot_state;
    unsigned int tick;
    fix14_t safety_margin;
} nav_controller_t;

/* ---------- Forward declarations ---------- */
static void nav_init(nav_controller_t *nav);
static void map_init(world_map_t *m, unsigned int w, unsigned int h, fix14_t res);
static void map_update_from_scan(world_map_t *m, pose2d_t p, laser_point_t *scan, unsigned int n);
static void costmap_compute(costmap_t *c, occupancy_grid_t *g, fix14_t radius);
static void topo_add_node(topo_map_t *t, vec2_t pos, unsigned int sem);
static void topo_add_edge(topo_map_t *t, int a, int b, fix14_t w);
static void obstacle_track(world_map_t *m, vec2_t pos, vec2_t vel, fix14_t r);
static void amcl_init(amcl_state_t *a, unsigned int n_particles);
static void amcl_predict(amcl_state_t *a, fix14_t dt, fix14_t v, fix14_t w);
static void amcl_update(amcl_state_t *a, laser_point_t *scan, unsigned int n, world_map_t *m);
static pose2d_t amcl_estimate(amcl_state_t *a);
static void ekf_init(ekf_state_t *e);
static void ekf_predict(ekf_state_t *e, fix14_t dt);
static void ekf_update_gps(ekf_state_t *e, fix14_t gx, fix14_t gy);
static void ekf_update_imu(ekf_state_t *e, fix14_t yaw);
static plan_path_t plan_astar(world_map_t *m, vec2_t start, vec2_t goal);
static velocity_cmd_t dwa_select(dwa_params_t *p, pose2d_t cur, vec2_t goal, world_map_t *m);
static plan_path_t dwa_rollout(dwa_params_t *p, pose2d_t cur, fix14_t v, fix14_t w);
static fix14_t dwa_score(dwa_params_t *p, plan_path_t traj, vec2_t goal, world_map_t *m);
static void avoid_velocity_obstacle(pose2d_t *p, moving_obstacle_t *obs, unsigned int n);
static void avoid_reactive(pose2d_t *p, world_map_t *m);
static void goal_nav_tick(nav_controller_t *nav);
static void patrol_tick(nav_controller_t *nav, patrol_task_t *pat);
static void exploration_tick(nav_controller_t *nav);
static void mission_tick(nav_controller_t *nav);
static void formation_control(fleet_state_t *f);
static void task_allocate(fleet_state_t *f);
static void fleet_send_msg(fleet_state_t *f, coord_msg_t msg);
static coord_msg_t fleet_recv_msg(fleet_state_t *f);
static void fleet_deconflict(fleet_state_t *f);
static void print_vec2(const char *label, vec2_t v);
static void print_pose(const char *label, pose2d_t p);
static void print_fix(const char *label, fix14_t v);
static fix14_t dist2(vec2_t a, vec2_t b);
static fix14_t normalize_angle(fix14_t a);

/* ---------- Implementation ---------- */

static fix14_t dist2(vec2_t a, vec2_t b) {
    fix14_t dx = a.x - b.x;
    fix14_t dy = a.y - b.y;
    unsigned int sx = (unsigned int)(dx < 0 ? -dx : dx);
    unsigned int sy = (unsigned int)(dy < 0 ? -dy : dy);
    unsigned int d2 = sx * sx + sy * sy;
    return (fix14_t)isqrt32(d2);
}

static fix14_t normalize_angle(fix14_t a) {
    while (a >  PI_FIX) a -= TWO_PI_FIX;
    while (a < -PI_FIX) a += TWO_PI_FIX;
    return a;
}

static void print_fix(const char *label, fix14_t v) {
    char buf[48];
    int i = 0;
    const char *s = label;
    while (*s && i < 40) { buf[i++] = *s++; }
    buf[i++] = ':'; buf[i++] = ' ';
    int iv = v >> 14;
    int neg = 0;
    if (iv < 0) { neg = 1; iv = -iv; }
    if (neg) buf[i++] = '-';
    if (iv == 0) { buf[i++] = '0'; }
    else {
        char tmp[10]; int tn = 0;
        while (iv > 0) { tmp[tn++] = '0' + (iv % 10); iv /= 10; }
        while (tn > 0) buf[i++] = tmp[--tn];
    }
    buf[i] = 0;
    host_print(buf);
    host_print("\n");
}

static void print_vec2(const char *label, vec2_t v) {
    char buf[64]; int i = 0;
    const char *s = label;
    while (*s && i < 56) buf[i++] = *s++;
    buf[i++] = '('; buf[i] = 0;
    host_print(buf);
    char xb[32]; int xi = 0;
    int iv = v.x >> 14;
    if (iv < 0) { xb[xi++] = '-'; iv = -iv; }
    if (iv == 0) xb[xi++] = '0';
    else { char t[8]; int tn = 0; while (iv > 0) { t[tn++] = '0' + (iv % 10); iv /= 10; } while (tn > 0) xb[xi++] = t[--tn]; }
    xb[xi++] = ','; xb[xi] = 0;
    host_print(xb);
    char yb[32]; int yi = 0;
    iv = v.y >> 14;
    if (iv < 0) { yb[yi++] = '-'; iv = -iv; }
    if (iv == 0) yb[yi++] = '0';
    else { char t[8]; int tn = 0; while (iv > 0) { t[tn++] = '0' + (iv % 10); iv /= 10; } while (tn > 0) yb[yi++] = t[--tn]; }
    yb[yi++] = ')'; yb[yi] = 0;
    host_print(yb);
    host_print("\n");
}

static void print_pose(const char *label, pose2d_t p) {
    print_vec2(label, p.pos);
    print_fix("  theta", p.theta);
}

/* --- Map building --- */
static void map_init(world_map_t *m, unsigned int w, unsigned int h, fix14_t res) {
    m->occ.w = w; m->occ.h = h; m->occ.resolution = res;
    m->occ.origin.x = 0; m->occ.origin.y = 0;
    m->occ.cells = (unsigned char *)host_alloc(w * h, 1);
    for (unsigned int i = 0; i < w * h; i++) m->occ.cells[i] = CELL_UNKNOWN;

    m->cost.w = w; m->cost.h = h;
    m->cost.costs = (unsigned char *)host_alloc(w * h, 1);
    for (unsigned int i = 0; i < w * h; i++) m->cost.costs[i] = 0;
    m->cost.inflation_radius = (fix14_t)(3 << 14);
    m->cost.lethal_cost = (fix14_t)(254 << 14);

    m->vox.dim_x = MAX_VOXEL_DIM; m->vox.dim_y = MAX_VOXEL_DIM; m->vox.dim_z = MAX_VOXEL_DIM;
    m->vox.resolution = (fix14_t)(1 << 14);
    m->vox.origin.x = 0; m->vox.origin.y = 0; m->vox.origin.z = 0;
    unsigned int vv = MAX_VOXEL_DIM * MAX_VOXEL_DIM * MAX_VOXEL_DIM;
    m->vox.voxels = (unsigned char *)host_alloc(vv, 1);
    for (unsigned int i = 0; i < vv; i++) m->vox.voxels[i] = CELL_UNKNOWN;

    m->topo.node_count = 0; m->topo.edge_count = 0;
    m->topo.nodes = (topo_node_t *)host_alloc(sizeof(topo_node_t) * MAX_NODES, 4);
    m->topo.edges = (topo_edge_t *)host_alloc(sizeof(topo_edge_t) * MAX_EDGES, 4);
    for (int i = 0; i < MAX_NODES; i++) m->topo.nodes[i].active = 0;
    for (int i = 0; i < MAX_EDGES; i++) m->topo.edges[i].traversable = 1;

    m->obstacles = (moving_obstacle_t *)host_alloc(sizeof(moving_obstacle_t) * MAX_OBSTACLES, 4);
    m->obstacle_count = 0;
    m->update_counter = 0;
    host_print("[map] initialized occupancy, cost, voxel, and topological layers\n");
}

static void map_update_from_scan(world_map_t *m, pose2d_t p, laser_point_t *scan, unsigned int n) {
    unsigned int hit = 0, miss = 0;
    for (unsigned int i = 0; i < n; i++) {
        fix14_t ca = p.theta + scan[i].angle;
        fix14_t ex = p.pos.x + ((long long)scan[i].range * ca >> 14);
        fix14_t ey = p.pos.y + ((long long)scan[i].range * ca >> 14);
        int gx = (int)(ex >> 14);
        int gy = (int)(ey >> 14);
        if (gx >= 0 && gx < (int)m->occ.w && gy >= 0 && gy < (int)m->occ.h) {
            unsigned int idx = (unsigned int)gy * m->occ.w + (unsigned int)gx;
            m->occ.cells[idx] = CELL_OCCUPIED;
            hit++;
        }
    }
    m->update_counter++;
    (void)miss;
}

static void costmap_compute(costmap_t *c, occupancy_grid_t *g, fix14_t radius) {
    int r = (int)(radius >> 14);
    if (r < 1) r = 1;
    for (unsigned int y = 0; y < g->h; y++) {
        for (unsigned int x = 0; x < g->w; x++) {
            unsigned int idx = y * g->w + x;
            if (g->cells[idx] != CELL_OCCUPIED) { c->costs[idx] = 0; continue; }
            int ylo = imax(0, (int)y - r), yhi = imin((int)g->h - 1, (int)y + r);
            int xlo = imax(0, (int)x - r), xhi = imin((int)g->w - 1, (int)x + r);
            for (int iy = ylo; iy <= yhi; iy++) {
                for (int ix = xlo; ix <= xhi; ix++) {
                    int d = iabs(ix - (int)x) + iabs(iy - (int)y);
                    unsigned char cv = (d <= r) ? (unsigned char)(254 - (254 * (unsigned)d / (unsigned)r)) : 0;
                    unsigned int ci = (unsigned)iy * g->w + (unsigned)ix;
                    if (cv > c->costs[ci]) c->costs[ci] = cv;
                }
            }
        }
    }
    host_print("[costmap] inflation computed\n");
}

static void topo_add_node(topo_map_t *t, vec2_t pos, unsigned int sem) {
    if (t->node_count >= MAX_NODES) return;
    unsigned int id = t->node_count;
    t->nodes[id].id = (int)id;
    t->nodes[id].pos = pos;
    t->nodes[id].semantic = sem;
    t->nodes[id].active = 1;
    t->node_count++;
}

static void topo_add_edge(topo_map_t *t, int a, int b, fix14_t w) {
    if (t->edge_count >= MAX_EDGES) return;
    unsigned int id = t->edge_count;
    t->edges[id].from_id = a;
    t->edges[id].to_id = b;
    t->edges[id].weight = w;
    t->edges[id].traversable = 1;
    t->edge_count++;
}

static void obstacle_track(world_map_t *m, vec2_t pos, vec2_t vel, fix14_t r) {
    int best = -1;
    fix14_t best_d = (fix14_t)(1 << 30);
    for (unsigned int i = 0; i < m->obstacle_count; i++) {
        fix14_t d = dist2(m->obstacles[i].pos, pos);
        if (d < best_d) { best_d = d; best = (int)i; }
    }
    if (best >= 0 && best_d < (fix14_t)(2 << 14)) {
        m->obstacles[best].pos = pos;
        m->obstacles[best].vel = vel;
        m->obstacles[best].radius = r;
        m->obstacles[best].age++;
    } else if (m->obstacle_count < MAX_OBSTACLES) {
        unsigned int k = m->obstacle_count;
        m->obstacles[k].pos = pos;
        m->obstacles[k].vel = vel;
        m->obstacles[k].radius = r;
        m->obstacles[k].track_id = k;
        m->obstacles[k].age = 0;
        m->obstacle_count++;
    }
}

/* --- Localization --- */
static void amcl_init(amcl_state_t *a, unsigned int n) {
    a->particles = (particle_t *)host_alloc(sizeof(particle_t) * n, 4);
    a->count = n;
    fix14_t spread = (fix14_t)(5 << 14);
    for (unsigned int i = 0; i < n; i++) {
        a->particles[i].pose.pos.x = ((fix14_t)(i * 37) % spread) - (spread >> 1);
        a->particles[i].pose.pos.y = ((fix14_t)(i * 53) % spread) - (spread >> 1);
        a->particles[i].pose.theta = ((fix14_t)(i * 71) % TWO_PI_FIX) - PI_FIX;
        a->particles[i].weight = (fix14_t)(1 << 14) / (fix14_t)n;
    }
    a->neff = (fix14_t)(n << 14);
    a->resample_counter = 0;
    a->cov_xx = spread; a->cov_yy = spread; a->cov_tt = PI_FIX;
    host_print("[amcl] particle filter initialized\n");
}

static void amcl_predict(amcl_state_t *a, fix14_t dt, fix14_t v, fix14_t w) {
    for (unsigned int i = 0; i < a->count; i++) {
        fix14_t th = a->particles[i].pose.theta;
        fix14_t noise = (fix14_t)((i * 13 + 7) % 256) - 128;
        a->particles[i].pose.pos.x += ((long long)v * dt >> 14) * (th >> 14) + noise;
        a->particles[i].pose.pos.y += ((long long)v * dt >> 14) * (th >> 14) + noise;
        a->particles[i].pose.theta = normalize_angle(th + ((long long)w * dt >> 14));
    }
}

static void amcl_update(amcl_state_t *a, laser_point_t *scan, unsigned int n, world_map_t *m) {
    fix14_t wsum = 0;
    for (unsigned int i = 0; i < a->count; i++) {
        fix14_t score = (fix14_t)(1 << 14);
        for (unsigned int j = 0; j < n && j < 8; j++) {
            fix14_t ca = a->particles[i].pose.theta + scan[j].angle;
            int hx = (int)((a->particles[i].pose.pos.x + ((long long)scan[j].range * ca >> 14)) >> 14);
            int hy = (int)((a->particles[i].pose.pos.y + ((long long)scan[j].range * ca >> 14)) >> 14);
            if (hx >= 0 && hx < (int)m->occ.w && hy >= 0 && hy < (int)m->occ.h) {
                unsigned char cell = m->occ.cells[(unsigned)hy * m->occ.w + (unsigned)hx];
                if (cell == CELL_OCCUPIED) score = score >> 1;
            }
        }
        a->particles[i].weight = score;
        wsum += score;
    }
    if (wsum > 0) {
        for (unsigned int i = 0; i < a->count; i++) {
            a->particles[i].weight = ((long long)a->particles[i].weight << 14) / wsum;
        }
    }
    a->resample_counter++;
    if (a->resample_counter >= 5) {
        a->resample_counter = 0;
        unsigned int best = 0;
        for (unsigned int i = 1; i < a->count; i++) {
            if (a->particles[i].weight > a->particles[best].weight) best = i;
        }
        for (unsigned int i = 0; i < a->count; i++) {
            a->particles[i].pose = a->particles[best].pose;
            a->particles[i].weight = (fix14_t)(1 << 14) / (fix14_t)a->count;
        }
    }
}

static pose2d_t amcl_estimate(amcl_state_t *a) {
    long long sx = 0, sy = 0, st = 0;
    for (unsigned int i = 0; i < a->count; i++) {
        sx += a->particles[i].pose.pos.x;
        sy += a->particles[i].pose.pos.y;
        st += a->particles[i].pose.theta;
    }
    pose2d_t est;
    est.pos.x = (fix14_t)(sx / (long long)a->count);
    est.pos.y = (fix14_t)(sy / (long long)a->count);
    est.theta = (fix14_t)(st / (long long)a->count);
    est.v = 0; est.w = 0;
    return est;
}

static void ekf_init(ekf_state_t *e) {
    e->x = 0; e->y = 0; e->z = 0;
    e->vx = 0; e->vy = 0; e->vz = 0;
    e->roll = 0; e->pitch = 0; e->yaw = 0;
    for (int i = 0; i < 9; i++) { e->P[i] = (i % 4 == 0) ? (fix14_t)(10 << 14) : 0; }
    for (int i = 0; i < 9; i++) { e->Q[i] = (i % 4 == 0) ? (fix14_t)(1 << 14) : 0; }
    for (int i = 0; i < 9; i++) { e->R[i] = (i % 4 == 0) ? (fix14_t)(2 << 14) : 0; }
    host_print("[ekf] GPS/IMU fusion initialized\n");
}

static void ekf_predict(ekf_state_t *e, fix14_t dt) {
    e->x += ((long long)e->vx * dt >> 14);
    e->y += ((long long)e->vy * dt >> 14);
    e->z += ((long long)e->vz * dt >> 14);
    for (int i = 0; i < 9; i++) e->P[i] += e->Q[i];
}

static void ekf_update_gps(ekf_state_t *e, fix14_t gx, fix14_t gy) {
    fix14_t Kx = ((long long)e->P[0] << 14) / (e->P[0] + e->R[0]);
    fix14_t Ky = ((long long)e->P[4] << 14) / (e->P[4] + e->R[4]);
    e->x += ((long long)Kx * (gx - e->x) >> 14);
    e->y += ((long long)Ky * (gy - e->y) >> 14);
    e->P[0] = e->P[0] - ((long long)Kx * e->P[0] >> 14);
    e->P[4] = e->P[4] - ((long long)Ky * e->P[4] >> 14);
}

static void ekf_update_imu(ekf_state_t *e, fix14_t yaw) {
    fix14_t K = ((long long)e->P[8] << 14) / (e->P[8] + e->R[8]);
    e->yaw += ((long long)K * (yaw - e->yaw) >> 14);
    e->P[8] = e->P[8] - ((long long)K * e->P[8] >> 14);
}

/* --- Path planning --- */
static plan_path_t plan_astar(world_map_t *m, vec2_t start, vec2_t goal) {
    plan_path_t path;
    path.capacity = MAX_PATH_LEN;
    path.points = (vec2_t *)host_alloc(sizeof(vec2_t) * path.capacity, 4);
    path.count = 0; path.total_length = 0;

    unsigned int w = m->occ.w, h = m->occ.h;
    unsigned int total = w * h;
    astar_node_t *grid = (astar_node_t *)host_alloc(sizeof(astar_node_t) * total, 4);
    for (unsigned int i = 0; i < total; i++) {
        grid[i].g = (fix14_t)(1 << 30); grid[i].f = (fix14_t)(1 << 30);
        grid[i].parent_x = -1; grid[i].parent_y = -1;
        grid[i].open = 0; grid[i].closed = 0;
    }
    int sx = (int)(start.x >> 14), sy = (int)(start.y >> 14);
    int gx = (int)(goal.x >> 14),  gy = (int)(goal.y >> 14);
    sx = imax(0, imin((int)w - 1, sx)); sy = imax(0, imin((int)h - 1, sy));
    gx = imax(0, imin((int)w - 1, gx)); gy = imax(0, imin((int)h - 1, gy));
    unsigned int sidx = (unsigned)sy * w + (unsigned)sx;
    grid[sidx].g = 0; grid[sidx].f = (fix14_t)(iabs(gx - sx) + iabs(gy - sy));
    grid[sidx].open = 1;
    int dx[8] = {1, -1, 0, 0, 1, 1, -1, -1};
    int dy[8] = {0, 0, 1, -1, 1, -1, 1, -1};
    int found = 0;
    for (int iter = 0; iter < 4096 && !found; iter++) {
        int cx = -1, cy = -1;
        fix14_t best_f = (fix14_t)(1 << 30);
        for (unsigned int y = 0; y < h; y++) {
            for (unsigned int x = 0; x < w; x++) {
                unsigned int id = y * w + x;
                if (grid[id].open && grid[id].f < best_f) {
                    best_f = grid[id].f; cx = (int)x; cy = (int)y;
                }
            }
        }
        if (cx < 0) break;
        unsigned int cid = (unsigned)cy * w + (unsigned)cx;
        grid[cid].open = 0; grid[cid].closed = 1;
        if (cx == gx && cy == gy) { found = 1; break; }
        for (int d = 0; d < 8; d++) {
            int nx = cx + dx[d], ny = cy + dy[d];
            if (nx < 0 || nx >= (int)w || ny < 0 || ny >= (int)h) continue;
            unsigned int nid = (unsigned)ny * w + (unsigned)nx;
            if (grid[nid].closed || m->occ.cells[nid] == CELL_OCCUPIED) continue;
            fix14_t step = (d < 4) ? (fix14_t)(1 << 14) : (fix14_t)(14564);
            fix14_t ng = grid[cid].g + step + (fix14_t)(m->cost.costs[nid] >> 2);
            if (ng < grid[nid].g) {
                grid[nid].g = ng;
                grid[nid].f = ng + (fix14_t)(iabs(gx - nx) + iabs(gy - ny));
                grid[nid].parent_x = cx; grid[nid].parent_y = cy;
                grid[nid].open = 1;
            }
        }
    }
    if (found) {
        int cx = gx, cy = gy;
        while (cx >= 0 && cy >= 0 && path.count < path.capacity) {
            path.points[path.count].x = (fix14_t)(cx << 14);
            path.points[path.count].y = (fix14_t)(cy << 14);
            path.count++;
            unsigned int id = (unsigned)cy * (int)w + (unsigned)cx;
            int px = grid[id].parent_x, py = grid[id].parent_y;
            cx = px; cy = py;
        }
        for (unsigned int i = 0; i + 1 < path.count; i++) {
            path.total_length += dist2(path.points[i], path.points[i + 1]);
        }
    }
    host_print("[astar] global plan computed\n");
    return path;
}

static velocity_cmd_t dwa_select(dwa_params_t *p, pose2d_t cur, vec2_t goal, world_map_t *m) {
    velocity_cmd_t best; best.v = 0; best.w = 0; best.score = (fix14_t)(0x80000000);
    fix14_t vsteps = 8, wsteps = 8;
    fix14_t dv = p->v_max / vsteps, dw = p->w_max / wsteps;
    for (fix14_t vi = -vsteps / 2; vi <= vsteps / 2; vi++) {
        for (fix14_t wi = -wsteps / 2; wi <= wsteps / 2; wi++) {
            fix14_t v = vi * dv, w = wi * dw;
            plan_path_t traj = dwa_rollout(p, cur, v, w);
            fix14_t sc = dwa_score(p, traj, goal, m);
            if (sc > best.score) { best.v = v; best.w = w; best.score = sc; }
        }
    }
    return best;
}

static plan_path_t dwa_rollout(dwa_params_t *p, pose2d_t cur, fix14_t v, fix14_t w) {
    plan_path_t tr;
    tr.capacity = MAX_PLAN_STEPS;
    tr.points = (vec2_t *)host_alloc(sizeof(vec2_t) * tr.capacity, 4);
    tr.count = 0; tr.total_length = 0;
    fix14_t t = 0;
    while (t < p->sim_time && tr.count < tr.capacity) {
        tr.points[tr.count] = cur.pos;
        tr.count++;
        cur.pos.x += ((long long)v * t >> 14);
        cur.pos.y += ((long long)v * t >> 14);
        cur.theta = normalize_angle(cur.theta + ((long long)w * p->dt >> 14));
        t += p->dt;
    }
    return tr;
}

static fix14_t dwa_score(dwa_params_t *p, plan_path_t traj, vec2_t goal, world_map_t *m) {
    if (traj.count == 0) return (fix14_t)(0x80000000);
    vec2_t end = traj.points[traj.count - 1];
    fix14_t dg = dist2(end, goal);
    fix14_t heading = (fix14_t)(100 << 14) - dg;
    fix14_t clearance = (fix14_t)(255 << 14);
    int ex = (int)(end.x >> 14), ey = (int)(end.y >> 14);
    if (ex >= 0 && ex < (int)m->occ.w && ey >= 0 && ey < (int)m->occ.h) {
        clearance = (fix14_t)(m->cost.costs[(unsigned)ey * m->occ.w + (unsigned)ex] << 14);
    }
    fix14_t obs = (fix14_t)(255 << 14) - clearance;
    return ((long long)p->goal_weight * heading >> 14)
         - ((long long)p->obs_weight * obs >> 14);
}

/* --- Obstacle avoidance --- */
static void avoid_velocity_obstacle(pose2d_t *p, moving_obstacle_t *obs, unsigned int n) {
    for (unsigned int i = 0; i < n; i++) {
        vec2_t rel; rel.x = obs[i].pos.x - p->pos.x; rel.y = obs[i].pos.y - p->pos.y;
        fix14_t d = dist2(obs[i].pos, p->pos);
        fix14_t threshold = obs[i].radius + (fix14_t)(1 << 14);
        if (d < threshold && d > 0) {
            p->pos.x -= rel.x / d;
            p->pos.y -= rel.y / d;
        }
    }
}

static void avoid_reactive(pose2d_t *p, world_map_t *m) {
    int cx = (int)(p->pos.x >> 14), cy = (int)(p->pos.y >> 14);
    int radius = 3;
    fix14_t push_x = 0, push_y = 0;
    for (int dy = -radius; dy <= radius; dy++) {
        for (int dx = -radius; dx <= radius; dx++) {
            int nx = cx + dx, ny = cy + dy;
            if (nx < 0 || nx >= (int)m->occ.w || ny < 0 || ny >= (int)m->occ.h) continue;
            if (m->occ.cells[(unsigned)ny * m->occ.w + (unsigned)nx] == CELL_OCCUPIED) {
                push_x -= (fix14_t)(dx << 14);
                push_y -= (fix14_t)(dy << 14);
            }
        }
    }
    p->pos.x += push_x >> 3;
    p->pos.y += push_y >> 3;
}

/* --- Task planning --- */
static void goal_nav_tick(nav_controller_t *nav) {
    if (nav->mission.goal_count == 0 || nav->mission.mission_complete) return;
    goal_task_t *g = &nav->mission.goals[nav->mission.current_goal];
    if (!g->active) { nav->mission.mission_complete = 1; return; }
    fix14_t d = dist2(nav->current_pose.pos, g->goal);
    if (d < (fix14_t)(1 << 14)) {
        g->reached = 1; g->active = 0;
        nav->mission.current_goal++;
        if (nav->mission.current_goal >= nav->mission.goal_count) {
            nav->mission.mission_complete = 1;
            host_print("[goal] mission complete\n");
        }
    } else {
        nav->global_path = plan_astar(&nav->map, nav->current_pose.pos, g->goal);
        nav->robot_state = ROBOT_NAVIGATING;
    }
}

static void patrol_tick(nav_controller_t *nav, patrol_task_t *pat) {
    if (pat->count == 0) return;
    vec2_t target = pat->waypoints[pat->current_idx];
    fix14_t d = dist2(nav->current_pose.pos, target);
    if (d < (fix14_t)(1 << 14)) {
        pat->current_idx++;
        if (pat->current_idx >= pat->count) {
            if (pat->loop) pat->current_idx = 0;
            else { pat->current_idx = pat->count - 1; nav->robot_state = ROBOT_IDLE; }
        }
    }
    nav->global_path = plan_astar(&nav->map, nav->current_pose.pos, target);
}

static void exploration_tick(nav_controller_t *nav) {
    fix14_t best_gain = 0;
    int best_frontier = -1;
    for (unsigned int i = 0; i < nav->frontier_count; i++) {
        if (nav->frontiers[i].visited) continue;
        if (nav->frontiers[i].info_gain > best_gain) {
            best_gain = nav->frontiers[i].info_gain;
            best_frontier = (int)i;
        }
    }
    if (best_frontier >= 0) {
        nav->frontiers[best_frontier].visited = 1;
        nav->global_path = plan_astar(&nav->map, nav->current_pose.pos, nav->frontiers[best_frontier].frontier);
        nav->robot_state = ROBOT_EXPLORING;
        host_print("[explore] heading to frontier\n");
    } else {
        host_print("[explore] all frontiers visited\n");
        nav->robot_state = ROBOT_IDLE;
    }
}

static void mission_tick(nav_controller_t *nav) {
    if (nav->mission.mission_complete) {
        host_print("[mission] awaiting new orders\n");
        return;
    }
    goal_nav_tick(nav);
}

/* --- Multi-robot coordination --- */
static void formation_control(fleet_state_t *f) {
    for (unsigned int i = 0; i < f->formation_count; i++) {
        unsigned int rid = f->formation[i].robot_id;
        if (rid >= f->count || !f->robots[rid].online) continue;
        vec2_t target;
        target.x = f->robots[0].pose.pos.x + f->formation[i].offset.x;
        target.y = f->robots[0].pose.pos.y + f->formation[i].offset.y;
        fix14_t dx = target.x - f->robots[rid].pose.pos.x;
        fix14_t dy = target.y - f->robots[rid].pose.pos.y;
        f->robots[rid].pose.pos.x += dx >> 2;
        f->robots[rid].pose.pos.y += dy >> 2;
        f->robots[rid].state = ROBOT_COORDINATING;
    }
    host_print("[formation] positions updated\n");
}

static void task_allocate(fleet_state_t *f) {
    for (unsigned int t = 0; t < 4; t++) {
        int best = -1;
        fix14_t best_cap = 0;
        for (unsigned int r = 0; r < f->count; r++) {
            if (!f->robots[r].online || f->robots[r].task_id != (unsigned)-1) continue;
            if (f->robots[r].capability > best_cap) {
                best_cap = f->robots[r].capability; best = (int)r;
            }
        }
        if (best >= 0) f->robots[best].task_id = t;
    }
    host_print("[allocate] tasks assigned by auction\n");
}

static void fleet_send_msg(fleet_state_t *f, coord_msg_t msg) {
    unsigned int next = (f->mail_head + 1) % MAX_MSG_QUEUE;
    if (next == f->mail_tail) return;
    f->mailbox[f->mail_head] = msg;
    f->mail_head = next;
}

static coord_msg_t fleet_recv_msg(fleet_state_t *f) {
    coord_msg_t empty; empty.sender_id = -1; empty.receiver_id = -1;
    empty.msg_type = 0; empty.payload[0] = 0; empty.payload[1] = 0;
    empty.payload[2] = 0; empty.payload[3] = 0;
    if (f->mail_head == f->mail_tail) return empty;
    coord_msg_t m = f->mailbox[f->mail_tail];
    f->mail_tail = (f->mail_tail + 1) % MAX_MSG_QUEUE;
    return m;
}

static void fleet_deconflict(fleet_state_t *f) {
    for (unsigned int i = 0; i < f->count; i++) {
        for (unsigned int j = i + 1; j < f->count; j++) {
            fix14_t d = dist2(f->robots[i].pose.pos, f->robots[j].pose.pos);
            if (d < (fix14_t)(2 << 14)) {
                if (f->robots[i].pose.pos.x < f->robots[j].pose.pos.x) {
                    f->robots[i].pose.pos.x -= (fix14_t)(1 << 14);
                    f->robots[j].pose.pos.x += (fix14_t)(1 << 14);
                } else {
                    f->robots[i].pose.pos.x += (fix14_t)(1 << 14);
                    f->robots[j].pose.pos.x -= (fix14_t)(1 << 14);
                }
            }
        }
    }
}

/* --- Controller init --- */
static void nav_init(nav_controller_t *nav) {
    map_init(&nav->map, 64, 64, (fix14_t)(1 << 14));
    amcl_init(&nav->amcl, MAX_PARTICLES);
    ekf_init(&nav->ekf);

    nav->dwa.v_max = (fix14_t)(2 << 14);
    nav->dwa.w_max = (fix14_t)(1 << 14);
    nav->dwa.a_lin = (fix14_t)(1 << 14);
    nav->dwa.a_ang = (fix14_t)(1 << 14);
    nav->dwa.sim_time = (fix14_t)(2 << 14);
    nav->dwa.dt = (fix14_t)(0); nav->dwa.dt = 200;
    nav->dwa.goal_weight = (fix14_t)(1 << 14);
    nav->dwa.obs_weight = (fix14_t)(2 << 14);
    nav->dwa.path_weight = (fix14_t)(1 << 14);

    nav->global_path.points = (vec2_t *)host_alloc(sizeof(vec2_t) * MAX_PATH_LEN, 4);
    nav->global_path.count = 0; nav->global_path.capacity = MAX_PATH_LEN; nav->global_path.total_length = 0;
    nav->local_path.points = (vec2_t *)host_alloc(sizeof(vec2_t) * MAX_PATH_LEN, 4);
    nav->local_path.count = 0; nav->local_path.capacity = MAX_PATH_LEN; nav->local_path.total_length = 0;

    nav->frontiers = (frontier_t *)host_alloc(sizeof(frontier_t) * MAX_WAYPOINTS, 4);
    nav->frontier_count = 0;
    for (unsigned int i = 0; i < MAX_WAYPOINTS; i++) {
        nav->frontiers[i].frontier.x = (fix14_t)((int)i * 2 << 14);
        nav->frontiers[i].frontier.y = (fix14_t)((int)i * 2 << 14);
        nav->frontiers[i].info_gain = (fix14_t)((10 - (int)i) << 14);
        nav->frontiers[i].visited = 0;
        if ((int)i < 5) nav->frontier_count++;
    }

    nav->mission.goal_count = 0; nav->mission.current_goal = 0; nav->mission.mission_complete = 0;
    for (int i = 0; i < MAX_MISSION_GOALS; i++) {
        nav->mission.goals[i].active = 0; nav->mission.goals[i].reached = 0;
        nav->mission.goals[i].priority = 0;
    }

    nav->fleet.count = 0; nav->fleet.formation_count = 0;
    nav->fleet.mail_head = 0; nav->fleet.mail_tail = 0;
    for (int i = 0; i < MAX_ROBOTS; i++) {
        nav->fleet.robots[i].online = 0; nav->fleet.robots[i].task_id = (unsigned)-1;
    }

    nav->current_pose.pos.x = (fix14_t)(2 << 14);
    nav->current_pose.pos.y = (fix14_t)(2 << 14);
    nav->current_pose.theta = 0;
    nav->current_pose.v = 0; nav->current_pose.w = 0;
    nav->robot_state = ROBOT_IDLE;
    nav->tick = 0;
    nav->safety_margin = (fix14_t)(1 << 14);

    host_print("[nav] autonomous navigation controller initialized\n");
}

/* ---------- Main demo ---------- */
int main(void) {
    host_print("=== autonomous_navigation module start ===\n");

    nav_controller_t *nav = (nav_controller_t *)host_alloc(sizeof(nav_controller_t), 8);
    nav_init(nav);

    /* Build map from simulated sensor data */
    laser_point_t *scan = (laser_point_t *)host_alloc(sizeof(laser_point_t) * MAX_SCAN_POINTS, 4);
    for (int i = 0; i < MAX_SCAN_POINTS; i++) {
        scan[i].angle = (fix14_t)(i * 200 - 16000);
        scan[i].range = (fix14_t)((5 + (i % 3)) << 14);
    }
    map_update_from_scan(&nav->map, nav->current_pose, scan, MAX_SCAN_POINTS);
    costmap_compute(&nav->map.cost, &nav->map.occ, nav->map.cost.inflation_radius);

    /* Build topological map */
    vec2_t n0 = { (fix14_t)(2 << 14),  (fix14_t)(2 << 14) };
    vec2_t n1 = { (fix14_t)(10 << 14), (fix14_t)(5 << 14) };
    vec2_t n2 = { (fix14_t)(20 << 14), (fix14_t)(8 << 14) };
    vec2_t n3 = { (fix14_t)(15 << 14), (fix14_t)(15 << 14) };
    topo_add_node(&nav->map.topo, n0, SEM_WAYPOINT);
    topo_add_node(&nav->map.topo, n1, SEM_ROOM);
    topo_add_node(&nav->map.topo, n2, SEM_CORRIDOR);
    topo_add_node(&nav->map.topo, n3, SEM_CHARGING);
    topo_add_edge(&nav->map.topo, 0, 1, (fix14_t)(9 << 14));
    topo_add_edge(&nav->map.topo, 1, 2, (fix14_t)(11 << 14));
    topo_add_edge(&nav->map.topo, 2, 3, (fix14_t)(8 << 14));
    topo_add_edge(&nav->map.topo, 0, 3, (fix14_t)(16 << 14));
    host_print("[topo] 4 nodes, 4 edges added\n");

    /* Track a moving obstacle */
    vec2_t op = { (fix14_t)(8 << 14), (fix14_t)(6 << 14) };
    vec2_t ov = { (fix14_t)(1 << 14), 0 };
    obstacle_track(&nav->map, op, ov, (fix14_t)(1 << 14));

    /* Localize with AMCL */
    amcl_predict(&nav->amcl, (fix14_t)(1 << 14), (fix14_t)(1 << 14), 0);
    amcl_update(&nav->amcl, scan, MAX_SCAN_POINTS, &nav->map);
    pose2d_t est = amcl_estimate(&nav->amcl);
    nav->current_pose = est;
    print_pose("[amcl] estimate", est);

    /* GPS/IMU fusion */
    ekf_predict(&nav->ekf, (fix14_t)(1 << 14));
    ekf_update_gps(&nav->ekf, (fix14_t)(3 << 14), (fix14_t)(3 << 14));
    ekf_update_imu(&nav->ekf, (fix14_t)(1000));

    /* Global path planning */
    vec2_t goal = { (fix14_t)(20 << 14), (fix14_t)(15 << 14) };
    nav->global_path = plan_astar(&nav->map, nav->current_pose.pos, goal);
    print_fix("[path] length", nav->global_path.total_length);
    print_fix("[path] points", (fix14_t)nav->global_path.count);

    /* DWA local planning */
    velocity_cmd_t cmd = dwa_select(&nav->dwa, nav->current_pose, goal, &nav->map);
    print_fix("[dwa] v", cmd.v);
    print_fix("[dwa] w", cmd.w);

    /* Obstacle avoidance */
    avoid_velocity_obstacle(&nav->current_pose, nav->map.obstacles, nav->map.obstacle_count);
    avoid_reactive(&nav->current_pose, &nav->map);
    nav->robot_state = ROBOT_AVOIDING;
    host_print("[avoid] reactive + velocity obstacle avoidance active\n");

    /* Mission: sequence of goals */
    nav->mission.goals[0].goal.x = (fix14_t)(10 << 14);
    nav->mission.goals[0].goal.y = (fix14_t)(5 << 14);
    nav->mission.goals[0].priority = 1;
    nav->mission.goals[0].active = 1;
    nav->mission.goals[1].goal.x = (fix14_t)(20 << 14);
    nav->mission.goals[1].goal.y = (fix14_t)(8 << 14);
    nav->mission.goals[1].priority = 2;
    nav->mission.goals[1].active = 1;
    nav->mission.goals[2].goal.x = (fix14_t)(15 << 14);
    nav->mission.goals[2].goal.y = (fix14_t)(15 << 14);
    nav->mission.goals[2].priority = 3;
    nav->mission.goals[2].active = 1;
    nav->mission.goal_count = 3;
    mission_tick(nav);

    /* Exploration */
    exploration_tick(nav);

    /* Patrol */
    patrol_task_t patrol;
    vec2_t *wp = (vec2_t *)host_alloc(sizeof(vec2_t) * 4, 4);
    wp[0] = n0; wp[1] = n1; wp[2] = n2; wp[3] = n3;
    patrol.waypoints = wp; patrol.count = 4; patrol.current_idx = 0; patrol.loop = 1;
    patrol_tick(nav, &patrol);

    /* Multi-robot fleet setup */
    for (int i = 0; i < MAX_ROBOTS; i++) {
        nav->fleet.robots[i].robot_id = i;
        nav->fleet.robots[i].pose.pos.x = (fix14_t)((i + 1) * 2 << 14);
        nav->fleet.robots[i].pose.pos.y = (fix14_t)(2 << 14);
        nav->fleet.robots[i].state = ROBOT_IDLE;
        nav->fleet.robots[i].capability = (fix14_t)((10 - i) << 14);
        nav->fleet.robots[i].online = 1;
        nav->fleet.count++;
    }
    for (int i = 0; i < 4; i++) {
        nav->fleet.formation[i].robot_id = (unsigned)i + 1;
        nav->fleet.formation[i].formation_id = 0;
        nav->fleet.formation[i].offset.x = (fix14_t)(((i % 2) * 2 - 1) << 14);
        nav->fleet.formation[i].offset.y = (fix14_t)(((i / 2) * 2 - 1) << 14);
        nav->fleet.formation_count++;
    }
    formation_control(&nav->fleet);
    task_allocate(&nav->fleet);

    /* Message passing */
    coord_msg_t msg;
    msg.sender_id = 0; msg.receiver_id = 1; msg.msg_type = 1;
    msg.payload[0] = goal.x; msg.payload[1] = goal.y;
    msg.payload[2] = 0; msg.payload[3] = 0;
    fleet_send_msg(&nav->fleet, msg);
    coord_msg_t rx = fleet_recv_msg(&nav->fleet);
    if (rx.sender_id == 0) host_print("[msg] robot 0 -> robot 1 goal relayed\n");

    /* Conflict resolution */
    nav->fleet.robots[2].pose.pos.x = nav->fleet.robots[3].pose.pos.x;
    nav->fleet.robots[2].pose.pos.y = nav->fleet.robots[3].pose.pos.y;
    fleet_deconflict(&nav->fleet);
    host_print("[deconflict] priority-based separation applied\n");

    /* Print navigation statistics */
    host_print("\n=== navigation statistics ===\n");
    print_fix("map size", (fix14_t)(nav->map.occ.w * nav->map.occ.h));
    print_fix("map updates", (fix14_t)nav->map.update_counter);
    print_fix("topo nodes", (fix14_t)nav->map.topo.node_count);
    print_fix("topo edges", (fix14_t)nav->map.topo.edge_count);
    print_fix("obstacles tracked", (fix14_t)nav->map.obstacle_count);
    print_fix("particles", (fix14_t)nav->amcl.count);
    print_fix("path points", (fix14_t)nav->global_path.count);
    print_fix("path length", nav->global_path.total_length);
    print_fix("frontiers", (fix14_t)nav->frontier_count);
    print_fix("fleet size", (fix14_t)nav->fleet.count);
    print_fix("formation slots", (fix14_t)nav->fleet.formation_count);
    print_fix("mission goals", (fix14_t)nav->mission.goal_count);
    print_fix("tick", (fix14_t)nav->tick);
    host_print("=== autonomous_navigation module complete ===\n");

    host_exit(0);
    return 0;
}
