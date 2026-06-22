/* telekia_admin: Telekia management technology administration (v1.0)
 * Telekia planning, telekia execution, telekia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tele_t;
typedef struct{int n_telep,n_tele,n_tele2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} tele_state_t;
static tele_t teles[N],teless[N-2],televss[N-4],teleas[N-6],telemks[N-6]; static tele_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(tele_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tele_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TELE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tele_init(void){if(init)return -1;st.n_telep=0;st.n_tele=0;st.n_tele2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)teles[i].active=0;for(int i=0;i<N-2;i++)teless[i].active=0;for(int i=0;i<N-4;i++)televss[i].active=0;for(int i=0;i<N-6;i++)teleas[i].active=0;for(int i=0;i<N-6;i++)telemks[i].active=0;init=1;ps("[TELE] Telekia initialized\n");return 0;}
int tele_planning(int t,int c,int a,int b,int d,int e,int y){return add(teles,&st.n_telep,&st.t_f1,N,t,c,a,b,d,e,y);}
int tele_execution(int t,int c,int a,int b,int d,int e,int y){return add(teless,&st.n_tele,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tele_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(televss,&st.n_tele2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tele_accessory(int t,int c,int a,int b,int d,int e,int y){return add(teleas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tele_market(int t,int c,int a,int b,int d,int e,int y){return add(telemks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tele_report(void){ps("[TELE] Telep: ");pi(st.n_telep);ps(" PCS=");pi(st.t_f1);ps("\nTelee: ");pi(st.n_tele);ps(" PCS=");pi(st.t_f2);ps("\nTelev: ");pi(st.n_tele2);ps(" PCS=");pi(st.t_f3);ps("\nTelec: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void tele_state(void){ps("[TELE] Telep=");pi(st.n_telep);ps(" Telee=");pi(st.n_tele);ps(" Telev=");pi(st.n_tele2);ps(" Telec=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Telekia Admin Demo ===\n\n");tele_init();
ps("Telekia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tele_planning(t,c,1347+(i*17),1336+(i*14),1316+(i*10),1298+(i*6),2020+(i%5));}
ps("\nTelekia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tele_execution(t,c,1336+(i*15),1325+(i*12),1307+(i*8),1294+(i*5),2021+(i%4));}
ps("\nTelekia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tele_evaluation(t,c,1328+(i*13),1317+(i*10),1301+(i*7),1290+(i*4),2022+(i%3));}
ps("\nTelekia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tele_accessory(t,c,1320+(i*11),1311+(i*9),1297+(i*6),1287+(i*3),2023+(i%2));}
ps("\nTelekia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tele_market(t,c,1314+(i*9),1305+(i*7),1292+(i*5),1284+(i*3),2024);}
ps("\n");tele_report();tele_state();ps("\n=== Demo Complete ===\n");return 0;}
