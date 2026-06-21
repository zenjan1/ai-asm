/* zephyranthes_admin: Zephyranthes management technology administration (v1.0)
 * Zephyranthes planning, zephyranthes execution, zephyranthes evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} zep_t;
typedef struct{int n_zep,n_zee,n_zev,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} zep_state_t;
static zep_t zeps[N],zees[N-2],zevs[N-4],acs[N-6],mks[N-6]; static zep_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(zep_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;zep_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ZEP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int zep_init(void){if(init)return -1;st.n_zep=0;st.n_zee=0;st.n_zev=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)zeps[i].active=0;for(int i=0;i<N-2;i++)zees[i].active=0;for(int i=0;i<N-4;i++)zevs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[ZEP] Zephyranthes initialized\n");return 0;}
int zep_planning(int t,int c,int a,int b,int d,int e,int y){return add(zeps,&st.n_zep,&st.t_f1,N,t,c,a,b,d,e,y);}
int zep_execution(int t,int c,int a,int b,int d,int e,int y){return add(zees,&st.n_zee,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int zep_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(zevs,&st.n_zev,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int zep_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int zep_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void zep_report(void){ps("[ZEP] Zep: ");pi(st.n_zep);ps(" PCS=");pi(st.t_f1);ps("\nZee: ");pi(st.n_zee);ps(" PCS=");pi(st.t_f2);ps("\nZev: ");pi(st.n_zev);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void zep_state(void){ps("[ZEP] Zep=");pi(st.n_zep);ps(" Zee=");pi(st.n_zee);ps(" Zev=");pi(st.n_zev);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Zephyranthes Admin Demo ===\n\n");zep_init();
ps("Zephyranthes planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;zep_planning(t,c,617+(i*17),606+(i*14),586+(i*10),568+(i*6),2020+(i%5));}
ps("\nZephyranthes execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;zep_execution(t,c,606+(i*15),595+(i*12),577+(i*8),564+(i*5),2021+(i%4));}
ps("\nZephyranthes evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;zep_evaluation(t,c,598+(i*13),587+(i*10),571+(i*7),560+(i*4),2022+(i%3));}
ps("\nZephyranthes accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zep_accessory(t,c,590+(i*11),581+(i*9),567+(i*6),557+(i*3),2023+(i%2));}
ps("\nZephyranthes marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zep_market(t,c,584+(i*9),575+(i*7),562+(i*5),554+(i*3),2024);}
ps("\n");zep_report();zep_state();ps("\n=== Demo Complete ===\n");return 0;}
