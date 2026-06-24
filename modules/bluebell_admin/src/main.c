/* bluebell_admin: Bluebell (Hyacinthoides) woodland spring flower management (v1.0)
 * Bluebell planning, planting, evaluation, bulb division, market
 * Features: bell length, leaf width, stem height, flower color, bulb size, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bell_ln,leaf_wd,stem_ht,flower_color,bulb_sz,bloom_wk,active;} blb_t;
typedef struct{int n_plan,n_exec,n_eval,n_div,n_mkt,t_bell,t_leaf,t_stem,t_color,t_bulb;} blb_state_t;
static blb_t blbps[N],blbes[N-2],blbvs[N-4],blbdv[N-6],blbms[N-6]; static blb_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(blb_t*a,int*cnt,int*sum,int mx,int lc,int bl,int lw,int sh,int fc,int bs,int bw){if(*cnt>=mx)return -1;blb_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bell_ln=bl;x->leaf_wd=lw;x->stem_ht=sh;x->flower_color=fc;x->bulb_sz=bs;x->bloom_wk=bw;x->active=1;*sum+=bl;(*cnt)++;ps("[BLB] Bluebell ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" lw=");pi(lw);ps(" sh=");pi(sh);ps(" fc=");pi(fc);ps(" bs=");pi(bs);ps("\n");return *cnt-1;}
int blb_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_div=0;st.n_mkt=0;st.t_bell=0;st.t_leaf=0;st.t_stem=0;st.t_color=0;st.t_bulb=0;for(int i=0;i<N;i++)blbps[i].active=0;for(int i=0;i<N-2;i++)blbes[i].active=0;for(int i=0;i<N-4;i++)blbvs[i].active=0;for(int i=0;i<N-6;i++)blbdv[i].active=0;for(int i=0;i<N-6;i++)blbms[i].active=0;init=1;ps("[BLB] Bluebell initialized\n");return 0;}
/* 1=woodland 2=shade_garden 3=border 4=naturalized 5=container */
int blb_planning(int lc,int bl,int lw,int sh,int fc,int bs,int bw){return add(blbps,&st.n_plan,&st.t_bell,N,lc,bl,lw,sh,fc,bs,bw);}
int blb_execution(int lc,int bl,int lw,int sh,int fc,int bs,int bw){return add(blbes,&st.n_exec,&st.t_leaf,N-2,lc,bl,lw,sh,fc,bs,bw);}
int blb_evaluation(int lc,int bl,int lw,int sh,int fc,int bs,int bw){return add(blbvs,&st.n_eval,&st.t_stem,N-4,lc,bl,lw,sh,fc,bs,bw);}
int blb_bulb_division(int lc,int bl,int lw,int sh,int fc,int bs,int bw){return add(blbdv,&st.n_div,&st.t_color,N-6,lc,bl,lw,sh,fc,bs,bw);}
int blb_market(int lc,int bl,int lw,int sh,int fc,int bs,int bw){return add(blbms,&st.n_mkt,&st.t_bulb,N-6,lc,bl,lw,sh,fc,bs,bw);}
void blb_report(void){ps("[BLB] Plan: ");pi(st.n_plan);ps(" bell=");pi(st.t_bell);ps("\nExec: ");pi(st.n_exec);ps(" leaf=");pi(st.t_leaf);ps("\nEval: ");pi(st.n_eval);ps(" stem=");pi(st.t_stem);ps("\nDiv: ");pi(st.n_div);ps(" color=");pi(st.t_color);ps("\nMkt: ");pi(st.n_mkt);ps(" bulb=");pi(st.t_bulb);ps("\n");}
void blb_state(void){ps("[BLB] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Div=");pi(st.n_div);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Bluebell (Hyacinthoides) Admin Demo ===\n\n");blb_init();
ps("Bluebell planning (woodland layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;blb_planning(lc,15+(i*3),8+(i*2),20+(i*5),(i%4)+1,3+(i*2),18+(i%6));}
ps("\nBluebell execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;blb_execution(lc,17+(i*2),9+(i*2),22+(i*4),(i%4)+1,4+(i*2),20+(i%5));}
ps("\nBluebell evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;blb_evaluation(lc,19+(i*2),10+(i*2),24+(i*3),(i%3)+2,5+(i*2),22+(i%4));}
ps("\nBluebell bulb division...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;blb_bulb_division(lc,13+(i*3),7+(i*2),18+(i*4),(i%4)+1,2+(i*2),16+(i%5));}
ps("\nBluebell spring flower market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;blb_market(lc,21+(i*2),11+(i*2),26+(i*3),(i%3)+3,6+(i*2),24+(i%3));}
ps("\n");blb_report();blb_state();ps("\n=== Demo Complete ===\n");return 0;}
