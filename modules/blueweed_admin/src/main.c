/* blueweed_admin: Blueweed (Echium vulgare) viper's bugloss management (v1.0)
 * Blueweed planning, planting, evaluation, deadheading, market
 * Features: spike height, flower count, leaf width, flower color, stem height, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,spike_ht,flower_ct,leaf_wd,flower_color,stem_ht,bloom_wk,active;} blw_t;
typedef struct{int n_plan,n_exec,n_eval,n_dead,n_mkt,t_spike,t_flower,t_leaf,t_color,t_stem;} blw_state_t;
static blw_t blwps[N],blwes[N-2],blwvs[N-4],blwdh[N-6],blwms[N-6]; static blw_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(blw_t*a,int*cnt,int*sum,int mx,int lc,int sh,int fc,int lw,int fl,int sth,int bw){if(*cnt>=mx)return -1;blw_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->spike_ht=sh;x->flower_ct=fc;x->leaf_wd=lw;x->flower_color=fl;x->stem_ht=sth;x->bloom_wk=bw;x->active=1;*sum+=sh;(*cnt)++;ps("[BLW] Blueweed ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" sh=");pi(sh);ps(" fc=");pi(fc);ps(" lw=");pi(lw);ps(" fl=");pi(fl);ps(" sth=");pi(sth);ps("\n");return *cnt-1;}
int blw_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_dead=0;st.n_mkt=0;st.t_spike=0;st.t_flower=0;st.t_leaf=0;st.t_color=0;st.t_stem=0;for(int i=0;i<N;i++)blwps[i].active=0;for(int i=0;i<N-2;i++)blwes[i].active=0;for(int i=0;i<N-4;i++)blwvs[i].active=0;for(int i=0;i<N-6;i++)blwdh[i].active=0;for(int i=0;i<N-6;i++)blwms[i].active=0;init=1;ps("[BLW] Blueweed initialized\n");return 0;}
/* 1=wasteland 2=roadsides 3=chalk_downland 4=pollinator_garden 5=meadow */
int blw_planning(int lc,int sh,int fc,int lw,int fl,int sth,int bw){return add(blwps,&st.n_plan,&st.t_spike,N,lc,sh,fc,lw,fl,sth,bw);}
int blw_execution(int lc,int sh,int fc,int lw,int fl,int sth,int bw){return add(blwes,&st.n_exec,&st.t_flower,N-2,lc,sh,fc,lw,fl,sth,bw);}
int blw_evaluation(int lc,int sh,int fc,int lw,int fl,int sth,int bw){return add(blwvs,&st.n_eval,&st.t_leaf,N-4,lc,sh,fc,lw,fl,sth,bw);}
int blw_deadheading(int lc,int sh,int fc,int lw,int fl,int sth,int bw){return add(blwdh,&st.n_dead,&st.t_color,N-6,lc,sh,fc,lw,fl,sth,bw);}
int blw_market(int lc,int sh,int fc,int lw,int fl,int sth,int bw){return add(blwms,&st.n_mkt,&st.t_stem,N-6,lc,sh,fc,lw,fl,sth,bw);}
void blw_report(void){ps("[BLW] Plan: ");pi(st.n_plan);ps(" spike=");pi(st.t_spike);ps("\nExec: ");pi(st.n_exec);ps(" flower=");pi(st.t_flower);ps("\nEval: ");pi(st.n_eval);ps(" leaf=");pi(st.t_leaf);ps("\nDead: ");pi(st.n_dead);ps(" color=");pi(st.t_color);ps("\nMkt: ");pi(st.n_mkt);ps(" stem=");pi(st.t_stem);ps("\n");}
void blw_state(void){ps("[BLW] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Dead=");pi(st.n_dead);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Blueweed (Echium vulgare) Admin Demo ===\n\n");blw_init();
ps("Blueweed planning (variety layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;blw_planning(lc,30+(i*6),15+(i*5),4+(i*2),(i%4)+1,35+(i*5),22+(i%6));}
ps("\nBlueweed execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;blw_execution(lc,35+(i*5),18+(i*4),5+(i*2),(i%4)+1,40+(i*4),24+(i%5));}
ps("\nBlueweed evaluation (growth check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;blw_evaluation(lc,40+(i*4),20+(i*3),6+(i*2),(i%3)+2,45+(i*3),26+(i%4));}
ps("\nBlueweed deadheading...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;blw_deadheading(lc,25+(i*6),12+(i*5),3+(i*2),(i%4)+1,30+(i*5),20+(i%5));}
ps("\nBlueweed market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;blw_market(lc,45+(i*3),22+(i*3),7+(i*2),(i%3)+3,50+(i*3),28+(i%3));}
ps("\n");blw_report();blw_state();ps("\n=== Demo Complete ===\n");return 0;}
