/* alismat_admin: Alismat management technology administration (v1.0)
 * Alismat planning, alismat execution, alismat evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} alis_t;
typedef struct{int n_alisp,n_alis,n_alis2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} alis_state_t;
static alis_t aliss[N],alise[N-2],alisv[N-4],alisac[N-6],alism[N-6]; static alis_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(alis_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;alis_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ALIS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int alis_init(void){if(init)return -1;st.n_alisp=0;st.n_alis=0;st.n_alis2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)aliss[i].active=0;for(int i=0;i<N-2;i++)alise[i].active=0;for(int i=0;i<N-4;i++)alisv[i].active=0;for(int i=0;i<N-6;i++)alisac[i].active=0;for(int i=0;i<N-6;i++)alism[i].active=0;init=1;ps("[ALIS] Alismat initialized\n");return 0;}
int alis_planning(int t,int c,int a,int b,int d,int e,int y){return add(aliss,&st.n_alisp,&st.t_f1,N,t,c,a,b,d,e,y);}
int alis_execution(int t,int c,int a,int b,int d,int e,int y){return add(alise,&st.n_alis,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int alis_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(alisv,&st.n_alis2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int alis_accessory(int t,int c,int a,int b,int d,int e,int y){return add(alisac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int alis_market(int t,int c,int a,int b,int d,int e,int y){return add(alism,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void alis_report(void){ps("[ALIS] Lisp: ");pi(st.n_alisp);ps(" PCS=");pi(st.t_f1);ps("\nLise: ");pi(st.n_alis);ps(" PCS=");pi(st.t_f2);ps("\nLisv: ");pi(st.n_alis2);ps(" PCS=");pi(st.t_f3);ps("\nLisac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void alis_state(void){ps("[ALIS] Lisp=");pi(st.n_alisp);ps(" Lise=");pi(st.n_alis);ps(" Lisv=");pi(st.n_alis2);ps(" Lisac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Alismat Admin Demo ===\n\n");alis_init();
ps("Alismat planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;alis_planning(t,c,1397+(i*17),1386+(i*14),1366+(i*10),1348+(i*6),2020+(i%5));}
ps("\nAlismat execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;alis_execution(t,c,1386+(i*15),1375+(i*12),1357+(i*8),1344+(i*5),2021+(i%4));}
ps("\nAlismat evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;alis_evaluation(t,c,1378+(i*13),1367+(i*10),1351+(i*7),1340+(i*4),2022+(i%3));}
ps("\nAlismat accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;alis_accessory(t,c,1370+(i*11),1361+(i*9),1347+(i*6),1337+(i*3),2023+(i%2));}
ps("\nAlismat marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;alis_market(t,c,1364+(i*9),1355+(i*7),1342+(i*5),1334+(i*3),2024);}
ps("\n");alis_report();alis_state();ps("\n=== Demo Complete ===\n");return 0;}
