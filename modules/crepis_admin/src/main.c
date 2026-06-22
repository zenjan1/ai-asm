/* crepis_admin: Crepis management technology administration (v1.0)
 * Crepis planning, crepis execution, crepis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} crep_t;
typedef struct{int n_crepp,n_crep,n_crep2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} crep_state_t;
static crep_t creps[N],crepe[N-2],crep2[N-4],crepac[N-6],crepam[N-6]; static crep_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(crep_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;crep_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CREP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int crep_init(void){if(init)return -1;st.n_crepp=0;st.n_crep=0;st.n_crep2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)creps[i].active=0;for(int i=0;i<N-2;i++)crepe[i].active=0;for(int i=0;i<N-4;i++)crep2[i].active=0;for(int i=0;i<N-6;i++)crepac[i].active=0;for(int i=0;i<N-6;i++)crepam[i].active=0;init=1;ps("[CREP] Crepis initialized\n");return 0;}
int crep_planning(int t,int c,int a,int b,int d,int e,int y){return add(creps,&st.n_crepp,&st.t_f1,N,t,c,a,b,d,e,y);}
int crep_execution(int t,int c,int a,int b,int d,int e,int y){return add(crepe,&st.n_crep,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int crep_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(crep2,&st.n_crep2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int crep_accessory(int t,int c,int a,int b,int d,int e,int y){return add(crepac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int crep_market(int t,int c,int a,int b,int d,int e,int y){return add(crepam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void crep_report(void){ps("[CREP] Epp: ");pi(st.n_crepp);ps(" PCS=");pi(st.t_f1);ps("\nEpe: ");pi(st.n_crep);ps(" PCS=");pi(st.t_f2);ps("\nEp2: ");pi(st.n_crep2);ps(" PCS=");pi(st.t_f3);ps("\nEac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void crep_state(void){ps("[CREP] Epp=");pi(st.n_crepp);ps(" Ep=");pi(st.n_crep);ps(" Ep2=");pi(st.n_crep2);ps(" Eac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Crepis Admin Demo ===\n\n");crep_init();
ps("Crepis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;crep_planning(t,c,1482+(i*17),1471+(i*14),1451+(i*10),1433+(i*6),2020+(i%5));}
ps("\nCrepis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;crep_execution(t,c,1471+(i*15),1460+(i*12),1442+(i*8),1429+(i*5),2021+(i%4));}
ps("\nCrepis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;crep_evaluation(t,c,1463+(i*13),1452+(i*10),1436+(i*7),1425+(i*4),2022+(i%3));}
ps("\nCrepis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;crep_accessory(t,c,1455+(i*11),1446+(i*9),1432+(i*6),1422+(i*3),2023+(i%2));}
ps("\nCrepis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;crep_market(t,c,1449+(i*9),1440+(i*7),1427+(i*5),1419+(i*3),2024);}
ps("\n");crep_report();crep_state();ps("\n=== Demo Complete ===\n");return 0;}
