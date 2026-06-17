/* edge_inference_engine: Optimized edge inference engine (v1.0) */
#include <stddef.h>

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Quantization methods */
#define QUANT_INT8       0x01
#define QUANT_FP16       0x02
#define QUANT_DYNAMIC    0x04
#define QUANT_STATIC     0x08
#define QUANT_AWARE      0x10
/* Pruning methods */
#define PRUNE_STRUCTURED   0x01
#define PRUNE_UNSTRUCTURED 0x02
#define PRUNE_ITERATIVE    0x04
/* Distillation modes */
#define DISTILL_FEATURE    0x01
#define DISTILL_ATTENTION  0x02
#define DISTILL_LOGIT      0x04
/* Hardware targets */
#define HW_GPU_NVIDIA    0x01
#define HW_GPU_AMD       0x02
#define HW_NPU_EDGETPU   0x04
#define HW_NPU_COREML    0x08
#define HW_DSP           0x10
#define HW_CPU           0x20
/* Batch modes */
#define BATCH_DYNAMIC    0x01
#define BATCH_STATIC     0x02
#define BATCH_STREAMING  0x04
#define BATCH_CONTINUOUS 0x08
/* States */
#define MODEL_NONE       0
#define MODEL_LOADED     1
#define MODEL_OPTIMIZED  2
/* Limits */
#define MAX_MODELS   16
#define MAX_CACHE    8
#define MAX_LAYERS   32
#define MAX_BATCH    64
#define MAX_REQ      32
#define NAME_LEN     32
/* ======================== DATA STRUCTURES ======================== */
typedef struct {
    int layer_id; int method; int zero_point; int scale_x256;
    float min_val; float max_val; float acc_loss;
} quant_params_t;

typedef struct {
    int layer_id; int method; int sparsity_pct;
    float sensitivity; int pruned;
} prune_result_t;

typedef struct {
    int teacher_id; int student_id; int mode;
    float temperature; float alpha;
    float feature_loss; float attn_loss;
} distill_config_t;

typedef struct {
    int type; char name[NAME_LEN]; int mem_mb;
    int tflops_x100; int power_mw; int active; int ops;
} hw_accel_t;

typedef struct {
    int mode; int max_size; int cur_batch;
    int pending; int total; int avg_lat;
    int queue[MAX_BATCH];
} batch_opt_t;

typedef struct {
    int model_id; int version; int size_kb;
    int last_access; int hits; int valid;
} cache_entry_t;

typedef struct {
    int model_id; char name[NAME_LEN];
    int state; int quant; int sparsity; int hw;
    int batch_mode; int layers; int params_m;
    int lat_us; int qps; int acc;
    int cache_idx; int version; int inf_count;
} inf_model_t;

typedef struct {
    int req_id; int model_id; int priority;
    int status; int lat_us; int ts;
} inf_request_t;

/* ======================== GLOBAL STATE ======================== */
static inf_model_t   g_models[MAX_MODELS];
static cache_entry_t g_cache[MAX_CACHE];
static hw_accel_t    g_hw[6];
static batch_opt_t   g_batcher;
static inf_request_t g_reqs[MAX_REQ];
static int g_mcnt=0, g_rcnt=0, g_nid=1, g_clk=0, g_hcnt=0;

/* ======================= UTILITIES ======================= */
static void my_strncpy(char *d, const char *s, int n) {
    int i=0; while(i<n-1 && s[i]){d[i]=s[i]; i++;} d[i]='\0';
}
static int my_strcmp(const char *a, const char *b) {
    while(*a && *b){if(*a!=*b)return *a-*b; a++; b++;} return *a-*b;
}
static void print_str(const char *s){host_print(s);}
static void print_int(int v) {
    char buf[32]; int p=0;
    if(v<0){buf[p++]='-'; v=-v;}
    if(v==0){buf[p++]='0';}
    else{int t=v,d=0; while(t>0){d++;t/=10;} p+=d; buf[p]='\0'; p--;
        while(v>0){buf[p--]='0'+(v%10);v/=10;}}
    host_print(buf);
}

/* ===================== QUANTIZATION ENGINE ===================== */
static void quant_compute(int lid, float mn, float mx, int method, quant_params_t *o) {
    o->layer_id=lid; o->method=method; o->min_val=mn; o->max_val=mx;
    if(method==QUANT_INT8){
        float r=mx-mn; o->scale_x256=(int)(r*256.0F/255.0F);
        o->zero_point=(int)(-128.0F-mn*255.0F/r); o->acc_loss=0.5F+(float)lid*0.1F;
    }else if(method==QUANT_FP16){
        o->scale_x256=256; o->zero_point=0; o->acc_loss=0.1F+(float)lid*0.02F;
    }else{ o->scale_x256=128; o->zero_point=0; o->acc_loss=1.0F; }
}

static int quantize_model(int mid, int method) {
    int mi=-1;
    for(int i=0;i<g_mcnt;i++) if(g_models[i].model_id==mid){mi=i;break;}
    if(mi<0){print_str("[quant] ERROR: model not found\n"); return -1;}
    int L=g_models[mi].layers; if(L>MAX_LAYERS)L=MAX_LAYERS;
    float tl=0.0F;
    for(int l=0;l<L;l++){
        quant_params_t qp;
        quant_compute(l,-1.0F-(float)l*0.05F,1.0F+(float)l*0.05F,method,&qp);
        tl+=qp.acc_loss;
    }
    g_models[mi].quant=method;
    int lr=(method==QUANT_INT8)?45:25;
    g_models[mi].lat_us-=g_models[mi].lat_us*lr/100;
    if(g_models[mi].lat_us<100)g_models[mi].lat_us=100;
    int tg=(method==QUANT_INT8)?80:35;
    g_models[mi].qps+=g_models[mi].qps*tg/100;
    g_models[mi].state=MODEL_OPTIMIZED;
    const char *mn=(method==QUANT_INT8)?"INT8":(method==QUANT_FP16)?"FP16":"DYN";
    print_str("[quant] model="); print_str(g_models[mi].name);
    print_str(" method="); print_str(mn);
    print_str(" layers="); print_int(L);
    print_str(" avg_loss="); print_int((int)((tl/(float)L)*100.0F));
    print_str("bps lat="); print_int(g_models[mi].lat_us);
    print_str("us qps="); print_int(g_models[mi].qps); print_str("\n");
    return 0;
}

/* Static quantization with calibration data simulation */
static int quantize_static(int mid, int calib_samples) {
    int mi=-1;
    for(int i=0;i<g_mcnt;i++) if(g_models[i].model_id==mid){mi=i;break;}
    if(mi<0){print_str("[quant] ERROR: model not found\n");return -1;}
    int L=g_models[mi].layers; if(L>MAX_LAYERS)L=MAX_LAYERS;
    float calib_range=0.0F;
    for(int s=0;s<calib_samples;s++){
        float se=0.5F+(float)s*0.01F;
        if(se>calib_range)calib_range=se;
    }
    float total_loss=0.0F;
    for(int l=0;l<L;l++) total_loss+=calib_range*0.3F/(float)(l+1);
    g_models[mi].quant=QUANT_STATIC;
    g_models[mi].lat_us-=g_models[mi].lat_us*40/100;
    if(g_models[mi].lat_us<100)g_models[mi].lat_us=100;
    g_models[mi].qps+=g_models[mi].qps*70/100;
    g_models[mi].state=MODEL_OPTIMIZED;
    print_str("[quant] static model="); print_str(g_models[mi].name);
    print_str(" calib="); print_int(calib_samples);
    print_str(" L="); print_int(L);
    print_str(" range="); print_int((int)(calib_range*1000.0F));
    print_str("m lat="); print_int(g_models[mi].lat_us);
    print_str("us\n");
    return 0;
}

/* Quantization-aware training simulation */
static int quant_aware_train(int mid, int epochs) {
    int mi=-1;
    for(int i=0;i<g_mcnt;i++) if(g_models[i].model_id==mid){mi=i;break;}
    if(mi<0){print_str("[quant] ERROR: model not found\n");return -1;}
    float acc_start=(float)g_models[mi].acc, acc=acc_start;
    for(int e=0;e<epochs;e++)
        acc+=0.3F*(1.0F-(float)e/(float)epochs);
    g_models[mi].quant=QUANT_AWARE;
    g_models[mi].acc=(int)acc;
    g_models[mi].lat_us-=g_models[mi].lat_us*35/100;
    if(g_models[mi].lat_us<80)g_models[mi].lat_us=80;
    g_models[mi].qps+=g_models[mi].qps*60/100;
    g_models[mi].state=MODEL_OPTIMIZED;
    print_str("[quant] QAT model="); print_str(g_models[mi].name);
    print_str(" epochs="); print_int(epochs);
    print_str(" acc="); print_int((int)acc_start);
    print_str("->"); print_int(g_models[mi].acc);
    print_str("% lat="); print_int(g_models[mi].lat_us);
    print_str("us\n");
    return 0;
}

/* ====================== PRUNING ENGINE ====================== */
static float layer_sens(int lid, int tot) {
    return 0.1F+((float)lid/(float)tot)*0.8F;
}
static int prune_model(int mid, int method, int target) {
    int mi=-1;
    for(int i=0;i<g_mcnt;i++) if(g_models[i].model_id==mid){mi=i;break;}
    if(mi<0){print_str("[prune] ERROR: model not found\n"); return -1;}
    int L=g_models[mi].layers; if(L>MAX_LAYERS)L=MAX_LAYERS;
    int iters=(method&PRUNE_ITERATIVE)?5:1; if(iters>10)iters=10;
    float tp=(float)g_models[mi].params_m, rem=tp;
    for(int it=0;it<iters;it++){
        float is=(float)target/100.0F/(float)iters;
        for(int l=0;l<L;l++){
            float s=layer_sens(l,L);
            rem-=rem*(is*(1.0F-s*0.5F));
        }
    }
    float sp=(1.0F-rem/tp)*100.0F;
    g_models[mi].sparsity=(int)sp;
    int spd=(int)(sp*0.7F);
    g_models[mi].lat_us-=g_models[mi].lat_us*spd/100;
    if(g_models[mi].lat_us<50)g_models[mi].lat_us=50;
    g_models[mi].acc-=(int)(sp*0.05F);
    g_models[mi].state=MODEL_OPTIMIZED;
    const char *mn=(method&PRUNE_STRUCTURED)?"structured":"unstructured";
    print_str("[prune] model="); print_str(g_models[mi].name);
    print_str(" method="); print_str(mn);
    print_str(" sparsity="); print_int((int)sp);
    print_str("% remaining="); print_int((int)rem);
    print_str("M speedup="); print_int(spd); print_str("%\n");
    return 0;
}
static void sensitivity_analysis(int mid) {
    int mi=-1;
    for(int i=0;i<g_mcnt;i++) if(g_models[i].model_id==mid){mi=i;break;}
    if(mi<0)return;
    int L=g_models[mi].layers; if(L>MAX_LAYERS)L=MAX_LAYERS;
    print_str("[prune] sensitivity: "); print_str(g_models[mi].name); print_str("\n");
    for(int l=0;l<L;l++){
        print_str("  L"); print_int(l); print_str("=");
        print_int((int)(layer_sens(l,L)*1000.0F)); print_str("m\n");
    }
}

/* =================== KNOWLEDGE DISTILLATION =================== */
static int distill_model(int tid, int sid, int mode, float temp, float alpha) {
    int ti=-1,si=-1;
    for(int i=0;i<g_mcnt;i++){
        if(g_models[i].model_id==tid)ti=i;
        if(g_models[i].model_id==sid)si=i;
    }
    if(ti<0||si<0){print_str("[distill] ERROR: teacher/student not found\n"); return -1;}
    float fl=0.0F,al=0.0F,ll=0.0F;
    int tL=g_models[ti].layers, sL=g_models[si].layers;
    if(tL>MAX_LAYERS)tL=MAX_LAYERS; if(sL>MAX_LAYERS)sL=MAX_LAYERS;
    if(mode&DISTILL_FEATURE){
        int pairs=(sL<tL)?sL:tL;
        for(int l=0;l<pairs;l++){
            float d=(1.0F/(1.0F+(float)l*0.1F))-(1.0F/(1.0F+(float)l*0.15F));
            fl+=d*d*alpha;
        }
        fl/=(float)pairs;
    }
    if(mode&DISTILL_ATTENTION){
        al=(1.0F-(float)6/(float)12)*temp*0.1F;
    }
    if(mode&DISTILL_LOGIT){
        ll=(1.0F-1.0F/temp)*alpha*0.05F;
    }
    float total=fl+al+ll;
    int gain=(int)((1.0F-total)*5.0F); if(gain<0)gain=0;
    g_models[si].acc+=gain;
    if(g_models[si].acc>g_models[ti].acc-1)g_models[si].acc=g_models[ti].acc-1;
    g_models[si].state=MODEL_OPTIMIZED;
    print_str("[distill] T="); print_str(g_models[ti].name);
    print_str(" S="); print_str(g_models[si].name);
    print_str(" temp="); print_int((int)(temp*10.0F));
    print_str(" loss="); print_int((int)(total*10000.0F));
    print_str(" S_acc="); print_int(g_models[si].acc); print_str("%\n");
    return 0;
}

/* ================== HARDWARE ACCELERATION ================== */
static void hw_register(int type, const char *nm, int mem, int tf, int pw) {
    if(g_hcnt>=6)return;
    g_hw[g_hcnt].type=type; my_strncpy(g_hw[g_hcnt].name,nm,NAME_LEN-1);
    g_hw[g_hcnt].mem_mb=mem; g_hw[g_hcnt].tflops_x100=tf;
    g_hw[g_hcnt].power_mw=pw; g_hw[g_hcnt].active=0; g_hw[g_hcnt].ops=0;
    g_hcnt++;
}
static void hw_init(void) {
    hw_register(HW_GPU_NVIDIA,"GPU-NVIDIA-T4",16384,8100,70000);
    hw_register(HW_GPU_AMD,"GPU-AMD-MI100",32768,11500,250000);
    hw_register(HW_NPU_EDGETPU,"NPU-EdgeTPU",512,400,2000);
    hw_register(HW_NPU_COREML,"NPU-CoreML",1024,600,3000);
    hw_register(HW_DSP,"DSP-Hexagon",256,200,1500);
    hw_register(HW_CPU,"CPU-AARM64",65536,150,5000);
}
static int hw_select(int mid) {
    int mi=-1;
    for(int i=0;i<g_mcnt;i++) if(g_models[i].model_id==mid){mi=i;break;}
    if(mi<0)return -1;
    int best=-1,bs=0;
    for(int i=0;i<g_hcnt;i++){
        int sc=g_hw[i].tflops_x100;
        if(g_hw[i].mem_mb>=g_models[mi].params_m*4)sc+=100;
        if(g_hw[i].power_mw<15000)sc+=50;
        if(g_models[mi].hw&&g_models[mi].hw==g_hw[i].type)sc+=200;
        if(sc>bs){bs=sc;best=i;}
    }
    if(best>=0){
        g_hw[best].active=1; g_models[mi].hw=g_hw[best].type;
        int sp=g_hw[best].tflops_x100/100; if(sp<2)sp=2;
        g_models[mi].lat_us/=sp; if(g_models[mi].lat_us<10)g_models[mi].lat_us=10;
        g_models[mi].qps*=sp;
        print_str("[hw] "); print_str(g_hw[best].name);
        print_str("->"); print_str(g_models[mi].name);
        print_str(" x"); print_int(sp);
        print_str(" lat="); print_int(g_models[mi].lat_us);
        print_str("us\n");
    }
    return best;
}

/* Print status of all hardware accelerators */
static void hw_print_status(void) {
    print_str("[hw] accelerator inventory:\n");
    for(int i=0;i<g_hcnt;i++){
        print_str("  "); print_str(g_hw[i].name);
        print_str(" mem="); print_int(g_hw[i].mem_mb);
        print_str("MB tflops="); print_int(g_hw[i].tflops_x100);
        print_str("x0.01 pw="); print_int(g_hw[i].power_mw);
        print_str("mW active="); print_int(g_hw[i].active);
        print_str("\n");
    }
}

/* =================== BATCH OPTIMIZER =================== */
static void batch_init(int mode, int maxs) {
    g_batcher.mode=mode; g_batcher.max_size=maxs;
    g_batcher.cur_batch=0; g_batcher.pending=0;
    g_batcher.total=0; g_batcher.avg_lat=0;
    const char *mn=(mode==BATCH_DYNAMIC)?"dynamic":(mode==BATCH_STATIC)?"static":
                   (mode==BATCH_STREAMING)?"streaming":"continuous";
    print_str("[batch] init mode="); print_str(mn);
    print_str(" max="); print_int(maxs); print_str("\n");
}
static int batch_enqueue(int mid, int prio) {
    if(g_rcnt>=MAX_REQ){print_str("[batch] queue full\n"); return -1;}
    g_reqs[g_rcnt].req_id=g_nid; g_reqs[g_rcnt].model_id=mid;
    g_reqs[g_rcnt].priority=prio; g_reqs[g_rcnt].status=0;
    g_reqs[g_rcnt].ts=g_clk; g_rcnt++; g_batcher.pending++;
    print_str("[batch] enq req="); print_int(g_nid);
    print_str(" m="); print_int(mid); print_str(" p="); print_int(prio);
    print_str(" pend="); print_int(g_batcher.pending); print_str("\n");
    return g_nid++;
}
static int batch_process(void) {
    /* priority sort */
    for(int i=0;i<g_rcnt-1;i++)
        for(int j=i+1;j<g_rcnt;j++)
            if(g_reqs[j].priority>g_reqs[i].priority&&g_reqs[i].status==0&&g_reqs[j].status==0){
                inf_request_t t=g_reqs[i]; g_reqs[i]=g_reqs[j]; g_reqs[j]=t;}
    int proc=0,bc=0,tl=0;
    for(int i=0;i<g_rcnt&&bc<g_batcher.max_size;i++){
        if(g_reqs[i].status!=0)continue;
        int mi=-1;
        for(int m=0;m<g_mcnt;m++)if(g_models[m].model_id==g_reqs[i].model_id){mi=m;break;}
        if(mi<0){g_reqs[i].status=3;continue;}
        g_reqs[i].status=1; g_reqs[i].lat_us=g_models[mi].lat_us; tl+=g_reqs[i].lat_us;
        g_models[mi].inf_count++; g_clk+=g_reqs[i].lat_us/1000;
        g_reqs[i].status=2; proc++; bc++;
    }
    if(proc>0){
        int na=tl/proc;
        g_batcher.avg_lat=(g_batcher.avg_lat==0)?na:(g_batcher.avg_lat+na)/2;
    }
    g_batcher.total+=proc; g_batcher.pending-=proc;
    if(g_batcher.pending<0)g_batcher.pending=0;
    print_str("[batch] done="); print_int(proc);
    print_str(" total="); print_int(g_batcher.total);
    print_str(" avg="); print_int(g_batcher.avg_lat);
    print_str("us pend="); print_int(g_batcher.pending); print_str("\n");
    return proc;
}

/* ==================== MODEL CACHE (LRU) ==================== */
static int cache_find(int mid) {
    for(int i=0;i<MAX_CACHE;i++)if(g_cache[i].valid&&g_cache[i].model_id==mid)return i;
    return -1;
}
static int cache_evict(void) {
    int li=-1,ol=0x7FFFFFFF;
    for(int i=0;i<MAX_CACHE;i++){if(!g_cache[i].valid)return i;
        if(g_cache[i].last_access<ol){ol=g_cache[i].last_access;li=i;}}
    if(li>=0){print_str("[cache] evict m=");print_int(g_cache[li].model_id);
        print_str(" v=");print_int(g_cache[li].version);print_str("\n");
        g_cache[li].valid=0;}
    return li;
}
static int cache_insert(int mid, int ver, int skb) {
    int idx=cache_find(mid);
    if(idx>=0){g_cache[idx].last_access=g_clk;g_cache[idx].hits++;
        g_cache[idx].version=ver;
        print_str("[cache] hit m=");print_int(mid);print_str(" hits=");
        print_int(g_cache[idx].hits);print_str("\n");return idx;}
    idx=cache_evict(); if(idx<0)return -1;
    g_cache[idx].model_id=mid;g_cache[idx].version=ver;g_cache[idx].size_kb=skb;
    g_cache[idx].last_access=g_clk;g_cache[idx].hits=1;g_cache[idx].valid=1;
    print_str("[cache] ins m=");print_int(mid);print_str(" v=");print_int(ver);
    print_str(" sz=");print_int(skb);print_str("KB\n");
    return idx;
}
static void cache_preload(int ids[], int cnt) {
    print_str("[cache] preload ");print_int(cnt);print_str(" models\n");
    for(int i=0;i<cnt;i++)for(int m=0;m<g_mcnt;m++)
        if(g_models[m].model_id==ids[i]){cache_insert(ids[i],g_models[m].version,
            g_models[m].params_m*4);break;}
}
static void cache_hot_update(int mid, int nver) {
    int idx=cache_find(mid);
    if(idx<0){print_str("[cache] hot_update insert m=");print_int(mid);
        print_str(" v=");print_int(nver);print_str("\n");
        for(int m=0;m<g_mcnt;m++)if(g_models[m].model_id==mid){
            g_models[m].version=nver;cache_insert(mid,nver,g_models[m].params_m*4);return;}
        return;}
    int ov=g_cache[idx].version; g_cache[idx].version=nver;g_cache[idx].last_access=g_clk;
    for(int m=0;m<g_mcnt;m++)if(g_models[m].model_id==mid){g_models[m].version=nver;break;}
    print_str("[cache] hot_update m=");print_int(mid);
    print_str(" v");print_int(ov);print_str("->v");print_int(nver);print_str("\n");
}

/* ===================== MODEL MANAGEMENT ===================== */
static int register_model(const char *nm, int lay, int pm, int lat, int acc) {
    if(g_mcnt>=MAX_MODELS){print_str("[engine] max models\n");return -1;}
    int i=g_mcnt; g_models[i].model_id=g_nid;
    my_strncpy(g_models[i].name,nm,NAME_LEN-1);
    g_models[i].state=MODEL_LOADED; g_models[i].quant=0; g_models[i].sparsity=0;
    g_models[i].hw=0; g_models[i].batch_mode=0; g_models[i].layers=lay;
    g_models[i].params_m=pm; g_models[i].lat_us=lat;
    g_models[i].qps=1000000/lat; g_models[i].acc=acc;
    g_models[i].cache_idx=-1; g_models[i].version=1; g_models[i].inf_count=0;
    g_mcnt++; int id=g_nid++;
    cache_insert(id,1,pm*4);
    print_str("[engine] reg ");print_str(nm);print_str(" id=");print_int(id);
    print_str(" L=");print_int(lay);print_str(" P=");print_int(pm);
    print_str("M acc=");print_int(acc);print_str("%\n");
    return id;
}

/* ======================== MAIN ======================== */
void main(void) {
    unsigned int buf=host_alloc(512,16);
    host_get_argv(buf,512);
    int demo=0,help=0; unsigned int pos=0; char *ap=(char*)buf;
    while(pos<512&&ap[pos])pos++; pos++;
    while(pos<512&&ap[pos]){char*a=&ap[pos];
        if(my_strcmp(a,"--help")==0)help=1;
        else if(my_strcmp(a,"--demo")==0)demo=1;
        while(pos<512&&ap[pos])pos++; pos++;}

    print_str("=== Edge Inference Engine v1.0 ===\n");
    if(help){print_str("Usage: edge_inference_engine [--demo|--help]\n");return;}
    if(!demo){print_str("Use --demo to run the full pipeline\n");return;}

    /* Phase 1: Hardware init */
    print_str("\n--- Phase 1: Hardware Init ---\n");
    hw_init();
    hw_print_status();

    /* Phase 2: Register models */
    print_str("\n--- Phase 2: Register Models ---\n");
    int tid=register_model("resnet50-teacher",53,25,8000,92);
    int sid=register_model("mobilenet-student",28,4,2500,78);
    int did=register_model("yolo-nano",15,7,4000,85);
    int nid=register_model("bert-tiny",12,14,6000,80);

    /* Phase 3: Quantization */
    print_str("\n--- Phase 3: Quantization ---\n");
    quantize_model(tid,QUANT_INT8);
    quantize_model(sid,QUANT_FP16);
    quantize_model(did,QUANT_INT8);
    quantize_static(nid,128);
    quant_aware_train(sid,10);

    /* Phase 4: Pruning */
    print_str("\n--- Phase 4: Pruning ---\n");
    sensitivity_analysis(did);
    prune_model(did,PRUNE_STRUCTURED|PRUNE_ITERATIVE,50);
    prune_model(nid,PRUNE_UNSTRUCTURED,30);

    /* Phase 5: Knowledge Distillation */
    print_str("\n--- Phase 5: Knowledge Distillation ---\n");
    distill_model(tid,sid,DISTILL_FEATURE|DISTILL_ATTENTION|DISTILL_LOGIT,4.0F,0.7F);

    /* Phase 6: HW Acceleration */
    print_str("\n--- Phase 6: HW Acceleration ---\n");
    hw_select(tid); hw_select(did); hw_select(nid);

    /* Phase 7: Batch Optimization */
    print_str("\n--- Phase 7: Batch Optimization ---\n");
    batch_init(BATCH_DYNAMIC,8);
    batch_enqueue(tid,1); batch_enqueue(sid,3); batch_enqueue(did,5); batch_enqueue(nid,2);
    batch_enqueue(tid,4); batch_enqueue(did,2); batch_enqueue(sid,1); batch_enqueue(nid,5);
    batch_process();
    batch_enqueue(did,3); batch_enqueue(tid,2);
    batch_process();

    /* Phase 8: Cache Management */
    print_str("\n--- Phase 8: Cache Management ---\n");
    int pids[3]; pids[0]=tid; pids[1]=did; pids[2]=nid;
    cache_preload(pids,3);
    cache_hot_update(sid,2);
    cache_hot_update(did,2);

    /* Final stats */
    print_str("\n--- Model Stats ---\n");
    for(int i=0;i<g_mcnt;i++){
        print_str("  "); print_str(g_models[i].name);
        print_str(": st="); print_int(g_models[i].state);
        print_str(" lat="); print_int(g_models[i].lat_us);
        print_str("us qps="); print_int(g_models[i].qps);
        print_str(" acc="); print_int(g_models[i].acc);
        print_str("% inf="); print_int(g_models[i].inf_count);
        print_str(" v="); print_int(g_models[i].version); print_str("\n");
    }
    print_str("\n--- Batch Stats ---\n");
    print_str("  total="); print_int(g_batcher.total);
    print_str(" avg="); print_int(g_batcher.avg_lat);
    print_str("us pend="); print_int(g_batcher.pending); print_str("\n");
    print_str("\n--- Cache Stats ---\n");
    for(int i=0;i<MAX_CACHE;i++)if(g_cache[i].valid){
        print_str("  ["); print_int(i); print_str("] m=");
        print_int(g_cache[i].model_id); print_str(" v=");
        print_int(g_cache[i].version); print_str(" sz=");
        print_int(g_cache[i].size_kb); print_str("KB h=");
        print_int(g_cache[i].hits); print_str("\n");
    }
    print_str("\n=== Edge Inference Engine Complete ===\n");
}
