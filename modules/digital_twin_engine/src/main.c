#include <stddef.h>

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_ENTITIES 64
#define MAX_COMPONENTS 16
#define MAX_SENSORS 32
#define STATUS_OK 0
#define STATUS_ERR -1

typedef struct {
    float x, y, z;
} Vec3;

typedef struct {
    float mass;
    Vec3 inertia;
    Vec3 position;
    Vec3 velocity;
    Vec3 orientation;
    Vec3 angular_velocity;
    int active;
} RigidBody;

typedef struct {
    int sensor_id;
    int type;
    float value;
    float noise_stddev;
    int active;
} Sensor;

typedef struct {
    int entity_id;
    int component_mask;
    RigidBody rigid_body;
    int sensor_count;
    Sensor sensors[MAX_SENSORS];
} Entity;

typedef struct {
    Entity entities[MAX_ENTITIES];
    int entity_count;
    float time_step;
    float current_time;
    int running;
} SimulationWorld;

static SimulationWorld* world = NULL;

static void print_str(const char* s) { host_print(s); }

static void print_int(int val) {
    char buf[32];
    int i = 0;
    if (val < 0) { buf[i++] = '-'; val = -val; }
    if (val == 0) { buf[i++] = '0'; }
    else {
        int start = i;
        while (val > 0) { buf[i++] = '0' + (val % 10); val /= 10; }
        int end = i - 1;
        while (start < end) { char t = buf[start]; buf[start] = buf[end]; buf[end] = t; start++; end--; }
    }
    buf[i] = '\0';
    host_print(buf);
}

static void print_float(float val) {
    int int_part = (int)val;
    float frac = val - (float)int_part;
    if (frac < 0) frac = -frac;
    int frac_int = (int)(frac * 1000.0f);
    print_int(int_part);
    print_str(".");
    if (frac_int < 100) print_str("0");
    if (frac_int < 10) print_str("0");
    print_int(frac_int);
}

int dt_init(void) {
    world = (SimulationWorld*)host_alloc(sizeof(SimulationWorld), 4);
    if (!world) return STATUS_ERR;
    world->entity_count = 0;
    world->time_step = 0.016f;
    world->current_time = 0.0f;
    world->running = 0;
    print_str("[DT] Digital twin engine initialized\n");
    return STATUS_OK;
}

int dt_create_entity(void) {
    if (world->entity_count >= MAX_ENTITIES) return STATUS_ERR;
    int id = world->entity_count++;
    Entity* e = &world->entities[id];
    e->entity_id = id;
    e->component_mask = 0;
    e->sensor_count = 0;
    e->rigid_body.active = 0;
    print_str("[DT] Created entity ");
    print_int(id);
    print_str("\n");
    return id;
}

int dt_add_rigid_body(int entity_id, float mass, float px, float py, float pz) {
    if (entity_id < 0 || entity_id >= world->entity_count) return STATUS_ERR;
    Entity* e = &world->entities[entity_id];
    RigidBody* rb = &e->rigid_body;
    rb->mass = mass;
    rb->inertia.x = mass * 0.1f;
    rb->inertia.y = mass * 0.1f;
    rb->inertia.z = mass * 0.1f;
    rb->position.x = px;
    rb->position.y = py;
    rb->position.z = pz;
    rb->velocity.x = 0.0f;
    rb->velocity.y = 0.0f;
    rb->velocity.z = 0.0f;
    rb->orientation.x = 0.0f;
    rb->orientation.y = 0.0f;
    rb->orientation.z = 0.0f;
    rb->angular_velocity.x = 0.0f;
    rb->angular_velocity.y = 0.0f;
    rb->angular_velocity.z = 0.0f;
    rb->active = 1;
    e->component_mask |= 0x01;
    print_str("[DT] Added rigid body to entity ");
    print_int(entity_id);
    print_str(" (mass=");
    print_float(mass);
    print_str(")\n");
    return STATUS_OK;
}

int dt_add_sensor(int entity_id, int sensor_type, float noise) {
    if (entity_id < 0 || entity_id >= world->entity_count) return STATUS_ERR;
    Entity* e = &world->entities[entity_id];
    if (e->sensor_count >= MAX_SENSORS) return STATUS_ERR;
    int sid = e->sensor_count++;
    Sensor* s = &e->sensors[sid];
    s->sensor_id = sid;
    s->type = sensor_type;
    s->value = 0.0f;
    s->noise_stddev = noise;
    s->active = 1;
    print_str("[DT] Added sensor ");
    print_int(sid);
    print_str(" type=");
    print_int(sensor_type);
    print_str(" to entity ");
    print_int(entity_id);
    print_str("\n");
    return sid;
}

void dt_update_physics(float dt) {
    float gravity = -9.81f;
    for (int i = 0; i < world->entity_count; i++) {
        Entity* e = &world->entities[i];
        if (!e->rigid_body.active) continue;
        RigidBody* rb = &e->rigid_body;
        rb->velocity.y += gravity * dt;
        rb->position.x += rb->velocity.x * dt;
        rb->position.y += rb->velocity.y * dt;
        rb->position.z += rb->velocity.z * dt;
        if (rb->position.y < 0.0f) {
            rb->position.y = 0.0f;
            rb->velocity.y = -rb->velocity.y * 0.8f;
        }
    }
}

void dt_update_sensors(void) {
    for (int i = 0; i < world->entity_count; i++) {
        Entity* e = &world->entities[i];
        if (!e->rigid_body.active) continue;
        RigidBody* rb = &e->rigid_body;
        for (int j = 0; j < e->sensor_count; j++) {
            Sensor* s = &e->sensors[j];
            if (!s->active) continue;
            if (s->type == 0) {
                s->value = rb->position.x + (s->noise_stddev * 0.01f);
            } else if (s->type == 1) {
                s->value = rb->velocity.y + (s->noise_stddev * 0.01f);
            }
        }
    }
}

int dt_step_simulation(void) {
    if (!world->running) return STATUS_ERR;
    dt_update_physics(world->time_step);
    dt_update_sensors();
    world->current_time += world->time_step;
    return STATUS_OK;
}

int dt_start_simulation(void) {
    world->running = 1;
    world->current_time = 0.0f;
    print_str("[DT] Simulation started\n");
    return STATUS_OK;
}

int dt_stop_simulation(void) {
    world->running = 0;
    print_str("[DT] Simulation stopped at time ");
    print_float(world->current_time);
    print_str("\n");
    return STATUS_OK;
}

void dt_print_entity_state(int entity_id) {
    if (entity_id < 0 || entity_id >= world->entity_count) return;
    Entity* e = &world->entities[entity_id];
    if (!e->rigid_body.active) return;
    RigidBody* rb = &e->rigid_body;
    print_str("Entity ");
    print_int(entity_id);
    print_str(": pos=(");
    print_float(rb->position.x);
    print_str(",");
    print_float(rb->position.y);
    print_str(",");
    print_float(rb->position.z);
    print_str(") vel=(");
    print_float(rb->velocity.x);
    print_str(",");
    print_float(rb->velocity.y);
    print_str(",");
    print_float(rb->velocity.z);
    print_str(") sensors=");
    print_int(e->sensor_count);
    print_str("\n");
}

int dt_kalman_filter(float* state, float measurement, float process_noise, float measurement_noise) {
    float predicted_state = *state;
    float predicted_variance = process_noise;
    float kalman_gain = predicted_variance / (predicted_variance + measurement_noise);
    *state = predicted_state + kalman_gain * (measurement - predicted_state);
    return STATUS_OK;
}

int dt_particle_filter(float* particles, int count, float measurement) {
    float total_weight = 0.0f;
    for (int i = 0; i < count; i++) {
        float diff = particles[i] - measurement;
        if (diff < 0) diff = -diff;
        float weight = 1.0f / (1.0f + diff);
        total_weight += weight;
    }
    return STATUS_OK;
}

void dt_print_stats(void) {
    print_str("[DT] Digital Twin Statistics:\n");
    print_str("  Entities: ");
    print_int(world->entity_count);
    print_str("\n");
    print_str("  Time: ");
    print_float(world->current_time);
    print_str("s\n");
    print_str("  Running: ");
    print_int(world->running);
    print_str("\n");
}

int main(void) {
    print_str("=== Digital Twin Engine Demo ===\n\n");
    dt_init();
    int e1 = dt_create_entity();
    dt_add_rigid_body(e1, 10.0f, 0.0f, 5.0f, 0.0f);
    dt_add_sensor(e1, 0, 0.1f);
    dt_add_sensor(e1, 1, 0.2f);
    int e2 = dt_create_entity();
    dt_add_rigid_body(e2, 5.0f, 2.0f, 10.0f, 0.0f);
    dt_add_sensor(e2, 0, 0.15f);
    print_str("\nInitial states:\n");
    dt_print_entity_state(e1);
    dt_print_entity_state(e2);
    dt_start_simulation();
    print_str("\nRunning simulation for 10 steps...\n");
    for (int i = 0; i < 10; i++) {
        dt_step_simulation();
    }
    print_str("\nAfter 10 steps:\n");
    dt_print_entity_state(e1);
    dt_print_entity_state(e2);
    dt_stop_simulation();
    dt_print_stats();
    float state = 5.0f;
    dt_kalman_filter(&state, 5.5f, 0.1f, 0.2f);
    print_str("\nKalman filter result: ");
    print_float(state);
    print_str("\n");
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
