/* ballota_admin: Ballota (Black Horehound) herbaceous perennial management (v1.0)
 * Ballota planning, planting, evaluation, cutting, market
 * Features: flower tube, leaf width, stem height, scent strength, soil pH, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,flower_tb,leaf_wd,stem_ht,scent_st,soil_ph,bloom_wk,active;} ball_t;
typedef struct{int n_plan,n_exec,n_eval,n_cut,n_mkt,t_flower,t_leaf,t_stem,t_scent,t_soil;} ball_state_t;
static ball_t ballps[N],balles[N-2],ballvs[N-4],ballct[N-6],ballms[N-6]; static ball_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ball_t*a,int*cnt,int*sum,int mx,int lc,int ft,int lw,int sh,int ss,int sph,int bw){if(*cnt>=mx)return -1;ball_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->flower_tb=ft;x->leaf_wd=lw;x->stem_ht=sh;x->scent_st=ss;x->soil_ph=sph;x->bloom_wk=bw;x->active=1;*sum+=ft;(*cnt)++;ps("[BALL] Ballota ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ft=");pi(ft);ps(" lw=");pi(lw);ps(" sh=");pi(sh);ps(" ss=");pi(ss);ps(" sph=");pi(sph);ps("\n");return *cnt-1;}
int ball_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_cut=0;st.n_mkt=0;st.t_flower=0;st.t_leaf=0;st.t_stem=0;st.t_scent=0;st.t_soil=0;for(int i=0;i<N;i++)ballps[i].active=0;for(int i=0;i<N-2;i++)balles[i].active=0;for(int i=0;i<N-4;i++)ballvs[i].active=0;for(int i=0;i<N-6;i++)ballct[i].active=0;for(int i=0;i<N-6;i++)ballms[i].active=0;init=1;ps("[BALL] Ballota initialized\n");return 0;}
/* 1=herb_garden 2=woodland_edge 3=waste_ground 4=container 5=hedge_base */
int ball_planning(int lc,int ft,int lw,int sh,int ss,int sph,int bw){return add(ballps,&st.n_plan,&st.t_flower,N,lc,ft,lw,sh,ss,sph,bw);}
int ball_execution(int lc,int ft,int lw,int sh,int ss,int sph,int bw){return add(balles,&st.n_exec,&st.t_leaf,N-2,lc,ft,lw,sh,ss,sph,bw);}
int ball_evaluation(int lc,int ft,int lw,int sh,int ss,int sph,int bw){return add(ballvs,&st.n_eval,&st.t_stem,N-4,lc,ft,lw,sh,ss,sph,bw);}
int ball_cutting(int lc,int ft,int lw,int sh,int ss,int sph,int bw){return add(ballct,&st.n_cut,&st.t_scent,N-6,lc,ft,lw,sh,ss,sph,bw);}
int ball_market(int lc,int ft,int lw,int sh,int ss,int sph,int bw){return add(ballms,&st.n_mkt,&st.t_soil,N-6,lc,ft,lw,sh,ss,sph,bw);}
void ball_report(void){ps("[BALL] Plan: ");pi(st.n_plan);ps(" flower=");pi(st.t_flower);ps("\nExec: ");pi(st.n_exec);ps(" leaf=");pi(st.t_leaf);ps("\nEval: ");pi(st.n_eval);ps(" stem=");pi(st.t_stem);ps("\nCut: ");pi(st.n_cut);ps(" scent=");pi(st.t_scent);ps("\nMkt: ");pi(st.n_mkt);ps(" soil=");pi(st.t_soil);ps("\n");}
void ball_state(void){ps("[BALL] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Cut=");pi(st.n_cut);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Ballota (Black Horehound) Admin Demo ===\n\n");ball_init();
ps("Ballota planning (herb garden layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;ball_planning(lc,10+(i*3),4+(i%4),25+(i*6),50+(i*5),60+(i%5),24+(i%6));}
ps("\nBallota execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;ball_execution(lc,12+(i*2),5+(i%3),28+(i*5),55+(i*4),62+(i%4),26+(i%5));}
ps("\nBallota evaluation (growth check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;ball_evaluation(lc,14+(i*2),6+(i%3),32+(i*4),58+(i*3),64+(i%3),28+(i%4));}
ps("\nBallota cutting...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;ball_cutting(lc,8+(i*3),3+(i%4),22+(i*6),45+(i*5),58+(i%5),22+(i%5));}
ps("\nBallota herbaceous perennial market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;ball_market(lc,16+(i*2),7+(i%3),(i%6)+2,65+(i*3),68+(i%3),30+(i%3));}
ps("\n");ball_report();ball_state();ps("\n=== Demo Complete ===\n");return 0;}
