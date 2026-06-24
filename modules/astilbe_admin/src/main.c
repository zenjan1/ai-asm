/* astilbe_admin: Astilbe (False Spirea) shade perennial management (v1.0)
 * Astilbe planning, planting, evaluation, plume cutting, market
 * Features: plume height, plume width, flower color, leaf type, moisture need, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,plume_ht,plume_wd,flower_color,leaf_tp,moisture_nd,bloom_wk,active;} astb_t;
typedef struct{int n_plan,n_exec,n_eval,n_cut,n_mkt,t_plume,t_width,t_color,t_leaf,t_moist;} astb_state_t;
static astb_t astbps[N],astbes[N-2],astbvs[N-4],astbct[N-6],astbms[N-6]; static astb_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(astb_t*a,int*cnt,int*sum,int mx,int lc,int ph,int pw,int fc,int lt,int mn,int bw){if(*cnt>=mx)return -1;astb_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->plume_ht=ph;x->plume_wd=pw;x->flower_color=fc;x->leaf_tp=lt;x->moisture_nd=mn;x->bloom_wk=bw;x->active=1;*sum+=ph;(*cnt)++;ps("[ASTB] Astilbe ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ph=");pi(ph);ps(" pw=");pi(pw);ps(" fc=");pi(fc);ps(" lt=");pi(lt);ps(" mn=");pi(mn);ps("\n");return *cnt-1;}
int astb_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_cut=0;st.n_mkt=0;st.t_plume=0;st.t_width=0;st.t_color=0;st.t_leaf=0;st.t_moist=0;for(int i=0;i<N;i++)astbps[i].active=0;for(int i=0;i<N-2;i++)astbes[i].active=0;for(int i=0;i<N-4;i++)astbvs[i].active=0;for(int i=0;i<N-6;i++)astbct[i].active=0;for(int i=0;i<N-6;i++)astbms[i].active=0;init=1;ps("[ASTB] Astilbe initialized\n");return 0;}
/* 1=shade_garden 2=woodland 3=riparian 4=container 5=bog */
int astb_planning(int lc,int ph,int pw,int fc,int lt,int mn,int bw){return add(astbps,&st.n_plan,&st.t_plume,N,lc,ph,pw,fc,lt,mn,bw);}
int astb_execution(int lc,int ph,int pw,int fc,int lt,int mn,int bw){return add(astbes,&st.n_exec,&st.t_width,N-2,lc,ph,pw,fc,lt,mn,bw);}
int astb_evaluation(int lc,int ph,int pw,int fc,int lt,int mn,int bw){return add(astbvs,&st.n_eval,&st.t_color,N-4,lc,ph,pw,fc,lt,mn,bw);}
int astb_plume_cutting(int lc,int ph,int pw,int fc,int lt,int mn,int bw){return add(astbct,&st.n_cut,&st.t_leaf,N-6,lc,ph,pw,fc,lt,mn,bw);}
int astb_market(int lc,int ph,int pw,int fc,int lt,int mn,int bw){return add(astbms,&st.n_mkt,&st.t_moist,N-6,lc,ph,pw,fc,lt,mn,bw);}
void astb_report(void){ps("[ASTB] Plan: ");pi(st.n_plan);ps(" plume=");pi(st.t_plume);ps("\nExec: ");pi(st.n_exec);ps(" width=");pi(st.t_width);ps("\nEval: ");pi(st.n_eval);ps(" color=");pi(st.t_color);ps("\nCut: ");pi(st.n_cut);ps(" leaf=");pi(st.t_leaf);ps("\nMkt: ");pi(st.n_mkt);ps(" moist=");pi(st.t_moist);ps("\n");}
void astb_state(void){ps("[ASTB] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Cut=");pi(st.n_cut);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Astilbe (False Spirea) Admin Demo ===\n\n");astb_init();
ps("Astilbe planning (shade layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;astb_planning(lc,30+(i*6),8+(i*2),(i%7)+1,3+(i%3),60+(i*5),22+(i%6));}
ps("\nAstilbe execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;astb_execution(lc,35+(i*5),9+(i*2),(i%7)+1,4+(i%3),65+(i*4),24+(i%5));}
ps("\nAstilbe evaluation (plume check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;astb_evaluation(lc,40+(i*4),10+(i*2),(i%6)+2,5+(i%2),70+(i*3),26+(i%4));}
ps("\nAstilbe plume cutting...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;astb_plume_cutting(lc,25+(i*5),7+(i*2),(i%5)+1,3+(i%3),55+(i*4),20+(i%5));}
ps("\nAstilbe shade perennial market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;astb_market(lc,45+(i*3),11+(i*2),(i%6)+2,6+(i%2),75+(i*3),28+(i%3));}
ps("\n");astb_report();astb_state();ps("\n=== Demo Complete ===\n");return 0;}
