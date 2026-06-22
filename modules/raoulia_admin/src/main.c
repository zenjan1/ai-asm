/* raoulia_admin: Raoulia management technology administration (v1.0)
 * Raoulia planning, raoulia execution, raoulia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rao_t;
typedef struct{int n_raop,n_raoe,n_rao2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} rao_state_t;
static rao_t raops[N],raoss[N-2],raovss[N-4],raoas[N-6],raomks[N-6]; static rao_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rao_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rao_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RAO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rao_init(void){if(init)return -1;st.n_raop=0;st.n_raoe=0;st.n_rao2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)raops[i].active=0;for(int i=0;i<N-2;i++)raoss[i].active=0;for(int i=0;i<N-4;i++)raovss[i].active=0;for(int i=0;i<N-6;i++)raoas[i].active=0;for(int i=0;i<N-6;i++)raomks[i].active=0;init=1;ps("[RAO] Raoulia initialized\n");return 0;}
int rao_planning(int t,int c,int a,int b,int d,int e,int y){return add(raops,&st.n_raop,&st.t_f1,N,t,c,a,b,d,e,y);}
int rao_execution(int t,int c,int a,int b,int d,int e,int y){return add(raoss,&st.n_raoe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rao_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(raovss,&st.n_rao2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rao_accessory(int t,int c,int a,int b,int d,int e,int y){return add(raoas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rao_market(int t,int c,int a,int b,int d,int e,int y){return add(raomks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rao_report(void){ps("[RAO] Raopp: ");pi(st.n_raop);ps(" PCS=");pi(st.t_f1);ps("\nRaoe: ");pi(st.n_raoe);ps(" PCS=");pi(st.t_f2);ps("\nRaov: ");pi(st.n_rao2);ps(" PCS=");pi(st.t_f3);ps("\nRaoc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void rao_state(void){ps("[RAO] Raopp=");pi(st.n_raop);ps(" Raoe=");pi(st.n_raoe);ps(" Raov=");pi(st.n_rao2);ps(" Raoc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Raoulia Admin Demo ===\n\n");rao_init();
ps("Raoulia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rao_planning(t,c,1176+(i*17),1165+(i*14),1145+(i*10),1127+(i*6),2020+(i%5));}
ps("\nRaoulia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rao_execution(t,c,1165+(i*15),1154+(i*12),1136+(i*8),1123+(i*5),2021+(i%4));}
ps("\nRaoulia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rao_evaluation(t,c,1157+(i*13),1146+(i*10),1130+(i*7),1119+(i*4),2022+(i%3));}
ps("\nRaoulia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rao_accessory(t,c,1149+(i*11),1140+(i*9),1126+(i*6),1116+(i*3),2023+(i%2));}
ps("\nRaoulia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rao_market(t,c,1143+(i*9),1134+(i*7),1121+(i*5),1113+(i*3),2024);}
ps("\n");rao_report();rao_state();ps("\n=== Demo Complete ===\n");return 0;}
