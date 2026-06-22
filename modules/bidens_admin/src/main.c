/* bidens_admin: Bidens management technology administration (v1.0)
 * Bidens planning, bidens execution, bidens evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} bide_t;
typedef struct{int n_bidep,n_bide,n_bide2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} bide_state_t;
static bide_t bides[N],bidee[N-2],bide2[N-4],bideac[N-6],bideam[N-6]; static bide_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(bide_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;bide_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BIDE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int bide_init(void){if(init)return -1;st.n_bidep=0;st.n_bide=0;st.n_bide2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)bides[i].active=0;for(int i=0;i<N-2;i++)bidee[i].active=0;for(int i=0;i<N-4;i++)bide2[i].active=0;for(int i=0;i<N-6;i++)bideac[i].active=0;for(int i=0;i<N-6;i++)bideam[i].active=0;init=1;ps("[BIDE] Bidens initialized\n");return 0;}
int bide_planning(int t,int c,int a,int b,int d,int e,int y){return add(bides,&st.n_bidep,&st.t_f1,N,t,c,a,b,d,e,y);}
int bide_execution(int t,int c,int a,int b,int d,int e,int y){return add(bidee,&st.n_bide,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int bide_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(bide2,&st.n_bide2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int bide_accessory(int t,int c,int a,int b,int d,int e,int y){return add(bideac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int bide_market(int t,int c,int a,int b,int d,int e,int y){return add(bideam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void bide_report(void){ps("[BIDE] Idep: ");pi(st.n_bidep);ps(" PCS=");pi(st.t_f1);ps("\nIdee: ");pi(st.n_bide);ps(" PCS=");pi(st.t_f2);ps("\nIde2: ");pi(st.n_bide2);ps(" PCS=");pi(st.t_f3);ps("\nIdeac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void bide_state(void){ps("[BIDE] Idep=");pi(st.n_bidep);ps(" Ide=");pi(st.n_bide);ps(" Ide2=");pi(st.n_bide2);ps(" Ideac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Bidens Admin Demo ===\n\n");bide_init();
ps("Bidens planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;bide_planning(t,c,1437+(i*17),1426+(i*14),1406+(i*10),1388+(i*6),2020+(i%5));}
ps("\nBidens execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;bide_execution(t,c,1426+(i*15),1415+(i*12),1397+(i*8),1384+(i*5),2021+(i%4));}
ps("\nBidens evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;bide_evaluation(t,c,1418+(i*13),1407+(i*10),1391+(i*7),1380+(i*4),2022+(i%3));}
ps("\nBidens accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bide_accessory(t,c,1410+(i*11),1401+(i*9),1387+(i*6),1377+(i*3),2023+(i%2));}
ps("\nBidens marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bide_market(t,c,1404+(i*9),1395+(i*7),1382+(i*5),1374+(i*3),2024);}
ps("\n");bide_report();bide_state();ps("\n=== Demo Complete ===\n");return 0;}
