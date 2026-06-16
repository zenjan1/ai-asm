/*
 * Brain-Computer Interface Module - AI-ASM OS (AArch64 WebAssembly-native)
 * Version: Brain-Computer Interface v55.0 - Neural Signal Processing
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

#define PI 3.14159265358979323846
#define MAX_CH 64
#define MAX_SAMP 2048
#define BUF_SZ 256
#define VERSION "Brain-Computer Interface v55.0 - Neural Signal Processing"

static unsigned int my_strlen(const char *s) { unsigned int l=0; while(s[l])l++; return l; }
static void my_strncpy(char *d, const char *s, unsigned int n) {
    unsigned int i=0; while(i<n&&s[i]){d[i]=s[i];i++;} while(i<n){d[i]='\0';i++;}
}
static void print_str(const char *s) { host_print(s); }
static void print_int(int v) {
    char b[16]; int i=0;
    if(v<0){host_print("-");v=-v;} if(v==0){host_print("0");return;}
    while(v>0&&i<15){b[i++]='0'+(v%10);v/=10;}
    char o[16]; for(int j=0;j<i;j++)o[j]=b[i-1-j]; o[i]='\0'; host_print(o);
}
static void print_double(double v) {
    if(v<0){host_print("-");v=-v;}
    int w=(int)v; double f=v-(double)w; print_int(w); host_print(".");
    for(int i=0;i<4;i++){f*=10.0;int d=(int)f;char c[2]={(char)('0'+d),'\0'};host_print(c);f-=(double)d;}
}
static double my_abs(double x){return x<0?-x:x;}
static double my_sqrt(double x){
    if(x<=0)return 0.0; double g=x/2.0;
    for(int i=0;i<30;i++)g=(g+x/g)/2.0; return g;
}
static double my_sin(double x){
    while(x>PI)x-=2.0*PI; while(x<-PI)x+=2.0*PI;
    double t=x,s=x;
    for(int i=1;i<=10;i++){t*=-x*x/((2.0*i)*(2.0*i+1.0));s+=t;} return s;
}
static double my_cos(double x){return my_sin(x+PI/2.0);}
static unsigned int lcg_st=12345;
static double srand_val(void){
    lcg_st=lcg_st*1103515245+12345;
    return(double)((lcg_st>>16)&0x7FFF)/32767.0;
}

typedef struct { int id; char label[8]; double buf[MAX_SAMP]; double impedance; int active,n; } eeg_channel_t;
typedef struct { int num_ch,sample_rate,ref_type; } eeg_config_t;
typedef struct { int channel; double csp[MAX_CH],fvec[MAX_CH]; int n_feat; } mi_features_t;
typedef struct { int paradigm,stim_dur_ms,isi_ms; double target_prob; } p300_config_t;
typedef struct { int mode,protocol,fb_type; double baseline; } nf_state_t;
typedef struct { int type,difficulty,trials; double score; } cog_task_t;
typedef struct {
    double mean,variance,skewness,kurtosis,h_act,h_mob,h_comp;
} time_feat_t;
typedef struct {
    double delta,theta,alpha,beta,gamma,total;
} freq_feat_t;

static eeg_config_t g_cfg={16,256,0};
static eeg_channel_t g_ch[MAX_CH];
static int g_n_active=0;
static nf_state_t g_nf={0,0,0,1.0};

/* ── 1. EEG Signal Acquisition ── */
static int eeg_init(eeg_config_t cfg) {
    g_cfg=cfg; g_n_active=cfg.num_ch;
    const char *lab[]={"Fp1","Fp2","F3","F4","C3","C4","P3","P4",
        "O1","O2","F7","F8","T3","T4","T5","T6",
        "Fz","Cz","Pz","Oz","A1","A2","FCz","CPz"};
    for(int i=0;i<cfg.num_ch&&i<MAX_CH;i++){
        g_ch[i].id=i; my_strncpy(g_ch[i].label,lab[i%24],7);
        g_ch[i].impedance=2000.0+srand_val()*3000.0;
        g_ch[i].active=1; g_ch[i].n=0;
    }
    print_str("[EEG] Initialized "); print_int(cfg.num_ch);
    print_str(" channels at "); print_int(cfg.sample_rate); print_str("Hz\n");
    return 0;
}
static int eeg_set_rate(int r){
    if(r!=256&&r!=512&&r!=1024)return -1;
    g_cfg.sample_rate=r;
    print_str("[EEG] Rate set to "); print_int(r); print_str("Hz\n"); return 0;
}
static int eeg_check_impedance(double ch[],int n,double thr){
    int good=0;
    for(int i=0;i<n;i++)if(ch[i]<thr)good++;
    print_str("[EEG] Impedance: "); print_int(good); print_str("/"); print_int(n);
    print_str(" channels < "); print_double(thr); print_str(" ohm\n"); return good;
}
static double gen_wave(double freq,double t,double amp,double ph){
    return amp*my_sin(2.0*PI*freq*t+ph);
}
static void eeg_simulate(eeg_channel_t ch[],int nch,int dur_ms){
    int ns=(g_cfg.sample_rate*dur_ms)/1000;
    if(ns>MAX_SAMP)ns=MAX_SAMP;
    for(int c=0;c<nch;c++){
        double ph=srand_val()*2.0*PI;
        for(int i=0;i<ns;i++){
            double t=(double)i/(double)g_cfg.sample_rate;
            ch[c].buf[i]=gen_wave(10.0,t,20.0,ph)+gen_wave(20.0,t,8.0,ph*1.3)
                +gen_wave(40.0,t,3.0,ph*0.7)+(srand_val()-0.5)*4.0;
        }
        ch[c].n=ns;
    }
    print_str("[EEG] Simulated "); print_int(dur_ms); print_str("ms, ");
    print_int(nch); print_str(" channels\n");
}

/* ── 2. Signal Preprocessing ── */
static void filter_bandpass(double d[],int len,double lo,double hi,int sr){
    double rc_l=1.0/(2.0*PI*lo),dt=1.0/(double)sr,a_l=rc_l/(rc_l+dt);
    double rc_h=1.0/(2.0*PI*hi),a_h=dt/(rc_h+dt);
    double pi2=d[0],po2=d[0];
    for(int i=0;i<len;i++){double o=a_h*(po2+d[i]-pi2);pi2=d[i];po2=o;d[i]=o;}
    double p=d[0];
    for(int i=0;i<len;i++){double o=p+a_l*(d[i]-p);p=o;d[i]=o;}
}
static void filter_notch(double d[],int len,double freq,int sr){
    double w0=2.0*PI*freq/(double)sr,bw=2.0/(double)sr,r=1.0-3.0*bw;
    if(r<0.0)r=0.0;
    double d1=2.0*r*my_cos(w0),d2=r*r,y1=0,y2=0;
    for(int i=0;i<len;i++){
        double y0=d[i]+d1*y1-d2*y2;
        d[i]=y0-2.0*my_cos(w0)*y1+y2; y2=y1; y1=y0;
    }
}
static void art_eog(double d[],double ref[],int len){
    double sxy=0,sxx=0;
    for(int i=0;i<len;i++){sxy+=d[i]*ref[i];sxx+=ref[i]*ref[i];}
    double b=(sxx>0)?sxy/sxx:0;
    for(int i=0;i<len;i++)d[i]-=b*ref[i];
}
static int art_emg(double d[],int len,double thr){
    int rm=0; for(int i=0;i<len;i++)if(my_abs(d[i])>thr){d[i]=0;rm++;} return rm;
}
static void ref_standardize(double d[],int nch,int len,int meth){
    for(int s=0;s<len;s++){
        if(meth==0){
            double a=0; for(int c=0;c<nch;c++)a+=d[c*MAX_SAMP+s]; a/=(double)nch;
            for(int c=0;c<nch;c++)d[c*MAX_SAMP+s]-=a;
        } else {
            double r=(d[(nch-2)*MAX_SAMP+s]+d[(nch-1)*MAX_SAMP+s])/2.0;
            for(int c=0;c<nch-2;c++)d[c*MAX_SAMP+s]-=r;
        }
    }
}

/* ── 3. Feature Extraction ── */
static time_feat_t time_features(double d[],int len){
    time_feat_t f; double s=0;
    for(int i=0;i<len;i++)s+=d[i];
    f.mean=(len>0)?s/(double)len:0;
    double m2=0,m3=0,m4=0;
    for(int i=0;i<len;i++){double e=d[i]-f.mean;m2+=e*e;m3+=e*e*e;m4+=e*e*e*e;}
    f.variance=(len>1)?m2/(double)(len-1):0;
    double std=my_sqrt(f.variance);
    f.skewness=(len>2&&std>0)?(m3/(double)len)/(std*std*std):0;
    f.kurtosis=(len>3&&f.variance>0)?(m4/(double)len)/(f.variance*f.variance)-3.0:0;
    double act=0; for(int i=0;i<len;i++)act+=d[i]*d[i];
    f.h_act=act/(double)len;
    double mn=0; for(int i=1;i<len;i++){double e=d[i]-d[i-1];mn+=e*e;}
    f.h_mob=(act>0)?my_sqrt(mn/(double)(len-1)/f.h_act):0;
    double md=0; for(int i=2;i<len;i++){double e=d[i]-2.0*d[i-1]+d[i-2];md+=e*e;}
    double mob_d1=(mn>0)?my_sqrt(md/(double)(len-2)/(mn/(double)(len-1))):0;
    f.h_comp=(f.h_mob>0)?mob_d1/f.h_mob:0;
    return f;
}
static void fft(double re[],double im[],int n){
    for(int i=1,j=0;i<n;i++){
        int b=n>>1; while(j&b){j^=b;b>>=1;} j^=b;
        if(i<j){double t=re[i];re[i]=re[j];re[j]=t;t=im[i];im[i]=im[j];im[j]=t;}
    }
    for(int l=2;l<=n;l<<=1){
        double a=-2.0*PI/(double)l,wr=my_cos(a),wi=my_sin(a);
        for(int i=0;i<n;i+=l){
            double cr=1.0,ci=0;
            for(int j=0;j<l/2;j++){
                double ur=re[i+j],ui=im[i+j];
                double vr=re[i+j+l/2]*cr-im[i+j+l/2]*ci;
                double vi=re[i+j+l/2]*ci+im[i+j+l/2]*cr;
                re[i+j]=ur+vr;im[i+j]=ui+vi;
                re[i+j+l/2]=ur-vr;im[i+j+l/2]=ui-vi;
                double nr=cr*wr-ci*wi; ci=cr*wi+ci*wr; cr=nr;
            }
        }
    }
}
static freq_feat_t freq_features(double d[],int len,int sr){
    freq_feat_t f={0}; int n=1;
    while(n<len&&n<MAX_SAMP)n<<=1;
    double re[MAX_SAMP],im[MAX_SAMP];
    for(int i=0;i<n;i++){re[i]=(i<len)?d[i]:0;im[i]=0;}
    fft(re,im,n);
    double fr=(double)sr/(double)n;
    for(int i=1;i<n/2;i++){
        double p=re[i]*re[i]+im[i]*im[i],fq=(double)i*fr;
        f.total+=p;
        if(fq>=0.5&&fq<4.0)f.delta+=p;
        else if(fq>=4.0&&fq<8.0)f.theta+=p;
        else if(fq>=8.0&&fq<13.0)f.alpha+=p;
        else if(fq>=13.0&&fq<30.0)f.beta+=p;
        else if(fq>=30.0&&fq<50.0)f.gamma+=p;
    }
    return f;
}
static void wavelet_dwt(double d[],int len,double out[]){
    double h0[]={0.48296,0.83652,0.22414,-0.12941};
    double h1[]={-0.12941,-0.22414,0.83652,-0.48296};
    int cl=len; double tmp[MAX_SAMP];
    for(int i=0;i<len;i++)tmp[i]=d[i];
    for(int lv=0;lv<4&&cl>4;lv++){
        int h=cl/2;
        for(int i=0;i<h;i++){
            double a=0,dd=0;
            for(int k=0;k<4&&(2*i+k)<cl;k++){a+=tmp[2*i+k]*h0[k];dd+=tmp[2*i+k]*h1[k];}
            out[lv*MAX_SAMP/4+i]=a;
            out[MAX_SAMP+lv*MAX_SAMP/4+i]=dd;
        }
        for(int i=0;i<h;i++)tmp[i]=out[lv*MAX_SAMP/4+i];
        cl=h;
    }
}
static void stft(double d[],int len,int ws,int hop,double mag[]){
    int n=1; while(n<ws)n<<=1;
    int nf=(len-ws)/hop+1;
    double re[MAX_SAMP],im[MAX_SAMP];
    for(int f=0;f<nf;f++){
        for(int i=0;i<n;i++){
            if(i<ws&&(f*hop+i)<len){
                double w=0.5*(1.0-my_cos(2.0*PI*(double)i/(double)(ws-1)));
                re[i]=d[f*hop+i]*w;
            } else re[i]=0;
            im[i]=0;
        }
        fft(re,im,n);
        for(int i=0;i<n/2;i++)mag[f*(n/2)+i]=my_sqrt(re[i]*re[i]+im[i]*im[i]);
    }
}

/* ── 4. Motor Imagery Classification ── */
static void csp(double d1[],double d2[],int nch,int len,double flt[]){
    double c1[MAX_CH*MAX_CH]={0},c2[MAX_CH*MAX_CH]={0};
    for(int c=0;c<nch;c++)for(int dd=0;dd<nch;dd++){
        for(int t=0;t<len;t++){
            c1[c*nch+dd]+=d1[c*MAX_SAMP+t]*d1[dd*MAX_SAMP+t];
            c2[c*nch+dd]+=d2[c*MAX_SAMP+t]*d2[dd*MAX_SAMP+t];
        }
        c1[c*nch+dd]/=(double)len; c2[c*nch+dd]/=(double)len;
    }
    for(int c=0;c<nch;c++){
        double v1=c1[c*nch+c],v2=c2[c*nch+c],tot=v1+v2;
        flt[c]=(tot>0)?(v1-v2)/tot:0;
    }
}
static double lda_classify(double f[],double w[],double b,int n){
    double s=b; for(int i=0;i<n;i++)s+=w[i]*f[i]; return(s>0)?1.0:-1.0;
}
static double svm_classify(double f[],double sv[],double al[],double b,int nsv,int nf){
    double s=b;
    for(int i=0;i<nsv;i++){
        double dot=0; for(int j=0;j<nf;j++)dot+=al[i]*sv[i*nf+j]*f[j]; s+=dot;
    }
    return(s>0)?1.0:-1.0;
}
static double cnn_classify(double f[],double w[],int ni,int nh){
    double h[32];
    for(int i=0;i<nh&&i<32;i++){
        double s=0; for(int j=0;j<ni;j++)s+=w[i*ni+j]*f[j];
        h[i]=(s>0)?s:0;
    }
    double o=0; for(int i=0;i<nh&&i<32;i++)o+=h[i]*w[nh*ni+i];
    return(o>0)?1.0:-1.0;
}
static int mi_train(double dl[],double dr[],int nch,int len,double flt[],double w[]){
    csp(dl,dr,nch,len,flt);
    for(int c=0;c<nch;c++){
        double vl=0,vr=0;
        for(int t=0;t<len;t++){
            double pl=dl[c*MAX_SAMP+t]*flt[c],pr=dr[c*MAX_SAMP+t]*flt[c];
            vl+=pl*pl; vr+=pr*pr;
        }
        vl/=(double)len; vr/=(double)len;
        w[c]=my_sqrt(vr>0?vr:0)-my_sqrt(vl>0?vl:0);
    }
    return 0;
}
static int mi_predict(double f[],double w[],int nf){return(int)lda_classify(f,w,0.0,nf);}

/* ── 5. P300 Detection ── */
static void p300_stim_seq(p300_config_t cfg,int rows,int cols,int seq[]){
    int tot=rows*cols,tgt=(int)(srand_val()*(double)tot),pos=0;
    for(int r=0;r<4;r++)for(int s=0;s<tot;s++)seq[pos++]=(s==tgt)?1:0;
    print_str("[P300] Sequence: "); print_int(tot*4);
    print_str(" stimuli, target="); print_int(tgt); print_str("\n");
}
static void p300_extract_erp(double d[],int trg[],int nt,int spt,double erp[]){
    int cnt=0; for(int i=0;i<spt;i++)erp[i]=0;
    for(int t=0;t<nt;t++)if(trg[t]){
        for(int s=0;s<spt;s++)erp[s]+=d[t*spt+s]; cnt++;
    }
    if(cnt>0)for(int i=0;i<spt;i++)erp[i]/=(double)cnt;
    print_str("[P300] ERP from "); print_int(cnt); print_str(" trials\n");
}
static double p300_template(double erp[],double tpl[],int len){
    double sxy=0,sxx=0,syy=0;
    for(int i=0;i<len;i++){sxy+=erp[i]*tpl[i];sxx+=erp[i]*erp[i];syy+=tpl[i]*tpl[i];}
    double dn=my_sqrt(sxx*syy); return(dn>0)?sxy/dn:0;
}
static void p300_adapt(double erp[],double model[],double lr,int len){
    for(int i=0;i<len;i++)model[i]+=lr*(erp[i]-model[i]);
}
static int p300_detect(double erp[],int len,int sr,double thr){
    int s0=(250*sr)/1000,s1=(500*sr)/1000;
    if(s1>len)s1=len;
    double pk=0; int pi=-1;
    for(int i=s0;i<s1;i++)if(erp[i]>pk){pk=erp[i];pi=i;}
    int det=(pk>thr)?1:0;
    print_str("[P300] Peak at "); print_int(pi>=0?(pi*1000/sr):0);
    print_str("ms, amp="); print_double(pk);
    print_str(det?" -> DETECTED\n":" -> not detected\n"); return det;
}

/* ── 6. Neurofeedback ── */
static int nf_init(int mode,int proto,int fb){
    g_nf.mode=mode; g_nf.protocol=proto; g_nf.fb_type=fb; g_nf.baseline=1.0;
    const char *m[]={"ATTENTION","RELAXATION","CUSTOM"};
    print_str("[NFB] Init: "); print_str(m[mode%3]); print_str("\n"); return 0;
}
static double nf_band_power(double d[],int len,int band,int sr){
    freq_feat_t ff=freq_features(d,len,sr);
    if(band==0)return ff.delta; if(band==1)return ff.theta;
    if(band==2)return ff.alpha; if(band==3)return ff.beta; return ff.gamma;
}
static double nf_attention(double alpha,double beta,double theta){
    double t=alpha+beta+theta; if(t<=0)return 0;
    double s=(beta/t)*100.0;
    print_str("[NFB] Attention: "); print_double(s); print_str("\n"); return s;
}
static double nf_relaxation(double alpha,double theta){
    double t=alpha+theta+1.0,s=(alpha+theta*0.8)/t*100.0;
    print_str("[NFB] Relaxation: "); print_double(s); print_str("\n"); return s;
}
static int nf_detect_state(double sc[],int n,double thr){
    double a=0; for(int i=0;i<n;i++)a+=sc[i]; a/=(double)n;
    if(a>thr*1.2){print_str("[NFB] State: FOCUSED\n");return 0;}
    if(a<thr*0.6){print_str("[NFB] State: DROWSY\n");return 2;}
    print_str("[NFB] State: RELAXED\n"); return 1;
}
static double nf_game_score(int st,int dur_ms){
    double b=(st==0)?10.0:(st==1)?5.0:1.0,s=b*((double)dur_ms/1000.0);
    print_str("[NFB] Score: "); print_double(s); print_str(" pts\n"); return s;
}

/* ── 7. Cognitive Enhancement ── */
static void cog_plan(cog_task_t t[],int nt,int spw,int dw){
    print_str("[COG] Plan: "); print_int(nt); print_str(" tasks, ");
    print_int(spw); print_str("x/wk for "); print_int(dw); print_str(" wks\n");
    const char *tp[]={"WORKING_MEMORY","ATTENTION","EXECUTIVE"};
    for(int i=0;i<nt;i++){
        print_str("  Task "); print_int(i+1); print_str(": ");
        print_str(tp[t[i].type%3]); print_str(" diff="); print_int(t[i].difficulty); print_str("\n");
    }
}
static int cog_load(double f[],int n){
    double l=0; for(int i=0;i<n;i++)l+=my_abs(f[i]); l/=(double)n;
    if(l<10.0){print_str("[COG] Load: LOW\n");return 0;}
    if(l<25.0){print_str("[COG] Load: MEDIUM\n");return 1;}
    print_str("[COG] Load: HIGH\n"); return 2;
}
static double cog_nback(int stim[],int len,int n){
    if(n<1||len<=n)return 0;
    int cor=0,tot=0;
    for(int i=n;i<len;i++){
        int m=(stim[i]==stim[i-n])?1:0;
        int r=(srand_val()<0.8)?m:(1-m);
        if(r==m)cor++; tot++;
    }
    double a=(tot>0)?(double)cor/(double)tot*100.0:0;
    print_str("[COG] N-back(n="); print_int(n); print_str("): ");
    print_double(a); print_str("%\n"); return a;
}
static double cog_attention(double d[],int len,int sr){
    double m=0; for(int i=0;i<len;i++)m+=d[i]; m/=(double)len;
    double v=0; for(int i=0;i<len;i++){double e=d[i]-m;v+=e*e;} v/=(double)len;
    double cv=(m!=0)?my_sqrt(v)/my_abs(m)*100.0:0;
    double a=(cv<50.0)?100.0-cv:50.0;
    print_str("[COG] Attention: "); print_double(a); print_str("%\n"); return a;
}
static void cog_progress(double hist[],int ns){
    if(ns<2){print_str("[COG] Insufficient data\n");return;}
    double h1=0,h2=0; int h=ns/2;
    for(int i=0;i<h;i++)h1+=hist[i];
    for(int i=h;i<ns;i++)h2+=hist[i];
    h1/=(double)h; h2/=(double)(ns-h);
    print_str("[COG] Progress: 1st="); print_double(h1);
    print_str(" 2nd="); print_double(h2); print_str(" diff=");
    print_double(h2-h1); print_str("\n");
}

/* ── 8. Entry Point ── */
static void print_help(void){
    print_str(VERSION);
    print_str("\n\nUsage: brain_computer_interface [options]\n");
    print_str("  -h    Show help\n  -t    Run test/demo\n\n");
    print_str("Features: EEG Acquisition, Preprocessing, Feature Extraction,\n");
    print_str("  Motor Imagery (CSP+LDA/SVM/CNN), P300 Detection,\n");
    print_str("  Neurofeedback, Cognitive Enhancement\n");
}
static void run_test(void){
    print_str("=== "); print_str(VERSION); print_str(" ===\n\n");
    /* EEG Init & Simulate */
    print_str("--- EEG Signal Acquisition ---\n");
    eeg_config_t cfg={16,256,0}; eeg_init(cfg); eeg_set_rate(256);
    double imp[16]; for(int i=0;i<16;i++)imp[i]=g_ch[i].impedance;
    eeg_check_impedance(imp,16,5000.0); eeg_simulate(g_ch,16,2000);
    /* Preprocessing */
    print_str("\n--- Preprocessing ---\n");
    filter_bandpass(g_ch[0].buf,1024,0.5,50.0,256);
    print_str("  Bandpass 0.5-50Hz applied\n");
    filter_notch(g_ch[0].buf,1024,50.0,256);
    print_str("  Notch 50Hz applied\n");
    double eog_r[MAX_SAMP];
    for(int i=0;i<MAX_SAMP;i++)eog_r[i]=srand_val()*2.0-1.0;
    art_eog(g_ch[0].buf,eog_r,1024); print_str("  EOG removal done\n");
    int emg_rm=art_emg(g_ch[0].buf,1024,100.0);
    print_str("  EMG removed: "); print_int(emg_rm); print_str("\n");
    double mcd[MAX_CH*MAX_SAMP];
    for(int c=0;c<16;c++)for(int s=0;s<1024;s++)mcd[c*MAX_SAMP+s]=g_ch[c].buf[s];
    ref_standardize(mcd,16,1024,0); print_str("  Common avg ref applied\n");
    /* Features */
    print_str("\n--- Feature Extraction ---\n");
    time_feat_t tf=time_features(g_ch[0].buf,1024);
    print_str("  Time: mean="); print_double(tf.mean);
    print_str(" var="); print_double(tf.variance);
    print_str(" skew="); print_double(tf.skewness);
    print_str(" kurt="); print_double(tf.kurtosis); print_str("\n");
    print_str("  Hjorth: act="); print_double(tf.h_act);
    print_str(" mob="); print_double(tf.h_mob);
    print_str(" comp="); print_double(tf.h_comp); print_str("\n");
    freq_feat_t ff=freq_features(g_ch[0].buf,1024,256);
    print_str("  Freq: d="); print_double(ff.delta);
    print_str(" th="); print_double(ff.theta);
    print_str(" a="); print_double(ff.alpha);
    print_str(" b="); print_double(ff.beta);
    print_str(" g="); print_double(ff.gamma); print_str("\n");
    double wout[MAX_SAMP*2]; wavelet_dwt(g_ch[0].buf,1024,wout);
    print_str("  Wavelet 4-level done\n");
    /* Motor Imagery */
    print_str("\n--- Motor Imagery ---\n");
    double dl[MAX_CH*MAX_SAMP],dr[MAX_CH*MAX_SAMP];
    for(int i=0;i<16*512;i++){dl[i]=srand_val()*10.0+5.0;dr[i]=srand_val()*10.0-5.0;}
    double cf[MAX_CH],lw[MAX_CH];
    mi_train(dl,dr,16,512,cf,lw);
    print_str("  CSP+LDA trained\n");
    double tf2[16]; for(int i=0;i<16;i++)tf2[i]=cf[i]*2.5;
    int pred=mi_predict(tf2,lw,16);
    print_str("  Predict: "); print_str(pred>0?"RIGHT":"LEFT"); print_str(" hand\n");
    /* P300 */
    print_str("\n--- P300 Detection ---\n");
    p300_config_t pc={1,100,200,0.15}; int sq[240];
    p300_stim_seq(pc,6,6,sq);
    double raw[MAX_SAMP]; int trg[64];
    for(int i=0;i<1024;i++)raw[i]=srand_val()*5.0;
    int pl=(300*256)/1000;
    for(int i=0;i<16&&pl+i<1024;i++)raw[pl+i]+=15.0*my_sin(PI*(double)i/16.0);
    for(int t=0;t<64;t++)trg[t]=(t%4==0)?1:0;
    double erp[256]; p300_extract_erp(raw,trg,64,16,erp);
    p300_detect(erp,256,256,5.0);
    double tpl[256];
    for(int i=0;i<256;i++)tpl[i]=(i>=64&&i<=128)?my_sin(PI*(double)(i-64)/64.0):0;
    double cr=p300_template(erp,tpl,256);
    print_str("  Correlation: "); print_double(cr); print_str("\n");
    /* Neurofeedback */
    print_str("\n--- Neurofeedback ---\n");
    nf_init(0,0,0);
    double ap=nf_band_power(g_ch[0].buf,1024,2,256);
    double bp=nf_band_power(g_ch[0].buf,1024,3,256);
    double tp=nf_band_power(g_ch[0].buf,1024,1,256);
    nf_attention(ap,bp,tp); nf_relaxation(ap,tp);
    double nsc[10]; for(int i=0;i<10;i++)nsc[i]=60.0+srand_val()*30.0;
    nf_detect_state(nsc,10,70.0); nf_game_score(0,30000);
    /* Cognitive */
    print_str("\n--- Cognitive Enhancement ---\n");
    cog_task_t tsk[3]={{0,2,20,0},{1,3,30,0},{2,1,15,0}};
    cog_plan(tsk,3,3,8);
    double cf2[5]={12.5,8.3,15.7,22.1,9.4}; cog_load(cf2,5);
    int nb[30]; for(int i=0;i<30;i++)nb[i]=(int)(srand_val()*5.0);
    cog_nback(nb,30,2); cog_attention(g_ch[1].buf,1024,256);
    double hist[6]={55,60,62,70,75,80}; cog_progress(hist,6);
    print_str("\n=== All BCI tests completed ===\n");
}
void _start(void){
    char ab[BUF_SZ];
    int al=host_get_argv(0,BUF_SZ);
    int tm=0,hm=0;
    if(al>0){
        host_get_argv(0,BUF_SZ);
        for(int i=0;i<al;i++){
            if(ab[i]=='-'&&i+1<al){
                if(ab[i+1]=='t')tm=1; if(ab[i+1]=='h')hm=1;
            }
        }
    }
    if(hm){print_help();host_exit(0);}
    if(tm){run_test();host_exit(0);}
    print_str(VERSION); print_str("\nUse -h for help, -t for test\n");
    host_exit(0);
}
