/* yucca_admin: Yucca desert succulent and architectural plant management (v1.0)
 * Yucca planning, planting, evaluation, offset division, market
 * Features: rosette diameter, flower spike height, drought tolerance, offset count, leaf rigidity
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,rosette_dia,spike_ht,drought_tol,offset_ct,leaf_rigid,flower_yr,active;} yuc_t;
typedef struct{int n_plan,n_exec,n_eval,n_div,n_mkt,t_rosette,t_spike,t_drought,t_offset,t_leaf;} yuc_state_t;
static yuc_t yucps[N],yuces[N-2],yucvs[N-4],yucdv[N-6],yucms[N-6]; static yuc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(yuc_t*a,int*cnt,int*sum,int mx,int lc,int rd,int sh,int dt,int oc,int lr,int fy){if(*cnt>=mx)return -1;yuc_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->rosette_dia=rd;x->spike_ht=sh;x->drought_tol=dt;x->offset_ct=oc;x->leaf_rigid=lr;x->flower_yr=fy;x->active=1;*sum+=rd;(*cnt)++;ps("[YUC] Yucca ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" rd=");pi(rd);ps(" sh=");pi(sh);ps(" dt=");pi(dt);ps(" oc=");pi(oc);ps(" lr=");pi(lr);ps("\n");return *cnt-1;}
int yuc_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_div=0;st.n_mkt=0;st.t_rosette=0;st.t_spike=0;st.t_drought=0;st.t_offset=0;st.t_leaf=0;for(int i=0;i<N;i++)yucps[i].active=0;for(int i=0;i<N-2;i++)yuces[i].active=0;for(int i=0;i<N-4;i++)yucvs[i].active=0;for(int i=0;i<N-6;i++)yucdv[i].active=0;for(int i=0;i<N-6;i++)yucms[i].active=0;init=1;ps("[YUC] Yucca initialized\n");return 0;}
/* 1=desert 2=rock_garden 3=xeriscape 4=container 5=architectural */
int yuc_planning(int lc,int rd,int sh,int dt,int oc,int lr,int fy){return add(yucps,&st.n_plan,&st.t_rosette,N,lc,rd,sh,dt,oc,lr,fy);}
int yuc_execution(int lc,int rd,int sh,int dt,int oc,int lr,int fy){return add(yuces,&st.n_exec,&st.t_spike,N-2,lc,rd,sh,dt,oc,lr,fy);}
int yuc_evaluation(int lc,int rd,int sh,int dt,int oc,int lr,int fy){return add(yucvs,&st.n_eval,&st.t_drought,N-4,lc,rd,sh,dt,oc,lr,fy);}
int yuc_division(int lc,int rd,int sh,int dt,int oc,int lr,int fy){return add(yucdv,&st.n_div,&st.t_offset,N-6,lc,rd,sh,dt,oc,lr,fy);}
int yuc_market(int lc,int rd,int sh,int dt,int oc,int lr,int fy){return add(yucms,&st.n_mkt,&st.t_leaf,N-6,lc,rd,sh,dt,oc,lr,fy);}
void yuc_report(void){ps("[YUC] Plan: ");pi(st.n_plan);ps(" rosette=");pi(st.t_rosette);ps("\nExec: ");pi(st.n_exec);ps(" spike=");pi(st.t_spike);ps("\nEval: ");pi(st.n_eval);ps(" drought=");pi(st.t_drought);ps("\nDiv: ");pi(st.n_div);ps(" offset=");pi(st.t_offset);ps("\nMkt: ");pi(st.n_mkt);ps(" leaf=");pi(st.t_leaf);ps("\n");}
void yuc_state(void){ps("[YUC] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Div=");pi(st.n_div);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Yucca Admin Demo ===\n\n");yuc_init();
ps("Yucca planning (desert layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;yuc_planning(lc,40+(i*10),80+(i*20),(i%5)+1,3+(i%4),(i%6)+1,3+(i%3));}
ps("\nYucca execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;yuc_execution(lc,45+(i*8),90+(i*18),(i%5)+1,4+(i%3),(i%6)+1,4+(i%2));}
ps("\nYucca evaluation (drought check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;yuc_evaluation(lc,50+(i*7),100+(i*15),(i%4)+2,5+(i%3),(i%5)+2,4+(i%2));}
ps("\nYucca offset division...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;yuc_division(lc,35+(i*8),70+(i*15),(i%5)+1,2+(i%3),(i%4)+1,3+(i%3));}
ps("\nYucca architectural plant market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;yuc_market(lc,55+(i*6),110+(i*12),(i%4)+2,6+(i%2),(i%6)+3,5+(i%2));}
ps("\n");yuc_report();yuc_state();ps("\n=== Demo Complete ===\n");return 0;}
