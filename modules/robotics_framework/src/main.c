/* robotics_framework: robotics control and planning (v1.0) */

__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_ROBOTS 8
#define MAX_OBSTACLES 64
#define MAX_WAYPOINTS 128
#define MAX_FEATURES 256
#define MAX_SENSORS 16
#define MAX_TASKS 32
#define MAX_ROBOT_NAME 32
#define MAX_MAP_CELLS 512

#define ROBOT_IDLE 0
#define ROBOT_NAVIGATING 1
#define ROBOT_DOCKED 2
#define ROBOT_ERROR 3

#define PLANNER_RRT 0
#define PLANNER_ASTAR 1

#define SENSOR_IMU 0
#define SENSOR_GPS 1
#define SENSOR_LIDAR 2
#define SENSOR_CAMERA 3

#define FORM_LINE 0
#define FORM_CIRCLE 1
#define FORM_VEE 2

typedef struct { double x, y, z; } vec3_t;
typedef struct { double roll, pitch, yaw; } euler_t;
typedef struct { double x, y; int parent; double cost; } node_t;
typedef struct { double x, y, radius; } obs_t;
typedef struct { double x, y, occ; } cell_t;
typedef struct { double x, y, strength; } feat_t;
typedef struct { double p, i, d, dt, integ, prev; } pid_t;
typedef struct { int id, state, planner; vec3_t pos, vel; euler_t orient;
    char name[MAX_ROBOT_NAME]; int wp_idx, path_len; double battery; } robot_t;
typedef struct { int id, robot_id, priority, status; vec3_t target; char label[16]; } task_t;
typedef struct { int type, robot_id, active; double accuracy, rate; } sensor_t;
typedef struct { int count, formation; int ids[MAX_ROBOTS]; vec3_t center; double spacing; } form_t;
typedef struct { int w, h, count; cell_t cells[MAX_MAP_CELLS]; } occ_map_t;

static unsigned int heap_pos = 65536;
static robot_t robots[MAX_ROBOTS]; static int robot_count = 0;
static obs_t obstacles[MAX_OBSTACLES]; static int obs_count = 0;
static node_t waypoints[MAX_WAYPOINTS]; static int wp_count = 0;
static sensor_t sensors[MAX_SENSORS]; static int sensor_count = 0;
static task_t tasks[MAX_TASKS]; static int task_count = 0;
static occ_map_t g_map;
static feat_t features[MAX_FEATURES]; static int feat_count = 0;
static form_t formations[4]; static int form_count = 0;
static double mpc_horizon = 2.0, mpc_dt = 0.1; static int mpc_steps = 20;
static double rl_discount = 0.99, rl_lr = 0.001; static int rl_episodes = 0;

static unsigned int alloc(unsigned int n) {
    unsigned int p = heap_pos; heap_pos += n; heap_pos = (heap_pos + 15) & ~15u; return p; }
static void copy_to_mem(const char *s, unsigned int d, unsigned int l) {
    char *p = (char *)d; for (unsigned int i = 0; i < l; i++) p[i] = s[i]; }
static unsigned int my_strlen(const char *s) { unsigned int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return 1; a++; b++; } return (*a != *b); }
static void copy_str(char *d, const char *s, unsigned int m) {
    unsigned int i = 0; while (s[i] && i < m - 1) { d[i] = s[i]; i++; } d[i] = 0; }
static void print_str(const char *s) {
    unsigned int l = my_strlen(s), b = alloc(l + 1); copy_to_mem(s, b, l + 1); host_print(b, l); }
static void print_int(int n) {
    char b[16]; int i = 15, neg = 0; b[15] = 0;
    if (n < 0) { neg = 1; n = -n; } if (n == 0) b[--i] = '0';
    while (n > 0) { b[--i] = '0' + (n % 10); n /= 10; } if (neg) b[--i] = '-';
    unsigned int l = my_strlen(&b[i]), d = alloc(l + 1); copy_to_mem(&b[i], d, l + 1); host_print(d, l); }
static void print_dbl(double v) {
    int w = (int)v, f = (int)((v - (double)w) * 1000.0); if (f < 0) f = -f;
    print_int(w); print_str(".");
    char fb[4]; fb[0]='0'+(f/100); f%=100; fb[1]='0'+(f/10); f%=10; fb[2]='0'+f; fb[3]=0; print_str(fb); }

static double abs_d(double x) { return x < 0.0 ? -x : x; }
static double sqrt_a(double x) { if (x <= 0.0) return 0.0; double g = x;
    for (int i = 0; i < 20; i++) g = (g + x / g) * 0.5; return g; }
static double dist2d(double x1, double y1, double x2, double y2) {
    double dx = x2 - x1, dy = y2 - y1; return sqrt_a(dx * dx + dy * dy); }
static double clamp(double v, double lo, double hi) { return v < lo ? lo : v > hi ? hi : v; }

static int in_obs(double x, double y) {
    for (int i = 0; i < obs_count; i++) if (dist2d(x, y, obstacles[i].x, obstacles[i].y) < obstacles[i].radius) return 1;
    return 0; }
static int add_obs(double x, double y, double r) {
    if (obs_count >= MAX_OBSTACLES) return -1;
    obstacles[obs_count].x = x; obstacles[obs_count].y = y; obstacles[obs_count++].radius = r; return 0; }

static int plan_rrt(double sx, double sy, double gx, double gy, double step, int max_iter) {
    if (in_obs(sx, sy) || in_obs(gx, gy)) return -1;
    wp_count = 1; waypoints[0] = (node_t){sx, sy, -1, 0.0};
    for (int it = 0; it < max_iter && wp_count < MAX_WAYPOINTS - 1; it++) {
        double rx = sx + (double)(it * 37 % 200) / 10.0 - 10.0;
        double ry = sy + (double)(it * 53 % 200) / 10.0 - 10.0;
        int nr = 0; double md = dist2d(waypoints[0].x, waypoints[0].y, rx, ry);
        for (int j = 1; j < wp_count; j++) { double d = dist2d(waypoints[j].x, waypoints[j].y, rx, ry);
            if (d < md) { md = d; nr = j; } }
        double d = dist2d(waypoints[nr].x, waypoints[nr].y, rx, ry);
        if (d < 1e-9) continue;
        double nx = waypoints[nr].x + step * (rx - waypoints[nr].x) / d;
        double ny = waypoints[nr].y + step * (ry - waypoints[nr].y) / d;
        if (in_obs(nx, ny)) continue;
        waypoints[wp_count++] = (node_t){nx, ny, nr, waypoints[nr].cost + step};
        if (dist2d(nx, ny, gx, gy) < step * 2.0) {
            int prev = wp_count - 1;
            waypoints[wp_count++] = (node_t){gx, gy, prev,
                waypoints[prev].cost + dist2d(nx, ny, gx, gy)}; return 0; }
    } return -1; }

static int plan_astar(double sx, double sy, double gx, double gy, int gw, int gh) {
    if (gw <= 0 || gh <= 0) return -1;
    wp_count = 1; waypoints[0] = (node_t){sx, sy, -1, dist2d(sx, sy, gx, gy)};
    int vis[64], vc = 0, cur = 0;
    for (int s = 0; s < 64; s++) {
        double cx = waypoints[cur].x, cy = waypoints[cur].y;
        int best = -1; double bf = 1e18;
        for (int dy = -1; dy <= 1; dy++) for (int dx = -1; dx <= 1; dx++) {
            if (!dx && !dy) continue;
            double nx = cx + dx, ny = cy + dy;
            if (nx < 0 || ny < 0 || nx >= gw || ny >= gh || in_obs(nx, ny)) continue;
            int nid = (int)ny * gw + (int)nx, skip = 0;
            for (int v = 0; v < vc; v++) if (vis[v] == nid) { skip = 1; break; } if (skip) continue;
            double f = waypoints[cur].cost + dist2d(cx, cy, nx, ny) + dist2d(nx, ny, gx, gy);
            if (f < bf) { bf = f; best = wp_count;
                waypoints[wp_count] = (node_t){nx, ny, cur, waypoints[cur].cost + dist2d(cx, cy, nx, ny)}; }
        }
        if (best == -1) return -1;
        vis[vc++] = (int)waypoints[best].y * gw + (int)waypoints[best].x; cur = best;
        if (++wp_count >= MAX_WAYPOINTS) return -1;
        if (dist2d(waypoints[cur].x, waypoints[cur].y, gx, gy) < 1.5) {
            waypoints[wp_count++] = (node_t){gx, gy, cur,
                waypoints[cur].cost + dist2d(waypoints[cur].x, waypoints[cur].y, gx, gy)}; return 0; }
    } return -1; }

static int extract_feats(double *scan, int n, double thr) {
    int f = 0;
    for (int i = 1; i < n - 1 && feat_count < MAX_FEATURES; i++) {
        double d = abs_d(scan[i] - scan[i - 1]);
        if (d > thr) { features[feat_count++] = (feat_t){(double)i, scan[i], d}; f++; }
    } return f; }

static int merge_maps(const occ_map_t *o) {
    int m = 0;
    for (int i = 0; i < o->count && g_map.count < MAX_MAP_CELLS; i++) {
        int found = 0;
        for (int j = 0; j < g_map.count; j++)
            if (abs_d(g_map.cells[j].x - o->cells[i].x) < 0.5 &&
                abs_d(g_map.cells[j].y - o->cells[i].y) < 0.5) {
                g_map.cells[j].occ = (g_map.cells[j].occ + o->cells[i].occ) * 0.5; found = 1; m++; break; }
        if (!found) g_map.cells[g_map.count++] = o->cells[i];
    } return m; }

static void slam_update(int rid, double px, double py, double *scan, int n) {
    (void)rid; extract_feats(scan, n, 0.5);
    for (int i = 0; i < n && g_map.count < MAX_MAP_CELLS; i++)
        g_map.cells[g_map.count++] = (cell_t){px + scan[i] * 0.1 * (double)(i % 10 - 5),
            py + scan[i] * 0.1 * (double)(i / 10 - 5), 0.5 + scan[i] * 0.01}; }

static int create_robot(const char *name, double x, double y) {
    if (robot_count >= MAX_ROBOTS) return -1;
    robot_t *r = &robots[robot_count];
    r->id = robot_count; r->state = ROBOT_IDLE; r->planner = PLANNER_RRT;
    r->pos = (vec3_t){x, y, 0}; r->vel = (vec3_t){0, 0, 0};
    r->orient = (euler_t){0, 0, 0}; r->wp_idx = 0; r->path_len = 0; r->battery = 100.0;
    copy_str(r->name, name, MAX_ROBOT_NAME); return robot_count++; }

static int assign_task(int rid, double tx, double ty, int pri) {
    if (task_count >= MAX_TASKS || rid < 0 || rid >= robot_count) return -1;
    tasks[task_count] = (task_t){task_count, rid, pri, 0, {tx, ty, 0}, "task"}; return task_count++; }

static int allocate_tasks(void) {
    int a = 0;
    for (int i = 0; i < task_count; i++) { if (tasks[i].status) continue;
        int best = -1; double bd = 1e18;
        for (int j = 0; j < robot_count; j++) { if (robots[j].state != ROBOT_IDLE) continue;
            double d = dist2d(robots[j].pos.x, robots[j].pos.y, tasks[i].target.x, tasks[i].target.y);
            if (d < bd) { bd = d; best = j; } }
        if (best >= 0) { tasks[i].robot_id = best; tasks[i].status = 1; a++; }
    } return a; }

static int create_form(int type, double cx, double cy, double sp) {
    if (form_count >= 4) return -1;
    formations[form_count] = (form_t){0, type, {0}, {cx, cy, 0}, sp}; return form_count++; }

static void update_form(int fi) {
    if (fi < 0 || fi >= form_count) return;
    form_t *f = &formations[fi]; f->count = 0;
    for (int i = 0; i < robot_count && f->count < MAX_ROBOTS; i++) {
        if (robots[i].state != ROBOT_IDLE && robots[i].state != ROBOT_NAVIGATING) continue;
        f->ids[f->count] = i; double ox = 0, oy = 0;
        if (f->formation == FORM_LINE) { ox = f->center.x + f->count * f->spacing; oy = f->center.y; }
        else if (f->formation == FORM_CIRCLE) { double a = 6.28318 * f->count / (robot_count > 0 ? robot_count : 1);
            ox = f->center.x + f->spacing * a; oy = f->center.y + f->spacing * (a - 3.14159); }
        else { ox = f->center.x + f->count * f->spacing;
            oy = f->center.y + abs_d((double)f->count - robot_count * 0.5) * f->spacing; }
        robots[i].pos.x = ox; robots[i].pos.y = oy; f->count++;
    } }

static int reg_sensor(int type, int rid, double acc, double rate) {
    if (sensor_count >= MAX_SENSORS) return -1;
    sensors[sensor_count] = (sensor_t){type, rid, 1, acc, rate}; return sensor_count++; }

static vec3_t fuse_pos(int rid, vec3_t imu, vec3_t gps, vec3_t lidar, vec3_t cam) {
    (void)rid; return (vec3_t){
        0.15*imu.x + 0.35*gps.x + 0.30*lidar.x + 0.20*cam.x,
        0.15*imu.y + 0.35*gps.y + 0.30*lidar.y + 0.20*cam.y,
        0.15*imu.z + 0.35*gps.z + 0.30*lidar.z + 0.20*cam.z}; }

static pid_t pid_mk(double kp, double ki, double kd, double dt) {
    return (pid_t){kp, ki, kd, dt, 0.0, 0.0}; }
static double pid_upd(pid_t *p, double sp, double m) {
    double e = sp - m; p->integ += e * p->dt; double d = (e - p->prev) / p->dt; p->prev = e;
    return p->p * e + p->i * p->integ + p->d * d; }

static double mpc_pred(double s, double *c, int n) {
    for (int i = 0; i < n && i < 32; i++) s = s * 0.95 + c[i] * mpc_dt; return s; }
static void mpc_opt(double tgt, double cur, double *c) {
    for (int it = 0; it < 10; it++) { double e = tgt - mpc_pred(cur, c, mpc_steps);
        for (int i = 0; i < mpc_steps && i < 32; i++) c[i] += 0.05 * e * mpc_dt; } }

static double rl_action(double state, int explore) {
    double a = explore ? (double)(rl_episodes * 17 % 100) / 50.0 - 1.0 : -0.5 * state;
    rl_episodes++; return a * clamp(state, -2.0, 2.0); }

static void physics_step(double dt) {
    for (int i = 0; i < robot_count; i++) {
        robots[i].pos.x += robots[i].vel.x * dt; robots[i].pos.y += robots[i].vel.y * dt;
        robots[i].pos.z += robots[i].vel.z * dt;
        robots[i].battery -= 0.01 * dt; if (robots[i].battery < 0) robots[i].battery = 0;
    } }
static void render(void) {
    print_str("  Scene: robots="); print_int(robot_count);
    print_str(" obs="); print_int(obs_count); print_str(" feats="); print_int(feat_count);
    print_str(" cells="); print_int(g_map.count); print_str("\n");
    for (int i = 0; i < robot_count; i++) { print_str("    ["); print_str(robots[i].name);
        print_str("] ("); print_dbl(robots[i].pos.x); print_str(","); print_dbl(robots[i].pos.y);
        print_str(") bat="); print_dbl(robots[i].battery); print_str("\n"); } }

static void show_help(void) { print_str("robotics_framework v1.0\n  -h help  -t test  -i info  -s status\n  -m motion  -S slam  -c control  -f formation\n"); }
static void show_info(void) { print_str("robotics_framework v1.0\n  Planning: RRT/A*  SLAM: features/merge\n  Multi-robot: tasks/formations\n  Sensors: IMU/GPS/LiDAR/Cam\n  Control: PID/MPC/RL  Sim: physics/3D\n"); }
static void show_status(void) { print_str("status: robots="); print_int(robot_count); print_str(" obs="); print_int(obs_count);
    print_str(" wp="); print_int(wp_count); print_str(" feats="); print_int(feat_count);
    print_str(" cells="); print_int(g_map.count); print_str(" sensors="); print_int(sensor_count);
    print_str(" tasks="); print_int(task_count); print_str(" forms="); print_int(form_count);
    print_str(" rl_ep="); print_int(rl_episodes); print_str("\n"); }

static void test_motion(void) {
    print_str("[motion] RRT+A* test\n"); obs_count = 0;
    add_obs(5, 5, 2); add_obs(10, 3, 1.5); add_obs(8, 8, 1);
    int r1 = plan_rrt(0, 0, 12, 10, 1.0, 500);
    print_str("  RRT: "); print_int(r1); print_str(" nodes="); print_int(wp_count); print_str("\n");
    int r2 = plan_astar(0, 0, 12, 10, 15, 12);
    print_str("  A*: "); print_int(r2); print_str(" nodes="); print_int(wp_count); print_str("\n");
    print_str(r1 == 0 || r2 == 0 ? "  OK\n" : "  FAIL\n"); }

static void test_slam(void) {
    print_str("[SLAM] test\n"); double scan[20];
    for (int i = 0; i < 20; i++) scan[i] = 1.0 + (i % 3) * 0.5;
    feat_count = 0; print_str("  features: "); print_int(extract_feats(scan, 20, 0.3)); print_str("\n");
    g_map.count = 0; slam_update(0, 0, 0, scan, 20);
    print_str("  cells: "); print_int(g_map.count); print_str("\n");
    occ_map_t o = {0, 0, 2, {{5, 5, 0.7}, {100, 100, 0.3}}};
    print_str("  merged: "); print_int(merge_maps(&o)); print_str("\n  OK\n"); }

static void test_multi(void) {
    print_str("[multi-robot] test\n"); robot_count = 0; task_count = 0; form_count = 0;
    create_robot("alpha", 0, 0); create_robot("beta", 1, 1); create_robot("gamma", 2, 2);
    print_str("  robots: "); print_int(robot_count); print_str("\n");
    assign_task(0, 10, 10, 1); assign_task(1, 5, 5, 2); assign_task(2, 8, 3, 3);
    print_str("  assigned: "); print_int(allocate_tasks()); print_str("\n");
    int f1 = create_form(FORM_LINE, 5, 5, 2); update_form(f1); print_str("  LINE ok\n");
    int f2 = create_form(FORM_CIRCLE, 10, 10, 3); update_form(f2); print_str("  CIRCLE ok\n  OK\n"); }

static void test_fusion(void) {
    print_str("[fusion] test\n"); sensor_count = 0;
    reg_sensor(SENSOR_IMU, 0, 0.01, 200); reg_sensor(SENSOR_GPS, 0, 2.0, 10);
    reg_sensor(SENSOR_LIDAR, 0, 0.05, 20); reg_sensor(SENSOR_CAMERA, 0, 0.5, 30);
    print_str("  sensors: "); print_int(sensor_count); print_str("\n");
    vec3_t f = fuse_pos(0, (vec3_t){1,2,0}, (vec3_t){1.5,2.5,0.1},
        (vec3_t){1.1,2.1,0}, (vec3_t){1.3,2.3,0.05});
    print_str("  fused: ("); print_dbl(f.x); print_str(","); print_dbl(f.y); print_str(","); print_dbl(f.z);
    print_str(")\n  OK\n"); }

static void test_ctrl(void) {
    print_str("[control] test\n");
    pid_t p = pid_mk(1.0, 0.1, 0.05, 0.01);
    print_str("  PID: "); print_dbl(pid_upd(&p, 10, 5)); print_str("\n");
    double c[32]; for (int i = 0; i < 32; i++) c[i] = 0;
    mpc_opt(5.0, 0.0, c); print_str("  MPC[0]: "); print_dbl(c[0]); print_str("\n");
    rl_episodes = 0;
    print_str("  RL exp: "); print_dbl(rl_action(1.0, 1)); print_str("\n");
    print_str("  RL expl: "); print_dbl(rl_action(1.0, 0)); print_str("\n  OK\n"); }

static void test_sim(void) {
    print_str("[simulation] test\n"); robot_count = 0;
    create_robot("sim1", 0, 0); create_robot("sim2", 5, 5);
    robots[0].vel = (vec3_t){1.0, 0.5, 0}; robots[1].vel = (vec3_t){-0.5, 1.0, 0};
    print_str("  before:\n"); render();
    for (int i = 0; i < 10; i++) physics_step(0.1);
    print_str("  after 10 steps:\n"); render(); print_str("  OK\n"); }

static void test_mode(int v) {
    print_str("robotics_framework: self-test\n");
    test_motion(); test_slam(); test_multi(); test_fusion(); test_ctrl(); test_sim();
    if (v) show_status(); print_str("robotics_framework: done\n"); }

void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    int argc = host_get_argv(buf, 512); (void)argc;
    int h = 0, i = 0, t = 0, s = 0, m = 0, sl = 0, c = 0, f = 0;
    char *ap = (char *)buf; unsigned int pos = 0;
    while (pos < 512 && ap[pos]) pos++; pos++;
    while (pos < 512 && ap[pos]) { char *a = &ap[pos];
        if (!my_strcmp(a, "-h") || !my_strcmp(a, "--help")) h = 1;
        else if (!my_strcmp(a, "-i")) i = 1; else if (!my_strcmp(a, "-t")) t = 1;
        else if (!my_strcmp(a, "-s")) s = 1; else if (!my_strcmp(a, "-m")) m = 1;
        else if (!my_strcmp(a, "-S")) sl = 1; else if (!my_strcmp(a, "-c")) c = 1;
        else if (!my_strcmp(a, "-f")) f = 1;
        while (pos < 512 && ap[pos]) pos++; pos++; }
    if (h) { show_help(); host_exit(0); } if (i) { show_info(); host_exit(0); }
    if (s) { show_status(); host_exit(0); } if (t) { test_mode(1); host_exit(0); }
    if (m) { test_motion(); host_exit(0); } if (sl) { test_slam(); host_exit(0); }
    if (c) { test_ctrl(); host_exit(0); } if (f) { test_multi(); host_exit(0); }
    print_str("robotics_framework: no action (use -h)\n"); host_exit(0); }
