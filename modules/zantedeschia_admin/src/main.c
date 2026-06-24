/* zantedeschia_admin: Zantedeschia (Calla Lily) elegant flower and rhizome plant management (v1.0)
 * Zantedeschia planning, planting, evaluation, division, market
 * Features: spathe count, flower color, bloom period, leaf pattern, rhizome size, moisture need
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,spathe_ct,flower_color,bloom_wk,leaf_pat,rhizome_sz,moisture_nd,active;} ztd_t;
typedef struct{int n_plan,n_exec,n_eval,n_div,n_mkt,t_spathe,t_color,t_bloom,t_leaf,t_rhizome;} ztd_state_t;
static ztd_t ztdps[N],ztdes[N-2],ztdvs[N-4],ztddv[N-6],ztdms[N-6]; static ztd_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ztd_t*a,int*cnt,int*sum,int mx,int lc,int sc,int fc,int bw,int lp,int rs,int mn){if(*cnt>=mx)return -1;ztd_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->spathe_ct=sc;x->flower_color=fc;x->bloom_wk=bw;x->leaf_pat=lp;x->rhizome_sz=rs;x->moisture_nd=mn;x->active=1;*sum+=sc;(*cnt)++;ps("[ZTD] Zantedeschia ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" sc=");pi(sc);ps(" fc=");pi(fc);ps(" bw=");pi(bw);ps(" lp=");pi(lp);ps(" rs=");pi(rs);ps("\n");return *cnt-1;}
int ztd_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_div=0;st.n_mkt=0;st.t_spathe=0;st.t_color=0;st.t_bloom=0;st.t_leaf=0;st.t_rhizome=0;for(int i=0;i<N;i++)ztdps[i].active=0;for(int i=0;i<N-2;i++)ztdes[i].active=0;for(int i=0;i<N-4;i++)ztdvs[i].active=0;for(int i=0;i<N-6;i++)ztddv[i].active=0;for(int i=0;i<N-6;i++)ztdms[i].active=0;init=1;ps("[ZTD] Zantedeschia (calla lily) initialized\n");return 0;}
/* 1=pond_edge 2=container 3=bog_garden 4=shade_border 5=cutting_garden */
int ztd_planning(int lc,int sc,int fc,int bw,int lp,int rs,int mn){return add(ztdps,&st.n_plan,&st.t_spathe,N,lc,sc,fc,bw,lp,rs,mn);}
int ztd_execution(int lc,int sc,int fc,int bw,int lp,int rs,int mn){return add(ztdes,&st.n_exec,&st.t_color,N-2,lc,sc,fc,bw,lp,rs,mn);}
int ztd_evaluation(int lc,int sc,int fc,int bw,int lp,int rs,int mn){return add(ztdvs,&st.n_eval,&st.t_bloom,N-4,lc,sc,fc,bw,lp,rs,mn);}
int ztd_division(int lc,int sc,int fc,int bw,int lp,int rs,int mn){return add(ztddv,&st.n_div,&st.t_leaf,N-6,lc,sc,fc,bw,lp,rs,mn);}
int ztd_market(int lc,int sc,int fc,int bw,int lp,int rs,int mn){return add(ztdms,&st.n_mkt,&st.t_rhizome,N-6,lc,sc,fc,bw,lp,rs,mn);}
void ztd_report(void){ps("[ZTD] Plan: ");pi(st.n_plan);ps(" spathe=");pi(st.t_spathe);ps("\nExec: ");pi(st.n_exec);ps(" color=");pi(st.t_color);ps("\nEval: ");pi(st.n_eval);ps(" bloom=");pi(st.t_bloom);ps("\nDiv: ");pi(st.n_div);ps(" leaf=");pi(st.t_leaf);ps("\nMkt: ");pi(st.n_mkt);ps(" rhizome=");pi(st.t_rhizome);ps("\n");}
void ztd_state(void){ps("[ZTD] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Div=");pi(st.n_div);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Zantedeschia (Calla Lily) Admin Demo ===\n\n");ztd_init();
ps("Zantedeschia planning (elegant layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;ztd_planning(lc,5+(i*2),(i%6)+1,20+(i%6),(i%4)+1,8+(i*3),(i%5)+1);}
ps("\nZantedeschia execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;ztd_execution(lc,6+(i*2),(i%6)+1,22+(i%5),(i%4)+1,10+(i*2),(i%5)+1);}
ps("\nZantedeschia evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;ztd_evaluation(lc,7+(i*2),(i%5)+2,24+(i%4),(i%3)+2,12+(i*2),(i%4)+2);}
ps("\nZantedeschia rhizome division...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;ztd_division(lc,4+(i*2),(i%4)+1,18+(i%5),(i%4)+1,6+(i*3),(i%5)+1);}
ps("\nZantedeschia cut flower market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;ztd_market(lc,8+(i*2),(i%6)+2,26+(i%3),(i%3)+2,14+(i*2),(i%4)+2);}
ps("\n");ztd_report();ztd_state();ps("\n=== Demo Complete ===\n");return 0;}
