/* ardisia_admin: Ardisia management technology administration (v1.0)
 * Ardisia planning, ardisia execution, ardisia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ardi_t;
typedef struct{int n_ardip,n_ardi,n_ardi2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ardi_state_t;
static ardi_t ardis[N],ardies[N-2],ardi2[N-4],ardiac[N-6],ardiam[N-6]; static ardi_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(ardi_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ardi_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ARDI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ardi_init(void){if(init)return -1;st.n_ardip=0;st.n_ardi=0;st.n_ardi2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ardis[i].active=0;for(int i=0;i<N-2;i++)ardies[i].active=0;for(int i=0;i<N-4;i++)ardi2[i].active=0;for(int i=0;i<N-6;i++)ardiac[i].active=0;for(int i=0;i<N-6;i++)ardiam[i].active=0;init=1;ps("[ARDI] Ardisia initialized\n");return 0;}
int ardi_planning(int t,int c,int a,int b,int d,int e,int y){return add(ardis,&st.n_ardip,&st.t_f1,N,t,c,a,b,d,e,y);}
int ardi_execution(int t,int c,int a,int b,int d,int e,int y){return add(ardies,&st.n_ardi,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ardi_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ardi2,&st.n_ardi2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ardi_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ardiac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ardi_market(int t,int c,int a,int b,int d,int e,int y){return add(ardiam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ardi_report(void){ps("[ARDI] Rdip: ");pi(st.n_ardip);ps(" PCS=");pi(st.t_f1);ps("\nRdie: ");pi(st.n_ardi);ps(" PCS=");pi(st.t_f2);ps("\nRdi2: ");pi(st.n_ardi2);ps(" PCS=");pi(st.t_f3);ps("\nRdiac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ardi_state(void){ps("[ARDI] Rdip=");pi(st.n_ardip);ps(" Rdie=");pi(st.n_ardi);ps(" Rdi2=");pi(st.n_ardi2);ps(" Rdiac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Ardisia Admin Demo ===\n\n");ardi_init();
ps("Ardisia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ardi_planning(t,c,1413+(i*17),1402+(i*14),1382+(i*10),1364+(i*6),2020+(i%5));}
ps("\nArdisia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ardi_execution(t,c,1402+(i*15),1391+(i*12),1373+(i*8),1360+(i*5),2021+(i%4));}
ps("\nArdisia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ardi_evaluation(t,c,1394+(i*13),1383+(i*10),1367+(i*7),1356+(i*4),2022+(i%3));}
ps("\nArdisia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ardi_accessory(t,c,1386+(i*11),1377+(i*9),1363+(i*6),1353+(i*3),2023+(i%2));}
ps("\nArdisia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ardi_market(t,c,1380+(i*9),1371+(i*7),1358+(i*5),1350+(i*3),2024);}
ps("\n");ardi_report();ardi_state();ps("\n=== Demo Complete ===\n");return 0;}
