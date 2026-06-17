/*
 * bci_neural: Neural Decoding Engine - AI-ASM OS (AArch64 WebAssembly-native)
 * Version: Neural Decoding v10.0 - Brain-Computer Interface Neural Processing
 * No standard library - uses only host imports and manual implementations.
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

/* Forward declarations */
static unsigned int my_strlen(const char *s);
static void my_strncpy(char *d, const char *s, unsigned int n);
static int my_strcmp(const char *a, const char *b);
static void print_str(const char *s);
static void print_int(int v);
static void print_double(double v);
static double my_abs(double x);
static double my_sqrt(double x);
static double my_sin(double x);
static double my_cos(double x);
static double my_exp(double x);
static double srand_val(void);

#define PI 3.14159265358979323846
#define MAX_CH     64
#define MAX_SAMP   256
#define MAX_FEAT   64
#define MAX_REG    32
#define MAX_EDGE   128
#define MAX_ST     16
#define MAX_TR     128
#define MAX_PHON   32
#define MAX_WORD   64
#define MAX_CAL    64
#define NAME_LEN   32
#define VERSION "bci_neural v10.0 - Neural Decoding Engine"

/* Mode constants */
#define MOTOR_INTENT     0
#define MOTOR_TRAJECTORY 1
#define MOTOR_VELOCITY   2
#define MOTOR_MULTIDOF   3
#define MOTOR_PROSTHETIC 4
#define SPEECH_SYNTH     0
#define SPEECH_INNER     1
#define SPEECH_ENCODE    2
#define SPEECH_RATE      3
#define SPEECH_RHYTHM    4
#define COG_ATTENTION    0
#define COG_WORKMEM      1
#define COG_DECISION     2
#define COG_LOAD         3
#define COG_FATIGUE      4
#define FB_REALTIME      0
#define FB_MODULATION    1
#define FB_BIOTHERAPY    2
#define FB_ATTENTION_TRN 3
#define NET_FUNCTIONAL   0
#define NET_EFFECTIVE    1
#define NET_TOPOLOGY     2
#define NET_DYNAMIC      3
#define NET_BRAINSTATE   4
#define ADAPT_CALIB      0
#define ADAPT_TRANSFER   1
#define ADAPT_ONLINE     2
#define ADAPT_PERF       3
#define ADAPT_LONGTERM   4

/* Structures */
typedef struct { int id,mode,channels,sample_rate; double threshold; int active; } motor_dec_t;
typedef struct { int x[MAX_TR],y[MAX_TR],vx[MAX_TR],vy[MAX_TR]; int length,dof; } trajectory_t;
typedef struct { int ids[MAX_PHON],conf[MAX_PHON]; int length,mode; } speech_dec_t;
typedef struct { char words[MAX_WORD][NAME_LEN]; int count,rate; } speech_out_t;
typedef struct { int mode; double attention,memory,decision,load,fatigue; } cog_state_t;
typedef struct { int mode; double baseline,target,current; int sessions,successes; } fb_state_t;
typedef struct { int id; char label[NAME_LEN]; double activity; int connected; } region_t;
typedef struct { int src,dst; double weight,delay; } edge_t;
typedef struct { int mode,regions,edges; double conn,cluster,efficiency; } net_analysis_t;
typedef struct { int id; double prob; int dur_ms,transitions; } bstate_t;
typedef struct { int mode; double calib[MAX_CAL]; int cal_n; double tw[MAX_FEAT]; int adapted; double perf; } adapt_t;
typedef struct { double w[MAX_FEAT]; int feat_n; double lr,err; } neural_model_t;

static motor_dec_t g_motor;
static trajectory_t g_traj;
static speech_dec_t g_speech;
static speech_out_t g_sout;
static cog_state_t g_cog;
static fb_state_t g_fb;
static net_analysis_t g_net;
static adapt_t g_adapt;
static region_t g_reg[MAX_REG];
static edge_t g_edges[MAX_EDGE];
static bstate_t g_states[MAX_ST];
static neural_model_t g_model;
static int g_reg_n=0, g_edge_n=0, g_state_n=0;

/* ── Utilities ── */
static unsigned int my_strlen(const char *s){unsigned int l=0;while(s[l])l++;return l;}
static void my_strncpy(char *d,const char *s,unsigned int n){
    unsigned int i=0;while(i<n-1&&s[i]){d[i]=s[i];i++;}d[i]='\0';
}
static int my_strcmp(const char *a,const char *b){
    while(*a&&*b){if(*a!=*b)return *a-*b;a++;b++;}return *a-*b;
}
static void print_str(const char *s){host_print(s);}
static void print_int(int v){
    char b[32];int i=0;
    if(v<0){host_print("-");v=-v;}
    if(v==0){host_print("0");return;}
    while(v>0&&i<31){b[i++]='0'+(v%10);v/=10;}
    char o[32];for(int j=0;j<i;j++)o[j]=b[i-1-j];o[i]='\0';host_print(o);
}
static void print_double(double v){
    if(v<0){host_print("-");v=-v;}
    int w=(int)v;double f=v-(double)w;print_int(w);host_print(".");
    for(int i=0;i<4;i++){f*=10.0;int d=(int)f;char c[2]={(char)('0'+d),'\0'};host_print(c);f-=(double)d;}
}
static double my_abs(double x){return x<0?-x:x;}
static double my_sqrt(double x){
    if(x<=0)return 0.0;double g=x/2.0;
    for(int i=0;i<30;i++)g=(g+x/g)/2.0;return g;
}
static double my_sin(double x){
    while(x>PI)x-=2.0*PI;while(x<-PI)x+=2.0*PI;
    double t=x,s=x;
    for(int i=1;i<=10;i++){t*=-x*x/((2.0*i)*(2.0*i+1.0));s+=t;}return s;
}
static double my_cos(double x){return my_sin(x+PI/2.0);}
static double my_exp(double x){
    if(x>10.0)return 22026.0;if(x<-10.0)return 0.0;
    double s=1.0,t=1.0;for(int i=1;i<=20;i++){t*=x/(double)i;s+=t;}return s;
}
static unsigned int lcg_st=54321;
static double srand_val(void){
    lcg_st=lcg_st*1103515245+12345;
    return(double)((lcg_st>>16)&0x7FFF)/32767.0;
}

/* ── 1. Motor Decoding ── */
static int motor_init(int mode,int ch,int rate){
    g_motor.id=1;g_motor.mode=mode;g_motor.channels=ch;
    g_motor.sample_rate=rate;g_motor.threshold=0.5;g_motor.active=1;
    const char *m[]={"INTENT","TRAJECTORY","VELOCITY","MULTIDOF","PROSTHETIC"};
    print_str("[MOTOR] Init: mode=");print_str(m[mode%5]);
    print_str(" ch=");print_int(ch);print_str(" rate=");print_int(rate);print_str("Hz\n");
    return 0;
}
static int motor_intent_decode(double feat[],int nf){
    double score=0.0;
    for(int i=0;i<nf&&i<MAX_FEAT;i++)score+=feat[i]*g_model.w[i];
    double prob=1.0/(1.0+my_exp(-score));
    const char *intents[]={"REACH","GRASP","RELEASE","PUSH","PULL"};
    int idx=(int)(prob*4.99);if(idx<0)idx=0;if(idx>4)idx=4;
    print_str("[MOTOR] Intent: ");print_str(intents[idx]);
    print_str(" prob=");print_double(prob);print_str("\n");
    return idx;
}
static int motor_trajectory_predict(double px,double py,double vx,double vy){
    g_traj.length=0;double cx=px,cy=py,cvx=vx,cvy=vy;
    for(int i=0;i<MAX_TR&&i<50;i++){
        g_traj.x[i]=(int)(cx*100.0);g_traj.y[i]=(int)(cy*100.0);
        g_traj.vx[i]=(int)(cvx*100.0);g_traj.vy[i]=(int)(cvy*100.0);
        g_traj.length++;cvx*=0.95;cvy*=0.95;
        cx+=cvx*0.01;cy+=cvy*0.01;
    }
    print_str("[MOTOR] Trajectory: ");print_int(g_traj.length);
    print_str(" pts end=(");print_int(g_traj.x[g_traj.length-1]);
    print_str(",");print_int(g_traj.y[g_traj.length-1]);print_str(")\n");
    return g_traj.length;
}
static double motor_velocity_decode(double sig[],int nch){
    double vx=0,vy=0,vz=0;
    for(int i=0;i<nch&&i<16;i++){
        double a=2.0*PI*(double)i/(double)nch;
        vx+=sig[i]*my_cos(a);vy+=sig[i]*my_sin(a);
        if(i<8)vz+=sig[i]*0.5;
    }
    double spd=my_sqrt(vx*vx+vy*vy+vz*vz);
    print_str("[MOTOR] Velocity: (");print_double(vx);print_str(",");
    print_double(vy);print_str(",");print_double(vz);
    print_str(") speed=");print_double(spd);print_str("\n");
    return spd;
}
static int motor_multidof(double sig[],int ndof){
    print_str("[MOTOR] Multi-DOF: ");print_int(ndof);print_str(" joints\n");
    for(int i=0;i<ndof&&i<7;i++){
        double a=sig[i]*180.0;if(a>180)a=180;if(a<-180)a=-180;
        print_str("  Joint ");print_int(i);print_str(": ");print_double(a);print_str(" deg\n");
    }
    return ndof;
}
static int motor_prosthetic(int grip,double force){
    const char *g[]={"POWER","PINCH","LATERAL","TRIPOD","HOOK"};
    print_str("[MOTOR] Prosthetic: grip=");print_str(g[grip%5]);
    print_str(" force=");print_double(force);print_str("N\n");
    g_traj.dof=5;return grip;
}

/* ── 2. Speech Decoding ── */
static int speech_init(int mode){
    g_speech.mode=mode;g_speech.length=0;
    const char *m[]={"SYNTHESIS","INNER","ENCODING","RATE","RHYTHM"};
    print_str("[SPEECH] Init: mode=");print_str(m[mode%5]);print_str("\n");return 0;
}
static int speech_phoneme_decode(double feat[],int nf){
    const char *ph[]={"AA","EE","IH","OH","UH","BA","DA","GA",
                      "PA","TA","KA","SA","FA","MA","NA","LA"};
    int np=0;
    for(int i=0;i<nf&&i<MAX_PHON&&np<16;i+=4){
        double e=0;for(int j=0;j<4&&(i+j)<nf;j++)e+=feat[i+j]*feat[i+j];
        e=my_sqrt(e);int pi=(int)(e*15.99);
        if(pi<0)pi=0;if(pi>15)pi=15;
        g_speech.ids[np]=pi;g_speech.conf[np]=(int)(e*100);np++;
    }
    g_speech.length=np;
    print_str("[SPEECH] Decoded ");print_int(np);print_str(" phonemes: ");
    for(int i=0;i<np&&i<6;i++){print_str(ph[g_speech.ids[i]]);if(i<np-1&&i<5)print_str("-");}
    if(np>6)print_str("...");print_str("\n");return np;
}
static int speech_inner_recognize(double sig[],int len){
    double e=0;for(int i=0;i<len&&i<MAX_SAMP;i++)e+=sig[i]*sig[i];
    e=my_sqrt(e/(double)len);
    const char *c[]={"QUESTION","STATEMENT","COMMAND","EMOTION","ABSTRACT"};
    int ci=(int)(e*4.99);if(ci<0)ci=0;if(ci>4)ci=4;
    print_str("[SPEECH] Inner: cat=");print_str(c[ci]);
    print_str(" conf=");print_double(e);print_str("\n");return ci;
}
static int speech_synthesize(int ids[],int np,int rate){
    print_str("[SPEECH] Synthesize: ");print_int(np);print_str(" ph at ");
    print_int(rate);print_str("BPM\n");
    g_sout.rate=rate;g_sout.count=0;int wl=0;
    for(int i=0;i<np&&g_sout.count<MAX_WORD;i++){
        wl++;if(wl>=3||i==np-1){
            my_strncpy(g_sout.words[g_sout.count],"word",NAME_LEN-1);
            g_sout.count++;wl=0;
        }
    }
    print_str("  Generated ");print_int(g_sout.count);print_str(" words\n");
    return g_sout.count;
}
static double speech_rate_ctrl(double cur,double target){
    double adj=(target-cur)*0.3,nr=cur+adj;
    print_str("[SPEECH] Rate: ");print_double(cur);print_str(" -> ");
    print_double(nr);print_str(" wpm\n");return nr;
}
static int speech_rhythm_ctrl(int pat[],int nb){
    print_str("[SPEECH] Rhythm: ");print_int(nb);print_str(" beats [");
    for(int i=0;i<nb&&i<8;i++){print_int(pat[i]);if(i<nb-1&&i<7)print_str(",");}
    print_str("]\n");return nb;
}

/* ── 3. Cognitive Decoding ── */
static int cog_init(int mode){
    g_cog.mode=mode;g_cog.attention=0;g_cog.memory=0;
    g_cog.decision=0;g_cog.load=0;g_cog.fatigue=0;
    const char *m[]={"ATTENTION","WORKMEM","DECISION","LOAD","FATIGUE"};
    print_str("[COG] Init: mode=");print_str(m[mode%5]);print_str("\n");return 0;
}
static double cog_attention(double alpha,double beta,double theta){
    double tot=alpha+beta+theta;if(tot<=0)tot=1.0;
    double att=(beta/tot)*100.0;if(att>100)att=100;
    g_cog.attention=att;
    print_str("[COG] Attention: ");print_double(att);
    print_str("% (a=");print_double(alpha);print_str(" b=");print_double(beta);
    print_str(" th=");print_double(theta);print_str(")\n");return att;
}
static double cog_working_memory(int items,double recall){
    double cap=(double)items/7.0;if(cap>1.0)cap=1.0;
    double ld=cap*(1.0-recall)*100.0;g_cog.memory=ld;
    print_str("[COG] WorkMem: ");print_int(items);print_str(" items recall=");
    print_double(recall*100.0);print_str("% load=");print_double(ld);print_str("%\n");
    return ld;
}
static int cog_decision_predict(double ev[],int nopt){
    int best=0;double bv=ev[0];
    for(int i=1;i<nopt&&i<10;i++)if(ev[i]>bv){bv=ev[i];best=i;}
    double tot=0;for(int i=0;i<nopt&&i<10;i++)tot+=ev[i];
    double conf=(tot>0)?(bv/tot)*100.0:0;g_cog.decision=conf;
    print_str("[COG] Decision: opt=");print_int(best);
    print_str(" conf=");print_double(conf);print_str("%\n");return best;
}
static double cog_load_assess(double frontal,double parietal){
    double ld=(frontal+parietal*0.7)/2.0;if(ld>100)ld=100;g_cog.load=ld;
    const char *lv="LOW";if(ld>70)lv="HIGH";else if(ld>40)lv="MEDIUM";
    print_str("[COG] Load: ");print_double(ld);print_str("% (");print_str(lv);print_str(")\n");
    return ld;
}
static double cog_fatigue_detect(double alpha_r,double blink){
    double f=alpha_r*60.0+blink*4.0;if(f>100)f=100;g_cog.fatigue=f;
    const char *s="ALERT";
    if(f>70)s="VERY_FATIGUED";else if(f>40)s="FATIGUED";else if(f>20)s="SLIGHTLY_TIRED";
    print_str("[COG] Fatigue: ");print_double(f);print_str("% (");print_str(s);print_str(")\n");
    return f;
}

/* ── 4. Neural Feedback ── */
static int fb_init(int mode,double base,double target){
    g_fb.mode=mode;g_fb.baseline=base;g_fb.target=target;
    g_fb.current=base;g_fb.sessions=0;g_fb.successes=0;
    const char *m[]={"REALTIME","MODULATION","BIOTHERAPY","ATTENTION_TRN"};
    print_str("[FB] Init: mode=");print_str(m[mode%4]);
    print_str(" base=");print_double(base);print_str(" target=");print_double(target);print_str("\n");
    return 0;
}
static double fb_realtime(double sig,double thr){
    double f=(sig>thr)?sig*1.1:sig*0.9;g_fb.current=f;
    int ok=(f>=g_fb.target)?1:0;g_fb.sessions++;g_fb.successes+=ok;
    print_str("[FB] Realtime: sig=");print_double(sig);
    print_str(" fb=");print_double(f);print_str(ok?" [OK]\n":" [BELOW]\n");return f;
}
static double fb_modulation(double in,double gain,double offset){
    double o=in*gain+offset;if(o>1)o=1;if(o<0)o=0;
    print_str("[FB] Modulation: in=");print_double(in);
    print_str(" gain=");print_double(gain);print_str(" out=");print_double(o);print_str("\n");
    return o;
}
static int fb_biotherapy(int session,double stress){
    double red=stress*0.15*(double)session,r=stress-red;if(r<0)r=0;
    print_str("[FB] Biotherapy s");print_int(session);
    print_str(": stress=");print_double(stress);print_str(" -> ");print_double(r);print_str("\n");
    return(int)(r*100);
}
static double fb_attention_training(double cur,int trial){
    double imp=0.05*(1.0-my_exp(-(double)trial/10.0));
    double na=cur+imp;if(na>100)na=100;
    print_str("[FB] AttentionTrn trial ");print_int(trial);
    print_str(": ");print_double(cur);print_str("% -> ");print_double(na);print_str("%\n");
    return na;
}

/* ── 5. Brain Network Analysis ── */
static int net_init_regions(int count){
    g_reg_n=0;
    const char *lb[]={"Prefrontal","Motor","SomatoSensory","Visual",
                      "Auditory","Temporal","Parietal","Occipital",
                      "Cingulate","Insula","Hippocampus","Amygdala",
                      "Thalamus","Cerebellum","BasalGanglia","BrainStem"};
    for(int i=0;i<count&&i<MAX_REG;i++){
        g_reg[i].id=i;my_strncpy(g_reg[i].label,lb[i%16],NAME_LEN-1);
        g_reg[i].activity=srand_val()*100.0;g_reg[i].connected=0;g_reg_n++;
    }
    print_str("[NET] Init ");print_int(count);print_str(" regions\n");return count;
}
static int net_functional_conn(double sigs[][MAX_SAMP],int nreg,int len){
    g_edge_n=0;int flen=(len>64)?64:len;
    for(int i=0;i<nreg&&i<MAX_REG;i++){
        for(int j=i+1;j<nreg&&j<MAX_REG;j++){
            double si=0,sj=0;
            for(int t=0;t<flen;t++){si+=sigs[i][t];sj+=sigs[j][t];}
            double mi=si/(double)flen,mj=sj/(double)flen;
            double corr=0,vi=0,vj=0;
            for(int t=0;t<flen;t++){
                double di=sigs[i][t]-mi,dj=sigs[j][t]-mj;
                corr+=di*dj;vi+=di*di;vj+=dj*dj;
            }
            double dn=my_sqrt(vi*vj),r=(dn>0)?corr/dn:0;
            if(my_abs(r)>0.3&&g_edge_n<MAX_EDGE){
                g_edges[g_edge_n].src=i;g_edges[g_edge_n].dst=j;
                g_edges[g_edge_n].weight=r;g_edges[g_edge_n].delay=srand_val()*20.0;
                g_reg[i].connected++;g_reg[j].connected++;g_edge_n++;
            }
        }
    }
    g_net.conn=(double)g_edge_n/(double)(nreg*(nreg-1)/2);
    print_str("[NET] Functional: ");print_int(g_edge_n);
    print_str(" edges density=");print_double(g_net.conn);print_str("\n");
    return g_edge_n;
}
static int net_effective_conn(int granger){
    print_str("[NET] Effective (Granger=");print_int(granger);print_str(")\n");
    int causal=0;
    for(int i=0;i<g_edge_n&&i<20;i++){
        double gc=srand_val()*2.0;
        if(gc>1.0){
            print_str("  ");print_str(g_reg[g_edges[i].src].label);print_str(" -> ");
            print_str(g_reg[g_edges[i].dst].label);print_str(" GC=");
            print_double(gc);print_str("\n");causal++;
        }
    }
    print_str("  Causal links: ");print_int(causal);print_str("\n");return causal;
}
static double net_topology(void){
    double avg=0;for(int i=0;i<g_reg_n;i++)avg+=(double)g_reg[i].connected;
    avg/=(double)g_reg_n;
    double cl=0;
    for(int i=0;i<g_edge_n;i++){
        int sh=0;
        for(int j=0;j<g_edge_n;j++){
            if(g_edges[j].src==g_edges[i].src&&g_edges[j].dst!=g_edges[i].dst)
                for(int k=0;k<g_edge_n;k++)
                    if(g_edges[k].src==g_edges[i].dst&&g_edges[k].dst==g_edges[j].dst)sh++;
        }
        if(sh>0)cl+=1.0;
    }
    cl=(g_edge_n>0)?cl/(double)g_edge_n:0;
    g_net.cluster=cl;g_net.efficiency=avg/(double)g_reg_n;
    print_str("[NET] Topology: deg=");print_double(avg);
    print_str(" cluster=");print_double(cl);
    print_str(" eff=");print_double(g_net.efficiency);print_str("\n");
    return cl;
}
static int net_dynamic_conn(int nwin){
    print_str("[NET] Dynamic: ");print_int(nwin);print_str(" windows\n");
    for(int w=0;w<nwin&&w<8;w++){
        double wc=g_net.conn*(0.8+srand_val()*0.4);
        print_str("  Win ");print_int(w);print_str(": density=");print_double(wc);print_str("\n");
    }
    return nwin;
}
static int net_brain_state(double feat[],int nf){
    g_state_n=0;double tot=0;
    for(int i=0;i<5&&i<nf;i++){
        g_states[i].id=i;g_states[i].prob=my_abs(feat[i]);
        g_states[i].dur_ms=(int)(srand_val()*500+100);
        g_states[i].transitions=(int)(srand_val()*10);
        tot+=g_states[i].prob;g_state_n++;
    }
    if(tot>0)for(int i=0;i<g_state_n;i++)g_states[i].prob/=tot;
    int dom=0;for(int i=1;i<g_state_n;i++)if(g_states[i].prob>g_states[dom].prob)dom=i;
    const char *st[]={"REST","TASK","DREAM","MEDITATE","FOCUS"};
    print_str("[NET] BrainState: ");print_str(st[dom%5]);
    print_str(" prob=");print_double(g_states[dom].prob);print_str("\n");
    return dom;
}

/* ── 6. Personalized Adaptation ── */
static int adapt_calibrate(double data[],int n){
    g_adapt.mode=ADAPT_CALIB;g_adapt.cal_n=0;
    for(int i=0;i<n&&i<MAX_CAL;i++){g_adapt.calib[i]=data[i];g_adapt.cal_n++;}
    print_str("[ADAPT] Calibrate: ");print_int(n);print_str(" samples\n");return n;
}
static int adapt_transfer(double sw[],int nw){
    g_adapt.mode=ADAPT_TRANSFER;
    for(int i=0;i<nw&&i<MAX_FEAT;i++)g_adapt.tw[i]=sw[i]*0.8;
    for(int i=0;i<MAX_FEAT;i++)g_model.w[i]=g_adapt.tw[i];
    g_model.feat_n=nw;
    print_str("[ADAPT] Transfer: ");print_int(nw);print_str(" weights\n");return nw;
}
static double adapt_online(double pred,double actual,double lr){
    double err=actual-pred;g_model.err=err;g_model.lr=lr;
    for(int i=0;i<g_model.feat_n&&i<MAX_FEAT;i++)g_model.w[i]+=lr*err*0.1;
    print_str("[ADAPT] Online: err=");print_double(err);
    print_str(" lr=");print_double(lr);print_str("\n");return err;
}
static double adapt_perf_optimize(int iters){
    double best=0;
    for(int i=0;i<iters&&i<20;i++){
        double sc=50.0+srand_val()*50.0;
        double la=g_model.lr*(1.0+0.1*(double)i);if(la>0.5)la=0.5;
        g_model.lr=la;if(sc>best)best=sc;
    }
    g_adapt.perf=best;
    print_str("[ADAPT] PerfOpt: ");print_int(iters);print_str(" iters best=");
    print_double(best);print_str(" lr=");print_double(g_model.lr);print_str("\n");
    return best;
}
static int adapt_longterm(int sid,double score){
    print_str("[ADAPT] LongTerm s");print_int(sid);
    print_str(": score=");print_double(score);print_str("\n");
    static double hist[16];static int hn=0;
    if(hn<16){hist[hn]=score;hn++;}
    if(hn>=2){
        double s=0;for(int i=0;i<hn;i++)s+=hist[i];
        print_str("  Avg: ");print_double(s/(double)hn);
        print_str(" over ");print_int(hn);print_str(" sessions\n");
    }
    return hn;
}

/* ── CLI ── */
static void print_help(void){
    print_str(VERSION);print_str("\n\nUsage: bci_neural [options]\n");
    print_str("  -h    Show help\n  -t    Run test/demo\n\n");
    print_str("Modules: Motor, Speech, Cognitive, Feedback,\n");
    print_str("  Network Analysis, Adaptation\n");
}
static void run_test(void){
    print_str("=== ");print_str(VERSION);print_str(" ===\n\n");
    /* Motor */
    print_str("--- Motor Decoding ---\n");
    motor_init(MOTOR_INTENT,32,512);
    double mf[16];for(int i=0;i<16;i++)mf[i]=srand_val()*2.0-1.0;
    motor_intent_decode(mf,16);
    motor_trajectory_predict(0,0,50,30);
    double ns[16];for(int i=0;i<16;i++)ns[i]=srand_val()*10.0;
    motor_velocity_decode(ns,16);
    double js[7];for(int i=0;i<7;i++)js[i]=srand_val();
    motor_multidof(js,7);motor_prosthetic(1,5.5);
    /* Speech */
    print_str("\n--- Speech Decoding ---\n");
    speech_init(SPEECH_SYNTH);
    double sf[32];for(int i=0;i<32;i++)sf[i]=srand_val()*5.0;
    speech_phoneme_decode(sf,32);
    double es[256];for(int i=0;i<256;i++)es[i]=srand_val()*3.0;
    speech_inner_recognize(es,256);
    speech_synthesize(g_speech.ids,g_speech.length,150);
    speech_rate_ctrl(120,150);
    int rp[8]={1,0,1,1,0,1,0,1};speech_rhythm_ctrl(rp,8);
    /* Cognitive */
    print_str("\n--- Cognitive Decoding ---\n");
    cog_init(COG_ATTENTION);
    cog_attention(15,25,10);cog_working_memory(5,0.85);
    double ev[4]={0.7,0.3,0.5,0.2};cog_decision_predict(ev,4);
    cog_load_assess(60,45);cog_fatigue_detect(0.4,12);
    /* Feedback */
    print_str("\n--- Neural Feedback ---\n");
    fb_init(FB_REALTIME,0.5,0.8);
    fb_realtime(0.7,0.6);fb_modulation(0.6,1.2,0.05);
    fb_biotherapy(1,75);fb_attention_training(60,5);
    /* Network */
    print_str("\n--- Brain Network Analysis ---\n");
    net_init_regions(12);
    double nsg[12][MAX_SAMP];
    for(int r=0;r<12;r++)for(int t=0;t<64;t++)
        nsg[r][t]=my_sin(2.0*PI*(double)(r+1)*(double)t/64.0)+srand_val()*0.5;
    net_functional_conn(nsg,12,64);net_effective_conn(1);
    net_topology();net_dynamic_conn(5);
    double stf[5]={0.2,0.5,0.1,0.05,0.15};net_brain_state(stf,5);
    /* Adaptation */
    print_str("\n--- Personalized Adaptation ---\n");
    double cd[32];for(int i=0;i<32;i++)cd[i]=srand_val()*10.0;
    adapt_calibrate(cd,32);
    double sw[32];for(int i=0;i<32;i++)sw[i]=srand_val()*2.0-1.0;
    adapt_transfer(sw,32);adapt_online(0.7,0.9,0.01);
    adapt_perf_optimize(10);adapt_longterm(1,78.5);adapt_longterm(2,82.3);
    print_str("\n=== ");print_str(VERSION);print_str(" Complete ===\n");
}

void _start(void){
    unsigned int buf=host_alloc(512,16);host_get_argv(buf,512);
    int help=0,test=0;unsigned int pos=0;char *ap=(char *)buf;
    while(pos<512&&ap[pos])pos++;pos++;
    while(pos<512&&ap[pos]){
        char *a=&ap[pos];
        if(my_strcmp(a,"-h")==0||my_strcmp(a,"--help")==0)help=1;
        else if(my_strcmp(a,"-t")==0||my_strcmp(a,"--test")==0)test=1;
        while(pos<512&&ap[pos])pos++;pos++;
    }
    if(help){print_help();return;}
    if(test){run_test();return;}
    print_str(VERSION);print_str("\nUse -h for help, -t for test\n");
}
