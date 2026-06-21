/* campanula_admin: Campanula management technology administration (v1.0)
 * Campanula planning, campanula execution, campanula evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cu_t;
typedef struct{int n_cup,n_cue,n_cuv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cu_state_t;
static cu_t cups[N],cuss[N-2],cuvss[N-4],cuas[N-6],cumks[N-6]; static cu_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cu_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cu_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CPN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cpn_init(void){if(init)return -1;st.n_cup=0;st.n_cue=0;st.n_cuv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cups[i].active=0;for(int i=0;i<N-2;i++)cuss[i].active=0;for(int i=0;i<N-4;i++)cuvss[i].active=0;for(int i=0;i<N-6;i++)cuas[i].active=0;for(int i=0;i<N-6;i++)cumks[i].active=0;init=1;ps("[CPN] Campanula initialized\n");return 0;}
int cpn_planning(int t,int c,int a,int b,int d,int e,int y){return add(cups,&st.n_cup,&st.t_f1,N,t,c,a,b,d,e,y);}
int cpn_execution(int t,int c,int a,int b,int d,int e,int y){return add(cuss,&st.n_cue,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cpn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cuvss,&st.n_cuv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cpn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cuas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cpn_market(int t,int c,int a,int b,int d,int e,int y){return add(cumks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cpn_report(void){ps("[CPN] Cup: ");pi(st.n_cup);ps(" PCS=");pi(st.t_f1);ps("\nCue: ");pi(st.n_cue);ps(" PCS=");pi(st.t_f2);ps("\nCuv: ");pi(st.n_cuv);ps(" PCS=");pi(st.t_f3);ps("\nCuc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cpn_state(void){ps("[CPN] Cup=");pi(st.n_cup);ps(" Cue=");pi(st.n_cue);ps(" Cuv=");pi(st.n_cuv);ps(" Cuc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Campanula Admin Demo ===\n\n");cpn_init();
ps("Campanula planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cpn_planning(t,c,724+(i*17),713+(i*14),693+(i*10),675+(i*6),2020+(i%5));}
ps("\nCampanula execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cpn_execution(t,c,713+(i*15),702+(i*12),684+(i*8),671+(i*5),2021+(i%4));}
ps("\nCampanula evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cpn_evaluation(t,c,705+(i*13),694+(i*10),678+(i*7),667+(i*4),2022+(i%3));}
ps("\nCampanula accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cpn_accessory(t,c,697+(i*11),688+(i*9),674+(i*6),664+(i*3),2023+(i%2));}
ps("\nCampanula marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cpn_market(t,c,691+(i*9),682+(i*7),669+(i*5),661+(i*3),2024);}
ps("\n");cpn_report();cpn_state();ps("\n=== Demo Complete ===\n");return 0;}
