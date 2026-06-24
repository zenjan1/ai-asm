/* ranunculus_admin: Ranunculus buttercup cultivation administration (v1.0)
 * Ranunculus planning, ranunculus execution, ranunculus evaluation, corm propagation, flower marketing
 * Features: corm tracking, flower quality grading, forcing schedules, pest monitoring, color variety mgmt
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,cultivar,flower_dia,petals,stem_len,vase_life,color,grade,active;} ran_t;
typedef struct{int n_plan,n_exec,n_eval,n_corm,n_mkt,t_dia,t_petals,t_stem,t_vase,t_grade;} ran_state_t;
static ran_t ranps[N],ranes[N-2],ranvs[N-4],rancs[N-6],ranms[N-6]; static ran_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ran_t*a,int*cnt,int*sum,int mx,int cv,int dia,int pt,int sl,int vl,int cl,int gr){if(*cnt>=mx)return -1;ran_t*x=&a[*cnt];x->id=*cnt;x->cultivar=cv;x->flower_dia=dia;x->petals=pt;x->stem_len=sl;x->vase_life=vl;x->color=cl;x->grade=gr;x->active=1;*sum+=dia;(*cnt)++;ps("[RAN] Buttercup ");pi(*cnt-1);ps(" cv=");pi(cv);ps(" dia=");pi(dia);ps(" pt=");pi(pt);ps(" sl=");pi(sl);ps(" vl=");pi(vl);ps("\n");return *cnt-1;}
int ran_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_corm=0;st.n_mkt=0;st.t_dia=0;st.t_petals=0;st.t_stem=0;st.t_vase=0;st.t_grade=0;for(int i=0;i<N;i++)ranps[i].active=0;for(int i=0;i<N-2;i++)ranes[i].active=0;for(int i=0;i<N-4;i++)ranvs[i].active=0;for(int i=0;i<N-6;i++)rancs[i].active=0;for(int i=0;i<N-6;i++)ranms[i].active=0;init=1;ps("[RAN] Ranunculus buttercup initialized\n");return 0;}
/* Categories: 1=Bloomingdale 2=Super French 3=Butterfly 4=Ponpon 5=Magic */
int ran_planning(int cv,int dia,int pt,int sl,int vl,int cl,int gr){return add(ranps,&st.n_plan,&st.t_dia,N,cv,dia,pt,sl,vl,cl,gr);}
int ran_execution(int cv,int dia,int pt,int sl,int vl,int cl,int gr){return add(ranes,&st.n_exec,&st.t_petals,N-2,cv,dia,pt,sl,vl,cl,gr);}
int ran_evaluation(int cv,int dia,int pt,int sl,int vl,int cl,int gr){return add(ranvs,&st.n_eval,&st.t_stem,N-4,cv,dia,pt,sl,vl,cl,gr);}
int ran_corm(int cv,int dia,int pt,int sl,int vl,int cl,int gr){return add(rancs,&st.n_corm,&st.t_vase,N-6,cv,dia,pt,sl,vl,cl,gr);}
int ran_market(int cv,int dia,int pt,int sl,int vl,int cl,int gr){return add(ranms,&st.n_mkt,&st.t_grade,N-6,cv,dia,pt,sl,vl,cl,gr);}
void ran_report(void){ps("[RAN] Plan: ");pi(st.n_plan);ps(" dia_sum=");pi(st.t_dia);ps("\nExec: ");pi(st.n_exec);ps(" petals=");pi(st.t_petals);ps("\nEval: ");pi(st.n_eval);ps(" stem=");pi(st.t_stem);ps("\nCorm: ");pi(st.n_corm);ps(" vase=");pi(st.t_vase);ps("\nMkt: ");pi(st.n_mkt);ps(" grade=");pi(st.t_grade);ps("\n");}
void ran_state(void){ps("[RAN] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Corm=");pi(st.n_corm);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Ranunculus Buttercup Admin Demo ===\n\n");ran_init();
ps("Ranunculus planning (spring flowering)...\n");
for(int i=0;i<N;i++){int cv=(i%5)+1;ran_planning(cv,45+(i*3),18+(i%8),280+(i*12),9+(i%4),(i%6)+1,(i%3)+1);}
ps("\nRanunculus execution (greenhouse forcing)...\n");
for(int i=0;i<N-2;i++){int cv=(i%4)+2;ran_execution(cv,48+(i*3),20+(i%7),295+(i*10),10+(i%3),(i%5)+1,(i%3)+1);}
ps("\nRanunculus evaluation (quality check)...\n");
for(int i=0;i<N-4;i++){int cv=(i%3)+1;ran_evaluation(cv,50+(i*4),22+(i%6),310+(i*8),11+(i%3),(i%4)+2,(i%2)+2);}
ps("\nRanunculus corm propagation...\n");
for(int i=0;i<N-6;i++){int cv=(i%5)+1;ran_corm(cv,35+(i*2),15+(i%5),200+(i*10),0,(i%3)+1,1);}
ps("\nRanunculus marketing (wholesale)...\n");
for(int i=0;i<N-6;i++){int cv=(i%4)+1;ran_market(cv,52+(i*3),24+(i%6),320+(i*9),12+(i%3),(i%5)+1,(i%3)+1);}
ps("\n");ran_report();ran_state();ps("\n=== Demo Complete ===\n");return 0;}
