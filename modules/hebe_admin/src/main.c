/* hebe_admin: Hebe management technology administration (v1.0)
 * Hebe planning, hebe execution, hebe evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} he_t;
typedef struct{int n_hep,n_hee,n_hev,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} he_state_t;
static he_t heps[N],hess[N-2],hevss[N-4],heas[N-6],hemks[N-6]; static he_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(he_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;he_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HEB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int heb_init(void){if(init)return -1;st.n_hep=0;st.n_hee=0;st.n_hev=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)heps[i].active=0;for(int i=0;i<N-2;i++)hess[i].active=0;for(int i=0;i<N-4;i++)hevss[i].active=0;for(int i=0;i<N-6;i++)heas[i].active=0;for(int i=0;i<N-6;i++)hemks[i].active=0;init=1;ps("[HEB] Hebe initialized\n");return 0;}
int heb_planning(int t,int c,int a,int b,int d,int e,int y){return add(heps,&st.n_hep,&st.t_f1,N,t,c,a,b,d,e,y);}
int heb_execution(int t,int c,int a,int b,int d,int e,int y){return add(hess,&st.n_hee,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int heb_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hevss,&st.n_hev,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int heb_accessory(int t,int c,int a,int b,int d,int e,int y){return add(heas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int heb_market(int t,int c,int a,int b,int d,int e,int y){return add(hemks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void heb_report(void){ps("[HEB] Hep: ");pi(st.n_hep);ps(" PCS=");pi(st.t_f1);ps("\nHee: ");pi(st.n_hee);ps(" PCS=");pi(st.t_f2);ps("\nHev: ");pi(st.n_hev);ps(" PCS=");pi(st.t_f3);ps("\nHec: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void heb_state(void){ps("[HEB] Hep=");pi(st.n_hep);ps(" Hee=");pi(st.n_hee);ps(" Hev=");pi(st.n_hev);ps(" Hec=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Hebe Admin Demo ===\n\n");heb_init();
ps("Hebe planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;heb_planning(t,c,781+(i*17),770+(i*14),750+(i*10),732+(i*6),2020+(i%5));}
ps("\nHebe execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;heb_execution(t,c,770+(i*15),759+(i*12),741+(i*8),728+(i*5),2021+(i%4));}
ps("\nHebe evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;heb_evaluation(t,c,762+(i*13),751+(i*10),737+(i*7),726+(i*4),2022+(i%3));}
ps("\nHebe accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;heb_accessory(t,c,754+(i*11),745+(i*9),731+(i*6),721+(i*3),2023+(i%2));}
ps("\nHebe marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;heb_market(t,c,748+(i*9),739+(i*7),726+(i*5),718+(i*3),2024);}
ps("\n");heb_report();heb_state();ps("\n=== Demo Complete ===\n");return 0;}
