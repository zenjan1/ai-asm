/* echium_admin: Echium management technology administration (v1.0)
 * Echium planning, echium execution, echium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ec_t;
typedef struct{int n_ecp,n_ece,n_ecv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ec_state_t;
static ec_t ecps[N],ecss[N-2],ecvss[N-4],ecas[N-6],ecmks[N-6]; static ec_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ec_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ec_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ECH] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ech_init(void){if(init)return -1;st.n_ecp=0;st.n_ece=0;st.n_ecv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ecps[i].active=0;for(int i=0;i<N-2;i++)ecss[i].active=0;for(int i=0;i<N-4;i++)ecvss[i].active=0;for(int i=0;i<N-6;i++)ecas[i].active=0;for(int i=0;i<N-6;i++)ecmks[i].active=0;init=1;ps("[ECH] Echium initialized\n");return 0;}
int ech_planning(int t,int c,int a,int b,int d,int e,int y){return add(ecps,&st.n_ecp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ech_execution(int t,int c,int a,int b,int d,int e,int y){return add(ecss,&st.n_ece,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ech_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ecvss,&st.n_ecv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ech_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ecas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ech_market(int t,int c,int a,int b,int d,int e,int y){return add(ecmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ech_report(void){ps("[ECH] Ecp: ");pi(st.n_ecp);ps(" PCS=");pi(st.t_f1);ps("\nEce: ");pi(st.n_ece);ps(" PCS=");pi(st.t_f2);ps("\nEcv: ");pi(st.n_ecv);ps(" PCS=");pi(st.t_f3);ps("\nEcc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ech_state(void){ps("[ECH] Ecp=");pi(st.n_ecp);ps(" Ece=");pi(st.n_ece);ps(" Ecv=");pi(st.n_ecv);ps(" Ecc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Echium Admin Demo ===\n\n");ech_init();
ps("Echium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ech_planning(t,c,765+(i*17),754+(i*14),734+(i*10),716+(i*6),2020+(i%5));}
ps("\nEchium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ech_execution(t,c,754+(i*15),743+(i*12),725+(i*8),712+(i*5),2021+(i%4));}
ps("\nEchium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ech_evaluation(t,c,746+(i*13),735+(i*10),721+(i*7),710+(i*4),2022+(i%3));}
ps("\nEchium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ech_accessory(t,c,738+(i*11),729+(i*9),715+(i*6),705+(i*3),2023+(i%2));}
ps("\nEchium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ech_market(t,c,732+(i*9),723+(i*7),710+(i*5),702+(i*3),2024);}
ps("\n");ech_report();ech_state();ps("\n=== Demo Complete ===\n");return 0;}
