/* gnaphalium_admin: Gnaphalium management technology administration (v1.0)
 * Gnaphalium planning, gnaphalium execution, gnaphalium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gnap_t;
typedef struct{int n_gnapp,n_gnap,n_gnap2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gnap_state_t;
static gnap_t gnaps[N],gnape[N-2],gnap2[N-4],gnapac[N-6],gnapam[N-6]; static gnap_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(gnap_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gnap_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GNAP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gnap_init(void){if(init)return -1;st.n_gnapp=0;st.n_gnap=0;st.n_gnap2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)gnaps[i].active=0;for(int i=0;i<N-2;i++)gnape[i].active=0;for(int i=0;i<N-4;i++)gnap2[i].active=0;for(int i=0;i<N-6;i++)gnapac[i].active=0;for(int i=0;i<N-6;i++)gnapam[i].active=0;init=1;ps("[GNAP] Gnaphalium initialized\n");return 0;}
int gnap_planning(int t,int c,int a,int b,int d,int e,int y){return add(gnaps,&st.n_gnapp,&st.t_f1,N,t,c,a,b,d,e,y);}
int gnap_execution(int t,int c,int a,int b,int d,int e,int y){return add(gnape,&st.n_gnap,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gnap_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(gnap2,&st.n_gnap2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gnap_accessory(int t,int c,int a,int b,int d,int e,int y){return add(gnapac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gnap_market(int t,int c,int a,int b,int d,int e,int y){return add(gnapam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gnap_report(void){ps("[GNAP] Gnapp: ");pi(st.n_gnapp);ps(" PCS=");pi(st.t_f1);ps("\nGnape: ");pi(st.n_gnap);ps(" PCS=");pi(st.t_f2);ps("\nGnap2: ");pi(st.n_gnap2);ps(" PCS=");pi(st.t_f3);ps("\nGnac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gnap_state(void){ps("[GNAP] Gnapp=");pi(st.n_gnapp);ps(" Gnap=");pi(st.n_gnap);ps(" Gnap2=");pi(st.n_gnap2);ps(" Gnac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Gnaphalium Admin Demo ===\n\n");gnap_init();
ps("Gnaphalium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gnap_planning(t,c,1518+(i*17),1507+(i*14),1487+(i*10),1469+(i*6),2020+(i%5));}
ps("\nGnaphalium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gnap_execution(t,c,1507+(i*15),1496+(i*12),1478+(i*8),1465+(i*5),2021+(i%4));}
ps("\nGnaphalium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gnap_evaluation(t,c,1499+(i*13),1488+(i*10),1472+(i*7),1461+(i*4),2022+(i%3));}
ps("\nGnaphalium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gnap_accessory(t,c,1491+(i*11),1482+(i*9),1468+(i*6),1458+(i*3),2023+(i%2));}
ps("\nGnaphalium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gnap_market(t,c,1485+(i*9),1476+(i*7),1463+(i*5),1455+(i*3),2024);}
ps("\n");gnap_report();gnap_state();ps("\n=== Demo Complete ===\n");return 0;}
