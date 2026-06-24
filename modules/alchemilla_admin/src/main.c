/* alchemilla_admin: Alchemilla (Lady's Mantle) medicinal herb and ornamental foliage plant management (v1.0)
 * Alchemilla planning, planting, evaluation, harvest, market
 * Features: leaf size, water droplet retention, flower cluster, medicinal compound, ground coverage
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,leaf_sz,water_drop,flower_clust,compound_mg,ground_cov,harvest_wk,active;} alc_t;
typedef struct{int n_plan,n_exec,n_eval,n_harv,n_mkt,t_leaf,t_water,t_flower,t_compound,t_ground;} alc_state_t;
static alc_t alcps[N],alces[N-2],alcvs[N-4],alchv[N-6],alcms[N-6]; static alc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(alc_t*a,int*cnt,int*sum,int mx,int lc,int ls,int wd,int fc,int cm,int gc,int hw){if(*cnt>=mx)return -1;alc_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->leaf_sz=ls;x->water_drop=wd;x->flower_clust=fc;x->compound_mg=cm;x->ground_cov=gc;x->harvest_wk=hw;x->active=1;*sum+=ls;(*cnt)++;ps("[ALC] Alchemilla ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ls=");pi(ls);ps(" wd=");pi(wd);ps(" fc=");pi(fc);ps(" cm=");pi(cm);ps(" gc=");pi(gc);ps("\n");return *cnt-1;}
int alc_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_harv=0;st.n_mkt=0;st.t_leaf=0;st.t_water=0;st.t_flower=0;st.t_compound=0;st.t_ground=0;for(int i=0;i<N;i++)alcps[i].active=0;for(int i=0;i<N-2;i++)alces[i].active=0;for(int i=0;i<N-4;i++)alcvs[i].active=0;for(int i=0;i<N-6;i++)alchv[i].active=0;for(int i=0;i<N-6;i++)alcms[i].active=0;init=1;ps("[ALC] Alchemilla (lady's mantle) initialized\n");return 0;}
/* 1=cottage_garden 2=medicinal 3=shade_border 4=edging 5=woodland */
int alc_planning(int lc,int ls,int wd,int fc,int cm,int gc,int hw){return add(alcps,&st.n_plan,&st.t_leaf,N,lc,ls,wd,fc,cm,gc,hw);}
int alc_execution(int lc,int ls,int wd,int fc,int cm,int gc,int hw){return add(alces,&st.n_exec,&st.t_water,N-2,lc,ls,wd,fc,cm,gc,hw);}
int alc_evaluation(int lc,int ls,int wd,int fc,int cm,int gc,int hw){return add(alcvs,&st.n_eval,&st.t_flower,N-4,lc,ls,wd,fc,cm,gc,hw);}
int alc_harvest(int lc,int ls,int wd,int fc,int cm,int gc,int hw){return add(alchv,&st.n_harv,&st.t_compound,N-6,lc,ls,wd,fc,cm,gc,hw);}
int alc_market(int lc,int ls,int wd,int fc,int cm,int gc,int hw){return add(alcms,&st.n_mkt,&st.t_ground,N-6,lc,ls,wd,fc,cm,gc,hw);}
void alc_report(void){ps("[ALC] Plan: ");pi(st.n_plan);ps(" leaf=");pi(st.t_leaf);ps("\nExec: ");pi(st.n_exec);ps(" water=");pi(st.t_water);ps("\nEval: ");pi(st.n_eval);ps(" flower=");pi(st.t_flower);ps("\nHarv: ");pi(st.n_harv);ps(" comp=");pi(st.t_compound);ps("\nMkt: ");pi(st.n_mkt);ps(" ground=");pi(st.t_ground);ps("\n");}
void alc_state(void){ps("[ALC] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Alchemilla (Lady's Mantle) Admin Demo ===\n\n");alc_init();
ps("Alchemilla planning (foliage layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;alc_planning(lc,30+(i*5),20+(i*8),8+(i*3),15+(i*4),(i*10)+20,18+(i%6));}
ps("\nAlchemilla execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;alc_execution(lc,35+(i*4),25+(i*6),9+(i*2),18+(i*3),(i*8)+25,19+(i%5));}
ps("\nAlchemilla evaluation (compound check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;alc_evaluation(lc,40+(i*3),30+(i*5),10+(i*2),20+(i*3),(i*6)+30,20+(i%4));}
ps("\nAlchemilla medicinal harvest...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;alc_harvest(lc,25+(i*4),15+(i*6),6+(i*3),12+(i*4),(i*8)+15,16+(i%5));}
ps("\nAlchemilla herbal market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;alc_market(lc,45+(i*3),35+(i*4),12+(i*2),22+(i*2),(i*6)+35,21+(i%3));}
ps("\n");alc_report();alc_state();ps("\n=== Demo Complete ===\n");return 0;}
