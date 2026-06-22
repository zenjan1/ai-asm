/* heliamphora_admin: Heliamphora management technology administration (v1.0)
 * Heliamphora planning, heliamphora execution, heliamphora evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} heli_t;
typedef struct{int n_helip,n_heli,n_heli2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} heli_state_t;
static heli_t helis[N],helie[N-2],heli2[N-4],heliac[N-6],heliam[N-6]; static heli_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(heli_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;heli_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HELI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int heli_init(void){if(init)return -1;st.n_helip=0;st.n_heli=0;st.n_heli2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)helis[i].active=0;for(int i=0;i<N-2;i++)helie[i].active=0;for(int i=0;i<N-4;i++)heli2[i].active=0;for(int i=0;i<N-6;i++)heliac[i].active=0;for(int i=0;i<N-6;i++)heliam[i].active=0;init=1;ps("[HELI] Heliamphora initialized\n");return 0;}
int heli_planning(int t,int c,int a,int b,int d,int e,int y){return add(helis,&st.n_helip,&st.t_f1,N,t,c,a,b,d,e,y);}
int heli_execution(int t,int c,int a,int b,int d,int e,int y){return add(helie,&st.n_heli,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int heli_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(heli2,&st.n_heli2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int heli_accessory(int t,int c,int a,int b,int d,int e,int y){return add(heliac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int heli_market(int t,int c,int a,int b,int d,int e,int y){return add(heliam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void heli_report(void){ps("[HELI] Helip: ");pi(st.n_helip);ps(" PCS=");pi(st.t_f1);ps("\nHelie: ");pi(st.n_heli);ps(" PCS=");pi(st.t_f2);ps("\nHeli2: ");pi(st.n_heli2);ps(" PCS=");pi(st.t_f3);ps("\nHac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void heli_state(void){ps("[HELI] Helip=");pi(st.n_helip);ps(" Heli=");pi(st.n_heli);ps(" Heli2=");pi(st.n_heli2);ps(" Hac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Heliamphora Admin Demo ===\n\n");heli_init();
ps("Heliamphora planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;heli_planning(t,c,1526+(i*17),1515+(i*14),1495+(i*10),1477+(i*6),2020+(i%5));}
ps("\nHeliamphora execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;heli_execution(t,c,1515+(i*15),1504+(i*12),1486+(i*8),1473+(i*5),2021+(i%4));}
ps("\nHeliamphora evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;heli_evaluation(t,c,1507+(i*13),1496+(i*10),1480+(i*7),1469+(i*4),2022+(i%3));}
ps("\nHeliamphora accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;heli_accessory(t,c,1499+(i*11),1490+(i*9),1476+(i*6),1466+(i*3),2023+(i%2));}
ps("\nHeliamphora marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;heli_market(t,c,1493+(i*9),1484+(i*7),1471+(i*5),1463+(i*3),2024);}
ps("\n");heli_report();heli_state();ps("\n=== Demo Complete ===\n");return 0;}
