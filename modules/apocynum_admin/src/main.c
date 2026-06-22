/* apocynum_admin: Apocynum management technology administration (v1.0)
 * Apocynum planning, apocynum execution, apocynum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} apo_t;
typedef struct{int n_apop,n_apoe,n_apo2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} apo_state_t;
static apo_t apops[N],aposs[N-2],apovss[N-4],apoas[N-6],apomks[N-6]; static apo_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(apo_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;apo_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[APO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int apo_init(void){if(init)return -1;st.n_apop=0;st.n_apoe=0;st.n_apo2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)apops[i].active=0;for(int i=0;i<N-2;i++)aposs[i].active=0;for(int i=0;i<N-4;i++)apovss[i].active=0;for(int i=0;i<N-6;i++)apoas[i].active=0;for(int i=0;i<N-6;i++)apomks[i].active=0;init=1;ps("[APO] Apocynum initialized\n");return 0;}
int apo_planning(int t,int c,int a,int b,int d,int e,int y){return add(apops,&st.n_apop,&st.t_f1,N,t,c,a,b,d,e,y);}
int apo_execution(int t,int c,int a,int b,int d,int e,int y){return add(aposs,&st.n_apoe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int apo_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(apovss,&st.n_apo2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int apo_accessory(int t,int c,int a,int b,int d,int e,int y){return add(apoas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int apo_market(int t,int c,int a,int b,int d,int e,int y){return add(apomks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void apo_report(void){ps("[APO] Apopp: ");pi(st.n_apop);ps(" PCS=");pi(st.t_f1);ps("\nApoe: ");pi(st.n_apoe);ps(" PCS=");pi(st.t_f2);ps("\nApov: ");pi(st.n_apo2);ps(" PCS=");pi(st.t_f3);ps("\nApoc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void apo_state(void){ps("[APO] Apopp=");pi(st.n_apop);ps(" Apoe=");pi(st.n_apoe);ps(" Apov=");pi(st.n_apo2);ps(" Apoc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Apocynum Admin Demo ===\n\n");apo_init();
ps("Apocynum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;apo_planning(t,c,1119+(i*17),1108+(i*14),1088+(i*10),1070+(i*6),2020+(i%5));}
ps("\nApocynum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;apo_execution(t,c,1108+(i*15),1097+(i*12),1079+(i*8),1066+(i*5),2021+(i%4));}
ps("\nApocynum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;apo_evaluation(t,c,1100+(i*13),1089+(i*10),1073+(i*7),1062+(i*4),2022+(i%3));}
ps("\nApocynum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;apo_accessory(t,c,1092+(i*11),1083+(i*9),1069+(i*6),1059+(i*3),2023+(i%2));}
ps("\nApocynum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;apo_market(t,c,1086+(i*9),1077+(i*7),1064+(i*5),1056+(i*3),2024);}
ps("\n");apo_report();apo_state();ps("\n=== Demo Complete ===\n");return 0;}
