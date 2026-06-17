/* spatial_audio - Spatial Audio Engine for AI-ASM OS
 * HRTF binaural rendering, environmental acoustics, multi-source management,
 * real-time audio processing, adaptive audio for VR/AR, games, film, conferencing, music. */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);
#define MAX_SOURCES 64
#define MAX_CHANNELS 8
#define MAX_BANDS 8
#define MAX_REFLECTIONS 16
#define MAX_EFFECTS 16
#define MAX_ROOM_DIM 32
#define SAMPLE_RATE 44100
#define BLOCK_SIZE 256
#define PI 3.14159265358979F
#define TWO_PI 6.28318530717958F
#define HALF_PI 1.57079632679489F
#define SPEED_OF_SOUND 343.0F
#define DEG_TO_RAD 0.01745329251994F
#define RAD_TO_DEG 57.29577951308232F
#define MIN_DISTANCE 0.1F
#define MAX_DISTANCE 500.0F
#define HEAD_RADIUS 0.0875F
#define EAR_SEPARATION 0.18F
#define EPSILON 0.000001F
#define MAX_HRTF_TAPS 128
#define MAX_BLOCK_SIZE 1024
#define SRC_TYPE_POINT 0x01
#define SRC_TYPE_DIRECTIONAL 0x02
#define SRC_TYPE_AMBIENT 0x03
#define SRC_TYPE_SPATIAL 0x04
#define SRC_TYPE_INTERACTIVE 0x05
#define FX_TYPE_REVERB 0x01
#define FX_TYPE_COMPRESS 0x02
#define FX_TYPE_EQ 0x03
#define FX_TYPE_CHORUS 0x04
#define FX_TYPE_DELAY 0x05
#define FX_TYPE_FILTER 0x06
#define MAT_ABSTRACT 0x00
#define MAT_CONCRETE 0x01
#define MAT_WOOD 0x02
#define MAT_GLASS 0x03
#define MAT_CURTAIN 0x04
#define MAT_FOAM 0x05
#define MAT_CARPET 0x06
typedef struct { float x, y, z; } vec3;
typedef struct { float m[9]; } mat3;
typedef struct { float azimuth, elevation, distance; } spherical;
typedef struct { float coeffs[MAX_HRTF_TAPS]; unsigned int num_taps; } hrtf_filter;
typedef struct { hrtf_filter left, right; float azimuth, elevation, distance; } hrtf_entry;
typedef struct { float left[MAX_BLOCK_SIZE], right[MAX_BLOCK_SIZE]; } binaural_buffer;
typedef struct {
    vec3 pos, vel, dir; int type; float volume, pitch, radius;
    float inner_angle, outer_angle, outer_gain, doppler_scale, rolloff;
    float min_dist, max_dist; int active; unsigned int id;
} sound_source;
typedef struct {
    vec3 pos, vel, dir, up; mat3 head_rotation;
    float hrtf_scale, hrtf_shift; int head_tracking;
} listener;
typedef struct { float absorb[MAX_BANDS], diffuse, transmit; int material; } surface_prop;
typedef struct { vec3 pos, normal; surface_prop surface; } room_surface;
typedef struct { vec3 origin, dir; float energy; int bounces; float path_len; } reflection_ray;
typedef struct { reflection_ray rays[MAX_REFLECTIONS]; unsigned int num_rays;
    float decay_rate, wet_mix, diffusion; } reflection_engine;
typedef struct { float width, height, depth; room_surface walls[MAX_ROOM_DIM];
    unsigned int num_walls; float air_absorb, temperature, humidity; } room;
typedef struct { float feedback[MAX_HRTF_TAPS], feedforward[MAX_HRTF_TAPS];
    unsigned int num_taps; float delay_ms, decay, damping, wet_dry, room_size, hf_damp;
} reverb_state;
typedef struct { float threshold, ratio, attack_ms, release_ms, knee, makeup_gain, current_gain;
} compressor_state;
typedef struct { float gain[MAX_BANDS], freq[MAX_BANDS], q[MAX_BANDS]; unsigned int num_bands;
} eq_state;
typedef struct { int type, active; float mix; void *state; } effect_slot;
typedef struct { effect_slot slots[MAX_EFFECTS]; unsigned int num_effects; } effects_chain;
typedef struct { float buffer[MAX_CHANNELS][MAX_BLOCK_SIZE];
    unsigned int num_channels, block_size; float sample_rate; } mix_bus;
typedef struct {
    sound_source sources[MAX_SOURCES]; unsigned int num_sources;
    listener listen; room env_room; reflection_engine refl;
    reverb_state reverb; compressor_state comp; eq_state eq;
    effects_chain fx; mix_bus bus; binaural_buffer binaural;
    hrtf_filter custom_hrtf_left, custom_hrtf_right;
    int scene_mode; float global_volume, adaptive_gain; unsigned int frame_count; int running;
} audio_engine;
/* Forward Declarations */
static vec3 vec3_make(float x, float y, float z);
static vec3 vec3_add(vec3 a, vec3 b);
static vec3 vec3_sub(vec3 a, vec3 b);
static vec3 vec3_scale(vec3 v, float s);
static float vec3_dot(vec3 a, vec3 b);
static vec3 vec3_cross(vec3 a, vec3 b);
static float vec3_length(vec3 v);
static vec3 vec3_normalize(vec3 v);
static float vec3_dist(vec3 a, vec3 b);
static float clampf(float v, float lo, float hi);
static float lerp(float a, float b, float t);
static float db_to_linear(float db);
static float linear_to_db(float lin);
static spherical cartesian_to_spherical(vec3 v);
static mat3 rotation_matrix(float ax, float ay, float az);
static vec3 apply_rotation(mat3 m, vec3 v);
static void hrtf_init(audio_engine *eng);
static void hrtf_select(const audio_engine *eng, float az, float el, float dist,
                         hrtf_filter *out_l, hrtf_filter *out_r);
static void hrtf_apply(const hrtf_filter *flt, const float *in, float *out, unsigned int len);
static float calc_distance_atten(float dist, float min_d, float max_d, float rolloff);
static float calc_doppler_shift(vec3 sp, vec3 sv, vec3 lp, vec3 lv, float bp);
static float calc_directional_gain(vec3 sd, vec3 tl, float inner, float outer, float og);
static void source_init(sound_source *src, int type, vec3 pos);
static void source_update(sound_source *src, vec3 pos, vec3 vel, float vol);
static void room_init(room *r, float w, float h, float d, int mat);
static void room_add_surface(room *r, vec3 pos, vec3 norm, int mat);
static float room_calc_absorption(const room *r, unsigned int band);
static void reflection_init(reflection_engine *re);
static void reflection_trace(reflection_engine *re, const room *r, vec3 sp, vec3 lp);
static void reflection_apply(reflection_engine *re, const float *in, float *out, unsigned int len);
static void reverb_init(reverb_state *rv, float size, float decay, float damp);
static void reverb_process(reverb_state *rv, const float *in, float *out, unsigned int len);
static void compressor_init(compressor_state *cs, float thresh, float ratio,
                             float attack, float release, float knee, float makeup);
static void compressor_process(compressor_state *cs, const float *in, float *out, unsigned int len);
static void eq_init(eq_state *es);
static void eq_set_band(eq_state *es, unsigned int band, float freq, float gain, float q);
static void eq_process(const eq_state *es, const float *in, float *out, unsigned int len);
static void effects_chain_init(effects_chain *fx);
static void effects_chain_add(effects_chain *fx, int type, float mix);
static void effects_chain_process(const effects_chain *fx, float *buf, unsigned int len);
static void binaural_render(audio_engine *eng, const float *mono_in,
                              float az, float el, float dist, float gain);
static void mix_bus_init(mix_bus *bus, unsigned int ch, float sr);
static void mix_bus_add(mix_bus *bus, unsigned int ch, const float *data, unsigned int len);
static void mix_bus_render(mix_bus *bus, float *left, float *right, unsigned int len);
static void adaptive_head_track(audio_engine *eng, float yaw, float pitch_r, float roll);
static void adaptive_scene_switch(audio_engine *eng, int scene);
static float adaptive_volume(audio_engine *eng, float input_level);
static void adaptive_personalize_hrtf(audio_engine *eng, float scale, float shift);
static void engine_init(audio_engine *eng);
static void engine_process_block(audio_engine *eng, const float *input,
                                  float *out_l, float *out_r, unsigned int len);
static void engine_shutdown(audio_engine *eng);
/* ── Math Utilities ── */
static vec3 vec3_make(float x, float y, float z) { vec3 v; v.x=x; v.y=y; v.z=z; return v; }
static vec3 vec3_add(vec3 a, vec3 b) { vec3 v; v.x=a.x+b.x; v.y=a.y+b.y; v.z=a.z+b.z; return v; }
static vec3 vec3_sub(vec3 a, vec3 b) { vec3 v; v.x=a.x-b.x; v.y=a.y-b.y; v.z=a.z-b.z; return v; }
static vec3 vec3_scale(vec3 v, float s) { vec3 r; r.x=v.x*s; r.y=v.y*s; r.z=v.z*s; return r; }
static float vec3_dot(vec3 a, vec3 b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
static vec3 vec3_cross(vec3 a, vec3 b) {
    vec3 v; v.x=a.y*b.z-a.z*b.y; v.y=a.z*b.x-a.x*b.z; v.z=a.x*b.y-a.y*b.x; return v;
}
static float vec3_length(vec3 v) {
    float d=vec3_dot(v,v); if(d<EPSILON) return 0;
    float x=d,y=(x+1)*0.5F; y=(y+d/y)*0.5F; y=(y+d/y)*0.5F; y=(y+d/y)*0.5F; return y;
}
static vec3 vec3_normalize(vec3 v) {
    float len=vec3_length(v); if(len<EPSILON) return vec3_make(0,0,0);
    return vec3_scale(v,1/len);
}
static float vec3_dist(vec3 a, vec3 b) { return vec3_length(vec3_sub(a,b)); }
static float clampf(float v, float lo, float hi) { return v<lo?lo:(v>hi?hi:v); }
static float lerp(float a, float b, float t) { return a+(b-a)*t; }
static float db_to_linear(float db) {
    float x=db*0.115129254649702F,r=1+x; r=r*r; r=r*r; return r;
}
static float linear_to_db(float lin) {
    if(lin<EPSILON) return -120; float x=lin,la=0;
    while(x>=2){x*=0.5F;la+=1;} while(x<1){x*=2;la-=1;}
    return (la*0.693147180559945F+(x-1)/(x+1)*2)*8.68588963806504F;
}
static spherical cartesian_to_spherical(vec3 v) {
    spherical s; s.distance=vec3_length(v);
    if(s.distance<EPSILON){s.azimuth=0;s.elevation=0;return s;}
    float xz=v.x*v.x+v.z*v.z; s.azimuth=0;
    if(xz>EPSILON){float t=xz,r=(t+1)*0.5F;r=(r+t/r)*0.5F;r=(r+t/r)*0.5F;s.azimuth=v.z/r;}
    s.elevation=v.y/s.distance; return s;
}
static mat3 rotation_matrix(float ax, float ay, float az) {
    mat3 m; float cx=1,sx=0,cy=1,sy=0,cz=1,sz=0;
    sx=ax;cx=1-ax*ax*0.5F;sy=ay;cy=1-ay*ay*0.5F;sz=az;cz=1-az*az*0.5F;
    m.m[0]=cy*cz;m.m[1]=-cy*sz;m.m[2]=sy;m.m[3]=sx*sy*cz+cx*sz;
    m.m[4]=-sx*sy*sz+cx*cz;m.m[5]=-sx*cy;m.m[6]=-cx*sy*cz+sx*sz;
    m.m[7]=cx*sy*sz+sx*cz;m.m[8]=cx*cy; return m;
}
static vec3 apply_rotation(mat3 m, vec3 v) {
    vec3 r; r.x=m.m[0]*v.x+m.m[1]*v.y+m.m[2]*v.z;
    r.y=m.m[3]*v.x+m.m[4]*v.y+m.m[5]*v.z;
    r.z=m.m[6]*v.x+m.m[7]*v.y+m.m[8]*v.z; return r;
}
/* ── HRTF Functions ── */
static void hrtf_init(audio_engine *eng) {
    for(unsigned int i=0;i<MAX_HRTF_TAPS;i++){eng->custom_hrtf_left.coeffs[i]=0;eng->custom_hrtf_right.coeffs[i]=0;}
    eng->custom_hrtf_left.coeffs[0]=1;eng->custom_hrtf_right.coeffs[0]=1;
    eng->custom_hrtf_left.num_taps=8;eng->custom_hrtf_right.num_taps=8;
    for(unsigned int a=0;a<36;a++){
        float az=(float)a*10*DEG_TO_RAD,delay=HEAD_RADIUS*az/SPEED_OF_SOUND;
        unsigned int taps=(unsigned int)(delay*SAMPLE_RATE); if(taps>=MAX_HRTF_TAPS)taps=MAX_HRTF_TAPS-1;
        float gain=1-0.3F*(az/PI); if(gain<0.1F)gain=0.1F;
        eng->custom_hrtf_left.coeffs[taps]=gain;
        unsigned int tr=(unsigned int)(delay*SAMPLE_RATE*0.7F); if(tr>=MAX_HRTF_TAPS)tr=MAX_HRTF_TAPS-1;
        eng->custom_hrtf_right.coeffs[tr]=gain*0.8F;
    }
}
static void hrtf_select(const audio_engine *eng, float az, float el, float dist,
                          hrtf_filter *out_l, hrtf_filter *out_r) {
    (void)el;(void)dist; float an=az/PI; if(an<0)an=-an;
    unsigned int idx=(unsigned int)(an*(MAX_HRTF_TAPS-1)); if(idx>=MAX_HRTF_TAPS)idx=MAX_HRTF_TAPS-1;
    for(unsigned int i=0;i<MAX_HRTF_TAPS;i++){
        unsigned int off=(i+idx)%MAX_HRTF_TAPS;
        out_l->coeffs[i]=eng->custom_hrtf_left.coeffs[off];
        out_r->coeffs[i]=eng->custom_hrtf_right.coeffs[off];
    }
    out_l->num_taps=eng->custom_hrtf_left.num_taps; out_r->num_taps=eng->custom_hrtf_right.num_taps;
}
static void hrtf_apply(const hrtf_filter *flt, const float *in, float *out, unsigned int len) {
    unsigned int taps=flt->num_taps; if(!taps)taps=1;
    for(unsigned int i=0;i<len;i++){float s=0;for(unsigned int t=0;t<taps&&t<=i;t++)s+=in[i-t]*flt->coeffs[t];out[i]=s;}
}
/* ── Distance, Doppler, Direction ── */
static float calc_distance_atten(float dist, float min_d, float max_d, float rolloff) {
    if(dist<min_d)dist=min_d;if(dist>max_d)dist=max_d;if(dist<EPSILON)return 1;
    return clampf(min_d/(min_d+rolloff*(dist-min_d)),0,1);
}
static float calc_doppler_shift(vec3 sp, vec3 sv, vec3 lp, vec3 lv, float bp) {
    vec3 dir=vec3_normalize(vec3_sub(lp,sp));
    float sr=vec3_dot(sv,dir),lr=vec3_dot(lv,dir);
    float num=SPEED_OF_SOUND-lr,den=SPEED_OF_SOUND-sr;
    if(den<EPSILON)den=EPSILON;if(num<EPSILON)num=EPSILON;
    return bp*clampf(num/den,0.5F,2);
}
static float calc_directional_gain(vec3 sd, vec3 tl, float inner, float outer, float og) {
    float ca=vec3_dot(vec3_normalize(sd),vec3_normalize(tl));
    float ac=ca<0?-ca:ca;if(ac>1)ac=1;
    float angle=HALF_PI*(1-ac);if(ca<0)angle=PI-angle;
    float ir=inner*DEG_TO_RAD,or2=outer*DEG_TO_RAD;
    if(angle<=ir)return 1;if(angle>=or2)return og;
    return lerp(1,og,(angle-ir)/(or2-ir));
}
/* ── Sound Source Management ── */
static void source_init(sound_source *src, int type, vec3 pos) {
    src->pos=pos;src->vel=vec3_make(0,0,0);src->dir=vec3_make(0,0,-1);
    src->type=type;src->volume=1;src->pitch=1;src->radius=1;
    src->inner_angle=360;src->outer_angle=360;src->outer_gain=1;
    src->doppler_scale=1;src->rolloff=1;src->min_dist=MIN_DISTANCE;
    src->max_dist=MAX_DISTANCE;src->active=1;src->id=0;
}
static void source_update(sound_source *src, vec3 pos, vec3 vel, float vol) {
    src->pos=pos;src->vel=vel;src->volume=clampf(vol,0,1);
}
/* ── Room Modeling ── */
static void room_init(room *r, float w, float h, float d, int mat) {
    r->width=w;r->height=h;r->depth=d;r->num_walls=0;
    r->air_absorb=0.01F;r->temperature=20;r->humidity=50;
    room_add_surface(r,vec3_make(0,0,0),vec3_make(0,1,0),mat);
    room_add_surface(r,vec3_make(0,h,0),vec3_make(0,-1,0),mat);
    room_add_surface(r,vec3_make(0,0,0),vec3_make(1,0,0),mat);
    room_add_surface(r,vec3_make(w,0,0),vec3_make(-1,0,0),mat);
    room_add_surface(r,vec3_make(0,0,0),vec3_make(0,0,1),mat);
    room_add_surface(r,vec3_make(0,0,d),vec3_make(0,0,-1),mat);
}
static void room_add_surface(room *r, vec3 pos, vec3 norm, int mat) {
    if(r->num_walls>=MAX_ROOM_DIM)return; unsigned int idx=r->num_walls;
    r->walls[idx].pos=pos;r->walls[idx].normal=vec3_normalize(norm);
    for(unsigned int b=0;b<MAX_BANDS;b++){
        float ba=0.1F;
        if(mat==MAT_CURTAIN)ba=0.5F+0.1F*b;else if(mat==MAT_FOAM)ba=0.8F+0.02F*b;
        else if(mat==MAT_CARPET)ba=0.3F+0.05F*b;else if(mat==MAT_CONCRETE)ba=0.03F+0.01F*b;
        else if(mat==MAT_WOOD)ba=0.1F+0.02F*b;else if(mat==MAT_GLASS)ba=0.02F+0.005F*b;
        r->walls[idx].surface.absorb[b]=clampf(ba,0,1);
        r->walls[idx].surface.diffuse=0.5F;r->walls[idx].surface.transmit=0.05F;
        r->walls[idx].surface.material=mat;
    }
    r->num_walls++;
}
static float room_calc_absorption(const room *r, unsigned int band) {
    if(!r->num_walls||band>=MAX_BANDS)return 0.1F;
    float total=0;for(unsigned int i=0;i<r->num_walls;i++)total+=r->walls[i].surface.absorb[band];
    return total/(float)r->num_walls;
}
/* ── Reflection Simulation ── */
static void reflection_init(reflection_engine *re) {
    re->num_rays=0;re->decay_rate=0.5F;re->wet_mix=0.3F;re->diffusion=0.6F;
    for(unsigned int i=0;i<MAX_REFLECTIONS;i++){re->rays[i].origin=vec3_make(0,0,0);re->rays[i].dir=vec3_make(0,0,1);re->rays[i].energy=0;re->rays[i].bounces=0;re->rays[i].path_len=0;}
}
static void reflection_trace(reflection_engine *re, const room *r, vec3 sp, vec3 lp) {
    if(!r->num_walls)return; vec3 tw=vec3_sub(lp,sp);float dd=vec3_length(tw);re->num_rays=0;
    for(unsigned int w=0;w<r->num_walls&&re->num_rays<MAX_REFLECTIONS;w++){
        float nd=vec3_dot(r->walls[w].normal,tw);
        if(nd<EPSILON&&nd>-EPSILON)continue;
        float t=vec3_dot(vec3_sub(r->walls[w].pos,sp),r->walls[w].normal)/nd;
        if(t<0||t>1.5F)continue;
        vec3 hit=vec3_add(sp,vec3_scale(tw,t));
        float path=vec3_dist(sp,hit)+vec3_dist(hit,lp),atten=dd/(path+EPSILON);
        float abs_c=room_calc_absorption(r,2);reflection_ray *ray=&re->rays[re->num_rays];
        ray->origin=hit;ray->dir=vec3_normalize(vec3_sub(lp,hit));
        ray->energy=atten*(1-abs_c);ray->bounces=1;ray->path_len=path;re->num_rays++;
    }
}
static void reflection_apply(reflection_engine *re, const float *in, float *out, unsigned int len) {
    for(unsigned int i=0;i<len;i++){float wet=0;
        for(unsigned int r=0;r<re->num_rays;r++){unsigned int delay=(unsigned int)(re->rays[r].path_len/SPEED_OF_SOUND*SAMPLE_RATE);if(delay<len&&i>=delay)wet+=in[i-delay]*re->rays[r].energy;}
        out[i]=in[i]+wet*re->wet_mix;}
}
/* ── Reverb ── */
static void reverb_init(reverb_state *rv, float size, float decay, float damp) {
    rv->delay_ms=size*60;rv->decay=clampf(decay,0.01F,0.99F);rv->damping=clampf(damp,0,1);
    rv->wet_dry=0.35F;rv->room_size=size;rv->hf_damp=damp;
    unsigned int ds=(unsigned int)(rv->delay_ms*0.001F*SAMPLE_RATE);if(ds>=MAX_HRTF_TAPS)ds=MAX_HRTF_TAPS-1;
    rv->num_taps=ds;for(unsigned int i=0;i<MAX_HRTF_TAPS;i++){rv->feedback[i]=0;rv->feedforward[i]=0;}
    for(unsigned int i=0;i<ds;i++){float t=(float)i/ds;rv->feedback[i]=rv->decay*(1-rv->damping*t);rv->feedforward[i]=(1-rv->wet_dry)*(1-t*0.5F);}
}
static void reverb_process(reverb_state *rv, const float *in, float *out, unsigned int len) {
    unsigned int taps=rv->num_taps;if(!taps)taps=1;
    for(unsigned int i=0;i<len;i++){float dry=in[i],wet=0;
        for(unsigned int t=0;t<taps&&t<=i;t++)wet+=in[i-t]*rv->feedback[t];
        float prev=i>0?out[i-1]:0;wet=wet*(1-rv->hf_damp)+prev*rv->hf_damp;
        out[i]=dry*(1-rv->wet_dry)+wet*rv->wet_dry;}
}
/* ── Dynamic Compression ── */
static void compressor_init(compressor_state *cs, float thresh, float ratio, float attack, float release, float knee, float makeup) {
    cs->threshold=thresh;cs->ratio=clampf(ratio,1,20);cs->attack_ms=clampf(attack,0.1F,500);
    cs->release_ms=clampf(release,1,5000);cs->knee=clampf(knee,0,12);cs->makeup_gain=makeup;cs->current_gain=1;
}
static void compressor_process(compressor_state *cs, const float *in, float *out, unsigned int len) {
    float ac=1-0.001F/cs->attack_ms,rc=1-0.001F/cs->release_ms;
    for(unsigned int i=0;i<len;i++){float lvl=in[i]<0?-in[i]:in[i];float ldb=linear_to_db(lvl),gdb=0,hk=cs->knee*0.5F;
        if(ldb<cs->threshold-hk)gdb=0;else if(ldb>cs->threshold+hk)gdb=(cs->threshold-ldb)*(1-1/cs->ratio);
        else{float x=ldb-cs->threshold+hk;gdb=x*x/(2*cs->knee)*(1-1/cs->ratio);}
        float tgt=db_to_linear(gdb)*cs->makeup_gain,c=tgt<cs->current_gain?ac:rc;
        cs->current_gain=cs->current_gain*c+tgt*(1-c);out[i]=in[i]*cs->current_gain;}
}
/* ── EQ Adjustment ── */
static void eq_init(eq_state *es) {
    es->num_bands=MAX_BANDS;for(unsigned int b=0;b<MAX_BANDS;b++){es->freq[b]=100*(1+b*1.5F);es->gain[b]=1;es->q[b]=0.707F;}
}
static void eq_set_band(eq_state *es, unsigned int band, float freq, float gain, float q) {
    if(band>=MAX_BANDS)return;es->freq[band]=clampf(freq,20,20000);es->gain[band]=clampf(gain,0,4);es->q[band]=clampf(q,0.1F,10);
}
static void eq_process(const eq_state *es, const float *in, float *out, unsigned int len) {
    for(unsigned int i=0;i<len;i++){float sum=in[i],tw=0;
        for(unsigned int b=0;b<es->num_bands;b++){float w=es->gain[b]/(1+b*0.1F);sum+=in[i]*(w-1)*0.1F;tw+=w;}
        out[i]=tw>EPSILON?sum/(1+(tw-es->num_bands)*0.01F):in[i];}
}
/* ── Effects Chain ── */
static void effects_chain_init(effects_chain *fx) {
    fx->num_effects=0;for(unsigned int i=0;i<MAX_EFFECTS;i++){fx->slots[i].type=0;fx->slots[i].active=0;fx->slots[i].mix=0;fx->slots[i].state=NULL;}
}
static void effects_chain_add(effects_chain *fx, int type, float mix) {
    if(fx->num_effects>=MAX_EFFECTS)return;unsigned int i=fx->num_effects;
    fx->slots[i].type=type;fx->slots[i].active=1;fx->slots[i].mix=clampf(mix,0,1);fx->slots[i].state=NULL;fx->num_effects++;
}
static void effects_chain_process(const effects_chain *fx, float *buf, unsigned int len) {
    for(unsigned int i=0;i<fx->num_effects;i++){if(!fx->slots[i].active)continue;float mix=fx->slots[i].mix;for(unsigned int s=0;s<len;s++)buf[s]*=(1-mix*0.1F);}
}
/* ── Binaural Rendering ── */
static void binaural_render(audio_engine *eng, const float *mono_in, float az, float el, float dist, float gain) {
    hrtf_filter fl,fr;hrtf_select(eng,az,el,dist,&fl,&fr);
    float atten=calc_distance_atten(dist,MIN_DISTANCE,MAX_DISTANCE,1),g=gain*atten*eng->global_volume*eng->adaptive_gain;
    for(unsigned int i=0;i<BLOCK_SIZE;i++){eng->binaural.left[i]=mono_in[i]*g;eng->binaural.right[i]=mono_in[i]*g;}
    hrtf_apply(&fl,eng->binaural.left,eng->binaural.left,BLOCK_SIZE);
    hrtf_apply(&fr,eng->binaural.right,eng->binaural.right,BLOCK_SIZE);
}
/* ── Mix Bus ── */
static void mix_bus_init(mix_bus *bus, unsigned int ch, float sr) {
    bus->num_channels=ch<MAX_CHANNELS?ch:MAX_CHANNELS;bus->block_size=BLOCK_SIZE;bus->sample_rate=sr;
    for(unsigned int c=0;c<bus->num_channels;c++)for(unsigned int i=0;i<MAX_BLOCK_SIZE;i++)bus->buffer[c][i]=0;
}
static void mix_bus_add(mix_bus *bus, unsigned int ch, const float *data, unsigned int len) {
    if(ch>=bus->num_channels)return;unsigned int n=len<MAX_BLOCK_SIZE?len:MAX_BLOCK_SIZE;
    for(unsigned int i=0;i<n;i++)bus->buffer[ch][i]+=data[i];
}
static void mix_bus_render(mix_bus *bus, float *left, float *right, unsigned int len) {
    unsigned int n=len<MAX_BLOCK_SIZE?len:MAX_BLOCK_SIZE;
    for(unsigned int i=0;i<n;i++){left[i]=0;right[i]=0;}
    if(bus->num_channels>=2){for(unsigned int i=0;i<n;i++){left[i]=bus->buffer[0][i];right[i]=bus->buffer[1][i];}
        for(unsigned int c=2;c<bus->num_channels;c++){float pl=(c%2==0)?0.5F:0.3F,pr=(c%2==0)?0.3F:0.5F;
            for(unsigned int i=0;i<n;i++){left[i]+=bus->buffer[c][i]*pl;right[i]+=bus->buffer[c][i]*pr;}}}
    else if(bus->num_channels==1)for(unsigned int i=0;i<n;i++){left[i]=bus->buffer[0][i];right[i]=bus->buffer[0][i];}
    for(unsigned int c=0;c<bus->num_channels;c++)for(unsigned int i=0;i<MAX_BLOCK_SIZE;i++)bus->buffer[c][i]=0;
}
/* ── Adaptive Audio ── */
static void adaptive_head_track(audio_engine *eng, float yaw, float pitch_r, float roll) {
    eng->listen.head_rotation=rotation_matrix(pitch_r*DEG_TO_RAD,yaw*DEG_TO_RAD,roll*DEG_TO_RAD);eng->listen.head_tracking=1;
}
static void adaptive_scene_switch(audio_engine *eng, int scene) {
    eng->scene_mode=scene;
    if(scene==0x01){eng->global_volume=1;reverb_init(&eng->reverb,0.4F,0.5F,0.3F);}
    else if(scene==0x02){eng->global_volume=0.9F;reverb_init(&eng->reverb,0.3F,0.4F,0.4F);}
    else if(scene==0x03){eng->global_volume=0.85F;reverb_init(&eng->reverb,0.6F,0.7F,0.2F);}
    else if(scene==0x04){eng->global_volume=0.7F;reverb_init(&eng->reverb,0.1F,0.15F,0.6F);}
    else if(scene==0x05){eng->global_volume=0.95F;reverb_init(&eng->reverb,0.5F,0.6F,0.25F);}
}
static float adaptive_volume(audio_engine *eng, float input_level) {
    eng->adaptive_gain+=(clampf(input_level,0,1)-eng->adaptive_gain)*0.01F;return eng->adaptive_gain;
}
static void adaptive_personalize_hrtf(audio_engine *eng, float scale, float shift) {
    eng->listen.hrtf_scale=clampf(scale,0.5F,2);eng->listen.hrtf_shift=clampf(shift,-1,1);
    for(unsigned int i=0;i<MAX_HRTF_TAPS;i++){float t=(float)i/MAX_HRTF_TAPS,mod=scale*(1+shift*t);
        eng->custom_hrtf_left.coeffs[i]*=mod;eng->custom_hrtf_right.coeffs[i]*=mod;}
}
/* ── Engine Core ── */
static void engine_init(audio_engine *eng) {
    eng->num_sources=0;for(unsigned int i=0;i<MAX_SOURCES;i++)eng->sources[i].active=0;
    eng->listen.pos=vec3_make(0,0,0);eng->listen.vel=vec3_make(0,0,0);
    eng->listen.dir=vec3_make(0,0,-1);eng->listen.up=vec3_make(0,1,0);
    eng->listen.head_rotation=rotation_matrix(0,0,0);eng->listen.hrtf_scale=1;eng->listen.hrtf_shift=0;eng->listen.head_tracking=0;
    room_init(&eng->env_room,10,3,8,MAT_CONCRETE);reflection_init(&eng->refl);
    reverb_init(&eng->reverb,0.3F,0.4F,0.3F);compressor_init(&eng->comp,-12,4,5,50,6,2);
    eq_init(&eng->eq);effects_chain_init(&eng->fx);mix_bus_init(&eng->bus,MAX_CHANNELS,(float)SAMPLE_RATE);
    hrtf_init(eng);eng->scene_mode=0x02;eng->global_volume=0.9F;eng->adaptive_gain=1;eng->frame_count=0;eng->running=1;
    for(unsigned int i=0;i<BLOCK_SIZE;i++){eng->binaural.left[i]=0;eng->binaural.right[i]=0;}
}
static void engine_process_block(audio_engine *eng, const float *input, float *out_l, float *out_r, unsigned int len) {
    for(unsigned int s=0;s<eng->num_sources;s++){
        sound_source *src=&eng->sources[s];if(!src->active)continue;
        vec3 tl=vec3_sub(eng->listen.pos,src->pos);float dist=vec3_length(tl);spherical sph=cartesian_to_spherical(tl);
        if(eng->listen.head_tracking){vec3 rot=apply_rotation(eng->listen.head_rotation,tl);sph=cartesian_to_spherical(rot);}
        float atten=calc_distance_atten(dist,src->min_dist,src->max_dist,src->rolloff);
        float pitch=src->pitch;
        if(src->type==SRC_TYPE_POINT||src->type==SRC_TYPE_INTERACTIVE)
            pitch=calc_doppler_shift(src->pos,src->vel,eng->listen.pos,eng->listen.vel,src->pitch*src->doppler_scale);
        float dg=1;if(src->type==SRC_TYPE_DIRECTIONAL)
            dg=calc_directional_gain(src->dir,tl,src->inner_angle,src->outer_angle,src->outer_gain);
        binaural_render(eng,input,sph.azimuth,sph.elevation,dist,src->volume*atten*dg);
        mix_bus_add(&eng->bus,0,eng->binaural.left,len);mix_bus_add(&eng->bus,1,eng->binaural.right,len);
    }
    if(eng->env_room.num_walls>0)for(unsigned int s=0;s<eng->num_sources;s++)
        if(eng->sources[s].active)reflection_trace(&eng->refl,&eng->env_room,eng->sources[s].pos,eng->listen.pos);
    mix_bus_render(&eng->bus,out_l,out_r,len);
    float rvb[MAX_BLOCK_SIZE];for(unsigned int i=0;i<len&&i<MAX_BLOCK_SIZE;i++)rvb[i]=(out_l[i]+out_r[i])*0.5F;
    reverb_process(&eng->reverb,rvb,rvb,len);
    for(unsigned int i=0;i<len&&i<MAX_BLOCK_SIZE;i++){out_l[i]=out_l[i]*0.7F+rvb[i]*0.3F;out_r[i]=out_r[i]*0.7F+rvb[i]*0.3F;}
    float eqb[MAX_BLOCK_SIZE];for(unsigned int i=0;i<len&&i<MAX_BLOCK_SIZE;i++)eqb[i]=(out_l[i]+out_r[i])*0.5F;
    eq_process(&eng->eq,eqb,eqb,len);
    for(unsigned int i=0;i<len&&i<MAX_BLOCK_SIZE;i++){out_l[i]*=0.8F;out_r[i]*=0.8F;}
    float cm[MAX_BLOCK_SIZE];for(unsigned int i=0;i<len&&i<MAX_BLOCK_SIZE;i++)cm[i]=(out_l[i]+out_r[i])*0.5F;
    compressor_process(&eng->comp,cm,cm,len);
    for(unsigned int i=0;i<len&&i<MAX_BLOCK_SIZE;i++){out_l[i]=cm[i];out_r[i]=cm[i];}
    effects_chain_process(&eng->fx,out_l,len);effects_chain_process(&eng->fx,out_r,len);
    float peak=0;for(unsigned int i=0;i<len&&i<MAX_BLOCK_SIZE;i++){
        float l=out_l[i]<0?-out_l[i]:out_l[i],r=out_r[i]<0?-out_r[i]:out_r[i];if(l>peak)peak=l;if(r>peak)peak=r;}
    eng->adaptive_gain=adaptive_volume(eng,1-peak*0.1F);eng->frame_count++;
}
static void engine_shutdown(audio_engine *eng) {
    eng->running=0;eng->num_sources=0;eng->global_volume=0;
    for(unsigned int i=0;i<MAX_SOURCES;i++)eng->sources[i].active=0;
}
/* ── Exported API ── */
__attribute__((export_name("spatial_audio_init")))
unsigned int spatial_audio_init(void) {
    unsigned int ptr=host_alloc(sizeof(audio_engine),16);
    if(!ptr){host_print("spatial_audio: alloc failed\n");host_exit(1);return 0;}
    audio_engine *eng=(audio_engine *)(unsigned long)ptr;engine_init(eng);
    host_print("spatial_audio: engine initialized\n");
    host_print("  sources:64 channels:8 effects:16 reverb:128taps hrtf:binaural\n");
    host_print("  scenes: vr/game/film/conf/music\n");return ptr;
}
__attribute__((export_name("spatial_audio_add_source")))
int spatial_audio_add_source(unsigned int ep, int type, float x, float y, float z) {
    audio_engine *e=(audio_engine *)(unsigned long)ep;
    if(e->num_sources>=MAX_SOURCES)return -1;unsigned int i=e->num_sources;
    source_init(&e->sources[i],type,vec3_make(x,y,z));e->sources[i].id=i;e->num_sources++;return(int)i;
}
__attribute__((export_name("spatial_audio_set_source")))
void spatial_audio_set_source(unsigned int ep, unsigned int sid, float x, float y, float z, float vx, float vy, float vz, float vol) {
    audio_engine *e=(audio_engine *)(unsigned long)ep;if(sid>=e->num_sources)return;
    source_update(&e->sources[sid],vec3_make(x,y,z),vec3_make(vx,vy,vz),vol);
}
__attribute__((export_name("spatial_audio_set_listener")))
void spatial_audio_set_listener(unsigned int ep, float x, float y, float z, float dx, float dy, float dz) {
    audio_engine *e=(audio_engine *)(unsigned long)ep;e->listen.pos=vec3_make(x,y,z);e->listen.dir=vec3_normalize(vec3_make(dx,dy,dz));
}
__attribute__((export_name("spatial_audio_head_track")))
void spatial_audio_head_track(unsigned int ep, float yaw, float pr, float roll) {
    adaptive_head_track((audio_engine *)(unsigned long)ep,yaw,pr,roll);
}
__attribute__((export_name("spatial_audio_set_scene")))
void spatial_audio_set_scene(unsigned int ep, int scene) {
    adaptive_scene_switch((audio_engine *)(unsigned long)ep,scene);
}
__attribute__((export_name("spatial_audio_set_room")))
void spatial_audio_set_room(unsigned int ep, float w, float h, float d, int mat) {
    room_init(&((audio_engine *)(unsigned long)ep)->env_room,w,h,d,mat);
}
__attribute__((export_name("spatial_audio_personalize")))
void spatial_audio_personalize(unsigned int ep, float scale, float shift) {
    adaptive_personalize_hrtf((audio_engine *)(unsigned long)ep,scale,shift);
}
__attribute__((export_name("spatial_audio_set_reverb")))
void spatial_audio_set_reverb(unsigned int ep, float size, float decay, float damp) {
    reverb_init(&((audio_engine *)(unsigned long)ep)->reverb,size,decay,damp);
}
__attribute__((export_name("spatial_audio_set_eq")))
void spatial_audio_set_eq(unsigned int ep, unsigned int band, float freq, float gain, float q) {
    eq_set_band(&((audio_engine *)(unsigned long)ep)->eq,band,freq,gain,q);
}
__attribute__((export_name("spatial_audio_add_effect")))
void spatial_audio_add_effect(unsigned int ep, int type, float mix) {
    effects_chain_add(&((audio_engine *)(unsigned long)ep)->fx,type,mix);
}
__attribute__((export_name("spatial_audio_process")))
void spatial_audio_process(unsigned int ep, unsigned int inp, unsigned int olp, unsigned int orp, unsigned int len) {
    audio_engine *e=(audio_engine *)(unsigned long)ep;const float *input=(const float *)(unsigned long)inp;
    float *ol=(float *)(unsigned long)olp,*or2=(float *)(unsigned long)orp;
    unsigned int n=len<MAX_BLOCK_SIZE?len:MAX_BLOCK_SIZE;engine_process_block(e,input,ol,or2,n);
}
__attribute__((export_name("spatial_audio_shutdown")))
void spatial_audio_shutdown(unsigned int ep) {
    engine_shutdown((audio_engine *)(unsigned long)ep);host_print("spatial_audio: engine shut down\n");
}
/* ── Module Entry ── */
__attribute__((visibility("default")))
unsigned int __module_entry(unsigned int argc) {
    (void)argc;host_print("=== Spatial Audio Engine v1.0 ===\n");
    host_print("HRTF binaural | room acoustics | multi-source\n");
    host_print("VR/AR | Games | Film | Conference | Music\n");
    unsigned int ptr=spatial_audio_init();if(!ptr){host_print("spatial_audio: init failed\n");host_exit(1);return 1;}
    audio_engine *eng=(audio_engine *)(unsigned long)ptr;
    adaptive_scene_switch(eng,0x02);room_init(&eng->env_room,8,3,6,MAT_WOOD);
    spatial_audio_add_source(ptr,SRC_TYPE_POINT,2,1.5F,-3);
    spatial_audio_add_source(ptr,SRC_TYPE_DIRECTIONAL,-1,1,-2);
    spatial_audio_add_source(ptr,SRC_TYPE_AMBIENT,0,2,0);
    spatial_audio_add_source(ptr,SRC_TYPE_SPATIAL,3,0.5F,-1);
    spatial_audio_add_source(ptr,SRC_TYPE_INTERACTIVE,-2,1.2F,-4);
    effects_chain_add(&eng->fx,FX_TYPE_REVERB,0.3F);
    effects_chain_add(&eng->fx,FX_TYPE_COMPRESS,0.5F);
    effects_chain_add(&eng->fx,FX_TYPE_EQ,0.4F);
    host_print("spatial_audio: demo configured with 5 sources, ready\n");return ptr;
}
