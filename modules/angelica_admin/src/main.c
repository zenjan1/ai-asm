/* angelica_admin: Angelica (Wild Celery) medicinal herb and aromatic plant management (v1.0)
 * Angelica planning, planting, evaluation, root harvest, market
 * Features: stem height, leaf spread, root weight, compound content, bloom year, harvest month
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,stem_ht,leaf_sp,root_wt,compound_ct,bloom_yr,harv_mo,active;} agl_t;
typedef struct{int n_plan,n_exec,n_eval,n_root,n_mkt,t_stem,t_leaf,t_root_w,t_compound,t_bloom;} agl_state_t;
static agl_t aglps[N],agles[N-2],aglvs[N-4],aglrv[N-6],aglms[N-6]; static agl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(agl_t*a,int*cnt,int*sum,int mx,int lc,int sh,int ls,int rw,int cc,int by,int hm){if(*cnt>=mx)return -1;agl_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->stem_ht=sh;x->leaf_sp=ls;x->root_wt=rw;x->compound_ct=cc;x->bloom_yr=by;x->harv_mo=hm;x->active=1;*sum+=sh;(*cnt)++;ps("[AGL] Angelica ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" sh=");pi(sh);ps(" ls=");pi(ls);ps(" rw=");pi(rw);ps(" cc=");pi(cc);ps(" by=");pi(by);ps("\n");return *cnt-1;}
int agl_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_root=0;st.n_mkt=0;st.t_stem=0;st.t_leaf=0;st.t_root_w=0;st.t_compound=0;st.t_bloom=0;for(int i=0;i<N;i++)aglps[i].active=0;for(int i=0;i<N-2;i++)agles[i].active=0;for(int i=0;i<N-4;i++)aglvs[i].active=0;for(int i=0;i<N-6;i++)aglrv[i].active=0;for(int i=0;i<N-6;i++)aglms[i].active=0;init=1;ps("[AGL] Angelica initialized\n");return 0;}
/* 1=herb_garden 2=medicinal 3=woodland 4=border 5=container */
int agl_planning(int lc,int sh,int ls,int rw,int cc,int by,int hm){return add(aglps,&st.n_plan,&st.t_stem,N,lc,sh,ls,rw,cc,by,hm);}
int agl_execution(int lc,int sh,int ls,int rw,int cc,int by,int hm){return add(agles,&st.n_exec,&st.t_leaf,N-2,lc,sh,ls,rw,cc,by,hm);}
int agl_evaluation(int lc,int sh,int ls,int rw,int cc,int by,int hm){return add(aglvs,&st.n_eval,&st.t_root_w,N-4,lc,sh,ls,rw,cc,by,hm);}
int agl_root_harvest(int lc,int sh,int ls,int rw,int cc,int by,int hm){return add(aglrv,&st.n_root,&st.t_compound,N-6,lc,sh,ls,rw,cc,by,hm);}
int agl_market(int lc,int sh,int ls,int rw,int cc,int by,int hm){return add(aglms,&st.n_mkt,&st.t_bloom,N-6,lc,sh,ls,rw,cc,by,hm);}
void agl_report(void){ps("[AGL] Plan: ");pi(st.n_plan);ps(" stem=");pi(st.t_stem);ps("\nExec: ");pi(st.n_exec);ps(" leaf=");pi(st.t_leaf);ps("\nEval: ");pi(st.n_eval);ps(" root=");pi(st.t_root_w);ps("\nRoot: ");pi(st.n_root);ps(" compound=");pi(st.t_compound);ps("\nMkt: ");pi(st.n_mkt);ps(" bloom=");pi(st.t_bloom);ps("\n");}
void agl_state(void){ps("[AGL] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Root=");pi(st.n_root);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Angelica (Wild Celery) Admin Demo ===\n\n");agl_init();
ps("Angelica planning (herb layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;agl_planning(lc,80+(i*15),25+(i*6),100+(i*20),12+(i*4),2+(i%3),6+(i%4));}
ps("\nAngelica execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;agl_execution(lc,90+(i*12),30+(i*5),120+(i*15),15+(i*3),3+(i%2),7+(i%3));}
ps("\nAngelica evaluation (growth check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;agl_evaluation(lc,100+(i*10),35+(i*4),140+(i*12),18+(i*3),3+(i%2),8+(i%2));}
ps("\nAngelica root harvest...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;agl_root_harvest(lc,70+(i*12),20+(i*5),80+(i*18),10+(i*4),2+(i%3),5+(i%4));}
ps("\nAngelica medicinal market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;agl_market(lc,110+(i*8),40+(i*3),160+(i*10),20+(i*3),4+(i%2),9+(i%2));}
ps("\n");agl_report();agl_state();ps("\n=== Demo Complete ===\n");return 0;}
