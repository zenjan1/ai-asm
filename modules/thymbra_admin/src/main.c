/* thymbra_admin: Thymbra management technology administration (v1.0)
 * Thymbra planning, thymbra execution, thymbra evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} thy_t;
typedef struct{int n_thyp,n_thye,n_thy2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} thy_state_t;
static thy_t thyps[N],thyss[N-2],thyvss[N-4],thyas[N-6],thymks[N-6]; static thy_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(thy_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;thy_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[THY] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int thy_init(void){if(init)return -1;st.n_thyp=0;st.n_thye=0;st.n_thy2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)thyps[i].active=0;for(int i=0;i<N-2;i++)thyss[i].active=0;for(int i=0;i<N-4;i++)thyvss[i].active=0;for(int i=0;i<N-6;i++)thyas[i].active=0;for(int i=0;i<N-6;i++)thymks[i].active=0;init=1;ps("[THY] Thymbra initialized\n");return 0;}
int thy_planning(int t,int c,int a,int b,int d,int e,int y){return add(thyps,&st.n_thyp,&st.t_f1,N,t,c,a,b,d,e,y);}
int thy_execution(int t,int c,int a,int b,int d,int e,int y){return add(thyss,&st.n_thye,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int thy_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(thyvss,&st.n_thy2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int thy_accessory(int t,int c,int a,int b,int d,int e,int y){return add(thyas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int thy_market(int t,int c,int a,int b,int d,int e,int y){return add(thymks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void thy_report(void){ps("[THY] Thypp: ");pi(st.n_thyp);ps(" PCS=");pi(st.t_f1);ps("\nThye: ");pi(st.n_thye);ps(" PCS=");pi(st.t_f2);ps("\nThyv: ");pi(st.n_thy2);ps(" PCS=");pi(st.t_f3);ps("\nThyc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void thy_state(void){ps("[THY] Thypp=");pi(st.n_thyp);ps(" Thye=");pi(st.n_thye);ps(" Thyv=");pi(st.n_thy2);ps(" Thyc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Thymbra Admin Demo ===\n\n");thy_init();
ps("Thymbra planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;thy_planning(t,c,1105+(i*17),1094+(i*14),1074+(i*10),1056+(i*6),2020+(i%5));}
ps("\nThymbra execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;thy_execution(t,c,1094+(i*15),1083+(i*12),1065+(i*8),1052+(i*5),2021+(i%4));}
ps("\nThymbra evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;thy_evaluation(t,c,1086+(i*13),1075+(i*10),1059+(i*7),1048+(i*4),2022+(i%3));}
ps("\nThymbra accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;thy_accessory(t,c,1078+(i*11),1069+(i*9),1055+(i*6),1045+(i*3),2023+(i%2));}
ps("\nThymbra marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;thy_market(t,c,1072+(i*9),1063+(i*7),1050+(i*5),1042+(i*3),2024);}
ps("\n");thy_report();thy_state();ps("\n=== Demo Complete ===\n");return 0;}
