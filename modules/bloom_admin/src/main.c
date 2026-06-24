/* bloom_admin: Bloom (Flowering Plant) ornamental garden management (v1.0)
 * Bloom planning, planting, evaluation, deadheading, market
 * Features: petal count, bloom diameter, stem height, flower color, fragrance level, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,petal_ct,bloom_dia,stem_ht,flower_color,frag_lv,bloom_wk,active;} blm_t;
typedef struct{int n_plan,n_exec,n_eval,n_dead,n_mkt,t_petal,t_bloom,t_stem,t_color,t_frag;} blm_state_t;
static blm_t blmps[N],blmes[N-2],blmvs[N-4],blmdh[N-6],blmms[N-6]; static blm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(blm_t*a,int*cnt,int*sum,int mx,int lc,int pc,int bd,int sh,int fc,int fl,int bw){if(*cnt>=mx)return -1;blm_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->petal_ct=pc;x->bloom_dia=bd;x->stem_ht=sh;x->flower_color=fc;x->frag_lv=fl;x->bloom_wk=bw;x->active=1;*sum+=pc;(*cnt)++;ps("[BLM] Bloom ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" pc=");pi(pc);ps(" bd=");pi(bd);ps(" sh=");pi(sh);ps(" fc=");pi(fc);ps(" fl=");pi(fl);ps(" bw=");pi(bw);ps("\n");return *cnt-1;}
int blm_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_dead=0;st.n_mkt=0;st.t_petal=0;st.t_bloom=0;st.t_stem=0;st.t_color=0;st.t_frag=0;for(int i=0;i<N;i++)blmps[i].active=0;for(int i=0;i<N-2;i++)blmes[i].active=0;for(int i=0;i<N-4;i++)blmvs[i].active=0;for(int i=0;i<N-6;i++)blmdh[i].active=0;for(int i=0;i<N-6;i++)blmms[i].active=0;init=1;ps("[BLM] Bloom initialized\n");return 0;}
int blm_planning(int lc,int pc,int bd,int sh,int fc,int fl,int bw){return add(blmps,&st.n_plan,&st.t_petal,N,lc,pc,bd,sh,fc,fl,bw);}
int blm_execution(int lc,int pc,int bd,int sh,int fc,int fl,int bw){return add(blmes,&st.n_exec,&st.t_bloom,N-2,lc,pc,bd,sh,fc,fl,bw);}
int blm_evaluation(int lc,int pc,int bd,int sh,int fc,int fl,int bw){return add(blmvs,&st.n_eval,&st.t_stem,N-4,lc,pc,bd,sh,fc,fl,bw);}
int blm_deadheading(int lc,int pc,int bd,int sh,int fc,int fl,int bw){return add(blmdh,&st.n_dead,&st.t_color,N-6,lc,pc,bd,sh,fc,fl,bw);}
int blm_market(int lc,int pc,int bd,int sh,int fc,int fl,int bw){return add(blmms,&st.n_mkt,&st.t_frag,N-6,lc,pc,bd,sh,fc,fl,bw);}
void blm_report(void){ps("[BLM] Plan: ");pi(st.n_plan);ps(" Petals=");pi(st.t_petal);ps("\nExec: ");pi(st.n_exec);ps(" Blooms=");pi(st.t_bloom);ps("\nEval: ");pi(st.n_eval);ps(" Stems=");pi(st.t_stem);ps("\nDead: ");pi(st.n_dead);ps(" Colors=");pi(st.t_color);ps("\nMkt: ");pi(st.n_mkt);ps(" Frag=");pi(st.t_frag);ps("\n");}
void blm_state(void){ps("[BLM] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Dead=");pi(st.n_dead);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Bloom Admin Demo ===\n\n");blm_init();
/* 1=garden_bed 2=border 3=container 4=hanging_basket 5=window_box */
ps("Bloom planning...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,pc=8+(i*3),bd=5+(i*2),sh=25+(i*5),fc=(i%5)+1,fl=3+(i*2),bw=24+(i%6);blm_planning(lc,pc,bd,sh,fc,fl,bw);}
ps("\nBloom execution...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,pc=10+(i*2),bd=6+(i*2),sh=28+(i*4),fc=(i%5)+1,fl=4+(i*2),bw=26+(i*5);blm_execution(lc,pc,bd,sh,fc,fl,bw);}
ps("\nBloom evaluation...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,pc=12+(i*2),bd=7+(i*2),sh=30+(i*3),fc=(i%4)+2,fl=5+(i*2),bw=28+(i*4);blm_evaluation(lc,pc,bd,sh,fc,fl,bw);}
ps("\nBloom deadheading...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,pc=6+(i*3),bd=4+(i*2),sh=22+(i*5),fc=(i%5)+1,fl=2+(i*2),bw=22+(i*5);blm_deadheading(lc,pc,bd,sh,fc,fl,bw);}
ps("\nBloom market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,pc=14+(i*2),bd=8+(i*2),sh=32+(i*3),fc=(i%3)+3,fl=6+(i*2),bw=30+(i*3);blm_market(lc,pc,bd,sh,fc,fl,bw);}
ps("\n");blm_report();blm_state();ps("\n=== Demo Complete ===\n");return 0;}
