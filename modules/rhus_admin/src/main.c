/* rhus_admin: Rhus management technology administration (v1.0)
 * Rhus planning, rhus execution, rhus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rhu_t;
typedef struct{int n_rhup,n_rhue,n_rhu2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} rhu_state_t;
static rhu_t rhups[N],rhuss[N-2],rhuvss[N-4],rhuas[N-6],rhumks[N-6]; static rhu_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rhu_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rhu_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RHU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rhu_init(void){if(init)return -1;st.n_rhup=0;st.n_rhue=0;st.n_rhu2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rhups[i].active=0;for(int i=0;i<N-2;i++)rhuss[i].active=0;for(int i=0;i<N-4;i++)rhuvss[i].active=0;for(int i=0;i<N-6;i++)rhuas[i].active=0;for(int i=0;i<N-6;i++)rhumks[i].active=0;init=1;ps("[RHU] Rhus initialized\n");return 0;}
int rhu_planning(int t,int c,int a,int b,int d,int e,int y){return add(rhups,&st.n_rhup,&st.t_f1,N,t,c,a,b,d,e,y);}
int rhu_execution(int t,int c,int a,int b,int d,int e,int y){return add(rhuss,&st.n_rhue,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rhu_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(rhuvss,&st.n_rhu2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rhu_accessory(int t,int c,int a,int b,int d,int e,int y){return add(rhuas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rhu_market(int t,int c,int a,int b,int d,int e,int y){return add(rhumks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rhu_report(void){ps("[RHU] rhup: ");pi(st.n_rhup);ps(" PCS=");pi(st.t_f1);ps("\nrhue: ");pi(st.n_rhue);ps(" PCS=");pi(st.t_f2);ps("\nrhuv: ");pi(st.n_rhu2);ps(" PCS=");pi(st.t_f3);ps("\nrhuc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void rhu_state(void){ps("[RHU] rhup=");pi(st.n_rhup);ps(" rhue=");pi(st.n_rhue);ps(" rhuv=");pi(st.n_rhu2);ps(" rhuc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Rhus Admin Demo ===\n\n");rhu_init();
ps("Rhus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rhu_planning(t,c,1093+(i*17),1082+(i*14),1062+(i*10),1044+(i*6),2020+(i%5));}
ps("\nRhus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rhu_execution(t,c,1082+(i*15),1071+(i*12),1053+(i*8),1040+(i*5),2021+(i%4));}
ps("\nRhus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rhu_evaluation(t,c,1074+(i*13),1063+(i*10),1047+(i*7),1036+(i*4),2022+(i%3));}
ps("\nRhus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rhu_accessory(t,c,1066+(i*11),1057+(i*9),1043+(i*6),1033+(i*3),2023+(i%2));}
ps("\nRhus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rhu_market(t,c,1060+(i*9),1051+(i*7),1038+(i*5),1030+(i*3),2024);}
ps("\n");rhu_report();rhu_state();ps("\n=== Demo Complete ===\n");return 0;}
