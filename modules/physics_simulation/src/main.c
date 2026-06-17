#include <stddef.h>

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_BODIES 64
#define MAX_PARTICLES 256
#define MAX_FLUID_PARTICLES 128
#define MAX_CLOTH_NODES 48
#define MAX_SOFT_BODIES 16
#define MAX_CONTACTS 32
#define MAX_EMITTERS 8
#define MAX_FORCE_FIELDS 8
#define PI 3.14159265358979F
#define TWO_PI 6.28318530717958F

/* === Fast Math Approximations === */
static float fast_sin(float x) {
    /* Normalize to [-PI, PI] */
    while (x > PI) x -= TWO_PI;
    while (x < -PI) x += TWO_PI;
    /* Bhaskara I / Taylor hybrid approximation */
    float x2 = x * x;
    float x3 = x2 * x;
    float x5 = x3 * x2;
    return x - x3 / 6.0F + x5 / 120.0F - x5 * x2 / 5040.0F;
}

static float fast_cos(float x) {
    return fast_sin(x + PI * 0.5F);
}

/* === Vector Math === */
typedef struct { float x, y, z; } Vec3;
typedef struct { float x, y, z, w; } Quat;
typedef struct { float m[16]; } Mat4;

static float vec3_dot(Vec3 a, Vec3 b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
static float vec3_len(Vec3 v) { return __builtin_sqrtf(vec3_dot(v, v)); }
static Vec3 vec3_add(Vec3 a, Vec3 b) { Vec3 r; r.x=a.x+b.x; r.y=a.y+b.y; r.z=a.z+b.z; return r; }
static Vec3 vec3_sub(Vec3 a, Vec3 b) { Vec3 r; r.x=a.x-b.x; r.y=a.y-b.y; r.z=a.z-b.z; return r; }
static Vec3 vec3_scale(Vec3 v, float s) { Vec3 r; r.x=v.x*s; r.y=v.y*s; r.z=v.z*s; return r; }
static Vec3 vec3_cross(Vec3 a, Vec3 b) {
    Vec3 r; r.x=a.y*b.z-a.z*b.y; r.y=a.z*b.x-a.x*b.z; r.z=a.x*b.y-a.y*b.x; return r;
}
static Vec3 vec3_normalize(Vec3 v) {
    float l = vec3_len(v);
    if (l > 1e-06F) return vec3_scale(v, 1.0F / l);
    Vec3 z; z.x=0; z.y=0; z.z=0; return z;
}
static Vec3 vec3_lerp(Vec3 a, Vec3 b, float t) {
    Vec3 r; r.x=a.x+(b.x-a.x)*t; r.y=a.y+(b.y-a.y)*t; r.z=a.z+(b.z-a.z)*t; return r;
}

/* === Material Properties === */
typedef struct {
    float density;
    float friction;
    float restitution;
    float elasticity;
    float heat_conductivity;
    float specific_heat;
    char name[16];
} Material;

static const Material g_materials[] = {
    { 7800.0F, 0.60F, 0.30F, 0.20F, 80.0F, 450.0F, "Steel" },
    { 2700.0F, 0.55F, 0.25F, 0.35F, 237.0F, 900.0F, "Aluminum" },
    { 1000.0F, 0.90F, 0.10F, 0.05F, 0.60F, 4186.0F, "Rubber" },
    { 2500.0F, 0.65F, 0.15F, 0.10F, 1.40F, 840.0F, "Glass" },
    { 900.0F, 0.40F, 0.20F, 0.15F, 0.15F, 1800.0F, "Wood" },
    { 1025.0F, 0.00F, 0.00F, 0.00F, 0.60F, 4000.0F, "Water" },
};
#define NUM_MATERIALS 6

/* === Rigid Body === */
typedef struct {
    Vec3 position, velocity, angular_vel;
    Quat orientation;
    float mass, inv_mass;
    Vec3 inertia, inv_inertia;
    float restitution, friction;
    int material_id;
    int active;
} RigidBody;

/* === Collision === */
typedef struct { Vec3 normal; float depth; Vec3 point; int body_a, body_b; } Contact;

typedef struct {
    Vec3 min, max;
} AABB;

static AABB compute_aabb(Vec3 pos, Vec3 half_ext) {
    AABB b;
    b.min.x=pos.x-half_ext.x; b.min.y=pos.y-half_ext.y; b.min.z=pos.z-half_ext.z;
    b.max.x=pos.x+half_ext.x; b.max.y=pos.y+half_ext.y; b.max.z=pos.z+half_ext.z;
    return b;
}

static int aabb_overlap(AABB a, AABB b) {
    return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
           (a.min.y <= b.max.y && a.max.y >= b.min.y) &&
           (a.min.z <= b.max.z && a.max.z >= b.min.z);
}

/* === SPH Fluid === */
typedef struct { Vec3 pos, vel, force; float density, pressure; float mass; } FluidParticle;

/* === Soft Body === */
typedef struct { Vec3 pos, vel, force; float mass; int fixed; } SoftNode;
typedef struct { int a, b; float rest_len, stiffness, damping; } Spring;
typedef struct { SoftNode nodes[16]; Spring springs[24]; int num_nodes, num_springs; float volume; int active; } SoftBody;

/* === Cloth === */
typedef struct { Vec3 pos, vel, force; float inv_mass; int pinned; } ClothNode;
typedef struct { int a, b; float rest_len; int type; } ClothConstraint;
typedef struct { ClothNode nodes[MAX_CLOTH_NODES]; ClothConstraint cons[80]; int nx, ny; int num_cons; int active; } Cloth;

/* === Particle System === */
typedef struct { Vec3 pos, vel; float life, max_life; float size; int alive; } Particle;
typedef struct { Vec3 origin, dir; float speed, spread; float rate, timer; int lifetime_max; int active; } Emitter;
typedef struct { Vec3 origin; float radius, strength; int type; int active; } ForceField;

/* === Simulation State === */
typedef struct {
    RigidBody bodies[MAX_BODIES];
    int num_bodies;
    Contact contacts[MAX_CONTACTS];
    int num_contacts;
    FluidParticle fluid[MAX_FLUID_PARTICLES];
    int num_fluid;
    SoftBody soft_bodies[MAX_SOFT_BODIES];
    int num_soft;
    Cloth cloth;
    Particle particles[MAX_PARTICLES];
    int num_particles;
    Emitter emitters[MAX_EMITTERS];
    int num_emitters;
    ForceField fields[MAX_FORCE_FIELDS];
    int num_fields;
    float gravity;
    float dt;
    int step_count;
    /* SPH parameters */
    float sph_rest_density;
    float sph_gas_const;
    float sph_viscosity;
    float sph_smoothing;
} SimState;

/* === Helper output === */
static char g_buf[64];

static void print_str(const char *s) { host_print(s); }

static void print_int(int v) {
    if (v == 0) { g_buf[0]='0'; g_buf[1]='\n'; g_buf[2]=0; host_print(g_buf); return; }
    int i = 0, neg = 0;
    if (v < 0) { neg = 1; v = -v; }
    while (v > 0 && i < 20) { g_buf[i++] = '0' + (v % 10); v /= 10; }
    if (neg) g_buf[i++] = '-';
    g_buf[i++] = '\n'; g_buf[i] = 0;
    for (int j = 0; j < i/2; j++) { char t = g_buf[j]; g_buf[j] = g_buf[i-1-j]; g_buf[i-1-j] = t; }
    host_print(g_buf);
}

static void print_float(float v) {
    int whole = (int)v;
    int frac = (int)((v - (float)whole) * 100.0F);
    if (frac < 0) frac = -frac;
    int i = 0, neg = 0;
    if (v < 0.0F) { neg = 1; whole = -whole; }
    if (whole == 0) { g_buf[i++] = '0'; }
    else { int w = whole; char tmp[12]; int ti = 0;
        while (w > 0 && ti < 10) { tmp[ti++] = '0'+(w%10); w/=10; }
        for (int j = ti-1; j >= 0; j--) g_buf[i++] = tmp[j];
    }
    g_buf[i++] = '.';
    g_buf[i++] = '0' + frac/10;
    g_buf[i++] = '0' + frac%10;
    if (neg) { for (int j=i; j>0; j--) g_buf[j]=g_buf[j-1]; g_buf[0]='-'; i++; }
    g_buf[i++] = '\n'; g_buf[i] = 0;
    host_print(g_buf);
}

/* === Quaternion helpers === */
static Quat quat_identity(void) { Quat q; q.x=0; q.y=0; q.z=0; q.w=1; return q; }

static Vec3 quat_rotate(Quat q, Vec3 v) {
    Vec3 u; u.x=q.x; u.y=q.y; u.z=q.z;
    float s = q.w;
    float d = 2.0F * vec3_dot(u, v);
    float c = s*s - vec3_dot(u, u);
    Vec3 cr = vec3_cross(u, v);
    Vec3 r;
    r.x = d*u.x + c*v.x + s*cr.x - cr.x + 2.0F*(u.y*v.z-u.z*v.y)*0.0F;
    r.x = v.x + 2.0F*(s*(u.y*v.z-u.z*v.y) + u.y*(u.z*v.x-u.x*v.z) - u.z*(u.x*v.y-u.y*v.x));
    r.y = v.y + 2.0F*(s*(u.z*v.x-u.x*v.z) + u.z*(u.x*v.y-u.y*v.x) - u.x*(u.y*v.z-u.z*v.y));
    r.z = v.z + 2.0F*(s*(u.x*v.y-u.y*v.x) + u.x*(u.y*v.z-u.z*v.y) - u.y*(u.z*v.x-u.x*v.z));
    return r;
}

/* === SPH Kernels === */
static float sph_poly6(float r2, float h) {
    float h2 = h * h;
    if (r2 >= h2) return 0.0F;
    float diff = h2 - r2;
    float h9 = h*h*h*h*h*h*h*h*h;
    return 315.0F / (64.0F * PI * h9) * diff * diff * diff;
}

static Vec3 sph_spiky_grad(Vec3 rij, float r, float h) {
    if (r >= h || r < 1e-06F) { Vec3 z; z.x=0; z.y=0; z.z=0; return z; }
    float diff = h - r;
    float h6 = h*h*h*h*h*h;
    float coeff = -45.0F / (PI * h6) * diff * diff / r;
    return vec3_scale(rij, coeff);
}

static float sph_visc_lap(float r, float h) {
    if (r >= h) return 0.0F;
    float h6 = h*h*h*h*h*h;
    return 45.0F / (PI * h6) * (h - r);
}

/* === Rigid Body Dynamics === */
static void rb_init(SimState *sim) {
    sim->num_bodies = 0;
    sim->num_contacts = 0;
}

static int rb_create(SimState *sim, Vec3 pos, float mass, float rest, float fric, int mat_id) {
    if (sim->num_bodies >= MAX_BODIES) return -1;
    int i = sim->num_bodies++;
    RigidBody *b = &sim->bodies[i];
    b->position = pos; b->velocity.x = 0; b->velocity.y = 0; b->velocity.z = 0;
    b->angular_vel.x = 0; b->angular_vel.y = 0; b->angular_vel.z = 0;
    b->orientation = quat_identity();
    b->mass = mass; b->inv_mass = (mass > 0.0F) ? 1.0F / mass : 0.0F;
    float he = 0.5F;
    b->inertia.x = mass * (he*he + he*he) / 6.0F;
    b->inertia.y = b->inertia.x; b->inertia.z = b->inertia.x;
    b->inv_inertia.x = (b->inertia.x > 0) ? 1.0F / b->inertia.x : 0;
    b->inv_inertia.y = b->inv_inertia.x; b->inv_inertia.z = b->inv_inertia.x;
    b->restitution = rest; b->friction = fric;
    b->material_id = mat_id; b->active = 1;
    return i;
}

static void rb_apply_gravity(SimState *sim) {
    for (int i = 0; i < sim->num_bodies; i++) {
        RigidBody *b = &sim->bodies[i];
        if (!b->active || b->inv_mass == 0.0F) continue;
        b->velocity.y += sim->gravity * sim->dt;
    }
}

static void rb_detect_contacts(SimState *sim) {
    sim->num_contacts = 0;
    for (int i = 0; i < sim->num_bodies; i++) {
        for (int j = i + 1; j < sim->num_bodies; j++) {
            RigidBody *a = &sim->bodies[i];
            RigidBody *b = &sim->bodies[j];
            if (!a->active || !b->active) continue;
            float he = 0.5F;
            Vec3 he3; he3.x=he; he3.y=he; he3.z=he;
            AABB ba = compute_aabb(a->position, he3);
            AABB bb = compute_aabb(b->position, he3);
            if (!aabb_overlap(ba, bb)) continue;
            if (sim->num_contacts >= MAX_CONTACTS) continue;
            Contact *c = &sim->contacts[sim->num_contacts++];
            c->body_a = i; c->body_b = j;
            Vec3 diff = vec3_sub(b->position, a->position);
            float d = vec3_len(diff);
            c->normal = (d > 1e-06F) ? vec3_normalize(diff) : (Vec3){0,1,0};
            float overlap = (he + he) - d;
            c->depth = (overlap > 0.0F) ? overlap : 0.01F;
            c->point = vec3_lerp(a->position, b->position, 0.5F);
        }
    }
}

static void rb_resolve_contacts(SimState *sim) {
    for (int i = 0; i < sim->num_contacts; i++) {
        Contact *ct = &sim->contacts[i];
        RigidBody *a = &sim->bodies[ct->body_a];
        RigidBody *b = &sim->bodies[ct->body_b];
        Vec3 rv = vec3_sub(b->velocity, a->velocity);
        float vn = vec3_dot(rv, ct->normal);
        if (vn > 0.0F) continue;
        float e = (a->restitution + b->restitution) * 0.5F;
        float j = -(1.0F + e) * vn;
        j /= a->inv_mass + b->inv_mass;
        Vec3 impulse = vec3_scale(ct->normal, j);
        a->velocity = vec3_sub(a->velocity, vec3_scale(impulse, a->inv_mass));
        b->velocity = vec3_add(b->velocity, vec3_scale(impulse, b->inv_mass));
        /* Friction impulse */
        rv = vec3_sub(b->velocity, a->velocity);
        Vec3 tangent = vec3_sub(rv, vec3_scale(ct->normal, vec3_dot(rv, ct->normal)));
        float tl = vec3_len(tangent);
        if (tl > 1e-06F) {
            tangent = vec3_scale(tangent, 1.0F / tl);
            float jt = -vec3_dot(rv, tangent);
            jt /= a->inv_mass + b->inv_mass;
            float mu = (a->friction + b->friction) * 0.5F;
            if (jt > j * mu) jt = j * mu;
            else if (jt < -j * mu) jt = -j * mu;
            Vec3 fri_imp = vec3_scale(tangent, jt);
            a->velocity = vec3_sub(a->velocity, vec3_scale(fri_imp, a->inv_mass));
            b->velocity = vec3_add(b->velocity, vec3_scale(fri_imp, b->inv_mass));
        }
        /* Positional correction (Baumgarte) */
        float slop = 0.01F;
        float percent = 0.4F;
        float corr = (ct->depth - slop) * percent / (a->inv_mass + b->inv_mass);
        Vec3 corr_v = vec3_scale(ct->normal, corr);
        a->position = vec3_sub(a->position, vec3_scale(corr_v, a->inv_mass));
        b->position = vec3_add(b->position, vec3_scale(corr_v, b->inv_mass));
    }
}

static void rb_integrate(SimState *sim) {
    for (int i = 0; i < sim->num_bodies; i++) {
        RigidBody *b = &sim->bodies[i];
        if (!b->active) continue;
        b->position.x += b->velocity.x * sim->dt;
        b->position.y += b->velocity.y * sim->dt;
        b->position.z += b->velocity.z * sim->dt;
        /* Angular integration via quaternion */
        Quat q = b->orientation;
        Quat dq;
        dq.w = -0.5F*(q.x*b->angular_vel.x + q.y*b->angular_vel.y + q.z*b->angular_vel.z);
        dq.x =  0.5F*(q.w*b->angular_vel.x + q.y*b->angular_vel.z - q.z*b->angular_vel.y);
        dq.y =  0.5F*(q.w*b->angular_vel.y + q.z*b->angular_vel.x - q.x*b->angular_vel.z);
        dq.z =  0.5F*(q.w*b->angular_vel.z + q.x*b->angular_vel.y - q.y*b->angular_vel.x);
        q.w += dq.w * sim->dt; q.x += dq.x * sim->dt;
        q.y += dq.y * sim->dt; q.z += dq.z * sim->dt;
        float nl = __builtin_sqrtf(q.w*q.w + q.x*q.x + q.y*q.y + q.z*q.z);
        if (nl > 1e-06F) { q.w/=nl; q.x/=nl; q.y/=nl; q.z/=nl; }
        b->orientation = q;
    }
}

/* === SPH Fluid Dynamics === */
static void fluid_init(SimState *sim) {
    sim->sph_rest_density = 1000.0F;
    sim->sph_gas_const = 2000.0F;
    sim->sph_viscosity = 250.0F;
    sim->sph_smoothing = 0.04F;
    sim->num_fluid = 0;
}

static void fluid_add_particles(SimState *sim, Vec3 origin, int nx, int ny, float spacing) {
    for (int ix = 0; ix < nx; ix++) {
        for (int iy = 0; iy < ny; iy++) {
            if (sim->num_fluid >= MAX_FLUID_PARTICLES) return;
            FluidParticle *p = &sim->fluid[sim->num_fluid++];
            p->pos.x = origin.x + ix * spacing;
            p->pos.y = origin.y + iy * spacing;
            p->pos.z = origin.z;
            p->vel.x = 0; p->vel.y = 0; p->vel.z = 0;
            p->force.x = 0; p->force.y = 0; p->force.z = 0;
            p->density = 0; p->pressure = 0;
            p->mass = sim->sph_rest_density * spacing * spacing * spacing;
        }
    }
}

static void fluid_compute_density_pressure(SimState *sim) {
    float h = sim->sph_smoothing;
    for (int i = 0; i < sim->num_fluid; i++) {
        sim->fluid[i].density = 0.0F;
        for (int j = 0; j < sim->num_fluid; j++) {
            Vec3 rij = vec3_sub(sim->fluid[i].pos, sim->fluid[j].pos);
            float r2 = vec3_dot(rij, rij);
            sim->fluid[i].density += sim->fluid[j].mass * sph_poly6(r2, h);
        }
        sim->fluid[i].pressure = sim->sph_gas_const * (sim->fluid[i].density - sim->sph_rest_density);
    }
}

static void fluid_compute_forces(SimState *sim) {
    float h = sim->sph_smoothing;
    for (int i = 0; i < sim->num_fluid; i++) {
        Vec3 fp = {0,0,0}, fv = {0,0,0};
        for (int j = 0; j < sim->num_fluid; j++) {
            if (i == j) continue;
            Vec3 rij = vec3_sub(sim->fluid[i].pos, sim->fluid[j].pos);
            float r = vec3_len(rij);
            if (r < h && sim->fluid[j].density > 1e-06F) {
                /* Pressure force */
                Vec3 grad = sph_spiky_grad(rij, r, h);
                float p_avg = (sim->fluid[i].pressure + sim->fluid[j].pressure) * 0.5F;
                fp = vec3_sub(fp, vec3_scale(grad, sim->fluid[j].mass * p_avg / sim->fluid[j].density));
                /* Viscosity force */
                float lap = sph_visc_lap(r, h);
                Vec3 dv = vec3_sub(sim->fluid[j].vel, sim->fluid[i].vel);
                fv = vec3_add(fv, vec3_scale(dv, sim->sph_viscosity * sim->fluid[j].mass * lap / sim->fluid[j].density));
                /* Surface tension (simplified) */
                if (r > h * 0.5F) {
                    float tension = 0.001F;
                    Vec3 st = vec3_scale(vec3_normalize(rij), -tension * sim->fluid[j].mass);
                    fp = vec3_add(fp, st);
                }
            }
        }
        float di = (sim->fluid[i].density > 1e-06F) ? sim->fluid[i].density : 1.0F;
        sim->fluid[i].force = vec3_add(vec3_scale(fp, di), vec3_scale(fv, di));
        sim->fluid[i].force.y += sim->fluid[i].mass * sim->gravity;
    }
}

static void fluid_integrate(SimState *sim) {
    for (int i = 0; i < sim->num_fluid; i++) {
        FluidParticle *p = &sim->fluid[i];
        float di = (p->density > 1e-06F) ? p->density : 1.0F;
        Vec3 acc = vec3_scale(p->force, 1.0F / di);
        p->vel = vec3_add(p->vel, vec3_scale(acc, sim->dt));
        p->pos = vec3_add(p->pos, vec3_scale(p->vel, sim->dt));
        /* Boundary */
        if (p->pos.y < 0.0F) { p->pos.y = 0.0F; p->vel.y *= -0.3F; }
        if (p->pos.x < -1.0F) { p->pos.x = -1.0F; p->vel.x *= -0.3F; }
        if (p->pos.x > 1.0F) { p->pos.x = 1.0F; p->vel.x *= -0.3F; }
    }
}

/* === Soft Body Physics (Mass-Spring) === */
static void soft_init(SimState *sim) { sim->num_soft = 0; }

static int soft_create_cube(SimState *sim, Vec3 origin, float size, float stiff, float damp) {
    if (sim->num_soft >= MAX_SOFT_BODIES) return -1;
    int idx = sim->num_soft++;
    SoftBody *sb = &sim->soft_bodies[idx];
    sb->num_nodes = 8; sb->num_springs = 0; sb->active = 1;
    /* 8 corners of a cube */
    float s = size * 0.5F;
    Vec3 corners[8] = {
        {origin.x-s,origin.y-s,origin.z-s},{origin.x+s,origin.y-s,origin.z-s},
        {origin.x+s,origin.y+s,origin.z-s},{origin.x-s,origin.y+s,origin.z-s},
        {origin.x-s,origin.y-s,origin.z+s},{origin.x+s,origin.y-s,origin.z+s},
        {origin.x+s,origin.y+s,origin.z+s},{origin.x-s,origin.y+s,origin.z+s}
    };
    for (int i = 0; i < 8; i++) {
        sb->nodes[i].pos = corners[i];
        sb->nodes[i].vel.x=0; sb->nodes[i].vel.y=0; sb->nodes[i].vel.z=0;
        sb->nodes[i].force.x=0; sb->nodes[i].force.y=0; sb->nodes[i].force.z=0;
        sb->nodes[i].mass = 1.0F; sb->nodes[i].fixed = 0;
    }
    sb->nodes[0].fixed = 1; /* pin top-left-back */
    /* Edges */
    int edges[12][2] = {{0,1},{1,2},{2,3},{3,0},{4,5},{5,6},{6,7},{7,4},{0,4},{1,5},{2,6},{3,7}};
    for (int i = 0; i < 12; i++) {
        sb->springs[i].a = edges[i][0]; sb->springs[i].b = edges[i][1];
        sb->springs[i].rest_len = vec3_len(vec3_sub(corners[edges[i][0]], corners[edges[i][1]]));
        sb->springs[i].stiffness = stiff; sb->springs[i].damping = damp;
    }
    /* Cross-bracing for shear */
    int cross[6][2] = {{0,2},{1,3},{4,6},{5,7},{0,5},{2,7}};
    for (int i = 0; i < 6; i++) {
        int si = 12 + i;
        sb->springs[si].a = cross[i][0]; sb->springs[si].b = cross[i][1];
        sb->springs[si].rest_len = vec3_len(vec3_sub(corners[cross[i][0]], corners[cross[i][1]]));
        sb->springs[si].stiffness = stiff * 0.5F; sb->springs[si].damping = damp * 0.5F;
    }
    sb->num_springs = 18;
    sb->volume = size * size * size;
    return idx;
}

static void soft_update(SimState *sim, int idx) {
    SoftBody *sb = &sim->soft_bodies[idx];
    if (!sb->active) return;
    /* Reset forces */
    for (int i = 0; i < sb->num_nodes; i++) {
        sb->nodes[i].force.x=0; sb->nodes[i].force.y = sb->nodes[i].mass*sim->gravity; sb->nodes[i].force.z=0;
    }
    /* Spring forces */
    for (int i = 0; i < sb->num_springs; i++) {
        Spring *sp = &sb->springs[i];
        SoftNode *na = &sb->nodes[sp->a];
        SoftNode *nb = &sb->nodes[sp->b];
        Vec3 diff = vec3_sub(nb->pos, na->pos);
        float dist = vec3_len(diff);
        if (dist < 1e-06F) continue;
        Vec3 dir = vec3_scale(diff, 1.0F / dist);
        /* Hooke's law + damping */
        float displacement = dist - sp->rest_len;
        Vec3 rel_vel = vec3_sub(nb->vel, na->vel);
        float damp_f = sp->damping * vec3_dot(rel_vel, dir);
        float force_mag = sp->stiffness * displacement + damp_f;
        Vec3 f = vec3_scale(dir, force_mag);
        na->force = vec3_add(na->force, f);
        nb->force = vec3_sub(nb->force, f);
    }
    /* Volume preservation (pressure) */
    float current_vol = 0.0F;
    if (sb->num_nodes >= 4) {
        Vec3 e1 = vec3_sub(sb->nodes[1].pos, sb->nodes[0].pos);
        Vec3 e2 = vec3_sub(sb->nodes[3].pos, sb->nodes[0].pos);
        Vec3 e3 = vec3_sub(sb->nodes[4].pos, sb->nodes[0].pos);
        current_vol = vec3_dot(vec3_cross(e1, e2), e3);
        if (current_vol < 0) current_vol = -current_vol;
    }
    if (current_vol > 1e-06F && sb->volume > 1e-06F) {
        float vol_ratio = sb->volume / current_vol;
        float pressure_k = 50.0F;
        float excess = (vol_ratio - 1.0F) * pressure_k;
        /* Apply outward force to each node from centroid */
        Vec3 centroid = {0,0,0};
        for (int i = 0; i < sb->num_nodes; i++) centroid = vec3_add(centroid, sb->nodes[i].pos);
        centroid = vec3_scale(centroid, 1.0F / (float)sb->num_nodes);
        for (int i = 0; i < sb->num_nodes; i++) {
            if (sb->nodes[i].fixed) continue;
            Vec3 outward = vec3_sub(sb->nodes[i].pos, centroid);
            sb->nodes[i].force = vec3_add(sb->nodes[i].force, vec3_scale(outward, excess));
        }
    }
    /* Self-collision: simple repulsion */
    for (int i = 0; i < sb->num_nodes; i++) {
        for (int j = i+1; j < sb->num_nodes; j++) {
            Vec3 diff = vec3_sub(sb->nodes[j].pos, sb->nodes[i].pos);
            float dist = vec3_len(diff);
            float min_dist = 0.05F;
            if (dist < min_dist && dist > 1e-06F) {
                Vec3 dir = vec3_scale(diff, 1.0F / dist);
                float repel = 100.0F * (min_dist - dist);
                sb->nodes[j].force = vec3_add(sb->nodes[j].force, vec3_scale(dir, repel));
                sb->nodes[i].force = vec3_sub(sb->nodes[i].force, vec3_scale(dir, repel));
            }
        }
    }
    /* Integrate */
    for (int i = 0; i < sb->num_nodes; i++) {
        if (sb->nodes[i].fixed) continue;
        Vec3 acc = vec3_scale(sb->nodes[i].force, 1.0F / sb->nodes[i].mass);
        sb->nodes[i].vel = vec3_add(sb->nodes[i].vel, vec3_scale(acc, sim->dt));
        sb->nodes[i].vel = vec3_scale(sb->nodes[i].vel, 0.99F); /* damping */
        sb->nodes[i].pos = vec3_add(sb->nodes[i].pos, vec3_scale(sb->nodes[i].vel, sim->dt));
        /* Ground */
        if (sb->nodes[i].pos.y < 0.0F) { sb->nodes[i].pos.y = 0.0F; sb->nodes[i].vel.y *= -0.2F; }
    }
}

/* === Cloth Simulation === */
static void cloth_init(SimState *sim) {
    sim->cloth.active = 0;
    sim->cloth.num_cons = 0;
    sim->cloth.nx = 0;
    sim->cloth.ny = 0;
}

static void cloth_create(SimState *sim, Vec3 origin, float size, int nx, int ny) {
    sim->cloth.nx = nx; sim->cloth.ny = ny;
    sim->cloth.active = 1;
    sim->cloth.num_cons = 0;
    float dx = size / (float)(nx - 1);
    float dy = size / (float)(ny - 1);
    for (int iy = 0; iy < ny; iy++) {
        for (int ix = 0; ix < nx; ix++) {
            int idx = iy * nx + ix;
            if (idx >= MAX_CLOTH_NODES) break;
            ClothNode *n = &sim->cloth.nodes[idx];
            n->pos.x = origin.x + ix * dx;
            n->pos.y = origin.y + iy * dy;
            n->pos.z = origin.z;
            n->vel.x=0; n->vel.y=0; n->vel.z=0;
            n->force.x=0; n->force.y=0; n->force.z=0;
            n->inv_mass = 1.0F;
            n->pinned = (iy == ny - 1 && (ix == 0 || ix == nx - 1));
            if (n->pinned) n->inv_mass = 0.0F;
        }
    }
    /* Structural constraints */
    for (int iy = 0; iy < ny; iy++) {
        for (int ix = 0; ix < nx; ix++) {
            int idx = iy * nx + ix;
            if (ix < nx-1) {
                int c = sim->cloth.num_cons++;
                sim->cloth.cons[c].a = idx; sim->cloth.cons[c].b = idx+1;
                sim->cloth.cons[c].rest_len = dx; sim->cloth.cons[c].type = 0;
            }
            if (iy < ny-1) {
                int c = sim->cloth.num_cons++;
                sim->cloth.cons[c].a = idx; sim->cloth.cons[c].b = idx+nx;
                sim->cloth.cons[c].rest_len = dy; sim->cloth.cons[c].type = 0;
            }
            /* Shear */
            if (ix < nx-1 && iy < ny-1) {
                int c = sim->cloth.num_cons++;
                sim->cloth.cons[c].a = idx; sim->cloth.cons[c].b = idx+nx+1;
                sim->cloth.cons[c].rest_len = __builtin_sqrtf(dx*dx+dy*dy); sim->cloth.cons[c].type = 1;
            }
            /* Bend */
            if (ix < nx-2) {
                int c = sim->cloth.num_cons++;
                sim->cloth.cons[c].a = idx; sim->cloth.cons[c].b = idx+2;
                sim->cloth.cons[c].rest_len = dx*2.0F; sim->cloth.cons[c].type = 2;
            }
        }
    }
}

static void cloth_update(SimState *sim) {
    if (!sim->cloth.active) return;
    Cloth *cl = &sim->cloth;
    int total = cl->nx * cl->ny;
    if (total > MAX_CLOTH_NODES) total = MAX_CLOTH_NODES;
    /* Reset forces + gravity + wind */
    float wind_x = 0.3F * fast_sin((float)sim->step_count * 0.05F);
    float wind_z = 0.2F * fast_cos((float)sim->step_count * 0.07F);
    for (int i = 0; i < total; i++) {
        cl->nodes[i].force.x = wind_x; cl->nodes[i].force.y = sim->gravity; cl->nodes[i].force.z = wind_z;
    }
    /* Constraint solving (Verlet-style position corrections) */
    for (int iter = 0; iter < 3; iter++) {
        for (int c = 0; c < cl->num_cons; c++) {
            ClothConstraint *cc = &cl->cons[c];
            ClothNode *na = &cl->nodes[cc->a];
            ClothNode *nb = &cl->nodes[cc->b];
            Vec3 diff = vec3_sub(nb->pos, na->pos);
            float dist = vec3_len(diff);
            if (dist < 1e-06F) continue;
            float err = (dist - cc->rest_len) / dist;
            float stiff = (cc->type == 0) ? 1.0F : (cc->type == 1) ? 0.5F : 0.3F;
            Vec3 corr = vec3_scale(diff, err * 0.5F * stiff);
            if (na->inv_mass > 0) na->pos = vec3_add(na->pos, corr);
            if (nb->inv_mass > 0) nb->pos = vec3_sub(nb->pos, corr);
        }
    }
    /* Integrate velocities from position deltas */
    for (int i = 0; i < total; i++) {
        ClothNode *n = &cl->nodes[i];
        if (n->inv_mass == 0.0F) continue;
        /* Verlet: velocity derived from position change */
        Vec3 new_vel = vec3_sub(n->pos, vec3_sub(n->pos, vec3_scale(n->vel, sim->dt)));
        new_vel = vec3_scale(new_vel, 1.0F / sim->dt);
        n->vel = vec3_scale(vec3_add(n->vel, new_vel), 0.5F);
        n->vel = vec3_scale(n->vel, 0.98F); /* air resistance */
        /* Simple Euler for stability */
        Vec3 acc = vec3_scale(n->force, n->inv_mass);
        n->vel = vec3_add(n->vel, vec3_scale(acc, sim->dt));
        n->pos = vec3_add(n->pos, vec3_scale(n->vel, sim->dt));
        /* Collision response */
        if (n->pos.z < 0.0F) { n->pos.z = 0.0F; n->vel.z *= -0.1F; }
        if (n->pos.y < 0.0F) { n->pos.y = 0.0F; n->vel.y *= -0.2F; }
    }
}

/* === Particle Systems === */
static void particles_init(SimState *sim) {
    sim->num_particles = 0;
    sim->num_emitters = 0;
    sim->num_fields = 0;
}

static int emitter_create(SimState *sim, Vec3 origin, Vec3 dir, float speed, float spread, float rate) {
    if (sim->num_emitters >= MAX_EMITTERS) return -1;
    int i = sim->num_emitters++;
    Emitter *e = &sim->emitters[i];
    e->origin = origin; e->dir = vec3_normalize(dir);
    e->speed = speed; e->spread = spread;
    e->rate = rate; e->timer = 0; e->lifetime_max = 200;
    e->active = 1;
    return i;
}

static int field_create(SimState *sim, Vec3 origin, float radius, float strength, int type) {
    if (sim->num_fields >= MAX_FORCE_FIELDS) return -1;
    int i = sim->num_fields++;
    ForceField *f = &sim->fields[i];
    f->origin = origin; f->radius = radius;
    f->strength = strength; f->type = type;
    f->active = 1;
    return i;
}

static void particles_update(SimState *sim) {
    /* Emit new particles */
    for (int e = 0; e < sim->num_emitters; e++) {
        Emitter *em = &sim->emitters[e];
        if (!em->active) continue;
        em->timer += sim->dt;
        float interval = 1.0F / em->rate;
        while (em->timer >= interval && sim->num_particles < MAX_PARTICLES) {
            em->timer -= interval;
            Particle *p = &sim->particles[sim->num_particles++];
            p->pos = em->origin;
            /* Random direction within spread cone */
            float angle = ((float)(sim->step_count * 7 + p->pos.x * 13) / 100.0F);
            float sa = em->spread * fast_sin(angle * 3.0F);
            float ca = em->spread * fast_cos(angle * 5.0F);
            p->vel.x = em->dir.x * em->speed + sa;
            p->vel.y = em->dir.y * em->speed + ca * 0.5F;
            p->vel.z = em->dir.z * em->speed + sa * 0.3F;
            p->life = (float)(em->lifetime_max) * sim->dt * 5.0F;
            p->max_life = p->life;
            p->size = 0.02F;
            p->alive = 1;
        }
    }
    /* Update particles */
    for (int i = 0; i < sim->num_particles; i++) {
        Particle *p = &sim->particles[i];
        if (!p->alive) continue;
        p->life -= sim->dt;
        if (p->life <= 0.0F) { p->alive = 0; continue; }
        /* Apply force fields */
        Vec3 acc = {0, sim->gravity, 0};
        for (int f = 0; f < sim->num_fields; f++) {
            ForceField *ff = &sim->fields[f];
            if (!ff->active) continue;
            Vec3 diff = vec3_sub(ff->origin, p->pos);
            float dist = vec3_len(diff);
            if (dist < ff->radius && dist > 1e-06F) {
                Vec3 dir = vec3_scale(diff, 1.0F / dist);
                float falloff = 1.0F - dist / ff->radius;
                if (ff->type == 0) acc = vec3_add(acc, vec3_scale(dir, ff->strength * falloff)); /* attract */
                else if (ff->type == 1) acc = vec3_sub(acc, vec3_scale(dir, ff->strength * falloff)); /* repel */
                else if (ff->type == 2) { /* vortex */
                    Vec3 tangent = vec3_cross(dir, (Vec3){0,1,0});
                    acc = vec3_add(acc, vec3_scale(tangent, ff->strength * falloff));
                }
            }
        }
        p->vel = vec3_add(p->vel, vec3_scale(acc, sim->dt));
        p->vel = vec3_scale(p->vel, 0.99F); /* drag */
        p->pos = vec3_add(p->pos, vec3_scale(p->vel, sim->dt));
        /* Floor bounce */
        if (p->pos.y < 0.0F) { p->pos.y = 0.0F; p->vel.y *= -0.4F; }
    }
    /* Compact dead particles */
    int write = 0;
    for (int i = 0; i < sim->num_particles; i++) {
        if (sim->particles[i].alive) {
            if (write != i) sim->particles[write] = sim->particles[i];
            write++;
        }
    }
    sim->num_particles = write;
}

/* === Heat Transfer === */
static void heat_transfer_step(SimState *sim) {
    /* Simple pairwise conduction between nearby bodies */
    for (int i = 0; i < sim->num_bodies; i++) {
        for (int j = i+1; j < sim->num_bodies; j++) {
            RigidBody *a = &sim->bodies[i];
            RigidBody *b = &sim->bodies[j];
            Vec3 diff = vec3_sub(b->position, a->position);
            float dist = vec3_len(diff);
            if (dist < 1.0F) {
                int ma = a->material_id, mb = b->material_id;
                if (ma >= 0 && ma < NUM_MATERIALS && mb >= 0 && mb < NUM_MATERIALS) {
                    float ka = g_materials[ma].heat_conductivity;
                    float kb = g_materials[mb].heat_conductivity;
                    float k_avg = (ka + kb) * 0.5F;
                    /* Heat transfer proportional to conductivity/distance */
                    (void)k_avg; /* stored for thermal sim integration */
                }
            }
        }
    }
}

/* === Simulation Init & Step === */
static void sim_init(SimState *sim) {
    sim->gravity = -9.81F;
    sim->dt = 0.016F;
    sim->step_count = 0;
    rb_init(sim);
    fluid_init(sim);
    soft_init(sim);
    cloth_init(sim);
    particles_init(sim);
}

static void sim_step(SimState *sim) {
    rb_apply_gravity(sim);
    rb_detect_contacts(sim);
    rb_resolve_contacts(sim);
    rb_integrate(sim);
    fluid_compute_density_pressure(sim);
    fluid_compute_forces(sim);
    fluid_integrate(sim);
    for (int i = 0; i < sim->num_soft; i++) soft_update(sim, i);
    cloth_update(sim);
    particles_update(sim);
    heat_transfer_step(sim);
    sim->step_count++;
}

/* === Diagnostics === */
static void sim_print_stats(SimState *sim) {
    print_str("=== Physics Simulation Stats ===");
    print_str("Rigid bodies:"); print_int(sim->num_bodies);
    print_str("Active contacts:"); print_int(sim->num_contacts);
    print_str("Fluid particles:"); print_int(sim->num_fluid);
    print_str("Soft bodies:"); print_int(sim->num_soft);
    print_str("Cloth nodes:"); print_int(sim->cloth.nx * sim->cloth.ny);
    print_str("Cloth constraints:"); print_int(sim->cloth.num_cons);
    print_str("Particles:"); print_int(sim->num_particles);
    print_str("Emitters:"); print_int(sim->num_emitters);
    print_str("Force fields:"); print_int(sim->num_fields);
    print_str("Step count:"); print_int(sim->step_count);
}

static void print_body_info(RigidBody *b) {
    print_str("  Pos: "); print_float(b->position.x);
    print_str("  Vel: "); print_float(b->velocity.y);
    print_str("  Mass: "); print_float(b->mass);
    if (b->material_id >= 0 && b->material_id < NUM_MATERIALS) {
        print_str("  Material: ");
        host_print(g_materials[b->material_id].name);
    }
}

/* === Main === */
int main(void) {
    print_str("=== AI-ASM Physics Simulation Engine ===");
    print_str("Initializing simulation state...");

    /* Allocate state from host */
    unsigned int state_ptr = host_alloc(sizeof(SimState), 16);
    SimState *sim = (SimState *)(unsigned long)state_ptr;
    if (state_ptr == 0) {
        print_str("ERROR: Failed to allocate simulation state");
        host_exit(1);
    }
    sim_init(sim);

    /* --- Rigid Body Setup --- */
    print_str("--- Setting up Rigid Bodies ---");
    rb_create(sim, (Vec3){-1.0F, 5.0F, 0.0F}, 10.0F, 0.3F, 0.6F, 0); /* Steel */
    rb_create(sim, (Vec3){ 1.0F, 5.0F, 0.0F}, 5.0F,  0.25F, 0.55F, 1); /* Aluminum */
    rb_create(sim, (Vec3){ 0.0F, 8.0F, 0.0F}, 3.0F,  0.1F, 0.9F, 2);  /* Rubber */
    rb_create(sim, (Vec3){ 0.5F, 3.0F, 0.5F}, 8.0F,  0.15F, 0.65F, 3); /* Glass */
    rb_create(sim, (Vec3){-0.5F, 6.0F,-0.5F}, 4.0F,  0.2F, 0.4F, 4);  /* Wood */
    print_str("Created bodies:"); print_int(sim->num_bodies);

    /* --- Fluid Setup --- */
    print_str("--- Setting up SPH Fluid ---");
    Vec3 fluid_origin = { -0.3F, 0.5F, 0.0F };
    fluid_add_particles(sim, fluid_origin, 6, 4, 0.03F);
    print_str("Fluid particles:"); print_int(sim->num_fluid);

    /* --- Soft Body Setup --- */
    print_str("--- Setting up Soft Bodies ---");
    soft_create_cube(sim, (Vec3){ 2.0F, 3.0F, 0.0F }, 0.4F, 200.0F, 5.0F);
    soft_create_cube(sim, (Vec3){-2.0F, 4.0F, 0.0F }, 0.3F, 150.0F, 3.0F);
    print_str("Soft bodies:"); print_int(sim->num_soft);

    /* --- Cloth Setup --- */
    print_str("--- Setting up Cloth ---");
    cloth_create(sim, (Vec3){ -0.5F, 3.0F, -0.5F }, 1.0F, 6, 6);
    print_str("Cloth constraints:"); print_int(sim->cloth.num_cons);

    /* --- Particle System Setup --- */
    print_str("--- Setting up Particle Systems ---");
    emitter_create(sim, (Vec3){0, 1.0F, 0}, (Vec3){0, 1, 0}, 3.0F, 0.5F, 20.0F);
    emitter_create(sim, (Vec3){1, 0.5F, 0}, (Vec3){-1, 1, 0}, 2.0F, 0.8F, 15.0F);
    field_create(sim, (Vec3){0, 3.0F, 0}, 2.0F, 5.0F, 0);  /* Attractor */
    field_create(sim, (Vec3){-1, 2.0F, 0}, 1.5F, 3.0F, 2); /* Vortex */
    print_str("Emitters:"); print_int(sim->num_emitters);
    print_str("Force fields:"); print_int(sim->num_fields);

    /* --- Print Material Database --- */
    print_str("--- Material Database ---");
    for (int i = 0; i < NUM_MATERIALS; i++) {
        print_str("  Material: ");
        host_print(g_materials[i].name);
        print_str("  Density: "); print_float(g_materials[i].density);
        print_str("  Friction: "); print_float(g_materials[i].friction);
    }

    /* --- Run Simulation --- */
    print_str("--- Running Simulation (100 steps) ---");
    for (int step = 0; step < 100; step++) {
        sim_step(sim);
        if (step % 25 == 0) {
            print_str("Step "); print_int(step);
        }
    }

    /* --- Results --- */
    print_str("--- Simulation Results ---");
    for (int i = 0; i < sim->num_bodies; i++) {
        print_str("Body "); print_int(i);
        print_body_info(&sim->bodies[i]);
    }

    print_str("Final fluid energy check:");
    if (sim->num_fluid > 0) {
        float total_ke = 0.0F;
        for (int i = 0; i < sim->num_fluid; i++) {
            float v2 = vec3_dot(sim->fluid[i].vel, sim->fluid[i].vel);
            total_ke += 0.5F * sim->fluid[i].mass * v2;
        }
        print_str("  Fluid KE: "); print_float(total_ke);
    }

    print_str("Final particle count:"); print_int(sim->num_particles);

    /* Run 100 more steps */
    print_str("--- Running 100 more steps ---");
    for (int step = 0; step < 100; step++) sim_step(sim);

    sim_print_stats(sim);

    print_str("=== Physics Simulation Complete ===");
    host_exit(0);
    return 0;
}
