/* canna_admin: Canna management technology administration (v1.0)
 * Canna planning, canna execution, canna evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} can_t;
typedef struct{int n_canp,n_canc,n_can2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} can_state_t;
static can_t canps[N],canas2[N-2],canvss[N-4],canas[N-6],canmks[N-6]; static can_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(can_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;can_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CAN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int can_init(void){if(init)return -1;st.n_canp=0;st.n_canc=0;st.n_can2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)canps[i].active=0;for(int i=0;i<N-2;i++)canas2[i].active=0;for(int i=0;i<N-4;i++)canvss[i].active=0;for(int i=0;i<N-6;i++)canas[i].active=0;for(int i=0;i<N-6;i++)canmks[i].active=0;init=1;ps("[CAN] Canna initialized\n");return 0;}
int can_planning(int t,int c,int a,int b,int d,int e,int y){return add(canps,&st.n_canp,&st.t_f1,N,t,c,a,b,d,e,y);}
int can_execution(int t,int c,int a,int b,int d,int e,int y){return add(canas2,&st.n_canc,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int can_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(canvss,&st.n_can2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int can_accessory(int t,int c,int a,int b,int d,int e,int y){return add(canas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int can_market(int t,int c,int a,int b,int d,int e,int y){return add(canmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void can_report(void){ps("[CAN] Canp: ");pi(st.n_canp);ps(" PCS=");pi(st.t_f1);ps("\nCanc: ");pi(st.n_canc);ps(" PCS=");pi(st.t_f2);ps("\nCanv: ");pi(st.n_can2);ps(" PCS=");pi(st.t_f3);ps("\nCac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void can_state(void){ps("[CAN] Canp=");pi(st.n_canp);ps(" Canc=");pi(st.n_canc);ps(" Canv=");pi(st.n_can2);ps(" Cac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Canna Admin Demo ===\n\n");can_init();
ps("Canna planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;can_planning(t,c,904+(i*17),893+(i*14),873+(i*10),855+(i*6),2020+(i%5));}
ps("\nCanna execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;can_execution(t,c,893+(i*15),882+(i*12),864+(i*8),851+(i*5),2021+(i%4));}
ps("\nCanna evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;can_evaluation(t,c,885+(i*13),874+(i*10),858+(i*7),847+(i*4),2022+(i%3));}
ps("\nCanna accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;can_accessory(t,c,877+(i*11),868+(i*9),854+(i*6),844+(i*3),2023+(i%2));}
ps("\nCanna marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;can_market(t,c,871+(i*9),862+(i*7),849+(i*5),841+(i*3),2024);}
ps("\n");can_report();can_state();ps("\n=== Demo Complete ===\n");return 0;}
