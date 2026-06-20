/* motortech_admin: Motor vehicle technology administration (v1.0)
 * Vehicle manufacturing, engine production, chassis systems, automotive electronics, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mtr_t;
typedef struct{int n_vm,n_eng,n_chs,n_ae,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mtr_state_t;
static mtr_t vms[N],engs[N-2],chss[N-4],aes[N-6],mks[N-6]; static mtr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mtr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mtr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MTR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mtr_init(void){if(init)return -1;st.n_vm=0;st.n_eng=0;st.n_chs=0;st.n_ae=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)vms[i].active=0;for(int i=0;i<N-2;i++)engs[i].active=0;for(int i=0;i<N-4;i++)chss[i].active=0;for(int i=0;i<N-6;i++)aes[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[MTR] Motortech initialized\n");return 0;}
int mtr_vehicle(int t,int c,int a,int b,int d,int e,int y){return add(vms,&st.n_vm,&st.t_f1,N,t,c,a,b,d,e,y);}
int mtr_engine(int t,int c,int a,int b,int d,int e,int y){return add(engs,&st.n_eng,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mtr_chassis(int t,int c,int a,int b,int d,int e,int y){return add(chss,&st.n_chs,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mtr_autoelec(int t,int c,int a,int b,int d,int e,int y){return add(aes,&st.n_ae,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mtr_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mtr_report(void){ps("[MTR] Veh: ");pi(st.n_vm);ps(" PCS=");pi(st.t_f1);ps("\nEng: ");pi(st.n_eng);ps(" PCS=");pi(st.t_f2);ps("\nChs: ");pi(st.n_chs);ps(" PCS=");pi(st.t_f3);ps("\nAe: ");pi(st.n_ae);ps(" PCS=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mtr_state(void){ps("[MTR] Vm=");pi(st.n_vm);ps(" Eng=");pi(st.n_eng);ps(" Chs=");pi(st.n_chs);ps(" Ae=");pi(st.n_ae);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Motor Vehicle Tech Admin Demo ===\n\n");mtr_init();
ps("Vehicle manufacturing...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mtr_vehicle(t,c,217+(i*17),202+(i*14),182+(i*10),164+(i*6),2020+(i%5));}
ps("\nEngine production...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mtr_engine(t,c,206+(i*15),192+(i*12),174+(i*8),161+(i*5),2021+(i%4));}
ps("\nChassis systems...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mtr_chassis(t,c,198+(i*13),184+(i*10),168+(i*7),157+(i*4),2022+(i%3));}
ps("\nAutomotive electronics...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mtr_autoelec(t,c,190+(i*11),178+(i*9),164+(i*6),154+(i*3),2023+(i%2));}
ps("\nMotor marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mtr_market(t,c,184+(i*9),173+(i*7),160+(i*5),152+(i*3),2024);}
ps("\n");mtr_report();mtr_state();ps("\n=== Demo Complete ===\n");return 0;}
