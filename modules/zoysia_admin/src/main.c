/* zoysia_admin: Zoysia management technology administration (v1.0)
 * Zoysia planning, zoysia execution, zoysia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} zoy_t;
typedef struct{int n_zoyp,n_zoye,n_zoy2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} zoy_state_t;
static zoy_t zoyps[N],zoyss[N-2],zoyvss[N-4],zoyas[N-6],zoymks[N-6]; static zoy_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(zoy_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;zoy_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ZOY] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int zoy_init(void){if(init)return -1;st.n_zoyp=0;st.n_zoye=0;st.n_zoy2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)zoyps[i].active=0;for(int i=0;i<N-2;i++)zoyss[i].active=0;for(int i=0;i<N-4;i++)zoyvss[i].active=0;for(int i=0;i<N-6;i++)zoyas[i].active=0;for(int i=0;i<N-6;i++)zoymks[i].active=0;init=1;ps("[ZOY] Zoysia initialized\n");return 0;}
int zoy_planning(int t,int c,int a,int b,int d,int e,int y){return add(zoyps,&st.n_zoyp,&st.t_f1,N,t,c,a,b,d,e,y);}
int zoy_execution(int t,int c,int a,int b,int d,int e,int y){return add(zoyss,&st.n_zoye,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int zoy_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(zoyvss,&st.n_zoy2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int zoy_accessory(int t,int c,int a,int b,int d,int e,int y){return add(zoyas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int zoy_market(int t,int c,int a,int b,int d,int e,int y){return add(zoymks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void zoy_report(void){ps("[ZOY] Zoypp: ");pi(st.n_zoyp);ps(" PCS=");pi(st.t_f1);ps("\nZoye: ");pi(st.n_zoye);ps(" PCS=");pi(st.t_f2);ps("\nZoyv: ");pi(st.n_zoy2);ps(" PCS=");pi(st.t_f3);ps("\nZoyc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void zoy_state(void){ps("[ZOY] Zoypp=");pi(st.n_zoyp);ps(" Zoye=");pi(st.n_zoye);ps(" Zoyv=");pi(st.n_zoy2);ps(" Zoyc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Zoysia Admin Demo ===\n\n");zoy_init();
ps("Zoysia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;zoy_planning(t,c,1114+(i*17),1103+(i*14),1083+(i*10),1065+(i*6),2020+(i%5));}
ps("\nZoysia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;zoy_execution(t,c,1103+(i*15),1092+(i*12),1074+(i*8),1061+(i*5),2021+(i%4));}
ps("\nZoysia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;zoy_evaluation(t,c,1095+(i*13),1084+(i*10),1068+(i*7),1057+(i*4),2022+(i%3));}
ps("\nZoysia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zoy_accessory(t,c,1087+(i*11),1078+(i*9),1064+(i*6),1054+(i*3),2023+(i%2));}
ps("\nZoysia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zoy_market(t,c,1081+(i*9),1072+(i*7),1059+(i*5),1051+(i*3),2024);}
ps("\n");zoy_report();zoy_state();ps("\n=== Demo Complete ===\n");return 0;}
