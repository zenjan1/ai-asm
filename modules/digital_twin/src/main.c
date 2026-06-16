/* digital_twin: Physical World Mirror Engine (v55.0)
 * Digital Twin Engine for AI-ASM OS - mirrors physical entities
 * with real-time sensor sync, predictive maintenance, scenario
 * simulation, and multi-scale modeling. No stdlib. */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);
#define MAX_ENT 32
#define MAX_PROP 16
#define MAX_STR 64
#define MAX_BUF 256
#define MAX_FM 8
#define MAX_ME 32
#define MAX_SC 16
#define MAX_MD 32
#define MAX_LNK 16
#define MAX_SUB 8
#define MAX_WG 24
#define MAX_TH 4
#define MAX_SMPL 1000
#define MAX_NM 32
#define VER "Digital Twin v55.0 - Physical World Mirror Engine"
/* --- Helpers --- */
static int my_strlen(const char *s){int n=0;while(s[n])n++;return n;}
static void my_strncpy(char *d,const char *s,int m){int i=0;while(i<m-1&&s[i]){d[i]=s[i];i++;}d[i]='\0';}
static void ps(const char *s){host_print(s);}
static void pi(int v){
    char b[16];int i=0,neg=0;unsigned u;
    if(v<0){neg=1;u=(unsigned)(-v);}else u=(unsigned)v;
    if(!u){b[i++]='0';}while(u>0){b[i++]='0'+(char)(u%10);u/=10;}
    char o[18];int k=0;if(neg)o[k++]='-';
    while(i>0)o[k++]=b[--i];o[k]='\0';host_print(o);
}
static void pd(double v){
    int w=(int)v;double f=v-(double)w;if(f<0.0)f=-f;
    pi(w);ps(".");int d=(int)(f*10000.0+0.5);
    if(d<1000)ps("0");if(d<100)ps("0");if(d<10)ps("0");pi(d);
}
static double my_sqrt(double x){
    if(x<=0.0)return 0.0;double g=x*0.5;
    for(int i=0;i<40;i++)g=(g+x/g)*0.5;return g;
}
static double abs_d(double x){return x<0.0?-x:x;}
static double clamp(double v,double lo,double hi){
    if(v<lo)return lo;if(v>hi)return hi;return v;
}
static unsigned rng=12345;
static double randf(void){rng=rng*1103515245u+12345u;return(double)(rng&0x7FFFFFFFu)/(double)0x7FFFFFFFu;}
static void *ta(unsigned int sz){unsigned o=host_alloc(sz,8);return(void*)o;}
/* ===== 1. Entity Modeling ===== */
#define E_MECH 0
#define E_ELEC 1
#define E_THER 2
#define E_CHEM 3
#define ST_NORM 0
#define ST_DEGR 1
#define ST_FAULT 2
#define ST_MAINT 3
#define ST_OFF 4
typedef struct{char key[24];double val;}tprop_t;
typedef struct{
    int id;char name[MAX_NM];int type;
    double pos[3],rot[4],scale,vel[3];
    tprop_t props[MAX_PROP];int pcnt;int state;
}twin_ent_t;
static twin_ent_t g_ent[MAX_ENT];static int g_ec=0;
static const char *st_name(int s){
    const char *n[]={"NORMAL","DEGRADED","FAULT","MAINTENANCE","OFFLINE"};
    return(s>=0&&s<=4)?n[s]:"UNKNOWN";
}
static twin_ent_t *ent_create(const char *nm,int tp){
    if(g_ec>=MAX_ENT)return NULL;
    twin_ent_t *e=&g_ent[g_ec++];
    e->id=g_ec;my_strncpy(e->name,nm,MAX_NM);e->type=tp;e->state=ST_NORM;e->scale=1.0;
    e->pcnt=0;for(int i=0;i<3;i++){e->pos[i]=0;e->vel[i]=0;}
    e->rot[0]=0;e->rot[1]=0;e->rot[2]=0;e->rot[3]=1;
    ps("[ENT] Created: ");ps(nm);ps("\n");return e;
}
static int ent_prop(twin_ent_t *e,const char *k,double v){
    if(!e)return -1;
    for(int i=0;i<e->pcnt;i++)if(e->props[i].key[0]==k[0]&&e->props[i].key[1]==k[1]){e->props[i].val=v;return 0;}
    if(e->pcnt>=MAX_PROP)return -1;
    my_strncpy(e->props[e->pcnt].key,k,24);e->props[e->pcnt].val=v;e->pcnt++;return 0;
}
static int ent_trans(twin_ent_t *e,int ns){
    if(!e)return -1;int old=e->state;e->state=ns;
    ps("[STATE] ");ps(e->name);ps(": ");ps(st_name(old));ps(" -> ");ps(st_name(ns));ps("\n");return 0;
}
/* ===== 2. Real-time Data Synchronization ===== */
#define D_TEMP 0
#define D_PRES 1
#define D_VOLT 2
#define D_RPM 3
#define D_VIB 4
typedef struct{
    int sid,eid,dt;double rate,buf[MAX_BUF];int blen,bpos;double ts;
    double kp,kx,kk;
}sstream_t;
static sstream_t g_ss[MAX_STR];static int g_sc=0;
static void sync_init(void){g_sc=0;ps("[SYNC] Engine initialized\n");}
static sstream_t *sync_create(int eid,int dt,double rate){
    if(g_sc>=MAX_STR)return NULL;
    sstream_t *s=&g_ss[g_sc++];
    s->sid=g_sc;s->eid=eid;s->dt=dt;s->rate=rate;s->blen=0;s->bpos=0;s->ts=0;
    s->kp=1.0;s->kx=0;s->kk=0;return s;
}
static void sync_ingest(sstream_t *s,double v,double ts){
    if(!s)return;s->buf[s->bpos]=v;s->bpos=(s->bpos+1)%MAX_BUF;
    if(s->blen<MAX_BUF)s->blen++;s->ts=ts;
}
static double sync_mavg(sstream_t *s,int win){
    if(!s||!s->blen)return 0;int w=win<s->blen?win:s->blen;double sum=0;
    for(int i=0;i<w;i++){int idx=(s->bpos-1-i+MAX_BUF)%MAX_BUF;sum+=s->buf[idx];}
    return sum/(double)w;
}
static void sync_kalman(sstream_t *s,double meas){
    if(!s)return;double q=0.01,r=0.1;
    s->kp=s->kp+q;s->kk=s->kp/(s->kp+r);
    s->kx=s->kx+s->kk*(meas-s->kx);s->kp=(1.0-s->kk)*s->kp;
}
static int sync_anomaly(sstream_t *s,double thr){
    if(!s||s->blen<4)return 0;
    double mean=sync_mavg(s,s->blen),var=0;
    for(int i=0;i<s->blen;i++){double d=s->buf[i]-mean;var+=d*d;}
    double sd=my_sqrt(var/(double)s->blen);if(sd<0.0001)return 0;
    double lat=s->buf[(s->bpos-1+MAX_BUF)%MAX_BUF];
    return abs_d((lat-mean)/sd)>thr?1:0;
}
static void sync_align(void){
    double lat=0;for(int i=0;i<g_sc;i++)if(g_ss[i].ts>lat)lat=g_ss[i].ts;
    ps("[SYNC] Aligned to ");pd(lat);ps("s across ");pi(g_sc);ps(" streams\n");
}
/* ===== 3. Predictive Maintenance ===== */
#define FM_BRG 0
#define FM_BAT 1
#define FM_MOT 2
#define FM_SNS 3
#define FM_GRB 4
typedef struct{int mid,etype;char name[MAX_NM];double mtbf;int sev;}fmode_t;
typedef struct{int eid,fm;double rul,conf,sched;}mevent_t;
static fmode_t g_fm[MAX_FM];static int g_fc=0;
static mevent_t g_me[MAX_ME];static int g_mc=0;
static void maint_init(void){
    g_fc=0;g_mc=0;
    struct{int et;const char *n;double m;int s;}fd[]={
        {E_MECH,"Bearing Wear",8000,3},{E_ELEC,"Battery Decay",5000,4},
        {E_MECH,"Motor Overheat",6000,5},{E_ELEC,"Sensor Drift",3000,2},
        {E_MECH,"Gearbox Backlash",10000,3}};
    for(int i=0;i<5;i++){
        g_fm[g_fc].mid=g_fc;g_fm[g_fc].etype=fd[i].et;
        my_strncpy(g_fm[g_fc].name,fd[i].n,MAX_NM);g_fm[g_fc].mtbf=fd[i].m;g_fm[g_fc].sev=fd[i].s;g_fc++;
    }
    ps("[MAINT] Library loaded: ");pi(g_fc);ps(" modes\n");
}
static double maint_degrade(double h,double el,double mtbf){
    double r=el/mtbf;return h/(1.0+r+r*r*0.5);
}
static double maint_predict(twin_ent_t *e,double sv,double thr){
    if(!e)return 0;double h=1.0-clamp(sv/thr,0,1),mtbf=5000;
    for(int i=0;i<g_fc;i++)if(g_fm[i].etype==e->type){mtbf=g_fm[i].mtbf;break;}
    double dg=maint_degrade(h,100,mtbf);
    double rul=(dg>0.05)?mtbf*dg/(1.0-h+0.01):0;rul=clamp(rul,0,mtbf);
    if(g_mc<MAX_ME){mevent_t *m=&g_me[g_mc++];m->eid=e->id;m->fm=e->type;m->rul=rul;m->conf=0.85;m->sched=0;}
    ps("[MAINT] ");ps(e->name);ps(" RUL=");pd(rul);ps("h h=");pd(h);ps("\n");return rul;
}
static void maint_sched(double budget,double horizon){
    ps("[MAINT] Scheduling budget=");pd(budget);ps(" horizon=");pd(horizon);ps("h\n");
    for(int i=0;i<g_mc;i++)for(int j=i+1;j<g_mc;j++)
        if(g_me[j].rul<g_me[i].rul){mevent_t t=g_me[i];g_me[i]=g_me[j];g_me[j]=t;}
    double spent=0;
    for(int i=0;i<g_mc;i++){
        double cost=100+(double)g_me[i].fm*50;
        if(spent+cost<=budget&&g_me[i].rul<=horizon){
            g_me[i].sched=g_me[i].rul*0.8;spent+=cost;
            ps("[MAINT]   Ev#");pi(i);ps(" at ");pd(g_me[i].sched);ps("h cost=");pd(cost);ps("\n");
        }
    }
}
/* ===== 4. Scenario Simulation ===== */
typedef struct{char nm[24];double val,lo,hi;}scparam_t;
typedef struct{
    char name[MAX_NM],desc[64];scparam_t params[16];int pc;
    double dur,res[MAX_SMPL];int rc;double smean,sstd,smin,smax;
}scenario_t;
static scenario_t g_scs[MAX_SC];static int g_sn=0;
static scenario_t *sc_create(const char *nm,const char *ds){
    if(g_sn>=MAX_SC)return NULL;
    scenario_t *s=&g_scs[g_sn++];
    my_strncpy(s->name,nm,MAX_NM);my_strncpy(s->desc,ds,64);
    s->pc=0;s->rc=0;s->dur=100;ps("[SC] Created: ");ps(nm);ps("\n");return s;
}
static void sc_param(scenario_t *s,const char *p,double v,double lo,double hi){
    if(!s||s->pc>=16)return;
    my_strncpy(s->params[s->pc].nm,p,24);s->params[s->pc].val=v;
    s->params[s->pc].lo=lo;s->params[s->pc].hi=hi;s->pc++;
}
static double sc_step(scenario_t *s,double dt){
    double b=25;for(int i=0;i<s->pc;i++){
        double n=(randf()-0.5)*(s->params[i].hi-s->params[i].lo)*0.1;
        b+=s->params[i].val*dt*0.01+n;}return b;
}
static void sc_stats(scenario_t *s){
    if(!s||!s->rc)return;
    double sum=0,mn=s->res[0],mx=s->res[0];
    for(int i=0;i<s->rc;i++){sum+=s->res[i];if(s->res[i]<mn)mn=s->res[i];if(s->res[i]>mx)mx=s->res[i];}
    s->smean=sum/(double)s->rc;double var=0;
    for(int i=0;i<s->rc;i++){double d=s->res[i]-s->smean;var+=d*d;}
    s->sstd=my_sqrt(var/(double)s->rc);s->smin=mn;s->smax=mx;
    ps("[SC] mean=");pd(s->smean);ps(" std=");pd(s->sstd);
    ps(" min=");pd(mn);ps(" max=");pd(mx);ps("\n");
}
static void sc_run(scenario_t *s){
    if(!s)return;s->rc=0;
    ps("[SC] Running: ");ps(s->name);ps(" (Monte Carlo 1000)\n");
    for(int i=0;i<MAX_SMPL;i++)s->res[s->rc++]=sc_step(s,s->dur*randf());
    sc_stats(s);
}
static void sc_parallel(int cnt){
    ps("[SC] Parallel run: ");pi(cnt);ps(" scenarios\n");
    for(int i=0;i<cnt&&i<g_sn;i++)sc_run(&g_scs[i]);
}
/* ===== 5. Multi-scale Modeling ===== */
#define L_MACRO 0
#define L_MESO 1
#define L_MICRO 2
typedef struct{
    int eid,lv;double params[8];int pc;
    int subs[MAX_SUB];int sc,par;
}tmodel_t;
static tmodel_t g_md[MAX_MD];static int g_mn=0;
static int g_lnk[MAX_LNK][2];static int g_ln=0;
static tmodel_t *md_create(int eid,int lv){
    if(g_mn>=MAX_MD)return NULL;
    tmodel_t *m=&g_md[g_mn++];m->eid=eid;m->lv=lv;m->pc=0;m->sc=0;m->par=-1;
    const char *l[]= {"MACRO","MESO","MICRO"};
    ps("[MD] Created ");ps(l[lv]);ps(" for entity ");pi(eid);ps("\n");return m;
}
static void md_param(tmodel_t *m,double v){if(!m||m->pc>=8)return;m->params[m->pc++]=v;}
static void md_link(tmodel_t *p,tmodel_t *c){
    if(!p||!c)return;int ci=(int)(c-g_md);
    if(p->sc<MAX_SUB)p->subs[p->sc++]=ci;c->par=(int)(p-g_md);
    if(g_ln<MAX_LNK){g_lnk[g_ln][0]=(int)(p-g_md);g_lnk[g_ln][1]=ci;g_ln++;}
    ps("[MD] Link #");pi((int)(p-g_md));ps(" -> #");pi(ci);ps("\n");
}
static double md_prop(tmodel_t *m,double inp){
    if(!m)return 0;double o=inp;
    for(int i=0;i<m->pc;i++)o*=m->params[i];
    o+=(m->lv==L_MACRO)?0:(m->lv==L_MESO)?inp*0.1:inp*0.01;
    ps("[MD] Prop L");pi(m->lv);ps(": ");pd(inp);ps("->");pd(o);ps("\n");
    for(int i=0;i<m->sc;i++)md_prop(&g_md[m->subs[i]],o);return o;
}
static double md_agg(tmodel_t *m){
    if(!m||!m->sc)return 0;double sum=0;
    for(int i=0;i<m->sc;i++){tmodel_t *c=&g_md[m->subs[i]];double v=0;
        for(int j=0;j<c->pc;j++)v+=c->params[j];sum+=v;}
    double a=sum/(double)m->sc;
    ps("[MD] Agg L");pi(m->lv);ps(": ch=");pi(m->sc);ps(" val=");pd(a);ps("\n");return a;
}
/* ===== 6. Dashboard & Visualization ===== */
#define W_GAUGE 0
#define W_CHART 1
#define W_ALERT 2
#define W_KPI 3
typedef struct{
    int type,eid;char metric[24];double mn,mx;double th[MAX_TH];int tc;
}widget_t;
typedef struct{char name[MAX_NM];int wc;widget_t wg[MAX_WG];int interval;}dash_t;
static dash_t *dash_create(const char *nm){
    dash_t *d=(dash_t*)ta(sizeof(dash_t));if(!d)return NULL;
    my_strncpy(d->name,nm,MAX_NM);d->wc=0;d->interval=1000;
    ps("[DASH] Created: ");ps(nm);ps("\n");return d;
}
static int dash_widget(dash_t *d,int tp,int eid,const char *met,double mn,double mx){
    if(!d||d->wc>=MAX_WG)return -1;
    widget_t *w=&d->wg[d->wc++];w->type=tp;w->eid=eid;
    my_strncpy(w->metric,met,24);w->mn=mn;w->mx=mx;w->tc=0;return d->wc-1;
}
static void dash_thresh(dash_t *d,int idx,double warn,double crit){
    if(!d||idx<0||idx>=d->wc)return;
    d->wg[idx].th[0]=warn;d->wg[idx].th[1]=crit;d->wg[idx].tc=2;
}
static double dash_kpi(widget_t *w,double up,double tot,double qual){
    if(!w)return 0;double av=(tot>0)?up/tot:0,perf=clamp(qual,0,1),oee=av*perf*qual;
    ps("[KPI] ");ps(w->metric);ps(" OEE=");pd(oee*100);ps("% av=");pd(av*100);ps("%\n");return oee;
}
static int dash_alert(dash_t *d,int idx,double v){
    if(!d||idx<0||idx>=d->wc)return 0;widget_t *w=&d->wg[idx];
    if(w->tc>=2&&v>=w->th[1]){ps("[ALERT] CRITICAL: ");ps(w->metric);ps("=");pd(v);ps("\n");return 2;}
    if(w->tc>=1&&v>=w->th[0]){ps("[ALERT] WARNING: ");ps(w->metric);ps("=");pd(v);ps("\n");return 1;}
    return 0;
}
static void dash_render(dash_t *d){
    if(!d)return;
    ps("========================================\n");
    ps("  Dashboard: ");ps(d->name);ps("\n");
    ps("  Widgets: ");pi(d->wc);ps("  Interval: ");pi(d->interval);ps("ms\n");
    ps("----------------------------------------\n");
    for(int i=0;i<d->wc;i++){
        widget_t *w=&d->wg[i];
        const char *t=(w->type==W_GAUGE)?"GAUGE":(w->type==W_CHART)?"CHART":(w->type==W_ALERT)?"ALERT":"KPI";
        ps("  [");ps(t);ps("] ");ps(w->metric);ps(" ent=");pi(w->eid);
        ps(" [");pd(w->mn);ps(",");pd(w->mx);ps("]\n");
    }
    ps("========================================\n");
}
/* ===== 7. Entry Point ===== */
static void show_help(void){
    ps(VER);ps("\n\nUsage: digital_twin [options]\n  -h    Show help\n  -t    Run test mode\n");
}
static void run_tests(void){
    ps("=== Digital Twin Test Suite ===\n\n");
    ps("--- Entity Modeling ---\n");
    twin_ent_t *mot=ent_create("Motor-01",E_MECH);
    twin_ent_t *brg=ent_create("Bearing-01",E_MECH);
    twin_ent_t *bat=ent_create("Battery-01",E_ELEC);
    twin_ent_t *sns=ent_create("Sensor-01",E_ELEC);
    ent_prop(mot,"temperature",72.5);ent_prop(mot,"rpm",3450);
    ent_prop(brg,"vibration",4.2);ent_prop(brg,"temperature",58);
    ent_prop(bat,"voltage",12.3);ent_prop(bat,"charge",85);
    ent_prop(sns,"sampling_hz",100);ps("\n");
    ps("--- Sensor Sync ---\n");
    sync_init();
    sstream_t *ts=sync_create(brg->id,D_TEMP,100);
    sstream_t *vs=sync_create(brg->id,D_VIB,200);
    for(int i=0;i<50;i++){
        double t=55+randf()*6,v=3.5+randf()*1.5;
        sync_ingest(ts,t,(double)i*0.01);sync_ingest(vs,v,(double)i*0.005);
        sync_kalman(ts,t);
    }
    sync_ingest(ts,95.0,0.51);sync_ingest(vs,15.0,0.255);
    sync_align();
    ps("[ANOMALY] Temp: ");ps(sync_anomaly(ts,2.5)?"DETECTED":"OK");ps("\n");
    ps("[ANOMALY] Vib: ");ps(sync_anomaly(vs,2.5)?"DETECTED":"OK");ps("\n");
    ps("[KALMAN] Temp=");pd(ts->kx);ps("\n\n");
    ps("--- Predictive Maintenance ---\n");
    maint_init();ent_trans(brg,ST_DEGR);
    double rul=maint_predict(brg,4.2,10);maint_predict(mot,72.5,100);maint_predict(bat,3.5,5);
    maint_sched(5000,10000);ps("[RUL] Bearing: ");pd(rul);ps("h\n\n");
    ps("--- Scenario Simulation ---\n");
    scenario_t *s1=sc_create("temp_increase","Temperature rise");
    sc_param(s1,"ambient_temp",35,20,50);sc_param(s1,"load_factor",1.5,0.8,2);
    scenario_t *s2=sc_create("high_load","Overload test");
    sc_param(s2,"rpm_target",5000,3000,6000);sc_param(s2,"duration_h",24,1,48);
    sc_parallel(2);ps("\n");
    ps("--- Multi-scale Modeling ---\n");
    tmodel_t *sys=md_create(mot->id,L_MACRO);md_param(sys,1.0);
    tmodel_t *comp=md_create(mot->id,L_MESO);md_param(comp,0.95);
    tmodel_t *part=md_create(mot->id,L_MICRO);md_param(part,0.99);
    md_link(sys,comp);md_link(comp,part);
    md_prop(sys,100);md_agg(sys);
    ps("[LINKS] Total: ");pi(g_ln);ps("\n\n");
    ps("--- Dashboard ---\n");
    dash_t *dash=dash_create("Operations Overview");
    int w0=dash_widget(dash,W_GAUGE,brg->id,"Bearing Temp",0,120);
    dash_thresh(dash,w0,70,90);
    int w1=dash_widget(dash,W_KPI,mot->id,"Motor OEE",0,100);
    dash_widget(dash,W_CHART,sns->id,"Sensor Feed",0,200);
    dash_widget(dash,W_ALERT,bat->id,"Battery Alert",0,100);
    dash_kpi(&dash->wg[w1],168,200,0.92);dash_alert(dash,w0,92.5);
    dash_render(dash);
    ps("--- State Machine ---\n");
    ent_trans(mot,ST_NORM);ent_trans(brg,ST_MAINT);ent_trans(bat,ST_NORM);ent_trans(sns,ST_OFF);
    ps("\n=== Test Suite Complete ===\n");
}
__attribute__((export_name("_start")))
void _start(void){
    char buf[128];int test=0,help=0;
    int got=host_get_argv(0,sizeof(buf));
    if(got>0){
        host_get_argv(0,sizeof(buf));
        for(int i=0;i<got&&i<(int)sizeof(buf)-1;i++){
            if(buf[i]=='-'&&i+1<got){
                if(buf[i+1]=='t')test=1;if(buf[i+1]=='h')help=1;
            }
        }
    }
    if(help){show_help();host_exit(0);return;}
    ps(VER);ps("\n");
    if(test)run_tests();else ps("Use -h for help, -t for test mode\n");
    host_exit(0);
}
