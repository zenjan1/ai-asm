/* bluecurls_admin: Bluecurls (Borago) borage herb management (v1.0)
 * Bluecurls planning, planting, evaluation, harvesting, market
 * Features: flower diameter, leaf length, stem height, flower color, hair density, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,flower_dia,leaf_ln,stem_ht,flower_color,hair_dt,bloom_wk,active;} blcu_t;
typedef struct{int n_plan,n_exec,n_eval,n_harv,n_mkt,t_flower,t_leaf,t_stem,t_color,t_hair;} blcu_state_t;
static blcu_t blcups[N],blcues[N-2],blcuvs[N-4],blcuhr[N-6],blcums[N-6]; static blcu_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(blcu_t*a,int*cnt,int*sum,int mx,int lc,int fd,int ll,int sh,int fc,int hd,int bw){if(*cnt>=mx)return -1;blcu_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->flower_dia=fd;x->leaf_ln=ll;x->stem_ht=sh;x->flower_color=fc;x->hair_dt=hd;x->bloom_wk=bw;x->active=1;*sum+=fd;(*cnt)++;ps("[BLCU] Bluecurls ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" fd=");pi(fd);ps(" ll=");pi(ll);ps(" sh=");pi(sh);ps(" fc=");pi(fc);ps(" hd=");pi(hd);ps("\n");return *cnt-1;}
int blcu_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_harv=0;st.n_mkt=0;st.t_flower=0;st.t_leaf=0;st.t_stem=0;st.t_color=0;st.t_hair=0;for(int i=0;i<N;i++)blcups[i].active=0;for(int i=0;i<N-2;i++)blcues[i].active=0;for(int i=0;i<N-4;i++)blcuvs[i].active=0;for(int i=0;i<N-6;i++)blcuhr[i].active=0;for(int i=0;i<N-6;i++)blcums[i].active=0;init=1;ps("[BLCU] Bluecurls initialized\n");return 0;}
/* 1=herb_garden 2=medicinal 3=kitchen 4=companion 5=wildflower */
int blcu_planning(int lc,int fd,int ll,int sh,int fc,int hd,int bw){return add(blcups,&st.n_plan,&st.t_flower,N,lc,fd,ll,sh,fc,hd,bw);}
int blcu_execution(int lc,int fd,int ll,int sh,int fc,int hd,int bw){return add(blcues,&st.n_exec,&st.t_leaf,N-2,lc,fd,ll,sh,fc,hd,bw);}
int blcu_evaluation(int lc,int fd,int ll,int sh,int fc,int hd,int bw){return add(blcuvs,&st.n_eval,&st.t_stem,N-4,lc,fd,ll,sh,fc,hd,bw);}
int blcu_harvesting(int lc,int fd,int ll,int sh,int fc,int hd,int bw){return add(blcuhr,&st.n_harv,&st.t_color,N-6,lc,fd,ll,sh,fc,hd,bw);}
int blcu_market(int lc,int fd,int ll,int sh,int fc,int hd,int bw){return add(blcums,&st.n_mkt,&st.t_hair,N-6,lc,fd,ll,sh,fc,hd,bw);}
void blcu_report(void){ps("[BLCU] Plan: ");pi(st.n_plan);ps(" flower=");pi(st.t_flower);ps("\nExec: ");pi(st.n_exec);ps(" leaf=");pi(st.t_leaf);ps("\nEval: ");pi(st.n_eval);ps(" stem=");pi(st.t_stem);ps("\nHarv: ");pi(st.n_harv);ps(" color=");pi(st.t_color);ps("\nMkt: ");pi(st.n_mkt);ps(" hair=");pi(st.t_hair);ps("\n");}
void blcu_state(void){ps("[BLCU] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Bluecurls (Borago) Admin Demo ===\n\n");blcu_init();
ps("Bluecurls planning (herb garden layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;blcu_planning(lc,2+(i*1),10+(i*3),40+(i*6),(i%4)+1,20+(i*5),22+(i%6));}
ps("\nBluecurls execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;blcu_execution(lc,3+(i*1),12+(i*2),45+(i*5),(i%4)+1,22+(i*4),24+(i%5));}
ps("\nBluecurls evaluation (herb check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;blcu_evaluation(lc,4+(i*1),14+(i*2),50+(i*4),(i%3)+2,24+(i*3),26+(i%4));}
ps("\nBluecurls harvesting...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;blcu_harvesting(lc,2+(i*1),8+(i*3),35+(i*6),(i%4)+1,18+(i*5),20+(i%5));}
ps("\nBluecurls market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;blcu_market(lc,5+(i*1),16+(i*2),55+(i*3),(i%3)+3,26+(i*3),28+(i%3));}
ps("\n");blcu_report();blcu_state();ps("\n=== Demo Complete ===\n");return 0;}
