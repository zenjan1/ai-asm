/* aponogeton_admin: Aponogeton management technology administration (v1.0)
 * Aponogeton planning, aponogeton execution, aponogeton evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} apg_t;
typedef struct{int n_apgp,n_apge,n_apg2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} apg_state_t;
static apg_t apgps[N],apgss[N-2],apgvss[N-4],apgas[N-6],apgmks[N-6]; static apg_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(apg_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;apg_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[APG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int apg_init(void){if(init)return -1;st.n_apgp=0;st.n_apge=0;st.n_apg2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)apgps[i].active=0;for(int i=0;i<N-2;i++)apgss[i].active=0;for(int i=0;i<N-4;i++)apgvss[i].active=0;for(int i=0;i<N-6;i++)apgas[i].active=0;for(int i=0;i<N-6;i++)apgmks[i].active=0;init=1;ps("[APG] Aponogeton initialized\n");return 0;}
int apg_planning(int t,int c,int a,int b,int d,int e,int y){return add(apgps,&st.n_apgp,&st.t_f1,N,t,c,a,b,d,e,y);}
int apg_execution(int t,int c,int a,int b,int d,int e,int y){return add(apgss,&st.n_apge,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int apg_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(apgvss,&st.n_apg2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int apg_accessory(int t,int c,int a,int b,int d,int e,int y){return add(apgas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int apg_market(int t,int c,int a,int b,int d,int e,int y){return add(apgmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void apg_report(void){ps("[APG] Apopp: ");pi(st.n_apgp);ps(" PCS=");pi(st.t_f1);ps("\nApope: ");pi(st.n_apge);ps(" PCS=");pi(st.t_f2);ps("\nApov: ");pi(st.n_apg2);ps(" PCS=");pi(st.t_f3);ps("\nApoc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void apg_state(void){ps("[APG] Apopp=");pi(st.n_apgp);ps(" Apope=");pi(st.n_apge);ps(" Apov=");pi(st.n_apg2);ps(" Apoc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Aponogeton Admin Demo ===\n\n");apg_init();
ps("Aponogeton planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;apg_planning(t,c,1217+(i*17),1206+(i*14),1186+(i*10),1168+(i*6),2020+(i%5));}
ps("\nAponogeton execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;apg_execution(t,c,1206+(i*15),1195+(i*12),1177+(i*8),1164+(i*5),2021+(i%4));}
ps("\nAponogeton evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;apg_evaluation(t,c,1198+(i*13),1187+(i*10),1171+(i*7),1160+(i*4),2022+(i%3));}
ps("\nAponogeton accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;apg_accessory(t,c,1190+(i*11),1181+(i*9),1167+(i*6),1157+(i*3),2023+(i%2));}
ps("\nAponogeton marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;apg_market(t,c,1184+(i*9),1175+(i*7),1162+(i*5),1154+(i*3),2024);}
ps("\n");apg_report();apg_state();ps("\n=== Demo Complete ===\n");return 0;}
