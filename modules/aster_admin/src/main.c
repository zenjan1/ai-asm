/* aster_admin: Aster (Michaelmas Daisy) autumn perennial management (v1.0)
 * Aster planning, planting, evaluation, division, market
 * Features: flower diameter, petal count, stem height, bloom color, leaf shape, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,flower_dia,petal_ct,stem_ht,bloom_color,leaf_shp,bloom_wk,active;} astr_t;
typedef struct{int n_plan,n_exec,n_eval,n_div,n_mkt,t_flower,t_petal,t_stem,t_color,t_leaf;} astr_state_t;
static astr_t astrps[N],astres[N-2],astrvs[N-4],astrdv[N-6],astrms[N-6]; static astr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(astr_t*a,int*cnt,int*sum,int mx,int lc,int fd,int pc,int sh,int bc,int ls,int bw){if(*cnt>=mx)return -1;astr_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->flower_dia=fd;x->petal_ct=pc;x->stem_ht=sh;x->bloom_color=bc;x->leaf_shp=ls;x->bloom_wk=bw;x->active=1;*sum+=fd;(*cnt)++;ps("[ASTR] Aster ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" fd=");pi(fd);ps(" pc=");pi(pc);ps(" sh=");pi(sh);ps(" bc=");pi(bc);ps(" ls=");pi(ls);ps("\n");return *cnt-1;}
int astr_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_div=0;st.n_mkt=0;st.t_flower=0;st.t_petal=0;st.t_stem=0;st.t_color=0;st.t_leaf=0;for(int i=0;i<N;i++)astrps[i].active=0;for(int i=0;i<N-2;i++)astres[i].active=0;for(int i=0;i<N-4;i++)astrvs[i].active=0;for(int i=0;i<N-6;i++)astrdv[i].active=0;for(int i=0;i<N-6;i++)astrms[i].active=0;init=1;ps("[ASTR] Aster initialized\n");return 0;}
/* 1=border 2=meadow 3=cutting 4=container 5=autumn_garden */
int astr_planning(int lc,int fd,int pc,int sh,int bc,int ls,int bw){return add(astrps,&st.n_plan,&st.t_flower,N,lc,fd,pc,sh,bc,ls,bw);}
int astr_execution(int lc,int fd,int pc,int sh,int bc,int ls,int bw){return add(astres,&st.n_exec,&st.t_petal,N-2,lc,fd,pc,sh,bc,ls,bw);}
int astr_evaluation(int lc,int fd,int pc,int sh,int bc,int ls,int bw){return add(astrvs,&st.n_eval,&st.t_stem,N-4,lc,fd,pc,sh,bc,ls,bw);}
int astr_division(int lc,int fd,int pc,int sh,int bc,int ls,int bw){return add(astrdv,&st.n_div,&st.t_color,N-6,lc,fd,pc,sh,bc,ls,bw);}
int astr_market(int lc,int fd,int pc,int sh,int bc,int ls,int bw){return add(astrms,&st.n_mkt,&st.t_leaf,N-6,lc,fd,pc,sh,bc,ls,bw);}
void astr_report(void){ps("[ASTR] Plan: ");pi(st.n_plan);ps(" flower=");pi(st.t_flower);ps("\nExec: ");pi(st.n_exec);ps(" petal=");pi(st.t_petal);ps("\nEval: ");pi(st.n_eval);ps(" stem=");pi(st.t_stem);ps("\nDiv: ");pi(st.n_div);ps(" color=");pi(st.t_color);ps("\nMkt: ");pi(st.n_mkt);ps(" leaf=");pi(st.t_leaf);ps("\n");}
void astr_state(void){ps("[ASTR] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Div=");pi(st.n_div);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Aster (Michaelmas Daisy) Admin Demo ===\n\n");astr_init();
ps("Aster planning (autumn layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;astr_planning(lc,5+(i*2),8+(i%5),40+(i*8),(i%7)+1,3+(i%3),32+(i%6));}
ps("\nAster execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;astr_execution(lc,6+(i*2),9+(i%4),45+(i*7),(i%7)+1,4+(i%3),34+(i%5));}
ps("\nAster evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;astr_evaluation(lc,7+(i*2),10+(i%3),50+(i*6),(i%6)+2,5+(i%2),36+(i%4));}
ps("\nAster division...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;astr_division(lc,4+(i*2),7+(i%4),(i%5)+1,3+(i%3),30+(i*6),(i%4)+1);}
ps("\nAster autumn flower market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;astr_market(lc,8+(i*2),11+(i%3),(i%6)+2,6+(i%2),55+(i*5),38+(i%3));}
ps("\n");astr_report();astr_state();ps("\n=== Demo Complete ===\n");return 0;}
