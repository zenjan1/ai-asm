/* fumitory_admin: Fumitory management technology administration (v1.0)
 * Fumitory planning, fumitory execution, fumitory evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} fum_t;
typedef struct{int n_fump,n_fume,n_fum2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} fum_state_t;
static fum_t fumps[N],fumss[N-2],fumvss[N-4],fumas[N-6],fum_mks[N-6]; static fum_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(fum_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;fum_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FUM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int fum_init(void){if(init)return -1;st.n_fump=0;st.n_fume=0;st.n_fum2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)fumps[i].active=0;for(int i=0;i<N-2;i++)fumss[i].active=0;for(int i=0;i<N-4;i++)fumvss[i].active=0;for(int i=0;i<N-6;i++)fumas[i].active=0;for(int i=0;i<N-6;i++)fum_mks[i].active=0;init=1;ps("[FUM] Fumitory initialized\n");return 0;}
int fum_planning(int t,int c,int a,int b,int d,int e,int y){return add(fumps,&st.n_fump,&st.t_f1,N,t,c,a,b,d,e,y);}
int fum_execution(int t,int c,int a,int b,int d,int e,int y){return add(fumss,&st.n_fume,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int fum_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(fumvss,&st.n_fum2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int fum_accessory(int t,int c,int a,int b,int d,int e,int y){return add(fumas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int fum_market(int t,int c,int a,int b,int d,int e,int y){return add(fum_mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void fum_report(void){ps("[FUM] Fump: ");pi(st.n_fump);ps(" PCS=");pi(st.t_f1);ps("\nFume: ");pi(st.n_fume);ps(" PCS=");pi(st.t_f2);ps("\nFumv: ");pi(st.n_fum2);ps(" PCS=");pi(st.t_f3);ps("\nFumc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void fum_state(void){ps("[FUM] Fump=");pi(st.n_fump);ps(" Fume=");pi(st.n_fume);ps(" Fumv=");pi(st.n_fum2);ps(" Fumc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Fumitory Admin Demo ===\n\n");fum_init();
ps("Fumitory planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;fum_planning(t,c,924+(i*17),913+(i*14),893+(i*10),875+(i*6),2020+(i%5));}
ps("\nFumitory execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;fum_execution(t,c,913+(i*15),902+(i*12),884+(i*8),871+(i*5),2021+(i%4));}
ps("\nFumitory evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;fum_evaluation(t,c,905+(i*13),894+(i*10),878+(i*7),867+(i*4),2022+(i%3));}
ps("\nFumitory accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fum_accessory(t,c,897+(i*11),888+(i*9),874+(i*6),864+(i*3),2023+(i%2));}
ps("\nFumitory marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fum_market(t,c,891+(i*9),882+(i*7),869+(i*5),861+(i*3),2024);}
ps("\n");fum_report();fum_state();ps("\n=== Demo Complete ===\n");return 0;}
