/* bergenia_admin: Bergenia (Elephant Ears) evergreen groundcover management (v1.0)
 * Bergenia planning, planting, evaluation, division, market
 * Features: leaf length, leaf width, flower cluster, flower color, winter color, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,leaf_ln,leaf_wd,flower_cl,flower_color,winter_cl,bloom_wk,active;} berg_t;
typedef struct{int n_plan,n_exec,n_eval,n_div,n_mkt,t_leaf,t_width,t_flower,t_color,t_winter;} berg_state_t;
static berg_t bergps[N],berges[N-2],bergvs[N-4],bergdv[N-6],bergms[N-6]; static berg_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(berg_t*a,int*cnt,int*sum,int mx,int lc,int ll,int lw,int fc,int fcc,int wc,int bw){if(*cnt>=mx)return -1;berg_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->leaf_ln=ll;x->leaf_wd=lw;x->flower_cl=fc;x->flower_color=fcc;x->winter_cl=wc;x->bloom_wk=bw;x->active=1;*sum+=ll;(*cnt)++;ps("[BERG] Bergenia ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ll=");pi(ll);ps(" lw=");pi(lw);ps(" fc=");pi(fc);ps(" fcc=");pi(fcc);ps(" wc=");pi(wc);ps("\n");return *cnt-1;}
int berg_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_div=0;st.n_mkt=0;st.t_leaf=0;st.t_width=0;st.t_flower=0;st.t_color=0;st.t_winter=0;for(int i=0;i<N;i++)bergps[i].active=0;for(int i=0;i<N-2;i++)berges[i].active=0;for(int i=0;i<N-4;i++)bergvs[i].active=0;for(int i=0;i<N-6;i++)bergdv[i].active=0;for(int i=0;i<N-6;i++)bergms[i].active=0;init=1;ps("[BERG] Bergenia initialized\n");return 0;}
/* 1=shade_border 2=woodland 3=rock_garden 4=container 5=evergreen_bed */
int berg_planning(int lc,int ll,int lw,int fc,int fcc,int wc,int bw){return add(bergps,&st.n_plan,&st.t_leaf,N,lc,ll,lw,fc,fcc,wc,bw);}
int berg_execution(int lc,int ll,int lw,int fc,int fcc,int wc,int bw){return add(berges,&st.n_exec,&st.t_width,N-2,lc,ll,lw,fc,fcc,wc,bw);}
int berg_evaluation(int lc,int ll,int lw,int fc,int fcc,int wc,int bw){return add(bergvs,&st.n_eval,&st.t_flower,N-4,lc,ll,lw,fc,fcc,wc,bw);}
int berg_division(int lc,int ll,int lw,int fc,int fcc,int wc,int bw){return add(bergdv,&st.n_div,&st.t_color,N-6,lc,ll,lw,fc,fcc,wc,bw);}
int berg_market(int lc,int ll,int lw,int fc,int fcc,int wc,int bw){return add(bergms,&st.n_mkt,&st.t_winter,N-6,lc,ll,lw,fc,fcc,wc,bw);}
void berg_report(void){ps("[BERG] Plan: ");pi(st.n_plan);ps(" leaf=");pi(st.t_leaf);ps("\nExec: ");pi(st.n_exec);ps(" width=");pi(st.t_width);ps("\nEval: ");pi(st.n_eval);ps(" flower=");pi(st.t_flower);ps("\nDiv: ");pi(st.n_div);ps(" color=");pi(st.t_color);ps("\nMkt: ");pi(st.n_mkt);ps(" winter=");pi(st.t_winter);ps("\n");}
void berg_state(void){ps("[BERG] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Div=");pi(st.n_div);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Bergenia (Elephant Ears) Admin Demo ===\n\n");berg_init();
ps("Bergenia planning (shade border layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;berg_planning(lc,15+(i*4),10+(i%5),5+(i*3),(i%5)+1,40+(i*5),14+(i%6));}
ps("\nBergenia execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;berg_execution(lc,18+(i*3),12+(i%4),6+(i*2),(i%5)+1,45+(i*4),16+(i%5));}
ps("\nBergenia evaluation (flower check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;berg_evaluation(lc,20+(i*3),14+(i%3),7+(i*2),(i%4)+2,50+(i%3),18+(i%4));}
ps("\nBergenia division...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;berg_division(lc,12+(i*4),8+(i%5),4+(i*3),(i%5)+1,35+(i%5),12+(i%5));}
ps("\nBergenia evergreen groundcover market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;berg_market(lc,22+(i*3),16+(i%2),8+(i%2),(i%4)+2,55+(i%3),20+(i%3));}
ps("\n");berg_report();berg_state();ps("\n=== Demo Complete ===\n");return 0;}
