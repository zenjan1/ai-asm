/* beardtongue_admin: Beardtongue (Penstemon) pollinator garden management (v1.0)
 * Beardtongue planning, planting, evaluation, deadheading, market
 * Features: flower tube, stem height, flower color, leaf width, drought tolerance, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,flower_tb,stem_ht,flower_color,leaf_wd,drought_tl,bloom_wk,active;} beat_t;
typedef struct{int n_plan,n_exec,n_eval,n_dead,n_mkt,t_flower,t_stem,t_color,t_leaf,t_drought;} beat_state_t;
static beat_t beatps[N],beates[N-2],beatvs[N-4],beatdh[N-6],beatms[N-6]; static beat_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(beat_t*a,int*cnt,int*sum,int mx,int lc,int ft,int sh,int fc,int lw,int dt,int bw){if(*cnt>=mx)return -1;beat_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->flower_tb=ft;x->stem_ht=sh;x->flower_color=fc;x->leaf_wd=lw;x->drought_tl=dt;x->bloom_wk=bw;x->active=1;*sum+=ft;(*cnt)++;ps("[BEAT] Beardtongue ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ft=");pi(ft);ps(" sh=");pi(sh);ps(" fc=");pi(fc);ps(" lw=");pi(lw);ps(" dt=");pi(dt);ps("\n");return *cnt-1;}
int beat_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_dead=0;st.n_mkt=0;st.t_flower=0;st.t_stem=0;st.t_color=0;st.t_leaf=0;st.t_drought=0;for(int i=0;i<N;i++)beatps[i].active=0;for(int i=0;i<N-2;i++)beates[i].active=0;for(int i=0;i<N-4;i++)beatvs[i].active=0;for(int i=0;i<N-6;i++)beatdh[i].active=0;for(int i=0;i<N-6;i++)beatms[i].active=0;init=1;ps("[BEAT] Beardtongue initialized\n");return 0;}
/* 1=pollinator_garden 2=xeriscape 3=border 4=rock_garden 5=meadow */
int beat_planning(int lc,int ft,int sh,int fc,int lw,int dt,int bw){return add(beatps,&st.n_plan,&st.t_flower,N,lc,ft,sh,fc,lw,dt,bw);}
int beat_execution(int lc,int ft,int sh,int fc,int lw,int dt,int bw){return add(beates,&st.n_exec,&st.t_stem,N-2,lc,ft,sh,fc,lw,dt,bw);}
int beat_evaluation(int lc,int ft,int sh,int fc,int lw,int dt,int bw){return add(beatvs,&st.n_eval,&st.t_color,N-4,lc,ft,sh,fc,lw,dt,bw);}
int beat_deadheading(int lc,int ft,int sh,int fc,int lw,int dt,int bw){return add(beatdh,&st.n_dead,&st.t_leaf,N-6,lc,ft,sh,fc,lw,dt,bw);}
int beat_market(int lc,int ft,int sh,int fc,int lw,int dt,int bw){return add(beatms,&st.n_mkt,&st.t_drought,N-6,lc,ft,sh,fc,lw,dt,bw);}
void beat_report(void){ps("[BEAT] Plan: ");pi(st.n_plan);ps(" flower=");pi(st.t_flower);ps("\nExec: ");pi(st.n_exec);ps(" stem=");pi(st.t_stem);ps("\nEval: ");pi(st.n_eval);ps(" color=");pi(st.t_color);ps("\nDead: ");pi(st.n_dead);ps(" leaf=");pi(st.t_leaf);ps("\nMkt: ");pi(st.n_mkt);ps(" drought=");pi(st.t_drought);ps("\n");}
void beat_state(void){ps("[BEAT] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Dead=");pi(st.n_dead);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Beardtongue (Penstemon) Admin Demo ===\n\n");beat_init();
ps("Beardtongue planning (pollinator layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;beat_planning(lc,15+(i*4),30+(i*6),(i%7)+1,4+(i%4),60+(i*4),22+(i%6));}
ps("\nBeardtongue execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;beat_execution(lc,18+(i*3),35+(i*5),(i%7)+1,5+(i%3),65+(i*3),24+(i%5));}
ps("\nBeardtongue evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;beat_evaluation(lc,20+(i*3),40+(i*4),(i%6)+2,6+(i%3),70+(i%3),26+(i%4));}
ps("\nBeardtongue deadheading...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;beat_deadheading(lc,12+(i*4),25+(i*6),(i%5)+1,3+(i%4),55+(i%4),20+(i%5));}
ps("\nBeardtongue pollinator flower market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;beat_market(lc,22+(i*3),45+(i*4),(i%6)+2,7+(i%3),75+(i%3),28+(i%3));}
ps("\n");beat_report();beat_state();ps("\n=== Demo Complete ===\n");return 0;}
