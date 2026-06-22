/* buglossoides_admin: Buglossoides management technology administration (v1.0)
 * Buglossoides planning, buglossoides execution, buglossoides evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} bugl_t;
typedef struct{int n_buglp,n_bugl,n_bugl2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} bugl_state_t;
static bugl_t bugls[N],bugle[N-2],bugl2[N-4],buglac[N-6],buglam[N-6]; static bugl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(bugl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;bugl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BUGL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int bugl_init(void){if(init)return -1;st.n_buglp=0;st.n_bugl=0;st.n_bugl2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)bugls[i].active=0;for(int i=0;i<N-2;i++)bugle[i].active=0;for(int i=0;i<N-4;i++)bugl2[i].active=0;for(int i=0;i<N-6;i++)buglac[i].active=0;for(int i=0;i<N-6;i++)buglam[i].active=0;init=1;ps("[BUGL] Buglossoides initialized\n");return 0;}
int bugl_planning(int t,int c,int a,int b,int d,int e,int y){return add(bugls,&st.n_buglp,&st.t_f1,N,t,c,a,b,d,e,y);}
int bugl_execution(int t,int c,int a,int b,int d,int e,int y){return add(bugle,&st.n_bugl,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int bugl_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(bugl2,&st.n_bugl2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int bugl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(buglac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int bugl_market(int t,int c,int a,int b,int d,int e,int y){return add(buglam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void bugl_report(void){ps("[BUGL] Gp: ");pi(st.n_buglp);ps(" PCS=");pi(st.t_f1);ps("\nGe: ");pi(st.n_bugl);ps(" PCS=");pi(st.t_f2);ps("\nGl2: ");pi(st.n_bugl2);ps(" PCS=");pi(st.t_f3);ps("\nGac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void bugl_state(void){ps("[BUGL] Gp=");pi(st.n_buglp);ps(" Gl=");pi(st.n_bugl);ps(" Gl2=");pi(st.n_bugl2);ps(" Gac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Buglossoides Admin Demo ===\n\n");bugl_init();
ps("Buglossoides planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;bugl_planning(t,c,1447+(i*17),1436+(i*14),1416+(i*10),1398+(i*6),2020+(i%5));}
ps("\nBuglossoides execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;bugl_execution(t,c,1436+(i*15),1425+(i*12),1407+(i*8),1394+(i*5),2021+(i%4));}
ps("\nBuglossoides evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;bugl_evaluation(t,c,1428+(i*13),1417+(i*10),1401+(i*7),1390+(i*4),2022+(i%3));}
ps("\nBuglossoides accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bugl_accessory(t,c,1420+(i*11),1411+(i*9),1397+(i*6),1387+(i*3),2023+(i%2));}
ps("\nBuglossoides marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bugl_market(t,c,1414+(i*9),1405+(i*7),1392+(i*5),1384+(i*3),2024);}
ps("\n");bugl_report();bugl_state();ps("\n=== Demo Complete ===\n");return 0;}
