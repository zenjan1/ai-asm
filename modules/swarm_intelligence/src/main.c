/* swarm_intelligence: distributed swarm coordination module for AI-ASM OS
 *
 * Implements Reynolds boids, ant colony optimization, particle swarm
 * optimization, bee foraging, fish schooling, consensus algorithms,
 * leader election, task allocation, and self-organization primitives.
 *
 * All memory is obtained via host_alloc.  All output uses host_print.
 * No external libraries.  Pure C targeting wasm32-unknown-unknown.
 */

#include <stddef.h>

/* ---------- host imports ---------- */

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* ---------- forward declarations ---------- */

typedef struct Agent Agent;
typedef struct Pheromone Pheromone;
typedef struct Swarm Swarm;
typedef struct Vec2  Vec2;

static Vec2  vec2_add(Vec2 a, Vec2 b);
static Vec2  vec2_sub(Vec2 a, Vec2 b);
static Vec2  vec2_scale(Vec2 v, float s);
static float vec2_len(Vec2 v);
static Vec2  vec2_norm(Vec2 v);
static float frand(unsigned int *seed);

static Agent *swarm_spawn(Swarm *s, float x, float y);
static void   swarm_step(Swarm *s, float dt);
static void   swarm_flocking(Swarm *s);
static void   swarm_pso_update(Swarm *s);
static void   swarm_aco_deposit(Swarm *s, unsigned int agent_id, float amount);
static float  swarm_aco_sample(Swarm *s, float x, float y);
static void   swarm_bee_waggle(Swarm *s, unsigned int agent_id, float quality);
static void   swarm_fish_school(Swarm *s);
static void   swarm_consensus_avg(Swarm *s);
static void   swarm_consensus_max(Swarm *s);
static int    swarm_leader_elect(Swarm *s);
static int    swarm_task_allocate(Swarm *s, float task_x, float task_y);
static void   swarm_self_repair(Swarm *s);
static void   swarm_self_adapt(Swarm *s);
static void   swarm_form_pattern(Swarm *s, int pattern, float cx, float cy, float r);
static void   swarm_communicate(Swarm *s);
static void   swarm_topology_repair(Swarm *s);
static void   swarm_gossip(Swarm *s);
static void   swarm_drone_formation(Swarm *s);
static void   swarm_warehouse_route(Swarm *s);
static void   swarm_search_cover(Swarm *s, float w, float h);
static float  swarm_sense_coverage(Swarm *s);
static void   swarm_print_stats(Swarm *s);
static void   print_f32(float v);
static void   print_u32(unsigned int v);
static void   print_sep(void);

/* ---------- constants ---------- */

#define MAX_AGENTS     64
#define MAX_PHEROMONES 32
#define GRID_W         16
#define GRID_H         16
#define COMM_RANGE     4.0f
#define SEP_RADIUS     1.0f
#define ALI_RADIUS     2.5f
#define COH_RADIUS     2.5f
#define MAX_SPEED      1.5f
#define SEP_WEIGHT     1.6f
#define ALI_WEIGHT     1.0f
#define COH_WEIGHT     0.9f
#define PSO_INERTIA    0.72f
#define PSO_COG        1.49f
#define PSO_SOC        1.49f
#define PHEROMONE_DECAY 0.92f
#define WAGGLE_DECAY   0.95f
#define FAILURE_THRESH 0.15f
#define ELECTION_RNDS  5
#define GOSSIP_ROUNDS  3

#define PATTERN_LINE   0
#define PATTERN_CIRCLE 1
#define PATTERN_GRID   2
#define PATTERN_VEE    3

#define PHASE_IDLE     0
#define PHASE_FLOCK    1
#define PHASE_ACO      2
#define PHASE_PSO      3
#define PHASE_FORM     4
#define PHASE_TASK     5

/* ---------- data structures ---------- */

struct Vec2 { float x; float y; };

struct Agent {
    unsigned int id;
    unsigned int alive;
    unsigned int role;
    unsigned int phase;
    unsigned int leader;
    unsigned int neighbors;
    float x;
    float y;
    float vx;
    float vy;
    float fitness;
    float best_x;
    float best_y;
    float best_fit;
    float opinion;
    float rank;
    float waggle;
    float energy;
    float trust;
    float task_x;
    float task_y;
    int   task_claim;
};

struct Pheromone {
    float x;
    float y;
    float intensity;
    unsigned int depositor;
};

struct Swarm {
    Agent *agents;
    unsigned int count;
    unsigned int capacity;
    Pheromone *pheromones;
    unsigned int phero_count;
    unsigned int phero_cap;
    float global_best_x;
    float global_best_y;
    float global_best_fit;
    float consensus;
    int   leader_id;
    unsigned int step;
    unsigned int failures_detected;
    unsigned int repairs_done;
    unsigned int tasks_allocated;
    unsigned int gossip_msgs;
    float coverage;
    unsigned int seed;
    float grid[GRID_W * GRID_H];
};

/* ---------- helpers: vec2 ---------- */

static Vec2 vec2_add(Vec2 a, Vec2 b) {
    Vec2 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    return r;
}

static Vec2 vec2_sub(Vec2 a, Vec2 b) {
    Vec2 r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    return r;
}

static Vec2 vec2_scale(Vec2 v, float s) {
    Vec2 r;
    r.x = v.x * s;
    r.y = v.y * s;
    return r;
}

static float vec2_len(Vec2 v) {
    return v.x * v.x + v.y * v.y;
}

static Vec2 vec2_norm(Vec2 v) {
    float l = v.x * v.x + v.y * v.y;
    float inv;
    Vec2 r;
    if (l < 1e-6f) { r.x = 0.0f; r.y = 0.0f; return r; }
    inv = 1.0f;
    while ((inv * inv) * l > 1.0001f) inv = inv * 0.5f;
    while ((inv * inv) * l < 0.9999f) inv = inv * 2.0f;
    r.x = v.x * inv;
    r.y = v.y * inv;
    return r;
}

static float fabs_f(float v) { return v < 0.0f ? -v : v; }

static float frand(unsigned int *seed) {
    unsigned int s = *seed;
    s = s ^ (s << 13);
    s = s ^ (s >> 17);
    s = s ^ (s << 5);
    *seed = s;
    return ((float)(s & 0xFFFFU)) / 65536.0f;
}

static float clamp_f(float v, float lo, float hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

/* ---------- output helpers ---------- */

static void print_sep(void) {
    host_print("------------------------------------\n");
}

static void print_u32(unsigned int v) {
    char buf[12];
    int i = 11;
    buf[i] = 0;
    if (v == 0) { host_print("0"); return; }
    while (v > 0 && i > 0) {
        i--;
        buf[i] = (char)('0' + (v % 10));
        v = v / 10;
    }
    host_print(&buf[i]);
}

static void print_f32(float v) {
    char buf[24];
    int i = 0;
    int whole;
    int frac;
    int neg = 0;
    if (v < 0.0f) { neg = 1; v = -v; }
    whole = (int)v;
    frac  = (int)((v - (float)whole) * 100.0f);
    if (frac < 0) frac = -frac;
    if (neg) { buf[i++] = '-'; }
    if (whole == 0) { buf[i++] = '0'; }
    else {
        char tmp[10];
        int n = 0;
        int w = whole;
        while (w > 0) { tmp[n++] = (char)('0' + (w % 10)); w = w / 10; }
        while (n > 0) { buf[i++] = tmp[--n]; }
    }
    buf[i++] = '.';
    buf[i++] = (char)('0' + (frac / 10));
    buf[i++] = (char)('0' + (frac % 10));
    buf[i] = 0;
    host_print(buf);
}

/* ---------- swarm lifecycle ---------- */

static Swarm *swarm_create(void) {
    Swarm *s = (Swarm *)host_alloc(sizeof(Swarm), 8);
    unsigned int i;
    if (s == (Swarm *)0) { host_print("ERR: alloc swarm\n"); host_exit(1); }
    s->agents = (Agent *)host_alloc(sizeof(Agent) * MAX_AGENTS, 8);
    s->pheromones = (Pheromone *)host_alloc(sizeof(Pheromone) * MAX_PHEROMONES, 8);
    if (s->agents == (Agent *)0 || s->pheromones == (Pheromone *)0) {
        host_print("ERR: alloc agents/pheromones\n");
        host_exit(1);
    }
    s->count = 0;
    s->capacity = MAX_AGENTS;
    s->phero_count = 0;
    s->phero_cap = MAX_PHEROMONES;
    s->global_best_x = 0.0f;
    s->global_best_y = 0.0f;
    s->global_best_fit = -1e9f;
    s->consensus = 0.0f;
    s->leader_id = -1;
    s->step = 0;
    s->failures_detected = 0;
    s->repairs_done = 0;
    s->tasks_allocated = 0;
    s->gossip_msgs = 0;
    s->coverage = 0.0f;
    s->seed = 0xA5B3C1D7U ^ 0x1234FEDCU;
    s->seed = s->seed ^ 0xA5A5A5A5U;
    for (i = 0; i < GRID_W * GRID_H; i++) s->grid[i] = 0.0f;
    for (i = 0; i < MAX_AGENTS; i++) {
        s->agents[i].alive = 0;
        s->agents[i].id = i;
        s->agents[i].phase = PHASE_IDLE;
    }
    for (i = 0; i < MAX_PHEROMONES; i++) s->pheromones[i].intensity = 0.0f;
    return s;
}

static Agent *swarm_spawn(Swarm *s, float x, float y) {
    Agent *a;
    if (s->count >= s->capacity) {
        host_print("ERR: swarm full\n");
        return (Agent *)0;
    }
    a = &s->agents[s->count];
    a->id = s->count;
    a->alive = 1;
    a->role = 0;
    a->phase = PHASE_IDLE;
    a->leader = 0;
    a->neighbors = 0;
    a->x = x;
    a->y = y;
    a->vx = (frand(&s->seed) - 0.5f) * 0.4f;
    a->vy = (frand(&s->seed) - 0.5f) * 0.4f;
    a->fitness = 0.0f;
    a->best_x = x;
    a->best_y = y;
    a->best_fit = -1e9f;
    a->opinion = frand(&s->seed) * 10.0f;
    a->rank = frand(&s->seed);
    a->waggle = 0.0f;
    a->energy = 1.0f;
    a->trust = 0.5f;
    a->task_x = 0.0f;
    a->task_y = 0.0f;
    a->task_claim = -1;
    s->count++;
    return a;
}

/* ---------- flocking (Reynolds boids) ---------- */

static void swarm_flocking(Swarm *s) {
    unsigned int i, j;
    for (i = 0; i < s->count; i++) {
        Agent *me = &s->agents[i];
        Vec2 sep, ali, coh, diff;
        float sep_n = 0.0f, ali_n = 0.0f, coh_n = 0.0f;
        Vec2 accel;
        float sp;
        if (!me->alive) continue;
        sep.x = 0.0f; sep.y = 0.0f;
        ali.x = 0.0f; ali.y = 0.0f;
        coh.x = 0.0f; coh.y = 0.0f;
        me->neighbors = 0;
        for (j = 0; j < s->count; j++) {
            Agent *o = &s->agents[j];
            float d2;
            if (i == j || !o->alive) continue;
            diff.x = o->x - me->x;
            diff.y = o->y - me->y;
            d2 = diff.x * diff.x + diff.y * diff.y;
            if (d2 < SEP_RADIUS * SEP_RADIUS && d2 > 1e-4f) {
                float inv = 1.0f / (d2 + 0.01f);
                sep.x -= diff.x * inv;
                sep.y -= diff.y * inv;
                sep_n += 1.0f;
            }
            if (d2 < ALI_RADIUS * ALI_RADIUS) {
                ali.x += o->vx;
                ali.y += o->vy;
                ali_n += 1.0f;
            }
            if (d2 < COH_RADIUS * COH_RADIUS) {
                coh.x += o->x;
                coh.y += o->y;
                coh_n += 1.0f;
            }
            me->neighbors++;
        }
        accel.x = 0.0f;
        accel.y = 0.0f;
        if (sep_n > 0.0f) {
            Vec2 sn = vec2_norm(sep);
            accel.x += sn.x * SEP_WEIGHT;
            accel.y += sn.y * SEP_WEIGHT;
        }
        if (ali_n > 0.0f) {
            Vec2 an = vec2_norm(ali);
            accel.x += (an.x - me->vx) * ALI_WEIGHT;
            accel.y += (an.y - me->vy) * ALI_WEIGHT;
        }
        if (coh_n > 0.0f) {
            Vec2 center;
            center.x = coh.x / coh_n - me->x;
            center.y = coh.y / coh_n - me->y;
            {
                Vec2 cn = vec2_norm(center);
                accel.x += cn.x * COH_WEIGHT;
                accel.y += cn.y * COH_WEIGHT;
            }
        }
        me->vx += accel.x * 0.1f;
        me->vy += accel.y * 0.1f;
        sp = me->vx * me->vx + me->vy * me->vy;
        if (sp > MAX_SPEED * MAX_SPEED) {
            float sc = MAX_SPEED / (sp + 1e-3f);
            me->vx *= sc * 0.5f;
            me->vy *= sc * 0.5f;
        }
        me->x += me->vx;
        me->y += me->vy;
        if (me->x < 0.0f) me->x = 0.0f;
        if (me->y < 0.0f) me->y = 0.0f;
        if (me->x > 20.0f) me->x = 20.0f;
        if (me->y > 20.0f) me->y = 20.0f;
    }
}

/* ---------- PSO update ---------- */

static void swarm_pso_update(Swarm *s) {
    unsigned int i;
    for (i = 0; i < s->count; i++) {
        Agent *a = &s->agents[i];
        float fx;
        if (!a->alive) continue;
        fx = (a->x - 10.0f) * (a->x - 10.0f)
           + (a->y - 10.0f) * (a->y - 10.0f);
        a->fitness = -fx;
        if (a->fitness > a->best_fit) {
            a->best_fit = a->fitness;
            a->best_x = a->x;
            a->best_y = a->y;
        }
        if (a->fitness > s->global_best_fit) {
            s->global_best_fit = a->fitness;
            s->global_best_x = a->x;
            s->global_best_y = a->y;
        }
    }
    for (i = 0; i < s->count; i++) {
        Agent *a = &s->agents[i];
        float r1, r2, dx_p, dy_p, dx_g, dy_g;
        if (!a->alive) continue;
        r1 = frand(&s->seed);
        r2 = frand(&s->seed);
        dx_p = a->best_x - a->x;
        dy_p = a->best_y - a->y;
        dx_g = s->global_best_x - a->x;
        dy_g = s->global_best_y - a->y;
        a->vx = PSO_INERTIA * a->vx + PSO_COG * r1 * dx_p + PSO_SOC * r2 * dx_g;
        a->vy = PSO_INERTIA * a->vy + PSO_COG * r1 * dy_p + PSO_SOC * r2 * dy_g;
        a->vx = clamp_f(a->vx, -MAX_SPEED, MAX_SPEED);
        a->vy = clamp_f(a->vy, -MAX_SPEED, MAX_SPEED);
        a->x += a->vx;
        a->y += a->vy;
        a->x = clamp_f(a->x, 0.0f, 20.0f);
        a->y = clamp_f(a->y, 0.0f, 20.0f);
    }
}

/* ---------- ACO pheromone ---------- */

static void swarm_aco_deposit(Swarm *s, unsigned int agent_id, float amount) {
    Agent *a;
    unsigned int slot;
    if (agent_id >= s->count) return;
    a = &s->agents[agent_id];
    if (!a->alive) return;
    if (s->phero_count < s->phero_cap) {
        slot = s->phero_count++;
    } else {
        slot = agent_id % s->phero_cap;
    }
    s->pheromones[slot].x = a->x;
    s->pheromones[slot].y = a->y;
    s->pheromones[slot].intensity += amount;
    s->pheromones[slot].depositor = agent_id;
}

static float swarm_aco_sample(Swarm *s, float x, float y) {
    unsigned int k;
    float total = 0.0f;
    for (k = 0; k < s->phero_count; k++) {
        Pheromone *p = &s->pheromones[k];
        float dx = p->x - x;
        float dy = p->y - y;
        float d2 = dx * dx + dy * dy + 0.1f;
        total += p->intensity / d2;
    }
    return total;
}

static void swarm_aco_evaporate(Swarm *s) {
    unsigned int k;
    for (k = 0; k < s->phero_count; k++) {
        s->pheromones[k].intensity *= PHEROMONE_DECAY;
        if (s->pheromones[k].intensity < 0.01f) {
            s->pheromones[k].intensity = 0.0f;
        }
    }
}

static void swarm_aco_step(Swarm *s) {
    unsigned int i;
    for (i = 0; i < s->count; i++) {
        Agent *a = &s->agents[i];
        float gx, gy, pull;
        float dx, dy, d;
        if (!a->alive) continue;
        gx = 15.0f;
        gy = 15.0f;
        pull = swarm_aco_sample(s, a->x, a->y);
        dx = gx - a->x;
        dy = gy - a->y;
        d = dx * dx + dy * dy + 0.01f;
        a->vx += (dx / d) * 0.3f + (frand(&s->seed) - 0.5f) * 0.2f;
        a->vy += (dy / d) * 0.3f + (frand(&s->seed) - 0.5f) * 0.2f;
        if (pull > 0.5f) {
            a->vx *= 1.05f;
            a->vy *= 1.05f;
        }
        a->vx = clamp_f(a->vx, -MAX_SPEED, MAX_SPEED);
        a->vy = clamp_f(a->vy, -MAX_SPEED, MAX_SPEED);
        a->x += a->vx;
        a->y += a->vy;
        a->x = clamp_f(a->x, 0.0f, 20.0f);
        a->y = clamp_f(a->y, 0.0f, 20.0f);
        swarm_aco_deposit(s, i, 1.0f / (d + 1.0f));
    }
    swarm_aco_evaporate(s);
}

/* ---------- bee waggle dance ---------- */

static void swarm_bee_waggle(Swarm *s, unsigned int agent_id, float quality) {
    unsigned int j;
    Agent *src;
    float radius;
    if (agent_id >= s->count) return;
    src = &s->agents[agent_id];
    if (!src->alive) return;
    src->waggle = quality;
    radius = 2.0f + quality * 2.0f;
    for (j = 0; j < s->count; j++) {
        Agent *rcv = &s->agents[j];
        float dx, dy, d2;
        if (j == agent_id || !rcv->alive) continue;
        dx = rcv->x - src->x;
        dy = rcv->y - src->y;
        d2 = dx * dx + dy * dy;
        if (d2 < radius * radius) {
            rcv->opinion = rcv->opinion * 0.7f + quality * 0.3f;
            rcv->trust = rcv->trust * 0.9f + src->trust * 0.1f;
            rcv->task_x = src->x + (src->vx * quality * 5.0f);
            rcv->task_y = src->y + (src->vy * quality * 5.0f);
        }
    }
}

static void swarm_bee_forage(Swarm *s) {
    unsigned int i;
    for (i = 0; i < s->count; i++) {
        Agent *a = &s->agents[i];
        float dx, dy, q;
        if (!a->alive) continue;
        dx = a->task_x - a->x;
        dy = a->task_y - a->y;
        q = 1.0f / (dx * dx + dy * dy + 1.0f);
        a->vx += dx * 0.05f + (frand(&s->seed) - 0.5f) * 0.3f;
        a->vy += dy * 0.05f + (frand(&s->seed) - 0.5f) * 0.3f;
        a->vx = clamp_f(a->vx, -MAX_SPEED, MAX_SPEED);
        a->vy = clamp_f(a->vy, -MAX_SPEED, MAX_SPEED);
        a->x += a->vx;
        a->y += a->vy;
        a->x = clamp_f(a->x, 0.0f, 20.0f);
        a->y = clamp_f(a->y, 0.0f, 20.0f);
        a->energy -= 0.005f;
        if (a->energy < 0.2f) {
            swarm_bee_waggle(s, i, q);
            a->energy = 1.0f;
        }
    }
}

/* ---------- fish school ---------- */

static void swarm_fish_school(Swarm *s) {
    unsigned int i, j;
    float pred_x = 10.0f, pred_y = 10.0f;
    float pred_active = ((s->step / 50) & 1) ? 1.0f : 0.0f;
    for (i = 0; i < s->count; i++) {
        Agent *me = &s->agents[i];
        Vec2 align, cohesion, escape;
        float n_a = 0.0f, n_c = 0.0f;
        if (!me->alive) continue;
        align.x = 0.0f; align.y = 0.0f;
        cohesion.x = 0.0f; cohesion.y = 0.0f;
        escape.x = 0.0f; escape.y = 0.0f;
        for (j = 0; j < s->count; j++) {
            Agent *o = &s->agents[j];
            float dx, dy, d2;
            if (i == j || !o->alive) continue;
            dx = o->x - me->x;
            dy = o->y - me->y;
            d2 = dx * dx + dy * dy;
            if (d2 < 4.0f) {
                align.x += o->vx;
                align.y += o->vy;
                n_a += 1.0f;
            }
            if (d2 < 9.0f) {
                cohesion.x += o->x;
                cohesion.y += o->y;
                n_c += 1.0f;
            }
            if (d2 < 1.0f && d2 > 1e-4f) {
                float inv = 1.0f / d2;
                escape.x -= dx * inv;
                escape.y -= dy * inv;
            }
        }
        if (n_a > 0.0f) {
            me->vx += (align.x / n_a - me->vx) * 0.08f;
            me->vy += (align.y / n_a - me->vy) * 0.08f;
        }
        if (n_c > 0.0f) {
            float cx = cohesion.x / n_c - me->x;
            float cy = cohesion.y / n_c - me->y;
            me->vx += cx * 0.01f;
            me->vy += cy * 0.01f;
        }
        me->vx += escape.x * 0.2f;
        me->vy += escape.y * 0.2f;
        if (pred_active > 0.5f) {
            float dxp = me->x - pred_x;
            float dyp = me->y - pred_y;
            float d2p = dxp * dxp + dyp * dyp + 0.01f;
            me->vx += (dxp / d2p) * 2.0f;
            me->vy += (dyp / d2p) * 2.0f;
        }
        me->vx = clamp_f(me->vx, -MAX_SPEED, MAX_SPEED);
        me->vy = clamp_f(me->vy, -MAX_SPEED, MAX_SPEED);
        me->x += me->vx;
        me->y += me->vy;
        me->x = clamp_f(me->x, 0.0f, 20.0f);
        me->y = clamp_f(me->y, 0.0f, 20.0f);
    }
}

/* ---------- consensus algorithms ---------- */

static void swarm_consensus_avg(Swarm *s) {
    unsigned int i, j, round;
    for (round = 0; round < 10; round++) {
        for (i = 0; i < s->count; i++) {
            Agent *me = &s->agents[i];
            float sum = me->opinion;
            float cnt = 1.0f;
            if (!me->alive) continue;
            for (j = 0; j < s->count; j++) {
                Agent *o = &s->agents[j];
                float dx, dy, d2;
                if (i == j || !o->alive) continue;
                dx = o->x - me->x;
                dy = o->y - me->y;
                d2 = dx * dx + dy * dy;
                if (d2 < COMM_RANGE * COMM_RANGE) {
                    sum += o->opinion;
                    cnt += 1.0f;
                }
            }
            me->opinion = sum / cnt;
        }
    }
    s->consensus = 0.0f;
    for (i = 0; i < s->count; i++) {
        if (s->agents[i].alive) s->consensus += s->agents[i].opinion;
    }
    if (s->count > 0) s->consensus /= (float)s->count;
}

static void swarm_consensus_max(Swarm *s) {
    unsigned int i, j, round;
    for (round = 0; round < 8; round++) {
        for (i = 0; i < s->count; i++) {
            Agent *me = &s->agents[i];
            float mx = me->opinion;
            if (!me->alive) continue;
            for (j = 0; j < s->count; j++) {
                Agent *o = &s->agents[j];
                float dx, dy, d2;
                if (i == j || !o->alive) continue;
                dx = o->x - me->x;
                dy = o->y - me->y;
                d2 = dx * dx + dy * dy;
                if (d2 < COMM_RANGE * COMM_RANGE && o->opinion > mx) {
                    mx = o->opinion;
                }
            }
            me->opinion = me->opinion * 0.6f + mx * 0.4f;
        }
    }
}

/* ---------- leader election (Bully-like) ---------- */

static int swarm_leader_elect(Swarm *s) {
    unsigned int i, j, round;
    int best_id = -1;
    float best_rank = -1.0f;
    for (i = 0; i < s->count; i++) s->agents[i].leader = 0;
    for (round = 0; round < ELECTION_RNDS; round++) {
        for (i = 0; i < s->count; i++) {
            Agent *me = &s->agents[i];
            float my_rank = me->rank + me->energy * 0.3f;
            if (!me->alive) continue;
            for (j = 0; j < s->count; j++) {
                Agent *o = &s->agents[j];
                float dx, dy, d2, o_rank;
                if (i == j || !o->alive) continue;
                dx = o->x - me->x;
                dy = o->y - me->y;
                d2 = dx * dx + dy * dy;
                if (d2 > COMM_RANGE * COMM_RANGE) continue;
                o_rank = o->rank + o->energy * 0.3f;
                if (o_rank > my_rank + 0.001f) {
                    my_rank = o_rank;
                }
            }
            me->rank = me->rank * 0.95f + my_rank * 0.05f;
        }
    }
    for (i = 0; i < s->count; i++) {
        if (s->agents[i].alive && s->agents[i].rank > best_rank) {
            best_rank = s->agents[i].rank;
            best_id = (int)i;
        }
    }
    if (best_id >= 0) {
        s->agents[best_id].leader = 1;
        s->leader_id = best_id;
    }
    return best_id;
}

/* ---------- task allocation (auction-based) ---------- */

static int swarm_task_allocate(Swarm *s, float task_x, float task_y) {
    unsigned int i;
    int best = -1;
    float best_cost = 1e9f;
    for (i = 0; i < s->count; i++) {
        Agent *a = &s->agents[i];
        float dx, dy, cost, bid;
        if (!a->alive || a->energy < 0.2f) continue;
        dx = task_x - a->x;
        dy = task_y - a->y;
        cost = dx * dx + dy * dy;
        bid = cost / (a->energy + 0.01f) - a->trust * 2.0f;
        if (bid < best_cost) {
            best_cost = bid;
            best = (int)i;
        }
    }
    if (best >= 0) {
        s->agents[best].task_x = task_x;
        s->agents[best].task_y = task_y;
        s->agents[best].task_claim = (int)s->step;
        s->agents[best].energy -= 0.1f;
        s->tasks_allocated++;
    }
    return best;
}

/* ---------- self-repair and self-adapt ---------- */

static void swarm_self_repair(Swarm *s) {
    unsigned int i;
    for (i = 0; i < s->count; i++) {
        Agent *a = &s->agents[i];
        if (!a->alive) {
            s->failures_detected++;
            if (s->seed % 3 == 0 && s->count < s->capacity) {
                float rx = frand(&s->seed) * 20.0f;
                float ry = frand(&s->seed) * 20.0f;
                Agent *r = swarm_spawn(s, rx, ry);
                if (r != (Agent *)0) {
                    r->role = 1;
                    s->repairs_done++;
                }
            }
        }
    }
}

static void swarm_self_adapt(Swarm *s) {
    unsigned int i;
    float avg_n = 0.0f;
    float adapt;
    for (i = 0; i < s->count; i++) {
        if (s->agents[i].alive) avg_n += (float)s->agents[i].neighbors;
    }
    if (s->count > 0) avg_n /= (float)s->count;
    adapt = avg_n / 4.0f;
    adapt = clamp_f(adapt, 0.5f, 2.0f);
    for (i = 0; i < s->count; i++) {
        Agent *a = &s->agents[i];
        if (!a->alive) continue;
        a->energy += 0.002f * adapt;
        a->energy = clamp_f(a->energy, 0.0f, 1.0f);
        a->trust = a->trust * 0.99f + adapt * 0.01f;
    }
}

/* ---------- pattern formation ---------- */

static void swarm_form_pattern(Swarm *s, int pattern, float cx, float cy, float r) {
    unsigned int i;
    float angle_step;
    int cols;
    for (i = 0; i < s->count; i++) {
        Agent *a = &s->agents[i];
        float tx, ty, angle, row, col;
        if (!a->alive) continue;
        tx = a->x;
        ty = a->y;
        if (pattern == PATTERN_CIRCLE) {
            angle_step = 6.28318f / (float)(s->count > 0 ? s->count : 1);
            angle = angle_step * (float)i;
            tx = cx + r * (angle * 0.15f);
            ty = cy + r * (angle * 0.15f - 3.14f);
            {
                float ca = angle;
                float sa_a = angle * 0.5f;
                float cos_a = 1.0f - (ca * ca) * 0.5f + (ca * ca * ca * ca) * 0.041666f;
                float sin_a = sa_a - (sa_a * sa_a * sa_a) * 0.166666f;
                tx = cx + r * cos_a;
                ty = cy + r * sin_a;
            }
        } else if (pattern == PATTERN_LINE) {
            tx = cx - r + (2.0f * r * (float)i) / (float)(s->count > 1 ? s->count - 1 : 1);
            ty = cy;
        } else if (pattern == PATTERN_GRID) {
            cols = 4;
            if (s->count > 0) {
                unsigned int c = 1;
                while (c * c < s->count) c++;
                cols = (int)c;
            }
            row = (float)(i / cols);
            col = (float)(i % cols);
            tx = cx - r + col * (2.0f * r / (float)(cols > 1 ? cols - 1 : 1));
            ty = cy - r + row * (2.0f * r / (float)(cols > 1 ? cols - 1 : 1));
        } else if (pattern == PATTERN_VEE) {
            float side = (i % 2 == 0) ? 1.0f : -1.0f;
            float depth = (float)(i / 2);
            tx = cx + side * depth * 0.8f;
            ty = cy + depth * 0.8f;
        }
        a->vx = (tx - a->x) * 0.15f;
        a->vy = (ty - a->y) * 0.15f;
        a->vx = clamp_f(a->vx, -MAX_SPEED, MAX_SPEED);
        a->vy = clamp_f(a->vy, -MAX_SPEED, MAX_SPEED);
        a->x += a->vx;
        a->y += a->vy;
        a->phase = PHASE_FORM;
    }
}

/* ---------- communication: gossip + topology ---------- */

static void swarm_communicate(Swarm *s) {
    unsigned int i, j;
    for (i = 0; i < s->count; i++) {
        Agent *me = &s->agents[i];
        if (!me->alive) continue;
        me->neighbors = 0;
        for (j = 0; j < s->count; j++) {
            Agent *o = &s->agents[j];
            float dx, dy, d2;
            if (i == j || !o->alive) continue;
            dx = o->x - me->x;
            dy = o->y - me->y;
            d2 = dx * dx + dy * dy;
            if (d2 < COMM_RANGE * COMM_RANGE) me->neighbors++;
        }
    }
}

static void swarm_topology_repair(Swarm *s) {
    unsigned int i;
    for (i = 0; i < s->count; i++) {
        Agent *a = &s->agents[i];
        if (!a->alive) continue;
        if (a->neighbors == 0 && s->count > 1) {
            float nearest_d2 = 1e9f;
            int nearest = -1;
            unsigned int j;
            for (j = 0; j < s->count; j++) {
                Agent *o = &s->agents[j];
                float dx, dy, d2;
                if (j == i || !o->alive) continue;
                dx = o->x - a->x;
                dy = o->y - a->y;
                d2 = dx * dx + dy * dy;
                if (d2 < nearest_d2) { nearest_d2 = d2; nearest = (int)j; }
            }
            if (nearest >= 0) {
                Agent *t = &s->agents[nearest];
                a->vx += (t->x - a->x) * 0.2f;
                a->vy += (t->y - a->y) * 0.2f;
            }
        }
    }
}

static void swarm_gossip(Swarm *s) {
    unsigned int round, i, j;
    for (round = 0; round < GOSSIP_ROUNDS; round++) {
        for (i = 0; i < s->count; i++) {
            Agent *me = &s->agents[i];
            int peer;
            float peer_val;
            if (!me->alive) continue;
            peer = (int)(frand(&s->seed) * (float)s->count);
            if (peer >= (int)s->count) peer = (int)s->count - 1;
            if (peer == (int)i) peer = ((int)i + 1) % (int)s->count;
            if (!s->agents[peer].alive) continue;
            peer_val = s->agents[peer].opinion;
            me->opinion = me->opinion * 0.5f + peer_val * 0.5f;
            s->gossip_msgs++;
        }
    }
    for (i = 0; i < s->count; i++) {
        for (j = i + 1; j < s->count; j++) {
            Agent *a = &s->agents[i];
            Agent *b = &s->agents[j];
            float dx, dy, d2;
            if (!a->alive || !b->alive) continue;
            dx = b->x - a->x;
            dy = b->y - a->y;
            d2 = dx * dx + dy * dy;
            if (d2 < COMM_RANGE * COMM_RANGE) {
                float avg = (a->opinion + b->opinion) * 0.5f;
                a->opinion = avg;
                b->opinion = avg;
            }
        }
    }
}

/* ---------- application-level behaviors ---------- */

static void swarm_drone_formation(Swarm *s) {
    unsigned int i;
    float cx = 10.0f, cy = 10.0f;
    for (i = 0; i < s->count; i++) {
        Agent *a = &s->agents[i];
        float ang, tx, ty;
        if (!a->alive) continue;
        ang = 6.28318f * (float)i / (float)(s->count > 0 ? s->count : 1);
        tx = cx + 3.0f + ang * 0.4f;
        ty = cy + 3.0f + ang * 0.4f;
        {
            float ca = ang;
            float cos_a = 1.0f - (ca * ca) * 0.5f + (ca * ca * ca * ca) * 0.041666f;
            float sa = ang - (ang * ang * ang) * 0.166666f;
            tx = cx + 3.0f * cos_a;
            ty = cy + 3.0f * sa;
        }
        a->vx = (tx - a->x) * 0.1f;
        a->vy = (ty - a->y) * 0.1f;
        a->vx = clamp_f(a->vx, -MAX_SPEED, MAX_SPEED);
        a->vy = clamp_f(a->vy, -MAX_SPEED, MAX_SPEED);
        a->x += a->vx;
        a->y += a->vy;
        a->role = 2;
    }
}

static void swarm_warehouse_route(Swarm *s) {
    unsigned int i;
    for (i = 0; i < s->count; i++) {
        Agent *a = &s->agents[i];
        float target_x, target_y;
        float dx, dy;
        if (!a->alive) continue;
        target_x = 5.0f + (float)(i % 4) * 4.0f;
        target_y = 5.0f + (float)(i / 4) * 3.0f;
        if (a->task_claim >= 0) {
            target_x = a->task_x;
            target_y = a->task_y;
        }
        dx = target_x - a->x;
        dy = target_y - a->y;
        a->vx = dx * 0.12f + (frand(&s->seed) - 0.5f) * 0.1f;
        a->vy = dy * 0.12f + (frand(&s->seed) - 0.5f) * 0.1f;
        a->vx = clamp_f(a->vx, -MAX_SPEED, MAX_SPEED);
        a->vy = clamp_f(a->vy, -MAX_SPEED, MAX_SPEED);
        a->x += a->vx;
        a->y += a->vy;
        a->x = clamp_f(a->x, 0.0f, 20.0f);
        a->y = clamp_f(a->y, 0.0f, 20.0f);
        a->role = 3;
    }
}

static void swarm_search_cover(Swarm *s, float w, float h) {
    unsigned int i;
    int gx, gy;
    for (i = 0; i < s->count; i++) {
        Agent *a = &s->agents[i];
        float target_x, target_y, dx, dy;
        if (!a->alive) continue;
        gx = (int)i % GRID_W;
        gy = (int)i / GRID_W;
        if (gy >= GRID_H) gy = GRID_H - 1;
        target_x = (float)gx * (w / (float)GRID_W) + 0.5f;
        target_y = (float)gy * (h / (float)GRID_H) + 0.5f;
        dx = target_x - a->x;
        dy = target_y - a->y;
        a->vx += dx * 0.05f;
        a->vy += dy * 0.05f;
        a->vx = clamp_f(a->vx, -MAX_SPEED, MAX_SPEED);
        a->vy = clamp_f(a->vy, -MAX_SPEED, MAX_SPEED);
        a->x += a->vx;
        a->y += a->vy;
        a->x = clamp_f(a->x, 0.0f, w);
        a->y = clamp_f(a->y, 0.0f, h);
        s->grid[gy * GRID_W + gx] += 0.1f;
        a->role = 4;
    }
}

static float swarm_sense_coverage(Swarm *s) {
    unsigned int i;
    unsigned int covered = 0;
    for (i = 0; i < GRID_W * GRID_H; i++) {
        if (s->grid[i] > 0.01f) covered++;
    }
    s->coverage = (float)covered / (float)(GRID_W * GRID_H);
    return s->coverage;
}

/* ---------- main step ---------- */

static void swarm_step(Swarm *s, float dt) {
    (void)dt;
    s->step++;
    swarm_communicate(s);
    if (s->step % 20 == 0) swarm_self_adapt(s);
    if (s->step % 30 == 0) swarm_self_repair(s);
}

/* ---------- statistics ---------- */

static void swarm_print_stats(Swarm *s) {
    unsigned int i;
    unsigned int alive = 0;
    float sum_x = 0.0f, sum_y = 0.0f;
    float avg_energy = 0.0f;
    float max_rank = -1.0f;
    int max_rank_id = -1;
    float cov;
    for (i = 0; i < s->count; i++) {
        Agent *a = &s->agents[i];
        if (!a->alive) continue;
        alive++;
        sum_x += a->x;
        sum_y += a->y;
        avg_energy += a->energy;
        if (a->rank > max_rank) { max_rank = a->rank; max_rank_id = (int)i; }
    }
    cov = swarm_sense_coverage(s);
    host_print("=== swarm statistics ===\n");
    host_print("step:        "); print_u32(s->step); host_print("\n");
    host_print("agents:      "); print_u32(s->count); host_print("\n");
    host_print("alive:       "); print_u32(alive); host_print("\n");
    host_print("leader_id:   "); print_u32(s->leader_id < 0 ? 0 : (unsigned int)s->leader_id); host_print("\n");
    host_print("consensus:   "); print_f32(s->consensus); host_print("\n");
    host_print("global_best: "); print_f32(s->global_best_fit); host_print("\n");
    host_print("coverage:    "); print_f32(cov); host_print("\n");
    host_print("tasks_alloc: "); print_u32(s->tasks_allocated); host_print("\n");
    host_print("repairs:     "); print_u32(s->repairs_done); host_print("\n");
    host_print("gossip_msgs: "); print_u32(s->gossip_msgs); host_print("\n");
    host_print("phero_trail: "); print_u32(s->phero_count); host_print("\n");
    if (alive > 0) {
        host_print("centroid:    (");
        print_f32(sum_x / (float)alive);
        host_print(", ");
        print_f32(sum_y / (float)alive);
        host_print(")\n");
        host_print("avg_energy:  "); print_f32(avg_energy / (float)alive); host_print("\n");
    }
    host_print("top_ranker:  "); print_u32(max_rank_id < 0 ? 0 : (unsigned int)max_rank_id); host_print("\n");
    print_sep();
}

/* ---------- main ---------- */

int main(void) {
    Swarm *s;
    unsigned int i;
    int elected;
    int assigned;
    float cov;

    host_print("[swarm_intelligence] AI-ASM OS module boot\n");
    print_sep();

    /* --- initialize swarm and spawn agents --- */
    s = swarm_create();
    host_print("[init] spawning 12 agents...\n");
    for (i = 0; i < 12; i++) {
        float ax = 2.0f + (float)(i % 4) * 2.5f;
        float ay = 2.0f + (float)(i / 4) * 2.5f;
        Agent *a = swarm_spawn(s, ax, ay);
        if (a != (Agent *)0) {
            a->opinion = (float)i * 1.5f;
            a->rank = frand(&s->seed);
        }
    }
    host_print("[init] spawned: "); print_u32(s->count); host_print("\n");
    print_sep();

    /* --- phase 1: flocking behavior --- */
    host_print("[phase 1] reynolds boids flocking (20 steps)...\n");
    for (i = 0; i < 20; i++) {
        swarm_flocking(s);
        swarm_step(s, 0.1f);
    }
    host_print("[phase 1] flock complete. neighbors[0]=");
    print_u32(s->agents[0].neighbors);
    host_print("\n");
    print_sep();

    /* --- phase 2: ant colony optimization --- */
    host_print("[phase 2] ant colony optimization for path finding...\n");
    for (i = 0; i < 15; i++) {
        swarm_aco_step(s);
    }
    host_print("[phase 2] pheromone trails: "); print_u32(s->phero_count); host_print("\n");
    host_print("[phase 2] sampled intensity at (10,10): ");
    print_f32(swarm_aco_sample(s, 10.0f, 10.0f));
    host_print("\n");
    print_sep();

    /* --- phase 3: consensus --- */
    host_print("[phase 3] average consensus algorithm...\n");
    host_print("[phase 3] pre-consensus opinions[0]="); print_f32(s->agents[0].opinion); host_print("\n");
    swarm_consensus_avg(s);
    host_print("[phase 3] post-consensus opinions[0]="); print_f32(s->agents[0].opinion); host_print("\n");
    host_print("[phase 3] consensus value: "); print_f32(s->consensus); host_print("\n");

    host_print("[phase 3] max consensus...\n");
    swarm_consensus_max(s);
    host_print("[phase 3] converged opinion[0]: "); print_f32(s->agents[0].opinion); host_print("\n");
    print_sep();

    /* --- phase 4: leader election --- */
    host_print("[phase 4] bully-algorithm leader election...\n");
    elected = swarm_leader_elect(s);
    host_print("[phase 4] elected leader id: "); print_u32(elected < 0 ? 0 : (unsigned int)elected); host_print("\n");
    print_sep();

    /* --- phase 5: PSO --- */
    host_print("[phase 5] particle swarm optimization (20 iters)...\n");
    for (i = 0; i < 20; i++) {
        swarm_pso_update(s);
    }
    host_print("[phase 5] global best at (");
    print_f32(s->global_best_x);
    host_print(", ");
    print_f32(s->global_best_y);
    host_print(") fitness=");
    print_f32(s->global_best_fit);
    host_print("\n");
    print_sep();

    /* --- phase 6: geometric pattern formation --- */
    host_print("[phase 6] forming circle pattern at (10,10) r=4...\n");
    for (i = 0; i < 10; i++) {
        swarm_form_pattern(s, PATTERN_CIRCLE, 10.0f, 10.0f, 4.0f);
    }
    host_print("[phase 6] agent[3] pos=(");
    print_f32(s->agents[3].x);
    host_print(", ");
    print_f32(s->agents[3].y);
    host_print(")\n");

    host_print("[phase 6] forming vee pattern...\n");
    for (i = 0; i < 8; i++) {
        swarm_form_pattern(s, PATTERN_VEE, 10.0f, 5.0f, 3.0f);
    }
    host_print("[phase 6] vee agent[5] pos=(");
    print_f32(s->agents[5].x);
    host_print(", ");
    print_f32(s->agents[5].y);
    host_print(")\n");
    print_sep();

    /* --- phase 7: bee foraging --- */
    host_print("[phase 7] bee waggle-dance foraging (10 steps)...\n");
    for (i = 0; i < 10; i++) {
        swarm_bee_forage(s);
        if (i == 5) swarm_bee_waggle(s, 0, 0.9f);
    }
    host_print("[phase 7] bee[0] energy="); print_f32(s->agents[0].energy); host_print("\n");
    print_sep();

    /* --- phase 8: fish schooling --- */
    host_print("[phase 8] fish schooling with predator avoidance...\n");
    for (i = 0; i < 10; i++) {
        swarm_fish_school(s);
    }
    host_print("[phase 8] school centroid[0]=(");
    print_f32(s->agents[0].x);
    host_print(", ");
    print_f32(s->agents[0].y);
    host_print(")\n");
    print_sep();

    /* --- phase 9: gossip communication --- */
    host_print("[phase 9] gossip protocol propagation...\n");
    swarm_gossip(s);
    host_print("[phase 9] gossip msgs exchanged: "); print_u32(s->gossip_msgs); host_print("\n");
    print_sep();

    /* --- phase 10: task allocation --- */
    host_print("[phase 10] auction-based task allocation...\n");
    assigned = swarm_task_allocate(s, 15.0f, 15.0f);
    host_print("[phase 10] task at (15,15) assigned to: ");
    print_u32(assigned < 0 ? 0 : (unsigned int)assigned);
    host_print("\n");
    assigned = swarm_task_allocate(s, 5.0f, 5.0f);
    host_print("[phase 10] task at (5,5) assigned to: ");
    print_u32(assigned < 0 ? 0 : (unsigned int)assigned);
    host_print("\n");
    print_sep();

    /* --- phase 11: drone formation flight --- */
    host_print("[phase 11] drone formation flight...\n");
    for (i = 0; i < 10; i++) {
        swarm_drone_formation(s);
    }
    host_print("[phase 11] drone[2] pos=(");
    print_f32(s->agents[2].x);
    host_print(", ");
    print_f32(s->agents[2].y);
    host_print(")\n");
    print_sep();

    /* --- phase 12: warehouse routing --- */
    host_print("[phase 12] warehouse robot routing...\n");
    for (i = 0; i < 10; i++) {
        swarm_warehouse_route(s);
    }
    host_print("[phase 12] robot[4] pos=(");
    print_f32(s->agents[4].x);
    host_print(", ");
    print_f32(s->agents[4].y);
    host_print(")\n");
    print_sep();

    /* --- phase 13: search and coverage --- */
    host_print("[phase 13] search-and-rescue area coverage...\n");
    for (i = 0; i < 15; i++) {
        swarm_search_cover(s, 20.0f, 20.0f);
    }
    cov = swarm_sense_coverage(s);
    host_print("[phase 13] area coverage: "); print_f32(cov); host_print("\n");
    print_sep();

    /* --- phase 14: adapt to failures --- */
    host_print("[phase 14] simulating agent failure and self-repair...\n");
    s->agents[2].alive = 0;
    s->agents[7].alive = 0;
    host_print("[phase 14] disabled agents 2 and 7\n");
    for (i = 0; i < 5; i++) {
        swarm_self_repair(s);
    }
    host_print("[phase 14] repairs done: "); print_u32(s->repairs_done); host_print("\n");
    host_print("[phase 14] new count: "); print_u32(s->count); host_print("\n");
    swarm_topology_repair(s);
    print_sep();

    /* --- final statistics --- */
    swarm_print_stats(s);

    host_print("[swarm_intelligence] all phases complete, exiting cleanly\n");
    host_exit(0);
    return 0;
}
