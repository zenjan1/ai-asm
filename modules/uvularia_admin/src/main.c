/* uvularia_admin: Uvularia (Bellwort) spring woodland wildflower and shade garden plant (v1.0)
 * Uvularia planning, planting, evaluation, propagation, market
 * Features: bloom timing, petal count, shade tolerance, root depth, woodland coverage, pollinator visits
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bloom_wk,petal_ct,shade_tol,root_dep,cover_sc,pollinator_v,active;} uva_t;
typedef struct{int n_plan,n_exec,n_eval,n_prop,n_mkt,t_bloom,t_petal,t_shade,t_root,t_cover;} uva_state_t;
static uva_t uvaps[N],uvaes[N-2],uvavs[N-4],uvapr[N-6],uvams[N-6]; static uva_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(uva_t*a,int*cnt,int*sum,int mx,int lc,int bw,int pc,int sh,int rd,int cs,int pv){if(*cnt>=mx)return -1;uva_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bloom_wk=bw;x->petal_ct=pc;x->shade_tol=sh;x->root_dep=rd;x->cover_sc=cs;x->pollinator_v=pv;x->active=1;*sum+=bw;(*cnt)++;ps("[UVA] Uvularia ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bw=");pi(bw);ps(" pc=");pi(pc);ps(" sh=");pi(sh);ps(" rd=");pi(rd);ps(" cs=");pi(cs);ps("\n");return *cnt-1;}
int uva_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_prop=0;st.n_mkt=0;st.t_bloom=0;st.t_petal=0;st.t_shade=0;st.t_root=0;st.t_cover=0;for(int i=0;i<N;i++)uvaps[i].active=0;for(int i=0;i<N-2;i++)uvaes[i].active=0;for(int i=0;i<N-4;i++)uvavs[i].active=0;for(int i=0;i<N-6;i++)uvapr[i].active=0;for(int i=0;i<N-6;i++)uvams[i].active=0;init=1;ps("[UVA] Uvularia (bellwort) initialized\n");return 0;}
/* 1=woodland 2=shade_garden 3=forest_edge 4=rock_garden 5=naturalized */
int uva_planning(int lc,int bw,int pc,int sh,int rd,int cs,int pv){return add(uvaps,&st.n_plan,&st.t_bloom,N,lc,bw,pc,sh,rd,cs,pv);}
int uva_execution(int lc,int bw,int pc,int sh,int rd,int cs,int pv){return add(uvaes,&st.n_exec,&st.t_petal,N-2,lc,bw,pc,sh,rd,cs,pv);}
int uva_evaluation(int lc,int bw,int pc,int sh,int rd,int cs,int pv){return add(uvavs,&st.n_eval,&st.t_shade,N-4,lc,bw,pc,sh,rd,cs,pv);}
int uva_propagation(int lc,int bw,int pc,int sh,int rd,int cs,int pv){return add(uvapr,&st.n_prop,&st.t_root,N-6,lc,bw,pc,sh,rd,cs,pv);}
int uva_market(int lc,int bw,int pc,int sh,int rd,int cs,int pv){return add(uvams,&st.n_mkt,&st.t_cover,N-6,lc,bw,pc,sh,rd,cs,pv);}
void uva_report(void){ps("[UVA] Plan: ");pi(st.n_plan);ps(" bloom=");pi(st.t_bloom);ps("\nExec: ");pi(st.n_exec);ps(" petal=");pi(st.t_petal);ps("\nEval: ");pi(st.n_eval);ps(" shade=");pi(st.t_shade);ps("\nProp: ");pi(st.n_prop);ps(" root=");pi(st.t_root);ps("\nMkt: ");pi(st.n_mkt);ps(" cover=");pi(st.t_cover);ps("\n");}
void uva_state(void){ps("[UVA] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Prop=");pi(st.n_prop);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Uvularia (Bellwort) Admin Demo ===\n\n");uva_init();
ps("Uvularia planning (woodland survey)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;uva_planning(lc,14+(i%6),6+(i%3),(i%4)+1,10+(i*3),(i*5)+10,(i%5)+1);}
ps("\nUvularia execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;uva_execution(lc,15+(i%5),6+(i%3),(i%4)+1,12+(i*2),(i*4)+12,(i%4)+2);}
ps("\nUvularia evaluation (shade assessment)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;uva_evaluation(lc,16+(i%4),6+(i%2),(i%3)+2,14+(i*2),(i*3)+15,(i%3)+2);}
ps("\nUvularia propagation (rhizome division)...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;uva_propagation(lc,13+(i%4),6+(i%2),(i%3)+1,8+(i*3),(i*4)+8,(i%4)+1);}
ps("\nUvularia woodland garden market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;uva_market(lc,17+(i%3),6+(i%2),(i%4)+2,15+(i*2),(i*3)+18,(i%3)+3);}
ps("\n");uva_report();uva_state();ps("\n=== Demo Complete ===\n");return 0;}
