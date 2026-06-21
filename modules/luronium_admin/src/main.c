/* luronium_admin: Luronium management technology administration (v1.0)
 * Luronium planning, luronium execution, luronium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lur_t;
typedef struct{int n_lurp,n_lure,n_lur2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lur_state_t;
static lur_t lurps[N],lurss[N-2],lurvss[N-4],luras[N-6],lurmks[N-6]; static lur_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lur_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lur_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LUR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lur_init(void){if(init)return -1;st.n_lurp=0;st.n_lure=0;st.n_lur2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lurps[i].active=0;for(int i=0;i<N-2;i++)lurss[i].active=0;for(int i=0;i<N-4;i++)lurvss[i].active=0;for(int i=0;i<N-6;i++)luras[i].active=0;for(int i=0;i<N-6;i++)lurmks[i].active=0;init=1;ps("[LUR] Luronium initialized\n");return 0;}
int lur_planning(int t,int c,int a,int b,int d,int e,int y){return add(lurps,&st.n_lurp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lur_execution(int t,int c,int a,int b,int d,int e,int y){return add(lurss,&st.n_lure,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lur_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lurvss,&st.n_lur2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lur_accessory(int t,int c,int a,int b,int d,int e,int y){return add(luras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lur_market(int t,int c,int a,int b,int d,int e,int y){return add(lurmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lur_report(void){ps("[LUR] Lurp: ");pi(st.n_lurp);ps(" PCS=");pi(st.t_f1);ps("\nLure: ");pi(st.n_lure);ps(" PCS=");pi(st.t_f2);ps("\nLurv: ");pi(st.n_lur2);ps(" PCS=");pi(st.t_f3);ps("\nLurc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lur_state(void){ps("[LUR] Lurp=");pi(st.n_lurp);ps(" Lure=");pi(st.n_lure);ps(" Lurv=");pi(st.n_lur2);ps(" Lurc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Luronium Admin Demo ===\n\n");lur_init();
ps("Luronium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lur_planning(t,c,1063+(i*17),1052+(i*14),1032+(i*10),1014+(i*6),2020+(i%5));}
ps("\nLuronium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lur_execution(t,c,1052+(i*15),1041+(i*12),1023+(i*8),1010+(i*5),2021+(i%4));}
ps("\nLuronium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lur_evaluation(t,c,1044+(i*13),1033+(i*10),1017+(i*7),1006+(i*4),2022+(i%3));}
ps("\nLuronium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lur_accessory(t,c,1036+(i*11),1027+(i*9),1013+(i*6),1003+(i*3),2023+(i%2));}
ps("\nLuronium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lur_market(t,c,1030+(i*9),1021+(i*7),1008+(i*5),1000+(i*3),2024);}
ps("\n");lur_report();lur_state();ps("\n=== Demo Complete ===\n");return 0;}
