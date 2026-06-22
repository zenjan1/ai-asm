/* dracunculus_admin: Dracunculus management technology administration (v1.0)
 * Dracunculus planning, dracunculus execution, dracunculus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} drac_t;
typedef struct{int n_dracp,n_drac,n_drac2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} drac_state_t;
static drac_t dracs[N],drace[N-2],drac2[N-4],dracac[N-6],dracam[N-6]; static drac_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(drac_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;drac_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DRAC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int drac_init(void){if(init)return -1;st.n_dracp=0;st.n_drac=0;st.n_drac2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dracs[i].active=0;for(int i=0;i<N-2;i++)drace[i].active=0;for(int i=0;i<N-4;i++)drac2[i].active=0;for(int i=0;i<N-6;i++)dracac[i].active=0;for(int i=0;i<N-6;i++)dracam[i].active=0;init=1;ps("[DRAC] Dracunculus initialized\n");return 0;}
int drac_planning(int t,int c,int a,int b,int d,int e,int y){return add(dracs,&st.n_dracp,&st.t_f1,N,t,c,a,b,d,e,y);}
int drac_execution(int t,int c,int a,int b,int d,int e,int y){return add(drace,&st.n_drac,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int drac_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(drac2,&st.n_drac2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int drac_accessory(int t,int c,int a,int b,int d,int e,int y){return add(dracac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int drac_market(int t,int c,int a,int b,int d,int e,int y){return add(dracam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void drac_report(void){ps("[DRAC] Acp: ");pi(st.n_dracp);ps(" PCS=");pi(st.t_f1);ps("\nAce: ");pi(st.n_drac);ps(" PCS=");pi(st.t_f2);ps("\nAc2: ");pi(st.n_drac2);ps(" PCS=");pi(st.t_f3);ps("\nAac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void drac_state(void){ps("[DRAC] Acp=");pi(st.n_dracp);ps(" Ac=");pi(st.n_drac);ps(" Ac2=");pi(st.n_drac2);ps(" Aac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Dracunculus Admin Demo ===\n\n");drac_init();
ps("Dracunculus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;drac_planning(t,c,1499+(i*17),1488+(i*14),1468+(i*10),1450+(i*6),2020+(i%5));}
ps("\nDracunculus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;drac_execution(t,c,1488+(i*15),1477+(i*12),1459+(i*8),1446+(i*5),2021+(i%4));}
ps("\nDracunculus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;drac_evaluation(t,c,1480+(i*13),1469+(i*10),1453+(i*7),1442+(i*4),2022+(i%3));}
ps("\nDracunculus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;drac_accessory(t,c,1472+(i*11),1463+(i*9),1449+(i*6),1439+(i*3),2023+(i%2));}
ps("\nDracunculus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;drac_market(t,c,1466+(i*9),1457+(i*7),1444+(i*5),1436+(i*3),2024);}
ps("\n");drac_report();drac_state();ps("\n=== Demo Complete ===\n");return 0;}
