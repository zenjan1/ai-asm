/* nemesia_admin: Nemesia management technology administration (v1.0)
 * Nemesia planning, nemesia execution, nemesia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} nem_t;
typedef struct{int n_nep,n_nee,n_nev,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} nem_state_t;
static nem_t neps[N],ness[N-2],nevss[N-4],neas[N-6],nemks[N-6]; static nem_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(nem_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;nem_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NEM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int nem_init(void){if(init)return -1;st.n_nep=0;st.n_nee=0;st.n_nev=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)neps[i].active=0;for(int i=0;i<N-2;i++)ness[i].active=0;for(int i=0;i<N-4;i++)nevss[i].active=0;for(int i=0;i<N-6;i++)neas[i].active=0;for(int i=0;i<N-6;i++)nemks[i].active=0;init=1;ps("[NEM] Nemesia initialized\n");return 0;}
int nem_planning(int t,int c,int a,int b,int d,int e,int y){return add(neps,&st.n_nep,&st.t_f1,N,t,c,a,b,d,e,y);}
int nem_execution(int t,int c,int a,int b,int d,int e,int y){return add(ness,&st.n_nee,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int nem_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(nevss,&st.n_nev,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int nem_accessory(int t,int c,int a,int b,int d,int e,int y){return add(neas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int nem_market(int t,int c,int a,int b,int d,int e,int y){return add(nemks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void nem_report(void){ps("[NEM] Nep: ");pi(st.n_nep);ps(" PCS=");pi(st.t_f1);ps("\nNee: ");pi(st.n_nee);ps(" PCS=");pi(st.t_f2);ps("\nNev: ");pi(st.n_nev);ps(" PCS=");pi(st.t_f3);ps("\nNec: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void nem_state(void){ps("[NEM] Nep=");pi(st.n_nep);ps(" Nee=");pi(st.n_nee);ps(" Nev=");pi(st.n_nev);ps(" Nec=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Nemesia Admin Demo ===\n\n");nem_init();
ps("Nemesia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;nem_planning(t,c,793+(i*17),782+(i*14),762+(i*10),744+(i*6),2020+(i%5));}
ps("\nNemesia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;nem_execution(t,c,782+(i*15),771+(i*12),753+(i*8),740+(i*5),2021+(i%4));}
ps("\nNemesia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;nem_evaluation(t,c,774+(i*13),763+(i*10),749+(i*7),738+(i*4),2022+(i%3));}
ps("\nNemesia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nem_accessory(t,c,766+(i*11),757+(i*9),743+(i*6),733+(i*3),2023+(i%2));}
ps("\nNemesia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nem_market(t,c,760+(i*9),751+(i*7),738+(i*5),730+(i*3),2024);}
ps("\n");nem_report();nem_state();ps("\n=== Demo Complete ===\n");return 0;}
