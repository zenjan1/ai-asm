/* adonis_admin: Adonis (Pheasant's Eye) bright annual flower and woodland plant management (v1.0)
 * Adonis planning, planting, evaluation, seed collection, market
 * Features: petal count, flower color, bloom period, seed head formation, height growth, light requirement
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,petal_ct,flower_color,bloom_wk,seed_head,ht_growth,light_req,active;} ado_t;
typedef struct{int n_plan,n_exec,n_eval,n_seed,n_mkt,t_petal,t_color,t_bloom,t_seed_h,t_height;} ado_state_t;
static ado_t adops[N],adoes[N-2],adovs[N-4],adosd[N-6],adoms[N-6]; static ado_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ado_t*a,int*cnt,int*sum,int mx,int lc,int pc,int fc,int bw,int sh,int hg,int lr){if(*cnt>=mx)return -1;ado_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->petal_ct=pc;x->flower_color=fc;x->bloom_wk=bw;x->seed_head=sh;x->ht_growth=hg;x->light_req=lr;x->active=1;*sum+=pc;(*cnt)++;ps("[ADO] Adonis ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" pc=");pi(pc);ps(" fc=");pi(fc);ps(" bw=");pi(bw);ps(" sh=");pi(sh);ps(" hg=");pi(hg);ps("\n");return *cnt-1;}
int ado_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_seed=0;st.n_mkt=0;st.t_petal=0;st.t_color=0;st.t_bloom=0;st.t_seed_h=0;st.t_height=0;for(int i=0;i<N;i++)adops[i].active=0;for(int i=0;i<N-2;i++)adoes[i].active=0;for(int i=0;i<N-4;i++)adovs[i].active=0;for(int i=0;i<N-6;i++)adosd[i].active=0;for(int i=0;i<N-6;i++)adoms[i].active=0;init=1;ps("[ADO] Adonis (pheasant's eye) initialized\n");return 0;}
/* 1=woodland 2=meadow 3=rock_garden 4=cottage_garden 5=wildflower */
int ado_planning(int lc,int pc,int fc,int bw,int sh,int hg,int lr){return add(adops,&st.n_plan,&st.t_petal,N,lc,pc,fc,bw,sh,hg,lr);}
int ado_execution(int lc,int pc,int fc,int bw,int sh,int hg,int lr){return add(adoes,&st.n_exec,&st.t_color,N-2,lc,pc,fc,bw,sh,hg,lr);}
int ado_evaluation(int lc,int pc,int fc,int bw,int sh,int hg,int lr){return add(adovs,&st.n_eval,&st.t_bloom,N-4,lc,pc,fc,bw,sh,hg,lr);}
int ado_seed_collect(int lc,int pc,int fc,int bw,int sh,int hg,int lr){return add(adosd,&st.n_seed,&st.t_seed_h,N-6,lc,pc,fc,bw,sh,hg,lr);}
int ado_market(int lc,int pc,int fc,int bw,int sh,int hg,int lr){return add(adoms,&st.n_mkt,&st.t_height,N-6,lc,pc,fc,bw,sh,hg,lr);}
void ado_report(void){ps("[ADO] Plan: ");pi(st.n_plan);ps(" petal=");pi(st.t_petal);ps("\nExec: ");pi(st.n_exec);ps(" color=");pi(st.t_color);ps("\nEval: ");pi(st.n_eval);ps(" bloom=");pi(st.t_bloom);ps("\nSeed: ");pi(st.n_seed);ps(" seed_h=");pi(st.t_seed_h);ps("\nMkt: ");pi(st.n_mkt);ps(" height=");pi(st.t_height);ps("\n");}
void ado_state(void){ps("[ADO] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Seed=");pi(st.n_seed);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Adonis (Pheasant's Eye) Admin Demo ===\n\n");ado_init();
ps("Adonis planning (bright layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;ado_planning(lc,8+(i%4),(i%5)+1,12+(i%5),(i*3)+5,15+(i*4),(i%4)+1);}
ps("\nAdonis execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;ado_execution(lc,9+(i%3),(i%5)+1,13+(i%4),(i*3)+6,18+(i*3),(i%4)+1);}
ps("\nAdonis evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;ado_evaluation(lc,10+(i%3),(i%4)+2,14+(i%3),(i*2)+8,20+(i*3),(i%3)+2);}
ps("\nAdonis seed collection...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;ado_seed_collect(lc,7+(i%3),(i%4)+1,11+(i%4),(i*2)+4,12+(i*3),(i%4)+1);}
ps("\nAdonis flower market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;ado_market(lc,11+(i%2),(i%5)+2,15+(i%3),(i*2)+10,22+(i*2),(i%3)+2);}
ps("\n");ado_report();ado_state();ps("\n=== Demo Complete ===\n");return 0;}
