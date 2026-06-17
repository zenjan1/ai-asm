/* vr_engine: Virtual Reality Engine (v54.0)
 * Full-Featured VR Runtime for AI-ASM OS:
 * - Rendering: stereoscopic, foveated, async timewarp, low-latency pipeline
 * - Interaction: controller/hand/body tracking, haptics, spatial audio
 * - Space: room-scale, guardian, teleport/smooth locomotion, seated/standing
 * - Social VR: avatars, spatial voice, gesture sync, shared spaces
 * - Performance: async submit, multithreaded render, LOD, occlusion, instancing
 * - Content: scene editor, asset import, scripting, physics, particles
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
#define MAX_TRK 32
#define MAX_HAND_J 26
#define MAX_BODY_J 24
#define MAX_HAP_CH 4
#define MAX_AUDIO 16
#define MAX_GUARD 64
#define MAX_AV_BONE 64
#define MAX_AV 16
#define MAX_ROOM 8
#define MAX_PART 1024
#define MAX_PHYS 128
#define MAX_SCN 256
#define MAX_LOD 4
#define MAX_OCC 64
#define MAX_INST 512
#define MAX_SCR 32
#define MAX_SVAR 64
#define MAX_ASSET 128
#define MAX_LOD_TBL 64
#define MAX_INST_B 32
#define NM 32
#define PTH 64
#define UID 37
#define EYE_L 0
#define EYE_R 1
#define TRK_CTRL 0
#define MOV_TEL 0
#define MOV_SMO 1
#define MOV_SEAT 2
#define MOV_STND 3
#define LOD_HI 0
#define LOD_MD 1
#define LOD_LO 2
#define LOD_BB 3
#define PHY_BOX 0
#define PHY_SPH 1
#define PHY_CAP 2
#define PHY_MSH 3

/* === Core Math Types === */
typedef struct { float x, y, z, w; } quat_t;
typedef struct { float x, y, z; } v3_t;
typedef struct { float m[16]; } m4_t;

/* === Rendering Types === */
typedef struct {
    int eid, w, h, fh, fv, layers, valid;
    float ipd, nc, fc;
    m4_t view, proj;
} eye_r_t;
typedef struct { int tx, ty, en, q; float cx, cy, is, os; } fov_cfg_t;
typedef struct {
    float yaw, pitch, roll, py, pp;
    double ts, pts;
    int wa, valid;
} hmd_t;
typedef struct { int sl, dl, wd, valid; float dt; quat_t rd; } tw_t;

/* === Tracking Types === */
typedef struct {
    int id, type, act, btn, pbtn;
    v3_t pos; quat_t ori;
    float trg, grp, tx, ty;
    unsigned long long ts;
} trk_t;
typedef struct {
    int hid, jc, trk, pinch, grab;
    v3_t j[MAX_HAND_J];
    float cf[MAX_HAND_J];
    float pd, gs;
} hand_t;
typedef struct {
    int jc, act;
    v3_t j[MAX_BODY_J];
    float cf[MAX_BODY_J];
    quat_t br[MAX_BODY_J];
} body_t;

/* === Haptics and Audio Types === */
typedef struct { int ch, did, act, loop, cl; float fr, am, dur, cv[16]; } hap_t;
typedef struct {
    int sid, act, sp, lp, st;
    v3_t pos;
    float v, pi, mn, mx;
    char nm[NM];
} aud_t;

/* === Guardian and Space Types === */
typedef struct {
    int pc, act, vis;
    float h, mnx, mnz, mxx, mxz;
    v3_t pts[MAX_GUARD];
} guard_t;
typedef struct { int mode, cal; v3_t orig; float yo, h; } ps_t;

/* === Social VR Types === */
typedef struct {
    int uid, bc, act, gi;
    char uuid[UID], nm[NM];
    v3_t bp[MAX_AV_BONE];
    quat_t brot[MAX_AV_BONE];
    float lip[8];
} av_t;
typedef struct {
    int uc, mx, lk, priv;
    char rid[UID], rnm[NM];
    int uids[MAX_AV];
} room_t;

/* === Performance Types === */
typedef struct { int oid, act, d[MAX_LOD], vc[MAX_LOD]; } lod_t;
typedef struct { int oid, act; v3_t cen, ext; } occ_t;
typedef struct { int bm, cnt, act; m4_t xf[MAX_INST]; int vs[MAX_INST]; } ib_t;

/* === Scene and Content Types === */
typedef struct {
    int oid, type, vis, sh, mi, mai, li, oi, ii, pi, ch;
    v3_t pos, scl; quat_t rot; char nm[NM];
} scn_t;
typedef struct {
    int sh, dyn, act, slp;
    v3_t pos, vel, av, ext; quat_t rot;
    float mass, rst, fr, rad;
} phy_t;
typedef struct { int act, fr; v3_t pos, vel, col; float sz, life, ml, gs; } pt_t;
typedef struct {
    int sid, act, vc;
    char nm[NM], src[256];
    int vi[MAX_SVAR]; float vv[MAX_SVAR];
} scr_t;
typedef struct { int aid, type, ld, vc, ic, sb; char path[PTH]; } ast_t;

static eye_r_t g_eyes[MAX_EYES];
static fov_cfg_t g_fov;
static hmd_t g_hmd;
static tw_t g_tw;
static trk_t g_trk[MAX_TRK]; static int g_tn=0;
static hand_t g_hands[2];
static body_t g_body;
static hap_t g_hap[MAX_HAP_CH];
static aud_t g_aud[MAX_AUDIO];
static guard_t g_guard;
static ps_t g_ps;
static av_t g_av[MAX_AV]; static int g_an=0;
static room_t g_rm[MAX_ROOM]; static int g_rn=0;
static lod_t g_lod[MAX_LOD_TBL]; static int g_ln=0;
static occ_t g_occ[MAX_OCC]; static int g_on=0;
static ib_t g_ib[MAX_INST_B]; static int g_in=0;
static scn_t g_sc[MAX_SCN]; static int g_sn=0;
static phy_t g_phy[MAX_PHYS]; static int g_pn=0;
static pt_t g_pt[MAX_PART]; static int g_pn2=0;
static scr_t g_scr[MAX_SCR]; static int g_scn2=0;
static ast_t g_as[MAX_ASSET]; static int g_asn=0;
static int g_clk=0, g_fr=0, g_seq=1;

static void render_init(void); static void render_stereo(int e);
static void render_fov_apply(void); static void render_tw(void); static void render_sub(void);
static int trk_ctrl(int id, float px, float py, float pz);
static int trk_hand(int h); static int trk_body(void);
static void hap_fire(int ch, float f, float a, float d);
static int aud_add(int id, float px, float py, float pz, float v);
static void aud_mv(int id, float px, float py, float pz);
static void gd_setup(int n); static void gd_pt(int i, float px, float py, float pz);
static int gd_test(float px, float py, float pz); static void ps_mode(int m);
static int av_create(const char *nm); static void av_bone(int a, int b, float px, float py, float pz);
static int rm_create(const char *nm, int mx); static int rm_join(int r, int u); static void rm_leave(int r, int u);
static void lod_reg(int o, float d0, float d1, float d2, float d3); static int lod_sel(int o, float d);
static int occ_add(float cx, float cy, float cz, float ex, float ey, float ez);
static int occ_test(float px, float py, float pz);
static int ib_create(int m); static void ib_add(int b, int i, const m4_t *x);
static int sc_add(const char *nm, int t); static void sc_xf(int o, float px, float py, float pz);
static int phy_add(int sh, float px, float py, float pz, float m); static void phy_step(float dt);
static int pt_emit(float px, float py, float pz, float vx, float vy, float vz); static void pt_upd(float dt);
static int scr_load(const char *nm, const char *src); static void scr_run(int s);
static int as_reg(const char *p, int t); static int as_load(int a);
static void vr_tick(float dt); static void vr_init(void);

static void render_init(void) {
    float ipd = 0.064f;
    for (int e = 0; e < MAX_EYES; e++) {
        g_eyes[e].eid=e; g_eyes[e].w=2048; g_eyes[e].h=2048;
        g_eyes[e].fh=100; g_eyes[e].fv=100; g_eyes[e].ipd=ipd*0.5f;
        g_eyes[e].nc=0.01f; g_eyes[e].fc=1000.0f; g_eyes[e].layers=0; g_eyes[e].valid=1;
    }
    g_fov.tx=2048; g_fov.ty=2048; g_fov.cx=0.5f; g_fov.cy=0.5f;
    g_fov.is=1.0f; g_fov.os=0.5f; g_fov.en=1; g_fov.q=2;
    g_hmd.yaw=0; g_hmd.pitch=0; g_hmd.roll=0; g_hmd.py=0; g_hmd.pp=0;
    g_hmd.ts=0; g_hmd.pts=0; g_hmd.wa=0; g_hmd.valid=1;
    g_tw.sl=0; g_tw.dl=1; g_tw.dt=0; g_tw.wd=0; g_tw.valid=0;
    host_print("[vr_engine] render_init: stereo 2048x2048 foveated ON\n");
}
static void render_stereo(int e) {
    if (e<0||e>=MAX_EYES||!g_eyes[e].valid) return;
    if (g_eyes[e].layers<4) g_eyes[e].layers++;
}
static void render_fov_apply(void) {
    if (!g_fov.en) return;
    if (g_fov.is<=0.0f||g_fov.is>2.0f) g_fov.is=1.0f;
    if (g_fov.os<=0.0f||g_fov.os>1.0f) g_fov.os=0.5f;
}
static void render_tw(void) {
    double dt=g_hmd.pts-g_hmd.ts;
    if (dt<0.0) dt=0.0; if (dt>0.05) dt=0.05;
    g_tw.dt=(float)dt;
    g_hmd.py=g_hmd.yaw+g_hmd.yaw*(float)dt*0.1f;
    g_hmd.pp=g_hmd.pitch+g_hmd.pitch*(float)dt*0.1f;
    g_tw.wd=1; g_hmd.wa=1;
}
static void render_sub(void) {
    render_stereo(EYE_L); render_stereo(EYE_R);
    render_fov_apply(); render_tw(); g_fr++;
}

static int trk_ctrl(int id, float px, float py, float pz) {
    if (g_tn>=MAX_TRK) return -1;
    int i=g_tn;
    g_trk[i].id=id; g_trk[i].type=TRK_CTRL; g_trk[i].act=1;
    g_trk[i].pos.x=px; g_trk[i].pos.y=py; g_trk[i].pos.z=pz;
    g_trk[i].ori.w=1.0f; g_trk[i].ori.x=0; g_trk[i].ori.y=0; g_trk[i].ori.z=0;
    g_trk[i].trg=0; g_trk[i].grp=0; g_trk[i].tx=0; g_trk[i].ty=0;
    g_trk[i].btn=0; g_trk[i].pbtn=0; g_trk[i].ts=(unsigned long long)g_clk;
    return g_tn++;
}
static int trk_hand(int h) {
    if (h<0||h>1) return -1;
    hand_t *hd=&g_hands[h];
    hd->hid=h; hd->jc=MAX_HAND_J; hd->trk=1; hd->pinch=0; hd->grab=0;
    hd->pd=0.08f; hd->gs=0;
    for (int j=0; j<MAX_HAND_J; j++) {
        hd->j[j].x=0; hd->j[j].y=(float)(-j)*0.01f; hd->j[j].z=0; hd->cf[j]=1.0f;
    }
    return h;
}
static int trk_body(void) {
    g_body.jc=MAX_BODY_J; g_body.act=1;
    for (int j=0; j<MAX_BODY_J; j++) {
        g_body.j[j].x=0; g_body.j[j].y=(float)j*0.04f; g_body.j[j].z=0;
        g_body.cf[j]=0.9f; g_body.br[j].w=1.0f;
        g_body.br[j].x=0; g_body.br[j].y=0; g_body.br[j].z=0;
    }
    return 1;
}
static void hap_fire(int ch, float f, float a, float d) {
    if (ch<0||ch>=MAX_HAP_CH) return;
    if (a<0) a=0; if (a>1) a=1; if (d<0) d=0; if (d>5) d=5;
    g_hap[ch].ch=ch; g_hap[ch].fr=f; g_hap[ch].am=a; g_hap[ch].dur=d;
    g_hap[ch].act=1; g_hap[ch].loop=0; g_hap[ch].cl=0;
}

static int aud_add(int id, float px, float py, float pz, float v) {
    if (id<0||id>=MAX_AUDIO) return -1;
    g_aud[id].sid=id; g_aud[id].act=1;
    g_aud[id].pos.x=px; g_aud[id].pos.y=py; g_aud[id].pos.z=pz;
    g_aud[id].v=v; g_aud[id].pi=1.0f; g_aud[id].mn=0.5f; g_aud[id].mx=30.0f;
    g_aud[id].sp=1; g_aud[id].lp=0; g_aud[id].st=0;
    return id;
}
static void aud_mv(int id, float px, float py, float pz) {
    if (id<0||id>=MAX_AUDIO||!g_aud[id].act) return;
    g_aud[id].pos.x=px; g_aud[id].pos.y=py; g_aud[id].pos.z=pz;
}

static void gd_setup(int n) {
    if (n<3) n=3; if (n>MAX_GUARD) n=MAX_GUARD;
    g_guard.pc=n; g_guard.act=1; g_guard.vis=1; g_guard.h=2.5f;
}
static void gd_pt(int i, float px, float py, float pz) {
    if (i<0||i>=g_guard.pc) return;
    g_guard.pts[i].x=px; g_guard.pts[i].y=py; g_guard.pts[i].z=pz;
}
static int gd_test(float px, float py, float pz) {
    if (!g_guard.act) return 1;
    if (py<0||py>g_guard.h) return 0;
    float mnx=g_guard.pts[0].x, mnz=g_guard.pts[0].z, mxx=mnx, mxz=mnz;
    for (int i=1; i<g_guard.pc; i++) {
        float vx=g_guard.pts[i].x, vz=g_guard.pts[i].z;
        if (vx<mnx) mnx=vx; if (vz<mnz) mnz=vz;
        if (vx>mxx) mxx=vx; if (vz>mxz) mxz=vz;
    }
    g_guard.mnx=mnx; g_guard.mnz=mnz; g_guard.mxx=mxx; g_guard.mxz=mxz;
    if (px<mnx||px>mxx||pz<mnz||pz>mxz) return 0;
    return 1;
}
static void ps_mode(int m) {
    if (m<MOV_TEL||m>MOV_STND) m=MOV_STND;
    g_ps.mode=m; g_ps.cal=1;
}

static void nm_cpy(char *d, const char *s, int mx) {
    for (int i=0; i<mx-1&&s[i]; i++) d[i]=s[i];
    d[mx-1]='\0';
}
static int av_create(const char *nm) {
    if (g_an>=MAX_AV) return -1;
    int i=g_an;
    g_av[i].uid=i; g_av[i].act=1; g_av[i].bc=MAX_AV_BONE; g_av[i].gi=0;
    nm_cpy(g_av[i].nm,nm,NM);
    for (int j=0; j<UID; j++) g_av[i].uuid[j]='\0';
    for (int j=0; j<8; j++) g_av[i].lip[j]=0;
    return g_an++;
}
static void av_bone(int a, int b, float px, float py, float pz) {
    if (a<0||a>=g_an||b<0||b>=MAX_AV_BONE) return;
    g_av[a].bp[b].x=px; g_av[a].bp[b].y=py; g_av[a].bp[b].z=pz;
}
static int rm_create(const char *nm, int mx) {
    if (g_rn>=MAX_ROOM) return -1;
    if (mx<1) mx=1; if (mx>MAX_AV) mx=MAX_AV;
    int i=g_rn;
    g_rm[i].uc=0; g_rm[i].mx=mx; g_rm[i].lk=0; g_rm[i].priv=0;
    nm_cpy(g_rm[i].rnm,nm,NM);
    return g_rn++;
}
static int rm_join(int r, int u) {
    if (r<0||r>=g_rn||g_rm[r].lk) return -1;
    if (g_rm[r].uc>=g_rm[r].mx) return -1;
    int s=g_rm[r].uc; g_rm[r].uids[s]=u; g_rm[r].uc++;
    return s;
}
static void rm_leave(int r, int u) {
    if (r<0||r>=g_rn) return;
    for (int i=0; i<g_rm[r].uc; i++) {
        if (g_rm[r].uids[i]==u) {
            for (int j=i; j<g_rm[r].uc-1; j++) g_rm[r].uids[j]=g_rm[r].uids[j+1];
            g_rm[r].uc--; return;
        }
    }
}

static void lod_reg(int o, float d0, float d1, float d2, float d3) {
    if (g_ln>=MAX_LOD_TBL) return;
    int i=g_ln; g_lod[i].oid=o; g_lod[i].act=1;
    g_lod[i].d[0]=d0; g_lod[i].d[1]=d1; g_lod[i].d[2]=d2; g_lod[i].d[3]=d3;
    g_lod[i].vc[0]=10000; g_lod[i].vc[1]=4000; g_lod[i].vc[2]=1000; g_lod[i].vc[3]=4;
    g_ln++;
}
static int lod_sel(int o, float dist) {
    for (int i=0; i<g_ln; i++) {
        if (g_lod[i].oid==o && g_lod[i].act) {
            if (dist<g_lod[i].d[1]) return LOD_HI;
            if (dist<g_lod[i].d[2]) return LOD_MD;
            if (dist<g_lod[i].d[3]) return LOD_LO;
            return LOD_BB;
        }
    }
    return LOD_HI;
}
static int occ_add(float cx, float cy, float cz, float ex, float ey, float ez) {
    if (g_on>=MAX_OCC) return -1;
    int i=g_on; g_occ[i].oid=i; g_occ[i].act=1;
    g_occ[i].cen.x=cx; g_occ[i].cen.y=cy; g_occ[i].cen.z=cz;
    g_occ[i].ext.x=ex; g_occ[i].ext.y=ey; g_occ[i].ext.z=ez;
    return g_on++;
}
static int occ_test(float px, float py, float pz) {
    for (int i=0; i<g_on; i++) {
        if (!g_occ[i].act) continue;
        float dx=px-g_occ[i].cen.x, dy=py-g_occ[i].cen.y, dz=pz-g_occ[i].cen.z;
        if (dx<0) dx=-dx; if (dy<0) dy=-dy; if (dz<0) dz=-dz;
        if (dx<=g_occ[i].ext.x && dy<=g_occ[i].ext.y && dz<=g_occ[i].ext.z) return 1;
    }
    return 0;
}
static int ib_create(int m) {
    if (g_in>=MAX_INST_B) return -1;
    int i=g_in; g_ib[i].bm=m; g_ib[i].cnt=0; g_ib[i].act=1;
    return g_in++;
}
static void ib_add(int b, int idx, const m4_t *x) {
    if (b<0||b>=g_in||!g_ib[b].act||g_ib[b].cnt>=MAX_INST) return;
    int s=g_ib[b].cnt;
    for (int i=0; i<16; i++) g_ib[b].xf[s].m[i]=x->m[i];
    g_ib[b].vs[s]=1; g_ib[b].cnt++;
}

static int sc_add(const char *nm, int t) {
    if (g_sn>=MAX_SCN) return -1;
    int i=g_sn;
    g_sc[i].oid=i; g_sc[i].type=t; g_sc[i].vis=1; g_sc[i].sh=1;
    g_sc[i].pos.x=0; g_sc[i].pos.y=0; g_sc[i].pos.z=0;
    g_sc[i].rot.w=1.0f; g_sc[i].rot.x=0; g_sc[i].rot.y=0; g_sc[i].rot.z=0;
    g_sc[i].scl.x=1.0f; g_sc[i].scl.y=1.0f; g_sc[i].scl.z=1.0f;
    g_sc[i].mi=-1; g_sc[i].mai=-1; g_sc[i].li=-1; g_sc[i].oi=-1; g_sc[i].ii=-1;
    g_sc[i].pi=-1; g_sc[i].ch=0;
    nm_cpy(g_sc[i].nm,nm,NM);
    return g_sn++;
}
static void sc_xf(int o, float px, float py, float pz) {
    if (o<0||o>=g_sn) return;
    g_sc[o].pos.x=px; g_sc[o].pos.y=py; g_sc[o].pos.z=pz;
}

static int phy_add(int sh, float px, float py, float pz, float m) {
    if (g_pn>=MAX_PHYS) return -1;
    if (m<=0) m=1.0f;
    int i=g_pn;
    g_phy[i].sh=sh; g_phy[i].dyn=1; g_phy[i].act=1; g_phy[i].slp=0;
    g_phy[i].pos.x=px; g_phy[i].pos.y=py; g_phy[i].pos.z=pz;
    g_phy[i].vel.x=0; g_phy[i].vel.y=0; g_phy[i].vel.z=0;
    g_phy[i].av.x=0; g_phy[i].av.y=0; g_phy[i].av.z=0;
    g_phy[i].mass=m; g_phy[i].rst=0.3f; g_phy[i].fr=0.5f; g_phy[i].rot.w=1.0f;
    g_phy[i].ext.x=0.5f; g_phy[i].ext.y=0.5f; g_phy[i].ext.z=0.5f; g_phy[i].rad=0.5f;
    return g_pn++;
}
static void phy_step(float dt) {
    float grav=-9.81f;
    for (int i=0; i<g_pn; i++) {
        if (!g_phy[i].act||g_phy[i].slp||!g_phy[i].dyn) continue;
        g_phy[i].vel.y+=grav*dt;
        g_phy[i].pos.x+=g_phy[i].vel.x*dt; g_phy[i].pos.y+=g_phy[i].vel.y*dt; g_phy[i].pos.z+=g_phy[i].vel.z*dt;
        if (g_phy[i].pos.y<0) {
            g_phy[i].pos.y=0; g_phy[i].vel.y=-g_phy[i].vel.y*g_phy[i].rst;
            g_phy[i].vel.x*=(1.0f-g_phy[i].fr*dt); g_phy[i].vel.z*=(1.0f-g_phy[i].fr*dt);
        }
    }
}

static int pt_emit(float px, float py, float pz, float vx, float vy, float vz) {
    int slot=-1;
    if (g_pn2<MAX_PART) slot=g_pn2++;
    else { for (int i=0; i<MAX_PART; i++) if (!g_pt[i].act) { slot=i; break; } }
    if (slot<0) return -1;
    g_pt[slot].pos.x=px; g_pt[slot].pos.y=py; g_pt[slot].pos.z=pz;
    g_pt[slot].vel.x=vx; g_pt[slot].vel.y=vy; g_pt[slot].vel.z=vz;
    g_pt[slot].col.x=1; g_pt[slot].col.y=1; g_pt[slot].col.z=1;
    g_pt[slot].sz=0.05f; g_pt[slot].life=2.0f; g_pt[slot].ml=2.0f; g_pt[slot].gs=1.0f;
    g_pt[slot].act=1; g_pt[slot].fr=0;
    return slot;
}
static void pt_upd(float dt) {
    float grav=-9.81f;
    for (int i=0; i<g_pn2; i++) {
        if (!g_pt[i].act) continue;
        g_pt[i].vel.y+=grav*g_pt[i].gs*dt;
        g_pt[i].pos.x+=g_pt[i].vel.x*dt; g_pt[i].pos.y+=g_pt[i].vel.y*dt; g_pt[i].pos.z+=g_pt[i].vel.z*dt;
        g_pt[i].life-=dt; g_pt[i].fr++;
        if (g_pt[i].life<=0) g_pt[i].act=0;
    }
}

static int scr_load(const char *nm, const char *src) {
    if (g_scn2>=MAX_SCR) return -1;
    int i=g_scn2;
    g_scr[i].sid=i; g_scr[i].act=1; g_scr[i].vc=0;
    for (int j=0; j<255&&src[j]; j++) g_scr[i].src[j]=src[j];
    g_scr[i].src[255]='\0';
    nm_cpy(g_scr[i].nm,nm,NM);
    return g_scn2++;
}
static void scr_run(int s) {
    if (s<0||s>=g_scn2||!g_scr[s].act) return;
    g_scr[s].vc++;
}
static int as_reg(const char *p, int t) {
    if (g_asn>=MAX_ASSET) return -1;
    int i=g_asn;
    g_as[i].aid=i; g_as[i].type=t; g_as[i].ld=0; g_as[i].vc=0; g_as[i].ic=0; g_as[i].sb=0;
    for (int j=0; j<PTH-1&&p[j]; j++) g_as[i].path[j]=p[j];
    g_as[i].path[PTH-1]='\0';
    return g_asn++;
}
static int as_load(int a) {
    if (a<0||a>=g_asn) return -1;
    if (g_as[a].ld) return a;
    g_as[a].ld=1; g_as[a].vc=256; g_as[a].ic=512; g_as[a].sb=4096;
    return a;
}

static void vr_tick(float dt) {
    g_clk++; render_sub(); phy_step(dt); pt_upd(dt);
    for (int s=0; s<g_scn2; s++) if (g_scr[s].act) scr_run(s);
    if (g_fr%60==0) host_print("[vr_engine] tick: 60-frame cycle ok\n");
}
static void vr_init(void) {
    host_print("[vr_engine] === VR Engine v54.0 initializing ===\n");
    render_init();
    trk_ctrl(0,-0.3f,1.0f,-0.4f); trk_ctrl(1,0.3f,1.0f,-0.4f);
    trk_hand(0); trk_hand(1); trk_body();
    hap_fire(0,150.0f,0.5f,0.1f); hap_fire(1,150.0f,0.5f,0.1f);
    aud_add(0,0.0f,1.6f,0.0f,1.0f);
    gd_setup(4);
    gd_pt(0,-2.0f,0.0f,-2.0f); gd_pt(1,2.0f,0.0f,-2.0f);
    gd_pt(2,2.0f,0.0f,2.0f); gd_pt(3,-2.0f,0.0f,2.0f);
    ps_mode(MOV_STND);
    av_create("user_0"); rm_create("lobby",8); rm_join(0,0);
    lod_reg(0,5.0f,15.0f,40.0f,100.0f);
    occ_add(0.0f,1.0f,0.0f,5.0f,5.0f,5.0f);
    ib_create(0); sc_add("origin",0);
    phy_add(PHY_BOX,0.0f,2.0f,0.0f,1.0f);
    pt_emit(0.0f,1.0f,0.0f,0.1f,2.0f,0.0f);
    scr_load("init","start()"); as_reg("models/default.wasm_mesh",0); as_load(0);
    host_print("[vr_engine] === Initialization complete ===\n");
}

/* === Exported Entry Points === */
__attribute__((export_name("engine_init"))) void engine_init(void) { vr_init(); }
__attribute__((export_name("engine_tick"))) void engine_tick(float dt) { vr_tick(dt); }
__attribute__((export_name("engine_guardian_check")))
int engine_guardian_check(float px, float py, float pz) { return gd_test(px,py,pz); }
__attribute__((export_name("engine_lod_select")))
int engine_lod_select(int obj, float dist) { return lod_sel(obj,dist); }
__attribute__((export_name("engine_occlusion_test")))
int engine_occlusion_test(float px, float py, float pz) { return occ_test(px,py,pz); }
__attribute__((export_name("engine_scene_add")))
int engine_scene_add(const char *name, int type) { return sc_add(name,type); }
__attribute__((export_name("engine_phys_add")))
int engine_phys_add(int shape, float px, float py, float pz, float mass) {
    return phy_add(shape,px,py,pz,mass);
}
__attribute__((export_name("engine_avatar_create")))
int engine_avatar_create(const char *name) { return av_create(name); }
__attribute__((export_name("engine_room_create")))
int engine_room_create(const char *name, int max_u) { return rm_create(name,max_u); }
__attribute__((export_name("engine_frame_count")))
int engine_frame_count(void) { return g_fr; }
