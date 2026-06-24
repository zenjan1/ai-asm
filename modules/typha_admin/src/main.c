/* typha_admin: Typha (Cattail) wetland grass and riparian zone management (v1.0)
 * Typha planning, planting, evaluation, control, wetland market
 * Features: stem density tracking, water depth tolerance, spread rate, rhizome expansion, habitat score
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,stem_den,water_dep,spread_rt,rhizome_len,habitat_sc,control_m,active;} typ_t;
typedef struct{int n_plan,n_exec,n_eval,n_ctrl,n_mkt,t_stem,t_water,t_spread,t_rhizome,t_habitat;} typ_state_t;
static typ_t typps[N],types[N-2],typvs[N-4],typcs[N-6],typms[N-6]; static typ_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(typ_t*a,int*cnt,int*sum,int mx,int lc,int sd,int wd,int sr,int rl,int hs,int cm){if(*cnt>=mx)return -1;typ_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->stem_den=sd;x->water_dep=wd;x->spread_rt=sr;x->rhizome_len=rl;x->habitat_sc=hs;x->control_m=cm;x->active=1;*sum+=sd;(*cnt)++;ps("[TYP] Typha ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" sd=");pi(sd);ps(" wd=");pi(wd);ps(" sr=");pi(sr);ps(" rl=");pi(rl);ps(" hs=");pi(hs);ps("\n");return *cnt-1;}
int typ_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_ctrl=0;st.n_mkt=0;st.t_stem=0;st.t_water=0;st.t_spread=0;st.t_rhizome=0;st.t_habitat=0;for(int i=0;i<N;i++)typps[i].active=0;for(int i=0;i<N-2;i++)types[i].active=0;for(int i=0;i<N-4;i++)typvs[i].active=0;for(int i=0;i<N-6;i++)typcs[i].active=0;for(int i=0;i<N-6;i++)typms[i].active=0;init=1;ps("[TYP] Typha (cattail) initialized\n");return 0;}
/* 1=marsh 2=pond_edge 3=stream_bank 4=lake_shore 5=swamp */
int typ_planning(int lc,int sd,int wd,int sr,int rl,int hs,int cm){return add(typps,&st.n_plan,&st.t_stem,N,lc,sd,wd,sr,rl,hs,cm);}
int typ_execution(int lc,int sd,int wd,int sr,int rl,int hs,int cm){return add(types,&st.n_exec,&st.t_water,N-2,lc,sd,wd,sr,rl,hs,cm);}
int typ_evaluation(int lc,int sd,int wd,int sr,int rl,int hs,int cm){return add(typvs,&st.n_eval,&st.t_spread,N-4,lc,sd,wd,sr,rl,hs,cm);}
int typ_control(int lc,int sd,int wd,int sr,int rl,int hs,int cm){return add(typcs,&st.n_ctrl,&st.t_rhizome,N-6,lc,sd,wd,sr,rl,hs,cm);}
int typ_market(int lc,int sd,int wd,int sr,int rl,int hs,int cm){return add(typms,&st.n_mkt,&st.t_habitat,N-6,lc,sd,wd,sr,rl,hs,cm);}
void typ_report(void){ps("[TYP] Plan: ");pi(st.n_plan);ps(" stem=");pi(st.t_stem);ps("\nExec: ");pi(st.n_exec);ps(" water=");pi(st.t_water);ps("\nEval: ");pi(st.n_eval);ps(" spread=");pi(st.t_spread);ps("\nCtrl: ");pi(st.n_ctrl);ps(" rhizome=");pi(st.t_rhizome);ps("\nMkt: ");pi(st.n_mkt);ps(" habitat=");pi(st.t_habitat);ps("\n");}
void typ_state(void){ps("[TYP] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Ctrl=");pi(st.n_ctrl);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Typha (Cattail) Admin Demo ===\n\n");typ_init();
ps("Typha planning (wetland survey)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;typ_planning(lc,20+(i*5),30+(i*8),5+(i%4),40+(i*10),(i%5)+1,(i%3)+1);}
ps("\nTypha execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;typ_execution(lc,25+(i*4),35+(i*7),6+(i%3),45+(i*9),(i%5)+1,(i%3)+1);}
ps("\nTypha evaluation (spread assessment)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;typ_evaluation(lc,30+(i*3),40+(i*6),7+(i%3),50+(i*8),(i%5)+2,(i%2)+2);}
ps("\nTypha control management...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;typ_control(lc,15+(i*4),25+(i*5),4+(i%3),35+(i*7),(i%4)+1,(i%3)+1);}
ps("\nTypha wetland market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;typ_market(lc,35+(i*3),45+(i*5),8+(i%2),55+(i*6),(i%5)+2,(i%3)+1);}
ps("\n");typ_report();typ_state();ps("\n=== Demo Complete ===\n");return 0;}
