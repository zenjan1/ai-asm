/* blueflag_admin: Blueflag (Iris versicolor) water iris management (v1.0)
 * Blueflag planning, planting, evaluation, division, market
 * Features: flower height, leaf width, spadix length, flower color, rhizome size, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,flower_ht,leaf_wd,spadix_ln,flower_color,rhiz_sz,bloom_wk,active;} blf_t;
typedef struct{int n_plan,n_exec,n_eval,n_div,n_mkt,t_flower,t_leaf,t_spadix,t_color,t_rhiz;} blf_state_t;
static blf_t blfps[N],blfes[N-2],blfvs[N-4],blfdv[N-6],blfms[N-6]; static blf_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(blf_t*a,int*cnt,int*sum,int mx,int lc,int fh,int lw,int sl,int fc,int rs,int bw){if(*cnt>=mx)return -1;blf_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->flower_ht=fh;x->leaf_wd=lw;x->spadix_ln=sl;x->flower_color=fc;x->rhiz_sz=rs;x->bloom_wk=bw;x->active=1;*sum+=fh;(*cnt)++;ps("[BLF] Blueflag ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" fh=");pi(fh);ps(" lw=");pi(lw);ps(" sl=");pi(sl);ps(" fc=");pi(fc);ps(" rs=");pi(rs);ps("\n");return *cnt-1;}
int blf_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_div=0;st.n_mkt=0;st.t_flower=0;st.t_leaf=0;st.t_spadix=0;st.t_color=0;st.t_rhiz=0;for(int i=0;i<N;i++)blfps[i].active=0;for(int i=0;i<N-2;i++)blfes[i].active=0;for(int i=0;i<N-4;i++)blfvs[i].active=0;for(int i=0;i<N-6;i++)blfdv[i].active=0;for(int i=0;i<N-6;i++)blfms[i].active=0;init=1;ps("[BLF] Blueflag initialized\n");return 0;}
/* 1=wetland 2=pond_edge 3=rain_garden 4=bog 5=streamside */
int blf_planning(int lc,int fh,int lw,int sl,int fc,int rs,int bw){return add(blfps,&st.n_plan,&st.t_flower,N,lc,fh,lw,sl,fc,rs,bw);}
int blf_execution(int lc,int fh,int lw,int sl,int fc,int rs,int bw){return add(blfes,&st.n_exec,&st.t_leaf,N-2,lc,fh,lw,sl,fc,rs,bw);}
int blf_evaluation(int lc,int fh,int lw,int sl,int fc,int rs,int bw){return add(blfvs,&st.n_eval,&st.t_spadix,N-4,lc,fh,lw,sl,fc,rs,bw);}
int blf_division(int lc,int fh,int lw,int sl,int fc,int rs,int bw){return add(blfdv,&st.n_div,&st.t_color,N-6,lc,fh,lw,sl,fc,rs,bw);}
int blf_market(int lc,int fh,int lw,int sl,int fc,int rs,int bw){return add(blfms,&st.n_mkt,&st.t_rhiz,N-6,lc,fh,lw,sl,fc,rs,bw);}
void blf_report(void){ps("[BLF] Plan: ");pi(st.n_plan);ps(" flower=");pi(st.t_flower);ps("\nExec: ");pi(st.n_exec);ps(" leaf=");pi(st.t_leaf);ps("\nEval: ");pi(st.n_eval);ps(" spadix=");pi(st.t_spadix);ps("\nDiv: ");pi(st.n_div);ps(" color=");pi(st.t_color);ps("\nMkt: ");pi(st.n_mkt);ps(" rhiz=");pi(st.t_rhiz);ps("\n");}
void blf_state(void){ps("[BLF] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Div=");pi(st.n_div);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Blueflag (Iris versicolor) Admin Demo ===\n\n");blf_init();
ps("Blueflag planning (wetland layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;blf_planning(lc,25+(i*5),3+(i*2),20+(i*4),(i%4)+1,4+(i*2),22+(i%6));}
ps("\nBlueflag execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;blf_execution(lc,28+(i*4),4+(i*2),22+(i*3),(i%4)+1,5+(i*2),24+(i%5));}
ps("\nBlueflag evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;blf_evaluation(lc,30+(i*3),5+(i*2),24+(i*3),(i%3)+2,6+(i*2),26+(i%4));}
ps("\nBlueflag division...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;blf_division(lc,22+(i*5),2+(i*2),18+(i*4),(i%4)+1,3+(i*2),20+(i%5));}
ps("\nBlueflag market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;blf_market(lc,32+(i*3),6+(i*2),26+(i*3),(i%3)+3,7+(i*2),28+(i%3));}
ps("\n");blf_report();blf_state();ps("\n=== Demo Complete ===\n");return 0;}
