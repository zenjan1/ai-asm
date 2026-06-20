/* measuringtech_admin: Measuring technology administration (v1.0)
 * Length, angle, form/position measurement, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} msr_t;
typedef struct{int n_ln,n_ag,n_fp,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} msr_state_t;
static msr_t lns[N],ags[N-2],fps[N-4],acs[N-6],mks[N-6]; static msr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(msr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;msr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MSR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int msr_init(void){if(init)return -1;st.n_ln=0;st.n_ag=0;st.n_fp=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lns[i].active=0;for(int i=0;i<N-2;i++)ags[i].active=0;for(int i=0;i<N-4;i++)fps[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[MSR] Measuringtech initialized\n");return 0;}
int msr_length(int t,int c,int a,int b,int d,int e,int y){return add(lns,&st.n_ln,&st.t_f1,N,t,c,a,b,d,e,y);}
int msr_angle(int t,int c,int a,int b,int d,int e,int y){return add(ags,&st.n_ag,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int msr_form(int t,int c,int a,int b,int d,int e,int y){return add(fps,&st.n_fp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int msr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int msr_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void msr_report(void){ps("[MSR] Ln: ");pi(st.n_ln);ps(" PCS=");pi(st.t_f1);ps("\nAg: ");pi(st.n_ag);ps(" PCS=");pi(st.t_f2);ps("\nFp: ");pi(st.n_fp);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void msr_state(void){ps("[MSR] Ln=");pi(st.n_ln);ps(" Ag=");pi(st.n_ag);ps(" Fp=");pi(st.n_fp);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Measuring Tech Admin Demo ===\n\n");msr_init();
ps("Length measurement...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;msr_length(t,c,286+(i*17),271+(i*14),251+(i*10),233+(i*6),2020+(i%5));}
ps("\nAngle measurement...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;msr_angle(t,c,275+(i*15),261+(i*12),243+(i*8),230+(i*5),2021+(i%4));}
ps("\nForm/position measurement...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;msr_form(t,c,267+(i*13),253+(i*10),237+(i*7),226+(i*4),2022+(i%3));}
ps("\nMeasuring accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;msr_accessory(t,c,259+(i*11),247+(i*9),233+(i*6),223+(i*3),2023+(i%2));}
ps("\nMeasuring marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;msr_market(t,c,253+(i*9),242+(i*7),229+(i*5),221+(i*3),2024);}
ps("\n");msr_report();msr_state();ps("\n=== Demo Complete ===\n");return 0;}
