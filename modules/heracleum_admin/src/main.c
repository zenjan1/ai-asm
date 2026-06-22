/* heracleum_admin: Heracleum management technology administration (v1.0)
 * Heracleum planning, heracleum execution, heracleum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hera_t;
typedef struct{int n_herap,n_hera,n_hera2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hera_state_t;
static hera_t heras[N],herae[N-2],hera2[N-4],heraac[N-6],heraam[N-6]; static hera_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(hera_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hera_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HERA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hera_init(void){if(init)return -1;st.n_herap=0;st.n_hera=0;st.n_hera2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)heras[i].active=0;for(int i=0;i<N-2;i++)herae[i].active=0;for(int i=0;i<N-4;i++)hera2[i].active=0;for(int i=0;i<N-6;i++)heraac[i].active=0;for(int i=0;i<N-6;i++)heraam[i].active=0;init=1;ps("[HERA] Heracleum initialized\n");return 0;}
int hera_planning(int t,int c,int a,int b,int d,int e,int y){return add(heras,&st.n_herap,&st.t_f1,N,t,c,a,b,d,e,y);}
int hera_execution(int t,int c,int a,int b,int d,int e,int y){return add(herae,&st.n_hera,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hera_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hera2,&st.n_hera2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hera_accessory(int t,int c,int a,int b,int d,int e,int y){return add(heraac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hera_market(int t,int c,int a,int b,int d,int e,int y){return add(heraam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hera_report(void){ps("[HERA] Herap: ");pi(st.n_herap);ps(" PCS=");pi(st.t_f1);ps("\nHeræ: ");pi(st.n_hera);ps(" PCS=");pi(st.t_f2);ps("\nHera2: ");pi(st.n_hera2);ps(" PCS=");pi(st.t_f3);ps("\nHac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hera_state(void){ps("[HERA] Herap=");pi(st.n_herap);ps(" Hera=");pi(st.n_hera);ps(" Hera2=");pi(st.n_hera2);ps(" Hac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Heracleum Admin Demo ===\n\n");hera_init();
ps("Heracleum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hera_planning(t,c,1529+(i*17),1518+(i*14),1498+(i*10),1480+(i*6),2020+(i%5));}
ps("\nHeracleum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hera_execution(t,c,1518+(i*15),1507+(i*12),1489+(i*8),1476+(i*5),2021+(i%4));}
ps("\nHeracleum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hera_evaluation(t,c,1510+(i*13),1499+(i*10),1483+(i*7),1472+(i*4),2022+(i%3));}
ps("\nHeracleum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hera_accessory(t,c,1502+(i*11),1493+(i*9),1479+(i*6),1469+(i*3),2023+(i%2));}
ps("\nHeracleum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hera_market(t,c,1496+(i*9),1487+(i*7),1474+(i*5),1466+(i*3),2024);}
ps("\n");hera_report();hera_state();ps("\n=== Demo Complete ===\n");return 0;}
