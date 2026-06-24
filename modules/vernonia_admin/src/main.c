/* vernonia_admin: Vernonia (Ironweed) prairie wildflower and pollinator plant management (v1.0)
 * Vernonia planning, planting, evaluation, seed collection, market
 * Features: bloom color intensity, stem height, pollinator attraction, seed set, drought tolerance
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bloom_ct,stem_ht,color_int,pollinator_at,seed_set,drought_tol,active;} vern_t;
typedef struct{int n_plan,n_exec,n_eval,n_seed,n_mkt,t_bloom,t_stem,t_color,t_pollinator,t_seed_s;} vern_state_t;
static vern_t vernps[N],vernes[N-2],vernvs[N-4],vernsd[N-6],vernms[N-6]; static vern_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(vern_t*a,int*cnt,int*sum,int mx,int lc,int bc,int sh,int ci,int pa,int ss,int dt){if(*cnt>=mx)return -1;vern_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bloom_ct=bc;x->stem_ht=sh;x->color_int=ci;x->pollinator_at=pa;x->seed_set=ss;x->drought_tol=dt;x->active=1;*sum+=bc;(*cnt)++;ps("[VERN] Vernonia ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bc=");pi(bc);ps(" sh=");pi(sh);ps(" ci=");pi(ci);ps(" pa=");pi(pa);ps(" ss=");pi(ss);ps("\n");return *cnt-1;}
int vern_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_seed=0;st.n_mkt=0;st.t_bloom=0;st.t_stem=0;st.t_color=0;st.t_pollinator=0;st.t_seed_s=0;for(int i=0;i<N;i++)vernps[i].active=0;for(int i=0;i<N-2;i++)vernes[i].active=0;for(int i=0;i<N-4;i++)vernvs[i].active=0;for(int i=0;i<N-6;i++)vernsd[i].active=0;for(int i=0;i<N-6;i++)vernms[i].active=0;init=1;ps("[VERN] Vernonia (ironweed) initialized\n");return 0;}
/* 1=prairie 2=meadow 3=roadside 4=garden_border 5=naturalized */
int vern_planning(int lc,int bc,int sh,int ci,int pa,int ss,int dt){return add(vernps,&st.n_plan,&st.t_bloom,N,lc,bc,sh,ci,pa,ss,dt);}
int vern_execution(int lc,int bc,int sh,int ci,int pa,int ss,int dt){return add(vernes,&st.n_exec,&st.t_stem,N-2,lc,bc,sh,ci,pa,ss,dt);}
int vern_evaluation(int lc,int bc,int sh,int ci,int pa,int ss,int dt){return add(vernvs,&st.n_eval,&st.t_color,N-4,lc,bc,sh,ci,pa,ss,dt);}
int vern_seed_collect(int lc,int bc,int sh,int ci,int pa,int ss,int dt){return add(vernsd,&st.n_seed,&st.t_pollinator,N-6,lc,bc,sh,ci,pa,ss,dt);}
int vern_market(int lc,int bc,int sh,int ci,int pa,int ss,int dt){return add(vernms,&st.n_mkt,&st.t_seed_s,N-6,lc,bc,sh,ci,pa,ss,dt);}
void vern_report(void){ps("[VERN] Plan: ");pi(st.n_plan);ps(" bloom=");pi(st.t_bloom);ps("\nExec: ");pi(st.n_exec);ps(" stem=");pi(st.t_stem);ps("\nEval: ");pi(st.n_eval);ps(" color=");pi(st.t_color);ps("\nSeed: ");pi(st.n_seed);ps(" poll=");pi(st.t_pollinator);ps("\nMkt: ");pi(st.n_mkt);ps(" seed=");pi(st.t_seed_s);ps("\n");}
void vern_state(void){ps("[VERN] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Seed=");pi(st.n_seed);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Vernonia (Ironweed) Admin Demo ===\n\n");vern_init();
ps("Vernonia planning (prairie survey)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;vern_planning(lc,20+(i*5),80+(i*10),(i%7)+1,15+(i*4),(i*8)+10,(i%4)+1);}
ps("\nVernonia execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;vern_execution(lc,25+(i*4),90+(i*8),(i%7)+1,18+(i*3),(i*7)+12,(i%4)+1);}
ps("\nVernonia evaluation (color assessment)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;vern_evaluation(lc,30+(i*3),100+(i*6),(i%7)+2,20+(i*3),(i*6)+15,(i%3)+2);}
ps("\nVernonia seed collection...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;vern_seed_collect(lc,15+(i*4),70+(i*8),(i%6)+1,12+(i*3),(i*5)+8,(i%4)+1);}
ps("\nVernonia pollinator garden market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;vern_market(lc,35+(i*3),110+(i*5),(i%7)+2,25+(i*2),(i*4)+20,(i%3)+2);}
ps("\n");vern_report();vern_state();ps("\n=== Demo Complete ===\n");return 0;}
