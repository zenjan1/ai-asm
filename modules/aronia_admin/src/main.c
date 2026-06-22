/* aronia_admin: Aronia management technology administration (v1.0)
 * Aronia planning, aronia execution, aronia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} aron_t;
typedef struct{int n_aronp,n_aron,n_aron2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} aron_state_t;
static aron_t arons[N],arone[N-2],aron2[N-4],aronac[N-6],aronam[N-6]; static aron_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(aron_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;aron_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ARON] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int aron_init(void){if(init)return -1;st.n_aronp=0;st.n_aron=0;st.n_aron2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)arons[i].active=0;for(int i=0;i<N-2;i++)arone[i].active=0;for(int i=0;i<N-4;i++)aron2[i].active=0;for(int i=0;i<N-6;i++)aronac[i].active=0;for(int i=0;i<N-6;i++)aronam[i].active=0;init=1;ps("[ARON] Aronia initialized\n");return 0;}
int aron_planning(int t,int c,int a,int b,int d,int e,int y){return add(arons,&st.n_aronp,&st.t_f1,N,t,c,a,b,d,e,y);}
int aron_execution(int t,int c,int a,int b,int d,int e,int y){return add(arone,&st.n_aron,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int aron_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(aron2,&st.n_aron2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int aron_accessory(int t,int c,int a,int b,int d,int e,int y){return add(aronac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int aron_market(int t,int c,int a,int b,int d,int e,int y){return add(aronam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void aron_report(void){ps("[ARON] Ronp: ");pi(st.n_aronp);ps(" PCS=");pi(st.t_f1);ps("\nRone: ");pi(st.n_aron);ps(" PCS=");pi(st.t_f2);ps("\nRon2: ");pi(st.n_aron2);ps(" PCS=");pi(st.t_f3);ps("\nRonac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void aron_state(void){ps("[ARON] Ronp=");pi(st.n_aronp);ps(" Rone=");pi(st.n_aron);ps(" Ron2=");pi(st.n_aron2);ps(" Ronac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Aronia Admin Demo ===\n\n");aron_init();
ps("Aronia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;aron_planning(t,c,1417+(i*17),1406+(i*14),1386+(i*10),1368+(i*6),2020+(i%5));}
ps("\nAronia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;aron_execution(t,c,1406+(i*15),1395+(i*12),1377+(i*8),1364+(i*5),2021+(i%4));}
ps("\nAronia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;aron_evaluation(t,c,1398+(i*13),1387+(i*10),1371+(i*7),1360+(i*4),2022+(i%3));}
ps("\nAronia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;aron_accessory(t,c,1390+(i*11),1381+(i*9),1367+(i*6),1357+(i*3),2023+(i%2));}
ps("\nAronia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;aron_market(t,c,1384+(i*9),1375+(i*7),1362+(i*5),1354+(i*3),2024);}
ps("\n");aron_report();aron_state();ps("\n=== Demo Complete ===\n");return 0;}
