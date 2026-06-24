/* agapanthus_admin: Agapanthus (Lily of the Nile) ornamental bulb flower and border plant management (v1.0)
 * Agapanthus planning, planting, evaluation, division, market
 * Features: flower head count, stem height, bloom color, root clump size, drought tolerance, division freq
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,flower_head,stem_ht,bloom_color,root_clump,drought_tol,division_fr,active;} aga_t;
typedef struct{int n_plan,n_exec,n_eval,n_div,n_mkt,t_flower,t_stem,t_color,t_root,t_drought;} aga_state_t;
static aga_t agaps[N],agaes[N-2],agavs[N-4],agadv[N-6],agams[N-6]; static aga_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(aga_t*a,int*cnt,int*sum,int mx,int lc,int fh,int sh,int bc,int rc,int dt,int df){if(*cnt>=mx)return -1;aga_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->flower_head=fh;x->stem_ht=sh;x->bloom_color=bc;x->root_clump=rc;x->drought_tol=dt;x->division_fr=df;x->active=1;*sum+=fh;(*cnt)++;ps("[AGA] Agapanthus ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" fh=");pi(fh);ps(" sh=");pi(sh);ps(" bc=");pi(bc);ps(" rc=");pi(rc);ps(" dt=");pi(dt);ps("\n");return *cnt-1;}
int aga_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_div=0;st.n_mkt=0;st.t_flower=0;st.t_stem=0;st.t_color=0;st.t_root=0;st.t_drought=0;for(int i=0;i<N;i++)agaps[i].active=0;for(int i=0;i<N-2;i++)agaes[i].active=0;for(int i=0;i<N-4;i++)agavs[i].active=0;for(int i=0;i<N-6;i++)agadv[i].active=0;for(int i=0;i<N-6;i++)agams[i].active=0;init=1;ps("[AGA] Agapanthus (lily of the Nile) initialized\n");return 0;}
/* 1=border 2=container 3=mediterranean 4=coastal 5=drought_tolerant */
int aga_planning(int lc,int fh,int sh,int bc,int rc,int dt,int df){return add(agaps,&st.n_plan,&st.t_flower,N,lc,fh,sh,bc,rc,dt,df);}
int aga_execution(int lc,int fh,int sh,int bc,int rc,int dt,int df){return add(agaes,&st.n_exec,&st.t_stem,N-2,lc,fh,sh,bc,rc,dt,df);}
int aga_evaluation(int lc,int fh,int sh,int bc,int rc,int dt,int df){return add(agavs,&st.n_eval,&st.t_color,N-4,lc,fh,sh,bc,rc,dt,df);}
int aga_division(int lc,int fh,int sh,int bc,int rc,int dt,int df){return add(agadv,&st.n_div,&st.t_root,N-6,lc,fh,sh,bc,rc,dt,df);}
int aga_market(int lc,int fh,int sh,int bc,int rc,int dt,int df){return add(agams,&st.n_mkt,&st.t_drought,N-6,lc,fh,sh,bc,rc,dt,df);}
void aga_report(void){ps("[AGA] Plan: ");pi(st.n_plan);ps(" flower=");pi(st.t_flower);ps("\nExec: ");pi(st.n_exec);ps(" stem=");pi(st.t_stem);ps("\nEval: ");pi(st.n_eval);ps(" color=");pi(st.t_color);ps("\nDiv: ");pi(st.n_div);ps(" root=");pi(st.t_root);ps("\nMkt: ");pi(st.n_mkt);ps(" drought=");pi(st.t_drought);ps("\n");}
void aga_state(void){ps("[AGA] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Div=");pi(st.n_div);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Agapanthus (Lily of the Nile) Admin Demo ===\n\n");aga_init();
ps("Agapanthus planning (border layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;aga_planning(lc,10+(i*3),60+(i*10),(i%5)+1,20+(i*5),(i%5)+1,3+(i%3));}
ps("\nAgapanthus execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;aga_execution(lc,12+(i*2),65+(i*8),(i%5)+1,22+(i*4),(i%5)+1,3+(i%3));}
ps("\nAgapanthus evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;aga_evaluation(lc,14+(i*2),70+(i*7),(i%4)+2,25+(i*3),(i%4)+2,4+(i%2));}
ps("\nAgapanthus root division...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;aga_division(lc,8+(i*2),55+(i*6),(i%4)+1,18+(i*4),(i%5)+1,3+(i%3));}
ps("\nAgapanthus ornamental market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;aga_market(lc,16+(i*2),75+(i*5),(i%5)+2,28+(i*3),(i%4)+2,4+(i%2));}
ps("\n");aga_report();aga_state();ps("\n=== Demo Complete ===\n");return 0;}
