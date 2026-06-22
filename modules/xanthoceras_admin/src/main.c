/* xanthoceras_admin: Xanthoceras management technology administration (v1.0)
 * Xanthoceras planning, xanthoceras execution, xanthoceras evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} xan_t;
typedef struct{int n_xanp,n_xane,n_xan2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} xan_state_t;
static xan_t xanps[N],xanss[N-2],xanvss[N-4],xanas[N-6],xanmks[N-6]; static xan_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(xan_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;xan_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[XAN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int xan_init(void){if(init)return -1;st.n_xanp=0;st.n_xane=0;st.n_xan2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)xanps[i].active=0;for(int i=0;i<N-2;i++)xanss[i].active=0;for(int i=0;i<N-4;i++)xanvss[i].active=0;for(int i=0;i<N-6;i++)xanas[i].active=0;for(int i=0;i<N-6;i++)xanmks[i].active=0;init=1;ps("[XAN] Xanthoceras initialized\n");return 0;}
int xan_planning(int t,int c,int a,int b,int d,int e,int y){return add(xanps,&st.n_xanp,&st.t_f1,N,t,c,a,b,d,e,y);}
int xan_execution(int t,int c,int a,int b,int d,int e,int y){return add(xanss,&st.n_xane,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int xan_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(xanvss,&st.n_xan2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int xan_accessory(int t,int c,int a,int b,int d,int e,int y){return add(xanas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int xan_market(int t,int c,int a,int b,int d,int e,int y){return add(xanmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void xan_report(void){ps("[XAN] Xanpp: ");pi(st.n_xanp);ps(" PCS=");pi(st.t_f1);ps("\nXane: ");pi(st.n_xane);ps(" PCS=");pi(st.t_f2);ps("\nXanv: ");pi(st.n_xan2);ps(" PCS=");pi(st.t_f3);ps("\nXanc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void xan_state(void){ps("[XAN] Xanpp=");pi(st.n_xanp);ps(" Xane=");pi(st.n_xane);ps(" Xanv=");pi(st.n_xan2);ps(" Xanc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Xanthoceras Admin Demo ===\n\n");xan_init();
ps("Xanthoceras planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;xan_planning(t,c,1203+(i*17),1192+(i*14),1172+(i*10),1154+(i*6),2020+(i%5));}
ps("\nXanthoceras execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;xan_execution(t,c,1192+(i*15),1181+(i*12),1163+(i*8),1150+(i*5),2021+(i%4));}
ps("\nXanthoceras evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;xan_evaluation(t,c,1184+(i*13),1173+(i*10),1157+(i*7),1146+(i*4),2022+(i%3));}
ps("\nXanthoceras accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;xan_accessory(t,c,1176+(i*11),1167+(i*9),1153+(i*6),1143+(i*3),2023+(i%2));}
ps("\nXanthoceras marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;xan_market(t,c,1170+(i*9),1161+(i*7),1148+(i*5),1140+(i*3),2024);}
ps("\n");xan_report();xan_state();ps("\n=== Demo Complete ===\n");return 0;}
