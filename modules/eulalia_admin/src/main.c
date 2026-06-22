/* eulalia_admin: Eulalia management technology administration (v1.0)
 * Eulalia planning, eulalia execution, eulalia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} eul_t;
typedef struct{int n_eulp,n_eule,n_eul2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} eul_state_t;
static eul_t eulps[N],eulss[N-2],eulvss[N-4],eulas[N-6],eulmks[N-6]; static eul_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(eul_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;eul_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[EUL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int eul_init(void){if(init)return -1;st.n_eulp=0;st.n_eule=0;st.n_eul2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)eulps[i].active=0;for(int i=0;i<N-2;i++)eulss[i].active=0;for(int i=0;i<N-4;i++)eulvss[i].active=0;for(int i=0;i<N-6;i++)eulas[i].active=0;for(int i=0;i<N-6;i++)eulmks[i].active=0;init=1;ps("[EUL] Eulalia initialized\n");return 0;}
int eul_planning(int t,int c,int a,int b,int d,int e,int y){return add(eulps,&st.n_eulp,&st.t_f1,N,t,c,a,b,d,e,y);}
int eul_execution(int t,int c,int a,int b,int d,int e,int y){return add(eulss,&st.n_eule,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int eul_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(eulvss,&st.n_eul2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int eul_accessory(int t,int c,int a,int b,int d,int e,int y){return add(eulas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int eul_market(int t,int c,int a,int b,int d,int e,int y){return add(eulmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void eul_report(void){ps("[EUL] Eulpp: ");pi(st.n_eulp);ps(" PCS=");pi(st.t_f1);ps("\nEule: ");pi(st.n_eule);ps(" PCS=");pi(st.t_f2);ps("\nEulv: ");pi(st.n_eul2);ps(" PCS=");pi(st.t_f3);ps("\nEulc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void eul_state(void){ps("[EUL] Eulpp=");pi(st.n_eulp);ps(" Eule=");pi(st.n_eule);ps(" Eulv=");pi(st.n_eul2);ps(" Eulc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Eulalia Admin Demo ===\n\n");eul_init();
ps("Eulalia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;eul_planning(t,c,1139+(i*17),1128+(i*14),1108+(i*10),1090+(i*6),2020+(i%5));}
ps("\nEulalia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;eul_execution(t,c,1128+(i*15),1117+(i*12),1099+(i*8),1086+(i*5),2021+(i%4));}
ps("\nEulalia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;eul_evaluation(t,c,1120+(i*13),1109+(i*10),1093+(i*7),1082+(i*4),2022+(i%3));}
ps("\nEulalia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;eul_accessory(t,c,1112+(i*11),1103+(i*9),1089+(i*6),1079+(i*3),2023+(i%2));}
ps("\nEulalia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;eul_market(t,c,1106+(i*9),1097+(i*7),1084+(i*5),1076+(i*3),2024);}
ps("\n");eul_report();eul_state();ps("\n=== Demo Complete ===\n");return 0;}
