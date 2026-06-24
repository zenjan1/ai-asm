/* wisteria_admin: Wisteria climbing vine and fragrant flowering creeper management (v1.0)
 * Wisteria planning, planting, evaluation, pruning, market
 * Features: vine length, flower cluster count, fragrance intensity, support structure, bloom timing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,vine_len,cluster_ct,fragrance,support,bloom_wk,prune_ct,active;} wis_t;
typedef struct{int n_plan,n_exec,n_eval,n_prune,n_mkt,t_vine,t_cluster,t_frag,t_support,t_bloom;} wis_state_t;
static wis_t wisps[N],wises[N-2],wisvs[N-4],wispm[N-6],wisms[N-6]; static wis_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(wis_t*a,int*cnt,int*sum,int mx,int lc,int vl,int cc,int fg,int sp,int bw,int pc){if(*cnt>=mx)return -1;wis_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->vine_len=vl;x->cluster_ct=cc;x->fragrance=fg;x->support=sp;x->bloom_wk=bw;x->prune_ct=pc;x->active=1;*sum+=vl;(*cnt)++;ps("[WIS] Wisteria ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" vl=");pi(vl);ps(" cc=");pi(cc);ps(" fg=");pi(fg);ps(" sp=");pi(sp);ps(" bw=");pi(bw);ps("\n");return *cnt-1;}
int wis_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_prune=0;st.n_mkt=0;st.t_vine=0;st.t_cluster=0;st.t_frag=0;st.t_support=0;st.t_bloom=0;for(int i=0;i<N;i++)wisps[i].active=0;for(int i=0;i<N-2;i++)wises[i].active=0;for(int i=0;i<N-4;i++)wisvs[i].active=0;for(int i=0;i<N-6;i++)wispm[i].active=0;for(int i=0;i<N-6;i++)wisms[i].active=0;init=1;ps("[WIS] Wisteria initialized\n");return 0;}
/* 1=pergola 2=arbor 3=wall 4=fence 5=tree */
int wis_planning(int lc,int vl,int cc,int fg,int sp,int bw,int pc){return add(wisps,&st.n_plan,&st.t_vine,N,lc,vl,cc,fg,sp,bw,pc);}
int wis_execution(int lc,int vl,int cc,int fg,int sp,int bw,int pc){return add(wises,&st.n_exec,&st.t_cluster,N-2,lc,vl,cc,fg,sp,bw,pc);}
int wis_evaluation(int lc,int vl,int cc,int fg,int sp,int bw,int pc){return add(wisvs,&st.n_eval,&st.t_frag,N-4,lc,vl,cc,fg,sp,bw,pc);}
int wis_pruning(int lc,int vl,int cc,int fg,int sp,int bw,int pc){return add(wispm,&st.n_prune,&st.t_support,N-6,lc,vl,cc,fg,sp,bw,pc);}
int wis_market(int lc,int vl,int cc,int fg,int sp,int bw,int pc){return add(wisms,&st.n_mkt,&st.t_bloom,N-6,lc,vl,cc,fg,sp,bw,pc);}
void wis_report(void){ps("[WIS] Plan: ");pi(st.n_plan);ps(" vine=");pi(st.t_vine);ps("\nExec: ");pi(st.n_exec);ps(" cluster=");pi(st.t_cluster);ps("\nEval: ");pi(st.n_eval);ps(" frag=");pi(st.t_frag);ps("\nPrune: ");pi(st.n_prune);ps(" support=");pi(st.t_support);ps("\nMkt: ");pi(st.n_mkt);ps(" bloom=");pi(st.t_bloom);ps("\n");}
void wis_state(void){ps("[WIS] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Prune=");pi(st.n_prune);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Wisteria Admin Demo ===\n\n");wis_init();
ps("Wisteria planning (vine layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;wis_planning(lc,100+(i*30),10+(i*5),(i%7)+1,(i%5)+1,18+(i%6),2+(i%3));}
ps("\nWisteria execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;wis_execution(lc,120+(i*25),12+(i*4),(i%7)+1,(i%5)+1,19+(i%5),3+(i%2));}
ps("\nWisteria evaluation (fragrance check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;wis_evaluation(lc,140+(i*20),14+(i*3),(i%6)+2,(i%4)+2,20+(i%4),3+(i%2));}
ps("\nWisteria pruning management...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;wis_pruning(lc,80+(i*20),8+(i*3),(i%5)+1,(i%5)+1,17+(i%5),2+(i%3));}
ps("\nWisteria ornamental vine market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;wis_market(lc,160+(i*15),16+(i*3),(i%7)+2,(i%4)+2,21+(i%3),4+(i%2));}
ps("\n");wis_report();wis_state();ps("\n=== Demo Complete ===\n");return 0;}
