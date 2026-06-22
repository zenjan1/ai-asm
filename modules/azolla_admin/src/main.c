/* azolla_admin: Azolla management technology administration (v1.0)
 * Azolla planning, azolla execution, azolla evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} azol_t;
typedef struct{int n_azolp,n_azol,n_azol2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} azol_state_t;
static azol_t azols[N],azole[N-2],azol2[N-4],azolac[N-6],azolam[N-6]; static azol_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(azol_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;azol_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[AZOL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int azol_init(void){if(init)return -1;st.n_azolp=0;st.n_azol=0;st.n_azol2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)azols[i].active=0;for(int i=0;i<N-2;i++)azole[i].active=0;for(int i=0;i<N-4;i++)azol2[i].active=0;for(int i=0;i<N-6;i++)azolac[i].active=0;for(int i=0;i<N-6;i++)azolam[i].active=0;init=1;ps("[AZOL] Azolla initialized\n");return 0;}
int azol_planning(int t,int c,int a,int b,int d,int e,int y){return add(azols,&st.n_azolp,&st.t_f1,N,t,c,a,b,d,e,y);}
int azol_execution(int t,int c,int a,int b,int d,int e,int y){return add(azole,&st.n_azol,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int azol_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(azol2,&st.n_azol2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int azol_accessory(int t,int c,int a,int b,int d,int e,int y){return add(azolac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int azol_market(int t,int c,int a,int b,int d,int e,int y){return add(azolam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void azol_report(void){ps("[AZOL] Zolp: ");pi(st.n_azolp);ps(" PCS=");pi(st.t_f1);ps("\nZole: ");pi(st.n_azol);ps(" PCS=");pi(st.t_f2);ps("\nZol2: ");pi(st.n_azol2);ps(" PCS=");pi(st.t_f3);ps("\nZolac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void azol_state(void){ps("[AZOL] Zolp=");pi(st.n_azolp);ps(" Zole=");pi(st.n_azol);ps(" Zol2=");pi(st.n_azol2);ps(" Zolac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Azolla Admin Demo ===\n\n");azol_init();
ps("Azolla planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;azol_planning(t,c,1425+(i*17),1414+(i*14),1394+(i*10),1376+(i*6),2020+(i%5));}
ps("\nAzolla execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;azol_execution(t,c,1414+(i*15),1403+(i*12),1385+(i*8),1372+(i*5),2021+(i%4));}
ps("\nAzolla evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;azol_evaluation(t,c,1406+(i*13),1395+(i*10),1379+(i*7),1368+(i*4),2022+(i%3));}
ps("\nAzolla accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;azol_accessory(t,c,1398+(i*11),1389+(i*9),1375+(i*6),1365+(i*3),2023+(i%2));}
ps("\nAzolla marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;azol_market(t,c,1392+(i*9),1383+(i*7),1370+(i*5),1362+(i*3),2024);}
ps("\n");azol_report();azol_state();ps("\n=== Demo Complete ===\n");return 0;}
