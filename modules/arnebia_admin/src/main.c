/* arnebia_admin: Arnebia management technology administration (v1.0)
 * Arnebia planning, arnebia execution, arnebia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} arne_t;
typedef struct{int n_arnep,n_arne,n_arne2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} arne_state_t;
static arne_t arnes[N],arnees[N-2],arne2[N-4],arneac[N-6],arneam[N-6]; static arne_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(arne_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;arne_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ARNE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int arne_init(void){if(init)return -1;st.n_arnep=0;st.n_arne=0;st.n_arne2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)arnes[i].active=0;for(int i=0;i<N-2;i++)arnees[i].active=0;for(int i=0;i<N-4;i++)arne2[i].active=0;for(int i=0;i<N-6;i++)arneac[i].active=0;for(int i=0;i<N-6;i++)arneam[i].active=0;init=1;ps("[ARNE] Arnebia initialized\n");return 0;}
int arne_planning(int t,int c,int a,int b,int d,int e,int y){return add(arnes,&st.n_arnep,&st.t_f1,N,t,c,a,b,d,e,y);}
int arne_execution(int t,int c,int a,int b,int d,int e,int y){return add(arnees,&st.n_arne,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int arne_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(arne2,&st.n_arne2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int arne_accessory(int t,int c,int a,int b,int d,int e,int y){return add(arneac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int arne_market(int t,int c,int a,int b,int d,int e,int y){return add(arneam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void arne_report(void){ps("[ARNE] Nrp: ");pi(st.n_arnep);ps(" PCS=");pi(st.t_f1);ps("\nNree: ");pi(st.n_arne);ps(" PCS=");pi(st.t_f2);ps("\nNr2: ");pi(st.n_arne2);ps(" PCS=");pi(st.t_f3);ps("\nNrac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void arne_state(void){ps("[ARNE] Nrp=");pi(st.n_arnep);ps(" Nree=");pi(st.n_arne);ps(" Nr2=");pi(st.n_arne2);ps(" Nrac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Arnebia Admin Demo ===\n\n");arne_init();
ps("Arnebia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;arne_planning(t,c,1416+(i*17),1405+(i*14),1385+(i*10),1367+(i*6),2020+(i%5));}
ps("\nArnebia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;arne_execution(t,c,1405+(i*15),1394+(i*12),1376+(i*8),1363+(i*5),2021+(i%4));}
ps("\nArnebia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;arne_evaluation(t,c,1397+(i*13),1386+(i*10),1370+(i*7),1359+(i*4),2022+(i%3));}
ps("\nArnebia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;arne_accessory(t,c,1389+(i*11),1380+(i*9),1366+(i*6),1356+(i*3),2023+(i%2));}
ps("\nArnebia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;arne_market(t,c,1383+(i*9),1374+(i*7),1361+(i*5),1353+(i*3),2024);}
ps("\n");arne_report();arne_state();ps("\n=== Demo Complete ===\n");return 0;}
