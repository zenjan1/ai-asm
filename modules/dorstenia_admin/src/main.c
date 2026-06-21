/* dorstenia_admin: Dorstenia management technology administration (v1.0)
 * Dorstenia planning, dorstenia execution, dorstenia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} dor_t;
typedef struct{int n_dorp,n_dore,n_dor2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} dor_state_t;
static dor_t dorps[N],dorss[N-2],dorvss[N-4],doras[N-6],dormks[N-6]; static dor_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dor_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;dor_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DOR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int dor_init(void){if(init)return -1;st.n_dorp=0;st.n_dore=0;st.n_dor2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dorps[i].active=0;for(int i=0;i<N-2;i++)dorss[i].active=0;for(int i=0;i<N-4;i++)dorvss[i].active=0;for(int i=0;i<N-6;i++)doras[i].active=0;for(int i=0;i<N-6;i++)dormks[i].active=0;init=1;ps("[DOR] Dorstenia initialized\n");return 0;}
int dor_planning(int t,int c,int a,int b,int d,int e,int y){return add(dorps,&st.n_dorp,&st.t_f1,N,t,c,a,b,d,e,y);}
int dor_execution(int t,int c,int a,int b,int d,int e,int y){return add(dorss,&st.n_dore,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int dor_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(dorvss,&st.n_dor2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int dor_accessory(int t,int c,int a,int b,int d,int e,int y){return add(doras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int dor_market(int t,int c,int a,int b,int d,int e,int y){return add(dormks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void dor_report(void){ps("[DOR] Dorrp: ");pi(st.n_dorp);ps(" PCS=");pi(st.t_f1);ps("\nDore: ");pi(st.n_dore);ps(" PCS=");pi(st.t_f2);ps("\nDorv: ");pi(st.n_dor2);ps(" PCS=");pi(st.t_f3);ps("\nDorc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void dor_state(void){ps("[DOR] Dorrp=");pi(st.n_dorp);ps(" Dore=");pi(st.n_dore);ps(" Dorv=");pi(st.n_dor2);ps(" Dorc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Dorstenia Admin Demo ===\n\n");dor_init();
ps("Dorstenia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;dor_planning(t,c,995+(i*17),984+(i*14),964+(i*10),946+(i*6),2020+(i%5));}
ps("\nDorstenia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;dor_execution(t,c,984+(i*15),973+(i*12),955+(i*8),942+(i*5),2021+(i%4));}
ps("\nDorstenia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;dor_evaluation(t,c,976+(i*13),965+(i*10),949+(i*7),938+(i*4),2022+(i%3));}
ps("\nDorstenia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dor_accessory(t,c,968+(i*11),959+(i*9),945+(i*6),935+(i*3),2023+(i%2));}
ps("\nDorstenia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dor_market(t,c,962+(i*9),953+(i*7),940+(i*5),932+(i*3),2024);}
ps("\n");dor_report();dor_state();ps("\n=== Demo Complete ===\n");return 0;}
