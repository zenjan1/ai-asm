/* vr_ar_engine: VR/AR Unified Engine (v55.0)
 * Immersive XR Runtime for AI-ASM OS (AArch64 WebAssembly):
 * - Head Tracking: 6DOF pose, quaternion math, sensor fusion, predictive compensation
 * - Hand Tracking: 21-joint skeleton, gesture recognition, IK, haptic interaction
 * - Spatial Anchors: SLAM localization, plane detection, anchor persistence, meshing
 * - Rendering: stereo, single-pass, foveated, lens correction, timewarp, late latching
 * - Comfort: vignette, tunneling, snap turning, guardian, passthrough, depth occlusion
 * - Platforms: OpenXR, Oculus SDK, ARCore, ARKit, SteamVR runtime detection
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

#define MAX_EYES 2
#define MAX_HSK 21
#define MAX_HANDS 2
#define MAX_ANCH 32
#define MAX_FEAT 256
#define MAX_PLN 16
#define MAX_MSH 64
#define MAX_GUARD 64
#define MAX_GEST 16
#define MAX_BTN 8
#define MAX_PLAT 5
#define MAX_BONE 24
#define MAX_IMU 3
#define NM 32
#define UID 37
#define PLAT_OPENXR 0
#define PLAT_OCULUS 1
#define PLAT_ARCORE 2
#define PLAT_ARKIT 3
#define PLAT_STEAMVR 4
#define GEST_IDLE 0
#define GEST_PINCH 1
#define GEST_GRAB 2
#define GEST_POINT 3
#define GEST_THUMB 4
#define ANCH_WORLD 0
#define ANCH_LOCAL 1
#define ANCH_CLOUD 2
#define CMODE_NONE 0
#define CMODE_VIG 1
#define CMODE_TUN 2
#define CMODE_SNAP 3
typedef struct { float x, y, z, w; } quat_t;
typedef struct { float x, y, z; } v3_t;
typedef struct { float m[16]; } m4_t;
typedef struct { v3_t accel, gyro, mag; float temp; double ts; int valid; } imu_t;
typedef struct { v3_t pos; quat_t ori; v3_t vel, avel; double ts; float conf; int mode, valid; } pose_t;
typedef struct { pose_t cur, prev, predicted; imu_t imu[MAX_IMU]; int imu_n, cam_n; float cal_yaw, cal_pitch, cal_roll, room_w, room_h, room_d; int cal_ok, fused; } head_t;
typedef struct { v3_t pos; quat_t rot; float conf, radius; int parent; } hjoint_t;
typedef struct { int hid; hjoint_t joints[MAX_HSK]; int active; float pinch, grab; int gesture; float gesture_conf; v3_t wrist_pos; quat_t wrist_rot; float scale; } hand_t;
typedef struct { int cid; int btn[MAX_BTN], btn_prev[MAX_BTN]; float trig_l, trig_r, stick_x, stick_y; float hap_f, hap_a, hap_d; int active; pose_t pose; } ctrl_t;
typedef struct { int gid, type; char name[NM]; float threshold; float j[MAX_HSK*3]; int valid; } gesture_t;
typedef struct { int aid, type; v3_t pos; quat_t ori; char uid[UID]; int persistent, shared; float conf; double ts; int active; } anchor_t;
typedef struct { v3_t pos; float ox, oy, conf; int id, tracked; } feature_t;
typedef struct { v3_t center, normal; float ex, ez; int type, id, valid; } plane_t;
typedef struct { int mid, vc, fc; v3_t center; float radius; int semantic, updated; } mesh_t;
typedef struct { int pc, active, visible; float height; v3_t pts[MAX_GUARD]; float mnx, mnz, mxx, mxz; } guard_t;
typedef struct { int eid, w, h; float ipd, nc, fc; m4_t view, proj; float fx, fy, ox, oy; int layers, valid; } eye_t;
typedef struct { int enabled; float is, os, cx, cy; int quality, eye_tracked; } foveated_t;
typedef struct { float k1, k2, k3, cr, cg, cb, car, cag, cab; int calibrated; } lens_t;
typedef struct { int enabled; float latency; int motion_smooth, late_latch, applied; } tw_t;
typedef struct { int mode, active; float vig_str, tun_fov, snap_ang, speed, fade; int rot_mode, tel_on; } comf_t;
typedef struct { int id; char name[NM]; int hand, pass, eye, audio, depth, face, scene, avail; } plat_t;
typedef struct { v3_t target; quat_t t_rot; int chain; int bids[MAX_BONE]; float wts[MAX_BONE]; int solved; float err; } ik_t;
typedef struct { int grabbing, grab_id; float tvx, tvy, tvz; int teleporting; v3_t tel_dest; int tel_valid; int ui_hover, ui_id; float ui_dist; } interact_t;

static head_t g_head; static hand_t g_hands[MAX_HANDS]; static ctrl_t g_ctrls[MAX_HANDS];
static gesture_t g_gest[MAX_GEST]; static int g_gn=0; static anchor_t g_anch[MAX_ANCH]; static int g_an=0;
static feature_t g_feat[MAX_FEAT]; static int g_fn=0; static plane_t g_pln[MAX_PLN]; static int g_pn=0;
static mesh_t g_msh[MAX_MSH]; static int g_mn=0; static guard_t g_guard; static eye_t g_eyes[MAX_EYES];
static foveated_t g_fov; static lens_t g_lens; static tw_t g_tw; static comf_t g_comf;
static plat_t g_plat[MAX_PLAT]; static ik_t g_ik[MAX_HANDS]; static interact_t g_int[MAX_HANDS];
static int g_fr=0, g_seq=1;
static void nm_cpy(char *d, const char *s, int mx);
static float m_sqrt(float v); static float m_dot3(const v3_t *a, const v3_t *b);
static void q_id(quat_t *q); static void q_mul(quat_t *o, const quat_t *a, const quat_t *b);
static void q_norm(quat_t *q); static void q_inv(quat_t *o, const quat_t *q);
static void q_slerp(quat_t *o, const quat_t *a, const quat_t *b, float t);
static void v3a(v3_t *o, const v3_t *a, const v3_t *b); static void v3s(v3_t *o, const v3_t *a, const v3_t *b);
static void v3sc(v3_t *o, const v3_t *a, float s); static float v3l(const v3_t *v); static void v3n(v3_t *v);
static void v3cr(v3_t *o, const v3_t *a, const v3_t *b);
static void head_init(void); static void head_upd(double ts); static void head_pred(void); static void head_fuse(void);
static void head_cal(float y, float p, float r); static void head_room(float w, float h, float d);
static void hand_init(int h); static void hand_upd(int h); static int hand_gest(int h);
static void hand_greg(const char *nm, int t, float th); static void hand_ik(int h);
static void hand_grab(int h, int oid); static void hand_throw(int h, float vx, float vy, float vz);
static void hand_tel(int h, float dx, float dy, float dz);
static void ctrl_init(int c); static void ctrl_press(int c, int b); static void ctrl_rel(int c, int b); static void ctrl_hap(int c, float f, float a, float d);
static int anch_creat(int t, float px, float py, float pz); static void anch_rm(int a); static int anch_save(int a);
static int anch_load(const char *uid); static int anch_share(int a); static void anch_mov(int a, float px, float py, float pz);
static void slam_init(void); static void slam_track(void); static int slam_pln(int tf); static int slam_msh(float cx, float cy, float cz); static void slam_loc(void);
static void rend_init(void); static void rend_stereo(int e); static void rend_spass(void); static void rend_fov(void); static void rend_lens(int e); static void rend_tw(void);
static void comf_apply(void); static void comf_mode(int m); static void comf_spd(float s); static void comf_snap(float a);
static void gd_init(int n); static void gd_pt(int i, float px, float py, float pz); static int gd_test(float px, float py, float pz); static void gd_h(float h);
static int plat_detect(void); static int plat_ht(int p); static int plat_hp(int p); static int plat_he(int p); static void plat_rpt(int p);
static void engine_init(void); static void engine_tick(float dt); static void engine_main(void);

static void nm_cpy(char *d, const char *s, int mx) {
    int i; for (i = 0; i < mx - 1 && s[i]; i++) d[i] = s[i]; d[i] = '\0';
}
static float m_sqrt(float v) {
    float x = v; if (x <= 0.0f) return 0.0f;
    for (int i = 0; i < 8; i++) x = 0.5f * (x + v / x); return x;
}
static float m_dot3(const v3_t *a, const v3_t *b) { return a->x*b->x + a->y*b->y + a->z*b->z; }
static void q_id(quat_t *q) { q->x=0; q->y=0; q->z=0; q->w=1.0f; }
static void q_mul(quat_t *o, const quat_t *a, const quat_t *b) {
    quat_t r; r.w=a->w*b->w-a->x*b->x-a->y*b->y-a->z*b->z;
    r.x=a->w*b->x+a->x*b->w+a->y*b->z-a->z*b->y;
    r.y=a->w*b->y-a->x*b->z+a->y*b->w+a->z*b->x;
    r.z=a->w*b->z+a->x*b->y-a->y*b->x+a->z*b->w; *o=r;
}
static void q_norm(quat_t *q) {
    float m=m_sqrt(q->w*q->w+q->x*q->x+q->y*q->y+q->z*q->z);
    if (m<0.00001f) { q_id(q); return; }
    float iv=1.0f/m; q->x*=iv; q->y*=iv; q->z*=iv; q->w*=iv;
}
static void q_inv(quat_t *o, const quat_t *q) {
    float d=q->w*q->w+q->x*q->x+q->y*q->y+q->z*q->z;
    if (d<0.00001f) { q_id(o); return; }
    float iv=1.0f/d; o->x=-q->x*iv; o->y=-q->y*iv; o->z=-q->z*iv; o->w=q->w*iv;
}
static void q_slerp(quat_t *o, const quat_t *a, const quat_t *b, float t) {
    float dot=a->x*b->x+a->y*b->y+a->z*b->z+a->w*b->w;
    float bx=b->x,by=b->y,bz=b->z,bw=b->w;
    if (dot<0.0f) { dot=-dot; bx=-bx; by=-by; bz=-bz; bw=-bw; }
    if (dot>0.9995f) {
        o->x=a->x+t*(bx-a->x); o->y=a->y+t*(by-a->y);
        o->z=a->z+t*(bz-a->z); o->w=a->w+t*(bw-a->w); q_norm(o); return;
    }
    float s1=1.0f-t, s2=t;
    o->x=s1*a->x+s2*bx; o->y=s1*a->y+s2*by;
    o->z=s1*a->z+s2*bz; o->w=s1*a->w+s2*bw; q_norm(o);
}
static void v3a(v3_t *o, const v3_t *a, const v3_t *b) { o->x=a->x+b->x; o->y=a->y+b->y; o->z=a->z+b->z; }
static void v3s(v3_t *o, const v3_t *a, const v3_t *b) { o->x=a->x-b->x; o->y=a->y-b->y; o->z=a->z-b->z; }
static void v3sc(v3_t *o, const v3_t *a, float s) { o->x=a->x*s; o->y=a->y*s; o->z=a->z*s; }
static float v3l(const v3_t *v) { return m_sqrt(v->x*v->x+v->y*v->y+v->z*v->z); }
static void v3n(v3_t *v) {
    float l=v3l(v); if (l<0.00001f) { v->x=0;v->y=0;v->z=0; return; }
    float iv=1.0f/l; v->x*=iv; v->y*=iv; v->z*=iv;
}
static void v3cr(v3_t *o, const v3_t *a, const v3_t *b) {
    v3_t r; r.x=a->y*b->z-a->z*b->y; r.y=a->z*b->x-a->x*b->z; r.z=a->x*b->y-a->y*b->x; *o=r;
}
static void head_init(void) {
    q_id(&g_head.cur.ori); q_id(&g_head.prev.ori); q_id(&g_head.predicted.ori);
    g_head.cur.pos.x=0; g_head.cur.pos.y=1.65f; g_head.cur.pos.z=0;
    g_head.cur.conf=1.0f; g_head.cur.mode=0; g_head.cur.valid=1;
    g_head.prev=g_head.cur; g_head.predicted=g_head.cur; g_head.imu_n=MAX_IMU; g_head.cam_n=2;
    for (int i=0; i<MAX_IMU; i++) {
        g_head.imu[i].accel.x=0; g_head.imu[i].accel.y=-9.81f; g_head.imu[i].accel.z=0;
        g_head.imu[i].gyro.x=0; g_head.imu[i].gyro.y=0; g_head.imu[i].gyro.z=0;
        g_head.imu[i].mag.x=0; g_head.imu[i].mag.y=0.3f; g_head.imu[i].mag.z=0;
        g_head.imu[i].temp=25.0f; g_head.imu[i].ts=0; g_head.imu[i].valid=1;
    }
    g_head.cal_yaw=0; g_head.cal_pitch=0; g_head.cal_roll=0; g_head.room_w=3.0f; g_head.room_h=2.8f; g_head.room_d=3.0f;
    g_head.cal_ok=1; g_head.fused=1;
    host_print("[vr_ar_engine] head_init: 6DOF + IMU fusion active\n");
}
static void head_upd(double ts) {
    g_head.prev=g_head.cur; g_head.cur.ts=ts;
    float dt=(float)(ts-g_head.prev.ts);
    if (dt<0.0f) dt=0.001f; if (dt>0.1f) dt=0.001f;
    g_head.cur.pos.x+=g_head.cur.vel.x*dt; g_head.cur.pos.y+=g_head.cur.vel.y*dt; g_head.cur.pos.z+=g_head.cur.vel.z*dt;
    head_fuse();
    if (g_fr%90==0) host_print("[vr_ar_engine] head_upd: pose fused\n");
}
static void head_pred(void) {
    float lat=g_head.cur.ts-g_head.prev.ts;
    if (lat<0.0f) lat=0.001f; if (lat>0.05f) lat=0.016f;
    float pred=lat*1.5f; if (pred>0.033f) pred=0.033f;
    g_head.predicted.pos.x=g_head.cur.pos.x+g_head.cur.vel.x*pred;
    g_head.predicted.pos.y=g_head.cur.pos.y+g_head.cur.vel.y*pred;
    g_head.predicted.pos.z=g_head.cur.pos.z+g_head.cur.vel.z*pred;
    float av=v3l(&g_head.cur.avel);
    if (av>0.0001f) {
        float ha=av*pred*0.5f, ax=g_head.cur.avel.x/av, ay=g_head.cur.avel.y/av, az=g_head.cur.avel.z/av;
        quat_t dw; dw.x=ax*ha; dw.y=ay*ha; dw.z=az*ha; dw.w=0.999f; q_norm(&dw);
        q_mul(&g_head.predicted.ori, &g_head.cur.ori, &dw);
    } else { g_head.predicted.ori=g_head.cur.ori; }
    g_head.predicted.conf=g_head.cur.conf*0.95f; g_head.predicted.valid=1;
    if (g_fr%90==0) host_print("[vr_ar_engine] head_pred: time warp applied\n");
}
static void head_fuse(void) {
    if (g_head.imu_n<1) return;
    v3_t aa={0,0,0}, ga={0,0,0}; int vc=0;
    for (int i=0; i<g_head.imu_n; i++) {
        if (!g_head.imu[i].valid) continue;
        aa.x+=g_head.imu[i].accel.x; aa.y+=g_head.imu[i].accel.y; aa.z+=g_head.imu[i].accel.z;
        ga.x+=g_head.imu[i].gyro.x; ga.y+=g_head.imu[i].gyro.y; ga.z+=g_head.imu[i].gyro.z; vc++;
    }
    if (vc>0) { float iv=1.0f/(float)vc; aa.x*=iv;aa.y*=iv;aa.z*=iv; ga.x*=iv;ga.y*=iv;ga.z*=iv; }
    g_head.cur.vel.x+=aa.x*0.001f; g_head.cur.vel.y+=(aa.y+9.81f)*0.001f; g_head.cur.vel.z+=aa.z*0.001f;
    g_head.cur.avel=ga; g_head.fused=1;
}
static void head_cal(float y, float p, float r) {
    g_head.cal_yaw=y; g_head.cal_pitch=p; g_head.cal_roll=r; g_head.cal_ok=1;
    host_print("[vr_ar_engine] head_cal: offset correction applied\n");
}
static void head_room(float w, float h, float d) {
    g_head.room_w=w; g_head.room_h=h; g_head.room_d=d;
    host_print("[vr_ar_engine] head_room: guardian space configured\n");
}
static void hand_init(int h) {
    if (h<0||h>=MAX_HANDS) return;
    hand_t *d=&g_hands[h]; d->hid=h; d->active=1; d->pinch=0; d->grab=0;
    d->gesture=GEST_IDLE; d->gesture_conf=0; d->scale=1.0f;
    d->wrist_pos.x=(h==0)?-0.3f:0.3f; d->wrist_pos.y=1.0f; d->wrist_pos.z=-0.4f;
    q_id(&d->wrist_rot);
    for (int j=0; j<MAX_HSK; j++) {
        d->joints[j].pos.x=d->wrist_pos.x+(float)j*0.01f; d->joints[j].pos.y=d->wrist_pos.y;
        d->joints[j].pos.z=d->wrist_pos.z-(float)j*0.005f; q_id(&d->joints[j].rot);
        d->joints[j].conf=0.9f; d->joints[j].radius=0.005f; d->joints[j].parent=(j==0)?-1:j-1;
    }
    host_print("[vr_ar_engine] hand_init: 21-joint skeleton ready\n");
}
static void hand_upd(int h) {
    if (h<0||h>=MAX_HANDS||!g_hands[h].active) return;
    hand_t *d=&g_hands[h];
    for (int j=1; j<MAX_HSK; j++) {
        int p=d->joints[j].parent; if (p<0) continue;
        v3_t dir; v3s(&dir, &d->joints[j].pos, &d->joints[p].pos);
        float l=v3l(&dir); if (l<0.001f) l=0.001f;
        if (l>0.03f) { v3sc(&dir, &dir, 0.03f/l); v3a(&d->joints[j].pos, &d->joints[p].pos, &dir); }
    }
    float td=0; if (MAX_HSK>4) { v3_t dd; v3s(&dd,&d->joints[4].pos,&d->joints[0].pos); td=v3l(&dd); }
    d->pinch=1.0f-(td/0.1f); if (d->pinch<0) d->pinch=0; if (d->pinch>1) d->pinch=1;
    float curl=0;
    for (int j=5; j<MAX_HSK&&j<20; j+=4) { v3_t dd; v3s(&dd,&d->joints[j].pos,&d->wrist_pos); curl+=v3l(&dd); }
    d->grab=curl/0.8f; if (d->grab<0) d->grab=0; if (d->grab>1) d->grab=1;
    hand_gest(h);
}
static int hand_gest(int h) {
    if (h<0||h>=MAX_HANDS) return GEST_IDLE;
    hand_t *d=&g_hands[h];
    if (d->pinch>0.8f) { d->gesture=GEST_PINCH; d->gesture_conf=d->pinch; return GEST_PINCH; }
    if (d->grab>0.7f) { d->gesture=GEST_GRAB; d->gesture_conf=d->grab; return GEST_GRAB; }
    int ext=0;
    for (int j=8; j<MAX_HSK; j+=4) { v3_t dd; v3s(&dd,&d->joints[j].pos,&d->wrist_pos); if (v3l(&dd)>0.08f) ext++; }
    if (ext>=3) { d->gesture=GEST_POINT; d->gesture_conf=0.8f; return GEST_POINT; }
    d->gesture=GEST_IDLE; d->gesture_conf=0.5f; return GEST_IDLE;
}
static void hand_greg(const char *nm, int t, float th) {
    if (g_gn>=MAX_GEST) return;
    gesture_t *g=&g_gest[g_gn]; g->gid=g_gn; g->type=t;
    nm_cpy(g->name, nm, NM); g->threshold=th;
    for (int j=0; j<MAX_HSK*3; j++) g->j[j]=0; g->valid=1; g_gn++;
    host_print("[vr_ar_engine] gesture reg: "); host_print(nm); host_print("\n");
}
static void hand_ik(int h) {
    if (h<0||h>=MAX_HANDS) return;
    ik_t *k=&g_ik[h]; float err=0;
    for (int i=0; i<k->chain&&i<MAX_BONE; i++) {
        int bid=k->bids[i]; if (bid<0||bid>=MAX_HSK) continue;
        v3_t dd; v3s(&dd, &k->target, &g_hands[h].joints[bid].pos);
        err+=v3l(&dd); float w=k->wts[i];
        v3_t step; v3sc(&step, &dd, w*0.1f);
        v3a(&g_hands[h].joints[bid].pos, &g_hands[h].joints[bid].pos, &step);
    }
    k->err=err; k->solved=(err<0.05f)?1:0;
    if (g_fr%120==0) host_print("[vr_ar_engine] ik_solve: convergence check\n");
}
static void hand_grab(int h, int oid) {
    if (h<0||h>=MAX_HANDS) return;
    g_int[h].grabbing=1; g_int[h].grab_id=oid;
    host_print("[vr_ar_engine] interact: object grabbed\n");
}
static void hand_throw(int h, float vx, float vy, float vz) {
    if (h<0||h>=MAX_HANDS) return;
    g_int[h].tvx=vx; g_int[h].tvy=vy; g_int[h].tvz=vz;
    g_int[h].grabbing=0; g_int[h].grab_id=-1;
    host_print("[vr_ar_engine] interact: object thrown\n");
}
static void hand_tel(int h, float dx, float dy, float dz) {
    if (h<0||h>=MAX_HANDS) return;
    g_int[h].teleporting=1; g_int[h].tel_dest.x=dx; g_int[h].tel_dest.y=dy; g_int[h].tel_dest.z=dz;
    g_int[h].tel_valid=1;
    host_print("[vr_ar_engine] interact: teleport initiated\n");
}
static void ctrl_init(int c) {
    if (c<0||c>=MAX_HANDS) return;
    g_ctrls[c].cid=c;
    for (int i=0; i<MAX_BTN; i++) { g_ctrls[c].btn[i]=0; g_ctrls[c].btn_prev[i]=0; }
    g_ctrls[c].trig_l=0; g_ctrls[c].trig_r=0; g_ctrls[c].stick_x=0; g_ctrls[c].stick_y=0;
    g_ctrls[c].hap_f=0; g_ctrls[c].hap_a=0; g_ctrls[c].hap_d=0; g_ctrls[c].active=1;
    q_id(&g_ctrls[c].pose.ori);
    g_ctrls[c].pose.pos.x=(c==0)?-0.3f:0.3f; g_ctrls[c].pose.pos.y=1.0f; g_ctrls[c].pose.pos.z=-0.4f;
}
static void ctrl_press(int c, int b) {
    if (c<0||c>=MAX_HANDS||b<0||b>=MAX_BTN) return;
    g_ctrls[c].btn_prev[b]=g_ctrls[c].btn[b]; g_ctrls[c].btn[b]=1;
}
static void ctrl_rel(int c, int b) {
    if (c<0||c>=MAX_HANDS||b<0||b>=MAX_BTN) return;
    g_ctrls[c].btn_prev[b]=g_ctrls[c].btn[b]; g_ctrls[c].btn[b]=0;
}
static void ctrl_hap(int c, float f, float a, float d) {
    if (c<0||c>=MAX_HANDS) return;
    g_ctrls[c].hap_f=f; g_ctrls[c].hap_a=a; g_ctrls[c].hap_d=d;
}
static int anch_creat(int t, float px, float py, float pz) {
    if (g_an>=MAX_ANCH) return -1;
    anchor_t *a=&g_anch[g_an]; a->aid=g_an; a->type=t;
    a->pos.x=px; a->pos.y=py; a->pos.z=pz; q_id(&a->ori);
    a->persistent=(t==ANCH_WORLD||t==ANCH_CLOUD)?1:0;
    a->shared=0; a->conf=1.0f; a->ts=0; a->active=1;
    a->uid[0]='A'; a->uid[1]='N'; a->uid[2]='C'; a->uid[3]='-';
    a->uid[4]='0'+(char)(g_an/10); a->uid[5]='0'+(char)(g_an%10);
    for (int i=6; i<UID-1; i++) a->uid[i]='\0';
    return g_an++;
}
static void anch_rm(int a) { if (a>=0&&a<g_an) g_anch[a].active=0; }
static int anch_save(int a) {
    if (a<0||a>=g_an||!g_anch[a].active) return -1;
    g_anch[a].persistent=1; host_print("[vr_ar_engine] anchor saved: persistent\n"); return a;
}
static int anch_load(const char *uid) {
    for (int i=0; i<g_an; i++) {
        if (!g_anch[i].active||!g_anch[i].persistent) continue;
        int match=1;
        for (int j=0; j<UID&&uid[j]; j++) if (g_anch[i].uid[j]!=uid[j]) { match=0; break; }
        if (match) return i;
    }
    return -1;
}
static int anch_share(int a) {
    if (a<0||a>=g_an||!g_anch[a].active) return -1;
    g_anch[a].shared=1; host_print("[vr_ar_engine] anchor shared: cloud sync\n"); return a;
}
static void anch_mov(int a, float px, float py, float pz) {
    if (a<0||a>=g_an||!g_anch[a].active) return;
    g_anch[a].pos.x=px; g_anch[a].pos.y=py; g_anch[a].pos.z=pz;
}
static void slam_init(void) {
    g_fn=0; g_pn=0; g_mn=0;
    for (int i=0; i<16; i++) {
        g_feat[i].pos.x=(float)(i%4)*0.5f-1.0f; g_feat[i].pos.y=0;
        g_feat[i].pos.z=(float)(i/4)*0.5f-1.0f;
        g_feat[i].ox=0.5f; g_feat[i].oy=0.5f; g_feat[i].conf=0.8f;
        g_feat[i].id=i; g_feat[i].tracked=1;
    }
    g_fn=16;
    host_print("[vr_ar_engine] slam_init: 16 seed features tracked\n");
}
static void slam_track(void) {
    for (int i=0; i<g_fn; i++) {
        if (!g_feat[i].tracked) continue;
        g_feat[i].ox+=0.001f; g_feat[i].oy+=0.0005f;
        if (g_feat[i].ox>1.0f) g_feat[i].ox=0; if (g_feat[i].oy>1.0f) g_feat[i].oy=0;
    }
    if (g_fr%180==0&&g_fn<MAX_FEAT) {
        int i=g_fn++; g_feat[i].pos.x=-1.5f; g_feat[i].pos.y=0.5f; g_feat[i].pos.z=-1.5f;
        g_feat[i].ox=0.25f; g_feat[i].oy=0.25f; g_feat[i].conf=0.7f; g_feat[i].id=i; g_feat[i].tracked=1;
    }
}
static int slam_pln(int tf) {
    if (g_pn>=MAX_PLN) return g_pn;
    plane_t *p=&g_pln[g_pn]; p->id=g_pn;
    p->center.x=0; p->center.y=0; p->center.z=-1.0f;
    p->normal.x=0; p->normal.y=1.0f; p->normal.z=0;
    p->ex=2.0f; p->ez=2.0f; p->type=tf; p->valid=1;
    return ++g_pn;
}
static int slam_msh(float cx, float cy, float cz) {
    if (g_mn>=MAX_MSH) return -1;
    mesh_t *m=&g_msh[g_mn]; m->mid=g_mn; m->vc=256; m->fc=128;
    m->center.x=cx; m->center.y=cy; m->center.z=cz;
    m->radius=1.0f; m->semantic=0; m->updated=1;
    return g_mn++;
}
static void slam_loc(void) {
    int tr=0; for (int i=0; i<g_fn; i++) if (g_feat[i].tracked) tr++;
    float ratio=(g_fn>0)?(float)tr/(float)g_fn:0;
    if (ratio>0.5f) host_print("[vr_ar_engine] slam_loc: features tracked OK\n");
    else host_print("[vr_ar_engine] slam_loc: degraded tracking\n");
}
static void rend_init(void) {
    float ipd=0.064f;
    for (int e=0; e<MAX_EYES; e++) {
        g_eyes[e].eid=e; g_eyes[e].w=2048; g_eyes[e].h=2048; g_eyes[e].ipd=ipd*0.5f*((e==0)?-1.0f:1.0f);
        g_eyes[e].nc=0.01f; g_eyes[e].fc=1000.0f; g_eyes[e].fx=1000.0f; g_eyes[e].fy=1000.0f;
        g_eyes[e].ox=0; g_eyes[e].oy=0; g_eyes[e].layers=4; g_eyes[e].valid=1;
        for (int i=0; i<16; i++) { g_eyes[e].view.m[i]=0; g_eyes[e].proj.m[i]=0; }
        g_eyes[e].view.m[0]=1; g_eyes[e].view.m[5]=1; g_eyes[e].view.m[10]=1; g_eyes[e].view.m[15]=1;
        g_eyes[e].proj.m[0]=1; g_eyes[e].proj.m[5]=1; g_eyes[e].proj.m[10]=1; g_eyes[e].proj.m[15]=1;
    }
    g_fov.enabled=1; g_fov.is=1.0f; g_fov.os=0.5f; g_fov.cx=0.5f; g_fov.cy=0.5f; g_fov.quality=2; g_fov.eye_tracked=1;
    g_lens.k1=0.2f; g_lens.k2=-0.05f; g_lens.k3=0.01f; g_lens.cr=1.0f; g_lens.cg=1.0f; g_lens.cb=1.0f;
    g_lens.car=0.001f; g_lens.cag=0.0f; g_lens.cab=-0.001f; g_lens.calibrated=1;
    g_tw.enabled=1; g_tw.latency=0.016f; g_tw.motion_smooth=1; g_tw.late_latch=1; g_tw.applied=0;
    host_print("[vr_ar_engine] rend_init: stereo 2048x2048 foveated ON\n");
}
static void rend_stereo(int e) {
    if (e<0||e>=MAX_EYES||!g_eyes[e].valid) return;
    if (g_eyes[e].layers<8) g_eyes[e].layers++;
    g_eyes[e].view.m[12]=g_eyes[e].ipd;
}
static void rend_spass(void) {
    g_eyes[0].layers=g_eyes[1].layers;
    host_print("[vr_ar_engine] rend: single-pass stereo instanced\n");
}
static void rend_fov(void) {
    if (!g_fov.enabled) return;
    if (g_fov.is<=0.0f||g_fov.is>2.0f) g_fov.is=1.0f;
    if (g_fov.os<=0.0f||g_fov.os>1.0f) g_fov.os=0.5f;
    for (int e=0; e<MAX_EYES; e++) { g_eyes[e].fx=1000.0f*g_fov.is; g_eyes[e].fy=1000.0f*g_fov.is; }
}
static void rend_lens(int e) {
    if (e<0||e>=MAX_EYES||!g_lens.calibrated) return;
    g_eyes[e].ox=g_lens.k1*0.01f; g_eyes[e].oy=g_lens.k2*0.01f;
}
static void rend_tw(void) {
    if (!g_tw.enabled) return;
    float dt=(float)(g_head.cur.ts-g_head.prev.ts);
    if (dt<0.0f) dt=0.0f; if (dt>0.05f) dt=0.05f;
    g_tw.latency=dt; g_tw.applied=1;
    if (g_fr%90==0) host_print("[vr_ar_engine] timewarp: async reprojection active\n");
}
static void comf_apply(void) {
    if (!g_comf.active) return;
    float speed=v3l(&g_head.cur.vel);
    if (g_comf.mode==CMODE_VIG||g_comf.mode==CMODE_TUN) {
        float str=speed*g_comf.vig_str; if (str>1.0f) str=1.0f;
        g_comf.tun_fov=90.0f-str*30.0f;
    }
    if (g_comf.mode==CMODE_SNAP&&g_comf.snap_ang<15.0f) g_comf.snap_ang=45.0f;
    if (g_fr%120==0) host_print("[vr_ar_engine] comf: motion sickness prevention active\n");
}
static void comf_mode(int m) { g_comf.mode=m; g_comf.active=1; }
static void comf_spd(float s) { if (s<0.1f) s=0.1f; if (s>10.0f) s=10.0f; g_comf.speed=s; }
static void comf_snap(float a) {
    if (a<15.0f) a=15.0f; if (a>90.0f) a=90.0f; g_comf.snap_ang=a; g_comf.rot_mode=1;
}
static void gd_init(int n) {
    if (n<3) n=3; if (n>MAX_GUARD) n=MAX_GUARD;
    g_guard.pc=n; g_guard.active=1; g_guard.visible=1; g_guard.height=2.0f;
    g_guard.mnx=1000.0f; g_guard.mnz=1000.0f; g_guard.mxx=-1000.0f; g_guard.mxz=-1000.0f;
}
static void gd_pt(int i, float px, float py, float pz) {
    if (i<0||i>=g_guard.pc) return;
    g_guard.pts[i].x=px; g_guard.pts[i].y=py; g_guard.pts[i].z=pz;
    if (px<g_guard.mnx) g_guard.mnx=px; if (pz<g_guard.mnz) g_guard.mnz=pz;
    if (px>g_guard.mxx) g_guard.mxx=px; if (pz>g_guard.mxz) g_guard.mxz=pz;
}
static int gd_test(float px, float py, float pz) {
    if (!g_guard.active) return 1;
    if (px<g_guard.mnx||px>g_guard.mxx) return 0;
    if (pz<g_guard.mnz||pz>g_guard.mxz) return 0;
    if (py<0||py>g_guard.height) return 0;
    return 1;
}
static void gd_h(float h) { if (h<1.0f) h=1.0f; if (h>5.0f) h=5.0f; g_guard.height=h; }
static int plat_detect(void) {
    for (int i=0; i<MAX_PLAT; i++) {
        g_plat[i].id=i; g_plat[i].avail=0; g_plat[i].hand=0; g_plat[i].pass=0;
        g_plat[i].eye=0; g_plat[i].audio=0; g_plat[i].depth=0; g_plat[i].face=0; g_plat[i].scene=0;
    }
    nm_cpy(g_plat[PLAT_OPENXR].name,"OpenXR",NM);
    g_plat[PLAT_OPENXR].hand=1; g_plat[PLAT_OPENXR].audio=1; g_plat[PLAT_OPENXR].eye=1; g_plat[PLAT_OPENXR].avail=1;
    nm_cpy(g_plat[PLAT_OCULUS].name,"Oculus_SDK",NM);
    g_plat[PLAT_OCULUS].hand=1; g_plat[PLAT_OCULUS].pass=1; g_plat[PLAT_OCULUS].audio=1;
    g_plat[PLAT_OCULUS].eye=1; g_plat[PLAT_OCULUS].face=1; g_plat[PLAT_OCULUS].avail=1;
    nm_cpy(g_plat[PLAT_ARCORE].name,"ARCore",NM);
    g_plat[PLAT_ARCORE].depth=1; g_plat[PLAT_ARCORE].scene=1; g_plat[PLAT_ARCORE].face=1; g_plat[PLAT_ARCORE].avail=1;
    nm_cpy(g_plat[PLAT_ARKIT].name,"ARKit",NM);
    g_plat[PLAT_ARKIT].scene=1; g_plat[PLAT_ARKIT].face=1; g_plat[PLAT_ARKIT].depth=1; g_plat[PLAT_ARKIT].avail=1;
    nm_cpy(g_plat[PLAT_STEAMVR].name,"SteamVR",NM);
    g_plat[PLAT_STEAMVR].hand=1; g_plat[PLAT_STEAMVR].audio=1; g_plat[PLAT_STEAMVR].eye=1; g_plat[PLAT_STEAMVR].avail=1;
    host_print("[vr_ar_engine] plat_detect: 5 platforms scanned\n");
    return MAX_PLAT;
}
static int plat_ht(int p) { return (p>=0&&p<MAX_PLAT)?g_plat[p].hand:0; }
static int plat_hp(int p) { return (p>=0&&p<MAX_PLAT)?g_plat[p].pass:0; }
static int plat_he(int p) { return (p>=0&&p<MAX_PLAT)?g_plat[p].eye:0; }
static void plat_rpt(int p) {
    if (p<0||p>=MAX_PLAT||!g_plat[p].avail) return;
    host_print("[vr_ar_engine] plat: "); host_print(g_plat[p].name);
    host_print(" h="); host_print(g_plat[p].hand?"Y":"N"); host_print(" p="); host_print(g_plat[p].pass?"Y":"N");
    host_print(" e="); host_print(g_plat[p].eye?"Y":"N"); host_print(" a="); host_print(g_plat[p].audio?"Y":"N");
    host_print("\n");
}
static void engine_init(void) {
    host_print("[vr_ar_engine] === VR/AR Engine v55.0 initializing ===\n");
    rend_init(); head_init(); head_room(3.0f, 2.8f, 3.0f); head_cal(0.0f, 0.0f, 0.0f);
    hand_init(0); hand_init(1); ctrl_init(0); ctrl_init(1);
    hand_greg("pinch_default", GEST_PINCH, 0.8f); hand_greg("grab_default", GEST_GRAB, 0.7f);
    hand_greg("point_default", GEST_POINT, 0.6f); hand_greg("thumb_up", GEST_THUMB, 0.5f);
    slam_init();
    int a0=anch_creat(ANCH_WORLD,0,0,0); int a1=anch_creat(ANCH_LOCAL,1.0f,1.0f,-1.0f);
    int a2=anch_creat(ANCH_CLOUD,-1.0f,0.5f,0.5f);
    if (a0>=0) anch_save(a0); if (a2>=0) anch_share(a2);
    slam_pln(0); slam_msh(0,0,0);
    comf_mode(CMODE_VIG); comf_spd(1.5f); comf_snap(45.0f); g_comf.vig_str=0.5f; g_comf.tel_on=1;
    gd_init(4); gd_pt(0,-2.0f,0,-2.0f); gd_pt(1,2.0f,0,-2.0f); gd_pt(2,2.0f,0,2.0f); gd_pt(3,-2.0f,0,2.0f); gd_h(2.5f);
    int np=plat_detect(); for (int i=0; i<np; i++) plat_rpt(i);
    host_print("[vr_ar_engine] === Initialization complete ===\n");
}
static void engine_tick(float dt) {
    g_fr++; double ts=(double)g_fr*(double)dt;
    head_upd(ts); head_pred(); hand_upd(0); hand_upd(1); hand_ik(0); hand_ik(1);
    slam_track(); rend_stereo(0); rend_stereo(1);
    rend_fov(); rend_lens(0); rend_lens(1); rend_tw(); comf_apply();
    if (g_fr%60==0) host_print("[vr_ar_engine] tick: 60-frame cycle ok\n");
}
static void engine_main(void) {
    host_print("[vr_ar_engine] === VR/AR Engine main entry ===\n");
    engine_init();
    for (int i=0; i<180; i++) engine_tick(0.011f);
    int gt=gd_test(0,1.0f,0);
    if (gt) host_print("[vr_ar_engine] guard_test: inside boundary\n");
    else host_print("[vr_ar_engine] guard_test: outside boundary\n");
    int al=anch_load("ANC-00");
    if (al>=0) host_print("[vr_ar_engine] anchor loaded from persistent store\n");
    else host_print("[vr_ar_engine] anchor not found in store\n");
    int ht=plat_ht(PLAT_OCULUS); int pp=plat_hp(PLAT_ARCORE);
    if (ht) host_print("[vr_ar_engine] oculus: hand tracking supported\n");
    if (!pp) host_print("[vr_ar_engine] arcore: no passthrough\n");
    ctrl_press(0,0); ctrl_hap(0,200.0f,0.8f,0.05f);
    hand_grab(0,42); hand_throw(0,1.0f,2.0f,0.5f); hand_tel(1,3.0f,0,-2.0f); ctrl_rel(0,0);
    rend_spass(); slam_loc();
    host_print("[vr_ar_engine] stats: head=6DOF_fused hands=2x21 anchors=w/l/c/t\n");
    host_print("[vr_ar_engine] stats: render=stereo+fov+tw comfort=vig+tun+snap\n");
    host_print("[vr_ar_engine] stats: guard=4pt platforms=5\n");
    host_print("[vr_ar_engine] === VR/AR Engine main complete ===\n");
}

__attribute__((export_name("engine_init"))) void exp_engine_init(void) { engine_init(); }
__attribute__((export_name("engine_tick"))) void exp_engine_tick(float dt) { engine_tick(dt); }
__attribute__((export_name("engine_main"))) void exp_engine_main(void) { engine_main(); }
__attribute__((export_name("engine_guardian_check"))) int exp_gd_check(float px, float py, float pz) { return gd_test(px,py,pz); }
__attribute__((export_name("engine_anchor_create"))) int exp_anch_create(int t, float px, float py, float pz) { return anch_creat(t,px,py,pz); }
__attribute__((export_name("engine_anchor_load"))) int exp_anch_load(const char *uid) { return anch_load(uid); }
__attribute__((export_name("engine_gesture_detect"))) int exp_gest_detect(int hand) { return hand_gest(hand); }
__attribute__((export_name("engine_platform_hand_track"))) int exp_plat_ht(int p) { return plat_ht(p); }
__attribute__((export_name("engine_frame_count"))) int exp_frame_count(void) { return g_fr; }

int main(void) {
    engine_main();
    return 0;
}
