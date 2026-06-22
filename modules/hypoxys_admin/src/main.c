/* hypoxys_admin: Hypoxys management technology administration (v1.0)
 * Hypoxys planning, hypoxys execution, hypoxys evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hypx_t;
typedef struct{int n_hyxp,n_hypx,n_hypx2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hypx_state_t;
static hypx_t hypxs[N],hypxe[N-2],hypx2[N-4],hypxac[N-6],hypxam[N-6]; static hypx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(hypx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hypx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HYPX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hypx_init(void){if(init)return -1;st.n_hyxp=0;st.n_hypx=0;st.n_hypx2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hypxs[i].active=0;for(int i=0;i<N-2;i++)hypxe[i].active=0;for(int i=0;i<N-4;i++)hypx2[i].active=0;for(int i=0;i<N-6;i++)hypxac[i].active=0;for(int i=0;i<N-6;i++)hypxam[i].active=0;init=1;ps("[HYPX] Hypoxys initialized\n");return 0;}
int hypx_planning(int t,int c,int a,int b,int d,int e,int y){return add(hypxs,&st.n_hyxp,&st.t_f1,N,t,c,a,b,d,e,y);}
int hypx_execution(int t,int c,int a,int b,int d,int e,int y){return add(hypxe,&st.n_hypx,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hypx_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hypx2,&st.n_hypx2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hypx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(hypxac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hypx_market(int t,int c,int a,int b,int d,int e,int y){return add(hypxam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hypx_report(void){ps("[HYPX] Hypx: ");pi(st.n_hyxp);ps(" PCS=");pi(st.t_f1);ps("\nHypxe: ");pi(st.n_hypx);ps(" PCS=");pi(st.t_f2);ps("\nHypx2: ");pi(st.n_hypx2);ps(" PCS=");pi(st.t_f3);ps("\nHac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hypx_state(void){ps("[HYPX] Hypx=");pi(st.n_hyxp);ps(" Hypx=");pi(st.n_hypx);ps(" Hypx2=");pi(st.n_hypx2);ps(" Hac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Hypoxys Admin Demo ===\n\n");hypx_init();
ps("Hypoxys planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hypx_planning(t,c,1540+(i*17),1529+(i*14),1509+(i*10),1491+(i*6),2020+(i%5));}
ps("\nHypoxys execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hypx_execution(t,c,1529+(i*15),1518+(i*12),1500+(i*8),1487+(i*5),2021+(i%4));}
ps("\nHypoxys evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hypx_evaluation(t,c,1521+(i*13),1510+(i*10),1494+(i*7),1483+(i*4),2022+(i%3));}
ps("\nHypoxys accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hypx_accessory(t,c,1513+(i*11),1504+(i*9),1490+(i*6),1480+(i*3),2023+(i%2));}
ps("\nHypoxys marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hypx_market(t,c,1507+(i*9),1498+(i*7),1485+(i*5),1477+(i*3),2024);}
ps("\n");hypx_report();hypx_state();ps("\n=== Demo Complete ===\n");return 0;}
