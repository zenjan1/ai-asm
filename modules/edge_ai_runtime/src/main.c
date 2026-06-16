/* edge_ai_runtime: Edge AI Inference Runtime (v56.0)
 * Optimized inference engine for AArch64 WebAssembly-native OS
 * Features: quantization, operator fusion, heterogeneous dispatch,
 *           dynamic batching, model hot-update, A/B testing */
#include <stddef.h>

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_DIMS 4
#define MAX_NODES 32
#define MAX_EDGES 64
#define MAX_DEVS 8
#define MAX_CAPS 16
#define MAX_QUEUE 64
#define MAX_VER 8
#define MAX_MET 128
#define MAX_PATH 48
#define MAX_NAME 24
#define MAX_OPS 8

/* Quantization modes */
#define Q_INT8_SYM 0
#define Q_INT8_ASYM 1
#define Q_FP16 2

/* Calibration methods */
#define CAL_MINMAX 0
#define CAL_PCT 1
#define CAL_KL 2

/* Data types */
#define DT_FP32 0
#define DT_FP16 1
#define DT_INT8 2

/* Operator types */
#define OP_CONV 0
#define OP_BN 1
#define OP_RELU 2
#define OP_MM 3
#define OP_ADD 4
#define OP_POOL 5
#define OP_SM 6
#define OP_FUSED 7

/* Device types */
#define DEV_CPU 0
#define DEV_GPU 1
#define DEV_NPU 2

/* Scheduler modes */
#define SCH_RR 0
#define SCH_LB 1
#define SCH_PREF 2

/* Model status */
#define MS_LOADING 0
#define MS_ACTIVE 1
#define MS_STANDBY 2
#define MS_ERROR 3

/* A/B split modes */
#define SPLIT_PCT 0
#define SPLIT_HASH 1

typedef struct { int mode; int cal; } quant_config_t;

typedef struct {
    int shape[MAX_DIMS]; int dtype; float *data; float scale; int zp; int numel;
} tensor_t;

typedef struct {
    int type; int in[MAX_OPS]; int out[MAX_OPS]; int nin; int nout;
    float params[8]; int fused; char name[MAX_NAME];
} op_node_t;

typedef struct { int from; int to; } edge_t;

typedef struct {
    op_node_t nodes[MAX_NODES]; edge_t edges[MAX_EDGES]; int nn; int ne;
} fusion_graph_t;

typedef struct {
    int id; int type; int caps[MAX_CAPS]; int ncaps;
    int mem_mb; int util; int tops; int tlat; int tpow;
} device_t;

typedef struct { device_t devs[MAX_DEVS]; int nd; int sched; int rr; } dispatch_ctx_t;

typedef struct { int maxbs; int tout; int qcap; } batch_config_t;

typedef struct { int rid; int mid; float *inp; int isz; int et; int st; } inf_req_t;

typedef struct {
    inf_req_t rq[MAX_QUEUE]; int cnt; int cap; int hd; int tl;
    batch_config_t cfg; int tsrv; int tw;
} req_queue_t;

typedef struct { int M; int m; int p; char path[MAX_PATH]; int lt; int st; } model_ver_t;

typedef struct { model_ver_t v[MAX_VER]; int vc; int act; int prv; } model_reg_t;

typedef struct { int var; int lat; float acc; int succ; } ab_metric_t;

typedef struct { int sm; int sp; int win; } ab_config_t;

typedef struct {
    char name[MAX_NAME]; int ma; int mb; ab_config_t cfg;
    ab_metric_t am[MAX_MET]; ab_metric_t bm[MAX_MET]; int ca; int cb;
} ab_test_t;

/* ── Helper utilities ────────────────────────────────────────────── */
static int __attribute__((unused)) my_strlen(const char *s) { int l=0; while(s[l]) l++; return l; }
static void my_strncpy(char *d, const char *s, int n) {
    int i=0; while(i<n-1&&s[i]){d[i]=s[i];i++;} d[i]='\0';
}
static void print_str(const char *s) { host_print(s); }
static void print_int(int v) {
    char b[32]; int p=0;
    if(v<0){b[p++]='-';v=-v;} if(v==0)b[p++]='0';
    else{int d=0,t=v;while(t>0){d++;t/=10;}p+=d;b[p]='\0';p--;
        while(v>0){b[p--]='0'+(v%10);v/=10;}}
    host_print(b);
}
static void print_double(double v) {
    int w=(int)v; double f=v-(double)w; if(f<0)f=-f;
    int fr=(int)(f*10000.0); print_int(w); print_str(".");
    if(!fr){print_str("0000");return;}
    char fb[8]; int fp=4; fb[fp]='\0';
    while(fr>0&&fp>0){fb[--fp]='0'+(fr%10);fr/=10;}
    while(fp>0)fb[--fp]='0'; print_str(fb);
}
static double my_abs(double x) { return x<0?-x:x; }
static double my_sqrt(double x) {
    if(x<=0)return 0; double g=x*0.5;
    for(int i=0;i<40;i++) g=(g+x/g)*0.5; return g;
}

/* ── 1. Model Quantization ───────────────────────────────────────── */
static void quant_calibrate(tensor_t *t, float *s, int ns, int meth) {
    if(ns<=0)return;
    float mn=s[0],mx=s[0];
    for(int i=1;i<ns;i++){if(s[i]<mn)mn=s[i];if(s[i]>mx)mx=s[i];}
    if(meth==CAL_PCT&&ns>4){float r=mx-mn;mn+=r*0.01f;mx-=r*0.01f;}
    if(meth==CAL_KL&&ns>2){float r=mx-mn;mn+=r*0.005f;mx-=r*0.005f;}
    float sc=(mx-mn)/255.0f; if(sc<1e-6f)sc=1e-6f;
    int zp=(int)(-mn/sc); if(zp<-128)zp=-128; if(zp>127)zp=127;
    t->scale=sc; t->zp=zp;
}
static void quant_apply(tensor_t *t, quant_config_t *c) {
    if(!t||!t->data)return;
    if(c->mode==Q_INT8_SYM||c->mode==Q_INT8_ASYM){
        t->dtype=DT_INT8;
        for(int i=0;i<t->numel;i++){
            int q=(int)(t->data[i]/t->scale)+t->zp;
            if(q<-128)q=-128;if(q>127)q=127; t->data[i]=(float)q;
        }
    } else if(c->mode==Q_FP16){
        t->dtype=DT_FP16;
        for(int i=0;i<t->numel;i++){
            float v=t->data[i]; int b; __builtin_memcpy(&b,&v,4);
            b&=0xFFFF0000; __builtin_memcpy(&v,&b,4); t->data[i]=v;
        }
    }
}
static void quant_dequantize(tensor_t *t) {
    if(!t||!t->data)return;
    for(int i=0;i<t->numel;i++) t->data[i]=(t->data[i]-(float)t->zp)*t->scale;
    t->dtype=DT_FP32;
}
static void quant_compute_error(tensor_t *o, tensor_t *q) {
    if(!o||!q)return;
    double ms=0,sg=0;
    for(int i=0;i<o->numel;i++){
        double d=(double)o->data[i]-(double)q->data[i];
        ms+=d*d; sg+=(double)o->data[i]*(double)o->data[i];
    }
    double mse=ms/(double)o->numel;
    double snr=mse>1e-7?10.0*my_sqrt(sg/mse):0;
    print_str("  Quant err: MSE="); print_double(mse);
    print_str(" SNR="); print_double(snr); print_str("\n");
}

/* ── 2. Operator Fusion ──────────────────────────────────────────── */
static fusion_graph_t *fusion_create_graph(void) {
    fusion_graph_t *g=(fusion_graph_t *)host_alloc(sizeof(fusion_graph_t),8);
    g->nn=0; g->ne=0; return g;
}
static int fusion_add_node(fusion_graph_t *g, int type, const char *name) {
    if(g->nn>=MAX_NODES)return -1;
    int i=g->nn; g->nodes[i].type=type; g->nodes[i].fused=0;
    g->nodes[i].nin=0; g->nodes[i].nout=0;
    for(int j=0;j<8;j++) g->nodes[i].params[j]=0;
    my_strncpy(g->nodes[i].name,name,MAX_NAME-1); g->nn++; return i;
}
static void fusion_add_edge(fusion_graph_t *g, int f, int t) {
    if(g->ne>=MAX_EDGES)return;
    g->edges[g->ne].from=f; g->edges[g->ne].to=t; g->ne++;
}
static int fusion_detect_patterns(fusion_graph_t *g) {
    int pf=0;
    for(int i=0;i<g->ne;i++){
        int a=g->edges[i].from, b=g->edges[i].to;
        if(g->nodes[a].type==OP_CONV&&g->nodes[b].type==OP_BN){
            for(int j=0;j<g->ne;j++)
                if(g->edges[j].from==b&&g->nodes[g->edges[j].to].type==OP_RELU){
                    g->nodes[a].fused=g->nodes[b].fused=g->nodes[g->edges[j].to].fused=1;
                    print_str("  Pattern: Conv+BN+ReLU fused\n"); pf++;
                }
        }
        if(g->nodes[a].type==OP_MM&&g->nodes[b].type==OP_ADD){
            g->nodes[a].fused=g->nodes[b].fused=1;
            print_str("  Pattern: MatMul+Add fused\n"); pf++;
        }
    }
    return pf;
}
static int fusion_apply(fusion_graph_t *g) {
    int fc=0,w=0;
    for(int i=0;i<g->nn;i++){
        if(g->nodes[i].fused)fc++;
        else{if(w!=i)g->nodes[w]=g->nodes[i];w++;}
    }
    for(int i=0;i<g->nn;i++) if(g->nodes[i].fused){
        g->nodes[w].type=OP_FUSED;g->nodes[w].fused=0;
        my_strncpy(g->nodes[w].name,"fused",MAX_NAME-1);w++;break;
    }
    g->nn=w; return fc;
}
static void fusion_optimize(fusion_graph_t *g) {
    print_str("  Detecting patterns...\n");
    int p=fusion_detect_patterns(g);
    print_str("  Found: "); print_int(p); print_str("\n");
    if(p>0){int f=fusion_apply(g);
        print_str("  Fused: ");print_int(f);print_str(" Remaining: ");
        print_int(g->nn);print_str("\n");}
}

/* ── 3. Heterogeneous Dispatch ───────────────────────────────────── */
static dispatch_ctx_t *dispatch_init(void) {
    dispatch_ctx_t *c=(dispatch_ctx_t *)host_alloc(sizeof(dispatch_ctx_t),8);
    c->nd=0;c->sched=SCH_LB;c->rr=0;
    c->devs[0]=(device_t){0,DEV_CPU,{OP_CONV,OP_MM},2,8192,35,0,0,0};
    c->devs[1]=(device_t){1,DEV_GPU,{OP_CONV,OP_MM,OP_POOL,OP_SM},4,4096,60,0,0,0};
    c->devs[2]=(device_t){2,DEV_NPU,{OP_CONV,OP_BN,OP_RELU},3,2048,20,0,0,0};
    c->nd=3; return c;
}
static dispatch_ctx_t *dispatch_create_context(void) { return dispatch_init(); }
static const char *dev_str(int t) {
    if(t==DEV_CPU)return "CPU";if(t==DEV_GPU)return "GPU";
    if(t==DEV_NPU)return "NPU";return "?";
}
static int dispatch_select_device(dispatch_ctx_t *c, int op) {
    if(!c->nd)return -1;
    if(c->sched==SCH_RR){int i=c->rr;c->rr=(c->rr+1)%c->nd;return i;}
    int best=-1,bu=101;
    for(int i=0;i<c->nd;i++){
        if(c->devs[i].util>=95)continue; int sup=0;
        for(int j=0;j<c->devs[i].ncaps;j++)
            if(c->devs[i].caps[j]==op){sup=1;break;}
        if(sup&&c->devs[i].util<bu){best=i;bu=c->devs[i].util;}
    }
    return best<0?0:best;
}
static int dispatch_execute(dispatch_ctx_t *c, int ni, int di) {
    if(di<0||di>=c->nd)return -1;
    device_t *d=&c->devs[di];
    int bl=10;if(d->type==DEV_GPU)bl=5;if(d->type==DEV_NPU)bl=3;
    int lat=bl+(d->util/10),pw=500;
    if(d->type==DEV_GPU)pw=2000;if(d->type==DEV_NPU)pw=300;
    d->tops++;d->tlat+=lat;d->tpow+=pw;(void)ni;return lat;
}
static void dispatch_update_stats(dispatch_ctx_t *c, int d, int lat, int pw) {
    if(d<0||d>=c->nd)return; c->devs[d].tlat+=lat;c->devs[d].tpow+=pw;c->devs[d].tops++;
}

/* ── 4. Dynamic Batching ─────────────────────────────────────────── */
static req_queue_t *batch_queue_init(batch_config_t *cfg) {
    req_queue_t *q=(req_queue_t *)host_alloc(sizeof(req_queue_t),8);
    q->cnt=0;q->cap=cfg->qcap;q->hd=0;q->tl=0;
    q->tsrv=0;q->tw=0;q->cfg=*cfg;return q;
}
static int batch_enqueue(req_queue_t *q, inf_req_t *r) {
    if(q->cnt>=q->cap)return -1;
    q->rq[q->tl]=*r;q->rq[q->tl].st=0;
    q->tl=(q->tl+1)%q->cap;q->cnt++;return 0;
}
static int batch_dequeue_batch(req_queue_t *q, int *bi, int mb) {
    int col=0,lim=q->cnt<mb?q->cnt:mb,idx=q->hd;
    for(int i=0;i<lim&&col<q->cfg.maxbs;i++){
        if(q->rq[idx].st==0){bi[col]=idx;q->rq[idx].st=1;col++;}
        idx=(idx+1)%q->cap;}
    return col;
}
static int batch_process_batch(req_queue_t *q, int *bi, int bs) {
    if(bs<=0)return 0; int sl=8+bs*2;
    for(int i=0;i<bs;i++){q->rq[bi[i]].st=2;q->tw+=sl;q->tsrv++;}
    while(q->cnt>0&&q->rq[q->hd].st==2){q->hd=(q->hd+1)%q->cap;q->cnt--;}
    return sl;
}
static void batch_compute_stats(req_queue_t *q) {
    print_str("  Queue: pending=");print_int(q->cnt);
    print_str(" served=");print_int(q->tsrv);
    if(q->tsrv>0){print_str(" avg=");print_int(q->tw/q->tsrv);
        print_str("ms tput=");
        print_double((double)q->tsrv*1000.0/(double)(q->tw>0?q->tw:1));
        print_str(" req/s");}
    print_str("\n");
}

/* ── 5. Model Hot Update ─────────────────────────────────────────── */
static model_reg_t *registry_init(void) {
    model_reg_t *r=(model_reg_t *)host_alloc(sizeof(model_reg_t),8);
    r->vc=0;r->act=-1;r->prv=-1;return r;
}
static int registry_load_version(model_reg_t *r, int M, int m, int p, const char *path) {
    if(r->vc>=MAX_VER)return -1; int i=r->vc;
    r->v[i].M=M;r->v[i].m=m;r->v[i].p=p;
    my_strncpy(r->v[i].path,path,MAX_PATH-1);
    r->v[i].lt=120+i*15;r->v[i].st=MS_STANDBY;r->vc++;
    print_str("  Loaded v");print_int(M);print_str(".");print_int(m);
    print_str(".");print_int(p);print_str(" (");print_int(r->v[i].lt);
    print_str("ms)\n");return i;
}
static int registry_activate(model_reg_t *r, int vi) {
    if(vi<0||vi>=r->vc)return -1;
    if(r->act>=0)r->v[r->act].st=MS_STANDBY;
    r->v[vi].st=MS_ACTIVE;r->prv=r->act;r->act=vi;
    print_str("  Active v");print_int(r->v[vi].M);print_str(".");
    print_int(r->v[vi].m);print_str(".");print_int(r->v[vi].p);print_str("\n");
    return 0;
}
static int registry_rollback(model_reg_t *r) {
    if(r->prv<0){print_str("  No prev version\n");return -1;}
    print_str("  Rollback...\n");return registry_activate(r,r->prv);
}
static void registry_list(model_reg_t *r) {
    for(int i=0;i<r->vc;i++){
        print_str("  [");print_int(i);print_str("] v");
        print_int(r->v[i].M);print_str(".");print_int(r->v[i].m);
        print_str(".");print_int(r->v[i].p);print_str(" ");
        const char *s=r->v[i].st==MS_ACTIVE?"ACTIVE":
            r->v[i].st==MS_STANDBY?"STANDBY":
            r->v[i].st==MS_LOADING?"LOADING":"ERROR";
        print_str(s);print_str("\n");
    }
}

/* ── 6. A/B Testing ──────────────────────────────────────────────── */
static ab_test_t *ab_create(const char *nm, int ma, int mb, ab_config_t *cfg) {
    ab_test_t *t=(ab_test_t *)host_alloc(sizeof(ab_test_t),8);
    my_strncpy(t->name,nm,MAX_NAME-1);t->ma=ma;t->mb=mb;
    t->cfg=*cfg;t->ca=0;t->cb=0;return t;
}
static int ab_assign(ab_test_t *t, int uid) {
    int b=t->cfg.sm==SPLIT_PCT?uid%100:(((uid*2654435761u)>>16)&0x7FFFFFFF)%100;
    return b<t->cfg.sp?0:1;
}
static void ab_record(ab_test_t *t, int v, int lat, float acc, int succ) {
    ab_metric_t m={v,lat,acc,succ};
    if(v==0&&t->ca<MAX_MET)t->am[t->ca++]=m;
    else if(v==1&&t->cb<MAX_MET)t->bm[t->cb++]=m;
}
static void ab_significance(ab_test_t *t) {
    double ma=0,mb=0;
    if(t->ca>0){for(int i=0;i<t->ca;i++)ma+=(double)t->am[i].acc;ma/=(double)t->ca;}
    if(t->cb>0){for(int i=0;i<t->cb;i++)mb+=(double)t->bm[i].acc;mb/=(double)t->cb;}
    double va=0,vb=0;
    if(t->ca>1){for(int i=0;i<t->ca;i++){double d=(double)t->am[i].acc-ma;va+=d*d;}va/=(double)(t->ca-1);}
    if(t->cb>1){for(int i=0;i<t->cb;i++){double d=(double)t->bm[i].acc-mb;vb+=d*d;}vb/=(double)(t->cb-1);}
    double den=0;if(t->ca>0)den+=va/(double)t->ca;if(t->cb>0)den+=vb/(double)t->cb;
    double ts=den>1e-7?(ma-mb)/my_sqrt(den):0;
    int sig=my_abs(ts)>1.96;
    print_str("  Mean acc A=");print_double(ma);print_str(" B=");print_double(mb);
    print_str("\n  t=");print_double(ts);
    print_str(sig?" (significant)\n":" (not significant)\n");
}
static void ab_report(ab_test_t *t) {
    print_str("  A/B: ");print_str(t->name);print_str(" A=");
    print_int(t->ca);print_str(" B=");print_int(t->cb);print_str("\n");
    int la=0,lb=0,sa=0,sb=0;
    if(t->ca>0){for(int i=0;i<t->ca;i++){la+=t->am[i].lat;sa+=t->am[i].succ;}la/=t->ca;}
    if(t->cb>0){for(int i=0;i<t->cb;i++){lb+=t->bm[i].lat;sb+=t->bm[i].succ;}lb/=t->cb;}
    print_str("  Lat A=");print_int(la);print_str("ms B=");print_int(lb);print_str("ms\n");
    print_str("  Succ A=");print_int(sa);print_str("/");print_int(t->ca);
    print_str(" B=");print_int(sb);print_str("/");print_int(t->cb);print_str("\n");
    ab_significance(t);
    double aa=0,ab2=0;
    if(t->ca>0){for(int i=0;i<t->ca;i++)aa+=(double)t->am[i].acc;aa/=(double)t->ca;}
    if(t->cb>0){for(int i=0;i<t->cb;i++)ab2+=(double)t->bm[i].acc;ab2/=(double)t->cb;}
    print_str(aa>ab2?"  => Deploy A\n":ab2>aa?"  => Deploy B\n":"  => Inconclusive\n");
}

/* ── Test subsystems ─────────────────────────────────────────────── */
static void test_quantization(void) {
    print_str("\n--- Quantization ---\n");
    float *m1=(float *)host_alloc(16*sizeof(float),16);
    tensor_t orig={0};orig.dtype=DT_FP32;orig.data=m1;orig.numel=16;
    for(int i=0;i<16;i++)orig.data[i]=(float)(i*10-80)*0.01f;
    float samp[8];for(int i=0;i<8;i++)samp[i]=(float)(i*10-40)*0.01f;
    quant_config_t cfg={Q_INT8_SYM,CAL_MINMAX};
    quant_calibrate(&orig,samp,8,CAL_MINMAX);
    print_str("  Scale=");print_double((double)orig.scale);
    print_str(" ZP=");print_int(orig.zp);print_str("\n");
    float *m2=(float *)host_alloc(16*sizeof(float),16);
    tensor_t bk={0};bk.data=m2;bk.numel=16;
    for(int i=0;i<16;i++)bk.data[i]=orig.data[i];
    quant_apply(&orig,&cfg);print_str("  Quantized INT8\n");
    quant_dequantize(&orig);print_str("  Dequant FP32\n");
    quant_compute_error(&bk,&orig);
}
static void test_fusion(void) {
    print_str("\n--- Operator Fusion ---\n");
    fusion_graph_t *g=fusion_create_graph();
    int n0=fusion_add_node(g,OP_CONV,"conv"),n1=fusion_add_node(g,OP_BN,"bn");
    int n2=fusion_add_node(g,OP_RELU,"relu"),n3=fusion_add_node(g,OP_MM,"mm");
    int n4=fusion_add_node(g,OP_ADD,"add"),n5=fusion_add_node(g,OP_SM,"sm");
    fusion_add_edge(g,n0,n1);fusion_add_edge(g,n1,n2);
    fusion_add_edge(g,n2,n3);fusion_add_edge(g,n3,n4);fusion_add_edge(g,n4,n5);
    print_str("  Graph: ");print_int(g->nn);print_str(" nodes ");
    print_int(g->ne);print_str(" edges\n");fusion_optimize(g);
}
static void test_dispatch(void) {
    print_str("\n--- Heterogeneous Dispatch ---\n");
    dispatch_ctx_t *c=dispatch_create_context();
    print_str("  Devices: ");print_int(c->nd);print_str("\n");
    for(int i=0;i<c->nd;i++){
        print_str("    [");print_int(i);print_str("] ");
        print_str(dev_str(c->devs[i].type));print_str(" ");
        print_int(c->devs[i].mem_mb);print_str("MB ");
        print_int(c->devs[i].util);print_str("%\n");}
    int ops[]={OP_CONV,OP_MM,OP_RELU,OP_SM};
    for(int i=0;i<4;i++){
        int d=dispatch_select_device(c,ops[i]);
        int l=dispatch_execute(c,i,d);
        print_str("  Op ");print_int(ops[i]);print_str("->");
        print_str(dev_str(c->devs[d].type));print_str(" lat=");
        print_int(l);print_str("ms\n");}
    dispatch_update_stats(c,0,15,800);
}
static void test_batching(void) {
    print_str("\n--- Dynamic Batching ---\n");
    batch_config_t bc={4,50,MAX_QUEUE};
    req_queue_t *q=batch_queue_init(&bc);
    for(int i=0;i<7;i++){
        inf_req_t r={1000+i,1,0,224,i*5,0};batch_enqueue(q,&r);}
    print_str("  Enqueued 7\n");
    int bi[MAX_QUEUE];
    int bs=batch_dequeue_batch(q,bi,4);
    print_str("  B1 sz=");print_int(bs);print_str(" lat=");
    print_int(batch_process_batch(q,bi,bs));print_str("ms\n");
    bs=batch_dequeue_batch(q,bi,4);
    print_str("  B2 sz=");print_int(bs);print_str(" lat=");
    print_int(batch_process_batch(q,bi,bs));print_str("ms\n");
    batch_compute_stats(q);
}
static void test_hot_update(void) {
    print_str("\n--- Model Hot Update ---\n");
    model_reg_t *r=registry_init();
    int v0=registry_load_version(r,1,0,0,"/models/resnet_v1.onnx");
    int v1=registry_load_version(r,1,1,0,"/models/resnet_v1.1.onnx");
    int v2=registry_load_version(r,2,0,0,"/models/resnet_v2.onnx");
    registry_activate(r,v0);print_str("  Versions:\n");registry_list(r);
    print_str("  Switch v2\n");registry_activate(r,v2);registry_list(r);
    registry_rollback(r);registry_list(r);(void)v1;
}
static void test_ab(void) {
    print_str("\n--- A/B Testing ---\n");
    ab_config_t ac={SPLIT_PCT,50,60000};
    ab_test_t *t=ab_create("resnet_test",1,2,&ac);
    for(int u=0;u<40;u++){
        int v=ab_assign(t,u);ab_record(t,v,v==0?12:8,v==0?0.92f:0.87f,u%10?1:0);}
    print_str("  A=");print_int(t->ca);print_str(" B=");print_int(t->cb);print_str("\n");
    ab_report(t);
}

/* ── 7. Entry point ──────────────────────────────────────────────── */
void _start(void) {
    unsigned int buf=host_alloc(512,16);
    host_get_argv(buf,512);
    int help=0,test=0,pos=0; char *ap=(char *)buf;
    while(pos<512&&ap[pos])pos++;pos++;
    while(pos<512&&ap[pos]){
        char *a=&ap[pos];
        if((a[0]=='-'&&a[1]=='h'&&!a[2])||(a[0]=='-'&&a[1]=='-'&&a[2]=='h'
           &&a[3]=='e'&&a[4]=='l'&&a[5]=='p'&&!a[6]))help=1;
        if((a[0]=='-'&&a[1]=='t'&&!a[2])||(a[0]=='-'&&a[1]=='-'&&a[2]=='t'
           &&a[3]=='e'&&a[4]=='s'&&a[5]=='t'&&!a[6]))test=1;
        while(pos<512&&ap[pos])pos++;pos++;}
    print_str("Edge AI Runtime v56.0 - Optimized Inference Engine\n");
    if(help){print_str("Usage: edge_ai_runtime [-h|-t]\n  -h  help\n  -t  test\n");return;}
    if(test){
        print_str("=== Edge AI Runtime Test ===\n");
        test_quantization();test_fusion();test_dispatch();
        test_batching();test_hot_update();test_ab();
        print_str("\n=== All Tests Complete ===\n");return;}
    print_str("Use -h for help, -t for test\n");
}
