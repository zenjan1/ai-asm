/* anacamptis_admin: Anacamptis (Swamp Orchid) wetland orchid management (v1.0)
 * Anacamptis planning, planting, evaluation, pollination, market
 * Features: spike height, flower count, labellum width, bloom color, tuber size, habitat moisture
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,spike_ht,flower_ct,labellum_wd,bloom_color,tuber_sz,habitat_mo,active;} anc_t;
typedef struct{int n_plan,n_exec,n_eval,n_poll,n_mkt,t_spike,t_flower,t_labellum,t_tuber,t_moist;} anc_state_t;
static anc_t ancps[N],ances[N-2],ancvs[N-4],ancpo[N-6],ancms[N-6]; static anc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(anc_t*a,int*cnt,int*sum,int mx,int lc,int sh,int fc,int lw,int bc,int ts,int hm){if(*cnt>=mx)return -1;anc_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->spike_ht=sh;x->flower_ct=fc;x->labellum_wd=lw;x->bloom_color=bc;x->tuber_sz=ts;x->habitat_mo=hm;x->active=1;*sum+=sh;(*cnt)++;ps("[ANC] Anacamptis ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" sh=");pi(sh);ps(" fc=");pi(fc);ps(" lw=");pi(lw);ps(" bc=");pi(bc);ps(" ts=");pi(ts);ps(" hm=");pi(hm);ps("\n");return *cnt-1;}
int anc_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_poll=0;st.n_mkt=0;st.t_spike=0;st.t_flower=0;st.t_labellum=0;st.t_tuber=0;st.t_moist=0;for(int i=0;i<N;i++)ancps[i].active=0;for(int i=0;i<N-2;i++)ances[i].active=0;for(int i=0;i<N-4;i++)ancvs[i].active=0;for(int i=0;i<N-6;i++)ancpo[i].active=0;for(int i=0;i<N-6;i++)ancms[i].active=0;init=1;ps("[ANC] Anacamptis initialized\n");return 0;}
/* 1=wetland 2=meadow 3=marsh 4=bog 5=riparian */
int anc_planning(int lc,int sh,int fc,int lw,int bc,int ts,int hm){return add(ancps,&st.n_plan,&st.t_spike,N,lc,sh,fc,lw,bc,ts,hm);}
int anc_execution(int lc,int sh,int fc,int lw,int bc,int ts,int hm){return add(ances,&st.n_exec,&st.t_flower,N-2,lc,sh,fc,lw,bc,ts,hm);}
int anc_evaluation(int lc,int sh,int fc,int lw,int bc,int ts,int hm){return add(ancvs,&st.n_eval,&st.t_labellum,N-4,lc,sh,fc,lw,bc,ts,hm);}
int anc_pollination(int lc,int sh,int fc,int lw,int bc,int ts,int hm){return add(ancpo,&st.n_poll,&st.t_tuber,N-6,lc,sh,fc,lw,bc,ts,hm);}
int anc_market(int lc,int sh,int fc,int lw,int bc,int ts,int hm){return add(ancms,&st.n_mkt,&st.t_moist,N-6,lc,sh,fc,lw,bc,ts,hm);}
void anc_report(void){ps("[ANC] Plan: ");pi(st.n_plan);ps(" spike=");pi(st.t_spike);ps("\nExec: ");pi(st.n_exec);ps(" flower=");pi(st.t_flower);ps("\nEval: ");pi(st.n_eval);ps(" labellum=");pi(st.t_labellum);ps("\nPoll: ");pi(st.n_poll);ps(" tuber=");pi(st.t_tuber);ps("\nMkt: ");pi(st.n_mkt);ps(" moist=");pi(st.t_moist);ps("\n");}
void anc_state(void){ps("[ANC] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Poll=");pi(st.n_poll);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Anacamptis (Swamp Orchid) Admin Demo ===\n\n");anc_init();
ps("Anacamptis planning (wetland layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;anc_planning(lc,25+(i*6),8+(i%5),12+(i*3),(i%6)+1,20+(i*4),7+(i%3));}
ps("\nAnacamptis execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;anc_execution(lc,30+(i*5),10+(i%4),14+(i*2),(i%6)+1,24+(i*3),8+(i%2));}
ps("\nAnacamptis evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;anc_evaluation(lc,35+(i*4),12+(i%3),16+(i*2),(i%5)+2,28+(i*3),9+(i%2));}
ps("\nAnacamptis pollination tracking...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;anc_pollination(lc,20+(i*5),6+(i%4),10+(i*3),(i%4)+1,16+(i*4),6+(i%3));}
ps("\nAnacamptis orchid market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;anc_market(lc,40+(i*3),14+(i%3),18+(i*2),(i%6)+2,32+(i*3),10+(i%2));}
ps("\n");anc_report();anc_state();ps("\n=== Demo Complete ===\n");return 0;}
