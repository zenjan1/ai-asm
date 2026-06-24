/* aronia_admin: Aronia (Chokeberry) fruit shrub management (v1.0)
 * Aronia planning, planting, evaluation, berry harvest, market
 * Features: berry cluster count, fruit weight, shrub height, leaf color, antioxidant level, harvest month
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,berry_cl,fruit_wt,shrub_ht,leaf_color,antiox_lv,harv_mo,active;} aro_t;
typedef struct{int n_plan,n_exec,n_eval,n_berry,n_mkt,t_berry_c,t_fruit,t_shrub,t_leaf,t_antiox;} aro_state_t;
static aro_t arops[N],aroes[N-2],arovs[N-4],arobv[N-6],aroms[N-6]; static aro_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(aro_t*a,int*cnt,int*sum,int mx,int lc,int bc,int fw,int sh,int lcc,int al,int hm){if(*cnt>=mx)return -1;aro_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->berry_cl=bc;x->fruit_wt=fw;x->shrub_ht=sh;x->leaf_color=lcc;x->antiox_lv=al;x->harv_mo=hm;x->active=1;*sum+=bc;(*cnt)++;ps("[ARO] Aronia ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bc=");pi(bc);ps(" fw=");pi(fw);ps(" sh=");pi(sh);ps(" lcc=");pi(lcc);ps(" al=");pi(al);ps("\n");return *cnt-1;}
int aro_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_berry=0;st.n_mkt=0;st.t_berry_c=0;st.t_fruit=0;st.t_shrub=0;st.t_leaf=0;st.t_antiox=0;for(int i=0;i<N;i++)arops[i].active=0;for(int i=0;i<N-2;i++)aroes[i].active=0;for(int i=0;i<N-4;i++)arovs[i].active=0;for(int i=0;i<N-6;i++)arobv[i].active=0;for(int i=0;i<N-6;i++)aroms[i].active=0;init=1;ps("[ARO] Aronia initialized\n");return 0;}
/* 1=fruit_farm 2=hedge 3=wildlife 4=container 5=ornamental */
int aro_planning(int lc,int bc,int fw,int sh,int lcc,int al,int hm){return add(arops,&st.n_plan,&st.t_berry_c,N,lc,bc,fw,sh,lcc,al,hm);}
int aro_execution(int lc,int bc,int fw,int sh,int lcc,int al,int hm){return add(aroes,&st.n_exec,&st.t_fruit,N-2,lc,bc,fw,sh,lcc,al,hm);}
int aro_evaluation(int lc,int bc,int fw,int sh,int lcc,int al,int hm){return add(arovs,&st.n_eval,&st.t_shrub,N-4,lc,bc,fw,sh,lcc,al,hm);}
int aro_berry_harvest(int lc,int bc,int fw,int sh,int lcc,int al,int hm){return add(arobv,&st.n_berry,&st.t_leaf,N-6,lc,bc,fw,sh,lcc,al,hm);}
int aro_market(int lc,int bc,int fw,int sh,int lcc,int al,int hm){return add(aroms,&st.n_mkt,&st.t_antiox,N-6,lc,bc,fw,sh,lcc,al,hm);}
void aro_report(void){ps("[ARO] Plan: ");pi(st.n_plan);ps(" berry=");pi(st.t_berry_c);ps("\nExec: ");pi(st.n_exec);ps(" fruit=");pi(st.t_fruit);ps("\nEval: ");pi(st.n_eval);ps(" shrub=");pi(st.t_shrub);ps("\nBerry: ");pi(st.n_berry);ps(" leaf=");pi(st.t_leaf);ps("\nMkt: ");pi(st.n_mkt);ps(" antiox=");pi(st.t_antiox);ps("\n");}
void aro_state(void){ps("[ARO] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Berry=");pi(st.n_berry);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Aronia (Chokeberry) Admin Demo ===\n\n");aro_init();
ps("Aronia planning (fruit shrub layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;aro_planning(lc,10+(i*3),5+(i*2),80+(i*12),(i%5)+1,200+(i*30),8+(i%3));}
ps("\nAronia execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;aro_execution(lc,12+(i*2),6+(i*2),90+(i*10),(i%5)+1,220+(i*25),9+(i%2));}
ps("\nAronia evaluation (growth check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;aro_evaluation(lc,14+(i*2),7+(i*2),100+(i*8),(i%4)+2,240+(i*20),9+(i%2));}
ps("\nAronia berry harvest...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;aro_berry_harvest(lc,8+(i*3),4+(i*2),70+(i*10),(i%4)+1,180+(i*25),7+(i%3));}
ps("\nAronia superfruit market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;aro_market(lc,16+(i*2),8+(i*2),110+(i*6),(i%5)+2,260+(i*15),10+(i%2));}
ps("\n");aro_report();aro_state();ps("\n=== Demo Complete ===\n");return 0;}
