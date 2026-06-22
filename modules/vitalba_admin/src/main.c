/* vitalba_admin: Vitalba management technology administration (v1.0)
 * Vitalba planning, vitalba execution, vitalba evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} vita_t;
typedef struct{int n_vitap,n_vita,n_vita2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} vita_state_t;
static vita_t vitas[N],vitaess[N-2],vitavss[N-4],vitaas[N-6],vitamks[N-6]; static vita_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(vita_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;vita_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[VITA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int vita_init(void){if(init)return -1;st.n_vitap=0;st.n_vita=0;st.n_vita2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)vitas[i].active=0;for(int i=0;i<N-2;i++)vitaess[i].active=0;for(int i=0;i<N-4;i++)vitavss[i].active=0;for(int i=0;i<N-6;i++)vitaas[i].active=0;for(int i=0;i<N-6;i++)vitamks[i].active=0;init=1;ps("[VITA] Vitalba initialized\n");return 0;}
int vita_planning(int t,int c,int a,int b,int d,int e,int y){return add(vitas,&st.n_vitap,&st.t_f1,N,t,c,a,b,d,e,y);}
int vita_execution(int t,int c,int a,int b,int d,int e,int y){return add(vitaess,&st.n_vita,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int vita_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(vitavss,&st.n_vita2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int vita_accessory(int t,int c,int a,int b,int d,int e,int y){return add(vitaas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int vita_market(int t,int c,int a,int b,int d,int e,int y){return add(vitamks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void vita_report(void){ps("[VITA] Vitap: ");pi(st.n_vitap);ps(" PCS=");pi(st.t_f1);ps("\nVitae: ");pi(st.n_vita);ps(" PCS=");pi(st.t_f2);ps("\nVitav: ");pi(st.n_vita2);ps(" PCS=");pi(st.t_f3);ps("\nVitac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void vita_state(void){ps("[VITA] Vitap=");pi(st.n_vitap);ps(" Vitae=");pi(st.n_vita);ps(" Vitav=");pi(st.n_vita2);ps(" Vitac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Vitalba Admin Demo ===\n\n");vita_init();
ps("Vitalba planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;vita_planning(t,c,1365+(i*17),1354+(i*14),1334+(i*10),1316+(i*6),2020+(i%5));}
ps("\nVitalba execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;vita_execution(t,c,1354+(i*15),1343+(i*12),1325+(i*8),1312+(i*5),2021+(i%4));}
ps("\nVitalba evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;vita_evaluation(t,c,1346+(i*13),1335+(i*10),1319+(i*7),1308+(i*4),2022+(i%3));}
ps("\nVitalba accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vita_accessory(t,c,1338+(i*11),1329+(i*9),1315+(i*6),1305+(i*3),2023+(i%2));}
ps("\nVitalba marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vita_market(t,c,1332+(i*9),1323+(i*7),1310+(i*5),1302+(i*3),2024);}
ps("\n");vita_report();vita_state();ps("\n=== Demo Complete ===\n");return 0;}
