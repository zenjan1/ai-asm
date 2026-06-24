/* viola_admin: Viola (Violet) spring flower and woodland groundcover management (v1.0)
 * Viola planning, planting, evaluation, propagation, market
 * Features: petal count, flower color, bloom period, leaf shape, fragrance level, seed pod count
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,petal_ct,flower_color,bloom_wk,leaf_shp,fragrance,seed_pod,active;} vla_t;
typedef struct{int n_plan,n_exec,n_eval,n_prop,n_mkt,t_petal,t_color,t_bloom,t_leaf,t_fragrance;} vla_state_t;
static vla_t vlaps[N],vlaes[N-2],vlavs[N-4],vlapr[N-6],vlams[N-6]; static vla_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(vla_t*a,int*cnt,int*sum,int mx,int lc,int pc,int fc,int bw,int ls,int fg,int sp){if(*cnt>=mx)return -1;vla_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->petal_ct=pc;x->flower_color=fc;x->bloom_wk=bw;x->leaf_shp=ls;x->fragrance=fg;x->seed_pod=sp;x->active=1;*sum+=pc;(*cnt)++;ps("[VLA] Viola ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" pc=");pi(pc);ps(" fc=");pi(fc);ps(" bw=");pi(bw);ps(" ls=");pi(ls);ps(" fg=");pi(fg);ps("\n");return *cnt-1;}
int vla_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_prop=0;st.n_mkt=0;st.t_petal=0;st.t_color=0;st.t_bloom=0;st.t_leaf=0;st.t_fragrance=0;for(int i=0;i<N;i++)vlaps[i].active=0;for(int i=0;i<N-2;i++)vlaes[i].active=0;for(int i=0;i<N-4;i++)vlavs[i].active=0;for(int i=0;i<N-6;i++)vlapr[i].active=0;for(int i=0;i<N-6;i++)vlams[i].active=0;init=1;ps("[VLA] Viola (violet) initialized\n");return 0;}
/* 1=woodland 2=rock_garden 3=border 4=container 5=wildflower_meadow */
int vla_planning(int lc,int pc,int fc,int bw,int ls,int fg,int sp){return add(vlaps,&st.n_plan,&st.t_petal,N,lc,pc,fc,bw,ls,fg,sp);}
int vla_execution(int lc,int pc,int fc,int bw,int ls,int fg,int sp){return add(vlaes,&st.n_exec,&st.t_color,N-2,lc,pc,fc,bw,ls,fg,sp);}
int vla_evaluation(int lc,int pc,int fc,int bw,int ls,int fg,int sp){return add(vlavs,&st.n_eval,&st.t_bloom,N-4,lc,pc,fc,bw,ls,fg,sp);}
int vla_propagation(int lc,int pc,int fc,int bw,int ls,int fg,int sp){return add(vlapr,&st.n_prop,&st.t_leaf,N-6,lc,pc,fc,bw,ls,fg,sp);}
int vla_market(int lc,int pc,int fc,int bw,int ls,int fg,int sp){return add(vlams,&st.n_mkt,&st.t_fragrance,N-6,lc,pc,fc,bw,ls,fg,sp);}
void vla_report(void){ps("[VLA] Plan: ");pi(st.n_plan);ps(" petal=");pi(st.t_petal);ps("\nExec: ");pi(st.n_exec);ps(" color=");pi(st.t_color);ps("\nEval: ");pi(st.n_eval);ps(" bloom=");pi(st.t_bloom);ps("\nProp: ");pi(st.n_prop);ps(" leaf=");pi(st.t_leaf);ps("\nMkt: ");pi(st.n_mkt);ps(" frag=");pi(st.t_fragrance);ps("\n");}
void vla_state(void){ps("[VLA] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Prop=");pi(st.n_prop);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Viola (Violet) Admin Demo ===\n\n");vla_init();
ps("Viola planning (spring layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;vla_planning(lc,5+(i%3),(i%6)+1,10+(i%6),(i%4)+1,(i%7)+1,3+(i*2));}
ps("\nViola execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;vla_execution(lc,5+(i%3),(i%6)+1,11+(i%5),(i%4)+1,(i%7)+1,4+(i*2));}
ps("\nViola evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;vla_evaluation(lc,5+(i%2),(i%5)+2,12+(i%4),(i%3)+2,(i%6)+2,5+(i*2));}
ps("\nViola propagation (division)...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;vla_propagation(lc,4+(i%3),(i%4)+1,9+(i%5),(i%4)+1,(i%5)+1,2+(i*2));}
ps("\nViola flower market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;vla_market(lc,6+(i%2),(i%6)+2,13+(i%3),(i%3)+2,(i%7)+3,6+(i*2));}
ps("\n");vla_report();vla_state();ps("\n=== Demo Complete ===\n");return 0;}
