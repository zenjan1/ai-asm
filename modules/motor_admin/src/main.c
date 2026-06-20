/* motor_admin: Motor technology administration (v1.0)
 * AC motor, DC motor, special motor, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mtr_t;
typedef struct{int n_ac,n_dc,n_sp,n_ax,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mtr_state_t;
static mtr_t acs[N],dcs[N-2],sps[N-4],axs[N-6],mks[N-6]; static mtr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mtr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mtr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MTR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mtr_init(void){if(init)return -1;st.n_ac=0;st.n_dc=0;st.n_sp=0;st.n_ax=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)acs[i].active=0;for(int i=0;i<N-2;i++)dcs[i].active=0;for(int i=0;i<N-4;i++)sps[i].active=0;for(int i=0;i<N-6;i++)axs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[MTR] Motor initialized\n");return 0;}
int mtr_ac(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f1,N,t,c,a,b,d,e,y);}
int mtr_dc(int t,int c,int a,int b,int d,int e,int y){return add(dcs,&st.n_dc,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mtr_special(int t,int c,int a,int b,int d,int e,int y){return add(sps,&st.n_sp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mtr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(axs,&st.n_ax,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mtr_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mtr_report(void){ps("[MTR] Ac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f1);ps("\nDc: ");pi(st.n_dc);ps(" PCS=");pi(st.t_f2);ps("\nSp: ");pi(st.n_sp);ps(" PCS=");pi(st.t_f3);ps("\nAx: ");pi(st.n_ax);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mtr_state(void){ps("[MTR] Ac=");pi(st.n_ac);ps(" Dc=");pi(st.n_dc);ps(" Sp=");pi(st.n_sp);ps(" Ax=");pi(st.n_ax);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Motor Admin Demo ===\n\n");mtr_init();
ps("AC motors...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mtr_ac(t,c,372+(i*17),357+(i*14),337+(i*10),319+(i*6),2020+(i%5));}
ps("\nDC motors...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mtr_dc(t,c,361+(i*15),347+(i*12),329+(i*8),316+(i*5),2021+(i%4));}
ps("\nSpecial motors...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mtr_special(t,c,353+(i*13),339+(i*10),323+(i*7),312+(i*4),2022+(i%3));}
ps("\nMotor accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mtr_accessory(t,c,345+(i*11),333+(i*9),319+(i*6),309+(i*3),2023+(i%2));}
ps("\nMotor marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mtr_market(t,c,339+(i*9),328+(i*7),315+(i*5),307+(i*3),2024);}
ps("\n");mtr_report();mtr_state();ps("\n=== Demo Complete ===\n");return 0;}
