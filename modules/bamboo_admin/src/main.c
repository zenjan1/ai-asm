/* bamboo_admin: Bamboo (Running Bamboo) screening management (v1.0)
 * Bamboo planning, planting, evaluation, thinning, market
 * Features: culm height, culm diameter, leaf length, growth rate, cold tolerance, spread rate
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,culm_ht,culm_dia,leaf_ln,growth_rt,cold_tl,spread_rt,active;} bam_t;
typedef struct{int n_plan,n_exec,n_eval,n_thin,n_mkt,t_ht,t_dia,t_leaf,t_growth,t_spread;} bam_state_t;
static bam_t bamps[N],bames[N-2],bamvs[N-4],bamth[N-6],bamms[N-6]; static bam_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bam_t*a,int*cnt,int*sum,int mx,int lc,int ch,int cd,int ll,int gr,int ct,int sr){if(*cnt>=mx)return -1;bam_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->culm_ht=ch;x->culm_dia=cd;x->leaf_ln=ll;x->growth_rt=gr;x->cold_tl=ct;x->spread_rt=sr;x->active=1;*sum+=ch;(*cnt)++;ps("[BAM] Bamboo ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ch=");pi(ch);ps(" cd=");pi(cd);ps(" ll=");pi(ll);ps(" gr=");pi(gr);ps(" ct=");pi(ct);ps("\n");return *cnt-1;}
int bam_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_thin=0;st.n_mkt=0;st.t_ht=0;st.t_dia=0;st.t_leaf=0;st.t_growth=0;st.t_spread=0;for(int i=0;i<N;i++)bamps[i].active=0;for(int i=0;i<N-2;i++)bames[i].active=0;for(int i=0;i<N-4;i++)bamvs[i].active=0;for(int i=0;i<N-6;i++)bamth[i].active=0;for(int i=0;i<N-6;i++)bamms[i].active=0;init=1;ps("[BAM] Bamboo initialized\n");return 0;}
/* 1=screen 2=grove 3=container 4=hedge 5=tropical */
int bam_planning(int lc,int ch,int cd,int ll,int gr,int ct,int sr){return add(bamps,&st.n_plan,&st.t_ht,N,lc,ch,cd,ll,gr,ct,sr);}
int bam_execution(int lc,int ch,int cd,int ll,int gr,int ct,int sr){return add(bames,&st.n_exec,&st.t_dia,N-2,lc,ch,cd,ll,gr,ct,sr);}
int bam_evaluation(int lc,int ch,int cd,int ll,int gr,int ct,int sr){return add(bamvs,&st.n_eval,&st.t_leaf,N-4,lc,ch,cd,ll,gr,ct,sr);}
int bam_thinning(int lc,int ch,int cd,int ll,int gr,int ct,int sr){return add(bamth,&st.n_thin,&st.t_growth,N-6,lc,ch,cd,ll,gr,ct,sr);}
int bam_market(int lc,int ch,int cd,int ll,int gr,int ct,int sr){return add(bamms,&st.n_mkt,&st.t_spread,N-6,lc,ch,cd,ll,gr,ct,sr);}
void bam_report(void){ps("[BAM] Plan: ");pi(st.n_plan);ps(" ht=");pi(st.t_ht);ps("\nExec: ");pi(st.n_exec);ps(" dia=");pi(st.t_dia);ps("\nEval: ");pi(st.n_eval);ps(" leaf=");pi(st.t_leaf);ps("\nThin: ");pi(st.n_thin);ps(" growth=");pi(st.t_growth);ps("\nMkt: ");pi(st.n_mkt);ps(" spread=");pi(st.t_spread);ps("\n");}
void bam_state(void){ps("[BAM] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Thin=");pi(st.n_thin);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Bamboo (Running Bamboo) Admin Demo ===\n\n");bam_init();
ps("Bamboo planning (screen layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;bam_planning(lc,300+(i*50),3+(i*2),15+(i*3),20+(i*5),-10+(i*2),10+(i*3));}
ps("\nBamboo execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;bam_execution(lc,350+(i*40),4+(i*2),18+(i*2),22+(i*4),-8+(i*2),12+(i*3));}
ps("\nBamboo evaluation (growth check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;bam_evaluation(lc,400+(i*30),5+(i*2),20+(i*2),25+(i*3),-6+(i*2),14+(i*2));}
ps("\nBamboo thinning...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;bam_thinning(lc,250+(i*45),2+(i*2),12+(i*3),18+(i*5),-12+(i*2),8+(i*3));}
ps("\nBamboo screening market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;bam_market(lc,450+(i*30),6+(i*2),22+(i*2),28+(i*3),-4+(i*2),16+(i*2));}
ps("\n");bam_report();bam_state();ps("\n=== Demo Complete ===\n");return 0;}
