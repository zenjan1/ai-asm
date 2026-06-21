/* adenium_admin: Adenium management technology administration (v1.0)
 * Adenium planning, adenium execution, adenium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ade_t;
typedef struct{int n_adep,n_adee,n_ade2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ade_state_t;
static ade_t adeps[N],adess[N-2],adevss[N-4],adeas[N-6],ademks[N-6]; static ade_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ade_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ade_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ADE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ade_init(void){if(init)return -1;st.n_adep=0;st.n_adee=0;st.n_ade2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)adeps[i].active=0;for(int i=0;i<N-2;i++)adess[i].active=0;for(int i=0;i<N-4;i++)adevss[i].active=0;for(int i=0;i<N-6;i++)adeas[i].active=0;for(int i=0;i<N-6;i++)ademks[i].active=0;init=1;ps("[ADE] Adenium initialized\n");return 0;}
int ade_planning(int t,int c,int a,int b,int d,int e,int y){return add(adeps,&st.n_adep,&st.t_f1,N,t,c,a,b,d,e,y);}
int ade_execution(int t,int c,int a,int b,int d,int e,int y){return add(adess,&st.n_adee,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ade_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(adevss,&st.n_ade2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ade_accessory(int t,int c,int a,int b,int d,int e,int y){return add(adeas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ade_market(int t,int c,int a,int b,int d,int e,int y){return add(ademks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ade_report(void){ps("[ADE] Adelp: ");pi(st.n_adep);ps(" PCS=");pi(st.t_f1);ps("\nAdel: ");pi(st.n_adee);ps(" PCS=");pi(st.t_f2);ps("\nAdev: ");pi(st.n_ade2);ps(" PCS=");pi(st.t_f3);ps("\nAdelc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ade_state(void){ps("[ADE] Adelp=");pi(st.n_adep);ps(" Adel=");pi(st.n_adee);ps(" Adev=");pi(st.n_ade2);ps(" Adelc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Adenium Admin Demo ===\n\n");ade_init();
ps("Adenium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ade_planning(t,c,968+(i*17),957+(i*14),937+(i*10),919+(i*6),2020+(i%5));}
ps("\nAdenium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ade_execution(t,c,957+(i*15),946+(i*12),928+(i*8),915+(i*5),2021+(i%4));}
ps("\nAdenium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ade_evaluation(t,c,949+(i*13),938+(i*10),922+(i*7),911+(i*4),2022+(i%3));}
ps("\nAdenium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ade_accessory(t,c,941+(i*11),932+(i*9),918+(i*6),908+(i*3),2023+(i%2));}
ps("\nAdenium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ade_market(t,c,935+(i*9),926+(i*7),913+(i*5),905+(i*3),2024);}
ps("\n");ade_report();ade_state();ps("\n=== Demo Complete ===\n");return 0;}
