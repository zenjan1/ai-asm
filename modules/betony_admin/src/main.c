/* betony_admin: Betony (Common Betony) perennial herb management (v1.0)
 * Betony planning, planting, evaluation, cutting, market
 * Features: flower tube, leaf shape, stem height, flower color, scent strength, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,flower_tb,leaf_shp,stem_ht,flower_color,scent_st,bloom_wk,active;} bt_t;
typedef struct{int n_plan,n_exec,n_eval,n_cut,n_mkt,t_flower,t_leaf,t_stem,t_color,t_scent;} bt_state_t;
static bt_t btps[N],btss[N-2],btvss[N-4],btct[N-6],btms[N-6]; static bt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bt_t*a,int*cnt,int*sum,int mx,int lc,int ft,int ls,int sh,int fc,int ss,int bw){if(*cnt>=mx)return -1;bt_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->flower_tb=ft;x->leaf_shp=ls;x->stem_ht=sh;x->flower_color=fc;x->scent_st=ss;x->bloom_wk=bw;x->active=1;*sum+=ft;(*cnt)++;ps("[BTN] Betony ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ft=");pi(ft);ps(" ls=");pi(ls);ps(" sh=");pi(sh);ps(" fc=");pi(fc);ps(" ss=");pi(ss);ps("\n");return *cnt-1;}
int bt_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_cut=0;st.n_mkt=0;st.t_flower=0;st.t_leaf=0;st.t_stem=0;st.t_color=0;st.t_scent=0;for(int i=0;i<N;i++)btps[i].active=0;for(int i=0;i<N-2;i++)btss[i].active=0;for(int i=0;i<N-4;i++)btvss[i].active=0;for(int i=0;i<N-6;i++)btct[i].active=0;for(int i=0;i<N-6;i++)btms[i].active=0;init=1;ps("[BTN] Betony initialized\n");return 0;}
/* 1=herb_garden 2=cottage_garden 3=woodland_edge 4=medicinal 5=container */
int bt_planning(int lc,int ft,int ls,int sh,int fc,int ss,int bw){return add(btps,&st.n_plan,&st.t_flower,N,lc,ft,ls,sh,fc,ss,bw);}
int bt_execution(int lc,int ft,int ls,int sh,int fc,int ss,int bw){return add(btss,&st.n_exec,&st.t_leaf,N-2,lc,ft,ls,sh,fc,ss,bw);}
int bt_evaluation(int lc,int ft,int ls,int sh,int fc,int ss,int bw){return add(btvss,&st.n_eval,&st.t_stem,N-4,lc,ft,ls,sh,fc,ss,bw);}
int bt_cutting(int lc,int ft,int ls,int sh,int fc,int ss,int bw){return add(btct,&st.n_cut,&st.t_color,N-6,lc,ft,ls,sh,fc,ss,bw);}
int bt_market(int lc,int ft,int ls,int sh,int fc,int ss,int bw){return add(btms,&st.n_mkt,&st.t_scent,N-6,lc,ft,ls,sh,fc,ss,bw);}
void bt_report(void){ps("[BTN] Plan: ");pi(st.n_plan);ps(" flower=");pi(st.t_flower);ps("\nExec: ");pi(st.n_exec);ps(" leaf=");pi(st.t_leaf);ps("\nEval: ");pi(st.n_eval);ps(" stem=");pi(st.t_stem);ps("\nCut: ");pi(st.n_cut);ps(" color=");pi(st.t_color);ps("\nMkt: ");pi(st.n_mkt);ps(" scent=");pi(st.t_scent);ps("\n");}
void bt_state(void){ps("[BTN] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Cut=");pi(st.n_cut);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Betony (Common Betony) Admin Demo ===\n\n");bt_init();
ps("Betony planning (herb garden layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;bt_planning(lc,10+(i*3),3+(i%4),20+(i*5),(i%5)+1,45+(i*5),22+(i%6));}
ps("\nBetony execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;bt_execution(lc,12+(i*2),4+(i%3),22+(i*4),(i%5)+1,50+(i*4),24+(i%5));}
ps("\nBetony evaluation (growth check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;bt_evaluation(lc,14+(i*2),5+(i%3),25+(i*3),(i%4)+2,55+(i%3),26+(i%4));}
ps("\nBetony cutting...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;bt_cutting(lc,8+(i*3),2+(i%4),18+(i*5),(i%5)+1,40+(i%5),20+(i%5));}
ps("\nBetony perennial herb market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;bt_market(lc,16+(i*2),6+(i%2),(i%6)+2,60+(i%3),28+(i%3),55+(i%4));}
ps("\n");bt_report();bt_state();ps("\n=== Demo Complete ===\n");return 0;}
