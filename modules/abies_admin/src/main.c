/* abies_admin: Abies management technology administration (v1.0)
 * Abies planning, abies execution, abies evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} abi_t;
typedef struct{int n_abip,n_abie,n_abi2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} abi_state_t;
static abi_t abips[N],abiss[N-2],abivss[N-4],abias[N-6],abimks[N-6]; static abi_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(abi_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;abi_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ABI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int abi_init(void){if(init)return -1;st.n_abip=0;st.n_abie=0;st.n_abi2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)abips[i].active=0;for(int i=0;i<N-2;i++)abiss[i].active=0;for(int i=0;i<N-4;i++)abivss[i].active=0;for(int i=0;i<N-6;i++)abias[i].active=0;for(int i=0;i<N-6;i++)abimks[i].active=0;init=1;ps("[ABI] Abies initialized\n");return 0;}
int abi_planning(int t,int c,int a,int b,int d,int e,int y){return add(abips,&st.n_abip,&st.t_f1,N,t,c,a,b,d,e,y);}
int abi_execution(int t,int c,int a,int b,int d,int e,int y){return add(abiss,&st.n_abie,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int abi_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(abivss,&st.n_abi2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int abi_accessory(int t,int c,int a,int b,int d,int e,int y){return add(abias,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int abi_market(int t,int c,int a,int b,int d,int e,int y){return add(abimks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void abi_report(void){ps("[ABI] Abipp: ");pi(st.n_abip);ps(" PCS=");pi(st.t_f1);ps("\nAbipe: ");pi(st.n_abie);ps(" PCS=");pi(st.t_f2);ps("\nAbiv: ");pi(st.n_abi2);ps(" PCS=");pi(st.t_f3);ps("\nAbic: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void abi_state(void){ps("[ABI] Abipp=");pi(st.n_abip);ps(" Abipe=");pi(st.n_abie);ps(" Abiv=");pi(st.n_abi2);ps(" Abic=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Abies Admin Demo ===\n\n");abi_init();
ps("Abies planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;abi_planning(t,c,1209+(i*17),1198+(i*14),1178+(i*10),1160+(i*6),2020+(i%5));}
ps("\nAbies execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;abi_execution(t,c,1198+(i*15),1187+(i*12),1169+(i*8),1156+(i*5),2021+(i%4));}
ps("\nAbies evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;abi_evaluation(t,c,1190+(i*13),1179+(i*10),1163+(i*7),1152+(i*4),2022+(i%3));}
ps("\nAbies accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;abi_accessory(t,c,1182+(i*11),1173+(i*9),1159+(i*6),1149+(i*3),2023+(i%2));}
ps("\nAbies marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;abi_market(t,c,1176+(i*9),1167+(i*7),1154+(i*5),1146+(i*3),2024);}
ps("\n");abi_report();abi_state();ps("\n=== Demo Complete ===\n");return 0;}
