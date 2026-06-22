/* vittaria_admin: Vittaria management technology administration (v1.0)
 * Vittaria planning, vittaria execution, vittaria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} vitt_t;
typedef struct{int n_vittp,n_vitt,n_vitt2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} vitt_state_t;
static vitt_t vitts[N],vitte[N-2],vittv[N-4],vittac[N-6],vittm[N-6]; static vitt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(vitt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;vitt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[VITT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int vitt_init(void){if(init)return -1;st.n_vittp=0;st.n_vitt=0;st.n_vitt2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)vitts[i].active=0;for(int i=0;i<N-2;i++)vitte[i].active=0;for(int i=0;i<N-4;i++)vittv[i].active=0;for(int i=0;i<N-6;i++)vittac[i].active=0;for(int i=0;i<N-6;i++)vittm[i].active=0;init=1;ps("[VITT] Vittaria initialized\n");return 0;}
int vitt_planning(int t,int c,int a,int b,int d,int e,int y){return add(vitts,&st.n_vittp,&st.t_f1,N,t,c,a,b,d,e,y);}
int vitt_execution(int t,int c,int a,int b,int d,int e,int y){return add(vitte,&st.n_vitt,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int vitt_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(vittv,&st.n_vitt2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int vitt_accessory(int t,int c,int a,int b,int d,int e,int y){return add(vittac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int vitt_market(int t,int c,int a,int b,int d,int e,int y){return add(vittm,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void vitt_report(void){ps("[VITT] Vittp: ");pi(st.n_vittp);ps(" PCS=");pi(st.t_f1);ps("\nVitte: ");pi(st.n_vitt);ps(" PCS=");pi(st.t_f2);ps("\nVittv: ");pi(st.n_vitt2);ps(" PCS=");pi(st.t_f3);ps("\nVittc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void vitt_state(void){ps("[VITT] Vittp=");pi(st.n_vittp);ps(" Vitte=");pi(st.n_vitt);ps(" Vittv=");pi(st.n_vitt2);ps(" Vittc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Vittaria Admin Demo ===\n\n");vitt_init();
ps("Vittaria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;vitt_planning(t,c,1367+(i*17),1356+(i*14),1336+(i*10),1318+(i*6),2020+(i%5));}
ps("\nVittaria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;vitt_execution(t,c,1356+(i*15),1345+(i*12),1327+(i*8),1314+(i*5),2021+(i%4));}
ps("\nVittaria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;vitt_evaluation(t,c,1348+(i*13),1337+(i*10),1321+(i*7),1310+(i*4),2022+(i%3));}
ps("\nVittaria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vitt_accessory(t,c,1340+(i*11),1331+(i*9),1317+(i*6),1307+(i*3),2023+(i%2));}
ps("\nVittaria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vitt_market(t,c,1334+(i*9),1325+(i*7),1312+(i*5),1304+(i*3),2024);}
ps("\n");vitt_report();vitt_state();ps("\n=== Demo Complete ===\n");return 0;}
