/* robotech_admin: Robot technology administration (v1.0)
 * Industrial robots, service robots, special robots, robot control, robot sensing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rbt_t;
typedef struct{int n_in,n_sv,n_sp,n_ct,n_sn,t_f1,t_f2,t_f3,t_f4,t_f5;} rbt_state_t;
static rbt_t ins[N],svs[N-2],sps[N-4],cts[N-6],sns[N-6]; static rbt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rbt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rbt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RBT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rbt_init(void){if(init)return -1;st.n_in=0;st.n_sv=0;st.n_sp=0;st.n_ct=0;st.n_sn=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ins[i].active=0;for(int i=0;i<N-2;i++)svs[i].active=0;for(int i=0;i<N-4;i++)sps[i].active=0;for(int i=0;i<N-6;i++)cts[i].active=0;for(int i=0;i<N-6;i++)sns[i].active=0;init=1;ps("[RBT] Robotech initialized\n");return 0;}
int rbt_industrial(int t,int c,int a,int b,int d,int e,int y){return add(ins,&st.n_in,&st.t_f1,N,t,c,a,b,d,e,y);}
int rbt_service(int t,int c,int a,int b,int d,int e,int y){return add(svs,&st.n_sv,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rbt_special(int t,int c,int a,int b,int d,int e,int y){return add(sps,&st.n_sp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rbt_control(int t,int c,int a,int b,int d,int e,int y){return add(cts,&st.n_ct,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rbt_sensing(int t,int c,int a,int b,int d,int e,int y){return add(sns,&st.n_sn,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rbt_report(void){ps("[RBT] Ind: ");pi(st.n_in);ps(" DoF=");pi(st.t_f1);ps("\nSvc: ");pi(st.n_sv);ps(" Task=");pi(st.t_f2);ps("\nSpec: ");pi(st.n_sp);ps(" Env=");pi(st.t_f3);ps("\nCtrl: ");pi(st.n_ct);ps(" Path=");pi(st.t_f4);ps("\nSense: ");pi(st.n_sn);ps(" Lidar=");pi(st.t_f5);ps("\n");}
void rbt_state(void){ps("[RBT] In=");pi(st.n_in);ps(" Sv=");pi(st.n_sv);ps(" Sp=");pi(st.n_sp);ps(" Ct=");pi(st.n_ct);ps(" Sn=");pi(st.n_sn);ps("\n");}
int main(void){
ps("=== Robot Tech Admin Demo ===\n\n");rbt_init();
ps("Industrial robots...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rbt_industrial(t,c,157+(i*17),142+(i*14),122+(i*10),105+(i*6),2020+(i%5));}
ps("\nService robots...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rbt_service(t,c,146+(i*15),132+(i*12),114+(i*8),101+(i*5),2021+(i%4));}
ps("\nSpecial robots...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rbt_special(t,c,138+(i*13),124+(i*10),108+(i*7),97+(i*4),2022+(i%3));}
ps("\nRobot control...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rbt_control(t,c,130+(i*11),118+(i*9),104+(i*6),94+(i*3),2023+(i%2));}
ps("\nRobot sensing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rbt_sensing(t,c,124+(i*9),113+(i*7),100+(i*5),92+(i*3),2024);}
ps("\n");rbt_report();rbt_state();ps("\n=== Demo Complete ===\n");return 0;}
