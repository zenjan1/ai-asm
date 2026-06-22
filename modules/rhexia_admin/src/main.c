/* rhexia_admin: Rhexia management technology administration (v1.0)
 * Rhexia planning, rhexia execution, rhexia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rhe_t;
typedef struct{int n_rhep,n_rhee,n_rhe2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} rhe_state_t;
static rhe_t rheps[N],rhess[N-2],rhevss[N-4],rheas[N-6],rhemks[N-6]; static rhe_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rhe_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rhe_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RHE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rhe_init(void){if(init)return -1;st.n_rhep=0;st.n_rhee=0;st.n_rhe2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rheps[i].active=0;for(int i=0;i<N-2;i++)rhess[i].active=0;for(int i=0;i<N-4;i++)rhevss[i].active=0;for(int i=0;i<N-6;i++)rheas[i].active=0;for(int i=0;i<N-6;i++)rhemks[i].active=0;init=1;ps("[RHE] Rhexia initialized\n");return 0;}
int rhe_planning(int t,int c,int a,int b,int d,int e,int y){return add(rheps,&st.n_rhep,&st.t_f1,N,t,c,a,b,d,e,y);}
int rhe_execution(int t,int c,int a,int b,int d,int e,int y){return add(rhess,&st.n_rhee,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rhe_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(rhevss,&st.n_rhe2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rhe_accessory(int t,int c,int a,int b,int d,int e,int y){return add(rheas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rhe_market(int t,int c,int a,int b,int d,int e,int y){return add(rhemks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rhe_report(void){ps("[RHE] Rhepp: ");pi(st.n_rhep);ps(" PCS=");pi(st.t_f1);ps("\nRhee: ");pi(st.n_rhee);ps(" PCS=");pi(st.t_f2);ps("\nRhev: ");pi(st.n_rhe2);ps(" PCS=");pi(st.t_f3);ps("\nRhec: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void rhe_state(void){ps("[RHE] Rhepp=");pi(st.n_rhep);ps(" Rhee=");pi(st.n_rhee);ps(" Rhev=");pi(st.n_rhe2);ps(" Rhec=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Rhexia Admin Demo ===\n\n");rhe_init();
ps("Rhexia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rhe_planning(t,c,1179+(i*17),1168+(i*14),1148+(i*10),1130+(i*6),2020+(i%5));}
ps("\nRhexia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rhe_execution(t,c,1168+(i*15),1157+(i*12),1139+(i*8),1126+(i*5),2021+(i%4));}
ps("\nRhexia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rhe_evaluation(t,c,1160+(i*13),1149+(i*10),1133+(i*7),1122+(i*4),2022+(i%3));}
ps("\nRhexia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rhe_accessory(t,c,1152+(i*11),1143+(i*9),1129+(i*6),1119+(i*3),2023+(i%2));}
ps("\nRhexia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rhe_market(t,c,1146+(i*9),1137+(i*7),1124+(i*5),1116+(i*3),2024);}
ps("\n");rhe_report();rhe_state();ps("\n=== Demo Complete ===\n");return 0;}
