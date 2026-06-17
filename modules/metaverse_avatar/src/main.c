/* metaverse_avatar - Virtual Avatar System for AI-ASM OS */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);
#define AV_MAX_NAME 64
#define AV_MAX_BONES 64
#define AV_MAX_ANIMS 64
#define AV_MAX_EXPR 32
#define AV_MAX_EQUIP 16
#define AV_MAX_FRIEND 128
#define AV_MAX_GROUP 32
#define AV_MAX_CHAT 32
#define AV_MAX_EMOTE 16
#define AV_MAX_ASSET 64
#define AV_MAX_FX 8
#define AV_MAX_DLG 32
#define AV_MAX_BEH 16
#define FL_ACTIVE 0x00000001U
#define FL_VISIBLE 0x00000002U
#define FL_ANIM 0x00000004U
#define FL_AI 0x00000008U
#define FL_SYNCED 0x00000010U
#define FL_TRADE 0x00000020U
#define FL_GROUP 0x00000040U
#define FL_ONLINE 0x00000080U
enum AvGender { G_N=0, G_M, G_F, G_C };
enum AvBodyType { B_S=0, B_A, B_V, B_H, B_T };
enum Emotion { E_N=0, E_H, E_D, E_R, E_P, E_F, E_I, E_C, E_X, E_L, E_Z };
enum EquipSlot { S_H=0, S_FA, S_TO, S_HA, S_LE, S_FE, S_W1, S_W2,
    S_BA, S_A1, S_A2, S_X, S_VE, S_PR, S_C };
enum AnimLayer { L_B=0, L_U, L_O, L_F, L_D, L_Z };
enum AssetFmt { F_G=0, F_FB, F_V, F_US, F_CU, F_Z };
typedef struct V3 V3; typedef struct Qt Qt; typedef struct BX BX;
typedef struct SK SK; typedef struct AC AC; typedef struct AS AS;
typedef struct FX FX; typedef struct AP AP; typedef struct OI OI;
typedef struct AI AI; typedef struct CH CH; typedef struct EI EI;
typedef struct EF EF; typedef struct FE FE; typedef struct GM GM;
typedef struct CM CM; typedef struct EM EM; typedef struct SO SO;
typedef struct DL DL; typedef struct BN BN; typedef struct BR BR;
typedef struct SY SY; typedef struct AD AD; typedef struct CX CX;
struct V3 { float x, y, z; };
struct Qt { float x, y, z, w; };
struct BX { unsigned int idx; V3 pos; Qt rot; V3 scl; };
struct SK { unsigned int bone_count; char names[AV_MAX_BONES][32];
    int parent[AV_MAX_BONES]; BX rest[AV_MAX_BONES]; };
struct AC { char name[64]; float dur, fps; unsigned int frames, bones; };
struct AS { unsigned int clip, layer, flags; float weight, time, speed; };
struct FX { unsigned int id; float weights[16]; unsigned int count; float intensity; };
struct AP { unsigned int gender, body; float height, weight;
    float skin[3], hair[3], eye[3];
    unsigned int hair_s, face_s, eye_s, nose_s, mouth_s; };
struct OI { unsigned int id, slot, color, variant; };
struct AI { unsigned int id, slot; float scale[3], offset[3]; };
struct CH { char name[AV_MAX_NAME]; unsigned int id, flags; AP appear;
    OI outfit[AV_MAX_EQUIP]; unsigned int outfit_n;
    AI acc[AV_MAX_EQUIP]; unsigned int acc_n; };
struct EI { unsigned int id, slot, rarity, level, upgrades; float bonus;
    unsigned int effect; char name[64]; };
struct EF { unsigned int fx_id, bone; float intensity, duration, elapsed;
    unsigned int active; };
struct FE { unsigned int avatar_id, status, seen; char name[AV_MAX_NAME];
    float relation; };
struct GM { unsigned int avatar_id, role, joined; };
struct CM { unsigned int sender, time, channel; char content[AV_MAX_CHAT]; };
struct EM { unsigned int id, clip; char name[32]; float duration; };
struct SO { FE friends[AV_MAX_FRIEND]; unsigned int friend_n;
    GM group[AV_MAX_GROUP]; unsigned int group_n, guild;
    CM inbox[AV_MAX_CHAT]; unsigned int inbox_n;
    EM emotes[AV_MAX_EMOTE]; unsigned int emote_n; };
struct DL { unsigned int id, emotion, gesture; char trigger[64], response[128];
    float mood; };
struct BN { unsigned int id, type, children_n, children[4], action; float weight; };
struct BR { unsigned int emotion, personality, dlg_n, beh_n;
    float mood, energy, social, learn;
    DL dialogues[AV_MAX_DLG]; BN behaviors[AV_MAX_BEH]; };
struct SY { unsigned int device, platform, version;
    unsigned int last_sync, dirty, checksum; };
struct AD { unsigned int id, format, size, checksum; char uri[128]; };
struct CX { CH character; SK skeleton; AC clips[AV_MAX_ANIMS]; unsigned int clip_n;
    AS states[AV_MAX_ANIMS]; unsigned int state_n;
    FX exprs[AV_MAX_EXPR]; unsigned int expr_n;
    EI equip[AV_MAX_EQUIP]; unsigned int equip_n;
    EF effects[AV_MAX_FX]; unsigned int fx_n;
    BR brain; SO social; SY sync;
    AD assets[AV_MAX_ASSET]; unsigned int asset_n, tick, flags; };
static void u_memset(void *dst, unsigned char v, unsigned int n) {
    unsigned char *p = (unsigned char *)dst;
    for (unsigned int i = 0; i < n; i++) p[i] = v;
}
static int u_strcmp(const char *a, const char *b) {
    while (*a && *b && *a == *b) { a++; b++; }
    return (unsigned char)*a - (unsigned char)*b;
}
static unsigned int u_strlen(const char *s) {
    unsigned int n = 0; while (s[n]) n++; return n;
}
static float u_clamp(float v, float lo, float hi) {
    if (v < lo) return lo; if (v > hi) return hi; return v;
}
static float u_lerp(float a, float b, float t) { return a + (b - a) * t; }
static Qt q_slerp(Qt a, Qt b, float t) {
    float dot = a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
    if (dot < 0.0f) { b.x=-b.x; b.y=-b.y; b.z=-b.z; b.w=-b.w; dot=-dot; }
    if (dot > 0.9995f) { Qt r; r.x=u_lerp(a.x,b.x,t); r.y=u_lerp(a.y,b.y,t);
        r.z=u_lerp(a.z,b.z,t); r.w=u_lerp(a.w,b.w,t); return r; }
    float wa=1.0f-t, wb=t; Qt r;
    r.x=a.x*wa+b.x*wb; r.y=a.y*wa+b.y*wb;
    r.z=a.z*wa+b.z*wb; r.w=a.w*wa+b.w*wb; return r;
}
static void str_copy(char *dst, const char *src, unsigned int max_len) {
    unsigned int len = u_strlen(src);
    if (len >= max_len) len = max_len - 1;
    for (unsigned int i = 0; i < len; i++) dst[i] = src[i];
    dst[len] = '\0';
}
static unsigned int char_create(CH *ch, const char *name) {
    if (!ch || !name) return 0;
    u_memset(ch, 0, sizeof(CH));
    str_copy(ch->name, name, AV_MAX_NAME);
    ch->appear.gender=G_N; ch->appear.body=B_V;
    ch->appear.height=1.7f; ch->appear.weight=70.0f;
    ch->appear.skin[0]=0.9f; ch->appear.skin[1]=0.75f; ch->appear.skin[2]=0.6f;
    ch->appear.hair[0]=0.2f; ch->appear.hair[1]=0.15f; ch->appear.hair[2]=0.1f;
    ch->appear.eye[0]=0.3f; ch->appear.eye[1]=0.5f; ch->appear.eye[2]=0.8f;
    ch->flags |= FL_ACTIVE; return 1;
}
static unsigned int char_customize(CH *ch, float h, float w, unsigned int bt) {
    if (!ch) return 0;
    ch->appear.height=u_clamp(h,1.2f,2.2f);
    ch->appear.weight=u_clamp(w,40.0f,150.0f);
    if (bt < B_T) ch->appear.body=bt;
    return 1;
}
static unsigned int char_edit_face(CH *ch, unsigned int region, unsigned int shape) {
    if (!ch) return 0;
    switch (region) {
    case 0: ch->appear.face_s=shape; break;
    case 1: ch->appear.eye_s=shape; break;
    case 2: ch->appear.nose_s=shape; break;
    case 3: ch->appear.mouth_s=shape; break;
    default: return 0; }
    return 1;
}
static unsigned int char_set_color(CH *ch, unsigned int which, float r, float g, float b) {
    if (!ch || which > 2) return 0;
    float *rgb = (which==0)?ch->appear.skin:(which==1)?ch->appear.hair:ch->appear.eye;
    rgb[0]=u_clamp(r,0.0f,1.0f); rgb[1]=u_clamp(g,0.0f,1.0f); rgb[2]=u_clamp(b,0.0f,1.0f);
    return 1;
}
static unsigned int char_equip_outfit(CH *ch, unsigned int slot, unsigned int item_id, unsigned int variant) {
    if (!ch || slot >= S_C) return 0;
    for (unsigned int i=0; i < ch->outfit_n; i++)
        if (ch->outfit[i].slot==slot) { ch->outfit[i].id=item_id; ch->outfit[i].variant=variant; return 1; }
    if (ch->outfit_n >= AV_MAX_EQUIP) return 0;
    OI *it=&ch->outfit[ch->outfit_n++];
    it->slot=slot; it->id=item_id; it->variant=variant; it->color=0xFFFFFFU;
    return 1;
}
static unsigned int char_attach_acc(CH *ch, unsigned int slot, unsigned int item_id,
    float sx, float sy, float sz) {
    if (!ch || ch->acc_n >= AV_MAX_EQUIP) return 0;
    AI *a=&ch->acc[ch->acc_n++];
    a->id=item_id; a->slot=slot;
    a->scale[0]=sx; a->scale[1]=sy; a->scale[2]=sz;
    a->offset[0]=0.0f; a->offset[1]=0.0f; a->offset[2]=0.0f;
    return 1;
}
static unsigned int char_match_outfit(const CH *ch) {
    if (!ch || ch->outfit_n==0) return 0;
    unsigned int base=ch->outfit[0].color, matches=0;
    for (unsigned int i=1; i < ch->outfit_n; i++)
        if ((ch->outfit[i].color&0xFF0000U)==(base&0xFF0000U)) matches++;
    return matches;
}
static unsigned int anim_init_skeleton(SK *sk) {
    if (!sk) return 0; u_memset(sk, 0, sizeof(SK)); return 1;
}
static unsigned int anim_add_bone(SK *sk, const char *name, int parent, V3 pos) {
    if (!sk || sk->bone_count >= AV_MAX_BONES) return 0;
    unsigned int idx=sk->bone_count++;
    str_copy(sk->names[idx], name, 32); sk->parent[idx]=parent;
    sk->rest[idx].idx=idx; sk->rest[idx].pos=pos;
    sk->rest[idx].rot=(Qt){0,0,0,1}; sk->rest[idx].scl=(V3){1,1,1};
    return 1;
}
static unsigned int anim_register_clip(CX *ctx, const char *name, float dur, float fps) {
    if (!ctx || ctx->clip_n >= AV_MAX_ANIMS) return 0;
    AC *c=&ctx->clips[ctx->clip_n];
    u_memset(c, 0, sizeof(AC)); str_copy(c->name, name, 64);
    c->dur=dur; c->fps=fps; c->frames=(unsigned int)(dur*fps);
    c->bones=ctx->skeleton.bone_count;
    return ctx->clip_n++;
}
static unsigned int anim_play(CX *ctx, unsigned int ci, unsigned int layer, float weight, float speed) {
    if (!ctx || ctx->state_n >= AV_MAX_ANIMS || ci >= ctx->clip_n) return 0;
    AS *s=&ctx->states[ctx->state_n++];
    s->clip=ci; s->layer=layer; s->weight=u_clamp(weight,0.0f,1.0f);
    s->speed=speed; s->time=0.0f; s->flags=1;
    return 1;
}
static void anim_update(CX *ctx, float dt) {
    if (!ctx) return;
    for (unsigned int i=0; i < ctx->state_n; i++) {
        AS *s=&ctx->states[i];
        if (!(s->flags&1)) continue;
        s->time += dt*s->speed;
        if (s->time >= ctx->clips[s->clip].dur) s->time=0.0f;
    }
}
static void anim_blend_xform(BX *out, const BX *a, const BX *b, float t) {
    out->pos.x=u_lerp(a->pos.x,b->pos.x,t);
    out->pos.y=u_lerp(a->pos.y,b->pos.y,t);
    out->pos.z=u_lerp(a->pos.z,b->pos.z,t);
    out->rot=q_slerp(a->rot,b->rot,t);
    out->scl.x=u_lerp(a->scl.x,b->scl.x,t);
    out->scl.y=u_lerp(a->scl.y,b->scl.y,t);
    out->scl.z=u_lerp(a->scl.z,b->scl.z,t);
}
static unsigned int anim_set_expr(CX *ctx, unsigned int id, float intensity) {
    if (!ctx || id >= AV_MAX_EXPR) return 0;
    FX *e=&ctx->exprs[id]; e->id=id;
    e->intensity=u_clamp(intensity,0.0f,1.0f);
    if (id >= ctx->expr_n) ctx->expr_n=id+1;
    return 1;
}
static unsigned int anim_register_gesture(CX *ctx, const char *name, unsigned int ci) {
    if (!ctx || ci >= ctx->clip_n) return 0; (void)name; return ci;
}
static void anim_eval_layers(CX *ctx, BX *out, unsigned int bc) {
    if (!ctx || !out) return;
    for (unsigned int b=0; b < bc && b < AV_MAX_BONES; b++) out[b]=ctx->skeleton.rest[b];
    for (unsigned int layer=0; layer < L_Z; layer++) {
        for (unsigned int i=0; i < ctx->state_n; i++) {
            if (ctx->states[i].layer!=layer || !(ctx->states[i].flags&1)) continue;
            float w=ctx->states[i].weight;
            for (unsigned int b=0; b < bc && b < AV_MAX_BONES; b++) {
                BX bl; anim_blend_xform(&bl,&out[b],&ctx->skeleton.rest[b],w); out[b]=bl;
            }
        }
    }
}
static unsigned int ai_init(BR *br, float lr) {
    if (!br) return 0; u_memset(br, 0, sizeof(BR));
    br->emotion=E_N; br->mood=0.5f; br->energy=1.0f;
    br->social=0.5f; br->learn=u_clamp(lr,0.01f,1.0f);
    return 1;
}
static unsigned int ai_add_dialogue(BR *br, const char *trigger, const char *response,
    unsigned int emo, float mood_shift) {
    if (!br || br->dlg_n >= AV_MAX_DLG) return 0;
    DL *e=&br->dialogues[br->dlg_n]; e->id=br->dlg_n;
    str_copy(e->trigger, trigger, 64); str_copy(e->response, response, 128);
    e->emotion=emo; e->gesture=0; e->mood=mood_shift;
    return br->dlg_n++;
}
static unsigned int ai_find_dialogue(const BR *br, const char *input) {
    if (!br || !input) return 0xFFFFFFFFU;
    for (unsigned int i=0; i < br->dlg_n; i++)
        if (u_strcmp(br->dialogues[i].trigger, input)==0) return i;
    return 0xFFFFFFFFU;
}
static unsigned int ai_decide_behavior(const BR *br) {
    if (!br) return 0;
    if (br->mood > 0.7f) return 1;
    if (br->mood < 0.3f) return 2;
    if (br->energy < 0.3f) return 3;
    if (br->social > 0.7f) return 4;
    return 0;
}
static void ai_update_emotions(BR *br, float dt) {
    if (!br) return;
    float decay=dt*0.01f;
    if (br->mood > 0.5f) br->mood-=decay;
    else if (br->mood < 0.5f) br->mood+=decay;
    br->energy-=dt*0.005f;
    if (br->energy < 0.0f) br->energy=0.0f;
    if (br->mood > 0.8f) br->emotion=E_H;
    else if (br->mood < 0.2f) br->emotion=E_D;
    else br->emotion=E_N;
}
static void ai_learn(BR *br, unsigned int outcome, float reward) {
    if (!br) return;
    float lr=br->learn;
    br->mood=u_clamp(br->mood+reward*lr,0.0f,1.0f);
    float sd=(outcome>0?0.05f:-0.05f)*lr;
    br->social=u_clamp(br->social+sd,0.0f,1.0f);
}
static unsigned int ai_personalize(BR *br, unsigned int mask) {
    if (!br) return 0; br->personality=mask; return 1;
}
static unsigned int ai_build_behavior_tree(BR *br) {
    if (!br) return 0; br->beh_n=3;
    br->behaviors[0]=(BN){0,1,2,{1,2},0,1.0f};
    br->behaviors[1]=(BN){1,2,0,{0},1,0.7f};
    br->behaviors[2]=(BN){2,2,0,{0},2,0.3f};
    return 1;
}
static unsigned int equip_attach(CX *ctx, unsigned int slot, unsigned int item_id, unsigned int rarity) {
    if (!ctx || ctx->equip_n >= AV_MAX_EQUIP || slot >= S_C) return 0;
    for (unsigned int i=0; i < ctx->equip_n; i++)
        if (ctx->equip[i].slot==slot) { ctx->equip[i].id=item_id; ctx->equip[i].rarity=rarity; return 1; }
    EI *eq=&ctx->equip[ctx->equip_n++]; u_memset(eq, 0, sizeof(EI));
    eq->slot=slot; eq->id=item_id; eq->rarity=rarity; eq->level=1;
    return 1;
}
static unsigned int equip_upgrade(CX *ctx, unsigned int item_id) {
    if (!ctx) return 0;
    for (unsigned int i=0; i < ctx->equip_n; i++)
        if (ctx->equip[i].id==item_id) {
            ctx->equip[i].level++; ctx->equip[i].upgrades++;
            ctx->equip[i].bonus+=0.1f; return 1;
        }
    return 0;
}
static unsigned int equip_add_fx(CX *ctx, unsigned int fx_id, unsigned int bone, float intensity, float duration) {
    if (!ctx || ctx->fx_n >= AV_MAX_FX) return 0;
    EF *ef=&ctx->effects[ctx->fx_n++];
    ef->fx_id=fx_id; ef->bone=bone;
    ef->intensity=u_clamp(intensity,0.0f,1.0f);
    ef->duration=duration; ef->elapsed=0.0f; ef->active=1;
    return 1;
}
static void equip_update_fx(CX *ctx, float dt) {
    if (!ctx) return;
    for (unsigned int i=0; i < ctx->fx_n; i++) {
        EF *ef=&ctx->effects[i];
        if (!ef->active) continue;
        ef->elapsed+=dt;
        if (ef->elapsed >= ef->duration) ef->active=0;
    }
}
static float equip_total_stats(const CX *ctx) {
    if (!ctx) return 0.0f;
    float total=0.0f;
    for (unsigned int i=0; i < ctx->equip_n; i++) total+=ctx->equip[i].bonus;
    return total;
}
static unsigned int equip_detach(CX *ctx, unsigned int slot) {
    if (!ctx) return 0;
    for (unsigned int i=0; i < ctx->equip_n; i++)
        if (ctx->equip[i].slot==slot) {
            for (unsigned int j=i; j+1 < ctx->equip_n; j++) ctx->equip[j]=ctx->equip[j+1];
            ctx->equip_n--; return 1;
        }
    return 0;
}
static unsigned int social_add_friend(SO *soc, unsigned int id, const char *name) {
    if (!soc || soc->friend_n >= AV_MAX_FRIEND) return 0;
    FE *f=&soc->friends[soc->friend_n++];
    f->avatar_id=id; str_copy(f->name, name, AV_MAX_NAME);
    f->status=1; f->seen=0; f->relation=0.5f;
    return 1;
}
static unsigned int social_remove_friend(SO *soc, unsigned int id) {
    if (!soc) return 0;
    for (unsigned int i=0; i < soc->friend_n; i++)
        if (soc->friends[i].avatar_id==id) {
            for (unsigned int j=i; j+1 < soc->friend_n; j++) soc->friends[j]=soc->friends[j+1];
            soc->friend_n--; return 1;
        }
    return 0;
}
static unsigned int social_invite_group(SO *soc, unsigned int id, unsigned int role) {
    if (!soc || soc->group_n >= AV_MAX_GROUP) return 0;
    GM *m=&soc->group[soc->group_n++];
    m->avatar_id=id; m->role=role; m->joined=0;
    return 1;
}
static unsigned int social_join_guild(SO *soc, unsigned int gid) {
    if (!soc) return 0; soc->guild=gid; return 1;
}
static unsigned int social_chat(SO *soc, unsigned int sender, const char *content, unsigned int channel) {
    if (!soc || soc->inbox_n >= AV_MAX_CHAT) return 0;
    CM *msg=&soc->inbox[soc->inbox_n++];
    msg->sender=sender; msg->time=0; msg->channel=channel;
    str_copy(msg->content, content, AV_MAX_CHAT);
    return 1;
}
static unsigned int social_register_emote(SO *soc, unsigned int id, const char *name, unsigned int clip, float dur) {
    if (!soc || soc->emote_n >= AV_MAX_EMOTE) return 0;
    EM *em=&soc->emotes[soc->emote_n++];
    em->id=id; str_copy(em->name, name, 32);
    em->clip=clip; em->duration=dur;
    return 1;
}
static unsigned int social_update_relation(SO *soc, unsigned int id, float delta) {
    if (!soc) return 0;
    for (unsigned int i=0; i < soc->friend_n; i++)
        if (soc->friends[i].avatar_id==id) {
            soc->friends[i].relation=u_clamp(soc->friends[i].relation+delta,0.0f,1.0f);
            return 1;
        }
    return 0;
}
static unsigned int sync_init(SY *sy, unsigned int dev, unsigned int platform) {
    if (!sy) return 0; u_memset(sy, 0, sizeof(SY));
    sy->device=dev; sy->platform=platform; sy->version=1; sy->dirty=0xFFFFFFFFU;
    return 1;
}
static unsigned int sync_checksum(const CX *ctx) {
    if (!ctx) return 0;
    unsigned int hash=5381U;
    const unsigned char *d=(const unsigned char*)&ctx->character;
    for (unsigned int i=0; i < sizeof(CH); i++) hash=((hash<<5)+hash)+d[i];
    return hash;
}
static unsigned int sync_mark_dirty(SY *sy, unsigned int flags) {
    if (!sy) return 0; sy->dirty|=flags; return 1;
}
static unsigned int sync_perform(SY *sy, unsigned int target) {
    if (!sy || sy->dirty==0) return 0;
    sy->last_sync++; sy->version++; sy->dirty=0; (void)target; return 1;
}
static unsigned int sync_register_asset(CX *ctx, unsigned int fmt, const char *uri, unsigned int size) {
    if (!ctx || ctx->asset_n >= AV_MAX_ASSET) return 0;
    AD *a=&ctx->assets[ctx->asset_n];
    a->id=ctx->asset_n; a->format=fmt; str_copy(a->uri, uri, 128);
    a->size=size; a->checksum=0; ctx->asset_n++;
    return a->id;
}
static unsigned int sync_validate_asset(const AD *asset) {
    if (!asset) return 0;
    if (asset->format >= F_Z || asset->size==0) return 0;
    return 1;
}
static unsigned int sync_convert_format(unsigned int from, unsigned int to) {
    if (from >= F_Z || to >= F_Z) return 0; return 1;
}
static unsigned int ctx_init(CX *ctx, const char *name) {
    if (!ctx) return 0; u_memset(ctx, 0, sizeof(CX));
    if (!char_create(&ctx->character, name)) return 0;
    if (!anim_init_skeleton(&ctx->skeleton)) return 0;
    if (!ai_init(&ctx->brain, 0.1f)) return 0;
    if (!sync_init(&ctx->sync, 1, 0x0FU)) return 0;
    ai_build_behavior_tree(&ctx->brain);
    ctx->flags=FL_ACTIVE|FL_AI; return 1;
}
static void ctx_update(CX *ctx, float dt) {
    if (!ctx) return;
    anim_update(ctx, dt); ai_update_emotions(&ctx->brain, dt);
    equip_update_fx(ctx, dt); ctx->tick++;
}
static unsigned int ctx_process_input(CX *ctx, const char *input) {
    if (!ctx || !input) return 0;
    unsigned int dlg=ai_find_dialogue(&ctx->brain, input);
    if (dlg != 0xFFFFFFFFU) {
        ctx->brain.emotion=ctx->brain.dialogues[dlg].emotion;
        ctx->brain.mood=u_clamp(ctx->brain.mood+ctx->brain.dialogues[dlg].mood,0.0f,1.0f);
        return dlg;
    }
    ai_learn(&ctx->brain, 0, -0.01f); return 0;
}
static float ctx_get_stats(const CX *ctx) { return equip_total_stats(ctx); }
__attribute__((export_name("avatar_init")))
unsigned int avatar_init(unsigned int name_ptr, unsigned int name_len) {
    unsigned int mem=host_alloc(sizeof(CX), 8);
    if (mem==0) { host_print("metaverse_avatar: allocation failed"); host_exit(1); return 0; }
    CX *ctx=(CX*)(unsigned long)mem; char name_buf[AV_MAX_NAME];
    if (name_len >= AV_MAX_NAME) name_len=AV_MAX_NAME-1;
    const char *src=(const char*)(unsigned long)name_ptr;
    for (unsigned int i=0; i < name_len; i++) name_buf[i]=src[i];
    name_buf[name_len]='\0';
    if (!ctx_init(ctx, name_buf)) { host_print("metaverse_avatar: context init failed"); host_exit(1); return 0; }
    V3 root={0,0,0}, spine={0,1,0}, head={0,0.5f,0};
    V3 arm_l={-0.3f,0,0}, arm_r={0.3f,0,0};
    V3 leg_l={-0.15f,-1,0}, leg_r={0.15f,-1,0};
    anim_add_bone(&ctx->skeleton,"root",-1,root);
    anim_add_bone(&ctx->skeleton,"spine",0,spine);
    anim_add_bone(&ctx->skeleton,"head",1,head);
    anim_add_bone(&ctx->skeleton,"arm_left",1,arm_l);
    anim_add_bone(&ctx->skeleton,"arm_right",1,arm_r);
    anim_add_bone(&ctx->skeleton,"leg_left",0,leg_l);
    anim_add_bone(&ctx->skeleton,"leg_right",0,leg_r);
    anim_register_clip(ctx,"idle",2.0f,30.0f);
    anim_register_clip(ctx,"walk",1.0f,30.0f);
    anim_register_clip(ctx,"run",0.8f,30.0f);
    anim_register_clip(ctx,"wave",1.5f,30.0f);
    anim_register_clip(ctx,"dance",3.0f,30.0f);
    anim_play(ctx, 0, L_B, 1.0f, 1.0f);
    ai_add_dialogue(&ctx->brain,"hello","Hi there! Nice to meet you!",E_H,0.1f);
    ai_add_dialogue(&ctx->brain,"bye","See you later!",E_D,-0.05f);
    ai_add_dialogue(&ctx->brain,"how are you","I am feeling great!",E_X,0.15f);
    social_register_emote(&ctx->social,0,"smile",0,1.0f);
    social_register_emote(&ctx->social,1,"laugh",4,2.0f);
    social_register_emote(&ctx->social,2,"cry",0,1.5f);
    social_register_emote(&ctx->social,3,"angry",0,1.0f);
    sync_register_asset(ctx,F_G,"/assets/avatar/body.glb",4096);
    sync_register_asset(ctx,F_G,"/assets/avatar/hair.glb",2048);
    sync_register_asset(ctx,F_V,"/assets/avatar/outfit.vrm",8192);
    host_print("metaverse_avatar: initialized successfully"); return mem;
}
__attribute__((export_name("avatar_update")))
void avatar_update(unsigned int ctx_ptr, float dt) {
    CX *ctx=(CX*)(unsigned long)ctx_ptr; if (ctx) ctx_update(ctx, dt);
}
__attribute__((export_name("avatar_speak")))
unsigned int avatar_speak(unsigned int ctx_ptr, unsigned int input_ptr, unsigned int input_len) {
    CX *ctx=(CX*)(unsigned long)ctx_ptr; if (!ctx) return 0;
    char buf[AV_MAX_CHAT];
    if (input_len >= AV_MAX_CHAT) input_len=AV_MAX_CHAT-1;
    const char *src=(const char*)(unsigned long)input_ptr;
    for (unsigned int i=0; i < input_len; i++) buf[i]=src[i];
    buf[input_len]='\0'; return ctx_process_input(ctx, buf);
}
__attribute__((export_name("avatar_equip")))
unsigned int avatar_equip(unsigned int ctx_ptr, unsigned int slot, unsigned int item_id, unsigned int rarity) {
    CX *ctx=(CX*)(unsigned long)ctx_ptr; if (!ctx) return 0;
    unsigned int res=equip_attach(ctx, slot, item_id, rarity);
    if (res) sync_mark_dirty(&ctx->sync, 0x00000004U); return res;
}
__attribute__((export_name("avatar_sync")))
unsigned int avatar_sync(unsigned int ctx_ptr, unsigned int target) {
    CX *ctx=(CX*)(unsigned long)ctx_ptr; if (!ctx) return 0;
    ctx->sync.checksum=sync_checksum(ctx); return sync_perform(&ctx->sync, target);
}
__attribute__((export_name("avatar_get_emotion")))
unsigned int avatar_get_emotion(unsigned int ctx_ptr) {
    CX *ctx=(CX*)(unsigned long)ctx_ptr; return ctx?ctx->brain.emotion:E_N;
}
__attribute__((export_name("avatar_get_stats")))
float avatar_get_stats(unsigned int ctx_ptr) {
    CX *ctx=(CX*)(unsigned long)ctx_ptr; return ctx?ctx_get_stats(ctx):0.0f;
}
__attribute__((export_name("avatar_get_version")))
unsigned int avatar_get_version(void) { return 0x01000000U; }
__attribute__((export_name("main")))
int main(void) {
    host_print("metaverse_avatar WASM module loaded");
    host_print("version: 1.0.0 | AI-ASM OS");
    host_print("subsystems: character, animation, AI, equipment, social, sync");
    unsigned int ctx=avatar_init(0, 0);
    if (ctx==0) { host_print("metaverse_avatar: fatal init failure"); host_exit(1); return 1; }
    host_print("metaverse_avatar: context ready");
    for (unsigned int i=0; i < 10; i++) avatar_update(ctx, 0.016f);
    avatar_equip(ctx, S_W1, 1001, 3);
    avatar_equip(ctx, S_TO, 2001, 2);
    unsigned int em=avatar_get_emotion(ctx); float stats=avatar_get_stats(ctx);
    (void)em; (void)stats; avatar_sync(ctx, 2);
    host_print("metaverse_avatar: sync complete");
    host_print("metaverse_avatar: module operational");
    return 0;
}
