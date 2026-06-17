/*
 * real_time_sync - Real-Time Synchronization System Module
 * AI-ASM OS: WebAssembly-native operating system
 * Network sync, time management, compression, consistency, distributed state, replay
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

/* ── Helpers ─────────────────────────────────────────────────────────── */
static void print_str(const char *s) { host_print(s); }
static char int_buf[24];
static void print_int(int v) {
    char *p = int_buf + 23;
    int neg = v < 0;
    unsigned int u = neg ? (unsigned int)(-(v + 1)) + 1U : (unsigned int)v;
    *p = 0;
    if (u == 0) { *(--p) = '0'; }
    else { while (u) { *(--p) = '0' + (char)(u % 10); u /= 10; } }
    if (neg) *(--p) = '-';
    host_print(p);
}
static void print_nl(void) { host_print("\n"); }
static void print_kv(const char *k, int v) {
    print_str(k); print_str(": "); print_int(v); print_nl();
}

/* ── Constants ───────────────────────────────────────────────────────── */
#define MAX_PEERS 32
#define MAX_ACTIONS 1024
#define MAX_SNAPSHOTS 64
#define MAX_SHARDS 16
#define MAX_REPLICAS 8
#define STATE_BUF_SIZE 4096
#define DELTA_BUF_SIZE 2048
#define SHARD_STATE_SIZE 1024
#define FRAME_HIST_SIZE 128
#define SNAP_STATE_SIZE 2048
#define VCLOCK_SIZE 32
#define MAX_PENDING 64
#define CLOCK_SAMPLES 16
#define CONSISTENCY_STRONG 0
#define CONSISTENCY_EVENTUAL 1
#define CONSISTENCY_CAUSAL 2
#define ACTION_MOVE 0x01
#define ACTION_ATTACK 0x02
#define ACTION_USE_ITEM 0x03
#define ACTION_CHAT 0x04
#define CHECKSUM_SEED 0xDEADBEEF

/* ── Data structures ─────────────────────────────────────────────────── */
typedef struct {
    unsigned int peer_id, sequence, ack_seq;
    int rtt_ms, clock_offset_us, drift_rate_ppm;
    unsigned int last_sync_time, state_version, state_size;
    unsigned char state_buf[256]; int connected;
} PeerState;
typedef struct {
    unsigned int frame_number, authoritative[16], predicted[16];
    int correction_applied; unsigned int rollback_count;
} FrameHistory;
typedef struct {
    long long local_time_us, remote_time_us, offset_us, last_sample_time;
    int drift_ppm, sample_count, sample_index;
    long long samples[CLOCK_SAMPLES];
} ClockSync;
typedef struct {
    long long virtual_time_us, real_time_us, interpolation_delay_us, last_tick_us;
    int time_scale_pct;
} TimeManager;
typedef struct {
    unsigned char buffer[STATE_BUF_SIZE];
    unsigned int size, compressed_size, checksum, version;
} CompressedState;
typedef struct {
    unsigned int base_version, current_version, delta_size;
    unsigned char delta_buffer[DELTA_BUF_SIZE];
} IncrementalSync;
typedef struct {
    unsigned int keyframe_interval, last_keyframe_ver, delta_count, keyframe_size;
    unsigned char keyframe_buf[STATE_BUF_SIZE];
} KeyframeCompressor;
typedef struct {
    unsigned int version, timestamp, peer_id, data_size, checksum;
    unsigned int causal_clock[VCLOCK_SIZE]; unsigned char data[256];
} VersionedState;
typedef struct {
    unsigned int local_clock[VCLOCK_SIZE];
    VersionedState pending[MAX_PENDING]; int pending_count, consistency_model;
} ConsistencyManager;
typedef struct {
    unsigned int shard_id, primary_peer, version, state_size;
    unsigned int replica_peers[MAX_REPLICAS];
    int replica_count, healthy; unsigned char state[SHARD_STATE_SIZE];
} StateShard;
typedef struct {
    StateShard shards[MAX_SHARDS]; int shard_count; unsigned int local_peer_id;
} DistributedState;
typedef struct {
    unsigned int action_id, frame, peer_id, action_type, data[8], data_size, timestamp;
} Action;
typedef struct {
    Action actions[MAX_ACTIONS];
    unsigned int action_count, current_frame, head_index;
    int playback_speed_pct, playing;
} ReplayBuffer;
typedef struct {
    unsigned int frame, state_size, checksum; unsigned char state[SNAP_STATE_SIZE];
} Snapshot;
typedef struct {
    Snapshot snapshots[MAX_SNAPSHOTS]; unsigned int snapshot_count, snapshot_interval;
} SnapshotManager;
typedef struct {
    PeerState peers[MAX_PEERS]; int peer_count;
    FrameHistory frame_history[FRAME_HIST_SIZE]; int frame_history_count;
    ClockSync clock_sync; TimeManager time_mgr;
    ConsistencyManager consistency; DistributedState distributed;
    ReplayBuffer replay; SnapshotManager snapshots;
    unsigned int local_frame, local_peer_id, bandwidth_bytes_sec, total_bytes_sent;
    int running;
} RealTimeSync;

/* ── Utility ─────────────────────────────────────────────────────────── */
static unsigned int compute_checksum(const unsigned char *d, unsigned int n) {
    unsigned int h = CHECKSUM_SEED;
    for (unsigned int i = 0; i < n; i++) h = ((h << 5) + h) + d[i];
    return h;
}
static void mem_copy(unsigned char *dst, const unsigned char *src, unsigned int n) {
    for (unsigned int i = 0; i < n; i++) dst[i] = src[i];
}
static void mem_zero(unsigned char *dst, unsigned int n) {
    for (unsigned int i = 0; i < n; i++) dst[i] = 0;
}
static int mem_equal(const unsigned char *a, const unsigned char *b, unsigned int n) {
    for (unsigned int i = 0; i < n; i++) if (a[i] != b[i]) return 0;
    return 1;
}
static int abs_val(int x) { return x < 0 ? -x : x; }
static int clamp(int v, int lo, int hi) { return v < lo ? lo : (v > hi ? hi : v); }

/* ── Network synchronization ─────────────────────────────────────────── */static void peer_init(PeerState *p, unsigned int id) {
    p->peer_id = id; p->sequence = 0; p->ack_seq = 0;
    p->rtt_ms = 0; p->clock_offset_us = 0; p->drift_rate_ppm = 0;
    p->last_sync_time = 0; p->state_version = 0; p->state_size = 0; p->connected = 1;
}
static void peer_send_state(PeerState *p, const unsigned char *s, unsigned int sz) {
    unsigned int n = sz < 256 ? sz : 256;
    mem_copy(p->state_buf, s, n); p->state_size = n;
    p->state_version++; p->sequence++;
}
static int peer_receive_state(PeerState *p, unsigned char *o, unsigned int *osz) {
    if (!p->connected || p->state_size == 0) return 0;
    mem_copy(o, p->state_buf, p->state_size); *osz = p->state_size; return 1;
}
static void peer_update_rtt(PeerState *p, int rtt) { p->rtt_ms = (p->rtt_ms * 7 + rtt) >> 3; }
static void frame_sync_init(FrameHistory *fh, unsigned int frame) {
    fh->frame_number = frame; fh->correction_applied = 0; fh->rollback_count = 0;
    for (int i = 0; i < 16; i++) { fh->authoritative[i] = 0; fh->predicted[i] = 0; }
}
static int frame_sync_detect_error(FrameHistory *fh) {
    int err = 0;
    for (int i = 0; i < 16; i++) err += abs_val((int)fh->authoritative[i] - (int)fh->predicted[i]);
    return err;
}static void frame_sync_rollback(RealTimeSync *sys, unsigned int target) {
    if (target >= sys->local_frame) return;
    unsigned int back = sys->local_frame - target;
    int idx = (int)(sys->local_frame % FRAME_HIST_SIZE) - (int)back;
    if (idx < 0) idx += FRAME_HIST_SIZE;
    FrameHistory *fh = &sys->frame_history[idx];
    for (int i = 0; i < 16; i++) fh->predicted[i] = fh->authoritative[i];
    fh->correction_applied = 1; fh->rollback_count++;
}
static void lag_compensate(PeerState *p, int *pos, int vel, int rtt_ms) {
    *pos = *pos + (vel * (rtt_ms >> 1)) / 1000; (void)p;
}
/* ── Time management ─────────────────────────────────────────────────── */
static void clock_sync_init(ClockSync *cs) {
    cs->local_time_us = 0; cs->remote_time_us = 0; cs->offset_us = 0;
    cs->drift_ppm = 0; cs->last_sample_time = 0; cs->sample_count = 0; cs->sample_index = 0;
    for (int i = 0; i < CLOCK_SAMPLES; i++) cs->samples[i] = 0;
}
static void clock_sync_add_sample(ClockSync *cs, long long loc, long long rem) {
    int idx = cs->sample_index % CLOCK_SAMPLES;
    cs->samples[idx] = rem - loc; cs->sample_index++;
    if (cs->sample_count < CLOCK_SAMPLES) cs->sample_count++;
    long long sorted[CLOCK_SAMPLES];
    for (int i = 0; i < cs->sample_count; i++) sorted[i] = cs->samples[i];
    for (int i = 0; i < cs->sample_count - 1; i++)
        for (int j = i + 1; j < cs->sample_count; j++)
            if (sorted[j] < sorted[i]) { long long t = sorted[i]; sorted[i] = sorted[j]; sorted[j] = t; }
    cs->offset_us = sorted[cs->sample_count >> 1];
    cs->local_time_us = loc; cs->remote_time_us = rem;
}
static void clock_sync_correct_drift(ClockSync *cs, long long el_loc, long long el_rem) {
    if (el_loc == 0) return;
    cs->drift_ppm = (int)(((el_rem - el_loc) * 1000000LL) / el_loc);
}
static long long clock_sync_get_corrected(ClockSync *cs, long long now) { return now + cs->offset_us; }
static void time_mgr_init(TimeManager *tm, long long start) {    tm->virtual_time_us = start; tm->real_time_us = start; tm->time_scale_pct = 100;
    tm->interpolation_delay_us = 100000; tm->last_tick_us = start;
}
static void time_mgr_tick(TimeManager *tm, long long real_now) {
    long long delta = real_now - tm->last_tick_us;
    tm->real_time_us = real_now;
    tm->virtual_time_us += (delta * tm->time_scale_pct) / 100;
    tm->last_tick_us = real_now;
}
static long long time_mgr_interpolate(TimeManager *tm, long long server_time) {
    long long target = server_time - tm->interpolation_delay_us;
    return target < tm->virtual_time_us ? tm->virtual_time_us : target;
}
static void time_mgr_set_scale(TimeManager *tm, int pct) { tm->time_scale_pct = clamp(pct, 10, 400); }
/* ── Data compression ────────────────────────────────────────────────── */
static unsigned int compress_rle(const unsigned char *in, unsigned int isz,
                                  unsigned char *out, unsigned int ocap) {
    unsigned int oi = 0, i = 0;
    while (i < isz && oi + 2 < ocap) {
        unsigned char val = in[i]; unsigned int run = 1;
        while (i + run < isz && in[i + run] == val && run < 255) run++;
        out[oi++] = (unsigned char)run; out[oi++] = val; i += run;
    }
    return oi;
}
static unsigned int decompress_rle(const unsigned char *in, unsigned int isz,
                                    unsigned char *out, unsigned int ocap) {
    unsigned int oi = 0, i = 0;
    while (i + 1 < isz && oi < ocap) {
        unsigned int run = in[i]; unsigned char val = in[i + 1];
        for (unsigned int r = 0; r < run && oi < ocap; r++) out[oi++] = val;
        i += 2;
    }
    return oi;
}
static void compressed_state_snap(CompressedState *cs, const unsigned char *st, unsigned int sz, unsigned int ver) {
    cs->compressed_size = compress_rle(st, sz, cs->buffer, STATE_BUF_SIZE);
    cs->size = sz; cs->version = ver; cs->checksum = compute_checksum(st, sz);
}
static void incremental_init(IncrementalSync *inc, unsigned int bver) {
    inc->base_version = bver; inc->current_version = bver; inc->delta_size = 0;
}
static unsigned int incremental_delta(IncrementalSync *inc, const unsigned char *old_s, unsigned int old_n,
                                       const unsigned char *new_s, unsigned int new_n) {
    unsigned int oi = 0, max_n = old_n < new_n ? old_n : new_n;
    for (unsigned int i = 0; i < max_n && oi + 5 < DELTA_BUF_SIZE; i++) {
        if (old_s[i] != new_s[i]) {
            inc->delta_buffer[oi++] = (unsigned char)(i >> 8);
            inc->delta_buffer[oi++] = (unsigned char)(i & 0xFF);
            unsigned int run = 1;
            while (i + run < max_n && old_s[i + run] != new_s[i + run] && run < 200) run++;
            inc->delta_buffer[oi++] = (unsigned char)run;
            for (unsigned int r = 0; r < run && oi < DELTA_BUF_SIZE; r++) inc->delta_buffer[oi++] = new_s[i + r];
            i += run - 1;
        }
    }
    inc->delta_size = oi; inc->current_version++; return oi;
}
static void keyframe_init(KeyframeCompressor *kc, unsigned int interval) {
    kc->keyframe_interval = interval; kc->last_keyframe_ver = 0;
    kc->keyframe_size = 0; kc->delta_count = 0;
}
static int keyframe_should_emit(KeyframeCompressor *kc) { return (unsigned int)kc->delta_count >= kc->keyframe_interval; }
static void keyframe_emit(KeyframeCompressor *kc, const unsigned char *st, unsigned int sz) {
    kc->keyframe_size = compress_rle(st, sz, kc->keyframe_buf, STATE_BUF_SIZE);
    kc->delta_count = 0;
}
static unsigned int estimate_bandwidth(unsigned int bytes, unsigned int ms) {
    return ms == 0 ? 0 : (bytes * 1000) / ms;
}
/* ── Consistency guarantees ──────────────────────────────────────────── */
static void consistency_init(ConsistencyManager *cm, int model) {
    cm->consistency_model = model; cm->pending_count = 0;
    for (int i = 0; i < VCLOCK_SIZE; i++) cm->local_clock[i] = 0;
}
static void consistency_inc_clock(ConsistencyManager *cm, unsigned int pid) {
    if (pid < VCLOCK_SIZE) cm->local_clock[pid]++;
}
static int happens_before(const unsigned int *a, const unsigned int *b) {
    int less = 0;
    for (int i = 0; i < VCLOCK_SIZE; i++) {
        if (a[i] > b[i]) return 0;
        if (a[i] < b[i]) less = 1;
    }
    return less;
}
static int concurrent(const unsigned int *a, const unsigned int *b) {
    return !happens_before(a, b) && !happens_before(b, a);
}
static void consistency_submit(ConsistencyManager *cm, const VersionedState *vs) {
    if (cm->pending_count >= MAX_PENDING) return;
    if (cm->consistency_model == CONSISTENCY_CAUSAL && cm->pending_count > 0) {
        if (!happens_before(vs->causal_clock, cm->local_clock)) return;
    }
    cm->pending[cm->pending_count++] = *vs;
}
static unsigned int resolve_lww(const VersionedState *a, const VersionedState *b) {
    if (a->timestamp > b->timestamp) return a->peer_id;
    if (b->timestamp > a->timestamp) return b->peer_id;
    return a->peer_id < b->peer_id ? a->peer_id : b->peer_id;
}
static unsigned int resolve_vclock(const VersionedState *a, const VersionedState *b) {
    if (happens_before(a->causal_clock, b->causal_clock)) return b->peer_id;
    if (happens_before(b->causal_clock, a->causal_clock)) return a->peer_id;
    return a->peer_id < b->peer_id ? b->peer_id : a->peer_id;
}
/* ── Distributed state ───────────────────────────────────────────────── */
static void shard_init(StateShard *s, unsigned int id, unsigned int pri) {
    s->shard_id = id; s->primary_peer = pri; s->replica_count = 0;
    s->version = 0; s->state_size = 0; s->healthy = 1;
    for (int i = 0; i < MAX_REPLICAS; i++) s->replica_peers[i] = 0;
}
static int shard_add_replica(StateShard *s, unsigned int pid) {
    if (s->replica_count >= MAX_REPLICAS) return 0;
    s->replica_peers[s->replica_count++] = pid; return 1;
}
static void shard_update(StateShard *s, const unsigned char *st, unsigned int sz) {
    unsigned int n = sz < SHARD_STATE_SIZE ? sz : SHARD_STATE_SIZE;
    mem_copy(s->state, st, n); s->state_size = n; s->version++;
}
static int shard_available(StateShard *s) { return s->healthy && s->state_size > 0; }
static void distributed_init(DistributedState *ds, unsigned int local) {
    ds->local_peer_id = local; ds->shard_count = 0;
    for (int i = 0; i < MAX_SHARDS; i++) shard_init(&ds->shards[i], (unsigned int)i, local);
}
static unsigned int hash_key(const unsigned char *k, unsigned int n) {
    unsigned int h = 0x811C9DC5;
    for (unsigned int i = 0; i < n; i++) { h ^= k[i]; h *= 0x01000193; }
    return h;
}
static int distributed_route(DistributedState *ds, const unsigned char *k, unsigned int n) {
    return (int)(hash_key(k, n) % (unsigned int)ds->shard_count);
}
static int distributed_migrate(DistributedState *ds, int idx, unsigned int new_pri) {
    if (idx < 0 || idx >= ds->shard_count) return 0;
    ds->shards[idx].primary_peer = new_pri; ds->shards[idx].replica_count = 0; return 1;
}
static int distributed_recover(DistributedState *ds, int idx) {
    if (idx < 0 || idx >= ds->shard_count) return 0;
    StateShard *s = &ds->shards[idx];
    if (s->replica_count <= 0) return 0;
    s->primary_peer = s->replica_peers[0];
    for (int i = 0; i < s->replica_count - 1; i++) s->replica_peers[i] = s->replica_peers[i + 1];
    s->replica_count--; s->healthy = 1; return 1;
}
/* ── Replay system ───────────────────────────────────────────────────── */
static void replay_init(ReplayBuffer *rb) {
    rb->action_count = 0; rb->current_frame = 0;
    rb->playback_speed_pct = 100; rb->playing = 0; rb->head_index = 0;
}
static int replay_record(ReplayBuffer *rb, unsigned int frame, unsigned int peer,
                          unsigned int type, const unsigned int *data, unsigned int dsz) {
    if (rb->action_count >= MAX_ACTIONS) return 0;
    Action *a = &rb->actions[rb->action_count];
    a->action_id = rb->action_count; a->frame = frame; a->peer_id = peer; a->action_type = type;
    unsigned int n = dsz < 8 ? dsz : 8;
    for (unsigned int i = 0; i < n; i++) a->data[i] = data[i];
    a->data_size = n; a->timestamp = frame; rb->action_count++; return 1;
}
static void snapshot_init(SnapshotManager *sm, unsigned int interval) {
    sm->snapshot_count = 0; sm->snapshot_interval = interval;
}
static int snapshot_save(SnapshotManager *sm, unsigned int frame, const unsigned char *st, unsigned int sz) {
    if (sm->snapshot_count >= MAX_SNAPSHOTS) {
        for (unsigned int i = 0; i + 1 < MAX_SNAPSHOTS; i++) sm->snapshots[i] = sm->snapshots[i + 1];
        sm->snapshot_count = MAX_SNAPSHOTS - 1;
    }
    Snapshot *sn = &sm->snapshots[sm->snapshot_count];
    sn->frame = frame;
    unsigned int n = sz < SNAP_STATE_SIZE ? sz : SNAP_STATE_SIZE;
    mem_copy(sn->state, st, n); sn->state_size = n;
    sn->checksum = compute_checksum(st, n); sm->snapshot_count++; return 1;
}
static int snapshot_load(SnapshotManager *sm, unsigned int frame, unsigned char *out, unsigned int *osz) {
    int best = -1;
    for (unsigned int i = 0; i < sm->snapshot_count; i++)
        if (sm->snapshots[i].frame <= frame) best = (int)i;
    if (best < 0) return 0;
    Snapshot *sn = &sm->snapshots[best];
    mem_copy(out, sn->state, sn->state_size); *osz = sn->state_size; return 1;
}
static void replay_play(ReplayBuffer *rb) { rb->playing = 1; }
static void replay_pause(ReplayBuffer *rb) { rb->playing = 0; }
static void replay_set_speed(ReplayBuffer *rb, int pct) { rb->playback_speed_pct = clamp(pct, 10, 800); }static int replay_step(ReplayBuffer *rb, unsigned int *out_frame) {
    if (!rb->playing || rb->head_index >= rb->action_count) return 0;
    *out_frame = rb->actions[rb->head_index].frame;
    rb->current_frame = *out_frame; rb->head_index++; return 1;
}
static void time_travel_to(RealTimeSync *sys, unsigned int target) {
    unsigned char buf[SNAP_STATE_SIZE]; unsigned int sz = 0;
    if (snapshot_load(&sys->snapshots, target, buf, &sz)) { sys->local_frame = target; (void)buf; }
}
/* ── System orchestration ────────────────────────────────────────────── */
static void sync_init(RealTimeSync *sys, unsigned int local) {
    sys->peer_count = 0; sys->frame_history_count = 0;
    sys->local_frame = 0; sys->local_peer_id = local;
    sys->running = 1; sys->bandwidth_bytes_sec = 0; sys->total_bytes_sent = 0;
    for (int i = 0; i < MAX_PEERS; i++) sys->peers[i].connected = 0;
    for (int i = 0; i < FRAME_HIST_SIZE; i++) frame_sync_init(&sys->frame_history[i], 0);
    clock_sync_init(&sys->clock_sync);
    time_mgr_init(&sys->time_mgr, 0);
    consistency_init(&sys->consistency, CONSISTENCY_CAUSAL);
    distributed_init(&sys->distributed, local);
    replay_init(&sys->replay);
    snapshot_init(&sys->snapshots, 30);
}
static int sync_add_peer(RealTimeSync *sys, unsigned int pid) {
    if (sys->peer_count >= MAX_PEERS) return 0;
    peer_init(&sys->peers[sys->peer_count++], pid); return 1;
}
static void sync_tick(RealTimeSync *sys, long long real_us) {
    time_mgr_tick(&sys->time_mgr, real_us); sys->local_frame++;
    frame_sync_init(&sys->frame_history[sys->local_frame % FRAME_HIST_SIZE], sys->local_frame);
    if (sys->local_frame % sys->snapshots.snapshot_interval == 0) {
        unsigned char ss[256]; mem_zero(ss, 256);
        ss[0] = (unsigned char)(sys->local_frame & 0xFF);
        snapshot_save(&sys->snapshots, sys->local_frame, ss, 256);
    }
}
static void sync_broadcast(RealTimeSync *sys, const unsigned char *st, unsigned int sz) {
    for (int i = 0; i < sys->peer_count; i++)
        if (sys->peers[i].connected) { peer_send_state(&sys->peers[i], st, sz); sys->total_bytes_sent += sz; }
}
static void sync_shutdown(RealTimeSync *sys) {
    sys->running = 0;
    for (int i = 0; i < sys->peer_count; i++) sys->peers[i].connected = 0;
}
/* ── Main demo ───────────────────────────────────────────────────────── */
int main(void) {
    print_str("=== real_time_sync module v1.0 ==="); print_nl();
    unsigned int ctx_ptr = host_alloc(sizeof(RealTimeSync), 8);
    if (ctx_ptr == 0) { print_str("FATAL: alloc failed"); print_nl(); host_exit(1); return 1; }
    RealTimeSync *sys = (RealTimeSync *)(unsigned long)ctx_ptr;
    sync_init(sys, 1);
    print_str("[network-sync]"); print_nl();
    sync_add_peer(sys, 2); sync_add_peer(sys, 3); sync_add_peer(sys, 4);
    print_kv("  peers_connected", sys->peer_count);
    unsigned char demo[64];
    for (int i = 0; i < 64; i++) demo[i] = (unsigned char)i;
    sync_broadcast(sys, demo, 64);
    print_kv("  bytes_sent", (int)sys->total_bytes_sent);
    peer_update_rtt(&sys->peers[0], 48);
    peer_update_rtt(&sys->peers[0], 52);
    peer_update_rtt(&sys->peers[0], 45);
    print_kv("  peer2_rtt_ms", sys->peers[0].rtt_ms);
    int pos = 1000;
    lag_compensate(&sys->peers[0], &pos, 200, sys->peers[0].rtt_ms);
    print_kv("  lag_comp_pos", pos);
    print_str("[time-mgmt]"); print_nl();
    clock_sync_add_sample(&sys->clock_sync, 1000000LL, 1000050LL);
    clock_sync_add_sample(&sys->clock_sync, 2000000LL, 2000055LL);
    clock_sync_add_sample(&sys->clock_sync, 3000000LL, 3000048LL);
    print_kv("  clock_offset_us", (int)sys->clock_sync.offset_us);
    clock_sync_correct_drift(&sys->clock_sync, 1000000LL, 1000005LL);
    print_kv("  drift_ppm", sys->clock_sync.drift_ppm);
    print_kv("  corrected_time_us", (int)clock_sync_get_corrected(&sys->clock_sync, 5000000LL));
    time_mgr_set_scale(&sys->time_mgr, 200);
    time_mgr_tick(&sys->time_mgr, 100000LL);
    print_kv("  time_scale_pct", sys->time_mgr.time_scale_pct);
    print_kv("  virtual_time_us", (int)sys->time_mgr.virtual_time_us);
    print_str("[compression]"); print_nl();
    unsigned char raw[128];
    for (int i = 0; i < 128; i++) raw[i] = (i < 64) ? 0xAA : 0x55;
    unsigned char comp[256];
    unsigned int csz = compress_rle(raw, 128, comp, 256);
    print_kv("  raw_sz", 128); print_kv("  comp_sz", (int)csz);
    unsigned char decomp[256];
    unsigned int dsz = decompress_rle(comp, csz, decomp, 256);
    print_kv("  roundtrip_ok", mem_equal(raw, decomp, 128));
    CompressedState cstate;
    compressed_state_snap(&cstate, raw, 128, 1);
    print_kv("  checksum", (int)cstate.checksum);
    unsigned char old_s[32], new_s[32];
    for (int i = 0; i < 32; i++) { old_s[i] = 0; new_s[i] = (unsigned char)i; }
    IncrementalSync inc; incremental_init(&inc, 0);
    print_kv("  delta_bytes", (int)incremental_delta(&inc, old_s, 32, new_s, 32));
    print_str("[consistency]"); print_nl();
    print_kv("  model", sys->consistency.consistency_model);
    VersionedState va, vb;
    va.version = 1; va.timestamp = 100; va.peer_id = 2; va.data_size = 4;
    for (int i = 0; i < VCLOCK_SIZE; i++) va.causal_clock[i] = 0;
    va.causal_clock[2] = 5;
    vb.version = 2; vb.timestamp = 110; vb.peer_id = 3; vb.data_size = 4;
    for (int i = 0; i < VCLOCK_SIZE; i++) vb.causal_clock[i] = 0;
    vb.causal_clock[3] = 3;
    consistency_submit(&sys->consistency, &va);
    consistency_submit(&sys->consistency, &vb);
    print_kv("  pending", sys->consistency.pending_count);
    print_kv("  a_hb_b", happens_before(va.causal_clock, vb.causal_clock));
    print_kv("  concurrent", concurrent(va.causal_clock, vb.causal_clock));
    print_kv("  conflict_winner", (int)resolve_vclock(&va, &vb));
    print_str("[distributed]"); print_nl();
    sys->distributed.shard_count = 4;
    for (int i = 0; i < 4; i++) {
        shard_init(&sys->distributed.shards[i], (unsigned int)i, sys->local_peer_id);
        shard_add_replica(&sys->distributed.shards[i], (unsigned int)(i + 2));
    }
    print_kv("  shard_count", sys->distributed.shard_count);
    unsigned char key[] = "player_42_position";
    int route = distributed_route(&sys->distributed, key, 18);
    print_kv("  route_shard", route);
    unsigned char sd[64];
    for (int i = 0; i < 64; i++) sd[i] = (unsigned char)(i + 10);
    shard_update(&sys->distributed.shards[route], sd, 64);
    print_kv("  shard_version", (int)sys->distributed.shards[route].version);
    print_kv("  shard_healthy", shard_available(&sys->distributed.shards[route]));
    sys->distributed.shards[route].healthy = 0;
    print_kv("  recovered", distributed_recover(&sys->distributed, route));
    print_kv("  new_primary", (int)sys->distributed.shards[route].primary_peer);
    print_kv("  migrated", distributed_migrate(&sys->distributed, 0, 5));
    print_str("[replay]"); print_nl();
    unsigned int ad[4] = { 100, 200, 0, 0 };
    replay_record(&sys->replay, 1, 1, ACTION_MOVE, ad, 2);
    ad[0] = 150; ad[1] = 220;
    replay_record(&sys->replay, 2, 1, ACTION_MOVE, ad, 2);
    ad[0] = 1;
    replay_record(&sys->replay, 3, 2, ACTION_ATTACK, ad, 1);
    print_kv("  recorded", (int)sys->replay.action_count);
    snapshot_save(&sys->snapshots, 0, demo, 64);
    snapshot_save(&sys->snapshots, 5, demo, 64);
    print_kv("  snapshots", (int)sys->snapshots.snapshot_count);
    replay_play(&sys->replay);
    unsigned int of = 0; int steps = 0;
    while (replay_step(&sys->replay, &of)) { steps++; if (steps > 10) break; }
    print_kv("  playback_steps", steps);
    print_kv("  final_frame", (int)sys->replay.current_frame);
    replay_set_speed(&sys->replay, 200);
    print_kv("  speed_pct", sys->replay.playback_speed_pct);
    replay_pause(&sys->replay);
    time_travel_to(sys, 2);
    print_kv("  time_travel_frame", (int)sys->local_frame);
    print_str("[simulation]"); print_nl();
    for (int t = 0; t < 10; t++) sync_tick(sys, (long long)(t + 1) * 16000LL);
    print_kv("  local_frame", (int)sys->local_frame);
    print_kv("  virtual_time_us", (int)sys->time_mgr.virtual_time_us);
    print_kv("  snapshots", (int)sys->snapshots.snapshot_count);
    print_kv("  bandwidth_Bps", (int)estimate_bandwidth(sys->total_bytes_sent, 160));
    sync_shutdown(sys);
    print_kv("  running", sys->running);
    print_str("=== real_time_sync OK ==="); print_nl();
    host_exit(0);
    return 0;
}
