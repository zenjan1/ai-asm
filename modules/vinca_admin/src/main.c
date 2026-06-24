/* vinca_admin: Vinca (Periwinkle) groundcover and trailing vine management (v1.0)
 * Vinca planning, planting, evaluation, trimming, market
 * Features: vine length tracking, flower count, ground coverage, shade tolerance, spread control
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,vine_len,flower_ct,cover_sc,shade_tol,spread_rt,trim_mo,active;} vnc_t;
typedef struct{int n_plan,n_exec,n_eval,n_trim,n_mkt,t_vine,t_flower,t_cover,t_shade,t_spread;} vnc_state_t;
static vnc_t vncps[N],vnces[N-2],vncvs[N-4],vnctm[N-6],vncms[N-6]; static vnc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(vnc_t*a,int*cnt,int*sum,int mx,int lc,int vl,int fc,int cs,int st_tol,int sr,int tm){if(*cnt>=mx)return -1;vnc_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->vine_len=vl;x->flower_ct=fc;x->cover_sc=cs;x->shade_tol=st_tol;x->spread_rt=sr;x->trim_mo=tm;x->active=1;*sum+=vl;(*cnt)++;ps("[VNC] Vinca ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" vl=");pi(vl);ps(" fc=");pi(fc);ps(" cs=");pi(cs);ps(" st=");pi(st_tol);ps(" sr=");pi(sr);ps("\n");return *cnt-1;}
int vnc_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_trim=0;st.n_mkt=0;st.t_vine=0;st.t_flower=0;st.t_cover=0;st.t_shade=0;st.t_spread=0;for(int i=0;i<N;i++)vncps[i].active=0;for(int i=0;i<N-2;i++)vnces[i].active=0;for(int i=0;i<N-4;i++)vncvs[i].active=0;for(int i=0;i<N-6;i++)vnctm[i].active=0;for(int i=0;i<N-6;i++)vncms[i].active=0;init=1;ps("[VNC] Vinca (periwinkle) initialized\n");return 0;}
/* 1=groundcover 2=hanging_basket 3=under_tree 4=slope 5=shade_border */
int vnc_planning(int lc,int vl,int fc,int cs,int st_tol,int sr,int tm){return add(vncps,&st.n_plan,&st.t_vine,N,lc,vl,fc,cs,st_tol,sr,tm);}
int vnc_execution(int lc,int vl,int fc,int cs,int st_tol,int sr,int tm){return add(vnces,&st.n_exec,&st.t_flower,N-2,lc,vl,fc,cs,st_tol,sr,tm);}
int vnc_evaluation(int lc,int vl,int fc,int cs,int st_tol,int sr,int tm){return add(vncvs,&st.n_eval,&st.t_cover,N-4,lc,vl,fc,cs,st_tol,sr,tm);}
int vnc_trimming(int lc,int vl,int fc,int cs,int st_tol,int sr,int tm){return add(vnctm,&st.n_trim,&st.t_shade,N-6,lc,vl,fc,cs,st_tol,sr,tm);}
int vnc_market(int lc,int vl,int fc,int cs,int st_tol,int sr,int tm){return add(vncms,&st.n_mkt,&st.t_spread,N-6,lc,vl,fc,cs,st_tol,sr,tm);}
void vnc_report(void){ps("[VNC] Plan: ");pi(st.n_plan);ps(" vine=");pi(st.t_vine);ps("\nExec: ");pi(st.n_exec);ps(" flower=");pi(st.t_flower);ps("\nEval: ");pi(st.n_eval);ps(" cover=");pi(st.t_cover);ps("\nTrim: ");pi(st.n_trim);ps(" shade=");pi(st.t_shade);ps("\nMkt: ");pi(st.n_mkt);ps(" spread=");pi(st.t_spread);ps("\n");}
void vnc_state(void){ps("[VNC] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Trim=");pi(st.n_trim);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Vinca (Periwinkle) Admin Demo ===\n\n");vnc_init();
ps("Vinca planning (groundcover layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;vnc_planning(lc,30+(i*10),5+(i*2),(i*8)+10,(i%4)+1,4+(i%3),(i%4)+1);}
ps("\nVinca execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;vnc_execution(lc,35+(i*8),6+(i*2),(i*8)+12,(i%4)+1,5+(i%2),(i%4)+1);}
ps("\nVinca evaluation (coverage check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;vnc_evaluation(lc,40+(i*7),7+(i*2),(i*7)+15,(i%3)+2,6+(i%2),(i%3)+2);}
ps("\nVinca trimming management...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;vnc_trimming(lc,25+(i*8),4+(i*2),(i*6)+8,(i%4)+1,3+(i%3),(i%4)+1);}
ps("\nVinca groundcover market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;vnc_market(lc,45+(i*6),8+(i*2),(i*8)+18,(i%3)+2,7+(i%2),(i%3)+2);}
ps("\n");vnc_report();vnc_state();ps("\n=== Demo Complete ===\n");return 0;}
