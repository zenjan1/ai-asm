/* geum_admin: Geum management technology administration (v1.0)
 * Geum planning, geum execution, geum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} geu_t;
typedef struct{int n_gep,n_gee,n_gev,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} geu_state_t;
static geu_t geps[N],gess[N-2],gevss[N-4],geas[N-6],gemks[N-6]; static geu_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(geu_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;geu_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GEU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int geu_init(void){if(init)return -1;st.n_gep=0;st.n_gee=0;st.n_gev=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)geps[i].active=0;for(int i=0;i<N-2;i++)gess[i].active=0;for(int i=0;i<N-4;i++)gevss[i].active=0;for(int i=0;i<N-6;i++)geas[i].active=0;for(int i=0;i<N-6;i++)gemks[i].active=0;init=1;ps("[GEU] Geum initialized\n");return 0;}
int geu_planning(int t,int c,int a,int b,int d,int e,int y){return add(geps,&st.n_gep,&st.t_f1,N,t,c,a,b,d,e,y);}
int geu_execution(int t,int c,int a,int b,int d,int e,int y){return add(gess,&st.n_gee,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int geu_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(gevss,&st.n_gev,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int geu_accessory(int t,int c,int a,int b,int d,int e,int y){return add(geas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int geu_market(int t,int c,int a,int b,int d,int e,int y){return add(gemks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void geu_report(void){ps("[GEU] Gep: ");pi(st.n_gep);ps(" PCS=");pi(st.t_f1);ps("\nGee: ");pi(st.n_gee);ps(" PCS=");pi(st.t_f2);ps("\nGev: ");pi(st.n_gev);ps(" PCS=");pi(st.t_f3);ps("\nGec: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void geu_state(void){ps("[GEU] Gep=");pi(st.n_gep);ps(" Gee=");pi(st.n_gee);ps(" Gev=");pi(st.n_gev);ps(" Gec=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Geum Admin Demo ===\n\n");geu_init();
ps("Geum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;geu_planning(t,c,824+(i*17),813+(i*14),793+(i*10),775+(i*6),2020+(i%5));}
ps("\nGeum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;geu_execution(t,c,813+(i*15),802+(i*12),784+(i*8),771+(i*5),2021+(i%4));}
ps("\nGeum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;geu_evaluation(t,c,805+(i*13),794+(i*10),780+(i*7),769+(i*4),2022+(i%3));}
ps("\nGeum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;geu_accessory(t,c,797+(i*11),788+(i*9),774+(i*6),764+(i*3),2023+(i%2));}
ps("\nGeum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;geu_market(t,c,791+(i*9),782+(i*7),769+(i*5),761+(i*3),2024);}
ps("\n");geu_report();geu_state();ps("\n=== Demo Complete ===\n");return 0;}
